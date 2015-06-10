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

#define PPTPD_MAX_COUNT 254

enum {
    PPTPD_ENABLE = 0,
    PPTPD_MTU,
    PPTPD_LOCALIP,
    PPTPD_LOCALIP_RANGE,
    PPTPD_REMOTEIP,
    PPTPD_REMOTEIP_RANGE,
    PPTPD_DNS_AUTO_ENABLE,
    PPTPD_DNS,
    PPTPD_CHAP_ENABLE2,
    PPTPD_MSCHAP_ENABLE2,
    PPTPD_MSCHAPV2_ENABLE2,
    PPTPD_EAP_ENABLE2,
    PPTPD_PAP_ENABLE2,
    PPTPD_MPPE128_ENABLE,
    PPTPD_PROXYARP_ENABLE,
    PPTPD_NAT_ENABLE,
    PPTPD_USERNAME,
    PPTPD_PASSWD
};

static struct variable pptpd_variables[] = {
    {longname: "PPTPD Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD MTU", argv:ARGV("600","1500"), nullok: FALSE},
    {longname: "PPTPD VPN IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "PPTPD VPN IP Address Range", 
            argv:ARGV("1", xstr(PPTPD_SESSION_MAX)), nullok: FALSE},
    {longname: "PPTPD Remote IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "PPTPD Remote IP Address Range", 
            argv:ARGV("1", xstr(PPTPD_SESSION_MAX)), nullok: FALSE},
    {longname: "PPTPD Auto DNS", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD DNS", argv:ARGV(""), nullok: FALSE},
    {longname: "PPTPD CHAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD MSCHAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD MSCHAP v2 Enable", argv:ARGV("0", "1"),
            nullok: FALSE},
    {longname: "PPTPD EAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD PAP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD MPPE128 Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD Proxy ARP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD NAT Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "PPTPD User Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "PPTPD Password", argv:ARGV("20"), nullok: FALSE}
};

int
valid_pptpd(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int nrule;
    int i;

    snprintf(tmp, sizeof(tmp), "pptpd_enable");
    if(nvram_get("pptpd_user_rule_max")) {
        pptpd_variables[PPTPD_LOCALIP_RANGE].argv[1] = nvram_get("pptpd_user_rule_max");
        pptpd_variables[PPTPD_REMOTEIP_RANGE].argv[1] = nvram_get("pptpd_user_rule_max");
    }
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* If hardware NAT is enabled, we can not start PPTPD. */
    ezplib_get_attr_val("nat_rule", 0, "hw_enable", tmp,
                            sizeof(tmp), EZPLIB_USE_CLI);
    if ((*val == '1') && !strcmp(tmp, "1")) {
        websDebugWrite(wp, "%s.<br> %s.<br>",
                lang_translate("Start VPN/PPTP failed"),
                lang_translate("Need to turn off Hardware Accelerator first"));
        return FALSE;
    }
    snprintf(tmp, sizeof(tmp), "pptpd_mtu");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &pptpd_variables[PPTPD_MTU]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_localip");
    val = websGetVar(wp, tmp, "");
    if (valid_ipaddr(wp, val, &pptpd_variables[PPTPD_LOCALIP]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_remoteip");
    val = websGetVar(wp, tmp, "");
    if (valid_ipaddr(wp, val, &pptpd_variables[PPTPD_REMOTEIP]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_remoteip_range");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &pptpd_variables[PPTPD_REMOTEIP_RANGE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_dns_auto_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_DNS_AUTO_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*val == '0') {
        /* If not enable Auto DNS */
        snprintf(tmp, sizeof(tmp), "pptpd_dns");
        val = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, val, &pptpd_variables[PPTPD_DNS]) == FALSE) {
            return FALSE;
        }
    }

    snprintf(tmp, sizeof(tmp), "pptpd_chap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_CHAP_ENABLE2]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_mschap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_MSCHAP_ENABLE2]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_mschapv2_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_MSCHAPV2_ENABLE2]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_eap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_EAP_ENABLE2]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_pap_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_PAP_ENABLE2]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_mppe128_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_MPPE128_ENABLE]) 
            == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "pptpd_proxyarp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_PROXYARP_ENABLE]) 
            == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "pptpd_nat_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &pptpd_variables[PPTPD_NAT_ENABLE]) == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "pptpd_user_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_username", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &pptpd_variables[PPTPD_USERNAME]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "pptpd_user_rule%d_passwd", i);
        val = websGetVar(wp, tmp, "");
        /* The password is shown as a public key for the administrator. */
        if (valid_name(wp, val, &pptpd_variables[PPTPD_PASSWD]) == FALSE) {
            return FALSE;
        }
    }
    
    return TRUE;
}

int 
save_pptpd(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN], buf[TMP_LEN];    
    char all_rule[LONG_BUF_LEN];
    char *enable, *mtu, *localip, *localip_range, *remoteip, *remoteip_range,
         *dns_auto_enable, *dns, *chap_enable, *mschap_enable, 
         *mschapv2_enable, *eap_enable, *pap_enable, *mppe128_enable, 
         *proxyarp_enable, *nat_enable, *user_rule_num, *username, *passwd,
         *provider, *ipaddr, *name, *redialperiod;
    char *nv_rule1 = "pptpd_rule";
    char *nv_rule2 = "pptpd_user_rule",
         *nv_rule2_num = "pptpd_user_rule_num",
         *nv_rule2_max = "pptpd_user_rule_max";
    int nrule, len,change = 0, max;
    int64_t map = 0;
    int i;

    snprintf(tmp, sizeof(tmp), "pptpd_enable");
    enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_name");
    name = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_redialperiod");
    redialperiod = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_mtu");
    mtu = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_localip");
    localip = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_remoteip");
    remoteip = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_remoteip_range");
    remoteip_range = websGetVar(wp, tmp, "");
    localip_range = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_dns_auto_enable");
    dns_auto_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_dns");
    dns = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_chap_enable");
    chap_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_mschap_enable");
    mschap_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_mschapv2_enable");
    mschapv2_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_eap_enable");
    eap_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_pap_enable");
    pap_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_mppe128_enable");
    mppe128_enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "pptpd_proxyarp_enable");
    proxyarp_enable = websGetVar(wp, tmp, "");
    
    snprintf(tmp, sizeof(tmp), "pptpd_nat_enable");
    nat_enable = websGetVar(wp, tmp, "");

    /* mtu = mru in this case */
    len = snprintf(buf, TMP_LEN, 
                    "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
                    enable, name, redialperiod, mtu, mtu, localip, localip_range,
                    remoteip, remoteip_range, dns_auto_enable, dns,
                    chap_enable, mschap_enable, mschapv2_enable, eap_enable,
                    pap_enable, mppe128_enable, proxyarp_enable, nat_enable);
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
