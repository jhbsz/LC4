#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <sys/klog.h>
#include <cyutils.h>

#include "shared.h"
#include "common.h"
#include "lang.h"
#include "ezp.h"
#include "ezp-lib.h"

#define EZP_SYSLOG_USR_FILE_PATH "/tmp/log_usr"
#define EZP_SYSLOG_IPT_FILE_PATH "/tmp/log_ipt"

enum {
    REMOTE_ENABLE = 0,
    REMOTE_IP,
    REMOTE_PROTOCOL,
    REMOTE_PORT,
};

static struct variable log_variables[] = {
    {longname: "Syslog Server Enable", argv:ARGV("0", "1")},
    {longname: "Remote IP Address", argv:ARGV("64"), nullok: FALSE},
    {longname: "Protocol", argv:ARGV("32"), nullok: FALSE},
    {longname: "Remote TCP Port", argv:ARGV("1", "65535")},
};

void
remove_illegal(char *buf, int len)
{
    int i;
    for(i = 0; i < len; i++) {
        if(buf[i] == '\'' || buf[i] == '\"' || buf[i] == ',') {
            buf[i] = ' ';
        }
    }
}


void
dump_log2json(char *path, webs_t wp)
{
    FILE *fp=NULL;
    char msg_orig[LONG_BUF_LEN];

    fp = fopen(path, "r");

#ifdef EZP_PROD_BRAND_AXIM
    while (fgets(msg_orig, sizeof(msg_orig), fp)){
	websWrite(wp, "%s", msg_orig);
    }
#else
    char *pos, msg[LONG_BUF_LEN];
    websWrite(wp, "{success: true, msg:'");
    while (fgets(msg_orig, sizeof(msg_orig), fp)){
        pos = strchr(msg_orig, '\n');
            bzero(msg, sizeof(msg));
        if (pos) {
            strncpy(msg, msg_orig, pos - msg_orig);
            remove_illegal(msg, sizeof(msg));
            websWrite(wp, "%s", msg);
            websWrite(wp, "\\r\\n");
        } else {
            strncpy(msg, msg_orig, strlen(msg_orig));
            remove_illegal(msg, sizeof(msg));
            websWrite(wp, "%s", msg);
            websWrite(wp, "\\r\\n");
        }
    }
    websWrite(wp, "'}");
#endif
    fclose(fp);
}

