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
    FW_ENABLE = 0,
    FW_SYN_ENABLE,
    FW_ICMP_ENABLE,
    FW_ICMP_REDIR_ENABLE,
    FW_RPFILTER_ENABLE,
    FW_STATE_ENABLE,
    FW_BC_STORM_ENABLE,
};

static struct variable fw_variables[] = {
    {longname: "FW Protection Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "FW SYN Protection Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "FW ICMP Proetection Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "FW ICMP Redir Protection Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "FW RP Filter Protection Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "FW SPI Protection Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "FW Broadcast Storming Enable", argv:ARGV("0", "1"), nullok: FALSE},
};

int
valid_fw(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "fw_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &fw_variables[FW_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* SYN Enable */
    snprintf(tmp, sizeof(tmp), "fw_syn_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &fw_variables[FW_SYN_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* ICMP Enable */
    snprintf(tmp, sizeof(tmp), "fw_icmp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &fw_variables[FW_ICMP_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* ICMP Redir Enable */
    snprintf(tmp, sizeof(tmp), "fw_icmp_redir_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &fw_variables[FW_ICMP_REDIR_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* RP FilterEnable */
    snprintf(tmp, sizeof(tmp), "fw_rpfilter_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &fw_variables[FW_RPFILTER_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* SPI Enable */
    snprintf(tmp, sizeof(tmp), "fw_state_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &fw_variables[FW_STATE_ENABLE]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_fw(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];    
    char *enable, *syn_enable, *icmp_enable, *icmp_redir_enable;
    char *rpfilter_enable, *state_enable, *bc_storm_enable;
    char *rule_set = "fw_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "fw_enable");
    enable = websGetVar(wp, tmp, "");
    /* IPSec */
    snprintf(tmp, sizeof(tmp), "fw_syn_enable");
    syn_enable = websGetVar(wp, tmp, "");
    /* PPTP */
    snprintf(tmp, sizeof(tmp), "fw_icmp_enable");
    icmp_enable = websGetVar(wp, tmp, "");
    /* L2TP */
    snprintf(tmp, sizeof(tmp), "fw_icmp_redir_enable");
    icmp_redir_enable = websGetVar(wp, tmp, "");
    /* H323 */
    snprintf(tmp, sizeof(tmp), "fw_rpfilter_enable");
    rpfilter_enable = websGetVar(wp, tmp, "");
    /* RTSP */
    snprintf(tmp, sizeof(tmp), "fw_state_enable");
    state_enable = websGetVar(wp, tmp, "");

    /* Broadcast Storming */
    snprintf(tmp, sizeof(tmp), "bc_storm_enable");
    bc_storm_enable = websGetVar(wp, tmp, "");

    /* Construct a rule tuple. */
    len = 
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s", enable, syn_enable, 
                icmp_enable, icmp_redir_enable, rpfilter_enable, state_enable, 
                bc_storm_enable);
                

    if (len > TMP_LEN - 1) {
        return 0;
    }

    if (strcmp(tmp, nvram_safe_get(rule_set))) {
#if defined (AR8316)
        if (*bc_storm_enable == '1') {
            /* If bc storm enabled, enable hw_nat. */
            ezplib_replace_attr("nat_rule", 0, "hw_enable","1");
        }
#else
        if (*bc_storm_enable == '1') {
            /* If bc storm enabled, enable hw_nat and reboot. */
            ezplib_replace_attr("nat_rule", 0, "hw_enable","0");
        } else {
            ezplib_replace_attr("nat_rule", 0, "hw_enable","1");
        }
#endif
        config_preaction(&map, v, services, "", ""); 
        nvram_set(rule_set, tmp);
        config_postaction(map, services, "", "");
        change = 1;
    }

    return change;
}
