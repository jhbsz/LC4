#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include "assert.h"
#include <time.h>

#include "common.h"
#include "ezp-lib.h"
#include "ezp.h"


void
als_ddns(webs_t wp)
{
    FILE *fp = NULL;
    char buff[1024] = {0};
    char *data = NULL;

    data = websGetVar(wp, "hostname_ddns", "");
    if(data)
        ezplib_replace_attr("wan_ddns_rule", 0 ,"hostname",data);

    system("/etc/rc.common /etc/init.d/als-ddns start");
    sleep(3);

    fp = fopen("/tmp/ddns_result","r");
    while (fgets(buff, sizeof(buff),fp) != NULL){    
        if(buff){
            websWrite2(wp,"%s",buff);
            break;
        }
    }
    fclose(fp);
}

int
ej_do_als_week(int eid, webs_t wp, int argc, char_t **argv)
{
    time_t t1 = time(NULL);
    struct tm *ptr = localtime(&t1);
    int wday = ptr->tm_wday + 1;

    websWrite2(wp,"%d",wday);

    return 1;

}

int
ej_do_als_time(int eid, webs_t wp, int argc, char_t **argv)
{
    time_t t1 = time(NULL);
    struct tm *ptr = localtime(&t1);
    int hour = (ptr->tm_hour * 2) + 1;
    hour +=(ptr->tm_min > 30 ) ? 1 : 0 ;

    websWrite2(wp,"%d",hour);

    return 1;
}

void command(webs_t wp)
{
    FILE *fp = NULL;
    char buf[128] = {0};
    char data[128] = {0};
    fp = popen("/usr/sbin/nvram get wl0_ssid_rule_default","r");
    if (fp)
    {
	fgets(buf,sizeof(buf),fp);
        websWrite2(wp,"%s",buf);
    }
    fp = popen("/usr/sbin/nvram get model","r");
    if (fp)
    {
	fgets(buf,sizeof(buf),fp);
        websWrite2(wp,"%s",buf);
    }
}


void getrssi(webs_t wp)
{
    char buf[TMP_LEN];
    char data[TMP_LEN];
    char newline[TMP_LEN];
    FILE *fp;
/*
    ezplib_get_attr_val( "wan_wwan_rule", 0, "devname", data, sizeof(data), EZPLIB_USE_CLI);
    sprintf(buf,"SCRIPT='At+csq' ezpcom -d /dev/%s",data);
    ezplib_get_attr_val( "wan_wwan_rule", 0, "ctrltty", data, sizeof(data), EZPLIB_USE_CLI);    
    sprintf(buf,"%d -s /etc/chatscripts/script.gcom | sed -n '2p' | awk '{FS=\":\";print $2$3}' | awk '{FS=\",\";print $1}' | tr -d \" \" ",atoi(buf)-1);

    fp = popen(buf,"r");
    if ((fgets(newline, 256, fp)) != NULL) {
        websWrite2(wp,"%d",atoi(newline)*2 - 113);
    }
    pclose(fp);

    sprintf(buf,"nvram replace attr wan_wwan_probe_rule 0 sig %d",atoi(newline)*2 - 113);
    system(buf);
    system("nvram commit");*/
    system("/bin/sh /sbin/ifdown wan0 &");

}

void gettime(webs_t wp)
{
    time_t t1 = time(NULL);
    struct tm *ptr = localtime(&t1);
    int hour = (ptr->tm_hour * 2) + 1;
    hour +=(ptr->tm_min > 30 ) ? 1 : 0 ;
    int wday = ptr->tm_wday + 1;

    websWrite2(wp,"%d,%d\n",hour,wday);
}

