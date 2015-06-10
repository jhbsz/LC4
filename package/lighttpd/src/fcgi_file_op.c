#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <utime.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>
#include <ctype.h>
#include "fcgi_memdbg.h"
#include "fcgi_nas_cgiutil.h"
#include "fcgi_file_op.h"
#include "fcgi_download.h"

#ifdef QBOX_SUPPORT
#include "fcgi_qbox.h"
#include "fcgi_qbox_op.h"
extern int qbox_user;
#endif

extern char user_sid[32];

#define CBIT_COPY_BUFF   (15)     // 2 ^ 15 = 32KB
#define CB_COPY_BUFF   (1L << CBIT_COPY_BUFF) // 32 KB
#ifndef O_NOATIME 
#define O_NOATIME 0 
#endif

//TODO: Move these definition to naslog
#define DESC_LENGTH 1024
#define SUCCESS     0

int _find_file_name(char *dest_path, char *file_name, char *dup);

int usb_file_system = -1; //FS_UNKNOWN;
unsigned long source_file_count=0;
unsigned long source_total_file_count=0;
unsigned long source_dir_count=0;
unsigned long source_total_dir_count=0;
unsigned long source_copyed_count=0;
unsigned long long source_size=0;
unsigned long long source_total_size=0;
float   percent = 0;
float   new_percent = 0;
float   copy_last=0;
pid_t   gPid=0;
char    thiscopypid[256]={0};
extern char remote_user[MAX_USER_LEN];
//extern session a_session;
//extern HTTP_INFO       http_info;
VETO_INFO *veto_files=NULL;
int veto_file_count=0;
unsigned long long gWFM2_FolderCnt=0;
unsigned long long gWFM2_FileCnt=0;
char    gSrcPath[BUF256+1]={0};
char    gDestPath[BUF256+1]={0};

void tran_path(char *path)
{
	int i;
	for(i=0; i< strlen(path); i++) {
		if((path[i] == '/') || (path[i] == ' ') || (path[i] == '\''))
			path[i] = '_';
	}
}

/*
 *  Desc: Check file is exist
 *  @return: 1 for success
 */
/*
static int file_is_exist(const char *filename)
{
    struct stat status;

    if (0 == stat(filename, &status))
        return 1;
    return 0;
}
*/
static int does_file_exist(const char *filename)
{
    struct stat status;

    return ( 0 == stat(filename, &status) );
}

int _get_share_relative_name(char *aFullPathName, char *aSharePath, int aShareLen)
{
    char *pShare=NULL, *p=NULL;
    char sSep[]="/";
    int  nLen=0;
    pShare=__ax_strdup(aFullPathName);
    if(!strncmp(aFullPathName, ROOT_PATH, strlen(ROOT_PATH))) {
        if((p=strtok(pShare, sSep)) != NULL) {
            nLen += (strlen(p)+1);
            if((p=strtok(NULL, sSep)) != NULL) {
                nLen += (strlen(p)+1);
                strcpy(aSharePath, &aFullPathName[nLen]);
                if(pShare) __ax_free(pShare);
                return 0;
            }
        }
    }

    strncpy(aSharePath, aFullPathName, aShareLen);
    if(pShare) __ax_free(pShare);

    return 1;
}

static int _move_file(const char *src, const char *dest, int option)
{
    int ret = SUCCESS;
    struct stat src_status;
    char    *extend_src = NULL, *extend_dest= NULL;

    extend_src = (char *)__ax_calloc(1, strlen(src)*2 + 1);
    extend_dest = (char *)__ax_calloc(1, strlen(dest)*2 + 1);

    stat(src, &src_status);

    transfer_special_char((char *)src, extend_src);
    transfer_special_char((char *)dest, extend_dest);
    if ( does_file_exist(dest) ) {
        if (option == WFM2_SKIP) {
            /*
               int src_mode, dest_mode;
               if (stat(dest, &dest_status)>=0)
               {
               src_mode  = src_status.st_mode  & S_IFMT;
               dest_mode = dest_status.st_mode & S_IFMT;
               if ( (dest_mode     == src_mode    ) &&
               (dest_status.st_mtime == src_status.st_mtime) &&
               (dest_status.st_size  == src_status.st_size ) ) {
               return SUCCESS;
               }
               }
             */
            return SUCCESS;
        }
        ret = rename(src, dest);
        if(ret) {
            ret = _copy_file(src, dest, WFM2_OVERWRITE);
            if(ret != SUCCESS) {
                FCGI_LOG("fail to copy file from %s to %s", src, dest);
            }
            else {
                unlink(src);
            }
        }
    }else{
        ret = rename(src, dest);
        if(ret) {
            ret = _copy_file(src, dest, WFM2_OVERWRITE);
            if(ret != SUCCESS) {
                FCGI_LOG("fail to copy file from %s to %s", src, dest);
            }
            else {
                unlink(src);
            }
        }
    }
    /*
    p_src=__ax_strdup(src);
       if(wfm2_check_video_mime_type(p_src) > 0) {
       p_dest=__ax_strdup(dest);
    //_clean_video(p_s, WFM_EMEDIA_ALL, 0);
    _move_video(p_src, p_dest, WFM_EMEDIA_ALL);
    if (p_dest) __ax_free(p_dest);
    }
    if (p_src) __ax_free(p_src);
     */

#ifdef QBOX_SUPPORT
    if(qbox_user == 1){
        p_dest=__ax_strdup(dest);

        WFM_Dup_Posix_ACL(dirname(p_dest), dest);
        if (p_dest)
            __ax_free(p_dest);
    }
#endif

    if (extend_src) __ax_free(extend_src);
    if (extend_dest) __ax_free(extend_dest);

    return ret;
}

int _move_dir(char *dest, char *source, char *file, int mode)
{
    DIR *destdir, *sourcedir;
    char    *dest_path=NULL, *source_path=NULL;
    int ret = SUCCESS;
    struct dirent   *entry;
    struct stat     statbuf;
    char    *parent_t=NULL, *path=NULL;
    char *path1=NULL, *src=NULL, *pSharePath=NULL;
    int exists=0, nPathLen=0, status=0;
    struct utimbuf  tbuf;

    dest_path = (char *)__ax_calloc(1, strlen(dest) + strlen(file) + 2);
    source_path = (char *)__ax_calloc(1, strlen(source) + strlen(file) + 2);

    strcpy(dest_path, dest);
    strcat(dest_path, "/");
    strcat(dest_path, file);

    strcpy(source_path, source);
    strcat(source_path, "/");
    strcat(source_path, file);

    // FCGI_LOG("full_source = %s, full_dest = %s\n", source_path, dest_path);

    if ((destdir = opendir(dest_path)) == NULL) {
        mkdir(dest_path,S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH);
        status = WFM2_FILE_NO_EXIST;
    }
    else {
        status = WFM2_FILE_EXIST;
        closedir(destdir);
    }
    stat(source_path, &statbuf);
    chmod(dest_path, statbuf.st_mode);

    if((sourcedir=opendir(source_path))==NULL){
        FCGI_LOG("cannot open directory = %s\n", source_path);
        ret = -1; //E_DEST; // FILE SYSTEM ERROR
        return ret;
    }

    FCGI_LOG("_move_dir:1. dest = %s, src = %s, file = %s, mode = %d\n", dest, source, file, mode);

    chdir(source_path);

    while((entry=readdir(sourcedir))!=NULL){
        exists = stat(entry->d_name, &statbuf);
        if(exists>=0 && S_ISDIR(statbuf.st_mode)){
            if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0)
                continue;
            parent_t = __ax_calloc(1, strlen(source_path)+ strlen(entry->d_name)+ 64);
            path = __ax_calloc(1, strlen(dest_path)+ strlen(entry->d_name)+ 64);
            strcpy(parent_t, source_path);
            strcpy(path, dest_path);
            mkdir(path,S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH);
            FCGI_LOG("_move_dir:2. source = %s, dest = %s, file = %s, mode = %d\n", parent_t, path, entry->d_name, mode);
            ret = _move_dir(path, parent_t, entry->d_name, mode);
            if (SUCCESS != ret) {
                FCGI_LOG("_move_dir:%s = %d\n", path, ret);
                if(parent_t) __ax_free(parent_t);
                if(path) __ax_free(path);
                break;
            }

            //delet empty dir
            FCGI_LOG("mode = %d, status = %d, dir = %s/%s\n", mode, status, parent_t, entry->d_name);
            if (mode == WFM2_OVERWRITE || status == WFM2_FILE_NO_EXIST) {
                char  *tmp = NULL;
                tmp = (char *)__ax_calloc(1, strlen(parent_t) + strlen(entry->d_name) + 2);
                sprintf(tmp, "%s/%s", parent_t, entry->d_name);
                remove(tmp);
                FCGI_LOG("_move_dir:1. delete dir = %s\n", tmp);
                if (tmp) __ax_free(tmp);
            }

            chmod(path, statbuf.st_mode);
            tbuf.actime = statbuf.st_atime;
            tbuf.modtime = statbuf.st_mtime;
            utime(path, &tbuf);
            /*if(usb_file_system == FS_EXT3 || usb_file_system == FS_EXT4){
                chown(path, statbuf.st_uid, statbuf.st_gid);
            }*/
            if(parent_t) __ax_free(parent_t);
            if(path) __ax_free(path);
        }else if(exists>=0 && (!S_ISLNK(statbuf.st_mode))){
            src = __ax_calloc(1, strlen(source_path)+ strlen(entry->d_name)+ 64);
            path1 = __ax_calloc(1, strlen(dest_path)+ strlen(entry->d_name)+ 64);
            sprintf(src,"%s/%s", source_path ,entry->d_name);
            sprintf(path1,"%s/%s", dest_path, entry->d_name);
            nPathLen=strlen(path1);
            pSharePath=__ax_calloc(1, nPathLen+1);
            _get_share_relative_name(path1, pSharePath, nPathLen);
            wfm_daemon_write_report_file(pSharePath, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, (int)(percent), 1, thiscopypid);

            ret = _move_file(src, path1, mode);
            if (SUCCESS != ret) {
/*
                nRet = wfm_check_volume(gSrcPath);
                if(nRet < 0) {
                    if(nRet < -100) {
                        //Set_Private_Profile_Integer("", COPY_STATUS, WFM2_FILE_NO_EXIST, thiscopypid);
                    }
                    else {
                        //Set_Private_Profile_Integer("", COPY_STATUS, WFM2_SRC_VOLUME_ERROR, thiscopypid);
                    }
                }
                else if(wfm_check_volume(gDestPath)) {
                    if(nRet < -100) {
                        //Set_Private_Profile_Integer("", COPY_STATUS, WFM2_FILE_NO_EXIST, thiscopypid);
                    }
                    else {
                        //Set_Private_Profile_Integer("", COPY_STATUS, WFM2_DES_VOLUME_ERROR, thiscopypid);
                    }
                }
*/
                Status_To_JSON(WFM2_FAIL);
                FCGI_LOG("_move_file:%s = %d\n", src, ret);
                if(pSharePath) __ax_free(pSharePath);
                if(path1) __ax_free(path1);
                if(src) __ax_free(src);
                break;
            }else{
                source_copyed_count++;
                chmod(path1, statbuf.st_mode);
                tbuf.actime = statbuf.st_atime;
                tbuf.modtime = statbuf.st_mtime;
                utime(path1, &tbuf);
                /*if(usb_file_system == FS_EXT3 || usb_file_system == FS_EXT4){
                    chown(path1, statbuf.st_uid, statbuf.st_gid);
                }*/
                source_size -= (unsigned long long)statbuf.st_size;
                percent = 100. * (source_total_size-source_size)/source_total_size;
                if((percent-copy_last)>0){
                    copy_last=percent;
                    if (percent == 100) {
                        wfm_daemon_write_report_file(pSharePath, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, 99, 1, thiscopypid);

                    }
                    else {
                        wfm_daemon_write_report_file(pSharePath, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, (int)(percent), 1, thiscopypid);
                    }
                }
            }
            if(pSharePath) __ax_free(pSharePath);
            if(path1) __ax_free(path1);
            if(src) __ax_free(src);
        } /*else if(exists>=0 && (S_ISLNK(statbuf.st_mode))){
            char *symlink_val=NULL;

            src = __ax_calloc(1, strlen(source_path)+ strlen(entry->d_name)+ 64);
            path1 = __ax_calloc(1, strlen(dest_path)+ strlen(entry->d_name)+ 64);
            sprintf(src,"%s/%s", source_path, entry->d_name);
            sprintf(path1,"%s/%s", dest_path, entry->d_name);

            unlink (path1);
            symlink_val = xreadlink(src);
            if (symlink (symlink_val, path1) == 0){
                chmod(path1, statbuf.st_mode);
                tbuf.actime = statbuf.st_atime;
                tbuf.modtime = statbuf.st_mtime;
                utime(path1, &tbuf);
                if(usb_file_system == FS_EXT3 || usb_file_system == FS_EXT4){
                    chown(path1, statbuf.st_uid, statbuf.st_gid);
                }
            }
            if(symlink_val) __ax_free(symlink_val);
            if(path1) __ax_free(path1);
            if(src) __ax_free(src);
        }*/
        if (exists < 0) {
            FCGI_LOG("Couldn't stat %s\n", entry->d_name);
        }
    }

    chdir("..");
    closedir(sourcedir);
    FCGI_LOG("Close dir %s\n", source_path);

    if (dest_path) __ax_free(dest_path);
    if (source_path) __ax_free(source_path);

    return ret;
}

