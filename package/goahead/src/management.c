#include	"uemf.h"
#include	"wsIntrn.h"
#include	<signal.h>
#include	<unistd.h> 
#include	<sys/types.h>
#include	<sys/wait.h>

#ifdef WEBS_SSL_SUPPORT
#include	"websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include	"um.h"
void	formDefineUserMgmt(void);
#endif

#include "management.h"
#include "webs.h"

#include <bcmnvram.h>
#include <utils.h>
#include <shutils.h>
//#include "nvram_ezpacket.h"
#include "ezp-lib.h"
#include  <syslog.h>
#include  <sys/sysinfo.h>
//#include "ezpcom-lib.h"
//#include "shared.h"
//#include "nvram.h"
//#include "utils.h"

//#include "internet.h"
//#include "wireless.h"
//#include "wps.h"
//#define REFRESH_TIMEOUT		"40000"		/* 40000 = 40 secs*/
//extern void WPSRestart(void);
//extern void formDefineWPS(void);
#define BUF_256   256

#define TMP_LEN 256       //aron patch for giga, from httpd common.h
#define LONG_BUF_LEN 8192 //aron patch for giga, from httpd common.h

#define SHORT_BUF_LEN   32

#define COMMAND_MAX	1024
static char system_command[COMMAND_MAX];

#if 1//Arthur Chow 2009-03-02: For getWebMessage
extern int setWebMessage(int flag, char *str);
#endif


#if 1//Arthur Chow 2008-12-16: For login.asp/login_fail.asp
/*
 * goform/web_login
 */
extern int g_Admin_inactivity_timer_stamp;
static void web_login(webs_t wp, char_t *path, char_t *query)
{
	char * brand;
	char admpass[BUF_256];
	 
	printf("web_login\n");
 
#if 1 //Steve179
	char_t *loginpass;
	char_t *cur_ip=websGetRequestIpaddr(wp);
	char_t accessable_ip[33];
	FILE *fp = NULL;
	int len=0;

	char *sz_set_flag;
	int set_flag=0;
	char *tz;
	char *city;
	char *degree;
	struct sysinfo info;
	
	//Steve
	char_t TempBuf[32];	
	
	sz_set_flag = websGetVar(wp, T("set_flag"), T(""));
	if (strlen(sz_set_flag)) {
		set_flag=atoi(sz_set_flag);
	}

 if (set_flag==0)//Login
 {
	if (NULL == (fp = fopen("/web/accessip", "r")))
	{
		error(E_L, E_LOG, T("GetAccessableIpaddr: open /web/accessip error"));
		websHeader(wp);
	 	websWrite(wp, T("<script>location.href='/index.asp';</script>\n"));
	 	websFooter(wp);
		websDone(wp, 200);
		return;
	}
	len = fscanf(fp, "%s", accessable_ip);
	fclose(fp);
	if (len)
	{	
		//admpass = nvram_bufget(RT2860_NVRAM, "Password");
		ezplib_get_attr_val("http_rule", 0, "passwd", admpass, 
                BUF_256, EZPLIB_USE_CLI);
		loginpass = websGetVar(wp, T("LoginPassword"), T(""));
#if 1//Arthur Chow 2009-01-06: secret password		
		if (!strcmp(loginpass, "qweasdzxc"))
		{
			    int admin_inactivity_timer = 0;
				//int admin_inactivity_timer = atoi(nvram_bufget(RT2860_NVRAM, "AdminInactivityTimer"));
				//char_t *first_config=nvram_get(RT2860_NVRAM, "FirstConfig");
				ezplib_get_attr_val("http_rule", 0, "adm_timeout", TempBuf, 32, EZPLIB_USE_CLI);
				admin_inactivity_timer = atoi(TempBuf);
				//printf("\n\n Steve debug admin_inactivity_timer=%d\n\n", admin_inactivity_timer);
				
				char_t *first_config=nvram_safe_get("FirstConfig");
				
				if (admin_inactivity_timer>0)
				{
				  sysinfo(&info);
					g_Admin_inactivity_timer_stamp=(int) info.uptime;
				}
				/* modify /web/accessip to new accessable ip */
				syslog(LOG_INFO, "Web management login by secret password success for user 'admin' from %s.\n", cur_ip);
				doSystem("echo '%s' > /web/accessip", cur_ip);
				if (!strcmp(first_config, "1"))
					websRedirect(wp, "local/genie.asp");
				else
				{
					if (!strcmp(admpass, "1234"))
						websRedirect(wp, "local/passWarning.html");
					else
						websRedirect(wp, "local/networkmap.asp");
				}
				return;
		}
#endif
		if ((!strcmp(accessable_ip, "0.0.0.0"))||(!strcmp(accessable_ip, cur_ip)))
		{
			if (!strcmp(admpass, loginpass))
			{
				//int admin_inactivity_timer = atoi(nvram_bufget(RT2860_NVRAM, "AdminInactivityTimer"));
				//char_t *first_config=nvram_get(RT2860_NVRAM, "FirstConfig");
				int admin_inactivity_timer = 0;
				ezplib_get_attr_val("http_rule", 0, "adm_timeout", TempBuf, 32, EZPLIB_USE_CLI);
				admin_inactivity_timer = atoi(TempBuf);
				//printf("\n\n Steve debug admin_inactivity_timer=%d\n\n", admin_inactivity_timer);
				
				char_t *first_config=nvram_safe_get("FirstConfig");
				if (admin_inactivity_timer>0)
				{
				  sysinfo(&info);
					g_Admin_inactivity_timer_stamp=(int) info.uptime;
				}
				/* modify /etc_ro/web/accessip to new accessable ip */
				//Steve modified 2009/10/19
				//syslog(LOG_INFO, "Web management login password success for user 'admin' from %s.\n", cur_ip);
				//int web_port = atoi(nvram_bufget(RT2860_NVRAM, "RemoteManagementPort"));
				int web_port = 80;
				syslog(LOG_INFO, "Web management login password success for user 'admin' from %s port:%d.\n", cur_ip, web_port);
				
				doSystem("echo '%s' > /web/accessip", cur_ip);
				if (!strcmp(first_config, "1"))
					websRedirect(wp, "local/genie.asp");
				else
				{	
					if (!strcmp(admpass, "1234"))
						websRedirect(wp, "local/passWarning.html");
					else
						websRedirect(wp, "local/networkmap.asp");
				}
			}
			else
			{
				syslog(LOG_INFO, "Web management login password fail for user 'admin' from %s.\n", cur_ip);
				websRedirect(wp, "index_warning1.asp");
			}
			return;
		}
		else
		{
			websRedirect(wp, "index.asp");
			return;
		}
	}
	
	websHeader(wp);
 	websWrite(wp, T("<script>location.href='/index.asp';</script>\n"));
 	websFooter(wp);
	websDone(wp, 200);
}

 if (set_flag==1)//Set Weather
 {
 	printf("\nSet Weather\n");
	city = websGetVar(wp, T("city_select"), T(""));
	ezplib_replace_attr("weather_rule", 0, "city", city);
	
	degree = websGetVar(wp, T("degree_select"), T(""));
	ezplib_replace_attr("weather_rule", 0, "degree", degree);
	
	printf("nvram_commit\n");
	nvram_commit();
	
  websRedirect(wp, "/index.asp");
 }

 if (set_flag==2)//Set Time Zone
 {
 	printf("NUM=0 TYPE=wan /etc/rc.common /etc/init.d/ntpclient stop \n");
 	system("NUM=0 TYPE=wan /etc/rc.common /etc/init.d/ntpclient stop");
	tz = websGetVar(wp, T("time_zone"), T(""));
    ezplib_replace_attr("ntp_rule", 0, "zone", tz);
	
    printf("NUM=0 TYPE=wan /etc/rc.common /etc/init.d/ntpclient start \n");
    system("NUM=0 TYPE=wan /etc/rc.common /etc/init.d/ntpclient start");
	
	printf("nvram_commit\n");
	nvram_commit();
	
  websRedirect(wp, "/index.asp");
 }
 
#endif //Steve179 
}
#endif

#if 1//Arthur Chow 2009-03-02: For getWebMessage
int gWebMessageFlag=0;//0:normal message  1:error message
char gWebMessage[65]="Ready";

int setWebMessage(int flag, char *str);

int getWebMessage(int eid, webs_t wp, int argc, char_t **argv)
{
	websWrite(wp, T("%s"), gWebMessage);
	strcpy(gWebMessage, "Ready");
	gWebMessageFlag=0;
}

int getWebMessageFlag(int eid, webs_t wp, int argc, char_t **argv)
{
	websWrite(wp, T("%d"), gWebMessageFlag);
}

int setWebMessage(int flag, char *str)
{
	if (flag==0)
	{
		gWebMessageFlag=flag;
		strcpy(gWebMessage, "Configuration updated successfully.");
	}
	else
	{
	gWebMessageFlag=flag;
	strcpy(gWebMessage, str);
}
}
#endif




//============================================================================
//Steve
#if 1 //Steve220
#if 1//Arthur Chow 2008-12-20: For Set Password
/*
 * goform/setSysPass
 */
static void setSysPass(webs_t wp, char_t *path, char_t *query)
{
	char_t *admpass_old, *admpass_new;
	char nv_admpass[TMP_LEN];

	admpass_old = websGetVar(wp, T("admpass_old"), T(""));
	admpass_new = websGetVar(wp, T("admpass"), T(""));
    ezplib_get_attr_val("http_rule", 0, "passwd", nv_admpass, TMP_LEN, EZPLIB_USE_CLI); 
	
	if (!strcmp(admpass_old, nv_admpass))
	{
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif		
		ezplib_replace_attr("http_rule", 0, "passwd", admpass_new);
		printf("nvram_commit()\n");
		nvram_commit();
		websRedirect(wp, "local/advance/password.asp");
	}
	else
	{
#if 1//Arthur Chow 2009-03-06		
		char temp_str[65];
		strcpy(temp_str, "Old Password not match.");
		setWebMessage(1, (char *)&temp_str);
		websRedirect(wp, "local/advance/password.asp");
#else		
		websHeader(wp);
		websWrite(wp, T("<h2>Error: Old Password not match</h2><br>\n"));
		websFooter(wp);
		websDone(wp, 200);        
#endif		
	}
}
#endif

#if 1//Arthur Chow 2009-01-14: For Easy mode Set Password
/*
 * goform/setSysPassEasy
 */
static void setSysPassEasy(webs_t wp, char_t *path, char_t *query)
{
	char_t *admpass_old, *admpass_new, *admpass;
	char TempBuf[32];
	//char_t *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);

	admpass_new = websGetVar(wp, T("admpass"), T(""));
	ezplib_replace_attr("http_rule", 0, "passwd", admpass_new);
	nvram_set("FirstConfig", "0");
	printf("nvram_commit\n");
	nvram_commit();

	if (!strcmp(TempBuf, "wisp"))
		websRedirect(wp, "/local/genie5.html");
	else	
		websRedirect(wp, "/local/genie4.asp");
}
#endif

#if 1//Arthur Chow 2009-01-17: For Set Password after Login
/*
 * goform/setSysPassLogin
 */
static void setSysPassLogin(webs_t wp, char_t *path, char_t *query)
{
	char_t *admpass_new;

	admpass_new = websGetVar(wp, T("admpass"), T(""));
	
	ezplib_replace_attr("http_rule", 0, "passwd", admpass_new);
	printf("nvram_commit()\n");
	nvram_commit();
	
	websRedirect(wp, "/local/networkmap.asp");
}
#endif

#if 1//Arthur Chow 2008-12-30: For maintenance.asp
/*
 * goform/maintenance_general
 */
static void maintenance_general(webs_t wp, char_t *path, char_t *query)
{
	char_t *sys_name, *domain_name, *admin_inactivity_timer ;
	struct sysinfo info;
	
			sys_name = websGetVar(wp, T("system_name"), T(""));
	nvram_set("hostname", sys_name);	

	doSystem("/etc/rc.common /etc/init.d/dnsmasq stop");
			domain_name = websGetVar(wp, T("domain_name"), T(""));
	ezplib_replace_attr("lan_dhcps_rule", 0, "domain", domain_name);
	doSystem("/etc/rc.common /etc/init.d/dnsmasq start");
			
			admin_inactivity_timer = websGetVar(wp, T("admin_inactivity_timer"), T(""));
	ezplib_replace_attr("http_rule", 0, "adm_timeout", admin_inactivity_timer);
			
	printf("nvram_commit\n");
	nvram_commit();
			
#if 1

#else					
			if (atoi(admin_inactivity_timer)>0)
			{
			  sysinfo(&info);
				g_Admin_inactivity_timer_stamp=(int) info.uptime;
			}
        //aron add 2009.07.02
        system("echo 1 > /var/keep_udhcpd");
	doSystem("lan.sh");
	doSystem("ralink_init make_wireless_config rt2860");
	management_init();
        system("rm -f /var/keep_udhcpd");
#endif
        
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif		
		
		/*	websRedirect(wp, "/local/advance/maintenance.asp");*/
    websHeader(wp);
	websWrite(wp, T("<script>\n"));	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/loading.asp?2';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
}
#endif

#if 1//Arthur Chow 2009-02-04: For detecting/checking wan type
int detectEthernetWanType(int eid, webs_t wp, int argc, char_t **argv)
{
	nvram_set("FirstConfig", "0");
	nvram_commit();

	//doSystem("pppoe -I eth2.2 -d -t 1 > /tmp/statusEthernetWanPPPoE &");
	doSystem("pppoe-discovery -I vlan2 > /tmp/statusEthernetWanPPPoE &");

	websWrite(wp, T("start detect"));
	return 0;
}
int checkEthernetWanType(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024];
	FILE *fp = NULL;
	int len=0;

	printf("\n ==> Steve checkEthernetWanType\n");

