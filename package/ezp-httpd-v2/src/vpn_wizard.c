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

enum {
    WIZARD_VPN_TYPE = 0,
	WIZARD_VPN_PPTP_NAME ,
	WIZARD_VPN_PPTP_PW,
	WIZARD_VPN_IPSEC_MODE,
	WIZARD_VPN_IPSEC_LOCAL_EXT_IFACE,
	WIZARD_VPN_IPSEC_LOCAL_IP,
	WIZARD_VPN_IPSEC_LOCAL_MASK,
	WIZARD_VPN_IPSEC_REMOTE_GW,
	WIZARD_VPN_IPSEC_REMOTE_SUB_IP,
	WIZARD_VPN_IPSEC_REMOTE_MASK,
	WIZARD_VPN_IPSEC_PKEY,
	WIZARD_VPN_IPSEC_CINT,
	WIZARD_VPN_IPSEC_ADV,
	WIZARD_VPN_IPSEC_P1_LEFT_ID,
	WIZARD_VPN_IPSEC_P1_RIGHT_ID,
	WIZARD_VPN_IPSEC_P1_AUTH,
	WIZARD_VPN_IPSEC_P1_ENC,
	WIZARD_VPN_IPSEC_P1_GKEY,
	WIZARD_VPN_IPSEC_P2_AUTH,
	WIZARD_VPN_IPSEC_P2_ENC,
	WIZARD_VPN_IPSEC_P2_GKEY
};

static struct variable wizard_variables[] = {
    {longname: "Wizard VPN Type", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Wizard PPTP Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard PPTP Password", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard IPSEC Mode", argv:ARGV("net2net", "rw_server"), nullok: FALSE},
    {longname: "Wizard IPSEC External Interface", argv:ARGV("wan0", "wan1"), nullok: FALSE},
    {longname: "Wizard IPSEC Local IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard IPSEC Local Netmask", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard IPSEC Remote Gateway", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard IPSEC Remote Subnet IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard IPSEC Remote Netmask", argv:ARGV(""), nullok: FALSE},
    {longname: "Wizard IPSEC Preshare Key", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard IPSEC Connection Initiation", argv:ARGV("on", ""), nullok: TRUE},
    {longname: "Wizard IPSEC Advance Option", argv:ARGV("on", ""), nullok: TRUE},
    {longname: "Wizard IPSEC Phase 1 Left ID", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard IPSEC Phase 1 Right ID", argv:ARGV("20"), nullok: FALSE},
    {longname: "Wizard IPSEC Phase 1 Authentication", argv:ARGV("md5", "sha1"), nullok: FALSE},
    {longname: "Wizard IPSEC Phase 1 Encryption", argv:ARGV("des", "3des", "aes128"), nullok: FALSE},
    {longname: "Wizard IPSEC Phase 1 Group Key Management", argv:ARGV("dh1", "dh2", "dh5"), nullok: FALSE},
    {longname: "Wizard IPSEC Phase 2 Authentication", argv:ARGV("md5", "sha1"), nullok: FALSE},
    {longname: "Wizard IPSEC Phase 2 Encryption", argv:ARGV("des", "3des", "aes128"), nullok: FALSE},
    {longname: "Wizard IPSEC Phase 2 Group Key Management", argv:ARGV("none", "dh1", "dh2", "dh5"), nullok: FALSE}
};

int
_valid_vpn_adv(webs_t wp, char *type)
{
    char tmp[TMP_LEN];    
    char *val;
    /* Phase 1 Left ID */
    snprintf(tmp, sizeof(tmp), "%sp1_left_id", type);
    val = websGetVar(wp, tmp, "0");
    if (valid_name(wp, val,
                &wizard_variables[WIZARD_VPN_IPSEC_P1_LEFT_ID]) == FALSE) {
        return FALSE;
    } 
    /* Phase 1 Right ID */
    snprintf(tmp, sizeof(tmp), "%sp1_right_id", type);
    val = websGetVar(wp, tmp, "0");
    if (valid_name(wp, val,
                &wizard_variables[WIZARD_VPN_IPSEC_P1_RIGHT_ID]) == FALSE) {
        return FALSE;
    } 
    /* Phase 1 Authentication */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp1_auth", type);
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val,
                &wizard_variables[WIZARD_VPN_IPSEC_P1_AUTH]) == FALSE) {
        return FALSE;
    } 
    /* Phase 1 Encryption */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp1_ept", type);
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val,
                &wizard_variables[WIZARD_VPN_IPSEC_P1_ENC]) == FALSE) {
        return FALSE;
    } 
    /* Phase 1 Group Key Management */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp1_gkey", type);
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val,
                &wizard_variables[WIZARD_VPN_IPSEC_P1_GKEY]) == FALSE) {
        return FALSE;
    } 
    /* Phase 2 Authentication */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp2_auth", type);
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val,
                &wizard_variables[WIZARD_VPN_IPSEC_P2_AUTH]) == FALSE) {
        return FALSE;
    } 
    /* Phase 2 Encryption */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp2_ept", type);
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val,
                &wizard_variables[WIZARD_VPN_IPSEC_P2_ENC]) == FALSE) {
        return FALSE;
    } 
    /* Phase 2 Group Key Management */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp2_gkey", type);
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val,
                &wizard_variables[WIZARD_VPN_IPSEC_P2_GKEY]) == FALSE) {
        return FALSE;
    } 
    return TRUE;
}

