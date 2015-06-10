/*
*	History---
*
*	2009-07-01, RL modified. Combine LAN and WLAN to only ONE class to LAN.
*	2009-07-04, RL modified. Added default class to show on bandwidth MGMT monitor.
*	2009-07-09, RL modified. Added check mechanism for sum of minimum rate.
													 Added service for rule configuration.
*/


#include    <stdlib.h>
#include    <sys/ioctl.h>
#include    <net/if.h>
#include    <net/route.h>
#include    <string.h>
#include    <dirent.h>

#include    "qos.h"
#include    "webs.h"
//#include    "internet.h"
//#include    "nvram.h"
//#include    "utils.h"
//#include    "firewall.h"
//#include    "config/autoconf.h" //user config
#include 	<bcmnvram.h>
#include 	"ezp-lib.h"

#if 1 //Steve18
extern char l7name[];							// in firewall.c
inline int getRuleNums(char *);
#if 0
struct entry_s QOS_PROFILE[QOS_PROFILE_ENTRYS_MAX] = {
	{"QoSAF5Name","High"},
	{"QoSAF5Rate","30"},
	{"QoSAF5Ceil","100"},
	{"QoSAF2Name","Middle"},
	{"QoSAF2Rate","20"},
	{"QoSAF2Ceil","100"},
	{"QoSAF6Name","Default"},
	{"QoSAF6Rate","5"},
	{"QoSAF6Ceil","100"},
	{"QoSAF1Name","Low"},
	{"QoSAF1Rate","10"},
	{"QoSAF1Ceil","100"},
	{"QoSRules", "ICMP_HIGH,5,1,,ICMP,,,,,,,,,,,,EF;Small_Packet_HIGH,5,1,,,,,0,128,,,,,,,,EF;VoIP_H323_HIGH,5,1,,Application,,,,,,,,,h323,,,EF;VoIP_SIP_HIGH,5,1,,Application,,,,,,,,,sip,,,EF;VoIP_Skype1_HIGH,5,1,,Application,,,,,,,,,skypeout,,,EF;VoIP_Skype2_HIGH,5,1,,Application,,,,,,,,,skypetoskype,,,EF;RTP_HIGH,5,1,,Application,,,,,,,,,rtp,,,EF;SSH_HIGH,5,1,,Application,,,,,,,,,ssh,,,EF;MSN_Messenger_MIDDLE,2,1,,Application,,,,,,,,,msnmessenger,,,AF21;Yahoo_MIDDLE,2,1,,Application,,,,,,,,,yahoo,,,AF21;PoP3_LOW,1,1,,Application,,,,,,,,,msnmessenger,,,AF11;SMTP_LOW,1,1,,Application,,,,,,,,,smtp,,,AF11;P2P_eMule_LOW,1,1,,Application,,,,,,,,,edonkey,,,AF11;P2P_BT_LOW,1,1,,Application,,,,,,,,,bittorrent,,,AF11"},
	{ NULL, NULL}
};
#endif
//RL define

#if 1//Arthur Chow 2009-03-02: For getWebMessage
extern int setWebMessage(int flag, char *str);
#endif
int QoS_EasyMode(int);
char_t STR_UPLINLK_SPEED[8]={0};
char_t STR_DOWNLINK_SPEED[8]={0};
int UPLINLK_SPEED;
int DOWNLINK_SPEED;
char_t *defCategory[]={"Game Console", "VoIP", "Instant Messenger", "Web Surfing", "P2P/FTP", "E-Mail" }; //6
char_t *appQoSService[]={"XBox Live", "PlayStation", "MSN Game Zone", "Battlenet", "VoIP", "Instant Messenger ","Web Surfing", "FTP", "eMule","BitTorrent", "E-Mail" }; //11
char_t *defCateName[]={"game", "voip", "media", "web", "p2p", "mail" };

//----------------------------------------------------
inline void QoSRestart(void)
{
#if 0
	FILE *fp = fopen("/bin/qos_run", "r");
	if(!fp)
		return;
	fclose(fp);
	doSystem("/bin/qos_run");
#endif
	//FILE *fp = fopen("/sbin/qos.sh", "r");
	FILE *fp = fopen("/sbin/rl_qos", "r");
	if(!fp)
		return;
	fclose(fp);
	//doSystem("/sbin/qos.sh");
	doSystem("/sbin/rl_qos");
}

inline void QoSInit(int isLan)
{
#if 0
	char *cm;
	FILE *fs_ppplink;
	char str[2];
	cm = nvram_bufget(RT2860_NVRAM, "wanConnectionMode");
	if (!strncmp(cm, "PPPOE", 6) || !strncmp(cm, "L2TP", 5) || !strncmp(cm, "PPTP", 5)){
	#if 0
		if((fs_ppplink=fopen("/etc/link","r"))==NULL)
		    return ;
		else
		{
		    fgets(str, 2, fs_ppplink);
		    //printf("The str[0]:%c ,The str[1]:%c \n",str[0],str[1]);
		}
		fclose(fs_ppplink);
		if (str[0] != '1')
		     return;
	#endif
		//RL added if LAN set qosinit, even we in PPPoE, we must run.
		if(isLan==1)
			QoSRestart();
		// Just return.
		// The PPP daemon would trigger "qos_run" after dial up successfully, so do nothing here.
		return;
	}

	QoSRestart();
#endif
}
//RL marked Ralink codes for reduce code size.
#if 0
static void QoSAFAttribute(webs_t wp, char_t *path, char_t *query)
{
	char tmp[512];
	char_t *index, *name, *rate, *ceil;
	int index_i;
	index = websGetVar(wp, T("af_index"), T(""));
	name = websGetVar(wp, T("af_name"), T(""));
	rate = websGetVar(wp, T("af_rate"), T(""));
	ceil = websGetVar(wp, T("af_ceil"), T(""));
	if(!index|| !name || !rate || !ceil)
		return;
	if(!strlen(index) || !strlen(name) || !strlen(rate) || !strlen(ceil))
		return;
	index_i = atoi(index);
	if(index_i > 6 || index_i <= 0)
		return;
	if(atoi(rate) > 100 || atoi(ceil) > 100)		//percentage
		return;

	sprintf(tmp, "QoSAF%dName", index_i);
	nvram_bufset(RT2860_NVRAM, tmp, name);
	sprintf(tmp, "QoSAF%dRate", index_i);
	nvram_bufset(RT2860_NVRAM, tmp, rate);
	sprintf(tmp, "QoSAF%dCeil", index_i);
	nvram_bufset(RT2860_NVRAM, tmp, ceil);
	nvram_commit(RT2860_NVRAM);

	QoSRestart();

    websWrite(wp, T("HTTP/1.0 200 OK\n"));
    websWrite(wp, T("Server: %s\r\n"), WEBS_NAME);
    websWrite(wp, T("Pragma: no-cache\n"));
    websWrite(wp, T("Cache-control: no-cache\n"));
    websWrite(wp, T("Content-Type: text/html\n"));
    websWrite(wp, T("\n"));
    websWrite(wp, T("<html>\n<head>\n"));
    websWrite(wp, T("<title>My Title</title>"));
    websWrite(wp, T("<link rel=\"stylesheet\" href=\"/style/normal_ws.css\" type=\"text/css\">"));
    websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\">"));
    websWrite(wp, T("</head>\n<body onload=\"opener.location.reload();window.close();\">\n"));

	websFooter(wp);
	websDone(wp, 200);
}

static int qosGetIndexByName(int af_index, int dp_index, char *match)
{
	int i;
	char rec[256];
	char asdp_str[512];
	char *rule, name[32];

	sprintf(asdp_str, "QoSAF%dDP%d", af_index, dp_index);
	rule = nvram_bufget(RT2860_NVRAM, asdp_str);
	if(!rule || !strlen(rule))
		return -1;

	i=0;
	while( (getNthValueSafe(i, rule, ';', rec, sizeof(rec)) != -1) ){
		if( (getNthValueSafe(0, rec, ',', name, sizeof(name)) == -1) )
			continue;
		if(!strcmp(name, match))
			return i;
		i++;
	}
	return -1;
}

static void qosClassifier(webs_t wp, char_t *path, char_t *query)
{
	char rule[4096];
	char *old_rule;
	char tmp[8];

	char_t *af_index, *dp_index, *comment, *mac_address, *dip_address, *sip_address, *pktlenfrom, *pktlento,
			*protocol, *dprf, *dprt, *sprf, *sprt, *layer7, *dscp, *ingress_if, *remark ;
	int af_index_i, dp_index_i, sprf_int, dprf_int, sprt_int, dprt_int;

	af_index = websGetVar(wp, T("af_index"), T(""));
	dp_index = websGetVar(wp, T("dp_index"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));
	mac_address = websGetVar(wp, T("mac_address"), T(""));
	dip_address = websGetVar(wp, T("dip_address"), T(""));
	sip_address = websGetVar(wp, T("sip_address"), T(""));
	pktlenfrom = websGetVar(wp, T("pktlenfrom"), T(""));
	pktlento = websGetVar(wp, T("pktlento"), T(""));
	protocol = websGetVar(wp, T("protocol"), T(""));
	dprf = websGetVar(wp, T("dFromPort"), T(""));
	dprt = websGetVar(wp, T("dToPort"), T(""));
	sprf = websGetVar(wp, T("sFromPort"), T(""));
	sprt = websGetVar(wp, T("sToPort"), T(""));
	layer7 = websGetVar(wp, T("layer7"), T(""));
	dscp = websGetVar(wp, T("dscp"), T(""));
	/*ingress_if = websGetVar(wp, T("ingress_if"), T(""));*/
	ingress_if = "";
	remark =  websGetVar(wp, T("remark_dscp"), T(""));

	if(!af_index || !dp_index || !comment || !remark)
		return;

	if(!strlen(af_index) || !strlen(dp_index) || strlen(comment) > 32)
		return;

	// i know you will try to break our box... ;)
	if(strchr(comment, ';') || strchr(comment, ','))
		return;

	af_index_i = atoi(af_index);
	dp_index_i = atoi(dp_index);
	if(qosGetIndexByName(af_index_i, dp_index_i, comment) != -1){
		// This name is not unique.
		// return;
	}

	// pkt len from/to must be co-exist.
	if( (!strlen(pktlenfrom) && strlen(pktlento)) ||
		(strlen(pktlenfrom) && !strlen(pktlento)) )
		return;

	if(!strlen(protocol)){
		if(	!strlen(mac_address) && !strlen(sip_address) && !strlen(dip_address) &&
			!strlen(dscp) /*&& !strlen(ingress_if)*/ && !strlen(pktlenfrom) )
			return;
		layer7 = "";
	}else if(!strcmp(protocol, "TCP") || !strcmp(protocol, "UDP")){
		if(!strlen(dprf) && !strlen(sprf))
			return;
		layer7 = "";
	}else if(!strcmp(protocol, "Application")){
		if(!strlen(layer7))
			return;
	}else if(!strcmp(protocol, "ICMP")){
		layer7 = "";	// do nothing.
	}else
		return;

	// we dont trust user input.....
	if(strlen(mac_address)){
		if(!isMacValid(mac_address))
			return;
	}

	if(strlen(sip_address)){
		if(!isIpNetmaskValid(sip_address))
			return;
	}

    if(strlen(dip_address)){
        if(!isIpNetmaskValid(dip_address))
            return;
    }


	if(!strlen(sprf)){
		sprf_int = 0;
	}else{
		sprf_int = atoi(sprf);
		if(sprf_int == 0 || sprf_int > 65535)
			return;
	}
	if(!strlen(sprt)){
		sprt_int = 0;
	}else{
		sprt_int = atoi(sprt);
		if(sprt_int ==0 || sprt_int > 65535)
			return;
	}
	if(!strlen(dprf)){
		dprf_int = 0;
	}else{
		dprf_int = atoi(dprf);
		if(dprf_int == 0 || dprf_int > 65535)
			return;
	}
	if(!strlen(dprt)){
		dprt_int = 0;
	}else{
		dprt_int = atoi(dprt);
		if(dprt_int == 0 || dprt_int > 65535)
			return;
	}

	if(!strcmp(remark, "Auto")){
		if(af_index_i == 5 && dp_index_i == 1){				/* EF class*/
			remark = "EF";
		}else if(af_index_i == 6 && dp_index_i == 1){		/* BE classes */
			remark = "BE";
		}else{                                  /* AF classes */
            snprintf(tmp, sizeof(tmp), "AF%d%d", af_index_i, dp_index_i);
            remark = tmp;
        }
	}

	old_rule = nvram_bufget(RT2860_NVRAM, "QoSRules");
	if(!old_rule || !strlen(old_rule))
		snprintf(rule, sizeof(rule), "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", comment, af_index, dp_index, mac_address, protocol, dip_address, sip_address, pktlenfrom, pktlento, dprf, dprt, sprf, sprt, layer7, dscp, ingress_if, remark);
	else
		snprintf(rule, sizeof(rule), "%s;%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", old_rule, comment, af_index, dp_index, mac_address, protocol, dip_address, sip_address, pktlenfrom, pktlento, dprf, dprt, sprf, sprt, layer7, dscp, ingress_if, remark);

	nvram_bufset(RT2860_NVRAM, "QoSRules", rule);
	nvram_commit(RT2860_NVRAM);

	QoSRestart();

//	websHeader(wp);
    websWrite(wp, T("HTTP/1.0 200 OK\n"));
    websWrite(wp, T("Server: %s\r\n"), WEBS_NAME);
    websWrite(wp, T("Pragma: no-cache\n"));
    websWrite(wp, T("Cache-control: no-cache\n"));
    websWrite(wp, T("Content-Type: text/html\n"));
    websWrite(wp, T("\n"));
    websWrite(wp, T("<html>\n<head>\n"));
    websWrite(wp, T("<title>My Title</title>"));
    websWrite(wp, T("<link rel=\"stylesheet\" href=\"/style/normal_ws.css\" type=\"text/css\">"));
    websWrite(wp, T("<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\">"));
    websWrite(wp, T("</head>\n<body onload=\"opener.location.reload();window.close();\">\n"));

	websWrite(wp, T("name: %s<br>\n"), comment);
	websWrite(wp, T("mac: %s<br>\n"), mac_address);
	websWrite(wp, T("ingress interface: %s<br>\n"), ingress_if);
	websWrite(wp, T("sip_address: %s<br>\n"), sip_address);
	websWrite(wp, T("sFromPort: %s<br>\n"), sprf);
	websWrite(wp, T("sToPort: %s<br>\n"), sprt);
	websWrite(wp, T("dip_address: %s<br>\n"), dip_address);
	websWrite(wp, T("dFromPort: %s<br>\n"), dprf);
	websWrite(wp, T("dToPort: %s<br>\n"), dprt);
	websWrite(wp, T("protocol: %s<br>\n"), protocol);
	websWrite(wp, T("application: %s<br>\n"), layer7);
	websWrite(wp, T("dscp: %s<br>\n"), dscp);
	websWrite(wp, T("remark: %s<br>\n"), remark);

	websFooter(wp);
	websDone(wp, 200);
}

static void QoSDelete(webs_t wp, char_t *path, char_t *query)
{
	char tmp[32];
	char *orig_rule, *new_rule;
	int del_index, af_i, dp_i;

	char_t af[2], dp[2], name[64];
    printf("query = %s\n", query);
    sscanf(query, "%2s %2s %64s", af, dp, name);
    printf("af = %s\n dp = %s\n name = %s\n", af, dp, name);

	af_i = atoi(af);
	dp_i = atoi(dp);

	snprintf(tmp, sizeof(tmp), "QoSAF%dDP%d", af_i, dp_i);
	orig_rule = nvram_bufget(RT2860_NVRAM, tmp);
	if(!orig_rule || !strlen(orig_rule))
		goto err;

	del_index = qosGetIndexByName(af_i, dp_i, name);
	printf("del_index = %d\n", del_index);
	if(del_index == -1)
		goto err;

	new_rule = strdup(orig_rule);
	deleteNthValueMulti(&del_index, 1, new_rule, ';');

	nvram_bufset(RT2860_NVRAM, tmp, new_rule);
    nvram_commit(RT2860_NVRAM);

	free(new_rule);

	doSystem("qos_run c");

err:

    websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
    websWrite(wp, T("success\n"));
    websDone(wp, 200);
}

static void QoSDeleteRules(webs_t wp, char_t *path, char_t *query)
{
    int i, j, rule_count;
    char_t name_buf[16];
    char_t *value;
    int *deleArray;

    char *rules = nvram_bufget(RT2860_NVRAM, "QoSRules");
    if(!rules || !strlen(rules) )
        return;

    rule_count = getRuleNums(rules);
    if(!rule_count)
        return;

    deleArray = (int *)malloc(rule_count * sizeof(int));

    for(i=0, j=0; i< rule_count; i++){
        snprintf(name_buf, 16, "del_qos_%d", i);
        value = websGetVar(wp, name_buf, NULL);
        if(value){
            deleArray[j++] = i;
        }
    }
    if(!j){
	    free(deleArray);
        websHeader(wp);
        websWrite(wp, T("You didn't select any rules to delete.<br>\n"));
        websFooter(wp);
        websDone(wp, 200);
        return;
    }

    deleteNthValueMulti(deleArray, j, rules, ';');

    nvram_set(RT2860_NVRAM, "QoSRules", rules);
    nvram_commit(RT2860_NVRAM);

	QoSRestart();

    websHeader(wp);
	websWrite(wp, T("Delete Rules:\n") );
	for(i=0; i<j; i++)
		websWrite(wp, "%d ", (deleArray[i] + 1) );

    websFooter(wp);
    websDone(wp, 200);
    free(deleArray);
}


static void QoSLoadDefaultProfile(webs_t wp, char_t *path, char_t *query)
{
    int i;
	for(i=0; i<QOS_PROFILE_ENTRYS_MAX ; i++){
		if(QOS_PROFILE[i].name){
			nvram_bufset(RT2860_NVRAM, QOS_PROFILE[i].name, QOS_PROFILE[i].value);
		}else{
			break;
		}
	}
	nvram_commit(RT2860_NVRAM);
	QoSRestart();

    websHeader(wp);
    websWrite(wp, T("Load default profile successfully.\n"));
    websFooter(wp);
    websDone(wp, 200);
}