int op_get_thumbnail(INPUT *input)
{
    INPUT *tmp = NULL;
    char *path = NULL, *name = NULL, *size = NULL, *path_uc = NULL, *name_uc = NULL;
    char *full_path = NULL, *full_path_uc = NULL, *tmp_path = NULL, *thumbnail_name = NULL, *full_thumbnail_path = NULL, *tmp_thumbnail_path = NULL, *full_thumbnail_path_uc = NULL;
    char f_size[256] = {0}, tmp_file[10] = {0};
    int exist = 0, nLenPath=0;
    int file_size, i;
    long long offset = 0;
    struct stat st;

    nLenPath = strlen(path);
    if( (tmp=CGI_Find_Parameter(input, "path")) ) {
        path = tmp->val;
        path_uc = tmp->val_uc;
        nLenPath = strlen(path);
    }
//FCGI_LOG("path_uc:%s", path_uc);
    nLenPath = strlen(path);

    if( (tmp=CGI_Find_Parameter(input, "name")) ) {
        name = tmp->val;
        name_uc = tmp->val_uc;
    }
//FCGI_LOG("name_uc:%s", name_uc);

    for(i=0; i<4 ; i++) {
        tmp_file[i] = tolower(name[strlen(name) - 4 + i]);
    }
//FCGI_LOG("tmp_file:%s", tmp_file);

    if( (tmp=CGI_Find_Parameter(input, "size")) ) {
        size = tmp->val;
    }
    else 
        return 0;
    /* filter */
    if(strcmp(tmp_file, ".jpg") && strcmp(tmp_file, ".gif") && strcmp(tmp_file, ".png") && strcmp(tmp_file, ".bmp") 
        && strcmp(tmp_file, ".ico") && strcmp(tmp_file, ".xbm") && strcmp(tmp_file, ".cur") && strcmp(tmp_file, "jpeg")) 
        return 0;

    tmp_path = (char *)__ax_calloc(1, strlen(path) + strlen(name) + 2);
    strcpy(tmp_path, path);
    strcat(tmp_path, "/");
    strcat(tmp_path, name);
//FCGI_LOG("tmp_path:%s", tmp_path);

    full_path = (char *)__ax_calloc(1, strlen(ROOT_PATH) + strlen(tmp_path) + 10);
    exist = check_exist(tmp_path, full_path);
//FCGI_LOG("full_path:%s", full_path);

    if(exist < 0) {
        Status_To_JSON(WFM2_FILE_NO_EXIST);
        if(tmp_path != NULL) __ax_free(tmp_path);
        if(full_path != NULL) __ax_free(full_path);
        return -1;
    }
    stat(full_path, &st);
    file_size = st.st_size;
//FCGI_LOG("size:%d", file_size);

    thumbnail_name = (char *)__ax_calloc(1, strlen(path) + strlen(name) + 100);
    strcpy(thumbnail_name, tmp_path);
    tran_path(thumbnail_name);
    sprintf(f_size, "%d", file_size);
    strcat(thumbnail_name, f_size);
    strcat(thumbnail_name, "_");
    strcat(thumbnail_name, size);
//FCGI_LOG("thumbnail_name:%s", thumbnail_name);
   
    tmp_thumbnail_path = (char *)__ax_calloc(1, strlen("/Qsync/.thumbnails/") + strlen(thumbnail_name) + 10);
    full_thumbnail_path = (char *)__ax_calloc(1, strlen(ROOT_PATH) + strlen("/Qsync/.thumbnails/") + strlen(thumbnail_name) + 10);
    sprintf(tmp_thumbnail_path, "/Qsync/.thumbnails/%s", thumbnail_name);
//FCGI_LOG("tmp_thumbnail_path:%s", tmp_thumbnail_path);
    exist = check_exist(tmp_thumbnail_path, full_thumbnail_path);
//FCGI_LOG("full_thumbnail_path:%s", full_thumbnail_path);

    if(exist < 0) {
        full_path_uc = (char *)__ax_calloc(1, strlen(ROOT_PATH) + strlen(path_uc) + strlen(name_uc) + 10);
        sprintf(full_path_uc, ROOT_PATH"%s/%s", path_uc, name_uc);
//FCGI_LOG("full_path_uc:%s", full_path_uc);
        _download_urlenc(full_path, name, full_path_uc, 0, 0, offset);
    }
    else {
        full_thumbnail_path_uc = (char *)__ax_calloc(1, strlen(ROOT_PATH) + strlen("%%2FQsync%%2F.thumbnails/") + strlen(thumbnail_name) + 10);
        sprintf(full_thumbnail_path_uc, "%s%%2FQsync%%2F.thumbnails/%s", ROOT_PATH, thumbnail_name);
//FCGI_LOG("full_thumbnail_path_uc:%s", full_thumbnail_path_uc);
        _download_urlenc(full_thumbnail_path, thumbnail_name, full_thumbnail_path_uc, 0, 0, offset);
    }
 
    if(full_path != NULL) __ax_free(full_path);
    if(full_thumbnail_path_uc != NULL) __ax_free(full_thumbnail_path_uc);
    if(full_path_uc != NULL) __ax_free(full_path_uc);
    if(tmp_path != NULL) __ax_free(tmp_path);
    if(thumbnail_name != NULL) __ax_free(thumbnail_name);
    if(full_thumbnail_path != NULL) __ax_free(full_thumbnail_path);
    if(tmp_thumbnail_path != NULL) __ax_free(tmp_thumbnail_path);

    return 0;
}

