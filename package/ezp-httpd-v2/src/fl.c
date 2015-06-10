#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ezp.h"
#include "common.h"
#include "ezp-lib.h"

enum {
    FL_ENABLE = 0,
    FL_RULE_DEFAULT,
    FL_RULE_NAME,
    FL_RULE_ENABLE,
    FL_RULE_EXTIF,
    FL_RULE_INIF,
    FL_RULE_PROTO,
    FL_RULE_INIPADDR_START,
    FL_RULE_INIPADDR_END,
    FL_RULE_INIPADDR_RANGE,
    FL_RULE_EXTIPADDR_START,
    FL_RULE_EXTIPADDR_END,
    FL_RULE_EXTIPADDR_RANGE,
    FL_RULE_EXTPORT_START,
    FL_RULE_EXTPORT_END,
    FL_RULE_EXTPORT_RANGE,
    FL_RULE_ACTION
};

enum {
    FL_HWADDR_ENABLE = 0,
    FL_HWADDR_RULE_DEFAULT,
    FL_HWADDR_RULE_NAME,
    FL_HWADDR_RULE_ACL_ENABLE,
    FL_HWADDR_RULE_DHCP_ENABLE,
    FL_HWADDR_RULE_ARP_ENABLE,
    FL_HWADDR_RULE_HWADDR,
    FL_HWADDR_RULE_ACTION,
    FL_HWADDR_RULE_IPADDR
};

struct variable fl_variables[] = {
  {longname: "ACL Enable", argv:ARGV("0", "1"), nullok: FALSE},
  {longname: "ACL Rule Default Action", argv:ARGV("0", "1"), nullok: FALSE},
  {longname: "ACL Rule Name", argv:ARGV("20"), nullok: TRUE},
  {longname: "ACL Rule Enable", argv:ARGV("0", "1"),
        nullok: FALSE},
  {longname: "ACL Rule External Interface", argv:ARGV("wan"),
        nullok: TRUE},
  {longname: "ACL Rule Internal Interface", argv:ARGV("lan"),
        nullok: TRUE},
  {longname: "ACL Rule Protocol", argv:ARGV("both","tcp","udp"), 
        nullok: TRUE},
  {longname: "ACL Rule Internal IP Start", argv:ARGV(""), nullok: TRUE},
  {longname: "ACL Rule Internal IP End", argv:ARGV(""), nullok: TRUE},
  {longname: "ACL Rule Internal IP Range", argv:ARGV(""), nullok: TRUE},
  {longname: "ACL Rule External IP Start", argv:ARGV(""), nullok: TRUE},
  {longname: "ACL Rule External IP End", argv:ARGV(""), nullok: TRUE},
  {longname: "ACL Rule External IP Range", argv:ARGV(""), nullok: TRUE},
  {longname: "ACL Rule External Port Start", argv:ARGV("1","65535"),
        nullok: TRUE},
  {longname: "ACL Rule External Port End", argv:ARGV("1","65535"), nullok: TRUE},
  {longname: "ACL Rule External Port Range", argv:ARGV("1","65535"),
        nullok: TRUE},
  {longname: "ACL Rule Action", argv:ARGV("0","1"), nullok: FALSE},
};

struct variable fl_hwaddr_variables[] = {
  {longname: "MAC ACL Enable", argv:ARGV("0", "1"), nullok: FALSE},
  {longname: "MAC ACL Rule Default Action", argv:ARGV("0", "1"), nullok: FALSE},
  {longname: "MAC ACL Rule Name", argv:ARGV("20"), nullok: TRUE},
  {longname: "MAC ACL Rule ACL Enable", argv:ARGV("0", "1"), nullok: FALSE},
  {longname: "MAC ACL Rule DHCP Enable", argv:ARGV("0", "1"), nullok: FALSE},
  {longname: "MAC ACL Rule ARP Enable", argv:ARGV("0", "1"), nullok: FALSE},
  {longname: "MAC ACL Rule MAC", argv:ARGV(""), nullok: FALSE},
  {longname: "MAC ACL Rule Action", argv:ARGV("0","1"), nullok: FALSE},
  {longname: "MAC ACL Rule IP", argv:ARGV(""), nullok: TRUE}
};

