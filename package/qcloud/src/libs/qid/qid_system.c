
#include <errno.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "std_hdr.h"
#include "comm_hdr.h"
#include "qid_system.h"
#include "qcloud_util.h"
#include "qcloud.h"
#include "qcloud_dao.h"

#include <fcntl.h>


struct qid_config_st
{
  /*! qcloud config verison */
  char version[VERSION_LEN + 1];
  /*! APP auth token of QID service */
  char app_token[MAX_APPKEY_LEN + 1];
  /*! refresh token of QID service */
  char refresh_token[MAX_APPKEY_LEN + 1];
  /*! auth token of QID service */
  char auth_token[MAX_APPKEY_LEN + 1];
  /*! valid period of this token */
  char token_valid_period[NAME_LEN + 1];
  /*! last update time of token */
  char last_token_update[NAME_LEN + 1];
  /*! QNAP ID, user name of QNAP portal service, a signed ascii string; max length 254*/
  char qid[MAX_QID_LEN + 1];
  /*! device unqiue ID of mycloudnas service */
  char device_id[DEVICE_ID_LEN + 1];
  /*! device unqiue token of mycloudnas service */
  char device_token[MAX_APPKEY_LEN + 1];
  /*! device alias name  */
  char device_name[NAME_LEN + 1];
  /*! bind status */
  char status[STATUS_FLAG_LEN + 1];
  /*! ddns status */
  char ddns_status[STATUS_FLAG_LEN + 1];
};
struct qid_config_st *p_qid_config = NULL;

// the format is version^app_token^refresh_token^auth_token^token_valid_period^last_token_update^qid^device_id^device_token^device_name^status^ddns_status
#if LOCAL_TEST

#define LOAD_NVRAM_CMD "/bin/cat "
#define SAVE_NVRAM_CMD "/bin/echo -n "

// for config
#define LOAD_NVRAM_QCLOUD_CONFIG_CMD        LOAD_NVRAM_CMD "/tmp/qgenie/myqnapcloud_id_rule 2>/dev/null"
#define SAVE_NVRAM_QCLOUD_CONFIG_CMD        SAVE_NVRAM_CMD "'%s' > /tmp/qgenie/myqnapcloud_id_rule"

// for hw & firmware info
#define LOAD_NVRAM_HWSN_CMD                 LOAD_NVRAM_CMD "/tmp/qgenie/serial_num_rule 2>/dev/null"
#define LOAD_NVRAM_SUID_CMD                 LOAD_NVRAM_CMD "/tmp/qgenie/license_key 2>/dev/null"
#define LOAD_NVRAM_LICENSE_INVALID_CMD      LOAD_NVRAM_CMD "/tmp/qgenie/license_invalid 2>/dev/null"
#define LOAD_NVRAM_MAC_ADDR_CMD             LOAD_NVRAM_CMD "/tmp/qgenie/lan_hwaddr_rule_default 2>/dev/null"
#define LOAD_NVRAM_MODEL_NAME_CMD           LOAD_NVRAM_CMD "/tmp/qgenie/model_name 2>/dev/null"
#define LOAD_NVRAM_FW_VER_CMD               LOAD_NVRAM_CMD "/tmp/qgenie/fw_version 2>/dev/null"

// for update status
#define SAVE_NVRAM_LAST_UPDATE_STATUS       SAVE_NVRAM_CMD "'%d' > /tmp/qgenie/myqnapcloud_last_update_status"
#define LOAD_NVRAM_LAST_UPDATE_STATUS       LOAD_NVRAM_CMD "/tmp/qgenie/myqnapcloud_last_update_status 2>/dev/null"
#define SAVE_NVRAM_LAST_UPDATE_SUCESS_TIME  SAVE_NVRAM_CMD "'%d' > /tmp/qgenie/myqnapcloud_last_update_sucess_time"
#define LOAD_NVRAM_LAST_UPDATE_SUCESS_TIME  LOAD_NVRAM_CMD "/tmp/qgenie/myqnapcloud_last_update_sucess_time 2>/dev/null"
#define SAVE_NVRAM_LAST_UPDATE_FAIL_TIME    SAVE_NVRAM_CMD "myqnapcloud_last_update_fail_time=%d"
#define LOAD_NVRAM_LAST_UPDATE_FAIL_TIME    LOAD_NVRAM_CMD "/tmp/qgenie/myqnapcloud_last_update_fail_time 2>/dev/null"

#else /* LOCAL_TEST */

#define LOAD_NVRAM_CMD "/usr/sbin/nvram get "
#define SAVE_NVRAM_CMD "/usr/sbin/nvram fset "

// for config
#define LOAD_NVRAM_QCLOUD_CONFIG_CMD        LOAD_NVRAM_CMD "myqnapcloud_id_rule 2>/dev/null"
#define SAVE_NVRAM_QCLOUD_CONFIG_CMD        SAVE_NVRAM_CMD "myqnapcloud_id_rule='%s'"

