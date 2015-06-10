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

/*enable^isns_serv^isid^alias^max_conn^max_tx^max_rx*/
enum {
    ISCSI_ENABLE = 0,
    ISCSI_ISNS,
    ISCSI_ID,
    ISCSI_ALIAS,
    ISCSI_MAX_CONN,
    ISCSI_MAX_TX,
    ISCSI_MAX_RX,
};

static struct variable iscsi_variables[] = {
    {longname: "iSCSI Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "iSCSI iSNS Server", argv:ARGV("1", "80"), nullok: TRUE},
    {longname: "iSCSI Target ID", argv:ARGV("1", "80"), nullok: FALSE},
    {longname: "iSCSI Alias Name", argv:ARGV("20"), nullok: TRUE},
    {longname: "iSCSI Max Connections", argv:ARGV("1","10"), nullok: TRUE},
    {longname: "iSCSI Max Xmit Data Length", argv:ARGV("512","40960"), nullok: TRUE},
    {longname: "iSCSI Max Recv Data Length", argv:ARGV("512","40960"), nullok: TRUE}
};

enum {
    IS_ENABLE = 0,
    IS_USERNAME,
    IS_PASSWD,
};

static struct variable isuser_variables[] = {
    {longname: "iSCSI User Enable", argv:ARGV("0","1"), nullok: FALSE},
    {longname: "iSCSI User Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "iSCSI Password", argv:ARGV("20"), nullok: FALSE},
};

#define CHECK_DOMAIN_NAME_WORD(check_char) (((check_char >= '0')&&(check_char <= '9')) ||\
                                            ((check_char >= 'A')&&(check_char <= 'z')) ||\
                                            (check_char == '.') || (check_char == '_') ||\
                                            (check_char == '-'))

static int
valid_domain_name(webs_t wp, char *value, struct variable *v)
{
    int len;
    char *check_ch;
    
    if (valid_length_range(wp, value, v) == FALSE)
    {
        return FALSE;
    }
    len = strlen(value);
    check_ch = value;
    for (; len > 0; len--, check_ch++)
    {
        if (!CHECK_DOMAIN_NAME_WORD(*check_ch))
        {
            websDebugWrite(wp, "Invalid <b>%s</b>: '%c' is not a valid character<br>",
                v->longname, *check_ch);
            return FALSE;
        }
    }
    return TRUE;
}

static int
valid_sns(webs_t wp, char *value, struct variable *v)
{
    if (valid_ipaddr(wp, value, v))
        return TRUE;
    if (valid_domain_name(wp, value, v))
        return TRUE;
    return FALSE;
}

static int
valid_id(webs_t wp, char *value, struct variable *v)
{
    if (valid_length_range(wp, value, v) == FALSE)
        return FALSE;
    return TRUE;
}

int
valid_iscsi(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int i, nrule;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "iscsi_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &iscsi_variables[ISCSI_ENABLE]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "iscsi_isns");
    val = websGetVar(wp, tmp, "");
    if (valid_sns(wp, val, &iscsi_variables[ISCSI_ISNS]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "iscsi_id");
    val = websGetVar(wp, tmp, "");
    if (valid_id(wp, val, &iscsi_variables[ISCSI_ID]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "iscsi_alias");
    val = websGetVar(wp, tmp, "");
    if (valid_name(wp, val, &iscsi_variables[ISCSI_ALIAS]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "iscsi_max_conn");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &iscsi_variables[ISCSI_MAX_CONN]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "iscsi_max_tx");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &iscsi_variables[ISCSI_MAX_TX]) == FALSE) {
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "iscsi_max_rx");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &iscsi_variables[ISCSI_MAX_RX]) == FALSE) {
        return FALSE;
    }
    
    snprintf(tmp, sizeof(tmp), "isuser_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "isuser_rule%d_enable", i);
        val = websGetVar(wp, tmp, "");
        if (valid_choice(wp, val, &isuser_variables[IS_ENABLE]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "isuser_rule%d_username", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &isuser_variables[IS_USERNAME]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "isuser_rule%d_password", i);
        val = websGetVar(wp, tmp, "");
        /* The password is shown as a public key for the administrator. */
        if (valid_name(wp, val, &isuser_variables[IS_PASSWD]) == FALSE) {
            return FALSE;
        }
    }
    
    return TRUE;
}

int
save_iscsi(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[LONG_BUF_LEN];    
    char all_rule[LONG_BUF_LEN];
    char *enable, *isns, *id, *alias, *max_conn,
         *max_tx, *max_rx;
    char *user_enable, *username, *password;
    int change =0, nrule, max, i, len;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "iscsi_enable");
    enable = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "iscsi_isns");
    isns = websGetVar(wp, tmp, "");
    
    snprintf(tmp, sizeof(tmp), "iscsi_id");
    id = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "iscsi_alias");
    alias = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "iscsi_max_conn");
    max_conn = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "iscsi_max_tx");
    max_tx = websGetVar(wp, tmp, "");

    snprintf(tmp, sizeof(tmp), "iscsi_max_rx");
    max_rx = websGetVar(wp, tmp, "");

    snprintf(buf, LONG_BUF_LEN, "%s^%s^%s^%s^%s^%s^%s",
            enable, isns, id, alias, max_conn, max_tx, max_rx);

    if (strcmp(buf, nvram_safe_get("is_rule"))) {
        config_preaction(&map, v, s, "NUM=0", "");
        nvram_set("is_rule", buf);
        change = 1;
    }

    nrule = atoi(websGetVar(wp, "isuser_rule_num", "0"));
    max = atoi(nvram_safe_get("isuser_rule_max"));
    nrule = (nrule >= max) ? max : nrule;
    if (nrule != atoi(nvram_safe_get("isuser_rule_num"))) {
        config_preaction(&map, v, s, "NUM=0", "");
        snprintf(tmp, sizeof(tmp), "%d", nrule);
        nvram_set("isuser_rule_num", tmp);
        change = 1;
    }
    
    all_rule[0]='\0';
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "isuser_rule%d_enable", i);
        user_enable = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "isuser_rule%d_username", i);
        username = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "isuser_rule%d_password", i);
        password= websGetVar(wp, tmp, "");

        len = snprintf(buf, LONG_BUF_LEN, "%s^%s^%s", 
                       user_enable, username, password);
        if (len >= LONG_BUF_LEN) {
            return 0;
        }

        if (ezplib_get_rule("isuser_rule", i, tmp, LONG_BUF_LEN) < 0) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_append_rule("isuser_rule", buf);
            change = 1;
        } else if (strcmp(tmp, buf)) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_rule("isuser_rule", i, buf);
            change = 1;
        }
        if (i == 0) snprintf(all_rule, sizeof(all_rule), "%s", buf);
        else snprintf(all_rule, sizeof(all_rule), "%s|%s", all_rule, buf);
    }
    nvram_set("isuser_rule", all_rule);
    if (change) {
        snprintf(tmp, LONG_BUF_LEN, "NUM=%d", i);
        /* Not defined the postaction yet. */
        config_postaction(map, s, tmp, "");
    }

    return change;
}
