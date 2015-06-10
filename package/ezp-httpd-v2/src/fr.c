#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ezp.h"
#include "common.h"
#include "ezp-lib.h"

enum {
    FR_DMZ_ENABLE = 0,
    FR_DMZ_IPADDR,
    FR_ENABLE,
    FR_RULE_ENABLE,
    FR_RULE_NAME,
    FR_RULE_EXTIF,
    FR_RULE_PROTO,
    FR_RULE_EXTPORT_START,
    FR_RULE_EXTPORT_END,
    FR_RULE_EXTPORT_RANGE,
    FR_RULE_IPADDR,
    FR_RULE_INPORT_START,
    FR_RULE_INPORT_END,
    FR_RULE_INPORT_RANGE
};

/* 
 * Local variable array to validate config and show the longname of each 
 * config item.
 */
struct variable fr_variables[] = {
    {longname: "DMZ Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "DMZ IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "Port Forward Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Port Forward Rule Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Port Forward Rule Name", argv:ARGV("20"), nullok: TRUE},
    {longname: "Port Forward Rule External IF", argv:ARGV("wan"), 
        nullok: FALSE},
    {longname: "Port Forward Rule Protocol", argv:ARGV("tcp", "udp", "both"), 
        nullok: FALSE},
    {longname: "Port Forward Rule External Port Start",
        argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "Port Forward Rule External Port End",
        argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "Port Forward Rule External Port Range", argv:ARGV(""),
        nullok: FALSE},
    {longname: "Port Forward Rule Internal IP Address", argv:ARGV(""),
        nullok: FALSE},
    {longname: "Port Forward Rule Internal Port Start",
        argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "Port Forward Rule Internal Port End",
        argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "Port Forward Rule Internal Port Range", argv:ARGV(""),
        nullok: FALSE}
};

static int
_valid_dmz(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN], lan_ip[TMP_LEN], val_netbit[TMP_LEN],
         val_netmask[TMP_LEN];
    char *enable, *ip, *rule_num;
    int nrule, i;

    rule_num = nvram_safe_get("wan_num");
    if (!*rule_num) {
        nrule = 1;
    } else {
        nrule = atoi(rule_num);
    }    

    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "fr_dmz%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, &fr_variables[FR_DMZ_ENABLE]) == FALSE) {
            return FALSE;
        }

        if (*enable == '0') {
            /* TODO: check how to do with it. */
            continue;
        }

        snprintf(tmp, sizeof(tmp), "fr_dmz%d_ipaddr", i);
        ip = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, ip, &fr_variables[FR_DMZ_IPADDR]) == FALSE) {
            return FALSE;
        }
        ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lan_ip, 
                TMP_LEN, EZPLIB_USE_CLI);
        ezplib_get_attr_val("lan_static_rule", 0, "mask", val_netbit, 
                TMP_LEN, EZPLIB_USE_CLI);
        trans_int2netmask(atoi(val_netbit), val_netmask, TMP_LEN);
        if (!is_legal_ip_netmask(lan_ip, val_netmask, ip)) {
            websDebugWrite(wp, "Invalid <b>%s</b> %s: not a LAN IP address<br>",
                    fr_variables[FR_DMZ_IPADDR].longname, ip);
            return FALSE;
        }
    }

    return TRUE;
}

static int
_save_dmz(webs_t wp, char *value, struct variable *v,
        struct service *s, int64_t *map)
{
    char rule[TMP_LEN], tmp[TMP_LEN];
    char *enable, *ip, *rule_num, *old_rule, *cur;
    int nrule, change = 0, i;

    rule_num = nvram_safe_get("wan_num");
    if (!*rule_num) {
        nrule = 1;
    } else {
        nrule = atoi(rule_num);
    }    
    for (i = 0, cur = rule; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "fr_dmz%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "fr_dmz%d_ipaddr", i);
        ip = websGetVar(wp, tmp, "");

        cur += 
            snprintf(cur, rule + TMP_LEN - cur, "%sDMZ%d^%s^%s",
                     (cur == rule) ?  "" : "|", i, enable, ip);
    }

    old_rule = nvram_safe_get("fr_dmz_rule");
    if (strcmp(old_rule, rule)) {
        config_preaction(map, v, s, "NUM=0", "");
        nvram_set("fr_dmz_rule", rule);
        change = 1;
    }

    return change;
}

