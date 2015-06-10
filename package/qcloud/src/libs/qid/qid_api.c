/*
 * qid_api.c
 *
 */

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/file.h>

#include "std_hdr.h"
#include "comm_hdr.h"
#include "qcloud_util.h"
#include "qid_struct.h"
#include "qid_system.h"
#include "qid_api.h"
#include "qid_httpc.h"
#include "qid_status_code.h"
#include "util_aes.h"
#include "qcloud_dao.h"
#include "qcloud_gen_apirequest.h"

static char null_string[] = {"\0"};

#define GET_TOKEN(GET_FUNC, CTX, BUFF, BUFF_SIZE)  \
  do{                                              \
    int ret = ERR_NULL_POINTER;                    \
    if(!CTX || !BUFF){                             \
      QNAP_LOG_ERR_NL("rc: [%d]", ret);            \
    }                                              \
    ret = GET_FUNC( BUFF, BUFF_SIZE);              \
    if(ret != NO_ERROR){                           \
      QNAP_LOG_ERR_NL("rc: [%d]", ret);            \
    }                                              \
    decrypt_str(CTX, BUFF, BUFF_SIZE);             \
                                                   \
    if(ret != NO_ERROR){                           \
      QNAP_LOG_ERR_NL("rc: [%d]", ret);            \
    }                                              \
  }while(0)

int reset_qcloud_request_hash()
{
  // debug message
  QNAP_LOG_INFO_NL("reset_qcloud_request_hash");

  // return code
  int ret = NO_ERROR;

  // counting var
  int cnt = 0;

  // list the hashes to clear
  key_pair_st url_fields[] =
  {
    {"ddns",    "\0" },
    {"cloudlink", "\0" },
    {"localservice",    "\0" },
    {"serviceport", "\0" },
    {"localuser",    "\0" },
    {"accesscode", "\0" },
    {"device",    "\0" },
    {"\0", "\0"}
  };

  //loop to clear hashes
  for(;cnt < num_request_data_type; cnt++){
    ret |= qid_sys_set_hash_string(url_fields[cnt].key, null_string);
    ret |= qid_sys_set_hash_string_ttl(url_fields[cnt].key, 0);
  }

  return ret;
}


int reset_qcloud_ctx_conf(struct qcloud_st *ctx)
{
  int ret = NO_ERROR;

  // log message
  QNAP_LOG_CRIT_NL("start resetting qcloud config");

  // reset qid
  QID_WRITE_SYSTEM_VAR(ctx->qid, null_string, MAX_QID_LEN, qid_sys_set_qid, ret);

  // reset device name
  QID_WRITE_SYSTEM_VAR(ctx->device_name, null_string, NAME_LEN,
                       qid_sys_set_device_name, ret);

  // reset auth token
  QID_WRITE_SYSTEM_VAR(ctx->auth_token, null_string, MAX_APPKEY_LEN,
                       qid_sys_set_auth_token, ret);

  // reset device token
  QID_WRITE_SYSTEM_VAR(ctx->device_token, null_string, MAX_APPKEY_LEN,
                       qid_sys_set_device_token, ret);

  // reset refresh token
  QID_WRITE_SYSTEM_VAR(ctx->refresh_token, null_string, MAX_APPKEY_LEN,
                       qid_sys_set_refresh_token, ret);

  // reset device id
  QID_WRITE_SYSTEM_VAR(ctx->device_id, null_string, MAX_QID_LEN,
                       qid_sys_set_qid_device_id, ret);

  // reset last token time
  qid_sys_set_last_update_token_time(null_string);
  ctx->token_last_update_time = (time_t)0;

  // reset token valid period
  QID_WRITE_SYSTEM_VAR(ctx->valid_period, null_string, NAME_LEN,
                       qid_sys_set_token_valid_period, ret);

  // reset ddns status
  qid_sys_set_ddns_status(QID_SYS_DISABLE_STR);

  // reset request hash
  reset_qcloud_request_hash();

  // reset qid status to checkout
  qid_sys_set_qid_status(QID_STAT_CHECKED_OUT);

  // reset qcloud version
  qid_sys_set_qcloud_ver(null_string);

  // log message
  QNAP_LOG_CRIT_NL("complete resetting qcloud config");

  return ret;
}

char *stripe_white_space_tail(char *str)
{
  if (strlen(str) > 0) {
    char *p = str + strlen(str) - 1;
    while (*p == ' ' || *p == '\t') p--;
    *(p+1) = '\0';
  }
  return str;
}

char *skip_white_space(char *str)
{
  char *p = str;
  while (*p == ' ' || *p == '\t') p++;
  return p;
}

int is_commented_string(char *str)
{
  char *q = skip_white_space(str);
  if (*q == '#' || *q == ';') return 1;
  return 0;
}

void decrypt_str(struct qcloud_st *ctx, char *str_buff, size_t buff_size)
{
  util_aes_decrypt_str(ctx->aes_dec_ctx, str_buff, buff_size);
}

char * encrypt_str(struct qcloud_st *ctx, char *src)
{
  return util_aes_encrypt_str(ctx->aes_enc_ctx, src);
}