// for hw & firmware info
#define LOAD_NVRAM_HWSN_CMD                 LOAD_NVRAM_CMD "serial_num_rule 2>/dev/null"
#define LOAD_NVRAM_SUID_CMD                 LOAD_NVRAM_CMD "license_key 2>/dev/null"
#define LOAD_NVRAM_LICENSE_INVALID_CMD      LOAD_NVRAM_CMD "license_invalid 2>/dev/null"
#define LOAD_NVRAM_MAC_ADDR_CMD             LOAD_NVRAM_CMD "lan_hwaddr_rule_default 2>/dev/null"
#define LOAD_NVRAM_MODEL_NAME_CMD           LOAD_NVRAM_CMD "model_name 2>/dev/null"
#define LOAD_NVRAM_FW_VER_CMD               LOAD_NVRAM_CMD "fw_version 2>/dev/null"

// for update status
#define SAVE_NVRAM_LAST_UPDATE_STATUS       SAVE_NVRAM_CMD "myqnapcloud_last_update_status=%d"
#define LOAD_NVRAM_LAST_UPDATE_STATUS       LOAD_NVRAM_CMD "myqnapcloud_last_update_status 2>/dev/null"
#define SAVE_NVRAM_LAST_UPDATE_SUCESS_TIME  SAVE_NVRAM_CMD "myqnapcloud_last_update_sucess_time=%d"
#define LOAD_NVRAM_LAST_UPDATE_SUCESS_TIME  LOAD_NVRAM_CMD "myqnapcloud_last_update_sucess_time 2>/dev/null"
#define SAVE_NVRAM_LAST_UPDATE_FAIL_TIME    SAVE_NVRAM_CMD "myqnapcloud_last_update_fail_time=%d"
#define LOAD_NVRAM_LAST_UPDATE_FAIL_TIME    LOAD_NVRAM_CMD "myqnapcloud_last_update_fail_time 2>/dev/null"

#endif /* LOCAL_TEST */

static int parse_nvram_qcloud_config(char** p_data_pos, char* buf, size_t buf_len)
{
  // check input
  if( p_data_pos == NULL || *p_data_pos == NULL || buf == NULL )
    return -1;

  // find separator
  char* token = strstr(*p_data_pos, "^");
  if( token == NULL ) return -1;

  // copy to output
  *token = '\0';
  strncpy(buf, *p_data_pos, buf_len);
  *p_data_pos = token + 1;

  return NO_ERROR;
}

