#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <time.h>

#include "common.h"
#include "lang.h"
#include "ezp.h"
#include "ezp-lib.h"
#include "common.h"

#define DHCP_MAX_COUNT 254

enum {
    DHCP_ENABLE = 0,
    DHCP_START,
    DHCP_NUM,
    DHCP_LEASE,
    DHCP_DOMAIN,
    DHCP_WINS,
    DHCP_DNSTYPE,
    DHCP_DNSADDR,
};

static struct variable dhcp_variables[] = {
    {longname: "DHCP Enable", argv:ARGV("0","1"), nullok: FALSE},
    {longname: "DHCP Start IP Address", argv:ARGV("1", "254"), nullok: FALSE},
    {longname: "DHCP Clients", argv:ARGV("1", "254"), nullok: FALSE},
    {longname: "DHCP Lease", argv:ARGV("300","604800"), nullok: FALSE},
    {longname: "DHCP Domain", argv:ARGV("20"), nullok: FALSE},
    {longname: "DHCP WINS IP Address", argv:ARGV(""), nullok: TRUE},
    {longname: "DNS Type", argv:ARGV("dnsrelay", "ispdns", "opendns", "googledns", "custom"), nullok: FALSE},
    {longname: "DNS Server", argv:ARGV(""), nullok: FALSE},
};

int
valid_dhcp(webs_t wp, char *value, struct variable *v)
{
    char *cur = nvram_safe_get("lan_num");
    char tmp[TMP_LEN], lan_ip[TMP_LEN], ip_24[TMP_LEN], 
         tmp_dhcp_start[TMP_LEN], tmp_dhcp_end[TMP_LEN],
         val_netbit[TMP_LEN], val_netmask[TMP_LEN];
    char *enable, *start, *num, *lease, *domain, *wins, *dnstype, *dnsaddr1, *dnsaddr2;
    int lan_num;
    int i, ip[4], dhcp_end;

    if (!*cur) {
        lan_num = 1;
    } else {
        lan_num = atoi(cur);
    }

    for (i = 0; i < lan_num; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "dhcp%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, &dhcp_variables[DHCP_ENABLE]) == FALSE) {
           return FALSE;
        }

        if (*enable == '0') {
            continue;
        }

        /* Start */
        snprintf(tmp, sizeof(tmp), "dhcp%d_start", i);
        start = websGetVar(wp, tmp, "");
        if (valid_range(wp, start, &dhcp_variables[DHCP_START]) == FALSE) {
           return FALSE;
        }

        /* Num */
        snprintf(tmp, sizeof(tmp), "dhcp%d_num", i);
        num = websGetVar(wp, tmp, "");
        if (valid_range(wp, num, &dhcp_variables[DHCP_NUM]) == FALSE) {
           return FALSE;
        }

        ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lan_ip, 
                TMP_LEN, EZPLIB_USE_CLI);
        sscanf(lan_ip, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]); 
        snprintf(ip_24, TMP_LEN , "%d.%d.%d.", ip[0], ip[1], ip[2]);
        snprintf(tmp_dhcp_start, TMP_LEN, "%s%s", ip_24, start); 
        dhcp_end = atoi(start) + atoi(num) - 1;
        snprintf(tmp_dhcp_end, TMP_LEN, "%s%d", ip_24, dhcp_end);
        ezplib_get_attr_val("lan_static_rule", 0, "mask", val_netbit, 
                TMP_LEN, EZPLIB_USE_CLI);
        trans_int2netmask(atoi(val_netbit), val_netmask, TMP_LEN);

        if (!is_legal_ip_netmask(lan_ip, val_netmask, tmp_dhcp_start) ||
            !is_legal_ip_netmask(lan_ip, val_netmask, tmp_dhcp_end) ||
            ((ip[3] >= atoi(start)) && (ip[3] <= dhcp_end)) ||
            (dhcp_end == 255)) {
            websDebugWrite(wp, "%s.<br>",
            lang_translate("The value you inputed conflicts with the LAN IP/Netmask setting"));
            return FALSE;
        }

        /* Lease */
        snprintf(tmp, sizeof(tmp), "dhcp%d_lease", i);
        lease = websGetVar(wp, tmp, "");
        if (valid_range(wp, lease, &dhcp_variables[DHCP_LEASE]) == FALSE) {
           return FALSE;
        }

        /* Domain */
        snprintf(tmp, sizeof(tmp), "dhcp%d_domain", i);
        domain = websGetVar(wp, tmp, "");
        if (valid_name(wp, domain, &dhcp_variables[DHCP_DOMAIN]) == FALSE) {
           return FALSE;
        }

        /* DNS Type  */
        snprintf(tmp, sizeof(tmp), "dhcp%d_dnstype", i);
        dnstype = websGetVar(wp, tmp, "");
        if (valid_choice(wp, dnstype, &dhcp_variables[DHCP_DNSTYPE]) == FALSE) {
           return FALSE;
        }

        /* DNS Server IP */
        snprintf(tmp, sizeof(tmp), "dhcp%d_dnsaddr1", i);
        dnsaddr1 = websGetVar(wp, tmp, "");
        if (!strcmp(dnstype, "custom")){
            if (valid_ipaddr(wp, dnsaddr1, &dhcp_variables[DHCP_DNSADDR]) == FALSE) {
                return FALSE;
            }
        }
        snprintf(tmp, sizeof(tmp), "dhcp%d_dnsaddr2", i);
        dnsaddr2 = websGetVar(wp, tmp, "");
        if (!strcmp(dnstype, "custom")){
            if (*dnsaddr2){
                if (valid_ipaddr(wp, dnsaddr2, &dhcp_variables[DHCP_DNSADDR]) == FALSE) {
                    return FALSE;
                }
            }
        }
        /* WINS */
        snprintf(tmp, sizeof(tmp), "dhcp%d_wins", i);
        wins = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, wins, &dhcp_variables[DHCP_WINS]) == FALSE) {
           return FALSE;
        }
    }

    return TRUE;

}

