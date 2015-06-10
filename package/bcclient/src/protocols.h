/*******************************************************************************************************************************
 *
 *	Copyright (c) 2007  QNAP Systems, Inc.  All Rights Reserved.
 *
 *	FILE:
 *		protocols.h
 *
 *	Abstract: 
 *		Common protocol definition header to all implementations.
 *		For necessary device-dependent customized definitions, please put in device.h.
 *		For optional device implementation definitions, please put in device_name.h, e.g. nas.h or nvr.h.
 *
 *	HISTORY:
 *		03/26/07	meiji chang re-create
 *
 ********************************************************************************************************************************/

#ifndef	__PROTOCOLS_INCLUDE_H__
#define	__PROTOCOLS_INCLUDE_H__

#ifndef __CPLUSPLUS
typedef int bool;
enum {
	false = 0,
	true = 1
};
#endif

#define	FINDER_PROTOCOL_VERSION			( 0x03020100 + 0323 )	// Jam 2009-03-23: version 3.2.1.0323 - Fixed bug: The ftp port number is incorrect when user changes the ftp port number.
//#define	FINDER_PROTOCOL_VERSION			( 0x03020000 + 1028 )	// Jam 2008-10-28: version 3.2.0.1028 - Fixed bug: Testing Vivotek 7000 series cameras always failed via Finder.
//#define	FINDER_PROTOCOL_VERSION			( 0x03020000 + 1008 )	// Meiji 2008-10-08: version 3.2.0.1008 - Merge all models of NAS/NVR/MP

//#define	USE_BIG_ENDIAN				YES 			// Makefile will set this up...
 
// protocol related definition
#define	MAX_PACKET_SIZE				23412			// single packet maximum size (12 + 180*130)
#define	MAX_TAG_DATA_SIZE			128				// single tag data maximum size
#define EXT_DATA_TAG_MAX_SIZE		123				// max data size for extended data (128 - 4 - 1)
#define	COMMON_BUFFER_LENGTH			128			// for common data, 128 bytes should be bigger enough
#define MAX_NIC_GROUP_MEMBER_NUM	8
#define MAX_NIC_DEVICE_NUM			8
#define MAX_NIC_BONDING_NUM			4
#define MAX_NIC_DEVICE_NAME_LEN		17

#ifdef NIC_BONDING_SUPPORT			// models with two NICS - normally models with more memory...
	#define	MAX_SESSION_NODE_TAG_COUNT		180			// Meiji 2008-09-26: enlarge maximum data tag number for a single finder command
#else
	#define	MAX_SESSION_NODE_TAG_COUNT		80			// maximum data tag number for a single finder command
#endif

// special PAD definition to have device to ignore the sequence number...
#define	PAD_IGNORE_SEQ_NUM			0x0425
#define	NEXT_SEND_SEQ_NUM(cur_seq)		( ( cur_seq + 1 ) % 10000 )
#define	NEXT_RECV_SEQ_NUM(cur_seq)		( ( cur_seq + 2 ) % 10000 )

// protocol customization for endian adjustment
#ifdef USE_BIG_ENDIAN							// for FreeScale
	#define SREV(x) ((((x)&0xFF)<<8) | (((x)>>8)&0xFF))
	#define IREV(x) ((SREV(x)<<16) | (SREV((x)>>16)))
#else									// for Intel
	#define SREV(x) (x)
	#define IREV(x) (x)
#endif

// generic error code for finder operation...
#define	ERROR_BAD_PARAMETERS			-4			// wrong parameters	
#define	ERROR_UNSUPPORTED			-3			// unsupported operation
#define	ERROR_AUTHENTICATION			-2			// bad credential
#define	ERROR_FAILURE				-1			// generic failure code
#define SUCCESS					0			// operation success
#define WORKING_IN_PROGRESS			1			// operation is still under processing, check results later

// COMMON CONSTANT DEFINITIONS...
#define	STRING_TRUE				"TRUE"			// some finder protocol, will return boolean string
#define	STRING_FALSE				"FALSE"

