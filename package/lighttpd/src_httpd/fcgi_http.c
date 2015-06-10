#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fcgi_common.h"
#include "lang.h"
#include <bcmnvram.h>
#include "ezp-lib.h"
#include "ezp.h"
#include "shared.h"
#include <stdbool.h>

extern char **environ;
extern int curr_auth_class;
int count;

static void getCookie(char * cookie, char * name, char * value)
{
    char tar[128], field[128];
    char *pos_S;
    strcpy(tar, name);
    strcat(tar, "=");
    if ((pos_S = strstr(cookie, tar)) != NULL) {
        sscanf(pos_S, "%[^;]", field);
        strcpy(value, field + strlen(tar));
    }
}

static void parse_cookie_info(char * cookie, char * auth)
{
    if (strlen(cookie) == 0)
        return;

    getCookie(cookie, "auth", auth);
    unescape(auth);
}

int check_user_permission(char *user, char *pass, char *authinfo) {
    int u_len = strlen(user);
    int p_len = strlen(pass);
    int user_ok = 0, pass_ok = 0;
    char b64_user[TMP_LEN] = {0};
    char b64_pass[TMP_LEN] = {0};
    int user_rule_num = 2;
    int ret_class = 0;
    unsigned long l = 128;
    if(!strncmp(user, authinfo, u_len) && (authinfo[u_len] == ':')) {
        user_ok = 1;
    } else {
        base64_decode((unsigned char *)user, u_len,
                (unsigned char *)b64_user, &l);
        if(!strncmp(b64_user, authinfo, l) && (authinfo[l] == ':')) {
            user_ok = 2;
            u_len = l;
        }
    }
    if(user_ok) {
        if(!strcmp(pass, authinfo + u_len + 1)) {
            pass_ok = 1;
        } else {
            base64_decode((unsigned char *)pass, p_len,
                    (unsigned char *)b64_pass, &l);
            if(!strcmp(b64_pass, authinfo + u_len + 1))
                pass_ok = 2;
        }
    }
    if(user_ok && pass_ok) {
        ret_class = 3;
        for(;user_rule_num > 0;user_rule_num--) {
            ezplib_get_attr_val("user_rule", user_rule_num - 1, "user", b64_pass, sizeof(b64_pass), EZPLIB_USE_CLI);
            if((user_ok == 1 && !strcmp(user, b64_pass)) || (user_ok == 2 && !strcmp(b64_user, b64_pass))) {
                ezplib_get_attr_val("user_rule", user_rule_num - 1, "permission_class", b64_pass, sizeof(b64_pass), EZPLIB_USE_CLI);
                ret_class = atoi(b64_pass);
                break;
            }

        }
    }
    return ret_class;
}

