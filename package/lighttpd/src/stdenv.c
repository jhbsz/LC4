#define _GNU_SOURCE
#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fcgi_common.h"
#include "ezp.h"

extern char **environ;
int count;
extern int qsync_prepare(INPUT *input);
extern int sys_request(INPUT *input);
extern int fcgi_handle_auth_login(INPUT *input);
extern int util_request(INPUT *input);
extern int sysinfo_req(INPUT *input);
/*
static int url_from_myqnapcloud = 0;

int check_request_from_myqnapcloud(void) {
    return url_from_myqnapcloud;

}

void set_from_myqnapcloud(int frommyq){
    url_from_myqnapcloud = frommyq;
}
*/
typedef enum _QCGI_Query_Cmd_ID {
    E_CMD_AUTHLOGIN,
    E_CMD_UTILREQUEST,
    E_CMD_SYSINFOREQUEST,
    E_CMD_GETTREE,
    E_CMD_GETLIST,
    E_CMD_SENDFILE,
    E_CMD_QSYNCPREPARE,
    E_CMD_SYSREQUEST,
    E_NUM_QFILE_CMDS,
    E_CMD_INVALID = 0xFF
} QCGI_Query_Cmd_ID;

typedef struct _QCGI_Query_Cmd {
    char *key;
    QCGI_Query_Cmd_ID id;
} QCGI_Query_Cmd;

QCGI_Query_Cmd qfile_query_cmds[] = {
    {"authLogin", E_CMD_AUTHLOGIN},
    {"utilRequest", E_CMD_UTILREQUEST},
    {"sysinfoReq", E_CMD_SYSINFOREQUEST},
    {"sendfile", E_CMD_SENDFILE},
    {"qsyncPrepare", E_CMD_QSYNCPREPARE},
    {"sysRequest", E_CMD_SYSREQUEST},
    {NULL, E_CMD_INVALID},
};

void sendfile(INPUT *input) {
    printf("X-Sendfile2: /tmp/mnt/fslite/Videos/VIDEO0003.mp4 342920000-343921000\r\n"
            "\r\n"
            "X-Sendfile2: /tmp/mnt/fslite/USB/test.html \n");
}

static QCGI_Query_Cmd_ID query_string_mux(void) {
    int i;
    char *query_string = NULL;
    set_from_myqnapcloud(0);
    for(i = 0; environ[i] != NULL; i++) {
        if(!strncmp(environ[i], "REQUEST_URI", (sizeof("REQUEST_URI") - 1))) {
            query_string = environ[i];
            break;
        } else if(!strncmp(environ[i], "SERVER_NAME", (sizeof("SERVER_NAME") - 1)) && strcasestr(environ[i], "myqnapcloud")) {
            set_from_myqnapcloud(1);
        }
    }
    if(!environ[i] || !query_string)
        return E_CMD_INVALID;
    for(i = 0; qfile_query_cmds[i].id != E_CMD_INVALID;i ++) {
        if(strstr(query_string, qfile_query_cmds[i].key)) {
            return qfile_query_cmds[i].id;
        }
    }
    return E_CMD_INVALID;
}

int main(void)
{
    QCGI_Query_Cmd_ID qcmd_id;

    /* Response loop. */
    while (FCGI_Accept() >= 0)   {
        INPUT *input = CGI_INIT();
        qcmd_id = query_string_mux();
        switch(qcmd_id) {
            case E_CMD_AUTHLOGIN:
                fcgi_handle_auth_login(input);
                break;
            case E_CMD_UTILREQUEST:
                util_request(input);
                break;
            case E_CMD_SYSINFOREQUEST:
                sysinfo_req(input);
                break;
            case E_CMD_SENDFILE:
                sendfile(input);
                break;
            case E_CMD_QSYNCPREPARE:
                qsync_prepare(input);
                break;
            case E_CMD_SYSREQUEST:
                sys_request(input);
                break;
            default:
                printf("<i>Under construction</i>\n");
                FCGI_LOG("Unimplemented command\n");
                break;
        }
        /*
        printf("Content-type: text/html\r\n"
                "\r\n"
                "<title>FastCGI Hello! (C, fcgi_stdio library)</title>"
                "<h1>FastCGI Hello! (C, fcgi_stdio library)</h1>"
                "Request number %d running on host <br/>\n",
                ++count);
        for(i = 0; ((envvar = environ[i]) != NULL);i ++) {
            printf("<i>%s</i><br/>\n", envvar);
        }
        */
    }
    return 0;
}