int op_move(INPUT *input)
{
    INPUT   *tmp;
    char    *source_path = NULL, *source_path_tmp = NULL, *full_source_path = NULL, *sid = NULL;
    char    *dest_path = NULL, *full_dest_path = NULL;
    char    *full_path = NULL, *last_file = NULL, msg[MAX_CONN_LOG_MSG_SIZE+1]={0};
    char    *source_file = NULL;
    int     exist = 0, mode = 0, source_total = 0, i=0;
    char    *p_path=NULL, *p_full_path=NULL;
    int     nLenSrcPath=0, nLenSrcFile=0;
    struct stat statbuf;
    pid_t pid;
    int nLenDestPath=0;

    /* guest deny */
    if( (tmp=CGI_Find_Parameter(input, "sid")) ) {
        sid = tmp->val;
        if(!strcmp(sid, GUEST_SID)) {
            Status_To_JSON(WFM2_PERMISSION_DENY);
            return -1;
        }
    }

#ifdef QBOX_SUPPORT
    if ((qbox_user == 1) && (op_qbox_check_file_profile(input) != 0)){
        return -1;
    }
#endif

    if( (tmp=CGI_Find_Parameter(input, "source_path")) ) {
        source_path = tmp->val;
        strncpy(gSrcPath, source_path, BUF256);
        nLenSrcPath=strlen(source_path);
        if(!Check_Illegal_Path(source_path)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }
    } 
    else return -1;

    source_path_tmp = (char *)__ax_calloc(1, nLenSrcPath + 1);
    strcpy(source_path_tmp, source_path);
/*
    if(check_user_read_write_privilege(remote_user, source_path_tmp) < 0){
        if(source_path_tmp)
            __ax_free(source_path_tmp);
        return -1;
    }
*/
    if(source_path_tmp) __ax_free(source_path_tmp);

    if( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
        source_file = tmp->val;
        nLenSrcFile=strlen(source_file);
        if(!Check_Illegal_Path(source_file)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }
    }
    else return -1;

    if( (tmp=CGI_Find_Parameter(input, "dest_path")) ) {
        dest_path = tmp->val;
        nLenDestPath = strlen(dest_path);
        if(dest_path[nLenDestPath-1] == '/') {
            dest_path[nLenDestPath-1] = '\0';
            nLenDestPath -= 1;
        }
        strncpy(gDestPath, dest_path, BUF256);
        if(!Check_Illegal_Path(dest_path)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }

        // check moving folder to the same source path
        p_path = (char *)__ax_calloc(1, nLenSrcPath + nLenSrcFile + 2);
        p_full_path = (char *)__ax_calloc(1, nLenSrcPath + nLenSrcFile + PATH_PREFIX_LENGTH + 256);
        sprintf(p_path, "%s/%s", source_path, source_file);
        if((exist = check_exist(p_path, p_full_path)) == 1) { // folder
            if((!strncmp(p_path, dest_path, strlen(p_path))) && (nLenDestPath == strlen(p_path))) {
                Status_To_JSON(WFM2_ILLEGAL_NAME);
                if(p_path != NULL) __ax_free(p_path);
                if(p_full_path != NULL) __ax_free(p_full_path);
                return -1;
            }
        }
        else if (exist < 0) {
            Status_To_JSON(WFM2_SRC_FILE_NO_EXIST);
            if(p_path != NULL) __ax_free(p_path);
            if(p_full_path != NULL) __ax_free(p_full_path);
            return -1;
        }

        if(p_path != NULL) __ax_free(p_path);
        if(p_full_path != NULL) __ax_free(p_full_path);
    }
    else return -1;

/*
    dest_path_tmp = (char *)__ax_calloc(1, strlen(dest_path) + 1);

    strcpy(dest_path_tmp, dest_path);

    if(check_user_read_write_privilege(remote_user, dest_path_tmp) < 0){
        if(dest_path_tmp)
            __ax_free(dest_path_tmp);
        return -1;
    }

    if(dest_path_tmp)
        __ax_free(dest_path_tmp);
*/

    full_source_path = (char *)__ax_calloc(1, nLenSrcPath + MAX_FILE_PATH + 15);
    exist = check_exist(source_path, full_source_path);
    if(exist < 0 || exist != 1) {
        Status_To_JSON(WFM2_SRC_FILE_NO_EXIST);
        if (full_source_path) __ax_free(full_source_path);
        return -1;
    }
/*
    if(check_qbox_write_permission(remote_user, full_source_path) != 0){
        Status_To_JSON(WFM2_PERMISSION_DENY);
        if (full_source_path) __ax_free(full_source_path);
        return -1;
    }
*/
    full_dest_path = (char *)__ax_calloc(1, strlen(dest_path) + MAX_FILE_PATH + 15);
    exist = check_exist(dest_path, full_dest_path);
    if(exist < 0 || exist != 1) {
        Status_To_JSON(WFM2_DES_FILE_NO_EXIST);
        if (full_dest_path) __ax_free(full_dest_path);
        return -1;
    }

/*
    if(check_qbox_write_permission(remote_user, full_dest_path) != 0){
        Status_To_JSON(WFM2_PERMISSION_DENY);
        if (full_dest_path) __ax_free(full_dest_path);
        return -1;
    }
*/
    
    if( (tmp=CGI_Find_Parameter(input, "mode")) )
        mode = atoi(tmp->val);
    if( (tmp=CGI_Find_Parameter(input, "source_total")) )
        source_total = atoi(tmp->val);

    pid = fork();
    if (pid < 0) {
        FCGI_LOG("%s[%d] create thread failed, reason: %s\n", __func__, __LINE__, strerror(errno));
        exit(0);
    }

    if (pid==0){
        close(0);
        close(1);
#ifndef __DEBUG__
        close(2);
#endif
        struct utimbuf tbuf;
        char    qsync_log_desc[DESC_LENGTH];
        int ret=0;

        wfm_daemon_create_report_file("admin", thiscopypid);
        wfm_daemon_write_report_file(NULL, 0, 0, 0, 0, 0, thiscopypid);
//        wfm_daemon_write_report_file(NULL, 0, 0, 0, -1, 0, thiscopypid);
        Set_Private_Profile_Integer("", COPY_STATUS, WFM2_DONE, thiscopypid);
#ifdef COPY_MOVE_LIST
        _daemon_update(0, source_path, dest_path, source_file, WFM_EDL_TMOVE, 0, -1);
#endif
        //calculate the total size of the sources first
        if ( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
            for (i=0; i<source_total; i++) {
                full_path = (char *)__ax_calloc(1, strlen(full_source_path) + MAX_FILE_LENGTH + 2);
                if (full_path == NULL) continue;
                strcpy(full_path, full_source_path);
                strcat(full_path, "/");
                strcat(full_path, tmp->val);
                Cal_Size_Ex(full_path , &source_total_file_count, &source_total_dir_count, &source_total_size, thiscopypid);
                tmp = tmp->next;
                if(full_path) __ax_free(full_path);
            }
            source_file_count = source_total_file_count;
            source_dir_count = source_total_dir_count;
            source_size = source_total_size;
#ifdef COPY_MOVE_LIST
            _daemon_update(1, source_path, dest_path, source_file, WFM_EDL_TMOVE, 1, 0);
#endif
        }

        if(source_total_size == 0) {
            source_total_size = source_file_count + source_dir_count;
            source_size = source_total_size;
        }

        if ( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
            while (tmp !=NULL && source_total != 0){
                char *dest_path_tmp = NULL, *file_name = NULL;
                struct stat stattmp;

                full_path = (char *)__ax_calloc(1, strlen(full_source_path) + MAX_FILE_LENGTH + 1);
                strcpy(full_path, full_source_path);
                strcat(full_path, "/");
                strcat(full_path, tmp->val);
                stat(full_path, &statbuf);

                dest_path_tmp = (char *)__ax_calloc(1, strlen(full_dest_path) + MAX_FILE_LENGTH + 1);
                strcpy(dest_path_tmp, full_dest_path);
                strcat(dest_path_tmp, "/");
                // FCGI_LOG("file_name : %s", file_name);
                //filename include "/"
                if (strstr(tmp->val, "/") != 0) {
                    file_name = strrchr(tmp->val, '/');
                    strcat(dest_path_tmp, file_name);
                }
                else{
                    file_name = tmp->val;
                    strcat(dest_path_tmp, tmp->val);
                }
                // FCGI_LOG("dest_path_tmp : %s", dest_path_tmp);

                if (S_ISDIR(statbuf.st_mode)) {
                    // Handle Directory 
                    ret = 0;
                    FCGI_LOG("_move_dir %s", full_path);
                    snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "%s/%s to %s/%s", source_path, file_name, dest_path, file_name);
                    //SendConnToLogEngine(EVENT_TYPE_INFO, remote_user, http_info.remote_addr, "---", CONN_SERV_HTTP, CONN_ACTION_MOVE,  msg);
                    snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Moved %s/%s to %s/%s", source_path, file_name, dest_path, file_name);

                    FCGI_LOG("log_Desc %s", qsync_log_desc);
                    if(stat(dest_path_tmp, &stattmp) >= 0) { // check the existing of destination same folder name
                        ///TODO: Move directory
                        ret = _move_dir(full_dest_path, full_source_path, tmp->val, mode);
                    } 
                    else 
                    if(strncmp(full_path, dest_path_tmp, SHARE_DATA_PATH_LEN) == 0) { // check the same volume
                        char    *extend_src = NULL, *extend_dest= NULL;

                        transfer_special_char((char *)full_path, extend_src);
                        transfer_special_char((char *)dest_path_tmp, extend_dest);

                        ret = rename(full_path, dest_path_tmp);
                        if(ret) {
                            ret = _copy_file(full_path, dest_path_tmp, WFM2_OVERWRITE);
                            if(ret != SUCCESS) {
                                FCGI_LOG("fail to copy file from %s to %s", full_path, dest_path_tmp);
                            }
                            else {
                                unlink(extend_src);
                            }
                        }

                        source_copyed_count++;
                        new_percent = (100. * (source_copyed_count)/source_total_file_count);
                        if(new_percent > percent)
                            percent = new_percent;
                        
                        wfm_daemon_write_report_file(tmp->val, source_copyed_count, source_total_file_count, (unsigned long long)source_total_size, (int)(percent), 1, thiscopypid);

                    } else {
                        ///TODO: Move Directory
                        ret = _move_dir(full_dest_path, full_source_path, tmp->val, mode);
                    }
/*
                    add_wfm2_file_op_log(dest_path_tmp, full_path, WFM2_EVENT_TYPE_INFO, 1, WFM2_EVENT_ACTION_MOVE, qsync_log_desc);
                    if (ret != SUCCESS) {
                        last_file = tmp->val;
                        snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "[Move fail] %s/%s to %s/%s", source_path, tmp->val, dest_path, tmp->val);
                        SendConnToLogEngine(EVENT_TYPE_ERROR, remote_user, http_info.remote_addr,
                                "---", CONN_SERV_HTTP, CONN_ACTION_MOVE,  msg);
                        snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Moved %s/%s to %s/%s failed", source_path, tmp->val, dest_path, tmp->  val);
                        add_wfm2_file_op_log(dest_path_tmp, full_path, WFM2_EVENT_TYPE_WARN, 1, WFM2_EVENT_ACTION_MOVE, qsync_log_desc);
                    }
                    else {
#ifdef COPY_MOVE_LIST
                        _daemon_update(1, source_path, dest_path, tmp->val, WFM_EDL_TMOVE, 1, percent);
#endif
                    }
*/
                    remove(full_path);
                    FCGI_LOG("_move_dir:2. delete dir = %s\n", full_path);
                } else if(!S_ISLNK(statbuf.st_mode)) {
                    ret = 0;
                    wfm_daemon_write_report_file(tmp->val, source_copyed_count, source_total_file_count, (unsigned long long)source_total_size, (int)(percent), 1, thiscopypid);
                    FCGI_LOG( "_move_file: source = %s, dest = %s, mode = %d\n", full_path, dest_path_tmp, mode);
                    snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "%s/%s to %s/%s", source_path, tmp->val, dest_path, tmp->val);
                    //SendConnToLogEngine(EVENT_TYPE_INFO, remote_user, http_info.remote_addr,
                    //        "---", CONN_SERV_HTTP, CONN_ACTION_MOVE,  msg);
                    snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Moved %s/%s to %s/%s", source_path, tmp->val, dest_path, tmp->val);
                    ret = _move_file(full_path, dest_path_tmp, mode);
                    //add_wfm2_file_op_log(dest_path_tmp, full_path ,WFM2_EVENT_TYPE_INFO, 0, WFM2_EVENT_ACTION_MOVE, qsync_log_desc);
                    if (SUCCESS != ret) {
                        ///TODO: Write log file 
                        /*
                        last_file = tmp->val;
                        FCGI_LOG("_move_file:%s = %d\n", full_path, ret);
                        snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "[Move fail] %s/%s to %s/%s", source_path, tmp->val, dest_path, tmp->val);
                        SendConnToLogEngine(EVENT_TYPE_ERROR, remote_user, http_info.remote_addr,
                                "---", CONN_SERV_HTTP, CONN_ACTION_MOVE,  msg);
                        snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Moved %s/%s to %s/%s failed", source_path, tmp->val, dest_path, tmp->  val);
                        add_wfm2_file_op_log(dest_path_tmp, full_path, WFM2_EVENT_TYPE_WARN, 0, WFM2_EVENT_ACTION_MOVE, qsync_log_desc);
                        nRet=0;
                        nRet = wfm_check_volume(gSrcPath);
                        if(nRet < 0) {
                            if(nRet < -100) {
                                Set_Private_Profile_Integer("", COPY_STATUS, WFM2_FILE_NO_EXIST, thiscopypid);
                                Status_To_JSON(WFM2_FILE_NO_EXIST);
                            }
                            else {
                                Set_Private_Profile_Integer("", COPY_STATUS, WFM2_SRC_VOLUME_ERROR, thiscopypid);
                                Status_To_JSON(WFM2_SRC_VOLUME_ERROR);
                            }
                        }
                        else if(wfm_check_volume(gDestPath)) {
                            if(nRet < -100) {
                                Set_Private_Profile_Integer("", COPY_STATUS, WFM2_FILE_NO_EXIST, thiscopypid);
                                Status_To_JSON(WFM2_FILE_NO_EXIST);
                            }
                            else {
                                Set_Private_Profile_Integer("", COPY_STATUS, WFM2_DES_VOLUME_ERROR, thiscopypid);
                                Status_To_JSON(WFM2_SRC_VOLUME_ERROR);
                            }
                        }
                        //Status_To_JSON(WFM2_FAIL);
                        if (dest_path_tmp) __ax_free(dest_path_tmp);
                        if (full_path) __ax_free(full_path);
                        break;
                        */
                    }
                    else {
                        source_copyed_count++;
                        chmod(dest_path_tmp, statbuf.st_mode);
                        tbuf.actime = statbuf.st_atime;
                        tbuf.modtime = statbuf.st_mtime;
                        utime(dest_path_tmp, &tbuf);
/*                        if(usb_file_system == FS_EXT3 || usb_file_system == FS_EXT4){
                            chown(dest_path_tmp, statbuf.st_uid, statbuf.st_gid);
                        }*/
                        source_size -= (unsigned long long)statbuf.st_size;
                        percent = 100. * (source_total_size-source_size)/source_total_size;
                        if((percent-copy_last)>0){
                            copy_last=percent;
                            if (percent == 100) {
                                wfm_daemon_write_report_file(tmp->val, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, 99, 1, thiscopypid);
                            }
                            else {
                                wfm_daemon_write_report_file(tmp->val, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, (int)(percent), 1, thiscopypid);
                            }
                        }
#ifdef COPY_MOVE_LIST
                        _daemon_update(1, source_path, dest_path, tmp->val, WFM_EDL_TMOVE, 1, percent);
#endif
                        
                    }
                } 
                
                last_file = tmp->val;
                source_total--;
                tmp = tmp->next;
                if (dest_path_tmp) __ax_free(dest_path_tmp);
                if (full_path) __ax_free(full_path);
            }
            //wfm_daemon_list_delete(gPid);
        }
        if (full_source_path) __ax_free(full_source_path);
        if (full_dest_path) __ax_free(full_dest_path);
