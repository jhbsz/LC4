/* vi: set sw=4 ts=4 sts=4 fdm=marker: */
/*
 *	internet.c -- Internet Settings
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 */

#include	<stdlib.h>
#include	<sys/ioctl.h>
#include	<net/if.h>
#include	<net/route.h>
#include    <string.h>
#include    <dirent.h>
#include	"internet.h"
//#include	"nvram.h"
#include	"webs.h"
#include	"utils.h"
#include 	"firewall.h"
#include	"management.h"
//#include	"station.h"
#include	"wireless.h"

#include 	<bcmnvram.h>
#include	<ezp-lib.h>
//#include	"linux/autoconf.h"  //kernel config
//#include	"config/autoconf.h" //user config
#define SHORT_BUF_LEN   32

#define TMP_LEN 256 //aron patch for giga, from httpd common.h

#if 1 //Steve27
#ifdef CONFIG_RALINKAPP_SWQOS
#include      "qos.h"
#endif

// Tommy, 2009/3/13 09:57
//extern void check_scheduler(void);

// Tommy, Add syslog, 2009/10/21 04:47
#include <syslog.h>

static int getDhcpCliList(int eid, webs_t wp, int argc, char_t **argv);
static int getDns(int eid, webs_t wp, int argc, char_t **argv);
static int getIpv6Dns(int eid, webs_t wp, int argc, char_t **argv);
static int getHostSupp(int eid, webs_t wp, int argc, char_t **argv);
static int getIfLiveWeb(int eid, webs_t wp, int argc, char_t **argv);
static int getIfIsUpWeb(int eid, webs_t wp, int argc, char_t **argv);
static int getStorageBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getFtpBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getSmbBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getMediaBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getIgmpProxyBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getLanIp(int eid, webs_t wp, int argc, char_t **argv);
static int getLanMac(int eid, webs_t wp, int argc, char_t **argv);
#if 1//Arthur Chow 2009-01-03
static int getWlanMac(int eid, webs_t wp, int argc, char_t **argv);
#endif
static int getLanIfNameWeb(int eid, webs_t wp, int argc, char_t **argv);
static int getLanNetmask(int eid, webs_t wp, int argc, char_t **argv);
static int getDnsmasqBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getLltdBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getPppoeRelayBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getUpnpBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getRadvdBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getWanIp(int eid, webs_t wp, int argc, char_t **argv);
static int getPPPoEMyWanIp(int eid, webs_t wp, int argc, char_t **argv); //aron add
static int getPPTPMyWanIp(int eid, webs_t wp, int argc, char_t **argv); //aron add
static int getL2TPMyWanIp(int eid, webs_t wp, int argc, char_t **argv); //aron add
static int getPhyWanIfIp(int eid, webs_t wp, int argc, char_t **argv); //aron add
static int getPhyWanIfNetMask(int eid, webs_t wp, int argc, char_t **argv); //aron add
static int getPhyWanIfGateway(int eid, webs_t wp, int argc, char_t **argv); //aron add
static int getLanIfGateway(int eid, webs_t wp, int argc, char_t **argv); //aron patch for giga
static int getWanMac(int eid, webs_t wp, int argc, char_t **argv);
static int getWanIfNameWeb(int eid, webs_t wp, int argc, char_t **argv);
static int getWanNetmask(int eid, webs_t wp, int argc, char_t **argv);
static int getWanGateway(int eid, webs_t wp, int argc, char_t **argv);
static int startRoutercheck(int eid, webs_t wp, int argc, char_t **argv);//Arthur Chow add
static int getRoutingTable(int eid, webs_t wp, int argc, char_t **argv);
static int getStaticRoutingNumbers(int eid, webs_t wp, int argc, char_t **argv);  //aron add
static void reflashClientTbl(webs_t wp, char_t *path, char_t *query);  //aron add
static void setLan(webs_t wp, char_t *path, char_t *query);
static void setLanIP(webs_t wp, char_t *path, char_t *query);  //aron add
static int doLanIPfunc(webs_t wp, char_t *path, char_t *query);  //aron add
static int doLanDhcpfunc(webs_t wp, char_t *path, char_t *query);  //aron add
static int doIgmpSnoopfunc(webs_t wp, char_t *path, char_t *query);  //aron add
static int doLanLeaseDnsfunc(webs_t wp, char_t *path, char_t *query);  //aron add
static void setLanIPAlias(webs_t wp, char_t *path, char_t *query);  //aron add
static void setLan_StaicLease_Dns(webs_t wp, char_t *path, char_t *query);  //aron add
static void setLanUPnP(webs_t wp, char_t *path, char_t *query);  //aron add
static void setIgmpSnooping(webs_t wp, char_t *path, char_t *query);  //aron add
static void setLanIGMP(webs_t wp, char_t *path, char_t *query);  //aron add
static void setVpnPaThru(webs_t wp, char_t *path, char_t *query);
static void setWan(webs_t wp, char_t *path, char_t *query);
static void setWanNormal(webs_t wp, char_t *path, char_t *query);
static void setLanDhcp(webs_t wp, char_t *path, char_t *query);  //aron add
static void getMyMAC(webs_t wp, char_t *path, char_t *query);
static void addRouting(webs_t wp, char_t *path, char_t *query);
static void delRouting(webs_t wp, char_t *path, char_t *query);
static void dynamicRouting(webs_t wp, char_t *path, char_t *query);
static void ripRouting(webs_t wp, char_t *path, char_t *query);  //aron add
static int getDynamicRoutingBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getSWQoSBuilt(int eid, webs_t wp, int argc, char_t **argv);
static void IPV6(webs_t wp, char_t *path, char_t *query);  //nico add
static int getLinkLocalAddr(int eid, webs_t wp, int argc, char_t **argv);
static void setWan_Simple(webs_t wp, char_t *path, char_t *query);//Steve add
inline void zebraRestart(void);
void ripdRestart(void);

//RL added for assure quota 2009/03/17
#ifdef CONFIG_RALINK_RT2880
extern void goaNotifyGpioForConntrack(void);
#endif

#if 1//Arthur Chow 2009-03-02: For getWebMessage
extern int setWebMessage(int flag, char *str);
#endif

//aron add
int lanIPchanged=0;

//extern char* prefix2mask(int idx);

//aron patch for giga
char* mask2prefix(char *mask)
{
    if (!strcmp(mask, "0.0.0.0"))
          mask="0";
    else if (!strcmp(mask, "128.0.0.0"))
          mask="1";
    else if (!strcmp(mask, "192.0.0.0"))
          mask="2";
    else if (!strcmp(mask, "224.0.0.0"))
          mask="3";
    else if (!strcmp(mask, "240.0.0.0"))
          mask="4";
    else if (!strcmp(mask, "248.0.0.0"))
          mask="5";
    else if (!strcmp(mask, "252.0.0.0"))
          mask="6";
    else if (!strcmp(mask, "254.0.0.0"))
          mask="7";
    else if (!strcmp(mask, "255.0.0.0"))
          mask="8";
    else if (!strcmp(mask, "255.128.0.0"))
          mask="9";
    else if (!strcmp(mask, "255.192.0.0"))
          mask="10";
    else if (!strcmp(mask, "255.224.0.0"))
          mask="11";
    else if (!strcmp(mask, "255.240.0.0"))
          mask="12";
    else if (!strcmp(mask, "255.248.0.0"))
          mask="13";
    else if (!strcmp(mask, "255.252.0.0"))
          mask="14";
    else if (!strcmp(mask, "255.254.0.0"))
          mask="15";
    else if (!strcmp(mask, "255.255.0.0"))
          mask="16";
    else if (!strcmp(mask, "255.255.128.0"))
          mask="17";
    else if (!strcmp(mask, "255.255.192.0"))
          mask="18";
    else if (!strcmp(mask, "255.255.224.0"))
          mask="19";
    else if (!strcmp(mask, "255.255.240.0"))
          mask="20";
    else if (!strcmp(mask, "255.255.248.0"))
          mask="21";
    else if (!strcmp(mask, "255.255.252.0"))
          mask="22";
    else if (!strcmp(mask, "255.255.254.0"))
          mask="23";
    else if (!strcmp(mask, "255.255.255.0"))
          mask="24";
    else if (!strcmp(mask, "255.255.255.128"))
          mask="25";
    else if (!strcmp(mask, "255.255.255.192"))
          mask="26";
    else if (!strcmp(mask, "255.255.255.224"))
          mask="27";
    else if (!strcmp(mask, "255.255.255.240"))
          mask="28";
    else if (!strcmp(mask, "255.255.255.248"))
          mask="29";
    else if (!strcmp(mask, "255.255.255.252"))
          mask="30";
    else if (!strcmp(mask, "255.255.255.254"))
          mask="31";
    else if (!strcmp(mask, "255.255.255.255"))
          mask="32";
    else
          mask="";
          
    return mask;
}

//aron patch for giga
static int
_save_wan_dns(webs_t wp, char *value)
{
    char tmp[TMP_LEN];
    char *dns1Type, *dns2Type, *staticPriDns, *staticSecDns;
    int idx = atoi(value), len, change = 0;

  if(idx==0) //Normally, the nth for "0"
  {
    /* dns1 Type */
    snprintf(tmp, sizeof(tmp), "dns1Type");
    dns1Type = websGetVar(wp, tmp, "");

    /* dns2 Type */
    snprintf(tmp, sizeof(tmp), "dns2Type");
    dns2Type = websGetVar(wp, tmp, "");

    /* Primary Dns */
    snprintf(tmp, sizeof(tmp), "staticPriDns");
    staticPriDns = websGetVar(wp, tmp, "");

    /* Secondary DNS */
    snprintf(tmp, sizeof(tmp), "staticSecDns");
    staticSecDns = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* dns1 Type */
    snprintf(tmp, sizeof(tmp), "dns1Type_%d", idx);
    dns1Type = websGetVar(wp, tmp, "");

    /* dns2 Type */
    snprintf(tmp, sizeof(tmp), "dns2Type_%d", idx);
    dns2Type = websGetVar(wp, tmp, "");

    /* Primary Dns */
    snprintf(tmp, sizeof(tmp), "staticPriDns_%d", idx);
    staticPriDns = websGetVar(wp, tmp, "");

    /* Secondary DNS */
    snprintf(tmp, sizeof(tmp), "staticSecDns_%d", idx);
    staticSecDns = websGetVar(wp, tmp, "");
  }
  
    if(!strcmp(dns1Type, "ISP"))
        dns1Type = "ispdns";
    else if(!strcmp(dns1Type, "USER"))
        dns1Type = "custom";
    else if(!strcmp(dns1Type, "NONE"))
        dns1Type = "none";
    
    if(!strcmp(dns2Type, "ISP"))
        dns2Type = "ispdns";
    else if(!strcmp(dns2Type, "USER"))
        dns2Type = "custom";
    else if(!strcmp(dns2Type, "NONE"))
        dns2Type = "none";
        
        
    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s", dns1Type, staticPriDns, 
                dns2Type, staticSecDns);

    if (len > TMP_LEN - 1) {
        return 0;
    }

    //preaction ??
    ezplib_replace_rule("wan_dns_rule", idx, tmp);
    change = 1;

    return change;
}

/*
 * arguments: ifname  - interface name
 * description: test the existence of interface through /proc/net/dev
 * return: -1 = fopen error, 1 = not found, 0 = found
 */
int getIfLive(char *ifname)
{
#if 0
	FILE *fp;
	char buf[256], *p;
	int i;

	if (NULL == (fp = fopen("/proc/net/dev", "r"))) {
		error(E_L, E_LOG, T("getIfLive: open /proc/net/dev error"));
		return -1;
	}

	fgets(buf, 256, fp);
	fgets(buf, 256, fp);
	while (NULL != fgets(buf, 256, fp)) {
		i = 0;
		while (isspace(buf[i++]))
			;
		p = buf + i - 1;
		while (':' != buf[i++])
			;
		buf[i-1] = '\0';
		if (!strcmp(p, ifname)) {
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	error(E_L, E_LOG, T("getIfLive: device %s not found"), ifname);
	return 1;
#endif
}

/*
 * arguments: ifname  - interface name
 *            if_addr - a 18-byte buffer to store mac address
 * description: fetch mac address according to given interface name
 */
int getIfMac(char *ifname, char *if_hw)
{
#if 0
	struct ifreq ifr;
	char *ptr;
	int skfd;

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error(E_L, E_LOG, T("getIfMac: open socket error"));
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
	if(ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
		close(skfd);
		//error(E_L, E_LOG, T("getIfMac: ioctl SIOCGIFHWADDR error for %s"), ifname);
		return -1;
	}

	ptr = (char *)&ifr.ifr_addr.sa_data;
	sprintf(if_hw, "%02X:%02X:%02X:%02X:%02X:%02X",
			(ptr[0] & 0377), (ptr[1] & 0377), (ptr[2] & 0377),
			(ptr[3] & 0377), (ptr[4] & 0377), (ptr[5] & 0377));

	close(skfd);
	return 0;
#endif
}

/*
 * arguments: ifname  - interface name
 *            if_addr - a 16-byte buffer to store ip address
 * description: fetch ip address, netmask associated to given interface name
 */
int getIfIp(char *ifname, char *if_addr)
{
#if 0
	struct ifreq ifr;
	int skfd = 0;

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error(E_L, E_LOG, T("getIfIp: open socket error"));
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
	if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) {
		close(skfd);
		//error(E_L, E_LOG, T("getIfIp: ioctl SIOCGIFADDR error for %s"), ifname);
		return -1;
	}
	strcpy(if_addr, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	close(skfd);
	return 0;
#endif
}

/*
 * arguments: ifname - interface name
 * description: return 1 if interface is up
 *              return 0 if interface is down
 */
int getIfIsUp(char *ifname)
{
#if 0
	struct ifreq ifr;
	int skfd;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd == -1) {
		perror("socket");
		return -1;
	}
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		perror("ioctl");
		close(skfd);
		return -1;
	}
	close(skfd);
	if (ifr.ifr_flags & IFF_UP)
		return 1;
	else
		return 0;
#endif
}

/*
 * arguments: ifname - interface name
 *            if_net - a 16-byte buffer to store subnet mask
 * description: fetch subnet mask associated to given interface name
 *              0 = bridge, 1 = gateway, 2 = wirelss isp
 */
int getIfNetmask(char *ifname, char *if_net)
{
#if 0
	struct ifreq ifr;
	int skfd = 0;

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error(E_L, E_LOG, T("getIfNetmask: open socket error"));
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
	if (ioctl(skfd, SIOCGIFNETMASK, &ifr) < 0) {
		close(skfd);
		//error(E_L, E_LOG, T("getIfNetmask: ioctl SIOCGIFNETMASK error for %s\n"), ifname);
		return -1;
	}
	strcpy(if_net, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	close(skfd);
	return 0;
#endif
}

/*
 * description: return WAN interface name
 *              0 = bridge, 1 = gateway, 2 = wirelss isp
 */
char* getWanIfName(void)
{
        // Test: 0:Bridge 1:Router 2:WISP 3:Universal Repeater
	char IfTemp[10]="1";
	
	//char *mode = nvram_bufget(RT2860_NVRAM, "OperationMode");
// Tommy, Need to modify, 2010/5/5 11:45
	char *mode = IfTemp;
	
	static char *if_name = "br0";

	if (NULL == mode)
		return if_name;
	if (!strncmp(mode, "0", 2))
		if_name = "br0";
	else if (!strncmp(mode, "1", 2)) {
		if_name = "eth2";
	}
	else if (!strncmp(mode, "2", 2))
		if_name = "ra0";
	else if (!strncmp(mode, "3", 2))
		if_name = "apcli0";
	return if_name;

}

char* getWanIfNamePPP(void)
{
#if 0
    char *cm;
    cm = nvram_bufget(RT2860_NVRAM, "wanConnectionMode");
    if (!strncmp(cm, "PPPOE", 6) || !strncmp(cm, "L2TP", 5) || !strncmp(cm, "PPTP", 5))
        return "ppp0";

    return getWanIfName();
#endif
}


/*
 * description: return LAN interface name
 */
char* getLanIfName(void)
{
        
        // Test: 0:Bridge 1:Router 2:WISP 3:Universal Repeater
	char IfTemp[10]="1";
	
// Tommy, Need to modify, 2010/5/5 11:45
	char *mode = IfTemp;	
	
	//char *mode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	static char *if_name = "br0";

	if (NULL == mode)
		return if_name;
	if (!strncmp(mode, "0", 2))
		if_name = "br0";
	else if (!strncmp(mode, "1", 2)) {
		if_name = "br0";
	}
	else if (!strncmp(mode, "2", 2)) {
		if_name = "eth2";
	}
	else if (!strncmp(mode, "3", 2)) {
		if_name = "br0";
	}
	return if_name;
}

/*
 * description: get the value "WAN" or "LAN" the interface is belong to.
 */
char *getLanWanNamebyIf(char *ifname)
{
#if 0
	char *mode = nvram_bufget(RT2860_NVRAM, "OperationMode");

	if (NULL == mode)
		return "Unknown";

	if (!strcmp(mode, "0")){	// bridge mode
		if(!strcmp(ifname, "br0"))
			return "LAN";
		return ifname;
	}

	if (!strcmp(mode, "1")) {	// gateway mode
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW
		if(!strcmp(ifname, "br0"))
			return "LAN";
		if(!strcmp(ifname, "eth2.2") || !strcmp(ifname, "ppp0"))
			return "WAN";
		return ifname;
#elif defined  CONFIG_ICPLUS_PHY && CONFIG_RT2860V2_AP_MBSS
		char *num_s = nvram_bufget(RT2860_NVRAM, "BssidNum");
		if(atoi(num_s) > 1 && !strcmp(ifname, "br0") )	// multiple ssid
			return "LAN";
		if(atoi(num_s) == 1 && !strcmp(ifname, "ra0"))
			return "LAN";
		if (!strcmp(ifname, "eth2") || !strcmp(ifname, "ppp0"))
			return "WAN";
		return ifname;
#else
		if(!strcmp(ifname, "ra0"))
			return "LAN";
		return ifname;
#endif
	}else if (!strncmp(mode, "2", 2)) {	// ethernet convertor
		if(!strcmp("eth2", ifname))
			return "LAN";
		if(!strcmp("ra0", ifname))
			return "WAN";
		return ifname;
	}else if (!strncmp(mode, "3", 2)) {	// apcli mode
		if(!strcmp("br0", ifname))
			return "LAN";
		if(!strcmp("apcli0", ifname))
			return "WAN";
		return ifname;
	}
	return ifname;
#endif
}

/*
 *  Aron add: refresh DHCP client table
 */
void reflashClientTbl(webs_t wp, char_t *path, char_t *query)
{
	char_t *submitUrl;
        submitUrl = websGetVar(wp, T("lan_dhcp_clientInfo_url"), T(""));   // aron add for hidden page
	websRedirect(wp, submitUrl);
}

/*
 * description: write DHCP client list
 */
static int getDhcpCliList(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	int clientCount=1; //aron add
	unsigned d, h, m;
	unsigned long expires;
        char ip[16], mac[32], mac2[64], name[255], expires_time[64];
        time_t now;

    if ((fp = fopen("/var/dhcp.leases", "r"))) {
    	
    	/* Get current time. */
        now = time(NULL);
        
	while (fscanf(fp, "%lu %s %s %s %s", &expires, mac, ip, name, mac2) == 5) {
		
		expires -= (unsigned long) now;
		websWrite(wp,T("<tr><td><center><span class='table_left'>"));
		websWrite(wp, T("%d"), clientCount);
		websWrite(wp,T("</span></center></td>"));

		websWrite(wp,T("<td><center><span class='table_font'>"));
		websWrite(wp, T("%s"), mac);
		websWrite(wp,T("</span></center></td>"));
		websWrite(wp,T("<td><center><span class='table_font'>"));
		websWrite(wp, T("%s"), ip);
		websWrite(wp,T("</span></center></td>"));
		websWrite(wp,T("<td><center><span class='table_right'>"));
		d = expires / (24*60*60); expires %= (24*60*60);
		h = expires / (60*60); expires %= (60*60);
		m = expires / 60; expires %= 60;
		if (d) websWrite(wp, T("%u days "), d);
		websWrite(wp, T("%02u:%02u:%02u\n"), h, m, (unsigned)expires);
		websWrite(wp,T("</span></center></td>")); //aron add
		websWrite(wp, T("</tr>")); //aron add
		clientCount++;
	}
	fclose(fp);
     }
	return 0;
}

/*
 * arguments: type - 1 = write Primary DNS
 *                   2 = write Secondary DNS
 * description: write DNS ip address accordingly
 */
static int getDns(int eid, webs_t wp, int argc, char_t **argv)
{
        int type, req=0;
        char *value;
        char buf[TMP_LEN];
        
        if (ejArgs(argc, argv, T("%d"), &type) == 1) {
		if (1 == type)
			req = 1;
		else if (2 == type)
			req = 2;
		else
			return websWrite(wp, T(""));
	}
	
	if(req == 1)
        {
            //value = nvram_safe_get("wan0_dns");
            ezplib_get_attr_val("wan_dns_rule", 0, "dns1", buf, TMP_LEN, EZPLIB_USE_CLI);
        }
        else if(req ==2)
        {
            //value = nvram_safe_get("wan0_dns2");
            ezplib_get_attr_val("wan_dns_rule", 0, "dns2", buf, TMP_LEN, EZPLIB_USE_CLI);  
        }
        return websWrite(wp, T("%s"), buf);
}

/*
 * arguments: type - 1 = write Primary DNS
 *                   2 = write Secondary DNS
 * description: write DNS ip address accordingly
 */
static int getIpv6Dns(int eid, webs_t wp, int argc, char_t **argv)
{
        int type, req=0;
        char *value;
        char buf[TMP_LEN];
        
        if (ejArgs(argc, argv, T("%d"), &type) == 1) {
		if (1 == type)
			req = 1;
		else if (2 == type)
			req = 2;
		else
			return websWrite(wp, T(""));
	}
	
	if(req == 1)
        {
            //value = nvram_safe_get("wan0_dns");
            ezplib_get_attr_val("wan_dnsv6_rule", 0, "dns1", buf, TMP_LEN, EZPLIB_USE_CLI);
        }
        else if(req ==2)
        {
            //value = nvram_safe_get("wan0_dns2");
            ezplib_get_attr_val("wan_dnsv6_rule", 0, "dns2", buf, TMP_LEN, EZPLIB_USE_CLI);  
        }
        return websWrite(wp, T("%s"), buf);
}

/*
 * arguments:
 * description: return 1 if hostname is supported
 */
static int getHostSupp(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#ifdef GA_HOSTNAME_SUPPORT
	ejSetResult(eid, "1");
#else
	ejSetResult(eid, "0");
#endif
	return 0;
#endif
}

/*
 * arguments: name - interface name (ex. eth0, rax ..etc)
 * description: write the existence of given interface,
 *              0 = ifc dosen't exist, 1 = ifc exists
 */
static int getIfLiveWeb(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	char_t *name;
	char exist[2] = "0";

	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	exist[0] = (getIfLive(name) == 0)? '1' : '0';
	return websWrite(wp, T("%s"), exist);
#endif
}

/*
 * arguments: name - interface name (ex. eth0, rax ..etc)
 * description: write the existence of given interface,
 *              0 = ifc is down, 1 = ifc is up
 */
static int getIfIsUpWeb(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	char_t *name;
	char up[2] = "1";

	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	up[0] = (getIfIsUp(name) == 1)? '1' : '0';
	return websWrite(wp, T("%s"), up);
#endif
}

static int getIgmpProxyBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#ifdef CONFIG_USER_IGMP_PROXY
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getStorageBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#if defined CONFIG_USER_STORAGE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getFtpBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#if defined CONFIG_USER_STUPID_FTPD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getSmbBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#if defined CONFIG_USER_SAMBA
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getMediaBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#if defined CONFIG_USER_USHARE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getDynamicRoutingBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#if defined CONFIG_USER_ZEBRA
    return websWrite(wp, T("1"));
#else
    return websWrite(wp, T("0"));
#endif
#endif
}

static int getSWQoSBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#if defined CONFIG_RALINKAPP_SWQOS
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

/*
 * description: write LAN ip address accordingly
 */
static int getLanIp(int eid, webs_t wp, int argc, char_t **argv)
{
	char *value;
	value = nvram_safe_get("lan0_ipaddr");
        return websWrite(wp, T("%s"), value);
}

/*
 * description: write LAN MAC address accordingly
 */
static int getLanMac(int eid, webs_t wp, int argc, char_t **argv)
{
    char ifname[16];
    char device[16];
    char hwaddr[64];
    char *if_ptr, *type;
    int ret = -1;

#if 0
    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
#endif

    if_ptr = nvram_safe_get("lan0_ifname");

    /* PPPoE interface doesn't have MAC address. */
    if (!strncmp(if_ptr, "ppp", 3)) {
        //if (snprintf(device, 16, "%s_device", type) >= 16)
        if (snprintf(device, 16, "lan0_device") >= 16)
            return -1;
        if_ptr = nvram_safe_get(device);
    }

    if (!show_hwaddr(if_ptr, hwaddr, 64)) {
        ret = websWrite(wp, "%s", hwaddr);
        return 0;
    } else {
        ret = websWrite(wp, "Not Connected");
        return ret;
	}
}


/*
 * description: write LAN MAC address accordingly
 */
static int getWlanMac(int eid, webs_t wp, int argc, char_t **argv)
{
    int if_idx, vif_idx;
    char tmp1[SHORT_BUF_LEN], tmp2[SHORT_BUF_LEN], hwaddr[SHORT_BUF_LEN];

    ezplib_get_attr_val("wl0_ifname_rule", 0, "ifname", tmp2, SHORT_BUF_LEN,
                EZPLIB_USE_CLI);

    if (!show_hwaddr(tmp2, hwaddr, SHORT_BUF_LEN)) {
        websWrite(wp, "%s", hwaddr);
        return 0;
    } else {
        return websWrite(wp, T("Not enabled"));
	}

}


/*
 * arguments: type - 0 = return LAN interface name (default)
 *                   1 = write LAN interface name
 * description: return or write LAN interface name accordingly
 */
static int getLanIfNameWeb(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	int type;
	char *name = getLanIfName();

	if (ejArgs(argc, argv, T("%d"), &type) == 1) {
		if (1 == type) {
			return websWrite(wp, T("%s"), name);
		}
	}
	ejSetResult(eid, name);
	return 0;
#endif
}

/*
 * description: write LAN subnet mask accordingly
 */
static int getLanNetmask(int eid, webs_t wp, int argc, char_t **argv)
{
	char *value;
	int idx;
	
	value = nvram_safe_get("lan0_mask");
	idx = atoi(value);
	value = prefix2mask(idx);
        return websWrite(wp, T("%s"), value);
}

static int getDnsmasqBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#ifdef CONFIG_USER_DNSMASQ
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getLltdBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#if defined CONFIG_LLDT && defined CONFIG_RT2860V2_AP_LLTD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getPppoeRelayBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#ifdef CONFIG_USER_RPPPPOE_RELAY
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getUpnpBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#ifdef CONFIG_USER_UPNP_IGD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

static int getRadvdBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
#ifdef CONFIG_USER_RADVD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
#endif
}

/* 
 *  aron add for get pppoe my wan ip 
    in PPPoE/PPTP/L2TP mode when get from DHCP 
 */
static int getPPPoEMyWanIp(int eid, webs_t wp, int argc, char_t **argv)
{
        char *value;
        value = nvram_safe_get("wan0_ipaddr");
        return websWrite(wp, T("%s"), value);
}

/* 
 *  aron add for get pptp my wan ip 
    in PPPoE/PPTP/L2TP mode when get from DHCP 
 */
