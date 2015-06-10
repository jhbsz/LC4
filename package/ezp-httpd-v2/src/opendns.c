#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <time.h>

#include "common.h"
#include "lang.h"
#include "ezp.h"
#include "ezp-lib.h"
#include "common.h"

#define DHCP_MAX_COUNT 254

enum {
    OPENDNS_ENABLE = 0,
    OPENDNS_USERNAME,
    OPENDNS_PASSWORD,
    OPENDNS_REDIRECT,
    OPENDNS_LABEL,
};

static struct variable opendns_variables[] = {
    {longname: "OpenDNS Enable", argv:ARGV("0","1"), nullok: FALSE},
    {longname: "OpenDNS UserName", argv:ARGV("30"), nullok: FALSE},
    {longname: "OpenDNS Password", argv:ARGV("30"), nullok: FALSE},
    {longname: "OpenDNS Redirect",  argv:ARGV("0","1"), nullok: FALSE},
    {longname: "OpenDNS Label",  argv:ARGV("30"), nullok: FALSE},
};

int
valid_opendns(webs_t wp, char *value, struct variable *v)
{
    char *cur = nvram_safe_get("wan_num");
    char tmp[TMP_LEN];
    char *enable, *username, *passwd, *redirect, *label;
    int wan_num;
    int i;

    if (!*cur) {
        wan_num = 1;
    } else {
        wan_num = atoi(cur);
    }

    for (i = 0; i < wan_num; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "opendns%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, &opendns_variables[OPENDNS_ENABLE]) == FALSE) {
           return FALSE;
        }

        if (*enable == '0') {
            continue;
        }

        /* UserName */
        snprintf(tmp, sizeof(tmp), "opendns%d_username", i);
        username = websGetVar(wp, tmp, "");
        if (valid_name(wp, username, &opendns_variables[OPENDNS_USERNAME]) == FALSE) {
           return FALSE;
        }

        /* Password */
        snprintf(tmp, sizeof(tmp), "opendns%d_passwd", i);
        passwd = websGetVar(wp, tmp, "");
        if (valid_name(wp, passwd, &opendns_variables[OPENDNS_PASSWORD]) == FALSE) {
           return FALSE;
        }

        /* OpenDNS Redirect */
        snprintf(tmp, sizeof(tmp), "opendns%d_redirect", i);
        redirect = websGetVar(wp, tmp, "");
        if (valid_choice(wp, redirect, &opendns_variables[OPENDNS_REDIRECT]) == FALSE) {
           return FALSE;
        }

        /* Label */
        snprintf(tmp, sizeof(tmp), "opendns%d_label", i);
        label = websGetVar(wp, tmp, "");
        if (valid_name(wp, label, &opendns_variables[OPENDNS_LABEL]) == FALSE) {
           return FALSE;
        }

    }

    return TRUE;

}

int 
save_opendns(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *rule_set = "wan_opendns_rule";
    char *rule_num = nvram_safe_get("wan_num"); 
    char tmp[TMP_LEN];
    char buf[TMP_LEN];
    char *enable, *username, *passwd, *redirect, *label;
    int wan_num, i, len, change = 0;
    int64_t map;

    if (!*rule_num) {
        wan_num = 1;
    } else {
        wan_num = atoi(rule_num);
    }

    for (i = 0; i < wan_num; i++) {
        map = 0;

        /* Enable */
        snprintf(tmp, sizeof(tmp), "opendns%d_enable", i);
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
            /* UserName */
            snprintf(tmp, sizeof(tmp), "opendns%d_username", i);
            username = websGetVar(wp, tmp, "");

            /* Password */
            
            snprintf(tmp, sizeof(tmp), "opendns%d_passwd", i);
            passwd = websGetVar(wp, tmp, "");
            
            /* OpendDNS Redirect */
            snprintf(tmp, sizeof(tmp), "opendns%d_redirect", i);
            redirect = websGetVar(wp, tmp, "");

            /* OpendDNS Label */
            snprintf(tmp, sizeof(tmp), "opendns%d_label", i);
            label = websGetVar(wp, tmp, "");

            len = 
                snprintf(buf, TMP_LEN,
                         "WAN%d_OPENDNS^%s^%s^%s^%s^%s", i+1,
                         enable, username, passwd, redirect, label);

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

        if (change) {
            snprintf(tmp, TMP_LEN, "NUM=%d", i);
            config_postaction(map, s, tmp, "");
        }
    }

    return change;
}


int 
ej_opendns_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
#ifdef EZP_PROD_BRAND_PROX
    for (i = num-1; i >=0; i--) {
#else
    for (i = 0; i < num; i++) {
#endif
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("opendns_setting.asp", wp, buf, "");
    }

    return 0;
}
