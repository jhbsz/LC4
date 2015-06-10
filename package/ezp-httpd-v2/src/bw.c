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
#include "lang.h"
#include "ezp-lib.h"
#include "nvram_ezpacket.h"

enum {
    _BW_ENABLE = 0,
    _BW_BW,
    _BW_RESV,
    _BW_DL,
    _BW_UL
};
enum {
    SBM_RULE_NAME = 0,
    SBM_RULE_ENABLE,
    SBM_RULE_INIPADDR_START,
    SBM_RULE_INIPADDR_END,
    SBM_RULE_INIPADDR_RANGE,
    SBM_RULE_PROTO,
    SBM_RULE_EXTPORT_START,
    SBM_RULE_EXTPORT_END,
    SBM_RULE_EXTPORT_RANGE,
    SBM_RULE_EXTIF,
    SBM_RULE_BW_ALLOC,
    SBM_RULE_BW_RATIO,
    SBM_RULE_BW_UPLOAD,
    SBM_RULE_BW_DOWNLOAD,
    SBM_RULE_EXCESS,
    SBM_RULE_EXCESS_RATIO,
    SBM_RULE_EXCESS_UPLOAD,
    SBM_RULE_EXCESS_DOWNLOAD,
    SBM_NUM
};

enum {
    DBM_RULE_NAME = 0,
    DBM_RULE_ENABLE,
    DBM_RULE_INIPADDR_START,
    DBM_RULE_INIPADDR_END,
    DBM_RULE_INIPADDR_RANGE,
    DBM_NUM
};

static struct variable bw_variables[] = {
    {longname: "BW Enable", argv:ARGV("0", "1")},
    {longname: "BW Bandwidth Type", argv:ARGV(BW_TYPE), nullok: FALSE},
    {longname: "BW Reserved Bandwidth", argv:ARGV("0", "100"), nullok: FALSE},
    {longname: "BW Download", argv:ARGV(BW_DOWNLOAD_MIN, BW_DOWNLOAD_MAX), 
                nullok: FALSE}, 
    {longname: "BW Upload", argv:ARGV(BW_UPLOAD_MIN, BW_UPLOAD_MAX),
                nullok: FALSE}
};

static struct variable sbm_variables[] = {
    {longname: "SBM Rule Name", argv:ARGV("20"), nullok: TRUE},
    {longname: "SBM Rule Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "SBM Rule Internal IP Start", argv:ARGV(""), nullok: TRUE},
    {longname: "SBM Rule Internal IP End", argv:ARGV(""), nullok: TRUE},
    {longname: "SBM Rule Internal IP Range", argv:ARGV(""), nullok: TRUE},
    {longname: "SBM Rule Protocol", argv:ARGV("tcp","udp"), nullok: TRUE},
    {longname: "SBM Rule Port Start", argv:ARGV("1","65535"), nullok: TRUE},
    {longname: "SBM Rule Port End", argv:ARGV("1","65535"), nullok: TRUE},
    {longname: "SBM Rule Port Range", argv:ARGV("1","65535"), nullok: TRUE},
    {longname: "SBM Rule External Interface", argv:ARGV("wan"), nullok: TRUE},
#ifdef EZP_PROD_BRAND_PLANET
    {longname: "SBM Rule Allocation", argv:ARGV("ratio", "group", "bw"), nullok: FALSE},
#else
    {longname: "SBM Rule Allocation", argv:ARGV("ratio", "bw"), nullok: FALSE},
#endif
    {longname: "SBM Rule Ratio", argv:ARGV("0", "100"), nullok: FALSE},
    {longname: "SBM Rule Upload", argv:ARGV(""), nullok: FALSE},
    {longname: "SBM Rule Download", argv:ARGV(""), nullok: FALSE},
    {longname: "SBM Excess", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "SBM Excess Ratio", argv:ARGV("0", "100"), nullok: FALSE},
    {longname: "SBM Excess UPLOAD", argv:ARGV(""), nullok: FALSE},
    {longname: "SBM Excess DOWNLOAD", argv:ARGV(""), nullok: FALSE},
    /* We limit the number of SBM rule. */
    {longname: "SBM NUM", argv:ARGV("0", BW_SBM_RULE_MAX), nullok: FALSE}
};

static struct variable dbm_variables[] = {
    {longname: "DBM Rule Name", argv:ARGV("20"), nullok: TRUE},
    {longname: "DBM Rule Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "DBM Rule Internal IP Start", argv:ARGV(""), nullok: TRUE},
    {longname: "DBM Rule Internal IP End", argv:ARGV(""), nullok: TRUE},
    {longname: "DBM Rule Internal IP Range", argv:ARGV(""), nullok: TRUE},
    /* We limit the total used DBM IP number. */
    {longname: "DBM NUM", argv:ARGV("0", BW_DBM_RULE_MAX), nullok: FALSE}
};

int
_valid_bw_sbm(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val, *inipaddr_start, *inipaddr_end, *proto, *extport_start, 
         *extport_end, *alloc, *enable, *num;
    int i;

    snprintf(tmp, sizeof(tmp), "bw_sbm_rule_num");
    if(nvram_get("bw_sbm_rule_max")) {
        sbm_variables[SBM_NUM].argv[1] = nvram_get("bw_sbm_rule_max");
    }
    num = websGetVar(wp, tmp, "0");
    if (valid_range(wp, num, &sbm_variables[SBM_NUM]) == FALSE) {
        return FALSE;
    }
    for (i = 0; i < atoi(num); i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, &sbm_variables[SBM_RULE_ENABLE]) == FALSE) {

            return FALSE;
        }

