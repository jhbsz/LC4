#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "bcmnvram.h"
#include "ezp.h"
#include "ezp-lib.h"
#include "ezpcom-lib.h"
#include "shared.h"

#include "fcgi_common.h"
#include "fcgi_stdio.h"
#include "http_common.h"
#include "lang.h"

#define WAN_PROTO_WISP      "wisp"
#define WAN_PROTO_DEFAULT   "dhcp"

enum {
    WAN_ENABLE = 0,
    WAN_PROTO,
    WAN_IPADDR,
    WAN_NETMASK,
    WAN_GATEWAY,
    WAN_DNS,
    WAN_HOSTNAME,
    WAN_DOMAIN,
    WAN_AUTH,
    WAN_USERNAME,
    WAN_PASSWORD,
    WAN_ONDEMAND,
    WAN_IDLETIME,
    WAN_LCP_ECHO_FAILURE,
    WAN_LCP_ECHO_INTERVAL,
    WAN_MTU,
    WAN_UPNP,
    WAN_STP,
    WAN_PPP_MTU,
    WAN_HTC_MTU,
    WAN_IPHONE_MTU,
    WAN_WIMAX_MTU,
    WAN_APN_TYPE,
    WAN_LOCATION,
    WAN_ISP,
    WAN_APN,
    WAN_PIN,
    WAN_MODE,
    WAN_WWAN_USERNAME,
    WAN_WWAN_PASSWORD,
    WAN_WWAN_MTU,
    WAN_WWAN_DIALSTR,
    WAN_WWAN_BRAND,
    WAN_WWAN_MODEL,
    WAN_WWAN_TTYNUM,
    WAN_WWAN_DATATTY,
    WAN_WWAN_CTRLTTY,
    WAN_WWAN_TURBOLINK,
    WAN_BARRY_BARRYPASSWORD,
    WAN_BARRY_APN_TYPE,
    WAN_BARRY_LOCATION,
    WAN_BARRY_ISP,
    WAN_BARRY_USERNAME,
    WAN_BARRY_PASSWORD,
    WAN_BARRY_DIALSTR,
    WAN_BARRY_TURBOLINK,
    WAN_HTC_TURBOLINK,
    WAN_IPHONE_TURBOLINK,
    WAN_WIMAX_TURBOLINK,
    WAN_DIRECTIP_TURBOLINK,
    WAN_BIGPOND_ENABLE,
    WAN_BIGPOND_SERVER,
    WAN_BIGPOND_USERNAME,
    WAN_BIGPOND_PASSWORD,
    WAN_VPN_ENABLE,
    WAN_VPN_TYPE,
    WAN_VPN_USERNAME,
    WAN_VPN_PW,
    WAN_VPN_HOST,
    WAN_VPN_ENCMODE,
    WAN_BECEEM_ISP,
    WAN_BECEEM_USERNAME,
    WAN_BECEEM_PASSWORD,
};


struct wan_proto {
    char *type;
    char *name;
    char *nv_val;
};

enum {
    WAN_CLONE_ENABLE = 0,
    WAN_CLONE_HWADDR,
};

