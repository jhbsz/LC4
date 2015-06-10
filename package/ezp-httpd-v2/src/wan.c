#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <cyutils.h>

#include "common.h"
#include "lang.h"
#include "ezp.h"
#include "ezp-lib.h"
#include "ezpcom-lib.h"
#include "nvram_ezpacket.h"
#include "shared.h"
#ifdef DEBUG
#include <sys/time.h>
#endif

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
#ifdef EZP_SUB_BRAND_APOLLO
    WAN_WISP_IP_TYPE,
#endif
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

#ifdef EZP_PROD_BRAND_ZYUS
static struct variable clone_variables[] = {
    {longname: "WAN Clone Enable", argv:ARGV("0", "1")},
    {longname: "WAN Hardware Address", argv:ARGV("")},
};
#endif

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
#ifdef EZP_SUB_BRAND_APOLLO
    {longname: "WAN WISP IP Type", argv:ARGV("dhcp","static"), nullok: FALSE},
#endif
};

static int
_valid_wan_bigpond(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* Bigpond Enable */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_enable", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_BIGPOND_ENABLE]) == FALSE) {
        return FALSE;
    }
    if (*val == '0') {
        return TRUE;
    }

    /* Bigpond Server */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_server", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_ipaddr(wp, val, &wan_variables[WAN_BIGPOND_SERVER]) == FALSE) {
        return FALSE;
    }

    /* Bigpond User Name*/
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_username", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_BIGPOND_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Bigpond Password */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_passwd", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_passwd(wp, val, &wan_variables[WAN_BIGPOND_PASSWORD]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_static(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* IP */
    snprintf(tmp, sizeof(tmp), "wan%d_static_ipaddr", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_ipaddr(wp, val, &wan_variables[WAN_IPADDR]) == FALSE) {
        return FALSE;
    }

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "wan%d_static_mask", idx);
    val = websGetVar(wp, tmp, "");

    if ((valid_range(wp, val, &wan_variables[WAN_NETMASK]) == FALSE) && (valid_netmask(wp, val, &wan_variables[WAN_NETMASK]) == FALSE)){
        return FALSE;
    }

    /* Gateway */
    snprintf(tmp, sizeof(tmp), "wan%d_static_gateway", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_ipaddr(wp, val, &wan_variables[WAN_GATEWAY]) == FALSE) {
        return FALSE;
    }

    /* DNS 1 */
    snprintf(tmp, sizeof(tmp), "wan%d_static_dns1", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_ipaddr(wp, val, &wan_variables[WAN_DNS]) == FALSE) {
        return FALSE;
    }

    /* DNS 2 */
    snprintf(tmp, sizeof(tmp), "wan%d_static_dns2", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_ipaddr(wp, val, &wan_variables[WAN_DNS]) == FALSE) {
        return FALSE;
    }

    /* Wins: Not used now. */

    return TRUE;
}


static int
_valid_wan_dhcp(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* Hostname */
    snprintf(tmp, sizeof(tmp), "wan%d_dhcp_hostname", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &wan_variables[WAN_HOSTNAME]) == FALSE) {
        return FALSE;
    }

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_dhcp_domain", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &wan_variables[WAN_DOMAIN]) == FALSE) {
        return FALSE;
    }

    if (_valid_wan_bigpond(wp, value, v) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_vpnc(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN], tmp2[TMP_LEN], i; 
    char *val, *val2, *vpnc_enable;
    int idx = atoi(value);

    snprintf(tmp, sizeof(tmp), "wan%d_vpnc_enable", idx);
    vpnc_enable = websGetVar(wp, tmp, "");

    if (valid_choice(wp, vpnc_enable, &wan_variables[WAN_VPN_ENABLE]) == FALSE) {
        return FALSE;
    }

    if ( *vpnc_enable == '1') {
        /* valid vpn type */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_type", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &wan_variables[WAN_VPN_TYPE]) == FALSE) {
            return FALSE;
        }

        if (!strcmp(val, "static")) {
            if (_valid_wan_static(wp, value, v) == FALSE) {
                return FALSE;
            }
        } else if (!strcmp(val, "dhcp")) {
            if (_valid_wan_dhcp(wp, value, v) == FALSE) {
                return FALSE;
            }
        }

        /* valid vpn user name */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_username", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &wan_variables[WAN_VPN_USERNAME]) == FALSE) {
            return FALSE;
        }
        /* valid vpn password */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_pw", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_passwd(wp, val, &wan_variables[WAN_VPN_PW]) == FALSE) {
            return FALSE;
        }
        /* valid vpn server host */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_host", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &wan_variables[WAN_VPN_HOST]) == FALSE) {
            return FALSE;
        }
        for (i=0;i<idx;i++) {
            snprintf(tmp2, sizeof(tmp2), "wan%d_vpnc_host", i);
            val2 = websGetVar(wp, tmp2, "");
            if (!strcmp(val, val2)) {
                websDebugWrite(wp, "%s.<br>",
                        lang_translate("VPN Host cannot be the same between different WAN interface."));

                return FALSE;
            }
        }
        /* valid vpn enc mode */
        snprintf(tmp, sizeof(tmp), "wan%d_vpnc_encmode", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &wan_variables[WAN_VPN_ENCMODE]) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

static int
_valid_wan_htc(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* Hostname */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_hostname", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &wan_variables[WAN_HOSTNAME]) == FALSE) {
        return FALSE;
    }

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_domain", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &wan_variables[WAN_DOMAIN]) == FALSE) {
        return FALSE;
    }

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_HTC_MTU]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_turbolink", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_HTC_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_iphone(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* Hostname */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_hostname", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &wan_variables[WAN_HOSTNAME]) == FALSE) {
        return FALSE;
    }

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_domain", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &wan_variables[WAN_DOMAIN]) == FALSE) {
        return FALSE;
    }

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_IPHONE_MTU]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_turbolink", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_IPHONE_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_beceem(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* ISP */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_isp", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_BECEEM_ISP]) == FALSE) {
        return FALSE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_username", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_nv_name(wp, val, &wan_variables[WAN_BECEEM_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_passwd", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_nv_passwd(wp, val, &wan_variables[WAN_BECEEM_PASSWORD]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_wimax(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_wimax_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_WIMAX_MTU]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_wimax_turbolink", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_WIMAX_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_wisp(webs_t wp, char *value, struct variable *v)
{
#ifdef EZP_SUB_BRAND_APOLLO
    char tmp[TMP_LEN];    
    char *ip_type;
    int idx = atoi(value);
#endif
    if(valid_wl_apcli(wp, value, v) == FALSE) {
        return FALSE;
    }
#ifdef EZP_SUB_BRAND_APOLLO
    /* IP Type */
    snprintf(tmp, sizeof(tmp), "wan%d_wisp_ip_type", idx);
    ip_type = websGetVar(wp, tmp, "");
    if (valid_choice(wp, ip_type, &wan_variables[WAN_WISP_IP_TYPE]) == FALSE) {
        return FALSE;
    }
    if (!strcmp(ip_type,"dhcp")){
        if(_valid_wan_dhcp(wp, value, v) == FALSE) {
            return FALSE;
        }
    } else {
        if(_valid_wan_static(wp, value, v) == FALSE) {
            return FALSE;
        }
    }
#endif

    return TRUE;
}

static int
_valid_wan_pppoe(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);
    int mtu, ppp_mtu;

    /* Auth */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_auth", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_AUTH]) == FALSE) {
        return FALSE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_username", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_nv_name(wp, val, &wan_variables[WAN_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_passwd", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_nv_passwd(wp, val, &wan_variables[WAN_PASSWORD]) == FALSE) {
        return FALSE;
    }

    /* On Demand */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_demand", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_ONDEMAND]) == FALSE) {
        return FALSE;
    }

    if (val[0] == '1') {
        /* Idle Time */
        snprintf(tmp, sizeof(tmp), "wan%d_ppp_idletime", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_IDLETIME]) == FALSE) {
            return FALSE;
        }
    } else {
        /* Echo Failure Number */
        snprintf(tmp, sizeof(tmp), "wan%d_ppp_lcp_echo_failure", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_LCP_ECHO_FAILURE])
                == FALSE) {
            return FALSE;
        }
        /* Redial Period */
        snprintf(tmp, sizeof(tmp), "wan%d_ppp_lcp_echo_interval", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_LCP_ECHO_INTERVAL]) == FALSE) {
            return FALSE;
        }
    }

    /*
     * MRU & MTU: the value is set as the value of MTU in the optional setting
     * minus 8(PPPoE header length) 
     */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_PPP_MTU]) == FALSE) {
        return FALSE;
    }
    ppp_mtu = atoi(val);

    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_MTU]) == FALSE) {
        return FALSE;
    }
    mtu = atoi(val);

    if (ppp_mtu + 8 > mtu) {
        websDebugWrite(wp, "Invalid <b>%s</b> %d: is larger than (MTU - 8)",
                wan_variables[WAN_PPP_MTU].longname, ppp_mtu);
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_wwan(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val, *apn_type;
    int idx = atoi(value);
    int mtu, wwan_mtu;

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn_type", idx);
    apn_type = websGetVar(wp, tmp, "");
    if (valid_choice(wp, apn_type, &wan_variables[WAN_APN_TYPE]) == FALSE) {
        return FALSE;
    }
    if (*apn_type == '0') {
        /* Location */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_location", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &wan_variables[WAN_LOCATION]) == FALSE) {
            return FALSE;
        }

        /* ISP */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_isp", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &wan_variables[WAN_ISP]) == FALSE) {
            return FALSE;
        }
    }

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_APN]) == FALSE) {
        return FALSE;
    }

    /* PIN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_pin", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_length_range(wp, val, &wan_variables[WAN_PIN]) == FALSE) {
        return FALSE;
    }

    /* Mode */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mode", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_choice(wp, val, &wan_variables[WAN_MODE]) == FALSE) {
        return FALSE;
    }

    /* Brand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_brand", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_number(wp, val, &wan_variables[WAN_WWAN_BRAND]) == FALSE) {
        return FALSE;
    }

    /* Model */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_model", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_number(wp, val, &wan_variables[WAN_WWAN_MODEL]) == FALSE) {
        return FALSE;
    }

    /* ttynum */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_ttynum", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_WWAN_TTYNUM]) == FALSE) {
        return FALSE;
    }

    /* datatty */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_datatty", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_WWAN_DATATTY]) == FALSE) {
        return FALSE;
    }

    /* ctrltty */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_ctrltty", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_WWAN_CTRLTTY]) == FALSE) {
        return FALSE;
    }

    /* Auth */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_auth", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_AUTH]) == FALSE) {
        return FALSE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_username", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_WWAN_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_passwd", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_passwd(wp, val, &wan_variables[WAN_WWAN_PASSWORD]) == FALSE) {
        return FALSE;
    }

    /* On Demand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_demand", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_ONDEMAND]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_turbolink", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_WWAN_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    if (val[0] == '1') {
        /* Idle Time */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_idletime", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_IDLETIME]) == FALSE) {
            return FALSE;
        }
    } else {
        /* Echo Failure Number */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_failure", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_LCP_ECHO_FAILURE])
                == FALSE) {
            return FALSE;
        }
        /* Redial Period */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_interval", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_LCP_ECHO_INTERVAL]) == FALSE) {
            return FALSE;
        }
    }

    /*
     * MRU & MTU: the value is set as the value of MTU in the optional setting
     * minus 8(PPPoE header length) 
     */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_WWAN_MTU]) == FALSE) {
        return FALSE;
    }
    wwan_mtu = atoi(val);

    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_MTU]) == FALSE) {
        return FALSE;
    }
    mtu = atoi(val);

    if (wwan_mtu + 8 > mtu) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: is larger than (MTU + 8)",
                wan_variables[WAN_WWAN_MTU].longname);
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_barry(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val,*apn_type;
    int idx = atoi(value);

    /* BlackBerry Password */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_barrypasswd", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_passwd(wp, val, &wan_variables[WAN_BARRY_BARRYPASSWORD]) == FALSE) {
        return FALSE;
    }

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_apn_type", idx);
    apn_type = websGetVar(wp, tmp, "");
    if (valid_choice(wp, apn_type, &wan_variables[WAN_BARRY_APN_TYPE]) == FALSE) {
        return FALSE;
    }
    if (*apn_type == '0') {
        /* Location */
        snprintf(tmp, sizeof(tmp), "wan%d_barry_location", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &wan_variables[WAN_BARRY_LOCATION]) == FALSE) {
            return FALSE;
        }

        /* ISP */
        snprintf(tmp, sizeof(tmp), "wan%d_barry_isp", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &wan_variables[WAN_BARRY_ISP]) == FALSE) {
            return FALSE;
        }
    }

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_apn", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_APN]) == FALSE) {
        return FALSE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_username", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_BARRY_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_passwd", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_passwd(wp, val, &wan_variables[WAN_BARRY_PASSWORD]) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_turbolink", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_BARRY_TURBOLINK]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wan_directip(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val, *apn_type;
    int idx = atoi(value);