#define	NIC_MODE_STANDALONE		0
#define NIC_MODE_LOAD_BALANCE	1
#define NIC_MODE_FAIL_OVER		2
#define NIC_MODE_BONDING_ROUNDROBIN		10
#define NIC_MODE_BONDING_ACTIVEBACKUP	11
#define NIC_MODE_BONDING_XOR			12
#define NIC_MODE_BONDING_BROADCAST		13
#define NIC_MODE_BONDING_8023AD			14
#define NIC_MODE_BONDING_TLB			15
#define NIC_MODE_BONDING_ALB			16

// Kenneth 2011-04-13: added for setting default gateway
#define DEFAULT_GATEWAY_ETH00	0
#define DEFAULT_GATEWAY_ETH01	1
#define DEFAULT_GATEWAY_ETH02	2
#define DEFAULT_GATEWAY_ETH03	3
#define DEFAULT_GATEWAY_ETH04	4
#define DEFAULT_GATEWAY_ETH05	5
#define DEFAULT_GATEWAY_ETH06	6
#define DEFAULT_GATEWAY_ETH07	7
#define DEFAULT_GATEWAY_BOND00	10
#define DEFAULT_GATEWAY_BOND01	11
#define DEFAULT_GATEWAY_BOND02	12
#define DEFAULT_GATEWAY_BOND03	13
#define DEFAULT_GATEWAY_WLAN00	20

//*************************************************************************************************************************************
//	Finder defined command id...
//	NOTE: command is represented by unsigned short (2-bytes), so can only from 0 ~ 65535
//*************************************************************************************************************************************
#define FINDER_CMD_ACQUIRE_SERVER			(1)
#define FINDER_CMD_ACQUIRE_CONFIG			(2)
#define FINDER_CMD_AUTHENTICATE				(3)
#define FINDER_CMD_MODIFY_CONFIG			(4)
#define FINDER_CMD_ACQUIRE_DETAILS			(5)
// LEGACY FINDER COMMAND (6~15), RESERVED FOR COMPATIBILITY ISSUES
//#define CmdBurnTestInit					(6)
//#define CmdBurnTestStart					(7)
//#define CmdBurnTestEnd					(8)
//#define CmdBurnTestStop					(9)
//#define CmdNULL						(10)
//#define CmdBurnTestQueryLog					(11)
//#define CmdBurnTestLogNum					(12)
//#define CmdOnLineTest					(13)
//#define CmdShutdown						(14)
//#define CmdCreateRaid						(15)
#define FINDER_CMD_IMPORT_USERS				(16)
#define FINDER_CMD_START_WIZARD_CONFIGURING		(17)	// use to indicate the QSW start changing settings, and stop all services, may take long time...
#define FINDER_CMD_GET_INIT_DISK_PROGRESS		(18)
#define FINDER_CMD_SET_SERVER_NAME			(19)
#define FINDER_CMD_SET_USER_PASSWORD			(20)
#define FINDER_CMD_SET_TIMEZONE				(21)
#define FINDER_CMD_SET_CODE_PAGE			(22)
#define FINDER_CMD_SET_NETWORK				(23)
#define FINDER_CMD_INIT_DISK				(24)
#define FINDER_CMD_REBOOT				(25)
// command 26 is reserved for compatibility issues
#define FINDER_CMD_UPDATE_FIRMWARE			(27)
//#define CmdUpdateFirmTransfer 				(28)
//#define CmdInstallFirm					(29)
#define FINDER_CMD_SET_IPCAM_CONFIG			(30)
#define FINDER_CMD_TEST_IPCAM				(31)
#define FINDER_CMD_PING_TEST				(32)
#define FINDER_CMD_BIND_IP				(33)
#define FINDER_CMD_UPDATE_FIRMWARE_PROGRESS		(34)
#define FINDER_CMD_CLIENT_IP				(35)		// return client PC's ip address...
#define FINDER_CMD_GET_IPCAM_CONFIG			(36)		
#define FINDER_CMD_GET_USER_LIST			(37)	
#define FINDER_CMD_CREATE_USER				(38)		
#define FINDER_CMD_REMOVE_USER				(39)	
#define FINDER_CMD_GET_IPCAM_MODEL_INFO			(40)
#define FINDER_CMD_UPLOADFILE				(41)
#define FINDER_CMD_GET_ISCSI_TARGET_INFO		(42)
	
