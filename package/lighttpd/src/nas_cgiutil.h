#ifndef _NAS_CGIUTIL_H
#define _NAS_CGIUTIL_H

#include "cgi.h"

#define INPUT_SIZE 256
typedef struct tagINPUT{
    char name[INPUT_SIZE];
    char val[INPUT_SIZE];
    struct tagINPUT *next;
}INPUT;

INPUT *CGI_Find_Parameter(INPUT *input, char *name);
INPUT* CGI_INIT();
INPUT* CGI_RELEASE(INPUT* cgi);
#endif /* _NAS_CGIUTIL_H */
