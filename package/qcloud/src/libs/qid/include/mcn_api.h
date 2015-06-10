#ifndef MCN_API_H_
#define MCN_API_H_

#include "qcloud.h"
#define MCN_VERSION "1.1"

typedef enum mcn_service_status_e{
  MCN_SERVC_DISABLE,                      //! This service is not enabled.
  MCN_SERVC_ENABLE,                       //! This service has been enabled.
  MCN_SERVC_INIT                          //! This service is doing initiation.
}MCN_SERVICES_STATUS_E;

typedef struct _mcn_ddns_pubilsh_st{
  //! service name, like FTP, WEBAdmin, WEBServer, etc.
  char name[QCLOUD_PUBLISHED_SERVC_NAME_LEN+1];
  //! detail description for this service, like Secured.
  char sub_name[QCLOUD_PUBLISHED_SERVC_NAME_LEN+1];
  //! login service path
  char login_path[MAX_QID_HOST_LEN+1];
  //! is this service is published; 1 for TRUE; 0 for FALSE
  int  is_published;
  //! is this service private
  int  is_private;
}mcn_ddns_publish_st;

typedef struct _mcn_service_port_st{
  //! service name, like FTP, WEBAdmin, WEBServer, etc.
  char name[QCLOUD_PUBLISHED_SERVC_NAME_LEN+1];
  //! detail description for this service, like Secured.
  char sub_name[QCLOUD_PUBLISHED_SERVC_NAME_LEN+1];
  //! protocol of port mapping. {TCP, UDP}
  char proto[POROT_MAPPING_PROTO_NAME_LEN+1];
  //! internal service port#
  unsigned short intnl_port;
  //! external service port#
  unsigned short extnl_port;

}mcn_service_port;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Update device information to cloud.
 *
 *  @return
 *            \li \c QID_ERR_NO_ERROR Successfully getting device status.
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int mycloudNAS_device_update_info(struct qcloud_st *ctx, time_t *report_period);

/*!
 * \brief Update all published service setting to mcnapi server
 *
 *  @param ctx status data for QCLOUD API
 *  @param servc_list Published service list to be sent.
 *  @param num_servc Number of entries in the service list.
 *
 *  @return
 *            \li \c\b QID_ERR_NO_ERROR           Successfully update service port list to mcnapi server.
 *            \li \c\b ERR_NULL_POINTER           Invalid input parameter.
 *            \li \c\b QID_ERR_NO_INTERNET        Connect server failed.
 *            \li \c\b QID_ERR_WONT_EXECUTED      This request will not be executed, due to device not checked-in mycloudNAS service.
 *            \li \c\b QID_ERR_INVALID_CREDENTIAL Device token failed, or in checked-out status;
 *            \li \c\b Otherwise please refer ERROR Code definitions.
 */
int mycloudNAS_update_published_service(struct qcloud_st *ctx,
                                        mcn_ddns_publish_st *servc_list, int num_servc);
/*!
 * \brief Update all device service port information to mcnapi server
 *
 *  @param ctx status data for QCLOUD API
 *  @param port_list Service port mapping list to be sent
 *  @param num_servc Number of entries in the mapping list
 *
 *  @return
 *            \li \c\b QID_ERR_NO_ERROR           Successfully update service port list to mcnapi server.
 *            \li \c\b ERR_NULL_POINTER           Invalid input parameter.
 *            \li \c\b QID_ERR_NO_INTERNET        Connect server failed.
 *            \li \c\b QID_ERR_WONT_EXECUTED      This request will not be executed, due to device not checked-in mycloudNAS service.
 *            \li \c\b QID_ERR_INVALID_CREDENTIAL Device token failed, or in checked-out status;
 *            \li \c\b Otherwise please refer ERROR Code definitions.
 */
int mycloudNAS_update_service_port(struct qcloud_st *ctx,
                                   mcn_service_port *port_list, int num_servc);


/*!
 * \brief Turn on/ off of DDNS at mycloudnas service
 *
 *  @param ctx status data for QCLOUD API
 *  @param activiaction 0 for off; 1 for on.
 *
 *  @return
 *            \li \c QID_ERR_NO_ERROR Service status is changed successfully.
 *            \li \c ERR_NULL_POINTER Invalid input parameter.
 *            \li \c QID_ERR_NO_INTERNET  Connect server failed.
 *            \li \c QID_ERR_INVALID_CREDENTIAL Device token failed, or in checked-out status;
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int mycloudNAS_set_ddns_status(struct qcloud_st *ctx, int activiaction);

/*!
 * \brief Get DDNS service status from NAS.
 *
 *  @param ctx status data for QCLOUD API
 *
 *  @return \li \c MCN_SERVC_DISABLE, if this service is turned off.
 *          \li \c MCN_SERVC_ENABLE, if this service is turned on.
 *          \li \c MCN_SERVC_INIT, this service is doing initiation.
 */
int mycloudNAS_get_ddns_status(struct qcloud_st *ctx);

int mycloudNAS_device_get_info(struct qcloud_st *ctx);
#ifdef __cplusplus
}
#endif

#endif /* MCN_API_H_ */
