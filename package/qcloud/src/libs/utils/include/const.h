/*
 * const.h
 *
 */

#ifndef CONST_H_
#define CONST_H_


/*
 * Redefine TRUE and FALSE, just in case.
 */
#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE                        1
#define FALSE                       0

#ifndef __PACKED__
#define __PACKED__                  __attribute__((__packed__))
#endif

#define VLINK_ID_LEN                32
#define NAS_ID_LEN                  64
#define DOMAIN_NAME_LEN             256
#define USER_NAME_LEN               256
/*
 * Constant defines.
 */
#define SYSTEM_NAME_LEN             64      // length of system name
#define SYSTEM_TIME_LEN             64      // length of system time in string
#define UUID_LEN                    36      // length of UUID string

#define ACCESS_KEY_LEN              (UUID_LEN + MAC_STR_LEN)
#define VENDOR_ID_LEN               32      // max length of vendor ID
#define MODEL_LEN                   32      // max length of model string
#define MODULE_LEN                  15      // max length of module name
#define VERSION_LEN                 32      // max length of FW version
#define CLIENT_ID_LEN               128     // max length of client ID
#define DEVICE_ID_LEN               256     // max length of device ID
#define SESSION_ID_LEN              64      // session ID is composed by
                                            // uuid + mac_addr
#define NAME_LEN                    32      // max length of device name
#define HEADER_LEN                  1024
#define PROFILE_LEN                 512
#define DEALER_LIST_LEN             (10)
#define MAX_UNIX_DOMAIN_PATH        108
#define VENDOR_ID_DEF               "QNAP"

#define IPv4_ALEN                   4       // 4 bytes of IPv4 address
#define IP_STR_LEN                  30      // formatted IP string length
#define MAC_STR_LEN                 12      // MAC string without ':'
#define MAC_STR_LONG_LEN            18      // MAC string with ':'
#define IP_HOST_LEN                 128     // host name length
#define URL_STR_LEN                 256     // same as IP_HOST_LEN
#define ADDR_STR_LEN                512     // IP:Port lists
#define META_STR_LEN                512     // recording meta
#define PROTO_STR_LEN               12      // max length of protocol name, e.g., HTTP/ HTTPS
/*
 * Assume packet header is not larger than 32.
 * This value should be updated accordingly if the packet format defined in
 * qnap_pkt.h changes.
 */
#define PKT_HDR_LEN_MAX             12      // maximum length of packet header
#define PAYLOAD_LEN_MAX             4096    // maximum length of payload data
#define PAYLOAD_LEN_1K              1024    // most output packet size
#define PKT_BUFF_LEN                (PKT_HDR_LEN_MAX + PAYLOAD_LEN_MAX)
#define PKT_BUFF_LEN_1K             (PKT_HDR_LEN_MAX + PAYLOAD_LEN_1K)
#define CMD_BUFF_LEN                256     // command buffer
#define CONF_BUFF_LEN               512     // for parameter config.
#define TMP_BUFF_LEN                1024    // misc. use of any tmp buffer
#define QID_CONF_BUFF_LEN           32768
#define DISP_BUFF_LEN               80
/*
 * Maximum 256K bytes allowed to be queued per connection.
 */
#define PKT_QUEUED_LEN_MAX          (32 * PKT_BUFF_LEN_1K)
#define HALF_PKT_QUEUED_LEN_MAX     (PKT_QUEUED_LEN_MAX/2)

#define HTTP_BUFF_LEN_MAX           40960 // maximum length of HTTP

#define PORT_MIN                    1       // min tcp/udp port
#define PORT_MAX                    65535   // max tcp/udp port
#define IPC_PORT_MIN                1025    // min ipc port
#define IPC_PORT_MAX                PORT_MAX// max ipc port
#define PORT_STR_LEN                5
#define CONNECTION_BACKLOG          128     // max. connections in listen queu

/*
 * Up/Down status.
 */
#define STATUS_UP                   TRUE
#define STATUS_DOWN                 FALSE

/*
 * Directory path.
 */
#define DIR_SPLITTER_CHAR           '/'
#define COMMENT_CHAR                '#'
#define DOUBLE_QUOTE_CHAR           '\"'
#define SINGLE_QUOTE_CHAR           '\''

#define GET_DEALER_REQ_FMT          "GET /v1/dealers HTTP/1.0\r\n"\
                                    "Host: %s:%d\r\n\r\n"
#define GET_DEALER_REQ_FMT_V11      "GET /v1.1/usher/dealer?service_type=tcp HTTP/1.0\r\n"\
                                    "Host: %s:%d\r\n\r\n"

/*
 * Server address/URL.
 */
#define INADDR_ANY_STR              "0.0.0.0"
#define MMC_SVR_ADDR_DEF            INADDR_ANY_STR ":11211"

/*
 * Default server port number.
 * NOTE: should be consistent with the above.
 */
#define QID_SVR_PORT_DEF            443
#define MCN_SVR_PORT_DEF            443
#define USHER_SVR_PORT_DEF          80
#define DEALER_SVR_CTRL_PORT_DEF    443
#define DEALER_SVR_PROXY_PORT_DEF   3128
#define MMC_SVR_PORT_DEF            11121
#define PORTER_NOTIF_PORT_DEF       22336
#define IP_PORT_LIST_SEP_CHAR       ','
#define IP_PORT_SEP_CHAR            ':'
#define URL_NAME_PORT_CHAR          ':'
#define URL_NAME_PREFIX_STR         "://"
#define URL_NAME_PATH_CHAR          '/'