static void QoSSetup(webs_t wp, char_t *path, char_t *query)
{
	char_t *qos_enable, *upload_bandwidth, *download_bandwidth, *upload_bandwidth_custom, *download_bandwidth_custom;
	qos_enable = websGetVar(wp, T("QoSSelect"), T(""));
	upload_bandwidth = websGetVar(wp, T("UploadBandwidth"), T(""));
	download_bandwidth = websGetVar(wp, T("DownloadBandwidth"), T(""));
	upload_bandwidth_custom = websGetVar(wp, T("UploadBandwidth_Custom"), T(""));
	download_bandwidth_custom = websGetVar(wp, T("DownloadBandwidth_Custom"), T(""));

	if(!qos_enable || !strlen(qos_enable) || !upload_bandwidth || !download_bandwidth)
		return;

	if(!strcmp(upload_bandwidth, "custom"))
			if(!upload_bandwidth_custom)
				return;
	if(!strcmp(download_bandwidth, "custom"))
			if(!download_bandwidth_custom)
				return;

	if(!strcmp(qos_enable, "1") || !strcmp(qos_enable, "2")){
		if(!strlen(upload_bandwidth)) /* in fact we don't care about the download bandwidth */
			return;
	}

	nvram_bufset(RT2860_NVRAM, "QoSEnable", qos_enable);

	if(!strcmp(qos_enable, "1") || !strcmp(qos_enable, "2")){
		char postfix[16];
		strncpy(postfix, upload_bandwidth_custom, sizeof(postfix));
		if(!strchr(postfix, 'k') && !strchr(postfix, 'K')  && !strchr(postfix, 'm') && !strchr(postfix, 'M') )
			strncat(postfix, "k", sizeof(postfix));
		nvram_bufset(RT2860_NVRAM, "QoSUploadBandwidth_custom", postfix);
		strncpy(postfix, download_bandwidth_custom, sizeof(postfix));
		if(!strchr(postfix, 'k') && !strchr(postfix, 'K')  && !strchr(postfix, 'm') && !strchr(postfix, 'M') )
			strncat(postfix, "k", sizeof(postfix));
		nvram_bufset(RT2860_NVRAM, "QoSDownloadBandwidth_custom", postfix);
		nvram_bufset(RT2860_NVRAM, "QoSUploadBandwidth", upload_bandwidth);
		nvram_bufset(RT2860_NVRAM, "QoSDownloadBandwidth", download_bandwidth);
	}

	nvram_commit(RT2860_NVRAM);

	QoSRestart();

	websHeader(wp);

	websWrite(wp, T("qos_enable: %s<br>\n"), qos_enable);
	websWrite(wp, T("upload bandwidth: %s<br>\n"), upload_bandwidth);
	websWrite(wp, T("download bandwidth: %s<br>\n"), download_bandwidth);
	websFooter(wp);
	websDone(wp, 200);
}

#endif
/*--------------------------------------------

		RL define QoS CGI for NBG-419N

-----------------------------------------------*/
//RL added for EZ mode switch
int QoS_EasyMode(int qos_on)
{

    printf("\nqos_on=%d\n", qos_on);
    ezplib_replace_attr("bw_rule", 0, "enable", qos_on   ? "1" : "0");

   //2010/07/28 Steve modified
   printf("\n/sbin/ezp-bw stop\n");
   system("/sbin/ezp-bw stop");

    if(qos_on){
		printf("\n/sbin/ezp-bw start\n");
    	system("/sbin/ezp-bw start"); 
	}
	printf("nvram_commit\n");
	nvram_commit();  

	return 0;
}

int queueCompare(char *goal, char *src)
{
	int i, prioG, prioS;
	for( i=0; i<QOS_CATEGORY_COUNT ; i++){
		if(!strcmp(goal, defCateName[i]))
			prioG=i;
		if(!strcmp(src, defCateName[i]))
			prioS=i;	}
		if(prioG < prioS )
			return 1;
		else
			return 0;
}

static void SortingEZModeRule()
{
#if 1
	int aprio[QOS_CATEGORY_COUNT];
	int want_app_num[QOS_CATEGORY_COUNT]={0, 4, 5, 6, 7, 10};
	char str_prio[3];
	int i;
	int high_num=0;
	int mid_num=0;
	int low_num=0;
	int num_index=0;
	int sort_aprio[QOS_CATEGORY_COUNT];
	char str_cat[32];
	
	for(i=0; i < QOS_CATEGORY_COUNT; i++){
		ezplib_get_attr_val("bw_app_rule", want_app_num[i], "prio", str_prio, sizeof(str_prio), EZPLIB_USE_CLI);
		aprio[i]= atoi(str_prio);
		if(aprio[i]==1)
			high_num++;
		else if (aprio[i]==2)
			mid_num++;
		else if (aprio[i]==3)
			low_num++;
		printf("aprio[%d]=%d\n",i,aprio[i]);
	}
	printf("\n\nhigh_num=%d\t mid_num=%d\t low_num=%d\n",high_num,mid_num,low_num);
	
	//1
	num_index=0;
	for(i=0; i < QOS_CATEGORY_COUNT; i++){
		if(aprio[i]==1){
		   sort_aprio[num_index]=i;
		   num_index++;
		}
	}
	printf("num_index=%d\n",num_index);
	//2
	for(i=0; i < QOS_CATEGORY_COUNT; i++){
		if(aprio[i]== 2){
		   sort_aprio[num_index]=i;
		   num_index++;
		}
	}
	printf("num_index=%d\n",num_index);
	//3
	for(i=0; i < QOS_CATEGORY_COUNT; i++){
		if(aprio[i]== 3){
		   sort_aprio[num_index]=i;
		   num_index++;
		}
	}
	
	for(i=0; i < QOS_CATEGORY_COUNT; i++){
		   //printf("sort_aprio[%d]=%d \n",i,sort_aprio[i]);
		   printf("sort_aprio[%d]=%d => %s\n",i,sort_aprio[i],defCategory[sort_aprio[i]]);
		   ezplib_replace_attr("bw_ezmodesort_rule", i, "catname", defCategory[sort_aprio[i]]);
		}
	printf("nvram_commit()\n");
	nvram_commit();	
	
#else
	int aprio[QOS_CATEGORY_COUNT];
	char_t tmpstr[20];
	//char_t *ezModeQos;
	//char_t q[QOS_CATEGORY_COUNT][10];
	int want_app_num[QOS_CATEGORY_COUNT]={0, 4, 5, 6, 7, 10};
	char_t *appRule;
	int i,j;
	char_t str_aprio[3];
	char_t ezQ[QOS_CATEGORY_COUNT][8]={"game", "voip", "media", "web", "p2p", "mail" };
	char_t tmp[8]={0};
	int tmp_int;
	char_t new_ezModeQos[128]={0};

	//find out every cateory priority
	for(i=0;i<QOS_CATEGORY_COUNT;i++){
		memset(tmpstr, 0, sizeof(tmpstr));
		sprintf(tmpstr, "QoSAppRules%d", want_app_num[i]);
		appRule = nvram_bufget(RT2860_NVRAM, tmpstr);
		getNthValueSafe(0, appRule, ',', str_aprio, sizeof(str_aprio));
		aprio[i]=atoi(str_aprio);
		//printf("RL aprio[i]=%d\n", aprio[i]);
	}


	//ezModeQos=nvram_bufget(RT2860_NVRAM,"EasyModeSortQoS");
	//for(i=0;i<QOS_CATEGORY_COUNT;i++)
		//getNthValueSafe(i, ezModeQos, ',', q[i], sizeof(q[i]));

	for(i=0;i<QOS_CATEGORY_COUNT-1;i++){
		for(j=i+1; j<QOS_CATEGORY_COUNT; j++){
			if( aprio[j] == aprio[i] ){
				if(queueCompare(ezQ[j],ezQ[i])){
					tmp_int= aprio[j];
					aprio[j] = aprio[i];
					aprio[i] = tmp_int;

					strcpy(tmp, ezQ[j]);
					strcpy(ezQ[j], ezQ[i]);
					strcpy(ezQ[i], tmp);
				}
			}else if( aprio[j] < aprio[i] ){
				tmp_int= aprio[j];
				aprio[j] = aprio[i];
				aprio[i] = tmp_int;

				strcpy(tmp, ezQ[j]);
				strcpy(ezQ[j], ezQ[i]);
				strcpy(ezQ[i], tmp);
			}
		}
	}
	snprintf(new_ezModeQos, sizeof(new_ezModeQos)
		, "%s,%s,%s,%s,%s,%s", ezQ[0], ezQ[1], ezQ[2], ezQ[3], ezQ[4], ezQ[5]);
	//printf("RL debug: new_ezModeQos=%s\n ", new_ezModeQos);

	nvram_bufset(RT2860_NVRAM, "EasyModeSortQoS", new_ezModeQos);
	nvram_commit(RT2860_NVRAM);
#endif
}

int ezModeGameEngine(int game_on)
{


	int i;
	char_t tmpstr[20];
	char_t *aOldRule;
	char_t aNewRule[128];
	char_t *ezModeQos;
	char_t ezModeNewRule[128]={0};
	char_t q[6][10];
	char_t nq[6][10];
	char_t *appRule;

	printf("\ngame_on=%d\n", game_on);
#if 1
	if(game_on == 1){
		// game engine is turn on
		ezplib_replace_attr("bw_rule", 0, "enable", "1");
		//nvram_bufset(RT2860_NVRAM, "RL_QoSEnable",  "1" );
		//nvram_bufset(RT2860_NVRAM, "RL_QoSAutoClassify",  "1" );

		//change Game console rules
		for(i=0;i<4;i++){
			ezplib_replace_attr("bw_app_rule", i, "enable", "1");
			ezplib_replace_attr("bw_app_rule", i, "prio", "1");//high
		}
		
		printf("nvram_commit()\n");
		nvram_commit();
		sleep(1);

		SortingEZModeRule();

	}else{
		// game engine is turn off
		for(i=0;i<4;i++){
			ezplib_replace_attr("bw_app_rule", i, "enable", "0");
			ezplib_replace_attr("bw_app_rule", i, "prio", "2"); //mid

		}
		printf("nvram_commit()\n");
		nvram_commit();
		sleep(1);

		SortingEZModeRule();
	}
	//QoSRestart();
#endif
	return 0;
}

static int QoSDelOneRule(int index)
{
#if 1
	char *nv_rule = "bw_sbm_rule";
	char rule[8192];
	
	snprintf(rule, sizeof(rule), "^0^192.168.1.20^192.168.1.30^tcp^wan0^0^10^Game Console^1^^^^^1");
	ezplib_replace_rule(nv_rule, index, rule);
	printf("nvram_commit\n");
	nvram_commit(); 
#else
	char_t delStr[32]={0};
	sprintf(delStr,"QoSUserRules%d",index);
	nvram_bufset(RT2860_NVRAM, delStr, DEFAULT_QOS_USER_RULE);
	nvram_commit(RT2860_NVRAM);
#endif	
	return 0;
}

static void QoSGeneral(webs_t wp, char_t *path, char_t *query)
{
#if 1
	char_t *qosEnable;
	qosEnable = websGetVar(wp, T("BW_Cfg_Active"), T(""));
	
	ezplib_replace_attr("bw_rule", 0, "enable", atoi(qosEnable) ? "1" : "0");

        //Steve modified 2010/07/28	
	printf("/sbin/ezp-bw stop \n");
	system("/sbin/ezp-bw stop");
	if(atoi(qosEnable)){
		printf("/sbin/ezp-bw start \n");
		system("/sbin/ezp-bw start");
	}
	
	printf("nvram_commit\n");
	nvram_commit();
	
	#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
	#endif
	websRedirect(wp, "local/advance/bm.asp");
#else
	char_t *qosEnable;
	char_t *qosAutoEnable;
	qosEnable = websGetVar(wp, T("BW_Cfg_Active"), T(""));
	qosAutoEnable = websGetVar(wp, T("BW_Auto_Active"), T(""));
	//printf("qosEnable=%s", qosEnable);
	nvram_bufset(RT2860_NVRAM, "RL_QoSEnable",   atoi(qosEnable)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "RL_QoSAutoClassify",   atoi(qosAutoEnable)   ? "1" : "0" );
	nvram_commit(RT2860_NVRAM);

	QoSRestart();

	#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
	#endif

	websRedirect(wp, "local/advance/bm.asp");
#endif
	
}

static void QoSAdvSetup(webs_t wp, char_t *path, char_t *query)
{
#if 1	
	char *rtnFlag, *rtnCurIndex, *rtnAppFlag, *rtnAppIndex;
	char redirStr[50]={0};
	char *aPrio, *apE;
	char *wanBudget, *lanBudget;
	int i;
	char tmpstr1[32], tmpstr2[32], tmpstr3[32], tmpstr4[32];
	char tmp1[3];
	char *ue, *udir, *utext, *uprio;
	char str_bw[10];
	char TempBuf[32];
	int app_index=0;
	
	//Steve add 2010/11/08
	char bw_bw[32];
	
	rtnFlag = websGetVar(wp, T("BW_rtnFlag"), T(""));
	rtnCurIndex = websGetVar(wp, T("BW_rtnCurIndex"), T(""));
	rtnAppFlag = websGetVar(wp, T("BW_rtnAppFlag"), T(""));
	rtnAppIndex = websGetVar(wp, T("BW_rtnAppIndex"), T(""));

	if(!rtnFlag || !rtnCurIndex || !rtnAppFlag || !rtnAppIndex)
		return;
		
	if(!strcmp(rtnAppFlag, "1"))
	{
		printf("rtnAppFlag=%s, Redirect to app edit page!!\n", rtnAppFlag);
		sprintf(redirStr, "local/advance/qos_app_edit.asp?idx=%d",  atoi(rtnAppIndex));
		websRedirect(wp, redirStr);
		return;
	}
	else if(!strcmp(rtnFlag, "1"))
	{
		printf("rtnFlag=%s Redirect to user edit page!!\n", rtnFlag);
		sprintf(redirStr, "local/advance/qos_cfg_edit.asp?idx=%d",  atoi(rtnCurIndex));
		websRedirect(wp, redirStr);
		return;
	}

	if(!strcmp(rtnFlag, "2"))
	{
		printf("Do delete rule No. %d!\n", atoi(rtnCurIndex));
		QoSDelOneRule((atoi(rtnCurIndex))-1);

		#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
		#endif
		websRedirect(wp, "local/advance/qos_adv.asp");
		return;
	}
	
	//
	printf("/sbin/ezp-bw stop \n");
	system("/sbin/ezp-bw stop");	
		
	printf("===	Management Bandwidth ===\n");
	#if 1
	wanBudget = websGetVar(wp, T("UploadBW_Vaule"), T(""));
	lanBudget = websGetVar(wp, T("DownloadBW_Vaule"), T(""));
	#else
	wanBudget = websGetVar(wp, T("UploadBandwidth"), T(""));
	lanBudget = websGetVar(wp, T("DownloadBandwidth"), T(""));
    #endif
    
    printf("wanBudget=%s\n",wanBudget);
    printf("lanBudget=%s\n",lanBudget);
    sprintf(str_bw,"%s", wanBudget);
    ezplib_replace_attr("bw_rule", 0, "bw_upload", str_bw);
    sprintf(str_bw,"%s", lanBudget);
    ezplib_replace_attr("bw_rule", 0, "bw_download", str_bw);
	//nvram_bufset(RT2860_NVRAM, "RL_QoSUploadBw", wanBudget);
	//nvram_bufset(RT2860_NVRAM, "RL_QoSDownloadBw", printf("wanBudget=%s\n",wanBudget););
	
	//app rule;
	printf("===	Application List  ===\n");
	for ( i=0; i<QOS_APPRULE_COUNT; i++ )
	{
		if( i==1 || i==2 || i==3 ){
			aPrio = websGetVar(wp, "appPrio0", T(""));
		}else if( i==8 || i==9){
			//  7,8,9  are the same category
			aPrio = websGetVar(wp, "appPrio7", T(""));
		}else{
			memset(tmpstr1, 0, sizeof(tmpstr1));
			sprintf(tmpstr1, "appPrio%d", i);
			aPrio = websGetVar(wp, tmpstr1, T(""));
		}
		
		memset(tmpstr2, 0, sizeof(tmpstr2));
		sprintf(tmpstr2, "appEnable%d", i);
		apE = websGetVar(wp, tmpstr2, T(""));
		printf("aPrio=%s , apE=%s\n",aPrio,apE);
		sprintf(tmp1,"%s",aPrio);
	    ezplib_replace_attr("bw_app_rule", i, "enable", atoi(apE) ? "1" : "0");
	    ezplib_replace_attr("bw_app_rule", i, "prio", tmp1);
	    
	}
	
	//user rule;
	printf("===	User List  ===\n");
	for ( i=0; i<QOS_USERRULE_COUNT; i++ )
	{
		memset(tmpstr1, 0, sizeof(tmpstr1));
		sprintf(tmpstr1, "userEnable%d", i);
		ue = websGetVar(wp, tmpstr1, T(""));
		memset(tmpstr2, 0, sizeof(tmpstr2));
		sprintf(tmpstr2, "userDir%d", i);
		udir = websGetVar(wp, tmpstr2, T(""));
		memset(tmpstr3, 0, sizeof(tmpstr3));
		sprintf(tmpstr3, "userTextField%d", i);
		utext = websGetVar(wp, tmpstr3, T(""));
		memset(tmpstr4, 0, sizeof(tmpstr4));
		sprintf(tmpstr4, "userPrio%d", i);
		uprio = websGetVar(wp, tmpstr4, T(""));

        #if 0
		printf("i=%d ue=%s\t", i, ue);
		printf("udir=%s\t", udir);
		printf("utext=%s\t", utext);
		printf("uprio=%s\n", uprio);
		#endif
		
		//User-defined Service -> Enable
		ezplib_replace_attr("bw_sbm_rule", i, "enable", atoi(ue) ? "1" : "0");
		
		//User-defined Service -> Direction
		if(!strcmp(udir, "0")){ //To LAN&WLAN
			ezplib_replace_attr("bw_sbm_rule", i, "bw_towan", "0");
			
			ezplib_get_attr_val("bw_sbm_rule", i, "bw_tolan", bw_bw, sizeof(bw_bw), EZPLIB_USE_CLI);
			if(!strcmp(bw_bw, "0")){
				printf("%d : To LAN&WLAN ==> 0 to 10\n",i);
			ezplib_replace_attr("bw_sbm_rule", i, "bw_tolan", "10");
			}else{
				printf("%d : To LAN&WLAN ==> reserved\n",i);
				ezplib_replace_attr("bw_sbm_rule", i, "bw_tolan", bw_bw);
			}
		}else{//To WAN
			ezplib_get_attr_val("bw_sbm_rule", i, "bw_towan", bw_bw, sizeof(bw_bw), EZPLIB_USE_CLI);
			
			if(!strcmp(bw_bw, "0")){
				printf("%d : To WAN ==> 0 to 10\n",i);
			ezplib_replace_attr("bw_sbm_rule", i, "bw_towan", "10");
			}else{
				printf("%d : To WAN ==> reserved\n",i);
				ezplib_replace_attr("bw_sbm_rule", i, "bw_towan", bw_bw);
			}	
			
			ezplib_replace_attr("bw_sbm_rule", i, "bw_tolan", "0");
		}
		
		//User-defined Service -> Service Name
		ezplib_replace_attr("bw_sbm_rule", i, "name", utext);
		
		//User-defined Service -> Category
		ezplib_replace_attr("bw_sbm_rule", i, "apptype", defCategory[atoi(uprio)]);
		
		//User-defined Service -> Prio
		if(atoi(uprio)==0)
		    app_index=0;
		else if (atoi(uprio)==1)
			app_index=4;
		else if (atoi(uprio)==2)
			app_index=5;
		else if (atoi(uprio)==3)
			app_index=6;
		else if (atoi(uprio)==4)
			app_index=7;
		else if (atoi(uprio)==5)
			app_index=10;
		else
			app_index=0;
		ezplib_get_attr_val("bw_app_rule", app_index, "prio", TempBuf, 32, EZPLIB_USE_CLI);	
		printf("app_index=%d prio=%d\n", app_index, atoi(TempBuf));
		ezplib_replace_attr("bw_sbm_rule", i, "prio", TempBuf);
	}
	
	//
	ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);	    
	if(!strcmp(TempBuf, "1")){
		printf("/sbin/ezp-bw start \n");
		system("/sbin/ezp-bw start");
	}
	printf("nvram_commit\n");
	nvram_commit(); 
	
	SortingEZModeRule();
	
	#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
	#endif

	websRedirect(wp, "local/advance/qos_adv.asp");
