#ifndef _EZP_H
#define _EZP_H

#include <httpd.h>
#include <typedefs.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <cy_conf.h>
#include <utils.h>
#include <syslog.h>
#include <netinet/in.h>


/* #define DEBUG */

#define POST_BUF_SIZE	        40960
#define WEBS_BUF_SIZE	        4096
#define STATIC_ROUTE_PAGE       20
#define TMP_PASSWD              "slide"
#define b64_TMP_PASSWD          "c2xpZGU="

#define websBufferInit(wp) {webs_buf = malloc(WEBS_BUF_SIZE); webs_buf_offset = 0;}
#define websBufferWrite(wp, fmt, args...) {webs_buf_offset += sprintf(webs_buf+webs_buf_offset, fmt, ## args);}
#define websBufferFlush(wp) {webs_buf[webs_buf_offset] = '\0'; fprintf(wp, webs_buf); fflush(wp); free(webs_buf); webs_buf = NULL;}

#define ARGV(args...) ((char *[]) { args, NULL })
#define XSTR(s) STR(s)
#define STR(s) #s
#define STRUCT_LEN(name)    sizeof(name)/sizeof(name[0])


/* Gozila */
extern int gozila_action;
#define GOZILA_SAFE_GET(name)	gozila_action ? websGetVar(wp, name, "") : nvram_safe_get(name);
#define GOZILA_GET(name)    gozila_action ? websGetVar(wp, name, NULL) : NULL

extern int reboot_action;
extern int error_value;
extern int config_validate_msg;
extern int generate_key;

/* Action flags. Max number is 32 now. */
#define ACTION_NOTHING                  (0x00LL << 0)

#define ACTION_REBOOT                   (1)
#define ACTION_RESTART                  (254)
#define ACTION_FORCE_NOT_REBOOT         (255)

#define SERV_NETWORK_LAN                (2)
#define ACTION_NETWORK_LAN_STOP         (2)
#define ACTION_NETWORK_LAN_START        (3)

#define SERV_NETWORK_WAN                (4)
#define ACTION_NETWORK_WAN_STOP         (4)
#define ACTION_NETWORK_WAN_START        (5)

#define SERV_IPTABLES_BW                (6)
#define ACTION_IPTABLES_BW_STOP         (6)
#define ACTION_IPTABLES_BW_START        (7)

#define SERV_DNSMASQ                    (8)
#define ACTION_DNSMASQ_STOP             (8)
#define ACTION_DNSMASQ_START            (9)

#define SERV_IPTABLES_TOS               (10)
#define ACTION_IPTABLES_TOS_STOP        (10)
#define ACTION_IPTABLES_TOS_START       (11)

#define SERV_UPNPD                  (12)
#define ACTION_UPNPD_STOP           (12)
#define ACTION_UPNPD_START          (13)

#define SERV_CRON                       (14)
#define ACTION_CRON_START               (14)
#define ACTION_CRON_STOP                (15)

#define SERV_UPDATEDD                   (16)
#define ACTION_UPDATEDD_STOP            (16)
#define ACTION_UPDATEDD_START           (17)

#define SERV_NTPCLIENT                  (18)
#define ACTION_NTPCLIENT_STOP           (18)
#define ACTION_NTPCLIENT_START          (19)

#define SERV_IPTABLES_RT                (20)
#define ACTION_IPTABLES_RT_STOP         (20)
#define ACTION_IPTABLES_RT_START        (21)

#define SERV_IPTABLES_FL_HWADDR         (22)
#define ACTION_IPTABLES_FL_HWADDR_STOP  (22)
#define ACTION_IPTABLES_FL_HWADDR_START (23)

#define SERV_IPTABLES_NAT_PASS          (24)
#define ACTION_IPTABLES_NAT_PASS_STOP   (24)
#define ACTION_IPTABLES_NAT_PASS_START  (25)

#define SERV_IPTABLES_FR                (26)
#define ACTION_IPTABLES_FR_STOP         (26)
#define ACTION_IPTABLES_FR_START        (27)

#define SERV_IPTABLES_FW                (28)
#define ACTION_IPTABLES_FW_STOP         (28)
#define ACTION_IPTABLES_FW_START        (29)

#define SERV_IPTABLES_FL                (30)
#define ACTION_IPTABLES_FL_STOP         (30)
#define ACTION_IPTABLES_FL_START        (31)

#define SERV_VNCREPEATER                (32)
#define ACTION_VNCREPEATER_STOP         (32)
#define ACTION_VNCREPEATER_START        (33)

#define SERV_PPTPD                      (34)
#define ACTION_PPTPD_STOP               (34)
#define ACTION_PPTPD_START              (35)

#define SERV_L2TPD                      (36)
#define ACTION_L2TPD_STOP               (36)
#define ACTION_L2TPD_START              (37)