// ...............................................................................................................................................................................
#define FINDER_CMD_RUN_PROCESS				(200)
#define FINDER_CMD_CHECK_PROCESS			(201)


//*************************************************************************************************************************************
//	Finder defined data tag id...
//	NOTE: Finder protocol use tag to define the data type. Tag is represented by byte (1-byte), so can only from 0 ~ 255
//*************************************************************************************************************************************
#define TAG_NULL					0
#define TAG_SERVER_NAME					1
#define TAG_VERSION					2
#define TAG_DATE					3
#define TAG_TIME					4
#define TAG_ETH00_IP_ADDRESS		5
#define TAG_ETH00_GATEWAY			6
#define TAG_ETH00_SUBNET_MASK		7
#define TAG_ETH00_DHCP				8
#define TAG_WORKGROUP				9
#define TAG_PASSWORD				10
#define TAG_ADMIN_NAME				11
#define TAG_ETH00_MAC_ADDRESS		12
#define TAG_DISK_NUMBER				13		// configurable disk number, originally TagDiskNum
//#define TagEventLogNum			14
//#define TagString					15
#define TAG_REBOOT					16
#define TAG_NEW_PROTOCOL			17
//#define TAG_PPPOE_USER					18
//#define TAG_PPPOE_PASSWORD				19
//#define TAG_PPTP_IP					20
//#define TAG_PPTP_USER					21
//#define TAG_PPTP_PASSWORD				22
#define TAG_ETH01_IP_ADDRESS			23
#define TAG_ETH01_GATEWAY				24
#define TAG_ETH01_SUBNET_MASK			25
#define TAG_ETH01_DHCP					26
#define TAG_ETH01_MAC_ADDRESS			27
//#define TAG_ETH01_DHCP_SERVER				28
//#define TAG_ETH01_DHCP_START				29
//#define TAR_ETH01_DHCP_END				30
#define TAG_IMPORT_USER					31		// BUGBUG: not implemented yet...
#define TAG_IS_CONFIGURED				32
#define TAG_TIME_ZONE					33
#define TAG_INIT_DISK_TYPE				34
#define TAG_CODE_PAGE					35		//add for QIW language setting
//#define TagDiskFinish					36
#define TAG_DISK_VOLUME_STATUS				37
#define TAG_HDD1_MODEL_NAME				38
#define TAG_HDD1_CAPACITY_INFO				39
#define TAG_MODEL_NAME				40
#define TAG_NAS_INTERNAL_MODEL_NAME			41
#define TAG_USE_NTP_SERVER				42
#define TAG_DNS1					43
#define TAG_DNS2					44
#define TAG_BUILD_NUMBER				45
// extension defined after VS-201
#define TAG_HDD2_MODEL_NAME				46
#define TAG_HDD2_CAPACITY_INFO				47
//#define TagIsTS100					48
#define TAG_NTP_SERVER					49
#define TAG_SHUTDOWN					50		// New: for FINDER_CMD_REBOOT
#define TAG_OLD_ADMIN_NAME				51		// Data: string of old admin name, for authentication when CMD_SET_PASSWORD
#define TAG_OLD_PASSWORD				52		// Data: string of old password, for authentication when CMD_SET_PASSWORD
#define TAG_PORT_NUMBER					53		// specify TCP port number...
// for USER management...
#define TAG_USER_NAME					54		// specify user name for user management...
#define TAG_USER_PASSWORD				55		// specify user password for user management...
#define TAG_IS_ADMINISTRATOR				56		// specify if user belongs to system administrators group for user management...
#define	TAG_SHARE_NAME					57		// specify the share name...
#define TAG_HDD3_MODEL_NAME				58
#define TAG_HDD3_CAPACITY_INFO				59
#define TAG_HDD4_MODEL_NAME				60
#define TAG_HDD4_CAPACITY_INFO				61
#define TAG_HDD5_MODEL_NAME				62
#define TAG_HDD5_CAPACITY_INFO				63
#define TAG_HDD6_MODEL_NAME				64
#define TAG_HDD6_CAPACITY_INFO				65
#define TAG_HDD7_MODEL_NAME				66
#define TAG_HDD7_CAPACITY_INFO				67
#define TAG_HDD8_MODEL_NAME				68
#define TAG_HDD8_CAPACITY_INFO				69