        /* Name */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_name", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &sbm_variables[SBM_RULE_NAME]) == FALSE) {

            return FALSE;
        }

        /* Internal IP Start */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_inipaddr_start", i);
        inipaddr_start = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, inipaddr_start, 
                    &sbm_variables[SBM_RULE_INIPADDR_START]) == FALSE) {

            return FALSE;
        }

        /* Internal IP End */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_inipaddr_end", i);
        inipaddr_end = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, inipaddr_end, 
                    &sbm_variables[SBM_RULE_INIPADDR_END]) == FALSE) {

            return FALSE;
        }

        if (valid_ipaddr_range(wp, inipaddr_start, inipaddr_end,
                &sbm_variables[SBM_RULE_INIPADDR_RANGE]) == FALSE) {

            return FALSE;
        }

        /* Proto */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_proto", i);
        proto = websGetVar(wp, tmp, "");
        if (valid_choice(wp, proto, 
                    &sbm_variables[SBM_RULE_PROTO]) == FALSE) {

            return FALSE;
        }

        if (*proto) {
            /* External Port Start */
            snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_port_start", i);
            extport_start = websGetVar(wp, tmp, "");
            if (valid_range(wp, extport_start, 
                        &sbm_variables[SBM_RULE_EXTPORT_START]) == FALSE) {

                return FALSE;
            }

            /* External Port End */
            snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_port_end", i);
            extport_end = websGetVar(wp, tmp, "");
            if (valid_range(wp, extport_end, 
                        &sbm_variables[SBM_RULE_EXTPORT_END]) == FALSE) {

                return FALSE;
            }

            if (valid_port_range(wp, extport_start, extport_end,
                    &sbm_variables[SBM_RULE_EXTPORT_RANGE]) == FALSE) {

                return FALSE;
            }
        }

        /* Direction */

        /* External IF */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_extif", i);
        val = websGetVar(wp, tmp, "");
        if (valid_if(wp, val, &sbm_variables[SBM_RULE_EXTIF]) == FALSE) {

            return FALSE;
        }

        /* Allcoation Way */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_bw_alloc", i);
        alloc = websGetVar(wp, tmp, "");
        if (valid_choice(wp, alloc, 
                    &sbm_variables[SBM_RULE_BW_ALLOC]) == FALSE) {
            return FALSE;
        }

        if (!strcmp(alloc, "ratio")) {
            /* BW Ratio */
            snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_bw_ratio", i);
            val = websGetVar(wp, tmp, "");
            if (valid_range(wp, val, 
                        &sbm_variables[SBM_RULE_BW_RATIO]) == FALSE) {

                return FALSE;
            }
#ifdef EZP_PROD_BRAND_PLANET
        } else if (!strcmp(alloc, "group")) {
            /* BW Group */
#endif
        } else {
            /* BW Upload */
            snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_bw_upload", i);
            val = websGetVar(wp, tmp, "");
            if (valid_number(wp, val, 
                        &sbm_variables[SBM_RULE_BW_UPLOAD]) == FALSE) {

                return FALSE;
            }

#ifndef EZP_PROD_BRAND_PLANET
            /* BW Download */
            snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_bw_download", i);
            val = websGetVar(wp, tmp, "");
            if (valid_number(wp, val, 
                        &sbm_variables[SBM_RULE_BW_DOWNLOAD]) == FALSE) {

                return FALSE;
            }
#endif
        }

        /* Schedule */

        /* Excess */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_excess", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &sbm_variables[SBM_RULE_EXCESS]) == FALSE) {
            return FALSE;
        }

        if (*val == '1') {
            if (!strcmp(alloc, "ratio")) {
                /* Excess Ratio */
                snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_excess_ratio", i);
                val = websGetVar(wp, tmp, "");
                if (valid_range(wp, val, 
                            &sbm_variables[SBM_RULE_EXCESS_RATIO]) == FALSE) {

                    return FALSE;
                }
#ifdef EZP_PROD_BRAND_PLANET
            } else if (!strcmp(alloc, "group")) {
                /* Excess Group */
#endif
            } else {
                /* Excess Upload */
                snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_excess_upload", i);
                val = websGetVar(wp, tmp, "");
                if (valid_number(wp, val, 
                            &sbm_variables[SBM_RULE_EXCESS_UPLOAD]) == FALSE) {

                    return FALSE;
                }

#ifndef EZP_PROD_BRAND_PLANET
                /* Excess Download */
                snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_excess_download", i);
                val = websGetVar(wp, tmp, "");
                if (valid_number(wp, val, 
                            &sbm_variables[SBM_RULE_EXCESS_DOWNLOAD]) == FALSE) {

                    return FALSE;
                }
#endif
            }
        }
    }
    return TRUE;
}

