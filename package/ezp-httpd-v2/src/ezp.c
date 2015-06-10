#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <httpd.h>
#include <errno.h>
 
#include <proto/ethernet.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/klog.h>
#include <sys/wait.h>

#include <cy_conf.h>

#include <fcntl.h>
#include <signal.h>
#include <sys/klog.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "common.h"
#include "lang.h"
#include "nvram_ezpacket.h"
#include "ezp-lib.h"
#include "ezpcom-lib.h"
#include "ezp.h"
#include "shared.h"

#if defined (PLATFORM_AXA)
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#define COMMAND_SOCKET_PATH  "/tmp/lighttpd_command"
#define COMMAND_COPY_CANCEL "cancel"
#define RET_COPY_SUCCESS "success"

int copy_success = 0;
#endif
#if defined(ENABLE_QCLOUD_ON_HTTPD)
#include "qcloud.h"
#include "qid_api.h"
#include "mcn_api.h"
#endif
#include <bcmnvram.h>

#define IS_NUM(NC) ((NC >= '0') && (NC <= '9'))
#define IS_ASCII(NC) (((NC >= 'a') && (NC <= 'z')) || ((NC >= 'A') && (NC <= 'Z')))
#define IS_ALLOWED_CHAR(NC) ((NC == '-') || (NC == '_') || (NC == '.'))
#define VALID_DOMAIN_NAME_CHAR(NC) (IS_NUM(NC) || IS_ASCII(NC) || IS_ALLOWED_CHAR(NC))
#define VALID_ENG_CHAR_AND_NUM(NC) (IS_NUM(NC) || IS_ASCII(NC) || IS_ALLOWED_CHAR(NC))

enum {
    OP_MODE=0,
};

static struct variable opmode_variables[] = {
    {longname: "Operation Mode", argv:ARGV("gateway", "bridge"), nullok: FALSE},
};

int gozila_action = 0;
int reboot_action = 0;
int error_value = 0;
int config_validate_msg = 0;
char post_buf[POST_BUF_SIZE];

extern int access_permission;
extern int post;
extern int auth_state;
#ifdef AXIMDDNS
extern char aximddns_message[];
#endif
extern int conn_fd;
int
get_postact_restart(void)
{
    return reboot_action;
}

static int
ui_parser(char *src)
{
    char tmp[TMP_LEN] = {0};
    int i, tmp_count=0;
    for(i=0;i<strlen(src);i++) {
        if((src[i]=='\'') || (src[i]=='\"') || (src[i]=='\\')) {
            tmp[tmp_count] = '\\';
            tmp_count++;
        }
        tmp[tmp_count] = src[i];
        tmp_count++;
    }
    strcpy(src, tmp);
    return 0;
}

static char *
survey_parser(char *src)
{
    char tmp[TMP_LEN] = {0};
    int i, tmp_count=0;
    for(i=0;i<strlen(src);i++) {
        if((src[i]=='\\') || (src[i]=='\"')) {
            tmp[tmp_count] = '\\';
            tmp_count++;
        }
        tmp[tmp_count] = src[i];
        tmp_count++;
    }
    strcpy(src, tmp);
    return src;
}

/*
 * Misc functions!
 */

/* Valid family. When returning w/ FALSE, websDebugWrite() must be invoked
 * before exit. Thus, error_value could be set to 1.
 * For NVRAM, ^ and | are ommitted. */
int
valid_word(webs_t wp, char *value, struct variable *v)
{
    char *longname;
#ifdef PLATFORM_AXM
    char *cptr = value;
#endif

    if (!value || !(*value))
        return TRUE;
    if (!v || !(v->longname))
        longname = "Input String";
    else
        longname = v->longname;
#ifdef PLATFORM_AXM
    for( ; *cptr ; cptr++) {
        if(!VALID_ENG_CHAR_AND_NUM(*cptr)) {
            return FALSE;
        }
    }
#else
    if (strchr(value, '^') || strchr(value, '|') || 
        strchr(value, '\'') || strchr(value, '"') ||
        strchr(value, ';') || strchr(value, '$') ||
        strchr(value, '`') || strchr(value, '&') ||
        strchr(value, '>') || strchr(value, '<')) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: "
                       "^, |, ', $, ;, `, &, <, > and \" are not allowed<br>", 
                       longname, value);
        return FALSE;
    }
#endif

    return TRUE;
}

int valid_nv_word(webs_t wp, char *value, struct variable *v)
{
    char *longname;

    if (!value || !(*value))
        return TRUE;
    if (!v || !(v->longname))
        longname = "Input String";
    else
        longname = v->longname;
    if (strchr(value, '|') || strchr(value, '\'') || strchr(value, '"') ||
        strchr(value, ';') || strchr(value, '$') ||
        strchr(value, '(') || strchr(value, ')') ||
        strchr(value, '`') || strchr(value, '&') ||
        strchr(value, '>') || strchr(value, '<')) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: "
                       "|, ', (, ), $, ;, `, &, <, > and \" are not allowed<br>", 
                       longname, value);
        return FALSE;
    }

    return TRUE;
}

int
valid_domain(webs_t wp, char *value, struct variable *v)
{
    int slen;
    char *p;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;
    if ( v->argv && *(v->argv) && ((slen = atoi(*(v->argv))) != 0) && (strlen(value) > slen)){
        /* value length too long */
        return FALSE;
    }
    for (p = value; *p; p++){
        if(!VALID_DOMAIN_NAME_CHAR(*p))
            return FALSE;
    }
    return TRUE;
}

int
valid_ipaddr(webs_t wp, char *value, struct variable *v)
{ 
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;

    if (!is_legal_ipaddr(value)) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: not an IP address<br>",
                       v->longname, value);
        return FALSE;
    }

    if ((*(value+strlen(value)-2)=='.' && *(value+strlen(value)-1)=='0') || 
            (*(value+strlen(value)-4)=='.' && *(value+strlen(value)-3)=='2' &&
             *(value+strlen(value)-2)=='5' && *(value+strlen(value)-1)=='5')) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: not an IP address<br>",
                v->longname, value);
        return FALSE;
    }
    return TRUE;
}

int
valid_netmask(webs_t wp, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;

    if (!is_legal_netmask(value)) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: not a legal netmask<br>",
                       v->longname, value);
        return FALSE;
    }

    /* TODO: 255.255.255.0, 255.255.255.127, 255.255.255.63, 255.255.255.X */

    return TRUE;
}

int
valid_subnet(webs_t wp, char *value, struct variable *v)
{ 
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;

    if (!is_legal_ipaddr(value)) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: not a subnet ip<br>",
                       v->longname, value);
        return FALSE;
    }

    if ( (*(value+strlen(value)-4)=='.' && *(value+strlen(value)-3)=='2' &&
             *(value+strlen(value)-2)=='5' && *(value+strlen(value)-1)=='5')) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: not a subnet ip<br>",
                v->longname, value);
        return FALSE;
    }
    return TRUE;
}

int
valid_choice(webs_t wp, char *value, struct variable *v)
{
    char **choice;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;

    for (choice = v->argv; *choice; choice++) {
        if (!strcmp(value, *choice))
            return TRUE;
    }

    websDebugWrite(wp, "Invalid <b>%s</b> %s: not one of ", v->longname,
                   value);
    for (choice = v->argv; *choice; choice++)
        websDebugWrite(wp, "%s%s", choice == v->argv ? "" : "/", *choice);
    websDebugWrite(wp, "<br>");
    return FALSE;
}
int
valid_length(webs_t wp, char *value, struct variable *v)
{
    int length;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE) {
        return FALSE;
    }

    length = atoi(v->argv[0]);
    if (strlen(value) != length) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: not %d-digits length", 
                    v->longname, value, length);
    }
        
    return TRUE;
}

int
valid_number(webs_t wp, char *value, struct variable *v)
{
    int i;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE) {
        return FALSE;
    }

    for (i = 0; value[i] != '\0'; i++) {
        if (value[i] > '9' || value[i] < '0') {
            websDebugWrite(wp, "Invalid <b>%s</b> %s: not a number", 
                    v->longname, value);
            return FALSE;
        }
    }

    return TRUE;
}

int
valid_range(webs_t wp, char *value, struct variable *v)
{
    int n, start, end;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_number(wp, value, v) == FALSE) {
        return FALSE;
    }

    n = atoi(value);
    start = atoi(v->argv[0]);
    end = atoi(v->argv[1]);

    if (!is_digit_str(value) || n < start || n > end) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: out of range %d-%d<br>",
                       v->longname, value, start, end);
        return FALSE;
    }

    return TRUE;
}

int
valid_in_a_range(webs_t wp, int value, int min, int max, struct variable *v)
{
    if (value < min || value > max) {
        websDebugWrite(wp, 
                "Invalid <b>%s</b> %d: not in a range (%d - %d)<br>",
                v->longname, value, min, max);
        return FALSE;
    }

    return TRUE;
}

int
valid_length_range(webs_t wp, char *value, struct variable *v)
{
    int length, min, max;

    min = atoi(v->argv[0]);
    max = atoi(v->argv[1]);
    length = strlen(value);
   
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }
    if (valid_word(wp, value, v) == FALSE) {
        return FALSE;
    }

    if (length < min || length > max) {
       websDebugWrite(wp, "Invalid <b>%s</b> %d: "
                          "length should between (%d - %d)<br>",
                      v->longname, length, min, max);
       return FALSE;
    }

    return TRUE;
}

int
valid_if(webs_t wp, char *value, struct variable *v)
{
    char *rule_num;
    char tmp[TMP_LEN];
    int if_str_len, diff, i = 0;
    int res = FALSE;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;
    /**
     * The input 'value' would be compose as prefix + num. Such as lan1,
     * wan2.
     */

    snprintf(tmp, TMP_LEN, "%s_num", (char *)v->argv[0]);
    rule_num = nvram_safe_get(tmp);
    while (v->argv[i]) {
        /* Validate the if-prefix: wan, lan, pppoe... */
        if_str_len = strlen(v->argv[i]);
        if (!strncmp(value, v->argv[i], if_str_len)) {
            /* Validate the if-num. */
            value += if_str_len;
            diff = (int)(*value - '0');

            /* For UI, the index is 1-based. */
            if (diff < 0 || diff >= atoi(rule_num)) {
                websDebugWrite(wp, "Invalid <b>%s</b> %s: "
                        "out of interface range 0-%d<br>",
                        v->longname, value, atoi(rule_num));
                res = FALSE;
            }
            else {
                res = TRUE;
            }
            break;
        }
        i++;
    }
    if (!v->argv[i]) {
        websDebugWrite(wp, "Invalid <b>%s</b> %s: not an interface name<br>",
                v->longname, value);
    }
    return res;
}

int
valid_name(webs_t wp, char *value, struct variable *v)
{
    int max;
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE) {
        return FALSE;
    }

    max = atoi(v->argv[0]);

    if (!is_ascii_str(value)) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: NULL or have illegal "
                       "characters<br>",
                       v->longname, value);
        return FALSE;
    }
    if (strlen(value) > max) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: out of range 1-%d characters<br>",
                       v->longname, value, max);
        return FALSE;
    }

    return TRUE;
}
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int
qid_update_device_info()
{
    int ret = QID_ERR_NO_ERROR;

    // init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));
    
    //report period (not used)
    time_t report_period = 0;

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //reset hash to force to issue api request
    //reset_qcloud_request_hash();

    //update dev info
    ret = mycloudNAS_device_update_info(ctx, &report_period);

    switch( ret )
    {
        case MCN_ERR_DEVICE_ID_NOT_FOUND:
            // do sign out if the device is already unbind
            reset_qcloud_ctx_conf(ctx);
            break;

        default:
            break;
    }
    qcloud_cleanup_ctx(ctx);
    return ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_get_device_info(char *qid_account, char *qid_status, char *device_id, char *device_name)
{
    int ret = QID_ERR_NO_ERROR;
    char buf[TMP_LEN];
    memset(buf, 0, sizeof(buf));

    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    //init qnap dev infor
    qnap_dev_info dev_info;
    memset(&dev_info, 0, sizeof(qnap_dev_info));

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //get dev info
    ret = qcloud_get_device_info(ctx, &dev_info);

    //if ok, print device info
    if (ret == QID_ERR_NO_ERROR) {
        //print qid status
        if( dev_info.qid_status[0] == '3' ){
            sprintf(buf, "sign-in");
            strncpy(qid_status, buf, strlen(buf));
        } else {
            sprintf(buf, "sign-out");
            strncpy(qid_status, buf, strlen(buf));
        }

        if (strcmp(dev_info.qid_account, "" )) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s", dev_info.qid_account);
        strncpy(qid_account, buf, strlen(buf));
       }

        //print device id
        if (strcmp(dev_info.device_id, "")) {
        sprintf(buf, "%s", dev_info.device_id);
        strncpy(device_id, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
        }
       if (strcmp(dev_info.device_name, "")) {
        sprintf(buf, "%s", dev_info.device_name);
        strncpy(device_name, buf, strlen(buf));
       }
    }

    //cleanup ctx
    qcloud_cleanup_ctx(ctx);
    return ret;

}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_signin(char* qid_id, char* qid_pw)
{
    int ret = QID_ERR_NO_ERROR;
    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //sign in
    ret = qnap_id_signin(ctx, qid_id, qid_pw);

    //cleanup ctx
     qcloud_cleanup_ctx(ctx);
     return ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_available_dev_name(char* device_name)
{
    int function_ret = 0;

    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //check available
    function_ret = qnap_id_is_device_name_available(ctx, device_name);
    
    // cleanup ctx
    qcloud_cleanup_ctx(ctx);

    return function_ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_bind_dev_name(char* device_name)
{
    int ret = QID_ERR_NO_ERROR;
    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //bind
    ret = qnap_id_bind_device(ctx, device_name);

    // cleanup ctx
    qcloud_cleanup_ctx(ctx);

    return ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_publish_service()
{
    int ret = QID_ERR_NO_ERROR;

    // init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //prepare service list
    mcn_ddns_publish_st ddns_service_tlb[2] = {
        {
            "ManagementServer",
            "",
            "/",
            1,
            0,
        },
        {
            "FileServer",
            "",
            "/",
            1,
            0,
        }
    };

    //update publis service
    ret = mycloudNAS_update_published_service(
            ctx,
            ddns_service_tlb,
            2);

    //cleanup ctx
    qcloud_cleanup_ctx(ctx);

    return ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_update_service_port(uint16_t management_external_port, uint16_t file_external_port) {
    int ret = QID_ERR_NO_ERROR;

    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //prepare service list
    mcn_service_port port_mapping_tlb[2] = {
        {
            "ManagementServer",
            "",
            "HTTP",
            80,
            management_external_port,
        },
        {
            "FileServer",
            "",
            "HTTP",
            8081,
            file_external_port,
        }
    };
    //update service port
    ret = mycloudNAS_update_service_port(
            ctx,
            port_mapping_tlb,
            2);

    //cleanup ctx
    qcloud_cleanup_ctx(ctx);

    return ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_update_service_port_by_upnpc(){

    int ret = QID_ERR_NO_ERROR;

#if LOCAL_TEST

    // this function is not supported in local test
    //printf("this function %s is not supported in local test", __FUNCTION__);

#else /* LOCAL_TEST */

    /* Setup upnp services */
    system("/usr/bin/setup_service_upnpc.sh");

    // var to store external port result from upnpc
    uint16_t external_ports[2] = {0};

    /* Get upnpc set result */
    int upnpc_num = atoi(nvram_safe_get("upnpc_setting_rule_num"));
    int upnpc_idx;
    unsigned short outport_num;
    char buf[10];
    for(upnpc_idx = 0; upnpc_idx < upnpc_num; upnpc_idx++) {
        ezplib_get_attr_val("upnpc_status_rule", upnpc_idx, "activated", buf, sizeof(buf), EZPLIB_USE_CLI);
        if(buf[0] == '1') {
            /* register success */
            ezplib_get_attr_val("upnpc_status_rule", upnpc_idx, "outport", buf, sizeof(buf), EZPLIB_USE_CLI);
            outport_num = atoi(buf);
            fprintf(stderr, "%s(%d) assigned %d item with outport %d\n", __func__, __LINE__, upnpc_idx, outport_num);

            // store the external port
            if( upnpc_idx < 2 )
                external_ports[upnpc_idx] = outport_num;
        }
    }

    // call update service port cloud api
    ret = qid_update_service_port(external_ports[0], external_ports[1]);


#endif /* LOCAL_TEST */

    return ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_fetch_web_page(const char* protocol, uint16_t external_port, const char* path) {
    int ret = QID_ERR_NO_ERROR;
//#if 0
    char buf_path[TMP_LEN];
    memset(buf_path, 0, sizeof(buf_path));

    sprintf(buf_path, "/%s", path);
    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //prepare buffer
    char* buffer = (char*)malloc(4096);
    memset(buffer, 0, 4096);

    //call fetch web page
    ret = qcloud_fetch_web_page(ctx,
            protocol, external_port, buf_path, 30,
            buffer, 4096);

    //output buffer  to file
    char tmp_buffer[TMP_LEN];
    sprintf(tmp_buffer, "echo \"%s\" > /tmp/.fetch_%s_log", buffer, path);
    system(tmp_buffer);

    //cleanup ctx
    qcloud_cleanup_ctx(ctx);

    //free buffer
    free(buffer);
//#endif
    return ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_unbind_dev_name()
{
    int ret = QID_ERR_NO_ERROR;

    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    ret = qnap_id_unbind_device(ctx);
    //cleanup ctx
    qcloud_cleanup_ctx(ctx);

    return ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int qid_btn_test_fetch_action()
{
    FILE  *f_deviceid, *f_deviceid_js;
    int qid_btn_test_fetch_ret = 0;
    int qid_btn_test_fetch_port_file_station_ret = 0;
    int qid_btn_test_fetch_port_web_mgmt_ret = 0;
    // int qid_get_upnpc_set_result_ret = 0;
    char *get_nvram_ssid_val;
    char buf[TMP_LEN], buf1[TMP_LEN], buf2[TMP_LEN];
    char buf_deviceid_file[TMP_LEN], buf_deviceid_js_file[TMP_LEN], check_file_station_result[TMP_LEN], check_web_mgmt_result[TMP_LEN];
    int tag_fetch_web_mgmt_result = 0;
    int tag_fetch_file_station_result = 0;

    uint16_t external_ports[2] = {0};
    
    FILE *check_fetch_file_station_ret, *check_fetch_web_mgmt_ret;
    int upnpc_idx;
    unsigned short outport_num;
    char buf_upnpc[10];
    char fetch_web_portocol[] = "http";
    char fetch_web_mgmt_path[] = "deviceid.js";
    char fetch_file_station_path[] = "deviceid";
    int call_qid_fetch_web_page_mgmt_ret;
    int call_qid_fetch_web_page_file_sta_ret;
    
    memset(buf, 0, sizeof(buf));
    memset(buf1, 0, sizeof(buf1));
    memset(buf2, 0, sizeof(buf2));
    memset(buf_deviceid_file, 0, sizeof(buf_deviceid_file));
    memset(buf_deviceid_js_file, 0, sizeof(buf_deviceid_js_file));

    /* created file for qcloud fetch web_mgmt and file_station portal */
    
    get_nvram_ssid_val = nvram_safe_get("wl0_ssid_rule");
    sprintf(buf, "nvram get wl0_ssid_rule > /tmp/mnt/deviceid");//for web_mgmt
    system(buf);
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "nvram get wl0_ssid_rule > /www/deviceid.js");//
    system(buf);
        
    /* checked fetch web_mgmt and file_station file content*/
    if ((f_deviceid_js = fopen("/www/deviceid.js", "r"))) {  //web_mgmt
        if (fgets(buf2,sizeof(buf2), f_deviceid_js) != NULL) {
            strncpy(buf_deviceid_js_file, buf2, strlen(buf2)-1);
            if (!strcmp(get_nvram_ssid_val, buf_deviceid_js_file)) {
            }
        }
        fclose(f_deviceid_js);
        qid_btn_test_fetch_port_web_mgmt_ret = 1;
    }

    if ((f_deviceid = fopen("/tmp/mnt/deviceid", "r"))) {   //file_station
        if (fgets(buf2,sizeof(buf2), f_deviceid) != NULL) {
            strncpy(buf_deviceid_file, buf2, strlen(buf2)-1);
            if (!strcmp(get_nvram_ssid_val, buf_deviceid_file)) {
            }
        }
        fclose(f_deviceid);
        qid_btn_test_fetch_port_file_station_ret = 1 ;
    }


    /* Get upnpc set result */
    int upnpc_num = atoi(nvram_safe_get("upnpc_setting_rule_num"));
    for(upnpc_idx = 0; upnpc_idx < upnpc_num; upnpc_idx++) {
        ezplib_get_attr_val("upnpc_status_rule", upnpc_idx, "activated", buf_upnpc, sizeof(buf_upnpc), EZPLIB_USE_CLI);
       
        if (buf_upnpc[0] == '0') {
            qid_update_service_port_by_upnpc();    
            ezplib_get_attr_val("upnpc_status_rule", upnpc_idx, "activated", buf_upnpc, sizeof(buf_upnpc), EZPLIB_USE_CLI);
        }

        if (buf_upnpc[0] == '1') {
            /* register success */
            ezplib_get_attr_val("upnpc_status_rule", upnpc_idx, "outport", buf_upnpc, sizeof(buf_upnpc), EZPLIB_USE_CLI);
            outport_num = atoi(buf_upnpc);

            // store the external port
            if( upnpc_idx < 2 )
                external_ports[upnpc_idx] = outport_num;
        }
    }
    
    /* call qid_fetch_web_page  for web_mgmt */
    if (qid_btn_test_fetch_port_web_mgmt_ret == 1) {
        call_qid_fetch_web_page_mgmt_ret = qid_fetch_web_page(fetch_web_portocol, external_ports[0], fetch_web_mgmt_path);
        if (call_qid_fetch_web_page_mgmt_ret == 0) {
            if ((check_fetch_web_mgmt_ret = fopen("tmp/.fetch_deviceid.js_log", "r"))){  
                if (fgets(buf1,sizeof(buf1), check_fetch_web_mgmt_ret) != NULL) {
                    strncpy(check_web_mgmt_result, buf1, strlen(buf1)-1);
                    if (!strcmp(buf_deviceid_js_file, check_web_mgmt_result)) { //fetch success replace to nvram 
                        tag_fetch_web_mgmt_result = 1;
                        ezplib_replace_attr("upnpc_status_rule" , 0, "fetch_result", "1");
                    }
                }
                fclose(check_fetch_file_station_ret);
            }
        } else { //fetch fail replace to nvram 
            ezplib_replace_attr("upnpc_status_rule" , 0, "fetch_result", "0");
        }
    }

    /* call qid_fetch_web_page for file_station */
    if (qid_btn_test_fetch_port_file_station_ret == 1) {
        call_qid_fetch_web_page_file_sta_ret = qid_fetch_web_page(fetch_web_portocol, external_ports[1], fetch_file_station_path); 
        if ( call_qid_fetch_web_page_file_sta_ret == 0 ) {
            if ((check_fetch_file_station_ret = fopen("/tmp/.fetch_deviceid_log", "r"))){  
                if (fgets(buf1,sizeof(buf1), check_fetch_file_station_ret) != NULL) {
                    strncpy(check_file_station_result, buf1, strlen(buf1)-1);
                    if (!strcmp(buf_deviceid_file, check_file_station_result)) {
                        tag_fetch_file_station_result = 1;
                        ezplib_replace_attr("upnpc_status_rule" , 1, "fetch_result", "1");
                    }
                }
                fclose(check_fetch_file_station_ret);
            }

        } else {
            ezplib_replace_attr("upnpc_status_rule" , 1, "fetch_result", "0");
        }

    }

    return qid_btn_test_fetch_ret;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int
valid_qcloud(webs_t wp, char *value, struct variable *v)
{
    int qid_signin_ret;
    char *qcloud_qid, *qcloud_pw, *wan0_ipaddr;
    qcloud_qid = websGetVar(wp, "qcloud_qid", "");
    qcloud_pw = websGetVar(wp, "qcloud_pw", "");
 
    /* check internet */
    wan0_ipaddr = nvram_safe_get("wan0_ipaddr");
    if (!strcmp(wan0_ipaddr, "")) {
        websDebugWrite(wp, "%s <br>", lang_translate("You have to choose a network."));
        return FALSE;
     } 
    
    qid_signin_ret = qid_signin(qcloud_qid, qcloud_pw);
    
    if (qid_signin_ret != 0) {
        websDebugWrite(wp, "%s <br> (%s : %d) <br>", lang_translate("Incorrect user name or password."), lang_translate("Error code"),qid_signin_ret);
        return FALSE;
    }
    return TRUE;
}
#endif
#if defined (ENABLE_QCLOUD_ON_HTTPD)
int
valid_qcloud_dev(webs_t wp, char *value, struct variable *v)
{
    int qid_available_dev_name_ret;
    char *qcloud_device_name;
    qcloud_device_name = websGetVar(wp, "qcloud_device_name", "");
    qid_available_dev_name_ret = qid_available_dev_name(qcloud_device_name);
    
    if (qid_available_dev_name_ret != QCLOUD_NAME_FREE) {
        websDebugWrite(wp, "%s <br> (%s : %d) <br>", lang_translate("The device name has been registered."), lang_translate("Error code"),qid_available_dev_name_ret);
        return FALSE;
    }

    return TRUE;
}
#endif
int
valid_nv_name(webs_t wp, char *value, struct variable *v)
{
    int max;
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_nv_word(wp, value, v) == FALSE) {
        return FALSE;
    }

    max = atoi(v->argv[0]);

    if (!is_ascii_str(value)) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: NULL or have illegal "
                       "characters<br>",
                       v->longname, value);
        return FALSE;
    }
    if (strlen(value) > max) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: out of range 1-%d characters<br>",
                       v->longname, value, max);
        return FALSE;
    }

    return TRUE;
}

int
valid_strict_name(webs_t wp, char *value, struct variable *v)
{
    int max;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE) {
        return FALSE;
    }

    max = atoi(v->argv[0]);
    if (strchr(value, ' ') || strchr(value, '*')) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: * and space are not "
                       "allowed<br>", 
                       v->longname, value);
        return FALSE;
    }

    if (!is_ascii_str(value)) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: NULL or have illegal "
                       "characters<br>",
                       v->longname, value);
        return FALSE;
    }

    if (is_digit_str(value)) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: can not use only number<br>",
                       v->longname, value);
        return FALSE;
    }

    if (strlen(value) > max) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: out of range 1-%d characters<br>",
                       v->longname, value, max);
        return FALSE;
    }

    return TRUE;
}

/* 
 * The HTML always show "asdfjkllkjfdsa"
 * so we must filter it.
 */
int
valid_passwd(webs_t wp, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE) {
        return FALSE;
    }

    if (!strcmp(value, TMP_PASSWD)) {
        return TRUE;
    }
    return valid_name(wp, value, v);
}

int
valid_nv_passwd(webs_t wp, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (!strcmp(value, TMP_PASSWD)) {
        return TRUE;
    }
    return valid_nv_name(wp, value, v);
}

int
valid_hwaddr(webs_t wp, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;

    /* Check for bad, multicast, broadcast, or null address */
    if (!is_legal_hwaddr(value)) {
        websDebugWrite(wp,
                       "Invalid <b>%s</b> %s: not a legal MAC address<br>",
                       v->longname, value);
        return FALSE;
    }

    return TRUE;
}

int
valid_port_range(webs_t wp, char *v1, char *v2, struct variable *v)
{
    int start, end;

    if ((!*v1 && *v2) || (*v1 && !*v2) ) {
        websDebugWrite(wp, "Invalid <b>%s</b>: "
                           "IP start and IP end are symbiosis.",
                       v->longname);
        return FALSE;
    }
   
    if (valid_word(wp, v1, v) == FALSE) {
        return FALSE;
    }

    if (valid_word(wp, v2, v) == FALSE) {
        return FALSE;
    }

    start = atoi(v1);
    end = atoi(v2);
    if (start > end) {
        websDebugWrite(wp, "Invalid <b>%s</b>: (%s-%s) range is not in right order.<br>",
                       v->longname, v1, v2);
        return FALSE;
    }
    return TRUE;
}

int
valid_ipaddr_range(webs_t wp, char *v1, char *v2, struct variable *v)
{
    int start, end;

    if ((!*v1 && *v2) || (*v1 && !*v2) ) {
        websDebugWrite(wp, "Invalid <b>%s</b>: "
                           "IP start and IP end are symbiosis.",
                       v->longname);
        return FALSE;
    }
   
    if (valid_word(wp, v1, v) == FALSE) {
        return FALSE;
    }

    if (valid_word(wp, v2, v) == FALSE) {
        return FALSE;
    }

    start = ntohl((int)inet_addr(v1));
    end = ntohl((int)inet_addr(v2));
    if (start > end) {
        websDebugWrite(wp, "Invalid <b>%s</b>: (%s-%s) range is not in right order.<br>",
                       v->longname, v1, v2);
        return FALSE;
    }
    return TRUE;
}

