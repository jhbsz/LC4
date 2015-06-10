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

enum {
    SM_MODE = 0,
};

static struct variable session_manager_variables[] = {
    {longname: "Session Manager Mode", argv:ARGV("10"), nullok: FALSE},
};

int
valid_session_manager(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Session Manager Mode */
    snprintf(tmp, sizeof(tmp), "bw_sm_mode");
    val = websGetVar(wp, tmp, "");
    if (valid_word(wp, val, &session_manager_variables[SM_MODE]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_session_manager(webs_t wp, char *value, struct variable *v, struct service *services)
{
    char tmp[TMP_LEN];    
    char *mode;
    char *rule_set = "sm_mode";
    int len, change = 0;
    int64_t map = 0;

    /* Session Manager Mode */
    snprintf(tmp, sizeof(tmp), "bw_sm_mode");
    mode = websGetVar(wp, tmp, "");

    /* Construct a rule tuple. */
    len = snprintf(tmp, TMP_LEN, "%s", mode);

    if (len > TMP_LEN - 1) {
        return 0;
    }

    if (strcmp(tmp, nvram_safe_get(rule_set))) {
        config_preaction(&map, v, services, "", ""); 
        nvram_set(rule_set, tmp);
        config_postaction(map, services, "", "");
        change = 1;
    }

    return change;
}