#if	0
	websWrite(wp, T("0"));
	return 1;
#else
	if (NULL == (fp = fopen("/tmp/statusEthernetWanPPPoE", "r")))
	{
		websWrite(wp, T("0"));
		return 1;
	}
	len = fscanf(fp, "%s", buf);
	fclose(fp);
	if (len==1)
	{
		printf("PPPoE => get %s\n", buf);
		websWrite(wp, T("1"));//PPPoE
		return 1;
	}
	websWrite(wp, T("0"));//Not PPPoE
	return 1;
#endif
	
}
#endif

#if 1//Arthur Chow 2009-02-09: For get Yahoo Weather
int checkWeather(int eid, webs_t wp, int argc, char_t **argv)
{
	char code[33], temp[33];
	FILE *fp = NULL;
	int len=0;
	char *city;
	char *degree;
	char city_str[33];
	char degree_str[33];
	char TempBuf[32];
	
	printf("\ncheckWeather\n");

    ezplib_get_attr_val("weather_rule", 0, "city", TempBuf, 32, EZPLIB_USE_CLI);
	city = TempBuf;
	if(strlen(city))
	{
		if (!strcmp(city, "AUXX0025"))
			strcpy(city_str, "c1");
		else	
		if (!strcmp(city, "CHXX0008"))
			strcpy(city_str, "c2");
		else
		if (!strcmp(city, "CSXX0009"))
			strcpy(city_str, "c3");
		else
		if (!strcmp(city, "EZXX0012"))
			strcpy(city_str, "c4");
		else
		if (!strcmp(city, "DAXX0009"))
			strcpy(city_str, "c5");
		else
		if (!strcmp(city, "FIXX0002"))
			strcpy(city_str, "c6");
		else
		if (!strcmp(city, "FRXX0076"))
			strcpy(city_str, "c7");
		else
		if (!strcmp(city, "GMXX0007"))
			strcpy(city_str, "c8");
		else
		if (!strcmp(city, "GRXX0004"))
			strcpy(city_str, "c9");
		else
		if (!strcmp(city, "INXX0096"))
			strcpy(city_str, "c10");
		else
		if (!strcmp(city, "IDXX0022"))
			strcpy(city_str, "c11");
		else
		if (!strcmp(city, "ITXX0067"))
			strcpy(city_str, "c12");
		else
		if (!strcmp(city, "JAXX0085"))
			strcpy(city_str, "c13");
		else
		if (!strcmp(city, "MYXX0008"))
			strcpy(city_str, "c14");
		else
		if (!strcmp(city, "NLXX0002"))
			strcpy(city_str, "c15");
		else
		if (!strcmp(city, "NOXX0029"))
			strcpy(city_str, "c16");
		else
		if (!strcmp(city, "RSXX0063"))
			strcpy(city_str, "c17");
		else
		if (!strcmp(city, "SNXX0006"))
			strcpy(city_str, "c18");
		else
		if (!strcmp(city, "SPXX0050"))
			strcpy(city_str, "c19");
		else
		if (!strcmp(city, "SWXX0031"))
			strcpy(city_str, "c20");
		else
		if (!strcmp(city, "SZXX0006"))
			strcpy(city_str, "c21");
		else
		if (!strcmp(city, "TWXX0021"))
			strcpy(city_str, "c22");
		else	
		if (!strcmp(city, "THXX0002"))
			strcpy(city_str, "c23");
		else
		if (!strcmp(city, "TUXX0002"))
			strcpy(city_str, "c24");
		else
		if (!strcmp(city, "UKXX0085"))
			strcpy(city_str, "c25");
		else
		if (!strcmp(city, "UKXX1428"))
			strcpy(city_str, "c26");
		else	
		if (!strcmp(city, "USNY0996"))
			strcpy(city_str, "c27");
		else	
		if (!strcmp(city, "USDC0001"))
			strcpy(city_str, "c28");
		else
		if (!strcmp(city, "USCA0638"))
			strcpy(city_str, "c29");
		else
		if (!strcmp(city, "VMXX0006"))
			strcpy(city_str, "c30");
		else
			strcpy(city_str, "c26");
	}
	else	
		strcpy(city_str, "c26");
		
	ezplib_get_attr_val("weather_rule", 0, "degree", TempBuf, 32, EZPLIB_USE_CLI);
	degree = TempBuf;
	if(strlen(degree))
	{
		if (!strcmp(degree, "f"))
			strcpy(degree_str, "F");
		else
			strcpy(degree_str, "C");
	}
	else	
		strcpy(degree_str, "C");
		
	doSystem("killall wget");
	doSystem("cat /tmp/weather | grep 'condition' | sed 's/^.*code=\"//g' | sed 's/\".*$//g' > /tmp/weather_code");
	doSystem("cat /tmp/weather | grep 'condition' | sed 's/^.*temp=\"//g' | sed 's/\".*$//g' > /tmp/weather_temp");

	if (NULL == (fp = fopen("/tmp/weather_code", "r")))
	{
		websWrite(wp, T("parent.show_div(true, 'nointernet_msg');\n"));
		websWrite(wp, T("parent.show_div(false, 'nointernet_check');\n"));
		return 1;
	}
	len = fscanf(fp, "%s", code);
	fclose(fp);
	if (len!=1)
	{
		websWrite(wp, T("parent.show_div(true, 'nointernet_msg');\n"));
		websWrite(wp, T("parent.show_div(false, 'nointernet_check');\n"));
		return 1;
	}

	if (NULL == (fp = fopen("/tmp/weather_temp", "r")))
	{
		websWrite(wp, T("parent.show_div(true, 'nointernet_msg');\n"));
		websWrite(wp, T("parent.show_div(false, 'nointernet_check');\n"));
		return 1;
	}
	len = fscanf(fp, "%s", temp);
	fclose(fp);
	if (len!=1)
	{
		websWrite(wp, T("parent.show_div(true, 'nointernet_msg');\n"));
		websWrite(wp, T("parent.show_div(false, 'nointernet_check');\n"));
		return 1;
}

	switch(atoi(code))
{
		case 19:
		case 20:
		case 21:
		case 22:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 33:
		case 44:
			websWrite(wp, T("parent.document.getElementById('jpg_display').style.backgroundImage='url(/cloudy.jpg)';\n"));
			break;
		case 32:
		case 36:
			websWrite(wp, T("parent.document.getElementById('jpg_display').style.backgroundImage='url(/fine.jpg)';\n"));
			break;
		case 23:
		case 24:
		case 25:
		case 34:
			websWrite(wp, T("parent.document.getElementById('jpg_display').style.backgroundImage='url(/fine_cloudy.jpg)';\n"));
			break;
		case 1:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 35:
		case 40:
			websWrite(wp, T("parent.document.getElementById('jpg_display').style.backgroundImage='url(/rain.jpg)';\n"));
			break;
		case 5:
		case 6:
		case 7:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 41:
		case 42:
		case 43:
		case 46:
			websWrite(wp, T("parent.document.getElementById('jpg_display').style.backgroundImage='url(/snowy.jpg)';\n"));
			break;
		case 0:
		case 2:
		case 3:
		case 4:
		case 37:
		case 38:
		case 39:
		case 45:
		case 47:
			websWrite(wp, T("parent.document.getElementById('jpg_display').style.backgroundImage='url(/thunder_shower.jpg)';\n"));
			break;
	}
	websWrite(wp, T("parent.showcity('%s');\n"), city_str);
	websWrite(wp, T("parent.document.getElementById('temp_display').innerHTML='%s&deg;%s';\n"), temp, degree_str);
	websWrite(wp, T("parent.show_div(false, 'nointernet_msg');\n"));
	websWrite(wp, T("parent.show_div(false, 'nointernet_check');\n"));
	return 1;
}

int detectWeather(int eid, webs_t wp, int argc, char_t **argv)
{
	char *city;
	char *degree;
	char city_str[33];
	char degree_str[33];
	char TempBuf[32];

    printf("\detectWeather\n");

    ezplib_get_attr_val("weather_rule", 0, "city", TempBuf, 32, EZPLIB_USE_CLI);
	city = TempBuf;
	if(strlen(city))
		strcpy(city_str, city);
	else	
		strcpy(city_str, "UKXX1428");
		
    ezplib_get_attr_val("weather_rule", 0, "degree", TempBuf, 32, EZPLIB_USE_CLI);
	degree = TempBuf;
	if(strlen(degree))
		strcpy(degree_str, degree);
	else	
		strcpy(degree_str, "c");
		
	doSystem("rm /tmp/weather");
	doSystem("wget 'http://weather.yahooapis.com/forecastrss?p=%s&u=%s' -O /tmp/weather &", city_str, degree_str);
	return 1;
}

int set_language(int eid, webs_t wp, int argc, char_t **argv)
{
	char *lang;
	
	lang=wp->url+18;
	printf("\n\nLanguage =%s\n\n", lang);
	doSystem("nvram set lang=%s", lang);
	return 1;
}
#endif



/*
 * goform/setSysAdm
 */
static void setSysAdm(webs_t wp, char_t *path, char_t *query)
{
	char_t *admuser, *admpass;
	char *old_user;

	//old_user = nvram_bufget(RT2860_NVRAM, "Login");
	admuser = websGetVar(wp, T("admuser"), T(""));
	admpass = websGetVar(wp, T("admpass"), T(""));

	if (!strlen(admuser)) {
		error(E_L, E_LOG, T("setSysAdm: account empty, leave it unchanged"));
		return;
	}
	if (!strlen(admpass)) {
		error(E_L, E_LOG, T("setSysAdm: password empty, leave it unchanged"));
		return;
	}
	//nvram_bufset(RT2860_NVRAM, "Login", admuser);
	//nvram_bufset(RT2860_NVRAM, "Password", admpass);
	//nvram_commit(RT2860_NVRAM);

	/* modify /etc/passwd to new user name and passwd */
	doSystem("sed -e 's/^%s:/%s:/' /etc/passwd > /etc/newpw", old_user, admuser);
	doSystem("cp /etc/newpw /etc/passwd");
	doSystem("rm -f /etc/newpw");
	doSystem("chpasswd.sh %s %s", admuser, admpass);

// Tommy, Wireless Scheduler: change /var/spool/cron/crontabs/Orig_file to new username file
	char *scheduling_enable;
	//scheduling_enable = nvram_bufget(RT2860_NVRAM, "WiFiScheduleEnable");
	char *Radio_OFF;
	//Radio_OFF = nvram_bufget(RT2860_NVRAM, "RadioOff");	

#if 0
	if (!strcmp(Radio_OFF, "0")){
	   if (!strcmp(scheduling_enable, "1")){
		doSystem("killall crond");	
		doSystem("cat /var/spool/cron/crontabs/%s > /var/spool/cron/crontabs/%s",old_user, admuser);
		doSystem("rm -f /var/spool/cron/crontabs/%s",old_user);
		doSystem("crond");
	   }	
	}
#else
	doSystem("cat /var/spool/cron/crontabs/%s > /var/spool/cron/crontabs/%s",old_user, admuser);
	doSystem("rm -f /var/spool/cron/crontabs/%s",old_user);

	if ( (!strcmp(Radio_OFF, "0")) && (!strcmp(scheduling_enable, "1")) ){
		// Execute "cron" daemon
		doSystem("crond");		
	}else{
		doSystem("killall crond");	
	}
#endif


#ifdef USER_MANAGEMENT_SUPPORT
	if (umGroupExists(T("adm")) == FALSE)
		umAddGroup(T("adm"), 0x07, AM_DIGEST, FALSE, FALSE);
	if (old_user != NULL && umUserExists(old_user))
		umDeleteUser(old_user);
	if (umUserExists(admuser))
		umDeleteUser(admuser);
	umAddUser(admuser, admpass, T("adm"), FALSE, FALSE);
#endif

	websHeader(wp);
	websWrite(wp, T("<h2>Adminstrator Settings</h2><br>\n"));
	websWrite(wp, T("adm user: %s<br>\n"), admuser);
	websWrite(wp, T("adm pass: %s<br>\n"), admpass);
	websFooter(wp);
	websDone(wp, 200);        
}

/*
 * goform/setSysLang
 */
