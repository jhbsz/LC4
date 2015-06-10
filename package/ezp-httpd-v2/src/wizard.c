#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include "assert.h"

#include "lang.h"
#include "common.h"
#include "ezp-lib.h"
#include "ezp.h"
#include "ezpcom-lib.h"
#include "shared.h"

enum {
    WIZARD_ADMIN_PW = 0,
	WIZARD_MODE ,
	WIZARD_WAN_ENABLE,
	WIZARD_WAN_TYPE,
	WIZARD_WAN_PPPOE_NAME,
	WIZARD_WAN_PPPOE_PASSWD,
	WIZARD_WAN_STATIC_IP,
	WIZARD_WAN_STATIC_MASK,
	WIZARD_WAN_STATIC_GW,
	WIZARD_WAN_STATIC_DNS1,
	WIZARD_WAN_STATIC_DNS2,
	WIZARD_BANDWIDTH_ENABLE,
	WIZARD_BANDWIDTH_ETH,
	WIZARD_BANDWIDTH_ETH_UP,
	WIZARD_BANDWIDTH_ETH_DOWN,
	WIZARD_BANDWIDTH_MOBILE,
	WIZARD_BANDWIDTH_MOBILE_UP,
	WIZARD_BANDWIDTH_MOBILE_DOWN,
	WIZARD_TIME_POOL,
	WIZARD_TIME_DAYLIGHT,
	WIZARD_TIME_ZONE,
	WIZARD_WL_ENABLE,
	WIZARD_WL_SSID,
	WIZARD_WL_SEC_ENABLE,
	WIZARD_WL_SEC_MODE,
	WIZARD_WL_SEC_METHOD,
	WIZARD_WL_SEC_KEY,
	WIZARD_MOBILE_ENABLE,
	WIZARD_MOBILE_APN_TYPE,
	WIZARD_MOBILE_PROVIDER,
	WIZARD_MOBILE_APN,
	WIZARD_MOBILE_PIN,
	WIZARD_MOBILE_NAME,
	WIZARD_MOBILE_PASSWD,
};

enum {
    DHCP = 0,
    PPPOE = 1,
    STATIC = 2,
    WWAN = 3,
    HTC = 4,
    DIRECTIP = 5,
    IPHONE = 6
};

