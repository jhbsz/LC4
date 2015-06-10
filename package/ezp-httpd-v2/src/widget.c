#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include "common.h"
#include "ezp.h"
#include "ezp-lib.h"
#include "lang.h"

int
ej_widget_execute(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_FV13XX)
    int i, num = atoi(nvram_safe_get("wan_num"));
    if(!atoi(nvram_safe_get("display_rule"))) {
        return 0;
    }
    for (i = 0; i < num; i++) {
        websWrite(wp, 
                "cpuShowLoading%d(rx[%d], tx[%d], rx_max[%d], tx_max[%d]);\n", 
                i, i, i, i, i);
    }
#endif
    return 0;
}

#ifdef EZP_PROD_BRAND_PROX
void
widget_show_frame_pre(webs_t wp)
{
    websWrite(wp, "<script type='text/javascript'>do_rightframe_pre()</script>\n");
}

void
widget_show_frame_post(webs_t wp)
{
    websWrite(wp, "<script type='text/javascript'>do_rightframe_post()</script>\n");
}
#else
void
widget_show_frame_pre(webs_t wp)
{
    websWrite(wp, "<td id='sidenav' class='right'>\n");
    websWrite(wp, "<div class='sidenav'>\n");
    websWrite(wp, "<div class='padding'>\n");
    websWrite(wp, "<div id='whitebox-padding'>\n");
}

void
widget_show_frame_post(webs_t wp)
{
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
    websWrite(wp, "</td>\n");
}
#endif


void
widget_show_pre(webs_t wp)
{
    websWrite(wp, "<div class='modulebox-white'>\n");
    websWrite(wp, "<div class='bx1'>\n");
    websWrite(wp, "<div class='bx2'>\n");
    websWrite(wp, "<div class='bx3'>\n");
    websWrite(wp, "<div class='bx4'>\n");
    websWrite(wp, "<div class='bx5'>\n");
    websWrite(wp, "<div class='padding'>\n");
}

void
widget_show_post(webs_t wp)
{
    websWrite(wp, "<span class='article_seperator'>&nbsp;</span>\n");
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
    websWrite(wp, "</div>\n");
}

#ifdef EZP_PROD_BRAND_PROX
void
widget_show_statusinfo(webs_t wp)
{
    do_ej("st_widget_status.asp", wp, "", "", "");
    return;
}
#else
void
widget_show_statusinfo(webs_t wp)
{
    char buf[TMP_LEN];

    websWrite(wp, "<div class='widgetInfo'>");
    websWrite(wp, "<div class='widget'>");
    websWrite(wp, "<div id='statusInfo'>");
    websWrite(wp, "<span class='cap-info'>");
    websWrite(wp, "<img src='picts/time01.gif' width=30 height=30>");
    websWrite(wp, "</span>");
    websWrite(wp, "<span class='data-info' style='font-size:10px'>");
    show_localtime(buf, sizeof(buf));
    websWrite(wp, buf);
    websWrite(wp, "</span><br/>");
    websWrite(wp, "<span class='cap-info'>");
    websWrite(wp, "<img src='picts/time02.gif' width=30 height=30>");
    websWrite(wp, "</span>");
    websWrite(wp, "<span class='data-info' style='font-size:10px'>");
    show_uptime(buf, sizeof(buf));
    websWrite(wp, buf);
    websWrite(wp, "</span>");
    websWrite(wp, "</div>");
    websWrite(wp, "</div>");
    websWrite(wp, "</div>");
    return;
}
#endif

int
ej_widget_show_signal(int eid, webs_t wp, int argc, char_t **argv)
{
    websWrite(wp, "<div style='font-size:9px' width=20>%s</div></td>\n",
            lang_translate("WWAN Signal Strength"));
    return 0;
}

