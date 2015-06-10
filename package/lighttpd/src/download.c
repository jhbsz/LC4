#include "nas_cgiutil.h"
#include "download.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <libgen.h>
#include "memdbg.h"

extern char remote_user[MAX_USER_LEN];
#ifdef QBOX_SUPPORT
extern int qbox_user;
#endif
extern char user_sid[32];

//#define M2_MULTIFILE_DOWNLOAD

int op_download(INPUT *input)
{
    char    *source_file = NULL, *source_path = NULL, *full_path = NULL;
    char    *full_download_path = NULL;
    INPUT	*tmp;
    int     compress = 0, source_total = 0, isfolder = 0, exist = 0, filetype=0;
    long long offset = 0;

    if ( (tmp=CGI_Find_Parameter(input, "source_path")) ) {
            source_path = tmp->val;
#ifdef STORAGE_V2
            snprintf(total_path, sizeof(total_path), "%s", tmp->val);
#endif
            if(!Check_Illegal_Path(source_path)) {
                Status_To_JSON(WFM2_ILLEGAL_NAME);
                return -1;
            }
    }

//    path_tmp = (char *)__ax_calloc(1, strlen(source_path) + 1);
//    strcpy(path_tmp, source_path);
//    if (path_tmp) __ax_free(path_tmp);

    if ( (tmp=CGI_Find_Parameter(input, "source_total")) )
            source_total = atoi(tmp->val);

    if ( (tmp=CGI_Find_Parameter(input, "isfolder")) )
            isfolder = atoi(tmp->val);

    if ( (tmp=CGI_Find_Parameter(input, "compress")) )
            compress = atoi(tmp->val);

    if ( (isfolder || source_total>1) && compress==0 ) //force multiple files or folder to be downloaded as a archive (zip) file
            compress = COMPRESS_ZIP_ARCHIVE_ONLY;

    full_path = (char *)__ax_calloc(1, strlen(source_path) + MAX_FILE_LENGTH + 15);
    exist = check_exist(source_path, full_path);

    if (exist < 0) {
//        add_wfm_download_log(full_path, 0, isfolder?WFM2_EVENT_ISFOLDER_FOLDER:WFM2_EVENT_ISFOLDER_FILE);
        Status_To_JSON(WFM2_FILE_NO_EXIST);
        if (full_path) __ax_free(full_path);
        return -1;
    }

    if (
#ifdef M2_MULTIFILE_DOWNLOAD
            source_total == 1 && 
#endif
            isfolder == 0) {
        //one file and not directory
        if ( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
                source_file = tmp->val;
                if(!Check_Illegal_Path(source_file)) {
                    Status_To_JSON(WFM2_ILLEGAL_NAME);
                    if (full_path) __ax_free(full_path);
                    return -1;
                }
        }
        
        if((tmp = CGI_Find_Parameter(input, "type"))) { // for media // 1:music, 2:video, 3:photo
            filetype = atoi(tmp->val);
            if(filetype < MUSIC || filetype > PHOTO) {
                Status_To_JSON(WFM2_FAIL);
                if (full_path) __ax_free(full_path);
                return -1;
            }

            if(filetype == VIDEO) {
                ///TODO: Porting NAS Video Handling here 
            } else {
                Status_To_JSON(WFM2_FAIL);
                if (full_path) __ax_free(full_path);
                return -1;
            }
        } else {
            full_download_path = (char *)__ax_calloc(1, strlen(full_path) + strlen(source_file) + 1024);
#ifdef STORAGE_V2
            if (no_check == 1)
                sprintf(full_download_path, "%s/%s", source_path, source_file);
            else
                sprintf(full_download_path, "%s/%s", full_path, source_file);
#else
            sprintf(full_download_path, "%s/%s", full_path, source_file);
#endif
        }

        //add_wfm_download_log(full_download_path, 1, isfolder?WFM2_EVENT_ISFOLDER_FOLDER:WFM2_EVENT_ISFOLDER_FILE);

        if ( (tmp=CGI_Find_Parameter(input, "offset")) ) {
                offset = atoll(tmp->val);
        }

        _download(full_download_path, source_file, compress, 0, offset);

        if(full_download_path) __ax_free(full_download_path);
//        if(video != NULL) __ax_free(video);

    } 
#ifdef M2_MULTIFILE_DOWNLOAD
    else {
        //more than one file or dir
        if ( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
            char    download_list[64], *compress_file = NULL;
            char    compressed[MAX_FILE_LENGTH + 5], *p = NULL;
            int     irand = 0;
            FILE    *fp = NULL;

            srand((int) time(0));
            irand = rand();
            //TODO: Create temproray download list, NAS -> "/tmp/%dwfm2"
            sprintf(download_list, "/tmp/mnt/USB/%dwfm2", irand);
            fp = fopen(download_list, "w+");
            if (fp == NULL) {
                if (full_path) __ax_free(full_path);
                return -1;
            }
            compress_file = (char *)__ax_calloc(1, strlen(full_path) + 128);
            p = strrchr(full_path, '/');
            //TODO: Handle Multifile download list
            for (i=0; i<source_total; i++) {
                source_file = tmp->val;
                if(!Check_Illegal_Path(source_file)) {
                    Status_To_JSON(WFM2_ILLEGAL_NAME);
                    if (full_path) __ax_free(full_path);
                    if (compress_file) __ax_free(compress_file);
                    return -1;
                }
                {
                full_download_path = (char *)__ax_calloc(1, strlen(full_path) + strlen(source_file) + 2);
#ifdef STORAGE_V2
                if (no_check == 1)
                    sprintf(full_download_path, "%s/%s", total_path, source_file);
                else
                    sprintf(full_download_path, "%s/%s", full_path, source_file);
#else
                sprintf(full_download_path, "%s/%s", full_path, source_file);
#endif
                //add_wfm_download_log(full_download_path, 1, isfolder?WFM2_EVENT_ISFOLDER_FOLDER:WFM2_EVENT_ISFOLDER_FILE);
                __ax_free(full_download_path);
                }
                fputs(source_file, fp);
                fputc('\n', fp);
                tmp = tmp->next;
            }
            fclose(fp);
            _get_compressed_time_string(compressed); // download_yyyy-mm-dd_hh-nn-ss.zip
            _download(download_list, compressed, COMPRESS_ZIP_ARCHIVE_ONLY, 1, 0);
            unlink(download_list);
            if (compress_file) __ax_free(compress_file);
        }
    }
#endif

    if (full_path) __ax_free(full_path);

    return 0;
}

