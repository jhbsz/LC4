/*
 * Broadcom Home Gateway Reference Design
 * Web Page Configuration Support Routines
 *
 * Copyright 2001-2003, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: wireless.c,v 1.44 2005/05/25 12:31:59 honor Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <error.h>
#include <signal.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <wlioctl.h>
#include <wlutils.h>

#include <md5.h>

#include "ezp.h"
#include "lang.h"
#include "ezp-lib.h"
#include "nvram_ezpacket.h"
#include "common.h"

#define WL_SSID_MAX_LENGTH "32"

static int _save_wl_adv(webs_t wp, char *value, struct variable *v,
        struct service *s, int64_t *map);
static int _save_wl_basic(webs_t wp, char *value, struct variable *v,
        struct service *s, int64_t *map);
static int _save_wlv_basic(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map);
static int _save_wl_wds(webs_t wp, char *value, struct variable *v,
        struct service *s, int64_t *map);
static int _save_wlv_wds(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map);
static int _save_wlv_apcli(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map);

char *_get_web_variable(webs_t wp, int nth, char *item);
static char *_get_virtual_web_variable(webs_t wp, int if_idx, int vif_idx, char *item);
static char *_get_wds_web_variable(webs_t wp, int if_idx, int vif_idx, char *item);
char *_get_apcli_web_variable(webs_t wp, int if_idx, int vif_idx, char *item);

static void 
wl_log_string(char *aLogStr_p);

int nv_count;

enum {
    WL_REGION = 0
};

struct variable wl_variables[] = {
  {longname: "Wireless Region", argv: ARGV("0", "1", "2", "5"), nullok: FALSE}
};

enum WlAdvEnum {
    WL_FRAGMENTATION,
    WL_RTS,
    WL_DTIM,
    WL_BCN,
    WL_PLCPHDR,
    WL_ANTDIV,
    WL_TXBURST,
    WL_PKTAGGR,
    WL_TXMODE,
    WL_RDG,
    WL_LINKADAPT,
    WL_HTC,
    WL_HTBW,
    WL_GI,
    WL_OPMODE,
    WL_MCS,
    WL_MPDU,
    WL_AMSDU,
    WL_AUTOBA,
    WL_BAWINSIZE,
    WL_BADECLINE
};

static struct variable wl_adv_variables[] = {
    {longname: "Packet Fragmentation", argv:ARGV("")},
    {longname: "RTS", argv:ARGV("")},
    {longname: "DTim", argv:ARGV("")},
    {longname: "Beacon Interval", argv:ARGV("")},
    {longname: "Header Preamble", argv:ARGV("long","short")},
    {longname: "Antinna Diversity", argv:ARGV("-1","0","1","3")},
    {longname: "Txburst", argv:ARGV("0","1")},
    {longname: "PktAggregation", argv:ARGV("0","1")},
    {longname: "TxMode", argv:ARGV("HT", "1", "2")},
    {longname: "RDG", argv:ARGV("0","1")},
    {longname: "LinkAdapt", argv:ARGV("0","1")},
    {longname: "HTC", argv:ARGV("0","1")},
    {longname: "HTBW", argv:ARGV("0","1")},
    {longname: "GI", argv:ARGV("0","1")},
    {longname: "OPMode", argv:ARGV("0","1")},
    {longname: "MCS", argv:ARGV("0","1")},
    {longname: "MPDU", argv:ARGV("0","1")},
    {longname: "AMSDU", argv:ARGV("0","1")},
    {longname: "AUTOBA", argv:ARGV("0","1")},
    {longname: "BAWINSIZE", argv:ARGV("1","64")},
    {longname: "BADECLINE", argv:ARGV("0","1")},
};

enum GuestLAN {
    GUEST_LAN_ENABLE,
    GUEST_LAN_IPADDR,
    GUEST_LAN_MASK
};

static struct variable guest_lan_variables[] = {
    {longname: "Guest LAN", argv:ARGV("0", "1")},
    {longname: "Guest LAN IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "Guest LAN Netmask", argv:ARGV("16", "32"), nullok: FALSE},
};

enum WlBasicEnum {
    WL_RADIO,
    WL_NETWORK_MODE,
    WL_TXPOWER,
    WL_CHANNEL,
    WL_BISOLATION
};

static struct variable wl_basic_variables[] = {
    {longname: "Wireless Connection Enable", argv:ARGV("0", "1")},
    {longname: "Wireless Network Mode", argv:ARGV("0", "1", "4","6", "7", "9")},
    {longname: "Tx Power", argv:ARGV("10","25","50","75","100")},
    {longname: "Channel", argv:ARGV("")},
    {longname: "BIsolation", argv:ARGV("0","1")},
};

enum WlvBasicEnum {
    WLV_SSID_ENABLE,
    WLV_HIDE_SSID,
    WLV_TXRATE,
    WLV_WME,
    WLV_ISOLATION,
    WLV_SSID,
    WLV_SECMODE,
    /* Security mode WEP */
    WLV_SEC_WEP_KEY_INDEX,
    WLV_SEC_WEP_KEY1,
    WLV_SEC_WEP_KEY2,
    WLV_SEC_WEP_KEY3,
    WLV_SEC_WEP_KEY4,
    /* Security mode PSK/PSK2 */
    WLV_SEC_PSK_KEY,
    WLV_SEC_PSK_CRYPTO,
    WLV_SEC_PSK_PREAUTH,
    /* Security mode WPA/WPA2 */
    WLV_SEC_WPA_RADIUS_KEY,
    WLV_SEC_WPA_RADIUS_IPADDR,
    WLV_SEC_WPA_RADIUS_PORT,
    WLV_SEC_WPA_RADIUS_CRYPTO,
    WLV_SEC_WPA_RADIUS_REKEY_METHOD,
    WLV_SEC_WPA_RADIUS_REKEY_TIME_INTERVAL,
    WLV_SEC_WPA_RADIUS_REKEY_PKT_INTERVAL,
    WLV_SEC_WPA_PREAUTH
};

/* enable^hidden^rate^wme */
static struct variable wlv_basic_variables[] = {
    {longname: "Wireless SSID Enable", argv:ARGV("0", "1")},
    {longname: "Wireless HideSSID", argv:ARGV("0", "1")},
    {longname: "Wireless Tx Rate", argv:ARGV("")},
    {longname: "Wireless Multimedia", argv:ARGV("0", "1")},
    {longname: "Wireless Isolation", argv:ARGV("0", "1")},
    {longname: "Wireless SSID", argv:ARGV(WL_SSID_MAX_LENGTH)},
    {longname: "Wireless Security Mode", 
            argv:ARGV("disabled", "wep", "psk", "wpa", "psk2", "wpa2")},
    /* WEP validate content */
    {longname: "Wireless WEP Key Index", argv:ARGV("1", "4")},
    {longname: "Wireless Key 1", argv:ARGV("")},
    {longname: "Wireless Key 2", argv:ARGV("")},
    {longname: "Wireless Key 3", argv:ARGV("")},
    {longname: "Wireless Key 4", argv:ARGV("")},
    /* PSK/PSK2 validate content */
    {longname: "Wireless WPA Key", argv:ARGV("")},
    {longname: "Wireless Encryption Method", argv:ARGV("tkip", "aes", "mixed")},
    {longname: "Wireless Pre-authentication", argv:ARGV("0","1")},
    /* WPA/WPA2 Wireless validate content */
    {longname: "Wireless Radius Key", argv:ARGV("64")},
    {longname: "Wireless Radius Server IP", argv:ARGV("")},
    {longname: "Wireless Radius Server Port", argv:ARGV("1", "65535")},
    {longname: "Wireless Encryption Method", argv:ARGV("tkip", "aes", "mixed")},
    {longname: "Wireless Rekey Method", argv:ARGV("disable", "time", "pkt")},
    {longname: "Wireless Rekey Time Interval", argv:ARGV("0", "99999")},
    {longname: "Wireless Rekey Packet Interval", argv:ARGV("0", "99999")},
    {longname: "Wireless Pre-authentication", argv:ARGV("0", "1")},
};

int
ej_wl_show_radius_html_start(int eid, webs_t wp, int argc, char_t **argv)
{
    char radius_disp[TMP_LEN];
    ezplib_get_attr_val("wl_wps_rule", 0, "mode", radius_disp,
            TMP_LEN, EZPLIB_USE_CLI);

    if (!strncmp(radius_disp, "7", TMP_LEN)) {
        websWrite(wp, "<!--");
    }
    return 0;
}

int
ej_wl_show_radius_html_end(int eid, webs_t wp, int argc, char_t **argv)
{
    char radius_disp[TMP_LEN];
    ezplib_get_attr_val("wl_wps_rule", 0, "mode", radius_disp,
            TMP_LEN, EZPLIB_USE_CLI);
    if (!strncmp(radius_disp, "7", TMP_LEN)) {
        websWrite(wp, "-->");
    }
    return 0;
}

int
valid_base64(webs_t wp, char *value, struct variable *v)
{
    char c[TMP_LEN];
    int i, flag=0;
    strcpy(c,value);
    for (i=0;i<strlen(value);i++){
        if (c[i]==' ') {
            c[i]='+';
            flag=1;
        }
        if (!(isalnum(c[i]) || (c[i]=='+') || (c[i]=='/') || (c[i]=='='))) {
            websDebugWrite(wp, "Invalid <b>%s</b> %s: "
                    "must be base64 code<br>", 
                    v->longname, value);
            return FALSE;
        }
    }
    if (flag) strcpy(value,c);
    return TRUE;
}

