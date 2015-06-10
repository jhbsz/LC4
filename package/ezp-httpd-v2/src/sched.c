#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <assert.h>

#include "common.h"
#include "ezp.h"
#include "ezp-lib.h"
#include "nvram_ezpacket.h"
#include "lang.h"

enum {
    SCHED_ENABLE = 0,
    SCHED_RULE_NAME,
    SCHED_RULE_ENABLE,
    SCHED_RULE_USED,
    SCHED_RULE_START_HOUR,
    SCHED_RULE_START_MIN,
    SCHED_RULE_STOP_HOUR,
    SCHED_RULE_STOP_MIN,
    SCHED_RULE_SUN,
    SCHED_RULE_MON,
    SCHED_RULE_TUE,
    SCHED_RULE_WED,
    SCHED_RULE_THU,
    SCHED_RULE_FRI,
    SCHED_RULE_SAT
};

static struct variable sched_variables[] = {
    {longname: "Schedule Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Schedule Rule Name", argv:ARGV("30"), nullok: FALSE},
    {longname: "Schedule Rule Enable", argv:ARGV(""), nullok: FALSE},
    {longname: "Schedule Rule Used", argv:ARGV("")},
    {longname: "Schedule Rule Start Hour", argv:ARGV("0", "24"), nullok: FALSE},
    {longname: "Schedule Rule Start Minute", argv:ARGV("0", "59"),
            nullok: FALSE},
    {longname: "Schedule Rule Stop Hour", argv:ARGV("0", "24"), nullok: FALSE},
    {longname: "Schedule Rule Stop Minute", argv:ARGV("0", "59"),
            nullok: FALSE},
    {longname: "Schedule Rule SUN", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Schedule Rule MON", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Schedule Rule TUE", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Schedule Rule WED", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Schedule Rule THU", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Schedule Rule FRI", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Schedule Rule SAT", argv:ARGV("0", "1"), nullok: FALSE}
};

int
valid_sched(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;
    int i, nrule;

    snprintf(tmp, sizeof(tmp), "sched_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    for (i = 0; i < nrule; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_enable", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &sched_variables[SCHED_RULE_ENABLE])) {
            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_name", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, 
                    &sched_variables[SCHED_RULE_NAME]) == FALSE) {

            return FALSE;
        }

        /* Start Hour */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_start_hour", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_START_HOUR]) == FALSE) {

            return FALSE;
        }

        /* Start Minute */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_start_min", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_START_MIN]) == FALSE) {

            return FALSE;
        }

        /* Stop Hour */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_stop_hour", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_STOP_HOUR]) == FALSE) {

            return FALSE;
        }

        /* Start Minute */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_stop_min", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_STOP_MIN]) == FALSE) {

            return FALSE;
        }

        /* Sunday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_sun", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_SUN]) == FALSE) {

            return FALSE;
        }

        /* Monday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_mon", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_MON]) == FALSE) {

            return FALSE;
        }

        /* Tuesday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_tue", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_TUE]) == FALSE) {

            return FALSE;
        }

        /* Wedesday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_wed", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_WED]) == FALSE) {

            return FALSE;
        }

        /* Thursday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_thu", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_THU]) == FALSE) {

            return FALSE;
        }

        /* Friday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_fri", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_FRI]) == FALSE) {

            return FALSE;
        }

        /* Saturday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_sat", i);
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, 
                    &sched_variables[SCHED_RULE_SAT]) == FALSE) {

            return FALSE;
        }
    }

    return TRUE;
}

int
save_sched(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *rule_set = "sched_rule";
    int i, nrule, change = 0, max;
    int64_t map = 0;
    char tmp[LONG_BUF_LEN], new[LONG_BUF_LEN], old[LONG_BUF_LEN];
    char all_rule[LONG_BUF_LEN];
    char *name, *enable, *used, *start_hour, *start_min, *stop_hour,
         *stop_min, *sun, *mon, *tue, *wed, *thu, *fri, *sat;

    /* the forwarding is enabled */
    snprintf(tmp, sizeof(tmp), "sched_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    max = atoi(nvram_safe_get("sched_rule_max"));
    nrule = (nrule >= max) ? max : nrule;
    
    if (nrule != atoi(nvram_safe_get("sched_rule_num"))) {
        /* In case the situation that the last rule is deleted but others are
         * kept the same and the check flow will treat it as 'nothing
         * changed'.
         */
        snprintf(tmp, sizeof(tmp), "%d", nrule);
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set("sched_rule_num", tmp);
        change = 1;
    }

    all_rule[0]='\0';
    for (i = 0; i < nrule; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        /* Name */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_name", i);
        name = websGetVar(wp, tmp, "");

        /* Used */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_used", i);
        used = websGetVar(wp, tmp, "");

        /* Start Hour */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_start_hour", i);
        start_hour = websGetVar(wp, tmp, "");

        /* Start Minute */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_start_min", i);
        start_min = websGetVar(wp, tmp, "");

        /* Stop Hour */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_stop_hour", i);
        stop_hour = websGetVar(wp, tmp, "");

        /* Stop Minute */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_stop_min", i);
        stop_min = websGetVar(wp, tmp, "");

        /* Sunday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_sun", i);
        sun = websGetVar(wp, tmp, "");

        /* Monday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_mon", i);
        mon = websGetVar(wp, tmp, "");

        /* Tuesday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_tue", i);
        tue = websGetVar(wp, tmp, "");

        /* Wednesday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_wed", i);
        wed = websGetVar(wp, tmp, "");

        /* Thursday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_thu", i);
        thu = websGetVar(wp, tmp, "");

        /* Friday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_fri", i);
        fri = websGetVar(wp, tmp, "");

        /* Saturday */
        snprintf(tmp, sizeof(tmp), "sched_rule%d_sat", i);
        sat = websGetVar(wp, tmp, "");

        snprintf(new, sizeof(new),
                 "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
                 name, enable, used, start_hour, start_min, stop_hour,
                 stop_min, sun, mon, tue, wed, thu, fri, sat);
        if (ezplib_get_rule(rule_set, i, old, TMP_LEN) < 0) {
            /* i-th rule doesn't exist */
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_append_rule(rule_set, new);
            change = 1;
        } else {
            if (strcmp(new, old)) {
                config_preaction(&map, v, s, "NUM=0", "");
                ezplib_replace_rule(rule_set, i, new);
                change = 1;
            }
        }
        if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", new);
        else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, new);
    }
    nvram_set(rule_set, all_rule);

    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }

    return change;
}

