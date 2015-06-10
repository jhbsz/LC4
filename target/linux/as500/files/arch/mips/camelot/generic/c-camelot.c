/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved. 
 *
 *  Camelot specific mmu/cache code.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/mmu_context.h>
#include <asm/system.h>
#include <asm/isadep.h>
#include <asm/io.h>
#include <asm/bootinfo.h>
#include <asm/cpu.h>
#include <asm/cpu-features.h>
#include <asm/mipsregs.h>
#include <asm/r4kcache.h>

static unsigned long icache_size, dcache_size;		/* Size in bytes */
static unsigned long icache_lsize, dcache_lsize;	/* Size in bytes */


static void __cpuinit camelot_probe_cache(void)
{
	dcache_size = 4096;
	icache_size = 4096;
	icache_lsize = cpu_icache_line_size();
	dcache_lsize = cpu_dcache_line_size();
}

/*
 * Cache Control Register fields
 */
#define CCTL_DINVAL				0x00000001
#define CCTL_IINVAL				0x00000002
#define CCTL_ILOCK				0x0000000c
#define CCTL_IMEMFILL			0x00000010
#define CCTL_IMEMOFF			0x00000020
#define CCTL_DWB				0x00000100
#define CCTL_DWBINVAL			0x00000200
#define CCTL_DMEMON				0x00000400
#define CCTL_DMEMOFF			0x00000800

#define nop()			__asm__ volatile("nop")

void camelot_dcache_wback_invalidate_all(void)
{
	volatile unsigned int cctl_reg;
	unsigned long flags;

	local_save_flags(flags);
	local_irq_disable();		


	cctl_reg = read_c0_cctl();
	nop(); nop();

	write_c0_cctl( cctl_reg &~ CCTL_DWBINVAL );
	nop(); nop();
	write_c0_cctl( cctl_reg | CCTL_DWBINVAL );
	nop(); nop();

	/* delay to allow cache to be flushed */
	__asm__ __volatile__(".set\tnoreorder\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t" "nop\n\t" ".set\treorder\n\t");


	write_c0_cctl( cctl_reg );
	nop(); nop();

	local_irq_restore(flags);
}

void camelot_dcache_invalidate_all(void)
{
	volatile unsigned int cctl_reg;
	unsigned long flags;

	local_save_flags(flags);
	local_irq_disable();		


	cctl_reg = read_c0_cctl();
	nop(); nop();

	write_c0_cctl( cctl_reg &~ CCTL_DINVAL );
	nop(); nop();
	write_c0_cctl( cctl_reg | CCTL_DINVAL );
	nop(); nop();

	/* delay to allow cache to be flushed */
	__asm__ __volatile__(".set\tnoreorder\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t" "nop\n\t" ".set\treorder\n\t");


	write_c0_cctl( cctl_reg );
	nop(); nop();

	local_irq_restore(flags);
}

void camelot_dcache_wback_all(void)
{
	volatile unsigned int cctl_reg;
	unsigned long flags;

	local_save_flags(flags);
	local_irq_disable();		


	cctl_reg = read_c0_cctl();
	nop(); nop();

	write_c0_cctl( cctl_reg &~ CCTL_DWB );
	nop(); nop();
	write_c0_cctl( cctl_reg | CCTL_DWB );
	nop(); nop();

	/* delay to allow cache to be flushed */
	__asm__ __volatile__(".set\tnoreorder\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t" "nop\n\t" ".set\treorder\n\t");


	write_c0_cctl( cctl_reg );
	nop(); nop();

	local_irq_restore(flags);
}

void camelot_icache_invalidate_all(void)
{
	volatile unsigned int cctl_reg;
	unsigned long flags;

	local_save_flags(flags);
	local_irq_disable();		


	cctl_reg = read_c0_cctl();
	nop(); nop();

	write_c0_cctl( cctl_reg &~ CCTL_IINVAL );
	nop(); nop();
	write_c0_cctl( cctl_reg | CCTL_IINVAL );
	nop(); nop();

	/* delay to allow cache to be flushed */
	__asm__ __volatile__(".set\tnoreorder\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t"
						 "nop\n\t" "nop\n\t" ".set\treorder\n\t");


	write_c0_cctl( cctl_reg );
	nop(); nop();

	local_irq_restore(flags);
}

static void camelot_flush_cache_all(void)
{
	camelot_dcache_wback_invalidate_all();
	camelot_icache_invalidate_all();
}

static void camelot_flush_icache_range(unsigned long start, unsigned long end)
{
	/* implement by invaliding the whole icache in camelot platform */
	camelot_icache_invalidate_all();
}

static void camelot_flush_dcache_range(unsigned long start, unsigned long end)
{
	int size = end - start;

	if (size >= dcache_size) {
		camelot_dcache_wback_invalidate_all();
	} else {
		blast_dcache_range(start, start + size);
	}
}

