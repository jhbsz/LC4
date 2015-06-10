#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nas_cgiutil.h"
#include "common.h"

int main(void)
{
    INPUT *input = CGI_INIT();
    INPUT *tmp;

    if ((tmp = CGI_Find_Parameter(input, "appsjson")))
        fprintf(stderr, "appsjson = %s\n", tmp->val);

    if ((tmp = CGI_Find_Parameter(input, "sid")))
        fprintf(stderr, "sid = %s\n", tmp->val);

    Sysinfo_To_JSON(2);
    CGI_RELEASE(input);
    return 0;
}
