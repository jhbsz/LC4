#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include "ezp.h"
#include "ezp-lib.h"
#include "lang.h"

#include "fcgi_common.h"
#include "fcgi_stdio.h"
#include "http_common.h"

#define WL_SSID_MAX_LENGTH "32"

enum {
    LAN_IPADDR = 0,
    LAN_NETMASK,
    LAN_MTU,
    LAN_STP
};
/*
static struct variable lan_variables[] = {
    {longname: "LAN IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "LAN Netmask", argv:ARGV("8", "32"), nullok: FALSE},
    {longname: "LAN MTU", argv:ARGV("600","1500"), nullok: FALSE},
    {longname: "LAN STP", argv:ARGV("0", "1"), nullok: FALSE},
};
*/
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

int
valid_exp_lan(INPUT *input, char *value, struct variable *v)
{
    char *dns_type, *secmode, *ssid, *dnsaddr1, *dnsaddr2, *key;
    INPUT *tmp;

    if ((tmp = CGI_Find_Parameter(input, "wl0_ssid0_secmode"))) {
        secmode = tmp->val;
    }
    else
        secmode = "";

    if(strcmp(secmode, "")) {
        if(!strcmp(secmode, "invalid")) {
            return TRUE;
        }
        if ((tmp = CGI_Find_Parameter(input, "wl0_ssid0_key"))) {
            key = tmp->val;
        }
        else
            key = "";

        if( strcmp(secmode, "disabled") )
        {
            if (valid_base64(input, key ,
                        &exp_lan_variables[WLV_SEC_WEP_KEY])
                    == FALSE){
              return FALSE;
            }
        }
    }

    if ((tmp = CGI_Find_Parameter(input, "dhcp0_dnstype"))) {
        dns_type = tmp->val;
    }
    else
        dns_type = "";

    if(strcmp(dns_type, "") && strcmp(dns_type,"dnsrelay"))
    {
        /* DNS Server IP */
        if ((tmp = CGI_Find_Parameter(input, "dhcp0_dnsaddr1"))) {
            dnsaddr1 = tmp->val;
        }
        else
            dnsaddr1 = "";

        if (valid_ipaddr(input, dnsaddr1, &exp_lan_variables[DHCP_DNSADDR]) == FALSE) {
            return FALSE;
        }
        if ((tmp = CGI_Find_Parameter(input, "dhcp0_dnsaddr2"))) {
            dnsaddr2 = tmp->val;
        }
        else
            dnsaddr2 = "";

        if (*dnsaddr2){
            if (valid_ipaddr(input, dnsaddr2, &exp_lan_variables[DHCP_DNSADDR]) == FALSE) {
                return FALSE;
            }
        }
    }

    /* Wireless */
    if ((tmp = CGI_Find_Parameter(input, "wl0_ssid"))) {
        ssid = tmp->val;
    }
    else
        ssid = "";

    if(strcmp(ssid, "")) {
        if (valid_name(input, ssid, &exp_lan_variables[WLV_SSID]) == FALSE) {
            return FALSE;
        }
    }
    
    return TRUE;
}

