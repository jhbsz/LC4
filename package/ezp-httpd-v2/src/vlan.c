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
#define SSID_MAX 2
#define VLAN_PORT_MAX 5
#define WLAN_NAME "ra"
#define WDS_NAME "wds"
#define UNIVERSAL_REPEATER_NAME "apcli"
#define WDS_MAX 4
#define HWIFNAME "eth2"
#define HIDDEN_PORTMEMBER "11"
enum {
    VLAN_NAME = 0,
    VLAN_ENABLE,
    VLAN_VID,
    VLAN_TAG,
    VLAN_P1,
    VLAN_P2,
    VLAN_P3,
    VLAN_P4,
    VLAN_P5,
    VLAN_S1,
    VLAN_S2,
    VLAN_WDS,
    VLAN_UR,
    VLANPORT_NAME,
    VLANPORT_PVID,
    VLANPORT_TAG,
};

static struct variable vlan_variables[] = {
    {longname: "VLAN NAME", argv:ARGV("20"), nullok: FALSE},
    {longname: "VLAN Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN VID", argv:ARGV("1","4094"), nullok: FALSE},
    {longname: "VLAN TAG", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN P1", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN P2", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN P3", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN P4", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN P5", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN S1", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN S2", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN WDS", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN UR", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "VLAN Port Name", argv:ARGV("20"), nullok: FALSE},
    {longname: "VLAN Port PVID", argv:ARGV("1","4096"), nullok: FALSE},
    {longname: "VLAN Port Tag", argv:ARGV("0","1"), nullok: FALSE},
};

/* vlan port member transfom to 0 or 1 */
void 
vlan_port_trans(char* portmember, int len)
{
   char buf[SHORT_BUF_LEN];
   int i;
   for(i = 0;i < len; i++) {
      if(portmember[i] == '1') {
          char *temp = "1^";
          strcat(buf,temp);
      } else {
          char *temp = "0^";
          strcat(buf,temp);
      }
   }
   buf[strlen(buf) - 1] = '\0';
   strcpy(portmember, buf);
}

/* 
 * If each ifname turn on, transform to 0 or 1.
 * e.g. vlan2 ra0 apcli0 => "XXXX"
 */
void
vlan_ifnames_trans(char *ifnames)
{
	char buf[TMP_LEN] = {0},
		 wl[SHORT_BUF_LEN];
	int i,
        wlv_num = atoi(nvram_safe_get("wlv_rule_num"));
	
    /* ARCH: NB: Please keep the squence here. 1. wireless 2. wds 3. apcli. */
	for(i = 0;i < wlv_num; i++) {
		snprintf(wl, sizeof(wl), "%s%d", WLAN_NAME,i);
		if (strstr(ifnames, wl)) {
			strcat(buf, "1^");
		} else {
			strcat(buf, "0^");	
		}
	}
	
	/* check "wds" */
	if (strstr(ifnames, WDS_NAME)) {
		strcat(buf, "1^");
	} else {
		strcat(buf, "0^");	
	}
	
	/* check "apcli" */
	if (strstr(ifnames, UNIVERSAL_REPEATER_NAME)) {
		strcat(buf, "1^");
	} else {
		strcat(buf, "0^");	
	}
	buf[strlen(buf) - 1] = '\0';
	strcpy(ifnames, buf);
}

int
ej_vlan_show_content(int eid, webs_t wp, int argc, char_t **argv)
{
    char buf[LONG_BUF_LEN] = {0}, buf2[LONG_BUF_LEN] = {0};
    int total_used = 0, i, j, rule_num, rule_max;
    /* Get how many vlan rule */
    ezplib_get_rule("vlan_rule_max", 0, buf, LONG_BUF_LEN);
    rule_max = atoi(buf); 

    ezplib_get_rule("vlan_rule_num", 0, buf, LONG_BUF_LEN);
    rule_num = atoi(buf);

    for(i = 0;i < rule_num; i++) {
        char name[SHORT_BUF_LEN],enable[SHORT_BUF_LEN],vid[SHORT_BUF_LEN],
             port[SHORT_BUF_LEN];
        char ifnames[TMP_LEN];

        ezplib_get_attr_val("vlan_rule", i, "name", name, sizeof(name),
                EZPLIB_USE_CLI);
        ezplib_get_attr_val("vlan_rule", i, "enable", enable, sizeof(enable),
                EZPLIB_USE_CLI);
        ezplib_get_attr_val("vlan_rule", i, "vid", vid, sizeof(vid),
                EZPLIB_USE_CLI);
        ezplib_get_attr_val("vlan_rule", i, "portmember", port, 
                sizeof(port), EZPLIB_USE_CLI);
        ezplib_get_attr_val("br_rule", i, "ifnames", ifnames, sizeof(ifnames),
                EZPLIB_USE_CLI);

        /* If platform is amit cde570, the switch port config is 
         * different with other router */
        if (strstr("AMC|AMD|GMA", nvram_safe_get("platform"))) {
            /* change port member form p5 p1 p2 p3 p4
             *                      to p1 p2 p3 p4 p5  */
            bzero(buf, sizeof(buf));
            int vlanport_num = atoi(nvram_safe_get("vlanport_rule_num"));
            for (j = 1; j < vlanport_num; j++) {
                buf[j - 1] = port[j];
            }
            buf[j - 1] = port[0];
            strcpy(port, buf);
        }

        vlan_port_trans(port, VLAN_PORT_MAX);
        vlan_ifnames_trans(ifnames);

        snprintf(buf, sizeof(buf), "%s^%s^%s^%s^%s",
                name, enable, vid, port, ifnames);
        strcat(buf2, buf);
        buf2[strlen(buf2) + 1] = '\0';
        buf2[strlen(buf2)] = '|';
    }    
    buf2[strlen(buf2) - 1] = '\0';
    total_used = strlen(buf2);
    strcpy(buf, buf2);
    /* XXX: FIXME! */
#ifdef EZP_PROD_BRAND_AXIM
    websWrite2(wp, "vlan_data = \"%s\";\n", buf);
#else
    trans2web(buf, total_used, buf2);
    websWrite2(wp, "vlan_data = [['%s']];\n", buf2);
#endif

    websWrite2(wp, "var rule_max = %d;\n", rule_max);
    /* Get how many vlanport rule */
    rule_num = atoi(nvram_get("vlanport_rule_num"));
    buf[0] = 0;

    for(i = 0;i < rule_num; i++) {
        char name[SHORT_BUF_LEN], portid[SHORT_BUF_LEN], pvid[SHORT_BUF_LEN],
             tag[SHORT_BUF_LEN], tmp[TMP_LEN];

        ezplib_get_attr_val("vlanport_rule", i, "name", name, sizeof(name),
                EZPLIB_USE_CLI);
        ezplib_get_attr_val("vlanport_rule", i, "portid", portid, sizeof(portid)              , EZPLIB_USE_CLI);
        ezplib_get_attr_val("vlanport_rule", i, "pvid", pvid, sizeof(pvid),
                EZPLIB_USE_CLI);
        ezplib_get_attr_val("vlanport_rule", i, "tag", tag, sizeof(tag), 
                EZPLIB_USE_CLI);
        /* ar8316 tag value is 2 or 1 */
        if (!strcmp(nvram_safe_get("switch_type"), "ar8316"))
            tag[0] -= 1;

        if (strstr("AMC|AMD|GMA", nvram_safe_get("platform"))) {
            if (i > 0) {
                name[4] += (5-i*2);
                snprintf(tmp, sizeof(tmp), "|%s^%s^%s", name, pvid, tag);
                strcat(tmp, buf);
                snprintf(buf, sizeof(buf), "%s", tmp);
                if(i == rule_num - 1) {
                    strcat(buf2,buf);
                    snprintf(buf, sizeof(buf), "%s", buf2);
                }
            } else  {
                name[4] = '1';
                snprintf(buf2, sizeof(buf2), "%s^%s^%s", name, pvid, tag);
            }
        } else {
            if(i != 0)
                strcat(buf, "|");
            snprintf(tmp, sizeof(tmp), "%s^%s^%s", name, pvid, tag);
            strcat(buf,tmp);
        }
    }
    total_used = strlen(buf);
    bzero(buf2,sizeof(buf2));

#ifdef EZP_PROD_BRAND_AXIM
    websWrite2(wp, "vlan_port_data = \"%s\";\n", buf);
#else
    trans2web(buf, total_used, buf2);
    websWrite2(wp, "vlan_port_data = [['%s']];\n", buf2);
#endif
    websWrite2(wp, "var vlan_port_rule_max = %d;\n", rule_num);

    return 0;    
}

int
ej_vlan_js(int eid, webs_t wp, int argc, char_t **argv)
{
    char *type, *platform;
    int i;
    int vlan_port_num = atoi(nvram_safe_get("vlanport_rule_num"));
    int wlv_num = atoi(nvram_safe_get("wlv_rule_num"));
    char buf[LONG_BUF_LEN] = {0}, tmp[TMP_LEN];
    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websDebugWrite(wp, "Insufficient args\n");
        return -1;
    }
    platform = nvram_safe_get("platform");
    if (!strcmp(type, "element")) {
        for (i = 0; i < vlan_port_num; i++) {
            if (i != 0)
                strcat(buf, ",");
#ifdef EZP_PROD_BRAND_AXIM
	    snprintf(tmp, sizeof(tmp), "p%d", i + 1);
#else
            snprintf(tmp, sizeof(tmp), "\'p%d\'", i + 1);
#endif
            strcat(buf, tmp);

        }
        /* If platform contain wireless function */
        if (strcmp("AMC", platform)) {
            strcat(buf, ",");
            for (i = 0; i < wlv_num; i++) {
                if (i != 0)
                    strcat(buf, ",");
#ifdef EZP_PROD_BRAND_AXIM
		snprintf(tmp, sizeof(tmp), "s%d", i + 1);
#else
                snprintf(tmp, sizeof(tmp), "\'s%d\'", i + 1);
#endif
                strcat(buf, tmp);
            }
#ifdef EZP_PROD_BRAND_AXIM
            strcat(buf, ",wds,ur");
#else
            strcat(buf, ",'wds','ur'");
#endif
        }
        websWrite(wp, buf);
    } else if (!strcmp(type, "vlan_table")) {
        char comment[10];
        int wan_num = atoi(nvram_safe_get("wan_num"));
        int lan_num = vlan_port_num - wan_num;
        if (wan_num > 1 && appmgr_check("wwan")) lan_num++; 
        /* construct vlan table switch port column element */
        for (i = 1; i <= vlan_port_num; i++) {
            if (i <= lan_num) {
                snprintf(comment, sizeof(comment), "L%d", lan_num - i + 1);
            } else {
                snprintf(comment, sizeof(comment), "W%d",
                        vlan_port_num - i + 1);
            }
#ifdef EZP_PROD_BRAND_AXIM
            websWrite(wp, "colNames_List.push(\"Port%d(%s)\");\n", 
                    vlan_port_num - i + 1, comment);
            websWrite(wp, "colModel_List.push({name:'p%d',index:'p%d', width:6, align:'center', editable:true, edittype:'checkbox', editoptions:{value:'1:0'},formatter:'checkbox', formatoptions:{disabled:0}});\n", i, i);
#else
            snprintf(tmp, sizeof(tmp), "var checkColumn_p%d = new Ext.grid.CheckColumn({header: 'Port%d(%s)', width: 40, dataIndex: 'p%d' });\n",
                    i, vlan_port_num - i + 1, comment, i);
            strcat(buf, tmp);
#endif
        }
        /* If platform contain wireless function */
        if (strcmp("AMC", platform)) {
            /* construct wlan table wireless column element */
            for (i = 1; i <= wlv_num; i++) {
#ifdef EZP_PROD_BRAND_AXIM
		websWrite(wp, "colNames_List.push(\"SSID%d\");\n",i);
        websWrite(wp, "colModel_List.push({name:'s%d',index:'s%d', width:6, align:'center', editable:true, edittype:'checkbox', editoptions:{value:'1:0'},formatter:'checkbox', formatoptions:{disabled:0}});\n", i, i);
#else
                snprintf(tmp, sizeof(tmp), "var radioColumn_s%d = new Ext.grid.RadioColumn({ header: 'SSID%d', width: 30, dataIndex: 's%d'});\n",
                        i, i, i);
            	strcat(buf, tmp);
#endif
            }
#ifdef EZP_PROD_BRAND_AXIM
	    websWrite(wp, "colNames_List.push(\"WDS\");\n");
        websWrite(wp, "colModel_List.push({name:'wds',index:'wds', width:6, align:'center', editable:true, edittype:'checkbox', editoptions:{value:'1:0'},formatter:'checkbox', formatoptions:{disabled:0}});\n");
	    websWrite(wp, "colNames_List.push(\"UR\");\n");
        websWrite(wp, "colModel_List.push({name:'ur',index:'ur', width:6, align:'center', editable:true, edittype:'checkbox', editoptions:{value:'1:0'},formatter:'checkbox', formatoptions:{disabled:0}});\n");
#else
            strcat(buf, "var radioColumn_wds = new Ext.grid.RadioColumn({ header: 'WDS', width: 30, dataIndex: 'wds'});\n");
            strcat(buf, "var radioColumn_ur = new Ext.grid.RadioColumn({ header: 'UR', width: 30, dataIndex: 'ur'});\n");
#endif
        }
#ifndef EZP_PROD_BRAND_AXIM
        websWrite(wp, buf);
#endif
    } else if (!strcmp(type, "cm")) {
        for (i = 0; i < vlan_port_num; i++) {
            if (i != 0)
                strcat(buf, ",\n");
            snprintf(tmp, sizeof(tmp), "checkColumn_p%d", i + 1);
            strcat(buf, tmp);
        }
        /* If platform contain wireless function */
        if (strcmp("AMC", platform)) {
            strcat(buf, ",\n");
            for (i = 0; i < wlv_num; i++) {
                if (i != 0)
                    strcat(buf, ",\n");
                snprintf(tmp, sizeof(tmp), "radioColumn_s%d", i + 1);
                strcat(buf, tmp);
            }
            strcat(buf, ",\nradioColumn_wds,\nradioColumn_ur");
        }
        websWrite(wp, buf);
    } else if (!strcmp(type, "store")) {
        for (i = 0; i < vlan_port_num; i++) {
            if (i != 0)
                strcat(buf, ",\n");
            snprintf(tmp, sizeof(tmp), "{name: 'p%d', type: 'bool'}", i + 1);
            strcat(buf, tmp);
        }
        /* If platform contain wireless function */
        if (strcmp("AMC", platform)) {
            strcat(buf, ",\n");
            for (i = 0; i < wlv_num; i++) {
                if (i != 0)
                    strcat(buf, ",\n");
                snprintf(tmp, sizeof(tmp), "{name: 's%d', type: 'bool'}",
                        i + 1);
                strcat(buf, tmp);
            }
            strcat(buf, ",\n{name: 'wds', type: 'bool'},\n{name: 'ur', type: 'bool'}\n");
        }
        websWrite(wp, buf);
    } else if (!strcmp(type, "sm")) {
        for (i = 0; i < vlan_port_num; i++) {
            if (i != 0)
                strcat(buf, ",\n");
            snprintf(tmp, sizeof(tmp), "p%d:false", i + 1);
            strcat(buf, tmp);
        }
        /* If platform contain wireless function */
        if (strcmp("AMC", platform)) {
            strcat(buf, ",\n");
            for (i = 0; i < wlv_num; i++) {
                if (i != 0)
                    strcat(buf, ",\n");
                snprintf(tmp, sizeof(tmp), "s%d:false", i + 1);
                strcat(buf, tmp);
            }
            strcat(buf, ",\nwds:false,\nur:false");
        }
        websWrite(wp, buf);
    }
    return 0;
}