int 
save_dhcp(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *rule_set = "lan_dhcps_rule";
    char *rule_num = nvram_safe_get("lan_num");
    char tmp[TMP_LEN];
    char buf[TMP_LEN];
    char *enable, *start, *num, *lease, *domain, *wins, *dnstype, *dnsaddr1, *dnsaddr2, *dnsaddr3, *dnsaddr4;
    int lan_num, i, len, change = 0;
    int64_t map;

    if (!*rule_num) {
        lan_num = 1;
    } else {
        lan_num = atoi(rule_num);
    }

    for (i = 0; i < lan_num; i++) {
        map = 0;

        /* Enable */
        snprintf(tmp, sizeof(tmp), "dhcp%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        if (*enable == '0') {
            ezplib_get_attr_val(rule_set, i, "enable", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            if (strcmp(tmp, enable)) {
                snprintf(tmp, TMP_LEN, "NUM=%d", i);
                config_preaction(&map, v, s, tmp, "");
                ezplib_replace_attr(rule_set, i, "enable", enable);
                change = 1;
            }
        } else {
            /* Start */
            snprintf(tmp, sizeof(tmp), "dhcp%d_start", i);
            start = websGetVar(wp, tmp, "");

            /* Number */
            snprintf(tmp, sizeof(tmp), "dhcp%d_num", i);
            num = websGetVar(wp, tmp, "");

            /* Lease */
            snprintf(tmp, sizeof(tmp), "dhcp%d_lease", i);
            lease = websGetVar(wp, tmp, "");

            /* Domain */
            snprintf(tmp, sizeof(tmp), "dhcp%d_domain", i);
            domain = websGetVar(wp, tmp, "");

            /* DNS Type */
            snprintf(tmp, sizeof(tmp), "dhcp%d_dnstype", i);
            dnstype = websGetVar(wp, tmp, "");            

            /* DNS Server IP */
            snprintf(tmp, sizeof(tmp), "dhcp%d_dnsaddr1", i);
            dnsaddr1 = websGetVar(wp, tmp, "");

            snprintf(tmp, sizeof(tmp), "dhcp%d_dnsaddr2", i);
            dnsaddr2 = websGetVar(wp, tmp, "");
            
            snprintf(tmp, sizeof(tmp), "dhcp%d_dnsaddr3", i);
            dnsaddr3 = websGetVar(wp, tmp, "");
            
            snprintf(tmp, sizeof(tmp), "dhcp%d_dnsaddr4", i);
            dnsaddr4 = websGetVar(wp, tmp, "");

            /* WINS */
            snprintf(tmp, sizeof(tmp), "dhcp%d_wins", i);
            wins = websGetVar(wp, tmp, "");

            len = 
                snprintf(buf, TMP_LEN,
                         "WAN%d^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s", i+1,
                         enable, start, num, lease, domain, wins, 
                         dnstype, dnsaddr1, dnsaddr2, dnsaddr3, dnsaddr4);

            if (len >= TMP_LEN) {
                return 0;
            }

            ezplib_get_rule(rule_set, i, tmp, TMP_LEN);
            if (strcmp(buf, tmp)) {
                snprintf(tmp, TMP_LEN, "NUM=%d", i);
                config_preaction(&map, v, s, tmp, "");
                ezplib_replace_rule(rule_set, i, buf);
                change = 1;
            }
        }

        if (change) {
            snprintf(tmp, TMP_LEN, "NUM=%d", i);
            config_postaction(map, s, tmp, "");
        }
    }

    return change;
}

int 
ej_dhcp_show_setting(int eid, webs_t wp, int argc, char_t **argv)
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
        do_ej("dhcp_setting.asp", wp, buf, "");
    }

    return 0;
}
