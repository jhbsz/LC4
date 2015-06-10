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
    NTP_ENABLE = 0,
    NTP_TYPE,
    NTP_POOL,
    NTP_IPADDR,
    NTP_DAYLIGHT,
    NTP_ZONE,
};

static struct variable ntp_variables[] = {
    {longname: "NTP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "NTP Type", argv:ARGV("pool", "ipaddr"), nullok: FALSE},
    {longname: "NTP Server Pool", argv:ARGV("pool.ntp.org", 
            "asia.pool.ntp.org", "europe.pool.ntp.org", 
            "north-america.pool.ntp.org", "south-america.pool.ntp.org", 
            "africa.pool.ntp.org"), nullok: FALSE},
    {longname: "NTP IP Address", argv:ARGV("48"), nullok: FALSE},
    {longname: "NTP Daylight Saving", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "NTP Zone", argv:ARGV("64"), nullok: FALSE},
};

int
valid_ntp(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "ntp_enable");
    val = websGetVar(wp, tmp, "");

    if (valid_choice(wp, val, &ntp_variables[NTP_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*val == '0') {
        return TRUE;
    }

    /* Type */
    snprintf(tmp, sizeof(tmp), "ntp_type");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &ntp_variables[NTP_TYPE]) == FALSE) {
        return FALSE;
    }

    if (!strcmp(val, "pool")) {
        /* Pool */
        snprintf(tmp, sizeof(tmp), "ntp_pool");
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &ntp_variables[NTP_POOL]) == FALSE) {
            return FALSE;
        }
    } else {
        /* IP */
        snprintf(tmp, sizeof(tmp), "ntp_ipaddr");
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &ntp_variables[NTP_IPADDR]) == FALSE) {
            return FALSE;
        }
    }


    /* Zone */
    snprintf(tmp, sizeof(tmp), "ntp_zone");
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &ntp_variables[NTP_ZONE]) == FALSE) {
        return FALSE;
    }
    /* Daylight Saving */
    snprintf(tmp, sizeof(tmp), "ntp_daylight");
    val = websGetVar(wp, tmp, "");

    if (valid_choice(wp, val, &ntp_variables[NTP_DAYLIGHT]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_ntp(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN], pool_tmp[TMP_LEN], ipaddr_tmp[TMP_LEN];
    char *enable, *type, *zone, *daylight;
    char *rule_set = "ntp_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "ntp_enable");
    enable = websGetVar(wp, tmp, "");

    if (*enable == '0') {
        ezplib_get_attr_val(rule_set, 0, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        if (strcmp(tmp, enable)) {
            config_preaction(&map, v, s, "NUM=0", ""); 
            ezplib_replace_attr(rule_set, 0, "enable", enable);
            change = 1;
        }
    } else {
        /* Type */
        snprintf(tmp, sizeof(tmp), "ntp_type");
        type = websGetVar(wp, tmp, "");

        if (!strcmp(type, "pool")) {
            /* Pool */
            snprintf(tmp, sizeof(tmp), "ntp_pool");
            strcpy(pool_tmp, websGetVar(wp, tmp, ""));

            ezplib_get_attr_val(rule_set, 0, "ipaddr", ipaddr_tmp, TMP_LEN,
                    EZPLIB_USE_CLI);
        } else {
            /* IP Address */
            ezplib_get_attr_val(rule_set, 0, "pool", pool_tmp, TMP_LEN, 
                    EZPLIB_USE_CLI);

            snprintf(tmp, sizeof(tmp), "ntp_ipaddr");
            strcpy(ipaddr_tmp, websGetVar(wp, tmp, ""));
        }
        /* Daylight Saving */
        snprintf(tmp, sizeof(tmp), "ntp_daylight");
        daylight = websGetVar(wp, tmp, "");

        /* Zone */
        snprintf(tmp, sizeof(tmp), "ntp_zone");
        zone = websGetVar(wp, tmp, "");

        /* Construct a rule tuple. */
        len = 
            snprintf(tmp, TMP_LEN, "^%s^%s^%s^%s^%s^%s", enable, type,
                    pool_tmp, ipaddr_tmp, zone, daylight);

        if (len > TMP_LEN - 1) {
            return 0;
        }

        if (strcmp(tmp, nvram_safe_get(rule_set))) {
            config_preaction(&map, v, s, "NUM=0", ""); 
            nvram_set(rule_set, tmp);
            change = 1;
        }
    }

    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }
    return change;
}