int
valid_vlan(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    int i, nrule;

    snprintf(tmp, sizeof(tmp), "vlan_rule_num");
    nrule = atoi(websGetVar(wp, tmp, "0"));
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "vlan_rule%d_name", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &vlan_variables[VLAN_NAME]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "vlan_rule%d_enable", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLAN_ENABLE]) == FALSE) {
            return FALSE;
        }

        snprintf(tmp, sizeof(tmp), "vlan_rule%d_vid", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLAN_VID]) == FALSE) {
            return FALSE;
        }
        snprintf(tmp, sizeof(tmp), "vlan_rule%d_p1", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLAN_P1]) == FALSE) {
            return FALSE;
        }
        snprintf(tmp, sizeof(tmp), "vlan_rule%d_p2", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLAN_P2]) == FALSE) {
            return FALSE;
        }
        snprintf(tmp, sizeof(tmp), "vlan_rule%d_p3", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLAN_P3]) == FALSE) {
            return FALSE;
        }
        snprintf(tmp, sizeof(tmp), "vlan_rule%d_p4", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLAN_P4]) == FALSE) {
            return FALSE;
        }
        snprintf(tmp, sizeof(tmp), "vlan_rule%d_p5", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLAN_P5]) == FALSE) {
            return FALSE;
        }
        /* If the platform contain wireless function */
        if (strcmp("AMC", nvram_safe_get("platform"))) {
            snprintf(tmp, sizeof(tmp), "vlan_rule%d_s1", i);
            val = websGetVar(wp, tmp, "");
            if (valid_number(wp, val, &vlan_variables[VLAN_S1]) == FALSE) {
                return FALSE;
            }
            snprintf(tmp, sizeof(tmp), "vlan_rule%d_s2", i);
            val = websGetVar(wp, tmp, "");
            if (valid_number(wp, val, &vlan_variables[VLAN_S2]) == FALSE) {
                return FALSE;
            }
            snprintf(tmp, sizeof(tmp), "vlan_rule%d_wds", i);
            val = websGetVar(wp, tmp, "");
            if (valid_number(wp, val, &vlan_variables[VLAN_WDS]) == FALSE) {
                return FALSE;
            }
            snprintf(tmp, sizeof(tmp), "vlan_rule%d_ur", i);
            val = websGetVar(wp, tmp, "");
            if (valid_number(wp, val, &vlan_variables[VLAN_UR]) == FALSE) {
                return FALSE;
            }
        }
    }
    nrule = atoi(websGetVar(wp, "vlanport_rule_num", "0"));
    for (i = 0; i < nrule; i++) {
        snprintf(tmp, sizeof(tmp), "vlanport_rule%d_name", i);
        val = websGetVar(wp, tmp, "");
        if (valid_name(wp, val, &vlan_variables[VLANPORT_NAME]) == FALSE) {
            return FALSE;
        }
        snprintf(tmp, sizeof(tmp), "vlanport_rule%d_pvid", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLANPORT_PVID]) == FALSE) {
            return FALSE;
        }
        snprintf(tmp, sizeof(tmp), "vlanport_rule%d_tag", i);
        val = websGetVar(wp, tmp, "");
        if (valid_number(wp, val, &vlan_variables[VLANPORT_TAG]) == FALSE) {
            return FALSE;
        }
    }
    
    return TRUE;
}

