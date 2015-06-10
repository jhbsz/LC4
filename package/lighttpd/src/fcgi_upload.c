#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "fcgi_nas_cgiutil.h"
#include "fcgi_upload.h"
#include "fcgi_memdbg.h"
#include "fcgi_file_op.h"

//extern char remote_user[MAX_USER_LEN];
#define MAX_USER_LEN 256
#define SUCCESS     0

extern char remote_user[MAX_USER_LEN];
extern char user_sid[32];

/*******************************************************
                    Upload
 ******************************************************/
/*
static ssize_t savefile(const char *filepath, const void *buf, size_t size)
{
    int fd = open(filepath, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (fd < 0) return -1;
    ssize_t count = write(fd, buf, size);
    close(fd);
    return count;
}
*/
int op_get_chunked_upload_id(INPUT *input) 
{
    INPUT *tmp;
    int ret = 0, exist;
    char *full_path, path[1024];
    
    char lastpath[1024] = "", *filename = NULL;
    int length = 0;
    int fd = -1;
    if( (tmp = CGI_Find_Parameter(input, "upload_root_dir")) ) {
       snprintf(path, sizeof(path), "%s/%s", tmp->val, UPLOAD_CACHE_DIR);
    }

    full_path = (char *)__ax_calloc(1, strlen(path) + MAX_FILE_LENGTH + 15);
    exist = check_exist(path, full_path);

    if (exist < 0) {
       if(mkdir(full_path, S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH)< 0){
                ret = WFM2_OPEN_FILE_FAIL;
                       goto end;
       }
       chmod(full_path, S_IRUSR | S_IWUSR | S_IXUSR |S_IRGRP | S_IWGRP | S_IXGRP |S_IROTH | S_IWOTH | S_IXOTH);
    }

    // Create unique file
    strcpy(lastpath, full_path);
    strcat(lastpath, "/");
    length = strlen(lastpath);
    strcat(lastpath, "XXXXXXX");
    mktemp(lastpath);    

    fd = open(lastpath, O_CREAT | O_RDWR);
    if (fd == -1) {
		ret = WFM2_OPEN_FILE_FAIL;
		goto end;
    }
    filename = __ax_strndup(lastpath + length, 7);
             
end:
    printf("{\"status\": %d, \"upload_id\":\"%s\"}\n", ret, filename);
    if (filename) __ax_free(filename);
    if (fd != -1) close(fd);
    if (full_path) __ax_free(full_path);
    return 0;
}

