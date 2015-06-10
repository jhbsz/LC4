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

enum {
    IPSEC_ENABLE = 0,
    IPSEC_RULE_NAME,
    IPSEC_RULE_ENABLE,
    IPSEC_RULE_MODE,
    IPSEC_RULE_L2TP,
    IPSEC_RULE_LOCAL_EXTIF,
    IPSEC_RULE_LOCAL_INIPADDR,
    IPSEC_RULE_LOCAL_NETMASK,
    IPSEC_RULE_REMOTE_GATEWAY,
    IPSEC_RULE_REMOTE_INIPADDR,
    IPSEC_RULE_REMOTE_NETMASK,
    IPSEC_RULE_CONN_INIT,
    IPSEC_RULE_IKE_KEYMODE,
    IPSEC_RULE_PSK,
    IPSEC_RULE_RSA,
    IPSEC_RULE_CA,
    IPSEC_RULE_ADV,
    IPSEC_RULE_PHASE1_MODE,
    IPSEC_RULE_PHASE1_LEFT_ID,
    IPSEC_RULE_PHASE1_RIGHT_ID,
    IPSEC_RULE_PHASE1_LIFETIME,
    IPSEC_RULE_PHASE2_LIFETIME,
    IPSEC_RULE_PHASE1_AUTH,
    IPSEC_RULE_PHASE1_ENCRYPT,
    IPSEC_RULE_PHASE1_GROUP,
    IPSEC_RULE_PHASE2_AUTH,
    IPSEC_RULE_PHASE2_ENCRYPT,
    IPSEC_RULE_PHASE2_GROUP,
    IPSEC_RULE_DPD_ENABLE,
    IPSEC_RULE_DPD_INTERVAL,
    IPSEC_RULE_DPD_TIMEOUT,
    IPSEC_RULE_SPLIT_TUNNEL,
};

static struct variable ipsec_variables[] = {
    {longname: "IPsec Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "IPsec Rule Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "IPsec Rule Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "IPsec Rule Mode", 
            argv:ARGV("net2net", "rw_client", "rw_server"), nullok: FALSE},
    {longname: "IPsec Rule L2TP", argv:ARGV("0", "1"), nullok: TRUE},
    {longname: "IPsec Rule Local External Interface", argv:ARGV("wan"), 
            nullok: FALSE},
    {longname: "IPsec Rule Local Internal IP Address", argv:ARGV(""),
            nullok: TRUE},
    {longname: "IPsec Rule Local Netmask", argv:ARGV(""),
            nullok: TRUE},
    {longname: "IPsec Rule Remote Gateway", argv:ARGV("30"), 
            nullok: TRUE},
    {longname: "IPsec Rule Remote IP Address", argv:ARGV(""), 
            nullok: TRUE},
    {longname: "IPsec Rule Remote Netmask", argv:ARGV(""), 
            nullok: TRUE},
    {longname: "IPsec Rule Connection Initiation", argv:ARGV("0", "1"),
            nullok: TRUE},
    {longname: "IPsec Rule IKE Key Mode", argv:ARGV("psk", "rsa", "caauth"),
            nullok: FALSE},
    {longname: "IPsec Rule PSK Key", argv:ARGV("8","40"), nullok: TRUE},
    {longname: "IPsec Rule RSA Key", argv:ARGV("20","1024"), nullok: TRUE},
    {longname: "IPsec Rule CA", argv:ARGV(""), nullok: TRUE},
    {longname: "IPsec Rule Advanced", argv:ARGV("0", "1"), nullok: TRUE },
    {longname: "IPsec Rule Phase1 Mode", argv:ARGV("main", "aggressive"),
            nullok: TRUE},
    {longname: "IPsec Rule Phase1 Local ID", argv:ARGV(""), nullok: TRUE},
    {longname: "IPsec Rule Phase1 Remote ID", argv:ARGV(""), nullok: TRUE},
    {longname: "IPsec Rule Phase1 Lifetime", argv:ARGV("1200", "86400"), 
            nullok: TRUE},
    {longname: "IPsec Rule Phase2 Lifetime", argv:ARGV("1200", "86400"), 
            nullok: TRUE},
    {longname: "IPsec Rule Phase1 Authentication", argv:ARGV("md5", "sha1"),
            nullok: TRUE},
    {longname: "IPsec Rule Phase1 Encryption", argv:ARGV("des", "3des", "aes128"),
            nullok: TRUE},
    {longname: "IPsec Rule Phase1 Group", argv:ARGV("dh1", "dh2", "dh5"),
            nullok: TRUE},
    {longname: "IPsec Rule Phase2 Authentication", argv:ARGV("md5", "sha1"),
            nullok: TRUE},
    {longname: "IPsec Rule Phase2 Encryption", 
            argv:ARGV("null", "des", "3des", "aes128"), nullok: TRUE},
    {longname: "IPsec Rule Phase2 Group", argv:ARGV("none", "dh1", "dh2", "dh5"), nullok: TRUE},
    {longname: "IPsec Dead Peer Protection", argv:ARGV("0", "1"), 
            nullok: TRUE},
    {longname: "IPsec Dead Peer Protection Interval", argv:ARGV("10", "1200"), 
            nullok: TRUE},
    {longname: "IPsec Dead Peer Protection Timeout", argv:ARGV("30", "3600"), 
            nullok: TRUE},
    {longname: "IPsec Rule Split Tunneling", argv:ARGV("enabled", "responder", "initiator"), nullok: TRUE},
};

