#include <stdio.h>
#include <stdlib.h>
/*#include <curses.h>*/
#include <signal.h>
#include <string.h>
#include <bcmnvram.h>
#include <fcntl.h>
#include "ezp-lib.h"

#define BUFLEN  1024
#define TMPLEN  64

int
get_status_by_name(char *name) {
    FILE *stfd = fopen("/tmp/ipsec_status.tmp", "r");
    char buf[BUFLEN];
    char sa_established = 0, isakmp_established = 0;
    while (fgets(buf, sizeof(buf), stfd)) {
        /* We get a IPsec SA established line */
        if (strstr(buf, name) && strstr(buf, "IPsec SA established") && (strstr(buf, "EVENT_SA_REPLACE") || strstr(buf, "EVENT_SA_EXPIRE"))) {
            sa_established = 1;
        }
        if (strstr(buf, name) && strstr(buf, "ISAKMP SA established") && (strstr(buf, "EVENT_SA_REPLACE") || strstr(buf, "EVENT_SA_EXPIRE"))) {
            isakmp_established = 1;
        }
    }
    fclose(stfd);
    return ((sa_established << 1) | isakmp_established);
}

int
main(int argc, char **argv)
{
    char tmpline[TMPLEN], statur_rule[BUFLEN] = {0};
    char *tmpstr, *ruleptr = statur_rule;
    int ipsec_num = 0, ipsec_enable = 0, idx = 0, rule_status;

    tmpstr = nvram_safe_get("ipsec_enable");
    if (tmpstr && (tmpstr[0] == '1')) {
        ipsec_enable = 1;
    }
    tmpstr = nvram_safe_get("ipsec_rule_num");
    /* Not a null pointer or empty string */
    if (tmpstr && tmpstr[0]) {
        ipsec_num = atoi(tmpstr);
    }
    system("ipsec auto status > /tmp/ipsec_status.tmp");
    for (idx = 0; idx < ipsec_num; idx++) {
        if (!ipsec_enable) {
            strcpy(ruleptr, "None^None|");
            ruleptr += strlen("None^None|");
            continue;
        }
        ezplib_get_attr_val("ipsec_rule", idx, "enable", tmpline, sizeof(tmpline), EZPLIB_USE_CLI);
        if (tmpline[0] == '0') {
            strcpy(ruleptr, "None^None|");
            ruleptr += strlen("None^None|");
            continue;
        }
        ezplib_get_attr_val("ipsec_rule", idx, "name", tmpline, sizeof(tmpline), EZPLIB_USE_CLI);
        rule_status = get_status_by_name(tmpline);
        if (rule_status & 0x01) {
            strcpy(ruleptr, "Established^");
            ruleptr += strlen("Established^");
        }
        else {
            strcpy(ruleptr, "None^");
            ruleptr += strlen("None^");
        }
        if (rule_status & (0x01 << 1)) {
            strcpy(ruleptr, "Established|");
            ruleptr += strlen("Established|");
        }
        else {
            strcpy(ruleptr, "None|");
            ruleptr += strlen("None|");
        }
    }
    ruleptr --;
    *ruleptr = 0;
    nvram_fset("ipsec_status_rule", statur_rule);
    return 0;
}
