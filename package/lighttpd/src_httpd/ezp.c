#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <httpd.h>
#include <errno.h>
#include <linux/sockios.h>

#include <proto/ethernet.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/klog.h>
#include <sys/wait.h>

#include <cy_conf.h>

#include <fcntl.h>
#include <signal.h>
#include <sys/klog.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "lang.h"
#include "nvram_ezpacket.h"
#include "ezp-lib.h"
#include "ezpcom-lib.h"
#include "ezp.h"
#include "shared.h"
#include "fcgi_common.h"
#include "fcgi_stdio.h"
#include "http_common.h"

#define SERV_BITLEN 8
#define SERV_MASK   0xff
#define SERV_NGROUP (64 / SERV_BITLEN)

int curr_auth_class; 
char login_user_name[64] = "";

int get_curr_auth_class(void) {
    return curr_auth_class;
}

void set_login_user_name(char *uname) {
    strncpy(login_user_name, uname, sizeof(login_user_name));
}

char *get_login_user_name(void) {
    return login_user_name;
}

typedef enum {
    ST_INFO_TOTAL = 2,
    ST_INFO_USED = 3,
    ST_INFO_FREE = 4,
} storage_info_enum;

struct menu_item
{           
    char *name;
    char *filename;
    int prod_cat_bitmap;
    int prod_subcat_low;
    int prod_subcat_high;
}; 
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
struct main_menu_item
{
    char *name;
    char *icon;
    struct menu_item *subp;
    int prod_cat_bitmap;
    int prod_subcat_low;
    int prod_subcat_high;
    int permission_class;
};
#endif

#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
struct main_menu_item main_menu[] = {
    {"Dashboard", "icon-dashboard", (struct menu_item *)"exp_dashboard.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, GUEST},
    {"Setting","icon-list" , (struct menu_item *)"exp_setting.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"Internet","icon-globe" , (struct menu_item *)"exp_internet.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"myQNAPcloud","icon-myqnapcloud" , (struct menu_item *)"exp_myQNAPcloud.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"File Station Lite","icon-folder-open-alt" , (struct menu_item *)"webdav_landing.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, GUEST},
   // {"Logout", "icon-list",NULL, PROD_CAT_ALL, 0, SUBCAT_MAX},
    {NULL,NULL, NULL, 0, 0, 0}
};
#endif

#define PROD_CAT_ALL               (PROD_CAT_B|PROD_CAT_C|PROD_CAT_D|PROD_CAT_L|PROD_CAT_M|PROD_CAT_N|PROD_CAT_P|PROD_CAT_O|PROD_CAT_W|PROD_CAT_T|PROD_CAT_V|PROD_CAT_U|PROD_CAT_G|PROD_CAT_I|PROD_CAT_A)
#define PROD_CAT_WL                (PROD_CAT_B|PROD_CAT_C|PROD_CAT_D|PROD_CAT_L|PROD_CAT_M|PROD_CAT_N|PROD_CAT_P|PROD_CAT_O|PROD_CAT_W|PROD_CAT_V|PROD_CAT_U|PROD_CAT_G|PROD_CAT_I)
#define PROD_CAT_LB                (PROD_CAT_D|PROD_CAT_L|PROD_CAT_N|PROD_CAT_O|PROD_CAT_A|PROD_CAT_I)
#define PROD_CAT_STORAGE           (PROD_CAT_C|PROD_CAT_D|PROD_CAT_T|PROD_CAT_K|PROD_CAT_G|PROD_CAT_P)
#define PROD_CAT_STORAGE_LOW       (PROD_CAT_C|PROD_CAT_D|PROD_CAT_T|PROD_CAT_K|PROD_CAT_P)
#define PROD_CAT_MYCLOUDWIFI          (PROD_CAT_P|PROD_CAT_U|PROD_CAT_G|PROD_CAT_I)
#define PROD_CAT_WWAN              (PROD_CAT_M|PROD_CAT_N|PROD_CAT_P|PROD_CAT_W|PROD_CAT_U|PROD_CAT_I|PROD_CAT_A)
#define PROD_CAT_WWAN_SINGLE       (PROD_CAT_M|PROD_CAT_P|PROD_CAT_W|PROD_CAT_U|PROD_CAT_I)
#define PROD_CAT_PPTP              (PROD_CAT_C|PROD_CAT_L|PROD_CAT_N|PROD_CAT_A)
#define PROD_CAT_PPTP_MR           (PROD_CAT_M)
#define PROD_CAT_PPTP_X            (PROD_CAT_G|PROD_CAT_I)
#define PROD_CAT_PPTP_VR           (PROD_CAT_O)
#define PROD_CAT_IPSEC             (PROD_CAT_L|PROD_CAT_N|PROD_CAT_A)
#define PROD_CAT_IPSEC_MR          (PROD_CAT_M)
#define PROD_CAT_IPSEC_PROX        (PROD_CAT_I)
#define PROD_CAT_IPSEC_VR          (PROD_CAT_O)
#define PROD_CAT_VLAN              (PROD_CAT_V|PROD_CAT_N|PROD_CAT_A)
#define PROD_CAT_VLAN_PROX         (PROD_CAT_I)
#define PROD_CAT_NAS               (PROD_CAT_ALL | PROD_CAT_K)
#define PROD_CAT_NAS_AND_WL        (PROD_CAT_WL | PROD_CAT_K)
#define PROD_CAT_NAS_AND_ROUTER    (PROD_CAT_ALL | PROD_CAT_K)
#define PROD_CAT_RT_VR             PROD_CAT_O

/* Because we need to verify app enable/disable status, so the
 * buffer doesn't need too large. */
static char appmgrbuf[2] = ""; 
/* We cache previous requested app name to avoid nvram access multiple times
 * for the same application. */
static char appmgrprevapp[10] = "";
int
appmgr_check(char *appname) {
    if (!appname)
        return 0;
    if (!strncmp(appmgrprevapp, appname, sizeof(appmgrprevapp))) {
        return appmgrbuf[0] - '0';
    }
    ezplib_get_attr_val("appmgr_rule", 0, appname, appmgrbuf, sizeof(appmgrbuf), EZPLIB_USE_CLI);
    if (appmgrbuf[0] && (appmgrbuf[0] - '0')) {
        return appmgrbuf[0] - '0';
    }   
    return 0;
}

static int check_menu_disabled(struct menu_item* item) {
    /* If we enabled wlan in appmgr, this means the wireless menu is enabled. */
    if (!strncmp(item->filename,"wl_",sizeof("wl_") - 1) && appmgr_check("wlan")) {
        return 0;
    }
    /* If the menu is not activated in this product sub/category */
    if (!(item->prod_cat_bitmap & PROD_CAT_BITMAP) ||
        (item->prod_subcat_low > ezp_prod_subcat) ||
        (item->prod_subcat_high < ezp_prod_subcat)) {
        return 1;
    }
    return 0;
}

static int check_main_menu_non_permission(struct main_menu_item* item) {
    if(item->permission_class  <= curr_auth_class)
        return 0;
    return 1;
}

static int check_main_menu_disabled(struct main_menu_item* item) {
    char *supercat;
    supercat = nvram_safe_get("prod_supercat");
#if !defined(EZP_SUB_BRAND_SONY) && !defined(PLATFORM_AXA) && !defined(PLATFORM_AXM) && !defined(PLATFORM_M2E)
    /* If we enabled wlan in appmgr, this means the wireless menu is enabled. */
    if ((item->subp == &sub_menu_wl[0]) && appmgr_check("wlan")) {
        return 0;
    }
#endif
    /* If the menu is not activated in this product sub/category */
    if (!(item->prod_cat_bitmap & PROD_CAT_BITMAP) ||
        (item->prod_subcat_low > ezp_prod_subcat) ||
        (item->prod_subcat_high < ezp_prod_subcat)) {
        return 1;
    }
    /* For T2L, we enabled large SBM items but disabled storage function */
    if ((item->prod_cat_bitmap == PROD_CAT_STORAGE) &&
        !strcmp(ezp_prod_subsubcat, "L") &&
        !strcmp(ezp_prod_cat, "T")) {
        return 1;
    }
    /* For X-101N */
    if ((item->prod_cat_bitmap == PROD_CAT_MYCLOUDWIFI) &&
        !strcmp(supercat, "X") && (ezp_prod_subcat == 0)) {
        return 1;
    }
    return 0;
}

int reboot_action = 0;
int error_value = 0;
int config_validate_msg = 0;
int gozila_action = 0;

static void setact(int64_t actflag, int64_t *actlist) {
    int i = 0;
    for (i = 0; i < SERV_NGROUP; i++) {
        actlist[i] = actflag & SERV_MASK;
        actflag = actflag >> SERV_BITLEN;
    }
}

int
valid_passwd(INPUT *input, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(input, value, v) == FALSE) {
        return FALSE;
    }

    if (!strcmp(value, TMP_PASSWD)) {
        return TRUE;
    }
    return valid_name(input, value, v);
}

int
valid_netmask(INPUT *input, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(input, value, v) == FALSE)
        return FALSE;
        
    if (!is_legal_netmask(value)) {
        printf("message=Invalid %s %s: not a legal netmask\n",
                       v->longname, value);
        return FALSE;
    }

    /* TODO: 255.255.255.0, 255.255.255.127, 255.255.255.63, 255.255.255.X */

    return TRUE;
}

int valid_nv_word(INPUT *input, char *value, struct variable *v)
{
    char *longname;

    if (!value || !(*value))
        return TRUE;
    if (!v || !(v->longname))
        longname = "Input String";
    else
        longname = v->longname;
    if (strchr(value, '|') || strchr(value, '\'') || strchr(value, '"') ||
        strchr(value, ';') || strchr(value, '$') ||
        strchr(value, '(') || strchr(value, ')') ||
        strchr(value, '`') || strchr(value, '&') ||
        strchr(value, '>') || strchr(value, '<')) {
        printf("message=Invalid %s %s: \n"
                       "|, ', (, ), $, ;, `, &, <, > and \" are not allowed<br>",
                       longname, value);
        return FALSE;
    }

    return TRUE;
}

int
valid_nv_name(INPUT *input, char *value, struct variable *v)
{
    int max;
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }   

    if (valid_nv_word(input, value, v) == FALSE) {
        return FALSE;
    }

    max = atoi(v->argv[0]);

    if (!is_ascii_str(value)) {
        printf("message=Invalid %s %s: NULL or have illegal \n"
                       "characters<br>",
                       v->longname, value);
        return FALSE;
    }
    if (strlen(value) > max) {
        printf("message=Invalid %s %s: out of range 1-%d characters\n",
                       v->longname, value, max);
        return FALSE;
    }

    return TRUE;
}

int
valid_nv_passwd(INPUT *input, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }

    if (!strcmp(value, TMP_PASSWD)) {
        return TRUE;
    }
    return valid_nv_name(input, value, v);
}

int     
valid_length_range(INPUT *input, char *value, struct variable *v)
{
    int length, min, max;

    min = atoi(v->argv[0]);
    max = atoi(v->argv[1]);
    length = strlen(value);
   
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }
    if (valid_word(input, value, v) == FALSE) {
        return FALSE;
    }

    if (length < min || length > max) {
       printf("message=Invalid %s %d: \n"
                          "length should between (%d - %d)<br>",
                      v->longname, length, min, max);
       return FALSE;
    }

    return TRUE;
}

int
valid_range(INPUT *input, char *value, struct variable *v)
{
    int n, start, end;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }   

    if (valid_number(input, value, v) == FALSE) {
        return FALSE;
    }

    n = atoi(value);
    start = atoi(v->argv[0]); 
    end = atoi(v->argv[1]);

    if (!is_digit_str(value) || n < start || n > end) {
        printf("message=Invalid %s %s: out of range %d-%d\n",
                       v->longname, value, start, end);
        return FALSE;
    }

    return TRUE;
}

int
valid_number(INPUT *input, char *value, struct variable *v)
{
    int i; 

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(input, value, v) == FALSE) {
        return FALSE;
    }

    for (i = 0; value[i] != '\0'; i++) {
        if (value[i] > '9' || value[i] < '0') {
            printf("message=Invalid %s %s: not a number\n", 
                    v->longname, value);
            return FALSE;
        }
    }

    return TRUE;
}

int     
valid_name(INPUT *input, char *value, struct variable *v)
{           
    int max;
    if (!*value) { 
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }
    
    if (valid_word(input, value, v) == FALSE) {
        return FALSE;
    }           
        
    max = atoi(v->argv[0]);

    if (!is_ascii_str(value)) {
            printf("message=Invalid %s %s: NULL or have illegal \n"
                       "characters<br>",
                       v->longname, value);
        return FALSE;
    }
    if (strlen(value) > max) {
            printf("message=Invalid %s %s: out of range 1-%d characters\n",
                       v->longname, value, max);
        return FALSE;
    }

    return TRUE;
}