int
valid_fl_hwaddr(webs_t wp, char *value, struct variable *v)
{
    int i, nrule;
    char tmp[TMP_LEN];
    char *enable, *acl_enable, *name, *dhcp_enable, *arp_enable, *hwaddr,
         *action, *rule_num, *ipaddr, *rule_default;

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "fl_hwaddr_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, 
                &fl_hwaddr_variables[FL_HWADDR_ENABLE]) == FALSE) {

        return FALSE;
    }

    if (*enable == '0') {
        return TRUE;
    }

    snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule_default");
    rule_default = websGetVar(wp, tmp, "");
    if (valid_choice(wp, rule_default, 
                &fl_hwaddr_variables[FL_HWADDR_RULE_DEFAULT]) == FALSE) {

        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule_num");
    rule_num = websGetVar(wp, tmp, "0");
    nrule = atoi(rule_num);
    for (i = 0; i < nrule; i++) {

        /* ACL Enable */
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_acl_enable", i);
        acl_enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, acl_enable, 
                    &fl_hwaddr_variables[FL_HWADDR_RULE_ACL_ENABLE]) 
                == FALSE) {

            return FALSE;
        }

        /* DHCP Enable */
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_dhcp_enable", i);
        dhcp_enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, dhcp_enable, 
                    &fl_hwaddr_variables[FL_HWADDR_RULE_DHCP_ENABLE]) 
                == FALSE) {

            return FALSE;
        }

        /* ARP Enable */
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_arp_enable", i);
        arp_enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, arp_enable, 
                    &fl_hwaddr_variables[FL_HWADDR_RULE_ARP_ENABLE]) 
                == FALSE) {

            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_name", i);
        name = websGetVar(wp, tmp, "");
        if (valid_name(wp, name, 
                    &fl_hwaddr_variables[FL_HWADDR_RULE_NAME]) == FALSE) {

            return FALSE;
        }

        /* HWAddr */
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_hwaddr", i);
        hwaddr = websGetVar(wp, tmp, "");
        if (valid_hwaddr(wp, hwaddr, 
                    &fl_hwaddr_variables[FL_HWADDR_RULE_HWADDR]) == FALSE) {

            return FALSE;
        }

        /* Action */
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_op", i);
        action = websGetVar(wp, tmp, "");
        if (valid_choice(wp, action, 
                    &fl_hwaddr_variables[FL_HWADDR_RULE_ACTION]) == FALSE) {

            return FALSE;
        }

        /* IPAddr */
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_ipaddr", i);
        ipaddr = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, ipaddr, 
                    &fl_hwaddr_variables[FL_HWADDR_RULE_IPADDR]) == FALSE) {

            return FALSE;
        }
    }
    return TRUE;
}

int
save_fl_hwaddr(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *nv_enable = "fl_hwaddr_enable";
    char *nv_rule_num = "fl_hwaddr_rule_num";
    char *nv_rule_default = "fl_hwaddr_rule_default";
    char *nv_rule_max = "fl_hwaddr_rule_max";
    char *nv_rule = "fl_hwaddr_rule";
    int i, nrule, max, change = 0;
    int64_t map = 0;
    char rule[TMP_LEN], old_rule[TMP_LEN];
    char all_rule[LONG_BUF_LEN];
    char tmp[TMP_LEN];
    char *enable, *name, *acl_enable, *dhcp_enable, *arp_enable, *hwaddr,
         *action, *ipaddr, *rule_default;

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "fl_hwaddr_enable");
    enable = websGetVar(wp, tmp, "");

    if (strcmp(enable, nvram_safe_get(nv_enable))) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set(nv_enable, enable);
        change = 1;
    }

    if (*enable == '1') {
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule_default");
        rule_default = websGetVar(wp, tmp, "");
        if (strcmp(rule_default, nvram_safe_get(nv_rule_default))) {
            config_preaction(&map, v, s, "NUM=0", "");
            nvram_set(nv_rule_default, rule_default);
            change = 1;
        }

        /* the forwarding is enabled */
        snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule_num");
        nrule = atoi(websGetVar(wp, tmp, "0"));
        max = atoi(nvram_safe_get(nv_rule_max));
        nrule = (nrule >= max) ? max : nrule;
        
        if (nrule != atoi(nvram_safe_get(nv_rule_num))) {
            snprintf(tmp, sizeof(tmp), "%d", nrule);
            config_preaction(&map, v, s, "NUM=0", "");
            nvram_set(nv_rule_num, tmp);
            change = 1;
        }

        all_rule[0]='\0';
        for (i = 0; i < nrule; i++) {

            /* Name */
            snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_name", i);
            name = websGetVar(wp, tmp, "");

            /* ACL Enable */
            snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_acl_enable", i);
            acl_enable = websGetVar(wp, tmp, "0");

            /* DHCP Enable */
            snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_dhcp_enable", i);
            dhcp_enable = websGetVar(wp, tmp, "0");

            /* ARP Enable */
            snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_arp_enable", i);
            arp_enable = websGetVar(wp, tmp, "0");

            /* HWaddr */
            snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_hwaddr", i);
            hwaddr = websGetVar(wp, tmp, "");

            /* Action */
            snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_op", i);
            action = websGetVar(wp, tmp, "");

            /* Ipaddr */
            snprintf(tmp, sizeof(tmp), "fl_hwaddr_rule%d_ipaddr", i);
            ipaddr = websGetVar(wp, tmp, "");

            snprintf(rule, sizeof(rule), "%s^%s^%s^%s^%s^%s^%s",
                     name, hwaddr, action, acl_enable, arp_enable, dhcp_enable,
                     ipaddr);

            if (ezplib_get_rule(nv_rule, i, old_rule, TMP_LEN) < 0) {
                /* i-th rule doesn't exist */
                config_preaction(&map, v, s, "NUM=0", "");
                ezplib_append_rule(nv_rule, rule);
                change = 1;
            } else {
                if (strcmp(rule, old_rule)) {
                    config_preaction(&map, v, s, "NUM=0", "");
                    ezplib_replace_rule(nv_rule, i, rule);
                    change = 1;
                }
            }
            if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", rule);
            else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, rule);
        }
        nvram_set(nv_rule, all_rule);
    }

    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }

    return change;
}

