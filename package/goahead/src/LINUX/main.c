/*
 * main.c -- Main program for the GoAhead WebServer (LINUX version)
 *
 * Copyright (c) GoAhead Software Inc., 1995-2000. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 * $Id: main.c,v 1.5 2003/09/11 14:03:46 bporter Exp $
 */

/******************************** Description *********************************/

/*
 *	Main program for for the GoAhead WebServer. This is a demonstration
 *	main program to initialize and configure the web server.
 */

/********************************* Includes ***********************************/

#include	"../uemf.h"
#include	"../wsIntrn.h"
#include	<signal.h>
#include	<unistd.h> 
#include	<sys/types.h>
#include	<sys/wait.h>

#ifdef WEBS_SSL_SUPPORT
#include	"../websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include	"../um.h"
void	formDefineUserMgmt(void);
#endif

#include <bcmnvram.h>
#include "ezp-lib.h"
#include	"../management.h"
#include	"../qos.h"
#include	"../firewall.h"
#include	"../wireless.h"
#include	"../internet.h"



/*********************************** Locals ***********************************/
/*
 *	Change configuration here
 */
//Steve checked
static char_t		*rootWeb = T("/web");			/* Root web directory */
static char_t		*password = T("");				/* Security password */
static int			port = 80;						/* Server port */
static int			retries = 5;					/* Server port retries */
static int			finished;						/* Finished flag */

/****************************** Forward Declarations **************************/

//Steve add
static int 	initSystem(void);
typedef struct {
	unsigned int irq;		//request irq pin number
	pid_t pid;			//process id to notify
} ralink_gpio_reg_info;
#define	RALINK_GPIO_SET_DIR		0x01
#define RALINK_GPIO_SET_DIR_IN		0x11
#define RALINK_GPIO_0			0x00000001
#define RALINK_GPIO_10			0x00000400
#define RALINK_GPIO_REG_IRQ		0x0A
extern void WPSAPPBCStartAll(void);
extern void WPSSingleTriggerHandler(int); //Steve patched 2010/09/30
//

static int 	initWebs();
static int	aspTest(int eid, webs_t wp, int argc, char_t **argv);
static void formTest(webs_t wp, char_t *path, char_t *query);
static int  websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
				int arg, char_t *url, char_t *path, char_t *query);
extern void defaultErrorHandler(int etype, char_t *msg);
extern void defaultTraceHandler(int level, char_t *buf);
#ifdef B_STATS
static void printMemStats(int handle, char_t *fmt, ...);
static void memLeaks();
#endif

/*********************************** Code *************************************/
/*
 *	Main -- entry point from LINUX
 */

int main(int argc, char** argv)
{
/*
 *	Initialize the memory allocator. Allow use of malloc and start 
 *	with a 60K heap.  For each page request approx 8KB is allocated.
 *	60KB allows for several concurrent page requests.  If more space
 *	is required, malloc will be used for the overflow.
 */
	bopen(NULL, (60 * 1024), B_USE_MALLOC);
	signal(SIGPIPE, SIG_IGN);
    
/*
 *	Initialize the web server
 */
	if (initWebs() < 0) {
		return -1;
	}

	//Steve add
	if (initSystem() < 0)
		return -1;

#ifdef WEBS_SSL_SUPPORT
	websSSLOpen();
#endif

/*
 *	Basic event loop. SocketReady returns true when a socket is ready for
 *	service. SocketSelect will block until an event occurs. SocketProcess
 *	will actually do the servicing.
 */
	while (!finished) {
		if (socketReady(-1) || socketSelect(-1, 1000)) {
			socketProcess(-1);
		}
		websCgiCleanup();
		emfSchedProcess();
	}

#ifdef WEBS_SSL_SUPPORT
	websSSLClose();
#endif

#ifdef USER_MANAGEMENT_SUPPORT
	umClose();
#endif

/*
 *	Close the socket module, report memory leaks and close the memory allocator
 */
 
	websCloseServer();
	socketClose();
#ifdef B_STATS
	memLeaks();
#endif
	bclose();
	return 0;
}

