/*
 * util.h --- utility routines header file.
 */

#ifndef __UTIL_H__
#define __UTIL_H__


#include <stdint.h>
#include <netinet/in.h>

#include "const.h"


/*
 * Offset for field in structure.
 */
#define OFFSET(T, F)            (size_t)(&(((T *)0)->F))
#define OFFSET_URL_ADDR()       OFFSET(URL_T, addr_str)

/*
 * Alignment to long word (4-bytes) boundary.
 */
#define DWORD_ALIGN(L)          ((((L) + 3) >> 2) << 2)

/*
 * Macro defines.
 */
#define BIT(B)                  (1 << (B))

/*
 * Display network byte order IP address in dotted-decimal format.
 */
#define IP_ADDR_DISP(Addr)      util_inet_ntoa((Addr))

/*
 * Display format for MAC string.
 */
#define MAC_DISP_STR_FMT        "[%c%c:%c%c:%c%c:%c%c:%c%c:%c%c]"
#define MAC_DISP_STR_DATA(M)    M[0], M[1], M[2], M[3], M[4], M[5],     \
                                M[6], M[7], M[8], M[9], M[10], M[11]
/*
 * Display MAC in hex format.
 */
#define MAC_DISP_HEX_FMT        "[%02X:%02X:%02X:%02X:%02X:%02X]"
#define MAC_DISP_HEX_DATA(M)    M[0], M[1], M[2], M[3], M[4], M[5]

#define MAC_DISP_COLON_FMT      "%02X:%02X:%02X:%02X:%02X:%02X"
#define MAC_DISP_COLON_DATA(M)  M[0], M[1], M[2], M[3], M[4], M[5]

#define MAC_ADDR_HEX_FMT        "%02X%02X%02X%02X%02X%02X"
#define MAC_ADDR_HEX_DATA(M)    M[0], M[1], M[2], M[3], M[4], M[5]

/*
 * Convert 6 bytes MAC address to 12 bytes format (in upper case).
 * Conver6 12 bytes MAC string (caller needs to verify all the characters
 * are valid hex digits) to 6 bytes MAC address.
 */
#define HEX_CHAR_VAL(C)         (((C) >= 'a') ? ((C)-'a'+10) :          \
                                 (((C)>= 'A') ? ((C)-'A'+10) : ((C)-'0')))
#define HEX_VAL_CHAR(V)         (((V) >= 10) ? ((V)-10+'A') : ((V)+'0'))

#define MAC_ADDR_CONV2STR(S, A)                                         \
  do {                                                                  \
    int _i, _j;                                                         \
                                                                        \
    for(_i = _j = 0; _i < ETHER_ADDR_LEN; _i++) {                       \
      S[_j++] = HEX_VAL_CHAR(A[_i] >> 4);                               \
      S[_j++] = HEX_VAL_CHAR(A[_i] & 0x0F);                             \
    }                                                                   \
    S[_j] = '\0';                                                       \
  } while (0)

#define MAC_ADDR_CONV2HEX(A, S)                                         \
  do {                                                                  \
    int _i, _j;                                                         \
                                                                        \
    for (_i = _j = 0; _i < ETHER_ADDR_LEN; _i++, _j += 2) {             \
      A[_i] = HEX_CHAR_VAL(S[_j]) << 4 | HEX_CHAR_VAL(S[_j+1]);         \
    }                                                                   \
  } while (0)

/*
 * Macro for MAC address operation.
 */
#define MAC_ADDR_COPY(D, S)             (memcpy((D), (S), ETHER_ADDR_LEN))
#define MAC_ADDR_COPY_WITH_NULL(D, S)                                   \
  do {                                                                  \
    memcpy((D), (S), ETHER_ADDR_LEN);                                   \
    (D)[ETHER_ADDR_LEN] = '\0';                                         \
  } while (0)
#define SAME_MAC_ADDR(A, B)             (memcmp((A), (B), ETHER_ADDR_LEN) == 0)

#define MYID_COPY(D, S)           (memcpy((D), (S), MYID_LEN))
#define SAME_MYID(A, B)           (memcmp((A), (B), MYID_LEN) == 0)

/*
 * Macro to get next index in range.
 */
#define NEXT_INDEX(I, T)        ((((I) + 1) == (T)) ? 0 : ((I) + 1))

/*
 * Key value pairing
 */
#define PAIR_KEY_LEN                  (64)
#define PAIR_VALUE_LEN                (1024)
#define PAIR_GET_KEY(NF, ID)          NF[ID].key
#define PAIR_GET_VALUE(NF, ID)        NF[ID].value

/*
 * Structure for daemon usage helper.
 */
#define UTIL_DEF_NONE     0       // no default value
#define UTIL_DEF_BOOL     1       // default is a boolean
#define UTIL_DEF_INT      2       // default is an interger
#define UTIL_DEF_STR      3       // default is a string
#define UTIL_DATA_RANGE   4       // data in range

typedef struct _key_pair{
  char key[PAIR_KEY_LEN+1];
  char value[PAIR_VALUE_LEN+1];
}key_pair_st;

#define NUM_KEY_PAIR(list)            (sizeof(list)/sizeof(key_pair_st))