int
_valid_bw_dbm(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val, *enable, *inipaddr_start, *inipaddr_end;
    int num, i, nb_ip_start, nb_ip_end, total_dbm_num = 0;

    snprintf(tmp, sizeof(tmp), "bw_dbm_rule_num");
    if(nvram_get("bw_dbm_rule_max")) {
        dbm_variables[DBM_NUM].argv[1] = nvram_get("bw_dbm_rule_max");
    }
    num = atoi(websGetVar(wp, tmp, "0"));
    for (i = 0; i < num; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "bw_dbm_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");
        if (valid_choice(wp, enable, &dbm_variables[DBM_RULE_ENABLE]) == FALSE) {

            return FALSE;
        }
        
        /* Name */
        snprintf(tmp, sizeof(tmp), "bw_dbm_rule%d_name", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &dbm_variables[DBM_RULE_NAME]) == FALSE) {

            return FALSE;
        }

        /* Internal IP Start */
        snprintf(tmp, sizeof(tmp), "bw_dbm_rule%d_inipaddr_start", i);
        inipaddr_start = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, inipaddr_start, 
                    &dbm_variables[DBM_RULE_INIPADDR_START]) == FALSE) {

            return FALSE;
        }

        /* Internal IP End */
        snprintf(tmp, sizeof(tmp), "bw_dbm_rule%d_inipaddr_end", i);
        inipaddr_end = websGetVar(wp, tmp, "");
        if (valid_ipaddr(wp, inipaddr_end, 
                    &dbm_variables[DBM_RULE_INIPADDR_END]) == FALSE) {

            return FALSE;
        }

        if (valid_ipaddr_range(wp, inipaddr_start, inipaddr_end,
                &dbm_variables[DBM_RULE_INIPADDR_RANGE]) == FALSE) {

            return FALSE;
        }

        /* DBM Num */
        /* If 'enable' is '0', the rule is invalid. */
        if (*inipaddr_start && *enable == '1') {
            nb_ip_start = (int)ntohl(inet_addr(inipaddr_start));
            nb_ip_end = (int)ntohl(inet_addr(inipaddr_end));
            total_dbm_num += (nb_ip_end - nb_ip_start + 1);
        }

        snprintf(tmp, sizeof(tmp), "%d", total_dbm_num);
        if (valid_range(wp, tmp, &dbm_variables[DBM_NUM]) == FALSE) {
            return FALSE;
        }
    }

    return TRUE;
}

int
valid_bw(webs_t wp, char *value, struct variable *v)
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
    
    val = websGetVar(wp, "bw_enable", "");
#ifndef EZP_PROD_BRAND_PLANET
    /* If hardware NAT is enabled, we can not start iDBM. */
    ezplib_get_attr_val("nat_rule", 0, "hw_enable", tmp,
                            sizeof(tmp), EZPLIB_USE_CLI);
#endif
    if (*val == '0') {
        /* Disable DBM */
        return TRUE;
    }
#ifndef EZP_PROD_BRAND_PLANET
    else if ((*val == '1') && (!strcmp(tmp, "1")) ){
        websDebugWrite(wp, "%s.<br> %s.<br>",
#ifdef EZP_PROD_BRAND_PROX
                lang_translate("Start Bandwidth Management failed"),
#else
                lang_translate("Start iDBM failed"),
#endif
                lang_translate("Need to turn off Hardware Accelerator first"));
        return FALSE;
    }
#endif

    for (i = 0; i < num; i++) {
        /* BW */
        snprintf(tmp, sizeof(tmp), "wan%d_bw_mode", i);
        val = websGetVar(wp, tmp, "");
        if (!strcmp(val, "auto")) {
            /* Type */
#ifdef EZP_PROD_BRAND_ZYUS
            /* Download */
            snprintf(tmp, sizeof(tmp), "wan%d_bw_dl", i);
            val = websGetVar(wp, tmp, "");
            if (!strcmp(val, "Custom")) {
                snprintf(tmp, sizeof(tmp), "wan%d_bw_dl_custom", i);
                val = websGetVar(wp, tmp, "");
            }
            if (valid_range(wp, val, &bw_variables[_BW_DL]) == FALSE) {
                return FALSE;
            }

            /* Upload
             * */
            snprintf(tmp, sizeof(tmp), "wan%d_bw_ul", i);
            val = websGetVar(wp, tmp, "");
            if (!strcmp(val, "Custom")) {
                snprintf(tmp, sizeof(tmp), "wan%d_bw_ul_custom", i);
                val = websGetVar(wp, tmp, "");
            }
            if (valid_range(wp, val, &bw_variables[_BW_UL]) == FALSE) {
                return FALSE;
            }
#else
            snprintf(tmp, sizeof(tmp), "wan%d_bw_type", i);
            val = websGetVar(wp, tmp, "");
            if (valid_choice(wp, val, &bw_variables[_BW_BW]) == FALSE) {
                return FALSE;
            }
#endif
            snprintf(tmp, sizeof(tmp), "wan%d_bw_resv", i);
            val = websGetVar(wp, tmp, "");
            if (valid_range(wp, val, &bw_variables[_BW_RESV]) == FALSE) {
                return FALSE;
            }

#ifndef EZP_PROD_BRAND_ZYUS
            if (!strcmp(val, "Custom")) {
                /* DL */
                snprintf(tmp, sizeof(tmp), "bw%d_bw_dl", i);
                val = websGetVar(wp, tmp, "");
                if (valid_range(wp, val, &bw_variables[_BW_DL]) == FALSE) {
                    return FALSE;
                }

                /* UL */
                snprintf(tmp, sizeof(tmp), "bw%d_bw_ul", i);
                val = websGetVar(wp, tmp, "");
                if (valid_range(wp, val, &bw_variables[_BW_UL]) == FALSE) {
                    return FALSE;
                }
            }
#endif
        } else if (!strcmp(val, "expert")) {
            /* DL */
            snprintf(tmp, sizeof(tmp), "bw%d_bw_expert_dl", i);
            val = websGetVar(wp, tmp, "");
            if (valid_range(wp, val, &bw_variables[_BW_DL]) == FALSE) {
                return FALSE;
            }

            /* UL */
            snprintf(tmp, sizeof(tmp), "bw%d_bw_expert_ul", i);
            val = websGetVar(wp, tmp, "");
            if (valid_range(wp, val, &bw_variables[_BW_UL]) == FALSE) {
                return FALSE;
            }
        } else {
            return FALSE;
        }
    }

    if (_valid_bw_sbm(wp, value, v) == FALSE) {
        return FALSE;
    }
    if (strcmp(ezp_prod_subsubcat, "L") != 0){ 
        if (_valid_bw_dbm(wp, value, v) == FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

#ifdef EZP_PROD_BRAND_PLANET
static int
_save_bw_group(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN];
    char *name, *rate, *ceil;
    int i, nrule, change = 0;

    snprintf(tmp, sizeof(tmp), "bw_group_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    if (nrule != atoi(nvram_safe_get("bw_group_rule_num"))) {
       /* In case the situation that the last rule is deleted but others are
         * kept the same and the check flow will treat it as 'nothing
         * changed'.
         */
        snprintf(tmp, sizeof(tmp), "%d", nrule);
        config_preaction(map, v, s, "NUM=0", "");
        nvram_set("bw_group_rule_num", tmp);
        change = 1;
    }
    for(i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "bw_group_rule%d_name", i);
        name = websGetVar(wp, tmp, "");
        snprintf(tmp, sizeof(tmp), "bw_group_rule%d_uprate", i);
        rate = websGetVar(wp, tmp, "");
        snprintf(tmp, sizeof(tmp), "bw_group_rule%d_upceil", i);
        ceil = websGetVar(wp, tmp, "");
        if (ezplib_check_attr_changed("bw_group_rule", i, "name", name) || 
            ezplib_check_attr_changed("bw_group_rule", i, "bw_uprate", rate) ||
            ezplib_check_attr_changed("bw_group_rule", i, "bw_upceil", ceil)) {
            if(!change) {
                config_preaction(map, v, s, "NUM=0", "");
                change = 1;
            }
            ezplib_replace_attr("bw_group_rule", i, "name", name);
           ezplib_replace_attr("bw_group_rule", i, "bw_uprate", rate);
            ezplib_replace_attr("bw_group_rule", i, "bw_upceil", ceil);
        }
    }
    return change;
}
#endif

