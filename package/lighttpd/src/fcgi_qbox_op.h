#ifndef __QBOX_OP_H__
#define __QBOX_OP_H__
#include "fcgi_nas_cgiutil.h"
#include "fcgi_common.h"
int op_qbox_get_sid(INPUT *input);
int op_get_domain_ip_list(DOMAIN_IP_INFO *aDomain_ip_info);
int op_qbox_get_qbox_info(INPUT *input);
int op_get_tree(INPUT *input);
int op_get_file_list(INPUT *input);
void _printJsonString(int isfirst, char *label, char *string);
void _printJsonInt(int isfirst, char *label, int value);
#endif
