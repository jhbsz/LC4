/*
 * qid_status_code.h
 *
 *  Created on: Oct 26, 2012
 *      Author: ch
 */

#ifndef QID_STATUS_CODE_H_
#define QID_STATUS_CODE_H_

#define MAX_STATUS_CODE_LEN           9
//!< Action error
#define QID_SERVC_ACTION_ERROR        10
//!< xxx is required
#define QID_SERVC_RES_REQUIRED        11
//!< xxx is too long
#define QID_SERVC_PARAM_TO_LONG       12
//<! xxx isn't valid
#define QID_SERVC_PARAM_INVALID       13
//<! xxx error
#define QID_SERVC_ERROR               14
//<! xxx expired
#define QID_SERVC_EXPIRED             15
//<! xxx isn't found
#define QID_SERVC_NOT_FOUND           16
//<! xxx had already be used
#define QID_SERVC_IS_USED             17
//<! Your call could not be completed as dialed.
#define QID_SERVC_AUTH_NOT            32
//<! Corresponds with an HTTP 404 - the specified resource was not found.
#define QID_SERVC_PAGE_NOT_FOUND      34
//<! The request limit for this resource has been reached for the current rate limit window.
#define QID_SERVC_RATE_LIMIT_EXCEEDED 88
//<! The access token used in the request is incorrect or has expired. Used in API v1.1
#define QID_SERVC_INVALID_TOKEN       89
//<! Corresponds with an HTTP 503 - Service is temporarily over capacity.
#define QID_SERVC_OVER_CAPACITY       130
//<! Corresponds with an HTTP 500 - An unknown internal error occurred.
#define QID_SERVC_INTERNAL_ERROR      131
//<! Corresponds with a HTTP 401 - it means that your oauth_timestamp is either ahead or behind our acceptable range
#define QID_SERVC_CANNOT_AUTH         135
//<! Bad authentication data Typically sent with 1.1 responses with HTTP code 400. The method requires authentication but it was not presented or was wholly invalid.
#define QID_SERVC_AUTH_DATA           215

//<! request is successfully made.
#define QID_SERCV_HTTP_OK             200

//<! request is made. but error occurs.
#define QID_SERCV_HTTP_ERROR             400

#endif /* QID_STATUS_CODE_H_ */