static struct variable wan_variables[] = {
    {longname: "WAN Enable", argv:ARGV("0","1"), nullok: FALSE},
    {longname: "WAN Protocol", argv:ARGV("dhcp", "static", "pppoe", "wwan",
            "directip","htc","iphone", "barry", "wimax", "wisp", "beceem"), nullok: FALSE},
    {longname: "WAN IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "WAN Netmask", argv:ARGV("16", "32"), nullok: FALSE},
    {longname: "WAN Gateway", argv:ARGV(""), nullok: TRUE},
    {longname: "WAN DNS", argv:ARGV(""), nullok: TRUE},
    {longname: "WAN Hostname", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN Domain", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN Auth", argv:ARGV("chap", "pap", "none"), nullok: FALSE},
    {longname: "WAN User Name", argv:ARGV("30"), nullok: FALSE},
    {longname: "WAN Password", argv:ARGV("30"), nullok: FALSE},
    {longname: "WAN On Demand", argv:ARGV("0","1"), nullok: FALSE},
    {longname: "WAN Idle Time", argv:ARGV("60","3600"), nullok: FALSE},
    {longname: "WAN PPP Echo Retry Threshold", argv:ARGV("3","50"), nullok: FALSE},
    {longname: "WAN PPP Echo Interval", argv:ARGV("20","180"), nullok: FALSE},
    {longname: "WAN MTU", argv:ARGV("600","1500"), nullok: FALSE},
    {longname: "WAN PnP", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN STP", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN PPPoE MTU", argv:ARGV("592", "1492"), nullok: FALSE},
    {longname: "WAN Mobile phone MTU", argv:ARGV("512", "1500"), nullok: FALSE},
    {longname: "WAN iPhone MTU", argv:ARGV("512", "1500"), nullok: FALSE},
    {longname: "WAN WiMAX MTU", argv:ARGV("512", "1500"), nullok: FALSE},
    {longname: "WAN APN TYPE", argv:ARGV("0", "1", "2"), nullok: TRUE},
    {longname: "WAN LOCATION", argv:ARGV(""), nullok: TRUE},
    {longname: "WAN ISP", argv:ARGV(""), nullok: TRUE},
    {longname: "WAN APN", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN PIN", argv:ARGV("4", "8"), nullok: TRUE},
    {longname: "WAN Mode", argv:ARGV("auto", "hsdpa", "umts", "edge", "gprs"), nullok: FALSE},
    {longname: "WAN WWAN User Name", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN WWAN Password", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN WWAN MTU", argv:ARGV("68", "1492"), nullok: FALSE},
    {longname: "WAN WWAN Dial String", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN WWAN Device Brand", argv:ARGV(""), nullok: TRUE},
    {longname: "WAN WWAN Device Model", argv:ARGV(""), nullok: TRUE},
    {longname: "WAN WWAN Interface Amount", argv:ARGV("10"), nullok: TRUE},
    {longname: "WAN WWAN Data Interface", argv:ARGV("10"), nullok: TRUE},
    {longname: "WAN WWAN Control Interface", argv:ARGV("10"), nullok: TRUE},
    {longname: "WAN WWAN TURBOLINK", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN BlackBerry Password", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN APN TYPE", argv:ARGV("0", "1", "2"), nullok: TRUE},
    {longname: "WAN LOCATION", argv:ARGV(""), nullok: TRUE},
    {longname: "WAN ISP", argv:ARGV(""), nullok: TRUE},
    {longname: "WAN User Name", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN Password", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN Dial String", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN BlackBerry TURBOLINK", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN HTC TURBOLINK", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN iPhone TURBOLINK", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN WiMAX TURBOLINK", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN DIRECTIP TURBOLINK", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN Bigpond Login", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN Bigpond Server", argv:ARGV(""), nullok: FALSE},
    {longname: "WAN Bigpond User Name", argv:ARGV("30"), nullok: FALSE},
    {longname: "WAN Bigpond Password", argv:ARGV("30"), nullok: FALSE},
    {longname: "WAN VPN Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN VPN Type", argv:ARGV("pptp", "l2tp"), nullok: FALSE},
    {longname: "WAN VPN User Name", argv:ARGV("30"), nullok: FALSE},
    {longname: "WAN VPN Password", argv:ARGV("30"), nullok: FALSE},
    {longname: "WAN VPN Host", argv:ARGV("30"), nullok: FALSE},
    {longname: "WAN VPN MPPE128", argv:ARGV("mppe", ""), nullok: TRUE},
    {longname: "WAN WiMAX ISP", argv:ARGV("g1", "vmax"), nullok: FALSE},
    {longname: "WAN WiMAX User Name", argv:ARGV("30"), nullok: TRUE},
    {longname: "WAN WiMAX Password", argv:ARGV("30"), nullok: TRUE},
};

int _valid_wan_static(INPUT *input, char *value, struct variable *v);
int _valid_wan_dhcp(INPUT *input, char *value, struct variable *v);


#if defined (EZP_SUB_BRAND_SONY) || defined(EZP_SUB_BRAND_GARMIN) ||  defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
int
valid_wisp_swap(INPUT *input, char *value, struct variable *v)
{
    /*
    if(valid_wl_apcli(wp, value, v) == FALSE) {
        return FALSE;
    }
    */
    return TRUE;
}
#endif

int
validate_swap_rule_with_input(char *secmode, char *wepkey, char *wpakey) {
    int keynote;
    int retval = 0;
    int decres = 0;
    unsigned long int declen;
    unsigned char dec_buf[TMP_LEN] = {0};
    declen = sizeof(dec_buf);
    /* secmode not specified */
    if(!secmode || !secmode[0])
        return 0;
    keynote = secmode[0];
    keynote = (keynote << 8) + secmode[1];
    switch(keynote) {
        /* disabled case */
        case ('d'<< 8) + 'i' :
            retval = 1;
            break;
        /* wep case */
        case ('w'<< 8) + 'e' :
            if(wepkey && wepkey[0]) {
                decres = base64_decode((unsigned char*)wepkey, strlen(wepkey), dec_buf, &declen);
                if(decres == 0) {
                    /* decode success, then we check if the decoded data is
                     * not an empty string */
                    if(dec_buf[0])
                        retval = 1;
                }
            }
            break;
        /* wpa/2 or psk/2 case  */
        case ('w'<< 8) + 'p' :
        case ('p'<< 8) + 's' :
            if(wpakey && wpakey[0]) {
                decres = base64_decode((unsigned char*)wpakey, strlen(wpakey), dec_buf, &declen);
                if(decres == 0) {
                    /* decode success, then we check if the decoded data is
                     * not an empty string */
                    if(dec_buf[0]) {
                        retval = 1;
                    }
                }
            }
            break;
    }
    return retval;
}

int
_valid_wan_bigpond(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* Bigpond Enable */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_enable", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_BIGPOND_ENABLE]) == FALSE) {
        return FALSE;
    }
    if (*val == '0') {
        return TRUE;
    }

    /* Bigpond Server */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_server", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_ipaddr(input, val, &wan_variables[WAN_BIGPOND_SERVER]) == FALSE) {
        return FALSE;
    }

    /* Bigpond User Name*/
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_username", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_BIGPOND_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Bigpond Password */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_passwd", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_passwd(input, val, &wan_variables[WAN_BIGPOND_PASSWORD]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_save_wan_wwan(INPUT *input, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], tmp_passwd[TMP_LEN], buf[TMP_LEN];
    char *apn, *pin, *mode, *username, *passwd, *demand, *idletime,
         *redialperiod, *mtu, *dialstr, *brand, *model, *apn_type,
         *location, *isp, *echo_failure, *auth, *turbolink ;
    int idx = atoi(value), len, val, change = 0, nbrand, nmodel;
    struct ezp_com_map_entry_type *map_ptr;
    INPUT *tmp_val;

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn_type", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        apn_type = tmp_val->val;
    }
    else
        apn_type = "";

    /* Location */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_location", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        location = tmp_val->val;
    }
    else
        location = "";

    /* ISP */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_isp", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        isp = tmp_val->val;
    }
    else
        isp = "";

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        apn = tmp_val->val;
    }
    else
        apn = "";

    /* PIN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_pin", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        pin = tmp_val->val;
    }
    else
        pin = "";

    /* Mode */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mode", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        mode = tmp_val->val;
    }
    else
        mode = "";

    /* Auth */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_auth", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        auth = tmp_val->val;
    }
    else
        auth = "";

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_username", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        username = tmp_val->val;
    }
    else
        username = "";

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_passwd", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        passwd = tmp_val->val;
    }
    else
        passwd = "";
    if (!strcmp(passwd, TMP_PASSWD)) {
        ezplib_get_attr_val("wan_wwan_rule", idx, "passwd", tmp_passwd,
                TMP_LEN, EZPLIB_USE_CLI);
        passwd = tmp_passwd;
    }

    /* Demand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_demand", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        demand = tmp_val->val;
    }
    else
        demand = "";
    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_idletime", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        idletime = tmp_val->val;
    }
    else
        idletime = "";

    /* Echo Failure Number */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_failure", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        echo_failure = tmp_val->val;
    }
    else
        echo_failure = "";

    /* Redial Period */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_interval", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        redialperiod = tmp_val->val;
    }
    else
        redialperiod = "";

    /* MRU & MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        mtu = tmp_val->val;
    }
    else
        mtu = "";
    val = atoi(mtu);

    /* Dial String */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_dialstr", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        dialstr = tmp_val->val;
    }
    else
        dialstr = "";

    /* Brand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_brand", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        brand = tmp_val->val;
    }
    else
        brand = "";
    nbrand = atoi(brand);

    /* Model */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_model", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        model = tmp_val->val;
    }
    else
        model = "";
    nmodel = atoi(model);

    map_ptr = ezp_com_pointer_map[nbrand];

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_turbolink", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        turbolink = tmp_val->val;
    }
    else
        turbolink = "";

    len =
      snprintf(tmp, TMP_LEN,
        "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%d^%d^%s^%s^%s^%d^%d^%d^%s^%d^%s",
            apn_type, location, isp, apn, pin, mode, auth, username, passwd,
            demand, idletime, echo_failure, redialperiod, val, val, dialstr,
            brand, model, map_ptr[nmodel].DeviceNum, map_ptr[nmodel].DataTTY,
            map_ptr[nmodel].CtrlTTY, map_ptr[nmodel].DevName,
            map_ptr[nmodel].Flag, turbolink);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_wwan_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_wwan_rule", idx, tmp);
        change = 1;
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
        ezplib_replace_attr("wan_main_rule", 0, "enable", "1");
        ezplib_replace_attr("cnnt_rule", 0, "connection", "0");
        ezplib_replace_attr("cnnt_rule", 1, "connection", "0");
        ezplib_replace_attr("cnnt_rule", 2, "connection", "1");
        ezplib_replace_attr("cnnt_rule", 2, "proto", "wwan");
#endif

    }

    return change;
}

int
_save_wisp_swap(INPUT *input, char *value, struct variable *v, struct service *s, int64_t *map)
{
#if defined (PLATFORM_AXA) || defined(PLATFORM_M2E)
    /* reset wisp trycount when user manual set wisp connection */
    ezplib_replace_attr("wan_status_rule", 0, "wisp_trycount", "0");
#endif

    int i, max, change=0, num, idx;
    char *submit_button, *not_encode_ssid, *bssid, *secmode, *channel, *tmp_wl0_apcli_swap_rule,
         *extcha, *key_index, *key1, *key, *crypto;
    char new_channel_rule_str[TMP_LEN], new_swap_rule_str[TMP_LEN],
         new_sec_wep_rule_str[TMP_LEN], new_sec_wpa_rule_str[TMP_LEN],
         new_sec_wpa2_rule_str[TMP_LEN],
         channel_rule_str[TMP_LEN], swap_rule_str[TMP_LEN],
         sec_wep_rule_str[TMP_LEN], sec_wpa_rule_str[TMP_LEN],
         sec_wpa2_rule_str[TMP_LEN], ssid[TMP_LEN]={0};
    char tmp[TMP_LEN],
         tmp_max[TMP_LEN],
         tmp_ssid[TMP_LEN], tmp_bssid[TMP_LEN],
         cur_bssid[TMP_LEN],
         first_wisp_swap_bssid[TMP_LEN];
    char *cur = nvram_safe_get("wan_num"), *setindex = NULL, *orig_proto;
    int  selected_swap_index = -1, save_button_pressed = 0, total_recorded_item = 0, restart_wan = 0;
    int swap_len = 0, channel_len = 0, wep_len = 0, wpa_len = 0, wpa2_len = 0;
    unsigned long outlen = TMP_LEN;
    INPUT *tmp_val;

    if (!cur || !*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    tmp_wl0_apcli_swap_rule = nvram_get("wl0_apcli_swap_rule");
    ezplib_get_attr_val("wl0_apcli_swap_basic_rule", 0, "max", tmp_max, TMP_LEN, EZPLIB_USE_CLI);
    max = atoi(tmp_max);

    ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", cur_bssid, sizeof(cur_bssid), EZPLIB_USE_CLI);

    /* Save basic visual interface rule */
    snprintf(tmp, sizeof(tmp), "submit_button");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        submit_button = tmp_val->val;
    }
    else
        submit_button = "";

    snprintf(tmp, sizeof(tmp), "wl0_apcli0_ssid");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        not_encode_ssid = tmp_val->val;
    }
    else
        not_encode_ssid = "";

    /* we encode ssid using base64 */
    base64_encode((unsigned char *)not_encode_ssid, strlen(not_encode_ssid),
                                   (unsigned char *)ssid, &outlen);
    snprintf(tmp, sizeof(tmp), "wl0_apcli0_bssid");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        bssid = tmp_val->val;
    }
    else
        bssid = "";

    snprintf(tmp, sizeof(tmp), "wl0_apcli0_secmode");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        secmode = tmp_val->val;
    }
    else
        secmode = "";

    snprintf(tmp, sizeof(tmp), "wl0_channel");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        channel = tmp_val->val;
    }
    else
        channel = "";

    snprintf(tmp, sizeof(tmp), "wl0_extcha");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        extcha = tmp_val->val;
    }
    else
        extcha = "";

    snprintf(tmp, sizeof(tmp), "wl0_apcli0_key_index");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        key_index = tmp_val->val;
    }
    else
        key_index = "";

    snprintf(tmp, sizeof(tmp), "wl0_apcli0_key1");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        key1 = tmp_val->val;
    }
    else
        key1 = "";

    snprintf(tmp, sizeof(tmp), "wl0_apcli0_key");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        key = tmp_val->val;
    }
    else
        key = "";

    snprintf(tmp, sizeof(tmp), "wl0_apcli0_crypto");
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        crypto = tmp_val->val;
    }
    else
        crypto = "";

