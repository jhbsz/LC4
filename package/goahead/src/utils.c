/* vi: set sw=4 ts=4 sts=4: */
/*
 *	utils.c -- System Utilities
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: utils.c,v 1.92.4.1 2008-07-29 13:02:54 steven Exp $
 */
#include	<time.h>
#include	<signal.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>

#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include  <sys/sysinfo.h>

#include	"webs.h"
#include	"utils.h"
#include <bcmnvram.h>
//#include	"wireless.h"
//#include	"internet.h"


//#include	"sdk_version.h"
//#include	"nvram.h"
//#include	"ralink_gpio.h"
#include       "led_gpio.h"
//#include	"linux/autoconf.h"  //kernel config
//#include	"config/autoconf.h" //user config

#include "ezp-lib.h"
#define TMP_LEN 256 //aron patch for giga, from httpd common.h

#if 1 //Steve2023
//#if 1//Arthur Chow 2009-03-30
//#include	"sdk_version.h"
//#endif
static	char reValue[256];
#define SHORT_BUF_LEN   32

static int  getCfgGeneral(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfgNthGeneral(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfgZero(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfgNthZero(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg2General(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg2NthGeneral(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg2Zero(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg2NthZero(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg3General(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg3Zero(int eid, webs_t wp, int argc, char_t **argv);
static int  getDpbSta(int eid, webs_t wp, int argc, char_t **argv);
static int  getLangBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int  getMiiInicBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int  getPlatform(int eid, webs_t wp, int argc, char_t **argv);
static int  getStationBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int  getSysBuildTime(int eid, webs_t wp, int argc, char_t **argv);
static int  getSdkVersion(int eid, webs_t wp, int argc, char_t **argv);
static int  getSysUptime(int eid, webs_t wp, int argc, char_t **argv);
#if 1//Arthur Chow 2009-01-03
static int  getSysResource(int eid, webs_t wp, int argc, char_t **argv);
#endif
static int  getPortStatus(int eid, webs_t wp, int argc, char_t **argv);
static int  isOnePortOnly(int eid, webs_t wp, int argc, char_t **argv);
static void forceMemUpgrade(webs_t wp, char_t *path, char_t *query);
static void setOpMode(webs_t wp, char_t *path, char_t *query);
//Tommy, 2008/12/16 08:14下午
static void SetOperationMode(webs_t wp, char_t *path, char_t *query);
// Steve, 2009/01/16 02:45下午
static int  getCfgLLTD(int eid, webs_t wp, int argc, char_t **argv);
#if 1//Arthur Chow 2009-02-15: For easy mode page
static int  startLLTDcheck(int eid, webs_t wp, int argc, char_t **argv);
static int  startInternetcheck(int eid, webs_t wp, int argc, char_t **argv);
static int  statusRoutercheck(int eid, webs_t wp, int argc, char_t **argv);
static int  statusInternetcheck(int eid, webs_t wp, int argc, char_t **argv);
static int  javascriptLLTD(int eid, webs_t wp, int argc, char_t **argv);
static void easy_setting(webs_t wp, char_t *path, char_t *query);
static int  clearLLTDInfo(int eid, webs_t wp, int argc, char_t **argv);
static int  existLLTDinfo(int eid, webs_t wp, int argc, char_t **argv);
#endif
#if 1//Arthur Chow 2009-03-30
static int  getFirmwareVersion(int eid, webs_t wp, int argc, char_t **argv);
static int  getFirmwareDate(int eid, webs_t wp, int argc, char_t **argv);
#endif
//Steve 2010-08-27
static int  getGetUploadFlag(int eid, webs_t wp, int argc, char_t **argv);

/*********************************************************************
 * System Utilities
 */

//aron patch for giga
char* prefix2mask(int idx)
{ 
    char *mask;
    switch(idx) //Translate AXIM's netmask to AboCom's UI format
    {
        case 0:
	        mask="0.0.0.0";
	        break;
	case 1:
	        mask="128.0.0.0";
	        break;
	case 2:
	        mask="192.0.0.0";
	        break;
	case 3:
	        mask="224.0.0.0";
	        break;
	case 4:
	        mask="240.0.0.0";
	        break;
	case 5:
	        mask="248.0.0.0";
	        break;
	case 6:
	        mask="252.0.0.0";
	        break;
	case 7:
	        mask="254.0.0.0";
	        break;    
        case 8:
	        mask="255.0.0.0";
	        break;
	case 9:
	        mask="255.128.0.0";
	        break;
	case 10:
	        mask="255.192.0.0";
	        break;
	case 11:
	        mask="255.224.0.0";
	        break;
	case 12:
	        mask="255.240.0.0";
	        break;
	case 13:
	        mask="255.248.0.0";
	        break;
	case 14:
	        mask="255.252.0.0";
	        break;
	case 15:
	        mask="255.254.0.0";
	        break;
        case 16:
	        mask="255.255.0.0";
	        break;
	case 17:
	        mask="255.255.128.0";
	        break;
	case 18:
	        mask="255.255.192.0";
	        break;
	case 19:
	        mask="255.255.224.0";
	        break;
	case 20:
	        mask="255.255.240.0";
	        break;
	case 21:
	        mask="255.255.248.0";
	        break;
	case 22:
	        mask="255.255.252.0";
	        break;
	case 23:
	        mask="255.255.254.0";
	        break;
	case 24:
	        mask="255.255.255.0";
	        break;
	case 25:
	        mask="255.255.255.128";
	        break;
	case 26:
	        mask="255.255.255.192";
	        break;
	case 27:
	        mask="255.255.255.224";
	        break;
	case 28:
	        mask="255.255.255.240";
	        break;
	case 29:
	        mask="255.255.255.248";
	        break;
	case 30:
	        mask="255.255.255.252";
	        break;
	case 31:
	        mask="255.255.255.254";
	        break;
	case 32:
	        mask="255.255.255.255";
	        break;
	default:
	        mask="";
    }
          
    return mask;
}

void arplookup(char *ip, char *arp)
{
    char buf[256];
    FILE *fp = fopen("/proc/net/arp", "r");
    if(!fp){
        trace(0, T("no proc fs mounted!\n"));
        return;
    }
    strcpy(arp, "00:00:00:00:00:00");
    while(fgets(buf, 256, fp)){
        char ip_entry[32], hw_type[8],flags[8], hw_address[32];
        sscanf(buf, "%s %s %s %s", ip_entry, hw_type, flags, hw_address);
        if(!strcmp(ip, ip_entry)){
            strcpy(arp, hw_address);
            break;
        }
    }

    fclose(fp);
    return;
}


/*
 * description: kill process whose pid was recorded in file
 *              (va is supported)
 */
int doKillPid(char_t *fmt, ...)
{
	va_list		vargs;
	char_t		*pid_fname = NULL;
	struct stat	st;

	va_start(vargs, fmt);
	if (fmtValloc(&pid_fname, WEBS_BUFSIZE, fmt, vargs) >= WEBS_BUFSIZE) {
		trace(0, T("doKillPid: lost data, buffer overflow\n"));
	}
	va_end(vargs);

	if (pid_fname) {
		if (-1 == stat(pid_fname, &st)) //check the file existence
			return 0;
		doSystem("kill `cat %s`", pid_fname);
		doSystem("rm -f %s", pid_fname);
		bfree(B_L, pid_fname);
	}
	return 0;
}

/*
 * description: parse va and do system
 */
int doSystem(char_t *fmt, ...)
{
	va_list		vargs;
	char_t		*cmd = NULL;
	int			rc = 0;

	va_start(vargs, fmt);
	if (fmtValloc(&cmd, WEBS_BUFSIZE, fmt, vargs) >= WEBS_BUFSIZE) {
		trace(0, T("doSystem: lost data, buffer overflow\n"));
	}
	va_end(vargs);

	if (cmd) {
		trace(0, T("%s\n"), cmd);
		rc = system(cmd);
		bfree(B_L, cmd);
	}
	return rc;
}

/*
 * arguments: modname - module name
 * description: test the insertion of module through /proc/modules
 * return: -1 = fopen error, 1 = module inserted, 0 = not inserted yet
 */
int getModIns(char *modname)
{
	FILE *fp;
	char buf[128];
	int i;

	if (NULL == (fp = fopen("/proc/modules", "r"))) {
		error(E_L, E_LOG, T("getModIns: open /proc/modules error"));
		return -1;
	}

	while (NULL != fgets(buf, 128, fp)) {
		i = 0;
		while (!isspace(buf[i++]))
			;
		buf[i - 1] = '\0';
		if (!strcmp(buf, modname)) {
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	//error(E_L, E_LOG, T("getModIns: module %s not inserted"), modname);
	return 0;
}

/*
 * arguments: index - index of the Nth value
 *            values - un-parsed values
 * description: parse values delimited by semicolon, and return the value
 *              according to given index (starts from 0)
 * WARNING: the original values will be destroyed by strtok
 */
char *getNthValue(int index, char *values)
{
	int i;
	static char *tok;

	if (NULL == values)
		return NULL;
	for (i = 0, tok = strtok(values, ";");
			(i < index) && tok;
			i++, tok = strtok(NULL, ";")) {
		;
	}
	if (NULL == tok)
		return "";
	return tok;
}

/*
 * arguments: index - index of the Nth value (starts from 0)
 *            old_values - un-parsed values
 *            new_value - new value to be replaced
 * description: parse values delimited by semicolon,
 *              replace the Nth value with new_value,
 *              and return the result
 * WARNING: return the internal static string -> use it carefully
 */
char *setNthValue(int index, char *old_values, char *new_value)
{
	int i;
	char *p, *q;
	static char ret[2048];
	char buf[8][256];

	memset(ret, 0, 2048);
	for (i = 0; i < 8; i++)
		memset(buf[i], 0, 256);

	//copy original values
	for ( i = 0, p = old_values, q = strchr(p, ';')  ;
	      i < 8 && q != NULL                         ;
	      i++, p = q + 1, q = strchr(p, ';')         )
	{
		strncpy(buf[i], p, q - p);
	}
	strcpy(buf[i], p); //the last one

	//replace buf[index] with new_value
	strncpy(buf[index], new_value, 256);

	//calculate maximum index
	index = (i > index)? i : index;

	//concatenate into a single string delimited by semicolons
	strcat(ret, buf[0]);
	for (i = 1; i <= index; i++) {
		strncat(ret, ";", 2);
		strncat(ret, buf[i], 256);
	}

	p = ret;
	return p;
}

/*
 * arguments: values - values delimited by semicolon
 * description: parse values delimited by semicolon, and return the number of
 *              values
 */
int getValueCount(char *values)
{
	int cnt = 0;

	if (NULL == values)
		return 0;
	while (*values++ != '\0') {
		if (*values == ';')
			++cnt;
	}
	return (cnt + 1);
}

/*
 * check the existence of semicolon in str
 */
int checkSemicolon(char *str)
{
	char *c = strchr(str, ';');
	if (c)
		return 1;
	return 0;
}

/*
 * argument: ip address
 * return: 1 = the given ip address is within LAN's scope
 *         0 = otherwise
 */
#if 0
int isInLan(char *radius_ip_addr)
{
    char lan_if_addr[16];
    char lan_if_netmask[16];

    struct in_addr lan_ip;
    struct in_addr lan_netmask;
    struct in_addr radius_ip;

    if ((getIfIp(getLanIfName(), lan_if_addr)) == -1) {
        printf("getLanIP error\n");
        return 0;
    }
    if ((getIfNetmask(getLanIfName(), lan_if_netmask)) == -1) {
        printf("getLanNetmask error\n");
        return 0;
    }

    inet_aton(lan_if_addr, &lan_ip);
    inet_aton(lan_if_netmask, &lan_netmask);
    inet_aton(radius_ip_addr, &radius_ip);

    printf("lan_ip:%08x\n", lan_ip.s_addr);
    printf("lan_netmask:%08x\n", lan_netmask.s_addr);
    printf("radius_ip:%08x\n", radius_ip.s_addr);

    if ((lan_ip.s_addr & lan_netmask.s_addr) == (radius_ip.s_addr & lan_netmask.s_addr) ){
        printf("in Lan\n");
        return 1;
    } else {
        printf("not in lan\n");
        return 0;
    }
}
#endif

/*
 * substitution of getNthValue which dosen't destroy the original value
 */
int getNthValueSafe(int index, char *value, char delimit, char *result, int len)
{
    int i=0, result_len=0;
    char *begin, *end;

    if(!value || !result || !len)
        return -1;

    begin = value;
    end = strchr(begin, delimit);

    while(i<index && end){
        begin = end+1;
        end = strchr(begin, delimit);
        i++;
    }

    //no delimit
    if(!end){
		if(i == index){
			end = begin + strlen(begin);
			result_len = (len-1) < (end-begin) ? (len-1) : (end-begin);
		}else
			return -1;
	}else
		result_len = (len-1) < (end-begin)? (len-1) : (end-begin);

	memcpy(result, begin, result_len );
	*(result+ result_len ) = '\0';

	return 0;
}

/*
 *  argument:  [IN]     index -- the index array of deleted items(begin from 0)
 *             [IN]     count -- deleted itmes count.
 *             [IN/OUT] value -- original string/return string
 *             [IN]     delimit -- delimitor
 */
int deleteNthValueMulti(int index[], int count, char *value, char delimit)
{
	char *begin, *end;
	int i=0,j=0;
	int need_check_flag=0;
	char *buf = strdup(value);

	begin = buf;

	end = strchr(begin, delimit);
	while(end){
		if(i == index[j]){
			memset(begin, 0, end - begin );
			if(index[j] == 0)
				need_check_flag = 1;
			j++;
			if(j >=count)
				break;
		}
		begin = end;

		end = strchr(begin+1, delimit);
		i++;
	}

	if(!end && index[j] == i)
		memset(begin, 0, strlen(begin));

	if(need_check_flag){
		for(i=0; i<strlen(value); i++){
			if(buf[i] == '\0')
				continue;
			if(buf[i] == ';')
				buf[i] = '\0';
			break;
		}
	}

	for(i=0, j=0; i<strlen(value); i++){
		if(buf[i] != '\0'){
			value[j++] = buf[i];
		}
	}
	value[j] = '\0';

	free(buf);
	return 0;
}



/*
 * nanosleep(2) don't depend on signal SIGALRM and could cooperate with
 * other SIGALRM-related functions(ex. setitimer(2))
 */
unsigned int Sleep(unsigned int secs)
{
	int rc;
	struct timespec ts, remain;
	ts.tv_sec  = secs;
	ts.tv_nsec = 0;

sleep_again:
	rc = nanosleep(&ts, &remain);
	if(rc == -1 && errno == EINTR){
		ts.tv_sec = remain.tv_sec;
		ts.tv_nsec = remain.tv_nsec;
		goto sleep_again;
	}
	return 0;
}

/*
 * The setitimer() is Linux-specified.
 */
int setTimer(int microsec, void ((*sigroutine)(int)))
{
	struct itimerval value, ovalue;

	signal(SIGALRM, sigroutine);
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = microsec;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = microsec;
	return setitimer(ITIMER_REAL, &value, &ovalue);
}

void stopTimer(void)
{
	struct itimerval value, ovalue;

	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &value, &ovalue);
}

/*
 * configure LED blinking with proper frequency (privatly use only)
 *   on: number of ticks that LED is on
 *   off: number of ticks that LED is off
 *   blinks: number of on/off cycles that LED blinks
 *   rests: number of on/off cycles that LED resting
 *   times: stop blinking after <times> times of blinking
 * where 1 tick == 100 ms
 */

#if 1 // Tommy debug +++++++++++++++++++++++ 
 
static int gpioLedSet(int gpio, unsigned int on, unsigned int off,
		unsigned int blinks, unsigned int rests, unsigned int times)
{
	int fd;
	ralink_gpio_led_info led;

	//parameters range check
	if (gpio < 0 || gpio >= RALINK_GPIO_NUMBER ||
			on > RALINK_GPIO_LED_INFINITY ||
			off > RALINK_GPIO_LED_INFINITY ||
			blinks > RALINK_GPIO_LED_INFINITY ||
			rests > RALINK_GPIO_LED_INFINITY ||
			times > RALINK_GPIO_LED_INFINITY) {
		return -1;
	}
	led.gpio = gpio;
	led.on = on;
	led.off = off;
	led.blinks = blinks;
	led.rests = rests;
	led.times = times;

	fd = open("/dev/gpio", O_RDONLY);
	if (fd < 0) {
		perror("/dev/gpio");
		return -1;
	}
	if (ioctl(fd, RALINK_GPIO_LED_SET, &led) < 0) {
		perror("ioctl");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int ledAlways(int gpio, int on)
{
	if (on)
		return gpioLedSet(gpio, RALINK_GPIO_LED_INFINITY, 0, 1, 1, RALINK_GPIO_LED_INFINITY);
	else
		return gpioLedSet(gpio, 0, RALINK_GPIO_LED_INFINITY, 1, 1, RALINK_GPIO_LED_INFINITY);
}

int ledWps(int gpio, int mode)
{
	switch (mode) {
		case WPS_LED_RESET:
			return gpioLedSet(gpio, 0, RALINK_GPIO_LED_INFINITY, 1, 1, RALINK_GPIO_LED_INFINITY);
			break;
		case WPS_LED_PROGRESS:
			return gpioLedSet(gpio, 2, 1, RALINK_GPIO_LED_INFINITY, 1, RALINK_GPIO_LED_INFINITY);
			break;
		case WPS_LED_ERROR:
			return gpioLedSet(gpio, 1, 1, RALINK_GPIO_LED_INFINITY, 1, RALINK_GPIO_LED_INFINITY);
			break;
		case WPS_LED_SESSION_OVERLAP:
			return gpioLedSet(gpio, 1, 1, 10, 5, RALINK_GPIO_LED_INFINITY);
			break;
		case WPS_LED_SUCCESS:
			gpioLedSet(gpio, 3000, 1, 1, 1, 1);
			break;
	}
	return 0;
}
#endif // Tommy debug -----------------------------

/*
 * concatenate a string with an integer
 * ex: racat("SSID", 1) will return "SSID1"
 */
char *racat(char *s, int i)
{
	static char str[32];
	snprintf(str, 32, "%s%1d", s, i);
	return str;
}

void websLongWrite(webs_t wp, char *longstr)
{
    char tmp[513] = {0};
    int len = strlen(longstr);
    char *end = longstr + len;

    while(longstr < end){
        strncpy(tmp, longstr, 512);
        websWrite(wp, T("%s"), tmp);
        longstr += 512;
    }
    return;
}

/*********************************************************************
 * Web Related Utilities
 */

void formDefineUtilities(void)
{
	websAspDefine(T("getCfgGeneral"), getCfgGeneral);
	websAspDefine(T("getCfgNthGeneral"), getCfgNthGeneral);
	websAspDefine(T("getCfgZero"), getCfgZero);
	websAspDefine(T("getCfgNthZero"), getCfgNthZero);
	websAspDefine(T("getCfg2General"), getCfg2General);
	websAspDefine(T("getCfg2NthGeneral"), getCfg2NthGeneral);
	websAspDefine(T("getCfg2Zero"), getCfg2Zero);
	websAspDefine(T("getCfg2NthZero"), getCfg2NthZero);
	websAspDefine(T("getCfg3General"), getCfg3General);
	websAspDefine(T("getCfg3Zero"), getCfg3Zero);
	websAspDefine(T("getDpbSta"), getDpbSta);
	websAspDefine(T("getLangBuilt"), getLangBuilt);
	websAspDefine(T("getMiiInicBuilt"), getMiiInicBuilt);
	websAspDefine(T("getPlatform"), getPlatform);
	websAspDefine(T("getStationBuilt"), getStationBuilt);
	websAspDefine(T("getSysBuildTime"), getSysBuildTime);
//	websAspDefine(T("getSdkVersion"), getSdkVersion);
	websAspDefine(T("getSysUptime"), getSysUptime);
#if 1//Arthur Chow 2009-01-03
	websAspDefine(T("getSysResource"), getSysResource);
#endif
	websAspDefine(T("getPortStatus"), getPortStatus);
	websAspDefine(T("isOnePortOnly"), isOnePortOnly);
//	websFormDefine(T("forceMemUpgrade"), forceMemUpgrade);
//	websFormDefine(T("setOpMode"), setOpMode);
// Tommy, 2008/12/16 08:14下午
	websFormDefine(T("SetOperationMode"), SetOperationMode);
// Steve, 2009/01/16 02:45下午
	websAspDefine(T("getCfgLLTD"), getCfgLLTD);
#if 1//Arthur Chow 2009-02-15: For easy mode page
	websAspDefine(T("startLLTDcheck"), startLLTDcheck);
	websAspDefine(T("startInternetcheck"), startInternetcheck);
	websAspDefine(T("statusRoutercheck"), statusRoutercheck);
	websAspDefine(T("statusInternetcheck"), statusInternetcheck);
	websAspDefine(T("javascriptLLTD"), javascriptLLTD);
	websFormDefine(T("easy_setting"), easy_setting);
	websAspDefine(T("clearLLTDInfo"), clearLLTDInfo);
	websAspDefine(T("existLLTDinfo"), existLLTDinfo);
#endif
#if 1//Arthur Chow 2009-03-30
	websAspDefine(T("getFirmwareVersion"), getFirmwareVersion);
	websAspDefine(T("getFirmwareDate"), getFirmwareDate);
#endif
//Steve add 2010-08-27
	websAspDefine(T("getGetUploadFlag"), getGetUploadFlag);
}


/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> ""
 */
static int getCfgGeneral(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;
	char TempBuf[32];
        char buf[TMP_LEN]; //aron add
	int ret; //aron add
	int idx; //aron add

	int i;//Steve
	char buf2[TMP_LEN]; //Steve add
	char *pTempBuf;	

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	
	//Steve
	printf("\n ---> getCfgGeneral(): type = %d & field = %s",type,field);
	if (!strcmp(field, "Language")){
		/* Get the setting of value from AXIMCom's nvram structure into reValue */
		value = nvram_safe_get("lang");
	}else if (!strcmp(field, "WeatherDegree")){ 
		ezplib_get_attr_val("weather_rule", 0, "degree", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;	
	}else if (!strcmp(field, "WeatherCity")){ 
		ezplib_get_attr_val("weather_rule", 0, "city", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;	
	}else if (!strcmp(field, "SystemName")){
		value = nvram_safe_get("hostname");
	}else if (!strcmp(field, "SSID1")){
		ezplib_get_attr_val("wl0_ssid_rule", 0, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}else if (!strcmp(field, "SSID2")){
		ezplib_get_attr_val("wl0_ssid_rule", 1, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}else if (!strcmp(field, "SSID3")){
		ezplib_get_attr_val("wl0_ssid_rule", 2, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}else if (!strcmp(field, "SSID4")){
		ezplib_get_attr_val("wl0_ssid_rule", 3, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}else if (!strcmp(field, "lan_ipaddr")){ 
		value = nvram_safe_get("lan0_ipaddr");
	}else if (!strcmp(field, "lan_netmask")){
		value = nvram_safe_get("lan0_mask");
		idx = atoi(value);
		value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
    else if (!strcmp(field, "fr_rule_num")){ 
		value = nvram_safe_get("fr_rule_num");
	}
	else if (!strcmp(field, "RemoteManagementPort")){
		//Because debug,temp
		#if 1
		ezplib_get_attr_val("http_rule", 0, "port", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;	
	    #else
	    	strcpy(reValue, "80");
	    	value = reValue;
	    #endif	
	}
	else if (!strcmp(field, "RemoteManagementSecuredIP")){
		ezplib_get_attr_val("http_rule", 0, "secipaddr", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);	
		if(!strlen(TempBuf))
		strcpy(reValue, "0.0.0.0"); 
		else
			strcpy(reValue, TempBuf); 	
		value = reValue;
	}		
	else if (!strcmp(field, "websHostFilters")){
		char *nv_rule_num = "wf_rule_num";
		int r_num;
		char strbuf[2048];
		int j=0;
		r_num = atoi(nvram_safe_get(nv_rule_num));
		printf("\n");
		for(i=0; i< r_num;i++){
			ezplib_get_attr_val("wf_rule", i, "keyword", buf, TMP_LEN, EZPLIB_USE_CLI);
			if(i!=0){			
				sprintf(buf2,";%s",buf);
        		strncpy(strbuf+j,buf2,strlen(buf2));
        		j=j+strlen(buf2);
        	}else{
        		//printf("buf=%s strlen(buf)=%d\n", buf, strlen(buf));
        		strncpy(strbuf+j,buf,strlen(buf));
        		j=strlen(buf);
        	}
		}
		printf("strbuf=%s\n", strbuf);
		value = strbuf;
	}
    //---Steve Start 
    else if (!strcmp(field, "DomainName")){
		ezplib_get_attr_val("lan_dhcps_rule", 0, "domain", buf, TMP_LEN, EZPLIB_USE_CLI);	
		strcpy(reValue, buf); 	
		value = reValue;	
	}		
	else if (!strcmp(field, "AdminInactivityTimer")){
		ezplib_get_attr_val("http_rule", 0, "adm_timeout", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);	
		strcpy(reValue, TempBuf); 	
		value = reValue;	
	}
	else if (!strcmp(field, "igmpEnabled")){
		pTempBuf = nvram_safe_get("igmp_proxy_rule");
		strcpy(reValue, pTempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WANPingFilter")){
		ezplib_get_attr_val("fw_rule", 0, "icmp_ping_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "fl_rule_num")){ 
		value = nvram_safe_get("fl_rule_num");
	}
    //---Steve End
        //--------------------------------------------- aron patch for giga
	else if (!strcmp(field, "wanConnectionMode")) //wan connection mode
	{ 
		value = nvram_get("wan0_proto");
		if (!strcmp(value, "dhcp"))
		    value =  "DHCP";
		else if(!strcmp(value, "static"))
		    value =  "STATIC";
		else if(!strcmp(value, "pppoe"))
		    value =  "PPPOE";
		else if(!strcmp(value, "pptp"))
		    value =  "PPTP";
		else if(!strcmp(value, "l2tp"))
		    value =  "L2TP";
		else if(!strcmp(value, "wwan"))
		    value =  "WWAN";
	}
	else if (!strcmp(field, "wan_pptp_user")) //wan0 pptp username
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_pass")) //wan0 pptp password
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_optime")) //wan0 pptp idletime
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_server")) //wan0 pptp server
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "serverip", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_ip")) //wan0 pptp physical IP Addr
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_netmask")) //wan0 pptp physical IP netmask
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "netmask", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        idx = atoi(reValue);
	        value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "wan_pptp_gateway")) //wan0 pptp physical IP gateway
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_wanip_addr")) //wan0 pptp PPP IP Addr
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_user")) //wan0 l2tp username
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_pass")) //wan0 l2tp password
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_server")) //wan0 l2tp server
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "serverip", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_ip")) //wan0 l2tp physical IP Addr
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_netmask")) //wan0 l2tp physical IP netmask
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "netmask", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        idx = atoi(reValue);
	        value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "wan_l2tp_gateway")) //wan0 l2tp physical IP gateway
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_wanip_addr")) //wan0 l2tp PPP IP Addr
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_ipaddr")) //wan0 static ip address
	{
		ezplib_get_attr_val("wan_static_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_netmask")) //wan0 static netmask
	{
		    ezplib_get_attr_val("wan_static_rule", 0, "mask", buf, TMP_LEN, EZPLIB_USE_CLI); 
	            strcpy(reValue, buf);
	            idx = atoi(reValue);
	            value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "wan_gateway")) //wan0 static gateway
	{
		ezplib_get_attr_val("wan_static_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_primary_dns")) //wan0 (static) primary dns
	{
		ezplib_get_attr_val("wan_static_rule", 0, "dns1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	}
        else if (!strcmp(field, "wan_secondary_dns")) //wan0 (static) secondary dns
	{
		ret = ezplib_get_attr_val("wan_static_rule", 0, "dns2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;   
	}
	else if (!strcmp(field, "wan_pppoe_user")) //wan0 pppoe username
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_pppoe_pass")) //wan0 pppoe password
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_pppoe_mtu")) //wan0 pppoe mtu size
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "mtu", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_pppoe_srvname")) //wan0 pppoe service name
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "servname", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_pppoe_optime")) //wan0 pppoe optime
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "idletime", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pppoe_wanip_addr")) //wan0 pppoe PPP IP Addr
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_dns1_type")) //wan0 primary dns type
	{
		ezplib_get_attr_val("wan_dns_rule", 0, "dnstype1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}
	else if (!strcmp(field, "wan_dns2_type")) //wan0 primary dns type
	{
		ezplib_get_attr_val("wan_dns_rule", 0, "dnstype2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}			
	else if (!strcmp(field, "macCloneMac")) //wan0 clone mac address
	{
		ezplib_get_attr_val("wan_hwaddr_clone_rule", 0, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "spoofIpCloneMac")) //wan0 clone ip address
	{
		ezplib_get_attr_val("wan_hwaddr_clone_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "apn_3g")) //wan0 3g apn
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "apn", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "pin_3g")) //wan0 3g pin
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "pin", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "username_3g")) //wan0 3g username
	{
		ret = ezplib_get_attr_val("wan_wwan_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "password_3g")) //wan0 3g password
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dial_3g")) //wan0 3g dial number
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "dialstr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "max_idle_time")) //wan0 3g ppp idle time
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "idletime", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "ppp_echo_interval")) //wan0 3g ppp echo interval
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "redialperiod", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "ppp_retry_threshold")) //wan0 3g ppp retry threshold
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "echo_failure", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "mtu_3g")) //wan0 3g mtu
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "mtu", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dhcpStart")) //lan0 dhcp start ip
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "start", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "lan_dhcp_pool_size")) //lan0 dhcp pool size
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "num", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dhcp_dns1_type")) //lan0 dhcp first dns type
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "dnstype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "dnsrelay"))
		     value =  "RELAY";
		else if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}
	else if (!strcmp(field, "dhcpPriDns")) //lan0 dhcp primary dns
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "dnsaddr1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dhcp_dns2_type")) //lan0 dhcp secondary dns type
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "dnstype2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "dnsrelay"))
		     value =  "RELAY";
		else if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}
	else if (!strcmp(field, "dhcpSecDns")) //lan0 dhcp secondary dns
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "dnsaddr2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	
	else if (!strcmp(field, "DDNSProvider")) //wan0 ddns provider
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "type", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "dyndns"))
		     value =  "dyndns.org";
		else if (!strcmp(buf, "noip"))
		     value =  "no-ip.com";
		else if (!strcmp(buf, "eurodns"))
		     value =  "eurodyndns";
		else if (!strcmp(buf, "regfish"))
		     value =  "regfish";
	}
	else if (!strcmp(field, "DDNS")) //wan0 ddns Hostname
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "hostname", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "DDNSAccount")) //wan0 ddns user name
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "DDNSPassword")) //wan0 ddns password
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "TIME_servtype")) //ntp time setting type
	{
		ezplib_get_attr_val("ntp_rule", 0, "custom_time", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        //value = reValue;
	        idx = atoi(reValue);
	        if(idx==0)      
	            value="1";
	        else if(idx==1) //AXIM treat "manual time" as "1"
	            value="0";
	}
	else if (!strcmp(field, "current_hour")) //ntp manual hour
	{
		ezplib_get_attr_val("ntp_rule", 0, "hour", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Min")) //ntp manual minute
	{
		ezplib_get_attr_val("ntp_rule", 0, "min", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Sec")) //ntp manual second
	{
		ezplib_get_attr_val("ntp_rule", 0, "sec", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Year")) //ntp manual year
	{
		ezplib_get_attr_val("ntp_rule", 0, "year", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Mon")) //ntp manual month
	{
		ezplib_get_attr_val("ntp_rule", 0, "mon", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Day")) //ntp manual day
	{
		ezplib_get_attr_val("ntp_rule", 0, "date", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "enabledaylight")) //ntp daylight saving enable
	{
		ezplib_get_attr_val("ntp_rule", 0, "daylight_saving", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "startMon")) //ntp daylight saving start month
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_start_mon", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "startDay")) //ntp daylight saving start day
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_start_day", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "startclock")) //ntp daylight saving start clock
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_start_hour", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "endMon")) //ntp daylight saving end month
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_end_mon", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "endDay")) //ntp daylight saving end day
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_end_day", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "endclock")) //ntp daylight saving end clock
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_end_hour", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "NTP_servtype")) //ntp server type
	{
		/*
		ezplib_get_attr_val("ntp_rule", 0, "type", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if (!strcmp(buf, "pool"))
		     value =  "0";
		else if (!strcmp(buf, "ipaddr"))
		     value =  "1";
		*/
		ezplib_get_attr_val("ntp_rule", 0, "custom_server", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "NTPServerIP")) //ntp server ip
	{
		//ezplib_get_attr_val("ntp_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("ntp_rule", 0, "serv_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;        
	}
	else if (!strcmp(field, "TZ")) //ntp time zone
	{
		ezplib_get_attr_val("ntp_rule", 0, "zone", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dhcpStaticMac1")) //dhcp static lease MAC 1
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 0, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp1")) //dhcp static lease IP 1
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac2")) //dhcp static lease MAC 2
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 1, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp2")) //dhcp static lease IP 2
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 1, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac3")) //dhcp static lease MAC 3
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 2, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp3")) //dhcp static lease IP 3
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 2, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac4")) //dhcp static lease MAC 4
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 3, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp4")) //dhcp static lease IP 4
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 3, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac5")) //dhcp static lease MAC 5
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 4, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp5")) //dhcp static lease IP 5
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 4, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac6")) //dhcp static lease MAC 6
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 5, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp6")) //dhcp static lease IP 6
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 5, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac7")) //dhcp static lease MAC 7
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 6, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp7")) //dhcp static lease IP 7
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 6, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac8")) //dhcp static lease MAC 8
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 7, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp8")) //dhcp static lease IP 8
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 7, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "RIPversion")) //RIP
	{
		ezplib_get_attr_val("rt_rip_rule", 0, "rip", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "none"))
		     value =  "0";
		else if (!strcmp(buf, "v1"))
		     value =  "1";
		else if (!strcmp(buf, "v2"))
		     value =  "2";
	}
	else if (!strcmp(field, "igmpEnabled")) //igmp proxy enable
	{
		ezplib_get_attr_val("igmp_proxy_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "lan2_ipaddr")) //lan ip alias IP Addr
	{
		ezplib_get_attr_val("lan_static_rule", 0, "alias_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "lan2_netmask")) //lan ip alias netmask
	{
		ezplib_get_attr_val("lan_static_rule", 0, "alias_netmask", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		idx = atoi(reValue);
		value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "bigPond_username")) //wan bigpond username
	{
		ezplib_get_attr_val("wan_bigpond_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "bigPond_pass")) //wan bigpond password
	{
		ezplib_get_attr_val("wan_bigpond_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	//-------------------------- Aron patch end
// WDS Start +++
	else if (!strcmp(field, "WdsList")){
		memset(reValue,0,sizeof(reValue));
		for (i = 0; i < 4; i++){		
			ezplib_get_attr_val("wl0_wds_basic_rule", i, "hwaddr", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "")){
				break;
			}else{
				strcat(reValue, TempBuf);
				strcat(reValue,";");
			}	
    	}
		value = reValue;
	}
	else if (!strcmp(field, "WdsEncrypType")){
		ezplib_get_attr_val("wl0_wds_basic_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "NONE");
		}else if (!strcmp(TempBuf, "wep")){
			strcpy(reValue, "WEP");
		}else if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl0_wds_sec_wpa_rule", 0, "crypto", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}
		}
		value = reValue;
	}
	else if (!strcmp(field, "Wds0Key")){
			ezplib_get_attr_val("wl0_wds_sec_wep_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
// WDS End ---	
// LogSetting Start +++
	else if (!strcmp(field, "log_SystemMaintenance")){
			ezplib_get_attr_val("log_rule", 0, "web_mgmt", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_dns")){
			ezplib_get_attr_val("log_rule", 0, "dns", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_PPP")){
			ezplib_get_attr_val("log_rule", 0, "ppp", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}	
	else if (!strcmp(field, "log_upnp")){
			ezplib_get_attr_val("log_rule", 0, "upnp", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_wlan")){
			ezplib_get_attr_val("log_rule", 0, "wireless", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_ntpclient")){
			ezplib_get_attr_val("log_rule", 0, "ntp", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_syswarning")){
			ezplib_get_attr_val("log_rule", 0, "sys_warning", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}		
	else if (!strcmp(field, "log_dhcpServer")){
			ezplib_get_attr_val("log_rule", 0, "dhcp_serv", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_dhcpClient")){
			ezplib_get_attr_val("log_rule", 0, "dhcp_cli", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_ddns")){
			ezplib_get_attr_val("log_rule", 0, "ddns", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_Firewall")){
			ezplib_get_attr_val("log_rule", 0, "firewall", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_index")){
			pTempBuf = nvram_safe_get("log_index");
			strcpy(reValue, pTempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_selected_num")){
			pTempBuf = nvram_safe_get("log_selected_num");
			strcpy(reValue, pTempBuf);
		value = reValue;
	}
// LogSetting End ---
// Set Operation Mode Start +++
	else if (!strcmp(field, "lan_ipaddr_router")){
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_normal", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "lan_ipaddr_ap")){
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_ap", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
// Set Operation Mode End ---
// APCli Start +++
	else if (!strcmp(field, "ApCliSsid")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliBssid")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliAuthMode")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "OPEN");
		}else if (!strcmp(TempBuf, "wep")){
			ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "encmode", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "open")){
				strcpy(reValue, "OPEN");
			}else if (!strcmp(TempBuf, "shared")){
				strcpy(reValue, "SHARED");
			}	
		}else if (!strcmp(TempBuf, "psk")){
			strcpy(reValue, "WPAPSK");
		}else if (!strcmp(TempBuf, "psk2")){
			strcpy(reValue, "WPA2PSK");
		}
		value = reValue;
	}
	else if (!strcmp(field, "ApCliEncrypType")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "NONE");
		}else if (!strcmp(TempBuf, "wep")){
			strcpy(reValue, "WEP");
		}else if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}else{
				strcpy(reValue, TempBuf);
			}
		}else if (!strcmp(TempBuf, "psk2")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}else{
				strcpy(reValue, TempBuf);
			}
		}
		value = reValue;
	}
	else if (!strcmp(field, "ApCliWPAPSK")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
		}else if (!strcmp(TempBuf, "psk2")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
		}else{
			strcpy(buf, "");
		}		
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliDefaultKeyId")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key_index", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey1Type")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey2Type")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey3Type")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey4Type")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey1Str")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key1", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey2Str")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key2", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey3Str")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key3", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey4Str")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key4", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
// APCli End ---
	//------------------------------------------------------------
	//----------------------------------------------------------IPv6
	else if(!strcmp(field, "lan_ipv6addr_static")) //IPv6 lan address for static
	{
		//ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "static_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); //Steve change 2011/03/09
		//ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dhcp_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); //aron change to "dhcp_prefix" 2010.11.19
		ezplib_get_attr_val("lan_staticv6_rule", 0, "static_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); //Steve change 2011/03/10
		strcpy(reValue, buf);
	        value = reValue;
printf("lan static value=====%s\n",value);
	}
	else if(!strcmp(field, "lan_ipv6addr_pppoe")) //IPv6 lan address for pppoe
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "pppoe_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan pppoe value=====%s\n",value);
	}
	else if(!strcmp(field, "lan_ipv6addr_dhcp")) //IPv6 lan address  for dhcp
	{
		//ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dhcp_prefix", buf, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_staticv6_rule", 0, "dhcp_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan dhcp value=====%s\n",value);
	}
/*
	else if(!strcmp(field, "lan_ipv6netsize")) //IPv6 lan net size
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "mask", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
*/	
	else if(!strcmp(field, "lan_ipv6gateway")) //IPv6 lan gateway
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "lan_ipv6dns")) //IPv6 lan dns
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "dns", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6addr")) //IPv6 Wan address
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6netsize")) //IPv6 Wan size
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "length", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6gateway")) //IPv6 Wan gateway
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6dns1")) //IPv6 Wan primary dns
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "dns1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6dns2")) //IPv6 Wan secondary dns
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "dns2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "Ipv6_dhcp_lifetime")) //IPv6 DHCP lifetime
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "lifetime", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "dhcp_ipv6dns1")) //IPv6 Dhcp primary dns
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dnsaddr1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "dhcp_ipv6dns2")) //IPv6 Dhcp secondary dns
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dnsaddr2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "ppoe_ipv6user")) //IPv6 PPPoE user name
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "pppoe_ipv6service")) //IPv6 PPPoE service name
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "servicename", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}	
	else if(!strcmp(field, "pppoe_ipv6idle")) //IPv6 PPPoE idle time
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "idletime", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "pppoe_ipv6mtu")) //IPv6 PPPoE mtu
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "mtu", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "pppoe_ipv6addr")) //IPv6 PPPoE custom address
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "pppoe_ipv6pass")) //IPv6 pppoe password
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "ipv6_dns1_type")) //pppoe primary dns type
	{
		ezplib_get_attr_val("wan_dnsv6_rule", 0, "dnstype1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}
	else if (!strcmp(field, "ipv6_dns2_type")) //ppp0e secondary dns type
	{
		ezplib_get_attr_val("wan_dnsv6_rule", 0, "dnstype2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}
	else if (!strcmp(field, "IPv6State")) //IPv6 dhcp state
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "type", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	        
	}

	else if (!strcmp(field, "Ipv6_dhcp_addrprefix0")) //IPv6 dhcp address prefix
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "static_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	        
printf("lan static prefix value=====%s\n",value);
	}
	else if (!strcmp(field, "Ipv6_dhcp_addrprefix1")) //IPv6 dhcp address prefix
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "pppoe_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan pppoe prefix value=====%s\n",value);	       	        
	}
	else if (!strcmp(field, "Ipv6_dhcp_addrprefix2")) //IPv6 dhcp address prefix
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dhcp_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan dhcp prefix value=====%s\n",value);	        	        
	}
	else if (!strcmp(field, "Ipv6_dhcp_addrstart")) //IPv6 dhcp address start
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "start", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan end value=====%s\n",value);	        	        
	}	
	else if (!strcmp(field, "Ipv6_dhcp_addrend")) //IPv6 dhcp address end
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "end", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan  start value=====%s\n",value);	        	        
	}
