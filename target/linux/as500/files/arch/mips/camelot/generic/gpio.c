/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved.
 */
#include <linux/module.h>

#include <asm/system.h>
#include <asm/camelot/camelot.h>
#include <asm/camelot/gpio.h>

#define GPVAL 0
#define GPSET 1
#define GPCLR 2
#define GPDIR 3
#define GPSEL 4

#define GPREG(reg) ((volatile unsigned long*)(GPIO_BASE))[reg]

/* internal function: change the shared pin to gpio mode */
static void __camelot_gpio_pin_enable(unsigned gpio)
{
	GPREG(GPSEL) |= ( 1 << gpio );
}

/* only GPIO0 support interrupt mode. It might not enabled, though. Check it on first usage! */
int camelot_gpio_to_irq(unsigned gpio)
{
	if(gpio==0)
	{
		printk("camelot_gpio_to_irq: gpio interrupt might not be enabled.\n");
		return IRQ_GPIO0;
	}
	else
		return -EINVAL;
}
EXPORT_SYMBOL(camelot_gpio_to_irq);

/* The values are boolean, zero for low, nonzero for high */
int camelot_gpio_get_value(unsigned gpio)
{
	unsigned long flags;
	int value;

	if(gpio < CAMELOT_GPIO_LINES)
	{
		local_irq_save(flags);

		__camelot_gpio_pin_enable(gpio);

		value = (GPREG(GPVAL) & (1 << gpio));

		local_irq_restore(flags);
		return value;
	} 
	else
		return 0;
}
EXPORT_SYMBOL(camelot_gpio_get_value);

void camelot_gpio_set_value(unsigned gpio, int value)
{
	unsigned long bit;
	unsigned long mask;
    unsigned long flags;

	if(gpio < CAMELOT_GPIO_LINES)
	{
		local_irq_save(flags);

		__camelot_gpio_pin_enable(gpio);

		bit = value ? (1 << gpio) : 0;
		mask = (1 << gpio);
	
		bit = (GPREG(GPVAL) & ~mask) | bit;
		GPREG(GPVAL) = bit;

		local_irq_restore(flags);
	}
}
EXPORT_SYMBOL(camelot_gpio_set_value);

int camelot_gpio_direction_input(unsigned gpio)
{
    unsigned long flags;

	if(gpio < CAMELOT_GPIO_LINES)
	{
		local_irq_save(flags);

		__camelot_gpio_pin_enable(gpio);

		GPREG(GPDIR) &= ~(1 << gpio);		/* set the corresponding bit to 0 for input */

		local_irq_restore(flags);
	}
	else
		return -EINVAL;

	return 0;
}
EXPORT_SYMBOL(camelot_gpio_direction_input);

int camelot_gpio_direction_output(unsigned gpio, int value)
{
    unsigned long flags;

	camelot_gpio_set_value(gpio, value);

	if(gpio < CAMELOT_GPIO_LINES)
	{
		local_irq_save(flags);

		GPREG(GPDIR) |= (1 << gpio);		/* set the corresponding bit to 1 for output */

		local_irq_restore(flags);
	}
	else
		return -EINVAL;

	return 0;
}
EXPORT_SYMBOL(camelot_gpio_direction_output);

