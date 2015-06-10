

#include <curl/curl.h>
#include <errno.h>
#include <math.h>
#include <json-c/json.h>
#include <time.h>

#include <sys/stat.h> /* struct stat, fchmod (), stat (), S_ISREG, S_ISDIR */
#include "std_hdr.h"
#include "comm_hdr.h"
#include "qcloud_util.h"
#include "qcloud.h"
#include "qid_struct.h"
#include "qid_status_code.h"
#include "mcn_api.h"
#include "qcloud_gen_apirequest.h"

#define SET_JSON_STR(ROOT, FIELD, VALUE)do{                                 \
  if((VALUE != NULL) && (VALUE[0] != '\0')) {                               \
    json_object_object_add(ROOT, FIELD,                                     \
                           json_object_new_string(VALUE));                  \
  }                                                                         \
}while(0)

#define JSON_OBJ_MALLOC(PTR, malloc_type, RETURN_VALUE) do{                \
    PTR = malloc_type();                                                    \
    if(PTR == NULL){                                                        \
      QNAP_LOG_ERR_NL("Failed to malloc for json_object");                  \
      RETURN_VALUE = ERR_NULL_POINTER;                                      \
    }                                                                        \
}while(0)

typedef struct _qcloud_response_st{
  int code;
  char *message;
  char *callstack;

  json_object*  root_obj;
  json_object*  result;

}qcloud_response_st;

char *qcloud_json_resp_code = "code";
char *qcloud_json_resp_message = "message";
char *qcloud_json_resp_callstack = "stacktrace";
char *qcloud_json_resp_result = "result";

char *EVENT_LEVEL[] = {"debug","info", "notice","warn", "error","crit","alert", "emerg"};

static int json_grep_value( json_object* json_obj, key_pair_st* list,
                            size_t list_size, int index)
{
  int ret = 0;
  json_object_object_foreach( json_obj, key, value)
  {
    if( json_object_get_type(value) == json_type_object){
      json_grep_value( value, list, list_size, index);
    }else{// if(json_object_get_type( value ) == json_type_string){
      if( strcmp( list[index].key, key) == 0){
        strncpy( list[index].value,  json_object_get_string( value ), PAIR_VALUE_LEN);
      }
    }
  }
  return ret;
}

int json_parse(char *resp, key_pair_st *entry_list, size_t num_list)
{

  json_object *new_obj;
  /*
   * send to json parser
   */
  new_obj = json_tokener_parse(resp);
  if (new_obj == NULL ) {
    QNAP_LOG_ERR_NL("json-c_parse: truncated JSON string");
    return ERR_INVALID_VALUE;
  }

  int index_cnt = 0;
  for (index_cnt = 0; index_cnt < num_list; index_cnt++) {
    json_grep_value(new_obj, entry_list, num_list, index_cnt);
  }
  /*
   * free this object in the end
   */
  json_object_put(new_obj);
  return NO_ERROR;
}

static void parse_json_free_qcloud_resp(qcloud_response_st *qresult){
  if(qresult){
    if(qresult->message){
      free(qresult->message);
    }

    if(qresult->callstack){
      free(qresult->callstack);
    }

    if(qresult->root_obj){
      json_object_put(qresult->root_obj);
    }

  }
}

