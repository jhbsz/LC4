#ifndef QID_API_H_
#define QID_API_H_

#include "qcloud.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Check the given device name is OKAY for check-in process.
 *
 * @param     ctx status data for QNAP ID API
 * @param[in] device_name Input device to check.
 *
 * @return \li \c QCLOUD_NAME_FREE This device is okay to use.
 *         \li \c QCLOUD_NAME_BEEN_USED This device name is not available.
 *         \li \c QCLOUD_NAME_INVALID_FORMAT Invalid device name format.
 *         \li \c < 0, for the error occurred in request.
 */
int qnap_id_is_device_name_available(struct qcloud_st *ctx, char *device_name);

/*!
 * \brief Update device name to QID server
 *
 *  @param ctx status data for QNAP ID API
 *  @return
 *            \li \c QID_ERR_DEVICE_NAME_ALREADY_USED The device name has been already in used.
 *            \li \c QID_ERR_QID_ALREADY_USED The device name has been already in used.
 *            \li \c QID_ERR_NO_ERROR This device is in checked-in status at server,
 *            \li \c ERR_NULL_POINTER Invalid input parameter.
 *            \li \c QID_ERR_NO_INTERNET  Connect server failed.
 *            \li \c QID_ERR_INVALID_CREDENTIAL Device token failed, or in checked-out status;
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int qnap_id_update_device_name(struct qcloud_st *ctx, char *name);

/*!
 * \brief Sign in QID
 *  @param ctx status data for QNAP ID API
 *  @param qid  The targeted email to resend validation email.
 *  @param passwd  The password for this QID
 *
 *  @return
 *            \li \c QID_ERR_NO_ERROR for successfully register the given QID at server.
 *            \li \c QID_ERR_CONNECT_SERVER Connect QID server failed.
 *            \li \c QID_ERR_RES_NOT_FOUND Means this qid is not available at QID server.
 *            \li \c QID_ERR_INVALID_QID_PASSWD password incorrect
 *            \li \c QID_ERR_QID_REGISTER_FAILED Error happened for sign in a validated user.
 *            \li \c Otherwise please refer ERROR Code definitions.
 */
int qnap_id_signin (struct qcloud_st *ctx, char *qid, char *passwd);

/*!
 * \brief bind QID with device
 *  @param ctx status data for QNAP ID API
 *  @param device_name device name that the user wants to assign
 *
 *  @return
 *            \li \c QID_ERR_NO_ERROR for successfully binding the given QID at server.
 *            \li \c QID_ERR_CONNECT_SERVER Connect QID server failed.
 *            \li \c QID_ERR_DEVICE_NAME_ALREADY_USED This device name is used by some one already.
 *            \li \c Otherwise please refer ERROR Code definitions.
 *
 */
int qnap_id_bind_device (struct qcloud_st *ctx, char* device_name);

/*!
 * \brief unbind this device
 *  @param ctx status data for QNAP ID API
 *
 *  @return
 *            \li \c QID_ERR_NO_ERROR for successfully unbinding the given QID at server.
 *            \li \c QID_ERR_CONNECT_SERVER Connect QID server failed.
 *            \li \c Otherwise please refer ERROR Code definitions.
 *
 */
int qnap_id_unbind_device (struct qcloud_st *ctx);

#ifdef __cplusplus
}
#endif

#endif /* QID_API_H_ */
