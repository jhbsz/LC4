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
    CLONE_ENABLE = 0,
    CLONE_HWADDR,
};

static struct variable clone_variables[] = {
    {longname: "Clone Enable", argv:ARGV("0", "1")},
    {longname: "Hardware Address", argv:ARGV("")},
};

int
ej_clone_show_hwaddr(int eid, webs_t wp, int argc, char_t **argv)
{
    int ret = 0;
    char *c;
    int which;
    char buf[TMP_LEN];

    if (ejArgs(argc, argv, "%d", &which) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* If using HTTP client MAC is indicated, then use it! */
    snprintf(buf, TMP_LEN, "wan%d_clone_http_client", which);
    c = GOZILA_SAFE_GET(buf);

    ret = snprintf(buf, TMP_LEN, "wan%d_hwaddr_clone_hwaddr", which);
    if (ret >= TMP_LEN) {
        return EZPLIB_VAL_TRUNC;
    }
    c = GOZILA_GET(buf);
    
    if (!c) {
        ret = 
            ezplib_get_attr_val("wan_hwaddr_clone_rule", which, "hwaddr", 
                    buf, TMP_LEN, EZPLIB_USE_CLI);
        if (ret < 0) {
            return ret;
        }
        c = buf;
    }
    ret += websWrite(wp, "%s", c);

    return 0;
}

int
ej_clone_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
#ifdef EZP_PROD_BRAND_ZYUS
    num = 1;
#endif

#define BUF_LEN 32

#ifdef EZP_PROD_BRAND_PROX
    for (i = num-1; i >=0; i--) {
#else
    for (i = 0; i < num; i++) {
#endif
        char buf[BUF_LEN];
        snprintf(buf, BUF_LEN, "%d", i);
        do_ej("clone_setting.asp", wp, "WAN", "wan", buf, "");
    }

#ifndef EZP_PROD_BRAND_ZYUS
    cur = nvram_safe_get("lan_num");
    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    for (i = 0; i < num; i++) {
        char buf[BUF_LEN];
        snprintf(buf, BUF_LEN, "%d", i);
        do_ej("clone_setting.asp", wp, "LAN", "lan", buf, "");
    }
#endif

    return 0;
}
static int 
_valid_iface(webs_t wp, char *iface)
{
    char *cur, tmp[TMP_LEN];
    int i, num;
    char *enable, *hwaddr;

    /* iface num */
    snprintf(tmp, sizeof(tmp), "%s_num", iface);
    cur = nvram_safe_get(tmp);

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    for (i = 0; i < num; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "%s%d_hwaddr_clone_enable", iface, i);
        enable = websGetVar(wp, tmp, "");

        if (valid_choice(wp, enable, &clone_variables[CLONE_ENABLE]) 
                == FALSE) {
            return FALSE;
        }

        if (strcmp(enable, "0") == 0) {
            continue;
        }

        /* Hardware Address */
        snprintf(tmp, sizeof(tmp), "%s%d_hwaddr_clone_hwaddr", iface, i);
        hwaddr = websGetVar(wp, tmp, "");
        if (valid_hwaddr(wp, hwaddr, &clone_variables[CLONE_HWADDR]) 
                == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}
void
do_get_mac_cgi(char *url, webs_t stream, ...)
{
    char *name;
    /* do action */
    name = websGetVar(stream, "action", "");
    websWrite(stream, "message=%s\n", client.mac);
    websWrite(stream, "name=%s\n", name);
    websWrite(stream, "return_value=4\n");
    websDone(stream, 200);

    /* Flush all key-value pairs in tha hash table. */
    init_cgi(NULL);
    return;
}

int
valid_clone(webs_t wp, char *value, struct variable *v)
{
    /* wan clone setting */
    if(_valid_iface(wp, "wan") == FALSE) {
        return FALSE;
    }

    /* lan clone setting */
    if(_valid_iface(wp, "lan") == FALSE) {
        return FALSE;
    }
    return TRUE;
}

static int
_save_iface(webs_t wp, char *iface, int64_t *map, struct variable *v, struct service *s)
{
    char *cur, rule_set[TMP_LEN], tmp[TMP_LEN], buf[TMP_LEN];
    char *enable, *hwaddr;
    int i, num, ret, change = 0;

    snprintf(tmp, sizeof(tmp), "%s_num", iface);
    snprintf(rule_set, sizeof(rule_set), "%s_hwaddr_clone_rule", iface);
    cur = nvram_safe_get(tmp);
    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
#ifdef EZP_PROD_BRAND_ZYUS
    num = 1;
#endif

    for (i = 0; i < num; i++) {
        *map = 0;

        snprintf(tmp, sizeof(tmp), "%s%d_hwaddr_clone_enable", iface, i);
        enable = websGetVar(wp, tmp, "");

        if (*enable == '0') {
            ezplib_get_attr_val(rule_set, i, "enable", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            if (strcmp(tmp, enable)) {
                snprintf(tmp, TMP_LEN, "NUM=%d", i);
                config_preaction(map, v, s, tmp, "");
                ezplib_replace_attr(rule_set, i, "enable", enable);
                change = 1;
            }
        } else {
            snprintf(tmp, sizeof(tmp), "%s%d_hwaddr_clone_hwaddr", iface, i);
            hwaddr = websGetVar(wp, tmp, "");

            ret = snprintf(tmp, TMP_LEN, "%s^%s", enable, hwaddr);
            if (ret >= TMP_LEN) {
                return FALSE;
            }
            ezplib_get_rule(rule_set, i, buf, TMP_LEN);
            if (strcmp(buf, tmp)) {
                snprintf(buf, TMP_LEN, "NUM=%d", i);
                config_preaction(map, v, s, buf, "");
                ezplib_replace_rule(rule_set, i, tmp);
                change = 1;
            }
        }
    }

    return change;
}

int
save_clone(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int change = 0;
    int64_t map = 0;
    /* save wan clone setting */
   if(_save_iface(wp, "wan", &map, v, s) == 1) {
       change = 1;
   }

#ifndef EZP_PROD_BRAND_ZYUS
    /* save lan clone setting */
    if(_save_iface(wp, "lan", &map, v, s) == 1) {
        change = 1;
    }
#endif

    if (change) {
        config_postaction(map, s, "", "");
    }
    return change;
}

