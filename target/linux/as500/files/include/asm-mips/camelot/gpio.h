/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved.
 */
#ifndef __ASM_CAMELOT_GPIO_H
#define __ASM_CAMELOT_GPIO_H

#include <linux/kernel.h>

#define CAMELOT_GPIO_LINES		20		/* GPIO[0~19] */

extern int camelot_gpio_to_irq(unsigned gpio);
extern int camelot_gpio_get_value(unsigned gpio);
extern void camelot_gpio_set_value(unsigned gpio, int value);
extern int camelot_gpio_direction_input(unsigned gpio);
extern int camelot_gpio_direction_output(unsigned gpio, int value);

static inline int gpio_request(unsigned gpio, const char *label)
{
    return 0;
}

static inline void gpio_free(unsigned gpio)
{
}

static inline int gpio_to_irq(unsigned gpio)
{
	return camelot_gpio_to_irq(gpio);
}

static inline int gpio_get_value(unsigned gpio)
{
	return camelot_gpio_get_value(gpio);
}

static inline void gpio_set_value(unsigned gpio, int value)
{
	camelot_gpio_set_value(gpio, value);
}

static inline int gpio_direction_input(unsigned gpio)
{
	return camelot_gpio_direction_input(gpio);
}

static inline int gpio_direction_output(unsigned gpio, int value)
{
	return camelot_gpio_direction_output(gpio, value);
}


/* cansleep wrappers */
#include <asm-generic/gpio.h>

#endif /* __ASM_CAMELOT_GPIO_H */