#else
	char_t *rtnFlag, *rtnCurIndex, *rtnAppFlag, *rtnAppIndex;
	char_t redirStr[50]={0};
	int i,j;
	char_t aRule[128], aRearRule[100], uRule[128], uRearRule[100];
	char_t *aOldRule, *uOldRule;
	char_t paramstr[32], tmpstr1[32], tmpstr2[32], tmpstr3[32], tmpstr4[32];
	char_t *qosEnable;
	char_t *wanBudget, *lanBudget;
	char_t *apE, *aPrio;
	char_t *ue, *udir, *utext, *uprio;
	char *begin, *end, *s_begin;
	int loops;

	rtnFlag = websGetVar(wp, T("BW_rtnFlag"), T(""));
	rtnCurIndex = websGetVar(wp, T("BW_rtnCurIndex"), T(""));
	rtnAppFlag = websGetVar(wp, T("BW_rtnAppFlag"), T(""));
	rtnAppIndex = websGetVar(wp, T("BW_rtnAppIndex"), T(""));

	if(!rtnFlag || !rtnCurIndex || !rtnAppFlag || !rtnAppIndex)
		return;

	if(!strcmp(rtnAppFlag, "1"))
	{
		//printf("rtnAppFlag=%s, Redirect to app edit page!!\n", rtnAppFlag);
		sprintf(redirStr, "local/advance/qos_app_edit.asp?idx=%d",  atoi(rtnAppIndex));
		websRedirect(wp, redirStr);
		return;
	}
	else if(!strcmp(rtnFlag, "1"))
	{
		//printf("rtnFlag=%s Redirect to user edit page!!\n", rtnFlag);
		sprintf(redirStr, "local/advance/qos_cfg_edit.asp?idx=%d",  atoi(rtnCurIndex));
		websRedirect(wp, redirStr);
		return;
	}

	if(!strcmp(rtnFlag, "2"))
	{
		printf("Do delete rule No. %d!\n", atoi(rtnCurIndex));
		QoSDelOneRule((atoi(rtnCurIndex))-1);

		#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
		#endif
		websRedirect(wp, "local/advance/qos_adv.asp");
		return;
	}
	//printf("1\n");

	qosEnable = nvram_bufget(RT2860_NVRAM, "RL_QoSEnable");
	wanBudget = websGetVar(wp, T("UploadBandwidth"), T(""));
	lanBudget = websGetVar(wp, T("DownloadBandwidth"), T(""));
	//wanBudget = websGetVar(wp, T("BW_Cfg_WanBudget"), T(""));
	//lanBudget = websGetVar(wp, T("BW_Cfg_LanBudget"), T(""));
	//printf("RL info WanBudget=%s, lanBudget=%s\n ", wanBudget, lanBudget);

	nvram_bufset(RT2860_NVRAM, "RL_QoSUploadBw", wanBudget);
	nvram_bufset(RT2860_NVRAM, "RL_QoSDownloadBw", lanBudget);
	//app rule;
	//printf("2\n");
	for ( i=0; i<QOS_APPRULE_COUNT; i++ )
	{
		if( i==1 || i==2 || i==3 ){
			aPrio = websGetVar(wp, "appPrio0", T(""));
		}else if( i==8 || i==9){
			//  7,8,9  are the same category
			aPrio = websGetVar(wp, "appPrio7", T(""));
		}else{
			memset(tmpstr1, 0, sizeof(tmpstr1));
			sprintf(tmpstr1, "appPrio%d", i);
			aPrio = websGetVar(wp, tmpstr1, T(""));
		}
		//printf("25\n");

		memset(tmpstr2, 0, sizeof(tmpstr2));
		sprintf(tmpstr2, "appEnable%d", i);
		apE = websGetVar(wp, tmpstr2, T(""));

		sprintf( paramstr, "QoSAppRules%d", i);
		//printf("4\n");
		aOldRule = nvram_bufget(RT2860_NVRAM, paramstr);
		//printf("uOldRule=%s\n", uOldRule);
		j=0;
		begin = aOldRule;
		while(j<2)
		{
    			end = strchr(begin, ',');
			begin = end+1;
			j++;
		}
		strcpy(aRearRule, begin);
		memset(aRule, 0, sizeof(aRule));//, priority,enable
		// if this service enable, we should enable all sub item.
		if(atoi(apE)==1){
			if( i==0 ||i==1 ||i==2 )
				loops=9; //9 common. loops=15;
			else
				loops=3; //loops=6;
			j=0;
			aRearRule[0]='1';
			begin = aRearRule;
			s_begin = aRearRule;
			while(j<loops)
			{
	    			end = strchr(begin, ',');
				begin = end+1;
				j++;
				if((j%3)==0){
					//printf("got it begin=%s\n", begin);
					aRearRule[begin-s_begin]='1';
				}
			}
			//printf("RL debug: aRearRule=%s\n", aRearRule);
		}
		snprintf(aRule, sizeof(aRule), "%d,%d,%s", atoi(aPrio), atoi(apE), aRearRule);
		nvram_bufset(RT2860_NVRAM, paramstr, aRule);
	}
	//user rule
	//printf("3\n");
	memset(paramstr, 0, sizeof(paramstr));

	for ( i=0; i<QOS_USERRULE_COUNT; i++ )
	{
		memset(tmpstr1, 0, sizeof(tmpstr1));
		sprintf(tmpstr1, "userEnable%d", i);
		ue = websGetVar(wp, tmpstr1, T(""));
		memset(tmpstr2, 0, sizeof(tmpstr2));
		sprintf(tmpstr2, "userDir%d", i);
		udir = websGetVar(wp, tmpstr2, T(""));
		memset(tmpstr3, 0, sizeof(tmpstr3));
		sprintf(tmpstr3, "userTextField%d", i);
		utext = websGetVar(wp, tmpstr3, T(""));
		memset(tmpstr4, 0, sizeof(tmpstr4));
		sprintf(tmpstr4, "userPrio%d", i);
		uprio = websGetVar(wp, tmpstr4, T(""));

		sprintf( paramstr, "QoSUserRules%d", i);
		//printf("4\n");
		uOldRule = nvram_bufget(RT2860_NVRAM, paramstr);
		//printf("uOldRule=%s\n", uOldRule);
		j=0;
		begin = uOldRule;
		while(j<4)
		{
    			end = strchr(begin, ',');
			begin = end+1;
			j++;
		}
		strcpy(uRearRule, begin);
		//printf("uRearRule=%s\n", uRearRule);
		//printf("begin=%s\n", begin);
		memset(uRule, 0, sizeof(uRule));
		//enable, direction, text field, priority
		snprintf(uRule, sizeof(uRule), "%d,%d,%s,%d,%s", atoi(ue), atoi(udir), utext, atoi(uprio), uRearRule);
		nvram_bufset(RT2860_NVRAM, paramstr, uRule);
	}

	nvram_commit(RT2860_NVRAM);
	sleep(1);

	SortingEZModeRule();

	QoSRestart();

	#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
	#endif

	websRedirect(wp, "local/advance/qos_adv.asp");
#endif
}

//2009-07-07, RL added,  for gather all effective minimal rate.
static int QoSGatherValidRate(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char bw_up_buf[32];
	char bw_down_buf[32];
	int i;
	int currLan=0;
	int currWan=0;
	char bw_app_enable[3];
	char bw_lan_bw[32];
	char bw_wan_bw[32];
	char bw_app_sub_enable[3];
	char bw_app_sub_min[3];
	
	char bw_user_enable[3];
	char bw_user_min[3];
	
	//APP
	for(i=0 ; i < QOS_APPRULE_COUNT ; i++)
	{
		ezplib_get_attr_val("bw_app_rule", i, "enable", bw_app_enable, sizeof(bw_app_enable), EZPLIB_USE_CLI);
		if(!strcmp(bw_app_enable, "1")){
			if((i==0)||(i==1)||(i==2)||(i==9)){
				//LAN BANDWIDTH UDP
				ezplib_get_attr_val("bw_app_rule", i, "lan_udp_enable", bw_app_sub_enable, sizeof(bw_app_sub_enable), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", i, "lan_udp_min", bw_app_sub_min, sizeof(bw_app_sub_min), EZPLIB_USE_CLI);	
				if((!strcmp(bw_app_sub_enable, "1")) && (!strcmp(bw_app_sub_min, "1"))){
					ezplib_get_attr_val("bw_app_rule", i, "lan_udp_bw", bw_lan_bw, sizeof(bw_lan_bw), EZPLIB_USE_CLI);
					printf("%d => lan_udp_bw=%s\n",i,bw_lan_bw);
					currLan+=atoi(bw_lan_bw);
				}
				
				//WAN BANDWIDTH UDP
				ezplib_get_attr_val("bw_app_rule", i, "wan_udp_enable", bw_app_sub_enable, sizeof(bw_app_sub_enable), EZPLIB_USE_CLI);	
				ezplib_get_attr_val("bw_app_rule", i, "wan_udp_min", bw_app_sub_min, sizeof(bw_app_sub_min), EZPLIB_USE_CLI);
				if((!strcmp(bw_app_sub_enable, "1")) && (!strcmp(bw_app_sub_min, "1"))){
					ezplib_get_attr_val("bw_app_rule", i, "wan_udp_bw", bw_wan_bw, sizeof(bw_wan_bw), EZPLIB_USE_CLI);
					printf("%d => wan_udp_bw=%s\n",i,bw_wan_bw);
					currWan+=atoi(bw_wan_bw);	
				}	
			}		
				//LAN BANDWIDTH TCP
				ezplib_get_attr_val("bw_app_rule", i, "lan_tcp_enable", bw_app_sub_enable, sizeof(bw_app_sub_enable), EZPLIB_USE_CLI);	
				ezplib_get_attr_val("bw_app_rule", i, "lan_tcp_min", bw_app_sub_min, sizeof(bw_app_sub_min), EZPLIB_USE_CLI);
				if((!strcmp(bw_app_sub_enable, "1")) && (!strcmp(bw_app_sub_min, "1"))){
					ezplib_get_attr_val("bw_app_rule", i, "lan_tcp_bw", bw_lan_bw, sizeof(bw_lan_bw), EZPLIB_USE_CLI);
					printf("%d => lan_tcp_bw=%s\n",i,bw_lan_bw);
					currLan+=atoi(bw_lan_bw);
				}
				
				//WAN BANDWIDTH TCP
				ezplib_get_attr_val("bw_app_rule", i, "wan_tcp_enable", bw_app_sub_enable, sizeof(bw_app_sub_enable), EZPLIB_USE_CLI);	
				ezplib_get_attr_val("bw_app_rule", i, "wan_tcp_min", bw_app_sub_min, sizeof(bw_app_sub_min), EZPLIB_USE_CLI);
				if((!strcmp(bw_app_sub_enable, "1")) && (!strcmp(bw_app_sub_min, "1"))){
					ezplib_get_attr_val("bw_app_rule", i, "wan_tcp_bw", bw_wan_bw, sizeof(bw_wan_bw), EZPLIB_USE_CLI);
					printf("%d => wan_tcp_bw=%s\n",i,bw_wan_bw);
					currWan+=atoi(bw_wan_bw);
				}
		}
	}
	printf("currLan=%d\n",currLan);
	printf("currWan=%d\n",currWan);
	
	//User define
	for(i=0 ; i < QOS_APPRULE_COUNT ; i++)
	{
		ezplib_get_attr_val("bw_sbm_rule", i, "enable", bw_user_enable, sizeof(bw_user_enable), EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_sbm_rule", i, "reserve_bw", bw_user_min, sizeof(bw_user_min), EZPLIB_USE_CLI);
		
		if((!strcmp(bw_user_enable, "1")) && (!strcmp(bw_user_min, "1"))) {
			//LAN
			ezplib_get_attr_val("bw_sbm_rule", i, "bw_tolan", bw_lan_bw, sizeof(bw_lan_bw), EZPLIB_USE_CLI);
			printf("%d => bw_lan_bw=%s\n",i,bw_lan_bw);
			currLan+=atoi(bw_lan_bw);
			//WAN
			ezplib_get_attr_val("bw_sbm_rule", i, "bw_towan", bw_wan_bw, sizeof(bw_wan_bw), EZPLIB_USE_CLI);
			printf("%d => bw_wan_bw=%s\n",i,bw_wan_bw);
			currWan+=atoi(bw_wan_bw);
		}
		
	}
	
	
	ezplib_get_attr_val("bw_rule", 0, "bw_upload", bw_up_buf, 32, EZPLIB_USE_CLI);
	ezplib_get_attr_val("bw_rule", 0, "bw_download", bw_down_buf, 32, EZPLIB_USE_CLI);

	websWrite( wp, "var totWanSum=%s; \n", bw_up_buf);
	websWrite( wp, "var totLanSum=%s; \n", bw_down_buf);
	websWrite( wp, "var currLanSum=%d; \n", currLan );	
	websWrite( wp, "var currWanSum=%d; ", currWan );

#else	
	char *appRule, *usrRule;
	char aEnable[3], uEnable[3];
	char_t subEnable[3], subSel[3], subRate[8]; 
	char_t uDir[3];
	char_t tmpStr[20];
	int totLanRate=0, totWanRate=0;
	int loops;
	int i, j, k;
	char_t sRate[8]={0};
	char_t *pRate;
	int iRate;

	for(i=0; i<QOS_APPRULE_COUNT ; i++)
	{
		sprintf(tmpStr, "QoSAppRules%d", i);
		appRule = nvram_bufget(RT2860_NVRAM,  tmpStr);
		getNthValueSafe(1, appRule, ',', aEnable, sizeof(aEnable));
		if(strcmp(aEnable, "1")) //disable, skip...
			continue;
		
		if(i==0 || i ==1 || i==2)
			loops=4; //lan tcp, udp ; wan tcp, udp	
		else
			loops=2;

		k=2;
		
		for(j=0; j<loops ; j++){ 
			if(j > 0)
				k+=3;
			
			getNthValueSafe(k, appRule, ',', subEnable, sizeof(subEnable));
			if(strcmp(subEnable, "1")) //disable, skip...
				continue;
			
			getNthValueSafe(k+1, appRule, ',', subSel, sizeof(subSel));
			if(strcmp(subSel, "1")) //max rate, skip...
				continue;
			
			getNthValueSafe(k+2, appRule, ',', subRate, sizeof(subRate));
			if((i==0 || i ==1 || i==2) && (j==2 || j==3)){ //game, wan
				//printf("RL Goa debug: To Wan rate =%s, i=%d\n", subRate, i);			
				totWanRate+=atoi(subRate);				

			}else if(i !=0 && i !=1 && i !=2 && j==1){ //other, wan
				//printf("RL Goa debug: To Wan rate =%s, i=%d\n", subRate, i);
				totWanRate+=atoi(subRate);				

			}else{ //lan
				//printf("RL Goa debug: To Lan rate =%s, i=%d\n", subRate, i);			
				totLanRate+=atoi(subRate);				

			}			
		}	

	}

	for(i=0; i<QOS_USERRULE_COUNT ; i++)
	{
		sprintf(tmpStr, "QoSUserRules%d", i);
		usrRule = nvram_bufget(RT2860_NVRAM,  tmpStr);
		getNthValueSafe( 0, usrRule, ',', uEnable, sizeof(uEnable));
		if(strcmp(uEnable, "1")) //disable, skip...
			continue;
	
		getNthValueSafe(4, usrRule, ',', subSel, sizeof(subSel));
		if(!strcmp(subSel, "0")) //max rate, skip..
			continue;
		
		getNthValueSafe(5, usrRule, ',', subRate, sizeof(subRate));
		getNthValueSafe(1, usrRule, ',', uDir, sizeof(uDir));
		if(!strcmp(uDir, "1")){ //to wan
			//printf("RL Goa debug: To Wan rate =%s, i=%d\n", subRate, i);	
			totWanRate+=atoi(subRate);
		}else{
			//printf("RL Goa debug: To Lan rate =%s, i=%d\n", subRate, i);			
			totLanRate+=atoi(subRate);			
		}
	}

	//printf("RL Goa debug: total effective LAN rate =%d\n", totLanRate);
	//printf("RL Goa debug: total effective WAN rate =%d\n", totWanRate);	

	strcpy(sRate, nvram_bufget(RT2860_NVRAM, "RL_QoSUploadBw"));
	if(strchr(sRate, 'k')){
		pRate = strtok(sRate, "k");
		iRate=atoi(pRate);
	}else if(strchr(sRate, 'M')){
		pRate = strtok(sRate, "M");
		iRate=(atoi(pRate)*1024);
	}
	websWrite( wp, "var totWanSum=%d; \n", iRate);

	memset(sRate, 0, sizeof(sRate));	
	strcpy(sRate, nvram_bufget(RT2860_NVRAM, "RL_QoSDownloadBw"));
	if(strchr(sRate, 'k')){
		pRate = strtok(sRate, "k");
		iRate=atoi(pRate);
	}else if(strchr(sRate, 'M')){
		pRate = strtok(sRate, "M");
		iRate=(atoi(pRate)*1024);
	}
	websWrite( wp, "var totLanSum=%d; \n", iRate);
	
	websWrite( wp, "var currLanSum=%d; \n", totLanRate );	
	websWrite( wp, "var currWanSum=%d; ", totWanRate );	
#endif
	return 0;	

}

static int QoSGetInfo(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t * name;
	char_t * pbuf;
	char_t *idx;
	int id;
	char TempBuf[32];
	char s_qosEnable[3],s_gameEnable[3],s_gamePrio[3];
	int has_1_enable=0;
	int i;
	
	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	
	//printf("\n\n Steve temp marked\n");
	printf("\nQoSGetInfo -> %s\n", name);
	if( !strcmp(name, "qos_Enable"))
	{
		    ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);	    
			if(!strcmp(TempBuf, "1"))
				websWrite(wp, T("checked"));
			else
			websWrite(wp, T(" "));
			
	}
	else if ( !strcmp(name, "ez_game_eng")){
		//Rule:  if qos enable, then check the priority of game console
		ezplib_get_attr_val("bw_rule", 0, "enable", s_qosEnable, sizeof(s_qosEnable), EZPLIB_USE_CLI);	    	
		ezplib_get_attr_val("bw_app_rule", 0, "prio", s_gamePrio, sizeof(s_gamePrio), EZPLIB_USE_CLI);

		for(i=0;i<4;i++){
			ezplib_get_attr_val("bw_app_rule", i, "enable", s_gameEnable, sizeof(s_gameEnable), EZPLIB_USE_CLI);	    
			if(!strcmp(s_gamePrio, "1")){
				has_1_enable=1;
				break;
			}
		}
		if(!strcmp(s_qosEnable, "1")  && !strcmp(s_gamePrio, "1") && has_1_enable==1 ){
			websWrite(wp, T("checked"));
		}else{
			websWrite(wp, T(" "));
		}
	}
	else if ( !strcmp(name, "RL_QoSUploadBw")){
			ezplib_get_attr_val("bw_rule", 0, "bw_upload", TempBuf, 32, EZPLIB_USE_CLI);
			websWrite(wp, T("%s"), TempBuf);
	}
	else if ( !strcmp(name, "RL_QoSDownloadBw")){
			ezplib_get_attr_val("bw_rule", 0, "bw_download", TempBuf, 32, EZPLIB_USE_CLI);
			websWrite(wp, T("%s"), TempBuf);
	}else if ( !strcmp(name, "qos_service")){ //for app
	
		idx = websGetVar(wp, "idx", T(""));
		if(idx)
			id = atoi(idx);
		
		websWrite( wp, "%s", appQoSService[id-1] );
		
	}else if ( !strcmp(name, "qos_usr_service")){ //for user define
	
		idx = websGetVar(wp, "idx", T(""));
		if(idx)
			id = atoi(idx);
		ezplib_get_attr_val("bw_sbm_rule", id-1, "name", TempBuf, 32, EZPLIB_USE_CLI);
		websWrite( wp, "%s ", TempBuf );		
	}
#if 0	
	char_t * name;
	char_t *qosEnable;
	char_t *appRule, *usrRule;
	char_t app_prio[3];
	char_t tmpStr[20];
	char_t app_enable[3];
	int i;
	int has_1_enable=0;
	char_t *idx;
	int id;
	char_t u_name[25]={0};

	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	//printf("RL: name=%s\n", name);

	if( !strcmp(name, "qos_Enable"))
	{
		qosEnable = nvram_bufget(RT2860_NVRAM, "RL_QoSEnable");
		if(!strcmp(qosEnable, "1"))
			websWrite(wp, T("checked"));
		else
			websWrite(wp, T(" "));
	}else if ( !strcmp(name, "qos_auto_Enable")){
		qosEnable = nvram_bufget(RT2860_NVRAM, "RL_QoSAutoClassify");
		if(!strcmp(qosEnable, "1"))
			websWrite(wp, T("checked"));
		else
			websWrite(wp, T(" "));
	}else if ( !strcmp(name, "ez_game_eng")){
	//Rule:  if qos enable, then check the priority of game console
		qosEnable = nvram_bufget(RT2860_NVRAM, "RL_QoSEnable");
		appRule = nvram_bufget(RT2860_NVRAM,  "QoSAppRules0");
		getNthValueSafe(0, appRule, ',', app_prio, sizeof(app_prio));

		//modified at least 1 enabled 2009/4/1 03:35pm
		for(i=0;i<4;i++){
			sprintf(tmpStr, "QoSAppRules%d", i);
			appRule = nvram_bufget(RT2860_NVRAM, tmpStr);
			getNthValueSafe(1, appRule, ',', app_enable, sizeof(app_enable));
			if(!strcmp(app_enable, "1")){
				has_1_enable=1;
				break;
			}
		}

		if(!strcmp(qosEnable, "1")  && !strcmp(app_prio, "0") && has_1_enable==1 ){
			websWrite(wp, T("checked"));
		}else{
			websWrite(wp, T(" "));
		}
	}else if ( !strcmp(name, "qos_service")){ //for app
	
		idx = websGetVar(wp, "idx", T(""));
		if(idx)
			id = atoi(idx);
		
		websWrite( wp, "%s", appQoSService[id-1] );
		
	}else if ( !strcmp(name, "qos_usr_service")){ //for user define
	
		idx = websGetVar(wp, "idx", T(""));
		if(idx)
			id = atoi(idx);

		sprintf(tmpStr, "QoSUserRules%d", id-1);
		usrRule = nvram_bufget(RT2860_NVRAM,  tmpStr);
		getNthValueSafe(2, usrRule, ',', u_name, sizeof(u_name));

		websWrite( wp, "%s ", u_name );		
	}
#endif

	return 0;
}

static int asp_QoS(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1	
	int i, j;
	int numArr[QOS_APPRULE_COUNT]={1, 1, 1, 1, 2, 3, 4, 5, 6, 6, 6};
	//char_t *appQoSitem[]={"XBox Live","VoIP","FTP", "eMule","BitTorrent", "E-Mail","Instant Messenger ","Web Surfing" };
	char_t *qosAppCategory[]={"Game Console", "Game Console", "Game Console", "Game Console","VoIP", "Instant Messenger","Web Surfing" ,"P2P/FTP", "P2P/FTP", "P2P/FTP", "E-Mail"};

	char *userRule;
	char *appRule;
	char bufStr[20];
	char app_enable[3], app_prio[3],app_cat[32],app_serv[32];
	char u_enable[3], u_dir[3], u_name[25], u_prio[3],u_tmp[32],u_apptype[32];
	int u_app_idx=0;
	
	//Steve add 2010/12/01
	int OpMode=0;
	char TempBuf[32];
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "normal")){
			OpMode=0;
	}else if (!strcmp(TempBuf, "ap")){
			OpMode=1;
	}else if (!strcmp(TempBuf, "ur")){
			OpMode=5;
	}else if (!strcmp(TempBuf, "wisp")){
			OpMode=6;
	}
	

	if(strcmp(argv[0],"app_list")==0)
	{
		for(i=0; i<QOS_APPRULE_COUNT; i++)
		{
			
			#if 1
				//enable
				ezplib_get_attr_val("bw_app_rule", i, "enable", app_enable, 3, EZPLIB_USE_CLI);
				//prio
				ezplib_get_attr_val("bw_app_rule", i, "prio", app_prio, 3, EZPLIB_USE_CLI);
				//cat
				ezplib_get_attr_val("bw_app_rule", i, "cat", app_cat, 32, EZPLIB_USE_CLI);
				//serv
				ezplib_get_attr_val("bw_app_rule", i, "serv", app_serv, 32, EZPLIB_USE_CLI);
			#else
				memset(bufStr, 0, sizeof(bufStr));
				sprintf(bufStr, "QoSAppRules%d", i);
				appRule = nvram_bufget(RT2860_NVRAM, bufStr);
				memset(app_enable, 0, sizeof(app_enable));
				memset(app_prio, 0, sizeof(app_prio));

				getNthValueSafe(0, appRule, ',', app_prio, sizeof(app_prio));
				getNthValueSafe(1, appRule, ',', app_enable, sizeof(app_enable));
			#endif
			if( i==1 || i==2 || i==3  || i==8  || i==9 ){
				websWrite(wp, "<tr><td><center><span class=table_left> </span></center></td>\n" \
							   "<td><center><span class=table_font> </span></center></td>\n" \
							   "<td><center><span class=table_font> </span></center></td>\n" );
			}else{
				websWrite(wp, T("<tr><td><center><span class=table_left>%d</span></center></td>\n"), numArr[i]);

				websWrite(wp, "<td><center><span class=table_font><select name=appPrio%d size=1> \n" \
						  	   "<option value=1 %s id=\"advAppSelectHigh%d\">High</option> \n" \
						  	   "<option value=2 %s id=\"advAppSelectMid%d\">Mid</option>\n" \
						  	   "<option value=3 %s id=\"advAppSelectLow%d\">Low</option>\n" \
						  	   "</select></span></center></td>\n"
						  	   , i, (atoi(app_prio)==1)?"selected":"", i, (atoi(app_prio)==2)?"selected":"", i, (atoi(app_prio)==3)?"selected":"", i );

				//websWrite(wp, T("<td><center><span class=table_font>%s</span></center></td>\n"), qosAppCategory[i]);
				websWrite(wp, T("<td><center><span class=table_font>%s</span></center></td>\n"), app_cat);
			}
			//websWrite(wp, T("<td><span class=table_font><input type=checkbox name=appEnable%d value=1 %s>  %s</span></td>\n")
			//			  , i, (atoi(app_enable)==1)?"checked":"", appQoSService[i]);
			websWrite(wp, T("<td><span class=table_font><input type=checkbox name=appEnable%d value=1 %s>  %s</span></td>\n")
						  , i, (atoi(app_enable)==1)?"checked":"", app_serv);

	               websWrite(wp, "<td><center><span class=table_right>\n" \
	               				  "<a href=\"javascript: doFindAppRule(%d); doEditAppRule(); document.forms[0].submit();\" id=%d \n" \
	               				  "onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('appImage%d','','images/i_edit_on.gif',1)\">\n" \
	               				  "<img src=\"images/i_edit.gif\" name=appImage width=18 height=18 border=0 id=appImage%d  title=Edit /></a></span>\n" \
	               				  "</center></td>\n" \
	               				  "</tr>\n",i+1, i+1, i, i);

		}
	}
	else if(strcmp(argv[0],"user_define")==0)
	{
		for(i=0; i<QOS_USERRULE_COUNT; i++)
		{
			memset(u_enable, 0, sizeof(u_enable));
			memset(u_dir, 0, sizeof(u_dir));
			memset(u_name, 0, sizeof(u_name));
			memset(u_prio, 0, sizeof(u_prio));

			//enable
			ezplib_get_attr_val("bw_sbm_rule", i, "enable", u_enable, sizeof(u_enable), EZPLIB_USE_CLI);
			//name
			ezplib_get_attr_val("bw_sbm_rule", i, "name", u_name, sizeof(u_name), EZPLIB_USE_CLI);
			//dir
			ezplib_get_attr_val("bw_sbm_rule", i, "bw_towan", u_tmp, 32, EZPLIB_USE_CLI);
			if (!strcmp(u_tmp, "0"))
				strcpy(u_dir, "0");
			else
				strcpy(u_dir, "1");
			//apptype
			ezplib_get_attr_val("bw_sbm_rule", i, "apptype", u_apptype, 32, EZPLIB_USE_CLI);	
			
			for(j=0;j<QOS_CATEGORY_COUNT;j++){
			    if (!strcmp(u_apptype,defCategory[j])){
			      u_app_idx=j;
			      break;
			    }
			}
			//printf("\nu_app_idx=%d\n",u_app_idx);
			 
			//prio
			ezplib_get_attr_val("bw_sbm_rule", i, "prio", u_prio, sizeof(u_prio), EZPLIB_USE_CLI);
			
			websWrite(wp, T("<tr ><td><center><span class=table_left>%d</span></center></td>\n"), i+1);

			websWrite(wp, T("<td><center><span class=table_font><input type=checkbox name=userEnable%d value=1 %s></span></center></td>\n")
						   , i, (atoi(u_enable)==1)?"checked":"");
	        websWrite(wp, "<td><center><span class=table_font><select name=userDir%d>\n", i);
	      	
	      	if(OpMode == 6)
	      		websWrite(wp, "<option value=0 %s id=\"advUserToLan%d\">To LAN</option>\n", (atoi(u_dir)==0)?"selected":"", i);
	      	else         	    
	        	websWrite(wp, "<option value=0 %s id=\"advUserToLan%d\">To LAN&WLAN</option>\n", (atoi(u_dir)==0)?"selected":"", i);
	                             	
	        websWrite(wp, "<option value=1 %s id=\"advUserToWan%d\">To WAN</option>\n", (atoi(u_dir)==1)?"selected":"", i);
	               	
	        websWrite(wp, T("<td><center><span class=table_font><input type=text size=12 maxlength=19 name=userTextField%d value=\"%s\"></span></center></td>")
	                			    , i, u_name);
	        websWrite(wp, "<td><center><span class=table_font>\n" \
	               		  "<select name=userPrio%d size=1>\n", i);
	               				 
	       for(j=0;j<QOS_CATEGORY_COUNT;j++)
	           websWrite(wp, "<option value=%d %s >%s</option>\n", j, (u_app_idx==j)?"selected":"", defCategory[j]);

	       websWrite(wp, "</select></span></center></td>\n");

	       websWrite(wp, "<td><center><span class=table_right>\n" \
	               				 "<a href=\"javascript: doFindCurrentRule(%d); doEditRule(); document.forms[0].submit();\" id=%d \n" \
	               				 "onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('userEditImage%d','','images/i_edit_on.gif',1)\">\n" \
	               				 "<img src=\"images/i_edit.gif\" name=userEditImage width=18 height=18 border=0 id=userEditImage%d title=Edit></a>\n"
	               				 , i+1, i+1, i, i );

	               	websWrite(wp, "<a href=\"javascript: doFindCurrentRule(%d); doDeleteRule(); document.forms[0].submit();\" id=%d \n" \
	               				 "onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('userDelImage%d','','images/i_delete_on.gif',1)\"> \n" \
	               				 "<img src=\"images/i_delete.gif\" name=userDelImage width=18 height=18 border=0 id=userDelImage%d title=Remove /></a></span></center></td></tr>\n"
	               				 , i+1, i+1, i, i );
		}
	}
