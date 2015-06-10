/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved. 
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel_stat.h>
#include <linux/kernel.h>
#include <linux/random.h>

#include <asm/traps.h>
#include <asm/irq_cpu.h>
#include <asm/irq_regs.h>
#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/io.h>
#include <asm/camelot/irq.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <asm/camelot/camelot.h>

#if 0
void camelot_irqdispatch(void);

/*
 * Version of ffs that only looks at bits 12..15.
 */
static inline unsigned int irq_ffs(unsigned int pending)
{
    unsigned int a0 = 7;
    unsigned int t0;

    t0 = pending & 0xf000;
    t0 = t0 < 1;
    t0 = t0 << 2;
    a0 = a0 - t0;
    pending = pending << t0;

    t0 = pending & 0xc000;
    t0 = t0 < 1;
    t0 = t0 << 1;
    a0 = a0 - t0;
    pending = pending << t0;

    t0 = pending & 0x8000;
    t0 = t0 < 1;
    /* t0 = t0 << 2; */
    a0 = a0 - t0;
    /* pending = pending << t0; */

    return a0;
}
#endif
/*
 * IRQs on the Malta board look basically (barring software IRQs which we
 * don't use at all and all external interrupt sources are combined together
 * on hardware interrupt 0 (MIPS IRQ 2)) like:
 *
 *	MIPS IRQ	Source
 *      --------        ------
 *             0	Software (ignored)
 *             1        Software (ignored)
 *             2        Combined hardware interrupt (hw0)
 *             3        Hardware (ignored)
 *             4        Hardware (ignored)
 *             5        Hardware (ignored)
 *             6        Hardware (ignored)
 *             7        R4k timer (what we use)
 *
 * We handle the IRQ according to _our_ priority which is:
 *
 * Highest ----     R4k Timer
 * Lowest  ----     Combined hardware interrupt
 *
 * then we just return, if multiple IRQs are pending then we will just take
 * another exception, big deal.
 */

asmlinkage void plat_irq_dispatch(void)
{
    unsigned int pending = ( read_c0_cause() & read_c0_status())>>8;  //take b15-8
    int i;

	// test each bit 
	for (i=7; i>1; i--)
		if (pending& (1<<i))
        	do_IRQ(i);
}

#ifdef CONFIG_MIPS_MT_SMP
    #error CONFIG_MIPS_MT_SMP not supported!
#endif

#define ALLINTS (IE_IRQ0 | IE_IRQ1 | IE_IRQ2 | IE_IRQ3 | IE_IRQ4 | IE_IRQ5)

static DEFINE_SPINLOCK(camelot_irq_lock);

static inline void camelot_mask_irq(unsigned int irq_nr)
{
    unsigned long flags;
    volatile unsigned int *ic = (unsigned int *) IC_BASE;

    spin_lock_irqsave(&camelot_irq_lock, flags);
    ic[IMSK] |= (1 << irq_nr);
    spin_unlock_irqrestore(&camelot_irq_lock, flags);
}

static inline void camelot_ack_irq(unsigned int irq_nr)
{
    unsigned long flags;
    volatile unsigned int *ic = (unsigned int *) IC_BASE;

    spin_lock_irqsave(&camelot_irq_lock, flags);
    ic[ISTS] |= (1 << irq_nr);
    spin_unlock_irqrestore(&camelot_irq_lock, flags);
}

static inline void camelot_mask_ack_irq(unsigned int irq_nr)
{
    unsigned long flags;
    volatile unsigned int *ic = (unsigned int *) IC_BASE;

    spin_lock_irqsave(&camelot_irq_lock, flags);
    ic[IMSK] |= (1 << irq_nr);
    ic[ISTS] |= (1 << irq_nr);
    spin_unlock_irqrestore(&camelot_irq_lock, flags);
}



static inline void camelot_unmask_irq(unsigned int irq_nr)
{
    unsigned long flags;
    volatile unsigned int *ic = (unsigned int *) IC_BASE;

    spin_lock_irqsave(&camelot_irq_lock, flags);
    ic[IMSK] &= ~(1 << irq_nr);
    spin_unlock_irqrestore(&camelot_irq_lock, flags);

}
/*
static struct irqaction camelot_irqaction = {
    .handler = no_action,
    .name = "Camelot VIC",
};
*/
static struct irq_chip camelot_irq_controller = {
    .name       = "Camelot VIC",
    .ack        = camelot_ack_irq,
    .mask       = camelot_mask_irq,
    .mask_ack   = camelot_mask_ack_irq,
    .unmask     = camelot_unmask_irq,
    .eoi        = camelot_unmask_irq,
};

#if 0 
void camelot_irqdispatch(void)
{
#if 0
    volatile unsigned int *ic = (unsigned int *) IC_BASE;
    printk("camelot_irqdispatch ISR1 %x ISR2 %x IMSK %x ISTS %x\n"
           ,ic[ISR1],ic[ISR2],ic[IMSK],ic[ISTS] );
#else
    int irq;

    irq = 6;

    /* XXX: need to check if interrupt has already been cleared ? */

    do_IRQ( IRQ_TMR1); //CAMELOT_VIC_IRQ_BASE + irq);
#endif
}
#endif

void __init init_camelot_irqs(void)
{
    volatile unsigned int *ic = (unsigned int *) IC_BASE;
    unsigned int i;

    /* Disable all hardware interrupts */
    change_c0_status(ST0_IM, 0x00);

    ic[ISR1]=0;
    ic[ISR2]=0xfe98d000;          /* mac=0xd */

    /* Clear interrupts */
    /* Disable all CAMELOT interrupts. */
    ic[IMSK]=0xffffffff;          /* b31-16: pol , b15-0: mask */
    ic[ISTS]=0xffffffff;          /* b31-15: cause, b15-0: status */

    /* Initialize IRQ action handlers */
    for (i = 0; i < 16; i++)
    {
        set_irq_chip_and_handler(i, &camelot_irq_controller, handle_level_irq);
    }

    /* Enable all interrupts */
    change_c0_status(ST0_IM, ALLINTS);
}



void __init arch_init_irq(void)
{
    init_camelot_irqs();

#if 0
    mips_cpu_irq_init();
    set_vi_handler(MIPSCPU_INT_CAMELOT_VIC, camelot_irqdispatch);
    setup_irq(MIPS_CPU_IRQ_BASE+MIPSCPU_INT_CAMELOT_VIC, &camelot_irqaction);

    /* enable camelot vic */
    change_c0_status(ST0_IM, IE_IRQ4);
#endif
#if defined(CONFIG_MIPS_MT_SMP)
#error CONFIG_MIPS_MT_SMP not supported!
#endif
}

