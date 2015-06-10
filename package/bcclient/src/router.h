/*******************************************************************************************************************************
 *
 *	Copyright (c) 2007  QNAP Systems, Inc.  All Rights Reserved.
 *
 *	FILE:
 *		nas.h
 *
 *	Abstract: 
 *		NAS finder operation implementation definitions.
 *		For common finder operation definition, refer to finder.h.
 *		For common device-independent protocol definitions, please put in protocol.h.
 *
 *	HISTORY:
 *		03/26/07	meiji chang re-create
 *
 ********************************************************************************************************************************/

#include <stdio.h> 
#include "protocols.h"
#include "encrypt.h"
#define BUF_LENGTH 256

//router server name
#define ROUTER_SERVER_NAME "AximCom"

#define INSTALL_FLAG_FILE	"/tmp/check_install.flag"
typedef enum {
    ST_INFO_TOTAL = 2,
    ST_INFO_USED = 3,
    ST_INFO_FREE = 4,
} storage_info_enum;

typedef struct ServerDetailInfoTag
{
    int AllowNonEncryptedAuthentication;
    char BatteryStatus[20];
    char SDTotal[20];
    char SDStatus[20];
    char USBTotal[20];
    char USBStatus[20];
    char MachineName[20];
    char ServerName[20];
    char Version[20];
    char Model[16];
    char Date[20];
    char Time[20];
    char lan_hwaddr_rule_default[COMMON_BUFFER_LENGTH];
    char wan_hwaddr_rule_default[COMMON_BUFFER_LENGTH];
    int CGIPort; // 8081 by default      TAG_PORT_NUMBER
    unsigned char wan0_ip[4];
    unsigned char wan0_mask[4];
    unsigned char lan0_ip[4];
    unsigned char lan0_mask[4];
    unsigned char MACAddress[6];
    char myQNAPcloudDeviceName[64];
} ServerDetailInfo, *PServerDetailInfo;

extern int r_lan_num;
extern int r_wan_num;

extern ServerDetailInfo SI, SIV, LAN, LANV;
extern int g_wlan_device_changed;
extern int g_nic_mode;		// Meiji 2008-09-20: for multiple network interfaces: fail-over, load-balance or standalone...
extern unsigned char MACAddr[10], LanMACAddr[10];
extern int g_bonding_type;		// Levi 2011-01-26: for bonding type: "eth0+eth1", "eth2+eth3", "eth0+eth1 & eth2+eth3" or "eth0+eth1+eth2+eth3"...
extern struct in_addr g_last_recv_sock_addr;

int if_get_broadaddr(const char *if_name, struct in_addr *broadaddr);
int query_config_file();
int set_install_flag(int option);
void set_ethernet_info(int ethindex);

int check_admistrator_password(FINDER_CMD_PACKET *src_packet, int src_packet_cnt);

int OnCmdAcquireServer(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdAuthenticate(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdAcquireConfig(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdModifyConfig(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdAcquireDetails(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdReboot(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdSetUserPassword(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdSetServerName(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdSetNetwork(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int OnCmdSetUserPassword(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list);
int query_config_file();
int check_admistrator_password(FINDER_CMD_PACKET *src_packet, int src_packet_cnt);
void Set_Admin_Configured_Status(int status); 
void Write_Log(char *logstr, int logstatus);
//Initial the Finder key list table
void KeyTableInit();
KEY_TYPE *GetKeyByPacket(FINDER_CMD_PACKET *src_packet);
KEY_TYPE *GetNewKeyByPacket(FINDER_CMD_PACKET *src_packet);
void ExpireKeyByPacket(FINDER_CMD_PACKET *src_packet);
int Is_Allow_Non_Encrypted_Authentication();

void mask_str_to_int(char *str);
void mask_int_to_str(int mask, char *str);
void adj_size(float val, char *buf, int blen);
void get_storage_space_info(char *mount_string, storage_info_enum info_type, char *result_buffer, int buffer_size, int space_plus_ro);
void get_storage_status(char *device_name, char *device_status, char *device_total_size);
void get_battery_status(char *battery_status);
