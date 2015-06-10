/*
 *	stats
 *	Copyright (C) 2006-2007 EZ Packet Technologies
 */
/*

	rstats
	Copyright (C) 2006-2007 Jonathan Zarate

	Licensed under GNU GPL v2 or later.
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <linux/if.h>

#include <linux/ezp-gpio.h>
#include <linux/ioctl.h>
#include "ra_ioctl.h"



typedef struct {
    int gpio;           //gpio number (0 ~ 23)
    unsigned int on;        //interval of led on
    unsigned int off;       //interval of led off
    unsigned int blinks;        //number of blinking cycles
    unsigned int rests;     //number of break cycles
    unsigned int times;     //blinking times
} ralink_gpio_led_info;

#define ETH_LED   22

const char pid_fn[] = "/var/run/ezp-eth-led.pid";
int close_daemon = 0;

#define     ETH_LED_ON  1
#define     ETH_LED_OFF 2
#define     ETH_LED_BLINK_FAST  3
#define     ETH_LED_BLINK       4
#define     ETH_LED_BLINK_SLOW  5

void
gpio_config(int fd){
    unsigned int cmd;
    /* config direction out */
    cmd = (1L << ETH_LED);
    ioctl(fd, EZP_GPIO_SET_DIR_OUT, cmd);
}

int esw_fd;
void 
socket_init(){
    esw_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (esw_fd < 0) {
        perror("Socket Open fail!!");
        exit(0);
    }
}

void 
socket_fini(){
    close(esw_fd);
}

#define DEVNAME "eth2"
int 
reg_read(int offset, int *value)
{
    struct ifreq ifr;
    esw_reg reg;

    if (value == NULL)
        return -1;
    reg.off = offset;
    strncpy(ifr.ifr_name, DEVNAME, sizeof(DEVNAME));
    ifr.ifr_data = &reg;
    if (-1 == ioctl(esw_fd, RAETH_ESW_REG_READ, &ifr)) {
        perror("ioctl");
        close(esw_fd);
        exit(0);
    }
    *value = reg.val;
    return 0;
}
//ESW Reg of Port Ability for link status polling.
#define POA 0x0080
//ESW Reg for Packet Count polling.
#define P0PC 0x00E8
//Bit Mask for PORT0 Link up/down
#define PORT0_LINK_MASK 0x2000000

unsigned int 
eth_link_status(){
    int result;
    reg_read(POA, &result);
    result = (result & PORT0_LINK_MASK) >> 25; 
    return (unsigned int)result;
}

#define FAST_BLINK 100 
#define SLOW_BLINK 20

int
get_flow_rate(){

    unsigned int result;
    int current_cnt = 0;
    static int pkt_cnt = 0;
    
    if (!eth_link_status()) 
        //Link is down
        result = ETH_LED_OFF;
    else {
        //Link is up
        reg_read(P0PC, &current_cnt);
        current_cnt &= 0xFFFF;
        if (pkt_cnt == current_cnt) {
            result = ETH_LED_ON;
        } else {
            unsigned int diff=0;
            if (current_cnt > pkt_cnt) {
                diff = current_cnt - pkt_cnt;
            } else {
                diff = 0xFFFF - pkt_cnt + current_cnt;
            }
            printf("ETH FLOW RATE is %d pkt/s!!!\n",diff);
            if (diff >= FAST_BLINK) 
                result = ETH_LED_BLINK_FAST;
            else if (diff >= SLOW_BLINK && diff < FAST_BLINK) 
                result = ETH_LED_BLINK;
            else if (diff < SLOW_BLINK) 
                result = ETH_LED_BLINK_SLOW;
        }
        pkt_cnt = current_cnt;
    }
    return (int)result;
}

void
led(int fd, int pin, int action){
    
    ralink_gpio_led_info led;
    static int eth_state;

    /* check action change? */
    if (eth_state == action) {
        /* if pin state do not change, leave function */
        return;
    } else {
        eth_state = action;
    }
    led.gpio = pin;
    switch(action){
        case ETH_LED_ON:
            led.on = 1;
            led.off = 1;
            led.blinks = 0;
            led.rests = 0;
            led.times = 1;
            break;
        case ETH_LED_OFF:
            led.on = 1;
            led.off = 0;
            led.blinks = 0;
            led.rests = 0;
            led.times = 0;
            break;
        case ETH_LED_BLINK_FAST:
            led.on = 1;
            led.off = 1;
            led.blinks = 1;
            led.rests = 0;
            led.times = 1;
            break;
        case ETH_LED_BLINK:
            led.on = 1;
            led.off = 5;
            led.blinks = 1;
            led.rests = 0;
            led.times = 1;
            break;
        case ETH_LED_BLINK_SLOW:
            led.on = 1;
            led.off = 10;
            led.blinks = 1;
            led.rests = 0;
            led.times = 1;
            break;
    }
    ioctl(fd, EZP_GPIO_LED_SET, &led);
}

static void
handle_sig_int(int sig)
{
    close_daemon = 1;
}

int
main(int argc, char *argv[])
{
    int fd, blink;
    FILE *f;
    pid_t pid, sid;

    /* Daemonize */
    if ((pid = fork()) > 0) {
        /* Parent: write a PID file. */
        if ((f = fopen(pid_fn, "w")) == NULL) {
            return 1;
        }
        fprintf(f, "%d\n", pid);
        fclose(f);
        return 0;
    }
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        /* Log any failures here */
        return -1;
    }
    signal(SIGTERM, handle_sig_int);
    /* Open device */
    if ((fd = open("/dev/gpio", O_RDWR)) < 0) {
        printf("Error whilst opening /dev/gpio\n");
        return -1;
    }
    /* Config gpio diretcion */
    gpio_config(fd);
    /* Polling for detect power change */
    while(!close_daemon) {
        socket_init();
        blink = get_flow_rate();
        printf("ETH LED BLINKS ==> %d\n", blink);
        led(fd, ETH_LED, blink);
        socket_fini();
        sleep(2);
    }
    close(fd);
    unlink(pid_fn);
    return 0;
}