void get_admin_user_pass(char *user, char *pass) {
    ezplib_get_attr_val("http_rule", 0, "username", user, TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("http_rule", 0, "passwd", pass, TMP_LEN, EZPLIB_USE_CLI);
}

void get_togo_user_pass(char *user, char *pass) {
    strcpy(user, "togo");
    ezplib_get_attr_val("lan_hwaddr_rule_default", 0, "hwaddr", pass, TMP_LEN, EZPLIB_USE_CLI);
}

static inline bool check_guest_allow(void) {
    char *guest_allow = nvram_get("allow_guest");
    if(guest_allow && (guest_allow[0] == '0')) {
        return false;
    }
    return true;
}

int check_access_permission(void) {
    int i;
    char *cookiestr = NULL;
    int max_perm_class = 0;

    char buf_user[TMP_LEN] = {0};
    char buf_pass[TMP_LEN] = {0};
    char authinfo[TMP_LEN] = {0};
    unsigned long l = 128;
    
    for(i = 0; environ[i] != NULL; i++) {
        if(!strncmp(environ[i],"HTTP_COOKIE=", sizeof("HTTP_COOKIE=") - 1)) {
            cookiestr = environ[i] + sizeof("HTTP_COOKIE=") - 1;
        }
    }
    if(!cookiestr) {
        set_login_user_name("");
        return 0;
    }
    parse_cookie_info(cookiestr, buf_user);
    base64_decode((unsigned char *)buf_user, strlen(buf_user),
                  (unsigned char *)authinfo, &l);
    set_login_user_name(authinfo);
    FCGI_LOG("authinfo:%s", authinfo);
    get_admin_user_pass(buf_user, buf_pass);
    max_perm_class = check_user_permission(buf_user, buf_pass, authinfo);
    if(max_perm_class >= 3) {
        return max_perm_class;
    }
    get_togo_user_pass(buf_user, buf_pass);
    max_perm_class = check_user_permission(buf_user, buf_pass, authinfo);
    if(max_perm_class >= 3) {
        return max_perm_class;
    }
    return check_guest_allow() ? check_user_permission("guest", "guest", authinfo) : 0;
}
/*
struct mime_handler mime_handlers[] = {
    {"landing.asp", "text/html", no_cache, NULL, do_ej, NULL},
    {"st_info.asp", "text/html", no_cache, NULL, do_ej, NULL},
    {"**.asp", "text/html", no_cache, NULL, do_ej, do_auth},
#if defined(PLATFORM_LIS)
    {"**.js", "text/javascript", no_cache, NULL, do_ej, NULL},
#elif defined(PLATFORM_M2E)
    {"common.js", "text/javascript", max_age_one_day, NULL, do_ej, NULL},
    {"log.js", "text/javascript", max_age_one_day, NULL, do_ej, NULL},
    {"table.js", "text/javascript", max_age_one_day, NULL, do_ej, NULL},
    {"**.js", "text/javascript", max_age_one_day, NULL, do_file, NULL},
#else
    {"**.js", "text/javascript", max_age_one_day, NULL, do_ej, NULL},
#endif
    {"**.css", "text/css", max_age_one_day, NULL, do_file, NULL},
    {"**.gif", "image/gif", max_age_one_day, NULL, do_file, NULL},
    {"**.jpg", "image/jpeg", max_age_one_day, NULL, do_file, NULL},
    {"**.png", "image/png", max_age_one_day, NULL, do_file, NULL},
    {"**.svg", "image/svg+xml", max_age_one_day, NULL, do_ej, NULL},
    {"**.ttf", "application/font-ttf", max_age_one_day, NULL, do_file, NULL},
    {"**.otf", "application/font-opentype",  max_age_one_day, NULL, do_file, NULL},
    {"**.woff", "application/font-woff",  max_age_one_day, NULL, do_file, NULL},
    {"**.eot", "application/vnd.ms-fontobject",  max_age_one_day, NULL, do_file, NULL},
    {"**.htc", "text/x-component", max_age_one_day, NULL, do_file, NULL},
    {"**.cfg", "application/octet-stream\r\nContent-Disposition: attachment; filename=\"config.cfg\"", no_cache, NULL, do_export, do_auth},
    {"apply.cgi*", "text/html", no_cache, do_apply_post, do_apply_cgi, do_auth},
    {"reboot.cgi*", "text/html", no_cache, do_apply_post, do_reboot_cgi, do_auth},
    {"default.cgi*", "text/html", no_cache, do_apply_post, do_default_cgi, do_auth},
    {"connect.cgi*", "text/html", no_cache, do_apply_post, do_connect_cgi, 
        do_auth},
    {"disconnect.cgi*", "text/html", no_cache, do_apply_post, do_disconnect_cgi, do_auth},
    {"update.cgi*", "text/html", no_cache, do_apply_post, do_update_cgi, 
        do_auth},
    {"upgrade.cgi*", "text/html", no_cache, do_upgrade_post, do_upgrade_cgi,
        do_auth},
    {"bt_upload.cgi*", "text/html", no_cache, do_bt_upload_post,
        do_bt_upload_cgi, do_auth},
    {"bt.cgi*", "text/html", no_cache, do_apply_post, do_bt_cgi, do_auth},
    {"format.cgi*", "text/html", no_cache, do_apply_post, do_format_cgi,
        do_auth},
    {"ad_tools.cgi*", "text/html", no_cache, do_apply_post, do_ad_tools_cgi,
        do_auth},
    {"at_tools.cgi*", "text/html", no_cache, do_apply_post, do_at_tools_cgi,
        do_auth},
    {"wps.cgi*", "text/html", no_cache, do_apply_post, do_wps_cgi, do_auth},
    {"log.cgi*", "text/html", no_cache, do_apply_post, do_log_cgi, do_auth},
    {"survey.cgi*", "text/html", no_cache, do_apply_post, do_survey_cgi, do_auth},
    {"ui_survey.cgi*", "text/html", no_cache, do_apply_post, do_ui_survey_cgi, do_auth},
    {"site_survey.cgi*", "text/html", no_cache, do_apply_post, do_site_survey_cgi, NULL},
#if defined (PLATFORM_AXA)
    {"copy_cancel.cgi*", "text/html", no_cache, do_apply_post, do_copy_cancel_cgi, NULL},
    {"eject_storage.cgi*", "text/html", no_cache, do_apply_post, do_eject_storage_cgi, NULL},
#endif
    {"date.cgi*", "text/html", no_cache, do_apply_post, do_date_cgi, do_auth},
    {"storage.cgi*", "text/html", no_cache, do_apply_post, do_storage_cgi, do_auth},
    {"storage_space.cgi*", "text/html", no_cache, do_apply_post, do_storage_space_cgi, do_auth},
    {"storage_capacity.cgi*", "text/html", no_cache, do_apply_post, do_storage_capacity_cgi, do_auth},
    {"storage_info.cgi*", "text/html", no_cache, do_apply_post, do_storage_info_cgi, do_auth},
    {"charge.cgi*", "text/html", no_cache, do_apply_post, do_charge_cgi, do_auth},
    {"voltage.cgi*", "text/html", no_cache, do_apply_post, do_voltage_cgi, do_auth},
    {"percentage.cgi*", "text/html", no_cache, do_apply_post, do_percentage_cgi, do_auth},
    {"gauge_state.cgi*", "text/html", no_cache, do_apply_post, do_gauge_state_cgi, do_auth},
    {"gauge_fw.cgi*", "text/html", no_cache, do_apply_post, do_gauge_fw_cgi, do_auth},
    {"mcu_fw.cgi*", "text/html", no_cache, do_apply_post, do_mcu_fw_cgi, do_auth},
    {"get_mac.cgi*", "text/html", no_cache, do_apply_post, do_get_mac_cgi, do_auth},
    {NULL, NULL, NULL, NULL, NULL, NULL}
};
*/

typedef enum _QCGI_Query_Cmd_ID {
    /* http cgi coyymmand */
    E_DO_EJ,
    E_CGI_APPLY,
    E_CGI_SURVEY,
    E_CGI_DETECT,
    E_CGI_DEFAULT,
    E_CGI_REBOOT,
    E_CGI_PERCENTAGE,
    E_CGI_CHARGE,
    E_CGI_STORAGE,
    E_CGI_STORAGE_SPACE,
    E_CGI_STORAGE_INFO,
    E_CGI_EJECT_STORAGE,
    E_CGI_DATE,
    E_CGI_UPGRADE,
    E_CMD_INVALID = 0xFF
} QCGI_Query_Cmd_ID;

typedef char* (*do_func)(char *uri, int authclass);
struct _mime_handler {
    char *pattern;
    QCGI_Query_Cmd_ID id;
    int authclass;
} mime_handlers[] = {
    /* WebUI CGI */
    {"common.js", E_DO_EJ, -10},
    {"exp_dashboard.asp", E_DO_EJ, 2},
    {".asp", E_DO_EJ, 3},
    {".htm", E_DO_EJ, 3},
    {"qidRequest.cgi", E_CGI_DETECT, 2},
    {"apply.cgi", E_CGI_APPLY, 2},
    {"survey.cgi", E_CGI_SURVEY, 2},
    {"percentage.cgi", E_CGI_PERCENTAGE, 2},
    {"charge.cgi", E_CGI_CHARGE, 2},
    {"eject_storage.cgi", E_CGI_EJECT_STORAGE,2},
    {"storage_info.cgi", E_CGI_STORAGE_INFO, 2},
    {"storage_space.cgi", E_CGI_STORAGE_SPACE, 2},
    {"storage.cgi", E_CGI_STORAGE, 2},
    {"date.cgi", E_CGI_DATE, 2},
    {"upgrade.cgi", E_CGI_UPGRADE, 2},
    {"reboot.cgi", E_CGI_REBOOT, 2},
    {"default.cgi", E_CGI_DEFAULT, 2},
    {NULL, E_CMD_INVALID,0}
};

char *auth_check(char *uri, int current_auth_class) {
    int i;
    char wan_state[5];
    char *tmp_uri = uri;
    if(!uri || !*uri || strstr(tmp_uri, ".htm")) goto no_matched_url;
    for(i = 0;(i < sizeof(mime_handlers) / sizeof(mime_handlers[0])); i++) {
        FCGI_LOG("checking pattern:%s uri:%s", mime_handlers[i].pattern, uri);
        if(mime_handlers[i].pattern && strstr(uri, mime_handlers[i].pattern)) {
            if(current_auth_class >= mime_handlers[i].authclass) {
                return uri;
            } else {
                if(current_auth_class > 0) {
                    return "guest_land.asp";
                } else {
                    return "login.asp";
                }
            }
        }
    }
no_matched_url:
    switch(current_auth_class) {
        case 3:
            ezplib_get_attr_val("wan_status_rule", 0, "state", wan_state, sizeof(wan_state), EZPLIB_USE_CLI);
            if(wan_state[0] == '4') {
                return "exp_dashboard.asp";
            } else {
                return "landing.asp";
            }
        case 2:
            return "guest_land.asp";
    }
    return "login.asp";
}

static const char common_asp_header[] = "HTTP/1.1 200 OK\n\
Server: EZPacket Web Server\n\
Date: Thu, 21 Aug 2014 11:42:59 GMT\n\
Cache-Control: no-cache\n\
Pragma: no-cache\n\
Expires: 0\n\
Content-Type: text/html; charset=utf-8\n\n";

static QCGI_Query_Cmd_ID query_string_mux(void) {
    int i;
    char *query_string = NULL;

    for(i = 0; environ[i] != NULL; i++) {
        if(!strncmp(environ[i], "REQUEST_URI", (sizeof("REQUEST_URI") - 1))) {
            query_string = environ[i];
            break;
        }
    }
    if(!strcmp(query_string, "REQUEST_URI=/")) {
        return E_DO_EJ;
    }
    if(!environ[i] || !query_string)
        return E_CMD_INVALID;
    for(i = 0; mime_handlers[i].id != E_CMD_INVALID;i ++) {
        if(strstr(query_string, mime_handlers[i].pattern)) {
            return mime_handlers[i].id;
        }
    }
    return E_CMD_INVALID;
}

int main(void)
{
    char *query_string = NULL, *rediruri = NULL;
    char tmp[TMP_LEN]={0};
//    int i, j, k, c, len = 0, curr_auth_class;
    int i, j, k;
    int counter = 0;
    QCGI_Query_Cmd_ID qcmd_id;

//    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
//    inital_prod();
    while (FCGI_Accept() >= 0)   {
        curr_auth_class = check_access_permission();
        INPUT *input = CGI_INIT();
        qcmd_id = query_string_mux();

        switch(qcmd_id) {
            /* webUI  */
            case E_DO_EJ:
                for(i = 0; environ[i] != NULL; i++) {
                    if(!strncmp(environ[i], "REQUEST_URI", (sizeof("REQUEST_URI") - 1))) {
                        query_string = environ[i];
                        for(j=strlen(query_string)-1; query_string[j] != '/'; j--) {
                            counter++;
                        }
                        j = j+1;
                        for(k=0;k<counter;k++,j++) {
                            tmp[k] = query_string[j];
                        }
                        rediruri = auth_check(tmp, curr_auth_class);
                        FCGI_LOG("redirect %s to %s with class %d\n", tmp, rediruri, curr_auth_class);
                        // printf(common_asp_header);
                        do_ej(rediruri, NULL, "");
                        break;
                    }
                }
                break;
            case E_CGI_DETECT:
                do_detect_cgi(input);
                break;
            case E_CGI_APPLY:
                do_apply_cgi(input);
                break;
            case E_CGI_SURVEY:
                do_survey_cgi(input);
                break;
            case E_CGI_DEFAULT:
                do_default_cgi(input);
                break;
            case E_CGI_REBOOT:
                do_reboot_cgi(input);
                break;
            case E_CGI_PERCENTAGE:
                do_percentage_cgi(input);
                break;
            case E_CGI_CHARGE:
                do_charge_cgi(input);
                break;
            case E_CGI_STORAGE:
                do_storage_cgi(input);
                break;
            case E_CGI_STORAGE_SPACE:
                do_storage_space_cgi(input);
                break;
            case E_CGI_STORAGE_INFO:
                do_storage_info_cgi(input);
                break;
            case E_CGI_EJECT_STORAGE:
                do_eject_storage_cgi(input);
                break;
            case E_CGI_DATE:
                do_date_cgi(input);
                break;
            case E_CGI_UPGRADE:
                do_upgrade_cgi(input);
                break;
            default:
                printf("<i>Under construction</i>\n");
                FCGI_LOG("Unimplemented command\n");
                break;
        }
    }

    return 0;
}