static int load_nvram_qcloud_config(int force_reload)
{
  int ret = NO_ERROR;

  char data_buf[TMP_BUFF_LEN+1];

  // prepare the required folders
  ret = system("/bin/mkdir -p " QCLOUD_VAR_CACHE_FOLDER);
  ret = system("/bin/mkdir -p " HASH_PATH);

#if LOCAL_TEST
  // prepare the folder required for local test
  ret = system("/bin/mkdir -p /tmp/qgenie");
#endif

  // if already get the config, check if need force reload
  if( p_qid_config )
  {
    if(1) {
      free(p_qid_config);
      p_qid_config = NULL;
    }
    else return NO_ERROR;
  }

  // try to allocate memory for nvram config
  if( p_qid_config == NULL )
  {
    p_qid_config = calloc(1, sizeof(struct qid_config_st));
    if( p_qid_config == NULL ) return ERR_NULL_POINTER;
  }

  // execute get config command
  memset(data_buf, 0, sizeof(data_buf));
  ret = util_system_cmd(LOAD_NVRAM_QCLOUD_CONFIG_CMD, data_buf, TMP_BUFF_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // use strstr to split value_ptr by separator "^"
  // and put the data into p_qid_config
  char* data_pos = data_buf;

  // get version
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->version, VERSION_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get app_token
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->app_token, MAX_APPKEY_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get refresh_token
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->refresh_token, MAX_APPKEY_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get auth_token
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->auth_token, MAX_APPKEY_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get token_valid_period
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->token_valid_period, NAME_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get last_token_update
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->last_token_update, NAME_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get qid
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->qid, MAX_QID_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get device_id
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->device_id, DEVICE_ID_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get device_token
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->device_token, MAX_APPKEY_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get device_name
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->device_name, NAME_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get status
  ret = parse_nvram_qcloud_config(&data_pos, p_qid_config->status, STATUS_FLAG_LEN);
  if(NO_ERROR != ret){
    return ret;
  }

  // get ddns_status
  strncpy(p_qid_config->ddns_status, data_pos, STATUS_FLAG_LEN);

  return NO_ERROR;
}

static int save_nvram_qcloud_config()
{
  int ret = NO_ERROR;

  char cmd_buf[TMP_BUFF_LEN+1];
  char data_buf[TMP_BUFF_LEN+1];

  // check if have data to store
  if( p_qid_config == NULL )
    return ERR_NULL_POINTER;

  // concate all fields in p_qid_config to a string
  snprintf(data_buf, TMP_BUFF_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
    p_qid_config->version,
    p_qid_config->app_token,
    p_qid_config->refresh_token,
    p_qid_config->auth_token,
    p_qid_config->token_valid_period,
    p_qid_config->last_token_update,
    p_qid_config->qid,
    p_qid_config->device_id,
    p_qid_config->device_token,
    p_qid_config->device_name,
    p_qid_config->status,
    p_qid_config->ddns_status);

  // generate the command
  snprintf(cmd_buf, TMP_BUFF_LEN, SAVE_NVRAM_QCLOUD_CONFIG_CMD, data_buf);
  QNAP_LOG_DEBUG_NL("cmd: %s", cmd_buf);

  // execute command
  ret = system(cmd_buf);

  // return status
  return ret;
}

int qid_sys_nvram_commit()
{
  int ret = NO_ERROR;

#if LOCAL_TEST
  // do nothing
#else
  // execute nvram commit
  ret = system("/usr/sbin/nvram commit");
#endif

  return ret;
}






#define SYS_DEVICE_NAME_ITEM    "DEVICE NAME"
#define SYS_DEVICE_ID_ITEM      "DEVICE ID"
#define SYS_APP_TOKEN_ITEM      "APP_TOKEN"
#define SYS_DDNS_UPDATE         "DDNS_UPDATE"

static int set_variable_safety(char *value, const char *set_fmt, const char *get_fmt)
{
  char cmd_buf[MAX_SYSTEM_CMD+1];
  char data_buf[TMP_BUFF_LEN+1];

  if(!value || !set_fmt || !get_fmt){
    return ERR_NULL_POINTER;
  }

  snprintf(cmd_buf, MAX_SYSTEM_CMD, set_fmt, value);

  if(NO_ERROR != util_system_cmd(cmd_buf, data_buf, TMP_BUFF_LEN)){
    return -1;
  }

  return NO_ERROR;
}

/*
 * this is straight from beej's network tutorial. It is a nice wrapper
 * for inet_ntop and helpes to make the program IPv6 ready
 */
static char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
  switch(sa->sa_family) {
  case AF_INET:
    inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, maxlen);
    break;

  case AF_INET6:
    inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, maxlen);
    break;

  default:
    strncpy(s, "Unknown AF", maxlen);
    return NULL;
  }

  return s;
}

static int check_error_string(char *pchSrc, int dlen)
{
    if (dlen<=0 || pchSrc==NULL)
        return -1;

    int index = 0;
    while (index < dlen)
    {
        if (isprint(*(pchSrc+index)))  // Checking if it is a printable char
        {
            index ++;
        }
        else
        {
			//clear string
			memset(pchSrc, 0x0, dlen);
            // Found not printable char, break while loop
            return ++index;
        }
    }
    return 0;
}

static char *skip_white_space(char *str)
{
  char *p = str;
  while (*p == ' ' || *p == '\t') p++;
  return p;
}

char* fgets_for_buffer(char* out_buf, size_t out_buf_len, char* in_buf )
{
  char* pos = NULL;
  size_t in_buf_len = strlen(in_buf) + 1;
  size_t search_len = out_buf_len;
  size_t copy_len = 0;
  if( in_buf_len < search_len ) search_len = in_buf_len;

  pos = (char*)memchr(in_buf, '\n', search_len);
  if( pos == NULL ) return NULL;
  else{
    copy_len = pos - in_buf;
    memcpy(out_buf, in_buf, copy_len);
    out_buf[copy_len] = 0;
  }

  return (pos + 1);
}

int qid_conf_get_entry(char* qid_conf_buf,char* section, char* key, char* value, size_t value_len)
{
  char p[TMP_BUFF_LEN+1];
  char q[TMP_BUFF_LEN+1];
  char target_section[TMP_BUFF_LEN+1];
  int section_found = 0;
  int cnt = 0;
  char* pch;
  char *delim = " = ";
  sprintf(target_section,"[%s]",section);
  int ret = NO_ERROR;

  if(qid_conf_buf == NULL){
    QNAP_LOG_ERR_NL("qid_conf_fp:NULL");
    ret = ERR_NULL_POINTER;
    goto exit;
  }
  while((qid_conf_buf = fgets_for_buffer(p, (TMP_BUFF_LEN+1)*sizeof(char), qid_conf_buf))){
    memset(q, 0, sizeof(char)*(TMP_BUFF_LEN+1));
    sprintf(p,"%s", skip_white_space(p));
    for(cnt = 0; cnt <= strlen(p); cnt++){
      if(p[cnt] >= 32 && p[cnt] <= 126){
        q[cnt] = p[cnt];
      }
    }
    if (q[0] == '['){ //start of section name...
      strcpy( p, q);

      if (strcmp(p, target_section)==0){
        section_found=1;
      }else{
        section_found=0;
      }
    }else if (section_found) {
      if (q[0] == '[' || q[0] == '#'){
        break;
      }
     pch = strtok(q,delim);
      if( pch != NULL){
        if( strcmp( pch, key) == 0){
          pch = strtok(NULL,delim);
          if(pch != NULL) strcpy( value, pch );
          break;
        }
      }
    }
  }
exit:
  //fseek(qid_conf_fp, 0, SEEK_SET);
  return ret;
}