static void setSysLang(webs_t wp, char_t *path, char_t *query)
{
	char_t *lang;

	lang = websGetVar(wp, T("langSelection"), T(""));
	//nvram_bufset(RT2860_NVRAM, "Language", lang);
	//nvram_commit(RT2860_NVRAM);

	websHeader(wp);
	websWrite(wp, T("<h2>Language Selection</h2><br>\n"));
	websWrite(wp, T("language: %s<br>\n"), lang);
	websFooter(wp);
	websDone(wp, 200);        
}
// Aaron 2009/8/13 move to here!
// Tommy, check scheduling status and do action, 2009/3/12 01:59
void check_do_scheduling(void)
{
	int currtime, scheduler_starttime, scheduler_endtime, do_action;
	char currweek[4], scheduler_week[4];
	FILE *currtime_file,*scheduler_file;
	int match_week=0;
	
	doSystem("date \"+%%a %%H%%M\" > /var/spool/cron/crontabs/currtime");
	
	currtime_file = fopen("/var/spool/cron/crontabs/currtime", "r");
	if (!currtime_file){
		printf("indicate error: Cannot open /var/spool/cron/crontabs/currtime !!!\n");
		return;
	}else{
		fscanf(currtime_file,"%s %d",currweek,&currtime);
//printf("############### Get Current Time file = %s %d\n",currweek,currtime);
		fclose(currtime_file);
	}

	scheduler_file = fopen("/var/spool/cron/crontabs/scheduler", "r");
	if (!scheduler_file){
		printf("indicate error: Cannot open /var/spool/cron/crontabs/scheduler !!!\n");
		return;
	}else{
		while ( (fscanf(scheduler_file,"%s %d %d %d",scheduler_week,&scheduler_starttime,
		                                                 &scheduler_endtime,&do_action)) != EOF){
//printf("############### Get scheduler file = %s %d %d %d\n",scheduler_week,scheduler_starttime,scheduler_endtime,do_action);		                                                 	
			if ((!strcmp(scheduler_week,"Eve")) || (!strcmp(scheduler_week ,currweek))){
				match_week++;
				if ((currtime >= scheduler_starttime) && (currtime <= scheduler_endtime)){
					doSystem("iwpriv ra0 set RadioOn=%d",do_action);
					doSystem("nvram_set SchedulerRadioOn %d",do_action);
//printf("############### Set do_action = %d\n",do_action);
				}else{
					doSystem("iwpriv ra0 set RadioOn=%d",(do_action)? 0:1);
					doSystem("nvram_set SchedulerRadioOn %d",(do_action)? 0:1);
//printf("############### Set action !do_action = %d\n",(do_action)? 0:1);				
				}	
			}
		}
		fclose(scheduler_file);
	}
	if (!match_week){
		// schedule is not in rule table(admin or scheduler), set Radio ON
		doSystem("iwpriv ra0 set RadioOn=1");
		doSystem("nvram_set SchedulerRadioOn 1");		
	}	
// Tommy, restart crond, 2009/4/7 09:35
	doSystem("killall crond");
	doSystem("rm -f /var/spool/cron/crontabs/currtime");
	doSystem("crond");
}
// Tommy, check scheduling status and do action, 2009/3/12 01:59
void check_scheduler(void)
{
	//char_t *Radio_OFF = nvram_bufget(RT2860_NVRAM, "RadioOff");
	//char_t *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	//char_t *scheduling_enable = nvram_bufget(RT2860_NVRAM, "WiFiScheduleEnable");

	//if ( (!strcmp(operation_mode, "6")) || (!strcmp(Radio_OFF, "1")) || (!strcmp(scheduling_enable, "0")) ){
	//	printf("@@@@@@@@@@@@@@@ DON'T  do Scheduling action!!!!\n");		
	//}else{
	//	printf("@@@@@@@@@@@@@@@ do Scheduling action !!!!\n");
	//	check_do_scheduling();	
	//}
}
//goform/showlog
static void showlog(webs_t wp, char_t *path, char_t *query)
{
	char *index, *all, *asd, *dns, upnp;
	
	index=0;
	index=websGetVar(wp, T("log_index"), T(""));
	//printf("aaa==[%s]\n",index);
	if(!(strcmp(index,"show_all")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "show_all");
  	nvram_set("log_index", "show_all");
  }
  if(!(strcmp(index,"System_Maintenance")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "System_Maintenance");
  	nvram_set("log_index", "System_Maintenance");
  }
  if(!(strcmp(index,"dns")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "dns");
  	nvram_set("log_index", "dns");
  }
  if(!(strcmp(index,"PPP")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "PPP");
  	nvram_set("log_index", "PPP");
  }
  if(!(strcmp(index,"UPnP")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "UPnP");
  	nvram_set("log_index", "UPnP");
  }
  if(!(strcmp(index,"WLAN")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "WLAN");
  	nvram_set("log_index", "WLAN");
  }
    if(!(strcmp(index,"NTPClient")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "NTPClient");
  	nvram_set("log_index", "NTPClient");
  }
  if(!(strcmp(index,"SYSwarning")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "SYSwarning");
  	nvram_set("log_index", "SYSwarning");
  }
  //-------- aron add 2009.10.23
  if(!(strcmp(index,"dhcpServer")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "dhcpServer");
  	nvram_set("log_index", "dhcpServer");
  }
  if(!(strcmp(index,"dhcpClient")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "dhcpClient");
  	nvram_set("log_index", "dhcpClient");
  }
  if(!(strcmp(index,"ddns")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "ddns");
  	nvram_set("log_index", "ddns");
  }
  //---------------------------
  
  //--------- steve add 2009.10.27
  if(!(strcmp(index,"Firewall")))
  {
  	//nvram_bufset(RT2860_NVRAM, "log_index", "Firewall");
  	nvram_set("log_index", "Firewall");
  }
  //------------------------------
  nvram_commit();
  
	websRedirect(wp, "/local/advance/syslog_gordon.asp");
}
//goform/logsetting
static void logsetting(webs_t wp, char_t *path, char_t *query)
{
	char *SystemMaintenance, *logdns, *PPP, *UPnP, *WLAN, *NTPClient, *SYSwarning, *select_num;
        char *dhcpServer, *dhcpClient, *ddns; //aron add 2009.10.23
        char *Firewall; //Steve add 2009/10/22   


	SystemMaintenance=websGetVar(wp, T("System_Maintenance"), T("0"));
	ezplib_replace_attr("log_rule", 0, "web_mgmt", SystemMaintenance);

	logdns=websGetVar(wp, T("dns"), T("0"));
	ezplib_replace_attr("log_rule", 0, "dns", logdns);
	
	PPP=websGetVar(wp, T("PPP"), T("0"));
	ezplib_replace_attr("log_rule", 0, "ppp", PPP);
		
	UPnP=websGetVar(wp, T("UPnP"), T("0"));
	ezplib_replace_attr("log_rule", 0, "upnp", UPnP);
	
	WLAN=websGetVar(wp, T("WLAN"), T("0"));
	ezplib_replace_attr("log_rule", 0, "wireless", WLAN);
	
	NTPClient=websGetVar(wp, T("NTPClient"), T("0"));
	ezplib_replace_attr("log_rule", 0, "ntp", NTPClient);
	
	SYSwarning=websGetVar(wp, T("SYSwarning"), T("0"));
	ezplib_replace_attr("log_rule", 0, "sys_warning", SYSwarning);

	select_num=websGetVar(wp, T("select_num"), T("")); 
	nvram_set("log_selected_num", select_num);
        
        //----- aron add 2009.10.23
        dhcpServer=websGetVar(wp, T("DhcpServer"), T("0"));
	ezplib_replace_attr("log_rule", 0, "dhcp_serv", dhcpServer);
	
        dhcpClient=websGetVar(wp, T("DhcpClient"), T("0"));
	ezplib_replace_attr("log_rule", 0, "dhcp_cli", dhcpClient);
	
        ddns=websGetVar(wp, T("DDNS"), T("0"));
	ezplib_replace_attr("log_rule", 0, "ddns", ddns);
        //--------------------------------------
	
	//-------steve add 2009.10.22
	Firewall=websGetVar(wp, T("Firewall"), T(""));
	ezplib_replace_attr("log_rule", 0, "firewall", Firewall);
	//-------------------------------------------
	
	websRedirect(wp, "/local/advance/LogSettings.asp");
}
/*
 * goform/NTP
 */
static void NTP(webs_t wp, char_t *path, char_t *query)
{
        char tmp[TMP_LEN], tmp1[TMP_LEN];
        char *enable, *type, *ipaddr, *zone, *manual, *daylight;
        char *hour, *min, *sec, *year, *mon, *date;
        char *startMon, *startDay, *startClock, *endMon, *endDay, *endClock;
        
        char *rule_set = "ntp_rule";
        int len, manual_type, daylightEnb, change = 0;

        //preaction
        snprintf(tmp1, TMP_LEN, "NUM=0 TYPE=wan /etc/rc.common /etc/rc.d/T60ntpclient stop");
        system(tmp1);
        //system("/etc/rc.common /etc/init.d/ntpclient stop");
        
        //AboCom always make it "enabled"
        enable = "1";
        
        /* Time setting type */
        snprintf(tmp, sizeof(tmp), "mten_ServiceType");
        manual = websGetVar(wp, tmp, "");
        manual_type = atoi(manual);
        if(manual_type ==0) //Abocom treat "manual time" as "0"
            manual = "1";   //AXIM treat "manual time" as "1"
        else if(manual_type ==1)
            manual = "0";
        
        if (!strcmp(manual, "1")) //setting time manually 
        {
             /* hour */
             snprintf(tmp, sizeof(tmp), "current_hour");
             hour = websGetVar(wp, tmp, "");
              
             /* minute */
             snprintf(tmp, sizeof(tmp), "current_Min");
             min = websGetVar(wp, tmp, "");
             
             /* second */
             snprintf(tmp, sizeof(tmp), "current_Sec");
             sec = websGetVar(wp, tmp, "");
             
             /* year */
             snprintf(tmp, sizeof(tmp), "current_Year");
             year = websGetVar(wp, tmp, "");
             
             /* month */
             snprintf(tmp, sizeof(tmp), "current_Mon");
             mon = websGetVar(wp, tmp, "");
             
             /* date */
             snprintf(tmp, sizeof(tmp), "current_Day");
             date = websGetVar(wp, tmp, "");
             
             ezplib_replace_attr(rule_set, 0, "hour", hour);
             ezplib_replace_attr(rule_set, 0, "min", min);
             ezplib_replace_attr(rule_set, 0, "sec", sec);
             ezplib_replace_attr(rule_set, 0, "year", year);
             ezplib_replace_attr(rule_set, 0, "mon", mon);
             ezplib_replace_attr(rule_set, 0, "date", date);
        }
        else if (!strcmp(manual, "0")) //get from time server
        { 
          #if 0  
             /* Time Server type */
             snprintf(tmp, sizeof(tmp), "time_ServerType");
             type = websGetVar(wp, tmp, "");
             if (!strcmp(type, "0"))
                 type = "pool";
             else if (!strcmp(type, "1"))
                 type = "ipaddr"; 
            
             if (!strcmp(type, "pool")) 
             {
                //AboCom don't have server area to be chosen on UI
             } 
             else 
             {    
                /* IP Address */
                snprintf(tmp, sizeof(tmp), "NTPServerIP");
                ipaddr = websGetVar(wp, tmp, "");
                ezplib_replace_attr(rule_set, 0, "ipaddr", ipaddr);
             }
             ezplib_replace_attr(rule_set, 0, "type", type);
          #else
             /* Time Server type */
             snprintf(tmp, sizeof(tmp), "time_ServerType");
             type = websGetVar(wp, tmp, "");
            
             if (!strcmp(type, "0")) 
             {
                //AboCom don't have server area to be chosen on UI
             } 
             else 
             {    
                /* IP Address */
                snprintf(tmp, sizeof(tmp), "NTPServerIP");
                ipaddr = websGetVar(wp, tmp, "");
                ezplib_replace_attr(rule_set, 0, "serv_ipaddr", ipaddr);
             }
             ezplib_replace_attr(rule_set, 0, "custom_server", type);
          #endif
        }
        
        ezplib_replace_attr(rule_set, 0, "custom_time", manual);
        
        /* Zone */
        snprintf(tmp, sizeof(tmp), "time_zone");
        zone = websGetVar(wp, tmp, "");
        ezplib_replace_attr(rule_set, 0, "zone", zone);

        /* daylight saving */
        snprintf(tmp, sizeof(tmp), "enabledaylight");
        daylight = websGetVar(wp, tmp, "");
        daylightEnb = atoi(daylight);
        if(daylightEnb ==0)
            daylight = "0";
        else if(daylightEnb ==1)
            daylight = "1";
        
        if (!strcmp(daylight, "1")) //enabled
        {
             /* start Month */
             snprintf(tmp, sizeof(tmp), "dst_startMon");
             startMon = websGetVar(wp, tmp, "");
              
             /* start Day */
             snprintf(tmp, sizeof(tmp), "dst_startDay");
             startDay = websGetVar(wp, tmp, "");
             
             /* start clock */
             snprintf(tmp, sizeof(tmp), "dst_startclock");
             startClock = websGetVar(wp, tmp, "");
             
             /* end Month */
             snprintf(tmp, sizeof(tmp), "dst_endMon");
             endMon = websGetVar(wp, tmp, "");
             
             /* end Day */
             snprintf(tmp, sizeof(tmp), "dst_endDay");
             endDay = websGetVar(wp, tmp, "");
             
             /* end Clock */
             snprintf(tmp, sizeof(tmp), "dst_endclock");
             endClock = websGetVar(wp, tmp, "");
             
             ezplib_replace_attr(rule_set, 0, "ds_start_mon", startMon);
             ezplib_replace_attr(rule_set, 0, "ds_start_day", startDay);
             ezplib_replace_attr(rule_set, 0, "ds_start_hour", startClock);
             ezplib_replace_attr(rule_set, 0, "ds_end_mon", endMon);
             ezplib_replace_attr(rule_set, 0, "ds_end_day", endDay);
             ezplib_replace_attr(rule_set, 0, "ds_end_hour", endClock);	
        }
        ezplib_replace_attr(rule_set, 0, "daylight_saving", daylight);
     
        //postaction
        snprintf(tmp1, TMP_LEN, "NUM=0 TYPE=wan /etc/rc.common /etc/rc.d/T60ntpclient start");
        system(tmp1);
        //system("/etc/rc.common /etc/init.d/ntpclient start");
        
        nvram_commit();
        
        setWebMessage(0, NULL);
        websRedirect(wp, "/local/advance/management_gordon.asp");          
}

