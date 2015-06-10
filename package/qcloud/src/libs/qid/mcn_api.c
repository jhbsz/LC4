/*
 * mcn_api.c
 *
 *  Created on: Jan 7, 2013
 */

#include <errno.h>
#include <sys/types.h>  // for regex
#include <regex.h>      // for regex

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fcntl.h> // for disk IO

#include "std_hdr.h"
#include "comm_hdr.h"
#include "mcn_api.h"

#include "qcloud_util.h"
#include "qid_struct.h"
#include "qid_system.h"

#include "qid_httpc.h"
#include "qid_status_code.h"
#include "qcloud_gen_apirequest.h"

#define MCN_API_VERSION       "v1.1"
#define UPDATE_PERIOD_MAX     24*60*60        // the longest period for device updating.
#define UPDATE_PERIOD_MIN     60              // the shortest period for device updating.
#define UPDATE_PERIOD_DEF     60*60           // the default period.
#define UPDATE_FAILED_NETWORK 5*60            // if this failed is due to networking, try next time after 5*60 seconds
#define UPDATE_FAILED_RETRY   60*60           // if this update is failed at server; try it again after 60*60 seconds.
#define MCN_PUBLISHED_UPDATE  "/sbin/qddns_update > /dev/null 2>&1 &"

#define MYCLOUD_GET_SHARED_LINK_URL       "%s/%s/"
#define MYCLOUD_NOTIFY_SHARED_LINK_URL    "%s/%s/"

int mycloudNAS_device_update_info(struct qcloud_st *ctx, time_t *report_period)
{

  int ret = ERR_NULL_POINTER;
  int flag = 0x0;
  if (!ctx || !report_period) {
    goto exit;
  }

  key_pair_st respn_fields[] = { { "code", "\0" },
                                 { "message", "\0" },
                                 { "device_id", "\0" },
                                 { "result", "\0" },
                                 { "updated_period", "\0" }};

  char msg_buff[PAYLOAD_LEN_MAX + 1];
  char* resp = NULL;
  char uri[TMP_BUFF_LEN + 1];

  *report_period = UPDATE_PERIOD_DEF;
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);
  qnap_id_get_device_name(ctx,ctx->device_name, NAME_LEN);
  if(0 == strlen(ctx->device_id)){
    goto exit;
  }

  snprintf(uri, TMP_BUFF_LEN, "%s/%s/device/%s", ctx->qid_servc_host,
           QID_API_VERSION, ctx->device_id);
  ret = generate_device_info_root(ctx, ctx->device_name, msg_buff, PAYLOAD_LEN_MAX);
  if (ret != NO_ERROR) {
    goto exit;
  }
  qcloud_reload_config(ctx);
  if(0 == strlen(ctx->device_id) || 0 == strlen(ctx->device_token)
    || 0 == strlen(ctx->device_name) || 0 == strlen(ctx->auth_token)){
    ret = NO_ERROR;
    QNAP_LOG_DEBUG_NL("device_id:%s", ctx->device_id);
    QNAP_LOG_DEBUG_NL("device_token:%s", ctx->device_token);
    QNAP_LOG_DEBUG_NL("device_name:%s", ctx->device_name);
    QNAP_LOG_DEBUG_NL("auth_token:%s", ctx->auth_token);
    goto exit;
  }

  flag = QID_HTTPC_FLAG_AUTH | QID_HTTPC_FLAG_DEVICE;
  ret = qid_httpc_request_ex(ctx, flag, PUT, uri, NULL, msg_buff, &resp, PAYLOAD_LEN_MAX);

  if (ret < NO_ERROR) {
    goto exit;
  }
  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));

exit:
  if (ret != NO_ERROR) {
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
    //there is not necessary to unbind in the lib. we should just return the error code.
    //Let caller decide what to do..
  }else{
    if (respn_fields[qid_update_period].value[0] != '\0'){
      /*
       * get the next update time
       */
       ret = atoi(respn_fields[qid_update_period].value);
       if(ret <= UPDATE_PERIOD_MAX && ret >= UPDATE_PERIOD_MIN){
         *report_period = ret;
       }
    }
    ret = NO_ERROR;
  }

  // set update status
  qid_sys_set_last_update_status(ret);

  return ret;
}