// Meiji 2008-09-20: added for multiple network interface model...
#define TAG_NETWORK_INTERFACE_NUMBER		70		// network controller number
#define TAG_NETWORK_CONFIG_MODE		71		// multiple network interface mode: load-balance(1), fail-over(2), standalone(0)

// Jam 2009-03-23: added for changing ftp port
#define TAG_FTPPORT_NUMBER		72		// ftp port

// Kenneth 2010-07-14: added for getting display model name
#define TAG_NAS_DISPLAY_MODEL_NAME		73

// Levi 2011-01-27: added for getting bonding type
#define TAG_NETWORK_BONDING_TYPE		74

// Kenneth 2011-04-13: added for setting default gateway
#define TAG_NETWORK_DEFAULT_GATEWAY		75

// Kenneth 2011-06-09: added for getting serial number
#define TAG_SERIAL_NUMBER		76		// serial number

// Wayne 2011-06-20: added for wireless network
#define TAG_WLAN_INTERFACE_NUMBER		77

// ...
// ...
// ...
// for VS series camera settings: reserved from 78 ~ 200
#define TAG_CAMERA_MODEL_ID				78		// camera model id string
#define TAG_CAMERA_NUMBER				79		// configurable camera number...
#define TAG_CAMERA1_SETTINGS				80
#define TAG_CAMERA2_SETTINGS				81
#define TAG_CAMERA3_SETTINGS				82
#define TAG_CAMERA4_SETTINGS				83
#define TAG_CAMERA5_SETTINGS				84
#define TAG_CAMERA6_SETTINGS				85
#define TAG_CAMERA7_SETTINGS				86
#define TAG_CAMERA8_SETTINGS				87

#define TAG_TEMP_KEY                        88
// AXIMCOM
#define TAG_BATTERY_STATUS                  95
#define TAG_USB_TOTAL_STORAGE               96
#define TAG_SD_TOTAL_STORAGE                97
#define TAG_USB_REMAIN_STORAGE              98
#define TAG_SD_REMAIN_STORAGE               99

// ...
#define	TAG_CAMERA_MODEL_USE_DUSR			101		// Meiji 2007-11-05: added for Sanyo camera to indicate that the admin_password & guest_password is used instead of username & password fields...

/* add for distinguishing NAS or router */
#define TAG_ETH02_IP_ADDRESS	150
#define TAG_ETH03_IP_ADDRESS	151
#define TAG_WLAN00_IP_ADDRESS	156

#define TAG_ETH02_GATEWAY		160
#define TAG_ETH03_GATEWAY		161
#define TAG_WLAN00_GATEWAY		166

#define TAG_ETH02_SUBNET_MASK	170
#define TAG_ETH03_SUBNET_MASK	171
#define TAG_WLAN00_SUBNET_MASK	176

#define TAG_ETH02_DHCP			180
#define TAG_ETH03_DHCP			181
#define TAG_WLAN00_DHCP			186

#define TAG_ETH02_MAC_ADDRESS	190
#define TAG_ETH03_MAC_ADDRESS	191
#define TAG_WLAN00_MAC_ADDRESS	196

// ...
#define TAG_PROGRAM				200
#define TAG_PROTOCOL_VERSION	201

// Wayne 2011-09-30: added for tag extension
#define TAG_EXTENSION			202

#define TAG_WLAN00_SPEED		210

#define TAG_ETH00_SPEED			220
#define TAG_ETH01_SPEED			221
#define TAG_ETH02_SPEED			222
#define TAG_ETH03_SPEED			223

