#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <bcmnvram.h>
#include <fcntl.h>
#include "ezp-lib.h"
#include <sys/time.h>
#include <sys/stat.h>

    
#define BUF_LEN     1024
#define skip_space(ptr) while (*ptr == ' ') { ptr++; }

static void
sig_handle(int sig)
{
    /* Ignore the signal */
    printf("Received a signal (%d).\n", sig);
    exit(0);
    return;
}
int decide_dot(char *ptr) {
    char *dotptr = strstr(ptr, ".");
    char *prevptr;
    while(dotptr) {
        prevptr = dotptr - 1;
        dotptr ++;
        if((*prevptr > '9') || (*prevptr < '0') || (*dotptr > '9') || (*dotptr < '0')) {
            /* Found a  "." but not belong to a number */
            return 1;
        }
        dotptr = strstr(dotptr, ".");
    }
    return 0;
}
int decide_minus(char *ptr) {
    char *tokptr = strstr(ptr, "-");
    char *prevptr;
    while(tokptr) {
        prevptr = tokptr - 1;
        tokptr ++;
        if((*prevptr != ' ') || (*tokptr > '9') || (*tokptr < '0')) {
            /* Found a  "-" but not belong to a number */
            return 1;
        }
        tokptr = strstr(tokptr, "-");
    }
    return 0;
}
int
main()
{
    int i;

    /* Catch all signals except SIGKILL. */
    for (i = 0; i < 15; i++) {
        signal(i, sig_handle);
    }

    while (1) {
        FILE *fp;
        char buf1[BUF_LEN], buf2[BUF_LEN];
        char *ptr, *ptr2;
#if defined(PLATFORM_LIS)
        printf("Mosra> ");
#else
        printf("ALLISCOM> ");
#endif
        ptr = fgets(buf1, sizeof(buf1), stdin);
        skip_space(ptr);

        if (*ptr == '\n') {
            continue;
        }

        /* If it is authenticated, perhibit all commands. */
        ptr2 = nvram_safe_get("license_invalid");
        if (*ptr2 == '0') {
            continue;
        }

        if (strncmp(ptr, "iwpriv", 6) && strncmp(ptr, "ated", 4) && 
            strncmp(ptr, "fw_version", 10) && strncmp(ptr, "ethernet", 8) &&
            strncmp(ptr, "gpio_stop", 9) && strncmp(ptr, "eeprom", 6) &&
            strncmp(ptr, "get_ssid", 8) && strncmp(ptr, "finalcheck", 10) &&
            strncmp(ptr, "mac", 3) && strncmp(ptr, "charger", 7) &&
            strncmp(ptr, "voltage", 7) && strncmp(ptr, "current", 7) &&
            strncmp(ptr, "percentage", 10) && strncmp(ptr, "set_ssid", 8) &&
            strncmp(ptr, "get_wifi_mac", 12) && strncmp(ptr, "storage", 7) &&
            strncmp(ptr, "mcu_version", 11) && strncmp(ptr, "gauge_fw", 8) &&
            strncmp(ptr, "usb_test", 8) && strncmp(ptr, "card_test", 9) &&
            strncmp(ptr, "internal_test", 13) &&
            strncmp(ptr, "endpoint_test", 13) && strncmp(ptr, "factory", 7) &&
            strncmp(ptr, "iperf_server", sizeof("iperf_server")-1) && 
            strncmp(ptr, "stop_iperf", sizeof("stop_iperf")-1) && 
            strncmp(ptr, "iperf_client", sizeof("iperf_client")-1) && 
            strncmp(ptr, "charger_status", 14) &&
            strncmp(ptr, "audio_test", 10) &&
            strncmp(ptr, "usb_throughput", 14) &&
            strncmp(ptr, "internal_throughput", 19) &&
            strncmp(ptr, "card_throughput", 15) &&
            strncmp(ptr, "get_serial_num", 14) &&
            strncmp(ptr, "set_serial_num", 14) &&
            strncmp(ptr, "get_mfg_test_result", 19) &&
            strncmp(ptr, "set_mfg_test_result", 19) &&
            strncmp(ptr, "w_red_led_on", 12) && 
            strncmp(ptr, "w_red_led_off", 13) && 
            strncmp(ptr, "w_green_led_resume", 18) && 
            strncmp(ptr, "w_green_led_off", 15) && 
            strncmp(ptr, "p_green_led_on", 14) && 
            strncmp(ptr, "p_green_led_off", 15) && 
            strncmp(ptr, "p_led_resume", 12) && 
            strncmp(ptr, "p_red_led_on", 12) &&
            strncmp(ptr, "p_red_led_off", 13) &&
            strncmp(ptr, "p_oled_on", 9) &&
            strncmp(ptr, "p_oled_off", 10) &&
            strncmp(ptr, "p_wm_led_on", 11) &&
            strncmp(ptr, "p_wm_led_off", 12) &&
            strncmp(ptr, "p_pm_led_on", 11) &&
            strncmp(ptr, "p_pm_led_off", 12) &&
            strncmp(ptr, "sec_status", 10) &&
            strncmp(ptr, "usb_status", 10) &&
            strncmp(ptr, "gpio_test", 9) &&
            strncmp(ptr, "led_test", 8) &&
            strncmp(ptr, "wlan_led_test_set", 17) &&
            strncmp(ptr, "wlan_led_test_clear", 19) &&
            strncmp(ptr, "p_blue_led_on", 13) &&
            strncmp(ptr, "set_ssid_mac", 12) && 
            strncmp(ptr, "tcpdump", 7) && 
            strncmp(ptr, "p_blue_led_off", 14) &&
            strncmp(ptr, "switch_lan", 10) &&
            strncmp(ptr, "switch_wan", 10) &&
            strncmp(ptr, "temp_offset", sizeof("temp_offset") - 1) &&
            strncmp(ptr, "temp", sizeof("temp") - 1) &&
            strncmp(ptr, "switch_wan", 10) &&
            strncmp(ptr, "resetbc", sizeof("resetbc") - 1) &&
            strncmp(ptr, "boot_test", sizeof("boot_test") - 1) &&
            strncmp(ptr, "exit", 4) &&
            strncmp(ptr, "quit", 4)){
            printf("Illegal command: %s", ptr);
            continue;
        }

        if (!strncmp(ptr, "set_ssid_mac", 12)) {
            FILE *pFile;
            char ssid[BUF_LEN];
	    char tmp[BUF_LEN];
	    char hwaddr[BUF_LEN], mac[BUF_LEN]={0};
            char *tmp_str, i=0;
            char target[]=":";

            if ((pFile = popen("cat /proc/ezp_machine_id", "r"))) {
                fgets(hwaddr, sizeof(hwaddr), pFile);
            }
            pclose(pFile);
            tmp_str = strtok(hwaddr,target);
            i=2;
            while(tmp_str != NULL)
            {
                tmp_str = strtok( NULL,target);
                if(tmp_str != NULL)
                {
                    if(i>3)
                    {
                        sprintf(mac,"%s%s",mac,tmp_str);
                    }
                }
                i++;
            }
            if(mac[strlen(mac) - 1] == '\n' || mac[strlen(mac) - 1] == '\t' || mac[strlen(mac) - 1] == ' ') mac[strlen(mac) - 1] = 0;

            sscanf(ptr,"set_ssid_mac%s\n",ssid);
            snprintf(tmp,BUF_LEN,"nvram fset wl0_ssid_rule_default=\"%s-%s\"",ssid,mac);
            pFile = popen(tmp, "r");
            snprintf(tmp,BUF_LEN,"nvram fset wl0_ssid_rule=\"%s-%s\"",ssid,mac);
            pclose(pFile);
            pFile = popen(tmp, "r");
            system("nvram commit");
            printf("SSID:%s-%s\n",ssid,mac);
            pclose(pFile);
            continue;
        }

        if (!strncmp(ptr, "set_ssid", 8)) {
            FILE *pFile;
            char ssid[BUF_LEN];
            char tmp[BUF_LEN];
            sscanf(ptr,"set_ssid%s\n",ssid);
            snprintf(tmp,BUF_LEN,"nvram fset wl0_ssid_rule_default=\"%s\"",ssid);
            pFile = popen(tmp, "r");
            snprintf(tmp,BUF_LEN,"nvram fset wl0_ssid_rule=\"%s\"",ssid);
            pclose(pFile);
            pFile = popen(tmp, "r");
            system("nvram commit");
            printf("SSID:%s\n",ssid);
            pclose(pFile);
            continue;
        }

        /* Don't allow these char */
        if (strtok(ptr, "`;\"'?<>|*&^%$@!\\+") && strtok(NULL, "`;\"'?<>|*&^%$@!\\+")) {
            printf("Illegal character: %s", ptr);
            continue;
        }
        if(decide_dot(ptr) || decide_minus(ptr)) {
            printf("Illegal character: %s", ptr);
            continue;
        }
        if (!strncmp(ptr, "fw_version", 10)) {
            char fw_version[BUF_LEN];
#if defined(PLATFORM_LIS)
                ezplib_get_rule("custom_fw_version", 0, fw_version, BUF_LEN);
#else
                ezplib_get_rule("fw_version", 0, fw_version, BUF_LEN);
#endif

            printf("FW Version:%s\n",fw_version);
            continue;
        }
        if (!strncmp(ptr, "storage", 7)) {
            char tmp[BUF_LEN]={0}, tmp2[BUF_LEN]={0};
            FILE *file;
            if ((file = popen("mount | grep USB", "r"))) {
                fgets(tmp, sizeof(tmp), file);
                pclose(file);
            }
            if (strcmp(tmp, "")) { 
                printf("USB:Ready\n");
            } else {
                printf("USB:Not Ready\n");
            }

            if ((file = popen("mount | grep Qsync", "r"))) {
                fgets(tmp, sizeof(tmp), file);
                pclose(file);
            }
            if (strcmp(tmp, "")) { 
                printf("INTERNAL:Ready\n");
            } else {
                printf("INTERNAL:Not Ready\n");
            }

            if ((file = popen("mount | grep SD", "r"))) {
                fgets(tmp2, sizeof(tmp2), file);
                pclose(file);
            }
            if (strcmp(tmp2, "")) { 
                printf("CARD:Ready\n");
            } else {
                printf("CARD:Not Ready\n");
            }

            // terminate
            continue;
        }

        if (!strncmp(ptr, "usb_status", 10)) {
            FILE *file;
            char result[64];
            if ((file = popen("cat /proc/bus/usb/devices|grep  Vendor= -c", "r"))) {
                fgets(result, sizeof(result), file);
                pclose(file);
            }
#if defined(PLATFORM_AXB) || defined(PLATFORM_AXM)
            if (!strncmp(result, "3", 1)) {
#elif defined(PLATFORM_AXA) 
            if (!strncmp(result, "5", 1)) {
#else
            if (0) {
#endif
                printf("USB:Pass\n");
            } else {
                printf("USB:Fail\n");
            }
            continue;
        }

        if (!strncmp(ptr, "sec_status", 10)) {
            FILE *pFile;
            long lSize;
            char *buffer;
            size_t result;
            pFile = popen ( "ezp-i2c security", "r");
            pclose (pFile);
            pFile = fopen ( "/tmp/ezp-i2c_security" , "rb" );
            if (pFile==NULL) {
                printf("No data\n");
                continue;
            }
            // obtain file size:
            fseek (pFile , 0 , SEEK_END);
            lSize = ftell (pFile);
            rewind (pFile);
            // allocate memory to contain the whole file:
            buffer = (char*) malloc (sizeof(char)*lSize);
            if (buffer == NULL) {
                printf("Memory error\n");
                continue;
            }
            // copy the file into the buffer:
            result = fread (buffer,1,lSize,pFile);
            if (result != lSize) {
                printf("Reading error\n");
                continue;
            }
            /* the whole file is now loaded in the memory buffer. */
            printf("%s\n",buffer);
            // terminate
            fclose (pFile);
            free (buffer);
            continue;
        }

        if (!strncmp(ptr, "gauge_fw", 8)) {
            FILE *pFile;
            char result[64];
            int version[1];
            pFile = popen("ezp-i2c gauge fw", "r");
            fgets(result, sizeof(result), pFile);
            pclose(pFile);
            sscanf(result,"Data Read: 0x%02X",&version[0]);
            printf("Gauge Version: %02d\n",version[0]);
            continue;
        }

        if (!strncmp(ptr, "mcu_version", 11)) {
            FILE *pFile;
            char result[64];
            int version[3];
            char platform[BUF_LEN], release[BUF_LEN];
            pFile = popen("ezp-i2c fw", "r");
            fgets(result, sizeof(result), pFile);
            pclose(pFile);
            sscanf(result,"Data Read: 0x%02X 0x%02X 0x%02X",&version[0],&version[1],&version[2]);
            if ( version[2] == 0x10 ) snprintf(platform,BUF_LEN,"Mosra");
            else if ( version[2] == 0x00 ) snprintf(platform,BUF_LEN,"M1");
            if ( version[0] == 0x01) snprintf(release,BUF_LEN,"A"); 
            else if ( version[0] == 0x02) snprintf(release,BUF_LEN,"B"); 
            else if ( version[0] == 0x03) snprintf(release,BUF_LEN,"C"); 
            else snprintf(release,BUF_LEN,"Unknown"); 
            printf("%s MCU Version: %s%02d\n",platform,release,version[1]);
            continue;
        }

        if (!strncmp(ptr, "get_ssid", 8)) {
            char ssid[BUF_LEN];
            ezplib_get_rule("wl0_ssid_rule", 0, ssid, BUF_LEN);
            printf("SSID:%s\n",ssid);
            continue;
        }

        if (!strncmp(ptr, "get_wifi_mac", 12)) {
            FILE *pFile;
            char hwaddr[64];
            if ((pFile = fopen("/proc/ezp_machine_id", "r"))) {
                fgets(hwaddr, sizeof(hwaddr), pFile);
                hwaddr[17] = '\0';
            }
            printf("WiFI MAC:%s\n",hwaddr);
            // terminate
            fclose (pFile);
            continue;
        }

        if (!strncmp(ptr, "ethernet", 8)) {
            FILE *pFile;
            int i=0;
            char tmp[BUF_LEN];
            for (i=0;i<=5;i++) {
                snprintf(tmp,BUF_LEN,"switch  mii w %d 0 0 800",i);
                pFile = popen(tmp, "r");
                pclose(pFile);
            }
            printf("Ethernet open\n");
            continue;
        }

        if (!strncmp(ptr, "gpio_stop", 9)) {
            system("rm /sbin/ezp-gpio");
            printf("GPIO Stop\n");
            continue;
        }

        if (!strncmp(ptr, "mac", 3)) {
            FILE *pFile;
            pFile = popen("/etc/rc.common /etc/init.d/lkey boot mac", "r");
            pclose(pFile);
            printf("MAC Write\n");
            continue;
        }
        if (!strncmp(ptr, "charger_status", 14)) {
            system("ezp-i2c charger status");
            continue;
        }

        if (!strncmp(ptr, "charger", 7)) {
            system("ezp-i2c charger");
            continue;
        }
        
        if (!strncmp(ptr, "voltage", 7)) {
            system("ezp-i2c gauge voltage");
            continue;
        }
        
        if (!strncmp(ptr, "current", 7)) {
            system("ezp-i2c gauge current");
            continue;
        }
        
        if (!strncmp(ptr, "percentage", 10)) {
            system("ezp-i2c gauge percentage");
            continue;
        }
        
        if (!strncmp(ptr, "eeprom", 6)) {
            FILE *pFile;
            int mac[6];
            char *mark;
            char tmp[BUF_LEN], wlv_rule_num[BUF_LEN], add_mac_num=3;
            ezplib_get_rule("wlv_rule_num", 0, wlv_rule_num, BUF_LEN);
            switch (atoi(wlv_rule_num)) {
                case 1:
                    add_mac_num=3;
                    break;
                case 2:
                default :
                    add_mac_num=5;
                    break;
            }
            mark=strstr(ptr,":");
            if(mark == NULL)
            {
                sscanf(ptr,"eeprom%02X%02X%02X%02X%02X%02X",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
            }
            else
            {
                sscanf(ptr,"eeprom%02X:%02X:%02X:%02X:%02X:%02X",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
            }
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 4=%02X%02X",mac[1],mac[0]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 6=%02X%02X",mac[3],mac[2]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 8=%02X%02X",mac[5],mac[4]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 28=%02X%02X",mac[1],mac[0]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 2a=%02X%02X",mac[3],mac[2]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 2c=%02X%02X",mac[5]+add_mac_num,mac[4]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 2e=%02X%02X",mac[1],mac[0]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 30=%02X%02X",mac[3],mac[2]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            snprintf(tmp,BUF_LEN,"iwpriv ra0 e2p 32=%02X%02X",mac[5]+add_mac_num-1,mac[4]);
            pFile = popen(tmp, "r");
            pclose(pFile);
            pFile = popen("/etc/rc.common /etc/init.d/lkey boot mac", "r");
            pclose(pFile);
            printf("EEPROM Write\n");
            continue;
        }
        if (!strncmp(ptr, "finalcheck", 10)) {
            FILE *pFile;
            long lSize;
            char *buffer;
            size_t result;
            pFile = popen("/etc/rc.common /etc/init.d/lkey boot security", "r");
            pclose(pFile);
            pFile = fopen ( "/tmp/ezp-i2c_security" , "rb" );
            if (pFile==NULL) {
                printf("No data\n");
                continue;
            }
            // obtain file size:
            fseek (pFile , 0 , SEEK_END);
            lSize = ftell (pFile);
            rewind (pFile);
            // allocate memory to contain the whole file:
            buffer = (char*) malloc (sizeof(char)*lSize);
            if (buffer == NULL) {
                printf("Memory error\n"); 
                continue;
            }
            // copy the file into the buffer:
            result = fread (buffer,1,lSize,pFile);
            if (result != lSize) {
                printf("Reading error\n");
                continue;
            }
            /* the whole file is now loaded in the memory buffer. */
            printf("%s\n",buffer);
            // terminate
            fclose (pFile);
            free (buffer);
            continue;
        }
        if (!strncmp(ptr, "usb_test", 8)) {
            FILE *pFile;
            char result[64]={0};
            pFile = fopen ( "/tmp/mnt/USB/mosra_test_a.bin" , "rb" );
            if (pFile==NULL) {
                printf("Cannot find test file mosra_test_a.bin\n");
                printf("Unmount USB!\n");
                system("umount -l /tmp/mnt/USB");
                continue;
            }
            fclose(pFile);
            system("cp /tmp/mnt/USB/mosra_test_a.bin /tmp/mosra_test_b.bin");
            system("cp /tmp/mosra_test_b.bin /tmp/mnt/USB");
            sync();
            if ((pFile = popen("diff -q /tmp/mnt/USB/mosra_test_a.bin /tmp/mnt/USB/mosra_test_b.bin", "r"))) {
                fgets(result, sizeof(result), pFile);
            }
            pclose(pFile);
            if (!strcmp(result, "")) {
                printf("USB test OK!!\n");
            } else {
                printf("USB test Fail!!\n");
            }
            system("rm /tmp/mnt/USB/mosra_test_b.bin");
            system("rm /tmp/mosra_test_b.bin");
            sync();
            printf("Unmount USB!\n");
            system("umount -l /tmp/mnt/USB");
            continue;
        }

        if (!strncmp(ptr, "internal_test", 13)) {
            FILE *pFile;
            char result[64]={0};
            pFile = fopen ( "/tmp/mnt/Qsync/mosra_test_a.bin" , "rb" );
            if (pFile==NULL) {
                printf("Cannot find test file mosra_test_a.bin\n");
                printf("Unmount Qsync!\n");
                system("umount -l /tmp/mnt/Qsync");
                continue;
            }
            fclose(pFile);
            system("cp /tmp/mnt/Qsync/mosra_test_a.bin /tmp/mosra_test_b.bin");
            system("cp /tmp/mosra_test_b.bin /tmp/mnt/Qsync");
            sync();
            if ((pFile = popen("diff -q /tmp/mnt/Qsync/mosra_test_a.bin /tmp/mnt/Qsync/mosra_test_b.bin", "r"))) {
                fgets(result, sizeof(result), pFile);
            }
            pclose(pFile);
            if (!strcmp(result, "")) {
                printf("Qsync test OK!!\n");
            } else {
                printf("Qsync test Fail!!\n");
            }
            system("rm /tmp/mnt/Qsync/mosra_test_b.bin");
            system("rm /tmp/mosra_test_b.bin");
            sync();
            printf("Unmount Qsync!\n");
            system("umount -l /tmp/mnt/Qsync");
            continue;
        }
        if (!strncmp(ptr, "card_test", 9)) {
            FILE *pFile;
            char result[64]={0};
            pFile = fopen ( "/tmp/mnt/SD/mosra_test_a.bin" , "rb" );
            if (pFile==NULL) {
                printf("Cannot find test file mosra_test_a.bin\n");
                printf("Unmount card!\n");
                system("umount -l /tmp/mnt/SD");
                continue;
            }
            fclose(pFile);
            system("cp /tmp/mnt/SD/mosra_test_a.bin /tmp/mosra_test_b.bin");
            system("cp /tmp/mosra_test_b.bin /tmp/mnt/SD");
            sync();
            if ((pFile = popen("diff -q /tmp/mnt/SD/mosra_test_a.bin /tmp/mnt/SD/mosra_test_b.bin", "r"))) {
                fgets(result, sizeof(result), pFile);
            }
            pclose(pFile);
            if (!strcmp(result, "")) {
                printf("Card test OK!!\n");
            } else {
                printf("Card test Fail!!\n");
            }
            system("rm /tmp/mnt/SD/mosra_test_b.bin");
            system("rm /tmp/mosra_test_b.bin");
            sync();
            printf("Unmount card!\n");
            system("umount -l /tmp/mnt/SD");
            continue;
        }

        if (!strncmp(ptr, "endpoint_test", 13)) {
            printf("\nChecking\n");
            int dirfilestate;
            struct stat dirfilebuf;

            dirfilestate = stat("/tmp/mnt/SD/endpoint/endpoint", &dirfilebuf);
            if ( dirfilestate == -1 ) {
                printf("open fail\n");
                continue;
            } else {
                if(dirfilebuf.st_size == 964664)
                    system("/tmp/mnt/SD/endpoint/endpoint");
            }
            continue;
        }
        
        if (!strncmp(ptr, "iperf_server", sizeof("iperf_server")-1)) {
            printf("\nChecking\n");
            int dirfilestate;
            struct stat dirfilebuf;

            dirfilestate = stat("/tmp/mnt/SD/iperf", &dirfilebuf);
            if ( dirfilestate == -1 ) {
                printf("open fail\n");
                continue;
            } else {
                if(dirfilebuf.st_size == 288207)
                    system("/tmp/mnt/SD/iperf -s &");
            }
            continue;
        }
        
        if (!strncmp(ptr, "stop_iperf", sizeof("stop_iperf")-1)) {
            system("kill -9 $(pidof iperf)");
            continue;
        }
        
        if (!strncmp(ptr, "iperf_client", sizeof("iperf_client")-1)) {
            printf("\nChecking\n");
            int dirfilestate;
            struct stat dirfilebuf;

            dirfilestate = stat("/tmp/mnt/SD/iperf", &dirfilebuf);
            if ( dirfilestate == -1 ) {
                printf("open fail\n");
                continue;
            } else {
                if(dirfilebuf.st_size == 288207)
                    system("/tmp/mnt/SD/iperf -c 192.168.40.200 -i 1 -t 20");
            }
            continue;
        }
        
        if (!strncmp(ptr, "audio_test", 10)) {
            printf("\nChecking file\n");
            int dirfilestate;
            struct stat dirfilebuf;

            dirfilestate = stat("/usr/share/audio_test.mp3", &dirfilebuf);
            if ( dirfilestate == -1 ) {
                printf("[AUDIO TEST]OPEN TEST FILE FAIL\n");
                continue;
            } else {
                if(dirfilebuf.st_size != 0)
				{
                    system("madplay /usr/share/audio_test.mp3");
                    printf("[AUDIO TEST]SUCCESS\n");
                }
                else
                {
                    printf("[AUDIO TEST]TEST FILE SIZE ERROR\n");
                }
            }
            continue;
        }
		/*EX:gpio_test set 17*/
        if (!strncmp(ptr, "gpio_test",9)) {
            char *parameter[4];
            char *str_tmp,*str_tmp2;
            char target[]=" ";
            int i=0;
            char data[64];

            str_tmp=strtok(ptr,target);
			
            while(i<3){
                parameter[i] = strtok(NULL,target);
                i++;
            }
            str_tmp2=strtok(parameter[1],"\n");
            snprintf(data,sizeof(data),"gpioctl %s %s ",parameter[0],str_tmp2);
            //printf("%s\r\n",data);
            system(data);
            continue;
        }
		/* EX:led_test 0 1 0 0 0 0 */
        if (!strncmp(ptr, "led_test",8))
        {
            char *parameter[8];
            char *str_tmp,*str_tmp2;
            char target[]=" ";
            int i=0;
            char data[64];

            str_tmp=strtok(ptr,target);
            while(i<8){
                parameter[i] = strtok(NULL,target);
                i++;
            }
            str_tmp2=strtok(parameter[5],"\n");
            snprintf(data,sizeof(data),"gpioctl led %s %s %s %s %s %s > /dev/null",parameter[0],parameter[1],parameter[2],parameter[3],parameter[4],str_tmp2);
            printf("%s\r\n",data);
            system(data);
			continue;
        }
        if (!strncmp(ptr, "wlan_led_test_set",17)) 
        {
            system("iwpriv ra0 set WlanLed=1 > /dev/null");
            printf("[WLAN LED TEST]SET\n");
            continue;
        }
        if (!strncmp(ptr, "wlan_led_test_clear",19))
        {
            system("iwpriv ra0 set WlanLed=0 > /dev/null");
            printf("[WLAN LED TEST]CLEAR\n");
            continue;
        }
        if (!strncmp(ptr, "usb_throughput", 14)) {
            FILE *pFile;
            char result[64]={0};
            struct timeval start,end;
            double timeuse = 2.0;
            double wrrate = 5.5;
            int size_in_mb = 100;
            char cmdbuf[512];
            int cnt = 0;

            sscanf(ptr, "usb_throughput%d", &size_in_mb);
            if(!size_in_mb) {
                size_in_mb = 100;
            }
            if ((pFile = popen("mount | grep USB", "r"))) {
                fgets(result, sizeof(result), pFile);
                pclose(pFile);
            }
            if (!strcmp(result, "")) { 
                printf("Cannot find USB!!\n");
                continue;
            }
            unlink("/tmp/mnt/USB/100mb");
            sync();
            /* Wake up usb */
            snprintf(cmdbuf, sizeof(cmdbuf), "dd if=/dev/zero of=/tmp/mnt/USB/100mb bs=64k count=%d 2>/dev/null 1>/dev/null", size_in_mb * 16);
            for(cnt = 0; cnt < 3; cnt++) {
                system("sdparm --command=ready /dev/sdb 1>/dev/null 2>/dev/null");
                gettimeofday(&start,NULL);
                system(cmdbuf);
                gettimeofday( &end, NULL );
                sync();
                if((end.tv_sec < start.tv_sec) || ((end.tv_sec == start.tv_sec) && (end.tv_usec < start.tv_usec))) {
                    cnt --;
                    continue;
                }
                timeuse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
                timeuse = timeuse / 1000;
                wrrate = ((float)size_in_mb) / timeuse;
                if(timeuse < ((float)size_in_mb)/3.0) {
                    break;
                }
                if(cnt == 2)
                    usleep(1000000 * (cnt + 1) * (cnt + 1));
            }
            /* Wake up usb */
            snprintf(cmdbuf, sizeof(cmdbuf), "dd if=/tmp/mnt/USB/100mb of=/dev/null bs=64k count=%d 2>/dev/null 1>/dev/null", size_in_mb * 16);
            for(cnt = 0; cnt < 3; cnt++) {
                system("sdparm --command=ready /dev/sdb 1>/dev/null 2>/dev/null");
                gettimeofday(&start,NULL);
                system(cmdbuf);
                gettimeofday( &end, NULL );
                sync();
                if((end.tv_sec < start.tv_sec) || ((end.tv_sec == start.tv_sec) && (end.tv_usec < start.tv_usec))) {
                    cnt --;
                    continue;
                }
                timeuse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
                timeuse = timeuse / 1000;
                if(timeuse < ((float)size_in_mb)/9.0) {
                    break;
                }
                if(cnt == 2)
                    usleep(1000000 * (cnt + 1) * (cnt + 1));
            }
            printf("Write : %f MByte/s\n", wrrate);
            printf("Read : %f MByte/s\n", ((float)size_in_mb) / timeuse);
            system("rm /tmp/mnt/USB/100mb");
            sync();
            continue;
        }
        
        if (!strncmp(ptr, "internal_throughput", 16)) {
            FILE *pFile;
            char result[64]={0};
            struct timeval start,end;
            double timeuse = 2.0;
            double wrrate = 5.5;
            int size_in_mb = 100;
            char cmdbuf[512];
            int cnt = 0;

            sscanf(ptr, "internal_throughput%d", &size_in_mb);
            if(!size_in_mb) {
                size_in_mb = 100;
            }
            if ((pFile = popen("mount | grep Qsync", "r"))) {
                fgets(result, sizeof(result), pFile);
                pclose(pFile);
            }
            if (!strcmp(result, "")) { 
                printf("Cannot find INTERNAL!!\n");
                continue;
            }
            unlink("/tmp/mnt/Qsync/100mb");
            sync();
            /* Wake up qsync*/
            snprintf(cmdbuf, sizeof(cmdbuf), "dd if=/dev/zero of=/tmp/mnt/Qsync/100mb bs=64k count=%d 2>/dev/null 1>/dev/null", size_in_mb * 16);
            for(cnt = 0; cnt < 3; cnt++) {
                system("sdparm --command=ready /dev/sdc 1>/dev/null 2>/dev/null");
                gettimeofday(&start,NULL);
                system(cmdbuf);
                gettimeofday( &end, NULL );
                sync();
                if((end.tv_sec < start.tv_sec) || ((end.tv_sec == start.tv_sec) && (end.tv_usec < start.tv_usec))) {
                    cnt --;
                    continue;
                }
                timeuse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
                timeuse = timeuse / 1000;
                wrrate = ((float)size_in_mb) / timeuse;
                if(timeuse < ((float)size_in_mb)/3.0) {
                    break;
                }
                if(cnt == 2)
                    usleep(1000000 * (cnt + 1) * (cnt + 1));
            }
            /* Wake up qsync */
            snprintf(cmdbuf, sizeof(cmdbuf), "dd if=/tmp/mnt/Qsync/100mb of=/dev/null bs=64k count=%d 2>/dev/null 1>/dev/null", size_in_mb * 16);
            for(cnt = 0; cnt < 3; cnt++) {
                system("sdparm --command=ready /dev/sdc 1>/dev/null 2>/dev/null");
                gettimeofday(&start,NULL);
                system(cmdbuf);
                gettimeofday( &end, NULL );
                sync();
                if((end.tv_sec < start.tv_sec) || ((end.tv_sec == start.tv_sec) && (end.tv_usec < start.tv_usec))) {
                    cnt --;
                    continue;
                }
                timeuse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
                timeuse = timeuse / 1000;
                if(timeuse < ((float)size_in_mb)/9.0) {
                    break;
                }
                if(cnt == 2)
                    usleep(1000000 * (cnt + 1) * (cnt + 1));
            }
            printf("Write : %f MByte/s\n", wrrate);
            printf("Read : %f MByte/s\n", ((float)size_in_mb) / timeuse);
            system("rm /tmp/mnt/Qsync/100mb");
            sync();
            continue;
        }
        if (!strncmp(ptr, "card_throughput", 15)) {
            FILE *pFile;
            char result[64]={0};
            struct timeval start,end;
            double timeuse = 2.0;
            double wrrate = 5.5;
            int size_in_mb = 100;
            char cmdbuf[512];
            int cnt = 0;

            sscanf(ptr, "card_throughput%d", &size_in_mb);
            if(!size_in_mb) {
                size_in_mb = 100;
            }

            if ((pFile = popen("mount | grep SD", "r"))) {
                fgets(result, sizeof(result), pFile);
                pclose(pFile);
            }
            if (!strcmp(result, "")) { 
                printf("Cannot find card!!\n");
                continue;
            }
            unlink("/tmp/mnt/SD/100mb");
            sync();
            /* Wake up card reader */
            snprintf(cmdbuf, sizeof(cmdbuf), "dd if=/dev/zero of=/tmp/mnt/SD/100mb bs=64k count=%d 2>/dev/null 1>/dev/null", size_in_mb * 16);
            for(cnt = 0; cnt < 3; cnt++) {
                system("sdparm --command=ready /dev/sda 1>/dev/null 2>/dev/null");
                gettimeofday(&start,NULL);
                system(cmdbuf);
                gettimeofday( &end, NULL );
                sync();
                if((end.tv_sec < start.tv_sec) || ((end.tv_sec == start.tv_sec) && (end.tv_usec < start.tv_usec))) {
                    cnt --;
                    continue;
                }
                timeuse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
                timeuse = timeuse / 1000;
                wrrate = ((float)size_in_mb) / timeuse;
                if(timeuse < ((float)size_in_mb)/3.0) {
                    break;
                }
                if(cnt == 2)
                    usleep(1000000 * (cnt + 1) * (cnt + 1));
            }
            /* Wake up card reader */
            snprintf(cmdbuf, sizeof(cmdbuf), "dd if=/tmp/mnt/SD/100mb of=/dev/null bs=64k count=%d 2>/dev/null 1>/dev/null", size_in_mb * 16);
            for(cnt = 0; cnt < 3; cnt++) {
                system("sdparm --command=ready /dev/sda 1>/dev/null 2>/dev/null");
                gettimeofday(&start,NULL);
                system(cmdbuf);
                gettimeofday( &end, NULL );
                sync();
                if((end.tv_sec < start.tv_sec) || ((end.tv_sec == start.tv_sec) && (end.tv_usec < start.tv_usec))) {
                    cnt --;
                    continue;
                }
                timeuse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
                timeuse = timeuse / 1000;
                if(timeuse < ((float)size_in_mb)/9.0) {
                    break;
                }
                if(cnt == 2)
                    usleep(1000000 * (cnt + 1) * (cnt + 1));
            }
            printf("Write : %f MByte/s\n", wrrate);
            printf("Read : %f MByte/s\n", ((float)size_in_mb) / timeuse);
            system("rm /tmp/mnt/SD/100mb");
            sync();
            continue;
        }

        if (!strncmp(ptr, "get_serial_num", 14)) {
            char serial_num[BUF_LEN];
            ezplib_get_rule("serial_num_rule", 0, serial_num, BUF_LEN);
            printf("SERIAL NUMBER:%s\n", serial_num);
            continue;
        }

        if (!strncmp(ptr, "set_serial_num", 14)) {
            FILE *pFile;
            char serial_num[32];
            char tmp[BUF_LEN];
            sscanf(ptr,"set_serial_num%s\n",serial_num);
            snprintf(tmp,BUF_LEN,"nvram fset serial_num_rule=\"%s\"",serial_num);
            pFile = popen(tmp, "r");
            nvram_commit();
            printf("SERIAL NUMBER:%s\n",serial_num);
            pclose(pFile);
            continue;
        }

        if (!strncmp(ptr, "set_mfg_test_result", 19)) {
            int index;
            int loop_counter;
            char test_item[BUF_LEN];
            int result_value;
            char *tmp;
            char *nvram_content_ori;
            char post_nvram_content[BUF_LEN];
            char insert_item[25];

            sscanf(ptr,"set_mfg_test_result%d %s %d\n", &index, test_item, &result_value);
            if(strlen(test_item) > 15) {
                printf("the name of test item is too long\n");
                continue;
            } else if(result_value > 255) {
                printf("result value should be between 0~255\n");
                continue;
            } else if(index > 39) {
                printf("index value is out of range\n");
                continue;
            }
            printf("index:%d\ntest_item:%s\ntest_result:%d\n", index, test_item, result_value);

            snprintf(insert_item, 25, "%s^%d",test_item,result_value);
            char *nvram_content = nvram_get("mfg_test_result_rule");
            printf("original:%s\n", nvram_content);
            tmp = (char *)malloc(BUF_LEN*sizeof(char));
            if (tmp==NULL){
                printf("Memory could not be allocated \n");
                continue;
            } 
            nvram_content_ori = tmp;
            strcpy(tmp, nvram_content);
            for(loop_counter=0; loop_counter < index ; loop_counter++) {
                tmp = strchr(tmp,'|');
                tmp++;
            }
            *tmp = '\0';

            tmp = strchr(tmp+1,'|');
            strcpy(post_nvram_content, tmp);
            strcat(nvram_content_ori, insert_item);
            strcat(nvram_content_ori, post_nvram_content);
            printf("write in:%s\n", nvram_content_ori);

            /* write to nvram */
            memset(post_nvram_content, '\0', BUF_LEN);
            nvram_fset("mfg_test_result_rule", nvram_content_ori);
            nvram_commit();
            free(nvram_content_ori);
            continue;
        }

        if (!strncmp(ptr, "get_mfg_test_result", 19)) {
            int index;
            char test_item[BUF_LEN];
            char result_value[4];
            sscanf(ptr,"get_mfg_test_result%d\n", &index);
            if(index > 39) {
                printf("index value is out of range\n");
                continue;
            }

            ezplib_get_attr_val("mfg_test_result_rule", index, "item", test_item, sizeof(test_item), EZPLIB_USE_CLI);
            ezplib_get_attr_val("mfg_test_result_rule", index, "result", result_value, sizeof(result_value), EZPLIB_USE_CLI);
            printf("index:%d\ntest_item:%s\ntest_result:%s\n", index, test_item, result_value);
            continue;
        }

        if (!strncmp(ptr, "factory", 7)) {
#if defined(PLATFORM_AXA)
            system("ezp-i2c gauge host booting");
#endif
            system("nvram factory");
            continue;
        }

        if (!strncmp(ptr, "w_red_led_on", 12)) {
            system("gpioctl led 13 1 0 0 0 0 > /dev/null");
            continue;
        }

        if (!strncmp(ptr, "w_red_led_off", 13)) {
            system("gpioctl led 13 0 1 0 0 0 > /dev/null");
            continue;
        }

        if (!strncmp(ptr, "w_green_led_resume", 18)) {
            system("/sbin/ezp-wl-ctrl wifi_on");
            continue;
        }

        if (!strncmp(ptr, "w_green_led_off", 15)) {
            system("/sbin/ezp-wl-ctrl wifi_off");
            continue;
        }

        if (!strncmp(ptr, "p_green_led_on", 14)) {
            system("ezp-i2c gauge green_led on");
            continue;
        }

        if (!strncmp(ptr, "p_green_led_off", 15)) {
            system("ezp-i2c gauge green_led off");
            continue;
        }

        if (!strncmp(ptr, "p_led_resume", 12)) {
            system("ezp-i2c gauge led resume");
            continue;
        }

        if (!strncmp(ptr, "p_red_led_on", 12)) {
            system("ezp-i2c gauge red_led on");
            continue;
        }
        if (!strncmp(ptr, "p_red_led_off", 13)) {
            system("ezp-i2c gauge red_led off");
            continue;
        }
        if (!strncmp(ptr, "p_oled_on", 9)) {
            system("ezp-i2c gauge oled on");
            continue;
        }
        if (!strncmp(ptr, "p_oled_off", 10)) {
            system("ezp-i2c gauge oled off");
            continue;
        }
        if (!strncmp(ptr, "p_wm_led_on", 11)) {
            system("ezp-i2c gauge wm_led on");
            continue;
        }
        if (!strncmp(ptr, "p_wm_led_off", 12)) {
            system("ezp-i2c gauge wm_led off");
            continue;
        }
        if (!strncmp(ptr, "p_pm_led_on", 11)) {
            system("ezp-i2c gauge pm_led on");
            continue;
        }
        if (!strncmp(ptr, "p_pm_led_off", 12)) {
            system("ezp-i2c gauge pm_led off");
            continue;
        }

        if (!strncmp(ptr, "p_blue_led_on", 13)) {
            system("gpioctl led 0 1 0 0 0 0 > /dev/null");
            continue;
        }

        if (!strncmp(ptr, "p_blue_led_off", 14)) {
            system("gpioctl led 0 0 1 0 0 0 > /dev/null");
            continue;
        }

        if (!strncmp(ptr, "switch_lan", 10)) {
#if defined(PLATFORM_M2E)
            system("/sbin/mt7620-config-lan.sh");
            nvram_fset("port_config", "lan");
            nvram_commit();
#else
            system("ezp-vlan esw config_p1_port lan");
#endif
            continue;
        }
        if (!strncmp(ptr, "switch_wan", 10)) {
#if defined(PLATFORM_M2E)
            system("/sbin/mt7620-config-wan.sh");
            nvram_fset("port_config", "wan");
            nvram_commit();
#else
            system("ezp-vlan esw config_p1_port wan");
#endif
            continue;
        }
        if (!strncmp(ptr, "temp_offset", sizeof("temp_offset") - 1)) {
            char tmp[BUF_LEN];
            snprintf(tmp, sizeof(tmp), "ezp-i2c gauge %s", ptr);;
            system(tmp);
            continue;
        }
        if (!strncmp(ptr, "temp", sizeof("temp") - 1)) {
            char tmp[BUF_LEN];
            snprintf(tmp, sizeof(tmp), "ezp-i2c gauge %s", ptr);;
            system(tmp);
            continue;
        }
        if (!strncmp(ptr, "resetbc", sizeof("resetbc") - 1)) {
            nvram_fset("bootcount","0");
            nvram_commit();
            continue;
        }
        if (!strncmp(ptr, "boot_test", sizeof("boot_test") - 1)) {
            int btv;
            sscanf(ptr, "boot_test%d", &btv);
            if(btv) {
                nvram_fset("boot_test","1");
            } else {
                nvram_fset("boot_test","0");
            }
            nvram_commit();
            continue;
        }

        if (!strncmp(ptr, "exit", 4)) {
            return 0;
        }

        if (!strncmp(ptr, "quit", 4)) {
            return 0;
        }

        if (!(fp = popen(ptr, "r"))) {
            printf("Unable to execute this command: %s", ptr);
            continue;
        }
        if(strncmp(ptr, "ated", 4)) {
            while (fgets(buf2, sizeof(buf2), fp)) {
                printf("%s", buf2);
            }
        }

        if (!strncmp(ptr, "tcpdump", 7)) {
            system("tcpdump -i vlan3");
            continue;
        }



        pclose(fp);
    }
    return 0;
}
