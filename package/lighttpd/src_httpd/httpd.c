#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/stat.h>
#include "httpd.h"
#include <bcmnvram.h>
#include <utils.h>
#include <shutils.h>

#if defined (HTTPS_SUPPORT)
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#include <error.h>
#include <sys/signal.h>

#include "http_common.h"
#include "lang.h"
#include "ezp-lib.h"
#include "shared.h"

#include "fcgi_common.h"
#include "fcgi_stdio.h"

int access_permission;

unsigned long PROD_CAT_BITMAP;
int ezp_prod_subcat;
char ezp_prod_cat[4];
char ezp_prod_subsubcat[4];
char *prod_subsubcat;

void inital_prod(){
    if (nvram_get("prod_cat")) {
        strncpy(ezp_prod_cat, nvram_get("prod_cat"), sizeof(ezp_prod_cat));
    }
    else {
        FCGI_LOG("Unassigned nvram variable prod_cat\n");
        ezp_prod_cat[0] = '\0';
    }
    ezp_prod_subcat=(atoi(nvram_safe_get("prod_subcat")));
    if (nvram_get("prod_subsubcat")) {
        strncpy(ezp_prod_subsubcat, nvram_get("prod_subsubcat"),
                sizeof(ezp_prod_subsubcat));
    }
    else {
        FCGI_LOG("Unassigned nvram variable prod_subsubcat\n");
        ezp_prod_subsubcat[0] = '\0';
    }
    if (!strcmp(ezp_prod_cat, "B")) PROD_CAT_BITMAP=PROD_CAT_B;
    else if (!strcmp(ezp_prod_cat, "L")) PROD_CAT_BITMAP=PROD_CAT_L;
    else if (!strcmp(ezp_prod_cat, "M")) PROD_CAT_BITMAP=PROD_CAT_M;
    else if (!strcmp(ezp_prod_cat, "N")) PROD_CAT_BITMAP=PROD_CAT_N;
    else if (!strcmp(ezp_prod_cat, "C")) PROD_CAT_BITMAP=PROD_CAT_C;
    else if (!strcmp(ezp_prod_cat, "G")) PROD_CAT_BITMAP=PROD_CAT_G;
    else if (!strcmp(ezp_prod_cat, "I")) PROD_CAT_BITMAP=PROD_CAT_I;
    else if (!strcmp(ezp_prod_cat, "D")) PROD_CAT_BITMAP=PROD_CAT_D;
    else if (!strcmp(ezp_prod_cat, "A")) PROD_CAT_BITMAP=PROD_CAT_A;
    else if (!strcmp(ezp_prod_cat, "T")) PROD_CAT_BITMAP=PROD_CAT_T;
    else if (!strcmp(ezp_prod_cat, "V")) PROD_CAT_BITMAP=PROD_CAT_V;
    else if (!strcmp(ezp_prod_cat, "P")) PROD_CAT_BITMAP=PROD_CAT_P;
    else if (!strcmp(ezp_prod_cat, "R")) PROD_CAT_BITMAP=PROD_CAT_R;
    else if (!strcmp(ezp_prod_cat, "W")) PROD_CAT_BITMAP=PROD_CAT_W;
    else if (!strcmp(ezp_prod_cat, "K")) PROD_CAT_BITMAP=PROD_CAT_K;
    else if (!strcmp(ezp_prod_cat, "U")) PROD_CAT_BITMAP=PROD_CAT_U;
    else if (!strcmp(ezp_prod_cat, "O")) PROD_CAT_BITMAP=PROD_CAT_O;
    if ((ezp_prod_subsubcat[0]) && !strcmp(ezp_prod_subsubcat, "L")) prod_subsubcat="PROD_SUBSUBCAT_L";
    else prod_subsubcat="NOT_SET";
}

void
do_file(char *path, INPUT *input, ...)
{

    int fd = open (path, O_RDONLY);
    char buffer[4096];
    int len = 0;
    do {
        len = read(fd, buffer, sizeof(buffer));
        printf("%s", buffer);
//        FCGI_LOG("%s", buffer);
    } while(len > 0);
    close(fd); 

/*
    FILE *fp;
    fp = fopen(path, "r");
    char buf[4096];
    int len = 0;

    if(fp != NULL) {
        while(fgets(buf, sizeof(buf), fp)) {
FCGI_LOG("%s", buf);
            printf("%s", buf);
        }
        fclose(fp); 
    }*/
}

