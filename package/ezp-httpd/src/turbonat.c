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
#include "lang.h"
#include "ezp-lib.h"
#include "ezp.h"

enum {
    TURBONAT_ENABLE = 0,
    HWNAT_ENABLE,
    HWNAT_CHECK_BW,
};

static struct variable turbonat_variables[] = {
    {longname: "TURBONAT Enable", argv:ARGV("0", "1"), nullok: FALSE},
#ifdef EZP_PROD_BRAND_ZYUS
    {longname: "Hardware Accelerator Enable", argv:ARGV("0", "1"), nullok: FALSE},
#else
    {longname: "Hardware NAT Enable", argv:ARGV("0", "1"), nullok: FALSE},
#endif
};

int
valid_turbonat(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val,*val1,*val2,val3[TMP_LEN],val4[TMP_LEN];

    /* Enable */
    snprintf(tmp, sizeof(tmp), "turbonat_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &turbonat_variables[TURBONAT_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* If Web Filtering is enabled, we can not start TurboNAT. */
    val2 = nvram_safe_get("wf_enable");
    if (!strcmp(val, "1") && !strncmp(val2, "1", sizeof("0"))) {
        websDebugWrite(wp, "%s.<br> %s.<br>",
                lang_translate("Start TurboNAT failed"),
                lang_translate("Need to turn off Web Filter VPN/PPTP first")
                );
        return FALSE;
    }

    snprintf(tmp, sizeof(tmp), "hwnat_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &turbonat_variables[HWNAT_ENABLE]) == FALSE) {
        return FALSE;
    }
    /* If Bandwidth Management is enabled, we can not start hardware NAT. */
    val1 = nvram_safe_get("bw_enable");
    /* If Universal Repeater is enabled, we can not start hardware NAT. */
    ezplib_get_attr_val("wl0_apcli_rule", 0, "enable", val3,
                            sizeof(val3), EZPLIB_USE_CLI);
    /* If PPTPD is enabled, we can not start hardware NAT. */
    ezplib_get_attr_val("pptpd_rule", 0, "enable", val4,
                            sizeof(val4), EZPLIB_USE_CLI);
    
    if (!strcmp(val, "1") && ((strncmp(val1, "0", sizeof("0")) || strncmp(val2, "0", sizeof("0")) || !strcmp(val3, "1")) || !strcmp(val4, "1"))) {
        websDebugWrite(wp, "%s.<br> %s.<br>",
#ifdef EZP_PROD_BRAND_ZYUS
                lang_translate("Hardware Accelerator cannot be used if Web Filtering, Dynamic Bandwidth Management or VPN is enabled"),
                lang_translate("Need to turn off iDBM, Web Filter and VPN/PPTP first")
#elif defined EZP_PROD_BRAND_PROX
                lang_translate("Start Hardware NAT failed"),
                lang_translate("Need to turn off Bandwidth Management, Web Filter, Universal Repeater and VPN/PPTP first")
#else
                lang_translate("Start Hardware NAT failed"),
                lang_translate("Need to turn off iDBM, Web Filter, Universal Repeater and VPN/PPTP first")
#endif
                );
        return FALSE;
    }

    return TRUE;
}

int
save_turbonat(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];    
    char *turbo_enable, *hwnat_enable;
    char *rule_set = "turbonat_enable";
    int len, change = 0;
    int64_t map = 0;

    /* hwnat Enable */
    snprintf(tmp, sizeof(tmp), "hwnat_enable");
    hwnat_enable = websGetVar(wp, tmp, "");
    /* Enable */
    snprintf(tmp, sizeof(tmp), "turbonat_enable");
    turbo_enable = websGetVar(wp, tmp, "");

    /* Construct a rule tuple. */
    len = snprintf(tmp, TMP_LEN, "%s", turbo_enable);

    if (len > TMP_LEN - 1) {
        return 0;
    }
    if (ezplib_check_attr_changed("nat_rule", 0, "hw_enable", hwnat_enable) ||
        ezplib_check_attr_changed("nat_rule", 0, "bw_enable", turbo_enable) ||
        strcmp(tmp, nvram_safe_get(rule_set))) {
        change = 1;
    }
    if(change) {
        config_preaction(&map, v, services, "", ""); 
        ezplib_replace_attr("nat_rule", 0, "hw_enable", hwnat_enable);
        ezplib_replace_attr("nat_rule", 0, "bw_enable", turbo_enable);
        nvram_set(rule_set, tmp);
        config_postaction(map, services, "", "");
    }

    return change;
}