#ifdef COPY_MOVE_LIST
        if(ret != SUCCESS) {
            _daemon_update(1, source_path, dest_path, last_file, WFM_EDL_TMOVE, 4, percent);
        }
        else {
            wfm_daemon_write_report_file(last_file, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, 100, 1,   thiscopypid);
            _daemon_update(1, source_path, dest_path, last_file, WFM_EDL_TMOVE, 2, 100);
        }
#endif
    wfm_daemon_write_report_file(last_file, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, 100, 1,   thiscopypid);
    }

    Status_With_Pid_To_JSON(WFM2_SUCCESS, pid);
    return 0;
}

int op_createdir(INPUT *input)
{
    INPUT   *tmp = NULL;
    char    *path = NULL, *dest_path = NULL, *folder = NULL;
    int     exist = 0, nRet=0, ret=0;
    DIR     *dir;

    if( (tmp=CGI_Find_Parameter(input, "dest_path")) ) {
        path = tmp->val;
        if(!Check_Illegal_Path(path)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }
    }
    else return -1;

    dest_path = (char *)__ax_calloc(1, strlen(path) + MAX_FILE_LENGTH + 15);
    exist = check_exist(path, dest_path);
    if (exist < 0) {
        Status_To_JSON(WFM2_FILE_NO_EXIST);
        if (dest_path) __ax_free(dest_path);
        return -1;
    }

    if ( (tmp=CGI_Find_Parameter(input, "dest_folder")) ) {
        folder = tmp->val;
        if(!Check_Illegal_Path(folder)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }
    }
    if(!strcmp(folder, "`"))
        strcat(dest_path, "/\\");
    else
        strcat(dest_path, "/");
    strcat(dest_path, folder);

    dir = opendir(dest_path);
    if (dir != NULL) {
        Status_To_JSON(WFM2_FILE_EXIST);
        closedir(dir);
        if (dest_path) free(dest_path);
        return -1;
    }

    nRet=mkdir(dest_path, 0770);
    if(nRet<0) {
        Status_To_JSON(WFM2_QUOTA_ERROR);
        return 0;
    }

    if (ret == 0)
        Status_To_JSON(WFM2_SUCCESS);
    else
        Status_To_JSON(WFM2_FAIL);

    if (dest_path) __ax_free(dest_path);

    return 0;
}

int _sys_delete(char *filename)
{

	struct dirent   *_dirent = NULL;
	struct stat     statbuf;
	char	    *file_basename = NULL;//*fullpath=NULL
	DIR	     *dir = NULL;
    char *path = NULL, fullpath[MAX_FILE_PATH];
	int		ret;
	int veto_hidden = 0, i, file_count = 0;
    //FCGI_LOG("sys_del_int %s","sys_start");
	if (lstat(filename, &statbuf)<0) {
		return -1;
    //FCGI_LOG("lstat  %s","fail");
	}

	//check file is in home or not
	/*path = __ax_calloc(1, strlen(filename) + 1);
	if(strncmp(filename, USER_HOME_PREFIX, 12) == 0){
		sprintf(path, "/home/%s", filename + 13);
	}else{
		sprintf(path, "/home/%s", filename + PATH_PREFIX_LENGTH);
	}*/

	if (S_ISDIR(statbuf.st_mode)) {//del dir
        //FCGI_LOG("S_ISDIR %s","ISDIR");
		if ((dir=opendir(filename)) == NULL){
          //  FCGI_LOG("dir %s","dir_fail");
			if(path) __ax_free(path);
			return -1;
		}
        //FCGI_LOG("dir = %s",dir);
		while ((_dirent=readdir(dir))) {
            //FCGI_LOG("dirent->d_name0 %s", _dirent->d_name);
			if (!strcmp(_dirent->d_name, ".") || !strcmp(_dirent->d_name, "..")){//null dir
			//FCGI_LOG("dirent->d_name1 %s", _dirent->d_name);
                continue;
            }
            sprintf(fullpath, "%s/%s", filename, _dirent->d_name);
			//FCGI_LOG("dirent->d_name1 %s", fullpath);
            //fixed for transcode link file 
			if (lstat(fullpath, &statbuf)<0){
                continue;
            }
			if (S_ISDIR(statbuf.st_mode)) {
                //FCGI_LOG("fullpath_2 %s",fullpath);
				ret = _sys_delete(fullpath);
               // FCGI_LOG("ret_dir_2 %s",ret);
				if (ret != 0){
                 //   FCGI_LOG("ret_error %s",ret);
					goto _sys_delete_error;
                }
			}
			else {
                //FCGI_LOG("else  %s", "reddir");
#ifdef RECYCLE_EX
				ret = unlink_wrap(fullpath);
#else
                //FCGI_LOG("fina %s", "succeed"); 
				ret = unlink(fullpath);
#endif
				//add_wfm2_file_versioning_log(fullpath, WFM2_EVENT_ACTION_DELETE, 2);
				if (ret != 0)
					goto _sys_delete_error;
			}
		}//while
		if (dir) {
			closedir(dir);
			dir = NULL;
		}
        //FCGI_LOG("rmdir_befor %s", filename);
		ret = rmdir(filename);
        //FCGI_LOG("succeed_dir %d", ret);
		//snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "Deleted %s", path);

		/* filter out veto file */
		file_basename = __ax_calloc(1, strlen(filename) + 1);
		sprintf(file_basename, "%s", basename(filename));

		//Get_Private_Profile_String(GLOBAL, VETO_HIDDEN, " ", tmp, sizeof(tmp), WFM2_CONFIG_PATH);
		//if (!strcmp(tmp, "yes"))
		//	 file_count = get_veto_files_Ex(&veto_files, 0, remote_user);
		for (i=0; i < file_count; i++) {
			if (strstr((veto_files + i)->file_name, "*")) { // abc*xyz, *xyz
				char *p = NULL, *delim = "*", str[BUF512], *pstr = NULL;
				strcpy(str, (veto_files + i)->file_name);
				p = strtok(str, delim); 
				if (((veto_files + i)->file_name)[0] == '*') { // *abc
					pstr = (file_basename) + strlen(file_basename);
					pstr = pstr - strlen(p);
					if (!strcmp(pstr, p)) {
						//total = total - 1;
						veto_hidden = 1;
						break;
					}
				}
				else if (((veto_files + i)->file_name)[strlen((veto_files + i)->file_name) - 1] == '*'){ // abc*
					if (!strncmp(file_basename, p, strlen(p))) {
						//total = total - 1;
						veto_hidden = 1;
						break;
					}
				}
				else {
					if (!strncmp(file_basename, p, strlen(p))) {
						p = strtok(NULL, delim);
						pstr = (file_basename) + strlen(file_basename);
						pstr = pstr - strlen(p);
						if (!strcmp(pstr, p)) {
							//total = total - 1;
							veto_hidden = 1;
							break;
						}
					}
				}
			}
			else {
				if (!strcmp(file_basename, (veto_files + i)->file_name)) {
					//total = total - 1;
					veto_hidden = 1;
					break;
				}
			}
		}//for_colse
		if(file_basename)
			__ax_free(file_basename);
		
		/* eof veto file */

		//if(veto_hidden != 1)
		//	add_wfm2_file_op_log(filename, NULL, WFM2_EVENT_TYPE_INFO, 1, WFM2_EVENT_ACTION_DELETE, msg);
		if (ret != 0){
			//snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "Deleted %s failed", path);
			//if(veto_hidden != 1)
				//add_wfm2_file_op_log(filename, NULL, WFM2_EVENT_TYPE_WARN, 1, WFM2_EVENT_ACTION_DELETE, msg);
			goto _sys_delete_error;
		}
	}
	else {  //del file
        //FCGI_LOG("else_path %s", "else_path");
#ifdef RECYCLE_EX
		ret = unlink_wrap(filename);
#else
       // FCGI_LOG("filename_else %s",filename);
		ret = unlink(filename);
        //FCGI_LOG("else_ret %d",ret);
#endif
		/*if(wfm2_check_video_mime_type(filename) > 0) {
			_clean_video(filename, WFM_EMEDIA_ALL, 0, 0);
		}
		else if(wfm2_check_media_mime_type(filename, PHOTO) > 0) {
			_clean_photo(filename);
		}*/

		//snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "Deleted %s", path);
	//	add_wfm2_file_op_log(filename, NULL, WFM2_EVENT_TYPE_INFO, 0, WFM2_EVENT_ACTION_DELETE, msg);
		if (ret != 0){
			//snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "Deleted %s failed", path);
			//add_wfm2_file_op_log(filename, NULL, WFM2_EVENT_TYPE_WARN, 0, WFM2_EVENT_ACTION_DELETE, msg);
			goto _sys_delete_error;
		}
	}
	if(path) __ax_free(path);
    if (dir)
        closedir(dir);
    return 0;

_sys_delete_error:
	if(path) __ax_free(path);
    if (dir)
        closedir(dir);
	return -1;
}

int _delete(char *file)
{
    char *extend_file = NULL;
    int ret = -1;

    extend_file = (char *)__ax_calloc(1, strlen(file)*2 + 2);
    //cmd = (char *)__ax_calloc(1, strlen(file)*2 + 64);
    //FCGI_LOG("init_file %s", file);
    transfer_special_char(file, extend_file);
    //FCGI_LOG("tran_file %s", file);
    //Become_ID(remote_user, &euid, &egid);
    ret =_sys_delete(file);
    //FCGI_LOG("tran_ret %s", ret);
    //Become_ROOT();

    if (ret == 0){ 
        //FCGI_LOG("rust = %s","Success" );
        ret = WFM2_SUCCESS;
    }
     else{ 
        //FCGI_LOG("rust = %s","FAIL" );
         ret = WFM2_FAIL;
     }
    if (extend_file) __ax_free(extend_file);
    //if (cmd) __ax_free(cmd);

    return ret;
}

static int _delete_trash_all_file(char *name)
{
    DIR             *dir;
    struct dirent   *file;
    char            *full_path=NULL;
    int             ret=0;   

    dir = opendir(name);
    if (dir == NULL)
        return -1;

    while ((file = readdir(dir)) != NULL) {
        if(!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) continue;
        full_path = (char *)__ax_calloc(1, strlen(name) + strlen(file->d_name) + 2);
        sprintf(full_path, "%s/%s", name, file->d_name);
        ret = _delete(full_path);
        if(ret == WFM2_FAIL) {
            if(full_path) __ax_free(full_path);
            break; 
        } 

        if(full_path) __ax_free(full_path);
    }

    closedir(dir);

    return ret;
}