int 
valid_ipsec(webs_t wp, char *value, struct variable *v)
{
    int i, nrule;
    char tmp[LONG_BUF_LEN];
    char *val, *mode, *ike_keymode;

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "ipsec_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &ipsec_variables[IPSEC_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*val == '0') {
        return TRUE;
    }

    snprintf(tmp, sizeof(tmp), "ipsec_rule_num");
    val = websGetVar(wp, tmp, "0");
    nrule = atoi(val);
    for (i = 0; i < nrule; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_enable", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &ipsec_variables[IPSEC_RULE_ENABLE]) 
                == FALSE) {

            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_name", i);
        val = websGetVar(wp, tmp, "");
        if (valid_strict_name(wp, val, &ipsec_variables[IPSEC_RULE_NAME]) 
                == FALSE) {

            return FALSE;
        }

        /* VPN Mode */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_mode", i);
        mode = websGetVar(wp, tmp, "");
        if (valid_choice(wp, mode, &ipsec_variables[IPSEC_RULE_MODE]) 
                == FALSE) {

            return FALSE;
        }

        /* L2TP Enable */
        if (!strcmp(mode, "roadwarrior")) {
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_l2tp", i);
            val = websGetVar(wp, tmp, "");
            if (valid_choice(wp, val, &ipsec_variables[IPSEC_RULE_L2TP]) 
                    == FALSE) {

                return FALSE;
            }
        }

        /* Local WAN Interface */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_extif", i);
        val = websGetVar(wp, tmp, "");
        if (valid_if(wp, val, &ipsec_variables[IPSEC_RULE_LOCAL_EXTIF]) 
                == FALSE) {

            return FALSE;
        }

        /* Local Subnet IP */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_inipaddr", i);
        val = websGetVar(wp, tmp, "");
        if (valid_subnet(wp, val, 
                    &ipsec_variables[IPSEC_RULE_LOCAL_INIPADDR]) 
                        == FALSE) {

            return FALSE;
        }

        /* Local Internal Netmask */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_netmask", i);
        val = websGetVar(wp, tmp, "");
        if (valid_netmask(wp, val, 
                    &ipsec_variables[IPSEC_RULE_LOCAL_NETMASK]) == FALSE) {

            return FALSE;
        }

        /* Remote External IP Address */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_gateway", i);
        val = websGetVar(wp, tmp, "");
        if (valid_strict_name(wp, val, 
                    &ipsec_variables[IPSEC_RULE_REMOTE_GATEWAY]) == FALSE) {

            return FALSE;
        }

        /* Remote Subnet IP */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_inipaddr", i);
        val = websGetVar(wp, tmp, "");
        if (valid_subnet(wp, val, 
                    &ipsec_variables[IPSEC_RULE_REMOTE_INIPADDR]) 
                        == FALSE) {

            return FALSE;
        }
        /* Remote Netmask */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_netmask", i);
        val = websGetVar(wp, tmp, "");
        if (valid_netmask(wp, val, 
                    &ipsec_variables[IPSEC_RULE_REMOTE_NETMASK]) == FALSE) {

            return FALSE;
        }

        /* Connection Initiation */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_conn_init", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_CONN_INIT]) == FALSE) {

            return FALSE;
        }

        /* Key Type */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_ike_keymode", i);
        ike_keymode = websGetVar(wp, tmp, "");
        if (valid_choice(wp, ike_keymode, 
                    &ipsec_variables[IPSEC_RULE_IKE_KEYMODE]) == FALSE) {

            return FALSE;
        }

        /* PSK */
        if (!strcmp(ike_keymode, "psk")) {
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_psk", i);
            val = websGetVar(wp, tmp, "");
            if (valid_length_range(wp, val, 
                        &ipsec_variables[IPSEC_RULE_PSK]) == FALSE) {

                return FALSE;
            }
        }

        /* RSA */
        if (!strcmp(ike_keymode, "rsa")) {
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_rsa", i);
            val = websGetVar(wp, tmp, "");
            if (valid_length_range(wp, val, 
                        &ipsec_variables[IPSEC_RULE_RSA]) == FALSE) {

                return FALSE;
            }
        }

        /* CA */
        /* TODO */

        /* Advanced Options */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_adv", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &ipsec_variables[IPSEC_RULE_ADV]) 
                == FALSE) {

            return FALSE;
        }

        /* Phase1 Mode */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_mode", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_MODE]) == FALSE) {

            return FALSE;
        }

