/*
 * util.c ---  common utility routines.
 */

#include "std_hdr.h"
#include <ctype.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/types.h>
#include <regex.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "comm_hdr.h"
#include "error_code.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define REG127 "^127\\.0\\.0\\.."
#define REG192 "^192\\.168\\.."
#define REG10 "^10\\.0\\.."
#define REG172 "^172\\.1[6-9]\\.."
#define REG0 "^0\\.."
#define REG169 "^169\\.254\\.."
#define REG_MULTICAST1 "^22[4-9]\\.."
#define REG_MULTICAST2 "^23[0-9]\\.."
#define REG_IPV4 "^[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}"


char* reg_rules[] =
{
  REG127, REG192, REG10, REG172,
  REG0, REG169, REG_MULTICAST1, REG_MULTICAST2
};

enum
{
  reg127,
  reg192,
  reg10,
  reg172,
  reg0,
  reg169,
  reg_multicast1,
  reg_multicast2,
  num_reg_rules
};

#include <resolv.h>
#include <netdb.h>
#define SET_SERV_ADDR(S, HOST, PORT)      do{                         \
    struct  hostent *hp;                                              \
    hp = gethostbyname(HOST);                                         \
    memset(&(S), 0x0, sizeof(S));                                     \
    if(hp){                                                           \
            memcpy(&(S).sin_addr.s_addr, hp->h_addr, hp->h_length);   \
            (S).sin_family = AF_INET;                                 \
            (S).sin_port = htons(PORT);                               \
    }else{                                                            \
      if(-1 == res_init()){                                           \
        exit(-1);                                                     \
      }                                                               \
    }                                                                 \
}while(0)


void util_reslove_host(struct sockaddr_in *addr, URL_T *serv_url)
{
  SET_SERV_ADDR(*addr, serv_url->host_name, serv_url->port);
}

/*
 * util_setup_signal()
 *
 * Setup signal handler.
 *
 * Input:
 *      @ signo       -- signal number.
 *      @ signal_func -- signal handler.
 *
 * Output:
 *      @ NO_ERROR   for successfully set up signal handler.
 *      @ ERR_SYSTEM for failed set up.
 */
int util_setup_signal (int signo, void (*signal_func)(int))
{
  if (signal(signo, signal_func) == SIG_ERR) {
    QNAP_LOG_ERR_NL("Cannot setup signal handler for %d", signo);
    return(ERR_SIGNAL);
  }
  return(NO_ERROR);
}

/*
 * util_get_daemon_name()
 *
 * Return name without directory.
 *
 * Input:
 *      @ prog_name -- name of daemon with or without path.
 *
 * Output:
 *      @ name of daemon without path.
 */
char *util_get_daemon_name (char *prog_name)
{
  char  *daemon_name;

  daemon_name = strrchr(prog_name, DIR_SPLITTER_CHAR);
  return(daemon_name ? (daemon_name + 1) : prog_name);
}

/*
 * util_show_version()
 *
 * Show version number and bulid time.
 *
 * Input:
 *      @ release_ver -- string of release version.
 *      @ source_ver  -- string of source version.
 *      @ build_time  -- time when program is built.
 *
 * Output:
 *      @ ERR_GENERIC to notify caller to quit.
 */
int util_show_version (char *release_ver, char *source_ver,
                             char *build_time)
{
  fprintf(stdout, "Release Version: %s, Source Version: %s, Bulid time: %s\n",
          release_ver, source_ver, build_time);
  /*
   * Always return an error.
   */
  return(ERR_GENERIC);
}

/*
 * util_show_usage()
 *
 * Show how to invoke execution of the program.
 *
 * Input:
 *      @ daemon_name       -- name of running daemon.
 *      @ opt_help_tbl      -- option help table definition.
 *      @ opt_help_tbl_size -- size of option help table structure.
 *
 * Output:
 *      @ ERR_GENERIC to notify caller to quit.
 */