static void camelot_flush_cache_mm(struct mm_struct *mm)
{
	/* checked and left to be blank */
}

static void camelot_flush_cache_range(struct vm_area_struct *vma,
				  unsigned long start, unsigned long end)
{
	/* checked to left to be blank */
}

static void camelot_flush_cache_page(struct vm_area_struct *vma,
				 unsigned long addr, unsigned long pfn)
{
	unsigned long kaddr = KSEG0ADDR(pfn << PAGE_SHIFT);
	int exec = vma->vm_flags & VM_EXEC;
	struct mm_struct *mm = vma->vm_mm;
	pgd_t *pgdp;
	pud_t *pudp;
	pmd_t *pmdp;
	pte_t *ptep;

	pr_debug("cpage[%08lx,%08lx]\n",
		 cpu_context(smp_processor_id(), mm), addr);

	/* No ASID => no such page in the cache.  */
	if (cpu_context(smp_processor_id(), mm) == 0)
		return;

	pgdp = pgd_offset(mm, addr);
	pudp = pud_offset(pgdp, addr);
	pmdp = pmd_offset(pudp, addr);
	ptep = pte_offset(pmdp, addr);

	/* Invalid => no such page in the cache.  */
	if (!(pte_val(*ptep) & _PAGE_PRESENT))
		return;

	camelot_flush_dcache_range(kaddr, kaddr + PAGE_SIZE);
	if (exec)
		camelot_flush_icache_range(kaddr, kaddr + PAGE_SIZE);
}

static void local_camelot_flush_data_cache_page(void *addr)
{
	if(PAGE_SIZE >= dcache_size)
	{
		camelot_dcache_wback_invalidate_all();
	}
	else
	{
		if(dcache_lsize == 32) 
			blast_dcache32_page((unsigned long) addr);
		else if(dcache_lsize == 16)
			blast_dcache16_page((unsigned long) addr);
		else if(dcache_lsize == 0)
			return;
		else
			BUG_ON(1);
	}
}

static void camelot_flush_data_cache_page(unsigned long addr)
{
	local_camelot_flush_data_cache_page((void*) addr);
}


static void camelot_flush_cache_sigtramp(unsigned long addr)
{
	/*
	 * Traditional R3000 processors flush the first three
	 * words ot the cache starting at addr. We must flush
	 * the entire cache.
	 */

	camelot_icache_invalidate_all();
}

static void camelot_dma_cache_inv(unsigned long start, unsigned long size)
{
	/* Catch bad driver code */
	BUG_ON(size == 0);

	iob();

	if (size >= dcache_size) {
		camelot_dcache_invalidate_all();
	} else {
		blast_inv_dcache_range(start, start + size);
	}

	//camelot_flush_dcache_range(start, start + size);
}

static void camelot_dma_cache_wback(unsigned long start, unsigned long size)
{
	/* Catch bad driver code */
	BUG_ON(size == 0);

	iob();

	if (size >= dcache_size) {
		camelot_dcache_wback_all();
	} else {
		/* use wback invalidate instead of wback */
		blast_dcache_range(start, start + size);
	}

	//camelot_flush_dcache_range(start, start + size);
}

static void camelot_dma_cache_wback_inv(unsigned long start, unsigned long size)
{
	/* Catch bad driver code */
	BUG_ON(size == 0);

	iob();

	if (size >= dcache_size) {
		camelot_dcache_wback_invalidate_all();
	} else {
		blast_dcache_range(start, start + size);
	}
}

void __cpuinit camelot_cache_init(void)
{
	extern void build_clear_page(void);
	extern void build_copy_page(void);

	camelot_probe_cache();

	wbflush_setup();

	flush_cache_all = camelot_flush_cache_all;
	__flush_cache_all = camelot_flush_cache_all;
	flush_cache_mm = camelot_flush_cache_mm;
	flush_cache_range = camelot_flush_cache_range;
	flush_cache_page = camelot_flush_cache_page;
	flush_icache_range = camelot_flush_icache_range;
	/* local_flush_icache_range = camelot_flush_icache_range; */

	flush_cache_sigtramp = camelot_flush_cache_sigtramp;
	local_flush_data_cache_page = local_camelot_flush_data_cache_page;
	flush_data_cache_page = camelot_flush_data_cache_page;

	_dma_cache_wback_inv = camelot_dma_cache_wback_inv;
	_dma_cache_wback = camelot_dma_cache_wback;
	_dma_cache_inv = camelot_dma_cache_inv;

	printk("Primary instruction cache %ldkB, linesize %ld bytes.\n",
		icache_size >> 10, icache_lsize);
	printk("Primary data cache %ldkB, linesize %ld bytes.\n",
		dcache_size >> 10, dcache_lsize);

	build_clear_page();
	build_copy_page();
}