#define QCLOUD_DEF_HTTP_TIMEOUT      (20)
uint32_t salt[] = {12345, 54321};

void qcloud_reload_config(struct qcloud_st *ctx)
{
  // return conde
  int ret = NO_ERROR;

  // check input
  if(!ctx){
    return;
  }

  // buffer to get urls
  char buff[URL_STR_LEN+1];
  char proto[PROTO_STR_LEN+1];
  memset( buff,0, sizeof(char)*(URL_STR_LEN+1));
  memset( proto,0, sizeof(char)*(PROTO_STR_LEN+1));

  // get general configs
  // qid
  qid_sys_get_qid(ctx->qid, MAX_QID_LEN);
  // device name
  qnap_id_get_device_name(ctx,ctx->device_name, NAME_LEN);
  // device id
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);

  // get authentication
  GET_TOKEN(qid_sys_get_auth_token, ctx, ctx->auth_token, MAX_APPKEY_LEN);
  GET_TOKEN(qid_sys_get_refresh_token, ctx, ctx->refresh_token, MAX_APPKEY_LEN);

  /*
   * because device token is just for the identified. so we do not encrypt.
   */
#ifdef ENCRYPT_DEVICE_TOKEN
  GET_TOKEN(qid_sys_get_device_token, ctx, ctx->device_token, MAX_APPKEY_LEN);
#else
  qid_sys_get_device_token(ctx->device_token, MAX_APPKEY_LEN);
#endif

  // get proto
  qid_sys_get_qcloud_api_proto(proto, PROTO_STR_LEN);
  if( proto [0] == '\0' ){
    sprintf(proto, "https");
  }

  // get server hosts
  qid_sys_get_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  sprintf(ctx->qid_servc_host, "%s://%s", proto, buff);
  qid_sys_get_mcn_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  sprintf(ctx->mcn_servc_host, "%s://%s", proto, buff);
  qid_sys_get_usher_server_host(ctx->is_production, buff, MAX_QID_HOST_LEN);
  sprintf(ctx->usher_servc_host, "%s://%s", proto, buff);
  memset(ctx->qid_conf_buf, 0 , sizeof(char)*(QID_CONF_BUFF_LEN+1));
}

#define QCLOUD_ALPHA_APPID       "53e33bd4ab6dd106e2f1f29c"
#define QCLOUD_ALPHA_APPKEY      "h7SCrJ94RNc2s4RUNsZkWNRtYGGPulvSxze23EoyS5SLdNFG"
#define QCLOUD_PRODUCTION_APPID  "53e33d04d6b82f62690be813"
#define QCLOUD_PRODUCTION_APPKEY "z7rpuNQKb0JkSi7OGqvhCW7wrVJXkWL8H0vTBveaVSoLWLsC"

#ifdef PERFORMANCE_ANALYSIS
#include <sys/time.h>
#endif

/*
 * read aes key type from config file (/etc/config/qid.conf)
 * for QTS 4.0 aes key could be hwsn. hwsn could be empty
 * for QTS 4.1 public beta. aes could be hwsn or suid (those whose hwsn is empty)
 * this function reads if NAS holds a valid hwsn or not.
 * yes: use hswn
 * no: decrypt auth token and encrypt with suid. save key type to qid.conf
 */
int qcloud_aes_ctx_generator(struct qcloud_st *ctx)
{
  int ret = ERR_INVALID_CREDENTIAL;

  uint8_t *key_data = NULL;
  int key_data_len = 0;
  EVP_CIPHER_CTX *en = NULL, *de = NULL;

  // token
  char auth_token[MAX_APPKEY_LEN + 1];
  char refresh_token[MAX_APPKEY_LEN + 1];
  memset(auth_token, 0, sizeof(char)*(MAX_APPKEY_LEN + 1));
  memset(refresh_token, 0, sizeof(char)*(MAX_APPKEY_LEN + 1));

  // buffer
  char tmp_buff[TMP_BUFF_LEN+1];

  // reset
  char *enc_token = NULL, *enc_refresh_token = NULL;

  // allocate cipher ctx
  en = (EVP_CIPHER_CTX*) calloc(1,sizeof(EVP_CIPHER_CTX));
  de = (EVP_CIPHER_CTX*) calloc(1,sizeof(EVP_CIPHER_CTX));
  if(!en || !de){
    QNAP_LOG_ERR_NL("failed to allocate EVP_CIPHER_CTX");
    goto exit;
  }

  // init key
  key_data = (uint8_t *)ctx->hw_sn;
  key_data_len = strlen(ctx->hw_sn);

  // do aes_init
  ret = util_aes_init(key_data, key_data_len, (uint8_t *)&salt, en, de);
  if( ret != NO_ERROR )
  {
    QNAP_LOG_ERR_NL("Couldn't initialize AES cipher");
  }

exit:
  // check if error
  if( ret != NO_ERROR )
  {
    if( en ) free(en);
    if( de ) free(de);
  }

  // store the cipher ctx
  ctx->aes_enc_ctx  = en;
  ctx->aes_dec_ctx  = de;

  return ret;
}

