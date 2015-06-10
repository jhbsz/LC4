/*
 * qcloud_util.c
 *
 *  Created on: Jan 7, 2013
 */

#include <sys/stat.h> /* struct stat, fchmod (), stat (), S_ISREG, S_ISDIR */
#include "std_hdr.h"
#include "comm_hdr.h"
#include "qcloud_util.h"

#define HOST_FMT                "%s.%smyqnapcloud.com"
#define QNAP_HOST_FMT           "%s.%sqnap.com"
#define QCLOUD_API_HOST_FMT     "%s.api.%smyqnapcloud.com"
#define QCLOUD_HOST_FMT         "%smyqnapcloud.com"

#define QCLOUD_SERVER_PREFIX(i)  server_info[i].host_prefix
#define QCLOUD_SERVER_TAG(i)     server_info[i].tag
#define QCLOUD_IS_PROD_SITE(S)   (S[0] == '\0')
#define QCLOUD_SEVC_NAME(I)      service_name_list[I]

char *service_name_list[] =
{
    QCLOUD_SERVC_WWW_QCLOUD_STR,
    QCLOUD_SERVC_WWW_QID_STR,
    QCLODU_SERVC_WWW_START_STR,
    QCLOUD_SERVC_API_QID_STR,
    QCLOUD_SERVC_API_MCN_STR,
    QCLOUD_SERVC_API_QCLOUD_STR,
    QCLOUD_SERVC_API_QPKG_STR,
    QCLOUD_SERVC_API_USHER_STR,
    QCLOUD_SERVC_API_DDNS_STR,
    QCLOUD_SERVC_API_QSTART_STR,
    QCLOUD_SERVC_API_QSOCIAL_STR,
    QCLOUD_SERVC_API_DEVICE_STR,
    "\0"
};

typedef struct _qcloudd_server_info_st{
  char tag[45];
  char host_prefix[32];
}qcloudd_server_info_st;

// prepare server info
qcloudd_server_info_st server_info[] = {
  { "c125b9c78c3ea79c8f49a570cc0c1d7d", "dev-" },
  { "4ea77977c6a9d9ad07de14a9f00d62fb", "alpha-" },
  { "ec84ec12cbf73cb2c717f9585acbdb40", "\0" },
  { "\0", "alpha-" }
};

static int check_service_name(char *service_name){
  int i = 0;
  for(; service_name_list[i] != '\0'; i++){
    if( 0 == strncasecmp(service_name, service_name_list[i], SERVICE_NAME_LEN)){
      return i;
    }
  }
  return -1;
}

static char * check_server_host(int is_production)
{
  // the site prefix to return
  char *pch_site;

  // a counting var
  int i = 0;

  // var to check if a file exist
  struct stat sts;

  // var to store the path to check
  char check_path[QCLOUD_HOST_LEN+1];

  // check if production
  // if yes
  if( is_production ) pch_site = QCLOUD_SERVER_PREFIX(2);
  // if not
  else
  {
    // loop production, dev, and alpha to determine the prefix to use
    for(i=0; QCLOUD_SERVER_TAG(i) != '\0'; i++){
      snprintf(check_path, QCLOUD_HOST_LEN, "/var/%s", QCLOUD_SERVER_TAG(i));
      if ((stat (check_path, &sts)) == 0){
        break;
      }
    }
    pch_site = QCLOUD_SERVER_PREFIX(i);
  }

  return pch_site;
}

char get_qid_common[] = {"/bin/cat /tmp/qcloud_%s"};
#define GET_QID_SERVER_HOST         "HOST"
#define GET_QID_SERVER_HOST_PORT    "QID_HOST_PORT"
#define GET_USHR_SERVER_HOST_PORT   "USHER_HOST_PORT"
#define GET_QID_DEVICE_NAME         "DEVICE NAME"