int qid_sys_get_ip_proc_net_route(in_addr_t * addr)
{
  long d, g;
  char buf[256];
  int line = 0;
  FILE * f;
  char * p;
  f = fopen("/proc/net/route", "r");
  if(!f)
    return ERR_NULL_POINTER;
  while(fgets(buf, sizeof(buf), f)) {
    if(line > 0) {
      p = buf;
      while(*p && !isspace(*p))
        p++;
      while(*p && isspace(*p))
        p++;
      if(sscanf(p, "%lx%lx", &d, &g)==2) {
        if(d == 0) { /* default */
          *addr = g;
          fclose(f);
          return NO_ERROR;
        }
      }
    }
    line++;
  }
  /* default route not found ! */
  if(f)
    fclose(f);
  return ERR_NULL_POINTER;
}

int qid_sys_get_gateway_ip(char* lan_gw_ip, char* lan_gw_interface)
{

  char cmd_buff[MAX_SYSTEM_CMD+1];
  int i;
  int ret = NO_ERROR;
  char UG_if[IFNAMSIZ+1];
  /*
   * get the interface name of default gateway.
   */
  snprintf(cmd_buff, MAX_SYSTEM_CMD, GET_GATEWAY_IF);
  i = 0;
  UG_if[0] = '\0';
  for(; i < 3 && 0 == strlen(UG_if); i ++){
    ret = util_system_cmd(cmd_buff, UG_if, IFNAMSIZ);
    sleep(1);
  }

  struct in_addr addr;
  memset( &addr, 0 ,sizeof(struct in_addr));
  qid_sys_get_ip_proc_net_route( &(addr.s_addr));
  char* gw_ip = inet_ntoa( addr );

  if( ret == NO_ERROR)
  {
    sprintf(lan_gw_ip, "%s", gw_ip);
    sprintf(lan_gw_interface, "%s", UG_if);
  }

  return ret;
}

