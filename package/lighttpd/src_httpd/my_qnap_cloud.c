#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include "ezp.h"
#include "ezp-lib.h"
#include "lang.h"

#include "fcgi_common.h"
#include "fcgi_stdio.h"
#include "http_common.h"

#include "qcloud.h"
#include "qid_api.h"
#include "mcn_api.h"
#include <bcmnvram.h>

enum {
    MYQNAPCLOUD = 0,
    MYQNAPCLOUD_DEV,
};

static struct variable exp_qcloud_variables[] = {
    {longname: "myQNAPcloud user name or password", argv:ARGV("")},
    {longname: "myQNAPcloud device name ", argv:ARGV("")},
};

int     
valid_qcloud_dev(INPUT *input, char *value, struct variable *v)
{  
    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));    
    int qid_available_dev_name_ret;
    char *qcloud_device_name;
    INPUT *tmp;
    
    if ((tmp = CGI_Find_Parameter(input, "qcloud_device_name"))) {
        qcloud_device_name = tmp->val;
    }
    else
        qcloud_device_name = "";

    qid_available_dev_name_ret = qid_available_dev_name(qcloud_device_name);
    
    if (qid_available_dev_name_ret != QCLOUD_NAME_FREE) {
        printf("message=%s <br> (%s : %d) <br>\n", lang_translate("The device name has been registered."), lang_translate("Error code"),qid_available_dev_name_ret);
        return FALSE;
    }

    return TRUE;
}

int
valid_qcloud(INPUT *input, char *value, struct variable *v)
{
    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
    int qid_signin_ret; 
    char *qcloud_qid, *qcloud_pw, *wan0_ipaddr;

    INPUT *tmp;

    if((tmp = CGI_Find_Parameter(input, "qcloud_qid"))) {
        qcloud_qid = tmp->val;
    }
    else
        qcloud_qid = "";

    if((tmp = CGI_Find_Parameter(input, "qcloud_pw"))) {
        qcloud_pw = tmp->val;
    }
    else
        qcloud_pw = "";

    /* check internet */
    wan0_ipaddr = nvram_safe_get("wan0_ipaddr");
    if (!strcmp(wan0_ipaddr, "")) {
        printf("message=%s <br>%s\n", lang_translate("Cannot access myQNAPcloud."), lang_translate("Please check your Internet Connection."));

        return FALSE;
     }  
    qid_signin_ret = qid_signin(qcloud_qid, qcloud_pw);
    
    if (qid_signin_ret == -11) {
        printf("message=%s <br>%s <br>(%s : %d)<br>\n", lang_translate("Cannot access myQNAPcloud."), lang_translate("Please check your Internet Connection."),  lang_translate("Error code"), qid_signin_ret);
        return FALSE;

    } else if (qid_signin_ret != 0 ) {  //1012005
        printf("message=%s <br> (%s : %d) <br>\n", lang_translate("Incorrect user name or password."), lang_translate("Error code"),qid_signin_ret);
        return FALSE;
    } 
    return TRUE;
}

int
valid_exp_myqnapcloud(INPUT *input, char *value, struct variable *v)
{
    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
    char *qcloud_qid;
    char *qcloud_device_name;
    char *btn_to_sign;
    char submit_button_action_in[] = "btn-to-signin";
    char get_qcloud_signin_stats[256];
    ezplib_get_attr_val("myqnapcloud_id_rule", 0, "qid", get_qcloud_signin_stats, sizeof(get_qcloud_signin_stats), EZPLIB_USE_CLI);

    INPUT *tmp;
    /*get post value*/
    if ((tmp = CGI_Find_Parameter(input, "qcloud_qid"))) {
        qcloud_qid = tmp->val;
    }
    else
        qcloud_qid = "";

    if ((tmp = CGI_Find_Parameter(input, "qcloud_device_name"))) {
        qcloud_device_name = tmp->val;
    }
    else
        qcloud_device_name = "";

    if ((tmp = CGI_Find_Parameter(input, "submit_button"))) {
        btn_to_sign = tmp->val;
    }
    else
        btn_to_sign = "";

    char *wan0_ipaddr;
    wan0_ipaddr = nvram_safe_get("wan0_ipaddr");
    if (!strcmp(wan0_ipaddr, "")) {
        printf("message=%s <br>%s\n", lang_translate("Cannot access myQNAPcloud."), lang_translate("Please check your Internet Connection."));
        return FALSE;
    }
    if (!(strcmp(submit_button_action_in, btn_to_sign))){ //signin 
      if(!strcmp(get_qcloud_signin_stats,"" )) {   
        if (valid_qcloud(input, qcloud_qid, &exp_qcloud_variables[MYQNAPCLOUD]) == FALSE) { //check_qid_account_pw
                return FALSE;
            }   
      }
        //qid_account is null
        if (valid_qcloud_dev(input, qcloud_device_name, &exp_qcloud_variables[MYQNAPCLOUD_DEV]) == FALSE) { //check_device
            return FALSE;
        }
    }
    return TRUE;
}