int util_show_usage (char *daemon_name, UTIL_USAGE_T *opt_help_tbl,
                           size_t opt_help_tbl_size)
{
  UTIL_USAGE_T    *opt;           // working pointer of option element
  char                  cmd[DWORD_ALIGN(DISP_BUFF_LEN)];// command buffer
  char                  *prefix = "   ";
  char                  empty_daemon[DWORD_ALIGN(DISP_BUFF_LEN)];
  int                   opt_count;      // number of options
  int                   i;              // working variable

  memset(empty_daemon, ' ', strlen(daemon_name));
  empty_daemon[strlen(daemon_name)] = '\0';

  fprintf(stdout, "\nUsage:\n");
  sprintf(cmd, "%s%s", prefix, daemon_name);

  opt_count = (int)(opt_help_tbl_size / sizeof(UTIL_USAGE_T));
  for (i = 0, opt = opt_help_tbl; i < opt_count; i++, opt++) {
    if ((strlen(cmd) + strlen(opt->cmd_opt)) >= DISP_BUFF_LEN) {
      fprintf(stdout, "%s\n", cmd);
      sprintf(cmd, "%s%s", prefix, empty_daemon);
    }
    sprintf(&cmd[strlen(cmd)], " %s", opt->cmd_opt);
  }
  fprintf(stdout, "%s\n\n", cmd);

  for (i = 0, opt = opt_help_tbl; i < opt_count; i++, opt++) {
    fprintf(stdout, "%s%s", prefix, opt->help_str);
    switch (opt->type) {
      case UTIL_DEF_BOOL:
        fprintf(stdout, ", default: %s\n",
                opt->param.def_value ? "True" : "False");
        break;

      case UTIL_DEF_INT:
        fprintf(stdout, ", default: %d\n", opt->param.def_value);
        break;

      case UTIL_DEF_STR:
        fprintf(stdout, ", default: \"%s\"\n", opt->param.def_string);
        break;

      case UTIL_DATA_RANGE:
        fprintf(stdout, ", data range [%d-%d], default: %d\n",
                opt->param.range.min_value, opt->param.range.max_value,
                opt->param.range.def_value);
        break;

      case UTIL_DEF_NONE:
      default:
        fprintf(stdout, "\n");
        break;
    }
  }
  fprintf(stdout, "\n");

  /*
   * Always return an error.
   */
  return(ERR_GENERIC);
}

/*
 * util_strip_mac()
 *
 * Remove ':' in between.
 *
 * Input:
 *      @ src_mac -- incoming source mac address string with or without ':'
 *                   in between.
 *      @ dst_mac -- mac address without ':' in between to be returned.
 *
 * Output:
 *      @ dst_mac -- mac address string without ':' in between.
 *                   or NULL string for any invalid format source string.
 */
void util_strip_mac (char *src_mac, char *dst_mac)
{
  char  *src;                           // working pointer for source mac string
  char  *dst = dst_mac;                 // working pointer for dest mac string
  char  c;                              // working variable
  int   len = 0;                        // length of mac address
  int   colon = 0;                      // number of ':' in source mac string

  /*
   * Sanity check.
   */
  if (!dst_mac) {
    QNAP_LOG_ERR_NL("Parameter dst_mac is NULL");
    return;
  }

  /*
   * Init. to null first.
   */
  *dst = '\0';

  /*
   * Remove leading and trailing spaces.
   */
  src = util_trim(src_mac, FALSE);
  if (!src)
    return;

  while ((c = *src++) != '\0') {
    if (c == ':') {
      colon++;
      continue;
    }

    *dst++ = c;
    if ((++len) == MAC_STR_LEN) {
      /*
       * Null terminated string.
       */
      *dst = '\0';
      if (*src != '\0') {
        colon = 1;
      }
      break;
    }
  }

  /*
   * Legal format would be xx:xx:xx:xx:xx:xx
   * so there should be 5 colon in between and 12 bytes length of mac data.
   */
  if ((colon && colon != 5) || (len != MAC_STR_LEN)) {
    *dst_mac = '\0';
  }
}

/*
 * util_verify_mac()
 *
 * Verify the given mac address string characters are all valid hex digits,
 * and then convert it to 6 bytes hex address format.
 *
 * Input:
 *      @ mac_str  -- mac address in 12 bytes string format.
 *      @ mac_addr -- for output.
 *
 * Output:
 *      @ TRUE  for successful conversion and
 *      @ mac_addr -- the converted mac address and null-terminated.
 *
 *      @ FALSE for any error.
 */
int util_verify_mac (char *mac_str, uint8_t *mac_addr)
{
  char  *mac;                           // working pointer
  int   i;                              // working variable

  /*
   * Verify all the string characters are hex digits.
   */
  for (i = 0, mac = mac_str; i < MAC_STR_LEN; i++, mac++) {
    if (!isxdigit(*mac))
      return(FALSE);
  }

  /*
   * Convert string to 6 bytes address data with null-terminated.
   */
  MAC_ADDR_CONV2HEX(mac_addr, mac_str);
  mac_addr[ETHER_ADDR_LEN] = '\0';

  return(TRUE);
}