void status(webs_t wp)
{
    char *als_status_rule = "als_status_rule";
    char buf[TMP_LEN];
    char data[TMP_LEN];
    char path[TMP_LEN];
    char Rx[TMP_LEN];
    char Tx[TMP_LEN];
    int i;
    FILE *fp;

    for (i = 0 ;i < 3 ; i++)
    {
        ezplib_get_attr_val( als_status_rule, i, "enable", buf, sizeof(buf), EZPLIB_USE_CLI);
        if(!strcmp(buf,"0"))continue;
        ezplib_get_attr_val( als_status_rule, i, "alias", buf, sizeof(buf), EZPLIB_USE_CLI);
        websWrite2(wp,"port=%s\n",buf);
        ezplib_get_attr_val( als_status_rule, i, "mode", buf, sizeof(buf), EZPLIB_USE_CLI);
        switch(atoi(buf))
        {
            case 0:
                websWrite2(wp,"TCP Server\n");
                break;
            case 1:
                websWrite2(wp,"TCP Client\n");
                break;
            case 2:
                websWrite2(wp,"UDP\n");
                break;
        }
        memset(data,0,sizeof(data));
        ezplib_get_attr_val( "als_serial_rule", i, "alias", buf, sizeof(buf), EZPLIB_USE_CLI);
        sprintf(path,"/bin/cat /tmp/%s-Rx",buf);
        fp = popen(path,"r");
        if(fp)
        {
            fgets(data,sizeof(data),fp);
            pclose(fp);
        }
        if (strlen(data) > 1 )
            websWrite2(wp,"%s",data);
        else
            websWrite2(wp,"0\n");

        ezplib_get_attr_val( als_status_rule, i, "isp1", buf, sizeof(buf), EZPLIB_USE_CLI);
        if (!strcmp(buf,"1"))websWrite2(wp,"Listen\n");
        else if(!strcmp(buf,"0"))websWrite2(wp,"Close\n");
        else if(!strcmp(buf,"2"))websWrite2(wp,"Loading...\n");
        else websWrite2(wp,"%s\n",buf);
        ezplib_get_attr_val( als_status_rule, i, "isp2", buf, sizeof(buf), EZPLIB_USE_CLI);
        if (!strcmp(buf,"1"))websWrite2(wp,"Listen\n");
        else if(!strcmp(buf,"0"))websWrite2(wp,"Close\n");
        else if(!strcmp(buf,"2"))websWrite2(wp,"Loading...\n");
        else websWrite2(wp,"%s\n",buf);
        ezplib_get_attr_val( als_status_rule, i, "isp3", buf, sizeof(buf), EZPLIB_USE_CLI);
        if (!strcmp(buf,"1"))websWrite2(wp,"Listen\n");
        else if(!strcmp(buf,"0"))websWrite2(wp,"Close\n");
        else if(!strcmp(buf,"2"))websWrite2(wp,"Loading...\n");
        else websWrite2(wp,"%s\n",buf);
        ezplib_get_attr_val( als_status_rule, i, "isp4", buf, sizeof(buf), EZPLIB_USE_CLI);
        if (!strcmp(buf,"1"))websWrite2(wp,"Listen\n");
        else if(!strcmp(buf,"0"))websWrite2(wp,"Close\n");
        else if(!strcmp(buf,"2"))websWrite2(wp,"Loading...\n");
        else websWrite2(wp,"%s\n",buf);
    }

    return;
}

int command_save(webs_t wp, char *value, struct variable *v, struct service *s)
{

    char *data;
    FILE *fp = NULL;
    int64_t map = 0;
    char default_wifi[128] = {0};    
    char command[128] = {0};    

    int change = 1;

    data = websGetVar(wp, "d_model", "");
    if(data){
	sprintf(command,"/usr/sbin/nvram fset model=\"%s\"",data);
        fp = popen(command,"w");
        if (fp)pclose(fp);
    }
    data = websGetVar(wp, "w_1_d", "");
    if (data){
        strcpy(default_wifi,data);
        strcat(default_wifi,"|");
    }
    data = websGetVar(wp, "w_2_d", "");
    if(data && default_wifi){
        strcat(default_wifi,data);
	sprintf(command,"/usr/sbin/nvram fset wl0_ssid_rule_default=\"%s\"",default_wifi);
        fp = popen(command,"w");
        if (fp)pclose(fp);
    }

    system("/usr/sbin/nvram commit");
    return change;
}

struct als_handler{
    char *pattern;
    void (*action)(webs_t);
};

struct als_handler status_handler[] = {
    {"serial",status},
    {"ddns",als_ddns},
    {"getservertime",gettime},
    {"rssi",getrssi},
    {"command",command},
    {"command_save",command_save},
    {NULL,NULL}
};

void
als_status(char *file, webs_t stream, ...) 
{
    struct als_handler *handler;
    char * value;
    
    value = websGetVar(stream,"action","");
    for(handler = &status_handler[0];handler->pattern;handler++)
    {
        if(!strcmp(handler->pattern,value))break;
    }
    if(handler->action){    
        handler->action(stream);
    }
    return;
}