int
valid_lang(webs_t wp, char *value, struct variable *v)
{
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;
    if (lang_get_index(value) == -1)
        return FALSE;
    return TRUE;
}

int
valid_factory(webs_t wp, char *value, struct variable *v)
{
    char *val, *license_invalid;
#ifndef CONFIG_EZP_ARCH_RT305X
    FILE *file;
    char buf[SHORT_BUF_LEN];
    char hwaddr[64]; 
#endif

    license_invalid = nvram_safe_get("license_invalid");
    if (*license_invalid == '0') {
        /* Has been authenticated. */
        return FALSE;
    }
    
    val = websGetVar(wp, "machine_id", "");
#ifdef CONFIG_EZP_ARCH_RT305X
    if (!is_legal_hwaddr(val)) {
        return FALSE;
    }
#else
    if ((file = fopen("/proc/ezp_machine_id", "r"))) {
        fgets(hwaddr, sizeof(hwaddr), file);
        hwaddr[17] = '\0';
        fclose(file); 
    } else {
        ezplib_get_attr_val("wl0_ifname_rule", 0, "ifname", buf, sizeof(buf),
                EZPLIB_USE_CLI);
        show_hwaddr(buf, hwaddr, 64);
    }
    if (strcmp(val, hwaddr)) {
        return FALSE;
    }
#endif

    /* check prod_supercat */
    val = websGetVar(wp, "prod_supercat", "");
    if(strcmp(val, nvram_safe_get("prod_supercat"))) {
        return FALSE;
    }

    return TRUE;
}

int
valid_license(webs_t wp, char *value, struct variable *v)
{
    char *license_invalid;

    license_invalid = nvram_safe_get("license_invalid");
    if (*license_invalid == '0') {
        /* Has been authenticated. */
        return FALSE;
    }
    return TRUE;
}

int
valid_backdoor(webs_t wp, char *value, struct variable *v)
{
    char *key1, *key2;

    key1 = websGetVar(wp, "enable_ssh", "");
    key2 = nvram_safe_get("license_key");
    if (strncmp(key2, key1, strlen(key2))) {
        return FALSE;
    }
    return TRUE;
}

