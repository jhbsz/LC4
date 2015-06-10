/*******************************************************************************************************************************
 *
 *	Copyright (c) 2007  QNAP Systems, Inc.  All Rights Reserved.
 *
 *	FILE:
 *		nas.c
 *
 *	Abstract:
 *		Real NAS finder operation implementation functions.
 *
 *	HISTORY:
 *		03/26/07	meiji chang re-create
 *
*********************************************************************************************************************************/
/* Don't use the default type definition from typedef.h. Instead, we use bool def protocols.h*/
#define TYPEDEF_BOOL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/reboot.h>

#include "finder.h"
#include "router.h"
#include "encrypt.h"
#include "dbg.h"

/* ezp-nvram */
#include "bcmnvram.h"
#include "ezp-lib.h"

#define TMP_LEN 256
#define SHORT_BUF_LEN 32

int r_lan_num;
int r_wan_num;

ServerDetailInfo router;

static KEY_TYPE FINDER_key_list[MAX_CONCURRENT_CLIENTS]; 
void mask_str_to_int(char *str)
{
	FILE *ipcalc;
	char buf[BUF_LENGTH];
	char buf2[BUF_LENGTH];
	sprintf( buf, "ipcalc  \"0.0.0.0\" \"%s\" | grep PREFIX | cut -d \"=\" -f 2", str);
	if( (ipcalc = popen(buf, "r")) != NULL )
	{
		fgets(buf2, sizeof(buf2), ipcalc);
		buf2[ strlen(buf2)-1 ] = '\0';
		strcpy(str, buf2);
        pclose(ipcalc);
	}
}

void mask_int_to_str(int mask, char *str)
{
	FILE *ipcalc;
	char buf[BUF_LENGTH];
	char buf2[BUF_LENGTH];
	sprintf( buf, "ipcalc  \"0.0.0.0\" \"%d\" | grep NETMASK | cut -d \"=\" -f 2", mask);
	if( (ipcalc = popen(buf, "r")) != NULL )
	{
		fgets(buf2, sizeof(buf2), ipcalc);
		strncpy(str, buf2, strlen(buf2)-1);
        pclose(ipcalc);
	}
}


int OnCmdAcquireServer(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    if (src_packet != NULL) {
        char *recv_ip_str = inet_ntoa(g_last_recv_sock_addr);
        if (recv_ip_str) {
            AddExtTagStringToTagList(lp_result_tag_list, TAG_EXT_FINDER_BROADCAST_IP, recv_ip_str);
        }
    }
    query_config_file();

    //add tag to tag_list ( QFinder must need these information)

    // CGI port
    AddTagIntegerToTagList(lp_result_tag_list, TAG_PORT_NUMBER, router.CGIPort); 
    // ssid
    AddTagStringToTagList(lp_result_tag_list, TAG_SERVER_NAME, router.ServerName );
    // model
    AddTagStringToTagList(lp_result_tag_list, TAG_MODEL_NAME, router.Model);
    // check if allow encrypted
    AddExtTagIntegerToTagList(lp_result_tag_list, TAG_EXT_ALLOW_NON_ENCRYPTED_AUTHENTICATION, router.AllowNonEncryptedAuthentication);
    // firmware version
    AddTagToTagList(lp_result_tag_list, TAG_VERSION, router.Version, 8);
    AddTagToTagList(lp_result_tag_list, TAG_TIME, router.Time, 6);
    AddTagToTagList(lp_result_tag_list, TAG_DATE, router.Date, 8);
    // battery status
    AddTagStringToTagList(lp_result_tag_list, TAG_BATTERY_STATUS, router.BatteryStatus );
    // storage status
    AddTagStringToTagList(lp_result_tag_list, TAG_USB_TOTAL_STORAGE, router.USBTotal );
    AddTagStringToTagList(lp_result_tag_list, TAG_SD_TOTAL_STORAGE, router.SDTotal );
    AddTagStringToTagList(lp_result_tag_list, TAG_USB_REMAIN_STORAGE, router.USBStatus );
    AddTagStringToTagList(lp_result_tag_list, TAG_SD_REMAIN_STORAGE, router.SDStatus );
    // MAC
    AddTagToTagList(lp_result_tag_list, TAG_ETH00_MAC_ADDRESS, MACAddr, 6);
    
    // myqnapcloud
    AddExtTagStringToTagList(lp_result_tag_list, TAG_EXT_MYCLOUDNAS_NAME, router.myQNAPcloudDeviceName );

    return SUCCESS;
}