//Steve==================================================================================
static void goaSigHandler(int signum)
{
// Tommy, WPS 2009/3/11 10:38
	char *Radio_ON,*operation_mode,*scheduling_enable,*schedulingRadioOn;
	//Radio_OFF = nvram_bufget(RT2860_NVRAM, "RadioOff");
	//operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	//wsc_enable = nvram_bufget(RT2860_NVRAM, "WscModeOption");
	//scheduling_enable = nvram_bufget(RT2860_NVRAM, "WiFiScheduleEnable");
	//schedulingRadioOn = nvram_bufget(RT2860_NVRAM, "SchedulerRadioOn");
	
	char OPMode[32];
	char TempBuf[32];
	char SchedEnable[32];
	char RadioON[32];
	char wsc_enable[3];
	
    ezplib_get_attr_val("wl_wps_rule", 0, "mode", wsc_enable, sizeof(wsc_enable), EZPLIB_USE_CLI);
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "normal")){
		strcpy(OPMode, "0");
	}else if (!strcmp(TempBuf, "ap")){
		strcpy(OPMode, "1");
	}else if (!strcmp(TempBuf, "ur")){
		strcpy(OPMode, "5");
	}else if (!strcmp(TempBuf, "wisp")){
		strcpy(OPMode, "6");
	}
	operation_mode = OPMode;
	scheduling_enable = nvram_safe_get("sched_enable");
	
	ezplib_get_attr_val("wl_basic_rule", 0, "enable", RadioON, 32, EZPLIB_USE_CLI);
	Radio_ON = RadioON;


//#ifdef CONFIG_RT2860V2_STA_WSC
//	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
//	char *ethCon = nvram_bufget(RT2860_NVRAM, "ethConvert");
//#endif

	if (signum != SIGUSR1)
		return;

// Tommy, 2009/3/10 08:29
#if 1
	#if 1
		if ( (!strcmp(operation_mode, "6")) || (!strcmp(Radio_ON, "0")) || (!strcmp(wsc_enable, "0")) ){
	#else
		if ( (!strcmp(operation_mode, "6")) || (!strcmp(Radio_OFF, "1")) || (!strcmp(wsc_enable, "0")) ||
			(!strcmp(scheduling_enable, "1")) && (!strcmp(schedulingRadioOn, "0")) ){
	#endif
printf("@@@@@@@@@@@@@@@ DON'T WPS !!!!\n");
		//system("gpio l 14 0 4000 0 1 4000"); //trun off WPS led
	}else{
printf("@@@@@@@@@@@@@@@ RUN WPS !!!!\n");
// AXIMCOM Holmas: Set following command(WPS LED) before execute WPS
	ezplib_replace_attr("wl_led_status_rule", 0, "wps", "1");
		WPSAPPBCStartAll();
}
#else
// AXIMCOM Holmas: Set following command(WPS LED) before execute WPS
	ezplib_replace_attr("wl_led_status_rule", 0, "wps", "1");
		WPSAPPBCStartAll();
#endif
}

static void goaInitGpio()
{
	int fd;
	ralink_gpio_reg_info info;

	fd = open("/dev/gpio", O_RDONLY);
	if (fd < 0) {
		perror("/dev/gpio");
		return;
	}

// Tommy, fixed SDK3.2 WPS push button problem
	if (ioctl(fd, RALINK_GPIO_SET_DIR_IN, RALINK_GPIO_0))
  	    goto ioctl_err;

// Tommy, fixed Reset button cannot work in SDK3.2
	if (ioctl(fd, RALINK_GPIO_SET_DIR_IN, RALINK_GPIO_10))
  	    goto ioctl_err;
  	    
	//register my information
	info.pid = getpid();
	info.irq = 0;
	if (ioctl(fd, RALINK_GPIO_REG_IRQ, &info) < 0)
		goto ioctl_err;
	close(fd);

	//issue a handler to handle SIGUSR1
	signal(SIGUSR1, goaSigHandler);
	
	//Steve patched 2010/09/30
	signal(SIGXFSZ, WPSSingleTriggerHandler);
	
	return;

ioctl_err:
	perror("ioctl");
	close(fd);
	return;
}

