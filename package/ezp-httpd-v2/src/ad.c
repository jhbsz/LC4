#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <ctype.h>
#include "assert.h"

#include "common.h"
#include "ezp-lib.h"
#include "ezp.h"
#include "lang.h"
#include "shared.h"

static int
valid_ping(webs_t wp, char *value, struct variable *v);
static int
valid_arping(webs_t wp, char *value, struct variable *v);
static int
valid_traceroute(webs_t wp, char *value, struct variable *v);


enum {
    HTTP_RMGT_ENABLE = 0,
#ifdef EZP_SUB_BRAND_GARMIN
	HTTP_ADMIN,
#endif
    HTTP_PASSWD,
    HTTP_PORT,
    HTTP_X_MODE,
};

static struct variable http_variables[] = {
    {longname: "Web Remote Management Enable", argv:ARGV("0", "1")},
#ifdef EZP_SUB_BRAND_GARMIN
    {longname: "Web Administrator Name", argv:ARGV("32")},
#endif
    {longname: "Web Administrator Password", argv:ARGV("32")},
    {longname: "Web Management Port", argv:ARGV("1", "65535")},
    {longname: "X Mode", argv:ARGV("")},
};

int
valid_x_mode(webs_t wp, char *value, struct variable *v)
{
    int i, rule_num;
    char prod[TMP_LEN];

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
                v->longname);
            return FALSE;
        }
    }

    /* Get X mode rule length */
    rule_num = ezplib_get_rule_num("x_mode_data");
    /* Iterative compare prod_cat in x_mode_data rule */
    for (i = 0;i < rule_num; i++) {
        ezplib_get_attr_val("x_mode_data", i,
                "prod_cat", prod, sizeof(prod), EZPLIB_USE_CLI);
        if(!strcmp(value, prod)) {
            return TRUE;
        }
    }
    websDebugWrite(wp, "Invalid <b>%s</b> %s: not one of X mode", v->longname);
    return FALSE;
}


