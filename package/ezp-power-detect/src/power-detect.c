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

/* for GPIO control */
#include <linux/ezp-gpio.h>
#include <linux/ioctl.h>

typedef struct {
    int gpio;           //gpio number (0 ~ 23)
    unsigned int on;        //interval of led on
    unsigned int off;       //interval of led off
    unsigned int blinks;        //number of blinking cycles
    unsigned int rests;     //number of break cycles
    unsigned int times;     //blinking times
} ralink_gpio_led_info;

#define DEBUG_MODE 0
#define MR_102N_V2

#if defined MR_102N_V2

#define GREEN_LED   19
#define RED_LED     23
#define LOW_BAT     10
#define AC_DET      12
#define CHARGE_END  11
#define CHARGE_FAULT   13

#else

#define GREEN_LED   7
#define RED_LED     9
#define LOW_BAT     9
#define AC_DET      10
#define CHARGE1     14
#define CHARGE2     0

#endif

/* LED action */
#define ON          0
#define OFF         1
#define BLINK       2

const char pid_fn[] = "/var/run/power-detect.pid";
int close_daemon = 0;
int red_state = -1;
int green_state = -1;

#if defined MR_102N_V2
void
gpio_config(int fd){
    unsigned int cmd;
    /* config direction in */
    cmd = (1L << CHARGE_END);
    ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
    cmd = (1L << CHARGE_FAULT);
    ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
    cmd = (1L << AC_DET);
    ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
    cmd = (1L << LOW_BAT);
    ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
    /* config direction out */
    cmd = (1L << GREEN_LED);
    ioctl(fd, EZP_GPIO_SET_DIR_OUT, cmd);
    cmd = (1L << RED_LED);
    ioctl(fd, EZP_GPIO_SET_DIR_OUT, cmd);
}
#else
void
gpio_config(int fd){
    unsigned int cmd;
    /* config direction in */
     cmd = (1L << CHARGE1);
     ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
     cmd = (1L << CHARGE2);
     ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
     cmd = (1L << AC_DET);
     ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
            
     /* config direction out */
     cmd = (1L << GREEN_LED);
     ioctl(fd, EZP_GPIO_SET_DIR_OUT, cmd);
     cmd = (1L << RED_LED);
     ioctl(fd, EZP_GPIO_SET_DIR_OUT, cmd);
}
#endif


void
led(int fd, int pin, int action){
    ralink_gpio_led_info led;
    static int green_state, red_state;

    /* check action change? */
    if (pin == GREEN_LED) {
        if (green_state == action) {
            /* if pin state do not change, leave function */
            return;
        } else {
            green_state = action;
        }
    } else if (pin == RED_LED) {
        if (red_state == action) {
            /* if pin state do not change, leave function */
            return;
        } else {
            red_state = action;
        }
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
            led.on = 30;
            led.off = 1;
            led.blinks = 1;
            led.rests = 0;
            led.times = 1;
            break;
    }
    ioctl(fd, EZP_GPIO_LED_SET, &led);
}

#define BAT_ERR 0
#define BAT_FULL 1
#define BAT_PRECHAR 2
#define BAT_CHAR 3
#define BAT_DISCHAR 4
#define BAT_UNKNOW 5

#if defined MR_102N_V2
int
battery_status(int fd){
    unsigned int cmd;
    unsigned int s1, s2, s3, result;
    cmd = (CHARGE_END << 24) + EZP_GPIO_READ_BIT;
    if ((ioctl(fd, cmd, &s1)) == -1) {
       printf("get battery EOC error\n");
    }
#if DEBUG_MODE
    printf("get battery EOC ==> %d\n", s1);
#endif

    cmd = (CHARGE_FAULT << 24) + EZP_GPIO_READ_BIT;
    if ((ioctl(fd, cmd, &s2)) == -1) {
       printf("get battery fault error\n");
    }
#if DEBUG_MODE
    printf("get battery fault ==> %d\n", s2);
#endif
    
    cmd = (AC_DET << 24) + EZP_GPIO_READ_BIT;
    if ((ioctl(fd, cmd, &s3)) == -1) {
       printf("detect AC error\n");
    }
#if DEBUG_MODE
    printf("get AC detect ==> %d\n", s3);
#endif
    
    //AC power in
    if ( s3 == 0 ){
        if (s1 == 0 && s2 == 0) {
            result = BAT_ERR;
        } else if (s1 == 0 && s2 == 1) {
            result = BAT_FULL;
        } else if (s1 == 1 && s2 == 0) {
            result = BAT_ERR;
        } else {
            result = BAT_CHAR;
        }
    //use battery power
    }else{
        if (s1 == 0 && s2 == 0) {
            result = BAT_ERR;
        } else if (s1 == 0 && s2 == 1) {
            result = BAT_DISCHAR;
        } else if (s1 == 1 && s2 == 0) {
            result = BAT_ERR;
        } else {
            result = BAT_DISCHAR;
        }
    }
    return result;
}
#else
int
battery_status(int fd){
    unsigned int cmd;
    unsigned int s1, s2, s3, result;
    cmd = (CHARGE1 << 24) + EZP_GPIO_READ_BIT;
    if ((ioctl(fd, cmd, &s1)) == -1) {
       printf("get battery status error\n");
    }
#if DEBUG_MODE
    printf("get battery stat1 ==> %d\n", s1);
#endif
    cmd = (CHARGE2 << 24) + EZP_GPIO_READ_BIT;
    if ((ioctl(fd, cmd, &s2)) == -1) {
       printf("get battery status error\n");
    }
#if DEBUG_MODE
    printf("get battery stat2 ==> %d\n", s2);
#endif
    cmd = (AC_DET << 24) + EZP_GPIO_READ_BIT;
    if ((ioctl(fd, cmd, &s3)) == -1) {
       printf("detect AC error\n");
    }
#if DEBUG_MODE
    printf("get AC detect ==> %d\n", s3);
#endif
    //AC power in
    if ( s3 == 0 ){
        if (s1 == 0 && s2 == 0) {
            result = BAT_FULL;
        } else if (s1 == 0 && s2 == 1) {
            result = BAT_ERR;
        } else if (s1 == 1 && s2 == 0) {
            result = BAT_PRECHAR;
        } else {
            result = BAT_CHAR;
        }
    //use battery power
    }else{
        if (s1 == 0 && s2 == 1) {
            result = BAT_DISCHAR;
        } else {
            result = BAT_UNKNOW;
        }
    }
    return result;
}