#define QNAP_EAGENT_LOG_PATH            "/tmp/.eagent.log"
#define QNAP_PORTER_LOG_PATH            "/tmp/.porter.log"
#define QNAP_QID_LOG_PATH               "/tmp/.qid.log"
#define QNAP_PLAYER_LOG_PATH            "/tmp/.player.log"
#ifdef ENABLE_TUTK_LOG
#define TUTK_IOTC_LOG_PATH              "/tmp/.tutk_iotc"
#define TUTK_RDT_LOG_PATH               "/tmp/.tutk_rdt"
#define TUTK_P2P_LOG_PATH               "/tmp/.tutk_tl"
#else
#define TUTK_IOTC_LOG_PATH              "/tmp/.tutk_iotc"
#define TUTK_RDT_LOG_PATH               NULL
#define TUTK_P2P_LOG_PATH               NULL
#endif

#define ERROR_MYQNAPCLOUD               "[myQNAPcloud] "

#define PORTER_DOMAIN_SOCKET_PATH       "/tmp/porter.sock"

// categorized by customer service responding method
//A
#define ERROR_HANDLING_ACTION_CHECK_DNS "Please check your DNS setting on NAS"
//B
#define ERROR_HANDLING_RESTART_CLOUDLINK "Please restart your CloudLink by disabling it and enabling it again"
//C
#define ERROR_HANDLING_CHECK_BLOCKING   "Please make sure your device is connected to the Internet and the UDP traffic is not blocked"
//D
#define ERROR_HANDLING_UPGRADE          "Please upgrade the firmware to latest version"
//H
#define ERROR_HANDLING_CHECK_INTERNET   "Please make sure your device is connected to the Internet"
// NEW
#define ERROR_HANDLING_QCLOUD_SERVER    "Please try again later"
// NEW
#define ERROR_HANDLING_SIGNIN_AGAIN     "Please sign in again"

#define QID_ERR_CAUSE_DEVICE_INFO       "failing to get device information %s"
#define QID_ERR_CAUSE_QID_MIGRATE       "hard disk migration"
#define QID_ERR_CAUSE_DEVICE_NOT_FOUND  "device not found at cloud server"
#define QID_ERR_CAUSE_INVALID_AUTH_TOKEN  "invalid or expired authentication token"
#define QID_ERR_CAUSE_UNABLE_TO_BIND    "being unable to bind the device to the cloud"
#define QID_ERR_CAUSE_EMPTY_WAN_IP      "WAN IP of the device is unknown"
#define QID_ERR_CAUSE_CONNECTION        "Internet connection error"
#define QID_ERR_CAUSE_PARSE_RESPONSE    "failing to parse server response "
#define QID_ERR_CAUSE_SERVER_INTERNAL   "server internal error"

#define CLOUD_LINK_SUCCESS              "CloudLink set up successfully"
#define CLOUD_LINK_INIT_ERR             "CloudLink cannot be initialized, %s. error code: [%d]"
#define CLOUD_LINK_START_ERR            "CloudLink cannot start, %s. error code: [%d]"
#define CLOUD_LINK_LOGIN_ERR            "CloudLink cannot set up, %s. error code: [%ld]"

#define QID_SYS_LOG_CHECKED_IN          "User [%s] has been signed in, and device name is [%s]"
#define QID_SYS_LOG_CHECKED_OUT         "User [%s] has been signed out manually, and the device name is [%s]"
#define QID_SYS_LOG_ERR_CHECKED_OUT     "User [%s] has been signed out (device name: [%s]) due to %s (error code: [%d]). %s."
#define QID_SYS_LOG_UPDATE_DEVICE_NAME  "Device name is changed to [%s] "
#define QID_SYS_LOG_CONFIG_FILE_WRITE_ERROR "Unable to write cloud agent configuration file"
#define QID_SYS_LOG_CONFIG_FILE_READ_ERROR  "Unable to read cloud agent configuration file"

#define MCN_SYS_LOG_DDNS_TURN_ON        "DDNS has been turned on"
#define MCN_SYS_LOG_DDNS_TURN_OFF       "DDNS has been turned off"
#define MCN_SYS_LOG_TUTK_TURN_ON        "CloudLink has been turned on"
#define MCN_SYS_LOG_TUTK_TURN_OFF       "CloudLink has been turned off"

#define MCN_SYS_LOG_DDNS_UPDATE_FAILED         "Unable to update DDNS setting due to %s (error code: [%d]). %s."
#define MCN_SYS_LOG_UPDATE_STATUS_FAILED       "Unable to update device status due to %s (error code: [%d]). %s."
#define MCN_SYS_LOG_PUBLISHED_FAILED           "Unable to update published list due to %s (error code: [%d]). %s."
#define MCN_SYS_LOG_PORT_MAPPING_FAILED        "Unable to update port mapping due to %s (error code: [%d]). %s."
#define MCN_SYS_LOG_ACCESS_CODE_FAILED         "Unable to update user access code due to %s (error code: [%d]). %s."
#define MCN_SYS_LOG_UPDATE_USER_LIST_FAILED    "Unable to update user list due to %s (error code: [%d]). %s."

#define MCN_SYS_LOG_SEND_SHARE_LINK_NOTIFY_FAILED  "Cloud agent cannot send share link notification, error code: [%d]"

#define QID_PUSH_NOTIFICATION_PAIRED 			"Paired %s"
#define QID_PUSH_NOTIFICATION_UNPAIRED 			"Unpaired %s"

#endif /* CONST_H_ */
