#ifndef __FILE_OP_H__
#define __FILE_OP_H__
#include "fcgi_common.h"
#include "fcgi_nas_cgiutil.h"

#ifndef BLK_SIZE
#define BLK_SIZE        (8 * 512)
#endif
//#define   WFM2_COPY_STATUS    "/tmp/wfm2_copy_status"
#define WFM2_MLQ_STATUS     "/tmp/wfm2_mlq_status"
//#define   WFM_COPY_MOVE_LIST  "/tmp/wfm_copy_move.list"
//#define   WFM_LOCK_COPY_MOVE  "/tmp/wfm_copy_move.lck"

#define TRANSCODE_FILE                  "transcode_file"
#define TRANSCODE_FILE_CNT              "transcode_count"
#define TRANSCODE_TRANSCODEING_STATUS   "transcodeing"

#define SHARE_DATA_PATH_LEN 15

#define MIN_THUMB_SIZE  100
#define DEFAULT_THUMB_SIZE  400 
#define DEFAULT_THUMB_SIZE_OLD  320
#define DEFAULT_REAL_THUMB_SIZE 500
#define MAX_THUMB_SIZE  800
#define THUMB_FOLDER_NAME       ".@__thumb"
#define THUMB_FOLDER_NAME_LEN   9
#define MIN_THUMB_PREFIX        "s100"
#define MAX_THUMB_PREFIX        "s800"
#define DEFAULT_THUMB_PREFIX    "default"
#define MAX_COPY_DUP_NUM    100000
#define COPY_WRITE_RECORD_STARTSIZE     10485670    // 1024*1024*10 // 10MB
#define COPY_WRITE_RECORD_BLOCKSIZE     131072      // 1024*128     // 128K
#define COPY_WRITE_RECORD_TIMES         80          // update progress every COPY_WRITE_RECORD_TIMES * COPY_WRITE_RECORD_BLOCKSIZE

typedef struct stat     WFM_STAT;

/* return status */
typedef enum {
    E_SRC = -100,
    E_DEST,
    E_COPY
} FILE_STATUS_TYPE;

void tran_path(char *path);
/* get thumbnail */
int op_get_thumbnail(INPUT *input);

int op_move(INPUT *input);
int op_createdir(INPUT *input);
int op_copy(INPUT *input);
int _copy_file(const char *src, const char *dest, int option);
int _copy_dir(char *dir,char *parent,char *dest, int option);
int op_delete(INPUT *input);
int op_rename(INPUT *input);
int op_cancel_move_or_copy(INPUT *input);
/* Get extract, copy and move process function */
int op_get_copy_and_move_status(INPUT *input);
int op_get_file_size(INPUT *input);
unsigned long long _get_folder_size(char *name);
#endif