static int
_build_vlan_rule(webs_t wp, int vlan_rule_num, char *buf, int buf_len) {
    int i, j, len;
    char tmp[TMP_LEN], portmember[SHORT_BUF_LEN] = {0}, 
         orig_portmember[SHORT_BUF_LEN];
    char *name, *enable, *vid;
    int vlanport_num = atoi(nvram_safe_get("vlanport_rule_num")),
        vlanport_max = atoi(nvram_safe_get("vlanport_rule_max"));

    for (i = 0; i < vlan_rule_num; i++) {
        snprintf(tmp, sizeof(tmp), "vlan_rule%d_name", i);
        name = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "vlan_rule%d_enable", i);
        enable = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "vlan_rule%d_vid", i);
        vid = websGetVar(wp, tmp, "");

        /* Generate port member list */
        for (j = 1; j <= vlanport_num; j++) {
            snprintf(tmp, sizeof(tmp), "vlan_rule%d_p%d", i, j);
            strcat(portmember,websGetVar(wp, tmp, ""));
        }
        /* If platform is amit cde570, the switch port config is 
         * different with other router */
        if (strstr("AMC|AMD|GMA", nvram_safe_get("platform"))) {
            /* change port member form p1 p2 p3 p4 p5 
             *                      to  p5 p1 p2 p3 p4 */
            for (j = 1; j < vlanport_num; j++) {
                tmp[j] = portmember[j - 1];
            }
            tmp[0] = portmember[j - 1];
            strcpy(portmember, tmp);
        }
        /* Because we are not to replace CPU and GMAC port, 
         * it is required to remain these 2 entries. */
        ezplib_get_attr_val("vlan_rule", i, "portmember",
                orig_portmember, sizeof(orig_portmember), EZPLIB_USE_CLI);
        if(orig_portmember[vlanport_num]) {
            for (j = vlanport_num; j < vlanport_max; j++) {
                portmember[j] = orig_portmember[j];
            }
        }
        else {
            for (j = vlanport_num; j < vlanport_max; j++) {
                portmember[j] = '1';
            }
        }
        portmember[vlanport_max] = '\0';
        
        /* Generate vlan rule entry */
        if(i) {
            len = snprintf(buf, buf_len, "%s|%s^%s^%s^%s^%s", 
                    buf, name, enable, HWIFNAME, vid, portmember);
        }
        else {
            len = snprintf(buf, buf_len, "%s^%s^%s^%s^%s", 
                    name, enable, HWIFNAME, vid, portmember);
        }
        portmember[0] = '\0';
    }
    printf("vlan rule %s\n",buf);
    if (len >= buf_len) {
        return 1;
    }
    return 0;
}

