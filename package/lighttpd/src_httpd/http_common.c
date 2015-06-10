#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typedefs.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>

#include "ezp.h"
#include "ezp-lib.h"
#include "http_common.h"
#include "fcgi_common.h"

int
is_digit_str(char *str)
{
    int i;

    for (i = 0; *(str + i); i++) {
        if (!isdigit(*(str + i))) {
            return FALSE;
        }
    }
    return TRUE;
}

int
is_ascii_str(char *str)
{
    int i;

    for (i = 0; *(str + i); i++) {
        if (!isascii(*(str + i))) {
            return FALSE;
        }
    }
    return TRUE;
}

int
is_legal_hwaddr(char *value)
{
    unsigned int hwaddr[6];
    int tag = TRUE;
    int i, count;

    /* Check for bad, multicast, broadcast, or null address */
    for (i = 0, count = 0; *(value + i); i++) {
        if (*(value + i) == ':') {
            if (i % 3 != 2) {
                tag = FALSE;
                break;
            }
            count++;
        } else if (isxdigit(*(value + i))) /* one of 0 - 9, a - f, A - F */
            continue;
        else {
            tag = FALSE;
            break;
        }
    }

    /* must have 17's characters and 5's ':' */
    if (!tag || i != 17 || count != 5)  
        tag = FALSE;
    else if (sscanf(value, "%x:%x:%x:%x:%x:%x",
                    &hwaddr[0], &hwaddr[1], &hwaddr[2],
                    &hwaddr[3], &hwaddr[4], &hwaddr[5]) != 6) {
        tag = FALSE;
    } else {
        tag = TRUE;
    }


    return tag;
}

/* 
 * XXX: Don't understand what this code does :(
 */

int
is_legal_netmask(char *value)
{
    struct in_addr netmask;
    unsigned long mask;
    int i, maskbits;

    if (!inet_aton(value,&netmask))  {
        return FALSE;
    }

    mask = ntohl(netmask.s_addr);
    for ( maskbits=32 ; (mask & (1L<<(32-maskbits))) == 0 ; maskbits-- )
        ;
    /* Re-create the netmask and compare to the origianl
     * to make sure it is a valid netmask.
     */
    mask = 0;
    for ( i=0 ; i<maskbits ; i++ )
        mask |= 1<<(31-i);
    if ( mask != ntohl(netmask.s_addr) ) {
        return FALSE;
    }
    return TRUE;
}

int
trans_netmask2int(char *value)
{
    struct in_addr netmask;
    unsigned long mask;
    int maskbits;

    if (!inet_aton(value,&netmask))  {
        return -1;
    }
    mask = ntohl(netmask.s_addr);
    for ( maskbits=32 ; (mask & (1L<<(32-maskbits))) == 0 ; maskbits-- );
    return maskbits;
}

int
trans_int2netmask(int maskbits, char *mask_buf, int buf_len)
{
    struct in_addr netmask;
    unsigned long mask;
    int i;
    if ( maskbits < 1 || maskbits > 32 ) {
        return -1;
    }
    /* Create the netmask from the number of bits */
    mask = 0;
    for ( i=0 ; i<maskbits ; i++ )
        mask |= 1<<(31-i);
    netmask.s_addr = htonl(mask);
    snprintf(mask_buf, buf_len, "%s", inet_ntoa(netmask));
    return 0;
}

/* 
 * Example:
 * legal_ip_netmask("192.168.1.1","255.255.255.0","192.168.1.100"); return true;
 * legal_ip_netmask("192.168.1.1","255.255.255.0","192.168.2.100"); return false;
 */