int
valid_fl(webs_t wp, char *value, struct variable *v)
{
    int i, nrule;
    char tmp[TMP_LEN];
    char *enable, *name, *proto, *extif, *inipaddr_start, *inipaddr_end,
         *extipaddr_start, *extipaddr_end, *extport_start, *extport_end,
         *action, *rule_num, *rule_default;

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "fl_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, 
                &fl_variables[FL_ENABLE]) == FALSE) {

        return FALSE;
    }

    if (*enable == '0') {
        return TRUE;
    }

    snprintf(tmp, sizeof(tmp), "fl_rule_default");
    rule_default = websGetVar(wp, tmp, "");
    if (valid_choice(wp, rule_default, 
                &fl_hwaddr_variables[FL_RULE_DEFAULT]) == FALSE) {

        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "fl_rule_num");
    rule_num = websGetVar(wp, tmp, "0");
    nrule = atoi(rule_num);
    for (i = 0; i < nrule; i++) {

        /* Enable */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, 
                    &fl_variables[FL_RULE_ENABLE]) == FALSE) {

            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_name", i);
        name = websGetVar(wp, tmp, "");
        if (valid_name(wp, name, 
                    &fl_variables[FL_RULE_NAME]) == FALSE) {

            return FALSE;
        }

        /* External IF */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_extif", i);
        extif = websGetVar(wp, tmp, "");
        if (valid_if(wp, extif, 
                    &fl_variables[FL_RULE_EXTIF]) == FALSE) {

            return FALSE;
        }

        /* Internal IP Start */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_inipaddr_start", i);
        inipaddr_start = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, inipaddr_start, 
                    &fl_variables[FL_RULE_INIPADDR_START]) == FALSE) {

            return FALSE;
        }

        /* Internal IP End */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_inipaddr_end", i);
        inipaddr_end = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, inipaddr_end, 
                    &fl_variables[FL_RULE_INIPADDR_END]) == FALSE) {

            return FALSE;
        }

        if (valid_ipaddr_range(wp, inipaddr_start, inipaddr_end,
                &fl_variables[FL_RULE_INIPADDR_RANGE]) == FALSE) {

            return FALSE;
        }

        /* External IP Start */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_extipaddr_start", i);
        extipaddr_start = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, extipaddr_start, 
                    &fl_variables[FL_RULE_EXTIPADDR_START]) == FALSE) {

            return FALSE;
        }

        /* External IP End */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_extipaddr_end", i);
        extipaddr_end = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, extipaddr_end, 
                    &fl_variables[FL_RULE_EXTIPADDR_END]) == FALSE) {

            return FALSE;
        }

        if (valid_ipaddr_range(wp, extipaddr_start, extipaddr_end,
                &fl_variables[FL_RULE_EXTIPADDR_RANGE]) == FALSE) {

            return FALSE;
        }

        /* Proto */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_proto", i);
        proto = websGetVar(wp, tmp, "");
        if (valid_choice(wp, proto, 
                    &fl_variables[FL_RULE_PROTO]) == FALSE) {

            return FALSE;
        }

        if (*proto) {
            /* External Port Start */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_extport_start", i);
            extport_start = websGetVar(wp, tmp, "");
            if (valid_range(wp, extport_start, 
                        &fl_variables[FL_RULE_EXTPORT_START]) == FALSE) {

                return FALSE;
            }

            /* External Port End */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_extport_end", i);
            extport_end = websGetVar(wp, tmp, "");
            if (valid_range(wp, extport_end, 
                        &fl_variables[FL_RULE_EXTPORT_END]) == FALSE) {

                return FALSE;
            }

            if (valid_port_range(wp, extport_start, extport_end,
                    &fl_variables[FL_RULE_EXTPORT_RANGE]) == FALSE) {

                return FALSE;
            }
        }

        /* Action */
        snprintf(tmp, sizeof(tmp), "fl_rule%d_op", i);
        action = websGetVar(wp, tmp, "");
        if (valid_choice(wp, action, 
                    &fl_variables[FL_RULE_ACTION]) == FALSE) {

            return FALSE;
        }
    }
    return TRUE;
}

