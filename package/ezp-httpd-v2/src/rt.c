#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cyutils.h>
#include "common.h"
#include "ezp.h"
#include "ezp-lib.h"

enum {
    RT_ENABLE = 0,
    RT_RULE_ENABLE,
    RT_RULE_NAME,
    RT_RULE_INIPADDR_START,
    RT_RULE_INIPADDR_END,
    RT_RULE_INIPADDR_RANGE,
    RT_RULE_EXTIPADDR_START,
    RT_RULE_EXTIPADDR_END,
    RT_RULE_EXTIPADDR_RANGE,
    RT_RULE_EXTPORT_START,
    RT_RULE_EXTPORT_END,
    RT_RULE_EXTPORT_RANGE,
    RT_RULE_PROTO,
    RT_RULE_EXTPORT_PROTO,
    RT_RULE_EXTIF,
    RT_RULE_ROUTING_TYPE,
    RT_RULE_GATEWAY_IPADDR,
};

static struct variable rt_variables[] = {
    {longname: "Routing Policy Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Routing Policy Rule Enable", argv:ARGV("0", "1"),
            nullok: FALSE},
    {longname: "Routing Policy Rule Name", argv:ARGV("20"), nullok: TRUE},
    {longname: "Routing Policy Rule Internal IP Start", argv:ARGV(""),
            nullok: TRUE},
    {longname: "Routing Policy Rule Internal IP End", argv:ARGV(""), 
            nullok: TRUE},
    {longname: "Routing Policy Rule Internal IP Range", argv:ARGV(""),
            nullok: FALSE},
    {longname: "Routing Policy Rule External IP Start", argv:ARGV(""),
            nullok: TRUE},
    {longname: "Routing Policy Rule External IP End", argv:ARGV(""), nullok: TRUE},
    {longname: "Routing Policy Rule External IP Range", argv:ARGV(""),
            nullok: FALSE},
    {longname: "Routing Policy Rule External Port Start", argv:ARGV("1","65535"),
            nullok: TRUE},
    {longname: "Routing Policy Rule External Port End", argv:ARGV("1","65535"),
            nullok: TRUE},
    {longname: "Routing Policy Rule External Port Range", argv:ARGV(""),
            nullok: FALSE},
    {longname: "Routing Policy Rule Protocol", argv:ARGV("tcp","udp","both"),
            nullok: TRUE},
    {longname: "Routing Policy Rule External Port/Protocol", argv:ARGV(""),
            nullok: FALSE},
    {longname: "Routing Policy Rule External Interface", argv:ARGV("wan", "lan"),
            nullok: TRUE},
    {longname: "Routing Policy Rule Routing Type", argv:ARGV("default","static"),
            nullok: TRUE},
    {longname: "Routing Policy Rule Gateway IP Address", argv:ARGV(""),
            nullok: FALSE},
};