static struct variable wizard_variables[] = {
    {longname: "Wizard Administrator password", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard Manager Mode", argv:ARGV("10"), nullok: FALSE},
    {longname: "Wizard Wan Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Wizard Wan Type", argv:ARGV("0", "5"), nullok: FALSE},
    {longname: "Wizard Wan PPPoE User Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard Wan PPPoE Password", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard Wan Static Ip", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard Wan Static Mask", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard Wan Static Gateway", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard Wan Static DNS 1", argv:ARGV(""), nullok: TRUE},
    {longname: "Wizard Wan Static DNS 2", argv:ARGV(""), nullok: TRUE},
    {longname: "Wizard Bandwidth Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Wizard Bandwidth Ethernet Type", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard Bandwidth Ethernet Uploac", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard Bandwidth Ethernet Download", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard Bandwidth Mobile Type", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard Bandwidth Mobile Uploac", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard Bandwidth Mobile Download", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard Time Area", argv:ARGV("pool.ntp.org", "asia.pool.ntp.org", "europe.pool.ntp.org", "north-america.pool.ntp.org", "south-america.pool.ntp.org", "africa.pool.ntp.org"), nullok: FALSE},
    {longname: "Wizard Time Daylight Saving", argv:ARGV("on", ""), nullok: TRUE},
    {longname: "Wizard Time Zone", argv:ARGV("64"), nullok: FALSE},
    {longname: "Wizard Wireless Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Wizard Wireless SSID", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard Wireless Security Enable", argv:ARGV("on", ""), nullok: TRUE},
    {longname: "Wizard Wireless Security Mode", argv:ARGV("0", "1", "2"), nullok: FALSE},
    {longname: "Wizard Wireless Security Method", argv:ARGV("aes", "tkip"), nullok: FALSE},
    {longname: "Wizard Wireless Security Key", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard Mobile Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Wizard Mobile APN Type", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Wizard Mobile Service Provider", argv:ARGV("0", "20"), nullok: FALSE},
    {longname: "Wizard Mobile APN Number", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard Mobile PIN Number", argv:ARGV("20"), nullok: TRUE},
    {longname: "Wizard Mobile User Name", argv:ARGV("20"), nullok: TRUE},
    {longname: "Wizard Mobile Password", argv:ARGV("20"), nullok: TRUE},
};

int
valid_wizard(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int admin, mode, wan_trad, wan_wwan, bw, time, wifi;
    ezplib_get_attr_val("wizard_rule", 0, "mode", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    admin = atoi(tmp);
    ezplib_get_attr_val("wizard_rule", 0, "mode", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    mode = atoi(tmp);
    ezplib_get_attr_val("wizard_rule", 0, "wan_trad", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    wan_trad = atoi(tmp);
    ezplib_get_attr_val("wizard_rule", 0, "wan_wwan", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    wan_wwan = atoi(tmp);
    ezplib_get_attr_val("wizard_rule", 0, "bw", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    bw = atoi(tmp);
    ezplib_get_attr_val("wizard_rule", 0, "time", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    time = atoi(tmp);
    ezplib_get_attr_val("wizard_rule", 0, "wifi", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    wifi = atoi(tmp);

    /* Admin password */
    if (admin) {
        snprintf(tmp, sizeof(tmp), "wizard_adminpw");
        val = websGetVar(wp, tmp, "0");
        if (valid_name(wp, val, &wizard_variables[WIZARD_ADMIN_PW]) == FALSE) {
            return FALSE;
        } 
    }
    if (mode) {
        /* Check Operation Mode */
        snprintf(tmp, sizeof(tmp), "wizard_model");
        val = websGetVar(wp, tmp, "0");
        if (valid_name(wp, val, &wizard_variables[WIZARD_MODE]) == FALSE) {
            return FALSE;
        } 
    }

    /* Check WAN Ethernet Type */
    snprintf(tmp, sizeof(tmp), "wan_eth_enable");
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val, &wizard_variables[WIZARD_WAN_ENABLE]) == FALSE) {
        return FALSE;
    } 
    if (atoi(val)) {
        snprintf(tmp, sizeof(tmp), "wizard_wan_type_value");
        val = websGetVar(wp, tmp, "0");
        if (valid_range(wp, val, &wizard_variables[WIZARD_WAN_TYPE]) == FALSE) {
            return FALSE;
        } 

        switch (atoi(val))  {
            case DHCP:
                nvram_fset("wan0_ifname", "vlan2");
                nvram_fset("wan0_device", "vlan2");
                nvram_fset("wan0_proto", "dhcp");
                break;
            case PPPOE: /* PPPoE */
                snprintf(tmp, sizeof(tmp), "wizard_wan0_ppp_username");
                val = websGetVar(wp, tmp, "0");
                if (valid_name(wp, val, 
                            &wizard_variables[WIZARD_WAN_PPPOE_NAME]) == FALSE) {
                    return FALSE;
                } 
                snprintf(tmp, sizeof(tmp), "wizard_wan0_ppp_passwd");
                val = websGetVar(wp, tmp, "0");
                if (valid_name(wp, val,
                            &wizard_variables[WIZARD_WAN_PPPOE_PASSWD]) == FALSE) {
                    return FALSE;
                } 
                nvram_fset("wan0_ifname", "ppp0");
                nvram_fset("wan0_device", "vlan2");
                nvram_fset("wan0_proto", "pppoe");
                break;
            case STATIC: /* Static IP */
                snprintf(tmp, sizeof(tmp), "wizard_wan0_static_ip");
                val = websGetVar(wp, tmp, "0");
                if (valid_ipaddr(wp, val, 
                            &wizard_variables[WIZARD_WAN_STATIC_IP]) == FALSE) {
                    return FALSE;
                } 
                snprintf(tmp, sizeof(tmp), "wizard_wan0_static_mask");
                val = websGetVar(wp, tmp, "0");
                if (valid_netmask(wp, val,
                            &wizard_variables[WIZARD_WAN_STATIC_MASK]) == FALSE) {
                    return FALSE;
                } 
                snprintf(tmp, sizeof(tmp), "wizard_wan0_static_gateway");
                val = websGetVar(wp, tmp, "0");
                if (valid_ipaddr(wp, val,
                            &wizard_variables[WIZARD_WAN_STATIC_GW]) == FALSE) {
                    return FALSE;
                } 
                snprintf(tmp, sizeof(tmp), "wizard_wan0_static_dns1");
                val = websGetVar(wp, tmp, "0");
                if (valid_ipaddr(wp, val, 
                            &wizard_variables[WIZARD_WAN_STATIC_DNS1]) == FALSE) {
                    return FALSE;
                } 
                snprintf(tmp, sizeof(tmp), "wizard_wan0_static_dns2");
                val = websGetVar(wp, tmp, "0");
                if (valid_ipaddr(wp, val, 
                            &wizard_variables[WIZARD_WAN_STATIC_DNS2]) == FALSE) {
                    return FALSE;
                } 
                nvram_fset("wan0_ifname", "vlan2");
                nvram_fset("wan0_device", "vlan2");
                nvram_fset("wan0_proto", "static");
                break;
        }
    }
    /* WAN Mobile */
    snprintf(tmp, sizeof(tmp), "wizard_mobile_enable");
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val,
                &wizard_variables[WIZARD_MOBILE_ENABLE]) == FALSE) {
        return FALSE;
    } 
    if (atoi(val)) {
        snprintf(tmp, sizeof(tmp), "wizard_wwan_apn_type");
        val = websGetVar(wp, tmp, "0");
        if (valid_range(wp, val,
                    &wizard_variables[WIZARD_MOBILE_APN_TYPE]) == FALSE) {
            return FALSE;
        } 
        if (atoi(val) == 0) {
            snprintf(tmp, sizeof(tmp), "wizard_wwan_isp");
            val = websGetVar(wp, tmp, "0");
            if (valid_range(wp, val,
                        &wizard_variables[WIZARD_MOBILE_PROVIDER]) == FALSE) {
                return FALSE;
            } 
        } else {
            snprintf(tmp, sizeof(tmp), "wizard_wwan_apn");
            val = websGetVar(wp, tmp, "0");
            if (valid_name(wp, val,
                        &wizard_variables[WIZARD_MOBILE_APN]) == FALSE) {
                return FALSE;
            } 
        }
        snprintf(tmp, sizeof(tmp), "wizard_wwan_pin");
        val = websGetVar(wp, tmp, "0");
        if (valid_number(wp, val,
                    &wizard_variables[WIZARD_MOBILE_PIN]) == FALSE) {
            return FALSE;
        } 
        snprintf(tmp, sizeof(tmp), "wizard_wwan_user");
        val = websGetVar(wp, tmp, "0");
        if (valid_name(wp, val,
                    &wizard_variables[WIZARD_MOBILE_NAME]) == FALSE) {
            return FALSE;
        } 
        snprintf(tmp, sizeof(tmp), "wizard_wwan_pw");
        val = websGetVar(wp, tmp, "0");
        if (valid_name(wp, val,
                    &wizard_variables[WIZARD_MOBILE_PASSWD]) == FALSE) {
            return FALSE;
        } 
    }
    if (bw) {
        /* Check Bandwidth Control Enable */
        snprintf(tmp, sizeof(tmp), "wizard_bandwidth_enable");
        val = websGetVar(wp, tmp, "0");
        if (valid_choice(wp, val, &wizard_variables[WIZARD_BANDWIDTH_ENABLE]) == FALSE) {
            return FALSE;
        } 
        
        if (atoi(val)) {
            /* Check WAN Ethernet Bandwidth */
            snprintf(tmp, sizeof(tmp), "wizard_bandwidth_eth_value");
            val = websGetVar(wp, tmp, "0");
            if (valid_name(wp, val,
                        &wizard_variables[WIZARD_BANDWIDTH_ETH]) == FALSE) {
                return FALSE;
            } 
            snprintf(tmp, sizeof(tmp), "wizard_eth_u_rate");
            val = websGetVar(wp, tmp, "0");
            if (valid_number(wp, val,
                        &wizard_variables[WIZARD_BANDWIDTH_ETH_UP]) == FALSE) {
                return FALSE;
            } 
            snprintf(tmp, sizeof(tmp), "wizard_eth_d_rate");
            val = websGetVar(wp, tmp, "0");
            if (valid_number(wp, val,
                        &wizard_variables[WIZARD_BANDWIDTH_ETH_DOWN]) == FALSE) {
                return FALSE;
            } 
            /* Check WAN Mobile Bandwidth */
            snprintf(tmp, sizeof(tmp), "wizard_bandwidth_wwan_value");
            val = websGetVar(wp, tmp, "0");
            if (valid_name(wp, val,
                        &wizard_variables[WIZARD_BANDWIDTH_MOBILE]) == FALSE) {
                return FALSE;
            } 
            snprintf(tmp, sizeof(tmp), "wizard_wwan_u_rate");
            val = websGetVar(wp, tmp, "0");
            if (valid_number(wp, val,
                        &wizard_variables[WIZARD_BANDWIDTH_MOBILE_UP]) == FALSE) {
                return FALSE;
            } 
            snprintf(tmp, sizeof(tmp), "wizard_wwan_d_rate");
            val = websGetVar(wp, tmp, "0");
            if (valid_number(wp, val,
                        &wizard_variables[WIZARD_BANDWIDTH_MOBILE_DOWN]) == FALSE) {
                return FALSE;
            } 
        }
    }

    if (time) {
        /* Pool */
        snprintf(tmp, sizeof(tmp), "wizard_time_pool");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &wizard_variables[WIZARD_TIME_POOL])
                == FALSE) {
            return FALSE;
        }
        /* Daylight Saving */
        snprintf(tmp, sizeof(tmp), "wizard_time_daylight");
        val = websGetVar(wp, tmp, "");

        if (valid_choice(wp, val, &wizard_variables[WIZARD_TIME_DAYLIGHT])
                == FALSE) {
            return FALSE;
        }
        /* Zone */
        snprintf(tmp, sizeof(tmp), "wizard_time_zone");
        val = websGetVar(wp, tmp, "");

        if (valid_name(wp, val, &wizard_variables[WIZARD_TIME_ZONE]) == FALSE) {
            return FALSE;
        }
    }

    if (wifi) {
        /* Check Wifi */
        snprintf(tmp, sizeof(tmp), "wizard_wl_enable");
        val = websGetVar(wp, tmp, "0");
        if (valid_range(wp, val,
                    &wizard_variables[WIZARD_WL_ENABLE]) == FALSE) {
            return FALSE;
        } 
        if (atoi(val) == 1) { /* Wireless Enable */
            snprintf(tmp, sizeof(tmp), "wizard_ssid");
            val = websGetVar(wp, tmp, "0");
            if (valid_name(wp, val,
                        &wizard_variables[WIZARD_WL_SSID]) == FALSE) {
                return FALSE;
            } 

            snprintf(tmp, sizeof(tmp), "wizard_wl_key_enable");
            val = websGetVar(wp, tmp, "0");
            if (valid_choice(wp, val,
                        &wizard_variables[WIZARD_WL_SEC_ENABLE]) == FALSE) {
                return FALSE;
            } 
            if (!strcmp(val, "on")) { /* Security Enable */
                snprintf(tmp, sizeof(tmp), "wizard_wl_sec_value");
                val = websGetVar(wp, tmp, "0");
                if (valid_choice(wp, val,
                            &wizard_variables[WIZARD_WL_SEC_MODE]) == FALSE) {
                    return FALSE;
                } 
                if(atoi(val) > 0) {
                    snprintf(tmp, sizeof(tmp), "wizard_wl_sec_method");
                    val = websGetVar(wp, tmp, "0");
                    if (valid_choice(wp, val,
                                &wizard_variables[WIZARD_WL_SEC_METHOD]) == FALSE) {
                        return FALSE;
                    } 
                }

                snprintf(tmp, sizeof(tmp), "wizard_wl_key");
                val = websGetVar(wp, tmp, "0");
                if (valid_name(wp, val,
                            &wizard_variables[WIZARD_WL_SEC_KEY]) == FALSE) {
                    return FALSE;
                } 
            }
        }
    }
    return TRUE;
}

static int
_save_wan_static(webs_t wp, char *value)
{
    char tmp[TMP_LEN];
    char *ip, *mask, *gateway, *dns1, *dns2;
    int idx = atoi(value), len;

    /* IP */
    snprintf(tmp, sizeof(tmp), "wizard_wan%d_static_ip", idx);
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "wizard_wan%d_static_mask", idx);
    mask = websGetVar(wp, tmp, "");

    /* Gateway */
    snprintf(tmp, sizeof(tmp), "wizard_wan%d_static_gateway", idx);
    gateway = websGetVar(wp, tmp, "");

    /* DNS 1 */
    snprintf(tmp, sizeof(tmp), "wizard_wan%d_static_dns1", idx);
    dns1 = websGetVar(wp, tmp, "");

    /* DNS 2 */
    snprintf(tmp, sizeof(tmp), "wizard_wan%d_static_dns2", idx);
    dns2 = websGetVar(wp, tmp, "");

    len =
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^^", ip, mask, gateway,
                dns1, dns2);

    if (len > TMP_LEN - 1) {
        return 0;
    }
    ezplib_replace_rule("wan_static_rule", idx, tmp);
    return 1;
}

static int
_save_wan_pppoe(webs_t wp, char *value) 
{
    char tmp[TMP_LEN];
    char *username, *passwd;
    char b64_username[TMP_LEN], b64_passwd[TMP_LEN];
    int idx = atoi(value), change = 0;
    unsigned long outlen = TMP_LEN;

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wizard_wan%d_ppp_username", idx);
    username = websGetVar(wp, tmp, "");
    base64_encode((unsigned char *)username, strlen(username),
            (unsigned char *)b64_username, &outlen);
    ezplib_replace_attr("wan_pppoe_rule", idx, "username", b64_username);

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wizard_wan%d_ppp_passwd", idx);
    passwd = websGetVar(wp, tmp, "");
    base64_encode((unsigned char *)passwd, strlen(passwd),
            (unsigned char *)b64_passwd, &outlen);
    ezplib_replace_attr("wan_pppoe_rule", idx, "passwd", b64_passwd);
    return change;
}
static void
_save_time(webs_t wp) 
{
    char tmp[TMP_LEN];
    char *val; 
    /* Pool */
    snprintf(tmp, sizeof(tmp), "wizard_time_pool");
    val = websGetVar(wp, tmp, "");
    ezplib_replace_attr("ntp_rule", 0, "pool", val); 

    /* Daylight Saving */
    snprintf(tmp, sizeof(tmp), "wizard_time_daylight");
    val= websGetVar(wp, tmp, "");
    if (!strcmp(val, "on")) {
        ezplib_replace_attr("ntp_rule", 0, "daylight", "1"); 
    } else {
        ezplib_replace_attr("ntp_rule", 0, "daylight", "0"); 
    }

    /* Zone */
    snprintf(tmp, sizeof(tmp), "wizard_time_zone");
    val = websGetVar(wp, tmp, "");
    ezplib_replace_attr("ntp_rule", 0, "zone", val); 
}

static void
_save_wifi(webs_t wp) 
{
    char *wifi_enable, *ssid, *sec_enable, *secmode, *key, *method;
    char tmp[TMP_LEN];
    /* wifi enable */
    snprintf(tmp, sizeof(tmp), "wizard_wl_enable");
    wifi_enable = websGetVar(wp, tmp, "0");
    ezplib_replace_attr("wl_basic_rule" , 0, "enable", wifi_enable);

    if (atoi(wifi_enable)) {
        /* SSID */
        snprintf(tmp, sizeof(tmp), "wizard_ssid");
        ssid = websGetVar(wp, tmp, "0");
        ezplib_replace_rule("wl0_ssid_rule", 0, ssid);
        ezplib_replace_attr("wl0_basic_rule" , 0, "enable", wifi_enable);

        /* enable wifi security */
        snprintf(tmp, sizeof(tmp), "wizard_wl_key_enable");
        sec_enable = websGetVar(wp, tmp, "0");
        
        if (!strcmp(sec_enable, "on")) {
            ezplib_replace_rule("wl0_sec_rule", 0, "enable");
            /* security type */
            snprintf(tmp, sizeof(tmp), "wizard_wl_sec_value");
            secmode = websGetVar(wp, tmp, "0");
            /* security key */
            snprintf(tmp, sizeof(tmp), "wizard_wl_key");
            key = websGetVar(wp, tmp, "0");
            switch (atoi(secmode)) {
                case 0:
                    /* wep */
                    ezplib_replace_rule("wl0_sec_rule", 0, "wep");
                    ezplib_replace_attr("wl0_sec_wep_rule", 0, "key1", key);
                    break;
                case 1:
                    /* wpa */
                    ezplib_replace_rule("wl0_sec_rule", 0, "psk");
                    ezplib_replace_attr("wl0_sec_wpa_rule", 0, "key", key);
                    snprintf(tmp, sizeof(tmp), "wizard_wl_sec_method");
                    method = websGetVar(wp, tmp, "0");
                    ezplib_replace_attr("wl0_sec_wpa_rule", 0,
                            "crypto", method);
                case 2:
                    /* wpa2 */
                    ezplib_replace_rule("wl0_sec_rule", 0, "psk2");
                    ezplib_replace_attr("wl0_sec_wpa2_rule", 0, "key", key);
                    snprintf(tmp, sizeof(tmp), "wizard_wl_sec_method");
                    method = websGetVar(wp, tmp, "0");
                    ezplib_replace_attr("wl0_sec_wpa2_rule", 0,
                            "crypto", method);
                    break;
            }
        } else {
            ezplib_replace_rule("wl0_sec_rule", 0, "disabled");
        }
    }
}
static int
_save_wan_wwan(webs_t wp, int idx)
{
    char tmp[TMP_LEN], buf[TMP_LEN];

    /* variale from user settting, get from web */
    char *pin, *username, *passwd, *apn_type, *tmp_get;
    /* variable user can not setting, get from nvram rule */
    char mode[TMP_LEN], auth[TMP_LEN], apn[TMP_LEN], dialstr[TMP_LEN];
    char isp[TMP_LEN];

    int len, change = 0, nbrand, nmodel;
    struct ezp_com_map_entry_type *map_ptr;
    struct ezp_com_isp_map_entry_type *map;

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wizard_wwan_apn_type");
    apn_type = websGetVar(wp, tmp, "");

    if (atoi(apn_type) == 0) {
        /* ISP */
        snprintf(tmp, sizeof(tmp), "wizard_wwan_isp");
        tmp_get = websGetVar(wp, tmp, "");
        strcpy(isp, tmp_get);
        /* location USA */
        map = ezp_com_isp_map[12];
        /* Dial String */
        strcpy(dialstr, map[atoi(isp)].dialup_str);
        /* APN */
        strcpy(apn, map[atoi(isp)].apn);

    } else {
        /* APN */
        snprintf(tmp, sizeof(tmp), "wizard_wwan_apn");
        tmp_get = websGetVar(wp, tmp, "");
        strcpy(apn, tmp_get);

        /* Dial String */
        snprintf(tmp, sizeof(tmp), "wizard_wwan_dialstr");
        tmp_get = websGetVar(wp, tmp, "");
        strcpy(dialstr, tmp_get);

        /* ISP */
        strcpy(isp, "0");
    }

    /* PIN */
    snprintf(tmp, sizeof(tmp), "wizard_wwan_pin");
    pin = websGetVar(wp, tmp, "");

    /* Mode */
    ezplib_get_attr_val("wan_wwan_rule", idx, "mode",
            mode, sizeof(mode), EZPLIB_USE_CLI);

    /* Auth */
    ezplib_get_attr_val("wan_wwan_rule", idx, "auth",
            auth, sizeof(auth), EZPLIB_USE_CLI);

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wizard_wwan_username");
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wizard_wwan_passwd");
    passwd = websGetVar(wp, tmp, "");

    /* demand^idletime^redialperiod^mru^mtu */
    ezplib_get_subrule("wan_wwan_rule", 0, 9, 14, buf, sizeof(buf));

    /* Brand */
    /* Auto */
    nbrand = 0;

    /* Model */
    /* Auto */
    nmodel = 0;

    map_ptr = ezp_com_pointer_map[nbrand];

    /* apn_type^location^isp^apn^pin^mode^auth^username^passwd^demand^
       idletime^redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^
       ctrltty^devname^flag^turbolink */
    len =
        snprintf(tmp, TMP_LEN,
                "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%d^%d^%d^%s^%d^%s",
                apn_type, "12", isp, apn, pin, mode, auth, username, passwd,                buf, dialstr, "0", "0", map_ptr[nmodel].DeviceNum,
                map_ptr[nmodel].DataTTY, map_ptr[nmodel].CtrlTTY,
                map_ptr[nmodel].DevName, map_ptr[nmodel].Flag, "0");
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_replace_rule("wan_wwan_rule", idx, tmp);
    change = 1;

    return change;
}
#if 0
static int
_save_bw(webs_t wp, int idx)
{
    char tmp[TMP_LEN];
    char *type, *ul, *dl, *link_type;
    int  len;
    int link_percent, global_percent, user_percent;
    int ulmax, ulmin, dlmax, dlmin, bw_num, auto_dl, auto_ul,
        expert_dl, expert_ul, used_dl, used_ul;

    /* Type */
    snprintf(tmp, sizeof(tmp), "wizard_wan%d_bw_type", idx);
    type = websGetVar(wp, tmp, "");

    if (!strcmp(type, "Custom")) {
        /* Download */
        snprintf(tmp, sizeof(tmp), "wizard_wan%d_bw_dl", idx);
        dl = websGetVar(wp, tmp, "");
        /* Upload */
        snprintf(tmp, sizeof(tmp), "wizard_wan%d_bw_ul", idx);
        ul = websGetVar(wp, tmp, "");
        link_type = "ADSL";
    } else if (!strcmp(type, "A1M64K")) {
        dl = "1024";
        ul = "64";
        link_type = "ADSL";
    } else if (!strcmp(type, "A1.5M640K")) {
        dl = "1536";
        ul = "640";
        link_type = "ADSL";
    } else if (!strcmp(type, "A2M256K")) {
        dl = "2048";
        ul = "256";
        link_type = "ADSL";
    } else if (!strcmp(type, "A2M512K")) {
        dl = "2048";
        ul = "512";
        link_type = "ADSL";
    } else if (!strcmp(type, "A3M640K")) {
        dl = "3072";
        ul = "640";
        link_type = "ADSL";
    } else if (!strcmp(type, "A4M1M")) {
        dl = "4096";
        ul = "1024";
        link_type = "ADSL";
    } else if (!strcmp(type, "A6M256K")) {
        dl = "6144";
        ul = "256";
        link_type = "ADSL";
    } else if (!strcmp(type, "A6M640K")) {
        dl = "6144";
        ul = "640";
        link_type = "ADSL";
    } else if (!strcmp(type, "A8M256K")) {
        dl = "8192";
        ul = "256";
        link_type = "ADSL";
    } else if (!strcmp(type, "A8M640K")) {
        dl = "8192";
        ul = "640";
        link_type = "ADSL";
    } else if (!strcmp(type, "A8M800K")) {
        dl = "8192";
        ul = "800";
        link_type = "ADSL";
    } else if (!strcmp(type, "A10M2M")) {
        dl = "10240";
        ul = "2048";
        link_type = "ADSL";
    } else if (!strcmp(type, "A12M1M")) {
        dl = "12288";
        ul = "1024";
        link_type = "ADSL";
    } else if (!strcmp(type, "V2M2M")) {
        dl = "2048";
        ul = "2048";
        link_type = "VDSL";
    } else if (!strcmp(type, "V4M4M")) {
        dl = "4096";
        ul = "4096";
        link_type = "VDSL";
    } else if (!strcmp(type, "V10M10M")) {
        dl = "10240";
        ul = "10240";
        link_type = "VDSL";
    } else if (!strcmp(type, "V25M25M")) {
        dl = "25600";
        ul = "25600";
        link_type = "VDSL";
    } else if (!strcmp(type, "V50M50M")) {
        dl = "51200";
        ul = "51200";
        link_type = "VDSL";
    } else if (!strcmp(type, "V100M100M")) {
        dl = "102400";
        ul = "102400";
        link_type = "VDSL";
    } else {
        dl = "2048";
        ul = "256";
        link_type = "ADSL";
    }

    used_ul = auto_ul = atoi(ul);
    used_dl = auto_dl = atoi(dl);

    /* reserv 25% bandwidth */
    link_percent = 75;

    /* Cushion percentage */
    if (!strcmp(link_type, "ADSL")) {
        global_percent = 90;
        user_percent = 90;
    } else if (!strcmp(link_type, "Cable")) {
        global_percent = 90;
        user_percent = 90;
    } else if (!strcmp(link_type, "VDSL")) {
        global_percent = 90;
        user_percent = 90;
    } else {
        global_percent = 90;
        user_percent = 90;
    }

    /* Other attributes */
    ezplib_get_attr_val("wan_bw_rule", idx, "expert_ul", tmp,
            TMP_LEN, EZPLIB_USE_CLI);
    expert_ul = atoi(tmp);
    ezplib_get_attr_val("wan_bw_rule", idx, "expert_dl", tmp,
            TMP_LEN, EZPLIB_USE_CLI);
    expert_dl = atoi(tmp);


    /* Percent */
    bw_num = atoi(nvram_safe_get("dbm_max"));
    /* XXX */
    if ( bw_num == 0 )
        bw_num = 1;
    ulmax = used_ul * link_percent / 100;
    ulmax = ulmax * global_percent / 100;
    ulmax = ulmax * user_percent / 100;
    ulmin = ulmax / bw_num;

    dlmax = used_dl * link_percent / 100;
    dlmax = dlmax * global_percent / 100;
    dlmax = dlmax * user_percent / 100;
    dlmin = dlmax / bw_num;

    len =
        snprintf(tmp, TMP_LEN, "%s^%d^%d^%d^%d^%d^%d^%d^%d^%d^%d^%d",
                type, auto_dl, auto_ul, expert_dl,
                expert_ul, link_percent, global_percent, user_percent,
                ulmax, ulmin, dlmax, dlmin);

    if (len >= TMP_LEN) {
        return 1;
    }
    ezplib_replace_rule("wan_bw_rule", idx, tmp);
    return 0;
}
#endif



int
save_wizard(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *cur = nvram_safe_get("wan_num");
    /* variale from user settting, get from web */
    char *wan_type, *pw, *enable;
    /* variable user can not setting, get from nvram rule */
    char tmp1[TMP_LEN], wan_ifname[TMP_LEN];    
    int wifi;

    ezplib_get_attr_val("wizard_rule", 0, "wifi", tmp1, sizeof(tmp1),
            EZPLIB_USE_CLI);
    wifi = atoi(tmp1);


    int num, change = 0;
    int64_t map = 0;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    /* admin setting */
    snprintf(tmp1, sizeof(tmp1), "wizard_admin_pw");
    pw = websGetVar(wp, tmp1, "0");
    if(strcmp(pw, "")) {
        ezplib_replace_attr("http_rule", 0, "passwd", pw);
    }

    /* WAN Ethernet */
    snprintf(tmp1, sizeof(tmp1), "wizard_wan0_enable");
    enable = websGetVar(wp, tmp1, "0");
    ezplib_replace_attr("wan_main_rule", 0, "enable", enable);
    if (atoi(enable)) {
        /* Enable WAN Ethernet */
        snprintf(tmp1, sizeof(tmp1), "wizard_wan_type_value");
        wan_type = websGetVar(wp, tmp1, "0");
        snprintf(wan_ifname, sizeof(wan_ifname), "wan%d_ifname", 0);
        snprintf(tmp1, sizeof(tmp1), "wan0_proto");
        switch (atoi(wan_type)) {
            case DHCP:
                nvram_fset(tmp1,"dhcp");
                snprintf(tmp1, sizeof(tmp1), "wan%d_device", 0);
                nvram_fset(wan_ifname, nvram_safe_get(tmp1));
                break;
            case PPPOE:
                nvram_fset(tmp1,"pppoe");
                snprintf(tmp1, sizeof(tmp1), "ppp%d", 0);
                nvram_fset(wan_ifname, tmp1);
                _save_wan_pppoe(wp, "0");
                break;
            case STATIC:
                nvram_fset(tmp1,"static");
                snprintf(tmp1, sizeof(tmp1), "wan%d_device", 0);
                nvram_fset(wan_ifname, nvram_safe_get(tmp1));
                _save_wan_static(wp, "0");
                break;
        }
    }
    /* WAN Mobile */
    snprintf(tmp1, sizeof(tmp1), "wizard_mobile_enable");
    enable = websGetVar(wp, tmp1, "0");
    /* Enable WAN Mobile */
    ezplib_replace_attr("wan_main_rule", 1, "enable", enable);
    if (atoi(enable)) {
        nvram_fset("wan1_ifname", "ppp1");
        nvram_fset("wan1_proto", "wwan");
        /* Save wwan */
        _save_wan_wwan(wp, 1);
    }
#if 0
    /* Bandwidth */
    snprintf(tmp1, sizeof(tmp1), "wizard_bandwidth_enable");
    enable = websGetVar(wp, tmp1, "0");
    nvram_fset("bw_enable", enable);
    if (atoi(enable)) {
        for (i = 0; i < 2; i++) {
            _save_bw(wp, i);
        }
    }
#endif

    /* Time */
    _save_time(wp);

    if(wifi) {
        /* Wifi */
        _save_wifi(wp);
    }
    ezplib_replace_attr("wizard_rule", 0, "set_wizard", "1");
    config_preaction(&map, v, s, "NUM=0", "");
    config_preaction(&map, v, s, "NUM=1", "");
    change = 1;

    return change;
}

int
ej_wizard_load_isp_data(int eid, webs_t wp, int argc, char_t **argv)
{
    int i, j;
    struct ezp_com_isp_map_entry_type *map_ptr;
    /* ISP Location */
    websWrite(wp, "var local_data = [");
    for (i = 0; strlen(ezp_com_location_map[i].desc); i++) {
        websWrite(wp, "[%d,\'%s\']", i,
                lang_translate(ezp_com_location_map[i].desc));
        if (strlen(ezp_com_location_map[i + 1].desc)) {
            websWrite(wp, ",");
        }
    }
    websWrite(wp, "];\n");
    /* ISP */
    for (j = 0; strlen(ezp_com_location_map[j].desc); j++) {
        map_ptr  = ezp_com_isp_map[j];
        websWrite(wp, "var isp_data_%d = [", j);
        for (i = 0; strlen(map_ptr[i].desc); i++) {
            websWrite(wp, "[%d,\'%s\',\'%s\',\'%s\']", i,
                    lang_translate(map_ptr[i].desc),
                    map_ptr[i].apn, map_ptr[i].dialup_str);
            if (strlen(map_ptr[i + 1].desc)) {
                websWrite(wp, ",");
            }
        }
        websWrite(wp, "];\n");
    }
    return 0;
}

int
ej_wizard_switch_isp(int eid, webs_t wp, int argc, char_t **argv)
{
    int i;
    websWrite(wp, "switch(this.value) {\n");
    for (i = 0; strlen(ezp_com_location_map[i].desc); i++) {
        websWrite(wp,
                "case %d:\n mobile_provider.loadData(isp_data_%d);\n break;\n",
                i, i);

    }
    websWrite(wp, "}\n");
    return 0;
}

int
ej_wizard_load_bandwidth_data(int eid, webs_t wp, int argc, char_t **argv)
{
    websWrite(wp, "var bandwidth_data = [\n");
    websWrite(wp, "['ADSL 1M / 64K bps', 'A1M64K', 64, 1024],\n");
    websWrite(wp, "['ADSL 1.5M / 640K bps', 'A1.5M640K', 640, 1536],\n");
    websWrite(wp, "['ADSL 2M / 256K bps', 'A2M256K', 256, 2048],\n");
    websWrite(wp, "['ADSL 2M / 512K bps', 'A2M512K', 512, 2048],\n");
    websWrite(wp, "['ADSL 3M / 640K bps', 'A3M640K', 640, 3072],\n");
    websWrite(wp, "['ADSL 4M / 1M bps', 'A4M1M', 1024, 4096],\n");
    websWrite(wp, "['ADSL 6M / 256K bps', 'A6M256K', 256, 6144],\n");
    websWrite(wp, "['ADSL 6M / 640K bps', 'A6M640K', 640, 6144],\n");
    websWrite(wp, "['ADSL 8M / 256K bps', 'A8M256K', 256, 8192],\n");
    websWrite(wp, "['ADSL 8M / 640K bps', 'A8M640K', 640, 8192],\n");
    websWrite(wp, "['ADSL 10M / 2M bps', 'A10M2M', 2048, 10240],\n");
    websWrite(wp, "['ADSL 12M / 1M bps', 'A12M1M', 1024, 12288],\n");
    websWrite(wp, "['VDSL 2M / 2M bps', 'V2M2M', 2048, 2048],\n");
    websWrite(wp, "['VDSL 4M / 4M bps', 'V4M4M', 4096, 4096],\n");
    websWrite(wp, "['VDSL 10M / 10M bps', 'V10M10M', 10240, 10240],\n");
    websWrite(wp, "['VDSL 25M / 25M bps', 'V25M25M', 25600, 25600],\n");
    websWrite(wp, "['VDSL 50M / 50M bps', 'V50M50M', 51200, 51200],\n");
    websWrite(wp, "['VDSL 100M / 100M bps', 'V100M100M', 102400, 102400],\n");
    websWrite(wp, "['Customize', 'Custom', , ]\n");
    websWrite(wp, "];\n");
    return 0;
}
void
title_select(char *title, char *page)
{
    char *p, p1[TMP_LEN] = {0};
    /* find target string pointer */
    if ((p = strstr(title, lang_translate(page)))) {
        /* copy string form targrt string to p1 */
        strncpy(p1, title, p - title);
        strcat(p1, "<font color=#FF0000>");
        strcat(p1, lang_translate(page));
        strcat(p1, "</font>");
        strcat(p1, p + strlen(lang_translate(page)));
        strcpy(title, p1);
    }
}

void
make_title(char *title, int len)
{
    char tmp[TMP_LEN];
    /* Admin */
    ezplib_get_attr_val("wizard_rule", 0, "admin", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        strcat(title, lang_translate("Admin"));
        strcat(title, " - ");
    }
    /* Mode */
    ezplib_get_attr_val("wizard_rule", 0, "model", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        strcat(title, lang_translate("Model"));
        strcat(title, " - ");
    }
    /* Ethernet WAN */
    ezplib_get_attr_val("wizard_rule", 0, "wan_trad", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        strcat(title, lang_translate("Ethernet WAN"));
        strcat(title, " - ");
    }
    /* Mobile WAN */
    ezplib_get_attr_val("wizard_rule", 0, "wan_wwan", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        strcat(title, lang_translate("Mobile WAN"));
        strcat(title, " - ");
    }
    /* Bandwidth */
    ezplib_get_attr_val("wizard_rule", 0, "bandwidth", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        strcat(title, lang_translate("Bandwidth"));
        strcat(title, " - ");
    }
    /* Time */
    ezplib_get_attr_val("wizard_rule", 0, "time", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        strcat(title, lang_translate("Time"));
        strcat(title, " - ");
    }
    /* Wifi */
    ezplib_get_attr_val("wizard_rule", 0, "wifi", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        strcat(title, lang_translate("Wireless"));
        strcat(title, " - ");
    }
    /* Summarize */
    strcat(title, lang_translate("Summary"));
}

int
ej_wizard_title(int eid, webs_t wp, int argc, char_t **argv)
{
    char *page;
    char title[TMP_LEN] = {0};

    if (ejArgs(argc, argv, "%s", &page) < 1) {
        websDebugWrite(wp, "Insufficient args\n");
        return -1;
    }
    /* make title */
    make_title(title, sizeof(title));
    /* change the title style of select page */
    title_select(title, page);
    websWrite(wp, "<font size=4px>%s</font>", title);
    return 0;
}

