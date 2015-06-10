#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__
#include "common.h"
#include "nas_cgiutil.h"
int op_download(INPUT *input);
int _download(char *full_path, char *download_file, int compress, int multiple, long long offset);
int op_viewer(INPUT *input);
#endif
