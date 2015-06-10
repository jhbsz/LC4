/*
 * porter_ev.c
 *
 */
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include "std_hdr.h"
#include "comm_hdr.h"

#include "qid_api.h"
#include "qid_struct.h"
#include "qid_system.h"
#include "mcn_api.h"
#include "qcloud.h"
 #include "porter_ev.h"


#ifdef DEBUG
#define     PORTER_CHECK_EXT_IP_TIMER      (15)         // in seconds
#else
#define     PORTER_CHECK_EXT_IP_TIMER      (15*60)         // in seconds
#endif

/*
 * Variables declaration.
 */
struct qcloud_st *qid_ctx;                            // context object of libQID
PORTER_EV_CB_T porter_ev_cb;                          // control block of porter event

static int porter_check_lanip( int* p_is_changed )
{
  int ret = QID_ERR_NO_ERROR;

  // check input
  if( p_is_changed == NULL )  return ERR_NULL_POINTER;

  // get lan ip list
  qid_sys_get_local_ip_list(&porter_ev_cb.new_lan_info);

  // reset is changed to FALSE
  *p_is_changed = FALSE;

  // check if changed
  if(0 != strncmp(porter_ev_cb.lan_info.lan_ip_list, porter_ev_cb.new_lan_info.lan_ip_list, LAN_IP_LIST_LEN) ||
      0 != strncmp(porter_ev_cb.lan_info.lan_gw, porter_ev_cb.new_lan_info.lan_gw, IP_STR_LEN))
  {
    // log
    QNAP_LOG_DEBUG_NL("need to update device information: lan_ip & gw changed from \"%s %s\" to \"%s %s\"",
      porter_ev_cb.lan_info.lan_ip_list, porter_ev_cb.lan_info.lan_gw,
      porter_ev_cb.new_lan_info.lan_ip_list, porter_ev_cb.new_lan_info.lan_gw);

    // set to  true to indicate lan ip is changed
    *p_is_changed = TRUE;
  }

  return ret;
}

static int porter_check_wanip( int* p_is_changed )
{
  int ret = QID_ERR_NO_ERROR;

  // check input
  if( p_is_changed == NULL )  return ERR_NULL_POINTER;

  // reset is changed to FALSE
  *p_is_changed = FALSE;

  // get external IP address from API server
  porter_ev_cb.new_external_ip[0] = 0;
  ret = qcloud_get_external_ip(qid_ctx, porter_ev_cb.new_external_ip, sizeof(porter_ev_cb.new_external_ip));
  if(strlen(porter_ev_cb.new_external_ip) == 0){
    QNAP_LOG_WARN_NL("get WAN IP failed");
    return ret;
  }

  // check if externel ip changed
  if( strncmp(porter_ev_cb.external_ip, porter_ev_cb.new_external_ip, IP_STR_LEN) )
  {
    QNAP_LOG_DEBUG_NL("need to update device information: WAN IP changed from %s to %s",
      porter_ev_cb.external_ip, porter_ev_cb.new_external_ip);
    *p_is_changed = TRUE;
  }

  return ret;
}

/*
 * porter_ev_init()
 *
 * Init event loop.
 *
 * Input:
 *      None.
 *
 * Output:
 *      @ NO_ERROR for successful initialization.
 *      @ ERR_xxxx for any kind of error.
 */
int porter_ev_init(void)
{
  // return code
  int rc = QID_ERR_NO_ERROR;

  // flag to check if need to log curl
  char curl_flag[TMP_BUFF_LEN+1];

  // flag to pass to qnap log function
  uint32_t log_flag;

  // init app info
  qnap_app_info info;
  memset(&info, 0, sizeof(qnap_app_info));

  /*
   * init libqcloud context object.
   */
  qid_ctx = qcloud_init_ctx(&info);
  if (qid_ctx == NULL) {
    QNAP_LOG_CRIT_NL("Cannot set up QID context");
    rc = -1;
    goto ret;
  }

  // get curl log flag
  qid_sys_get_curl_log_flag(curl_flag, TMP_BUFF_LEN);
  log_flag = QID_LOG_FILE;
  if (MAGIC_LOG_NUMBER == atoi(curl_flag)){
    log_flag |= QID_LOG_CURL;
  }

  // enable log
  qcloud_set_log(qid_ctx, log_flag, LOG_DEBUG);
  qnap_log_to_file(QNAP_PORTER_LOG_PATH);

  /*
   * init porter_event control block
   */
  memset (&porter_ev_cb, 0x0, sizeof(porter_ev_cb));

ret:
  return (rc);

}

/*
 * stop event loop, and release allocated resource.
 */
void porter_ev_cleanup()
{
  // stop the loop
  porter_ev_cb.exit_flag = TRUE;

  // cleanup ctx
  qcloud_cleanup_ctx(qid_ctx);
  qid_ctx = NULL;
}