static int parse_json_qcloud_root(char *resp, qcloud_response_st *qresult)
{
  int rc = ERR_NULL_POINTER;
  json_object* root_obj = NULL;
  json_object* target;

  if(!resp || !qresult){
    goto exit;
  }

  if(resp[0] == '\0'){
    rc = ERR_INPUT_LEN;
    goto exit;
  }

  memset(qresult, 0x0, sizeof(qcloud_response_st));

  /*
   * parse response.
   */
  root_obj = json_tokener_parse(resp);
  if(!root_obj){
    rc = ERR_INVALID_FORMAT;
    QNAP_LOG_ERR_NL("Failed to parse Qcloud json response.");
    QNAP_LOG_ERR_NL("%s", resp);
    goto exit;
  }

  /*
   * Get code.
   */
  target = json_object_object_get(root_obj, qcloud_json_resp_code);
  if( target != NULL )
  {
    qresult->code = json_object_get_int(target);
    QNAP_LOG_DEBUG_NL("DEBUG Qcloud code: %d", qresult->code);
  }
  else
  {
    QNAP_LOG_DEBUG_NL("The response json format is invalid");
    qresult->code = ERR_INVALID_FORMAT;
  }

  /*
   * Get message.
   */
  target = json_object_object_get(root_obj, qcloud_json_resp_message);
  qresult->message = (target)?strdup(json_object_get_string(target)):NULL;

  if(qresult->code != NO_ERROR){
    /*
     * Get callstack.
     */
    target = json_object_object_get(root_obj, qcloud_json_resp_callstack);
    qresult->callstack = target?strdup(json_object_get_string(target)):NULL;
    QNAP_LOG_ERR_NL("Qcloud code: %d", qresult->code);
    QNAP_LOG_ERR_NL("Qcloud msg: %s", qresult->message?qresult->message:"NULL");
    QNAP_LOG_ERR_NL("Qcloud callstack: %s", qresult->callstack?qresult->callstack:"NULL");
    qresult->result = NULL;
    rc = qresult->code;
    goto exit;
  }

  qresult->result = json_object_object_get(root_obj, qcloud_json_resp_result);
  if(!qresult->result){
    QNAP_LOG_ERR_NL("Qcloud result object not found");
    rc = ERR_INVALID_VALUE;
    goto exit;
  }

  rc = NO_ERROR;

exit:
  if(rc != NO_ERROR){
    QNAP_LOG_ERR_NL("rc: %d", rc);
  }

  if(qresult){
    qresult->root_obj = root_obj;
  }

  return rc;

}

static json_object* generate_device_local_user_arr(char** users, int num)

{
  int cnt = 0, rc = NO_ERROR;
  json_object *entry = NULL;
  json_object *ret = NULL;
  JSON_OBJ_MALLOC(ret, json_object_new_array, rc);
  for (cnt = 0; cnt < num; cnt++, users++) {
    if (strlen(*users)) {
      JSON_OBJ_MALLOC(entry, json_object_new_object, rc);
      SET_JSON_STR(entry, "user_name", *users);
      json_object_array_add(ret, entry);
    }
    else{
      break;
    }
  }

  if(NO_ERROR != rc){
    json_object_put(ret);
    ret = NULL;
  }

  return ret;
}

int parse_external_ip(char* resp, char* external_ip, size_t ip_len)
{
  int ret = ERR_NULL_POINTER;
  qcloud_response_st qresult;                   // qcloud response ctx
  if( !external_ip || !resp)
  {
    goto exit;
  }
  memset(&qresult, 0x0, sizeof(qcloud_response_st));
  ret = parse_json_qcloud_root(resp, &qresult);
  if(ret != NO_ERROR){
    if(ret == ERR_INVALID_VALUE){
      QNAP_LOG_DEBUG_NL("parse_json_qcloud_root error: ret:%d", ret);
      ret = MCN_ERR_JSON_FORMAT;
    }
    goto exit;
  }

  struct json_object *obj = json_object_object_get(qresult.result, "public_ip");
  const char *public_ip = json_object_get_string(obj);
  if(public_ip){
    strncpy(external_ip, public_ip, ip_len);
    QNAP_LOG_DEBUG_NL("parse_external_ip ret:%s", public_ip);
  }else{
    ret = ERR_NULL_POINTER;
  }

exit:
  parse_json_free_qcloud_resp(&qresult);
  return ret;
}

int generate_port_test_str(struct qcloud_st *ctx, char* ret, size_t size,
                               char* url)
{
  int rc = NO_ERROR;
  json_object *entry = NULL;
  JSON_OBJ_MALLOC(entry, json_object_new_object, rc);
  if( NO_ERROR != rc)
    goto exit;

  SET_JSON_STR(entry, "url",url);
  SET_JSON_STR(entry, "method", "GET");
  snprintf(ret, size, "%s", json_object_to_json_string(entry));
exit:
  return rc;
}