//Steve new add 2011 	
	else if (!strcmp(field, "Ipv6_wan_dhcp_type")) //IPv6 wan dhcp type
	{
		ezplib_get_attr_val("wan_dhcpv6_rule", 0, "type", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "stateful"))
		     value =  "1";
		else if (!strcmp(buf, "stateless"))
		     value =  "0";
		else 
		     value =  "0";
printf("Ipv6 wan dhcp type=====%s\n",value);	        	        
	}
	else if(!strcmp(field, "lan_ipv6addr_dhcp_suf")) //IPv6 lan address suffix for dhcp
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "dhcp_suffix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan dhcp value=====%s\n",value);
	}
	else if(!strcmp(field, "lan_ipv6addr_static_suf")) //IPv6 lan address suffix for static
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "static_suffix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan dhcp value=====%s\n",value);
	}
//+++ WLAN Guest function (Begin)
	else if (!strcmp(field, "WlanGuestIP"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestMASK"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "netmask", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		idx = atoi(reValue);
		value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "WlanGuestBWMax"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "bw", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
//--- WLAN Guest function (End)
	else{
		printf(" No get Cfg action !!!! \n");	
	}
	
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), value);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 *            idx - index of nth
 * description: read general configurations from nvram (if value is NULL -> "")
 *              parse it and return the Nth value delimited by semicolon
 */
