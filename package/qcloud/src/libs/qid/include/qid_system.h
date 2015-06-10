/*
 * qid_system.h
 *
 */

#ifndef QID_SYSTEM_H_
#define QID_SYSTEM_H_

#include "const.h"

// the ramdisk folder path to store cache
#define QCLOUD_VAR_CACHE_FOLDER         "/tmp/.qcloud-vars-cache"


#define DDNS_DOMAIN_NAME_LEN        (64)
#define DDNS_SERVER_NAME_LEN        (64)
#define LAN_IP_LIST_LEN             (512)
#define MAX_IP_NUM                  (8)
#define IP_VERSION_LEN              (8)
#define IP_VERSION6                     "ip_v6"
#define IP_VERSION4                     "ip_v4"

/* command strings*/
#define QID_SYS_ENABLE_STR              "Enable"
#define QID_SYS_INIT_STR                "Init"
#define QID_SYS_DISABLE_STR             "Disable"
#define QID_SYS_TRUE_STR                "TRUE"
#define QID_SYS_FALSE_STR               "FALSE"

#define QNAP_EVENT_TYPE_INFO   0
#define QNAP_EVENT_TYPE_WARN   1
#define QNAP_EVENT_TYPE_ERROR  2

#define MAX_SYSTEM_CMD                (512)

/*/etc/config/qid.conf related*/
#define GET_QID_SERVER_HOST   "/sbin/getcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"HOST\" -d \"\""
#define GET_MCN_SERVER_HOST   "/sbin/getcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"MCN_HOST\" -d \"\""
#define GET_USHER_SERVER_HOST "/sbin/getcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"USHER_HOST\" -d \"\""
#define SET_QID_AUTH_TOKEN "/sbin/setcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"AUTH TOKEN\" \"%s\""
#define GET_QID_AUTH_TOKEN "/sbin/getcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"AUTH TOKEN\" -d \"\""
#define SET_QID_REFRESH_TOKEN "/sbin/setcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"REFRESH_TOKEN\" \"%s\""
#define GET_QID_REFRESH_TOKEN "/sbin/getcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"REFRESH_TOKEN\" -d \"\""
#define SET_QID_VALID_PERIOD "/sbin/setcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"TOKEN_VALID_PERIOD\" \"%s\""
#define GET_QID_VALID_PERIOD "/sbin/getcfg -f /etc/config/qid.conf \"QNAP ID Service\" \"TOKEN_VALID_PERIOD\" -d \"\""

#define RESTART_PORTER      "/etc/rc.d/S99porter restart &> /dev/null"

#define GET_GATEWAY_IF      "netstat -rn |awk '{if($1==\"0.0.0.0\"&&$4==\"UG\") print $8}'"

/* API digest releated. */
#define HASH_PATH QCLOUD_VAR_CACHE_FOLDER "/hash"
#define GET_QID_REQ_HASH "/bin/cat " HASH_PATH "/request_hash_%s 2>/dev/null"
#define SET_QID_REQ_HASH "/bin/echo -n '%s' > " HASH_PATH "/request_hash_%s"
#define GET_QID_REQ_HASH_TTL "/bin/cat " HASH_PATH "/request_hash_ttl_%s 2>/dev/null"
#define SET_QID_REQ_HASH_TTL "/bin/echo -n '%d' > " HASH_PATH "/request_hash_ttl_%s"

enum
{
  qidDB_qid = 0,
  qidDB_local_name,
  num_qidDB
};

enum
{
  qid_conf_ddns_last_check_time = 0,
  qid_conf_ddns_last_update_time,
  qid_conf_ddns_last_update_status_code,
  qid_conf_ddns_next_check_time
};

typedef struct _lan_ip_info
{
  int lan_ip_count;
  char lan_ip[MAX_IP_NUM][INET6_ADDRSTRLEN];
  char lan_ip_version[MAX_IP_NUM][IP_VERSION_LEN];
  char interface[MAX_IP_NUM][IP_VERSION_LEN];
  char lan_gw_interface[IP_VERSION_LEN];
  /*
   * the maximum lenght: 8*15+7
   * 192.168.68.100,192.168.68.101,...,192.168.68.107
   */
  char lan_ip_list[LAN_IP_LIST_LEN+1];
  char lan_gw[IP_STR_LEN+1];          // save the LAN IP which is bound at UG interface.
}lan_ip_info_st;