static json_object*
generate_device_service_port_arr(mcn_service_port *port_list, int num_servc)
{
  int cnt = 0, rc = NO_ERROR;
  mcn_service_port *list = port_list;
  json_object *entry = NULL;
  json_object *ret = NULL;
  JSON_OBJ_MALLOC(ret, json_object_new_array, rc);
  for (cnt = 0; cnt < num_servc; cnt++, list++) {
    if (strlen(list->name)) {
      JSON_OBJ_MALLOC(entry, json_object_new_object, rc);

      if(list->sub_name[0] != '\0'){
        char service_name[QCLOUD_PUBLISHED_SERVC_NAME_LEN+1];
        snprintf(service_name, QCLOUD_PUBLISHED_SERVC_NAME_LEN, "%s %s",
                 list->sub_name, list->name);
        strncpy(list->name, service_name, QCLOUD_PUBLISHED_SERVC_NAME_LEN);
      }

      SET_JSON_STR(entry, "service_name", list->name);
      json_object_object_add(entry, "external_port",
                             json_object_new_int(list->extnl_port));
      json_object_object_add(entry, "internal_port",
                             json_object_new_int(list->intnl_port));
      if(list->extnl_port <= 0 || list->intnl_port <= 0){
        rc = ERR_INVALID_PORT;
        goto exit;
      }

      SET_JSON_STR(entry, "protocol",list->proto);
      json_object_array_add(ret, entry);
    }
  }
exit:
  if(NO_ERROR != rc){
    json_object_put(ret);
    ret = NULL;
  }

  return ret;
}

static json_object*
generate_device_local_service_arr(mcn_ddns_publish_st *servc_list,
                                   int num_servc)
{
  int cnt = 0, rc = NO_ERROR;
  json_object *entry = NULL;
  json_object *ret = NULL;
  JSON_OBJ_MALLOC(ret, json_object_new_array, rc);
  mcn_ddns_publish_st* servc = servc_list;
  char servcname[2*DDNS_SERVC_NAME_LEN+1];
  for (cnt = 0; cnt < num_servc; cnt++, servc++) {
    if (strlen(servc->name)) {
      JSON_OBJ_MALLOC(entry, json_object_new_object, rc);
      if(strlen(servc->sub_name) > 0){
        memset(servcname, 0, sizeof(servcname));
        sprintf(servcname,"%s %s", servc->sub_name,servc->name);
        SET_JSON_STR(entry, "service_name",servcname);
      }else{
        SET_JSON_STR(entry, "service_name",servc->name);
      }
      SET_JSON_STR(entry, "uri",servc->login_path);
      json_object_object_add(entry, "private",
                             json_object_new_boolean(servc->is_private));
      json_object_object_add(entry, "published",
                             json_object_new_boolean(servc->is_published));
      json_object_array_add(ret, entry);
    }
  }

  if(NO_ERROR != rc){
    json_object_put(ret);
    ret = NULL;
  }

  return ret;
}

int generate_device_local_user_str(char* ret, size_t ret_size, char** users, int user_count)
{
  json_object* res = generate_device_local_user_arr(users, user_count);
  if( res == NULL ){
    return ERR_NULL_POINTER;
  }
  snprintf(ret, ret_size, "%s", json_object_to_json_string(res));
  json_object_put(res);
  return NO_ERROR;
}

int generate_device_service_port_str(char* ret, size_t ret_size, mcn_service_port *port_list, int num_servc)
{
  json_object* res = generate_device_service_port_arr(port_list, num_servc);
  if( res == NULL ){
    return ERR_NULL_POINTER;
  }
  snprintf(ret, ret_size, "%s", json_object_to_json_string(res));
  json_object_put(res);
  return NO_ERROR;
}