int
is_legal_ip_netmask(char *sip, char *smask, char *dip)
{
    struct in_addr ipaddr, netaddr, netmask;

    if (!inet_aton(sip, &netaddr))
        return FALSE;
    if (!inet_aton(smask, &netmask))
        return FALSE;
    if (!inet_aton(dip, &ipaddr))
        return FALSE;

    netaddr.s_addr &= netmask.s_addr;

    if (netaddr.s_addr != (ipaddr.s_addr & netmask.s_addr)) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/* 
 * Example:
 * legal_ipaddr("192.168.1.1"); return true;
 * legal_ipaddr("192.168.1.1111"); return false;
 */
int
is_legal_ipaddr(char *value)
{
    struct in_addr ipaddr;

    /* Use inet_aton() to verify the IP address format. */
    if (!inet_aton(value, &ipaddr)) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/* Example:
 * lan0_ipaddr = 192.168.1.1
 * show_ipaddr_single("lan0_ipaddr", 1); produces "168"
 */
int
show_ipaddr_single(char *ipaddr, int which)
{
    int ip[4] = { 0, 0, 0, 0 };

    /* XXX: Check the return value. */
    sscanf(ipaddr, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);

    return ip[which];
}

/* Example:
 * show_ipaddr_prefix24("192.168.1.1"); produces "192.168.1."
 */
int
show_ipaddr_prefix24(char *ipaddr, char *buf, int buf_len)
{
    int ip[4];

    if (sscanf(ipaddr, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]) != 4)
        return 1;

    /* XXX: Check the return value. */
    snprintf(buf, buf_len , "%d.%d.%d.", ip[0], ip[1], ip[2]);

    return 0;
}

/* Example:
 * show_ipaddr_prefix24_calc("192.168.1.1","3","5"); produces "192.168.1.7"
 */
int
show_ipaddr_prefix24_calc(char *ipaddr, int start, int num,
        char *buf, int buf_len)
{
    int ip[4];

    if (sscanf(ipaddr, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]) < 3)
        return 1;

    /* XXX: Check the return value. */
    snprintf(buf, buf_len , "%d.%d.%d.%d", ip[0], ip[1], ip[2], 
            start + num - 1);

    return 0;
}


int
show_localtime(char *buf, int buf_len)
{
#ifdef EZP_PROD_BRAND_PROX
    /* char tzarray[13][3] = {"00", "01", "02", "03", "04", "05", "06", \ */
    /* "07", "08", "09", "10", "11", "12"}; */
    /* char tzmin[][3] = {"00", "30"}; */
    long dtime = 0;
    char *strtm, *strzone;
    time_t tm1, tm2;
    struct tm *t1, *t2;
#else    
    struct tm tm;
    time_t timep;
#endif



#ifdef EZP_PROD_BRAND_PROX
    strtm = malloc(120 * sizeof(char));
    strzone = malloc(50 * sizeof(char));
    tm1 = time(NULL);
    t2 = gmtime(&tm1);
    tm2 = mktime(t2);
    t1 = localtime(&tm1);
    dtime = (long)(tm1 - tm2);

    /* Print local time as a string */
#if 0
    strftime(strtm, 100, "%a %b %d %Y %H:%M:%S GMT", t1);
    if (dtime >= 0 )    
        strcat(strtm, "+");
    else 
        strcat(strtm, "-");
    strcat(strtm, tzarray[(short)(abs(dtime) / 3600)]);
    if (1800 == (short)(abs(dtime) % 3600))
        strcat(strtm, tzmin[1]);
    else
        strcat(strtm, tzmin[0]);
#else
    strftime(strtm, 100, "%a %b %d %Y %H:%M:%S %Z", t1);
#endif    
    snprintf(buf, buf_len - 1, "%s", strtm);
    free(strtm);
    free(strzone);
#else    
    timep = time(NULL);
    memcpy(&tm, localtime(&timep), sizeof(struct tm));
    strftime(buf, buf_len - 1, "%a, %d %b %Y %H:%M:%S", &tm);
#endif    

    return 0;
}

int read_file(const char *filename, char *buffer, size_t size)
{
    FILE *fp;
    size_t rv;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        //lprintf(LOG_WARNING, "could not open %s: %m\n", filename);
        return -1;
    }
    rv = fread(buffer, 1, size - 1, fp);
    if (rv == 0) {
        //lprintf(LOG_WARNING, "could not read %s: %m\n", filename);
        fclose(fp);
        return -1;
    }
    buffer[rv] = '\0';
    if (fclose(fp) == -1) {
        //lprintf(LOG_WARNING, "could not close %s: %m\n", filename);
    }
    return 0;
}