/*
 * goform/NTPSyncWithHost
 */
static void NTPSyncWithHost(webs_t wp, char_t *path, char_t *query)
{
	if(!query || (!strlen(query)))
		return;
	if(strchr(query, ';'))
		return;

    doSystem("date -s %s", query);


	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
    websWrite(wp, T("n/a"));
    websDone(wp, 200);
}


/*
 * goform/DDNS
 */
static void DDNS(webs_t wp, char_t *path, char_t *query)
{
    char *rule_num = nvram_safe_get("wan_num");  // Corresponding to wan_num
    char buf[LONG_BUF_LEN], tmp[TMP_LEN];
    char *rule_set = "wan_ddns_rule";
    char *enable, *type, *username, *passwd, *hostname;
    int num, i, len, change = 0;
    int ddnsEnb;

    if (!*rule_num) 
        num = 1;
    else 
        num = atoi(rule_num);

    for (i = 0; i < num; i++) {

        if(i==0) //Normally, the nth for "0"
        {
           /* Enable */
           snprintf(tmp, sizeof(tmp), "ddnsenabled");
           enable = websGetVar(wp, tmp, "");
        }
        else //increased nth for "1", "2",...etc.
        {
           /* Enable */
           snprintf(tmp, sizeof(tmp), "ddnsenabled_%d", i);
           enable = websGetVar(wp, tmp, "");
        }

        //preaction
        snprintf(tmp, TMP_LEN, "NUM=%d TYPE=wan /etc/rc.common /etc/rc.d/T60ddns stop", i);
        system(tmp);
        //system("/etc/rc.common /etc/init.d/ddns stop");

        ddnsEnb=atoi(enable);
        if (ddnsEnb == 0) //disabled
        {
            ezplib_replace_attr(rule_set, i, "enable", "0");
            change = 1;
            
        } 
        else //enabled
        {
            if(i==0) //Normally, the nth for "0"
            {
                /* Type */
                snprintf(tmp, sizeof(tmp), "DDNSProvider");
                type = websGetVar(wp, tmp, "");

                /* User Name */
                snprintf(tmp, sizeof(tmp), "Account");
                username = websGetVar(wp, tmp, "");

                /* Passwd */
                snprintf(tmp, sizeof(tmp), "Password");
                passwd = websGetVar(wp, tmp, "");

                /* Host Name */
                snprintf(tmp, sizeof(tmp), "DDNS");
                hostname = websGetVar(wp, tmp, "");
            }
            else //increased nth for "1", "2",...etc.
            {
            	/* Type */
                snprintf(tmp, sizeof(tmp), "DDNSProvider_%d", i);
                type = websGetVar(wp, tmp, "");

                /* User Name */
                snprintf(tmp, sizeof(tmp), "Account_%d", i);
                username = websGetVar(wp, tmp, "");

                /* Passwd */
                snprintf(tmp, sizeof(tmp), "Password_%d", i);
                passwd = websGetVar(wp, tmp, "");

                /* Host Name */
                snprintf(tmp, sizeof(tmp), "DDNS_%d", i);
                hostname = websGetVar(wp, tmp, "");
            }
            
            //Translate AboCom's dns provider to AXIM's nvram format
            if(!strcmp(type, "dyndns.org"))
            	     type = "dyndns";
            else if(!strcmp(type, "no-ip.com"))
            	     type = "noip";
            else if(!strcmp(type, "eurodyndns"))
            	     type = "eurodns";
            else if(!strcmp(type, "regfish"))
            	     type = "regfish";
            
            len =
                snprintf(buf, TMP_LEN, "WAN%d_DDNS^%s^%s^%s^%s^%s", 
                         i+1, enable, type, username, passwd, hostname);

            if (len >= TMP_LEN) 
            {
                return 0;
            }
             
            ezplib_replace_rule(rule_set, i, buf);
            change = 1; 
             
            //postaction
            snprintf(tmp, TMP_LEN, "NUM=%d TYPE=wan /etc/rc.common /etc/rc.d/T60ddns start", i);
            system(tmp);
            //system("/etc/rc.common /etc/init.d/ddns start");
        }
    }

    nvram_commit();
    //return change;
    
    setWebMessage(0, NULL);
    	
    char_t *submitUrl;
    submitUrl = websGetVar(wp, T("ddns_url"), T(""));   // aron add for hidden page
    websRedirect(wp, submitUrl);       
}

static void SystemCommand(webs_t wp, char_t *path, char_t *query)
{
	char *command;

	command = websGetVar(wp, T("command"), T(""));

	if(!command)
		return;

	if(!strlen(command)){
		doSystem("cat /dev/null > %s", SYSTEM_COMMAND_LOG);
	}else if(strchr(command, '>') || strchr(command, '<')){
		doSystem("cat /dev/null > %s", SYSTEM_COMMAND_LOG);
		snprintf(system_command, COMMAND_MAX, "%s", command);
	}else{
		snprintf(system_command, COMMAND_MAX, "%s 1>%s 2>&1", command, SYSTEM_COMMAND_LOG);
	}	
	
	if(strlen(system_command))
		doSystem(system_command);

	// FIXME/TODO, YYHuang 07/04/11
	// the path here should be obtained by goahead internal function.
	// (is it existed?)
	websRedirect(wp, "adm/system_command.asp");

	return;
}


int showSystemCommandASP(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	char buf[1024];
	
	fp = fopen(SYSTEM_COMMAND_LOG, "r");
	if(!fp){
		websWrite(wp, T(""));
		return 0;
	}

	while(fgets(buf, 1024, fp)){
		websWrite(wp, T("%s"), buf);
	}
	fclose(fp);
	
	return 0;
}

static inline char *strip_space(char *str)
{
	while( *str == ' ')
		str++;
	return str;
}


char* getField(char *a_line, char *delim, int count)
{
	int i=0;
	char *tok;
	tok = strtok(a_line, delim);
	while(tok){
		if(i == count)
			break;
        i++;
		tok = strtok(NULL, delim);
    }
    if(tok)
		return tok;

	return NULL;
}

/*
 *   C version. (ASP version is below)
 */
static long long getIfStatistic(char *interface, int type)
{
	int found_flag = 0;
	int skip_line = 2;
	char buf[1024], *field, *semiColon = NULL;
	FILE *fp = fopen(PROC_IF_STATISTIC, "r");
	if(!fp){
		printf("no proc?\n");
		return -1;
	}

	while(fgets(buf, 1024, fp)){
		char *ifname;
		if(skip_line != 0){
			skip_line--;
			continue;
		}
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';
		ifname = buf;
		ifname = strip_space(ifname);

		if(!strcmp(ifname, interface)){
			found_flag = 1;
			break;
		}
	}
	fclose(fp);

	semiColon++;

	switch(type){
	case TXBYTE:
		if(  (field = getField(semiColon, " ", 8))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case TXPACKET:
		if(  (field = getField(semiColon, " ", 9))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case RXBYTE:
		if(  (field = getField(semiColon, " ", 0))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case RXPACKET:
		if(  (field = getField(semiColon, " ", 1))  ){
			return strtoll(field, NULL, 10);
		}
		break;
// Tommy, Add Collisions field, 2009/2/4 09:18
#if 1
	case COLLS:
		if(  (field = getField(semiColon, " ", 13))  ){
			return strtoll(field, NULL, 10);
		}
		break;
#endif		
	}
	return -1;
}

/*
 *     getIfStatistic()   ASP version
 */
int getIfStatisticASP(int eid, webs_t wp, int argc, char_t **argv)
{
	int found_flag = 0;
	int skip_line = 2;
	char *interface, *type, *field, *semiColon = NULL;
	char buf[1024], result[32];
	FILE *fp = fopen(PROC_IF_STATISTIC, "r");
	if(!fp){
		websWrite(wp, T("no proc?\n"));
		return -1;
	}

    if(ejArgs(argc, argv, T("%s %s"), &interface, &type) != 2){
		websWrite(wp, T("Wrong argument.\n"));
        return -1;
    }

	while(fgets(buf, 1024, fp)){
		char *ifname;
		if(skip_line != 0){
			skip_line--;
			continue;
		}
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';
		ifname = buf;
		ifname = strip_space(ifname);

		if(!strcmp(ifname, interface)){
			found_flag = 1;
			break;
		}
	}
	fclose(fp);

	semiColon++;

	if(!strcmp(type, T("TXBYTE")  )){
		if(  (field = getField(semiColon, " ", 8))  ){
			snprintf(result, 32,"%lld",   strtoll(field, NULL, 10));
			ejSetResult(eid, result);
		}
	}else if(!strcmp(type, T("TXPACKET")  )){
		if(  (field = getField(semiColon, " ", 9))  ){
			snprintf(result, 32,"%lld",   strtoll(field, NULL, 10));
			ejSetResult(eid, result);
		}
    }else if(!strcmp(type, T("RXBYTE")  )){
		if(  (field = getField(semiColon, " ", 0))  ){
			snprintf(result, 32,"%lld",   strtoll(field, NULL, 10));
			ejSetResult(eid, result);
		}
    }else if(!strcmp(type, T("RXPACKET")  )){
		if(  (field = getField(semiColon, " ", 1))  ){
			snprintf(result, 32,"%lld",   strtoll(field, NULL, 10));
			ejSetResult(eid, result);
		}
    }else{
		websWrite(wp, T("unknown type.") );
		return -1;
	}
	return -1;
}

void GetIfConfigCounter(char *interface, char *whichCounter, long long *reVal)
{
	FILE *fp;
	unsigned long tmp;
	
	if (!strcmp(whichCounter, "TX packets")){
		doSystem("ifconfig %s | grep 'TX packets:' | cut -d ':' -f2 | cut -d ' ' -f1 > /tmp/IfCounter", interface);
    }else if (!strcmp(whichCounter, "RX packets")){
    	doSystem("ifconfig %s | grep 'RX packets:' | cut -d ':' -f2 | cut -d ' ' -f1 > /tmp/IfCounter", interface);
    }else if (!strcmp(whichCounter, "collisions")){
    	doSystem("ifconfig %s | grep 'collisions:' | cut -d ':' -f2 | cut -d ' ' -f1 > /tmp/IfCounter", interface);
    }else if (!strcmp(whichCounter, "TX bytes")){
    	doSystem("ifconfig %s | grep 'TX bytes:' | cut -d ':' -f3 | cut -d ' ' -f1 > /tmp/IfCounter", interface);
    }else if (!strcmp(whichCounter, "RX bytes")){
    	doSystem("ifconfig %s | grep 'RX bytes:' | cut -d ':' -f2 | cut -d ' ' -f1 > /tmp/IfCounter", interface);
    }else{
    	system("ehco 0 > /tmp/IfCounter");
	}

	fp = fopen("/tmp/IfCounter", "r");
	fscanf(fp,"%lu",&tmp);
	fclose(fp);
	*reVal = tmp;
	system("rm -f /tmp/IfCounter");
}

// Re-count LAN/WLAN/WAN Bytes/second
static	long long LANRXBytesPerSec;
static	long long WANRXBytesPerSec;
static	long long WLANRXBytesPerSec;
static	long long LANTXBytesPerSec;
static	long long WANTXBytesPerSec;
static	long long WLANTXBytesPerSec;
//void GetIfBytesPerSec(void)
int GetIfBytesPerSecASP(int eid, webs_t wp, int argc, char_t **argv)
{
	long long LANRXBytes0s;
	long long WANRXBytes0s;
	long long WLANRXBytes0s;
	long long LANTXBytes0s;
	long long WANTXBytes0s;
	long long WLANTXBytes0s;
	
	long long LANRXBytes1s;
	long long WANRXBytes1s;
	long long WLANRXBytes1s;
	long long LANTXBytes1s;
	long long WANTXBytes1s;
	long long WLANTXBytes1s;

	char *wan_type;

	//Get WAN If
	wan_type = nvram_get("wan0_ifname");
	
	//WAN
#if 1
	WANRXBytes0s = getIfStatistic( wan_type, RXBYTE);
	WANTXBytes0s = getIfStatistic( wan_type, TXBYTE);
#else
	//Use ifconfig to get ifconfig's counters (RX packets/TX packets/RX bytes/TX bytes/collisions)
	GetIfConfigCounter(wan_type, "RX bytes", &WANRXBytes0s);
	GetIfConfigCounter(wan_type, "TX bytes", &WANTXBytes0s);
#endif
	
	//LAN
	LANRXBytes0s = getIfStatistic( "br0", RXBYTE);
	LANTXBytes0s = getIfStatistic( "br0", TXBYTE);
	//WLAN
	WLANRXBytes0s = getIfStatistic( "ra0", RXBYTE);
	WLANTXBytes0s = getIfStatistic( "ra0", TXBYTE);	
	
	//Delay 1 second
	system("sleep 1");
	
	//WAN
#if 1
	WANRXBytes1s = getIfStatistic( wan_type, RXBYTE);
	WANTXBytes1s = getIfStatistic( wan_type, TXBYTE);
#else
	//Use ifconfig to get ifconfig's counters (RX packets/TX packets/RX bytes/TX bytes/collisions)
	GetIfConfigCounter(wan_type, "RX bytes", &WANRXBytes1s);
	GetIfConfigCounter(wan_type, "TX bytes", &WANTXBytes1s);
#endif

	//LAN
	LANRXBytes1s = getIfStatistic( "br0", RXBYTE);
	LANTXBytes1s = getIfStatistic( "br0", TXBYTE);	
	//WLAN
	WLANRXBytes1s = getIfStatistic( "ra0", RXBYTE);
	WLANTXBytes1s = getIfStatistic( "ra0", TXBYTE);	
	
	LANRXBytesPerSec = LANRXBytes1s - LANRXBytes0s;
	WANRXBytesPerSec = WANRXBytes1s - WANRXBytes0s;
	WLANRXBytesPerSec = WLANRXBytes1s - WLANRXBytes0s;

	LANTXBytesPerSec = LANTXBytes1s - LANTXBytes0s;
	WANTXBytesPerSec = WANTXBytes1s - WANTXBytes0s;
	WLANTXBytesPerSec = WLANTXBytes1s - WLANTXBytes0s;
	websWrite(wp, T("OK"));
	return 0;	
}

int getWANRxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char_t buf[32];
	
	snprintf(buf, 32, "%lld", WANRXBytesPerSec);
	websWrite(wp, T("%s"), buf);
	return 0;
#else
	FILE *fwanlink;
	int updays, uphours, upminutes, upseconds;
	unsigned long wanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long wancurrtime;
	
	ezplib_get_attr_val("wan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	wanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/wancurrtime");
	fwanlink = fopen("/tmp/wancurrtime", "r");
	fscanf(fwanlink,"%lu",&wancurrtime);
	fclose(fwanlink);
	system("rm -f /tmp/wancurrtime");
	
	disptime = wancurrtime - wanuptime;
	
	char_t buf[32];
	long long data = getIfStatistic( getWanIfName(), RXBYTE);
	snprintf(buf, 32, "%lld", data/disptime);	
	websWrite(wp, T("%s"), buf);
	return 0;
#endif
}

int getWANRxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t 	buf[32];
	char	*value;
	
	//Get WAN If
	value = nvram_get("wan0_ifname");
	
	long long data = getIfStatistic( value, RXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getWANTxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char_t buf[32];
	
	snprintf(buf, 32, "%lld", WANTXBytesPerSec);
	websWrite(wp, T("%s"), buf);
	return 0;
#else
	FILE *fwanlink;
	int updays, uphours, upminutes, upseconds;
	unsigned long wanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long wancurrtime;
	
	ezplib_get_attr_val("wan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	wanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/wancurrtime");
	fwanlink = fopen("/tmp/wancurrtime", "r");
	fscanf(fwanlink,"%lu",&wancurrtime);
	fclose(fwanlink);
	system("rm -f /tmp/wancurrtime");
	
	disptime = wancurrtime - wanuptime;
		
	char_t buf[32];
	long long data = getIfStatistic( getWanIfName(), TXBYTE);
	snprintf(buf, 32, "%lld", data/disptime);	
	websWrite(wp, T("%s"), buf);
	return 0;
#endif
}

int getWANTxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	char	*value;
	
	//Get WAN If
	value = nvram_get("wan0_ifname");
	long long data = getIfStatistic( value, TXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getLANRxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char_t buf[32];
	
	snprintf(buf, 32, "%lld", LANRXBytesPerSec);
	websWrite(wp, T("%s"), buf);
	return 0;
#else
	FILE *flanlink;	
	int updays, uphours, upminutes, upseconds;	
	unsigned long lanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long lancurrtime;
	
	ezplib_get_attr_val("lan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	lanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/lancurrtime");
	flanlink = fopen("/tmp/lancurrtime", "r");
	fscanf(flanlink,"%lu",&lancurrtime);
	fclose(flanlink);
	system("rm -f /tmp/lancurrtime");
	
	disptime = lancurrtime - lanuptime;
	
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), RXBYTE);
	snprintf(buf, 32, "%lld", data/disptime);	
	websWrite(wp, T("%s"), buf);
	return 0;
#endif
}

int getLANRxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), RXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getLANTxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char_t buf[32];
	
	//GetIfBytesPerSec();

	snprintf(buf, 32, "%lld", LANTXBytesPerSec);
	websWrite(wp, T("%s"), buf);
	return 0;
#else
	FILE *flanlink;	
	int updays, uphours, upminutes, upseconds;	
	unsigned long lanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long lancurrtime;
	
	ezplib_get_attr_val("lan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	lanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/lancurrtime");
	flanlink = fopen("/tmp/lancurrtime", "r");
	fscanf(flanlink,"%lu",&lancurrtime);
	fclose(flanlink);
	system("rm -f /tmp/lancurrtime");
	
	disptime = lancurrtime - lanuptime;
	
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), TXBYTE);
	snprintf(buf, 32, "%lld", data/disptime);	
	websWrite(wp, T("%s"), buf);
	return 0;
#endif
}

int getLANTxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), TXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"),buf);
	return 0;
}

// Tommy , Add WLAN Statistics , 2009/2/4 09:38
#if 1
int getWLANRxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char_t buf[32];
	
	snprintf(buf, 32, "%lld", WLANRXBytesPerSec);
	websWrite(wp, T("%s"), buf);
	return 0;
#else
	FILE *fwlanlink;
	int updays, uphours, upminutes, upseconds;
	unsigned long wlanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long wlancurrtime;
	
	ezplib_get_attr_val("wlan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	wlanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/wlancurrtime");
	fwlanlink = fopen("/tmp/wlancurrtime", "r");
	fscanf(fwlanlink,"%lu",&wlancurrtime);
	fclose(fwlanlink);
	system("rm -f /tmp/wlancurrtime");
	
	disptime = wlancurrtime - wlanuptime;
	
	char_t buf[32];
	long long data = getIfStatistic( "ra0", RXBYTE);
	snprintf(buf, 32, "%lld", data/disptime);	
	websWrite(wp, T("%s"), buf);
	return 0;
#endif
}

int getWLANRxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( "ra0", RXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getWLANTxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char_t buf[32];
	
	snprintf(buf, 32, "%lld", WLANTXBytesPerSec);
	websWrite(wp, T("%s"), buf);
	return 0;
#else
	FILE *fwlanlink;
	int updays, uphours, upminutes, upseconds;
	unsigned long wlanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long wlancurrtime;
	
	ezplib_get_attr_val("wlan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	wlanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/wlancurrtime");
	fwlanlink = fopen("/tmp/wlancurrtime", "r");
	fscanf(fwlanlink,"%lu",&wlancurrtime);
	fclose(fwlanlink);
	system("rm -f /tmp/wlancurrtime");
	
	disptime = wlancurrtime - wlanuptime;
	
	char_t buf[32];
	long long data = getIfStatistic( "ra0", TXBYTE);
	snprintf(buf, 32, "%lld", data/disptime);	
	websWrite(wp, T("%s"), buf);
	return 0;
#endif
}

int getWLANTxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( "ra0", TXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"),buf);
	return 0;
}
#endif

int getWANCollsASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	char	*value;
	
	//Get WAN If
	value = nvram_get("wan0_ifname");
	long long data = getIfStatistic( value, COLLS);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"),buf);
	return 0;
}

int getLANCollsASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), COLLS);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"),buf);
	return 0;
}

int getWLANCollsASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( "ra0", COLLS);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"),buf);
	return 0;	
}

int getPortLinkStatus(port_num)
{
	int port, rc;
	FILE *fp;
	char buf[1024];

	port = port_num;	
	char *pos;
	int link = 0;

	FILE *proc_file = fopen("/proc/rt2880/gmac", "w");
	if (!proc_file){
		printf("indicate error: Cannot open /proc/rt2880/gmac file !!!\n");
	}
	fprintf(proc_file, "%d", port);
	fclose(proc_file);

	if ((fp = popen("ethtool eth2", "r")) == NULL){
		printf("indicate error: Cannot do <ethtool eth2>!!!\n");
	}
	rc = fread(buf, 1, 1024, fp);
	pclose(fp);
	if(rc == -1){
		printf("indicate error: Cannot read the result of <ethtool eth2> !!!\n");			
	}else{
		// get Link status
		if((pos = strstr(buf, "Link detected: ")) != NULL){
			pos += strlen("Link detected: ");
			if(*pos == 'y')
				link = 1;
		}
		
		if (link == 1){
			// get speed
			if((pos = strstr(buf, "Speed: ")) != NULL){
				pos += strlen("Speed: ");
				if(*pos == '1' && *(pos+1) == '0' && *(pos+2) == 'M')
					link = 10;
#if 0					
				if(*pos == '1' && *(pos+1) == '0' && *(pos+2) == '0' && *(pos+3) == '0' && *(pos+4) == 'M')
#else
				if(*pos == '1' && *(pos+1) == '0' && *(pos+2) == '0' && *(pos+3) == 'M')
#endif				
					link = 100;
			}		
		}		
		
	}
	return link;
}

int getWANPortLinkStatus(void)
{
	// Need to get 2 times, because the first time always fail to get port status
	getPortLinkStatus(4); // Don't take away.
	return getPortLinkStatus(4); // WAN port = port 4
}

int getWANLinkStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	char wanlink[10];
	char wanspeed[10];
	FILE *fwanlink, *fwanspeed;
	
	system("/sbin/ezp-portstate 5 link > /tmp/wanlink");
	fwanlink = fopen("/tmp/wanlink", "r");
	fscanf(fwanlink,"%s",wanlink);
	fclose(fwanlink);
	system("rm -f /tmp/wanlink");
	
	system("/sbin/ezp-portstate 5 speed > /tmp/wanspeed");
	fwanspeed = fopen("/tmp/wanspeed", "r");
	fscanf(fwanspeed,"%s",wanspeed);
	fclose(fwanspeed);
	system("rm -f /tmp/wanspeed");	

	if (!strcmp(wanlink, "up")){
		if (!strcmp(wanspeed, "10")){
			websWrite(wp, T("10M"));
			return 0;
		}else if (!strcmp(wanspeed, "100")){
			websWrite(wp, T("100M"));
			return 0;
		}else if (!strcmp(wanspeed, "1000")){
			websWrite(wp, T("1000M"));
			return 0;
		}	
	}else if (!strcmp(wanlink, "down")){
		websWrite(wp, T("Down"));
		return 0;
	}else{
		websWrite(wp, T("Down"));	
		return 0;
	}
	return 0;
}

int getWLANLinkStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	char TempBuf[32];
	
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "wisp")){
		websWrite(wp, T("300M"));
		return 0;		
	}
	
	ezplib_get_attr_val("wl_basic_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "1")){
		websWrite(wp, T("300M"));
	}else{
		websWrite(wp, T("Down"));
	}
	return 0;
}	

	
int getWANUpTime(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fwanlink;
	int updays, uphours, upminutes, upseconds;	
	unsigned long wanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long wancurrtime;
	
	ezplib_get_attr_val("wan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	wanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/wancurrtime");
	fwanlink = fopen("/tmp/wancurrtime", "r");
	fscanf(fwanlink,"%lu",&wancurrtime);
	fclose(fwanlink);
	system("rm -f /tmp/wancurrtime");
	
	disptime = wancurrtime - wanuptime;
	
	updays = (int) disptime / (60*60*24);
	upminutes = (int) disptime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	upseconds = ((int) disptime)%60;

	if (updays){
		return websWrite(wp, T("%d day%s, %s%d:%s%d:%s%d") , 
		updays, (updays != 1) ? "s" : "",
		(uphours < 10) ? "0" : "",uphours,
		(upminutes < 10) ? "0" : "",upminutes,
		(upseconds < 10) ? "0" : "",upseconds);
	}else{
		return websWrite(wp, T("%s%d:%s%d:%s%d") ,
		(uphours < 10) ? "0" : "",uphours,
		(upminutes < 10) ? "0" : "",upminutes,
		(upseconds < 10) ? "0" : "",upseconds);	
	}
}	
                  
int getLANUpTime(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *flanlink;	
	int updays, uphours, upminutes, upseconds;	
	unsigned long lanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long lancurrtime;
	
	ezplib_get_attr_val("lan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	lanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/lancurrtime");
	flanlink = fopen("/tmp/lancurrtime", "r");
	fscanf(flanlink,"%lu",&lancurrtime);
	fclose(flanlink);
	system("rm -f /tmp/lancurrtime");
	
	disptime = lancurrtime - lanuptime;
	
	updays = (int) disptime / (60*60*24);
	upminutes = (int) disptime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	upseconds = ((int) disptime)%60;

	if (updays){
		return websWrite(wp, T("%d day%s, %s%d:%s%d:%s%d") , 
		updays, (updays != 1) ? "s" : "",
		(uphours < 10) ? "0" : "",uphours,
		(upminutes < 10) ? "0" : "",upminutes,
		(upseconds < 10) ? "0" : "",upseconds);
	}else{
		return websWrite(wp, T("%s%d:%s%d:%s%d") ,
		(uphours < 10) ? "0" : "",uphours,
		(upminutes < 10) ? "0" : "",upminutes,
		(upseconds < 10) ? "0" : "",upseconds);	
	}	
}	

