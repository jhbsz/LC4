#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ezp.h"
#include "lang.h"
#include "common.h"
#include "ezp-lib.h"

enum {
    WF_CONTENT_RULE_ENABLE = 0,
    WF_CONTENT_RULE_ACTIVEX_ENABLE,
    WF_CONTENT_RULE_JAVA_ENABLE,
    WF_CONTENT_RULE_PROXY_ENABLE
};

enum {
    WF_RULE_ENABLE = 0,
    WF_RULE_KEYWORD,
    WF_RULE_TYPE,
    WF_RULE_ACCESS
};

/* 
 * Local variable array to validate config and show the longname of each 
 * config item.
 */
struct variable wf_content_variables[] = {
    {longname: "Web Filter Rule Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Web Filter Rule ActiveX Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Web Filter Rule Java Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Web Filter Rule Proxy Enable", argv:ARGV("0", "1"), nullok: FALSE},
};

struct variable wf_variables[] = {
    {longname: "Web Filter Rule Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Web Filter Rule Keyword", argv:ARGV("20"), nullok: FALSE},
    {longname: "Web Filter Rule Keyword Type", argv:ARGV("host","url","content"), nullok: FALSE},
    {longname: "Web Filter Rule Access Type", argv:ARGV("0","1"), nullok: FALSE},
};

/* enable^keyword^type^access */
int
valid_wf(webs_t wp, char *value, struct variable *v)
{
    int i, nrule;
    char tmp[TMP_LEN];
    char *enable, *keyword, *type, *access, *rule_num, *turbonat_enable;

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "wf_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, &wf_content_variables[WF_CONTENT_RULE_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* If TurboNAT is enabled, we can not start Web Filtering. */
    turbonat_enable = nvram_safe_get("turbonat_enable");
    /* If hardware NAT is enabled, we can not start Web Filtering. */
    ezplib_get_attr_val("nat_rule", 0, "hw_enable", tmp,
                            sizeof(tmp), EZPLIB_USE_CLI);
    if ((*enable == '1') && (!strcmp(tmp, "1") || (*turbonat_enable == '1'))) {
        websDebugWrite(wp, "%s.<br> %s<br> %s.<br>",
                lang_translate("Start Web Filtering failed"),
#ifdef EZP_PROD_BRAND_PROX
                lang_translate("Need to turn off Ultra-NAT and"),
                lang_translate("Hardware NAT first"));
#elif defined EZP_PROD_BRAND_ZYUS
                lang_translate("Need to turn off"),
                lang_translate("Hardware Accelerator first"));
#else
                lang_translate("Need to turn off TurboNAT and"),
                lang_translate("Hardware NAT first"));
#endif
        return FALSE;
    }
    if (*enable == '0') {
        return TRUE;
    }
    snprintf(tmp, sizeof(tmp), "activex_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, &wf_content_variables[WF_CONTENT_RULE_ACTIVEX_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "java_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, &wf_content_variables[WF_CONTENT_RULE_JAVA_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "proxy_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, &wf_content_variables[WF_CONTENT_RULE_PROXY_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "wf_rule_num");
    rule_num = websGetVar(wp, tmp, "0");
    nrule = atoi(rule_num);
    for (i = 0; i < nrule; i++) {
        /* Get variables. */

        /* Enable */
        snprintf(tmp, sizeof(tmp), "wf_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, &wf_variables[WF_RULE_ENABLE]) 
                == FALSE) {
            return FALSE;
        }

        /* Keyword */
        snprintf(tmp, sizeof(tmp), "wf_rule%d_keyword", i);
        keyword = websGetVar(wp, tmp, "");
        if (valid_name(wp, keyword, &wf_variables[WF_RULE_KEYWORD]) == FALSE) {
            return FALSE;
        }

        /* Type */
        snprintf(tmp, sizeof(tmp), "wf_rule%d_type", i);
        type = websGetVar(wp, tmp, "");
        if (valid_choice(wp, type, &wf_variables[WF_RULE_TYPE]) == FALSE) {
            return FALSE;
        }
        
        /* Access */
        snprintf(tmp, sizeof(tmp), "wf_rule%d_access", i);
        access = websGetVar(wp, tmp, "");
        if (valid_choice(wp, access, &wf_variables[WF_RULE_ACCESS]) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

int
save_wf(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *nv_rule_num = "wf_rule_num";
    char *nv_rule_max = "wf_rule_max";
    char *nv_rule = "wf_content_rule";
    char *wf_enable_rule = "wf_enable";
    int i, nrule, change = 0, max;
    int64_t map = 0;
    char rule[LONG_BUF_LEN], old_rule[LONG_BUF_LEN];
    char all_rule[LONG_BUF_LEN];
    char tmp[TMP_LEN];
    char *wf_enable, *activex_enable, *java_enable, *proxy_enable,
         *enable, *keyword, *type, *access, *orig_wf_enable;

    /* Enable virtual server. */
    /* WF enable */
    snprintf(tmp, sizeof(tmp), wf_enable_rule);
    wf_enable = websGetVar(wp, tmp, "");
    /* ActiveX enable */
    snprintf(tmp, sizeof(tmp), "activex_enable");
    activex_enable = websGetVar(wp, tmp, "");
    /* Java enable */
    snprintf(tmp, sizeof(tmp), "java_enable");
    java_enable = websGetVar(wp, tmp, "");
    /* Proxy enable */
    snprintf(tmp, sizeof(tmp), "proxy_enable");
    proxy_enable = websGetVar(wp, tmp, "");

    orig_wf_enable = nvram_safe_get(wf_enable_rule);
    printf("%s old:%s new:%s", wf_enable_rule, orig_wf_enable, wf_enable);
    if (strcmp(orig_wf_enable, wf_enable)){
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set(wf_enable_rule, wf_enable);
        change = 1;
    }
    /* Compare to original rule, check changes */
    snprintf(rule, sizeof(rule), "%s^%s^%s", 
            activex_enable, java_enable, proxy_enable);
    if (ezplib_get_rule(nv_rule, 0, old_rule, sizeof(old_rule)) < 0) {
        /* i-th rule doesn't exist */
        config_preaction(&map, v, s, "NUM=0", "");
        ezplib_append_rule(nv_rule, rule);
        change = 1;
    } else {
        if (strcmp(rule, old_rule)) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule(nv_rule, 0, rule);
            change = 1;
        }
    }

    nv_rule = "wf_rule";    
    if (*wf_enable == '1') {
        /* Get rule number. */
        snprintf(tmp, sizeof(tmp), "wf_rule_num");
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
            snprintf(tmp, sizeof(tmp), "wf_rule%d_enable", i);
            enable = websGetVar(wp, tmp, "0");

            /* Name */
            snprintf(tmp, sizeof(tmp), "wf_rule%d_keyword", i);
            keyword = websGetVar(wp, tmp, "");

            /* External IF */
            snprintf(tmp, sizeof(tmp), "wf_rule%d_type", i);
            type = websGetVar(wp, tmp, "");

            /* Alias Address */
            snprintf(tmp, sizeof(tmp), "wf_rule%d_access", i);
            access = websGetVar(wp, tmp, "");

            snprintf(rule, sizeof(rule), "%s^%s^%s^%s",
                     enable, keyword, type, access);
            printf("%s\n",rule);
            if (ezplib_get_rule(nv_rule, i, old_rule, sizeof(old_rule)) < 0) {
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

