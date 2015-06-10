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
#include "lang.h"

enum {
    LAN_IPADDR = 0,
    LAN_NETMASK,
    LAN_MTU,
    LAN_STP
};

static struct variable lan_variables[] = {
    {longname: "LAN IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "LAN Netmask", argv:ARGV("8", "32"), nullok: FALSE},
    {longname: "LAN MTU", argv:ARGV("600","1500"), nullok: FALSE},
    {longname: "LAN STP", argv:ARGV("0", "1"), nullok: FALSE},
};

int
ej_lan_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("lan_num");
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

#define BUF_LEN 32
    for (i = 0; i < num; i++) {
        char buf[BUF_LEN];
        snprintf(buf, BUF_LEN, "%d", i);
        do_ej("lan_setting.asp", wp, buf, "");
    }

    return 0;
}
int
ej_lan_show_subnet(int eid, webs_t wp, int argc, char_t **argv)
{
    char ip[TMP_LEN], mask[TMP_LEN];
    int ip1, ip2, ip3, ip4;

    ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", ip, TMP_LEN, EZPLIB_USE_CLI); 
    sscanf( ip, "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4 );
    ezplib_get_attr_val("lan_static_rule", 0, "mask", mask, TMP_LEN, EZPLIB_USE_CLI);
    /* XXX: FIXME! for any netmask */
    switch (atoi(mask)) { 
        case 24:
            websWrite(wp, "%d.%d.%d.0", ip1, ip2, ip3);
            break;
        case 16:
            websWrite(wp, "%d.%d.0.0", ip1, ip2);
            break;
        case 8:
            websWrite(wp, "%d.0.0.0", ip1);
            break;
    }
    return 0; 
}

static int
valid_lan_static(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val_ip, *val_netbit;
    int idx = atoi(value);
    char dhcp_enable[TMP_LEN], ip_24[TMP_LEN], dhcp_start[TMP_LEN],
         dhcp_num[TMP_LEN], tmp_dhcp_start[TMP_LEN], 
         tmp_dhcp_end[TMP_LEN], val_netmask[TMP_LEN];
    int ip[4], dhcp_end;

    /* IP */
    snprintf(tmp, sizeof(tmp), "lan%d_static_ipaddr", idx);
    val_ip = websGetVar(wp, tmp, "");

    if (valid_ipaddr(wp, val_ip, &lan_variables[LAN_IPADDR]) == FALSE) {
        return FALSE;
    }

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "lan%d_static_mask", idx);
    val_netbit = websGetVar(wp, tmp, "");

    if (valid_range(wp, val_netbit, &lan_variables[LAN_NETMASK]) == FALSE) {
        return FALSE;
    }

    ezplib_get_attr_val("lan_dhcps_rule", 0, "enable", dhcp_enable, 
            TMP_LEN, EZPLIB_USE_CLI);

    if (!strcmp(dhcp_enable, "1")) {
        sscanf(val_ip, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]); 
        snprintf(ip_24, TMP_LEN , "%d.%d.%d.", ip[0], ip[1], ip[2]);
        ezplib_get_attr_val("lan_dhcps_rule", 0, "start", dhcp_start, 
                TMP_LEN, EZPLIB_USE_CLI);
        snprintf(tmp_dhcp_start, TMP_LEN, "%s%s", ip_24, dhcp_start); 
        ezplib_get_attr_val("lan_dhcps_rule", 0, "num", dhcp_num, 
                TMP_LEN, EZPLIB_USE_CLI);
        dhcp_end = atoi(dhcp_start) + atoi(dhcp_num) - 1;
        snprintf(tmp_dhcp_end, TMP_LEN, "%s%d", ip_24, dhcp_end);
        trans_int2netmask(atoi(val_netbit), val_netmask, TMP_LEN);

        if (!is_legal_ip_netmask(val_ip, val_netmask, tmp_dhcp_start) ||
            !is_legal_ip_netmask(val_ip, val_netmask, tmp_dhcp_end) ||
            ((ip[3] >= atoi(dhcp_start)) && (ip[3] <= dhcp_end ))) {
            websDebugWrite(wp, "%s.<br>",
            lang_translate("IP Address or Netmask conflicts with the DHCP setting"));
            return FALSE;
        }
    }
    return TRUE;
}

static int
valid_lan_main(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* MTU */
    snprintf(tmp, sizeof(tmp), "lan%d_mtu", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_range(wp, val, &lan_variables[LAN_MTU]) == FALSE) {
        return FALSE;
    }

    /* STP */
    snprintf(tmp, sizeof(tmp), "lan%d_stp", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_choice(wp, val, &lan_variables[LAN_STP]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_save_lan_static(webs_t wp, char *value, struct variable *v,
        struct service *s, int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *ip, *mask, *gateway, *dns;
    int idx = atoi(value), len, change = 0;

    /* IP */
    snprintf(tmp, sizeof(tmp), "lan%d_static_ipaddr", idx);
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "lan%d_static_mask", idx);
    mask = websGetVar(wp, tmp, "");

    /* Gateway: empty for LAN */
    gateway = "";

    /* DNS */
    snprintf(tmp, sizeof(tmp), "lan%d_static_dns", idx);
    dns = websGetVar(wp, tmp, "");

    /* Empty value for Wins attribute. */
    len = 
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^", ip, mask, gateway, dns);

    if (len > TMP_LEN - 1) {
        return FALSE;
    }

    ezplib_get_rule("lan_static_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("lan_static_rule", idx, tmp);
        change = 1;
    }

    return change;
}

static int
_save_lan_main(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *mtu, *stp, *weight;
    int idx = atoi(value), len, change = 0;

    /* MTU */
    snprintf(tmp, sizeof(tmp), "lan%d_mtu", idx);
    mtu = websGetVar(wp, tmp, "");

    /* STP */
    snprintf(tmp, sizeof(tmp), "lan%d_stp", idx);
    stp = websGetVar(wp, tmp, "");

    /* Weight */
    snprintf(tmp, sizeof(tmp), "lan%d_weight", idx);
    weight = websGetVar(wp, tmp, "");

    /* Enable is always true. */
    len = snprintf(tmp, TMP_LEN, "LAN%d^1^%s^%s^%s", idx+1, mtu, stp, weight);
    if (len >= TMP_LEN) {
        return FALSE;
    }

    ezplib_get_rule("lan_main_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("lan_main_rule", idx, tmp);
        change = 1;
    }

    return change;
}

int
valid_lan(webs_t wp, char *value, struct variable *v)
{
    char *cur = nvram_safe_get("lan_num");
    char tmp[TMP_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        snprintf(tmp, sizeof(tmp), "%d", i);

        if (valid_lan_static(wp, tmp, v) == FALSE) {
            return FALSE;
        }

        if (valid_lan_main(wp, tmp, v) == FALSE) {
            return FALSE;
        }
    }

    /* Weight is not allowed to configure by users. */

    return TRUE;
}

int
save_lan(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *cur = nvram_safe_get("lan_num");
    char tmp[TMP_LEN];
    int num;
    int i, change = 0;
    int64_t map;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        map = 0;

        snprintf(tmp, sizeof(tmp), "%d", i);

        if (_save_lan_static(wp, tmp, v, s, &map)) {
            change = 1;
        }

        if (_save_lan_main(wp, tmp, v, s, &map)) {
            change = 1;
        }
        
        if (change) {
            snprintf(tmp, TMP_LEN, "NUM=%d", i);
            config_postaction(map, s, tmp, "");
        }
    }

    return change;
}