int
widget_show_svg(webs_t wp)
{
    int num, height, i;

    num = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < num; i++) {
        websWrite(wp, "<input type='hidden' id='wan%d_status' /><br/>\n", i);
    }
    for (i = 0; i < num; i++) {
        websWrite(wp, "<table border=0 style='font-size:11px'>\n");
        websWrite(wp, "<tr>\n");
        websWrite(wp, "<td><img src='picts/w%d.gif' width=25 height=25></td>", i+1);
        websWrite(wp, "<td><div id='wan%d_ipaddr' style='font-size:9px' "
                            "width=20>&nbsp;</div></td>\n", i);
        websWrite(wp, "<td><div id='vpn%d_ipaddr' style='font-size:9px' "
                            "width=20>&nbsp;</div></td>\n", i);
        websWrite(wp, "<td><div id='wan%d_light'>"
                        "&nbsp;</div></td>\n", i, i);
        websWrite(wp, "<td><div class='action' id='wan%d_action' "
                        "onclick='act_for_connect(%d)'>&nbsp;</div></td>\n",
                        i, i);
        websWrite(wp, "</tr>\n");
        websWrite(wp, "</table>\n");
#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_FV13XX)
        height = 130 * num;
    #ifdef EZP_PROD_BRAND_AXIM
	websWrite(wp, "<div class='widget'>");
	websWrite(wp, "<table border=0 style='font-size:11px'>\n");
	websWrite(wp, "	<tr>\n");
	websWrite(wp, "		<td align='right' width='50' >%s:</td>\n", lang_translate("Download"));
        websWrite(wp, "     <td><div id='rx_%d' style='font-size:9px'></div></td>\n", i);
	websWrite(wp, "	</tr>\n");
        websWrite(wp, " <tr>\n");
        websWrite(wp, "     <td align='right' width='50'>%s:</td>\n", lang_translate("Upload"));
        websWrite(wp, "     <td><div id='tx_%d' style='font-size:9px'></div></td>\n", i);
        websWrite(wp, " </tr>\n");
	websWrite(wp, "</table>\n");
	websWrite(wp, "</div>");

    #else
        websWrite(wp, "<div class='widget'>");
        websWrite(wp, "<embed src='widget%d.svg' id='rx%d' "
                      "style='width:160px;height:100px;margin:0;padding:0;"
                            "z-index:-1' "
                      "type='image/svg+xml' "
                      "pluginspage='http://www.adobe.com/svg/viewer/install/'>",
                      i, i);
        websWrite(wp, "</embed>\n");
        websWrite(wp, "</div>");
    #endif

#endif
    }

#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_FV13XX)
    #ifndef EZP_PROD_BRAND_AXIM
        websWrite(wp, 
            "<div style='font-size:9px'>%s "
            "[<a href='http://www.adobe.com/svg/viewer/install/main.html'>%s</a>]"
            "</div>",
            lang_translate("SVG plugin is required to display the graphs"),
            lang_translate("Download"));
    #endif
#endif
    return 0;
}

#ifdef EZP_PROD_BRAND_PROX
void
widget_show_usb(webs_t wp, int is_login)
{
    char buff[SHORT_BUF_LEN];
    int wwanid;

    if (appmgr_check("wwan")) {
	    wwanid = atoi(nvram_safe_get("wan_num"))-1;
	    sprintf(buff,"%d",wwanid);

        if (is_login) 
            do_ej("st_widget_usb_login.asp",wp,buff,"");
        else
            do_ej("st_widget_usb.asp",wp,buff,"");
    } else if(appmgr_check("storage")) 
	    do_ej("st_widget_storage.asp",wp,"","");
     
    return;
}
#else
void
widget_show_usb(webs_t wp)
{
    char port[SHORT_BUF_LEN];
    websWrite(wp, "<table border=0 style='font-size:11px'>\n");
    websWrite(wp, "<tr>\n");
    if (appmgr_check("wwan")) {
            websWrite(wp, "<td><img src='picts/3G.gif' width=25 height=25></td>\n");
            websWrite(wp, "<td width='100px'>%s</td>\n", lang_translate("USB Device"));
    }
    else if(appmgr_check("storage")) {
            if (strcmp(ezp_prod_subsubcat, "L") != 0){
                websWrite(wp, "<td width='50px'><img src='picts/HDD.gif' width=25 height=25></td>\n");
            ezplib_get_attr_val("http_rule", 0, "port", port, sizeof(port), 
                    EZPLIB_USE_CLI);
            websWrite(wp, "<td width='100px'><a href=\"http://%s:%s/ad_storage.asp\">%s</a></td>\n", 
                    nvram_safe_get("lan0_ipaddr"), port, lang_translate("USB Device"));
            }
    }
    websWrite(wp, "<td><div id='usb_info1' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "<td><div id='wwan_signal' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "<td><div class='action' id='usb_img' "
                            "onclick='act_for_unload();'>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");
    websWrite(wp, "</table>\n");
}
#endif