int
save_exp_myqnapcloud(INPUT *input, char *value, struct variable *v, struct service *s)
{   
    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
    char tmp[TMP_LEN]; 
    int64_t map = 0;

    int change = 0;
    char *qcloud_device_name;
    char *submit_button_action; 
    char submit_button_action_in[] = "btn-to-signin";
    char submit_button_action_out[] = "btn-to-signout";
    char submit_button_action_test_portal[] = "qcloud_test_portal";
    char submit_button_action_diagnostic[] = "qcloud_diagnostic";

    int qid_bind_dev_name_ret;
    //int qid_update_device_info_ret;
    int qid_unbind_dev_name_ret;
    int qid_publish_service_ret;
    int qid_update_service_port_by_upnpc_ret;
    int qid_btn_test_fetch_action_ret;
    int qid_update_dev_info_ret;
    int reset_qcloud_request_hash_ret;
    int qid_check_dev_info_ret;

    INPUT *tmp_val;

    char *wan0_ipaddr;
    wan0_ipaddr = nvram_safe_get("wan0_ipaddr");

    if (!strcmp(wan0_ipaddr, "")) {
        printf("message=%s <br>%s\n", lang_translate("Cannot access myQNAPcloud."), lang_translate("Please check your Internet Connection."));
        return FALSE;
    }

    if ((tmp_val = CGI_Find_Parameter(input, "qcloud_device_name"))) {
        qcloud_device_name = tmp_val->val;
    }
    else
        qcloud_device_name = "";

    if ((tmp_val = CGI_Find_Parameter(input, "submit_button"))) {
        submit_button_action = tmp_val->val;
    }
    else
        submit_button_action = "";

    qid_check_dev_info_ret = qid_check_device_info();
    if (!strcmp(submit_button_action_in, submit_button_action)) { //signin bind device
         qid_bind_dev_name_ret = qid_bind_dev_name(qcloud_device_name);
         if (qid_bind_dev_name_ret == 0) { //
            qid_publish_service_ret = qid_publish_service();//qid publish service
            qid_update_service_port_by_upnpc_ret = qid_update_service_port_by_upnpc();//qid servic port by upnpc 
            
            if( qid_update_service_port_by_upnpc_ret == -967 ) {
                       qid_update_service_port_by_upnpc_ret = qid_update_service_port_by_upnpc();//update_service_port failed
                    if(qid_update_service_port_by_upnpc_ret == 0) {
                        change = 1 ;
                    }
            }
                    change = 1 ;
         }
    } else if (!strcmp(submit_button_action_out, submit_button_action)) {   //signed-out
        reset_qcloud_request_hash_ret = reset_qcloud_request_hash();
        qid_unbind_dev_name_ret = qid_unbind_dev_name();
        change = 1;
    } else if (!strcmp(submit_button_action_test_portal, submit_button_action)) {  //test_fetch
        reset_qcloud_request_hash_ret = reset_qcloud_request_hash();
        qid_btn_test_fetch_action_ret = qid_btn_test_fetch_action();
        change = 1;
    } else if (!strcmp(submit_button_action_diagnostic, submit_button_action)) {   //diagnostic
            reset_qcloud_request_hash_ret = reset_qcloud_request_hash();
            qid_publish_service_ret = qid_publish_service();//qid publish service
            qid_update_service_port_by_upnpc_ret = qid_update_service_port_by_upnpc();//qid servic port by upnpc 
            //change = 1;
            if( qid_update_service_port_by_upnpc_ret == 0 ) {
                    qid_update_dev_info_ret = qid_update_device_info();
                    qid_btn_test_fetch_action_ret = qid_btn_test_fetch_action();
                    change = 1 ;
            } else {
                    qid_update_service_port_by_upnpc_ret = qid_update_service_port_by_upnpc();//update_service_port failed
                    if(qid_update_service_port_by_upnpc_ret == 0) {
                        qid_btn_test_fetch_action_ret = qid_btn_test_fetch_action();
                        change = 1 ;
                    }
            }

    }
        if (change) {
            snprintf(tmp, TMP_LEN, "NUM=0");
            config_postaction(map, s, tmp, "");
        }
    return change;
}

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

int qid_get_device_info(char *qid_account, char *qid_status, char *device_id, char *device_name, char *last_update_time, int *last_update_code_ret)
{
    int ret = QID_ERR_NO_ERROR;

    //for converting timestamp to local time
    struct tm ts;
    char time_str_buf[TMP_LEN];

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
        sprintf(buf, "%s", dev_info.qid_account);
        strncpy(qid_account, buf, strlen(buf));
       }

        //print device id
        
        if (strcmp(dev_info.device_id, "")) {
        sprintf(buf, "%s", dev_info.device_id);
        strncpy(device_id, buf, strlen(buf));
        }
        
        //print device name
       if (strcmp(dev_info.device_name, "")) {
        sprintf(buf, "%s", dev_info.device_name);
        strncpy(device_name, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
       }

       // check last update error code
      if ( dev_info.last_update_error_code == QID_ERR_NO_ERROR ) {
          localtime_r(&dev_info.last_update_success_timestamp, &ts);
          strftime(time_str_buf, sizeof(time_str_buf), "%Y-%m-%d %H:%M:%S", &ts);
          sprintf(buf, "%s", time_str_buf);
          strncpy(last_update_time, buf, strlen(buf));
          memset(buf, 0, sizeof(buf));
      } else {
          localtime_r(&dev_info.last_update_fail_timestamp, &ts);
          strftime(time_str_buf, sizeof(time_str_buf), "%Y-%m-%d %H:%M:%S", &ts);
          sprintf(buf, "%s", time_str_buf);
          strncpy(last_update_time, buf, strlen(buf));
          memset(buf, 0, sizeof(buf));
      }
    
       *last_update_code_ret = dev_info.last_update_error_code;

    }


    //cleanup ctx
    qcloud_cleanup_ctx(ctx);
    return ret;

}