static void determine_log_settings(struct qcloud_st *ctx)
{
  // log flags
  char curl_flag[TMP_BUFF_LEN+1];
  uint32_t log_flag;

  // set debug log to file
  log_flag = QID_LOG_FILE;

  // curl debug
  qid_sys_get_curl_log_flag(curl_flag, TMP_BUFF_LEN);
  if (MAGIC_LOG_NUMBER == atoi(curl_flag)){
    log_flag |= QID_LOG_CURL;
  }

  // debug log settings
  qnap_log_init_setting(&(ctx->log_cb));
  qnap_log_init("QID_API", NULL, NULL);
  qcloud_set_log(ctx, log_flag, LOG_DEBUG);
}

static void determine_app_id_n_key(qnap_app_info *info, struct qcloud_st *ctx)
{
  // set qcloud agent version
  strncpy(ctx->app_info.appver, VERSION, MAX_APPVER_LEN);

  // determine the app & key to use
  if( ctx->is_production )
  {
    strncpy(ctx->app_info.appid, QCLOUD_PRODUCTION_APPID, MAX_APPID_LEN);
    strncpy(ctx->app_info.appkey, QCLOUD_PRODUCTION_APPKEY, MAX_APPKEY_LEN);
  }
  else
  {
    strncpy(ctx->app_info.appid, QCLOUD_ALPHA_APPID, MAX_APPID_LEN);
    strncpy(ctx->app_info.appkey, QCLOUD_ALPHA_APPKEY, MAX_APPKEY_LEN);
  }

  // We use sub_appid to identify each service at device.
  // this seems to be useless
  strncpy(ctx->app_info.sub_appid, info->appid, MAX_APPID_LEN);
}

struct qcloud_st* qcloud_init_ctx(qnap_app_info *info)
{
  // allocate the qcloud context
  struct qcloud_st *ctx = malloc(sizeof(struct qcloud_st));

  // return code
  int ret = ERR_NULL_POINTER;

  // license invalid flag buf
  char license_invalid[STATUS_FLAG_LEN+1] = {0};

  if(ctx && info)
  {
    // reset context
    memset(ctx, 0x0, sizeof(struct qcloud_st));

    // setup debug log
    determine_log_settings(ctx);

    // get license invalid
    qid_sys_get_license_invalid(license_invalid, STATUS_FLAG_LEN);

    // according to Holmas
    // 0 : ready production
    // 1 : alpha production with full function ( evaluation license not expired )
    // 2 : alpha production with limited function ( evaluation license expired )
    if( license_invalid[0] == '0' ) ctx->is_production = 1;
    else ctx->is_production = 0;

    // set app id & key
    determine_app_id_n_key(info, ctx);

    /*
     * get device information
     */

    /* remove getting mac for better performance */
    //qid_sys_get_mac(ctx->mac_str, MAC_STR_LONG_LEN);

    ret = qid_sys_get_hw_serial_number(ctx->hw_sn, META_STR_LEN);

    if( NO_ERROR != ret){
      QNAP_LOG_ERR_NL("failed to get hw serial number by binary, ctx->hw_sn:%s", ctx->hw_sn);
    }

    qid_sys_get_suid(ctx->suid, META_STR_LEN);
    qid_sys_get_model(ctx->model, NAME_LEN);
    qid_sys_get_fw_ver(ctx->fw_ver, VERSION_LEN);

    /*
     * initialize AES cipher context.
     */
    qcloud_aes_ctx_generator(ctx);
    ctx->http_timeout = QCLOUD_DEF_HTTP_TIMEOUT;

    ctx->http_resp = buf_size(NULL, PAYLOAD_LEN_MAX);

    // load config from qid.conf or nvram
    qcloud_reload_config(ctx);

    // mark this debug log to avoid flooding
    //QNAP_LOG_INFO_NL("core:%s", ctx->qid_servc_host);
    ret = NO_ERROR;
  }

exit:
  // check if error, need to free ctx & return NULL
  if(ret != NO_ERROR && ctx != NULL)
  {
    if(ctx->aes_enc_ctx){free(ctx->aes_enc_ctx);}
    if(ctx->aes_dec_ctx){free(ctx->aes_dec_ctx);}
    if(ctx){
      free(ctx);
      ctx = NULL;
    }
  }

  return ctx;
}

void qcloud_cleanup_ctx(struct qcloud_st *ctx)
{
  if(ctx){
    /*
     * release AES cipher ctx object
     */
    if(ctx->aes_enc_ctx){
      util_aes_clean_up(ctx->aes_enc_ctx);
      free(ctx->aes_enc_ctx);
      ctx->aes_enc_ctx = NULL;
    }

    if(ctx->aes_dec_ctx){
      util_aes_clean_up(ctx->aes_dec_ctx);
      free(ctx->aes_dec_ctx);
      ctx->aes_dec_ctx = NULL;
    }

    // commit nvram
    qid_sys_nvram_commit();

    // cleanuup log
    qnap_log_cleanup();

    // cleanup http buffer
    buf_destory(ctx->http_resp);

    // cleanup ctx content
    memset(ctx, 0x0, sizeof(struct qcloud_st));

    // free ctx
    free(ctx);
  }
}