int
valid_wl_channel(webs_t wp, char *value, struct variable *v)
{
    int region = atoi(nvram_safe_get("wl_region"));
    int channel = atoi(value), min = 0, max = 0;

    if (region == 5) {
        min = 1;
        max = 14;
    } else if (region == 3) {
        min = 10;
        max = 13;
    } else if (region == 1) {
        min = 1;
        max = 13;
    } else {
        min = 1;
        max = 11;
    }

    if ((channel < min || channel > max) && channel != 0) {
        websDebugWrite(wp, "Invalid <b>%s</b>: valid channels are %d-%d<br>",
                       v->longname, min, max);
        return FALSE;
    }

    return TRUE;
}

int
valid_wl_adv(webs_t wp, char *value, struct variable *v)
{
    int wl_num, idx;
    char *frag, *rts, *dtim, *bcn, *plcphdr, *antdiv;
    char *region;

    region = websGetVar(wp, "wl_region", "");
    if (valid_choice(wp, region, &wl_variables[WL_REGION]) == FALSE) {
        return FALSE;
    }

    wl_num = atoi(nvram_safe_get("wl_rule_num"));
    for (idx = 0;idx < wl_num;idx++) {
        frag = _get_web_variable(wp, idx, "frag");
        rts = _get_web_variable(wp, idx, "rts");
        dtim = _get_web_variable(wp, idx, "dtim");
        bcn = _get_web_variable(wp, idx, "bcn");
        plcphdr = _get_web_variable(wp, idx, "plcphdr");
        antdiv = _get_web_variable(wp, idx, "antdiv");

        if (valid_number(wp, frag, &wl_adv_variables[WL_FRAGMENTATION])
                == FALSE) {
            return FALSE;
        }

        if (valid_number(wp, rts, &wl_adv_variables[WL_RTS]) == FALSE) {
            return FALSE;
        }

        if (valid_number(wp, dtim, &wl_adv_variables[WL_DTIM]) == FALSE) {
            return FALSE;
        }

        if (valid_number(wp, bcn, &wl_adv_variables[WL_BCN]) == FALSE) {
            return FALSE;
        }
        if (valid_choice(wp, plcphdr, &wl_adv_variables[WL_PLCPHDR]) == FALSE) {
            return FALSE;
        }

        if (valid_choice(wp, antdiv, &wl_adv_variables[WL_ANTDIV]) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

static int 
_valid_wl_basic(webs_t wp, char *value, struct variable *v)
{
    /* radio^net_mode^txpower^channel */
    char *radio_enable, *net_mode, *txpower, *channel, *bisolation;
    int idx = atoi(value);

    radio_enable = _get_web_variable(wp, idx, "radio_enable");
    if (valid_choice(wp, radio_enable, &wl_basic_variables[WL_RADIO]) 
            == FALSE) {
        return FALSE;
    }


    if (*radio_enable) {
        net_mode = _get_web_variable(wp, idx, "net_mode");
        if (valid_choice(wp, net_mode, &wl_basic_variables[WL_NETWORK_MODE])
                == FALSE) {
            return FALSE;
        }

        txpower = _get_web_variable(wp, idx, "txpower");
        if (valid_choice(wp, txpower, &wl_basic_variables[WL_TXPOWER]) 
                == FALSE) {
            return FALSE;
        }

        channel = _get_web_variable(wp, idx, "channel");
        if (valid_wl_channel(wp, channel, 
                             &wl_basic_variables[WL_CHANNEL]) == FALSE) {
            return FALSE;
        }

        bisolation = _get_web_variable(wp, idx, "bisolation_enable");
        /* *bisolation could be empty for a single SSID platform. */
        if (*bisolation &&
                valid_choice(wp, bisolation, 
                    &wl_basic_variables[WL_BISOLATION]) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

static int 
_valid_wlv_sec_wep(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{
    char *key_index, *key, key_name[TMP_LEN];
    int index, i;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);

    key_index = _get_virtual_web_variable(wp, if_idx, vif_idx, "key_index");
    /* Validate key index */
    if (valid_range(wp, key_index, &wlv_basic_variables[WLV_SEC_WEP_KEY_INDEX])
            == FALSE) {
        return FALSE;
    }

    index = key_index[0] - '0';
    for (i = 1; i <= 4; i++) {
        snprintf(key_name, TMP_LEN, "key%d", i);
        key = _get_virtual_web_variable(wp, if_idx, vif_idx, key_name);
        /* 
         * There are two cases required to be checked:
         * - the indexed key
         * - non-zero-length keys other than the indexed key
         */
        if ((index == i) || strlen(key)) {
            if (valid_base64(wp, key , 
                        &wlv_basic_variables[WLV_SEC_WEP_KEY1 + index - 1])
                    == FALSE){
                return FALSE;
            }
        }
    }
    return TRUE;
}

static int 
_valid_wlv_sec_wpa(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{
    char *crypto, *radius_key, *radius_ipaddr, *radius_port, 
         *rekey_mode, *rekey_interval;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);
    
    crypto = _get_virtual_web_variable(wp, if_idx, vif_idx, "crypto");
    /* Validate encryption method */
    if (valid_choice(wp, crypto, &wlv_basic_variables[WLV_SEC_PSK_CRYPTO])
            == FALSE) {
        return FALSE;
    }

    /* Validate radius encryption key */
    radius_key = _get_virtual_web_variable(wp, if_idx, vif_idx, "radius_key");
    if (valid_base64(wp, radius_key, 
                                    &wlv_basic_variables[WLV_SEC_WPA_RADIUS_KEY])
            == FALSE) {
        return FALSE;
    }

    /* Validate radius server ip address */
    radius_ipaddr = _get_virtual_web_variable(wp, if_idx, vif_idx, "radius_ipaddr");
    if (valid_ipaddr(wp, radius_ipaddr, 
                &wlv_basic_variables[WLV_SEC_WPA_RADIUS_IPADDR]) == FALSE) {
        return FALSE;
    }

    /* Validate radius server port */
    radius_port = _get_virtual_web_variable(wp, if_idx, vif_idx, "radius_port");
    if (valid_range(wp, radius_port, 
                &wlv_basic_variables[WLV_SEC_WPA_RADIUS_PORT]) == FALSE) {
        return FALSE;
    }

    /* Validate rekey method */
    rekey_mode = _get_virtual_web_variable(wp, if_idx, vif_idx, "rekey_mode");
    if (valid_choice(wp, rekey_mode, 
                &wlv_basic_variables[WLV_SEC_WPA_RADIUS_REKEY_METHOD]) == FALSE) {
        return FALSE;
    }

    /* Validate rekey interval */
    if (!strcmp(rekey_mode, "time")) {
        rekey_interval =
            _get_virtual_web_variable(wp, if_idx, vif_idx, "rekey_time_interval");
        if (valid_range(wp, rekey_interval, 
                &wlv_basic_variables[WLV_SEC_WPA_RADIUS_REKEY_TIME_INTERVAL])
                == FALSE) {
            return FALSE;
        }
    } else if (!strcmp(rekey_mode, "pkt")) {
        rekey_interval = 
            _get_virtual_web_variable(wp, if_idx, vif_idx, "rekey_pkt_interval");
        if (valid_range(wp, rekey_interval, 
                &wlv_basic_variables[WLV_SEC_WPA_RADIUS_REKEY_PKT_INTERVAL])
                == FALSE) {
            return FALSE;
        }
    }

    return TRUE;
}

static int 
_valid_wlv_sec_psk(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{
    char *key, *crypto;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);
    
    /* Validate authentication method */
    crypto = _get_virtual_web_variable(wp, if_idx, vif_idx, "crypto");
    if (valid_choice(wp, crypto, &wlv_basic_variables[WLV_SEC_PSK_CRYPTO]) 
            == FALSE) {
        return FALSE;
    }

    key = _get_virtual_web_variable(wp, if_idx, vif_idx, "key");
    if (valid_base64(wp, key, 
                &wlv_basic_variables[WLV_SEC_PSK_KEY]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int 
_valid_wlv_sec_psk2(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{
    char *preauth;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);

    if (_valid_wlv_sec_psk(wp, ifidx, vifidx, v) == FALSE) {
        return FALSE;
    }
    preauth = _get_virtual_web_variable(wp, if_idx, vif_idx, "preauth");
    if (valid_choice(wp, preauth, &wlv_basic_variables[WLV_SEC_PSK_PREAUTH]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

static int 
_valid_wlv_sec_wpa2(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{
    char *preauth;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);

    if (_valid_wlv_sec_wpa(wp, ifidx, vifidx, v) == FALSE) {
        return FALSE;
    }
    preauth = _get_virtual_web_variable(wp, if_idx, vif_idx, "preauth");
    if (valid_choice(wp, preauth, 
                    &wlv_basic_variables[WLV_SEC_WPA_PREAUTH]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

static int 
_valid_wlv_basic(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{
    char *ssid_enable, *ssid, *ssid_hide, *wme_enable, *secmode, *rate, *isolation, *guest_lan_enable, *guest_lan_ipaddr, *guest_lan_mask;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);
    char tmp[TMP_LEN];

    ssid_enable = _get_virtual_web_variable(wp, if_idx, vif_idx, "enable");
    if (valid_choice(wp, ssid_enable, &wlv_basic_variables[WLV_SSID_ENABLE]) 
            == FALSE) {
       return FALSE;
    }

    if (!strcmp(ssid_enable, "1")) {
        rate = _get_virtual_web_variable(wp, if_idx, vif_idx, "txrate");

        ssid_hide = _get_virtual_web_variable(wp, if_idx, vif_idx, "hide");
        if (valid_choice(wp, ssid_hide, &wlv_basic_variables[WLV_HIDE_SSID]) 
                == FALSE) {
            return FALSE;
        }

        ssid = _get_virtual_web_variable(wp, if_idx, vif_idx, "ssid");
        if (valid_name(wp, ssid, &wlv_basic_variables[WLV_SSID]) == FALSE) {
            return FALSE;
        }

        wme_enable = _get_virtual_web_variable(wp, if_idx, vif_idx, 
                "wme_enable");
        if (valid_choice(wp, wme_enable, &wlv_basic_variables[WLV_WME])
                == FALSE) {
            return FALSE;
        }
        
        isolation = _get_virtual_web_variable(wp, if_idx, vif_idx, "isolation_enable");

        if (valid_choice(wp, isolation, &wlv_basic_variables[WLV_ISOLATION])
                == FALSE) {
            return FALSE;
        }

        secmode = _get_virtual_web_variable(wp, if_idx, vif_idx, "secmode");
        if (valid_choice(wp, secmode , &wlv_basic_variables[WLV_SECMODE])
                == FALSE) {
            return FALSE;
        }
        if (!strcmp(secmode, "wep")) {
            if (_valid_wlv_sec_wep(wp, ifidx, vifidx , v) == FALSE) {
                return FALSE;
            }
        } else if (!strcmp(secmode,"psk")) {
            if (_valid_wlv_sec_psk(wp, ifidx, vifidx , v) == FALSE) {
                return FALSE;
            }
        } else if (!strcmp(secmode,"wpa")) {
            if (_valid_wlv_sec_wpa(wp, ifidx, vifidx, v) == FALSE) {
                return FALSE;
            }
        } else if (!strcmp(secmode,"psk2")) {
            if (_valid_wlv_sec_psk2(wp, ifidx, vifidx, v) == FALSE) {
                return FALSE;
            }
        } else if (!strcmp(secmode,"wpa2")) {
            if (_valid_wlv_sec_wpa2(wp, ifidx, vifidx, v) == FALSE) {
                return FALSE;
            }
        }
        if (appmgr_check("guest_lan")) {
          ezplib_get_rule("wlv_rule_num", 0, tmp, TMP_LEN);
          if (vifidx == tmp - 1) {
            guest_lan_enable = websGetVar(wp, "guest_lan_enable", "");
            if (valid_choice(wp, guest_lan_enable, 
                        &guest_lan_variables[GUEST_LAN_ENABLE]) == FALSE) {
                return FALSE;
            }
            if (guest_lan_enable) {
                guest_lan_ipaddr = websGetVar(wp, "guest_lan_ipaddr", "");
                if (valid_choice(wp, guest_lan_ipaddr, 
                            &guest_lan_variables[GUEST_LAN_IPADDR]) == FALSE) {
                    return FALSE;
                }
                guest_lan_mask = websGetVar(wp, "guest_lan_mask", "");
                if (valid_choice(wp, guest_lan_mask, 
                            &guest_lan_variables[GUEST_LAN_MASK]) == FALSE) {
                    return FALSE;
                }
            }
          }
        }
    } 

    return TRUE;
}

int
_valid_ssid_enable(webs_t wp, char *value, struct variable *v)
{
    int i;
    int nth = atoi(value);
    int ssid_num = atoi(nvram_safe_get("wlv_rule_num"));
    int disabled = FALSE;
    char *ssid_enable;

    for (i = 0; i < ssid_num; i++) {
        ssid_enable = _get_virtual_web_variable(wp, nth, i, "enable");
        if (*ssid_enable == '1') {
            if (disabled == TRUE) {
                websDebugWrite(wp, 
                               "Invalid SSID %d: the former SSID has been "
                               "disabled.<br>",
                               v->longname, value);
                return FALSE;
            }
        } else {
           disabled = TRUE;
        } 
    }
    return TRUE;
}

int
valid_wl_basic(webs_t wp, char *value, struct variable *v)
{
    int wl_num, idx, wlv_num, vidx;
    char idx_tmp[SHORT_BUF_LEN], vidx_tmp[SHORT_BUF_LEN];
    char tmp[SHORT_BUF_LEN];

    wl_num = atoi(nvram_safe_get("wl_rule_num"));
    for (idx = 0; idx < wl_num; idx++) {
        snprintf(idx_tmp, sizeof(idx_tmp), "%d", idx);

        if (_valid_wl_basic(wp, idx_tmp, v) == FALSE) {
            return FALSE;
        }

        /* 
         * Ensure the later SSID won't be enabled if the former is disabled.
         */
        if (_valid_ssid_enable(wp, idx_tmp, v) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, TMP_LEN , "wlv_rule_num");
        wlv_num = atoi(nvram_safe_get(tmp));
        for (vidx = 0; vidx < wlv_num; vidx++) {
            snprintf(vidx_tmp, sizeof(vidx_tmp), "%d", vidx);
            if (_valid_wlv_basic(wp, idx_tmp, vidx_tmp, v) == FALSE) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

enum WlWDSEnum {
    WDS_MODE,
    WDS_TIMEOUT,
    WDS_HWADDR,
    WDS_SEC_MODE,
    WDS_SEC_WEP_KEY,
    WDS_SEC_PSK_CRYPTO,
    WDS_SEC_PSK_KEY,
};

static struct variable wl_wds_variables[] = {
    {longname: "WDS Mode", argv:ARGV("disabled", "bridge", "repeater", "lazy")},
    {longname: "WDS Timeout", argv:ARGV("")},
    {longname: "WDS MAC Address", argv:ARGV("")},
    {longname: "WDS Security Mode", argv:ARGV("disabled", "wep", "psk")},
    {longname: "WDS WEP Key", argv:ARGV("")},
    {longname: "WDS WPAPSK Auth Method", argv:ARGV("tkip", "aes", "mixed")},
    {longname: "WDS WPAPSK Key", argv:ARGV("")},
};

static int
_valid_wl_wds(webs_t wp, char *value, int if_idx, struct variable *v)
{
    char *wds_mode, *wds_timeout;

    wds_mode = _get_web_variable(wp, if_idx, "wds_mode");
    wds_timeout = _get_web_variable(wp, if_idx, "wds_timeout");

    if (valid_choice(wp, wds_mode, &wl_wds_variables[WDS_MODE]) == FALSE) {
        return FALSE;
    }

    /* EZP: We don't honor wds_timeout now.
    if (valid_number(wp, wds_timeout, &wl_wds_variables[WDS_TIMEOUT]) 
            == FALSE) {
        return FALSE;
    }
    */
    wl_log_string("wds basic");
    return TRUE;
}

static int
_valid_wl_wds_sec_wep(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{
    char *key;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);

    key = _get_wds_web_variable(wp, if_idx, vif_idx, "key");
    /* Special WEP config for WDS. No WEP index is required. */
    if (valid_base64(wp, key, &wl_wds_variables[WDS_SEC_WEP_KEY]) 
            == FALSE) {
        return FALSE;
    }

    return TRUE;
}

static int
_valid_wl_wds_sec_psk(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{
    char *key, *crypto;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);
    
    /* We still use the term "key" in the WDS PSK case because 
     * wlv_sec_wpa_rule contains the WPAPSK and WPA modes at the same time. */
    key = _get_wds_web_variable(wp, if_idx, vif_idx, "key");
    crypto = _get_wds_web_variable(wp, if_idx, vif_idx, "crypto");

    if (valid_choice(wp, crypto, &wl_wds_variables[WDS_SEC_PSK_CRYPTO])
            == FALSE) {
        return FALSE;
    }

    if (valid_base64(wp, key, &wl_wds_variables[WDS_SEC_PSK_KEY])
            == FALSE) {
        return FALSE;
    }
    
    return TRUE;
}

static int
_valid_wlv_wds(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{

    char *hwaddr, *secmode;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);

    hwaddr = _get_wds_web_variable(wp, if_idx, vif_idx, "hwaddr");
    if (strlen(hwaddr) == 0) {
        return TRUE;
    }
    wl_log_string("valid hwaddr");
    if (valid_hwaddr(wp, hwaddr, &wl_wds_variables[WDS_HWADDR]) == FALSE) {
        return FALSE;
    }

    secmode = _get_wds_web_variable(wp, if_idx, vif_idx, "secmode");
    if (valid_choice(wp, secmode, &wl_wds_variables[WDS_SEC_MODE]) == FALSE) {
        return FALSE;
    }

    if (!strcmp(secmode,"wep")) {
        if (_valid_wl_wds_sec_wep(wp, ifidx, vifidx, 
                                  &wl_wds_variables[WDS_SEC_WEP_KEY]) 
                == FALSE) {
            return FALSE;
        }
    } else if (!strcmp(secmode,"psk")) {
        if (_valid_wl_wds_sec_psk(wp, ifidx, vifidx,
                                  &wl_wds_variables[WDS_SEC_PSK_CRYPTO]) 
                == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

int
valid_wl_wds(webs_t wp, char *value, struct variable *v)
{
    int wl_num, idx, wds_num, wdsidx;
    char temp[SHORT_BUF_LEN], ifidx[SHORT_BUF_LEN], vifidx[SHORT_BUF_LEN];
    wl_num = atoi(nvram_safe_get("wl_rule_num"));
    for (idx = 0; idx < wl_num; idx++) {
        if (_valid_wl_wds(wp, value, idx, v) == FALSE) {
            return FALSE;
        }

        snprintf(ifidx, SHORT_BUF_LEN, "%d", idx);

        /* TODO: wlv_wds_rule_num instead */
        snprintf(temp, TMP_LEN , "wl%d_wds_rule_num" , idx);
        wds_num = atoi(nvram_safe_get(temp));
        for (wdsidx = 0; wdsidx < wds_num; wdsidx++) {
            snprintf(vifidx, SHORT_BUF_LEN, "%d", wdsidx);

            if (_valid_wlv_wds(wp, ifidx, vifidx, v) == FALSE) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

enum WlAPCLIEnum {
    APCLI_ENABLE,
    APCLI_SSID,
    APCLI_BSSID,
    APCLI_EXTCHANNEL,
    APCLI_SEC_MODE,
    APCLI_SEC_WEP_KEY_INDEX,
    APCLI_SEC_WEP_KEY1,
    APCLI_SEC_WEP_KEY2,
    APCLI_SEC_WEP_KEY3,
    APCLI_SEC_WEP_KEY4,
    APCLI_SEC_PSK_CRYPTO,
    APCLI_SEC_PSK_KEY,
};

static struct variable wl_apcli_variables[] = {
    {longname: "URepeater Enable/Disable", argv:ARGV("0", "1")},
    {longname: "URepeater SSID", argv:ARGV("32")},
    {longname: "URepeater BSSID", argv:ARGV("")},
    {longname: "URepeater Extention Channel", argv:ARGV("0", "1")},
    {longname: "URepeater Security Mode", argv:ARGV("disabled", "wep", 
            "psk", "psk2")},
    {longname: "URepater WEP Key Index", argv:ARGV("1", "4")},
    {longname: "URepater Key 1", argv:ARGV("")},
    {longname: "URepater Key 2", argv:ARGV("")},
    {longname: "URepater Key 3", argv:ARGV("")},
    {longname: "URepater Key 4", argv:ARGV("")},
    {longname: "URepeater WPAPSK Auth Method", argv:ARGV("tkip","aes","mixed")},
    {longname: "URepeater WPAPSK Key", argv:ARGV("")},
};

static int
_valid_wl_apcli_sec_wep(webs_t wp, char *ifidx, char *vifidx,
        struct variable *v)
{
    char *key_index, *key, iKeyNameBuf[TMP_LEN];
    int iKeyIndex, i;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);

    key_index = _get_apcli_web_variable(wp, if_idx, vif_idx, "key_index");
    /* Validate key index */
    if (valid_range(wp, key_index, &wl_apcli_variables[APCLI_SEC_WEP_KEY_INDEX])
            == FALSE) {
        return FALSE;
    }

    /* Validate wep keys. */

    iKeyIndex = key_index[0] - '0';

    for (i = 1; i <= 4; i++) {
        snprintf(iKeyNameBuf, TMP_LEN, "key%d", i);
        key = _get_apcli_web_variable(wp, if_idx, vif_idx, iKeyNameBuf);
        /* 
         * There are two cases required to be checked:
         * - the indexed key
         * - non-zero-length keys other than the indexed key
         */
        if ((iKeyIndex == i) || strlen(key)) {
            if (valid_base64(wp, key , &wl_apcli_variables[APCLI_SEC_WEP_KEY1 + iKeyIndex - 1]) == FALSE){
                return FALSE;
            }
        }
    }
    return TRUE;

}

static int
_valid_wl_apcli_sec_psk_or_psk2(webs_t wp, char *ifidx, char *vifidx, 
        struct variable *v)
{
    char *key, *crypto;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);
    
    crypto = _get_apcli_web_variable(wp, if_idx, vif_idx, "crypto");
    if (valid_choice(wp, crypto, &wl_apcli_variables[APCLI_SEC_PSK_CRYPTO]) 
            == FALSE) {
        return FALSE;
    }

    key = _get_apcli_web_variable(wp, if_idx, vif_idx, "key");
    if (valid_base64(wp, key, &wl_apcli_variables[APCLI_SEC_PSK_KEY])
            == FALSE) {
        return FALSE;
    }
    
    return TRUE;
}

static int
_valid_wlv_apcli(webs_t wp, char *ifidx, char *vifidx, struct variable *v)
{

    char *enable, *ssid, *bssid, *secmode, *extcha;
    int if_idx = atoi(ifidx);
    int vif_idx = atoi(vifidx);
    char tmp[TMP_LEN];

    /* if use WISP */
    enable = websGetVar(wp, "wan0_enable", "");
    if(strlen(enable) == 0) {
        /* Universal Repeater */
        enable = _get_apcli_web_variable(wp, if_idx, vif_idx, "enable");
    }
    if (valid_choice(wp, enable, 
                &wl_apcli_variables[APCLI_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* If hardware NAT is enabled, we can not start Universal Repeater. */
    ezplib_get_attr_val("nat_rule", 0, "hw_enable", tmp,
                            sizeof(tmp), EZPLIB_USE_CLI);
    if ((*enable == '1') && !strcmp(tmp, "1")) {
        websDebugWrite(wp, "%s.<br> %s.<br>",
                lang_translate("Start Universal Repeater failed"),
                lang_translate("Need to turn off Hardware Accelerator first"));
        return FALSE;
    }

    if (!strcmp(enable, "1")) {
        ssid = _get_apcli_web_variable(wp, if_idx, vif_idx, "ssid");
        if (valid_name(wp, ssid, &wl_apcli_variables[APCLI_SSID]) == FALSE) {
            return FALSE;
        }

        bssid = _get_apcli_web_variable(wp, if_idx, vif_idx, "bssid");
        if (strlen(bssid)) {
            if (valid_hwaddr(wp, bssid, 
                        &wl_apcli_variables[APCLI_BSSID]) == FALSE) {
                return FALSE;
            }
        }

        extcha = _get_web_variable(wp, if_idx, "extcha");
        if (strlen(extcha)) {
            if (valid_choice(wp, extcha, 
                        &wl_apcli_variables[APCLI_EXTCHANNEL]) == FALSE) {
                return FALSE;
            }
        }

        secmode = _get_apcli_web_variable(wp, if_idx, vif_idx, "secmode");
        if (valid_choice(wp, secmode, &wl_apcli_variables[APCLI_SEC_MODE]) 
                == FALSE) {
            return FALSE;
        }

        if (!strcmp(secmode,"wep")) {
            if (_valid_wl_apcli_sec_wep(wp, ifidx, vifidx, 
                        &wl_apcli_variables[APCLI_SEC_WEP_KEY_INDEX])
                    == FALSE) {
                return FALSE;
            }
        } else if (!strcmp(secmode, "psk") || !strcmp(secmode, "psk2")) {
            if (_valid_wl_apcli_sec_psk_or_psk2(wp, ifidx, vifidx, 
                        &wl_apcli_variables[APCLI_SEC_PSK_CRYPTO]) == FALSE) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

int
valid_wl_apcli(webs_t wp, char *value, struct variable *v)
{
    int wl_num, idx, apcli_num, apcli_idx;
    char tmp[TMP_LEN], ifidx[SHORT_BUF_LEN], vifidx[SHORT_BUF_LEN];
    wl_num = atoi(nvram_safe_get("wl_rule_num"));
    for (idx = 0; idx < wl_num; idx++) {
        snprintf(ifidx, SHORT_BUF_LEN, "%d", idx);
        /* XXX: Validate the interface-wise apcli.
        if (_valid_wl_apcli(wp, value, idx, v) == FALSE) {
            return FALSE;
        }*/
        snprintf(tmp, TMP_LEN , "wl%d_apcli_rule_num" , idx);
        apcli_num = atoi(nvram_safe_get(tmp));
        for (apcli_idx = 0;apcli_idx < apcli_num;apcli_idx++) {
            snprintf(vifidx, SHORT_BUF_LEN, "%d", apcli_idx);
            if (_valid_wlv_apcli(wp, ifidx, vifidx, v) == FALSE) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/* 
 * The function family of saving variables.
 * Return 1 if there is a changed; return 0 if none.
 */
int
save_wl_adv(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;
    char tmp[TMP_LEN];
    char *region;
    int wl_num = atoi(nvram_safe_get("wl_rule_num"));
    int i, change = 0;

    region = websGetVar(wp, "wl_region", "");
    if (strcmp(region, nvram_safe_get("wl_region"))) {
        config_preaction(&map, v, s, "NUM=1", "");
        nvram_set("wl_region", region);
        change = 1; 
    }

    /* develop each rule set */
    for (i = 0; i < wl_num; i++) {
        map = 0;
        snprintf(tmp, sizeof(tmp), "%d", i);
        if (_save_wl_adv(wp, tmp, v, s, &map)) {
            change = 1;
        }

        if (change) {
            snprintf(tmp, TMP_LEN, "NUM=%d", i);
            config_postaction(map, s, tmp, "");
        }
    }

    return change;
}

int
save_wl_basic(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;
    char temp[TMP_LEN];
    int wl_num, i, change = 0, wlv_num, j;

    /* get wl_num */
    wl_num = atoi(nvram_safe_get("wl_rule_num"));

    /* develop each rule set */
    for (i = 0;i < wl_num;i++) {
        map = 0;
        snprintf(temp, sizeof(temp), "%d", i);
        if (_save_wl_basic(wp, temp, v, s, &map)) {
            change = 1;
        }

        /* get wl_num */
        wlv_num = atoi(nvram_safe_get("wlv_rule_num"));
        for (j = 0;j < wlv_num;j++) {
            if (_save_wlv_basic(wp, i, j, v, s, &map)) {
                change = 1;
            }
        }

        if (change) {
            snprintf(temp, TMP_LEN, "NUM=%d", i);
            config_postaction(map, s, temp, "");
        }
    }

    return change;
}

int
save_wl_wds(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;
    char temp[TMP_LEN];
    int wl_num, i, change = 0, wds_num, j;
    /* get wl_num */
    wl_num = atoi(nvram_safe_get("wl_rule_num"));

    /* develop each rule set */
    for (i = 0; i < wl_num; i++) {
        map = 0;
        snprintf(temp, sizeof(temp), "%d", i);
        if (_save_wl_wds(wp, temp, v, s, &map)) {
            change = 1;
        }

        /* TODO: wlv_wds_rule_num instead */
        snprintf(temp, TMP_LEN , "wl%d_wds_rule_num" , i);
        wds_num = atoi(nvram_safe_get(temp));
        for (j = 0;j < wds_num;j++) {
            if (_save_wlv_wds(wp, i, j, v, s, &map)) {
                change = 1;
            }
        }

        if (change) {
            snprintf(temp, TMP_LEN, "NUM=%d", i);
            config_postaction(map, s, temp, "");
        }

    }

    return change;
}

int
save_wl_apcli(webs_t wp, char *value, struct variable *v, struct service *s) {
    int64_t map = 0;
    char temp[TMP_LEN];
    int wl_num, i, change = 0, apcli_num, j;
    /* get wl_num */
    wl_num = atoi(nvram_safe_get("wl_rule_num"));

    /* develop each rule set */
    for (i = 0;i < wl_num;i++) {
        map = 0;
        /* XXX: Save the interface-wise apcli.
        snprintf(temp, sizeof(temp), "%d", i);
        if (_save_wl_apcli(wp, temp, v, s, &map)) {
            change = 1;
        }*/

        /* TODO: wlv_apcli_rule_num instead*/
        snprintf(temp, TMP_LEN , "wl%d_apcli_rule_num" , i);
        apcli_num = atoi(nvram_safe_get(temp));
        for (j = 0;j < apcli_num;j++) {
            if (_save_wlv_apcli(wp, i, j, v, s, &map)) {
                change = 1;
            }
        }

        if (change) {
            snprintf(temp, TMP_LEN, "NUM=%d", i);
            config_postaction(map, s, temp, "");
        }

    }

    return change;
}

int
ej_wl_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *file;
    char buf[TMP_LEN];
    int num = atoi(nvram_safe_get("wl_rule_num"));
    int i;

    /* Get physical interface index */
    if (ejArgs(argc, argv, "%s", &file) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return FALSE;
    }

    for (i = 0; i < num; i++) {
        snprintf(buf, TMP_LEN, "%d", i);
        do_ej(file, wp, buf, "");
    }

    return 0;
}

int
ej_wlv_show_entry_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    int i, num;
    char *ifidx, *file, buf_vif[TMP_LEN];

    /* Get physical interface index and entry page*/
    if (ejArgs(argc, argv, "%s %s", &ifidx, &file) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return FALSE;
    }

    /* Get number of virtual interfaces. */
    num = atoi(nvram_safe_get("wlv_rule_num"));

    for (i = 0; i < num; i++) {
        snprintf(buf_vif, TMP_LEN, "%d", i);
        /* Handle each ssid interface. */
        do_ej(file, wp, ifidx, buf_vif, "");
    }

    return 0;
}

int
ej_wlv_sec_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char buf_sec[TMP_LEN], buf_if[TMP_LEN], buf_vif[TMP_LEN];
    int if_idx, vif_idx, num;
    char *secmode;

    /* Get physical interface index */
    if (ejArgs(argc, argv, "%d %d", &if_idx, &vif_idx) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return FALSE;
    }
    num = atoi(nvram_safe_get("wlv_rule_num"));

    /* Get security mode rule */
    snprintf(buf_if, TMP_LEN, "wl%d_ssid%d_secmode" , if_idx, vif_idx);
    secmode = websGetVar(wp, buf_if, "");
    if (!*secmode) {
        snprintf(buf_if, TMP_LEN, "wl%d_sec_rule", if_idx);
        ezplib_get_attr_val(buf_if, vif_idx, "secmode", buf_sec, 
                    TMP_LEN, EZPLIB_USE_CLI); 
    } else{
        sprintf(buf_sec, "%s", secmode);
    }
    /*----------------
    buf_sec : security mode
    buf_if : interface index
    buf_vif : ssid index
    -----------------*/
    snprintf(buf_if, TMP_LEN, "%d", if_idx);
    snprintf(buf_vif, TMP_LEN, "%d", vif_idx);
    if (!strcmp(buf_sec, "wep")) {
        do_ej("wlv_sec_wep.asp", wp, buf_if, buf_vif, "");
    } else if (!strcmp(buf_sec, "psk")) {
        do_ej("wlv_sec_psk.asp", wp, buf_if, buf_vif, "");
    } else if (!strcmp(buf_sec, "wpa")) {
        do_ej("wlv_sec_wpa.asp", wp, buf_if, buf_vif, "");
    } else if (!strcmp(buf_sec, "psk2")) {
        do_ej("wlv_sec_psk2.asp", wp, buf_if, buf_vif, "");
    } else if (!strcmp(buf_sec, "wpa2")) {
        do_ej("wlv_sec_wpa2.asp", wp, buf_if, buf_vif, "");
    }
    if (appmgr_check("guest_lan") && ( vif_idx == num - 1 )) {
            do_ej("guest_lan_setting.asp", wp, buf_if, buf_vif, "");
    }
    return 0;
}

int
ej_wl_wds_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char buf_if[EZPLIB_BUF_LEN], buf_vif[EZPLIB_BUF_LEN];
    char *wds_num_p, *secmode = NULL;
    int if_ind, i, wds_num;

    if (ejArgs(argc, argv, "%d", &if_ind) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /*Get wds security mode */
    snprintf(buf_if, TMP_LEN, "wl%d_wds_mode" ,if_ind);
    secmode = websGetVar(wp, buf_if, "");
    if (!*secmode) {
        ezplib_get_attr_val("wl_wds_rule", if_ind, "mode", buf_if, 
                    TMP_LEN, EZPLIB_USE_CLI); 
    } else{
        sprintf(buf_if, "%s", secmode);
    }

    if (!strncmp(buf_if, "disabled", TMP_LEN)) {
        return 0;
    }
    
    snprintf(buf_if, TMP_LEN, "wl%d_wds_rule_num", if_ind);
    wds_num_p = nvram_safe_get(buf_if);
    wds_num = atoi(wds_num_p);

    /*-------------------------------------
    Construct interface index and multissid index.
    buf_if : interface index
    buf_vif : ssid index
    --------------------------------------*/
    snprintf(buf_if, TMP_LEN, "%d", if_ind);
    for (i = 0;i < wds_num; i++) {
        snprintf(buf_vif, TMP_LEN, "%d", i);
        do_ej("wlv_wds_basic.asp", wp, buf_if, buf_vif, "");
    }
    return 0;
}

int
ej_wlv_wds_sec_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char buf_attr[TMP_LEN], buf_if[TMP_LEN], buf_vif[TMP_LEN];
    int if_ind, vif_ind;
    char *secmode;

    /* Get physical interface index */
    if (ejArgs(argc, argv, "%d %d", &if_ind, &vif_ind) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return FALSE;
    }

    /*Get wds security mode */
    snprintf(buf_if, TMP_LEN, "wl%d_wds%d_secmode" , if_ind, vif_ind);
    secmode = websGetVar(wp, buf_if, "");
    if (!*secmode) {
        /* Not in CGI parameters. Looking for the value from nvram rules. */
        snprintf(buf_if, TMP_LEN, "wl%d_wds_basic_rule" , if_ind);
        ezplib_get_attr_val(buf_if, vif_ind, "secmode", buf_attr, 
                    TMP_LEN, EZPLIB_USE_CLI); 
    } else{
        sprintf(buf_attr,"%s",secmode);
    }
    /*----------------
    buf_attr : security mode
    buf_if : interface index
    buf_vif : ssid index
    -----------------*/
    snprintf(buf_if, TMP_LEN, "%d" ,if_ind);
    snprintf(buf_vif, TMP_LEN, "%d" ,vif_ind);

    if (!strncmp(buf_attr,"wep",TMP_LEN)) {
        do_ej("wlv_wds_sec_wep.asp", wp, buf_if, buf_vif, "");
    } else if (!strncmp(buf_attr,"psk",TMP_LEN)) {
        do_ej("wlv_wds_sec_psk.asp", wp, buf_if, buf_vif, "");
    }
    return 0;
}

int
ej_wl_apcli_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char buf_if[EZPLIB_BUF_LEN], buf_vif[EZPLIB_BUF_LEN];
    int if_ind, i, apcli_num;

    if (ejArgs(argc, argv, "%d", &if_ind) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(buf_if, TMP_LEN, "wl%d_apcli_rule_num", if_ind);
    apcli_num = atoi(nvram_safe_get(buf_if));

    /*-------------------------------------
    Construct interface index and multissid index.
    buf_if : interface index
    buf_vif : ssid index
    --------------------------------------*/
    snprintf(buf_if, TMP_LEN, "%d", if_ind);
    for (i = 0; i < apcli_num; i++) {
        snprintf(buf_vif, TMP_LEN, "%d", i);
        /* We use ap client both on wan and wifi.
         * In wifi, we want to see "wlan" and "Universal Repeater" */
        do_ej("wlv_apcli_basic.asp", wp, 
                buf_if, buf_vif, lang_translate("WLAN"), lang_translate("Universal Repeater"), "wl_apcli", "");
    }
    return 0;
}

int
ej_wlv_single_ssid_show_mark_begin(int eid, webs_t wp, int argc, char_t **argv)
{
    int ssid_num; 
    ssid_num = atoi(nvram_safe_get("wlv_rule_max"));
    if (ssid_num == 1) {
        websWrite(wp, "<!--\n");
    }
    return 0;
}

int
ej_wlv_single_ssid_show_mark_end(int eid, webs_t wp, int argc, char_t **argv)
{
    int ssid_num; 
    ssid_num = atoi(nvram_safe_get("wlv_rule_max"));
    if (ssid_num == 1) {
        websWrite(wp, "-->\n");
    }
    return 0;
}

int
ej_wlv_no_11n_show_mark_begin(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_EZP_ARCH_RTL865X
    websWrite(wp, "<!--\n");
#endif
    return 0;
}

int
ej_wlv_no_11n_show_mark_end(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_EZP_ARCH_RTL865X
    websWrite(wp, "-->\n");
#endif
    return 0;
}

int
ej_wlv_apcli_sec_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char buf_attr[TMP_LEN], buf_if[TMP_LEN], buf_vif[TMP_LEN];
    int if_ind, vif_ind;
    char *secmode;

    /* Get physical interface index */
    if (ejArgs(argc, argv, "%d %d", &if_ind, &vif_ind) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return FALSE;
    }

    /*Get apcli security mode */
    snprintf(buf_if, TMP_LEN, "wl%d_apcli%d_secmode" , if_ind, vif_ind);
    secmode = websGetVar(wp, buf_if, "");
    if (!*secmode) {
        snprintf(buf_if, TMP_LEN, "wl%d_apcli_rule" , if_ind);
        ezplib_get_attr_val(buf_if, vif_ind, "secmode", buf_attr, 
                    TMP_LEN, EZPLIB_USE_CLI); 
    } else {
        sprintf(buf_attr, "%s", secmode);
    }
    /*----------------
    buf_attr : security mode
    buf_if : interface index
    buf_vif : ssid index
    -----------------*/
    snprintf(buf_if, TMP_LEN, "%d" ,if_ind);
    snprintf(buf_vif, TMP_LEN, "%d" ,vif_ind);

    if (!strncmp(buf_attr, "wep", TMP_LEN)) {
        websWrite(wp, "<input type=\"hidden\" name=\"wl%s_apcli%s_crypto\" value=\"\">", buf_if, buf_vif);
        do_ej("wlv_apcli_sec_wep.asp", wp, buf_if, buf_vif, "");
    } else if (!strncmp(buf_attr,"psk",TMP_LEN)) {
        do_ej("wlv_apcli_sec_psk.asp", wp, buf_if, buf_vif, "");
    } else if (!strncmp(buf_attr,"psk2",TMP_LEN)) {
        do_ej("wlv_apcli_sec_psk2.asp", wp, buf_if, buf_vif, "");
    } else {
        websWrite(wp, "<input type=\"hidden\" name=\"wl%s_apcli%s_crypto\" value=\"\">", buf_if, buf_vif);
    }
    return 0;
}

char *
_get_web_variable(webs_t wp, int nth, char *item)
{
    char buf[TMP_LEN];
    snprintf(buf, TMP_LEN, "wl%d_%s", nth, item);
    return websGetVar(wp, buf, "");
}

static char *
_get_virtual_web_variable(webs_t wp, int if_idx, int vif_idx, char *item)
{
    char buf[TMP_LEN];
    snprintf(buf, TMP_LEN, "wl%d_ssid%d_%s", if_idx, vif_idx, item);
    return websGetVar(wp, buf, "");
}

static char *
_get_wds_web_variable(webs_t wp, int if_idx, int vif_idx, char *item)
{
    char buf[TMP_LEN];
    snprintf(buf, TMP_LEN, "wl%d_wds%d_%s", if_idx, vif_idx, item);
    return websGetVar(wp, buf, "");
}

char *
_get_apcli_web_variable(webs_t wp, int if_idx, int vif_idx, char *item) {
    char buf[TMP_LEN];
    snprintf(buf, TMP_LEN, "wl%d_apcli%d_%s", if_idx, vif_idx, item);
    return websGetVar(wp, buf, "");
}

static int
_save_wl_rule(char *rule_set, int nth, char *new, struct variable *v,
        struct service *s, int64_t *map)
{
    int change = 0;
    char old[TMP_LEN];

    ezplib_get_rule(rule_set, nth, old, TMP_LEN);

    if (strcmp(new, old)) {
        snprintf(old, TMP_LEN, "NUM=%d", nth);
        config_preaction(map, v, s, old, "");
        ezplib_replace_rule(rule_set, nth, new);
        change = 1;
    }
    return change;
}

static int 
_save_wl_adv(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    /*
    infra^ap_mode^rateset^mrate^frag^rts^dtim^bcn^plcphdr^
    antdiv^gmode^gmode_protection^afterburner^frameburst
    */
    char new[TMP_LEN];
    char *infra, *ap_mode, *rateset, *mrate, *frag, *rts, *dtim, *bcn,
         *plcphdr, *antdiv, *gmode, *gmode_protection, *afterburner, 
         *frameburst, *txburst, *pktaggr, *txmode, *rdg, *linkadapt,
         *htc, *htbw, *gi, *opmode, *mcs, *mpdu, *amsdu,
         *autoba, *bawinsize, *badecline, *extcha, *wdstxmode;
    int idx = atoi(value), len, change = 0;

    infra = _get_web_variable(wp, idx, "infra");
    ap_mode = _get_web_variable(wp, idx, "ap_mode");
    rateset = _get_web_variable(wp, idx, "rateset");
    mrate = _get_web_variable(wp, idx, "mrate");
    frag = _get_web_variable(wp, idx, "frag");
    rts = _get_web_variable(wp, idx, "rts");
    dtim = _get_web_variable(wp, idx, "dtim");
    bcn = _get_web_variable(wp, idx, "bcn");
    plcphdr = _get_web_variable(wp, idx, "plcphdr");
    antdiv = _get_web_variable(wp, idx, "antdiv");
    gmode = _get_web_variable(wp, idx, "gmode");
    gmode_protection = _get_web_variable(wp, idx, "gmode_protection");
    afterburner = _get_web_variable(wp, idx, "afterburner");
    frameburst = _get_web_variable(wp, idx, "frameburst");
    txburst = _get_web_variable(wp, idx, "txburst");
    pktaggr = _get_web_variable(wp, idx, "pktaggr");
    txmode = _get_web_variable(wp, idx, "txmode");
    rdg = _get_web_variable(wp, idx, "rdg");
    linkadapt = _get_web_variable(wp, idx, "linkadapt");
    htc = _get_web_variable(wp, idx, "htc");
    htbw = _get_web_variable(wp, idx, "htbw");
    gi = _get_web_variable(wp, idx, "gi");
    opmode = _get_web_variable(wp, idx, "opmode");
    mcs = _get_web_variable(wp, idx, "mcs");
    mpdu = _get_web_variable(wp, idx, "mpdu");
    amsdu = _get_web_variable(wp, idx, "amsdu");
    autoba = _get_web_variable(wp, idx, "autoba");
    bawinsize = _get_web_variable(wp, idx, "bawinsize");
    badecline = _get_web_variable(wp, idx, "badecline");
    extcha = _get_web_variable(wp, idx, "extcha");
    wdstxmode = _get_web_variable(wp, idx, "wdstxmode");
    /* Empty value for Wins attribute. */
    len = 
        snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
                    infra, ap_mode, rateset, mrate, frag, rts, dtim, bcn,
                    plcphdr, antdiv, gmode, gmode_protection, afterburner,
                    frameburst, txburst, pktaggr, txmode, rdg, linkadapt,
                    htc, htbw, gi, opmode, mcs, mpdu, amsdu,
                    autoba, bawinsize, badecline, extcha, wdstxmode);

    if (len > TMP_LEN - 1) {
        return FALSE;
    }

    change = _save_wl_rule("wl_advanced_rule", idx, new, v, s, map);
    return change;
}

static int 
_save_wl_basic(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    /* enable^net_mode^txpower^channel */
    char *secmode, *ssid_enable, tmp[TMP_LEN], tmp_sec_radius=0;
    char new[TMP_LEN];
    char *radio_enable, *txpower, *channel, *bisolation, *net_mode;
    int idx = atoi(value), len, change = 0;
    /* Get security mode rule */
#if ( WL_VIRTUAL_NUM >= 1 ) /* WL0_IFNAME_RULE*/
    ssid_enable = websGetVar(wp, "wl0_ssid0_enable", "");
    if ( !strcmp(ssid_enable, "1")) {
        secmode = websGetVar(wp, "wl0_ssid0_secmode", "");
        snprintf(tmp, sizeof(tmp), secmode);
        tmp_sec_radius|=!strcmp(tmp, "wpa");
        tmp_sec_radius|=!strcmp(tmp, "wpa2");
    }
#endif /* WL0_IFNAME_RULE*/
#if ( WL_VIRTUAL_NUM >= 2 )
    ssid_enable = websGetVar(wp, "wl0_ssid1_enable", "");
    if ( !strcmp(ssid_enable, "1")) {
        secmode = websGetVar(wp, "wl0_ssid1_secmode", "");
        snprintf(tmp, sizeof(tmp), secmode);
        tmp_sec_radius|=!strcmp(tmp, "wpa");
        tmp_sec_radius|=!strcmp(tmp, "wpa2");
    }
#endif /* WL0_IFNAME_RULE*/
#if ( WL_VIRTUAL_NUM >= 4 )
    ssid_enable = websGetVar(wp, "wl0_ssid2_enable", "");
    if ( !strcmp(ssid_enable, "1")) {
        secmode = websGetVar(wp, "wl0_ssid2_secmode", "");
        snprintf(tmp, sizeof(tmp), secmode);
        tmp_sec_radius|=!strcmp(tmp, "wpa");
        tmp_sec_radius|=!strcmp(tmp, "wpa2");
    }
    ssid_enable = websGetVar(wp, "wl0_ssid3_enable", "");
    if ( !strcmp(ssid_enable, "1")) {
        secmode = websGetVar(wp, "wl0_ssid1_secmode", "");
        snprintf(tmp, sizeof(tmp), secmode);
        tmp_sec_radius|=!strcmp(tmp, "wpa");
        tmp_sec_radius|=!strcmp(tmp, "wpa2");
    }
#endif /* WL0_IFNAME_RULE*/
    if (tmp_sec_radius) ezplib_replace_attr("wl_wps_rule", 0, "display", "0");
    else ezplib_replace_attr("wl_wps_rule", 0, "display", "1");
    net_mode = _get_web_variable(wp, idx, "net_mode");
    /* radio_enable is used */
    radio_enable = _get_web_variable(wp, idx, "radio_enable");
    if (strcmp(radio_enable, "1")) ezplib_replace_attr("wl_wps_rule", 0, "display", "0");
    txpower = _get_web_variable(wp, idx, "txpower");
    channel = _get_web_variable(wp, idx, "channel");
    bisolation = _get_web_variable(wp, idx, "bisolation_enable");
    if (!*bisolation) {
        /* Single SSID platform. */
        bisolation = "0";
    }
    /* Empty value for Wins attribute. */

    len = 
        snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s", radio_enable, net_mode, 
                txpower, channel, bisolation);

    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule("wl_basic_rule", idx, new, v, s, map))
        change = 1;
    return change;
}

static int 
_save_wl_wds(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char new[TMP_LEN];
    char *wds_mode, *wds_timeout;
    int idx = atoi(value), len, change = 0;

    wds_mode = _get_web_variable(wp, idx, "wds_mode");
    wds_timeout = _get_web_variable(wp, idx, "wds_timeout");

    /* Empty value for Wins attribute. */

    len = snprintf(new, TMP_LEN, "%s^%s", wds_mode, wds_timeout);

    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule("wl_wds_rule", idx, new, v, s, map)) {
        change = 1;
    }

    /* When WDS is enabled, lan0_ifnames should be changed! */
    if (change) {
        if (!strcmp(wds_mode, "disabled")) {
            nvram_set("lan0_ifnames", LAN0_IFNAMES);
        } else {
            nvram_set("lan0_ifnames", LAN0_IFNAMES_WDS);
        }
    }

    return change;
}

static int
_save_wl_sec_wep(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map)
{
    /* key_index^key1^key2^key3^key4 */    
    char new[TMP_LEN], old[TMP_LEN];
    char *key_index, *key1, *key2, *key3, *key4, *encmode, *keytype;
    int change = 0, len;
    
    key_index = _get_virtual_web_variable(wp, if_idx, vif_idx, "key_index");
    key1 = _get_virtual_web_variable(wp, if_idx, vif_idx, "key1");
    key2 = _get_virtual_web_variable(wp, if_idx, vif_idx, "key2");
    key3 = _get_virtual_web_variable(wp, if_idx, vif_idx, "key3");
    key4 = _get_virtual_web_variable(wp, if_idx, vif_idx, "key4");
    keytype = _get_virtual_web_variable(wp, if_idx, vif_idx, "keytype");
    encmode = _get_virtual_web_variable(wp, if_idx, vif_idx, "encmode");

    len = 
        snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s", key_index, key1, key2, 
                 key3, key4, keytype, encmode);
    snprintf(old, TMP_LEN, "wl%d_sec_wep_rule",if_idx);
    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;
    return change;
}

static int
_save_wl_wds_sec_wep(webs_t wp, int if_idx, int vif_idx, struct variable *v, 
        struct service *s, int64_t *map)
{
    /* key */    
    char new[TMP_LEN], old[TMP_LEN];
    char *key;
    int change = 0, len;
    
    key = _get_wds_web_variable(wp, if_idx, vif_idx, "key");

    len = snprintf(new, TMP_LEN, "%s", key);
    snprintf(old, TMP_LEN, "wl%d_wds_sec_wep_rule", if_idx);
    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;
    return change;
}

int
_save_wl_apcli_sec_wep(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map)
{
    /* key_index^key1^key2^key3^key4 */    
    char new[TMP_LEN], old[TMP_LEN];
    char *key_index, *key1, *key2, *key3, *key4;
    int change = 0, len;
    
    key_index = _get_apcli_web_variable(wp, if_idx, vif_idx, "key_index");
    key1 = _get_apcli_web_variable(wp, if_idx, vif_idx, "key1");
    key2 = _get_apcli_web_variable(wp, if_idx, vif_idx, "key2");
    key3 = _get_apcli_web_variable(wp, if_idx, vif_idx, "key3");
    key4 = _get_apcli_web_variable(wp, if_idx, vif_idx, "key4");

    len = 
        snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s", key_index,
            key1, key2, key3, key4);
    snprintf(old, TMP_LEN, "wl%d_apcli_sec_wep_rule", if_idx);
    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;
    return change;
}

static int
_save_wl_sec_wpa_wpa2_or_psk_psk2(webs_t wp, int if_idx, int vif_idx,
        int wpa2, struct variable *v, struct service *s, int64_t *map)
{
    /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^rekey_time_interval^rekey_pkt_interval */
    /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^rekey_time_interval^rekey_pkt_interval^preauth */
    char new[TMP_LEN], old[TMP_LEN], time_tmp[TMP_LEN], pkt_tmp[TMP_LEN],
         tmp[TMP_LEN];
    char *key, *crypto, *radius_key, *radius_ipaddr, *radius_port, *rekey_mode,
         *rekey_time_interval, *rekey_pkt_interval, *preauth, 
         *pmkperiod, *session_timeout, *wpacap;
    int change = 0, len;
    
    key = _get_virtual_web_variable(wp, if_idx, vif_idx, "key");
    crypto = _get_virtual_web_variable(wp, if_idx, vif_idx, "crypto");
    radius_key = _get_virtual_web_variable(wp, if_idx, vif_idx, "radius_key");
    radius_port = _get_virtual_web_variable(wp, if_idx, vif_idx, "radius_port");
    radius_ipaddr = _get_virtual_web_variable(wp, if_idx, vif_idx,
                                              "radius_ipaddr");
    rekey_mode = _get_virtual_web_variable(wp, if_idx, vif_idx, "rekey_mode");

    snprintf(tmp, TMP_LEN, "wl%d_sec_wpa2_rule", if_idx);

    /* Keep the interval values in nvram even if secmode is set to be 
     * disabled and the component values are absent.
     */
    rekey_time_interval = _get_virtual_web_variable(wp, if_idx, vif_idx,
                                                    "rekey_time_interval");
    if (!*rekey_time_interval) {
        ezplib_get_attr_val(tmp, vif_idx, "rekey_time_interval", time_tmp,
                            TMP_LEN, EZPLIB_USE_CLI);
    } else {
        rekey_time_interval =
            _get_virtual_web_variable(wp, if_idx, vif_idx,
                                      "rekey_time_interval");
        strcpy(time_tmp, rekey_time_interval);
    }

    rekey_pkt_interval = _get_virtual_web_variable(wp, if_idx, vif_idx,
                                                   "rekey_pkt_interval");
    if (!*rekey_pkt_interval) {
        ezplib_get_attr_val(tmp, vif_idx, "rekey_pkt_interval", pkt_tmp,
                            TMP_LEN, EZPLIB_USE_CLI);
    } else {
        rekey_pkt_interval =
            _get_virtual_web_variable(wp, if_idx, vif_idx,
                                      "rekey_pkt_interval");
        strcpy(pkt_tmp, rekey_pkt_interval);
    }

    session_timeout = _get_virtual_web_variable(wp, if_idx, vif_idx, "session_timeout");

    if (wpa2) {
        preauth = _get_virtual_web_variable(wp, if_idx, vif_idx, "preauth");
        pmkperiod = _get_virtual_web_variable(wp, if_idx, vif_idx, "pmkperiod");
        wpacap = _get_virtual_web_variable(wp, if_idx, vif_idx, "wpacap");
        printf("if:%d vif:%d pmk:%s wpacap:%s\n", if_idx, vif_idx, pmkperiod, wpacap);
        len = 
            snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s", key, crypto, 
                     radius_key, radius_ipaddr, radius_port, rekey_mode,
                     time_tmp, pkt_tmp, preauth, pmkperiod, session_timeout, wpacap);
        snprintf(old, TMP_LEN, "wl%d_sec_wpa2_rule", if_idx);
    } else {
        len = 
            snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s", key, crypto,
                     radius_key, radius_ipaddr, radius_port, rekey_mode,
                     time_tmp, pkt_tmp, session_timeout);
        snprintf(old, TMP_LEN, "wl%d_sec_wpa_rule",if_idx);
    }

    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;
    return change;
}

/* TODO: wpa2 unused */
static int
_save_wl_wds_sec_psk(webs_t wp, int if_idx, int vif_idx, int wpa2,
        struct variable *v, struct service *s, int64_t *map)
{
    /* key^crypto */
    char new[TMP_LEN], old[TMP_LEN];
    char *key, *crypto;
    int change = 0, len;
    
    key = _get_wds_web_variable(wp, if_idx, vif_idx, "key");
    crypto = _get_wds_web_variable(wp, if_idx, vif_idx, "crypto");
    len = snprintf(new, TMP_LEN, "%s^%s", key, crypto);
    snprintf(old, TMP_LEN, "wl%d_wds_sec_wpa_rule",if_idx);

    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;
    return change;
}

int
_save_wl_apcli_sec_psk_or_psk2(webs_t wp, int if_idx, int vif_idx, int wpa2,
        struct variable *v, struct service *s, int64_t *map)
{
    /* key^crypto */
    char new[TMP_LEN], old[TMP_LEN];
    char *key, *crypto;
    int change = 0, len;
    
    key = _get_apcli_web_variable(wp, if_idx, vif_idx, "key");
    crypto = _get_apcli_web_variable(wp, if_idx, vif_idx, "crypto");
    len = snprintf(new, TMP_LEN, "%s^%s", key, crypto);
    if (wpa2) {
        snprintf(old, TMP_LEN, "wl%d_apcli_sec_wpa2_rule",if_idx);
    } else {
        snprintf(old, TMP_LEN, "wl%d_apcli_sec_wpa_rule",if_idx);
    }

    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;
    return change;
}

static int 
_save_wlv_basic(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map)
{
    char new[TMP_LEN], old[TMP_LEN];
    char *ssid_enable, *ssid, *ssid_hide, *wme_enable, *secmode, *rate, *isolation, *guest_lan_enable, *guest_lan_ipaddr, *guest_lan_mask, *radio_enable;
    int len, change = 0;
    /* Save basic visual interface rule */
    ssid_enable = _get_virtual_web_variable(wp, if_idx, vif_idx, "enable");
    radio_enable = _get_web_variable(wp, if_idx, "radio_enable");
    if (vif_idx == 0) {
        if (!strcmp(ssid_enable,"1") && !strcmp(radio_enable,"1")) ezplib_replace_attr("wl_wps_rule", 0, "display", "1");
        else ezplib_replace_attr("wl_wps_rule", 0, "display", "0");
    }
    ssid = _get_virtual_web_variable(wp, if_idx, vif_idx, "ssid");
    ssid_hide = _get_virtual_web_variable(wp, if_idx, vif_idx, "hide");
    rate = _get_virtual_web_variable(wp, if_idx, vif_idx, "txrate");
    wme_enable = _get_virtual_web_variable(wp, if_idx, vif_idx, "wme_enable");
    isolation = _get_virtual_web_variable(wp, if_idx, vif_idx, "isolation_enable");
    secmode = _get_virtual_web_variable(wp, if_idx, vif_idx, "secmode");
    if (appmgr_check("guest_lan")) {
      guest_lan_enable = websGetVar(wp, "guest_lan_enable", "");
      if (ezplib_check_attr_changed("guest_lan_rule", 
                  0, "enable", guest_lan_enable)) {
          ezplib_replace_attr("guest_lan_rule", 0, "enable", guest_lan_enable);
          *map |= ACTION_WIRELESS_START;
          change = 1;
      }    
      if (guest_lan_enable) {
          guest_lan_ipaddr = websGetVar(wp, "guest_lan_ipaddr", "");
          if (ezplib_check_attr_changed("guest_lan_rule",
                      0, "ipaddr", guest_lan_ipaddr)) {
              ezplib_replace_attr("guest_lan_rule", 0, "ipaddr",
                      guest_lan_ipaddr);
              *map |= ACTION_WIRELESS_START;
              change = 1;
          }    
          guest_lan_mask = websGetVar(wp, "guest_lan_mask", "");
          if (ezplib_check_attr_changed("guest_lan_rule",
                      0, "mask", guest_lan_mask)) {
              ezplib_replace_attr("guest_lan_rule", 0, "mask",
                      guest_lan_mask);
              *map |= ACTION_WIRELESS_START;
              change = 1;
          }    
      }
    }

    /* Save virtual interface basic rule */
    len = 
        snprintf(new, TMP_LEN, "%s^%s^%s^%s^%s", ssid_enable, ssid_hide, rate,
                 wme_enable, isolation);
    snprintf(old, TMP_LEN, "wl%d_basic_rule",if_idx);
    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;

    /* Save virtual interface ssid rule */
    len = snprintf(new, TMP_LEN, "%s", ssid);
    snprintf(old, TMP_LEN, "wl%d_ssid_rule",if_idx);
    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;

    /* Save virtual interface security rule */
    len = snprintf(new, TMP_LEN, "%s", secmode);
    snprintf(old, TMP_LEN, "wl%d_sec_rule",if_idx);
    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;

    /* Save virtual interface security mode rule */
    if (!strcmp(secmode,"wep")) {
        if (_save_wl_sec_wep(wp, if_idx, vif_idx, v, s, map))
            change = 1;
    } else if ((!strcmp(secmode,"psk")) || (!strcmp(secmode,"wpa"))) {
        if (_save_wl_sec_wpa_wpa2_or_psk_psk2(wp, if_idx, vif_idx, 0, v, s,
                                              map))
            change = 1;
    }
    else if ((!strcmp(secmode,"psk2")) || (!strcmp(secmode,"wpa2"))) {
        if (_save_wl_sec_wpa_wpa2_or_psk_psk2(wp, if_idx, vif_idx, 1, v, s,
                                              map))
            change = 1;
    }
    /* Save security rule*/
    
    return change;
}

static int 
_save_wlv_apcli(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map)
{
    char new[TMP_LEN], old[TMP_LEN];
    char *enable, *ssid, *bssid, *secmode, *channel, *extcha;
    int len, change = 0;
    /* Save basic visual interface rule */
    enable = _get_apcli_web_variable(wp, if_idx, vif_idx, "enable");
    ssid = _get_apcli_web_variable(wp, if_idx, vif_idx, "ssid");
    bssid = _get_apcli_web_variable(wp, if_idx, vif_idx, "bssid");
    secmode = _get_apcli_web_variable(wp, if_idx, vif_idx, "secmode");
    /* Save apcli rule */
    /* EZP: In normal universal mode, apcli is applied to lan,
     * the wifiwan is disabled.*/
    len = snprintf(new, TMP_LEN, "%s^%s^%s^%s^0",
            enable, ssid, bssid, secmode);
    snprintf(old, TMP_LEN, "wl%d_apcli_rule",if_idx);
    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;
    /* EZP: apcli uses same channel with wifi card. */
    channel = _get_web_variable(wp, if_idx, "channel");
    if (ezplib_check_attr_changed("wl_basic_rule",
                if_idx, "channel", channel)) {
        ezplib_replace_attr("wl_basic_rule", if_idx, "channel", channel);
        change = 1;
    }    
    /* extention channel setting */
    extcha = _get_web_variable(wp, if_idx, "extcha");
    if (ezplib_check_attr_changed("wl_advanced_rule",
                if_idx, "extcha", extcha)) {
        ezplib_replace_attr("wl_advanced_rule", if_idx, "extcha", extcha);
        change = 1;
    }    
    /* Save WDS interface security mode rule */
    if (!strcmp(secmode,"wep")) {
        if (_save_wl_apcli_sec_wep(wp, if_idx, vif_idx, v, s, map))
            change = 1;
    } else if (!strcmp(secmode,"psk")) {
        if (_save_wl_apcli_sec_psk_or_psk2(wp, if_idx, vif_idx, 0, v, s, map))
            change = 1;
    } else if (!strcmp(secmode,"psk2")) {
        if (_save_wl_apcli_sec_psk_or_psk2(wp, if_idx, vif_idx, 1, v, s, map))
            change = 1;
    }
    
    return change;
}

static int 
_save_wlv_wds(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map)
{
    char new[TMP_LEN], old[TMP_LEN];
    char *hwaddr, *secmode;
    int len, change = 0;
    /* Save basic visual interface rule */
    hwaddr = _get_wds_web_variable(wp, if_idx, vif_idx, "hwaddr");
    secmode = _get_wds_web_variable(wp, if_idx, vif_idx, "secmode");
    /* Save WDS interface basic rule */
    len = snprintf(new, TMP_LEN, "%s^%s", hwaddr, secmode);
    snprintf(old, TMP_LEN, "wl%d_wds_basic_rule",if_idx);
    if (len > TMP_LEN - 1) {
        return FALSE;
    }
    if (_save_wl_rule(old, vif_idx, new, v, s, map))
        change = 1;

    /* Save WDS interface security mode rule */
    if (!strcmp(secmode,"wep")) {
        if (_save_wl_wds_sec_wep(wp, if_idx, vif_idx, v, s, map))
            change = 1;
    } else if (!strcmp(secmode,"psk")) {
        if (_save_wl_wds_sec_psk(wp, if_idx, vif_idx, 0, v, s, map))
            change = 1;
    }
    
    return change;
}

static void 
wl_log_string(char *aLogStr_p)
{
    FILE * fp = fopen("/tmp/wl_log.txt","a");
    int iLen = strlen(aLogStr_p);
    fwrite(aLogStr_p, sizeof(char), iLen, fp);
    fwrite("\n", sizeof(char), strlen("\n"), fp);
    fclose(fp);
}

int
ej_apcli_fieldset_need(int eid, webs_t wp, int argc, char_t **argv)
{
    char *type, *fieldset;
    if (ejArgs(argc, argv, "%s %s", &type, &fieldset) < 2) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }
    if(!strcmp("none", fieldset)) {
        if(!strcmp("start", type)) {
            websWrite(wp, "<!--");
        } else if(!strcmp("end", type)) {
            websWrite(wp, "-->");
        }
    }
    return 0;
}
// Base on BRCM V3.51.21.10
/* UI Mode		GMODE			Afterburner Override	Basic Rate Set	FrameBurst	CTS Protection
 * Mixed		6 - AfterBurner		-1			Default		ON		-1(auto)
			(wl_net_mode=mixed, wl_gmode=6, wl_afterburner_override=-1, wl_rateset=default, wl_frameburst=on, wl_gmode_protection=auto)
 * 54g-Only		6 - AfterBurner		-1			ALL		ON		0(off)
			(wl_net_mode=g-only, wl_gmode=6, wl_afterburner_override=-1, wl_rateset=all, wl_frameburst=on, wl_gmode_protection=off)
 * 11b-Only		0 - 54g Legacy B	NA			Default		ON		-1(auto)
			(wl_net_mode=b-only, wl_gmode=0, wl_rateset=default, wl_frameburst=on, wl_gmode_protection=auto)
 */

// Base on BRCM V3.61.13.0
/* UI Mode		GMODE			Afterburner Override	Basic Rate Set	FrameBurst	CTS Protection
 * Mixed		1 - AfterBurner		-1			Default		ON		0(off)
			(wl_net_mode=mixed, wl_gmode=1, wl_afterburner=auto, wl_rateset=default, wl_frameburst=on, wl_gmode_protection=off)
 * 54g-Only		1 - AfterBurner		-1			ALL		ON		0(off)
			(wl_net_mode=g-only, wl_gmode=1, wl_afterburner=auto, wl_rateset=all, wl_frameburst=on, wl_gmode_protection=off)
 * 11b-Only		0 - 54g Legacy B	 0			Default		ON		0(off)
			(wl_net_mode=b-only, wl_gmode=0, wl_afterburner=off, wl_rateset=default, wl_frameburst=on, wl_gmode_protection=off)
 * (GMODE=6 is no longer suppored)
 */