int
valid_op_mode(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *enable;
    snprintf(tmp, sizeof(tmp), "op_mode");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, &opmode_variables[OP_MODE]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

enum {
    USBLED=0,
};

static struct variable usbled_variables[] = {
    {longname: "USB Led Blink Behavior", argv:ARGV("0", "1"), nullok: FALSE},
};

int
valid_usbled(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *usbled;
    snprintf(tmp, sizeof(tmp), "usbled");
    usbled = websGetVar(wp, tmp, "");
    if (valid_choice(wp, usbled, &usbled_variables[USBLED]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

/* 
 * The function family of saving variables.
 * Return 1 if there is a changed; return 0 if none.
 */
int
save_generic(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;

    if (strcmp(value, nvram_safe_get(v->name))) {
        config_preaction(&map, v, s, "", "");
        nvram_set(v->name, value);
        config_postaction(map, s, "", "");
        return 1;
    }
    return 0;
}

int
save_passwd(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;
    /* No default passwd and different from the original passwd. */
    if (strcmp(value, TMP_PASSWD) &&
            strcmp(value, nvram_safe_get(v->name))) {
        config_preaction(&map, v, s, "", "");
        nvram_set(v->name, value);
        config_postaction(map, s, "", "");
        return 1;
    }
    return 0;
}

int
save_lang(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;
    if (strcmp(value, nvram_safe_get(v->name))) {
        config_preaction(&map, v, s, "", "");
        nvram_set(v->name, value);
        lang_construct_translator(LANG_MAP_FILE, value);
        config_postaction(map, s, "", "");
        return 1;
    }
    return 0;
}

int
save_factory(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int num, i;
    char *wan_hwaddr = NULL, *lan_hwaddr, *cur;
    char tmp_wan[TMP_LEN], tmp_lan[TMP_LEN];
    char *brand, *model, *license_key, *lang_default,
         *prod_cat, *prod_subcat, *lan_ipaddr, *port, *uid, *pw,
         *wl0_ssid_default, *wl0_region_default, *machine_id,
         *prod_supercat;
    char *http_rule_set = "http_rule_default";
    char *lan_rule_set = "lan_static_rule_default";
    char tmp[TMP_LEN], tmp_name[TMP_LEN], tmp_enable[TMP_LEN],
         tmp_rmgt[TMP_LEN], tmp_mask[TMP_LEN], tmp_gateway[TMP_LEN],
         tmp_dns[TMP_LEN], tmp_wins[TMP_LEN];
    int len;
#ifdef EZP_PROD_BRAND_PROX
    char *wlan_hwaddr;
    int mac[6];
    char buf[TMP_LEN];
#endif
    
    brand = websGetVar(wp, "brand", "");
    nvram_fset("brand", brand);

    model = websGetVar(wp, "model", "");
    nvram_fset("model", model);

    license_key = websGetVar(wp, "license_key", "");
    nvram_fset("license_key", license_key);

    lang_default = websGetVar(wp, "lang_default", "");
    nvram_fset("lang_default", lang_default);

    prod_supercat = websGetVar(wp, "prod_supercat", "");
    nvram_fset("prod_supercat", prod_supercat);

    prod_cat = websGetVar(wp, "prod_cat", "");
    nvram_fset("prod_cat", prod_cat);
    
    prod_subcat = websGetVar(wp, "prod_subcat", "");
    nvram_fset("prod_subcat", prod_subcat);

    wl0_ssid_default = websGetVar(wp, "wl0_ssid_default", "");
    nvram_fset("wl0_ssid_rule_default", wl0_ssid_default);

    wl0_region_default = websGetVar(wp, "wl0_region_default", "");
    nvram_fset("wl0_region_default", wl0_region_default);

    /* Only one attribute, hwaddr */
    num = atoi(websGetVar(wp, "wan_num", "1"));
    for (i = 0, cur = tmp_wan; i < num; i++) {
        /* 1-based */
        snprintf(tmp, TMP_LEN, "wan%d_hwaddr_default", i+1);
        wan_hwaddr = websGetVar(wp, tmp, "");
        cur +=
            snprintf(cur, TMP_LEN, "%s%s",
                    (tmp_wan == cur) ? "" : "|", wan_hwaddr);
    }
    nvram_fset("wan_hwaddr_rule_default", tmp_wan);

    /* Only one attribute, hwaddr */
    num = atoi(websGetVar(wp, "lan_num", "1"));
    for (i = 0, cur = tmp_lan; i < num; i++) {
        snprintf(tmp, TMP_LEN, "lan%d_hwaddr_default", i+1);
        lan_hwaddr = websGetVar(wp, tmp, "");
        cur +=
            snprintf(cur, TMP_LEN, "%s%s",
                    (tmp_lan == cur) ? "" : "|", lan_hwaddr);
    }
    nvram_fset("lan_hwaddr_rule_default", tmp_lan);
#ifdef EZP_PROD_BRAND_PROX
    wlan_hwaddr = websGetVar(wp, "wlan_hwaddr_default", "");
    sscanf(wlan_hwaddr,"%2X:%2X:%2X:%2X:%2X:%2X", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    snprintf(buf, sizeof(buf), "iwpriv ra0 e2p 4=%02X%02X", mac[1], mac[0]);
    system(buf);
    snprintf(buf, sizeof(buf), "iwpriv ra0 e2p 6=%02X%02X", mac[3], mac[2]);
    system(buf);
    snprintf(buf, sizeof(buf), "iwpriv ra0 e2p 8=%02X%02X", mac[5], mac[4]);
    system(buf);
    if(wan_hwaddr) {
        sscanf(wan_hwaddr,"%2X:%2X:%2X:%2X:%2X:%2X", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
        snprintf(buf, sizeof(buf), "iwpriv ra0 e2p 28=%02X%02X", mac[1], mac[0]);
        system(buf);
        snprintf(buf, sizeof(buf), "iwpriv ra0 e2p 2a=%02X%02X", mac[3], mac[2]);
        system(buf);
        snprintf(buf, sizeof(buf), "iwpriv ra0 e2p 2c=%02X%02X", mac[5], mac[4]);
        system(buf);
    }
#endif

    /* Replace lan_static_rule_default with nvram_set(). */
    lan_ipaddr = websGetVar(wp, "lan_ipaddr_default", "");
    ezplib_get_attr_val(lan_rule_set, 0, "mask", tmp_mask, TMP_LEN,
            EZPLIB_USE_CLI);
    ezplib_get_attr_val(lan_rule_set, 0, "gateway", tmp_gateway, TMP_LEN,
            EZPLIB_USE_CLI);
    ezplib_get_attr_val(lan_rule_set, 0, "dns", tmp_dns, TMP_LEN,
            EZPLIB_USE_CLI);
    ezplib_get_attr_val(lan_rule_set, 0, "wins", tmp_wins, TMP_LEN,
            EZPLIB_USE_CLI);
    len =
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s", lan_ipaddr, tmp_mask,
                tmp_gateway, tmp_dns, tmp_wins);
    if (len > TMP_LEN - 1) {
        return 0;
    }        
    nvram_fset(lan_rule_set, tmp);

    /* Replace http_rule_default with nvram_set(). */
    ezplib_get_attr_val(http_rule_set, 0, "name", tmp_name, TMP_LEN,
            EZPLIB_USE_CLI);
    ezplib_get_attr_val(http_rule_set, 0, "enable", tmp_enable, TMP_LEN,
            EZPLIB_USE_CLI);
    ezplib_get_attr_val(http_rule_set, 0, "rmgt_enable", tmp_rmgt, TMP_LEN,
            EZPLIB_USE_CLI);

    port = websGetVar(wp, "http_port_default", "");
    uid = websGetVar(wp, "userid_default", "");
    pw = websGetVar(wp, "passwd_default", "");

    len =
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s", tmp_name, tmp_enable,
                tmp_rmgt, uid, pw, port);
    if (len > TMP_LEN - 1) {
        return 0;
    }        

    nvram_fset(http_rule_set, tmp);

#ifdef CONFIG_EZP_ARCH_RT305X
    machine_id = websGetVar(wp, "machine_id", "");
    snprintf(tmp, sizeof(tmp), "echo \"ezpacket:%s\" > /proc/ezp_machine_id",
            machine_id);
    system(tmp);
#endif

    return 1;
}

int
save_license(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char *lkey;
    int64_t map = 0;
    char buf[LONG_BUF_LEN];
    FILE *fp;

    snprintf(buf, sizeof(buf),"/sbin/getsysinfo license_key"); 

    /* Obtain the license key according to the key values. */
    fp = popen(buf, "r");
    fscanf(fp, "%s\n", buf);
    if (fp) {
        pclose(fp);
    }

    lkey = websGetVar(wp, "license_key", "");
    if (!strcmp(lkey, buf)) {
        config_preaction(&map, v, s, "", "");
        /* Temporarily authenticate the machine. */
        nvram_fset("license_invalid", "0");
        config_postaction(map, s, "", "");
    }

    return 1;
}

int
save_backdoor(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int64_t map = 0;

    config_preaction(&map, v, s, "", "");
    ezplib_replace_attr("ssh_rule", 0, "enable", "1");
    config_postaction(map, s, "", "");

    return 1;
}

int
_save_operation_mode(char *mode)
{
    int i;
    if (!strcmp(mode, "gateway")) {
        if (strstr("ar8316", nvram_safe_get("switch_type"))) {
            /* set port 5 pvid to 2 */
            ezplib_replace_attr("vlanport_rule", 4, "pvid", "2");
            ezplib_replace_attr("vlan_rule", 0, "portmember", "111101");
            ezplib_replace_attr("vlan_rule", 1, "portmember", "000011");
        }
        else {
            /* set port 1 pvid to 2 */
            ezplib_replace_attr("vlanport_rule", 0, "pvid", "2");
            ezplib_replace_attr("vlan_rule", 0, "portmember", "0111101");
            ezplib_replace_attr("vlan_rule", 1, "portmember", "1000001");
        }
        /* enable wan */
        for (i = 0; i < atoi(nvram_safe_get("wan_num")); i++ ) {
            ezplib_replace_attr("wan_main_rule", i, "enable", "1");
        }
        /* enable DHCP server */
        ezplib_replace_attr("lan_dhcps_rule", 0, "enable", "1");
    } else if (!strcmp(mode, "bridge")) {
        if (strstr("ar8316", nvram_safe_get("switch_type"))) {
            /* set port 5 pvid to 1 */
            ezplib_replace_attr("vlanport_rule", 4, "pvid", "1");
            ezplib_replace_attr("vlan_rule", 0, "portmember", "111111");
            ezplib_replace_attr("vlan_rule", 1, "portmember", "000001");
        }
        else {
            /* set port 0 pvid to 1 */
            ezplib_replace_attr("vlanport_rule", 0, "pvid", "1");
            ezplib_replace_attr("vlan_rule", 0, "portmember", "1111101");
            ezplib_replace_attr("vlan_rule", 1, "portmember", "0000001");
        }
        /* disable wan */
        for (i = 0; i < atoi(nvram_safe_get("wan_num")); i++ ) {
            ezplib_replace_attr("wan_main_rule", i, "enable", "0");
        }
        /* disable DHCP server */
        ezplib_replace_attr("lan_dhcps_rule", 0, "enable", "0");
    }
    return 0;
}

int
save_op_mode(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN];
    char *op_mode;
    int64_t map = 0;
    snprintf(tmp, sizeof(tmp), "op_mode");
    op_mode = websGetVar(wp, tmp, "");
    if (strcmp(op_mode, nvram_safe_get("op_mode"))) {
        _save_operation_mode(op_mode);
        nvram_fset("op_mode", op_mode);
        map |= ACTION_REBOOT;
        config_preaction(&map, v, s, "", "");
        return 1;
    }
    return 0;
}

int
save_usbled(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN];
    char *usbled;
    int64_t map = 0;
    snprintf(tmp, sizeof(tmp), "usbled");
    usbled = websGetVar(wp, tmp, "");
    ezplib_get_attr_val("appmgr_rule", 0, "usbled", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    if (strcmp(usbled, tmp)) {
        config_preaction(&map, v, s, "", "");
        ezplib_replace_attr("appmgr_rule", 0, "usbled", usbled);
        ezplib_get_attr_val("appmgr_rule", 0, "usbled", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        config_postaction(map, s, "", "");
        return 1;
    }
    return 0;
}

struct service services[] = {
    { SERV_NETWORK_LAN, SERV_RC_COMMON "network-lan", "" },
    { SERV_NETWORK_WAN, SERV_RC_COMMON "network-wan", "" },
    { SERV_DNSMASQ, SERV_RC_COMMON "dnsmasq", "" },
    { SERV_CRON, SERV_RC_COMMON "cron", "" }, 
    { SERV_VNCREPEATER, SERV_RC_COMMON "vncrepeater", "" },
    { SERV_PPTPD, SERV_RC_COMMON "pptpd", "" },
    { SERV_L2TPD, SERV_RC_COMMON "l2tpd", "" },
    { SERV_IPSEC, "TYPE=wan" SERV_RC_COMMON "ipsec", "" },
    { SERV_OPENDNS, "TYPE=wan" SERV_RC_COMMON "opendns", "" },
    { SERV_UPNPD, "TYPE=wan" SERV_RC_COMMON "miniupnpd", "" },
    /* Specify TYPE=wan to run this script. */
    { SERV_UPDATEDD, "TYPE=wan" SERV_RC_COMMON "ddns", "" },
    /* Specify TYPE=wan to run this script. */
#ifdef AXIMDDNS
    { SERV_UPDATEAXIMDDNS, "TYPE=wan" SERV_RC_COMMON "aximddns", "" },
#endif
    { SERV_NTPCLIENT, "TYPE=wan" SERV_RC_COMMON "ntpclient", "" },
    { SERV_IPTABLES_RT, SERV_RC_COMMON "network-acl", "rt" },
    { SERV_IPTABLES_FL_HWADDR, SERV_RC_COMMON "network-acl", "fl_hwaddr" },
    { SERV_IPTABLES_FR, SERV_RC_COMMON "network-acl", "fr" },
    { SERV_IPTABLES_NAT_PASS, SERV_RC_COMMON "network-acl", "nat_pass" },
    { SERV_IPTABLES_FW, SERV_RC_COMMON "network-acl", "fw" },
    { SERV_IPTABLES_BW, SERV_RC_COMMON "network-acl", "bw" },
    { SERV_IPTABLES_FL, SERV_RC_COMMON "network-acl", "fl" },
    { SERV_IPTABLES_TOS, SERV_RC_COMMON "network-acl", "tos" },
    { SERV_IPTABLES_VS, SERV_RC_COMMON "network-acl", "vs" },
    { SERV_IPTABLES_WF, SERV_RC_COMMON "network-acl", "wf" },
    { SERV_TURBONAT, SERV_RC_COMMON "turbonat", "" },
    { SERV_MAC_CLONE, "/usr/sbin/ezp-mac-clone", "" },
    { SERV_BACKDOOR, SERV_RC_COMMON "dropbear", "" },
    { SERV_FTP, SERV_RC_COMMON "pure-ftpd", "" },
    { SERV_BT, SERV_RC_COMMON "bt", "" },
    { SERV_ISCSI, SERV_RC_COMMON "iscsi-target", "" },
    { SERV_SMB, SERV_RC_COMMON "samba", "" },
    { SERV_ITUNES, SERV_RC_COMMON "mt-daapd", "" },
    { SERV_SESSION_MANAGER, SERV_RC_COMMON "session", "" },
    { SERV_SMBC, SERV_RC_COMMON "smbc", "" },
    { SERV_WPS, SERV_RC_COMMON "wps", "" },
    { SERV_WIRELESS, "/sbin/ezp-wl-ctrl wds", "" },
    { SERV_WISP, "/sbin/ezp-wl-ctrl wisp", "" },
    { SERV_SNMP, SERV_RC_COMMON "mini_snmpd", "" },
    { SERV_DRIVE, SERV_RC_COMMON "lighttpd", "" },
    { SERV_CAMERA, SERV_RC_COMMON "mjpg-streamer", "" },
    { SERV_MUSIC, SERV_RC_COMMON "shairport", "" },
    { SERV_SYSLOGD, "/sbin/syslog-ng-init restart", "" },
    { SERV_BONJOUR, SERV_RC_COMMON "mDNSResponder", "" },
    { SERV_NETWORK_VPN, SERV_RC_COMMON "pptp", "" },
    { SERV_NETWORK_MYQNAPCLOUD, "qcloud_sample get_device_info", "" },
    { 0, NULL }
};

/* 
 * Variables are set in order (put dependent variables later). Set
 * nullok to TRUE to ignore zero-length values of the variable itself.
 * For more complicated validation that cannot be done in one pass or
 * depends on additional form components or can throw an error in a
 * unique painful way, write your own validation routine and assign it
 * to a hidden variable (e.g. filter_ip).
 */
struct variable variables[] = {
    /* CNNT */
    {"cnnt_rule", "cnnt", valid_cnnt, save_cnnt, ARGV(""), FALSE,
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
    /* EXPAND LAN */
    {"exp_lan_rule", "EXP Lan", valid_exp_lan, save_exp_lan,
     ARGV(""), FALSE, ACTION_REBOOT},
    /* EXPAND VPN */
    {"exp_vpn_rule", "EXP VPN", valid_exp_lan, save_exp_lan,
     ARGV(""), FALSE, (ACTION_NETWORK_VPN_STOP << 8)|ACTION_NETWORK_VPN_START},
    /*  */
#if defined(ENABLE_QCLOUD_ON_HTTPD)
    {"exp_myqnapcloud_rule", "EXP MyQnapCloud", valid_exp_myqnapcloud, save_exp_myqnapcloud,
     ARGV(""), FALSE, (ACTION_MYQNAPCLOUD_STOP << 8)|ACTION_MYQNAPCLOUD_START},
#endif
    /* WAN PRIORITY */
    {"wan_priority_rule", "WAN Priority", valid_wan_priority, save_wan_priority, ARGV(""), FALSE, 
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
    /* WAN */
#if defined(PLATFORM_M2E) || defined(PLATFORM_AXA)
    {"wan_rule", "WAN Protocol", valid_wan, save_cnnt, ARGV(""), FALSE,
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
#else
    {"wan_rule", "WAN Protocol", valid_wan, save_wan, ARGV(""), FALSE,
#ifdef EZP_PROD_BRAND_ZYUS
     (ACTION_MAC_CLONE_START<< 24)|
#endif
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
#endif
#if defined(PLATFORM_M2E) || defined(PLATFORM_AXA)
    /* SWAP WISP */
    {"wisp_swap_rule", "Internet Connection", valid_wisp_swap, save_cnnt, ARGV(""), FALSE,
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
#elif defined(EZP_SUB_BRAND_SONY) || defined(EZP_SUB_BRAND_GARMIN) || defined(PLATFORM_AXM)
    /* SWAP WISP */
    {"wisp_swap_rule", "Internet Connection", valid_wisp_swap, save_wisp_swap, ARGV(""), FALSE,
    (ACTION_WIRELESS_START << 16)|(ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},
#endif

    /* DDNS */
    {"ddns_rule", "DDNS Settings", valid_ddns, save_ddns,
     ARGV(""), FALSE, (ACTION_UPDATEDD_STOP << 8)|ACTION_UPDATEDD_START},
#ifdef AXIMDDNS
    /* AXIM DDNS */
    {"aximddns_rule", "AXIM DDNS Settings", valid_aximddns, save_aximddns,
     ARGV(""), FALSE, (ACTION_UPDATEAXIMDDNS_STOP << 8)|ACTION_UPDATEAXIMDDNS_START },
#endif
    /* LAN */
    {"lan_rule", "LAN Settings", valid_lan, save_lan,
     ARGV(""), FALSE, ACTION_REBOOT},

    /* DHCP */
    {"dhcp_rule", "DHCP Settings", valid_dhcp, save_dhcp, ARGV(""), FALSE,
     (ACTION_DNSMASQ_STOP << 8)|ACTION_DNSMASQ_START},

    /* OpenDNS */
    {"wan_opendns_rule", "OpenDNS Settings", valid_opendns, save_opendns, ARGV(""), FALSE,
                 (ACTION_OPENDNS_STOP << 8)|ACTION_OPENDNS_START},

    /* Hardware Address Clone */
    {"hwaddr_clone_rule", "Hardware Address Clone Rule", valid_clone, 
     save_clone, ARGV(""), FALSE, (ACTION_MAC_CLONE_STOP << 8)|ACTION_MAC_CLONE_START},

    /* DBM */
    {"bw_rule", "DBM Settings", valid_bw, save_bw, ARGV(""), FALSE, 
     (ACTION_IPTABLES_BW_STOP << 8)|ACTION_IPTABLES_BW_START},

    /* TurboNAT */
    {"turbonat_rule", "TurboNAT Settings", valid_turbonat, save_turbonat, ARGV(""), FALSE, 
     (ACTION_TURBONAT_STOP << 8)|ACTION_TURBONAT_START},

    /* Session Manager */
    {"sm_rule", "Session Manager Mode", valid_session_manager, save_session_manager, ARGV(""), FALSE, 
     (ACTION_SESSION_MANAGER_STOP << 8)|ACTION_SESSION_MANAGER_START},

    /* TOS */
    {"tos_rule", "TOS Settings", valid_tos, save_tos, ARGV(""), FALSE, 
     (ACTION_IPTABLES_TOS_STOP << 8)|ACTION_IPTABLES_TOS_START},

    /* Load Balance */
    {"lb_rule", "Load Balance", valid_lb, save_lb, ARGV(""), FALSE,
     (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},

    /* Routing */
    {"rt_rule", "Routing Policy", valid_rt, save_rt, ARGV(""), FALSE,
     (ACTION_IPTABLES_FL_STOP << 24)|(ACTION_IPTABLES_FL_START << 16)|
     (ACTION_IPTABLES_RT_STOP << 8)|ACTION_IPTABLES_RT_START},

    /* Cron */
    {"cron_rule", "Cron Rule", valid_cron, save_cron, 
     ARGV(""), FALSE, (ACTION_CRON_STOP << 8)|ACTION_CRON_START},

    /* NTP */
    {"ntp_rule", "Time Synchronization Rule", valid_ntp, save_ntp, 
     ARGV(""), FALSE, (ACTION_NTPCLIENT_STOP << 8)|ACTION_NTPCLIENT_START},

    /* PPTP */
    {"pptpd_rule", "PPTP", valid_pptpd, save_pptpd, 
     ARGV(""), FALSE, 
     (ACTION_IPTABLES_FR_STOP << 24)|(ACTION_IPTABLES_FR_START << 16)|
     (ACTION_PPTPD_STOP << 8)|ACTION_PPTPD_START},

    /* L2TP */
    {"l2tpd_rule", "l2TP", valid_l2tpd, save_l2tpd, 
     ARGV(""), FALSE, (ACTION_L2TPD_STOP << 8)|ACTION_L2TPD_START},

    /* IPsec */
    {"ipsec_rule", "IPsec", valid_ipsec, save_ipsec, 
     ARGV(""), FALSE, (ACTION_IPSEC_STOP << 8)|ACTION_IPSEC_START},

    /* Port Forward */
    {"fr_rule", "Port Forward", valid_fr, save_fr,
     ARGV(""), FALSE, (ACTION_IPTABLES_FR_STOP << 8)|ACTION_IPTABLES_FR_START},

    /* Virtual Server */
    {"vs_rule", "Virtual Server", valid_vs, save_vs,
     ARGV(""), FALSE, (ACTION_IPTABLES_VS_STOP << 8)|ACTION_IPTABLES_VS_START},

    /* FTP */
    {"ftpd_rule", "FTP", valid_ftpd, save_ftpd, 
     ARGV(""), FALSE, (ACTION_FTP_STOP << 8)|ACTION_FTP_START},

    /* SAMBA */
    {"smbd_rule", "SAMBA", valid_smbd, save_smbd, 
     ARGV(""), FALSE, (ACTION_SMB_STOP << 8)|ACTION_SMB_START},
    
    /* Drive */
    {"drive_rule", "Drive", valid_drive, save_drive, 
     ARGV(""), FALSE, (ACTION_DRIVE_STOP << 8)|ACTION_DRIVE_START},
    
    /* Power Saving */
    {"power_saving_rule", "Power Saving", valid_power, save_power, 
     ARGV(""), FALSE, },
    
    /* For togoPower App */
    {"setup_togoPower", "togoEye", valid_togopower, save_togopower, 
     ARGV(""), FALSE, 0},
    
    /* For togoEye App */
    {"setup_togoeye", "togoEye", valid_togoeye, save_togoeye, 
     ARGV(""), FALSE, (ACTION_CAMERA_STOP << 8)|ACTION_CAMERA_START},
    
    /* For togoDrive App */
    {"setup_togodrive", "togoDrive", valid_togodrive, save_togodrive, 
     ARGV(""), FALSE, (ACTION_DRIVE_STOP << 8)|ACTION_DRIVE_START},
    
    /* Camera */
    {"camera_rule", "Camera", valid_camera, save_camera, 
     ARGV(""), FALSE, (ACTION_CAMERA_STOP << 8)|ACTION_CAMERA_START},
    
    /* Music */
    {"music_rule", "Music", valid_music, save_music, 
    ARGV(""), FALSE, (ACTION_MUSIC_STOP << 24)|(ACTION_MUSIC_START << 16)|
    (ACTION_IPTABLES_BW_STOP << 8)|ACTION_IPTABLES_BW_START},
    /* iTunes */
    {"itunes_rule", "iTunes", valid_itunes, save_itunes,
    ARGV(""), FALSE, (ACTION_ITUNES_STOP << 8)|ACTION_ITUNES_START},
    
    /* BT */
    {"bt_rule", "BT", valid_bt, save_bt, 
     ARGV(""), FALSE, (ACTION_BT_STOP << 8)|ACTION_BT_START},

    /* Vlan */
    {"vlan_rule", "Vlan", valid_vlan, save_vlan, 
     ARGV(""), FALSE, ACTION_REBOOT},

    /* iSCSI */
    {"is_rule", "iSCSI", valid_iscsi, save_iscsi, 
     ARGV(""), FALSE, (ACTION_ISCSI_STOP << 8)|ACTION_ISCSI_START},

    /* Web Filter */
    {"wf_rule", "Web Filter", valid_wf, save_wf,
     ARGV(""), FALSE, (ACTION_IPTABLES_WF_STOP << 8)|ACTION_IPTABLES_WF_START},

    /* ACL */
    {"fl_rule", "ACL", valid_fl, save_fl,
     ARGV(""), FALSE, (ACTION_IPTABLES_FL_STOP << 8)|ACTION_IPTABLES_FL_START},

    /* Hwaddr Filter */
    {"fl_hwaddr_rule", "MAC Rule", valid_fl_hwaddr, save_fl_hwaddr,
     ARGV(""), FALSE, 
     (ACTION_IPTABLES_FL_HWADDR_STOP << 24)|(ACTION_IPTABLES_FL_HWADDR_START << 16)|
     (ACTION_DNSMASQ_STOP << 8)|ACTION_DNSMASQ_START},


    /* XXX: remove the unused items. */

    /* bw_rule. */

    /* for Management */
    {"http_rule", "HTTP Rule", valid_http, save_http,
     ARGV(""), FALSE,
     (ACTION_IPTABLES_FR_STOP << 16)|(ACTION_IPTABLES_FR_START << 8)|ACTION_RESTART},
    {"snmp_rule", "SNMP Enable", valid_snmp, save_snmp,
     ARGV(""), FALSE, (ACTION_SNMP_STOP << 8)|ACTION_SNMP_START},
    {"lang", "Web Language Setting", valid_lang, save_lang,
     ARGV("3"), FALSE, ACTION_NOTHING},
    {"adtool_rule", "System Utility Rule", valid_adtool, save_adtool,
     ARGV(""), FALSE, ACTION_NOTHING},

    /* for USB storage */
    {"storage_rule", "Storage Rule", valid_storage, save_storage,
     ARGV(""), FALSE, (ACTION_SMBC_STOP << 8)|ACTION_SMBC_START},

    /* for Wireless */
    {"wl_basic_rule", "Wireless Basic Rule", valid_wl_basic, save_wl_basic,
     ARGV(""), FALSE, (ACTION_WIRELESS_START << 16)|(ACTION_BONJOUR_STOP << 8)|ACTION_BONJOUR_START},
    {"wl_advanced_rule", "Wireless Advanced Rule", valid_wl_adv, save_wl_adv,
     ARGV(""), FALSE, ACTION_WIRELESS_START},
    {"wl_wds_rule", "Wireless WDS Rule", valid_wl_wds, save_wl_wds,
     ARGV(""), FALSE, ACTION_WIRELESS_START},
    {"wl_apcli_rule", "Wireless Universal Repeater Rule", valid_wl_apcli, 
        save_wl_apcli, ARGV(""), FALSE, ACTION_WIRELESS_START},
    {"wl_wps_rule", "Wireless WPS Rule", valid_wl_wps, save_wl_wps, ARGV(""),
        FALSE, (ACTION_WPS_STOP << 8)|ACTION_WPS_START},

    /* for UPnP */
    {"upnp_rule", "UPnP Rule", valid_upnp, save_upnp,
     ARGV("0", "1"), FALSE, (ACTION_UPNPD_STOP << 8)|ACTION_UPNPD_START},

    {"vnc_rule", "VNC Rule", valid_vnc, save_vnc,
     ARGV("0", "1"), FALSE, (ACTION_VNCREPEATER_STOP << 8)|ACTION_VNCREPEATER_START},

    /* for syslog */ 
    {"log_rule", "Log Rule", valid_log, save_log,
     ARGV("0", "1"), FALSE, (ACTION_SYSLOGD_STOP << 8)|ACTION_SYSLOGD_START},

    /* for firewall */
    {"fw_rule", "Firewall Rule", valid_fw,
     save_fw, ARGV("0", "1"), FALSE,
     (ACTION_IPTABLES_FW_STOP << 8)|ACTION_IPTABLES_FW_START},

    /* for nat passthrough */
    {"nat_pass_rule", "Firewall Rule", valid_nat_pass,
     save_nat_pass, ARGV("0", "1"), FALSE, 
     (ACTION_IPTABLES_NAT_PASS_STOP << 8)|ACTION_IPTABLES_NAT_PASS_START},

    /* for factory initialization */
    {"factory", "Authentication", valid_factory, save_factory, 
     ARGV(""), FALSE, ACTION_NOTHING},

    /* for special-case use to authenticate the machine */
    {"license", "License", valid_license, save_license, 
     ARGV(""), FALSE, (ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},

    /* for special-case use to enable ssh server */
    {"backdoor", "Backdoor", valid_backdoor, save_backdoor, 
     ARGV(""), FALSE, (ACTION_BACKDOOR_STOP << 8)|ACTION_BACKDOOR_START},

    /* for special-case use to operation mode */
    {"op_mode", "Operation Mode", valid_op_mode, save_op_mode, 
     ARGV(""), FALSE, ACTION_NOTHING},

    /* for user to access usb led to avoid authentication status */
    {"usbled", "USB Led Blink Behavior", valid_usbled, save_usbled, 
     ARGV(""), FALSE, ACTION_NOTHING},

    /* for Getting Started Wizard */
    {"wizard_rule", "Wizard", valid_wizard, save_wizard, ARGV(""), FALSE,
     (ACTION_NTPCLIENT_STOP << 24)|(ACTION_NTPCLIENT_START << 16)|(ACTION_NETWORK_WAN_STOP << 8)|ACTION_NETWORK_WAN_START},

    /* for VPN Wizard */
    {"vpn_wizard_rule", "VPN Wizard", valid_vpn_wizard, save_vpn_wizard,
     ARGV(""), FALSE,
     (ACTION_PPTPD_STOP << 24)|(ACTION_PPTPD_START << 16)|(ACTION_IPSEC_STOP << 8)|ACTION_IPSEC_START}

};

/*
 * Return TRUE/FALSE if all of config inputs are correct/incorrect.
 * Also, 
 */
static int
config_validate(webs_t wp, int show_msg)
{
    struct variable *v;
    char *value;
    int ret = TRUE;

#ifdef DEBUG
    printf("config_validate()\n");
#endif

    /* Set to show config validation message or not. */
    config_validate_msg = show_msg;

    /* Validate variables in table order. */
    for (v = variables; v < &variables[STRUCT_LEN(variables)]; v++) {
        value = websGetVar(wp, v->name, NULL);
#ifdef DEBUG
        printf("Matching: %s -> %s\n", v->name, (value) ? value : "");
#endif
        if (!value) {
            /* v->name doesn't exist in the incoming POST */
            continue;
        }
        if ((!*value) && v->nullok) {
            /* Empty string and nullok is TRUE. */
            continue;
        }

        if (v->valid(wp, value, v) == FALSE) {
#ifdef DEBUG
            printf("v->name=%s[%s] FALSE [error_value:%d]\n", v->name,
                   value, error_value);
#endif
            ret = FALSE;
        } else {
#ifdef DEBUG
            printf("v->name=%s[%s] TRUE [error_value:%d]\n", v->name,
                   value, error_value);
#endif
        }
    }

    config_validate_msg = 0;
    return ret;
}

int
_widget_wwan_cgi(webs_t wp, char_t * urlPrefix, char_t * webDir, int arg,
           char_t * url, char_t * path, char_t * query)
{
    FILE *f;
    char buf[TMP_LEN];
    int nbrand = 0, nmodel = 0;
    int num;
#if defined (EZP_SUB_BRAND_GARMIN) || (EZP_PROD_BRAND_PROX)
    FILE *ppf;
    int wlan_clients=0;
#endif
#if defined (EZP_PROD_BRAND_PROX)
    int lan_clients=0;
    int ipsec_tunnel_num=0;
    int pptpc_num=0;
    int ipsec_enable=0;
    int ipsec_rule_num=0;
    int run_ipsec_auto=0;
    int i;
#endif
    /* Modem information */
    num = atoi(nvram_safe_get("wan_num"))-1;

    /* Manufacturer */
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "brand", buf, sizeof(buf),
            EZPLIB_USE_CLI);
    if (buf[0] == '\0') {
        websWrite(wp, "wwan_on=0\n");
    }

    if (buf[0]) {
        nbrand = atoi(buf);
        websWrite(wp, "Manufacturer=%s\n", ezp_com_vendor_map[nbrand].desc);
    } 
    else {
        ezplib_get_attr_val("wan_wwan_rule", num, "brand", buf, sizeof(buf),
            EZPLIB_USE_CLI);
        nbrand = atoi(buf);
        websWrite(wp, "Manufacturer=%s\n", ezp_com_vendor_map[nbrand].desc);
    }

    /* Model */
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "model", buf, sizeof(buf),
            EZPLIB_USE_CLI);

    if (buf[0]) {
        nmodel = atoi(buf);
        websWrite(wp, "Model=%s\n", ezp_com_pointer_map[nbrand][nmodel].desc);
    }
    else {
        ezplib_get_attr_val("wan_wwan_rule", num, "model", buf, sizeof(buf),
            EZPLIB_USE_CLI);
        nmodel = atoi(buf);
        websWrite(wp, "Model=%s\n", ezp_com_pointer_map[nbrand][nmodel].desc);
    }

    /* Signal */
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "sig", buf, sizeof(buf),
            EZPLIB_USE_CLI);
    if (buf[0])  websWrite(wp, "Signal=%s\n", buf); 

    /* Signal in Percentage Presentation */
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "signal", buf, sizeof(buf),
            EZPLIB_USE_CLI);
    if (buf[0])  websWrite(wp, "Sig_percent=%s\n", buf); 

    /* IMEI */
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "imei", buf, sizeof(buf),
            EZPLIB_USE_CLI);
    if (buf[0])  websWrite(wp, "IMEI=%s\n", buf); 

    /* Provider */
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "provider", buf, sizeof(buf),
            EZPLIB_USE_CLI);
    if (buf[0]){  
        websWrite(wp, "ISP=%s\n", buf); 
    } else {
        ezplib_get_attr_val("wan_wwan_probe_rule", num, "isp", buf, sizeof(buf),
            EZPLIB_USE_CLI);
        websWrite(wp, "ISP=%s\n", buf); 
    }

    /* Firmware Version */
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "firmware", buf, sizeof(buf),
            EZPLIB_USE_CLI);
    if (buf[0])  websWrite(wp, "Firm_ver=%s\n", buf); 
    
#if defined (EZP_SUB_BRAND_GARMIN) || (EZP_PROD_BRAND_PROX)
    if ((ppf = popen("/sbin/getsysinfo wlan_clients", "r"))) {
        fscanf(ppf, "%d\n", &wlan_clients);
        pclose(ppf);
    }
    websWrite(wp, "wlan_clients=%d\n", wlan_clients);
#endif

#if defined (EZP_PROD_BRAND_PROX)
    ipsec_enable=atoi(nvram_safe_get("ipsec_enable"));
    ipsec_rule_num=atoi(nvram_safe_get("ipsec_rule_num"));
    if (ipsec_enable && ipsec_rule_num > 0) 
    {
        for (i=0; i < ipsec_rule_num; i++) {
            ezplib_get_attr_val("ipsec_rule", i, "enable", buf, TMP_LEN, EZPLIB_USE_CLI);
            if (atoi(buf)==1) {
                run_ipsec_auto=1;
                break;
            }
        }
        if ( run_ipsec_auto ) {
            if ((ppf = popen("/sbin/getsysinfo ipsec_tunnel_num", "r"))) {
                fscanf(ppf, "%d\n", &ipsec_tunnel_num);
                pclose(ppf);
            }
        }
    }
    websWrite(wp, "ipsec_tunnel=%d\n", ipsec_tunnel_num);

    if ((ppf = popen("/sbin/getsysinfo pptpc_num", "r"))) {
        fscanf(ppf, "%d\n", &pptpc_num);
        pclose(ppf);
    }
    websWrite(wp, "pptpc_num=%d\n", pptpc_num);

    if ((ppf = popen("/sbin/getsysinfo lan_clients", "r"))) {
        fscanf(ppf, "%d\n", &lan_clients);
        pclose(ppf);
    }
    websWrite(wp, "lan_clients=%d\n", lan_clients-wlan_clients);
#endif
    /* WWAN connection information */
    if (!(f = fopen("/var/stats/stats-widget-wwan", "r"))) {
        return 1;
    }

    while (fgets(buf, sizeof(buf), f)) {
        websWrite(wp, buf);
    }

    fclose(f);
    return 0;
}



void
adj_size(float val, char *buf, int blen) 
{
    if (val < 1024) {
        snprintf(buf, blen,"%.1fKB", val);
    } else {
        val = val / 1024;
        if (val < 1024) {
        snprintf(buf, blen,"%.1fMB", val);
        } else {
            val = val / 1024;
            snprintf(buf, blen,"%.1fGB", val);
        }
    }
    return; 
}
void
get_storage_info(webs_t wp, char *device_name)
{
    FILE *f;
    int free_size, size, use_size;
    char buf[TMP_LEN];
    char storage_size[SHORT_BUF_LEN],storage_usize[SHORT_BUF_LEN],
         storage_upercentage[SHORT_BUF_LEN],storage_fsize[SHORT_BUF_LEN];

    char path[SHORT_BUF_LEN],f_size[SHORT_BUF_LEN],dir[SHORT_BUF_LEN]={0};
    snprintf(buf, sizeof(buf),
            "/sbin/getsysinfo disk_info %s", device_name);
    if ((f = popen(buf, "r"))) {
        fscanf(f, "%s %s %s %s %s %s", path, storage_size, storage_usize,
               f_size, storage_upercentage, dir);
        /* If the result path of df is too long, the output will be two lines.
         * In this case, the string length of "dir" is zero, and the output
         * result shift forward one word */
        if ((strlen(dir)) > 0) {
            /* output result is normal */
            size = atoi(storage_size); 
            use_size = atoi(storage_usize);
        } else {
            /* output result is two lines */
            size = atoi(path); 
            use_size = atoi(storage_size);
            strcpy(storage_upercentage, f_size);
        }
        adj_size((float)size, storage_size, sizeof(storage_size));
        free_size = size - use_size; 
        adj_size((float)use_size, storage_usize, sizeof(storage_usize));
        adj_size((float)free_size, storage_fsize, sizeof(storage_fsize));
        websWrite(wp, "storage_size=%s\n", storage_size);
        websWrite(wp, "storage_info=%s/%s(%s)\n",
                storage_usize, storage_fsize, storage_upercentage);
        websWrite(wp, "dev_msg=%s\n", lang_translate("Mounted"));

        pclose(f);
    }
}
int
_widget_storage_cgi(webs_t wp, char_t * urlPrefix, char_t * webDir, int arg,
           char_t * url, char_t * path, char_t * query)
{

    FILE *f;
    char buf[TMP_LEN];
    char device_name[SHORT_BUF_LEN],storage_type[SHORT_BUF_LEN],
         storage_state[SHORT_BUF_LEN];

    /* Storage infomation */
    ezplib_get_attr_val("storage_state_rule", 0, "dev_name",
                        device_name, sizeof(device_name), EZPLIB_USE_CLI);
    ezplib_get_attr_val("storage_state_rule", 0, "mount_state",
                        storage_state, sizeof(storage_state), EZPLIB_USE_CLI);

    if(!strcmp(device_name, "mnt/USB") || !strcmp(device_name, "nfs")) {
        ezplib_get_attr_val("storage_state_rule", 0, "format",
                storage_type, sizeof(storage_type), EZPLIB_USE_CLI);
        websWrite(wp, "storage_type=%s\n", storage_type);
        get_storage_info(wp, device_name);
    } else {
        websWrite(wp, "storage_type=\n");
        websWrite(wp, "storage_size=\n");
        websWrite(wp, "storage_info=\n");
        if (strcmp(storage_state, "unknown") == 0) {
            websWrite(wp,
                    "dev_msg=%s\n", lang_translate("Unknown"));
        } else if(strcmp(storage_state, "format") == 0) {
            websWrite(wp,
                    "dev_msg=%s\n", lang_translate("Formatting"));
        } else if(strcmp(storage_state, "con_err") == 0) {
            websWrite(wp,
                    "dev_msg=%s\n", lang_translate("Connect Error"));
        } else {
            websWrite(wp,
                    "dev_msg=%s\n", lang_translate("Unplugged"));
        }
    }
    memset(buf, 0, sizeof(buf));
    if ((f = popen("pidof transmission-daemon", "r"))) {
        fgets(buf, sizeof(buf), f);    
        if(atoi(buf) > 0 ) {
            websWrite(wp, "bt_on=0\n");
        } else {
            websWrite(wp, "bt_on=1\n");
        }
        pclose(f);
    }
    
    memset(buf, 0, sizeof(buf));
    if ((f = popen("pidof pure-ftpd", "r"))) {
        fgets(buf, sizeof(buf), f);    
        if(atoi(buf) > 0 ) {
            websWrite(wp, "ftp_on=0\n");
        } else {
            websWrite(wp, "ftp_on=1\n");
        }
        pclose(f);
    }

    memset(buf, 0, sizeof(buf));
    if ((f = popen("pidof smbd", "r"))) {
        fgets(buf, sizeof(buf), f);    
        if(atoi(buf) > 0 ) {
            websWrite(wp, "samba_on=0\n");
        } else {
            websWrite(wp, "samba_on=1\n");
        }
        pclose(f);
    }

    memset(buf, 0, sizeof(buf));
    if ((f = popen("pidof mt-daapd", "r"))) {
        fgets(buf, sizeof(buf), f);    
        if(atoi(buf) > 0 ) {
            websWrite(wp, "itunes_on=0\n");
        } else {
            websWrite(wp, "itunes_on=1\n");
        }
        pclose(f);
    }

    memset(buf, 0, sizeof(buf));
    if ((f = popen("pidof ietd", "r"))) {
        fgets(buf, sizeof(buf), f);    
        if(atoi(buf) > 0 ) {
            websWrite(wp, "iscsi_on=0\n");
        } else {
            websWrite(wp, "iscsi_on=1\n");
        }
        pclose(f);
    }

    if ((f = popen("/sbin/getsysinfo swap_onoff", "r"))) {
        fgets(buf, sizeof(buf), f);    
        if(atoi(buf) > 0 ) {
            websWrite(wp, "swap_on=0\n");
        } else {
            websWrite(wp, "swap_on=1\n");
        }
        pclose(f);
    }
    return 0;
}

static int
widget_cgi(webs_t wp, char_t * urlPrefix, char_t * webDir, int arg,
           char_t * url, char_t * path, char_t * query)
{
    FILE *f;
    char buf[TMP_LEN];
    char status[TMP_LEN];
    char *val;
    int i, num;
    char uptime[TMP_LEN];
#if defined (EZP_PROD_BRAND_PROX)
    int j, wnum;
    char localtime[TMP_LEN], buf2[TMP_LEN];
#endif
    char mask[TMP_LEN];

    /* Status: 0/1/2 for disconnected/connected/else */
    num = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < num; i++) {
        snprintf(buf, sizeof(buf), "wan%d_proto", i);
        val = nvram_safe_get(buf);
        if (!strcmp(val,"wisp")) websWrite(wp, "wan%d_widget_proto=Wi-Fi&nbsp;Client\n", i);
        else websWrite(wp, "wan%d_widget_proto=%s\n", i, val);
        /* Get wan status */
        ezplib_get_attr_val("wan_status_rule", i, "state", status,
                sizeof(status), EZPLIB_USE_CLI);
        websWrite(wp, "wan%d_status=%s\n", i, status);
        if (atoi(status) >= 2) {
            snprintf(buf, sizeof(buf), "wan%d_ipaddr", i);
            val = nvram_safe_get(buf);
            websWrite(wp, "wan%d_ipaddr=%s\n", i, val);
            snprintf(buf, sizeof(buf), "wan%d_mask", i);
            val = nvram_safe_get(buf);
            trans_int2netmask(atoi(val), mask, sizeof(mask));
            websWrite(wp, "wan%d_mask=%s\n", i, mask);
            snprintf(buf, sizeof(buf), "wan%d_gateway", i);
            val = nvram_safe_get(buf);
            websWrite(wp, "wan%d_gateway=%s\n", i, val);
        } else {
            websWrite(wp, "wan%d_ipaddr=%s\n",  i, "0.0.0.0");
            websWrite(wp, "wan%d_mask=%s\n",    i, "0.0.0.0");
            websWrite(wp, "wan%d_gateway=%s\n", i, "0.0.0.0");
        }
        ezplib_get_attr_val("wan_status_rule", i, "uptime", buf,sizeof(buf), 
                EZPLIB_USE_CLI);
        cal_ifuptime(uptime, TMP_LEN, atol(buf));
        websWrite(wp, "wan%d_uptime=%s\n", i, uptime);

        /* Get vpn status */
        ezplib_get_attr_val("vpn_status_rule", i, "state", status,
                sizeof(status), EZPLIB_USE_CLI);
        websWrite(wp, "vpn%d_status=%s\n", i, status);
        if (atoi(status) >= 2) {
            snprintf(buf, sizeof(buf), "vpn%d_ipaddr", i);
            val = nvram_safe_get(buf);
            websWrite(wp, "vpn%d_ipaddr=%s\n", i, val);
            snprintf(buf, sizeof(buf), "vpn%d_mask", i);
            val = nvram_safe_get(buf);
            trans_int2netmask(atoi(val), mask, sizeof(mask));
            websWrite(wp, "vpn%d_mask=%s\n", i, mask);
        }
#if defined (EZP_PROD_BRAND_PROX)
        else {
            websWrite(wp, "vpn%d_ipaddr=%s\n",  i, "0.0.0.0");
            websWrite(wp, "vpn%d_mask=%s\n",    i, "0.0.0.0");
            websWrite(wp, "vpn%d_gateway=%s\n", i, "0.0.0.0");
        }
#endif 
        ezplib_get_attr_val("vpn_status_rule", i, "uptime", buf, 
                sizeof(buf), EZPLIB_USE_CLI);
        cal_ifuptime(uptime,TMP_LEN, atoi(buf));
        websWrite(wp, "vpn%d_uptime=%s\n", i, uptime);
    }

#if defined (EZP_PROD_BRAND_PROX)
    //Wi-Fi on/off status
    ezplib_get_attr_val("wl_basic_rule", 0, "enable", buf, sizeof(buf), EZPLIB_USE_CLI);
    websWrite(wp, "wlan_status=%s\n",buf);

    //system uptime display
    show_uptime(uptime,TMP_LEN);
    websWrite(wp, "system_uptime=%s\n",uptime);

    //local time display
    show_localtime(localtime,TMP_LEN);
    websWrite(wp, "local_time=%s\n",localtime);

    num = atoi(nvram_safe_get("wl_num"));
    for (i = 0; i < num; i++) {
        snprintf(buf, sizeof(buf), "wl%d_sec_rule", i);
        wnum = atoi(nvram_safe_get("wlv_rule_num"));
        for (j = 0; j < wnum; j++) {
            ezplib_get_attr_val(buf, j, "secmode", buf2,sizeof(buf2), 
                    EZPLIB_USE_CLI);
            if (!strcmp(buf2,"disabled")) snprintf(buf2, sizeof(buf2), "Disabled");
            websWrite(wp, "wl%d_security%d=%s\n", i, j, buf2);
        }
    }
#endif 
    if (!(f = fopen("/var/stats/stats-widget-system", "r"))) {
        return 1;
    }

    while (fgets(buf, sizeof(buf), f)) {
        websWrite(wp, buf);
    }

    fclose(f);

    if (appmgr_check("wwan")){
            _widget_wwan_cgi(wp, urlPrefix, webDir, arg, url, path, query);
    }
    else if (appmgr_check("storage")){
            if (strcmp(ezp_prod_subsubcat, "L") != 0){
                _widget_storage_cgi(wp, urlPrefix, webDir, arg, url, path, query);
            }
    }
    return 0;
}

int
gozila_cgi(webs_t wp, char_t * urlPrefix, char_t * webDir, int arg,
           char_t * url, char_t * path, char_t * query)
{
    char *submit_button;

    gozila_action = 1;

    /* every html must have the name */
    submit_button = websGetVar(wp, "submit_button", NULL); 

#ifdef DEBUG
    printf("gozila_cgi submit_button=[%s]\n", submit_button);
#endif

    sprintf(path, "%s.asp", submit_button);

    do_ej(path, wp, "");                  //refresh
    websDone(wp, 200);

    gozila_action = 0;                      //reset gozila_action

    return 1;
}

/* 
 * If no change occurs, return 0. If changes, return 1.
 */
static int
config_save(webs_t wp)
{
    struct variable *v;
    char *value;
    int ret = 0;

#ifdef DEBUG
    printf("config_save()\n");
#endif

    /* Save variables in the table order. */
    for (v = variables; v < &variables[STRUCT_LEN(variables)]; v++) {
        value = websGetVar(wp, v->name, NULL);
#ifdef DEBUG
        printf("Matching: %s -> %s\n", v->name, (value) ? value : "");
#endif
        if (!value) {
            continue;
        }
        /* See whether any changes occur and save the change to NVRAM. */
        if (v->save(wp, value, v, services)) {
            ret = 1;
        }
    }
    return ret;
}


#define SERV_BITLEN 8
#define SERV_MASK   0xff
#define SERV_NGROUP (64 / SERV_BITLEN)

static void setact(int64_t actflag, int64_t *actlist) {
    int i = 0;
    for (i = 0; i < SERV_NGROUP; i++) {
        actlist[i] = actflag & SERV_MASK;
        actflag = actflag >> SERV_BITLEN;
    }
}

/* 
 * (1) Check whether the given service has been stopped. If none, stop it. 
 * (2) Add the action bits into the map.  
 */
void
config_preaction(int64_t *map, struct variable *v, struct service *s, char *prefix,
        char *postfix)
{
    char tmp[TMP_LEN];
    int64_t servact[SERV_NGROUP], mapact[SERV_NGROUP];
    int i;

    setact(v->action_flag, servact);
    setact(*map, mapact);
    for (; s && s->script; s++) {
        /* Find out what service should be stopped. */
        for (i = 0; i < SERV_NGROUP; i++) {
            if ((mapact[i] == ACTION_REBOOT) || 
                ((servact[i] == ACTION_REBOOT) && 
                 (mapact[i] != ACTION_FORCE_NOT_REBOOT))) {
                reboot_action = 1;
            }
            else if ((mapact[i] == ACTION_RESTART) ||
                     (servact[i] == ACTION_RESTART)) {
                reboot_action = 2;
            }
            else if ((s->flag == servact[i]) && 
                     (s->flag != mapact[i]) && 
                     (mapact[i] != ACTION_REBOOT)) {
                snprintf(tmp, TMP_LEN, "%s %s stop %s %s", prefix, s->script, 
                        s->script2, postfix);
#ifdef DEBUG
                printf("%s: %s (preaction_begin)\n", __FUNCTION__, tmp);
#endif
                system(tmp);
#ifdef DEBUG
                printf("%s: %s (preaction_end)\n", __FUNCTION__, tmp);
#endif
            }
        }
    }

    /* Add the action bits into the map. */
    (*map) = v->action_flag;
    /* If the reboot bit is on, turn on the reboot action variable. */
    /*
    if (*map & ACTION_REBOOT) {
        reboot_action = 1;
    }
    */
}

/* 
 * Check whether the givne service has been started.
 */
void
config_postaction(int64_t map, struct service *s, char *prefix, char *postfix)
{
    char tmp[TMP_LEN];
    int64_t mapact[SERV_NGROUP];
    int i;

    setact(map, mapact);
    for (; s->script; s++) {
        /* Find out what service should be started. */
        for (i = 0; i < SERV_NGROUP; i++) {
            if ((s->flag + 1) == mapact[i]) {
                snprintf(tmp, TMP_LEN, "%s %s start %s %s", prefix, s->script, 
                        s->script2, postfix);
#ifdef DEBUG
                printf("%s: %s(postaction_begin)\n", __FUNCTION__, tmp);
#endif
                if (fork() == 0)
                {
                    close(conn_fd);
                    system(tmp);
                    exit(0);
                }
#ifdef DEBUG
                printf("%s: %s(postaction_end)\n", __FUNCTION__, tmp);
#endif
            }
        }
    }
}

/*
 * This function requires some clean up! 
 */
int
apply_cgi(webs_t wp, char_t * urlPrefix, char_t * webDir, int arg,
          char_t * url, char_t * path, char_t * query)
{
    char *value;
    char page[SHORT_BUF_LEN];

    /* If the applied content is empty, quit then. */
    if (!query)
        goto footer;

    /* Static variables. */
    error_value = 0;

    /* Find out the value of "action". */
    value = websGetVar(wp, "action", "");
    if (!*value) {
        /* No action! */
        /* TODO:XXX: Send out "action required" message. */
        return 1;
    } else if (!strcmp(value, "Widget")) {
        widget_cgi(wp, urlPrefix, webDir, arg, url, path, query);
        return 1; 
    } else if (!strcmp(value, "Gozila")) {
        /* Gozilla action. */
        gozila_cgi(wp, urlPrefix, webDir, arg, url, path, query);
        return 1;
    } else if (!strcmp(value, "Apply")) {
        /* 
         * Validate the query first. The checking result will be stored at
         * error_value. If errors occur, the failure reason page should be 
         * shown. 
         */
        int ret = config_validate(wp, 0);
        if (ret == FALSE) {
#ifdef DEBUG
            printf("config_validate() error_value=%d\n", error_value);
#endif
            goto err;
        }
#ifdef DEBUG
        printf("config_validate() error_value=%d\n", error_value);
#endif
        snprintf(page, SHORT_BUF_LEN, "%s.asp",
                 websGetVar(wp, "submit_button", "wan"));        
        /* 
         * Check if any changes of variables. If yes, 
         * (1) config_preaction() to turn on action bits and stop related 
         * service.
         * (2) nvram_set() and nvram_commit() all of the changes.
         * (3) config_postaction() to restart the services.
         *
         * If any change, return 1. Otherwise, return 0.
         */
        if (config_save(wp)) {
            nvram_commit();
            goto footer;
        } else {                /* Nothing is changed. */
            goto no_change;
        }
    } else if (!strcmp(value, "lang")) { /*for M1 login UI*/
        char *value;
        struct variable *v;
        value = websGetVar(wp, "lang", NULL);
        for (v = variables; v < &variables[STRUCT_LEN(variables)]; v++) {
            if (strcmp(v->name, "lang") == 0) {
                break;
            }
        }
        if (valid_lang(wp, value, v) == FALSE) {
            goto err;
        }
        if (save_lang(wp, value, v, services)) {
            nvram_commit();
        }
    } else if (!strcmp(value, "Login")) {
        char *value;
        struct variable *v;
        if (!auth_state) {
#ifndef EZP_PROD_BRAND_ZYUS
            /* search language position at variable table */
            for (v = variables; v < &variables[STRUCT_LEN(variables)]; v++) {
                if (strcmp(v->name, "lang") == 0) {
                    break;
                }
            }
            /* Check Login page submit language */
            value = websGetVar(wp, "lang", NULL);
            if (valid_lang(wp, value, v) == FALSE) {
                goto err;
            }
            if (save_lang(wp, value, v, services)) {
                nvram_commit();
            }
#endif
            websWrite(wp, "return_value=%s", EZP_RETURN_LOGIN_SUCCESS); 
            websDone(wp, 200);
            return 1;
        } else {
            goto err;
        }
    } else {
        /* Undefined action. */
        websDebugWrite(wp, "Undefined action %s<br>", value);
    }

footer:
    if (reboot_action == 1) {
        char lanip[TMP_LEN], port[TMP_LEN];
        ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lanip,
                TMP_LEN, EZPLIB_USE_CLI);
        ezplib_get_attr_val("http_rule", 0, "port", port,
                TMP_LEN, EZPLIB_USE_CLI);
        /* XXX:System reboot. */
        websWrite(wp, "return_value=%s,%s:%s", EZP_RETURN_REBOOT, lanip, port); 
        /* Force to flush right away! */
        websDone(wp, 200);
#if defined(PLATFORM_AXA)
        system("ezp-i2c gauge host booting");
#endif
        system("/sbin/reboot");
    } else if (reboot_action == 2) {
        char lanip[TMP_LEN], port[TMP_LEN];
        ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lanip,
                TMP_LEN, EZPLIB_USE_CLI);
        ezplib_get_attr_val("http_rule", 0, "port", port,
                TMP_LEN, EZPLIB_USE_CLI);
        /* Web server restart. */
        websWrite(wp, "return_value=%s,%s:%s", EZP_RETURN_RESTART, lanip, port); 
        /* Force to flush right away! */
        websDone(wp, 200);
#ifdef AXIMDDNS
    } else if (reboot_action == 3) {
        websWrite(wp, "return_value=%s,%s",EZP_RETURN_MESSAGE,aximddns_message);
        websDone(wp, 200);
        reboot_action = 0;
#endif
    } else {
        websWrite(wp, "return_value=%s", EZP_RETURN_SUCCESS); 
        websDone(wp, 200);
    }

    return 1;

no_change:
    websWrite(wp, "return_value=%s", EZP_RETURN_NOCHANGE); 
    websDone(wp, 200);
    return 1;

err:
    printf("%s(%d) return_value=%s\n", __func__, __LINE__, EZP_RETURN_FAIL);
    websWrite(wp, "return_value=%s", EZP_RETURN_FAIL); 
    websDone(wp, 200);
    return 1;
}


static void
do_auth(char *userid, char *passwd, char *realm)
{
    ezplib_get_attr_val("http_rule", 0, "username", userid, 
            AUTH_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("http_rule", 0, "passwd", passwd, 
            AUTH_LEN, EZPLIB_USE_CLI);
    strncpy(realm, nvram_safe_get("brand"), AUTH_LEN);
}

void
do_apply_post(char *url, webs_t stream, int len, char *boundary)
{
#if defined(HTTPS_SUPPORT)
    char buf[1024];
#endif
    int count;

    post_buf[0] = '\0';;
    if (post == 1) {
        if (len > sizeof(post_buf) - 1) {
            cprintf("The POST data exceed length limit!\n");
            return;
        }

        /* Get query */
        if (!(count = wfread(post_buf, 1, len, stream)))
            return;
        post_buf[count] = '\0';;
        len -= strlen(post_buf);

        /* Skip anything remaining in the request */
        while (--len > 0)
#if defined(HTTPS_SUPPORT)
            if (do_ssl)
                BIO_gets((BIO *) stream, buf, 1);
            else
#endif
                (void) fgetc(stream);
        init_cgi(post_buf);
    }
}

static void
do_apply_cgi(char *url, webs_t stream, ...)
{
    char *path, *query;

    if ((auth_state) && ((strcmp(post_buf,"login_rule") != 0) && \
                (strcmp(url,"login.asp") != 0))) {
        do_ej("login.asp", stream, "");
        return;
    }
    if (post == 1) {            /* POST */
        query = post_buf;
        path = url;
    } else {                    /* GET */
        query = url;
        path = strsep(&query, "?") ? : url;
        init_cgi(query);
    }

    if (!query) {
        goto err;
    }
    apply_cgi(stream, NULL, NULL, 0, url, path, query);
err:
    /* Flush all key-value pairs in tha hash table. */
    init_cgi(NULL);
}

static void
do_connect_cgi(char *url, webs_t stream, ...)
{
    char *value, *num;
    char buf[TMP_LEN];
    /* Find out the value of "type". */
    value = websGetVar(stream, "type", "");
    if (!*value) {
        /* No type! */
        websDebugWrite(stream, "Empty type at %s<br>", __FUNCTION__);
    } else if (!strcmp(value, "iface")) {
        num = websGetVar(stream, "num", "");
        if (!*num) {
            goto err;
        }
        /* enable wan */
        ezplib_replace_attr("wan_main_rule" , atoi(num), "enable", "1");
        snprintf(buf, TMP_LEN,
                "NUM=%s /sbin/ifcontrol wan_updown start", num);
        system(buf);
        nvram_commit();
        
#if defined (EZP_PROD_BRAND_PROX)
    } else if (!strcmp(value, "wifi")) {
        /* enable wifi */
        ezplib_replace_attr("wl_basic_rule" , 0, "enable", "1");
        snprintf(buf, TMP_LEN,"/sbin/ezp-wl-ctrl wds start");
        system(buf);
        nvram_commit();
#endif        
    } else if (!strcmp(value, "usb")) {
        return;
    } else if (!strcmp(value, "bt")) {
        system("/etc/rc.common /etc/init.d/bt start");
    } else if (!strcmp(value, "ftp")) {
        system("/etc/rc.common /etc/init.d/pure-ftpd start");
    } else if (!strcmp(value, "samba")) {
        system("/etc/rc.common /etc/init.d/samba start");
    } else if (!strcmp(value, "itunes")) {
        system("/etc/rc.common /etc/init.d/mt-daapd start");
    } else if (!strcmp(value, "iscsi")) {
        system("/etc/rc.common /etc/init.d/iscsi-target start");
    } else {
        /* Undefined action. */
        websDebugWrite(stream, "Undefined type at %s %s<br>", __FUNCTION__, value);
    }
err:
    /* Flush all key-value pairs in tha hash table. */
    init_cgi(NULL);
}

static void
do_disconnect_cgi(char *url, webs_t stream, ...)
{
    char *value, *num;
    char buf[TMP_LEN];
    /* Find out the value of "type". */
    value = websGetVar(stream, "type", "");
    if (!*value) {
        /* No type! */
        websDebugWrite(stream, "Empty type at %s<br>", __FUNCTION__);
    } else if (!strcmp(value, "iface")) {
        num = websGetVar(stream, "num", "");
        if (!*num) {
            goto err;
        }
        /* disable wan */
        ezplib_replace_attr("wan_main_rule" , atoi(num), "enable", "0");
        snprintf(buf, TMP_LEN,
                "NUM=%s /sbin/ifcontrol wan_updown stop", num);
        system(buf);
        nvram_commit();
    } else if (!strcmp(value, "wifi")) {
        /* disable wifi */
        ezplib_replace_attr("wl_basic_rule" , 0, "enable", "0");
        snprintf(buf, TMP_LEN,"/sbin/ezp-wl-ctrl wds stop");
        system(buf);
        nvram_commit();
    } else if (!strcmp(value, "usb")) {
        system("/sbin/usb-remove");
    } else if (!strcmp(value, "bt")) {
        system("/etc/rc.common /etc/init.d/bt stop");
    } else if (!strcmp(value, "ftp")) {
        system("/etc/rc.common /etc/init.d/pure-ftpd stop");
    } else if (!strcmp(value, "samba")) {
        system("/etc/rc.common /etc/init.d/samba stop");
    } else if (!strcmp(value, "itunes")) {
        system("/etc/rc.common /etc/init.d/mt-daapd stop");    
    } else if (!strcmp(value, "iscsi")) {
        system("/etc/rc.common /etc/init.d/iscsi-target stop");
    } else {
        /* Undefined action. */
        websDebugWrite(stream, "Undefined type at %s %s<br>", __FUNCTION__, value);
    }
err:
    /* Flush all key-value pairs in tha hash table. */
    init_cgi(NULL);
}

static void
do_update_cgi(char *url, webs_t stream, ...)
{
    char *value, *num;
    char buf[TMP_LEN];
    int64_t map = 0;
    /* Find out the value of "type". */
    value = websGetVar(stream, "type", "");
    num = websGetVar(stream, "num", "");
    if (!*value) {
        /* No type! */
        websDebugWrite(stream, "Empty type at %s<br>", __FUNCTION__);
    } else if (!strcmp(value, "ntp")) {
        map |= ACTION_NTPCLIENT_START;
        snprintf(buf, TMP_LEN, "NUM=0");
        config_postaction(map, services, buf, "");
#if defined (EZP_PROD_BRAND_PROX)
	websWrite(stream, "message=Synchronize done.\n");
#else
	websWrite(stream, "message=Update successfully.\n");
#endif
	websWrite(stream, "return_value=-3\n");
        websDone(stream, 200);
    } else if (!strcmp(value, "ddns")) {
        map |= ACTION_UPDATEDD_START;
        snprintf(buf, TMP_LEN, "NUM=%s",num);
        config_postaction(map, services, buf, "");
#if defined (EZP_PROD_BRAND_PROX)
	websWrite(stream, "message=Synchronize done.\n");
#else
	websWrite(stream, "message=Update successfully.\n");
#endif
	websWrite(stream, "return_value=-3\n");
        websDone(stream, 200);
    } else if (!strcmp(value, "smbc")) {
        map |= ACTION_SMBC_START;
        snprintf(buf, TMP_LEN, "NUM=0");
        config_postaction(map, services, buf, "");
#if defined (EZP_PROD_BRAND_PROX)
	websWrite(stream, "message=Synchronize done.\n");
#else
	websWrite(stream, "message=Update successfully.\n");
#endif
	websWrite(stream, "return_value=-3\n");
        websDone(stream, 200);
    } else {
        /* Undefined action. */
        websDebugWrite(stream, "Undefined type at %s %s<br>", __FUNCTION__, value);
    }
    init_cgi(NULL);
}

static void
do_reboot_cgi(char *url, webs_t stream, ...)
{
    char lanip[TMP_LEN], port[TMP_LEN];
    ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", lanip,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("http_rule", 0, "port", port,
            TMP_LEN, EZPLIB_USE_CLI);
    /* XXX:System reboot. */
    websWrite(stream, "return_value=%s,%s:%s", EZP_RETURN_REBOOT, lanip, port); 
    /* Force to flush right away! */
    websDone(stream, 200);
#if defined(PLATFORM_AXA)
    system("ezp-i2c gauge host booting");
#endif
    system("/sbin/reboot");
}

static void
do_format_cgi(char *url, webs_t stream, ...)
{
    char *value, *submit_button;
    char buf[TMP_LEN];

    /* Find out the value of "type". */
    value = websGetVar(stream, "format_type", "");
    if (!*value) {
        /* No type! */
        websDebugWrite(stream, "Empty type at %s<br>", __FUNCTION__);
        return;
    } else if (strcmp(value, "vfat") && strcmp(value, "ext3")) {
        /* Undefined action. */
        websDebugWrite(stream, "Undefined type at %s %s<br>",
                __FUNCTION__, value);
        return;
    }
    snprintf(buf, sizeof(buf), "/sbin/format %s", value);
    system(buf);

    submit_button = websGetVar(stream, "submit_button", NULL); 
    sprintf(buf, "%s.asp", submit_button);

    /* TODO: error checking and handling are needed */

    do_ej(buf, stream, "");
    websDone(stream, 200);

    /* Flush all key-value pairs in tha hash table. */
    init_cgi(NULL);
}
void
do_date_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0};
    char *value, *date;
    FILE *file, *file2;
    char time[TMP_LEN];
    value = websGetVar(wp, "action", "");
    if (!strcmp(value, "Update")) {
        date = websGetVar(wp, "date", "");
        snprintf(time, 21, "date -s %s", date);
        if ((file2 = popen(time, "r"))) {
            fgets(tmp, sizeof(tmp), file2);
        }
        pclose(file2);
    }
    if ((file = popen("date '+%m%d%H%M%Y'", "r"))) {
        fgets(tmp, sizeof(tmp), file);
    }

    websWrite(wp, "%s", tmp);
    pclose(file);
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}
#ifdef EZP_SUB_BRAND_SONY 
#define USB_PATH "/sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.2/1-1.2:1.0/bInterfaceClass"
#define CARD_PATH "/sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.1/1-1.1:1.0/bInterfaceClass"
#else
#define CARD_PATH "/sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.2/1-1.2:1.0/bInterfaceClass"
#define USB_PATH "/sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.1/1-1.1:1.0/bInterfaceClass"
#endif
#define SD_RULE_IDX 1
#define USB_RULE_IDX 0
#define QSync_RULE_IDX 2
void
do_storage_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0}, tmp2[TMP_LEN]={0};
    char *value;
    char usb=0, card=0;
