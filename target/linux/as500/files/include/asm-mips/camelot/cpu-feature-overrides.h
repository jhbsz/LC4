/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved.
 *
 *  Camelot platform CPU feature overrides
 */
#ifndef __ASM_CAMELOT_CPU_FEATURE_OVERRIDES_H
#define __ASM_CAMELOT_CPU_FEATURE_OVERRIDES_H

#define cpu_has_tlb			1
#define cpu_has_4kex			0
#define cpu_has_3k_cache		0
#define cpu_has_4k_cache		0
#define cpu_has_tx39_cache		0
#define cpu_has_sb1_cache		0
#define cpu_has_fpu			0
#define cpu_has_32fpr			0
#define cpu_has_counter			1
#define cpu_has_watch			1
#define cpu_has_divec			0
#define cpu_has_vce			0
#define cpu_has_cache_cdex_p		0
#define cpu_has_cache_cdex_s		0
#define cpu_has_prefetch		0  /* to be confirm */
#define cpu_has_mcheck			1
#define cpu_has_ejtag			1
#define cpu_has_llsc			0
#define cpu_has_veic            0
#define cpu_has_vint            0

#define cpu_has_mips16			0
#define cpu_has_mdmx			0
#define cpu_has_mips3d			0
#define cpu_has_smartmips		0

#define cpu_has_vtag_icache		0
//#define cpu_has_dc_aliases
//#define cpu_has_ic_fills_f_dc
//#define cpu_has_pindexed_dcache

//#define cpu_icache_snoops_remote_store

#define cpu_has_mips32r1		0
#define cpu_has_mips32r2		0
#define cpu_has_mips64r1		0
#define cpu_has_mips64r2		0

#define cpu_has_dsp			0
#define cpu_has_mipsmt			0

//#define cpu_has_nofpuex
#define cpu_has_64bits			0
#define cpu_has_64bit_zero_reg		0
#define cpu_has_64bit_gp_regs		0
#define cpu_has_64bit_addresses		0

#define cpu_has_inclusive_pcaches	0

#define cpu_dcache_line_size()		32
#define cpu_icache_line_size()		32

#endif /* __ASM_CAMELOT_CPU_FEATURE_OVERRIDES_H */