int mycloudNAS_device_get_info(struct qcloud_st *ctx)
{
  return qcloud_check_binding_status(ctx);
}

int mycloudNAS_set_ddns_status(struct qcloud_st *ctx, int activiaction)
{
  // return code
  int ret = ERR_NULL_POINTER;

  // flag used in http request
  int flag = 0;

  // buffer used to send event log
  char err_msg[MAX_SYSTEM_CMD+1];

  char current_status[TMP_BUFF_LEN + 1];                      // current status before changed
  int ddns_status = MCN_SERVC_DISABLE;
  char uri[TMP_BUFF_LEN + 1];                                 // HTTP uri buffer
  char msg[PAYLOAD_LEN_MAX + 1];                              // HTTP POST body
  char temp_str[PAYLOAD_LEN_MAX + 1];
  char* resp = NULL;
  key_pair_st respn_fields[] =                                // parsed result of device query
      { { "code", "\0" }, { "message", "\0" }, };
  char status[STATUS_FLAG_LEN+1];
  char buff[URL_STR_LEN+1];
  char proto[PROTO_STR_LEN+1];
  QCLOUD_STATUS_MSG *last_error;

  // check input argument
  if (!ctx) {
    goto exit;
  }

  // get current ddns status
  ddns_status = mycloudNAS_get_ddns_status(ctx);

  // get qid status
  qid_sys_get_qid_status(msg, PAYLOAD_LEN_MAX);

  // get device name
  qid_sys_get_device_name(ctx->device_name, NAME_LEN);

  // get device id
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);

  // make sure we have device id & qid status is already bind
  if (strlen(ctx->device_id) == 0 || msg[0] != QID_STAT_E_BIND) {
    QNAP_LOG_DEBUG_NL("status:%s, device_id:%s", msg, ctx->device_id);
    ret = QID_ERR_WONT_EXECUTED;
    goto exit;
  }

  // if ddns already enabled, just return
  if(ddns_status == MCN_SERVC_ENABLE && activiaction == TRUE
      && ctx->rebind_flag == FALSE){
    ret = NO_ERROR;
    goto exit;
  }

  // get qid
  qid_sys_get_qid(ctx->qid, MAX_QID_LEN);

  // get server host
  qid_sys_get_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);

  // get protocol
  qid_sys_get_qcloud_api_proto(proto, PROTO_STR_LEN);
  if( proto [0] == '\0' ){
    sprintf(proto, "https");
  }

  // generate the hosts
  sprintf(ctx->qid_servc_host, "%s://%s", proto, buff);
  qid_sys_get_mcn_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  sprintf(ctx->mcn_servc_host, "%s://%s", proto, buff);
  if(strlen(ctx->mcn_servc_host) == 0 ){
    ret = ERR_GET_DEVICE_INFO;
    goto exit;
  }

  // sending http request
#define MYCLOUD_DDNS_UPDATE_URI   "%s/%s/ddns/device/%s"
#define MYCLOUD_DDNS_POST_URI     "%s/%s/ddns"
#define MYCLOUD_DDNS_UPDATE_BODY  "{\"ddns_name\": \"%s\"}"
  flag = QID_HTTPC_FLAG_AUTH;
  // for disable ddns
  if (activiaction == FALSE ) {
    snprintf(uri, TMP_BUFF_LEN, MYCLOUD_DDNS_UPDATE_URI, ctx->mcn_servc_host,
             MCN_API_VERSION, ctx->device_id);
    ret = qid_httpc_request_ex(ctx, flag, DELETE, uri, NULL, msg, &resp, PAYLOAD_LEN_MAX);
  }
  // for enable ddns
  else {
    snprintf(uri, TMP_BUFF_LEN, MYCLOUD_DDNS_POST_URI, ctx->mcn_servc_host,
             MCN_API_VERSION);
    generate_ddns_service_str(ctx, msg, PAYLOAD_LEN_MAX, activiaction);
    ret = qid_httpc_request_ex(ctx, flag, POST, uri, NULL, msg, &resp, PAYLOAD_LEN_MAX);
  }

  // check http request return code
  if (ret < NO_ERROR) {
    goto exit;
  }

  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));
  if(ret != NO_ERROR){
    if( (ret == MCN_ERR_DDNS_EXISTS && activiaction == TRUE) ||
        (ret == MCN_ERR_DDNS_NAME_NOT_FOUND && activiaction == FALSE)){
      /*
       * ignore the dummy command between API server and device.
       */
      ret = NO_ERROR;
    }
  }