int op_delete(INPUT *input)
{
    INPUT   *tmp = NULL;
    char    *path = NULL, *full_path = NULL, *file_name = NULL, msg[MAX_CONN_LOG_MSG_SIZE+1]={0}, *sid = NULL;
    int     exist = 0, file_total = 0, i, ret = -1, nTrash=0;

    /* guest deny */
    if( (tmp=CGI_Find_Parameter(input, "sid")) ) {
        sid = tmp->val;
        if(!strcmp(sid, GUEST_SID)) {
            Status_To_JSON(WFM2_PERMISSION_DENY);
            return -1;
        }
    }
#ifdef QBOX_SUPPORT 
    if ((qbox_user == 1) && (op_qbox_check_file_profile(input) != 0)){
        return -1;   
    }
#endif

    if( (tmp=CGI_Find_Parameter(input, "path")) ) {
        path = tmp->val;
        //if(strstr(path, TRASH_CAN) != NULL)
          // nTrash = 1;
        if(!Check_Illegal_Path(path)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        } 
    }
    else return -1;
    
    full_path = (char *)__ax_calloc(1, strlen(path) + MAX_FILE_PATH + 15);
    exist = check_exist(path, full_path);
    if (exist < 0) {
        Status_To_JSON(WFM2_FILE_NO_EXIST);
        if (full_path) __ax_free(full_path);
        return -1;
    }

    if( (tmp=CGI_Find_Parameter(input, "file_total")) )
        file_total = atoi(tmp->val);

    if( (tmp=CGI_Find_Parameter(input, "file_name")) ) {
        //FCGI_LOG("File name %s", tmp->val);
        //FCGI_LOG("file total_0 %d", file_total);
        if(nTrash == 1 && !strcmp(tmp->val, path)) { // cancel all
            ret = _delete_trash_all_file(full_path);
        } 
        else{
            //FCGI_LOG("file total_1 %d", file_total);
            for (i=0; i<file_total; i++) {
                //FCGI_LOG("%s", "check?");
                if(!Check_Illegal_Path(tmp->val)) {
                    if (full_path) __ax_free(full_path);
                    Status_To_JSON(WFM2_ILLEGAL_NAME);
                    return -1;
                }
                //__ax_malloc size 3 = "\n" + "/" + "\\"
                file_name = (char *)__ax_calloc(1, strlen(full_path) + strlen(tmp->val) + 3);
                strcpy(file_name, full_path);
                if (!strcmp(tmp->val, "`"))
                    strcat(file_name, "/\\");
                else
                    strcat(file_name, "/");
                strcat(file_name, tmp->val);
                //FCGI_LOG("file name %s", file_name);
                snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "%s/%s", path, tmp->val);
                //FCGI_LOG("%s", msg);
#ifdef QBOX_SUPPORT
                /*if(qbox_user == 1){
                    char share_id[64];
                    int event_id = 0;
                   // if(qbox_check_my_shared(remote_user, file_name, share_id, sizeof(share_id), &event_id) > 0){
                     //   qbox_leave(event_id, remote_user)
                   // }
                }*/
#endif
                ret = _delete(file_name);
                if (ret == WFM2_FAIL) {
                    //FCGI_LOG("%s", "Fail?");
                    if (file_name) __ax_free(file_name);
                    break;
                }

                if (file_name){ 
                    //FCGI_LOG("file_name_fina %s",file_name);
                    __ax_free(file_name);
                }
                if(tmp->next != NULL) 
                    tmp = tmp->next;
                else {
                    //FCGI_LOG("tmp->next %s", tmp->next);
                    break;
                }
            }//for
        }//else

    }
    if(full_path)
        __ax_free(full_path);

    Status_To_JSON(ret);

    return 0;
}