void
widget_show_signal(webs_t wp)
{
    websWrite(wp, "<table border=0>\n");
    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td>%s</td>\n", lang_translate("Modem Brand"));
    websWrite(wp, "<td><div id='wwan_mf' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");

    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td>%s</td>\n", lang_translate("Modem Model"));
    websWrite(wp, "<td><div id='wwan_model' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");

    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td>%s</td>\n", lang_translate("IMEI"));
    websWrite(wp, "<td><div id='wwan_imei' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");

    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td>%s</td>\n", lang_translate("Service Provider"));
    websWrite(wp, "<td><div id='wwan_isp' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");

    websWrite(wp, "</table>\n");
}
void
widget_show_hdd(webs_t wp)
{
    websWrite(wp, "<table border=0 style='font-size:11px' width='200px'>\n");

    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td width='80px'>%s</td>\n",
                         lang_translate("Storage Size"));
    websWrite(wp, "<td><div id='storage_size' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");
    
    websWrite(wp, "<tr>\n");

    websWrite(wp, "<td>%s</td>\n", lang_translate("Used / Free"));
    websWrite(wp, "<td><div id='storage_info' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");

    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td>%s</td>\n", lang_translate("File System"));
    websWrite(wp, "<td><div id='storage_type' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");
    websWrite(wp, "</table>\n");

    websWrite(wp, "<table border=0 style='font-size:11px' width='200px'>\n");
    websWrite(wp, "<tr>\n");
    if (!(ezp_prod_subcat==2 && (!strcmp(ezp_prod_cat, "G")))) {
        websWrite(wp, "<td width='100px'>%s</td>\n", lang_translate("BT Engine"));
        websWrite(wp, "<td><div id='bt_on' style='font-size:10px' width=20>"
                "&nbsp;</div></td>\n");
        websWrite(wp, "<td><div class='action' id='bt_action' "
                "onclick='act_for_enable(\"bt\")'>"
                "&nbsp;</div></td>\n");
        websWrite(wp, "</tr>\n");
    }   

    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td>%s</td>\n", lang_translate("FTP Server"));
    websWrite(wp, "<td><div id='ftp_on' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "<td><div class='action' id='ftp_action' "
                            "onclick='act_for_enable(\"ftp\");'>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");

    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td>%s</td>\n", lang_translate("Samba Server"));
    websWrite(wp, "<td><div id='samba_on' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "<td><div class='action' id='samba_action' "
                            "onclick='act_for_enable(\"samba\");'>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");

    if (!(ezp_prod_subcat==2 && (!strcmp(ezp_prod_cat, "G")))) {
        websWrite(wp, "<tr>\n");
        websWrite(wp, "<td>%s</td>\n", lang_translate("iTunes Server"));
        websWrite(wp, "<td><div id='itunes_on' style='font-size:10px' width=20>"
                "&nbsp;</div></td>\n");
        websWrite(wp, "<td><div class='action' id='itunes_action' "
                "onclick='act_for_enable(\"itunes\");'>"
                "&nbsp;</div></td>\n");
        websWrite(wp, "</tr>\n");
    }
    if (ezp_prod_subcat >=3 ){
        websWrite(wp, "<tr>\n");
        websWrite(wp, "<td>%s</td>\n", lang_translate("iSCSI Server"));
        websWrite(wp, "<td><div id='iscsi_on' style='font-size:10px' width=20>"
                "&nbsp;</div></td>\n");
        websWrite(wp, "<td><div class='action' id='iscsi_action' "
                "onclick='act_for_enable(\"iscsi\");'>"
                "&nbsp;</div></td>\n");
        websWrite(wp, "</tr>\n");
    }
    websWrite(wp, "</table>\n");
    websWrite(wp, "<table border=0 style='font-size:11px' width='200px'>\n");
    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td width='50px'>%s</td>\n", lang_translate("Swap"));
    websWrite(wp, "<td><div id='swap_on' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");

    websWrite(wp, "<tr>\n");
    websWrite(wp, "<td>%s</td>\n", lang_translate("Device"));
    websWrite(wp, "<td><div id='dev_msg' style='font-size:10px' width=20>"
                            "&nbsp;</div></td>\n");
    websWrite(wp, "</tr>\n");
    websWrite(wp, "</table>\n");
}

