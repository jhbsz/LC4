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
    CAMERA_ENABLE = 0,
    CAMERA_PORT,
    CAMERA_AUTH_ENABLE,
    CAMERA_RESOLUTION,
};

static struct variable camera_variables[] = {
    {longname: "Camera Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Camera Port", argv:ARGV("1", "65535")},
    {longname: "Camera Auth_Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Camera Resolution", argv:ARGV("1280x720","640x480","320x240")},
};

int
valid_togoeye(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int
save_togoeye(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
    char tmp_enable[TMP_LEN], tmp_port[TMP_LEN], tmp_format[TMP_LEN] ,tmp_auth_enable[TMP_LEN];
    char *enable, *port, *format, *auth_enable, *user, *passwd;
    int change = 0;
    int64_t map = 0;

    ezplib_get_attr_val("camera_rule", 0, "enable", tmp_enable, 
            sizeof(tmp_enable), EZPLIB_USE_CLI); 
    enable = tmp_enable;
    ezplib_get_attr_val("camera_rule", 0, "port", tmp_port, 
            sizeof(tmp_port), EZPLIB_USE_CLI); 
    port = tmp_port;
    ezplib_get_attr_val("camera_rule", 0, "format", tmp_format, 
            sizeof(tmp_format), EZPLIB_USE_CLI); 
    format = tmp_format;
    ezplib_get_attr_val("camera_rule", 0, "auth_enable", tmp_auth_enable, 
            sizeof(tmp_auth_enable), EZPLIB_USE_CLI); 
    auth_enable = tmp_auth_enable;

    snprintf(tmp, sizeof(tmp), "user");
    user = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "passwd");
    passwd = websGetVar(wp, tmp, "");

    /* save camera_rule */
    ezplib_get_rule("camera_rule", 0, tmp, sizeof(tmp));
    snprintf(buf, sizeof(buf), "%s^%s^%s^%s^%s^%s^1" ,
            enable, port, format, auth_enable, user, passwd);

    if (strcmp(buf, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_rule("camera_rule", 0, buf);
        config_postaction(map, s, "", "");
        change = 1;
    }

    return change;
}
int
valid_camera(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "camera_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &camera_variables[CAMERA_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "camera_port");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &camera_variables[CAMERA_PORT]) == FALSE) {
		return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "camera_auth_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &camera_variables[CAMERA_AUTH_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "camera_resolution");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &camera_variables[CAMERA_RESOLUTION]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_camera(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
    char  tmp_format[TMP_LEN], tmp_user[TMP_LEN], tmp_passwd[TMP_LEN], tmp_config[TMP_LEN];
    char *enable, *port, *format, *auth_enable, *user, *passwd, *config, *resolution;
    int change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "camera_enable");
    enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "camera_port");
    port = websGetVar(wp, tmp, "");

    ezplib_get_attr_val("camera_rule", 0, "format", tmp_format, 
            sizeof(tmp_format), EZPLIB_USE_CLI); 
    format = tmp_format;

    snprintf(tmp, sizeof(tmp), "camera_auth_enable");
    auth_enable = websGetVar(wp, tmp, "");

    ezplib_get_attr_val("camera_rule", 0, "user", tmp_user,
            sizeof(tmp_user), EZPLIB_USE_CLI); 
    user = tmp_user;

    ezplib_get_attr_val("camera_rule", 0, "passwd", tmp_passwd,
            sizeof(tmp_passwd), EZPLIB_USE_CLI); 
    passwd = tmp_passwd;

    ezplib_get_attr_val("camera_rule", 0, "config", tmp_config,
            sizeof(tmp_config), EZPLIB_USE_CLI); 
    config = tmp_config;

    snprintf(tmp, sizeof(tmp), "camera_resolution");
    resolution = websGetVar(wp, tmp, "");

    /* save camera_rule */
    ezplib_get_rule("camera_rule", 0, tmp, sizeof(tmp));
    snprintf(buf, sizeof(buf), "%s^%s^%s^%s^%s^%s^%s^%s",
            enable, port, format, auth_enable, user, passwd, config, resolution);

    if (strcmp(buf, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_rule("camera_rule", 0, buf);
        config_postaction(map, s, "", "");
        change = 1;
    }

    return change;
}