int OnCmdAuthenticate(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    query_config_file();
    char username[BUF_LENGTH], pwd[BUF_LENGTH];
    int name_len, pwd_len;

    memset(username, 0, sizeof(username));
    memset(pwd, 0, sizeof(pwd));
    name_len = GetTagData(src_packet, src_packet_cnt, TAG_ADMIN_NAME, username, sizeof(username));
    pwd_len = GetTagData(src_packet, src_packet_cnt, TAG_PASSWORD, pwd, sizeof(pwd));

    if (name_len == 0 && pwd_len == 0) {
        KEY_TYPE *key;
        if (src_packet != NULL) {
            char *recv_ip_str = inet_ntoa(g_last_recv_sock_addr);
            if (recv_ip_str) {
                AddExtTagStringToTagList(lp_result_tag_list, TAG_EXT_FINDER_BROADCAST_IP, recv_ip_str);
            }
        }
        ExpireKeyByPacket(src_packet); //expire the previous key (if any) to close previous session for this client
        
        key=GetNewKeyByPacket(src_packet);

        if(key!=NULL) {
            //device name : router
            AddTagIntegerArrayToTagList(lp_result_tag_list, TAG_TEMP_KEY, (int *)key->key, KEY_LENGTH32);
            return SUCCESS;
        } else {
            return ERROR_AUTHENTICATION; // no more key for this Finder, possible DOS attack?
        }
    }

    int ret = check_admistrator_password(src_packet, src_packet_cnt);

    return ret;
}

int OnCmdAcquireConfig(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    query_config_file();   
    // ssid or servername 
    AddTagStringToTagList(lp_result_tag_list, TAG_SERVER_NAME , router.ServerName );
    // network settings...
    AddTagToTagList(lp_result_tag_list, TAG_ETH00_IP_ADDRESS, router.lan0_ip , sizeof(router.lan0_ip) );
    AddTagToTagList(lp_result_tag_list, TAG_ETH00_SUBNET_MASK, router.lan0_mask , sizeof(router.lan0_mask) );
    // time
    AddTagToTagList(lp_result_tag_list, TAG_DATE, router.Date , 8);
    AddTagToTagList(lp_result_tag_list, TAG_TIME, router.Time, 6);

    return SUCCESS;
}

// Common system setting change handling function...
int OnCmdModifyConfig(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    if (check_admistrator_password(src_packet, src_packet_cnt) != 0)    // verified credential, this is previledge operation...
        return ERROR_AUTHENTICATION;

    char buf[COMMON_BUFFER_LENGTH];
    char time[COMMON_BUFFER_LENGTH];
    memset(buf, 0, sizeof(buf));
    GetTagData(src_packet, src_packet_cnt, TAG_SERVER_NAME, buf, sizeof(buf));

    memset(buf, 0, sizeof(buf));
    GetTagData(src_packet, src_packet_cnt, TAG_DATE, buf, sizeof(buf));
    if( buf != NULL )
        strcpy(time,buf);

    memset(buf, 0, sizeof(buf));
    GetTagData(src_packet, src_packet_cnt, TAG_TIME, buf, sizeof(buf));
    if( buf != NULL ) {
        strcat(time,buf);
        memset(buf, 0, sizeof(buf));
        sprintf( buf, "date -s \"%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c\"", time[4], time[5], time[6], time[7], time[0], time[1]    , time[2], time[3], time[8], time[9], time[10], time[11], time[12], time[13] );
        system( buf );
    }
    unsigned char ip[4];
    unsigned char sub_mask[4];
    GetTagData(src_packet, src_packet_cnt, TAG_ETH00_IP_ADDRESS , ip , sizeof(ip));
    GetTagData(src_packet, src_packet_cnt, TAG_ETH00_SUBNET_MASK , sub_mask , sizeof(sub_mask));
    if( ip != NULL ) {
        char saddr[16];
        sprintf(saddr, "%d.%d.%d.%d", ip[0] , ip[1], ip[2], ip[3]);
        nvram_set("lan0_ipaddr", saddr);
        ezplib_replace_attr("lan_static_rule", 0, "ipaddr", saddr);
    }
    if( sub_mask != NULL ) {
        char maddr[16];
        sprintf(maddr, "%d.%d.%d.%d", sub_mask[0] , sub_mask[1], sub_mask[2], sub_mask[3]);
        mask_str_to_int(maddr);
        nvram_set("lan0_mask", maddr);
        ezplib_replace_attr("lan_static_rule", 0, "mask", maddr);
    }
    nvram_commit();
    unsigned char reboot_router = 0;
    GetTagData(src_packet, src_packet_cnt, TAG_REBOOT, &reboot_router, sizeof(reboot_router));

    if( reboot_router )
        reboot(RB_AUTOBOOT);
   
    return SUCCESS;
}


