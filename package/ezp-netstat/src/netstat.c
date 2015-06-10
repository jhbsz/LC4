#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <getopt.h>
#include <errno.h>
#include <ctype.h>
#include "bcmnvram.h"
#include "ezp-lib.h"


#define MARK_COUNT 64
#define APPMARK_FILE "/tmp/appmark"
#define APPFLOW_FILE "/tmp/appmarkflow"
#define UPLOAD_TRAFFIC_FILE "/tmp/uprate"
#define DOWNLOAD_TRAFFIC_FILE "/tmp/downrate"

#ifdef DEBUG
# define EZP_BWM_DBG(x...) do{ if (!silent_output) printf( x ); }while(0)
#else
# define EZP_BWM_DBG(x...) {}
#endif /*DEBUG*/

static char foreground = 0;
static char silent_output = 0;
static char monitor_bw = 1;
static char monitor_conn = 0;
static int interval = 5;
#ifndef DISABLE_WIFI_MONITOR
static char monitor_wifi = 1;
#endif
static char buf[254];
static char *markp[MARK_COUNT];
static int markcnt = 0;
#define DEFAULT_BW_LIMIT_LOW    3500
#define DEFAULT_BW_LIMIT_MED    5000
#define DEFAULT_BW_LIMIT_HIGH    6500

static long bw_limit_low = DEFAULT_BW_LIMIT_LOW, 
            bw_limit_med = DEFAULT_BW_LIMIT_MED, 
            bw_limit_high = DEFAULT_BW_LIMIT_HIGH;
struct flowmonitor {
    long int usent;
    long int dsent;
    long int urate;
    long int drate;
} *flowp;