int
valid_fr(webs_t wp, char *value, struct variable *v)
{
    int i, nrule;
    char tmp[TMP_LEN], lan_ip[TMP_LEN], val_netbit[TMP_LEN],
         val_netmask[TMP_LEN];
    char *enable, *name, *extif, *proto, *extport_start, *extport_end, *ip,
         *rule_num;

    if (_valid_dmz(wp, tmp, v) == FALSE) {
        return FALSE;
    }

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "fr_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, &fr_variables[FR_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*enable == '0') {
        return TRUE;
    }

    snprintf(tmp, sizeof(tmp), "fr_rule_num");
    rule_num = websGetVar(wp, tmp, "0");
    nrule = atoi(rule_num);
    for (i = 0; i < nrule; i++) {

        /* Get variables. */

        /* Enable */
        snprintf(tmp, sizeof(tmp), "fr_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, &fr_variables[FR_RULE_ENABLE]) 
                == FALSE) {

            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "fr_rule%d_name", i);
        name = websGetVar(wp, tmp, "");
        if (valid_name(wp, name, &fr_variables[FR_RULE_NAME]) == FALSE) {

            return FALSE;
        }

        /* External IF */
        snprintf(tmp, sizeof(tmp), "fr_rule%d_extif", i);
        extif = websGetVar(wp, tmp, "");
        if (valid_if(wp, extif, &fr_variables[FR_RULE_EXTIF]) == FALSE) {
            return FALSE;
        }

        /* Proto */
        snprintf(tmp, sizeof(tmp), "fr_rule%d_proto", i);
        proto = websGetVar(wp, tmp, "");
        if (valid_choice(wp, proto, &fr_variables[FR_RULE_PROTO]) == FALSE) {
            return FALSE;
        }

        /* External Port Start */
        snprintf(tmp, sizeof(tmp), "fr_rule%d_extport_start", i);
        extport_start = websGetVar(wp, tmp, "");
        if (valid_range(wp, extport_start, 
                        &fr_variables[FR_RULE_EXTPORT_START]) 
                == FALSE) {

            return FALSE;
        }

        /* External Port End */
        snprintf(tmp, sizeof(tmp), "fr_rule%d_extport_end", i);
        extport_end = websGetVar(wp, tmp, "");
        if (valid_range(wp, extport_end,
                        &fr_variables[FR_RULE_EXTPORT_END]) == FALSE) {
            return FALSE;
        }

        if (valid_port_range(wp, extport_start, extport_end,
                           &fr_variables[FR_RULE_EXTPORT_RANGE]) == FALSE) {
            return FALSE;
        }

        /* IP */
        snprintf(tmp, sizeof(tmp), "fr_rule%d_ipaddr", i);
        ip = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, ip, &fr_variables[FR_RULE_IPADDR]) == FALSE) {
            return FALSE;
        }
        ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lan_ip, 
                TMP_LEN, EZPLIB_USE_CLI);
        ezplib_get_attr_val("lan_static_rule", 0, "mask", val_netbit, 
                TMP_LEN, EZPLIB_USE_CLI);
        trans_int2netmask(atoi(val_netbit), val_netmask, TMP_LEN);
        if (!is_legal_ip_netmask(lan_ip, val_netmask, ip)) {
            websDebugWrite(wp, "Invalid <b>%s</b> %s: not a LAN IP address<br>",
                    fr_variables[FR_RULE_IPADDR].longname, ip);
            return FALSE;
        }
    }
    return TRUE;
}

int
save_fr(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *nv_enable = "fr_enable";
    char *nv_rule_num = "fr_rule_num";
    char *nv_rule_max = "fr_rule_max";
    char *nv_rule = "fr_rule";
    int i, nrule, change = 0, max;
    int64_t map = 0;
    char rule[LONG_BUF_LEN], old_rule[LONG_BUF_LEN];
    char all_rule[LONG_BUF_LEN];
    char tmp[TMP_LEN];
    char *enable, *name, *extif, *proto, *extport_start, *extport_end, *ip,
         *inport_start, *inport_end;

    if (_save_dmz(wp, NULL, v, s, &map)) {
        change = 1;
    }

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "fr_enable");
    enable = websGetVar(wp, tmp, "");
    if (strcmp(enable, nvram_safe_get(nv_enable))) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set(nv_enable, enable);
        change = 1;
    }

    if (*enable == '1') {
        /* the forwarding is enabled */
        snprintf(tmp, sizeof(tmp), "fr_rule_num");
        nrule = atoi(websGetVar(wp, tmp, "0"));
        max = atoi(nvram_safe_get(nv_rule_max));
        nrule = (nrule >= max) ? max : nrule;
        
        if (nrule != atoi(nvram_safe_get(nv_rule_num))) {
            /* In case the situation that the last rule is deleted but others are
             * kept the same and the check flow will treat it as 'nothing
             * changed'.
             */
            snprintf(tmp, sizeof(tmp), "%d", nrule);
            config_preaction(&map, v, s, "NUM=0", "");
            nvram_set(nv_rule_num, tmp);
            change = 1;
        }

        all_rule[0]='\0';
        for (i = 0; i < nrule; i++) {

            /* Enable */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_enable", i);
            enable = websGetVar(wp, tmp, "0");

            /* Name */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_name", i);
            name = websGetVar(wp, tmp, "");

            /* External IF */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_extif", i);
            extif = websGetVar(wp, tmp, "");

            /* Proto */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_proto", i);
            proto = websGetVar(wp, tmp, "");

            /* External Port Start */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_extport_start", i);
            extport_start = websGetVar(wp, tmp, "");

            /* External Port End */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_extport_end", i);
            extport_end = websGetVar(wp, tmp, "");

            /* IP */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_ipaddr", i);
            ip = websGetVar(wp, tmp, "");

            /* Internal Port Start */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_inport_start", i);
            inport_start = websGetVar(wp, tmp, "");

            /* Internal Port End */
            snprintf(tmp, sizeof(tmp), "fr_rule%d_inport_end", i);
            inport_end = websGetVar(wp, tmp, "");

            snprintf(rule, sizeof(rule), "%s^%s^%s^%s^%s^%s^%s^%s^%s",
                     name, enable, extif, proto, extport_start, extport_end, ip, 
                     inport_start, inport_end);

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
ej_fr_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
#ifdef EZP_PROD_BRAND_PROX
    for (i = num-1; i >=0; i--) {
#else
    for (i = 0; i < num; i++) {
#endif
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("fr_dmz_setting.asp", wp, buf, "");
    }

    return 0;
}