int op_chunked_upload(INPUT *input)
{
    INPUT    *tmp; 
    int     exist = 0, overwrite = 0, ret = 0;
    char    *dest_path = "", *upload_id = "", *upload_dir = NULL, *upload_name = NULL;
    char    *type, *progress;
    char    *rename_string;
    char    *thumbnail_name = NULL, *thumbnail_origin_filesize = NULL, *thumbnailsize = NULL;
    char    qsync_dir[512]={0};

    int     multipart = 0;
    long long offset = 0, filesize = 0;
    FILE    *fp = NULL;
    time_t  mtime = 0, actime = 0;

    /* small thumbnail size */
    if( (tmp=CGI_Find_Parameter(input, "thumbnailsize")) ) {
        thumbnailsize = tmp->val;
    }
    else
        thumbnailsize = "80";

    if( (tmp=CGI_Find_Parameter(input, "thumbnail_origin_filesize")) ) {
        thumbnail_origin_filesize = tmp->val;
    }

    if( (tmp=CGI_Find_Parameter(input, "dest_path")) ) 
         dest_path = tmp->val;

    if( (tmp=CGI_Find_Parameter(input, "rename_dest")) ) {
        rename_string = tmp->val;
        if(strlen(rename_string) == 0)
            rename_string = NULL;
    } 

    if( (tmp=CGI_Find_Parameter(input, "upload_name")) ) {
        upload_name = tmp->val;
        if(strlen(upload_name) == 0)
           upload_name = NULL;
    }
    
    //path_tmp = (char *)__ax_calloc(1, strlen(dest_path) + 1);

    //strcpy(path_tmp, dest_path);
/*
    if(check_user_read_write_privilege(remote_user, path_tmp) < 0){
       if(path_tmp) __ax_free(path_tmp);
       //Status_With_Size_To_JSON(WFM2_PERMISSION_DENY, size_str);
       return -1;
    }
*/
    if( (tmp=CGI_Find_Parameter(input, "overwrite")) ) {
         overwrite = atoi(tmp->val);
    }

    if( (tmp=CGI_Find_Parameter(input, "type")) ) {
         type = tmp->val;
    }

    if( (tmp=CGI_Find_Parameter(input, "offset")) ) {
         offset = atoll(tmp->val);
    }

    if( (tmp=CGI_Find_Parameter(input, "filesize")) ) {
        filesize = atoll(tmp->val);
    }

    if( (tmp=CGI_Find_Parameter(input, "multipart")) ) {
         multipart = atoll(tmp->val);
    }

    if( (tmp=CGI_Find_Parameter(input, "upload_id")) ) {
         upload_id = tmp->val;
    }

    //if(path_tmp) __ax_free(path_tmp);

    if( (tmp=CGI_Find_Parameter(input, "progress")) ) {
        progress = (char *)__ax_calloc(1, strlen(tmp->val) + 6);
        if(strlen(tmp->val) > MAX_FILE_NAME_LENGTH) {
            // if progress name too long.
            //_get_progress_time_string(progress);
        }
        else {
            sprintf(progress, "/tmp/%s", tmp->val);
        }
        //sprintf(progress, "/tmp/%s", tmp->val);

        fp=fopen(progress, "w+");
        if (fp == NULL) {
            if (progress) __ax_free(progress);
            // Status_With_Size_To_JSON(WFM2_FAIL, size_str);
            return -1;
        }
        fclose(fp);

//         Set_Private_Profile_Integer("", UPLOAD_PID, pid, progress);
        if (progress) __ax_free(progress);
    }
   
    // Become_ID(remote_user, &euid, &egid);

    if((tmp=CGI_Find_Parameter(input, "upload_root_dir")) ) {
        char uppath[1024];
        snprintf(uppath, sizeof(uppath), "%s/%s", tmp->val, UPLOAD_CACHE_DIR);
        upload_dir = (char *)__ax_calloc(1, strlen(uppath) + MAX_FILE_LENGTH + 15);
//FCGI_LOG("uppath: %s", uppath);
        exist = check_exist(uppath, upload_dir);
        if (exist < 0) {
            //Status_With_Size_To_JSON(WFM2_FILE_NO_EXIST, size_str);
            if (upload_dir) __ax_free(upload_dir);
            return -1;
        }
    }

    // Finish uploading, now rename (move) it to real file and name
    if((tmp=CGI_Find_Parameter(input, "Filename")) && (upload_id != NULL) ) {

        char final_dest_path[2048], tmpfile[2048];
        if(thumbnail_origin_filesize) {

            thumbnail_name = (char *)__ax_calloc(1, strlen(dest_path) + strlen(tmp->val) + 100);
            strcpy(thumbnail_name, dest_path);
            strcat(thumbnail_name, "/");
            strcat(thumbnail_name, upload_name);
            tran_path(thumbnail_name);
            strcat(thumbnail_name, thumbnail_origin_filesize);
            strcat(thumbnail_name, "_");
            strcat(thumbnail_name, thumbnailsize);
//FCGI_LOG("thumbnail_name: %s", thumbnail_name);
            sprintf(qsync_dir, "%s/Qsync/.thumbnails/", ROOT_PATH);
            strcpy(final_dest_path, qsync_dir);
            strcat(final_dest_path, thumbnail_name);
//FCGI_LOG("final_dest_path: %s", final_dest_path);
        }
        else {
            // Destination folder path
            strcpy(final_dest_path, ROOT_PATH);
            strcat(final_dest_path, dest_path);
            strcat(final_dest_path, "/");
		    char *file_name = tmp->val;
            if (file_name[0] == '\\') {
                file_name++;
                strcat(final_dest_path, file_name);
            }
            else
                strcat(final_dest_path, file_name);
        }

        // Original temp file path
        strcpy(tmpfile, upload_dir);
        strcat(tmpfile, "/");
        strcat(tmpfile, upload_id);

        ret = rename(tmpfile, final_dest_path);
        if(ret) {
            ret = _copy_file(tmpfile, final_dest_path, WFM2_OVERWRITE);
            if(ret != SUCCESS) {
                FCGI_LOG("fail to copy file from %s to %s", tmpfile, final_dest_path);
            }
            else {
                unlink(tmpfile);
            }
        }
        
		if((tmp=CGI_Find_Parameter(input, "settime")) && tmp->val && atoi(tmp->val)!=0){
			if((tmp=CGI_Find_Parameter(input, "mtime")) && tmp->val && strlen(tmp->val)>0) {
				mtime = (time_t)atoi(tmp->val);
            }

			if((tmp=CGI_Find_Parameter(input, "atime")) && tmp->val && strlen(tmp->val)>0) {
				actime = (time_t)atoi(tmp->val);
            }
            if(mtime || actime) {
                set_filetime(final_dest_path, mtime, actime);
            }
		}

    }
    if (upload_dir) __ax_free(upload_dir);
    if (thumbnail_name) __ax_free(thumbnail_name);

    return ret;
}

