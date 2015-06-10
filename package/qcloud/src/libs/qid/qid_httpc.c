/*
 * qid_httpc.c
 *
 */

#include <curl/curl.h>
#include <errno.h>
#include <math.h>
#include <json-c/json.h>

#include "std_hdr.h"
#include "comm_hdr.h"
#include "qcloud_util.h"
#include "qcloud.h"
#include "qid_struct.h"
#include "qid_status_code.h"

#include "buff.h"
#include "sha1.h"

#include "qid_httpc.h"
#include "qcloud_gen_apirequest.h"

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

//#define BUFFER_SIZE                 (4*1024)
//#define JSON_TOKENS                 (256)
struct data {
  char trace_ascii; /* 1 or 0 */
  struct qcloud_st *ctx;
};

static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size,
                 char nohex)
{
  size_t i;
  size_t c;

  unsigned int width=0x10;

  if(nohex){
    /* without the hex output, we can fit more on screen */
    width = 0x40;
  }

  for(i=0; i<size; i+= width) {

    if(!nohex) {
      /* hex not disabled, show it */
      for(c = 0; c < width; c++)
        if(i+c < size)
          fprintf(stream, "%02x ", ptr[i+c]);
        else
          fputs("   ", stream);
    }

    for(c = 0; (c < width) && (i+c < size); c++) {
      /* check for 0D0A; if found, skip past and start a new line of output */
      if (nohex && (i+c+1 < size) && ptr[i+c]==0x0D && ptr[i+c+1]==0x0A) {
        i+=(c+2-width);
        break;
      }
      fprintf(stream, "%c",
              (ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.');
      /* check again for 0D0A, to avoid an extra \n if it's at width */
      if (nohex && (i+c+2 < size) && ptr[i+c+1]==0x0D && ptr[i+c+2]==0x0A) {
        i+=(c+3-width);
        break;
      }
    }
    fputc('\n', stream); /* newline */
  }
  fflush(stream);
}

static int my_trace(CURL *handle, curl_infotype type, char *data, size_t size,
                    void *userp)
{
  struct data *config = (struct data *)userp;
  const char *text;
  (void)handle; /* prevent compiler warning */
  FILE *log_fd = pqnap_log->log_fp;

  /* check wheather the log file size is over size limit */
  qnap_log_chk_file_size();

  switch (type) {
  case CURLINFO_TEXT:
    fprintf(log_fd, "== Info: %s", data);
    /* no break */
  default: /* in case a new one is introduced to shock us */
    return 0;
  case CURLINFO_HEADER_OUT:
    text = "=> Send header";
    break;
  case CURLINFO_DATA_OUT:
    text = "=> Send data";
    break;
  case CURLINFO_SSL_DATA_OUT:
    text = "=> Send SSL data";
    return 0;
  case CURLINFO_HEADER_IN:
    text = "<= Recv header";
    break;
  case CURLINFO_DATA_IN:
    text = "<= Recv data";
    break;
  case CURLINFO_SSL_DATA_IN:
    text = "<= Recv SSL data";
    return 0;
  }

  dump(text, log_fd, (unsigned char *)data, size, config->trace_ascii);
  return 0;
}

struct curl_slist * qid_httpc_gen_qid_hdr(struct qcloud_st *ctx, int flag)
{
  struct curl_slist * http_hdr = NULL;
  char hdr_field[TMP_BUFF_LEN+1];
  int rc = -1;

  strncpy(hdr_field, "Accept: application/json", TMP_BUFF_LEN);
  http_hdr = curl_slist_append(http_hdr, hdr_field);
  if(!http_hdr){
    goto exit;
  }

  /*APPID to identify device request*/
  snprintf( hdr_field, TMP_BUFF_LEN, X_QNAP_APPID_HDR_FIELD, ctx->app_info.appid );
  http_hdr = curl_slist_append(http_hdr, hdr_field);
  if(!http_hdr){
    goto exit;
  }

  /*SUBAPP-ID to identify device service*/
  snprintf( hdr_field, TMP_BUFF_LEN, X_QNAP_SUBAPP_ID, ctx->app_info.sub_appid);
  http_hdr = curl_slist_append(http_hdr, hdr_field);
  if(!http_hdr){
    goto exit;
  }

  /*APPVER*/
  snprintf( hdr_field, TMP_BUFF_LEN, X_QNAP_APP_VER_HDR_FIELD, ctx->app_info.appver);
  http_hdr = curl_slist_append(http_hdr, hdr_field);
  if(!http_hdr){
    goto exit;
  }
  /*Append auth-token if it is required*/
  if((flag&QID_HTTPC_FLAG_AUTH) && strlen(ctx->auth_token)){
    snprintf( hdr_field, TMP_BUFF_LEN, OAUTH_HDR_FIELD, ctx->auth_token );
    http_hdr = curl_slist_append(http_hdr, hdr_field);
    if(!http_hdr){
      goto exit;
    }
    qid_sys_get_device_token(ctx->device_token, MAX_APPKEY_LEN);
    snprintf( hdr_field, TMP_BUFF_LEN, X_QNAP_DEVICE_TOKEN_HDR_FIELD, ctx->device_token );
    http_hdr = curl_slist_append(http_hdr, hdr_field);
    if(!http_hdr){
      goto exit;
    }
  }

  if(flag&QID_HTTPC_FLAG_APP_AUTH){
    qnap_id_app_signin(ctx);
    snprintf( hdr_field, TMP_BUFF_LEN, OAUTH_HDR_FIELD, ctx->app_token);
    http_hdr = curl_slist_append(http_hdr, hdr_field);
    if(!http_hdr){
      goto exit;
    }
  }

  if(flag&QID_HTTPC_FLAG_CHECK_SUM){
    /*Calculate checksum*/
    char sha1_string[HEADER_LEN+1];

    time_t curr_time = time(0);
    snprintf( hdr_field, TMP_BUFF_LEN, X_QNAP_TIME_HDR_FIELD, curr_time );
    http_hdr = curl_slist_append(http_hdr, hdr_field);
    if(!http_hdr){
      QNAP_LOG_ERR_NL("failed to insert:  %s", hdr_field);
      goto exit;
    }

    /*Generate the auth string [appkey|appid|timestamp|appver]*/
    snprintf( sha1_string, HEADER_LEN, "%s%s%ld%s", ctx->app_info.appkey,
              ctx->app_info.appid, curr_time, ctx->app_info.appver);
    /*Calculate SHA1 digest*/
    sha1_csum((uint8_t*)sha1_string, strlen(sha1_string), (uint8_t*)hdr_field);
    /* convert the hex array to printable string */
    util_convert_hex_to_char((uint8_t*)hdr_field, SHA1_SUM_LEN, sha1_string, TMP_BUFF_LEN);

    snprintf(hdr_field, TMP_BUFF_LEN, X_QNAP_CHECK_HDR_FIELD, sha1_string);
    http_hdr = curl_slist_append(http_hdr, hdr_field);
    if(!http_hdr){
      QNAP_LOG_ERR_NL("failed to insert:  %s", hdr_field);
      goto exit;
    }
  }

  rc = NO_ERROR;

exit:
  if(rc != NO_ERROR){
    QNAP_LOG_ERR_NL("Failed to append [%s] in HTTP header", hdr_field);
  }

  return http_hdr;
}

static size_t fetch_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
  buf_t *buf = (buf_t *) userp;
  size_t total = size * nmemb;

  if (buf->limit - buf->len < total) {
    buf = buf_size(buf, buf->limit + total);
    if (buf->limit - buf->len < total) {
      QNAP_LOG_ERR_NL("allocate buff failed");
      return 0;
    }
  }

  buf_concat(buf, buffer, total);
  return total;
}

static int handle_http_request (struct qcloud_st *ctx, int flag,
                                HTTP_METHOD_E method, char *url, char *header,
                                char *body, char **resp, size_t resp_buf_size)
{
  CURL *curl = curl_easy_init();
  struct curl_slist *http_hdr = NULL;
  int ret = NO_ERROR;
  long http_code = 0;

  if(!url /*|| !resp*/){
    QNAP_LOG_ERR_NL("invalid parameter");
    ret = ERR_NULL_POINTER;
    goto exit;
  }

  if (!curl) {
    QNAP_LOG_ERR_NL("Create curl object failed");
    ret = ERR_OVERFLOW;
    goto exit;
  }

  if (ctx->http_resp && ctx->http_resp->data){
    ctx->http_resp->len = 0;
    memset( ctx->http_resp->data, 0, ctx->http_resp->limit);
  }else{
    ctx->http_resp = buf_size(NULL, PAYLOAD_LEN_MAX);
  }

  if (!ctx->http_resp) {
    QNAP_LOG_ERR_NL("ctx->http_resp is NULL");
    ret = ERR_OVERFLOW;
    goto exit;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  /*follow redirection*/
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  /* write callback function pointer */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fetch_data);
  /* write callback context */
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, ctx->http_resp);
//  curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);
  switch (method){
  case GET:
    break;
  case POST:
    curl_easy_setopt(curl, CURLOPT_POST, 1L);  //POST
    if(body){
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    }
    break;
  case PUT:
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    if(body){
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    }
    break;
  case DELETE:
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    break;
  default:
    break;
  }

  if(QNAP_LOG_DETAIL_IS_ON()){
    struct data config;
    config.trace_ascii = 1; /* enable ascii tracing */
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &config);
    /* the DEBUGFUNCTION has no effect until we enable VERBOSE */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  }

  /* connect timeout */
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT,  ctx->http_timeout);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT,  ctx->http_timeout);

  /* append HTTP request header.. */
  http_hdr = qid_httpc_gen_qid_hdr(ctx, flag);

  /* append HTTP request header.. */
  if(http_hdr)
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_hdr);