int _download(char *full_path, char *download_file, int compress, int multiple, long long offset)
{
    FILE    *fp = NULL;
    int     nbyte = 0;
    char    s[BLK_SIZE], filename[BUF_SIZE], *ptr=NULL;
    char    filename_tmp[BUF_SIZE] = {0};
    char    *pVeto=NULL;
    char    *cmd = NULL;

    // handle special case for IE
    ptr = (char*)getenv("HTTP_USER_AGENT");
    if (ptr && strstr(ptr, "MSIE")) {
        //CGI_Handle_IE_Filename(filename_tmp, download_file);
    } else {
        strcpy(filename_tmp, download_file);
    }
    // end

    //TODO: check and detect hidden files

    //Become_UID(remote_user, &uid, &gid);
    strcpy(filename, basename(filename_tmp));

    if (compress) {
        cmd = (char *)__ax_calloc(1, (strlen(full_path) + strlen(pVeto) + 80));
        if (!multiple) {
            char *dirtmp=NULL;
            dirtmp = (char *)__ax_calloc(1, (strlen(full_path)+1));
            strcpy(dirtmp,full_path);
            chdir(dirname(dirtmp));
            printf("Content-Disposition: attachment; filename=\"%s.zip\"\n", filename);
            printf("Content-Type: application/force-download%c%c%c%c", 13, 10, 13, 10);

            if (compress == COMPRESS_ZIP)
                sprintf(cmd, "/usr/bin/zip -q - \"%s\" 2>/dev/null", filename);
            else
                sprintf(cmd, "/usr/bin/zip -q0 - \"%s\" 2>/dev/null", filename);
        } else {
            printf("Content-Disposition: attachment; filename=\"%s\"\n", filename);
            printf("Content-Type: application/force-download%c%c%c%c", 13, 10, 13, 10);
            if (compress == COMPRESS_ZIP){
                //sprintf(cmd, "cat \"%s\" | /usr/bin/zip -rq@ - -x %s 2>/dev/null", full_path, pVeto);
                sprintf(cmd, "cat \"%s\" | /usr/bin/zip -rq@ - 2>/dev/null", full_path);
            } else {
                //sprintf(cmd, "cat \"%s\" | /usr/bin/zip -rq0@ - -x %s 2>/dev/null", full_path, pVeto);
                sprintf(cmd, "cat \"%s\" | /usr/bin/zip -rq0@ - -x 2>/dev/null", full_path);
            }
        }

        fp = popen(cmd, "r");
        if(fp == NULL) {
            if (cmd) __ax_free(cmd);
            return -1;
        }
        pclose(fp);
    
        //if (pVeto) __ax_free(pVeto);
        if (cmd) __ax_free(cmd);

    } else {
        char *ptr_httprange=NULL;
        ptr_httprange = (char*)getenv("HTTP_RANGE");
        /*if(wfm2_user_agent_filter(ptr) || ptr_httprange != NULL) {
            if(!wfm2_range_download(full_path, WFM2_DL_FLAG_DEFAULT)) return 0;
            else return -1;
        } else */ 
        { 
            struct stat _fstat;
            if(stat(full_path, &_fstat)) {
                DBG("file %s not exist", full_path);
                return -1;
            }
            printf("Content-Length: %llu\n", (unsigned long long)(_fstat.st_size));
            printf("Content-Disposition: attachment; filename=\"%s\"\n", filename);
            printf("Content-Type: application/force-download%c%c%c%c", 13, 10, 13, 10);
        }

        fp = fopen(full_path,"r");

        if(fp == NULL)
            return -1;
        
        if(offset){
            if(fseeko(fp, offset, SEEK_SET) != 0){
                return -1;
            }
        }

        do {
            nbyte = fread(s, 1, BLK_SIZE, fp);
            fwrite(s, 1, nbyte, stdout);
        } while (nbyte > 0);

        fclose(fp);

    }

    return 0;
}