/*
 * util_str_to_upper()
 *
 * Convert a string to upper case.
 *
 * Input:
 *      @ dst_str -- converted string to be returned.
 *      @ src_str -- source string to be converted.
 *      @ len     -- length of string.
 *
 * Output:
 *      @ dst_str -- converted string.
 */
void util_str_to_upper (char *dst_str, char *src_str, int len)
{
  char  c;                              // working variable
  int   i;                              // working variable

  for (i = 0; i < len; i++) {
    c = *src_str++;
    *dst_str++ = toupper(c);
  }
}

/*
 * util_read_string()
 *
 * Read in user input ASCII string and make sure the length won't exceed
 * maximum length.
 *
 * Input:
 *      @ str        -- user input string.
 *      @ str_save   -- copied string to be returned.
 *      @ param_name -- name of parameter.
 *      @ max_len    -- maximum length of string allowed for this parameter.
 * Outupt:
 *      @ NO_ERROR for successfully copy the string and
 *      @ str_save   -- copy of input string.
 *
 *      @ ERR_LEN_TOO_LONG for buffer is not big enough.
 */
int util_read_string (char *str, char *str_save, char *param_name,
                            size_t max_len)
{
  if (strlen(str) <= max_len) {
    strcpy(str_save, str);
    return(NO_ERROR);
  } else {
    fprintf(stdout, "Length of [%s] cannot exceed %zd\n", param_name, max_len);
    return(ERR_STR_TOO_LONG);
  }
}

/*
 * util_read_integer()
 *
 * Read in user input integer and make sure it is in the data range.
 *
 * Input:
 *      @ int_str -- integer string of user input.
 *      @ int_val -- integer to be returned.
 *      @ min_val -- minimum integer allowed.
 *      @ max_val -- maximum integer allowed.
 *
 * Output:
 *      @ NO_ERROR for successfully retrieving integer and
 *      @ int_val -- integer converted from string.
 *
 *      @ ERR_INVALID_FORMAT for invalid integer format or integer
 *        value is out of range.
 */
int util_read_integer (char *int_str, int *int_val, int min_val,
                             int max_val)
{
  int   tmp;                            // working variable
  char  c;                              // working variable
  int   n;                              // working variable

  /*
   * Skip leading and trailing spaces.
   */
  if ((int_str = util_trim(int_str, FALSE)) == NULL) {
    fprintf(stdout, "Empty interger value string");
    return(ERR_INVALID_FORMAT);
  }

  n = sscanf(int_str, "%d%c", &tmp, &c);
  if ((n == 1) && (tmp >= min_val) && (tmp <= max_val)) {
    *int_val = tmp;
    return(NO_ERROR);
  } else {
    fprintf(stdout, "Data value must be in the range [%d - %d]\n",
            min_val, max_val);
    return(ERR_INVALID_VALUE);
  }
}

/*
 * util_read_mac_str()
 *
 * Check if the given string is legal mac address format.
 *
 * Input:
 *      @ mac_str      -- mac address string to be parsed.
 *      @ mac_str_save -- copied mac address string to be returned.
 *      @ mac_addr     -- converted mac address to be returned.
 *
 * Output:
 *      @ NO_ERROR for successfully retrieving mac address string and
 *      @ mac_str_save -- copied mac address string.
 *      @ mac_addr     -- mac address in binary hex format.
 *
 *      @ ERR_INVALID_FORMAT for invalid mac address format.
 */
int util_read_mac_str (char *mac_str, char *mac_str_save,
                             uint8_t *mac_addr)
{
  char      tmp_mac[DWORD_ALIGN(MAC_STR_LEN + 1)]; // tmp buffer for mac string
  uint32_t  tmp[DWORD_ALIGN(ETHER_ADDR_LEN)];      // tmp buffer for mac address
  char      c;                          // working variable
  int       n;                          // working variable

  /*
   * Remove ':' in between and conver string to upper case first.
   */
  util_strip_mac(mac_str, tmp_mac);
  util_str_to_upper(tmp_mac, tmp_mac, strlen(tmp_mac));
  n = sscanf(tmp_mac, "%02X%02X%02X%02X%02X%02X%c",
             &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4], &tmp[5], &c);
  if (n == ETHER_ADDR_LEN) {
    if (mac_str_save) {
      strcpy(mac_str_save, tmp_mac);
    }
    for (n = 0; n < ETHER_ADDR_LEN; n++) {
      mac_addr[n] = tmp[n] & 0x0FF;
    }
    return(NO_ERROR);
  } else {
    fprintf(stdout, "Invalid MAC address\n");
    return(ERR_INVALID_VALUE);
  }
}

