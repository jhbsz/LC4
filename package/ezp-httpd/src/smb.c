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
    SMBD_ENABLE = 0,
    SMBD_SHARE_NAME,
    SMBD_READ_ONLY,
    SMBD_GUEST_OK,
    SMBD_PASSWD,
    SMBD_EXT_ACC,
    SMBD_CODEPAGE
};

static struct variable smbd_variables[] = {
    {longname: "SMBD Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "SMBD SHARE NAME", argv:ARGV("30"), nullok: FALSE},
    {longname: "SMBD READ ONLY", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "SMBD GUEST OK", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "SMBD PASSWD", argv:ARGV("30"), nullok: FALSE},
    {longname: "SMBD EXTERNAL ACCESS", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "SMBD CODEPAGE", argv:ARGV("30"), nullok: FALSE},
};

int
valid_smbd(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "smbd_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &smbd_variables[SMBD_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "smbd_share_name");
    val = websGetVar(wp, tmp, "");
    if (valid_passwd(wp, val, &smbd_variables[SMBD_SHARE_NAME]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "smbd_read_only");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &smbd_variables[SMBD_READ_ONLY]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "smbd_guest_ok");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &smbd_variables[SMBD_GUEST_OK]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "smbd_ext_access");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &smbd_variables[SMBD_EXT_ACC]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "smbd_passwd");
    val = websGetVar(wp, tmp, "");
    if (valid_passwd(wp, val, &smbd_variables[SMBD_PASSWD]) == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "smbd_codepage");
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &smbd_variables[SMBD_CODEPAGE]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

int
save_smbd(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[LONG_BUF_LEN], tmp2[LONG_BUF_LEN];    
    char *enable, *share_name, *read_only, *guest_ok, *passwd, *ext_access;
    char *codepage;
    int change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "smbd_enable");
    enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "smbd_share_name");
    share_name = websGetVar(wp, tmp, "");
    
    snprintf(tmp, sizeof(tmp), "smbd_read_only");
    read_only = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "smbd_guest_ok");
    guest_ok = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "smbd_ext_access");
    ext_access = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "smbd_passwd");
    passwd = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "smbd_codepage");
    codepage = websGetVar(wp, tmp, "");

    if (!strcmp(passwd, TMP_PASSWD)) {
        ezplib_get_attr_val("smbd_user_rule", 0, "passwd", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        strcpy(passwd, tmp);
    }
    /* save smbd_rule */
    ezplib_get_subrule("smbd_rule", 0, 1, 5, tmp, sizeof(tmp));
    ezplib_get_rule("smbd_rule", 0, tmp2, sizeof(tmp2));
    snprintf(buf, sizeof(buf), "%s^%s^^%s^sda1^^%s^%s^%s^%s" ,
            enable, tmp, share_name, read_only, guest_ok,
            ext_access, codepage);

    if (strcmp(buf, tmp2)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_rule("smbd_rule", 0, buf);
        change = 1;
    }
    /* save smbd_user_rule */
    ezplib_get_attr_val("smbd_user_rule", 0, "passwd", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);

    if (strcmp(passwd, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_attr("smbd_user_rule", 0, "passwd", passwd);
        change = 1;
    }
    
    if (change) {
        snprintf(tmp, LONG_BUF_LEN, "NUM=0");
        /* Not defined the postaction yet. */
        config_postaction(map, s, tmp, "");
    }

    return change;
}
