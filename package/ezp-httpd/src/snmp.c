#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include "assert.h"

#include "ezp.h"
#include "lang.h"
#include "ezp-lib.h"
#include "nvram_ezpacket.h"
#include "common.h"

// enable[0|1]^udpport^tcpport^community^vendor^sysname^location^contact^timeout^auth
enum {
    SNMP_ENABLE = 0,
    SNMP_UDPPORT,
    SNMP_TCPPORT,
    SNMP_COMMUNITY,
    SNMP_VENDOR,
    SNMP_SYSNAME,
    SNMP_LOCATION,
    SNMP_CONTACT,
    SNMP_TIMEOUT,
    SNMP_AUTH,
};

static struct variable snmp_variables[] = {
    {longname: "SNMP Enable",           argv:ARGV("0", "1")},
    {longname: "SNMP UDP Port",         argv:ARGV("1", "65535")},
    {longname: "SNMP TCP Port",         argv:ARGV("1", "65535")},
    {longname: "SNMP Community",        argv:ARGV("64")},
    {longname: "SNMP VENDOR",           argv:ARGV("64")},
    {longname: "SNMP System Name",      argv:ARGV("64"), nullok:TRUE},
    {longname: "SNMP Location",         argv:ARGV("64"), nullok:TRUE},
    {longname: "SNMP Contact",          argv:ARGV("64"), nullok:TRUE},
    {longname: "SNMP Update Timeout",   argv:ARGV("1", "10")},
    {longname: "SNMP v2C Enable",       argv:ARGV("0", "1")},
};
    
int
valid_snmp(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
printf(" valid \n");

    /* Enable */
    /* SNMP */
    snprintf(tmp, sizeof(tmp), "snmp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &snmp_variables[SNMP_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*val == '0') {

        return TRUE;
    }

    /*SNMP udp port */
    snprintf(tmp, sizeof(tmp), "snmp_udpport");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &snmp_variables[SNMP_UDPPORT]) == FALSE) {
        return FALSE;
    }

    /*SNMP tcp port */
    snprintf(tmp, sizeof(tmp), "snmp_tcpport");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &snmp_variables[SNMP_TCPPORT]) == FALSE) {
        return FALSE;
    }

    /*SNMP community */
    snprintf(tmp, sizeof(tmp), "snmp_community");
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &snmp_variables[SNMP_COMMUNITY]) == FALSE) {
        return FALSE;
    }

    /*SNMP sysname */
    snprintf(tmp, sizeof(tmp), "snmp_sysname");
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &snmp_variables[SNMP_SYSNAME]) == FALSE) {
        return FALSE;
    }

    /*SNMP location */
    snprintf(tmp, sizeof(tmp), "snmp_location");
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &snmp_variables[SNMP_LOCATION]) == FALSE) {
        return FALSE;
    }

    /*SNMP contact */
    snprintf(tmp, sizeof(tmp), "snmp_contact");
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &snmp_variables[SNMP_CONTACT]) == FALSE) {
        return FALSE;
    }

    /*SNMP timeout */
    snprintf(tmp, sizeof(tmp), "snmp_timeout");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &snmp_variables[SNMP_TIMEOUT]) == FALSE) {
        return FALSE;
    }

    /*SNMP auth */
    snprintf(tmp, sizeof(tmp), "snmp_auth");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &snmp_variables[SNMP_AUTH]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_snmp(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];
    char *snmp_enable;
    char *snmp_tcpport;
    char *snmp_udpport;
    char *snmp_community;
    char snmp_vendor[TMP_LEN]; 
    char *snmp_sysname;
    char *snmp_location;
    char *snmp_contact;
    char *snmp_timeout; 
    char *snmp_auth;
    char *rule_set = "snmp_rule";
    int len, change = 0;
    int64_t map = 0;

    /* SNMP Enable */
    snprintf(tmp, sizeof(tmp), "snmp_enable");
    snmp_enable = websGetVar(wp, tmp, "");

    if (*snmp_enable == '0') {
        ezplib_get_attr_val(rule_set, 0, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        if (strcmp(tmp, snmp_enable)) {
            config_preaction(&map, v, services, "", "");
            ezplib_replace_attr(rule_set, 0, "enable", snmp_enable);
            change = 1;
        }
    } else {
        /* SNMP UDP Port */
        snprintf(tmp, sizeof(tmp), "snmp_udpport");
        snmp_udpport = websGetVar(wp, tmp, "");

        /* SNMP TCP Port */
        snprintf(tmp, sizeof(tmp), "snmp_tcpport");
        snmp_tcpport = websGetVar(wp, tmp, "");

        /* SNMP Community */
        snprintf(tmp, sizeof(tmp), "snmp_community");
        snmp_community = websGetVar(wp, tmp, "");

        /* SNMP Vendor */
        ezplib_get_attr_val(rule_set, 0, "vendor", snmp_vendor, TMP_LEN, EZPLIB_USE_CLI);

        /* SNMP Sysname */
        snprintf(tmp, sizeof(tmp), "snmp_sysname");
        snmp_sysname = websGetVar(wp, tmp, "");

        /* SNMP Location */
        snprintf(tmp, sizeof(tmp), "snmp_location");
        snmp_location = websGetVar(wp, tmp, "");


        /* SNMP Contact */
        snprintf(tmp, sizeof(tmp), "snmp_contact");
        snmp_contact = websGetVar(wp, tmp, "");

        /* SNMP Timeout*/
        snprintf(tmp, sizeof(tmp), "snmp_timeout");
        snmp_timeout = websGetVar(wp, tmp, "");

        /* SNMP Auth */
        snprintf(tmp, sizeof(tmp), "snmp_auth");
        snmp_auth = websGetVar(wp, tmp, "");

        /* Construct a rule tuple. */
        len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s", 
                snmp_enable, snmp_udpport, snmp_tcpport, snmp_community, snmp_vendor, 
                snmp_sysname, snmp_location, snmp_contact, snmp_timeout, snmp_auth);

        if (len > TMP_LEN - 1) {
            return 0;
        }

        if (strcmp(tmp, nvram_safe_get(rule_set))) {
            config_preaction(&map, v, services, "", ""); 
            ezplib_replace_rule(rule_set, 0, tmp);
            config_postaction(map, services, "", "");
            change = 1;
        }
    }

    return change;
}
