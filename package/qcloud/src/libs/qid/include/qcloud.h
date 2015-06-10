#ifndef QCLOUD_H_
#define QCLOUD_H_

#include <stdint.h>

#define STATUS_FLAG_LEN                       (16)
#define QID_DEVICE_NAME_LEN                   (30)
#define QID_DEVICE_ID_LEN                     (256)
#define QID_MAC_STR_LONG_LEN                  (20)
#define QID_SN_STR_LEN                        (512)

//@{
/**@name Constants definitions*/
//! Version of qid.conf
#define QID_CONFIG_VERSION                "1"
//! Supported QCloud Version.
#define QID_API_VERSION                   "v1.1"
//! The max. length of QID host
#define MAX_QID_HOST_LEN                  (254)
//! The max. length of QID.
#define MAX_QID_LEN                       (254)
//! The max. length of password.
#define MAX_PASSWD_LEN                    (512)
//! The max. length of APP ID
#define MAX_APPID_LEN                     (256)
//! The max. length of APP Key
#define MAX_APPKEY_LEN                    (256)
//! The max. length of APP Version value
#define MAX_APPVER_LEN                    (64)
//! Max. length of DDNS service name
#define DDNS_SERVC_NAME_LEN               (32)
//! Max. length of protocol name of port mapping
#define POROT_MAPPING_PROTO_NAME_LEN      (4)
//! Maximum length of status message
#define QCLOUD_STATUS_MSG_STR_LEN         (512)
//! Max. length of published service name
#define QCLOUD_PUBLISHED_SERVC_NAME_LEN   (32)
//! Max. len of GEO code
#define GEOINFO_STR_LEN                   64
//! Max. len of file path
#define MAX_PATH_LEN                      (32*1024)
//@}

//@{
/**@name Error code definitions*/
//! operation done successfully.
#define QID_ERR_NO_ERROR          (0)
//! invalid input parameter. */
#define QID_ERR_INVALID_PARAM     (-1)
//! given buffer size it too small.
#define QID_ERR_BUFFER_LEN        (-2)
//! User name/password, or auth token is NOT matched.
#define QID_ERR_INVALID_CREDENTIAL (-3)
//! There is no internet connection.
#define QID_ERR_NO_INTERNET       (-10)
//! Connect to server failed.
#define QID_ERR_CONNECT_SERVER    (-11)
//! Unexpected response from server.
#define QID_ERR_UNKNOWN_RESPONSE  (-12)
//! Wait for the response from server timeout.
#define QID_ERR_RESPONSE_TIME     (-13)
//! The given resource/ID is not found.
#define QID_ERR_RES_NOT_FOUND     (-100)
//! This service is not signed in by this device.
#define QID_ERR_SERVICE_NO_SIGNIN (-101)
//! The registered ID is waiting for mail confirm.
#define QID_ERR_QID_VALIDATING    (-102)
//! Server response registeration failed
#define QID_ERR_QID_REGISTER_FAILED (-103)
//! This email has been registered already.
#define QID_ERR_QID_ALREADY_USED  (-104)
//! The device have been used.
#define QID_ERR_DEVICE_NAME_ALREADY_USED (-105)
//! The checked QID has been verified.
#define QID_ERR_QID_VALIDATED      (-106)
//! This request will not be executed.
#define QID_ERR_WONT_EXECUTED       (-107)
//! The given password is checked error by server
#define QID_ERR_INVALID_QID_PASSWD  (-108)
//! The given QID is not found by server.
#define QID_ERR_QID_NOT_FOUND       (-109)
//!
#define QID_SERVC_AUTH_TOKEN_NOT_FOUND    (100009)
//! UNKNOWN ERROR
#define ERR_UNKNOWN_ERROR                 (1000001)

