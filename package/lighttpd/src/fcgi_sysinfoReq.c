#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <stdlib.h>
#include <string.h>
#include "fcgi_nas_cgiutil.h"
#include "fcgi_common.h"

int sysinfo_req(INPUT *input)
{
    INPUT *tmp;

    if ((tmp = CGI_Find_Parameter(input, "appsjson"))) {
        FCGI_LOG("appsjson = %s\n", tmp->val);
    }

    if ((tmp = CGI_Find_Parameter(input, "sid")))
        FCGI_LOG("sid = %s\n", tmp->val);

    Sysinfo_To_JSON(2);
    CGI_RELEASE(input);
    return 0;
}