void
do_log_cgi(char *url, webs_t stream, ...)
{
    char cmd[TMP_LEN];
    char logread[] = "logread -k";
#if defined(EZP_PROD_BRAND_PROX)
    char run_tac[] = "tac ";
    char filename[TMP_LEN];
#endif
    char *class = NULL;

    /* Get log class */
    class = websGetVar(wp, "log_class", "");
   
    /* do action */
#ifdef EZP_PROD_BRAND_AXIM
    snprintf(cmd, sizeof(cmd), "%s %s > %s",
                        logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
#else

    switch (atoi(class)) {
        case 0:
            snprintf(cmd, sizeof(cmd), "%s %s > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
        case 1:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<SYS>\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
        case 2:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<DHCP\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
        case 3:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<DNS\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
        case 4:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<UPnP\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
        case 5:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<PPTP\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
        case 6:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<L2TP\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
        case 7:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<IPSEC\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;

    #if defined(EZP_PROD_BRAND_PROX)
        case 8:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<DDNS\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
        case 9:
            snprintf(cmd, sizeof(cmd), "%s %s | grep \"<NTP\" > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;

        case 99:
            snprintf(cmd,sizeof(cmd),"logwrite -d -k USR");
            break;
    #endif
        default:
            snprintf(cmd, sizeof(cmd), "%s %s > %s",
                    logread, "USR", EZP_SYSLOG_USR_FILE_PATH);
            break;
    }

#endif
    system(cmd);
#if defined(EZP_PROD_BRAND_PROX)
/* For reversing the content of EZP_SYSLOG_USR_FILE_PATH */
    snprintf(cmd, sizeof(cmd), "%s %s > %s.tmp",run_tac, EZP_SYSLOG_USR_FILE_PATH, EZP_SYSLOG_USR_FILE_PATH);
    system(cmd);
    snprintf(filename, sizeof(filename), "%s.tmp", EZP_SYSLOG_USR_FILE_PATH);
    unlink(EZP_SYSLOG_USR_FILE_PATH);
    rename(filename, EZP_SYSLOG_USR_FILE_PATH);
#endif
    dump_log2json(EZP_SYSLOG_USR_FILE_PATH, stream);
    websDone(stream, 200);

    /* Flush all key-value pairs in tha hash table. */
    init_cgi(NULL);
    return;
}

int
ej_st_log_show_system(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name;
    char *cmd_argv[4];
    char tmp[TMP_LEN];

    name=EZP_SYSLOG_USR_FILE_PATH;
    cmd_argv[0] = "logread";
    cmd_argv[1] = "-k";
    cmd_argv[2] = "USR";
    cmd_argv[3] = NULL;

    /* Kick stats to generate the file. */
    snprintf(tmp, TMP_LEN, ">%s", name);
    if (!_eval(cmd_argv, tmp, 15, NULL)) {
        do_file(name, wp, NULL);
    }
    unlink(name);

    return 0;
}

int
ej_st_log_show_iptables(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name;
    char *cmd_argv[4];
    char tmp[TMP_LEN];

    name=EZP_SYSLOG_IPT_FILE_PATH;
    cmd_argv[0] = "logread";
    cmd_argv[1] = "-k";
    cmd_argv[2] = "IPT";
    cmd_argv[3] = NULL;

    /* Kick stats to generate the file. */
    snprintf(tmp, TMP_LEN, ">%s", name);
    if (!_eval(cmd_argv, tmp, 15, NULL)) {
        do_file(name, wp, NULL);
    }
    unlink(name);

    return 0;
}

#if 0
/* Dump firewall log: We might reuse the following code in the future. */
/* max buf, total have 64 entries */
#define LOG_BUF	16384           
int
ej_st_log_show_iptables(int eid, webs_t wp, int argc, char_t ** argv)
{
    char buf[LOG_BUF], *line, *next, *s;
    int len, ret = 0;
    char *type;

    time_t tm;
    char *verdict, *src, *dst, *proto, *spt, *dpt, *in, *out;
    char src_old[10] = "", dpt_old[10] = "", dst_old[10] = "";

    int _dport, _sport;
    char *_proto = NULL;
    struct servent *servp;
    //struct servent *d_servp;

    char skip[10];
    int debug = 0;

    char *wan_if = get_wan_face();
    char *lan_if = nvram_safe_get("lan_ifname");

    int count = 0;

    if (ejArgs(argc, argv, "%s", &type) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    //if (klogctl(3, buf, 4096) < 0) {
    if (klogctl(3, buf, LOG_BUF) < 0) {
        websError(wp, 400, "Insufficient memory\n");
        return -1;
    }

    for (next = buf; (line = strsep(&next, "\n"));) {
        strcpy(skip, "");
        if (!strncmp(line, "<4>DROP", 7))
            verdict = "denied";
        else if (!strncmp(line, "<4>ACCEPT", 9))
            verdict = "accepted";
        else
            continue;


        /* Parse into tokens */
        s = line;
        len = strlen(s);
        while (strsep(&s, " "));

        /* Initialize token values */
        time(&tm);
        in = out = src = dst = proto = spt = dpt = "n/a";

        /* Set token values */
        for (s = line; s < &line[len] && *s; s += strlen(s) + 1) {
            if (!strncmp(s, "TIME=", 5))
                tm = strtoul(&s[5], NULL, 10);
            else if (!strncmp(s, "IN=", 3))
                in = &s[3];
            else if (!strncmp(s, "OUT=", 4))
                out = &s[4];
            else if (!strncmp(s, "SRC=", 4))
                src = &s[4];
            else if (!strncmp(s, "DST=", 4))
                dst = &s[4];
            else if (!strncmp(s, "PROTO=", 6))
                proto = &s[6];
            else if (!strncmp(s, "SPT=", 4))
                spt = &s[4];
            else if (!strncmp(s, "DPT=", 4))
                dpt = &s[4];
        }

        if (!strncmp(dpt, "n/a", 3))    // example: ping
            continue;

        _dport = atoi(dpt);
        _sport = atoi(spt);

        if (!strncmp(proto, "TCP", 3))
            _proto = "tcp";
        else if (!strncmp(proto, "UDP", 3))
            _proto = "udp";

#ifdef EZPACKET
        servp = my_getservbyport(htons(_dport), _proto);
#else
        servp = NULL;
#endif

        if (!strcmp(type, "incoming")) {
            if ((!strncmp(in, "ppp", 3) && !strncmp(in, wan_if, 3)) ||
                (!strcmp(in, wan_if))) {
                if (!strcmp(src, src_old) && !strcmp(dpt, dpt_old)) {   // skip same record
                    if (debug)
                        strcpy(skip, "(skip) ");
                    else
                        continue;
                } else {
                    strcpy(src_old, src);
                    strcpy(dpt_old, dpt);
                }
                ret +=
                    websWrite(wp,
                              "<TR bgcolor=cccccc> <TD align=middle height=1><SPAN STYLE=\"FONT-SIZE: 12pt\">%s%s</TD> <TD align=middle height=1><SPAN STYLE=\"FONT-SIZE: 12pt\">%s</TD></TR>\n",
                              skip, src, servp ? servp->s_name : dpt);
            }
        } else if (!strcmp(type, "outgoing")) {
            if (!strncmp(in, lan_if, 3) &&
                ((!strncmp(out, "ppp", 3) && !strncmp(out, wan_if, 3)) ||
                 (!strcmp(out, wan_if)))) {
                if (_dport == 53) {     // skip DNS
                    if (debug)
                        strcpy(skip, "(skip) ");
                    else
                        continue;
                }
                if (!strcmp(src, src_old) && !strcmp(dpt, dpt_old) && !strcmp(dst, dst_old)) {  // skip same record
                    if (debug)
                        strcpy(skip, "(skip) ");
                    else
                        continue;
                } else {
                    strcpy(src_old, src);
                    strcpy(dpt_old, dpt);
                    strcpy(dst_old, dst);
                }
                ret +=
                    websWrite(wp,
                              "<TR bgcolor=cccccc> <TD align=middle><SPAN STYLE=\"FONT-SIZE: 12pt\">%s%s</TD><TD align=middle><SPAN STYLE=\"FONT-SIZE: 12pt\">%s</TD><TD align=middle><SPAN STYLE=\"FONT-SIZE: 12pt\">%s</TD></TR>\n",
                              skip, src, dst, servp ? servp->s_name : dpt);
            }
        } else if (!strcmp(type, "all")) {
            int dir = 0;
            if ((!strncmp(out, "ppp", 3) && !strncmp(out, wan_if, 3)) || (!strcmp(out, wan_if)))        // incoming
                dir = 1;
            else if (!strncmp(in, lan_if, 3) && ((!strncmp(out, "ppp", 3) && !strncmp(out, wan_if, 3)) || (!strcmp(out, wan_if))))      // outgoing
                dir = 2;
            else
                continue;

            if (_dport == 53) { // skip DNS
                if (debug)
                    strcpy(skip, "(skip) ");
                else
                    continue;
            }
            if (!strcmp(src, src_old) && !strcmp(dpt, dpt_old) && !strcmp(dst, dst_old)) {      // skip same record
                if (debug)
                    strcpy(skip, "(skip) ");
                else
                    continue;
            } else {
                strcpy(src_old, src);
                strcpy(dpt_old, dpt);
                strcpy(dst_old, dst);
            }
            ret +=
                websWrite(wp, "%c'%s','%s','%s','%s','%d'\n",
                          count ? ',' : ' ', proto, src, dst,
                          servp ? servp->s_name : dpt, dir);
            count++;
        }
    }

    return ret;
}
#endif

int
valid_log(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "remote_enable");
    val = websGetVar(wp, tmp, "");

    if (valid_choice(wp, val, &log_variables[REMOTE_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* IP */
    snprintf(tmp, sizeof(tmp), "remote_ip_ipaddr");
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &log_variables[REMOTE_IP]) == FALSE) {
        return FALSE;
    }

    /* Protocol */
    snprintf(tmp, sizeof(tmp), "proto");
    val = websGetVar(wp, tmp, "");

    if (valid_name(wp, val, &log_variables[REMOTE_PROTOCOL]) == FALSE) {
        return FALSE;
    }

    /* Port */
    snprintf(tmp, sizeof(tmp), "remote_port");
    val = websGetVar(wp, tmp, "");

    if (valid_range(wp, val, &log_variables[REMOTE_PORT]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_log(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];
    char name[TMP_LEN],enable[TMP_LEN],ipaddr[TMP_LEN],ipt_enable[TMP_LEN],
         web_mgmt[TMP_LEN],dns[TMP_LEN],ppp[TMP_LEN],upnp[TMP_LEN],
         wireless[TMP_LEN],ntp[TMP_LEN],sys_warning[TMP_LEN],
         dhcp_serv[TMP_LEN],dhcp_cli[TMP_LEN],ddns[TMP_LEN],firewall[TMP_LEN];
    char *remote_enable, *remote_ip, *proto, *remote_port;
    char *rule_set = "log_rule";
    int len, change = 0;
    int64_t map = 0;

    /* Syslog server Enable */
    snprintf(tmp, sizeof(tmp), "remote_enable");
    remote_enable = websGetVar(wp, tmp, "");

    /* IP */
    snprintf(tmp, sizeof(tmp), "remote_ip_ipaddr");
    remote_ip = websGetVar(wp, tmp, "");

    /* Protocol */
    snprintf(tmp, sizeof(tmp), "proto");
    proto = websGetVar(wp, tmp, "");

    /* Port */
    snprintf(tmp, sizeof(tmp), "remote_port");
    remote_port = websGetVar(wp, tmp, "");

    ezplib_get_attr_val("log_rule", 0, "name", name,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "enable", enable,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "ipaddr", ipaddr,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "ipt_enable", ipt_enable,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "web_mgmt", web_mgmt,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "dns", dns,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "ppp", ppp,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "upnp", upnp,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "wireless", wireless,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "ntp", ntp,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "sys_warning", sys_warning,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "dhcp_serv", dhcp_serv,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "dhcp_cli", dhcp_cli,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "ddns", ddns,
            TMP_LEN, EZPLIB_USE_CLI);
    ezplib_get_attr_val("log_rule", 0, "firewall", firewall,
            TMP_LEN, EZPLIB_USE_CLI);

    /* Construct a rule */
        len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s", name, enable, ipaddr, ipt_enable, web_mgmt, dns, ppp, upnp, wireless, ntp, sys_warning, dhcp_serv, dhcp_cli, ddns, firewall, remote_enable, remote_ip, proto, remote_port);

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

int
ej_st_log_show_class(int eid, webs_t wp, int argc, char_t ** argv)
{
    char tmp[TMP_LEN];
    websWrite(wp,
            "[[0,'ALL'],[1,'SYS'],[2,'  DHCP'],[3,'DNS'], [4,'UPNP']");
    /* PPTP */
    ezplib_get_attr_val("appmgr_rule", 0, "pptp", tmp,
            sizeof(tmp), EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        websWrite(wp, ",[5,'PPTP']");
    }
    /* L2TP */
    ezplib_get_attr_val("appmgr_rule", 0, "l2tp", tmp,
            sizeof(tmp), EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        websWrite(wp, ",[6,'L2TP']");
    }
    /* IPSEC */
    ezplib_get_attr_val("appmgr_rule", 0, "ipsec", tmp,
            sizeof(tmp), EZPLIB_USE_CLI);
    if(tmp[0] == '1') {
        websWrite(wp, ",[7,'IPSEC']");
    }
#if defined(EZP_PROD_BRAND_PROX)
    websWrite(wp, ",[8,'DDNS']");
    websWrite(wp, ",[9,'NTP']");
#endif
    websWrite(wp, "]");
    return 0;
}