int qid_sys_get_local_ip_list(lan_ip_info_st *lan_info)
{
  int ret = NO_ERROR;

  // counting var
  int i = 0, j = 0;

  // store the total number of interfaces for the loop max
  int num_nics = 0;

  // buffers
  char ip_addr_buf[TMP_BUFF_LEN+1];
  char cmd_buff[MAX_SYSTEM_CMD+1];
  char interface_buf[2][64];
  char buf[TMP_BUFF_LEN] = {0};

  // var for ioctl to get interface info
  struct ifconf ifc;
  struct ifreq *ifr;
  int           sck;

  // snprintf ret
  int snprintf_ret = 0;

  // check input
  if(!lan_info){
    QNAP_LOG_ERR_NL("invalid parameter");
    return ERR_INVALID_VALUE;
  }

  // reset parameters
  memset(lan_info, 0x0, sizeof(lan_ip_info_st));

  // get lan interface list str ptr for processing convenience
  char *p_ip_list = lan_info->lan_ip_list;
  size_t free_buff_len = LAN_IP_LIST_LEN;

  // get the interfaces for WAN
#if DEBUG
  strcpy(interface_buf[0], "eth0");
#else
  ret = util_system_cmd("/usr/sbin/nvram get wan0_ifname", interface_buf[0], 63);
  if( ret != NO_ERROR ) return ret;
#endif

  // get the interface for LAN
  strcpy(interface_buf[1], "br0");

  /* Get a socket handle. */
  sck = socket(AF_INET, SOCK_DGRAM, 0);
  if(sck < 0){
    QNAP_LOG_ERR_NL("socket error: %d", errno);
  }

  /* Query available interfaces. */
  ifc.ifc_len = sizeof(buf);
  ifc.ifc_buf = buf;
  if(ioctl(sck, SIOCGIFCONF, &ifc) < 0){
    QNAP_LOG_ERR_NL("ioctl(SIOCGIFCONF): %d", errno);
  }

  /* Iterate through the list of interfaces. */
  ifr = ifc.ifc_req;
  num_nics = ifc.ifc_len / sizeof(struct ifreq);
  for(i = 0; i < num_nics; i++){

    // get this interface item
    struct ifreq *item = &ifr[i];
    struct sockaddr *addr = &(item->ifr_addr);

    // get ip address
    char *ip = get_ip_str(addr, ip_addr_buf, INET6_ADDRSTRLEN);

    // check if it's the interface to Internet
    for(j=0; j<2; j++){
      if(strcmp(item->ifr_name, interface_buf[j]) == 0){
        if( addr->sa_family == AF_INET){
          strncpy(lan_info->lan_ip[lan_info->lan_ip_count], ip, INET6_ADDRSTRLEN);
          snprintf(lan_info->lan_ip_version[lan_info->lan_ip_count], IP_VERSION_LEN,IP_VERSION4);
          snprintf(lan_info->interface[lan_info->lan_ip_count], IP_VERSION_LEN,"%s",item->ifr_name);
        }else if(addr->sa_family == AF_INET6){
          strncpy(lan_info->lan_ip[lan_info->lan_ip_count], ip, INET6_ADDRSTRLEN);
          snprintf(lan_info->lan_ip_version[lan_info->lan_ip_count], IP_VERSION_LEN,IP_VERSION6);
          snprintf(lan_info->interface[lan_info->lan_ip_count], IP_VERSION_LEN,"%s",item->ifr_name);
        }

        // this code is quite weird here
        // according to Paul's comment
        // we only send the default gateway interface's IP address
        // not the default gateway IP itself
        if( j == 0 )
        {
          strncpy(lan_info->lan_gw, ip, IP_STR_LEN);
          lan_info->lan_gw[IP_STR_LEN] = '\0';
        }

        // add to interface list str
        snprintf_ret = snprintf(p_ip_list, free_buff_len, "%s,", ip);
        p_ip_list += snprintf_ret;
        free_buff_len -= snprintf_ret;

        // only have one interface for internet, so just break
        lan_info->lan_ip_count++;
        break;
      }
    }
  }

  /*
   * remove last ','
   */
  if((i = strlen(lan_info->lan_ip_list)) > 0){
    if(lan_info->lan_ip_list[i-1] == ','){
      lan_info->lan_ip_list[i-1] = '\0';
    }
  }

  // garbage collection
  if(sck > 0){
    close(sck);
  }

  return ret;
}

int qid_sys_get_hw_serial_number(char *serial_num, size_t buff_len)
{
  return util_system_cmd(LOAD_NVRAM_HWSN_CMD, serial_num, buff_len);
}

int qid_sys_get_suid(char *suid, size_t buff_len)
{
  int ret = NO_ERROR;

  ret = util_system_cmd(LOAD_NVRAM_SUID_CMD, suid, buff_len);
  if( strlen(suid) == 0 ) sprintf(suid, "N/A");

  return ret;
}

int qid_sys_get_license_invalid(char* license_invalid, size_t buff_len)
{
  int ret = NO_ERROR;

  ret = util_system_cmd(LOAD_NVRAM_LICENSE_INVALID_CMD, license_invalid, buff_len);

  return ret;
}

int qid_sys_get_qcloud_api_proto(char *proto, size_t buff_len)
{
  // FIXME : fix to https
  strncpy(proto, "https", buff_len);
  return NO_ERROR;
}

int qid_sys_get_mac(char *mac_str, size_t buff_len)
{
  int i;

  util_system_cmd(LOAD_NVRAM_MAC_ADDR_CMD, mac_str, buff_len);

  for(i = 0; i < buff_len && mac_str[i] != '\0'; i++){
    if(isalpha(mac_str[i])){
      mac_str[i] = tolower(mac_str[i]);
    }
  }

  return 0;
}

/*nassid = [hw_serial_number-mac address]*/
int qid_sys_get_nassid( char *nassid, size_t buff_len)
{
  int ret = 0;

  /* get hardware serial number of NAS */
  ret = qid_sys_get_hw_serial_number(nassid, buff_len);
  if(ret != NO_ERROR){
    return ret;
  }

  ret = strlen(nassid);
  if(ret < 6){
    /*not regular hardware number*/
    ret = 0;
  }else{
    nassid[ret] = '-';
    ret ++;
  }

  /* get MAC address */
  ret = qid_sys_get_mac(nassid+ret, buff_len-ret);

  return ret;
}

/*
 *  #define EVENT_TYPE_INFO   0
    #define EVENT_TYPE_WARN   1
    #define EVENT_TYPE_ERROR  2
 */

int qid_sys_send_event_log(int event_type, char *event_msg)
{

  if(!event_msg){
    return ERR_NULL_POINTER;
  }

  char msg[TMP_BUFF_LEN+1];
  return util_system_cmd(event_msg, msg, TMP_BUFF_LEN);

}

