#ifndef __SEARCH_H__
#define __SEARCH_H__
#include "fcgi_common.h"
#include "fcgi_nas_cgiutil.h"

int op_search(INPUT *input);

#define TYPE_FILE   1
#define TYPE_FOLDER 2
#define TYPE_SLINK  3

#define tagFileInfo "fileInfo"
#define tagFileName     "name"
#define tagFilePermission   "permission"
#define tagFileSize     "size"
#define tagFileType     "type"
#define tagFileMtime        "mtime"
#define tagFileOwner        "owner"
#define tagFileGroup        "group"

#define tagTotal        "totalMatch"
#endif

