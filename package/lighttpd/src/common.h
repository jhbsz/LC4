
#ifndef _COMMON_H_
#define _COMMON_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <json-c/json.h>
#include "tag.h"

//TODO: Future to move these define to naslog files
// =================================================
#define USER_LENGTH     64
#define IP_LENGTH       64
#define COMP_LENGTH     64
#define KEY_LENGTH      64
#define ACTION_LENGTH   64
#define DATE_LENGTH     10  // 2011-06-24
#define FILTER_LENGTH   256
#define DESC_LENGTH     1024
#define RES_LENGTH      1024

//TODO: Future to move these define to file_op.c files
// =================================================
#define COPY_PERCENT    "percent"
#define COPY_FILE_NUM   "copy_number"
#define COPY_FILE_CNT   "files_count"
#define COPY_DIR_CNT    "dirs_count"
#define COPY_FILE       "copy_file"
#define COPY_SIZE       "copy_size"
#define COPY_COPYING_STATUS     "copying"
#define COPY_STATUS "copy_status"

#define WFM2_OVERWRITE  0
#define WFM2_SKIP   1

#define WFM_FILE_STATION        "File Station"
#define WFM_FILE_STATION_LENGTH 12

#define BUF256                  256
#define BUF512                  512
#define BUF_SIZE                512
#define MAX_FILE_LENGTH	        257
#define MAX_FILE_NAME_LENGTH    255
#define MAX_FILE_PATH           256
#define MAX_CONN_LOG_MSG_SIZE   1023

#define BLK_SIZE        (8 * 512)

#ifndef QNAP_HAL_SUPPORT
    #define PATH_PREFIX_LENGTH  16
#else
    #define PATH_PREFIX_LENGTH  33
#endif

#define ADMIN_SID          "0ysn22vr"
#define GUEST_SID          "mkothm"
#define MAX_USER_LEN       256
#define WFM2_LIST_MODE_LEN  32
#define WFM2_SORT_MODE_LEN  32 // same as common.h sort_mode length
#define WFM2_SORT_TYPE_LEN   8 // ASC / DESC
#define USER_NAME_LENGTH    32 
#define GROUP_NAME_LENGTH   32 

#define SHARE_PATH_LENGTH    256

#define ROOT_PATH          "/tmp/mnt"

#define COMPRESS_ZIP_ARCHIVE_ONLY   1
#define COMPRESS_ZIP    2

#define __DEBUG__
#ifdef __DEBUG__
    #define DBG(fmt, ...) fprintf(stderr, "File: "__FILE__", Line: %d, Func:%s | "fmt"\n", __LINE__, __func__, ##__VA_ARGS__) 
    #define DBG_T() fprintf(stderr, "File: "__FILE__", Line: %d, Func:%s\n", __LINE__, __func__)
#else
    #define DBG(fmt, ...)
    #define DBG_T()
#endif

#define WFM2_MIME_LENGTH    64

#define WFM2_VIDEO_TRANSCODE_PATH           ".@__thumb/transcode"   // 
#define WFM2_VIDEO_TRANSCODE_PATH_LEN       19  // 
//#define WFM2_VIDEO_TRANSCODED_FOLDER      "_transcoded"   // 
#define WFM2_VIDEO_TRANSCODED_FOLDER        MEDIALIB_TRANSCODE_SHOW_FOLDER
 
