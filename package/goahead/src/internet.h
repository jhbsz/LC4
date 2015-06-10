/*
 *	internet.h -- Internet Configuration Header
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: internet.h,v 1.8 2008-03-17 07:47:16 yy Exp $
 */

void formDefineInternet(void);
int getIfLive(char *ifname);
int getIfMac(char *ifname, char *if_hw);
int getIfIp(char *ifname, char *if_addr);
int getIfIsUp(char *ifname);
int getIfNetmask(char *ifname, char *if_net);
char* getWanIfName(void);
char* getWanIfNamePPP(void);
char* getLanIfName(void);
char *getLanWanNamebyIf(char *ifname);
int initInternet(void);
#define RECONNECT_MSG(url){ \
	websHeader(wp); \
	websWrite(wp, T("<h3>Change setting successfully!</h3><BR><BR>" \
	"<p>Please wait a moment to let the new settings take effect!! (about 40~50 seconds)<BR><BR>" \
	"If the IP address is changed, please go to <a href=\"http://%s\" target=\"_top\" alt=\"%s\" ><b>New IP address</b></a> to continue your work.</p>"), url, url);\
	websFooter(wp); \
	websDone(wp, 200); \
}