static int getCfgNthGeneral(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RT2860_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		nth = getNthValue(idx, value);
		if (NULL == nth)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), nth);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	nth = getNthValue(idx, value);
	if (NULL == nth)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> "0"
 */
static int getCfgZero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;
	char TempBuf[32];
	char buf[TMP_LEN]; //aron add
	int ret; //aron add
	int idx,i; //aron add

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}

// Tommy for Debug	
printf("\n ---> getCfgZero(): type = %d & field = %s",type,field);
	if (!strcmp(field, "OP_Mode")){
		/* Get the setting of value from AXIMCom's nvram structure into reValue */
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "normal")){
			strcpy(reValue, "0");
		}else if (!strcmp(TempBuf, "ap")){
			strcpy(reValue, "1");
		}else if (!strcmp(TempBuf, "ur")){
			strcpy(reValue, "5");
		}else if (!strcmp(TempBuf, "wisp")){
			strcpy(reValue, "6");
		}else{
			printf("Error: Cannot get the mode value !!!\n");
			strcpy(reValue, "0"); // default normal mode
		}
		value = reValue;
	}else if (!strcmp(field, "AAA")){ 
	}
	else if (!strcmp(field, "Channel")){
//		value = nvram_safe_get("AP_Channel");
//printf("AP channel ======%s******\n", value);
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", TempBuf, 32, EZPLIB_USE_CLI);		
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}else if (!strcmp(field, "WirelessMode")){
		ezplib_get_attr_val("wl_basic_rule", 0, "net_mode", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}else if (!strcmp(field, "natEnabled")){ 
		value = nvram_safe_get("nat_rule");
	}else if (!strcmp(field, "FIREWALLEnabled")){ 
		ezplib_get_attr_val("fw_rule", 0, "state_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}else if (!strcmp(field, "RL_QoSEnable")){ 
		ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);	    
		strcpy(reValue, TempBuf);
		value = reValue;
	}else if (!strcmp(field, "RemoteManagementSecuredSel")){ 
		ezplib_get_attr_val("http_rule", 0, "secipaddr", TempBuf, 32, EZPLIB_USE_CLI);
		if(!strlen(TempBuf))
			strcpy(reValue, "0");
		else
			strcpy(reValue, "1");
		value = reValue;
	}
	//-----Steve Start
	else if (!strcmp(field, "websFilterActivex")){
		ezplib_get_attr_val("wf_content_rule", 0, "activex_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}
	else if (!strcmp(field, "websFilterJava")){
		ezplib_get_attr_val("wf_content_rule", 0, "java_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}
	else if (!strcmp(field, "websFilterProxy")){
		ezplib_get_attr_val("wf_content_rule", 0, "proxy_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}
	else if (!strcmp(field, "websFilterCookies")){
		ezplib_get_attr_val("wf_content_rule", 0, "cookie_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "websHostFiltersEnabled")){
		ezplib_get_attr_val("wf_content_rule", 0, "url_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "RemoteManagementSel")){
		ezplib_get_attr_val("http_rule", 0, "rmgt_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "AutoBridge")){
		ezplib_get_attr_val("wl_mode_rule", 0, "auto_bridge", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "IPPortFilterEnable")){
		value = nvram_safe_get("fl_enable");
	}
	//-----Steve End
	//-----Wireless Start
	else if (!strcmp(field, "easy_wireless_security")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staProfile")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staAuth")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staKey1")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staKey2")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staKey3")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staKey4")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "IEEE8021X")){
		strcpy(reValue, "0");
		value = reValue;
	}
// EZ mode Wireless Security Start +++	
	else if (!strcmp(field, "AuthMode")){
			ezplib_get_attr_val("wl0_sec_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "disabled")){
		strcpy(reValue, "OPEN");
			}else if (!strcmp(TempBuf, "wep")){
				strcpy(reValue, "SHARED");
			}else if (!strcmp(TempBuf, "psk")){
				strcpy(reValue, "WPAPSK");
			}else if (!strcmp(TempBuf, "psk2")){
				strcpy(reValue, "WPA2PSK");
			}else if (!strcmp(TempBuf, "wpa")){
				strcpy(reValue, "WPA");
			}else if (!strcmp(TempBuf, "wpa2")){
				strcpy(reValue, "WPA2");
			}
		value = reValue;
	}
	else if (!strcmp(field, "EncrypType")){
			ezplib_get_attr_val("wl0_sec_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "disabled")){
		strcpy(reValue, "NONE");
			}else if (!strcmp(TempBuf, "wep")){
				strcpy(reValue, "WEP");
			}else if (!strcmp(TempBuf, "psk")){
				ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "psk2")){
				ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "wpa")){
				ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "wpa2")){
				ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}
			value = reValue;
	}
	else if (!strcmp(field, "WPAPSK1")){
			ezplib_get_attr_val("wl0_sec_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "disabled")){
				strcpy(reValue, "");
			}else if (!strcmp(TempBuf, "wep")){
				ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key_index", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "1")){
					ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key1", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				}else if (!strcmp(TempBuf, "2")){
					ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key2", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				}else if (!strcmp(TempBuf, "3")){
					ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key3", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				}else if (!strcmp(TempBuf, "4")){
					ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key4", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				}
				strcpy(reValue, TempBuf);
			}else if (!strcmp(TempBuf, "psk")){
				ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
					strcpy(reValue, buf);
			}else if (!strcmp(TempBuf, "psk2")){
				ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
					strcpy(reValue, buf);
			}else if (!strcmp(TempBuf, "wpa")){
					ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
					strcpy(reValue, buf);
			}else if (!strcmp(TempBuf, "wpa2")){
				ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
					strcpy(reValue, buf);
			}
		value = reValue;
	}
// EZ mode Wireless Security End ---
// WDS Start ++++++
	else if (!strcmp(field, "WdsEnable")){
    	ezplib_get_attr_val("wl_wds_rule", 0, "mode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
        if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "0");
    	}else if (!strcmp(TempBuf, "repeater")){
			strcpy(reValue, "1");
    	}else if (!strcmp(TempBuf, "bridge")){
			strcpy(reValue, "2");
    	}else{
			printf(" Error : No arg !!!! \n");	
    	}
		value = reValue;
	}
	else if (!strcmp(field, "WdsPhyMode")){
		ezplib_get_attr_val("wl_advanced_rule", 0, "wdstxmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
#if 1
		if (!strcmp(TempBuf, "")){
			strcpy(reValue, "CCK");
		}else{	
			strcpy(reValue, TempBuf);
		}
		value = reValue;
#else
		if (!strcmp(TempBuf, "CCK")){
			strcpy(reValue, "CCK;CCK;CCK;CCK");
    	}else if (!strcmp(TempBuf, "OFDM")){
			strcpy(reValue, "OFDM;OFDM;OFDM;OFDM");
    	}else if (!strcmp(TempBuf, "HTMIX")){
			strcpy(reValue, "HTMIX;HTMIX;HTMIX;HTMIX");
    	}else if (!strcmp(TempBuf, "GREENFIELD")){
			strcpy(reValue, "GREENFIELD;GREENFIELD;GREENFIELD;GREENFIELD");
    	}
		value = reValue;
#endif		
	}
// WDS End   ------
// Scheduling & Power Saving Start +++
	else if (!strcmp(field, "WiFiScheduleEnable")){
		value = nvram_safe_get("sched_enable");
	}
	/* Everyday */
	else if (!strcmp(field, "WiFischeduling_action_0")){
		ezplib_get_attr_val("sched_rule", 0, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_0")){
		ezplib_get_attr_val("sched_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_0")){
		ezplib_get_attr_val("sched_rule", 0, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_0")){
		ezplib_get_attr_val("sched_rule", 0, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_0")){
		ezplib_get_attr_val("sched_rule", 0, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_0")){
		ezplib_get_attr_val("sched_rule", 0, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Sunday */
	else if (!strcmp(field, "WiFischeduling_action_7")){
		ezplib_get_attr_val("sched_rule", 1, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_7")){
		ezplib_get_attr_val("sched_rule", 1, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_7")){
		ezplib_get_attr_val("sched_rule", 1, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_7")){
		ezplib_get_attr_val("sched_rule", 1, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_7")){
		ezplib_get_attr_val("sched_rule", 1, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_7")){
		ezplib_get_attr_val("sched_rule", 1, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Monday */
	else if (!strcmp(field, "WiFischeduling_action_1")){
		ezplib_get_attr_val("sched_rule", 2, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_1")){
		ezplib_get_attr_val("sched_rule", 2, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_1")){
		ezplib_get_attr_val("sched_rule", 2, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_1")){
		ezplib_get_attr_val("sched_rule", 2, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_1")){
		ezplib_get_attr_val("sched_rule", 2, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_1")){
		ezplib_get_attr_val("sched_rule", 2, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Tuesday */
	else if (!strcmp(field, "WiFischeduling_action_2")){
		ezplib_get_attr_val("sched_rule", 3, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_2")){
		ezplib_get_attr_val("sched_rule", 3, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_2")){
		ezplib_get_attr_val("sched_rule", 3, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_2")){
		ezplib_get_attr_val("sched_rule", 3, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_2")){
		ezplib_get_attr_val("sched_rule", 3, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_2")){
		ezplib_get_attr_val("sched_rule", 3, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Wednesday */
	else if (!strcmp(field, "WiFischeduling_action_3")){
		ezplib_get_attr_val("sched_rule", 4, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_3")){
		ezplib_get_attr_val("sched_rule", 4, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_3")){
		ezplib_get_attr_val("sched_rule", 4, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_3")){
		ezplib_get_attr_val("sched_rule", 4, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_3")){
		ezplib_get_attr_val("sched_rule", 4, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_3")){
		ezplib_get_attr_val("sched_rule", 4, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Thursday */
	else if (!strcmp(field, "WiFischeduling_action_4")){
		ezplib_get_attr_val("sched_rule", 5, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_4")){
		ezplib_get_attr_val("sched_rule", 5, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_4")){
		ezplib_get_attr_val("sched_rule", 5, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_4")){
		ezplib_get_attr_val("sched_rule", 5, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_4")){
		ezplib_get_attr_val("sched_rule", 5, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_4")){
		ezplib_get_attr_val("sched_rule", 5, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}	
	/* Friday */
	else if (!strcmp(field, "WiFischeduling_action_5")){
		ezplib_get_attr_val("sched_rule", 6, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_5")){
		ezplib_get_attr_val("sched_rule", 6, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_5")){
		ezplib_get_attr_val("sched_rule", 6, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_5")){
		ezplib_get_attr_val("sched_rule", 6, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_5")){
		ezplib_get_attr_val("sched_rule", 6, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_5")){
		ezplib_get_attr_val("sched_rule", 6, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Saturday */
	else if (!strcmp(field, "WiFischeduling_action_6")){
		ezplib_get_attr_val("sched_rule", 7, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_6")){
		ezplib_get_attr_val("sched_rule", 7, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_6")){
		ezplib_get_attr_val("sched_rule", 7, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_6")){
		ezplib_get_attr_val("sched_rule", 7, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_6")){
		ezplib_get_attr_val("sched_rule", 7, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_6")){
		ezplib_get_attr_val("sched_rule", 7, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}		
// Scheduling & Power Saving End ---
	//-----Wireless End
        //------------------ aron patch for giga
        else if (!strcmp(field, "wan_ip_auto")){ //Ethernet is DHCP or Static IP
		value = nvram_get("wan0_proto");
		//Translate AXIM's wan proto to AboCom's UI format 
		if (!strcmp(value, "dhcp"))
		    value =  "0";
		else if(!strcmp(value, "static"))
		    value =  "1";
	}
	else if (!strcmp(field, "wan_pppoe_nailup")) //wan0 pppoe nailup
	{
                   ezplib_get_attr_val("wan_pppoe_rule", 0, "demand", buf, TMP_LEN, EZPLIB_USE_CLI); 
	           strcpy(reValue, buf);
	           idx = atoi(reValue);
	           if(idx==0)      //AXIM treat nailup "checked" as "0"
	               value="1";
	           else if(idx==1) //on-demand
	               value="0";
	}
	else if (!strcmp(field, "wan_pppoe_wanip_mode")) //wan0 pppoe PPP IP type
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "pppiptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "isp"))
		     value =  "1";
		else if (!strcmp(buf, "custom"))
		     value =  "0";
	}
	else if (!strcmp(field, "wan_MacClone_mode")) //wan0 clone mac type
	{
		   ezplib_get_attr_val("wan_hwaddr_clone_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
	           strcpy(reValue, buf);
	           idx = atoi(reValue);
	           if(idx==0)      //AXIM treat factory default as "0"
	               value="0";
	           else if(idx==1)
	           {
	               ezplib_get_attr_val("wan_hwaddr_clone_rule", 0, "addrtype", buf, TMP_LEN, EZPLIB_USE_CLI); 
	               if (!strcmp(buf, "ipaddr")) //clone mac by IP
		            value =  "1";
		       else if (!strcmp(buf, "hwaddr")) //clone mac by MAC
		            value =  "2";
	           }
	}
	else if (!strcmp(field, "brand_3g")) //wan0 3g brand
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "brand", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "model_3g")) //wan0 3g model
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "model", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "apnType_3g")) //wan0 3g apn type
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "apn_type", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "location_3g")) //wan0 3g location
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "location", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "srvPro_3g")) //wan0 3g location
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "isp", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "auth_3g")) //wan0 3g authentication
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "auth", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "connMode_3g")) //wan0 3g connection mode
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "ppp_conn_type")) //wan0 3g ppp connection type
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "demand", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "turbo_3g")) //wan0 3g turbo link
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "turbolink", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_3g_nailup")) //wan0 3g nailup
	{
                   ezplib_get_attr_val("wan_wwan_rule", 0, "demand", buf, TMP_LEN, EZPLIB_USE_CLI); 
	           strcpy(reValue, buf);
	           idx = atoi(reValue);
	           if(idx==0)      //AXIM treat nailup "checked" as "0"
	               value="1";
	           else if(idx==1) //on-demand
	               value="0";
	}
	else if (!strcmp(field, "dhcpEnabled")) //lan0 dhcp enable
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "DDNSEnabled")) //wan0 ddns enable
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "upnpEnabled")) //upnp enable
	{
		ezplib_get_attr_val("upnp_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingEnabled")) //igmp snooping enable
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingLAN1")) //igmp snooping port1
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "p1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingLAN2")) //igmp snooping port2
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "p2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingLAN3")) //igmp snooping port3
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "p3", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingLAN4")) //igmp snooping port4
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "p4", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "OperationMode")) //OperationMode
	{
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "normal"))
		     value =  "0";
		else if (!strcmp(buf, "ap"))
		     value =  "1";
		else if (!strcmp(buf, "ur"))
		     value =  "5";
		else if (!strcmp(buf, "wisp"))     
		     value =  "6";
	}
	else if (!strcmp(field, "wan_pptp_nailup")) //wan0 pptp nailup
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "nailup", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "0"))
		     value =  "0";
		else if (!strcmp(buf, "1"))
		     value =  "1";
	}
	else if (!strcmp(field, "wan_pptp_mode")) //wan0 pptp physical IP type
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "iptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "dhcp"))
		     value =  "1";
		else if (!strcmp(buf, "static"))
		     value =  "0";
	}
	else if (!strcmp(field, "wan_pptp_wanip_mode")) //wan0 pptp PPP IP type
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "pppiptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "isp"))
		     value =  "1";
		else if (!strcmp(buf, "custom"))
		     value =  "0";
	}
	else if (!strcmp(field, "wan_l2tp_mode")) //wan0 l2tp physical IP type
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "iptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "dhcp"))
		     value =  "1";
		else if (!strcmp(buf, "static"))
		     value =  "0";
	}
	else if (!strcmp(field, "wan_l2tp_wanip_mode")) //wan0 l2tp PPP IP type
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "pppiptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "isp"))
		     value =  "1";
		else if (!strcmp(buf, "custom"))
		     value =  "0";
	}
	else if (!strcmp(field, "Lan2Enabled")) //lan ip alias enable
	{
		ezplib_get_attr_val("lan_static_rule", 0, "alias_enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "lanIp_mode")) //lan0 ip mode
	{
		value = nvram_get("lan0_proto");
		if (!strcmp(value, "dhcp"))
		    value =  "0";
		else if(!strcmp(value, "static"))
		    value =  "1";
	}
        else if (!strcmp(field, "bigPond_srv")) //wan0 bigpond server
	{
		ezplib_get_attr_val("wan_bigpond_rule", 0, "server", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "bigPond_enb")) //wan0 bigpond enable
	{
		ezplib_get_attr_val("wan_bigpond_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	//------------------------ Aron patch end
	else if(!strcmp(field, "EnableSSID")) //enable ssid
	{
		ezplib_get_attr_val("wl0_basic_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	

	}else if(!strcmp(field, "EnableSSID1")) //enable ssid1
	{
		ezplib_get_attr_val("wl0_basic_rule", 1, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}else if(!strcmp(field, "EnableSSID2")) //enable ssid2
	{
		ezplib_get_attr_val("wl0_basic_rule", 2, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}else if(!strcmp(field, "EnableSSID3")) //enable ssid3
	{
		ezplib_get_attr_val("wl0_basic_rule", 3, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID")) //hidden ssid1
	{
		ezplib_get_attr_val("wl0_basic_rule", 0, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	

	}else if(!strcmp(field, "HideSSID1")) //hidden ssid2
	{
		ezplib_get_attr_val("wl0_basic_rule", 1, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}else if(!strcmp(field, "HideSSID2")) //hidden ssid3
	{
		ezplib_get_attr_val("wl0_basic_rule", 2, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}else if(!strcmp(field, "HideSSID3")) //hidden ssid4
	{
		ezplib_get_attr_val("wl0_basic_rule", 3, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	//------------------------------------------------
	else if(!strcmp(field, "NoForwarding")) //Intra_BSS1
	{
		ezplib_get_attr_val("wl0_basic_rule", 0, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	
	}else if(!strcmp(field, "NoForwarding1")) //Intra_BSS2
	{
		ezplib_get_attr_val("wl0_basic_rule", 1, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
	
	}else if(!strcmp(field, "NoForwarding2")) //Intra_BSS3
	{
		ezplib_get_attr_val("wl0_basic_rule", 2, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	

	}else if(!strcmp(field, "NoForwarding3")) //Intra_BSS4
	{
		ezplib_get_attr_val("wl0_basic_rule", 3, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
		
	}else if(!strcmp(field, "MainIntra_BSS")) //Main Intra_BSS
	{
		ezplib_get_attr_val("wl_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;      

	}

// APCli Start +++
	else if (!strcmp(field, "ApCliEnable")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
// APCli Start ---
//------------------------------------------------------------Wireless Security
	else if(!strcmp(field, "DefaultWEPKey")) //DefaultWEPKey
	{
		ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key_index", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
			
	}//-------------------------------------------------------------Wireless Advanced
	else if(!strcmp(field, "RTSThreshold")) //RTSThreshold
	{
		ezplib_get_attr_val("wl_advanced_rule", 0, "rts", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "FragThreshold")) //Fragmentation
	{
		ezplib_get_attr_val("wl_advanced_rule", 0, "frag", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "TxPower")) //Output power
	{
		ezplib_get_attr_val("wl_basic_rule", 0, "txpower", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "NoForwardingAdv")) //Intra-BSS enable
	{
		ezplib_get_attr_val("wl_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        			
	}else if(!strcmp(field, "HT_OpMode")) //Operating Mode
	{
		ezplib_get_attr_val("wl_advanced_rule", 0, "opmode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "HT_BW")) //Channel Bandwidth
	{
		ezplib_get_attr_val("wl_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "HT_GI")) //Guard Interval
	{
		ezplib_get_attr_val("wl_advanced_rule", 0, "gi", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}//---------------------------------------------------------Wireless QoS
	else if(!strcmp(field, "WmmCapable")) //Guard Interval
	{
		ezplib_get_attr_val("wl0_basic_rule", 0, "wme", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}//----------------------------------------------------------IPV6
	else if(!strcmp(field, "Ipv6DhcpCapable")) //enable Ipv6 dhcp 
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
printf("enable dhcp *********%s\n",value);
	}
	else if(!strcmp(field, "dnsType")) //Ipv6 dhcp dns type
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dnstype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "auto"))
		     value =  "0";
		else if (!strcmp(buf, "custom"))
		     value =  "1";		     			
	}
	else if(!strcmp(field, "PppoeIPv6_iptype")) //Ipv6 dhcp pppoe address type
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "pppiptype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "isp"))
		     value =  "0";
		else if (!strcmp(buf, "custom"))
		     value =  "1";		     			
	}
        else if (!strcmp(field, "wan_ipv6_proto")){ //IPV6 wan proto
		value = nvram_get("wan0_protov6");
		if (!strcmp(value, "static"))
		    value =  "0";
		else if(!strcmp(value, "dhcp"))
		    value =  "1";
		else if(!strcmp(value, "pppoe"))
		    value =  "2";
		else if(!strcmp(value, "link-local")) //aron add  2010.12.01
		    value =  "3";
	}
//+++ WLAN Guest function (Begin)
	else if (!strcmp(field, "WlanGuestEnable"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestBWEnable"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "bwen", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestBWPriority"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "prio", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
//--- WLAN Guest function (End)
	else{
		printf(" No get Cfg action !!!! \n");	
	}
	
	if (1 == type) {
		if (!strcmp(value, "")){
			return websWrite(wp, T("0"));
		}
		return websWrite(wp, T("%s"), value);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}


/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 *            idx - index of nth
 * description: read general configurations from nvram (if value is NULL -> "0")
 *              parse it and return the Nth value delimited by semicolon
 */
static int getCfgNthZero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RT2860_NVRAM, field);
	if (1 == type) {
		if (!strcmp(value, ""))
			return websWrite(wp, T("0"));
		nth = getNthValue(idx, value);
		if (NULL == nth)
			return websWrite(wp, T("0"));
		return websWrite(wp, T("%s"), nth);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	nth = getNthValue(idx, value);
	if (NULL == nth)
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> ""
 */
static int getCfg2General(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RTINIC_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), value);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 *            idx - index of nth
 * description: read general configurations from nvram (if value is NULL -> "")
 *              parse it and return the Nth value delimited by semicolon
 */
static int getCfg2NthGeneral(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RTINIC_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		nth = getNthValue(idx, value);
		if (NULL == nth)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), nth);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	nth = getNthValue(idx, value);
	if (NULL == nth)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> "0"
 */
static int getCfg2Zero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RTINIC_NVRAM, field);
	if (1 == type) {
		if (!strcmp(value, ""))
			return websWrite(wp, T("0"));
		return websWrite(wp, T("%s"), value);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 *            idx - index of nth
 * description: read general configurations from nvram (if value is NULL -> "0")
 *              parse it and return the Nth value delimited by semicolon
 */
static int getCfg2NthZero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RTINIC_NVRAM, field);
	if (1 == type) {
		if (!strcmp(value, ""))
			return websWrite(wp, T("0"));
		nth = getNthValue(idx, value);
		if (NULL == nth)
			return websWrite(wp, T("0"));
		return websWrite(wp, T("%s"), nth);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	nth = getNthValue(idx, value);
	if (NULL == nth)
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> ""
 */
static int getCfg3General(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RT2561_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), value);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> "0"
 */
static int getCfg3Zero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args"));
	}
	//value = nvram_bufget(RT2561_NVRAM, field);
	if (1 == type) {
		if (!strcmp(value, ""))
			return websWrite(wp, T("0"));
		return websWrite(wp, T("%s"), value);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}

static int getDpbSta(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_RT2860V2_STA_DPB
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getLangBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t *lang;

	if (ejArgs(argc, argv, T("%s"), &lang) < 1) {
		return websWrite(wp, T("0"));
	}
	if (!strncmp(lang, "en", 3))
#if 1 //def CONFIG_USER_GOAHEAD_LANG_EN
		return websWrite(wp, T("1"));
#else
		return websWrite(wp, T("0"));
#endif
	else if (!strncmp(lang, "zhtw", 5))
#if 1 //def CONFIG_USER_GOAHEAD_LANG_ZHTW
		return websWrite(wp, T("1"));
#else
		return websWrite(wp, T("0"));
#endif
	else if (!strncmp(lang, "zhcn", 5))
#ifdef CONFIG_USER_GOAHEAD_LANG_ZHCN
		return websWrite(wp, T("0"));
#else
		return websWrite(wp, T("0"));
#endif
#if 1//Arthur Chow 2009-03-21: For Multi-Language
	if (!strncmp(lang, "de", 3))
		return websWrite(wp, T("1"));
	if (!strncmp(lang, "fr", 3))
		return websWrite(wp, T("1"));
	if (!strncmp(lang, "es", 3))
		return websWrite(wp, T("1"));
	if (!strncmp(lang, "it", 3))
		return websWrite(wp, T("1"));
#endif

	return websWrite(wp, T("0"));
}

static int getMiiInicBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined (CONFIG_RT2880v2_INIC_MII) || defined (CONFIG_RT2880v2_INIC_MII_MODULE)
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

/*
 * description: write the current system platform accordingly
 */
static int getPlatform(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_RAETH_ROUTER
	return websWrite(wp, T("RT2880 with IC+ MACPHY"));
#endif
#ifdef CONFIG_ICPLUS_PHY
    return websWrite(wp, T("RT2880 with IC+ PHY"));
#endif
#ifdef CONFIG_RT_MARVELL
	return websWrite(wp, T("RT2880 with MARVELL"));
#endif
#ifdef CONFIG_MAC_TO_MAC_MODE
	return websWrite(wp, T("RT2880 with Vitesse"));
#endif
#ifdef CONFIG_RT_3052_ESW
	return websWrite(wp, T("RT3052 embedded switch"));
#endif

	return 0;
}

static int getStationBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_RT2860V2_STA || defined CONFIG_RT2860V2_STA_MODULE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

/*
 * description: write System build time
 */
static int getSysBuildTime(int eid, webs_t wp, int argc, char_t **argv)
{
	return websWrite(wp, T("%s"), __DATE__);
}

/*
 * description: write RT288x SDK version
 */
#if 0 // Tommy debug
static int getSdkVersion(int eid, webs_t wp, int argc, char_t **argv)
{
	return websWrite(wp, T("%s"), RT288X_SDK_VERSION);
}
#endif // Tommy debug

//Steve 2010/12/01
int getMem()
{
	int gmem=0;
	FILE *fp;
	char cmdBuf[200]={0};

	memset(cmdBuf, 0, sizeof(cmdBuf)); 
	sprintf(cmdBuf, "cat /proc/meminfo | grep MemTotal | cut -d' ' -f 9 > /tmp/sysmem");	
	system(cmdBuf);
	//printf("	%s \n",cmdBuf);
		
	fp = fopen("/tmp/sysmem", "r");
	if (!fp){
		printf("indicate error: Cannot open /tmp/sysmem !!!\n");
		return 16000;
	}else{
		fscanf(fp,"%d",&gmem);
		fclose(fp);
	}
	if(gmem > 0)
		return gmem;
	else
		return 16000;
}

#if 1//Arthur Chow 2009-01-03
/*
 * description: write System Resource
 */
static int getSysResource(int eid, webs_t wp, int argc, char_t **argv)
{
		FILE *fp = NULL;
		int len=0;
		char_t top_info[80];
		char_t mem_info[10];
		char_t cpu_info[10];
		int mem_num=0;

		//system("top -n 1 > /etc_ro/web/sys_resource");
		system("top -n 1 > /tmp/sys_resource");

		//if (NULL == (fp = fopen("/etc_ro/web/sys_resource", "r")))
		if (NULL == (fp = fopen("/tmp/sys_resource", "r")))
		{
			printf("getSysResource: open /tmp/sys_resource error\n");
			error(E_L, E_LOG, T("getSysResource: open /tmp/sys_resource error"));
			strcpy(mem_info, "0");
			strcpy(cpu_info, "0");
		}
		else
		{
			len = fscanf(fp, "%s", top_info);
//			printf("0000[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("1111[%d][%s]\n", len, top_info);
			if (len)
			{
				int str_len=0;
				strcpy(mem_info, top_info);
				str_len=strlen(mem_info);
				mem_info[str_len-1]=0;
			}
			else
				strcpy(mem_info, "0");
			len = fscanf(fp, "%s", top_info);
//			printf("0001[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0002[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0003[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0004[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0005[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0006[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0007[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0008[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0009[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0010[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("2222[%d][%s]\n", len, top_info);
			if (len)
				strcpy(cpu_info, top_info);
			else
				strcpy(cpu_info, "0");
			fclose(fp);
		}

		websWrite(wp, T("<tr >\n"));
		websWrite(wp, T("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSystatCpu'>CPU Usage</font>:</span></td>\n"));
		websWrite(wp, T("<td valign='middle' ><span class='table_font2'>\n"));
		websWrite(wp, T("<table width='100%%' height='18' border='0' cellpadding='0' cellspacing='0' bordercolor='#7895c8'><tr><td width='60%%'>\n"));
		websWrite(wp, T("<table width='100%%' height='18' border='1' cellpadding='0' cellspacing='0' bordercolor='#7895c8'>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("<tr>\n"));
		websWrite(wp, T("<td background='images/i_percentage_center.gif'>\n"));
		websWrite(wp, T("<table width='%s' border='0' cellspacing='0' cellpadding='0'>\n"), cpu_info);
		websWrite(wp, T("<tr>\n"));
		websWrite(wp, T("<td height='18' background='images/i_percentage_blue.gif'><img src='images/air.gif' width='1' height='1' /></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("</table></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("</table>\n"));
		websWrite(wp, T("</td><td>&nbsp;%s</td></tr></table>\n"), cpu_info);
		websWrite(wp, T("</span></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("<tr >\n"));
		websWrite(wp, T("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSystatMem'>Memory Usage</font>:</span></td>\n"));
		websWrite(wp, T("<td valign='middle' ><span class='table_font2'>\n"));
		websWrite(wp, T("<table width='100%%' height='18' border='0' cellpadding='0' cellspacing='0' bordercolor='#7895c8'><tr><td width='60%%'>\n"));
		websWrite(wp, T("<table width='100%%' height='18' border='1' cellpadding='0' cellspacing='0' bordercolor='#7895c8'>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("<tr>\n"));
		websWrite(wp, T("<td background='images/i_percentage_center.gif'>\n"));
#if 1 //def CONFIG_RT2880_DRAM_32M
		mem_num = getMem();
		websWrite(wp, T("<table width='%d%%' border='0' cellspacing='0' cellpadding='0'>\n"), (atoi(mem_info)*100)/mem_num);
		//websWrite(wp, T("<table width='%d%%' border='0' cellspacing='0' cellpadding='0'>\n"), (atoi(mem_info)*100)/29068);
#else
		websWrite(wp, T("<table width='%d%%' border='0' cellspacing='0' cellpadding='0'>\n"), (atoi(mem_info)*100)/62012);
#endif		
		websWrite(wp, T("<tr>\n"));
		websWrite(wp, T("<td height='18' background='images/i_percentage_blue.gif'><img src='images/air.gif' width='1' height='1' /></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("</table></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("</table>\n"));
		
#if 1 //def CONFIG_RT2880_DRAM_32M
        websWrite(wp, T("</td><td>&nbsp;%d%%</td></tr></table>\n"), (atoi(mem_info)*100)/mem_num);
		//websWrite(wp, T("</td><td>&nbsp;%d%%</td></tr></table>\n"), (atoi(mem_info)*100)/29068);
#else
		websWrite(wp, T("</td><td>&nbsp;%d%%</td></tr></table>\n"), (atoi(mem_info)*100)/62012);
#endif		
		websWrite(wp, T("</span></td>\n"));
//		websWrite(wp, T("<tr >\n"));
//		websWrite(wp, T("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- Memory Usage:</span></td>\n"));
//		websWrite(wp, T("<td ><span class='table_font2'>%sB</span></td>\n"), mem_info);
		return websWrite(wp, T("</tr>\n"));
}
#endif

/*marked by Gordon 2009/1/5
 * description: write System Uptime
 */
/*static int getSysUptime(int eid, webs_t wp, int argc, char_t **argv)
{
	struct tm *utime;
	time_t usecs;

	time(&usecs);
	utime = localtime(&usecs);

	if (utime->tm_hour > 0)
		return websWrite(wp, T("%d hour%s, %d min%s, %d sec%s"),
				utime->tm_hour, (utime->tm_hour == 1)? "" : "s",
				utime->tm_min, (utime->tm_min == 1)? "" : "s",
				utime->tm_sec, (utime->tm_sec == 1)? "" : "s");
	else if (utime->tm_min > 0)
		return websWrite(wp, T("%d min%s, %d sec%s"),
				utime->tm_min, (utime->tm_min == 1)? "" : "s",
				utime->tm_sec, (utime->tm_sec == 1)? "" : "s");
	else
		return websWrite(wp, T("%d sec%s"),
				utime->tm_sec, (utime->tm_sec == 1)? "" : "s");
	return 0;
}*/

static int getSysUptime(int eid, webs_t wp, int argc, char_t **argv)//modified by Gordon 2009/1/5
{
	int updays, uphours, upminutes, upseconds;
	struct sysinfo info;
	struct tm *current_time;
	time_t current_secs;

  time(&current_secs);
	current_time = localtime(&current_secs);
	sysinfo(&info);


	updays = (int) info.uptime / (60*60*24);
	upminutes = (int) info.uptime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	upseconds = ((int) info.uptime)%60;
	if (updays)
		return websWrite(wp, T("%d day%s, %d hour%s, %d min%s, %d sec%s") ,
		updays, (updays != 1) ? "s" : "",
		uphours, (uphours != 1) ? "s" : "",
		upminutes, (upminutes != 1) ? "s" : "",
		upseconds, (upseconds != 1) ? "s" : "");
	if (uphours)
		return websWrite(wp, T("%d hour%s, %d min%s, %d sec%s") ,
		uphours, (uphours != 1) ? "s" : "",
		upminutes, (upminutes != 1) ? "s" : "",
		upseconds, (upseconds != 1) ? "s" : "");
	if (upminutes)
	  return websWrite(wp, T("%d min%s, %d sec%s") ,
		upminutes, (upminutes != 1) ? "s" : "",
		upseconds, (upseconds != 1) ? "s" : "");
	else
		return websWrite(wp, T("%d sec%s") ,
		upseconds, (upseconds != 1) ? "s" : "");

	  return 0;
}

static int getPortStatus(int eid, webs_t wp, int argc, char_t **argv)
{

printf("\n ==> getPortStatus\n");
//#if (defined (CONFIG_RAETH_ROUTER) || defined CONFIG_RT_3052_ESW) && defined (CONFIG_USER_ETHTOOL)
#if 1
	
	char wanlink[10]={0};
	char wanspeed[10]={0};
		char link = '0';
		int speed = 100;
		char duplex = 'F';

	FILE *fp = popen("/sbin/ezp-portstate 5 link", "r");
	if(!fp){
		websWrite(wp, T("none"));
			return 0;
		}
	fgets(wanlink, 10, fp);
		pclose(fp);
		
  	FILE *fp_1 = popen("/sbin/ezp-portstate 5 speed", "r");
	if(!fp_1){
		websWrite(wp, T("none"));
			return 0;
	}
	fgets(wanspeed, 10, fp_1);
	pclose(fp_1);
	
	if (!strncmp(wanlink, "up", 2)){
					link = '1';
	}else if (!strncmp(wanlink, "down",4)){
		link='0';
	}else{
		link='0';
			}
	
	if (!strncmp(wanspeed, "1000", 4)){
					speed = 1000;
	}else if (!strncmp(wanlink, "100", 3)){
		speed=100;
	}else if (!strncmp(wanlink, "10", 2)){
		speed=10;
	}else{
		speed=10;
			}
	websWrite(wp, T("%c"), link);

	return 0;
#else
printf("\n ==> getPortStatus ->0\n");
	websWrite(wp, T("-1"));
	return 0;
#endif

}

inline int getOnePortOnly(void)
{
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW
	return 0;
#elif defined CONFIG_ICPLUS_PHY
	return 1;
#else
	return 0;
#endif
	return 0;
}

static int isOnePortOnly(int eid, webs_t wp, int argc, char_t **argv)
{
	if( getOnePortOnly() == 1)
		websWrite(wp, T("true"));
	else
		websWrite(wp, T("false"));
	return 0;
}

void redirect_wholepage(webs_t wp, const char *url)
{
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/html\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("<html><head><script language=\"JavaScript\">"));
	websWrite(wp, T("parent.location.replace(\"%s\");"), url);
	websWrite(wp, T("</script></head></html>"));
}

int netmask_aton(const char *ip)
{
	int i, a[4], result = 0;
	sscanf(ip, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]);
	for(i=0; i<4; i++){	//this is dirty
		if(a[i] == 255){
			result += 8;
			continue;
		}
		if(a[i] == 254)
			result += 7;
		if(a[i] == 252)
			result += 6;
		if(a[i] == 248)
			result += 5;
		if(a[i] == 240)
			result += 4;
		if(a[i] == 224)
			result += 3;
		if(a[i] == 192)
			result += 2;
		if(a[i] == 128)
			result += 1;
		//if(a[i] == 0)
		//	result += 0;
		break;
	}
	return result;
}

#if 0 // Tommy debug
static void forceMemUpgrade(webs_t wp, char_t *path, char_t *query)
{
	char_t *mode  = websGetVar(wp, T("ForceMemUpgradeSelect"), T("0"));
	if(!mode)
		return;
	if(!strcmp(mode, "1"))
		nvram_bufset(RT2860_NVRAM, "Force_mem_upgrade", "1");
	else
		nvram_bufset(RT2860_NVRAM, "Force_mem_upgrade", "0");
	nvram_commit(RT2860_NVRAM);
    websHeader(wp);
    websWrite(wp, T("<h2>force mem upgrade</h2>\n"));
    websWrite(wp, T("mode: %s<br>\n"), mode);
    websFooter(wp);
    websDone(wp, 200);
}
#endif // Tommy debug

/* goform/setOpMode */
#if 0 // Tommy debug
static void setOpMode(webs_t wp, char_t *path, char_t *query)
{
	char_t	*mode, *nat_en;
	char	*old_mode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char	*old_nat = nvram_bufget(RT2860_NVRAM, "natEnabled");
	int		need_commit = 0;
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW || defined CONFIG_ICPLUS_PHY
#else
	char	*wan_ip, *lan_ip;
#endif
// Tommy, 2008/12/23 09:08上午
//#ifdef CONFIG_RT2860V2_STA_DPB
	char_t	*econv = "";
//#endif
#if defined INIC_SUPPORT || defined INICv2_SUPPORT
	char_t	*mii;
#endif

	mode = websGetVar(wp, T("opMode"), T("0"));
	nat_en = websGetVar(wp, T("natEnbl"), T("0"));

	if (!strncmp(old_mode, "0", 2)) {
	}
	else if (!strncmp(old_mode, "1", 2) || !strncmp(old_mode, "3", 2)) {
		if (!strncmp(mode, "0", 2)) {
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW || defined CONFIG_ICPLUS_PHY
#else
			/*
			 * mode: gateway (or ap-client) -> bridge
			 * config: wan_ip(wired) overwrites lan_ip(bridge)
			 */
			wan_ip = nvram_bufget(RT2860_NVRAM, "wan_ipaddr");
			nvram_bufset(RT2860_NVRAM, "lan_ipaddr", wan_ip);
			need_commit = 1;
#endif
		}
		if (!strncmp(mode, "2", 2)) {
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW || defined CONFIG_ICPLUS_PHY
#else
			/*
			 * mode: gateway (or ap-client) -> ethernet-converter
			 * config: wan_ip(wired) overwrites lan_ip(wired)
			 *         lan_ip(wireless) overwrites wan_ip(wireless)
			 */
			wan_ip = nvram_bufget(RT2860_NVRAM, "wan_ipaddr");
			lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
			nvram_bufset(RT2860_NVRAM, "lan_ipaddr", wan_ip);
			nvram_bufset(RT2860_NVRAM, "wan_ipaddr", lan_ip);
			need_commit = 1;
#endif
		}
	}
	else if (!strncmp(old_mode, "2", 2)) {
		if (!strncmp(mode, "0", 2)) {
			/*
			 * mode: wireless-isp -> bridge
			 * config: lan_ip(wired) overwrites lan_ip(bridge) -> the same
			 */
		}
		else if (!strncmp(mode, "1", 2) || !strncmp(mode, "3", 2)) {
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW || defined CONFIG_ICPLUS_PHY
#else
			/*
			 * mode: ethernet-converter -> gateway (or ap-client)
			 * config: lan_ip(wired) overwrites wan_ip(wired)
			 *         wan_ip(wireless) overwrites lan_ip(wireless)
			 */
			wan_ip = nvram_bufget(RT2860_NVRAM, "wan_ipaddr");
			lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
			nvram_bufset(RT2860_NVRAM, "lan_ipaddr", wan_ip);
			nvram_bufset(RT2860_NVRAM, "wan_ipaddr", lan_ip);
			need_commit = 1;
#endif
		}
	}

#ifdef CONFIG_RT2860V2_STA_DPB
	if (!strncmp(mode, "0", 2)) {
		char *old;

		econv = websGetVar(wp, T("ethConv"), T("0"));
		old = nvram_bufget(RT2860_NVRAM, "ethConvert");
		if (strncmp(old, econv, 2)) {
			nvram_bufset(RT2860_NVRAM, "ethConvert", econv);
			need_commit = 1;
		}
// Tommy, 2008/10/27 11:36上午
#if 0
		if (!strncmp(econv, "1", 2)) {
#else
		if (strncmp(econv, "0", 2)) {
#endif
			//disable dhcp server in this mode
			old = nvram_bufget(RT2860_NVRAM, "dhcpEnabled");
			if (!strncmp(old, "1", 2)) {
				nvram_bufset(RT2860_NVRAM, "dhcpEnabled", "0");
				need_commit = 1;
			}
		}
	}
#endif // #ifdef CONFIG_RT2860V2_STA_DPB

	//new OperationMode
// Tommy 2008/10/29 09:47上午
	if (strncmp(mode, old_mode, 2)) {
		nvram_bufset(RT2860_NVRAM, "OperationMode", mode);
	}
		//from or to ap client mode
//		if (!strncmp(mode, "3", 2))
		if ( (!strncmp(mode, "3", 2)) || (!strncmp(econv, "2", 2)) )
			nvram_bufset(RT2860_NVRAM, "ApCliEnable", "1");
//		else if (!strncmp(old_mode, "3", 2))
		else if (strncmp(econv, "2", 2)) {
			nvram_bufset(RT2860_NVRAM, "ApCliEnable", "0");
			doSystem("ifconfig apcli0 down");
	}
		need_commit = 1;
//	} //if (strncmp(mode, old_mode, 2))

	if (strncmp(nat_en, old_nat, 2)) {
		nvram_bufset(RT2860_NVRAM, "natEnabled", nat_en);
		need_commit = 1;
	}

	// For 100PHY  ( Ethernet Convertor with one port only)
	// If this is one port only board(IC+ PHY) then redirect
	// the user browser to our alias ip address.
	if( getOnePortOnly() ){
		//     old mode is Gateway, and new mode is BRIDGE/WirelessISP/Apcli
		if (    (!strcmp(old_mode, "1") && !strcmp(mode, "0"))  ||
				(!strcmp(old_mode, "1") && !strcmp(mode, "2"))  ||
				(!strcmp(old_mode, "1") && !strcmp(mode, "3"))  ){
			char redirect_url[512];
			char *lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");

			if(! strlen(lan_ip))
				lan_ip = "10.10.10.254";
			snprintf(redirect_url, 512, "http://%s", lan_ip);
			redirect_wholepage(wp, redirect_url);
			goto final;
        }

		//     old mode is BRIDGE/WirelessISP/Apcli, and new mode is Gateway
		if (    (!strcmp(old_mode, "0") && !strcmp(mode, "1"))  ||
				(!strcmp(old_mode, "2") && !strcmp(mode, "1"))  ||
				(!strcmp(old_mode, "3") && !strcmp(mode, "1"))  ){
			redirect_wholepage(wp, "http://172.32.1.254");
			goto final;
		}
	}

#if defined INIC_SUPPORT || defined INICv2_SUPPORT
	mii = websGetVar(wp, T("miiMode"), T("0"));
	if (!strncmp(mode, "0", 2)) {
		char *old_mii = nvram_bufget(RTINIC_NVRAM, "InicMiiEnable");

		if (strncmp(mii, old_mii, 2)) {
			nvram_set(RTINIC_NVRAM, "InicMiiEnable", mii);
			need_commit = 1; //force to run initInternet
		}
	}
#endif

	websHeader(wp);
	websWrite(wp, T("<h2>Operation Mode</h2>\n"));
	websWrite(wp, T("mode: %s<br>\n"), mode);
	if (strncmp(mode, "0", 2))
		websWrite(wp, T("NAT enabled: %s<br>\n"), nat_en);
#ifdef CONFIG_RT2860V2_STA_DPB
	else
		websWrite(wp, T("DPB station: %s<br>\n"), econv);
#endif
#if defined INIC_SUPPORT || defined INICv2_SUPPORT
	websWrite(wp, T("INIC MII mode: %s<br>\n"), mii);
#endif
	websFooter(wp);
	websDone(wp, 200);

final:
	sleep(1);	// wait for websDone() to finish tcp http session(close socket)

	//restart internet if any changes
	if (need_commit) {
		nvram_commit(RT2860_NVRAM);
		updateFlash8021x(RT2860_NVRAM);
		initInternet();
	}
}
#endif // Tommy debug

// Tommy, 2008/12/16 08:15下午
/* goform/SetOperationMode */
static void SetOperationMode(webs_t wp, char_t *path, char_t *query)
{
	char_t	*mode;
	mode = websGetVar(wp, T("OPMode"), T("0"));   
	char buf_ipaddr[TMP_LEN], buf_mask[TMP_LEN], wispchannel[TMP_LEN],apchannel[TMP_LEN] ;
	char *ipAddr, *netMask, *channel; 
        
// Tommy, don't need to set these addresses
#if 0
    char	*lan_ip_router = nvram_get("lan_ipaddr_router");
    char	*lan_mask_router = nvram_get("lan_netmask_router");
    char	*lan_ip_ap = nvram_get("lan_ipaddr_ap");
    char	*lan_mask_ap = nvram_get("lan_netmask_ap");
#endif

	if (!strncmp(mode, "0", 2)) {
	    // Router Mode
	    system("/sbin/ezp-wl-mode normal");
	    //ezplib_replace_attr("wl_mode_rule", 0, "mode", "normal");
// Tommy, don't need to set these addresses
#if 1
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_normal", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_normal", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
                ipAddr = buf_ipaddr;
                netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		//ezplib_replace_attr("lan_static_rule", 0, "ipaddr", ipAddr);
                //ezplib_replace_attr("lan_static_rule", 0, "mask", netMask);
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", wispchannel, TMP_LEN, EZPLIB_USE_CLI);                
//printf("wispchannel====%s*******\n",wispchannel);
                channel = wispchannel;
                nvram_set("WISP_Channel", channel);
//printf("WISP_Channel ===%s******\n", channel);
		ezplib_replace_attr("wl_basic_rule", 0, "channel", nvram_safe_get("AP_Channel"));
		
		//Steve set lan0_proto to static when set router mode
		nvram_set("lan0_proto", "static");
#endif
	}else if (!strncmp(mode, "1", 2)){
	    //AP Mode
	    system("/sbin/ezp-wl-mode ap");
	    //ezplib_replace_attr("wl_mode_rule", 0, "mode", "ap");
// Tommy, don't need to set these addresses
#if 1
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_ap", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_ap", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
                ipAddr = buf_ipaddr;
                netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		//ezplib_replace_attr("lan_static_rule", 0, "ipaddr", ipAddr);
                //ezplib_replace_attr("lan_static_rule", 0, "mask", netMask);
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", wispchannel, TMP_LEN, EZPLIB_USE_CLI);                
//printf("wispchannel====%s*******\n",wispchannel);
                channel = wispchannel;
                nvram_set("WISP_Channel", channel);
//printf("WISP_Channel ===%s******\n", channel);  
		ezplib_replace_attr("wl_basic_rule", 0, "channel", nvram_safe_get("AP_Channel"));     
#endif		
		//nvram_bufset(RT2860_NVRAM, "lanIp_mode", "1");//fixed IP : Need to implement
	}else if (!strncmp(mode, "2", 2)){
	    // Client Mode
	}else if (!strncmp(mode, "3", 2)){
	    // WDS Mode
	}else if (!strncmp(mode, "4", 2)){
	    // AP + WDS
	}else if (!strncmp(mode, "5", 2)){
	    // Universal Repeater (AP + Client)
	    system("/sbin/ezp-wl-mode ur");
	    //ezplib_replace_attr("wl_mode_rule", 0, "mode", "ur");
// Tommy, don't need to set these addresses
#if 1
		//nvram_set("lan0_proto", "static");
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_ap", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_ap", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
                ipAddr = buf_ipaddr;
                netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		//ezplib_replace_attr("lan_static_rule", 0, "ipaddr", ipAddr);
                //ezplib_replace_attr("lan_static_rule", 0, "mask", netMask);
#endif
		//nvram_bufset(RT2860_NVRAM, "lanIp_mode", "1");//fixed IP : Need to implement
	}else if (!strncmp(mode, "6", 2)){
	    // WISP Mode <Ralink: Ethernet Converter>
	    system("/sbin/ezp-wl-mode wisp");
	    
// Enable apcli0 interface in WISP	    
	    ezplib_replace_attr("wl0_apcli_rule", 0, "enable", "1");
	    
	    //ezplib_replace_attr("wl_mode_rule", 0, "mode", "wisp");
// Tommy, don't need to set these addresses
#if 1
		nvram_set("lan0_proto", "static");
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_normal", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_normal", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
                ipAddr = buf_ipaddr;
                netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		
		//ezplib_replace_attr("lan_static_rule", 0, "ipaddr", ipAddr);
                //ezplib_replace_attr("lan_static_rule", 0, "mask", netMask);
                
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", apchannel, TMP_LEN, EZPLIB_USE_CLI);                
//printf("apchannel ===%s******\n", apchannel);
                channel = apchannel;
                nvram_set("AP_Channel", channel);
//printf("AP_Channel ===%s******\n", channel);
		ezplib_replace_attr("wl_basic_rule", 0, "channel", nvram_safe_get("WISP_Channel"));
#endif
	}
        nvram_commit();

	//added by Gordon  2009/03/18
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

}


// Steve, 2009/01/16 02:45下午
/*  */
char *ReadInfo(int type,char *s, int index)
{
	static char str[64]={0};
	char buf[70]={0};
	char file_name[20]={0};

	if(type==0){ //lan
		if(index==0)//num
		    sprintf(file_name, "/var/lltd_num_lan");
		else //info
			sprintf(file_name, "/var/lltd_inf_lan%d", index);
	}else{
	    if(index==0)//num
	    	sprintf(file_name, "/var/lltd_num_wlan");
	    else
	    	sprintf(file_name, "/var/lltd_inf_wlan%d", index);
	}

    FILE *fp = fopen(file_name, "r");
    if(!fp){
        //printf("no file ===> \n");
        return "NULL";
    }

    while(fgets(buf, 64, fp)){
    	memset(str,0,sizeof(str));
        if(!strncmp(buf,s,2)){
        	memcpy(str,buf+3,(strlen(buf)-3));
        	if (str[strlen(buf)-4]=='\n')
        		str[strlen(buf)-4]=0;
        	break;
        }
        memset(buf,0,strlen(buf));
    }
    fclose(fp);

	return str;
}

static int getCfgLLTD(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}

	return websWrite(wp, T("%s"), ReadInfo(type,field,idx));

}

#if 1//Arthur Chow 2009-02-15: For easy mode page
static int startLLTDcheck(int eid, webs_t wp, int argc, char_t **argv)
{

printf("startLLTDcheck\n");
#if 1
    FILE *fp = NULL;
    int scanned=1;
 
    fp = fopen("/var/lltd_num_lan", "r");
    if(!fp){
        scanned=0;
    }
		else
		{
			fclose(fp);
    	fp = fopen("/var/lltd_num_wlan", "r");
    	if(!fp){
        scanned=0;
    	}
			fclose(fp);
  	}

	if (!scanned)
	{
	system("lltd.sh 5 &");
	return websWrite(wp, T("1"));
}
	else	
	{
		return websWrite(wp, T("0"));
	}
#else
printf("\nSteve marked Temporarily\n");
return websWrite(wp, T("0"));
#endif
}

static int statusRoutercheck(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024];
	FILE *fp = NULL;
	int len=0;

	if (NULL == (fp = fopen("/tmp/ping_router", "r")))
	{
		websWrite(wp, T("0"));
		return 1;
	}
	len = fscanf(fp, "%s", buf);
//	printf("1.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("2.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("3.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("4.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("5.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("6.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("7.buf[%s]\n", buf);
	fclose(fp);
	if (len==1)
	{
		if (!strcmp(buf, "64"))
			websWrite(wp, T("1"));
		else
			websWrite(wp, T("0"));
		return 1;
	}
	websWrite(wp, T("0"));
	return 1;
}

static int statusInternetcheck(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024];
	FILE *fp = NULL;
	int len=0;

	if (NULL == (fp = fopen("/tmp/ping_internet", "r")))
	{
		websWrite(wp, T("0"));
		return 1;
	}
	len = fscanf(fp, "%s", buf);
//	printf("1.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("2.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("3.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("4.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("5.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("6.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("7.buf[%s]\n", buf);
	fclose(fp);
	if (len==1)
	{
		if (!strcmp(buf, "64"))
			websWrite(wp, T("1"));
		else
			websWrite(wp, T("0"));
		return 1;
	}
	websWrite(wp, T("0"));
	return 1;
}

static int startInternetcheck(int eid, webs_t wp, int argc, char_t **argv)
{
  FILE *fp = NULL;
 
  fp = fopen("/tmp/ping_internet", "r");
  if(fp){
		fclose(fp);
		return websWrite(wp, T("1"));
	}
	else
	{
	system("ping 168.95.1.1 -c 1 > /tmp/ping_internet &");
	return websWrite(wp, T("1"));
	}
}

static int javascriptLLTD(int eid, webs_t wp, int argc, char_t **argv)
{
	int idx=0;
	int lan_num=0;
	int wlan_num=0;

	lan_num=atoi(ReadInfo(0,"00",0));

	websWrite(wp, T("var lan_num=%d;\n"), lan_num);
	if (lan_num)
	{
	 websWrite(wp, T("var lan_mac=new Array(%d);\n"), lan_num);
	 websWrite(wp, T("var lan_ip=new Array(%d);\n"), lan_num);
	 websWrite(wp, T("var lan_name=new Array(%d);\n"), lan_num);
	 websWrite(wp, T("var lan_type=new Array(%d);\n"), lan_num);

	 for (idx=1; idx<=lan_num; idx++)
	 {
		websWrite(wp, T("lan_mac[%d]='%s';\n"), idx-1, ReadInfo(0, "01",idx));
		websWrite(wp, T("lan_ip[%d]='%s';\n"), idx-1, ReadInfo(0, "07",idx));
		websWrite(wp, T("lan_name[%d]='%s';\n"), idx-1, ReadInfo(0, "15",idx));
		websWrite(wp, T("lan_type[%d]=%s;\n"), idx-1, ReadInfo(0, "99",idx));
	 }
	}

	wlan_num=atoi(ReadInfo(1,"00",0));
	websWrite(wp, T("var wlan_num=%d;\n"), wlan_num);
	if (wlan_num)
	{
	 websWrite(wp, T("var wlan_mac=new Array(%d);\n"), wlan_num);
	 websWrite(wp, T("var wlan_ip=new Array(%d);\n"), wlan_num);
	 websWrite(wp, T("var wlan_name=new Array(%d);\n"), wlan_num);
	 websWrite(wp, T("var wlan_type=new Array(%d);\n"), wlan_num);

	 for (idx=1; idx<=wlan_num; idx++)
	 {
		websWrite(wp, T("wlan_mac[%d]='%s';\n"), idx-1, ReadInfo(1, "01",idx));
		websWrite(wp, T("wlan_ip[%d]='%s';\n"), idx-1, ReadInfo(1, "07",idx));
		websWrite(wp, T("wlan_name[%d]='%s';\n"), idx-1, ReadInfo(1, "15",idx));
		websWrite(wp, T("wlan_type[%d]=%s;\n"), idx-1, ReadInfo(1, "99",idx));
	 }
	}

	//Steve add 2009/03/20
	system("lltd.sh 1 &");
		
	return 1;
}

/* goform/easy_setting */
extern int WLAN_Scheduler(int setting);
extern int parental_control_switch(int flag);
extern int QoS_EasyMode(int qos_on);
extern int firewall_switch(int flag);

static void easy_setting(webs_t wp, char_t *path, char_t *query)
{
	char_t	*job_str;
	char_t	*mode_str;
	int job=0;
	int mode=0;

	job_str = websGetVar(wp, T("easy_set_button_job"), T("0"));
	if (job_str[0]!=0)
		job=atoi(job_str);

	mode_str = websGetVar(wp, T("easy_set_button_mode"), T("0"));
	if (mode_str[0]!=0)
		mode=atoi(mode_str);

	if (job==1)
	{
		if (ezModeGameEngine(mode))
		{
		}
	}

	if (job==2)
	{
		if (WLAN_Scheduler(mode))
		{
		}
	}

	if (job==3)
	{
		if (parental_control_switch(mode))
		{
		}
	}

	if (job==4)
	{
		if (QoS_EasyMode(mode))
		{
		}
	}

	if (job==5)
	{
		if (firewall_switch(mode))
		{
		}
	}

	if (job==6)
	{
//		if (set_wireless_security(mode))
		{
			//nvram_bufset(RT2860_NVRAM, "easy_wireless_security", mode_str);
			//nvram_commit(RT2860_NVRAM);
		}
	}

	websRedirect(wp, "local/easy_info.asp");
}

static int clearLLTDInfo(int eid, webs_t wp, int argc, char_t **argv)
{
	system("rm /tmp/ping_internet");
	system("rm /tmp/ping_router");
	system("rm /var/lltd_num_lan");
	system("rm /var/lltd_num_wlan");
	return websWrite(wp, T(""));
}

static int existLLTDinfo(int eid, webs_t wp, int argc, char_t **argv)
{
    FILE *fp = NULL;
    int scanned=1;
 
    fp = fopen("/var/lltd_num_lan", "r");
    if(!fp){
        scanned=0;
    }
		else
		{
			fclose(fp);
    	fp = fopen("/var/lltd_num_wlan", "r");
    	if(!fp){
        scanned=0;
    	}
			fclose(fp);
  	}

	if (!scanned)
	{
	return websWrite(wp, T("0"));
}
	else	
	{
		return websWrite(wp, T("1"));
	}
}
#endif

//FW_DATE & FW_VERSION at utils.h
#if 1//Arthur Chow 2009-03-30
static int getFirmwareDate(int eid, webs_t wp, int argc, char_t **argv)
{
    websWrite(wp, "%s ", FW_DATE);  
    return 1;
}
static int getFirmwareVersion(int eid, webs_t wp, int argc, char_t **argv)
{
    #if 1
    websWrite(wp, "%s", FW_VERSION);
    #else
    websWrite(wp, "%s ", nvram_safe_get("fw_version"));
    websWrite(wp, "(%s.", nvram_safe_get("prod_cat"));
    websWrite(wp, "%s)", nvram_safe_get("prod_subcat"));
    #endif
    return 1;
}
#endif

//Steve 2010-08-27
static int getGetUploadFlag(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fs;
	char str[2];
	if((fs=fopen("/tmp/uploadOK","r"))==NULL){
	 	websWrite(wp, T("0"));
	    return 0;
	}else
	{
	    fgets(str,2,fs);
	    //printf("The str[0]:%c ,The str[1]:%c \n",str[0],str[1]);
	}
	fclose(fs);
	if (str[0] == '1'){
	     websWrite(wp, T("1"));
	     return 1;
	}else{
	     websWrite(wp, T("0"));
	     return 0;
	}
}

#endif //Steve2023