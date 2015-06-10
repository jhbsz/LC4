/* 
 *	Camelot
 *
 */

#ifndef __CAMELOT_H__
#define __CAMELOT_H__

#include <asm/addrspace.h>

#define CAMELOT_REG_BASE	(0xaf000000)

#define	IRQ_TMR0	7
#define	IRQ_TMR1	6
#define	IRQ_UR	 	5
#define	IRQ_WDT		4
#define	IRQ_MAC		3	
#define IRQ_GPIO0   2

#define MI_BASE     CAMELOT_REG_BASE
#define	TM_BASE		(MI_BASE+0x1000)
#define UR_BASE		(MI_BASE+0x2000)
#define MAC_BASE	(MI_BASE+0x3000)
#define	IC_BASE		(MI_BASE+0x4000)
#define GPIO_BASE   (MI_BASE+0x5000)

#define	MR1C		0x00
#define	MR2C		0x04
#define	MSDC		0x10
#define	MSR1C		0x14
#define	MSR1A		0x18
#define	MSR1D		0x1c
#define	MSR2C		0x20
#define	MSR2A		0x24
#define	MSR2D		0x28

#ifdef	OLD_TIMER
#define	T0CR		(0x00/4)	/* current value */
#define	T0LR		(0x04/4)	/* reload value */
#define	T1CR		(0x08/4)
#define	T1LR		(0x0c/4)
#define	T2CR		(0x10/4)
#define	T2LR		(0x14/4)
#define	TPR			(0x18/4)	/* prescaler */
#define	TCN			(0x1c/4)	/* control */
#define	TIC			(0x20/4)	/* interrupt clear */
#else
#define	T0CR		(0x00/4)	/* current value */
#define	T0LR		(0x04/4)	/* reload value */
#define	T0PR		(0x08/4)	/* prescaler */
#define	T0CN		(0x0c/4)	/* control */
#define T0IS		(0x10/4)	/* interrupt status */
#define	T1CR		(0x14/4)	/* current value */
#define	T1LR		(0x18/4)	/* reload value */
#define	T1PR		(0x1c/4)	/* prescaler */
#define	T1CN		(0x20/4)	/* control */
#define T1IS		(0x24/4)	/* interrupt status */
#define	T2CR		(0x28/4)	/* current value */
#define	T2LR		(0x2c/4)	/* reload value */
#define	T2PR		(0x30/4)	/* prescaler */
#define	T2CN		(0x34/4)	/* control */
#define T2IS		(0x38/4)	/* interrupt status */
#endif


#define	ISTS		(000>>2)
#define	IMSK		(0x4>>2)
#define	ISR1		(0x8>>2)
#define	ISR2		(0xc>>2)

#define	URBR		0x0			/* tx/rx buffer register */
#define	URCS		0x4			/* control/status */

#define	URCS_TIE	(1<<15)
#define	URCS_RIE	(1<<14)		
#define	URCS_PE		(1<<13)		
#define	URCS_EVEN	(1<<12)
#define	URCS_SP2	(1<<11)
#define	URCS_LB		(1<<10)
#define	URCS_P		(1<<9)
#define	URCS_PER	(1<<8)
#define	URCS_FE		(1<<7)
#define	URCS_RTH_S	(4)			/* rx threshold */
#define	URCS_TF		(1<<3)		/* tx fifo full */
#define	URCS_TE		(1<<2)		/* tx empty */
#define	URCS_RF		(1<<1)		/* rx full */
#define	URCS_TB		(1<<0)		/* tx busy */

//#define	SYS_CLK		40625000
#define	SYS_CLK		150000000
#define UART_CLK	SYS_CLK

#endif
