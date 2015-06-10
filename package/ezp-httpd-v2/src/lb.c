#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <cyutils.h>

#include "lang.h"
#include "common.h"
#include "ezp.h"
#include "ezp-lib.h"

enum {
    WAN_WEIGHT = 0,
    WAN_DETECT_ENABLE,
    WAN_DETECT_TYPE,
    WAN_DETECT_HOST,
    WAN_DETECT_THRESHOLD,
    WAN_DETECT_TIMEOUT,
    WAN_MODE,
    WAN_BACKUP,
    WAN_MAXCOUNT
};

enum {
    WAN_FAILOVER_FAILBACK = 0,
};

static struct variable lb_variables[] = {
    {longname: "WAN Weight", argv:ARGV("1","10"), nullok: FALSE},
    {longname: "WAN Detection Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "WAN Detection Type", argv:ARGV("gateway", "custom"), nullok: FALSE},
    {longname: "WAN Target IP", argv:ARGV("48"), nullok: FALSE},
    {longname: "WAN Detection Threshold", argv:ARGV("1", "32"), nullok: FALSE},
    {longname: "WAN Detection Timeout", argv:ARGV("1", "60"), nullok: FALSE},
    {longname: "WAN Connect Mode", argv:ARGV("alwayson", "backup_standby", "backup"), nullok: FALSE},
    {longname: "WAN Backup", argv:ARGV("none", "wan0", "wan1", "wan2", "wan3", "wan4", "wan5"), nullok: FALSE},
    {longname: "WAN Wait Time", argv:ARGV("1", "60"), nullok: FALSE}
};

int
valid_lb(webs_t wp, char *value, struct variable *v)
{
    char *cur = nvram_safe_get("wan_num");
    char tmp[TMP_LEN];
    char *val;
    int num, i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

#ifdef EZP_PROD_BRAND_ZYUS
    /* ZYUS only nees to show one section. */
    num = 1;
#endif
    /* WAN individual setting */
    for (i = 0; i < num; i++) {
        /* if wan num great than 2,
         * we need these variables to config failover function */
        if(num >= 2) { 
            /* Connection Mode */
            snprintf(tmp, sizeof(tmp), "wan%d_mode", i);
            val = websGetVar(wp, tmp, "");

            if (valid_choice(wp, val,
                        &lb_variables[WAN_MODE]) == FALSE) {
                return FALSE;
            }

            /* Backup WAN */
            snprintf(tmp, sizeof(tmp), "wan%d_backup", i);
            val = websGetVar(wp, tmp, "");

            if (valid_choice(wp, val,
                        &lb_variables[WAN_BACKUP]) == FALSE) {
                return FALSE;
            }

            /* Load Balance Weight */
            snprintf(tmp, sizeof(tmp), "wan%d_weight", i);
            val = websGetVar(wp, tmp, "");

            if (valid_range(wp, val,
                        &lb_variables[WAN_WEIGHT]) == FALSE) {
                return FALSE;
            }

            /* Max Try Count */
            snprintf(tmp, sizeof(tmp), "wan%d_maxcount", i);
            val = websGetVar(wp, tmp, "");

            if (valid_range(wp, val,
                        &lb_variables[WAN_MAXCOUNT]) == FALSE) {
                return FALSE;
            }

        }

        /* Detect Enable */
        snprintf(tmp, sizeof(tmp), "wan%d_detect_enable", i);
        val = websGetVar(wp, tmp, "");

        if (valid_choice(wp, val,
                   &lb_variables[WAN_DETECT_ENABLE]) == FALSE) {
            return FALSE;
        }

        if (*val == '1') {
            /* Detect Threshold */
            snprintf(tmp, sizeof(tmp), "wan%d_detect_threshold", i);
            val = websGetVar(wp, tmp, "");

            if (valid_range(wp, val,
                        &lb_variables[WAN_DETECT_THRESHOLD]) == FALSE) {
                return FALSE;
            }
            /* Detect Timeout */
            snprintf(tmp, sizeof(tmp), "wan%d_detect_timeout", i);
            val = websGetVar(wp, tmp, "");

            if (valid_range(wp, val,
                        &lb_variables[WAN_DETECT_TIMEOUT]) == FALSE) {
                return FALSE;
            }
            /* Detect Host Type */
            snprintf(tmp, sizeof(tmp), "wan%d_detect_type", i);
            val = websGetVar(wp, tmp, "");

            if (valid_choice(wp, val,
                  &lb_variables[WAN_DETECT_TYPE]) == FALSE) {
                return FALSE;
            }
            if (!strcmp(val, "custom")) {
                snprintf(tmp, sizeof(tmp), "wan%d_detect_host", i);
                val = websGetVar(wp, tmp, "");

                if (valid_ipaddr(wp, val,
                            &lb_variables[WAN_DETECT_HOST]) == FALSE) {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

int
save_lb(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *cur = nvram_safe_get("wan_num");
    char tmp[TMP_LEN], rule[TMP_LEN];
    /* wan_detect_rule */
    char *enable, *type, *host, *threshold, *timeout;
    char *mode, *backup, *weight; /* wan_lb_rule */
    char *maxcount; /* wan_main_rule */
    int num, len;
    int i, change = 0;
    int64_t map = 0;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

#ifdef EZP_PROD_BRAND_ZYUS
    /* ZYUS only nees to show one section. */
    num = 1;
#endif
    /* WAN individual setting */
    for (i = 0; i < num; i++) {

        /* if wan num great than 2,
         * we need these variables to config failover function */
        if(num >= 2) { 
            /* lb rule */
            /* Connection Mode */
            snprintf(tmp, sizeof(tmp), "wan%d_mode", i);
            mode = websGetVar(wp, tmp, "");

            /* Backup WAN */
            snprintf(tmp, sizeof(tmp), "wan%d_backup", i);
            backup = websGetVar(wp, tmp, "");

            /* Load Balance Weight */
            snprintf(tmp, sizeof(tmp), "wan%d_weight", i);
            weight= websGetVar(wp, tmp, "");

            len = snprintf(tmp, TMP_LEN, "%s^%s^^%s^1", mode, weight, backup);
            if (len >= TMP_LEN) {
                return 0;
            }

            ezplib_get_rule("wan_lb_rule", i, rule, sizeof(rule));
            if (strcmp(rule, tmp)) {
                config_preaction(&map, v, s, "", "");
                ezplib_replace_rule("wan_lb_rule", i, tmp);
                change = 1;
            }

            /* WAN Try Count */
            snprintf(tmp, sizeof(tmp), "wan%d_maxcount", i);
            maxcount= websGetVar(wp, tmp, "");
            ezplib_get_attr_val("wan_main_rule", i, "maxcount", tmp, 
                    sizeof(tmp), EZPLIB_USE_CLI);
            if(strcmp(tmp, maxcount)) {
                config_preaction(&map, v, s, "", "");
                ezplib_replace_attr("wan_main_rule", i, "maxcount", maxcount);
                change = 1;
            }

        }
        /* wan detect check rule */
        /* Detect Eanble */
        snprintf(tmp, sizeof(tmp), "wan%d_detect_enable", i);
        enable = websGetVar(wp, tmp, "");

        /* Detect Threshold */
        snprintf(tmp, sizeof(tmp), "wan%d_detect_threshold", i);
        threshold = websGetVar(wp, tmp, "");

        /* Detect timeout*/
        snprintf(tmp, sizeof(tmp), "wan%d_detect_timeout", i);
        timeout = websGetVar(wp, tmp, "");

        /* Detect Type */
        snprintf(tmp, sizeof(tmp), "wan%d_detect_type", i);
        type = websGetVar(wp, tmp, "");

        /* Detect Host */
        snprintf(tmp, sizeof(tmp), "wan%d_detect_host", i);
        host = websGetVar(wp, tmp, "");

        len = snprintf(rule, sizeof(rule), "%s^%s^%s^ping^%s^%s",
                enable, threshold, timeout, type, host);

        if (len >= TMP_LEN) {
            return FALSE;
        }

        ezplib_get_rule("wan_detect_rule", i, tmp, TMP_LEN);
        if (strcmp(rule, tmp)) {
            config_preaction(&map, v, s, "", "");
            ezplib_replace_rule("wan_detect_rule", i, rule);
            change = 1;
        }
#if 0
        /* failover rule */
        /* Failback */
        snprintf(tmp, sizeof(tmp), "wan%d_failback_enable", i);
        fo_failback = websGetVar(wp, tmp, "");
        ezplib_get_attr_val("wan_failover_rule", i, "failback",
                tmp, sizeof(tmp), EZPLIB_USE_CLI);
        if(strcmp(fo_failback, tmp)) {
            config_preaction(&map, v, s, "", "");
            ezplib_replace_attr("wan_failover_rule", i,
                    "failback", fo_failback);
            change = 1;
        }
#endif
    }

    if(change) {
        config_postaction(map, s, "", "");
    }

    return change;
}

int
ej_lb_show_setting(int eid, webs_t wp, int argc, char_t **argv)
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
#ifdef EZP_PROD_BRAND_ZYUS
    /* ZYUS only nees to show one section. */
    num = 1;
#endif
    
#ifdef EZP_PROD_BRAND_PROX
    for (i = num-1; i >= 0; i--){
#else
    for (i = 0; i < num; i++) {
#endif
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("lb_setting.asp", wp, buf, "");
    }

    return 0;
}

int 
ej_lb_legend(int eid, webs_t wp, int argc, char_t **argv)
{
    int num = atoi(nvram_safe_get("wan_num"));
#ifdef EZP_PROD_BRAND_ZYUS
    websWrite(wp, "%s", lang_translate(""));
#else
    if (num > 1)
        websWrite(wp, "%s", lang_translate("Load Balance"));
    else
        websWrite(wp, "%s", lang_translate("WAN Detect"));
#endif
    return 0;
}

int 
is_curr_selected(webs_t wp, int wan_num, char *find, int cur)
{
    int i;
    char *backup, name[TMP_LEN];
    for(i = 0; i < wan_num; i++) {
        if(i == cur) 
            break;
        snprintf(name, sizeof(name), "wan%d_backup", i);
        backup = websGetVar(wp, name, "");
        if(strlen(backup) == 0) {
            ezplib_get_attr_val("wan_lb_rule", i, "backup", name, sizeof(name),
                    EZPLIB_USE_CLI);
            backup = name;
        }
        if(!strcmp(find, backup))
            return 1;
    }
    return 0;
}

int
ej_lb_show_available(int eid, webs_t wp, int argc, char_t **argv)
{
    int i, idx;
    int wan_num = atoi(nvram_safe_get("wan_num"));
    char *mode, *backup, tmp[TMP_LEN], select[TMP_LEN];
    char buf1[TMP_LEN];
    if (ejArgs(argc, argv, "%d", &idx) < 1) {
        websDebugWrite(wp, "Insufficient args\n");
        return -1;
    }
    snprintf(tmp, sizeof(tmp), "wan%d_backup", idx);
    backup = websGetVar(wp, tmp, "");
    if(strlen(backup) == 0){
        ezplib_get_attr_val("wan_lb_rule", idx, "backup", buf1, sizeof(buf1),
                EZPLIB_USE_CLI);
        backup = buf1;
    }
    for (i = 0; i < wan_num; i++) {
        select[0] = '\0';
        snprintf(tmp, sizeof(tmp), "wan%d_mode", i);
        mode = websGetVar(wp, tmp, "");
        if(strlen(mode) == 0) {
            ezplib_get_attr_val("wan_lb_rule", i, "type", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            mode = tmp;
        }
        /* check if wan mode is backup */
        if(!strcmp(mode, "alwayson")) continue;

        /* if backup wan is selected by other wan */
        snprintf(tmp, sizeof(tmp), "wan%d", i);
        if(is_curr_selected(wp, wan_num, tmp, idx)) continue;

        /* if backup wan is current wan */
        if(i == idx) continue;

        if(!strcmp(backup, tmp))
            strcpy(select, "selected");
        else 
            strcpy(select, "");
#ifdef EZP_PROD_BRAND_PROX
        websWrite(wp, "<option value=\"wan%d\" %s>WAN%s</option>", 
                i, select, !i ? "(Ethernet)":"(USB Modem)" );
#else        
        websWrite(wp, "<option value=\"wan%d\" %s>WAN%d</option>", 
                i, select, i + 1);
#endif        
    }

    select[0] = '\0';
    if(!strcmp("none", backup))
        strcpy(select, "selected");
    else
        strcpy(select, "");

    websWrite(wp, "<option value=\"none\" %s>None</option>", select);
    return 0;
}