int OnCmdAcquireDetails(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    query_config_file();
    // ssid
    AddTagStringToTagList(lp_result_tag_list, TAG_SERVER_NAME, router.ServerName );
    // model
    AddTagStringToTagList(lp_result_tag_list, TAG_MODEL_NAME, router.Model);
    // firmware version
    AddTagToTagList(lp_result_tag_list, TAG_VERSION, router.Version, 8);
    // network settings...
    AddTagToTagList(lp_result_tag_list, TAG_ETH00_IP_ADDRESS, router.lan0_ip , sizeof(router.lan0_ip) );
    AddTagToTagList(lp_result_tag_list, TAG_ETH00_SUBNET_MASK, router.lan0_mask , sizeof(router.lan0_mask) );

    return SUCCESS;
}

int OnCmdSetServerName(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    if (check_admistrator_password(src_packet, src_packet_cnt) != 0)    // verified credential, this is previledge operation...
        return ERROR_AUTHENTICATION;
    
    char buf[COMMON_BUFFER_LENGTH];
    memset(buf, 0, sizeof(buf));
    GetTagData(src_packet, src_packet_cnt, TAG_SERVER_NAME, buf, sizeof(buf));
    nvram_set("wl0_ssid_rule", buf);
    nvram_commit();

    return SUCCESS;
}

int OnCmdSetNetwork(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    if (check_admistrator_password(src_packet, src_packet_cnt) != 0)    // verified credential, this is previledge operation...
        return ERROR_AUTHENTICATION;

    unsigned char ip[4];
    /* get lanip and mask setting form client */
    GetTagData(src_packet, src_packet_cnt, TAG_ETH00_IP_ADDRESS , ip , sizeof(ip));
    /* set lanip and mask */
    if( ip != NULL ) {
        char saddr[16];
        sprintf(saddr, "%d.%d.%d.%d", ip[0] , ip[1], ip[2], ip[3]);
        nvram_set("lan0_ipaddr", saddr);
        ezplib_replace_attr("lan_static_rule", 0, "ipaddr", saddr);
    }
    nvram_commit();
    return SUCCESS;
}

int OnCmdReboot(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    system("reboot -d 5 &");
    return SUCCESS;
}

