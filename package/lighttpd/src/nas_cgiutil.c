
#include "nas_cgiutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "common.h"
#include "memdbg.h"

#define UPLOAD_RESULT           "result"
#define UPLOAD_PID              "upload_pid"
#define UPLOAD_TMP              "upload_tmp"
#define UPLOAD_CACHE_DIR        ".upload_cache"

#define BLK_SIZE                (8 * 512)

struct cgi_state *cgi;

INPUT *CGI_Find_Parameter(INPUT *input, char *name)
{
    INPUT *tmp = input;
    while(tmp){
        if(!strcmp(tmp->name, name))
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

INPUT* CGI_INIT()
{
	cgi = cgi_init();
    setup(cgi);
    load_variables(cgi);
    struct cgi_var *var;

    INPUT *tmp, *head = NULL;

    for (var = cgi->variables; var; var = var->next) {
        tmp = (INPUT*)__ax_malloc(sizeof(INPUT));
        strcpy(tmp->name, var->name);
        strcpy(tmp->val, var->value);
        tmp->next = head;
        head = tmp;
    }
    tmp = head;

    destroy(cgi);

    return head;
}

INPUT* CGI_RELEASE(INPUT* cgi) {
    INPUT *tmp = cgi, *tmp1;
    DBG("Release memory for %p", cgi);
    while(tmp) {
        tmp1 = tmp->next;
        DBG("Release memory for %p", tmp);
        __ax_free(tmp);
        tmp = tmp1;
    }
    return NULL;
}