void qcloud_set_log(struct qcloud_st *ctx, int mode, int level)
{

  if(ctx){
    ctx->log_flag |= mode;
    pqnap_log->level = level;
    if(QID_LOG_FILE & ctx->log_flag){
      qnap_log_to_file(QNAP_QID_LOG_PATH);
    }else if(QID_LOG_STDOUT & ctx->log_flag){
      pqnap_log->log_fp = stdout;
    }else if(QID_LOG_STDERR & ctx->log_flag){
      pqnap_log->log_fp = stderr;
    }
    if(QID_LOG_CURL & ctx->log_flag){
      QNAP_LOG_SET_DETAIL();
    }
  }

}

QCLOUD_STATUS_MSG * qcloud_get_last_error(struct qcloud_st *ctx)
{
  if(ctx) return &(ctx->last_status);
  return NULL;
}

int qcloud_get_device_info(struct qcloud_st *ctx, qnap_dev_info *dev_info)
{
  int ret = NO_ERROR;

  // check input argument
  if( dev_info == NULL )
  {
    QNAP_LOG_ERR_NL("input null pointer");
    return QID_ERR_INVALID_PARAM;
  }

  // reset the input var
  memset(dev_info, 0X0, sizeof(qnap_dev_info));

  // get qid
  qid_sys_get_qid(dev_info->qid_account, MAX_QID_LEN);

  // get hardware serial number
  qid_sys_get_hw_serial_number(dev_info->hw_sn, QID_SN_STR_LEN);

  // get mac
  qid_sys_get_mac(dev_info->mac_str, QID_MAC_STR_LONG_LEN);

  // get qid status
  qid_sys_get_qid_status(dev_info->qid_status, STATUS_FLAG_LEN);

  // check if sign-in
  // only get the folowing values when signin
  if(dev_info->qid_status[0] != '0') {
    // device id
    qid_sys_get_qid_device_id(dev_info->device_id, DEVICE_ID_LEN);

    // device name
    qnap_id_get_device_name(ctx, dev_info->device_name, QID_DEVICE_NAME_LEN);

    // ddns status (it should be always Enable when signin
    qid_sys_get_ddns_status(dev_info->ddns_status, STATUS_FLAG_LEN);
    // eg: myqnapcloud.com
    qcloud_util_get_host(ctx->is_production, QCLOUD_SERVC_API_DDNS_STR, dev_info->ddns_server_host);
    dev_info->ddns_server_host[MAX_QID_HOST_LEN]='\0';

    // the update info
    qid_sys_get_last_update_status(
      &dev_info->last_update_error_code,
      &dev_info->last_update_success_timestamp,
      &dev_info->last_update_fail_timestamp);
  }

  return ret;
}

#define API_SERVER_RELAY_HTTP   "/v1.1/utility/fetch"
int qcloud_fetch_web_page(struct qcloud_st *ctx,
  const char* protocol, uint16_t extl_port,
  const char* path, time_t timeout,
  char* out_resp_buf, int out_resp_buf_len)
{
  // return code
  int rc = QID_ERR_INVALID_PARAM;

  // var to store wan ip
  char wan_ip[IP_STR_LEN+1];

  // var to store the whole url to fetch
  char url_to_fetch[PAYLOAD_LEN_1K+1];

  // var to store the cloud api uri
  char cloud_api_uri[TMP_BUFF_LEN+1];

  // the req api body
  char req_body[PAYLOAD_LEN_1K+1];

  // sgtore the response
  char* resp_body = NULL;

  // check imnpuyt
  if(!ctx || !protocol || !path || !out_resp_buf || !out_resp_buf_len){
    QNAP_LOG_ERR_NL("invalid parameter!!");
    goto exit;
  }

  // get external ip
  qcloud_get_external_ip(ctx, wan_ip, sizeof(wan_ip));
  if(strlen(wan_ip) == 0){
    goto exit;
  }

  // prepare url
  snprintf(url_to_fetch, PAYLOAD_LEN_1K,
           "%s://%s:%u%s",
           protocol, wan_ip, extl_port, path);
  generate_port_test_str(ctx, req_body, PAYLOAD_LEN_1K, url_to_fetch);

  // prepare the cloud api uri
  snprintf(cloud_api_uri, TMP_BUFF_LEN, "%s%s", ctx->qid_servc_host, API_SERVER_RELAY_HTTP);
  QNAP_LOG_ERR_NL("cloud_api_uri: %s", cloud_api_uri);

  // send the request
  rc = qid_httpc_request_ex(ctx, QID_HTTPC_FLAG_AUTH, POST,
                            cloud_api_uri, NULL, req_body, &resp_body,
                            PAYLOAD_LEN_MAX);

  // check return
  if(rc < NO_ERROR){
    goto exit;
  }

  // prepare the output
  if (strlen(resp_body) > PAYLOAD_LEN_MAX){
    resp_body[PAYLOAD_LEN_MAX] = '\0';
  }
  strncpy(out_resp_buf, resp_body, out_resp_buf_len - 1);
  out_resp_buf[out_resp_buf_len - 1] = 0;

exit:
  if(rc != NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(rc));
  }
  return rc;
}

