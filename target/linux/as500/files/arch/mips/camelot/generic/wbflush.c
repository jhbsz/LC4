/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved. 
 */
#include <linux/init.h>

#include <asm/bootinfo.h>
#include <asm/system.h>
#include <asm/wbflush.h>

static void wbflush_mips(void);

void (*__wbflush) (void);

void wbflush_setup(void)
{
	__wbflush = wbflush_mips;
}

/*
   CPUs that allow neither of these changes of sequence, peforming all reads and writes in program
   order, are called strongly ordered. Most MIPS CPUs, when configured as uniprocessors, are strongly
   ordered. But there are exceptions: Even some early R3000 systems would allow reads to overtake pdning
   writes. 
   On a MIPS III or subsequent CPU that is not strongly ordered, a sync instruction defines a 
   load/store barrier. You are guaranteed that all load/stores initiated before the sync will be
   seen before any load/store initiated afterward.
 */
static void wbflush_mips(void)
{
	__fast_iob();
}

#include <linux/module.h>

EXPORT_SYMBOL(__wbflush);