static int
_save_bw_sbm(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char all_rule[LONG_BUF_LEN];
    char *enable, *name, *inipaddr_start, *inipaddr_end,
         *proto, *extport_start, *extport_end, *extif, *alloc, *ratio, *ul,
#ifdef EZP_PROD_BRAND_PLANET
         *dl, *excess, *excess_ratio, *excess_ul, *excess_dl, 
         *groupid, *dscpid, *remarkdscpid;
#else
         *dl, *excess, *excess_ratio, *excess_ul, *excess_dl;
#endif
    int i, max, nrule, start, end, num, len, change = 0;

    snprintf(tmp, sizeof(tmp), "bw_sbm_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    max = atoi(nvram_safe_get("bw_sbm_rule_max"));
    nrule = (nrule >= max) ? max : nrule;
    
    if (nrule != atoi(nvram_safe_get("bw_sbm_rule_num"))) {
        /* In case the situation that the last rule is deleted but others are
         * kept the same and the check flow will treat it as 'nothing
         * changed'.
         */
        snprintf(tmp, sizeof(tmp), "%d", nrule);
        config_preaction(map, v, s, "NUM=0", "");
        nvram_set("bw_sbm_rule_num", tmp);
        change = 1;
    }
    all_rule[0]='\0';
    for (i = 0; i < nrule; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        /* Name */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_name", i);
        name = websGetVar(wp, tmp, "");

        /* Internal IP Start */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_inipaddr_start", i);
        inipaddr_start = websGetVar(wp, tmp, "");

        /* Internal IP End */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_inipaddr_end", i);
        inipaddr_end = websGetVar(wp, tmp, "");

        start = ntohl((int)inet_addr(inipaddr_start));
        end = ntohl((int)inet_addr(inipaddr_end));
        num = end - start + 1;

        /* Proto */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_proto", i);
        proto = websGetVar(wp, tmp, "");

        /* External Port Start */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_extport_start", i);
        extport_start = websGetVar(wp, tmp, "");

        /* External Port End */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_extport_end", i);
        extport_end = websGetVar(wp, tmp, "");

        /* Direction */

        /* External IF */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_extif", i);
        extif = websGetVar(wp, tmp, "");

        /* Allcoation Way */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_bw_alloc", i);
        alloc = websGetVar(wp, tmp, "");

        /* BW Ratio */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_bw_ratio", i);
        ratio = websGetVar(wp, tmp, "");

        /* BW Upload */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_bw_upload", i);
        ul = websGetVar(wp, tmp, "");

        /* BW Download */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_bw_download", i);
        dl = websGetVar(wp, tmp, "");

        /* Schedule */

        /* Excess */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_excess", i);
        excess = websGetVar(wp, tmp, "");

        /* Excess Ratio */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_excess_ratio", i);
        excess_ratio = websGetVar(wp, tmp, "");

        /* Excess Upload */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_excess_upload", i);
        excess_ul = websGetVar(wp, tmp, "");

        /* Excess Download */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_excess_download", i);
        excess_dl = websGetVar(wp, tmp, "");

#ifdef EZP_PROD_BRAND_PLANET
        /* Bandwidth Group */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_groupid", i);
        groupid = websGetVar(wp, tmp, "");
        /* DSCP */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_dscpid", i);
        dscpid = websGetVar(wp, tmp, "");

        /* Remark DSCP */
        snprintf(tmp, sizeof(tmp), "bw_sbm_rule%d_remark_dscpid", i);
        remarkdscpid = websGetVar(wp, tmp, "");
#endif
        len =
            snprintf(tmp, TMP_LEN, 
#ifdef EZP_PROD_BRAND_PLANET
                     "%s^%s^%s^%s^%d^%s^%s^%s^%s^%s^%s^%s^%s^^%s^%s^%s^%s^%s^%s^%s",
#else
                     "%s^%s^%s^%s^%d^%s^%s^%s^%s^%s^%s^%s^%s^^%s^%s^%s^%s",
#endif
                     name, enable, inipaddr_start, inipaddr_end, num, proto, 
                     extport_start, extport_end, extif, alloc, ratio, ul, dl,
#ifdef EZP_PROD_BRAND_PLANET
                     excess, excess_ratio, excess_ul, excess_dl, dscpid, remarkdscpid, groupid);
#else
                     excess, excess_ratio, excess_ul, excess_dl);
#endif
        if (len >= TMP_LEN) {
            return 0;
        }

        if (ezplib_get_rule("bw_sbm_rule", i, buf, TMP_LEN) < 0) {
            /* i-th rule doesn't exist */
            config_preaction(map, v, s, "NUM=0", "");
            ezplib_append_rule("bw_sbm_rule", tmp);
            change = 1;
        } else {
            if (strcmp(buf, tmp)) {
                config_preaction(map, v, s, "NUM=0", "");
                ezplib_replace_rule("bw_sbm_rule", i, tmp);
                change = 1;
            }
        }
         if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", tmp);
         else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, tmp);
    }
    nvram_set("bw_sbm_rule", all_rule);
    
    return change;
}