#define DELIM_AND                      "&"
#define DELIM_EQU                      "="

#define PRINT_CGI_RESP(Fmt, Arg...)   fprintf(stdout, Fmt, ##Arg)
#define INSERT_HEADER_TERM()          PRINT_CGI_RESP("\r\n");

typedef struct _util_usage_st {
  char  *cmd_opt;                       // command option string
  char  *help_str;                      // option help string
  short type;                           // parameter type
  short filler;                         // alignment
                                        // default value
  union {
    int     def_value;                  // parameter's default value
    char    *def_string;                // parameter's string data
    struct {
      int   min_value;                  // parameter's minimum value
      int   max_value;                  // parameter's maximum value
      int   def_value;                  // parameter's default value
    } range;
  } param;
} UTIL_USAGE_T;

/*
 * Address in URL format.
 */
typedef struct _url_st {
  char      host_name[DWORD_ALIGN(URL_STR_LEN + 1)];// IP addres or host name + : + port num
  int       port;                                   // TCP/UDP/IPC port number
  char      *path;
  char      addr_str[DWORD_ALIGN(URL_STR_LEN + 1)];// IP addres + : + port num
  in_addr_t ip_addr;                               // IP address in network byte order
} URL_T;



/*
 * Forward variables declaration.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Forward functions declaration.
 */
extern void util_reslove_host(struct sockaddr_in *addr, URL_T *serv_url);
extern int  util_setup_signal(int signo, void (*signal_func)(int));
extern char *util_get_daemon_name(char *prog_name);
extern int  util_show_version(char *release_ver, char *source_ver,
                                    char *build_time);
extern int  util_show_usage(char *daemon_name,
                                  UTIL_USAGE_T *opt_help_tbl,
                                  size_t opt_help_tbl_size);
extern int  util_verify_mac(char *mac_str, uint8_t *mac_addr);
extern void util_str_to_upper(char *dst_str, char *src_str, int len);
extern int  util_read_string(char *str, char *str_save, char *param_name,
                                   size_t max_len);
extern void util_strip_mac(char *src_mac, char *dst_mac);
extern int  util_read_integer(char *int_str, int *int_val, int min_val,
                                    int max_val);
extern int  util_read_mac_str(char *mac_str, char *mac_str_save,
                                    uint8_t *mac_addr);
extern int  util_read_ipv4_str(char *ip_str, char *ip_str_save,
                                     in_addr_t *ip_addr, int print_err);
extern int  util_read_ipv4_port_list(char *ip_port_str,
                                           URL_T *ip_list, int max_num,
                                           int def_port, int *count,
                                           int ip_dotted_only);
int util_read_domain_port_str(char * domain_port_str, char *domain_str_save,
                              uint16_t *port, int def_port, int print_err);
extern char *util_trim(char *str, int leading_only);
extern char *util_trim_quotes(char *str);
extern FILE *util_open_file(char *file_name, char *mode);
extern void util_close_file(FILE *fp);
extern char *util_inet_ntoa(uint32_t ip_addr);
extern void util_strip(char *s);
extern int util_system_cmd(char *cmd_buff, char *data_buff, size_t data_buff_len);
extern int util_convert_hex_to_char( uint8_t *hex, size_t hex_len,
                              char *str_buff, size_t buff_len);
extern int util_convert_char_to_hex(char *str, size_t str_len,
                                    uint8_t *hex_buff );
extern void util_daemonize(void);
extern int util_parse_query_string(char *input_string, key_pair_st *fields);
extern char *util_url_encode(char *str);
extern char *util_url_decode(char *str);
extern char* util_cgi_get_input();
extern int util_cgi_parse_input(key_pair_st *input_fields);
extern void util_key_pair_list_items(key_pair_st *input_fields);
extern void util_cgi_print_result(key_pair_st *output_fields);
extern int util_is_valid_ipv4(char *ip_str);
extern int util_is_public_ip(char* input_ip);
extern int util_read_file_to_string(char* filename,char** output, size_t* string_size);
extern int util_auth_token_validator(char* str, size_t len);
extern void util_convert_to_utc_iso8601(time_t timer, char* output_str);
extern int util_get_value_by_key(char* input,char* key, char *value, size_t value_size);
extern void util_convert_iso8601(const char *time_string, int ts_len, struct tm *tm_data);

#define QNAP_SYSTEM_EVENT_INFO   0
#define QNAP_SYSTEM_EVENT_WARN   1
#define QNAP_SYSTEM_EVENT_ERROR  2

#define UTIL_QNAP_SEND_SYSTEM_EVENT(Lv, Fmt, Arg...)do{     \
    char msg[256];                                 \
    char resp[TMP_BUFF_LEN+1];                      \
    snprintf(msg, sizeof(msg),                     \
             "LD_LIBRARY_PATH=/usr:/usr/lib /sbin/log_tool -t %d -a \""  Fmt "\"", Lv, ##Arg);\
    util_system_cmd(msg, resp, TMP_BUFF_LEN);      \
  }while(0)

#ifdef __cplusplus
}
#endif

#endif /* __UTIL_H__ */
