#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include <cyutils.h>

#include "common.h"
#include "lang.h"
#include "ezp.h"
#include "ezp-lib.h"

#define EZP_DHCP_SERVER_LEASE_FILE_PATH     "/var/dhcp.leases"
#define EZP_PROC_NET_ARP_PATH               "/proc/net/arp"

int
ej_show_license_status(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *status = nvram_safe_get("license_invalid");

    if (!strcmp(status, "0")) {
        websWrite(wp, lang_translate("Authorized"));
    } else if (!strcmp(status, "1")) {
        websWrite(wp, lang_translate("Unauthorized"));
        websWrite(wp, "(%s)", nvram_safe_get("bootcount"));
    } else if (!strcmp(status, "2")) {
        websWrite(wp, lang_translate("Unauthorized"));
        websWrite(wp, "(%s)", lang_translate("Locked"));
    } else {
        websWrite(wp, lang_translate("Unknown"));
    }
    return 0;
}

int
ej_togo_show_wan_proto(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("togo_wan_proto.asp", wp, buf, "");
    }

    return 0;
}

int
ej_st_show_wan_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
#ifdef EZP_PROD_BRAND_PROX
    for (i = num-1; i >=0; i--) {
#else
    for (i = 0; i < num; i++) {
#endif
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_wan_setting.asp", wp, buf, "");
    }

    return 0;
}

int
ej_st_show_wan_setting_bytype(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN];
    char wan_type[10];
    int num;
    int i;
    char wan_state[5];
    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }
    for (i = 0; i < num; i++) {
        memset(buf,'\0',SHORT_BUF_LEN);
        snprintf(buf, SHORT_BUF_LEN, "wan%d_proto", i);
        strncpy(wan_type,nvram_safe_get(buf),10);
        memset(buf,'\0',SHORT_BUF_LEN);
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        ezplib_get_attr_val("wan_status_rule", i, "state", wan_state, sizeof(wan_state), EZPLIB_USE_CLI);
        if(strcmp(wan_type,"wisp") == 0) {
            switch (atoi(wan_state)) {
                case 0:
                case 1:
                    do_ej("st_wisp_wan_setting.asp", wp, buf, "Disconnected", "");
                    break;
                case 2:
                case 3:
                    do_ej("st_wisp_wan_setting.asp", wp, buf, "Connecting", "");
                    break;
                case 4:
                    do_ej("st_wisp_wan_setting.asp", wp, buf, "Connected", "");
                    break;
                default :
                    do_ej("st_wisp_wan_setting.asp", wp, buf, "Unknown", "");
                    break;
            }
        } else if(!strcmp(wan_type,"static") || !strcmp(wan_type,"pppoe") || !strcmp(wan_type,"dhcp")) {
            switch (atoi(wan_state)) {
                case 0:
                    do_ej("st_ether_wan_setting.asp", wp, buf, "Unplugged","");
                    break;
                case 1:
                case 2:
                case 3:
                    do_ej("st_ether_wan_setting.asp", wp, buf, "Connecting", "");
                    break;
                case 4:
                    do_ej("st_ether_wan_setting.asp", wp, buf, "Connected", "");
                    break;
                default :
                    do_ej("st_ether_wan_setting.asp", wp, buf, "Unknown", "");
                    break;
            }
        } else if(!strncmp(wan_type,"htc",3) || !strncmp(wan_type,"barry",5) || !strncmp(wan_type,"iphone",6) ||  !strncmp(wan_type,"wwan",4)) {
            switch (atoi(wan_state)) {
                case 0:
                    do_ej("st_usb_wan_setting.asp", wp, buf, "Unplugged","");
                    break;
                case 1:
                case 2:
                case 3:
                    do_ej("st_usb_wan_setting.asp", wp, buf, "Connecting", "");
                    break;
                case 4:
                    do_ej("st_usb_wan_setting.asp", wp, buf, "Connected", "");
                    break;
                default :
                    do_ej("st_usb_wan_setting.asp", wp, buf, "Unknown", "");
                    break;
            }
        }
    }

    return 0;
}

