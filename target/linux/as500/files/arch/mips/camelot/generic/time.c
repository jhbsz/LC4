/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved. 
 */
#include <linux/clockchips.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>

#include <asm/io.h>
#include <asm/camelot/camelot.h>

#define CAMELOT_TIMER_IRQ   IRQ_TMR1

static void camelot_set_mode(enum clock_event_mode mode,
                             struct clock_event_device *evt)
{
    /* Nothing to do ...  */
}

struct clock_event_device camelot_clockevent = {
    .name       = "camelot",
    .features   = CLOCK_EVT_FEAT_PERIODIC,
    .rating     = 100,
    .irq        = CAMELOT_TIMER_IRQ,
    .set_mode   = camelot_set_mode,
    .mult       = 1,
};

#define ISTS_VAL        (1<<IRQ_TMR1)                   /* timer1 */
#define TCN_INI         (1<<2)|(1<<1)|(1<<0)    /* b2: go, b1: ie, b0: auto-reload */
#define T1PR_INI        0xf
#define TICKS_PER_SEC   (SYS_CLK/(T1PR+1))
#define TICKS_PER_JIFFY (TICKS_PER_SEC/(HZ*2))

//static __init 
void camelot_timer_init(void)
{
    volatile unsigned int *pt = (unsigned int *) TM_BASE;

    pt[T1LR] = TICKS_PER_JIFFY;
    pt[T1PR] = T1PR_INI;
    pt[T1CN] = TCN_INI;

    //fill imem
    //set_cctl(CCTL_IMEMFILL);
}

static irqreturn_t camelot_timer_interrupt(int irq, void *dev_id)
{
    struct clock_event_device *cd = dev_id;

    volatile unsigned int *pt = (unsigned int *) TM_BASE ;
    //volatile unsigned int *ic = (unsigned int *) IC_BASE ;

    pt[T1IS]=1;
    //ic[ISTS]=(1<<IRQ_TMR1);

    #if 0
    {
        static int count = 0;
        count++;
        if((count %100) == 0)
            printk("here %d\n", count);
    }
    #endif

    cd->event_handler(cd);

    return IRQ_HANDLED;
}

static struct irqaction camelot_timer_irqaction = {
    .handler    = camelot_timer_interrupt,
    .flags      = IRQF_DISABLED,
    .mask       = CPU_MASK_NONE,
    .name       = "Camelot timer",
};

void __init plat_time_init(void)
{
    struct clock_event_device *cd = &camelot_clockevent;
    struct irqaction *action = &camelot_timer_irqaction;
    unsigned int cpu = smp_processor_id();

    BUG_ON(HZ != 100);

    cd->cpumask             = cpumask_of_cpu(cpu);

    clockevents_register_device(cd);
    action->dev_id = cd;
    setup_irq(CAMELOT_TIMER_IRQ, action);

    camelot_timer_init();
}