int _get_compressed_time_string(char *aCompressName)
{
    struct tm *tm=NULL;
    time_t tt=time(NULL);

    if(aCompressName == NULL) {
        return -1;
    }

    tm = localtime(&tt);
    sprintf(aCompressName, "download_%04d-%02d-%02d_%02d-%02d-%02d.zip",
                            (tm->tm_year+1900), (tm->tm_mon+1), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    return 0;
}

int op_viewer(INPUT *input)
{
    INPUT   *tmp=NULL;
    char    *source_file=NULL, *source_path=NULL, *full_path=NULL;
    char    *full_download_path=NULL, *ssid=NULL, *format=NULL;
    int     exist=0, nRet=0, nGot=0, nCheck=0;
    //SHARE_LINK_EVENT_T  sShareData;
    char    mode[WFM2_VIDEO_MODE_LEN+1]={0};
    int player = 0, realtime_transcode = 0, vq = WFM_TQ_ORIGINAL;
    int sstart = 0;

    if((tmp=CGI_Find_Parameter(input, "player")) && atoi(tmp->val) > 0) {
        player=atoi(tmp->val);
    }

    if((tmp=CGI_Find_Parameter(input, "rtt")) && atoi(tmp->val) > 0) {
        realtime_transcode=atoi(tmp->val);
    }

    if((tmp=CGI_Find_Parameter(input, "vq"))) {
        int _vq=atoi(tmp->val);
        if(_vq == WFM_TQ_ORIGINAL)
            vq = WFM_TQ_ORIGINAL;
        else if(_vq >= WFM_TQ_HIGH)
            vq = WFM_TQ_HIGH;
        else if(_vq <= WFM_TQ_LOW)
            vq = WFM_TQ_LOW;
        else
            vq = _vq;
    }

    if((tmp=CGI_Find_Parameter(input, "ss")) && atoi(tmp->val) > 0) {
        sstart=atoi(tmp->val);
    }

    // for share link open
    if((tmp=CGI_Find_Parameter(input, "ssid"))) {
        DBG("Device not support share link");
    } else {
        if ( (tmp=CGI_Find_Parameter(input, "source_path")) ) {
            source_path = tmp->val;
            if(!Check_Illegal_Path(source_path)) {
                Status_To_JSON(WFM2_ILLEGAL_NAME);
                return -1;
            }
        }
/*
        path_tmp = (char *)__ax_calloc(1, strlen(source_path) + 1);
        strcpy(path_tmp, source_path);
        if(check_user_privilege(remote_user, path_tmp) < 0){
            if(path_tmp)
                __ax_free(path_tmp);
            return -1;
        }
        if (path_tmp) __ax_free(path_tmp);
*/
        full_path = (char *)__ax_calloc(1, strlen(source_path) + MAX_FILE_LENGTH + 15);
        exist = check_exist(source_path, full_path);

        if (exist < 0) {
            Status_To_JSON(WFM2_FILE_NO_EXIST);
            if (full_path) __ax_free(full_path);
            return -1;
        }

        if ( (tmp=CGI_Find_Parameter(input, "source_file")) ) {
            source_file = tmp->val;
            if(!Check_Illegal_Path(source_file)) {
                Status_To_JSON(WFM2_ILLEGAL_NAME);
                if (full_path) __ax_free(full_path);
                return -1;
            }
        }

        if ( (tmp=CGI_Find_Parameter(input, "format")) ) {
            format = tmp->val;
        }

        if(format != NULL) {
            full_download_path = (char *)__ax_calloc(1, strlen(full_path) + WFM2_VIDEO_TRANSCODE_PATH_LEN + strlen(source_file) + 20);
            if(!strcmp(format, WFM2_VFORMAT_MP4_240) || !strcmp(format, WFM2_VFORMAT_FLV_240)) {
                sprintf(full_download_path, "%s/%s/%s%s", full_path, WFM2_VIDEO_TRANSCODE_PATH, source_file, WFM2_TRANSCODED_FILE_POSTFIX_MP4_240);
                nGot = 1;
            }
            else if(!strcmp(format, WFM2_VFORMAT_MP4_360) || !strcmp(format, WFM2_VFORMAT_FLV_360)) {
                sprintf(full_download_path, "%s/%s/%s%s", full_path, WFM2_VIDEO_TRANSCODE_PATH, source_file, WFM2_TRANSCODED_FILE_POSTFIX_MP4_360);
                nGot = 1;
            }
            else if(!strcmp(format, WFM2_VFORMAT_MP4_720) || !strcmp(format, WFM2_VFORMAT_FLV_720)) {
                sprintf(full_download_path, "%s/%s/%s%s", full_path, WFM2_VIDEO_TRANSCODE_PATH, source_file, WFM2_TRANSCODED_FILE_POSTFIX_MP4_720);
                nGot = 1;
            }
            else if(!strcmp(format, WFM2_VFORMAT_MP4_480) || !strcmp(format, WFM2_VFORMAT_FLV_480)) {
                sprintf(full_download_path, "%s/%s/%s%s", full_path, WFM2_VIDEO_TRANSCODE_PATH, source_file, WFM2_TRANSCODED_FILE_POSTFIX_MP4_480);
                nGot = 1;
            }
            else if(!strcmp(format, WFM2_VFORMAT_MP4_1080) || !strcmp(format, WFM2_VFORMAT_FLV_1080)) {
                sprintf(full_download_path, "%s/%s/%s%s", full_path, WFM2_VIDEO_TRANSCODE_PATH, source_file, WFM2_TRANSCODED_FILE_POSTFIX_MP4_1080);
                nGot = 1;
            }
            else {
                sprintf(full_download_path, "%s/%s", full_path, source_file);
            }
        }
        else {
            full_download_path = (char *)__ax_calloc(1, strlen(full_path) + strlen(source_file) + 6);
            sprintf(full_download_path, "%s/%s", full_path, source_file);
        }
    }

    if(ssid != NULL && strcmp(mode, "normal")) {
        DBG("mode %s ssid %s nCheck %d", mode, ssid, nCheck);
        if(nCheck == 1) {
            if(wfm2_range_download(full_download_path, WFM2_DL_FLAG_CHECK_MIMETYPE) < 0) nRet=-1;
        }
        else {
            if(wfm2_range_download(full_download_path, WFM2_DL_FLAG_DEFAULT) < 0) nRet=-1;
        }
    }
    else if(ssid != NULL && !strcmp(mode, "normal")) {
        /*if(realtime_transcode == FLV || realtime_transcode == MP3){
            // init  transcode client info
            memset(&clientInfo, 0, sizeof(WFM_CLIENT_INFO));
            clientInfo.uid = -1;
            if(strlen(remote_user) > 0)
                strncpy(clientInfo.username, remote_user, USER_LENGTH);
            else
                strcpy(clientInfo.username, "UNKNOWN_USER");

            if(strlen(http_info.remote_addr) > 0)
                strncpy(clientInfo.ip, http_info.remote_addr, IP_LENGTH);
            else
                strncpy(clientInfo.ip, "127.0.0.1", IP_LENGTH);

            strncpy(clientInfo.tag, WFM_FILE_STATION, WFM_FILE_STATION_LENGTH);
            if(wfm2_realtime_transcode(full_download_path, realtime_transcode, vq, sstart, &clientInfo) < 0) {
                nRet=-1;
            }
        }
        else */ {
            if(wfm2_range_download(full_download_path, WFM2_DL_FLAG_DEFAULT) < 0) nRet=-1;
        }
    }
    /*else if(player){
        if(stat(full_download_path, &statbuf) != 0) { // not find
            if(full_path) __ax_free(full_path);
            return -1;
        }
        op_player(input);
    }*/
    else if(format != NULL && nGot == 1) {
       if(!strcmp(format, WFM2_VFORMAT_FLV_240) ||
                !strcmp(format, WFM2_VFORMAT_FLV_360) ||
                !strcmp(format, WFM2_VFORMAT_FLV_480) ||
                !strcmp(format, WFM2_VFORMAT_FLV_720) ||
                !strcmp(format, WFM2_VFORMAT_FLV_1080)){
/* 
           int throttle = THROTTLE_WAIT_DEFAULT;//enable by default, sleep 0.5 sec per 2 secs
            if((tmp=CGI_Find_Parameter(input, "throttle")) && atoi(tmp->val) >= 0) {
                throttle = atoi(tmp->val);
            }
            if(wfm2_mp42flv(full_download_path, throttle, sstart) < 0) nRet=-1;
*/
        }
        else 
            if(wfm2_range_download(full_download_path, WFM2_DL_FLAG_CHECK_MIMETYPE) < 0) nRet=-1;
    }
    else if(format != NULL && nGot != 1) {
        if(wfm2_range_download(full_download_path, WFM2_DL_FLAG_DEFAULT) < 0) nRet=-1;
    }
    else {
        //Become_UID(remote_user, &uid, &gid);
        if(realtime_transcode == FLV || realtime_transcode == MP3){
            /*
            memset(&clientInfo, 0, sizeof(WFM_CLIENT_INFO));
            clientInfo.uid = -1;
            if(strlen(remote_user) > 0)
                strncpy(clientInfo.username, remote_user, USER_LENGTH);
            else
                strcpy(clientInfo.username, "UNKNOWN_USER");

            if(strlen(http_info.remote_addr) > 0)
                strncpy(clientInfo.ip, http_info.remote_addr, IP_LENGTH);
            else
                strncpy(clientInfo.ip, "127.0.0.1", IP_LENGTH);

            strncpy(clientInfo.tag, WFM_FILE_STATION, WFM_FILE_STATION_LENGTH);
            if(wfm2_realtime_transcode(full_download_path, realtime_transcode, vq, sstart, &clientInfo) < 0) {
                nRet=-1;
            }
            */
        }
        else {
            if(wfm2_range_download(full_download_path, WFM2_DL_FLAG_DEFAULT) < 0) nRet=-1;
        }
    }

    if (full_download_path) __ax_free(full_download_path);
    if (full_path) __ax_free(full_path);

    return nRet;
}
