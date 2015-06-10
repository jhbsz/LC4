#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <assert.h>

#include <cyutils.h>

#include "shared.h"
#include "common.h"
#include "lang.h"
#include "ezp.h"
#include "ezp-lib.h"

#define EZP_GX_MRTG_FILE_PATH       "/var/stats/stats-mrtg.js"
#define EZP_GX_SUM_FILE_PATH        "/var/stats/stats-hist.js"

#define UNIT_NHOUR  "60"
#define UNIT_NDAY   "600"
#define UNIT_NWEEK  "3600"
#define UNIT_NMONTH "21600"

int wait_file_exists(const char *name, int max, int invert)
{
	while (max-- > 0) {
		if (f_exists(name) ^ invert) {
            return 1;
        }
		sleep(1);
	}
	return 0;
}

int
ej_st_mrtg_show(int eid, webs_t wp, int argc, char_t **argv)
{
    char *name, *type, *ifname, *proto, *time;
    char tmp[SHORT_BUF_LEN];
    int sig;

    if (ejArgs(argc, argv, "%s, %s", &name, &type) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return FALSE;
    }
    ezplib_get_attr_val("stats_rule", 0, "enable", tmp, 
            SHORT_BUF_LEN, EZPLIB_USE_CLI);
    if (!strcmp(tmp, "1")) {
        nvram_set("stats_type", type);

        if (!strcmp(type, "ct")) {
            /* proto name and time */
            /* TODO: reserved it for the enhanced feature: choose the protocol
             * the users want.
             */
            proto = GOZILA_SAFE_GET("stats_proto");
            if (!*proto) {
                proto = "all";
            }
            nvram_set("stats_proto", proto);

            time = GOZILA_SAFE_GET("stats_interval");
            if (!*time) {
                time = "60";
            }
            nvram_set("stats_interval", time);
        } else if (!strcmp(type, "thp")) {
            /* ifname and time */
            ifname = GOZILA_SAFE_GET("stats_ifname");
            if (!*ifname) {
                ifname = "br0";
            }
            /* Find out the real physical interface. */
            nvram_set("stats_ifname", ifname);

            time = GOZILA_SAFE_GET("stats_interval");
            if (!*time) {
                time = "60";
            }
            nvram_set("stats_interval", time);
        } else {
            assert(0);
            return 1;
        }

        if (!strcmp(name, "graph")) { /* graph */
            sig = SIGUSR1;
            name = EZP_GX_MRTG_FILE_PATH;
        }
        else { /* sum */
            sig = SIGUSR2;
            name = EZP_GX_SUM_FILE_PATH;
        }
        unlink(name);
        /* Kick stats to generate the file. */
        // snprintf(tmp, sizeof(tmp), "killall -%d stats", sig);
        // system(tmp);
        killall("stats", sig);
        if (wait_file_exists(name, 15, 0)) {
            do_file(name, wp, NULL);
        }
        unlink(name);
    }

    return 0;
}

int
ej_st_show_if(int eid, webs_t wp, int argc, char_t **argv)
{
    int i;
    int nwan = atoi(nvram_safe_get("wan_num")), 
        nlan = atoi(nvram_safe_get("lan_num"));
    char *ifname = GOZILA_SAFE_GET("stats_ifname");
    char *val;
    char buf[SHORT_BUF_LEN];

#ifndef EZP_PROD_BRAND_PROX
    for (i=nwan-1; i >= 0; i--) {
#else
    for (i = 0; i < nwan; i++) {
#endif
        snprintf(buf, sizeof(buf), "wan%d_proto", i);
        val = nvram_safe_get(buf);
        if (!strcmp(val, "wwan") || !strcmp(val, "directip") ||
                !strcmp(val, "htc")) {
            snprintf(buf, SHORT_BUF_LEN, "wan%d_ifname", i);
        } else {
            snprintf(buf, SHORT_BUF_LEN, "wan%d_device", i);
        }
        val = nvram_safe_get(buf);
        if (nwan > 1) {
#if defined EZP_PROD_BRAND_ZYUS
            websWrite(wp, "<option value='%s' %s>%s %s</option>",
                    val, (!strcmp(val, ifname)) ? "selected" : "",
                    i?"Mobile":"Ethernet", lang_translate("WAN")) ;
#elif defined EZP_PROD_BRAND_PROX
            websWrite(wp, "<option value='%s' %s>%s%s</option>",
                          val, (!strcmp(val, ifname)) ? "selected" : "", 
                          lang_translate("WAN"),i?"(USB Modem)":"(Ethernet)");
#else
            websWrite(wp, "<option value='%s' %s>%s %d</option>",
                          val, (!strcmp(val, ifname)) ? "selected" : "", 
                          lang_translate("WAN"), i+1);
#endif
        } else {
            websWrite(wp, "<option value='%s' %s>%s</option>",
                          val, (!strcmp(val, ifname)) ? "selected" : "", 
                          lang_translate("WAN"));
        }
    }

#ifdef CONFIG_EZP_ARCH_RTL865X
    val = nvram_safe_get("lan0_ifname");
    websWrite(wp, "<option value='%s' %s>%s</option>\n",
                  val, (!strcmp(val, ifname)) ? "selected" : "", 
                  lang_translate("LAN"));

#else
    /* XXX: lan0_ifname presents the combination of wired and wireless. */
    val = nvram_safe_get("lan0_ifname");
    if (appmgr_check("wlan")) {
        websWrite(wp, "<option value='%s' %s>%s (%s)</option>\n",
                val, (!strcmp(val, ifname)) ? "selected" : "", 
                lang_translate("LAN"), lang_translate("Wireless and Wired"));
    }
    for (i = 0; i < nlan; i++) {
        snprintf(buf, SHORT_BUF_LEN, "lan%d_device", i);
        val = nvram_safe_get(buf);
        if (nlan > 1) {
            websWrite(wp, "<option value='%s' %s>%s (%s) %d</option>\n",
                          val, (!strcmp(val, ifname)) ? "selected" : "", 
                          lang_translate("LAN"), lang_translate("Wired"), i+1);
        } else {
            websWrite(wp, "<option value='%s' %s>%s (%s)</option>\n",
                          val, (!strcmp(val, ifname)) ? "selected" : "", 
                          lang_translate("LAN"), lang_translate("Wired"));
        }
    }
    if (appmgr_check("wlan")) {
        ezplib_get_attr_val("wl0_ifname_rule", 0, "ifname", buf, sizeof(buf),
                EZPLIB_USE_CLI);
        websWrite(wp, "<option value='%s' %s>%s (%s)</option>\n",
                buf, (!strcmp(buf, ifname)) ? "selected" : "", 
                lang_translate("LAN"), lang_translate("Wireless"));
    }
    /* End wireless product model config */
#endif
    return 0;
}

int
ej_st_show_period(int eid, webs_t wp, int argc, char_t **argv)
{
    char *interval;
    interval = nvram_safe_get("stats_interval");
    if (!strcmp(interval, UNIT_NHOUR)) {
        websWrite(wp, lang_translate("Two Hours"));
    } else if (!strcmp(interval, UNIT_NDAY)) {
        websWrite(wp, lang_translate("One Day"));
    } else if (!strcmp(interval, UNIT_NWEEK)) {
        websWrite(wp, lang_translate("One Week"));
    } else if (!strcmp(interval, UNIT_NMONTH)) {
        websWrite(wp, lang_translate("One Month"));
    } else {
        return 1;
    }
    return 0;
}
