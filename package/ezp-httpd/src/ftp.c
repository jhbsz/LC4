#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include "assert.h"

#include "common.h"
#include "ezp-lib.h"
#include "ezp.h"

enum {
    FTPD_ENABLE = 0,
    FTPD_PORT,
    FTPD_BANDWIDTH,
    FTPD_USERNAME,
    FTPD_PASSWD,
    FTPD_UPLOAD,
    FTPD_DOWNLOAD,
    FTPD_EXT_ACCESS,
};

static struct variable ftpd_variables[] = {
    {longname: "FTPD Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "FTPD Port", argv:ARGV("1","65535"), nullok: FALSE},
    {longname: "FTPD Bandwidth", argv:ARGV(""), nullok: FALSE},
    {longname: "FTPD User Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "FTPD Password", argv:ARGV("20"), nullok: FALSE},
    {longname: "FTPD Upload rate", argv:ARGV(""), nullok: TRUE},
    {longname: "FTPD Download rate", argv:ARGV(""), nullok: TRUE},
    {longname: "FTPD Allow External Access", argv:ARGV("0", "1"), nullok: FALSE}
};

int
valid_ftpd(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int i, nrule;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "ftpd_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &ftpd_variables[FTPD_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "ftpd_port");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &ftpd_variables[FTPD_PORT]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "ftpd_bandwidth");
    val = websGetVar(wp, tmp, "");
    if (valid_number(wp, val, &ftpd_variables[FTPD_BANDWIDTH]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "ftpd_allow_ext_access");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &ftpd_variables[FTPD_EXT_ACCESS]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "ftpd_user_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_username", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &ftpd_variables[FTPD_USERNAME]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_passwd", i);
        val = websGetVar(wp, tmp, "");
        /* The password is shown as a public key for the administrator. */
        if (valid_name(wp, val, &ftpd_variables[FTPD_PASSWD]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_upload", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &ftpd_variables[FTPD_UPLOAD]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_download", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &ftpd_variables[FTPD_DOWNLOAD]) == FALSE) {
            return FALSE;
        }
    }
    
    return TRUE;
}

int
save_ftpd(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[LONG_BUF_LEN];    
    char all_rule[LONG_BUF_LEN];
    char *enable, *port, *bandwidth, *username, *passwd,
         *upload, *download, *ipaddr, *allow_ext_access;
    int change =0, nrule, max, i, len;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "ftpd_enable");
    enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "ftpd_port");
    port = websGetVar(wp, tmp, "");
    
    snprintf(tmp, sizeof(tmp), "ftpd_bandwidth");
    bandwidth = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "ftpd_allow_ext_access");
    allow_ext_access = websGetVar(wp, tmp, "");
    
    ezplib_get_attr_val("ftpd_rule", 0, "enable", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if (strcmp(enable, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_attr("ftpd_rule", 0, "enable", enable);
        change = 1;
    }

    ezplib_get_attr_val("ftpd_rule", 0, "port", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if (strcmp(port, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_attr("ftpd_rule", 0, "port", port);
        change = 1;
    }

    ezplib_get_attr_val("ftpd_rule", 0, "userbandwidth", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if (strcmp(bandwidth, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_attr("ftpd_rule", 0, "userbandwidth", bandwidth);
        change = 1;
    }

    ezplib_get_attr_val("ftpd_rule", 0, "allow_ext_access", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if (strcmp(bandwidth, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_attr("ftpd_rule", 0, "allow_ext_access", allow_ext_access);
        change = 1;
    }

    nrule = atoi(websGetVar(wp, "ftpd_user_rule_num", "0"));
    max = atoi(nvram_safe_get("ftpd_user_rule_max"));
    nrule = (nrule >= max) ? max : nrule;
    if (nrule != atoi(nvram_safe_get("ftpd_user_rule_num"))) {
        config_preaction(&map, v, s, "NUM=0", "");
        snprintf(tmp, sizeof(tmp), "%d", nrule);
        nvram_set("ftpd_user_rule_num", tmp);
        change = 1;
    }
    
    all_rule[0]='\0';
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_username", i);
        username = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_passwd", i);
        passwd= websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_upload", i);
        upload= websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_download", i);
        download= websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "ftpd_user_rule%d_ipaddr", i);
        ipaddr = websGetVar(wp, tmp, "");

        len = snprintf(buf, LONG_BUF_LEN, "%s^%s^%s^%s^%s^%s", 
                       enable, username, passwd, upload, download, ipaddr);
        if (len >= LONG_BUF_LEN) {
            return 0;
        }

        if (ezplib_get_rule("ftpd_user_rule", i, tmp, LONG_BUF_LEN) < 0) {
            /* i-th rule doesn't exist */
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_append_rule("ftpd_user_rule", buf);
            change = 1;
        } else if (strcmp(tmp, buf)) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule("ftpd_user_rule", i, buf);
            change = 1;
        }
        if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", buf);
        else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, buf);
    }
    nvram_set("ftpd_user_rule", all_rule);

    if (change) {
        snprintf(tmp, LONG_BUF_LEN, "NUM=%d", i);
        /* Not defined the postaction yet. */
        config_postaction(map, s, tmp, "");
    }

    return change;
}
