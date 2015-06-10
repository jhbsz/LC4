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
    STORAGE_CODEPAGE = 0,
    STORAGE_SWAPSIZE,
    STORAGE_EXT_ENABLE,
    STORAGE_TARGET_IP,
    STORAGE_TARGET_DIR,
    STORAGE_USER,
    STORAGE_PASSWD,
    STORAGE_GROUP
};

static struct variable storage_variables[] = {
    {longname: "Storgae Fat Codepage", argv:ARGV("")},
    {longname: "Storage Swap File Size", argv:ARGV("")},
    {longname: "Storage Ext Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Storage Targer IP", argv:ARGV(""), nullok: FALSE},
    {longname: "Storage Targer Directory", argv:ARGV("20"), nullok: FALSE},
    {longname: "Storage Targer User Name", argv:ARGV("20"), nullok: TRUE},
    {longname: "Storage Targer Password", argv:ARGV("20"), nullok: TRUE},
    {longname: "Storage Targer Group", argv:ARGV("20"), nullok: TRUE},
};

int
valid_storage(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Codepage */
    snprintf(tmp, sizeof(tmp), "storage_codepage");
    val = websGetVar(wp, tmp, "");
    if (valid_number(wp, val, &storage_variables[STORAGE_CODEPAGE]) == FALSE) {
        return FALSE;
    }

    /* Swap file size */
    snprintf(tmp, sizeof(tmp), "storage_swap_size");
    val = websGetVar(wp, tmp, "");
    if (valid_number(wp, val, &storage_variables[STORAGE_SWAPSIZE]) == FALSE) {
        return FALSE;
    }

    /* External storage enable */
    snprintf(tmp, sizeof(tmp), "storage_ext_enable");
    val = websGetVar(wp, tmp, "");

    if (valid_choice(wp, val, &storage_variables[STORAGE_EXT_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* Target IP */
    snprintf(tmp, sizeof(tmp), "storage_target_ip");
    val = websGetVar(wp, tmp, "");

    if (valid_ipaddr(wp, val, &storage_variables[STORAGE_TARGET_IP]) == FALSE) {
        return FALSE;
    }

    /* Target Diretcory */
    snprintf(tmp, sizeof(tmp), "storage_target_dir");
    val = websGetVar(wp, tmp, "");

    if (valid_word(wp, val, &storage_variables[STORAGE_TARGET_DIR]) == FALSE) {
        return FALSE;
    }
    /* Target User */
    snprintf(tmp, sizeof(tmp), "storage_user");
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &storage_variables[STORAGE_USER]) == FALSE) {
        return FALSE;
    }
    /* Target Password */
    snprintf(tmp, sizeof(tmp), "storage_passwd");
    val = websGetVar(wp, tmp, "");

    if (valid_passwd(wp, val, &storage_variables[STORAGE_PASSWD]) == FALSE) {
        return FALSE;
    }

    /* Target Group */
    snprintf(tmp, sizeof(tmp), "storage_group");
    val = websGetVar(wp, tmp, "");

    if (valid_word(wp, val, &storage_variables[STORAGE_GROUP]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

int
save_storage(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[LONG_BUF_LEN], bs[TMP_LEN];
    char *codepage, *swap_size, *ext_enable, *ip, *dir, *user, *pw, *gp;
    char *rule_set = "storage_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Codepage */ 
    snprintf(tmp, sizeof(tmp), "storage_codepage");
    codepage = websGetVar(wp, tmp, "");

    /* Swap size */
    snprintf(tmp, sizeof(tmp), "storage_swap_size");
    swap_size = websGetVar(wp, tmp, "");

    /* Ext enable */
    snprintf(tmp, sizeof(tmp), "storage_ext_enable");
    ext_enable = websGetVar(wp, tmp, "");
    
    /* Target IP */
    snprintf(tmp, sizeof(tmp), "storage_target_ip");
    ip = websGetVar(wp, tmp, "");

    /* Target Dir */
    snprintf(tmp, sizeof(tmp), "storage_target_dir");
    dir = websGetVar(wp, tmp, "");

    /* User */
    snprintf(tmp, sizeof(tmp), "storage_user");
    user = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "storage_passwd");
    pw = websGetVar(wp, tmp, "");

    /* Group */
    snprintf(tmp, sizeof(tmp), "storage_group");
    gp = websGetVar(wp, tmp, "");

    ezplib_get_attr_val("storage_rule", 0, "block_size", bs, sizeof(bs),
            EZPLIB_USE_CLI);

    /* Construct a rule tuple. */
    /* codepage^swap_size^block_size^ext_enable^target_ip^taget_dir^user^passwd^group */
    len = 
        snprintf(tmp, LONG_BUF_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s",
                 codepage, swap_size, bs, ext_enable, ip, dir, user, pw, gp);

    if (len > LONG_BUF_LEN - 1) {
        return 0;
    }

    if (strcmp(tmp, nvram_safe_get(rule_set))) {
        config_preaction(&map, v, services, "", ""); 
        ezplib_replace_rule(rule_set, 0, tmp);
        config_postaction(map, services, "", "");
        change = 1;
    }

    return change;
}