int
save_exp_lan(INPUT *input, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN], buf[TMP_LEN], buf1[TMP_LEN];
    char *ip, *start, *ssid, *secmode, *key, *keyc, *passwd;
    char *allow_guest_new, *allow_guest_ori;
    char *allow_ftp_new;
    char allow_ftp_ori[TMP_LEN];
    int len, change = 0;
    int64_t map;
    INPUT *tmp_cgi;
    map = 0;

    /* Wireless */
    if ((tmp_cgi = CGI_Find_Parameter(input, "wl0_ssid0_secmode"))) {
        secmode = tmp_cgi->val;
    }
    else
        secmode = "";

    if ((tmp_cgi = CGI_Find_Parameter(input, "wl0_ssid"))) {
        ssid = tmp_cgi->val;
    }
    else
        ssid = "";

    if(strcmp(ssid, "") && strcmp(secmode, "")) {
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
    }

    /* LAN */
    if ((tmp_cgi = CGI_Find_Parameter(input, "lan0_static_ipclass"))) {
        ip = tmp_cgi->val;
    }
    else
        ip = "";

    if(strcmp(ip, "")) {
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
    }

    /* DHCP */
    if ((tmp_cgi = CGI_Find_Parameter(input, "dhcp0_start"))) {
        start = tmp_cgi->val;
    }
    else
        start = "";

    if(strcmp(start, "")) {
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
        if ((tmp_cgi = CGI_Find_Parameter(input, "wl0_ssid0_key"))) {
            key = tmp_cgi->val;
        }
        else
            key = "";

        if ((tmp_cgi = CGI_Find_Parameter(input, "wl0_ssid0_keyc"))) {
            keyc = tmp_cgi->val;
        }
        else
            keyc = "";

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
        if ((tmp_cgi = CGI_Find_Parameter(input, "wl0_ssid0_key"))) {
            key = tmp_cgi->val;
        }
        else
            key = "";

        if ((tmp_cgi = CGI_Find_Parameter(input, "wl0_ssid0_keyc"))) {
            keyc = tmp_cgi->val;
        }
        else
            keyc = "";

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
        if ((tmp_cgi = CGI_Find_Parameter(input, "wl0_ssid0_key"))) {
            key = tmp_cgi->val;
        }
        else
            key = "";

        if ((tmp_cgi = CGI_Find_Parameter(input, "wl0_ssid0_keyc"))) {
            keyc = tmp_cgi->val;
        }
        else
            keyc = "";
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
    if ((tmp_cgi = CGI_Find_Parameter(input, "http_passwd"))) {
        passwd = tmp_cgi->val;
    }
    else
        passwd = "";

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

    if ((tmp_cgi = CGI_Find_Parameter(input, "allow_guest"))) {
        allow_guest_new = tmp_cgi->val;
    }
    else
        allow_guest_new = "";

    FCGI_LOG("Get allow guest option %s", passwd);
    allow_guest_ori = nvram_safe_get("allow_guest");
    /* We prevent old version app setting without allow_guest parameter to
     * interference our setting. */
    if(strcmp(allow_guest_new, "")) {
        /* If allow guest setting is not exist or setting changed */
        if(!allow_guest_ori || !allow_guest_ori[0] || strcmp(allow_guest_ori, allow_guest_new)) {
            if(change == 0)
                config_preaction(&map, v, s, "NUM=0", "");
            nvram_fset("allow_guest", allow_guest_new);
            change = 1;
        }
    }
    
    if ((tmp_cgi = CGI_Find_Parameter(input, "allow_ftpd"))) {
        allow_ftp_new = tmp_cgi->val;
    }
    else
        allow_ftp_new = "";

    ezplib_get_attr_val("ftpd_rule", 0, "enable", allow_ftp_ori, TMP_LEN, EZPLIB_USE_CLI);

    if(strcmp(allow_ftp_new, "")) {
        /* If allow guest setting is not exist or setting changed */
        if((allow_ftp_ori[0] != '\0')|| strcmp(allow_ftp_ori, allow_ftp_new)) {
            if(change == 0)
                config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_attr("ftpd_rule", 0, "enable", allow_ftp_new);
            change = 1;
        }
    }

    /*vpn
    if ((tmp_cgi = CGI_Find_Parameter(input, "vpn_enable_id"))) {
        vpn_enable = tmp_cgi->val;
    }
    if ((tmp_cgi = CGI_Find_Parameter(input, "vpn_profile_enable"))) {
        vpn_profile_enable = tmp_cgi->val;
    }
    if ((tmp_cgi = CGI_Find_Parameter(input, "vpn_select"))) {
        vpn_select = tmp_cgi->val;
    }
    if ((tmp_cgi = CGI_Find_Parameter(input, "vpn_ip_server"))) {
        vpn_ip_server = tmp_cgi->val;
    }
    if ((tmp_cgi = CGI_Find_Parameter(input, "vpn_name"))) {
        vpn_name = tmp_cgi->val;
    }
    if ((tmp_cgi = CGI_Find_Parameter(input, "vpn_profile_name"))) {
        vpn_profile_name = tmp_cgi->val;
    }
    if ((tmp_cgi = CGI_Find_Parameter(input, "vpn_password"))) {
        vpn_pw = tmp_cgi->val;
    }*/

    if (change) {
        snprintf(tmp, TMP_LEN, "NUM=0");
        config_postaction(map, s, tmp, "");
    }

    return change;
}