void
schedule_cgi(webs_t wp)
{
    char tmp[SHORT_BUF_LEN], rule[LONG_BUF_LEN];
    char *ptr;
    int nrule = atoi(nvram_safe_get("sched_rule_num"));
    int i, j, len = 0;

    char *days[][2] = {
        {"sun", "Sun"},
        {"mon", "Mon"},
        {"tue", "Tue"},
        {"wed", "Wed"},
        {"thu", "Thu"},
        {"fri", "Fri"},
        {"sat", "Sat"}};
            
    for (i = 0; i < nrule; i++) {
        ptr = &rule[0];
        ezplib_get_attr_val("sched_rule", i, "name", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len += snprintf(ptr, sizeof(rule), "%s=", tmp);
        ptr = &rule[0] + len;

        ezplib_get_attr_val("sched_rule", i, "start_hour", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len += snprintf(ptr, sizeof(rule) - len, "%s:", tmp);
        ptr = &rule[0] + len;
        
        ezplib_get_attr_val("sched_rule", i, "start_min", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len += snprintf(ptr, sizeof(rule) - len, "%s - ", tmp);
        ptr = &rule[0] + len;
        
        ezplib_get_attr_val("sched_rule", i, "stop_hour", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len += snprintf(ptr, sizeof(rule) - len, "%s:", tmp);
        ptr = &rule[0] + len;
        
        ezplib_get_attr_val("sched_rule", i, "stop_min", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len += snprintf(ptr, sizeof(rule) - len, "%s (", tmp);
        ptr = &rule[0] + len;
        
        for (j = 0; j < 7; j++) {
            ezplib_get_attr_val("sched_rule", i, days[j][0], tmp, sizeof(tmp),
                   EZPLIB_USE_CLI); 
            if (tmp[0] == '1') {
                len +=
                    snprintf(ptr, sizeof(rule) - len, "%s, ",
                        lang_translate(days[j][1]));
                ptr = &rule[0] + len;
            }    
        }
        ptr -= 2;   // Remove the last , and white space
        *ptr++ = ')';
        ptr = '\0';

        websWrite(wp, rule);
    }    
}