int 
show_portcfg(char *buf, int len)
{
    char  *port_config_rule = "portcfg_rule", dispstr[TMP_LEN];
    int pcfgnum = atoi(nvram_get("portcfg_rule_num"));
    char lan_member_now[TMP_LEN], lan_member_rule[TMP_LEN];
    char wan_member_now[TMP_LEN], wan_member_rule[TMP_LEN];
    int idx;
    /* find out LAN and WAN vlan member now use */
    ezplib_find_attr_val("vlan_rule", "name", "LAN", "portmember",
            lan_member_now, sizeof(lan_member_now));
    ezplib_find_attr_val("vlan_rule", "name", "WAN", "portmember",
            wan_member_now, sizeof(wan_member_now));
    for (idx = 0; idx < pcfgnum; idx++) {
        /* get port member in portcfg_rule */
        ezplib_get_attr_val(port_config_rule, idx, "lan_member", 
                lan_member_rule, sizeof(lan_member_rule), EZPLIB_USE_CLI);
        ezplib_get_attr_val(port_config_rule, idx, "wan_member", 
                wan_member_rule, sizeof(wan_member_rule), EZPLIB_USE_CLI);

        /* Compare lan_member and wan_member of portcfg_rule with 
         * vlan_rule to identify what port config is selected now */
        if(!strcmp(lan_member_now, lan_member_rule) && 
                !strcmp(wan_member_now, wan_member_rule)){
            ezplib_get_attr_val(port_config_rule, idx, "dispstr", 
                    dispstr, sizeof(dispstr), EZPLIB_USE_CLI);
            snprintf(buf, len, "%s", lang_translate(dispstr));
            return 0;
        }
    }
    return 1;
}

void
widget_show_portinfo(webs_t wp)
{
    char buf[TMP_LEN];

    websWrite(wp, "<div class='widgetInfo'>");
    websWrite(wp, "<table>");
    websWrite(wp, "<tr>");
    websWrite(wp, "<td width=145>%s</td>", lang_translate("Ethernet Port"));
    show_portcfg(buf, sizeof(buf));
    websWrite(wp, "<td>%s</td>", buf);
    websWrite(wp, "</tr>");
    websWrite(wp, "</table>");
    websWrite(wp, "</div>");
}