int 
valid_rt(webs_t wp, char *value, struct variable *v)
{
    int i, nrule;
    char tmp[TMP_LEN];
    char *enable, *rule_enable, *name, *extif, *inipaddr_start, *inipaddr_end,
         *extipaddr_start, *extipaddr_end, *extport_start, *extport_end, *proto,
         *rule_num, *routing_type, *gateway_ipaddr;

    /* Enable of the whole forwarding mechanism. */
    snprintf(tmp, sizeof(tmp), "rt_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, 
                &rt_variables[RT_ENABLE]) == FALSE) {

        return FALSE;
    }

    if (*enable == '0') {
        return TRUE;
    }

    snprintf(tmp, sizeof(tmp), "rt_rule_num");
    rule_num = websGetVar(wp, tmp, "0");
    nrule = atoi(rule_num);
    for (i = 0; i < nrule; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_enable", i);
        rule_enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, rule_enable, 
                    &rt_variables[RT_RULE_ENABLE]) == FALSE) {

            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_name", i);
        name = websGetVar(wp, tmp, "");
        if (valid_name(wp, name, 
                    &rt_variables[RT_RULE_NAME]) == FALSE) {

            return FALSE;
        }

        /* Internal IP Start */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_inipaddr_start", i);
        inipaddr_start = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, inipaddr_start, 
                    &rt_variables[RT_RULE_INIPADDR_START]) == FALSE) {

            return FALSE;
        }

        /* Internal IP End */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_inipaddr_end", i);
        inipaddr_end = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, inipaddr_end, 
                    &rt_variables[RT_RULE_INIPADDR_END]) == FALSE) {

            return FALSE;
        }

        /**
         * Make sure Internal IP Start and Internal IP End indicate a IP range.
         */
        if (valid_ipaddr_range(wp, inipaddr_start, inipaddr_end,
                &rt_variables[RT_RULE_INIPADDR_RANGE]) == FALSE) {

            return FALSE;
        }

        /* External IP Start */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_extipaddr_start", i);
        extipaddr_start = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, extipaddr_start, 
                    &rt_variables[RT_RULE_EXTIPADDR_START]) == FALSE) {

            return FALSE;
        }

        /* External IP End */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_extipaddr_end", i);
        extipaddr_end = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, extipaddr_end, 
                    &rt_variables[RT_RULE_EXTIPADDR_END]) == FALSE) {

            return FALSE;
        }

        /**
         * Make sure External IP Start and External IP End indicate a IP range.
         */
        if (valid_ipaddr_range(wp, extipaddr_start, extipaddr_end,
                &rt_variables[RT_RULE_EXTIPADDR_RANGE]) == FALSE) {

            return FALSE;
        }

        /* Proto */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_proto", i);
        proto = websGetVar(wp, tmp, "");
        if (valid_choice(wp, proto, &rt_variables[RT_RULE_PROTO]) 
                == FALSE) {

            return FALSE;
        }

        if (*proto) {
            /* External Port Start */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_extport_start", i);
            extport_start = websGetVar(wp, tmp, "");
            if (valid_range(wp, extport_start, 
                        &rt_variables[RT_RULE_EXTPORT_START]) == FALSE) {

                return FALSE;
            }

            /* External Port End */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_extport_end", i);
            extport_end = websGetVar(wp, tmp, "");
            if (valid_range(wp, extport_end, 
                        &rt_variables[RT_RULE_EXTPORT_END]) == FALSE) {

                return FALSE;
            }

            /**
             * Make sure External Port Start and External Port End indicate a Port range.
             */
            if (valid_port_range(wp, extport_start, extport_end,
                    &rt_variables[RT_RULE_EXTPORT_RANGE]) == FALSE) {

                return FALSE;
            }
        }

        /* External IF */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_extif", i);
        extif = websGetVar(wp, tmp, "");
        if (valid_if(wp, extif, 
                    &rt_variables[RT_RULE_EXTIF]) == FALSE) {

            return FALSE;
        }
        
        /* Routing Type */
        snprintf(tmp, sizeof(tmp), "rt_rule%d_routing_type", i);
        routing_type = websGetVar(wp, tmp, "");
        if (valid_choice(wp, routing_type, 
                    &rt_variables[RT_RULE_ROUTING_TYPE]) == FALSE) {

            return FALSE;
        }
        
        /* Gateway is only applied on static routing. */
        if (!strcmp(routing_type, "static")) {
            /* Gateway IP Address */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_gateway_ipaddr", i);
            gateway_ipaddr = websGetVar(wp, tmp, "");
            if (valid_ipaddr(wp, gateway_ipaddr, 
                        &rt_variables[RT_RULE_GATEWAY_IPADDR]) == FALSE) {

                return FALSE;
            }
        }

    }
    return TRUE;
}