//! Invalid OAUTH token, need to refresh
#define QID_INVALID_AUTH_TOKEN            (1013001)
//! Access token scope unmatched: assigned scope {token_scope}, api scope={api_scope}
#define QID_TOKEN_SCOPE_UNMATCHED         (1013002)
//! The given QNAP user account is NOT found.
#define QID_SERVC_QID_NOT_FOUND           (10131006)
//! The given QID password is wrong.
#define QID_SERVC_WRONG_PASSWORD          (10131007)

//! The device '{device_id}' is not found.
#define MCN_ERR_DEVICE_ID_NOT_FOUND       (1020001)
//! The given device name has been used
#define QID_ERR_DEVICE_NAME_BEEN_USED     (1020002)
//! IGD does not own public IP address
#define MCN_ERR_IGD_NOT_PUBLIC            (1020010)
//! IGD IP detection error
#define MCN_ERR_IGD_IP_DETECT             (1020011)
//! Backend server json format error
#define MCN_ERR_JSON_FORMAT               (1020012)
//! IGD does not able to offer upnp service
#define MCN_ERR_IGD_UPNP_NA               (1020013)
//! The ddns of device '{device_id}' is not found.
#define MCN_ERR_DDNS_NAME_NOT_FOUND       (1060001)
//! The ddns of device '{device_id}' exists.
#define MCN_ERR_DDNS_EXISTS               (1060003)
//!  The wan ip of device is empty
#define MCN_ERR_EMPTY_WAN_IP              (1060004)
//! The cloud link of device '{device_id}' is not found.
#define MCN_ERR_VLINK_NOT_FOUND           (1070001)
//! The cloud link of device '{device_id}' exists.
#define MCN_ERR_VLINK_EXISTS              (1070002)
//! The qpkg with given name is not found
#define QID_ERR_QPKG_NOT_FOUND            (1030002)

//@}

//@{
//!@name Log setting flags
//! print log to stderr; default output target.
#define QID_LOG_STDERR       1<<0
//! print log to stdout. Do not enable this flag, if caller is CGI.
#define QID_LOG_STDOUT       1<<1
//! print log to file, /tmp/qid.log.
#define QID_LOG_FILE         1<<2
//! enable to display curl log; default off.
#define QID_LOG_CURL         1<<5
//@}a

//! Application information context object.
typedef struct _qnap_app_info
{
  //! application ID for device, a signed accii string; max length 200.
  char appid[MAX_APPID_LEN+1];
  //! application ID for device service, a signed accii string; max length 200.
  char sub_appid[MAX_APPID_LEN+1];
  //! application key, a signed accii string; max length 200.
  char appkey[MAX_APPKEY_LEN+1];
  //! application version, a signed accii string; max length 64
  char appver[MAX_APPVER_LEN+1];
}qnap_app_info;

//! Device information saved at NAS config.
typedef struct _qnap_dev_info
{
  //! QID account; user's email
  char qid_account[MAX_QID_LEN+1];
  //! Hardware serial number
  char hw_sn[QID_SN_STR_LEN+1];
  //! Device's eth0 MAC string with ':'
  char mac_str[QID_MAC_STR_LONG_LEN+1];
  //! QID status: 0 for checked out, 1: QID validating, 2: validated, 3: checked-in*/
  char qid_status[STATUS_FLAG_LEN+1];
  ///! device id at mycloudnas service
  char device_id[QID_DEVICE_ID_LEN+1];
  //! device name of mycloudnas service.
  char device_name[QID_DEVICE_NAME_LEN+1];
  //! status flag of mycloudNAS DDNS service {"Disable", "Enable"}
  char ddns_status[STATUS_FLAG_LEN+1];
  //! server host name of DDNS server
  char ddns_server_host[MAX_QID_HOST_LEN+1];
  //! the timestamp of last update sucessfully
  time_t last_update_success_timestamp;
  //! the timestamp of last update fail
  time_t last_update_fail_timestamp;
  //! the error code of last update fail
  int last_update_error_code;
}qnap_dev_info;

