#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include "fcgi_common.h"
#include "fcgi_stdio.h"

#include <cyutils.h>

#include "lang.h"
#include "ezp.h"
#include "ezp-lib.h"

#define SHORT_BUF_LEN   32

#define EZP_DHCP_SERVER_LEASE_FILE_PATH     "/var/dhcp.leases"
#define EZP_PROC_NET_ARP_PATH               "/proc/net/arp"

int
ej_show_license_status(int eid, INPUT *input, int argc, char_t ** argv)
{
    char *status = nvram_safe_get("license_invalid");
    if (!strcmp(status, "0")) {
        printf("%s", lang_translate("Authorized"));
    } else if (!strcmp(status, "1")) {
        printf("%s", lang_translate("Unauthorized"));
        printf("(%s)", nvram_safe_get("bootcount"));
    } else if (!strcmp(status, "2")) {
        printf("%s", lang_translate("Unauthorized"));
        printf("(%s)", lang_translate("Locked"));
    } else {
        printf("%s", lang_translate("Unknown"));
    }
    return 0;
}

int
ej_togo_show_wan_proto(int eid, INPUT *input, int argc, char_t **argv)
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
    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("togo_wan_proto.asp", input, buf);
    }

    return 0;
}

int
ej_st_show_wan_priv_setting(int eid, INPUT *input, int argc, char_t **argv)
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

    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_wan_priv_setting.asp", input, buf, "");
    }

    return 0;
}

int
ej_st_show_lan_priv_setting(int eid, INPUT *input, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("lan_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_lan_priv_setting.asp", input, buf, "");
    }

    return 0;
}

int
ej_st_show_wl_priv_setting(int eid, INPUT *input, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wl_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_wl_priv_setting.asp", input, buf, "");
    }

    return 0;
}