int OnCmdSetUserPassword(FINDER_CMD_PACKET *src_packet, int src_packet_cnt, TAG_LIST *lp_result_tag_list)
{
    int result = ERROR_BAD_PARAMETERS;  // default return code if no parameters...
    char data[256];
    char user[MAXIMUM_USER_NAME_BUFFER_LENGTH], pwd[MAXIMUM_PASSWORD_BUFFER_LENGTH];
    char router_user[MAXIMUM_USER_NAME_BUFFER_LENGTH], router_pwd[MAXIMUM_PASSWORD_BUFFER_LENGTH];

    memset(user, 0, sizeof(user));
    memset(pwd, 0, sizeof(pwd));
    memset(router_user, 0, sizeof(router_user));
    memset(router_pwd, 0, sizeof(router_pwd));
    memset(data, 0, sizeof(data));

    /////////////////////////////////////////////////////////
    //              Try encrypt path                       //
    /////////////////////////////////////////////////////////
        bool input_value_verify_ok = false;
        KEY_TYPE *key = GetKeyByPacket(src_packet);
        unsigned char *plain_text = NULL;
        int plain_text_length = 0, new_user_length = 0;
        
    if (key != NULL) 
    {
        // perform authentication on old username & password of administartor...
        if (!GetTagData(src_packet, src_packet_cnt, TAG_OLD_ADMIN_NAME, data, sizeof(data))) return ERROR_AUTHENTICATION;
        plain_text_length = get_decrypt_output_length(data);
        plain_text = (unsigned char*) malloc(plain_text_length*sizeof(unsigned char));
        memset(plain_text, 0, plain_text_length*sizeof(unsigned char));
        _to_decrypt_str(key->key, USERNAME_KEY, data, sizeof(data), plain_text, plain_text_length);
        strncpy(user, (char*)plain_text, sizeof(user));
        free(plain_text);

        if (!GetTagData(src_packet, src_packet_cnt, TAG_OLD_PASSWORD, data, sizeof(data))) return ERROR_AUTHENTICATION;
        plain_text_length = get_decrypt_output_length(data);
        plain_text = (unsigned char*) malloc(plain_text_length*sizeof(unsigned char));
        memset(plain_text, 0, plain_text_length*sizeof(unsigned char));
        _to_decrypt_str(key->key, PASSWORD_KEY, data, sizeof(data), plain_text, plain_text_length);
        strncpy(pwd, (char*)plain_text, sizeof(pwd));
        free(plain_text);

        ezplib_get_attr_val("http_rule", 0, "username", router_user, sizeof(router_user), 0);
        ezplib_get_attr_val("http_rule", 0, "passwd", router_pwd, sizeof(router_pwd), 0);

        if ( strcmp(user, router_user) || strcmp(pwd, router_pwd))
            return ERROR_AUTHENTICATION;  // authentication error again, return error
 
        GetTagData(src_packet, src_packet_cnt, TAG_ADMIN_NAME, data, sizeof(data));
        memset(user, 0, sizeof(user));
        plain_text_length = get_decrypt_output_length(data);
        plain_text = (unsigned char*) malloc(plain_text_length*sizeof(unsigned char));
        memset(plain_text, 0, plain_text_length*sizeof(unsigned char));
        _to_decrypt_str(key->key, USERNAME_KEY, data, sizeof(data), plain_text, plain_text_length);
        strncpy(user, (char *)plain_text, MAXIMUM_USER_NAME_BUFFER_LENGTH);
        new_user_length = strlen((char*)plain_text);
        free(plain_text);
        
        if (new_user_length > 0)
        {
            if (GetTagData(src_packet, src_packet_cnt, TAG_PASSWORD, data, sizeof(data)))
            {
                memset(pwd, 0, sizeof(pwd));
                plain_text_length = get_decrypt_output_length(data);
                plain_text = (unsigned char*) malloc(plain_text_length*sizeof(unsigned char));
                memset(plain_text, 0, plain_text_length*sizeof(unsigned char));
                _to_decrypt_str(key->key, PASSWORD_KEY, data, sizeof(data), plain_text, plain_text_length);
                strncpy(pwd, (char *)plain_text, MAXIMUM_PASSWORD_BUFFER_LENGTH);
                free(plain_text);
                input_value_verify_ok=true;
            }
            else
            {
                input_value_verify_ok=false;
            }
        }
        else
        {
            input_value_verify_ok=false;
        }
        ExpireKeyByPacket(src_packet);
    }
    else
        input_value_verify_ok=false;

    if (strlen(user))
    {
        if (strlen(pwd))
        {
            ezplib_replace_attr("http_rule", 0, "username", user);
            ezplib_replace_attr("http_rule", 0, "passwd", pwd);
            nvram_commit();
            result = SUCCESS;
        }
    }

    return result;
}

