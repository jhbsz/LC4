/*
 * Broadcom Home Gateway Reference Design
 * Web Page Configuration Support Routines
 *
 * Copyright 2001-2003, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: wireless.c,v 1.44 2005/05/25 12:31:59 honor Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <error.h>
#include <signal.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <wlioctl.h>
#include <wlutils.h>

#include <md5.h>

#include "ezp.h"
#include "lang.h"
#include "ezp-lib.h"
#include "nvram_ezpacket.h"
#include "fcgi_common.h"
#include "fcgi_stdio.h"
#include "http_common.h"

#define WL_SSID_MAX_LENGTH "32"

int
ej_wlv_sec_show_setting(int eid, INPUT *input, int argc, char_t **argv)
{
    char buf_sec[TMP_LEN], buf_if[TMP_LEN], buf_vif[TMP_LEN];
    int if_idx, vif_idx, num;
    char *secmode = NULL;
    INPUT *tmp;


    /* Get physical interface index */
    if (ejArgs(argc, argv, "%d %d", &if_idx, &vif_idx) < 2) {
        FCGI_LOG("Insufficient args");
        return FALSE;
    }
    num = atoi(nvram_safe_get("wlv_rule_num"));

    /* Get security mode rule */
    snprintf(buf_if, TMP_LEN, "wl%d_ssid%d_secmode" , if_idx, vif_idx);

    if ((tmp = CGI_Find_Parameter(input, buf_if))) {
       secmode = tmp->val;
    }

    if (!secmode || !*secmode) {
        snprintf(buf_if, TMP_LEN, "wl%d_sec_rule", if_idx);
        ezplib_get_attr_val(buf_if, vif_idx, "secmode", buf_sec,
                    TMP_LEN, EZPLIB_USE_CLI);
    } else{
        sprintf(buf_sec, "%s", secmode);
    }
    /*----------------
    buf_sec : security mode
    buf_if : interface index
    buf_vif : ssid index
    -----------------*/
    snprintf(buf_if, TMP_LEN, "%d", if_idx);
    snprintf(buf_vif, TMP_LEN, "%d", vif_idx);
    if (!strcmp(buf_sec, "wep")) {
        do_ej("wlv_sec_wep.asp", input, buf_if, buf_vif, "");
    } else if (!strcmp(buf_sec, "psk")) {
        do_ej("wlv_sec_psk.asp", input, buf_if, buf_vif, "");
    } else if (!strcmp(buf_sec, "wpa")) {
        do_ej("wlv_sec_wpa.asp", input, buf_if, buf_vif, "");
    } else if (!strcmp(buf_sec, "psk2")) {
        do_ej("wlv_sec_psk2.asp", input, buf_if, buf_vif, "");
    } else if (!strcmp(buf_sec, "wpa2")) {
        do_ej("wlv_sec_wpa2.asp", input, buf_if, buf_vif, "");
    }

    if (appmgr_check("guest_lan") && ( vif_idx == num - 1 )) {
            do_ej("guest_lan_setting.asp", input, buf_if, buf_vif, "");
    }
    return 0;
}

int
valid_base64(INPUT *input, char *value, struct variable *v)
{
    char c[TMP_LEN];
    int i, flag=0;
    strcpy(c,value);
    for (i=0;i<strlen(value);i++){
        if (c[i]==' ') {
            c[i]='+';
            flag=1;
        }
        if (!(isalnum(c[i]) || (c[i]=='+') || (c[i]=='/') || (c[i]=='='))) {
            printf("Invalid <b>%s</b> %s: "
                    "must be base64 code<br>",
                    v->longname, value);
            return FALSE;
        }
    }
    if (flag) strcpy(value,c);
    return TRUE;
}