unsigned long read_system_uptime(void)
{
    /* We need the uptime in 1/100 seconds, so we can't use sysinfo() */
    char buffer[128];

    if (read_file("/proc/uptime", buffer, sizeof (buffer)) != -1) {
        return (unsigned long)(atof(buffer) * 100);
    } else {
        return -1;
    }
}

int cal_ifuptime(char* ptr, int length, unsigned long if_start_time)
{
    int n, updays, uphours, upminutes;
    unsigned long ifuptime;

    if( if_start_time == 0 ) {
        snprintf(ptr, length, "0");
        return 1;
    }

    ifuptime=(unsigned long)((read_system_uptime()-if_start_time)/100);

    updays = (int)(ifuptime / (60*60*24));

    if (updays) {
        n = snprintf(ptr, length, "%d day%s, ", updays, 
                (updays != 1) ? "s" : "");
        if (n <= 0)
            return 1;
        ptr += n;
        length -=n;
    }
    upminutes = (int) ifuptime / 60;
    uphours = (upminutes / 60) % 24;
    upminutes %= 60;
    if(uphours) {
        n = snprintf(ptr, length, "%2d hour%s, ", uphours,
                (uphours > 1) ? "s" : "");
        if (n <= 0)
            return 1;
        ptr += n;
        length -=n;
    } 

    n = snprintf(ptr, length, "%2d min%s", upminutes, 
            (upminutes > 1) ? "s" : "");
    if (n <= 0)
        return 1;
    ptr += n;
    length -=n;

    return 0;
}

int 
show_uptime(char *ptr, int length)
{
    int n, updays, uphours, upminutes;
    struct sysinfo info;
    sysinfo(&info);
    updays = (int) (info.uptime / (60*60*24));

    if (updays) {
        n = snprintf(ptr, length, "%d day%s, ", updays, 
                                    (updays != 1) ? "s" : "");
        if (n <= 0)
            return 1;
        ptr += n;
        length -=n;
    }
    upminutes = (int) info.uptime / 60;
    uphours = (upminutes / 60) % 24;
    upminutes %= 60;
    if(uphours) {
        n = snprintf(ptr, length, "%2d hour%s, ", uphours,
                (uphours > 1) ? "s" : "");
        if (n <= 0)
            return 1;
        ptr += n;
        length -=n;
    } 

    n = snprintf(ptr, length, "%2d min%s", upminutes, 
            (upminutes > 1) ? "s" : "");
    if (n <= 0)
        return 1;
    ptr += n;
    length -=n;

    return 0;
}

int 
show_ipaddr(char *ifname, char *ptr, int length)
{
    int fd = -1, n;
    unsigned char *addr;
    struct ifreq ifr;

    if ((fd = socket (AF_INET, SOCK_DGRAM, IPPROTO_IP)) <= -1)
        goto err;

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy (ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0)
        goto err;

    addr=(unsigned char *)&(((struct sockaddr_in * )&ifr.ifr_addr)->sin_addr);
    n = snprintf(ptr, length, "%d.%d.%d.%d", addr[0],addr[1],addr[2],addr[3]);
    if (n <= 0)
        goto err;

    close(fd);
    return 0;

err:
    if (fd > -1)
        close(fd);
    return 1;

}

/* Example:
 * wan_mac = 00:11:22:33:44:55
 * show_hwaddr_single("wan_mac", 1); produces "11"
 */
int
show_hwaddr_single(char *hwaddr, int which)
{
    int mac[6] = { 0, 0, 0, 0, 0, 0 };
    int ret;

    /* XXX: Check the return value. */
    ret = sscanf(hwaddr, "%2X:%2X:%2X:%2X:%2X:%2X", &mac[0], &mac[1],
               &mac[2], &mac[3], &mac[4], &mac[5]);
    return mac[which];
}