#ifdef DEBUG
    FCGI_LOG("Received ssid:%s bssid:%s secmode:%s channel:%s extcha:%s crypto:%s key:%s", ssid, bssid, secmode, channel, extcha, crypto, key);
#endif
    if(!strcmp(submit_button, "save")){
        save_button_pressed = 1;
    }
    /* !!! FIXME we may required to use other wan as wisp */
    orig_proto = nvram_get("wan0_proto");
    for (i = 0; i < max; i++){
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "bssid", tmp_bssid, sizeof(tmp_bssid), EZPLIB_USE_CLI);
        if(strcmp(bssid, tmp_bssid)){
            ezplib_get_rule("wl0_apcli_swap_rule", i, tmp, sizeof(tmp));
            swap_len = snprintf(swap_rule_str, TMP_LEN, "%s%s|", swap_rule_str, tmp);
            ezplib_get_rule("wl0_apcli_swap_channel_rule", i, tmp, sizeof(tmp));
            channel_len = snprintf(channel_rule_str, TMP_LEN, "%s%s|", channel_rule_str, tmp);
            ezplib_get_rule("wl0_apcli_swap_sec_wep_rule", i, tmp, sizeof(tmp));
            wep_len = snprintf(sec_wep_rule_str, TMP_LEN, "%s%s|", sec_wep_rule_str, tmp);
            ezplib_get_rule("wl0_apcli_swap_sec_wpa_rule", i, tmp, sizeof(tmp));
            wpa_len = snprintf(sec_wpa_rule_str, TMP_LEN, "%s%s|", sec_wpa_rule_str, tmp);
            ezplib_get_rule("wl0_apcli_swap_sec_wpa2_rule", i, tmp, sizeof(tmp));
            wpa2_len = snprintf(sec_wpa2_rule_str, TMP_LEN, "%s%s|", sec_wpa2_rule_str, tmp);
            total_recorded_item ++;
            if(total_recorded_item == (max - 1))
                break;
        } else {
            selected_swap_index = i;
        }
    }
    swap_rule_str[swap_len - 1] = 0;
    channel_rule_str[channel_len - 1] = 0;
    sec_wep_rule_str[wep_len - 1] = 0;
    sec_wpa_rule_str[wpa_len - 1] = 0;
    sec_wpa2_rule_str[wpa2_len - 1] = 0;

    if(save_button_pressed){
        if(selected_swap_index == -1) {
            snprintf(new_swap_rule_str, TMP_LEN, "%s^%s^%s^%s^%s|%s", "1", ssid, bssid, secmode, "1", swap_rule_str);
            snprintf(new_channel_rule_str, TMP_LEN, "%s^%s|%s", channel, extcha, channel_rule_str);
            snprintf(new_sec_wep_rule_str, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s|%s", key_index, key1,"","","","","", sec_wep_rule_str);
            snprintf(new_sec_wpa_rule_str, TMP_LEN, "%s^%s|%s", key, crypto,sec_wpa_rule_str);
            snprintf(new_sec_wpa2_rule_str, TMP_LEN, "%s^%s|%s", key, crypto,sec_wpa2_rule_str);
        } else {
            if(validate_swap_rule_with_input(secmode, key1, key)) {
                snprintf(new_swap_rule_str, TMP_LEN, "%s^%s^%s^%s^%s|%s", "1", ssid, bssid, secmode, "1", swap_rule_str);
                snprintf(new_channel_rule_str, TMP_LEN, "%s^%s|%s", channel, extcha, channel_rule_str);
                snprintf(new_sec_wep_rule_str, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s|%s", key_index, key1,"","","","","", sec_wep_rule_str);
                snprintf(new_sec_wpa_rule_str, TMP_LEN, "%s^%s|%s", key, crypto,sec_wpa_rule_str);
                snprintf(new_sec_wpa2_rule_str, TMP_LEN, "%s^%s|%s", key, crypto,sec_wpa2_rule_str);
                change = 1;
            } else {
                ezplib_get_rule("wl0_apcli_swap_rule", selected_swap_index, tmp, sizeof(tmp));
                snprintf(new_swap_rule_str, TMP_LEN, "%s|%s", tmp, swap_rule_str);
                ezplib_get_rule("wl0_apcli_swap_channel_rule", selected_swap_index, tmp, sizeof(tmp));
                snprintf(new_channel_rule_str, TMP_LEN, "%s|%s", tmp, channel_rule_str);
                ezplib_get_rule("wl0_apcli_swap_sec_wep_rule", selected_swap_index, tmp, sizeof(tmp));
                snprintf(new_sec_wep_rule_str, TMP_LEN, "%s|%s", tmp, sec_wep_rule_str);
                ezplib_get_rule("wl0_apcli_swap_sec_wpa_rule", selected_swap_index, tmp, sizeof(tmp));
                snprintf(new_sec_wpa_rule_str, TMP_LEN, "%s|%s", tmp, sec_wpa_rule_str);
                ezplib_get_rule("wl0_apcli_swap_sec_wpa2_rule", selected_swap_index, tmp, sizeof(tmp));
                snprintf(new_sec_wpa2_rule_str, TMP_LEN, "%s|%s", tmp, sec_wpa2_rule_str);
            }
        }
        strcpy(swap_rule_str, new_swap_rule_str);
        strcpy(channel_rule_str, new_channel_rule_str);
        strcpy(sec_wep_rule_str, new_sec_wep_rule_str);
        strcpy(sec_wpa_rule_str, new_sec_wpa_rule_str);
        strcpy(sec_wpa2_rule_str, new_sec_wpa2_rule_str);
        setindex = "0";
        if(selected_swap_index != 0) {
            change = 1;
        }
    } else {    // Forget button
        snprintf(swap_rule_str, TMP_LEN, "%s|%s^%s^%s^%s^%s",swap_rule_str, "0","","","disabled","0");
        snprintf(channel_rule_str, TMP_LEN, "%s|%s^%s",channel_rule_str, "6","0");
        snprintf(sec_wep_rule_str, TMP_LEN, "%s|%s^%s^%s^%s^%s^%s^%s",sec_wep_rule_str, "1","","","","","","");
        snprintf(sec_wpa_rule_str, TMP_LEN, "%s|%s^%s",sec_wpa_rule_str, "", "tkip");
        snprintf(sec_wpa2_rule_str, TMP_LEN, "%s|%s^%s",sec_wpa2_rule_str, "", "aes");

        if (strcmp(tmp_wl0_apcli_swap_rule, swap_rule_str)) {
            change = 1;
        }else{
            change = 0;
        }
    }
    if(!strcmp(orig_proto, "wisp")) {
        if(save_button_pressed) {
            if((selected_swap_index != 0) || (change == 1)) {
                restart_wan = 1;
                change = 1;
            } else {
                restart_wan = 0;
            }
        } else {
            ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", tmp_ssid, sizeof(tmp_ssid), EZPLIB_USE_CLI);
            ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", tmp_bssid, sizeof(tmp_bssid), EZPLIB_USE_CLI);
            if(!strcmp(bssid, tmp_bssid) || ((bssid[0] == 0) && !strcmp(ssid, tmp_ssid)) || (selected_swap_index == 0)) {
                restart_wan = 1;
                change = 1;
            } else {
                restart_wan = 0;
            }
        }
    } else {
        /* If we are not using wisp and 
         * pressed save/connect : we set connect and changed, because we will siwtch proto
         * pressed forget : we set changed but not connect
         * */
        if(save_button_pressed) {
            restart_wan = 1;
            change = 1;
        } else {
            restart_wan = 0;
            change = 1;
        }
    }

    if (change) {
        char buf[TMP_LEN];
        /* Here is a trick, if we turn down wan, it will happen that spcli
         * will be turned off. So we first turn off wan, then switch on apcli */
        if(restart_wan) {
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
            idx = atoi(value);
            snprintf(buf, TMP_LEN, "NUM=%d", idx);
            config_preaction(map, v, s, buf, "");
#else
            snprintf(tmp, sizeof(tmp), "/etc/rc.common /etc/init.d/network-wan stop");
            system(tmp);
#endif
            nvram_set("wan0_proto", "wisp");
            nvram_set("wan0_ifname", "apcli0");
#if defined (EZP_SUB_BRAND_GARMIN)
            ezplib_replace_attr("wan_status_rule",0, "priority_num", "0");
#endif
        }
        if(setindex) {
            ezplib_replace_attr("wl0_apcli_swap_basic_rule", 0, "index", setindex);
        }
        nvram_set("wl0_apcli_swap_rule", swap_rule_str);
        nvram_set("wl0_apcli_swap_channel_rule", channel_rule_str);
        nvram_set("wl0_apcli_swap_sec_wep_rule", sec_wep_rule_str);
        nvram_set("wl0_apcli_swap_sec_wpa_rule", sec_wpa_rule_str);
        nvram_set("wl0_apcli_swap_sec_wpa2_rule", sec_wpa2_rule_str);
        ezplib_replace_attr("wl0_apcli_swap_rule", 0, "enable", "1");

        ezplib_get_attr_val("wl0_apcli_swap_rule", 0, "bssid", first_wisp_swap_bssid, sizeof(first_wisp_swap_bssid), EZPLIB_USE_CLI);
        if(restart_wan) {
            nvram_set("wl0_apcli_rule", "0^^^disabled^0");
#if defined (PLATFORM_AXA) || defined(PLATFORM_M2E)
            ezplib_replace_attr("wan_main_rule", 0, "enable", "1");
            ezplib_replace_attr("cnnt_rule", 0, "connection", "0");
            ezplib_replace_attr("cnnt_rule", 1, "connection", "1");
            ezplib_replace_attr("cnnt_rule", 2, "connection", "0");
            ezplib_replace_attr("cnnt_rule", 1, "proto", "wisp");
#elif defined (PLATFORM_AXM)
            ezplib_replace_attr("wan_main_rule", 0, "enable", "1");
            ezplib_replace_attr("cnnt_rule", 0, "connection", "1");
            ezplib_replace_attr("cnnt_rule", 0, "proto", "wisp");
#endif
#if !defined(PLATFORM_AXA) && !defined(PLATFORM_M2E)
            snprintf(tmp, sizeof(tmp), "/sbin/start_delayed_action 5 /etc/rc.common /etc/init.d/network-wan start &");
            system(tmp);
#endif
        }
    }

    return restart_wan;
}


static int
_save_wan_static(INPUT *input, char *value, struct variable *v, struct service *s, int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *ip, *mask, *gateway, *dns1, *dns2, *wins;
    int idx = atoi(value), len, change = 0;
    INPUT *tmp_val;

    /* IP */
    snprintf(tmp, sizeof(tmp), "wan%d_static_ipaddr", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        ip = tmp_val->val;
    }
    else
        ip = "";

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "wan%d_static_mask", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        mask = tmp_val->val;
    }
    else
        mask = "";

    /* Gateway */
    snprintf(tmp, sizeof(tmp), "wan%d_static_gateway", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        gateway = tmp_val->val;
    }
    else
        gateway = "";

    /* DNS 1 */
    snprintf(tmp, sizeof(tmp), "wan%d_static_dns1", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        dns1 = tmp_val->val;
    }
    else
        dns1 = "";

    /* DNS 2 */
    snprintf(tmp, sizeof(tmp), "wan%d_static_dns2", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        dns2 = tmp_val->val;
    }
    else
        dns2 = "";

    /* Wins */
    snprintf(tmp, sizeof(tmp), "wan%d_static_wins", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        wins = tmp_val->val;
    }
    else
        wins = "";

    /* TODO: keep consistent with nvram. */
    len =
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^^%s", ip, mask, gateway,
                dns1, dns2, wins);

    if (len > TMP_LEN - 1) {
        return 0;
    }

    ezplib_get_rule("wan_static_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_static_rule", idx, tmp);
        change = 1;
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
        ezplib_replace_attr("wan_main_rule", 0, "enable", "1");
        ezplib_replace_attr("cnnt_rule", 0, "connection", "1");
        ezplib_replace_attr("cnnt_rule", 1, "connection", "0");
        ezplib_replace_attr("cnnt_rule", 2, "connection", "0");
        ezplib_replace_attr("cnnt_rule", 0, "proto", "static");
