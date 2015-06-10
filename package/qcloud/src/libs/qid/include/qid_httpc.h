/*
 * qid_httpc.h
 *
 *  Created on: Oct 18, 2012
 *      Author: ch
 */

#ifndef QID_HTTPC_H_
#define QID_HTTPC_H_

#include "qid_api.h"
#include "mcn_api.h"
#include "qid_struct.h"

#define QID_HTTPC_FLAG_NONE         0x00000000
#define QID_HTTPC_FLAG_AUTH         0x00000001        //This HTTP request will be sent with auth token.
#define QID_HTTPC_FLAG_PROXY        0x00000002        //This Request will be sent through HTTP Proxy.
#define QID_HTTPC_FLAG_CHECK_SUM    0x00000004        //Add v1 API checking sum in HTTP header.
#define QID_HTTPC_FLAG_REQUEST      0x00000008        //Flag to identify digest check (cached).
#define QID_HTTPC_FLAG_APP_AUTH     0X00000010        // Flag to identify to use APP id and APP secret.

#define QID_HTTPC_FLAG_DDNS         (0x0100|QID_HTTPC_FLAG_REQUEST)
#define QID_HTTPC_FLAG_CLOUDLINK    (0x0200|QID_HTTPC_FLAG_REQUEST)
#define QID_HTTPC_FLAG_SERVICE      (0x0300|QID_HTTPC_FLAG_REQUEST)
#define QID_HTTPC_FLAG_PORTMAPPING  (0x0400|QID_HTTPC_FLAG_REQUEST)
#define QID_HTTPC_FLAG_USERS        (0x0500|QID_HTTPC_FLAG_REQUEST)
#define QID_HTTPC_FLAG_ACCESSCODE   (0x0600|QID_HTTPC_FLAG_REQUEST)
#define QID_HTTPC_FLAG_DEVICE       (0x0700|QID_HTTPC_FLAG_REQUEST)

#define X_QNAP_MAC_HDR_FIELD        "X-QNAP-MAC: %s"
#define X_QNAP_HW_SN_HDR_FIELD      "X-QNAP-HW-SN: %s"
#define X_QNAP_MODEL_HDR_FIELD      "X-QNAP-MODEL: %s"
#define X_QNAP_FW_VER_HDR_FIELD     "X-QNAP-FW-VER: %s"
#define X_QNAP_APPID_HDR_FIELD      "X-QNAP-APP-ID: %s"
#define X_QNAP_APP_VER_HDR_FIELD    "X-QNAP-APP-VER: %s"
#define X_QNAP_TIME_HDR_FIELD       "X-QNAP-TIME: %ld"
#define X_QNAP_CHECK_HDR_FIELD      "X-QNAP-CHECK: %s"
#define OAUTH_HDR_FIELD             "Authorization: Bearer %s"
#define X_QNAP_DEVICE_TOKEN_HDR_FIELD "X-QNAP-DEVICE-TOKEN: %s"
#define X_QNAP_SUID_HDR_FIELD       "X-QNAP-SUID: %s"
#define X_QNAP_SUBAPP_ID            "X-QNAP-SUBAPP-ID: %s"


#define HTTP_RESP_OK                "{\"message\": \"OK.\", \"code\": 0}"
#define HTTP_HASHED_RESP_OK         (100)
#define AUTH_TOKEN_VALID_LIMIT      (0.9)

typedef enum _METHOD{
  GET = 0,
  POST,
  PUT,
  DELETE
}HTTP_METHOD_E;

enum request_data_type{
  e_ddns = 0,
  e_cloudlink,
  e_localservice,
  e_service_port,
  e_users,
  e_access_code,
  e_device_info,
  num_request_data_type
}request_data_type_E;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @return > 0 Gets response from server, but not 200 OK.
 *         = 0 No error happened during processing HTTP request, and gets 200 OK.
 *         < 0 Error happened during processing HTTP request.
 */
int qid_httpc_request_ex( struct qcloud_st *ctx, int flag,
                          HTTP_METHOD_E method, char *url, char *header,
                          char *body, char **resp_size, size_t resp_buf_size);

int qid_httpc_request( struct qcloud_st *ctx, int flag,
                       HTTP_METHOD_E method, char *url, char *data, char **resp);

/*
 * refresh OAUTH2 token.
 */
int qid_httpc_refresh_token (struct qcloud_st *ctx);

/*
 * use to refresh v1 devcie auth token to OAUTH2 token.
 */
int qid_httpc_refresh_v1_token (struct qcloud_st *ctx);

int qid_httpc_oauth_handler(struct qcloud_st *ctx, char * resp, int grant_type,
                            key_pair_st *respn_fields, size_t num_fields);

int qid_httpc_api_handler(struct qcloud_st *ctx, char * resp,
                          key_pair_st *respn_fields, size_t num_fields);

int qid_httpc_is_token_invalid( struct qcloud_st *ctx, char* resp);

int qid_httpc_is_token_expired();


#ifdef __cplusplus
}
#endif


#endif /* QID_HTTPC_H_ */
