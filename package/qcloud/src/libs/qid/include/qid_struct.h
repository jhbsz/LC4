/*!
 * @file: qid_struct.h
 * @description: Private function and data structure declaration.
 *  Created on: Jan 7, 2013
 */

#ifndef QID_H_
#define QID_H_

#include "comm_hdr.h"
#include "qcloud_util.h"
#include "qid_api.h"
#include "qid_system.h"
#include "buff.h"

#define QID_STAT_CHECKED_OUT              "0"
#define QID_STAT_MIGRATION                "2"
#define QID_STAT_BIND                     "3"

#define OAUTH_GRANT_TYPE_CLIENT_CRED      "client_credentials"
#define OAUTH_GRANT_TYPE_SIGNIN           "password"
#define OAUTH_GRANT_TYPE_REFRESH_TOKEN    "refresh_token"

enum {
  OAUTH_GRANT_E_CLIENT_CRED = 0,
  OAUTH_GRANT_E_PASSWD,
  OAUTH_GRANT_E_REFRESH_TOKEN
};

#define MAGIC_LOG_NUMBER                  80682695

enum
{
  qid_user_code = 0,
  qid_user_msg,
  qid_user_registered_device,
  qid_user_query_status = qid_user_registered_device,     // 2
  num_qid_user_query_resp
};

enum qid_oauth_request
{
  qid_grant_type = 0,
  qid_client_id,
  qid_client_secret,
  qid_username,
  qid_password,
  num_qid_oauth_request
};

enum qid_oauth_response
{
  qid_oauth_error = 0,
  qid_oauth_error_description,
  qid_oauth_resp_access_token,
  qid_oauth_resp_token_type,
  qid_oauth_resp_expires_in,
  qid_oauth_resp_refresh_token,
  num_oauth_resp_qid_oauth_response
};

enum qid_general_response
{
  qid_code = 0, qid_message, qid_result, num_qid_general_response
};

enum qid_check_in
{
  qid_checkin_auth_token = 2, qid_checkin_device_id, num_checkin_resp_fields
};

enum qid_device_info_root
{
  info_root_device_name,
  info_root_description,
  info_root_published_access_code,
  num_device_info_root_fields
};

enum qid_device_info
{
  qid_device_device_type,
  qid_device_model_name,
  qid_device_hw_serial_number,
  qid_device_fw_serial_number,
  qid_device_mac_address,

  qid_device_host_name,
  qid_device_firmware_version,
  qid_device_client_agent_version,
  qid_device_lan_ip,
  qid_device_lan_gateway,

  qid_device_wan_ip,
  qid_device_hdd_info,
  qid_device_enc_info,
  num_device_fields
};

enum qid_register_info
{
  qid_register_device_id = 0,
  qid_register_user_id,
  qid_register_device_name,
  qid_register_description,
  qid_register_device_info,

  qid_register_device_type,
  qid_register_model_name,
  qid_register_hw_serial_number,
  qid_register_fw_serial_number,
  qid_register_mac_address,

  qid_register_host_name,
  qid_register_firmware_version,
  qid_register_client_agent_version,
  qid_register_lan_ip,
  qid_register_lan_gateway,

  qid_register_wan_ip,
  qid_register_create_date,
  qid_register_update_date,
  qid_register_status,
  qid_register_published_access_code,

  num_qid_register
};

enum qid_update_response
{
  qid_update_period = 4, qid_update_push_msg_id, qid_update_push_msg
};

enum qid_bind_device_response
{
  qid_bind_code = 0, qid_bind_message, qid_bind_device_id,
  qid_bind_token, qid_bind_result
};

enum mycloudnas
{
  mycloudnas_vlinkid = 2
};

typedef enum _qid_stat_e
{
  QID_STAT_E_CHECKED_OUT = '0',
  QID_STAT_E_VALIDATING = '1',
  QID_STAT_E_MIGRATION = '2',
  QID_STAT_E_BIND = '3',
} QID_STAT_E;

enum ip_status
{
  IPV4 = 1,
  IPV6,
  IP_PUBLIC,
  IP_PRIVATE
} ;

#define ERROR_TO_CHECKOUT         (-9999)

/*move to other header file.*/
struct qcloud_st
{

  /*! app information */
  qnap_app_info app_info;
  /*! QID host name */
  char qid_servc_host[MAX_QID_HOST_LEN + 1];
  /*! host name of mycloudnas api server*/
  char mcn_servc_host[MAX_QID_HOST_LEN + 1];
  /*! host name of usher api server*/
  char usher_servc_host[MAX_QID_HOST_LEN + 1];
  /*! QNAP ID, user name of QNAP portal service, a signed ascii string; max length 254*/
  char qid[MAX_QID_LEN + 1];
  /* Device mac string*/
  char mac_str[MAC_STR_LONG_LEN + 1];
  /*! Hardware serial number*/
  char hw_sn[META_STR_LEN + 1];
  /*! Hardware serial number*/
  char suid[META_STR_LEN + 1];
  /*! Device model type*/
  char model[NAME_LEN + 1];
  /*! auth token of QID service */
  char auth_token[MAX_APPKEY_LEN + 1];
  /*! refresh token of QID service */
  char refresh_token[MAX_APPKEY_LEN + 1];
  /*! valid period of this token */
  char valid_period[NAME_LEN + 1];
  /*! last update time of token */
  time_t token_last_update_time;
  /*! device firmware version */
  char fw_ver[VERSION_LEN + 1];
  /*! device alias name  */
  char device_name[NAME_LEN + 1];
  /*! device unqiue ID of mycloudnas service */
  char device_id[DEVICE_ID_LEN + 1];
  /*! device unqiue token of mycloudnas service */
  char device_token[MAX_APPKEY_LEN + 1];
  /*! HTTP timeout */
  int http_timeout;

  void * aes_enc_ctx;
  void * aes_dec_ctx;

  /*! debug log flags */
  int log_flag;
  /*! logging control block of CA */
  QNAP_LOG_T log_cb;

  QCLOUD_STATUS_MSG last_status;

  buf_t* http_resp;

  /*! Flag to mark this a rebinding device request.*/
  int rebind_flag;

  /*! APP auth token of QID service */
  char app_token[MAX_APPKEY_LEN + 1];

  FILE* qid_conf_fp;
  char qid_conf_buf[QID_CONF_BUFF_LEN+1];

  char last_event_log[MAX_SYSTEM_CMD+1];



  // a flag to check if production site
  int is_production;
};

#define QID_WRITE_SYSTEM_VAR(DEST, VAR, LEN, SET_F, RET)   do{                               \
    if(DEST && DEST != VAR){\
      strncpy(DEST, VAR, LEN);\
      DEST[LEN] = '\0'; \
    }\
    RET |= SET_F(VAR);\
}while(0)

#ifdef __cplusplus
extern "C" {
#endif

  void decrypt_str(struct qcloud_st *ctx, char *str_buff, size_t buff_size);
  char * encrypt_str(struct qcloud_st *ctx, char *src);

  void qcloud_reload_config(struct qcloud_st *ctx);
  int qnap_id_get_device_passwd(struct qcloud_st *ctx, char *pwd_buff,
                                size_t buff_len);

  int qcloud_get_status_code(char * code_str);

#ifdef __cplusplus
}
#endif

#endif /* QID_H_ */
