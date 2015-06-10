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

#define WL_SSID_MAX_LENGTH "32"

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

enum {
    DHCP_DNSADDR = 0,
    WLV_SSID,
    WLV_SEC_WEP_KEY,
};

static struct variable exp_lan_variables[] = {
    {longname: "DNS Server", argv:ARGV(""), nullok: FALSE},
    {longname: "Wirieless SSID", argv:ARGV(WL_SSID_MAX_LENGTH)},
    {longname: "Wireless Key", argv:ARGV("")},
};

enum {
    MYQNAPCLOUD = 0,
    MYQNAPCLOUD_DEV,
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
valid_exp_lan(webs_t wp, char *value, struct variable *v)
{
    char *dns_type, *secmode, *ssid, *dnsaddr1, *dnsaddr2, *key;

    secmode = websGetVar(wp, "wl0_ssid0_secmode", "");
    if(!strcmp(secmode, "invalid")) {
        return TRUE;
    }
    key = websGetVar(wp, "wl0_ssid0_key", "");
    if( strcmp(secmode, "disabled") )
    {
        if (valid_base64(wp, key ,
                    &exp_lan_variables[WLV_SEC_WEP_KEY])
                == FALSE){
            return FALSE;
        }
    }
    dns_type = websGetVar(wp, "dhcp0_dnstype", "");

    if(strcmp(dns_type,"dnsrelay"))
    {
        /* DNS Server IP */
        dnsaddr1 = websGetVar(wp, "dhcp0_dnsaddr1", "");
        if (valid_ipaddr(wp, dnsaddr1, &exp_lan_variables[DHCP_DNSADDR]) == FALSE) {
            return FALSE;
        }
        dnsaddr2 = websGetVar(wp, "dhcp0_dnsaddr2", "");
        if (*dnsaddr2){
            if (valid_ipaddr(wp, dnsaddr2, &exp_lan_variables[DHCP_DNSADDR]) == FALSE) {
                return FALSE;
            }
        }
    }

    /* Wireless */
    ssid = websGetVar(wp, "wl0_ssid", "");
    if (valid_name(wp, ssid, &exp_lan_variables[WLV_SSID]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}
extern int
valid_qcloud(webs_t wp, char *value, struct variable *v);
extern int
valid_qcloud_dev(webs_t wp, char *value, struct variable *v);
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int
valid_exp_myqnapcloud(webs_t wp, char *value, struct variable *v)
{
    char *qcloud_qid;
    char *qcloud_device_name;
    char *btn_to_sign;
    char submit_button_action_in[] = "btn-to-signin";
    char submit_button_action_out[] = "btn-to-signout";

    /*get post value*/
    qcloud_qid = websGetVar(wp, "qcloud_qid", "");
    qcloud_device_name = websGetVar(wp, "qcloud_device_name", "");
    btn_to_sign = websGetVar(wp, "submit_button", "");

    char *wan0_ipaddr;
    wan0_ipaddr = nvram_safe_get("wan0_ipaddr");
    if (!strcmp(wan0_ipaddr, "")) {
        websDebugWrite(wp, "%s <br>", lang_translate("You have to choose a network"));
        return FALSE;
    }

    if (!(strcmp(submit_button_action_in, btn_to_sign))){ //signin 
        if (valid_qcloud(wp, qcloud_qid, &exp_qcloud_variables[MYQNAPCLOUD]) == FALSE) { //check_qid_account_pw
                return FALSE;
            } 
        //qid_account is null
        if (valid_qcloud_dev(wp, qcloud_device_name, &exp_qcloud_variables[MYQNAPCLOUD_DEV]) == FALSE) { //check_device
            return FALSE;
        }
    }
    return TRUE;
}
#endif

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
save_exp_lan(webs_t wp, char *value, struct variable *v, struct service *s)
{
    
    //char *qcloud_qid; 
   // qcloud_qid = websGetVar(wp, "qcloud_qid", "");
   // printf(">>>>>>>>>>>>>>> qcloud_id :%s >>>>>>",qcloud_qid);

    char tmp[TMP_LEN], buf[TMP_LEN], buf1[TMP_LEN], vpn_num_reset[TMP_LEN];
    char *ip, *start, *ssid, *secmode, *key, *keyc, *passwd;
    char *vpn_enable, *vpn_ip_server, *vpn_name, *vpn_pw;
    char *vpn_profile_enable, *vpn_profile_name;
    char vpn_ip_tmp[TMP_LEN], vpn_name_tmp[TMP_LEN], vpn_pw_tmp[TMP_LEN]; 
    char *vpn_num_rule, *vpn_max_rule;
    int len, change = 0;
    int vpn_num_rule_atoi, vpn_max_rule_atoi;
    int64_t map;
    int i, vpn_selected_index;
    char *vpn_select;
    map = 0;

    /* Wireless */
    ssid = websGetVar(wp, "wl0_ssid", "");
    secmode = websGetVar(wp, "wl0_ssid0_secmode", "");
    ezplib_get_rule("wl0_ssid_rule", 0, tmp, TMP_LEN);
    if(!strcmp(secmode, "invalid")) {
        if (strcmp(ssid, tmp)) {
            if(change == 0)
                config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule("wl0_ssid_rule", 0, ssid);
            config_postaction(map, s, "NUM=0", "");
            return 1;
        }
        else {
            return 0;
        }
    }
    if (strcmp(ssid, tmp)) {
        if(change == 0)
            config_preaction(&map, v, s, "NUM=0", "");
        ezplib_replace_rule("wl0_ssid_rule", 0, ssid);
        change = 1;
    }

    /* LAN */
    ip = websGetVar(wp, "lan0_static_ipclass", "");

    /* ip address */
    len =
        snprintf(tmp, TMP_LEN, "192.168.%s.1", ip);

    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
    if (strcmp(buf, tmp)) {
        if(change == 0)
            config_preaction(&map, v, s, "NUM=0", "");
        ezplib_replace_attr("lan_static_rule", 0, "ipaddr", tmp);
        change = 1;
    }
    /* DHCP */
    start = websGetVar(wp, "dhcp0_start", "");

    len = snprintf(buf, TMP_LEN, "%s", start);
    if (len >= TMP_LEN) {
        return 0;
    }
    ezplib_get_attr_val("lan_dhcps_rule", 0, "start", tmp, TMP_LEN, EZPLIB_USE_CLI);
    if (strcmp(buf, tmp)) {
        if(change == 0)
            config_preaction(&map, v, s, "NUM=0", "");
        ezplib_replace_attr("lan_dhcps_rule", 0, "start", start);
        change = 1;
    }
    ezplib_get_rule("wl0_sec_rule", 0, buf1, TMP_LEN);  
    if (!strcmp(secmode,"disabled")) {
        if (strcmp(secmode, buf1)) {
            if(change == 0)
                config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule("wl0_sec_rule", 0, secmode);
            change = 1;
        }
    }
    else if (!strcmp(secmode,"wep")) {
        key = websGetVar(wp, "wl0_ssid0_key", "");
        keyc = websGetVar(wp, "wl0_ssid0_keyc", "");
        ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key1", buf, TMP_LEN, EZPLIB_USE_CLI);
        if (strcmp(secmode, buf1) || strcmp(buf, keyc)) {
            if(change == 0)
                config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule("wl0_sec_rule", 0, secmode);
            ezplib_replace_attr("wl0_sec_wep_rule", 0, "key1", key);
            change = 1;
        }
    }        
    else if (!strcmp(secmode,"psk")) {
        key = websGetVar(wp, "wl0_ssid0_key", "");
        keyc = websGetVar(wp, "wl0_ssid0_keyc", "");
        ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "key", buf, TMP_LEN, EZPLIB_USE_CLI);
        if (strcmp(secmode, buf1) || strcmp(buf, keyc)) {
            if(change == 0)
                config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule("wl0_sec_rule", 0, secmode);
            ezplib_replace_attr("wl0_sec_wpa_rule", 0, "key", key);
            change = 1;
        }
    }
    else if (!strcmp(secmode,"psk2")) {
        key = websGetVar(wp, "wl0_ssid0_key", "");
        keyc = websGetVar(wp, "wl0_ssid0_keyc", "");
        ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "key", buf, TMP_LEN, EZPLIB_USE_CLI);
        if (strcmp(secmode, buf1) || strcmp(buf, keyc)) {
            if(change == 0)
                config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule("wl0_sec_rule", 0, secmode);
            ezplib_replace_attr("wl0_sec_wpa2_rule", 0, "key", key);
            change = 1;
        }
    }
    /* password */
    passwd = websGetVar(wp, "http_passwd", "");
    if(strcmp(passwd,"") != 0) {
        len =
            snprintf(tmp, TMP_LEN, "%s", passwd);

        if (len > TMP_LEN - 1) {
            return FALSE;
        }
        ezplib_get_attr_val("http_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI);
        if (strcmp(buf, tmp)) {
            if(change == 0)
                config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_attr("http_rule", 0, "passwd", passwd);
            change = 1;
        }
    }
    /*vpn*/
    vpn_enable = websGetVar(wp, "vpn_enable_id", "");
    vpn_profile_enable = websGetVar(wp, "vpn_profile_enable", "");
    vpn_select = websGetVar(wp, "vpn_select", "");
    vpn_ip_server = websGetVar(wp, "vpn_ip_server", "");
    vpn_name = websGetVar(wp, "vpn_name", "");
    vpn_profile_name = websGetVar(wp, "vpn_profile_name", "");
    vpn_pw = websGetVar(wp, "vpn_password", "");

    vpn_num_rule = nvram_safe_get("wan_pptp_l2tp_rule_num");
    vpn_max_rule = nvram_safe_get("wan_pptp_l2tp_rule_max");
    vpn_num_rule_atoi = atoi(vpn_num_rule);
    vpn_max_rule_atoi = atoi(vpn_max_rule);
    /* we check
     * 1. vpn enabled
     * 2. construct sub-rule
     * 3. if vpn_select > vpn_max_rule
     *   3.1 if vpn_num_rule >= vpn_max_rule, ##ignore##
     *   3.2 else vpn_num_rule++ ; vpn_select = vpn_num_rule
     * 4. according to vpn_select, compare constructed sub-rule with nvram sub-rule
     * 5. 1 equal and 4 equal
     *   5.1 yes, not change
     *   5.2 no, preaction, update, postaction
     * */
    /* make sure vpn_enable and vpn_select must set */

    if ((strcmp(vpn_enable,"") != 0) && (strcmp(vpn_select,"") != 0)) {
        if(change == 0)
            config_preaction(&map, v, s, "NUM=0", "");
        vpn_selected_index = atoi(vpn_select) - 1;
        if(vpn_selected_index > vpn_max_rule_atoi) {
            /* selected > max, means App/WebGUI try to add a profile, we first check if max rule exceed, if yes, to max exceed policy */
            if(vpn_num_rule_atoi >= vpn_max_rule_atoi) {
                /* FIXME !!! Do max rule exceed policy */
                vpn_selected_index = vpn_num_rule_atoi - 1;
            } else {
                vpn_selected_index = vpn_num_rule_atoi;
                vpn_num_rule_atoi++;
                change = 1;
            }
        } else if(vpn_selected_index >= vpn_num_rule_atoi) {
            vpn_selected_index = vpn_num_rule_atoi;
            vpn_num_rule_atoi++;
            change = 1;
        }
        /* checking for empty string */
        /* set profile to be deleted and set enable to negetive */
        if(!vpn_ip_server[0] || !vpn_name[0] || !vpn_pw[0]) {
            vpn_ip_server = "";
            vpn_name = "";
            vpn_pw = "";
            vpn_profile_enable = "0";
        }
        if(!vpn_enable || !vpn_enable[0]) {
            vpn_enable = "0";
        } else if(vpn_enable[0] != '0' && vpn_enable[0] != '1') {
            vpn_enable = "1";
        }
        if(!vpn_profile_enable || !vpn_profile_enable[0]) {
            vpn_profile_enable = "0";
        } else if(vpn_profile_enable[0] != '0' && vpn_profile_enable[0] != '1') {
            vpn_profile_enable = "1";
        }
        /* Then we reconstruct i-th rule */
        /* default 0^^pptp^^static^^^^^^1^300^isp^^^chap */
        /* enable^name^vpn_type^host^ipaddr^netmask^gateway^username^
         * passwd^nailup^timeout^pppiptype[isp|custom]^custom_ipaddr^
         * encmode[disabled|mppe]^auth[chap|pap|none] */
        printf("vpn_enable:%s vpn_profile_enable:%s vpn_selected_index:%d\n", vpn_enable, vpn_profile_enable, vpn_selected_index);
        len = snprintf(buf, sizeof(buf), "%s^%s^pptp^%s^static^^^%s^%s^^1^300^isp^^^chap", 
                vpn_profile_enable, vpn_profile_name, vpn_ip_server, vpn_name, vpn_pw);
        if(len >= sizeof(buf)) {
            /* Buffer overflow, return */
            return FALSE;
        }
        /* Then we reconstruct vpn global rule */
        /* default 0^0^0 */
        /* enable[0:disabled|1:enabled]^selected(selected profile index)^enabled(enabled profile index) */
        snprintf(tmp, sizeof(tmp), "%s^%d^%d", 
                vpn_enable, vpn_selected_index, vpn_selected_index);

        ezplib_get_rule("wan_pptp_l2tp_rule", vpn_selected_index, buf1, sizeof(buf1));

        printf("tmp:%s wan_pptp_l2tp_global_rule:%s buf:%s buf1:%s\n", tmp, nvram_safe_get("wan_pptp_l2tp_global_rule"), buf, buf1);
        if(strcmp(tmp, nvram_safe_get("wan_pptp_l2tp_global_rule"))) {
            nvram_set("wan_pptp_l2tp_global_rule", tmp);
            change = 1;
        }
        if(strcmp(buf, buf1)) {
            ezplib_replace_rule("wan_pptp_l2tp_rule", vpn_selected_index, buf);
            change = 1;
        }
        /* Finally, we cleanup multiple enabled items */
        int valid_profile_num = 0;
        int enable_profile_num = 0;
        int selected_profile_enabled = vpn_profile_enable[0] - '0';
        int selected_profile_enabled_in_profile = 0;
        int first_enabled_profile = -1;
        for(i = 0;i < vpn_max_rule_atoi;i++) {
            // ezplib_get_rule("wan_pptp_l2tp_rule", i, tmp, sizeof(tmp));
            ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "enable", tmp, sizeof(tmp), EZPLIB_USE_CLI);
            ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "host", vpn_ip_tmp, TMP_LEN, EZPLIB_USE_CLI);
            ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "username", vpn_name_tmp, TMP_LEN, EZPLIB_USE_CLI);
            ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "passwd", vpn_pw_tmp, TMP_LEN, EZPLIB_USE_CLI);
            if(tmp[0] != '0') {
                enable_profile_num ++;
                if((i == vpn_selected_index) && selected_profile_enabled){
                    selected_profile_enabled_in_profile = 1;
                }
                if(first_enabled_profile < 0) {
                    first_enabled_profile = i;
                }
            }
            if(strcmp(vpn_ip_tmp, "") && strcmp(vpn_name_tmp, "") && strcmp(vpn_pw_tmp, "")) {
                valid_profile_num ++;
            }
        }
        /* Now we have 4 cases : 
         * 0 profile enabled ==> enable first profile
         * 1 profile enabled ==> do nothing
         * >1 profile enabled and selected profile enabled ==> disable not selected profiles
         * >1 profile enabled and selected profile not enabled ==> disable second and later profiles */
        int min_activated_one_rule = 1;
        int activate_multiple_rules = 0;
        if(valid_profile_num) {
            if(enable_profile_num == 0 && min_activated_one_rule) {
                ezplib_replace_attr("wan_pptp_l2tp_rule", 0, "enable", "1");
                change = 1;
            } else if(enable_profile_num > 1 && !activate_multiple_rules){
                if(selected_profile_enabled_in_profile) {
                    for(i = 0;i < vpn_max_rule_atoi;i++) {
                        if(i != vpn_selected_index) {
                            ezplib_replace_attr("wan_pptp_l2tp_rule", i, "enable", "0");
                        }
                    }
                } else {
                    for(i = (first_enabled_profile + 1);i < vpn_max_rule_atoi;i++) {
                        ezplib_replace_attr("wan_pptp_l2tp_rule", i, "enable", "0");
                    }
                } 
                change = 1;
            }
        }
        /* Some profile been deleted */
        if(vpn_num_rule_atoi > valid_profile_num) {
            for(i = (vpn_num_rule_atoi - 1);i >= 0;i --) {
                ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "host", vpn_ip_tmp, TMP_LEN, EZPLIB_USE_CLI);
                ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "username", vpn_name_tmp, TMP_LEN, EZPLIB_USE_CLI);
                ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "passwd", vpn_pw_tmp, TMP_LEN, EZPLIB_USE_CLI);
                if(!vpn_ip_tmp[0] || !vpn_name_tmp[0] || !vpn_pw_tmp[0]) {
                    ezplib_delete_rule("wan_pptp_l2tp_rule", i);
                    ezplib_append_rule("wan_pptp_l2tp_rule", "0^^pptp^^static^^^^^^1^300^isp^^^chap");
                    vpn_num_rule_atoi --;
                    change = 1;
                }
            }
            if(vpn_num_rule_atoi != valid_profile_num) {
                fprintf(stderr , "valid rule number wrong valid_profile_num:%d vpn_num:%d\n", valid_profile_num, vpn_num_rule_atoi);
            }
        }
        if(vpn_num_rule[0] != (vpn_num_rule_atoi + '0')) {
            snprintf(vpn_num_reset, sizeof(vpn_num_reset), "%d", vpn_num_rule_atoi);
            nvram_set("wan_pptp_l2tp_rule_num", vpn_num_reset);
            change = 1;
        }
        /*
        if (!strcmp(vpn_enable,"0")) { //dis
            for (i = 0; i < vpn_max_rule_atoi; i++ ) {
                ezplib_replace_attr("wan_pptp_l2tp_rule", i, "enable", vpn_enable);
                set_tag = 1;
            }
        } else { //enable = 1
            ezplib_get_attr_val("wan_pptp_l2tp_rule", vpn_selected_index, "host", vpn_ip_tmp, TMP_LEN, EZPLIB_USE_CLI);
            ezplib_get_attr_val("wan_pptp_l2tp_rule", vpn_selected_index, "username", vpn_name_tmp, TMP_LEN, EZPLIB_USE_CLI);
            ezplib_get_attr_val("wan_pptp_l2tp_rule", vpn_selected_index, "passwd", vpn_pw_tmp, TMP_LEN, EZPLIB_USE_CLI);
            for (i = 0; i < vpn_max_rule_atoi; i++ ) {
                // if( i == vpn_selected_index){
                //   ezplib_replace_attr("wan_pptp_l2tp_rule", i, "enable", vpn_enable);
                // } else {
                ezplib_replace_attr("wan_pptp_l2tp_rule", i, "enable","0");
                //  }
            }
            if (strcmp(vpn_ip_tmp,"") != 0 && (strcmp(vpn_name_tmp,"") != 0) && (strcmp(vpn_pw_tmp,"") != 0)) { //open old
                if ((strcmp(vpn_ip_server,vpn_ip_tmp) == 0) && (strcmp(vpn_name,vpn_name_tmp) == 0) && (strcmp(vpn_pw,vpn_pw_tmp) == 0)) {//open old 
                    ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "enable","1");
                    set_tag = 1;
                } else { //chang old val
                    if ((strcmp(vpn_ip_server,"") != 0) && (strcmp(vpn_name,"") != 0) && (strcmp(vpn_pw,"") != 0)){
                        ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "enable","1");
                        ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "host", vpn_ip_server);
                        ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "username", vpn_name);
                        ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "passwd", vpn_pw);
                        set_tag = 1;
                    }
                    set_tag = 0;
                }
            }
        }
        if (vpn_num_rule_atoi <= vpn_max_rule_atoi) {
            if ((strcmp(vpn_ip_server,"") != 0) && (strcmp(vpn_name,"") != 0) && (strcmp(vpn_pw,"") != 0) && (set_tag == 0)) { //add
                ezplib_get_attr_val("wan_pptp_l2tp_rule", vpn_selected_index, "host", vpn_ip_tmp, TMP_LEN, EZPLIB_USE_CLI);
                ezplib_get_attr_val("wan_pptp_l2tp_rule", vpn_selected_index, "username", vpn_name_tmp, TMP_LEN, EZPLIB_USE_CLI);
                ezplib_get_attr_val("wan_pptp_l2tp_rule", vpn_selected_index, "passwd", vpn_pw_tmp, TMP_LEN, EZPLIB_USE_CLI);
                for (i = 0; i < vpn_max_rule_atoi; i++ ) {
                    ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "host", vpn_ip_tmp, TMP_LEN, EZPLIB_USE_CLI);
                    ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "username", vpn_name_tmp, TMP_LEN, EZPLIB_USE_CLI);
                    ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "passwd", vpn_pw_tmp, TMP_LEN, EZPLIB_USE_CLI);
                    if (strcmp(vpn_ip_tmp,"") != 0 && (strcmp(vpn_name_tmp,"") != 0) && (strcmp(vpn_pw_tmp,"") != 0) && (set_change == 0)) {
                        vpn_num_set = vpn_num_set + 1;
                    } else if (strcmp(vpn_ip_tmp,"") == 0 && (strcmp(vpn_name_tmp,"") == 0) && (strcmp(vpn_pw_tmp,"") == 0) && (set_change == 0)) {
                        ezplib_replace_attr("wan_pptp_l2tp_rule", i, "enable", vpn_enable);
                        ezplib_replace_attr("wan_pptp_l2tp_rule", i, "host", vpn_ip_server);
                        ezplib_replace_attr("wan_pptp_l2tp_rule", i, "username", vpn_name);
                        ezplib_replace_attr("wan_pptp_l2tp_rule", i, "passwd", vpn_pw);
                        set_change = 1;
                        vpn_num_set = vpn_num_set + 1;
                    } else {
                        continue;
                        ;
                    }
                }
                change = 1;
            } else if ((strcmp(vpn_ip_server,"") == 0) && (strcmp(vpn_name,"") == 0) && (strcmp(vpn_pw,"") == 0) && (set_tag == 0)) { //del
                ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "enable","0");
                ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "host", "");
                ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "username", "");
                ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_selected_index, "passwd", "");

                for (i = 0; i < vpn_max_rule_atoi; i++ ) {
                    ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "host", vpn_ip_tmp, TMP_LEN, EZPLIB_USE_CLI);
                    ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "username", vpn_name_tmp, TMP_LEN, EZPLIB_USE_CLI);
                    ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "passwd", vpn_pw_tmp, TMP_LEN, EZPLIB_USE_CLI);
                    if (strcmp(vpn_ip_tmp,"")!=0 && (strcmp(vpn_name_tmp,"") != 0) && (strcmp(vpn_pw_tmp,"") != 0)) {
                        ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_num_set, "enable","0");
                        ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_num_set, "host", vpn_ip_tmp);
                        ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_num_set, "username", vpn_name_tmp);
                        ezplib_replace_attr("wan_pptp_l2tp_rule", vpn_num_set, "passwd", vpn_pw_tmp);
                        vpn_num_set = vpn_num_set + 1;
                        vpn_num = i - vpn_num_set;
                        if ( vpn_num == 0) {
                            ezplib_replace_attr("wan_pptp_l2tp_rule", i, "enable","0");
                            ezplib_replace_attr("wan_pptp_l2tp_rule", i, "host", "");
                            ezplib_replace_attr("wan_pptp_l2tp_rule", i, "username", "");
                            ezplib_replace_attr("wan_pptp_l2tp_rule", i, "passwd", "");
                        }
                    }
                }
                change = 1;
            } else {
                vpn_num_set = vpn_num_rule_atoi;
                change = 1;
            }
            sprintf(vpn_num_reset,"%d",vpn_num_set);
            ezplib_replace_rule("wan_pptp_l2tp_rule_num", 0, vpn_num_reset);
        }
        */
    }

    if (change) {
        snprintf(tmp, TMP_LEN, "NUM=0");
        config_postaction(map, s, tmp, "");
    }

    return change;
}
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int
save_exp_myqnapcloud(webs_t wp, char *value, struct variable *v, struct service *s)
{   
    char tmp[TMP_LEN];
    int num;
    int64_t map;

    FILE *f_device_id, *f_device_id_js;
    int change = 0;
    char *qcloud_device_name;
    char *submit_button_action; 
    char submit_button_action_in[] = "btn-to-signin";
    char submit_button_action_out[] = "btn-to-signout";
    char submit_button_action_test_portal[] = "qcloud_test_portal";

    int qid_bind_dev_name_ret;
    int qid_update_device_info_ret;
    int qid_unbind_dev_name_ret;
    int qid_publish_service_ret;
    int qid_update_service_port_by_upnpc_ret;
    int qid_btn_test_fetch_action_ret;

    char *wan0_ipaddr;
    wan0_ipaddr = nvram_safe_get("wan0_ipaddr");
    if (!strcmp(wan0_ipaddr, "")) {
        websDebugWrite(wp, "%s <br>", lang_translate("You have to choose a network"));
        return FALSE;
    }

    qcloud_device_name = websGetVar(wp, "qcloud_device_name", "");
    submit_button_action = websGetVar(wp, "submit_button", "");
    
    qid_update_device_info_ret = qid_update_device_info();
    
    if (!strcmp(submit_button_action_in, submit_button_action)) { //signin bind device
         qid_bind_dev_name_ret = qid_bind_dev_name(qcloud_device_name);
         if (qid_bind_dev_name_ret == 0) { //
            qid_publish_service_ret = qid_publish_service();//qid publish service
            qid_update_service_port_by_upnpc_ret = qid_update_service_port_by_upnpc();//qid servic port by upnpc 
            change = 1 ;
         }
    } else if (!strcmp(submit_button_action_out, submit_button_action)) {   //signout
        if (qid_update_device_info_ret == 0) { //unbind
            qid_unbind_dev_name_ret = qid_unbind_dev_name();
            change = 1;
        }
    
    } else if (!strcmp(submit_button_action_test_portal, submit_button_action)) {   //test_fetch
        qid_btn_test_fetch_action_ret = qid_btn_test_fetch_action();
        change = 1;
    }
        if (change) {
            snprintf(tmp, TMP_LEN, "NUM=0");
            config_postaction(map, s, tmp, "");
        }
    return change;
}
#endif

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

