#ifndef __ASM_CAMELOT_IRQ_H
#define __ASM_CAMELOT_IRQ_H

#define MIPS_CPU_IRQ_BASE       16
#define NR_IRQS                 24          //  16 VIC intrs + 8 CPU intrs

#define MIPSCPU_INT_CAMELOT_VIC		6
#define CAMELOT_VIC_IRQ_BASE		0


#include_next <irq.h>

#endif /* __ASM_CAMELOT_IRQ_H */

