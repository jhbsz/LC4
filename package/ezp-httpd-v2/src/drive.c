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
    DRIVE_ENABLE = 0,
    DRIVE_GUEST,
    DRIVE_PORT,
    DRIVE_EXT_ACCESS,
};

static struct variable drive_variables[] = {
    {longname: "Drive Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Drive Guset", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Drive Port", argv:ARGV("1", "65535")},
    {longname: "Drive External Access", argv:ARGV("0", "1"), nullok: FALSE},
};

int
valid_togodrive(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int
save_togodrive(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
    char tmp_enable[TMP_LEN], tmp_port[TMP_LEN], tmp_guest[TMP_LEN] ,tmp_ext_access[TMP_LEN];
    char *enable, *port, *guest, *ext_access, *name, *password;
    int change = 0;
    int64_t map = 0;

    ezplib_get_attr_val("drive_rule", 0, "enable", tmp_enable, 
            sizeof(tmp_enable), EZPLIB_USE_CLI); 
    enable = tmp_enable;
    ezplib_get_attr_val("drive_rule", 0, "port", tmp_port, 
            sizeof(tmp_port), EZPLIB_USE_CLI); 
    port = tmp_port;
    ezplib_get_attr_val("drive_rule", 0, "guest", tmp_guest, 
            sizeof(tmp_guest), EZPLIB_USE_CLI); 
    guest = tmp_guest;
    ezplib_get_attr_val("drive_rule", 0, "ext_access", tmp_ext_access, 
            sizeof(tmp_ext_access), EZPLIB_USE_CLI); 
    ext_access = tmp_ext_access;

    snprintf(tmp, sizeof(tmp), "user");
    name = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "passwd");
    password = websGetVar(wp, tmp, "");

    /* save drive_rule */
    ezplib_get_rule("drive_rule", 0, tmp, sizeof(tmp));
    snprintf(buf, sizeof(buf), "%s^%s^%s^%s^%s^%s^1" ,
            enable, guest, port, ext_access, name, password);

    if (strcmp(buf, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_rule("drive_rule", 0, buf);
        config_postaction(map, s, "", "");
        change = 1;
    }

    return change;
}

int
valid_drive(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "drive_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &drive_variables[DRIVE_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "drive_guest");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &drive_variables[DRIVE_GUEST]) == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "drive_port");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &drive_variables[DRIVE_PORT]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "drive_ext_access");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &drive_variables[DRIVE_EXT_ACCESS]) == FALSE) {
        return FALSE;
    }
    
    return TRUE;
}

int
save_drive(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
    char  tmp_name[TMP_LEN], tmp_password[TMP_LEN], tmp_config[TMP_LEN];
    char *enable, *guest, *port, *ext_access, *name, *password, *config;
    int change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "drive_enable");
    enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "drive_guest");
    guest = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "drive_port");
    port = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "drive_ext_access");
    ext_access = websGetVar(wp, tmp, "");

    ezplib_get_attr_val("drive_rule", 0, "name", tmp_name,
            sizeof(tmp_name), EZPLIB_USE_CLI); 
    name = tmp_name;

    ezplib_get_attr_val("drive_rule", 0, "password", tmp_password,
            sizeof(tmp_password), EZPLIB_USE_CLI); 
    password = tmp_password;

    ezplib_get_attr_val("drive_rule", 0, "config", tmp_config,
            sizeof(tmp_config), EZPLIB_USE_CLI); 
    config = tmp_config;

    /* save drive_rule */
    ezplib_get_rule("drive_rule", 0, tmp, sizeof(tmp));
    snprintf(buf, sizeof(buf), "%s^%s^%s^%s^%s^%s^%s" ,
            enable, guest, port, ext_access, name, password, config);

    if (strcmp(buf, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_rule("drive_rule", 0, buf);
        config_postaction(map, s, "", "");
        change = 1;
    }

    return change;
}