int qid_sys_set_qid(char *qid)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->qid, qid, MAX_QID_LEN);

  // save config
  return save_nvram_qcloud_config();
}

int qid_sys_set_auth_token(char *qid_auth_token)
{
#if 1
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->auth_token, qid_auth_token, MAX_APPKEY_LEN);

  // save config
  return save_nvram_qcloud_config();
#else
  char cmd[MAX_SYSTEM_CMD+1];
  char data[MAX_SYSTEM_CMD+1];

  if(!qid_auth_token){
    return ERR_NULL_POINTER;
  }

  snprintf(cmd, MAX_SYSTEM_CMD, SET_QID_AUTH_TOKEN, qid_auth_token);
  return util_system_cmd(cmd, data, MAX_SYSTEM_CMD);
#endif
}

int qid_sys_get_auth_token(char *token, size_t buff_len)
{
#if 1
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(token, p_qid_config->auth_token, buff_len);

  return ret;
#else
  return util_system_cmd(GET_QID_AUTH_TOKEN, token, buff_len);
#endif
}

int qid_sys_get_refresh_token(char *refresh_token, size_t buff_len)
{
#if 1
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(refresh_token, p_qid_config->refresh_token, buff_len);

  return ret;
#else
  return util_system_cmd(GET_QID_REFRESH_TOKEN, refresh_token, buff_len);
#endif
}

int qid_sys_set_refresh_token(char *refresh_token)
{
#if 1
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->refresh_token, refresh_token, MAX_APPKEY_LEN);

  // save config
  return save_nvram_qcloud_config();
#else
  char cmd[MAX_SYSTEM_CMD+1];
  char data[MAX_SYSTEM_CMD+1];

  if(!refresh_token){
    return ERR_NULL_POINTER;
  }

  snprintf(cmd, MAX_SYSTEM_CMD, SET_QID_REFRESH_TOKEN, refresh_token);
  return util_system_cmd(cmd, data, MAX_SYSTEM_CMD);
#endif
}

int qid_sys_get_last_update_token_time(char *at, size_t buff_len)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(at, p_qid_config->last_token_update, buff_len);

  return ret;
}

int qid_sys_set_last_update_token_time(char *at)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->last_token_update, at, MAX_APPKEY_LEN);

  // save config
  return save_nvram_qcloud_config();
}

int qid_sys_get_token_valid_period(char *valid_period, size_t buff_len)
{
  return util_system_cmd(GET_QID_VALID_PERIOD, valid_period, buff_len);
}

int qid_sys_set_token_valid_period(char *valid_period)
{
#if 1
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->token_valid_period, valid_period, NAME_LEN);

  // save config
  return save_nvram_qcloud_config();
#else
  char cmd[MAX_SYSTEM_CMD+1];
  char data[MAX_SYSTEM_CMD+1];

  if(!valid_period){
    return ERR_NULL_POINTER;
  }

  snprintf(cmd, MAX_SYSTEM_CMD, SET_QID_VALID_PERIOD, valid_period);
  return util_system_cmd(cmd, data, MAX_SYSTEM_CMD);
#endif
}

int qid_sys_get_qid(char *qid, size_t buff_len)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(qid, p_qid_config->qid, buff_len);

  return ret;
}

int qid_sys_get_model(char *model, size_t buff_len)
{
  int ret = NO_ERROR;

  //ret = util_system_cmd(LOAD_NVRAM_MODEL_NAME_CMD, model, buff_len);
  strncpy(model, "QGenie", buff_len);

  if (model && model[0] == '\0'){
    strncpy(model, "Unknown Model", buff_len);
  }
  return ret;
}

int qid_sys_get_fw_ver(char *fw_ver, size_t buff_len)
{
  return util_system_cmd(LOAD_NVRAM_FW_VER_CMD, fw_ver, buff_len);
}

int qid_sys_get_server_host(int is_production, char *host, size_t buff_len)
{
  return qcloud_util_get_host(is_production, QCLOUD_SERVC_API_QCLOUD_STR, host);
}

int qid_sys_get_mcn_server_host(int is_production, char *host, size_t buff_len)
{
  return qcloud_util_get_host(is_production, QCLOUD_SERVC_API_QCLOUD_STR, host);
}

int qid_sys_get_usher_server_host(int is_production, char *host, size_t buff_len)
{
  return qcloud_util_get_host(is_production, QCLOUD_SERVC_API_USHER_STR, host);
}


int qid_sys_set_device_name(char *name)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->device_name, name, NAME_LEN);

  // save config
  return save_nvram_qcloud_config();
}

int qid_sys_get_device_name (char *name, size_t buff_len)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(name, p_qid_config->device_name, buff_len);

  return ret;
}