#if 0
    int mtu, wwan_mtu;
#endif

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn_type", idx);
    apn_type = websGetVar(wp, tmp, "");
    if (valid_choice(wp, apn_type, &wan_variables[WAN_APN_TYPE]) == FALSE) {
        return FALSE;
    }
    if (*apn_type == '0') {
        /* Location */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_location", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &wan_variables[WAN_LOCATION]) == FALSE) {
            return FALSE;
        }

        /* ISP */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_isp", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &wan_variables[WAN_ISP]) == FALSE) {
            return FALSE;
        }

    }

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_APN]) == FALSE) {
        return FALSE;
    }

    /* PIN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_pin", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_number(wp, val, &wan_variables[WAN_PIN]) == FALSE) {
        return FALSE;
    }

    /* Mode */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mode", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_choice(wp, val, &wan_variables[WAN_MODE]) == FALSE) {
        return FALSE;
    }

#if 0 /* Direct IP doesn't need the following items. */
    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_username", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &wan_variables[WAN_WWAN_USERNAME]) == FALSE) {
        return FALSE;
    }

    /* Password */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_passwd", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_passwd(wp, val, &wan_variables[WAN_WWAN_PASSWORD]) == FALSE) {
        return FALSE;
    }

    /* On Demand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_demand", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_ONDEMAND]) == FALSE) {
        return FALSE;
    }

    if (val[0] == '1') {
        /* Idle Time */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_idletime", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_IDLETIME]) == FALSE) {
            return FALSE;
        }
    } else {
        /* Echo Failure Number */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_failure", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_LCP_ECHO_FAILURE])
                == FALSE) {
            return FALSE;
        }
        /* Redial Period */
        snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_interval", idx);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &wan_variables[WAN_LCP_ECHO_INTERVAL])
                == FALSE) {
            return FALSE;
        }
    }
    /* Dial String */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_dialstr", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_WWAN_DIALSTR]) == FALSE) {
        return FALSE;
    }
#endif

    /* Brand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_brand", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_number(wp, val, &wan_variables[WAN_WWAN_BRAND]) == FALSE) {
        return FALSE;
    }

    /* Model */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_model", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_number(wp, val, &wan_variables[WAN_WWAN_MODEL]) == FALSE) {
        return FALSE;
    }

    /* ttynum */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_ttynum", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_WWAN_TTYNUM]) == FALSE) {
        return FALSE;
    }

    /* datatty */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_datatty", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_WWAN_DATATTY]) == FALSE) {
        return FALSE;
    }

    /* ctrltty */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_ctrltty", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &wan_variables[WAN_WWAN_CTRLTTY]) == FALSE) {
        return FALSE;
    }

    /*
     * MRU & MTU: the value is set as the value of MTU in the optional setting
     * minus 8(PPPoE header length) 
     */
#if 0
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_WWAN_MTU]) == FALSE) {
        return FALSE;
    }
    wwan_mtu = atoi(val);
#endif

    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &wan_variables[WAN_MTU]) == FALSE) {
        return FALSE;
    }
#if 0
    mtu = atoi(val);

    if (wwan_mtu + 8 > mtu) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: is larger than (MTU + 8)",
                wan_variables[WAN_WWAN_MTU].longname);
        return FALSE;
    }
#endif

    if (_valid_wan_bigpond(wp, value, v) == FALSE) {
        return FALSE;
    }

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_directip_turbolink", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_DIRECTIP_TURBOLINK]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

static int
_save_wan_bigpond(webs_t wp, char *value, struct variable *v,
        struct service *s, int64_t *map)
{
    char tmp[TMP_LEN], tmp_passwd[TMP_LEN], buf[TMP_LEN];
    int idx = atoi(value), len, change = 0;
    char *enable, *server, *username, *passwd;

    /* Bigpond Enable */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_enable", idx);
    enable = websGetVar(wp, tmp, "");

    /* Bigpond Server */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_server", idx);
    server = websGetVar(wp, tmp, "");

    /* Bigpond User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_username", idx);
    username = websGetVar(wp, tmp, "");

    /* Bigpond Password */
    snprintf(tmp, sizeof(tmp), "wan%d_bigpond_passwd", idx);
    passwd = websGetVar(wp, tmp, "");
    if (!strcmp(passwd, TMP_PASSWD)) {
        ezplib_get_attr_val("wan_bigpond_rule", 0, "passwd", tmp_passwd, 
                sizeof(tmp_passwd), EZPLIB_USE_CLI); 
        passwd = tmp_passwd;
    }

    len =
        snprintf(tmp, sizeof(tmp), "%s^%s^%s^%s", enable, server, username,
                passwd);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_bigpond_rule", idx, buf, sizeof(buf));
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_bigpond_rule", idx, tmp);
        change = 1;
    }

    return change;
}

static int
_save_wan_static(webs_t wp, char *value, struct variable *v, struct service *s, int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *ip, *mask, *gateway, *dns1, *dns2, *wins;
    int idx = atoi(value), len, change = 0;

    /* IP */
    snprintf(tmp, sizeof(tmp), "wan%d_static_ipaddr", idx);
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "wan%d_static_mask", idx);
    mask = websGetVar(wp, tmp, "");

    /* Gateway */
    snprintf(tmp, sizeof(tmp), "wan%d_static_gateway", idx);
    gateway = websGetVar(wp, tmp, "");

    /* DNS 1 */
    snprintf(tmp, sizeof(tmp), "wan%d_static_dns1", idx);
    dns1 = websGetVar(wp, tmp, "");

    /* DNS 2 */
    snprintf(tmp, sizeof(tmp), "wan%d_static_dns2", idx);
    dns2 = websGetVar(wp, tmp, "");

    /* Wins */
    snprintf(tmp, sizeof(tmp), "wan%d_static_wins", idx);
    wins = websGetVar(wp, tmp, "");

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
_save_wan_dhcp(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *hostname, *domain, *lease;
    int idx = atoi(value), len, change = 0;

    /* Host Name */
    snprintf(tmp, sizeof(tmp), "wan%d_dhcp_hostname", idx);
    hostname = websGetVar(wp, tmp, "");

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_dhcp_domain", idx);
    domain = websGetVar(wp, tmp, "");

    /* Lease */
    snprintf(tmp, sizeof(tmp), "wan%d_dhcp_lease", idx);
    lease = websGetVar(wp, tmp, "");

    len = snprintf(tmp, TMP_LEN, "%s^%s^%s", hostname, domain, lease);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_dhcp_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_dhcp_rule", idx, tmp);
        change = 1;
    }

#ifndef EZP_PROD_BRAND_ZYUS
    if (_save_wan_bigpond(wp, value, v, s, map)) {
        change = 1;
    } 
#endif
    return change;
}

static int
_save_wan_vpnc(webs_t wp, char *value, struct variable *v,
        struct service *s, int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *enable, *vpn_type, *host, *username, *pw, *encmode;
    int idx = atoi(value), change = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "wan%d_vpnc_enable", idx);
    enable = websGetVar(wp, tmp, "");
    
    /* vpn type */
    snprintf(tmp, sizeof(tmp), "wan%d_vpnc_type", idx);
    vpn_type = websGetVar(wp, tmp, "");

    /* vpn user name */
    snprintf(tmp, sizeof(tmp), "wan%d_vpnc_username", idx);
    username = websGetVar(wp, tmp, "");

    /* vpn password */
    snprintf(tmp, sizeof(tmp), "wan%d_vpnc_pw", idx);
    pw = websGetVar(wp, tmp, "");

    /* vpn server */
    snprintf(tmp, sizeof(tmp), "wan%d_vpnc_host", idx);
    host = websGetVar(wp, tmp, "");

    /* valid vpn enc mode */
    snprintf(tmp, sizeof(tmp), "wan%d_vpnc_encmode", idx);
    encmode = websGetVar(wp, tmp, "");
    if (*enable == '1') {
        if (ezplib_check_attr_changed("wan_pptp_l2tp_rule", idx, "enable", enable) ||
            ezplib_check_attr_changed("wan_pptp_l2tp_rule", idx, "vpn_type", vpn_type) ||
            ezplib_check_attr_changed("wan_pptp_l2tp_rule", idx, "vpn_type", vpn_type) ||
            ezplib_check_attr_changed("wan_pptp_l2tp_rule", idx, "host", host) ||
            ezplib_check_attr_changed("wan_pptp_l2tp_rule", idx, "username", username) ||
            ezplib_check_attr_changed("wan_pptp_l2tp_rule", idx, "passwd", pw) ||
            ezplib_check_attr_changed("wan_pptp_l2tp_rule", idx, "encmode", encmode)) {
            snprintf(buf, TMP_LEN, "NUM=%d", idx);
            config_preaction(map, v, s, buf, "");
            ezplib_replace_attr("wan_pptp_l2tp_rule", idx, "enable", enable);
            ezplib_replace_attr("wan_pptp_l2tp_rule", idx, "vpn_type", vpn_type);
            ezplib_replace_attr("wan_pptp_l2tp_rule", idx, "host", host);
            ezplib_replace_attr("wan_pptp_l2tp_rule", idx, "username", username);
            ezplib_replace_attr("wan_pptp_l2tp_rule", idx, "passwd", pw);
            ezplib_replace_attr("wan_pptp_l2tp_rule", idx, "encmode", encmode);
            change = 1;
        }
    } else {
        if (ezplib_check_attr_changed("wan_pptp_l2tp_rule", idx, "enable", enable)) {
            snprintf(buf, TMP_LEN, "NUM=%d", idx);
            config_preaction(map, v, s, buf, "");
            ezplib_replace_attr("wan_pptp_l2tp_rule", idx, "enable", enable);
            change = 1;
        }
    }
    return change;
}

static int
_save_wan_htc(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *hostname, *domain, *lease, *mtu, *turbolink;
    int idx = atoi(value), len, change = 0;

    /* Host Name */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_hostname", idx);
    hostname = websGetVar(wp, tmp, "");

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_domain", idx);
    domain = websGetVar(wp, tmp, "");

    /* Lease */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_lease", idx);
    lease = websGetVar(wp, tmp, "");

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_mtu", idx);
    mtu = websGetVar(wp, tmp, "");

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_htc_turbolink", idx);
    turbolink = websGetVar(wp, tmp, "");

    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s", hostname, domain, lease, mtu, turbolink);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_htc_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_htc_rule", idx, tmp);
        change = 1;
    }

    return change;
}