exit:

  if (ret != NO_ERROR) {
    // reset ddns status to disabled if the current status is init??
    // don't know why have QID_SYS_INIT_STR
    qid_sys_get_ddns_status(current_status, TMP_BUFF_LEN);
    if (0 == strncmp(current_status, QID_SYS_INIT_STR, TMP_BUFF_LEN)) {
      qid_sys_set_ddns_status(QID_SYS_DISABLE_STR);
    }
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));

    // get last error
    last_error = qcloud_get_last_error(ctx);

    // generate corresponding event logs
    if(MCN_ERR_EMPTY_WAN_IP == ret){
      snprintf(temp_str, TMP_BUFF_LEN, QID_ERR_CAUSE_DEVICE_INFO, "WAN IP");
      snprintf(msg, TMP_BUFF_LEN, MCN_SYS_LOG_DDNS_UPDATE_FAILED,
                temp_str, ret, ERROR_HANDLING_CHECK_INTERNET);
      qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, msg);
    }else if(ERR_CONNECT_SERVER == ret){
      snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_DDNS_UPDATE_FAILED,
               QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_CHECK_INTERNET);
      qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, temp_str);
    }else if(ERR_API_SERVER == ret || ERR_INVALID_VALUE == ret){
      snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_DDNS_UPDATE_FAILED,
               QID_ERR_CAUSE_PARSE_RESPONSE,ret, ERROR_HANDLING_QCLOUD_SERVER);
      qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, temp_str);
    }
  }
  else{
    /*
     * update DDNS Status to /etc/config/qid.conf.
     */
    if (activiaction == TRUE ) {
      // store the ddns status to enabled
      qid_sys_set_ddns_status(QID_SYS_ENABLE_STR);

      // send ddns turn on event log
      snprintf(err_msg, TMP_BUFF_LEN, MCN_SYS_LOG_DDNS_TURN_ON);
      qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, err_msg);
    }
    else {
      // store the ddns status to disabled
      qid_sys_set_ddns_status(QID_SYS_DISABLE_STR);

      // send ddns turn off event log
      snprintf(err_msg, TMP_BUFF_LEN, MCN_SYS_LOG_DDNS_TURN_OFF);
      qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, err_msg);
    }

    /*
     * notify porter after DDNS changed
     */
    qid_sys_restart_porter();
  }
  return ret;

}

int mycloudNAS_get_ddns_status(struct qcloud_st *ctx)
{
  char status[TMP_BUFF_LEN + 1];

  qid_sys_get_ddns_status(status, TMP_BUFF_LEN);
  if (0 == strncmp(status, QID_SYS_ENABLE_STR, TMP_BUFF_LEN)) {
    return MCN_SERVC_ENABLE;
  } else if (0 == strncmp(status, QID_SYS_INIT_STR, TMP_BUFF_LEN)) {
    return MCN_SERVC_INIT;
  }
  return MCN_SERVC_DISABLE;
}