int op_copy(INPUT *input)
{
    INPUT   *tmp = NULL;
    int exist  = 0, source_total = 0, mode = 0, i = 0;
    struct stat statbuf;
    char    *root_path = NULL, *full_path = NULL, *source_path = NULL, *source_file = NULL;
    char    *dest_path = NULL, *dest_root_path = NULL;
    char    *last_file = NULL, *dup=NULL;
    char    *p_path=NULL, *p_full_path=NULL;
    int     nLenSrcPath=0, nLenSrcFile=0;
    int     nLenDup=0, nLenDestPath=0;
    pid_t pid;

    if( (tmp=CGI_Find_Parameter(input, "mode")) )
        mode = atoi(tmp->val);

    if( (tmp=CGI_Find_Parameter(input, "dest_path")) ) {
        dest_path = tmp->val;
        nLenDestPath = strlen(dest_path);
        if(dest_path[nLenDestPath-1] == '/') {
            dest_path[nLenDestPath-1] = '\0';
            nLenDestPath -= 1;
        }
        strncpy(gDestPath, dest_path, BUF256);
        if(!Check_Illegal_Path(dest_path)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }
    }
    else return -1;

    if( (tmp=CGI_Find_Parameter(input, "dup")) ) {
        dup = tmp->val;
        nLenDup = strlen(dup);
        if(!Check_Illegal_Path(dup)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }
    }

    if( (tmp=CGI_Find_Parameter(input, "source_total")) )
        source_total = atoi(tmp->val);

    if( (tmp=CGI_Find_Parameter(input, "source_path")) ) {
        source_path = tmp->val;
        strncpy(gSrcPath, source_path, BUF256);
        nLenSrcPath=strlen(source_path);
        if(!Check_Illegal_Path(source_path)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }
    }

    if( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
        source_file = tmp->val;
        nLenSrcFile=strlen(source_file);
        if(!Check_Illegal_Path(source_file)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }

        // check copying folder to the same source path
        p_path = (char *)__ax_calloc(1, nLenSrcPath + nLenSrcFile + 2);
        p_full_path = (char *)__ax_calloc(1, nLenSrcPath + nLenSrcFile + PATH_PREFIX_LENGTH + 256);
        sprintf(p_path, "%s/%s", source_path, source_file);
        if((exist = check_exist(p_path, p_full_path)) == 1) { // folder
            if((!strncmp(p_path, dest_path, strlen(p_path))) && (nLenDestPath == strlen(p_path)) && (nLenDup == 0)) {
                Status_To_JSON(WFM2_ILLEGAL_NAME);
                if(p_path != NULL) __ax_free(p_path);
                if(p_full_path != NULL) __ax_free(p_full_path);
                return -1;
            }
        }
        else if (exist < 0) {
            Status_To_JSON(WFM2_SRC_FILE_NO_EXIST);
            if(p_path != NULL) __ax_free(p_path);
            if(p_full_path != NULL) __ax_free(p_full_path);
            return -1;
        }

        if(p_path != NULL) __ax_free(p_path);
        if(p_full_path != NULL) __ax_free(p_full_path);
    }
    else return -1;

    root_path = (char *)__ax_calloc(1, nLenSrcPath + MAX_FILE_LENGTH + 15);

    exist = check_exist(source_path, root_path);
    if(exist < 0 || exist != 1) {
        Status_To_JSON(WFM2_SRC_FILE_NO_EXIST);
        if(root_path)
            __ax_free(root_path);

        return -1;
    }

    dest_root_path = (char *)__ax_calloc(1, strlen(dest_path) + MAX_FILE_LENGTH + 15);
    exist = check_exist(dest_path, dest_root_path);
    if(exist < 0 || exist != 1) {
        Status_To_JSON(WFM2_DES_FILE_NO_EXIST);
        if(dest_root_path != NULL) __ax_free(dest_root_path);
        return -1;
    }

    pid = fork();
    if (pid < 0){
        exit(0);
    }
    if (pid == 0) {
        close(0);
        close(1);
#ifndef __DEBUG__
        close(2);
#endif
        struct utimbuf tbuf;
        int ret = 0;

        wfm_daemon_create_report_file("admin", thiscopypid);
        wfm_daemon_write_report_file(NULL, 0, 0, 0, 0, 0, thiscopypid);
//        wfm_daemon_write_report_file(NULL, 0, 0, 0, -1, 0, thiscopypid);
        Set_Private_Profile_Integer("", COPY_STATUS, WFM2_DONE, thiscopypid);

#ifdef COPY_MOVE_LIST
        _daemon_update(0, source_path, dest_path, source_file, WFM_EDL_TCOPY, 0, -1);
#endif

        //dest_root_path = (char *)__ax_calloc(1, strlen(dest_path) + MAX_FILE_LENGTH*2 + 15);
        //check_exist(dest_path, dest_root_path);   

        //calculate the total size of the sources first
        if ( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
            for (i=0; i<source_total; i++) {
                full_path = (char *)__ax_calloc(1, strlen(root_path) + MAX_FILE_LENGTH + 2);
                if (full_path == NULL) continue;
                strcpy(full_path, root_path);
                strcat(full_path, "/");
                strcat(full_path, tmp->val);
                Cal_Size_Ex(full_path , &source_total_file_count, &source_total_dir_count, &source_total_size, thiscopypid);
                if(full_path) __ax_free(full_path);
                if(tmp->next != NULL) tmp = tmp->next;
                else break;
            }
            source_file_count = source_total_file_count;
            source_dir_count = source_total_dir_count;
            source_size = source_total_size;
#ifdef COPY_MOVE_LIST
            _daemon_update(1, source_path, dest_path, source_file, WFM_EDL_TCOPY, 1, 0);
#endif
        }
        if(source_total_size == 0) {
            source_total_size = source_file_count + source_dir_count;
            source_size = source_total_size;
        }
        if ( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
            while (tmp != NULL && source_total != 0) {
                char *dest_path_tmp = NULL, *file_name = NULL;
                full_path = (char *)__ax_calloc(1, strlen(root_path) + MAX_FILE_LENGTH + 1);
                strcpy(full_path, root_path);
                strcat(full_path, "/");
                strcat(full_path, tmp->val);
                stat(full_path, &statbuf);

                if(dup != NULL &&  (nLenDup > 0)) {
                    dest_path_tmp = (char *)__ax_calloc(1, strlen(dest_root_path) + MAX_FILE_LENGTH*2 + nLenDup + 20);
                }
                else {
                    dest_path_tmp = (char *)__ax_calloc(1, strlen(dest_root_path) + MAX_FILE_LENGTH*2 + 1);
                }
                strcpy(dest_path_tmp, dest_root_path);
                strcat(dest_path_tmp, "/");
                //filename include "/"
                if (strstr(tmp->val, "/") != 0) {
                    file_name = strrchr(tmp->val, '/');
                    if(dup != NULL && (nLenDup > 0)) {
                        _find_file_name(dest_path_tmp, file_name, dup);
                        //strcat(dest_path_tmp, file_name);
                    }
                    else {
                        strcat(dest_path_tmp, file_name);
                    }
                }
                else {
                    //strcat(dest_path_tmp, tmp->val);
                    file_name = tmp->val;
                    if(dup != NULL && (nLenDup > 0)) {
                        _find_file_name(dest_path_tmp, file_name, dup);
                        //strcat(dest_path_tmp, file_name);
                    }
                    else {
                        strcat(dest_path_tmp, file_name);
                    }
                }

                // check destination file name length
                if(strlen(basename(dest_path_tmp)) > MAX_FILE_NAME_LENGTH) {
                    wfm_daemon_write_report_file(tmp->val, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, (int)(percent), 4, thiscopypid);
                    Set_Private_Profile_Integer("", COPY_STATUS, WFM2_FILE_NAME_TOO_LONG, thiscopypid);
#ifdef COPY_MOVE_LIST
                    _daemon_update(1, source_path, dest_path, tmp->val, WFM_EDL_TCOPY, 4, percent);
#endif

/*                    snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "[Copy fail] Destination file name %s too long.", basename(dest_path_tmp));
                    SendConnToLogEngine(EVENT_TYPE_ERROR, remote_user, http_info.remote_addr,
                        "---", CONN_SERV_HTTP, CONN_ACTION_COPY,  msg);
                    snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Copied %s/%s to %s/%s failed", source_path, file_name, dest_path, basename(dest_path_tmp));
                    add_wfm2_file_op_log(dest_path_tmp, full_path, WFM2_EVENT_TYPE_WARN, 1, WFM2_EVENT_ACTION_COPY, qsync_log_desc); */
                    Status_To_JSON(WFM2_FILE_NAME_TOO_LONG);
                    if (dest_path_tmp) __ax_free(dest_path_tmp);
                    if (full_path) __ax_free(full_path);

                    return -1;
                }
                if (S_ISDIR(statbuf.st_mode)) {
                    ret=0;
/*                    snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "%s/%s to %s/%s", source_path, file_name, dest_path, basename(dest_path_tmp));
                    SendConnToLogEngine(EVENT_TYPE_INFO, remote_user, http_info.remote_addr,
                        "---", CONN_SERV_HTTP, CONN_ACTION_COPY,  msg);
                    snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Copied %s/%s to %s/%s", source_path, file_name, dest_path, basename(dest_path_tmp));*/
//Change_Root_To_Users(remote_user, &euid, &egid);
//                  Become_ID(remote_user, &euid, &egid);
                    ret = _copy_dir(full_path, full_path, dest_path_tmp, mode);
//                    add_wfm2_file_op_log(dest_path_tmp, full_path, WFM2_EVENT_TYPE_INFO, 1, WFM2_EVENT_ACTION_COPY, qsync_log_desc);
//Change_Users_To_Root();
//                  Become_ROOT();
                    if (ret != SUCCESS) {
                        last_file = tmp->val;
/*                        snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "[Copy fail] %s/%s to %s/%s", source_path, file_name, dest_path, basename(dest_path_tmp));
                        SendConnToLogEngine(EVENT_TYPE_ERROR, remote_user, http_info.remote_addr,
                            "---", CONN_SERV_HTTP, CONN_ACTION_COPY,  msg);
                        snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Copied %s/%s to %s/%s failed", source_path, file_name, dest_path, basename(dest_path_tmp));
                        add_wfm2_file_op_log(dest_path_tmp, full_path, WFM2_EVENT_TYPE_WARN, 1, WFM2_EVENT_ACTION_COPY, qsync_log_desc);*/
                    }
                }
                else if (!S_ISLNK(statbuf.st_mode)) {
                    ret=0;
                    wfm_daemon_write_report_file(tmp->val, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, (int)(percent), 1, thiscopypid);
#ifdef COPY_MOVE_LIST
                    _daemon_update(1, source_path, dest_path, tmp->val, WFM_EDL_TCOPY, 1, percent);
#endif
/*                    snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "%s/%s to %s/%s", source_path, file_name, dest_path, basename(dest_path_tmp));
                    SendConnToLogEngine(EVENT_TYPE_INFO, remote_user, http_info.remote_addr,
                        "---", CONN_SERV_HTTP, CONN_ACTION_COPY,  msg); 
                    snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Copied %s/%s to %s/%s", source_path, file_name, dest_path, basename(dest_path_tmp));
                    add_wfm2_file_op_log(dest_path_tmp, full_path, WFM2_EVENT_TYPE_INFO, 0, WFM2_EVENT_ACTION_COPY, qsync_log_desc); */
                    ret = _copy_file(full_path, dest_path_tmp, mode);
                    if (SUCCESS != ret) {
                        last_file = tmp->val;
/*                        snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "[Copy fail] %s/%s to %s/%s", source_path, file_name, dest_path, basename(dest_path_tmp));
                        SendConnToLogEngine(EVENT_TYPE_ERROR, remote_user, http_info.remote_addr,
                            "---", CONN_SERV_HTTP, CONN_ACTION_COPY,  msg);
                        snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Copied %s/%s to %s/%s failed", source_path, file_name, dest_path, basename(dest_path_tmp));
                        add_wfm2_file_op_log(dest_path_tmp, full_path, WFM2_EVENT_TYPE_WARN, 0, WFM2_EVENT_ACTION_COPY, qsync_log_desc); */
                        if (dest_path_tmp) __ax_free(dest_path_tmp);
                        if (full_path) __ax_free(full_path);
                        if (ret == E_SRC) {
//                            if(!_check_volume(gSrcPath)) {
                                Set_Private_Profile_Integer("", COPY_STATUS, WFM2_SRC_PERMISSION_DENY, thiscopypid);
                                Status_To_JSON(WFM2_SRC_PERMISSION_DENY);
//                            }
                        }
                        else if (ret == E_DEST) {
//                            if(!_check_volume(gDestPath)) {
                                Set_Private_Profile_Integer("", COPY_STATUS, WFM2_DES_PERMISSION_DENY, thiscopypid);
                                Status_To_JSON(WFM2_DES_PERMISSION_DENY);
//                            }
                        }
                        else if (ret == E_COPY) {
//                            if(!_check_volume(gDestPath)) {
                                Set_Private_Profile_Integer("", COPY_STATUS, WFM2_QUOTA_ERROR, thiscopypid);
                                Status_To_JSON(WFM2_QUOTA_ERROR);
//                            }
                        }

                        //Status_To_JSON(WFM2_FAIL);
                        break;
                    } 
                    else {
                        source_copyed_count++;
                        tbuf.actime = statbuf.st_atime;
                        tbuf.modtime = statbuf.st_mtime;
                        utime(dest_path_tmp, &tbuf);
/*                        if(usb_file_system == FS_EXT3 || usb_file_system == FS_EXT4){
                            chown(dest_path_tmp, statbuf.st_uid, statbuf.st_gid);
                        }*/
                        source_size -= (unsigned long long)statbuf.st_size;
                        percent = 100. * (source_total_size-source_size)/source_total_size;
                        if((percent-copy_last)>0){
                            copy_last=percent;
                            if (percent == 100) {
                                wfm_daemon_write_report_file(tmp->val, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, 99, 1, thiscopypid);
                            }
                            else {
                                wfm_daemon_write_report_file(tmp->val, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, (int)(percent), 1, thiscopypid);
                            }
                        }
                    }
                }
                last_file = tmp->val;
                source_total--;
                if(tmp->next != NULL)
                    tmp = tmp->next;

                if (dest_path_tmp) __ax_free(dest_path_tmp);
                if (full_path) __ax_free(full_path);
            } // while

            //wfm_daemon_list_delete(gPid);
        }

        if(root_path)
            __ax_free(root_path);
        if(dest_root_path)
            __ax_free(dest_root_path);

#ifdef COPY_MOVE_LIST
        if(ret != SUCCESS) {
            _daemon_update(1, source_path, dest_path, last_file, WFM_EDL_TCOPY, 4, percent);
        }
        else {
            wfm_daemon_write_report_file(last_file, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, 100, 2, thiscopypid);
            _daemon_update(1, source_path, dest_path, last_file, WFM_EDL_TCOPY, 2, 100);
        }
#endif
    wfm_daemon_write_report_file(last_file, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, 100, 2, thiscopypid);
    }

    Status_With_Pid_To_JSON(WFM2_SUCCESS, pid);

    return 0;
}