int
save_fl(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *nv_fl_enable = "fl_enable";
    char *nv_fl_rule_num = "fl_rule_num";
    char *nv_fl_rule_default = "fl_rule_default";
    char *nv_fl_rule_max = "fl_rule_max";
    char *nv_fl_rule = "fl_rule";
    char *nv_bw_rule_num = "bw_rule_num";
    int i, nrule, change = 0, max;
    int64_t map = 0;
    char fl_rule[LONG_BUF_LEN], old_rule[LONG_BUF_LEN];
    char all_rule[LONG_BUF_LEN];
    char tmp[TMP_LEN];
    char *enable, *name, *extif, *proto, *inipaddr_start, *inipaddr_end,
         *extipaddr_start, *extipaddr_end, *extport_start, *extport_end,
         *action, *rule_default;

    /* Enable of the whole forwarding mechanism. */
    enable = websGetVar(wp, "fl_enable", "");
    if (strcmp(enable, nvram_safe_get(nv_fl_enable))) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set(nv_fl_enable, enable);
        change = 1;
    }

    if (*enable == '1') {
        snprintf(tmp, sizeof(tmp), "fl_rule_default");
        rule_default = websGetVar(wp, tmp, "");
        if (strcmp(rule_default, nvram_safe_get(nv_fl_rule_default))) {
            config_preaction(&map, v, s, "NUM=0", "");
            nvram_set(nv_fl_rule_default, rule_default);
            change = 1;
        }

        /* the forwarding is enabled */
        snprintf(tmp, sizeof(tmp), "fl_rule_num");
        nrule = atoi(websGetVar(wp, tmp, "0"));
        max = atoi(nvram_safe_get(nv_fl_rule_max));
        nrule = (nrule >= max) ? max : nrule;
        
        if (nrule != atoi(nvram_safe_get(nv_fl_rule_num))) {
            /* In case the situation that the last rule is deleted but others are
             * kept the same and the check flow will treat it as 'nothing
             * changed'.
             */
            snprintf(tmp, sizeof(tmp), "%d", nrule);
            config_preaction(&map, v, s, "NUM=0", "");
            nvram_set(nv_fl_rule_num, tmp);
            nvram_set(nv_bw_rule_num, tmp);
            change = 1;
        }

        all_rule[0]='\0';
        for (i = 0; i < nrule; i++) {

            /* Enable */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_enable", i);
            enable = websGetVar(wp, tmp, "");

            /* Name */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_name", i);
            name = websGetVar(wp, tmp, "");

            /* External IF */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_extif", i);
            extif = websGetVar(wp, tmp, "");

            /* Protocol */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_proto", i);
            proto = websGetVar(wp, tmp, "");

            /* Internal IP Start */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_inipaddr_start", i);
            inipaddr_start = websGetVar(wp, tmp, "");

            /* Internal IP End */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_inipaddr_end", i);
            inipaddr_end = websGetVar(wp, tmp, "");

            /* External IP Start */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_extipaddr_start", i);
            extipaddr_start = websGetVar(wp, tmp, "");

            /* External IP End */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_extipaddr_end", i);
            extipaddr_end = websGetVar(wp, tmp, "");

            /* External Port Start */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_extport_start", i);
            extport_start = websGetVar(wp, tmp, "");

            /* External Port End */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_extport_end", i);
            extport_end = websGetVar(wp, tmp, "");

            /* Action */
            snprintf(tmp, sizeof(tmp), "fl_rule%d_op", i);
            action = websGetVar(wp, tmp, "");

            snprintf(fl_rule, sizeof(fl_rule),
                     "%s^%s^%s^^%s^%s^%s^%s^%s^^^%s^%s^%s^%04X^%d^%d",
                     name, enable, extif, inipaddr_start, inipaddr_end,
                     extipaddr_start, extipaddr_end, proto, extport_start, extport_end,
                     action, i+1, i, i);

            if (ezplib_get_rule(nv_fl_rule, i, old_rule, TMP_LEN) < 0) {
                /* i-th rule doesn't exist */
                config_preaction(&map, v, s, "NUM=0", "");
                ezplib_append_rule(nv_fl_rule, fl_rule);
                change = 1;
            } else {
                if (strcmp(fl_rule, old_rule)) {
                    config_preaction(&map, v, s, "NUM=0", "");
                    ezplib_replace_rule(nv_fl_rule, i, fl_rule);
                    change = 1;
                }
            }
            if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", fl_rule);
            else snprintf(all_rule,sizeof(all_rule),"%s|%s",all_rule, fl_rule);
        }
        nvram_set(nv_fl_rule, all_rule);
    }

    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }

    return change;
}