#if defined(PLATFORM_M2E)
    char qsync=0;
#endif
    value = websGetVar(wp, "action", "");
    if(!*value) {
        usb=1;card=1;
#if defined(PLATFORM_M2E)
        qsync=1;
#endif
    } else if (!strcmp(value, "usb")) {
        usb=1;
    } else if (!strcmp(value, "card")) {
        card=1;
#if defined(PLATFORM_M2E)
    } else if (!strcmp(value, "qsync")) {
        qsync=1;
#endif
    }
    if (usb) {
        ezplib_get_attr_val("storage_state_rule", USB_RULE_IDX, 
                "insert_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(tmp2[0] == '1') {
            snprintf(tmp, sizeof(tmp), "USB:08\n");
        } else {
            snprintf(tmp, sizeof(tmp), "USB:NA\n");
        }
        /*
        snprintf(tmp, sizeof(tmp), "USB:");
        if ((file = fopen(USB_PATH, "r"))) {
            fgets(tmp2, sizeof(tmp2), file);
        } else {
            snprintf(tmp2, sizeof(tmp2), "NA\n");
        }
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        */
        websWrite(wp, "%s", tmp);
    }

    if (card) {
        ezplib_get_attr_val("storage_state_rule", SD_RULE_IDX, 
                "insert_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(tmp2[0] == '1') {
            snprintf(tmp, sizeof(tmp), "CARD:08\n");
        } else {
            snprintf(tmp, sizeof(tmp), "CARD:NA\n");
        }
        /*
        snprintf(tmp, sizeof(tmp), "CARD:", tmp);
        if ((file = fopen(CARD_PATH, "r"))) {
            fgets(tmp2, sizeof(tmp2), file);
        } else {
            snprintf(tmp2, sizeof(tmp2), "NA\n");
        }
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        */
        websWrite(wp, "%s", tmp);
    }
#if defined(PLATFORM_M2E)
    if (qsync) {
        ezplib_get_attr_val("storage_state_rule", QSync_RULE_IDX, 
                "insert_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(tmp2[0] == '1') {
            snprintf(tmp, sizeof(tmp), "QSync:08\n");
        } else {
            snprintf(tmp, sizeof(tmp), "QSync:NA\n");
        }
        websWrite(wp, "%s", tmp);
    }
#endif
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}

typedef enum {
    ST_INFO_TOTAL = 2,
    ST_INFO_USED = 3,
    ST_INFO_FREE = 4,
} storage_info_enum;
/* Usage :
 * get_storage_info(char *STRING_TO_FILTER_MOUNT_PATH, storage_info_enum TYPE_OF_INFO_TO_GET, char * result_buffer);
 * example : get SD card free space ==> get_storage_info("SD", ST_INFO_FREE, buf);
 * The result will be stored in buf. If no match or no result, an empty string
 * will be applied
 */
void
get_storage_space_info(char *mount_string, storage_info_enum info_type, char *result_buffer, int buffer_size, int space_plus_ro) {
    char tmp3[TMP_LEN]={0}, tmp4[TMP_LEN]={0};
    FILE *file, *file2;
    snprintf(tmp3, sizeof(tmp3), "mount | grep %s", mount_string);
    if ((file2 = popen(tmp3, "r"))) {
        fgets(tmp4, sizeof(tmp4), file2);
    }
    pclose(file2);
    /* The df format is 
     * /dev/sdb1              3944740   3182596    762144 81% /tmp/mnt/USB 
     * $1                     $2:total  $3:used    $4:free
     * */
    if (strcmp(tmp4, "")) { 
        if (space_plus_ro || strstr(tmp4, "rw")) { 
            snprintf(tmp3, sizeof(tmp3), "df | grep %s | awk '{print $%d}'", mount_string, info_type);
            if ((file = popen(tmp3, "r"))) {
                fgets(result_buffer, buffer_size, file);
                if(space_plus_ro && (!strstr(tmp4, "rw"))) {
                    sprintf(result_buffer, "%s,RO", result_buffer);
                }
                pclose(file);
            } else {
                snprintf(result_buffer, buffer_size, "NA\n");
            }
        } else {
            snprintf(result_buffer, buffer_size, "RO\n");
        }
    } else {
        snprintf(result_buffer, buffer_size, "NA\n");
    }
}

void
do_storage_space_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0}, tmp2[TMP_LEN]={0};
    // char tmp3[TMP_LEN]={0}, tmp4[TMP_LEN]={0}, tmp5[TMP_LEN]={0};
    // FILE *file, *file2;
    char *value;
    char usb=0, card=0;
#if defined(PLATFORM_M2E)
    char qsync=0;
