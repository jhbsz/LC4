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
    WPS_ENABLE = 0,
    WPS_PIN
};

static struct variable wps_variables[] = {
    {longname: "WPS Enable", argv:ARGV("0", "7"), nullok: FALSE},
    {longname: "WPS PIN", argv:ARGV("0", "99999999"), nullok: FALSE},
};

void
do_wps_cgi(char *url, webs_t stream, ...)
{
    FILE *fp=NULL;
    char buf[TMP_LEN];
    char *act = NULL, *pin = NULL;
    
    /* Get wps action */
    act = websGetVar(wp, "action", "");
    

    /* do action */
    if (!strcmp(act, "WPS_Gen")) {
        /* generate router pin code */
        fp = popen("/sbin/wps-ctrl gen_pin", "r");
        if (fp) {
            fgets(buf, sizeof(buf),fp);
            websWrite(stream, "message=Router Pin Code is %s", buf);
            pclose(fp);
        } else {
            websWrite(stream, "message=Generate Pin error\n", buf);
        }
    } else if (!strcmp(act, "WPS_Btn")) {
        /* wps push button */
        fp = popen("/sbin/wps-ctrl pbc", "r");
        if (fp) {
            fgets(buf, sizeof(buf),fp);
            websWrite(stream, "message=%s\n", buf);
            pclose(fp);
        } else {
            websWrite(stream, "message=WPS PBC error\n", buf);
        }
    } else if (!strcmp(act, "WPS_Pin")) {
        /* wps pin connect */
        char cmd[TMP_LEN];
        pin = websGetVar(wp, "wps_pin", "");
        if (valid_range(stream, pin, &wps_variables[WPS_PIN]) == FALSE) {
            websWrite(stream, "return_value=-2\n");
            websDone(stream, 200);
            return;
        }
        snprintf(cmd, sizeof(cmd), "/sbin/wps-ctrl pin %s", pin);
        fp = popen(cmd, "r");
        if (fp) {
            fgets(buf, sizeof(buf),fp);
            websWrite(stream, "message=%s\n", buf);
            pclose(fp);
        } else {
            websWrite(stream, "message=WPS PIN error\n", buf);
        }
    }
    websWrite(stream, "return_value=-3\n");
    websDone(stream, 200);

    /* Flush all key-value pairs in tha hash table. */
    init_cgi(NULL);
    return;
}

int
valid_wl_wps(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "wl_wps_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &wps_variables[WPS_ENABLE]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

int
save_wl_wps(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN];
    char *enable;
    char *rule_set = "wl_wps_rule";
    int change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "wl_wps_enable");
    enable = websGetVar(wp, tmp, "");

    ezplib_get_attr_val(rule_set, 0, "mode", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if (strcmp(tmp, enable)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_attr(rule_set, 0, "mode", enable);
        change = 1;
        config_postaction(map, s, "", "");
    }

    return change;
}