#define TAG_HDD9_MODEL_NAME				230
#define TAG_HDD9_CAPACITY_INFO			231
#define TAG_HDD10_MODEL_NAME			232
#define TAG_HDD10_CAPACITY_INFO			233
#define TAG_HDD11_MODEL_NAME			234
#define TAG_HDD11_CAPACITY_INFO			235
#define TAG_HDD12_MODEL_NAME			236
#define TAG_HDD12_CAPACITY_INFO			237
#define TAG_HDD13_MODEL_NAME			238
#define TAG_HDD13_CAPACITY_INFO			239
#define TAG_HDD14_MODEL_NAME			240
#define TAG_HDD14_CAPACITY_INFO			241
#define TAG_HDD15_MODEL_NAME			242
#define TAG_HDD15_CAPACITY_INFO			243
#define TAG_HDD16_MODEL_NAME			244
#define TAG_HDD16_CAPACITY_INFO			245

// ...
// ...
// ...
#define TAG_INDEX						251
#define TAG_COUNT						252
#define TAG_MAGIC_WORD					253
#define TAG_CHECKSUM					254
#define TAG_OPERATION_RESULT			255

// Wayne 2011-09-30: added extended tags
#define TAG_EXT_IS_FTP_ENABLE				256
#define	TAG_EXT_IS_WEB_ENABLE				257
#define	TAG_EXT_IS_SECURE_WEB_ENABLE		258
#define	TAG_EXT_IS_SECURE_ADMIN_ENABLE		259
#define	TAG_EXT_IS_ISCSI_ENABLE				260

#define TAG_EXT_WEBPORT_NUMBER				261
#define TAG_EXT_SECURE_WEBPORT_NUMBER		262
#define TAG_EXT_SECURE_ADMINPORT_NUMBER		263
#define TAG_EXT_ISCSIPORT_NUMBER			264

// Wayne 2011-10-05: added extended tags for iSCSI operations
#define TAG_EXT_ISCSI_TARGETNAME						265
//#define TAG_EXT_ISCSI_CONFLICT_INFO_REQUIRED			266
//#define TAG_EXT_ISCSI_NUMBER_OF_CONNECTED_INITIATORS	267
//
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN1			268
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN2			269
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN3			270
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN4			271
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN5			272
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN6			273
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN7			274
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN8			275
//// 276~305 reserved for TAG_EXT_ISCSI_CONNECTED_INITIATOR_IQN tag expansion
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP1			306
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP2			307
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP3			308
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP4			309
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP5			310
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP6			311
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP7			312
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP8			313
//// 314~343 reserved for TAG_EXT_ISCSI_CONNECTED_INITIATOR_IP tag expansion
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME1	344
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME2	345
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME3	346
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME4	347
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME5	348
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME6	349
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME7	350
//#define TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME8	351
//// 352~381 reserved for TAG_EXT_ISCSI_CONNECTED_INITIATOR_LOGINTIME tag expansion
#define TAG_EXT_ISCSI_HAS_CONNECTED_INITIATOR			382

//// 383~399 reserved for TAG_EXT_ISCSI tag expansion

// Wayne 2012-02-22: added extended tags for NIC grouping mechanism using zero-based index
#define TAG_EXT_ADVANCED_BONDING_SUPPORT				400
#define TAG_EXT_NIC_GROUP_COUNT							401					
#define TAG_EXT_NIC_GROUP_INFO_0						402
#define TAG_EXT_NIC_GROUP_INFO_1						403					
#define TAG_EXT_NIC_GROUP_INFO_2						404					
#define TAG_EXT_NIC_GROUP_INFO_3						405
#define TAG_EXT_NIC_GROUP_INFO_4						406					
#define TAG_EXT_NIC_GROUP_INFO_5						407					
#define TAG_EXT_NIC_GROUP_INFO_6						408					
#define TAG_EXT_NIC_GROUP_INFO_7						409	

