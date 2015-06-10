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
        ret = websWrite(wp, lang_translate("Not Available"));
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
	websWrite(wp, " jQuery('#DHCPListData').jqGrid({\n");
	websWrite(wp, "         data: DHCPData,\n");
	websWrite(wp, "         datatype: 'local',\n");
	websWrite(wp, "         colNames:[Lang_Name, Lang_IP, Lang_MAC, Lang_Exp],\n");
	websWrite(wp, "         colModel:[\n");
	websWrite(wp, "                 {name:'Name',index:'Name', width:330},\n");
	websWrite(wp, "                 {name:'IP',index:'IP', width:150},\n");
	websWrite(wp, "                 {name:'MAC',index:'MAC', width:150, align:'left'},\n");
	websWrite(wp, "                 {name:'Expiration',index:'Expiration', width:100}\n");
	websWrite(wp, "         ],\n");
	websWrite(wp, "         height: 'auto',\n");
	websWrite(wp, "         rowNum: 10,\n");
	websWrite(wp, "         rowList: [10,20,30],\n");
	websWrite(wp, "         sortname: 'Name',\n");
	websWrite(wp, "         sortorder: 'asc',\n");
	websWrite(wp, "         pager: '#DHCPListDataPager',\n");
	websWrite(wp, "         viewrecords: true,\n");
	websWrite(wp, "         caption: Lang_Caption\n");
	websWrite(wp, " });\n");
	websWrite(wp, " jQuery('#DHCPListData').jqGrid('navGrid','#DHCPListDataPager',{edit: false,add: false,del: false,search: false,refresh: false}\n");
	websWrite(wp, " );\n");
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
	websWrite(wp, "jQuery('#ARPListData').jqGrid({\n");
	websWrite(wp, "        data: ARPData,\n");
	websWrite(wp, "        datatype: 'local',\n");
	websWrite(wp, "        colNames:[ Lang_IP, Lang_MAC, Lang_Type],\n");
	websWrite(wp, "        colModel:[\n");
	websWrite(wp, "                {name:'IP',index:'IP', width:350},\n");
	websWrite(wp, "                {name:'MAC',index:'MAC', width:190},\n");
	websWrite(wp, "                {name:'ARPType',index:'ARPType', width:100, align:'left'}\n");
	websWrite(wp, "        ],\n");
	websWrite(wp, "        autowidth: 'auto',\n");
	websWrite(wp, "        width: 750,\n");
	websWrite(wp, "        height: 'auto',\n");
	websWrite(wp, "        rowNum: 10,\n");
	websWrite(wp, "        rowList: [10,20,30],\n");
	websWrite(wp, "        sortname: 'IP',\n");
	websWrite(wp, "        sortorder: 'asc',\n");
	websWrite(wp, "        pager: '#ARPListDataPager',\n");
	websWrite(wp, "        viewrecords: true,\n");
	websWrite(wp, "        caption: Lang_Caption\n");
	websWrite(wp, "});\n");
	websWrite(wp, "\n");
	websWrite(wp, "jQuery('#ARPListData').jqGrid('navGrid','#ARPListDataPager',{edit: false,add: false,del: false,search: false,refresh: false}\n");
	websWrite(wp, ");\n");
#endif

        fclose(fp);
    }

    return 0;
}