#endif
    }
    return change;
}

static int
_save_wan_pppoe(INPUT *input, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], tmp_passwd[TMP_LEN], buf[TMP_LEN];
    char *username, *passwd, *demand, *idletime, *redialperiod, *mtu,
         *echo_failure, *auth;
    char b64_username[TMP_LEN], b64_passwd[TMP_LEN];
    int idx = atoi(value), len, val, change = 0;
    unsigned long outlen = TMP_LEN;
    INPUT *tmp_val;

    /* Auth */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_auth", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        auth = tmp_val->val;
    }
    else
        auth = "";

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_username", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        username = tmp_val->val;
    }
    else
        username = "";

    base64_encode((unsigned char *)username, strlen(username),
                                   (unsigned char *)b64_username, &outlen);

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_passwd", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        passwd = tmp_val->val;
    }
    else
        passwd = "";

    outlen = TMP_LEN;
    if (!strcmp(passwd, TMP_PASSWD)) {
        ezplib_get_attr_val("wan_pppoe_rule", idx, "passwd", tmp_passwd,
                TMP_LEN, EZPLIB_USE_CLI);
        base64_encode((unsigned char *)tmp_passwd, strlen(tmp_passwd),
                (unsigned char *)b64_passwd, &outlen);
    } else {
        base64_encode((unsigned char *)passwd, strlen(passwd),
                (unsigned char *)b64_passwd, &outlen);
    }

    /* Demand */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_demand", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        demand = tmp_val->val;
    }
    else
        demand = "";

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_idletime", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        idletime = tmp_val->val;
    }
    else
        idletime = "";

    /* Echo Failure Number */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_lcp_echo_failure", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        echo_failure = tmp_val->val;
    }
    else
        echo_failure = "";

    /* Redial Period */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_lcp_echo_interval", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        redialperiod = tmp_val->val;
    }
    else
        redialperiod = "";

    /* MRU & MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        mtu = tmp_val->val;
    }
    else
        mtu = "";
    val = atoi(mtu);

    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%d^%d",
            auth, b64_username, b64_passwd, demand, idletime, echo_failure,
            redialperiod, val, val);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_pppoe_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_pppoe_rule", idx, tmp);
        change = 1;
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
        ezplib_replace_attr("wan_main_rule", 0, "enable", "1");
        ezplib_replace_attr("cnnt_rule", 0, "connection", "1");
        ezplib_replace_attr("cnnt_rule", 1, "connection", "0");
        ezplib_replace_attr("cnnt_rule", 2, "connection", "0");
        ezplib_replace_attr("cnnt_rule", 0, "proto", "pppoe");
#endif

    }

    return change;
}


static void
decide_proto_ctype_index(char *new_proto, int *proto_index, int *ctype_index, char **new_ifname) {
    if(new_proto[0] == 'p') {
        /* pppoe */
        if(proto_index)
            *proto_index = 0;
        if(ctype_index)
            *ctype_index = 0;
        if(new_ifname)
            *new_ifname = "ppp0";
    } else if(new_proto[0] == 's') {
        /* static ip*/
        if(proto_index)
            *proto_index = 1;
        if(ctype_index)
            *ctype_index = 0;
        if(new_ifname)
            *new_ifname = "vlan2";
    } else if(new_proto[0] == 'd') {
        /* dhcp */
        if(proto_index)
            *proto_index = 2;
        if(ctype_index)
            *ctype_index = 0;
        if(new_ifname)
            *new_ifname = "vlan2";
    } else if(new_proto[1] == 'i') {
        /* wisp */
        if(proto_index)
            *proto_index = 3;
        if(ctype_index)
            *ctype_index = 1;
        if(new_ifname)
            *new_ifname = "apcli0";
    } else if(new_proto[0] == 'i') {
        /* iphone */
        if(proto_index)
            *proto_index = 4;
        if(ctype_index)
            *ctype_index = 2;
        if(new_ifname)
            *new_ifname = "eth0";
    } else if(new_proto[0] == 'h') {
        /* android/htc */
        if(proto_index)
            *proto_index = 5;
        if(ctype_index)
            *ctype_index = 2;
        if(new_ifname)
            *new_ifname = "usb0";
    } else if(new_proto[1] == 'w') {
        /* wwan */
        if(proto_index)
            *proto_index = 6;
        if(ctype_index)
            *ctype_index = 2;
        if(new_ifname)
            *new_ifname = "ppp0";
    }
}