static int getPPTPMyWanIp(int eid, webs_t wp, int argc, char_t **argv)
{
        char *value;
        char buf[TMP_LEN];
        //value = nvram_safe_get("wan0_ipaddr");
        ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
        return websWrite(wp, T("%s"), buf);
}

/* 
 *  aron add for get l2tp my wan ip 
    in PPPoE/PPTP/L2TP mode when get from DHCP 
 */
static int getL2TPMyWanIp(int eid, webs_t wp, int argc, char_t **argv)
{
        char *value;
        char buf[TMP_LEN];
        //value = nvram_safe_get("wan0_ipaddr");
        ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
        return websWrite(wp, T("%s"), buf);
}

/* 
 *  aron add for get physical wan interface ip 
    in PPTP/L2TP mode when get from DHCP 
 */
static int getPhyWanIfIp(int eid, webs_t wp, int argc, char_t **argv)
{
        char *value;
        char buf[TMP_LEN];
        
        value = nvram_get("wan0_proto");
	if (!strcmp(value, "pptp") || !strcmp(value, "l2tp"))
	{
	    ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
            return websWrite(wp, T("%s"), buf);
        }
	else
	{
	    value = nvram_safe_get("wan0_ipaddr");
            return websWrite(wp, T("%s"), value);
        }
        /*
        value = nvram_safe_get("wan0_ipaddr");
        return websWrite(wp, T("%s"), value);
        */
}

/* 
 *  aron add for get physical wan interface netmask 
    in PPTP/L2TP mode when get from DHCP 
 */
static int getPhyWanIfNetMask(int eid, webs_t wp, int argc, char_t **argv)
{
        char *value;
        char buf[TMP_LEN];
        int idx;
         
	value = nvram_get("wan0_proto");
	if (!strcmp(value, "pptp") || !strcmp(value, "l2tp"))
	{
	    ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "netmask", buf, TMP_LEN, EZPLIB_USE_CLI);
	    if (buf ==NULL)
                return websWrite(wp, T(""));
            else
            {
	        if(!strcmp(buf, ""))
	            return websWrite(wp, T(""));
	        else
	        {
	            idx = atoi(buf);
	            value = prefix2mask(idx);
                    return websWrite(wp, T("%s"), value);
                }
            }
        }
	else
	{
	    value = nvram_safe_get("wan0_mask");
            if (value ==NULL)
                return websWrite(wp, T(""));
            else
            {
                if(!strcmp(value, ""))
	            return websWrite(wp, T(""));
	        else
	        {
	            idx = atoi(value);
	            value = prefix2mask(idx);
                    return websWrite(wp, T("%s"), value);
                }
            }
        }
        /*
        value = nvram_safe_get("wan0_mask");
        if (value ==NULL)
            return websWrite(wp, T(""));
        else
        {
            if(!strcmp(value, ""))
	        return websWrite(wp, T(""));
	    else
	    {
	        idx = atoi(value);
	        value = prefix2mask(idx);
                return websWrite(wp, T("%s"), value);
            }
        }
        */
}

/* 
 *  aron add for get physical wan interface gateway 
    in PPTP/L2TP mode when get from DHCP 
 */
static int getPhyWanIfGateway(int eid, webs_t wp, int argc, char_t **argv)
{
        char *value;
        char buf[TMP_LEN];
        
        value = nvram_get("wan0_proto");
	if (!strcmp(value, "pptp") || !strcmp(value, "l2tp"))
	{
	    ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI);
            return websWrite(wp, T("%s"), buf);
        }
	else
	{
	    value = nvram_safe_get("wan0_gateway");
            return websWrite(wp, T("%s"), value);
        }
        /*
        value = nvram_safe_get("wan0_gateway");
        return websWrite(wp, T("%s"), value);
        */
}

//aron patch for giga
static int getLanIfGateway(int eid, webs_t wp, int argc, char_t **argv)
{
	char *value;
	value = nvram_safe_get("lan0_gateway");
        return websWrite(wp, T("%s"), value);
}

/*
 * description: write WAN ip address accordingly
 */
static int getWanIp(int eid, webs_t wp, int argc, char_t **argv)
{
	char *value;
        
        value = nvram_safe_get("wan0_ipaddr");
        if (value ==NULL)
            return websWrite(wp, T("0.0.0.0"));
        else
        {
            if(!strcmp(value, ""))
	        return websWrite(wp, T("0.0.0.0"));
	    else
	        return websWrite(wp, "%s", value);
	}
}


// Tommy, add form AXIMcom
int show_hwaddr(char *ifname, char *ptr, int length)
	{
    int fd = -1, n;
    unsigned char *tmp;
    struct ifreq ifr;

    if ((fd = socket (AF_INET, SOCK_DGRAM, IPPROTO_IP)) <= -1)
        goto err;

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy (ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0)
        goto err;

    tmp = (unsigned char *) &ifr.ifr_hwaddr.sa_data[0];

    n = snprintf(ptr, length, "%02X:%02X:%02X:%02X:%02X:%02X", 
            tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);

    if (n <= 0)
        goto err;

    close(fd);
    return 0;

err:
    if (fd > -1)
        close(fd);
    return 1;
	   }


/*
 * description: write WAN MAC address accordingly
 */
static int getWanMac(int eid, webs_t wp, int argc, char_t **argv)
{
    char ifname[16];
    char device[16];
    char hwaddr[64];
    char *if_ptr, *type;
    int ret = -1;

#if 0
    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
#endif
    
    if_ptr = nvram_safe_get("wan0_ifname");

    /* PPPoE interface doesn't have MAC address. */
    if (!strncmp(if_ptr, "ppp", 3)) {
        //if (snprintf(device, 16, "%s_device", type) >= 16)
        if (snprintf(device, 16, "wan0_device") >= 16)
            return -1;
        if_ptr = nvram_safe_get(device);
    }

    if (!show_hwaddr(if_ptr, hwaddr, 64)) {
        ret = websWrite(wp, "%s", hwaddr);
        return 0;
    } else {
        ret = websWrite(wp, "Not Connected");
        return ret;
	}
}

/*
 * arguments: type - 0 = return WAN interface name (default)
 *                   1 = write WAN interface name
 * description: return or write WAN interface name accordingly
 */
static int getWanIfNameWeb(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	int type;
	char *name = getWanIfName();

	if (ejArgs(argc, argv, T("%d"), &type) == 1) {
		if (1 == type) {
			return websWrite(wp, T("%s"), name);
		}
	}
	ejSetResult(eid, name);
	return 0;
#endif
}

/*
 * description: write WAN subnet mask accordingly
 */
static int getWanNetmask(int eid, webs_t wp, int argc, char_t **argv)
{
        char *value;
        int idx;
         
	value = nvram_safe_get("wan0_mask");
	if (value ==NULL)
            return websWrite(wp, T("0.0.0.0"));
        else
        {
	    if(!strcmp(value, ""))
	        return websWrite(wp, T("0.0.0.0"));
	    else
	    {
	        idx = atoi(value);
	        value = prefix2mask(idx);
                return websWrite(wp, T("%s"), value);
	    }
        }
}

/*
 * description: write WAN default gateway accordingly
 */
static int getWanGateway(int eid, webs_t wp, int argc, char_t **argv)
{
        char *value;
        
        value = nvram_safe_get("wan0_gateway");
        
        printf("\nwan0_gateway=%s\n",value);
        if (value ==NULL)
            return websWrite(wp, T("0.0.0.0"));
        else
        {
            if(!strcmp(value, ""))
	        return websWrite(wp, T("0.0.0.0"));
	    else
	        return websWrite(wp, "%s", value);
	}
}

static int startRoutercheck(int eid, webs_t wp, int argc, char_t **argv)
{

#if 1
	char   buff[256];
	int    nl = 0 ;
	struct in_addr dest;
	struct in_addr gw;
	int    flgs, ref, use, metric;
	unsigned long int d,g,m;
	int    find_default_flag = 0;

	char sgw[16];
	char *cm; //aron add

  FILE *fp = NULL;
 
  fp = fopen("/tmp/ping_router", "r");
  if(fp){
		fclose(fp);
		return websWrite(wp, T("1"));
	}
	else
	{
	fp = fopen("/proc/net/route", "r");

	while (fgets(buff, sizeof(buff), fp) != NULL) {
		if (nl) {
			int ifl = 0;
			while (buff[ifl]!=' ' && buff[ifl]!='\t' && buff[ifl]!='\0')
				ifl++;
			buff[ifl]=0;    /* interface */
			if (sscanf(buff+ifl+1, "%lx%lx%X%d%d%d%lx",
						&d, &g, &flgs, &ref, &use, &metric, &m)!=7) {
				fclose(fp);
				return websWrite(wp, T("0"));
			}

			if (flgs&RTF_UP) {
				dest.s_addr = d;
				gw.s_addr   = g;
				strcpy(sgw, (gw.s_addr==0 ? "" : inet_ntoa(gw)));

				if (dest.s_addr == 0) {
					find_default_flag = 1;
					break;
				}
			}
		}
		nl++;
	}
	fclose(fp);

        //----- aron modify for showing ppp connected IP including dial on-demand  2008.11.25
	//cm = nvram_bufget(RT2860_NVRAM, "wanConnectionMode");
	cm = nvram_safe_get("wan0_proto");
	if (!strncmp(cm, "pppoe", 6) || !strncmp(cm, "l2tp", 5) ||
			!strncmp(cm, "pptp", 5)) //fetch from ppp
	{
           if ( !isConnectPPP() ) //not connected
	   {
	      return websWrite(wp, T("0"));
	   }
        }
        //--------------------------------------------------

	if (find_default_flag == 1)
	{
		if (!strncmp(cm, "wwan", 4))
				doSystem("ping 168.95.1.1 -c 1 > /tmp/ping_router &");
			else
				doSystem("ping %s -c 1 > /tmp/ping_router &", sgw);
		return websWrite(wp, T("1"));
	}
	else
		return websWrite(wp, T("0"));

	return websWrite(wp, T("1"));
	}
#endif
}

#define DD printf("%d\n", __LINE__);fflush(stdout);

/*
 *
 */
