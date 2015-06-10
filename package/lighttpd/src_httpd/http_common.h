#ifndef _HTTP_COMMON_H
#define _HTTP_COMMON_H

/* Copy each token in wordlist delimited by space into word */
#define foreach(word, wordlist, next) \
	for (next = &wordlist[strspn(wordlist, " ")], \
	     strncpy(word, next, sizeof(word)), \
	     word[strcspn(word, " ")] = '\0', \
	     word[sizeof(word) - 1] = '\0', \
	     next = strchr(next, ' '); \
	     strlen(word); \
	     next = next ? &next[strspn(next, " ")] : "", \
	     strncpy(word, next, sizeof(word)), \
	     word[strcspn(word, " ")] = '\0', \
	     word[sizeof(word) - 1] = '\0', \
	     next = strchr(next, ' '))

#define TMP_LEN 256
#define LONG_BUF_LEN    8192
#define SHORT_BUF_LEN   32

enum
{ SET, GET };

int is_digit_str(char *value);
int is_ascii_str(char *str);
int is_legal_hwaddr(char *value);
int is_legal_netmask(char *value);
int is_legal_ipaddr(char *value);
int is_legal_ip_netmask(char *sip, char *smask, char *dip);

int show_uptime(char *ptr, int length);
int cal_ifuptime(char *ptr, int length, unsigned long  if_start_time);
unsigned long read_system_uptime(void);
int read_file(const char *filename, char *buffer, size_t size);
int show_ipaddr(char *ifname, char *ptr, int length);
int show_ipaddr_single(char *ipaddr, int which);
int show_ipaddr_prefix24(char *ipaddr, char *buf, int buf_len);
int show_ipaddr_prefix24_calc(char *ipaddr, int start, int num,
        char *buf, int buf_len);
int show_hwaddr(char *ifname, char *ptr, int length);
int show_hwaddr_single(char *hwaddr, int which);
int show_localtime(char *buf, int buf_len);
char *show_iface_dev(char *iface);

int nbsp_translate(char *old_name, char *new_name, size_t size, int type);
char *num_to_protocol(int num);
int protocol_to_num(char *proto);
int trans_netmask2int(char *value);
int trans_int2netmask(int maskbits, char *mask_buf, int buf_len);
char * get_hwaddr_by_ipaddr(char * ip);
void inital_prod();
void do_apply_cgi(INPUT *input);
void do_survey_cgi(INPUT *input);
void do_default_cgi(INPUT *input);
void do_reboot_cgi(INPUT *input);
void do_percentage_cgi(INPUT *input);
void do_charge_cgi(INPUT *input);
void do_storage_cgi(INPUT *input);
void do_storage_space_cgi(INPUT *input);
void do_storage_info_cgi(INPUT *input);
void do_eject_storage_cgi(INPUT *input);
void do_date_cgi(INPUT *input);
void do_upgrade_cgi(INPUT *input);
int get_curr_auth_class(void);
void set_login_user_name(char *uname);
char *get_login_user_name(void);

#endif /* _HTTP_COMMON_H */