static int
switch_to_proto(char *proto, INPUT *input, char *value, struct variable *v, struct service *s, int64_t *map) {
    int changed = 0;
    switch((proto[0]<<8) | proto[1]) {
        case ('p'<<8) | 'p':
            /* for pppoe */
            changed = _save_wan_pppoe(input, value, v, s, map);
            break;
        case ('s'<<8) | 't':
            /* for static */
            changed = _save_wan_static(input, value, v, s, map);
            break;
        case ('w'<<8) | 'i':
            /* for wisp */
            // ezplib_replace_attr("wl0_apcli_rule", 0, "enable", "1");
            changed = _save_wisp_swap(input, value, v, s, map);
            break;
        case ('w'<<8) | 'w':
            /* for wwan */
            changed = _save_wan_wwan(input, value, v, s, map);
            break;
    }
    return changed;
}

static void
switch_out_from_proto(char *proto) {
    if(proto[0] == 'w' && proto[1] == 'i') {
        ezplib_replace_attr("wl0_apcli_rule", 0, "enable", "0");
        // ezplib_replace_attr("wl0_apcli_swap_rule", 0, "enable", "0");
        ezplib_replace_attr("wl_basic_rule", 0, "channel", "0");
        /* nvram_set("wl0_apcli_rule", "^^^^"); */
    }
}

static int
push_string_right(char *buf, int buf_len, int offset_len) {
    int i;
    /* move original buf data right rule_len chars, ie
     * "some_string" to "......some_string" */
    for(i = buf_len;i > 0; i--) {
        buf[i + offset_len - 1] = buf[i - 1];
    }
    buf[buf_len + offset_len] = '\0';
    return buf_len + offset_len;
}

static char proto_buf[100];
static inline void
fix_cnnt_rule_parm(char *wan_proto, int idx, char **name, char **enable, char **expand, char **connection, char **proto) {
    int pidx, cidx;
    char *ifname;
    if(!wan_proto) {
        return;
    }

    decide_proto_ctype_index(wan_proto, &pidx, &cidx, &ifname);
    if(!*name || !**name) {
        switch(idx) {
            case 0:
                *name = "Ethernet";
                break;
            case 1:
                *name = "WISP";
                break;
            case 2:
                *name = "USB";
                break;
            default:
                *name = "Ethernet";
                break;
        }
    }
    if(!*enable || !**enable) *enable = "1";
    if(!*expand || !**expand) *expand = "1";
    if(!*connection || !**connection) {
        if(idx == cidx) {
            *connection = "1";
        } else {
            *connection = "0";
        }
    }
    if(!*proto || !**proto) {
        if(idx == cidx) {
            *proto = wan_proto;
        } else {
            ezplib_get_attr_val("cnnt_rule", idx, "proto", proto_buf, sizeof(proto_buf), EZPLIB_USE_CLI);
            *proto = proto_buf;
        }
    }
}

static inline void
get_cnnt_rule_parm(INPUT *input, int idx, char **name, char **enable, char **expand, char **connection, char **proto) {
    char buf[128];
    INPUT *tmp_val;

    snprintf(buf, TMP_LEN, "cnnt_rule%d_name", idx);
    if ((tmp_val = CGI_Find_Parameter(input, buf))) {
        *name = tmp_val->val;
    }
    else
        *name = NULL;

    snprintf(buf, TMP_LEN, "cnnt_rule%d_enable", idx);
    if ((tmp_val = CGI_Find_Parameter(input, buf))) {
        *enable = tmp_val->val;
    }
    else
        *enable = NULL;

    snprintf(buf, TMP_LEN, "cnnt_rule%d_expand", idx);
    if ((tmp_val = CGI_Find_Parameter(input, buf))) {
        *expand = tmp_val->val;
    }
    else
        *expand = NULL;

    snprintf(buf, TMP_LEN, "cnnt_rule%d_connection", idx);
    if ((tmp_val = CGI_Find_Parameter(input, buf))) {
        *connection = tmp_val->val;
    }
    else
        *connection = NULL;

    snprintf(buf, TMP_LEN, "cnnt_rule%d_proto", idx);
    if ((tmp_val = CGI_Find_Parameter(input, buf))) {
        *proto = tmp_val->val;
    }
    else
        *proto = NULL;
}

int
ej_wan_show_wisp_swap_list(int eid, INPUT *input, int argc, char_t ** argv)
{
    char tmp_max[TMP_LEN];
    int max = 0 , i;
    ezplib_get_attr_val("wl0_apcli_swap_basic_rule", 0, "max", tmp_max, TMP_LEN, EZPLIB_USE_CLI);
    max = atoi(tmp_max);

    printf("var SavedWispData = [");

    for(i = 0;i < max; i++) {
        char enable[SHORT_BUF_LEN], ssid[SHORT_BUF_LEN], bssid[SHORT_BUF_LEN],
             secmode[SHORT_BUF_LEN], wifiwan[SHORT_BUF_LEN];

        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "enable", enable, sizeof(enable),EZPLIB_USE_CLI);
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "ssid", ssid, sizeof(ssid),EZPLIB_USE_CLI);
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "bssid", bssid, sizeof(bssid),EZPLIB_USE_CLI);
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "secmode", secmode, sizeof(secmode),EZPLIB_USE_CLI);
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "wifiwan", wifiwan, sizeof(wifiwan),EZPLIB_USE_CLI);
        if (i == 0) {
            printf("{Enable:\"%s\",SSID:\"%s\",BSSID:\"%s\",SecMode:\"%s\",WiFiWan:\"%s\"}", enable, ssid, bssid, secmode, wifiwan);
        } else {
            printf(",{Enable:\"%s\",SSID:\"%s\",BSSID:\"%s\",SecMode:\"%s\",WiFiWan:\"%s\"}", enable, ssid, bssid, secmode, wifiwan);
        }
    }

    printf("];\n");

    return 0;
}

int 
ej_static_show_setting(int eid, INPUT *input, int argc, char_t **argv)
{   
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN], msg[TMP_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    if(num > 1 && appmgr_check("static")) { /* X-116NX and multi-wan production */
        num--;
    }
    
    
    for (i = 0; i < num; i++) { /* ethernet wan */
        snprintf(msg, sizeof(msg), "%s %d", lang_translate("WAN"), i + 1);
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("static_setting.asp", input, buf, msg, "");
    }
    
    return 0;
}

int 
ej_pppoe_show_setting(int eid, INPUT *input, int argc, char_t **argv)
{   
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN], msg[TMP_LEN];
    int num;
    int i;
    
    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    if(num > 1 && appmgr_check("pppoe")) { /* X-116NX and multi-wan production */
        num--;
    }
    
    
    for (i = 0; i < num; i++) { /* ethernet wan */
        snprintf(msg, sizeof(msg), "%s %d", lang_translate("WAN"), i + 1);
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("pppoe_setting.asp", input, buf, msg, "");
    }
    
    return 0;
}