int qid_sys_get_device_type(char *type, size_t buff_len)
{
  strncpy(type, "MobileNas", buff_len);
}

int qid_sys_set_qid_status(char *status)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(p_qid_config->status, status, STATUS_FLAG_LEN);

  // save config
  return save_nvram_qcloud_config();
}

int qid_sys_get_qid_status(char *status, size_t buff_len)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(status, p_qid_config->status, buff_len);

  return ret;
}

int qid_sys_set_device_token(char *device_token)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->device_token, device_token, MAX_APPKEY_LEN);

  // save config
  return save_nvram_qcloud_config();
}

int qid_sys_get_device_token(char *device_token, size_t buff_len)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(device_token, p_qid_config->device_token, buff_len);

  return ret;
}

int qid_sys_set_qid_device_id(char *did)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->device_id, did, NAME_LEN);

  // save config
  return save_nvram_qcloud_config();
}

int qid_sys_get_qid_device_id(char *did, size_t buff_len)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(did, p_qid_config->device_id, buff_len);

  return ret;
}

int qid_sys_set_ddns_status(char *status)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  if (0 == strncmp(status, QID_SYS_ENABLE_STR, strlen(QID_SYS_ENABLE_STR))) {
    strncpy(p_qid_config->ddns_status, QID_SYS_TRUE_STR, STATUS_FLAG_LEN);
  }
  else {
    strncpy(p_qid_config->ddns_status, QID_SYS_FALSE_STR, STATUS_FLAG_LEN);
  }

  // save config
  ret = save_nvram_qcloud_config();

  // update hash
  if(strncmp(QID_SYS_DISABLE_STR, status, strlen(QID_SYS_DISABLE_STR)) == 0){
    qid_sys_set_hash_string("ddns", "\0");
  }

  // log message
  QNAP_LOG_INFO_NL("Set ddns to: %s", status);

  return ret;
}

int qid_sys_get_ddns_status(char *status, size_t buff_len)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(status, p_qid_config->ddns_status, buff_len);

  return ret;
}

int qid_send_sys_event_log(unsigned int level, char *prev_msg, char* msg)
{
  // compare the current & previous message to avoid flooding
  if(strcmp(prev_msg , msg) == 0){
    return 0;
  }

  // update prev message
  strncpy(prev_msg, msg, MAX_SYSTEM_CMD);

#if 1
  if( level == QNAP_EVENT_TYPE_INFO )
  {
    QNAP_LOG_INFO_NL(msg);
  }
  else if( level == QNAP_EVENT_TYPE_WARN )
  {
    QNAP_LOG_WARN_NL(msg);
  }
  else
  {
    QNAP_LOG_ERR_NL(msg);
  }
#else
  char cmd[MAX_SYSTEM_CMD+1];
  char data[TMP_BUFF_LEN+1];

  // prepare the command
  snprintf(cmd, 1024*sizeof(char), "/sbin/log_tool -t %d -a \"%s%s\"", level, ERROR_MYQNAPCLOUD, msg);

  // send the event log
  return util_system_cmd(buff, temp, TMP_BUFF_LEN);
#endif

}

int qid_sys_get_hash_string(char* request_type, char* output, size_t buff_len)
{
  char cmd[MAX_SYSTEM_CMD+1];
  snprintf(cmd, MAX_SYSTEM_CMD, GET_QID_REQ_HASH, request_type);
  return util_system_cmd(cmd, output, buff_len);
}

int qid_sys_set_hash_string(char* hash_type, char* input)
{
  char set_cmd[MAX_SYSTEM_CMD+1];
  char data[MAX_SYSTEM_CMD+1];

  snprintf(set_cmd, MAX_SYSTEM_CMD, SET_QID_REQ_HASH, input, hash_type);

  return util_system_cmd(set_cmd, data, MAX_SYSTEM_CMD);
}

int qid_sys_set_hash_string_ttl(char* hash_type, int period)
{
  char set_cmd[MAX_SYSTEM_CMD+1];
  char data[MAX_SYSTEM_CMD+1];
  snprintf(set_cmd, MAX_SYSTEM_CMD, SET_QID_REQ_HASH_TTL, period, hash_type);

  return util_system_cmd(set_cmd, data, MAX_SYSTEM_CMD);
}

int qid_sys_get_hash_string_ttl(char* request_type, char* output, size_t buff_len)
{
  char set_cmd[MAX_SYSTEM_CMD+1];
  snprintf(set_cmd, MAX_SYSTEM_CMD, GET_QID_REQ_HASH_TTL, request_type);
  return util_system_cmd(set_cmd, output, buff_len);
}

int qid_sys_get_curl_log_flag(char* curl_flag, size_t buff_len)
{
  // FIXME : fix the value now
  strncpy(curl_flag, "80682695", buff_len);
  return NO_ERROR;
}