int
ej_st_show_wan_priv_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wan_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_wan_priv_setting.asp", wp, buf, "");
    }

    return 0;
}

int
ej_st_show_lan_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("lan_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_lan_setting.asp", wp, buf, "");
    }

    return 0;
}

int
ej_st_show_lan_priv_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("lan_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_lan_priv_setting.asp", wp, buf, "");
    }

    return 0;
}

int
ej_st_show_wl_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wl_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_wl_setting.asp", wp, buf, "");
    }

    return 0;
}

int
ej_st_show_wl_priv_setting(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cur = nvram_safe_get("wl_num");
    char buf[SHORT_BUF_LEN];
    int num;
    int i;

    if (!*cur) {
        num = 1;
    } else {
        num = atoi(cur);
    }

    for (i = 0; i < num; i++) {
        snprintf(buf, SHORT_BUF_LEN, "%d", i);
        do_ej("st_wl_priv_setting.asp", wp, buf, "");
    }

    return 0;
}

int
ej_show_ipaddr_prefix24_calc(int eid, webs_t wp, int argc, char_t ** argv)
{
    char tmp[32];
    char *name, *type, *rule;
    int start, num;
    int nth;
    int ret = -1;

    if (ejArgs(argc, argv, "%s %s %d", &type, &rule, &nth) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    /* IP address */
    name = nvram_safe_get(type);
    /* Start XXX: Check the return value. */
    ezplib_get_attr_val(rule, nth, "start", tmp, 32, EZPLIB_USE_CLI);
    start = atoi(tmp);
    /* End  XXX: Check the return value. */
    ezplib_get_attr_val(rule, nth, "num", tmp, 32, EZPLIB_USE_CLI);
    num = atoi(tmp);

    if (show_ipaddr_prefix24_calc(name, start, num , tmp, 32)) {
        ret = websWrite(wp,"%s", lang_translate("Not Available"));
        return ret;
    } else {
        ret = websWrite(wp, "%s", tmp);
        return 0;
    }
}

int
ej_st_user_show_dhcp_user_num(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *fp;
    unsigned long expires;
    char ip[16], mac[32], mac2[64], name[255];
    time_t now;
    int count = 0;

    /* Parse leases file */
    if ((fp = fopen(EZP_DHCP_SERVER_LEASE_FILE_PATH, "r"))) {

        /* Get current time. */
        now = time(NULL);

        while (fscanf(fp, "%lu %s %s %s %s",
                    &expires, mac, ip, name, mac2) == 5) {
            count++;
        }
        if (count > 1) {
            websWrite(wp, "%d %s", count, lang_translate("users"));
        } else {
            websWrite(wp, "%d %s", count, lang_translate("user"));
        }

        fclose(fp);
    }
    return 0;
}

int
ej_st_user_show_dhcp(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *fp;
    unsigned long expires;
    char ip[16], mac[32], mac2[64], name[255], expires_time[64];
    time_t now;
#ifdef EZP_PROD_BRAND_AXIM
    int DataCounter;
    DataCounter = 0;
    websWrite(wp, "var DHCPData = [");
#endif


    /* Parse leases file */
    if ((fp = fopen(EZP_DHCP_SERVER_LEASE_FILE_PATH, "r"))) {

        /* Get current time. */
        now = time(NULL);

        while (fscanf(fp, "%lu %s %s %s %s",
                    &expires, mac, ip, name, mac2) == 5) {

            expires_time[0] = '\0';
            expires -= (unsigned long) now;

            if (expires > 60 * 60 * 24) {
                sprintf(expires_time + strlen(expires_time),
                        "%ld %s, ", expires / (60 * 60 * 24), 
                        lang_translate("day(s)"));

                expires %= 60 * 60 * 24;
            }
            if (expires > 60 * 60) {
                sprintf(expires_time + strlen(expires_time), "%02ld:", 
                        expires / (60 * 60));
                expires %= 60 * 60;
            } else {
                sprintf(expires_time + strlen(expires_time), "00:");        
            }
            if (expires > 60) {
                sprintf(expires_time + strlen(expires_time), "%02ld:", 
                        expires / 60);
                expires %= 60;
            } else {
                sprintf(expires_time + strlen(expires_time), "00:");
            }
            sprintf(expires_time + strlen(expires_time), "%02ld", expires);


#ifdef EZP_PROD_BRAND_AXIM
	    if (DataCounter == 0) {
	    	websWrite(wp, "{Name:\"%s\",IP:\"%s\",MAC:\"%s\",Expiration:\"%s\"}", name, ip, mac, expires_time);
	    } else {
	    	websWrite(wp, ",{Name:\"%s\",IP:\"%s\",MAC:\"%s\",Expiration:\"%s\"}", name, ip, mac, expires_time);
	    }
	    DataCounter ++;
#else
            websWrite(wp, "<tr><td>%s</td><td>%s</td><td>%s</td>"
                    "<td>%s</td></tr>", name, ip, mac, expires_time);
#endif

        }

#ifdef EZP_PROD_BRAND_AXIM
        websWrite(wp, "];\n");
#endif

        fclose(fp);
    }
    return 0;
}

int
ej_st_user_show_arp_user_num(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *fp;
    unsigned int hw_type, arp_type;
    char ip[SHORT_BUF_LEN], mac[SHORT_BUF_LEN], mask[SHORT_BUF_LEN], 
         ifname[SHORT_BUF_LEN];
    int count = 0;

    /* Parse ARP file */
    if ((fp = fopen(EZP_PROC_NET_ARP_PATH, "r"))) {

        /* Eat the fist line (schema). */
        while (fgetc(fp) != '\n');

        while (fscanf(fp, "%s %x %x %s %s %s",
                    ip, &hw_type, &arp_type, mac, mask, ifname) == 6) {
            count++;
        }
        if (count > 1) {
            websWrite(wp, "%d %s", count, lang_translate("users"));
        } else {
            websWrite(wp, "%d %s", count, lang_translate("user"));
        }
        fclose(fp);
    }

    return 0;
}

int
ej_st_user_show_arp(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *fp;
    unsigned int hw_type, arp_type;
    char ip[SHORT_BUF_LEN], mac[SHORT_BUF_LEN], mask[SHORT_BUF_LEN], 
         ifname[SHORT_BUF_LEN];
    char *arp_type_name;

#ifdef EZP_PROD_BRAND_AXIM
    int DataCounter;
    DataCounter = 0;
    websWrite(wp, "var ARPData = [");
#endif

    /* Parse ARP file */
    if ((fp = fopen(EZP_PROC_NET_ARP_PATH, "r"))) {

        /* Eat the fist line (schema). */
        while (fgetc(fp) != '\n');

        while (fscanf(fp, "%s %x %x %s %s %s",
                    ip, &hw_type, &arp_type, mac, mask, ifname) == 6) {

            if (arp_type == 0x06) {
                arp_type_name = "Static";
            } else if (arp_type == 0x02) {
                arp_type_name = "Dynamic";
            } else {
                arp_type_name = "Unknown";
            }

            /* XXX: Add iface in the future! */
#ifdef EZP_PROD_BRAND_AXIM
	    if ( DataCounter == 0 ) {
		websWrite(wp, "{IP:\"%s\",MAC:\"%s\",ARPType:\"%s\"}", ip, mac, lang_translate(arp_type_name));
	    } else {
		websWrite(wp, ",{IP:\"%s\",MAC:\"%s\",ARPType:\"%s\"}", ip, mac, lang_translate(arp_type_name));
	    }
            DataCounter++;
#else
            websWrite(wp, "<tr><td>%s</td><td>%s</td><td>%s</td></tr>", 
                    ip, mac, lang_translate(arp_type_name));
#endif

        }
#ifdef EZP_PROD_BRAND_AXIM
	websWrite(wp, "];\n");
#endif

        fclose(fp);
    }

    return 0;
}


