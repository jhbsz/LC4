/*
 *
 * @file: qcloud_tool.h
 *  Created on: Jan 7, 2013
 */

#ifndef QCLOUD_UTIL_H_
#define QCLOUD_UTIL_H_

#define QCLOUD_HOST_LEN     (256)

/*!
 * \brief To get the server host according environment setting.
 * @param service_name given service name, like qpkg, or usher
 * @param server_host character buffer for returned server host, like qpkg.api.myqnapcloud.com
 * @return
 *  \li \c   0 for getting server host successfully.
 *  \li \cc -1 for invalid input parameter.
 *  \li \cc -2 for generic error.
 */
int qcloud_tool_get_host(char * server_name, char *server_host);

#endif /* QCLOUD_TOOL_H_ */