#ifdef __cplusplus
extern "C" {
#endif

int qid_sys_get_local_ip_list(lan_ip_info_st *lan_info);

int qid_sys_get_hw_serial_number(char *serial_num, size_t buff_len);

int qid_sys_get_suid(char *suid, size_t buff_len);

int qid_sys_get_license_invalid(char* license_invalid, size_t buff_len);

int qid_sys_get_mac(char *mac_str, size_t buff_len);

int qid_sys_get_nassid( char *nassid, size_t buff_len);

int qid_send_sys_event_log(unsigned int level, char *prev_msg, char* msg);

int qid_sys_set_qid(char *qid);

int qid_sys_get_qid(char *qid, size_t buff_len);

int qid_sys_set_auth_token(char *qid_auth_token);

int qid_sys_get_auth_token(char *token, size_t buff_len);

int qid_sys_get_model(char *mode, size_t buff_len);

int qid_sys_get_fw_ver(char *fw_ver, size_t buff_len);

int qid_sys_get_server_host(int is_production, char *host, size_t buff_len);

int qid_sys_get_mcn_server_host(int is_production, char *host, size_t buff_len);

int qid_sys_get_usher_server_host(int is_production, char *host, size_t buff_len);

int qid_sys_set_device_name(char *name);

int qid_sys_get_device_name (char *name, size_t buff_len);

int qid_sys_get_device_type(char *type, size_t buff_len);

int qid_sys_get_qid_status(char *status, size_t buff_len);

int qid_sys_set_qid_status(char *status);

int qid_sys_set_qid_device_id(char *did);

int qid_sys_get_qid_device_id(char *did, size_t buff_len);

int qid_sys_set_ddns_status(char *status);

int qid_sys_get_ddns_status(char *status, size_t buff_len);

int qid_sys_enable_default_RA_services();

int qid_sys_set_refresh_token(char *refresh_token);

int qid_sys_get_refresh_token(char *refresh_token, size_t buff_len);

int qid_sys_get_token_valid_period(char *valid_period, size_t buff_len);

int qid_sys_set_token_valid_period(char *valid_period);

int qid_sys_get_gateway_ip(char* lan_gw_ip, char* lan_gw_interface);

int qid_sys_set_device_token(char *device_token);

int qid_sys_get_device_token(char *device_token, size_t buff_len);

int qid_sys_get_last_update_token_time(char *at, size_t buff_len);

int qid_sys_set_last_update_token_time(char *at);

int qid_sys_set_hash_string(char* hash_type, char* input);

int qid_sys_get_hash_string(char* request_type, char* output, size_t buff_len);

int qid_sys_set_hash_string_ttl(char* hash_type, int period);

int qid_sys_get_hash_string_ttl(char* request_type, char* output, size_t buff_len);

int qid_sys_get_curl_log_flag(char* curl_flag, size_t buff_len);

int qid_sys_get_qcloud_api_proto(char *proto, size_t buff_len);

int qid_sys_set_qcloud_ver(char *version);

int qid_sys_get_qcloud_ver(char *version, size_t buff_len);

int qid_sys_set_app_token(char *value);

int qid_sys_get_app_token(char *value, size_t buff_len);

int qid_conf_get_entry(char* qid_conf_buf,char* section, char* key, char* value, size_t value_len);

int qid_sys_set_last_update_status( int error_code );

int qid_sys_get_last_update_status( int* p_error_code, time_t* p_last_sucess_timestamp, time_t* p_last_fail_timestamp );

int qid_sys_restart_porter();

int qid_sys_nvram_commit();

// for get external ip cache
#define EXTERNAP_IP_HASH_PERIOD 120
int qid_sys_set_last_get_external_ip_time();
int qid_sys_get_last_get_external_ip_time();
int qid_sys_set_tmp_external_ip(char* ext_ip);
int qid_sys_get_tmp_external_ip(char* ext_ip, size_t buff_len );

#ifdef __cplusplus
}
#endif

#endif /* QID_SYSTEM_H_ */