#endif

#if defined MR_102N_V2
int
low_battery(int fd){
    unsigned int cmd;
    int result;
    /* get pin statud */
    cmd = (LOW_BAT << 24) + EZP_GPIO_READ_BIT;
    if ((ioctl(fd, cmd, &result)) == -1) {
       printf("get battery low error\n");
    }
#if DEBUG_MODE
    printf("get battery low ==> %d\n", result);
#endif
    return result;
}
#else
int
low_battery(int fd){
    unsigned int cmd;
    int result;
    int tmp;
    /* config gpio pin direction to in */
    /* this pin is shared to RED_LED pin */
    cmd = (1L << LOW_BAT);
    tmp = ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
    /* get pin statud */
    cmd = (LOW_BAT << 24) + EZP_GPIO_READ_BIT;
    if ((ioctl(fd, cmd, &result)) == -1) {
       printf("get battery low error\n");
    }
    /* Put this pin back to output mode */
    cmd = (1L << LOW_BAT);
    tmp = ioctl(fd, EZP_GPIO_SET_DIR_OUT, cmd);
#if DEBUG_MODE
    printf("get battery low ==> %d\n", result);
#endif
    return result;
}
#endif

static void
handle_sig_int(int sig)
{
    close_daemon = 1;
}

int
main(int argc, char *argv[])
{
    int fd;
    FILE *f;
    unsigned int bat_stat = 0;
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
    /* Open Device */
    if ((fd = open("/dev/gpio", O_RDWR)) < 0) {
        printf("Error whilst opening /dev/gpio\n");
        return -1;
    }
    /* Config gpio diretcion */
    gpio_config(fd);
    /* Polling for detect power change */
    while(!close_daemon) {
        bat_stat = battery_status(fd);
        switch(bat_stat){
            
            case BAT_ERR:
                led(fd, GREEN_LED, OFF);
                led(fd, RED_LED, BLINK);
#if DEBUG_MODE
                printf("PWR STATUS ==> BAT in ERROR.\n");
#endif
                break;
            case BAT_CHAR:
                led(fd, GREEN_LED, OFF);
                led(fd, RED_LED, OFF);
#if DEBUG_MODE
                printf("PWR STATUS ==> BAT in CHARGE.\n");
#endif
                break;
            case BAT_PRECHAR:
                led(fd, GREEN_LED, OFF);
                led(fd, RED_LED, OFF);
#if DEBUG_MODE
                printf("PWR STATUS ==> BAT in PRECHARGE.\n");
#endif
                break;
            case BAT_FULL:
                led(fd, GREEN_LED, ON);
                led(fd, RED_LED, OFF);
#if DEBUG_MODE
                printf("PWR STATUS ==> BAT in FULL.\n");
#endif
                break;
            case BAT_DISCHAR:
                if(low_battery(fd) == 1 ) {
                    /* battery low voltage */
                    led(fd, GREEN_LED, OFF);
                    led(fd, RED_LED, ON);
#if DEBUG_MODE
                    printf("PWR STATUS ==> BAT in DISCHARGE and LOW BAT.\n");
#endif
                } else {
                    led(fd, GREEN_LED, BLINK);
                    led(fd, RED_LED, OFF);
#if DEBUG_MODE
                    printf("PWR STATUS ==> BAT in DISCHARGE.\n");
#endif
                }
                break;
            default:
                led(fd, GREEN_LED, OFF);
                led(fd, RED_LED, OFF);
#if DEBUG_MODE
                printf("PWR STATUS ==> BAT in UNKNOW.\n");
#endif
        }
        sleep(1);
    }
    close(fd);
    unlink(pid_fn);
    return 0;
}