#define WFM2_VIDEO_TRANSCODED_FOLDER_LEN    11  // 
#define WFM2_VIDEO_MACHINE_LEN  10  // 
#define WFM2_VIDEO_TYPE_LEN     5   // mp4
#define WFM2_VIDEO_MODE_LEN     12  // mobile360p / mobile720p
#define WFM2_VFORMAT_MP4_240    "mp4_240"
#define WFM2_VFORMAT_MP4_360    "mp4_360"
#define WFM2_VFORMAT_MP4_480    "mp4_480"
#define WFM2_VFORMAT_MP4_720    "mp4_720"
#define WFM2_VFORMAT_MP4_1080   "mp4_1080"
#define WFM2_VFORMAT_FLV_720    "flv_720"
#define WFM2_VFORMAT_FLV_360    "flv_360"
#define WFM2_VFORMAT_FLV_240    "flv_240"
#define WFM2_VFORMAT_FLV_480    "flv_480"
#define WFM2_VFORMAT_FLV_1080   "flv_1080"
#define WFM2_TRANSCODED_FILE_POSTFIX_MP4_720    ".ipad720p" 
#define WFM2_TRANSCODED_FILE_POSTFIX_MP4_240    ".mobile240p"
#define WFM2_TRANSCODED_FILE_POSTFIX_MP4_360    ".mobile360p"
#define WFM2_TRANSCODED_FILE_POSTFIX_FLV_360    ".mobile360p" //use WFM2_TRANSCODED_FILE_POSTFIX_MP4_360
#define WFM2_TRANSCODED_FILE_POSTFIX_MP4_480    ".mobile480p"
#define WFM2_TRANSCODED_FILE_POSTFIX_MP4P_720   ".mobile720p"
#define WFM2_TRANSCODED_FILE_POSTFIX_MP4_1080   ".mobile1080p"
#define WFM2_TRANSCODED_FILE_POSTFIX_FLV_720    ".ipad720p" //x86 only generate 360/720 mp4(h.264)
#define THROTTLE_WAIT_USEC  100000
#define THROTTLE_WAIT_MAX   20
#define THROTTLE_WAIT_DEFAULT   6

#ifndef FFMPEG_CMD
#define FFMPEG_CMD  "/usr/bin/ffmpeg"
#endif
#ifndef MEDIALIB_FFMPEG_CMD
#define MEDIALIB_FFMPEG_CMD "/usr/local/medialibrary/bin/ffmpeg"
#endif

//TODO: Remove Share no access
#define SHARE_NOACCESS 0

typedef struct wfm2_mime{
    const int index;
    const char *name;
    const char *type;
}WFM2_MIMES;

typedef struct tagSORT {
    char    sort_mode[32];
    int     (*function)(char *sort_type, int total);
} SORT_FUNC;

typedef struct tagVETO_INFO {
        char file_name[BUF512];
} VETO_INFO;

typedef struct _WFM2_VIDEO_INFO {
    int  mp4_240;
    int  mp4_360;
    int  mp4_480;
    int  mp4_720;
    int  mp4_1080;
    int  transcoded;
    int  play;
    char filename[MAX_FILE_LENGTH]; // 257
} WFM2_VIDEO_INFO;

typedef enum {
    MUSIC = 1,
    VIDEO,
    PHOTO,
    LIST_FOLDER,
    MEDIA // 1||2||3
} WFM2_MEDIA_TYPE;

typedef enum {
    WFM_EMEDIA_MODE_MP4_240 = 1,
    WFM_EMEDIA_MODE_MP4_360,
    WFM_EMEDIA_MODE_MP4_720,
} WFM2_MEDIA_MODE_FLAG;

typedef struct wfm_video_quality{
    const int index;
    const int qmin;
    const int qmax;
    const int frame_rate;
    const char *bitrate;
} WFM_VIDEO_TRANSCODE_QUALITY_STRUCT;

#define WFM_TQ_ORIGINAL 100
typedef enum {
    WFM_TQ_LOW = 1,
    WFM_TQ_MEDIUM,
    WFM_TQ_HIGH
} WFM_VIDEO_TRANSCODE_QUALITY_DEFINE;

typedef enum {
    FLV = 1,
    MP4,
    MP3,
    HLS
} WFM_TRANSCODE_FORMAT;

typedef enum {
    WFM_EMEDIA_IMAGE = 1,
    WFM_EMEDIA_VIDEO,
    WFM_EMEDIA_ALL
} WFM_MEDIA_FLAG_DEFINE;

typedef enum {
    WFM2_DL_FLAG_DEFAULT = 0,
    WFM2_DL_FLAG_CHECK_MIMETYPE,
    WFM2_DL_FLAG_REALTIME_TRANSCODE,
    WFM2_DL_FLAG_LAUNCH_PLAYER
} WFM2_DOWNLOAD_FLAG;

