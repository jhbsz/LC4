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
    ITUNES_ENABLE = 0,
    ITUNES_PASSWD,
    ITUNES_SERVERNAME
};

static struct variable itunes_variables[] = {
    {longname: "iTunes Server Enable", argv:ARGV("0", "1"),nullok: FALSE},
    {longname: "iTunes Server Passwd", argv:ARGV("30"),nullok: TRUE},
    {longname: "iTunes Server Name", argv:ARGV("30"),nullok: FALSE},
};

int
valid_itunes(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *enable, *passwd, *servername;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "enable");
    enable = websGetVar(wp, tmp, "");

    if (valid_choice(wp, enable, &itunes_variables[ITUNES_ENABLE])
            == FALSE) {
        return FALSE;
    }

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "itunes_passwd");
    passwd = websGetVar(wp, tmp, "");

    if (valid_passwd(wp, passwd, &itunes_variables[ITUNES_PASSWD]) 
            == FALSE) {
        return FALSE;
    }

    /* Server Name */
    snprintf(tmp, sizeof(tmp), "itunes_servername" );
    servername = websGetVar(wp, tmp, "");

    if (valid_name(wp, servername, &itunes_variables[ITUNES_SERVERNAME]) 
            == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_itunes(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[TMP_LEN]; 
    char port[TMP_LEN], adminpasswd[TMP_LEN], mp3dir[TMP_LEN], runas[TMP_LEN], extensions[TMP_LEN];
    char *rule_set = "itunes_rule";
    char *enable, *passwd, *servername;
    int len, change = 0;
    int64_t map;

    map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "enable");
    enable = websGetVar(wp, tmp, "");

    if (*enable == '0') {
        ezplib_get_attr_val(rule_set, 0, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        if (strcmp(tmp, enable)) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_attr(rule_set, 0, "enable", enable);
            change = 1;
        }
    } else {

        /* Passwd */
        snprintf(tmp, sizeof(tmp), "itunes_passwd");
        passwd = websGetVar(wp, tmp, "");

        /* Server Name */
        snprintf(tmp, sizeof(tmp), "itunes_servername");
        servername = websGetVar(wp, tmp, "");
       
        ezplib_get_attr_val(rule_set, 0, "port", port, sizeof(port),
            EZPLIB_USE_CLI);

        ezplib_get_attr_val(rule_set, 0, "adminpasswd", adminpasswd, sizeof(adminpasswd),
            EZPLIB_USE_CLI);

        ezplib_get_attr_val(rule_set, 0, "mp3dir", mp3dir, sizeof(mp3dir),
                            EZPLIB_USE_CLI);

        ezplib_get_attr_val(rule_set, 0, "runas", runas, sizeof(runas),
                                    EZPLIB_USE_CLI);

        ezplib_get_attr_val(rule_set, 0, "extensions", extensions, sizeof(extensions),
                                            EZPLIB_USE_CLI);

        len =
            snprintf(buf, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s", 
                     enable, port, adminpasswd, mp3dir, servername, runas, passwd, extensions);

        if (len >= TMP_LEN) {
            return 0;
        }

        ezplib_get_rule(rule_set, 0, tmp, TMP_LEN);
        if (strcmp(buf, tmp)) {
            snprintf(tmp, TMP_LEN, "NUM=0");
            config_preaction(&map, v, s, tmp, "");
            ezplib_replace_rule(rule_set, 0, buf);
            change = 1;
        }
    }

    if (change) {
        snprintf(tmp, TMP_LEN, "NUM=0");
        config_postaction(map, s, tmp, "");
    }

    return change;
}

int
ej_itunes_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    do_ej("itunes_setting.asp", wp, "", "");
    return 0;
}
