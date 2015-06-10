
#ifndef __UPLOAD_H__
#define __UPLOAD_H__
#include "common.h"
#include "nas_cgiutil.h"
#define UPLOAD_RESULT           "result"
#define UPLOAD_PID              "upload_pid"
#define UPLOAD_TMP              "upload_tmp"
#define UPLOAD_CACHE_DIR        ".upload_cache"
int op_get_chunked_upload_id(INPUT *input);
//int op_upload(char *input);
int op_chunked_upload(INPUT *input);
int op_delete_chunked_upload_file(INPUT *input);
int op_get_chunked_upload_status(INPUT *input);


/*
int op_cancel_upload(char *input);
int op_get_chunked_upload_status(char *input);
int op_delete_chunked_upload_file(char *input);
int op_combine_upload(char *input);
int op_repository_upload(char *input);
*/


#endif