#endif
	return 0;
}
#if 1
static int asp_AppCfgQoS(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char *idx;
	int id, count=0;
	int i, j=2;
	char bufStr[20];
	char *appRule;
	char apE[3], apSel[3], apVal[8];
	idx = websGetVar(wp, "idx", T(""));
	if(idx)
		id = atoi(idx);
	else
		return -1;

	char *op_mode;
	int opm;
	char OPMode[32];
	char TempBuf[32];
	int rule_idx=0;
	
	#if 1
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
		op_mode=OPMode;
		if(op_mode)
			opm=atoi(op_mode);
	#else
		op_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
		if(op_mode)
			opm=atoi(op_mode);
	#endif
	
	websWrite(wp, "<input type=hidden name=cfg_idx value=%d> \n", id);
	rule_idx=id-1;
	printf("\n\n Steve debug idx=%d, rule_idx=%d\n", id, rule_idx);
	
	#if 1
		ezplib_get_attr_val("bw_app_rule", rule_idx, "enable", apE, sizeof(apE), EZPLIB_USE_CLI);
	#else
		sprintf(bufStr, "QoSAppRules%d", (id-1));
		appRule = nvram_bufget(RT2860_NVRAM, bufStr);
		//2009-07-09, RL added,  for checked min rate;
		getNthValueSafe(1, appRule, ',', apE, sizeof(apE));
	#endif
	
	websWrite(wp, "<input type=hidden name=service_e value=%d> \n", atoi(apE));
	
	//if WISP mode, remove WLAN item.
	if(id==1 || id==2 || id==3 || id==10){
		if(opm!=6)
			count=4;//count=6;
		else
			count=4;
	}else{
		if(opm!=6)
			count=2;//count=3
		else
			count=2;
	}

#if 1

	for(i=0; i<count; i++)
	{
		if(count == 4){
			if(i == 0){
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_tcp_enable", apE, sizeof(apE), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_tcp_bw", apVal, sizeof(apVal), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_tcp_min", apSel, sizeof(apSel), EZPLIB_USE_CLI);
				//getNthValueSafe(j, appRule, ',', apE, sizeof(apE));
				//getNthValueSafe(j+1, appRule, ',', apSel, sizeof(apSel));
				//getNthValueSafe(j+2, appRule, ',', apVal, sizeof(apVal));
				//printf("id=%d, no %d, apVal=%s\n", id,  i, apVal);
				//j=j+3;
			}else if(i == 1){
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_udp_enable", apE, sizeof(apE), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_udp_bw", apVal, sizeof(apVal), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_udp_min", apSel, sizeof(apSel), EZPLIB_USE_CLI);
			}else if(i == 2){
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_tcp_enable", apE, sizeof(apE), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_tcp_bw", apVal, sizeof(apVal), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_tcp_min", apSel, sizeof(apSel), EZPLIB_USE_CLI);
			}else if(i == 3){
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_udp_enable", apE, sizeof(apE), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_udp_bw", apVal, sizeof(apVal), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_udp_min", apSel, sizeof(apSel), EZPLIB_USE_CLI);
			}
		}else{
			if(i == 0){
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_tcp_enable", apE, sizeof(apE), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_tcp_bw", apVal, sizeof(apVal), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "lan_tcp_min", apSel, sizeof(apSel), EZPLIB_USE_CLI);
			}else if(i == 1){
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_tcp_enable", apE, sizeof(apE), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_tcp_bw", apVal, sizeof(apVal), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_app_rule", rule_idx, "wan_tcp_min", apSel, sizeof(apSel), EZPLIB_USE_CLI);
			}
		}
		websWrite(wp, T("<tr align=center><td>%d</td>\n"), i+1 );
		websWrite(wp, "<td><input type=checkbox id=\"appEditEnable%d\" name=BW_Class_Active%d value=1 %s></td>\n"
					 , i, i, (atoi(apE)==1)?"checked":"");
		//direction
		if(id==1 || id==2 || id==3 || id==10 ){
			switch(i){
				case 0:
				case 1:
					if(opm == 6)
						websWrite(wp, T("<td>LAN</td>\n"));
					else
						websWrite(wp, T("<td>LAN /WLAN</td>\n"));
					break;
				case 2:
				case 3:
					websWrite(wp, T("<td>WAN</td>\n"));
					break;
				case 4:
				case 5:
					websWrite(wp, T("<td>WLAN</td>\n"));
					break;
			}
		}else {
			switch(i){
				case 0:
					if(opm == 6)
						websWrite(wp, T("<td>LAN</td>\n"));
					else
						websWrite(wp, T("<td>LAN / WLAN</td>\n"));
					break;
				case 1:
					websWrite(wp, T("<td>WAN</td>\n"));
					break;
				case 2:
					websWrite(wp, T("<td>WLAN</td>\n"));
					break;
			}
		}
		//bw class range
		websWrite(wp, "<td><select id=\"appEditSelect%d\" name=BW_Class_Range%d size=1>\n" \
					   "<option value=0 %s id=\"appEditMaxBW%d\">Maximum Bandwidth</option>\n" \
					   "<option value=1 %s id=\"appEditMinBW%d\">Minimum Bandwidth</option></select>\n" \
					   "<input type=text id=\"appEditRate%d\" name=BWClassBudget%d size=6 maxlength=6 value=%d onblur=checkBWBudget(this) >(kbps)</td>\n"
					   , i, i, (atoi(apSel)==0)?"selected":"", i, (atoi(apSel)==1)?"selected":"", i, i, i, atoi(apVal));
		if( id==1 || id==2 || id==3 || id==10){ //game
			switch(i){
				case 0:
				case 1:
				case 4:
				case 5:
					//websWrite(wp, T("<td>-</td><td>3074</td>\n"));
					websWrite(wp, T("<td>-</td><td>-</td>\n"));
					break;
				case 2:
				case 3:
					//websWrite(wp, T("<td>3074</td><td>-</td>\n"));
					websWrite(wp, T("<td>-</td><td>-</td>\n"));
					break;
			}
		}else if(id==11){ //email
			switch(i){
				case 0:
				case 2:
					websWrite(wp, T("<td>-</td><td>110</td>\n"));
					break;
				case 1:
					websWrite(wp, T("<td>25</td><td>-</td>\n"));
					break;
			}
		}else if(id==7){ //www
			switch(i){
				case 0:
				case 2:
					websWrite(wp, T("<td>-</td><td>80, 443</td>\n"));
					break;
				case 1:

					websWrite(wp, T("<td>80, 443</td><td>-</td>\n"));
					break;
			}
		}else
			websWrite(wp, T("<td>-</td><td>-</td>\n"));

		switch(id){
			case 1:
			case 2:
			case 3:
				if((i%2)!=1)
					websWrite(wp, T("<td>TCP</td></tr>\n"));
				else
					websWrite(wp, T("<td>UDP</td></tr>\n"));

				break;
			case 10:
				#if 1
				if((i%2)!=1)
					websWrite(wp, T("<td>TCP</td></tr>\n"));
				else
					websWrite(wp, T("<td>UDP</td></tr>\n"));
				#else
				websWrite(wp, T("<td>TCP/UDP</td></tr>\n"));
				#endif
				break;
			default:
				websWrite(wp, T("<td>TCP</td></tr>\n"));
				break;
		}
	}