//#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//#endif

//#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//#endif

  ret = curl_easy_perform(curl);
  if (ret != CURLE_OK) {
    QNAP_LOG_ERR_NL("curl_easy_perform failed: %s", curl_easy_strerror(ret));
    QNAP_LOG_ERR_NL("url: %s", url);
    ret = QID_ERR_CONNECT_SERVER;
  }else{
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
  }

exit:

  if(curl){
    curl_easy_cleanup(curl);
  }

  if(http_hdr){
    curl_slist_free_all(http_hdr);
  }

  if(ctx->http_resp){
    if(ret == NO_ERROR){
      *resp = (char*)(ctx->http_resp->data);
      QNAP_LOG_DEBUG_NL("url: %s", url);
      ret = http_code;
      QNAP_LOG_DEBUG_NL("ret:%d", ret);
    }
  }else{
    QNAP_LOG_ERR_NL("response buffer is null");
  }
  return ret;
}

int qid_httpc_oauth_handler(struct qcloud_st *ctx, char * resp, int grant_type,
                            key_pair_st *respn_fields, size_t num_fields)
{
  int ret = ERR_INVALID_VALUE;
  char update_at[TMP_BUFF_LEN+1];
  char* refresh_token, *auth_token, *code, *valid_time, *error_desc;
  char *enc_token = NULL, *enc_refresh_token = NULL;

  if(NO_ERROR != json_parse( resp, respn_fields, num_fields)){
    goto exit;
  }

  refresh_token = respn_fields[qid_oauth_resp_refresh_token].value;
  auth_token = respn_fields[qid_oauth_resp_access_token].value;
  code = respn_fields[qid_oauth_error].value;
  valid_time = respn_fields[qid_oauth_resp_expires_in].value;
  error_desc = respn_fields[qid_oauth_error_description].value;

  ret = qcloud_get_status_code(code);
  if(ret != NO_ERROR || auth_token[0] == '\0'){

    QNAP_LOG_DEBUG_NL("ret: %d", ret);
    QNAP_LOG_ERR_NL("%s [%s]", error_desc, code);

    ctx->last_status.status_code = ret;
    strncpy(ctx->last_status.status_msg, error_desc, QCLOUD_STATUS_MSG_STR_LEN);
    goto exit;
  }

  if( ret == NO_ERROR)
  {

    enc_token = encrypt_str(ctx, auth_token);
    if(!enc_token){
      QNAP_LOG_ERR_NL("Encrypt auth_token");
      ret = ERR_SYSTEM;
      goto exit;
    }

    if((grant_type == OAUTH_GRANT_E_PASSWD || grant_type == OAUTH_GRANT_E_REFRESH_TOKEN)){
      enc_refresh_token = encrypt_str(ctx, refresh_token);
      if(!enc_refresh_token){
        QNAP_LOG_ERR_NL("Encrypt refresh_token");
        ret = ERR_SYSTEM;
        goto exit;
      }

      strncpy(ctx->refresh_token, refresh_token, MAX_APPKEY_LEN);
      qid_sys_set_refresh_token(enc_refresh_token);
      strncpy(ctx->auth_token, auth_token, MAX_APPKEY_LEN);
      qid_sys_set_auth_token(enc_token);
      QID_WRITE_SYSTEM_VAR( ctx->valid_period, valid_time, NAME_LEN,
                            qid_sys_set_token_valid_period, ret);
      ctx->token_last_update_time = time(0);
      snprintf( update_at,TMP_BUFF_LEN, "%d", (int)ctx->token_last_update_time);
      qid_sys_set_last_update_token_time(update_at);
    }else if(grant_type == OAUTH_GRANT_E_CLIENT_CRED){
      strncpy(ctx->app_token, auth_token, MAX_APPKEY_LEN);
      qid_sys_set_app_token(enc_token);
    }

    ctx->last_status.status_code = NO_ERROR;
    ctx->last_status.status_msg[0] = '\0';
  }

exit:

  if(enc_token){
    free(enc_token);
  }

  if(enc_refresh_token){
    free(enc_refresh_token);
  }

  return ret;
}

