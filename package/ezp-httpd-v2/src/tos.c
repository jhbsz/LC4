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
    TOS_ENABLE = 0,
    TOS_ICMP_ENABLE,
    TOS_DNS_ENABLE,
    TOS_SSH_ENABLE,
    TOS_TELNET_ENABLE,
    TOS_CHECK_ENABLE,
    TOS_ACK_ENABLE
};

static struct variable tos_variables[] = {
    {longname: "TOS Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "TOS ICMP Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "TOS DNS Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "TOS SSH Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "TOS TELNET Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "TOS CHECK Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "TOS ACK Enable", argv:ARGV("0", "1"), nullok: FALSE},
};

int
valid_tos(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "tos_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &tos_variables[TOS_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* ICMP Enable */
    snprintf(tmp, sizeof(tmp), "tos_icmp_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &tos_variables[TOS_ICMP_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* DNS Enable */
    snprintf(tmp, sizeof(tmp), "tos_dns_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &tos_variables[TOS_DNS_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* SSH Enable */
    snprintf(tmp, sizeof(tmp), "tos_ssh_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &tos_variables[TOS_SSH_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* TELNET FilterEnable */
    snprintf(tmp, sizeof(tmp), "tos_telnet_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &tos_variables[TOS_TELNET_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* CHECK Enable */
    snprintf(tmp, sizeof(tmp), "tos_check_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &tos_variables[TOS_CHECK_ENABLE]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_tos(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];    
    char *enable, *dns_enable, *icmp_enable, *ssh_enable;
    char *telnet_enable, *check_enable, *ack_enable;
    char *rule_set = "tos_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "tos_enable");
    enable = websGetVar(wp, tmp, "");
    /* ICMP */
    snprintf(tmp, sizeof(tmp), "tos_icmp_enable");
    icmp_enable = websGetVar(wp, tmp, "");
    /* DNS */
    snprintf(tmp, sizeof(tmp), "tos_dns_enable");
    dns_enable = websGetVar(wp, tmp, "");
    /* SSH */
    snprintf(tmp, sizeof(tmp), "tos_ssh_enable");
    ssh_enable = websGetVar(wp, tmp, "");
    /* TELNET */
    snprintf(tmp, sizeof(tmp), "tos_telnet_enable");
    telnet_enable = websGetVar(wp, tmp, "");
    /* CHECK */
    snprintf(tmp, sizeof(tmp), "tos_check_enable");
    check_enable = websGetVar(wp, tmp, "");
    /* ACK */
    snprintf(tmp, sizeof(tmp), "tos_ack_enable");
    ack_enable = websGetVar(wp, tmp, "");

    /* Construct a rule tuple. */
    len = 
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s", enable, icmp_enable,
                dns_enable, ssh_enable, telnet_enable, check_enable,
                ack_enable);

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