#endif	
#endif
	return 0;
}
#else
static int asp_AppCfgQoS(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t *idx;
	int id, count=0;
	int i, j=2;
	char_t bufStr[20];
	char_t *appRule;
	char_t apE[3], apSel[3], apVal[8];
	idx = websGetVar(wp, "idx", T(""));
	if(idx)
		id = atoi(idx);
	else
		return -1;

	char_t *op_mode;
	int opm;
	op_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	if(op_mode)
		opm=atoi(op_mode);

	websWrite(wp, "<input type=hidden name=cfg_idx value=%d> \n", id);
	sprintf(bufStr, "QoSAppRules%d", (id-1));
	appRule = nvram_bufget(RT2860_NVRAM, bufStr);
	//2009-07-09, RL added,  for checked min rate;
	getNthValueSafe(1, appRule, ',', apE, sizeof(apE));
	websWrite(wp, "<input type=hidden name=service_e value=%d> \n", atoi(apE));
	//if WISP mode, remove WLAN item.
	if(id==1 || id==2 || id==3){
		if(opm!=6)
			count=4;//count=6;
		else
			count=4;
	}else{
		if(opm!=6)
			count=2;//count=3
		else
			count=2;
	}

	for(i=0; i<count; i++)
	{
		getNthValueSafe(j, appRule, ',', apE, sizeof(apE));
		getNthValueSafe(j+1, appRule, ',', apSel, sizeof(apSel));
		getNthValueSafe(j+2, appRule, ',', apVal, sizeof(apVal));
		//printf("id=%d, no %d, apVal=%s\n", id,  i, apVal);
		j=j+3;
		websWrite(wp, T("<tr align=center><td>%d</td>\n"), i+1 );
		websWrite(wp, "<td><input type=checkbox id=\"appEditEnable%d\" name=BW_Class_Active%d value=1 %s></td>\n"
					 , i, i, (atoi(apE)==1)?"checked":"");
		//direction
		if(id==1 || id==2 || id==3 ){
			switch(i){
				case 0:
				case 1:
					//websWrite(wp, T("<td>LAN</td>\n"));
					websWrite(wp, T("<td>LAN /WLAN</td>\n"));
					break;
				case 2:
				case 3:
					websWrite(wp, T("<td>WAN</td>\n"));
					break;
				case 4:
				case 5:
					websWrite(wp, T("<td>WLAN</td>\n"));
					break;
			}
		}else {
			switch(i){
				case 0:
					//websWrite(wp, T("<td>LAN</td>\n"));
					websWrite(wp, T("<td>LAN / WLAN</td>\n"));
					break;
				case 1:
					websWrite(wp, T("<td>WAN</td>\n"));
					break;
				case 2:
					websWrite(wp, T("<td>WLAN</td>\n"));
					break;
			}
		}
		//bw class range
		websWrite(wp, "<td><select id=\"appEditSelect%d\" name=BW_Class_Range%d size=1>\n" \
					   "<option value=0 %s id=\"appEditMaxBW%d\">Maximum Bandwidth</option>\n" \
					   "<option value=1 %s id=\"appEditMinBW%d\">Minimum Bandwidth</option></select>\n" \
					   "<input type=text id=\"appEditRate%d\" name=BWClassBudget%d size=6 maxlength=6 value=%d onblur=checkBWBudget(this) >(kbps)</td>\n"
					   , i, i, (atoi(apSel)==0)?"selected":"", i, (atoi(apSel)==1)?"selected":"", i, i, i, atoi(apVal));
		if( id==1 || id==2 || id==3 ){ //game
			switch(i){
				case 0:
				case 1:
				case 4:
				case 5:
					//websWrite(wp, T("<td>-</td><td>3074</td>\n"));
					websWrite(wp, T("<td>-</td><td>-</td>\n"));
					break;
				case 2:
				case 3:
					//websWrite(wp, T("<td>3074</td><td>-</td>\n"));
					websWrite(wp, T("<td>-</td><td>-</td>\n"));
					break;
			}
		}else if(id==11){ //email
			switch(i){
				case 0:
				case 2:
					websWrite(wp, T("<td>-</td><td>110</td>\n"));
					break;
				case 1:
					websWrite(wp, T("<td>25</td><td>-</td>\n"));
					break;
			}
		}else if(id==7){ //www
			switch(i){
				case 0:
				case 2:
					websWrite(wp, T("<td>-</td><td>80, 443</td>\n"));
					break;
				case 1:

					websWrite(wp, T("<td>80, 443</td><td>-</td>\n"));
					break;
			}
		}else
			websWrite(wp, T("<td>-</td><td>-</td>\n"));

		switch(id){
			case 1:
			case 2:
			case 3:
				if((i%2)!=1)
					websWrite(wp, T("<td>TCP</td></tr>\n"));
				else
					websWrite(wp, T("<td>UDP</td></tr>\n"));

				break;
			case 10:
				websWrite(wp, T("<td>TCP/UDP</td></tr>\n"));
				break;
			default:
				websWrite(wp, T("<td>TCP</td></tr>\n"));
				break;
		}
	}

	return 0;
}
#endif

static void QoSAppCfg(webs_t wp, char_t *path, char_t *query)
{
#if 1
	int id;
	char *butFlag = websGetVar(wp, "BW_ButtonFlag", T(""));
	int i,count;
	char_t paramStr[32], tmpStr1[32], tmpStr2[32], tmpStr3[32];
	char_t *appE, *aRange, *aVal;
	int rule_idx;
	char str_bw[10];
	char TempBuf[32]={0};
	
	if(butFlag && (atoi(butFlag)==2)){
		websRedirect(wp, "local/advance/qos_adv.asp");
		return;
	}
	char *idx = websGetVar(wp, "cfg_idx", T(""));
	if(idx)
		id = atoi(idx);
	else
		return -1;
	
	rule_idx = id -1;
	printf("\nid=%d butFlag=%s rule_idx=%d\n",id,butFlag,rule_idx);	
	
	if(id==1 || id==2 || id==3 || id==10)
		count=4;//count=6;
	else
		count=2;//count=3;	
	
	for(i=0; i<count; i++)
	{
			//Enable
			memset(tmpStr1, 0, sizeof(tmpStr1));
			sprintf(tmpStr1, "BW_Class_Active%d", i);
			appE = websGetVar(wp, tmpStr1, T(""));
			printf("\ni=%d appE=%s\t",i,appE);
			
			//
			memset(tmpStr2, 0, sizeof(tmpStr2));
			sprintf(tmpStr2, "BW_Class_Range%d", i);
			aRange = websGetVar(wp, tmpStr2, T(""));
			printf("aRange=%s\t",aRange);
			
			//Bandwidth
			memset(tmpStr3, 0, sizeof(tmpStr3));
			sprintf(tmpStr3, "BWClassBudget%d", i);
			aVal = websGetVar(wp, tmpStr3, T(""));
			printf("aVal=%s\n",aVal);
			sprintf( str_bw, "%s", aVal);
		if(count == 4){    
		    if(i==0){
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_tcp_enable", atoi(appE) ? "1" : "0");
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_tcp_bw", str_bw);
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_tcp_min", atoi(aRange) ? "1" : "0");
			}else if(i==1){
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_udp_enable", atoi(appE) ? "1" : "0");
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_udp_bw", str_bw);
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_udp_min", atoi(aRange) ? "1" : "0");
			}else if(i==2){
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_tcp_enable", atoi(appE) ? "1" : "0");
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_tcp_bw", str_bw);
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_tcp_min", atoi(aRange) ? "1" : "0");
			}else if(i==3){
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_udp_enable", atoi(appE) ? "1" : "0");	
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_udp_bw", str_bw);
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_udp_min", atoi(aRange) ? "1" : "0");
			}
		}else{
			if(i==0){
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_tcp_enable", atoi(appE) ? "1" : "0");
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_tcp_bw", str_bw);
				ezplib_replace_attr("bw_app_rule", rule_idx, "lan_tcp_min", atoi(aRange) ? "1" : "0");
			}else if(i==1){
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_tcp_enable", atoi(appE) ? "1" : "0");
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_tcp_bw", str_bw);
				ezplib_replace_attr("bw_app_rule", rule_idx, "wan_tcp_min", atoi(aRange) ? "1" : "0");
			}
		}
	}	

    printf("/sbin/ezp-bw stop \n");
	system("/sbin/ezp-bw stop");
    ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);	    
	if(!strcmp(TempBuf, "1")){
		printf("/sbin/ezp-bw start \n");
		system("/sbin/ezp-bw start");
	}
    
    printf("nvram_commit\n");
	nvram_commit();  		
	
	#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
	#endif

	websRedirect(wp, "local/advance/qos_adv.asp");

#else	
	char_t aE[3], aPrio[3];
	int id, i, count;
	int j=0;
	char_t paramStr[32], tmpStr1[32], tmpStr2[32], tmpStr3[32];
	char_t aRule[128];
	char_t *olduRule;
	char_t *appE, *aRange, *aVal;
	int ar[18]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	char_t *butFlag = websGetVar(wp, "BW_ButtonFlag", T(""));
	if(butFlag && (atoi(butFlag)==2)){
		websRedirect(wp, "local/advance/qos_adv.asp");
		return;
	}
	char_t *idx = websGetVar(wp, "cfg_idx", T(""));
	if(idx)
		id = atoi(idx);
	else
		return -1;

	sprintf(paramStr, "QoSAppRules%d", (id-1));
	olduRule = nvram_bufget(RT2860_NVRAM, paramStr);

	getNthValueSafe(0, olduRule, ',', aPrio, sizeof(aPrio));
	getNthValueSafe(1, olduRule, ',', aE, sizeof(aE));

	if(id==1 || id==2 || id==3)
		count=4;//count=6;
	else
		count=2;//count=3;

	for(i=0; i<count; i++)
	{
		memset(tmpStr1, 0, sizeof(tmpStr1));
		sprintf(tmpStr1, "BW_Class_Active%d", i);
		appE = websGetVar(wp, tmpStr1, T(""));
		ar[j]=atoi(appE);
		memset(tmpStr2, 0, sizeof(tmpStr2));
		sprintf(tmpStr2, "BW_Class_Range%d", i);
		aRange = websGetVar(wp, tmpStr2, T(""));
		ar[j+1]=atoi(aRange);
		memset(tmpStr3, 0, sizeof(tmpStr3));
		sprintf(tmpStr3, "BWClassBudget%d", i);
		aVal = websGetVar(wp, tmpStr3, T(""));
		ar[j+2]=atoi(aVal);
		j=j+3;
	}

	memset(aRule, 0, sizeof(aRule));//enable, priority
	if(id==1 || id==2 || id==3)
		snprintf(aRule, sizeof(aRule), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", atoi(aPrio), atoi(aE), ar[0], ar[1], ar[2], ar[3], ar[4], ar[5], ar[6], ar[7], ar[8], ar[9], ar[10], ar[11] );
		//snprintf(aRule, sizeof(aRule), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", atoi(aPrio), atoi(aE), ar[0], ar[1], ar[2], ar[3], ar[4], ar[5], ar[6], ar[7], ar[8], ar[9], ar[10], ar[11], ar[12], ar[13], ar[14], ar[15], ar[16], ar[17]);
	else
		snprintf(aRule, sizeof(aRule), "%d,%d,%d,%d,%d,%d,%d,%d", atoi(aPrio), atoi(aE), ar[0], ar[1], ar[2], ar[3], ar[4], ar[5]);
		//snprintf(aRule, sizeof(aRule), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", atoi(aPrio), atoi(aE), ar[0], ar[1], ar[2], ar[3], ar[4], ar[5], ar[6], ar[7], ar[8]);

	nvram_bufset(RT2860_NVRAM, paramStr, aRule);
	nvram_commit(RT2860_NVRAM);

	QoSRestart();

	#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
	#endif

	websRedirect(wp, "local/advance/qos_adv.asp");
#endif	
}

static int asp_UsrCfgQoS(int eid, webs_t wp, int argc, char_t **argv)
{	
	char_t *idx;
	char_t bufStr[20]={0};
	char_t *usrRule;
	char_t *uE[3], uDir[3];
	char_t uSel[8], uVal[8], uDstAddr[20], uDstMask[20], uDstPort[8];
	char_t  uSrcAddr[20], uSrcMask[20], uSrcPort[8], uProto[8], uProVal[8];
	char_t u_tmp[32];

	idx = websGetVar(wp, "idx", T(""));
	int rule_idx=0;
	char_t uDstAddr2[20],uSrcAddr2[20];
	
	#if 1
		printf("idx=%d\n",atoi(idx));
		rule_idx=atoi(idx)-1;
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "enable", uE, sizeof(uE), EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "reserve_bw", uSel, sizeof(uSel), EZPLIB_USE_CLI);
		
		
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "bw_towan", u_tmp, 32, EZPLIB_USE_CLI);
		if (!strcmp(u_tmp, "0")){
				strcpy(uDir, "0");//To LAN/WLAN
				ezplib_get_attr_val("bw_sbm_rule", rule_idx, "bw_tolan", uVal, sizeof(uVal), EZPLIB_USE_CLI);
		}else{
				strcpy(uDir, "1");//To WAN
				ezplib_get_attr_val("bw_sbm_rule", rule_idx, "bw_towan", uVal, sizeof(uVal), EZPLIB_USE_CLI);
		}
		
		//Destination Address         
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "inipaddr_start", uSrcAddr, sizeof(uSrcAddr), EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "inipaddr_end", uSrcAddr2, sizeof(uSrcAddr2), EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "inport", uSrcPort, sizeof(uSrcPort), EZPLIB_USE_CLI);
		//Source Address
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "outipaddr_start", uDstAddr, sizeof(uDstAddr), EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "outipaddr_end", uDstAddr2, sizeof(uDstAddr2), EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "outport", uDstPort, sizeof(uDstPort), EZPLIB_USE_CLI);
		
		//Proto
		ezplib_get_attr_val("bw_sbm_rule", rule_idx, "proto", uProto, sizeof(uProto), EZPLIB_USE_CLI);
		
		#if 1
		printf("uDstAddr=%s\n", uDstAddr);
		printf("uDstAddr2%s\n", uDstAddr2);
		printf("uDstPort=%s\n", uDstPort);
		printf("uSrcAddr=%s\n", uSrcAddr);
		printf("uSrcAddr2=%s\n", uSrcAddr2);
		printf("uSrcPort=%s\n", uSrcPort);
		
		#endif
		
		websWrite(wp, "<input type=hidden name=cfg_idx value=%d> \n", atoi(idx));
		websWrite(wp, "<input type=hidden name=service_e value=%d> \n", atoi(uE));
		websWrite(wp, "<input type=hidden name=service_dire value=%d> \n", atoi(uDir));
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditBW\">BW Budget</td>\n" \
				  "<td><select id=\"userEditSelect\" name=BW_Cfg_Range size=1>\n" \
				  "<option value=0 %s id=\"userEditMaxBW\">Maximum Bandwidth</option>\n" \
				  "<option value=1 %s id=\"userEditMinBW\">Minimum Bandwidth</option></select>\n" \
				  "<input type=text id=\"userEditRate\" name=BWCCBudget size=6 maxlength=6 value=%d  onblur=chk_wzBWWANBudget(this.value) >(kbps)</td></tr>\n"
				  , (atoi(uSel)==0)?"selected":"",  (atoi(uSel)==1)?"selected":"", atoi(uVal));
		if(!strlen(uDstAddr)){
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditDstAddr1\">Destination Address Start</td>\n" \
				  "<td colspan=2><input type=text name=BWDestAddr size=15 maxlength=15 value=%s >\n" \
				  "</td></tr> \n", "0.0.0.0");
		}else{
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditDstAddr1\">Destination Address Start</td>\n" \
				  "<td colspan=2><input type=text name=BWDestAddr size=15 maxlength=15 value=%s >\n" \
				  "</td></tr> \n", uDstAddr);
		}
		if(!strlen(uDstAddr2)){
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditDstAddr2\">Destination Address End</td>\n" \
				  "<td colspan=2><input type=text name=BWDestNetmask size=15 maxlength=15 value=%s >\n" \
				  "</td></tr> \n", "0.0.0.0");
		}else{
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditDstAddr2\">Destination Address End</td>\n" \
				  "<td colspan=2><input type=text name=BWDestNetmask size=15 maxlength=15 value=%s >\n" \
				  "</td></tr> \n", uDstAddr2);
		}
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditDstPort\">Destination Port</td>\n" \
				  "<td colspan=2><input type=text name=BWDestPort size=5 maxlength=5 value=%d  onblur=checkBWPort(this) >\n" \
				  "</td></tr> \n", atoi(uDstPort));
		if(!strlen(uSrcAddr)){
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditSrcAddr1\">Source Address Start</td>\n" \
				  "<td colspan=2><input type=text name=BWSrcAddr size=15 maxlength=15 value=%s >\n" \
				  "</td></tr> \n", "0.0.0.0");
		}else{
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditSrcAddr1\">Source Address Start</td>\n" \
				  "<td colspan=2><input type=text name=BWSrcAddr size=15 maxlength=15 value=%s >\n" \
				  "</td></tr> \n", uSrcAddr);
		}
		if(!strlen(uSrcAddr2)){
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditSrcAddr2\">Source Address End</td>\n" \
				  "<td colspan=2><input type=text name=BWSrcNetmask size=15 maxlength=15 value=%s >\n" \
				  "</td></tr> \n", "0.0.0.0");
		}else{
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditSrcAddr2\">Source Address End</td>\n" \
				  "<td colspan=2><input type=text name=BWSrcNetmask size=15 maxlength=15 value=%s >\n" \
				  "</td></tr> \n", uSrcAddr2);
		
		}
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditSrcPort\">Source Port</td>\n" \
				  "<td colspan=2><input type=text name=BWSrcPort size=5 maxlength=5 value=%d onblur=checkBWPort(this) >\n" \
				  "</td></tr> \n", atoi(uSrcPort));
		websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditProto\">Protocol</td>\n" \
				  "<td colspan=2><select name=BWCCfgProtocolSel size=1 onChange=\"doBWProtocol();\">\n" \
				  "<option value=tcp %s>TCP</option>\n" \
				  "<option value=udp %s>UDP</option>\n" \
				  "<option value=both %s>BOTH</option>\n" \
				  "</select>&nbsp;&nbsp;\n" \
				  "</td></tr>\n"
				  ,  (strcmp(uProto,"tcp")==0)?"selected":"",  (strcmp(uProto,"udp")==0)?"selected":"",  (strcmp(uProto,"both")==0)?"selected":"");
	#else
	sprintf(bufStr, "QoSUserRules%d", (atoi(idx)-1));
	usrRule = nvram_bufget(RT2860_NVRAM, bufStr);
	//printf("usrRule=%s\n", usrRule);
	getNthValueSafe(0, usrRule, ',', uE, sizeof(uE));
	getNthValueSafe(1, usrRule, ',', uDir, sizeof(uDir));
	getNthValueSafe(4, usrRule, ',', uSel, sizeof(uSel));
	getNthValueSafe(5, usrRule, ',', uVal, sizeof(uVal));
	getNthValueSafe(6, usrRule, ',', uDstAddr, sizeof(uDstAddr));
	getNthValueSafe(7, usrRule, ',', uDstMask, sizeof(uDstMask));
	getNthValueSafe(8, usrRule, ',', uDstPort, sizeof(uDstPort));
	getNthValueSafe(9, usrRule, ',', uSrcAddr, sizeof(uSrcAddr));
	getNthValueSafe(10, usrRule, ',', uSrcMask, sizeof(uSrcMask));
	getNthValueSafe(11, usrRule, ',', uSrcPort, sizeof(uSrcPort));
	getNthValueSafe(12, usrRule, ',', uProto, sizeof(uProto));
	getNthValueSafe(13, usrRule, ',', uProVal, sizeof(uProVal));
	

	websWrite(wp, "<input type=hidden name=cfg_idx value=%d> \n", atoi(idx));
	websWrite(wp, "<input type=hidden name=service_e value=%d> \n", atoi(uE));
	websWrite(wp, "<input type=hidden name=service_dire value=%d> \n", atoi(uDir));
	websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditBW\">BW Budget</td>\n" \
				  "<td><select id=\"userEditSelect\" name=BW_Cfg_Range size=1>\n" \
				  "<option value=0 %s id=\"userEditMaxBW\">Maximum Bandwidth</option>\n" \
				  "<option value=1 %s id=\"userEditMinBW\">Minimum Bandwidth</option></select>\n" \
				  "<input type=text id=\"userEditRate\" name=BWCCBudget size=6 maxlength=6 value=%d  onblur=chk_wzBWWANBudget(this.value) >(kbps)</td></tr>\n"
				  , (atoi(uSel)==0)?"selected":"",  (atoi(uSel)==1)?"selected":"", atoi(uVal));
	websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditDstAddr\">Destination Address</td>\n" \
				  "<td colspan=2><input type=text name=BWDestAddr size=15 maxlength=15 value=%s onBlur=\"getDefaultNetmask(BWDestAddr,BWDestNetmask);\">\n" \
				  "</td></tr> \n", uDstAddr);
	websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditDstMask\">Destination Subnet Netmask</td>\n" \
				  "<td colspan=2><input type=text name=BWDestNetmask size=15 maxlength=15 value=%s onBlur=\"SubnetCheck( BWDestAddr, this);\">\n" \
				  "</td></tr> \n", uDstMask);
	websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditDstPort\">Destination Port</td>\n" \
				  "<td colspan=2><input type=text name=BWDestPort size=5 maxlength=5 value=%d  onblur=checkBWPort(this) >\n" \
				  "</td></tr> \n", atoi(uDstPort));
	websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditSrcAddr\">Source Address</td>\n" \
				  "<td colspan=2><input type=text name=BWSrcAddr size=15 maxlength=15 value=%s onBlur=\"getDefaultNetmask(BWSrcAddr,BWSrcNetmask);\">\n" \
				  "</td></tr> \n", uSrcAddr);
	websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditSrcMask\">Source Subnet Netmask</td>\n" \
				  "<td colspan=2><input type=text name=BWSrcNetmask size=15 maxlength=15 value=%s onBlur=\"SubnetCheck( BWSrcAddr, this);\">\n" \
				  "</td></tr> \n", uSrcMask);
	websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditSrcPort\">Source Port</td>\n" \
				  "<td colspan=2><input type=text name=BWSrcPort size=5 maxlength=5 value=%d onblur=checkBWPort(this) >\n" \
				  "</td></tr> \n", atoi(uSrcPort));
	websWrite(wp, "<tr> \n" \
				  "<td width=40%% id=\"userEditProto\">Protocol</td>\n" \
				  "<td colspan=2><select name=BWCCfgProtocolSel size=1 onChange=\"doBWProtocol();\">\n" \
				  "<option value=0 %s>TCP</option>\n" \
				  "<option value=1 %s>UDP</option>\n" \
				  "<option value=2 %s id=\"userEditProtoDefine\">User defined</option>\n" \
				  "</select>&nbsp;&nbsp;\n" \
				  "<input type=text name=BWCCfgProtocol size=5 maxlength=5 value=%d onBlur=\"checkProtoVal(this)\" ></td></tr>\n"
				  ,  (atoi(uProto)==0)?"selected":"",  (atoi(uProto)==1)?"selected":"", (atoi(uProto)==2)?"selected":"", atoi(uProVal));
	#endif
	return 0;
}

