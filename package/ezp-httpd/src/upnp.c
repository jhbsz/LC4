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
    UPNP_ENABLE = 0,
    UPNP_PORT,
    UPNP_EXTIF,
    UPNP_INIF,
};

static struct variable upnp_variables[] = {
    {longname: "UPNP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "UPNP Port", argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "UPNP External Interface", argv:ARGV("8"), nullok: FALSE},
    {longname: "UPNP Internal Interface", argv:ARGV("8"), nullok: FALSE},
};

int
valid_upnp(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "upnp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &upnp_variables[UPNP_ENABLE]) == FALSE) {
        return FALSE;
    }

    if (*val== '0') {
        return TRUE;
    }

    /* Port */
    snprintf(tmp, sizeof(tmp), "upnp_port");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &upnp_variables[UPNP_PORT]) == FALSE) {
        return FALSE;
    }

    /* External IF */
    snprintf(tmp, sizeof(tmp), "upnp_extif");
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &upnp_variables[UPNP_EXTIF]) == FALSE) {
        return FALSE;
    }

    /* Internal IF */
    snprintf(tmp, sizeof(tmp), "upnp_inif");
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &upnp_variables[UPNP_INIF]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_upnp(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];    
    char *enable, *extif, *inif, *port;
    char *rule_set = "upnp_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "upnp_enable");
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
        /* Port */
        snprintf(tmp, sizeof(tmp), "upnp_port");
        port = websGetVar(wp, tmp, "");

        /* External IF */
        snprintf(tmp, sizeof(tmp), "upnp_extif");
        extif = websGetVar(wp, tmp, "");

        /* Internal IF */
        snprintf(tmp, sizeof(tmp), "upnp_inif");
        inif = websGetVar(wp, tmp, "");

        /* Construct a rule tuple. */
        len = 
            snprintf(tmp, TMP_LEN, "^%s^0^%s^%s^%s", enable,
                    extif, inif, port);

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