int generate_device_local_service_str(char* ret, size_t ret_size, mcn_ddns_publish_st *servc_list,
                                         int num_servc)
{
  json_object* res = generate_device_local_service_arr(servc_list, num_servc);
  if( res == NULL ){
    return ERR_NULL_POINTER;
  }
  snprintf(ret, ret_size, "%s", json_object_to_json_string(res));
  json_object_put(res);
  return NO_ERROR;
}

int generate_ddns_service_str(struct qcloud_st *ctx, char* ret, size_t size,
                               int need_device_id)
{
  int rc = NO_ERROR;
  json_object *entry = NULL;

  JSON_OBJ_MALLOC(entry, json_object_new_object, rc);
  if (need_device_id)
    SET_JSON_STR(entry, "device_id",ctx->device_id);

  snprintf(ret, size, "%s", json_object_to_json_string(entry));

exit:
  return rc;
}

static json_object* generate_device_lanip_list_arr(lan_ip_info_st *lan_ip_info)
{
  int cnt, rc = NO_ERROR, num_json_obj = 0;;
  json_object *ret = NULL;
  json_object *entry = NULL;
  JSON_OBJ_MALLOC(ret, json_object_new_array, rc);

  if(lan_ip_info->lan_ip_count == 0){
    rc = ERR_INVALID_VALUE;
    goto exit;
  }

  for (cnt = 0; cnt < lan_ip_info->lan_ip_count; cnt++) {
    JSON_OBJ_MALLOC(entry, json_object_new_object, rc);
    SET_JSON_STR(entry, "name",lan_ip_info->interface[cnt]);
    if( strcmp(IP_VERSION4, lan_ip_info->lan_ip_version[cnt]) == 0){
      if(strlen(lan_ip_info->lan_ip[cnt]) != 0){
        SET_JSON_STR(entry, JSON_STR_IPV4,lan_ip_info->lan_ip[cnt]);
      }
    }else if(strcmp(IP_VERSION6, lan_ip_info->lan_ip_version[cnt]) == 0){
      if(strlen(lan_ip_info->lan_ip[cnt]) != 0){
        SET_JSON_STR(entry, JSON_STR_IPV6,lan_ip_info->lan_ip[cnt]);
      }
    }
    json_object_array_add(ret, entry);
  }
  num_json_obj = json_object_array_length(ret);
  if( num_json_obj == 0){
    rc = ERR_INVALID_VALUE;
  }
exit:
  if(NO_ERROR != rc){
    QNAP_LOG_INFO_NL("num_json_obj:%d lan_ip_info->lan_ip_count:%d", num_json_obj, lan_ip_info->lan_ip_count);
    json_object_put(ret);
    ret = NULL;
  }

  return ret;
}

static json_object* generate_device_wanip_list_arr(struct qcloud_st *ctx)
{
  int rc = NO_ERROR;

  // json object to return
  json_object *ret = NULL;

  // json entry for temp use
  json_object *entry = NULL;

  // tmp buffer to put ext ip
  char ext_ip[IP_STR_LEN+1];

  // call cli to get external iop
  qcloud_get_external_ip(ctx, ext_ip, sizeof(ext_ip));

  // check if get external ip
  if( strlen(ext_ip) == 0 )
  {
    rc = ERR_GET_DEVICE_INFO;
  }
  else
  {
    // allocate for the json object to return
    JSON_OBJ_MALLOC(ret, json_object_new_array, rc);

    // allocate memory for the new object
    JSON_OBJ_MALLOC(entry, json_object_new_object, rc);

    // check it's ipv4 or ipv6
    if( util_is_valid_ipv4(ext_ip) == TRUE){
      SET_JSON_STR(entry, JSON_STR_IPV4, ext_ip);
    }else{
      SET_JSON_STR(entry, JSON_STR_IPV6, ext_ip);
    }
    json_object_array_add(ret, entry);
  }

  // if error, remember to free json object
  if(NO_ERROR != rc){
    json_object_put(ret);
    ret = NULL;
  }

  return ret;
}

