#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "qcloud.h"
#include "qid_api.h"
#include "mcn_api.h"

#if LOCAL_TEST

/* for local testing, not include nvram headers */

#else /* LOCAL_TEST */

#include "ezp-lib.h"
#include <bcmnvram.h>

#endif /* LOCAL_TEST */

int signin(char* id, char* pw)
{
  int ret = QID_ERR_NO_ERROR;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // sign in
  ret = qnap_id_signin(ctx, id, pw);
  if( ret == QID_ERR_NO_ERROR) {
      printf("qid_and_pw: correct\n");
  } else {
      printf("qid_and_pw: incorrect\n");
  }

  printf("signin_ret:%d\n",ret);

  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int available(char* device_name)
{
  int ret = QID_ERR_NO_ERROR;
  int function_ret = 0;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // check available
  function_ret = qnap_id_is_device_name_available(ctx, device_name);
  if( function_ret == QCLOUD_NAME_FREE )
    printf("device_name: available\n");
  else if( function_ret == QCLOUD_NAME_BEEN_USED )
    printf("device_name: no_available\n");
  else
    printf("device_name: no_available\n");
printf("available_ret:%d\n",ret);

  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int bind(char* device_name)
{
  int ret = QID_ERR_NO_ERROR;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // bind
  ret = qnap_id_bind_device(ctx, device_name);
printf("bind_ret:%d\n",ret);

  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int unbind()
{
  int ret = QID_ERR_NO_ERROR;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  ret = qnap_id_unbind_device(ctx);
printf("unbind_ret:%d\n",ret);

  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int publish_service()
{
  int ret = QID_ERR_NO_ERROR;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // prepare service list
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

  // update publis service
  ret = mycloudNAS_update_published_service(
    ctx,
    ddns_service_tlb,
    2);

printf("published_service_ret:%d\n",ret);
  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int update_service_port(uint16_t management_external_port, uint16_t file_external_port)
{
  int ret = QID_ERR_NO_ERROR;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // prepare service list
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

  // update service port
  ret = mycloudNAS_update_service_port(
    ctx,
    port_mapping_tlb,
    2);

printf("update_service_port_ret:%d\n",ret);

  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int update_service_port_by_upnpc()
{
  int ret = QID_ERR_NO_ERROR;

#if LOCAL_TEST

  // this function is not supported in local test
  printf("this function %s is not supported in local test", __FUNCTION__);

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
  ret = update_service_port(external_ports[0], external_ports[1]);

printf("update_service_port_by_upnpc_ret:%d\n",ret);

#endif /* LOCAL_TEST */

  return ret;
}

int get_device_info()
{
  int ret = QID_ERR_NO_ERROR;

  // for converting timestamp to local time
  struct tm ts;
  char time_str_buf[80];

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // init qnap dev infor
  qnap_dev_info dev_info;
  memset(&dev_info, 0, sizeof(qnap_dev_info));

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // get dev info
  ret = qcloud_get_device_info(ctx, &dev_info);

  // if ok, print device info
  if( ret == QID_ERR_NO_ERROR ) {
    // print qid
    printf("qid_account: %s\n", dev_info.qid_account);

    // print qid status
    if( dev_info.qid_status[0] == '3' )
        printf("qid_status: sign-in\n");
    else
        printf("qid_status: sign-out\n");

    // print device id
    printf("device_id: %s\n", dev_info.device_id);

    // print device name
    printf("device_name: %s\n", dev_info.device_name);

    // check last update error code
    if( dev_info.last_update_error_code == QID_ERR_NO_ERROR )
    {
      localtime_r(&dev_info.last_update_success_timestamp, &ts);
      strftime(time_str_buf, sizeof(time_str_buf), "%Y-%m-%d %H:%M:%S", &ts);
      //printf("last update success on %s\n", time_str_buf);
      printf("last_update_status: success\n");
      printf("last_update_time:%s\n", time_str_buf);
      printf("update_error_code %d\n", dev_info.last_update_error_code);
    }
    else
    {
      localtime_r(&dev_info.last_update_fail_timestamp, &ts);
      strftime(time_str_buf, sizeof(time_str_buf), "%Y-%m-%d %H:%M:%S", &ts);
      //printf("last update failed with error code %d on %s\n", dev_info.last_update_error_code, time_str_buf);
      printf("last_update_status: failed\n");
      printf("last_update_time: %s\n", time_str_buf);
      printf("update_error_code %d\n", dev_info.last_update_error_code);
    }
  }
  printf("get_info_dev_ret: %d\n", ret) ;
  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int update_device_info()
{
  int ret = QID_ERR_NO_ERROR;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // report period (not used)
  time_t report_period = 0;

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // reset hash to force to issue api request
  //reset_qcloud_request_hash();

  // update dev info
  ret = mycloudNAS_device_update_info(ctx, &report_period);
  printf("update_ret: %d \n", ret);

  // check error code
  switch( ret )
  {
    case MCN_ERR_DEVICE_ID_NOT_FOUND:
      // do sign out if the device is already unbind
      //printf("device id not found, maybe unbind from portal");
      printf("portal: %d\n", ret);
      reset_qcloud_ctx_conf(ctx);
      break;

    default:
      break;
  }

printf("update_device_info_ret:%d");
  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int check_device_info()
{
  int ret = QID_ERR_NO_ERROR;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // report period (not used)
  time_t report_period = 0;

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // check dev info
  ret = mycloudNAS_device_get_info(ctx);
  printf("check_ret: %d \n", ret);

  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  return ret;
}

int fetch_web_page(const char* protocol, uint16_t external_port, const char* path)
{
  int ret = QID_ERR_NO_ERROR;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  // init ctx
  struct qcloud_st* ctx = qcloud_init_ctx(&info);

  // prepare buffer
  char* buffer = (char*)malloc(4096);
  memset(buffer, 0, 4096);

  // call fetch web page
  ret = qcloud_fetch_web_page(ctx,
      protocol, external_port, path, 30,
      buffer, 4096);
  printf("%s\n", buffer);

  // cleanup ctx
  qcloud_cleanup_ctx(ctx);

  printf("fetch_web_page_ret_XXXXXX:%d\n",ret);
  // free buffer
  free(buffer);

  return ret;
}

void print_usage(char* prog)
{
  printf("usage: %s command arguments:\n", prog);
  printf("command list:\n");
  printf("signin qid pw\n");
  printf("available device_name\n");
  printf("bind device_name\n");
  printf("publish_service\n");
  printf("update_service_port management_external_port file_external_port\n");
  printf("get_device_info\n");
  printf("update_device_info\n");
  printf("check_device_info\n");
  printf("update_service_port_by_upnpc\n");
  printf("fetch_web_page protocol externel_port path\n");
  printf("unbind\n");
  printf("\n");
}


int main(int argc, char* argv[])
{
  // check argument number
  if( argc <= 1 )
  {
    print_usage(argv[0]);
    return -1;
  }

  // check commands
  if( !strcmp(argv[1], "signin") && argc == 4 ) {
    return signin(argv[2], argv[3]);
  }
  else if( !strcmp(argv[1], "available") && argc == 3 ) {
    return available(argv[2]);
  }
  if( !strcmp(argv[1], "bind") && argc == 3 ) {
    return bind(argv[2]);
  }
  else if( !strcmp(argv[1], "publish_service") && argc == 2 ) {
    return publish_service();
  }
  else if( !strcmp(argv[1], "update_service_port") && argc == 4 ) {
    return update_service_port(atoi(argv[2]), atoi(argv[3]));
  }
  else if( !strcmp(argv[1], "update_service_port_by_upnpc") && argc == 2 ) {
    return update_service_port_by_upnpc();
  }
  else if( !strcmp(argv[1], "get_device_info") && argc == 2 ) {
    return get_device_info();
  }
  else if( !strcmp(argv[1], "update_device_info") && argc == 2 ) {
    return update_device_info();
  }
  else if( !strcmp(argv[1], "check_device_info") && argc == 2 ) {
    return check_device_info();
  }
  else if( !strcmp(argv[1], "fetch_web_page") && argc == 5 ) {
    return fetch_web_page(argv[2], atoi(argv[3]), argv[4]);
  }
  else if( !strcmp(argv[1], "unbind") && argc == 2 ) {
    return unbind();
  }
  else {
    print_usage(argv[0]);
    return -1;
  }

  return 0;
}