static int
_save_bw_dbm(webs_t wp, char *value, struct variable *v, struct service *s,
        int64_t *map)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char all_rule[LONG_BUF_LEN];
    char *enable, *name, *inipaddr_start, *inipaddr_end;
    int i, max, nrule, start, end, num, len, change = 0;

    snprintf(tmp, sizeof(tmp), "bw_dbm_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    max = atoi(nvram_safe_get("bw_dbm_rule_max"));
    nrule = (nrule >= max) ? max : nrule;
    
    if (nrule != atoi(nvram_safe_get("bw_dbm_rule_num"))) {
        /* In case the situation that the last rule is deleted but others are
         * kept the same and the check flow will treat it as 'nothing
         * changed'.
         */
        snprintf(tmp, sizeof(tmp), "%d", nrule);
        config_preaction(map, v, s, "NUM=0", "");
        nvram_set("bw_dbm_rule_num", tmp);
        change = 1;
    }
    for (i = 0; i < nrule; i++) {
        /* Enable */
        snprintf(tmp, sizeof(tmp), "bw_dbm_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        /* Name */
        snprintf(tmp, sizeof(tmp), "bw_dbm_rule%d_name", i);
        name = websGetVar(wp, tmp, "");

        /* Internal IP Start */
        snprintf(tmp, sizeof(tmp), "bw_dbm_rule%d_inipaddr_start", i);
        inipaddr_start = websGetVar(wp, tmp, "");

        /* Internal IP End */
        snprintf(tmp, sizeof(tmp), "bw_dbm_rule%d_inipaddr_end", i);
        inipaddr_end = websGetVar(wp, tmp, "");

        start = ntohl((int)inet_addr(inipaddr_start));
        end = ntohl((int)inet_addr(inipaddr_end));
        num = end - start + 1;

        /* Schedule */

        len =
            snprintf(tmp, TMP_LEN, 
                     "%s^%s^%s^%s^%d^",
                     name, enable, inipaddr_start, inipaddr_end, num);

        if (len >= TMP_LEN) {
            return 0;
        }

        if (ezplib_get_rule("bw_dbm_rule", i, buf, TMP_LEN) < 0) {
            /* i-th rule doesn't exist */
            config_preaction(map, v, s, "NUM=0", "");
            ezplib_append_rule("bw_dbm_rule", tmp);
            change = 1;
        } else {
            if (strcmp(buf, tmp)) {
                config_preaction(map, v, s, "NUM=0", "");
                ezplib_replace_rule("bw_dbm_rule", i, tmp);
                change = 1;
            }
        }
        if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", tmp);
        else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, tmp);
    }
    nvram_set("bw_dbm_rule", all_rule);
    return change;
}
int
save_bw(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *bw, *type = "", *ul, *dl, *cur = nvram_safe_get("wan_num"),
#ifdef EZP_PROD_BRAND_PLANET
         *bw_enable, *link_type, *resv;
#else
         *bw_enable, *resv;
#endif
    int i, num, len, change = 0;
    int64_t map = 0;
    int link_percent, global_percent, user_percent;
    int ulmax, ulmin, dlmax, dlmin, bw_num, auto_dl, auto_ul,
        expert_dl, expert_ul, used_dl, used_ul;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    bw_enable = websGetVar(wp, "bw_enable", "");
    if (strcmp(bw_enable, nvram_safe_get("bw_enable"))) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set("bw_enable", bw_enable);
        change = 1;
    }
    
    if (*bw_enable == '1') {
        for (i = 0; i < num; i++) {
            /* BW */
            snprintf(tmp, sizeof(tmp), "wan%d_bw_mode", i);
            bw = websGetVar(wp, tmp, "");
            if (strcmp(bw, nvram_safe_get(tmp))) {
                snprintf(buf, TMP_LEN, "NUM=%d", i);
                config_preaction(&map, v, s, buf, "");
                nvram_set(tmp, bw);
                change = 1;
            }

            if (!strcmp(bw, "auto")) {
                /* Type */
#ifdef EZP_PROD_BRAND_ZYUS
                /* Download */
                snprintf(tmp, sizeof(tmp), "wan%d_bw_dl", i);
                dl = websGetVar(wp, tmp, "");
                printf("dl:%s\n", dl);
                if (!strcmp(dl, "Custom")) {
                    /* Download
                     * */
                    snprintf(tmp, sizeof(tmp), "wan%d_bw_dl_custom", i);
                    dl = websGetVar(wp, tmp, "");
                }
                /* Upload
                 * */
                snprintf(tmp, sizeof(tmp), "wan%d_bw_ul", i);
                ul = websGetVar(wp, tmp, "");
                printf("ul:%s\n", ul);
                if (!strcmp(ul, "Custom")) {
                    snprintf(tmp, sizeof(tmp), "wan%d_bw_ul_custom", i);
                    ul = websGetVar(wp, tmp, "");
                }

                snprintf(tmp, sizeof(tmp), "wan%d_bw_resv", i);
                resv = websGetVar(wp, tmp, "0");
#elif defined EZP_PROD_BRAND_PLANET
                 snprintf(tmp, sizeof(tmp), "wan%d_bw_type", i);
                 type = websGetVar(wp, tmp, "");
 
                 snprintf(tmp, sizeof(tmp), "wan%d_bw_resv", i);
                 resv = websGetVar(wp, tmp, "0");
 
                if (!strcmp(type, "Custom")) {
                    /* Download */
                    snprintf(tmp, sizeof(tmp), "wan%d_bw_dl", i);
                    dl = websGetVar(wp, tmp, "");
                    /* Upload */
                    snprintf(tmp, sizeof(tmp), "wan%d_bw_ul", i);
                    ul = websGetVar(wp, tmp, "");
                    link_type = "ADSL";
                } else if (!strcmp(type, "A1M64K")) {
                    dl = "1024";
                    ul = "64";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A1.5M640K")) {
                    dl = "1536";
                    ul = "640";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A2M256K")) {
                    dl = "2048";
                    ul = "256";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A2M512K")) {
                    dl = "2048";
                    ul = "512";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A3M640K")) {
                    dl = "3072";
                    ul = "640";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A4M1M")) {
                    dl = "4096";
                    ul = "1024";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A6M256K")) {
                    dl = "6144";
                    ul = "256";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A6M640K")) {
                    dl = "6144";
                    ul = "640";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A8M256K")) {
                    dl = "8192";
                    ul = "256";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A8M640K")) {
                    dl = "8192";
                    ul = "640";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A8M800K")) {
                    dl = "8192";
                    ul = "800";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A10M2M")) {
                    dl = "10240";
                    ul = "2048";
                    link_type = "ADSL";
                } else if (!strcmp(type, "A12M1M")) {
                    dl = "12288";
                    ul = "1024";
                    link_type = "ADSL";
                } else if (!strcmp(type, "C1M128K")) {
                    dl = "1024";
                    ul = "128";
                    link_type = "Cable";
                } else if (!strcmp(type, "C2M128K")) {
                    dl = "2048";
                    ul = "128";
                    link_type = "Cable";
                } else if (!strcmp(type, "C2M384K")) {
                    dl = "2048";
                    ul = "384";
                    link_type = "Cable";
                } else if (!strcmp(type, "C3M384K")) {
                    dl = "3072";
                    ul = "384";
                    link_type = "Cable";
                } else if (!strcmp(type, "C6M640K")) {
                    dl = "6144";
                    ul = "640";
                    link_type = "Cable";
                } else if (!strcmp(type, "C8M640K")) {
                    dl = "8192";
                    ul = "640";
                    link_type = "Cable";
                } else if (!strcmp(type, "C10M1M")) {
                    dl = "10240";
                    ul = "1024";
                    link_type = "Cable";
                } else if (!strcmp(type, "V2M2M")) {
                    dl = "2048";
                    ul = "2048";
                    link_type = "VDSL";
                } else if (!strcmp(type, "V4M4M")) {
                    dl = "4096";
                    ul = "4096";
                    link_type = "VDSL";
                } else if (!strcmp(type, "V10M10M")) {
                    dl = "10240";
                    ul = "10240";
                    link_type = "VDSL";
                } else if (!strcmp(type, "V25M25M")) {
                    dl = "25600";
                    ul = "25600";
                    link_type = "VDSL";
                } else if (!strcmp(type, "V50M50M")) {
                    dl = "51200";
                    ul = "51200";
                    link_type = "VDSL";
                } else if (!strcmp(type, "V100M100M")) {
                    dl = "102400";
                    ul = "102400";
                    link_type = "VDSL";
                } else {
                    dl = "2048";
                    ul = "256";
                    link_type = "ADSL";
                }
#else
                snprintf(tmp, sizeof(tmp), "wan%d_bw_resv", i);
                resv = websGetVar(wp, tmp, "");

                /* Download */
                snprintf(tmp, sizeof(tmp), "wan%d_bw_dl", i);
                dl = websGetVar(wp, tmp, "");
                /* Upload */
                snprintf(tmp, sizeof(tmp), "wan%d_bw_ul", i);
                ul = websGetVar(wp, tmp, "");
#endif

                snprintf(tmp, sizeof(tmp), "wan%d_bw_type", i);
                type = websGetVar(wp, tmp, "");

                used_ul = auto_ul = atoi(ul);
                used_dl = auto_dl = atoi(dl);

                link_percent = 100 - atoi(resv);

#ifdef EZP_PROD_BRAND_PLANET
                /* Cushion percentage */
                if (!strcmp(link_type, "ADSL")) {
                    global_percent = 90;
                    user_percent = 90;
                } else if (!strcmp(link_type, "Cable")) {
                    global_percent = 90;
                    user_percent = 90;
                } else if (!strcmp(link_type, "VDSL")) {
                    global_percent = 90;
                    user_percent = 90;
                } else {
                    global_percent = 90;
                   user_percent = 90;
                }
#else
                /* global percent */
                ezplib_get_attr_val("wan_bw_rule", i, "global_percent", tmp,
                        TMP_LEN, EZPLIB_USE_CLI);
                global_percent = atoi(tmp);

                /* user percent */
                ezplib_get_attr_val("wan_bw_rule", i, "user_percent", tmp,
                        TMP_LEN, EZPLIB_USE_CLI);
                user_percent = atoi(tmp);

                /* Other attributes */
                ezplib_get_attr_val("wan_bw_rule", i, "expert_ul", tmp,
                        TMP_LEN, EZPLIB_USE_CLI);
                expert_ul = atoi(tmp);
                ezplib_get_attr_val("wan_bw_rule", i, "expert_dl", tmp,
                        TMP_LEN, EZPLIB_USE_CLI);
                expert_dl = atoi(tmp);
#endif
                
            } else if (!strcmp(bw, "expert")) {
                /* DL */
                snprintf(tmp, sizeof(tmp), "wan%d_bw_expert_dl", i);
                used_dl = expert_dl = atoi(websGetVar(wp, tmp, ""));

                /* UL */
                snprintf(tmp, sizeof(tmp), "wan%d_bw_expert_ul", i);
                used_ul = expert_ul = atoi(websGetVar(wp, tmp, ""));

                link_percent = 95;
                global_percent = 95;
                user_percent = 95;

                /* Other attributes */
                ezplib_get_attr_val("wan_bw_rule", i, "ul", tmp,
                        TMP_LEN, EZPLIB_USE_CLI);
                auto_ul = atoi(tmp);
                ezplib_get_attr_val("wan_bw_rule", i, "dl", tmp,
                        TMP_LEN, EZPLIB_USE_CLI);
                auto_dl = atoi(tmp);
            } else {
                assert(0);
                return 0;
            }

            /* Percent */
            bw_num = atoi(nvram_safe_get("dbm_max"));
            ulmax = used_ul * link_percent / 100;
#ifdef EZP_PROD_BRAND_PLANET
            ulmax = ulmax * global_percent / 100;
            ulmax = ulmax * user_percent / 100;
#endif
            ulmin = ulmax / bw_num;

            dlmax = used_dl * link_percent / 100;
#ifdef EZP_PROD_BRAND_PLANET
            dlmax = dlmax * global_percent / 100;
            dlmax = dlmax * user_percent / 100;
#endif
            dlmin = dlmax / bw_num;

            len =
#ifdef EZP_PROD_BRAND_ZYUS
                snprintf(tmp, TMP_LEN, "^%d^%d^%d^%d^%d^%d^%d^%d^%d^%d^%d",
                        auto_dl, auto_ul, expert_dl,
                         expert_ul, link_percent, global_percent, user_percent,
                         ulmax, ulmin, dlmax, dlmin);
#else
                snprintf(tmp, TMP_LEN, "%s^%d^%d^%d^%d^%d^%d^%d^%d^%d^%d^%d",
                         type, auto_dl, auto_ul, expert_dl,
                         expert_ul, link_percent, global_percent, user_percent,
                         ulmax, ulmin, dlmax, dlmin);
#endif

            if (len >= TMP_LEN) {
                return 0;
            }

            ezplib_get_rule("wan_bw_rule", i, buf, TMP_LEN);
            if (strcmp(buf, tmp)) {
                snprintf(buf, TMP_LEN, "NUM=%d", i);
                config_preaction(&map, v, s, buf, "");
                ezplib_replace_rule("wan_bw_rule", i, tmp);
                change = 1;
            }
        }
#ifdef EZP_PROD_BRAND_PLANET
        if (_save_bw_group(wp, value, v, s, &map)) {
            change = 1;
        }
#endif

        if (_save_bw_sbm(wp, value, v, s, &map)) {
            change = 1;
        }

        if (strcmp(ezp_prod_subsubcat, "L") != 0){
            if (_save_bw_dbm(wp, value, v, s, &map)) {
                change = 1;
            }
        }
    }

    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }
    return change;
}