//// 409~451 reserved for TAG_EXT_NIC_GROUP_INFO tag expansion
// Wayne 2012-02-25: added extended tags for NIC grouping mechanism using zero-based index
#define TAG_EXT_ENABLE_AUTO_DNS                   		452
#define TAG_EXT_NIC_BOND_COUNT                   		453
#define TAG_EXT_FINDER_BROADCAST_IP                		454

// Wayne 2012-04-02: added extension tags for NIC speed (10M, 100M, 1G, 10G)
#define TAG_EXT_ETH00_SPEED								455
#define TAG_EXT_ETH01_SPEED								456
#define TAG_EXT_ETH02_SPEED								457
#define TAG_EXT_ETH03_SPEED								458
#define TAG_EXT_ETH04_SPEED								459
#define TAG_EXT_ETH05_SPEED								460
#define TAG_EXT_ETH06_SPEED								461
#define TAG_EXT_ETH07_SPEED								462
//// 463~480 reserved for TAG_EXT_ETH_SPEED tag expansion

#define TAG_EXT_ETH00_DNS1								481
#define TAG_EXT_ETH00_DNS2								482
//// 483~550 reserved for TAG_EXT_ETH_DNS tag expansion

#define TAG_EXT_IS_CONFIGURED							551
#define TAG_EXT_MYCLOUDNAS_NAME                         552
#define TAG_EXT_EXTERNAL_IP_ADDRESS                     553
#define TAG_EXT_DDNS_HOST								554
#define TAG_EXT_ALLOW_NON_ENCRYPTED_AUTHENTICATION      563

//*************************************************************************************************************************************
//	Finder protocol packet format...
//*************************************************************************************************************************************

// Command header of finder packet, struct should be kept in 8bytes alignment.
typedef struct FINDER_CMD_HEADER 
{
	unsigned char	Preamble[8];			// filled in MAC address as premable
	unsigned short	SeqNum;				// sequence number
	unsigned short	Cmd;				// command id
	unsigned short	Result;				// operation results (0 if it's simple request)
	unsigned short	pad;				// pad data, used for special option flag as well.
//}     FINDER_CMD_HEADER;				// Roylin modify for kernel-2.6.30.6-arm
} __attribute__((packed)) FINDER_CMD_HEADER;


// generic data tag format of finder packet, one finder packet can be appended with serveral data tags
typedef struct 
{
	unsigned char tag;
	unsigned char len;
	unsigned char data[MAX_TAG_DATA_SIZE];
} __attribute__((packed)) DATA_TAG;

// Extension Tag struct
typedef struct 
{
	unsigned int tag;
	unsigned char len;
	unsigned char data[EXT_DATA_TAG_MAX_SIZE];
} __attribute__((packed)) EXT_DATA_TAG;

// Finder packet format.
// Struct should be kept in 8bytes alignment
#pragma warning( disable : 4200 70 )
typedef struct FINDER_CMD_PACKET 
{
	FINDER_CMD_HEADER	Cmd_Hdr;		// command header
	DATA_TAG			Tag_List[];		// allow variable data tag list attached...
} __attribute__((packed)) FINDER_CMD_PACKET;
#pragma warning( default : 4200 70 )


// Wayne 2012-03-09: added extended tags for NIC grouping mechanism using zero-based index
// Structure for advance network settings: NIC group info
typedef struct _FINDER_NIC_GROUP_CONTEXT
{
	char			DeviceName[MAX_NIC_DEVICE_NAME_LEN];	// Device Name: eth[0-7] or bond[0-3] for 8 LANs
	unsigned char	MACAddress[6];							// MAC address
	unsigned char	NetSpeed;								// Configured network speed
	unsigned char	BondingMode;							// NIC_MODE_BONDING_ROUNDROBIN, NIC_MODE_BONDING_ACTIVEBACKUP, ...
	unsigned char	MemberCount;							// Number of bonded devices in this group
	unsigned char	Members[MAX_NIC_GROUP_MEMBER_NUM];		// 0-7: eth[0-7]
	unsigned char	UseDHCP;								// 0: DHCP disabled, 1: use DHCP
	unsigned char	IPAddress[4];						// Static IP address shall be set if UseDHCP is zero
	unsigned char	SubnetMask[4];						// Subnet mask shall be set if UseDHCP is zero
	unsigned char	Gateway[4];							// Gateway shall be set if UseDHCP is zero
	unsigned char	UIGroupIndex;
} __attribute__((packed)) FINDER_NIC_GROUP_CONTEXT;