int qid_httpc_api_handler(struct qcloud_st *ctx, char * resp, key_pair_st *respn_fields,
                          size_t num_fields)
{
  int ret = NO_ERROR;
  /*
   * For checking oauth token refreshment.
   */
  if(ctx->last_status.status_code != NO_ERROR){
    ret = ctx->last_status.status_code;
    goto exit;
  }

  ret = json_parse(resp, respn_fields, num_fields);
  if(ret != NO_ERROR){
    goto exit;
  }

  ret = qcloud_get_status_code(respn_fields[qid_user_code].value);

exit:

  if(ret != NO_ERROR){
    key_pair_st stacktrace = {"stacktrace", "\0"};
    json_parse(resp, &stacktrace, NUM_KEY_PAIR(stacktrace));
    ctx->last_status.status_code = atoi(respn_fields[qid_user_code].value);
    strncpy(ctx->last_status.status_msg, respn_fields[qid_user_msg].value,
            QCLOUD_STATUS_MSG_STR_LEN);
    QNAP_LOG_ERR_NL("QID: [%d] %s", ctx->last_status.status_code,
                    ctx->last_status.status_msg);
    QNAP_LOG_ERR_NL("stacktrace: %s", stacktrace.value);
  }

  return ret;
}

#define REFRESH_TOKEN_BODY  "grant_type=%s&client_id=%s&client_secret=%s&refresh_token=%s"
#define RESEND_REQ_TIMES       (2)