#if EZPACKET
        /* Phase1 Left ID */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_left_id", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_LEFT_ID]) == FALSE) {

            return FALSE;
        }
        /* Phase1 Right ID */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_right_id", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_RIGHT_ID]) == FALSE) {

            return FALSE;
        }
#endif

        /* Phase1 Lifetime */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_lifetime", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_LIFETIME]) == FALSE) {

            return FALSE;
        }

        /* Phase2 Lifetime */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_lifetime", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE2_LIFETIME]) == FALSE) {

            return FALSE;
        }
        /* Phase1 Auth */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_auth", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_AUTH]) == FALSE) {

            return FALSE;
        }

        /* Phase1 Encrypt */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_encrypt", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_ENCRYPT]) == FALSE) {

            return FALSE;
        }

        /* Phase1 Group */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_group", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE1_GROUP]) == FALSE) {

            return FALSE;
        }


        /* Phase2 Auth */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_auth", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE2_AUTH]) == FALSE) {

            return FALSE;
        }

        /* Phase2 Encrypt */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_encrypt", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE2_ENCRYPT]) == FALSE) {

            return FALSE;
        }

        /* Phase2 Group */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_group", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, 
                    &ipsec_variables[IPSEC_RULE_PHASE2_GROUP]) == FALSE) {

            return FALSE;
        }
        /* DPD Enable */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_enable", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_DPD_ENABLE]) == FALSE) {

            return FALSE;
        }
        /* DPD Interval */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_interval", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_DPD_INTERVAL]) == FALSE) {

            return FALSE;
        }
        /* DPD Timeout */
        snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_timeout", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &ipsec_variables[IPSEC_RULE_DPD_TIMEOUT]) == FALSE) {

            return FALSE;
        }
    }
    return TRUE;
}