int 
ej_wwan_show_setting(int eid, INPUT *input, int argc, char_t **argv)
{   
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN], msg[TMP_LEN];
    int num, function_wan_flag = 0;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    if(num > 1 && appmgr_check("wwan")) { /* X-116NX and multi-wan production */
        num--;
        function_wan_flag = 1;
    }
    
    
    for (i = 0; i < num; i++) { /* ethernet wan */
        snprintf(msg, sizeof(msg), "%s %d", lang_translate("WAN"), i + 1);
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("wwan_setting.asp", input, buf, msg, "");
    }
    
    return 0;
}

static int
_valid_wan_barry(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val,*apn_type;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* BlackBerry Password */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_barrypasswd", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_passwd(input, val, &wan_variables[WAN_BARRY_BARRYPASSWORD]) == FALSE) {
        return FALSE;
    }

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_apn_type", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        apn_type = tmp_val->val;
    }
    else
        apn_type = "";

    if (valid_choice(input, apn_type, &wan_variables[WAN_BARRY_APN_TYPE]) == FALSE) {
        return FALSE;
    }
    if (*apn_type == '0') {
        /* Location */
        snprintf(tmp, sizeof(tmp), "wan%d_barry_location", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_number(input, val, &wan_variables[WAN_BARRY_LOCATION]) == FALSE) {
            return FALSE;
        }

        /* ISP */
        snprintf(tmp, sizeof(tmp), "wan%d_barry_isp", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_number(input, val, &wan_variables[WAN_BARRY_ISP]) == FALSE) {
            return FALSE;
        }
    }

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_apn", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_APN]) == FALSE) {
        return FALSE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_username", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_BARRY_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_passwd", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_passwd(input, val, &wan_variables[WAN_BARRY_PASSWORD]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_turbolink", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_BARRY_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}


static int
_valid_wan_wwan(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val, *apn_type;
    int idx = atoi(value);
    int mtu, wwan_mtu;
    INPUT *tmp_val;

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn_type", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        apn_type = tmp_val->val;
    }
    else
        apn_type = "";

    if (valid_choice(input, apn_type, &wan_variables[WAN_APN_TYPE]) == FALSE) {
        return FALSE;
    }
    if (*apn_type == '0') {
        /* Location */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_location", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_number(input, val, &wan_variables[WAN_LOCATION]) == FALSE) {
            return FALSE;
        }

        /* ISP */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_isp", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_number(input, val, &wan_variables[WAN_ISP]) == FALSE) {
            return FALSE;
        }
    }

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_APN]) == FALSE) {
        return FALSE;
    }

    /* PIN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_pin", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_length_range(input, val, &wan_variables[WAN_PIN]) == FALSE) {
        return FALSE;
    }

    /* Mode */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mode", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_MODE]) == FALSE) {
        return FALSE;
    }

    /* Brand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_brand", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_number(input, val, &wan_variables[WAN_WWAN_BRAND]) == FALSE) {
        return FALSE;
    }

    /* Model */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_model", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_number(input, val, &wan_variables[WAN_WWAN_MODEL]) == FALSE) {
        return FALSE;
    }

    /* ttynum */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_ttynum", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_WWAN_TTYNUM]) == FALSE) {
        return FALSE;
    }

    /* datatty */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_datatty", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_WWAN_DATATTY]) == FALSE) {
        return FALSE;
    }

    /* ctrltty */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_ctrltty", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_WWAN_CTRLTTY]) == FALSE) {
        return FALSE;
    }

    /* Auth */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_auth", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_AUTH]) == FALSE) {
        return FALSE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_username", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_WWAN_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_passwd", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_passwd(input, val, &wan_variables[WAN_WWAN_PASSWORD]) == FALSE) {
        return FALSE;
    }
    /* On Demand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_demand", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_ONDEMAND]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_turbolink", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_WWAN_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    if (val[0] == '1') {
        /* Idle Time */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_idletime", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_range(input, val, &wan_variables[WAN_IDLETIME]) == FALSE) {
            return FALSE;
        }
    } else {
        /* Echo Failure Number */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_failure", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_range(input, val, &wan_variables[WAN_LCP_ECHO_FAILURE])
                == FALSE) {
            return FALSE;
        }
        /* Redial Period */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_interval", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_range(input, val, &wan_variables[WAN_LCP_ECHO_INTERVAL]) == FALSE) {
            return FALSE;
        }
    }

    /*
     * MRU & MTU: the value is set as the value of MTU in the optional setting
     * minus 8(PPPoE header length) 
     */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_WWAN_MTU]) == FALSE) {
        return FALSE;
    }
    wwan_mtu = atoi(val);

    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_MTU]) == FALSE) {
        return FALSE;
    }
    mtu = atoi(val);

    if (wwan_mtu + 8 > mtu) {
        printf("message=Invalid <b>%s</b> %s: is larger than (MTU + 8)\n",
                wan_variables[WAN_WWAN_MTU].longname);
        return FALSE;
    }

    return TRUE;
}


static int
_valid_wan_pppoe(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    int mtu, ppp_mtu;
    INPUT *tmp_val;

    /* Auth */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_auth", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_AUTH]) == FALSE) {
        return FALSE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_username", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_nv_name(input, val, &wan_variables[WAN_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_passwd", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_nv_passwd(input, val, &wan_variables[WAN_PASSWORD]) == FALSE) {
        return FALSE;
    }

    /* On Demand */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_demand", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_ONDEMAND]) == FALSE) {
        return FALSE;
    }

    if (val[0] == '1') {
        /* Idle Time */
        snprintf(tmp, sizeof(tmp), "wan%d_ppp_idletime", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_range(input, val, &wan_variables[WAN_IDLETIME]) == FALSE) {
            return FALSE;
        }
    } else {
        /* Echo Failure Number */
        snprintf(tmp, sizeof(tmp), "wan%d_ppp_lcp_echo_failure", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_range(input, val, &wan_variables[WAN_LCP_ECHO_FAILURE])
                == FALSE) {
            return FALSE;
        }
        /* Redial Period */
        snprintf(tmp, sizeof(tmp), "wan%d_ppp_lcp_echo_interval", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_range(input, val, &wan_variables[WAN_LCP_ECHO_INTERVAL]) == FALSE) {
            return FALSE;
        }
    }

    /*
     * MRU & MTU: the value is set as the value of MTU in the optional setting
     * minus 8(PPPoE header length) 
     */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_PPP_MTU]) == FALSE) {
        return FALSE;
    }
    ppp_mtu = atoi(val);

    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_MTU]) == FALSE) {
        return FALSE;
    }
    mtu = atoi(val);

    if (ppp_mtu + 8 > mtu) {
        printf("message=Invalid <b>%s</b> %d: is larger than (MTU - 8)\n",
                wan_variables[WAN_PPP_MTU].longname, ppp_mtu);
        return FALSE;
    }

    return TRUE;
}

static int
valid_wan_main(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "wan%d_enable", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_MTU]) == FALSE) {
        return FALSE;
    }

    if (appmgr_check("wwan") && (ezp_prod_subcat < 3)){
        if ((tmp_val = CGI_Find_Parameter(input, "wanupnp_enable"))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_choice(input, val, &wan_variables[WAN_UPNP]) == FALSE) {
            return FALSE;
        }
    }

    return TRUE;
}

static int
_valid_wan_vpnc(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN], tmp2[TMP_LEN], i;
    char *val, *val2, *vpnc_enable;
    int idx = atoi(value);
    INPUT *tmp_val;

    snprintf(tmp, sizeof(tmp), "wan%d_vpnc_enable", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        vpnc_enable = tmp_val->val;
    }
    else
        vpnc_enable = "";

    if (valid_choice(input, vpnc_enable, &wan_variables[WAN_VPN_ENABLE]) == FALSE) {
        return FALSE;
    }

    if ( *vpnc_enable == '1') {
        /* valid vpn type */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_type", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_choice(input, val, &wan_variables[WAN_VPN_TYPE]) == FALSE) {
            return FALSE;
        }

        if (!strcmp(val, "static")) {
            if (_valid_wan_static(input, value, v) == FALSE) {
                return FALSE;
            }
        } else if (!strcmp(val, "dhcp")) {
            if (_valid_wan_dhcp(input, value, v) == FALSE) {
                return FALSE;
            }
        }

        /* valid vpn user name */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_username", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_name(input, val, &wan_variables[WAN_VPN_USERNAME]) == FALSE) {
            return FALSE;
        }
        /* valid vpn password */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_pw", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_passwd(input, val, &wan_variables[WAN_VPN_PW]) == FALSE) {
            return FALSE;
        }
        /* valid vpn server host */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_host", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_name(input, val, &wan_variables[WAN_VPN_HOST]) == FALSE) {
            return FALSE;
        }
        for (i=0;i<idx;i++) {
            snprintf(tmp2, sizeof(tmp2), "wan%d_vpnc_host", i);
            if ((tmp_val = CGI_Find_Parameter(input, tmp2))) {
                val2 = tmp_val->val;
            }
            else
                val2 = "";

            if (!strcmp(val, val2)) {
                printf("%s.<br>",
                        lang_translate("VPN Host cannot be the same between different WAN interface."));

                return FALSE;
            }
        }
        /* valid vpn enc mode */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_encmode", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_choice(input, val, &wan_variables[WAN_VPN_ENCMODE]) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

