/*
 *  ezp-wps	
 *	Copyright (C) 2010 EZ Packet Technologies
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/wireless.h>
#include <getopt.h>

/* for GPIO control */
#include <linux/ezp-gpio.h>
#include <linux/ioctl.h>

#define AP_MODE
#include "oid.h"
     

typedef struct {
    int gpio;           //gpio number (0 ~ 23)
    unsigned int on;        //interval of led on
    unsigned int off;       //interval of led off
    unsigned int blinks;        //number of blinking cycles
    unsigned int rests;     //number of break cycles
    unsigned int times;     //blinking times
} ralink_gpio_led_info;


/* LED action */
#define ON          0
#define OFF         1
#define BLINK       2

#define DEFAULT_LED  14 

#define DEFAULT_TIME 120

int start_process = 0;
int led_state = -1;
int isEnrollee = 0;
char interface[32];
unsigned int wps_time = 0, count = 0;

void
led(int fd, int pin, int action){
    ralink_gpio_led_info led;
    /* check action change? */
    if (led_state == action) {
        /* if pin state do not change, leave function */
        return;
    } else {
        led_state = action;
    }
    led.gpio = pin;
    switch(action){
        case ON:
            led.on = 1;
            led.off = 1;
            led.blinks = 0;
            led.rests = 0;
            led.times = 1;
            break;
        case OFF:
            led.on = 1;
            led.off = 0;
            led.blinks = 0;
            led.rests = 0;
            led.times = 0;
            break;
        case BLINK:
            led.on = 40;
            led.off = 1;
            led.blinks = 1;
            led.rests = 0;
            led.times = 1;
            break;
    }
    ioctl(fd, EZP_GPIO_LED_SET, &led);
}

unsigned int getAPPIN(char *interface)
{
    int socket_id;
    struct iwreq wrq;
    unsigned int data = 0;
    socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(wrq.ifr_name, interface);
    wrq.u.data.length = sizeof(data);
    wrq.u.data.pointer = (caddr_t) &data;
    wrq.u.data.flags = RT_OID_WSC_PIN_CODE;
    if( ioctl(socket_id, RT_PRIV_IOCTL, &wrq) == -1)
        printf("ioctl error\n");
    close(socket_id);
    return data;
}

int getWscStatus(char *interface)
{
    int socket_id;
    struct iwreq wrq;
    int data = 0;
    socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(wrq.ifr_name, interface);
    wrq.u.data.length = sizeof(data);
    wrq.u.data.pointer = (caddr_t) &data;
    wrq.u.data.flags = RT_OID_WSC_QUERY_STATUS;
    if( ioctl(socket_id, RT_PRIV_IOCTL, &wrq) == -1)
        printf("ioctl error\n");
    close(socket_id);
    return data;
}

static void
handle_sig_int(int sig)
{
    start_process = 0;
    fprintf(stdout, "Stop wps process by user\n");
}

int
main(int argc, char *argv[])
{
    /* int c, fd, led_pin = DEFAULT_LED; */
    int c, led_pin = DEFAULT_LED;
    int WscStatus = 0;

    signal(SIGTERM, handle_sig_int);
    /* Open gpio device for led control */
    /* if ((fd = open("/dev/gpio", O_RDWR)) < 0) {
        printf("Error whilst opening /dev/gpio\n");
        return -1;
    } */
    /* get command */
    while ((c = getopt(argc, argv, "g:p:l:h")) != -1)
        switch (c) {
            case 'g':
                /* get router pin code */
                if (*optarg) {
                    strcpy(interface, optarg);
                } else {
                    fprintf(stderr, "please input interface\n");
                    exit(0);
                }
                fprintf(stdout, "%08d\n", getAPPIN(interface));
                break;
            case 'l':
                if (*optarg) {
                    led_pin = atoi(optarg);
                } else {
                    fprintf(stderr, "please input gpio pin\n");
                    exit(0);
                }
                break;
            case 'p':
                if (*optarg) {
                    wps_time = atoi(optarg);
                } else {
                    wps_time = DEFAULT_TIME;
                }
                start_process = 1;
                break;
            case 'h':
                fprintf(stderr, "Usage: %s [-p time] [-g interface]\n"
                        "	-p time: wps process waiting time (sec)\n"
                        "	-l led pin: wps led gpio pin \n"
                        "	-g interface: get router pin code\n", argv[0]);
                exit(0);
                break;
        }


    /* Polling for wps process starting */
    while(start_process) {
        WscStatus = getWscStatus("ra0");
        /* printf("WscStatus == %d\n", WscStatus); */
        /* "Start WSC Process" */
        if( WscStatus == 3 ){  
            /* printf("Start to monitor WSC Status...\n"); */
            /* led(fd, led_pin, BLINK); */
        }

        /* deal with error */
        if(WscStatus == 2){ // 2 == "Wsc Process failed"
            if(isEnrollee){ 
                /* go on monitoring */
            }else{
                printf("WPS Error occured.  Is the PIN correct?\n");
                break;
            }
        }

        /* Driver 1.9 supports AP PBC Session Overlapping Detection. */
        if(WscStatus == 0x109 /* PBC_SESSION_OVERLAP */){
            printf("PBC_SESSION_OVERLAP\n");
            break;
        }

        /* then check idle status */
        if(WscStatus == 1 ){
            /*Idle*/
            /*printf("WPS idle\n");  */
        }

        /* 34 == "Configured */
        if (WscStatus != 34 ){    
            /* still in progress and keep monitoring. */
        }else{
            /* WPS process success */
            /* led(fd, led_pin, ON); */
            printf("WPS Connect Success\n");
            break;
        }

        usleep(100000);
        count++;
        if (count > wps_time) {
            /* Timeout happened.
             Set g_WscResult to indicate WSC process failed. */
            /* led(fd, led_pin, ON); */
            printf("WPS Failed, Timeout\n");
            break;
        }
    }
    return 0;
}