int qid_httpc_refresh_token (struct qcloud_st *ctx)
{
  int ret = NO_ERROR;
  if(!ctx ){
    ret = ERR_NULL_POINTER;
    goto exit;
  }

  key_pair_st respn_fields[] =                        // parsed result of register response
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

  qcloud_reload_config(ctx);

  snprintf(uri, TMP_BUFF_LEN, "%s/oauth/token", ctx->qid_servc_host);
  snprintf(msg_buff, PAYLOAD_LEN_MAX, REFRESH_TOKEN_BODY,
           OAUTH_GRANT_TYPE_REFRESH_TOKEN,
           ctx->app_info.appid, ctx->app_info.appkey, ctx->refresh_token);

  ret = qid_httpc_request_ex(ctx, FALSE, POST, uri, NULL, msg_buff, &resp, PAYLOAD_LEN_MAX);
  if(ret < NO_ERROR){
    goto exit;
  }

  ret = qid_httpc_oauth_handler(ctx, resp, OAUTH_GRANT_E_REFRESH_TOKEN,
                                respn_fields, NUM_KEY_PAIR(respn_fields));

exit:
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("[%s]", get_strerror(ret));
  }
  return ret;
}

#define V1_REFRESH_TOKEN_BODY  "client_id=%s&client_secret=%s&auth_token=%s"

int qid_httpc_refresh_v1_token (struct qcloud_st *ctx)
{
  int ret = NO_ERROR;
  if(!ctx ){
    ret = ERR_NULL_POINTER;
    goto exit;
  }

  key_pair_st respn_fields[] =                        // parsed result of register response
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

  qcloud_reload_config(ctx);

  snprintf(uri, TMP_BUFF_LEN, "%s/oauth/upgrade", ctx->qid_servc_host);
  snprintf(msg_buff, PAYLOAD_LEN_MAX, V1_REFRESH_TOKEN_BODY,
           ctx->app_info.appid, ctx->app_info.appkey, ctx->refresh_token);

  ret = qid_httpc_request_ex(ctx, FALSE, POST, uri, NULL, msg_buff, &resp, PAYLOAD_LEN_MAX);
  if(ret < NO_ERROR){
    goto exit;
  }

  ret = qid_httpc_oauth_handler(ctx, resp, OAUTH_GRANT_E_REFRESH_TOKEN,
                                respn_fields, NUM_KEY_PAIR(respn_fields));

exit:
  if(ret != NO_ERROR){
    QNAP_LOG_ERR_NL("[%s]", get_strerror(ret));
  }
  return ret;
}