static int
_valid_wan_wimax(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_wimax_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_WIMAX_MTU]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_wimax_turbolink", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_WIMAX_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
_valid_wan_beceem(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* ISP */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_isp", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_BECEEM_ISP]) == FALSE) {
        return FALSE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_username", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";


    if (valid_nv_name(input, val, &wan_variables[WAN_BECEEM_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_passwd", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_nv_passwd(input, val, &wan_variables[WAN_BECEEM_PASSWORD]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
_valid_wan_iphone(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* Hostname */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_hostname", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_HOSTNAME]) == FALSE) {
        return FALSE;
    }

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_domain", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_DOMAIN]) == FALSE) {
        return FALSE;
    }

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_IPHONE_MTU]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_turbolink", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_IPHONE_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
_valid_wan_htc(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* Hostname */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_hostname", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_HOSTNAME]) == FALSE) {
        return FALSE;
    }

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_domain", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_DOMAIN]) == FALSE) {
        return FALSE;
    }

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_HTC_MTU]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_turbolink", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_HTC_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
_valid_wan_directip(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val, *apn_type;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn_type", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        apn_type = tmp_val->val;
    }
    else
        apn_type = "";

    if (valid_choice(input, apn_type, &wan_variables[WAN_APN_TYPE]) == FALSE) {
        return FALSE;
    }
    if (*apn_type == '0') {
        /* Location */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_location", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_number(input, val, &wan_variables[WAN_LOCATION]) == FALSE) {
            return FALSE;
        }

        /* ISP */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_isp", idx);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            val = tmp_val->val;
        }
        else
            val = "";

        if (valid_number(input, val, &wan_variables[WAN_ISP]) == FALSE) {
            return FALSE;
        }

    }

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_APN]) == FALSE) {
        return FALSE;
    }

    /* PIN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_pin", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_number(input, val, &wan_variables[WAN_PIN]) == FALSE) {
        return FALSE;
    }

    /* Mode */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mode", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_MODE]) == FALSE) {
        return FALSE;
    }
    /* Brand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_brand", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_number(input, val, &wan_variables[WAN_WWAN_BRAND]) == FALSE) {
        return FALSE;
    }

    /* Model */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_model", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_number(input, val, &wan_variables[WAN_WWAN_MODEL]) == FALSE) {
        return FALSE;
    }

    /* ttynum */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_ttynum", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_WWAN_TTYNUM]) == FALSE) {
        return FALSE;
    }

    /* datatty */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_datatty", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_WWAN_DATATTY]) == FALSE) {
        return FALSE;
    }

    /* ctrltty */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_ctrltty", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_WWAN_CTRLTTY]) == FALSE) {
        return FALSE;
    }
    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_range(input, val, &wan_variables[WAN_MTU]) == FALSE) {
        return FALSE;
    }
    if (_valid_wan_bigpond(input, value, v) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_directip_turbolink", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_choice(input, val, &wan_variables[WAN_DIRECTIP_TURBOLINK]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}


int
_valid_wan_dhcp(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* Hostname */
    snprintf(tmp, sizeof(tmp), "wan%d_dhcp_hostname", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_HOSTNAME]) == FALSE) {
        return FALSE;
    }

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_dhcp_domain", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_name(input, val, &wan_variables[WAN_DOMAIN]) == FALSE) {
        return FALSE;
    }

    if (_valid_wan_bigpond(input, value, v) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
_valid_wan_static(INPUT *input, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int idx = atoi(value);
    INPUT *tmp_val;

    /* IP */
    snprintf(tmp, sizeof(tmp), "wan%d_static_ipaddr", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_ipaddr(input, val, &wan_variables[WAN_IPADDR]) == FALSE) {
        return FALSE;
    }

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "wan%d_static_mask", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if ((valid_range(input, val, &wan_variables[WAN_NETMASK]) == FALSE) && (valid_netmask(input, val, &wan_variables[WAN_NETMASK]) == FALSE)){
        return FALSE;
    }

    /* Gateway */
    snprintf(tmp, sizeof(tmp), "wan%d_static_gateway", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_ipaddr(input, val, &wan_variables[WAN_GATEWAY]) == FALSE) {
        return FALSE;
    }

    /* DNS 1 */
    snprintf(tmp, sizeof(tmp), "wan%d_static_dns1", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_ipaddr(input, val, &wan_variables[WAN_DNS]) == FALSE) {
        return FALSE;
    }

    /* DNS 2 */
    snprintf(tmp, sizeof(tmp), "wan%d_static_dns2", idx);
    if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
        val = tmp_val->val;
    }
    else
        val = "";

    if (valid_ipaddr(input, val, &wan_variables[WAN_DNS]) == FALSE) {
        return FALSE;
    }

    /* Wins: Not used now. */

    return TRUE;
}


