/*
 * error_code.h
 *
 *  Created on: Apr 18, 2011
 *      Author: ch
 */

#ifndef ERROR_CODE_H_
#define ERROR_CODE_H_

#define ERR_NONE                                    (0)

#ifndef S_OK
#define S_OK                                        ERR_NONE
#endif

#define EA_ERR_NONE                                 ERR_NONE
#define TUNNEL_ERR_NONE                             ERR_NONE
#define EA_TLS_DONE                                 ERR_NONE
#define QNAP_NO_ERROR                               ERR_NONE
#define NO_ERROR                                    ERR_NONE

typedef enum _errno
{

/* mycloudnas service error */
    ERR_MCN_END            =   -3999,
    ERR_MCN_NO_SERVER      =   -3002,
    ERR_MCN_NOT_ENABLED    =   -3001,
    ERR_MCN_START          =   -3000,

/*TLS error (-5999 ~ -5000 */
    TSA_TLS_END = -5999,
    TSA_TLS_ERROR_MISC = -5998,
    TSA_TLS_SYSCALL,
    TSA_TLS_ZERORETURN,
    TSA_TLS_ERROR_IO,
    TSA_TLS_CONNREFUSED,
    TSA_TLS_CONNRESET,
    TSA_TLS_NO_ROUTE,
    TSA_TLS_TIMEOUT,
    TSA_TLS_CLOSE, // -5990
    TSA_TLS_WANTREAD,
    TSA_TLS_WANTWRITE,

/* socket error (-2999 ~ -2000) */
    ERR_SOCK_END            =   -2999,
    ERR_SOCK_BADTYPE,                          // invalid socket type
    ERR_SOCK_IMPL,                             // not implemented.
    ERR_SOCK_BADSOCK,                          // invalid socket accessing
    ERR_SOCK_TIMEOUT,                          // request timeout
    ERR_SOCK_REFUSED,                          // request is refused
    ERR_SOCK_NOCONN,                           // connection is not available.
    ERR_SOCK_NOBIND,                           // binding operation failed.
    ERR_SOCK_BADADDR,                          // invalid socket addr.
    ERR_SOCK_NOLISTEN,                         // listening operation failed.
    ERR_SOCK_NOACCEPT,                         // accepting operation failed.
    ERR_SOCK_WOULDBLOCK,                       // current operation would be blocked for a while.
    ERR_SOCK_BADOPT,                           // invalid socket operation.
    ERR_SOCK_BADFD,                            // invalid file description
    ERR_SOCK_INVAL,                            // invalid paramter.
    ERR_SOCK_UNCONN,                           // seems the same as noconn
    ERR_SOCK_INPROGRESS,                       // operation is working.
    ERR_SOCK_RECV,                             // receiving failed.
    ERR_SOCK_SEND,                             // sending failed.
    ERR_SOCK_OUTBOUND,                         // ??
    ERR_SOCK_STOP,                             // socket thread stopped.
/* end of socket error */


    ERR_BUFF_END            =   -1999,
    ERR_BUFF_INVALID,                           // invalid parameter
    //ERR_BUFF_OUTOFBOUND,                      // memory access is out of bound
    ERR_BUFF_OUTOFMEM,                          // run out of memory
    ERR_BUFF_ALLOC_FAILED,                      // allocate memory failed
    ERR_BUFF_SHUFFLE_FAILED,                    // failed to rearrange memory resource

/* end of buffer error */

/* generic error (-0999 ~ -0001) */

    ERR_GENERIC_END         = -999,
    ERR_EVENT_VERSION,                          // event version mis-match
    ERR_EVENT_LOOP,                             // failed to set up main event loop
    ERR_STR_TOO_LONG,
    ERR_INVALID_VALUE,
    ERR_SIGNAL,
    ERR_TO_DROP,                                // drop unrecognized packets
    ERR_TO_CLOSE,                               // to close connection
    ERR_MISMATCH,                               // encoded string mismatches
    ERR_RANDOM_STRING,                          // fail to generate random str
    ERR_EXIST,                                  // entry exists already 989
    ERR_ATTRIBUTE,                              // missing required attributes
    ERR_INVALID_CMD,                            // invalid request command
    ERR_INPUT_LEN,                              // invalid packet length
    ERR_MAC_MISMATCH,                           // mac is not the right one
    ERR_XML_DOC,                                // xml document format error
    ERR_MMC_TAG_EMPTY,                          // tag without value
    ERR_NO_SITE,                                // does not belong to any site
    ERR_AUTH_FAIL,                              // authentication failed
    ERR_INVALID_PORT,                           // invalid tcp/udp port
    ERR_NO_FILE,                                // file does not exist 979
    ERR_INVALID_FORMAT,                         // invalid data format
    ERR_SYSTEM,                                 // system error
    ERR_OVERFLOW,                               // buffer is not big enough
    ERR_MMC_SET,                                // store data to memcache failed
    ERR_MMC_KEY,                                // invalid key
    ERR_MMC_TAG,                                // invalid tag
    ERR_DB_QUERY,                               // query database failed
    ERR_NO_CONNECTION,                          // connection to server failed
    ERR_RANGE,                                  // data is out of range
    ERR_NOT_FOUND,                              // data is not found 969
    ERR_NO_MEMORY,                              // out of memory
    ERR_NULL_POINTER,                           // pointer is null
    ERR_GET_DEVICE_INFO,                        // getting device information error
    ERR_API_SERVER,                             // http response error 965
    ERR_CONNECT_SERVER = -11,
    ERR_NO_INTERNET,
    ERR_INVALID_CREDENTIAL = -3,
    ERR_BUFFER_LEN,
    ERR_GENERIC = -1,                           // generic error

/* OAuth related error*/
    OAUTH_ERR_INVALID_REQ = 1012001,            // Missing OAuth parameters
    OAUTH_ERR_INVALID_CLI = 1010002,            // Invalid client id or secret
    OAUTH_ERR_INVALID_USER = 1012005,           // Invalid user email
    OAUTH_ERR_INVALID_PASS,                     // Invalid password
    OAUTH_ERR_INVALID_GRANT,                    // Wrong grant type {grant_type}
    OAUTH_ERR_QID_NOT_VERIFIED,                 // User email not yet verified
    OAUTH_ERR_SERVER = 1019999,                 // Authorization Failed
/* end of generic error */

}ea_errno;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

const char* get_strerror(int e);

#ifdef __cplusplus
    }		/* extern "C" */
#endif	/* __cplusplus */

#endif /* ERROR_CODE_H_ */