static void QoSUserCfg(webs_t wp, char_t *path, char_t *query)
{
	printf("\nQoSUserCfg\n");
	
	char_t uE[3], uDir[3], uName[25], uPrio[3];
	char_t *uSel, *uVal, *uDstAddr, *uDstMask, *uDstPort;
	char_t *uSrcAddr, *uSrcMask, *uSrcPort, *uProto, *uProVal;
	char_t *butFlag;
	char_t *olduRule;
	char_t paramStr[20], uRule[128];
	
	int rule_idx,id;
	char_t str_val[32];
	char_t u_tmp[32];
	char TempBuf[32]={0};

	butFlag = websGetVar(wp, "BW_ButtonFlag", T(""));
	if(butFlag && (atoi(butFlag)==2)){
		websRedirect(wp, "local/advance/qos_adv.asp");
		return;
	}
	char_t *idx = websGetVar(wp, "cfg_idx", T(""));
	
	id = atoi(idx);
	rule_idx = id -1;
	printf("\nid=%d butFlag=%s rule_idx=%d\n",id,butFlag,rule_idx);	
	
	//sprintf(paramStr, "QoSUserRules%d", (atoi(idx)-1));
	//olduRule = nvram_bufget(RT2860_NVRAM, paramStr);

	//getNthValueSafe(0, olduRule, ',', uE, sizeof(uE));
	//getNthValueSafe(1, olduRule, ',', uDir, sizeof(uDir));
	//getNthValueSafe(2, olduRule, ',', uName, sizeof(uName));
	//getNthValueSafe(3, olduRule, ',', uPrio, sizeof(uPrio));

	uSel = websGetVar(wp, "BW_Cfg_Range", T(""));
	uVal = websGetVar(wp, "BWCCBudget", T(""));
	uDstAddr = websGetVar(wp, "BWDestAddr", T(""));
	uDstMask = websGetVar(wp, "BWDestNetmask", T(""));
	uDstPort = websGetVar(wp, "BWDestPort", T(""));
	uSrcAddr = websGetVar(wp, "BWSrcAddr", T(""));
	uSrcMask = websGetVar(wp, "BWSrcNetmask", T(""));
	uSrcPort = websGetVar(wp, "BWSrcPort", T(""));
	uProto = websGetVar(wp, "BWCCfgProtocolSel", T(""));
	uProVal = websGetVar(wp, "BWCCfgProtocol", T(""));
	printf("uSel=%s\n", uSel);
	printf("uVal=%s\n", uVal);
	printf("uDstAddr=%s\n", uDstAddr);
	printf("uDstMask=%s\n", uDstMask);
	printf("uDstPort=%s\n", uDstPort);
	printf("uSrcAddr=%s\n", uSrcAddr);
	printf("uSrcMask=%s\n", uSrcMask);
	printf("uSrcPort=%s\n", uSrcPort);
	printf("uProto=%s\n", uProto);
	//printf("uProVal=%s\n", uProVal);
	

	ezplib_replace_attr("bw_sbm_rule", rule_idx, "reserve_bw", atoi(uSel) ? "1" : "0");
	
	//band width
	ezplib_get_attr_val("bw_sbm_rule", rule_idx, "bw_towan", u_tmp, 32, EZPLIB_USE_CLI);
	sprintf(str_val,"%s",uVal);
	if (!strcmp(u_tmp, "0")){
		//To LAN
		ezplib_replace_attr("bw_sbm_rule", rule_idx, "bw_tolan", str_val);
	}else{
		//To WAN
		ezplib_replace_attr("bw_sbm_rule", rule_idx, "bw_towan", str_val);
	}
	
	//uDstAddr
	if (!strcmp(uDstAddr, "0.0.0.0"))
		sprintf(str_val,"%s","");
	else
		sprintf(str_val,"%s",uDstAddr);		
	//ezplib_replace_attr("bw_sbm_rule", rule_idx, "inipaddr_start", str_val);
	ezplib_replace_attr("bw_sbm_rule", rule_idx, "outipaddr_start", str_val);
	
	//uDstMask temp
	if (!strcmp(uDstMask, "0.0.0.0"))
		sprintf(str_val,"%s","");
	else
		sprintf(str_val,"%s",uDstMask);
	//ezplib_replace_attr("bw_sbm_rule", rule_idx, "inipaddr_end", str_val);
	ezplib_replace_attr("bw_sbm_rule", rule_idx, "outipaddr_end", str_val);
	
	//uDstPort
	if (!strcmp(uDstPort, "0")){
		ezplib_replace_attr("bw_sbm_rule", rule_idx, "outport", "");
	}else{
		sprintf(str_val,"%s",uDstPort);
		ezplib_replace_attr("bw_sbm_rule", rule_idx, "outport", str_val);
	}
	//uSrcAddr
	if (!strcmp(uSrcAddr, "0.0.0.0"))
		sprintf(str_val,"%s","");
	else
		sprintf(str_val,"%s",uSrcAddr);
	//ezplib_replace_attr("bw_sbm_rule", rule_idx, "outipaddr_start", str_val);
	ezplib_replace_attr("bw_sbm_rule", rule_idx, "inipaddr_start", str_val);
	
	//uSrcMask temp
	if (!strcmp(uSrcMask, "0.0.0.0"))
		sprintf(str_val,"%s","");
	else
		sprintf(str_val,"%s",uSrcMask);
	//ezplib_replace_attr("bw_sbm_rule", rule_idx, "outipaddr_end", str_val);
	ezplib_replace_attr("bw_sbm_rule", rule_idx, "inipaddr_end", str_val);
	
	//uSrcPort
	if (!strcmp(uSrcPort, "0")){
		ezplib_replace_attr("bw_sbm_rule", rule_idx, "inport", "");
	}else{
		sprintf(str_val,"%s",uSrcPort);
		ezplib_replace_attr("bw_sbm_rule", rule_idx, "inport", str_val);
	}
	
	//uProto
	sprintf(str_val,"%s",uProto);
	ezplib_replace_attr("bw_sbm_rule", rule_idx, "proto", str_val);
	
	//if(!isIpNetmaskValid(uDstAddr))
	//	return;
	//if(!isIpNetmaskValid(uSrcAddr))
	//	return;

	//memset(uRule, 0, sizeof(uRule));
	//snprintf(uRule, sizeof(uRule), "%d,%d,%s,%d,%d,%d,%s,%s,%d,%s,%s,%d,%d,%d",atoi(uE),atoi(uDir),uName,atoi(uPrio),atoi(uSel),atoi(uVal),uDstAddr,uDstMask,atoi(uDstPort),uSrcAddr,uSrcMask,atoi(uSrcPort),atoi(uProto),atoi(uProVal));
	//nvram_bufset(RT2860_NVRAM, paramStr, uRule);

	//nvram_commit(RT2860_NVRAM);
	//QoSRestart();
	
	printf("/sbin/ezp-bw stop \n");
	system("/sbin/ezp-bw stop");
    ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);	    
	if(!strcmp(TempBuf, "1")){
		printf("/sbin/ezp-bw start \n");
		system("/sbin/ezp-bw start");
	}
	
	printf("nvram_commit\n");
	nvram_commit(); 

	#if 1//Arthur Chow 2009-03-06
		setWebMessage(0, NULL);
	#endif

	websRedirect(wp, "local/advance/qos_adv.asp");
}

static int doWriteMonitor_4615(webs_t wp, char *type, char *serviceName, int diretion)
{
	FILE *fp1;
	char cmdBuf[200]={0};
	char rateStr[10]={0}, tmpStr[10]={0};
	int rate=0;
	int ratePercent=0;
	int i,k=0;

	sprintf(cmdBuf, "cat /tmp/appmarkflow | grep \"%s|%s\" | cut -f%d -d \"|\" >/tmp/class_rate", serviceName, type, diretion);
	system(cmdBuf);

	if(!(fp1=fopen("/tmp/class_rate", "r")))
	{
		printf("Can not open class rate file !!\n");
		fclose(fp1);
		return 1;
	}else{
		fgets(rateStr, 10, fp1);
		//printf("rateStr=%s\n", rateStr);
		rate=atoi(rateStr);
		//printf("rate=%d\n", rate);
	}
	fclose(fp1);
	if(rate < 128){
		rate=0;
		ratePercent=0;
	}
	else{
                //Steve patch 2010/10/29
		rate=(rate)/1024;
		//rate=(rate*8)/1024;
		ratePercent=(rate*100/UPLINLK_SPEED);
	}
/*
	for(i=0;i<strlen(rateStr);i++){
		if(rateStr[i]=='K')
			k=1;
	}
	if(k==1){
		strncpy(tmpStr, rateStr, strlen(rateStr)-1);
		rate=atoi(tmpStr);
		ratePercent=(rate*100/UPLINLK_SPEED);
	}else{
		strncpy(tmpStr, rateStr, strlen(rateStr));
		rate=atoi(tmpStr);
		if(rate < 1024){ //less than 1024, ingore it.
			rate=0;
			ratePercent=0;
		}else{
			rate= (rate/1024);
			ratePercent=(rate*100/UPLINLK_SPEED);
		}
	}
*/
	
	websWrite(wp, "<table width=90%% border=0 align=center cellspacing=0 class=wtext_3> \n" \
				  "<tr><td><table width=100%% border=0 cellspacing=0 cellpadding=0> \n" \
				  "<tr><td width=30%% align=center>%s</td> \n", serviceName);

	websWrite(wp, "<td height=10> \n" \
				  "<table width=100%% border=1 cellpadding=0 cellspacing=0  bordercolor=\"#000000\" ><tr> \n" \
				  "<td height=20 background=\"images/i_percentage_center.gif\"> \n" \
				  "<table border=0 cellspacing=0 cellpadding=0 width=%d%%> \n", ratePercent);

	websWrite(wp, "<tr><td background=\"images/i_percentage_blue.gif\"  height=20></td> \n" \
				  "</tr></table></td></tr></table></td> \n" \
				  "<td width=30%% align=center>%d / %d   &nbsp;kbps </td></tr> \n", rate, UPLINLK_SPEED);

	websWrite(wp, "<tr><td width=30%% align=right></td> \n" \
				   "<td align=center>%d%%</td> \n" \
				   "<td></td></tr></table></td></tr></table> \n" \
				   , ratePercent);

	return 0;
	
}
static int doWriteMonitor(webs_t wp, int classNum, char *serviceName)
{
	FILE *fp, *fp1;
	char cmdBuf[200]={0};
	char lineNumStr[4], rateStr[8]={0}, tmpStr[8]={0};
	int lineNum;
	int rate;
	int ratePercent;
	int i,k=0;

	sprintf(cmdBuf, "cat /etc_ro/web/tc_class_show | grep -n \"1:%d\" | sed 's/:class.*$//g' > /etc_ro/web/class_line_num", classNum);
	system(cmdBuf);

	if(!(fp=fopen("/etc_ro/web/class_line_num", "r")))
	{
		printf("Can not open class num file !!\n");
		fclose(fp);
		return 1;
	}else{
		fgets(lineNumStr, 4, fp);
		//printf("lineNumStr=%s\n", lineNumStr);
		lineNum=atoi(lineNumStr);
		//printf("lineNum=%d\n", lineNum);
	}
	fclose(fp);
	lineNum=lineNum+2;
	memset(cmdBuf, 0, sizeof(cmdBuf));
	sprintf(cmdBuf, "cat /etc_ro/web/tc_class_show | sed -n '%dp' | sed 's/^.rate.//g' | sed 's/bit.*$//g' >/etc_ro/web/class_rate ", lineNum);
	system(cmdBuf);
	if(!(fp1=fopen("/etc_ro/web/class_rate", "r")))
	{
		printf("Can not open class rate file !!\n");
		fclose(fp);
		return 1;
	}else{
		fgets(rateStr, 8, fp1);
		//printf("rateStr=%s\n", rateStr);
		//lineNum=atoi(lineNumStr);
		//printf("lineNum=%d\n", lineNum);
	}
	fclose(fp1);
	//if kilo
	for(i=0;i<strlen(rateStr);i++){
		if(rateStr[i]=='K')
			k=1;
	}
	if(k==1){
		strncpy(tmpStr, rateStr, strlen(rateStr)-1);
		rate=atoi(tmpStr);
		ratePercent=(rate*100/UPLINLK_SPEED);
	}else{
		strncpy(tmpStr, rateStr, strlen(rateStr));
		rate=atoi(tmpStr);
		if(rate < 1024){ //less than 1024, ingore it.
			rate=0;
			ratePercent=0;
		}else{
			rate= (rate/1024);
			ratePercent=(rate*100/UPLINLK_SPEED);
		}
	}
	websWrite(wp, "<table width=90%% border=0 align=center cellspacing=0 class=wtext_3> \n" \
				  "<tr><td><table width=100%% border=0 cellspacing=0 cellpadding=0> \n" \
				  "<tr><td width=30%% align=center>%s</td> \n", serviceName);

	websWrite(wp, "<td height=10> \n" \
				  "<table width=100%% border=1 cellpadding=0 cellspacing=0  bordercolor=\"#000000\" ><tr> \n" \
				  "<td height=20 background=\"images/i_percentage_center.gif\"> \n" \
				  "<table border=0 cellspacing=0 cellpadding=0 width=%d%%> \n", ratePercent);

	websWrite(wp, "<tr><td background=\"images/i_percentage_blue.gif\"  height=20></td> \n" \
				  "</tr></table></td></tr></table></td> \n" \
				  "<td width=30%% align=center>%d / %d   &nbsp;kbps </td></tr> \n", rate, UPLINLK_SPEED);

	websWrite(wp, "<tr><td width=30%% align=right></td> \n" \
				   "<td align=center>%d%%</td> \n" \
				   "<td></td></tr></table></td></tr></table> \n" \
				   , ratePercent);

	return 0;

}