int 
show_hwaddr(char *ifname, char *ptr, int length)
{
    int fd = -1, n;
    unsigned char *tmp;
    struct ifreq ifr;

    if ((fd = socket (AF_INET, SOCK_DGRAM, IPPROTO_IP)) <= -1)
        goto err;

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy (ifr.ifr_name, ifname, IFNAMSIZ);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0)
        goto err;

    tmp = (unsigned char *) &ifr.ifr_hwaddr.sa_data[0];

    n = snprintf(ptr, length, "%02X:%02X:%02X:%02X:%02X:%02X", 
            tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
    if (n <= 0)
        goto err;

    close(fd);
    return 0;

err:
    if (fd > -1)
        close(fd);
    return 1;

}

/* Find out the physical interface by iface name. */
char *
show_iface_dev(char *iface)
{
    char *ifname;
    char tmp[SHORT_BUF_LEN];

    snprintf(tmp, SHORT_BUF_LEN, "%s_ifname", iface);
    ifname = nvram_safe_get(tmp);

    /* PPPoE and PPTP */
    if (!strncmp(ifname, "pptp", 4) || !strncmp(ifname, "ppp", 3)) {
        snprintf(tmp, SHORT_BUF_LEN, "%s_device", iface);
        ifname = nvram_safe_get(tmp);
    }

    return ifname;
}

/* Format:
 * type = SET :  " " => "&nbsp;" , ":" => "&semi;"
 * type = GET :  "&nbsp;" => " " , "&semi;" => ":"
 * Example:
 * name1 = test 123:abc
 * nbsp_translate("name1", new_name, SET); new_name="test&nbsp;123&semi;abc"
 * name2 = test&nbsp;123&semi;abc
 * nbsp_translate("name2", new_name, GET); new_name="test 123:abc"
 */
int
nbsp_translate(char *old_name, char *new_name, size_t size, int type)
{
    int i, j, match;

    struct pattern
    {
        char ch;
        char *string;
    };

    struct pattern patterns[] = {
        {' ', "&nbsp;"},
        {':', "&semi;"},
    };

    struct pattern *v;

    strcpy(new_name, "");

    switch (type) {
    case SET:
        for (i = 0; *(old_name + i); i++) {
            match = 0;
            for (v = patterns; v < &patterns[STRUCT_LEN(patterns)]; v++) {
                if (*(old_name + i) == v->ch) {
                    if (strlen(new_name) + strlen(v->string) > size) {  // avoid overflow
                        cprintf("%s(): overflow\n", __FUNCTION__);
                        new_name[strlen(new_name)] = '\0';
                        return 1;
                    }
                    sprintf(new_name + strlen(new_name), "%s", v->string);
                    match = 1;
                    break;
                }
            }
            if (!match) {
                if (strlen(new_name) + 1 > size) {
                    cprintf("%s(): overflow\n", __FUNCTION__);  // avoid overflow
                    new_name[strlen(new_name)] = '\0';
                    return 1;
                }
                sprintf(new_name + strlen(new_name), "%c",
                        *(old_name + i));
            }
        }

        break;
    case GET:
        for (i = 0, j = 0; *(old_name + j); j++) {
            match = 0;
            for (v = patterns; v < &patterns[STRUCT_LEN(patterns)]; v++) {
                if (!memcmp(old_name + j, v->string, strlen(v->string))) {
                    *(new_name + i) = v->ch;
                    j = j + strlen(v->string) - 1;
                    match = 1;
                    break;
                }
            }
            if (!match)
                *(new_name + i) = *(old_name + j);

            i++;
        }
        *(new_name + i) = '\0';
        break;
    default:
        cprintf("%s():Invalid type!\n", __FUNCTION__);
        break;
    }

    return 1;
}

