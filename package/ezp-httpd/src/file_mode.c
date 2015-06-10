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

int valid_file_mode(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int save_file_mode(webs_t wp, char *value, struct variable *v, struct service *s)
{

    char *data;
    int64_t map = 0;
    
    int change = 1;

    config_preaction(&map, v, s, "", "");
    
    data = websGetVar(wp, "file_enable", "");
    ezplib_replace_attr("file_mode_rule",0,"enable",data);

    data = websGetVar(wp, "file_sd", "");
    ezplib_replace_attr("file_mode_rule",0,"sd",data);

    data = websGetVar(wp, "file_usb", "");
    ezplib_replace_attr("file_mode_rule",0,"usb",data);

    data = websGetVar(wp, "file_record_1", "");
    ezplib_replace_attr("file_mode_rule",0,"record_1",data);

    data = websGetVar(wp, "file_record_2", "");
    ezplib_replace_attr("file_mode_rule",0,"record_2",data);

    data = websGetVar(wp, "file_record_3", "");
    ezplib_replace_attr("file_mode_rule",0,"record_3",data);

    config_postaction(map, s, "", "");
    return change;
}


