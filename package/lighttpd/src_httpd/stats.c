#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <assert.h>
#include "fcgi_common.h"
#include "fcgi_stdio.h"
    
#include <cyutils.h>
    
#include "shared.h"
#include "lang.h"
#include "ezp.h"
#include "ezp-lib.h"
    
#define EZP_GX_MRTG_FILE_PATH       "/var/stats/stats-mrtg.js"
#define EZP_GX_SUM_FILE_PATH        "/var/stats/stats-hist.js"
    
#define UNIT_NHOUR  "60"
#define UNIT_NDAY   "600"
#define UNIT_NWEEK  "3600"
#define UNIT_NMONTH "21600"

#define SHORT_BUF_LEN   32

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
ej_st_mrtg_show(int eid, INPUT *input, int argc, char_t **argv)
{   
    char *name, *type, *ifname, *proto, *time;
    char tmp[SHORT_BUF_LEN];
    int sig;

    if (ejArgs(argc, argv, "%s, %s", &name, &type) < 2) {
        FCGI_LOG("Insufficient args");
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
        killall("stats", sig);
        if (wait_file_exists(name, 15, 0)) {
            do_file(name, input, NULL);
        }
        unlink(name);
    }

    return 0;
}