/*
 * util_read_ipv4_str()
 *
 * Check if the given string is legal ipv4 dotted-decimal format.
 *
 * Input:
 *      @ ip_str      -- IP address string to be parsed.
 *      @ ip_str_save -- copied IP string to be returned.
 *      @ ip_addr     -- converted IP address to be returned.
 *      @ print_err   -- flag indicating if need to output error info.
 *
 * Output:
 *      @ NO_ERROR for successfully retrieving IP address string and
 *      @ ip_str_save -- copied IP address string.
 *      @ ip_addr     -- IP address in network byte order.
 *
 *      @ ERR_INVALID_FORMAT for invalid IP address format.
 */
int util_read_ipv4_str (char *ip_str, char *ip_str_save, in_addr_t *ip_addr,
                        int print_err)
{
  uint32_t  tmp[DWORD_ALIGN(IPv4_ALEN)];// tmp buffer for IP address
  int       n;                          // working variable
  char      c;                          // working variable

  /*
   * Remove leading and trailing spaces.
   */
  if ((ip_str = util_trim(ip_str, FALSE)) == NULL) {
    fprintf(stdout, "Empty IP address string");
    return(ERR_INVALID_FORMAT);
  }

  n = sscanf(ip_str, "%3d.%3d.%3d.%3d%c",
             &tmp[0], &tmp[1], &tmp[2], &tmp[3], &c);
  if (n == IPv4_ALEN) {
    if (ip_str_save) {
      strcpy(ip_str_save, ip_str);
    }
    inet_aton(ip_str, (struct in_addr *)ip_addr);
    return(NO_ERROR);
  } else {
    if (print_err) {
      fprintf(stdout, "Invalid IP address\n");
    }
    return(ERR_INVALID_VALUE);
  }
}

int util_read_domain_port_str(char * domain_port_str, char *domain_str_save,
                              uint16_t *port, int def_port, int print_err)
{
  char      domain[URL_STR_LEN+1];
  int       n;                          // working variable
  int       tmp_port;                   // tmp buffer for Port number.

  /*
   * Remove leading and trailing spaces.
   */
  if ((domain_port_str = util_trim(domain_port_str, FALSE)) == NULL) {
    fprintf(stdout, "Empty IP address string");
    return(ERR_INVALID_FORMAT);
  }
  tmp_port = def_port;
  n = sscanf(domain_port_str, "%99[^:]:%5d[^\n]", domain, &tmp_port);
  if ( n > 0 ) {
    if (domain_str_save) {
      strncpy(domain_str_save, domain, URL_STR_LEN);
    }
    if(port){
      *port = (uint16_t)tmp_port;
    }
    return(NO_ERROR);
  } else {
    if (print_err) {
      fprintf(stdout, "Invalid IP address: %s\n", domain_port_str);
    }
    return(ERR_INVALID_VALUE);
  }
}

/*
 * util_read_ipv4_port_list()
 *
 * Check if the given IP:Port list is legal, the valid format is
 *  ip1:port1, ip2:port2, ip3:port3, ...
 *
 * Input:
 *      @ ip_port_str    -- incoming IP:port string to be parsed.
 *      @ ip_list        -- list array of IP:port to be returned.
 *      @ max_num        -- maximum number of IP:port pairs allowed.
 *      @ def_port       -- default port number if :port is missing.
 *      @ count          -- number of IP:port to be returned.
 *      @ ip_dotted_only -- flag indicating that IP address must in dotted-
 *                          decimal format, i.e IP name is not allowed.
 *
 * Output:
 *      @ NO_ERROR for successfully parsing IP:port string and
 *      @ ip_list        -- list array of each IP:port.
 *      @ count          -- number of good IP:port pairs.
 *
 *      @ ERR_xxxx for any kind of error.
 */