int mycloudNAS_update_published_service(struct qcloud_st *ctx,
                                        mcn_ddns_publish_st *servc_list,
                                        int num_servc)
{
  int ret = ERR_NULL_POINTER, flag = 0;
  if(!ctx){
    goto exit;
  }
  char uri[TMP_BUFF_LEN + 1];                                 // HTTP uri buffer
  char msg[PAYLOAD_LEN_MAX + 1];                              // HTTP POST body
  char* resp = NULL;
  char temp_str[TMP_BUFF_LEN + 1];
  mcn_ddns_publish_st *servc;
  key_pair_st respn_fields[] =                                // parsed result of device query
      { { "code", "\0" }, { "message", "\0" }, };
  char buff[URL_STR_LEN+1];
  char proto[PROTO_STR_LEN+1];
  qid_sys_get_qid(ctx->qid, MAX_QID_LEN);
  qid_sys_get_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  qid_sys_get_qcloud_api_proto(proto, PROTO_STR_LEN);
  if( proto [0] == '\0' ){
    sprintf(proto, "https");
  }
  sprintf(ctx->qid_servc_host, "%s://%s", proto, buff);
  qid_sys_get_mcn_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  sprintf(ctx->mcn_servc_host, "%s://%s", proto, buff);
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);
  qid_sys_get_qid_status(msg, PAYLOAD_LEN_MAX);
  if (strlen(ctx->device_id) == 0 || msg[0] != QID_STAT_E_BIND
      || strlen(ctx->mcn_servc_host) == 0) {
    QNAP_LOG_DEBUG_NL("%d", QID_ERR_WONT_EXECUTED);
    return QID_ERR_WONT_EXECUTED;
  }

#define MYCLOUD_SERVICE_PUBLISH_URI   "%s/%s/device/%s/localservice"
  snprintf(uri, TMP_BUFF_LEN, MYCLOUD_SERVICE_PUBLISH_URI, ctx->mcn_servc_host,
           MCN_API_VERSION, ctx->device_id);

  servc = servc_list;
  generate_device_local_service_str(msg,PAYLOAD_LEN_MAX, servc, num_servc);
  if( num_servc <= 0){
    goto exit;
  }
  flag = QID_HTTPC_FLAG_AUTH | QID_HTTPC_FLAG_SERVICE;
  ret = qid_httpc_request_ex(ctx, flag, PUT, uri, NULL, msg, &resp, PAYLOAD_LEN_MAX);
  if( ret < NO_ERROR){
    goto exit;
  }
  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));

exit:
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
    switch(ret){
      default:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_PUBLISHED_FAILED,
                 QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_QCLOUD_SERVER);
        break;
      case ERR_CONNECT_SERVER:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_PUBLISHED_FAILED,
                 QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_CHECK_INTERNET);
        break;
      case ERR_API_SERVER:
      case ERR_INVALID_VALUE:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_PUBLISHED_FAILED,
                 QID_ERR_CAUSE_PARSE_RESPONSE,ret, ERROR_HANDLING_QCLOUD_SERVER);
        break;
    }
    qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, temp_str);
  }

  return ret;
}

int mycloudNAS_update_service_port(struct qcloud_st *ctx,
                                   mcn_service_port *port_list, int num_servc)
{

  int ret = ERR_NULL_POINTER, flag = 0;

  if(!ctx){
    goto exit;
  }

  char uri[TMP_BUFF_LEN + 1];                                 // HTTP uri buffer
  char msg[PAYLOAD_LEN_MAX + 1];                              // HTTP POST body
  char* resp = NULL;
  char temp_str[TMP_BUFF_LEN + 1];

  key_pair_st respn_fields[] =                                 // parsed result of device query
      { { "code", "\0" }, { "message", "\0" }, };
  char buff[URL_STR_LEN+1];
  char proto[PROTO_STR_LEN+1];
  qid_sys_get_qid(ctx->qid, MAX_QID_LEN);
  qid_sys_get_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  qid_sys_get_qcloud_api_proto(proto, PROTO_STR_LEN);
  if( proto [0] == '\0' ){
    sprintf(proto, "https");
  }
  sprintf(ctx->qid_servc_host, "%s://%s", proto, buff);
  qid_sys_get_mcn_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  sprintf(ctx->mcn_servc_host, "%s://%s", proto, buff);
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);
  qid_sys_get_qid_status(msg, PAYLOAD_LEN_MAX);
  if (strlen(ctx->device_id) == 0 || msg[0] != QID_STAT_E_BIND
      || strlen(ctx->mcn_servc_host) == 0) {
    QNAP_LOG_DEBUG_NL("%d", QID_ERR_WONT_EXECUTED);
    return QID_ERR_WONT_EXECUTED;
  }