/******************************************************************************/
/*
 *	Initialize System Parameters
 */
static int initSystem(void)
{

	goaInitGpio();

	return 0;
}

//Steve


/******************************************************************************/
/*
 *	Initialize the web server.
 */

static int initWebs()
{
	struct hostent	*hp;
	struct in_addr	intaddr;
	char			host[128], dir[128], webdir[128];
	char			*cp;
	char_t			wbuf[128];
	
	//Steve tmp add 2010/04/22
	char			*lan_ip=nvram_safe_get("lan0_ipaddr");
	

/*
 *	Initialize the socket subsystem
 */
	socketOpen();
printf("\n\n Check initWebs()=%s \n\n", lan_ip);
system("/usr/sbin/cplange.sh");
#if 0 //def USER_MANAGEMENT_SUPPORT
/*
 *	Initialize the User Management database
 */
 //Steve patch
 #if 1
 	//char *admu = nvram_bufget(RT2860_NVRAM, "Login");
	//char *admp = nvram_bufget(RT2860_NVRAM, "Password");
	printf("\n\n Check USER_MANAGEMENT_SUPPORT\n\n");
	char *admu = "admin"
	char *admp = "1234";
	umOpen();
	//umRestore(T("umconfig.txt"));
	//winfred: instead of using umconfig.txt, we create 'the one' adm defined in nvram
	umAddGroup(T("adm"), 0x07, AM_DIGEST, FALSE, FALSE);
	if (admu && strcmp(admu, "") && admp && strcmp(admp, "")) {
		umAddUser(admu, admp, T("adm"), FALSE, FALSE);
		umAddAccessLimit(T("/"), AM_DIGEST, FALSE, T("adm"));
	}
	else
		error(E_L, E_LOG, T("gohead.c: Warning: empty administrator account or password"));
 #else
	umOpen();
	umRestore(T("umconfig.txt"));
 #endif
#endif

//Steve modified 2010/04/22
#if 0
/*
 *	Define the local Ip address, host name, default home page and the 
 *	root web directory.
 */
	if (gethostname(host, sizeof(host)) < 0) {
		error(E_L, E_LOG, T("Can't get hostname"));
		return -1;
	}
	if ((hp = gethostbyname(host)) == NULL) {
		error(E_L, E_LOG, T("Can't get host address"));
		return -1;
	}
	memcpy((char *) &intaddr, (char *) hp->h_addr_list[0],
		(size_t) hp->h_length);
#else
/*
 * get ip address from nvram configuration (we executed initInternet)
 */

	if (NULL == lan_ip) {
		error(E_L, E_LOG, T("initWebs: cannot find lan_ip in NVRAM"));
		return -1;
	}
	intaddr.s_addr = inet_addr(lan_ip);
	if (intaddr.s_addr == INADDR_NONE) {
		error(E_L, E_LOG, T("initWebs: failed to convert %s to binary ip data"),
				lan_ip);
		return -1;
	}
#endif


/*
 *	Set ../web as the root web. Modify this to suit your needs
 */
//Steve modified 2010/04/22
#if 0
	getcwd(dir, sizeof(dir)); 
	if ((cp = strrchr(dir, '/'))) {
		*cp = '\0';
	}
	sprintf(webdir, "%s/%s", dir, rootWeb);
#endif
	sprintf(webdir, "%s", rootWeb);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultDir(webdir);
	cp = inet_ntoa(intaddr);
	ascToUni(wbuf, cp, min(strlen(cp) + 1, sizeof(wbuf)));
	websSetIpaddr(wbuf);
//Steve patched 2010/05/26
#if 0 // GA_HOSTNAME_SUPPORT
	ascToUni(wbuf, host, min(strlen(host) + 1, sizeof(wbuf)));
#else
	//use ip address (already in wbuf) as host
#endif
//Steve
	websSetHost(wbuf);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultPage(T("default.asp"));
	websSetPassword(password);

/* 
 *	Open the web server on the given port. If that port is taken, try
 *	the next sequential port for up to "retries" attempts.
 */
	websOpenServer(port, retries);

/*
 * 	First create the URL handlers. Note: handlers are called in sorted order
 *	with the longest path handler examined first. Here we define the security 
 *	handler, forms handler and the default web page handler.
 */
	websUrlHandlerDefine(T(""), NULL, 0, websSecurityHandler, 
		WEBS_HANDLER_FIRST);
	websUrlHandlerDefine(T("/goform"), NULL, 0, websFormHandler, 0);
	websUrlHandlerDefine(T("/cgi-bin"), NULL, 0, websCgiHandler, 0);
	websUrlHandlerDefine(T(""), NULL, 0, websDefaultHandler, 
		WEBS_HANDLER_LAST); 

//Steve patched 2010/04/22
/*
 *	Define our functions
 */

	formDefineManagement();
	formDefineQoS();
	formDefineFirewall();
	formDefineWireless();
	formDefineWPS();
	formDefineInternet();
	formDefineUtilities();

/*
 *	Create the Form handlers for the User Management pages
 */
#ifdef USER_MANAGEMENT_SUPPORT
	//formDefineUserMgmt(); //Steve marked base on NBG-419 goahead
#endif

/*
 *	Create a handler for the default home page
 */
	websUrlHandlerDefine(T("/"), NULL, 0, websHomePageHandler, 0); 
	return 0;
}