int
ej_bw_show_bw_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    int i, ret = 0;
    char *type;
    char tmp[TMP_LEN];

    if (ejArgs(argc, argv, "%d", &i) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(tmp, TMP_LEN, "wan%d_bw_mode", i);
    type = GOZILA_SAFE_GET(tmp);
    if (!*type) {
        /**
         * As the disable the WAN, the proto from gozila get will be empty.
         * Therefore, the proto should be retrieved from nvram again.
         */
        type = nvram_safe_get(tmp);
    }

    snprintf(tmp, TMP_LEN, "%d", i);
    if (!strcmp(type, "auto")) {
        do_ej("bw_auto.asp", wp, tmp, "");
    } else if (!strcmp(type, "expert")) {
        do_ej("bw_expert.asp", wp, tmp, "");
    } else {
        assert(0);
    }

    return ret;
}
int
ej_bw_show_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");  // Corresponding to wan_num
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }


#ifdef EZP_PROD_BRAND_PROX
    for (i = num-1; i >=0 ; i--) {
#else
    for (i = 0; i < num; i++) {
#endif
        char buf[SHORT_BUF_LEN];
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("bw_setting.asp", wp, buf, "");
    }

    return 0;
}

int
ej_bw_show_adv_bw_type(int eid, webs_t wp, int argc, char_t **argv)
{
    int rule_num;
    char buf[TMP_LEN];
    char *selected;
    char *bw_type[] = { BW_TYPE };
    int i;

    if (ejArgs(argc, argv, "%d", &rule_num) < 1) {
        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }

    for (i = 0; i < sizeof(bw_type)/sizeof(char *); i++) {
        char *type = bw_type[i];
#ifdef EZP_PROD_BRAND_PLANET
        char tmp_dl[TMP_LEN], tmp_ul[TMP_LEN], *tmp_type;
#else
        char tmp_dl[TMP_LEN], tmp_ul[TMP_LEN];
#endif
        char *ptr;

        snprintf(buf, TMP_LEN, "wan%d_bw_type", rule_num);
        selected = 
            nvg_attr_match(buf, "wan_bw_rule", rule_num, "type", type, 
                    "selected");
        if (!strcmp(type, "Custom")) {
            websWrite(wp, "<option value='%s' %s >%s</option>\n", 
                    type, (selected) ? selected : "", lang_translate("Custom"));
        } else {
#ifdef EZP_PROD_BRAND_PLANET
            ptr = strchr(&type[1], 'M');
#else
            ptr = strchr(type, 'M');
#endif
            if (!ptr) {
                strcpy(tmp_dl, "1M");
                strcpy(tmp_ul, "64K");
            } else {
#ifdef EZP_PROD_BRAND_PLANET
                strncpy(tmp_dl, &type[1], (int)(ptr - &type[1] + 1));
                tmp_dl[ptr - &type[1] + 1] = '\0';
#else
                strncpy(tmp_dl, type, (int)(ptr - type + 1));
                tmp_dl[ptr - type + 1] = '\0';
#endif
                strcpy(tmp_ul, ptr + 1);
            }

#ifdef EZP_PROD_BRAND_PLANET
            if (type[0] == 'A') {
                tmp_type = "ADSL";
            } else if (type[0] == 'C') {
                tmp_type = "Cable";
            } else if (type[0] == 'V') {
                tmp_type = "VDSL";
            } else {
                tmp_type = "ADSL";
            }
            
            websWrite(wp, "<option value='%s' %s >%s %s / %s bps</option>\n", 
                    type, (selected) ? selected : "", tmp_type, tmp_dl, tmp_ul);
#else
            websWrite(wp, "<option value='%s' %s > %s / %s bps</option>\n", 
                    type, (selected) ? selected : "", tmp_dl, tmp_ul);
#endif
        }
    }
        
    return 0;
}

