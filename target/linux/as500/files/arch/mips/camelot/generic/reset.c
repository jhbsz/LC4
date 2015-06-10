/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved. 
 *
 *  Routines to reset the Camelot platform
 *
 */
#include <linux/pm.h>

#include <asm/io.h>
#include <asm/reboot.h>

#include <asm/camelot/camelot.h>

static void camelot_machine_restart(char *command);
static void camelot_machine_halt(void);
static void camelot_machine_power_off(void);

#if defined(CONFIG_CAMELOT_WDT)
extern void camelot_wdt_enable(void);
extern void camelot_wdt_disable(void);
#endif

static void camelot_machine_restart(char *command)
{
    volatile unsigned int *TimerReg = (unsigned int *) TM_BASE;

#if defined(CONFIG_CAMELOT_WDT)
    camelot_wdt_disable();
#endif

    TimerReg[T2PR]=1;
    TimerReg[T2LR]=10;
    TimerReg[T2CR]=0;
    TimerReg[T2CN]=0xc;   /* b3: watch-dog , b2: start */
}

static void camelot_machine_halt(void)
{
#if defined(CONFIG_CAMELOT_WDT)
    camelot_wdt_disable();
#endif

    local_irq_disable();
    while(1);
}

static void camelot_machine_power_off(void)
{
    printk("System halted. Please turn off power.\n");
    camelot_machine_halt();
}

void camelot_reboot_setup(void)
{
	_machine_restart = camelot_machine_restart;
	_machine_halt = camelot_machine_halt;
	pm_power_off = camelot_machine_power_off;
}