int qid_sys_set_qcloud_ver(char *version)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->version, version, VERSION_LEN);

  // save config
  return save_nvram_qcloud_config();
}

int qid_sys_get_qcloud_ver(char *version, size_t buff_len)
{
  int ret = ERR_NULL_POINTER;

  // check input
  if(!version){
    return ret;
  }

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(version, p_qid_config->version, buff_len);

  return ret;
}

int qid_sys_set_app_token(char *value)
{
  int ret = NO_ERROR;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // modify value
  strncpy(p_qid_config->app_token, value, MAX_APPKEY_LEN);

  // save config
  return save_nvram_qcloud_config();
}

int qid_sys_get_app_token(char *value, size_t buff_len)
{
  int ret = ERR_NULL_POINTER;

  // load from nvram
  ret = load_nvram_qcloud_config(0);
  if( ret != NO_ERROR ) return ret;

  // get the config
  strncpy(value, p_qid_config->app_token, buff_len);

  return ret;
}

int qid_sys_set_last_update_status( int error_code )
{
  int ret_code = NO_ERROR;

  // command buffer
  char cmd[MAX_SYSTEM_CMD+1];

  // data buffer
  char data[TMP_BUFF_LEN+1];

  // set status
  snprintf(cmd, MAX_SYSTEM_CMD, SAVE_NVRAM_LAST_UPDATE_STATUS, error_code);
  util_system_cmd(cmd, data, TMP_BUFF_LEN);

  // check if success
  if( error_code == NO_ERROR )
    snprintf(cmd, MAX_SYSTEM_CMD, SAVE_NVRAM_LAST_UPDATE_SUCESS_TIME, time(NULL));
  else
    snprintf(cmd, MAX_SYSTEM_CMD, SAVE_NVRAM_LAST_UPDATE_FAIL_TIME, time(NULL));

  // execute set time command
  util_system_cmd(cmd, data, TMP_BUFF_LEN);

  return ret_code;
}

int qid_sys_get_last_update_status( int* p_error_code, time_t* p_last_sucess_timestamp, time_t* p_last_fail_timestamp )
{
  int ret_code = NO_ERROR;

  // data buffer
  char data[TMP_BUFF_LEN+1];

  // check input
  if( p_error_code == NULL || p_last_sucess_timestamp == NULL || p_last_fail_timestamp == NULL )
    return ERR_NULL_POINTER;

  // get status
  util_system_cmd(LOAD_NVRAM_LAST_UPDATE_STATUS, data, TMP_BUFF_LEN);
  *p_error_code = atoi(data);

  // get last success time
  util_system_cmd(LOAD_NVRAM_LAST_UPDATE_SUCESS_TIME, data, TMP_BUFF_LEN);
  *p_last_sucess_timestamp = atoi(data);


  // get last fail time
  util_system_cmd(LOAD_NVRAM_LAST_UPDATE_FAIL_TIME, data, TMP_BUFF_LEN);
  *p_last_fail_timestamp = atoi(data);

  return ret_code;
}

int qid_sys_restart_porter()
{
  int ret = system(RESTART_PORTER);
  return ret;
}

int qid_sys_set_last_get_external_ip_time()
{
  char cmd[MAX_SYSTEM_CMD+1];
  char data[TMP_BUFF_LEN+1];

  // set last get wan ip time time
  snprintf(cmd, MAX_SYSTEM_CMD,
    "/bin/echo -n '%d' > " QCLOUD_VAR_CACHE_FOLDER "/ExternalIP_Time",
    (int)time(NULL));

  return util_system_cmd(cmd, data, MAX_SYSTEM_CMD);
}

int qid_sys_get_last_get_external_ip_time()
{
  char data[TMP_BUFF_LEN+1];

  // get time
  util_system_cmd("/bin/cat " QCLOUD_VAR_CACHE_FOLDER "/ExternalIP_Time 2>/dev/null", data, TMP_BUFF_LEN);

  // check if have value
  if( strlen(data) <= 0 )
    return 0;

  return atoi(data);
}

int qid_sys_set_tmp_external_ip(char* ext_ip)
{
  char cmd[MAX_SYSTEM_CMD+1];
  char data[TMP_BUFF_LEN+1];

  // set last get wan ip
  snprintf(cmd, MAX_SYSTEM_CMD,
    "/bin/echo -n '%s' > " QCLOUD_VAR_CACHE_FOLDER "/ExternalIP_IP",
    ext_ip);

  return util_system_cmd(cmd, data, TMP_BUFF_LEN);
}

int qid_sys_get_tmp_external_ip(char* ext_ip, size_t buff_len )
{
  return util_system_cmd("/bin/cat " QCLOUD_VAR_CACHE_FOLDER "/ExternalIP_IP 2>/dev/null", ext_ip, buff_len);
}