// prepare service list
// fix the value for now
// should dynamically read from some config file in the future
static mcn_ddns_publish_st ddns_service_tlb[2] = {
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

static int porter_period_check(time_t* p_update_period) {
  int ret = QID_ERR_NO_ERROR;

  // flag to check if need to update device
  int is_update_device = FALSE;

  // get current time
  time_t current_time = time(0);

  do
  {
    // check if need to check
    if( (current_time - porter_ev_cb.check_time) < PORTER_CHECK_EXT_IP_TIMER )
      break;

    // debug log
    QNAP_LOG_INFO_NL("start to check LAN / WAN IP");

    // update check time
    porter_ev_cb.check_time = current_time;

    // check if lan ip changed
    ret = porter_check_lanip(&is_update_device);
    if( ret != QID_ERR_NO_ERROR )
    {
      QNAP_LOG_ERR_NL("failed to check LAN IP");
      break;
    }

    // if lan not changed, check wan
    if( !is_update_device )
    {
      ret = porter_check_wanip(&is_update_device);
      if( ret != QID_ERR_NO_ERROR )
      {
        QNAP_LOG_ERR_NL("failed to check WAN IP");
        break;
      }
    }

    // check if over 1 day
    if( !is_update_device )
    {
      if( (current_time - porter_ev_cb.update_time) > (*p_update_period) )
        is_update_device = TRUE;
    }

    // no need to update, just return
    if( !is_update_device )
    {
      QNAP_LOG_DEBUG_NL("no need to update device");
      break;
    }

    // update publish service
    ret = mycloudNAS_update_published_service(
      qid_ctx,
      ddns_service_tlb,
      2);
    if( ret != QID_ERR_NO_ERROR )
    {
      QNAP_LOG_ERR_NL("failed to update publish service");
      break;
    }

    // update device info
    ret = mycloudNAS_device_update_info(qid_ctx, p_update_period);
    if( ret != QID_ERR_NO_ERROR )
    {
      QNAP_LOG_ERR_NL("failed to update device info");
      break;
    }
    else
    {
      QNAP_LOG_DEBUG_NL("next update period: %d", *p_update_period);
    }

    // update lan ip
    strncpy(porter_ev_cb.lan_info.lan_ip_list, porter_ev_cb.new_lan_info.lan_ip_list, LAN_IP_LIST_LEN);
    strncpy(porter_ev_cb.lan_info.lan_gw, porter_ev_cb.new_lan_info.lan_gw, IP_STR_LEN);
    porter_ev_cb.lan_info.lan_ip_list[LAN_IP_LIST_LEN] = '\0';
    porter_ev_cb.lan_info.lan_gw[IP_STR_LEN] = '\0';

    // update wan ip
    strncpy(porter_ev_cb.external_ip, porter_ev_cb.new_external_ip, IP_STR_LEN);
    porter_ev_cb.external_ip[IP_STR_LEN] = 0;

    // update update time
    porter_ev_cb.update_time = current_time;
  } while(0);

  return ret;
}


/*
 * start event loop
 */
int porter_ev_run()
{
  int ret = QID_ERR_NO_ERROR;

  // var to get the update period from cloud server
  time_t update_period = 86400;

  while( porter_ev_cb.exit_flag == FALSE ) {
    // get device status from /etc/config/qid.conf
    qid_sys_get_qid_status(porter_ev_cb.device_status,
                           sizeof(porter_ev_cb.device_status));
    if( porter_ev_cb.device_status[0] != QID_STAT_E_BIND )
    {
      QNAP_LOG_INFO_NL("not sign-in (status is %s), exit porter",
        porter_ev_cb.device_status);
      break;
    }

    // period check if status is binding
    ret = porter_period_check(&update_period);

    // processing error
    if( ret != QID_ERR_NO_ERROR )
    {
      // check error code
      switch( ret )
      {
        // for all the following situations
        // should reset config
        case OAUTH_ERR_INVALID_GRANT:
        case OAUTH_ERR_INVALID_PASS:
        case OAUTH_ERR_INVALID_USER:
        case OAUTH_ERR_INVALID_CLI:
        case OAUTH_ERR_INVALID_REQ:
        case QID_ERR_RES_NOT_FOUND:
        case MCN_ERR_DEVICE_ID_NOT_FOUND:
          QNAP_LOG_ERR_NL("device id not found or auth fail, need to reset config (%d)", ret);
          reset_qcloud_ctx_conf(qid_ctx);
          goto exit;
          break;

        default:
          QNAP_LOG_INFO_NL("got not handled error (%d)", ret);
          break;
      }

      // if we didn't update device yet
      // need to check immediately
      if( porter_ev_cb.external_ip[0] == '\0' )
        porter_ev_cb.check_time = 0;
    }

    // sleep
    sleep(6);
  }

exit:
  // exit loop log
  QNAP_LOG_DEBUG_NL("bye");

  return ret;
}