static int
_save_wan_iphone(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *hostname, *domain, *lease, *mtu, *turbolink;
    int idx = atoi(value), len, change = 0;

    /* Host Name */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_hostname", idx);
    hostname = websGetVar(wp, tmp, "");

    /* Domain */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_domain", idx);
    domain = websGetVar(wp, tmp, "");

    /* Lease */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_lease", idx);
    lease = websGetVar(wp, tmp, "");

        /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_mtu", idx);
    mtu = websGetVar(wp, tmp, "");

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_iphone_turbolink", idx);
    turbolink = websGetVar(wp, tmp, "");

    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s", hostname, domain, lease, mtu, turbolink);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_iphone_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_iphone_rule", idx, tmp);
        change = 1;
    }

    return change;
}

static int
_save_wan_beceem(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], tmp_passwd[TMP_LEN], buf[TMP_LEN];
    char *isp, *username, *passwd;
    char b64_username[TMP_LEN], b64_passwd[TMP_LEN];
    int idx = atoi(value), len, change = 0;
    unsigned long outlen = TMP_LEN;

    /* ISP */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_isp", idx);
    isp = websGetVar(wp, tmp, "");

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_username", idx);
    username = websGetVar(wp, tmp, "");

    base64_encode((unsigned char *)username, strlen(username),
                                   (unsigned char *)b64_username, &outlen);

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wan%d_beceem_passwd", idx);
    passwd = websGetVar(wp, tmp, "");
    outlen = TMP_LEN;
    if (!strcmp(passwd, TMP_PASSWD)) {
        ezplib_get_attr_val("wan_beceem_rule", idx, "passwd", tmp_passwd,
                TMP_LEN, EZPLIB_USE_CLI);
        base64_encode((unsigned char *)tmp_passwd, strlen(tmp_passwd),
                (unsigned char *)b64_passwd, &outlen);
    } else {
        base64_encode((unsigned char *)passwd, strlen(passwd),
                (unsigned char *)b64_passwd, &outlen);
    }

    len = snprintf(tmp, TMP_LEN, "%s^%s^%s", isp,username, passwd);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_beceem_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_beceem_rule", idx, tmp);
        change = 1;
    }

    return change;
}

static int
_save_wan_wimax(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *mtu, *turbolink;
    int idx = atoi(value), len, change = 0;

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_wimax_mtu", idx);
    mtu = websGetVar(wp, tmp, "");

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_wimax_turbolink", idx);
    turbolink = websGetVar(wp, tmp, "");

    len = snprintf(tmp, TMP_LEN, "%s^%s", mtu, turbolink);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_wimax_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_wimax_rule", idx, tmp);
        change = 1;
    }

    return change;
}

static int
_save_wan_pppoe(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], tmp_passwd[TMP_LEN], buf[TMP_LEN];
    char *username, *passwd, *demand, *idletime, *redialperiod, *mtu,
         *echo_failure, *auth;
    char b64_username[TMP_LEN], b64_passwd[TMP_LEN];
    int idx = atoi(value), len, val, change = 0;
    unsigned long outlen = TMP_LEN;

    /* Auth */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_auth", idx);
    auth = websGetVar(wp, tmp, "");

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_username", idx);
    username = websGetVar(wp, tmp, "");

    base64_encode((unsigned char *)username, strlen(username),
                                   (unsigned char *)b64_username, &outlen);

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_passwd", idx);
    passwd = websGetVar(wp, tmp, "");
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
    demand = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_idletime", idx);
    idletime = websGetVar(wp, tmp, "");

    /* Echo Failure Number */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_lcp_echo_failure", idx);
    echo_failure = websGetVar(wp, tmp, "");

    /* Redial Period */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_lcp_echo_interval", idx);
    redialperiod = websGetVar(wp, tmp, "");

    /* MRU & MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_ppp_mtu", idx);
    mtu = websGetVar(wp, tmp, "");
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

static int
_save_wan_wwan(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], tmp_passwd[TMP_LEN], buf[TMP_LEN];
    char *apn, *pin, *mode, *username, *passwd, *demand, *idletime, 
         *redialperiod, *mtu, *dialstr, *brand, *model, *apn_type,
         *location, *isp, *echo_failure, *auth, *turbolink ;
    int idx = atoi(value), len, val, change = 0, nbrand, nmodel;
    struct ezp_com_map_entry_type *map_ptr;

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn_type", idx);
    apn_type = websGetVar(wp, tmp, "");

    /* Location */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_location", idx);
    location = websGetVar(wp, tmp, "");

    /* ISP */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_isp", idx);
    isp = websGetVar(wp, tmp, "");

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn", idx);
    apn = websGetVar(wp, tmp, "");

    /* PIN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_pin", idx);
    pin = websGetVar(wp, tmp, "");

    /* Mode */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mode", idx);
    mode = websGetVar(wp, tmp, "");

    /* Auth */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_auth", idx);
    auth = websGetVar(wp, tmp, "");

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_username", idx);
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_passwd", idx);
    passwd = websGetVar(wp, tmp, "");
    if (!strcmp(passwd, TMP_PASSWD)) {
        ezplib_get_attr_val("wan_wwan_rule", idx, "passwd", tmp_passwd,
                TMP_LEN, EZPLIB_USE_CLI);
        passwd = tmp_passwd;
    }

    /* Demand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_demand", idx);
    demand = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_idletime", idx);
    idletime = websGetVar(wp, tmp, "");

    /* Echo Failure Number */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_failure", idx);
    echo_failure = websGetVar(wp, tmp, "");

    /* Redial Period */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_lcp_echo_interval", idx);
    redialperiod = websGetVar(wp, tmp, "");

    /* MRU & MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mtu", idx);
    mtu = websGetVar(wp, tmp, "");
    val = atoi(mtu);

    /* Dial String */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_dialstr", idx);
    dialstr = websGetVar(wp, tmp, "");

    /* Brand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_brand", idx);
    brand = websGetVar(wp, tmp, "");
    nbrand = atoi(brand);

    /* Model */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_model", idx);
    model = websGetVar(wp, tmp, "");
    nmodel = atoi(model);

    map_ptr = ezp_com_pointer_map[nbrand]; 

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_turbolink", idx);
    turbolink = websGetVar(wp, tmp, "");

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

static int
_save_wan_barry(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], tmp_barrypasswd[TMP_LEN], tmp_passwd[TMP_LEN], buf[TMP_LEN];
    char *barrypasswd, *apn_type, *location, *isp, *apn, *username, *passwd, *dialstr, *turbolink ;
    int idx = atoi(value), len, change = 0;

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_barrypasswd", idx);
    barrypasswd = websGetVar(wp, tmp, "");
    if (!strcmp(barrypasswd, TMP_PASSWD)) {
        ezplib_get_attr_val("wan_barry_rule", idx, "barrypasswd", tmp_barrypasswd, TMP_LEN, EZPLIB_USE_CLI);
        barrypasswd = tmp_barrypasswd;
    }

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_apn_type", idx);
    apn_type = websGetVar(wp, tmp, "");

    /* Location */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_location", idx);
    location = websGetVar(wp, tmp, "");

    /* ISP */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_isp", idx);
    isp = websGetVar(wp, tmp, "");

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_apn", idx);
    apn = websGetVar(wp, tmp, "");

    /* User Name */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_username", idx);
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_passwd", idx);
    passwd = websGetVar(wp, tmp, "");
    if (!strcmp(passwd, TMP_PASSWD)) {
        ezplib_get_attr_val("wan_barry_rule", idx, "passwd", tmp_passwd,
                TMP_LEN, EZPLIB_USE_CLI);
        passwd = tmp_passwd;
    }

    /* Dial String */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_dialstr", idx);
    dialstr = websGetVar(wp, tmp, "");

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_barry_turbolink", idx);
    turbolink = websGetVar(wp, tmp, "");

    len = 
      snprintf(tmp, TMP_LEN, 
        "%s^%s^%s^%s^%s^%s^%s^%s^%s",
            barrypasswd, apn_type, location, isp, apn, username, passwd, dialstr, turbolink);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_barry_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_barry_rule", idx, tmp);
        change = 1;
    }

    return change;
}

static int
_save_wan_directip(webs_t wp, char *value, struct variable *v,
        struct service *s, int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char mtu[TMP_LEN], username[TMP_LEN], passwd[TMP_LEN], demand[TMP_LEN],
         idletime[TMP_LEN], redialperiod[TMP_LEN], dialstr[TMP_LEN],
         echo_failure[TMP_LEN], auth[TMP_LEN];
    int val;
    char *apn, *pin, *mode, *brand, *model, *apn_type, *location, *isp, *turbolink;
    int idx = atoi(value), len, change = 0, nbrand, nmodel;
    struct ezp_com_map_entry_type *map_ptr;

    /* APN Type */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn_type", idx);
    apn_type = websGetVar(wp, tmp, "");

    /* Location */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_location", idx);
    location = websGetVar(wp, tmp, "");

    /* ISP */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_isp", idx);
    isp = websGetVar(wp, tmp, "");

    /* APN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_apn", idx);
    apn = websGetVar(wp, tmp, "");

    /* PIN */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_pin", idx);
    pin = websGetVar(wp, tmp, "");

    /* Mode */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_mode", idx);
    mode = websGetVar(wp, tmp, "");

    /* Auth */
    ezplib_get_attr_val("wan_wwan_rule", idx, "auth", auth,
            TMP_LEN, EZPLIB_USE_CLI);

    /* User Name */
    ezplib_get_attr_val("wan_wwan_rule", idx, "username", username,
            TMP_LEN, EZPLIB_USE_CLI);

    /* Passwd */
    ezplib_get_attr_val("wan_wwan_rule", idx, "username", username,
            TMP_LEN, EZPLIB_USE_CLI);

    ezplib_get_attr_val("wan_wwan_rule", idx, "passwd", passwd,
            TMP_LEN, EZPLIB_USE_CLI);

    /* Demand */
    ezplib_get_attr_val("wan_wwan_rule", idx, "demand", demand,
            TMP_LEN, EZPLIB_USE_CLI);

    /* Idle Time */
    ezplib_get_attr_val("wan_wwan_rule", idx, "idletime", idletime,
            TMP_LEN, EZPLIB_USE_CLI);

    /* Echo Failure Number */
    ezplib_get_attr_val("wan_wwan_rule", idx, "echo_failure", echo_failure,
            TMP_LEN, EZPLIB_USE_CLI);

    /* Redial Period */
    ezplib_get_attr_val("wan_wwan_rule", idx, "redialperiod", redialperiod,
            TMP_LEN, EZPLIB_USE_CLI);

    /* MRU & MTU */
    ezplib_get_attr_val("wan_wwan_rule", idx, "mtu", mtu,
            TMP_LEN, EZPLIB_USE_CLI);
    val = atoi(mtu);

    /* Dial String */
    ezplib_get_attr_val("wan_wwan_rule", idx, "dialstr", dialstr,
            TMP_LEN, EZPLIB_USE_CLI);

    /* Brand */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_brand", idx);
    brand = websGetVar(wp, tmp, "");
    nbrand = atoi(brand);

    /* Model */
    snprintf(tmp, sizeof(tmp), "wan%d_wwan_model", idx);
    model = websGetVar(wp, tmp, "");
    nmodel = atoi(model);

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "wan%d_directip_turbolink", idx);
    turbolink = websGetVar(wp, tmp, "");

    map_ptr = ezp_com_pointer_map[nbrand]; 

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
    }

    if (_save_wan_bigpond(wp, value, v, s, map)) {
        change = 1;
    } 

    return change;
}