#define SERV_IPSEC                      (38)
#define ACTION_IPSEC_STOP               (38)
#define ACTION_IPSEC_START              (39)

#define SERV_TURBONAT                   (40)
#define ACTION_TURBONAT_STOP            (40)
#define ACTION_TURBONAT_START           (41)

#define SERV_BACKDOOR                   (42)
#define ACTION_BACKDOOR_STOP            (42)
#define ACTION_BACKDOOR_START           (43)

#define SERV_FTP                        (44)
#define ACTION_FTP_STOP                 (44)
#define ACTION_FTP_START                (45)

#define SERV_BT                         (46)
#define ACTION_BT_STOP                  (46)
#define ACTION_BT_START                 (47)

#define SERV_ISCSI                      (48)
#define ACTION_ISCSI_STOP               (48)
#define ACTION_ISCSI_START              (49)

#define SERV_OPENDNS                    (50)
#define ACTION_OPENDNS_STOP             (50)
#define ACTION_OPENDNS_START            (51)

#define SERV_SMB                        (52)
#define ACTION_SMB_STOP                 (52)
#define ACTION_SMB_START                (53)

#define SERV_IPTABLES_WF                (54)
#define ACTION_IPTABLES_WF_STOP         (54)
#define ACTION_IPTABLES_WF_START        (55)

#define SERV_ITUNES                     (56)
#define ACTION_ITUNES_STOP              (56)
#define ACTION_ITUNES_START             (57)

#define SERV_SESSION_MANAGER            (58)
#define ACTION_SESSION_MANAGER_STOP     (58)
#define ACTION_SESSION_MANAGER_START    (59)

#define SERV_SMBC                       (60)
#define ACTION_SMBC_STOP                (60)
#define ACTION_SMBC_START               (61)

#define SERV_IPTABLES_VS                (62)
#define ACTION_IPTABLES_VS_STOP         (62)
#define ACTION_IPTABLES_VS_START        (63)

#define SERV_WPS                        (64)
#define ACTION_WPS_STOP                 (64)
#define ACTION_WPS_START                (65)

#define SERV_WIRELESS                   (66)
#define ACTION_WIRELESS_STOP            (66)
#define ACTION_WIRELESS_START           (67)

#define SERV_DRIVE                      (68)
#define ACTION_DRIVE_STOP               (68)
#define ACTION_DRIVE_START              (69)

#define SERV_SNMP                       (70)
#define ACTION_SNMP_STOP                (70)
#define ACTION_SNMP_START               (71)

#define SERV_SYSLOGD                    (72)
#define ACTION_SYSLOGD_STOP             (72)
#define ACTION_SYSLOGD_START            (73)

#define SERV_MAC_CLONE                  (74)
#define ACTION_MAC_CLONE_STOP           (74)
#define ACTION_MAC_CLONE_START          (75)

#define SERV_WISP                       (76)
#define ACTION_WISP_STOP                (76)
#define ACTION_WISP_START               (77)

#define SERV_CAMERA                     (78)
#define ACTION_CAMERA_STOP              (78)
#define ACTION_CAMERA_START             (79)

#define SERV_BONJOUR                    (80)
#define ACTION_BONJOUR_STOP             (80)
#define ACTION_BONJOUR_START            (81)

#define SERV_MUSIC                      (82)
#define ACTION_MUSIC_STOP               (82)
#define ACTION_MUSIC_START              (83)

#ifdef AXIMDDNS
#define SERV_UPDATEAXIMDDNS                   (84)
#define ACTION_UPDATEAXIMDDNS_STOP            (84)
#define ACTION_UPDATEAXIMDDNS_START           (85)
#endif

#define SERV_NETWORK_VPN                (86)
#define ACTION_NETWORK_VPN_STOP         (86)
#define ACTION_NETWORK_VPN_START        (87)