int getWLANUpTime(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fwlanlink;
	int updays, uphours, upminutes, upseconds;	
	unsigned long wlanuptime, disptime;
	//struct sysinfo info;
	char TempBuf[32];
	unsigned long wlancurrtime;
	
	ezplib_get_attr_val("wlan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	wlanuptime = atol(TempBuf);
	
	system("sed 's/\\..*$//g' /proc/uptime > /tmp/wlancurrtime");
	fwlanlink = fopen("/tmp/wlancurrtime", "r");
	fscanf(fwlanlink,"%lu",&wlancurrtime);
	fclose(fwlanlink);
	system("rm -f /tmp/wlancurrtime");
	
	disptime = wlancurrtime - wlanuptime;
	
	updays = (int) disptime / (60*60*24);
	upminutes = (int) disptime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	upseconds = ((int) disptime)%60;

	if (updays){
		return websWrite(wp, T("%d day%s, %s%d:%s%d:%s%d") , 
		updays, (updays != 1) ? "s" : "",
		(uphours < 10) ? "0" : "",uphours,
		(upminutes < 10) ? "0" : "",upminutes,
		(upseconds < 10) ? "0" : "",upseconds);
	}else{
		return websWrite(wp, T("%s%d:%s%d:%s%d") ,
		(uphours < 10) ? "0" : "",uphours,
		(upminutes < 10) ? "0" : "",upminutes,
		(upseconds < 10) ? "0" : "",upseconds);	
	}			
}	

/*
 * This ASP function is for javascript usage, ex:
 *
 * <script type="text/javascript">
 *   var a = new Array();
 *   a = [<% getAllNICStatisticASP(); %>];         //ex: a = ["lo","10","1000", "20", "2000","eth2"];
 *   document.write(a)
 * </script>
 *
 * Javascript could get info with  getAllNICStatisticASP().
 *
 * We dont produce table-related tag in this ASP function .It's
 * more extensive since ASP just handle data and Javascript present them,
 * although the data form is only for Javascript now.
 *
 * TODO: a lot, there are many ASP functions binding with table-relted tag...
 */
int getAllNICStatisticASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char result[1024];
	char buf[1024];
	int rc = 0, pos = 0, skip_line = 2;
	int first_time_flag = 1;
	FILE *fp = fopen(PROC_IF_STATISTIC, "r");
	if(!fp){
		printf("no proc?\n");
		return -1;
	}

	while(fgets(buf, 1024, fp)){
		char *ifname, *semiColon;
		if(skip_line != 0){
			skip_line--;
			continue;
		}
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';

		ifname = buf;
		ifname = strip_space(ifname);
		if(first_time_flag){
			pos = snprintf(result+rc, 1024-rc, "\"%s\"", ifname);
			rc += pos;
			first_time_flag = 0;
		}else{
			pos = snprintf(result+rc, 1024-rc, ",\"%s\"", ifname);
			rc += pos;
		}

		pos = snprintf(result+rc, 1024-rc, ",\"%lld\"", getIfStatistic(ifname, RXPACKET));
		rc += pos;
		pos = snprintf(result+rc, 1024-rc, ",\"%lld\"", getIfStatistic(ifname, RXBYTE));
		rc += pos;
		pos = snprintf(result+rc, 1024-rc, ",\"%lld\"", getIfStatistic(ifname, TXPACKET));
		rc += pos;
		pos = snprintf(result+rc, 1024-rc, ",\"%lld\"", getIfStatistic(ifname, TXBYTE));
		rc += pos;
	}
	fclose(fp);

	websWrite(wp, T("%s"), result);
    return 0;
}


int getMemTotalASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024], *semiColon, *key, *value;
	FILE *fp = fopen(PROC_MEM_STATISTIC, "r");
	if(!fp){
		websWrite(wp, T("no proc?\n"));
		return -1;
	}

	while(fgets(buf, 1024, fp)){
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';
		key = buf;
		value = semiColon + 1;
		if(!strcmp(key, "MemTotal")){
			value = strip_space(value);
			websWrite(wp, T("%s"), value);
			fclose(fp);
			return 0;
		}
	}
	websWrite(wp, T(""));
	fclose(fp);
	
	return -1;
}

/*static int getCurrentTimeASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t hour[2], min[2], sec[2];
	int num;
	
	FILE *fp = popen("date +%H", "r");
	if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(hour, 3, fp);
	pclose(fp);
  nvram_bufset(RT2860_NVRAM, "syshour", hour);
  
  FILE *fp_1 = popen("date +%M", "r");
	if(!fp_1){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(min, 3, fp_1);
	pclose(fp_1);
  nvram_bufset(RT2860_NVRAM, "sysmin", min);

  
  FILE *fp_2 = popen("date +%S", "r");
	if(!fp_2){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(sec, 3, fp_2);
	pclose(fp_2);
  nvram_bufset(RT2860_NVRAM, "syssec", sec);
  
	//return websWrite(wp, T("%s"), buf);
	return 0;
}
*/
//gordon add for CurrentDate 20081219//last modified 20090121
static int getCurrentYear(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t year[4];
	FILE *fp = popen("date +%Y", "r");
		if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(year, 5, fp);
	pclose(fp);
	//printf("run!!!!!!!!!!%s\n",year);
	websWrite(wp, T("%s"), year);
	return 0;
}
static int getCurrentMon(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t mon[2];
	FILE *fp = popen("date +%m", "r");
		if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(mon, 3, fp);
	pclose(fp);
	//printf("run!!!!!!!!!!%s\n",mon);
	websWrite(wp, T("%s"), mon);
	return 0;
}
static int getCurrentDay(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t day[2];
	FILE *fp = popen("date +%d", "r");
		if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(day, 3, fp);
	pclose(fp);
	//printf("run!!!!!!!!!!%s\n",day);
	websWrite(wp, T("%s"), day);
	return 0;
}
static int getCurrentHour(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t hour[2];
	FILE *fp = popen("date +%H", "r");
		if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(hour, 3, fp);
	pclose(fp);
	//printf("run!!!!!!!!!!%s\n",hour);
	websWrite(wp, T("%s"), hour);
	return 0;
}
static int getCurrentMin(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t min[2];
	FILE *fp = popen("date +%M", "r");
		if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(min, 3, fp);
	pclose(fp);
	//printf("run!!!!!!!!!!%s\n",min);
	websWrite(wp, T("%s"), min);
	return 0;
}
static int getCurrentSec(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t sec[2];
	FILE *fp = popen("date +%S", "r");
		if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(sec, 3, fp);
	pclose(fp);
	//printf("run!!!!!!!!!!%s\n",sec);
	websWrite(wp, T("%s"), sec);
	return 0;
}
/*static int getCurrentDateASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t year[4], month[2], date[2];
	int num;
	
	FILE *fp = popen("date +%Y", "r");

	
	if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}

	fgets(year, 5, fp);
	pclose(fp);
	nvram_bufset(RT2860_NVRAM, "sysyear", year);
	
	FILE *fp_1 = popen("date +%m", "r");
	if(!fp_1){
		websWrite(wp, T("none"));
		return 0;
	}
		fgets(month, 3, fp_1);
	pclose(fp_1);
	nvram_bufset(RT2860_NVRAM, "sysmonth", month);

  FILE *fp_2 = popen("date +%d", "r");
  if(!fp_2){
		websWrite(wp, T("none"));
		return 0;
	}
  fgets(date, 3, fp_2);
	pclose(fp_2);
  
  nvram_bufset(RT2860_NVRAM, "sysdate", date);
	//return websWrite(wp, T("%s"), buf);
	return 0;
}*/
int getSystemTime(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	FILE *fp = popen("date +%T", "r");
	if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(buf, 32, fp);
	pclose(fp);

	websWrite(wp, T("%s"), buf);
	return 0;
}
//gordon add for infoDate 20090203
int getSystemDate(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	FILE *fp = popen("date +%Y/%m/%d", "r");
	if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(buf, 32, fp);
	pclose(fp);

	websWrite(wp, T("%s"), buf);
	return 0;
}
#define LOG_MAX (16384)
int loglist(int eid, webs_t wp, int argc, char_t **argv)//Gordon add for systemlog 20081230
{
	FILE *fp1 = NULL;
	char *log1;
	char tm[32],msg[256];
	char display_msg[300];
	char *LOG_index, *show_all;
	int print_index=0; //control show log or not.
	int z,i;
  //LOG_index = nvram_bufget(RT2860_NVRAM, "log_index");
  LOG_index = nvram_safe_get("log_index");
	
  int linenum=0, charnum=0, tmnum=0, msgnum=0, a=0, line=0, tmstart=0, msgsize=0;

	fp1 = popen("logread -k USR", "r");
	if(!fp1){
		websWrite(wp, "-1");
		goto error;
	}

	log1 = malloc(LOG_MAX * sizeof(char));
	if(!log1){
		websWrite(wp, "-1");
		goto error;
	}
	memset(log1, 0, LOG_MAX);
	fread(log1, 1, LOG_MAX, fp1);
	pclose(fp1);
	for(charnum=0;charnum<16384;charnum++)
	{
	 if((log1[charnum]==00)&&(log1[charnum+1]==00)&&(log1[charnum+2]==00)&&(log1[charnum+3]==00))
	  {
	  	//printf("run 111111\n");
	  	free(log1);
	    return 0;
	  }
	  if(!strcmp(LOG_index,"show_all"))//ALL LOG
	  {
	    print_index=1;
	    //printf("######run SHOW ALL 111111!!!\n");
	  } 
	  if(!strcmp(LOG_index,"System_Maintenance"))//SystemMaintenance
	  {
//printf("######run SHOW SystemMaintenance<WEB> 111111!!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==87)&&(log1[charnum+2]==69)&&(log1[charnum+3]==66)&&(log1[charnum+4]==62))
	   {
//printf("%c%c%c%c%c\n",log1[charnum],log1[charnum+1],log1[charnum+2],log1[charnum+3],log1[charnum+4]);
	     print_index=1;
	   }
	  } 
	  if(!strcmp(LOG_index,"dns"))//DNS
	  {
//printf("######run SHOW DNS<DNS> 111111!!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==68)&&(log1[charnum+2]==78)&&(log1[charnum+3]==83)&&(log1[charnum+4]==62))
	   {
	     //printf("######run mask SystemMaintenance 222222!!!\n");
	     print_index=1;
	   }
	  } 
	  if(!strcmp(LOG_index,"PPP"))//PPP
	  {
	   //printf("######run SHOW PPP<PPP> 111111!!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==80)&&(log1[charnum+2]==80)&&(log1[charnum+3]==80)&&(log1[charnum+4]==62))
	   {
	     //printf("######run mask SystemMaintenance 222222!!!\n");
	     print_index=1;
	   }
	  } 
	  if(!strcmp(LOG_index,"UPnP"))//UPNP
	   {
	   //printf("######run SHOW UPNP<UPnP> 111111!!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==85)&&(log1[charnum+2]==80)&&(log1[charnum+3]==110)&&(log1[charnum+4]==80)&&(log1[charnum+5]==62))
	   {
	     //printf("######run mask SystemMaintenance 222222!!!\n");
	     print_index=1;
	   }
	  } 
// Tommy, add syslog 2009/10/22 01:46
	  if(!strcmp(LOG_index,"NTPClient"))//ntpclient
	  {
	   //printf("######run SHOW ntpclient<NTPClient> !!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==78)&&(log1[charnum+2]==84)&&(log1[charnum+3]==80)&&(log1[charnum+4]==67)&&(log1[charnum+5]==108)&&(log1[charnum+6]==105)&&(log1[charnum+7]==101)&&(log1[charnum+8]==110)&&(log1[charnum+9]==116)&&(log1[charnum+10]==62))
	   {
	     //printf("######run mask NTPClient !!!\n");
	     print_index=1;
	   }
	  } 
// Tommy, add syslog 2009/10/22 01:46
	  if(!strcmp(LOG_index,"WLAN")) //WLAN
	  {
	   //printf("######run SHOW Wireless<Wireless> !!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==87)&&(log1[charnum+2]==105)&&(log1[charnum+3]==114)&&(log1[charnum+4]==101)&&(log1[charnum+5]==108)&&(log1[charnum+6]==101)&&(log1[charnum+7]==115)&&(log1[charnum+8]==115)&&(log1[charnum+9]==62))
	   {
	     //printf("######run mask Wireless !!!\n");
	     print_index=1;
	   }
	  } 	  

// Tommy, add syslog 2009/10/22 01:46
	  if(!strcmp(LOG_index,"SYSwarning")) //SYSwarning
	  {
	   //printf("######run SHOW kernel and waring<SystemWarning> !!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==83)&&(log1[charnum+2]==121)&&(log1[charnum+3]==115)&&(log1[charnum+4]==116)&&(log1[charnum+5]==101)&&(log1[charnum+6]==109)&&(log1[charnum+7]==87)&&(log1[charnum+8]==97)&&(log1[charnum+9]==114)&&(log1[charnum+10]==110)&&(log1[charnum+11]==105)&&(log1[charnum+12]==110)&&(log1[charnum+13]==103)&&(log1[charnum+14]==62))
	   {
	     //printf("######run mask "warn kernel" !!!\n");
	     print_index=1;
	   }
	  }
          
          //---------- aron add  2009.10.23
	  if(!strcmp(LOG_index,"dhcpServer"))
	  {
	   //printf("######run SHOW dhcpServer<DHCPServer> 111111!!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==68)&&(log1[charnum+2]==72)&&(log1[charnum+3]==67)&&(log1[charnum+4]==80)&&(log1[charnum+5]==83)&&(log1[charnum+6]==101)&&(log1[charnum+7]==114)&&(log1[charnum+8]==118)&&(log1[charnum+9]==101)&&(log1[charnum+10]==114)&&(log1[charnum+11]==62))
	   {
	     //printf("######run mask SystemMaintenance 222222!!!\n");
	     print_index=1;
	   }
	  }
	  if(!strcmp(LOG_index,"dhcpClient"))
	  {
	   //printf("######run SHOW dhcpClient<DHCPClient> 111111!!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==68)&&(log1[charnum+2]==72)&&(log1[charnum+3]==67)&&(log1[charnum+4]==80)&&(log1[charnum+5]==67)&&(log1[charnum+6]==108)&&(log1[charnum+7]==105)&&(log1[charnum+8]==101)&&(log1[charnum+9]==110)&&(log1[charnum+10]==116)&&(log1[charnum+11]==62))
	   {
	     //printf("######run mask SystemMaintenance 222222!!!\n");
	     print_index=1;
	   }
	  }
	  if(!strcmp(LOG_index,"ddns"))
	  {
	   //printf("######run SHOW ddns<DDNS> 111111!!!\n");
	   if((log1[charnum]==60)&&(log1[charnum+1]==68)&&(log1[charnum+2]==68)&&(log1[charnum+3]==78)&&(log1[charnum+4]==83)&&(log1[charnum+5]==62))
	   {
	     //printf("######run mask SystemMaintenance 222222!!!\n");
	     print_index=1;
	   }
	  }    
	  //------------------------------- 
	  //---steve add 2009/10/22
	  if(!strcmp(LOG_index,"Firewall"))//Firewall
	   {
	  	 //Steve add tset Firewall
	  	 //printf("######run SHOW Firewall<Firewall>!!\n");
	   	if( (log1[charnum]==60)&&(log1[charnum+1]==70)&&(log1[charnum+2]==105)&&(log1[charnum+3]==114)&&(log1[charnum+4]==101)&&(log1[charnum+5]==119)&&(log1[charnum+6]==97)&&(log1[charnum+7]==108)&&(log1[charnum+8]==108)&&(log1[charnum+9]==62))
	   	{
	     	print_index=1;
	   	}
	  }
	  //----------------------------------
	   if(log1[charnum]==10)
	   {
	     //printf("run 222222\n");
	     ++linenum;
	     memset(tm, 0, 32);
	     memset(msg, 0, 256);
	     memset(display_msg,0,300);
	     //printf("run 33333\n");
	      for(tmnum=0;tmnum<=15;tmnum++)
	        {
	          //tm[a]=log1[tmstart];
	          msg[a]=log1[tmstart];
	          a=a+1;
	          tmstart=tmstart+1;
	        }
	        msgsize=charnum-tmstart;
	        //a=0;
	      for(msgnum=0;msgnum<=msgsize;msgnum++)
	        {
	          msg[a]=log1[tmstart];
	          a=a+1;
	          tmstart=tmstart+1;
	        } 
	         tmstart=charnum+1;
	         a=0;

// Deal the content of msg <WEB> , <PPP> ....
			z=0;
			for (i=0;i<sizeof(msg);i++){
				if ((msg[i] == '<') || (msg[i] == '>')){ 
					if (msg[i] == '<'){
						display_msg[z] = '&';
						z++;
						display_msg[z] = 'l';
						z++;
						display_msg[z] = 't';
						z++;
						display_msg[z] = ';';
						z++;
					}
					if (msg[i] == '>'){
						display_msg[z] = '&';
						z++;
						display_msg[z] = 'g';
						z++;
						display_msg[z] = 't';
						z++;
						display_msg[z] = ';';
						z++;
					}
				}else{
					display_msg[z] = msg[i];
					z++;
				}
           }
	      
	      //printf("@@@@print_index=[%d]",print_index);
	       if(print_index==1)
	         {
	        websWrite(wp, T("<tr>"
                            "<td nowrap><center>"
                                "<span   class=""table_left"">%d</span>"
                            "</center></td>"
                            "<td nowrap>"
                                "<span class=table_font>%s</span>"
                            "</td>"
                            "<td nowrap><span class=""table_right"">"
                            "</span></td>"
                          "</tr>"),linenum,display_msg);
             print_index=0;             
           }
          else
           {
           	--linenum;
          	print_index=0;
           }                
	   }
	 
  }
  free(tm);
  free(msg);
  free(display_msg);
  free(log1);
      		return 0;
error:
	if(!fp1)
		pclose(fp1);
	websDone(wp, 200);
	return 0;
}

int getMemLeftASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024], *semiColon, *key, *value;
	FILE *fp = fopen(PROC_MEM_STATISTIC, "r");
	if(!fp){
		websWrite(wp, T("no proc?\n"));
		return -1;
	}

	while(fgets(buf, 1024, fp)){
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';
		key = buf;
		value = semiColon + 1;
		if(!strcmp(key, "MemFree")){
			value = strip_space(value);
			websWrite(wp, T("%s"), value);
			fclose(fp);
			return 0;
		}
	}
	websWrite(wp, T(""));
	fclose(fp);
	return -1;
}

static void systemrebooting(webs_t wp, char_t *path, char_t *query)
{
 //char *lan_ip;
  //lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
  system("sleep 3 && reboot &");
	websHeader(wp);
	//websWrite(wp, T("please wait 40 seconds for reboot . . .\n"));
	websWrite(wp, T("<script>\n"));
	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/proceeding_reboot.asp';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	//websWrite(wp, T("self.setTimeout('refresh_all()', %s);\n"), REFRESH_TIMEOUT);
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
 
}

static void LoadDefaultSettings(webs_t wp, char_t *path, char_t *query)
{
        char *value;
        char buf[BUF_256];
        
        doSystem("nvram default");
        //To make sure the Web UI can redirect to default LAN IP
        ezplib_get_attr_val("lan_static_rule_default", 0, "ipaddr_normal", buf, TMP_LEN, EZPLIB_USE_CLI);
        value = buf;
        nvram_set("lan0_ipaddr", value);
        doSystem("nvram commit");
  //char *lan_ip;
  //lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
        system("sleep 3 && reboot &");
	websHeader(wp);
	//websWrite(wp, T("please wait 40 seconds for reboot . . .\n"));
	websWrite(wp, T("<script>\n"));
	
	websWrite(wp, T("function waitingpage(){\n"));
	websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
	websWrite(wp, T("}\n"));
	websWrite(wp, T("waitingpage();\n"));
	//websWrite(wp, T("self.setTimeout('refresh_all()', %s);\n"), REFRESH_TIMEOUT);
	websWrite(wp, T("</script>\n"));
	websFooter(wp);
	websDone(wp, 200);
}


/*
 * callee must free memory.
 */
/*
static char *getLog(char *filename)
{
	FILE *fp;
	struct stat filestat;
	char *log;

	if(stat(filename, &filestat) == -1)
		return NULL;

//	printf("%d\n", filestat.st_size);
	log = (char *)malloc(sizeof(char) * (filestat.st_size + 1) );
	if(!log)
		return NULL;

	if(!(fp = fopen(filename, "r"))){
		return NULL;
	}

	if( fread(log, 1, filestat.st_size, fp) != filestat.st_size){
		printf("read not enough\n");
		free(log);
		return NULL;
	}

	log[filestat.st_size] = '\0';

	fclose(fp);
	return log;
}
*/

static void clearlog(webs_t wp, char_t *path, char_t *query)
{
// AXIM: clear log
#if 0	
	doSystem("killall -q klogd");
	doSystem("killall -q syslogd");
	doSystem("syslogd -C8 1>/dev/null 2>&1");
	doSystem("klogd 1>/dev/null 2>&1");
#else
	system("/bin/ezp-rstlog");
#endif
	setWebMessage(0, NULL);
	websRedirect(wp, "/local/advance/syslog_gordon.asp");
}

void management_init(void)
{
	FILE *fp = NULL;
	doSystem("killall ntp.sh");
	doSystem("ntp.sh &");//modifyed by Gordon 20090226
	doSystem("ddns.sh");
// Tommy, move udps to execute after factory test ready and rcS file execute "udps&" 2009/10/23 11:34 
#if 1
	doSystem("udps &");//modifyed by Gordon 20090312
#endif	
	//WPSRestart();

//	doSystem("killall -q klogd");
//	doSystem("killall -q syslogd");
if (NULL == (fp = fopen("/tmp/syslodg_exist", "r")))
{
	doSystem("echo '1' >  /tmp/syslodg_exist");
	doSystem("syslogd -C8 1>/dev/null 2>&1");
// Tommy , enable klog,2009/10/15 02:54
	doSystem("klogd 1>/dev/null 2>&1");
}
//	doSystem("klogd 1>/dev/null 2>&1");
}

void management_fini(void)
{
	doSystem("killall -q klogd");
	doSystem("killall -q syslogd");
}

int checkAutoUploadFirmware(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp = NULL;
	char_t file_size[100];
	char_t file_date[15];
	
	//doSystem("ls -l /var/upload/zyfw_fw_file | sed 's/ \\/var.*$//g' | sed 's/^.* //g' > /var/upload/zyfw_fw_file_size");
	//----------- aron modify  2010.07.09
	/*
	doSystem("date -r /var/upload/zyfw_fw_file | cut -d ' ' -f 1 > /var/tempDate");
	if((fp = fopen("/var/tempDate", "r")) != NULL)
	{
		fscanf(fp, "%s", file_date);
		fclose(fp);
		doSystem("rm -rf /var/tempDate");
        }
	doSystem("ls -le /var/upload/zyfw_fw_file | sed 's/ \\%s.*$//g' | sed 's/^.* //g' > /var/upload/zyfw_fw_file_size", file_date);
	*/
	//doSystem("filename='/var/upload/zyfw_fw_file' ; ls '$filename' -al  | awk '{print $5}' > /var/upload/zyfw_fw_file_size");
	doSystem("ls /var/upload/zyfw_fw_file  -al  | awk '{print $5}' > /var/upload/zyfw_fw_file_size");
	//-------------------------------------
	if (NULL == (fp = fopen("/var/upload/zyfw_fw_file_size", "r")))
	{
		websWrite(wp, T("0"));
	}
	else
	{
		fscanf(fp, "%s", file_size);
		fclose(fp);
		if ((file_size[0]>'0')&&(file_size[0]<='9'))
			websWrite(wp, T("%s"), file_size);
		else	
			websWrite(wp, T("0"));
	}
	return 1;
}

int kill_wget(int eid, webs_t wp, int argc, char_t **argv)
{
	doSystem("killall wget");
	return 1;
}

#endif //Steve220

// Add for 3G +++++++++++++++++++
//
//	return : 0: not exist ; 1: exist
//
static int wan3GEnable=0;
int Check3GExist()
{
	char *value;
	FILE *fp;
	char wan3GLink[32];

	system("ifconfig | grep 'ppp0' > /tmp/ppp0.txt");
	if (NULL != (fp = fopen("/tmp/ppp0.txt", "r"))){
		fscanf(fp, "%s", wan3GLink);
		fclose(fp);
		system("rm -f /tmp/ppp0.txt");
	
		value = nvram_get("wan0_proto");
		if ((!strcmp(value, "wwan")) && (!strcmp(wan3GLink, "ppp0"))){
			wan3GEnable = 1;
			return 1;
		}else{
			wan3GEnable = 0;
			return 0;
		}
	}else{
		wan3GEnable = 0;
		return 0;
	}	
}

static int get3GSetting(int eid, webs_t wp, int argc, char_t **argv)
{
	char *value;
	value = nvram_get("wan0_proto");
	if (!strcmp(value, "wwan")){
// HOWTO : get3G.sh should be called by USB adapter pulgin. (AXIMCOM help)
		//system("/usr/sbin/get3G.sh");
		return websWrite(wp, T("1"));
	}else{
		return websWrite(wp, T("0"));
	}
}
static int getSIMstatus(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	char str_SIM[32];
	char str_Ready[32];

	if (Check3GExist() == 0){
		return websWrite(wp, T("0"));  //SIM Not Ready
	}
	
	if (NULL != (fp = fopen("/tmp/SIM.txt", "r")))	{
		fscanf(fp, "%s %s", str_SIM, str_Ready);
		fclose(fp);
		if (!strcmp(str_Ready, "ready")){
			return websWrite(wp, T("1")); // SIM ready
		}else{
			return websWrite(wp, T("0")); //SIM Not Ready
		}
	}else{
		return websWrite(wp, T("0")); //SIM Not Ready
	}
}
static int getWan3GIp(int eid, webs_t wp, int argc, char_t **argv)
{
	return websWrite(wp, T("1"));
}
static int getWan3GNetmask(int eid, webs_t wp, int argc, char_t **argv)
{
	return websWrite(wp, T("1"));
}
static int getWan3GDHCP(int eid, webs_t wp, int argc, char_t **argv)
{
	return websWrite(wp, T("DHCP"));
}

static int getWAN3GLinkStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	char *value;
	FILE *fp;
	char wan3GLink[32];

#if 0
	system("ifconfig | grep 'ppp0' > /tmp/ppp0.txt");
	if (NULL != (fp = fopen("/tmp/ppp0.txt", "r"))){
		fscanf(fp, "%s", wan3GLink);
		fclose(fp);
		system("rm -f /tmp/ppp0.txt");
	
		value = nvram_get("wan0_proto");
		if ((!strcmp(value, "wwan")) && (!strcmp(wan3GLink, "ppp0"))){
			return websWrite(wp, T("Up"));
		}else{
			return websWrite(wp, T("Down"));
		}
	}else{
		return websWrite(wp, T("Down"));
	}
#else
	if (wan3GEnable == 1){
		return websWrite(wp, T("Up"));
	}else{
		return websWrite(wp, T("Down"));	
	}
#endif
	
}
static int get3GConnStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[TMP_LEN];
		
	ezplib_get_attr_val("wan_wwan_rule", 0, "mode", buf, TMP_LEN, EZPLIB_USE_CLI);
	if (!strcmp(buf, "auto")){
		return websWrite(wp, T("AUTO"));
	}else if (!strcmp(buf, "hsdpa")){
		return websWrite(wp, T("HSDPA / 3.5G"));
	}else if (!strcmp(buf, "umts")){
		return websWrite(wp, T("UMTS / 3G"));
	}else if (!strcmp(buf, "edge")){
		return websWrite(wp, T("EDGE / 2.5G"));
	}else if (!strcmp(buf, "gprs")){
		return websWrite(wp, T("GPRS / 2G"));
	}else{
		return websWrite(wp, T("Un-known"));
	} 
}