int op_delete_chunked_upload_file(INPUT *input)
{
	INPUT   *tmp = NULL;
	int ret = 0, exist = 0;
	char *upload_id = NULL, *full_path = NULL, *upload_dir = NULL;
	struct stat statbuf;

	if( (tmp=CGI_Find_Parameter(input, "upload_id")) ) {
		upload_id = tmp->val;
	}

	if( (tmp=CGI_Find_Parameter(input, "upload_root_dir")) ) {
		char uppath[1024];
		snprintf(uppath, sizeof(uppath), "%s/%s", tmp->val, UPLOAD_CACHE_DIR);
		upload_dir = (char *)calloc(1, strlen(uppath) + MAX_FILE_LENGTH + 15);
		exist = check_exist(uppath, upload_dir);
		if (exist < 0) {
			ret = WFM2_FILE_NO_EXIST;
			goto end;
		}
		full_path = (char *)calloc(1, strlen(upload_dir) + strlen(upload_id) + 2);
		sprintf(full_path, "%s/%s", upload_dir, upload_id);
	}
	if(!stat(full_path, &statbuf)){
		if(S_ISDIR(statbuf.st_mode))
			ret = WFM2_OPEN_FILE_FAIL;
		else
#ifdef RECYCLE_EX
			qnap_unlink(full_path);
#else
		unlink(full_path);
                 
#endif
	}else
		ret = WFM2_FILE_NO_EXIST;

end:
	if(full_path)
		free(full_path);
         if(upload_dir)
                free(upload_dir);

	Status_To_JSON(ret);
	return ret;

}

int op_get_chunked_upload_status(INPUT *input)
{
    INPUT	*tmp = NULL;
    int ret = 0, exist = 0;
    char *upload_id = NULL, *full_path = NULL, size_str[64] = "0", size_str2[64] = "0", *upload_dir = NULL;
    struct stat statbuf;
    unsigned long long size = 0;
    //char buf[BLK_SIZE], md5_str[128] = "---";
    char md5_str[128] = {0};

    if( (tmp=CGI_Find_Parameter(input, "upload_id")) ) {
        upload_id = tmp->val;
    }

    if( (tmp=CGI_Find_Parameter(input, "upload_root_dir")) ) {
        char uppath[1024];
        snprintf(uppath, sizeof(uppath), "%s/%s", tmp->val, UPLOAD_CACHE_DIR);
        upload_dir = (char *)calloc(1, strlen(uppath) + MAX_FILE_LENGTH + 15);
        exist = check_exist(uppath, upload_dir);
        if (exist < 0) {
            ret = WFM2_FILE_NO_EXIST;
            goto end;
        }
        full_path = (char *)calloc(1, strlen(upload_dir) + strlen(upload_id) + 2);
        sprintf(full_path, "%s/%s", upload_dir, upload_id);
    }

    if(!stat(full_path, &statbuf)){
        if(S_ISDIR(statbuf.st_mode)){
            ret = WFM2_FILE_NO_EXIST;
        }else{
            size = statbuf.st_size;
            sprintf(size_str, "%lld", statbuf.st_size);
        }
    }else
        ret = WFM2_FILE_NO_EXIST;

end:
    if(full_path)
        free(full_path);
    if(upload_dir) 
        free(upload_dir);
    Status_With_Size_And_MD5_To_JSON(ret, size_str, md5_str, size_str2);
    return ret;
}