int
protocol_to_num(char *proto)
{
    if (!strcmp(proto, "icmp"))
        return 1;
    else if (!strcmp(proto, "tcp"))
        return 6;
    else if (!strcmp(proto, "udp"))
        return 17;
    else if (!strcmp(proto, "both"))
        return 23;
    else
        return 0;
}

char *
num_to_protocol(int num)
{
    switch (num) {
    case 1:
        return "icmp";
    case 6:
        return "tcp";
    case 17:
        return "udp";
    case 23:
        return "both";
    default:
        return "unknown";
    }
}

struct mtu_lists mtu_list[] = {
#if COUNTRY == JAPAN
    {"pppoe", "576", "1454"},
#else
    {"pppoe", "576", "1492"},
#endif

    {"pptp", "576", "1460"},

    {"l2tp", "576", "1460"},

    {"dhcp", "576", "1500"},
    {"static", "576", "1500"},
    {"heartbeat", "576", "1500"},
    {"default", "576", "1500"}, // The value must be at last
};

struct mtu_lists *
get_mtu(char *proto)
{
    struct mtu_lists *v = NULL;

    for (v = mtu_list; v < &mtu_list[STRUCT_LEN(mtu_list)]; v++) {
        if (!strcmp(proto, v->proto))
            return v;
    }
    return v;                   // Use default settings
}

void
encode(char *buf, int len)
{
    int i;
    char ch;

    for (i = 0; i < len; i++) {
        ch = (buf[i] & 0x03) << 6;
        buf[i] = (buf[i] >> 2);
        buf[i] &= 0x3f;
        buf[i] |= ch;
        buf[i] = ~buf[i];
    }
}

void
decode(char *buf, int len)
{
    int i;
    char ch;

    for (i = 0; i < len; i++) {
        ch = (buf[i] & 0xC0) >> 6;
        buf[i] = (buf[i] << 2) | ch;
        buf[i] = ~buf[i];
    }
}

char *
get_hwaddr_by_ipaddr(char *ip)
{
    FILE *fp;
    char line[100];

    char ipa[50];               // ip address
    char hwa[50];               // HW address / MAC
    char mask[50];              // ntemask   
    char dev[50];               // interface
    int type;                   // HW type
    int flags;                  // flags
    static char mac[20];


    if ((fp = fopen("/proc/net/arp", "r")) == NULL)
        return NULL;

    // Bypass header -- read until newline 
    if (fgets(line, sizeof(line), fp) != (char *) NULL) {
        // Read the ARP cache entries.
        // IP address       HW type     Flags       HW address            Mask     Device
        // 192.168.1.1      0x1         0x2         00:90:4C:21:00:2A     *        eth0
        for (; fgets(line, sizeof(line), fp);) {
            if (sscanf
                (line, "%s 0x%x 0x%x %100s %100s %100s\n", ipa, &type,
                 &flags, hwa, mask, dev) != 6)
                continue;
            //cprintf("ip1=[%s] ip2=[%s] mac=[%s] (flags & ATF_COM)=%d\n", ip, ipa, hwa, (flags & ATF_COM));
            if (strcmp(ip, ipa))
                continue;
            //if (!(flags & ATF_COM)) {       //ATF_COM = 0x02   completed entry (ha valid)
            strcpy(mac, hwa);
            fclose(fp);
            return mac;
            //}
        }
    }

    fclose(fp);
    return "";
}

/* It is abstracted from ej_nvg_attr_match() because this function is used by
 * bw.c as well. NOTE: if it doesn't match, NULL returns.
 */
char *
nvg_attr_match(INPUT *input, char *tag, char *rule_set, int rule_num, char *attr, char *match,
        char *output)
{
    char *val;
    char buf[TMP_LEN];
    int ret;

    val = GOZILA_GET(tag);
    if (!val) {
        ret = 
            ezplib_get_attr_val(rule_set, rule_num, attr, buf, sizeof(buf), 
                    EZPLIB_USE_CLI);
        if (ret < 0) {
            return NULL;
        }
        val = buf;
    }

    if (val && strcmp(val, match) == 0) {
        return output;
    } 
    return NULL;
}