static int
_build_vlanport_rule(webs_t wp, char *buf, int buf_len) {
    int i, len;
    char tmp[TMP_LEN] = {0}, buf2[TMP_LEN] = {0};
    char mode[TMP_LEN], gmemb[TMP_LEN];
    char *name, *tag, *pvid, *prio = "1";

    int vlanport_num = atoi(nvram_safe_get("vlanport_rule_num")),
        vlanport_max = atoi(nvram_safe_get("vlanport_rule_max"));

    bzero(buf, buf_len);

    for (i = 0; i < vlanport_num; i++){

        snprintf(tmp, sizeof(tmp), "vlanport_rule%d_name", i);
        name = websGetVar(wp, tmp, "");
        snprintf(tmp, sizeof(tmp), "vlanport_rule%d_pvid", i);
        pvid = websGetVar(wp, tmp, "");
        snprintf(tmp, sizeof(tmp), "vlanport_rule%d_tag", i);
        tag = websGetVar(wp, tmp, "");
        /* switch type */
        if (!strcmp(nvram_safe_get("switch_type"),"ar8316")) { 
            /* AR8316 */
            tag[0] += 1;
            /* If platform is amit cde570, the switch port config is 
             * different with other router */
            if (strstr("AMC|AMD|GMA", nvram_safe_get("platform"))) {
                /* Generate vlan rule entry */
                /* shift vlanport_rule from P1|P2|P3|P4|P5
                 *                      to  P5|P1|P2|P3|P4 */
                if (i == (vlanport_num - 1)) { /* PORT 5 */
                    ezplib_get_attr_val("vlanport_rule", 0, "mode",
                            mode, sizeof(mode), EZPLIB_USE_CLI);
                    ezplib_get_attr_val("vlanport_rule", 0, "gmemb",
                            gmemb, sizeof(gmemb), EZPLIB_USE_CLI);
                    name[4] = '1';
                    len = snprintf(buf, buf_len, "%s^%d^%s^%s^%s^%s^%s%s",
                            name, 0, pvid, tag, mode, gmemb, prio, buf2);
                } else { /* shift P1 P2 P3 P4 rule */
                    ezplib_get_attr_val("vlanport_rule", i+1, "mode",
                            mode, sizeof(mode), EZPLIB_USE_CLI);
                    ezplib_get_attr_val("vlanport_rule", i+1, "gmemb",
                            gmemb, sizeof(gmemb), EZPLIB_USE_CLI);
                    name[4] += 1;
                    len = snprintf(buf2, sizeof(buf2),
                            "%s|%s^%d^%s^%s^%s^%s^%s",
                            buf2, name, i + 1, pvid, tag, mode, gmemb, prio);
                }
            } else {
                ezplib_get_attr_val("vlanport_rule", i, "mode",
                        mode, sizeof(mode), EZPLIB_USE_CLI);
                ezplib_get_attr_val("vlanport_rule", i, "gmemb",
                        gmemb, sizeof(gmemb), EZPLIB_USE_CLI);
                /* Generate vlan rule entry */
                if (i) {
                    len = snprintf(buf, buf_len, "%s|%s^%d^%s^%s^%s^%s^%s",
                            buf, name, i, pvid, tag, mode, gmemb, prio);
                } else {
                    len = snprintf(buf, buf_len, "%s^%d^%s^%s^%s^%s^%s",
                            name, i, pvid, tag, mode, gmemb, prio);
                }
            }
        } else { 
            /* rt3052 ESW */
            /* Generate vlan rule entry */
            if (i) {
                len = snprintf(buf, buf_len, "%s|%s^%d^%s^%s^%s", 
                        buf, name, i, pvid, tag, prio);
            } else {
                len = snprintf(buf, buf_len, "%s^%d^%s^%s^%s", 
                        name, i, pvid, tag, prio);
            }
        }
    }
    /* We don't want user to set GMAC port(port5) and CPU port(port6)
     * but we still need to remain the correct nvram setting. */
    for (i = vlanport_num; i < vlanport_max; i++) {
        ezplib_get_rule("vlanport_rule", i, tmp, sizeof(tmp));
        len = snprintf(buf, buf_len, "%s|%s", buf, tmp);
        if (len >= buf_len) {
            return 1;
        }
    }
    if (len >= buf_len) {
        return 1;
    }
    return 0;
}