//RL: Show upload bandwidth monitor
static int QoSDispMonitor(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	int i;
	
	printf("Steve DBG =>%s\n",__FUNCTION__);
	char_t TempBuf[32]={0};
	char_t upSpeedBuf[32]={0};
	char_t downSpeedBuf[32]={0};
	char_t ruleEnableBuf[32]={0};
	
	char bw_user_en[3];
	char bw_user_proto[32];
	char bw_user_name[32];
	char bw_user_dir[32];
	
	ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		    
	if(!strcmp(TempBuf, "1")){
		ezplib_get_attr_val("bw_rule", 0, "bw_upload", upSpeedBuf, 32, EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_rule", 0, "bw_download", downSpeedBuf, 32, EZPLIB_USE_CLI);
		
		UPLINLK_SPEED=atoi(upSpeedBuf);
			
		for(i=0; i<11; i++)
		{		
			ezplib_get_attr_val("bw_app_rule", i, "enable", ruleEnableBuf, 32, EZPLIB_USE_CLI);
			if(!strcmp(ruleEnableBuf, "1")){
				if(i==0 || i==1 || i==2 || i==9 ){
					doWriteMonitor_4615(wp, "tcp" , appQoSService[i], 6);
					doWriteMonitor_4615(wp, "udp" , appQoSService[i], 6);
				}
				else{
					doWriteMonitor_4615(wp, "tcp" , appQoSService[i], 6);
				}
			}
		}	
		
		for(i=0; i<8; i++){
			ezplib_get_attr_val("bw_sbm_rule", i, "enable", bw_user_en, sizeof(bw_user_en), EZPLIB_USE_CLI);
			if(!strcmp(bw_user_en, "1")){
				ezplib_get_attr_val("bw_sbm_rule", i, "proto", bw_user_proto, sizeof(bw_user_proto), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_sbm_rule", i, "name", bw_user_name, sizeof(bw_user_name), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_sbm_rule", i, "bw_towan", bw_user_dir, sizeof(bw_user_dir), EZPLIB_USE_CLI);
			    //printf("i=%d protc=%s name=%s\n", i, bw_user_proto, bw_user_name);  
			    if(strcmp(bw_user_dir, "0")){ //To WAN
			    	doWriteMonitor_4615(wp, bw_user_proto , bw_user_name, 6);
			    }
			}
		}
	}
#else	
	//char_t *appQoSitem[]={"XBox Live","VoIP","FTP", "eMule","BitTorrent", "E-Mail","Instant Messenger ","Web Surfing" };
	int classMark[14]={ 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 61};
	char_t *upSpeed, *downSpeed;
	char_t *aRule, *uRule;
	char_t aE[3], uE[3], uDir[3], uName[25];
	char_t subaE[3], subaE2[3];
	char_t paramStr[20];
	FILE *fp;
	int i, j;
	int wanStart=8;
	int wan_mark_start=70;
	//int xboxMark=8;
	char_t *QoS_Enable = nvram_bufget(RT2860_NVRAM, "RL_QoSEnable");

	if( !strncmp(QoS_Enable, "1", 2))
	{
		char *cm, *op;
		char *wanIfName;

		op = nvram_bufget(RT2860_NVRAM, "OP_Mode");
		cm = nvram_bufget(RT2860_NVRAM, "wanConnectionMode");
		if( !strncmp(op, "0", 2) || !strncmp(op, "6", 2)){
			if (!strncmp(cm, "PPPOE", 6) || !strncmp(cm, "L2TP", 5) || !strncmp(cm, "PPTP", 5))
				wanIfName="ppp0";
			else if(!strncmp(op, "6", 2))
				wanIfName="ra0";
			else if(!strncmp(op, "0", 2))
				wanIfName="eth2.2";
		}else
			return 0;

		doSystem("tc -s class ls dev %s > /etc_ro/web/tc_class_show", wanIfName);

		upSpeed=nvram_bufget(RT2860_NVRAM, "RL_QoSUploadBw");
		downSpeed=nvram_bufget(RT2860_NVRAM, "RL_QoSDownloadBw");

		if(!strcmp(upSpeed, "64k"))
			UPLINLK_SPEED=64;
		else if(!strcmp(upSpeed, "96k"))
			UPLINLK_SPEED=96;
		else if(!strcmp(upSpeed, "128k"))
			UPLINLK_SPEED=128;
		else if(!strcmp(upSpeed, "192k"))
			UPLINLK_SPEED=192;
		else if(!strcmp(upSpeed, "256k"))
			UPLINLK_SPEED=256;
		else if(!strcmp(upSpeed, "384k"))
			UPLINLK_SPEED=384;
		else if(!strcmp(upSpeed, "512k"))
			UPLINLK_SPEED=512;
		else if(!strcmp(upSpeed, "768k"))
			UPLINLK_SPEED=768;
		else if(!strcmp(upSpeed, "1M"))
			UPLINLK_SPEED=1*1024;
		else if(!strcmp(upSpeed, "2M"))
			UPLINLK_SPEED=2*1024;
		else if(!strcmp(upSpeed, "4M"))
			UPLINLK_SPEED=4*1024;
		else if(!strcmp(upSpeed, "8M"))
			UPLINLK_SPEED=8*1024;
		else if(!strcmp(upSpeed, "10M"))
			UPLINLK_SPEED=10*1024;
		else if(!strcmp(upSpeed, "12M"))
			UPLINLK_SPEED=12*1024;
		else if(!strcmp(upSpeed, "16M"))
			UPLINLK_SPEED=16*1024;
		else if(!strcmp(upSpeed, "20M"))
			UPLINLK_SPEED=20*1024;
		else if(!strcmp(upSpeed, "24M"))
			UPLINLK_SPEED=24*1024;
		else if(!strcmp(upSpeed, "32M"))
			UPLINLK_SPEED=32*1024;

		strcpy( STR_UPLINLK_SPEED, upSpeed);
		strcpy( STR_DOWNLINK_SPEED,downSpeed);

		//2009-07-04, added default class to show
		//doWriteMonitor(wp, 2, "High Prio Class");
		//doWriteMonitor(wp, 4, "Default Class");

		//app define
		for(i=0; i<QOS_APPRULE_COUNT; i++)
		{
			sprintf(paramStr, "QoSAppRules%d", i);
			aRule = nvram_bufget(RT2860_NVRAM, paramStr);
			getNthValueSafe(1, aRule, ',', aE, sizeof(aE));
			if(atoi(aE) != 1)
				continue;

			if(i==0 || i==1 || i==2 ){
				for(j=0; j<2; j++)
				{
					getNthValueSafe(wanStart, aRule, ',', subaE, sizeof(subaE));
					wanStart=wanStart+3;
					if(atoi(subaE) != 1)
						continue;
					//printf("xbox enable\n");
					doWriteMonitor(wp, classMark[ j + (i *2) ], appQoSService[i]);
				}
			}else{
				getNthValueSafe(5, aRule, ',', subaE, sizeof(subaE));
				if(atoi(subaE) != 1)
					continue;
				doWriteMonitor(wp, classMark[i+3], appQoSService[i]);
			}
		}

		memset(paramStr, 0, sizeof(paramStr));
		//user defined
		for(i=0; i<QOS_USERRULE_COUNT; i++)
		{
			sprintf(paramStr, "QoSUserRules%d", i);
			uRule = nvram_bufget(RT2860_NVRAM, paramStr);
			getNthValueSafe(0, uRule, ',', uE, sizeof(uE));
			getNthValueSafe(1, uRule, ',', uDir, sizeof(uDir));
			getNthValueSafe(2, uRule, ',', uName, sizeof(uName));
			if((atoi(uE) == 1) && (atoi(uDir) == 1)) //enable and to WAN
				doWriteMonitor(wp, (wan_mark_start)+i, uName);
			else
				continue;

		}

	}
#endif
	return 0;

}

static int doWriteDwMonitor(webs_t wp, int type, int classNum, char *serviceName)
{
	FILE *fp, *fp1;
	char *append_file_name;
	char cmdBuf[200]={0};
	char lineNumStr[4], rateStr[8]={0}, tmpStr[8]={0};
	int lineNum;
	int rate;
	int ratePercent;
	int i,k=0;
	if(type==3)
		append_file_name="lan";		
	else if(type==5)
		append_file_name="wlan";
	
	
	sprintf(cmdBuf, "cat /etc_ro/web/tc_class_show_%s | grep -n \"%d:%d \" | sed 's/:class.*$//g' > /etc_ro/web/class_line_num_lan",
				append_file_name, type, classNum);
	system(cmdBuf);

	if(!(fp=fopen("/etc_ro/web/class_line_num_lan", "r")))
	{
		printf("Can not open class num file !!\n");
		fclose(fp);
		return 1;
	}else{
		fgets(lineNumStr, 4, fp);
		//printf("lineNumStr=%s\n", lineNumStr);
		lineNum=atoi(lineNumStr);
		//printf("lineNum=%d\n", lineNum);
	}
	fclose(fp);
	lineNum=lineNum+2;
	memset(cmdBuf, 0, sizeof(cmdBuf));
	sprintf(cmdBuf, "cat /etc_ro/web/tc_class_show_%s | sed -n '%dp' | sed 's/^.rate.//g' | sed 's/bit.*$//g' >/etc_ro/web/class_rate_lan ", append_file_name, lineNum);
	system(cmdBuf);
	if(!(fp1=fopen("/etc_ro/web/class_rate_lan", "r")))
	{
		printf("Can not open class rate file !!\n");
		fclose(fp);
		return 1;
	}else{
		fgets(rateStr, 8, fp1);
		//printf("rateStr=%s\n", rateStr);
		//lineNum=atoi(lineNumStr);
		//printf("lineNum=%d\n", lineNum);
	}
	fclose(fp1);
	//if kilo
	for(i=0;i<strlen(rateStr);i++){
		if(rateStr[i]=='K')
			k=1;
	}
	if(k==1){
		strncpy(tmpStr, rateStr, strlen(rateStr)-1);
		rate=atoi(tmpStr);
		ratePercent=(rate*100/DOWNLINK_SPEED);
	}else{
		strncpy(tmpStr, rateStr, strlen(rateStr));
		rate=atoi(tmpStr);
		if(rate < 1024){ //less than 1024, ingore it.
			rate=0;
			ratePercent=0;
		}else{
			rate= (rate/1024);
			ratePercent=(rate*100/DOWNLINK_SPEED);
		}
	}

	websWrite(wp, "<table width=90%% border=0 align=center cellspacing=0 class=wtext_3> \n" \
				  "<tr><td><table width=100%% border=0 cellspacing=0 cellpadding=0> \n" \
				  "<tr><td width=30%% align=center>%s</td> \n", serviceName);

	websWrite(wp, "<td height=10> \n" \
				  "<table width=100%% border=1 cellpadding=0 cellspacing=0  bordercolor=\"#000000\" ><tr> \n" \
				  "<td height=20 background=\"images/i_percentage_center.gif\"> \n" \
				  "<table border=0 cellspacing=0 cellpadding=0 width=%d%%> \n", ratePercent);

	websWrite(wp, "<tr><td background=\"images/i_percentage_blue.gif\"  height=20></td> \n" \
				  "</tr></table></td></tr></table></td> \n" \
				  "<td width=30%% align=center>%d / %d   &nbsp;kbps </td></tr> \n", rate, DOWNLINK_SPEED);

	websWrite(wp, "<tr><td width=30%% align=right></td> \n" \
				   "<td align=center>%d%%</td> \n" \
				   "<td></td></tr></table></td></tr></table> \n" \
				   , ratePercent);

	return 0;

}

//RL: Show download bandwidth monitor
static int QoSDispDwMonitor(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	int i;
	
	printf("Steve DBG =>%s\n",__FUNCTION__);
	char_t TempBuf[32]={0};
	char_t upSpeedBuf[32]={0};
	char_t downSpeedBuf[32]={0};
	char_t ruleEnableBuf[32]={0};
	
	char bw_user_en[3];
	char bw_user_proto[32];
	char bw_user_name[32];
	char bw_user_dir[32];
	
	ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		    
	if(!strcmp(TempBuf, "1")){
		ezplib_get_attr_val("bw_rule", 0, "bw_upload", upSpeedBuf, 32, EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_rule", 0, "bw_download", downSpeedBuf, 32, EZPLIB_USE_CLI);
		
		UPLINLK_SPEED=atoi(downSpeedBuf);
			
		for(i=0; i<11; i++)
		{		
			ezplib_get_attr_val("bw_app_rule", i, "enable", ruleEnableBuf, 32, EZPLIB_USE_CLI);
			if(!strcmp(ruleEnableBuf, "1")){
				if(i==0 || i==1 || i==2 || i==9 ){
					doWriteMonitor_4615(wp, "tcp" , appQoSService[i], 8);
					doWriteMonitor_4615(wp, "udp" , appQoSService[i], 8);
				}
				else{
					doWriteMonitor_4615(wp, "tcp" , appQoSService[i], 8);
				}
			}
		}	
		
		for(i=0; i<8; i++){
			ezplib_get_attr_val("bw_sbm_rule", i, "enable", bw_user_en, sizeof(bw_user_en), EZPLIB_USE_CLI);
			if(!strcmp(bw_user_en, "1")){
				ezplib_get_attr_val("bw_sbm_rule", i, "proto", bw_user_proto, sizeof(bw_user_proto), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_sbm_rule", i, "name", bw_user_name, sizeof(bw_user_name), EZPLIB_USE_CLI);
				ezplib_get_attr_val("bw_sbm_rule", i, "bw_towan", bw_user_dir, sizeof(bw_user_dir), EZPLIB_USE_CLI);
			    //printf("i=%d protc=%s name=%s\n", i, bw_user_proto, bw_user_name);  
			    if(!strcmp(bw_user_dir, "0")){ //To WAN
			    	doWriteMonitor_4615(wp, bw_user_proto , bw_user_name, 8);
			    }
			}
		}
	}
#else
	char *op;
	char_t *lanIfName, *wlanIfName;
	int devLanId, devWlanId;
	char_t downS[8]={0};
	char_t *downSpeed;
	
	int classMark[14]={ 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60};
	char_t *aRule, *uRule;
	char_t aE[3], uE[3], uDir[3], uName[25];
	char_t subaE[3], subaE2[3];
	char_t paramStr[20];
	FILE *fp;
	int i, j;
	int lanStart=2, wlanStart=14;
	char *protoStr;
	char serviceStr[80]={0};
	int lan_mark_start=80, wlan_mark_start=90;

	char_t *QoS_Enable = nvram_bufget(RT2860_NVRAM, "RL_QoSEnable");

	if( !strncmp(QoS_Enable, "1", 2))
	{
		//interface
		op = nvram_bufget(RT2860_NVRAM, "OP_Mode");
		if( !strncmp(op, "0", 2) ){
			devLanId=3;
			//lanIfName="eth2.1";
			lanIfName="br0";
			doSystem("tc -s class ls dev %s > /etc_ro/web/tc_class_show_lan", lanIfName);
			//devWlanId=5;
			//wlanIfName="ra0";
			//doSystem("tc -s class ls dev %s > /etc_ro/web/tc_class_show_wlan", wlanIfName);
		}else if(!strncmp(op, "6", 2)){
			devLanId=3;
			lanIfName="eth2";
			wlanIfName=NULL;
		}	

		//speed
		strcpy(downS, nvram_bufget(RT2860_NVRAM, "RL_QoSDownloadBw"));
		if(strchr(downS, 'k')){
			downSpeed = strtok(downS, "k");
			DOWNLINK_SPEED=atoi(downSpeed);
		}else if(strchr(downS, 'M')){
			downSpeed = strtok(downS, "M");
			DOWNLINK_SPEED=(atoi(downSpeed)*1024);
		}	

		strcpy( STR_DOWNLINK_SPEED, downS);

		//2009-07-04, added default class to show
		doWriteDwMonitor(wp, 3, 2, "High Prio Class");
		doWriteDwMonitor(wp, 3, 4, "Default Class");

		//app define
		for(i=0; i<QOS_APPRULE_COUNT; i++)
		{
			sprintf(paramStr, "QoSAppRules%d", i);
			aRule = nvram_bufget(RT2860_NVRAM, paramStr);
			getNthValueSafe(1, aRule, ',', aE, sizeof(aE));
			if(atoi(aE) != 1)
				continue;

			memset(serviceStr, 0, sizeof(serviceStr));

			if(i==0 || i==1 || i==2 ){
				for(j=0; j<2; j++)
				{					
					if(j==0)
						protoStr= " tcp";
					else
						protoStr= " udp";
					
					//lan
					getNthValueSafe( lanStart, aRule, ',', subaE, sizeof(subaE));
					lanStart=lanStart+3;
					if(atoi(subaE) == 1){
						//sprintf(serviceStr, "To LAN-  %s %s", appQoSService[i], protoStr);
						sprintf(serviceStr, "%s %s", appQoSService[i], protoStr);
						doWriteDwMonitor(wp, devLanId, classMark[ j + (i *2) ], serviceStr);
					}
	
					#if 0
					//wlan
					getNthValueSafe( wlanStart, aRule, ',', subaE, sizeof(subaE));
					wlanStart=wlanStart+3;
					if(atoi(subaE) == 1){
						sprintf(serviceStr, "To WLAN-  %s %s", appQoSService[i], protoStr);
						doWriteDwMonitor(wp, devWlanId, classMark[ j + (i *2) ], serviceStr);
					}
					#endif
				}
			}else{
				getNthValueSafe(2, aRule, ',', subaE, sizeof(subaE));
				if(atoi(subaE) == 1){
					//sprintf(serviceStr, "To LAN-  %s", appQoSService[i]);
					sprintf(serviceStr, " %s", appQoSService[i]);
					doWriteDwMonitor(wp, devLanId, classMark[i+3], serviceStr);
				}

				#if 0
				getNthValueSafe(8, aRule, ',', subaE, sizeof(subaE));
				if(atoi(subaE) == 1){
					sprintf(serviceStr, "To WLAN-  %s", appQoSService[i]);
					doWriteDwMonitor(wp, devWlanId, classMark[i+3], serviceStr);
				}
				#endif
			}
		}

		memset(paramStr, 0, sizeof(paramStr));
		//user defined
		for(i=0; i<QOS_USERRULE_COUNT; i++)
		{
			memset(serviceStr, 0, sizeof(serviceStr));
			sprintf(paramStr, "QoSUserRules%d", i);
			uRule = nvram_bufget(RT2860_NVRAM, paramStr);
			getNthValueSafe(0, uRule, ',', uE, sizeof(uE));
			getNthValueSafe(1, uRule, ',', uDir, sizeof(uDir));
			getNthValueSafe(2, uRule, ',', uName, sizeof(uName));
			if((atoi(uE) == 1) && (atoi(uDir) == 0)){ //enable and to LAN
				//sprintf(serviceStr, "To LAN-%s", uName);
				sprintf(serviceStr, " %s", uName);
				doWriteDwMonitor(wp, devLanId, (lan_mark_start)+i, serviceStr);
			}else if((atoi(uE) == 1) && (atoi(uDir) == 2) && ( !strncmp(op, "0", 2) )){ //enable and to WLAN
				sprintf(serviceStr, "To WLAN- %s", uName);
				doWriteDwMonitor(wp, devWlanId, (wlan_mark_start)+i, serviceStr);
			}
		}

	}
#endif
	return 0;

}
static int EasyModeShowSort(int eid, webs_t wp, int argc, char_t **argv)
{
	
	//SortingEZModeRule();
	
#if 1
	char_t sortQN[6][20];
	char_t sortQ[6][10];
	int i,j;
	char_t s_gameEnable[3],s_gamePrio[3];
	int has_1_enable=0;
	char_t usrName[QOS_USERRULE_COUNT][25];
	char_t usrCate[QOS_USERRULE_COUNT][20];
	
	 //printf("\n");
	 
	//app 
	for(i=0;i<QOS_CATEGORY_COUNT;i++){
		ezplib_get_attr_val("bw_ezmodesort_rule", i, "catname", sortQN[i], sizeof(sortQN[i]), EZPLIB_USE_CLI);
	    //printf("sortQN[%d]=%s\n",i,sortQN[i]);
	    
	    if(!strncmp( sortQN[i], "Game Console", 12)){
	     strcpy(sortQ[i],"game");	
	    }
	    if(!strncmp( sortQN[i], "VoIP", 4)){
	     strcpy(sortQ[i],"voip");	
	    }
	    if(!strncmp( sortQN[i], "Instant Messenger", 17)){
	     strcpy(sortQ[i],"media");	
	    }
	    if(!strncmp( sortQN[i], "Web Surfing", 11)){
	     strcpy(sortQ[i],"web");	
	    }
	    if(!strncmp( sortQN[i], "P2P/FTP", 7)){
	     strcpy(sortQ[i],"p2p");	
	    }
	    if(!strncmp( sortQN[i], "E-Mail", 6)){
	     strcpy(sortQ[i],"mail");	
	    }
	}
	
	//
	for(i=0;i<QOS_USERRULE_COUNT;i++){
		ezplib_get_attr_val("bw_sbm_rule", i, "name", usrName[i], sizeof(usrName[i]), EZPLIB_USE_CLI);
		ezplib_get_attr_val("bw_sbm_rule", i, "apptype", usrCate[i], sizeof(usrCate[i]), EZPLIB_USE_CLI);
	    //printf("usrName[%d]=%s\n",i,usrName[i]);
	    //printf("usrCate[%d]=%s\n",i,usrCate[i]);
	}
	
	//
	//Rule:  if qos enable, then check the priority of game console	    	
	ezplib_get_attr_val("bw_app_rule", 0, "prio", s_gamePrio, sizeof(s_gamePrio), EZPLIB_USE_CLI);
	for(i=0;i<4;i++){
		ezplib_get_attr_val("bw_app_rule", i, "enable", s_gameEnable, sizeof(s_gameEnable), EZPLIB_USE_CLI);	    
		if(!strcmp(s_gamePrio, "1")){
			has_1_enable=1;
			break;
		}
	}
	
	for(i=0;i<QOS_CATEGORY_COUNT;i++){
		//printf("RL debug: sortQ[%d]=%s\n", i, sortQ[i] );
		//RL added rule, if game engine enable, we will fix its position.2009/4/2 03:11pm
		if(!strncmp( sortQ[i], "game", 4) && !strcmp(s_gamePrio, "1") && has_1_enable==1 )
			websWrite(wp, "<div  class=\"groupItemFixed\"> \n" );
		else
			websWrite(wp, "<div  class=\"groupItem\"> \n" );

		websWrite(wp, "<div id = \"%s\" class=\"itemContent\"> \n" \
					  "<input type=hidden name=%s value=%s > \n" , sortQ[i], sortQ[i], sortQ[i]);
		websWrite(wp, "<table width=100%% border=0 cellspacing=0 cellpadding=0 class=tab> \n" \
					  "<tr><td width=1 height=38px><img src=\"images/air.gif\" id=Image1></td> \n");
		websWrite(wp, "<td width=54 background=\"images/b_%s.gif\"></td> \n" \
					  "<td background=\"images/b_right.gif\" class=\"tabTitle\" > \n" , sortQ[i]);

		if (!strncmp( sortQ[i], "web", 3))
			websWrite(wp, "<div align=left><a href=\"\">Web Surfing<span>- Web Surfing\n");
		else if(!strncmp( sortQ[i], "game", 4))
			websWrite(wp, "<div align=left><a href=\"\">Game Console<span>- XBox Live<br />- PlayStation<br />- MSN Game Zone<br />- Battlenet \n");
		else if(!strncmp( sortQ[i], "mail", 4))
			websWrite(wp, "<div align=left><a href=\"\">E-mail<span>- E-Mail \n");
		else if(!strncmp( sortQ[i], "voip", 4))
			websWrite(wp, "<div align=left><a href=\"\">VoIP<span>- VoIP  \n");
		else if(!strncmp( sortQ[i], "media", 5))
			websWrite(wp, "<div align=left><a href=\"\">Instant Messenger<span>- Instant Messenger  \n");
		else if(!strncmp( sortQ[i], "p2p", 3))
			websWrite(wp, "<div align=left><a href=\"\">P2P/FTP<span>- eMule <br />- Bit Torrent<br />- FTP \n");

		//RL added user define service name
		for(j=0;j<QOS_USERRULE_COUNT;j++){
			if ( !strcmp(usrCate[j], sortQN[i]) ){
				if(strlen(usrName[j]) >= 1 )
					websWrite(wp, "<br />- %s\n", usrName[j] );
			}
		}
		websWrite(wp, "</span></a></div></td></tr></table></div></div> \n");
	}

	
#else	
	char_t *Rule;
	char_t *uRule;
	char_t sortQ[6][20];
	char_t usrName[QOS_USERRULE_COUNT][25];
	char_t usrCate[QOS_USERRULE_COUNT][3];
	char_t bufStr[20]={0};
	int i,j;
	int sq; //sorting queue
	char *appRule;
	char app_prio[3];
	char tmpStr[20];
	char app_enable[3];
	int has_1_enable=0;

	Rule = nvram_bufget(RT2860_NVRAM, "EasyModeSortQoS");
	printf("RL Ez Mo: Rule Queue=%s\n", Rule );

	//get user define service name and category
	for(i=0;i<QOS_USERRULE_COUNT;i++){
		memset(bufStr, 0, sizeof(bufStr));
		sprintf(bufStr, "QoSUserRules%d", i);
		uRule = nvram_bufget(RT2860_NVRAM, bufStr);

		getNthValueSafe(2, uRule, ',', usrName[i], sizeof(usrName[i]));
		//printf("usrName[%d])=%s\n", i, usrName[i]));
		getNthValueSafe(3, uRule, ',', usrCate[i], sizeof(usrCate[i]));
	}

	for(i=0;i<QOS_CATEGORY_COUNT;i++)
		getNthValueSafe(i, Rule, ',', sortQ[i], sizeof(sortQ[i]));

	//RL added rule, if game engine enable, we will fix its position.2009/4/2 03:11pm
	appRule = nvram_bufget(RT2860_NVRAM,  "QoSAppRules0");
	getNthValueSafe(0, appRule, ',', app_prio, sizeof(app_prio));
	for(i=0;i<4;i++){
		sprintf(tmpStr, "QoSAppRules%d", i);
		appRule = nvram_bufget(RT2860_NVRAM, tmpStr);
		getNthValueSafe(1, appRule, ',', app_enable, sizeof(app_enable));
		if(!strcmp(app_enable, "1")){
			has_1_enable=1;
			break;
		}
	}

	for(i=0;i<QOS_CATEGORY_COUNT;i++){
		//printf("RL debug: sortQ[%d]=%s\n", i, sortQ[i] );
		//RL added rule, if game engine enable, we will fix its position.2009/4/2 03:11pm
		if(!strncmp( sortQ[i], "game", 5) && !strcmp(app_prio, "0") && has_1_enable==1 )
			websWrite(wp, "<div  class=\"groupItemFixed\"> \n" );
		else
			websWrite(wp, "<div  class=\"groupItem\"> \n" );

		websWrite(wp, "<div id = \"%s\" class=\"itemContent\"> \n" \
					  "<input type=hidden name=%s value=%s > \n" , sortQ[i], sortQ[i], sortQ[i]);
		websWrite(wp, "<table width=100%% border=0 cellspacing=0 cellpadding=0 class=tab> \n" \
					  "<tr><td width=1 height=38px><img src=\"images/air.gif\" id=Image1></td> \n");
		websWrite(wp, "<td width=54 background=\"images/b_%s.gif\"></td> \n" \
					  "<td background=\"images/b_right.gif\" class=\"tabTitle\" > \n" , sortQ[i]);

		if (!strncmp( sortQ[i], "web", 4))
			websWrite(wp, "<div align=left><a href=\"\">Web Surfing<span>- Web Surfing\n");
		else if(!strncmp( sortQ[i], "game", 5))
			websWrite(wp, "<div align=left><a href=\"\">Game Console<span>- XBox Live<br />- PlayStation<br />- MSN Game Zone<br />- Battlenet \n");
		else if(!strncmp( sortQ[i], "mail", 6))
			websWrite(wp, "<div align=left><a href=\"\">E-mail<span>- E-Mail \n");
		else if(!strncmp( sortQ[i], "voip", 5))
			websWrite(wp, "<div align=left><a href=\"\">VoIP<span>- VoIP  \n");
		else if(!strncmp( sortQ[i], "media", 6))
			websWrite(wp, "<div align=left><a href=\"\">Instant Messenger<span>- Instant Messenger  \n");
		else if(!strncmp( sortQ[i], "p2p", 4))
			websWrite(wp, "<div align=left><a href=\"\">P2P/FTP<span>- eMule <br />- Bit Torrent<br />- FTP \n");

		//RL added user define service name
		for(j=0;j<QOS_USERRULE_COUNT;j++){
			sq = atoi(usrCate[j]);
			if ( !strcmp(defCateName[sq], sortQ[i]) ){
				if(strlen(usrName[j]) >= 1 )
					websWrite(wp, "<br />- %s\n", usrName[j] );
			}

		}
		websWrite(wp, "</span></a></div></td></tr></table></div></div> \n");
	}
#endif
	return 0;

}

#if 0
		QoS Rules specific :
	1. Easy mode :
		Rule:
			high	1st
			high	2nd
			mid		3rd
			mid		4th
			low		5th
			low		6th
	2. Setting:
	Data base:
		0	XBox Live
		1 	PlayStation
		2 	MSN Game Zone
		3 	Battlenet
		4	VoIP
		5	MSN WebCam
		6 	WWW
		7	FTP
		8	EMULE
		9	BT
		10 	Email
		Category:
			0	game
			1	voip
			2	media
			3	web
			4	p2p
			5	mail
#endif

//static int ChangeAppRule(int idx, char_t *src)
int ChangeAppRule(int idx, char_t *src)
{
	char *begin, *end, *s_begin;
	int loops, j;
	char_t mod[128]={0};
	strcpy(mod, src);
	begin = mod;
	s_begin = mod;
	if( idx==0 || idx==1 ||idx==2 )
		loops=11;//loops=17;
	else
		loops=5;//loops=8;

	j=0;
	while(j<loops)
	{
		end = strchr(begin, ',');
		begin = end+1;
		j++;
		if((j%3)==2){
			//printf("EZ mode:got it begin=%s\n", begin);
			mod[begin-s_begin]='1';
		}
	}
	strcpy(src, mod);

	return 0;
}

//RL: set EZ mode page
static void EasyModeQoS(webs_t wp, char_t *path, char_t *query)
{
	int i,j;
	char_t str[8]={0};
	char_t *q[6];
	char_t str_prio[3]={0};
	char_t TempBuf[32]={0};
	
	for(i=0;i<QOS_CATEGORY_COUNT;i++){
		sprintf(str, "p%d", i);
		q[i] = websGetVar(wp, str, T(""));
		printf("q[%d]=%s\n",i,q[i]);
	}
	printf("RL EasyModeQoS 1 \n");
	printf("/sbin/ezp-bw stop \n");
	system("/sbin/ezp-bw stop");
	
	for ( i=0; i<6; i++ )
	{
		if((i==0) ||(i==1))
			strcpy(str_prio,"1");
		else if((i==2) ||(i==3))
			strcpy(str_prio,"2");
		else if((i==4) ||(i==5))
			strcpy(str_prio,"3");	
		
        if(!strncmp(q[i],"game",4))
        {   
        	for(j=0;j<4;j++){
        		ezplib_replace_attr("bw_app_rule", j, "enable", "1");
	    		ezplib_replace_attr("bw_app_rule", j, "prio", str_prio);
	    	}
	    	ezplib_replace_attr("bw_ezmodesort_rule", i, "catname", "Game Console");
        }
        if(!strncmp(q[i],"voip",4))
        { 
        	ezplib_replace_attr("bw_app_rule", 4, "enable", "1");
	    	ezplib_replace_attr("bw_app_rule", 4, "prio", str_prio);
	    	ezplib_replace_attr("bw_ezmodesort_rule", i, "catname", "VoIP");
        }
        if(!strncmp(q[i],"media",5))
        { 
        	ezplib_replace_attr("bw_app_rule", 5, "enable", "1");
	    	ezplib_replace_attr("bw_app_rule", 5, "prio", str_prio);
	    	ezplib_replace_attr("bw_ezmodesort_rule", i, "catname", "Instant Messenger");
        }
        if(!strncmp(q[i],"web",3))
        { 
        	ezplib_replace_attr("bw_app_rule", 6, "enable", "1");
	    	ezplib_replace_attr("bw_app_rule", 6, "prio", str_prio);
	    	ezplib_replace_attr("bw_ezmodesort_rule", i, "catname", "Web Surfing");
        }
        if(!strncmp(q[i],"p2p",3))
        { 
        	for(j=7;j<10;j++){
        		ezplib_replace_attr("bw_app_rule", j, "enable", "1");
	    		ezplib_replace_attr("bw_app_rule", j, "prio", str_prio);
	    	}
	    	ezplib_replace_attr("bw_ezmodesort_rule", i, "catname", "P2P/FTP");
        }
        if(!strncmp(q[i],"mail",4))
        { 
        		ezplib_replace_attr("bw_app_rule", 10, "enable", "1");
	    		ezplib_replace_attr("bw_app_rule", 10, "prio", str_prio);
	    		ezplib_replace_attr("bw_ezmodesort_rule", i, "catname", "E-Mail");
        }
    
	}
	
	//
	ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);	    
	if(!strcmp(TempBuf, "1")){
		printf("/sbin/ezp-bw start \n");
		system("/sbin/ezp-bw start");
	}
	
	printf("nvram_commit();\n");
	nvram_commit();
	websRedirect(wp, "local/bandwidth.asp");
#if 0
	//char_t *q0, *q1, *q2, *q3, *q4, *q5;
	char_t *q[6];
	char_t Rule[128];
	char_t str[8]={0};
	char_t bufStr[20], bufStr1[20], bufStr2[20];
	char_t *aRule, *aRule1, *aRule2;
	char_t appRule[128], *modRule;
	int i, j, k, which=0;

	for(i=0;i<QOS_CATEGORY_COUNT;i++){
		sprintf(str, "p%d", i);
		q[i] = websGetVar(wp, str, T(""));
	}
	printf("RL EasyModeQoS 1 \n");

	/*q0 = websGetVar(wp, "p0", T(""));
	q1 = websGetVar(wp, "p1", T(""));
	q2 = websGetVar(wp, "p2", T(""));
	q3 = websGetVar(wp, "p3", T(""));
	q4 = websGetVar(wp, "p4", T(""));
	q5 = websGetVar(wp, "p5", T(""));*/

	memset(Rule, 0, sizeof(Rule));
	//snprintf(Rule, sizeof(Rule), "%s,%s,%s,%s,%s,%s", q0, q1, q2, q3, q4, q5);
	snprintf(Rule, sizeof(Rule), "%s,%s,%s,%s,%s,%s", q[0], q[1], q[2], q[3], q[4], q[5]);
	nvram_bufset(RT2860_NVRAM, "EasyModeSortQoS", Rule);

	//find category and set their priority
	for(i=0;i<QOS_CATEGORY_COUNT;i++){
		for(j=0;j<QOS_CATEGORY_COUNT;j++){
			if(!strcmp(defCateName[j], q[i]) ){
				if(j==0){ //game have 4  service
					for(k=0;k<4;k++){
						memset(bufStr, 0, sizeof(bufStr));
						sprintf(bufStr, "QoSAppRules%d", j+k); // 0, 1, 2, 3
						aRule = nvram_bufget(RT2860_NVRAM, bufStr);
						switch(i){
							case 0:
							case 1:
								//high
								strcpy(appRule, aRule);
								appRule[0]='0'; //prio
								appRule[2]='1'; //enable
								ChangeAppRule(j+k, appRule);
								printf("High : %s\n", q[i]);
								break;
							case 2:
							case 3:
								//mid
								strcpy(appRule, aRule);
								appRule[0]='1'; //prio
								appRule[2]='1'; //enable
								ChangeAppRule(j+k, appRule);
								printf("Mid  : %s\n", q[i]);
								break;
							case 4:
							case 5:
								//low
								strcpy(appRule, aRule);
								appRule[0]='2'; //prio
								appRule[2]='1'; //enable
								ChangeAppRule(j+k, appRule);
								printf("Low  : %s\n", q[i]);
								break;
						}
						//printf("RL after modifed appRule=%s\n", appRule);
						nvram_bufset(RT2860_NVRAM, bufStr, appRule);
					}
				}else if(j==4){ //p2p have 3 service
					for(k=3;k<6;k++){
						memset(bufStr, 0, sizeof(bufStr));
						sprintf(bufStr, "QoSAppRules%d", j+k); // 2, 3, 4
						aRule = nvram_bufget(RT2860_NVRAM, bufStr);
						switch(i){
							case 0:
							case 1:
								//high
								strcpy(appRule, aRule);
								appRule[0]='0'; //prio
								appRule[2]='1'; //enable
								ChangeAppRule(j+k, appRule);
								printf("High : %s\n", q[i]);
								break;
							case 2:
							case 3:
								//mid
								strcpy(appRule, aRule);
								appRule[0]='1'; //prio
								appRule[2]='1'; //enable
								ChangeAppRule(j+k, appRule);
								printf("Mid  : %s\n", q[i]);
								break;
							case 4:
							case 5:
								//low
								strcpy(appRule, aRule);
								appRule[0]='2'; //prio
								appRule[2]='1'; //enable
								ChangeAppRule(j+k, appRule);
								printf("Low  : %s\n", q[i]);
								break;
						}
						//printf("RL after modifed appRule=%s\n", appRule);
						nvram_bufset(RT2860_NVRAM, bufStr, appRule);
					}
				}else{
					memset(bufStr, 0, sizeof(bufStr));
					if(j==1) {	//voip
						strcpy(bufStr, "QoSAppRules4");
						which=5;
					}else if(j==2){	 //media
						strcpy(bufStr, "QoSAppRules5");
						which=6;
					}else if(j==3){ //web
						strcpy(bufStr, "QoSAppRules6");
						which=7;
					}else if(j==5){ //mail
						strcpy(bufStr, "QoSAppRules10");
						which=10;
					}

					aRule = nvram_bufget(RT2860_NVRAM, bufStr);
					switch(i){
						case 0:
						case 1:
							//high
							strcpy(appRule, aRule);
							appRule[0]='0';
							appRule[2]='1'; //enable
							ChangeAppRule(which, appRule);
							printf("High : %s\n", q[i]);
							break;
						case 2:
						case 3:
							//mid
							strcpy(appRule, aRule);
							appRule[0]='1';
							appRule[2]='1'; //enable
							ChangeAppRule(which, appRule);
							printf("Mid  : %s\n", q[i]);
							break;
						case 4:
						case 5:
							//low
							strcpy(appRule, aRule);
							appRule[0]='2';
							appRule[2]='1'; //enable
							ChangeAppRule(which, appRule);
							printf("Low  : %s\n", q[i]);
							break;
					}
					//printf("RL after modifed appRule=%s\n", appRule);
					nvram_bufset(RT2860_NVRAM, bufStr, appRule);
				}

			}

		}
	}

	nvram_commit(RT2860_NVRAM);
	QoSRestart();
	websRedirect(wp, "local/bandwidth.asp");
#endif
}
#endif //Steve18
void formDefineQoS()
{
	#if 0
	websFormDefine(T("QoSAFAttribute"), QoSAFAttribute);
	websFormDefine(T("qosClassifier"), qosClassifier);
	websFormDefine(T("QoSSetup"), QoSSetup);
	websFormDefine(T("QoSDelete"), QoSDelete);
	websFormDefine(T("QoSDeleteRules"), QoSDeleteRules);
	websFormDefine(T("QoSLoadDefaultProfile"), QoSLoadDefaultProfile);
	#endif

    #if 1
	websFormDefine(T("QoSAdvSetup"), QoSAdvSetup);
	websFormDefine(T("QoSGeneral"), QoSGeneral);
	websFormDefine(T("QoSAppCfg"), QoSAppCfg);
	websFormDefine(T("QoSUserCfg"), QoSUserCfg);
	websAspDefine(T("QoSGetInfo"), QoSGetInfo);
	websAspDefine(T("asp_QoS"), asp_QoS);
	websAspDefine(T("asp_AppCfgQoS"), asp_AppCfgQoS);
	websAspDefine(T("asp_UsrCfgQoS"), asp_UsrCfgQoS);
	websAspDefine(T("QoSDispMonitor"), QoSDispMonitor);
	websAspDefine(T("QoSDispDwMonitor"), QoSDispDwMonitor);
	websAspDefine(T("QoSGatherValidRate"), QoSGatherValidRate);

	websAspDefine(T("EasyModeShowSort"), EasyModeShowSort);
	websFormDefine(T("EasyModeQoS"), EasyModeQoS);
	#endif
}