int util_read_ipv4_port_list (char *ip_port_str, URL_T *ip_list,
                                    int max_num, int def_port, int *count,
                                    int ip_dotted_only)
{
  char      tmp_buff[DWORD_ALIGN(ADDR_STR_LEN + 1)]; // IP:port list
  int       pair_num = 0;               // number of good IP:port address
  char      *start, *end, *ptr;         // working pointers
  char      *list_sep, *port_sep;       // working pointers
  char      *path_sep;
  char      protocol[DWORD_ALIGN(URL_STR_LEN + 1)]; // protocol string
  in_addr_t ip_addr;                    // IP address in network byte order
  int       port;                       // port number
  int       len;                        // working variable
  int       rc;                         // return code

  /*
   * Copy string to temp buffer to parse, i.e. not to modify the original
   * string.
   */
  if ((len = strlen(ip_port_str)) > ADDR_STR_LEN)
    return(ERR_STR_TOO_LONG);

  strcpy(tmp_buff, ip_port_str);
  start = tmp_buff;
  end = start + len;
  while ((start < end) && (pair_num < max_num)) {
    /*
     * Skip protocol prefix.
     */
    len = 0;
    if ((ptr = strstr(start,  URL_NAME_PREFIX_STR)) != NULL) {
      ptr += strlen(URL_NAME_PREFIX_STR);
      len = (ptr - start);
      memcpy(protocol, start, len);
      start = ptr;
    }
    protocol[len] = '\0';

    /*
     * Look for separate charater first.
     */
    if ((list_sep = strchr(start, IP_PORT_LIST_SEP_CHAR)) != NULL) {
      *list_sep = '\0';
    }

    /*
     * Look for path separate character
     */
    if ((path_sep = strchr(start, URL_NAME_PATH_CHAR)) != NULL) {
      *path_sep = '\0';
    }

    /*
     * Remove leading and trailing spaces.
     */
    if ((start = util_trim(start, FALSE)) == NULL)
      break;

    /*
     * Then check if port is provided, set to default port if not provided.
     */
    port = def_port;
    if ((port_sep = strchr(start, IP_PORT_SEP_CHAR)) != NULL) {
      *port_sep = '\0';
      if (util_read_integer((port_sep + 1), &port, PORT_MIN, PORT_MAX)
          != NO_ERROR)
        goto err_ret;
    }

    /*
     * Parse IP string, convert to network byte format if the given IP string
     * is valid dotted-decimal format.
     */
    rc = util_read_ipv4_str(start, NULL, &ip_addr, ip_dotted_only);
    if (ip_dotted_only && (rc != NO_ERROR))
      goto err_ret;

    /*
     * Restore port seperate character back.
     */
    if (port_sep) {
      *port_sep = IP_PORT_SEP_CHAR;
    }

    if (path_sep) {
      *path_sep = URL_NAME_PATH_CHAR;
    }

    /*
     * find URL path
     */
    if ((ptr = strstr(ip_port_str, URL_NAME_PREFIX_STR)) != NULL)
    {
      ptr += strlen(URL_NAME_PREFIX_STR);
    }
    else
      ptr = ip_port_str;
    if ((ptr = strchr(ptr, URL_NAME_PATH_CHAR)) != NULL) {
      ip_list->path = ptr+1;
    }
    else {
      ip_list->path = NULL;
    }

    strcpy(ip_list->addr_str, protocol);
    strcat(ip_list->addr_str, start);
    ip_list->ip_addr = (rc == NO_ERROR) ? ip_addr : 0;
    ip_list->port = port;
    ip_list++;
    pair_num++;

    /*
     * Restore list seperator character back.
     */
    if (list_sep) {
      *list_sep = IP_PORT_LIST_SEP_CHAR;
      start = list_sep + 1;
    } else {
      start = end;
    }
  }

  if (count) {
    *count = pair_num;
  }
  return(NO_ERROR);

err_ret:
  return(ERR_INVALID_FORMAT);
}

/*
 * util_trim()
 *
 * Remove leading and trailing spaces.
 * If argument 'leading_only' is TRUE then only remove leading spaces.
 *
 * Input:
 *      @ str          -- string to be removed spaces.
 *      @ leading_only -- flag indicating if only leading spaces need to be
 *                        taken care of.
 *
 * Output:
 *      @ string without leading spaces and/or without trailing spaces.
 */