/******************************************************************************/
/*
 *	Test Javascript binding for ASP. This will be invoked when "aspTest" is
 *	embedded in an ASP page. See web/asp.asp for usage. Set browser to 
 *	"localhost/asp.asp" to test.
 */

static int aspTest(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*name, *address;

	if (ejArgs(argc, argv, T("%s %s"), &name, &address) < 2) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	return websWrite(wp, T("Name: %s, Address %s"), name, address);
}

/******************************************************************************/
/*
 *	Test form for posted data (in-memory CGI). This will be called when the
 *	form in web/forms.asp is invoked. Set browser to "localhost/forms.asp" to test.
 */

static void formTest(webs_t wp, char_t *path, char_t *query)
{
	char_t	*name, *address;

	name = websGetVar(wp, T("name"), T("Joe Smith")); 
	address = websGetVar(wp, T("address"), T("1212 Milky Way Ave.")); 

	websHeader(wp);
	websWrite(wp, T("<body><h2>Name: %s, Address: %s</h2>\n"), name, address);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *	Home page handler
 */

static int websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t *url, char_t *path, char_t *query)
{
/*
 *	If the empty or "/" URL is invoked, redirect default URLs to the home page
 */
	if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
		
		//Steve patched base on NBG-419N goahead 2010/04/22
		#if 1//Arthur Chow 2008-11-14: Define Root page
			//char_t *first_config=nvram_get(RT2860_NVRAM, "FirstConfig");
			char_t *first_config=nvram_safe_get("FirstConfig");
			if (!strcmp(first_config, "1"))
				websRedirect(wp, T("easy.asp"));
			else
				websRedirect(wp, T("index.asp"));
		#else
			websRedirect(wp, T("home.asp"));
		#endif
		return 1;
	}
	return 0;
}

/******************************************************************************/
/*
 *	Default error handler.  The developer should insert code to handle
 *	error messages in the desired manner.
 */

void defaultErrorHandler(int etype, char_t *msg)
{
//Steve base om NBG-419
//#if 0
	write(1, msg, gstrlen(msg));
//#endif
}

/******************************************************************************/
/*
 *	Trace log. Customize this function to log trace output
 */

void defaultTraceHandler(int level, char_t *buf)
{
/*
 *	The following code would write all trace regardless of level
 *	to stdout.
 */
 //Steve modified base on 419N
//#if 0
	if (buf) {
		if (0 == level)
			write(1, buf, gstrlen(buf));
	}
//#endif
}

