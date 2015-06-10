/*******************************************************************************************************************************
 *
 *	Copyright (c) 2007  QNAP Systems, Inc.  All Rights Reserved.
 *
 *	FILE:
 *		finder.h
 *
 *	Abstract: 
 *		Major include header file for finder implementations.  Customize necessary definitions within this file.
 *		For common device-independent protocol definitions, please put in protocol.h.
 *		For optional device implementation definitions, please put in device_name.h, e.g. nas.h or nvr.h.
 *
 *	HISTORY:
 *		03/26/07	meiji chang re-create
 *
 ********************************************************************************************************************************/

#include "protocols.h" 

// supported model name string definitions
#define	MODEL_NAME_TS_101			"TS-101"
#define	MODEL_NAME_TS_201			"TS-201"
#define	MODEL_NAME_TS_409			"TS-409"
#define	MODEL_NAME_VS_101			"VS-101"
#define	MODEL_NAME_VS_201			"VS-201"
#define MODEL_NAME_TS_119                       "TS-119"
#define MODEL_NAME_TS_219                       "TS-219"
#define MODEL_NAME_TS_419                       "TS-419"
#define MODEL_NAME_TS_239			"TS-239"
#define MODEL_NAME_TS_439                       "TS-439"
#define MODEL_NAME_TS_509			"TS-509"
#define MODEL_NAME_TS_609			"TS-609"
#define MODEL_NAME_TS_639                       "TS-639"
#define MODEL_NAME_TS_809			"TS-809"
#define MODEL_NAME_TS_839			"TS-839"
#define MODEL_NAME_TS_259			"TS-259"
#define MODEL_NAME_TS_459			"TS-459"
#define MODEL_NAME_TS_559			"TS-559"
#define MODEL_NAME_TS_659			"TS-659"
#define MODEL_NAME_TS_859			"TS-859"
#define MODEL_NAME_TS_879			"TS-879"
#define MODEL_NAME_TS_1079			"TS-1079"
#define MODEL_NAME_TS_1279			"TS-1279"
#define MODEL_NAME_TS_1679			"TS-1679"
#define MODEL_NAME_TS_269			"TS-269"
#define MODEL_NAME_TS_469			"TS-469"
#define MODEL_NAME_TS_569			"TS-569"
#define MODEL_NAME_TS_669			"TS-669"
#define MODEL_NAME_TS_869			"TS-869"
#define MODEL_NAME_TS_1269			"TS-1269"

// Finder service port definitions 
#define PORT_NAS_INSTALL_WIZARD		8097
#define PORT_CMS_FINDER				8097
#define PORT_VIRUS_AGENT			8098		// reserved, shoud not use...
#define PORT_NVR_FINDER				9500
#define	PORT_ATHENS_FINDER			9100

#if defined(NVR_FINDER)
	#define	FINDER_SERVICE_PORT		PORT_NVR_FINDER		
#elif defined(Athens)
	#define	FINDER_SERVICE_PORT		PORT_ATHENS_FINDER
#elif defined(CMS_FINDER)
	#define	FINDER_SERVICE_PORT		PORT_CMS_FINDER
#else
	#define	FINDER_SERVICE_PORT		PORT_NAS_INSTALL_WIZARD		
#endif

#ifdef USE_SHORT_ADMIN_NAME
#define	DEFAULT_ADMIN_USER_NAME			"admin"
#else
#define	DEFAULT_ADMIN_USER_NAME			"administrator"
#endif

#ifdef	ONE_BAY		// VS_101
#define	CONFIGURABLE_DISK_NUMBER		1
#elif	FIVE_BAY
#define	CONFIGURABLE_DISK_NUMBER		5
#elif	SIX_BAY
#define	CONFIGURABLE_DISK_NUMBER		6
#elif	EIGHT_BAY
#define	CONFIGURABLE_DISK_NUMBER		8
#elif	TEN_BAY
#define	CONFIGURABLE_DISK_NUMBER		10
#elif	TWELVE_BAY
#define	CONFIGURABLE_DISK_NUMBER		12
#endif


// Meiji  2008-08-13: add NIC count for multi-NIC models...
#ifdef	TWO_NIC
#define	NIC_NUMBER		2
#elif	THREE_NIC		// not used yet...
#define	NIC_NUMBER		3
#else
#define	NIC_NUMBER		1	// default one NIC
#endif

// device type definition...
#define LINUX_NAS				0
#define WIN_NAS					1
#define NAS_TS_101				2

// data related definition
#define	MAXIMUM_USER_NAME_LENGTH		32
#define	MAXIMUM_USER_NAME_BUFFER_LENGTH		(MAXIMUM_USER_NAME_LENGTH+1)
#define	MAXIMUM_PASSWORD_LENGTH			16
#define	MAXIMUM_PASSWORD_BUFFER_LENGTH		(MAXIMUM_PASSWORD_LENGTH+1)
#define	MAXIMUM_HOSTNAME_LENGTH			64
#define	MAXIMUM_HOSTNAME_BUFFER_LENGTH		(MAXIMUM_HOSTNAME_LENGTH+1)
#define	MAXIMUM_SHARE_NAME_LENGTH		32
#define	MAXIMUM_SHARE_NAME_BUFFER_LENGTH	(MAXIMUM_SHARE_NAME_LENGTH+1)


#define	DEFAULT_PING_TEST_COUNT			3		// for ping test...


#define	FINDER_MAGIC_WORD			"QNAP"

typedef int (*ON_COMMAND_PROC) (FINDER_CMD_PACKET *, int, TAG_LIST *);
typedef struct ON_COMMAND_FUNC
{
	int				nCommandID;
	ON_COMMAND_PROC			pOnCommandFunc;
}	ON_COMMAND_FUNC;



int OnAcquireServer(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnAcquireConfig(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnAuthenticate(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);


void ip_str_to_bytes(char *str, unsigned char *pb);
// pb should be at least 6 bytes buffer, str must be in format "xx:xx:xx:xx:xx:xx"...
void mac_str_to_bytes(char *str, unsigned char *pb);
int Get_Command_Line_Parameter_String(const char *in_str, char *out_str, int buf_size);



// Meiji 2007-12-10: since different model may use different way to write system log, add one function to ease the maintain efforts to write system log, 
void write_event_log(const char *log_str, int log_type);

// Wave 2009-07-01: this function can write the ip address and name of user into system log.
#if _VIOSTOR_
void write_event_log_nvr(const char *log_str, int log_type, const char *user_name);
#endif