int qcloud_check_binding_status(struct qcloud_st *ctx)
{
  int rc ;
  char err_msg[TMP_BUFF_LEN+1];

  if(ctx){
    int ret = ERR_NULL_POINTER, flag = 0;

    char uri[TMP_BUFF_LEN + 1];
    char* resp = NULL;
    key_pair_st respn_fields[] = { { "code", "\0" }, { "message", "\0" }, {"token", "\0"} };

    // check device id
    if (ctx->device_id[0] == '\0') {
      QNAP_LOG_ERR_NL("empty device id");
      goto exit;
    }

    // prepare uri to get
    snprintf(uri, TMP_BUFF_LEN, "%s/%s/device/%s", ctx->qid_servc_host,
             QID_API_VERSION, ctx->device_id);

    // do http request
    flag = QID_HTTPC_FLAG_AUTH;
    ret = qid_httpc_request_ex(ctx, flag, GET, uri, NULL, NULL, &resp, PAYLOAD_LEN_MAX);
    if (ret < NO_ERROR || ret == QID_SERCV_HTTP_ERROR) {
      goto exit;
    }
    /*
     * parse server response.
     */
    ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));
    /*
     * NOTE: for Qcloud migration, we need to refresh device token.
     * TODO: We may sync up the device status between device and cloud.
     */
    strncpy(ctx->device_token, respn_fields[2].value, MAX_APPKEY_LEN);

  exit:
    /*
     * if backend server returns MCN_ERR_DEVICE_ID_NOT_FOUND, reset qid config
     */
    if (ret != NO_ERROR) {
      // for all the following errors, need to reset config
      switch(ret)
      {
        case OAUTH_ERR_INVALID_GRANT:
        case OAUTH_ERR_INVALID_PASS:
        case OAUTH_ERR_INVALID_USER:
        case OAUTH_ERR_INVALID_CLI:
        case OAUTH_ERR_INVALID_REQ:
        case QID_ERR_RES_NOT_FOUND:
        case MCN_ERR_DEVICE_ID_NOT_FOUND:
          reset_qcloud_ctx_conf(ctx);
          QNAP_LOG_ERR_NL("reset config for error: %d %s", ret, get_strerror(ret));
          break;

        default:
          QNAP_LOG_ERR_NL("do nothing for error: %d %s", ret, get_strerror(ret));
          break;
      }
    }
    return ret;
  }else{
    rc = ERR_NULL_POINTER;
  }
  QNAP_LOG_DEBUG_NL("rc: %d", rc);
  return rc;
}

int qnap_id_device_checkout(struct qcloud_st *ctx)
{
  return qnap_id_unbind_device(ctx);
}

int qnap_id_download_file (struct qcloud_st *ctx, char *uri, time_t timeout)
{

  int ret = ERR_NULL_POINTER;

  if(!ctx){
    goto exit;
  }
  char *resp = NULL;

  if(timeout){
    ctx->http_timeout = timeout;
  }

  ret = qid_httpc_request(ctx, FALSE, GET, uri, NULL, &resp);
  if(ret != NO_ERROR){
    goto exit;
  }

exit:
  if(ret < NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
  }
  return ret;

}

int qnap_id_check_internet(struct qcloud_st *ctx, time_t timeout)
{
  char buff[MAX_QID_HOST_LEN+1];
  snprintf(buff, MAX_QID_HOST_LEN, "%s/v1/", ctx->qid_servc_host);
  return qnap_id_download_file (ctx, buff, timeout);
}

int qnap_id_get_device_name(struct qcloud_st *ctx, char *name, size_t buff_size)
{
  int ret = ERR_NULL_POINTER;

  char config_version[VERSION_LEN+1];
  qid_sys_get_qcloud_ver(config_version, VERSION_LEN);
  if( 0 == strncmp(config_version, QID_CONFIG_VERSION, VERSION_LEN)){
    ret = qid_sys_get_device_name(name, buff_size);
  }
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
  }

  return ret;
}

int qnap_id_update_device_name(struct qcloud_st *ctx, char *name)
{
  int ret = ERR_NULL_POINTER, flag = 0;

  if(!ctx){
    goto exit;
  }

  char uri[TMP_BUFF_LEN+1];
  char err_msg[TMP_BUFF_LEN+1];
  char msg[PAYLOAD_LEN_MAX+1];

  char* resp = NULL;
  key_pair_st respn_fields[] =
  {
   {"code",    "\0" },
   {"message", "\0" }
  };
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);
  snprintf(uri, TMP_BUFF_LEN, "%s/%s/device/%s", ctx->qid_servc_host,
           QID_API_VERSION, ctx->device_id);
  ret = generate_device_info_root( ctx, name, msg, PAYLOAD_LEN_MAX);
  if (ret != NO_ERROR) {
    goto exit;
  }
  flag = QID_HTTPC_FLAG_AUTH | QID_HTTPC_FLAG_DEVICE;
  ret = qid_httpc_request_ex(ctx, flag, PUT, uri, NULL, msg, &resp, PAYLOAD_LEN_MAX);
  if(ret < NO_ERROR){
    goto exit;
  }

  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));
  if(ret != NO_ERROR){
    goto exit;
  }

  strncpy(ctx->device_name, name, NAME_LEN);
  qid_sys_set_device_name(ctx->device_name);

  /*
   * notify porter to reload the device name.
   */
  qid_sys_restart_porter();

