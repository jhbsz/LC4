/*
 *	inic.h -- Wireless Configuration Header 
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: firewall.h,v 1.5 2008-04-14 09:26:00 yy Exp $
 */


#define PROTO_UNKNOWN	0
#define PROTO_TCP		1
#define PROTO_UDP		2
#define PROTO_TCP_UDP	3
#define PROTO_ICMP		4
#define PROTO_NONE		5
#define PROTO_ALL		6


#define RULE_MODE_DISABLE	0
#define RULE_MODE_DROP		1
#define RULE_MODE_ACCEPT	2

#define ACTION_DROP		0
#define ACTION_ACCEPT	1

#define WEB_FILTER_CHAIN	"web_filter"
#define IPPORT_FILTER_CHAIN	"macipport_filter"

#define DMZ_CHAIN			"DMZ"
#define PORT_FORWARD_CHAIN	"port_forward"
#define PORT_FORWARD_POST_CHAIN	"port_forward_post" //Steve add 2009/02/23
#define DMZ_FORWARD_CHAIN		"DMZ_FORWARD"	//Steve add 2009/03/09
#define DMZ_POST_CHAIN		"DMZ_POST"	//Steve add 2009/12/08

//steve add for port trigger
#define PORT_TRIGGER_CHAIN	"port_trigger"
#define DOS_FILTER_CHAIN	"dos_block"
#define DOS_LOG_CHAIN	"dos_log"
#define NAT_APPLICATION_NUM 10
#define NAT_ADVANCE_NUM 12

#define MAGIC_NUM			"IPCHAINS"			

#define HTML_NO_FIREWALL_UNDER_BRIDGE_MODE	\
"<img src=\"/graphics/warning.gif\"><font color=#ff0000>&nbsp; &nbsp;Warning: The current operation mode is \"Bridge mode\" and these settings may not be functional.</font>"

void iptablesFilterClear(void);
void iptablesFilterRun(char *rule);
void iptablesWebsFilterRun(void);
void formDefineFirewall(void);
void firewall_init(void);
int isIpNetmaskValid(char *s);	// export for QoS
int isMacValid(char *);			// export for QoS
