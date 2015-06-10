#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>

#include "ezp.h"
#include "ezp-lib.h"
#include "lang.h"

#include "fcgi_common.h"
#include "fcgi_stdio.h"

int
ej_widget_start(int eid, INPUT *input, int argc, char_t **argv)
{
    /* If display rule is 0, do not show widget */
    if(!atoi(nvram_safe_get("display_rule"))) {
        return 0;
    }
    printf("setTimeout('widget_query(F);', 3000);");
    return 0;
}

