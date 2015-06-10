

/*
 * qcloud_gen_apirequest.h
 *
 *      Author: Kevin Chien
 */

#ifndef QCLOUD_GEN_APIREQUEST_H_
#define QCLOUD_GEN_APIREQUEST_H_

#include "qcloud.h"
#include "comm_hdr.h"

#define JSON_STR_LOCAL_SERVICE              "local_services"
#define JSON_STR_LOCAL_USERS                "local_users"
#define JSON_STR_DEVICE_INFO                "info"
#define JSON_STR_PUBLISHED_ACCESS_CODE      "access_code"
#define JSON_STR_IPV4                       "ipv4"
#define JSON_STR_IPV6                       "ipv6"
#define JSON_STR_WAN_IPS                    "wan_ips"
#define JSON_STR_LAN_IPS                    "lan_ips"
#define JSON_STR_LAN_GATEWAY                "lan_gateway"

#define JSON_STR_PACKAGE_ID                 "package_id"
#define JSON_STR_PACKAGE_AUTHOR             "author"
#define JSON_STR_PACKAGE_CATEGORY           "category"
#define JSON_STR_PACKAGE_CURR_VERSION       "current_version"
#define JSON_STR_PACKAGE_NAME               "name"
#define JSON_STR_PACKAGE_DESC               "description"
#define JSON_STR_PACKAGE_LANG_EN_US         "en_US"
#define JSON_STR_PACKAGE_CNT_SUM            "counter_sum"
#define JSON_STR_PACKAGE_CNT_DOWNLOADED     "downloaded"
#define JSON_STR_PACKAGE_CNT_UPDATED        "updated"
#define JSON_STR_PACKAGE_CNT_UNINSTALLED    "uninstalled"
#define JSON_STR_PACKAGE_CNT_INSTALLED      "installed"


#ifdef __cplusplus
extern "C" {
#endif

void generate_oauth_request(char* grant_type, char* client_id,
                            char* client_secret, char* qid,
                            char* passwd, char *msg_buff, size_t buff_size );

int generate_device_info_root(struct qcloud_st *ctx, char *device_name,
                               char *msg_buff, size_t buff_size);

int json_parse(char *resp, key_pair_st *entry_list, size_t num_list);

int generate_device_local_user_str(char* ret,
                                      size_t ret_size,
                                      char** users,
                                      int user_count);

int generate_device_service_port_str(char* ret,
                                        size_t ret_size,
                                        mcn_service_port *port_list,
                                        int num_servc);

int generate_device_local_service_str(char* ret,
                                         size_t ret_size,
                                         mcn_ddns_publish_st *servc_list,
                                         int num_servc);

int generate_ddns_service_str(struct qcloud_st *ctx, char* ret, size_t size,
                               int need_device_id);

int generate_port_test_str(struct qcloud_st *ctx, char* ret, size_t size,
                               char* url);

int parse_external_ip(char* resp, char* external_ip, size_t ip_len);

#ifdef __cplusplus
}
#endif

#endif