#define SERV_NETWORK_MYQNAPCLOUD    (88)
#define ACTION_MYQNAPCLOUD_STOP     (88)
#define ACTION_MYQNAPCLOUD_START    (89)
/*
#define ACTION_NOTHING                  (0x00LL << 0)

#define ACTION_REBOOT                   (0x01LL << 0)

#define SERV_NETWORK_LAN                (0x01LL << 2)
#define ACTION_NETWORK_LAN_STOP         (0x01LL << 2)
#define ACTION_NETWORK_LAN_START        (0x01LL << 3)

#define SERV_NETWORK_WAN                (0x01LL << 4)
#define ACTION_NETWORK_WAN_STOP         (0x01LL << 4)
#define ACTION_NETWORK_WAN_START        (0x01LL << 5)

#define SERV_IPTABLES_BW                (0x01LL << 6)
#define ACTION_IPTABLES_BW_STOP         (0x01LL << 6)
#define ACTION_IPTABLES_BW_START        (0x01LL << 7)

#define SERV_DNSMASQ                    (0x01LL << 8)
#define ACTION_DNSMASQ_STOP             (0x01LL << 8)
#define ACTION_DNSMASQ_START            (0x01LL << 9)

#define SERV_IPTABLES_TOS               (0x01LL << 10)
#define ACTION_IPTABLES_TOS_STOP        (0x01LL << 10)
#define ACTION_IPTABLES_TOS_START       (0x01LL << 11)

#define SERV_MINIUPNPD                  (0x01LL << 12)
#define ACTION_MINIUPNPD_STOP           (0x01LL << 12)
#define ACTION_MINIUPNPD_START          (0x01LL << 13)

#define SERV_CRON                       (0x01LL << 14)
#define ACTION_CRON_START               (0x01LL << 14)
#define ACTION_CRON_STOP                (0x01LL << 15)

#define SERV_UPDATEDD                   (0x01LL << 16)
#define ACTION_UPDATEDD_STOP            (0x01LL << 16)
#define ACTION_UPDATEDD_START           (0x01LL << 17)

#define SERV_NTPCLIENT                  (0x01LL << 18)
#define ACTION_NTPCLIENT_STOP           (0x01LL << 18)
#define ACTION_NTPCLIENT_START          (0x01LL << 19)

#define SERV_IPTABLES_RT                (0x01LL << 20)
#define ACTION_IPTABLES_RT_STOP         (0x01LL << 20)
#define ACTION_IPTABLES_RT_START        (0x01LL << 21)

#define SERV_IPTABLES_FL_HWADDR         (0x01LL << 22)
#define ACTION_IPTABLES_FL_HWADDR_STOP  (0x01LL << 22)
#define ACTION_IPTABLES_FL_HWADDR_START (0x01LL << 23)

#define SERV_IPTABLES_NAT_PASS          (0x01LL << 24)
#define ACTION_IPTABLES_NAT_PASS_STOP   (0x01LL << 24)
#define ACTION_IPTABLES_NAT_PASS_START  (0x01LL << 25)

#define SERV_IPTABLES_FR                (0x01LL << 26)
#define ACTION_IPTABLES_FR_STOP         (0x01LL << 26)
#define ACTION_IPTABLES_FR_START        (0x01LL << 27)

#define SERV_IPTABLES_FW                (0x01LL << 28)
#define ACTION_IPTABLES_FW_STOP         (0x01LL << 28)
#define ACTION_IPTABLES_FW_START        (0x01LL << 29)

#define SERV_IPTABLES_FL                (0x01LL << 30)
#define ACTION_IPTABLES_FL_STOP         (0x01LL << 30)
#define ACTION_IPTABLES_FL_START        (0x01LL << 31)

#define SERV_VNCREPEATER                (0x01LL << 32)
#define ACTION_VNCREPEATER_STOP         (0x01LL << 32)
#define ACTION_VNCREPEATER_START        (0x01LL << 33)

#define SERV_PPTPD                      (0x01LL << 34)
#define ACTION_PPTPD_STOP               (0x01LL << 34)
#define ACTION_PPTPD_START              (0x01LL << 35)

#define SERV_L2TPD                      (0x01LL << 36)
#define ACTION_L2TPD_STOP               (0x01LL << 36)
#define ACTION_L2TPD_START              (0x01LL << 37)

#define SERV_IPSEC                      (0x01LL << 38)
#define ACTION_IPSEC_STOP               (0x01LL << 38)
#define ACTION_IPSEC_START              (0x01LL << 39)

#define SERV_TURBONAT                   (0x01LL << 40)
#define ACTION_TURBONAT_STOP            (0x01LL << 40)
#define ACTION_TURBONAT_START           (0x01LL << 41)

#define SERV_BACKDOOR                   (0x01LL << 42)
#define ACTION_BACKDOOR_STOP            (0x01LL << 42)
#define ACTION_BACKDOOR_START           (0x01LL << 43)

#define SERV_FTP                        (0x01LL << 44)
#define ACTION_FTP_STOP                 (0x01LL << 44)
#define ACTION_FTP_START                (0x01LL << 45)

#define SERV_BT                         (0x01LL << 46)
#define ACTION_BT_STOP                  (0x01LL << 46)
#define ACTION_BT_START                 (0x01LL << 47)

#define SERV_ISCSI                      (0x01LL << 48)
#define ACTION_ISCSI_STOP               (0x01LL << 48)
#define ACTION_ISCSI_START              (0x01LL << 49)

#define SERV_OPENDNS                    (0x01LL << 50)
#define ACTION_OPENDNS_STOP             (0x01LL << 50)
#define ACTION_OPENDNS_START            (0x01LL << 51)

#define SERV_SMB                        (0x01LL << 52)
#define ACTION_SMB_STOP                 (0x01LL << 52)
#define ACTION_SMB_START                (0x01LL << 53)

#define SERV_IPTABLES_WF                (0x01LL << 54)
#define ACTION_IPTABLES_WF_STOP         (0x01LL << 54)
#define ACTION_IPTABLES_WF_START        (0x01LL << 55)

#define SERV_ITUNES                     (0x01LL << 56)
#define ACTION_ITUNES_STOP              (0x01LL << 56)
#define ACTION_ITUNES_START             (0x01LL << 57)

#define SERV_SESSION_MANAGER            (0x01LL << 58)
#define ACTION_SESSION_MANAGER_STOP     (0x01LL << 58)
#define ACTION_SESSION_MANAGER_START    (0x01LL << 59)

#define SERV_SMBC                       (0x01LL << 60)
#define ACTION_SMBC_STOP                (0x01LL << 60)
#define ACTION_SMBC_START               (0x01LL << 61)

#define SERV_IPTABLES_VS                (0x01LL << 62)
#define ACTION_IPTABLES_VS_STOP         (0x01LL << 62)
#define ACTION_IPTABLES_VS_START        (0x01LL << 63)
*/
/*XXX: An extra space in the front is required for the case of "TYPE=wan". */ 
#define SERV_RC_COMMON                  " /etc/rc.common /etc/init.d/"
#define STORAGE_MODEL "C|G|T|D|K"
#define WWAN_MODEL "M|N|I|U|P|W|A"
#define WIRELESS_MODEL "M|N|P|W|U|C|D|V|K|I|G"
#define WISP_MODEL "K|O"
typedef struct {
    struct in_addr ipaddr;
    char mac[20];
}peer;

