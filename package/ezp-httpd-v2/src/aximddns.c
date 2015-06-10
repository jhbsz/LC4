#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include "common.h"
#include "ezp.h"
#include "ezp-lib.h"

enum {
    USER_DDNS_ENABLE = 0,
    USER_DDNS_USERNAME,
};

static struct variable wan_aximddns_variables[] = {
    {longname: "AXIM DDNS Enable", argv:ARGV("0", "1")},
    {longname: "AXIM DDNS User Name", argv:ARGV("30")},
};

char aximddns_message[256]={0};

extern int reboot_action;

int
valid_aximddns(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];
    char *enable, *username;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "axim_ddns_enable");
    enable = websGetVar(wp, tmp, "");
    if (valid_choice(wp, enable, &wan_aximddns_variables[USER_DDNS_ENABLE]) ==  FALSE )
    {
       return FALSE;
    }
    if ( *enable == '0') 
    {
        /* Validate the following entities only when DDNS is enabled. */
       return TRUE;
    }

    /* User Name */
    snprintf(tmp, sizeof(tmp), "aximddns_user_name");
    username = websGetVar(wp, tmp, "");
    if (valid_name(wp, username, &wan_aximddns_variables[USER_DDNS_USERNAME]) == FALSE ) 
    {
        return FALSE;
    }

    return TRUE;
}

int
save_aximddns(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char tmp[TMP_LEN];
    char *rule_set = "aximddns_rule";
    char *rule_tmp_set = "aximddns_tmp_rule";
    char *enable, *web_username,*retp = NULL ,tmp_buffer[256]={0}  ,*p = NULL;
    int  change = 0;
    int64_t map;
    char logfile[]="/bin/cat /tmp/aximddns_ret_msg";
    FILE *pipe = NULL;

    map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "axim_ddns_enable");
    enable = websGetVar(wp, tmp, "");

    if (*enable == '0') {
        ezplib_get_attr_val(rule_set, 0, "user_enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        if (strcmp(tmp, enable)) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_attr(rule_tmp_set, 0, "user_enable", enable);
            ezplib_replace_attr(rule_set, 0, "user_enable", enable);
            change = 1;
        }
    } else {

        ezplib_get_attr_val(rule_set, 0, "user_enable", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
        if (strcmp(tmp, enable)) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_attr(rule_tmp_set, 0, "user_enable", enable);
            change = 1;
        }


        /* User Name */
        snprintf(tmp, sizeof(tmp), "aximddns_user_name");
        web_username = websGetVar(wp, tmp, "");

        ezplib_get_attr_val(rule_set, 0, "user_name", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        if (strcmp(tmp, web_username)) {
            config_preaction(&map, v, s, "NUM=0", "");
            ezplib_replace_attr(rule_tmp_set, 0, "user_name", web_username);
            change = 1;
        }


    }
    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }

    pipe = popen(logfile, "r");
    if(pipe){
        do{
        
            retp = fgets(tmp_buffer,sizeof(tmp_buffer), pipe);
            if(retp != NULL) {
                if((p = strstr(tmp_buffer, "Setting succes"))!= 0){
                    reboot_action = 0;
                    break;
                }
                if((p = strstr(tmp_buffer, "Error Message"))!= 0){
                    reboot_action = 3;
                    p = strtok(tmp_buffer,":");
                    p = strtok( NULL,":");
                    sprintf(aximddns_message,"%s",p);
                    break;
                }

            }
        } while (retp);
    	pclose(pipe);      
    }

    return change;
}


