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
    NAT_PASS_ENABLE = 0,
    NAT_PASS_IPSEC_ENABLE,
    NAT_PASS_PPTP_ENABLE,
    NAT_PASS_L2TP_ENABLE,
    NAT_PASS_H323_ENABLE,
    NAT_PASS_RTSP_ENABLE,
    NAT_PASS_MMS_ENABLE,
};

static struct variable nat_pass_variables[] = {
    {longname: "NAT Pass Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "NAT IPSec Traversal Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "NAT PPTP Traversal Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "NAT L2TP Traversal Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "NAT H323 Traversal Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "NAT RTSP Traversal Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "NAT MMS Traversal Enable", argv:ARGV("0", "1"), nullok: FALSE},
};

int
valid_nat_pass(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "nat_pass_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &nat_pass_variables[NAT_PASS_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* IPSec Enable */
    snprintf(tmp, sizeof(tmp), "nat_pass_ipsec_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &nat_pass_variables[NAT_PASS_IPSEC_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* PPTP Enable */
    snprintf(tmp, sizeof(tmp), "nat_pass_pptp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &nat_pass_variables[NAT_PASS_PPTP_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* L2TP Enable */
    snprintf(tmp, sizeof(tmp), "nat_pass_l2tp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &nat_pass_variables[NAT_PASS_L2TP_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* H323 Enable */
    snprintf(tmp, sizeof(tmp), "nat_pass_h323_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &nat_pass_variables[NAT_PASS_H323_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* RTSP Enable */
    snprintf(tmp, sizeof(tmp), "nat_pass_rtsp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &nat_pass_variables[NAT_PASS_RTSP_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* MMS Enable */
    snprintf(tmp, sizeof(tmp), "nat_pass_mms_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &nat_pass_variables[NAT_PASS_MMS_ENABLE]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_nat_pass(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];    
    char *enable, *ipsec_enable, *pptp_enable, *l2tp_enable;
    char *h323_enable, *rtsp_enable, *mms_enable;
    char *rule_set = "nat_pass_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "nat_pass_enable");
    enable = websGetVar(wp, tmp, "");
    /* IPSec */
    snprintf(tmp, sizeof(tmp), "nat_pass_ipsec_enable");
    ipsec_enable = websGetVar(wp, tmp, "");
    /* PPTP */
    snprintf(tmp, sizeof(tmp), "nat_pass_pptp_enable");
    pptp_enable = websGetVar(wp, tmp, "");
    /* L2TP */
    snprintf(tmp, sizeof(tmp), "nat_pass_l2tp_enable");
    l2tp_enable = websGetVar(wp, tmp, "");
    /* H323 */
    snprintf(tmp, sizeof(tmp), "nat_pass_h323_enable");
    h323_enable = websGetVar(wp, tmp, "");
    /* RTSP */
    snprintf(tmp, sizeof(tmp), "nat_pass_rtsp_enable");
    rtsp_enable = websGetVar(wp, tmp, "");
    /* MMS */
    snprintf(tmp, sizeof(tmp), "nat_pass_mms_enable");
    mms_enable = websGetVar(wp, tmp, "");


    /* Construct a rule tuple. */
    len = 
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s", enable, ipsec_enable, 
                pptp_enable, l2tp_enable, h323_enable, rtsp_enable, mms_enable);

    if (len > TMP_LEN - 1) {
        return 0;
    }

    if (strcmp(tmp, nvram_safe_get(rule_set))) {
        config_preaction(&map, v, services, "", ""); 
        nvram_set(rule_set, tmp);
        config_postaction(map, services, "", "");
        change = 1;
    }

    return change;
}