#define MYCLOUD_PORT_MAPPING_URI   "%s/%s/device/%s/localport"
  snprintf(uri, TMP_BUFF_LEN, MYCLOUD_PORT_MAPPING_URI, ctx->mcn_servc_host,
           MCN_API_VERSION, ctx->device_id);
  ret = generate_device_service_port_str(msg, PAYLOAD_LEN_MAX, port_list, num_servc);
  if(ret != NO_ERROR){
    goto exit;
  }
  flag = QID_HTTPC_FLAG_AUTH | QID_HTTPC_FLAG_PORTMAPPING;
  ret = qid_httpc_request_ex( ctx, flag, PUT, uri, NULL, msg, &resp, PAYLOAD_LEN_MAX);
  if( ret < NO_ERROR){
    goto exit;
  }
  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));

exit:
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
    switch(ret){
      default:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_PORT_MAPPING_FAILED,
                 QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_QCLOUD_SERVER);
        break;
      case ERR_CONNECT_SERVER:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_PORT_MAPPING_FAILED,
                 QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_CHECK_INTERNET);
        break;
      case ERR_API_SERVER:
      case ERR_INVALID_VALUE:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_PORT_MAPPING_FAILED,
                 QID_ERR_CAUSE_PARSE_RESPONSE,ret, ERROR_HANDLING_QCLOUD_SERVER);
        break;
    }

    qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, temp_str);
  }

  // set update status
  qid_sys_set_last_update_status(ret);

  return ret;
}

#define ALLOWED_USERS_MSG_FMT "allowedUsers={\"allowedUsers\": [%s]}"
#define SERVC_USERS_FMT        "\"%s\","
int mycloudNAS_update_private_user(struct qcloud_st *ctx, char **users,
                                   int num_users)
{
  int ret = ERR_NULL_POINTER, flag = 0;

  if (!ctx || !users) {
    goto exit;
  }

  char uri[TMP_BUFF_LEN + 1];                                 // HTTP uri buffer
  char msg[PAYLOAD_LEN_MAX + 1];                              // HTTP POST body
  char temp_str[TMP_BUFF_LEN + 1];
  char* resp = NULL;
  key_pair_st respn_fields[] =                                 // parsed result of device query
      { { "code", "\0" }, { "message", "\0" }, };
  char buff[URL_STR_LEN+1];
  char proto[PROTO_STR_LEN+1];
  qid_sys_get_qid(ctx->qid, MAX_QID_LEN);
  qid_sys_get_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  qid_sys_get_qcloud_api_proto(proto, PROTO_STR_LEN);
  if( proto [0] == '\0' ){
    sprintf(proto, "https");
  }
  sprintf(ctx->qid_servc_host, "%s://%s", proto, buff);
  qid_sys_get_mcn_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  sprintf(ctx->mcn_servc_host, "%s://%s", proto, buff);
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);
  qid_sys_get_qid_status(msg, PAYLOAD_LEN_MAX);
  if (strlen(ctx->device_id) == 0 || msg[0] != QID_STAT_E_BIND
      || strlen(ctx->mcn_servc_host) == 0) {
    QNAP_LOG_DEBUG_NL("%d", QID_ERR_WONT_EXECUTED);
    return QID_ERR_WONT_EXECUTED;
  }

#define MYCLOUD_PUBLISHED_USERS_URI   "%s/%s/device/%s/localuser"
  snprintf(uri, TMP_BUFF_LEN, MYCLOUD_PUBLISHED_USERS_URI, ctx->mcn_servc_host,
           MCN_API_VERSION, ctx->device_id);
  if(num_users > 0){
    generate_device_local_user_str(msg,PAYLOAD_LEN_MAX, users, num_users);
  }else{
    qid_sys_set_hash_string("localuser", "0");
  }

  flag = QID_HTTPC_FLAG_AUTH | QID_HTTPC_FLAG_USERS;
  ret = qid_httpc_request_ex(ctx, flag, (num_users > 0 ? PUT:DELETE),
                             uri, NULL, msg, &resp, PAYLOAD_LEN_MAX);
  if( ret < NO_ERROR){
    goto exit;
  }
  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));