#ifdef EZP_PROD_BRAND_PROX
int
ej_widget_show_login(int eid, webs_t wp, int argc, char_t **argv)
{
    int i, ewan, nlan;
    char buff[SHORT_BUF_LEN];
    char widget_asp_name[SHORT_BUF_LEN];

    /* If display rule is 0, do not show widget */
    if(!atoi(nvram_safe_get("display_rule"))) {
        return 0;
    }

    widget_show_frame_pre(wp);

    widget_show_statusinfo(wp);
  
    widget_show_usb(wp,1);

    ewan = atoi(nvram_safe_get("wan_num"));
    if (appmgr_check("wwan")) { ewan=ewan-1; }
        sprintf(widget_asp_name,"st_widget_wan_login.asp");
    for (i=0;i<ewan;i++) {
        sprintf(buff,"%d",i);
        do_ej(widget_asp_name, wp, buff, "", "");
    }
   
   
    websWrite(wp, "<div style=\"display:none;\">\n");

    sprintf(widget_asp_name,"st_widget_wifi_login.asp");
    do_ej(widget_asp_name, wp, "Wi-Fi","","");

    nlan = atoi(nvram_safe_get("lan_num"));
    for (i=0;i<nlan;i++) {
        sprintf(buff,"%d",i);
        do_ej("st_widget_lan.asp", wp, buff, "", "");
    }

    do_ej("st_widget_vpn.asp", wp, "", "", "");

    websWrite(wp, "</div>\n");

    widget_show_frame_post(wp);
    return 0;
}
#endif


int
ej_widget_show(int eid, webs_t wp, int argc, char_t **argv)
{
    int i;
#if !defined(EZP_PROD_BRAND_PROX)
    int nwan;
#endif    
#ifdef EZP_PROD_BRAND_PROX
    int  ewan, nlan;
    char buff[SHORT_BUF_LEN];
    char widget_asp_name[SHORT_BUF_LEN];
#endif    

    /* If display rule is 0, do not show widget */
    if(!atoi(nvram_safe_get("display_rule"))) {
        return 0;
    }

    widget_show_frame_pre(wp);
#if !defined(EZP_PROD_BRAND_PROX)
    widget_show_pre(wp);
#endif
    widget_show_statusinfo(wp);
    
#if !defined(EZP_PROD_BRAND_PROX)
    widget_show_svg(wp);
#endif

    
#if defined(EZP_PROD_BRAND_PROX)
    widget_show_usb(wp,0);
    ewan = atoi(nvram_safe_get("wan_num"));
    if (appmgr_check("wwan")) { ewan=ewan-1; }
    sprintf(widget_asp_name,"st_widget_wan.asp");
    for (i=0;i<ewan;i++) {
        sprintf(buff,"%d",i);
        do_ej(widget_asp_name, wp, buff, "", "");
    }
   
    sprintf(widget_asp_name,"st_widget_wifi.asp");
    do_ej(widget_asp_name, wp, "Wi-Fi","","");
 
    nlan = atoi(nvram_safe_get("lan_num"));
    for (i=0;i<nlan;i++) {
        sprintf(buff,"%d",i);
        do_ej("st_widget_lan.asp", wp, buff, "", "");
    }

    do_ej("st_widget_vpn.asp", wp, "", "", "");
#endif    

    if (appmgr_check("portcfg")) {
        widget_show_portinfo(wp);
    }

#if !defined(EZP_PROD_BRAND_PROX)
    widget_show_post(wp);

    if (appmgr_check("wwan")){
            nwan = atoi(nvram_safe_get("wan_num"));
            for (i = 0; i < nwan; i++) {
                char buf[SHORT_BUF_LEN];
                char *proto;
                snprintf(buf, sizeof(buf), "wan%d_proto", i);
                proto = nvram_safe_get(buf);
                if (!strcmp(proto, "wwan") || !strcmp(proto, "directip")) {
                    widget_show_pre(wp);
                    widget_show_usb(wp);
                    widget_show_signal(wp);
                    widget_show_post(wp);
                    /* Break it because we only need one widget for 3G modem. */
                    break;
                }
            }
    }
    else if (appmgr_check("storage")){
            if (strcmp(ezp_prod_subsubcat, "L") != 0){
                widget_show_pre(wp);
                widget_show_usb(wp);
                widget_show_hdd(wp);
                widget_show_post(wp);
            }
    }
#endif

    widget_show_frame_post(wp);
    return 0;
}

int
ej_widget_start(int eid, webs_t wp, int argc, char_t **argv)
{
    /* If display rule is 0, do not show widget */
    if(!atoi(nvram_safe_get("display_rule"))) {
        return 0;
    }
    websWrite(wp, "setTimeout('widget_query(F);', 3000);");
    return 0;
}