int query_config_file()
{
    // set device name
    strcpy(router.MachineName, "router");
	// get network status...
    r_lan_num =  atoi(nvram_get("lan_num"));	
    r_wan_num =  atoi(nvram_get("wan_num"));

    for( int i=0 ; i < r_lan_num ; i++ )
    {
        int mask = 0;
        char buf[BUF_LENGTH];
        switch(i)
        {
            case 0:
                ip_str_to_bytes( nvram_get("lan0_ipaddr") ,router.lan0_ip);
                mask = atoi( nvram_get("lan0_mask") );
                mask_int_to_str( mask, buf);
                ip_str_to_bytes( buf ,router.lan0_mask);
                break;
        }
    }
/*
    for( int i=0 ; i < r_wan_num ; i++ )
    {
        int mask = 0;
        char buf[BUF_LENGTH];
        switch(i)
        {
            case 0:
                ip_str_to_bytes( nvram_get("wan0_ipaddr") ,router.wan0_ip);
                mask = atoi( nvram_get("wan0_mask") );
                mask_int_to_str( mask, buf);
                ip_str_to_bytes( buf ,router.wan0_mask);
                break;
        }
    }
*/
    // add ServerName
    char buf[20], str[20];
    ezplib_get_attr_val("wl0_ssid_rule", 0, "ssid", buf, sizeof(buf), 0);
    if( buf != NULL )
        strcpy( router.ServerName , buf );
    else
        strcpy( router.ServerName , ROUTER_SERVER_NAME );

    //version : code from Qnap
    strcpy(str, nvram_get("fw_version"));
    router.Version[0]  = str[0] - '0';
    router.Version[0]  = (router.Version[0] << 4) ;
    router.Version[0]  |= (str[2] - '0');
    router.Version[1]  = str[3] - '0';
    strncpy((char*)(router.Version + 2), str + 4, sizeof(str));

    // add model
    strcpy( router.Model, nvram_get("model"));

    // add time & date
    time_t dat = time(NULL);
    strftime( router.Date , sizeof(router.Date) , "%m%d%Y" ,localtime(&dat) );
    time_t tim = time(NULL);
    strftime( router.Time , sizeof(router.Time) , "%H%M%S" ,localtime(&tim) );

    // CGI port
    router.CGIPort = 8081;
    // check encrypt status
    if (Is_Allow_Non_Encrypted_Authentication())
        router.AllowNonEncryptedAuthentication = 1;
    else
        router.AllowNonEncryptedAuthentication = 0;
    
    // add myQNAPcloudDeviceName
    char myqnapcloud_buf[64], myqnapcloud_site_buf[256];
    char myqnapcloud_site_production[]=".myqnapcloud.com";
    char myqnapcloud_site_alpha[]=".alpha-myqnapcloud.com";
    int lic_key = atoi(nvram_get("license_invalid"));
    ezplib_get_attr_val("myqnapcloud_id_rule", 0, "device_name", myqnapcloud_buf, sizeof(myqnapcloud_buf), 0);
    if (myqnapcloud_buf[0] != 0){
            if (lic_key == 0) {
                    sprintf(myqnapcloud_site_buf, "%s%s", myqnapcloud_buf, myqnapcloud_site_production);
                    strcpy( router.myQNAPcloudDeviceName , myqnapcloud_site_buf );
            } else {
                    sprintf(myqnapcloud_site_buf, "%s%s", myqnapcloud_buf, myqnapcloud_site_alpha);
                    strcpy( router.myQNAPcloudDeviceName , myqnapcloud_site_buf );
            }
    }

    // get battery status
    get_battery_status(router.BatteryStatus);

    // get storage status
    get_storage_status("USB", router.USBStatus, router.USBTotal);
    get_storage_status("SD", router.SDStatus, router.SDTotal);

    return 1;
}
char user_name[MAXIMUM_USER_NAME_BUFFER_LENGTH];
int check_admistrator_password(FINDER_CMD_PACKET *src_packet, int src_packet_cnt)
{
    char buf1[BUF_LENGTH], buf2[BUF_LENGTH];
    char buf3[BUF_LENGTH], buf4[BUF_LENGTH];
    memset(buf1, 0, sizeof(buf1));
    memset(buf2, 0, sizeof(buf2));
    memset(buf3, 0, sizeof(buf3));
    memset(buf4, 0, sizeof(buf4));

    // receive QFinder's username & passwd
    GetTagData(src_packet, src_packet_cnt, TAG_ADMIN_NAME, buf1, sizeof(buf1));
    GetTagData(src_packet, src_packet_cnt, TAG_PASSWORD, buf2, sizeof(buf2));

    // get router's username & passwd
    ezplib_get_attr_val("http_rule", 0, "username", buf3, sizeof(buf3), 0);
    ezplib_get_attr_val("http_rule", 0, "passwd", buf4, sizeof(buf4), 0);
	
    KEY_TYPE *key = GetKeyByPacket(src_packet);
    unsigned char *plain_text = NULL;
    int plain_text_length = 0;
    char pwd[MAXIMUM_PASSWORD_BUFFER_LENGTH];

    if (key==NULL) return ERROR_AUTHENTICATION; //no valid key, reject

    memset(pwd, 0, sizeof(pwd));
    DBG("check_admistrator_password: buf1=%s...\n", buf1);
    plain_text_length = get_decrypt_output_length(buf1);
    plain_text = (unsigned char*) malloc(plain_text_length*sizeof(unsigned char));
    memset(plain_text, 0, plain_text_length*sizeof(unsigned char));
    _to_decrypt_str(key->key, USERNAME_KEY, buf1, sizeof(buf1), plain_text, plain_text_length);
    strncpy(user_name, (char*)plain_text, sizeof(user_name));
    free(plain_text);

    DBG("check_admistrator_password: buf2=%s...\n", buf2);
    plain_text_length = get_decrypt_output_length(buf2);
    plain_text = (unsigned char*) malloc(plain_text_length*sizeof(unsigned char));
    memset(plain_text, 0, plain_text_length*sizeof(unsigned char));
    _to_decrypt_str(key->key, PASSWORD_KEY, buf2, sizeof(buf2), plain_text, plain_text_length);
    strncpy(pwd, (char*)plain_text, sizeof(pwd));
    free(plain_text);

    // compare username & passwd
    if( (strcmp(user_name,buf3) == 0) && (strcmp(pwd,buf4) == 0) ) {
        return SUCCESS;
    } else {
        return ERROR_BAD_PARAMETERS;
    }
}