static int daemonize(void) {
    pid_t pid, sid;

    // Put igd in the background as a daemon process.
    pid = fork();
    if (pid < 0)
    { perror("Error forking a new process.");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // become session leader
    if ((sid = setsid()) < 0)
    {
        perror("Error running setsid");
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0)
    {
        perror("Error setting root directory");
        exit(EXIT_FAILURE);
    }

    umask(0);

    /* Redirect standard files to /dev/null */
    if (silent_output) {
        freopen( "/dev/null", "r", stdin);
        freopen( "/dev/null", "w", stdout);
        freopen( "/dev/null", "w", stderr);
    }
}

static int getmarktable(void) {
    FILE *mfd;
    char *tokptr;

    if (!(mfd = fopen(APPMARK_FILE, "r"))) {
        return -1;
    }
    while (fgets(buf, sizeof(buf), mfd)) {
        /* Format like bt|bt|tcp|1023 , so we ignore first 3 '|'s */
        tokptr = strrchr(buf, '|') + 1;
        strtok(tokptr, "\r\n");
        markp[markcnt] = strdup(tokptr);
        EZP_BWM_DBG("Get mark:%s\n", 
                markp[markcnt]);
        markcnt ++;
        if (markcnt == MARK_COUNT) {
            break;
        }
    }
    fclose(mfd);
    flowp = (struct flowmonitor *) malloc (sizeof(struct flowmonitor) * markcnt);
    return 0;
}

static void freemarktable(void) {
    int i;
    if (!markcnt) {
        for (i = 0;i < markcnt;i++) {
            free(markp[i]);
            markp[i] = NULL;
        }
        free(flowp);
    }
    markcnt = 0;
}

static int filtmarktable(void) {
    FILE *mfd, *urfd, *drfd, *ffd;
    char *tokptr, ratebuf[30], *pktcnt_endp = NULL;
    int i, proccnt = 0, ret = 0;

    if (!(drfd = fopen(DOWNLOAD_TRAFFIC_FILE, "r"))) {
        return -1;
    }
    if (!(urfd = fopen(UPLOAD_TRAFFIC_FILE, "r"))) {
        ret = -1;
        goto close_download_fd;
    }
    if (!(mfd = fopen(APPMARK_FILE, "r"))) {
        ret = -1;
        goto close_upload_fd;
    }
    if (!(ffd = fopen(APPFLOW_FILE, "w"))) {
        ret = -1;
        goto close_mark_fd;
    }
    proccnt = 0;
    while (fgets(buf, sizeof(buf), urfd)) {
        for (i = 0;i < markcnt;i++) {
            snprintf(ratebuf, sizeof(ratebuf), "class htb 1:%s", markp[i]);
            if (strstr(buf, ratebuf)) {
                /* Get upload sent byte count */
                /* Format : Sent 100 bytes 0 pkt (dropped 0,... */
                fgets(buf, sizeof(buf), urfd);
                tokptr = strstr(buf, "Sent") + sizeof("Sent");
                EZP_BWM_DBG("Get upload i:%d mark:%s tokptr[0][1][2][3]:%c%c%c%c tokptr:%s ",i,markp[i], tokptr[0], tokptr[1],tokptr[2],tokptr[3], tokptr);
                strtok(tokptr, " ");
                pktcnt_endp = NULL;
                flowp[i].usent = strtol(tokptr, &pktcnt_endp, 10);
                /* In some case of 1000Kbits/sec, we need to get 1000 with
                 * multiplier 1K:1024 */
                if (*pktcnt_endp == 'K' || *pktcnt_endp == 'k') {
                    flowp[i].usent *= 1024;
                }
                EZP_BWM_DBG("Get upload i:%d flowp[i].usent:%d\n",i,flowp[i].usent);
                /* Get upload rate */
                /* Format : rate 0bit 0pps backlog 0b 0p ... */
                fgets(buf, sizeof(buf), urfd);
                tokptr = strstr(buf, "rate") + sizeof("rate");
                EZP_BWM_DBG("Get upload i:%d tokptr[0][1][2][3]:%c%c%c%c tokptr:%s ",i,tokptr[0], tokptr[1],tokptr[2],tokptr[3],tokptr);
                strtok(tokptr,"b");
                pktcnt_endp = NULL;
                flowp[i].urate =  strtol(tokptr, &pktcnt_endp, 10);
                /* In some case of 1000Kbits/sec, we need to get 1000 with
                 * multiplier 1K:1024 */
                if (*pktcnt_endp == 'K' || *pktcnt_endp == 'k') {
                    flowp[i].urate *= 1024;
                }
                EZP_BWM_DBG("Get upload i:%d flowp[i].urate:%d\n",i,flowp[i].urate);
                proccnt ++;
                break;
            }
        }
        if (proccnt == markcnt) {
            /* We processed all mark entries */
            break;
        }
    }
    if (proccnt != markcnt) {
        /* Some thing wrong */
        perror("upload rate monitor incomplete markcnt proccnt\n");
    }
    proccnt = 0;
    while (fgets(buf, sizeof(buf), drfd)) {
        for (i = 0;i < markcnt;i++) {
            snprintf(ratebuf, sizeof(ratebuf), "class htb 1:%s", markp[i]);
            if (strstr(buf, ratebuf)) {
                /* Get upload sent byte count */
                /* Format : Sent 100 bytes 0 pkt (dropped 0,... */
                fgets(buf, sizeof(buf), drfd);
                tokptr = strstr(buf, "Sent") + sizeof("Sent");
                EZP_BWM_DBG("Get download i:%d mark:%s tokptr[0][1][2][3]:%c%c%c%c tokptr:%s ",i,markp[i], tokptr[0], tokptr[1],tokptr[2],tokptr[3], tokptr);
                strtok(tokptr, " ");
                pktcnt_endp = NULL;
                flowp[i].dsent = strtol(tokptr, &pktcnt_endp, 10);
                /* In some case of 1000Kbits/sec, we need to get 1000 with
                 * multiplier 1K:1024 */
                if (*pktcnt_endp == 'K' || *pktcnt_endp == 'k') {
                    flowp[i].dsent *= 1024;
                }
                EZP_BWM_DBG("Get download i:%d flowp[i].dsent:%d\n",i,flowp[i].dsent);
                /* Get upload rate */
                /* Format : rate 0bit 0pps backlog 0b 0p ... */
                fgets(buf, sizeof(buf), drfd);
                tokptr = strstr(buf, "rate") + sizeof("rate");
                EZP_BWM_DBG("Get download i:%d tokptr[0][1][2][3]:%c%c%c%c tokptr:%s ",i,tokptr[0], tokptr[1],tokptr[2],tokptr[3], tokptr);
                strtok(tokptr,"b");
                pktcnt_endp = NULL;
                flowp[i].drate =  strtol(tokptr, &pktcnt_endp, 10);
                /* In some case of 1000Kbits/sec, we need to get 1000 with
                 * multiplier 1K:1024 */
                if (*pktcnt_endp == 'K' || *pktcnt_endp == 'k') {
                    flowp[i].drate *= 1024;
                }
                EZP_BWM_DBG("Get download i:%d flowp[i].drate:%d\n",i,flowp[i].drate);
                proccnt ++;
                break;
            }
        }
        if (proccnt == markcnt) {
            /* We processed all mark entries */
            break;
        }
    }
    if (proccnt != markcnt) {
        /* Some thing wrong */
        perror("download rate monitor incomplete markcnt proccnt");
    }
    i = 0;
    while (fgets(buf, sizeof(buf), mfd)) {
        /* Set occurance of "\r" or "\n" to 0 */
        tokptr = strtok(buf,"\n\r");
        fprintf(ffd, "%s|%d|%d|%d|%d\n", buf, flowp[i].usent, flowp[i].urate, flowp[i].dsent, flowp[i].drate);
        EZP_BWM_DBG("Output to file /tmp/appmarkflow %s|%d|%d|%d|%d\n", 
                buf, flowp[i].usent, flowp[i].urate, flowp[i].dsent, flowp[i].drate);
        i++;
        if (i == markcnt) {
            break;
        }
    }
close_flow_fd:
    fclose(ffd);
close_mark_fd:
    fclose(mfd);
close_upload_fd:
    fclose(urfd);
close_download_fd:
    fclose(drfd);
    return ret;
}

static int monitor_conn_func(int stat) {
    FILE *procfd;
    char buf[64];
    long int freemem = 0;
    int new_sess_num = 12000;
    procfd = popen("free | grep Mem | awk '{print $4}'", "r");
    if (procfd) {
        fgets(buf, sizeof(buf), procfd);
        EZP_BWM_DBG("read free memory %s",buf);
        pclose(procfd);
        procfd = NULL;
        freemem = strtol(buf, NULL, 10);
        EZP_BWM_DBG("converted free memory %d\n",freemem);
    } else {
        return stat;
    }
    procfd = fopen("/proc/sys/net/netfilter/nf_conntrack_count", "r");
    if (procfd) {
        fgets(buf, sizeof(buf), procfd);
        new_sess_num = atoi(buf);
        fclose(procfd);
        procfd = NULL;
    }
    if ((freemem < bw_limit_low) || ((stat == 1) && (freemem < (bw_limit_low + 500)))) { 
        /* Debouunce login: when we are in state 1 and free memory is at , for
         * example 3100, we don't go to other state to prevent bounce. */
        /* Very close to crash, we have to stop accepting any connection
         * request */
        if (stat != 1) {
            EZP_BWM_DBG("session %d\n",new_sess_num);
            doSystem("echo %d > /proc/sys/net/netfilter/nf_conntrack_max",
                    new_sess_num);
            system("[ -n \"$(pidof wscd)\" ] && killall wscd");
            system("[ -n \"$(pidof lld2d)\" ] && killall lld2d");
            system("/sbin/sysctl -p /etc/session_manager/sm_fast");
            stat = 1;
        }
    }
    else if ((freemem < bw_limit_med) || ((stat == 2) && (freemem < (bw_limit_med + 500)))) {
        /* Debouunce login: when we are in state 1 and free memory is at , for
         * example 4700, we don't go to other state to prevent bounce. */
        if (stat != 2) {
            new_sess_num = (((new_sess_num + 1000) < 12000) ? new_sess_num + 1000 : 12000);
            EZP_BWM_DBG("session %d\n", new_sess_num);
            doSystem("echo %d > /proc/sys/net/netfilter/nf_conntrack_max",
                    new_sess_num);
            system("[ -n \"$(pidof wscd)\" ] && killall wscd");
            system("[ -n \"$(pidof lld2d)\" ] && killall lld2d");
            system("/sbin/sysctl -p /etc/session_manager/sm_fast");
            stat = 2;
        }
    }
    else if ((freemem < bw_limit_high) || (((stat == 3) && (freemem < (bw_limit_high + 500))))) {
        /* Debouunce login: when we are in state 1 and free memory is at , for
         * example 6200, we don't go to other state to prevent bounce. */
        if (stat != 3) {
            new_sess_num = (((new_sess_num + 3000) < 13000) ? new_sess_num + 3000 : 13000);
            EZP_BWM_DBG("session %d\n", new_sess_num);
            doSystem("echo %d > /proc/sys/net/netfilter/nf_conntrack_max",
                    new_sess_num);
            system("[ -n \"$(pidof wscd)\" ] && killall wscd");
            system("[ -z \"$(pidof lld2d)\" ] && sh /etc/rc.common /etc/init.d/lldt start");
            system("/sbin/sysctl -p /etc/session_manager/sm_fast");
            stat = 3;
        }
    }
    else {
        if (stat != 0) {
            EZP_BWM_DBG("session %d\n",16000);
            system("echo $(nvram get ct_max) > /proc/sys/net/netfilter/nf_conntrack_max");
            system("[ -z \"$(pidof wscd)\" ] && sh /etc/rc.common /etc/init.d/wps start");
            system("[ -z \"$(pidof lld2d)\" ] && sh /etc/rc.common /etc/init.d/lldt start");
            system("[ -z \"$(pidof goahead)\" ] && sh /etc/rc.common /etc/init.d/goahead start");
            system("/sbin/sysctl -p /etc/session_manager/sm_slow");
            stat = 0;
        }
    }
    return stat;
}
#define NET_DEVICE_PROC        "/proc/net/dev"
#define GPIOCTL_BIN             "/usr/bin/gpioctl"
#define MONITOR_WIFI_DEVICE     "ra0"
#define MONITOR_MOBILE_DEVICE   "ppp1"
#define WIFI_OVERRIDE_GPIO      14 /* Led gpio number */
#define MOBILE_OVERRIDE_GPIO    13 /* Led gpio number */
#define GPIO_ON                 5 /* Led up time, in 1/10 second */
#define GPIO_OFF                5  /* Led off time, in 1/10 second */
#define LED_STAT_OFF     0
#define LED_STAT_ON      1
#define LED_STAT_BLINK   2
#define LED_STAT_UNINIT  0xff
//static char wl_led_stat = WL_LED_STAT_UNINIT;
static char mobile_led_stat = LED_STAT_UNINIT;
static long int prev_wl_rxbytes = 0;
static long int prev_mobile_rxbytes = 0;
static char next_line(FILE* fd) {
    char ch = 0;
    while ((ch = fgetc(fd)) && (ch != '\n') && (ch != EOF)) {
    }
}

static int get_device(FILE* fd, char *devbuf, int buflen) {
    int i = 0;
    while ((devbuf[i] = fgetc(fd)) && (devbuf[i] != EOF)) {
        /* ignore spaces */
        if ((devbuf[i] == ' ') || (devbuf[i] == '\t') || (devbuf[i] == '\n'))
            continue;
        if (devbuf[i] == ':') {
            devbuf[i] = '\0';
            break;
        }
        i++;
        if (i == (buflen - 1)) {
            devbuf[i] = '\0';
            i++;
            break;
        }
    }
    return i;
}

static void triggle_led_function(char led_stat, int pin) {
    /* check the led status if need change */
    if(pin == MOBILE_OVERRIDE_GPIO) {
        if (mobile_led_stat != led_stat){
            mobile_led_stat = led_stat;
        } else {
            return;
        }
    }
    switch (led_stat){
        case LED_STAT_ON:
            doSystem("%s led %d 1 0 0 0 0", GPIOCTL_BIN, pin);
            break;
        case LED_STAT_BLINK:
            doSystem("%s led %d %d %d 1 0 1", GPIOCTL_BIN,
                    pin, GPIO_ON, GPIO_OFF);
            break;
        case LED_STAT_OFF:
            doSystem("%s led %d 0 1 0 0 0", GPIOCTL_BIN, pin);
            break;
    }
}

static void monitor_wifi_func(void) {
    FILE *procfd;
    char strbuf[30];
    int i;
    long int rx_bytes;
    char nv_led_stat;
    /* leave if wifi is off or wps push button is started */
    ezplib_get_attr_val("wl_basic_rule", 0, "enable", strbuf, sizeof(strbuf),
            EZPLIB_USE_CLI);
    if ((strbuf[0] == '0') || (strbuf[0] == 'd')) {
        /* wlan is disabled */
        return;
    }
    ezplib_get_attr_val("wl_led_status_rule", 0, "wps", strbuf, sizeof(strbuf),
            EZPLIB_USE_CLI);
    if ((strbuf[0] == '1') || (strbuf[0] == 'o')) {
        /* wps push button is started */
        return;
    }
    ezplib_get_attr_val("wl_led_status_rule", 0, "wl", strbuf, sizeof(strbuf),
            EZPLIB_USE_CLI);
    switch (strbuf[0]) {
        case '0':
        case 'd':
            nv_led_stat = LED_STAT_OFF;
            break;
        case '2':
        case 'b':
            nv_led_stat = LED_STAT_BLINK;
            break;
        default:
            nv_led_stat = LED_STAT_ON;
            break;
    }
    /* open proc */
    procfd = fopen(NET_DEVICE_PROC, "r");
    if (!procfd)
        return;
    /* Ignore first line. */
    next_line(procfd);
    while(get_device(procfd, strbuf, sizeof(strbuf)) &&
            strncmp(strbuf, MONITOR_WIFI_DEVICE, sizeof(strbuf))) {
        next_line(procfd);
    }
    EZP_BWM_DBG("look for "MONITOR_WIFI_DEVICE" get:%s ... ", strbuf);
    i = 0;
    while ((strbuf[i] = fgetc(procfd)) && (strbuf[i] != EOF)) {
        if ((strbuf[i] == ' ') || (strbuf[i] == '\t') || (strbuf[i] == '\n')) {
            if (i) {
                break;
            } else {
                continue;
            }
        }
        else if ((strbuf[i] >= '0') && (strbuf[i] <= '9')) {
            i++;
        }
    }
    strbuf[i] = '\0';
    rx_bytes = strtol(strbuf, NULL, 10);
    EZP_BWM_DBG("flow:%s prev:%d rx_bytes-prev_rxbytes:%d nv_led_stat:%d\n",
            strbuf, prev_wl_rxbytes, rx_bytes - prev_wl_rxbytes, nv_led_stat);
    /* We recognize as if traffic > 1.5k bps */
    if (prev_wl_rxbytes && ((rx_bytes - prev_wl_rxbytes) >= 30000)
            && (nv_led_stat != LED_STAT_BLINK)) {
        ezplib_replace_attr("wl_led_status_rule", 0, "wl", "2");
        //wl_led_stat = LED_STAT_BLINK;
        triggle_led_function(LED_STAT_BLINK, WIFI_OVERRIDE_GPIO);
    }
    else if (((rx_bytes - prev_wl_rxbytes) < 29000) &&
            (nv_led_stat != LED_STAT_ON)){ /* We uses 29000 here to debounce */
        ezplib_replace_attr("wl_led_status_rule", 0, "wl", "1");
        //wl_led_stat = LED_STAT_ON;
        triggle_led_function(LED_STAT_ON, WIFI_OVERRIDE_GPIO);
    }
    prev_wl_rxbytes = rx_bytes;
    fclose(procfd);
}

static void monitor_mobile_func(void) {
    FILE *procfd;
    char strbuf[30], device[30], byte[30], packet[30];
    int i, res;
    long int rx_bytes = -1;
    char nv_led_stat;
    /* open proc */
    procfd = fopen(NET_DEVICE_PROC, "r");
    if (!procfd)
        return;
    /* Ignore first line. */
    next_line(procfd);
    while(fgets(strbuf, sizeof(strbuf), procfd)) {
        res = sscanf(strbuf, " %[^:]:%s\t%s", device, byte, packet);
        if(!strcmp(device, MONITOR_MOBILE_DEVICE)) {
            printf("look for "MONITOR_MOBILE_DEVICE" get:... \n");
            printf("byte:%s packet:%s\n", byte, packet);
            rx_bytes = strtol(byte, NULL, 10);
            break;
        }
    }
    if(rx_bytes >= 0){
        EZP_BWM_DBG("flow:%d\n", rx_bytes - prev_mobile_rxbytes);
        /* We recognize as if traffic > 10000 byte */
        if (prev_mobile_rxbytes &&
                ((rx_bytes - prev_mobile_rxbytes) >= 10000)) {
            triggle_led_function(LED_STAT_BLINK, MOBILE_OVERRIDE_GPIO);
        } else if ((rx_bytes - prev_mobile_rxbytes) < 9000){
            /* We uses 9000 here to debounce */
            triggle_led_function(LED_STAT_ON, MOBILE_OVERRIDE_GPIO);
        }
        prev_mobile_rxbytes = rx_bytes;
    } else {
        printf("device not found\n");
    }
    fclose(procfd);
}


int ar8316_wan_enable = 0;
void
wanled_switch() /* for ZYUS request */
{
    char wan0[16], wan1[16], wan0_state[16], wan1_state[16];
    /* get active wan */
    ezplib_get_attr_val("wan_status_rule", 0, "action", wan0, sizeof(wan0),
            EZPLIB_USE_CLI);
    ezplib_get_attr_val("wan_status_rule", 1, "action", wan1, sizeof(wan1),
            EZPLIB_USE_CLI);
    ezplib_get_attr_val("wan_status_rule", 0, "state", wan0_state,
            sizeof(wan0_state), EZPLIB_USE_CLI);
    ezplib_get_attr_val("wan_status_rule", 1, "state", wan1_state,
            sizeof(wan1_state), EZPLIB_USE_CLI);

    if(!strcmp(wan0, "up") && !strcmp(wan0_state, "4")) { 
        /* if ethernet wan is active and state is 4*/
        /* set the gpio 13 direction to input */
        doSystem("%s dirin %d", GPIOCTL_BIN, MOBILE_OVERRIDE_GPIO);
        /* enable the ar8316 wan port led */
        if(!ar8316_wan_enable) {
            system("switch greg w 00b0 cc35cc35");
            system("switch greg w 00b4 ca35ca35");
            system("switch greg w 00b8 c935c935");
            ar8316_wan_enable = 1;
        }
    } else if(!strcmp(wan1, "up") && !strcmp(wan1_state, "4")) { 
        /* if mobile wan is active and state is 4*/
        /* disable the ar8316 wan port led */
        if(ar8316_wan_enable) {
            system("switch greg w 00b0 0c35cc35");
            system("switch greg w 00b4 0a35ca35");
            system("switch greg w 00b8 0935c935");
            ar8316_wan_enable = 0;
            prev_mobile_rxbytes = 0;
            mobile_led_stat = LED_STAT_UNINIT;
        }
        /* monitor mobile wan throughput */
        monitor_mobile_func();
    } else { /* if no wan is active */
        /* disable the ar8316 wan port led */
        if(ar8316_wan_enable) {
            system("switch greg w 00b0 0c35cc35");
            system("switch greg w 00b4 0a35ca35");
            system("switch greg w 00b8 0935c935");
            ar8316_wan_enable = 0;
            prev_mobile_rxbytes = 0;
            mobile_led_stat = LED_STAT_UNINIT;
        }
        /* monitor mobile wan throughput */
        monitor_mobile_func();
    }
}

void 
usage()
{
    printf("argument error\n");
    exit(0);
}

int main(int argc, char **argv)
{
    int ret, stat = 0;
    char *wandev, *landev, *blim;
    /* Parse parameters*/
    while( (ret = getopt(argc, argv, "fsncwl:m:h:N:i:")) != -1 ){
        switch (ret) 
        {
            case 'f':
                foreground = 1;
                break;
            case 's':
                silent_output = 1;
                break;
            case 'n':
            /* Monitor for bandwidth */
                monitor_bw = 0;
                break;
            case 'c':
            /* Monitor for connection */
                monitor_conn = 1;
                break;
            case 'i':
                interval = atoi(optarg);
                break;
#ifndef DISABLE_WIFI_MONITOR
            case 'w':
                monitor_wifi = 0;
                break;
#endif
            case 'l':
                bw_limit_low = atoi(optarg);
                break;
            case 'm':
                bw_limit_med = atoi(optarg);
                break;
            case 'h':
                bw_limit_high = atoi(optarg);
                break;
            case '?':
                usage();
                break;
        }
    }
    /* Daemonize */
    if (!foreground) {
        daemonize();
    }

    if (monitor_bw) {
        wandev = nvram_get("wan0_ifname");
        landev = nvram_get("lan0_ifname");
        if ((!wandev) || (!landev) || !(*wandev) || !(*landev)) {
            perror("Lan/Wan device not ready.");
            exit(EXIT_FAILURE);
        }
    }

    /* main loop */
    while (1) {
        /* do wan interface control */
        if(doSystem("pidof ifcontrol")) {
            doSystem("ifcontrol lb");
        }
#ifdef EZP_PROD_BRAND_ZYUS /* for ZYUS VFG series */
        /* change the wan led status */
        wanled_switch();
#endif
#if 0
        if (monitor_bw) {
            /* system tc command */
            doSystem("tc -s class ls dev %s > /tmp/downrate", landev);
            doSystem("tc -s class ls dev %s > /tmp/uprate", wandev);
            if(filtmarktable()) {
                perror("Filt flow fail, check if /tmp/appmark /tmp/downrate /tmp/uprate are correct. ");
                exit(EXIT_FAILURE);
            }
        }
#endif
        if (monitor_conn) {
            stat = monitor_conn_func(stat);
        }
#if 0
#ifndef DISABLE_WIFI_MONITOR
        if (monitor_wifi) {
            monitor_wifi_func();
        }
#endif
#endif
sleep_next:
        sleep(interval);
    }

    if (monitor_bw) {
        freemarktable();
    }
    return ret ;
}

