#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ezp.h"
#include "common.h"
#include "ezp-lib.h"

enum {
    VS_ENABLE = 0,
    VS_RULE_ENABLE,
    VS_RULE_NAME,
    VS_RULE_EXTIF,
    VS_RULE_INDEX,
    VS_RULE_ALIAS_ADDR,
    VS_RULE_DEST_ADDR
};

struct variable vs_variables[] = {
    {longname: "Virtual Server Enable", argv:ARGV("0", "1"), 
        nullok: FALSE},
    {longname: "Virtual Server Rule Enable", argv:ARGV("0", "1"), 
        nullok: FALSE},
    {longname: "Virtual Server Rule Name", argv:ARGV("20"), 
        nullok: TRUE},
    {longname: "Virtual Server Rule External IF", argv:ARGV("wan"), 
        nullok: FALSE},
    {longname: "Virtual Server Index", argv:ARGV(""), 
        nullok: FALSE},
    {longname: "Virtual Server Alias IP Address", argv:ARGV(""), 
        nullok: FALSE},
    {longname: "Virtual Server Destination IP Address", argv:ARGV(""), 
        nullok: FALSE},
};

/* name^enable[0|1]^extif^index^wan_ipaddr^mapped_ipaddr */
int
valid_vs(webs_t wp, char *value, struct variable *v)
{
    int i, nrule;
    char tmp[TMP_LEN];
    char *enable, *name, *extif, *ip, *rule_num;

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "vs_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, &vs_variables[VS_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*enable == '0') {
        return TRUE;
    }
    printf("verify\n");
    snprintf(tmp, sizeof(tmp), "vs_rule_num");
    rule_num = websGetVar(wp, tmp, "0");
    nrule = atoi(rule_num);
    for (i = 0; i < nrule; i++) {

        /* Get variables. */

        /* Enable */
        snprintf(tmp, sizeof(tmp), "vs_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, &vs_variables[VS_RULE_ENABLE]) 
                == FALSE) {
            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "vs_rule%d_name", i);
        name = websGetVar(wp, tmp, "");
        if (valid_name(wp, name, &vs_variables[VS_RULE_NAME]) == FALSE) {
            return FALSE;
        }

        /* External IF */
        snprintf(tmp, sizeof(tmp), "vs_rule%d_extif", i);
        extif = websGetVar(wp, tmp, "");
        if (valid_if(wp, extif, &vs_variables[VS_RULE_EXTIF]) == FALSE) {
            return FALSE;
        }

        /* Alias IP */
        snprintf(tmp, sizeof(tmp), "vs_rule%d_wan_ipaddr", i);
        ip = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, ip, &vs_variables[VS_RULE_ALIAS_ADDR]) == FALSE) {
            return FALSE;
        }
        
        /* Destination IP */
        snprintf(tmp, sizeof(tmp), "vs_rule%d_mapped_ipaddr", i);
        ip = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, ip, &vs_variables[VS_RULE_DEST_ADDR]) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

int
save_vs(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *nv_enable = "vs_enable";
    char *nv_rule_num = "vs_rule_num";
    char *nv_rule_max = "vs_rule_max";
    char *nv_rule = "vs_rule";
    int i, nrule, change = 0, max;
    int64_t map = 0;
    char rule[LONG_BUF_LEN], old_rule[LONG_BUF_LEN];
    char all_rule[LONG_BUF_LEN];
    char tmp[TMP_LEN];
    char *enable, *name, *extif, *wan_ipaddr, *mapped_ipaddr;

    /* Enable virtual server. */
    snprintf(tmp, sizeof(tmp), "vs_enable");
    enable = websGetVar(wp, tmp, "");
    if (strcmp(enable, nvram_safe_get(nv_enable))) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set(nv_enable, enable);
        change = 1;
    }
    if (*enable == '1') {
        /* Get rule number. */
        snprintf(tmp, sizeof(tmp), "vs_rule_num");
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
            snprintf(tmp, sizeof(tmp), "vs_rule%d_enable", i);
            enable = websGetVar(wp, tmp, "0");

            /* Name */
            snprintf(tmp, sizeof(tmp), "vs_rule%d_name", i);
            name = websGetVar(wp, tmp, "");

            /* External IF */
            snprintf(tmp, sizeof(tmp), "vs_rule%d_extif", i);
            extif = websGetVar(wp, tmp, "");

            /* Alias Address */
            snprintf(tmp, sizeof(tmp), "vs_rule%d_wan_ipaddr", i);
            wan_ipaddr = websGetVar(wp, tmp, "");

            /* Destination Address */
            snprintf(tmp, sizeof(tmp), "vs_rule%d_mapped_ipaddr", i);
            mapped_ipaddr = websGetVar(wp, tmp, "");

            snprintf(rule, sizeof(rule), "%s^%s^%s^%d^%s^%s",
                     name, enable, extif, i, wan_ipaddr, mapped_ipaddr);
            printf("%d:%s\n",i,rule);
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

