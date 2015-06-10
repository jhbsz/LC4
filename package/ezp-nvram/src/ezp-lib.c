#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <bcmnvram.h>
#include <assert.h>
#include "ezp-lib.h"
#include "shared.h"


struct attr {
    char *name;
    int (*func_orig)(char *, char *, char *, int);
    int (*func_wrap)(char *, char *, char *, int);
};

static int
show_enable_value(char *type, char *val, char *buf, int bsize)
{
    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    if (snprintf(buf, bsize, "%s", 
                !strcmp(val, "1") ? "enabled" : "disabled") >= bsize) {
        return EZPLIB_VAL_TRUNC;
    }

    return strlen(buf);
}

static int
show_value(char *type, char *val, char *buf, int bsize)
{
    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    if (snprintf(buf, bsize, "%s", val) >= bsize) {
        return EZPLIB_VAL_TRUNC;
    }
    return strlen(buf);
}

static char *iftype[] = { "wan", "lan", NULL };

static int
show_if_value(char *type, char *val, char *buf, int bsize)
{
    char *ptr;
    int i, len, idx;

    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    for (i = 0; iftype[i]; i++) {
        len = strlen(iftype[i]);
        if (!strcmp(val, iftype[i])) {
            break;
        }
    }

    if (!iftype[i]) {
        if (snprintf(buf, bsize, "%s", val) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else {
        ptr = val + len;
        /* 1-based index for GUI display. */
        idx = atoi(ptr) + 1;    
        if (snprintf(buf, bsize, "%s%d", iftype[i], idx) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    }

    return strlen(buf);
}

static int
show_proto_value(char *type, char *val, char *buf, int bsize)
{
    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    if (!strcmp(val, "both")) {
        if (snprintf(buf, bsize, "UDP/TCP") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else if (!strcmp(val, "tcp")) {
        if (snprintf(buf, bsize, "TCP") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else if (!strcmp(val, "udp")) {
        if (snprintf(buf, bsize, "UDP") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else {
        if (snprintf(buf, bsize, "%s", val) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    }
    return strlen(buf);
}

/* show 24 to 255.255.255.0 */
static int
show_mask_value(char *type, char *val, char *buf, int bsize)
{
    int i;
    unsigned long mask;
    struct in_addr netmask;

    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    int maskbits = atoi(val);

    if (1 <= maskbits && maskbits <= 32) {
        mask = 0;
        for ( i=0 ; i<maskbits ; i++ )
            mask |= 1<<(31-i);
        netmask.s_addr = htonl(mask);
        if (snprintf(buf, bsize, "%s", inet_ntoa(netmask)) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else {
        if (snprintf(buf, bsize, "") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    }

    return strlen(buf);
}

/* show 255.255.255.0 to 24 */
static int
show_mask2int_value(char *type, char *val, char *buf, int bsize)
{
    struct in_addr netmask;
    unsigned long mask;
    int maskbits;

    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }
    if (inet_aton(val,&netmask))  {
        mask = ntohl(netmask.s_addr);
        for ( maskbits=32 ; (mask & (1L<<(32-maskbits))) == 0 ; maskbits-- );
        if (snprintf(buf, bsize, "%d", maskbits) >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    } else {
        if (snprintf(buf, bsize, "") >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
    }
    return strlen(buf);
}

static int
show_b64_value(char *type, char *val, char *buf, int bsize)
{
    unsigned long outlen = EZPLIB_BUF_LEN;
    char tmp[EZPLIB_BUF_LEN];
    if (!val) {
        return EZPLIB_NO_ATTRIBUTE;
    }

    base64_decode((unsigned char *)val, strlen(val),
            (unsigned char *)tmp, &outlen);
    tmp[outlen] = '\0';
    if (snprintf(buf, bsize, "%s", tmp) >= bsize) {
        return EZPLIB_VAL_TRUNC;
    }
    return strlen(buf);
}
#ifdef AXIMDDNS
/* AXIM DDNS rule */
/* user_enable^user_name^user_counter^default_counter */
struct attr aximddns_rule_attr[] = {
    { "user_enable", show_value, show_enable_value },
    { "user_name", show_value, show_enable_value },
    { "user_counter", show_value, show_enable_value },
    { "default_counter", show_value, show_enable_value },
    { "trycnnt_countdown", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* AXIM DDNS tmp rule*/
/* user_enable^user_name^user_counter */
struct attr aximddns_tmp_rule_attr[] = {
    { "user_enable", show_value, show_enable_value },
    { "user_name", show_value, show_enable_value },
    { "user_counter", show_value, show_enable_value },
    { "wan_ip", show_value, show_enable_value },
    { NULL, NULL, NULL }
};
#endif

#ifdef QFILE_CGI
/* using for CGI progress bar */
struct attr cgi_progress_attr[] = {
    { "pid", show_value, show_value },
    { "copy_file", show_value, show_value },
    { "copy_number", show_value, show_value },
    { "files_count", show_value, show_value },
    { "copy_size", show_value, show_value },
    { "percent", show_value, show_value },
    { "copying", show_value, show_value },
    { "copy_status", show_value, show_value },
    { NULL, NULL, NULL }
};
#endif

/* ip^port^time */
struct attr mac_sense_attr[] = {
    { "proto", show_value, show_value },
    { "server_ip", show_value, show_value },
    { "server_port", show_value, show_value },
    { "server_time", show_value, show_value },
    { "server_timeout", show_value, show_value },
    { "wds_enable", show_value, show_value },
    { "remote_ip", show_value, show_value },
    { NULL, NULL, NULL }
};

#if defined (PLATFORM_ALS)
struct attr file_mode_attr[] = {
    { "enable", show_value, show_value },
    { "sd", show_value, show_value },
    { "usb", show_value, show_value },
    { "record_1", show_value, show_value },
    { "record_2", show_value, show_value },
    { "record_3", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr relay_schedule_attr[] = {
    { "sun", show_value, show_value },
    { "mon", show_value, show_value },
    { "tue", show_value, show_value },
    { "wed", show_value, show_value },
    { "thu", show_value, show_value },
    { "fri", show_value, show_value },
    { "sat", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr mail_attr[] = {
    { "battery", show_value, show_value },
    { "sd", show_value, show_value },
    { "usb", show_value, show_value },
    { "relay", show_value, show_value },
    { "gpio", show_value, show_value },
    { "sms", show_value, show_value },
    { "mail", show_value, show_value },
    { "address_1", show_value, show_value },
    { "address_2", show_value, show_value },
    { "send_mail", show_value, show_value },
    { "email", show_value, show_value },
    { "phone", show_value, show_value },
    { "server", show_value, show_value },
    { "act", show_value, show_value },
    { "pwd", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr failover_attr[] = {
    { "enable", show_value, show_value },
    { "type", show_value, show_value },
    { "count", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr alert_attr[] = {
    { "battery", show_value, show_value },
    { "sd", show_value, show_value },
    { "usb", show_value, show_value },
    { "relay", show_value, show_value },
    { "gpio", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr relay_attr[] = {
    { "enable", show_value, show_value },
    { "option", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr als_battery_rule_attr[] = {
    {"battery", show_value, show_value},
    { NULL, NULL, NULL }
};
struct attr als_status_rule_attr[] = {
    {"enable", show_value, show_value},
    {"mode", show_value, show_value},
    {"tx", show_value, show_value},
    {"rx", show_value, show_value},
    {"isp1", show_value, show_value},
    {"isp2", show_value, show_value},
    {"isp3", show_value, show_value},
    {"isp4", show_value, show_value},
    { NULL, NULL, NULL }
};
struct attr als_serial_rule_attr[] = {
    {"alias", show_value, show_value},
    {"baudrate", show_value, show_value},
    {"databit", show_value, show_value},
    {"stopbit", show_value, show_value},
    {"parity", show_value, show_value},
    {"flow", show_value, show_value},
    {"fifo", show_value, show_value},
    {"device", show_value, show_value},
    { NULL, NULL, NULL }
};
struct attr als_operating_rule_attr[] = {
    {"mode", show_value, show_value},
    {"alive", show_value, show_value},
    {"inactive", show_value, show_value},
    {"max_connect", show_value, show_value},
    {"jammed_ip", show_value, show_value},
    { NULL, NULL, NULL }
};

struct attr als_operating_data_packing_rule_attr[] = {
    {"length", show_value, show_value},
    {"transmit", show_value, show_value},
    { NULL, NULL, NULL }
};

struct attr als_operating_option_rule_attr[] = {
    {"tcp_port", show_value, show_value},
    {"ip_1", show_value, show_value},
    {"ip_port_1", show_value, show_value},
    {"ip_2", show_value, show_value},
    {"ip_port_2", show_value, show_value},
    {"ip_3", show_value, show_value},
    {"ip_port_3", show_value, show_value},
    {"ip_4", show_value, show_value},
    {"ip_port_4", show_value, show_value},
    {"port_1", show_value, show_value},
    {"port_2", show_value, show_value},
    {"port_3", show_value, show_value},
    {"port_4", show_value, show_value},
    {"udp_port", show_value, show_value},
    { NULL, NULL, NULL }
};

#endif
/* enable[0|1]^enable[0|1|2]^enable[0|1] */
struct attr plug_rule_attr[] = {
    { "ethernet", show_value, show_value },
    { "wisp", show_value, show_value },
    { "usb", show_value, show_value },
    { NULL, NULL, NULL }
};
/* expend[0|1]^enable[0|1]^name^connection[0|1]^proto[0|1|2] */
struct attr cnnt_rule_attr[] = {
    { "expand", show_value, show_value },
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "connection", show_value, show_value },
    { "proto", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^name^extif^proto^extport_start^extport_end^ipaddr^inport_start^inport_end */
struct attr fr_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "extif", show_value, show_value },
    { "proto", show_value, show_proto_value },
    { "extport_start", show_value, show_value },
    { "extport_end", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "inport_start", show_value, show_value },
    { "inport_end", show_value, show_value },
    { NULL, NULL, NULL }
};

/* set_wizard^admin^mode^wan^bw^wifi */
struct attr wizard_rule_attr[] = {
    { "set_wizard", show_value, show_value },
    { "admin", show_value, show_value },
    { "mode", show_value, show_value },
    { "wan_trad", show_value, show_value },
    { "wan_wwan", show_value, show_value },
    { "bw", show_value, show_value },
    { "time", show_value, show_value },
    { "wifi", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^extif^index^wan_ipaddr^mapped_ipaddr */
struct attr vs_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "extif", show_value, show_value },
    { "index", show_value, show_value },
    { "wan_ipaddr", show_value, show_value },
    { "mapped_ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^mtu^stp^maxcount */
struct attr wan_main_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "mtu", show_value, show_value },
    { "stp", show_value, show_value },
    { "maxcount", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable[0|1]^threshold^timeout^type[arping|ping]^host_type[gateway|custom]^host */
struct attr wan_detect_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "threshold", show_value, show_value },
    { "timeout", show_value, show_value },
    { "type", show_value, show_value },
    { "host_type", show_value, show_value },
    { "host", show_value, show_value },
    { NULL, NULL, NULL }
};

/* type[alwayson|backup|backup_standby]^weight^target^backup^expect^failback[0|1] */
struct attr wan_lb_rule_attr[] = {
    { "type", show_value, show_value },
    { "weight", show_value, show_value },
    { "target", show_value, show_value },
    { "backup", show_value, show_value },
    { "failback", show_value, show_value },
    { NULL, NULL, NULL }
};

/* state^trycount^action^uptime^usb_trycount */
struct attr wan_status_rule_attr[] = {
    { "state", show_value, show_value },
    { "trycount", show_value, show_value },
    { "action", show_value, show_value },
    { "uptime", show_value, show_value },
#if defined (PLATFORM_AXA) || defined (PLATFORM_M2E) || defined (PLATFORM_ALS)
    { "wisp_trycount", show_value, show_value },
#endif
#if defined (EZP_PROD_BRAND_PROX)    
    { "usb_trycount", show_value, show_value },
#endif
#if defined (EZP_SUB_BRAND_GARMIN)
    { "priority_index", show_value, show_value },
    { "priority_num", show_value, show_value },
#endif
    { NULL, NULL, NULL }
};

/* Controlling ifcontrol do ifup/ifdown */
/* upfb^downfb */
struct attr wan_action_style_rule_attr[] = {
    { "upfb", show_value, show_value },
    { "downfb", show_value, show_value },
    { NULL, NULL, NULL }
};

/* uport^mnt */
struct attr storage_device_mount_rule_attr[] = {
    { "uport", show_value, show_value },
    { "mnt", show_value, show_value },
    { "smb_create_mask", show_value, show_value },
    { "smb_dir_mask", show_value, show_value },
    { "smb_guest_ok", show_value, show_value },
    { "smb_read_only", show_value, show_value },
    { NULL, NULL, NULL }
};
/* w_enable^w_countdown^w_count^p_enable^p_countdown^p_count */
struct attr countdown_rule_attr[] = {
    { "w_enable", show_value, show_value },
    { "w_countdown", show_value, show_value },
    { "w_count", show_value, show_value },
    { "p_enable", show_value, show_value },
    { "p_countdown", show_value, show_value },
    { "p_count", show_value, show_value },
    { NULL, NULL, NULL }
};

/* state^count */
struct attr license_status_rule_attr[] = {
    { "state", show_value, show_value },
    { "trycount", show_value, show_value },
    { NULL, NULL, NULL }
};

/* state[0|1|2]^action[none|up|down]^proto */
struct attr wan_upnp_status_rule_attr[] = {
    { "state", show_value, show_value },
    { "action", show_value, show_value },
    { "proto", show_value, show_value },
    { NULL, NULL, NULL }
};

/* type^dl^ul^percent */
struct attr wan_bw_rule_attr[] = {
    { "type", show_value, show_value },
    { "dl", show_value, show_value },
    { "ul", show_value, show_value },
    { "expert_dl", show_value, show_value },
    { "expert_ul", show_value, show_value },
    { "link_percent", show_value, show_value },
    { "global_percent", show_value, show_value },
    { "user_percent", show_value, show_value },
    { "dbm_upmax", show_value, show_value },
    { "dbm_upmin", show_value, show_value },
    { "dbm_downmax", show_value, show_value },
    { "dbm_downmin", show_value, show_value },
    { NULL, NULL, NULL }
};


/* ipaddr^mask^gateway^dns^wins */
struct attr wan_static_rule_attr[] = {
    { "ipaddr", show_value, show_value },
    { "mask", show_value, show_value },
    { "gateway", show_value, show_value },
    { "dns1", show_value, show_value },
    { "dns2", show_value, show_value },
    { "wins", show_value, show_value },
    { NULL, NULL, NULL }
};

/* hostname^domain^lease*/
struct attr wan_dhcp_rule_attr[] = {
    { "hostname", show_value, show_value },
    { "domain", show_value, show_value },
    { "lease", show_value, show_value },
    { NULL, NULL, NULL }
};

/* username^passwd^demand^idletime^echo_failure^redialperiod^mru^mtu */
struct attr wan_pppoe_rule_attr[] = {
    { "auth", show_value, show_value },
    { "username", show_b64_value, show_value },
    { "passwd", show_b64_value, show_value },
    { "demand", show_value, show_value },
    { "idletime", show_value, show_value },
    { "echo_failure", show_value, show_value },
    { "redialperiod", show_value, show_value },
    { "mru", show_value, show_value },
    { "mtu", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^server^username^passwd */
struct attr wan_bigpond_rule_attr[] = {
    { "enable", show_value, show_value },
    { "server", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { NULL, NULL, NULL }
};

/* apn_type^location^isp^apn^pin^mode^username^passwd^demand^idletime^
 * redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^ctrltty^devname^flag^turbolink
 */
struct attr wan_wwan_rule_attr[] = {
    { "apn_type", show_value, show_value },
    { "location", show_value, show_value },
    { "isp", show_value, show_value },
    { "apn", show_value, show_value },
    { "pin", show_value, show_value },
    { "mode", show_value, show_value },
    { "auth", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "demand", show_value, show_value },
    { "idletime", show_value, show_value },
    { "echo_failure", show_value, show_value },
    { "redialperiod", show_value, show_value },
    { "mru", show_value, show_value },
    { "mtu", show_value, show_value },
    { "dialstr", show_value, show_value },
    { "brand", show_value, show_value },
    { "model", show_value, show_value },
    { "ttynum", show_value, show_value },
    { "datatty", show_value, show_value },
    { "ctrltty", show_value, show_value },
    { "devname", show_value, show_value },
    { "flag", show_value, show_value },
    { "turbolink", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* barrypasswd^apn_type^location^isp^apn^usrname^password^dialstr^turbolink */
struct attr wan_barry_rule_attr[] = {
    { "barrypasswd", show_value, show_value },
    { "apn_type", show_value, show_value },
    { "location", show_value, show_value },
    { "isp", show_value, show_value },
    { "apn", show_value, show_value },
    { "username", show_value, show_value },
    { "password", show_value, show_value },
    { "dialstr", show_value, show_value },
    { "turbolink", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* hostname^domain^lease^mtu^turbolink */
struct attr wan_htc_rule_attr[] = {
    { "hostname", show_value, show_value },
    { "domain", show_value, show_value },
    { "lease", show_value, show_value },
    { "mtu", show_value, show_value },
    { "turbolink", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* isp^username^password */
struct attr wan_beceem_rule_attr[] = {
    { "isp", show_value, show_value },
    { "username", show_value, show_value },
    { "password", show_value, show_value },
    { NULL, NULL, NULL }
};

/* mtu^turbolink */
struct attr wan_wimax_rule_attr[] = {
    { "mtu", show_value, show_value },
    { "turbolink", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* hostname^domain^lease^mtu^turbolink */
struct attr wan_iphone_rule_attr[] = {
    { "hostname", show_value, show_value },
    { "domain", show_value, show_value },
    { "lease", show_value, show_value },
    { "mtu", show_value, show_value },
    { "turbolink", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* brand^model^signal^ttynum^datatty^ctrltty^devname^flag^sig^imei^provider^firmware^apn^name^vendor^product^stayfg */
struct attr wan_wwan_probe_rule_attr[] = {
    { "brand", show_value, show_value },
    { "model", show_value, show_value },
    { "signal", show_value, show_value },
    { "ttynum", show_value, show_value },
    { "datatty", show_value, show_value },
    { "ctrltty", show_value, show_value },
    { "devname", show_value, show_value },
    { "flag", show_value, show_value },
    { "sig", show_value, show_value },
    { "imei", show_value, show_value },
    { "provider", show_value, show_value },
    { "firmware", show_value, show_value },
    { "apn", show_value, show_value },
    { "vendor", show_value, show_value },
    { "product", show_value, show_value },
    { "stayfg", show_value, show_value },
    { NULL, NULL, NULL }
};

/* XX:XX:XX:XX:XX:XX */
struct attr wan_hwaddr_rule_default_attr[] = {
    { "hwaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* [0|1]^XX:XX:XX:XX:XX:XX */
struct attr wan_hwaddr_clone_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "hwaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/** 
 * name^enable[0|1]^type[dyndns|tzo|zoneedit]^
 * username^passwd^hostname^server
 */
struct attr wan_ddns_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "type", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "hostname", show_value, show_value },
    { "server", show_value, show_value },
    { NULL, NULL, NULL }
};

/* dns filter for ios 7 */
struct attr wan_apple_filter_rule_attr[] = {
    { "host", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};


/** 
 * name^enable[0|1]^
 * username^passwd^redirect[0|1]
 */
struct attr wan_opendns_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "redirect", show_value, show_value },
    { "label", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^mtu^stp^weight */
struct attr lan_main_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "mtu", show_value, show_value },
    { "stp", show_value, show_value },
    { "weight", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable[0:disabled|1:enabled]^selected(selected profile index)^enabled(enabled profile index) */
/* enable : vpn is enabled or not
 * selected : selected profile index
 * enabled : enabled profile index
 * */
struct attr wan_pptp_l2tp_global_rule_attr[] = {
    { "enable", show_value, show_value },
    { "selected", show_value, show_value },
    { "enabled", show_value, show_value },
};

/* enable^name^vpn_type^host^ipaddr^netmask^gateway^username^
 * passwd^nailup^timeout^pppiptype[isp|custom]^custom_ipaddr^
 * encmode[disabled|mppe]^auth[chap|pap|none] */
struct attr wan_pptp_l2tp_rule_attr[] = {
    { "enable", show_value, show_value },
    { "name", show_value, show_value },
    { "vpn_type", show_value, show_value },
    { "host", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "netmask", show_value, show_value },
    { "gateway", show_value, show_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "nailup", show_value, show_value },
    { "timeout", show_value, show_value },
    { "pppiptype", show_value, show_value },
    { "custom_ipaddr", show_value, show_value },
    { "encmode", show_value, show_value },
    { "auth", show_value, show_value },
    { NULL, NULL, NULL }
};

/* first_start */
struct attr wan_l2tp_status_rule_attr[] = {
    { "first_start", show_value, show_value },
    { NULL, NULL, NULL }
};

/* XX:XX:XX:XX:XX:XX */
struct attr lan_hwaddr_rule_default_attr[] = {
    { "hwaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* [0|1]^XX:XX:XX:XX:XX:XX */
struct attr lan_hwaddr_clone_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "hwaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* hw_enable */
struct attr nat_rule_attr[] = {
    { "hw_enable", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^ipaddr^mask^gateway */
struct attr guest_lan_rule_attr[] = {
    { "enable", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "mask", show_value, show_value },
    { NULL, NULL, NULL }
};

/* ipaddr^mask^gateway^dns^wins */
struct attr lan_static_rule_attr[] = {
    { "ipaddr", show_value, show_value },
    { "mask", show_value, show_value },
    { "gateway", show_value, show_value },
    { "dns", show_value, show_value },
    { "wins", show_value, show_value },
    { NULL, NULL, NULL }
};

 /*
  * name^enable[0|1]^start^num^lease^domain^wins^dnstype^dnsaddr1^dnsaddr2^dnsaddr3^dnsaddr4
  */
struct attr lan_dhcps_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "start", show_value, show_value },
    { "num", show_value, show_value },
    { "lease", show_value, show_value },
    { "domain", show_value, show_value },
    { "wins", show_value, show_value },
    { "dnstype", show_value, show_value},
    { "dnsaddr1", show_value, show_value},
    { "dnsaddr2", show_value, show_value},
    { "dnsaddr3", show_value, show_value},   
    { "dnsaddr4", show_value, show_value},
    { NULL, NULL, NULL }
};

 /*
  * hwaddr0^hwaddr1^hwaddr2^hwaddr3
  */
struct attr wl0_wds_attr[] = {
    { "hwaddr0", show_value, show_value },
    { "hwaddr1", show_value, show_value },
    { "hwaddr2", show_value, show_value },
    { "hwaddr3", show_value, show_value },
    { NULL, NULL, NULL }
};

/* mode[0:disabled|1:pin mode|2:PBC mode]^pin
 * ^reg_ssid^reg_auth^reg_enc^reg_keyindex^reg_key */
struct attr wl_wps_sta_rule_attr[] = {
    { "mode", show_value, show_value },
    { "pin", show_value, show_value },
    { "reg_ssid", show_value, show_value },
    { "reg_auth", show_value, show_value },
    { "reg_enc", show_value, show_value },
    { "reg_keyindex", show_value, show_value },
    { "reg_key", show_value, show_value },
    { NULL, NULL, NULL }
};

/* mode[0:disabled|7:enabled]^pin^configured[0:unconfigured|1:configured]^display */
struct attr wl_wps_rule_attr[] = {
    { "mode", show_value, show_value },
    { "pin", show_value, show_value },
    { "configured", show_value, show_value },
    { "display", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr bw_sbm_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "inipaddr_start", show_value, show_value },
    { "inipaddr_end", show_value, show_value },
    { "inipaddr_num", show_value, show_value },
    { "proto", show_value, show_value },
    { "port_start", show_value, show_value },
    { "port_end", show_value, show_value },
    { "extif", show_value, show_value },
    { "bw_alloc", show_value, show_value },
    { "bw_ratio", show_value, show_value },
    { "bw_upload", show_value, show_value },
    { "bw_download", show_value, show_value },
    { "sched_idx", show_value, show_value },
    { "excess", show_value, show_value },
    { "excess_ratio", show_value, show_value },
    { "excess_upload", show_value, show_value },
    { "excess_download", show_value, show_value },
#ifdef EZP_PROD_BRAND_PLANET
    { "dscpid", show_value, show_value },
    { "remarkdscpid", show_value, show_value },
    { "groupid", show_value, show_value },
#endif
    { NULL, NULL, NULL }
};

struct attr bw_dbm_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "inipaddr_start", show_value, show_value },
    { "inipaddr_end", show_value, show_value },
    { "inipaddr_num", show_value, show_value },
    { "sched_idx", show_value, show_value }
};

/* name^enable^bw_uprate^bw_upceil^prio */
struct attr bw_group_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "bw_uprate", show_value, show_value },
    { "bw_upceil", show_value, show_value },
    { "prio", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^srcipaddr_start^srcipaddr_end^dstipaddr_start^
    dstipaddr_end^proto^srcport_start^srcport_end^dstport_start^
    dstport_end^extif^routing_type[default|static]^gateway_ipaddr */
struct attr rt_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "srcipaddr_start", show_value, show_value },
    { "srcipaddr_end", show_value, show_value },
    { "dstipaddr_start", show_value, show_value },
    { "dstipaddr_end", show_value, show_value },
    { "proto", show_value, show_proto_value },
    { "srcport_start", show_value, show_value },
    { "srcport_end", show_value, show_value },
    { "dstport_start", show_value, show_value },
    { "dstport_end", show_value, show_value },
    { "extif", show_value, show_value },
    { "routing_type", show_value, show_value },
    { "gateway_ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* activex_enable^java_enable^proxy_enable */
struct attr wf_content_rule_attr[] = {
    { "activex_enable", show_value, show_enable_value },
    { "java_enable", show_value, show_enable_value },
    { "proxy_enable", show_value, show_enable_value },
};

 /* enable^keyword^type^access */
struct attr wf_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "keyword", show_value, show_value },
    { "type", show_value, show_value },
    { "access", show_value, show_value },
};

 /* name^enable[0|1]^used^start^stop^days */
struct attr sched_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "used", show_value, show_value },
    { "start_hour", show_value, show_value },
    { "start_min", show_value, show_value },
    { "stop_hour", show_value, show_value },
    { "stop_min", show_value, show_value },
    { "sun", show_value, show_value },
    { "mon", show_value, show_value },
    { "tue", show_value, show_value },
    { "wed", show_value, show_value },
    { "thu", show_value, show_value },
    { "fri", show_value, show_value },
    { "sat", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* enable^ntp_enable[0|1]^ntp_interval^reboot_interval^reboot_countdown^reboot_hour^ddns_countdown */
struct attr cron_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "ntp_enable", show_value, show_enable_value },
    { "ntp_interval", show_value, show_value },
    { "reboot_interval", show_value, show_value },
    { "reboot_countdown", show_value, show_value },
    { "reboot_hour", show_value, show_value },
    { "ddns_countdown", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^type[pool|ipaddr]^pool^ipaddr^zone */
struct attr ntp_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "type", show_value, show_value },
    { "pool", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "zone", show_value, show_value },
    { "daylight", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^ifname^vid^portmember^tag^prio */
struct attr vlan_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "ifname", show_value, show_value },
    { "vid", show_value, show_value },
    { "portmember", show_value, show_value },
#if defined(AR8316) || defined(AR8328)
    /* Per VLAN tag, RT3052 doesn't support. */
    { "tag", show_value, show_value },
    /* Per VLAN 801.1P priority, RT3052 doesn't support. */
    { "prio", show_value, show_value },
#endif
    { NULL, NULL, NULL }
};

 /* name^portid^pvid^tag^mode^gmemb^priority */
struct attr vlanport_rule_attr[] = {
    { "name", show_value, show_value },
    { "portid", show_value, show_value },
    { "pvid", show_value, show_value },
    /* Port based vlan, RT3052 supports. */
    { "tag", show_value, show_value },
#if defined(AR8316) || defined(AR8328)
    { "mode", show_value, show_value },
    { "gmemb", show_value, show_value },
#endif
    /* Per port 801.1P priority, RT3052 supports. */
    { "prio", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^ifnames^flag */
struct attr br_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "ifnames", show_value, show_value },
    { "flag", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^port */
struct attr ssh_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "port", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^ipaddr^ipt_enable^web_mgmt^dns^ppp^
    upnp^wireless^ntp^sys_warning^dhcp_serv^dhcp_cli^
    ddns^firewall^remote_enable[0|1]^remote_ip^proto^remote_port */
struct attr log_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "ipaddr", show_value, show_value },
    { "ipt_enable", show_value, show_enable_value },
    { "web_mgmt", show_value, show_enable_value },
    { "dns", show_value, show_enable_value },
    { "ppp", show_value, show_enable_value },
    { "upnp", show_value, show_enable_value },
    { "wireless", show_value, show_enable_value },
    { "ntp", show_value, show_enable_value },
    { "sys_warning", show_value, show_enable_value },
    { "dhcp_serv", show_value, show_enable_value },
    { "dhcp_cli", show_value, show_enable_value },
    { "ddns", show_value, show_enable_value },
    { "firewall", show_value, show_enable_value },
    { "remote_enable", show_value, show_enable_value },
    { "remote_ip", show_value, show_value },
    { "proto", show_value, show_value },
    { "remote_port", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^extif^inif^port */
struct attr upnp_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "pmp_enable", show_value, show_enable_value },
    { "extif", show_value, show_value },
    { "inif", show_value, show_value },
    { "port", show_value, show_value },
    { NULL, NULL, NULL }
};

/* 
 * name^enable[0|1]^extif^inif^proto^srcip^srcmask^dstip^dstmask^srcport^
 * dstport^action[(0)DROP|(1)ACCEPT]^mark^bw_idx^sched_idx
 */
struct attr fl_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "extif", show_value, show_if_value },
    { "inif", show_value, show_if_value },
    { "srcipaddr_start", show_value, show_value },
    { "srcipaddr_end", show_value, show_mask_value },
    { "dstipaddr_start", show_value, show_value },
    { "dstipaddr_end", show_value, show_mask_value},
    { "proto", show_value, show_proto_value },
    { "srcport_start", show_value, show_value },
    { "srcport_end", show_value, show_value },
    { "dstport_start", show_value, show_value },
    { "dstport_end", show_value, show_value },
    { "action", show_value, show_value },
    { "mark", show_value, show_value },
    { "bw_idx", show_value, show_value },
    { "sched_idx", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^hwaddr^action^acl_enable^dhcp_enable^arp_enable^
 * ipaddr */
struct attr fl_hwaddr_rule_attr[] = {
    { "name", show_value, show_value },
    { "hwaddr", show_value, show_value },
    { "action", show_value, show_value },
    { "acl_enable", show_value, show_enable_value },
    { "arp_enable", show_value, show_enable_value },
    { "dhcp_enable", show_value, show_enable_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable[0|1]^ipaddr */
struct attr fr_dmz_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* TOS enable^icmp_enable^dns_enable^ssh_enable^
   telnet_enable^check_enable^ack_enable */
struct attr tos_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "icmp_enable", show_value, show_enable_value },
    { "dns_enable", show_value, show_enable_value },
    { "ssh_enable", show_value, show_enable_value },
    { "telnet_enable", show_value, show_enable_value },
    { "check_enable", show_value, show_enable_value },
    { "ack_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^value */
struct attr ttl_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "value", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* user^permission_class */
struct attr user_rule_attr[] = {
    { "user", show_value, show_value },
    { "permission_class", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^rmgt_enable[0|1]^username^passwd^port */
struct attr http_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "rmgt_enable", show_value, show_enable_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "port", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* enable[0|1] */
struct attr http_redirect_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};


/* enable[0|1]^udpport^tcpport^community^vendor^sysname^location^contact^timeout^auth */
struct attr snmp_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "udpport", show_value, show_enable_value },
    { "tcpport", show_value, show_enable_value },
    { "community", show_value, show_value },
    { "vendor", show_value, show_value },
    { "sysname", show_value, show_value },
    { "location", show_value, show_value },
    { "contact", show_b64_value, show_value },
    { "timeout", show_value, show_value },
    { "auth", show_value, show_enable_value },
    { NULL, NULL, NULL }
};


 /* pingiface^pinghost^pingcnt^arpingiface^arpinghost^arpingcnt^tracerouteiface^traceroutehost^traceroutecnt */
struct attr adtool_rule_attr[] = {
    { "pingiface", show_value, show_value },
    { "pinghost", show_value, show_value },
    { "pingcnt", show_value, show_value },
    { "arpingiface", show_value, show_value },
    { "arpinghost", show_value, show_value },
    { "arpingcnt", show_value, show_value },
    { "tracerouteiface", show_value, show_value },
    { "traceroutehost", show_value, show_value },
    { "traceroutecnt", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* name^enable[0|1]^stime^ifnames */
struct attr stats_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "stime", show_value, show_value },
    { "ifnames", show_value, show_value },
    { NULL, NULL, NULL }
};

 /* enable[0|1]^vport^sport^ipaddr^mode^event_enable^event_ipaddr^event_port */
struct attr vnc_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "vport", show_value, show_value },
    { "sport", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { "mode", show_value, show_value },
    { "event_enable", show_value, show_enable_value },
    { "event_ipaddr", show_value, show_enable_value },
    { "event_port", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* ipaddr */
struct attr vnc_allow_rule_attr[] = {
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable[0|1]^name^redialperiod^mtu^mru^localip^localip_range^remoteip^remoteip_range^dns_auto_enable[0|1]^dns^chap_enable[0|1]^mschap_enable[0|1]^mschapv2_enable[0|1]^eap_enable[0|1]^pap_enable[0|1]^mppe128_enable[0|1]^proxyarp_enable[0|1]^nat_enable[0|1] */
struct attr pptpd_rule_attr[] = {
    { "enable", show_value, show_value },
    { "name", show_value, show_value },
    { "redialperiod", show_value, show_value },
    { "mtu", show_value, show_value },
    { "mru", show_value, show_value },
    { "localip", show_value, show_value },
    { "localip_range", show_value, show_value },
    { "remoteip", show_value, show_value },
    { "remoteip_range", show_value, show_value },
    { "dns_auto_enable", show_value, show_value },
    { "dns", show_value, show_value },
    { "chap_enable", show_value, show_value },
    { "mschap_enable", show_value, show_value },
    { "mschapv2_enable", show_value, show_value },
    { "eap_enable", show_value, show_value },
    { "pap_enable", show_value, show_value },
    { "mppe128_enable", show_value, show_value },
    { "proxyarp_enable", show_value, show_value },
    { "nat_enable", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr pptpd_user_rule_attr[] = {
    { "enable", show_value, show_value },
    { "username", show_value, show_value },
    { "provider", show_value, show_value },
    { "passwd", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable[0|1]^name^mtu^mru^localip^localip_range^remoteip^remoteip_range^dns^proxyarp_enable[0|1]^nat_enable[0|1] */
struct attr l2tpd_rule_attr[] = {
    { "enable", show_value, show_value },
    { "name", show_value, show_value },
    { "mtu", show_value, show_value },
    { "mru", show_value, show_value },
    { "localip", show_value, show_value },
    { "localip_range", show_value, show_value },
    { "remoteip", show_value, show_value },
    { "remoteip_range", show_value, show_value },
    { "dns_auto_enable", show_value, show_value },
    { "dns", show_value, show_value },
    { "chap_enable", show_value, show_value },
    { "pap_enable", show_value, show_value },
    { "proxyarp_enable", show_value, show_value },
    { "nat_enable", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr l2tpd_user_rule_attr[] = {
    { "enable", show_value, show_value },
    { "username", show_value, show_value },
    { "provider", show_value, show_value },
    { "passwd", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr wol_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "bcast", show_value, show_value },
    { "hwaddr", show_value, show_value },
    { "iface", show_value, show_value },
    { "passwd", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^
 * remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^
 * phase1_mode^phase1_auth^phase1_encrypt^phase1_group^phase1_left_id^
 * phase1_right_id^phase2_auth^phase2_encrypt^phase2_group^phase1_lifetime^
 * phase2_lifetime^dpd_enable^dpd_interval^dpd_timeout^ddns_gateway^
 * split_tunnel[enabled|initiator|responder]  */
struct attr ipsec_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "mode", show_value, show_value },
    { "l2tp", show_value, show_value },
    { "local_extif", show_value, show_value },
    { "local_inipaddr", show_value, show_value },
    { "local_netmask", show_value, show_value },
    { "remote_gateway", show_value, show_value },
    { "remote_inipaddr", show_value, show_value },
    { "remote_netmask", show_value, show_value },
    { "conn_init", show_value, show_value },
    { "ike_keymode", show_value, show_value },
    { "psk", show_value, show_value },
    { "rsa", show_value, show_value },
    { "ca", show_value, show_value },
    { "adv", show_value, show_value },
    { "phase1_mode", show_value, show_value },
    { "phase1_left_id", show_value, show_value },
    { "phase1_right_id", show_value, show_value },
    { "phase1_lifetime", show_value, show_value },
    { "phase2_lifetime", show_value, show_value },
    { "phase1_auth", show_value, show_value },
    { "phase1_encrypt", show_value, show_value },
    { "phase1_group", show_value, show_value },
    { "phase2_auth", show_value, show_value },
    { "phase2_encrypt", show_value, show_value },
    { "phase2_group", show_value, show_value },
    { "dpd_enable", show_value, show_value },
    { "dpd_interval", show_value, show_value },
    { "dpd_timeout", show_value, show_value },
    { "ddns_gateway", show_value, show_value },
#ifdef EZP_PROD_BRAND_PROX
    { "split_tunnel", show_value, show_value },
#endif
    { NULL, NULL, NULL }
};

/* phase1[none|established]^phase2[none|established] */
struct attr ipsec_status_rule_attr[] = {
    { "phase1", show_value, show_value },
    { "phase2", show_value, show_value },
    { NULL, NULL, NULL }
};

/* web_serv_ipaddr^web_serv_port^logon_page^status_page^logoff_page^admin_ipaddr^dns_type^dns_ipaddr^radius_enable^radius_ipaddr^radius_auth_port^radius_accp_port^radius_secret^nasid^locid^rlocname^rlocid^userdb */
struct attr auth_server_rule_attr[] = {
    { "web_serv_ipaddr" , show_value, show_value },
    { "web_serv_port", show_value, show_value },
    { "logon_page"  , show_value, show_value },
    { "status_page" , show_value, show_value },
    { "logoff_page" , show_value, show_value },
    { "admin_ipaddr"    , show_value, show_value },
    { "dns_type"    , show_value, show_value },
    { "dns_ipaddr"      , show_value, show_value },
    { "radius_enable"   , show_value, show_enable_value },
    { "radius_ipaddr"   , show_value, show_value },
    { "radius_auth_port", show_value, show_value },
    { "radius_accp_port", show_value, show_value },
    { "radius_secret"   , show_value, show_value },
    { "nasid"      , show_value, show_value },
    { "locid"      , show_value, show_value },
    { "rlocname"   , show_value, show_value },
    { "rlocid"     , show_value, show_value },
    { "userdb"     , show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^password^valid_time */
struct attr auth_user_rule_attr[] = {
    { "name"    , show_value, show_value },
    { "password", show_value, show_value },
    { "valid_time"  , show_value, show_value },
    { NULL, NULL, NULL }
};

 /* enable^syn_enable^icmp_enable^icmp_redir_enable
    ^rpfilter_enable^state_enable^bc_storm_enable */
struct attr fw_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "syn_enable", show_value, show_enable_value },
    { "icmp_enable", show_value, show_enable_value },
    { "icmp_redir_enable", show_value, show_enable_value },
    { "rpfilter_enable", show_value, show_enable_value },
    { "state_enable", show_value, show_enable_value },
    { "bc_storm_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

 /* enable^ipsec_enable^pptp_enable^l2tp_enable^
    h323_enable^rtsp_enable^mms_enable */
struct attr nat_pass_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "ipsec_enable", show_value, show_enable_value },
    { "pptp_enable", show_value, show_enable_value },
    { "l2tp_redir_enable", show_value, show_enable_value },
    { "h323_enable", show_value, show_enable_value },
    { "rtsp_enable", show_value, show_enable_value },
    { "mms_enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* For new added wireless rules */
/* Physical interface related rules*/
/* enable^net_mode^txpower^channel^bisolation */
struct attr wl_basic_rule_attr[] = {
    { "enable", show_value, show_value },
    { "net_mode", show_value, show_value },
    { "txpower", show_value, show_value },
    { "channel", show_value, show_value },
    { "bisolation", show_value, show_value },
    { NULL, NULL, NULL }
};

/* infra^ap_mode^rateset^mrate^frag^rts^dtim^bcn^plcphdr^
antdiv^gmode^gmode_protection^afterburner^frameburst */
struct attr wl_advanced_rule_attr[] = {
    { "infra", show_value, show_value },
    { "ap_mode", show_value, show_value },
    { "rateset", show_value, show_value },
    { "mrate", show_value, show_value },
    { "frag", show_value, show_value },
    { "rts", show_value, show_value },
    { "dtim", show_value, show_value },
    { "bcn", show_value, show_value },
    { "plcphdr", show_value, show_value },
    { "antdiv", show_value, show_value },
    { "gmode", show_value, show_value },
    { "gmode_protection", show_value, show_value },
    { "afterburner", show_value, show_value },
    { "frameburst", show_value, show_value },
    { "txburst", show_value, show_enable_value},
    { "pktaggr", show_value, show_enable_value},
    { "txmode", show_value, show_value},

    { "rdg", show_value, show_enable_value},
    { "linkadapt", show_value, show_enable_value},
    { "htc", show_value, show_enable_value},

    { "htbw", show_value, show_enable_value},
    { "gi", show_value, show_enable_value},
    { "opmode", show_value, show_value},
    { "mcs", show_value, show_value},
    { "mpdu", show_value, show_value},
    { "amsdu", show_value, show_enable_value},
    { "autoba", show_value, show_enable_value},
    { "bawinsize", show_value, show_value},
    { "badecline", show_value, show_enable_value},
    { "extcha", show_value, show_value},
    { "wdstxmode", show_value, show_value},
    { NULL, NULL, NULL }
};

/* mode^peer_list^timeout */
/* mode:0 disable 1:restrict 2:bridge 3:repeater 4:lazy*/
struct attr wl_wds_rule_attr[] = {
    { "mode", show_value, show_value },
    { "timeout", show_value, show_value },
    { NULL, NULL, NULL }
};

/* sta_bk^Sta_be^sta_vi^sta_vo^ap_bk^ap_be^ap_vi^ap_vo^no_ack^max_assoc */
struct attr wl_wme_rule_attr[] = {
    { "sta_bk", show_value, show_value },
    { "sta_be", show_value, show_value },
    { "sta_vi", show_value, show_value },
    { "sta_vo", show_value, show_value },
    { "ap_bk", show_value, show_value },
    { "ap_be", show_value, show_value },
    { "ap_vi", show_value, show_value },
    { "ap_vo", show_value, show_value },
    { "no_ack", show_value, show_value },
    { "max_assoc", show_value, show_value },
    { NULL, NULL, NULL }
};

/* Virtual interface related rules for wl0/wl1/wl2/wl3 */
/* enable^hidden^rate^wme^isolation */
struct attr wlv_basic_rule_attr[] = {
    { "enable", show_value, show_value },
    { "hidden", show_value, show_value },
    { "rate", show_value, show_value },
    { "wme", show_value, show_value },
    { "isolation", show_value, show_value },
    { NULL, NULL, NULL }
};

/* ifname */
struct attr wlv_ifname_rule_attr[] = {
    { "ifname", show_value, show_value },
    { NULL, NULL, NULL }
};

/* ssid */
struct attr wlv_ssid_rule_attr[] = {
    { "ssid", show_value, show_value },
    { NULL, NULL, NULL }
};

/* secmode */
struct attr wlv_sec_rule_attr[] = {
    { "secmode", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key_index^key1^key2^key3^key4^
 * keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
struct attr wlv_sec_wep_rule_attr[] = {
    { "key_index", show_value, show_value },
    { "key1", show_b64_value, show_value },
    { "key2", show_b64_value, show_value },
    { "key3", show_b64_value, show_value },
    { "key4", show_b64_value, show_value },
    { "keytype", show_value, show_value },
    { "encmode", show_value, show_value },
    { NULL, NULL, NULL }
};

/* smbd rule
 * enable^name^workgroup^description^create_mask^dir_mask^security^share_name^share_path^share_users^read_only^guest_ok^ext_access^codepage
 */
struct attr smbd_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "name", show_value, show_value },
    { "workgroup", show_value, show_value },
    { "description", show_value, show_value },
    { "create_mask", show_value, show_value },
    { "dir_mask", show_value, show_value },
    { "security", show_value, show_value },
    { "share_name", show_value, show_value },
    { "share_path", show_value, show_value },
    { "share_users", show_value, show_value },
    { "read_only", show_value, show_value },
    { "guest_ok", show_value, show_value },
    { "ext_access", show_value, show_value },
    { "codepage", show_value, show_value },
    { NULL, NULL, NULL }
};
/* smbd user rule
 * enable^name^passwd
 */
struct attr smbd_user_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "name", show_value, show_value },
    { "passwd", show_value, show_value },
    { NULL, NULL, NULL }
};
/* togo rule
 * status^name^proto^ip^port^mac^config
 */
struct attr togo_rule_attr[] = {
    { "status", show_value, show_value },
    { "name", show_value, show_value },
    { "proto", show_value, show_value },
    { "ip", show_value, show_value },
    { "port", show_value, show_value },
    { "mac", show_value, show_value },
    { "config", show_value, show_value },
    { NULL, NULL, NULL }
};
/* camera rule
 * enable^port^format^auth_enable^user^passwd^config[0|1]^resolution
 */
struct attr camera_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "port", show_value, show_value },
    { "format", show_value, show_value },
    { "auth_enable", show_value, show_enable_value },
    { "user", show_value, show_value },
    { "passwd", show_value, show_value },
    { "config", show_value, show_value },
    { "resolution", show_value, show_value },
    { NULL, NULL, NULL }
};
/* drive rule
 * enable^guset^port^ext_access^name^password^config
 */
struct attr drive_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "guest", show_value, show_value },
    { "port", show_value, show_value },
    { "ext_access", show_value, show_value },
    { "name", show_value, show_value },
    { "password", show_value, show_value },
    { "config", show_value, show_value },
    { NULL, NULL, NULL }
};
/* music rule
 * enable
 */
struct attr music_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { NULL, NULL, NULL }
};
/* led rule
 * enable^guset^name^password
 */
struct attr led_rule_attr[] = {
    { "boot", show_value, show_value },
    { "usb", show_value, show_value },
    { NULL, NULL, NULL }
};
/* bt rule
 * enable[0|1]^peer^max_peer^max_task^download_rate^upload_rate^directory^session^load_start^port_range_start^port_range_end^dht_port^check_hash
 */
struct attr bt_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "peer", show_value, show_value },
    { "max_peer", show_value, show_value },
    { "max_task", show_value, show_value },
    { "download_rate", show_value, show_value },
    { "upload_rate", show_value, show_value },
    { "directory", show_value, show_value },
    { "session", show_value, show_value },
    { "load_start", show_value, show_value },
    { "port_range_start", show_value, show_value },
    { "port_range_end", show_value, show_value },
    { "dht_port", show_value, show_value },
    { "check_hash", show_value, show_value },
    { NULL, NULL, NULL }
};
/*FTP rule
 enable[0|1]^port^noanonyous^chrooteveryone^maxclientsperip^maxclientnumber^peruserlimits^umask^userbandwidth^timeout^allow_ext_access
 */
struct attr ftpd_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "port", show_value, show_value },
    { "noanonyous", show_value, show_value },
    { "chrooteveryone", show_value, show_value },
    { "maxclientsperip", show_value, show_value },
    { "maxclientnumber", show_value, show_value },
    { "peruserlimits", show_value, show_value },
    { "umask", show_value, show_value },
    { "userbandwidth", show_value, show_value },
    { "timeout", show_value, show_value },
    { "allow_ext_access", show_value, show_value },
    { NULL, NULL, NULL }
};
/* FTP user rule
   enable[1|0]^user^pw^upload^download^ipaddr
 */
struct attr ftpd_user_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "username", show_value, show_value },
    { "passwd", show_value, show_value },
    { "upload", show_value, show_value },
    { "download", show_value, show_value },
    { "ipaddr", show_value, show_value },
    { NULL, NULL, NULL }
};
/* iTunes server rule
   enable[0|1]^port^adminpasswd^mp3dir^servername^runas^password^extensions
 */
struct attr itunes_rule_attr[] = {
        { "enable", show_value, show_enable_value },
        { "port", show_value, show_value },
        { "adminpasswd", show_value, show_value },
        { "mp3dir", show_value, show_value },
        { "servername", show_value, show_value },
        { "runas", show_value, show_value },
        { "password", show_value, show_value },
        { "extensions", show_value, show_value },
};
/*Storage rule
  codepage^swap_size^block_size^ext_enable^target_ip^target_dir^user^pw^gp 
 */
struct attr storage_rule_attr[] = {
    { "codepage", show_value, show_value },
    { "swap_size", show_value, show_value },
    { "block_size", show_value, show_value },
    { "ext_enable", show_value, show_enable_value },
    { "target_ip", show_value, show_value },
    { "target_dir", show_value, show_value },
    { "user", show_value, show_value },
    { "pw", show_value, show_value },
    { "gp", show_value, show_value },
    { NULL, NULL, NULL }
};

/*Storage state rule
 dev_name^format^mount_state^insert_state
 */
struct attr storage_state_rule_attr[] = {
    { "dev_name", show_value, show_value },
    { "format", show_value, show_value },
    { "mount_state", show_value, show_value },
    { "insert_state", show_value, show_value },
    { NULL, NULL, NULL }
};

/*Storage used state rule
 dev_name^mount_state^free_state^total_state^diff_state
 */
struct attr storage_used_state_rule_attr[] = {
    { "dev_name", show_value, show_value },
    { "mount_state", show_value, show_value },
    { "free_state", show_value, show_value },
    { "total_state", show_value, show_value },
    { "diff_state", show_value, show_value },
    { NULL, NULL, NULL }
};
/*myqnapcloud_id rule
  version^app_token^refresh_token^auth_token^token_valid_period^last_token_update^qid^device_id^device_token^device_name^status^ddns_status
*/
struct attr myqnapcloud_id_rule_attr[] = {
    { "version", show_value, show_value },
    { "app_token", show_value, show_value },
    { "refresh_token", show_value, show_value },
    { "auth_token", show_value, show_value },
    { "token_valid_period", show_value, show_value },
    { "last_token_update", show_value, show_value },
    { "qid", show_value, show_value },
    { "device_id", show_value, show_value },
    { "device_token", show_value, show_value },
    { "device_name", show_value, show_value },
    { "status", show_value, show_value },
    { "ddns_status", show_value, show_value },
    { NULL, NULL, NULL }
};
struct attr model_name_attr[] = {
    { "modelname", show_value, show_value },
    { NULL, NULL, NULL }
};
/* iSCSI target rule
 * enable^isns_serv^isid^alias^max_conn^max_tx^max_rx
 */
struct attr is_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "isns_serv", show_value, show_value },
    { "isid", show_value, show_value },
    { "alias", show_value, show_value },
    { "max_conn", show_value, show_value },
    { "max_tx", show_value, show_value },
    { "max_rx", show_value, show_value },
    { NULL, NULL, NULL }
};

/* iSCSI host rule
 enable^host^action
 */
struct attr ishost_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "host", show_value, show_value },
    { "action", show_value, show_value },
    { NULL, NULL, NULL }
};

/* iSCSI user rule
 enable^username^password
 */
struct attr isuser_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "username", show_value, show_value },
    { "password", show_value, show_value },
    { NULL, NULL, NULL }
};

/* Application manager rule */
/* wlan^iscsi^pptp^l2tp^ipsec^storage^wwan^wisp^portcfg^hwnat^firmware^
 * iphone^brry^smart^usbled^wimax^beceem^guest_lan^vpnc^wire */
struct attr appmgr_rule_attr[] = {
    { "wlan", show_value, show_value },
    { "iscsi", show_value, show_value },
    { "pptp", show_value, show_value },
    { "l2tp", show_value, show_value },
    { "ipsec", show_value, show_value },
    { "storage", show_value, show_value },
    { "wwan", show_value, show_value },
    { "wisp", show_value, show_value },
    { "portcfg", show_value, show_value },
    { "hwnat", show_value, show_value },
    { "firmware", show_value, show_value },
    { "iphone", show_value, show_value },
    { "barry", show_value, show_value },
    { "smart", show_value, show_value },
    { "usbled", show_value, show_value },
    { "wimax", show_value, show_value },
    { "beceem", show_value, show_value },
    { "guest_lan", show_value, show_value },
    { "vpnc", show_value, show_value },
    { "wire", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
 * rekey_time_interval^rekey_pkt_interval^session_timeout */
struct attr wlv_sec_wpa_rule_attr[] = {
    { "key", show_b64_value, show_value },
    { "crypto", show_value, show_value },
    { "radius_key", show_b64_value, show_value },
    { "radius_ipaddr", show_value, show_value },
    { "radius_port", show_value, show_value },
    { "rekey_mode", show_value, show_value },
    { "rekey_time_interval", show_value, show_value },
    { "rekey_pkt_interval", show_value, show_value },
    { "session_timeout", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
 * rekey_time_interval^rekey_pkt_interval^preauth^pmkperiod^session_timeout */
struct attr wlv_sec_wpa2_rule_attr[] = {
    { "key", show_b64_value, show_value },
    { "crypto", show_value, show_value },
    { "radius_key", show_b64_value, show_value },
    { "radius_ipaddr", show_value, show_value },
    { "radius_port", show_value, show_value },
    { "rekey_mode", show_value, show_value },
    { "rekey_time_interval", show_value, show_value },
    { "rekey_pkt_interval", show_value, show_value },
    { "preauth", show_value, show_value },
    { "pmkperiod", show_value, show_value },
    { "session_timeout", show_value, show_value },
    { "wpacap", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^defpolicy */
struct attr wlv_acl_basic_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "defpolicy", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^enable^mac^policy */
struct attr wlv_acl_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "mac", show_value, show_value },
    { "policy", show_value, show_value },
    { NULL, NULL, NULL }
};

/* wds related rules */
/* hwaddr^secmode */
struct attr wl_wds_basic_rule_attr[] = {
    { "hwaddr", show_value, show_value },
    { "secmode", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key */
struct attr wl_wds_sec_wep_rule_attr[] = {
    { "key", show_b64_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto */
struct attr wl_wds_sec_wpa_rule_attr[] = {
    { "key", show_b64_value, show_value },
    { "crypto", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^ssid^bssid^secmode^wifiwan^ip_type[dhcp|static] */
struct attr wl_apcli_rule_attr[] = {
    { "enable", show_value, show_value },
    { "ssid", show_value, show_value },
    { "bssid", show_value, show_value },
    { "secmode", show_value, show_value },
    { "wifiwan", show_value, show_value },
#if defined(EZP_SUB_BRAND_APOLLO) || defined(EZP_PROD_BRAND_PROX)
    { "ip_type", show_value, show_value },
#endif
    { NULL, NULL, NULL }
};

/* key_index^key1^key2^key3^key4^keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
struct attr wl_apcli_sec_wep_rule_attr[] = {
    { "key_index", show_value, show_value },
    { "key1", show_b64_value, show_value },
    { "key2", show_b64_value, show_value },
    { "key3", show_b64_value, show_value },
    { "key4", show_b64_value, show_value },
    { "keytype", show_value, show_value },
    { "encmode", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto */
struct attr wl_apcli_sec_wpa_rule_attr[] = {
    { "key", show_b64_value, show_value },
    { "crypto", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^max^index */
struct attr wl_apcli_swap_basic_rule_attr[] = {
    { "enable", show_value, show_value },
    { "max", show_value, show_value },
    { "index", show_value, show_value },
    { NULL, NULL, NULL }
};

/* channel^extcha */
struct attr wl_apcli_swap_channel_rule_attr[] = {
    { "channel", show_value, show_value },
    { "extcha", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^ssid^bssid^secmode^wifiwan^ip_type[dhcp|static] */
struct attr wl_apcli_swap_rule_attr[] = {
    { "enable", show_value, show_value },
    { "ssid", show_value, show_value },
    { "bssid", show_value, show_value },
    { "secmode", show_value, show_value },
    { "wifiwan", show_value, show_value },
#if defined(EZP_SUB_BRAND_APOLLO) || defined(EZP_PROD_BRAND_PROX)
    { "ip_type", show_value, show_value },
#endif
    { NULL, NULL, NULL }
};

/* key_index^key1^key2^key3^key4^keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
struct attr wl_apcli_swap_sec_wep_rule_attr[] = {
    { "key_index", show_value, show_value },
    { "key1", show_b64_value, show_value },
    { "key2", show_b64_value, show_value },
    { "key3", show_b64_value, show_value },
    { "key4", show_b64_value, show_value },
    { "keytype", show_value, show_value },
    { "encmode", show_value, show_value },
    { NULL, NULL, NULL }
};

/* key^crypto */
struct attr wl_apcli_swap_sec_wpa_rule_attr[] = {
    { "key", show_b64_value, show_value },
    { "crypto", show_value, show_value },
    { NULL, NULL, NULL }
};


/* name^prod_cat */
struct attr x_mode_data_attr[] = {
    { "name", show_value, show_value },
    { "prod_cat", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^val^dispstr^lan_member^wan_member */
struct attr portcfg_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_value },
    { "dispstr", show_value, show_value },
    { "lan_member", show_value, show_value },
    { "wan_member", show_value, show_value },
    { NULL, NULL, NULL }
};

/* app_enable_rule */
/* sbm^dbm^gbm */
struct attr app_enable_rule_attr[] = {
    { "sbm", show_value, show_value },
    { "dbm", show_value, show_value },
    { "gbm", show_value, show_value },
    { NULL, NULL, NULL }
};

/* serial_number */
/* serial_num */
struct attr serial_num_rule_attr[] = {
    { "serial_num", show_value, show_value },
    { NULL, NULL, NULL }
};

/* MFG testing result */
/* test_item^result */
struct attr mfg_test_result_rule_attr[] = {
    { "item", show_value, show_value },
    { "result", show_value, show_value },
    { NULL, NULL, NULL }
};
#if defined (PLATFORM_GMA)
/* detect captive portal and network status */
/* captive_portal^status */
/* captive_portal:0-no captive, 1-captive, 2-after entering username and passwd */
/* status:0-could not get ncsi file, 1- could get ncsi file */
struct attr WISP_dtct_captive_portal_rule_attr[] = {
    { "captive_portal", show_value, show_value },
    { "status", show_value, show_value },
    { NULL, NULL, NULL }
};
#endif

/* IGMP PROXY FUNCTION */
struct attr igmp_proxy_rule_attr[] = {
    { "enable", show_value, show_value },
    { NULL, NULL, NULL }
};

/* ppp_control_rule */
/* gateway_valid*/
struct attr ppp_control_rule_attr[] = {
    { "gateway_valid", show_value, show_value },
    { NULL, NULL, NULL }
};

struct attr wan_priority_rule_attr[] = {
    { "proto", show_value, show_value },
    { NULL, NULL, NULL }
};

/* enable^port */
struct attr captive_rule_attr[] = {
    { "enable", show_value, show_enable_value },
    { "port", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

/* For system view, some multiple card reader can read different size sdcards
 * with mutiple gpios, such as mosra.
 * We define a card reader with "device point", "gpio number" and "gpio polarity"
 * dev^gpio^gpiopol */
struct attr card_reader_hardware_rule_attr[] = {
    { "dev", show_value, show_enable_value },
    { "gpio", show_value, show_enable_value },
    { "gpiopol", show_value, show_enable_value },
    { NULL, NULL, NULL }
};

#if defined (PLATFORM_M2E)
/* num^enable^
 * func[in|out|nc]^
 *   gpio function :
 *   in : config as gpio input
 *   out : config as gpio output
 *   nc : config as gpio non-configured
 * initval^
 *   gpio initial value
 *   0 or 1 : set to 0 or 1 at boot
 *   nc : do not set at boot time
 * desc 
 *   brief description of the gpio
 * */
struct attr gpiodef_rule_attr[] = {
    { "num", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "func", show_value, show_value },
    { "initval", show_value, show_value },
    { "desc", show_value, show_value },
    { NULL, NULL, NULL }
};

/* upnpc_setting_rule used for upnp-client as default port redirect
 * upnpc_status_rule used for upnp-client as real bounded port 
 * ie : we may have a 
 * upnpc_setting_rule as inport:80 outport:8880, 
 * but after binding, the port finally get 8890, so the
 * upnpc_status_rule as inport:80 outport:8890 */
/* name^enable^inport^outport^proto */
struct attr upnpc_setting_rule_attr[] = {
    { "name", show_value, show_value },
    { "enable", show_value, show_enable_value },
    { "inport", show_value, show_value },
    { "outport", show_value, show_value },
    { "proto", show_value, show_value },
    { NULL, NULL, NULL }
};

/* name^avtivated^inport^outport */
struct attr upnpc_status_rule_attr[] = {
    { "name", show_value, show_value },
    { "activated", show_value, show_value },
    { "inport", show_value, show_value },
    { "outport", show_value, show_value },
    { "fetch_result", show_value, show_value },
    { NULL, NULL, NULL }
};

/* defpath^dev^linkp */
struct attr st_defpath_rule_attr[] = {
    { "defpath", show_value, show_value },
    { "dev", show_value, show_value },
    { "linkp", show_value, show_value },
    { NULL, NULL, NULL }
};
#endif

#if defined (PLATFORM_GMA)
/* GARMIN nat rule */
/* orig_intf^orig_ip^targ_intf^targ_ip */
struct attr gmn_nat_rule_attr[] = {
    {"orig_intf", show_value, show_value},
    {"orig_ip", show_value, show_value},
    {"targ_intf", show_value, show_value},
    {"targ_ip", show_value, show_value},
    { NULL, NULL, NULL }
};

/* GARMIN port filter */
/* src_intf^dst_intf^portfrom^portto */
struct attr gmn_filter_rule_attr[] = {
    {"src_intf", show_value, show_value},
    {"dst_intf", show_value, show_value},
    {"port_from", show_value, show_value},
    {"port_to", show_value, show_value},
    { NULL, NULL, NULL }
};

/* GARMIN Multicast rule */
/* orig_intf^orig_ip^mcast_intf^mcast_group */
struct attr gmn_mcast_rule_attr[] = {
    {"orig_intf", show_value, show_value},
    {"orig_ip", show_value, show_value},
    {"mcast_intf", show_value, show_value},
    {"mcast_group", show_value, show_value},
    { NULL, NULL, NULL }
};
#endif

struct rule {
    char *name;
    struct attr *attr;
};

struct rule rules[] = {
    { "fr_rule", fr_rule_attr },
    { "plug_rule", plug_rule_attr },
    { "cnnt_rule", cnnt_rule_attr },
    { "wizard_rule", wizard_rule_attr },
    { "vs_rule", vs_rule_attr },
    { "fl_rule", fl_rule_attr },
    { "fr_dmz_rule", fr_dmz_rule_attr },
    { "fl_hwaddr_rule", fl_hwaddr_rule_attr },
#ifdef AXIMDDNS
    { "aximddns_rule", aximddns_rule_attr },
    { "aximddns_tmp_rule", aximddns_tmp_rule_attr },
#endif
#ifdef QFILE_CGI
    { "cgi_progress", cgi_progress_attr },
#endif
    { "mac_sense_rule", mac_sense_attr },	
#if defined(PLATFORM_ALS) 
    { "als_failover", failover_attr },   
    { "alert_rule", alert_attr },   
    { "mail_rule", mail_attr },   
    { "relay_rule", relay_attr },   
    { "relay_schedule_rule", relay_schedule_attr },    
    { "file_mode_rule", file_mode_attr },  
    { "als_battery_rule", als_battery_rule_attr},
    { "als_status_rule", als_status_rule_attr},
    { "als_serial_rule", als_serial_rule_attr},
    { "als_operating_rule", als_operating_rule_attr},
    { "als_operating_data_packing_rule", als_operating_data_packing_rule_attr},
    { "als_operating_option_rule", als_operating_option_rule_attr},
#endif   
    { "wan_main_rule", wan_main_rule_attr },
    { "wan_lb_rule", wan_lb_rule_attr },
    { "wan_detect_rule", wan_detect_rule_attr },
    { "wan_status_rule", wan_status_rule_attr },
    { "wan_action_style_rule", wan_action_style_rule_attr },
    { "storage_device_mount_rule", storage_device_mount_rule_attr },
    { "countdown_rule", countdown_rule_attr },
    { "license_status_rule", license_status_rule_attr },
    { "vpn_status_rule", wan_status_rule_attr },
    { "wan_upnp_status_rule", wan_upnp_status_rule_attr },
    { "wan_bw_rule", wan_bw_rule_attr },
    { "wan_static_rule", wan_static_rule_attr },
    { "wan_dhcp_rule", wan_dhcp_rule_attr },
    { "wan_pppoe_rule", wan_pppoe_rule_attr },
    { "wan_wwan_rule", wan_wwan_rule_attr },
    { "wan_barry_rule", wan_barry_rule_attr },
    { "wan_pptp_l2tp_global_rule", wan_pptp_l2tp_global_rule_attr },
    { "wan_pptp_l2tp_rule", wan_pptp_l2tp_rule_attr },
    { "wan_l2tp_status_rule", wan_l2tp_status_rule_attr },
    { "wan_htc_rule", wan_htc_rule_attr },
    { "wan_iphone_rule", wan_iphone_rule_attr },
    { "wan_beceem_rule", wan_beceem_rule_attr },
    { "wan_wimax_rule", wan_wimax_rule_attr },
    { "wan_wwan_probe_rule", wan_wwan_probe_rule_attr },
    { "wan_bigpond_rule", wan_bigpond_rule_attr },
    { "wan_hwaddr_rule_default", wan_hwaddr_rule_default_attr},
    { "wan_hwaddr_clone_rule", wan_hwaddr_clone_rule_attr },
    { "wan_ddns_rule", wan_ddns_rule_attr },
    { "wan_apple_filter_rule", wan_apple_filter_rule_attr },
    { "wan_opendns_rule", wan_opendns_rule_attr },
    { "lan_main_rule", lan_main_rule_attr },
    { "lan_hwaddr_rule_default", lan_hwaddr_rule_default_attr },
    { "lan_hwaddr_clone_rule", lan_hwaddr_clone_rule_attr },
    { "guest_lan_rule", guest_lan_rule_attr },
    { "lan_static_rule", lan_static_rule_attr },
    { "lan_status_rule", wan_status_rule_attr },
    { "lan_static_rule_default", lan_static_rule_attr },
    { "lan_dhcps_rule", lan_dhcps_rule_attr },
    /* XXX: TODO: Fix this in the future. */
    { "wl0_wds", wl0_wds_attr },
    { "bw_sbm_rule", bw_sbm_rule_attr },
    { "bw_dbm_rule", bw_dbm_rule_attr },
    { "rt_rule", rt_rule_attr },
    { "wf_content_rule", wf_content_rule_attr },
    { "wf_rule", wf_rule_attr },
    { "sched_rule", sched_rule_attr },
    { "cron_rule", cron_rule_attr },
    { "vlan_rule", vlan_rule_attr },
    { "vlanport_rule", vlanport_rule_attr },
    { "br_rule", br_rule_attr },
    { "ntp_rule", ntp_rule_attr },
    { "ssh_rule", ssh_rule_attr },
    { "log_rule", log_rule_attr },
    { "ttl_rule", ttl_rule_attr },
    { "tos_rule", tos_rule_attr },
    { "upnp_rule", upnp_rule_attr },
    { "http_rule", http_rule_attr },
    { "user_rule", user_rule_attr },
    { "http_redirect_rule", http_redirect_rule_attr },
    { "snmp_rule", snmp_rule_attr },
    { "adtool_rule", adtool_rule_attr },
    { "smbd_rule", smbd_rule_attr },
    { "smbd_user_rule", smbd_user_rule_attr },
    { "togo_rule", togo_rule_attr },
    { "camera_rule", camera_rule_attr },
    { "drive_rule", drive_rule_attr },
    { "music_rule", music_rule_attr },
    { "led_rule", led_rule_attr },
    { "bt_rule", bt_rule_attr },
    { "ftpd_rule", ftpd_rule_attr },
    { "ftpd_user_rule", ftpd_user_rule_attr },
    { "itunes_rule", itunes_rule_attr},
    { "storage_rule", storage_rule_attr },
    { "storage_state_rule", storage_state_rule_attr },
    { "storage_used_state_rule", storage_used_state_rule_attr },
    { "myqnapcloud_id_rule", myqnapcloud_id_rule_attr },
    { "model_name", model_name_attr },
    { "is_rule", is_rule_attr },
    { "ishost_rule", ishost_rule_attr },
    { "isuser_rule", isuser_rule_attr },
    { "appmgr_rule", appmgr_rule_attr },
    { "http_rule_default", http_rule_attr },
    { "stats_rule", stats_rule_attr },
    { "vnc_rule", vnc_rule_attr },
    { "vnc_allow_rule", vnc_allow_rule_attr },
    { "pptpd_rule", pptpd_rule_attr },
    { "pptpd_user_rule", pptpd_user_rule_attr },
    { "wol_rule", wol_rule_attr },
    { "ipsec_rule", ipsec_rule_attr },
    { "ipsec_status_rule", ipsec_status_rule_attr },
    { "l2tpd_rule", l2tpd_rule_attr },
    { "l2tpd_user_rule", pptpd_user_rule_attr },
    { "auth_server_rule", auth_server_rule_attr },
    { "auth_user_rule", auth_user_rule_attr },
    { "fw_rule", fw_rule_attr },
    { "nat_pass_rule", nat_pass_rule_attr },
    { "wl_basic_rule", wl_basic_rule_attr },
    { "wl_advanced_rule", wl_advanced_rule_attr },
    { "wl_wds_rule", wl_wds_rule_attr },
    { "wl_wme_rule", wl_wme_rule_attr },
    { "wl_wps_sta_rule", wl_wps_sta_rule_attr },  
    { "wl_wps_rule", wl_wps_rule_attr },
    { "wl0_basic_rule", wlv_basic_rule_attr },
    { "wl0_ifname_rule", wlv_ifname_rule_attr },
    { "wl0_ssid_rule", wlv_ssid_rule_attr },
    { "wl0_sec_rule", wlv_sec_rule_attr },
    { "wl0_sec_wep_rule", wlv_sec_wep_rule_attr },
    { "wl0_sec_wpa_rule", wlv_sec_wpa_rule_attr },
    { "wl0_sec_wpa2_rule", wlv_sec_wpa2_rule_attr },
    { "wl00_acl_basic_rule", wlv_acl_basic_rule_attr },
    { "wl00_acl_rule", wlv_acl_rule_attr },
    { "wl01_acl_basic_rule", wlv_acl_basic_rule_attr },
    { "wl01_acl_rule", wlv_acl_rule_attr },
    { "wl02_acl_basic_rule", wlv_acl_basic_rule_attr },
    { "wl02_acl_rule", wlv_acl_rule_attr },
    { "wl03_acl_basic_rule", wlv_acl_basic_rule_attr },
    { "wl03_acl_rule", wlv_acl_rule_attr },
    { "wl1_basic_rule", wlv_basic_rule_attr },
    { "wl1_ifname_rule", wlv_ifname_rule_attr },
    { "wl1_ssid_rule", wlv_ssid_rule_attr },
    { "wl1_sec_rule", wlv_sec_rule_attr },
    { "wl1_sec_wep_rule", wlv_sec_wep_rule_attr },
    { "wl1_sec_wpa_rule", wlv_sec_wpa_rule_attr },
    { "wl1_sec_wpa2_rule", wlv_sec_wpa2_rule_attr },
    { "wl0_wds_basic_rule", wl_wds_basic_rule_attr },
    { "wl0_wds_sec_wep_rule", wl_wds_sec_wep_rule_attr },
    { "wl0_wds_sec_wpa_rule", wl_wds_sec_wpa_rule_attr },
    { "wl0_apcli_rule", wl_apcli_rule_attr },
    { "wl0_apcli_sec_wep_rule", wl_apcli_sec_wep_rule_attr },
    { "wl0_apcli_sec_wpa_rule", wl_apcli_sec_wpa_rule_attr },
    { "wl0_apcli_sec_wpa2_rule", wl_apcli_sec_wpa_rule_attr },
    { "wl0_apcli_swap_rule", wl_apcli_swap_rule_attr },
    { "wl0_apcli_swap_basic_rule", wl_apcli_swap_basic_rule_attr },
    { "wl0_apcli_swap_channel_rule", wl_apcli_swap_channel_rule_attr },
    { "wl0_apcli_swap_sec_wep_rule", wl_apcli_swap_sec_wep_rule_attr },
    { "wl0_apcli_swap_sec_wpa_rule", wl_apcli_swap_sec_wpa_rule_attr },
    { "wl0_apcli_swap_sec_wpa2_rule", wl_apcli_swap_sec_wpa_rule_attr },
    { "wl1_wds_basic_rule", wl_wds_basic_rule_attr },
    { "wl1_wds_sec_wep_rule", wl_wds_sec_wep_rule_attr },
    { "wl1_wds_sec_wpa_rule", wl_wds_sec_wpa_rule_attr },
    { "wl1_apcli_rule", wl_apcli_rule_attr },
    { "wl1_apcli_sec_wep_rule", wl_apcli_sec_wep_rule_attr },
    { "wl1_apcli_sec_wpa_rule", wl_apcli_sec_wpa_rule_attr },
    { "wl1_apcli_sec_wpa2_rule", wl_apcli_sec_wpa_rule_attr },
    { "x_mode_data", x_mode_data_attr },
    { "portcfg_rule", portcfg_rule_attr },
    { "nat_rule", nat_rule_attr},
    { "app_enable_rule", app_enable_rule_attr},
    { "mfg_test_result_rule", mfg_test_result_rule_attr},
    { "serial_num_rule", serial_num_rule_attr},
    { "captive_rule", captive_rule_attr},
    { "igmp_proxy_rule", igmp_proxy_rule_attr},
    { "ppp_control_rule", ppp_control_rule_attr},
    { "card_reader_hardware_rule", card_reader_hardware_rule_attr},
#if defined (PLATFORM_M2E)
    { "st_defpath_rule", st_defpath_rule_attr},
    { "gpiodef_rule", gpiodef_rule_attr},
    { "upnpc_setting_rule", upnpc_setting_rule_attr},
    { "upnpc_status_rule", upnpc_status_rule_attr},
#endif
#if defined (PLATFORM_GMA)
    { "wan_priority_rule", wan_priority_rule_attr},
    { "WISP_detect_portal_rule",WISP_dtct_captive_portal_rule_attr},
    { "gmn_nat_rule", gmn_nat_rule_attr},
    { "gmn_filter_rule", gmn_filter_rule_attr},
    { "gmn_mcast_rule", gmn_mcast_rule_attr},
#endif
    { NULL, NULL }
};

#define MAX_ATTR_NUM    32
static int
sep_string(char *word, const char *delim, char **idx_arr, int max_tok)
{
    char *str = word;
    char *ptr;
    int pos = 0;
    int i;

    for (i = 0; i < max_tok; i++) {
        idx_arr[i] = NULL;
    }

    while (str) {
        ptr = strsep(&str, delim);
        if (ptr && pos < max_tok) {
            idx_arr[pos++] = ptr;
        } else {
            break;
        }
    }
    return pos;
}

/**
 * \brief Get a specified rule set from nvram, parse it by '|' character, 
 * and copy the obtained rule into the given buffer.
 * \return Return the length of the rule if successful. Otherwise, a negative
 * indicates an error.
 * NOTE: If the return value is larger than or equal to bsize, it means the 
 * rule is truncated.
 * \param[in] rule_set: a point to the specific rule name. Eg., fr_rule
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[out] buf: a pointer to a buffer for copying the parsed data.
 * \param[in] bsize: to specify the size of the imported area.
 */
int
ezplib_get_rule(char *rule_set, int nth, char *buf, int bsize)
{
    char tmp[EZPLIB_BUF_LEN];
    char *next;
    char *wordlist;
    char *str, *ptr;
    int ret;

    assert(strlen(RULE_SEP) == 1);

    if (!buf) {
        return EZPLIB_INVALID;
    }

    /* Clean up the buffer that carries the retrieved value. */
    memset(buf, '\0', bsize);

    if (!rule_set || !*rule_set) {
        return EZPLIB_INVALID;
    }

    wordlist = nvram_get(rule_set);
    if (!wordlist) {
        return EZPLIB_NO_RULE_SET;
    }

    if (!*wordlist) {
        return EZPLIB_NO_RULE;
    }

    ret = snprintf(tmp, EZPLIB_BUF_LEN, "%s", wordlist);
    if (ret >= EZPLIB_BUF_LEN) {
        return EZPLIB_VAL_TRUNC;
    }

    str = tmp;
    while (str) {
        ptr = strsep(&str, RULE_SEP);
        if (!ptr) {
            return EZPLIB_NO_RULE;
        }

        if (nth != 0) {
            nth --;
            continue;
        }
        ret = snprintf(buf, bsize, ptr);
        if (ret >= bsize) {
            return EZPLIB_VAL_TRUNC;
        }
        return ret;
    }

    if (nth >= 0) {
        return EZPLIB_NO_RULE;
    }
    return 0;
}

/**
 * \brief Get a specified rule set from nvram, parse it by '|' character, 
 * and copy the obtained rule into the given buffer.
 * \return Return the length of the rule if successful. Otherwise, a negative
 * indicates an error.
 * NOTE: If the return value is larger than or equal to bsize, it means the 
 * rule is truncated.
 * \param[in] rule_set: a point to the specific rule name. Eg., fr_rule
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] start: to specify the position of the start attribute 
 *                   in the rule set.
 * \param[in] end: to specify the position of the end attribute 
 *                 in the rule set.
 * \param[out] buf: a pointer to a buffer for copying the parsed data.
 * \param[in] bsize: to specify the size of the imported area.
 */
int
ezplib_get_subrule(char *rule_set, int nth, int start, int end,
        char *buf, int bsize)
{
    char *ptr_array[MAX_ATTR_NUM];
    char word[EZPLIB_BUF_LEN];
    int ret, i, j, len;

    if (!rule_set || !*rule_set || !buf) {
        return EZPLIB_INVALID;
    }

    if (start > end) {
        return EZPLIB_INVALID;
    }

    /**
     * Clean up the buffer for carrying on the retrieved value, in case errors
     * happen while processing of attribute.
     */
    memset(buf, '\0', bsize);

    ret = ezplib_get_rule(rule_set, nth, word, EZPLIB_BUF_LEN);
    if (ret < 0) {
        return ret;
    }

    sep_string(word, ATTR_SEP, ptr_array, MAX_ATTR_NUM);
    for (i = j = 0; i < MAX_ATTR_NUM; i++) {
        if (!ptr_array[i]) {
            break;
        }
        if (start <= i && i <= end) {
            len = snprintf(buf + j, bsize - j, "%s%s", ptr_array[i], ATTR_SEP);
            j += len;
        }
    }

    if (j > 0 && buf[j-1]) {
        /* Remove the last ATTR_SEP. */
        buf[j-1] = '\0'; 
    }

    return 0;
}

/**
 * \brief Get a specified attribute of the given rule from the rule set in 
 * nvram. The rules are separated by RULE_SEP character and the attributes
 * in a rule are separated by ATTR_SEP character.
 * \return Return the length of the attribute if successful. Otherwise, a 
 * negative indicates an error.
 * \param[in] rule_set: a point to the specific rule name. Eg., fr_rule
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] type: a pointer to the specified type of attribute.
 * \param[out] buf: a pointer to a buffer for copying the parsed data.
 * \param[in] bsize: to specify the size of the imported area.
 */
int 
ezplib_get_attr_val(char *rule_set, int nth, char *type, char *buf, int bsize, 
                    int use)
{
    char *ptr_array[MAX_ATTR_NUM];
    char word[EZPLIB_BUF_LEN];
    int ret, i;


    if (!rule_set || !*rule_set || !type || !*type || !buf) {
        return EZPLIB_INVALID;
    }

    /**
     * Clean up the buffer for carrying on the retrieved value, in case errors
     * happen while processing of attribute.
     */
    memset(buf, '\0', bsize);

    ret = ezplib_get_rule(rule_set, nth, word, EZPLIB_BUF_LEN);
    if (ret < 0) {
        return ret;
    }

    sep_string(word, ATTR_SEP, ptr_array, MAX_ATTR_NUM);
    for (i = 0; rules[i].name; i++) {
        struct attr *attr;
        int j;
        if (strcmp(rules[i].name, rule_set) != 0) {
            continue;
        }
    
        attr = rules[i].attr;
        for (j = 0; attr[j].name && ptr_array[j]; j++) {
            if (strcmp(attr[j].name, type) == 0) {
                if (use == EZPLIB_USE_CLI && attr[j].func_orig) {
                    /* Don't wrap. Just get the value stored in nvram. */
                    return attr[j].func_orig(type, ptr_array[j], 
                                             buf, bsize);
                } else if (use == EZPLIB_USE_WEB && attr[j].func_wrap) {
                    /* Wrap the value from nvram. Eg., the attribute
                     * <enable> is 1 will be wrapped to 'checked'.
                     */
                    return attr[j].func_wrap(type, ptr_array[j], buf, 
                                             bsize);
                }
            }
        }
    }

    return EZPLIB_NO_ATTRIBUTE;
}

enum opcode {
    ADD_RULE = 0,
    DELETE_RULE = 1,
    REPLACE_RULE = 2
};

static int 
ezplib_op_rule(char *rule_set, enum opcode op, int nth, char *new_rule)
{
    char buf[EZPLIB_BUF_LEN];
    char word[EZPLIB_BUF_LEN];
    char *wordlist;
    char *ptr;
    int len, ret, i, rule_num;
    char *rule_sep = RULE_SEP;

    /* Get the number of rules in the rule set. */
    rule_num = ezplib_get_rule_num(rule_set);
    if (rule_num < 0) {
        return EZPLIB_NO_RULE_SET;
    }
    
    len = EZPLIB_BUF_LEN;
    ptr = buf;
    for (i = 0; i < rule_num; i++) {
        char *rule;
        ret = ezplib_get_rule(rule_set, i, word, EZPLIB_BUF_LEN);
        if (ret < 0) {
            return ret;
        }

        if (i != nth) {
            ret = snprintf(ptr, len, "%s%s", word, RULE_SEP);
            if (ret >= len) {
                return EZPLIB_VAL_TRUNC;
            }
        } else {
            switch (op) {
                case ADD_RULE:
                    ret = snprintf(ptr, len, "%s%s", new_rule, RULE_SEP);
                    if (ret >= len) {
                        return EZPLIB_VAL_TRUNC;
                    }

                    /**
                     * Disable nth so that the following flow will only go
                     * through the block of 'if (i != nth)'.
                     */
                    nth = -1;

                    /* Re-fetch the ith rule for the next round loop. */
                    i--;
                    break;
                case DELETE_RULE:
                    /* Do nothing but ignore on the old, fetched rule. */
                    continue;
                    break;
                case REPLACE_RULE:
                    ret = snprintf(ptr, len, "%s%s", new_rule, RULE_SEP);
                    if (ret >= len) {
                        return EZPLIB_VAL_TRUNC;
                    }
                    break;
                default:
                    return EZPLIB_INVALID;
                    break;
            }
        }

        ptr += ret;
        len -= ret;
    }

    /* Calculate the length of the rule set. */
    len = EZPLIB_BUF_LEN - len;

    /* Remove the last whitespace. */
    assert(strlen(RULE_SEP) == 1);
    if (buf[len - 1] == rule_sep[0]) {
        buf[--len] = '\0';
    }

    nvram_set(rule_set, buf);
    return len;
}

/**
 * \brief Replace the nth rule in the rule set.
 * parameter.
 * \return The total length of the rule set.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] new_rule: the replacing rule.
 */
int 
ezplib_replace_rule(char *rule_set, int nth, char *new_rule)
{
    int rule_num;

    if (!rule_set || !*rule_set || !new_rule) {
        return EZPLIB_INVALID;
    }

    rule_num = ezplib_get_rule_num(rule_set);
    if (rule_num < 0) {
        return EZPLIB_NO_RULE_SET;
    }

    if (nth > rule_num || nth < 0) {
        return EZPLIB_IDX_OUT_RANGE;
    }

    ezplib_op_rule(rule_set, REPLACE_RULE, nth, new_rule);
    return 0;
}

/**
 * \brief Replace the specific attribute in the nth rule in the rule set.
 * parameter.
 * \return The total length of the rule set.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] new_rule: the replacing rule.
 */
int 
ezplib_replace_attr(char *rule_set, int nth, char *attr_name, char *new_attr)
{
    char *ptr_array[MAX_ATTR_NUM];
    char word[EZPLIB_BUF_LEN];
    char tmp[EZPLIB_BUF_LEN];
    int ret, i, bytes, attr_len;
    char *val, *str, *sep = ATTR_SEP;

    if (!rule_set || !*rule_set || !attr_name || !*attr_name || !new_attr) {
        return EZPLIB_INVALID;
    }

    ret = ezplib_get_rule(rule_set, nth, word, EZPLIB_BUF_LEN);
    if (ret < 0) {
        return ret;
    }

    sep_string(word, ATTR_SEP, ptr_array, MAX_ATTR_NUM);
    for (i = 0; rules[i].name; i++) {
        struct attr *attr;
        int j;
        if (strcmp(rules[i].name, rule_set) != 0) {
            continue;
        }
    
        attr = rules[i].attr;
        bytes = EZPLIB_BUF_LEN;
        str = tmp;
        for (j = 0; attr[j].name && ptr_array[j]; j++) {
            if (strcmp(attr[j].name, attr_name) == 0) {
                val = new_attr;  
            } else {
                val = ptr_array[j];
            }

            /* include the separator */
            attr_len = strlen(val) + 1;
            ret = snprintf(str, bytes, "%s%s", val, sep);
            if (ret >= bytes) {
                return EZPLIB_VAL_TRUNC;
            }

            assert(ret == attr_len);

            bytes -= attr_len;
            str += attr_len;
        }

        /* remove the last separator */
        if (*(--str) == sep[0]) {
            *str = '\0';
        }

        return ezplib_replace_rule(rule_set, nth, tmp);
    }

    return EZPLIB_NO_ATTRIBUTE;
}

/**
 * \brief Delete the nth rule in the rule set.
 * parameter.
 * \return 0 for success; otherwise a negtive value would be returned.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 */
int 
ezplib_delete_rule(char *rule_set, int nth)
{
    if (!rule_set || !*rule_set) {
        return EZPLIB_INVALID;
    }

    ezplib_op_rule(rule_set, DELETE_RULE, nth, NULL);
    return 0;
}

/**
 * \brief Add a new rule into the nth position in the rule set.
 * parameter.
 * \return 0 for success; otherwise a negtive value would be returned.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] new_rule: the replacing rule.
 */
int 
ezplib_add_rule(char *rule_set, int nth, char *new_rule)
{
    int rule_num;

    if (!rule_set || !*rule_set || !new_rule) {
        return EZPLIB_INVALID;
    }

    rule_num = ezplib_get_rule_num(rule_set);
    if (rule_num < 0) {
        return EZPLIB_NO_RULE_SET;
    }

    if (nth > rule_num || nth < 0) {
        return EZPLIB_IDX_OUT_RANGE;
    }

    if (nth == rule_num) {
        ezplib_append_rule(rule_set, new_rule);
    } else {
        ezplib_op_rule(rule_set, ADD_RULE, nth, new_rule);
    }
    return 0;
}

/**
 * \brief Prepend a new rule into the head of the rule set.
 * parameter.
 * \return The total length of the rule set.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] new_rule: the replacing rule.
 */
int 
ezplib_prepend_rule(char *rule_set, char *new_rule)
{
    if (!rule_set || !*rule_set || !new_rule) {
        return EZPLIB_INVALID;
    }

    return ezplib_op_rule(rule_set, ADD_RULE, 0, new_rule);
}

/**
 * \brief append a new rule into the tail of the rule set.
 * parameter.
 * \return The total length of the rule set.
 * \param[in] rule_set: to specify the rule_set.
 * \param[in] nth: to specify the nth rule in the rule set.
 * \param[in] new_rule: the replacing rule.
 */
int 
ezplib_append_rule(char *rule_set, char *new_rule)
{
    char buf[EZPLIB_BUF_LEN];
    int len;
    char *old_rules;

    if (!rule_set || !*rule_set || !new_rule) {
        return EZPLIB_INVALID;
    }

    old_rules = nvram_get(rule_set);
    if (!old_rules) {
        return EZPLIB_NO_RULE_SET;
    }

    if (!*old_rules) {
        /* Empty rule set. */
        len = snprintf(buf, EZPLIB_BUF_LEN, "%s", new_rule);
    } else {
        /* Existing rule(s). */
        len = snprintf(buf, EZPLIB_BUF_LEN, "%s%s%s", old_rules, 
                       RULE_SEP, new_rule);
    }

    if (len >= EZPLIB_BUF_LEN) {
        return EZPLIB_VAL_TRUNC;
    }
    nvram_set(rule_set, buf);

    return len;
}

/**
 * \brief 
 * \return The number of rule in the rule set.
 * \param[in] rule_set: to specify the rule_set.
 */
int
ezplib_get_rule_num(char *rule_set)
{
    char *delim;
    char *sep = RULE_SEP;
    int i;
    int count;

    assert(strlen(RULE_SEP) == 1);

    rule_set = nvram_get(rule_set);
    if (!rule_set) {
        return EZPLIB_NO_RULE_SET;
    }

    /* Empty */
    if (!*rule_set) {
        return 0;
    }

    for (i =  0, count = 0; rule_set[i] != '\0'; i++) {
        if (rule_set[i] == sep[0]) {
           count++;
        }
    }

    /* The number of rules will one more than the count of the separators. */
    return count+1;
}

/* Return value : 
 * 0: rule is not changed
 * 1: rule is changed */
int 
ezplib_check_rule_changed(char *rule_set, int nth, char *new_rule) {
    char tmp[256];
    int len;

    if ((len = ezplib_get_rule(rule_set, nth, tmp, sizeof(tmp))) <= 0) {
        /* We find something wrong and just don't do any nvram change. */
        printf("Error : ezplib_get_rule() returns %d\n", len);
        return 0;
    }
    return strncmp(new_rule, tmp, len);
}

/* Return value : 
 * 0: rule is not changed
 * 1: rule is changed */
int 
ezplib_check_attr_changed(char *rule_set, int nth, char *attr, char *new_rule) {
    char tmp[256];
    int len;

    if ((len = ezplib_get_attr_val(rule_set, 
                    nth, attr, tmp, sizeof(tmp), EZPLIB_USE_CLI)) < 0) {
        /* We find something wrong and just don't do any nvram change. */
        printf("Error : ezplib_get_attr_val() returns %d\n", len);
        return 0;
    }
    if (strlen(new_rule) != len) {
        /* Length changed */
        return 1;
    }
    return strncmp(new_rule, tmp, len);
}

int
ezplib_find_attr_val(char *rule_set, char *find_attr, char *find_val,
        char *attr, char *buf, int bsize)
{
    int i;
    char tmp[256];
    for (i = 0; i < ezplib_get_rule_num(rule_set); i++) {
        ezplib_get_attr_val(rule_set, i , find_attr, tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        if(strstr(tmp, find_val)) {
            ezplib_get_attr_val(rule_set, i, attr, buf, bsize,
                    EZPLIB_USE_CLI);
            return 0;
        }
    }
    return 1;
}

