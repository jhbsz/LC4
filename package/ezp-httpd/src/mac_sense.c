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

int valid_mac_sense(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int save_mac_sense(webs_t wp, char *value, struct variable *v, struct service *s)
{

    char *data;
    int64_t map = 0;
    
    int change = 1;

    config_preaction(&map, v, s, "", "");
    
    data = websGetVar(wp, "server_ip", "");
    ezplib_replace_attr("mac_sense_rule",0,"server_ip",data);

    data = websGetVar(wp, "server_port", "");
    ezplib_replace_attr("mac_sense_rule",0,"server_port",data);

    data = websGetVar(wp, "server_time", "");
    ezplib_replace_attr("mac_sense_rule",0,"server_time",data);

    data = websGetVar(wp, "server_timeout", "");
    ezplib_replace_attr("mac_sense_rule",0,"server_timeout",data);

    data = websGetVar(wp, "wds_enable", "");
    ezplib_replace_attr("mac_sense_rule",0,"wds_enable",data);

    data = websGetVar(wp, "remote_ip", "");
    ezplib_replace_attr("mac_sense_rule",0,"remote_ip",data);

    config_postaction(map, s, "", "");
    return change;
}