void Write_Log(char *logstr, int logstatus)
{
    fprintf(stderr, "%s(%d) %d %s\n", __func__, __LINE__, logstatus, logstr);
}

// Initial the Finder key list table
void KeyTableInit()
{
    int i;
    for(i=0;i<MAX_CONCURRENT_CLIENTS;i++)
    {
        FINDER_key_list[i].refresh_time.tv_sec=0;
        FINDER_key_list[i].refresh_time.tv_usec=0;
        FINDER_key_list[i].mac_hash=0;
    }
}

// Get the key by packet
// return NULL when not found
KEY_TYPE *GetKeyByPacket(FINDER_CMD_PACKET *src_packet)
{
    KEY_TYPE *ret = NULL;
    int i;
    struct timeval tv;
    struct timezone tz;
    unsigned long long src_mac = 0;
    char mac_buf[8];

    if(src_packet==NULL) return NULL;
       
    memset(mac_buf,0,8);
    memcpy(mac_buf,src_packet->Cmd_Hdr.Preamble,6); //get only the MAC address files but drop "Se/Cl" in the last two bytes
    memcpy((char *)&src_mac, mac_buf, 8); 
    DBG("GetKeyByPacket: src_mac=%u...\n", src_mac);

    gettimeofday(&tv,&tz);

    for(i=0;i<MAX_CONCURRENT_CLIENTS;i++)
    {
        if(tv.tv_sec > (FINDER_key_list[i].refresh_time.tv_sec + KEY_TABLE_AGING_SEC)) {
                       continue; //skip the expired entries
        }

        DBG("GetKeyByPacket: FINDER_key_list[%d].mac_hash=%u...\n", i, FINDER_key_list[i].mac_hash);
        if(FINDER_key_list[i].mac_hash == src_mac) {
            FINDER_key_list[i].refresh_time.tv_sec = tv.tv_sec;
            FINDER_key_list[i].refresh_time.tv_usec = tv.tv_usec;
            ret=&FINDER_key_list[i];
            break;
        }
    }
    return ret;
}