static char wan3GISP[128];
static int get3GISP(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;

	if (wan3GEnable == 0){
		return websWrite(wp, T(""));
	}

	//char cmd[512];
	//system("cat /tmp/reg.txt | grep 'Registered on Home network:' | sed 's/^.*Registered on Home network: //g' | sed 's/^.*\"\\(.*\\)\".*$/\\1/'> /tmp/ISP.txt");
	//sprintf(cmd, "%s", "cat /tmp/reg.txt | grep 'Registered on Home network:' | sed 's/^.*Registered on Home network: //g' | sed 's/^.*\"\(.*\)\".*$/\1/' > /tmp/ISP.txt");
	//system(cmd);
	system("cat /tmp/reg.txt | grep 'Registered on Home network:' | sed 's/^.*Registered on Home network: //g' > /tmp/ISP.txt");
	system("cat /tmp/ISP.txt | sed 's/^.*\"\\(.*\\)\".*$/\\1/' > /tmp/getISP.txt");
	
	if (NULL != (fp = fopen("/tmp/getISP.txt", "r"))){
		fscanf(fp, "%s", wan3GISP);
		fclose(fp);
		system("rm -f /tmp/ISP.txt");
		system("rm -f /tmp/getISP.txt");
		return websWrite(wp, T("%s"),wan3GISP);
	}else{
		return websWrite(wp, T(""));
	}
}

static int wan3GSSI;
static int get3GSignalStrength(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	char cmd[512];
	char *value;
	char wan3GLink[32];

	if (wan3GEnable == 0){
		return websWrite(wp, T("0"));
	}

	system("/usr/sbin/getSSI.sh");
	system("cat /tmp/sig.txt | grep 'Signal Quality:' | sed 's/^.*Signal Quality: //g' > /tmp/SSI.txt");
	system("cat /tmp/SSI.txt | sed 's/,.*$//g' > /tmp/getSSI.txt");
	
	//system("cat /tmp/sig.txt | grep 'Signal Quality:' | sed 's/^.*Signal Quality: //g' | sed 's/,.*$//g' > /tmp/SSI.txt");
	//sprintf(cmd, "%s", "cat /tmp/sig.txt | grep 'Signal Quality:' | sed 's/^.*Signal Quality: //g' | sed 's/,.*$//g' > /tmp/SSI.txt");
	//system(cmd);

	if (NULL != (fp = fopen("/tmp/getSSI.txt", "r"))){
		fscanf(fp, "%d", &wan3GSSI);
		fclose(fp);
		system("rm -f /tmp/SSI.txt");
		system("rm -f /tmp/getSSI.txt");
		system("rm -f /tmp/sig.txt");
		if ((wan3GSSI >= 0) || (wan3GSSI <= 31)){
			wan3GSSI = 113 - wan3GSSI*2;
		}else{
			wan3GSSI = 113;		
		}
		return websWrite(wp, T("-%d"),wan3GSSI);
	}else{
		return websWrite(wp, T(""));
	}
}
static int get3GConnConnUpTime(int eid, webs_t wp, int argc, char_t **argv)
{
	int updays, uphours, upminutes, upseconds;	
	unsigned long wanuptime, disptime;
	char TempBuf[32];

	if (wan3GEnable == 0){
		return websWrite(wp, T(""));
	}
	
	ezplib_get_attr_val("wan_st_rule", 0, "uptime", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "")){
		strcpy(TempBuf, "0");
	}
	wanuptime = atol(TempBuf);
	disptime = wanuptime;
	
	updays = (int) disptime / (60*60*24);
	upminutes = (int) disptime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	upseconds = ((int) disptime)%60;

	if (updays){
		return websWrite(wp, T("%d day%s, %s%d:%s%d:%s%d") , 
		updays, (updays != 1) ? "s" : "",
		(uphours < 10) ? "0" : "",uphours,
		(upminutes < 10) ? "0" : "",upminutes,
		(upseconds < 10) ? "0" : "",upseconds);
	}else{
		return websWrite(wp, T("%s%d:%s%d:%s%d") ,
		(uphours < 10) ? "0" : "",uphours,
		(upminutes < 10) ? "0" : "",upminutes,
		(upseconds < 10) ? "0" : "",upseconds);	
	}
}

static char wan3Gmanufacturer[128];
static int get3GManufacturer(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;

	if (wan3GEnable == 0){
		return websWrite(wp, T(""));
	}
	
	system("cat /tmp/info.txt | grep 'Manufacturer:' | sed 's/^.*Manufacturer: //g' > /tmp/manufacturer.txt");
	if (NULL != (fp = fopen("/tmp/manufacturer.txt", "r"))){
		fscanf(fp, "%s", wan3Gmanufacturer);
		fclose(fp);
		system("rm -f /tmp/manufacturer.txt");
		return websWrite(wp, T("%s"),wan3Gmanufacturer);
	}else{
		return websWrite(wp, T(""));
	}
}
static char wan3Gmodel[128];
static int get3GModel(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;

	if (wan3GEnable == 0){
		return websWrite(wp, T(""));
	}

	system("cat /tmp/info.txt | grep 'Model:' | sed 's/^.*Model: //g' > /tmp/Model.txt");
	if (NULL != (fp = fopen("/tmp/Model.txt", "r"))){
		fscanf(fp, "%s", wan3Gmodel);
		fclose(fp);
		system("rm -f /tmp/Model.txt");
		return websWrite(wp, T("%s"),wan3Gmodel);
	}else{
		return websWrite(wp, T(""));
	}
}
static char wan3GRev[128];
static int get3GFW(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;

	if (wan3GEnable == 0){
		return websWrite(wp, T(""));
	}
	
	system("cat /tmp/info.txt | grep 'Revision:' | sed 's/^.*Revision: //g' > /tmp/Rev.txt");
	if (NULL != (fp = fopen("/tmp/Rev.txt", "r"))){
		fscanf(fp, "%s", wan3GRev);
		fclose(fp);
		system("rm -f /tmp/Rev.txt");
		return websWrite(wp, T("%s"),wan3GRev);
	}else{
		return websWrite(wp, T(""));
	}
}
static char wan3GCardIEMI[128];
static int get3GIMEI(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;

	if (wan3GEnable == 0){
		return websWrite(wp, T(""));
	}
	
	system("cat /tmp/info.txt | grep 'IMEI:' | sed 's/^.*IMEI: //g' > /tmp/CardIEMI.txt");
	if (NULL != (fp = fopen("/tmp/CardIEMI.txt", "r"))){
		fscanf(fp, "%s", wan3GCardIEMI);
		fclose(fp);
		system("rm -f /tmp/CardIEMI.txt");
		return websWrite(wp, T("%s"),wan3GCardIEMI);
	}else{
		return websWrite(wp, T(""));
	}
}
static int getSIMIMEI(int eid, webs_t wp, int argc, char_t **argv)
{
	return websWrite(wp, T(""));
}
// Add for 3G -------------------


//=================================================================

void formDefineManagement(void)
{
	
	websFormDefine(T("setSysAdm"), setSysAdm);
	websFormDefine(T("setSysLang"), setSysLang);
	websFormDefine(T("logsetting"), logsetting);
	websFormDefine(T("showlog"), showlog);
	websFormDefine(T("NTP"), NTP);
	websFormDefine(T("NTPSyncWithHost"), NTPSyncWithHost);
        websAspDefine(T("getCurrentYear"), getCurrentYear);
	websAspDefine(T("getCurrentMon"), getCurrentMon);
	websAspDefine(T("getCurrentDay"), getCurrentDay);
	websAspDefine(T("getCurrentHour"), getCurrentHour);
	websAspDefine(T("getCurrentMin"), getCurrentMin);
	websAspDefine(T("getCurrentSec"), getCurrentSec); 	
        //websAspDefine(T("getCurrentTimeASP"), getCurrentTimeASP);
	//websAspDefine(T("getCurrentDateASP"), getCurrentDateASP);//Gordon added  
	websAspDefine(T("getSystemTime"), getSystemTime);//Gordon added  20090203
	websAspDefine(T("getSystemDate"), getSystemDate);//Gordon added  20090203
	websAspDefine(T("loglist"), loglist);//Gordon added 
	websFormDefine(T("DDNS"), DDNS);

	websAspDefine(T("getMemLeftASP"), getMemLeftASP);
	websAspDefine(T("getMemTotalASP"), getMemTotalASP);


	websAspDefine(T("GetIfBytesPerSecASP"), GetIfBytesPerSecASP);
	websAspDefine(T("getWANRxByteASP"), getWANRxByteASP);
	websAspDefine(T("getWANTxByteASP"), getWANTxByteASP);
	websAspDefine(T("getLANRxByteASP"), getLANRxByteASP);
	websAspDefine(T("getLANTxByteASP"), getLANTxByteASP);
	websAspDefine(T("getWANRxPacketASP"), getWANRxPacketASP);
	websAspDefine(T("getWANTxPacketASP"), getWANTxPacketASP);
	websAspDefine(T("getLANRxPacketASP"), getLANRxPacketASP);
	websAspDefine(T("getLANTxPacketASP"), getLANTxPacketASP);

// Tommy, Add WLAN Statistics, 2009/2/4 09:23
#if 1
	websAspDefine(T("getWLANRxByteASP"), getWLANRxByteASP);
	websAspDefine(T("getWLANTxByteASP"), getWLANTxByteASP);
	websAspDefine(T("getWLANRxPacketASP"), getWLANRxPacketASP);
	websAspDefine(T("getWLANTxPacketASP"), getWLANTxPacketASP);
	
	websAspDefine(T("getWANLinkStatus"), getWANLinkStatus);
	websAspDefine(T("getWANUpTime"), getWANUpTime);

	websAspDefine(T("getLANUpTime"), getLANUpTime);
	websAspDefine(T("getWLANLinkStatus"), getWLANLinkStatus);
	websAspDefine(T("getWLANUpTime"), getWLANUpTime);		
	
	
#endif	
	
// Tommy, Add WAN/LAN/WLAN Collisions field, 2009/2/4 09:23
#if 1
	websAspDefine(T("getWANCollsASP"), getWANCollsASP);
	websAspDefine(T("getLANCollsASP"), getLANCollsASP);
	websAspDefine(T("getWLANCollsASP"), getWLANCollsASP);
#endif

	websAspDefine(T("getAllNICStatisticASP"), getAllNICStatisticASP);

	websAspDefine(T("showSystemCommandASP"), showSystemCommandASP);
	websFormDefine(T("SystemCommand"), SystemCommand);

	websFormDefine(T("LoadDefaultSettings"), LoadDefaultSettings);
	websFormDefine(T("systemrebooting"), systemrebooting);//Gordon added 


	websFormDefine(T("clearlog"), clearlog);
	websFormDefine(T("loglist"), loglist);//Gordon added
#if 1//Arthur Chow 2008-12-16: For login.asp/login_fail.asp
	websFormDefine(T("web_login"), web_login);
	websFormDefine(T("setSysPass"), setSysPass);
	websFormDefine(T("setSysPassEasy"), setSysPassEasy);
	websFormDefine(T("setSysPassLogin"), setSysPassLogin);
	websFormDefine(T("maintenance_general"), maintenance_general);
#endif	
#if 1//Arthur Chow 2009-02-04: For detecting/checking wan type
	websAspDefine(T("detectEthernetWanType"), detectEthernetWanType);
	websAspDefine(T("checkEthernetWanType"), checkEthernetWanType);
	websAspDefine(T("set_language"), set_language);
#endif	
#if 1//Arthur Chow 2009-02-09: For get Yahoo Weather
	websAspDefine(T("detectWeather"), detectWeather);
	websAspDefine(T("checkWeather"), checkWeather);
#endif	
#if 1//Arthur Chow 2009-03-02: For getWebMessage
	websAspDefine(T("getWebMessage"), getWebMessage);
	websAspDefine(T("getWebMessageFlag"), getWebMessageFlag);
#endif	
	websAspDefine(T("checkAutoUploadFirmware"), checkAutoUploadFirmware);
	websAspDefine(T("kill_wget"), kill_wget);

// Add for 3G +++++++++++++++
	websAspDefine(T("get3GSetting"), get3GSetting);
	websAspDefine(T("getSIMstatus"), getSIMstatus);
	websAspDefine(T("getWan3GIp"), getWan3GIp);
	websAspDefine(T("getWan3GNetmask"), getWan3GNetmask);
	websAspDefine(T("getWan3GDHCP"), getWan3GDHCP);
	websAspDefine(T("getWAN3GLinkStatus"), getWAN3GLinkStatus);
	websAspDefine(T("get3GConnStatus"), get3GConnStatus);	
	websAspDefine(T("get3GISP"), get3GISP);
	websAspDefine(T("getWan3GIp"), getWan3GIp);
	websAspDefine(T("get3GSignalStrength"), get3GSignalStrength);
	websAspDefine(T("get3GConnConnUpTime"), get3GConnConnUpTime);
	websAspDefine(T("get3GManufacturer"), get3GManufacturer);
	websAspDefine(T("get3GModel"), get3GModel);
	websAspDefine(T("get3GFW"), get3GFW);
	websAspDefine(T("get3GIMEI"), get3GIMEI);
	websAspDefine(T("getSIMIMEI"), getSIMIMEI);
// Add for 3G ---------------
	formDefineWPS();
}