static json_object* generate_device_gateway(lan_ip_info_st *lan_ip_info)
{
  int rc = NO_ERROR;
  json_object *ret = NULL;
  JSON_OBJ_MALLOC(ret, json_object_new_object, rc);
  char gateway_ip[IP_STR_LEN+1];
  char gw_interface[IP_STR_LEN+1];
  memset(gateway_ip, 0, IP_STR_LEN);
  memset(gw_interface, 0, IP_STR_LEN);
  rc = qid_sys_get_gateway_ip(gateway_ip, gw_interface);
  if(NO_ERROR != rc || strlen(gateway_ip) == 0 || strlen(gw_interface) == 0){
    rc = ERR_INVALID_VALUE;
    goto exit;
  }
  SET_JSON_STR(ret, "name",gw_interface);
  if( util_is_valid_ipv4(lan_ip_info->lan_gw) == TRUE){
    SET_JSON_STR(ret, JSON_STR_IPV4,lan_ip_info->lan_gw);
  }else{
    SET_JSON_STR(ret, JSON_STR_IPV6,lan_ip_info->lan_gw);
  }

exit:
  if(NO_ERROR != rc){
    json_object_put(ret);
    ret = NULL;
  }

  return ret;
}

static json_object* generate_device_info(struct qcloud_st *ctx, char* new_device_name)
{
  int rc = NO_ERROR;
  json_object *ret = NULL;
  key_pair_st device_info [] =
  {
    {"device_type", "\0"},
    {"model_name", "\0"},
    {"hw_serial_number", "\0"},
    {"fw_serial_number", "\0"},
    {"mac_address", "\0"},

    {"host_name", "\0"},
    {"firmware_version", "\0"},
    {"agent_version", "\0"},
    {JSON_STR_LAN_IPS, "\0"},
    {JSON_STR_LAN_GATEWAY, "\0"},

    {JSON_STR_WAN_IPS, "\0"}
  };

  key_pair_st device_msg [] =
  {
    {"Device type", "\0"},
    {"Model name", "\0"},
    {"Hardware serial number", "\0"},
    {"Firmware serial number", "\0"},
    {"MAC address", "\0"},

    {"Host name", "\0"},
    {"Firmware version", "\0"},
    {"Agent version", "\0"},
    {"LAN IP", "\0"},
    {"LAN gateway", "\0"},

    {"WAN IP", "\0"},
    {"Harddisk information","\0"}
  };

  key_pair_st *fields = device_info;
  key_pair_st *msg_fields = device_msg;
  JSON_OBJ_MALLOC(ret, json_object_new_object, rc);
  if( NO_ERROR != rc){
    goto exit;
  }
  lan_ip_info_st lan_info;

  rc = qid_sys_get_local_ip_list(&lan_info);
  if( NO_ERROR != rc){
    goto exit;
  }
  json_object* lan_ip = generate_device_lanip_list_arr(&lan_info);
  if(!lan_ip){
    rc = ERR_GET_DEVICE_INFO;
    ctx->last_status.status_code = rc;
    strncpy(ctx->last_status.status_msg, PAIR_GET_KEY(msg_fields, qid_device_lan_ip)
            , QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }
  json_object* gateway = generate_device_gateway(&lan_info);
  if(!gateway){
    rc = ERR_GET_DEVICE_INFO;
    ctx->last_status.status_code = rc;
    strncpy(ctx->last_status.status_msg, PAIR_GET_KEY(msg_fields, qid_device_lan_gateway)
            , QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  json_object* wan_ip = generate_device_wanip_list_arr(ctx);
  if(!wan_ip){
    QNAP_LOG_ERR_NL("generate_device_wanip_list_arr error");
    rc = ERR_GET_DEVICE_INFO;
    ctx->last_status.status_code = rc;
    strncpy(ctx->last_status.status_msg, PAIR_GET_KEY(msg_fields, qid_device_wan_ip)
            , QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  /*get data from configurations at NAS*/
  qid_sys_get_device_type(PAIR_GET_VALUE(fields, qid_device_device_type),
                          PAIR_VALUE_LEN);
  if( strlen(PAIR_GET_VALUE(fields, qid_device_device_type)) == 0 ){
    rc = ERR_GET_DEVICE_INFO;
    ctx->last_status.status_code = rc;
    strncpy(ctx->last_status.status_msg, PAIR_GET_KEY(msg_fields, qid_device_device_type)
            , QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  qid_sys_get_model(PAIR_GET_VALUE(fields, qid_device_model_name),
                    PAIR_VALUE_LEN);
  if( strlen(PAIR_GET_VALUE(fields, qid_device_model_name)) == 0 ){
    rc = ERR_GET_DEVICE_INFO;
    ctx->last_status.status_code = rc;
    strncpy(ctx->last_status.status_msg, PAIR_GET_KEY(msg_fields, qid_device_model_name)
            , QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  rc = qid_sys_get_hw_serial_number( PAIR_GET_VALUE(fields, qid_device_hw_serial_number),
                                     PAIR_VALUE_LEN);

  rc = qid_sys_get_suid( PAIR_GET_VALUE(fields, qid_device_fw_serial_number),
                         PAIR_VALUE_LEN);
  if( strlen(PAIR_GET_VALUE(fields, qid_device_fw_serial_number)) == 0 ){
    rc = ERR_GET_DEVICE_INFO;
    ctx->last_status.status_code = rc;
    strncpy(ctx->last_status.status_msg, PAIR_GET_KEY(msg_fields, qid_device_fw_serial_number)
            , QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  rc = qid_sys_get_mac(PAIR_GET_VALUE(fields, qid_device_mac_address),
                       PAIR_VALUE_LEN);
  if( strlen(PAIR_GET_VALUE(fields, qid_device_mac_address)) == 0 ){
    rc = ERR_GET_DEVICE_INFO;
    ctx->last_status.status_code = rc;
    strncpy(ctx->last_status.status_msg, PAIR_GET_KEY(msg_fields, qid_device_mac_address)
            , QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  strncpy(PAIR_GET_VALUE(fields, qid_device_host_name), new_device_name, PAIR_VALUE_LEN);
  qid_sys_get_fw_ver(PAIR_GET_VALUE(fields, qid_device_firmware_version),
                     PAIR_VALUE_LEN);
  if( strlen(PAIR_GET_VALUE(fields, qid_device_firmware_version)) == 0 ){
    rc = ERR_GET_DEVICE_INFO;
    ctx->last_status.status_code = rc;
    strncpy(ctx->last_status.status_msg, PAIR_GET_KEY(msg_fields, qid_device_firmware_version)
            , QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  strncpy(PAIR_GET_VALUE(fields, qid_device_client_agent_version),
          VERSION, PAIR_VALUE_LEN);
  PAIR_GET_VALUE(fields, qid_device_client_agent_version)[PAIR_VALUE_LEN] = '\0';

  int cnt = 0;
  for(cnt = 0; cnt < num_device_fields; cnt++){
    if(cnt == qid_device_lan_ip ){
      json_object_object_add(ret, JSON_STR_LAN_IPS, lan_ip);
    }else if(cnt == qid_device_lan_gateway ){
      json_object_object_add(ret, JSON_STR_LAN_GATEWAY, gateway);
    }else if(cnt == qid_device_wan_ip ){
      json_object_object_add(ret, JSON_STR_WAN_IPS,wan_ip);
    }else if(strlen(PAIR_GET_VALUE(fields, cnt))){
      SET_JSON_STR(ret, PAIR_GET_KEY(fields, cnt),PAIR_GET_VALUE(fields, cnt));
    }

  }
exit:
  if(NO_ERROR != rc){
    json_object_put(ret);
    ret = NULL;
  }
  return ret;
}


int generate_device_info_root(struct qcloud_st *ctx, char *device_name,
                               char *msg_buff, size_t buff_size)
{
  int ret = ERR_GET_DEVICE_INFO;

  QCLOUD_STATUS_MSG *last_error;
  json_object *root = NULL;
  JSON_OBJ_MALLOC(root, json_object_new_object, ret);
  json_object* dinfo = generate_device_info(ctx, device_name);
  if( !dinfo){
    QNAP_LOG_ERR_NL("Failed to create json object for generate_device_info.");
    last_error = qcloud_get_last_error(ctx);
    ret = last_error->status_code;
    goto exit;
  }

  SET_JSON_STR(root, "device_name", device_name);

  json_object_object_add(root, "rebind_by_device_name",
                         json_object_new_boolean(ctx->rebind_flag));

  json_object_object_add(root, JSON_STR_DEVICE_INFO, dinfo);

  snprintf(msg_buff, buff_size,"%s", json_object_to_json_string(root));

  ret = json_object_put(root);
  if( TRUE != ret ){
    QNAP_LOG_ERR_NL("Failed to free json object for generate_device_info_root.");
    ret = ERR_NULL_POINTER;
  }else{
    ret = NO_ERROR;
  }

exit:
  return ret;
}

void generate_oauth_request(char* grant_type, char* client_id,
                            char* client_secret, char* qid,
                            char* passwd, char *msg_buff, size_t buff_size )
{
  key_pair_st oauth_request_fields[] =    // HTTP fields of oauth request
  {
     {"grant_type", "\0"},
     {"client_id", "\0"},
     {"client_secret", "\0"},
     {"username", "\0"},
     {"password", "\0"},
     {"\0", "\0"}
  };

  key_pair_st *fields = oauth_request_fields;
  strncpy(PAIR_GET_VALUE(fields, qid_grant_type), grant_type, PAIR_VALUE_LEN);
  strncpy(PAIR_GET_VALUE(fields, qid_client_id), client_id, PAIR_VALUE_LEN);
  strncpy(PAIR_GET_VALUE(fields, qid_client_secret), client_secret, PAIR_VALUE_LEN);

  /*
   * if grant type is 'password', then qid and passwd will be given
   * if grant type is 'refresh_token', then qid and passwd will not be given
   */
  if(qid){
    strncpy(PAIR_GET_VALUE(fields, qid_username), qid, PAIR_VALUE_LEN);
  }

  if(passwd){
    strncpy(PAIR_GET_VALUE(fields, qid_password), passwd, PAIR_VALUE_LEN);
  }

  char *msg_fmt;
  int i, ret;
  char *payload = msg_buff;
  char *encoded_val;
  for(i = 0, ret = 0; ret < buff_size; i++){

    if (PAIR_GET_VALUE(fields, i)[0] != '\0'){

      if( i == num_qid_oauth_request -1 ){
        msg_fmt = "%s=%s";
      }else{
        msg_fmt = "%s=%s&";
      }

      encoded_val = util_url_encode(PAIR_GET_VALUE(fields, i)) ;
      if(encoded_val){
        ret += snprintf(payload+ret, buff_size-ret, msg_fmt,
                        PAIR_GET_KEY(fields, i), encoded_val);
        free(encoded_val);
      }
    }

    if( i == num_qid_oauth_request -1 ){
      break;
    }
  }

  for(i=0; i < num_qid_oauth_request ;i++){
    if( i != qid_password && i != qid_client_secret){
      QNAP_LOG_DEBUG_NL("%s[%d]= %s", PAIR_GET_KEY(fields, i), i,
                        PAIR_GET_VALUE(fields, i));
    }
  }
}


static json_object* generate_json_share_link_resource(char *ssid, char *file_path,
                                                       char *file_type){
  int rc = NO_ERROR;
  json_object *res = NULL;

  JSON_OBJ_MALLOC(res, json_object_new_object, rc);
  if(NO_ERROR == rc && res){
    SET_JSON_STR(res, "ssid", ssid);
    SET_JSON_STR(res, "file_path", file_path);
    SET_JSON_STR(res, "file_type", file_type);
  }
  return res;
}
