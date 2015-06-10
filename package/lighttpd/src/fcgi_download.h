#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__
#include "fcgi_common.h"
#include "fcgi_nas_cgiutil.h"
int op_download(INPUT *input);
int _download(char *full_path, char *download_file, int compress, int multiple, long long offset);
int op_viewer(INPUT *input);
//#define M2_MULTIFILE_DOWNLOAD
int _download_urlenc(char *full_path, char *download_file, char *url_encoded_path, int compress, int multiple, long long offset);
#endif
