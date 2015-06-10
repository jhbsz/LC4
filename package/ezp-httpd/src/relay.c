#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include "assert.h"

#include "common.h"
#include "ezp-lib.h"
#include "ezp.h"

int valid_relay(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int valid_alert(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}
int save_relay(webs_t wp, char *value, struct variable *v, struct service *s)
{

    char *data;
    char tmp[24];
    char *option;
    int64_t map = 0;
    
    int change = 1;

    config_preaction(&map, v, s, "", "");
    
    option = websGetVar(wp, "relay_option", "");
    if(option)
        ezplib_replace_attr("relay_rule",0,"option",option);

    data = websGetVar(wp, "relay_1_enable", "");
    if(data){
        ezplib_get_attr_val("relay_rule", 0, "enable", tmp, sizeof(tmp), EZPLIB_USE_CLI);
        if(strcmp(data,tmp)){
            ezplib_replace_attr("relay_rule",0,"enable",data);
            
            if (atoi(data) == 0)
                system("/bin/sh /etc/init.d/send_mail relay 1 Off");
            if (atoi(data) == 1)
                system("/bin/sh /etc/init.d/send_mail relay 1 On");
        }
    }

    data = websGetVar(wp, "relay_2_enable", "");
    if(data){
        ezplib_get_attr_val("relay_rule", 1, "enable", tmp, sizeof(tmp), EZPLIB_USE_CLI);
        if(strcmp(data,tmp)){
            ezplib_replace_attr("relay_rule",1,"enable",data);
            if (atoi(data) == 0)
                system("/bin/sh /etc/init.d/send_mail relay 2 Off");
            if (atoi(data) == 1)
                system("/bin/sh /etc/init.d/send_mail relay 2 On");
        }
    }

    data = websGetVar(wp, "schedule_1_sun", "");
        ezplib_replace_attr("relay_schedule_rule",0,"sun",data);

    data = websGetVar(wp, "schedule_1_mon", "");
        ezplib_replace_attr("relay_schedule_rule",0,"mon",data);
    
    data = websGetVar(wp, "schedule_1_tue", "");
        ezplib_replace_attr("relay_schedule_rule",0,"tue",data);
    
    data = websGetVar(wp, "schedule_1_wed", "");
        ezplib_replace_attr("relay_schedule_rule",0,"wed",data);
    
    data = websGetVar(wp, "schedule_1_thu", "");
        ezplib_replace_attr("relay_schedule_rule",0,"thu",data);
    
    data = websGetVar(wp, "schedule_1_fri", "");
        ezplib_replace_attr("relay_schedule_rule",0,"fri",data);
    
    data = websGetVar(wp, "schedule_1_sat", "");
        ezplib_replace_attr("relay_schedule_rule",0,"sat",data);

    data = websGetVar(wp, "schedule_2_sun", "");
        ezplib_replace_attr("relay_schedule_rule",1,"sun",data);
    
    data = websGetVar(wp, "schedule_2_mon", "");
        ezplib_replace_attr("relay_schedule_rule",1,"mon",data);
    
    data = websGetVar(wp, "schedule_2_tue", "");
        ezplib_replace_attr("relay_schedule_rule",1,"tue",data);
    
    data = websGetVar(wp, "schedule_2_wed", "");
        ezplib_replace_attr("relay_schedule_rule",1,"wed",data);
    
    data = websGetVar(wp, "schedule_2_thu", "");
        ezplib_replace_attr("relay_schedule_rule",1,"thu",data);
    
    data = websGetVar(wp, "schedule_2_fri", "");
        ezplib_replace_attr("relay_schedule_rule",1,"fri",data);
    
    data = websGetVar(wp, "schedule_2_sat", "");
        ezplib_replace_attr("relay_schedule_rule",1,"sat",data);

    config_postaction(map, s, "", "");
    return change;
}