//*************************************************************************************************************************************
//	Utility functions for Finder protocol - function prototype & data structure
//*************************************************************************************************************************************

typedef struct 
{
	DATA_TAG		tags[MAX_SESSION_NODE_TAG_COUNT];	// tag data list
	int				tag_num;							// number of tags
}	TAG_LIST;



// search packet (cmd + tag_buf) and get specific tag data
// the data buffer is provided with length, will check for buffer overflow...
// return written length if found, o/w 0
int GetTagData(FINDER_CMD_PACKET *packet_buf, int packet_buf_len, int tag_id, void *tag_buf, int tag_buf_size);
// search packet (cmd + tag_buf) and get specific tag data
// the data buffer is provided with length, will check for buffer overflow...
// return written length if found, o/w 0
char *GetTagDataString(FINDER_CMD_PACKET *packet_buf, int packet_buf_len, int tag_id, void *tag_buf, int tag_buf_size);


// return true if add successfully...
bool AddTagToTagList(TAG_LIST *pTagList, int tag_id, void *data, int len);
bool AddExtTagToTagList(TAG_LIST *pTagList, int tag_id, void *data, int len);
// return true if add successfully...
// NOTE: this function will do IREV automatically...
bool AddTagIntegerToTagList(TAG_LIST *pTagList, int tag_id, int value);
bool AddExtTagIntegerToTagList(TAG_LIST *pTagList, int tag_id, int value);
bool AddTagIntegerArrayToTagList(TAG_LIST *pTagList, int tag_id, int *value_array, int count);
bool AddExtTagIntegerArrayToTagList(TAG_LIST *pTagList, int tag_id, int *value_array, int count);

// return true if add successfully...
bool AddTagStringToTagList(TAG_LIST *pTagList, int tag_id, const char *szData);
bool AddExtTagStringToTagList(TAG_LIST *pTagList, int tag_id, const char *szData);


// return the data length written in the buf, return 0 if buffer too small...
int TagsToBuffer(DATA_TAG tags[], int tag_cnt, char *buf, int buf_size);
// return the data length written in the buf, return 0 if buffer too small...
int TagListToBuffer(TAG_LIST *tag_list, char *buf, int buf_size);
// Organize all command data (cmd, seq_num, result, tag_buf) into packet_buf for sending out
// Note: cmd & seq_num & result will be handled with SREV() automatically, do no do SREV in advance
int MakeFinderCommandPacket(char *packet_buf, unsigned int packet_buf_size, unsigned char preamble[8], unsigned short cmd, 
	unsigned short seq_num, unsigned short result, const char *tag_buf, unsigned int tag_buf_len);
	

/*
// Meiji: SESSION_NODE is obsolete, don't use it unless you have really good reasons.
typedef struct 
{
	FINDER_CMD_HEADER	ch;					// command header for this session
	DATA_TAG		dt[MAX_SESSION_NODE_TAG_COUNT];		// attached data tags for this session
	int			dtIdx;					// number of attached tags 
	unsigned char		data[MAX_PACKET_SIZE];			// buffer for the final finder data packet
}	SESSION_NODE;


// return true if add successfully...
bool AddTagToSessionNode(SESSION_NODE *pSN, int tag_id, void *data, int len);
bool AddExtTagToSessionNode(SESSION_NODE *pSN, int tag_id, void *data, int len);
// value will be handled with IREV() automatically.
bool AddTagIntegerToSessionNode(SESSION_NODE *pSN, int tag_id, int value);
bool AddExtTagIntegerToSessionNode(SESSION_NODE *pSN, int tag_id, int value);

*/


#endif

