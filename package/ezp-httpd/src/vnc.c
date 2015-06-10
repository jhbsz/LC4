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
    VNC_ENABLE = 0,
    VNC_VPORT,
    VNC_SPORT,
    VNC_IPADDR,
    VNC_MODE,
    VNC_EVENT_ENABLE,
    VNC_EVENT_IPADDR,
    VNC_EVENT_PORT,
};

static struct variable vnc_variables[] = {
    {longname: "VNC Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VNC Viewer Port", argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "VNC Server Port", argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "VNC IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "VNC Mode", argv:ARGV("0", "1", "2"), nullok: FALSE},
    {longname: "VNC Event Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VNC Event Server IP Address", argv:ARGV(""), nullok: FALSE},
    {longname: "VNC Event Server Port", argv:ARGV("1", "65535"), nullok: FALSE},
};

int
valid_vnc(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "vnc_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &vnc_variables[VNC_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*val== '0') {
        return TRUE;
    }

    /* Viwer port */
    snprintf(tmp, sizeof(tmp), "vnc_vport");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &vnc_variables[VNC_VPORT]) == FALSE) {
        return FALSE;
    }

    /* Server port */
    snprintf(tmp, sizeof(tmp), "vnc_sport");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &vnc_variables[VNC_SPORT]) == FALSE) {
        return FALSE;
    }

    /* VNC listen ipaddr */
    snprintf(tmp, sizeof(tmp), "vnc_ipaddr");
    val = websGetVar(wp, tmp, "");
    if (valid_subnet(wp, val, &vnc_variables[VNC_IPADDR]) == FALSE) {
        return FALSE;
    }

    /* VNC mode */
    snprintf(tmp, sizeof(tmp), "vnc_mode");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &vnc_variables[VNC_MODE]) == FALSE) {
        return FALSE;
    }

    /* VNC event enable */
    snprintf(tmp, sizeof(tmp), "vnc_event_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &vnc_variables[VNC_EVENT_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* VNC event server ipaddr */
    snprintf(tmp, sizeof(tmp), "vnc_event_ipaddr");
    val = websGetVar(wp, tmp, "");
    if (valid_ipaddr(wp, val, &vnc_variables[VNC_EVENT_IPADDR]) == FALSE) {
        return FALSE;
    }

    /* VNC event server port */
    snprintf(tmp, sizeof(tmp), "vnc_event_port");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &vnc_variables[VNC_EVENT_PORT]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_vnc(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];    
    char *enable, *vport, *sport, *mode, *event_enable, *event_ipaddr, 
         *event_port, *ipaddr;
    char *rule_set = "vnc_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "vnc_enable");
    enable = websGetVar(wp, tmp, "");

    if (*enable == '0') {
        ezplib_get_attr_val(rule_set, 0, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        if (strcmp(tmp, enable)) {
            config_preaction(&map, v, services, "NUM=0", ""); 
            ezplib_replace_attr(rule_set, 0, "enable", enable);
            change = 1;
        }
    } else {
        /* Viewer port */
        snprintf(tmp, sizeof(tmp), "vnc_vport");
        vport = websGetVar(wp, tmp, "");

        /* Server port */
        snprintf(tmp, sizeof(tmp), "vnc_sport");
        sport = websGetVar(wp, tmp, "");

        /* IP address */
        snprintf(tmp, sizeof(tmp), "vnc_ipaddr");
        ipaddr = websGetVar(wp, tmp, "");

        /* Mode */
        snprintf(tmp, sizeof(tmp), "vnc_mode");
        mode = websGetVar(wp, tmp, "");

        /* Event enable */
        snprintf(tmp, sizeof(tmp), "vnc_event_enable");
        event_enable = websGetVar(wp, tmp, "");

        /* Event ipaddr */
        snprintf(tmp, sizeof(tmp), "vnc_event_ipaddr");
        event_ipaddr = websGetVar(wp, tmp, "");

        /* Event port */
        snprintf(tmp, sizeof(tmp), "vnc_event_port");
        event_port = websGetVar(wp, tmp, "");

        /* Construct a rule tuple. */
        len = 
            snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s", enable, vport,
                    sport, ipaddr, mode, event_enable, event_ipaddr, 
                    event_port);

        if (len > TMP_LEN - 1) {
            return 0;
        }

        if (strcmp(tmp, nvram_safe_get(rule_set))) {
            config_preaction(&map, v, services, "NUM=0", ""); 
            nvram_set(rule_set, tmp);
            change = 1;
        }
    }

    if (change) {
        config_postaction(map, services, "NUM=0", "");
    }

    return change;
}