int
ej_bw_show_resv(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth, resv;
    char buf[TMP_LEN];
    char *val;

    if (ejArgs(argc, argv, "%d", &nth) < 1) {
        websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }
    
    snprintf(buf, TMP_LEN, "wan%d_bw_resv", nth);
    val = GOZILA_GET(buf);
    if (!val) {
        ezplib_get_attr_val("wan_bw_rule", nth, "link_percent", buf,
                    sizeof(buf), EZPLIB_USE_CLI);
        val = buf;
    }

    resv = 100 - atoi(val);
    snprintf(buf, TMP_LEN, "%d", resv);
    return websWrite(wp, buf);
}
#define BW_TYPE_DL \
    "Custom", \
    "51200", "30720", "25600", "24576", "18432", "15360", \
    "12288", "10240", "7168", "6144", "4096", "3072", \
    "1536", "1024", "768"

#define BW_TYPE_UL \
    "Custom", \
    "25600", "20480", "5120", "3072", "2048", "1024", \
    "768", "512", "384"

int
ej_bw_show_rate_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char buf[TMP_LEN];
    int num;
    char *selected;
    char *type;
    int i;

    if (ejArgs(argc, argv, "%s %d", &type, &num) < 1) {
        websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }
    if (!strcmp(type, "ul")) {
        char *bw_type[] = { BW_TYPE_UL };
        for (i = 0; i < sizeof(bw_type)/sizeof(char *); i++) {
            char *bw = bw_type[i];
            snprintf(buf, TMP_LEN, "wan%d_bw_type", num);
            selected =
                nvg_attr_match(buf, "wan_bw_rule", num, type, bw,
                        "selected");
            websWrite(wp, "<option value='%s' %s >%s</option>\n",
                    bw, (selected) ? selected : "", bw);
        }
    } else if (!strcmp(type, "dl")) {
        char *bw_type[] = { BW_TYPE_DL };
        for (i = 0; i < sizeof(bw_type)/sizeof(char *); i++) {
            char *bw = bw_type[i];
            snprintf(buf, TMP_LEN, "wan%d_bw_type", num);
            selected =
                nvg_attr_match(buf, "wan_bw_rule", num, type, bw,
                        "selected");
            websWrite(wp, "<option value='%s' %s >%s</option>\n",
                    bw, (selected) ? selected : "", bw);
        }
    }
    return 0;
}
