#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include "assert.h"

#include "common.h"
#include "ezp-lib.h"
#include "ezp.h"

enum {
    CRON_ENABLE = 0,
    CRON_NTP_ENABLE,
    CRON_NTP_INTERVAL,
    CRON_REBOOT_INTERVAL,
    CRON_REBOOT_HOUR,
};

static struct variable cron_variables[] = {
    {longname: "CRON Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "CRON NTP Client Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "CRON NTP Client Interval", argv:ARGV("32"), nullok: FALSE},
    {longname: "Reboot Interval", argv:ARGV("5","43200"), nullok: TRUE},
    {longname: "Daily Reboot", argv:ARGV("32"), nullok: FALSE},
};

int
valid_cron(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *val;

    if (!strcmp(value, "ntp")) {
        /* NTP Cron is under the situation of NTP Enable */
        snprintf(tmp, sizeof(tmp), "ntp_enable");
        val = websGetVar(wp, tmp, "");
        if (*val == '0') {
            return TRUE;
        }
        
        /* NTP Enable */
        snprintf(tmp, sizeof(tmp), "cron_ntp_enable");
        val = websGetVar(wp, tmp, "");

        if (valid_choice(wp, val, &cron_variables[CRON_NTP_ENABLE]) == FALSE) {
            return FALSE;
        }

        if (*val == '0') {
            return TRUE;
        }

        /* NTP Interval */
        snprintf(tmp, sizeof(tmp), "cron_ntp_interval");
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &cron_variables[CRON_NTP_INTERVAL]) == FALSE){
            return FALSE;
        }

        /* Reboot Interval */
        snprintf(tmp, sizeof(tmp), "cron_reboot_interval");
        val = websGetVar(wp, tmp, "");
        if (valid_range(wp, val, &cron_variables[CRON_REBOOT_INTERVAL]) == FALSE){
            return FALSE;
        }

        /* Hour */
        snprintf(tmp, sizeof(tmp), "cron_reboot_hour");
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &cron_variables[CRON_REBOOT_HOUR]) == FALSE){
            return FALSE;
        }

    } else {
        return FALSE;
    }

    return TRUE;
}

int
save_cron(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN], cron_ntp[TMP_LEN], cron_reboot[TMP_LEN];
    char tmp_ddns_countdown[TMP_LEN];
    char *ntp_enable, *ntp_interval, *reboot_interval, *reboot_hour;
    char *rule_set = "cron_rule", *ddns_countdown;
    int len, change = 0;
    int64_t map = 0;


    /* NTP */
    if (!strcmp(value, "ntp")) {
        /* NTP Enable */
        snprintf(tmp, sizeof(tmp), "cron_ntp_enable");
        ntp_enable = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "cron_ntp_interval");
        ntp_interval = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "cron_reboot_interval");
        reboot_interval = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "cron_reboot_hour");
        reboot_hour = websGetVar(wp, tmp, "");

        /* DDNS countdown */
        ezplib_get_attr_val("cron_rule", 0, "ddns_countdown", tmp_ddns_countdown, sizeof(tmp_ddns_countdown), EZPLIB_USE_CLI);
        ddns_countdown=tmp_ddns_countdown;

        /* Construct a ntp cron string. */
        len = snprintf(cron_ntp, TMP_LEN, "%s^%s", ntp_enable, ntp_interval);

        if (len > TMP_LEN - 1) {
            return 0;
        }
        /* Construct a reboot cron string. */
        len = snprintf(cron_reboot, TMP_LEN, "%s^%s^%s^%s", reboot_interval, reboot_interval, reboot_hour, ddns_countdown);

        if (len > TMP_LEN - 1) {
            return 0;
        }

        /* cron_rule */
        len = snprintf(tmp, TMP_LEN, "1^%s^%s", cron_ntp, cron_reboot);

        if (len > TMP_LEN - 1) {
            return 0;
        }

        if (strcmp(tmp, nvram_safe_get(rule_set))) {
            config_preaction(&map, v, services, "NUM=0", ""); 
            nvram_set(rule_set, tmp);
            config_postaction(map, services, "", "");
            change = 1;
        }

    }

    return change;
}
