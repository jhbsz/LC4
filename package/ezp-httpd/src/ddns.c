#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include "common.h"
#include "ezp.h"
#include "ezp-lib.h"

enum {
    DDNS_ENABLE = 0,
    DDNS_TYPE,
    DDNS_USERNAME,
    DDNS_PASSWD,
    DDNS_HOSTNAME,
    DDNS_SERVER
};

static struct variable wan_ddns_variables[] = {
    {longname: "DDNS Enable", argv:ARGV("0", "1")},
    {longname: "DDNS Type",
        argv:ARGV("dyndns","tzo","zoneedit","changeip","changeip","eurodns",
                  "noip","ods","ovh","regfish")},
    {longname: "DDNS User Name", argv:ARGV("30")},
    {longname: "DDNS Passwd", argv:ARGV("30")},
    {longname: "DDNS Host Name", argv:ARGV("128")},
    {longname: "DDNS Custom Server", argv:ARGV("30"), nullok: TRUE},
};

int
valid_ddns(webs_t wp, char *value, struct variable *v)
{
    char *cur = nvram_safe_get("wan_num");  // Corresponding to wan_num
    char tmp[TMP_LEN];
    char *enable, *type, *username, *passwd, *hostname, *server;
    int num;
    int i;

    return TRUE;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "ddns%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        if (valid_choice(wp, enable, &wan_ddns_variables[DDNS_ENABLE])
                                                                == FALSE) {
           return FALSE;
        }

        if (enable[0] == '0') {
            /* Validate the following entities only when DDNS is enabled. */
            continue;
        }

        /* Type */
        snprintf(tmp, sizeof(tmp), "ddns%d_type", i);
        type = websGetVar(wp, tmp, "");

        if (valid_choice(wp, type, &wan_ddns_variables[DDNS_TYPE]) 
                                                                == FALSE) {
           return FALSE;
        }

        /* User Name */
        /*
        snprintf(tmp, sizeof(tmp), "ddns%d_username", i);
        username = websGetVar(wp, tmp, "");

        if (valid_name(wp, username, &wan_ddns_variables[DDNS_USERNAME]) 
                                                                == FALSE) {
           return FALSE;
        }
        */
        /* Passwd */
        /*
        snprintf(tmp, sizeof(tmp), "ddns%d_passwd", i);
        passwd = websGetVar(wp, tmp, "");

        if (valid_passwd(wp, passwd, &wan_ddns_variables[DDNS_PASSWD]) 
                                                                == FALSE) {
           return FALSE;
        }
        */
        /* Host Name */
	/*
        snprintf(tmp, sizeof(tmp), "ddns%d_hostname", i);
        hostname = websGetVar(wp, tmp, "");

        if (valid_name(wp, hostname, &wan_ddns_variables[DDNS_HOSTNAME]) 
                                                                == FALSE) {
           return FALSE;
        }
*/
        /* Custom Server */
        /*
        snprintf(tmp, sizeof(tmp), "ddns%d_server", i);
        server = websGetVar(wp, tmp, "");

        if (valid_name(wp, server, &wan_ddns_variables[DDNS_SERVER]) 
                                                                == FALSE) {
           return FALSE;
        }
        */
    }

    return TRUE;
}

int
save_ddns(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *rule_num = nvram_safe_get("wan_num");  // Corresponding to wan_num
    char buf[LONG_BUF_LEN], tmp[TMP_LEN];
    char *rule_set = "wan_ddns_rule";
    char *enable, *type, *username, *passwd, *hostname, *server;
    int num, i, len, change = 0;
    int64_t map;

    if (!*rule_num) {
        num = 1;
    } else {
        num = atoi(rule_num);
    }
    printf("%s(%d)\n",__func__,__LINE__);

    for (i = 0; i < num; i++) {
        map = 0;

        /* Enable */
        snprintf(tmp, sizeof(tmp), "ddns%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        if (*enable == '0') {
            ezplib_get_attr_val(rule_set, i, "enable", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            if (strcmp(tmp, enable)) {
                snprintf(tmp, TMP_LEN, "NUM=%d", i);
                config_preaction(&map, v, s, tmp, "");
                ezplib_replace_attr(rule_set, i, "enable", enable);
                change = 1;
            }
        } else {
            /* Type */
            snprintf(tmp, sizeof(tmp), "ddns%d_type", i);
            type = websGetVar(wp, tmp, "");
    printf("%s(%d) %s\n",__func__,__LINE__,type);

            /* User Name */
            snprintf(tmp, sizeof(tmp), "ddns%d_username", i);
            username = websGetVar(wp, tmp, "");
    printf("%s(%d) %s\n",__func__,__LINE__,username);

            /* Passwd */
            snprintf(tmp, sizeof(tmp), "ddns%d_passwd", i);
            passwd = websGetVar(wp, tmp, "");
    printf("%s(%d) %s\n",__func__,__LINE__,passwd);

            /* Host Name */
            snprintf(tmp, sizeof(tmp), "ddns%d_hostname", i);
            hostname = websGetVar(wp, tmp, "");
    printf("%s(%d) %s\n",__func__,__LINE__,hostname);

            /* Custom Server */
            snprintf(tmp, sizeof(tmp), "ddns%d_server", i);
            server = websGetVar(wp, tmp, "");
    printf("%s(%d) %s\n",__func__,__LINE__,server);
            if (atoi(server) < 10 )strcpy(server,"10");
            
    printf("%s(%d)\n",__func__,__LINE__);
            len =
                snprintf(buf, TMP_LEN, "WAN%d_DDNS^%s^%s^%s^%s^%s^%s", 
                         i+1, enable, type, username, passwd, hostname, server);

    printf("%s(%d) %s\n",__func__,__LINE__,buf);
            if (len >= TMP_LEN) {
                return 0;
            }
           
            ezplib_get_rule(rule_set, i, tmp, TMP_LEN);
            if (strcmp(buf, tmp)) {
                snprintf(tmp, TMP_LEN, "NUM=%d", i);
                config_preaction(&map, v, s, tmp, "");
                ezplib_replace_rule(rule_set, i, buf);
                change = 1;
            }
        }

        change = 1;
        if (change) {
            snprintf(tmp, TMP_LEN, "NUM=%d", i);
            config_postaction(map, s, tmp, "");
        }
    }

    return change;
}

int
ej_ddns_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");  // Corresponding to wan_num
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

#define BUF_LEN 32


#ifdef EZP_PROD_BRAND_PROX
    for (i = num-1; i >= 0; i--) {
#else
    for (i = 0; i < num; i++) {
#endif
        char buf[BUF_LEN];
        snprintf(buf, BUF_LEN, "%d", i);
        do_ej("ddns_setting.asp", wp, buf, "");
    }

    return 0;
}