int
valid_http(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* If prod_supercat is X, check X_mode is valid*/
    if(!strcmp("X", nvram_safe_get("prod_supercat"))) {
        snprintf(tmp, sizeof(tmp), "x_mode");
        val = websGetVar(wp, tmp, "");
        if (valid_x_mode(wp, val, &http_variables[HTTP_X_MODE]) == FALSE) {
            return FALSE;
        }
    }

#ifdef EZP_SUB_BRAND_GARMIN
	/* admin */
    snprintf(tmp, sizeof(tmp), "http_admin");
    val = websGetVar(wp, tmp, "");
#endif
    /* Password */
    snprintf(tmp, sizeof(tmp), "http_passwd");
    val = websGetVar(wp, tmp, "");
    if (valid_passwd(wp, val, &http_variables[HTTP_PASSWD]) == FALSE) {
        return FALSE;
    }

    /* Enable */
    snprintf(tmp, sizeof(tmp), "http_rmgt_enable");
    val = websGetVar(wp, tmp, "");

    if (valid_choice(wp, val, &http_variables[HTTP_RMGT_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* Port */
    snprintf(tmp, sizeof(tmp), "http_port");
    val = websGetVar(wp, tmp, "");

    if (valid_range(wp, val, &http_variables[HTTP_PORT]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
select_port_config(char *selection) {
#define STR_LEN 80
#define VLAN_LAN_RULE_ENTRY 0
#define VLAN_WAN_RULE_ENTRY 1
    char *port_config_rule = "portcfg_rule",
         namestr[STR_LEN],
         lanmem[STR_LEN],
         wanmem[STR_LEN];
    int pcfgnum = atoi(nvram_get("portcfg_rule_num"));
    int idx;
    for (idx = 0; idx < pcfgnum; idx ++) {
        ezplib_get_attr_val(port_config_rule, idx, "name", 
                namestr, sizeof(namestr), EZPLIB_USE_CLI);
        /* Found current */
        if (!strncmp(selection, namestr, strlen(namestr))) {
            int port_rule_num = atoi(nvram_safe_get("vlanport_rule_num")), portidx;
            ezplib_get_attr_val(port_config_rule, idx, "lan_member", 
                    lanmem, sizeof(lanmem), EZPLIB_USE_CLI);
            ezplib_get_attr_val(port_config_rule, idx, "wan_member", 
                    wanmem, sizeof(wanmem), EZPLIB_USE_CLI);
            ezplib_replace_attr("vlan_rule", VLAN_WAN_RULE_ENTRY, "portmember", wanmem);
            ezplib_replace_attr("vlan_rule", VLAN_LAN_RULE_ENTRY, "portmember", lanmem);
            for (portidx = 0;portidx < port_rule_num; portidx++) {
                /* If this port should config to wan port */
                if (wanmem[portidx] == '1') {
                    ezplib_replace_attr("vlanport_rule", portidx, "pvid", "2");
                }
                else {
                    ezplib_replace_attr("vlanport_rule", portidx, "pvid", "1");
                }
            }
            return 1;
        }
    }
    return 0;
}

int
_save_port_config(webs_t wp, char *value, 
        struct variable *v, struct service *services, int64_t *map) {
    char *pcfg = websGetVar(wp, "port_config", ""); 
    char lan_member_new[TMP_LEN], lan_member_old[TMP_LEN];
    ezplib_find_attr_val("portcfg_rule", "name", pcfg, "lan_member",
            lan_member_new, sizeof(lan_member_new));
    ezplib_get_attr_val("vlan_rule", 0, "portmember", lan_member_old,
            sizeof(lan_member_old), EZPLIB_USE_CLI);
    if (strcmp(lan_member_new, lan_member_old)) {
        config_preaction(map, v, services, "", ""); 
        select_port_config(pcfg);
        config_postaction(*map, services, "", "");
        return 1;
    }
    return 0;
}

int
save_http(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN], tmp_passwd[TMP_LEN];
    char *rmgt_enable, *passwd, *port, *x_mode;
#ifdef EZP_SUB_BRAND_GARMIN
	char *admin;
#endif

    char *rule_set = "http_rule";
    int len, change = 0;
    int64_t map = 0;
    /* If prod_supercat is X, Change prod_cat*/
    if(!strcmp("X", nvram_safe_get("prod_supercat"))) {
        snprintf(tmp, sizeof(tmp), "x_mode");
        x_mode = websGetVar(wp, tmp, "");
        if(strcmp(x_mode, ezp_prod_cat)) {
            /* If X mode is changed, we need to reboot */
            v->action_flag = ((v->action_flag >> 8) << 8) | ACTION_REBOOT ;
            config_preaction(&map, v, services, "", ""); 
            nvram_fset("prod_cat", x_mode);
            config_postaction(map, services, "", "");
            change = 1;
        }
    }

    /* Only apply to P1 */
    /*
    if(!strncmp(ezp_prod_cat, "P", sizeof("P")) && ezp_prod_subcat == 1) {*/
    if (appmgr_check("portcfg")) {
        /* config ethernet port */
        if(_save_port_config(wp, value, v, services, &map)) {
            change = 1;
        }
    }
    /* Remote Management Enable */
    snprintf(tmp, sizeof(tmp), "http_rmgt_enable");
    rmgt_enable = websGetVar(wp, tmp, "");
#ifdef EZP_SUB_BRAND_GARMIN
	/* admin */
    snprintf(tmp, sizeof(tmp), "http_admin");
    admin = websGetVar(wp, tmp, "");
#endif
    /* Password */
    snprintf(tmp, sizeof(tmp), "http_passwd");
    passwd = websGetVar(wp, tmp, "");
    if (!strcmp(passwd, TMP_PASSWD) || !strcmp(passwd, b64_TMP_PASSWD)) {
        /* Use the original passwd. */
        ezplib_get_attr_val("http_rule", 0, "passwd", tmp_passwd, 
                TMP_LEN, EZPLIB_USE_CLI); 
        passwd = tmp_passwd;
    }

    /* Port */
    snprintf(tmp, sizeof(tmp), "http_port");
    port = websGetVar(wp, tmp, "");

    /* Construct a rule tuple. */
    /* name^enable[0|1]^rmgt_enable[0|1]^username^passwd^port */
#ifdef EZP_SUB_BRAND_GARMIN
    len = 
        snprintf(tmp, TMP_LEN, "^1^%s^%s^%s^%s", rmgt_enable, admin, passwd, port);
#else

    len = 
        snprintf(tmp, TMP_LEN, "^1^%s^admin^%s^%s", rmgt_enable, passwd, port);
#endif

    if (len > TMP_LEN - 1) {
        return 0;
    }
    if (strcmp(tmp, nvram_safe_get(rule_set))) {
        config_preaction(&map, v, services, "", ""); 
        ezplib_replace_rule(rule_set, 0, tmp);
        config_postaction(map, services, "", "");
        change = 1;
    }

    return change;
}

static void
ad_tools_setup_cmdline(char *cmdline, char *act, char *iface, char *host, char *count)
{
    char tmp[30]={0}, *ptr = NULL, iface_opt[5]={0}, cnt_opt[20]={0};

    if(!strcmp(act, "traceroute")){
        strcpy(iface_opt,"-i");
        snprintf(cnt_opt,sizeof(cnt_opt),"-q 1 -l -n -m %s", count);
    }
    else {
        strcpy(iface_opt,"-I");
        snprintf(cnt_opt,sizeof(cnt_opt), "-c %s", count);
    }

    if (!strcmp(iface, "any")){
        sprintf(cmdline, "%s %s %s 2>&1 ", act, cnt_opt, host);
    }
    else {
        snprintf(tmp,sizeof(tmp), "%s_ifname", iface);
        ptr = nvram_safe_get(tmp);
        sprintf(cmdline, "%s %s %s %s %s 2>&1 ", act, iface_opt, ptr, cnt_opt, host);
    }

    if (!strcmp(act, "traceroute")){
        sprintf(cmdline, "%s > /tmp/traceroute.tmp", cmdline);
    }
}

static char*
ad_tools_find_skip_str(char *origstr, char *skip)
{
    char *p;
    p = strstr(origstr, skip);
    if (!p) return NULL;
    p += strlen(skip);
    return p;
}

static int
ad_tools_parse_ping_result(char *act, char *res, char *output)
{
    char *workptr = res;
    int npktx = 0, npktr = 0, avgtime = 0;
    double totdur = 0, tottime = 0;

    if (!strcmp(act, "ping")){
        if (!(workptr = ad_tools_find_skip_str(res, "ping statistics ---\n"))) goto pingerror;
        npktx = atoi(workptr);

        if (!(workptr = ad_tools_find_skip_str(res, "packets transmitted, "))) goto pingerror;
        npktr = atoi(workptr);
        if (npktr == 0) {
            goto pingerror;
        }

        if (!(workptr = ad_tools_find_skip_str(res, "round-trip min/avg/max = "))) goto pingerror;
        if (!(workptr = ad_tools_find_skip_str(workptr, "/"))) return 0;
        avgtime = atoi(workptr);
        sprintf(output, "<p>%s : %d<br/>%s : %d<br/>%s : %d %s<br/></p>", 
                lang_translate("Packet Sent"), npktx, 
                lang_translate("Packet Received"), npktr, 
                lang_translate("Average Time"), avgtime,
                lang_translate("ms"));
        return 1;
pingerror:
        sprintf(output, "Cannot send packet to host");
        return 1;
    }
    else if (!strcmp(act, "arping")){
        if(strstr(res, "interface") && strstr(res, "is not ARPable")){
            sprintf(output, "Device is not ARPable");
            return 1;
        }
        if(!(workptr = ad_tools_find_skip_str(res, "Sent "))) {
            goto arpingerror;
        }
        npktx = atoi(workptr);

        if(!(workptr = ad_tools_find_skip_str(res, "Received "))) {
            goto arpingerror;
        }
        npktr = atoi(workptr);

        if (npktr == 0) {
            goto arpingerror;
        }
        workptr = res;

        while (1){
            if (!(workptr = ad_tools_find_skip_str(workptr, "] "))) break;
            totdur += atof(workptr);
            tottime += 1;
        }
        if (0 == tottime) {
            sprintf(output, "<p>%s : %d<br/>%s : %d<br/></p>", 
                    lang_translate("Packet Sent"), npktx, 
                    lang_translate("Packet Received"), npktr);
        }
        else {
            sprintf(output, "<p>%s : %d<br/>%s : %d<br/>%s : %2.2f%s<br/></p>", 
                    lang_translate("Packet Sent"), npktx, 
                    lang_translate("Packet Received"), npktr, 
                    lang_translate("Average Time"), totdur/tottime,
                    lang_translate("ms"));
        }
        return 1;
arpingerror:
        sprintf(output, "Cannot send packet to host");
        return 1;
    }
    else if(!strcmp(act, "traceroute")){
        sprintf(output, "%s %s", act, lang_translate("complete"));
        return 1;
    }
    return 0;
}

void
do_at_tools_cgi(char *url, webs_t stream, ...)
{
    int num;
    char devname[TMP_LEN], tty[TMP_LEN];
    char *val, *val2, *val3, *delim="\n", atcommand[TMP_LEN];
    FILE *fp=NULL;
    char buf1[TMP_LEN], buf2[1400], *buf3, res[1400], log_res[TMP_LEN];
    int len = 1400;
    unsigned long outlen = TMP_LEN;

    memset(res,'\0',1400);
    
    num = atoi(nvram_safe_get("wan_num"))-1;
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "devname", devname, sizeof(devname), EZPLIB_USE_CLI);
    ezplib_get_attr_val("wan_wwan_probe_rule", num, "datatty", tty, sizeof(tty),
            EZPLIB_USE_CLI);
    if (!strlen(devname)) goto return_with_nodongle;
    system("rm /tmp/atcommand.tmp");
    val = websGetVar(wp, "at-command-textarea", "");
    base64_decode((unsigned char *)val, strlen(val),
                                   (unsigned char *)buf1, &outlen);
    if (!strlen(buf1)) goto return_with_nothing;
    val=buf1;
    while((val2=strsep(&val,delim))!=NULL){
        bool first=1;
        snprintf(atcommand, TMP_LEN, "SCRIPT=\'%s\' ezpcom -d /dev/%s%s -s /etc/chatscripts/script.gcom", val2, devname, tty);
        fp = popen(atcommand, "rw");
        if (fp){
            char* ptr=fgets(buf2, len, fp);
            while (ptr){
                len = len - strlen(ptr);
                ptr += strlen(ptr);
                ptr = fgets(ptr, len, fp);
            }
            pclose(fp);
        }
        if (!strlen(buf2)) goto return_with_datamode;
        buf3=buf2;
        while((val3=strsep(&buf3,delim))!=NULL){
            if (first) first=0;
            else if (!first && strlen(val3)) {
                snprintf(log_res,TMP_LEN,"logger EZP_USR %s",val3);
                snprintf(res,TMP_LEN,"echo %s >> /tmp/atcommand.tmp",val3);
                system(log_res);
                system(res);
            }
        }
    }
    websWrite(stream, "message=AT Command complete\n");
    websWrite(stream, "return_value=-3\n");
    goto return_and_init;
return_with_datamode:
    websWrite(stream, "message=Command Mode Not Available\n");
    websWrite(stream, "return_value=-2\n");
    goto return_and_init;
return_with_nothing:
    websWrite(stream, "message=Please write script first\n");
    websWrite(stream, "return_value=-2\n");
    goto return_and_init;
return_with_nodongle:
    websWrite(stream, "message=Please plug-in 3G/4G dongle first\n");
    websWrite(stream, "return_value=-4\n");
return_and_init:
    websDone(stream, 200);
    init_cgi(NULL);
    return;
}
void
do_ad_tools_cgi(char *url, webs_t stream, ...)
{
    FILE *fp=NULL;
    char buf[1400], cmd_line[256];
    int len = 1400;
    char *act = NULL, *host = NULL, *iface = NULL, *cnt = NULL;
    
    act = websGetVar(stream, "action", "");
    
    sprintf(buf, "%shost", act);
    host = websGetVar(stream, buf, "");
    
    sprintf(buf, "%siface", act);
    iface = websGetVar(stream, buf, "");

    sprintf(buf, "%scnt", act);
    cnt = websGetVar(stream, buf, "");

    switch(act[0])
    {
    case 'p': /* ping */
        /* host can't be zero string */
        if(!host[0] || !valid_ping(stream, "", NULL)){
            /* Nothing to do, just return */
            goto return_with_nothing;
        }
        break;
    case 'a': /* arping */
        /* host can't be zero string */
        if(!host[0] || !valid_arping(stream, "", NULL)){
            /* Nothing to do, just return */
            goto return_with_nothing;
        }
        break;
    case 't': /* traceroute */
        /* host can't be zero string */
        if(!host[0] || !valid_traceroute(stream, "", NULL)){
            /* Nothing to do, just return */
            goto return_with_nothing;
        }
        break;
    default:
        /* Nothing to do, just return */
        goto return_with_nothing;
    }
    ad_tools_setup_cmdline(cmd_line, act, iface, host, cnt);
    fp = popen(cmd_line, "rw");
    if (fp){
        char* ptr=fgets(buf, len, fp);
        while (ptr){
            len = len - strlen(ptr);
            ptr += strlen(ptr);
            ptr = fgets(ptr, len, fp);
        }
        pclose(fp);
    }
    if (ad_tools_parse_ping_result(act, buf, buf)){
        websWrite(stream, "message=%s\n", buf);
        websWrite(stream, "return_value=-3\n");
    }
    else {
        websWrite(stream, "return_value=-4\n");
    }
    goto return_and_init;
return_with_nothing:
    websWrite(stream, "return_value=-4\n");
return_and_init:
    websDone(stream, 200);
    init_cgi(NULL);
    return;
}

enum {
    ADTOOL_PINGDEV = 0,
    ADTOOL_PINGHOST,
    ADTOOL_PINGCNT,
    ADTOOL_ARPINGDEV,
    ADTOOL_ARPINGHOST,
    ADTOOL_ARPINGCNT,
    ADTOOL_TRACEROUTEDEV,
    ADTOOL_TRACEROUTEHOST,
    ADTOOL_TRACEROUTECNT,
};

static struct variable adtool_variables[] = {
    {longname: "Ping Device", argv:ARGV(""), nullok: TRUE},
    {longname: "Ping Host", argv:ARGV(""), nullok: TRUE},
    {longname: "Ping Packet Count", argv:ARGV("1", "15"), nullok: TRUE},
    {longname: "ARPing Device", argv:ARGV(""), nullok: TRUE},
    {longname: "ARPing Host", argv:ARGV(""), nullok: TRUE},
    {longname: "ARPing Packet Count", argv:ARGV("1", "15"), nullok: TRUE},
    {longname: "Traceroute Device", argv:ARGV(""), nullok: TRUE},
    {longname: "Traceroute Host", argv:ARGV(""), nullok: TRUE},
    {longname: "Traceroute Hop Count", argv:ARGV("1", "30"), nullok: TRUE},
};

static int
valid_ping(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    /* Ping device */
    snprintf(tmp, sizeof(tmp), "pingiface");
    val = websGetVar(wp, tmp, "");

    /* Host address */
    snprintf(tmp, sizeof(tmp), "pinghost");
    val = websGetVar(wp, tmp, "");

    if ((valid_ipaddr(wp, val, &adtool_variables[ADTOOL_PINGHOST]) == FALSE) &&
        (valid_domain(wp, val, &adtool_variables[ADTOOL_PINGHOST]) == FALSE)) {
        return FALSE;
    }

    /* Ping Packet Count */
    snprintf(tmp, sizeof(tmp), "pingcnt");
    val = websGetVar(wp, tmp, "");

    if (valid_range(wp, val, &adtool_variables[ADTOOL_PINGCNT]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

static int
valid_arping(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    /* ARPing device */
    snprintf(tmp, sizeof(tmp), "arpingiface");
    val = websGetVar(wp, tmp, "");

    /* Host address */
    snprintf(tmp, sizeof(tmp), "arpinghost");
    val = websGetVar(wp, tmp, "");

    if ((valid_ipaddr(wp, val, &adtool_variables[ADTOOL_ARPINGHOST]) == FALSE) &&
        (valid_domain(wp, val, &adtool_variables[ADTOOL_ARPINGHOST]) == FALSE)) {
        return FALSE;
    }

    /* ARPing Packet Count */
    snprintf(tmp, sizeof(tmp), "arpingcnt");
    val = websGetVar(wp, tmp, "");

    if (valid_range(wp, val, &adtool_variables[ADTOOL_ARPINGCNT]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

static int
valid_traceroute(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;
    /* Traceroute device */
    snprintf(tmp, sizeof(tmp), "tracerouteiface");
    val = websGetVar(wp, tmp, "");

    /* Host address */
    snprintf(tmp, sizeof(tmp), "traceroutehost");
    val = websGetVar(wp, tmp, "");

    if ((valid_ipaddr(wp, val, &adtool_variables[ADTOOL_TRACEROUTEHOST]) == FALSE) &&
        (valid_domain(wp, val, &adtool_variables[ADTOOL_TRACEROUTEHOST]) == FALSE)) {
        return FALSE;
    }

    /* Traceroute Hop Count */
    snprintf(tmp, sizeof(tmp), "traceroutecnt");
    val = websGetVar(wp, tmp, "");

    if (valid_range(wp, val, &adtool_variables[ADTOOL_TRACEROUTECNT]) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

int
valid_adtool(webs_t wp, char *value, struct variable *v)
{
    if(!valid_ping(wp, value, v))
        return FALSE;

    if(!valid_arping(wp, value, v))
        return FALSE;

    if(!valid_traceroute(wp, value, v))
        return FALSE;

    return TRUE;
}

int
save_adtool(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];
    char *pingiface, *pinghost, *pingcnt, *arpingiface, *arpinghost, 
         *arpingcnt, *tracerouteiface, *traceroutehost, *traceroutecnt;
    char *rule_set = "adtool_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Ping device */
    snprintf(tmp, sizeof(tmp), "pingiface");
    pingiface = websGetVar(wp, tmp, "");

    /* Host address */
    snprintf(tmp, sizeof(tmp), "pinghost");
    pinghost = websGetVar(wp, tmp, "");

    /* Ping packet count */
    snprintf(tmp, sizeof(tmp), "pingcnt");
    pingcnt = websGetVar(wp, tmp, "");

    /* ARPing device */
    snprintf(tmp, sizeof(tmp), "arpingiface");
    arpingiface = websGetVar(wp, tmp, "");

    /* Host address */
    snprintf(tmp, sizeof(tmp), "arpinghost");
    arpinghost = websGetVar(wp, tmp, "");

    /* ARPing packet count */
    snprintf(tmp, sizeof(tmp), "arpingcnt");
    arpingcnt = websGetVar(wp, tmp, "");

    /* Traceroute device */
    snprintf(tmp, sizeof(tmp), "tracerouteiface");
    tracerouteiface = websGetVar(wp, tmp, "");

    /* Host address */
    snprintf(tmp, sizeof(tmp), "traceroutehost");
    traceroutehost = websGetVar(wp, tmp, "");

    /* Traceroute hop count */
    snprintf(tmp, sizeof(tmp), "traceroutecnt");
    traceroutecnt = websGetVar(wp, tmp, "");

    /* pingiface^pinghost^pingcnt^arpingiface^arpinghost^arpingcnt^tracerouteiface^traceroutehost^traceroutecnt */
    len = 
        snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s", 
                pingiface, pinghost, pingcnt, arpingiface, arpinghost, arpingcnt,
                tracerouteiface, traceroutehost, traceroutecnt);

    if (len > TMP_LEN - 1) {
        return 0;
    }

    if (strcmp(tmp, nvram_safe_get(rule_set))) {
        config_preaction(&map, v, services, "", ""); 
        ezplib_replace_rule(rule_set, 0, tmp);
        config_postaction(map, services, "", "");
        change = 1;
    }

    return change;
}

static void
strtoupper(char *upper, char *orig){
    if(!orig || !upper)
        return;
    while(orig && *orig){
        *upper = toupper(*orig);
        upper++;
        orig++;
    }
    *upper = 0;
}
#ifdef EZP_PROD_BRAND_PROX
int
ej_ad_tools_showif(int eid, webs_t wp, int argc, char_t **argv){
    char *iface, *act, buf[TMP_LEN], upperbuf[SHORT_BUF_LEN];
    int num, i;

    if (ejArgs(argc, argv, "%s %s", &iface, &act) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    sprintf(buf, "%s_num", iface);
    num = atoi(nvram_safe_get(buf));
    for (i = num-1; i >=0; i--){
        if (!strcmp(iface, "wan")) {
            switch (i) {
                case 0:
                    strcpy(upperbuf, "WAN (Ethernet)");
                    break;
                case 1 :
                    strcpy(upperbuf, "WAN (USB Modem)");
                    break;
            }
        } else {
            strcpy(upperbuf, "LAN");
        }
        sprintf(buf, "<option value=\"%s%d\" >%s</option>\n",iface,i,upperbuf);
        websWrite(wp, buf);
    }
    return 0;
}
#else
int
ej_ad_tools_showif(int eid, webs_t wp, int argc, char_t **argv){
    char *iface, *act, *rule_set = "adtool_rule", 
         buf[TMP_LEN], buf_sel[SHORT_BUF_LEN],
         upperbuf[SHORT_BUF_LEN];
    int num, i;

    if (ejArgs(argc, argv, "%s %s", &iface, &act) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    sprintf(buf, "%siface", act);
    ezplib_get_attr_val(rule_set, 0, buf, buf_sel, SHORT_BUF_LEN, EZPLIB_USE_CLI);
    sprintf(buf, "%s_num", iface);
    num = atoi(nvram_safe_get(buf));
    for (i = 0; i < num; i++){
#ifdef EZP_PROD_BRAND_ZYUS
        if (!strcmp(iface, "wan")) {
            switch (i) {
                case 0:
                    strcpy(upperbuf, "Ethernet WAN");
                    break;
                case 1 :
                    strcpy(upperbuf, "Mobile WAN");
                    break;
            }
        } else {
            strcpy(upperbuf, "LAN");
        }
#endif
        if(!strcmp(buf_sel, buf))
        {
            /* Selected WAN iface */
#ifdef EZP_PROD_BRAND_ZYUS
            sprintf(buf, "<option value=\"%s%d\" selected> %s</option>",
                                        iface, i, upperbuf);
#else
            strtoupper(upperbuf, iface);
            sprintf(buf, "<option value=\"%s%d\" selected> %s%d</option>", 
                    iface, i, upperbuf, i+1);
#endif
        }
        else
        {
            /* Unselected WAN iface */
#ifdef EZP_PROD_BRAND_ZYUS
            sprintf(buf, "<option value=\"%s%d\"> %s</option>",
                    iface, i, upperbuf);
#else
            strtoupper(upperbuf, iface);
            sprintf(buf, "<option value=\"%s%d\"> %s%d</option>", 
                    iface, i, upperbuf, i+1);
#endif
        }
        websWrite(wp, buf);
    }
    return 0;
}
#endif
int
ej_ad_x_mode_switch(int eid, webs_t wp, int argc, char_t **argv)
{
    char *rule_set = "x_mode_data", name[TMP_LEN], value[TMP_LEN]; 
    int i, rule_num;
    /* If prod super cat is not "X", function return */
    if(strcmp(nvram_safe_get("prod_supercat"), "X")) {
        return 0;
    }
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "<div class='row'>\n");
    websWrite(wp, "    <div class='span12'>\n");
    websWrite(wp, "        <legend>%s</legend>\n", lang_translate("Mode Switch"));
    websWrite(wp, "        <div class='row show-grid'>\n");
    websWrite(wp, "            <div class='span4'>%s</div>\n", lang_translate("USB Mode"));
    websWrite(wp, "            <div class='span8'>\n");
#else    
    websWrite(wp, "<fieldset>\n");
    websWrite(wp, "<legend>%s</legend>", lang_translate("Mode Switch"));
    websWrite(wp, "<div class=\"setting\">");
    websWrite(wp, "<div class=\"label\">%s</div>", lang_translate("USB Mode"));
    websWrite(wp, "</div>");
#endif    
    websWrite(wp, "<select name=\"x_mode\" id=\"x_mode\" >");
    /* Construct ComboBox select option */
    rule_num = ezplib_get_rule_num("x_mode_data");
    for (i = 0; i < rule_num; i++) {
        ezplib_get_attr_val(rule_set, i,
                "name", name, sizeof(name), EZPLIB_USE_CLI);
        ezplib_get_attr_val(rule_set, i,
                "prod_cat", value, sizeof(value), EZPLIB_USE_CLI);
        websWrite(wp, "<option value=\"%s\"\n", value);
        if(!strcmp(nvram_safe_get("prod_cat"), value)){
            websWrite(wp, "selected=\"selected\"\n");
        }
        websWrite(wp, ">%s</option>\n", name);
    }
    websWrite(wp, "</select>");
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "            </div>\n");
    websWrite(wp, "        </div>\n");
    websWrite(wp, "    </div>\n");
    websWrite(wp, "</div><!-- row -->\n");
    websWrite(wp, "<br>\n");
#else
    websWrite(wp, "</fieldset><br>");
#endif    
    
    return 0;
}

int
ej_ad_vlanport_config(int eid, webs_t wp, int argc, char_t **argv)
{
    char  *port_config_rule = "portcfg_rule",
          *proto = nvram_safe_get("wan0_proto"),
         dispstr[TMP_LEN], namestr[TMP_LEN];
    char lan_member_now[TMP_LEN], lan_member_rule[TMP_LEN];
    char wan_member_now[TMP_LEN], wan_member_rule[TMP_LEN];
    int pcfgnum = atoi(nvram_get("portcfg_rule_num"));
    int idx;

    /* If prod cat is not "P", function return */
    /*if(strncmp(ezp_prod_cat, "P", sizeof("P")) || ezp_prod_subcat != 1) {*/
    if (!appmgr_check("portcfg")) {
        return 0;
    }
    /* find out LAN and WAN vlan member now use */
    ezplib_find_attr_val("vlan_rule", "name", "LAN", "portmember",
            lan_member_now, sizeof(lan_member_now));
    ezplib_find_attr_val("vlan_rule", "name", "WAN", "portmember",
            wan_member_now, sizeof(wan_member_now));

    /* Current selection */
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "<div class='row'>\n");
    websWrite(wp, "    <div class='span12'>\n");
    websWrite(wp, "        <legend>%s</legend>\n", lang_translate("Ethernet Port"));
    websWrite(wp, "        <div class='row show-grid'>\n");
    websWrite(wp, "            <div class='span4'>%s</div>\n", lang_translate("Ethernet Port"));
    websWrite(wp, "            <div class='span8'>\n");
#else    
    websWrite(wp, "<fieldset>\n");
    websWrite(wp, "<legend>%s</legend>", lang_translate("Ethernet Port"));
    websWrite(wp, "<div class=\"setting\">");
    websWrite(wp, "<div class=\"label\">%s</div>", lang_translate("Ethernet Port"));
    websWrite(wp, "</div>");
#endif
    websWrite(wp, "<select name=\"port_config\" id=\"port_config\"");    
    if (!strcmp(proto, "dhcp") || !strcmp(proto, "static") ||
            !strcmp(proto, "pppoe")) {
        websWrite(wp, "onchange=\"alert('%s')\">",
                lang_translate("Configuring the ethernet port as the LAN port will cause the WAN connectivity issue"));
    } else {
        websWrite(wp, ">");
    }
    /* Construct ComboBox select option */
    for (idx = 0; idx < pcfgnum; idx++) {
        ezplib_get_attr_val(port_config_rule, idx, "name", 
                namestr, sizeof(namestr), EZPLIB_USE_CLI);
        ezplib_get_attr_val(port_config_rule, idx, "dispstr", 
                dispstr, sizeof(dispstr), EZPLIB_USE_CLI);
        websWrite(wp, "<option value=\"%s\"", namestr);
        /* get port member in portcfg_rule */
        ezplib_get_attr_val(port_config_rule, idx, "lan_member", 
                lan_member_rule, sizeof(lan_member_rule), EZPLIB_USE_CLI);
        ezplib_get_attr_val(port_config_rule, idx, "wan_member", 
                wan_member_rule, sizeof(wan_member_rule), EZPLIB_USE_CLI);

        /* Compare lan_member and wan_member of portcfg_rule with 
         * vlan_rule to identify what port config is selected now */
        if(!strcmp(lan_member_now, lan_member_rule) && 
                !strcmp(wan_member_now, wan_member_rule)){
            websWrite(wp, "selected=\"selected\"\n");
        }
        websWrite(wp, ">%s</option>\n", lang_translate(dispstr));
    }
    websWrite(wp, "</select>");
#ifdef EZP_PROD_BRAND_AXIM
    websWrite(wp, "            </div>\n");
    websWrite(wp, "        </div>\n");
    websWrite(wp, "    </div>\n");
    websWrite(wp, "</div><!-- row -->\n");
    websWrite(wp, "<br>\n");
#else    
    websWrite(wp, "</fieldset><br>");
#endif    

    return 0;
}