// return status
typedef enum {
        WFM2_FAIL = 0,
        WFM2_DONE = 1,
        WFM2_SUCCESS = 1,
        WFM2_FILE_EXIST,
        WFM2_AUTH_FAIL,
        WFM2_PERMISSION_DENY,
        WFM2_FILE_NO_EXIST = 5,
        WFM2_SRC_FILE_NO_EXIST = 5, // equal to WFM2_FILE_NO_EXIST
        WFM2_EXTRACTING,
        WFM2_OPEN_FILE_FAIL,
        WFM2_DISABLE,
        WFM2_QUOTA_ERROR,
        WFM2_SRC_PERMISSION_DENY,
        WFM2_DES_PERMISSION_DENY,
        WFM2_ILLEGAL_NAME,
        WFM2_EXCEED_ISO_MAX,
        WFM2_EXCEED_SHARE_MAX,
        WFM2_NEED_CHECK,
        WFM2_RECYCLE_BIN_NOT_ENABLE,
        WFM2_CHECK_PASSWORD_FAIL,
        WFM2_VIDEO_TCS_DISABLE,
        WFM2_DB_FAIL,
        WFM2_PARAMETER_ERROR,
        WFM2_DEMO_SITE,
        WFM2_TRANSCODE_ONGOING,
        WFM2_SRC_VOLUME_ERROR,
        WFM2_DES_VOLUME_ERROR,
        WFM2_DES_FILE_NO_EXIST,
        WFM2_FILE_NAME_TOO_LONG,
        WFM2_FOLDER_ENCRYPTION
} WFM2_STATUS_TYPE;

typedef struct DOMAIN_IP_INFO{
        char ext_ip[BUF_SIZE];
        char mycloudnas_dn[BUF_SIZE];
        char ddns_dn[BUF_SIZE];
        char local_ip[BUF_SIZE];
        char host_ip[BUF_SIZE];
        char local_ip_list[BUF_SIZE];
        int  port;
        int  sslPort;
        int  extPort;
        int  extSslPort;
}DOMAIN_IP_INFO;

typedef struct __file_list_param_ {
    int  start;
    int  end;
    int  limit;
    int  is_iso;
    int  sortmode;
    int  filetype; // 1:music, 2:video, 3:photo
    int  hidden_file;
    int  recycle;   // 1:donot display @Recycle folder, 0:display
    int  permission;
    int  mp4_240;
    int  mp4_360;
    int  mp4_480;
    int  mp4_720;
    int  mp4_1080;
    int  check_rtt;
#ifdef STORAGE_V2
       int  no_check;
#endif
#ifdef LIBRSYNC
    int view_versioning;
#endif
    char remoteUser[MAX_USER_LEN+1];
    //char node[MAX_FILE_PATH+1];
    char listMode[WFM2_LIST_MODE_LEN+1]; // all/sharelink
    char sort[WFM2_SORT_MODE_LEN+1];
    char dir[WFM2_SORT_TYPE_LEN+1];
    char filename[MAX_FILE_LENGTH]; // 257
    char *node;
//    WFM_FILE_LIST_AD_PARAM *ad; // for advanced filter spec
} WFM2_FILE_LIST_PARAM;

typedef struct _WFM_CLIENT_INFO {
    char username[USER_LENGTH+1];
    char ip[IP_LENGTH+1];
    char tag[WFM_FILE_STATION_LENGTH+1];
    int  uid;
} WFM_CLIENT_INFO;

typedef struct wfm2_splitNode_t {
    char *s;
    struct wfm2_splitNode_t *next;
} WFM2_SPLIT_NODE_T;

typedef struct wfm2_splitString_t{
    int count;
    int isFilter;
    WFM2_SPLIT_NODE_T *data;
} WFM2_SPLIT_TYPE_T;

typedef struct _WFM_SECTION_OBJECT{
    char    share[MAX_FILE_LENGTH];
    WFM2_SPLIT_TYPE_T split_name;
} WFM_SECTION_OBJECT;

typedef struct JSON_FILE_OBJECT{
    char filename[MAX_FILE_PATH];
    unsigned long long filesize;
    int isfolder;
    int iscompressed;
    int filetype;
    int exist;
    int mt;
    char mt_str[32];
    char privilege[5];
    char owner[USER_NAME_LENGTH];
    char group[GROUP_NAME_LENGTH];
    char type_str[12];
    WFM2_SPLIT_TYPE_T split_name;
#ifdef QBOX_SUPPORT
    int qbox_type;
    int qbox_share_id_status;
    char qbox_share_id[64];
#endif
#ifdef LIBRSYNC
    int support_versioning;
#endif
    int  mp4_240;
    int  mp4_360;
    int  mp4_480;
    int  mp4_720;
    int  mp4_1080;
    int  transcoded;
    int  play;  
    int  t;
//    MY_AV_INFO  av_info;
} JSON_FILE_OBJECT;