#endif
    value = websGetVar(wp, "action", "");
    if(!*value) {
        usb=1;card=1;
#if defined(PLATFORM_M2E)
        qsync=1;
#endif
    } else if (!strcmp(value, "usb")) {
        usb=1;
    } else if (!strcmp(value, "card")) {
        card=1;
#if defined(PLATFORM_M2E)
    } else if (!strcmp(value, "qsync")) {
        qsync=1;
#endif
    }
    if (usb) {
        snprintf(tmp, sizeof(tmp), "USB:");
        get_storage_space_info("USB", ST_INFO_FREE, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }

    if (card) {
        snprintf(tmp, sizeof(tmp), "CARD:");
        get_storage_space_info("SD", ST_INFO_FREE, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }

#if defined(PLATFORM_M2E)
    if (qsync) {
        snprintf(tmp, sizeof(tmp), "QSync:");
        get_storage_space_info("Qsync", ST_INFO_FREE, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }
#endif

    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}
void
do_storage_capacity_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0}, tmp2[TMP_LEN]={0};
    //char tmp3[TMP_LEN]={0}, tmp4[TMP_LEN]={0}, tmp5[TMP_LEN]={0};
    //FILE *file, *file2;
    char *value;
    char usb=0, card=0;
#if defined(PLATFORM_M2E)
    char qsync=0;
#endif
    value = websGetVar(wp, "action", "");
    if(!*value) {
        usb=1;card=1;
#if defined(PLATFORM_M2E)
        qsync=1;
#endif
    } else if (!strcmp(value, "usb")) {
        usb=1;
    } else if (!strcmp(value, "card")) {
        card=1;
#if defined(PLATFORM_M2E)
    } else if (!strcmp(value, "qsync")) {
        qsync=1;
#endif
    }
    if (usb) {
        snprintf(tmp, sizeof(tmp), "USB:");
        get_storage_space_info("USB", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }

    if (card) {
        snprintf(tmp, sizeof(tmp), "CARD:");
        get_storage_space_info("SD", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }

#if defined(PLATFORM_M2E)
    if (qsync) {
        snprintf(tmp, sizeof(tmp), "QSync:");
        get_storage_space_info("Qsync", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 0);
        snprintf(tmp, sizeof(tmp), "%s%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }
#endif
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}

void
do_storage_info_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0}, tmp2[TMP_LEN]={0};
    char *value;
    char usb=0, card=0;
#if defined(PLATFORM_M2E)
    char qsync=0;
#endif
    value = websGetVar(wp, "action", "");
    if(!*value) {
        usb=1;card=1;
#if defined(PLATFORM_M2E)
        qsync=1;
#endif
    } else if (!strcmp(value, "usb")) {
        usb=1;
    } else if (!strcmp(value, "card")) {
        card=1;
#if defined(PLATFORM_M2E)
    } else if (!strcmp(value, "qsync")) {
        qsync=1;
#endif
    }
    if (usb) {
        ezplib_get_attr_val("storage_state_rule", USB_RULE_IDX, 
                "mount_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(!strcmp("mounted", tmp2)) {
            get_storage_space_info("USB", ST_INFO_FREE, tmp2, sizeof(tmp2), 1);
            if((value = strstr(tmp2, ",RO"))) {
                snprintf(tmp, sizeof(tmp), "USB:RO\n");
                *value = 0;
            }
            else
                snprintf(tmp, sizeof(tmp), "USB:08\n");
            snprintf(tmp, sizeof(tmp), "%sUSB_FREE:%s\n", tmp, tmp2);
        } else {
            snprintf(tmp, sizeof(tmp), "USB:NA\n");
            snprintf(tmp, sizeof(tmp), "%sUSB_FREE:NA\n", tmp);
        }

        get_storage_space_info("USB", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 1);
        if((value = strstr(tmp2, ",RO"))) {
            *value = 0;
        }
        snprintf(tmp, sizeof(tmp), "%sUSB_TOTAL:%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }

    if (card) {
        ezplib_get_attr_val("storage_state_rule", SD_RULE_IDX, 
                "mount_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(!strcmp("mounted", tmp2)) {
            get_storage_space_info("SD", ST_INFO_FREE, tmp2, sizeof(tmp2), 1);
            if((value = strstr(tmp2, ",RO"))) {
                snprintf(tmp, sizeof(tmp), "CARD:RO\n");
                *value = 0;
            }
            else
                snprintf(tmp, sizeof(tmp), "CARD:08\n");
            snprintf(tmp, sizeof(tmp), "%sCARD_FREE:%s\n", tmp, tmp2);
        } else {
            snprintf(tmp, sizeof(tmp), "CARD:NA\n");
            snprintf(tmp, sizeof(tmp), "%sCARD_FREE:NA\n", tmp);
        }

        get_storage_space_info("SD", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 1);
        if((value = strstr(tmp2, ",RO"))) {
            *value = 0;
        }
        snprintf(tmp, sizeof(tmp), "%sCARD_TOTAL:%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }
#if defined(PLATFORM_M2E)
    if (qsync) {
        ezplib_get_attr_val("storage_state_rule", QSync_RULE_IDX, 
                "mount_state", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        if(!strcmp("mounted", tmp2)) {
            get_storage_space_info("Qsync", ST_INFO_FREE, tmp2, sizeof(tmp2), 1);
            if((value = strstr(tmp2, ",RO"))) {
                snprintf(tmp, sizeof(tmp), "QSync:RO");
                *value = 0;
            }
            else
                snprintf(tmp, sizeof(tmp), "QSync:08\n");
            snprintf(tmp, sizeof(tmp), "%sQSync_FREE:%s\n", tmp, tmp2);
        } else {
            snprintf(tmp, sizeof(tmp), "QSync:NA\n");
            snprintf(tmp, sizeof(tmp), "%sQSync_FREE:NA\n", tmp);
        }

        get_storage_space_info("Qsync", ST_INFO_TOTAL, tmp2, sizeof(tmp2), 1);
        if((value = strstr(tmp2, ",RO"))) {
            *value = 0;
        }
        snprintf(tmp, sizeof(tmp), "%sQSync_TOTAL:%s", tmp, tmp2);
        websWrite(wp, "%s", tmp);
    }
#endif
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}

void
do_charge_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    if ((file = popen("ezp-i2c charger", "r"))) {
        fgets(tmp, sizeof(tmp), file);
    } 
    if (!strncmp(tmp, "No charger", 10)) {
        websWrite(wp, "0");
    } else {
        websWrite(wp, "1");
    }

    pclose(file);
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}
void
do_voltage_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    system("/usr/sbin/ezp-i2c gauge voltage");
    if ((file = fopen("/tmp/gauge_voltage", "r"))) {
        fgets(tmp, sizeof(tmp), file);
        fclose(file);
    }
    websWrite(wp, "%s", tmp);
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}
void
do_percentage_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    system("/usr/sbin/ezp-i2c gauge percentage");
    if ((file = fopen("/tmp/gauge_percentage", "r"))) {
        fgets(tmp, sizeof(tmp), file);
        fclose(file);
    }
    websWrite(wp, "%s", tmp);
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}
void
do_gauge_state_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    system("/usr/sbin/ezp-i2c gauge state");
    if ((file = fopen("/tmp/gauge_state", "r"))) {
        fgets(tmp, sizeof(tmp), file);
        fclose(file);
    }
    websWrite(wp, "%s", tmp);
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}
void
do_mcu_fw_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    system("/usr/sbin/ezp-i2c fw");
    if ((file = fopen("/tmp/mcu_fw", "r"))) {
        fgets(tmp, sizeof(tmp), file);
        fclose(file);
    }
    websWrite(wp, "%s", tmp);
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}
void
do_gauge_fw_cgi(char *url, webs_t wp, ...)
{
    char tmp[TMP_LEN]={0};
    FILE *file;
    system("/usr/sbin/ezp-i2c gauge fw");
    if ((file = fopen("/tmp/gauge_fw", "r"))) {
        fgets(tmp, sizeof(tmp), file);
        fclose(file);
    }
    websWrite(wp, "%s", tmp);
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}
#if defined (PLATFORM_AXA)
void *cancel_copy_signaling(void* param)
{
    int         client_sockfd;
    int         server_len;
    struct sockaddr_un server_address;
    char buf[100] = {0};

    if ((client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
	    return 0;
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, COMMAND_SOCKET_PATH);

    server_len = strlen(server_address.sun_path) + sizeof(server_address.sun_family);

    if((connect(client_sockfd, (struct sockaddr*)&server_address, server_len)) < 0)
        return 0;

    if(send(client_sockfd, COMMAND_COPY_CANCEL, strlen(COMMAND_COPY_CANCEL), 0) < 0) 
        return 0;

    while(1)
    {
        if((recv(client_sockfd, buf, 100, 0)) < 0)
            continue;
        else
        {
            if(strcmp(buf, RET_COPY_SUCCESS) == 0)
              copy_success = 1;
            break;
        }
    }
    close(client_sockfd);
    return 0;
}

/* CGI for sending a umount command to SD card or USB storge */
void
do_eject_storage_cgi(char *url, webs_t wp, ...)
{
    char umount_command[128],buf[128];
    FILE *eject;
    char *device_id;

    memset(buf, 0, sizeof(buf));
    memset(umount_command, 0, sizeof(umount_command));
    device_id = websGetVar(wp, "device_id", ""); 
    if(device_id != NULL) {
        /* umount storage(USB/SD) */
        sprintf(umount_command,"umount -l /tmp/mnt/%s/", device_id);
        system(umount_command);

        /* check device is busy or not */
        memset(umount_command, 0, sizeof(umount_command));
        sprintf(umount_command,"mount | grep %s", device_id);
        if((eject = popen(umount_command, "r")) != NULL) {
            fgets(buf,sizeof(buf),eject);
            if(strstr(buf,device_id)) {
                websWrite(wp, "busy");  
            } else {
                websWrite(wp, "success");  
            }
            pclose(eject);
        }
    }
    init_cgi(NULL);
}


/* CGI for sending a cancel request to webdav */
void
do_copy_cancel_cgi(char *url, webs_t wp, ...)
{
	/* creat a pthread to open socket */
    pthread_t thread;
    pthread_create(&thread, NULL, cancel_copy_signaling, NULL);
    int i;
    for(i=0 ; i < 20 ; i++)
    {
        usleep(100000);
        if(copy_success == 1)
        {
            websWrite(wp, "success");
            copy_success = 0;
            break;
        }
    }
    if(i == 20)
        websWrite(wp, "fail");
    copy_success = 0;
    /* if pthread alive, kill it */
    if((pthread_kill(thread, 0)) == 0 )
        pthread_cancel(thread);
    init_cgi(NULL);
}
#endif
void
do_site_survey_cgi(char *url, webs_t wp, ...)
{
	char buf[20];
	memset(buf, 0, sizeof(buf));
	int i = 0;
	while(	(ezplib_get_attr_val("wl0_apcli_swap_rule", i, "ssid", buf, sizeof(buf), 0))  >= 0 )
	{
		websWrite(wp, "ssid : %s", buf);	
		websWrite(wp, "<br>");	
   		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("wl0_apcli_swap_rule", i, "bssid", buf, sizeof(buf), 0);
		websWrite(wp, "bssid : %s", buf);	
		websWrite(wp, "<br>");	
   		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("wl0_apcli_swap_rule", i, "secmode", buf, sizeof(buf), 0);
		websWrite(wp, "secmode : %s", buf);	
		websWrite(wp, "<br>");	
   		memset(buf, 0, sizeof(buf));
		ezplib_get_attr_val("wl0_apcli_swap_rule", i, "wifiwan", buf, sizeof(buf), 0);
		websWrite(wp, "wifiwan : %s", buf);	
		websWrite(wp, "<br>");	
		websWrite(wp, "<br>");	
		i++;
	}
   	init_cgi(NULL);

}
void
do_ui_survey_cgi(char *url, webs_t wp, ...)
{
    char *value;
    FILE *fp = NULL;
    char tmp[LONG_BUF_LEN] = {0};
    int i=0;

    system("/etc/wl/site_survey");
    value = websGetVar(wp, "action", "");
    if(!*value) {
        /* No action! */
        /* TODO:XXX: Send out "action required" message. */
    } else if (!strcmp(value, "Update")) {
        /* search wifi station */
        if((fp = fopen("/tmp/wifi_site_not_encode", "r"))) {
            websWrite(wp, "{\"success\": \"true\", \"msg\": \"");
            while(fgets(tmp, sizeof(tmp), fp)) {
                char ssid[TMP_LEN]={0},bssid[TMP_LEN]={0},sec[TMP_LEN]={0};
                char mode[TMP_LEN]={0},t1[TMP_LEN]={0},t2[TMP_LEN]={0};
                char sig[TMP_LEN] = {0};
                int ch=0, extch=0;
                sscanf(tmp, "%d$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]\n",
                        &ch, ssid, bssid, sec, sig, mode, t1, t2);
                /* filter special character  *;=|^ */
                if ((strstr(ssid, "*")!=NULL) || (strstr(ssid, ";")!=NULL) || (strstr(ssid, "=")!=NULL) || (strstr(ssid, "|")!=NULL) || (strstr(ssid, "^")!=NULL))
                    continue;
                if (ch == 0)
                    break;
                if ( !strcmp(ssid, "")) { // prevent error when SSID is null
                    sscanf(tmp, "%d$$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]\n",
                            &ch, bssid, sec, sig, mode, t1, t2);
                }
                if(i > 0)
                    websWrite(wp, "|");
                if (t2[0] == 0) {
                    /* this case is for empty ssid */
                    if(!strcmp(mode, "ABOVE")) { 
                        extch = 1;
                    } else if(!strcmp(mode, "BELOW")) {
                        extch = 0;
                    } else {
                        extch = 0;
                    }
                    websWrite(wp, "%d^%s^%s^%s^%s^%s",
                            ch, "", survey_parser(ssid), bssid, sec, sig, extch);
                } else {
                    if(!strcmp(t1, "ABOVE")) { 
                        extch = 1;
                    } else if(!strcmp(t1, "BELOW")) {
                        extch = 0;
                    } else {
                        extch = 0;
                    }
                    websWrite(wp, "%d^%s^%s^%s^%s^%s^%d",
                            ch, survey_parser(ssid), bssid, sec, sig, mode, extch);
                }
                t2[0] = 0;
                ch = 0;
                i++;
            }
            fclose(fp);
            websWrite(wp, "\"}");
        } else {
            websWrite2(wp, "{\"success\": \"false\", \"msg\": \"%s\"}", "Site Survey Fail");
        }
    }

    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}

void
do_survey_cgi(char *url, webs_t wp, ...)
{
    char *value;
    FILE *fp = NULL;
    char tmp[LONG_BUF_LEN] = {0};
    int i=0;

    system("/etc/wl/site_survey");
    value = websGetVar(wp, "action", "");
    if(!*value) {
        /* No action! */
        /* TODO:XXX: Send out "action required" message. */
    } else if (!strcmp(value, "Update")) {
        /* search wifi station */
        if((fp = fopen("/tmp/wifi_site_not_encode", "r"))) {
            websWrite(wp, "{\"success\": \"true\", \"msg\": \"");
            while(fgets(tmp, sizeof(tmp), fp)) {
                char ssid[TMP_LEN]={0},bssid[TMP_LEN]={0},sec[TMP_LEN]={0};
                char mode[TMP_LEN]={0},t1[TMP_LEN]={0},t2[TMP_LEN]={0};
                char sig[TMP_LEN] = {0};
                int ch=0, extch=0;
                sscanf(tmp, "%d$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]\n",
                        &ch, ssid, bssid, sec, sig, mode, t1, t2);

                /* filter special character  *;=|^ */
                if ((strstr(ssid, "*")!=NULL) || (strstr(ssid, ";")!=NULL) || (strstr(ssid, "=")!=NULL) || (strstr(ssid, "|")!=NULL) || (strstr(ssid, "^")!=NULL))
                    continue;
                if (ch == 0)
                    break;
                if ( !strcmp(ssid, "")) { // prevent error when SSID is null
                    sscanf(tmp, "%d$$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]\n",
                            &ch, bssid, sec, sig, mode, t1, t2);
                }
                if(i > 0)
                    websWrite(wp, "|");
                if (t2[0] == 0) {
                    /* this case is for empty ssid */
                    if(!strcmp(mode, "ABOVE")) {
                        extch = 1;
                    } else if(!strcmp(mode, "BELOW")) {
                        extch = 0;
                    } else {
                        extch = 0;
                    }
                    websWrite(wp, "%d^%s^%s^%s^%s^%s",
                            ch, "", ssid, bssid, sec, sig, extch);
                } else {
                    if(!strcmp(t1, "ABOVE")) {
                        extch = 1;
                    } else if(!strcmp(t1, "BELOW")) {
                        extch = 0;
                    } else {
                        extch = 0;
                    }
                    websWrite(wp, "%d^%s^%s^%s^%s^%s^%d",
                            ch, ssid, bssid, sec, sig, mode, extch);
                }
                t2[0] = 0;
                ch = 0;
                i++;
            }
            fclose(fp);
            websWrite(wp, "\"}");
        } else {
            websWrite2(wp, "{\"success\": \"false\", \"msg\": \"%s\"}", "Site Survey Fail");
        }
    }

    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}


static const char *station_list[] = {
    "http://122.116.172.47:8000",
    "http://210.70.173.116:7000",
    "http://38.96.148.141:4674",
    "http://38.96.148.141:4642",
    "http://38.96.148.141:4670",
    "http://203.150.224.142:8000",
    "http://83.170.76.32:8610"
};

void
do_radio_cgi(char *url, webs_t wp, ...)
{
    char *value, *stationid;
    char tmp[LONG_BUF_LEN];
    int ch;

    value = websGetVar(wp, "action", "");
    if(!*value) {
        /* No action! */
        /* TODO:XXX: Send out "action
         * required" message. */
    } else {
        stationid = websGetVar(wp, "radio_station", "");
        switch (value[0]) {
            case 'P':
                stationid = websGetVar(wp, "radio_station", "");
                ch = atoi(stationid);
                printf("Play station %d %s\n", ch, station_list[ch]);
                snprintf(tmp, sizeof(tmp), "wget %s -O - | madplay - &",
                        station_list[ch]);
                printf("Cmd %s\n", tmp);
                system(tmp);
                break;
            case 'S':
                system("killall madplay");
                break;
            case 'U':
                break;
            case 'D':
                break;
        }
    }
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}

static void
do_default_cgi(char *url, webs_t stream, ...)
{
    nvram_default();
    nvram_commit();
    char lanip[TMP_LEN], port[TMP_LEN];
    ezplib_get_attr_val("lan_static_rule_default", 0, "ipaddr", lanip,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("http_rule_default", 0, "port", port,
            TMP_LEN, EZPLIB_USE_CLI);
    /* XXX:System reboot. */
    websWrite(stream, "return_value=%s,%s:%s", EZP_RETURN_REBOOT, lanip, port); 
    /* Force to flush right away! */
    websDone(stream, 200);
#if defined(PLATFORM_AXA)
    system("ezp-i2c gauge host booting");
#endif
    system("/sbin/reboot");
}

char no_cache[] =
    "Cache-Control: no-cache\r\n" "Pragma: no-cache\r\n" "Expires: 0";
char max_age_one_day[] =
    "Cache-Control: max-age=86400";

struct mime_handler mime_handlers[] = {
    {"landing.asp", "text/html", no_cache, NULL, do_ej, NULL}, /* for redirect */
    {"st_info.asp", "text/html", no_cache, NULL, do_ej, NULL}, /* for factory */
    {"**.asp", "text/html", no_cache, NULL, do_ej, do_auth},
#if defined(PLATFORM_LIS)
    {"**.js", "text/javascript", no_cache, NULL, do_ej, NULL},
#elif defined(PLATFORM_M2E)
    {"common.js", "text/javascript", max_age_one_day, NULL, do_ej, NULL},
    {"log.js", "text/javascript", max_age_one_day, NULL, do_ej, NULL},
    {"table.js", "text/javascript", max_age_one_day, NULL, do_ej, NULL},
    {"**.js", "text/javascript", max_age_one_day, NULL, do_file, NULL},
#else
    {"**.js", "text/javascript", max_age_one_day, NULL, do_ej, NULL},
#endif
    {"**.css", "text/css", max_age_one_day, NULL, do_file, NULL},
    {"**.gif", "image/gif", max_age_one_day, NULL, do_file, NULL},
    {"**.jpg", "image/jpeg", max_age_one_day, NULL, do_file, NULL},
    {"**.png", "image/png", max_age_one_day, NULL, do_file, NULL},
    {"**.svg", "image/svg+xml", max_age_one_day, NULL, do_ej, NULL},
    {"**.ttf", "application/font-ttf", max_age_one_day, NULL, do_file, NULL},
    {"**.otf", "application/font-opentype",  max_age_one_day, NULL, do_file, NULL},
    {"**.woff", "application/font-woff",  max_age_one_day, NULL, do_file, NULL},
    {"**.eot", "application/vnd.ms-fontobject",  max_age_one_day, NULL, do_file, NULL},
    {"**.htc", "text/x-component", max_age_one_day, NULL, do_file, NULL},
    {"**.cfg", "application/octet-stream\r\nContent-Disposition: attachment; filename=\"config.cfg\"", no_cache, NULL, do_export, do_auth},
    {"apply.cgi*", "text/html", no_cache, do_apply_post, do_apply_cgi, do_auth},
    /* do_apply_post is needed for some initialization reason. */
    {"reboot.cgi*", "text/html", no_cache, do_apply_post, do_reboot_cgi, do_auth},
    {"default.cgi*", "text/html", no_cache, do_apply_post, do_default_cgi, do_auth},
    {"connect.cgi*", "text/html", no_cache, do_apply_post, do_connect_cgi, 
        do_auth},
    {"disconnect.cgi*", "text/html", no_cache, do_apply_post, do_disconnect_cgi, do_auth},
    {"update.cgi*", "text/html", no_cache, do_apply_post, do_update_cgi, 
        do_auth},
    {"upgrade.cgi*", "text/html", no_cache, do_upgrade_post, do_upgrade_cgi,
        do_auth},
    {"bt_upload.cgi*", "text/html", no_cache, do_bt_upload_post,
        do_bt_upload_cgi, do_auth},
    {"bt.cgi*", "text/html", no_cache, do_apply_post, do_bt_cgi, do_auth},
    {"format.cgi*", "text/html", no_cache, do_apply_post, do_format_cgi,
        do_auth},
    {"ad_tools.cgi*", "text/html", no_cache, do_apply_post, do_ad_tools_cgi,
        do_auth},
    {"at_tools.cgi*", "text/html", no_cache, do_apply_post, do_at_tools_cgi,
        do_auth},
    {"wps.cgi*", "text/html", no_cache, do_apply_post, do_wps_cgi, do_auth},
    {"log.cgi*", "text/html", no_cache, do_apply_post, do_log_cgi, do_auth},
    {"survey.cgi*", "text/html", no_cache, do_apply_post, do_survey_cgi, do_auth},
    {"ui_survey.cgi*", "text/html", no_cache, do_apply_post, do_ui_survey_cgi, do_auth},
    {"site_survey.cgi*", "text/html", no_cache, do_apply_post, do_site_survey_cgi, NULL},
#if defined (PLATFORM_AXA)
    {"copy_cancel.cgi*", "text/html", no_cache, do_apply_post, do_copy_cancel_cgi, NULL},
    {"eject_storage.cgi*", "text/html", no_cache, do_apply_post, do_eject_storage_cgi, NULL},
#endif
    {"date.cgi*", "text/html", no_cache, do_apply_post, do_date_cgi, do_auth},
    {"storage.cgi*", "text/html", no_cache, do_apply_post, do_storage_cgi, do_auth},
    {"storage_space.cgi*", "text/html", no_cache, do_apply_post, do_storage_space_cgi, do_auth},
    {"storage_capacity.cgi*", "text/html", no_cache, do_apply_post, do_storage_capacity_cgi, do_auth},
    {"storage_info.cgi*", "text/html", no_cache, do_apply_post, do_storage_info_cgi, do_auth},
    {"charge.cgi*", "text/html", no_cache, do_apply_post, do_charge_cgi, do_auth},
    {"voltage.cgi*", "text/html", no_cache, do_apply_post, do_voltage_cgi, do_auth},
    {"percentage.cgi*", "text/html", no_cache, do_apply_post, do_percentage_cgi, do_auth},
    {"gauge_state.cgi*", "text/html", no_cache, do_apply_post, do_gauge_state_cgi, do_auth},
    {"gauge_fw.cgi*", "text/html", no_cache, do_apply_post, do_gauge_fw_cgi, do_auth},
    {"mcu_fw.cgi*", "text/html", no_cache, do_apply_post, do_mcu_fw_cgi, do_auth},
    {"get_mac.cgi*", "text/html", no_cache, do_apply_post, do_get_mac_cgi, do_auth},
    {NULL, NULL, NULL, NULL, NULL, NULL}
};

struct menu_item
{
    char *name;
    char *filename;
    int prod_cat_bitmap;
    int prod_subcat_low;
    int prod_subcat_high;
};
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
struct main_menu_item
{
    char *name;
    char *icon;
    struct menu_item *subp;
    int prod_cat_bitmap;
    int prod_subcat_low;
    int prod_subcat_high;
    int permission_class;
};
#else
struct main_menu_item
{
    char *name;
    struct menu_item *subp;
    int prod_cat_bitmap;
    int prod_subcat_low;
    int prod_subcat_high;
};
#endif

#define PROD_CAT_ALL               (PROD_CAT_B|PROD_CAT_C|PROD_CAT_D|PROD_CAT_L|PROD_CAT_M|PROD_CAT_N|PROD_CAT_P|PROD_CAT_O|PROD_CAT_W|PROD_CAT_T|PROD_CAT_V|PROD_CAT_U|PROD_CAT_G|PROD_CAT_I|PROD_CAT_A)
#define PROD_CAT_WL                (PROD_CAT_B|PROD_CAT_C|PROD_CAT_D|PROD_CAT_L|PROD_CAT_M|PROD_CAT_N|PROD_CAT_P|PROD_CAT_O|PROD_CAT_W|PROD_CAT_V|PROD_CAT_U|PROD_CAT_G|PROD_CAT_I)
#define PROD_CAT_LB                (PROD_CAT_D|PROD_CAT_L|PROD_CAT_N|PROD_CAT_O|PROD_CAT_A|PROD_CAT_I)
#define PROD_CAT_STORAGE           (PROD_CAT_C|PROD_CAT_D|PROD_CAT_T|PROD_CAT_K|PROD_CAT_G|PROD_CAT_P)
#define PROD_CAT_STORAGE_LOW       (PROD_CAT_C|PROD_CAT_D|PROD_CAT_T|PROD_CAT_K|PROD_CAT_P)
#define PROD_CAT_MYCLOUDWIFI          (PROD_CAT_P|PROD_CAT_U|PROD_CAT_G|PROD_CAT_I)
#define PROD_CAT_WWAN              (PROD_CAT_M|PROD_CAT_N|PROD_CAT_P|PROD_CAT_W|PROD_CAT_U|PROD_CAT_I|PROD_CAT_A)
#define PROD_CAT_WWAN_SINGLE       (PROD_CAT_M|PROD_CAT_P|PROD_CAT_W|PROD_CAT_U|PROD_CAT_I)
#define PROD_CAT_PPTP              (PROD_CAT_C|PROD_CAT_L|PROD_CAT_N|PROD_CAT_A)
#define PROD_CAT_PPTP_MR           (PROD_CAT_M)
#define PROD_CAT_PPTP_X            (PROD_CAT_G|PROD_CAT_I)
#define PROD_CAT_PPTP_VR           (PROD_CAT_O)
#define PROD_CAT_IPSEC             (PROD_CAT_L|PROD_CAT_N|PROD_CAT_A)
#define PROD_CAT_IPSEC_MR          (PROD_CAT_M)
#define PROD_CAT_IPSEC_PROX        (PROD_CAT_I)
#define PROD_CAT_IPSEC_VR          (PROD_CAT_O)
#define PROD_CAT_VLAN              (PROD_CAT_V|PROD_CAT_N|PROD_CAT_A)
#define PROD_CAT_VLAN_PROX         (PROD_CAT_I)
#define PROD_CAT_NAS               (PROD_CAT_ALL | PROD_CAT_K)
#define PROD_CAT_NAS_AND_WL        (PROD_CAT_WL | PROD_CAT_K)
#define PROD_CAT_NAS_AND_ROUTER    (PROD_CAT_ALL | PROD_CAT_K)
#define PROD_CAT_RT_VR             PROD_CAT_O

#ifdef EZP_SUB_BRAND_SONY
struct main_menu_item main_menu[] = {
    {"Security", "wl_basic.asp", PROD_CAT_R, 0, SUBCAT_MAX},
    {"Internet Connection", "exp_wan.asp", PROD_CAT_R, 0, SUBCAT_MAX},
    {"Power Management", "power.asp", PROD_CAT_R, 0, SUBCAT_MAX},
    {"System", "ad_basic.asp", PROD_CAT_R, 0, SUBCAT_MAX},
    //{"Logout", NULL, PROD_CAT_R, 0, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};

#elif defined(PLATFORM_AXA)
struct main_menu_item main_menu[] = {
    {"Dashboard", "icon-dashboard", (struct menu_item *)"exp_dashboard.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, GUEST},
    {"Setting","icon-list" , (struct menu_item *)"exp_setting.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"Internet","icon-globe" , (struct menu_item *)"exp_internet.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"Web Folder","icon-folder-open-alt" , (struct menu_item *)"webdav_landing.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, GUEST},
   // {"Logout", "icon-list",NULL, PROD_CAT_ALL, 0, SUBCAT_MAX},
    {NULL,NULL, NULL, 0, 0, 0}
};
#elif defined(PLATFORM_M2E)
struct main_menu_item main_menu[] = {
    {"Dashboard", "icon-dashboard", (struct menu_item *)"exp_dashboard.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, GUEST},
    {"Setting","icon-list" , (struct menu_item *)"exp_setting.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"Internet","icon-globe" , (struct menu_item *)"exp_internet.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"myQNAPcloud","icon-cloud" , (struct menu_item *)"exp_myQNAPcloud.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"File Station Lite","icon-folder-open-alt" , (struct menu_item *)"webdav_landing.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, GUEST},
   // {"Logout", "icon-list",NULL, PROD_CAT_ALL, 0, SUBCAT_MAX},
    {NULL,NULL, NULL, 0, 0, 0}
};
#elif defined(PLATFORM_AXM)
struct main_menu_item main_menu[] = {
    {"Dashboard", "icon-dashboard", (struct menu_item *)"exp_dashboard.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, GUEST},
    {"Setting","icon-list" , (struct menu_item *)"exp_setting.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
    {"Internet","icon-globe" , (struct menu_item *)"exp_internet.asp", PROD_CAT_P|PROD_CAR_ONE_PAGE_SUBMENU, 0, SUBCAT_MAX, FULL_ALLOW},
   // {"Logout", "icon-list",NULL, PROD_CAT_ALL, 0, SUBCAT_MAX},
    {NULL,NULL, NULL, 0, 0, 0}
};
#else /* EZP_SUB_BRAND_SONY */
struct menu_item sub_menu_setup[] = {
    {"WAN", "wan.asp", PROD_CAT_NAS , 0, SUBCAT_MAX},
#ifdef EZP_PROD_BRAND_ZYUS
    {"WAN Failover", "lb_basic.asp", PROD_CAT_LB, 2, SUBCAT_MAX},
#else
    {"WAN Advanced", "lb_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#endif
#ifdef EZP_SUB_BRAND_GARMIN
    {"WAN Priority", "wan_priority.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#endif
    {"LAN", "lan.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
#ifndef EZP_PROD_BRAND_ZYUS
    {"Routing", "lb_routing.asp", PROD_CAT_RT_VR, 0, 1},
    {"Routing", "lb_routing.asp", PROD_CAT_ALL, 2, SUBCAT_MAX},
#endif
#ifdef EZP_SUB_BRAND_GARMIN
    {"DDNS", "ddns.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
#else
    {"DHCP Server", "dhcp.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
    {"DDNS", "ddns.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
#ifdef AXIMDDNS
    {"AXIMDDNS", "aximddns.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
#endif
#endif
#ifndef EZP_PROD_BRAND_ZYUS
    {"MAC Address Clone", "clone.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
    {"VLAN", "vlan.asp", PROD_CAT_VLAN, 0, SUBCAT_MAX},
#endif
#ifdef EZP_PROD_BRAND_PROX   
    {"VLAN", "vlan.asp", PROD_CAT_VLAN_PROX, 0, SUBCAT_MAX},
#endif
    {"Time", "ntp.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_wl[] = {
    {"Basic", "wl_basic.asp", PROD_CAT_NAS_AND_WL, 0, SUBCAT_MAX},
    {"Advanced", "wl_adv.asp", PROD_CAT_NAS_AND_WL, 0, SUBCAT_MAX},
#ifndef EZP_PROD_BRAND_ZYUS
    {"WDS", "wl_wds.asp", PROD_CAT_WL, 0, SUBCAT_MAX},
#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_FV13XX)
    /* XXX: Enable apcli for Ralink. */
    {"Universal Repeater", "wl_apcli.asp", PROD_CAT_WL, 0, SUBCAT_MAX},
#endif
#endif
    {"WPS", "wl_wps.asp", PROD_CAT_WL, 0, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_wizard[] = {
    {"Setup Wizard", "wizard.asp", PROD_CAT_ALL , 0, SUBCAT_MAX},
    {"VPN Wizard", "vpn_wizard.asp", PROD_CAT_ALL , 0, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_fw[] = {
    {"Firewall", "fw_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#if defined (EZP_PROD_BRAND_PROX)
    {"IP Access Control", "fl_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
    {"Outbound MAC ACL", "fl_hwaddr.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#else
    {"Access Control", "fl_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
    {"MAC Access Control", "fl_hwaddr.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#endif
    {"OpenDNS", "opendns.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},  
    {"Web Filtering", "wf_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
    {"VPN / PPTP", "vpn_pptpd.asp", PROD_CAT_PPTP, 3, SUBCAT_MAX},
    {"VPN / PPTP", "vpn_pptpd.asp", PROD_CAT_PPTP_MR, 2, SUBCAT_MAX},
    {"VPN / PPTP", "vpn_pptpd.asp", PROD_CAT_PPTP_X, 2, SUBCAT_MAX},
    {"VPN / PPTP", "vpn_pptpd.asp", PROD_CAT_PPTP_VR, 0, SUBCAT_MAX},
#ifndef EZP_PROD_BRAND_ZYUS
    {"VPN / L2TP", "vpn_l2tpd.asp", PROD_CAT_IPSEC, 3, SUBCAT_MAX},
#endif
    {"VPN / IPsec", "vpn_ipsec.asp", PROD_CAT_IPSEC, 3, SUBCAT_MAX},
    {"VPN / IPsec", "vpn_ipsec.asp", PROD_CAT_IPSEC_MR, 2, SUBCAT_MAX},
#ifdef EZP_PROD_BRAND_PROX
    {"VPN / IPsec", "vpn_ipsec.asp", PROD_CAT_IPSEC_PROX, 2, SUBCAT_MAX},
#endif
    {"VPN / IPsec", "vpn_ipsec.asp", PROD_CAT_IPSEC_VR, 0, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_fl[] = {
#ifdef EZP_PROD_BRAND_ZYUS
    {"DBM", "bw_basic.asp", PROD_CAT_ALL, 2, SUBCAT_MAX},
#elif defined EZP_PROD_BRAND_PLANET
    {"QoS", "bw_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#else
    {"iDBM", "bw_basic.asp", PROD_CAT_ALL, 2, SUBCAT_MAX},
#endif
#ifndef EZP_PROD_BRAND_PLANET
    {"Throughput Optimizer", "bw_tos.asp", PROD_CAT_ALL, 2, SUBCAT_MAX},
#ifdef EZP_PROD_BRAND_ZYUS
    {"Hardware Accelerator", "hwnat.asp", PROD_CAT_ALL, 2, SUBCAT_MAX},
#else
    {"TurboNAT", "turbonat.asp", PROD_CAT_ALL, 2, SUBCAT_MAX},
#endif
    {"TurboNAT", "turbonat.asp", PROD_CAT_U, 0, SUBCAT_MAX},
    {"Session Manager", "bw_sm.asp", PROD_CAT_ALL, 2, SUBCAT_MAX},
#endif
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_fr[] = {
    {"Port Range Forward", "fr_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#ifdef EZP_PROD_BRAND_ZYUS
    {"1-1 NAT", "vs_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#else
    {"Virtual Hosts", "vs_basic.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#endif
    {"Streaming / VPN", "fr_nat_pass.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
    {"UPnP", "fr_upnp.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#if !defined(EZP_PROD_BRAND_ZYUS) && !defined(EZP_PROD_BRAND_PLANET) && !defined(EZP_PROD_BRAND_PROX)
    {"VNC KVM", "fr_vnc.asp", PROD_CAT_ALL, 3, SUBCAT_MAX},
#endif    
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_mycloudwifi[] = {
#if defined(EZP_DRIVE)
    {"Drive", "drive.asp", PROD_CAT_MYCLOUDWIFI, 0, SUBCAT_MAX},
#endif
#if defined(EZP_CAMERA)
    {"Camera", "camera.asp", PROD_CAT_MYCLOUDWIFI, 0, SUBCAT_MAX},
#endif    
#if defined(EZP_MUSIC)
    {"Music", "music.asp", PROD_CAT_MYCLOUDWIFI, 0, SUBCAT_MAX},
#endif    
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_usb[] = {
    {"Storage Device", "ad_storage.asp", PROD_CAT_STORAGE_LOW, 0, 2},
    {"Storage Device", "ad_storage.asp", PROD_CAT_STORAGE, 3, SUBCAT_MAX},
    {"BT Download", "bt.asp", PROD_CAT_STORAGE_LOW, 0, 2},
    {"BT Download", "bt.asp", PROD_CAT_STORAGE, 3, SUBCAT_MAX},
    {"FTP Server", "ftp.asp", PROD_CAT_STORAGE, 0, SUBCAT_MAX},
    {"iSCSI", "iscsi.asp", PROD_CAT_STORAGE, 3, SUBCAT_MAX},
    {"Samba Server", "smb.asp", PROD_CAT_STORAGE, 0, SUBCAT_MAX},
    {"iTunes Server", "itunes.asp", PROD_CAT_STORAGE_LOW, 0, 2},
    {"iTunes Server", "itunes.asp", PROD_CAT_STORAGE, 3, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_ad[] = {
    {"Management", "ad_basic.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
#if defined(EZP_SNMPD)
    {"SNMP", "snmp.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
#endif    
    {"System Utilities", "ad_tools.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
    {"Log", "st_log.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};

struct menu_item sub_menu_st[] = {
    {"Router", "st_basic.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
#ifndef EZP_PROD_BRAND_PLANET
    {"Traffic", "st_thp.asp", PROD_CAT_ALL, 2, SUBCAT_MAX},
    {"Session", "st_ct.asp", PROD_CAT_NAS, 2, SUBCAT_MAX},
#endif
    {"User / DHCP", "st_user_dhcp.asp", PROD_CAT_ALL, 0, SUBCAT_MAX},
    {"User / Current", "st_user_arp.asp", PROD_CAT_NAS, 0, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};

struct main_menu_item main_menu[] = {
#ifdef EZP_PROD_BRAND_ZYUS
    {"Wizard", &sub_menu_wizard[0], PROD_CAT_ALL, 0, SUBCAT_MAX},
#endif
    {"Status", &sub_menu_st[0], PROD_CAT_NAS, 0, SUBCAT_MAX},
    {"Setup", &sub_menu_setup[0], PROD_CAT_NAS, 0, SUBCAT_MAX},
    {"Wireless", &sub_menu_wl[0], PROD_CAT_NAS_AND_WL, 0, SUBCAT_MAX},
    {"Security", &sub_menu_fw[0], PROD_CAT_ALL, 0, SUBCAT_MAX},
#ifdef EZP_PROD_BRAND_PLANET
    {"App", &sub_menu_fr[0], PROD_CAT_ALL, 0, SUBCAT_MAX},
    {"BWM", &sub_menu_fl[0], PROD_CAT_ALL, 0, SUBCAT_MAX},
#else
    {"Applications", &sub_menu_fr[0], PROD_CAT_ALL, 0, SUBCAT_MAX},
    {"Bandwidth", &sub_menu_fl[0], PROD_CAT_ALL, 2, SUBCAT_MAX},
#endif
    {"Bandwidth", &sub_menu_fl[0], PROD_CAT_U, 0, SUBCAT_MAX},
#if !defined(EZP_PROD_BRAND_ZYUS) && !defined(EZP_PROD_BRAND_PLANET) && !defined(EZP_PROD_BRAND_PROX)
    {"Storage", &sub_menu_usb[0], PROD_CAT_STORAGE, 0, SUBCAT_MAX},
#endif
#if defined(EZP_DRIVE) || defined(EZP_CAMERA) || defined(EZP_MUSIC)
    {"myCLOUDwifi", &sub_menu_mycloudwifi[0], PROD_CAT_MYCLOUDWIFI, 0, SUBCAT_MAX},
#endif
    {"Admin", &sub_menu_ad[0], PROD_CAT_NAS, 0, SUBCAT_MAX},
    {"Logout", NULL, PROD_CAT_ALL, 0, SUBCAT_MAX},
    {NULL, NULL, 0, 0}
};
#endif

static int check_main_menu_disabled(struct main_menu_item* item) {
    char *supercat;
    supercat = nvram_safe_get("prod_supercat");
#if !defined(EZP_SUB_BRAND_SONY) && !defined(PLATFORM_AXA) && !defined(PLATFORM_AXM) && !defined(PLATFORM_M2E)
    /* If we enabled wlan in appmgr, this means the wireless menu is enabled. */
    if ((item->subp == &sub_menu_wl[0]) && appmgr_check("wlan")) {
        return 0;
    }
#endif
    /* If the menu is not activated in this product sub/category */
    if (!(item->prod_cat_bitmap & PROD_CAT_BITMAP) ||
        (item->prod_subcat_low > ezp_prod_subcat) ||
        (item->prod_subcat_high < ezp_prod_subcat)) {
        return 1;
    }
    /* For T2L, we enabled large SBM items but disabled storage function */
    if ((item->prod_cat_bitmap == PROD_CAT_STORAGE) &&
        !strcmp(ezp_prod_subsubcat, "L") &&
        !strcmp(ezp_prod_cat, "T")) {
        return 1;
    }
    /* For X-101N */
    if ((item->prod_cat_bitmap == PROD_CAT_MYCLOUDWIFI) &&
        !strcmp(supercat, "X") && (ezp_prod_subcat == 0)) {
        return 1;
    }
    return 0;
}

static int check_main_menu_non_permission(struct main_menu_item* item) {
    if(item->permission_class  <= access_permission)
        return 0;
    return 1;
}


static int check_menu_disabled(struct menu_item* item) {
    /* If we enabled wlan in appmgr, this means the wireless menu is enabled. */
    if (!strncmp(item->filename,"wl_",sizeof("wl_") - 1) && appmgr_check("wlan")) {
        return 0;
    }
    /* If the menu is not activated in this product sub/category */
    if (!(item->prod_cat_bitmap & PROD_CAT_BITMAP) ||
        (item->prod_subcat_low > ezp_prod_subcat) ||
        (item->prod_subcat_high < ezp_prod_subcat)) {
        return 1;
    }
    return 0;
}

/*
 * XXX: Requires some fixes. Stupid Europeans wrote stupid code!
 */
int
ej_get_ezp_machine_id(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *pFile;
    char mac[254]={0};
    int i;
    if ((pFile = popen("cat /proc/ezp_machine_id", "r"))) {
        fgets(mac, sizeof(mac), pFile);
    }
    pclose(pFile);

    for(i=0;i<strlen(mac);i++) {
        if(mac[i]=='\n') 
            mac[i] = '\0';
    }
    websWrite(wp, "%s", mac);
    return 0;
}

int
ej_do_menu(int eid, webs_t wp, int argc, char_t ** argv)
{
    int i;
    for (i = 0; main_menu[i].name; i++) {
        if (check_main_menu_disabled(&(main_menu[i]))) {
            continue;
        }
        if (!strcmp(main_menu[i].name, "Logout")) {
            websWrite(wp, "<li class='top'>\n");
            websWrite(wp, "<a class='menu-item' href='#' onclick='logout()'>\n");
            websWrite(wp, "        <span>%s</span></a>\n",
                    lang_translate(main_menu[i].name));
            websWrite(wp, "    </li>\n");
            continue;
        }

        
        websWrite(wp, "    <li class='top'>\n");
#if defined (EZP_PROD_BRAND_PROX)
        /* websWrite(wp, "   <a class='menu-item' href='%s'>\n",main_menu[i].subp->filename); */
        websWrite(wp, "      <a class='menu-item' href='#'>\n");
#elif defined (EZP_SUB_BRAND_SONY) || defined (PLATFORM_AXA) || defined (PLATFORM_AXM) || defined (PLATFORM_M2E) 
        websWrite(wp, "      <a id='menu-item%i' class='menu-item' href='%s'>\n",i,main_menu[i].subp);
#else
        websWrite(wp, "      <a class='menu-item' href='#'>\n");
#endif
        websWrite(wp, "        <span>%s</span></a>\n", 
                            lang_translate(main_menu[i].name));

#if !defined(EZP_SUB_BRAND_SONY) && !defined (PLATFORM_AXA) && !defined (PLATFORM_AXM) && !defined (PLATFORM_M2E)
        websWrite(wp, "<!--[if gte IE 7]><!--></a><!--<![endif]-->\n");
        websWrite(wp, "<!--[if lte IE 6]>\n");
        websWrite(wp, "<table>\n");
        websWrite(wp, "  <tbody>\n");
        websWrite(wp, "    <tr>\n");
        websWrite(wp, "      <td>\n");
        websWrite(wp, "        <iframe class=one src='about:blank'></iframe>");
        websWrite(wp, "<![endif]-->\n");

        /* Submenu */
#if defined (EZP_PROD_BRAND_PROX)
        websWrite(wp, "      <ul name='sub' class='sub'>\n");
#else
        websWrite(wp, "      <ul class='sub'>\n");
#endif
        for (subp = (main_menu[i].subp); subp->name; subp++) {
            if (check_menu_disabled(subp)) {
                continue;
            }
            websWrite(wp, "          <li><a href='%s'>", subp->filename);
            websWrite(wp, "<div class='menu-subitem'>%s</div>", 
                            lang_translate(subp->name));
            websWrite(wp, "</a></li>\n");
        }
        websWrite(wp, "      </ul>\n");

        websWrite(wp, "<!--[if lte IE 6]>\n");
        websWrite(wp, "      </td>\n");
        websWrite(wp, "    </tr>\n");
        websWrite(wp, "  </tbody>\n");
        websWrite(wp, "</table>\n");
        websWrite(wp, "</a>\n");
        websWrite(wp, "<![endif]-->\n");
#endif

        websWrite(wp, "    </li>\n");
    }

    return 0;
}

int
ej_do_pagehead1(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp,
              "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' '"
              "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n");
    return 0;
}

int
ej_do_pagehead2(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *no_table;

    /* It's allowed not to feed a parameter. */
    ejArgs(argc, argv, "%s", &no_table);

    websWrite(wp,
              "<meta http-equiv='Content-Type' content='"
              "application/xhtml+xml' />\n");
    websWrite(wp,
              "<link type='text/css' rel='stylesheet' href='"
              "style/style.css' />\n");
    if (!no_table || strcmp(no_table, "no_table")) {
        websWrite(wp,
                  "<link type='text/css' rel='stylesheet' href='"
                  "style/table.css' />\n");
    }

    websWrite(wp, "<link rel='stylesheet' type='text/css' media='screen' href='ext/css/themes/jquery-ui-custom.min.css' />\n");
    websWrite(wp, "<link rel='stylesheet' type='text/css' media='screen' href='ext/css/ui.jqgrid.css' />\n");
    websWrite(wp, "<link rel='stylesheet' type='text/css' href='ext/css/jquery.jqplot.css' />\n");
    websWrite(wp, "<link rel='stylesheet' type='text/css' href='ext/css/zebra_dialog.css'>\n");

    websWrite(wp,
              "<!--[if lte IE 6]>\n"
              "<style type='text/css'>\n"
              "     #logo { behavior: url(iepngfix.htc); } \n"
              "</style>\n"
              "<link type='text/css' rel='stylesheet' href='"
              "style/ie6.css' />\n"
              "<![endif]-->\n");

    websWrite(wp, "<script type='text/javascript' src='common.js'></script>\n");
    websWrite(wp, "<script type='text/javascript' src='code.js'></script>\n");
    websWrite(wp, "<script type='text/javascript' src='svg-common.js'></script>\n");

    websWrite(wp, "<!--[if lt IE 9]><script language='javascript' type='text/javascript' src='ext/js/jqPlot/excanvas.js'></script><![endif]-->\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/jquery-min.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/jquery-ui-custom.min.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/date.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/jqGrid/i18n/grid.locale-en.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/jqGrid/jquery.jqGrid.min.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/jqPlot/jquery.jqplot.min.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.highlighter.min.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.cursor.min.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.dateAxisRenderer.min.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/zebra_dialog.js'></script>\n");
    websWrite(wp, "<script language='javascript' type='text/javascript' src='ext/js/ajaxfileupload.js'></script>\n");

#if defined (EZP_PROD_BRAND_PROX)
    websWrite(wp, "<script type='text/javascript' src='jquery-1.7.min.js'></script>\n");
#endif
    return 0;
}
int
ej_do_bootstrap_menu(int eid, webs_t wp, int argc, char_t ** argv)
{
    int i;
    struct menu_item *subp;
    char dev_name_sd[TMP_LEN] = "";
    char dev_name_usb[TMP_LEN] = "";
    char dev_name_qsync[TMP_LEN] = "";
    
    ezplib_get_attr_val("storage_state_rule", 0, "mount_state", dev_name_usb, TMP_LEN, EZPLIB_USE_CLI);  
    ezplib_get_attr_val("storage_state_rule", 1, "mount_state", dev_name_sd, TMP_LEN, EZPLIB_USE_CLI);  
    ezplib_get_attr_val("storage_state_rule", 2, "mount_state", dev_name_qsync, TMP_LEN, EZPLIB_USE_CLI);  
  
    websWrite(wp, "<div class='nav nav-list'>\n");

    for (i = 0; main_menu[i].name; i++) {
        if (check_main_menu_disabled(&(main_menu[i]))) {
            continue;
        }
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
        if (check_main_menu_non_permission(&(main_menu[i]))) {
            continue;
        }
#endif
        if (!strcmp(main_menu[i].name, "Logout")) {
            websWrite(wp, "<li><a href='#' onclick='logout()'>%s</a></li>\n", lang_translate(main_menu[i].name));
            continue;
        }
        if(!(main_menu[i].prod_cat_bitmap & PROD_CAR_ONE_PAGE_SUBMENU)) {
            websWrite(wp, "<li class='dropdown'>\n");
            websWrite(wp, " <a href='#' class='dropdown-toggle' data-toggle='dropdown'>%s <b class='caret'></b></a>\n", lang_translate(main_menu[i].name));
            websWrite(wp, " <ul class='dropdown-menu'>\n");

            /* Submenu */
            for (subp = (main_menu[i].subp); subp->name; subp++) {
                if (check_menu_disabled(subp)) {
                    continue;
                }
                websWrite(wp, "<li><a href='%s'>", subp->filename);
                websWrite(wp, "%s</a></li>\n", lang_translate(subp->name));
            }
            websWrite(wp, " </ul>\n");
            websWrite(wp, "</li>\n");
        } else {
            // websWrite(wp, "<li class='active'>\n");
            if(i==4) {
                if (!strcmp(dev_name_usb, "mounted") || !strcmp(dev_name_sd, "mounted") || !strcmp(dev_name_qsync, "mounted")){
                     websWrite(wp, "<li id='%d'>\n", i);
                } else {
                     websWrite(wp, "<li id='%d' style='display:none;'>\n", i);
                }
            } else {
                websWrite(wp, "<li id='%d'>\n", i);
            }
            websWrite(wp, "<a href='%s'>\n", main_menu[i].subp);
            websWrite(wp, "<i class='%s'></i> \n", main_menu[i].icon);
            websWrite(wp, "<span class='menu-text'>%s</span></a>\n", lang_translate(main_menu[i].name));
            websWrite(wp, "</li>\n");
            

        }

    }


    websWrite(wp, "</div>\n");

    return 0;
}
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
int
ej_do_bootstrap_navbar_menu(int eid, webs_t wp, int argc, char_t ** argv)
{
#if defined(PLATFORM_M2E)
websWrite(wp, "<div class='navbar'>\n");
#else
websWrite(wp, "<div class='navbar navbar-inverse'>\n");
#endif
websWrite(wp, " <div class='navbar-inner'>\n");
websWrite(wp, "     <div class='container-fluid'>\n");

#if defined(PLATFORM_M2E)
websWrite(wp, "            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src='picts/logo.png' style='margin-top: 10px;'></img>\n");
#else
websWrite(wp, "            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src='picts/logo.png'></img>\n");
#endif

websWrite(wp, "              <a><small>&nbsp;</small></a>\n");
websWrite(wp, "                 <ul class='nav ace-nav pull-right'>\n");

websWrite(wp, "                     <li class='user-profile' id='navbar_battery' style='float:left;display: none; background-color:#fff;'>\n");
websWrite(wp, "                     <a>\n");
websWrite(wp, "                             <i class='icon-battery' style='color:rgb(178,0,0);'></i>\n");
websWrite(wp, "                             <span id='nav_battery' style='color:rgb(178,0,0);'></span>\n");
websWrite(wp, "                     </a>\n");
websWrite(wp, "                     </li>\n");

websWrite(wp, "                     <li class='user-profile' id='navbar_storage' style='float:left;display: none; background-color:#fff;'>\n");
websWrite(wp, "                     <a>\n");
websWrite(wp, "                             <i class='icon-storage' style='color:rgb(178,0,0);'></i>\n");
websWrite(wp, "                             <span id='nav_storage' style='color:rgb(178,0,0);'></span>\n");
websWrite(wp, "                     </a>\n");
websWrite(wp, "                     </li>\n");

websWrite(wp, "                     <li class='user-profile'>\n");
websWrite(wp, "                         <a class='dropdown-toggle' href='#' data-toggle='dropdown' style='background-color:rgb(255,255,255);' onclick='refresh()'>\n");
websWrite(wp, " 						    &nbsp;&nbsp;\n");
websWrite(wp, " 							<i class=\"icon-refresh\" style='color:rgb(67,67,67);'></i>\n");
websWrite(wp, " 						    &nbsp;&nbsp;\n");
websWrite(wp, "                         </a>\n");
websWrite(wp, "                     </li>\n");

websWrite(wp, "                     <li class='light-aximOrange user-profile'>\n");
websWrite(wp, "                         <a class='user-menu dropdown-toggle' href='#' data-toggle='dropdown'>\n");
websWrite(wp, "                         <img alt='Jason's Photo' src='picts/admin.png' class='nav-user-photo' style='background-color:white;'></img>\n");
websWrite(wp, "                             <span class='user-info' id='user_info'>\n");
if(access_permission == GUEST)
websWrite(wp, "                             <small>%s,</small> %s\n", lang_translate("Welcome"), lang_translate("Guest"));
else
websWrite(wp, "                             <small>%s,</small> %s\n", lang_translate("Welcome"), lang_translate("Admin"));

websWrite(wp, "                             </span>\n");
websWrite(wp, "                             <i class='icon-caret-down'></i>\n");
websWrite(wp, "                         </a>\n");
websWrite(wp, "                         <ul id='user_menu' class='pull-right dropdown-menu dropdown-yellow dropdown-caret dropdown-closer'>\n");
websWrite(wp, "                             <li><a href='#' onclick='logout()'><i class='icon-off'></i> %s</a></li>\n", lang_translate("Logout"));
websWrite(wp, "                         </ul>\n");
websWrite(wp, "                     </li>\n");

websWrite(wp, "                     </ul>\n");
websWrite(wp, "     </div>\n");
websWrite(wp, " </div><!-- /.navbar -->\n");
websWrite(wp, "</div>\n");

    return 0;
}
#endif
int
ej_do_headmeta(int eid, webs_t wp, int argc, char_t ** argv)
{
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    websWrite(wp, "<meta charset='utf-8'>\n");
    websWrite(wp, "<meta name='description' content='overview & stats'>\n");
    websWrite(wp, "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n");
#else
    websWrite(wp, "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>\n");
    websWrite(wp, "<meta charset='utf-8'>\n");
    websWrite(wp, "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n");
#endif
    return 0;
}            

int
ej_do_basic_css(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- bootstrap CSS -->\n");
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    websWrite(wp, "<link href='css/bootstrap.min.css' rel='stylesheet' type='text/css'>\n");
    websWrite(wp, "<link href='css/bootstrap-responsive.min.css' rel='stylesheet' type='text/css'>\n");
#else
    websWrite(wp, "<link href='ext/css/bootstrap/bootstrap.min.css' rel='stylesheet' type='text/css'>\n");
    websWrite(wp, "<link href='ext/css/bootstrap/bootstrap-responsive.min.css' rel='stylesheet' type='text/css'>\n");
#endif
    return 0;
}

int
ej_do_ace_font_css(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- Font Awesome CSS for Ace Plug-in -->\n");
    websWrite(wp, "<link href='font-awesome/css/font-awesome.css' rel='stylesheet' type='text/css' />\n");
    return 0;
}

int 
ej_do_ace_font_ie7_css(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!--[if IE 7]>\n");
    websWrite(wp, "  <link rel=stylesheet' href='font-awesome/css/font-awesome-ie7.min.css' />\n");
    websWrite(wp, "<![endif]-->\n");
    return 0;
}

int
ej_do_ace_basic_css(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- Ace Basic CSS -->\n");
    websWrite(wp, "<link rel='stylesheet' href='css/ace.min.css' />\n");
    websWrite(wp, "<link rel='stylesheet' href='css/ace-responsive.min.css' />\n");
    websWrite(wp, "<link rel='stylesheet' href='css/ace-skins.min.css' />\n");
    return 0;
}

int 
ej_do_ace_basic_ie9_css(int eid, webs_t wp, int argc, char_t ** argv )
{
    websWrite(wp, "<!--[if lt IE 9]>\n");
    websWrite(wp, "  <link rel=stylesheet' href='css/ace-ie.min.css' />\n");
    websWrite(wp, "<![endif]-->\n");
   return 0;
}

int
ej_do_basic_js(int eid, webs_t wp, int argc, char_t ** argv)
{
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    websWrite(wp, "<!-- jQuery JS -->\n");
    websWrite(wp, "<script src='js/jquery.min.js' type='text/javascript'></script>\n");
    websWrite(wp, "<!-- bootstrap JS -->\n");
    websWrite(wp, "<script src='js/bootstrap.min.js' type='text/javascript'></script>\n");
#else
    websWrite(wp, "<!-- jQuery JS -->\n");
    websWrite(wp, "<script src='ext/js/jquery-min.js' type='text/javascript'></script>\n");
    websWrite(wp, "<!-- bootstrap JS -->\n");
    websWrite(wp, "<script src='ext/js/bootstrap.min.js' type='text/javascript'></script>\n");
    websWrite(wp, "<noscript>JavaScript? is off. Please enable to view full site.</noscript>\n");
#endif
    return 0;
}

int
ej_do_custom_css(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- custom AXIM Style  CSS -->\n");
    websWrite(wp, "<link href='css/aximcolor.css' rel='stylesheet' type='text/css'>\n");
    return 0;
}

int
ej_do_custom_js(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- custom JS -->\n");
    websWrite(wp, "<script src='common.js' type='text/javascript'></script>\n");
    return 0;
}

int
ej_do_datatables_css(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- DataTables CSS -->\n");
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    websWrite(wp, "<link href='css/DataTables/jquery.dataTables.css' rel='stylesheet' type='text/css'>\n");
#else
    websWrite(wp, "<link href='ext/css/DataTables/jquery.dataTables.css' rel='stylesheet' type='text/css'>\n");
#endif
    return 0;
}

int
ej_do_datatables_js(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- DataTables JS -->\n");
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    websWrite(wp, "<script src='js/jquery.dataTables.min.js' type='text/javascript'></script>\n");
    websWrite(wp, "<script src='js/jquery.dataTables.bootstrap.js' type='text/javascript'></script>\n");
#else
    websWrite(wp, "<script src='ext/js/jquery.dataTables.min.js' type='text/javascript'></script>\n");
    websWrite(wp, "<script src='ext/js/jquery.dataTables-ext.js' type='text/javascript'></script>\n");
#endif
    return 0;
}

int
ej_do_jquery_ui_js(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- Jquery UI JS  -->\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery-ui.custom.min.js' > </script>\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.ui.touch-punch.min.js'> </script>\n");
    return 0;
}

int
ej_do_jquery_other_js(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- Jquery Other JS  -->\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.slimscroll.min.js' > </script>\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.easy-pie-chart.min.js'> </script>\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.sparkline.min.js'> </script>\n");
    return 0;
}

int
ej_do_jquery_flot_js(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- Jquery Flot  JS  -->\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.flot.min.js' > </script>\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.flot.time.js' > </script>\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.flot.axislabels.js' > </script>\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.flot.pie.min.js'> </script>\n");
    websWrite(wp, "<script type='text/javascript' src='js/jquery.flot.resize.min.js'> </script>\n");
    return 0;
}

int
ej_do_ace_basic_js(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- Ace Basic  JS  -->\n");
    websWrite(wp, "<script src='js/ace-elements.min.js' > </script>\n");
    websWrite(wp, "<script src='js/ace.min.js'> </script>\n");
    return 0;
}

int
ej_show_valid_msg(int eid, webs_t wp, int argc, char_t ** argv)
{
    return config_validate(wp, 1);
}


int
ej_do_bootstrap_modal(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "<!-- bootstrap modal css -->\n");
    websWrite(wp, "<link rel='stylesheet' href='css/bootstrap-modal.css'>\n");
    websWrite(wp, "<!-- bootstrap modal js -->\n");
    websWrite(wp, "<script src='js/bootstrap-modalmanager.js' type='text/javascript'></script>\n");
    websWrite(wp, "<script src='js/bootstrap-modal.js' type='text/javascript'></script>\n");
    return 0;
}

/*
 * ej family. Return 0 if everything is fine; return 1 if something wrong.
 */

static int
ej_nv_get(int eid, webs_t wp, int argc, char_t **argv)
{
    char *name, *c;
    int ret = 0;
#if defined (EZP_PROD_BRAND_PROX)
    char netmask[16];
#endif

    if (ejArgs(argc, argv, "%s", &name) < 1) {
        websDebugWrite(wp, "Insufficient args\n");
        return -1;
    }

#if defined (EZP_PROD_BRAND_PROX)
    if (!strcmp("model",name)) { return ej_nv_get_model_for_prox(wp); }
    if (!strcmp("get_model_only", name)) { sprintf(name,"model"); }
    if (strstr(name,"_mask") && (strstr(name,"wan")||strstr(name,"lan") )) {
        if ( trans_int2netmask(atoi(nvram_safe_get(name)),netmask,sizeof(netmask)) != -1 ) 
                return websWrite(wp,"%s",netmask);
        else 
                return websWrite(wp,"%s","");
    }
#endif

    for (c = nvram_safe_get(name); *c; c++) {
        if (isprint((int) *c) &&
            *c != '"' && *c != '&' && *c != '<' && *c != '>' && *c != '\'')
            ret += websWrite(wp, "%c", *c);
        else
            ret += websWrite(wp, "&#%d;", *c);
    }
    return ret;
}

#if defined (EZP_PROD_BRAND_PROX)
int
ej_nv_get_model_for_prox(webs_t wp)
{
    int  ret = 0;
    char mask[32];
    char ipaddr[32];
    int  come_from_lan=0;
    char *c;
   
    trans_int2netmask(atoi(nvram_safe_get("lan0_mask")), mask, sizeof(mask));
    if (is_legal_ip_netmask(nvram_safe_get("lan0_ipaddr"), mask,inet_ntoa(client.ipaddr))) {
        strcpy(ipaddr, nvram_safe_get("lan0_ipaddr"));
        come_from_lan=1;
    } else {
        if ( strlen(nvram_safe_get("wan1_ipaddr")) ) 
            strcpy(ipaddr, nvram_safe_get("wan1_ipaddr"));
        else
            strcpy(ipaddr, nvram_safe_get("wan0_ipaddr"));
        come_from_lan=0;
    }

    if (!come_from_lan) ret +=  websWrite(wp,"(");	

    for (c = ipaddr; *c; c++) {
        if (isprint((int) *c) &&
            *c != '"' && *c != '&' && *c != '<' && *c != '>' && *c != '\'')
            ret += websWrite(wp, "%c", *c);
        else
            ret += websWrite(wp, "&#%d;", *c);
    }

#ifdef EZP_SUB_BRAND_APOLLO
   ret += come_from_lan ? websWrite(wp,"-Apollo ") : websWrite(wp,")-WAN-Apollo ");	
#else
   ret += come_from_lan ? websWrite(wp,"-Proxicast ") : websWrite(wp,")-WAN-Proxicast ");	
#endif

   for (c = nvram_safe_get("model"); *c; c++) {
        if (isprint((int) *c) &&
            *c != '"' && *c != '&' && *c != '<' && *c != '>' && *c != '\'')
            ret += websWrite(wp, "%c", *c);
        else
            ret += websWrite(wp, "&#%d;", *c);
    }

    return ret;
}
#endif

static int
ej_nv_attr_get(int eid, webs_t wp, int argc, char_t **argv)
{
    char *name, *type;
    int nth;
    char buf[TMP_LEN], tmp[TMP_LEN]={0};
    int ret;
    unsigned long n = TMP_LEN;

    if (ejArgs(argc, argv, "%s %d %s", &name, &nth, &type) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    ret = ezplib_get_attr_val(name, nth, type, buf, TMP_LEN, EZPLIB_USE_CLI); 

    if (ret > 0) {
	    if((!strcmp(name, "wl0_apcli_swap_rule") && !strcmp(type, "ssid")) || (!strcmp(name, "wl0_apcli_rule") && !strcmp(type, "ssid"))) {
		    base64_decode((unsigned char *)buf, strlen(buf), (unsigned char *)tmp, &n);
            ui_parser(tmp);	
            websWrite(wp, "%s", tmp);
	    }
        else
            websWrite(wp, "%s", buf);
    }

    return ret; 
}

static int
ej_lang(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *str;
    char *trans;

    if (ejArgs(argc, argv, "%s", &str) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    trans = lang_translate(str);
    return websWrite(wp, "%s", trans);
}

#ifdef EZP_PROD_BRAND_ZYUS
static int
ej_increase_org(int eid, webs_t wp, int argc, char_t ** argv)
{
    int val, inc;

    if (ejArgs(argc, argv, "%d %d", &val, &inc) < 2) {
        websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }

    return websWrite(wp, "%d", val+inc);
}
static int
ej_increase(int eid, webs_t wp, int argc, char_t ** argv)
{
    int val, inc;

    if (ejArgs(argc, argv, "%d %d", &val, &inc) < 2) {
        websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }
    if (val == 0 )
        websWrite(wp, "%s", "Ethernet");
    else if (val == 1 )
        websWrite(wp, "%s", "Mobile");
    return 0;
}
#else
static int
ej_increase(int eid, webs_t wp, int argc, char_t ** argv)
{
    int val, inc;

    if (ejArgs(argc, argv, "%d %d", &val, &inc) < 2) {
        websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }

    return websWrite(wp, "%d", val+inc);
}
#endif

#if defined (EZP_PROD_BRAND_PROX)
static int
ej_increase_wan(int eid, webs_t wp, int argc, char_t ** argv)
{
    int val, inc;

    if (ejArgs(argc, argv, "%d %d", &val, &inc) < 2) {
        websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }

    if (val == 0 )
        websWrite(wp, "(Ethernet)");
    else if (val == 1 )
        websWrite(wp, "(USB Modem)");
    return 0;
}
#endif 

/*
 * Example: 
 * wan_proto = dhcp; gozilla = 0;
 * <% nvg_get("wan_proto"); %> produces "dhcp"
 *
 * wan_proto = dhcp; gozilla = 1; websGetVar(wp, "wan_proto", NULL) = static;
 * <% nvg_get("wan_proto"); %> produces "static"
 */
static int
ej_nvg_get(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name, *type;

    if (ejArgs(argc, argv, "%s", &name) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    type = GOZILA_SAFE_GET(name);

    return websWrite(wp, "%s", type);
}

static int
ej_show_next_page(int eid, webs_t wp, int argc, char_t ** argv)
{
    return websWrite(wp, "%s.asp",
            websGetVar(wp, "submit_button", "wan"));
}


/*
 * Example: 
 * wan_proto = dhcp; gozilla = 0;
 * <% nvg_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 *
 * wan_proto = dhcp; gozilla = 1; websGetVar(wp, "wan_proto", NULL) = static;
 * <% nvg_match("wan_proto", "static", "selected"); %> produces "selected"
 */
int
ej_nvg_match(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name, *match, *output;
    char *type;

    if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    type = GOZILA_SAFE_GET(name);

    if (!*type) {
        if (nvram_match(name, match)) {
            return websWrite(wp, output);
        }
    } else {
        if (!strcmp(type, match)) {
            return websWrite(wp, output);
        } }

    return 0;
}

int
ej_nvg_else_match(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name, *match, *output1, *output2;
    char *type;

    if (ejArgs
        (argc, argv, "%s %s %s %s", &name, &match, &output1,
         &output2) < 4) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    type = GOZILA_SAFE_GET(name);

    if (!*type) {
        if (nvram_match(name, match)) {
            return websWrite(wp, output1);
        } else
            return websWrite(wp, output2);
    } else {
        if (!strcmp(type, match)) {
            return websWrite(wp, output1);
        } else
            return websWrite(wp, output2);
    }

    return 0;
}

static int
prod_cat_check(char *cat)
{
    if (strstr(cat, "PROD_CAT_B") && (PROD_CAT_BITMAP & PROD_CAT_B)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_C") && (PROD_CAT_BITMAP & PROD_CAT_C)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_G") && (PROD_CAT_BITMAP & PROD_CAT_G)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_I") && (PROD_CAT_BITMAP & PROD_CAT_I)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_D") && (PROD_CAT_BITMAP & PROD_CAT_D)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_L") && (PROD_CAT_BITMAP & PROD_CAT_L)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_M") && (PROD_CAT_BITMAP & PROD_CAT_M)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_U") && (PROD_CAT_BITMAP & PROD_CAT_U)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_N") && (PROD_CAT_BITMAP & PROD_CAT_N)) {
        return 1;
    }
    
    if (strstr(cat, "PROD_CAT_A") && (PROD_CAT_BITMAP & PROD_CAT_A)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_P") && (PROD_CAT_BITMAP & PROD_CAT_P)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_R") && (PROD_CAT_BITMAP & PROD_CAT_R)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_W") && (PROD_CAT_BITMAP & PROD_CAT_W)) {
        return 1;
    }

    if (strstr(cat, "PROD_CAT_T") && (PROD_CAT_BITMAP & PROD_CAT_T)) {
        return 1;
    }
    if (strstr(cat, "PROD_CAT_V") && (PROD_CAT_BITMAP & PROD_CAT_V)) {
        return 1;
    }
    if (strstr(cat, "PROD_CAT_K") && (PROD_CAT_BITMAP & PROD_CAT_K)) {
        return 1;
    }
    return 0;
}

static int
prod_subcat_check(int subcat_low, int subcat_high)
{
    if (subcat_low > ezp_prod_subcat || subcat_high < ezp_prod_subcat) {
        return 0;
    } else {
        return 1;
    }
}
static int
prod_subsubcat_check(char *subsubcat)
{
    if(!strcmp(subsubcat, ""))
        return 1;
    if (strstr(subsubcat, prod_subsubcat)) {
        return 1;
    } else {
        return 0;
    }
}
/* Because we need to verify app enable/disable status, so the
 * buffer doesn't need too large. */
static char appmgrbuf[2] = "";
/* We cache previous requested app name to avoid nvram access multiple times
 * for the same application. */
static char appmgrprevapp[10] = "";
int
appmgr_check(char *appname) {
    if (!appname)
        return 0;
    if (!strncmp(appmgrprevapp, appname, sizeof(appmgrprevapp))) {
        return appmgrbuf[0] - '0';
    }
    ezplib_get_attr_val("appmgr_rule", 0, appname, appmgrbuf, sizeof(appmgrbuf), EZPLIB_USE_CLI);
    if (appmgrbuf[0] && (appmgrbuf[0] - '0')) {
        return appmgrbuf[0] - '0';
    }
    return 0;
}

int
ej_prod_show_js_start(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cat;
    int subcat_low, subcat_high;

    if (ejArgs(argc, argv, "%s %d %d", &cat, &subcat_low, &subcat_high) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* Process the string of prod categories. eg., PROD_CAT_B|PROD_CAT_L */
    if (!(prod_cat_check(cat) || appmgr_check(cat))) {
        websWrite(wp, "/*");
        return 0;
    }

    if (!prod_subcat_check(subcat_low, subcat_high)) {
        websWrite(wp, "/*");
        return 0;
    }

    return 0;
}

int
ej_prod_show_js_end(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cat;
    int subcat_low, subcat_high;

    if (ejArgs(argc, argv, "%s %d %d", &cat, &subcat_low, &subcat_high) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* Process the string of prod categories. eg., PROD_CAT_B|PROD_CAT_L */
    if (!(prod_cat_check(cat) || appmgr_check(cat))) {
        websWrite(wp, "*/");
        return 0;
    }

    if (!prod_subcat_check(subcat_low, subcat_high)) {
        websWrite(wp, "*/");
        return 0;
    }

    return 0;
}

int
ej_prod_show_html_start(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cat;
    int subcat_low, subcat_high;

    if (ejArgs(argc, argv, "%s %d %d", &cat, &subcat_low, &subcat_high) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* Process the string of prod categories. eg., PROD_CAT_B|PROD_CAT_L */
    if (!(prod_cat_check(cat) || appmgr_check(cat))) {
        websWrite(wp, "<!--");
        return 0;
    }

    if (!prod_subcat_check(subcat_low, subcat_high)) {
        websWrite(wp, "<!--");
        return 0;
    }

    return 0;
}

int
ej_prod_show_js(int eid, webs_t wp, int argc, char_t **argv)
{
    char *type, *function;
    int show_flag = 0;
    if (ejArgs(argc, argv, "%s %s", &type, &function) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    if(!strcmp("wisp", function) && appmgr_check("wisp")) {
        show_flag = 1;
    }
    if (!show_flag) {
        if(!strcmp("start", type)) {
            websWrite(wp, "/*");
        } else if(!strcmp("end", type)) {
            websWrite(wp, "*/");
        }
    }
    return 0;
}

int
ej_prod_show_html(int eid, webs_t wp, int argc, char_t **argv)
{
    char *type, *function;
    int show_flag = 0, num = 0;
    num=atoi(nvram_safe_get("wan_num"));
#ifdef EZP_PROD_BRAND_ZYUS
    /* ZYUS only nees to show one section. */
    num = 1;
#endif
    if (ejArgs(argc, argv, "%s %s", &type, &function) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    if(!strcmp("wisp", function) && !appmgr_check("wisp")) {
        show_flag = 1;
    } else if(!strcmp("wlan", function) && (atoi(nvram_safe_get("wl_num")))) {
        show_flag = 1;
    } else if(!strcmp("lb", function) && (num >= 2)) {
        show_flag = 1;
    }
    if (!show_flag) {
        if(!strcmp("start", type)) {
            websWrite(wp, "<!--");
        } else if(!strcmp("end", type)) {
            websWrite(wp, "-->");
        }
    }
    return 0;
}

int
ej_prod_show_html_end(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cat;
    int subcat_low, subcat_high;

    if (ejArgs(argc, argv, "%s %d %d", &cat, &subcat_low, &subcat_high) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* Process the string of prod categories. eg., PROD_CAT_B|PROD_CAT_L */
    if (!(prod_cat_check(cat) || appmgr_check(cat))) {
        websWrite(wp, "-->");
        return 0;
    }

    if (!prod_subcat_check(subcat_low, subcat_high)) {
        websWrite(wp, "-->");
        return 0;
    }

    return 0;
}
int
ej_prod_comment_html_start(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cat, *sub_subcat;
    int subcat_low, subcat_high;

    if (ejArgs(argc, argv, "%s %d %d %s",
                &cat, &subcat_low, &subcat_high, &sub_subcat) < 4) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* if match all condition, this function will comment html code after this
     * position */
    if (prod_cat_check(cat) && prod_subcat_check(subcat_low, subcat_high) && 
            prod_subsubcat_check(sub_subcat)) {
        websWrite(wp, "<!--");
    }
    /* special use case, 
    cat = app_enable, we will check if application "sub_subcat" is enabled
    if this application is not enabled, we will send out a comment mark. */
    if ( (!strncmp(cat, "app_enable", sizeof("app_enable"))) && 
         (ezplib_check_attr_changed("app_enable_rule", 0, sub_subcat, "1"))) {
        websWrite(wp, "<!--");
    }
    return 0;
}
int
ej_prod_comment_html_end(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cat, *sub_subcat;
    int subcat_low, subcat_high;

    if (ejArgs(argc, argv, "%s %d %d %s",
                &cat, &subcat_low, &subcat_high, &sub_subcat) < 4) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* if match all condition, this function will comment html code until this
     * position */
    if (prod_cat_check(cat) && prod_subcat_check(subcat_low, subcat_high) && 
            prod_subsubcat_check(sub_subcat)) {
        websWrite(wp, "-->");
    }
    /* special use case, 
    cat = app_enable, we will check if application "sub_subcat" is enabled
    if this application is not enabled, we will send out a comment mark. */
    if ( (!strncmp(cat, "app_enable", sizeof("app_enable"))) && 
         (ezplib_check_attr_changed("app_enable_rule", 0, sub_subcat, "1"))) {
        websWrite(wp, "-->");
    }
    return 0;
}

int
ej_prod_comment_js_start(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cat, *sub_subcat;
    int subcat_low, subcat_high;

    if (ejArgs(argc, argv, "%s %d %d %s",
                &cat, &subcat_low, &subcat_high, &sub_subcat) < 4) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* if match all condition, this function will comment java script after this
     * position */
    if (prod_cat_check(cat) && prod_subcat_check(subcat_low, subcat_high) && 
            prod_subsubcat_check(sub_subcat)) {
        websWrite(wp, "/*");
    }
    /* special use case, 
    cat = app_enable, we will check if application "sub_subcat" is enabled
    if this application is not enabled, we will send out a comment mark. */
    if ( (!strncmp(cat, "app_enable", sizeof("app_enable"))) && 
         (ezplib_check_attr_changed("app_enable_rule", 0, sub_subcat, "1"))) {
        websWrite(wp, "/*");
    }
    return 0;
}

int
ej_prod_comment_js_end(int eid, webs_t wp, int argc, char_t **argv)
{
    char *cat, *sub_subcat;
    int subcat_low, subcat_high;

    if (ejArgs(argc, argv, "%s %d %d %s",
                &cat, &subcat_low, &subcat_high, &sub_subcat) < 4) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    /* if match all condition, this function will comment java script until this
     * position */
    if (prod_cat_check(cat) && prod_subcat_check(subcat_low, subcat_high) && 
            prod_subsubcat_check(sub_subcat)) {
        websWrite(wp, "*/");
    }
    /* special use case, 
    cat = app_enable, we will check if application "sub_subcat" is enabled
    if this application is not enabled, we will send out a comment mark. */
    if ( (!strncmp(cat, "app_enable", sizeof("app_enable"))) && 
         (ezplib_check_attr_changed("app_enable_rule", 0, sub_subcat, "1"))) {
        websWrite(wp, "*/");
    }
    return 0;
}
/*
 * Example: 
 * wan_proto=dhcp
 * <% nv_else_match("wan_proto", "dhcp", "0","1"); %> produces "0"
 * <% nv_else_match("wan_proto", "static", "0","1"); %> produces "1"
 */
static int
ej_nv_else_match(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name, *match, *output1, *output2;

    if (ejArgs
        (argc, argv, "%s %s %s %s", &name, &match, &output1,
         &output2) < 4) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if (nvram_match(name, match))
        return websWrite(wp, output1);
    else
        return websWrite(wp, output2);

    return 0;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 * <% nv_match("wan_proto", "static", "selected"); %> does not produce
 */
static int
ej_nv_match(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name, *match, *output;

    if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if (nvram_match(name, match))
        return websWrite(wp, output);

    return 0;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nv_invmatch("wan_proto", "dhcp", "disabled"); %> does not produce
 * <% nv_invmatch("wan_proto", "static", "disabled"); %> produces "disabled"
 */
static int
ej_nv_invmatch(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name, *invmatch, *output;

    if (ejArgs(argc, argv, "%s %s %s", &name, &invmatch, &output) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if (nvram_invmatch(name, invmatch))
        return websWrite(wp, output);

    return 0;
}

/*
 * EZP 
 */
int
ej_show_control(int eid, webs_t wp, int argc, char_t ** argv)
{
    websWrite(wp, "EZ Packet Management Panel");
    return 0;
}

int
ej_show_localtime(int eid, webs_t wp, int argc, char_t ** argv)
{
    char time[TMP_LEN];
    int ret = -1;

    if (!show_localtime(time, TMP_LEN)) {
        ret = websWrite(wp, "%s", time);
        return 0;
    } else {
        ret = websWrite(wp, "Unknown");
        return ret;
    }
}

int
ej_show_uptime(int eid, webs_t wp, int argc, char_t ** argv)
{
    char uptime[TMP_LEN];
    int ret = -1;

    if (!show_uptime(uptime, TMP_LEN)) {
        ret = websWrite(wp, "%s", uptime);
        return 0;
    } else {
        ret = websWrite(wp, "Unknown");
        return ret;
    }
}

int
ej_show_ipaddr(int eid, webs_t wp, int argc, char_t ** argv)
{
    char ipaddr[32];
    char *name, *type;
    int ret = -1;

    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    name = nvram_safe_get(type);

    if (show_ipaddr(name, ipaddr, 32)) {
        ret = websWrite(wp,"%s", lang_translate("Not Available"));
        return ret;
    } else {
        ret = websWrite(wp, "%s", ipaddr);
        return 0;
    }
}

int
ej_show_ipaddr_prefix24(int eid, webs_t wp, int argc, char_t ** argv)
{
    char ipaddr[32];
    char *name, *type;
    int ret = -1;

    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    name = nvram_safe_get(type);

    if (show_ipaddr_prefix24(name, ipaddr, 32)) {
        ret = websWrite(wp,"%s", lang_translate("Not Available"));
        return ret;
    } else {
        ret = websWrite(wp, "%s", ipaddr);
        return 0;
    }
}

int
ej_show_hwaddr(int eid, webs_t wp, int argc, char_t ** argv)
{
    char ifname[16], ifproto[16];
    char device[16];
    char hwaddr[64];
    char *if_ptr, *type, *if_proto;
    int ret = -1;

    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if (snprintf(ifproto, 16, "%s_proto", type) >= 16)
        return -1;

    if_proto = nvram_safe_get(ifproto);
    if (!strncmp(if_proto, "wwan", 4)) {
        ret = websWrite(wp, "No MAC Address");
        return ret;
    } else {

        if (snprintf(ifname, 16, "%s_ifname", type) >= 16)
            return -1;

        if_ptr = nvram_safe_get(ifname);

        /* PPPoE interface doesn't have MAC address. */
        /* br0 should use vlan1'a MAC address */
        if (!strncmp(if_ptr, "ppp", 3) || !strncmp(if_ptr, "br", 2)) {
            if (snprintf(device, 16, "%s_device", type) >= 16)
                return -1;
            if_ptr = nvram_safe_get(device);
        }
        if (!show_hwaddr(if_ptr, hwaddr, 64)) {
            ret = websWrite(wp, "%s", hwaddr);
            return 0;
        } else {
            ret = websWrite(wp, "Not Connected");
            return ret;
        }
    }
}

int
ej_st_show_battery_status(int eid, webs_t wp, int argc, char_t ** argv)
{
    int ret = -1;

    FILE *pFile;
    char result[24]={0};
    int percentage = 0;
    if ((pFile = popen("ezp-i2c gauge percentage", "r"))) {
        fgets(result, sizeof(result), pFile);
        sscanf(result,"Battery: %d %%", &percentage);
        ret = websWrite(wp, "%d %%", percentage);
    } else {
        ret = websWrite(wp, "Unknown");
        return ret;
    }
    // terminate
    pclose (pFile);
    return ret;
}

int
ej_st_show_charge_status(int eid, webs_t wp, int argc, char_t ** argv)
{
    int ret = -1;

    FILE *pFile;
    char result[24]={0};
    if ((pFile = popen("ezp-i2c charger", "r"))) {
        fgets(result, sizeof(result), pFile);
        //sscanf(result,"%s", &charge_status);
        if(strncmp(result,"No charger",10) == 0)
            ret = websWrite(wp, "%s", lang_translate("Not Charging"));
        else
            ret = websWrite(wp, "%s", lang_translate("Charging"));
    } else {
        ret = websWrite(wp, "%s", lang_translate("Unknown"));
        return ret;
    }
    // terminate
    pclose (pFile);
    return ret;
}

int
ej_st_show_storage_status(int eid, webs_t wp, int argc, char_t ** argv)
{

    FILE *f;
    int free_size, size, use_size;
    char buf[TMP_LEN];
    char storage_size[SHORT_BUF_LEN],storage_usize[SHORT_BUF_LEN],
         storage_upercentage[SHORT_BUF_LEN],storage_fsize[SHORT_BUF_LEN];

    char path[SHORT_BUF_LEN],f_size[SHORT_BUF_LEN],dir[SHORT_BUF_LEN]={0};
    char storage_state[SHORT_BUF_LEN];
    char *lan_ip;
    char *device_name;
    int device_id = 0; //default USB=0

    if (ejArgs(argc, argv, "%s", &device_name) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if(strcmp("SD",device_name)==0)
        device_id=1;

    /* Storage infomation */
    ezplib_get_attr_val("storage_state_rule", device_id, "mount_state",
                        storage_state, sizeof(storage_state), EZPLIB_USE_CLI);

    lan_ip = nvram_get("lan0_ipaddr");
    if (strcmp(storage_state, "mounted") == 0) {
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf),
                "/sbin/getsysinfo disk_info %s", device_name);
        if ((f = popen(buf, "r"))) {
            fscanf(f, "%s %s %s %s %s %s", path, storage_size, storage_usize,
                   f_size, storage_upercentage, dir);
            if ((strlen(dir)) > 0) {
                size = atoi(storage_size);
                use_size = atoi(storage_usize);
            } else {
                size = atoi(path);
                use_size = atoi(storage_size);
                strcpy(storage_upercentage, f_size);
            }
            adj_size((float)size, storage_size, sizeof(storage_size));
            free_size = size - use_size;
            adj_size((float)free_size, storage_fsize, sizeof(storage_fsize));
            websWrite(wp, "<a href='http://%s:8081/'>%s/%s (free/total)</a>",lan_ip,
                    storage_fsize, storage_size);
            pclose(f);
        }
    } else if (strcmp(storage_state, "unknown") == 0) {
        websWrite(wp, lang_translate("Unknown"));
    } else if(strcmp(storage_state, "format") == 0) {
        websWrite(wp, lang_translate("Formatting"));
    } else if(strcmp(storage_state, "con_err") == 0) {
        websWrite(wp, lang_translate("Connect Error"));
    } else {
        websWrite(wp, lang_translate("Unplugged"));
    }

    return 0;
}

int
ej_show_server_ip(int eid, webs_t wp, int argc, char_t ** argv)
{
    extern int server_port;
    extern char server_ipaddr[32];
    websWrite(wp, "%s:%d",server_ipaddr,server_port);

    return 0;
}

int
iwpriv_test()
{
    FILE *fp;
    char tmp[TMP_LEN] = {0};
    char cmd[TMP_LEN] = {0};
    snprintf(cmd, sizeof(cmd), "iwpriv ra0 e2p 00");
    if((fp = popen(cmd, "r"))) {
        fgets(tmp, sizeof(tmp), fp);
        pclose(fp);
    }
    if (!strcmp(tmp,"")) return 0;
    else return 1;
}

int
iwpriv_get(int addr, char *res, int len)
{
    FILE *fp;
    char iwpriv[] = "iwpriv ra0 e2p";
    char cmd[TMP_LEN] = {0};
    snprintf(cmd, sizeof(cmd), "%s %2X", iwpriv, addr);
    if((fp = popen(cmd, "r"))) {
        fgets(res, len, fp);
        fgets(res, len, fp);
        pclose(fp);
        return 0;
    }
    return 1;
}

int 
ej_show_eeprom_hwaddr(int eid, webs_t wp, int argc, char_t ** argv)
{
    char tmp[TMP_LEN] = {0};
    int wan[3] = { 40,42,44 };
    int wlan[3] = { 4,6,8 };
    int mac1, mac2, i;
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    websWrite(wp, "WAN=");
    for (i = 0; i < 3;i++) {
        iwpriv_get(wan[i], tmp, sizeof(tmp));
        sscanf(tmp,"[0x00%*2X]:0x%2X%2X", &mac1, &mac2);
        websWrite(wp, "%02X:%02X", mac2, mac1);
        if (i < 2)
            websWrite(wp, ":");
    }
    websWrite(wp, "<br>WLAN=");
    for (i = 0; i < 3;i++) {
        iwpriv_get(wlan[i], tmp, sizeof(tmp));
        sscanf(tmp,"[0x00%*2X]:0x%2X%2X", &mac1, &mac2);
        websWrite(wp, "%02X:%02X", mac2, mac1);
        if (i < 2)
            websWrite(wp, ":");
    }
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int 
ej_show_tx_channel(int eid, webs_t wp, int argc, char_t ** argv)
{
    int reg, i;
    char tmp[TMP_LEN];
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    for (i = 82; i <= 108; i+=2) {
        iwpriv_get(i, tmp, sizeof(tmp));
        sscanf(tmp,"[0x00%*2X]:0x%4X", &reg);
        websWrite(wp, "%04X", reg);
    }
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int 
ej_show_wl_region(int eid, webs_t wp, int argc, char_t ** argv)
{
    int reg;
    char tmp[TMP_LEN];
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    iwpriv_get(56, tmp, sizeof(tmp));
    sscanf(tmp,"[0x00%*2X]:0x%2X%*2X", &reg);
    websWrite(wp, "%2X", reg);
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int 
ej_show_wl_band(int eid, webs_t wp, int argc, char_t ** argv)
{
    int reg;
    char tmp[TMP_LEN];
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    iwpriv_get(56, tmp, sizeof(tmp));
    sscanf(tmp,"[0x00%*2X]:0x%*2X%2X", &reg);
    websWrite(wp, "%2X", reg);
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int 
ej_show_freq_off(int eid, webs_t wp, int argc, char_t ** argv)
{
    int reg;
    char tmp[TMP_LEN];
    char ra0_flag=iwpriv_test();
    if (!ra0_flag) system("ifconfig ra0 up");
    iwpriv_get(58, tmp, sizeof(tmp));
    sscanf(tmp,"[0x00%*2X]:0x%*2X%2X", &reg);
    websWrite(wp, "%02X", reg);
    if (!ra0_flag) system("ifconfig ra0 down");
    return 0;
}

int 
ej_show_uboot(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *fp;
    char cmd[] = "grep -r U-Boot /dev/mtd1 | grep -r \\\(|sed -n '1p'|sed 's/^.*U-Boot/U-Boot/g'";
    char tmp[TMP_LEN];
    if((fp = popen(cmd, "r"))) {
        fgets(tmp, sizeof(tmp), fp);
        
        websWrite(wp, "%s", tmp);
        pclose(fp);
    }
    return 0;
}

int 
ej_show_build_time(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *fp;
    char cmd[] = "uname -a";
    char tmp[TMP_LEN];
    if((fp = popen(cmd, "r"))) {
        fgets(tmp, sizeof(tmp), fp);
        websWrite(wp, "%s", tmp);
        pclose(fp);
    }
    return 0;
}

int 
ej_show_gauge_fw(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *fp;
    char tmp[TMP_LEN];
    system("ezp-i2c gauge fw");
    if((fp = fopen("/tmp/gauge_fw", "r"))) {
        fgets(tmp, sizeof(tmp), fp);
        websWrite(wp, "%s", tmp);
        fclose(fp);
    }
    return 0;
}

int 
ej_show_mcu_fw(int eid, webs_t wp, int argc, char_t ** argv)
{
    FILE *fp;
    char tmp[TMP_LEN];
    system("ezp-i2c fw");
    if((fp = fopen("/tmp/mcu_fw", "r"))) {
        fgets(tmp, sizeof(tmp), fp);
        websWrite(wp, "%s", tmp);
        fclose(fp);
    }
    return 0;
}

int
ej_show_wl_ifname_hwaddr(int eid, webs_t wp, int argc, char_t **argv)
{
    int if_idx, vif_idx;
    char tmp1[SHORT_BUF_LEN], tmp2[SHORT_BUF_LEN], hwaddr[SHORT_BUF_LEN];

    if (ejArgs(argc, argv, "%d %d", &if_idx, &vif_idx) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(tmp1, SHORT_BUF_LEN, "wl%d_ifname_rule", if_idx);
    ezplib_get_attr_val(tmp1, vif_idx, "ifname", tmp2, SHORT_BUF_LEN,
            EZPLIB_USE_CLI);

    if (!show_hwaddr(tmp2, hwaddr, SHORT_BUF_LEN)) {
        websWrite(wp, "%s", hwaddr);
        return 0;
    } else {
        return websWrite(wp, lang_translate("Not enabled"));
    }
}

int
ej_show_auth_if_hwaddr(int eid, webs_t wp, int argc, char_t **argv)
{
    int i, num;
    char *if_type, *ifname;
    char tmp1[SHORT_BUF_LEN], hwaddr[SHORT_BUF_LEN];

    if (ejArgs(argc, argv, "%s", &if_type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(tmp1, SHORT_BUF_LEN, "%s_num", if_type);
    num = atoi(nvram_safe_get(tmp1));
    for (i = 0; i < num; i++) {
        if (!strcmp(if_type, "wan")) {
            /* Because the default wan ifname of MR series is ppp0,
             * this will cause issue at authentication */
            snprintf(tmp1, SHORT_BUF_LEN, "%s%d_device", if_type, i);
        } else {
            snprintf(tmp1, SHORT_BUF_LEN, "%s%d_ifname", if_type, i);
        }
        ifname = nvram_safe_get(tmp1);
 
        if (!strncmp(ifname, "ppp", 3) || !strncmp(ifname, "br", 2)) {
            if (snprintf(tmp1, 16, "%s%d_device", if_type, i) >= 16)
                return -1;
            ifname = nvram_safe_get(tmp1);
        }
        if (!show_hwaddr(ifname, hwaddr, SHORT_BUF_LEN)) {
            websWrite(wp, 
                    "<input type=\"hidden\" name=\"ezp-%s%d\" value=\"%s\" />",
                    if_type, i, hwaddr);
            return 0;
        } else {
           return websWrite(wp, lang_translate("Not enabled"));
        }
    }

    return 0;
}

int
ej_show_auth_wl_hwaddr(int eid, webs_t wp, int argc, char_t **argv)
{
    int if_idx, vif_idx;
    char tmp1[SHORT_BUF_LEN], tmp2[SHORT_BUF_LEN], hwaddr[SHORT_BUF_LEN];
    FILE *file;

    if (ejArgs(argc, argv, "%d %d", &if_idx, &vif_idx) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if ((file = fopen("/proc/ezp_machine_id", "r"))) {
         fgets(hwaddr, sizeof(hwaddr), file);
         /* The MAC address should be XX:XX:XX:XX:XX:XX. Trim the unnecessary
          * \r\n derived from /proc/ezp_machine_id.
          */
         hwaddr[17] = '\0';
         fclose(file);
         return websWrite(wp, "%s", hwaddr);
     } else {
         snprintf(tmp1, SHORT_BUF_LEN, "wl%d_ifname_rule", if_idx);
         ezplib_get_attr_val(tmp1, vif_idx, "ifname", tmp2, SHORT_BUF_LEN,
                 EZPLIB_USE_CLI);
 
         if (!show_hwaddr(tmp2, hwaddr, SHORT_BUF_LEN)) {
             websWrite(wp, "%s", hwaddr);
             return 0;
         } else {
            return websWrite(wp, lang_translate("Not enabled"));
         }
     }
}

int
ej_show_wl_ifname_hwaddr_priv(int eid, webs_t wp, int argc, char_t **argv)
{
    int if_idx, vif_idx;
    char tmp1[SHORT_BUF_LEN], tmp2[SHORT_BUF_LEN], hwaddr[SHORT_BUF_LEN];
    FILE *file;

    if (ejArgs(argc, argv, "%d %d", &if_idx, &vif_idx) < 2) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(tmp1, SHORT_BUF_LEN, "wl%d_ifname_rule", if_idx);
    ezplib_get_attr_val(tmp1, vif_idx, "ifname", tmp2, SHORT_BUF_LEN,
            EZPLIB_USE_CLI);

    if ((file = fopen("/proc/ezp_machine_id", "r"))) {
        char *ptr; 
        int x, y;

        fgets(hwaddr, sizeof(hwaddr), file);
        /* The MAC address should be XX:XX:XX:XX:XX:XX. Trim the unnecessary
         * \r\n derived from /proc/ezp_machine_id.
         */
        hwaddr[17] = '\0';
        sscanf(&hwaddr[15], "%2X", &x);
        y = x >> 3;
        y = y << 3;

        /* Print out the modified wireless MAC address, and quoted the delta
         * value.. */
        ptr = &hwaddr[15];
        snprintf(ptr, 3, "%02X", y);
        fclose(file);
        return websWrite(wp, "%s(%d)", hwaddr, x - y);
    }
    return websWrite(wp, lang_translate("Not enabled"));
}

int
ej_b64_nvg_attr_get(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth, ret;
    char *tag, *rule_set, *attr, *val;
    char buf[TMP_LEN];
    if (ejArgs(argc, argv, "%s %s %d %s", &tag, &rule_set, &nth, &attr) < 4) {
        websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }

    ret = ezplib_get_attr_val(rule_set, nth, attr, buf, sizeof(buf), 
                EZPLIB_USE_WEB);
    if (ret < 0) {
        return ret;
    }
    val = buf;
    return websWrite(wp, val);
}

int
ej_nvg_attr_get(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth, ret;
    char *tag, *rule_set, *attr, *val;
    char buf[TMP_LEN], tmp[TMP_LEN] ={0};
    unsigned long n = TMP_LEN;

    if (ejArgs(argc, argv, "%s %s %d %s", &tag, &rule_set, &nth, &attr) < 4) {
        websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }

    val = GOZILA_GET(tag);
    if (!val) {
        ret = 
            ezplib_get_attr_val(rule_set, nth, attr, buf, TMP_LEN, 
                    EZPLIB_USE_CLI);
        if (ret < 0) {
            return ret;
        }
        val = buf;
    }
    if((!strcmp(rule_set, "wl0_apcli_swap_rule") && !strcmp(attr, "ssid")) || (!strcmp(rule_set, "wl0_apcli_rule") && !strcmp(attr, "ssid"))) {
        base64_decode((unsigned char *)buf, strlen(buf), (unsigned char *)tmp, &n);
        ui_parser(tmp);
        return websWrite(wp, "%s", tmp);
    }
    else
        return websWrite(wp, val);
}

int
ej_nvg_attr_match(int eid, webs_t wp, int argc, char_t **argv)
{
    char *tag, *rule_set, *attr, *match, *output, *selected;
    int nth;

    if (ejArgs(argc, argv, "%s %s %d %s %s %s", 
                &tag, &rule_set, &nth, &attr, &match, &output) < 6) {

        websDebugWrite(wp, "Insufficient args");
        return EZPLIB_INVALID;
    }

    selected = nvg_attr_match(tag, rule_set, nth, attr, match, output);
    if (selected) {
        websWrite(wp, selected);
    }
    return 0;
}

int
ej_tbl_get(int eid, webs_t wp, int argc, char **argv)
{
    char rule[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
    char *rule_set;
    int num; 
    /* 1-based. Attribute num is equal to the number of '^' plus 1. */
    int nattr = 1;  
    char *cur;
    int i, j, rlen;

    if (ejArgs(argc, argv, "%s", &rule_set) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    num = ezplib_get_rule_num(rule_set);

    websWrite(wp, "<script language='javascript'>\n");
    websWrite(wp, "var %s = ", rule_set);

    cur = tmp;
    *cur++ = '[';
    for (i = 0; i < num; i++) {
        rlen = ezplib_get_rule(rule_set, i, rule, LONG_BUF_LEN);
        /* The beginning of a rule to replace */
        cur += 
            snprintf(cur, tmp + LONG_BUF_LEN - cur, "%s[\"", 
                    (cur == tmp + 1) ? "" : ",");

        for (j = 0; j < rlen; j++) {
            if (rule[j] == '^') {
                cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "\",\"");
                nattr++;
            } else {
                *cur++ = rule[j];
            }
        }
        cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "\"]");

        /* Dump for every rule in case 'tmp' is overflowed. */
        websWrite(wp, "%s", tmp);
        cur = tmp;
    }
    cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "];\n");
    websWrite(wp, "%s", tmp);

    websWrite(wp, "</script>");


    return 0;
}

int
ej_tbl_create_table(int eid, webs_t wp, int argc, char **argv) {
    char *rname, *tname; 
    int mv;
    int max, nusr;
    char buf1[SHORT_BUF_LEN], buf2[SHORT_BUF_LEN];

    if (ejArgs(argc, argv, "%s %s %d", &tname, &rname, &mv) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(buf1, SHORT_BUF_LEN, "%s_max", rname);
    snprintf(buf2, SHORT_BUF_LEN, "%s_num", rname);
    max = atoi(nvram_safe_get(buf1));
    nusr = atoi(nvram_safe_get(buf2));

    websWrite(wp, "<div id='%s'></div>", tname);
    websWrite(wp, "<input type='hidden' id='%s_max_rule_num' value='%d' />", tname, max);
    websWrite(wp, "<input type='hidden' id='%s_cur_rule_num' value='%d' />", tname, nusr);
    websWrite(wp, "<input type='hidden' id='%s_rule_flag' value='1' />", tname);
    websWrite(wp, "<input type='hidden' id='%s_rule_no' value='1' />", tname);

#ifndef EZP_PROD_BRAND_AXIM
        websWrite(wp, "<input type='button' id='%s_add' ", tname);
        websWrite(wp, "onClick='tbl_show_input(\"%s\", 1, %s);' value='%s' />",
                      tname, rname, lang_translate("Add"));
        websWrite(wp, "<input disabled type='button' id='%s_delete' ", tname);
        websWrite(wp, "onClick='tbl_del_tr(\"%s\", %s);' value='%s' />",
                       tname, rname, lang_translate("Delete"));
        websWrite(wp, "<input disabled type='button' id='%s_modify' ", tname);
        websWrite(wp, "onclick='tbl_show_input(\"%s\", 0, %s);' value='%s' />",
                       tname, rname, lang_translate("Modify"));
    
        if (mv == 1) {
            websWrite(wp, "<input disabled type='button' id='%s_up' ", tname);
            websWrite(wp, "onClick='tbl_moveup_tr(\"%s\", %s);' value='%s' />", 
                          tname, rname, lang_translate("Up"));
            websWrite(wp, "<input disabled type='button' id='%s_down' ", tname);
            websWrite(wp, "onclick='tbl_movedown_tr(\"%s\", %s);' value='%s' />",
                          tname, rname, lang_translate("Down"));
            websWrite(wp, "<input type='hidden' id='%s_enable_up_down' "
                                "value='true' />", tname);
        } else {
            websWrite(wp, "<input type='hidden' id='%s_enable_up_down' "
                                "value='false' />", tname);
        }
#endif
    return 1;
}

int
ej_show_version(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *supercat, *type = NULL;
    supercat = nvram_safe_get("prod_supercat");
    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    if(!strlen(type)) {
        char *version = nvram_safe_get("custom_fw_version");
        if (!strcmp(version, "")) {
            websWrite(wp, "%s  ", nvram_safe_get("fw_version"));
            if(!strcmp(supercat, "")){
                websWrite(wp, "(%s.", ezp_prod_cat);
            } else {
                websWrite(wp, "(%s.", supercat);
            }
            if(!strcmp(ezp_prod_subsubcat, "")) {
                websWrite(wp, "%d)", ezp_prod_subcat);
            } else {
                websWrite(wp, "%d.%s)", ezp_prod_subcat, ezp_prod_subsubcat);
            }
        } else {
            websWrite(wp, "%s  ", version);
        }
    } else if (!strcmp(type, "custom")) {
        char *version = nvram_safe_get("custom_fw_version");
        if (!strcmp(version, "")) {
            websWrite(wp, "NONE  ");
        } else {
            websWrite(wp, "%s  ", version);
        }
    } else if (!strcmp(type, "factory")) {
        websWrite(wp, "%s  ", nvram_safe_get("fw_version"));
        if(!strcmp(supercat, "")){
            websWrite(wp, "(%s.", ezp_prod_cat);
        } else {
            websWrite(wp, "(%s.", supercat);
        }
        if(!strcmp(ezp_prod_subsubcat, "")) {
            websWrite(wp, "%d)", ezp_prod_subcat);
        } else {
            websWrite(wp, "%d.%s)", ezp_prod_subcat, ezp_prod_subsubcat);
        }
    }
    return 1;
}

int
ej_show_first_boot_time(int eid, webs_t wp, int argc, char_t ** argv)
{
#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_FV13XX)
    websWrite(wp, "60"); return 1;
#endif
}

int
ej_st_show_traceroute(int eid, webs_t wp, int argc, char_t ** argv)
{
    do_file("/tmp/traceroute.tmp", wp, NULL);
    return 1;
}

int
ej_st_show_atcommand(int eid, webs_t wp, int argc, char_t ** argv)
{
    do_file("/tmp/atcommand.tmp", wp, NULL);
    return 1;
}

int
ej_get_ipsec_status(int eid, webs_t wp, int argc, char_t ** argv)
{
    system("/usr/bin/vpnstatus");
    return 1;
}

int
ej_show_logo(int eid, webs_t wp, int argc, char_t ** argv)
{
#if !defined(EZP_LOGO_y)
    websWrite(wp, "//");
#endif
    return 1;
}

int
ej_show_copyright(int eid, webs_t wp, int argc, char_t ** argv)                 {
    if(strcmp(EZP_COPYRIGHT, "")) {
        char tmp[TMP_LEN];
        int i;
        strcpy(tmp, lang_translate(EZP_COPYRIGHT));
        for (i = 0; i < strlen(tmp); i++) {
            if (tmp[i] == '_')
                tmp[i] = ' ';
        }
        websWrite(wp, "W(\"<div style='font-size: 12px' />%s &copy; ", lang_translate("Copyright"));
        websWrite(wp," %s", lang_translate(tmp));                                                             
#ifdef PLATFORM_M2E
        websWrite(wp, "%s</div>\");", lang_translate("All rights reserved."));
#else
        websWrite(wp, "<br>%s</div>\");", lang_translate("All rights reserved."));
#endif
    }
    return 1;
}

#if defined (ENABLE_QCLOUD_ON_HTTPD)
int
ej_show_myqnapcloud_status(int eid, webs_t wp, int argc, char_t ** argv)
{
    //FILE *f_status, *f_dev_name, *f_qid;
    //char buf[TMP_LEN], buf2[TMP_LEN];
    
    char *wan0_ipaddr;
    int license_invalid;
    int qid_update_dev_info_ret;
    int qid_get_device_info_ret;
    char qcloud_account[TMP_LEN], qcloud_status[TMP_LEN], qcloud_device_id[TMP_LEN], qcloud_device_name[TMP_LEN];

    // memset(buf, 0, sizeof(buf));
    // memset(buf2, 0, sizeof(buf2));

    memset(qcloud_account, 0, sizeof(qcloud_account));
    memset(qcloud_status, 0, sizeof(qcloud_status));
    memset(qcloud_device_id, 0, sizeof(qcloud_device_id));
    memset(qcloud_device_name, 0, sizeof(qcloud_device_name));

    /*check internet*/ 
    wan0_ipaddr = nvram_safe_get("wan0_ipaddr");
    if (strcmp(wan0_ipaddr, "")) {
        websWrite(wp, "wan0_ipaddr = \"%s\";\n", wan0_ipaddr);
    }
    /*check license invalid*/
    license_invalid = atoi(nvram_safe_get("license_invalid"));
    websWrite(wp, "license_invalid = \"%d\";\n", license_invalid);


    /* refresh qcloud page action have qid_update_device_info and 
     * qid_get_device_info */
    qid_update_dev_info_ret = qid_update_device_info();
    qid_get_device_info_ret = qid_get_device_info(qcloud_account, qcloud_status, qcloud_device_id, qcloud_device_name); 

    /*check qid status*/
    if (strcmp(qcloud_status,"")) {
        websWrite(wp, "qid_status = \"%s\";\n", qcloud_status);
    } else {
        websWrite(wp, "qid_status = \"\";\n");
    }
    /* get_qid_account*/
    if (strcmp(qcloud_account,"")) {
        websWrite(wp, "qcloud_qid = \"%s\";\n", qcloud_account);
    } else {
        websWrite(wp, "qcloud_qid = \"\";\n");
    }
    /*get_qid_dev_name*/
    if (strcmp(qcloud_device_name,"")) {
        websWrite(wp, "qcloud_dev_name = \"%s\";\n", qcloud_device_name);
    } else {
        websWrite(wp, "qcloud_dev_name = \"\";\n");
    }

    /* get upnpc status rule outport */
    char qcloud_get_web_mgmt_outport[TMP_LEN], qcloud_get_file_station_outport[TMP_LEN];
    ezplib_get_attr_val("upnpc_status_rule", 0, "outport", qcloud_get_web_mgmt_outport, sizeof(qcloud_get_web_mgmt_outport), EZPLIB_USE_CLI);
    ezplib_get_attr_val("upnpc_status_rule", 1, "outport", qcloud_get_file_station_outport, sizeof(qcloud_get_file_station_outport), EZPLIB_USE_CLI);
    websWrite(wp, "qcloud_get_web_mgmt_outport = \"%s\";\n", qcloud_get_web_mgmt_outport);
    websWrite(wp, "qcloud_get_file_station_outport = \"%s\";\n", qcloud_get_file_station_outport);
    
    /* get fetch_result of last time */
    char qcloud_test_fetch_result_web_mgmt[TMP_LEN], qcloud_test_fetch_result_file_station[TMP_LEN];
    ezplib_get_attr_val("upnpc_status_rule", 0, "fetch_result", qcloud_test_fetch_result_web_mgmt, sizeof(qcloud_test_fetch_result_web_mgmt), EZPLIB_USE_CLI);
    ezplib_get_attr_val("upnpc_status_rule", 1, "fetch_result", qcloud_test_fetch_result_file_station, sizeof(qcloud_test_fetch_result_file_station), EZPLIB_USE_CLI);
    websWrite(wp, "qcloud_test_fetch_result_web_mgmt = \"%s\";\n", qcloud_test_fetch_result_web_mgmt);
    websWrite(wp, "qcloud_test_fetch_result_file_station = \"%s\";\n", qcloud_test_fetch_result_file_station);
    
    /*pre qid update info*/
    //system("qcloud_sample update_device_info | grep update_ret | awk '{print $2}' > /tmp/.qid_action_update_info");
    /*check qid status*/    
   /* system("qcloud_sample get_device_info | grep qid_status | awk '{print $2}' > /tmp/.qid_status");    
    system("qcloud_sample get_device_info | grep device_name | awk '{print $2}' > /tmp/.qid_device_name");    
    system("qcloud_sample get_device_info | grep qid_account | awk '{print $2}' > /tmp/.qid_account");    
    */
    /* get_qid_status*/
   /* if ((f_status = fopen("/tmp/.qid_status", "r"))) {
        if (fgets(buf,sizeof(buf), f_status) != NULL) {
            strncpy(buf2, buf, strlen(buf)-1);
            websWrite(wp, "qid_status = \"%s\";\n", buf2);
        }
    fclose(f_status);
    }*/

    /*get_qid_device*/
   /* if ((f_dev_name = fopen("/tmp/.qid_device_name", "r"))) {
        if (fgets(buf,sizeof(buf), f_dev_name) != NULL) {
            strncpy(buf2, buf, strlen(buf)-1);
            if (strcmp(buf2,"")) {
                websWrite(wp, "var qcloud_dev_name = \"%s\";\n", buf2);
            } 
        }
    fclose(f_dev_name);
    }*/

    /*get_qid_accont*/
    /*if ((f_qid= fopen("/tmp/.qid_account", "r"))) {
        if (fgets(buf,sizeof(buf), f_qid) != NULL) {
            strncpy(buf2, buf, strlen(buf)-1);
            if (strcmp(buf2,"")) {
                websWrite(wp, "var qcloud_qid = \"%s\";\n", buf2);
            } 
        }
    fclose(f_qid);
    }*/

    return 1;
}
#endif


struct ej_handler ej_handlers[] = {
    /* for all */
    {"lang", ej_lang},
    {"increase", ej_increase},
#ifdef EZP_PROD_BRAND_ZYUS
    {"increase_org", ej_increase_org},
#endif
#ifdef EZP_PROD_BRAND_PROX
    {"increase_wan", ej_increase_wan},
#endif
    {"nv_get", ej_nv_get},
    {"nv_attr_get", ej_nv_attr_get},
    {"nvg_attr_get", ej_nvg_attr_get},
    {"b64_nvg_attr_get", ej_b64_nvg_attr_get},
    {"nvg_attr_match", ej_nvg_attr_match},
    {"nv_match", ej_nv_match},
    {"nv_invmatch", ej_nv_invmatch},
    {"nv_else_match", ej_nv_else_match},
    {"nvg_get", ej_nvg_get},
    {"nvg_match", ej_nvg_match},
    {"nvg_else_match", ej_nvg_else_match},
    {"prod_show_js_start", ej_prod_show_js_start},
    {"prod_show_js_end", ej_prod_show_js_end},
    {"prod_show_js", ej_prod_show_js},
    {"prod_show_html", ej_prod_show_html},
    {"prod_show_html_start", ej_prod_show_html_start},
    {"prod_show_html_end", ej_prod_show_html_end},
    {"prod_comment_js_start", ej_prod_comment_js_start},
    {"prod_comment_js_end", ej_prod_comment_js_end},
    {"prod_comment_html_start", ej_prod_comment_html_start},
    {"prod_comment_html_end", ej_prod_comment_html_end},
    
    /* for GUI  */
    {"do_menu",ej_do_menu},
    {"do_pagehead1",ej_do_pagehead1}, 
    {"do_pagehead2",ej_do_pagehead2},
    {"do_bootstrap_menu",ej_do_bootstrap_menu},
    {"do_bootstrap_navbar_menu",ej_do_bootstrap_navbar_menu},
    {"do_headmeta",ej_do_headmeta},
    {"do_basic_css", ej_do_basic_css},
    {"do_ace_font_css", ej_do_ace_font_css},
    {"do_ace_font_ie7_css", ej_do_ace_font_ie7_css},
    {"do_ace_basic_css", ej_do_ace_basic_css},
    {"do_ace_basic_ie9_css", ej_do_ace_basic_ie9_css},
    {"do_basic_js", ej_do_basic_js},
    {"do_custom_css", ej_do_custom_css}, 
    {"do_custom_js", ej_do_custom_js},
    {"do_datatables_css", ej_do_datatables_css}, 
    {"do_datatables_js", ej_do_datatables_js},
    {"do_jquery_ui_js", ej_do_jquery_ui_js},
    {"do_jquery_other_js", ej_do_jquery_other_js},
    {"do_jquery_flot_js", ej_do_jquery_flot_js},    
    {"do_ace_basic_js", ej_do_ace_basic_js},

    {"do_bootstrap_modal", ej_do_bootstrap_modal},

    {"do_print_begin", ej_do_print_begin},
    {"do_print_end", ej_do_print_end},

    {"show_first_boot_time", ej_show_first_boot_time},
    {"show_logo", ej_show_logo},
    {"show_copyright", ej_show_copyright},
#if defined (ENABLE_QCLOUD_ON_HTTPD)
    {"show_myqnapcloud_status", ej_show_myqnapcloud_status},
#endif
    
    /* for table */
    {"tbl_get", ej_tbl_get},
    {"tbl_create_table", ej_tbl_create_table},

    /* EZP: Config validation and config applying. */
    {"show_valid_msg", ej_show_valid_msg},
    {"show_control", ej_show_control},
    {"show_next_page", ej_show_next_page},
    {"show_version", ej_show_version},
    {"show_license_status", ej_show_license_status},
    {"show_localtime", ej_show_localtime},
    {"show_uptime", ej_show_uptime},
    {"show_ipaddr", ej_show_ipaddr},
    {"show_ipaddr_prefix24", ej_show_ipaddr_prefix24},
    {"show_ipaddr_prefix24_calc", ej_show_ipaddr_prefix24_calc},
    {"show_hwaddr", ej_show_hwaddr},
    {"show_eeprom_hwaddr", ej_show_eeprom_hwaddr},
    {"show_wl_region", ej_show_wl_region},
    {"show_wl_band", ej_show_wl_band},
    {"show_tx_channel", ej_show_tx_channel},
    {"show_freq_off", ej_show_freq_off},
    {"show_uboot", ej_show_uboot},
    {"show_build_time", ej_show_build_time},
    {"show_gauge_fw", ej_show_gauge_fw},
    {"show_mcu_fw", ej_show_mcu_fw},
    {"show_auth_if_hwaddr", ej_show_auth_if_hwaddr},
    {"show_auth_wl_hwaddr", ej_show_auth_wl_hwaddr},
    {"show_wl_ifname_hwaddr", ej_show_wl_ifname_hwaddr},
    {"show_wl_ifname_hwaddr_priv", ej_show_wl_ifname_hwaddr_priv},
    {"st_show_battery_status", ej_st_show_battery_status},
    {"st_show_charge_status", ej_st_show_charge_status},
    {"st_show_storage_status", ej_st_show_storage_status},
    {"show_server_ip", ej_show_server_ip},

    /* for WAN */
    {"wan_show_setting", ej_wan_show_setting},
    {"wwan_show_setting", ej_wwan_show_setting},
    {"pppoe_show_setting", ej_pppoe_show_setting},
    {"static_show_setting", ej_static_show_setting},
    {"wan_show_proto_type", ej_wan_show_proto_type},
    {"wan_show_type_setting", ej_wan_show_type_setting},
    {"wan_show_vpnc_type_setting", ej_wan_show_vpnc_type_setting},
#ifdef EZP_SUB_BRAND_APOLLO
    {"wan_show_wisp_type_setting", ej_wan_show_wisp_type_setting},
#endif
#if defined (EZP_SUB_BRAND_SONY) || defined(EZP_SUB_BRAND_GARMIN) || defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    {"wan_show_wisp_swap_list", ej_wan_show_wisp_swap_list},
#endif
    {"wan_wwan_show_modem", ej_wan_wwan_show_modem},
    {"wan_wwan_create_modem_array", ej_wan_wwan_create_modem_array},
    {"wan_wwan_show_isp", ej_wan_wwan_show_isp},
    {"wan_barry_show_isp", ej_wan_barry_show_isp},
    {"wan_wwan_create_isp_array", ej_wan_wwan_create_isp_array},
    {"wan_create_proto_array", ej_wan_create_proto_array},
    {"wl_show_radius_html_start", ej_wl_show_radius_html_start},
    {"wl_show_radius_html_end", ej_wl_show_radius_html_end},
    {"wan_show_wan_type_html_start", ej_wan_show_wan_type_html_start},
    {"wan_show_wan_type_html_end", ej_wan_show_wan_type_html_end},
#ifdef EZP_PROD_BRAND_ZYUS 
    {"wwan_show_wan_type_html_start", ej_wwan_show_wan_type_html_start},
    {"wwan_show_wan_type_html_end", ej_wwan_show_wan_type_html_end},
#endif


    /* for Wizard */
    {"wizard_load_isp_data", ej_wizard_load_isp_data},
    {"wizard_switch_isp", ej_wizard_switch_isp},
    {"wizard_load_bandwidth_data", ej_wizard_load_bandwidth_data},
    {"wizard_vpn_show_num", ej_wizard_vpn_show_num},
    {"wizard_vpn_show_max", ej_wizard_vpn_show_max},
    {"wizard_get_lan_mask", ej_wizard_get_lan_mask},
    {"wizard_title", ej_wizard_title},

    /* for LAN */
    {"lan_show_setting", ej_lan_show_setting},
    {"lan_show_subnet", ej_lan_show_subnet},

    /* for DHCP */
    {"dhcp_show_setting", ej_dhcp_show_setting},

    /* for OpenDNS */
    {"opendns_show_setting", ej_opendns_show_setting},

    /* for WAN Detect */
    {"wan_detect_show_setting", ej_wan_detect_show_setting},

    /* for DDNS */
    {"ddns_show_setting", ej_ddns_show_setting},

    /* for hwaddr clone */
    {"clone_show_setting", ej_clone_show_setting},
    {"clone_show_hwaddr", ej_clone_show_hwaddr},

    /* for iTunes */
    {"itunes_show_setting",ej_itunes_show_setting},

    /* for DBM */
    {"bw_show_setting", ej_bw_show_setting},
    {"bw_show_bw_setting", ej_bw_show_bw_setting},
    {"bw_show_rate_setting", ej_bw_show_rate_setting},
    {"bw_show_adv_bw_type", ej_bw_show_adv_bw_type},
    {"bw_show_resv", ej_bw_show_resv},

    /* for load balance */
    {"lb_legend", ej_lb_legend},
    {"lb_show_available", ej_lb_show_available},
    {"lb_show_setting", ej_lb_show_setting},

    /* for port forwarding */
    {"fr_show_setting", ej_fr_show_setting},
    
    /* for wireless */
    {"wl_show_setting", ej_wl_show_setting},
    /* for wireless WDS */
    {"wl_wds_show_setting", ej_wl_wds_show_setting},
    {"wlv_wds_sec_show_setting", ej_wlv_wds_sec_show_setting},
    /* for wireless APCLI */
    {"apcli_fieldset_need", ej_apcli_fieldset_need},
    {"wl_apcli_show_setting", ej_wl_apcli_show_setting},
    {"wlv_single_ssid_show_mark_begin", ej_wlv_single_ssid_show_mark_begin},
    {"wlv_single_ssid_show_mark_end", ej_wlv_single_ssid_show_mark_end},
    {"wlv_apcli_sec_show_setting", ej_wlv_apcli_sec_show_setting},
    {"wlv_no_11n_show_mark_begin", ej_wlv_no_11n_show_mark_begin},
    {"wlv_no_11n_show_mark_end", ej_wlv_no_11n_show_mark_end},
    /* for wireless multiple SSID */
    {"wlv_sec_show_setting", ej_wlv_sec_show_setting},
    {"wlv_show_entry_setting", ej_wlv_show_entry_setting},
    /* for status */
    {"togo_show_wan_proto", ej_togo_show_wan_proto},
    {"st_show_wan_setting", ej_st_show_wan_setting},
    {"st_show_wan_setting_bytype", ej_st_show_wan_setting_bytype},
    {"st_show_wan_priv_setting", ej_st_show_wan_priv_setting},
    {"st_show_lan_setting", ej_st_show_lan_setting},
    {"st_show_lan_priv_setting", ej_st_show_lan_priv_setting},
    {"st_show_wl_setting", ej_st_show_wl_setting},
    {"st_show_wl_priv_setting", ej_st_show_wl_priv_setting},
    {"st_user_show_dhcp_user_num", ej_st_user_show_dhcp_user_num},
    {"st_user_show_dhcp", ej_st_user_show_dhcp},
    {"st_user_show_arp_user_num", ej_st_user_show_arp_user_num},
    {"st_user_show_arp", ej_st_user_show_arp},
    {"st_mrtg_show", ej_st_mrtg_show},
    {"st_show_period", ej_st_show_period},
    {"st_log_show_system", ej_st_log_show_system},
    {"st_log_show_class", ej_st_log_show_class},
    {"st_log_show_iptables", ej_st_log_show_iptables},
    {"st_show_if", ej_st_show_if},
    {"st_show_traceroute", ej_st_show_traceroute},
    {"st_show_atcommand", ej_st_show_atcommand},
    {"get_ezp_machine_id", ej_get_ezp_machine_id},

    /* for widget */
    {"widget_execute", ej_widget_execute},
    {"widget_show", ej_widget_show},
#ifdef EZP_PROD_BRAND_PROX
    {"widget_show_login", ej_widget_show_login},
#endif
    {"widget_show_signal", ej_widget_show_signal},
    {"widget_start", ej_widget_start},

    /* for BT */
    {"bt_show_content", ej_bt_show_content},

    /* for vlan */
    {"vlan_show_content", ej_vlan_show_content},
    {"vlan_js", ej_vlan_js},

    /* for admin tools */
    {"ad_tools_showif", ej_ad_tools_showif},

    /* for X mode switch */
    {"ad_x_mode_switch", ej_ad_x_mode_switch},

    /* for wan/lan mode switch */
    {"ad_vlanport_config", ej_ad_vlanport_config},
    {"get_ipsec_status", ej_get_ipsec_status},

#ifdef EZPACKET
    /* for test */
    {"diag_ping", ej_diag_ping},
    {"diag_traceroute", ej_diag_traceroute},
#endif
    {NULL, NULL}
};