int 
valid_ipaddr(INPUT *input, char *value, struct variable *v)
{                   
    if (!*value) { 
        if (v->nullok) {
            return TRUE;
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }   
        
    if (valid_word(input, value, v) == FALSE)
        return FALSE;
        
    if (!is_legal_ipaddr(value)) {
        printf("message=Invalid %s %s: not an IP address\n",
                       v->longname, value);
        return FALSE;
    }       
        
    if ((*(value+strlen(value)-2)=='.' && *(value+strlen(value)-1)=='0') ||
            (*(value+strlen(value)-4)=='.' && *(value+strlen(value)-3)=='2' &&
             *(value+strlen(value)-2)=='5' && *(value+strlen(value)-1)=='5')) {
        printf("message=Invalid %s %s: not an IP address\n",
                v->longname, value);
        return FALSE;
    }
    return TRUE;
}

int
valid_lang(INPUT *input, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            FCGI_LOG("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(input, value, v) == FALSE)
        return FALSE;
    if (lang_get_index(value) == -1)
        return FALSE;
    return TRUE;
}

int
valid_backdoor(INPUT *input, char *value, struct variable *v)
{
    char *key1, *key2;
    INPUT *tmp;

    if ((tmp = CGI_Find_Parameter(input, "enable_ssh"))) {
        key1 = tmp->val;
    }
    else
        key1 = "";

    key2 = nvram_safe_get("license_key");
    if (strncmp(key2, key1, strlen(key2))) {
        return FALSE;
    }
    return TRUE;
}

int
valid_word(INPUT *input, char *value, struct variable *v)
{
    char *longname;
#ifdef PLATFORM_AXM
    char *cptr = value;
#endif

    if (!value || !(*value))
        return TRUE;
    if (!v || !(v->longname))
        longname = "Input String";
    else
        longname = v->longname;
#ifdef PLATFORM_AXM
    for( ; *cptr ; cptr++) {
        if(!VALID_ENG_CHAR_AND_NUM(*cptr)) {
            return FALSE;
        }
    }
#else
    if (strchr(value, '^') || strchr(value, '|') ||
        strchr(value, '\'') || strchr(value, '"') ||
        strchr(value, ';') || strchr(value, '$') ||
        strchr(value, '`') || strchr(value, '&') ||
        strchr(value, '>') || strchr(value, '<')) {
            printf("message=Invalid %s %s: "
                       "^, |, ', $, ;, `, &, <, > and \" are not allowed\n",
                       longname, value);
        return FALSE;
    }
#endif

    return TRUE;
}

int
valid_choice(INPUT *input, char *value, struct variable *v)
{       
    char **choice;

    if (!*value) {
        if (v->nullok) {
            return TRUE; 
        } else {
            printf("message=Invalid %s: empty value\n",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(input, value, v) == FALSE)
        return FALSE;

    for (choice = v->argv; *choice; choice++) {
        if (!strcmp(value, *choice))
            return TRUE;
    }
         
    printf("message=Invalid %s %s: not one of \n", v->longname,
                   value);
    for (choice = v->argv; *choice; choice++)
        printf("%s%s", choice == v->argv ? "" : "/", *choice);
    printf("<br>");
    return FALSE;
}



int
save_lang(INPUT *input, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;
    if (strcmp(value, nvram_safe_get(v->name))) {
        config_preaction(&map, v, s, "", "");
        nvram_set(v->name, value);
        lang_construct_translator(LANG_MAP_FILE, value);
        config_postaction(map, s, "", "");
        return 1;
    }
    return 0;
}

int
save_backdoor(INPUT *input, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;

    config_preaction(&map, v, s, "", "");
    ezplib_replace_attr("ssh_rule", 0, "enable", "1");
    config_postaction(map, s, "", "");

    return 1;
}

struct variable variables[] = {
#if defined(PLATFORM_M2E) || defined(PLATFORM_AXA)
    /* SWAP WISP */
    {"wisp_swap_rule", "Internet Connection", valid_wisp_swap, save_cnnt, ARGV(""), FALSE,
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
    /* MY QNAP CLOUD */
    {"exp_myqnapcloud_rule", "EXP MyQnapCloud", valid_exp_myqnapcloud, save_exp_myqnapcloud,
     ARGV(""), FALSE, (ACTION_MYQNAPCLOUD_STOP << 8)|ACTION_MYQNAPCLOUD_START},
#elif defined(EZP_SUB_BRAND_SONY) || defined(EZP_SUB_BRAND_GARMIN) || defined(PLATFORM_AXM)
    /* SWAP WISP */
    {"wisp_swap_rule", "Internet Connection", valid_wisp_swap, save_wisp_swap, ARGV(""), FALSE,
    (ACTION_WIRELESS_START << 16)|(ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
#endif
    /* CNNT */
    {"cnnt_rule", "cnnt", valid_cnnt, save_cnnt, ARGV(""), FALSE,
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
    /* WAN */
#if defined(PLATFORM_M2E)
    {"wan_rule", "WAN Protocol", valid_wan, save_cnnt, ARGV(""), FALSE,
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
#endif
    /* EXPAND LAN */
    {"exp_lan_rule", "EXP Lan", valid_exp_lan, save_exp_lan,
     ARGV(""), FALSE, ACTION_REBOOT},
    /* for special-case use to enable ssh server */
    {"backdoor", "Backdoor", valid_backdoor, save_backdoor,
     ARGV(""), FALSE, (ACTION_BACKDOOR_STOP << 8)|ACTION_BACKDOOR_START},
    {"lang", "Web Language Setting", valid_lang, save_lang,
     ARGV("3"), FALSE, ACTION_NOTHING},

};

struct service services[] = {
    { SERV_BACKDOOR, SERV_RC_COMMON "dropbear", "" },
    { SERV_NETWORK_WAN, SERV_RC_COMMON "network-wan", "" },
    { SERV_NETWORK_LAN, SERV_RC_COMMON "network-lan", "" },
    { SERV_NETWORK_MYQNAPCLOUD, "qcloud_sample get_device_info", "" },
    { 0, NULL }
};



void        
config_preaction(int64_t *map, struct variable *v, struct service *s, char *prefix,
        char *postfix)
{           
    char tmp[TMP_LEN];
    int64_t servact[SERV_NGROUP], mapact[SERV_NGROUP];
    int i;

    setact(v->action_flag, servact);
    setact(*map, mapact);
    for (; s && s->script; s++) {
        /* Find out what service should be stopped. */
        for (i = 0; i < SERV_NGROUP; i++) {
            if ((mapact[i] == ACTION_REBOOT) || 
                ((servact[i] == ACTION_REBOOT) &&
                 (mapact[i] != ACTION_FORCE_NOT_REBOOT))) {
                reboot_action = 1;
            }
            else if ((mapact[i] == ACTION_RESTART) ||
                     (servact[i] == ACTION_RESTART)) {
                reboot_action = 2;
            }
            else if ((s->flag == servact[i]) &&
                     (s->flag != mapact[i]) &&
                     (mapact[i] != ACTION_REBOOT)) {
                snprintf(tmp, TMP_LEN, "%s %s stop %s %s", prefix, s->script,
                        s->script2, postfix);
#ifdef DEBUG 
                FCGI_LOG("%s: %s (preaction_begin)", __FUNCTION__, tmp);
#endif
                system(tmp);
#ifdef DEBUG
                FCGI_LOG("%s: %s (preaction_end)", __FUNCTION__, tmp);
#endif
            }
        }
    }

    /* Add the action bits into the map. */
    (*map) = v->action_flag;
    /* If the reboot bit is on, turn on the reboot action variable. */
    /*
    if (*map & ACTION_REBOOT) {
        reboot_action = 1;
    }
    */
}

/* 
 * Check whether the givne service has been started.
 */
void
config_postaction(int64_t map, struct service *s, char *prefix, char *postfix)
{
    char tmp[TMP_LEN];
    int64_t mapact[SERV_NGROUP];
    int i;

    setact(map, mapact);
    for (; s->script; s++) {
        /* Find out what service should be started. */
        for (i = 0; i < SERV_NGROUP; i++) {
            if ((s->flag + 1) == mapact[i]) {
                snprintf(tmp, TMP_LEN, "%s %s start %s %s", prefix, s->script,
                        s->script2, postfix);
#ifdef DEBUG
                FCGI_LOG("%s: %s(postaction_begin)", __FUNCTION__, tmp);
#endif
                if (fork() == 0)
                {
//                    close(conn_fd);
                    system(tmp);
                    exit(0);
                }
#ifdef DEBUG
                FCGI_LOG("%s: %s(postaction_end)", __FUNCTION__, tmp);
#endif 
            }
        }
    }
}

/*
 * Return TRUE/FALSE if all of config inputs are correct/incorrect.
 * Also, 
 */
static int
config_validate(INPUT *input, int show_msg)
{
    struct variable *v;
    char *value;
    int ret = TRUE;
    INPUT *tmp;

#ifdef DEBUG
    FCGI_LOG("config_validate()");
#endif

    /* Set to show config validation message or not. */
    config_validate_msg = show_msg;

    /* Validate variables in table order. */
    for (v = variables; v < &variables[STRUCT_LEN(variables)]; v++) {
        if ((tmp = CGI_Find_Parameter(input, v->name))) {
//            value = tmp->val;
            value = "valid";
        }
        else
            value = NULL;
//#ifdef DEBUG
        FCGI_LOG("Matching: %s -> %s", v->name, (value) ? value : "");
//#endif
        if (!value) {
            /* v->name doesn't exist in the incoming POST */
            continue;
        }
        if ((!*value) && v->nullok) {
            /* Empty string and nullok is TRUE. */
            continue;
        }

        if (v->valid(input, value, v) == FALSE) {
//#ifdef DEBUG
            FCGI_LOG("v->name=%s[%s] FALSE [error_value:%d]", v->name,
                   value, error_value);
//#endif
            ret = FALSE;
        } else {
//#ifdef DEBUG
            FCGI_LOG("v->name=%s[%s] TRUE [error_value:%d]", v->name,
                   value, error_value);
//#endif
        }
    }

    config_validate_msg = 0;
    return ret;
}

int
gozila_cgi(INPUT *input, char_t * urlPrefix, char_t * webDir, int arg,
           char_t * url, char_t * path, char_t * query)
{
    char *submit_button;
    char URI[TMP_LEN];
    INPUT *tmp;
    gozila_action = 1;

    /* every html must have the name */
    if ((tmp = CGI_Find_Parameter(input, "submit_button"))) {
        submit_button = tmp->val;
    }
    else
        submit_button = "";

    sprintf(URI, "%s.asp", submit_button);
    // do_ej("login.asp", input, "");                  //refresh
    do_ej(URI, input, "");                  //refresh

//    do_ej(URI, input, "");                  //refresh
    gozila_action = 0;                      //reset gozila_action

    return 1;
}

static int
config_save(INPUT *input)
{
    struct variable *v;
    char *value;
    INPUT *tmp;
    int ret = 0;

#ifdef DEBUG
    FCGI_LOG("config_save()");
#endif

    /* Save variables in the table order. */
    for (v = variables; v < &variables[STRUCT_LEN(variables)]; v++) {
        if ((tmp = CGI_Find_Parameter(input, v->name))) {
//            value = tmp->val;
            value = "save";
        }
        else
            value = NULL;

#ifdef DEBUG
        FCGI_LOG("Matching: %s -> %s", v->name, (value) ? value : "");
#endif
        if (!value) {
            continue;
        }
        /* See whether any changes occur and save the change to NVRAM. */
        if (v->save(input, value, v, services)) {
            ret = 1;
        }
    }
    return ret;
}

int
iwpriv_get(int addr, char *res, int len)
{
    FILE *fp;
    char iwpriv[] = "iwpriv ra0 e2p";
    char cmd[TMP_LEN] = {0};
    snprintf(cmd, sizeof(cmd), "%s %2X", iwpriv, addr);
    if((fp = popen(cmd, "r"))) {
        fgets(res, len, fp);
        fgets(res, len, fp);
        pclose(fp);
        return 0;
    }
    return 1;
}

int
iwpriv_test()
{
    FILE *fp;
    char tmp[TMP_LEN] = {0};
    char cmd[TMP_LEN] = {0};
    snprintf(cmd, sizeof(cmd), "iwpriv ra0 e2p 00");
    if((fp = popen(cmd, "r"))) {
        fgets(tmp, sizeof(tmp), fp);
        pclose(fp);
    }
    if (!strcmp(tmp,"")) return 0;
    else return 1;
}

static int
ui_parser(char *src)
{
    char tmp[TMP_LEN] = {0};
    int i, tmp_count=0;
    for(i=0;i<strlen(src);i++) {
        if((src[i]=='\'') || (src[i]=='\"') || (src[i]=='\\')) {
            tmp[tmp_count] = '\\';
            tmp_count++;
        }
        tmp[tmp_count] = src[i];
        tmp_count++;
    }
    strcpy(src, tmp);
    return 0;
}

static int
ej_increase(int eid, INPUT *input, int argc, char_t ** argv)
{
    int val, inc;

    if (ejArgs(argc, argv, "%d %d", &val, &inc) < 2) {
        FCGI_LOG("Insufficient args");
        return EZPLIB_INVALID;
    }

    return printf("%d", val+inc);
}

int
ej_lang(int eid, INPUT *input, int argc, char_t ** argv)
{
    char *str;
    char *trans;

    if (ejArgs(argc, argv, "%s", &str) < 1) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    trans = lang_translate(str);
    return printf("%s", trans);
}

int
ej_nv_get(int eid, INPUT *input, int argc, char_t **argv)
{
    char *name, *c;
    int ret = 0;

    if (ejArgs(argc, argv, "%s", &name) < 1) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    for (c = nvram_safe_get(name); *c; c++) {
        if (isprint((int) *c) &&
            *c != '"' && *c != '&' && *c != '<' && *c != '>' && *c != '\'')
            ret += printf("%c", *c);
        else
            ret += printf("&#%d;", *c);
    }
    return ret;
}

static int
ej_nvg_get(int eid, INPUT *input, int argc, char_t ** argv)
{
    char *name, *type;

    if (ejArgs(argc, argv, "%s", &name) < 1) {
        FCGI_LOG("Insufficient args");
        return -1;
    }
    type = GOZILA_SAFE_GET(name);

    return printf("%s", type);
}

int
ej_nvg_match(int eid, INPUT *input, int argc, char_t ** argv)
{
    char *name, *match, *output;
    char *type;

    if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    type = GOZILA_SAFE_GET(name);

    if (!*type) {
        if (nvram_match(name, match)) {
            return printf("%s", output);
        }
    } else {
        if (!strcmp(type, match)) {
            return printf("%s", output);
        } }

    return 0;
}

static int
ej_nv_match(int eid, INPUT *input, int argc, char_t ** argv)
{
    char *name, *match, *output;

    if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    if (nvram_match(name, match))
        return printf("%s", output);

    return 0;
}

int
ej_nvg_attr_match(int eid, INPUT *input, int argc, char_t **argv)
{
    char *tag, *rule_set, *attr, *match, *output, *selected;
    int nth;

    if (ejArgs(argc, argv, "%s %s %d %s %s %s",
                &tag, &rule_set, &nth, &attr, &match, &output) < 6) {

        FCGI_LOG("Insufficient args");
        return EZPLIB_INVALID;
    }

    selected = nvg_attr_match(input, tag, rule_set, nth, attr, match, output);
    if (selected) {
        printf("%s", selected);
    }
    return 0;
}

static int
ej_nv_attr_get(int eid, INPUT *input, int argc, char_t **argv)
{
    char *name, *type;
    int nth;
    char buf[TMP_LEN], tmp[TMP_LEN]={0};
    int ret;
    unsigned long n = TMP_LEN;

    if (ejArgs(argc, argv, "%s %d %s", &name, &nth, &type) < 3) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    ret = ezplib_get_attr_val(name, nth, type, buf, TMP_LEN, EZPLIB_USE_CLI);

    if (ret > 0) {
        if((!strcmp(name, "wl0_apcli_swap_rule") && !strcmp(type, "ssid")) || (!strcmp(name, "wl0_apcli_rule") && !strcmp(type, "ssid"))) {
            base64_decode((unsigned char *)buf, strlen(buf), (unsigned char *)tmp, &n);
            ui_parser(tmp);
            printf("%s", tmp);
        }
        else
            printf("%s", buf);
    }

    return ret;
}

int
ej_nvg_attr_get(int eid, INPUT *input, int argc, char_t **argv)
{
    int nth, ret;
    char *tag, *rule_set, *attr, *val;
    char buf[TMP_LEN], tmp[TMP_LEN] ={0};
    unsigned long n = TMP_LEN;
    if (ejArgs(argc, argv, "%s %s %d %s", &tag, &rule_set, &nth, &attr) < 4) {
        FCGI_LOG("Insufficient args");
        return EZPLIB_INVALID;
    }

    val = GOZILA_GET(tag);
    if (!val || !val[0]) {
        ret =
            ezplib_get_attr_val(rule_set, nth, attr, buf, TMP_LEN,
                    EZPLIB_USE_CLI);
        if (ret < 0) {
            return ret;
        }
        val = buf;
    }
    if((!strcmp(rule_set, "wl0_apcli_swap_rule") && !strcmp(attr, "ssid")) || (!strcmp(rule_set, "wl0_apcli_rule") && !strcmp(attr, "ssid"))) {
        base64_decode((unsigned char *)buf, strlen(buf), (unsigned char *)tmp, &n);
        ui_parser(tmp);
        return printf("%s", tmp);
    }
    else {
        return printf("%s", val);
    }
}

int
ej_b64_nvg_attr_get(int eid, INPUT *input, int argc, char_t **argv)
{
    int nth, ret;
    char *tag, *rule_set, *attr, *val;
    char buf[TMP_LEN];
    if (ejArgs(argc, argv, "%s %s %d %s", &tag, &rule_set, &nth, &attr) < 4) {
        FCGI_LOG("Insufficient args");
        return EZPLIB_INVALID;
    }

    ret = ezplib_get_attr_val(rule_set, nth, attr, buf, sizeof(buf),
                EZPLIB_USE_WEB);
    if (ret < 0) {
        return ret;
    }
    val = buf;
    return printf("%s", val);
}

int
ej_tbl_get(int eid, INPUT *input, int argc, char **argv)
{
    char rule[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
    char *rule_set;
    int num;
    /* 1-based. Attribute num is equal to the number of '^' plus 1. */
    int nattr = 1;
    char *cur;
    int i, j, rlen;

    if (ejArgs(argc, argv, "%s", &rule_set) < 1) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    num = ezplib_get_rule_num(rule_set);

    printf("<script language='javascript'>");
    printf("var %s = ", rule_set);

    cur = tmp;
    *cur++ = '[';
    for (i = 0; i < num; i++) {
        rlen = ezplib_get_rule(rule_set, i, rule, LONG_BUF_LEN);
        /* The beginning of a rule to replace */
        cur +=
            snprintf(cur, tmp + LONG_BUF_LEN - cur, "%s[\"",
                    (cur == tmp + 1) ? "" : ",");

        for (j = 0; j < rlen; j++) {
            if (rule[j] == '^') {
                cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "\",\"");
                nattr++;
            } else {
                *cur++ = rule[j];
            }
        }
        cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "\"]");

        /* Dump for every rule in case 'tmp' is overflowed. */
        printf("%s", tmp);
        cur = tmp;
    }
    cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "];\n");
    printf("%s", tmp);

    printf("</script>");


    return 0;
}

int
ej_tbl_create_table(int eid, INPUT *input, int argc, char **argv) {
    char *rname, *tname;
    int mv;
    int max, nusr;
    char buf1[SHORT_BUF_LEN], buf2[SHORT_BUF_LEN];

    if (ejArgs(argc, argv, "%s %s %d", &tname, &rname, &mv) < 3) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    snprintf(buf1, SHORT_BUF_LEN, "%s_max", rname);
    snprintf(buf2, SHORT_BUF_LEN, "%s_num", rname);
    max = atoi(nvram_safe_get(buf1));
    nusr = atoi(nvram_safe_get(buf2));

    printf("<div id='%s'></div>", tname);
    printf("<input type='hidden' id='%s_max_rule_num' value='%d' />", tname, max);
    printf("<input type='hidden' id='%s_cur_rule_num' value='%d' />", tname, nusr);
    printf("<input type='hidden' id='%s_rule_flag' value='1' />", tname);
    printf("<input type='hidden' id='%s_rule_no' value='1' />", tname);

#ifndef EZP_PROD_BRAND_AXIM
        printf("<input type='button' id='%s_add' ", tname);
        printf("onClick='tbl_show_input(\"%s\", 1, %s);' value='%s' />",
                      tname, rname, lang_translate("Add"));
        printf("<input disabled type='button' id='%s_delete' ", tname);
        printf("onClick='tbl_del_tr(\"%s\", %s);' value='%s' />",
                       tname, rname, lang_translate("Delete"));
        printf("<input disabled type='button' id='%s_modify' ", tname);
        printf("onclick='tbl_show_input(\"%s\", 0, %s);' value='%s' />",
                       tname, rname, lang_translate("Modify"));

        if (mv == 1) {
            printf("<input disabled type='button' id='%s_up' ", tname);
            printf("onClick='tbl_moveup_tr(\"%s\", %s);' value='%s' />",
                          tname, rname, lang_translate("Up"));
            printf("<input disabled type='button' id='%s_down' ", tname);
            printf("onclick='tbl_movedown_tr(\"%s\", %s);' value='%s' />",
                          tname, rname, lang_translate("Down"));
            printf("<input type='hidden' id='%s_enable_up_down' "
                                "value='true' />", tname);
        } else {
            printf("<input type='hidden' id='%s_enable_up_down' "
                                "value='false' />", tname);
        }
#endif
    return 1;
}

int
ej_do_headmeta(int eid, INPUT *input, int argc, char_t ** argv)
{
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    printf("<meta charset='utf-8'>");
    printf("<meta name='description' content='overview & stats'>");
    printf("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
#endif
    return 0;
}

int
ej_do_basic_css(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- bootstrap CSS -->");
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    printf("<link href='css/bootstrap.min.css' rel='stylesheet' type='text/css'>");
    printf("<link href='css/bootstrap-responsive.min.css' rel='stylesheet' type='text/css'>");
#endif
    return 0;
}

int
ej_do_ace_font_css(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- Font Awesome CSS for Ace Plug-in -->");
    printf("<link href='font-awesome/css/font-awesome.css' rel='stylesheet' type='text/css' />");
    printf("<link rel='shortcut icon' type='image/x-icon' href='picts/favicon.ico' />");
    return 0;
}

int
ej_do_ace_font_ie7_css(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!--[if IE 7]>");
    printf("  <link rel=stylesheet' href='font-awesome/css/font-awesome-ie7.min.css' />");
    printf("<link rel='shortcut icon' type='image/x-icon' href='picts/favicon.ico' />");
    printf("<![endif]-->");
    return 0;
}

int
ej_do_ace_basic_css(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- Ace Basic CSS -->");
    printf("<link rel='stylesheet' href='css/ace.min.css' />");
    printf("<link rel='stylesheet' href='css/ace-responsive.min.css' />");
    printf("<link rel='stylesheet' href='css/ace-skins.min.css' />");
    return 0;
}

int
ej_do_ace_basic_ie9_css(int eid, INPUT *input, int argc, char_t ** argv )
{
    printf("<!--[if lt IE 9]-->");
    printf("  <link rel=stylesheet' href='css/ace-ie.min.css' />");
    printf("<!--[endif]-->");
   return 0;
}

int
ej_do_custom_css(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- custom AXIM Style  CSS -->");
    printf("<link href='css/aximcolor.css' rel='stylesheet' type='text/css'>");
    return 0;
}

int
ej_do_basic_js(int eid, INPUT *input, int argc, char_t ** argv)
{
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    printf("<!-- jQuery JS -->");
    printf("<script src='js/jquery.min.js' type='text/javascript'></script>");
    printf("<!-- bootstrap JS -->");
    printf("<script src='js/bootstrap.min.js' type='text/javascript'></script>");
#endif
    return 0;
}

int
ej_do_jquery_ui_js(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- Jquery UI JS  -->");
    printf("<script type='text/javascript' src='js/jquery-ui.custom.min.js' > </script>");
    printf("<script type='text/javascript' src='js/jquery.ui.touch-punch.min.js'> </script>");
    return 0;
}

int
ej_do_jquery_other_js(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- Jquery Other JS  -->");
    printf("<script type='text/javascript' src='js/jquery.slimscroll.min.js' > </script>");
    printf("<script type='text/javascript' src='js/jquery.easy-pie-chart.min.js'> </script>");
    printf("<script type='text/javascript' src='js/jquery.sparkline.min.js'> </script>");
    return 0;
}

int
ej_do_jquery_flot_js(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- Jquery Flot  JS  -->");
    printf("<script type='text/javascript' src='js/jquery.flot.min.js' > </script>");
    printf("<script type='text/javascript' src='js/jquery.flot.time.js' > </script>");
    printf("<script type='text/javascript' src='js/jquery.flot.axislabels.js' > </script>");
    printf("<script type='text/javascript' src='js/jquery.flot.pie.min.js'> </script>");
    printf("<script type='text/javascript' src='js/jquery.flot.resize.min.js'> </script>");
    return 0;
}

int
ej_do_datatables_js(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- DataTables JS -->");
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    printf("<script src='js/jquery.dataTables.min.js' type='text/javascript'></script>");
    printf("<script src='js/jquery.dataTables.bootstrap.js' type='text/javascript'></script>");
#endif
    return 0;
}

int
ej_do_ace_basic_js(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- Ace Basic  JS  -->");
    printf("<script src='js/ace-elements.min.js' > </script>");
    printf("<script src='js/ace.min.js'> </script>");
    return 0;
}

int
ej_do_custom_js(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<script src='common.js' type='text/javascript'></script>");
    return 0;
}

int
ej_show_copyright(int eid, INPUT *input, int argc, char_t ** argv)                 {
    if(strcmp(EZP_COPYRIGHT, "")) {
        char tmp[TMP_LEN];
        int i;
        strcpy(tmp, lang_translate(EZP_COPYRIGHT));
        for (i = 0; i < strlen(tmp); i++) {
            if (tmp[i] == '_')
                tmp[i] = ' ';
        }
        printf("W(\"<div style='font-size: 12px' />%s &copy; ", lang_translate("Copyright"));
        printf(" %s", lang_translate(tmp));
#ifdef PLATFORM_M2E
        printf("%s</div>\");", lang_translate("All rights reserved."));
#else
        printf("<br>%s</div>\");", lang_translate("All rights reserved."));
#endif
    }
    return 1;
}

int
ej_do_pagehead1(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' '");
    printf("http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>");
    return 0;
}

int
ej_do_pagehead2(int eid, INPUT *input, int argc, char_t ** argv)
{
    /*char *no_table;
     It's allowed not to feed a parameter. 
    ejArgs(argc, argv, "%s", &no_table);

    printf( "<meta http-equiv='Content-Type' content='"
            "application/xhtml+xml' />");
    printf( "<link type='text/css' rel='stylesheet' href='"
            "style/style.css' />");
    if (!no_table || strcmp(no_table, "no_table")) {
        printf( "<link type='text/css' rel='stylesheet' href='"
                "style/table.css' />");
    }

    printf("<link rel='stylesheet' type='text/css' media='screen' href='ext/css/themes/jquery-ui-custom.min.css' />");
    printf("<link rel='stylesheet' type='text/css' media='screen' href='ext/css/ui.jqgrid.css' />");
    printf("<link rel='stylesheet' type='text/css' href='ext/css/jquery.jqplot.css' />");
    printf("<link rel='stylesheet' type='text/css' href='ext/css/zebra_dialog.css'>");

    printf("<!--[if lte IE 6]>");
    printf("<style type='text/css'>");
    printf("#logo { behavior: url(iepngfix.htc); }");
    printf("</style>");
    printf("<link type='text/css' rel='stylesheet' href='");
    printf("style/ie6.css' />");
    printf("<![endif]-->");

    printf("<script type='text/javascript' src='common.js'></script>");
    printf("<script type='text/javascript' src='code.js'></script>");
    printf("<script type='text/javascript' src='svg-common.js'></script>");

    printf("<!--[if lt IE 9]><script language='javascript' type='text/javascript' src='ext/js/jqPlot/excanvas.js'></script><![endif]-->");
    printf("<script language='javascript' type='text/javascript' src='ext/js/jquery-min.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/jquery-ui-custom.min.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/date.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/jqGrid/i18n/grid.locale-en.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/jqGrid/jquery.jqGrid.min.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/jqPlot/jquery.jqplot.min.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.highlighter.min.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.cursor.min.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.dateAxisRenderer.min.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/zebra_dialog.js'></script>");
    printf("<script language='javascript' type='text/javascript' src='ext/js/ajaxfileupload.js'></script>");
    */
    return 0;
}

int
ej_show_version(int eid, INPUT *input, int argc, char_t ** argv)
{
    char *supercat, *type = NULL;
    supercat = nvram_safe_get("prod_supercat");
    if (ejArgs(argc, argv, "%s", &type) < 1) {
        FCGI_LOG("Insufficient args");
        return -1;
    }
    if(!strlen(type)) {
        char *version = nvram_safe_get("custom_fw_version");
        if (!strcmp(version, "")) {
            printf("%s  ", nvram_safe_get("fw_version"));
            if(!strcmp(supercat, "")){
                printf("(%s.", ezp_prod_cat);
            } else {
                printf("(%s.", supercat);
            }
            if(!strcmp(ezp_prod_subsubcat, "")) {
                printf("%d)", ezp_prod_subcat);
            } else {
                printf("%d.%s)", ezp_prod_subcat, ezp_prod_subsubcat);
            }
        } else {
            printf("%s  ", version);
        }
    } else if (!strcmp(type, "custom")) {
        char *version = nvram_safe_get("custom_fw_version");
        if (!strcmp(version, "")) {
            printf("NONE  ");
        } else {
            printf("%s  ", version);
        }
    } else if (!strcmp(type, "factory")) {
        printf("%s  ", nvram_safe_get("fw_version"));
        if(!strcmp(supercat, "")){
            printf("(%s.", ezp_prod_cat);
        } else {
            printf("(%s.", supercat);
        }
        if(!strcmp(ezp_prod_subsubcat, "")) {
            printf("%d)", ezp_prod_subcat);
        } else {
            printf("%d.%s)", ezp_prod_subcat, ezp_prod_subsubcat);
        }
    }
    return 1;
}

int
ej_show_uptime(int eid, INPUT *input, int argc, char_t ** argv)
{
    char uptime[TMP_LEN];
    int ret = -1;

    if (!show_uptime(uptime, TMP_LEN)) {
        ret = printf("%s", uptime);
        return 0;
    } else {
        ret = printf("Unknown");
        return ret;
    }
}

int
ej_show_eeprom_hwaddr(int eid, INPUT *input, int argc, char_t ** argv)
{
    char tmp[TMP_LEN] = {0};
    int wan[3] = { 40,42,44 };
    int wlan[3] = { 4,6,8 };
    int mac1, mac2, i;
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    printf("WAN=");
    for (i = 0; i < 3;i++) {
        iwpriv_get(wan[i], tmp, sizeof(tmp));
        sscanf(tmp,"[0x00%*2X]:0x%2X%2X", &mac1, &mac2);
        printf("%02X:%02X", mac2, mac1);
        if (i < 2)
            printf(":");
    }
    printf("<br>WLAN=");
    for (i = 0; i < 3;i++) {
        iwpriv_get(wlan[i], tmp, sizeof(tmp));
        sscanf(tmp,"[0x00%*2X]:0x%2X%2X", &mac1, &mac2);
        printf("%02X:%02X", mac2, mac1);
        if (i < 2)
            printf(":");
    }
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int
ej_show_wl_region(int eid, INPUT *input, int argc, char_t ** argv)
{
    int reg;
    char tmp[TMP_LEN];
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    iwpriv_get(56, tmp, sizeof(tmp));
    sscanf(tmp,"[0x00%*2X]:0x%2X%*2X", &reg);
    printf("%2X", reg);
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int
ej_show_ipaddr_prefix24(int eid, INPUT *input, int argc, char_t ** argv)
{
    char ipaddr[32];
    char *name, *type;
    int ret = -1;

    if (ejArgs(argc, argv, "%s", &type) < 1) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    name = nvram_safe_get(type);

    if (show_ipaddr_prefix24(name, ipaddr, 32)) {
        ret = printf("%s", lang_translate("Not Available"));
        return ret;
    } else {
        ret = printf("%s", ipaddr);
        return 0;
    }
}

int
ej_show_wl_band(int eid, INPUT *input, int argc, char_t ** argv)
{
    int reg;
    char tmp[TMP_LEN];
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    iwpriv_get(56, tmp, sizeof(tmp));
    sscanf(tmp,"[0x00%*2X]:0x%*2X%2X", &reg);
    printf("%2X", reg);
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int
ej_show_tx_channel(int eid, INPUT *input, int argc, char_t ** argv)
{
    int reg, i;
    char tmp[TMP_LEN];
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    for (i = 82; i <= 108; i+=2) {
        iwpriv_get(i, tmp, sizeof(tmp));
        sscanf(tmp,"[0x00%*2X]:0x%4X", &reg);
        printf("%04X", reg);
    }
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int
ej_show_freq_off(int eid, INPUT *input, int argc, char_t ** argv)
{
    int reg;
    char tmp[TMP_LEN];
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    iwpriv_get(58, tmp, sizeof(tmp));
    sscanf(tmp,"[0x00%*2X]:0x%*2X%2X", &reg);
    printf("%02X", reg);
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int
ej_show_build_time(int eid, INPUT *input, int argc, char_t ** argv)
{
    FILE *fp;
    char cmd[] = "uname -a";
    char tmp[TMP_LEN];
    if((fp = popen(cmd, "r"))) {
        fgets(tmp, sizeof(tmp), fp);
        printf("%s", tmp);
        pclose(fp);
    }
    return 0;
}

int
ej_show_uboot(int eid, INPUT *input, int argc, char_t ** argv)
{
    FILE *fp;
    char cmd[] = "grep -r U-Boot /dev/mtd1 | grep -r \\\(|sed -n '1p'|sed 's/^.*U-Boot/U-Boot/g'";
    char tmp[TMP_LEN];
    if((fp = popen(cmd, "r"))) {
        fgets(tmp, sizeof(tmp), fp);

        printf("%s", tmp);
        pclose(fp);
    }
    return 0;
}

int
ej_show_hwaddr(int eid, INPUT *input, int argc, char_t ** argv)
{
    char ifname[16], ifproto[16];
    char device[16];
    char hwaddr[64];
    char *if_ptr, *type, *if_proto;
    int ret = -1;

    if (ejArgs(argc, argv, "%s", &type) < 1) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    if (snprintf(ifproto, 16, "%s_proto", type) >= 16)
        return -1;

    if_proto = nvram_safe_get(ifproto);
    if (!strncmp(if_proto, "wwan", 4)) {
        ret = printf("No MAC Address");
        return ret;
    } else {

        if (snprintf(ifname, 16, "%s_ifname", type) >= 16)
            return -1;

        if_ptr = nvram_safe_get(ifname);

        /* PPPoE interface doesn't have MAC address. */
        /* br0 should use vlan1'a MAC address */
        if (!strncmp(if_ptr, "ppp", 3) || !strncmp(if_ptr, "br", 2)) {
            if (snprintf(device, 16, "%s_device", type) >= 16)
                return -1;
            if_ptr = nvram_safe_get(device);
        }
        if (!show_hwaddr(if_ptr, hwaddr, 64)) {
            ret = printf("%s", hwaddr);
            return 0;
        } else {
            ret = printf("Not Connected");
            return ret;
        }
    }
}

int
ej_show_myqnapcloud_status(int eid, INPUT *input, int argc, char_t ** argv)
{
    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
    char *wan0_ipaddr;
    int license_invalid;
    int qid_get_device_info_ret;
    char qcloud_account[TMP_LEN], qcloud_status[TMP_LEN], qcloud_device_id[TMP_LEN], qcloud_device_name[TMP_LEN];
    char qcloud_last_update_time[TMP_LEN]; 
    int  qcloud_last_update_time_ret;
    int  qid_check_dev_info_ret;
    memset(qcloud_account, 0, sizeof(qcloud_account));
    memset(qcloud_status, 0, sizeof(qcloud_status));
    memset(qcloud_device_id, 0, sizeof(qcloud_device_id));
    memset(qcloud_device_name, 0, sizeof(qcloud_device_name));
    memset(qcloud_last_update_time, 0, sizeof(qcloud_last_update_time));

    /*check internet*/
    wan0_ipaddr = nvram_safe_get("wan0_ipaddr");
    if (strcmp(wan0_ipaddr, "")) {
        printf("wan0_ipaddr = \"%s\";", wan0_ipaddr);
    }
    /*check license invalid*/
    license_invalid = atoi(nvram_safe_get("license_invalid"));
    printf("license_invalid = \"%d\";", license_invalid);

    /* refresh qcloud page action have qid_check_device_info and 
     * qid_get_device_info */
    qid_check_dev_info_ret = qid_check_device_info();
    qid_get_device_info_ret = qid_get_device_info(qcloud_account, qcloud_status, qcloud_device_id, qcloud_device_name, qcloud_last_update_time, &qcloud_last_update_time_ret);

    if (!strcmp(qcloud_status,"sign-in")) {  //check last update time
        if (qid_check_dev_info_ret == 0) {
                if(qcloud_last_update_time_ret == 0) { // update status success
                        printf("qid_last_update_status = \"%s\";", "success");
                } else {        //update status  failed
                        printf("qid_last_update_status = \"%s\";", "failed");
                }
            printf("qcloud_last_update_time_ret = \"%d\";", qcloud_last_update_time_ret);
        } else {
            printf("qid_last_update_status = \"%s\";", "failed");
            printf("qcloud_last_update_time_ret = \"%d\";", qid_check_dev_info_ret);
        }
    }
    /*check qid status*/
    if (strcmp(qcloud_status,"")) {
        printf("qid_status = \"%s\";", qcloud_status);
    } else {
        printf("qid_status = \"\";");
    }
    /* get_qid_account*/
    if (strcmp(qcloud_account,"")) {
        printf("qcloud_qid = \"%s\";", qcloud_account);
    } else {
        printf("qcloud_qid = \"\";");
    }
    /*get_qid_dev_name*/
    if (strcmp(qcloud_device_name,"")) {
        printf("qcloud_dev_name = \"%s\";", qcloud_device_name);
    } else {
        printf("qcloud_dev_name = \"\";");
    }

    /* get upnpc status rule outport */
    char qcloud_get_web_mgmt_outport[TMP_LEN], qcloud_get_file_station_outport[TMP_LEN];
    ezplib_get_attr_val("upnpc_status_rule", 0, "outport", qcloud_get_web_mgmt_outport, sizeof(qcloud_get_web_mgmt_outport), EZPLIB_USE_CLI);
    ezplib_get_attr_val("upnpc_status_rule", 1, "outport", qcloud_get_file_station_outport, sizeof(qcloud_get_file_station_outport), EZPLIB_USE_CLI);
    printf("qcloud_get_web_mgmt_outport = \"%s\";", qcloud_get_web_mgmt_outport);
    printf("qcloud_get_file_station_outport = \"%s\";", qcloud_get_file_station_outport);

    /* get fetch_result of last time */
    char qcloud_test_fetch_result_web_mgmt[TMP_LEN], qcloud_test_fetch_result_file_station[TMP_LEN];
    ezplib_get_attr_val("upnpc_status_rule", 0, "fetch_result", qcloud_test_fetch_result_web_mgmt, sizeof(qcloud_test_fetch_result_web_mgmt), EZPLIB_USE_CLI);
    ezplib_get_attr_val("upnpc_status_rule", 1, "fetch_result", qcloud_test_fetch_result_file_station, sizeof(qcloud_test_fetch_result_file_station), EZPLIB_USE_CLI);
    printf("qcloud_test_fetch_result_web_mgmt = \"%s\";", qcloud_test_fetch_result_web_mgmt);
    printf("qcloud_test_fetch_result_file_station = \"%s\";", qcloud_test_fetch_result_file_station);

    return 1;
}

int
ej_show_mycloudnas_hn(int eid, INPUT *input, int argc, char_t ** argv)
{
    char buf[TMP_LEN];
    ezplib_get_attr_val("myqnapcloud_id_rule", 0, "device_name", buf, sizeof(buf), EZPLIB_USE_CLI);
    if(buf[0]) {
        printf("%s.myqnapcloud.com", buf);
    }
    return 0;
}
int
ej_show_myqnapcloud_last_update_status(int eid, INPUT *input, int argc, char_t ** argv)
{
    char qcloud_account[TMP_LEN], qcloud_status[TMP_LEN], qcloud_device_id[TMP_LEN], qcloud_device_name[TMP_LEN];
    char qcloud_last_update_time[TMP_LEN]; 
    int  qcloud_last_update_time_ret, qid_check_dev_info_ret, qid_get_device_info_ret;
    qid_check_dev_info_ret = qid_check_device_info();
    qid_get_device_info_ret = qid_get_device_info(qcloud_account, qcloud_status, qcloud_device_id, qcloud_device_name, qcloud_last_update_time  , &qcloud_last_update_time_ret);
    
    if (!strcmp(qcloud_status,"sign-in")) {
            if(qid_check_dev_info_ret == 0) {
                 if (qcloud_last_update_time_ret == 0) {
                      printf("success");
                 } else {
                      printf("failed");
                 }
            } else {
                 printf("failed");
            }
    }
    return 0;
}
int
ej_show_myqnapcloud_last_update_ret_code(int eid, INPUT *input, int argc, char_t ** argv)
{
    char qcloud_account[TMP_LEN], qcloud_status[TMP_LEN], qcloud_device_id[TMP_LEN], qcloud_device_name[TMP_LEN];
    char qcloud_last_update_time[TMP_LEN]; 
    int  qcloud_last_update_time_ret, qid_check_dev_info_ret, qid_get_device_info_ret;

    qid_check_dev_info_ret = qid_check_device_info();
    qid_get_device_info_ret = qid_get_device_info(qcloud_account, qcloud_status, qcloud_device_id, qcloud_device_name, qcloud_last_update_time  , &qcloud_last_update_time_ret);
    
    if (!strcmp(qcloud_status,"sign-in")) {
       printf("%d", qcloud_last_update_time_ret);
    }
    return 0;
}

int
ej_do_bootstrap_navbar_menu(int eid, INPUT *input, int argc, char_t ** argv)
{
#if defined(PLATFORM_M2E)
printf("<div class='navbar'>");
#else
printf("<div class='navbar navbar-inverse'>");
#endif
printf(" <div class='navbar-inner'>");
printf("     <div class='container-fluid'>");

#if defined(PLATFORM_M2E)
printf("            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src='picts/logo.png' style='margin-top: 10px;'></img>");
#else
printf("            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src='picts/logo.png'></img>");
#endif

printf("              <a><small>&nbsp;</small></a>");
printf("                 <ul class='nav ace-nav pull-right'>");

#if defined(PLATFORM_M2E)
/*
printf("                     <li style='float:left; background-color:transparent; border-style:none'>\n");
printf("                     <a href='http://www.qnap.com.tw/_doc/qgmanual/' target='_blank'>\n");
printf("                             <span style='float:left; background-color:transparent'>User Manual</spcn>\n");
printf("                     </a>\n");
printf("                     </li>\n");

printf("                     <li style='float:left; background-color:transparent; border-style:none'>\n");
printf("                     <a href='http://www.qnap.com/_jump/mobile/qfile.php?lang=' target='_blank'>\n");
printf("                             &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Qfile\n");
printf("                     </a>\n");
printf("                     </li>\n");

printf("                     <li style='float:left; background-color:transparent; border-style:none'>\n");
printf("                     <a href='http://www.qnap.com/_jump/utility/qsync.php?lang=' target='_blank'>\n");
printf("                             &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Qsync\n");
printf("                     </a>\n");
printf("                     </li>\n");

printf("                     <li style='float:left; background-color:transparent; border-style:none'>\n");
printf("                     <a href='http://www.qnap.com/_jump/utility/qfinder.php?lang=' target='_blank'>\n");
printf("                             &nbsp;&nbsp;&nbsp;&nbsp;Qfinder &nbsp;&nbsp;&nbsp;&nbsp;\n");
printf("                     </a>\n");
printf("                     </li>\n");
#endif
*/
printf("                     <li class='light-aximOrange user-profile'>");
printf("                         <a class='dropdown-toggle' href='#' data-toggle='dropdown' style='background-color:rgb(108,168,198);'>");
printf("                             &nbsp;&nbsp;");
printf("                             <i class=\"icon-list-ul\" style='color:rgb(255,255,255);'></i>");
printf("                             &nbsp;&nbsp;");
printf("                         </a>");

printf("                         <ul id='user_menu' class='pull-right dropdown-menu dropdown-yellow dropdown-caret dropdown-closer'>");
printf("                             <li><a href='http://www.qnap.com.tw/_doc/qgmanual/' target='_blank'>User Manual</a></li>");
printf("                             <li><a href='http://www.qnap.com/_jump/mobile/qfile.php?lang=' target='_blank'>Qfile</a></li>");
printf("                             <li><a href='http://www.qnap.com/_jump/utility/qsync.php?lang=' target='_blank'>Qsync</a></li>");
printf("                             <li><a href='http://www.qnap.com/_jump/utility/qfinder.php?lang=' target='_blank'>Qfinder</a></li>");
printf("                         </ul>");
printf("                     </li>");

printf("                     <li class='user-profile' id='navbar_battery' style='float:left;display: none; background-color:#fff;'>");
printf("                     <a>");
printf("                             <i class='icon-battery' style='color:rgb(178,0,0);'></i>");
printf("                             <span id='nav_battery' style='color:rgb(178,0,0);'></span>");
printf("                     </a>");
printf("                     </li>");

printf("                     <li class='user-profile' id='navbar_storage' style='float:left;display: none; background-color:#fff;'>");
printf("                     <a>");
printf("                             <i class='icon-storage' style='color:rgb(178,0,0);'></i>");
printf("                             <span id='nav_storage' style='color:rgb(178,0,0);'></span>");
printf("                     </a>");
printf("                     </li>");
#endif

printf("                     <li class='user-profile'>");
printf("                         <a class='dropdown-toggle' href='#' data-toggle='dropdown' style='background-color:rgb(255,255,255);'>");
printf("                             &nbsp;&nbsp;");
printf("                             <i class=\"icon-globe\" style='color:rgb(67,67,67);'></i>");
printf("                             &nbsp;&nbsp;");
printf("                         </a>");
printf("                         <ul id='user_menu' class='pull-right dropdown-menu dropdown-yellow dropdown-caret dropdown-closer'>");
printf("                             <li><a href='#' onclick='setLanguage(\"EN\")' value='EN'></i>English</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"CE\")' value='CE'></i>Čeština</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"DA\")' value='DA'></i>Dansk</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"DE\")' value='DE'></i>Deutsch</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"EP\")' value='EP'></i>Español (España)</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"ES\")' value='ES'></i>Español (Mexicano)</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"EX\")' value='EX'></i>Ελληνικά</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"FR\")' value='FR'></i>Français</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"IT\")' value='IT'></i>Italiano</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"MA\")' value='MA'></i>Magyar</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"NE\")' value='NE'></i>Nederlands</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"NO\")' value='NO'></i>Norsk</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"PO\")' value='PO'></i>Polski</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"PB\")' value='PB'></i>Português do Brasil</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"RO\")' value='RO'></i>Română</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"SU\")' value='SU'></i>Suomi</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"SV\")' value='SV'></i>Svenska</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"TU\")' value='TU'></i>Türkçe</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"PY\")' value='PY'></i>Русский</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"TA\")' value='TA'></i>ไทย</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"JP\")' value='JP'></i>日本語</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"KR\")' value='KR'></i>한국어</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"SC\")' value='SC'></i>简体中文</a></li>");
printf("                             <li><a href='#' onclick='setLanguage(\"TW\")' value='TW'></i>繁體中文</a></li>");
printf("                         </ul>");
printf("                     </li>");

printf("                     <li class='light-aximOrange user-profile'i style='background-color:rgb(75,73,74);'>");
printf("                         <a class='user-menu dropdown-toggle' href='#' data-toggle='dropdown'>");
printf("                             &nbsp;");
printf("                             <i class=\"icon-external-link\" style='color:rgb(255,255,255);'></i>");
printf("                             &nbsp;");
/*
printf("                             <span class='user-info' id='user_info' style='top:20px; left:10px;'>");
if(curr_auth_class == 2)
printf("                             %s", lang_translate("Guest"));
else
printf("                             %s", lang_translate("Admin"));

printf("                             </span>");
printf("                             <i class='icon-caret-down'></i>");
*/
printf("                         </a>");
printf("                         <ul id='user_menu' class='pull-right dropdown-menu dropdown-yellow dropdown-caret dropdown-closer'>");
printf("                             <li><a href='#' onclick='logout()'> %s</a></li>", lang_translate("Logout"));
if(curr_auth_class > 2) {
printf("                             <li><a href='#' onclick='to_reboot_nav()'>  %s</a></li>", lang_translate("Reboot"));
printf("                             <li><a href='#' onclick='to_default_nav()'> %s</a></li>", lang_translate("Reset"));
}
printf("                         </ul>");
printf("                     </li>");

printf("                     </ul>");
printf("     </div>");
printf(" </div><!-- /.navbar -->");
printf("</div>");

    return 0;
}

int
ej_do_bootstrap_menu(int eid, INPUT *input, int argc, char_t ** argv)
{
    int i;
    struct menu_item *subp;
    char dev_name_sd[TMP_LEN] = "";
    char dev_name_usb[TMP_LEN] = "";
    char dev_name_qsync[TMP_LEN] = "";

    ezplib_get_attr_val("storage_state_rule", 0, "mount_state", dev_name_usb, TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("storage_state_rule", 1, "mount_state", dev_name_sd, TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("storage_state_rule", 2, "mount_state", dev_name_qsync, TMP_LEN, EZPLIB_USE_CLI);

    printf("<div class='nav nav-list'>");

    for (i = 0; main_menu[i].name; i++) {
        if (check_main_menu_disabled(&(main_menu[i]))) {
            continue;
        }
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
        if (check_main_menu_non_permission(&(main_menu[i]))) {
            continue;
        }
#endif
        if (!strcmp(main_menu[i].name, "Logout")) {
            printf("<li><a href='#' onclick='logout()'>%s</a></li>", lang_translate(main_menu[i].name));
            continue;
        }
        if(!(main_menu[i].prod_cat_bitmap & PROD_CAR_ONE_PAGE_SUBMENU)) {
            printf("<li class='dropdown'>");
            printf(" <a href='#' class='dropdown-toggle' data-toggle='dropdown'>%s <b class='caret'></b></a>", lang_translate(main_menu[i].name));
            printf(" <ul class='dropdown-menu'>");

            /* Submenu */
            for (subp = (main_menu[i].subp); subp->name; subp++) {
                if (check_menu_disabled(subp)) {
                    continue;
                }
                printf("<li><a href='%s'>", subp->filename);
                printf("%s</a></li>", lang_translate(subp->name));
            }
            printf(" </ul>");
            printf("</li>");
        } else {
            // printf("<li class='active'>");
            if(i==4) {
                if (!strcmp(dev_name_usb, "mounted") || !strcmp(dev_name_sd, "mounted") || !strcmp(dev_name_qsync, "mounted")){
                     printf("<li id='%d'>", i);
                } else {
                     printf("<li id='%d' style='display:none;'>", i);
                }
#if defined(PLATFORM_M2E) 
                printf("<a target='_blank' href='dav.asp'>");
#else
                printf("<a href='%s'>", main_menu[i].subp);
#endif
            } else {
                printf("<li id='%d'>", i);
                printf("<a href='%s'>", main_menu[i].subp);
            }
            printf("<i class='%s'></i> ", main_menu[i].icon);
            printf("<span class='menu-text'>%s</span></a>", lang_translate(main_menu[i].name));
            printf("</li>");
        }

    }


    printf("</div>");

    return 0;
}

int
ej_do_bootstrap_modal(int eid, INPUT *input, int argc, char_t ** argv)
{
    printf("<!-- bootstrap modal css -->");
    printf("<link rel='stylesheet' href='css/bootstrap-modal.css'>");
    printf("<!-- bootstrap modal js -->");
    printf("<script src='js/bootstrap-modalmanager.js' type='text/javascript'></script>");
    printf("<script src='js/bootstrap-modal.js' type='text/javascript'></script>");
    return 0;
}

int
ej_show_wl_ifname_hwaddr_priv(int eid, INPUT *input, int argc, char_t **argv)
{
    int if_idx, vif_idx;
    char tmp1[SHORT_BUF_LEN], tmp2[SHORT_BUF_LEN], hwaddr[SHORT_BUF_LEN];
    FILE *file;

    if (ejArgs(argc, argv, "%d %d", &if_idx, &vif_idx) < 2) {
        FCGI_LOG("Insufficient args");
        return -1;
    }

    snprintf(tmp1, SHORT_BUF_LEN, "wl%d_ifname_rule", if_idx);
    ezplib_get_attr_val(tmp1, vif_idx, "ifname", tmp2, SHORT_BUF_LEN,
            EZPLIB_USE_CLI);

    if ((file = fopen("/proc/ezp_machine_id", "r"))) {
        char *ptr;
        int x, y;

        fgets(hwaddr, sizeof(hwaddr), file);
        /* The MAC address should be XX:XX:XX:XX:XX:XX. Trim the unnecessary
         * \r derived from /proc/ezp_machine_id.
         */
        hwaddr[17] = '\0';
        sscanf(&hwaddr[15], "%2X", &x);
        y = x >> 3;
        y = y << 3;

        /* Print out the modified wireless MAC address, and quoted the delta
         * value.. */
        ptr = &hwaddr[15];
        snprintf(ptr, 3, "%02X", y);
        fclose(file);
        return printf("%s(%d)", hwaddr, x - y);
    }
    return printf("%s", lang_translate("Not enabled"));
}

/* CGI */
static int
widget_cgi(INPUT *input, char_t * urlPrefix, char_t * webDir, int arg,
           char_t * url, char_t * path, char_t * query)
{
    FILE *f;
    char buf[TMP_LEN];
    char status[TMP_LEN];
    char *val;
    int i, num;
    char uptime[TMP_LEN];
#if defined (EZP_PROD_BRAND_PROX)
    int j, wnum;
    char localtime[TMP_LEN], buf2[TMP_LEN];
#endif
    char mask[TMP_LEN];

    /* Status: 0/1/2 for disconnected/connected/else */
    num = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < num; i++) {
        snprintf(buf, sizeof(buf), "wan%d_proto", i);
        val = nvram_safe_get(buf);
        if (!strcmp(val,"wisp")) printf("wan%d_widget_proto=Wi-Fi&nbsp;Client\n", i);
        else printf("wan%d_widget_proto=%s\n", i, val);
        /* Get wan status */
        ezplib_get_attr_val("wan_status_rule", i, "state", status,
                sizeof(status), EZPLIB_USE_CLI);
        printf("wan%d_status=%s\n", i, status);
        if (atoi(status) >= 2) {
            snprintf(buf, sizeof(buf), "wan%d_ipaddr", i);
            val = nvram_safe_get(buf);
            printf("wan%d_ipaddr=%s\n", i, val);
            snprintf(buf, sizeof(buf), "wan%d_mask", i);
            val = nvram_safe_get(buf);
            trans_int2netmask(atoi(val), mask, sizeof(mask));
            printf("wan%d_mask=%s\n", i, mask);
            snprintf(buf, sizeof(buf), "wan%d_gateway", i);
            val = nvram_safe_get(buf);
            printf("wan%d_gateway=%s\n", i, val);
        } else {
            printf("wan%d_ipaddr=%s\n",  i, "0.0.0.0");
            printf("wan%d_mask=%s\n",    i, "0.0.0.0");
            printf("wan%d_gateway=%s\n", i, "0.0.0.0");
        }
        ezplib_get_attr_val("wan_status_rule", i, "uptime", buf,sizeof(buf),
                EZPLIB_USE_CLI);
        cal_ifuptime(uptime, TMP_LEN, atol(buf));
        printf("wan%d_uptime=%s\n", i, uptime);

        /* Get vpn status */
        ezplib_get_attr_val("vpn_status_rule", i, "state", status,
                sizeof(status), EZPLIB_USE_CLI);
        printf("vpn%d_status=%s\n", i, status);
        if (atoi(status) >= 2) {
            snprintf(buf, sizeof(buf), "vpn%d_ipaddr", i);
            val = nvram_safe_get(buf);
            printf("vpn%d_ipaddr=%s\n", i, val);
            snprintf(buf, sizeof(buf), "vpn%d_mask", i);
            val = nvram_safe_get(buf);
            trans_int2netmask(atoi(val), mask, sizeof(mask));
            printf("vpn%d_mask=%s\n", i, mask);
        }
#if defined (EZP_PROD_BRAND_PROX)
        else {
            printf("vpn%d_ipaddr=%s\n",  i, "0.0.0.0");
            printf("vpn%d_mask=%s\n",    i, "0.0.0.0");
            printf("vpn%d_gateway=%s\n", i, "0.0.0.0");
        }
#endif
        ezplib_get_attr_val("vpn_status_rule", i, "uptime", buf,
                sizeof(buf), EZPLIB_USE_CLI);
        cal_ifuptime(uptime,TMP_LEN, atoi(buf));
        printf("vpn%d_uptime=%s\n", i, uptime);
    }

#if defined (EZP_PROD_BRAND_PROX)
    //Wi-Fi on/off status
    ezplib_get_attr_val("wl_basic_rule", 0, "enable", buf, sizeof(buf), EZPLIB_USE_CLI);
    printf("wlan_status=%s\n",buf);

    //system uptime display
    show_uptime(uptime,TMP_LEN);
    printf("system_uptime=%s\n",uptime);

    //local time display
    show_localtime(localtime,TMP_LEN);
    printf("local_time=%s\n",localtime);

    num = atoi(nvram_safe_get("wl_num"));
    for (i = 0; i < num; i++) {
        snprintf(buf, sizeof(buf), "wl%d_sec_rule", i);
        wnum = atoi(nvram_safe_get("wlv_rule_num"));
        for (j = 0; j < wnum; j++) {
            ezplib_get_attr_val(buf, j, "secmode", buf2,sizeof(buf2),
                    EZPLIB_USE_CLI);
            if (!strcmp(buf2,"disabled")) snprintf(buf2, sizeof(buf2), "Disabled");
            printf("wl%d_security%d=%s\n", i, j, buf2);
        }
    }
#endif
    if (!(f = fopen("/var/stats/stats-widget-system", "r"))) {
        return 1;
    }

    while (fgets(buf, sizeof(buf), f)) {
        printf("%s", buf);
    }

    fclose(f);
/*
    if (appmgr_check("wwan")){
            _widget_wwan_cgi(wp, urlPrefix, webDir, arg, url, path, query);
    }
    else if (appmgr_check("storage")){
            if (strcmp(ezp_prod_subsubcat, "L") != 0){
                _widget_storage_cgi(wp, urlPrefix, webDir, arg, url, path, query);
            }
    }*/
    return 0;
}

int
apply_cgi(INPUT *input, char_t * urlPrefix, char_t * webDir, int arg,
          char_t * url, char_t * path, char_t * query)
{
    char *value = NULL, *submit_button;
    char page[SHORT_BUF_LEN];

    INPUT *tmp;
    /* Find out the value of "action". */
    if ((tmp = CGI_Find_Parameter(input, "action"))) {
       value = tmp->val;
    }

    if (!value || !*value) {
        /* No action! */
        /* TODO:XXX: Send out "action required" message. */
        return 1;
    } else if (!strcmp(value, "Gozila")) {
        gozila_cgi(input, urlPrefix, webDir, arg, url, path, query);
        return 1;
    } else if (!strcmp(value, "Widget")) {
        widget_cgi(input, urlPrefix, webDir, arg, url, path, query);
        return 1;
    }
    else if(!strcmp(value, "Apply")) {
        /* 
         * Validate the query first. The checking result will be stored at
         * error_value. If errors occur, the failure reason page should be 
         * shown. 
         */
        if (config_validate(input, 0) == FALSE) {
#ifdef DEBUG
            FCGI_LOG("config_validate() error_value=%d", error_value);
#endif
            goto err;
        }
#ifdef DEBUG
        FCGI_LOG("config_validate() error_value=%d", error_value);
#endif
        if ((tmp = CGI_Find_Parameter(input, "submit_button"))) {
            submit_button = tmp->val;
        }
        else
            submit_button = "";

        snprintf(page, SHORT_BUF_LEN, "%s.asp", submit_button);
        /* 
         * Check if any changes of variables. If yes, 
         * (1) config_preaction() to turn on action bits and stop related 
         * service.
         * (2) nvram_set() and nvram_commit() all of the changes.
         * (3) config_postaction() to restart the services.
         *
         * If any change, return 1. Otherwise, return 0.
         */
        if (config_save(input)) {
            nvram_commit();
            goto footer;
        } else {                /* Nothing is changed. */
            goto no_change;
        }
    } else if (!strcmp(value, "lang")) { /*for M2 login UI*/
        char *value;
        char URI[TMP_LEN];
        char *submit_button;
        struct variable *v;
        if ((tmp = CGI_Find_Parameter(input, "lang"))) {
            value = tmp->val;
        }
        else
            value = "";

        if ((tmp = CGI_Find_Parameter(input, "submit_button"))) {
            submit_button = tmp->val;
        }
        else
            submit_button = "";

        for (v = variables; v < &variables[STRUCT_LEN(variables)]; v++) {
            if (strcmp(v->name, "lang") == 0) {
                break;
            }
        }
        if (valid_lang(input, value, v) == FALSE) {
            goto err;
        }
        if (save_lang(input, value, v, services)) {
            nvram_commit();
        }
        sprintf(URI, "%s.asp", submit_button);
        do_ej(URI, NULL, "");
    } else if (!strcmp(value, "Login")) {
        if (get_curr_auth_class()) {
            printf("return_value=%s", EZP_RETURN_LOGIN_SUCCESS); 
            // fcgi_websDone(200);
            return 1;
        } else {
            /* Check if it is a guest login */
            lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
            if(strncasecmp(get_login_user_name(), "guest:", sizeof("guest:") - 1)) {
                /* Normal users */
                printf("message=%s\n", lang_translate("Incorrect admin password."));
            } else {
                /* Guest */
                printf("message=%s\n", lang_translate("Guest Access has been disabled on this QGenie. Please ask the owner to enable Guest Access."));
            }
            goto err;
        }
#if 0
    } else {
        websDebugWrite(wp, "Undefined action %s<br>", value);
#endif
    }
footer:
    if (reboot_action == 1) {
        char lanip[TMP_LEN], port[TMP_LEN];
        ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lanip,
                TMP_LEN, EZPLIB_USE_CLI);
        ezplib_get_attr_val("http_rule", 0, "port", port,
                TMP_LEN, EZPLIB_USE_CLI);
        /* XXX:System reboot. */
        printf("return_value=%s,%s:%s", EZP_RETURN_REBOOT, lanip, port);
        /* Force to flush right away! */
#if defined(PLATFORM_AXA)  
        system("ezp-i2c gauge host booting");
#endif
        system("/sbin/reboot");
    } else if (reboot_action == 2) {
        char lanip[TMP_LEN], port[TMP_LEN];
        ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lanip,
                TMP_LEN, EZPLIB_USE_CLI);
        ezplib_get_attr_val("http_rule", 0, "port", port,
                TMP_LEN, EZPLIB_USE_CLI);
        /* Web server restart. */
        printf("return_value=%s,%s:%s", EZP_RETURN_RESTART, lanip, port);
        /* Force to flush right away! */
#ifdef AXIMDDNS 
    } else if (reboot_action == 3) {
        printf("return_value=%s,%s",EZP_RETURN_MESSAGE,aximddns_message);
        reboot_action = 0;
#endif
    } else {
        printf("return_value=%s", EZP_RETURN_SUCCESS);
    }

    return 1;

no_change:
    printf("return_value=%s", EZP_RETURN_NOCHANGE);
    return 1;

err:
    printf("return_value=%s", EZP_RETURN_FAIL);
    return 1;
}

#ifdef EZP_SUB_BRAND_SONY 
#define USB_PATH "/sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.2/1-1.2:1.0/bInterfaceClass"
#define CARD_PATH "/sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.1/1-1.1:1.0/bInterfaceClass"
#else
#define CARD_PATH "/sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.2/1-1.2:1.0/bInterfaceClass"
#define USB_PATH "/sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.1/1-1.1:1.0/bInterfaceClass"
#endif
#define SD_RULE_IDX 1
#define USB_RULE_IDX 0
#define QSync_RULE_IDX 2

/* Usage :
 * get_storage_info(char *STRING_TO_FILTER_MOUNT_PATH, storage_info_enum TYPE_OF_INFO_TO_GET, char * result_buffer);
 * example : get SD card free space ==> get_storage_info("SD", ST_INFO_FREE, buf);
 * The result will be stored in buf. If no match or no result, an empty string
 * will be applied
 */
void
get_storage_space_info(char *mount_string, storage_info_enum info_type, char *result_buffer, int buffer_size, int space_plus_ro) {
    char tmp3[TMP_LEN]={0}, tmp4[TMP_LEN]={0};
    FILE *file, *file2;
    snprintf(tmp3, sizeof(tmp3), "mount | grep %s", mount_string);
    if ((file2 = popen(tmp3, "r"))) {
        fgets(tmp4, sizeof(tmp4), file2);
    }
    pclose(file2);
    /* The df format is 
     * /dev/sdb1              3944740   3182596    762144 81% /tmp/mnt/fslite/USB 
     * $1                     $2:total  $3:used    $4:free
     * */
    if (strcmp(tmp4, "")) {
        if (space_plus_ro || strstr(tmp4, "rw")) {
            snprintf(tmp3, sizeof(tmp3), "df | grep %s | awk '{print $%d}'", mount_string, info_type);
            if ((file = popen(tmp3, "r"))) {
                fgets(result_buffer, buffer_size, file);
                if(space_plus_ro && (!strstr(tmp4, "rw"))) {
                    sprintf(result_buffer, "%s,RO", result_buffer);
                }
                pclose(file);
            } else {
                snprintf(result_buffer, buffer_size, "NA\n");
            }
        } else {
            snprintf(result_buffer, buffer_size, "RO\n");
        }
    } else {
        snprintf(result_buffer, buffer_size, "NA\n");
    }
}

/* CGI for sending a umount command to SD card or USB storge */
void
do_eject_storage_cgi(INPUT *input)
{
    char umount_command[128],buf[128];
    FILE *eject;
    char *device_id = NULL;
    INPUT *tmp;

    memset(buf, 0, sizeof(buf));
    memset(umount_command, 0, sizeof(umount_command));
    if ((tmp = CGI_Find_Parameter(input, "device_id"))) {
        device_id = tmp->val;
    }
    if(device_id != NULL) {
        /* umount storage(USB/SD) */
        sprintf(umount_command,"umount -l %s/%s/", ROOT_PATH, device_id);
        system(umount_command);

        /* check device is busy or not */
        memset(umount_command, 0, sizeof(umount_command));
        sprintf(umount_command,"mount | grep %s", device_id);
        if((eject = popen(umount_command, "r")) != NULL) {
            fgets(buf,sizeof(buf),eject);
            if(strstr(buf,device_id)) {
                printf("busy");
            } else {
                printf("success");
                sprintf(umount_command,"rm -rf %s/%s/", ROOT_PATH, device_id);
                system(umount_command);
            }
            if(!strcmp(device_id, "USB"))
                system("/usr/sbin/ezp-i2c wifi usb_1");
            else if(!strcmp(device_id, "SD"))
                system("/usr/sbin/ezp-i2c wifi sd");
            system("/usr/sbin/ezp-i2c wifi storage");
            pclose(eject);
        }
    }
}

void
do_storage_cgi(INPUT *input)
{
    char tmp[TMP_LEN]={0}, tmp2[TMP_LEN]={0};
    char *value;
    char usb=0, card=0;
#if defined(PLATFORM_M2E)
    char qsync=0;
#endif
    INPUT *tmp_val;

    if ((tmp_val = CGI_Find_Parameter(input, "action"))) {
        value = tmp_val->val;
    }
    else
        value = "";

    if(!value || !*value) {
        usb=1;card=1;
#if defined(PLATFORM_M2E)
        qsync=1;
#endif
    } else if (!strcmp(value, "usb")) {
        usb=1;
    } else if (!strcmp(value, "card")) {
        card=1;
#if defined(PLATFORM_M2E)
    } else if (!strcmp(value, "qsync")) {
        qsync=1;
#endif
    }
    if (usb) {
        ezplib_get_attr_val("storage_state_rule", USB_RULE_IDX,
                "insert_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(tmp2[0] == '1') {
            snprintf(tmp, sizeof(tmp), "USB:08\n");
        } else {
            snprintf(tmp, sizeof(tmp), "USB:NA\n");
        }
        /*
        snprintf(tmp, sizeof(tmp), "USB:");
        if ((file = fopen(USB_PATH, "r"))) {
            fgets(tmp2, sizeof(tmp2), file);
        } else {
            snprintf(tmp2, sizeof(tmp2), "NA\n");
        }
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        */
        printf("%s", tmp);
    }

    if (card) {
        ezplib_get_attr_val("storage_state_rule", SD_RULE_IDX,
                "insert_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(tmp2[0] == '1') {
            snprintf(tmp, sizeof(tmp), "CARD:08\n");
        } else {
            snprintf(tmp, sizeof(tmp), "CARD:NA\n");
        }
        /*
        snprintf(tmp, sizeof(tmp), "CARD:", tmp);
        if ((file = fopen(CARD_PATH, "r"))) {
            fgets(tmp2, sizeof(tmp2), file);
        } else {
            snprintf(tmp2, sizeof(tmp2), "NA\n");
        }
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        */
        printf("%s", tmp);
    }
#if defined(PLATFORM_M2E)
    if (qsync) {
        ezplib_get_attr_val("storage_state_rule", QSync_RULE_IDX,
                "insert_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(tmp2[0] == '1') {
            snprintf(tmp, sizeof(tmp), "QSync:08\n");
        } else {
            snprintf(tmp, sizeof(tmp), "QSync:NA\n");
        }
        printf("%s", tmp);
    }
#endif
}


void
do_storage_info_cgi(INPUT *input)
{
    char tmp[TMP_LEN]={0}, tmp2[TMP_LEN]={0};
    char *value=NULL;
    char usb=0, card=0;
    INPUT *tmp_val;
#if defined(PLATFORM_M2E)
    char qsync=0;
#endif
    if ((tmp_val = CGI_Find_Parameter(input, "action"))) {
       value = tmp_val->val;
    }
    if(!value || !*value) {
        usb=1;card=1;
#if defined(PLATFORM_M2E)
        qsync=1;
#endif
    } else if (!strcmp(value, "usb")) {
        usb=1;
    } else if (!strcmp(value, "card")) {
        card=1;
#if defined(PLATFORM_M2E)
    } else if (!strcmp(value, "qsync")) {
        qsync=1;
#endif
    }
    if (usb) {
        ezplib_get_attr_val("storage_state_rule", USB_RULE_IDX,
                "mount_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(!strcmp("mounted", tmp2)) {
            get_storage_space_info("USB", ST_INFO_FREE, tmp2, sizeof(tmp2), 1);
            if((value = strstr(tmp2, ",RO"))) {
                snprintf(tmp, sizeof(tmp), "USB:RO\n");
                *value = 0;
            }
            else
                snprintf(tmp, sizeof(tmp), "USB:08\n");
            snprintf(tmp, sizeof(tmp), "%sUSB_FREE:%s ", tmp, tmp2);
        } else {
            snprintf(tmp, sizeof(tmp), "USB:NA\n");
            snprintf(tmp, sizeof(tmp), "%sUSB_FREE:NA ", tmp);
        }

        get_storage_space_info("USB", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 1);
        if((value = strstr(tmp2, ",RO"))) {
            *value = 0;
        }
        snprintf(tmp, sizeof(tmp), "%sUSB_TOTAL:%s", tmp, tmp2);
        printf("%s", tmp);
    }

    if (card) {
        ezplib_get_attr_val("storage_state_rule", SD_RULE_IDX,
                "mount_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(!strcmp("mounted", tmp2)) {
            get_storage_space_info("SD", ST_INFO_FREE, tmp2, sizeof(tmp2), 1);
            if((value = strstr(tmp2, ",RO"))) {
                snprintf(tmp, sizeof(tmp), "CARD:RO\n");
                *value = 0;
            }
            else
                snprintf(tmp, sizeof(tmp), "CARD:08\n");
            snprintf(tmp, sizeof(tmp), "%sCARD_FREE:%s ", tmp, tmp2);
        } else {
            snprintf(tmp, sizeof(tmp), "CARD:NA\n");
            snprintf(tmp, sizeof(tmp), "%sCARD_FREE:NA ", tmp);
        }

        get_storage_space_info("SD", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 1);
        if((value = strstr(tmp2, ",RO"))) {
            *value = 0;
        }
        snprintf(tmp, sizeof(tmp), "%sCARD_TOTAL:%s", tmp, tmp2);
        printf("%s", tmp);
    }
#if defined(PLATFORM_M2E)
    if (qsync) {
        ezplib_get_attr_val("storage_state_rule", QSync_RULE_IDX,
                "mount_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(!strcmp("mounted", tmp2)) {
            get_storage_space_info("Qsync", ST_INFO_FREE, tmp2, sizeof(tmp2), 1);
            if((value = strstr(tmp2, ",RO"))) {
                snprintf(tmp, sizeof(tmp), "QSync:RO\n");
                *value = 0;
            }
            else
                snprintf(tmp, sizeof(tmp), "QSync:08\n");
            snprintf(tmp, sizeof(tmp), "%sQSync_FREE:%s ", tmp, tmp2);
        } else {
            snprintf(tmp, sizeof(tmp), "QSync:NA\n");
            snprintf(tmp, sizeof(tmp), "%sQSync_FREE:NA ", tmp);
        }

        get_storage_space_info("Qsync", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 1);
        if((value = strstr(tmp2, ",RO"))) {
            *value = 0;
        }
        snprintf(tmp, sizeof(tmp), "%sQSync_TOTAL:%s", tmp, tmp2);
        printf("%s", tmp);
    }
#endif
}

void
do_storage_space_cgi(INPUT *input)
{
    char tmp[TMP_LEN]={0}, tmp2[TMP_LEN]={0};
    // char tmp3[TMP_LEN]={0}, tmp4[TMP_LEN]={0}, tmp5[TMP_LEN]={0};
    // FILE *file, *file2;
    char *value=NULL;
    char usb=0, card=0;
    INPUT *tmp_val;
#if defined(PLATFORM_M2E)
    char qsync=0;
#endif
    if((tmp_val=CGI_Find_Parameter(input, "action"))) {
        value = tmp_val->val;
    }
    if(!value || !*value) {
        usb=1;card=1;
#if defined(PLATFORM_M2E)
        qsync=1;
#endif
    } else if (!strcmp(value, "usb")) {
        usb=1;
    } else if (!strcmp(value, "card")) {
        card=1;
#if defined(PLATFORM_M2E)
    } else if (!strcmp(value, "qsync")) {
        qsync=1;
#endif
    }
    if (usb) {
        snprintf(tmp, sizeof(tmp), "USB:");
        get_storage_space_info("USB", ST_INFO_FREE, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        printf("%s", tmp);
    }

    if (card) {
        snprintf(tmp, sizeof(tmp), "CARD:");
        get_storage_space_info("SD", ST_INFO_FREE, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        printf("%s", tmp);
    }

#if defined(PLATFORM_M2E)
    if (qsync) {
        snprintf(tmp, sizeof(tmp), "QSync:");
        get_storage_space_info("Qsync", ST_INFO_FREE, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        printf("%s", tmp);
    }
#endif
}

void
do_date_cgi(INPUT *input)
{
    char tmp[TMP_LEN]={0};
    char *value, *date;
    FILE *file, *file2;
    char time[TMP_LEN];
    INPUT *tmp_val;

    if ((tmp_val = CGI_Find_Parameter(input, "action"))) {
        value = tmp_val->val;
    }
    else
        value = "";

    if (!strcmp(value, "Update")) {
        if ((tmp_val = CGI_Find_Parameter(input, "date"))) {
            date = tmp_val->val;
        }
        else
            date = "";

        snprintf(time, 21, "date -s %s", date);
        if ((file2 = popen(time, "r"))) {
            fgets(tmp, sizeof(tmp), file2);
        }
        pclose(file2);
    }
    if ((file = popen("date '+%m%d%H%M%Y'", "r"))) {
        fgets(tmp, sizeof(tmp), file);
    }

    printf("%s", tmp);
    pclose(file);
}

void
do_percentage_cgi(INPUT *input)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    system("/usr/sbin/ezp-i2c gauge percentage");
    if ((file = fopen("/tmp/gauge_percentage", "r"))) {
        fgets(tmp, sizeof(tmp), file);
        fclose(file);
    }
    printf("%s", tmp);
}

void
do_charge_cgi(INPUT *inmput)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    if ((file = popen("ezp-i2c charger", "r"))) {
        fgets(tmp, sizeof(tmp), file);
    }
    if (!strncmp(tmp, "No charger", 10)) {
        printf("0");
    } else {
        printf("1");
    }

    pclose(file);
}

void
do_reboot_cgi(INPUT *input)
{
    char lanip[TMP_LEN], port[TMP_LEN];
    ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lanip,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("http_rule", 0, "port", port,
            TMP_LEN, EZPLIB_USE_CLI);
    /* XXX:System reboot. */
    printf("return_value=%s,%s:%s", EZP_RETURN_REBOOT, lanip, port);
#if defined(PLATFORM_AXA)
    system("ezp-i2c gauge host booting");
#endif
    system("/sbin/reboot");
}

void
do_default_cgi(INPUT *input)
{
    nvram_default();
    nvram_commit();
    char lanip[TMP_LEN], port[TMP_LEN];
    ezplib_get_attr_val("lan_static_rule_default", 0, "ipaddr", lanip,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("http_rule_default", 0, "port", port,
            TMP_LEN, EZPLIB_USE_CLI);
    /* XXX:System reboot. */
    printf("return_value=%s,%s:%s", EZP_RETURN_REBOOT, lanip, port);
#if defined(PLATFORM_AXA)
    system("ezp-i2c gauge host booting");
#endif
    system("/sbin/reboot");
}

void
do_detect_cgi(INPUT *input)
{
    INPUT *tmp_var;
    char *value;
    //apply_cgi(input, NULL, NULL, 0, url, path, query);
   if ((tmp_var = CGI_Find_Parameter(input, "callback"))) {
       value = tmp_var->val;
       printf("%s({\"verify_result\": true})",value);
       //printf("{\"verify_result\": true})");
   }

    
    /*
    fp = fopen("/tmp/dhcp.leases","r");
    if(fp){
         while(fgets(buf,sizeof(buf),fp) != NULL){
            sscanf(buf,"%s %s %s %s *",time,mac,ip,name);
            printf("%s-%s-%s-%s,",time,mac,ip,name);
            memset(buf,0,sizeof(buf));
         }
         fclose(fp);
    }*/

}

void
do_apply_cgi(INPUT *input)
{
    char *url = NULL, *path = NULL, *query = NULL;
    apply_cgi(input, NULL, NULL, 0, url, path, query);
}

void
do_survey_cgi(INPUT *input)
{
    char *value = NULL;
    FILE *fp = NULL;
    char tmp[LONG_BUF_LEN] = {0};
    INPUT *tmp_var;
    int i=0;

    system("/etc/wl/site_survey");
    if ((tmp_var = CGI_Find_Parameter(input, "action"))) {
       value = tmp_var->val;
    }

    if(!value || !*value) {
        /* No action! */
        /* TODO:XXX: Send out "action required" message. */
    } else if (!strcmp(value, "Update")) {
        /* search wifi station */
        if((fp = fopen("/tmp/wifi_site_not_encode", "r"))) {
            printf("{\"success\": \"true\", \"msg\": \"");
            while(fgets(tmp, sizeof(tmp), fp)) {
                char ssid[TMP_LEN]={0},bssid[TMP_LEN]={0},sec[TMP_LEN]={0};
                char mode[TMP_LEN]={0},t1[TMP_LEN]={0},t2[TMP_LEN]={0};
                char sig[TMP_LEN] = {0};
                int ch=0, extch=0;
                sscanf(tmp, "%d$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]\n",
                        &ch, ssid, bssid, sec, sig, mode, t1, t2);

                /* filter special character  *;=|^ */
                if ((strstr(ssid, "*")!=NULL) || (strstr(ssid, ";")!=NULL) || (strstr(ssid, "=")!=NULL) || (strstr(ssid, "|")!=NULL) || (strstr(ssid, "^")!=NULL))
                    continue;
                if (ch == 0)
                    break;
                if ( !strcmp(ssid, "")) { // prevent error when SSID is null
                    sscanf(tmp, "%d$$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]\n",
                            &ch, bssid, sec, sig, mode, t1, t2);
                }
                if(i > 0)
                    printf("|");
                if (t2[0] == 0) {
                    /* this case is for empty ssid */
                    if(!strcmp(mode, "ABOVE")) {
                        extch = 1;
                    } else if(!strcmp(mode, "BELOW")) {
                        extch = 0;
                    } else {
                        extch = 0;
                    }
                    printf("%d^%s^%s^%s^%s^%s",
                            ch, "", ssid, bssid, sec, sig, extch);
                } else {
                    if(!strcmp(t1, "ABOVE")) {
                        extch = 1;
                    } else if(!strcmp(t1, "BELOW")) {
                        extch = 0;
                    } else {
                        extch = 0;
                    }
                    printf("%d^%s^%s^%s^%s^%s^%d",
                            ch, ssid, bssid, sec, sig, mode, extch);
                }
                t2[0] = 0;
                ch = 0;
                i++;
            }
            fclose(fp);
            printf("\"}");
        } else {
            printf("{\"success\": \"false\", \"msg\": \"%s\"}", "Site Survey Fail");
        }
    }
}



struct ej_handler ej_handlers[] = {
    /* for all */
    {"lang", ej_lang},
    {"increase", ej_increase},
    {"nv_get", ej_nv_get},
    {"nvg_get", ej_nvg_get},
    {"nv_match", ej_nv_match},
    {"nvg_match", ej_nvg_match},
    {"nv_attr_get", ej_nv_attr_get},
    {"nvg_attr_get", ej_nvg_attr_get},
    {"b64_nvg_attr_get", ej_b64_nvg_attr_get},
    {"nvg_attr_match", ej_nvg_attr_match},

    /* for GUI */
    {"do_pagehead1",ej_do_pagehead1},
    {"do_pagehead2",ej_do_pagehead2},
    {"do_headmeta",ej_do_headmeta},
    {"do_basic_css", ej_do_basic_css},
    {"do_ace_font_css", ej_do_ace_font_css},
    {"do_ace_font_ie7_css", ej_do_ace_font_ie7_css},
    {"do_ace_basic_css", ej_do_ace_basic_css},
    {"do_ace_basic_ie9_css", ej_do_ace_basic_ie9_css},
    {"do_custom_css", ej_do_custom_css},
    {"do_basic_js", ej_do_basic_js},
    {"do_jquery_ui_js", ej_do_jquery_ui_js},
    {"do_jquery_other_js", ej_do_jquery_other_js},
    {"do_jquery_flot_js", ej_do_jquery_flot_js},
    {"do_datatables_js", ej_do_datatables_js},
    {"do_ace_basic_js", ej_do_ace_basic_js},
    {"do_custom_js", ej_do_custom_js},
    {"show_copyright", ej_show_copyright},
    {"do_bootstrap_navbar_menu",ej_do_bootstrap_navbar_menu},
    {"do_bootstrap_menu",ej_do_bootstrap_menu},
    {"do_bootstrap_modal", ej_do_bootstrap_modal},

    /* EZP: Config validation and config applying. */
    {"show_version", ej_show_version},
    {"show_license_status", ej_show_license_status},
    {"show_uptime", ej_show_uptime},
    {"show_eeprom_hwaddr", ej_show_eeprom_hwaddr},
    {"show_wl_region", ej_show_wl_region},
    {"show_wl_band", ej_show_wl_band},
    {"show_tx_channel", ej_show_tx_channel},
    {"show_freq_off", ej_show_freq_off},
    {"show_build_time", ej_show_build_time},
    {"show_uboot", ej_show_uboot},
    {"show_hwaddr", ej_show_hwaddr},
    {"show_wl_ifname_hwaddr_priv", ej_show_wl_ifname_hwaddr_priv},
    {"show_ipaddr_prefix24", ej_show_ipaddr_prefix24},
    /* for status */
    {"togo_show_wan_proto", ej_togo_show_wan_proto},
    {"st_show_wan_priv_setting", ej_st_show_wan_priv_setting},
    {"st_show_lan_priv_setting", ej_st_show_lan_priv_setting},
    {"st_show_wl_priv_setting", ej_st_show_wl_priv_setting},
    {"st_mrtg_show", ej_st_mrtg_show},

    /* for wireless multiple SSID */
    {"wlv_sec_show_setting", ej_wlv_sec_show_setting},

    /* for widget */
    {"widget_start", ej_widget_start},

    /* for table */
    {"tbl_get", ej_tbl_get},
    {"tbl_create_table", ej_tbl_create_table},

    /*for myqnapcloud*/
    {"show_myqnapcloud_status", ej_show_myqnapcloud_status},

    /* for WAN */
#if defined (EZP_SUB_BRAND_SONY) || defined(EZP_SUB_BRAND_GARMIN) || defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    {"wan_show_wisp_swap_list", ej_wan_show_wisp_swap_list},
#endif
    {"static_show_setting", ej_static_show_setting},
    {"pppoe_show_setting", ej_pppoe_show_setting},
    {"wwan_show_setting", ej_wwan_show_setting},
    {"show_mycloudnas_hn", ej_show_mycloudnas_hn},
    {"show_myqnapcloud_last_update_status", ej_show_myqnapcloud_last_update_status},
    {"show_myqnapcloud_last_update_ret_code", ej_show_myqnapcloud_last_update_ret_code},

    {NULL, NULL}
};