int qid_signin(char* qid_id, char* qid_pw)
{
    int ret = QID_ERR_NO_ERROR;
    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));
    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //sign in
    ret = qnap_id_signin(ctx, qid_id, qid_pw);

    //cleanup ctx
     qcloud_cleanup_ctx(ctx);
     return ret;
}

int qid_available_dev_name(char* device_name)
{
    int function_ret = 0;

    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));
    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //check available
    function_ret = qnap_id_is_device_name_available(ctx, device_name);
    
    // cleanup ctx
    qcloud_cleanup_ctx(ctx);

    return function_ret;
}

int qid_bind_dev_name(char* device_name)
{
    int ret = QID_ERR_NO_ERROR;
    //init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
    //init ctx
    struct qcloud_st* ctx = qcloud_init_ctx(&info);

    //bind 
    ret = qnap_id_bind_device(ctx, device_name);

    // cleanup ctx
    qcloud_cleanup_ctx(ctx);

    return ret;
}

int qid_publish_service()
{
    int ret = QID_ERR_NO_ERROR;

    // init app info
    qnap_app_info info;
    memset(&info, 0, sizeof(qnap_app_info));

    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
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
            "/dav.asp",
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

int qid_update_service_port(uint16_t management_external_port, uint16_t file_external_port) {
    int ret = QID_ERR_NO_ERROR;

    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
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

int qid_update_service_port_by_upnpc(){

    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
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

int qid_check_device_info() {
        int ret = QID_ERR_NO_ERROR;

        // init app info
        qnap_app_info info;
        memset(&info, 0, sizeof(qnap_app_info));

        // init ctx
        struct qcloud_st* ctx = qcloud_init_ctx(&info);

        // check dev info
        ret = mycloudNAS_device_get_info(ctx);
       // printf("%d \n", ret);

        // cleanup ctx
        qcloud_cleanup_ctx(ctx);
        return ret;
}

int qid_fetch_web_page(const char* protocol, uint16_t external_port, const char* path) {
    int ret = QID_ERR_NO_ERROR;
//#if 0
    char buf_path[TMP_LEN];
    memset(buf_path, 0, sizeof(buf_path));
    if (!strcmp(path,"deviceid")) {
        sprintf(buf_path, "/fslite/%s", path);
    } else {
        sprintf(buf_path, "/%s", path);
    }
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
    char q_dir[TMP_LEN]={0};
    char q_js_path[TMP_LEN]={0};
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
    sprintf(buf, "nvram get wl0_ssid_rule > %s/deviceid", ROOT_PATH);//for web_mgmt
    system(buf);
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "nvram get wl0_ssid_rule > %s/deviceid.js", MYQNAPCLOUD_UPNP_PATH);//
    system(buf);
    /* checked fetch web_mgmt and file_station file content*/
    sprintf(q_js_path, "%s/deviceid.js", MYQNAPCLOUD_UPNP_PATH);
    if ((f_deviceid_js = fopen(q_js_path, "r"))) {  //web_mgmt
        if (fgets(buf2,sizeof(buf2), f_deviceid_js) != NULL) {
            strncpy(buf_deviceid_js_file, buf2, strlen(buf2)-1);
            if (!strcmp(get_nvram_ssid_val, buf_deviceid_js_file)) {
            }
        }
        fclose(f_deviceid_js);
        qid_btn_test_fetch_port_web_mgmt_ret = 1;
    }

    sprintf(q_dir, "%s/deviceid", ROOT_PATH);
    if ((f_deviceid = fopen(q_dir, "r"))) {   //file_station
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
            if ((check_fetch_web_mgmt_ret = fopen("/tmp/.fetch_deviceid.js_log", "r"))){
                if (fgets(buf1,sizeof(buf1), check_fetch_web_mgmt_ret) != NULL) {
                    strncpy(check_web_mgmt_result, buf1, strlen(buf1)-1);
                    if (!strcmp(buf_deviceid_js_file, check_web_mgmt_result)) { //fetch success replace to nvram 
                        tag_fetch_web_mgmt_result = 1;
                        ezplib_replace_attr("upnpc_status_rule" , 0, "fetch_result", "1");
                    }
                }
                fclose(check_fetch_web_mgmt_ret);
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