static int
_save_wan_main(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], tmp_mtu[TMP_LEN], buf[TMP_LEN], wanupnp[TMP_LEN],
         wanupnp_tmp[TMP_LEN], maxcount[TMP_LEN];
    char *enable, *mtu, *stp, *wan_upnp;
    int idx = atoi(value), len, change = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "wan%d_enable", idx);
    enable = websGetVar(wp, tmp, "");
    
    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    mtu = websGetVar(wp, tmp, "");
    if (!*mtu) {
        ezplib_get_attr_val("wan_main_rule", idx, "mtu", tmp_mtu, TMP_LEN,
                EZPLIB_USE_WEB);
        mtu = tmp_mtu;
    }

    /* STP */
    snprintf(tmp, sizeof(tmp), "wan%d_stp", idx);
    stp = websGetVar(wp, tmp, "");

    /* WAN upnp */
    if (appmgr_check("wwan") && (ezp_prod_subcat < 3)){
        wan_upnp = websGetVar(wp, "wanupnp_enable", "");
        strcpy(wanupnp, wan_upnp);
        ezplib_get_rule("wanupnp_rule", 0, wanupnp_tmp, TMP_LEN);
        if (strcmp(wanupnp, wanupnp_tmp)) {
            ezplib_replace_rule("wanupnp_rule", 0, wanupnp);
            change = 1;
        }
    }
    /* WAN try max count */
    ezplib_get_attr_val("wan_main_rule", idx, "maxcount", maxcount, TMP_LEN,
                EZPLIB_USE_WEB);

    len = snprintf(tmp, TMP_LEN, "WAN%d^%s^%s^%s^%s", 
            idx+1, enable, mtu, stp,  maxcount);
    if (len >= TMP_LEN) {
        return 0;
    }

    ezplib_get_rule("wan_main_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_preaction(map, v, s, buf, "");
        ezplib_replace_rule("wan_main_rule", idx, tmp);
        change = 1;
    }

    return change;
}