static int
_build_br_rule(webs_t wp, int vlan_rume_num, char *buf, int buf_len) {
    int i, j, br_enable = 0, len,
        wds_num = atoi(nvram_safe_get("wl0_wds_rule_num")),
        wlv_num = atoi(nvram_safe_get("wlv_rule_num"));
    char *name, *vid;
    char ifnames[TMP_LEN], tmp[TMP_LEN];

    for (i = 0; i < vlan_rume_num; i++) {
        br_enable = 0;
        snprintf(tmp, sizeof(tmp), "vlan_rule%d_name", i);
        name = websGetVar(wp, tmp, "");

        snprintf(tmp, sizeof(tmp), "vlan_rule%d_vid", i);
        vid = websGetVar(wp, tmp, "");

        snprintf(ifnames, sizeof(ifnames), "vlan%s", vid);
        /* If the platform contain wireless function */
        if(strcmp("AMC", nvram_safe_get("platform"))) {
            /* Generate wireless, wds and universal repeater interfaces */
            for (j = 0; j < wlv_num; j++) {
                snprintf(tmp, sizeof(tmp), "vlan_rule%d_s%d", i, j + 1);
                if(*websGetVar(wp, tmp, "") == '1') {
                    snprintf(ifnames, sizeof(ifnames), "%s %s%d", ifnames, WLAN_NAME, j);
                    br_enable = 1;
                }
            }

            snprintf(tmp, sizeof(tmp), "vlan_rule%d_wds", i);
            if (*websGetVar(wp, tmp, "") == '1') {
                for (j = 0; j < wds_num; j++) {
                    snprintf(ifnames, sizeof(ifnames), "%s %s%d", ifnames, WDS_NAME, j);
                }
                br_enable = 1;
            }

            snprintf(tmp, sizeof(tmp), "vlan_rule%d_ur", i);
            if (*websGetVar(wp, tmp, "") == '1') {
                strcat(ifnames, " apcli0");
                br_enable = 1;
            }
        } else if (!i) {
            /* If platform is AMC ,
             * add wireless device into the first br_rule */
            snprintf(ifnames, sizeof(ifnames), "%s %s",
                   ifnames, "ra0");
        }
        if (i) {
            len = snprintf(buf, buf_len, "%s|%s^%d^%s^", 
                    buf, name, br_enable, ifnames);
        }
        else {
            len = snprintf(buf, buf_len, "%s^%d^%s^", 
                    name, br_enable, ifnames);
        }
    }
    printf("br rule %s\n",buf);
    if (len >= buf_len) {
        return 1;
    }
    return br_enable;
}