char *util_trim (char *str, int leading_only)
{
  char  *start = str, *end, c;          // working varialbes

  /*
   * Sanity check.
   */
  if (!str) {
    QNAP_LOG_ERR_NL("Parameter str is NULL");
    return(NULL);
  }

  /*
   * Start from the beginning, find the first non-space character.
   */
  while ((c = *start) != '\0') {
    if (!isspace(c))
      break;
    start++;
  }

  /*
   * String contains all the spaces or string is null.
   */
  if (*start == '\0')
    return(NULL);

  /*
   * Stop to remove trailing spaces if asked to.
   */
  if (leading_only)
    return(start);

  /*
   * Start from the end of staring, backward scan to find the first non-space
   * character.
   */
  end = start + strlen(start) - 1;

  while (end > start) {
    if (isspace(*end)) {
      end--;
    } else {
      /*
       * Always null-terminated string before return.
       */
      *(end + 1) = '\0';
      break;
    }
  }

  return(start);
}

/*
 * util_trim_quotes()
 *
 * Remove pair of double quotes or single quote of a string.
 * (The given string has been trimed out without leading spaces).
 *
 * Input:
 *      @ str -- string to be taken out quotes.
 *
 * Output:
 *      @ string without quotes.
 */
char *util_trim_quotes (char *str)
{
  char  c;                              // working variable
  char  *end;                           // end of string

  /*
   * Sanity check.
   */
  if (!str) {
    QNAP_LOG_ERR_NL("Parameter str is NULL");
    return(NULL);
  }

  c = *str;
  if ((c == DOUBLE_QUOTE_CHAR) || (c == SINGLE_QUOTE_CHAR)) {
    if ((end = strchr((str + 1), c)) != NULL) {
      str++;
      *end = '\0';
    }
  }

  return(str);
}

/*
 * util_open_file()
 *
 * Wrapper routine to open file.
 *
 * Input:
 *      @ file_name -- name of opening file.
 *      @ mode      -- i.e. create, read, write, attach ...
 *
 * Output:
 *      @ file descriptor for successfully open the file.
 *
 *      @ NULL for failed to open file.
 */
FILE *util_open_file (char *file_name, char *mode)
{
  FILE  *fp;                            // file descriptor

  if ((fp = fopen(file_name, mode)) == NULL) {
    QNAP_LOG_WARN_NL("Cannot open file[%s]", file_name);
  }
  return(fp);
}

/*
 * util_close_file()
 *
 * Wrapper routine to close file.
 *
 * Input:
 *      @ fp -- file descriptor of the closed file.
 *
 * Output:
 *      None.
 */
void util_close_file (FILE *fp)
{
  if (fp) {
    fclose(fp);
  }
}

/*
 * util_inet_ntoa()
 *
 * Wrapper routine to resolve strict-alilasing rules to convert network
 * byte order IP address to a string in dotted-decimal form.
 *
 * Input:
 *      @ ip_addr -- IP address with uint32_t type in network byte order.
 *
 * Output:
 *      @ dotted-decimal format string of IP address.
 */
char *util_inet_ntoa (uint32_t ip_addr)
{
  struct in_addr    in;

  in.s_addr = ip_addr;
  return(inet_ntoa(in));
}

void util_strip(char *s)
{
#if 0
   char *p2 = s;
   while(s && *s != '\0') {
      if(*s != '\t' && *s != '\n' && *s != '\r') {
         *p2++ = *s++;
      } else {
         ++s;
      }
   }

   if(p2){
     *p2 = '\0';
   }
#else

   if(s){

     int len = strlen(s);
     int i = len -1;

     for(; i >= 0; i--){
       switch (s[i]){
       case '\t':
       case '\n':
       case '\r':
         s[i] = '\0';;
         break;
       default:
         return;
       }
     }

   }

#endif
}

int util_system_cmd(char *cmd_buff, char *data_buff, size_t data_buff_len)
{

  //char  *tmp_buff;                      // resolve compile warning
  FILE  *fp;                            // file descriptor
 
  if(!cmd_buff || !data_buff || data_buff_len <= 0){
    QNAP_LOG_ERR_NL("invalid parameters");
    return ERR_NULL_POINTER;
  }

  memset(data_buff, 0x0, data_buff_len);
  if ((fp = popen(cmd_buff, "r")) != NULL) {
    if( fgets(data_buff, data_buff_len, fp) != NULL){
	/* nothing to do*/
    }
    pclose(fp);
    util_strip(data_buff);
  }else{
    QNAP_LOG_ERR_NL("popen failed: cmd: %s: err: %s", cmd_buff, strerror(errno));
  }

  return NO_ERROR;

}

void util_daemonize(void)
{
    pid_t pid, sid;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* At this point we are executing as the child process */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null */
    //freopen( "/dev/null", "r", stdin);
    //freopen( "/dev/null", "w", stdout);
    //freopen( "/dev/null", "w", stderr);
}