exit:
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
  }else{
    snprintf(err_msg, TMP_BUFF_LEN, QID_SYS_LOG_UPDATE_DEVICE_NAME, ctx->device_name);
    qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, err_msg);
  }
  return ret;
}

int qnap_id_is_device_name_available(struct qcloud_st *ctx, char *input_device_name)
{
  int rc = ERR_NULL_POINTER;

  if(!ctx){
    goto exit;
  }

  char device_name[QID_DEVICE_NAME_LEN+1];
  char uri[TMP_BUFF_LEN+1];                // URL string

  char* resp = NULL;
  key_pair_st check_fields[] =            // json parsed result
  {
   {"code",    "\0" },
   {"message", "\0" },
   {"is_unique", "\0"}
  };

  strncpy(device_name, input_device_name, QID_DEVICE_NAME_LEN);
  device_name[QID_DEVICE_NAME_LEN] = '\0';
  /*
   * get geoInfo from server
   */
  snprintf(uri, TMP_BUFF_LEN, "%s/%s/device/search?device_name=%s&is_unique=true",
           ctx->qid_servc_host, QID_API_VERSION, device_name);
  rc = qid_httpc_request_ex(ctx, QID_HTTPC_FLAG_AUTH, GET, uri, NULL, NULL, &resp,PAYLOAD_LEN_MAX);
  if(rc < NO_ERROR){
    goto exit;
  }

  json_parse(resp, check_fields, NUM_KEY_PAIR(check_fields));

  /*
   * parse server response.
   */
  rc = qid_httpc_api_handler(ctx, resp, check_fields, NUM_KEY_PAIR(check_fields));
  if(rc != NO_ERROR){
    if(rc == QID_SERVC_PARAM_INVALID){
      rc = QCLOUD_NAME_INVALID_FORMAT;
    }
    goto exit;
  }
  else{
    if( strcmp( "true", check_fields[qid_result].value) != 0 ){
      rc = QID_ERR_DEVICE_NAME_BEEN_USED;
    }else{
      rc = QCLOUD_NAME_FREE;
    }
  }

exit:
  if(rc != QCLOUD_NAME_FREE){
    QNAP_LOG_ERR_NL("%s", get_strerror(rc));
  }

  return rc;
}

int qnap_id_refresh_token(struct qcloud_st *ctx)
{
  return qid_httpc_refresh_token(ctx);
}

int qnap_id_is_token_expired()
{
  return qid_httpc_is_token_expired();
}

int qnap_id_app_signin(struct qcloud_st *ctx)
{
  int rc = ERR_NULL_POINTER;

  if(!ctx){
    goto exit;
  }

  key_pair_st signin_respn_fields[] =                        // parsed result of register response
  {
   {"error", "\0"},
   {"error_description", "\0"},
   {"access_token", "\0"},
   {"token_type", "\0"},
   {"expires_in", "\0"},
   {"refresh_token", "\0"},
  };

  char msg_buff[PAYLOAD_LEN_MAX+1];
  char uri[TMP_BUFF_LEN+1];
  char* resp = NULL;
  char *request = msg_buff;

  GET_TOKEN(qid_sys_get_app_token, ctx, ctx->app_token, MAX_APPKEY_LEN);

  if(ctx->app_token[0] != '\0'){
    rc = NO_ERROR;
    goto exit;
  }

  snprintf(uri, TMP_BUFF_LEN, "%s/oauth/token", ctx->qid_servc_host);
  generate_oauth_request( OAUTH_GRANT_TYPE_CLIENT_CRED, ctx->app_info.appid,
                          ctx->app_info.appkey, NULL, NULL, request, PAYLOAD_LEN_MAX);
  rc = qid_httpc_request_ex(ctx, FALSE, POST, uri, NULL, request, &resp, PAYLOAD_LEN_MAX);
  if(rc < NO_ERROR){
    goto exit;
  }

  rc = qid_httpc_oauth_handler(ctx, resp, OAUTH_GRANT_E_CLIENT_CRED, signin_respn_fields,
                                NUM_KEY_PAIR(signin_respn_fields));

exit:
  if(rc != NO_ERROR){
    ctx->app_token[0] = '\0';
    QNAP_LOG_ERR_NL("%s", get_strerror(rc));
  }else{
    //strncpy((client_auth_token), ctx->app_token, PAIR_VALUE_LEN);
  }
  return rc;
}

