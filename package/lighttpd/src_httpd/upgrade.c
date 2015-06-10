/*
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/reboot.h>

#include "ezp.h"

#include "fcgi_common.h"
#include "fcgi_stdio.h"
#include "http_common.h"

#define CODE_PATTERN_ERROR 9999
#define CONFIG  1
#define FIRMWARE    2

static void start_upgrade_indicator(void) {
#if defined(PLATFORM_LIS)
    system("gpioctl led 13 1 0 0 0 0");
#elif defined(PLATFORM_M2E)
    system("/usr/sbin/ezp-i2c gauge upgrade start");
    system("source /etc/arch.sh ; arch_led_start boot");
#elif defined(PLATFORM_AXA)
    system("source /etc/arch.sh ; arch_led_start boot");
#else
    system("source /etc/arch.sh ; arch_led_start boot");
#endif
}

int
do_upgrade(INPUT *input)
{
    char upload_file[] = "/tmp/ezp_firmware_XXXXXX";
    char *write_argv[4];
    pid_t pid;
    int ret=0;
    int upgrade_cmd_complete = 0;
    struct stat st;

    system("/sbin/stop_services.sh");
    start_upgrade_indicator();

    write_argv[0] = "/usr/sbin/upgraded";
    write_argv[1] = upload_file;
    write_argv[2] = NULL;

    _eval(write_argv, NULL, 0, &pid);
    upgrade_cmd_complete = 1;
    while((stat("/tmp/fw_incorrect",&st) != 0) && (stat("/tmp/fw_correct",&st) != 0)){
        usleep(200000);
    }
    if(stat("/tmp/fw_incorrect", &st) == 0) ret = 1;
    else if(stat("/tmp/fw_correct", &st) == 0) ret = 0;
FCGI_LOG("done with ret:%d", ret);
    if(!upgrade_cmd_complete) {
        system("/tmp/ezp-i2c gauge upgrade finish");
    }
    return ret;
}

void
do_upgrade_cgi(INPUT *input)
{
    int upgrade_result = do_upgrade(input);
    if (upgrade_result == 1) {
FCGI_LOG("fail");
        do_ej("resp_upgrade_fail.asp", input, "");
        sleep(5);

        system("ifconfig br0 down");
        system("ifconfig ra0 down");
        system("ifconfig apcli0 down");
        system("ifconfig vlan1 down");
        system("ifconfig vlan2 down");
        system("ifconfig eth2 down");
        reboot(RB_AUTOBOOT);
    } else {
FCGI_LOG("success");
        /* Reboot if successful. */
        do_ej("resp_upgrade_success.asp", input, "");
    }
}