/******************************************************************************/
/*
 *	Returns a pointer to an allocated qualified unique temporary file name.
 *	This filename must eventually be deleted with bfree();
 */
//Steve patched 2010/05/26
#if 0 //defined CONFIG_USER_STORAGE
char_t *websGetCgiCommName(webs_t wp)
{
	char *force_mem_upgrade = nvram_bufget(RT2860_NVRAM, "Force_mem_upgrade");
	char_t	*pname1 = NULL, *pname2 = NULL;
	char *part;

	if(!strcmp(force_mem_upgrade, "1")){
		pname1 = (char_t *)tempnam(T("/var"), T("cgi"));
	}else if(wp && (wp->flags & WEBS_CGI_FIRMWARE_UPLOAD) ){
		// see if usb disk is present and available space is enough?
		if( (part = isStorageOK()) )
			pname1 = (char_t *)tempnam(part, T("cgi"));
		else
			pname1 = (char_t *)tempnam(T("/var"), T("cgi"));
	}else{
		pname1 = (char_t *)tempnam(T("/var"), T("cgi"));
	}

	pname2 = bstrdup(B_L, pname1);
	free(pname1);

	return pname2;
}
#else
char_t *websGetCgiCommName(webs_t wp)
{
	char_t	*pname1, *pname2;

	pname1 = (char_t *)tempnam(T("/var"), T("cgi"));
	pname2 = bstrdup(B_L, pname1);
	free(pname1);

	return pname2;
}
#endif
//Steve
/******************************************************************************/
/*
 *	Launch the CGI process and return a handle to it.
 */

int websLaunchCgiProc(char_t *cgiPath, char_t **argp, char_t **envp,
					  char_t *stdIn, char_t *stdOut)
{
	int	pid, fdin, fdout, hstdin, hstdout, rc;

	fdin = fdout = hstdin = hstdout = rc = -1; 
	if ((fdin = open(stdIn, O_RDWR | O_CREAT, 0666)) < 0 ||
		(fdout = open(stdOut, O_RDWR | O_CREAT, 0666)) < 0 ||
		(hstdin = dup(0)) == -1 ||
		(hstdout = dup(1)) == -1 ||
		dup2(fdin, 0) == -1 ||
		dup2(fdout, 1) == -1) {
		goto DONE;
	}
		
 	rc = pid = fork();
 	if (pid == 0) {
/*
 *		if pid == 0, then we are in the child process
 */
		if (execve(cgiPath, argp, envp) == -1) {
			printf("content-type: text/html\n\n"
				"Execution of cgi process failed\n");
		}
		exit (0);
	} 

DONE:
	if (hstdout >= 0) {
		dup2(hstdout, 1);
      close(hstdout);
	}
	if (hstdin >= 0) {
		dup2(hstdin, 0);
      close(hstdin);
	}
	if (fdout >= 0) {
		close(fdout);
	}
	if (fdin >= 0) {
		close(fdin);
	}
	return rc;
}

/******************************************************************************/
/*
 *	Check the CGI process.  Return 0 if it does not exist; non 0 if it does.
 */

int websCheckCgiProc(int handle)
{
/*
 *	Check to see if the CGI child process has terminated or not yet.  
 */
	if (waitpid(handle, NULL, WNOHANG) == handle) {
		return 0;
	} else {
		return 1;
	}
}

/******************************************************************************/

#ifdef B_STATS
static void memLeaks() 
{
	int		fd;

	if ((fd = gopen(T("leak.txt"), O_CREAT | O_TRUNC | O_WRONLY, 0666)) >= 0) {
		bstats(fd, printMemStats);
		close(fd);
	}
}

/******************************************************************************/
/*
 *	Print memory usage / leaks
 */

static void printMemStats(int handle, char_t *fmt, ...)
{
	va_list		args;
	char_t		buf[256];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	write(handle, buf, strlen(buf));
}
#endif

/******************************************************************************/