/*!
 * \brief Check auth token expired from response message.
 *
 * True for the token is invalid.
 * False for the token is valid.
 * < 0 for invalid response message.
 */
int qid_httpc_is_token_invalid( struct qcloud_st *ctx, char* resp)
{

  int ret = NO_ERROR;
  key_pair_st msg_fields[] =
  {
   {"code",    "\0" },
   {"message", "\0" },
   {"\0", "\0"}
  };

  ret = json_parse(resp, msg_fields, NUM_KEY_PAIR(msg_fields));
  if(ret == NO_ERROR && msg_fields[qid_code].value[0] != '\0'){
    ret = atoi(msg_fields[qid_code].value);
    if(ret == QID_INVALID_AUTH_TOKEN || ret == QID_TOKEN_SCOPE_UNMATCHED){
      ret = TRUE;
    }else{
      ret = FALSE;
    }
  }else{
    QNAP_LOG_DEBUG_NL("Invalid response: %s", resp);
  }

  if(ret != FALSE){
    QNAP_LOG_ERR_NL("ret: [%d]", ret);
  }
  return ret;
}

/*!
 * \brief Check last update time of getting auth token
 *
 * True for the token is or about to expired.
 * False for the token is valid.
 * < 0 for invalid response message.
 */
int qid_httpc_is_token_expired()
{
  int ret = 0;
  char lastUpdate[TMP_BUFF_LEN+1];
  char tokenValidPeriod[TMP_BUFF_LEN+1];
  qid_sys_get_token_valid_period(tokenValidPeriod, TMP_BUFF_LEN);
  qid_sys_get_last_update_token_time(lastUpdate, TMP_BUFF_LEN);
  QNAP_LOG_INFO_NL("lastupdate:%s validperiod:%s sum:%lf cur:%d",
                  lastUpdate,tokenValidPeriod,
                  (atoi(lastUpdate) + atoi(tokenValidPeriod)*AUTH_TOKEN_VALID_LIMIT),
                  (int)time(0));
  if(atoi(lastUpdate) + atoi(tokenValidPeriod)*AUTH_TOKEN_VALID_LIMIT < time(0)){
    ret = TRUE;
  }else{
    ret = FALSE;
  }

  if(ret != FALSE){
    QNAP_LOG_ERR_NL("ret: [%d]", ret);
  }
  return ret;
}


#define HTTPC_RETRY_TIMES       (2)

static char *http_method_str []= {
  "GET",
  "POST",
  "PUT",
  "DELETE"
};