//!
typedef enum qcloud_name_status_e{
  //! This name is okay to use
  QCLOUD_NAME_FREE,
  //! This name has already been used
  QCLOUD_NAME_BEEN_USED,
  //! Invalid device name format
  QCLOUD_NAME_INVALID_FORMAT
}QCLOUD_NAME_STATUS_E;

//@{
//! Returned code/message from API server
typedef struct qcloud_returned_msg_st_{
  //! Server returned status code
  int status_code;
  //! Server returned message
  char status_msg[QCLOUD_STATUS_MSG_STR_LEN+1];
}QCLOUD_STATUS_MSG;
//@}

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Create and initialize context object for QNAP ID API services.
 *
 *  \return A context pointer will be returned back, callee needs to call
 *          qnap_id_cleanup() to free this object. Returns NULL for error.
 *
 */
struct qcloud_st* qcloud_init_ctx(qnap_app_info *info);

/*!
 * \brief Free context object.
 *
 * @param ctx context object of libqcloud
 *
 */
void qcloud_cleanup_ctx(struct qcloud_st *ctx);

/*!
 * \brief reset config file
 *
 * @param ctx context object of libqcloud
 *
 * @return
 *            \li \c QID_ERR_NO_ERROR Successfully getting device status.
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int reset_qcloud_ctx_conf(struct qcloud_st *ctx);

/*!
 * \brief reset hash files
 *
 * @return
 *            \li \c QID_ERR_NO_ERROR Successfully getting device status.
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int reset_qcloud_request_hash();

/*!
 * \brief set log output target, and its level.
 *
 * This function needs to be called after qnap_id_init_ctx()
 *
 * @param ctx context object of libqcloud
 * @param mode
 * @param level Logging level
 */
void qcloud_set_log(struct qcloud_st *ctx, int mode, int level);

/*!
 * \brief Get device information from /etc/config/qid.conf.
 *
 *  @return
 *            \li \c QID_ERR_NO_ERROR Successfully getting device status.
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int qcloud_get_device_info(struct qcloud_st *ctx, qnap_dev_info *dev_info);

/*!
 * \brief Check functionality of UPnP port mapping setting
 *
 * @param ctx context object of libqcloud
 * @param protocol    http or https
 * @param extl_port   External port number for testing.
 * @param path
 * @param timeout     Timeout value to stop testing function; default is 20 seconds.
 * @param out_res_buf
 * @param out_resp_buf_len
 *
 * @return
 *          \li \c QID_ERR_NO_ERROR This service port could be accessed from public server.
 *          \li \c Otherwise for checking service port failed.
 *
 */
int qcloud_fetch_web_page(struct qcloud_st *ctx,
  const char* protocol, uint16_t extl_port,
  const char* path, time_t timeout,
  char* out_res_buf, int out_resp_buf_len);

/*!
 * \brief Query device status from qcloud API server.
 *
 *  @param ctx status data for QCLOUD API
 *
 *  @return
 *            \li \c QID_ERR_NO_ERROR This device is in checked-in status at server,
 *            \li \c ERR_NULL_POINTER Invalid input parameter.
 *            \li \c QID_ERR_NO_INTERNET  Connect server failed.
 *            \li \c QID_ERR_INVALID_CREDENTIAL Device token failed, or in checked-out status;
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int qcloud_check_binding_status(struct qcloud_st *ctx);

/*!
 * \brief Get external IP
 *
 *  @param ctx status data for QCLOUD API
 *  @param buf Buffer to store external_ip output
 *  @param buf_len Length of the buffer
 *
 *  @return
 *            \li \c QID_ERR_NO_ERROR Service status is changed successfully.
 *            \li \c ERR_NULL_POINTER Invalid input parameter.
 *            \li \c QID_ERR_NO_INTERNET  Connect server failed.
 *            \li \c QID_ERR_INVALID_CREDENTIAL Device token failed, or in checked-out status;
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int qcloud_get_external_ip(struct qcloud_st *ctx, char* buf, size_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* QCLOUD_H_ */