int
valid_vpn_wizard(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* VPN Type */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_type");
    val = websGetVar(wp, tmp, "0");
    if (valid_choice(wp, val, &wizard_variables[WIZARD_VPN_TYPE]) == FALSE) {
        return FALSE;
    } 

    if (*val == '0') {
        /* If hardware NAT is enabled, we can not start PPTPD. */
        ezplib_get_attr_val("nat_rule", 0, "hw_enable", tmp,
                sizeof(tmp), EZPLIB_USE_CLI);
        if (!strcmp(tmp, "1")) {
            websDebugWrite(wp, "%s.<br> %s.<br>",
                    lang_translate("Start VPN/PPTP failed"),
                    lang_translate("Need to turn off Hardware Accelerator first"));
            return FALSE;
        }
        /* PPTP User Name */
        snprintf(tmp, sizeof(tmp), "pptp_user");
        val = websGetVar(wp, tmp, "0");
        if (valid_name(wp, val,
                    &wizard_variables[WIZARD_VPN_PPTP_NAME]) == FALSE) {
            return FALSE;
        } 
        /* PPTP Password */
        snprintf(tmp, sizeof(tmp), "pptp_pw");
        val = websGetVar(wp, tmp, "0");
        if (valid_name(wp, val,
                    &wizard_variables[WIZARD_VPN_PPTP_PW]) == FALSE) {
            return FALSE;
        } 
    } else {
        /* IPSEC Mode */
        snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_mode");
        val = websGetVar(wp, tmp, "0");
        if (valid_choice(wp, val,
                    &wizard_variables[WIZARD_VPN_IPSEC_MODE]) == FALSE) {
            return FALSE;
        } 
        if (!strcmp(val, "net2net")) {
            /* Net to Net */
            /* External Interface */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_nif");
            val = websGetVar(wp, tmp, "0");
            if (valid_choice(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_LOCAL_EXT_IFACE]) == FALSE) {
                return FALSE;
            } 
            /* Local IP Address */
            snprintf(tmp, sizeof(tmp), "nlocal_ip");
            val = websGetVar(wp, tmp, "0");
            if (valid_ipaddr(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_LOCAL_IP]) == FALSE) {
                return FALSE;
            } 
            /* Local Netmask */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_nlocal_mask");
            val = websGetVar(wp, tmp, "0");
            if (valid_netmask(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_LOCAL_MASK]) == FALSE) {
                return FALSE;
            } 
            /* Remote Gateway */
            snprintf(tmp, sizeof(tmp), "nremote_gw");
            val = websGetVar(wp, tmp, "0");
            if (valid_ipaddr(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_REMOTE_GW]) == FALSE) {
                return FALSE;
            } 
            /* Remote Subnet IP Address */
            snprintf(tmp, sizeof(tmp), "nremote_ip");
            val = websGetVar(wp, tmp, "0");
            if (valid_ipaddr(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_REMOTE_SUB_IP]) == FALSE) {
                return FALSE;
            } 
            /* Remote Netmask */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_nremote_mask");
            val = websGetVar(wp, tmp, "0");
            if (valid_netmask(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_REMOTE_MASK]) == FALSE) {
                return FALSE;
            } 
            /* Preshare Key */
            snprintf(tmp, sizeof(tmp), "npkey");
            val = websGetVar(wp, tmp, "0");
            if (valid_name(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_PKEY]) == FALSE) {
                return FALSE;
            } 
            /* Connection Initiation */
            snprintf(tmp, sizeof(tmp), "con_init");
            val = websGetVar(wp, tmp, "0");
            if (valid_choice(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_CINT]) == FALSE) {
                return FALSE;
            } 
            /* Advance Option */
            snprintf(tmp, sizeof(tmp), "nadv_opt");
            val = websGetVar(wp, tmp, "0");
            if (valid_choice(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_ADV]) == FALSE) {
                return FALSE;
            } 
            if (!strcmp(val, "on")) {
              if(!_valid_vpn_adv(wp, "n")) {
                  return FALSE;
              };
            }
        } else {
            /* Road Warror */
            /* External Interface */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_rif");
            val = websGetVar(wp, tmp, "0");
            if (valid_choice(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_LOCAL_EXT_IFACE]) == FALSE) {
                return FALSE;
            } 
            /* Local IP Address */
            snprintf(tmp, sizeof(tmp), "rlocal_ip");
            val = websGetVar(wp, tmp, "0");
            if (valid_ipaddr(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_LOCAL_IP]) == FALSE) {
                return FALSE;
            } 
            /* Local Netmask */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_rlocal_mask");
            val = websGetVar(wp, tmp, "0");
            if (valid_netmask(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_LOCAL_MASK]) == FALSE) {
                return FALSE;
            } 
            /* Preshare Key */
            snprintf(tmp, sizeof(tmp), "rpkey");
            val = websGetVar(wp, tmp, "0");
            if (valid_name(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_PKEY]) == FALSE) {
                return FALSE;
            } 
            /* Advance Option */
            snprintf(tmp, sizeof(tmp), "radv_opt");
            val = websGetVar(wp, tmp, "0");
            if (valid_choice(wp, val,
                        &wizard_variables[WIZARD_VPN_IPSEC_ADV]) == FALSE) {
                return FALSE;
            } 
            if (!strcmp(val, "on")) {
              if(!_valid_vpn_adv(wp, "r")) {
                  return FALSE;
              };
            }
        }
    }
    return TRUE;
}
static int
_save_vpn_adv(webs_t wp, char *type, char *adv_rule, int len)
{
    char *phase1_auth, *phase1_encrypt, *phase1_group, *phase1_left_id, * phase1_right_id;
    char *phase2_auth, *phase2_encrypt, *phase2_group;
    char tmp[TMP_LEN];

    /* Phase 1 Left ID */
    snprintf(tmp, sizeof(tmp), "%sp1_left_id", type);
    phase1_left_id = websGetVar(wp, tmp, "0");
    /* Phase 1 Right ID */
    snprintf(tmp, sizeof(tmp), "%sp1_right_id", type);
    phase1_right_id = websGetVar(wp, tmp, "0");
    /* Phase 1 Authentication */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp1_auth", type);
    phase1_auth = websGetVar(wp, tmp, "0");
    /* Phase 1 Encryption */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp1_ept", type);
    phase1_encrypt = websGetVar(wp, tmp, "0");
    /* Phase 1 Group Key Management */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp1_gkey", type);
    phase1_group = websGetVar(wp, tmp, "0");
    /* Phase 2 Authentication */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp2_auth", type);
    phase2_auth = websGetVar(wp, tmp, "0");
    /* Phase 2 Encryption */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp2_ept", type);
    phase2_encrypt = websGetVar(wp, tmp, "0");
    /* Phase 2 Group Key Management */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_%sp2_gkey", type);
    phase2_group = websGetVar(wp, tmp, "0");
    snprintf(adv_rule, len, "main^%s^%s^3600^28800^%s^%s^%s^%s^%s^%s",
            phase1_left_id, phase1_right_id, phase1_auth, phase1_encrypt, 
            phase1_group, phase2_auth, phase2_encrypt, phase2_group);
    return 0;
}