int qid_httpc_request_ex( struct qcloud_st *ctx, int flag,
                          HTTP_METHOD_E method, char *url, char *header,
                          char *body, char **resp, size_t resp_buf_size)
{
#ifdef PERFORMANCE_ANALYSIS
  struct timeval tv, tv2;
  unsigned long long start_utime, end_utime;
  gettimeofday(&tv,NULL);
  start_utime = tv.tv_sec * 1000000 + tv.tv_usec;
#endif
  unsigned char hex_digest[SHA1_SUM_LEN+1];
  char saved_digest[TMP_BUFF_LEN+1];
  char digest_ttl[TMP_BUFF_LEN+1];
  char digest[TMP_BUFF_LEN+1];
  int request_type = 0;
  int ret = NO_ERROR;
  int i;
  char url_list[num_request_data_type][SHA1_SUM_LEN] =
  {
     "ddns",
     "cloudlink",
     "localservice",
     "serviceport",
     "localuser",
     "accesscode",
     "device"
  };

  QNAP_LOG_DEBUG_NL("-----------------------------------------------");
  ctx->last_status.status_code = NO_ERROR;
  ctx->last_status.status_msg[0] = '\0';

  if(flag & QID_HTTPC_FLAG_AUTH){
    GET_TOKEN(qid_sys_get_auth_token, ctx, ctx->auth_token, MAX_APPKEY_LEN);
    GET_TOKEN(qid_sys_get_refresh_token, ctx, ctx->refresh_token, MAX_APPKEY_LEN);
  }

  if( flag & QID_HTTPC_FLAG_REQUEST){

    // get the request type from mapping array.
    request_type = (flag >> 8)-1;
  }else{
    /*
     * this request does not need to check digest.
     */
    goto do_request;
  }
  /*
   * check hash string TTL, set to null_string if expired
   */
  qid_sys_get_hash_string_ttl(url_list[request_type], digest_ttl,TMP_BUFF_LEN);
  if( time(0) > atoi(digest_ttl)){
    QNAP_LOG_DEBUG_NL("Digest[%s]'s TTL(%d) is expired, update",
                      url_list[request_type], atoi(digest_ttl));
    qid_sys_set_hash_string(url_list[request_type], "\0");
  }

  if(flag&QID_HTTPC_FLAG_REQUEST){
    switch( method)
    {
    case DELETE:
  #ifdef CHECK_DELETE_REQUEST
      //TODO: check if we need DELETE to be hashed in future
      /*
       * if saved_digest is "0", then skip
       */
      qid_sys_get_hash_string(url_list[request_type],
                                  saved_digest,TMP_BUFF_LEN);
      if( saved_digest[0] == '0')
        return ret;
  #endif
      /*
       * set hash string to "0" after DELETE
       */
      qid_sys_set_hash_string(url_list[request_type], "\0");
      qid_sys_set_hash_string_ttl(url_list[request_type], 0);
      break;
    case PUT:
    case POST:
      /*
       * calculate sha1 hash string of request body
       */
      sha1_csum((unsigned char*)body, strlen(body), hex_digest);
      util_convert_hex_to_char((uint8_t*)hex_digest, SHA1_SUM_LEN, digest
                                   , TMP_BUFF_LEN);
      /*
       * take hashed string from /etc/config/qid.conf and compare with digest
       */
      qid_sys_get_hash_string(url_list[request_type],
                              saved_digest,TMP_BUFF_LEN);

      /*
       * check if digest and saved_digest are the same or not
       * goto exit if equal
       * make http request if they are different ( and will be saved to /etc/config/qid.conf)
       */
      if( strncmp(digest, saved_digest, SHA1_SUM_LEN*2) == 0 ){

        if (ctx->http_resp && ctx->http_resp->data){
          buf_concat(ctx->http_resp, (uint8_t*)HTTP_RESP_OK, strlen(HTTP_RESP_OK));
          *resp = (char*)(ctx->http_resp->data);
          ret = HTTP_HASHED_RESP_OK;
        }else{
          ret = ERR_BUFF_ALLOC_FAILED;
        }
        goto exit;
      }
      QNAP_LOG_DEBUG_NL("curr digest[%s] not equal with previous[%s]", digest, saved_digest);
      break;
    case GET:
      break;
    default:
      break;
    }
  }

do_request:
  for( i = 0; i < HTTPC_RETRY_TIMES; i++){

    ret = handle_http_request (ctx, flag, method, url, header, body, resp,
                               resp_buf_size);

    if(QID_SERCV_HTTP_OK == ret || ret == NO_ERROR){
      ret = NO_ERROR;
      break;
    }else if(ret == QID_ERR_CONNECT_SERVER ){
      /*
       * connect server failed, try it again.
       */
      continue;
    }

    if (i ==0){
      /* i == 0 for we only tries to refresh token in the first auth failed.
       * try to get a new token.
       */
      ret = qid_httpc_is_token_invalid(ctx, *resp);
      if(ret != TRUE){
        break; // no business with tokens.
      }
      /*
       * if the token is expired then try to get a new one.
       */
      if((flag&QID_HTTPC_FLAG_AUTH)){
        int refresh_ret = qid_httpc_refresh_token(ctx);
        if((refresh_ret == NO_ERROR) || (QID_SERCV_HTTP_OK == refresh_ret)){
          // refresh token successfully, try this request again.
          continue;
        }
      }else if(flag&QID_HTTPC_FLAG_APP_AUTH){
        ctx->app_token[0] = '\0';
        qid_sys_set_app_token(ctx->app_token);
        /*
         * clean up app token and try it again. the in gen_qid_header, we will
         * get a new token again.
         */
        continue;
      }
    }
    break; // break the for loop for http request retry
  }

  /*
   * save digest string if http request was successfully made
   */
  if(NO_ERROR == ret && (flag&QID_HTTPC_FLAG_REQUEST)){
    switch( method)
    {
    case PUT:
    case POST:
      /*
       * calculate sha1 hash string of request body
       */
      sha1_csum((unsigned char*)body, strlen(body), hex_digest);
      util_convert_hex_to_char((uint8_t*)hex_digest, SHA1_SUM_LEN, digest
                                   , TMP_BUFF_LEN);

      /*
       * save digest string to /etc/config/qid.conf
       */
      qid_sys_set_hash_string(url_list[request_type], digest);

      memset(digest_ttl, 0 ,TMP_BUFF_LEN);


#ifdef DEBUG
#define HASH_STRING_VALID_PERIOD (5)
#else
#define HASH_STRING_VALID_PERIOD (86400)
#endif
      qid_sys_set_hash_string_ttl(url_list[request_type],
                                  (int)time(0)+HASH_STRING_VALID_PERIOD);

      break;

    case GET:
      break;
    default:
      break;
    }
  }

exit:
  QNAP_LOG_DEBUG_NL("Appid: %s", ctx->app_info.appid);
  QNAP_LOG_DEBUG_NL("Sub-Appid: %s", ctx->app_info.sub_appid);
  QNAP_LOG_INFO_NL("%s %s ", http_method_str[method], url);
  if(ret == NO_ERROR){
    QNAP_LOG_INFO_NL("Done");
  }else{
    if(ret < NO_ERROR){
      QNAP_LOG_ERR_NL("Internal error: %s", get_strerror(ret));
    }else if (ret == HTTP_HASHED_RESP_OK){
      QNAP_LOG_INFO_NL("is cached");
    }else{
      QNAP_LOG_ERR_NL("%d", ret);
    }
  }
#ifdef PERFORMANCE_ANALYSIS
  gettimeofday(&tv2,NULL);
  end_utime = tv2.tv_sec * 1000000 + tv2.tv_usec;
  QNAP_LOG_INFO_NL("http request elapse time :%llu", end_utime - start_utime);
#endif
  QNAP_LOG_DEBUG_NL("-----------------------------------------------");
  return ret;
}