int _copy_file(const char *src, const char *dest, int option)
{
    struct stat src_status, dest_status;
    int   src_fd, dest_fd;
    int   count, ret=0;
//  char    buf[BLK_SIZE];
    char    *buf=NULL, *src_tmp=NULL, *filename=NULL;
    //int     block_size = 128 << 10;
    int     block_size = COPY_WRITE_RECORD_BLOCKSIZE;
    int     nRecord=0, nRecordCounter=0;
    unsigned long long source_size_tmp=0;
    unsigned long long source_total_size_tmp=0;
    float   percent_tmp = 0;

    if( !(buf = (char *)__ax_malloc(block_size)) ) {
        /* out of memory */
        return E_COPY;
    }

    if (stat(dest, &dest_status)>=0 && S_ISDIR(dest_status.st_mode)){ //cannot copy a file to the folder with the same name
        if (option)
            return SUCCESS;
        return E_DEST;
    }

    ret = 1;
/*
    ret = fstatat(AT_FDCWD, src, &fstat, AT_SYMLINK_NOFOLLOW);

    // Check if this file is a sparse file.
    if (((511 + fstat.st_size) >> 9) > fstat.st_blocks) {
        int iRet = 0;
        iRet = _copy_sparse_file(src, dest, fstat.st_size, fstat.st_blksize, &fstat);
        return iRet;
    }
*/
    //Change_Root_To_Users(remote_user, &euid, &egid);
    stat(src, &src_status);
    nRecord = 0;
    if(src_status.st_size > COPY_WRITE_RECORD_STARTSIZE) {
        nRecord = 1;
        source_total_size_tmp = source_total_size;
        source_size_tmp = source_size;
        if(source_total_size_tmp <= 0 || source_size_tmp <=0) {
            return E_COPY;
        }
        src_tmp = __ax_strdup(src);
        filename=__ax_calloc(1, strlen(src_tmp)+1);
        strcpy(filename, basename(src_tmp));
    }

    if ( does_file_exist(dest) ) {
        if (stat(dest, &dest_status)>=0)
        {
            if (option) {
                    //Change_Users_To_Root();
                    return SUCCESS;
            }
        }

        dest_fd = open(dest, O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
    }else{
        dest_fd = open(dest, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
    }

    chmod(dest, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    //copy src to dest
    //Change_Root_To_Users(remote_user, &euid, &egid);
    src_fd  = open(src, O_RDONLY);
    if (0 > src_fd) {
        //Change_Users_To_Root();
        unlink(dest);
        return E_SRC; // I/O ERROR
    }

    if (0 > dest_fd) {
        //Change_Users_To_Root();
        return E_DEST; // I/O ERROR
    }

    while ( 0 < ( count = read(src_fd, buf, block_size) ) ) {
        if (count != write(dest_fd, buf, count)) {
            __ax_free(buf);
            close(src_fd);
            close(dest_fd);
            //Change_Users_To_Root();
            unlink(dest);
            return E_COPY;
        }
        else {
            if(nRecord == 1) {
                source_size_tmp -= (unsigned long long)count;
                if(source_size_tmp <= 0) source_size_tmp = 1;
                nRecordCounter += 1;
                if(nRecordCounter == COPY_WRITE_RECORD_TIMES) { // 20 
                    percent_tmp = 100. * (source_total_size_tmp-source_size_tmp)/source_total_size_tmp;
//__dbg("%s %d, percent_tmp=%d, source_size_tmp=%lld, source_total_size_tmp=%lld\n", __func__, __LINE__, (int)percent_tmp, source_size_tmp, source_total_size_tmp);
                    if(percent_tmp >= 100) percent_tmp = 99;

                    wfm_daemon_write_report_file(filename, source_copyed_count, source_total_file_count, (unsigned long long)src_status.st_size, (int)(percent_tmp), 1, thiscopypid);

                    nRecordCounter = 0;
                }
            }
        }
    }

    if(nRecord == 1) {
        nRecordCounter = 0;
        if(src_tmp != NULL) __ax_free(src_tmp);
        if(filename != NULL) __ax_free(filename);
    }
    __ax_free(buf);
    if (0 != count) {
        close(src_fd);
        close(dest_fd);
        //Change_Users_To_Root();
        unlink(dest);
        return E_SRC; // I/O ERROR
    }
    if ( 0 > close(src_fd) ) {
        close(dest_fd);
        //Change_Users_To_Root();
        unlink(dest);
        return E_SRC; // I/O ERROR
    }
    if ( 0 > close(dest_fd) ){
        //Change_Users_To_Root();
        unlink(dest);
        return E_COPY; // I/O ERROR
    }

    return SUCCESS;
}

int _find_file_name(char *dest_path, char *file_name, char *dup)
{
    struct stat statbuf;
    int i=0, j=0, nLen_destpath=0, nLen_filename=0;
    char *subname=NULL, *name=NULL;

    nLen_destpath = strlen(dest_path);
    nLen_filename = strlen(file_name);
    name = __ax_calloc(1, nLen_filename+1);
    subname = strrchr(file_name, '.');
    if(subname != NULL) {
        j = nLen_filename - strlen(subname);
    }
    else {
        j = nLen_filename;
    }

    for(i=0; i<j; i++) {
        name[i] = file_name[i];
    }

    for(i=0; i<MAX_COPY_DUP_NUM; i++) {
        if(subname != NULL) {
            sprintf(dest_path+nLen_destpath, "%s-%s(%d)%s", name, dup, (i+1), subname);
        }
        else {
            sprintf(dest_path+nLen_destpath, "%s-%s(%d)", name, dup, (i+1));
        }
        if(stat(dest_path, &statbuf) == -1) {
            break;
        }
    }

    if(name != NULL) __ax_free(name);

    return 0;
}

int _copy_dir(char *dir,char *parent,char *dest, int option)
{
    DIR     *dp,*dest_dp;
    struct dirent   *entry;
    struct stat     statbuf;
    int     ret_val = SUCCESS;
    char *parent_t=NULL, *path=NULL;
    char *path1=NULL, *src=NULL, *pSharePath=NULL;
    int exists=0, nPathLen=0;
    struct utimbuf tbuf;

    exists = stat(dest,&statbuf);
    if(exists>=0 && !S_ISDIR(statbuf.st_mode)){ //cannot copy a folder to the file with the same name
        return E_DEST;
    }
    //Change_Users_To_Root();
    if((dest_dp=opendir(dest))==NULL)
    {
        mkdir(dest,S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH);
    }
    else
        closedir(dest_dp);
    //stat(dir, &statbuf);
    //chmod(dest, statbuf.st_mode);
    chmod(dest, S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH);
//    chown(dest, euid, egid);

    if((dp=opendir(dir))==NULL){
        FCGI_LOG("cannot open directory : %s\n", parent);
        ret_val = E_DEST;
        return ret_val;
    }

    chdir(dir);

    while((entry=readdir(dp))!=NULL){
        exists = stat(entry->d_name,&statbuf);
        if(exists>=0 && S_ISDIR(statbuf.st_mode)){
            if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0)
                continue;
            parent_t = __ax_calloc(1, strlen(parent)+ strlen(entry->d_name)+ 64);
            path = __ax_calloc(1, strlen(dest)+ strlen(entry->d_name)+ 64);
            sprintf(parent_t,"%s/%s",parent,entry->d_name);
            sprintf(path,"%s/%s",dest,entry->d_name);
            mkdir(path,S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH);
            ret_val = _copy_dir(entry->d_name, parent_t, path, option);
            if (SUCCESS != ret_val) {
                FCGI_LOG("_copy_dir:%s = %d\n", path, ret_val);
                if(parent_t) __ax_free(parent_t);
                if(path) __ax_free(path);
                break;
            }
            chmod(path, statbuf.st_mode);
            tbuf.actime = statbuf.st_atime;
            tbuf.modtime = statbuf.st_mtime;
            utime(path, &tbuf);
/*            if(usb_file_system == FS_EXT3 || usb_file_system == FS_EXT4){
                chown(path, statbuf.st_uid, statbuf.st_gid);
            }*/
            if(parent_t) __ax_free(parent_t);
            if(path) __ax_free(path);
        } else if(exists>=0 && (!S_ISLNK(statbuf.st_mode))){
            int tmp_len = strlen(entry->d_name);
            path1 = __ax_calloc(1, strlen(dest)+tmp_len+64);
            src = __ax_calloc(1, strlen(parent)+tmp_len+64);
            sprintf(src,"%s/%s",parent,entry->d_name);
            sprintf(path1,"%s/%s",dest,entry->d_name);

            nPathLen=strlen(path1);
            pSharePath=__ax_calloc(1, nPathLen+1);
            _get_share_relative_name(path1, pSharePath, nPathLen);
            wfm_daemon_write_report_file(pSharePath, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, (int)(percent), 1, thiscopypid);

            ret_val = _copy_file(src, path1, option);
            if (SUCCESS != ret_val) {
                if (ret_val == E_SRC) {
//                    if(!_check_volume(gSrcPath)) {
                        Set_Private_Profile_Integer("", COPY_STATUS, WFM2_SRC_PERMISSION_DENY, thiscopypid);
                        Status_To_JSON(WFM2_SRC_PERMISSION_DENY);
//                    }
                }
                else if (ret_val == E_DEST) {
//                    if(!_check_volume(gDestPath)) {
                        Set_Private_Profile_Integer("", COPY_STATUS, WFM2_DES_PERMISSION_DENY, thiscopypid);
                        Status_To_JSON(WFM2_DES_PERMISSION_DENY);
//                    }
                }
                else if (ret_val == E_COPY) {
//                    if(!_check_volume(gDestPath)) {
                        Set_Private_Profile_Integer("", COPY_STATUS, WFM2_QUOTA_ERROR, thiscopypid);
                        Status_To_JSON(WFM2_QUOTA_ERROR);
//                    }
                }

                //Status_To_JSON(WFM2_FAIL);
                FCGI_LOG("_copy_file:%s = %d\n", src, ret_val);
                if(pSharePath) __ax_free(pSharePath);
                if(path1) __ax_free(path1);
                if(src) __ax_free(src);
                break;
            }
            else {
                source_copyed_count++;
                chmod(path1, statbuf.st_mode);
                tbuf.actime = statbuf.st_atime;
                tbuf.modtime = statbuf.st_mtime;
                utime(path1, &tbuf);
/*                if(usb_file_system == FS_EXT3 || usb_file_system == FS_EXT4){
                    chown(path1, statbuf.st_uid, statbuf.st_gid);
                }*/
                source_size -= (unsigned long long)statbuf.st_size;
                percent = 100. * (source_total_size-source_size)/source_total_size;

                if((percent-copy_last)>0){
                    copy_last=percent;
                    if (percent == 100) {
                        wfm_daemon_write_report_file(pSharePath, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, 99, 1, thiscopypid);
                    }
                    else {
                        wfm_daemon_write_report_file(pSharePath, source_copyed_count, source_total_file_count, (unsigned long long)statbuf.st_size, (int)(percent), 1, thiscopypid);
                    }
                }
            }
            if(pSharePath) __ax_free(pSharePath);
            if(path1) __ax_free(path1);
            if(src) __ax_free(src);
        }
/*        else if(exists>=0 && (S_ISLNK(statbuf.st_mode))){
            char *symlink_val=NULL;

            src = __ax_calloc(1, strlen(parent)+ strlen(entry->d_name)+ 64);
            path1 = __ax_calloc(1, strlen(dest)+ strlen(entry->d_name)+ 64);
            sprintf(src,"%s/%s",parent,entry->d_name);
            sprintf(path1,"%s/%s",dest,entry->d_name);

            unlink (path1);
            symlink_val = xreadlink(src);
            if (symlink (symlink_val, path1) == 0){
                chmod(path1, statbuf.st_mode);
                tbuf.actime = statbuf.st_atime;
                tbuf.modtime = statbuf.st_mtime;
                utime(path1, &tbuf);
                if(usb_file_system == FS_EXT3 || usb_file_system == FS_EXT4){
                    chown(path1, statbuf.st_uid, statbuf.st_gid);
                }
            }
            if(symlink_val) __ax_free(symlink_val);
            if(path1) __ax_free(path1);
            if(src) __ax_free(src);
        }
*/
        if (exists < 0) {
            FCGI_LOG("Couldn't stat %s\n", entry->d_name);
        }
    }
    closedir(dp);
    chdir("..");
    return ret_val;
}

int op_get_copy_and_move_status(INPUT *input)
{
    INPUT   *tmp=NULL;
    int     pid=0, ret=WFM2_SUCCESS, percent=0, status=WFM2_SUCCESS, copying=0;
    char    thiscopypid[256]={0};
    char    file_name[MAX_FILE_LENGTH]={0};
    char    file_size[32]={0};
    char    files_cnt[32]={0};
    char    th_num[32]={0};
    int     nDelFlag=0;

    if( (tmp=CGI_Find_Parameter(input, "pid")) )
        pid = atoi(tmp->val);

    sprintf(thiscopypid,"%d", pid);
    Get_Private_Profile_String("", COPY_FILE, "", file_name, sizeof(file_name), thiscopypid);
    Get_Private_Profile_String("", COPY_SIZE, "", file_size, sizeof(file_size), thiscopypid);
    Get_Private_Profile_String("", COPY_FILE_NUM, "", th_num, sizeof(th_num), thiscopypid);
    Get_Private_Profile_String("", COPY_FILE_CNT, "", files_cnt, sizeof(files_cnt), thiscopypid);
    percent = Get_Private_Profile_Integer("", COPY_PERCENT, 0, thiscopypid);
    copying = Get_Private_Profile_Integer("", COPY_COPYING_STATUS, 0, thiscopypid);
    status = Get_Private_Profile_Integer("", COPY_STATUS, 0, thiscopypid);

    if (status >= WFM2_QUOTA_ERROR && status <= WFM2_DES_PERMISSION_DENY)
        ret = status;
    else if(ret!=0)
        ret = WFM2_SUCCESS;
    else
        ret = WFM2_FAIL;

    if ((100 == percent) || ((percent == -1) && (copying == 0))) {
        nDelFlag = 1;
    }

    if(nDelFlag > 0 || ret == WFM2_FAIL || (ret >= WFM2_QUOTA_ERROR && ret <= WFM2_DES_PERMISSION_DENY)) 
        wfm_daemon_delete_report_file(thiscopypid);
    CopyAndMove_Status_To_JSON(ret, th_num, file_name, file_size, files_cnt, percent, copying);

    return 0;
}

int op_rename(INPUT *input)
{
    INPUT   *tmp = NULL;
    char    *dest_name = NULL, *path = NULL, *source_name = NULL, *path_tmp = NULL, *full_path = NULL, *sid = NULL;
    char    *full_dest_name = NULL, *full_source_name = NULL;
    int     ret = -1, exist = 0, isfolder = 0;
    FILE    *fp = NULL;
    struct stat statbuf;
    char    qsync_log_desc[DESC_LENGTH];

    /* guest deny */
    if( (tmp=CGI_Find_Parameter(input, "sid")) ) {
        sid = tmp->val;
        if(!strcmp(sid, GUEST_SID)) {
            Status_To_JSON(WFM2_PERMISSION_DENY);
            return -1;
        }
    }
#ifdef QBOX_SUPPORT
    if ((qbox_user == 1) && (op_qbox_check_file_profile(input) != 0)){
        return -1;
    }
#endif

    if( (tmp=CGI_Find_Parameter(input, "path")) ) {
        path = tmp->val;
        if(!Check_Illegal_Path(path)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }
    }
    else return -1;
    
    if(path_tmp)
        __ax_free(path_tmp);

    full_path = (char *)__ax_calloc(1, strlen(path) + MAX_FILE_PATH + 15);
    exist = check_exist(path, full_path);
    if (exist < 0) {
        Status_To_JSON(WFM2_FILE_NO_EXIST);
        if (full_path) __ax_free(full_path);
        return -1;
    }

    /*if(check_qbox_write_permission(remote_user, full_path) != 0){
        Status_To_JSON(WFM2_PERMISSION_DENY);
        if (full_path) __ax_free(full_path);
        return -1;
    }*/

    if( (tmp=CGI_Find_Parameter(input, "dest_name")) ) {
        dest_name = tmp->val;
        if(!Check_Illegal_Path(dest_name)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            if (full_path) __ax_free(full_path);
            return -1;
        }
    }
    else return -1;

    if( (tmp=CGI_Find_Parameter(input, "source_name")) ) {
        source_name = tmp->val;
        if(!Check_Illegal_Path(source_name)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            if (full_path) __ax_free(full_path);
            return -1;
        }
    }
    else return -1;

//If the destination name exist, return file exist.
    full_dest_name = (char *)__ax_calloc(1, strlen(full_path) + strlen(dest_name) + 2);
    sprintf(full_dest_name, "%s/%s", full_path, dest_name);
    fp = fopen(full_dest_name, "r");
    if (fp != NULL) {
        if (full_dest_name) __ax_free(full_dest_name);
        Status_To_JSON(WFM2_FILE_EXIST);
        return 0;
    }
    if (full_dest_name) __ax_free(full_dest_name);

    full_dest_name = (char *)__ax_calloc(1, strlen(dest_name) + strlen(full_path) + 2);
    full_source_name = (char *)__ax_calloc(1, strlen(source_name) + strlen(full_path) + 2);

    sprintf(full_source_name, "%s/%s", full_path, source_name);
    sprintf(full_dest_name, "%s/%s", full_path, dest_name);

    stat(full_source_name, &statbuf);
    if (S_ISDIR(statbuf.st_mode)) {
        isfolder = 1;
    }
    //Become_ID(remote_user, &euid, &egid);
    ret = rename(full_source_name, full_dest_name);
    //Become_ROOT();
  //sprintf(msg, "%s/%s to %s/%s", full_path+PATH_PREFIX_LENGTH, source_name,full_path+PATH_PREFIX_LENGTH, dest_name);
    //snprintf(msg, MAX_CONN_LOG_MSG_SIZE, "%s/%s to %s/%s", path, source_name, path, dest_name);
    //SendConnToLogEngine(EVENT_TYPE_INFO, remote_user, http_info.remote_addr,
      //      "---", CONN_SERV_HTTP, CONN_ACTION_RENAME,  msg);

    snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Renamed %s/%s to %s/%s",  path, source_name, path, dest_name);
   // add_wfm2_file_op_log(full_dest_name, full_source_name, WFM2_EVENT_TYPE_INFO, isfolder, WFM2_EVENT_ACTION_RENAME, qsync_log_desc);

    if (ret == 0) ret = WFM2_SUCCESS;
    else{
        snprintf(qsync_log_desc, sizeof(qsync_log_desc), "Renamed %s/%s to %s/%s failed",  path, source_name, path, dest_name);
        //add_wfm2_file_op_log(full_dest_name, full_source_name, WFM2_EVENT_TYPE_WARN, isfolder, WFM2_EVENT_ACTION_RENAME, qsync_log_desc);
        ret = WFM2_FAIL;
    }
    if (full_path) __ax_free(full_path);
    if (full_dest_name) __ax_free(full_dest_name);
    if (full_source_name) __ax_free(full_source_name);

    Status_To_JSON(ret);

    return 0;
}

int op_cancel_move_or_copy(INPUT *input)
{
    INPUT   *tmp = NULL;
    char    cmd[64]={0};
    char    pid[64]={0};
    int     ret = WFM2_SUCCESS;

    if( (tmp=CGI_Find_Parameter(input, "pid")) ) {
        strcpy(pid, tmp->val);
    }
    else return -1;

    sprintf(cmd, "kill -9 %s", pid);
    wfm_daemon_delete_report_file(pid);
    system(cmd);

    Status_To_JSON(ret);

    return 0;
}
unsigned long long _get_folder_size(char *name)
{

    DIR *dir;
    struct dirent   *file;
    char    *full_path = NULL;
    struct stat     statbuf;
    unsigned long long  sum = 0, size = 0;

    dir = opendir(name);
    if (dir == NULL)
        return -1;
    while ((file = readdir(dir)) != NULL) {
        if(!(wfm2_check_veto_files(veto_file_count, veto_files, file->d_name))) {
            full_path = (char *)__ax_calloc(1, strlen(name) + strlen(file->d_name) + 2);
            sprintf(full_path, "%s/%s", name, file->d_name);
            size=0;
            if (lstat(full_path, &statbuf)<0) {
                if (full_path) __ax_free(full_path);
                break;
            }

            if (S_ISDIR(statbuf.st_mode)==0) {
                if (S_ISLNK(statbuf.st_mode)) {
                    if (full_path) __ax_free(full_path);
                    continue;
                }
                //file
                gWFM2_FileCnt++;
                sum = sum + statbuf.st_size;
            }

            else {
                //dir
                if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")){
                    if (full_path) __ax_free(full_path);
                    continue;
                }
                gWFM2_FolderCnt++;
                size = _get_folder_size(full_path);
                sum = sum + size;

            }
            if (full_path) __ax_free(full_path);
        }
    }

    closedir(dir);

    return sum;
}

int op_get_file_size(INPUT *input)
{

    INPUT   *tmp = NULL;
    char    *path = NULL, *path_tmp = NULL, *full_path = NULL, *name = NULL;
    char    s_sum[32], s_folder[32], s_file[32];
    int     exist = 0, total = 0, i, nRoot=0;
    struct  stat buf;
    unsigned long long sum = 0, size = 0;
    int     nRet=0, nLenPath=0;
    char    *ssid=NULL, *fid=NULL;
    //SHARE_FID_FILTER_T sFidFilter;
    //SHARE_FID_JOIN_T *pShareJoinData=NULL;
    //SHARE_LINK_EVENT_T  sShareData;

    if ((tmp=CGI_Find_Parameter(input, "total")))
        total = atoi(tmp->val);

    if ((tmp=CGI_Find_Parameter(input, "ssid"))) {
        ssid = tmp->val;
    }

    if((tmp=CGI_Find_Parameter(input, "fid"))) {
        fid = tmp->val;
    }

    if ((tmp=CGI_Find_Parameter(input, "path"))) {
        path = tmp->val;
        nLenPath = strlen(path);
        if(!Check_Illegal_Path(path)) {
            Status_To_JSON(WFM2_ILLEGAL_NAME);
            return -1;
        }

        nRoot = (strcmp(path, "/") == 0) ? 1 : 0;
        if(nRoot == 0) {
            path_tmp = (char *)__ax_calloc(1, strlen(path) + 1);
            strcpy(path_tmp, path);
            //if (check_user_privilege(remote_user, path_tmp) < 0) {
            //  if(path_tmp) __ax_free(path_tmp);
            //return -1;
            //}
            if (path_tmp) __ax_free(path_tmp);
        }
    }
    else {
        if(ssid == NULL) {
            Status_To_JSON(WFM2_FAIL);
            return -1;
        }
    }

    if ((tmp=CGI_Find_Parameter(input, "name"))) {
        name = tmp->val;
    }

    //Get_Private_Profile_String(GLOBAL, VETO_HIDDEN, " ", sTmp, sizeof(sTmp), WFM2_CONFIG_PATH);
    //if (!strcmp(sTmp, "yes"))
    //    veto_file_count = get_veto_files_Ex(&veto_files, 0, "admin");
    
    if(ssid != NULL) {
        FCGI_LOG("%s","share");
    }
    else{
        if ((tmp=CGI_Find_Parameter(input, "name"))) {
            for (i=0; i<total; i++) {
                //FCGI_LOG("%d",total);
                name = tmp->val;
                if(!Check_Illegal_Path(name)) {
                    Status_To_JSON(WFM2_ILLEGAL_NAME);
                    return -1;
                }

                if((wfm2_check_veto_files(veto_file_count, veto_files, name))){
                    continue;
                }

                full_path = (char *)__ax_calloc(1, strlen(path) + strlen(name) + 256 + MAX_FILE_PATH);
                if(nRoot == 0) {
                    exist = check_exist(path, full_path);
                    if (exist < 0) {
                        Status_To_JSON(WFM2_FILE_NO_EXIST);
                        if(full_path) __ax_free(full_path);
                        if(veto_files) __ax_free(veto_files);
                        return -1;
                    }
                }
                else {
                    path_tmp = (char *)__ax_calloc(1, strlen(name) + 2);
                    sprintf(path_tmp, "/%s", name);
                    //FCGI_LOG("%s",path_tmp);
                    /*if (check_user_privilege(remote_user, path_tmp) < 0) {
                        Status_To_JSON(WFM2_PERMISSION_DENY);
                        if(path_tmp) __ax_free(path_tmp);
                        if(full_path) __ax_free(full_path);
                        if(veto_files) __ax_free(veto_files);
                        return -1;
                    }*/

                    exist = check_exist(path_tmp, full_path);
                    if (exist < 0) {
                        Status_To_JSON(WFM2_FILE_NO_EXIST);
                        if(path_tmp) __ax_free(path_tmp);
                        if(full_path) __ax_free(full_path);
                        if(veto_files) __ax_free(veto_files);
                        return -1;
                    }

                    if (path_tmp) __ax_free(path_tmp);
                }

                if(nRoot == 0) {
                    strcat(full_path, "/");
                    strcat(full_path, name);
                }

                size=0;
                nRet = stat(full_path, &buf);
                if(nRet == 0) {
                    if (S_ISDIR(buf.st_mode)) {
                        gWFM2_FolderCnt++;
                        size = _get_folder_size(full_path);
                    }
                    else if (!S_ISLNK(buf.st_mode)) {
                        gWFM2_FileCnt++;
                        size = (unsigned long long)(buf.st_size);
                    }

                }

                sum += size;
                if (full_path) __ax_free(full_path);

                if(tmp->next != NULL)
                    tmp = tmp->next;
                else break;
            }
        }
    }

    sprintf(s_sum, "%lld", sum);
    sprintf(s_folder, "%lld", gWFM2_FolderCnt);
    sprintf(s_file, "%lld", gWFM2_FileCnt);

    Status_With_Size_And_Counter_To_JSON(WFM2_SUCCESS, s_sum, s_folder, s_file);

    if (veto_files) __ax_free(veto_files);

    return 0;
}