/* Converts a hex character to its integer value */
static char from_hex(char ch)
{
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
static char to_hex(char code)
{
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}


int util_convert_hex_to_char( uint8_t *hex, size_t hex_len,
                                 char *str_buff, size_t buff_len)
{
  const static char ghex_string[]      = "0123456789abcdef";
  char *p;
  int i;

  //qnap_log_dump_data(7, RED, hex, hex_len);

  /*
   * convert hex to ascii string
   */
  for(p = str_buff, i=0; i < hex_len; i++){
      *p++ = ghex_string[(hex[i] >> 4) & 0xf];
      *p++ = ghex_string[hex[i] & 0xf];
  }

  *p = '\0';

  return NO_ERROR;
}

#define A2I(c)       ((c>='a'&&c<='z')?(c-'a'+10):((c>='A'&&c<='Z')?(c-'A'+10):(c-'0')))
int util_convert_char_to_hex( char *str, size_t str_len, uint8_t *hex_buff )
{
  int i ;
  uint8_t *phex = hex_buff;

  for(i = 0; i < str_len && phex; i+=2, phex++){
    *phex = A2I(str[i]);
    *phex = (*phex)<<4;
    *phex |= A2I(str[i+1]);
  }
  return NO_ERROR;
}


/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *util_url_encode(char *str)
{
  char *pstr = str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' ||
        *pstr == '~')
      *pbuf++ = *pstr;
    else if (*pstr == ' ')
      *pbuf++ = '+';
    else
      *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *util_url_decode(char *str)
{
  char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } else if (*pstr == '+') {
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

#ifdef CGI_INPUT
int util_parse_query_string(char *input_string, key_pair_st *fields)
{

  int i = 0;

  char *input;
  char *key;
  char *value;
  char *save_query;

  if (!input_string || strlen(input_string) == 0){
    QNAP_LOG_ERR_NL("invalid input string");
    return ERR_INPUT_LEN;
  }

  if(!fields){
    QNAP_LOG_ERR_NL("invalid fields");
    return ERR_INPUT_LEN;
  }

  input = strtok_r(input_string, DELIM_AND, &save_query);
  while(input && *input){

    value = strchr(input, '=');
    if(value){
      key = input;
      *value = '\0';
      value++;
      for( i = 0; fields[i].key[0] != '\0'; i++){
        if(0 == strncasecmp(fields[i].key, key, PAIR_KEY_LEN)){
          strncpy(fields[i].value, value, PAIR_VALUE_LEN);
          break;
        }
      }
    }

    input = strtok_r(NULL, DELIM_AND, &save_query);
  }

  for( i = 0; fields[i].key[0] != '\0'; i++){
    QNAP_LOG_DEBUG_NL("%s = [%s]", fields[i].key, fields[i].value);
  }
  return 0;
}

char* util_cgi_get_input()
{
  return getenv("QUERY_STRING");
}

int util_cgi_parse_input(key_pair_st *input_fields)
{

  char *cgi_input;
  char *input_string;

  if(!input_fields){
    return ERR_INVALID_VALUE;
  }

  cgi_input = util_cgi_get_input();

  input_string = util_url_decode(cgi_input);

  if(input_string){

    QNAP_LOG_DEBUG_NL("QUERY_STRING: %s", input_string);

    util_parse_query_string(input_string, input_fields);

    free(input_string);

  }

  return NO_ERROR;

}

void util_key_pair_list_items(key_pair_st *input_fields)
{
  key_pair_st *item = input_fields;
  for(;item && item->key[0] != '\0'; item++){
    QNAP_LOG_DEBUG_NL("%s = %s", item->key, item->value);
  }
}

void util_cgi_print_result(key_pair_st *output_fields)
{

  key_pair_st *item;

  if(!output_fields){
    return;
  }

  util_key_pair_list_items(output_fields);

  INSERT_HEADER_TERM();

  PRINT_CGI_RESP("{");

  for( item = output_fields; item != NULL ; ){
    PRINT_CGI_RESP("\"%s\": \"%s\"", item->key, item->value);
    item++;
    if(item->key[0] == '\0'){
      break;
    }
    PRINT_CGI_RESP(", ");
  }

  PRINT_CGI_RESP("}");

}
#endif

int util_is_valid_ipv4(char * ip_str){

  if (ip_str == NULL || strlen(ip_str) < 7 || strlen(ip_str) > INET_ADDRSTRLEN){
    return FALSE;
  }
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, ip_str, &(sa.sin_addr));
  //http://linux.die.net/man/3/inet_pton: returns 1 on success
  //(network address was successfully converted)
  return result == 1;
}

#if 0
int util_is_valid_ipv6(char * ip_str){

  if (ip_str == NULL || strlen(ip_str) == 0 || strlen(ip_str) > INET6_ADDRSTRLEN){
    return FALSE;
  }
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET6, ip_str, &(sa.sin_addr));
  return result == 1;
}
#endif