int qid_httpc_request(struct qcloud_st *ctx, int flag,
                      HTTP_METHOD_E method, char *url, char *data, char **resp)
{
  int ret = 0;
  int i;

  for( i = 0; i < HTTPC_RETRY_TIMES; i++){
    ret = handle_http_request(ctx, flag, method, url, NULL, data, resp,
                               PAYLOAD_LEN_MAX);
    if( ret == QID_ERR_CONNECT_SERVER ){
      /*
       * connect server failed, try it again.
       */
      continue;
    }
    break;
  }

  if(QID_SERCV_HTTP_OK == ret)
    ret = NO_ERROR;

  return ret;
}

#define OAUTH_ERR(code, s) { code, s }

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

typedef struct error_set{
  int code;
  const char *msg;
}error_set;

error_set oauth_errorstr[] = {
  OAUTH_ERR(OAUTH_ERR_INVALID_REQ, "invalid_request"),
  OAUTH_ERR(OAUTH_ERR_INVALID_CLI, "invalid_client"),
  OAUTH_ERR(OAUTH_ERR_INVALID_USER, "invalid_user"),
  OAUTH_ERR(OAUTH_ERR_INVALID_PASS, "invalid_password"),
  OAUTH_ERR(OAUTH_ERR_INVALID_GRANT, "invalid_grant"),
  OAUTH_ERR(OAUTH_ERR_QID_NOT_VERIFIED, "user_not_activated"),
  OAUTH_ERR(OAUTH_ERR_SERVER, "server_error"),
  OAUTH_ERR(NO_ERROR, "0")
};

int qcloud_get_status_code( char * code_str )
{
  int cnt = 0;

  /*
   * In auth response, if there is no error happened, then code is not existent.
   */
  if( code_str[0] == '\0'){
    return NO_ERROR;
  }

  /*
   * check if 'error' is not in the format for integer,
   * which may occur in oauth related operations
   */
  for( cnt = 0; cnt < ARRAY_SIZE(oauth_errorstr); cnt++){
    if( strncmp(oauth_errorstr[cnt].msg,
                code_str, strlen(oauth_errorstr[cnt].msg)) == 0){
      return oauth_errorstr[cnt].code;
    }
  }
  return atoi( code_str);

}

