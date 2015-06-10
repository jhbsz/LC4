#ifndef _AXIMDDNS_H
#define _AXIMDDNS_H

#define AXIMDDNS_SUCCESS                0
#define CGIERROR_BASE                   -100
#define CGIERROR_UNKNOWN_FUNC           (CGIERROR_BASE-1)       //subfunc not match
#define CGIERROR_PARAMETER_NOT_FOUND    (CGIERROR_BASE-2)       //some parameters not found
#define CGIERROR_USED                   (CGIERROR_BASE-3)       //domain name is used
#define CGIERROR_NO_DATA                (CGIERROR_BASE-4)       //internal use
#define CGIERROR_DB_NOT_FOUND           (CGIERROR_BASE-5)       //Database error
#define CGIERROR_KEY_NOT_MATCH          (CGIERROR_BASE-6)       //key is not match
#define CGIERROR_UNKNOWN_ERROR          (CGIERROR_BASE-99)

#define QNAPDDNS_ERR_NAME_USED            (CGIERROR_BASE-101)
#define QNAPDDNS_ERR_NAME_FAIL            (CGIERROR_BASE-102)
#define QNAPDDNS_ERR_NETWORK_OUT        (CGIERROR_BASE-103)
#define QNAPDDNS_ERR_SERVER_BUZY        (CGIERROR_BASE-104)
#define QNAPDDNS_ERR_SN_MAC_FAIL        (CGIERROR_BASE-105)
#define QNAPDDNS_ERR_NOUSER                (CGIERROR_BASE-106)
#define QNAPDDNS_ERR_OPEN_FILE            (CGIERROR_BASE-107)
#define QNAPDDNS_ERR_INPUT                (CGIERROR_BASE-108)
#define QNAPDDNS_ERR_OUT_OF_MEMORY        (CGIERROR_BASE-109)
#define QNAPDDNS_ERR_NO_USERNAME        (CGIERROR_BASE-110)
#define QNAPDDNS_ERR_USER_IS_EXIST        (CGIERROR_BASE-111)
#define QNAPDDNS_ERR_NO_EMAIL            (CGIERROR_BASE-112)
#define QNAPDDNS_ERR_NO_SMTP            (CGIERROR_BASE-113)
#define QNAPDDNS_ERR_NO_MAILADDR        (CGIERROR_BASE-114)

#define AXIMDDNS_ERR_BASE       -300
#define AXIMDDNS_ERR_NO_LAN_MAC        (AXIMDDNS_ERR_BASE-1)       
#define AXIMDDNS_ERR_NO_WAN_MAC        (AXIMDDNS_ERR_BASE-2)       
#define AXIMDDNS_ERR_ARGV        (AXIMDDNS_ERR_BASE-3)       
#define AXIMDDNS_ERR_ARGC        (AXIMDDNS_ERR_BASE-4)       
#define AXIMDDNS_ERR_MODELNAME        (AXIMDDNS_ERR_BASE-5)       


int do_check_mycloudwifi_name(int argc, char **argv);
int do_set_mycloudwifi_ddns(int argc, char **argv);
int do_disable_mycloudwifi_ddns(int argc, char **argv);
int do_update_mycloudwifi_ddns(int argc, char **argv);
int set_mycloudwifi_ddns(char *dname, char *qname);
int check_mycloudwifi_name_ex(char *dname, char *qname);
int create_update_name_wanip_postfile(char *postfile, char *dname, char *qname,char *wan_ip);
int create_update_wanip_postfile(char *postfile, char *wan_ip);
int update_mycloudwifi_ddns(char *postfile);
int disable_mycloudwifi_ddns(void);

#endif /* _AXIMDDNS_H */