int util_is_public_ip(char* input_ip)
{
  if(input_ip == NULL)
    return ERR_NULL_POINTER;
  regex_t regex_struct;
  int ret = 0, i = 0;
  for( i = 0; i < num_reg_rules; i++){
    ret = regcomp( &regex_struct, reg_rules[i], 0);
    ret = regexec(&regex_struct, input_ip, 0, NULL, 0);
    regfree(&regex_struct);
    if( ret == REG_NOMATCH ){
      ret = TRUE;
    }else if( ret == 0){
      ret = FALSE;
      goto exit;
    }else{
      /*
        error occurs.
      */
      ret = -1;
      goto exit;
    }
  }

exit:
  return ret;
}

/*
 *  BE SURE TO FREE output after using this function
 */
int util_read_file_to_string(char* filename,char** output, size_t* string_size)
{
  int ret = ERR_SYSTEM;
  FILE* f = NULL;
  f = util_open_file(filename, "r");
  if(!f)
    goto exit;
  *output = (char*) calloc(sizeof(char), TMP_BUFF_LEN);
  *string_size = 0;
  char buf[TMP_BUFF_LEN+1];
  while(0 != (ret = fread(buf, 1, TMP_BUFF_LEN, f)) ){
    *output = realloc( *output, *string_size + ret +1);
    strncat( *output, buf, ret);
    *string_size += ret;
  }
  if( feof(f) ){
    ret = NO_ERROR;
  }else{
    ret = ferror(f);
  }

  util_close_file(f);
exit:
  return ret;
}

void util_convert_iso8601(const char *time_string, int ts_len, struct tm *tm_data)
{
  tzset();

  char temp[64];
  memset(temp, 0, sizeof(temp));
  strncpy(temp, time_string, ts_len);

  struct tm ctime;
  memset(&ctime, 0, sizeof(struct tm));
  strptime(temp, "%FT%T%z", &ctime);

  long ts = mktime(&ctime) - timezone;
  localtime_r(&ts, tm_data);
}

int util_auth_token_validator(char* str, size_t len)
{
  int ret = ERR_INVALID_FORMAT,cnt = 0;
  for(cnt = 0; cnt < len; cnt++){
    if( (str[cnt] >= 48 && str[cnt] <= 57) ||
        (str[cnt] >= 65 && str[cnt] <= 90) ||
        (str[cnt] >= 97 && str[cnt] <= 122)){
      ret = NO_ERROR;
    }else{
      ret = ERR_INVALID_FORMAT;
      break;
    }
  }
  return ret;
}

#define ISO8601_FORMAT  "%Y-%m-%dT%T"
void util_convert_to_utc_iso8601(time_t timer, char* output_str)
{
  struct tm* tm_info;
  struct timeval tv;
  char buffer[CMD_BUFF_LEN];
  char temp[CMD_BUFF_LEN];

  tm_info = gmtime(&timer);
  gettimeofday(&tv,NULL);
  strftime(buffer, CMD_BUFF_LEN, ISO8601_FORMAT, tm_info);
  snprintf(temp,CMD_BUFF_LEN,".%03d000", (int)tv.tv_usec/1000);
  strcat(buffer,temp);
  strcpy(output_str, buffer);
}

int util_get_value_by_key(char* input,char* key, char *value, size_t value_size)
{
  int nel = 0, len = strlen(input)+1;
  char *q, *name, *qvalue, *qry_str = (char*)malloc(len*sizeof(char));
  strncpy( qry_str, input, len*sizeof(char));
  q = qry_str;
  while (strsep(&q, "&")){
    nel++;
  }
  for (q = qry_str; q < (qry_str + len);) {
    qvalue = name = q;
    for (q += strlen(q); q < (qry_str + len) && !*q; q++);
    name = strsep(&qvalue, "=");
    if(strcmp(key,name) == 0){
      strncpy(value, qvalue, value_size);
    }
  }
  free(qry_str);
  return 0;
}