int
valid_wan(INPUT *input, char *value, struct variable *v)
{
    char *cur = nvram_safe_get("wan_num");
    char tmp[TMP_LEN];
    char *enable, *proto, *access_type;
    int num;
    int i;
    INPUT *tmp_val;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    if ((tmp_val = CGI_Find_Parameter(input, "access_type"))) {
        access_type = tmp_val->val;
    }
    else
        access_type = "";

    if (!strcmp(access_type,"Apps")) num=1;

    for (i = 0; i < num; i++) {
        snprintf(tmp, sizeof(tmp), "wan%d_enable", i);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            enable = tmp_val->val;
        }
        else
            enable = "";

        if (valid_choice(input, enable, &wan_variables[WAN_ENABLE]) == FALSE) {
           return FALSE;
        }

        if (*enable == '0') {
            continue;
        }

        snprintf(tmp, sizeof(tmp), "wan%d_proto", i);
        if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
            proto = tmp_val->val;
        }
        else
            proto = "";

        if (valid_choice(input, proto, &wan_variables[WAN_PROTO]) == FALSE) {
           return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "%d", i);
        assert(atoi(tmp) == i);
        if (strcmp(proto, "static") == 0) {
            if (_valid_wan_static(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "dhcp") == 0) {
            if (_valid_wan_dhcp(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "pppoe") == 0) {
            if (_valid_wan_pppoe(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "wwan") == 0) {
            if (_valid_wan_wwan(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "barry") == 0) {
            if (_valid_wan_barry(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "directip") == 0) {
            if (_valid_wan_directip(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "htc") == 0) {
            if (_valid_wan_htc(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "iphone") == 0) {
            if (_valid_wan_iphone(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "beceem") == 0) {
            if (_valid_wan_beceem(input, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "wimax") == 0) {
            if (_valid_wan_wimax(input, tmp, v) == FALSE) {
                return FALSE;
            }
/*        } else if (strcmp(proto, "wisp") == 0) {
            if (_valid_wan_wisp(input, tmp, v) == FALSE) {
                return FALSE;
            }*/
        } else {
            assert(0);
        }
        if (appmgr_check("vpnc")){
            if (_valid_wan_vpnc(input, tmp, v) == FALSE) {
                return FALSE;
            }
        }

        if (valid_wan_main(input, tmp, v) == FALSE) {
            return FALSE;
        }
#ifdef EZP_PROD_BRAND_ZYUS
        /* valid mac clone */
        if ( i == 0 ) {
            /* Enable */
            snprintf(tmp, sizeof(tmp), "wan%d_hwaddr_clone_enable", i);
            if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
                enable = tmp_val->val;
            }
            else
                enable = "";

            if (valid_choice(input, enable, &clone_variables[WAN_CLONE_ENABLE])
                    == FALSE) {
                return FALSE;
            }

            if (*enable == '1'){
                /* Hardware Address */
                snprintf(tmp, sizeof(tmp), "wan%d_hwaddr_clone_hwaddr", i);
                if ((tmp_val = CGI_Find_Parameter(input, tmp))) {
                    enable = tmp_val->val;
                }
                else
                    enable = "";

                if (valid_hwaddr(input, enable, &clone_variables[WAN_CLONE_HWADDR])
                        == FALSE) {
                    return FALSE;
                }
            }
        }
#endif
    }

    /* STP and Weight is not allowed to configure by users. */

    return TRUE;
}

int 
valid_cnnt(INPUT *input, char *value, struct variable *v)
{    
    return TRUE;
}   

int 
save_cnnt(INPUT *input, char *value, struct variable *v, struct service *s)
{
    int i, num, change = 0, connect_num = 0, change_t;
    char buf[TMP_LEN], tmp[TMP_LEN], cnnt_rule_seq[10];
    int64_t map = 0;
    char *cnnt_num = NULL, *name = NULL, *expand = NULL, *enable = NULL, *connection = NULL, *proto = NULL, *new_proto = NULL, *p_lookup = NULL, *new_ifname = NULL;
    char *orig_proto = nvram_safe_get("wan0_proto");
    int proto_index = -1, ctype_index = -1, cnnt_index = -1, rule_set_len = 0, rule_len = 0, first_rule_end_pos = 0;
    char *o_ifname = NULL;
    int o_proto_index = -1, o_ctype_index = -1;
    INPUT *tmp_val;
    /*
     * Protocol policy : 
     * 1. If we only have wan0_proto, no cnnt rule, then use cnnt_rule
     * 2. If we only cnnt_rule, no wan0_proto, use cnnt_rule
     * 3. If we have wan0_proto and cnnt_rule but different, use wan0_proto to
     * fix cnnt rule 
     * */
    if ((tmp_val = CGI_Find_Parameter(input, "wan0_proto"))) {
        new_proto = tmp_val->val;
    }
    else
        new_proto = "";

    if ((tmp_val = CGI_Find_Parameter(input, "cnnt_rule_num"))) {
        cnnt_num = tmp_val->val;
    }
    if(!cnnt_num || !*cnnt_num) {
        cnnt_num = nvram_safe_get("cnnt_rule_num");
    }
    num = atoi(cnnt_num);
    /* First, we have USB/WISP/Ethernet, so we create a string with the first
     * chars of each cnnt_rule name, looks like "EWU" */
    for(i = 0; i < num; i++) {
        ezplib_get_attr_val("cnnt_rule", i, "name", tmp, sizeof(tmp), EZPLIB_USE_CLI);
        cnnt_rule_seq[i] = tmp[0];
    }
    cnnt_rule_seq[i] = 0;
    /* Get original ifname */
    if(orig_proto) {
        decide_proto_ctype_index(orig_proto, &o_proto_index, &o_ctype_index, &o_ifname);
    }
    if(new_proto && *new_proto) {
        /* pppoe : proto_index = 0, ctype_index = 0
         * static IP : proto_index = 1, ctype_index = 0
         * dhcp : proto_index = 2, ctype_index = 0
         * wisp : proto_index = 3, ctype_index = 1
         * iphone : proto_index = 4, ctype_index = 2
         * htc : proto_index = 5, ctype_index = 2
         * wwan : proto_index = 6, ctype_index = 2
         * */
        decide_proto_ctype_index(new_proto, &proto_index, &ctype_index, &new_ifname);
    }
    buf[0] = 0;
    rule_set_len = 0;
    for(i = 0; i < num; i++) {
        get_cnnt_rule_parm(input, i, &name, &enable, &expand, &connection, &proto);
        /* For wisp, it doesn't offer any wan0_proto, we have to use
         * "wisp_swap_rule" to check */
        if((!new_proto || !*new_proto) && (!name || !*name) && (!proto || !*proto)) {
            if ((tmp_val = CGI_Find_Parameter(input, "wisp_swap_rule"))) {
//                new_proto = tmp_val->val;
                new_proto = WAN_PROTO_WISP;
            }
            else {
                new_proto = WAN_PROTO_DEFAULT;
            }
            decide_proto_ctype_index(new_proto, &proto_index, &ctype_index, &new_ifname);
        }
        /* Fix parameters in case some old app send only proto instead of cnnt
         * parameters. */
        fix_cnnt_rule_parm(new_proto, i, &name, &enable, &expand, &connection, &proto);
        if((p_lookup = strchr(cnnt_rule_seq, name[0]))) {
            /* cnnt_index should be 0,1,2 */
            cnnt_index = p_lookup - cnnt_rule_seq;
            if(ctype_index == -1) {
                /* Decide proto from cnnt_rule and avoid strange "more than 1"
                 * cnnt_rule with connection is 1 */
                if(new_proto && *new_proto) {
                    rule_len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%d^%s", expand, name, enable, 0, proto);
                } else {
                    if(connection[0] == '1') {
                        new_proto = proto;
                        connect_num ++;
                    }
                    rule_len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s", expand, name, enable, connection, proto);
                }
            } else {
                if(cnnt_index == ctype_index) {
                    rule_len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%d^%s", expand, name, enable, 1, new_proto);
                    connect_num ++;
                } else {
                    rule_len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%d^%s", expand, name, enable, 0, proto);
                }
            }
            switch(cnnt_index) {
                case 0:
                    /* move original buf data right rule_len + 1 chars, ie
                     * "some_string" to "......some_string" */
                    push_string_right(buf, rule_set_len, rule_len + 1);
                    /* set new rule to first part of new rule */
                    strncpy(buf, tmp, rule_len);
                    first_rule_end_pos = rule_len + 1;
                    rule_set_len += first_rule_end_pos;
                    /* Now we have a "new_rule|buf" rule set */
                    buf[rule_len] = '|';
                    buf[rule_set_len] = 0;
                    break;
                case 1:
                    /* move original buf data right rule_len chars, ie
                     * "some_string" to "......some_string" */
                    push_string_right(buf + first_rule_end_pos, rule_set_len - first_rule_end_pos, rule_len);
                    /* set new rule to first part of new rule */
                    strncpy(buf + first_rule_end_pos, tmp, rule_len);
                    rule_set_len += rule_len;
                    /* Now we have a "new_rule|buf" rule set */
                    buf[rule_set_len] = 0;
                    break;
                case 2:
                    buf[rule_set_len] = '|';
                    rule_set_len ++;
                    buf[rule_set_len] = '\0';
                    strncat(buf, tmp, sizeof(buf));
                    rule_set_len += rule_len;
                    break;
                default:
                    /* WOW, why this happen ? */
                    break;
            }
        } else {
            /* !!! Wrong, way can't we find the connection type. */
        }
    }
    if(!new_proto || !*new_proto) {
        /* Strange here, why we don't have a proto ? */
        connect_num = 0;
        new_proto = "dhcp";
    }
    if(ctype_index == -1) {
        decide_proto_ctype_index(new_proto, &proto_index, &ctype_index, &new_ifname);
    }
    /* If we found a valid wan proto is selected or cnnt rule is connected */
    if((new_proto && *new_proto) || connect_num) {
        if((strcmp(new_proto, orig_proto) || strcmp(buf, nvram_safe_get("cnnt_rule")))) {
            if(change==0)
                config_preaction(&map, v, s, "NUM=0", "");
            switch_out_from_proto(orig_proto);
            change = 1;
            nvram_set("cnnt_rule", buf);
            nvram_set("wan0_proto", new_proto);
            nvram_set("wan0_ifname", new_ifname);
            if(ctype_index == 0)
                nvram_set("wan0_device", "vlan2");
        }
    }
    else {
    /* No ? Then we just reconnect the wan */
        if(change==0)
            config_preaction(&map, v, s, "NUM=0", "");
        change = 1;
    }
    change_t = switch_to_proto(new_proto, input, value, v, s, &map);
    change = (change == 1 ? 1 : change_t);
    if(change) {
        if(connect_num==0) {
        /* disable wan */
            ezplib_replace_attr("wan_main_rule", 0, "enable", "0");
        }
        else {
            ezplib_replace_attr("wan_main_rule", 0, "enable", "1");
        }
        config_postaction(map, s, "NUM=0", "");
    }

    return change;
}


