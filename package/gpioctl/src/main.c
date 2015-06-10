/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA02111-1307USA
*
* Feedback, Bugs...  blogic@openwrt.org 
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <linux/gpio_dev.h>
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

void
print_usage()
{
	printf("gpioctl start\n");
	printf("gpioctl dirin|dirout|get|set|clear|led gpio\n");
	exit(0);
}

int
main(int argc, char **argv)
{
    int fd;
    unsigned int gpio_pin,cmd;
    int result = 0;

	if ((fd = open("/dev/gpio", O_RDWR)) < 0)
	{
        printf("Error whilst opening /dev/gpio\n");
        return -1;
	}
	if (argc < 3)
	{
        if (!strcmp(argv[1], "start")) {
#if defined(PLATFORM_M2E) || defined(PLATFORM_ALS)
            cmd = 0xFF;
#else
            cmd = (1L << 0xFF);
#endif
            result = ioctl(fd, EZP_GPIO_START, &cmd);
            return result;
        } else {
            print_usage();
        }
	}

	gpio_pin = atoi(argv[2]);

	printf("using gpio pin %d\n", gpio_pin);

	if (!strcmp(argv[1], "dirin"))
	{
        cmd = (1L << gpio_pin);
#if defined(PLATFORM_M2E) || defined(PLATFORM_ALS) 
        result = ioctl(fd, EZP_GPIO_SET_DIR_IN, gpio_pin);
#else
        result = ioctl(fd, EZP_GPIO_SET_DIR_IN, cmd);
#endif
        printf("result = %d\n",result);
	} else if (!strcmp(argv[1], "dirout"))
	{
        cmd = (1L << gpio_pin);
#if defined(PLATFORM_M2E) || defined(PLATFORM_ALS)
		result = ioctl(fd, EZP_GPIO_SET_DIR_OUT, gpio_pin);
#else
		result = ioctl(fd, EZP_GPIO_SET_DIR_OUT, cmd);
#endif
        printf("result = %d\n",result);
	} else if (!strcmp(argv[1], "get"))
	{
        cmd = (gpio_pin << 24) + EZP_GPIO_READ_BIT;
        if ((ioctl(fd, cmd, &result)) != -1)
        {
            printf("Pin %d is %d\n", gpio_pin,result);
        } else
        {
            printf("error\n");
        }
    } else if (!strcmp(argv[1], "set"))
	{
        cmd = (1L << gpio_pin);
#if defined(PLATFORM_M2E) || defined(PLATFORM_ALS)
		result = ioctl(fd, EZP_GPIO_SET, gpio_pin);
#else
		result = ioctl(fd, EZP_GPIO_SET, cmd);
#endif
        printf("cmd=0x%08x  result = %d\n",cmd,result);
	} else if (!strcmp(argv[1], "clear"))
	{
        cmd = (1L << gpio_pin);
#if defined(PLATFORM_M2E) || defined(PLATFORM_ALS)
        result = ioctl(fd, EZP_GPIO_CLEAR, gpio_pin);
#else
        result = ioctl(fd, EZP_GPIO_CLEAR, cmd);
#endif
        printf("cmd=0x%08x  result = %d\n",cmd,result);
	} else if (!strcmp(argv[1], "led"))
	{
        if (argc != 8)
        {
            printf("gpioctl led gpio on off blinks rests times\n");
            exit(0);
        }
        ralink_gpio_led_info led;
        led.gpio=atoi(argv[2]);
        led.on=atoi(argv[3]);
        led.off=atoi(argv[4]);
        led.blinks=atoi(argv[5]);
        led.rests=atoi(argv[6]);
        led.times=atoi(argv[7]);
        result = ioctl(fd, EZP_GPIO_LED_SET, &led);
        printf("result = %d\n",result);
	} else if (!strcmp(argv[1], "show"))
	{
		result = ioctl(fd, EZP_GPIO_READ_INT, &cmd);
        printf("result = %d state = 0x%08X\n",result,cmd);
    } else
    {
        print_usage();
    }

	return result;
}