int valid_als_func(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int save_als_func(webs_t wp, char *value, struct variable *v, struct service *s)
{

    char *data,*page;
    int64_t map = 0;
    
    int change = 0;

    config_preaction(&map, v, s, "", "");

    page = websGetVar(wp, "als_page", "");
    if(strlen(page) > 0 ) change = 1;
    else return change;
    
    data = websGetVar(wp, "alias", "");
    ezplib_replace_attr("als_serial_rule",atoi(page),"alias",data);

    data = websGetVar(wp, "baudrate", "");
    ezplib_replace_attr("als_serial_rule",atoi(page),"baudrate",data);

    data = websGetVar(wp, "databit", "");
    ezplib_replace_attr("als_serial_rule",atoi(page),"databit",data);

    data = websGetVar(wp, "stopbit", "");
    ezplib_replace_attr("als_serial_rule",atoi(page),"stopbit",data);

    data = websGetVar(wp, "parity", "");
    ezplib_replace_attr("als_serial_rule",atoi(page),"parity",data);

    data = websGetVar(wp, "flow", "");
    ezplib_replace_attr("als_serial_rule",atoi(page),"flow",data);

    data = websGetVar(wp, "als_fifo", "");
    ezplib_replace_attr("als_serial_rule",atoi(page),"fifo",data);


    data = websGetVar(wp, "mode", "");
    ezplib_replace_attr("als_operating_rule",atoi(page),"mode",data);

    data = websGetVar(wp, "alive", "");
    ezplib_replace_attr("als_operating_rule",atoi(page),"alive",data);

    data = websGetVar(wp, "inactive", "");
    ezplib_replace_attr("als_operating_rule",atoi(page),"inactive",data);

    data = websGetVar(wp, "max_connect", "");
    ezplib_replace_attr("als_operating_rule",atoi(page),"max_connect",data);

    data = websGetVar(wp, "jammed_ip", "");
    ezplib_replace_attr("als_operating_rule",atoi(page),"jammed_ip",data);


    data = websGetVar(wp, "length", "");
    ezplib_replace_attr("als_operating_data_packing_rule",atoi(page),"length",data);

    data = websGetVar(wp, "transmit", "");
    ezplib_replace_attr("als_operating_data_packing_rule",atoi(page),"transmit",data);


    data = websGetVar(wp, "tcp_port", "");
    if(data)
        ezplib_replace_attr("als_operating_option_rule",atoi(page),"tcp_port",data);

    data = websGetVar(wp, "ip_1", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_1",data);

    data = websGetVar(wp, "ip_1_1", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_1_1",data);

    data = websGetVar(wp, "ip_port_1", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_port_1",data);

    data = websGetVar(wp, "ip_2", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_2",data);

    data = websGetVar(wp, "ip_2_1", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_2_1",data);

    data = websGetVar(wp, "ip_port_2", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_port_2",data);

    data = websGetVar(wp, "ip_3", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_3",data);

    data = websGetVar(wp, "ip_3_1", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_3_1",data);

    data = websGetVar(wp, "ip_port_3", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_port_3",data);

    data = websGetVar(wp, "ip_4", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_4",data);
   
    data = websGetVar(wp, "ip_4_1", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_4_1",data);

    data = websGetVar(wp, "ip_port_4", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"ip_port_4",data);

    data = websGetVar(wp, "port_1", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"port_1",data);

    data = websGetVar(wp, "port_2", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"port_2",data);

    data = websGetVar(wp, "port_3", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"port_3",data);

    data = websGetVar(wp, "port_4", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"port_4",data);

    data = websGetVar(wp, "udp_port", "");
    ezplib_replace_attr("als_operating_option_rule",atoi(page),"udp_port",data);

    config_postaction(map, s, "", "");
    return change;
}


int save_alert(webs_t wp, char *value, struct variable *v, struct service *s)
{

    char *data;
    int64_t map = 0;
    
    config_preaction(&map, v, s, "", "");

    data = websGetVar(wp, "battery", "");
    ezplib_replace_attr("mail_rule", 0 ,"battery",(!strcmp(data,"on") ? "1" : "0"));

    data = websGetVar(wp, "sd", "");
    ezplib_replace_attr("mail_rule", 0 ,"sd",(!strcmp(data,"on") ? "1" : "0"));

    data = websGetVar(wp, "usb", "");
    ezplib_replace_attr("mail_rule", 0 ,"usb",(!strcmp(data,"on") ? "1" : "0"));

    data = websGetVar(wp, "relay_opt", "");
    ezplib_replace_attr("mail_rule", 0 ,"relay",(!strcmp(data,"on") ? "1" : "0"));

    data = websGetVar(wp, "gpio", "");
    ezplib_replace_attr("mail_rule", 0 ,"gpio",(!strcmp(data,"on") ? "1" : "0"));

    data = websGetVar(wp, "sms", "");
    ezplib_replace_attr("mail_rule", 0 ,"sms",(!strcmp(data,"on") ? "1" : "0"));

    data = websGetVar(wp, "mail", "");
    ezplib_replace_attr("mail_rule", 0 ,"mail",(!strcmp(data,"on") ? "1" : "0"));

    data = websGetVar(wp, "address_1", "");
    ezplib_replace_attr("mail_rule", 0 ,"address_1",data);

    data = websGetVar(wp, "address_2", "");
    ezplib_replace_attr("mail_rule", 0 ,"address_2",data);

    data = websGetVar(wp, "phone", "");
    ezplib_replace_attr("mail_rule", 0 ,"phone",data);

    data = websGetVar(wp, "email", "");
    ezplib_replace_attr("mail_rule", 0 ,"email",data);

    data = websGetVar(wp, "send_mail", "");
    ezplib_replace_attr("mail_rule", 0 ,"send_mail",data);

    data = websGetVar(wp, "server", "");
    ezplib_replace_attr("mail_rule", 0 ,"server",data);

    data = websGetVar(wp, "act", "");
    ezplib_replace_attr("mail_rule", 0 ,"act",data);

    data = websGetVar(wp, "pwd", "");
    ezplib_replace_attr("mail_rule", 0 ,"pwd",data);

    config_postaction(map, s, "", "");
    return 1;
}