// Get new key for packet and store in table
// return NULL if no available entry now
KEY_TYPE *GetNewKeyByPacket(FINDER_CMD_PACKET *src_packet)
{
    KEY_TYPE *ret = NULL;
    int i;
    struct timeval tv;
    struct timezone tz;             
    char mac_buf[8];    
    unsigned long long src_mac=0;

    memset(mac_buf,0,8);
    memcpy(mac_buf,src_packet->Cmd_Hdr.Preamble,6); //get only the MAC address files but drop "Se/Cl" in the last two bytes
    memcpy((char *)&src_mac, mac_buf, 8);

    gettimeofday(&tv,&tz);

    for(i=0;i<MAX_CONCURRENT_CLIENTS;i++)
    {
        if(FINDER_key_list[i].mac_hash == src_mac) { //found entry with matched MAC address
            if(tv.tv_sec > (FINDER_key_list[i].refresh_time.tv_sec + KEY_TABLE_AGING_SEC)) {       
                // reuse this entry with new key
                get_16byte_key(FINDER_key_list[i].key);
            }       
                    
            FINDER_key_list[i].refresh_time.tv_sec = tv.tv_sec;
            FINDER_key_list[i].refresh_time.tv_usec = tv.tv_usec;
            ret=&FINDER_key_list[i];
            break;
        }
    }

    if(ret==NULL) {
        for(i=0;i<MAX_CONCURRENT_CLIENTS;i++)
        {       
            if(tv.tv_sec > (FINDER_key_list[i].refresh_time.tv_sec + KEY_TABLE_AGING_SEC)) {       
                // use  first expired entry
                get_16byte_key(FINDER_key_list[i].key);
                FINDER_key_list[i].refresh_time.tv_sec = tv.tv_sec;
                FINDER_key_list[i].refresh_time.tv_usec = tv.tv_usec;
                FINDER_key_list[i].mac_hash = src_mac;
                ret=&FINDER_key_list[i];
                break;
            }
        }
    }
    // If still NULL, means there is no empty slot
    return ret;
}


// Expire key entry for packat (after real function call)
// 
void ExpireKeyByPacket(FINDER_CMD_PACKET *src_packet)
{
    int i;
    char mac_buf[8];
    unsigned long long src_mac=0;

    if(src_packet == NULL) return;

    memset(mac_buf, 0, 8);
    memcpy(mac_buf, src_packet->Cmd_Hdr.Preamble, 6); //get only the MAC address files but drop "Se/Cl" in the last two bytes
    memcpy((char *)&src_mac, mac_buf, 8);   
    for(i=0;i<MAX_CONCURRENT_CLIENTS;i++)
    {
        if(FINDER_key_list[i].mac_hash == src_mac) { //found entry with matched MAC address
            FINDER_key_list[i].refresh_time.tv_sec = 0;
            FINDER_key_list[i].refresh_time.tv_usec = 0;
            break;
        }
    }
}

int Is_Allow_Non_Encrypted_Authentication()
{
    return 0;
}