static int
valid_wan_main(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int idx = atoi(value);

    /* Enable */
    snprintf(tmp, sizeof(tmp), "wan%d_enable", idx);
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wan_variables[WAN_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* MTU */
    snprintf(tmp, sizeof(tmp), "wan%d_mtu", idx);
    val = websGetVar(wp, tmp, "");

    if (valid_range(wp, val, &wan_variables[WAN_MTU]) == FALSE) {
        return FALSE;
    }

    if (appmgr_check("wwan") && (ezp_prod_subcat < 3)){
        val = websGetVar(wp, "wanupnp_enable", "");
        if (valid_choice(wp, val, &wan_variables[WAN_UPNP]) == FALSE) {
            return FALSE;
        }
    }

    return TRUE;
}

int
valid_wan_priority(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int
valid_cnnt(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int
valid_wan(webs_t wp, char *value, struct variable *v)
{
    char *cur = nvram_safe_get("wan_num");
    char tmp[TMP_LEN];
    char *enable, *proto, *access_type;
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    access_type = websGetVar(wp, "access_type", "");
    if (!strcmp(access_type,"Apps")) num=1; 

    for (i = 0; i < num; i++) {
        snprintf(tmp, sizeof(tmp), "wan%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        if (valid_choice(wp, enable, &wan_variables[WAN_ENABLE]) == FALSE) {
           return FALSE;
        }

        if (*enable == '0') {
            continue;
        }

        snprintf(tmp, sizeof(tmp), "wan%d_proto", i);
        proto = websGetVar(wp, tmp, "");

        if (valid_choice(wp, proto, &wan_variables[WAN_PROTO]) == FALSE) {
           return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "%d", i);
        assert(atoi(tmp) == i);
        if (strcmp(proto, "static") == 0) {
            if (_valid_wan_static(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "dhcp") == 0) {
            if (_valid_wan_dhcp(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "pppoe") == 0) {
            if (_valid_wan_pppoe(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "wwan") == 0) {
            if (_valid_wan_wwan(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "barry") == 0) {
            if (_valid_wan_barry(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "directip") == 0) {
            if (_valid_wan_directip(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "htc") == 0) {
            if (_valid_wan_htc(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "iphone") == 0) {
            if (_valid_wan_iphone(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "beceem") == 0) {
            if (_valid_wan_beceem(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "wimax") == 0) {
            if (_valid_wan_wimax(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else if (strcmp(proto, "wisp") == 0) {
            if (_valid_wan_wisp(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        } else {
            assert(0);
        }
        if (appmgr_check("vpnc")){
            if (_valid_wan_vpnc(wp, tmp, v) == FALSE) {
                return FALSE;
            }
        }

        if (valid_wan_main(wp, tmp, v) == FALSE) {
            return FALSE;
        }
#ifdef EZP_PROD_BRAND_ZYUS
        /* valid mac clone */
        if ( i == 0 ) {
            /* Enable */
            snprintf(tmp, sizeof(tmp), "wan%d_hwaddr_clone_enable", i);
            enable = websGetVar(wp, tmp, "");

            if (valid_choice(wp, enable, &clone_variables[WAN_CLONE_ENABLE])
                    == FALSE) {
                return FALSE;
            }

            if (*enable == '1'){
                /* Hardware Address */
                snprintf(tmp, sizeof(tmp), "wan%d_hwaddr_clone_hwaddr", i);
                enable = websGetVar(wp, tmp, "");
                if (valid_hwaddr(wp, enable, &clone_variables[WAN_CLONE_HWADDR])
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

#if defined (EZP_SUB_BRAND_SONY) || defined(EZP_SUB_BRAND_GARMIN) ||  defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
int
valid_wisp_swap(webs_t wp, char *value, struct variable *v)
{
    /*
    if(valid_wl_apcli(wp, value, v) == FALSE) {
        return FALSE;
    }
    */
    return TRUE;
}
#ifdef DEBUG
void
log_time(const char *str1, int num) {
    struct timeval tv;
    long int time_milli = 0;
    gettimeofday(&tv, NULL);
    time_milli = tv.tv_sec * 1000 + (tv.tv_usec / 1000);
    printf("%s(%d) : time:%ld\n", str1, num, time_milli);
}
#else
#define log_time(___A___, ___B___) {}
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
_save_wisp_swap(webs_t wp, char *value, struct variable *v, struct service *s, int64_t *map)
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
         sec_wpa2_rule_str[TMP_LEN], ssid[TMP_LEN];
    char tmp[TMP_LEN],
         tmp_max[TMP_LEN],
         tmp_ssid[TMP_LEN], tmp_bssid[TMP_LEN], 
         cur_bssid[TMP_LEN],
         first_wisp_swap_bssid[TMP_LEN];
    char *cur = nvram_safe_get("wan_num"), *setindex = NULL, *orig_proto;
    int  selected_swap_index = -1, save_button_pressed = 0, total_recorded_item = 0, restart_wan = 0;
    int swap_len = 0, channel_len = 0, wep_len = 0, wpa_len = 0, wpa2_len = 0;
    unsigned long outlen = TMP_LEN;

    if (!cur || !*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    log_time(__func__, __LINE__);
    tmp_wl0_apcli_swap_rule = nvram_get("wl0_apcli_swap_rule");
    ezplib_get_attr_val("wl0_apcli_swap_basic_rule", 0, "max", tmp_max, TMP_LEN, EZPLIB_USE_CLI);
    max = atoi(tmp_max);

    ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", cur_bssid, sizeof(cur_bssid), EZPLIB_USE_CLI);

    /* Save basic visual interface rule */
    snprintf(tmp, sizeof(tmp), "submit_button");
    submit_button = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "wl0_apcli0_ssid");
    not_encode_ssid = websGetVar(wp, tmp, "");
	/* we encode ssid using base64 */
    base64_encode((unsigned char *)not_encode_ssid, strlen(not_encode_ssid),
                                   (unsigned char *)ssid, &outlen);
    snprintf(tmp, sizeof(tmp), "wl0_apcli0_bssid");
    bssid = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "wl0_apcli0_secmode");
    secmode = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "wl0_channel");
    channel = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "wl0_extcha");
    extcha = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "wl0_apcli0_key_index");
    key_index = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "wl0_apcli0_key1");
    key1 = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "wl0_apcli0_key");
    key = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "wl0_apcli0_crypto");
    crypto = websGetVar(wp, tmp, "");
#ifdef DEBUG
    printf("Received ssid:%s bssid:%s secmode:%s channel:%s extcha:%s crypto:%s key:%s\n", ssid, bssid, secmode, channel, extcha, crypto, key);
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

    log_time(__func__, __LINE__);
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

int 
save_wisp_swap(webs_t wp, char *value, struct variable *v, struct service *s) {
    int64_t map = 0;
    int changed = 0, idx;
    char buf[TMP_LEN];
    idx = atoi(value);
    changed = _save_wisp_swap(wp, value, v, s, &map);
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
    if(changed) {
        snprintf(buf, TMP_LEN, "NUM=%d", idx);
        config_postaction(map, s, buf, "");
    }
#endif
    return changed;
}
int
ej_wan_show_wisp_swap_list(int eid, webs_t wp, int argc, char_t ** argv)
{
    char tmp_max[TMP_LEN];
    int max = 0 , i;
    ezplib_get_attr_val("wl0_apcli_swap_basic_rule", 0, "max", tmp_max, TMP_LEN, EZPLIB_USE_CLI);
    max = atoi(tmp_max);

    websWrite(wp, "var SavedWispData = [");

    for(i = 0;i < max; i++) {
        char enable[SHORT_BUF_LEN], ssid[SHORT_BUF_LEN], bssid[SHORT_BUF_LEN],
             secmode[SHORT_BUF_LEN], wifiwan[SHORT_BUF_LEN];

        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "enable", enable, sizeof(enable),EZPLIB_USE_CLI);
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "ssid", ssid, sizeof(ssid),EZPLIB_USE_CLI);
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "bssid", bssid, sizeof(bssid),EZPLIB_USE_CLI);
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "secmode", secmode, sizeof(secmode),EZPLIB_USE_CLI);
        ezplib_get_attr_val("wl0_apcli_swap_rule", i, "wifiwan", wifiwan, sizeof(wifiwan),EZPLIB_USE_CLI);
  	    if (i == 0) {
	    	websWrite(wp, "{Enable:\"%s\",SSID:\"%s\",BSSID:\"%s\",SecMode:\"%s\",WiFiWan:\"%s\"}", enable, ssid, bssid, secmode, wifiwan);
	    } else {
	    	websWrite(wp, ",{Enable:\"%s\",SSID:\"%s\",BSSID:\"%s\",SecMode:\"%s\",WiFiWan:\"%s\"}", enable, ssid, bssid, secmode, wifiwan);
	    }
    }

    websWrite(wp, "];\n");

    return 0;
}
#endif

static int 
_save_wan_wisp(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char new[TMP_LEN], old[TMP_LEN], ssid[TMP_LEN];
    char *enable, *not_encode_ssid, *bssid, *secmode, *channel, *extcha;
#ifdef EZP_SUB_BRAND_APOLLO
    char *ip_type;
#endif
    int len, idx, change = 0;
    unsigned long outlen = TMP_LEN;
    /* Save basic visual interface rule */
    enable = websGetVar(wp, "wan0_enable", "");
    idx = atoi(value);
    not_encode_ssid = _get_apcli_web_variable(wp, idx, idx, "ssid");
    /* we encode ssid using base64 */
    base64_encode((unsigned char *)not_encode_ssid, strlen(not_encode_ssid),
                                   (unsigned char *)ssid, &outlen);
    bssid = _get_apcli_web_variable(wp, idx, idx, "bssid");
    secmode = _get_apcli_web_variable(wp, idx, idx, "secmode");
#ifdef EZP_SUB_BRAND_APOLLO
    ip_type = websGetVar(wp, "wan0_wisp_ip_type", "");
#endif
    /* Save apcli rule */
    /* EZP: In normal universal mode, apcli is applied to lan,
     * the WISP is disabled.*/
    /* EZP: In WISP, we use the apcli0 as wan device and will not bind it
     * with br0. In this case, enabling universal repeater also implies 
     * enabling WISP device. */
#ifdef EZP_SUB_BRAND_APOLLO
    len = snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s^%s", enable, ssid, bssid, 
            secmode, enable,ip_type);
#else
    len = snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s", enable, ssid, bssid, 
            secmode, enable);
#endif
    snprintf(old, TMP_LEN, "wl%d_apcli_rule", idx);
    if (len > TMP_LEN - 1) {
        return 1;
    }
    if (ezplib_check_rule_changed(old, idx, new)) {
        ezplib_replace_rule(old, idx, new);
        change = 1;
    }
    /* EZP: apcli uses same channel with wifi card. */
    channel = _get_web_variable(wp, idx, "channel");
    if (ezplib_check_attr_changed("wl_basic_rule", idx, "channel", channel)) {
        ezplib_replace_attr("wl_basic_rule", idx, "channel", channel);
        change = 1;
    }    
    /* extention channel setting */
    extcha = _get_web_variable(wp, idx, "extcha");
    if (ezplib_check_attr_changed("wl_advanced_rule",
                idx, "extcha", extcha)) {
        ezplib_replace_attr("wl_advanced_rule", idx, "extcha", extcha);
        change = 1;
    }    
    /* Save WDS interface security mode rule */
    if (!strcmp(secmode,"wep")) {
        if (_save_wl_apcli_sec_wep(wp, idx, idx, v, s, map))
            change = 1;
    } else if (!strcmp(secmode,"psk")) {
        if (_save_wl_apcli_sec_psk_or_psk2(wp, idx, idx, 0, v, s, map))
            change = 1;
    } else if (!strcmp(secmode,"psk2")) {
        if (_save_wl_apcli_sec_psk_or_psk2(wp, idx, idx, 1, v, s, map))
            change = 1;
    }
#ifdef EZP_SUB_BRAND_APOLLO
    if (!strcmp(ip_type,"dhcp")){
        if (_save_wan_dhcp(wp, value, v, s, map)) {
            change = 1;
        } 
    } else {
        if (_save_wan_static(wp, value, v, s, map)) {
            change = 1;
        } 
    }
#endif
    if (change) *map |= (ACTION_WIRELESS_START << 16);
    return change;
}

enum {
    ADD = 0,
    REMOVE
};

static int
br_device(int action, char *device)
{
    int ret = 0, len;
    char old[TMP_LEN], new[TMP_LEN];
    switch(action) {
        case ADD:
            ezplib_get_attr_val("br_rule", 0, "ifnames", new, sizeof(new),
                    EZPLIB_USE_CLI);
            if(!strstr(new, "apcli0")) {
                strcat(new, " apcli0");
                ezplib_replace_attr("br_rule", 0, "ifnames", new);
                ret = 1;
            }
            break;
        case REMOVE:
            ezplib_get_attr_val("br_rule", 0, "ifnames", old, sizeof(old),
                    EZPLIB_USE_CLI);
            if(strstr(old, "apcli0")) {
                len = strstr(old, "apcli0") - old;
                strncpy(new, old, len - 1);
                ezplib_replace_attr("br_rule", 0, "ifnames", new);
                ret = 1;
            }
            break;
    }
    return ret;
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


static inline void 
get_cnnt_rule_parm(webs_t wp, int idx, char **name, char **enable, char **expand, char **connection, char **proto) {
    char buf[128];

    snprintf(buf, TMP_LEN, "cnnt_rule%d_name", idx);
    *name = websGetVar(wp, buf, "");

    snprintf(buf, TMP_LEN, "cnnt_rule%d_enable", idx);
    *enable = websGetVar(wp, buf, "");

    snprintf(buf, TMP_LEN, "cnnt_rule%d_expand", idx);
    *expand = websGetVar(wp, buf, "");

    snprintf(buf, TMP_LEN, "cnnt_rule%d_connection", idx);
    *connection = websGetVar(wp, buf, "");

    snprintf(buf, TMP_LEN, "cnnt_rule%d_proto", idx);
    *proto = websGetVar(wp, buf, "");
}

/* In case some GUI doesn't offer any cnnt parameters, only wan0_proto, we
 * have to fix those parameters. */
static char proto_buf[100];
static inline void 
fix_cnnt_rule_parm(char *wan_proto, int idx, char **name, char **enable, char **expand, char **connection, char **proto) {
    int pidx, cidx;
    char *ifname;
    if(!wan_proto)
        return;

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

/* Sometimes, when we leave from a proto, we need to clean up something,
 * like close apcli interface when leave wisp proto. We do it here. */
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
switch_to_proto(char *proto, webs_t wp, char *value, struct variable *v, struct service *s, int64_t *map) {
    int changed = 0;
    switch((proto[0]<<8) | proto[1]) {
        case ('p'<<8) | 'p':
            /* for pppoe */
            changed = _save_wan_pppoe(wp, value, v, s, map);
            break;
        case ('s'<<8) | 't':
            /* for static */
            changed = _save_wan_static(wp, value, v, s, map);
            break;
        case ('w'<<8) | 'i':
            /* for wisp */
            // ezplib_replace_attr("wl0_apcli_rule", 0, "enable", "1");
            changed = _save_wisp_swap(wp, value, v, s, map);
            break;
        case ('w'<<8) | 'w':
            /* for wwan */
            changed = _save_wan_wwan(wp, value, v, s, map);
            break;
    }
    return changed;
}

int
save_cnnt(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int i, num, change = 0, connect_num = 0, change_t;
    char buf[TMP_LEN], tmp[TMP_LEN], cnnt_rule_seq[10];
    int64_t map = 0;
    char *cnnt_num, *name, *expand, *enable, *connection, *proto, *new_proto = NULL, *p_lookup, *new_ifname = NULL;
    char *orig_proto = nvram_safe_get("wan0_proto");
    int proto_index = -1, ctype_index = -1, cnnt_index = -1, rule_set_len = 0, rule_len = 0, first_rule_end_pos = 0;
    char *o_ifname = NULL;
    int o_proto_index = -1, o_ctype_index = -1;
    /*
     * Protocol policy : 
     * 1. If we only have wan0_proto, no cnnt rule, then use cnnt_rule
     * 2. If we only cnnt_rule, no wan0_proto, use cnnt_rule
     * 3. If we have wan0_proto and cnnt_rule but different, use wan0_proto to
     * fix cnnt rule 
     * */
    new_proto = websGetVar(wp, "wan0_proto", "");
    cnnt_num = websGetVar(wp, "cnnt_rule_num", "");
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
    if(new_proto) {
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
        get_cnnt_rule_parm(wp, i, &name, &enable, &expand, &connection, &proto);
        /* For wisp, it doesn't offer any wan0_proto, we have to use
         * "wisp_swap_rule" to check */
        if((!new_proto || !*new_proto) && (!name || !*name) && (!proto || !*proto)) {
            if(websGetVar(wp, "wisp_swap_rule", NULL)) {
                new_proto = WAN_PROTO_WISP;
            } else {
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
    if(!new_proto) {
        /* Strange here, why we don't have a proto ? */
        connect_num = 0;
        new_proto = "dhcp";
    }
    if(ctype_index == -1) {
        decide_proto_ctype_index(new_proto, &proto_index, &ctype_index, &new_ifname);
    }
    if(strcmp(new_proto, orig_proto) || strcmp(buf, nvram_safe_get("cnnt_rule"))) {
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
    change_t = switch_to_proto(new_proto, wp, value, v, s, &map);
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

int
save_wan_priority(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int num;
    char buf[TMP_LEN];
    int64_t map = 0; 
    char *cur = nvram_safe_get("wan_num");
    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    snprintf(buf, TMP_LEN, "NUM=%d", num-1);
    config_preaction(&map, v, s, buf, "");

    char tmp[TMP_LEN];
    char *priority_num, *high, *middle, *low;

    snprintf(tmp, sizeof(tmp), "priority_enable");
    priority_num = websGetVar(wp, tmp, "");
    
    snprintf(tmp, sizeof(tmp), "high");
    high = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "middle");
    middle = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "low");
    low = websGetVar(wp, tmp, "");
    ezplib_replace_attr("wan_priority_rule", 0, "proto", high);
    ezplib_replace_attr("wan_priority_rule", 1, "proto", middle);
    ezplib_replace_attr("wan_priority_rule", 2, "proto", low);
    ezplib_replace_attr("wan_status_rule", 0, "priority_index", "0");
    ezplib_replace_attr("wan_status_rule", 0, "priority_num", priority_num);
    config_postaction(map, s, buf, "");

    return 1;
}

int
save_wan(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *cur = nvram_safe_get("wan_num");
    char tmp[TMP_LEN];
    char buf1[TMP_LEN], buf2[TMP_LEN], buf3[TMP_LEN] = {0};
    char *enable, *proto, *access_type;
    int num;
#ifdef EZP_PROD_BRAND_ZYUS
    char *wan_rule_set = "wan_hwaddr_clone_rule";
    int ret;
    char *hwaddr;
#endif
    int i, change = 0;
    int64_t map;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    access_type = websGetVar(wp, "access_type", "");
    if (!strcmp(access_type,"Apps")) num=1;

    for (i = 0; i < num; i++) {
        char *old_proto;
        map = 0;

        snprintf(buf3, sizeof(tmp), "%d", i);

        snprintf(tmp, sizeof(tmp), "wan%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        if (_save_wan_main(wp, buf3, v, s, &map)) {
            change = 1;
        }
        snprintf(tmp, sizeof(tmp), "wan%d_proto", i);
        /* get old wan proto */ 
        old_proto = nvram_safe_get(tmp);
        /* get new wan proto */
        proto = websGetVar(wp, tmp, "");
        /* for WISP need to change br_rule */
        if (appmgr_check("wisp") && (!i)) {
            if(!strcmp("wisp", proto)) {
                /* if old proto or new proto is wisp 
                 * router need to change br_rule and reboot */
                if (*enable == '1') {
                    /* remove apcli0 from br_rule */
                    br_device(REMOVE, "apcli0");
                    system("brctl delif br0 apcli0");
                } else {
                    /* need to disable apcli rule */
                    ezplib_replace_attr("wl0_apcli_rule",0, "enable", "0"); 
                    /* Add apcli0 into br_rule */
                    br_device(ADD, "apcli0");
                    system("brctl addif br0 apcli0");
                    change = 1;
                }
                v->action_flag |= (ACTION_WIRELESS_START<<24|ACTION_WISP_START<<16);
            } else if(!strcmp("wisp", old_proto)) {
                /* if new proto is not wisp but old is wisp
                 * need to disable apcli rule and remove apcli device from br
                 * rule*/
                ezplib_replace_attr("wl0_apcli_rule",0, "enable", "0"); 
                br_device(ADD, "apcli0");
                system("brctl addif br0 apcli0");
                change = 1;
            }
        }
        if (*enable == '1') {
            if (strcmp(proto, old_proto)) {
                snprintf(buf1, TMP_LEN, "NUM=%d", i);
                config_preaction(&map, v, s, buf1, "");
                nvram_set(tmp, proto);

                /* Set ifname based on proto */
                snprintf(buf1, sizeof(buf1), "wan%d_ifname", i);
                if (!strcmp(proto, "static")) {
                    snprintf(buf2, sizeof(buf2), "wan%d_device", i);
                    nvram_set(buf1, nvram_safe_get(buf2));
                } else if (!strcmp(proto, "dhcp")) {
                    snprintf(buf2, sizeof(buf2), "wan%d_device", i);
                    nvram_set(buf1, nvram_safe_get(buf2));
                } else if (!strcmp(proto, "pppoe")) {
                    snprintf(buf2, sizeof(buf2), "ppp%d", i);
                    nvram_set(buf1, buf2);
                } else if (!strcmp(proto, "wwan")) {
                    snprintf(buf2, sizeof(buf2), "ppp%d", i);
                    nvram_set(buf1, buf2);
                } else if (!strcmp(proto, "directip")) {
                    snprintf(buf2, sizeof(buf2), "usb0");
                    nvram_set(buf1, buf2);
                } else if (!strcmp(proto, "htc")) {
                    snprintf(buf2, sizeof(buf2), "usb0");
                    nvram_set(buf1, buf2);
                } else if (!strcmp(proto, "iphone")) {
                    snprintf(buf2, sizeof(buf2), "eth0");
                    nvram_set(buf1, buf2);
                } else if (!strcmp(proto, "barry")) {
                    snprintf(buf2, sizeof(buf2), "ppp%d", i);
                    nvram_set(buf1, buf2);
                } else if (!strcmp(proto, "beceem")) {
                    snprintf(buf2, sizeof(buf2), "eth0");
                    nvram_set(buf1, buf2);
                } else if (!strcmp(proto, "wimax")) {
                    snprintf(buf2, sizeof(buf2), "wimax0");
                    nvram_set(buf1, buf2);
                } else if (!strcmp(proto, "wisp")) {
                    snprintf(buf2, sizeof(buf2), "apcli0");
                    nvram_set(buf1, buf2);
                } else {
                    assert(0);
                }
                change = 1;

                /* Changing from DHCP, the IP will reside in wan0_ipaddr until 
                 * accquiring the new IP address.
                 */
                snprintf(tmp, sizeof(tmp), "wan%d_ipaddr", i);
                nvram_unset(tmp);
            }

            if (!strcmp(proto, "static")) {
                if (_save_wan_static(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "dhcp")) {
                if (_save_wan_dhcp(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "pppoe")) {
                if (_save_wan_pppoe(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "wwan")) {
                if (_save_wan_wwan(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "barry")) {
                if (_save_wan_barry(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "directip")) {
                if (_save_wan_directip(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "htc")) {
                if (_save_wan_htc(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "iphone")) {
                if (_save_wan_iphone(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "beceem")) {
                if (_save_wan_beceem(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (!strcmp(proto, "wimax")) {
                if (_save_wan_wimax(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else if (strcmp(proto, "wisp") == 0) {
                if (_save_wan_wisp(wp, buf3, v, s, &map)) {
                    change = 1;
                }
            } else {
                assert(0);
            }

            if (_save_wan_vpnc(wp, buf3, v, s, &map)) {
                change = 1;
            }

            /* if product is P1, need to config eth port to wan or lan by wan
             * proto */
            if (appmgr_check("portcfg")) {
                if(!strcmp(proto, "static") || !strcmp(proto, "dhcp") ||
                        !strcmp(proto, "pppoe")){
                    snprintf(tmp, sizeof(tmp), "wan");
                }else {
                    snprintf(tmp, sizeof(tmp), "lan");
                }
                select_port_config(tmp);
            }
        }
#ifdef EZP_PROD_BRAND_ZYUS
        /* for mac clone */
        if (i == 0) {
            snprintf(tmp, sizeof(tmp), "wan%d_hwaddr_clone_enable", i);
            enable = websGetVar(wp, tmp, "");
            snprintf(tmp, sizeof(tmp), "wan%d_hwaddr_clone_hwaddr", i);
            hwaddr = websGetVar(wp, tmp, "");
            ret = snprintf(tmp, TMP_LEN, "%s^%s", enable, hwaddr);
            if (ret >= TMP_LEN) {
                return FALSE;
            }
            ezplib_get_rule(wan_rule_set, i, buf1, sizeof(buf1));
            /* Is clone rule change ? */
            if (strcmp(buf1, tmp)) {
                snprintf(buf1, TMP_LEN, "NUM=%d", i);
                config_preaction(&map, v, s, buf1, "");
                ezplib_replace_rule(wan_rule_set, i, tmp);
                change = 1;
            }
        }
#endif
        if (change) {
            snprintf(buf1, TMP_LEN, "NUM=%d", i);
#if defined (EZP_SUB_BRAND_GARMIN)
            nvram_set("wl0_apcli_rule", "0^^^^");
            ezplib_replace_attr("wan_status_rule",0, "priority_num", "0");
#endif
            config_postaction(map, s, buf1, "");
        }
    }
    return change;
}

int
ej_static_show_setting(int eid, webs_t wp, int argc, char_t **argv)
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
        do_ej("static_setting.asp", wp, buf, msg, "");
    }

    return 0;
}


int
ej_pppoe_show_setting(int eid, webs_t wp, int argc, char_t **argv)
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
        do_ej("pppoe_setting.asp", wp, buf, msg, "");
    }

    return 0;
}


int
ej_wwan_show_setting(int eid, webs_t wp, int argc, char_t **argv)
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
        do_ej("wwan_setting.asp", wp, buf, msg, "");
    }

    return 0;
}


int
ej_wan_show_setting(int eid, webs_t wp, int argc, char_t **argv)
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

#if defined (EZP_PROD_BRAND_PROX)
    /* special wan such like usb */
    if(strstr(WWAN_MODEL, ezp_prod_cat) && function_wan_flag){
        snprintf(msg, sizeof(msg), "%s %d", lang_translate("WAN"), num + 1);
        snprintf(buf, SHORT_BUF_LEN, "%d", num);
        do_ej("wan_setting.asp", wp, buf, msg, "");
    }
#endif

#if  defined (EZP_PROD_BRAND_PROX)
    for (i = num-1; i >= 0; i--) { /* ethernet wan */
#else
    for (i = 0; i < num; i++) { /* ethernet wan */
#endif
        snprintf(msg, sizeof(msg), "%s %d", lang_translate("WAN"), i + 1);
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("wan_setting.asp", wp, buf, msg, "");
#if defined(EZP_PROD_BRAND_ZYUS)
        if (i == 0)
            do_ej("clone_setting.asp", wp, "WAN", "wan", buf, "");
        /* show mac clone setting */
#endif
    }

#if ! defined (EZP_PROD_BRAND_PROX)
    /* special wan such like usb */
    if(strstr(WWAN_MODEL, ezp_prod_cat) && function_wan_flag){
        snprintf(msg, sizeof(msg), "%s %d", lang_translate("WAN"), i + 1);
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("wan_setting.asp", wp, buf, msg, "");
    }
#endif
    return 0;
}

struct wan_proto wire[] = { 
    { "dhcp", "DHCP", NULL},
    { "pppoe", "PPPoE", NULL},
    { "static", "Static IP", NULL},
    { NULL, NULL}
};

struct wan_proto wwan[] = { 
#ifdef EZP_PROD_BRAND_PROX
    { "wwan", "3G/4G Standard Modems", "wwan"},
#else    
    { "wwan", "3G/4G Mobile Internet", "wwan"},
#endif    
    { "htc", "Windows Mobile / Google Android Phones", "smart"},
#ifdef EZP_PROD_BRAND_PROX
    { "directip", "4G DirectIP Modems", "wwan"},
#else    
    { "directip", "HSPA+ Super Speed", "wwan"},
#endif   
    { "barry", "BlackBerry", "barry"},
    { "iphone", "Apple iPhone", "iphone"},
    { "wimax", "WiMAX", "wimax"},
    { "beceem", "WiMAX", "beceem"},
    { NULL, NULL}
};

static void
add_wire_proto(webs_t wp, char *proto)
{
    struct wan_proto *w;
    char tmp[TMP_LEN];
    for (w = &wire[0]; w->name; w++) {
        if(w->name != NULL) {
            /* check this proto is selected */
            if(!strcmp(proto, w->type))
                strcpy(tmp, "selected");
            else
                strcpy(tmp, "");

            websWrite(wp, "<option value=\"%s\" %s>%s</option>",
                    w->type, tmp, w->name);
        }
    }
}

static void
add_wwan_proto(webs_t wp, char *proto)
{
    struct wan_proto *w;
    char tmp[TMP_LEN];
    for (w = &wwan[0]; w->name; w++) {
        if(w->name != NULL) {
            /* check if need show this proto  */
            if(ezplib_get_attr_val("appmgr_rule", 0, w->nv_val,
                 tmp, sizeof(tmp), EZPLIB_USE_CLI) != EZPLIB_NO_ATTRIBUTE) {
                if(tmp[0] == '0')
                    continue;
            }
            /* check this proto is selected */
            if(!strcmp(proto, w->type))
                strcpy(tmp, "selected");
            else
                strcpy(tmp, "");
            websWrite(wp, "<option value=\"%s\" %s>%s</option>",
                    w->type, tmp, w->name);
        }
    }
}
static void
add_wisp(webs_t wp, char *proto)
{
    char tmp[TMP_LEN];
    /* check this proto is selected */
    if(!strcmp(proto, "wisp"))
        strcpy(tmp, "selected");
    else
        strcpy(tmp, "");
#ifdef EZP_PROD_BRAND_PROX
    websWrite(wp, "<option value=\"wisp\" %s>Wi-Fi Client</option>", tmp);
#else
    websWrite(wp, "<option value=\"wisp\" %s>WISP</option>", tmp);
#endif
}

int
ej_wan_show_proto_type(int eid, webs_t wp, int argc, char_t **argv)
{
    int idx;
    int wan_num = atoi(nvram_safe_get("wan_num"));
    char tmp[TMP_LEN];
    char *select_proto;
    if (ejArgs(argc, argv, "%d", &idx) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    snprintf(tmp, sizeof(tmp), "wan%d_proto", idx);
    select_proto = GOZILA_SAFE_GET(tmp);
    if(wan_num == 1) {
        if (appmgr_check("wire")) add_wire_proto(wp, select_proto);
        if(appmgr_check("wwan"))
            add_wwan_proto(wp, select_proto);
        if(appmgr_check("wisp"))
            add_wisp(wp, select_proto);
    } else if(wan_num == 2) { /* X-116NX */
        if(idx == 0) {
            /* wan 1 */
            add_wire_proto(wp, select_proto);
            if(appmgr_check("wisp"))
                add_wisp(wp, select_proto);
        } else {
            /* wan 2 */
            if(appmgr_check("wwan"))
                add_wwan_proto(wp, select_proto);
            else
                add_wire_proto(wp, select_proto);
        }
    } else {
        /* wan num great than 2 */
        if(idx == 0) {
            /* wan 1 */
            add_wire_proto(wp, select_proto);
            if(appmgr_check("wisp"))
                add_wisp(wp, select_proto);
        } else if (idx == (wan_num - 1)){
            /* the last wan */
            if(appmgr_check("wwan"))
                add_wwan_proto(wp, select_proto);
            else 
                add_wire_proto(wp, select_proto);
        } else {
            add_wire_proto(wp, select_proto);
        }
    }
    
    return 0;
}

int
ej_wan_show_type_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    int idx, ret = 0;
    char *type;
    char tmp[TMP_LEN];

    if (ejArgs(argc, argv, "%d", &idx) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(tmp, TMP_LEN, "wan%d_proto", idx);
    type = GOZILA_SAFE_GET(tmp);
    if (!*type) {
        /**
         * As the disable the WAN, the proto from gozila get will be empty.
         * Therefore, the proto should be retrieved from nvram again.
         */
        type = nvram_safe_get(tmp);
    }

    snprintf(tmp, TMP_LEN, "%d", idx);
    if (!strcmp(type, "static")) {
        do_ej("wan_static.asp", wp, tmp, "");
    } else if (!strcmp(type, "pppoe")) {
        do_ej("wan_pppoe.asp", wp, tmp, "");
    } else if (!strcmp(type, "dhcp")) {
        do_ej("wan_dhcp.asp", wp, tmp, "");
    } else if (!strcmp(type, "wwan")) {
        do_ej("wan_wwan.asp", wp, tmp, "");
    } else if (!strcmp(type, "directip")) {
        do_ej("wan_directip.asp", wp, tmp, "");
    } else if (!strcmp(type, "barry")) {
        do_ej("wan_barry.asp", wp, tmp, "");
    } else if (!strcmp(type, "htc")) {
        do_ej("wan_htc.asp", wp, tmp, "");
    } else if (!strcmp(type, "iphone")) {
        do_ej("wan_iphone.asp", wp, tmp, "");
    } else if (!strcmp(type, "beceem")) {
        do_ej("wan_beceem.asp", wp, tmp, "");
    } else if (!strcmp(type, "wimax")) {
        do_ej("wan_wimax.asp", wp, tmp, "");
    } else if (!strcmp(type, "wisp")) {
        do_ej("wlv_apcli_basic.asp", wp, tmp, tmp, "none", "none", "wan");
#ifdef EZP_SUB_BRAND_APOLLO
        do_ej("wan_wisp.asp", wp, tmp, "");
#endif
    } else {
        assert(0);
    }

    return ret;
}
int
ej_wan_show_vpnc_type_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    int idx, ret = 0;
    char *enable;
    char tmp[TMP_LEN], tmp2[TMP_LEN];

    if (ejArgs(argc, argv, "%d", &idx) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(tmp, TMP_LEN, "wan%d_vpnc_enable", idx);
    enable = websGetVar(wp, tmp, "");
    if (!*enable) {
        snprintf(tmp, TMP_LEN, "wan_pptp_l2tp_rule");
        ezplib_get_attr_val(tmp, idx, "enable", tmp2, 
                    TMP_LEN, EZPLIB_USE_CLI); 
    } else{
        sprintf(tmp2, "%s", enable);
    }
    if (appmgr_check("vpnc")) {
        snprintf(tmp, TMP_LEN, "%d", idx);
        if (!strcmp(tmp2, "1")) {
            do_ej("wan_vpnc.asp", wp, tmp, "");
        }
    }

    return ret;
}
#ifdef EZP_SUB_BRAND_APOLLO
int
ej_wan_show_wisp_type_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    int idx, ret = 0;
    char *enable;
    char tmp[TMP_LEN], tmp2[TMP_LEN];

    if (ejArgs(argc, argv, "%d", &idx) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(tmp, TMP_LEN, "wan%d_wisp_ip_type", idx);
    enable = websGetVar(wp, tmp, "");
    if (!*enable) {
        snprintf(tmp, TMP_LEN, "wl0_apcli_rule");
        ezplib_get_attr_val(tmp, idx, "ip_type", tmp2, 
                    TMP_LEN, EZPLIB_USE_CLI); 
    } else{
        sprintf(tmp2, "%s", enable);
    }
    snprintf(tmp, TMP_LEN, "%d", idx);
    if (!strcmp(tmp2, "dhcp")) {
        do_ej("wan_dhcp.asp", wp, tmp, "");
    } else {
        do_ej("wan_static.asp", wp, tmp, "");
    }

    return ret;
}
#endif

int
ej_wan_wwan_create_modem_array(int eid, webs_t wp, int argc, char_t **argv)
{
    int i, j;
    struct ezp_com_map_entry_type *map_ptr;

    /* wwan */
    websWrite(wp, "var modem_models = new Array();\n");
    for (i = 0; strlen(ezp_com_vendor_map[i].desc); i++) {
        /* XXX: We might have the empty */
        if (ezp_com_vendor_map[i].shown) {
            map_ptr = ezp_com_pointer_map[i];
            websWrite(wp, "modem_models[%d] = new Array();\n", i);
            for (j = 0; strlen(map_ptr[j].desc); j++)  {
                /* Only list the correspondent wwan cards. */
                if (map_ptr[j].Flag == 0x1) {
                    websWrite(wp, "modem_models[%d][%d] = \"%s\";\n", i, j, 
                            map_ptr[j].desc);
                }
            }
        }
    }
    /* directip */
    websWrite(wp, "var directip_modem_models = new Array();\n");

    websWrite(wp, "directip_modem_models[0] = new Array();\n");
    websWrite(wp, "directip_modem_models[0][0] = \"%s\";\n",
            lang_translate("Auto"));
    for (i = 1; strlen(ezp_com_vendor_map[i].desc); i++) {
        /* XXX: We might have the empty */
        if (ezp_com_vendor_map[i].shown) {
            map_ptr = ezp_com_pointer_map[i];
            websWrite(wp, "directip_modem_models[%d] = new Array();\n", i);
            for (j = 0; strlen(map_ptr[j].desc); j++)  {
                /* Only list the correspondent directip cards. */
                if (map_ptr[j].Flag >= 0x2) {
                    websWrite(wp, "directip_modem_models[%d][%d] = \"%s\";\n", 
                            i, j, map_ptr[j].desc);
                }
            }
        }
    }
    return 0;
}

int
ej_wan_wwan_show_modem(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth, shown;
    char *val;
    char tmp[TMP_LEN], buf[TMP_LEN];
    int i, nbrand, nmodel;
    struct ezp_com_map_entry_type *map_ptr;

    if (ejArgs(argc, argv, "%d %d", &nth, &shown) < 2) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }

    snprintf(tmp, sizeof(tmp), "wan%d_wwan_brand", nth);
    val = GOZILA_SAFE_GET(tmp);
    if (!*val) {
        ezplib_get_attr_val("wan_wwan_rule", nth, "brand", buf, sizeof(buf),
                EZPLIB_USE_CLI);
        val = buf;
    }
    nbrand = atoi(val);

    snprintf(tmp, sizeof(tmp), "wan%d_wwan_model", nth);
    val = GOZILA_SAFE_GET(tmp);
    if (!*val) {
        ezplib_get_attr_val("wan_wwan_rule", nth, "model", buf, sizeof(buf),
                EZPLIB_USE_CLI);
        val = buf;
    }
    nmodel = atoi(val);

    /* Brand elements*/
  #ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp,"<div class='row show-grid'>\n");
    websWrite(wp,"  <div class='span4'>%s</div>\n", lang_translate("Modem Brand"));
    websWrite(wp,"  <div class='span8'>\n");    
  #else
    websWrite(wp, "<div class='setting'>\n");
    websWrite(wp, "<div class='label'>%s</div>\n", lang_translate("Modem Brand"));
  #endif
    websWrite(wp, "<select name='wan%d_wwan_brand' id='wan%d_wwan_brand' "
                        "value='%d' onchange='sel_brand%d(%d)'>\n",
                        nth, nth, nbrand, shown, nth);

    /* Checked the 'shown' flag of selected brand/model. */
    i=ezp_com_pointer_map[nbrand][nmodel].Flag;
    if (i>2) i=2;
    if (i != shown) {
        nbrand = nmodel = 0;
    }
        
    websWrite(wp, "<option value=z'0' %s>%s</option>\n",
            (nbrand == 0) ? "selected" : "", lang_translate("Auto"));

    for (i = 1; strlen(ezp_com_vendor_map[i].desc); i++) {
        if (ezp_com_vendor_map[i].shown) {
            int j, check = 0;

            /* Don't show the vendor if all models of them don't belong to
             * this protocol (wwan/directip).
             */
            map_ptr = ezp_com_pointer_map[i]; 
            for (j = 0; strlen(map_ptr[j].desc); j++) {
                if (shown == 2){
                    if (map_ptr[j].Flag >= 2){
                        check = 1;
                        break;
                    }
                }
                else if (map_ptr[j].Flag == shown) {
                    check = 1;
                    break;
                }
            }
            if (check == 1) {
                websWrite(wp, "<option value='%d' %s>%s</option>\n", i,
                        (nbrand == i) ? "selected" : "",
                        ezp_com_vendor_map[i].desc);
            }
        }
    }
    websWrite(wp, "</select>\n");
#ifdef EZP_PROD_BRAND_AXIM 
    websWrite(wp,"  </div>\n");
    websWrite(wp,"</div>\n");
#else
    websWrite(wp, "</div>\n");
#endif

    /* Model elements */
#ifdef EZP_PROD_BRAND_AXIM
     websWrite(wp, "<div class='row show-grid'>\n");
     websWrite(wp, "    <div class='span4'>%s</div>\n", lang_translate("Modem Model"));
     websWrite(wp, "    <div class='span8'>\n");
#else    
    websWrite(wp, "<div class='setting'>\n");
    websWrite(wp, "<div class='label'>%s</div>\n", lang_translate("Modem Model"));
#endif

    websWrite(wp, "<div id='wan%d_wwan_model_wrap'>\n", nth);
    websWrite(wp, "<select name='wan%d_wwan_model' id='wan%d_wwan_model'>\n",
                    nth, nth);
    if (nbrand == 0) {
        /* Auto */
        websWrite(wp, "<option value='0' selected>%s</option>\n",
                lang_translate("Auto"));
    } else {
        /* Get the model table according the selected brand */
        map_ptr = ezp_com_pointer_map[nbrand]; 
        for (i = 0; strlen(map_ptr[i].desc); i++) {
            if (shown == 2){
                if (map_ptr[i].Flag >= 2){
                    websWrite(wp, "<option value='%d' %s>%s</option>\n", i,
                            (nmodel == i) ? "selected" : "", map_ptr[i].desc);
                }
            }
            else if (map_ptr[i].Flag == shown) {
                websWrite(wp, "<option value='%d' %s>%s</option>\n", i,
                        (nmodel == i) ? "selected" : "", map_ptr[i].desc);
            }
        }
    }
        
    websWrite(wp, "</select>\n");
    websWrite(wp, "</div>\n");
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "    </div>\n");
    websWrite(wp, "</div>\n");
#else
    websWrite(wp, "</div>\n");
#endif
    return 0;
}

int
ej_wan_wwan_create_isp_array(int eid, webs_t wp, int argc, char_t **argv)
{
    int i, j;

    websWrite(wp, "var isps = new Array();\n");
    for (i = 0; strlen(ezp_com_location_map[i].desc); i++) {
        struct ezp_com_isp_map_entry_type *map = ezp_com_isp_map[i];

        websWrite(wp, "isps[%d] = new Array();\n", i);
        for (j = 0; strlen(map[j].desc); j++)  {
            websWrite(wp, "/* %s */\n", map[j].desc);
            websWrite(wp, "isps[%d][%d] = new Array();\n", i, j);
            websWrite(wp, "isps[%d][%d][0] = \"%s\";\n", i, j,
                    lang_translate(map[j].desc));
            websWrite(wp, "isps[%d][%d][1] = \"%s\";\n", i, j, map[j].apn);
            websWrite(wp, "isps[%d][%d][2] = \"%s\";\n", i, j, map[j].pin);
            websWrite(wp, "isps[%d][%d][3] = \"%s\";\n", i, j, map[j].auth);
            websWrite(wp, "isps[%d][%d][4] = \"%s\";\n", i, j,
                    map[j].user_name);
            websWrite(wp, "isps[%d][%d][5] = \"%s\";\n", i, j, map[j].passwd);
            websWrite(wp, "isps[%d][%d][6] = \"%s\";\n", i, j,
                    map[j].dialup_str);
        }
    }
    return 0;
}

int
ej_wan_wwan_show_isp(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth, type;
    char *val, *proto;
    char buf[TMP_LEN];
    int i, nlocation, nisp;
    struct ezp_com_isp_map_entry_type *map_ptr;

    if (ejArgs(argc, argv, "%d", &nth) < 1) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }

    snprintf(buf, sizeof(buf), "wan%d_proto", nth);
    proto = websGetVar(wp, buf, "");
    if (!strcmp(proto, "wwan")) {
        type = 1;
    } else  if (!strcmp(proto, "directip")) {
        type = 2;
    } else {
        type = 0;
    }

    /* Location */
    snprintf(buf, sizeof(buf), "wan%d_wwan_location", nth);
    val = GOZILA_SAFE_GET(buf);
    if (!*val) {
        ezplib_get_attr_val("wan_wwan_rule", nth, "location", buf, sizeof(buf),
                EZPLIB_USE_CLI);
        val = buf;
    }
    nlocation = atoi(val);
#ifndef EZP_PROD_BRAND_ZYUS
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "<div class='row show-grid'>\n");
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
    websWrite(wp, "     <div class='span2'>%s</div>\n", lang_translate("Location"));
    websWrite(wp, "     <div class='span4'>\n");
#else
    websWrite(wp, "     <div class='span4'>%s</div>\n", lang_translate("Location"));
    websWrite(wp, "     <div class='span8'>\n");
#endif
#else    
    websWrite(wp, "<div class='setting'>\n");
    websWrite(wp, "<div class='label'>%s</div>\n", lang_translate("Location"));
#endif
#endif

#ifdef EZP_PROD_BRAND_ZYUS
    websWrite(wp, "<input type=hidden name='wan%d_wwan_location' id='wan%d_wwan_location' "
                        "value='%d' onchange='sel_location(%d, %d)'>\n",
                        nth, nth, nlocation, nth, type);
#else
    websWrite(wp, "<select name='wan%d_wwan_location' id='wan%d_wwan_location' value='%d' onchange='sel_location(%d, %d)'>\n",
                        nth, nth, nlocation, nth, type);
#endif
#ifndef EZP_PROD_BRAND_ZYUS
    for (i = 0; strlen(ezp_com_location_map[i].desc); i++) {
        websWrite(wp, "<option value='%d' %s>%s</option>\n",
                i, (nlocation == i) ? "selected" : "",
                lang_translate(ezp_com_location_map[i].desc));
    }
#endif
    websWrite(wp, "</select>\n");
#ifndef EZP_PROD_BRAND_ZYUS
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "     </div>\n");
    websWrite(wp, "</div>\n");
#else    
    websWrite(wp, "</div>\n");
#endif
#endif

    /* ISP */
    snprintf(buf, sizeof(buf), "wan%d_wwan_isp", nth);
    val = GOZILA_SAFE_GET(buf);
    if (!*val) {
        ezplib_get_attr_val("wan_wwan_rule", nth, "isp", buf, sizeof(buf),
                EZPLIB_USE_CLI);
        val = buf;
    }
    nisp = atoi(val);
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "<div class='row show-grid'>\n");
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
    websWrite(wp, "     <div class='span2'>%s</div>\n", lang_translate("Service Provider"));
    websWrite(wp, "     <div class='span4'>\n");
#else
    websWrite(wp, "     <div class='span4'>%s</div>\n", lang_translate("Service Provider"));
    websWrite(wp, "     <div class='span8'>\n");
#endif
#else    
    websWrite(wp, "<div class='setting'>\n");
    websWrite(wp, "<div class='label'>%s</div>\n", lang_translate("Service Provider"));
#endif    
    websWrite(wp, "<div id='wan%d_wwan_isp_wrap'>\n", nth);
#ifdef EZP_PROD_BRAND_ZYUS
    websWrite(wp, "<select name='wan%d_wwan_isp' id='wan%d_wwan_isp' "
            "value='%d' onchange='sel_isp(%d, %d)'>\n",
            nth, nth, nisp, nth, type);
#else
    websWrite(wp, "<select name='wan%d_wwan_isp' id='wan%d_wwan_isp' "
            "value='%d' onchange='sel_isp(%d)'>\n",
            nth, nth, nisp, nth);
#endif

    map_ptr  = ezp_com_isp_map[nlocation];

    for (i = 0; strlen(map_ptr[i].desc); i++) {
        websWrite(wp, "<option value='%d' %s>%s</option>\n",
                i, (nisp == i) ? "selected" : "",
                lang_translate(map_ptr[i].desc));
    }
    websWrite(wp, "</select>\n");
    websWrite(wp, "</div>\n");
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "     </div>\n");
    websWrite(wp, "</div>\n");
#else    
    websWrite(wp, "</div>\n");
#endif
    return 0;
}

int
ej_wan_barry_show_isp(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth, type;
    char *val, *proto;
    char buf[TMP_LEN];
    int i, nlocation, nisp;
    struct ezp_com_isp_map_entry_type *map_ptr;

    if (ejArgs(argc, argv, "%d", &nth) < 1) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }

    snprintf(buf, sizeof(buf), "wan%d_proto", nth);
    proto = websGetVar(wp, buf, "");
    if (!strcmp(proto, "barry")) {
        type = 1;
    } else {
        type = 0;
    }

    /* Location */
    snprintf(buf, sizeof(buf), "wan%d_barry_location", nth);
    val = GOZILA_SAFE_GET(buf);
    if (!*val) {
        ezplib_get_attr_val("wan_barry_rule", nth, "location", buf, sizeof(buf),
                EZPLIB_USE_CLI);
        val = buf;
    }
    nlocation = atoi(val);
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "<div class='row show-grid'>\n");
    websWrite(wp, "     <div class='span4'>%s</div>\n", lang_translate("Location"));
    websWrite(wp, "     <div class='span8'>\n");
#else
    websWrite(wp, "<div class='setting'>\n");
    websWrite(wp, "<div class='label'>%s</div>\n", lang_translate("Location"));
#endif    
    websWrite(wp, "<select name='wan%d_barry_location' id='wan%d_barry_location' "
                        "value='%d' onchange='sel_barry_location(%d, %d)'>\n",
                        nth, nth, nlocation, nth, type);

    for (i = 0; strlen(ezp_com_location_map[i].desc); i++) {
        websWrite(wp, "<option value='%d' %s>%s</option>\n",
                i, (nlocation == i) ? "selected" : "",
                lang_translate(ezp_com_location_map[i].desc));
    }
    websWrite(wp, "</select>\n");
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "     </div>\n");
    websWrite(wp, "</div>\n");
#else
    websWrite(wp, "</div>\n");
#endif
    /* ISP */
    snprintf(buf, sizeof(buf), "wan%d_barry_isp", nth);
    val = GOZILA_SAFE_GET(buf);
    if (!*val) {
        ezplib_get_attr_val("wan_barry_rule", nth, "isp", buf, sizeof(buf),
                EZPLIB_USE_CLI);
        val = buf;
    }
    nisp = atoi(val);
#ifdef EZP_PROD_BRAND_AXIM    
    websWrite(wp, "<div class='row show-grid'>\n");
    websWrite(wp, "     <div class='span4'>%s</div>\n", lang_translate("Service Provider"));
    websWrite(wp, "     <div class='span8'>\n");
#else
    websWrite(wp, "<div class='setting'>\n");
    websWrite(wp, "<div class='label'>%s</div>\n", lang_translate("Service Provider"));
#endif    
    websWrite(wp, "<div id='wan%d_barry_isp_wrap'>\n", nth);
    websWrite(wp, "<select name='wan%d_barry_isp' id='wan%d_barry_isp' "
                        "value='%d' onchange='sel_barry_isp(%d)'>\n",
                        nth, nth, nisp, nth);

    map_ptr  = ezp_com_isp_map[nlocation];

    for (i = 0; strlen(map_ptr[i].desc); i++) {
        websWrite(wp, "<option value='%d' %s>%s</option>\n",
                i, (nisp == i) ? "selected" : "",
                lang_translate(map_ptr[i].desc));
    }
    websWrite(wp, "</select>\n");
    websWrite(wp, "</div>\n");
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
#else
    websWrite(wp, "</div>\n");
#endif
    return 0;
}

int
ej_wan_create_proto_array(int eid, webs_t wp, int argc, char_t **argv)
{
    char tmp[TMP_LEN];
    char *cur = nvram_safe_get("wan_num");
    char *proto;
    int i, num;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    websWrite(wp, "var wan_proto = new Array();\n");
    for (i = 0; i < num; i++) {
        snprintf(tmp, sizeof(tmp), "wan%d_proto", i);
        proto = nvram_safe_get(tmp);
        websWrite(wp, "wan_proto[%d] = '%s';\n", i, proto);
    }
    return 0;
}

int
ej_wan_show_wan_type_html_start(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth;

    if (ejArgs(argc, argv, "%d", &nth) < 1) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }
    if (nth == 1) {
        websWrite(wp, "<!--");
    }
    return 0;
}

int
ej_wan_show_wan_type_html_end(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth;

    if (ejArgs(argc, argv,"%d", &nth) < 1) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }

    if (nth == 1) {
        websWrite(wp, "-->");
    }
    return 0;
}
#ifdef EZP_PROD_BRAND_ZYUS
int
ej_wwan_show_wan_type_html_start(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    char *wwan_type;
    char wwan_disp[3], smartphone_disp[3];
    int num, nth;

    if (ejArgs(argc, argv, "%d %s", &nth, &wwan_type) < 1) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }

    ezplib_get_attr_val("display_rule", 0, "wwan", wwan_disp,
            sizeof(wwan_disp), EZPLIB_USE_CLI);
    if(!strncmp(wwan_type, "wwan", sizeof("wwan")) &&
            (wwan_disp[0] == '0')) {
        websWrite(wp, "<!--");
        return 0;
    }
    ezplib_get_attr_val("display_rule", 0, "smartphone", smartphone_disp,
            sizeof(smartphone_disp), EZPLIB_USE_CLI);
    if(!strncmp(wwan_type, "smartphone", sizeof("smartphone")) &&
            (smartphone_disp[0] == '0')) {
        websWrite(wp, "<!--");
        return 0;
    }
    ezplib_get_attr_val("display_rule", 0, "iphone", smartphone_disp,
            sizeof(smartphone_disp), EZPLIB_USE_CLI);
    if(!strncmp(wwan_type, "iphone", sizeof("iphone")) &&
            (smartphone_disp[0] == '0')) {
        websWrite(wp, "<!--");
        return 0;
    }


    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    /* FIXME: We don't support multiple 3G now. So we only allow the "last"
     * WAN to display the 3G related items.
     * eg., 1 WAN => allow wan0
     *      2 WAN => allow wan1
     */
    if (nth != num -1) {
        websWrite(wp, "<!--");
        return 0;
    }
    return 0;
}

int
ej_wwan_show_wan_type_html_end(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    char *wwan_type;
    char wwan_disp[3], smartphone_disp[3];
    int num, nth;

    if (ejArgs(argc, argv, "%d %s", &nth, &wwan_type) < 1) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }

    ezplib_get_attr_val("m_rule", 0, "wwan", wwan_disp,
            sizeof(wwan_disp), EZPLIB_USE_CLI);
    if(!strncmp(wwan_type, "wwan", sizeof("wwan")) &&
            (wwan_disp[0] == '0')) {
        websWrite(wp, "-->");
        return 0;
    }
    ezplib_get_attr_val("appmgr_rule", 0, "smartphone", smartphone_disp,
            sizeof(smartphone_disp), EZPLIB_USE_CLI);
    if(!strncmp(wwan_type, "smartphone", sizeof("smartphone")) &&
            (smartphone_disp[0] == '0')) {
        websWrite(wp, "-->");
        return 0;
    }
    ezplib_get_attr_val("appmgr_rule", 0, "iphone", smartphone_disp,
            sizeof(smartphone_disp), EZPLIB_USE_CLI);
    if(!strncmp(wwan_type, "iphone", sizeof("iphone")) &&
            (smartphone_disp[0] == '0')) {
        websWrite(wp, "-->");
        return 0;
    }

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    /* FIXME: We don't support multiple 3G now. So we only allow the "last"        * WAN to display the 3G related items. 
     * eg., 1 WAN => allow wan0
     *      2 WAN => allow wan1
     */
    if (nth != num -1) {
        websWrite(wp, "-->");
        return 0;
    }
    return 0;
}
#endif
