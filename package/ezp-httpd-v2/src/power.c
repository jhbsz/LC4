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
enum con_type{
    CH341=0
};

enum {
    W_ENABLE = 0,
    W_COUNTDOWN,
    P_ENABLE,
    P_COUNTDOWN,
};

static struct variable power_variables[] = {
    {longname: "Wireless Auto Disable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "Wireless Auto Disable interval", argv:ARGV("10", "60"), nullok: FALSE},
    {longname: "System Auto Turn off", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "System Auto Turn off interval", argv:ARGV("10", "60", "120"), nullok: FALSE},
};

int
valid_power(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

#ifndef EZP_SUB_BRAND_SONY 
    /* Wireless Auto Disable */
    snprintf(tmp, sizeof(tmp), "w_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &power_variables[W_ENABLE]) == FALSE) {
        return FALSE;
    }
    
    /* Wireless interval */
    snprintf(tmp, sizeof(tmp), "w_countdown");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &power_variables[W_COUNTDOWN]) == FALSE) {
        return FALSE;
    }
#endif

    /* System Auto Turn off */
    snprintf(tmp, sizeof(tmp), "p_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &power_variables[P_ENABLE]) == FALSE) {
        return FALSE;
    }
    
    /* System interval */
    snprintf(tmp, sizeof(tmp), "p_countdown");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &power_variables[P_COUNTDOWN]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_power(webs_t wp, char *value, struct variable *v,
        struct service *s)
{
    char tmp[TMP_LEN], buf[TMP_LEN];
    char *w_enable, *w_countdown, *p_enable, *p_countdown;
    int idx = atoi(value), len, change = 0;

#ifndef EZP_SUB_BRAND_SONY 
    /* Wireless Auto Disable */
    snprintf(tmp, sizeof(tmp), "w_enable");
    w_enable = websGetVar(wp, tmp, "");

    /* Wireless interval */
    snprintf(tmp, sizeof(tmp), "w_countdown");
    w_countdown = websGetVar(wp, tmp, "");
#else
    char tmp_w_enable[TMP_LEN], tmp_w_countdown[TMP_LEN];
    ezplib_get_attr_val("countdown_rule", 0, "w_enable", tmp_w_enable, sizeof(tmp_w_enable), EZPLIB_USE_CLI);
    w_enable=tmp_w_enable;
    ezplib_get_attr_val("countdown_rule", 0, "w_countdown", tmp_w_countdown, sizeof(tmp_w_countdown), EZPLIB_USE_CLI);
    w_countdown=tmp_w_countdown;
#endif

    /* System Auto Turn off */
    snprintf(tmp, sizeof(tmp), "p_enable");
    p_enable = websGetVar(wp, tmp, "");

    /* System interval */
    snprintf(tmp, sizeof(tmp), "p_countdown");
    p_countdown = websGetVar(wp, tmp, "");

    len = snprintf(tmp, TMP_LEN, "%s^%s^%s^%s^%s^%s", w_enable, w_countdown, w_countdown, p_enable, p_countdown, p_countdown);

    ezplib_get_rule("countdown_rule", idx, buf, TMP_LEN);
    if (strcmp(buf, tmp)) {
        ezplib_replace_rule("countdown_rule", idx, tmp);
        change = 1;
    }

    return change;
}

int
valid_togopower(webs_t wp, char *value, struct variable *v)
{
    return TRUE;
}

int
save_togopower(webs_t wp, char *value, struct variable *v, struct service *s)
{
    int fd = socket(AF_INET,SOCK_DGRAM,0);  
    struct sockaddr_in addr={};  
    char *type, *id, *togo_action;
    char contype; 
    char tmp[LONG_BUF_LEN], output[LONG_BUF_LEN];
    unsigned char checksum;
    addr.sin_family = AF_INET;  
    addr.sin_port = htons(6666);  
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");  

    snprintf(tmp, sizeof(tmp), "type");
    type = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "id");
    id = websGetVar(wp, tmp, "");
    snprintf(tmp, sizeof(tmp), "togo_action");
    togo_action = websGetVar(wp, tmp, "");
    checksum=atoi(togo_action)+0xa6;
    if (!strcmp(type,"ch341")) contype=CH341;
    snprintf(output, sizeof(output), "type=%d,id=%s,action=%2s,checksum=%2X,", contype, id, togo_action, checksum);
    sendto(fd,&output,strlen(output)+1,0,(struct sockaddr*)&addr,sizeof(addr));  
    close(fd);  

    return 0;
}