exit:
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
    switch(ret){
      default:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_UPDATE_USER_LIST_FAILED,
                 QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_QCLOUD_SERVER);
        break;
      case ERR_CONNECT_SERVER:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_UPDATE_USER_LIST_FAILED,
                 QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_CHECK_INTERNET);
        break;
      case ERR_API_SERVER:
      case ERR_INVALID_VALUE:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_UPDATE_USER_LIST_FAILED,
                 QID_ERR_CAUSE_PARSE_RESPONSE,ret, ERROR_HANDLING_QCLOUD_SERVER);
        break;
    }

    qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, temp_str);

  }
  return ret;
}


#define MYCLOUD_ACCESS_CODE_BODY  "{ \"access_code\": \"%s\" }"
int mycloudNAS_update_access_code(struct qcloud_st *ctx, char *access_code)
{
  int ret = ERR_NULL_POINTER, flag = 0;

  if(!ctx || !access_code){
    goto exit;
  }

  char uri[TMP_BUFF_LEN+1];                                 // HTTP uri buffer
  char msg[PAYLOAD_LEN_MAX+1];                              // HTTP POST body
  char* resp = NULL;
  char temp_str[TMP_BUFF_LEN + 1];
  key_pair_st respn_fields[] =                          // parsed result of device query
  {
   {"code",    "\0" },
   {"message", "\0" },
  };
  char buff[URL_STR_LEN+1];
  char proto[PROTO_STR_LEN+1];
  qid_sys_get_qid(ctx->qid, MAX_QID_LEN);
  qid_sys_get_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  qid_sys_get_qcloud_api_proto(proto, PROTO_STR_LEN);
  if( proto [0] == '\0' ){
    sprintf(proto, "https");
  }
  sprintf(ctx->qid_servc_host, "%s://%s", proto, buff);
  qid_sys_get_mcn_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  sprintf(ctx->mcn_servc_host, "%s://%s", proto, buff);
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);
  qid_sys_get_qid_status(msg, PAYLOAD_LEN_MAX);
  if(strlen(ctx->device_id) == 0 || msg[0] != QID_STAT_E_BIND
      || strlen(ctx->mcn_servc_host) == 0){
     QNAP_LOG_DEBUG_NL("%d", QID_ERR_WONT_EXECUTED);
    return QID_ERR_WONT_EXECUTED;
  }

#define MYCLOUD_UPDATE_ACCESS_CODE_URI   "%s/%s/device/%s/accesscode"
  snprintf(uri, TMP_BUFF_LEN, MYCLOUD_UPDATE_ACCESS_CODE_URI,
           ctx->mcn_servc_host, MCN_API_VERSION, ctx->device_id);

  snprintf( msg, PAYLOAD_LEN_MAX, MYCLOUD_ACCESS_CODE_BODY, access_code);
  flag = QID_HTTPC_FLAG_AUTH | QID_HTTPC_FLAG_ACCESSCODE;
  ret = qid_httpc_request_ex(ctx, flag, PUT, uri, NULL, msg, &resp, PAYLOAD_LEN_MAX);
  if( ret < NO_ERROR){
    goto exit;
  }
  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));

exit:
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
    switch(ret){
      default:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_ACCESS_CODE_FAILED,
                 QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_QCLOUD_SERVER);
        break;
      case ERR_CONNECT_SERVER:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_ACCESS_CODE_FAILED,
                 QID_ERR_CAUSE_CONNECTION,ret, ERROR_HANDLING_CHECK_INTERNET);
        break;
      case ERR_API_SERVER:
      case ERR_INVALID_VALUE:
        snprintf(temp_str, TMP_BUFF_LEN, MCN_SYS_LOG_ACCESS_CODE_FAILED,
                 QID_ERR_CAUSE_PARSE_RESPONSE,ret, ERROR_HANDLING_QCLOUD_SERVER);
        break;
    }

    qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, temp_str);
  }
  return ret;
}

int mycloudNAS_is_report_port_mapping(struct qcloud_st *ctx)
{
  int ret = FALSE;
  ret |= mycloudNAS_get_ddns_status(ctx);
  return ret;
}