int qnap_id_signin (struct qcloud_st *ctx, char *qnapid, char *passwd){
  int ret = NO_ERROR;

  // check input
  if(!ctx || !qnapid || !passwd ){
    ret = ERR_NULL_POINTER;
    goto exit;
  }

  key_pair_st signin_respn_fields[] =                        // parsed result of register response
  {
   {"error", "\0"},
   {"error_description", "\0"},
   {"access_token", "\0"},
   {"token_type", "\0"},
   {"expires_in", "\0"},
   {"refresh_token", "\0"},
  };

  char msg_buff[PAYLOAD_LEN_MAX+1];
  char uri[TMP_BUFF_LEN+1];
  char* resp = NULL;
  char *request = msg_buff;

  snprintf(uri, TMP_BUFF_LEN, "%s/oauth/token", ctx->qid_servc_host);

  generate_oauth_request( OAUTH_GRANT_TYPE_SIGNIN, ctx->app_info.appid,
                          ctx->app_info.appkey, qnapid, passwd, request, PAYLOAD_LEN_MAX);
  ret = qid_httpc_request_ex(ctx, 0, POST, uri, NULL, request, &resp, PAYLOAD_LEN_MAX);
  if(ret < NO_ERROR){
    goto exit;
  }

  ret = qid_httpc_oauth_handler(ctx, resp, OAUTH_GRANT_E_PASSWD, signin_respn_fields,
                                NUM_KEY_PAIR(signin_respn_fields));
  if(ret == NO_ERROR){
    /*
     * save the QID
     */
    QID_WRITE_SYSTEM_VAR( ctx->qid, qnapid, MAX_QID_LEN, qid_sys_set_qid, ret);

    /* save version */
    qid_sys_set_qcloud_ver(QID_CONFIG_VERSION);
  }

exit:
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
  }
  return ret;
}

int qnap_id_bind_device (struct qcloud_st *ctx, char* device_name)
{
  int ret = NO_ERROR, flag = 0;

  QCLOUD_STATUS_MSG *last_error;

  // check input
  if(!ctx || !device_name  ){
    ret = ERR_NULL_POINTER;
    goto exit;
  }

  key_pair_st respn_fields[] =
  {
   {"code",    "\0" },
   {"message", "\0" },
   {"device_id", "\0"},
   {"token", "\0"},
   {"result", "\0"}
  };
  char err_msg[MAX_SYSTEM_CMD+1];
  char msg_buff[PAYLOAD_LEN_MAX+1];
  char* resp = NULL;
  char uri[TMP_BUFF_LEN+1];

  // generate the request
  snprintf(uri, TMP_BUFF_LEN, "%s/%s/device", ctx->qid_servc_host, QID_API_VERSION);
  ret = generate_device_info_root( ctx, device_name, msg_buff, PAYLOAD_LEN_MAX);
  if (ret != NO_ERROR) {
    goto exit;
  }
  flag = QID_HTTPC_FLAG_AUTH;

  // send request
  ret = qid_httpc_request_ex(ctx, flag, POST, uri, NULL, msg_buff, &resp, PAYLOAD_LEN_MAX);
  if( ret < NO_ERROR){
    goto exit;
  }

  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));
  if( ret == NO_ERROR)
  {
    // save the device id
    QID_WRITE_SYSTEM_VAR(ctx->device_id, respn_fields[qid_bind_device_id].value, DEVICE_ID_LEN, qid_sys_set_qid_device_id, ret);

    // save the device token
    strncpy( ctx->device_token, respn_fields[qid_bind_token].value, MAX_APPKEY_LEN);
    ctx->device_token[MAX_APPKEY_LEN] = '\0';
#ifdef ENCRYPT_DEBVICE_TOKEN
    char *enc_dtoken = encrypt_str(ctx, ctx->device_token);
    qid_sys_set_device_token(enc_dtoken);
#else
    qid_sys_set_device_token(ctx->device_token);
#endif

    /*
     * save the device name
     */
    QID_WRITE_SYSTEM_VAR(ctx->device_name, device_name, NAME_LEN, qid_sys_set_device_name, ret);

    // set qcloud config status to bind (which is "3")
    qid_sys_set_qid_status(QID_STAT_BIND);

    // sent checked in event log
    snprintf(err_msg, TMP_BUFF_LEN, QID_SYS_LOG_CHECKED_IN, ctx->qid, ctx->device_name);
    qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, err_msg);

    // always enable ddns
    mycloudNAS_set_ddns_status(ctx, TRUE);

    // set the qcloud config version
    qid_sys_set_qcloud_ver(QID_CONFIG_VERSION);

    /*
     * start porter
     */
    qid_sys_restart_porter();
  }

exit:
  if(ret != NO_ERROR){
    last_error = qcloud_get_last_error(ctx);
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));
    QNAP_LOG_ERR_NL("code:%d msg:%s", last_error->status_code, last_error->status_msg);
  }
  return ret;
}

int qnap_id_rebind_device (struct qcloud_st *ctx, char* device_name)
{
  ctx->rebind_flag = TRUE;
  return qnap_id_bind_device(ctx, device_name);
}