int 
save_rt(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *nv_rt_enable = "rt_enable";
    char *nv_rt_rule_num = "rt_rule_num";
    char *nv_rt_rule_max = "rt_rule_max";
    char *nv_rt_rule = "rt_rule";
    int i, nrule, change = 0, max;
    int64_t map = 0;
    char old_rule[TMP_LEN], rule[TMP_LEN], tmp[TMP_LEN];
    char all_rule[LONG_BUF_LEN];
    char *enable, *name, *extif, *inipaddr_start, *inipaddr_end,
         *extipaddr_start, *extipaddr_end, *extport_start, *extport_end,
         *proto, *routing_type, *gateway_ipaddr;

    /* Enable of the whole forwarding mechanism. */
    enable = websGetVar(wp, "rt_enable", "");
    if (strcmp(enable, nvram_safe_get(nv_rt_enable))) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set(nv_rt_enable, enable);
        change = 1;
    }


    if (*enable == '1') {
        /* the forwarding is enabled */
        snprintf(tmp, sizeof(tmp), "rt_rule_num");
        nrule = atoi(websGetVar(wp, tmp, "0"));
        max = atoi(nvram_safe_get(nv_rt_rule_max));
        nrule = (nrule >= max) ? max : nrule;

        snprintf(tmp, sizeof(tmp), "%d", nrule);
        if (nrule != atoi(nvram_safe_get(nv_rt_rule_num))) {
            /* In case the situation that the last rule is deleted but others are
             * kept the same and the check flow will treat it as 'nothing
             * changed'.
             */
            config_preaction(&map, v, s, "NUM=0", "");
            nvram_set(nv_rt_rule_num, tmp);
            change = 1;
        }

        all_rule[0]='\0';
        for (i = 0; i < nrule; i++) {
            /* Enable */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_enable", i);
            enable = websGetVar(wp, tmp, "");

            /* Name */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_name", i);
            name = websGetVar(wp, tmp, "");

            /* Internal IP Start */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_inipaddr_start", i);
            inipaddr_start = websGetVar(wp, tmp, "");

            /* Internal IP End */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_inipaddr_end", i);
            inipaddr_end  = websGetVar(wp, tmp, "");
            if (!*inipaddr_end) {
                inipaddr_end = inipaddr_start;
            }

            /* External IP Start */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_extipaddr_start", i);
            extipaddr_start = websGetVar(wp, tmp, "");

            /* External IP End */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_extipaddr_end", i);
            extipaddr_end = websGetVar(wp, tmp, "");
            if (!*extipaddr_end) {
                extipaddr_end = extipaddr_start;
            }

            /* External Port Start */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_extport_start", i);
            extport_start = websGetVar(wp, tmp, "");

            /* External Port End */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_extport_end", i);
            extport_end = websGetVar(wp, tmp, "");

            /* Protocol */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_proto", i);
            proto = websGetVar(wp, tmp, "");

            /* External IF */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_extif", i);
            extif = websGetVar(wp, tmp, "");

            /* Routing Type */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_routing_type", i);
            routing_type = websGetVar(wp, tmp, "");

            /* Gateway IP Address */
            snprintf(tmp, sizeof(tmp), "rt_rule%d_gateway_ipaddr", i);
            gateway_ipaddr = websGetVar(wp, tmp, "");

            snprintf(rule, sizeof(rule),
                     "%s^%s^%s^%s^%s^%s^%s^^^%s^%s^%s^%s^%s",
                     name, enable, inipaddr_start, inipaddr_end, extipaddr_start,
                     extipaddr_end, proto, extport_start, extport_end, extif,
                     routing_type, gateway_ipaddr);

            if (ezplib_get_rule(nv_rt_rule, i, old_rule, TMP_LEN) < 0) {
                /* i-th rule doesn't exist */
                config_preaction(&map, v, s, "NUM=0", "");
                ezplib_append_rule(nv_rt_rule, rule);
                change = 1;
            } else {
                if (strcmp(rule, old_rule)) {
                    config_preaction(&map, v, s, "NUM=0", "");
                    ezplib_replace_rule(nv_rt_rule, i, rule);
                    change = 1;
                }
            }
            if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", rule);
            else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, rule);
        }
        nvram_set(nv_rt_rule, all_rule);
    }

    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }

    return change;
}