int qcloud_util_get_config_site(char* url, int servc_id)
{
  char cmd[QCLOUD_HOST_LEN+1];
  char local_url[QCLOUD_HOST_LEN+1];
  char port[QCLOUD_HOST_LEN+1];

  snprintf(cmd, QCLOUD_HOST_LEN, get_qid_common, GET_QID_SERVER_HOST);
  util_system_cmd(cmd, local_url, QCLOUD_HOST_LEN);

  // port default value
  char port_default[QCLOUD_HOST_LEN+1];

  switch(servc_id)
  {
  default:
  case QCLODU_SERVC_API_QID:
    snprintf(cmd, QCLOUD_HOST_LEN, get_qid_common, GET_QID_SERVER_HOST_PORT);
    strcpy(port_default, "443");
    break;
  case QCLODU_SERVC_USHER:
    snprintf(cmd, QCLOUD_HOST_LEN, get_qid_common, GET_USHR_SERVER_HOST_PORT);
    strcpy(port_default, "80");
    break;
  }

  // execute command
  util_system_cmd(cmd, port, QCLOUD_HOST_LEN);

  // get port
  if( strlen(port) == 0 ) strcpy(port, port_default);

  if(strlen(local_url) == 0 || strlen(port) == 0){
    return -1;
  }
  else{
    snprintf(url, QCLOUD_HOST_LEN,"%s:%s", local_url, port);
    return 0;
  }

}

int qcloud_util_get_host(int is_production, char* service_name, char* server_host)
{
  char *pch_site;
  int servc_id;
  char device_name[NAME_LEN+1];
  char cmd[QCLOUD_HOST_LEN+1];
  if(!service_name || !server_host){
    fprintf(stderr, "null input parameter\n");
    return -1;
  }

  server_host[0] = '\0';

  // check if the service name exist
  if(-1 == (servc_id = check_service_name(service_name))){
    fprintf(stderr, "invalid parameter\n");
    return -1;
  }

  // get the site prefix
  pch_site = check_server_host(is_production);

  // get the server host
  switch(servc_id)
  {
    case QCLODU_SERVC_WWW_START:
    case QCLOUD_SERVC_QSTART:
      if( !is_production )
      {
        if(QCLOUD_IS_PROD_SITE(pch_site)){
          snprintf(server_host, QCLOUD_HOST_LEN, "%s.qnap.com",
                   QCLOUD_SEVC_NAME(servc_id));
        }else{
          snprintf(server_host, QCLOUD_HOST_LEN, "alpha-%s.qnap.com",
                   QCLOUD_SEVC_NAME(servc_id));
        }
      }
      else
        snprintf(server_host, QCLOUD_HOST_LEN, "%s.qnap.com",
                 QCLOUD_SEVC_NAME(servc_id));
      break;
    case  QCLOUD_SERVC_WWW_QCLOUD:
      snprintf(server_host, QCLOUD_HOST_LEN, "www."QCLOUD_HOST_FMT, pch_site);
      break;
    case  QCLODU_SERVC_WWW_QID:
      snprintf(server_host, QCLOUD_HOST_LEN, HOST_FMT, service_name, pch_site);
      break;
    case  QCLODU_SERVC_API_QID:
    case  QCLODU_SERVC_API_MCN:
    case  QCLODU_SERVC_CORE:
    case  QCLODU_SERVC_QPKG:
    case  QCLODU_SERVC_USHER:
    case  QCLOUD_SERVC_QSOCIAL:
      if( strcmp(pch_site, "dev-") == 0){
        if( qcloud_util_get_config_site(server_host, servc_id) == 0){
          ;
        }
        else{
          snprintf(server_host, QCLOUD_HOST_LEN, QCLOUD_API_HOST_FMT, service_name, pch_site);
        }
      }
      else{
        snprintf(server_host, QCLOUD_HOST_LEN, QCLOUD_API_HOST_FMT, service_name, pch_site);
      }
      break;
    case QCLOUD_SERVC_DEVICE:
      memset(cmd, 0, QCLOUD_HOST_LEN);
      snprintf(cmd, QCLOUD_HOST_LEN, get_qid_common, "\"DEVICE NAME\"");
      util_system_cmd(cmd, device_name, NAME_LEN);
      snprintf(server_host, QCLOUD_HOST_LEN, HOST_FMT, device_name, pch_site);
      break;
    case QCLOUD_SERVC_DDNS:
    default:
      snprintf(server_host, QCLOUD_HOST_LEN, QCLOUD_HOST_FMT, pch_site);
      break;
  }

  return 0;
}