int qnap_id_unbind_device (struct qcloud_st *ctx)
{
  // return code
  int ret = NO_ERROR;

  // http flag
  int flag = 0;

  // check input argument
  if(!ctx){
    QNAP_LOG_DEBUG_NL("ctx is NULL");
    ret = ERR_NULL_POINTER;
    goto exit;
  }

  // error message to send to event log
  char err_msg[MAX_SYSTEM_CMD+1];

  key_pair_st respn_fields[] =
  {
   {"code",    "\0" },
   {"message", "\0" },
   {"result", "\0"}
  };

  char msg_buff[PAYLOAD_LEN_MAX+1];
  char* resp = NULL;
  char uri[TMP_BUFF_LEN+1];

  // get device id
  qid_sys_get_qid_device_id(ctx->device_id, DEVICE_ID_LEN);
  if(strlen(ctx->device_id) == 0){
    goto exit;
  }

  // prepare request
  snprintf(uri, TMP_BUFF_LEN, "%s/%s/device/%s", ctx->qid_servc_host,
           QID_API_VERSION, ctx->device_id);

  // send request
  flag = QID_HTTPC_FLAG_AUTH | QID_HTTPC_FLAG_DEVICE;
  ret = qid_httpc_request_ex(ctx, flag, DELETE, uri, NULL, msg_buff, &resp,
                             PAYLOAD_LEN_MAX);
  if(ret < NO_ERROR){
    goto exit;
  }

  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));
  if(ret != NO_ERROR){
    goto exit;
  }

exit:
  // if have error, print the log
  if(ret != NO_ERROR) {
    QNAP_LOG_ERR_NL("%s", get_strerror(ret));

    // for device id not found, assume already unbind
    if( ret == MCN_ERR_DEVICE_ID_NOT_FOUND ) ret = NO_ERROR;
  }

  // if no error
  if(ret == NO_ERROR) {
    // get device name & send event log
    qid_sys_get_device_name(ctx->device_name, QID_DEVICE_NAME_LEN);
    if(0 != strlen(ctx->device_name)){
      snprintf(err_msg, TMP_BUFF_LEN, QID_SYS_LOG_CHECKED_OUT, ctx->qid, ctx->device_name);
      qid_send_sys_event_log(QNAP_EVENT_TYPE_INFO, ctx->last_event_log, err_msg);
    }
    reset_qcloud_ctx_conf(ctx);
  }

  return ret;
}

#define MYCLOUD_GET_EXTERNAL_IP   "%s/%s/utility/geo/search"
int qcloud_get_external_ip(struct qcloud_st *ctx, char* external_ip, size_t ip_len)
{
  int ret = ERR_NULL_POINTER , cnt = 0;
  char uri[TMP_BUFF_LEN+1];
  char* resp = NULL;
  lan_ip_info_st lan_info;
  key_pair_st respn_fields[] =
  {
   {"code",    "\0" },
   {"message", "\0" },
   {"result", "\0" }
  };

  // check the cached external IP
  int last_get_external_ip_time = qid_sys_get_last_get_external_ip_time();
  if((int)time(0) - last_get_external_ip_time <= EXTERNAP_IP_HASH_PERIOD){
    ret = qid_sys_get_tmp_external_ip( external_ip, ip_len);
    goto exit;
  }


  snprintf(uri, TMP_BUFF_LEN, MYCLOUD_GET_EXTERNAL_IP, ctx->mcn_servc_host,
           QID_API_VERSION);
  ret = qid_httpc_request_ex(ctx, QID_HTTPC_FLAG_APP_AUTH, GET, uri, NULL, NULL, &resp, PAYLOAD_LEN_MAX);

  if(ret < NO_ERROR){
    ctx->last_status.status_code = ret;
    strncpy(ctx->last_status.status_msg, "Connection error", QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  /*
   * parse server response.
   */
  ret = qid_httpc_api_handler(ctx, resp, respn_fields, NUM_KEY_PAIR(respn_fields));
  if(ret < NO_ERROR){
    ctx->last_status.status_code = ret;
    strncpy(ctx->last_status.status_msg, respn_fields[qid_message].value, QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  ret = parse_external_ip( resp, external_ip, ip_len);

  if(NO_ERROR != ret){
    qid_sys_get_local_ip_list(&lan_info);
    for (cnt = 0; cnt < lan_info.lan_ip_count; cnt++) {
      if( strcmp(IP_VERSION4, lan_info.lan_ip_version[cnt]) == 0){
        strncpy( external_ip, lan_info.lan_ip[cnt], ip_len);
        break;
      }else if(strcmp(IP_VERSION6, lan_info.lan_ip_version[cnt]) == 0){
        strncpy( external_ip, lan_info.lan_ip[cnt], ip_len);
        break;
      }
    }
  }
  // get wan IP succcess, set it to the cache
  else
  {
    qid_sys_set_tmp_external_ip(external_ip);
    qid_sys_set_last_get_external_ip_time();
  }

exit:

  return ret;
}