extern peer client;

struct service
{
    int64_t flag;
    char *script;
    char *script2;
};

struct variable
{
    char *name;
    char *longname;
    int (*valid) (webs_t wp, char *value, struct variable *v);
    int (*save) (webs_t wp, char *value, struct variable *v, struct service *s);
    char **argv;
    int nullok;
    int64_t action_flag;
};

/* XXX: EZP clean up */
extern int ej_tbl_get(int eid, webs_t wp, int argc, char **argv);
extern int ej_tbl_create_table(int eid, webs_t wp, int argc, char **argv);

/* for WAN */
extern int valid_cnnt(webs_t wp, char *value, struct variable *v);
extern int valid_wan(webs_t wp, char *value, struct variable *v);
extern int valid_wan_priority(webs_t wp, char *value, struct variable *v);
extern int save_cnnt(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int save_wan(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int save_wan_priority(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int ej_wan_show_proto_type(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wan_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wwan_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_pppoe_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_static_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wan_show_type_setting(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wan_show_vpnc_type_setting(int eid, webs_t wp, int argc,
        char_t **argv);
#ifdef EZP_SUB_BRAND_APOLLO
extern int ej_wan_show_wisp_type_setting(int eid, webs_t wp, int argc,
        char_t **argv);
#endif
#if defined (EZP_SUB_BRAND_SONY) || defined(EZP_SUB_BRAND_GARMIN) || defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
extern int valid_wisp_swap(webs_t wp, char *value, struct variable *v);
extern int save_wisp_swap(webs_t wp, char *value, struct variable *v,
                struct service *s);
extern int ej_wan_show_wisp_swap_list(int eid, webs_t wp, int argc,
                char_t **argv);
#endif
extern int ej_wan_wwan_show_modem(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wan_wwan_create_modem_array(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wan_wwan_show_isp(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wan_barry_show_isp(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wan_wwan_create_isp_array(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wan_create_proto_array(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wl_show_radius_html_start(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wl_show_radius_html_end(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wan_show_wan_type_html_start(int eid, webs_t wp, int argc,
                char_t **argv);
extern int ej_wan_show_wan_type_html_end(int eid, webs_t wp, int argc,
                char_t **argv);
#ifdef EZP_PROD_BRAND_ZYUS
extern int ej_wwan_show_wan_type_html_start(int eid, webs_t wp, int argc,
                char_t **argv);
extern int ej_wwan_show_wan_type_html_end(int eid, webs_t wp, int argc,
                char_t **argv);
#endif

/* for Wizard */
extern int valid_wizard(webs_t wp, char *value, struct variable *v);
extern int save_wizard(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int valid_vpn_wizard(webs_t wp, char *value, struct variable *v);
extern int save_vpn_wizard(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int ej_wizard_load_isp_data(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wizard_switch_isp(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wizard_load_bandwidth_data(int eid, webs_t wp, int argc,
        char_t **argv);
extern int ej_wizard_vpn_show_max(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_wizard_vpn_show_num(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_wizard_get_lan_mask(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_wizard_title(int eid, webs_t wp, int argc, char_t ** argv);



/* for LAN */
extern int valid_exp_lan(webs_t wp, char *value, struct variable *v);
extern int save_exp_lan(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int valid_lan(webs_t wp, char *value, struct variable *v);
extern int save_lan(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int ej_lan_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_lan_show_subnet(int eid, webs_t wp, int argc, char_t **argv);

/*for myQNAPcloud*/
extern int valid_exp_myqnapcloud(webs_t wp, char *value, struct variable *v);
extern int save_exp_myqnapcloud(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int qid_update_device_info();
extern int qid_get_device_info(char *qid_account, char *qid_status, char *device_id, char *device_name);
extern int qid_signin(char *qid_id, char *qid_pw);
extern int qid_available_dev_name(char* device_name);
extern int qid_bind_dev_name(char* device_name);
extern int qid_publish_service();
extern int qid_update_service_port(uint16_t management_external_port, uint16_t file_external_port);
extern int qid_update_service_port_by_upnpc();
extern int qid_fetch_web_page(const char* protocol, uint16_t external_port, const char* path);
extern int qid_unbind_dev_name();
extern int qid_btn_test_fetch_action();
/* for DHCP */
extern int valid_dhcp(webs_t wp, char *value, struct variable *v);
extern int save_dhcp(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int ej_dhcp_show_setting(int eid, webs_t wp, int argc, char_t **argv);

/* for OpenDNS */
extern int valid_opendns(webs_t wp, char *value, struct variable *v);
extern int save_opendns(webs_t wp, char *value, struct variable *v,
                struct service *s);
extern int ej_opendns_show_setting(int eid, webs_t wp, int argc, char_t **argv);

/* for DDNS */
extern int valid_ddns(webs_t wp, char *value, struct variable *v);
extern int save_ddns(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int ej_ddns_show_setting(int eid, webs_t wp, int argc, char_t **argv);

/* for hwaddr clone */
extern int valid_clone(webs_t wp, char *value, struct variable *v);
extern int save_clone(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern void do_get_mac_cgi(char *url, webs_t stream, ...);
extern int ej_clone_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_clone_show_hwaddr(int eid, webs_t wp, int argc, char_t **argv);

/* for WAN detect */
extern int ej_wan_detect_show_setting(int eid, webs_t wp, int argc, char_t **argv);

/* for DBM */
extern int valid_bw(webs_t wp, char *value, struct variable *v);
extern int save_bw(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int ej_bw_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_bw_show_bw_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_bw_show_rate_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_bw_show_adv_bw_type(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_bw_show_resv(int eid, webs_t wp, int argc, char_t **argv);

/* for TurboNAT */
extern int valid_turbonat(webs_t wp, char *value, struct variable *v);
extern int save_turbonat(webs_t wp, char *value, struct variable *v, struct service *s);

extern int valid_snmp(webs_t wp, char *value, struct variable *v);
extern int save_snmp(webs_t wp, char *value, struct variable *v, struct service *s);

/* for Session Manager */
extern int valid_session_manager(webs_t wp, char *value, struct variable *v);
extern int save_session_manager(webs_t wp, char *value, struct variable *v, struct service *s);

/* for TOS */
extern int valid_tos(webs_t wp, char *value, struct variable *v);
extern int save_tos(webs_t wp, char *value, struct variable *v, struct service *s);

/* for load balance */
extern int valid_lb(webs_t wp, char *value, struct variable *v);
extern int save_lb(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int ej_lb_legend(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_lb_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_lb_show_available(int eid, webs_t wp, int argc, char_t **argv);

/* for routing */
extern int valid_rt(webs_t wp, char *value, struct variable *v);
extern int save_rt(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for Cron */
extern int valid_cron(webs_t wp, char *value, struct variable *v);
extern int save_cron(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for NTP */
extern int valid_ntp(webs_t wp, char *value, struct variable *v);
extern int save_ntp(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for FTP */
extern int valid_ftpd(webs_t wp, char *value, struct variable *v);
extern int save_ftpd(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for VLAN */
extern int valid_vlan(webs_t wp, char *value, struct variable *v);
extern int save_vlan(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern int ej_vlan_show_content(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_vlan_js(int eid, webs_t wp, int argc, char_t **argv);

/* for SAMBA */
extern int valid_smbd(webs_t wp, char *value, struct variable *v);
extern int save_smbd(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for Drive */
extern int valid_drive(webs_t wp, char *value, struct variable *v);
extern int save_drive(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* Power Saving */
extern int valid_power(webs_t wp, char *value, struct variable *v);
extern int save_power(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for togoPower App */
extern int valid_togopower(webs_t wp, char *value, struct variable *v);
extern int save_togopower(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for togoEye App */
extern int valid_togoeye(webs_t wp, char *value, struct variable *v);
extern int save_togoeye(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for togoDrive App */
extern int valid_togodrive(webs_t wp, char *value, struct variable *v);
extern int save_togodrive(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for Camera */
extern int valid_camera(webs_t wp, char *value, struct variable *v);
extern int save_camera(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for Music */
extern int valid_music(webs_t wp, char *value, struct variable *v);
extern int save_music(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for WPS */
extern int valid_wl_wps(webs_t wp, char *value, struct variable *v);
extern int save_wl_wps(webs_t wp, char *value, struct variable *v,
                struct service *s);

/* for ITUNES */
extern int valid_itunes(webs_t wp, char *value, struct variable *v);
extern int save_itunes(webs_t wp, char *value, struct variable *v,
                struct service *s);
extern int ej_itunes_show_setting(int eid, webs_t wp, int argc, char_t **argv);

/* for USB storage */
extern int valid_storage(webs_t wp, char *value, struct variable *v);
extern int save_storage(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for BT */
extern int valid_bt(webs_t wp, char *value, struct variable *v);
extern int save_bt(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for iSCSI */
extern int valid_iscsi(webs_t wp, char *value, struct variable *v);
extern int save_iscsi(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for PPTP */
extern int valid_pptpd(webs_t wp, char *value, struct variable *v);
extern int save_pptpd(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for L2TP */
extern int valid_l2tpd(webs_t wp, char *value, struct variable *v);
extern int save_l2tpd(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for IPsec */
extern int valid_ipsec(webs_t wp, char *value, struct variable *v);
extern int save_ipsec(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for port forwarding */
extern int ej_fr_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int valid_fr(webs_t wp, char *value, struct variable *v);
extern int save_fr(webs_t wp, char *value, struct variable *v, 
        struct service *s);
extern int valid_wf(webs_t wp, char *value, struct variable *v);
extern int save_wf(webs_t wp, char *value, struct variable *v, 
        struct service *s);
extern int valid_vs(webs_t wp, char *value, struct variable *v);
extern int save_vs(webs_t wp, char *value, struct variable *v, 
        struct service *s);

/* for nat pass */
extern int valid_nat_pass(webs_t wp, char *value, struct variable *v);
extern int save_nat_pass(webs_t wp, char *value, struct variable *v, struct service *s);

/* for fw */
extern int valid_fw(webs_t wp, char *value, struct variable *v);
extern int save_fw(webs_t wp, char *value, struct variable *v, struct service *s);

/* for upnp */
extern int valid_upnp(webs_t wp, char *value, struct variable *v);
extern int save_upnp(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for vnc */
extern int valid_vnc(webs_t wp, char *value, struct variable *v);
extern int save_vnc(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for schedules. */
extern int valid_sched(webs_t wp, char *value, struct variable *v);
extern int save_sched(webs_t wp, char *value, struct variable *v,
        struct service *s);
extern void schedule_cgi(webs_t wp);

/* for filters. */
extern int valid_fl(webs_t wp, char *value, struct variable *v);
extern int save_fl(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for hwaddr filters. */
extern int valid_fl_hwaddr(webs_t wp, char *value, struct variable *v);
extern int save_fl_hwaddr(webs_t wp, char *value, struct variable *v,
        struct service *s);

/* for Managmeent */
extern int valid_http(webs_t wp, char *value, struct variable *v);
extern int save_http(webs_t wp, char *value, struct variable *v, struct service  *s);
extern int valid_adtool(webs_t wp, char *value, struct variable *v);
extern int save_adtool(webs_t wp, char *value, struct variable *v, struct service  *s);
extern int select_port_config(char *selection);
extern int ej_ad_tools_showif(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_ad_x_mode_switch(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_ad_vlanport_config(int eid, webs_t wp, int argc, char_t **argv);

/* for Syslog */
extern int valid_log(webs_t wp, char *value, struct variable *v);
extern int save_log(webs_t wp, char *value, struct variable *v, struct service  *s);

/* TODO: Remove the unsued functions. */
/* for upgrade */
extern void do_export(char *path, webs_t stream, ...);
extern void do_upgrade_post(char *url, webs_t stream, int len,
                            char *boundary);
extern void do_upgrade_cgi(char *url, webs_t stream, ...);

/* for BT */
int ej_bt_show_content(int eid, webs_t wp, int argc, char_t **argv);
extern void do_bt_upload_post(char *url, webs_t stream, int len,
                            char *boundary);
extern void do_bt_upload_cgi(char *url, webs_t stream, ...);
extern void do_bt_post(char *url, webs_t stream, int len,
                            char *boundary);
extern void do_bt_cgi(char *url, webs_t stream, ...);


/* for administration tools */
void
do_ad_tools_cgi(char *url, webs_t stream, ...);
void
do_at_tools_cgi(char *url, webs_t stream, ...);

/* for WPS action */
void
do_wps_cgi(char *url, webs_t stream, ...);

/* for Log */
void
do_log_cgi(char *url, webs_t stream, ...);

/* for mac address binding */
extern int ej_fl_hwaddr_ip_show_rule(int eid, webs_t wp, int argc,
                                 char_t ** argv);;
extern int ej_fl_hwaddr_ip_get_rule_index(int eid, webs_t wp, int argc,
                                      char_t ** argv);
extern int ej_fl_hwaddr_ip_show_rule_table(int eid, webs_t wp, int argc,
                                       char_t ** argv);
extern int ej_fl_hwaddr_ip_get_rule_mac(int eid, webs_t wp, int argc,
                                    char_t ** argv);
extern int ej_fl_hwaddr_ip_get_rule_ip(int eid, webs_t wp, int argc,
                                   char_t ** argv);

extern int valid_fl_hwaddr_ip_rule(webs_t wp, char *value, struct variable *v);
extern int save_fl_hwaddr_ip_rule(webs_t wp, char *value, struct variable *v);

/* for static route */
extern int valid_static_route(webs_t wp, char *value, struct variable *v);
extern int delete_static_route(webs_t wp);

/* for wireless */
extern int ej_wl_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wlv_show_entry_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wlv_sec_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int valid_wl_wep_key(webs_t wp, char *value, struct variable *v);
extern int valid_wl_channel(webs_t wp, char *value, struct variable *v);
extern int valid_wl_wpa_psk(webs_t wp, char *value, struct variable *v);
extern int valid_wl_adv(webs_t wp, char *value, struct variable *v);
extern int valid_wl_basic(webs_t wp, char *value, struct variable *v);
extern int valid_wl_wds_2(webs_t wp, char *value, struct variable *v);
extern int valid_wl_apcli(webs_t wp, char *value, struct variable *v);

extern int save_wl_sec_mode(webs_t wp, char *value, struct variable *v, struct service *s);
extern int save_wl_wpa_psk(webs_t wp, char *value, struct variable *v, struct service *s);
extern int save_wl_adv(webs_t wp, char *value, struct variable *v, struct service *s);
extern int save_wl_basic(webs_t wp, char *value, struct variable *v, struct service *s);
extern int save_wl_wds_2(webs_t wp, char *value, struct variable *v, struct service *s);
extern int save_wl_apcli(webs_t wp, char *value, struct variable *v, struct service *s);
extern int ej_wl_wds_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wlv_wds_sec_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wl_apcli_show_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_apcli_fieldset_need(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_wlv_apcli_sec_show_setting(int eid, webs_t wp, int argc, char_t **argv);

extern int valid_wl_wds(webs_t wp, char *value, struct variable *v);
extern int save_wl_wds(webs_t wp, char *value, struct variable *v, struct service *s);
extern int save_wl_wds_enable(webs_t wp, char *value, struct variable *v, struct service *s);
extern int ej_wl_wds_hwaddr_show_rule(int eid, webs_t wp, int argc,
                                      char_t **argv);
extern int ej_wlv_single_ssid_show_mark_begin(int eid, webs_t wp, int argc,
                                              char_t **argv);
extern int ej_wlv_single_ssid_show_mark_end(int eid, webs_t wp, int argc,
                                            char_t **argv);
extern int ej_wlv_no_11n_show_mark_begin(int eid, webs_t wp, int argc,
                                              char_t **argv);
extern int ej_wlv_no_11n_show_mark_end(int eid, webs_t wp, int argc,
                                            char_t **argv);
char *
_get_web_variable(webs_t wp, int nth, char *item);
char *
_get_apcli_web_variable(webs_t wp, int if_idx, int vif_idx, char *item);
int
_save_wl_apcli_sec_wep(webs_t wp, int if_idx, int vif_idx, struct variable *v,
        struct service *s, int64_t *map);
int
_save_wl_apcli_sec_psk_or_psk2(webs_t wp, int if_idx, int vif_idx, int wpa2,
        struct variable *v, struct service *s, int64_t *map);

/* for ddns */
extern int ddns_save_value(webs_t wp);
extern int ddns_update_value(webs_t wp);

#ifdef EZPACKET
extern int ej_diag_ping(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_diag_traceroute(int eid, webs_t wp, int argc,
                                  char_t ** argv);
#endif
/* for status */
extern int ej_togo_show_wan_proto(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_show_wan_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_show_wan_setting_bytype(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_show_wan_priv_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_show_lan_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_show_lan_priv_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_show_wl_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_show_wl_priv_setting(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_show_license_status(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_show_ipaddr_prefix24_calc(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_version(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_mrtg_show(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_show_period(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_st_user_show_dhcp(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_user_show_dhcp_user_num(int eid, webs_t wp, int argc, 
        char_t **argv);
extern int ej_st_user_show_arp(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_user_show_arp_user_num(int eid, webs_t wp, int argc, 
        char_t **argv);
extern int ej_st_log_show_iptables(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_log_show_system(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_log_show_class(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_show_if(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_eeprom_hwaddr(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_wl_region(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_build_time(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_get_ezp_machine_id(int eid, webs_t wp, int argc, char_t ** argv);

/* Valid family functions */
extern int valid_domain(webs_t wp, char *value, struct variable *v);
extern int valid_ipaddr(webs_t wp, char *value, struct variable *v);
extern int valid_subnet(webs_t wp, char *value, struct variable *v);
extern int valid_hwaddr(webs_t wp, char *value, struct variable *v);
extern int valid_netmask(webs_t wp, char *value, struct variable *v);
extern int valid_strict_name(webs_t wp, char *value, struct variable *v);
extern int valid_name(webs_t wp, char *value, struct variable *v);
extern int valid_nv_name(webs_t wp, char *value, struct variable *v);
extern int valid_choice(webs_t wp, char *value, struct variable *v);
extern int valid_range(webs_t wp, char *value, struct variable *v);
extern int valid_if(webs_t wp, char *value, struct variable *v);
extern int valid_number(webs_t wp, char *value, struct variable *v);
extern int valid_word(webs_t wp, char *value, struct variable *v);
extern int valid_passwd(webs_t wp, char *value, struct variable *v);
extern int valid_nv_passwd(webs_t wp, char *value, struct variable *v);
extern int valid_in_a_range(webs_t, int value, int min, int max,
        struct variable *v);
extern int valid_ipaddr_range(webs_t wp, char *v1, char *v2,
        struct variable *v);
extern int valid_length_range(webs_t wp, char *value, struct variable *v);
extern int valid_port_range(webs_t wp, char *v1, char *v2,
        struct variable *v);
extern int valid_wep_key(webs_t wp, char *value, struct variable *v);
extern int valid_word(webs_t wp, char *value, struct variable *v);

/* EZP: Borrow from DD-WRT */
extern int ej_show_control(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_localtime(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_uptime(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_ipaddr(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_hwaddr(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_show_battery_status(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_show_charge_status(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_st_show_storage_status(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_show_server_ip(int eid, webs_t wp, int argc, char_t ** argv);

#ifdef AXIMDDNS
/* for AXIMDDNS */
extern int valid_aximddns(webs_t wp, char *value, struct variable *v);
extern int save_aximddns(webs_t wp, char *value, struct variable *v,
        struct service *s);
#endif
/* GUI */
extern int ej_do_menu(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_pagehead1(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_pagehead2(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_pagehead3(int eid, webs_t wp, int argc, char_t ** argv);
#ifdef EZP_PROD_BRAND_PROX
extern int ej_nv_get_model_for_prox(webs_t wp);
#endif
extern int ej_do_bootstrap_navbar_menu(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_bootstrap_menu(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_headmeta(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_basic_css(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_basic_js(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_custom_css(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_ace_font_css(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_ace_font_ie7_css(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_ace_basic_css(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_ace_basic_ie9_css(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_basic_js(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_custom_js(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_datatables_css(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_datatables_js(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_jquery_ui_js(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_jquery_other_js(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_jquery_flot_js(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_ace_basic_js(int eid, webs_t wp, int argc, char_t ** argv);

/* Print */
extern int ej_do_print_begin(int eid, webs_t wp, int argc, char_t ** argv);
extern int ej_do_print_end(int eid, webs_t wp, int argc, char_t ** argv);

/* Widget */
extern int ej_widget_execute(int eid, webs_t wp, int argc, char_t **argv);
extern int ej_widget_show(int eid, webs_t wp, int argc, 
        char_t **argv);
extern int ej_widget_show_login(int eid, webs_t wp, int argc, 
        char_t **argv);
extern int ej_widget_show_signal(int eid, webs_t wp, int argc, 
        char_t **argv);
extern int ej_widget_start(int eid, webs_t wp, int argc, char_t **argv);

/* Common Used */
void config_preaction(int64_t *map, struct variable *v, struct service *s,
        char *prefix, char *postfix);
void config_postaction(int64_t map, struct service *s, char *prefix, char *postfix);
extern char *nvg_attr_match(char *tag, char *rule_set, int rule_num, 
       char *attr, char *match, char *output);
/* Interface transfer rule form into url put form. */
void trans2web(char *buf, int len, char *buf2); 

/* API for application manager checking */
int appmgr_check(char *appname);

extern int valid_base64(webs_t wp, char *value, struct variable *v);
/* Prod cat */
extern unsigned long PROD_CAT_BITMAP;
extern int ezp_prod_subcat;
extern char ezp_prod_cat[]; 
extern char ezp_prod_subsubcat[];
extern char *prod_subsubcat;

#endif /* _EZP_H */