typedef struct WFM2_FILE_OBJECT{
    int total_obj;
    int total;
    JSON_FILE_OBJECT *fileobj;
} WFM2_FILE_OBJECT;

// String Utility
int formatString(char* strbuf, char *fmt, ...);
int check_exist(char *inode, char *real_path);
int check_user_read_write_privilege(char *user, char *path);
void replace_share_path(char *path, size_t size);
int Check_Illegal_Path(char *aPath);
void transfer_special_char(char *original, char *new_char);

/********************************************
 *               File Operation
 ********************************************/
void Cal_Size(char *sours, unsigned long *file_count, unsigned long *dir_count, unsigned long long *total_size);
void Cal_Size_Ex(char *sours, unsigned long *file_count, unsigned long *dir_count, unsigned long long *total_size, char *copy_pid);

/********************************************
                 XML Utility
 ********************************************/
void PrintXMLContent(xmlDocPtr doc, xmlNodePtr cur, char *nodeName);

void GenModel();
void GenFirmware();
void GenSpecVerionAndHostname();
void GenDemoSiteSupport();
void GenCustomLogo();

// Gen basic information
void GenGlobal();
void GenAuthPassedXML(int authPassed);
void GenAuthSidXML(char *sid);

// Support Service
void GenMultimedia();
void GenDownloadStation();
void GenMusic();
void GenVideo();
void GenPhotoStation();
void GenTunnel();
void GenPasswordConstraints();

// Basic XML print method 
void PrintXML(char *nodeName, int depth, char *fmt);

void QDoc_Begin();
void QDoc_End();
void OutputRootXML();
void GenAximSetting();

/************************************************
                   JSON Utility
 ************************************************/
int Start_JSON();
int Status_To_JSON(int type);
int Sysinfo_To_JSON(int total);
int Status_With_Pid_To_JSON(int type, int pid);
int Share_List_To_JSON(char *path, char (*share)[MAX_FILE_LENGTH], int count, int isdraggable, char (*property)[8], char (*type)[16]);
int File_List_To_JSON(JSON_FILE_OBJECT *file, int count, int total_file, int permission, int verbose);
int Status_Progress_To_JSON(int number);
int DOMAIN_IP_LIST_To_JSON(int type, DOMAIN_IP_INFO *dn_ip_info);
void update_qsync_device_status(char *user, char *ip, char *sid, char *uid,
                                int action, char *comp, char *desc, int type, int channel, int type2);
int split_sort_filename(char *aFileName, WFM2_SPLIT_TYPE_T *aSplitType);
void free_wfm2_split_data(WFM2_SPLIT_NODE_T *aNode);
int get_veto_files(VETO_INFO **veto_files);
int get_veto_files_Ex(VETO_INFO **veto_files, int hidden_file, char *user);
int wfm2_range_download(char *aFullPath, int flag);
int wfm2_getMIME(char *aFileName, char *aType);
void WFM_HTTP_Not_Modified(void);
void WFM_HTTP_Cache_Control(time_t last_modified);
int wfm2_compare_filter(int aA, int aB);
int CopyAndMove_Status_To_JSON(int type, char *th_num, char *file_name, char *file_size, char *files_cnt, int percent, int copying);

int wfm2_check_veto_files(int veto_file_count, VETO_INFO *veto_files, char *name);
int Status_With_Size_And_Counter_To_JSON(int type, char *size, char *folder, char *file);

int Status_With_Size_And_MD5_To_JSON(int type, char *size, char *md5, char *size2);

// daemon process function
void Set_Private_Profile_String(char *section, char *param, char *var, char *file);
void Get_Private_Profile_String(char *section, char *param, char *tmp, char *var, int var_size, char *file);
void Set_Private_Profile_Integer(char *section, char *param, int var, char *file);
int Get_Private_Profile_Integer(char *section, char *param, int var, char *file);
void wfm_daemon_create_report_file(char *aRemote_user, char *aStatus_file);
void wfm_daemon_delete_report_file(char *pid);
int wfm_daemon_write_report_file(char *name, unsigned long num, unsigned long files, unsigned long long size, int percent, int copying, char *copypid);

#endif