int getIndexOfRoutingRule(char *dest, char *netmask, char *interface)
{
#if 0
	int index=0;
	char *rrs, one_rule[256];
	char dest_f[32], netmask_f[32], interface_f[32];
	char    *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode"); //aron add 2009.05.25

    //aron modify 2009.05.25
    if(!strcmp(opmode, "1")) //Router mode
	rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules");
    else if(!strcmp(opmode, "2")) //WISP mode
	rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules2");

	if(!rrs || !strlen(rrs))
		return -1;

	while( getNthValueSafe(index, rrs, ';', one_rule, 256) != -1 ){
		if((getNthValueSafe(0, one_rule, ',', dest_f, sizeof(dest_f)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(1, one_rule, ',', netmask_f, sizeof(netmask_f)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(4, one_rule, ',', interface_f, sizeof(interface_f)) == -1)){
			index++;
			continue;
		}
		//printf("@@@@@ %s %s %s\n", dest_f, netmask_f, interface_f);
		//printf("----- %s %s %s\n", dest, netmask, interface);
		if( (!strcmp(dest, dest_f)) && (!strcmp(netmask, netmask_f)) && (!strcmp(interface, interface_f))){
			return index;
		}
		index++;
	}

	return -1;
#endif
}

/*
 *  aron add for avoiding multiple same static route can be set  2009.05.25
 */
int getIndexOfStaticRoute(char *dest, char *netmask)
{
        char buf_dest[TMP_LEN], buf_mask[TMP_LEN];
        int i, rule_num = atoi(nvram_safe_get("rt_rule_num"));
        
        for(i=0; i<rule_num; i++)
        {
            ezplib_get_attr_val("rt_rule", i, "dstipaddr_start", buf_dest, TMP_LEN, EZPLIB_USE_CLI);
            ezplib_get_attr_val("rt_rule", i, "netmask", buf_mask, TMP_LEN, EZPLIB_USE_CLI); 
            if( (!strcmp(dest, buf_dest)) && (!strcmp(netmask, buf_mask))){
			return i;
	    }
	}
	return -1;
}

static void removeRoutingRule(char *dest, char *netmask, char *ifname)
{
#if 0
	char cmd[1024];
	strcpy(cmd, "route del ");

	// host or net?
	if(!strcmp(netmask, "255.255.255.255") )
		strcat(cmd, "-host ");
	else
		strcat(cmd, "-net ");

	// destination
	strcat(cmd, dest);
	strcat(cmd, " ");

	// netmask
	if(strcmp(netmask, "255.255.255.255"))
		sprintf(cmd, "%s netmask %s", cmd, netmask);

	//interface
	sprintf(cmd, "%s dev %s ", cmd, ifname);
	doSystem(cmd);
#endif
}

void staticRoutingInit(void)
{
#if 0
	int index=0;
	char one_rule[256];
	char *rrs;
	struct in_addr dest_s, gw_s, netmask_s;
	char dest[32], netmask[32], gw[32], interface[32], true_interface[32], custom_interface[32], comment[32], metric_set[32]; //aron add metric_set
	int	flgs, ref, use, metric, nl=0;
	unsigned long int d,g,m;
	int isGatewayMode = (!strcmp("1", nvram_bufget(RT2860_NVRAM, "OperationMode"))) ? 1 : 0 ;

	// delete old user rules
	FILE *fp = fopen("/proc/net/route", "r");
	if(!fp)
		return;

	while (fgets(one_rule, sizeof(one_rule), fp) != NULL) {
		if (nl) {
			if (sscanf(one_rule, "%s%lx%lx%X%d%d%d%lx",
					interface, &d, &g, &flgs, &ref, &use, &metric, &m) != 8) {
				printf("format error\n");
				fclose(fp);
				return;
			}
			dest_s.s_addr = d;
			gw_s.s_addr = g;
			netmask_s.s_addr = m;

			strncpy(dest, inet_ntoa(dest_s), sizeof(dest));
			strncpy(gw, inet_ntoa(gw_s), sizeof(gw));
			strncpy(netmask, inet_ntoa(netmask_s), sizeof(netmask));

			// check if internal routing rules
			if( (index=getIndexOfRoutingRule(dest, netmask, interface)) != -1){
				removeRoutingRule(dest, netmask, interface);
			}
		}
		nl++;
	}
	fclose(fp);

	index = 0;
	rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules");
	if(!rrs|| !strlen(rrs))
		return;

	while( getNthValueSafe(index, rrs, ';', one_rule, 256) != -1 ){
		char cmd[1024];

		if((getNthValueSafe(0, one_rule, ',', dest, sizeof(dest)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(1, one_rule, ',', netmask, sizeof(netmask)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(2, one_rule, ',', gw, sizeof(gw)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(3, one_rule, ',', interface, sizeof(interface)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(4, one_rule, ',', true_interface, sizeof(true_interface)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(5, one_rule, ',', custom_interface, sizeof(custom_interface)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(6, one_rule, ',', comment, sizeof(comment)) == -1)){
			index++;
			continue;
		}
		//aron add for metric
		if((getNthValueSafe(7, one_rule, ',', metric_set, sizeof(metric_set)) == -1)){
			index++;
			continue;
		}

		strcpy(cmd, "route add ");

		// host or net?
		if(!strcmp(netmask, "255.255.255.255") )
			strcat(cmd, "-host ");
		else
			strcat(cmd, "-net ");

		// destination
		strcat(cmd, dest);
		strcat(cmd, " ");

		// netmask
		if(strcmp(netmask, "255.255.255.255") )
			sprintf(cmd, "%s netmask %s", cmd, netmask);

		// gateway
		if(strcmp(gw, "0.0.0.0"))
			sprintf(cmd, "%s gw %s", cmd, gw);

		//aron add for user input metric
		if(strcmp(metric_set, "0"))
			sprintf(cmd, "%s metric %s", cmd, metric_set);

		//interface
//		if (!strcmp(interface, "WAN")){
//			true_interface = getWanIfName();
//		}else if (!gstrcmp(interface, "Custom")){
//			true_interface = custom_interface;
//		}else	// LAN & unknown
//			true_interface = getLanIfName();

//------------ aron add  2009.05.22
                if (!strcmp(interface, "WAN")){
			//true_interface = getWanIfName();
			strcpy(true_interface, getWanIfName());
		}else if (!gstrcmp(interface, "Custom")){
			//true_interface = custom_interface;
		}else	// LAN & unknown
			//true_interface = getLanIfName();
			strcpy(true_interface, getLanIfName());
		//printf("true_interface is %s\n", true_interface);
//----------------------------------------------------------------------------
		sprintf(cmd, "%s dev %s ", cmd, true_interface);

		strcat(cmd, "2>&1 ");

          //aron modify 2009.05.22
          #if 0
		if(strcmp(interface, "WAN") || (!strcmp(interface, "WAN") && isGatewayMode)  ){
			doSystem(cmd);
		}else{
			printf("Skip WAN routing rule in the non-Gateway mode: %s\n", cmd);
		}
	  #else
	        doSystem(cmd);
	  #endif

		index++;
	}
	return;
#endif
}

void dynamicRoutingInit(void)
{
#if 0
	zebraRestart();
	ripdRestart();
#endif
}

void RoutingInit(void)
{
#if 0
	staticRoutingInit();
	dynamicRoutingInit();
#endif
}

static inline int getNums(char *value, char delimit)
{
#if 0
    char *pos = value;
    int count=1;
    if(!pos || !strlen(pos))
        return 0;
    while( (pos = strchr(pos, delimit))){
        pos = pos+1;
        count++;
    }
    return count;
#endif
}

/*
 *  aron add: retrieve static routing numbers from nvram(flash)
 */
static int getStaticRoutingNumbers(int eid, webs_t wp, int argc, char_t **argv)
{
        int rule_number = atoi(nvram_safe_get("rt_rule_num"));
        if(rule_number == 8)
	      return websWrite(wp, "1");  //tables is full
	else
	      return websWrite(wp, "0");
}

/*
 * description: get routing table
 */
static int getRoutingTable(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	char   result[4096] = {0};
	char   buff[512];
	int    nl = 0, index;
	char   ifname[32], interface[128];
	struct in_addr dest, gw, netmask;
	char   dest_str[32], gw_str[32], netmask_str[32], comment[32];
	int    flgs, ref, use, metric;
	int	   *running_rules = NULL;
	unsigned long int d,g,m;
	char *rrs;
	int  rule_count;
	char    *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode"); //aron add 2009.05.25
	
	FILE *fp = fopen("/proc/net/route", "r");
	if(!fp)
		return -1;

    if(!strcmp(opmode, "1")) //Router mode
	rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules");
    else if(!strcmp(opmode, "2")) //WISP mode
        rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules2");
	
	rule_count = getNums(rrs, ';');

	if(rule_count){
		running_rules = calloc(1, sizeof(int) * rule_count);
		if(!running_rules)
			return -1;
	}

	strncat(result, "\"", sizeof(result));
	while (fgets(buff, sizeof(buff), fp) != NULL) {
		if (nl) {
			if (sscanf(buff, "%s%lx%lx%X%d%d%d%lx",
					ifname, &d, &g, &flgs, &ref, &use, &metric, &m) != 8) {
				printf("format error\n");
				fclose(fp);
				return websWrite(wp, T(""));
			}
			dest.s_addr = d;
			gw.s_addr = g;
			netmask.s_addr = m;

			if(! (flgs & 0x1) )	// skip not usable
				continue;

			strncpy(dest_str, inet_ntoa(dest), sizeof(dest_str));
			strncpy(gw_str, inet_ntoa(gw), sizeof(gw_str));
			strncpy(netmask_str, inet_ntoa(netmask), sizeof(netmask_str));

			if(nl > 1)
				strncat(result, ";", sizeof(result));
			strncat(result, ifname, sizeof(result));		strncat(result, ",", sizeof(result));
			strncat(result, dest_str, sizeof(result));		strncat(result, ",", sizeof(result));
			strncat(result, gw_str, sizeof(result));			strncat(result, ",", sizeof(result));
			strncat(result, netmask_str, sizeof(result) );	strncat(result, ",", sizeof(result));
			snprintf(result, sizeof(result), "%s%d,%d,%d,%d,", result, flgs, ref, use, metric);
                        //printf("Aron:1.....result=%s.....\n", result);  //aron test

			// check if internal routing rules
			strcpy(comment, " ");
			if( (index=getIndexOfRoutingRule(dest_str, netmask_str, ifname)) != -1){
				char one_rule[256];

				if(index < rule_count)
					running_rules[index] = 1;
				else
					printf("fatal error in %s\n", __FUNCTION__);

				snprintf(result, sizeof(result), "%s%d,", result, index);
				if(rrs && strlen(rrs)){
					if( getNthValueSafe(index, rrs, ';', one_rule, sizeof(one_rule)) != -1){

						if( getNthValueSafe(3, one_rule, ',', interface, sizeof(interface)) != -1){
							strncat(result, interface, sizeof(result));
							strncat(result, ",", sizeof(result));
						}
						if( getNthValueSafe(6, one_rule, ',', comment, sizeof(comment)) != -1){
							// do nothing;
						}
					}
				}
			}else{
				strncat(result, "-1,", sizeof(result));
				strncat(result, getLanWanNamebyIf(ifname), sizeof(result));
				strncat(result, ",", sizeof(result));
			}
			strncat(result, "0,", sizeof(result));	// used rule
			strncat(result, comment, sizeof(result));
			//printf("Aron:2.....result=%s.....\n", result);  //aron test
		}
		nl++;
	}

        //printf("Aron:3.....result=%s.....\n", result);  //aron test

	for(index=0; index < rule_count; index++){
		char one_rule[256];

		if(running_rules[index])
			continue;

		if(getNthValueSafe(index, rrs, ';', one_rule, sizeof(one_rule)) == -1)
			continue;

		if(getNthValueSafe(0, one_rule, ',', dest_str, sizeof(dest_str)) == -1)
			continue;

		if(getNthValueSafe(1, one_rule, ',', netmask_str, sizeof(netmask_str)) == -1)
			continue;

		if(getNthValueSafe(2, one_rule, ',', gw_str, sizeof(gw_str)) == -1)
			continue;

		if(getNthValueSafe(3, one_rule, ',', interface, sizeof(interface)) == -1)
			continue;

		if(getNthValueSafe(4, one_rule, ',', ifname, sizeof(ifname)) == -1)
			continue;

		if(getNthValueSafe(6, one_rule, ',', comment, sizeof(comment)) == -1)
			continue;

		if(strlen(result))
			strncat(result, ";", sizeof(result));

		snprintf(result, sizeof(result), "%s%s,%s,%s,%s,0,0,0,0,%d,%s,1,%s", result, ifname, dest_str, gw_str, netmask_str, index, interface, comment);
	        //printf("Aron:4.....result=%s.....\n", result);  //aron test
	}

        //printf("Aron:5.....result=%s.....\n", result);  //aron test

	strcat(result, "\"");
	websLongWrite(wp, result);
	fclose(fp);
	if(running_rules)
		free(running_rules);
	//printf("%s\n", result);
	return 0;
	
#else //for AXIM platform
        char result[4096] = {0};
        char tmp[TMP_LEN], buf[TMP_LEN];
        int i, rule_num;
        
        rule_num = atoi(nvram_safe_get("rt_rule_num"));
        strcat(result, "\"");
        for(i=0; i<rule_num; i++)
        {
            ezplib_get_rule("rt_rule", i, buf, TMP_LEN);
            if(i==0)
            {
                strcat(result, buf);
            }
            else
            {
                sprintf(tmp,"|%s", buf);
                strcat(result, tmp);
            } 
	}
	strcat(result, "\"");
	//printf("result is %s.............\n", result);
        websLongWrite(wp, result);
#endif
}

static void addRouting(webs_t wp, char_t *path, char_t *query)
{
#if 0
        char_t *dest, *netmask, *gateway, *interface, *comment, *metric;  //aron add metric
	char tmp[TMP_LEN], rule[TMP_LEN];
	char *nv_rt_rule_num = "rt_rule_num";
	int index=0;
        
        dest = websGetVar(wp, T("dest"), T(""));
	netmask = websGetVar(wp, T("netmask"), T(""));
	gateway = websGetVar(wp, T("gateway"), T(""));
	interface = websGetVar(wp, T("interface"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));
	metric = websGetVar(wp, T("metric"), T(""));  //aron add

        if( (index=getIndexOfStaticRoute(dest, netmask)) != -1) //user input route is already existing
        {
        	char temp2_str[65];
		strcpy(temp2_str, "Static Route already exists.");
		setWebMessage(1, (char *)&temp2_str);
		websRedirect(wp, "/local/advance/sr.asp");
	        return;
	}

        if(!strcmp(interface, "WAN"))
             interface = "wan0";
        else if(!strcmp(interface, "LAN"))
             interface = "lan0";
        
        netmask = mask2prefix(netmask);
        
        int rt_num = atoi(nvram_safe_get(nv_rt_rule_num));
        rt_num++;
        sprintf(tmp,"%d", rt_num);
        nvram_set(nv_rt_rule_num, tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "%s^%s^^^%s^^^^^^^%s^%s^%s^%s^%s",
                     comment, "1", dest, interface, "static", gateway, netmask, metric);
        
        //preaction
        snprintf(tmp, TMP_LEN, "NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop rt");
        system(tmp);
        
        ezplib_append_rule("rt_rule", rule);
        
        //postaction
        snprintf(tmp, TMP_LEN, "NUM=0 /etc/rc.common /etc/rc.d/S60network-acl start rt");
        system(tmp);
    
        nvram_commit();
        
        setWebMessage(0, NULL);
        websRedirect(wp, "/local/advance/sr.asp");
        return;
        
#else //to do user-input error check
        char_t *dest, *netmask, *gateway, *interface, *comment, *metric;  //aron add metric
	char tmp[TMP_LEN], rule[TMP_LEN];
	char *nv_rt_rule_num = "rt_rule_num";
	int index=0;
	
	char cmd[256] = {0}; //aron add
	char result[256] = {0}; //aron add
	char TempBuf[32]; //aron add
	FILE *fp; //aron add
	char_t *true_interface; //aron add
        
        dest = websGetVar(wp, T("dest"), T(""));
	netmask = websGetVar(wp, T("netmask"), T(""));
	gateway = websGetVar(wp, T("gateway"), T(""));
	interface = websGetVar(wp, T("interface"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));
	metric = websGetVar(wp, T("metric"), T(""));  //aron add

        if( (index=getIndexOfStaticRoute(dest, netmask)) != -1) //user input route is already existing
        {
        	char temp2_str[65];
		strcpy(temp2_str, "Static Route already exists.");
		setWebMessage(1, (char *)&temp2_str);
		websRedirect(wp, "/local/advance/sr.asp");
	        return;
	}

        ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
        if(!strcmp(interface, "WAN"))
        {
             interface = "wan0";
             if (!strcmp(TempBuf, "normal"))
                  true_interface = "vlan2";
             else if (!strcmp(TempBuf, "wisp"))
                  true_interface = "apcli0";
        }
        else if(!strcmp(interface, "LAN"))
        {
             interface = "lan0";
             true_interface = "br0";
        }

        //check user-input correctness at first
        snprintf(cmd, sizeof(cmd),
                     "route add -net %s netmask %s gw %s dev %s 2>&1 ",
                     dest, netmask, gateway, true_interface);
       
        printf("%s\n", cmd);
	fp = popen(cmd, "r");
	fgets(result, sizeof(result), fp);
	pclose(fp);
	
	if(!strlen(result)){ // success, write down to the flash
	       //remove tested routing rule first
	       if(!strcmp(netmask, "255.255.255.255")) //host
	       {
	          snprintf(cmd, sizeof(cmd),
                            "route del -host %s dev %s", dest, true_interface);
               }
               else //net
               {
               	  snprintf(cmd, sizeof(cmd),
                            "route del -net %s netmask %s dev %s", dest, netmask, true_interface);
               }
               system(cmd);
               //remove default gateway possibly set here
               snprintf(cmd, sizeof(cmd), "route del default gw %s", gateway);
               system(cmd);
	       
	       netmask = mask2prefix(netmask);
	       
	       //update nvram in flash
	       int rt_num = atoi(nvram_safe_get(nv_rt_rule_num));
               rt_num++;
               sprintf(tmp,"%d", rt_num);
               nvram_set(nv_rt_rule_num, tmp); //update rule numbers
        
               snprintf(rule, sizeof(rule),
                         "%s^%s^^^%s^^^^^^^%s^%s^%s^%s^%s",
                         comment, "1", dest, interface, "static", gateway, netmask, metric);
  
               //preaction
               snprintf(tmp, TMP_LEN, "NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop rt");
               system(tmp);
        
               ezplib_append_rule("rt_rule", rule);
        
              //postaction
              snprintf(tmp, TMP_LEN, "NUM=0 /etc/rc.common /etc/rc.d/S60network-acl start rt");
              system(tmp);
    
              nvram_commit();
        
              setWebMessage(0, NULL);
              websRedirect(wp, "/local/advance/sr.asp");
              return;
        }
        else
        {
	      char temp_str[65];
	      strcpy(temp_str, "Static Route rule setting fail.");
	      setWebMessage(1, (char *)&temp_str);
	      websRedirect(wp, "/local/advance/sr.asp");
	      return;
	}
#endif
}

static void delRouting(webs_t wp, char_t *path, char_t *query)
{
        char *value;
        char tmp[TMP_LEN], buf[TMP_LEN];
        int i, j=0, left_num=0, rule_num;
        
        //preaction
        snprintf(buf, TMP_LEN, "NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop rt");
        system(buf);
        
        rule_num = atoi(nvram_safe_get("rt_rule_num"));
        for(i=0; i<rule_num; i++)
        {
                snprintf(tmp, sizeof(tmp), "DR%d", i);
		value = websGetVar(wp, tmp, NULL);
		if(value)
		{
		    if( (left_num = ezplib_get_rule_num("rt_rule")) ==1)
		    {
		    	nvram_set("rt_rule", "");
		    }
		    else
		        ezplib_delete_rule("rt_rule", i);
		    j++;
		}
	}
	
	if(j>0)
	{
           rule_num = rule_num - j;
           sprintf(tmp,"%d", rule_num);
           nvram_set("rt_rule_num", tmp); //update rule numbers
        }
        
        //postaction
        snprintf(buf, TMP_LEN, "NUM=0 /etc/rc.common /etc/rc.d/S60network-acl start rt");
        system(buf);
        
        nvram_commit();
        
        setWebMessage(0, NULL);
        websRedirect(wp, "/local/advance/sr.asp");
}

void ripdRestart(void)
{
#if 0
	char lan_ip[16], wan_ip[16], lan_mask[16], wan_mask[16];

	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *password = nvram_bufget(RT2860_NVRAM, "Password");
	char *RIPEnable = nvram_bufget(RT2860_NVRAM, "RIPEnable");
	char *RIPver = nvram_bufget(RT2860_NVRAM, "RIPversion"); //aron add

	doSystem("killall -q ripd");

	if(!opmode||!strlen(opmode))
		return;
	if(!strcmp(opmode, "0"))	// bridge
		return;

	if(!RIPEnable || !strlen(RIPEnable) || !strcmp(RIPEnable,"0"))
        return;

	if(!password || !strlen(password))
		password = "rt2880";

	doSystem("echo \"hostname linux.router1\" > /etc/ripd.conf ");
	doSystem("echo \"password %s\" >> /etc/ripd.conf ", password);
	doSystem("echo \"router rip\" >> /etc/ripd.conf ");
        if(!strcmp(RIPver, "1"))  //aron add for advertise default route(0.0.0.0) in v1  2009.02.24
	     doSystem("echo \"default-information originate\" >> /etc/ripd.conf");

	// deal with WAN
	if(getIfIp(getWanIfName(), wan_ip) != -1){
		if(getIfNetmask(getWanIfName(), wan_mask) != -1){
			doSystem("echo \"network %s/%d\" >> /etc/ripd.conf", wan_ip, netmask_aton(wan_mask));
			doSystem("echo \"network %s\" >> /etc/ripd.conf", getWanIfName());
		}else
			printf("ripdRestart(): The WAN IP is still undeterminated...\n");
	}else
		printf("ripdRestart(): The WAN IP is still undeterminated...\n");

	// deal with LAN
	if(getIfIp(getLanIfName(), lan_ip) != -1){
		if(getIfNetmask(getLanIfName(), lan_mask) != -1){
			doSystem("echo \"network %s/%d\" >> /etc/ripd.conf", lan_ip, netmask_aton(lan_mask));
			doSystem("echo \"network %s\" >> /etc/ripd.conf", getLanIfName());
		}
	}
	if(!strcmp(RIPver, "1"))  //aron modify
	     doSystem("echo \"version 1\" >> /etc/ripd.conf");
	else if(!strcmp(RIPver, "2"))
	     doSystem("echo \"version 2\" >> /etc/ripd.conf");
	doSystem("echo \"log syslog\" >> /etc/ripd.conf");
	doSystem("ripd -f /etc/ripd.conf -d");
#endif
}

inline void zebraRestart(void)
{
#if 0
	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *password = nvram_bufget(RT2860_NVRAM, "Password");

	char *RIPEnable = nvram_bufget(RT2860_NVRAM, "RIPEnable");

	doSystem("killall -q zebra");

	if(!opmode||!strlen(opmode))
		return;
	if(!strcmp(opmode, "0"))	// bridge
		return;

	if(!RIPEnable || !strlen(RIPEnable) || !strcmp(RIPEnable,"0"))
		return;

	if(!password || !strlen(password))
		password = "rt2880";

	doSystem("echo \"hostname linux.router1\" > /etc/zebra.conf ");
	doSystem("echo \"password %s\" >> /etc/zebra.conf ", password);
	doSystem("echo \"enable password rt2880\" >> /etc/zebra.conf ");
	doSystem("echo \"log syslog\" >> /etc/zebra.conf ");
	doSystem("zebra -d -f /etc/zebra.conf");
#endif
}

//aron add for RIP
static void ripRouting(webs_t wp, char_t *path, char_t *query)
{
        char *ripSelect;
        ripSelect = websGetVar(wp, "RIPSelect", "");
        
        if(!strcmp(ripSelect, "0"))
            ripSelect = "none";
	else if(!strcmp(ripSelect, "1"))
	    ripSelect = "v1";
	else if(!strcmp(ripSelect, "2"))
            ripSelect = "v2";
            
        //preaction
        system("/etc/init.d/quagga stop");
       
        ezplib_replace_attr("rt_rip_rule", 0, "rip", ripSelect);
     
        if(!strcmp(ripSelect, "v1") || !strcmp(ripSelect, "v2"))
        {
           //postaction
           system("/etc/init.d/quagga start");
        }
    
        nvram_commit();
	    
	setWebMessage(0, NULL);

	char_t *submitUrl;
        submitUrl = websGetVar(wp, T("rip_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
}

static void dynamicRouting(webs_t wp, char_t *path, char_t *query)
{
#if 0
	char_t *rip;
	char *RIPEnable;
	rip = websGetVar(wp, T("RIPSelect"), T(""));
	if(!rip || !strlen(rip))
		return;

	RIPEnable = nvram_bufget(RT2860_NVRAM, "RIPEnable");
	if(!RIPEnable || !strlen(RIPEnable))
		RIPEnable = "0";

	if(!gstrcmp(rip, "0") && !strcmp(RIPEnable, "0")){
		// nothing changed
	}else if(!gstrcmp(rip, "1") && !strcmp(RIPEnable, "1")){
		// nothing changed
	}else if(!gstrcmp(rip, "0") && !strcmp(RIPEnable, "1")){
		nvram_bufset(RT2860_NVRAM, "RIPEnable", rip);
		nvram_commit(RT2860_NVRAM);
		doSystem("killall -q ripd");
		doSystem("killall -q zebra");
	}else if(!gstrcmp(rip, "1") && !strcmp(RIPEnable, "0")){
		nvram_bufset(RT2860_NVRAM, "RIPEnable", rip);
		nvram_commit(RT2860_NVRAM);
		zebraRestart();
		ripdRestart();
	}else{
		return;
	}

	//debug print
	websHeader(wp);
	websWrite(wp, T("<h3>Dynamic Routing:</h3><br>\n"));
	websWrite(wp, T("RIPEnable %s<br>\n"), rip);
	websFooter(wp);
	websDone(wp, 200);
#endif
}

// Tommy, WLAN Scheduling , 2008/12/9 05:15¤U¤È
void create_crond_file(void)
{
#if 0
	char *Everyday,*Mon,*Tue,*Wed,*Thu,*Fri,*Sat,*Sun;
	char *Everyday_ON,*Mon_ON,*Tue_ON,*Wed_ON,*Thu_ON,*Fri_ON,*Sat_ON,*Sun_ON;
	char *Everyday_start_hour,*Everyday_start_min,*Everyday_end_hour,*Everyday_end_min,
	     *Mon_start_hour,*Mon_start_min,*Mon_end_hour,*Mon_end_min,
	     *Tue_start_hour,*Tue_start_min,*Tue_end_hour,*Tue_end_min,
	     *Wed_start_hour,*Wed_start_min,*Wed_end_hour,*Wed_end_min,
	     *Thu_start_hour,*Thu_start_min,*Thu_end_hour,*Thu_end_min,
	     *Fri_start_hour,*Fri_start_min,*Fri_end_hour,*Fri_end_min,
	     *Sat_start_hour,*Sat_start_min,*Sat_end_hour,*Sat_end_min,
	     *Sun_start_hour,*Sun_start_min,*Sun_end_hour,*Sun_end_min;
	int  min_30=30,min_0=0;

	char *username;

	/* WLAN Scheduling */
	char *scheduling_enable;
	scheduling_enable = nvram_bufget(RT2860_NVRAM, "WiFiScheduleEnable");

	username = nvram_bufget(RT2860_NVRAM, "Login");

	// Create "/var/spool/cron/crontabs" directory, otherwise
	// it cannot open "/var/spool/cron/crontabs/temp.dat"
	doSystem("mkdir -p /var/spool/cron/crontabs");
	// Tommy, re-create scheduler file, so delete the file first ,2009/3/13 10:03
	doSystem("rm -f /var/spool/cron/crontabs/scheduler");

	FILE *cron_conf = fopen( "/var/spool/cron/crontabs/temp.dat", "w" );

	/* Add setting into crontab file from web page */
	Everyday = nvram_bufget(RT2860_NVRAM, "WiFiScheduling_day_Active_0");
	Mon = nvram_bufget(RT2860_NVRAM, "WiFiScheduling_day_Active_1");
	Tue = nvram_bufget(RT2860_NVRAM, "WiFiScheduling_day_Active_2");
	Wed = nvram_bufget(RT2860_NVRAM, "WiFiScheduling_day_Active_3");
	Thu = nvram_bufget(RT2860_NVRAM, "WiFiScheduling_day_Active_4");
	Fri = nvram_bufget(RT2860_NVRAM, "WiFiScheduling_day_Active_5");
	Sat = nvram_bufget(RT2860_NVRAM, "WiFiScheduling_day_Active_6");
	Sun = nvram_bufget(RT2860_NVRAM, "WiFiScheduling_day_Active_7");

	Everyday_ON = nvram_bufget(RT2860_NVRAM, "WiFischeduling_action_0");
	Mon_ON = nvram_bufget(RT2860_NVRAM, "WiFischeduling_action_1");
	Tue_ON = nvram_bufget(RT2860_NVRAM, "WiFischeduling_action_2");
	Wed_ON = nvram_bufget(RT2860_NVRAM, "WiFischeduling_action_3");
	Thu_ON = nvram_bufget(RT2860_NVRAM, "WiFischeduling_action_4");
	Fri_ON = nvram_bufget(RT2860_NVRAM, "WiFischeduling_action_5");
	Sat_ON = nvram_bufget(RT2860_NVRAM, "WiFischeduling_action_6");
	Sun_ON = nvram_bufget(RT2860_NVRAM, "WiFischeduling_action_7");

	Everyday_start_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_hour_0");
	Everyday_start_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_min_0");
	Everyday_end_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_hour_0");
	Everyday_end_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_min_0");


	Mon_start_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_hour_1");
	Mon_start_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_min_1");
	Mon_end_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_hour_1");
	Mon_end_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_min_1");

	Tue_start_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_hour_2");
	Tue_start_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_min_2");
	Tue_end_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_hour_2");
	Tue_end_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_min_2");

	Wed_start_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_hour_3");
	Wed_start_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_min_3");
	Wed_end_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_hour_3");
	Wed_end_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_min_3");

	Thu_start_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_hour_4");
	Thu_start_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_min_4");
	Thu_end_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_hour_4");
	Thu_end_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_min_4");

	Fri_start_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_hour_5");
	Fri_start_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_min_5");
	Fri_end_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_hour_5");
	Fri_end_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_min_5");

	Sat_start_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_hour_6");
	Sat_start_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_min_6");
	Sat_end_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_hour_6");
	Sat_end_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_min_6");

	Sun_start_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_hour_7");
	Sun_start_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_start_min_7");
	Sun_end_hour = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_hour_7");
	Sun_end_min = nvram_bufget(RT2860_NVRAM, "WiFischeduling_end_min_7");

	if (!strcmp(Everyday, "1")){
	    if ( (strcmp(Everyday_start_hour,Everyday_end_hour)) || (strcmp(Everyday_start_min,Everyday_end_min))){
	    	if (strcmp(Everyday_ON, "1")){
		    fprintf(cron_conf,"%d %d * * 0-6 iwpriv ra0 set RadioOn=1\n", (atoi(Everyday_start_min)? min_30:min_0),atoi(Everyday_start_hour) );
		    fprintf(cron_conf,"%d %d * * 0-6 nvram_set SchedulerRadioOn 1\n", (atoi(Everyday_start_min)? min_30:min_0),atoi(Everyday_start_hour) );

		    if (!strcmp(Everyday_end_hour,"24")){
		    	if ((!strcmp(Everyday_start_hour, "0")) && (!strcmp(Everyday_start_min,"0"))){	
			}else{
		    		fprintf(cron_conf,"0 0 * * 0-6 iwpriv ra0 set RadioOn=0\n" );
		    		fprintf(cron_conf,"0 0 * * 0-6 nvram_set SchedulerRadioOn 0\n" );			
			}
		    }else{
		    	fprintf(cron_conf,"%d %d * * 0-6 iwpriv ra0 set RadioOn=0\n", (atoi(Everyday_end_min)? min_30:min_0),atoi(Everyday_end_hour) );
		    	fprintf(cron_conf,"%d %d * * 0-6 nvram_set SchedulerRadioOn 0\n", (atoi(Everyday_end_min)? min_30:min_0),atoi(Everyday_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Eve %d%s %d%s 1 >> /var/spool/cron/crontabs/scheduler", atoi(Everyday_start_hour),(atoi(Everyday_start_min)? "30":"00"), atoi(Everyday_end_hour),(atoi(Everyday_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
				syslog(LOG_NOTICE, "Scheduling %s, Everyday %d:%s - %d:%s Radio ON",atoi(scheduling_enable)? "enable":"disable", atoi(Everyday_start_hour),(atoi(Everyday_start_min)? "30":"00"), atoi(Everyday_end_hour),(atoi(Everyday_end_min)? "30":"00"));	
		}else{
		    fprintf(cron_conf,"%d %d * * 0-6 iwpriv ra0 set RadioOn=0\n", (atoi(Everyday_start_min)? min_30:min_0),atoi(Everyday_start_hour) );
		    fprintf(cron_conf,"%d %d * * 0-6 nvram_set SchedulerRadioOn 0\n", (atoi(Everyday_start_min)? min_30:min_0),atoi(Everyday_start_hour) );
		    
		    if (!strcmp(Everyday_end_hour,"24")){
		    	if ((!strcmp(Everyday_start_hour, "0")) && (!strcmp(Everyday_start_min,"0"))){	
			}else{
		    		fprintf(cron_conf,"0 0 * * 0-6 iwpriv ra0 set RadioOn=1\n" );
		    		fprintf(cron_conf,"0 0 * * 0-6 nvram_set SchedulerRadioOn 1\n" );			
			}
		    }else{
		    fprintf(cron_conf,"%d %d * * 0-6 iwpriv ra0 set RadioOn=1\n", (atoi(Everyday_end_min)? min_30:min_0),atoi(Everyday_end_hour) );
		    fprintf(cron_conf,"%d %d * * 0-6 nvram_set SchedulerRadioOn 1\n", (atoi(Everyday_end_min)? min_30:min_0),atoi(Everyday_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Eve %d%s %d%s 0 >> /var/spool/cron/crontabs/scheduler", atoi(Everyday_start_hour),(atoi(Everyday_start_min)? "30":"00"), atoi(Everyday_end_hour),(atoi(Everyday_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Everyday %d:%s - %d:%s Radio OFF",atoi(scheduling_enable)? "enable":"disable", atoi(Everyday_start_hour),(atoi(Everyday_start_min)? "30":"00"), atoi(Everyday_end_hour),(atoi(Everyday_end_min)? "30":"00"));	
	    	}
	    }
	}

	if (!strcmp(Mon, "1")){
	    if ( (strcmp(Mon_start_hour,Mon_end_hour)) || (strcmp(Mon_start_min,Mon_end_min))){
	    	if (strcmp(Mon_ON, "1")){
		    fprintf(cron_conf,"%d %d * * 1 iwpriv ra0 set RadioOn=1\n", (atoi(Mon_start_min)? min_30:min_0),atoi(Mon_start_hour) );
		    fprintf(cron_conf,"%d %d * * 1 nvram_set SchedulerRadioOn 1\n", (atoi(Mon_start_min)? min_30:min_0),atoi(Mon_start_hour) );
		    
		    if (!strcmp(Mon_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 2 iwpriv ra0 set RadioOn=0\n");
		    	fprintf(cron_conf,"0 0 * * 2 nvram_set SchedulerRadioOn 0\n");
		    }else{
		    	fprintf(cron_conf,"%d %d * * 1 iwpriv ra0 set RadioOn=0\n", (atoi(Mon_end_min)? min_30:min_0),atoi(Mon_end_hour) );
		    	fprintf(cron_conf,"%d %d * * 1 nvram_set SchedulerRadioOn 0\n", (atoi(Mon_end_min)? min_30:min_0),atoi(Mon_end_hour) );		    
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Mon %d%s %d%s 1 >> /var/spool/cron/crontabs/scheduler", atoi(Mon_start_hour),(atoi(Mon_start_min)? "30":"00"), atoi(Mon_end_hour),(atoi(Mon_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Monday %d:%s - %d:%s Radio ON",atoi(scheduling_enable)? "enable":"disable", atoi(Mon_start_hour),(atoi(Mon_start_min)? "30":"00"), atoi(Mon_end_hour),(atoi(Mon_end_min)? "30":"00"));	
		}else{
		    fprintf(cron_conf,"%d %d * * 1 iwpriv ra0 set RadioOn=0\n", (atoi(Mon_start_min)? min_30:min_0),atoi(Mon_start_hour) );
		    fprintf(cron_conf,"%d %d * * 1 nvram_set SchedulerRadioOn 0\n", (atoi(Mon_start_min)? min_30:min_0),atoi(Mon_start_hour) );

		    if (!strcmp(Mon_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 2 iwpriv ra0 set RadioOn=1\n");
		    	fprintf(cron_conf,"0 0 * * 2 nvram_set SchedulerRadioOn 1\n");
		    }else{		    
		    fprintf(cron_conf,"%d %d * * 1 iwpriv ra0 set RadioOn=1\n", (atoi(Mon_end_min)? min_30:min_0),atoi(Mon_end_hour) );
		    fprintf(cron_conf,"%d %d * * 1 nvram_set SchedulerRadioOn 1\n", (atoi(Mon_end_min)? min_30:min_0),atoi(Mon_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Mon %d%s %d%s 0 >> /var/spool/cron/crontabs/scheduler", atoi(Mon_start_hour),(atoi(Mon_start_min)? "30":"00"), atoi(Mon_end_hour),(atoi(Mon_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Monday %d:%s - %d:%s Radio OFF",atoi(scheduling_enable)? "enable":"disable", atoi(Mon_start_hour),(atoi(Mon_start_min)? "30":"00"), atoi(Mon_end_hour),(atoi(Mon_end_min)? "30":"00"));	

	    	}
	    }
	}

	if (!strcmp(Tue, "1")){
	    if ( (strcmp(Tue_start_hour,Tue_end_hour)) || (strcmp(Tue_start_min,Tue_end_min))){
	    	if (strcmp(Tue_ON, "1")){
		    fprintf(cron_conf,"%d %d * * 2 iwpriv ra0 set RadioOn=1\n", (atoi(Tue_start_min)? min_30:min_0),atoi(Tue_start_hour) );
		    fprintf(cron_conf,"%d %d * * 2 nvram_set SchedulerRadioOn 1\n", (atoi(Tue_start_min)? min_30:min_0),atoi(Tue_start_hour) );
		    
		    if (!strcmp(Tue_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 3 iwpriv ra0 set RadioOn=0\n");
		    	fprintf(cron_conf,"0 0 * * 3 nvram_set SchedulerRadioOn 0\n");
		    }else{
		    	fprintf(cron_conf,"%d %d * * 2 iwpriv ra0 set RadioOn=0\n", (atoi(Tue_end_min)? min_30:min_0),atoi(Tue_end_hour) );
		    	fprintf(cron_conf,"%d %d * * 2 nvram_set SchedulerRadioOn 0\n", (atoi(Tue_end_min)? min_30:min_0),atoi(Tue_end_hour) );		    
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Tue %d%s %d%s 1 >> /var/spool/cron/crontabs/scheduler", atoi(Tue_start_hour),(atoi(Tue_start_min)? "30":"00"), atoi(Tue_end_hour),(atoi(Tue_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Tuesday %d:%s - %d:%s Radio ON",atoi(scheduling_enable)? "enable":"disable", atoi(Tue_start_hour),(atoi(Tue_start_min)? "30":"00"), atoi(Tue_end_hour),(atoi(Tue_end_min)? "30":"00"));	

		}else{
		    fprintf(cron_conf,"%d %d * * 2 iwpriv ra0 set RadioOn=0\n", (atoi(Tue_start_min)? min_30:min_0),atoi(Tue_start_hour) );
		    fprintf(cron_conf,"%d %d * * 2 nvram_set SchedulerRadioOn 0\n", (atoi(Tue_start_min)? min_30:min_0),atoi(Tue_start_hour) );
		    
		    if (!strcmp(Tue_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 3 iwpriv ra0 set RadioOn=1\n");
		    	fprintf(cron_conf,"0 0 * * 3 nvram_set SchedulerRadioOn 1\n");
		    }else{		    
		    fprintf(cron_conf,"%d %d * * 2 iwpriv ra0 set RadioOn=1\n", (atoi(Tue_end_min)? min_30:min_0),atoi(Tue_end_hour) );
		    fprintf(cron_conf,"%d %d * * 2 nvram_set SchedulerRadioOn 1\n", (atoi(Tue_end_min)? min_30:min_0),atoi(Tue_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Tue %d%s %d%s 0 >> /var/spool/cron/crontabs/scheduler", atoi(Tue_start_hour),(atoi(Tue_start_min)? "30":"00"), atoi(Tue_end_hour),(atoi(Tue_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Tuesday %d:%s - %d:%s Radio OFF",atoi(scheduling_enable)? "enable":"disable", atoi(Tue_start_hour),(atoi(Tue_start_min)? "30":"00"), atoi(Tue_end_hour),(atoi(Tue_end_min)? "30":"00"));
	    	}
	    }
	}

	if (!strcmp(Wed, "1")){
	    if ( (strcmp(Wed_start_hour,Wed_end_hour)) || (strcmp(Wed_start_min,Wed_end_min))){
	    	if (strcmp(Wed_ON, "1")){
		    fprintf(cron_conf,"%d %d * * 3 iwpriv ra0 set RadioOn=1\n", (atoi(Wed_start_min)? min_30:min_0),atoi(Wed_start_hour) );
		    fprintf(cron_conf,"%d %d * * 3 nvram_set SchedulerRadioOn 1\n", (atoi(Wed_start_min)? min_30:min_0),atoi(Wed_start_hour) );
		    
		    if (!strcmp(Wed_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 4 iwpriv ra0 set RadioOn=0\n");
		    	fprintf(cron_conf,"0 0 * * 4 nvram_set SchedulerRadioOn 0\n");
		    }else{
		    	fprintf(cron_conf,"%d %d * * 3 iwpriv ra0 set RadioOn=0\n", (atoi(Wed_end_min)? min_30:min_0),atoi(Wed_end_hour) );
		    	fprintf(cron_conf,"%d %d * * 3 nvram_set SchedulerRadioOn 0\n", (atoi(Wed_end_min)? min_30:min_0),atoi(Wed_end_hour) );		    
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Wed %d%s %d%s 1 >> /var/spool/cron/crontabs/scheduler", atoi(Wed_start_hour),(atoi(Wed_start_min)? "30":"00"), atoi(Wed_end_hour),(atoi(Wed_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Wednesday %d:%s - %d:%s Radio ON",atoi(scheduling_enable)? "enable":"disable", atoi(Wed_start_hour),(atoi(Wed_start_min)? "30":"00"), atoi(Wed_end_hour),(atoi(Wed_end_min)? "30":"00"));
		}else{
		    fprintf(cron_conf,"%d %d * * 3 iwpriv ra0 set RadioOn=0\n", (atoi(Wed_start_min)? min_30:min_0),atoi(Wed_start_hour) );
		    fprintf(cron_conf,"%d %d * * 3 nvram_set SchedulerRadioOn 0\n", (atoi(Wed_start_min)? min_30:min_0),atoi(Wed_start_hour) );
		    
		    if (!strcmp(Wed_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 4 iwpriv ra0 set RadioOn=1\n");
		    	fprintf(cron_conf,"0 0 * * 4 nvram_set SchedulerRadioOn 1\n");
		    }else{		    
		    fprintf(cron_conf,"%d %d * * 3 iwpriv ra0 set RadioOn=1\n", (atoi(Wed_end_min)? min_30:min_0),atoi(Wed_end_hour) );
		    fprintf(cron_conf,"%d %d * * 3 nvram_set SchedulerRadioOn 1\n", (atoi(Wed_end_min)? min_30:min_0),atoi(Wed_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Wed %d%s %d%s 0 >> /var/spool/cron/crontabs/scheduler", atoi(Wed_start_hour),(atoi(Wed_start_min)? "30":"00"), atoi(Wed_end_hour),(atoi(Wed_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Wednesday %d:%s - %d:%s Radio OFF",atoi(scheduling_enable)? "enable":"disable", atoi(Wed_start_hour),(atoi(Wed_start_min)? "30":"00"), atoi(Wed_end_hour),(atoi(Wed_end_min)? "30":"00"));
	    	}
	    }
	}

	if (!strcmp(Thu, "1")){
	    if ( (strcmp(Thu_start_hour,Thu_end_hour)) || (strcmp(Thu_start_min,Thu_end_min))){
	    	if (strcmp(Thu_ON, "1")){
		    fprintf(cron_conf,"%d %d * * 4 iwpriv ra0 set RadioOn=1\n", (atoi(Thu_start_min)? min_30:min_0),atoi(Thu_start_hour) );
		    fprintf(cron_conf,"%d %d * * 4 nvram_set SchedulerRadioOn 1\n", (atoi(Thu_start_min)? min_30:min_0),atoi(Thu_start_hour) );
		    
		    if (!strcmp(Thu_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 5 iwpriv ra0 set RadioOn=0\n");
		    	fprintf(cron_conf,"0 0 * * 5 nvram_set SchedulerRadioOn 0\n");
		    }else{
		    	fprintf(cron_conf,"%d %d * * 4 iwpriv ra0 set RadioOn=0\n", (atoi(Thu_end_min)? min_30:min_0),atoi(Thu_end_hour) );
		    	fprintf(cron_conf,"%d %d * * 4 nvram_set SchedulerRadioOn 0\n", (atoi(Thu_end_min)? min_30:min_0),atoi(Thu_end_hour) );		    
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Thu %d%s %d%s 1 >> /var/spool/cron/crontabs/scheduler", atoi(Thu_start_hour),(atoi(Thu_start_min)? "30":"00"), atoi(Thu_end_hour),(atoi(Thu_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Thursday %d:%s - %d:%s Radio ON",atoi(scheduling_enable)? "enable":"disable", atoi(Thu_start_hour),(atoi(Thu_start_min)? "30":"00"), atoi(Thu_end_hour),(atoi(Thu_end_min)? "30":"00"));

		}else{
		    fprintf(cron_conf,"%d %d * * 4 iwpriv ra0 set RadioOn=0\n", (atoi(Thu_start_min)? min_30:min_0),atoi(Thu_start_hour) );
		    fprintf(cron_conf,"%d %d * * 4 nvram_set SchedulerRadioOn 0\n", (atoi(Thu_start_min)? min_30:min_0),atoi(Thu_start_hour) );
		    
		    if (!strcmp(Thu_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 5 iwpriv ra0 set RadioOn=1\n");
		    	fprintf(cron_conf,"0 0 * * 5 nvram_set SchedulerRadioOn 1\n");
		    }else{		    
		    fprintf(cron_conf,"%d %d * * 4 iwpriv ra0 set RadioOn=1\n", (atoi(Thu_end_min)? min_30:min_0),atoi(Thu_end_hour) );
		    fprintf(cron_conf,"%d %d * * 4 nvram_set SchedulerRadioOn 1\n", (atoi(Thu_end_min)? min_30:min_0),atoi(Thu_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Thu %d%s %d%s 0 >> /var/spool/cron/crontabs/scheduler", atoi(Thu_start_hour),(atoi(Thu_start_min)? "30":"00"), atoi(Thu_end_hour),(atoi(Thu_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Thursday %d:%s - %d:%s Radio OFF",atoi(scheduling_enable)? "enable":"disable", atoi(Thu_start_hour),(atoi(Thu_start_min)? "30":"00"), atoi(Thu_end_hour),(atoi(Thu_end_min)? "30":"00"));
	    	}
	    }
	}

	if (!strcmp(Fri, "1")){
	    if ( (strcmp(Fri_start_hour,Fri_end_hour)) || (strcmp(Fri_start_min,Fri_end_min))){
	    	if (strcmp(Fri_ON, "1")){
		    fprintf(cron_conf,"%d %d * * 5 iwpriv ra0 set RadioOn=1\n", (atoi(Fri_start_min)? min_30:min_0),atoi(Fri_start_hour) );
		    fprintf(cron_conf,"%d %d * * 5 nvram_set SchedulerRadioOn 1\n", (atoi(Fri_start_min)? min_30:min_0),atoi(Fri_start_hour) );
		    
		    if (!strcmp(Fri_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 6 iwpriv ra0 set RadioOn=0\n");
		    	fprintf(cron_conf,"0 0 * * 6 nvram_set SchedulerRadioOn 0\n");
		    }else{
		    	fprintf(cron_conf,"%d %d * * 5 iwpriv ra0 set RadioOn=0\n", (atoi(Fri_end_min)? min_30:min_0),atoi(Fri_end_hour) );
		    	fprintf(cron_conf,"%d %d * * 5 nvram_set SchedulerRadioOn 0\n", (atoi(Fri_end_min)? min_30:min_0),atoi(Fri_end_hour) );		    
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Fri %d%s %d%s 1 >> /var/spool/cron/crontabs/scheduler", atoi(Fri_start_hour),(atoi(Fri_start_min)? "30":"00"), atoi(Fri_end_hour),(atoi(Fri_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Friday %d:%s - %d:%s Radio ON",atoi(scheduling_enable)? "enable":"disable", atoi(Fri_start_hour),(atoi(Fri_start_min)? "30":"00"), atoi(Fri_end_hour),(atoi(Fri_end_min)? "30":"00"));
		}else{
		    fprintf(cron_conf,"%d %d * * 5 iwpriv ra0 set RadioOn=0\n", (atoi(Fri_start_min)? min_30:min_0),atoi(Fri_start_hour) );
		    fprintf(cron_conf,"%d %d * * 5 nvram_set SchedulerRadioOn 0\n", (atoi(Fri_start_min)? min_30:min_0),atoi(Fri_start_hour) );
		    
		    if (!strcmp(Fri_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 6 iwpriv ra0 set RadioOn=1\n");
		    	fprintf(cron_conf,"0 0 * * 6 nvram_set SchedulerRadioOn 1\n");
		    }else{		    
		    fprintf(cron_conf,"%d %d * * 5 iwpriv ra0 set RadioOn=1\n", (atoi(Fri_end_min)? min_30:min_0),atoi(Fri_end_hour) );
		    fprintf(cron_conf,"%d %d * * 5 nvram_set SchedulerRadioOn 1\n", (atoi(Fri_end_min)? min_30:min_0),atoi(Fri_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Fri %d%s %d%s 0 >> /var/spool/cron/crontabs/scheduler", atoi(Fri_start_hour),(atoi(Fri_start_min)? "30":"00"), atoi(Fri_end_hour),(atoi(Fri_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Friday %d:%s - %d:%s Radio OFF",atoi(scheduling_enable)? "enable":"disable", atoi(Fri_start_hour),(atoi(Fri_start_min)? "30":"00"), atoi(Fri_end_hour),(atoi(Fri_end_min)? "30":"00"));
	    	}
	    }
	}

	if (!strcmp(Sat, "1")){
	    if ( (strcmp(Sat_start_hour,Sat_end_hour)) || (strcmp(Sat_start_min,Sat_end_min))){
	    	if (strcmp(Sat_ON, "1")){
		    fprintf(cron_conf,"%d %d * * 6 iwpriv ra0 set RadioOn=1\n", (atoi(Sat_start_min)? min_30:min_0),atoi(Sat_start_hour) );
		    fprintf(cron_conf,"%d %d * * 6 nvram_set SchedulerRadioOn 1\n", (atoi(Sat_start_min)? min_30:min_0),atoi(Sat_start_hour) );
		    
		    if (!strcmp(Sat_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 0 iwpriv ra0 set RadioOn=0\n");
		    	fprintf(cron_conf,"0 0 * * 0 nvram_set SchedulerRadioOn 0\n");
		    }else{
		    	fprintf(cron_conf,"%d %d * * 6 iwpriv ra0 set RadioOn=0\n", (atoi(Sat_end_min)? min_30:min_0),atoi(Sat_end_hour) );
		    	fprintf(cron_conf,"%d %d * * 6 nvram_set SchedulerRadioOn 0\n", (atoi(Sat_end_min)? min_30:min_0),atoi(Sat_end_hour) );		    
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Sat %d%s %d%s 1 >> /var/spool/cron/crontabs/scheduler", atoi(Sat_start_hour),(atoi(Sat_start_min)? "30":"00"), atoi(Sat_end_hour),(atoi(Sat_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Saturday %d:%s - %d:%s Radio ON",atoi(scheduling_enable)? "enable":"disable", atoi(Sat_start_hour),(atoi(Sat_start_min)? "30":"00"), atoi(Sat_end_hour),(atoi(Sat_end_min)? "30":"00"));

		}else{
		    fprintf(cron_conf,"%d %d * * 6 iwpriv ra0 set RadioOn=0\n", (atoi(Sat_start_min)? min_30:min_0),atoi(Sat_start_hour) );
		    fprintf(cron_conf,"%d %d * * 6 nvram_set SchedulerRadioOn 0\n", (atoi(Sat_start_min)? min_30:min_0),atoi(Sat_start_hour) );
		    
		    if (!strcmp(Sat_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 0 iwpriv ra0 set RadioOn=1\n");
		    	fprintf(cron_conf,"0 0 * * 0 nvram_set SchedulerRadioOn 1\n");
		    }else{		    
		    fprintf(cron_conf,"%d %d * * 6 iwpriv ra0 set RadioOn=1\n", (atoi(Sat_end_min)? min_30:min_0),atoi(Sat_end_hour) );
		    fprintf(cron_conf,"%d %d * * 6 nvram_set SchedulerRadioOn 1\n", (atoi(Sat_end_min)? min_30:min_0),atoi(Sat_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Sat %d%s %d%s 0 >> /var/spool/cron/crontabs/scheduler", atoi(Sat_start_hour),(atoi(Sat_start_min)? "30":"00"), atoi(Sat_end_hour),(atoi(Sat_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Saturday %d:%s - %d:%s Radio OFF",atoi(scheduling_enable)? "enable":"disable", atoi(Sat_start_hour),(atoi(Sat_start_min)? "30":"00"), atoi(Sat_end_hour),(atoi(Sat_end_min)? "30":"00"));
	    	}
	    }
	}

	if (!strcmp(Sun, "1")){
	    if ( (strcmp(Sun_start_hour,Sun_end_hour)) || (strcmp(Sun_start_min,Sun_end_min))){
	    	if (strcmp(Sun_ON, "1")){
		    fprintf(cron_conf,"%d %d * * 0 iwpriv ra0 set RadioOn=1\n", (atoi(Sun_start_min)? min_30:min_0),atoi(Sun_start_hour) );
		    fprintf(cron_conf,"%d %d * * 0 nvram_set SchedulerRadioOn 1\n", (atoi(Sun_start_min)? min_30:min_0),atoi(Sun_start_hour) );
		    
		    if (!strcmp(Sun_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 1 iwpriv ra0 set RadioOn=0\n");
		    	fprintf(cron_conf,"0 0 * * 1 nvram_set SchedulerRadioOn 0\n");
		    }else{		    
		    fprintf(cron_conf,"%d %d * * 0 iwpriv ra0 set RadioOn=0\n", (atoi(Sun_end_min)? min_30:min_0),atoi(Sun_end_hour) );
		    fprintf(cron_conf,"%d %d * * 0 nvram_set SchedulerRadioOn 0\n", (atoi(Sun_end_min)? min_30:min_0),atoi(Sun_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Sun %d%s %d%s 1 >> /var/spool/cron/crontabs/scheduler", atoi(Sun_start_hour),(atoi(Sun_start_min)? "30":"00"), atoi(Sun_end_hour),(atoi(Sun_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Sunday %d:%s - %d:%s Radio ON",atoi(scheduling_enable)? "enable":"disable", atoi(Sun_start_hour),(atoi(Sun_start_min)? "30":"00"), atoi(Sun_end_hour),(atoi(Sun_end_min)? "30":"00"));
		}else{
		    fprintf(cron_conf,"%d %d * * 0 iwpriv ra0 set RadioOn=0\n", (atoi(Sun_start_min)? min_30:min_0),atoi(Sun_start_hour) );
		    fprintf(cron_conf,"%d %d * * 0 nvram_set SchedulerRadioOn 0\n", (atoi(Sun_start_min)? min_30:min_0),atoi(Sun_start_hour) );
		    
		    if (!strcmp(Sun_end_hour,"24")){
		    	fprintf(cron_conf,"0 0 * * 0 iwpriv ra0 set RadioOn=1\n");
		    	fprintf(cron_conf,"0 0 * * 0 nvram_set SchedulerRadioOn 1\n");
		    }else{		    
		    	fprintf(cron_conf,"%d %d * * 6 iwpriv ra0 set RadioOn=1\n", (atoi(Sun_end_min)? min_30:min_0),atoi(Sun_end_hour) );
		    	fprintf(cron_conf,"%d %d * * 6 nvram_set SchedulerRadioOn 1\n", (atoi(Sun_end_min)? min_30:min_0),atoi(Sun_end_hour) );
		    }
// Tommy, create readable file for scheduling , 2009/3/12 09:14
	doSystem("echo  Sun %d%s %d%s 0 >> /var/spool/cron/crontabs/scheduler", atoi(Sun_start_hour),(atoi(Sun_start_min)? "30":"00"), atoi(Sun_end_hour),(atoi(Sun_end_min)? "30":"00") );
// Tommy, Add syslog , 2009/10/21 04:57
	syslog(LOG_NOTICE, "Scheduling %s, Sunday %d:%s - %d:%s Radio OFF",atoi(scheduling_enable)? "enable":"disable", atoi(Sun_start_hour),(atoi(Sun_start_min)? "30":"00"), atoi(Sun_end_hour),(atoi(Sun_end_min)? "30":"00"));

	    	}
	    }
	}

	fclose(cron_conf);

	doSystem("cat /var/spool/cron/crontabs/temp.dat > /var/spool/cron/crontabs/%s",username);
	doSystem("rm -f /var/spool/cron/crontabs/temp.dat");
#endif
}

/*
 * description: setup internet according to nvram configurations
 *              (assume that nvram_init has already been called)
 *              return value: 0 = successful, -1 = failed
 */
int initInternet(void)
{
#if 0
#ifndef CONFIG_RALINK_RT2880
	char *auth_mode = nvram_bufget(RT2860_NVRAM, "AuthMode");
#endif
#if defined CONFIG_RT2860V2_STA || defined CONFIG_RT2860V2_STA_MODULE
	char *opmode;
#endif

    STEVEPRINT("\n\n============================================\n");
    STEVEPRINT("	Steve test => internet.c: %s\n",__FUNCTION__);
	doSystem("internet.sh");

	//RL added for assure quota 2009/03/17
#ifdef CONFIG_RALINK_GPIO
	goaNotifyGpioForConntrack();
#endif

	//automatically connect to AP according to the active profile
#if defined CONFIG_RT2860V2_STA || defined CONFIG_RT2860V2_STA_MODULE
	opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	if (!strcmp(opmode, "2") || (!strcmp(opmode, "0") &&
				!strcmp("1", nvram_get(RT2860_NVRAM, "ethConvert")))) {
		if (-1 != initStaProfile())
			initStaConnection();
	}
#endif

#ifndef CONFIG_RALINK_RT2880
//	if (!strcmp(auth_mode, "Disable") || !strcmp(auth_mode, "OPEN"))
//		ledAlways(13, LED_OFF); //turn off security LED (gpio 13)
//	else
//		ledAlways(13, LED_ON); //turn on security LED (gpio 13)
#endif

#if defined (CONFIG_RT2860V2_AP) || defined (CONFIG_RT2860V2_AP_MODULE)
	restart8021XDaemon(RT2860_NVRAM);	// in wireless.c
#endif
#if defined (CONFIG_RT2880v2_INIC_MII) || defined (CONFIG_RT2880v2_INIC_MII_MODULE) || \
	defined (CONFIG_RT2880v2_INIC_PCI) || defined (CONFIG_RT2880v2_INIC_PCI_MODULE)
	restart8021XDaemon(RTINIC_NVRAM);	// in wireless.c
#endif
#if defined (CONFIG_RT2561_AP) || defined (CONFIG_RT2561_AP_MODULE)
	restart8021XDaemon(RT2561_NVRAM);	// in wireless.c
#endif

#ifdef CONFIG_RT2860V2_AP_ANTENNA_DIVERSITY
	AntennaDiversityInit();
#endif

// Tommy, WLAN Scheduling , 2008/12/9 05:13¤U¤È
	/* WLAN Scheduling */
	char *scheduling_enable;
	scheduling_enable = nvram_bufget(RT2860_NVRAM, "WiFiScheduleEnable");
	char *Radio_OFF;
	Radio_OFF = nvram_bufget(RT2860_NVRAM, "RadioOff");

// Tommy, Zyxel bug:No.32 , 2009/2/21 09:38
	create_crond_file();
#if 0
	if ( (!strcmp(Radio_OFF, "0")) && (!strcmp(scheduling_enable, "1")) ){
		// Execute "cron" daemon
		doSystem("killall crond");
		doSystem("crond");
	   }
#else
// Tommy, 2009/2/23 02:44
	char *oper_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	if ( (strcmp(oper_mode, "2")) && (strcmp(oper_mode, "6")) ){
		if (!strcmp(Radio_OFF, "0")){
			if (!strcmp(scheduling_enable, "1")){
			// Execute "cron" daemon
				doSystem("killall crond");
			doSystem("crond");
			// Tommy, 2009/3/13 09:54
printf("----> check_scheduler\n");
//			check_scheduler();
printf("<---- check_scheduler\n");
			}else{
				doSystem("killall crond");
				doSystem("iwpriv ra0 set RadioOn=1");
				doSystem("nvram_set SchedulerRadioOn 1");
			}
		}else{
			doSystem("killall crond");
			doSystem("iwpriv ra0 set RadioOn=0");
		}
	}
// Tommy, WISP mode add Radio on/off function , 2009/3/30 01:20	
	if (!strcmp(oper_mode, "6")){
		if (!strcmp(Radio_OFF, "1")){
#if 1//Arthur Chow 2009-03-30
		char *wan_dhcp;
		wan_dhcp = nvram_bufget(RT2860_NVRAM, "wanConnectionMode");
		//char *pptp_mode=nvram_get(RT2860_NVRAM, "wan_pptp_mode");
                //char *l2tp_mode=nvram_get(RT2860_NVRAM, "wan_l2tp_mode");
		char *wan_pptp_wanip_Mode=nvram_get(RT2860_NVRAM, "wan_pptp_wanip_mode");
		char *wan_l2tp_wanip_Mode=nvram_get(RT2860_NVRAM, "wan_l2tp_wanip_mode");
		char *wan_pppoe_wanip_Mode=nvram_get(RT2860_NVRAM, "wan_pppoe_wanip_mode");
		
		if (!strcmp(wan_dhcp, "DHCP")){
			system("ifconfig ra0 0.0.0.0");		
			system("rm -f /var/physical_wanIf_gw");		
			system("killall udhcpc");		
		}
		//else if((!strcmp(wan_dhcp, "PPTP")) && (!strcmp(pptp_mode, "1")))
		else if((!strcmp(wan_dhcp, "PPTP")) && (!strcmp(wan_pptp_wanip_Mode, "1")))
		{
			char *pptp_Mode=nvram_get(RT2860_NVRAM, "wan_pptp_mode");
			
			system("ifconfig ra0 0.0.0.0");
			system("rm -f /var/physical_wanIf_gw");
                        if(!strcmp(pptp_Mode, "1")) //dhcp
                        {
                        system("killall udhcpc");
	        }
	        }
	        //else if((!strcmp(wan_dhcp, "L2TP")) && (!strcmp(l2tp_mode, "1")))
	        else if((!strcmp(wan_dhcp, "L2TP")) && (!strcmp(wan_l2tp_wanip_Mode, "1")))
		{
			char *l2tp_Mode=nvram_get(RT2860_NVRAM, "wan_l2tp_mode");
			
			system("ifconfig ra0 0.0.0.0");
		        system("rm -f /var/physical_wanIf_gw");
		        if(!strcmp(l2tp_Mode, "1")) //dhcp
		        {
		        system("killall -q udhcpc");
		        }
		        system("killall -q pppd");
		        system("killall -q l2tpd");
	        }
	        //else if (!strcmp(wan_dhcp, "PPPOE"))
	        else if((!strcmp(wan_dhcp, "PPPOE")) && (!strcmp(wan_pppoe_wanip_Mode, "1")))
	        {
	        	system("ifconfig ppp0 0.0.0.0");
	        	system("rm -f /var/physical_wanIf_gw");
	                system("killall -q pppd");
	        }
#endif		
			doSystem("ifconfig ra0 down");
		}			
	}
	
#endif

// Tommy, deal WPS led, 2009/4/6 03:03
//#WPS led
	char *wsc_enable = nvram_bufget(RT2860_NVRAM, "WscModeOption");	
	if (strcmp(oper_mode, "6")){
		if(!strcmp(wsc_enable, "0")){
			//doSystem("gpio l 14 0 4000 0 1 4000"); //trun off WPS led
			doSystem("gpioctl led 14 0 4000 0 1 4000"); //trun off WPS led
		}else{
			//doSystem("gpio l 14 4000 0 1 0 4000"); //trun on WPS led
			doSystem("gpioctl led 14 4000 0 1 0 4000"); //trun on WPS led
		}
	}
//#WPS led

    STEVEPRINT("	Steve test => internet.c: firewall_init()\n");
//	firewall_init();
	 STEVEPRINT("	Steve test => internet.c: management_init()\n");
//	management_init();
	 STEVEPRINT("	Steve test => internet.c: RoutingInit()\n");
	RoutingInit();
#ifdef CONFIG_RALINKAPP_SWQOS
 STEVEPRINT("	Steve test => internet.c: QoSInit(0)\n");
	QoSInit(0);
#endif

//RL 2009/4/12 01:28pm, modified some parameter in GW and WISP mode
if ( !strcmp(oper_mode, "0") || !strcmp(oper_mode, "6") ){
#ifdef CONFIG_RT2880_DRAM_32M
	printf("RL modified timeout parameter in 32M.\n");
	
#if 1 // +++ Tommy patch : for Windows 7 Logo Xbox 2Nics test
	system("echo 180 > /proc/sys/net/netfilter/nf_conntrack_udp_timeout" );
	system("echo 180 > /proc/sys/net/netfilter/nf_conntrack_tcp_timeout_established");
	system("echo 60 > /proc/sys/net/ipv4/tcp_fin_timeout");
	system("echo 7200 > /proc/sys/net/ipv4/tcp_keepalive_time");
#else // --- for RL BT test 
	system("echo 20 > /proc/sys/net/netfilter/nf_conntrack_udp_timeout" );
	system("echo 600 > /proc/sys/net/netfilter/nf_conntrack_tcp_timeout_established");
	system("echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout");
	system("echo 1400 > /proc/sys/net/ipv4/tcp_keepalive_time");
#endif // +++ Tommy patch : for Windows 7 Logo Xbox 2Nics test

	system("echo 8192 > /proc/sys/net/netfilter/nf_conntrack_max");
#else
	printf("RL modified timeout parameter in xxM.\n");
	system("echo 30 > /proc/sys/net/netfilter/nf_conntrack_udp_timeout " );
#endif

	//RL added to reduced timeout for udpstream. 2009/4/8 05:03pm	
	system("echo 120 > /proc/sys/net/netfilter/nf_conntrack_udp_timeout_stream " );

}
       //aron add to restart upnp  2009.04.15
       system("restart_upnp.sh");

	return 0;
#endif
}

static void getMyMAC(webs_t wp, char_t *path, char_t *query)
{
#if 0
	char myMAC[32];

	arplookup(wp->ipaddr, myMAC);
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("%s"), myMAC);
	websDone(wp, 200);
#endif
}

//aron patch for giga
static int
_save_lan_static(webs_t wp, char *value)
{
    char tmp[TMP_LEN];
    char *ip, *mask;
    int idx = atoi(value), change = 0;

  if(idx==0) //Normally, the nth for "0"
  {
    /* IP */
    snprintf(tmp, sizeof(tmp), "lanIp");
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "lanNetmask");
    mask = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* IP */
    snprintf(tmp, sizeof(tmp), "lanIp_%d", idx);
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "lanNetmask_%d", idx);
    mask = websGetVar(wp, tmp, "");
  }
    
    //Translate AboCom's netmask to AXIM's nvram format
    mask = mask2prefix(mask);

    //preaction ??
    nvram_set("lan0_ipaddr", ip);
    nvram_set("lan0_mask", mask);
    ezplib_replace_attr("lan_static_rule", idx, "ipaddr", ip);
    ezplib_replace_attr("lan_static_rule", idx, "mask", mask);
    ezplib_replace_attr("lan_static_rule", idx, "ipaddr_normal", ip);
    ezplib_replace_attr("lan_static_rule", idx, "mask_normal", mask);
    change = 1;
    
    return change;
}

//aron patch for giga
static int
_save_lan_static_ur(webs_t wp, char *value)
{
    char tmp[TMP_LEN];
    char *ip, *mask;
    int idx = atoi(value), change = 0;

  if(idx==0) //Normally, the nth for "0"
  {
    /* IP */
    snprintf(tmp, sizeof(tmp), "lanIp");
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "lanNetmask");
    mask = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* IP */
    snprintf(tmp, sizeof(tmp), "lanIp_%d", idx);
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "lanNetmask_%d", idx);
    mask = websGetVar(wp, tmp, "");
  }
    
    //Translate AboCom's netmask to AXIM's nvram format
    mask = mask2prefix(mask);

    //preaction ??
    nvram_set("lan0_ipaddr", ip);
    nvram_set("lan0_mask", mask);
    ezplib_replace_attr("lan_static_rule", idx, "ipaddr", ip);
    ezplib_replace_attr("lan_static_rule", idx, "mask", mask);
    ezplib_replace_attr("lan_static_rule", idx, "ipaddr_ap", ip);
    ezplib_replace_attr("lan_static_rule", idx, "mask_ap", mask);
    change = 1;
    
    return change;
}

//aron patch for giga
static int
_save_lan_static_apMode(webs_t wp, char *value)
{
    char tmp[TMP_LEN];
    char *ip, *mask, *gateway;
    int idx = atoi(value), change = 0;

  if(idx==0) //Normally, the nth for "0"
  {
    /* IP */
    snprintf(tmp, sizeof(tmp), "lanIpApMode");
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "lanNetmaskApMode");
    mask = websGetVar(wp, tmp, "");
    
    /* Gateway */
    snprintf(tmp, sizeof(tmp), "lanGatewayApMode");
    gateway = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* IP */
    snprintf(tmp, sizeof(tmp), "lanIpApMode_%d", idx);
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "lanNetmaskApMode_%d", idx);
    mask = websGetVar(wp, tmp, "");
    
    /* Gateway */
    snprintf(tmp, sizeof(tmp), "lanGatewayApMode_%d", idx);
    gateway = websGetVar(wp, tmp, "");
  }
    
    //Translate AboCom's netmask to AXIM's nvram format
    mask = mask2prefix(mask);

    //preaction ??
    nvram_set("lan0_ipaddr", ip);
    nvram_set("lan0_mask", mask);
    ezplib_replace_attr("lan_static_rule", idx, "ipaddr", ip);
    ezplib_replace_attr("lan_static_rule", idx, "mask", mask);
    ezplib_replace_attr("lan_static_rule", idx, "ipaddr_ap", ip);
    ezplib_replace_attr("lan_static_rule", idx, "mask_ap", mask);
    //nvram_set("lan0_gateway", gateway);
    ezplib_replace_attr("lan_static_rule", idx, "gateway", gateway);
    change = 1;
    
    return change;
}

//aron patch for giga
static int
//_save_lan_main(webs_t wp, char *value, struct variable *v, struct service *s,
//        int64_t *map)
_save_lan_main(webs_t wp, char *value)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *mtu, *stp, *weight;
    int idx = atoi(value), len, change = 0;

  if(idx==0) //Normally, the nth for "0"
  {
    /* MTU */
    snprintf(tmp, sizeof(tmp), "lanMtu");
    mtu = websGetVar(wp, tmp, "");

    /* STP */
    snprintf(tmp, sizeof(tmp), "lanStp");
    stp = websGetVar(wp, tmp, "");

    /* Weight */
    snprintf(tmp, sizeof(tmp), "lanWeight");
    weight = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* MTU */
    snprintf(tmp, sizeof(tmp), "lanMtu_%d", idx);
    mtu = websGetVar(wp, tmp, "");

    /* STP */
    snprintf(tmp, sizeof(tmp), "lanStp_%d", idx);
    stp = websGetVar(wp, tmp, "");

    /* Weight */
    snprintf(tmp, sizeof(tmp), "lanWeight_%d", idx);
    weight = websGetVar(wp, tmp, "");
  }
     /* Enable is always true. */
    len = snprintf(tmp, TMP_LEN, "LAN%d^1^%s^%s^%s", idx+1, mtu, stp, weight);
    if (len >= TMP_LEN) {
        return FALSE;
    }
    
    //ezplib_get_rule("lan_main_rule", idx, buf, TMP_LEN);
    //if (strcmp(buf, tmp)) { //if new UI and current nvram value is not equal
        //snprintf(buf, TMP_LEN, "NUM=%d", idx);
        //config_preaction(map, v, s, buf, ""); //because system will reboot, do we need any preaction ??
        ezplib_replace_rule("lan_main_rule", idx, tmp);
        change = 1;
    //}

    return change;
}
void readaddr(char *Buf1, char* Buf2)
{
	while(*Buf1 != 0x20)
	{
		*Buf2++ = *Buf1++;
	}

}
//get local link address
static int getLinkLocalAddr(int eid, webs_t wp, int argc, char_t **argv)
{
	/*
	FILE *fp;
	char *list = (char*)malloc(512 * sizeof (char));
	unsigned char addr[100], tmp[100], tmp1[100];
	int i;	
	system("ifconfig br0 > /etc/br0status.dat");

	if(NULL == (fp = fopen("/etc/br0status.dat" , "r")))
	{
		websWrite(wp, T("0"));
	}
	else {		
		fgets(list, 512, fp);  //1st line
		fgets(list, 512, fp);  //2nd line
		
		memset(addr, 0x00, sizeof(addr));			
		fgets(list, 512, fp);  //3rd line
		snprintf(addr, 39, "%s", list+22);			
		readaddr(addr, &tmp[0]);
		//websWrite(wp, T("%s"), tmp);
		
		fgets(list, 512, fp);  //4th line
		snprintf(addr, 6, "%s", list+10);
		readaddr(addr, &tmp1[0]);
		if(!strcmp(&tmp1[0], "inet6"))
		{
		     snprintf(addr, 39, "%s", list+22);			
		     readaddr(addr, &tmp1[0]);
		     websWrite(wp, T("%s"), tmp1);
	        }   
	        else
	             websWrite(wp, T("%s"), tmp);
	            
	            	
	}
	free(list);
	fclose(fp);
	return 0;
	*/
	FILE *fp;
	char link_local_addr[100];
	system("ifconfig br0 | grep 'Scope:Link' | sed 's/^.*inet6 addr: //g' | sed 's/ Scope.*$//g' > /etc/br0status");
	
	if((fp = fopen("/etc/br0status", "r")) != NULL)
	{
		fscanf(fp, "%s", link_local_addr);
		fclose(fp);
		websWrite(wp, T("%s"), link_local_addr);
        }
        return 1;      	
}
static void IPV6(webs_t wp, char_t *path, char_t *query)
{
return setIPV6(  wp, path, query);
}

/* goform/IPV6 */ //----- nico added
void setIPV6(webs_t wp, char_t *path, char_t *query)
{
	char_t *lanIPv6_addr_static, *lanIPv6_addr_pppoe, *lanIPv6_addr_dhcp, *lanIPv6_Netsize, *wanIPv6_addr, *wanIPv6_Netsize, *wanIPv6_gateway, *wanIPv6_dns1, *wanIPv6_dns2, *dhcpIPv6_capable, *dhcpIPv6_LifeTime ;  
	char_t *pppoeIPv6_user, *pppoeIPv6_pass, *pppoeIPv6_service, *pppoeIPv6_idle, *pppoeIPv6_mtu, *pppoeIPv6_addr, *pppoeIPv6_dns1, *pppoeIPv6_dns2, *dns1Type, *dns2Type, *IPv6Type, *dhcpIPv6_addrprefix0, *dhcpIPv6_addrprefix00, *dhcpIPv6_addrstart0, *dhcpIPv6_addrend0;
	char_t *dhcpIPv6_addrprefix1, *dhcpIPv6_addrprefix11, *dhcpIPv6_addrstart1, *dhcpIPv6_addrend1, *dhcpIPv6_addrprefix2, *dhcpIPv6_addrprefix22, *dhcpIPv6_addrstart2, *dhcpIPv6_addrend2;
	char 	*Ipv6DhcpEnalbe = malloc(sizeof(char));
//	char    *Ipv6DhcpType = malloc(sizeof(char)*10);
	char   *DnsType, *dhcp_ipv6dns1, *dhcp_ipv6dns2, *AddrType;
    	char    Ipv6DhcpType[TMP_LEN];
    	char    Ipv6PppoeipType[TMP_LEN];
    	char    tmp[TMP_LEN];
	char   *proto, *wan_dhcp_fixedip;
	
	//Steve
	char_t *wandhcpv6type,*lanIPv6_addr_dhcp_suffix,*lanIPv6_addr_static_suffix;
	char    wanIpv6DhcpType[TMP_LEN];
		
	wan_dhcp_fixedip = websGetVar(wp, "connectionType", "");
                      
	//Translate AboCom' UI value to conform with AXIM's nvram format at first
	if (!strcmp(wan_dhcp_fixedip, "0")) 
		proto = "static";
	else if	(!strcmp(wan_dhcp_fixedip, "1"))  
		proto = "dhcp";
//	else if (!strcmp(wan_dhcp_fixedip, "2")) 
//		proto = "pppoe";		
        else if	(!strcmp(wan_dhcp_fixedip, "3")) //aron add  2010.12.01	
                proto = "link-local";

	nvram_set("wan0_protov6", proto);
                          
printf("proto===%s\n",proto); 
 
//nvram_safe_get();                         	

	//ipv6 wan static
	if (!strcmp(proto, "static")){
		wanIPv6_addr = websGetVar(wp, T("wanIPv6_addr"), T(""));
		wanIPv6_Netsize = websGetVar(wp, T("wanIPv6_Netsize"), T(""));
		wanIPv6_gateway = websGetVar(wp, T("wanIPv6_gateway"), T(""));
		wanIPv6_dns1 = websGetVar(wp, T("wanIPv6_dns1"), T(""));	
		wanIPv6_dns2 = websGetVar(wp, T("wanIPv6_dns2"), T(""));

        //ipv6 lan
        lanIPv6_addr_static = websGetVar(wp, T("lanIPv6_addr_static"), T(""));
        lanIPv6_addr_static_suffix = websGetVar(wp, T("lanIPv6_addr_static_suf"), T(""));

	    IPv6Type = websGetVar(wp, T("IPv6Type"), T(""));
/*		dhcpIPv6_addrprefix0 = websGetVar(wp, T("dhcpIPv6_addrprefix0"), T(""));
		dhcpIPv6_addrprefix00 = websGetVar(wp, T("dhcpIPv6_addrprefix00"), T(""));	*/
        dhcpIPv6_addrstart0 = websGetVar(wp, T("dhcpIPv6_addrstart0"), T(""));	
        dhcpIPv6_addrend0 = websGetVar(wp, T("dhcpIPv6_addrend0"), T(""));

		ezplib_replace_attr("wan_staticv6_rule", 0, "ipaddr", wanIPv6_addr);
		ezplib_replace_attr("wan_staticv6_rule", 0, "length", wanIPv6_Netsize);
		ezplib_replace_attr("wan_staticv6_rule", 0, "gateway", wanIPv6_gateway);
		ezplib_replace_attr("wan_staticv6_rule", 0, "dns1", wanIPv6_dns1);
		ezplib_replace_attr("wan_staticv6_rule", 0, "dns2", wanIPv6_dns2);
//printf("lanIPv6_addr_static===%s\n",lanIPv6_addr_static);		
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "static_prefix", lanIPv6_addr_static); //steve change 2011/03/09
		//ezplib_replace_attr("lan_dhcpsv6_rule", 0, "dhcp_prefix", lanIPv6_addr_static); //aron change to "dhcp_prefix" 2010.11.19
		ezplib_replace_attr("lan_staticv6_rule", 0, "static_prefix", lanIPv6_addr_static); //steve add 2011/03/10
		ezplib_replace_attr("lan_staticv6_rule", 0, "static_suffix", lanIPv6_addr_static_suffix); //steve add 2011/03/10
		
		
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "type", IPv6Type);
/*		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "static_prefix", dhcpIPv6_addrprefix0);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "static_prefix", dhcpIPv6_addrprefix00);*/
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "start", dhcpIPv6_addrstart0);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "end", dhcpIPv6_addrend0);


	//ipv6 dhcp
    dhcpIPv6_capable = websGetVar(wp, T("dhcpIPv6_capable"), T(""));	
	dhcpIPv6_LifeTime = websGetVar(wp, T("dhcpIPv6_LifeTime"), T(""));
			
	if (!strncmp(dhcpIPv6_capable, "1", 2)){
		sprintf(Ipv6DhcpEnalbe, "%s", "1");
	}else{
		sprintf(Ipv6DhcpEnalbe, "%s", "0");
	}
	
	ezplib_replace_attr("lan_dhcpsv6_rule", 0, "enable", Ipv6DhcpEnalbe);
	ezplib_replace_attr("lan_dhcpsv6_rule", 0, "lifetime", dhcpIPv6_LifeTime);

//printf("ifupv6 wan0\n");
	system("ifupv6 wan0");	
	}
	//ipv6 PPPoE
	else if (!strcmp(proto, "pppoe")){
        	AddrType = websGetVar(wp, T("AddrType"), T(""));
        	pppoeIPv6_addr = websGetVar(wp, T("pppoeIPv6_addr"), T(""));		
		pppoeIPv6_user = websGetVar(wp, T("pppoeIPv6_user"), T(""));
		pppoeIPv6_pass = websGetVar(wp, T("pppoeIPv6_pass"), T(""));
		pppoeIPv6_service = websGetVar(wp, T("pppoeIPv6_service"), T(""));
		pppoeIPv6_idle = websGetVar(wp, T("pppoeIPv6_idle"), T(""));
		pppoeIPv6_mtu = websGetVar(wp, T("pppoeIPv6_mtu"), T(""));
		pppoeIPv6_dns1 = websGetVar(wp, T("pppoeIPv6_dns1"), T(""));
		pppoeIPv6_dns2 = websGetVar(wp, T("pppoeIPv6_dns2"), T(""));		
		dns1Type = websGetVar(wp, T("dns1Type"), T(""));			
		dns2Type = websGetVar(wp, T("dns2Type"), T(""));

        //ipv6 lan
        lanIPv6_addr_pppoe = websGetVar(wp, T("lanIPv6_addr_pppoe"), T(""));
/*		
	    IPv6Type = websGetVar(wp, T("IPv6Type"), T(""));		
		dhcpIPv6_addrprefix1 = websGetVar(wp, T("dhcpIPv6_addrprefix1"), T(""));
		dhcpIPv6_addrprefix11 = websGetVar(wp, T("dhcpIPv6_addrprefix11"), T(""));	
        dhcpIPv6_addrstart1 = websGetVar(wp, T("dhcpIPv6_addrstart1"), T(""));	
        dhcpIPv6_addrend1 = websGetVar(wp, T("dhcpIPv6_addrend1"), T(""));
*/
		if (!strncmp(AddrType, "0", 2)){
			sprintf(Ipv6PppoeipType, "%s", "isp");
		}else if(!strncmp(AddrType, "1", 2)){
			sprintf(Ipv6PppoeipType, "%s", "custom");
		}	

		ezplib_replace_attr("wan_pppoev6_rule", 0, "pppiptype", Ipv6PppoeipType);
		ezplib_replace_attr("wan_pppoev6_rule", 0, "custom_ipaddr", pppoeIPv6_addr);
		ezplib_replace_attr("wan_pppoev6_rule", 0, "username", pppoeIPv6_user);
		ezplib_replace_attr("wan_pppoev6_rule", 0, "passwd", pppoeIPv6_pass);
		ezplib_replace_attr("wan_pppoev6_rule", 0, "servicename", pppoeIPv6_service);
		ezplib_replace_attr("wan_pppoev6_rule", 0, "idletime", pppoeIPv6_idle);
		ezplib_replace_attr("wan_pppoev6_rule", 0, "mtu", pppoeIPv6_mtu);
		
		if(!strcmp(dns1Type, "ISP"))
			dns1Type = "ispdns";
		else if(!strcmp(dns1Type, "USER"))
			dns1Type = "custom";
		else if(!strcmp(dns1Type, "NONE"))
			dns1Type = "none";
    
		if(!strcmp(dns2Type, "ISP"))
			dns2Type = "ispdns";
		else if(!strcmp(dns2Type, "USER"))
			dns2Type = "custom";
		else if(!strcmp(dns2Type, "NONE"))
			dns2Type = "none";
			
		ezplib_replace_attr("wan_dnsv6_rule", 0, "dnstype1", dns1Type);
		ezplib_replace_attr("wan_dnsv6_rule", 0, "dnstype2", dns2Type);        
		ezplib_replace_attr("wan_dnsv6_rule", 0, "dns1", pppoeIPv6_dns1);
		ezplib_replace_attr("wan_dnsv6_rule", 0, "dns2", pppoeIPv6_dns2);

//printf("lanIPv6_addr_pppoe===%s\n",lanIPv6_addr_pppoe);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "pppoe_prefix", lanIPv6_addr_pppoe);
		
/*		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "type", IPv6Type);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "pppoe_prefix", dhcpIPv6_addrprefix1);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "pppoe_prefix", dhcpIPv6_addrprefix11);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "start", dhcpIPv6_addrstart1);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "end", dhcpIPv6_addrend1);
*/

	//ipv6 dhcp
        dhcpIPv6_capable = websGetVar(wp, T("dhcpIPv6_capable"), T(""));	
	dhcpIPv6_LifeTime = websGetVar(wp, T("dhcpIPv6_LifeTime"), T(""));
			
	if (!strncmp(dhcpIPv6_capable, "1", 2)){
		sprintf(Ipv6DhcpEnalbe, "%s", "1");
	}else{
		sprintf(Ipv6DhcpEnalbe, "%s", "0");
	}
	
	ezplib_replace_attr("lan_dhcpsv6_rule", 0, "enable", Ipv6DhcpEnalbe);
	ezplib_replace_attr("lan_dhcpsv6_rule", 0, "lifetime", dhcpIPv6_LifeTime);
	
	
	}		
	//ipv6 DHCP dns
	else if (!strcmp(proto, "dhcp")){
        DnsType = websGetVar(wp, T("DnsType"), T(""));	
		dhcp_ipv6dns1 = websGetVar(wp, T("dhcpIPv6_dns1"), T(""));
		dhcp_ipv6dns2 = websGetVar(wp, T("dhcpIPv6_dns2"), T(""));
		
		//wan dhcp type
		wandhcpv6type = websGetVar(wp, T("wandhcpv6type"), T(""));
		if (!strncmp(wandhcpv6type, "0", 2)){
			sprintf(wanIpv6DhcpType, "%s", "stateless");
		}else if(!strncmp(wandhcpv6type, "1", 2)){
			sprintf(wanIpv6DhcpType, "%s", "stateful");
		}
		ezplib_replace_attr("wan_dhcpv6_rule", 0, "type", wanIpv6DhcpType);

        //ipv6 lan
        lanIPv6_addr_dhcp = websGetVar(wp, T("lanIPv6_addr_dhcp"), T(""));
        lanIPv6_addr_dhcp_suffix = websGetVar(wp, T("lanIPv6_addr_dhcp_suf"), T(""));
		
        IPv6Type = websGetVar(wp, T("IPv6Type"), T(""));
/*		dhcpIPv6_addrprefix2 = websGetVar(wp, T("dhcpIPv6_addrprefix2"), T(""));
		dhcpIPv6_addrprefix22 = websGetVar(wp, T("dhcpIPv6_addrprefix22"), T(""));	*/
        dhcpIPv6_addrstart2 = websGetVar(wp, T("dhcpIPv6_addrstart2"), T(""));	
        dhcpIPv6_addrend2 = websGetVar(wp, T("dhcpIPv6_addrend2"), T(""));	

		if (!strncmp(DnsType, "0", 2)){
			sprintf(Ipv6DhcpType, "%s", "auto");
		}else if(!strncmp(DnsType, "1", 2)){
			sprintf(Ipv6DhcpType, "%s", "custom");
		}
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "dnstype", Ipv6DhcpType);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "dnsaddr1", dhcp_ipv6dns1);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "dnsaddr2", dhcp_ipv6dns2);	

//printf("lanIPv6_addr_dhcp===%s\n",lanIPv6_addr_dhcp);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "dhcp_prefix", lanIPv6_addr_dhcp);
		ezplib_replace_attr("lan_staticv6_rule", 0, "dhcp_prefix", lanIPv6_addr_dhcp); //steve add 2011/03/10
		ezplib_replace_attr("lan_staticv6_rule", 0, "dhcp_suffix", lanIPv6_addr_dhcp_suffix); //steve add 2011/03/10
	
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "type", IPv6Type);
/*		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "dhcp_prefix", dhcpIPv6_addrprefix2);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "dhcp_prefix", dhcpIPv6_addrprefix22);*/
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "start", dhcpIPv6_addrstart2);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "end", dhcpIPv6_addrend2);


		//ipv6 dhcp
    	dhcpIPv6_capable = websGetVar(wp, T("dhcpIPv6_capable"), T(""));	
		dhcpIPv6_LifeTime = websGetVar(wp, T("dhcpIPv6_LifeTime"), T(""));
			
		if (!strncmp(dhcpIPv6_capable, "1", 2)){
			sprintf(Ipv6DhcpEnalbe, "%s", "1");
		}else{
			sprintf(Ipv6DhcpEnalbe, "%s", "0");
		}
	
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "enable", Ipv6DhcpEnalbe);
		ezplib_replace_attr("lan_dhcpsv6_rule", 0, "lifetime", dhcpIPv6_LifeTime);	
	}
	
        //ipv6 lan
//      lanIPv6_addr = websGetVar(wp, T("lanIPv6_addr"), T(""));	
//	lanIPv6_Netsize = websGetVar(wp, T("lanIPv6_Netsize"), T(""));
	
//	ezplib_replace_attr("lan_dhcpsv6_rule", 0, "ipaddr", lanIPv6_addr);
//	ezplib_replace_attr("lan_staticv6_rule", 0, "mask", lanIPv6_Netsize);
//printf("/etc/rc.common /etc/init.d/radvd.init restart\n");
	//steve
	//system("/etc/rc.common /etc/init.d/radvd.init restart");
	system("/etc/rc.common /etc/init.d/radvd restart");		
        nvram_commit();
#if 1		
	setWebMessage(0, NULL);
#endif				
	websRedirect(wp, "local/advance/ipv6.asp");
	
}

/* goform/setLanIP */ //----- aron add
static void setLanIP(webs_t wp, char_t *path, char_t *query)
{
        char *cur = nvram_get("lan_num");
        char tmp[TMP_LEN], buf[TMP_LEN];
        int num;
        int i, change = 0;
        char *lanIp_mode;

        if (!*cur) {
             num = 1;
        } else {
             num = atoi(cur);
        }

        for (i = 0; i < num; i++) {
           
           snprintf(tmp, sizeof(tmp), "%d", i); 	
           ezplib_get_attr_val("wl_mode_rule", 0, "mode", buf, TMP_LEN, EZPLIB_USE_CLI);
           
	   if (!strcmp(buf, "normal") || !strcmp(buf, "wisp"))
	   {
	        if (_save_lan_static(wp, tmp)) 
                      change = 1;
           }
	   else if (!strcmp(buf, "ap") || !strcmp(buf, "ur"))
           { 
           	lanIp_mode = websGetVar(wp, T("lanIpMode"), T(""));
           	if(!strcmp(lanIp_mode, "0")) //from dhcp
           	{
           	    nvram_set("lan0_proto", "dhcp");
                }
                else if(!strcmp(lanIp_mode, "1")) //static ip
           	{
                    nvram_set("lan0_proto", "static");
                    if (_save_lan_static_apMode(wp, tmp)) 
	                  change = 1;
                }
                //sync with wan dns
                if (_save_wan_dns(wp, tmp)) 
                      change = 1;
           }
           /*
           else if (!strcmp(buf, "ur"))
           { 
           	if (_save_lan_static_ur(wp, tmp)) 
                      change = 1;
           }
           */
           //postaction ?? reboot, do we need any postaction ??
        }

        nvram_commit();
      
   #if 1
        system("sleep 3 && reboot &");
	websHeader(wp);

	websWrite(wp, T("<script>\n"));	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
   #else      
        setWebMessage(0, NULL);
        char_t *submitUrl;
        submitUrl = websGetVar(wp, T("lan_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
   #endif
}

/* goform/setLanDhcp */ //----- aron add
static void setLanDhcp(webs_t wp, char_t *path, char_t *query)
{
	char *rule_set = "lan_dhcps_rule";
        char *rule_num = nvram_safe_get("lan_num");
        char tmp[TMP_LEN];
        char buf[TMP_LEN];
        char *enable, *start, *num;
        int lan_num, i, change = 0;
        int64_t map;
        int dhcpEnb;
        
        if (!*rule_num) {
             lan_num = 1;
        } else {
             lan_num = atoi(rule_num);
        }

        for (i = 0; i < lan_num; i++) {
            map = 0;

            if(i==0) //Normally, the nth for "0"
            {
                 /* Enable */
                 snprintf(tmp, sizeof(tmp), "lanDhcpType");
                 enable = websGetVar(wp, tmp, "");
        
                 /* Start */
                 snprintf(tmp, sizeof(tmp), "dhcpStarting");
                 start = websGetVar(wp, tmp, "");

                 /* Number */
                 snprintf(tmp, sizeof(tmp), "dhcpPoolSize");
                 num = websGetVar(wp, tmp, "");
            }
            else //increased nth for "1", "2",...etc.
            {
            	 /* Enable */
                 snprintf(tmp, sizeof(tmp), "lanDhcpType_%d", i);
                 enable = websGetVar(wp, tmp, "");
        
                 /* Start */
                 snprintf(tmp, sizeof(tmp), "dhcpStarting_%d", i);
                 start = websGetVar(wp, tmp, "");

                 /* Number */
                 snprintf(tmp, sizeof(tmp), "dhcpPoolSize_%d", i);
                 num = websGetVar(wp, tmp, "");
            }
            
            //preaction
            //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S50dnsmasq stop", i);
            //system(buf);
            system("/etc/rc.common /etc/init.d/dnsmasq stop");
            
            dhcpEnb=atoi(enable); 
            if (dhcpEnb == 0) //disabled
            {
                 ezplib_replace_attr(rule_set, i, "enable", "0");
                 change = 1;
            }
            else if(dhcpEnb == 1) //enabled
            {
            	 ezplib_replace_attr(rule_set, i, "enable", "1");
                 ezplib_replace_attr(rule_set, i, "start", start);
                 ezplib_replace_attr(rule_set, i, "num", num);
                 change = 1;
                 
                 //postaction
                 //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S50dnsmasq start", i);
                 //system(buf);
                 system("/etc/rc.common /etc/init.d/dnsmasq start");
            }
        }
        
        nvram_commit();
        //return change;
   #if 1
        websHeader(wp);
	websWrite(wp, T("<script>\n"));	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/loading.asp?2';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
   #else        
        setWebMessage(0, NULL);
        char_t *submitUrl;
        submitUrl = websGetVar(wp, T("lan_dhcp_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
   #endif
}

/* goform/setLanIPAlias */ //----- aron add
static void setLanIPAlias(webs_t wp, char_t *path, char_t *query)
{
        char   tmp[TMP_LEN];
        char   *rule_set = "lan_static_rule"; 
        char   *aliasEnb, *aliasIpAddr, *aliasNetmask;
        int enable;
         
        aliasEnb = websGetVar(wp, T("lan2enabled"), T(""));
        aliasIpAddr = websGetVar(wp, T("lan2Ip"), T(""));
        aliasNetmask = websGetVar(wp, T("lan2Netmask"), T(""));
        
        enable = atoi(aliasEnb);
        if(enable ==0)
            aliasEnb = "0";
        else if(enable ==1)
            aliasEnb = "1";
            
        //Translate AboCom's netmask to AXIM's nvram format
        aliasNetmask = mask2prefix(aliasNetmask);
            
        //preaction
        system("/etc/rc.common /etc/init.d/network-lan stop");
        
         ezplib_replace_attr(rule_set, 0, "alias_enable", aliasEnb);
         ezplib_replace_attr(rule_set, 0, "alias_ipaddr", aliasIpAddr);
         ezplib_replace_attr(rule_set, 0, "alias_netmask", aliasNetmask);
        
        //postaction
        system("/etc/rc.common /etc/init.d/network-lan start");
        
        nvram_commit();
        
	sleep(5);
	setWebMessage(0, NULL);

	char_t *submitUrl;
        submitUrl = websGetVar(wp, T("ip_alias_url"), T(""));   // aron add for hidden page
	websRedirect(wp, submitUrl);
}

//aron patch for giga
int isDuplicateMacIp(int index, char *staticMac, char *staticIp)
{
        char buf_mac[TMP_LEN], buf_ip[TMP_LEN];
        int i, rule_num = 8;
        
        index--;
        for(i=0; i<rule_num; i++)
        {
            if(i != index)
            {
               ezplib_get_attr_val("fl_hwaddr_rule", i, "hwaddr", buf_mac, TMP_LEN, EZPLIB_USE_CLI);
               ezplib_get_attr_val("fl_hwaddr_rule", i, "ipaddr", buf_ip, TMP_LEN, EZPLIB_USE_CLI); 
               if(!strcmp(staticMac, buf_mac))
                   return 1;
               if(!strcmp(staticIp, buf_ip))
                   return 2;
            }
	}
	return 0;
}

static void setLan_StaicLease_Dns(webs_t wp, char_t *path, char_t *query)
{    
        char *rule_set = "lan_dhcps_rule", *lease_rule_set = "fl_hwaddr_rule";
        char *rule_num = nvram_safe_get("lan_num");
        char tmp[TMP_LEN], cmd[TMP_LEN];
        char buf[TMP_LEN], buf_name[TMP_LEN], buf_action[TMP_LEN],
             buf_acl_enable[TMP_LEN], buf_arp_enable[TMP_LEN];
        char *name, *action, *acl_enable, *arp_enable, *static_enable;
        char *dnstype1, *dnstype2, *dnsaddr1, *dnsaddr2;
        int lan_num, i, len, change = 0;
        int64_t map;
        char *dhcpStaticMac, *dhcpStaticIp;
        int j, dhcp_static_lease_num=8; //Max lease numbers
        int curr_lease_num=8; //Current lease numbers(always be "8")
        int duplicate=0;
        int h,m;
        char *dhcpStaticMacOther, *dhcpStaticIpOther;
        
        //check if duplicated mac and ip have been set firstly
        for(h=1; h<=8; h++)
        {
            snprintf(tmp, sizeof(tmp), "dhcpStaticMac%d", h);
            dhcpStaticMac = websGetVar(wp, tmp, "");
            snprintf(tmp, sizeof(tmp), "dhcpStaticIp%d", h);
            dhcpStaticIp = websGetVar(wp, tmp, "");
                
            //Check for MAC address
            if(strcmp(dhcpStaticMac, "00:00:00:00:00:00"))
            {
                for(m=1; m<=8; m++)
                {
           	   if(h != m)
           	   {
           	      snprintf(tmp, sizeof(tmp), "dhcpStaticMac%d", m);
                      dhcpStaticMacOther = websGetVar(wp, tmp, "");
                      if(!strcmp(dhcpStaticMac, dhcpStaticMacOther))
                      {
                          duplicate=1;
                          break;
                      }
                   }
                }
            }
            if(duplicate<1) //MAC is fine, then check for IP
            {
                //Check for IP address
                if(strcmp(dhcpStaticIp, "0.0.0.0"))
                {
           	    for(m=1; m<=8; m++)
           	    {
           	        if(h != m)
           	        {
           	           snprintf(tmp, sizeof(tmp), "dhcpStaticIp%d", m);
                           dhcpStaticIpOther = websGetVar(wp, tmp, "");
                           if(!strcmp(dhcpStaticIp, dhcpStaticIpOther))
                           {
                              duplicate=2;
                              break;
                           }
                        }
                    }
                }
            }
            if(duplicate>=1) //either MAC or IP has been duplicated
               break;
        }
        if(duplicate)
        {
        	char temp2_str[65];
        	if(duplicate==1)
		    strcpy(temp2_str, "Mac Address already exists.");
		else if(duplicate==2)
		    strcpy(temp2_str, "IP Address already exists.");
		setWebMessage(1, (char *)&temp2_str);
                char_t *submitUrl;
                submitUrl = websGetVar(wp, T("lan_dhcp_advance_url"), T(""));  // aron add for hidden page
	        websRedirect(wp, submitUrl);
	        return;
	}
	
        /*
        for(j=1; j<=dhcp_static_lease_num; j++)
        {
           snprintf(tmp, sizeof(tmp), "dhcpStaticMac%d", j);
           dhcpStaticMac = websGetVar(wp, tmp, "");
           
           snprintf(tmp, sizeof(tmp), "dhcpStaticIp%d", j);
           dhcpStaticIp = websGetVar(wp, tmp, "");
           
           //int duplicate = isDuplicateMacIp(j, dhcpStaticMac, dhcpStaticIp);
           duplicate = isDuplicateMacIp(j, dhcpStaticMac, dhcpStaticIp);
           if(duplicate)
           {
        	char temp2_str[65];
        	if(duplicate==1)
		    strcpy(temp2_str, "Mac Address already exists.");
		else if(duplicate==2)
		    strcpy(temp2_str, "IP Address already exists.");
		setWebMessage(1, (char *)&temp2_str);
                char_t *submitUrl;
                submitUrl = websGetVar(wp, T("lan_dhcp_advance_url"), T(""));  // aron add for hidden page
	        websRedirect(wp, submitUrl);
	        return;
	   }
        }
        */
 
        //preaction
        system("/etc/rc.common /etc/init.d/dnsmasq stop");
        system("/etc/rc.common /etc/init.d/network-acl stop fl_hwaddr");

        /* static IP/MAC lease settings*/
        for(j=1; j<=dhcp_static_lease_num; j++)
        {
            /* MAC Addr */
            snprintf(tmp, sizeof(tmp), "dhcpStaticMac%d", j);
            dhcpStaticMac = websGetVar(wp, tmp, "");
            
            /* IP Addr */
            snprintf(tmp, sizeof(tmp), "dhcpStaticIp%d", j);
            dhcpStaticIp = websGetVar(wp, tmp, "");
            
            if(!strcmp(dhcpStaticMac, "00:00:00:00:00:00"))
            {
                dhcpStaticMac= "";
                static_enable= "0";
            }
            else
            {
                static_enable = "1";
            }
                
            if(!strcmp(dhcpStaticIp, "0.0.0.0"))
                dhcpStaticIp = "";
            
            /* Not configured by UI, just retrieve it by nvram to make the whole 
               rule complete in order to compare with or replace old rule */
            /* Name */
            ezplib_get_attr_val(lease_rule_set, j-1, "name", buf_name, TMP_LEN, EZPLIB_USE_CLI);
            name = buf_name;
            
            /* Action */
            ezplib_get_attr_val(lease_rule_set, j-1, "action", buf_action, TMP_LEN, EZPLIB_USE_CLI);
            action = buf_action;
            
            /* Acl_enable */
            ezplib_get_attr_val(lease_rule_set, j-1, "acl_enable", buf_acl_enable, TMP_LEN, EZPLIB_USE_CLI);
            acl_enable = buf_acl_enable;
            
            /* Arp_enable */
            ezplib_get_attr_val(lease_rule_set, j-1, "arp_enable", buf_arp_enable, TMP_LEN, EZPLIB_USE_CLI);
            arp_enable = buf_arp_enable;
            
            len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s", name, dhcpStaticMac, action, 
                acl_enable, arp_enable, static_enable, dhcpStaticIp);

            if (len > TMP_LEN - 1) {
                return 0;
            }

            //preaction for dhcp static lease
            //snprintf(cmd, TMP_LEN, "NUM=0  /etc/rc.common /etc/rc.d/S60network-acl stop fl_hwaddr");
            //system(cmd);
            //system("/etc/rc.common /etc/init.d/network-acl stop fl_hwaddr");
            
            ezplib_delete_rule(lease_rule_set, j-1);
            ezplib_add_rule(lease_rule_set, j-1, tmp);
        }
        sprintf(tmp,"%d", curr_lease_num);
        nvram_set("fl_hwaddr_rule_num", tmp);
        //postaction for dhcp static lease
        //snprintf(cmd, TMP_LEN, "NUM=0  /etc/rc.common /etc/rc.d/S60network-acl start fl_hwaddr");
        //system(cmd);
        //system("/etc/rc.common /etc/init.d/network-acl start fl_hwaddr");
        
        /* dhcp dns settings */
        if (!*rule_num) {
             lan_num = 1;
        } else {
             lan_num = atoi(rule_num);
        }

        for (i = 0; i < lan_num; i++) {
            map = 0;

            if(i==0) //Normally, the nth for "0"
            {
                 /* Fisrt DNS Type */
                 snprintf(tmp, sizeof(tmp), "dhcpDns1Type");
                 dnstype1 = websGetVar(wp, tmp, "");            

                 /* First DNS Server IP */
                 snprintf(tmp, sizeof(tmp), "dhcpPriDns");
                 dnsaddr1 = websGetVar(wp, tmp, "");
                 
                 /* Second DNS Type */
                 snprintf(tmp, sizeof(tmp), "dhcpDns2Type");
                 dnstype2 = websGetVar(wp, tmp, "");            

                 /* Second DNS Server IP */
                 snprintf(tmp, sizeof(tmp), "dhcpSecDns");
                 dnsaddr2 = websGetVar(wp, tmp, "");
            }
            else //increased nth for "1", "2",...etc.
            {
                 /* Fisrt DNS Type */
                 snprintf(tmp, sizeof(tmp), "dhcpDns1Type_%d", i);
                 dnstype1 = websGetVar(wp, tmp, "");            

                 /* First DNS Server IP */
                 snprintf(tmp, sizeof(tmp), "dhcpPriDns_%d", i);
                 dnsaddr1 = websGetVar(wp, tmp, "");
                 
                 /* Second DNS Type */
                 snprintf(tmp, sizeof(tmp), "dhcpDns2Type_%d", i);
                 dnstype2 = websGetVar(wp, tmp, "");            

                 /* Second DNS Server IP */
                 snprintf(tmp, sizeof(tmp), "dhcpSecDns_%d", i);
                 dnsaddr2 = websGetVar(wp, tmp, "");
            }
            
            //preaction
            //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S50dnsmasq stop", i);
            //system(buf);
            //system("/etc/rc.common /etc/init.d/dnsmasq stop");
            
            //First DNS
            if(!strcmp(dnstype1, "USER"))
            {
                 ezplib_replace_attr(rule_set, i, "dnstype", "custom");
                 ezplib_replace_attr(rule_set, i, "dnsaddr1", dnsaddr1);
                 change = 1;
            }
            else
            {
            	 if(!strcmp(dnstype1, "RELAY"))
            	     dnstype1 = "dnsrelay";
            	 else if(!strcmp(dnstype1, "ISP"))
            	     dnstype1 = "ispdns";
            	 else if(!strcmp(dnstype1, "NONE"))
            	     dnstype1 = "none";
            	 ezplib_replace_attr(rule_set, i, "dnstype", dnstype1);
                 ezplib_replace_attr(rule_set, i, "dnsaddr1", "");
                 change = 1;
            }
            //Second DNS
            if(!strcmp(dnstype2, "USER"))
            {
                 ezplib_replace_attr(rule_set, i, "dnstype2", "custom");
                 ezplib_replace_attr(rule_set, i, "dnsaddr2", dnsaddr2);
                 change = 1;
            }
            else
            {
            	 if(!strcmp(dnstype2, "RELAY"))
            	     dnstype2 = "dnsrelay";
            	 else if(!strcmp(dnstype2, "ISP"))
            	     dnstype2 = "ispdns";
            	 else if(!strcmp(dnstype2, "NONE"))
            	     dnstype2 = "none";
            	 ezplib_replace_attr(rule_set, i, "dnstype2", dnstype2);
                 ezplib_replace_attr(rule_set, i, "dnsaddr2", "");
                 change = 1;
            }
            
            //postaction
            //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S50dnsmasq start", i);
            //system(buf);
            //system("/etc/rc.common /etc/init.d/dnsmasq start");
           
        }
        
        //postaction
        system("/etc/rc.common /etc/init.d/dnsmasq start");
        system("/etc/rc.common /etc/init.d/network-acl start fl_hwaddr");
        
        nvram_commit();
        //return change;
   #if 1
        websHeader(wp);
	websWrite(wp, T("<script>\n"));	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/loading.asp?3';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
   #else    
        setWebMessage(0, NULL);  
        char_t *submitUrl;
        submitUrl = websGetVar(wp, T("lan_dhcp_advance_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
   #endif
}

/* goform/setLanUPnP */ //aron add
static void setLanUPnP(webs_t wp, char_t *path, char_t *query)
{
    char tmp[TMP_LEN];    
    char *enable;
    char *rule_set = "upnp_rule";
    int upnpEnb;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "upnpEnbl");
    enable = websGetVar(wp, tmp, "");
    upnpEnb = atoi(enable);
    
    //preaction tmp 
    #if 1
    	snprintf(tmp, TMP_LEN, "NUM=0 TYPE=wan /etc/rc.common /etc/rc.d/T90upnpd stop");
    #else
    	snprintf(tmp, TMP_LEN, "NUM=0 TYPE=wan /etc/rc.common /etc/rc.d/T90miniupnpd stop");
    #endif
    system(tmp);
    
    if(upnpEnb ==0)
        enable = "0";
    else if(upnpEnb ==1)
        enable = "1";
    ezplib_replace_attr(rule_set, 0, "enable", enable);
     
    if(!strcmp(enable, "1"))
    { 
       //postaction
       #if 1
       	snprintf(tmp, TMP_LEN, "NUM=0 TYPE=wan /etc/rc.common /etc/rc.d/T90upnpd start");
       #else
       	snprintf(tmp, TMP_LEN, "NUM=0 TYPE=wan /etc/rc.common /etc/rc.d/T90miniupnpd start");
       #endif
       system(tmp);
    }
    
    nvram_commit();

    setWebMessage(0, NULL);
    	
    char_t *submitUrl;
    submitUrl = websGetVar(wp, T("upnp_url"), T(""));  // aron add for hidden page
    websRedirect(wp, submitUrl);
}

/* goform/setIgmpSnooping */ //aron add  2009.02.12
static void setIgmpSnooping(webs_t wp, char_t *path, char_t *query)
{
        char   cmd[TMP_LEN];
        char   *rule_set = "igmp_snoop_rule"; 
        char   *SnoopingEnabled, *SnoopingLAN1, *SnoopingLAN2, *SnoopingLAN3, *SnoopingLAN4;
         
        //preaction
        //system("/etc/init.d/igmpproxy stop");
        
        SnoopingEnabled = websGetVar(wp, T("snoopingEnbl"), T(""));
        
        if(!strcmp(SnoopingEnabled,"1")) //enabled
        { 
             SnoopingLAN1 = websGetVar(wp, T("snoopingLAN1"), T(""));
             SnoopingLAN2 = websGetVar(wp, T("snoopingLAN2"), T(""));
             SnoopingLAN3 = websGetVar(wp, T("snoopingLAN3"), T(""));
             SnoopingLAN4 = websGetVar(wp, T("snoopingLAN4"), T(""));
             
             if(!strcmp(SnoopingLAN1,"1"))
                  ezplib_replace_attr(rule_set, 0, "p1", "1");
             else
                  ezplib_replace_attr(rule_set, 0, "p1", "0");
             
             if(!strcmp(SnoopingLAN2,"1"))
                  ezplib_replace_attr(rule_set, 0, "p2", "1");
             else
                  ezplib_replace_attr(rule_set, 0, "p2", "0");
             
             if(!strcmp(SnoopingLAN3,"1"))
                  ezplib_replace_attr(rule_set, 0, "p3", "1");
             else
                  ezplib_replace_attr(rule_set, 0, "p3", "0");
             
             if(!strcmp(SnoopingLAN4,"1"))
                  ezplib_replace_attr(rule_set, 0, "p4", "1");
             else
                  ezplib_replace_attr(rule_set, 0, "p4", "0");
             
             ezplib_replace_attr(rule_set, 0, "enable", "1");
        }
        else //disabled
        {
             ezplib_replace_attr(rule_set, 0, "enable", "0");
        }
        
        /*
        if(!strcmp(SnoopingEnabled,"1"))
        {
            //postaction
            system("/etc/init.d/igmpproxy start");
        }
        */
        
        nvram_commit();
        
   #if 1
        websHeader(wp);
	websWrite(wp, T("<script>\n"));	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/loading.asp?4';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
   #else        
        setWebMessage(0, NULL);
	char_t *submitUrl;
        submitUrl = websGetVar(wp, T("igmp_snooping_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
   #endif
}

/* goform/setLanIGMP */ //aron add
static void setLanIGMP(webs_t wp, char_t *path, char_t *query)
{
	char_t   *igmp_en;
	char   *auto_bridge_en;	//Steve add 2009/02/09

        //preaction
        system("/etc/init.d/igmpproxy stop");

	igmp_en = websGetVar(wp, T("igmpEnbl"), T("0"));
	nvram_set("igmp_proxy_rule", atoi(igmp_en) ? "1" : "0");	

	//Steve add 2009/02/09
	auto_bridge_en = websGetVar(wp, T("WAN_AUTO_BRIDGE"), T("0"));
	//ezplib_replace_attr("wl_mode_rule", 0, "auto_bridge", atoi(auto_bridge_en) ? "1" : "0");
	ezplib_replace_attr("wl_mode_rule", 0, "auto_bridge", auto_bridge_en);

        if(!strcmp(igmp_en, "1"))
        {
            //postaction
            system("/etc/init.d/igmpproxy start");
        }
        
        if(!strcmp(auto_bridge_en, "1"))
        {
        	 printf("auto_bridge_en=1 -> /sbin/ezp-auto-bridge");
        	 //system("/sbin/ezp-auto-bridge");
        }

	nvram_commit();

	setWebMessage(0, NULL);
	char_t *submitUrl;
        submitUrl = websGetVar(wp, T("igmp_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
}

/* goform/setLan */
static void setLan(webs_t wp, char_t *path, char_t *query)
{
#if 0
	char_t	*ip, *nm, *dhcp_tp, *stp_en, *lltd_en, *igmp_en, *upnp_en,
			*radvd_en, *pppoer_en, *dnsp_en;
	char_t	*gw = NULL, *pd = NULL, *sd = NULL;
	char_t *lan2enabled, *lan2_ip, *lan2_nm;
#ifdef GA_HOSTNAME_SUPPORT
	char_t	*host;
#endif
	char_t  *dhcp_s, *dhcp_e, *dhcp_m, *dhcp_pd, *dhcp_sd, *dhcp_g, *dhcp_l;
	char_t	*dhcp_sl1, *dhcp_sl2, *dhcp_sl3;
	char	*opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char	*wan_ip = nvram_bufget(RT2860_NVRAM, "wan_ipaddr");

	ip = websGetVar(wp, T("lanIp"), T(""));
	nm = websGetVar(wp, T("lanNetmask"), T(""));
	lan2enabled = websGetVar(wp, T("lan2enabled"), T(""));
	lan2_ip = websGetVar(wp, T("lan2Ip"), T(""));
	lan2_nm = websGetVar(wp, T("lan2Netmask"), T(""));
#ifdef GA_HOSTNAME_SUPPORT
	host = websGetVar(wp, T("hostname"), T("0"));
#endif
	dhcp_tp = websGetVar(wp, T("lanDhcpType"), T("DISABLE"));
	stp_en = websGetVar(wp, T("stpEnbl"), T("0"));
	lltd_en = websGetVar(wp, T("lltdEnbl"), T("0"));
	igmp_en = websGetVar(wp, T("igmpEnbl"), T("0"));
	upnp_en = websGetVar(wp, T("upnpEnbl"), T("0"));
	radvd_en = websGetVar(wp, T("radvdEnbl"), T("0"));
	pppoer_en = websGetVar(wp, T("pppoeREnbl"), T("0"));
	dnsp_en = websGetVar(wp, T("dnspEnbl"), T("0"));
	dhcp_s = websGetVar(wp, T("dhcpStart"), T(""));
	dhcp_e = websGetVar(wp, T("dhcpEnd"), T(""));
	dhcp_m = websGetVar(wp, T("dhcpMask"), T(""));
	dhcp_pd = websGetVar(wp, T("dhcpPriDns"), T(""));
	dhcp_sd = websGetVar(wp, T("dhcpSecDns"), T(""));
	dhcp_g = websGetVar(wp, T("dhcpGateway"), T(""));
	dhcp_l = websGetVar(wp, T("dhcpLease"), T("86400"));
	dhcp_sl1 = websGetVar(wp, T("dhcpStatic1"), T(""));
	dhcp_sl2 = websGetVar(wp, T("dhcpStatic2"), T(""));
	dhcp_sl3 = websGetVar(wp, T("dhcpStatic3"), T(""));

	/*
	 * lan and wan ip should not be the same except in bridge mode
	 */
	if (NULL != opmode && strcmp(opmode, "0") && !strncmp(ip, wan_ip, 15)) {
		websError(wp, 200, "IP address is identical to WAN");
		return;
	}
	if (!strcmp(lan2enabled, "1"))
	{
		if (NULL != opmode && strcmp(opmode, "0") && !strncmp(lan2_ip, wan_ip, 15)) {
			websError(wp, 200, "LAN2 IP address is identical to WAN");
			return;
		}
		else if (NULL != opmode && strcmp(opmode, "0") && !strncmp(lan2_ip, ip, 15)) {
			websError(wp, 200, "LAN2 IP address is identical to LAN1");
			return;
		}
	}
	// configure gateway and dns (WAN) at bridge mode
	if (!strncmp(opmode, "0", 2)) {
		gw = websGetVar(wp, T("lanGateway"), T(""));
		pd = websGetVar(wp, T("lanPriDns"), T(""));
		sd = websGetVar(wp, T("lanSecDns"), T(""));
		nvram_bufset(RT2860_NVRAM, "wan_gateway", gw);
		nvram_bufset(RT2860_NVRAM, "wan_primary_dns", pd);
		nvram_bufset(RT2860_NVRAM, "wan_secondary_dns", sd);
	}
	nvram_bufset(RT2860_NVRAM, "lan_ipaddr", ip);
	nvram_bufset(RT2860_NVRAM, "lan_netmask", nm);
	nvram_bufset(RT2860_NVRAM, "Lan2Enabled", lan2enabled);
	nvram_bufset(RT2860_NVRAM, "lan2_ipaddr", lan2_ip);
	nvram_bufset(RT2860_NVRAM, "lan2_netmask", lan2_nm);
#ifdef GA_HOSTNAME_SUPPORT
	nvram_bufset(RT2860_NVRAM, "HostName", host);
#endif

	if (!strncmp(dhcp_tp, "DISABLE", 8))
		nvram_bufset(RT2860_NVRAM, "dhcpEnabled", "0");
	else if (!strncmp(dhcp_tp, "SERVER", 7)) {
		if (-1 == inet_addr(dhcp_s)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "invalid DHCP Start IP");
			return;
		}
		nvram_bufset(RT2860_NVRAM, "dhcpStart", dhcp_s);
		if (-1 == inet_addr(dhcp_e)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "invalid DHCP End IP");
			return;
		}
		nvram_bufset(RT2860_NVRAM, "dhcpEnd", dhcp_e);
		if (-1 == inet_addr(dhcp_m)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "invalid DHCP Subnet Mask");
			return;
		}
		nvram_bufset(RT2860_NVRAM, "dhcpMask", dhcp_m);
		nvram_bufset(RT2860_NVRAM, "dhcpEnabled", "1");
		nvram_bufset(RT2860_NVRAM, "dhcpPriDns", dhcp_pd);
		nvram_bufset(RT2860_NVRAM, "dhcpSecDns", dhcp_sd);
		nvram_bufset(RT2860_NVRAM, "dhcpGateway", dhcp_g);
		nvram_bufset(RT2860_NVRAM, "dhcpLease", dhcp_l);
		nvram_bufset(RT2860_NVRAM, "dhcpStatic1", dhcp_sl1);
		nvram_bufset(RT2860_NVRAM, "dhcpStatic2", dhcp_sl2);
		nvram_bufset(RT2860_NVRAM, "dhcpStatic3", dhcp_sl3);
	}

	nvram_bufset(RT2860_NVRAM, "stpEnabled", stp_en);
	nvram_bufset(RT2860_NVRAM, "lltdEnabled", lltd_en);
	nvram_bufset(RT2860_NVRAM, "igmpEnabled", igmp_en);
	nvram_bufset(RT2860_NVRAM, "upnpEnabled", upnp_en);
	nvram_bufset(RT2860_NVRAM, "radvdEnabled", radvd_en);
	nvram_bufset(RT2860_NVRAM, "pppoeREnabled", pppoer_en);
	nvram_bufset(RT2860_NVRAM, "dnsPEnabled", dnsp_en);
	nvram_commit(RT2860_NVRAM);
// Ralink fixed: Ethernet Converter bug that cannot routing well
#if 0
	doSystem("lan.sh");

	updateFlash8021x(RT2860_NVRAM);
	doSystem("ralink_init make_wireless_config rt2860");
	restart8021XDaemon(RT2860_NVRAM);

	firewall_init();
	management_init();
	RoutingInit();
#else
	initInternet();
#endif
	//debug print
	websHeader(wp);
	websWrite(wp, T("<h3>LAN Interface Setup</h3><br>\n"));

#ifdef GA_HOSTNAME_SUPPORT
	websWrite(wp, T("Hostname: %s<br>\n"), host);
#endif
	websWrite(wp, T("IP: %s<br>\n"), ip);
	websWrite(wp, T("Netmask: %s<br>\n"), nm);
	websWrite(wp, T("LAN2 Enabled: %s<br>\n"), lan2enabled);
	websWrite(wp, T("LAN2 IP: %s<br>\n"), lan2_ip);
	websWrite(wp, T("LAN2 Netmask: %s<br>\n"), lan2_nm);
	if (!strncmp(opmode, "0", 2)) {
		websWrite(wp, T("Gateway: %s<br>\n"), gw);
		websWrite(wp, T("PriDns: %s<br>\n"), pd);
		websWrite(wp, T("SecDns: %s<br>\n"), sd);
	}
	websWrite(wp, T("DHCP type: %s<br>\n"), dhcp_tp);
	if (strncmp(dhcp_tp, "DISABLE", 8)) {
		websWrite(wp, T("--> DHCP start: %s<br>\n"), dhcp_s);
		websWrite(wp, T("--> DHCP end: %s<br>\n"), dhcp_e);
		websWrite(wp, T("--> DHCP mask: %s<br>\n"), dhcp_m);
		websWrite(wp, T("--> DHCP DNS: %s %s<br>\n"), dhcp_pd, dhcp_sd);
		websWrite(wp, T("--> DHCP gateway: %s<br>\n"), dhcp_g);
		websWrite(wp, T("--> DHCP lease: %s<br>\n"), dhcp_l);
		websWrite(wp, T("--> DHCP static 1: %s<br>\n"), dhcp_sl1);
		websWrite(wp, T("--> DHCP static 2: %s<br>\n"), dhcp_sl2);
		websWrite(wp, T("--> DHCP static 3: %s<br>\n"), dhcp_sl3);
	}
	websWrite(wp, T("STP enable: %s<br>\n"), stp_en);
	websWrite(wp, T("LLTD enable: %s<br>\n"), lltd_en);
	websWrite(wp, T("IGMP proxy enable: %s<br>\n"), igmp_en);
	websWrite(wp, T("UPNP enable: %s<br>\n"), upnp_en);
	websWrite(wp, T("RADVD enable: %s<br>\n"), radvd_en);
	websWrite(wp, T("DNS proxy enable: %s<br>\n"), dnsp_en);
	websFooter(wp);
	websDone(wp, 200);
#endif
}

/* goform/setVpnPaThru */
static void setVpnPaThru(webs_t wp, char_t *path, char_t *query)
{
#if 0
	char_t	*l2tp_pt, *ipsec_pt, *pptp_pt;

	l2tp_pt = websGetVar(wp, T("l2tpPT"), T("0"));
	ipsec_pt = websGetVar(wp, T("ipsecPT"), T("0"));
	pptp_pt = websGetVar(wp, T("pptpPT"), T("0"));

	nvram_bufset(RT2860_NVRAM, "l2tpPassThru", l2tp_pt);
	nvram_bufset(RT2860_NVRAM, "ipsecPassThru", ipsec_pt);
	nvram_bufset(RT2860_NVRAM, "pptpPassThru", pptp_pt);
	nvram_commit(RT2860_NVRAM);

	doSystem("vpn-passthru.sh");

	//debug print
	websHeader(wp);
	websWrite(wp, T("<h3>VPN Pass Through</h3><br>\n"));
	websWrite(wp, T("l2tp: %s<br>\n"), l2tp_pt);
	websWrite(wp, T("ipsec: %s<br>\n"), ipsec_pt);
	websWrite(wp, T("pptp: %s<br>\n"), pptp_pt);
	websFooter(wp);
	websDone(wp, 200);
#endif
}

//aron patch for giga
static int
_save_wan_dhcp(webs_t wp, char *value)
{
#if 0
    /* 
     * In dhcp mode, we don't have any settings configured by UI.
     * The only needs is to call preaction.
     */
    
    //char buf[TMP_LEN];
    //int idx = atoi(value);
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    return 1;
    
#else
    char tmp[TMP_LEN];
    char *enable, *server, *username, *passwd;
    int idx = atoi(value), len, change = 0;
  
  if(idx==0) //Normally, the nth for "0"
  {
    /* Enable */
    snprintf(tmp, sizeof(tmp), "bigPondEnb");
    enable = websGetVar(wp, tmp, "");
    
    /* Server */
    snprintf(tmp, sizeof(tmp), "bigPondSrv");
    server = websGetVar(wp, tmp, "");
    
    /* Username */
    snprintf(tmp, sizeof(tmp), "bigPondUser");
    username = websGetVar(wp, tmp, "");

    /* Password */
    snprintf(tmp, sizeof(tmp), "bigPondPass");
    passwd = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* Enable */
    snprintf(tmp, sizeof(tmp), "bigPondEnb_%d", idx);
    enable = websGetVar(wp, tmp, "");
    
    /* Server */
    snprintf(tmp, sizeof(tmp), "bigPondSrv_%d", idx);
    server = websGetVar(wp, tmp, "");
    
    /* Username */
    snprintf(tmp, sizeof(tmp), "bigPondUser_%d", idx);
    username = websGetVar(wp, tmp, "");

    /* Password */
    snprintf(tmp, sizeof(tmp), "bigPondPass_%d", idx);
    passwd = websGetVar(wp, tmp, "");
  }
  
    if(!strcmp(enable, "1")) //bigPond enabled
    {
        len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s", 
                       enable, server, username, passwd);
        if (len >= TMP_LEN) {
        return 0;
        }
        //preaction
        //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
        //system(buf);
        //system("ifdown wan0");
        ezplib_replace_rule("wan_bigpond_rule", idx, tmp);
    }
    else
    {
    	ezplib_replace_attr("wan_bigpond_rule", idx, "enable", "0");
    }
    change = 1;
    return change;   
#endif
}

//aron patch for giga
static int
_save_wan_wwan(webs_t wp, char *value)
{
#if 0 //full configuration
    char tmp[TMP_LEN], buf[TMP_LEN], buf_idletime[TMP_LEN],
         buf_echo[TMP_LEN], buf_redial[TMP_LEN];
    char *apn, *pin, *mode, *username, *passwd, *demand, *idletime, 
         *redialperiod, *mtu, *dialstr, *brand, *model, *apn_type,
         *location, *isp, *echo_failure, *auth, *turbolink ;
    int idx = atoi(value), len, val, change = 0;
    int nttynum, ndatatty, nctrltty;
    int nflag = 1;
    char *devname = "0";
    nttynum = ndatatty = nctrltty =0;

 if(idx==0) //Normally, the nth for "0"
 {
    /* APN Type */
    snprintf(tmp, sizeof(tmp), "apnType3g");
    apn_type = websGetVar(wp, tmp, "");

    /* Location */
    snprintf(tmp, sizeof(tmp), "location3g");
    location = websGetVar(wp, tmp, "");

    /* ISP */
    snprintf(tmp, sizeof(tmp), "srvPro3g");
    isp = websGetVar(wp, tmp, "");

    /* APN */
    snprintf(tmp, sizeof(tmp), "apn");
    apn = websGetVar(wp, tmp, "");

    /* PIN */
    snprintf(tmp, sizeof(tmp), "pin3g");
    pin = websGetVar(wp, tmp, "");

    /* Mode */
    snprintf(tmp, sizeof(tmp), "connMode3g");
    mode = websGetVar(wp, tmp, "");

    /* Auth */
    snprintf(tmp, sizeof(tmp), "auth3g");
    auth = websGetVar(wp, tmp, "");

    /* User Name */
    snprintf(tmp, sizeof(tmp), "username3g");
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "password3g");
    passwd = websGetVar(wp, tmp, "");

    /* Demand */
    snprintf(tmp, sizeof(tmp), "pppType");
    demand = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "maxIdle");
    idletime = websGetVar(wp, tmp, "");

    /* Echo Failure Number */
    snprintf(tmp, sizeof(tmp), "retryThreld");
    echo_failure = websGetVar(wp, tmp, "");

    /* Redial Period */
    snprintf(tmp, sizeof(tmp), "echoIntvl");
    redialperiod = websGetVar(wp, tmp, "");

    /* MRU & MTU */
    snprintf(tmp, sizeof(tmp), "mtu3g");
    mtu = websGetVar(wp, tmp, "");
    val = atoi(mtu);

    /* Dial String */
    snprintf(tmp, sizeof(tmp), "dial3g");
    dialstr = websGetVar(wp, tmp, "");

    /* Brand */
    snprintf(tmp, sizeof(tmp), "brand3g");
    brand = websGetVar(wp, tmp, "");

    /* Model */
    snprintf(tmp, sizeof(tmp), "model3g");
    model = websGetVar(wp, tmp, "");

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "turbo3g");
    turbolink = websGetVar(wp, tmp, "");
 }
 else //increased nth for "1", "2",...etc.
 {
    /* APN Type */
    snprintf(tmp, sizeof(tmp), "apnType3g_%d", idx);
    apn_type = websGetVar(wp, tmp, "");

    /* Location */
    snprintf(tmp, sizeof(tmp), "location3g_%d", idx);
    location = websGetVar(wp, tmp, "");

    /* ISP */
    snprintf(tmp, sizeof(tmp), "srvPro3g_%d", idx);
    isp = websGetVar(wp, tmp, "");

    /* APN */
    snprintf(tmp, sizeof(tmp), "apn_%d", idx);
    apn = websGetVar(wp, tmp, "");

    /* PIN */
    snprintf(tmp, sizeof(tmp), "pin3g_%d", idx);
    pin = websGetVar(wp, tmp, "");

    /* Mode */
    snprintf(tmp, sizeof(tmp), "connMode3g_%d", idx);
    mode = websGetVar(wp, tmp, "");

    /* Auth */
    snprintf(tmp, sizeof(tmp), "auth3g_%d", idx);
    auth = websGetVar(wp, tmp, "");

    /* User Name */
    snprintf(tmp, sizeof(tmp), "username3g_%d", idx);
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "password3g_%d", idx);
    passwd = websGetVar(wp, tmp, "");

    /* Demand */
    snprintf(tmp, sizeof(tmp), "pppType_%d", idx);
    demand = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "maxIdle_%d", idx);
    idletime = websGetVar(wp, tmp, "");

    /* Echo Failure Number */
    snprintf(tmp, sizeof(tmp), "retryThreld_%d", idx);
    echo_failure = websGetVar(wp, tmp, "");

    /* Redial Period */
    snprintf(tmp, sizeof(tmp), "echoIntvl_%d", idx);
    redialperiod = websGetVar(wp, tmp, "");

    /* MRU & MTU */
    snprintf(tmp, sizeof(tmp), "mtu3g_%d", idx);
    mtu = websGetVar(wp, tmp, "");
    val = atoi(mtu);

    /* Dial String */
    snprintf(tmp, sizeof(tmp), "dial3g_%d", idx);
    dialstr = websGetVar(wp, tmp, "");

    /* Brand */
    snprintf(tmp, sizeof(tmp), "brand3g_%d", idx);
    brand = websGetVar(wp, tmp, "");

    /* Model */
    snprintf(tmp, sizeof(tmp), "model3g_%d", idx);
    model = websGetVar(wp, tmp, ""); 

    /* TurboLink */
    snprintf(tmp, sizeof(tmp), "turbo3g_%d", idx);
    turbolink = websGetVar(wp, tmp, "");
 }
    
    if(!strcmp(demand, "0")) //keep alive
    {
        ezplib_get_attr_val("wan_wwan_rule", idx, "idletime", buf_idletime, TMP_LEN, EZPLIB_USE_CLI);
        idletime = buf_idletime;
    }
    else if(!strcmp(demand, "1")) //on-demand
    {
        ezplib_get_attr_val("wan_wwan_rule", idx, "echo_failure", buf_echo, TMP_LEN, EZPLIB_USE_CLI);
        ezplib_get_attr_val("wan_wwan_rule", idx, "redialperiod", buf_redial, TMP_LEN, EZPLIB_USE_CLI);
        echo_failure = buf_echo;
        redialperiod = buf_redial;
    }
    
    /* ttynum, datatty, ctrltty, devname, flag will be determined
     * by ezpcom-mode automatically. So we just set them to default
     * values here.
     */
    len = 
      snprintf(tmp, TMP_LEN, 
        "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%d^%d^%s^%s^%s^%d^%d^%d^%s^%d^%s",
            apn_type, location, isp, apn, pin, mode, auth, username, passwd, 
            demand, idletime, echo_failure, redialperiod, val, val, dialstr, 
            brand, model, nttynum, ndatatty, nctrltty, devname, nflag, turbolink);
            
    if (len >= TMP_LEN) {
        return 0;
    }
    
    //preaction
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    
    ezplib_replace_rule("wan_wwan_rule", idx, tmp);
    change = 1;
    
    return change;
    
#else //simplified configuration
    char tmp[TMP_LEN];
    char *apn, *pin, *username, *passwd, *dialstr, *demand, *idletime;
    int idx = atoi(value), change = 0, demand_index;
    
 if(idx==0) //Normally, the nth for "0"
 {
    /* APN */
    snprintf(tmp, sizeof(tmp), "apn");
    apn = websGetVar(wp, tmp, "");

    /* PIN */
    snprintf(tmp, sizeof(tmp), "pin3g");
    pin = websGetVar(wp, tmp, "");

    /* User Name */
    snprintf(tmp, sizeof(tmp), "username3g");
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "password3g");
    passwd = websGetVar(wp, tmp, "");

    /* Dial String */
    snprintf(tmp, sizeof(tmp), "dial3g");
    dialstr = websGetVar(wp, tmp, "");
    
    /* Demand */
    snprintf(tmp, sizeof(tmp), "nailedup3g");
    demand = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "idleTime3g");
    idletime = websGetVar(wp, tmp, "");

 }
 else //increased nth for "1", "2",...etc.
 {
    /* APN */
    snprintf(tmp, sizeof(tmp), "apn_%d", idx);
    apn = websGetVar(wp, tmp, "");

    /* PIN */
    snprintf(tmp, sizeof(tmp), "pin3g_%d", idx);
    pin = websGetVar(wp, tmp, "");

    /* User Name */
    snprintf(tmp, sizeof(tmp), "username3g_%d", idx);
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "password3g_%d", idx);
    passwd = websGetVar(wp, tmp, "");

    /* Dial String */
    snprintf(tmp, sizeof(tmp), "dial3g_%d", idx);
    dialstr = websGetVar(wp, tmp, "");
    
    /* Demand */
    snprintf(tmp, sizeof(tmp), "nailedup3g_%d", idx);
    demand = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "idleTime3g_%d", idx);
    idletime = websGetVar(wp, tmp, "");
 }
 
    //Translate AboCom's nailup to AXIM's nvram format
    demand_index=atoi(demand);
    if(demand_index==1) //Abocom treat nailup "checked" as "1"
    {
    	ezplib_replace_attr("wan_wwan_rule", idx, "demand", "0"); //AXIM treat nailup "checked" as "0"
    }
    else if(demand_index==0)
    {
        ezplib_replace_attr("wan_wwan_rule", idx, "demand", "1");
        ezplib_replace_attr("wan_wwan_rule", idx, "idletime", idletime); //update idletime value
    }
    
    ezplib_replace_attr("wan_wwan_rule", idx, "apn", apn);
    ezplib_replace_attr("wan_wwan_rule", idx, "pin", pin);
    ezplib_replace_attr("wan_wwan_rule", idx, "username", username);
    ezplib_replace_attr("wan_wwan_rule", idx, "passwd", passwd);
    ezplib_replace_attr("wan_wwan_rule", idx, "dialstr", dialstr);
    
    //preaction
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    
    change = 1;
    return change;

#endif
}

//aron patch for giga
static int
_save_wan_static(webs_t wp, char *value)
{
    char tmp[TMP_LEN], buf[TMP_LEN], buf_wins[TMP_LEN];
    char *ip, *mask, *gateway, *dns1, *dns2, *wins;
    int idx = atoi(value), len, change = 0;
    int ret; //aron add

  if(idx==0) //Normally, the nth for "0"
  {
    /* IP */
    snprintf(tmp, sizeof(tmp), "staticIp");
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "staticNetmask");
    mask = websGetVar(wp, tmp, "");

    /* Gateway */
    snprintf(tmp, sizeof(tmp), "staticGateway");
    gateway = websGetVar(wp, tmp, "");

    /* DNS 1 */
    snprintf(tmp, sizeof(tmp), "staticPriDns");
    dns1 = websGetVar(wp, tmp, "");

    /* DNS 2 */
    snprintf(tmp, sizeof(tmp), "staticSecDns");
    dns2 = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* IP */
    snprintf(tmp, sizeof(tmp), "staticIp_%d", idx);
    ip = websGetVar(wp, tmp, "");

    /* Netmask */
    snprintf(tmp, sizeof(tmp), "staticNetmask_%d", idx);
    mask = websGetVar(wp, tmp, "");

    /* Gateway */
    snprintf(tmp, sizeof(tmp), "staticGateway_%d", idx);
    gateway = websGetVar(wp, tmp, "");

    /* DNS 1 */
    snprintf(tmp, sizeof(tmp), "staticPriDns_%d", idx);
    dns1 = websGetVar(wp, tmp, "");

    /* DNS 2 */
    snprintf(tmp, sizeof(tmp), "staticSecDns_%d", idx);
    dns2 = websGetVar(wp, tmp, "");
  }
    
    /* Not configured by UI, just retrieve it by nvram to make the whole 
       rule complete in order to compare with or replace old rule */
    /* Wins */
    ret = ezplib_get_attr_val("wan_static_rule", idx, "wins", buf, TMP_LEN, EZPLIB_USE_CLI);
    strcpy(buf_wins, buf);
    wins = buf_wins;
    
    
    //Translate AboCom's netmask to AXIM's nvram format
    mask = mask2prefix(mask);
    
    /* TODO: keep consistent with nvram. */
    len = 
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^^%s", ip, mask, gateway, 
                dns1, dns2, wins);

    if (len > TMP_LEN - 1) {
        return 0;
    }

    //preaction
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    ezplib_replace_rule("wan_static_rule", idx, tmp);
    change = 1; 

    return change;
}

//aron patch for giga
static int
_save_wan_main(webs_t wp, char *value)
{
    char tmp[TMP_LEN], tmp_mtu[TMP_LEN], buf[TMP_LEN];
    char *enable, *mtu, *stp;
    int idx = atoi(value), len, change = 0;

  if(idx==0) //Normally, the nth for "0"
  {
    /* Enable */
    snprintf(tmp, sizeof(tmp), "wanEnable");
    enable = websGetVar(wp, tmp, "");
    
    /* MTU */
    snprintf(tmp, sizeof(tmp), "wanMtu");
    mtu = websGetVar(wp, tmp, "");
    if (!*mtu) {
        ezplib_get_attr_val("wan_main_rule", 0, "mtu", tmp_mtu, TMP_LEN,
                EZPLIB_USE_WEB);
        mtu = tmp_mtu;
    }

    /* STP */
    snprintf(tmp, sizeof(tmp), "wanStp");
    stp = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* Enable */
    snprintf(tmp, sizeof(tmp), "wanEnable_%d", idx);
    enable = websGetVar(wp, tmp, "");
    
    /* MTU */
    snprintf(tmp, sizeof(tmp), "wanMtu_%d", idx);
    mtu = websGetVar(wp, tmp, "");
    if (!*mtu) {
        ezplib_get_attr_val("wan_main_rule", idx, "mtu", tmp_mtu, TMP_LEN,
                EZPLIB_USE_WEB);
        mtu = tmp_mtu;
    }

    /* STP */
    snprintf(tmp, sizeof(tmp), "wanStp_%d", idx);
    stp = websGetVar(wp, tmp, "");
  }
    /* Always turn on WAN upnp. Only for 3G/4G */
    len = snprintf(tmp, TMP_LEN, "WAN%d^%s^%s^%s^1", 
            idx+1, enable, mtu, stp);
    if (len >= TMP_LEN) {
        return 0;
    }

    //preaction
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    ezplib_replace_rule("wan_main_rule", idx, tmp);
    change = 1;

    return change; 
}

//aron patch for giga
#if 0
static int
_save_wan_bigpond(webs_t wp, char *value)
{
    char tmp[TMP_LEN];
    char *enable, *server, *username, *passwd;
    int idx = atoi(value), len, change = 0;

  enable = "1"; //always enabled
  
  if(idx==0) //Normally, the nth for "0"
  {
    /* Server */
    snprintf(tmp, sizeof(tmp), "bigPondSrv");
    server = websGetVar(wp, tmp, "");
    
    /* Username */
    snprintf(tmp, sizeof(tmp), "bigPondUser");
    username = websGetVar(wp, tmp, "");

    /* Password */
    snprintf(tmp, sizeof(tmp), "bigPondPass");
    passwd = websGetVar(wp, tmp, "");
  }
  else //increased nth for "1", "2",...etc.
  {
    /* Server */
    snprintf(tmp, sizeof(tmp), "bigPondSrv_%d", idx);
    server = websGetVar(wp, tmp, "");
    
    /* Username */
    snprintf(tmp, sizeof(tmp), "bigPondUser_%d", idx);
    username = websGetVar(wp, tmp, "");

    /* Password */
    snprintf(tmp, sizeof(tmp), "bigPondPass_%d", idx);
    passwd = websGetVar(wp, tmp, "");
  }
  
    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s", 
            enable, server, username, passwd);
    if (len >= TMP_LEN) {
        return 0;
    }

    //preaction
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    ezplib_replace_rule("wan_bigpond_rule", idx, tmp);
    change = 1;

    return change; 
}
#endif

//aron patch for giga
static int
_save_wan_pppoe(webs_t wp, char *value)
{
    char tmp[TMP_LEN], buf[TMP_LEN], buf_auth[TMP_LEN],
           buf_echo[TMP_LEN], buf_redial[TMP_LEN], buf_idle[TMP_LEN];
    char *username, *passwd, *demand, *idletime, *redialperiod, *mtu,
         *echo_failure, *auth, *pppIp_type, *pppIp_addr, *srvname;
    int idx = atoi(value), len, val, change = 0;
    int ret, demand_index;

 if(idx==0) //Normally, the nth for "0"
 {
    /* User Name */
    snprintf(tmp, sizeof(tmp), "pppoeUser");
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "pppoePass");
    passwd = websGetVar(wp, tmp, "");
    
    /* MRU & MTU */
    snprintf(tmp, sizeof(tmp), "pppoeMtu");
    mtu = websGetVar(wp, tmp, "");
    val = atoi(mtu);
    
    /* Demand */
    snprintf(tmp, sizeof(tmp), "pppoeNailedup");
    demand = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "pppoeIdleTime");
    idletime = websGetVar(wp, tmp, "");
    
    /* PPP IP Type */
    snprintf(tmp, sizeof(tmp), "pppoeWanIPMode");
    pppIp_type = websGetVar(wp, tmp, "");
    
    /* PPP IP Addr */
    snprintf(tmp, sizeof(tmp), "pppoeWanIpAddr");
    pppIp_addr = websGetVar(wp, tmp, "");
    
    /* Service Name */
    snprintf(tmp, sizeof(tmp), "pppoeSrvName");
    srvname = websGetVar(wp, tmp, "");
 }
 else //increased nth for "1", "2",...etc.
 {
    /* User Name */
    snprintf(tmp, sizeof(tmp), "pppoeUser_%d", idx);
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "pppoePass_%d", idx);
    passwd = websGetVar(wp, tmp, "");
    
    /* MRU & MTU */
    snprintf(tmp, sizeof(tmp), "pppoeMtu_%d", idx);
    mtu = websGetVar(wp, tmp, "");
    val = atoi(mtu);
    
    /* Demand */
    snprintf(tmp, sizeof(tmp), "pppoeNailedup_%d", idx);
    demand = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "pppoeIdleTime_%d", idx);
    idletime = websGetVar(wp, tmp, "");
    
    /* PPP IP Type */
    snprintf(tmp, sizeof(tmp), "pppoeWanIPMode_%d", idx);
    pppIp_type = websGetVar(wp, tmp, "");
    
    /* PPP IP Addr */
    snprintf(tmp, sizeof(tmp), "pppoeWanIpAddr_%d", idx);
    pppIp_addr = websGetVar(wp, tmp, "");
    
    /* Service Name */
    snprintf(tmp, sizeof(tmp), "pppoeSrvName_%d", idx);
    srvname = websGetVar(wp, tmp, "");
 }
    
    /* Not configured by UI, just retrieve it by nvram to make the whole 
       rule complete in order to compare with or replace old rule */
    /* Auth */
    ret = ezplib_get_attr_val("wan_pppoe_rule", idx, "auth", buf_auth, TMP_LEN, EZPLIB_USE_CLI);
    auth = buf_auth;
    
    /* Echo Failure Number */
    ret = ezplib_get_attr_val("wan_pppoe_rule", idx, "echo_failure", buf_echo, TMP_LEN, EZPLIB_USE_CLI);
    echo_failure = buf_echo;
    
    /* Redial Period */
    ret = ezplib_get_attr_val("wan_pppoe_rule", idx, "redialperiod", buf_redial, TMP_LEN, EZPLIB_USE_CLI);
    redialperiod = buf_redial;
    
    //Translate AboCom's nailup to AXIM's nvram format
    demand_index=atoi(demand);
    if(demand_index==1) //Abocom treat nailup "checked" as "1"
    {
    	demand="0";
    	//We don't need to update idletime value from UI when nailup is checked
    	ret = ezplib_get_attr_val("wan_pppoe_rule", idx, "idletime", buf_idle, TMP_LEN, EZPLIB_USE_CLI);
	idletime = buf_idle;
    }
    else if(demand_index==0)
    {
        demand="1";
    }
    
    if(!strcmp(pppIp_type, "1"))
        pppIp_type = "isp";
    else if(!strcmp(pppIp_type, "0"))
        pppIp_type = "custom";
    
    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%d^%d^%s^%s^%s", 
            auth, username, passwd, demand, idletime, echo_failure,
            redialperiod, val, val, pppIp_type, pppIp_addr, srvname);
    
    if (len >= TMP_LEN) {
        return 0;
    }

    //preaction
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    ezplib_replace_rule("wan_pppoe_rule", idx, tmp);
    change = 1;

    return change;
}

//aron patch for giga
static int
_save_wan_pptp(webs_t wp, char *value)
{
    char tmp[TMP_LEN], buf[TMP_LEN], buf_idle[TMP_LEN];
    char *username, *passwd, *nailup, *idletime, *server, *physicalIp_type,
         *physicalIp_addr, *physicalIp_netmask, *physicalIp_gateway,
         *pppIp_type, *pppIp_addr;
    int idx = atoi(value), nailup_index, len, change = 0;
    char *encmode;
    
    encmode = "disabled"; //always disabled

 if(idx==0) //Normally, the nth for "0"
 {
    /* User Name */
    snprintf(tmp, sizeof(tmp), "pptpUser");
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "pptpPass");
    passwd = websGetVar(wp, tmp, "");
    
    /* Nailup */
    snprintf(tmp, sizeof(tmp), "pptpNailup");
    nailup = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "pptpIdleTime");
    idletime = websGetVar(wp, tmp, "");
    
    /* Server IP */
    snprintf(tmp, sizeof(tmp), "pptpServer");
    server = websGetVar(wp, tmp, "");
    
    /* Physical IP Type */
    snprintf(tmp, sizeof(tmp), "pptpMode");
    physicalIp_type = websGetVar(wp, tmp, "");
    
    /* Physical IP Addr */
    snprintf(tmp, sizeof(tmp), "pptpIp");
    physicalIp_addr = websGetVar(wp, tmp, "");
    
    /* Physical IP Netmask */
    snprintf(tmp, sizeof(tmp), "pptpNetmask");
    physicalIp_netmask = websGetVar(wp, tmp, "");
    
    /* Physical IP Gateway */
    snprintf(tmp, sizeof(tmp), "pptpGateway");
    physicalIp_gateway = websGetVar(wp, tmp, "");
    
    /* PPP IP Type */
    snprintf(tmp, sizeof(tmp), "pptpWanIPMode");
    pppIp_type = websGetVar(wp, tmp, "");
    
    /* PPP IP Addr */
    snprintf(tmp, sizeof(tmp), "pptpWanIpAddr");
    pppIp_addr = websGetVar(wp, tmp, "");
 }
 else //increased nth for "1", "2",...etc.
 {
    /* User Name */
    snprintf(tmp, sizeof(tmp), "pptpUser_%d", idx);
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "pptpPass_%d", idx);
    passwd = websGetVar(wp, tmp, "");
    
    /* Nailup */
    snprintf(tmp, sizeof(tmp), "pptpNailup_%d", idx);
    nailup = websGetVar(wp, tmp, "");

    /* Idle Time */
    snprintf(tmp, sizeof(tmp), "pptpIdleTime_%d", idx);
    idletime = websGetVar(wp, tmp, "");
    
    /* Server IP */
    snprintf(tmp, sizeof(tmp), "pptpServer_%d", idx);
    server = websGetVar(wp, tmp, "");
    
    /* Physical IP Type */
    snprintf(tmp, sizeof(tmp), "pptpMode_%d", idx);
    physicalIp_type = websGetVar(wp, tmp, "");
    
    /* Physical IP Addr */
    snprintf(tmp, sizeof(tmp), "pptpIp_%d", idx);
    physicalIp_addr = websGetVar(wp, tmp, "");
    
    /* Physical IP Netmask */
    snprintf(tmp, sizeof(tmp), "pptpNetmask_%d", idx);
    physicalIp_netmask = websGetVar(wp, tmp, "");
    
    /* Physical IP Gateway */
    snprintf(tmp, sizeof(tmp), "pptpGateway_%d", idx);
    physicalIp_gateway = websGetVar(wp, tmp, "");
    
    /* PPP IP Type */
    snprintf(tmp, sizeof(tmp), "pptpWanIPMode_%d", idx);
    pppIp_type = websGetVar(wp, tmp, "");
    
    /* PPP IP Addr */
    snprintf(tmp, sizeof(tmp), "pptpWanIpAddr_%d", idx);
    pppIp_addr = websGetVar(wp, tmp, "");
 }
     
    //Translate AboCom's UI value to AXIM's nvram format
    physicalIp_netmask = mask2prefix(physicalIp_netmask);
    
    nailup_index=atoi(nailup);
    if(nailup_index==1) //Abocom treat nailup "checked" as "1"
    {
    	nailup="1";
    	//We don't need to update idletime value from UI when nailup is checked
    	ezplib_get_attr_val("wan_pptp_l2tp_rule", idx, "timeout", buf_idle, TMP_LEN, EZPLIB_USE_CLI);
	idletime = buf_idle;
    }
    else if(nailup_index==0)
    {
        nailup="0";
    }
    
    if(!strcmp(physicalIp_type, "1"))
        physicalIp_type = "dhcp";
    else if(!strcmp(physicalIp_type, "0"))
        physicalIp_type = "static";
        
    if(!strcmp(pppIp_type, "1"))
        pppIp_type = "isp";
    else if(!strcmp(pppIp_type, "0"))
        pppIp_type = "custom";
   
    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s", 
            server, physicalIp_type, physicalIp_addr, physicalIp_netmask,
            physicalIp_gateway, username, passwd, nailup, idletime,
            pppIp_type, pppIp_addr, encmode);
            
    if (len >= TMP_LEN) {
        return 0;
    }

    //preaction
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    ezplib_replace_rule("wan_pptp_l2tp_rule", idx, tmp);
    change = 1;

    return change;
}

//aron patch for giga
static int
_save_wan_l2tp(webs_t wp, char *value)
{
    char tmp[TMP_LEN], buf[TMP_LEN], buf_nailup[TMP_LEN], buf_idle[TMP_LEN];
    char *username, *passwd, *nailup, *idletime, *server, *physicalIp_type,
         *physicalIp_addr, *physicalIp_netmask, *physicalIp_gateway,
         *pppIp_type, *pppIp_addr;
    int idx = atoi(value), len, change = 0;
    char *encmode;
    
    encmode = "disabled"; //always disabled

 if(idx==0) //Normally, the nth for "0"
 {
    /* User Name */
    snprintf(tmp, sizeof(tmp), "l2tpUser");
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "l2tpPass");
    passwd = websGetVar(wp, tmp, "");
    
    /* Server IP */
    snprintf(tmp, sizeof(tmp), "l2tpServer");
    server = websGetVar(wp, tmp, "");
    
    /* Physical IP Type */
    snprintf(tmp, sizeof(tmp), "l2tpMode");
    physicalIp_type = websGetVar(wp, tmp, "");
    
    /* Physical IP Addr */
    snprintf(tmp, sizeof(tmp), "l2tpIp");
    physicalIp_addr = websGetVar(wp, tmp, "");
    
    /* Physical IP Netmask */
    snprintf(tmp, sizeof(tmp), "l2tpNetmask");
    physicalIp_netmask = websGetVar(wp, tmp, "");
    
    /* Physical IP Gateway */
    snprintf(tmp, sizeof(tmp), "l2tpGateway");
    physicalIp_gateway = websGetVar(wp, tmp, "");
    
    /* PPP IP Type */
    snprintf(tmp, sizeof(tmp), "l2tpWanIPMode");
    pppIp_type = websGetVar(wp, tmp, "");
    
    /* PPP IP Addr */
    snprintf(tmp, sizeof(tmp), "l2tpWanIpAddr");
    pppIp_addr = websGetVar(wp, tmp, "");
 }
 else //increased nth for "1", "2",...etc.
 {
    /* User Name */
    snprintf(tmp, sizeof(tmp), "l2tpUser_%d", idx);
    username = websGetVar(wp, tmp, "");

    /* Passwd */
    snprintf(tmp, sizeof(tmp), "l2tpPass_%d", idx);
    passwd = websGetVar(wp, tmp, "");
    
    /* Server IP */
    snprintf(tmp, sizeof(tmp), "l2tpServer_%d", idx);
    server = websGetVar(wp, tmp, "");
    
    /* Physical IP Type */
    snprintf(tmp, sizeof(tmp), "l2tpMode_%d", idx);
    physicalIp_type = websGetVar(wp, tmp, "");
    
    /* Physical IP Addr */
    snprintf(tmp, sizeof(tmp), "l2tpIp_%d", idx);
    physicalIp_addr = websGetVar(wp, tmp, "");
    
    /* Physical IP Netmask */
    snprintf(tmp, sizeof(tmp), "l2tpNetmask_%d", idx);
    physicalIp_netmask = websGetVar(wp, tmp, "");
    
    /* Physical IP Gateway */
    snprintf(tmp, sizeof(tmp), "l2tpGateway_%d", idx);
    physicalIp_gateway = websGetVar(wp, tmp, "");
    
    /* PPP IP Type */
    snprintf(tmp, sizeof(tmp), "l2tpWanIPMode_%d", idx);
    pppIp_type = websGetVar(wp, tmp, "");
    
    /* PPP IP Addr */
    snprintf(tmp, sizeof(tmp), "l2tpWanIpAddr_%d", idx);
    pppIp_addr = websGetVar(wp, tmp, "");
 }
     
    /* Not configured by UI, just retrieve it by nvram to make the whole 
       rule complete in order to compare with or replace old rule */
    /* nailup */
    ezplib_get_attr_val("wan_pptp_l2tp_rule", idx, "nailup", buf_nailup, TMP_LEN, EZPLIB_USE_CLI);
    nailup = buf_nailup;
    
    /* idletime */
    ezplib_get_attr_val("wan_pptp_l2tp_rule", idx, "timeout", buf_idle, TMP_LEN, EZPLIB_USE_CLI);
    idletime = buf_idle;
    
    //Translate AboCom's UI value to AXIM's nvram format
    physicalIp_netmask = mask2prefix(physicalIp_netmask);
     
    if(!strcmp(physicalIp_type, "1"))
        physicalIp_type = "dhcp";
    else if(!strcmp(physicalIp_type, "0"))
        physicalIp_type = "static";
        
    if(!strcmp(pppIp_type, "1"))
        pppIp_type = "isp";
    else if(!strcmp(pppIp_type, "0"))
        pppIp_type = "custom";
   
    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s", 
            server, physicalIp_type, physicalIp_addr, physicalIp_netmask,
            physicalIp_gateway, username, passwd, nailup, idletime,
            pppIp_type, pppIp_addr, encmode);
            
    if (len >= TMP_LEN) {
        return 0;
    }

    //preaction ??
    //snprintf(buf, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", idx);
    //system(buf);
    //system("ifdown wan0");
    ezplib_replace_rule("wan_pptp_l2tp_rule", idx, tmp);
    change = 1;

    return change;
}

//aron patch for giga
int
_save_wan_clone(webs_t wp, char *value)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *cloneType, *cloneMacAddr, *cloneIPAddr;
    int idx = atoi(value), len, change = 0;
    
    if(idx==0) //Normally, the nth for "0"
    {
       snprintf(tmp, sizeof(tmp), "WAN_MAC_Clone");
       cloneType = websGetVar(wp, tmp, "");
    
       snprintf(tmp, sizeof(tmp), "macCloneMac");
       cloneMacAddr = websGetVar(wp, tmp, "");
       
       snprintf(tmp, sizeof(tmp), "spoofIPAddr");
       cloneIPAddr = websGetVar(wp, tmp, "");
    }
    else //increased nth for "1", "2",...etc.
    {
       snprintf(tmp, sizeof(tmp), "WAN_MAC_Clone_%d", idx);
       cloneType = websGetVar(wp, tmp, "");
    
       snprintf(tmp, sizeof(tmp), "macCloneMac_%d", idx);
       cloneMacAddr = websGetVar(wp, tmp, "");
       
       snprintf(tmp, sizeof(tmp), "spoofIPAddr_%d", idx);
       cloneIPAddr = websGetVar(wp, tmp, "");
    }
     
    if (!strcmp(cloneType, "0")) //factory default
    {	
    	//preaction ??
        ezplib_replace_attr("wan_hwaddr_clone_rule", idx, "enable", cloneType);
        change = 1;
    }
    else if (!strcmp(cloneType, "1")) //manual ip address
    {
    	//preaction ??
        ezplib_replace_attr("wan_hwaddr_clone_rule", idx, "enable", "1");
    	ezplib_replace_attr("wan_hwaddr_clone_rule", idx, "addrtype", "ipaddr");
    	ezplib_replace_attr("wan_hwaddr_clone_rule", idx, "ipaddr", cloneIPAddr);
        change = 1;
    }
    else if (!strcmp(cloneType, "2")) //manual mac address
    {
        //preaction ??
        ezplib_replace_attr("wan_hwaddr_clone_rule", idx, "enable", "1");
    	ezplib_replace_attr("wan_hwaddr_clone_rule", idx, "addrtype", "hwaddr");
    	ezplib_replace_attr("wan_hwaddr_clone_rule", idx, "hwaddr", cloneMacAddr);
        change = 1;
    }
    
    //postaction ??
    return change;
}

//Only used for wan.asp
static void setWanNormal(webs_t wp, char_t *path, char_t *query)
{
        char *cur = nvram_safe_get("wan_num");
        char tmp[TMP_LEN], tmp1[TMP_LEN];
        char buf1[TMP_LEN], buf2[TMP_LEN], buf3[TMP_LEN];
        char *enable, *proto, *wan_dhcp_fixedip;
        int num;
        int i, change = 0;
        int64_t map;

        if (!*cur) {
             num = 1;
        } else {
             num = atoi(cur);
        }

        for (i = 0; i < num; i++) {
             map = 0;

             snprintf(buf3, sizeof(tmp), "%d", i);
             enable="1"; //We always treat WAN as enabled
             
             if (*enable == '1') 
             {
                      if(i==0) //Normally, the nth for "0"
                      {
                         snprintf(tmp, sizeof(tmp), "connectionType");
                         snprintf(tmp1, sizeof(tmp1), "WAN_IP_Auto");
                      }
                      else  //increased nth for "1", "2",...etc.
                      {
                         snprintf(tmp, sizeof(tmp), "connectionType_%d", i);
                         snprintf(tmp1, sizeof(tmp1), "WAN_IP_Auto_%d", i);
                      }
                      proto = websGetVar(wp, tmp, "");
                      wan_dhcp_fixedip = websGetVar(wp, tmp1, "");
                      
                      //Translate AboCom' UI value to conform with AXIM's nvram format at first
                      if(!strcmp(proto, "Ethernet"))
                      {  if (!strcmp(wan_dhcp_fixedip, "0")) //DHCP
	                     proto = "dhcp";
	                 else if (!strcmp(wan_dhcp_fixedip, "1")) //STATIC IP
	                     proto = "static";
                      }
                      else if(!strcmp(proto, "PPPOE"))  
                             proto = "pppoe";
                      else if(!strcmp(proto, "PPTP"))  
                             proto = "pptp";
                      else if(!strcmp(proto, "L2TP"))  
                             proto = "l2tp";
                      else if(!strcmp(proto, "WWAN")) //3G
                             proto = "wwan";
                      
                      snprintf(tmp, sizeof(tmp), "wan%d_proto", i); //Translate back to wan%d_proto for setting nvram
                      if (strcmp(proto, nvram_safe_get(tmp))) //if new UI and current nvram value is not equal
                      {
                          //preaction
                          //snprintf(buf1, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", i);
                          //system(buf1);
                          //system("ifdown wan0");
                          
                          nvram_set(tmp, proto);

                          /* Set ifname based on proto */
                          snprintf(buf1, sizeof(buf1), "wan%d_ifname", i);
                          if (!strcmp(proto, "static")) 
                          {
                             snprintf(buf2, sizeof(buf2), "wan%d_device", i);
                             nvram_set(buf1, nvram_safe_get(buf2));
                          }
                          else if (!strcmp(proto, "dhcp")) 
                          {
                             snprintf(buf2, sizeof(buf2), "wan%d_device", i);
                             nvram_set(buf1, nvram_safe_get(buf2));
                          } 
                          else if (!strcmp(proto, "pppoe")) 
                          {
                             snprintf(buf2, sizeof(buf2), "ppp%d", i);
                             nvram_set(buf1, buf2);
                          }
                          else if (!strcmp(proto, "wwan")) 
                          {
                             snprintf(buf2, sizeof(buf2), "ppp%d", i);
                             nvram_set(buf1, buf2);
                          } 
                          else if (!strcmp(proto, "pptp"))
                          {
                             snprintf(buf2, sizeof(buf2), "ppp%d", i + atoi(nvram_safe_get("wan_num")));
                             nvram_set(buf1, buf2);
                          }
                          else if (!strcmp(proto, "l2tp"))
                          {
                             snprintf(buf2, sizeof(buf2), "ppp%d", i + atoi(nvram_safe_get("wan_num")));
                             nvram_set(buf1, buf2);
                          }
                          else 
                          {
                             websWrite(wp, T("<h2>Unknown Connection Type: %s</h2><br>\n"), proto);
                          }
                          change = 1;

                          /* Changing from DHCP, the IP will reside in wan0_ipaddr until 
                           * accquiring the new IP address.
                           */
                          snprintf(tmp, sizeof(tmp), "wan%d_ipaddr", i);
                          nvram_unset(tmp);
                      }
                      if (!strcmp(proto, "static")) 
                      {
                          if (_save_wan_static(wp, buf3))
                          {
                             change = 1;
                          }
                      } 
                      else if (!strcmp(proto, "dhcp")) 
                      {
                          if (_save_wan_dhcp(wp, buf3))
                          {
                             change = 1;
                          }
                      }
                      else if (!strcmp(proto, "pppoe"))
                      {
                          if (_save_wan_pppoe(wp, buf3)) 
                          {
                             change = 1;
                          }
                      }
                      else if (!strcmp(proto, "pptp"))
                      {
                          if (_save_wan_pptp(wp, buf3)) 
                          {
                             change = 1;
                          }
                      }
                      else if (!strcmp(proto, "l2tp"))
                      {
                          if (_save_wan_l2tp(wp, buf3)) 
                          {
                             change = 1;
                          }
                      }
                      else if (!strcmp(proto, "wwan"))
                      {
                          if (_save_wan_wwan(wp, buf3))  
                          {
                             change = 1;
                          }
                      }
                      else 
                      {
                         websWrite(wp, T("<h2>Unknown Connection Type: %s</h2><br>\n"), proto);
                      }
                      
                      //for clone mac
                      if (_save_wan_clone(wp, buf3)) 
                      {
                             change = 1;
                      }
                      
                      //for WAN dns settings
                      if (_save_wan_dns(wp, buf3)) 
                      {
                             change = 1;
                      }
             }
                  //postaction
                  //snprintf(buf1, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan start", i);
                  //system(buf1);
                  //system("ifup wan0");
        }
        
        nvram_commit();
        
   #if 1
        system("sleep 3 && reboot &");
	websHeader(wp);

	websWrite(wp, T("<script>\n"));	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
   #else   
        setWebMessage(0, NULL);
        char_t *submitUrl;
        submitUrl = websGetVar(wp, T("wan_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
   #endif
}

/* goform/setWan */
static void setWan(webs_t wp, char_t *path, char_t *query)
{
        char *cur = nvram_safe_get("wan_num");
        char tmp[TMP_LEN], tmp1[TMP_LEN];
        char buf1[TMP_LEN], buf2[TMP_LEN], buf3[TMP_LEN];
        char *enable, *proto, *wan_dhcp_fixedip;
        int num;
        int i, change = 0;
        int64_t map;

        if (!*cur) {
             num = 1;
        } else {
             num = atoi(cur);
        }

        for (i = 0; i < num; i++) {
             map = 0;

             //preaction
             system("ifdown wan0");
             
             snprintf(buf3, sizeof(tmp), "%d", i);
             enable="1"; //We always treat WAN as enabled
             
             if (*enable == '1') 
             {
                      if(i==0) //Normally, the nth for "0"
                      {
                         snprintf(tmp, sizeof(tmp), "connectionType");
                         snprintf(tmp1, sizeof(tmp1), "WAN_IP_Auto");
                      }
                      else  //increased nth for "1", "2",...etc.
                      {
                         snprintf(tmp, sizeof(tmp), "connectionType_%d", i);
                         snprintf(tmp1, sizeof(tmp1), "WAN_IP_Auto_%d", i);
                      }
                      proto = websGetVar(wp, tmp, "");
                      wan_dhcp_fixedip = websGetVar(wp, tmp1, "");
                      
                      //Translate AboCom' UI value to conform with AXIM's nvram format at first
                      if(!strcmp(proto, "Ethernet"))
                      {  if (!strcmp(wan_dhcp_fixedip, "0")) //DHCP
	                     proto = "dhcp";
	                 else if (!strcmp(wan_dhcp_fixedip, "1")) //STATIC IP
	                     proto = "static";
                      }
                      else if(!strcmp(proto, "PPPOE"))  
                             proto = "pppoe";
                      else if(!strcmp(proto, "PPTP"))  
                             proto = "pptp";
                      else if(!strcmp(proto, "L2TP"))  
                             proto = "l2tp";
                      else if(!strcmp(proto, "WWAN")) //3G
                             proto = "wwan";
                      
                      snprintf(tmp, sizeof(tmp), "wan%d_proto", i); //Translate back to wan%d_proto for setting nvram
                      if (strcmp(proto, nvram_safe_get(tmp))) //if new UI and current nvram value is not equal
                      {
                          //preaction
                          //snprintf(buf1, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan stop", i);
                          //system(buf1);
                          //system("ifdown wan0");
                          
                          nvram_set(tmp, proto);

                          /* Set ifname based on proto */
                          snprintf(buf1, sizeof(buf1), "wan%d_ifname", i);
                          if (!strcmp(proto, "static")) 
                          {
                             snprintf(buf2, sizeof(buf2), "wan%d_device", i);
                             nvram_set(buf1, nvram_safe_get(buf2));
                          }
                          else if (!strcmp(proto, "dhcp")) 
                          {
                             snprintf(buf2, sizeof(buf2), "wan%d_device", i);
                             nvram_set(buf1, nvram_safe_get(buf2));
                          } 
                          else if (!strcmp(proto, "pppoe")) 
                          {
                             snprintf(buf2, sizeof(buf2), "ppp%d", i);
                             nvram_set(buf1, buf2);
                          }
                          else if (!strcmp(proto, "wwan")) 
                          {
                             snprintf(buf2, sizeof(buf2), "ppp%d", i);
                             nvram_set(buf1, buf2);
                          } 
                          else if (!strcmp(proto, "pptp"))
                          {
                             snprintf(buf2, sizeof(buf2), "ppp%d", i + atoi(nvram_safe_get("wan_num")));
                             nvram_set(buf1, buf2);
                          }
                          else if (!strcmp(proto, "l2tp"))
                          {
                             snprintf(buf2, sizeof(buf2), "ppp%d", i + atoi(nvram_safe_get("wan_num")));
                             nvram_set(buf1, buf2);
                          }
                          else 
                          {
                             websWrite(wp, T("<h2>Unknown Connection Type: %s</h2><br>\n"), proto);
                          }
                          change = 1;

                          /* Changing from DHCP, the IP will reside in wan0_ipaddr until 
                           * accquiring the new IP address.
                           */
                          snprintf(tmp, sizeof(tmp), "wan%d_ipaddr", i);
                          nvram_unset(tmp);
                      }
                      if (!strcmp(proto, "static")) 
                      {
                          if (_save_wan_static(wp, buf3))
                          {
                             change = 1;
                          }
                      } 
                      else if (!strcmp(proto, "dhcp")) 
                      {
                          if (_save_wan_dhcp(wp, buf3))
                          {
                             change = 1;
                          }
                      }
                      else if (!strcmp(proto, "pppoe"))
                      {
                          if (_save_wan_pppoe(wp, buf3)) 
                          {
                             change = 1;
                          }
                      }
                      else if (!strcmp(proto, "pptp"))
                      {
                          if (_save_wan_pptp(wp, buf3)) 
                          {
                             change = 1;
                          }
                      }
                      else if (!strcmp(proto, "l2tp"))
                      {
                          if (_save_wan_l2tp(wp, buf3)) 
                          {
                             change = 1;
                          }
                      }
                      else if (!strcmp(proto, "wwan"))
                      {
                          if (_save_wan_wwan(wp, buf3))  
                          {
                             change = 1;
                          }
                      }
                      else 
                      {
                         websWrite(wp, T("<h2>Unknown Connection Type: %s</h2><br>\n"), proto);
                      }
                      
                      //for clone mac
                      /*
                      if (_save_wan_clone(wp, buf3)) 
                      {
                             change = 1;
                      }
                      */
                      
                      //for WAN dns settings
                      if (_save_wan_dns(wp, buf3)) 
                      {
                             change = 1;
                      }
             }
                  //postaction
                  //snprintf(buf1, TMP_LEN, "NUM=%d /etc/rc.common /etc/rc.d/S96network-wan start", i);
                  //system(buf1);
                  system("ifup wan0");
        }
        
        nvram_commit();
        
   #if 0
        system("sleep 3 && reboot &");
	websHeader(wp);

	websWrite(wp, T("<script>\n"));	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
   #else   
        setWebMessage(0, NULL);
        char_t *submitUrl;
        submitUrl = websGetVar(wp, T("wan_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
   #endif
}

/* goform/setWan *///Steve add
static void setWan_Simple(webs_t wp, char_t *path, char_t *query)
{     
	printf("Run setWan_Simple & sleep(5)\n");
	sleep(5);
    setWebMessage(0, NULL);
    char_t *submitUrl;
    submitUrl = websGetVar(wp, T("wan_url"), T(""));  // aron add for hidden page
	websRedirect(wp, submitUrl);
}

//aron modify for showing ppp connected IP including dial on-demand  2008.11.25
int isConnectPPP()
{
#if 0
	FILE *fs_ppplink;
	char str[2];
	if((fs_ppplink=fopen("/etc/link","r"))==NULL)
	    return 0;
	else
	{
	    fgets(str,2,fs_ppplink);
	    //printf("The str[0]:%c ,The str[1]:%c \n",str[0],str[1]);
	}
	fclose(fs_ppplink);
	if (str[0] == '1')
	     return 1;
	else
	     return 0;
#endif
}

//aron add  2009.01.16
int doRealDhcpIPRenew()
{
#if 0
	doSystem("killall -q udhcpd");
	doSystem("echo "" > /var/udhcpd.leases");
	doSystem("echo 1 > /var/resetPhy");
	doSystem("echo 1 > /var/realDhcpStartEnd");
	doSystem("config-udhcpd.sh -r");
	return 1;
#endif
}

//aron add  2009.03.19
static int doLanIPfunc(webs_t wp, char_t *path, char_t *query)
{
#if 0
    char  *opMode = nvram_bufget(RT2860_NVRAM, "OperationMode");
    
    if(strcmp(opMode, "0")!=0) /* WISP/Router mode */
    {     
      char *newDhcpEnabled = nvram_bufget(RT2860_NVRAM, "dhcpEnabled");
      if (!strcmp(newDhcpEnabled, "1")) {
        if(lanIPchanged == 1)  //aron add:dhcp gateway changed accordingly, do spoof dhcp start/end address at first  2009.01.16 
        {
           doSystem("echo 1 > /var/resetPhy");
           doSystem("echo 1 > /var/doSpoofFirst");
	}
      }
        
        //RL added for assure quota 2009/03/17
#ifdef CONFIG_RALINK_GPIO
		goaNotifyGpioForConntrack();
#endif
        
	doSystem("lan.sh");

      if (!strcmp(newDhcpEnabled, "1")) {
	 if(lanIPchanged == 1) //dhcp gateway changed
         {
	   //aron add:after sleep 7 secs, do real dhcp start/end address renew  2009.01.16
	   Sleep(7);
	   doRealDhcpIPRenew();
	   doSystem("echo 0.0.0.0 > /etc_ro/web/accessip"); //reset access ip 
         }
      }
	updateFlash8021x(RT2860_NVRAM);
	doSystem("ralink_init make_wireless_config rt2860");
	restart8021XDaemon(RT2860_NVRAM);

	firewall_init();
	management_init();
	RoutingInit();
//RL added for some rule ralted LAN IP, so we should run qos 2009/3/21 01:57pm
#ifdef CONFIG_RALINKAPP_SWQOS
	QoSInit(1);
#endif
	lanIPchanged=0;
    }
    else /* AP mode */
    {
    	//RL added for assure quota 2009/03/17
#ifdef CONFIG_RALINK_GPIO
		goaNotifyGpioForConntrack();
#endif
	
	doSystem("lan.sh");
	updateFlash8021x(RT2860_NVRAM);
	doSystem("ralink_init make_wireless_config rt2860");
	restart8021XDaemon(RT2860_NVRAM);

	//firewall_init(); //operate in Router/WISP mode only
	management_init(); //ntp, ddns, logd
	//RoutingInit(); //rip&zebra and static route
	
	doSystem("echo 0.0.0.0 > /etc_ro/web/accessip"); //reset access ip 
    }
#endif 
}

static int doIgmpSnoopfunc(webs_t wp, char_t *path, char_t *query)
{
      system("/sbin/ezp-setup-snoop");   
}

static int doLanDhcpfunc(webs_t wp, char_t *path, char_t *query)
{
      system("/usr/sbin/ezp-phy-rst 10");
      system("echo 0.0.0.0 > /web/accessip"); //reset access ip
}

static int doLanLeaseDnsfunc(webs_t wp, char_t *path, char_t *query)
{
      system("/usr/sbin/ezp-phy-rst 10");
      system("echo 0.0.0.0 > /web/accessip"); //reset access ip
}
#endif//Steve27

void formDefineInternet(void) {

#if 1	
	websAspDefine(T("getDhcpCliList"), getDhcpCliList);
	websAspDefine(T("getDns"), getDns);
	websAspDefine(T("getIpv6Dns"), getIpv6Dns);
	websAspDefine(T("getHostSupp"), getHostSupp);
	websAspDefine(T("getIfLiveWeb"), getIfLiveWeb);
	websAspDefine(T("getIfIsUpWeb"), getIfIsUpWeb);
	websAspDefine(T("getIgmpProxyBuilt"), getIgmpProxyBuilt);
	websAspDefine(T("getLanIp"), getLanIp);
	websAspDefine(T("getLanMac"), getLanMac);
#if 1//Arthur Chow 2009-01-03
	websAspDefine(T("getWlanMac"), getWlanMac);
#endif
	websAspDefine(T("getLanIfNameWeb"), getLanIfNameWeb);
	websAspDefine(T("getLanNetmask"), getLanNetmask);
	websAspDefine(T("getDnsmasqBuilt"), getDnsmasqBuilt);
	websAspDefine(T("getLltdBuilt"), getLltdBuilt);
	websAspDefine(T("getPppoeRelayBuilt"), getPppoeRelayBuilt);
	websAspDefine(T("getUpnpBuilt"), getUpnpBuilt);
	websAspDefine(T("getRadvdBuilt"), getRadvdBuilt);
	websAspDefine(T("getWanIp"), getWanIp);
	websAspDefine(T("getPPPoEMyWanIp"), getPPPoEMyWanIp); //aron add
	websAspDefine(T("getPPTPMyWanIp"), getPPTPMyWanIp); //aron add
	websAspDefine(T("getL2TPMyWanIp"), getL2TPMyWanIp); //aron add
	websAspDefine(T("getPhyWanIfIp"), getPhyWanIfIp); //aron add
	websAspDefine(T("getPhyWanIfNetMask"), getPhyWanIfNetMask); //aron add
	websAspDefine(T("getPhyWanIfGateway"), getPhyWanIfGateway); //aron add
	websAspDefine(T("getLanIfGateway"), getLanIfGateway); //aron patch for giga
	websAspDefine(T("getWanMac"), getWanMac);
	websAspDefine(T("getWanIfNameWeb"), getWanIfNameWeb);
	websAspDefine(T("getWanNetmask"), getWanNetmask);
	websAspDefine(T("getWanGateway"), getWanGateway);
	websAspDefine(T("startRoutercheck"), startRoutercheck);//Arthur Chow add
	websAspDefine(T("getRoutingTable"), getRoutingTable);
	websAspDefine(T("getStaticRoutingNumbers"), getStaticRoutingNumbers);  //aron add
	websAspDefine(T("getStorageBuilt"), getStorageBuilt);
	websAspDefine(T("getFtpBuilt"), getFtpBuilt);
	websAspDefine(T("getSmbBuilt"), getSmbBuilt);
	websAspDefine(T("getMediaBuilt"), getMediaBuilt);
	websFormDefine(T("reflashClientTbl"), reflashClientTbl);  //aron add
	websFormDefine(T("setLan"), setLan);
	websFormDefine(T("setLanIP"), setLanIP);  //aron add
	websAspDefine(T("doLanIPfunc"), doLanIPfunc);  //aron add
	websAspDefine(T("doLanDhcpfunc"), doLanDhcpfunc);  //aron add
	websAspDefine(T("doIgmpSnoopfunc"), doIgmpSnoopfunc);  //aron add
	websAspDefine(T("doLanLeaseDnsfunc"), doLanLeaseDnsfunc);  //aron add
	websFormDefine(T("setLanIPAlias"), setLanIPAlias);  //aron add
	websFormDefine(T("setLan_StaicLease_Dns"), setLan_StaicLease_Dns);  //aron add
	websFormDefine(T("setLanUPnP"), setLanUPnP);  //aron add
	websFormDefine(T("setIgmpSnooping"), setIgmpSnooping);  //aron add
	websFormDefine(T("setLanIGMP"), setLanIGMP);  //aron add
	websFormDefine(T("setVpnPaThru"), setVpnPaThru);
	websFormDefine(T("setWan"), setWan);
	websFormDefine(T("setWanNormal"), setWanNormal);
	websFormDefine(T("setLanDhcp"), setLanDhcp);  //aron add
	websFormDefine(T("getMyMAC"), getMyMAC);
	websFormDefine(T("addRouting"), addRouting);
	websFormDefine(T("delRouting"), delRouting);
	websFormDefine(T("dynamicRouting"), dynamicRouting);
	websFormDefine(T("ripRouting"), ripRouting);  //aron add
	websAspDefine(T("getDynamicRoutingBuilt"), getDynamicRoutingBuilt);
	websAspDefine(T("getSWQoSBuilt"), getSWQoSBuilt);
	websFormDefine(T("IPV6"), IPV6);  //nico add	
	websAspDefine(T("getLinkLocalAddr"), getLinkLocalAddr);
	websFormDefine(T("setWan_Simple"), setWan_Simple);//steve add
#endif	
}