int 
save_ipsec(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int i, nrule, change = 0;
    int64_t map = 0;
    char rule[LONG_BUF_LEN], old_rule[LONG_BUF_LEN], tmp[LONG_BUF_LEN], ddns_gw[TMP_LEN];
    char all_rule[LONG_BUF_LEN];
    char *enable, *rule_num, *rule_enable, *name, *adv, *mode, 
         *l2tp, *local_extif, *local_inipaddr, *local_netmask, *remote_gw, 
         *remote_inipaddr, *remote_netmask, *conn_init, *ph1_mode, 
         *ph1_auth, *ph1_encrypt, *ph1_left_id, *ph1_right_id, *ph1_group,
         *ph2_group, *dpd_enable, *dpd_interval, *dpd_timeout,
         *ph2_auth, *ph2_encrypt, *ph1_lifetime, *ph2_lifetime, 
         *ike_keymode, *psk, *rsa, *ca;
#ifdef EZP_PROD_BRAND_PROX 
    char *split_tunnel;
#endif
    char *rule_set = "ipsec_rule";

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "ipsec_enable");
    enable = websGetVar(wp, tmp, "");
    if (strcmp(enable, nvram_safe_get("ipsec_enable"))) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set("ipsec_enable", enable);
        change = 1;
    }

    if (*enable == '1') {
        snprintf(tmp, sizeof(tmp), "ipsec_rule_num");
        rule_num = websGetVar(wp, tmp, "0");
        nrule = atoi(rule_num);

        snprintf(tmp, sizeof(tmp), "%d", nrule);
        if (nrule != atoi(nvram_safe_get("ipsec_rule_num"))) {
            /* In case the situation that the last rule is deleted but others are
             * kept the same and the check flow will treat it as 'nothing
             * changed'.
             */
            config_preaction(&map, v, s, "NUM=0", "");
            nvram_set("ipsec_rule_num", tmp);
            change = 1;
        }

        all_rule[0]='\0';
        for (i = 0; i < nrule; i++) {
            /* Enable */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_enable", i);
            rule_enable = websGetVar(wp, tmp, "");

            /* Name */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_name", i);
            name = websGetVar(wp, tmp, "");

            /* VPN Mode */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_mode", i);
            mode = websGetVar(wp, tmp, "");

            /* L2TP Enable */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_l2tp", i);
            l2tp = websGetVar(wp, tmp, "");

            /* Local WAN Interface */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_extif", i);
            local_extif = websGetVar(wp, tmp, "");

            /* Local Internal IP Address */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_inipaddr", i);
            local_inipaddr = websGetVar(wp, tmp, "");

            /* Local Internal Netmask */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_local_netmask", i);
            local_netmask = websGetVar(wp, tmp, "");

            /* Remote External IP Address */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_gateway", i);
            remote_gw = websGetVar(wp, tmp, "");

            /* Remote Internal IP Address */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_inipaddr", i);
            remote_inipaddr = websGetVar(wp, tmp, "");

            /* Remote Internal Netmask */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_remote_netmask", i);
            remote_netmask = websGetVar(wp, tmp, "");

            /* Connection Initiation */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_conn_init", i);
            conn_init = websGetVar(wp, tmp, "");

            /* Key Type */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_ike_keymode", i);
            ike_keymode = websGetVar(wp, tmp, "");

            /* PSK */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_psk", i);
            psk = websGetVar(wp, tmp, "");

            /* RSA */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_rsa", i);
            rsa = websGetVar(wp, tmp, "");

            /* CA */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_ca", i);
            ca = websGetVar(wp, tmp, "");

            /* Advanced Options*/
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_adv", i);
            adv = websGetVar(wp, tmp, "");

            /* Phase 1 Mode */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_mode", i);
            ph1_mode = websGetVar(wp, tmp, "");

            /* Phase 1 Left ID */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_left_id", i);
            ph1_left_id = websGetVar(wp, tmp, "");

            /* Phase 1 Right ID */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_right_id", i);
            ph1_right_id = websGetVar(wp, tmp, "");

            /* Phase 1 Lifetime */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_lifetime", i);
            ph1_lifetime = websGetVar(wp, tmp, "");

            /* Phase 2 Lifetime */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_lifetime", i);
            ph2_lifetime = websGetVar(wp, tmp, "");

            /* Phase 1 Authentication */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_auth", i);
            ph1_auth = websGetVar(wp, tmp, "");

            /* Phase 1 Encryption */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_encrypt", i);
            ph1_encrypt = websGetVar(wp, tmp, "");

            /* Phase 1 Group */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase1_group", i);
            ph1_group = websGetVar(wp, tmp, "");

            /* Phase 2 Authentication */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_auth", i);
            ph2_auth = websGetVar(wp, tmp, "");

            /* Phase 2 Encryption */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_encrypt", i);
            ph2_encrypt = websGetVar(wp, tmp, "");

            /* Phase 2 Group */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_phase2_group", i);
            ph2_group = websGetVar(wp, tmp, "");

            /* DPD Enable */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_enable", i);
            dpd_enable = websGetVar(wp, tmp, "");

            /* DPD Interval */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_interval", i);
            dpd_interval = websGetVar(wp, tmp, "");

            /* DPD Timeout */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_dpd_timeout", i);
            dpd_timeout = websGetVar(wp, tmp, "");

            /* DPD Timeout */
            snprintf(tmp, sizeof(tmp), "ipsec_rule%d_split_tunnel", i);
#ifdef EZP_PROD_BRAND_PROX 
            split_tunnel = websGetVar(wp, tmp, "");
            if(!split_tunnel) split_tunnel = "enabled";
#endif

            if(ezplib_get_attr_val(rule_set, i, "ddns_gateway", ddns_gw, sizeof(ddns_gw), EZPLIB_USE_CLI) < 0) {
                /* We need to restore ddns gateway value. If failed, use a
                 * default "0.0.0.0" */
                strcpy(ddns_gw, "0.0.0.0");
            }
#ifdef EZP_PROD_BRAND_PROX 
            snprintf(rule, sizeof(rule),
                     "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s"
                     "^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
                     name, rule_enable, mode, l2tp, local_extif, 
                     local_inipaddr, local_netmask, remote_gw, 
                     remote_inipaddr, remote_netmask, conn_init, 
                     ike_keymode, psk, rsa, ca, adv, ph1_mode, ph1_left_id, 
                     ph1_right_id,ph1_lifetime, ph2_lifetime, ph1_auth, 
                     ph1_encrypt, ph1_group, ph2_auth, ph2_encrypt, 
                     ph2_group, dpd_enable, dpd_interval, dpd_timeout, 
                     ddns_gw, split_tunnel);
#else
            snprintf(rule, sizeof(rule),
                     "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s"
                     "^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
                     name, rule_enable, mode, l2tp, local_extif, 
                     local_inipaddr, local_netmask, remote_gw, 
                     remote_inipaddr, remote_netmask, conn_init, 
                     ike_keymode, psk, rsa, ca, adv, ph1_mode, ph1_left_id, 
                     ph1_right_id,ph1_lifetime, ph2_lifetime, ph1_auth, 
                     ph1_encrypt, ph1_group, ph2_auth, ph2_encrypt, 
                     ph2_group, dpd_enable, dpd_interval, dpd_timeout, 
                     ddns_gw);
#endif

            if (ezplib_get_rule(rule_set, i, old_rule, LONG_BUF_LEN) < 0) {
                /* i-th rule doesn't exist */
                config_preaction(&map, v, s, "NUM=0", "");
                ezplib_append_rule(rule_set, rule);
                change = 1;
            } else {
                if (strcmp(rule, old_rule)) {
                    config_preaction(&map, v, s, "NUM=0", "");
                    ezplib_replace_rule(rule_set, i, rule);
                    change = 1;
                }
            }
            if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", rule);
            else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, rule);
        }
        nvram_set(rule_set, all_rule);
    }

    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }

    return change;
}