void
get_battery_status(char *battery_status)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    system("/usr/sbin/ezp-i2c gauge percentage");
    if ((file = fopen("/tmp/gauge_percentage", "r"))) {
        fgets(tmp, sizeof(tmp), file);
        strcpy(battery_status, tmp);
        fclose(file);
    } else {
        strcpy(battery_status, "N/A");
    }
}
void
get_storage_status(char *device_name, char *device_status, char *device_total_size)
{
    FILE *f;
    uint64_t free_size, size, use_size;
    char buf[TMP_LEN], tmp2[TMP_LEN];
    char storage_size[SHORT_BUF_LEN],storage_usize[SHORT_BUF_LEN],
         storage_upercentage[SHORT_BUF_LEN],storage_fsize[SHORT_BUF_LEN];

    char path[SHORT_BUF_LEN],f_size[SHORT_BUF_LEN],dir[SHORT_BUF_LEN]={0};
    char storage_state[SHORT_BUF_LEN];
    int device_id = 0; //default USB=0

    if(strcmp("SD",device_name)==0)
        device_id=1;
    else if(strcmp("USB",device_name)==0)
        device_id=0;

    /* Storage infomation */
    ezplib_get_attr_val("storage_state_rule", device_id, "mount_state",
                        storage_state, sizeof(storage_state), EZPLIB_USE_CLI);
    if (strcmp(storage_state, "mounted") == 0) {
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf),
                "/sbin/getsysinfo disk_info %s", device_name);
        if ((f = popen(buf, "r"))) {
            fscanf(f, "%s %s %s %s %s %s", path, storage_size, storage_usize,
                   f_size, storage_upercentage, dir);
            if ((strlen(dir)) > 0) {
                size = atoi(storage_size);
                use_size = atoi(storage_usize);
            } else {
                size = atoi(path);
                use_size = atoi(storage_size);
                strcpy(storage_upercentage, f_size);
            }
            /* adjust storage total size */
            adj_size((float)size, storage_size, sizeof(storage_size));
            free_size = size - use_size;
            strcpy(device_total_size, storage_size);
            /* adjust storage remain size, write to device_status */
            adj_size((float)free_size, storage_fsize, sizeof(storage_fsize));
            strcpy(device_status, storage_fsize);
            pclose(f);

            get_storage_space_info(device_name, 4, tmp2, sizeof(tmp2), 0);
            /*  device is read only, write to device status */
            if(!strcmp(tmp2, "Read Only")) {
                strcpy(device_status, tmp2);
            }
        }
    } else {
        strcpy(device_total_size, "N/A");
        strcpy(device_status, "N/A");
    }

    get_storage_space_info(device_name, 4, tmp2, sizeof(tmp2), 0);
    /* if device is read only, show in device status */
    if(!strcmp(tmp2, "Read Only")) {
        strcpy(device_status, tmp2);
    }
}
void
get_storage_space_info(char *mount_string, storage_info_enum info_type, char *result_buffer, int buffer_size, int space_plus_ro) {
    char tmp3[TMP_LEN]={0}, tmp4[TMP_LEN]={0};
    FILE *file, *file2;
    snprintf(tmp3, sizeof(tmp3), "mount | grep %s", mount_string);
    if ((file2 = popen(tmp3, "r"))) {
        fgets(tmp4, sizeof(tmp4), file2);
    }
    pclose(file2);
    /* The df format is 
     * /dev/sdb1              3944740   3182596    762144 81% /tmp/mnt/USB 
     * $1                     $2:total  $3:used    $4:free
     * */
    if (strcmp(tmp4, "")) {
        if (space_plus_ro || strstr(tmp4, "rw")) {
            snprintf(tmp3, sizeof(tmp3), "df | grep %s | awk '{print $%d}'", mount_string, info_type);
            if ((file = popen(tmp3, "r"))) {
                if(space_plus_ro && (!strstr(tmp4, "rw"))) {
                    snprintf(result_buffer, buffer_size, "Read Only");
                } else {
                    snprintf(result_buffer, buffer_size, "Read/Write");
                }
                pclose(file);
            } else {
                snprintf(result_buffer, buffer_size, "Not Available");
            }
        } else {
            snprintf(result_buffer, buffer_size, "Read Only");
        }
    } else {
        snprintf(result_buffer, buffer_size, "Not Available");
    }
}
void
adj_size(float val, char *buf, int blen)
{
    if (val < 1024) {
        snprintf(buf, blen,"%.1fKB", val);
    } else {
        val = val / 1024;
        snprintf(buf, blen,"%.1fMB", val);
    }
    return;
}

