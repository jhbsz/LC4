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
#include "nvram_ezpacket.h"
#include "common.h"

#define L2TPD_MAX_COUNT 254

enum {
    L2TPD_ENABLE = 0,
    L2TPD_MTU,
    L2TPD_LOCALIP,
    L2TPD_LOCALIP_RANGE,
    L2TPD_REMOTEIP,
    L2TPD_REMOTEIP_RANGE,
    L2TPD_DNS_AUTO_ENABLE,
    L2TPD_DNS,
    L2TPD_CHAP_ENABLE,
    L2TPD_PROXYARP_ENABLE,
    L2TPD_NAT_ENABLE,
    L2TPD_USERNAME,
    L2TPD_PASSWD
};

static struct variable l2tpd_variables[] = {
    {longname: "L2TPD Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "L2TPD MTU", argv:ARGV("600","1400"), nullok: FALSE},
    {longname: "L2TPD VPN IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "L2TPD VPN IP Address Range", 
            argv:ARGV("1", xstr(L2TPD_SESSION_MAX)), nullok: FALSE},
    {longname: "L2TPD Remote IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "L2TPD Remote IP Address Range", 
            argv:ARGV("1", xstr(L2TPD_SESSION_MAX)), nullok: FALSE},
    {longname: "L2TPD Auto DNS", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "L2TPD DNS", argv:ARGV(""), nullok: FALSE},
    {longname: "L2TPD CHAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "L2TPD Proxy ARP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "L2TPD NAT Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "L2TPD User Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "L2TPD Password", argv:ARGV("20"), nullok: FALSE}
};

int
valid_l2tpd(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int nrule;
    int i;

    snprintf(tmp, sizeof(tmp), "l2tpd_enable");
    if(nvram_get("l2tpd_user_rule_max")) {
        l2tpd_variables[L2TPD_LOCALIP_RANGE].argv[1] = nvram_get("l2tpd_user_rule_max");
        l2tpd_variables[L2TPD_REMOTEIP_RANGE].argv[1] = nvram_get("l2tpd_user_rule_max");
    }
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &l2tpd_variables[L2TPD_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "l2tpd_mtu");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &l2tpd_variables[L2TPD_MTU]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "l2tpd_localip");
    val = websGetVar(wp, tmp, "");
    if (valid_ipaddr(wp, val, &l2tpd_variables[L2TPD_LOCALIP]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "l2tpd_remoteip");
    val = websGetVar(wp, tmp, "");
    if (valid_ipaddr(wp, val, &l2tpd_variables[L2TPD_REMOTEIP]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "l2tpd_remoteip_range");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &l2tpd_variables[L2TPD_REMOTEIP_RANGE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "l2tpd_dns_auto_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &l2tpd_variables[L2TPD_DNS_AUTO_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* If not enable Auto DNS */
    if (*val == '0') {
        snprintf(tmp, sizeof(tmp), "l2tpd_dns");
        val = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, val, &l2tpd_variables[L2TPD_DNS]) == FALSE) {
            return FALSE;
        }
    }

    snprintf(tmp, sizeof(tmp), "l2tpd_chap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &l2tpd_variables[L2TPD_CHAP_ENABLE]) 
            == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "l2tpd_proxyarp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &l2tpd_variables[L2TPD_PROXYARP_ENABLE]) 
            == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "l2tpd_nat_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &l2tpd_variables[L2TPD_NAT_ENABLE]) == FALSE) {
        return FALSE;
    }
    
    /* PPTP and L2TP share the same table. */
    snprintf(tmp, sizeof(tmp), "pptpd_user_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_username", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &l2tpd_variables[L2TPD_USERNAME]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_passwd", i);
        val = websGetVar(wp, tmp, "");
        /* The password is shown as a public key for the administrator. */
        if (valid_name(wp, val, &l2tpd_variables[L2TPD_PASSWD]) == FALSE) {
            return FALSE;
        }
    }
    
    return TRUE;
}

int 
save_l2tpd(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN], buf[TMP_LEN];    
    char all_rule[LONG_BUF_LEN];
    char *enable, *mtu, *localip, *localip_range, *remoteip, *remoteip_range,
         *proxyarp_enable, *nat_enable, *user_rule_num, *username, *passwd,
         *dns_auto_enable, *dns, *provider, *ipaddr, *name, *chap_enable, 
         *pap_enable;
    char *nv_rule1 = "l2tpd_rule";
    /* L2TP shares the pptpd_table with PPTP. */
    char *nv_rule2 = "pptpd_user_rule",
         *nv_rule2_num = "pptpd_user_rule_num",
         *nv_rule2_max = "pptpd_user_rule_max";
    int nrule, len, change = 0, max;
    int64_t map = 0;
    int i;

    snprintf(tmp, sizeof(tmp), "l2tpd_enable");
    enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_name");
    name = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_mtu");
    mtu = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_localip");
    localip = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_remoteip");
    remoteip = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_remoteip_range");
    remoteip_range = websGetVar(wp, tmp, "");
    localip_range = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_dns_auto_enable");
    dns_auto_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_dns");
    dns = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_chap_enable");
    chap_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_pap_enable");
    pap_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "l2tpd_proxyarp_enable");
    proxyarp_enable = websGetVar(wp, tmp, "");
    
    snprintf(tmp, sizeof(tmp), "l2tpd_nat_enable");
    nat_enable = websGetVar(wp, tmp, "");

    /* mtu = mru in this case */
    len = snprintf(buf, TMP_LEN, 
                    "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
                    enable, name, mtu, mtu, localip, localip_range, remoteip, 
                    remoteip_range, dns_auto_enable, dns, chap_enable, 
                    pap_enable, proxyarp_enable, nat_enable);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule(nv_rule1, 0, tmp, TMP_LEN);
    if (strcmp(buf, tmp)) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set(nv_rule1, buf);
        change = 1;
    }

    snprintf(tmp, sizeof(tmp), nv_rule2_num);
    user_rule_num = websGetVar(wp, tmp, "0");

    nrule = atoi(user_rule_num);
    max = atoi(nvram_safe_get(nv_rule2_max));
    nrule = (nrule >= max) ? max : nrule;

    if (nrule != atoi(nvram_safe_get(nv_rule2_num))) {
        /* In case the situation that the last rule is deleted but others are
         * kept the same and the check flow will treat it as 'nothing
         * changed'.
         */
        config_preaction(&map, v, s, "NUM=0", "");
        snprintf(tmp, sizeof(tmp), "%d", nrule);
        nvram_set(nv_rule2_num, tmp);
        change = 1;
    }

    all_rule[0]='\0';
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_username", i);
        username = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_provider", i);
        provider = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_passwd", i);
        passwd= websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_ipaddr", i);
        ipaddr = websGetVar(wp, tmp, "");

        len = snprintf(buf, TMP_LEN, "%s^%s^%s^%s^%s", 
                       enable, username, provider, passwd, ipaddr);
        if (len >= TMP_LEN) {
            return 0;
        }

        if (ezplib_get_rule(nv_rule2, i, tmp, TMP_LEN) < 0) {
            /* i-th rule doesn't exist */
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_append_rule(nv_rule2, buf);
            change = 1;
        } else if (strcmp(buf, tmp)) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule(nv_rule2, i, buf);
            change = 1;
        }
        if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", buf);
        else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, buf);
    }
    nvram_set(nv_rule2, all_rule);

    if (change) {
        snprintf(tmp, TMP_LEN, "NUM=%d", i);
        /* Not defined the postaction yet. */
        config_postaction(map, s, tmp, "");
    }

    return change;
}
