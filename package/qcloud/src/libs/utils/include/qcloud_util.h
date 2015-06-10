/*!
 * @file: qcloud_util.h
 *
 *  Created on: Feb 22, 2013
 *      Author: ch
 */

#ifndef QCLOUD_UTIL_H_
#define QCLOUD_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define QCLOUD_HOST_LEN                 (256)
#define SERVICE_NAME_LEN                (32)

//@{
/**@name QCloud service list*/
//! Web service of qcloud
#define QCLOUD_SERVC_WWW_QCLOUD_STR     "www.qcloud"
//! Web service of QID
#define QCLOUD_SERVC_WWW_QID_STR        "www"
//! Web service of QNAP start
#define QCLODU_SERVC_WWW_START_STR      "start"
//! API service of QID
#define QCLOUD_SERVC_API_QID_STR        "qid"
//! API service of mcn
#define QCLOUD_SERVC_API_MCN_STR        "mcn"
//! API service of QCloud core services (v2)
#define QCLOUD_SERVC_API_QCLOUD_STR     "core"
//! API service of qpkg
#define QCLOUD_SERVC_API_QPKG_STR       "qpkg"
//! API service of usher
#define QCLOUD_SERVC_API_USHER_STR      "usher"
//! API service of ddns
#define QCLOUD_SERVC_API_DDNS_STR       "ddns"
//! API service Quick start service
#define QCLOUD_SERVC_API_QSTART_STR     "qsapi"

#define QCLOUD_SERVC_API_QSOCIAL_STR    "qsocial"

#define QCLOUD_SERVC_API_DEVICE_STR     "device"
//@}

typedef enum _qcloud_servc_e{
  QCLOUD_SERVC_START = 0,
  QCLOUD_SERVC_WWW_QCLOUD = QCLOUD_SERVC_START,
  QCLODU_SERVC_WWW_QID,
  QCLODU_SERVC_WWW_START,
  QCLODU_SERVC_API_QID,
  QCLODU_SERVC_API_MCN,
  QCLODU_SERVC_CORE, //core.api.myqnapcloud.com
  QCLODU_SERVC_QPKG,
  QCLODU_SERVC_USHER,
  QCLOUD_SERVC_DDNS,
  QCLOUD_SERVC_QSTART,
  QCLOUD_SERVC_QSOCIAL,
  QCLOUD_SERVC_DEVICE,
  QCLOUD_SERVC_END
}QCLOUD_SERVC_E;

/*!
 * \brief To get the server host according environment setting.
 * @param[in] is_production
 * @param[in] service_name given service name, like qpkg, or usher
 * @param[out] server_host character buffer for returned server host.
 *
    \li If "/var/c125b9c78c3ea79c8f49a570cc0c1d7d" is existed, then returns developing server host. (echo "dev--qnapcloud" | md5sum)
    \li If "/var/4ea77977c6a9d9ad07de14a9f00d62fb" is existed, then returns alpha server host, like qpkg.api.alpha-myqnapcloud.com. (echo "alpha--qnapcloud" | md5sum)
    \li If "/var/ec84ec12cbf73cb2c717f9585acbdb40" is existed, then returns production server host, like qpkg.api.myqnapcloud.com. (echo "qnapcloud" | md5sum)
    \li (For daily build) If there are no these files, and -DPRODUCTION is not given at compiling time, then returns alpha server host, otherwise returns the mapped server host.
    \li (For production build) If -DPRODUCTION is given at compiling time, then only returns production server host, like qpkg.api.myqnapcloud.com.
 * @return
 *  \li \c   0 for getting server host successfully.
 *  \li \cc -1 for invalid input parameter.
 *  \li \cc -2 for generic error.
 */
int qcloud_util_get_host(int is_production, char* service_name, char* server_host);

#ifdef __cplusplus
}
#endif

#endif /* QCLOUD_UTIL_H_ */