int
save_vlan(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], *orig_rule;
    int change = 0, nrule, max, preact_needed = 1;
    int64_t map = 0;

    nrule = atoi(websGetVar(wp, "vlan_rule_num", "0"));
    max = atoi(nvram_safe_get("vlan_rule_max"));
    nrule = (nrule >= max) ? max : nrule;

    if (nrule != atoi(nvram_safe_get("vlan_rule_num"))) {
        config_preaction(&map, v, s, "NUM=0", "");
        snprintf(buf, sizeof(buf), "%d", nrule);
        nvram_set("vlan_rule_num", buf);
        nvram_set("br_rule_num", buf);
        change = 1;
        preact_needed = 0;
    }

    /* vlan_rule */
    _build_vlan_rule(wp, nrule, buf, sizeof(buf));
    orig_rule = nvram_safe_get("vlan_rule");
    if (orig_rule && *orig_rule && strcmp(orig_rule, buf)) {
        /* If the preaction is proceeded before, we don't do it again */
        if(preact_needed) {config_preaction(&map, v, s, "NUM=0", "");}
        nvram_set("vlan_rule", buf);
        change = 1;
        preact_needed = 0;
    }
    /* br_rule */
    _build_br_rule(wp, nrule, buf, sizeof(buf));
    orig_rule = nvram_safe_get("br_rule");
    if (orig_rule && *orig_rule && strcmp(orig_rule, buf)) {
        /* If the preaction is proceeded before, we don't do it again */
        if(preact_needed) {config_preaction(&map, v, s, "NUM=0", "");}
        nvram_set("br_rule", buf);
        change = 1;
        preact_needed = 0;
    }
    /* vlanport_rule */
    _build_vlanport_rule(wp, buf, sizeof(buf));
    orig_rule = nvram_safe_get("vlanport_rule");
    if (orig_rule && *orig_rule && strcmp(orig_rule, buf)) {
        /* If the preaction is proceeded before, we don't do it again */
        if(preact_needed) {config_preaction(&map, v, s, "NUM=0", "");}
        nvram_set("vlanport_rule", buf);
        change = 1;
        preact_needed = 0;
    }

    if (change) {
        /* Not defined the postaction yet. */
        config_postaction(map, s, "NUM=0", "");
    }

    return change;
}
