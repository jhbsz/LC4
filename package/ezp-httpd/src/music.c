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
    MUSIC_ENABLE = 0,
};

static struct variable music_variables[] = {
    {longname: "Music Enable", argv:ARGV("0", "1"), nullok: FALSE},
};

int
valid_music(webs_t wp, char *value, struct variable *v)
{
    char tmp[TMP_LEN];    
    char *val;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "music_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &music_variables[MUSIC_ENABLE]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_music(webs_t wp, char *value, struct variable *v, struct service *s)
{
    char buf[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
    char *enable;
    int change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "music_enable");
    enable = websGetVar(wp, tmp, "");

    /* save music_rule */
    ezplib_get_rule("music_rule", 0, tmp, sizeof(tmp));
    snprintf(buf, sizeof(buf), "%s",
            enable);

    if (strcmp(buf, tmp)) {
        config_preaction(&map, v, s, "", ""); 
        ezplib_replace_rule("music_rule", 0, buf);
        ezplib_replace_rule("bw_enable", 0, buf);
        config_postaction(map, s, "", "");
        change = 1;
    }

    return change;
}