int
save_vpn_wizard(webs_t wp, char *value, struct variable *v, struct service *s)
{
    /* variale from user settting, get from web */
    char *name, *pw, *type, *mode, *val;
    /* variable user can not setting, get from nvram rule */
    char tmp[TMP_LEN], rule_set[TMP_LEN], rule[TMP_LEN];
    char rule_set_num[TMP_LEN], old_rule[LONG_BUF_LEN];

    int num, change = 0;
    int64_t map = 0;

    /* VPN Type */
    snprintf(tmp, sizeof(tmp), "wizard_vpn_type");
    type = websGetVar(wp, tmp, "0");

    if (*type == '0') {
        /* PPTP User Name */
        snprintf(tmp, sizeof(tmp), "pptp_user");
        name = websGetVar(wp, tmp, "0");
        /* PPTP Password */
        snprintf(tmp, sizeof(tmp), "pptp_pw");
        pw = websGetVar(wp, tmp, "0");
        /* construct pptp user rule */
        snprintf(rule_set, sizeof(rule_set), "pptpd_user_rule");
        snprintf(rule_set_num, sizeof(rule_set_num), "pptpd_user_rule_num");
        num = atoi(nvram_safe_get("pptpd_user_rule_num")) + 1;
        snprintf(rule, sizeof(rule), "1^%s^^%s^", name, pw);
    } else {
        char *local_extif, *local_inipaddr, *local_netmask, *remote_gateway;
        char *remote_inipaddr, *remote_netmask,*psk, *name;
        char conn_init[TMP_LEN], adv[TMP_LEN], adv_rule[TMP_LEN];
        /* IPSEC Mode */
        snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_mode");
        mode = websGetVar(wp, tmp, "0");
        if (!strcmp(mode, "net2net")) {
            /* Net to Net */
            /* Rule Name */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_name");
            name = websGetVar(wp, tmp, "0");
            /* External Interface */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_nif");
            local_extif = websGetVar(wp, tmp, "0");
            /* Local IP Address */
            snprintf(tmp, sizeof(tmp), "nlocal_ip");
            local_inipaddr = websGetVar(wp, tmp, "0");
            /* Local Netmask */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_nlocal_mask");
            local_netmask = websGetVar(wp, tmp, "0");
            /* Remote Gateway */
            snprintf(tmp, sizeof(tmp), "nremote_gw");
            remote_gateway = websGetVar(wp, tmp, "0");
            /* Remote Subnet IP Address */
            snprintf(tmp, sizeof(tmp), "nremote_ip");
            remote_inipaddr = websGetVar(wp, tmp, "0");
            /* Remote Netmask */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_nremote_mask");
            remote_netmask = websGetVar(wp, tmp, "0");
            /* Preshare Key */
            snprintf(tmp, sizeof(tmp), "npkey");
            psk = websGetVar(wp, tmp, "0");
            /* Connection Initiation */
            snprintf(tmp, sizeof(tmp), "con_init");
            val = websGetVar(wp, tmp, "0");
            if(!strcmp(val, "on")) {
                strcpy(conn_init, "1");
            } else {
                strcpy(conn_init, "0");
            }
            /* Advance Option */
            snprintf(tmp, sizeof(tmp), "nadv_opt");
            val = websGetVar(wp, tmp, "0");
            if (!strcmp(val, "on")) {
              strcpy(adv, "1");
              _save_vpn_adv(wp, "n", adv_rule, sizeof(adv_rule));
            } else {
              strcpy(adv, "0");
              snprintf(adv_rule, sizeof(adv_rule),
                      "main^^3600^28800^sha1^3des^dh2^sha1^3des^none");
            }
            snprintf(rule, sizeof(rule),
              "%s^1^%s^0^%s^%s^%s^%s^%s^%s^%s^psk^%s^undefined^undefined^%s^%s^0^10^60",
                    name, mode, local_extif, local_inipaddr,
                    local_netmask, remote_gateway, remote_inipaddr,
                    remote_netmask, conn_init, psk, adv, adv_rule);
        } else {
            /* Road Warror */
            /* Rule Name */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_name");
            name = websGetVar(wp, tmp, "0");
            /* External Interface */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_rif");
            local_extif = websGetVar(wp, tmp, "0");
            /* Local IP Address */
            snprintf(tmp, sizeof(tmp), "rlocal_ip");
            local_inipaddr = websGetVar(wp, tmp, "0");
            /* Local Netmask */
            snprintf(tmp, sizeof(tmp), "wizard_vpn_ipsec_rlocal_mask");
            local_netmask = websGetVar(wp, tmp, "0");
            /* Preshare Key */
            snprintf(tmp, sizeof(tmp), "rpkey");
            psk = websGetVar(wp, tmp, "0");
            /* Advance Option */
            snprintf(tmp, sizeof(tmp), "radv_opt");
            val = websGetVar(wp, tmp, "0");
            if (!strcmp(val, "on")) {
              strcpy(adv, "1");
              _save_vpn_adv(wp, "r", adv_rule, sizeof(adv_rule));
            } else {
              strcpy(adv, "0");
              snprintf(adv_rule, sizeof(adv_rule),
                      "main^^3600^28800^sha1^aes^dh2^sha1^aes^dh2");
            }
            snprintf(rule, sizeof(rule),
              "%s^1^%s^0^%s^%s^%s^^^^0^psk^%s^undefined^undefined^%s^%s^0^10^60",
                    name, mode, local_extif, local_inipaddr,
                    local_netmask, psk, adv, adv_rule);
        }
        /* construct ipsec rule */
        snprintf(rule_set, sizeof(rule_set), "ipsec_rule");
        snprintf(rule_set_num, sizeof(rule_set_num), "ipsec_rule_num");
        num = atoi(nvram_safe_get("ipsec_rule_num")) + 1;
    }

    /* service stop */
    config_preaction(&map, v, s, "NUM=0", "");

    /* config nvram */
    if (ezplib_get_rule(rule_set, num, old_rule, sizeof(old_rule)) < 0) {
        ezplib_append_rule(rule_set, rule);
    } else {
        ezplib_replace_rule(rule_set, num, rule);
    }
    snprintf(tmp, sizeof(tmp), "%d", num);
    nvram_fset(rule_set_num, tmp);
    if(*type == '0') {
        ezplib_replace_attr("pptpd_rule", 0, "enable", "1");
    } else if (*type == '1') {
        nvram_fset("ipsec_enable", "1");
    }
    change = 1;

    /* service start */
    snprintf(tmp, LONG_BUF_LEN, "NUM=0");
    config_postaction(map, s, tmp, "");

    return change;
}

int
ej_wizard_vpn_show_num(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *type;

    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if(!strcmp(type, "pptp")) {
        websWrite(wp, "%s", nvram_safe_get("pptpd_user_rule_num"));
    } else if (!strcmp(type, "ipsec")) {
        websWrite(wp, "%s", nvram_safe_get("ipsec_rule_num"));
    }
    return 0;
}

int
ej_wizard_vpn_show_max(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *type;
    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if(!strcmp(type, "pptp")) {
        websWrite(wp, "%s", nvram_safe_get("pptpd_user_rule_max"));
    } else if (!strcmp(type, "ipsec")) {
        websWrite(wp, "%s", nvram_safe_get("ipsec_rule_max"));
    }
    return 0;
}

int
ej_wizard_get_lan_mask(int eid, webs_t wp, int argc, char_t ** argv)
{
    char tmp[TMP_LEN], mask[TMP_LEN];
    ezplib_get_attr_val("lan_static_rule", 0, "mask", tmp,
            sizeof(tmp),EZPLIB_USE_CLI);
    if (trans_int2netmask(atoi(tmp), mask, sizeof(mask)) != -1) {
        websWrite(wp, "%s", mask);
    } else {
        websWrite(wp, "");
    }

    return 0;
}
