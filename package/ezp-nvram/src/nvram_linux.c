/*
 * NVRAM variable manipulation (Linux user mode half)
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: nvram_linux.c 4726 2006-09-02 18:37:11Z nbd $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <typedefs.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <utils.h>
#include "ezp-lib.h"
#include "nvram_fw.h"
#include "nvram_ezpacket.h"
#include "nvram_data.h"

/* 3G/4G Wireless broadband device definition. */
#if defined(EZP_PROD_CAT_M) || defined(EZP_PROD_CAT_N) || defined(EZP_PROD_CAT_P) || defined(EZP_PROD_CAT_U)
#include "ezpcom-lib.h"
#endif

#define PATH_DEV_NVRAM "/dev/nvram"
#define NVRAM_TMP_LEN 4096

#ifndef EZP_PROD_VERSION
#error "Undefinied firmware version"
#endif


/* Globals */
#if defined(NVRAM_MMAP) && NVRAM_MMAP > 0
static char *nvram_buf = NULL;
#endif
static int nvram_fd = -1;
int check_action(void);
int file_to_buf(char *path, char *buf, int len);

/* Firmware. */
int fw_func_1_6_5_to_0_0_0(void);
int fw_func_0_0_0_to_1_6_5(void);
/* 1.6.6 */
int fw_func_1_6_5_to_1_6_6(void);
int fw_func_1_6_6_to_1_6_5(void);
/* 1.6.7 */
int fw_func_1_6_6_to_1_6_7(void);
int fw_func_1_6_7_to_1_6_6(void);
/* 1.7.0 */
int fw_func_1_6_7_to_1_7_0(void);
int fw_func_1_7_0_to_1_6_7(void);
/* 1.7.1 */
int fw_func_1_7_0_to_1_7_1(void);
int fw_func_1_7_1_to_1_7_0(void);
/* 1.7.2 */
int fw_func_1_7_1_to_1_7_2(void);
int fw_func_1_7_2_to_1_7_1(void);
/* 1.7.3 */
int fw_func_1_7_2_to_1_7_3(void);
int fw_func_1_7_3_to_1_7_2(void);
/* 1.7.4 */
int fw_func_1_7_3_to_1_7_4(void);
int fw_func_1_7_4_to_1_7_3(void);
/* 1.7.5 */
int fw_func_1_7_4_to_1_7_5(void);
int fw_func_1_7_5_to_1_7_4(void);
/* 1.7.5 */
int fw_func_1_7_4_to_1_7_5(void);
int fw_func_1_7_5_to_1_7_4(void);
/* 1.7.6 */
int fw_func_1_7_5_to_1_7_6(void);
int fw_func_1_7_6_to_1_7_5(void);
/* 1.7.7 */
int fw_func_1_7_6_to_1_7_7(void);
int fw_func_1_7_7_to_1_7_6(void);
/* 1.7.8 */
int fw_func_1_7_7_to_1_7_8(void);
int fw_func_1_7_8_to_1_7_7(void);
/* 1.7.9 */
int fw_func_1_7_8_to_1_7_9(void);
int fw_func_1_7_9_to_1_7_8(void);
/* 1.7.10 */
int fw_func_1_7_9_to_1_7_10(void);
int fw_func_1_7_10_to_1_7_9(void);
/* 1.7.11 */
int fw_func_1_7_10_to_1_7_11(void);
int fw_func_1_7_11_to_1_7_10(void);
/* 2.0.0 */
int fw_func_1_7_11_to_2_0_0(void);
int fw_func_2_0_0_to_1_7_11(void);
/* 2.0.1 */
int fw_func_2_0_0_to_2_0_1(void);
int fw_func_2_0_1_to_2_0_0(void);
/* 2.0.2 */
int fw_func_2_0_1_to_2_0_2(void);
int fw_func_2_0_2_to_2_0_1(void);
/* 2.0.3 */
int fw_func_2_0_2_to_2_0_3(void);
int fw_func_2_0_3_to_2_0_2(void);
/* 2.0.4 */
int fw_func_2_0_3_to_2_0_4(void);
int fw_func_2_0_4_to_2_0_3(void);
/* 2.0.5 */
int fw_func_2_0_4_to_2_0_5(void);
int fw_func_2_0_5_to_2_0_4(void);
/* 2.0.6 */
int fw_func_2_0_5_to_2_0_6(void);
int fw_func_2_0_6_to_2_0_5(void);
/* 2.0.7 */
int fw_func_2_0_6_to_2_0_7(void);
int fw_func_2_0_7_to_2_0_6(void);
/* 2.0.8 */
int fw_func_2_0_7_to_2_0_8(void);
int fw_func_2_0_8_to_2_0_7(void);
/* 2.0.9 */
int fw_func_2_0_8_to_2_0_9(void);
int fw_func_2_0_9_to_2_0_8(void);
/* 2.0.10 */
int fw_func_2_0_9_to_2_0_10(void);
int fw_func_2_0_10_to_2_0_9(void);
/* 2.0.11 */
int fw_func_2_0_10_to_2_0_11(void);
int fw_func_2_0_11_to_2_0_10(void);
/* 2.0.12 */
int fw_func_2_0_11_to_2_0_12(void);
int fw_func_2_0_12_to_2_0_11(void);
/* 2.0.13 */
int fw_func_2_0_12_to_2_0_13(void);
int fw_func_2_0_13_to_2_0_12(void);
/* 2.0.14 */
int fw_func_2_0_13_to_2_0_14(void);
int fw_func_2_0_14_to_2_0_13(void);
/* 2.0.15 */
int fw_func_2_0_14_to_2_0_15(void);
int fw_func_2_0_15_to_2_0_14(void);
/* 3.0.0 */
int fw_func_2_0_15_to_3_0_0(void);
int fw_func_3_0_0_to_2_0_15(void);
/* 3.0.1 */
int fw_func_3_0_0_to_3_0_1(void);
int fw_func_3_0_1_to_3_0_0(void);
/* 3.0.2 */
int fw_func_3_0_1_to_3_0_2(void);
int fw_func_3_0_2_to_3_0_1(void);
/* 3.0.3*/
int fw_func_3_0_2_to_3_0_3(void);
int fw_func_3_0_3_to_3_0_2(void);
/* 3.0.4 */
int fw_func_3_0_3_to_3_0_4(void);
int fw_func_3_0_4_to_3_0_3(void);
/* 3.0.5 */
int fw_func_3_0_4_to_3_0_5(void);
int fw_func_3_0_5_to_3_0_4(void);
/* 3.0.6 */
int fw_func_3_0_5_to_3_0_6(void);
int fw_func_3_0_6_to_3_0_5(void);
/* 3.0.7 */
int fw_func_3_0_6_to_3_0_7(void);
int fw_func_3_0_7_to_3_0_6(void);
/* 3.0.8 */
int fw_func_3_0_7_to_3_0_8(void);
int fw_func_3_0_8_to_3_0_7(void);
/* 3.0.9 */
int fw_func_3_0_8_to_3_0_9(void);
int fw_func_3_0_9_to_3_0_8(void);
/* 3.0.10 */
int fw_func_3_0_9_to_3_0_10(void);
int fw_func_3_0_10_to_3_0_9(void);
/* 3.0.11 */
int fw_func_3_0_10_to_3_0_11(void);
int fw_func_3_0_11_to_3_0_10(void);
/* 3.0.12 */
int fw_func_3_0_11_to_3_0_12(void);
int fw_func_3_0_12_to_3_0_11(void);
/* 3.0.13 */
int fw_func_3_0_12_to_3_0_13(void);
int fw_func_3_0_13_to_3_0_12(void);
/* 3.0.14 */
int fw_func_3_0_13_to_3_0_14(void);
int fw_func_3_0_14_to_3_0_13(void);
/* 3.0.15 */
int fw_func_3_0_14_to_3_0_15(void);
int fw_func_3_0_15_to_3_0_14(void);
/* 3.0.16 */
int fw_func_3_0_15_to_3_0_16(void);
int fw_func_3_0_16_to_3_0_15(void);
/* 3.0.17 */
int fw_func_3_0_16_to_3_0_17(void);
int fw_func_3_0_17_to_3_0_16(void);
/* 3.0.18 */
int fw_func_3_0_17_to_3_0_18(void);
int fw_func_3_0_18_to_3_0_17(void);
/* 3.0.19 */
int fw_func_3_0_18_to_3_0_19(void);
int fw_func_3_0_19_to_3_0_18(void);
/* 3.0.20 */
int fw_func_3_0_19_to_3_0_20(void);
int fw_func_3_0_20_to_3_0_19(void);
/* 3.1.0 */
int fw_func_3_0_20_to_3_1_0(void);
int fw_func_3_1_0_to_3_0_20(void);
/* 3.1.1 */
int fw_func_3_1_0_to_3_1_1(void);
int fw_func_3_1_1_to_3_1_0(void);
/* 3.1.2 */
int fw_func_3_1_1_to_3_1_2(void);
int fw_func_3_1_2_to_3_1_1(void);
/* 3.1.3 */
int fw_func_3_1_2_to_3_1_3(void);
int fw_func_3_1_3_to_3_1_2(void);
/* 3.1.4 */
int fw_func_3_1_3_to_3_1_4(void);
int fw_func_3_1_4_to_3_1_3(void);
/* 3.1.5 */
int fw_func_3_1_4_to_3_1_5(void);
int fw_func_3_1_5_to_3_1_4(void);
/* 3.1.6 */
int fw_func_3_1_5_to_3_1_6(void);
int fw_func_3_1_6_to_3_1_5(void);
/* 3.1.7 */
int fw_func_3_1_6_to_3_1_7(void);
int fw_func_3_1_7_to_3_1_6(void);
/* 3.1.8 */
int fw_func_3_1_7_to_3_1_8(void);
int fw_func_3_1_8_to_3_1_7(void);
/* 3.1.9 */
int fw_func_3_1_8_to_3_1_9(void);
int fw_func_3_1_9_to_3_1_8(void);
/* 3.1.10 */
int fw_func_3_1_9_to_3_1_10(void);
int fw_func_3_1_10_to_3_1_9(void);
/* 3.2.0 */
int fw_func_3_1_9_to_3_2_0(void);
int fw_func_3_2_0_to_3_1_9(void);
/* 3.2.1 */
int fw_func_3_2_0_to_3_2_1(void);
int fw_func_3_2_1_to_3_2_0(void);
/* 3.2.2 */
int fw_func_3_2_1_to_3_2_2(void);
int fw_func_3_2_2_to_3_2_1(void);
struct nvram_fw_tuple nvram_fw_table[] = {
    { "0.0.0", NVRAM_FW_0_0_0, NULL, NULL },   
    { "1.6.5", NVRAM_FW_1_6_5, fw_func_0_0_0_to_1_6_5, fw_func_1_6_5_to_0_0_0 },
    { "1.6.6", NVRAM_FW_1_6_6, fw_func_1_6_5_to_1_6_6, fw_func_1_6_6_to_1_6_5 },
    { "1.6.7", NVRAM_FW_1_6_7, fw_func_1_6_6_to_1_6_7, fw_func_1_6_7_to_1_6_6 },
    { "1.7.0", NVRAM_FW_1_7_0, fw_func_1_6_7_to_1_7_0, fw_func_1_7_0_to_1_6_7 },
    { "1.7.1", NVRAM_FW_1_7_1, fw_func_1_7_0_to_1_7_1, fw_func_1_7_1_to_1_7_0 },
    { "1.7.2", NVRAM_FW_1_7_2, fw_func_1_7_1_to_1_7_2, fw_func_1_7_2_to_1_7_1 },
    { "1.7.3", NVRAM_FW_1_7_3, fw_func_1_7_2_to_1_7_3, fw_func_1_7_3_to_1_7_2 },
    { "1.7.4", NVRAM_FW_1_7_4, fw_func_1_7_3_to_1_7_4, fw_func_1_7_4_to_1_7_3 },
    { "1.7.5", NVRAM_FW_1_7_5, fw_func_1_7_4_to_1_7_5, fw_func_1_7_5_to_1_7_4 },
    { "1.7.6", NVRAM_FW_1_7_6, fw_func_1_7_5_to_1_7_6, fw_func_1_7_6_to_1_7_5 },
    { "1.7.7", NVRAM_FW_1_7_7, fw_func_1_7_6_to_1_7_7, fw_func_1_7_7_to_1_7_6 },
    { "1.7.8", NVRAM_FW_1_7_8, fw_func_1_7_7_to_1_7_8, fw_func_1_7_8_to_1_7_7 },
    { "1.7.9", NVRAM_FW_1_7_9, fw_func_1_7_8_to_1_7_9, fw_func_1_7_9_to_1_7_8 },
    { "1.7.10", NVRAM_FW_1_7_10, fw_func_1_7_9_to_1_7_10,
        fw_func_1_7_10_to_1_7_9 },
    { "1.7.11", NVRAM_FW_1_7_11, fw_func_1_7_10_to_1_7_11,
        fw_func_1_7_11_to_1_7_10 },
    { "2.0.0", NVRAM_FW_2_0_0, fw_func_1_7_11_to_2_0_0, fw_func_2_0_0_to_1_7_11 },
    { "2.0.1", NVRAM_FW_2_0_1, fw_func_2_0_0_to_2_0_1, fw_func_2_0_1_to_2_0_0 },
    { "2.0.2", NVRAM_FW_2_0_2, fw_func_2_0_1_to_2_0_2, fw_func_2_0_2_to_2_0_1 },
    { "2.0.3", NVRAM_FW_2_0_3, fw_func_2_0_2_to_2_0_3, fw_func_2_0_3_to_2_0_2 },
    { "2.0.4", NVRAM_FW_2_0_4, fw_func_2_0_3_to_2_0_4, fw_func_2_0_4_to_2_0_3 },
    { "2.0.5", NVRAM_FW_2_0_5, fw_func_2_0_4_to_2_0_5, fw_func_2_0_5_to_2_0_4 },
    { "2.0.6", NVRAM_FW_2_0_6, fw_func_2_0_5_to_2_0_6, fw_func_2_0_6_to_2_0_5 },
    { "2.0.7", NVRAM_FW_2_0_7, fw_func_2_0_6_to_2_0_7, fw_func_2_0_7_to_2_0_6 },
    { "2.0.8", NVRAM_FW_2_0_8, fw_func_2_0_7_to_2_0_8, fw_func_2_0_8_to_2_0_7 },
    { "2.0.9", NVRAM_FW_2_0_9, fw_func_2_0_8_to_2_0_9, fw_func_2_0_9_to_2_0_8 },
    { "2.0.10", NVRAM_FW_2_0_10, fw_func_2_0_9_to_2_0_10, fw_func_2_0_10_to_2_0_9 },
    { "2.0.11", NVRAM_FW_2_0_11, fw_func_2_0_10_to_2_0_11, fw_func_2_0_11_to_2_0_10 },
    { "2.0.12", NVRAM_FW_2_0_12, fw_func_2_0_11_to_2_0_12, fw_func_2_0_12_to_2_0_11 },
    { "2.0.12", NVRAM_FW_2_0_13, fw_func_2_0_12_to_2_0_13, fw_func_2_0_13_to_2_0_12 },
    { "2.0.12", NVRAM_FW_2_0_14, fw_func_2_0_13_to_2_0_14, fw_func_2_0_14_to_2_0_13 },
    { "2.0.12", NVRAM_FW_2_0_15, fw_func_2_0_14_to_2_0_15, fw_func_2_0_15_to_2_0_14 },
    { "3.0.0", NVRAM_FW_3_0_0, fw_func_2_0_15_to_3_0_0, fw_func_3_0_0_to_2_0_15 },
    { "3.0.1", NVRAM_FW_3_0_1, fw_func_3_0_0_to_3_0_1, fw_func_3_0_1_to_3_0_0 },
    { "3.0.2", NVRAM_FW_3_0_2, fw_func_3_0_1_to_3_0_2, fw_func_3_0_2_to_3_0_1 },
    { "3.0.3", NVRAM_FW_3_0_3, fw_func_3_0_2_to_3_0_3, fw_func_3_0_3_to_3_0_2 },
    { "3.0.4", NVRAM_FW_3_0_4, fw_func_3_0_3_to_3_0_4, fw_func_3_0_4_to_3_0_3 },
    { "3.0.5", NVRAM_FW_3_0_5, fw_func_3_0_4_to_3_0_5, fw_func_3_0_5_to_3_0_4 },
    { "3.0.6", NVRAM_FW_3_0_6, fw_func_3_0_5_to_3_0_6, fw_func_3_0_6_to_3_0_5 },
    { "3.0.7", NVRAM_FW_3_0_7, fw_func_3_0_6_to_3_0_7, fw_func_3_0_7_to_3_0_6 },
    { "3.0.8", NVRAM_FW_3_0_8, fw_func_3_0_7_to_3_0_8, fw_func_3_0_8_to_3_0_7 },
    { "3.0.9", NVRAM_FW_3_0_9, fw_func_3_0_8_to_3_0_9, fw_func_3_0_9_to_3_0_8 },
    { "3.0.10", NVRAM_FW_3_0_10, fw_func_3_0_9_to_3_0_10, fw_func_3_0_10_to_3_0_9 },
    { "3.0.11", NVRAM_FW_3_0_11, fw_func_3_0_10_to_3_0_11, fw_func_3_0_11_to_3_0_10 },
    { "3.0.12", NVRAM_FW_3_0_12, fw_func_3_0_11_to_3_0_12, fw_func_3_0_12_to_3_0_11 },
    { "3.0.13", NVRAM_FW_3_0_13, fw_func_3_0_12_to_3_0_13, fw_func_3_0_13_to_3_0_12 },
    { "3.0.14", NVRAM_FW_3_0_14, fw_func_3_0_13_to_3_0_14, fw_func_3_0_14_to_3_0_13 },
    { "3.0.15", NVRAM_FW_3_0_15, fw_func_3_0_14_to_3_0_15, fw_func_3_0_15_to_3_0_14 },
    { "3.0.16", NVRAM_FW_3_0_16, fw_func_3_0_15_to_3_0_16, fw_func_3_0_16_to_3_0_15 },
    { "3.0.17", NVRAM_FW_3_0_17, fw_func_3_0_16_to_3_0_17, fw_func_3_0_17_to_3_0_16 },
    { "3.0.18", NVRAM_FW_3_0_18, fw_func_3_0_17_to_3_0_18, fw_func_3_0_18_to_3_0_17 },
    { "3.0.19", NVRAM_FW_3_0_19, fw_func_3_0_18_to_3_0_19, fw_func_3_0_19_to_3_0_18 },
    { "3.0.20", NVRAM_FW_3_0_20, fw_func_3_0_19_to_3_0_20, fw_func_3_0_20_to_3_0_19 },
    { "3.1.0", NVRAM_FW_3_1_0, fw_func_3_0_20_to_3_1_0, fw_func_3_1_0_to_3_0_20 },
    { "3.1.1", NVRAM_FW_3_1_1, fw_func_3_1_0_to_3_1_1, fw_func_3_1_1_to_3_1_0 },
    { "3.1.2", NVRAM_FW_3_1_2, fw_func_3_1_1_to_3_1_2, fw_func_3_1_2_to_3_1_1 },
    { "3.1.3", NVRAM_FW_3_1_3, fw_func_3_1_2_to_3_1_3, fw_func_3_1_3_to_3_1_2 },
    { "3.1.4", NVRAM_FW_3_1_4, fw_func_3_1_3_to_3_1_4, fw_func_3_1_4_to_3_1_3 },
    { "3.1.5", NVRAM_FW_3_1_5, fw_func_3_1_4_to_3_1_5, fw_func_3_1_5_to_3_1_4 },
    { "3.1.6", NVRAM_FW_3_1_6, fw_func_3_1_5_to_3_1_6, fw_func_3_1_6_to_3_1_5 },
    { "3.1.7", NVRAM_FW_3_1_7, fw_func_3_1_6_to_3_1_7, fw_func_3_1_7_to_3_1_6 },
    { "3.1.8", NVRAM_FW_3_1_8, fw_func_3_1_7_to_3_1_8, fw_func_3_1_8_to_3_1_7 },
    { "3.1.9", NVRAM_FW_3_1_9, fw_func_3_1_8_to_3_1_9, fw_func_3_1_9_to_3_1_8 },
    { "3.1.10", NVRAM_FW_3_1_10, fw_func_3_1_9_to_3_1_10, fw_func_3_1_10_to_3_1_9 },
    { "3.2.0", NVRAM_FW_3_2_0, fw_func_3_1_9_to_3_2_0, fw_func_3_2_0_to_3_1_9 },
    { "3.2.1", NVRAM_FW_3_2_1, fw_func_3_2_0_to_3_2_1, fw_func_3_2_1_to_3_2_0 },
    { "3.2.2", NVRAM_FW_3_2_2, fw_func_3_2_1_to_3_2_2, fw_func_3_2_2_to_3_2_1 },
    { NULL, 0, NULL, NULL }
};

int
nvram_init(void *unused)
{
	if ((nvram_fd = open(PATH_DEV_NVRAM, O_RDWR)) < 0)
		goto err;

#if defined(NVRAM_MMAP) && NVRAM_MMAP > 0
	/* Map kernel string buffer into user space */
	if ((nvram_buf = mmap(NULL, NVRAM_SPACE, PROT_READ, MAP_SHARED, nvram_fd, 0)) == MAP_FAILED) {
		close(nvram_fd);
		nvram_fd = -1;
		goto err;
	}
#endif

	return 0;

 err:
	perror(PATH_DEV_NVRAM);
	return errno;
}

#ifdef EZP_PROD_SUPERCAT_X
/* Interface to get product category */
void get_prod_cat(char *prod_cat_buf, size_t bufsz)
{
    char *prod_cat_rule = "prod_cat"; 
	size_t count;
	char tmp[100];
	unsigned long *off = (unsigned long *) tmp;

    if (nvram_fd < 0) {
		if (nvram_init(NULL)){
            prod_cat_buf[0] = '\0';
			return;
        }
    }
    count = strlen(prod_cat_rule) + 1;
    strcpy((char *) off, prod_cat_rule);
    count = read(nvram_fd, off, count);
    if (count == sizeof(unsigned long)) {
        strncpy(prod_cat_buf, (char*)&nvram_buf[*off], bufsz);
    }
    else {
        prod_cat_buf[0] = '\0';
    }
}

void get_name_with_prodcat(const char *name, char *bufname, size_t bufsz) {
    char prod_cat_buf[4];
    get_prod_cat(prod_cat_buf, sizeof(prod_cat_buf));
    snprintf(bufname, bufsz, "%s_%s", name, prod_cat_buf);
}
#endif

#if defined(NVRAM_MMAP) && NVRAM_MMAP > 0
/* nvram clean get, doesn't related to product category */
char *
#ifdef EZP_PROD_SUPERCAT_X
_nvram_get(const char *name, int with_prod_cat)
#else
nvram_get(const char *name)
#endif /* EZP_PROD_SUPERCAT_X */
{
	size_t count = strlen(name) + 1;
	char tmp[100], *value;
	unsigned long *off = (unsigned long *) tmp;
#ifdef EZP_PROD_SUPERCAT_X
    char curr_prod_cat[4];
    char *prod_cat_rule = "prod_cat"; 
#endif /* EZP_PROD_SUPERCAT_X */
	if (nvram_fd < 0)
		if (nvram_init(NULL))
			return NULL;

	if (count > sizeof(tmp)) {
		if (!(off = malloc(count)))
			return NULL;
	}
#ifdef EZP_PROD_SUPERCAT_X
    /* Get product category */
    if (with_prod_cat) {
        get_prod_cat(curr_prod_cat, sizeof(curr_prod_cat));
    }
#endif /* EZP_PROD_SUPERCAT_X */
    count = strlen(name) + 1;
    off = (unsigned long *) tmp;
	/* Get offset into mmap() space */
	strcpy((char *) off, name);
	count = read(nvram_fd, off, count);

	if (count == sizeof(unsigned long))
		value = &nvram_buf[*off];
	else if (count < 0) {
		value = NULL;
		perror(PATH_DEV_NVRAM);
    }
    else {
		value = NULL;
#ifdef EZP_PROD_SUPERCAT_X
        /* EZP:This nvram entry is replace with name "_" prod_cat */
        if (with_prod_cat && curr_prod_cat[0]) {
            sprintf(tmp, "%s_%s", name, curr_prod_cat);
            count = strlen(tmp) + 1;
            off = (unsigned long *) tmp;
            count = read(nvram_fd, off, count);
            if (count == sizeof(unsigned long))
                value = &nvram_buf[*off];
            else if (count < 0) {
                perror(PATH_DEV_NVRAM);
            }
            else
                value = NULL;
        }
#endif /* EZP_PROD_SUPERCAT_X */
    }

	if (off != (unsigned long *) tmp)
		free(off);
	return value;
}



#ifdef EZP_PROD_SUPERCAT_X
char *
nvram_get(const char *name)
{
	return _nvram_get(name,1);
}
#endif /* EZP_PROD_SUPERCAT_X */

#else /* #if defined(NVRAM_MMAP) && NVRAM_MMAP > 0 */

/* put returned value in value[]; the caller is responsible for freeing it */
char *
nvram_get(const char *name)
{
	size_t count = 1024 ; /* max len =1024 */
	char *value;

	if (nvram_fd < 0)
		if (nvram_init(NULL))
			return NULL;

	if (!(value = malloc(count)))
		return NULL;

	/* Get offset into mmap() space */
	strcpy((char *) value, name);

	count = read(nvram_fd, value, count);

	if (count < 0) {
		perror(PATH_DEV_NVRAM);
		free(value) ;
		value = NULL;
	}
	if (count == 0) {
		value[0] = '\0' ;
	}

	return value;
}
#endif /* #if defined(NVRAM_MMAP) && NVRAM_MMAP > 0 */

int
nvram_getall(char *buf, int count)
{
	int ret;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	if (count == 0)
		return 0;

	/* Get all variables */
	*buf = '\0';

	ret = read(nvram_fd, buf, count);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return (ret == count) ? 0 : ret;
}

int
nvram_get_option(const char *name)
{
    struct nvram_tuple *v;
    for (v = &nvram_ezpacket_default[0]; v->name ; v++) {
        if (!strcmp(v->name, name)) {
            return v->option;
        }
    }
    /* No option is found. */
    return NVRAM_UNDEFINED;
}

static int
_nvram_set(const char *name, const char *value)
{
	size_t count = strlen(name) + 1;
	char tmp[100], *buf = tmp;
	int ret;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	/* Unset if value is NULL */
	if (value)
		count += strlen(value) + 1;

	if (count > sizeof(tmp)) {
		if (!(buf = malloc(count)))
			return -ENOMEM;
	}

	if (value)
		sprintf(buf, "%s=%s", name, value);
	else
		strcpy(buf, name);

	ret = write(nvram_fd, buf, count);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	if (buf != tmp)
		free(buf);

	return (ret == count) ? 0 : ret;
}

int
#ifdef EZP_PROD_SUPERCAT_X
nvram_set_prod(const char *name, const char *value, int with_prod_cat)
#else
nvram_set(const char *name, const char *value)
#endif /* EZP_PROD_SUPERCAT_X */
{
#ifdef EZP_PROD_SUPERCAT_X
    char curr_prod_cat[4];
    char *prod_cat_rule = "prod_cat";
#endif /* EZP_PROD_SUPERCAT_X */
    uint32 opt;
    char tmpname[100];

    strncpy(tmpname, name, sizeof(tmpname));
    opt = nvram_get_option(tmpname);
    /* If "name" entry exists in nvram_default, use it. */
    if (opt & NVRAM_UNDEFINED) {
#ifdef EZP_PROD_SUPERCAT_X
        /* Get product category */
        if (with_prod_cat) {
            get_name_with_prodcat(name, tmpname, sizeof(tmpname));
            opt = nvram_get_option(tmpname);
            /* If name_prodcat exists in nvram_default, use it. */
            if (opt & NVRAM_UNDEFINED) {
                return EINVAL;
            }
            /* In case we have "name" in nvram but we want to set
             * name_prodcat into nvram. In this case, we have to 
             * remove "name". */
            if (_nvram_get(name, 0)) {
                _nvram_set(name, NULL);
            }
        }
        else 
#endif /* EZP_PROD_SUPERCAT_X */
        return EINVAL;
    }

    if (opt & NVRAM_PROTECTED) {
        char *exist = nvram_get(tmpname);
        if (exist && *exist) {
            return EACCES; /* If anything exists, return permission denied. */
        }
    }
    return _nvram_set(tmpname, value);
}


#ifdef EZP_PROD_SUPERCAT_X
int
nvram_set(const char *name, const char *value) {
    return nvram_set_prod(name, value, 1);
}
#endif /* EZP_PROD_SUPERCAT_X */

int
nvram_fset(const char *name, const char *value)
{
#ifdef EZP_PROD_SUPERCAT_X
    uint32 opt;
    char tmpname[100];
    char curr_prod_cat[4];
    char *prod_cat_rule = "prod_cat"; 
    size_t count;

    get_name_with_prodcat(name, tmpname, sizeof(tmpname));
    opt = nvram_get_option(tmpname);
    /* If name_prodcat exists in default structure., use it */
    if (!(opt & NVRAM_UNDEFINED)) {
        /* In case we have "name" in nvram but we want to set
         * name_prodcat into nvram. In this case, we have to 
         * remove "name". */
        if (_nvram_get(name, 0)) {
            _nvram_set(name, NULL);
        }
        return _nvram_set(tmpname, value);
    }
    /* on the other hand, uses name directly. */
    else
#endif
    return _nvram_set(name, value);
}

int
nvram_unset(const char *name)
{
	return nvram_fset(name, NULL);
}

int
nvram_commit(void)
{
	int ret;
	
	if((check_action() == ACT_IDLE) || 
	   (check_action() == ACT_SW_RESTORE) || 
	   (check_action() == ACT_HW_RESTORE)){
		if (nvram_fd < 0)
			if ((ret = nvram_init(NULL)))
				return ret;

		ret = ioctl(nvram_fd, NVRAM_MAGIC, NULL);

		if (ret < 0)
			perror(PATH_DEV_NVRAM);
	}

	return ret;
}

void
nvram_boot(void)
{
    struct nvram_tuple *v;
    char *value;
    for (v = &nvram_ezpacket_default[0]; v->name ; v++) {
        value = nvram_get(v->name);
        if (!value || !*value) {
            /* NULL or "\0" */
            if (v->option & NVRAM_EMPTY)
                continue; /* NULL or "\0" is allowed. */

            if (v->option & NVRAM_DEFAULT) {
                char default_name[64];
                /* Get the default value. */
                sprintf(default_name, "%s_default", v->name);
                v->value = nvram_get(default_name);
            }

            nvram_fset(v->name, v->value);
        } else {
            /* Some value exist. */
            if (v->option & NVRAM_TEMP)
                /* Reset the value! */
                nvram_fset(v->name, v->value);
        }
    }
}
#define VER_TO_VERID(__MAJOR__, __SUB__, __MINOR__, __AB__, __VAB__) ((__MAJOR__<<(4+8+4+8)) + (__SUB__<<(8+4+8)) + (__MINOR__<<(4+8)) +((__AB__ == 'r') ? 0 :((__AB__-'a' + 1)<<8)) + (__VAB__))
static void get_ver(char *verstr, unsigned int *relver, unsigned int *alphabetaver, unsigned char *stage) {
    char *dashptr;
    char *dotptr;
    char *ahphaptr;
    char *betaptr;
    char *verstart;
    int relv = 0, alphabetav = 0, dot_level = 0, shift_val = 8;

    if(!verstr) return;
    dashptr = strchr(verstr, '-');
    dotptr = strchr(verstr, '.');
    ahphaptr = strchr(verstr, 'A');
    betaptr = strchr(verstr, 'B');
    verstart = verstr;
    if(!dotptr) return;
    /* For first 2 level, we uses 4 bit, 1-16 versions, that is 
     * 3.1.17 ==> 3 << (4 + 8) + 1 << (8) + 17 */
    while(dotptr) {
        if(dot_level < 2) {
            shift_val = 4;
        } else {
            shift_val = 8;
        }
        relv = (relv << shift_val) + atoi(verstart);
        verstart = dotptr + 1;
        dotptr = strchr(verstart, '.');
        dot_level ++;
    }
    if(!dotptr) {
        relv = (relv << 8) + atoi(verstart);
    }

    if(ahphaptr && dashptr) {
        *stage = 'b';
        relv = (relv << 4) + 1;
        alphabetav = atoi(ahphaptr + 1);
    } else if(betaptr && dashptr) {
        *stage = 'c';
        relv = (relv << 4) + 2;
        alphabetav = atoi(betaptr + 1);
    } else {
        *stage = 'a';
        relv = (relv << 4);
        alphabetav = 0;
    }
    relv = (relv << 8) + alphabetav;
    *alphabetaver = alphabetav;
    *relver = relv;
}

static int
non_standard_release_process(char *old_str, char *new_str) {
    unsigned int oldver = 0, oldabver = 0, newver = 0, newabver = 0;
    unsigned char oldab = 'r', newab = 'r';
    int tocommit = 0;
    get_ver(old_str, &oldver, &oldabver, &oldab);
    get_ver(new_str, &newver, &newabver, &newab);
    if(oldab != newab && oldab != 'a' && newab != 'a') return 0;
    if((oldver >> 12) != (newver >> 12)) return 0;
    /*
     1111 1111 1111 1111 1111 1111 1111 1111
           maj  sub       min   ab       vab
     * */
    //if(oldabver == newabver) return 1;
    /* For version 3.0.7-AXX to 3.0.7-AYY XX < 37 and YY >= 37 */
    /*  3.0.7-A37 ==> 0x0 3 0 07 1 25 ==> 0x03007125 */
    //if((newver == 0x00030007) && (newabver >= 37) && (oldabver < 37)) {
    if(newver == VER_TO_VERID(3,0,7,'a',37)) {
        ezplib_replace_attr("ntp_rule", 0, "zone", "UTC0");
        ezplib_replace_attr("ntp_rule", 0, "daylight", "0");
        tocommit = 1;
    }
#ifdef PLATFORM_M2E
    /* From smaller than 3.1.0-a02 to greater than 3.1.0-a02, 
     * we need to default */
    if((oldver <= VER_TO_VERID(3,1,0,'a',2)) && (newver >= VER_TO_VERID(3,1,0,'a',2))) {
        nvram_default();
        tocommit = 1;
    }
    /* From 3.1.0-A08 to 3.1.0-A09, we need to default PPTP. */
    if((oldver < VER_TO_VERID(3,1,0,'a',9)) && (newver >= VER_TO_VERID(3,1,0,'a',9))) {
        nvram_default_rule("wan_pptp_l2tp_rule");
        tocommit = 1;
    }
    /* From 3.1.0-A09 to 3.1.0-A08, we need to
     * rewrite pptp PPTP. */
    if((oldver >= VER_TO_VERID(3,1,0,'a',9)) && (newver < VER_TO_VERID(3,1,0,'a',9))) {
        nvram_set("wan_pptp_l2tp_rule", "");
        tocommit = 1;
    }
    if((oldver < VER_TO_VERID(3,1,0,'a',14)) && (newver >= VER_TO_VERID(3,1,0,'a',14))) {
	system("mtd write /rom/rom/uboot.bin Bootload");	
        nvram_default();
        tocommit = 1;
    }
#endif
#ifdef PLATFORM_AXA
    /* Do nvram default if we upgrade from rev a49 to a50 */
    if((oldver < VER_TO_VERID(3,0,7,'a',50)) && (newver >= VER_TO_VERID(3,0,7,'a',50))) {
        nvram_default();
        tocommit = 1;
    }
#endif
    return tocommit;
}

static int
special_processing(char *new_str)
{
    char tmp[32];
    char *val;
    int change = 0;

    ezplib_get_attr_val("ssh_rule", 0, "enable", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
    if (tmp[0] != '0') {
        ezplib_replace_attr("ssh_rule", 0, "enable", "0");
    }

    val = nvram_get("turbonat_enable");
    if (!val || *val == '0') {
        nvram_fset("turbonat_enable", "1");
    }
    /* XXX need to remove later for X-116NX
    if ((!strcmp("X", nvram_safe_get("prod_supercat")))) {
        nvram_unset("x_mode_data");
        nvram_unset("prod_cat");
        nvram_unset("wan_main_rule_I");
        nvram_unset("wan_main_rule_G");
        nvram_unset("appmgr_rule"); nvram_unset("appmgr_rule_I"); nvram_unset("appmgr_rule_G");
        nvram_unset("wan_num"); nvram_unset("wan_num_G"); nvram_unset("wan_num_I");
        nvram_unset("wan0_device"); nvram_unset("wan0_device_I"); nvram_unset("wan0_device_G");
        nvram_unset("wan1_device"); nvram_unset("wan1_device_I"); nvram_unset("wan1_device_G");
        nvram_unset("wan0_ifname"); nvram_unset("wan0_ifname_I"); nvram_unset("wan0_ifname_G");
        nvram_unset("wan1_ifname"); nvram_unset("wan1_ifname_I"); nvram_unset("wan1_ifname_G");
        nvram_unset("wan0_proto"); nvram_unset("wan0_proto_I"); nvram_unset("wan0_proto_G");
        nvram_unset("wan1_proto"); nvram_unset("wan1_proto_I"); nvram_unset("wan1_proto_G");
    } */
    nvram_commit();
    return 1;
}

static int
fw_version_update(char *new_str)
{
    /* nvram is the only place to set fw_version. */
    nvram_fset("fw_version", new_str);
    nvram_fset("custom_fw_version", xstr(CUSTOM_PROD_VERSION));
    nvram_commit();
    return 1;
}

static int
mosra_special_firmware_setting()
{
    char *tmp;

    tmp = nvram_get("ntp_rule");
    if (!tmp) {
        nvram_fset("ntp_rule", "^1^pool^north-america.pool.ntp.org^^UTC0^1");
        nvram_commit();
    } else if (strcmp(tmp, "^1^pool^north-america.pool.ntp.org^^UTC0^1")) {
        /* compel ntp TIME ZONE to be fixed as UTC0 for Mosra */
        nvram_fset("ntp_rule", "^1^pool^north-america.pool.ntp.org^^UTC0^1");
        nvram_commit();
    }
    return 1;
}

int
nvram_upgrade(char *source)
{
    struct nvram_fw_tuple *v;
    int old, new;
    char old_str[32];
    char new_str[32];
    char dst_ver_str[32];
    int i, change = 0;
    /* If source is empty, get it from fw_version. */
    if (source && *source) {
        strcpy(old_str, source);
    } else {
        strcpy(old_str, nvram_safe_get("fw_version"));
    }
    strcpy(new_str, xstr(EZP_PROD_VERSION));
    strcpy(dst_ver_str, xstr(EZP_PROD_VERSION));
    if (strcmp(old_str, new_str)) {
        /* Change from other version fo 3.0.7-A37 */
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
        if(non_standard_release_process(old_str, new_str))
            change = 1;
#endif
        /* If the versions have any difference, eg., 2.0.2-RC1 to 2.0.2-RC3, we
         * should enforce the special processing, fg., closing the backdoor.
         */
        if(special_processing(new_str))
            change = 1;
        fw_version_update(dst_ver_str);
    }

    /* Purify new_str. e.g. 1.6.5-RC1 => 1.6.5 */
    for (i = 0; old_str[i] == '.' || isdigit(old_str[i]) ; i++);
    old_str[i]='\0';
    for (i = 0; new_str[i] == '.' || isdigit(new_str[i]) ; i++);
    new_str[i]='\0';

    /* Use "0.0.0" instead of "". */
    if (!*old_str) {
        strcpy(old_str, nvram_fw_table[0].fw_str);
    }

    /* We might not be able to find out the version. Skip upgrade if cannot. */
    old = 0xEFFFFFFF;
    for (v = &nvram_fw_table[0]; v->fw_str ; v++) {
        if (!strcmp(v->fw_str, old_str)) {
            old = v->fw_version;
        }
        if (!strcmp(v->fw_str, new_str)) {
            new = v->fw_version;
        }
    }

#if defined(EZP_SUB_BRAND_SONY)
    mosra_special_firmware_setting();
#endif

    if (old < new) {
        printf("Upgrade [%s->%s]\n", old_str, new_str);
        /* Upgrade. Use the upgrade functions from (old + 1) to new. */
        for (i = old + 1; i <= new; i++) {
            if (nvram_fw_table[i].fw_upgrade_func) {
                (*nvram_fw_table[i].fw_upgrade_func)();
            }
        }
        change = 1;
    }
    
    if(strcmp(nvram_safe_get("prod_subsubcat"), "")) {
        /* unset prod_subsubcat to make sure prod_subsubcat the same with 
         * special firmware */
        nvram_unset("prod_subsubcat"); 
        change = 1;
    }
    return change;
}

int
nvram_downgrade(char *target)
{
    struct nvram_fw_tuple *v;
    int old, new = NVRAM_FW_INVALID;
    char old_str[32];
    char new_str[32];
    char dst_ver_str[32];
    int i, change = 0;

    strcpy(old_str, nvram_safe_get("fw_version"));
    strcpy(new_str, target);
    strcpy(dst_ver_str, target);

    if (strcmp(old_str, new_str)) {
        /* If the versions have any difference, eg., 2.0.2-RC1 to 2.0.2-RC3, we
         * should enforce the special processing, fg., closing the backdoor.
         */
#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
        non_standard_release_process(old_str, new_str);
#endif
        change = special_processing(new_str);
    }

    /* Purify new_str. e.g. 1.6.5-RC1 => 1.6.5 */
    for (i = 0; old_str[i] == '.' || isdigit(old_str[i]) ; i++);
    old_str[i]='\0';
    for (i = 0; new_str[i] == '.' || isdigit(new_str[i]) ; i++);
    new_str[i]='\0';

    /* Use "0.0.0" instead of "". */
    if (!*old_str) {
        strcpy(old_str, nvram_fw_table[0].fw_str);
    }
    if (!*new_str) {
        strcpy(new_str, nvram_fw_table[0].fw_str);
    }

    /* We might not find out the version. Skip downgrade if cannot. */
    old = 0;
    for (v = &nvram_fw_table[0]; v->fw_str ; v++) {
        if (!strcmp(v->fw_str, old_str)) {
            old = v->fw_version;
        }
        if (!strcmp(v->fw_str, new_str)) {
            new = v->fw_version;
        }
    }
    if (old > new) {
        printf("Downgrade [%s->%s]\n", old_str, new_str);
        /* Downgrade. Use the upgrade functions from old to (new + 1). */
        for (i = old; i >= new + 1; i-- ) {
            if (nvram_fw_table[i].fw_downgrade_func) {
                (*nvram_fw_table[i].fw_downgrade_func)();
            }
        }
        fw_version_update(dst_ver_str);
        change = 1;
    }
    if(strcmp(nvram_safe_get("prod_subsubcat"), "")) {
        /* unset prod_subsubcat to make sure prod_subsubcat the same with 
         * special firmware */
        nvram_unset("prod_subsubcat"); 
        change = 1;
    }
    return change;
}

void
nvram_default(void)
{
    struct nvram_tuple *v;
    for (v = &nvram_ezpacket_default[0]; v->name ; v++) {
        nvram_set(v->name, v->value);
    }
}

void
nvram_default_rule(char *rulename)
{
    struct nvram_tuple *v;
    for (v = &nvram_ezpacket_default[0]; v->name ; v++) {
        if(!strcmp(v->name, rulename)) {
            nvram_set(v->name, v->value);
            break;
        }
    }
}

void
nvram_export(char *filename)
{
    FILE *fp;
    struct nvram_tuple *v;
    char *value;

    if ( !(fp = fopen(filename, "wb") ))
        return;

    fprintf(fp, "[EZP_LOG v1.1] %s %s [EZP_%s%s] " xstr(EZP_PROD_VERSION) "\n", 
            nvram_safe_get("brand"), nvram_safe_get("model"), 
            nvram_safe_get("prod_cat"), nvram_safe_get("prod_subcat")); 

    for (v = &nvram_ezpacket_default[0]; v->name ; v++) {
        /* Skip NVRAM_PROTECTED or NVRAM_TEMP. */
        if ((v->option & NVRAM_PROTECTED) || 
                (v->option & NVRAM_TEMP)) {
            continue;
        } 
        value = nvram_safe_get(v->name);
        fprintf(fp, "%s=%s\n", v->name, value);
    }
    fclose(fp);
}

int
nvram_import(char *filename)
{
    FILE *fp;
    char *p, *q;
    char buf[NVRAM_TMP_LEN];
    char old_str[32], new_str[32];
    int old, new;
    int i;

    struct nvram_tuple *v;
    struct nvram_fw_tuple *w;

    if ( !(fp = fopen(filename, "r") ))
        return 1;

    /* First line should begin with "EZP_LOG". */
    fgets(buf, sizeof(buf), fp);
    if ((p = strstr(buf, "EZP_LOG")) == NULL) {
        printf("log file format error\n");
        return 1;
    }
    if ((p = strstr(p + strlen("EZP_LOG"), "EZP_")) == NULL) {
        printf("log file format error: product\n");
        return 1;
    }
    p += strlen("EZP_");
    /* prod_cat */
    q = nvram_safe_get("prod_cat");
    if (p[0] != q[0]) {
        printf("log file format error: category\n");
        return 1;
    }
    /* prod_subcat */
    q = nvram_safe_get("prod_subcat");
    if (p[1] != q[0]) {
        printf("log file format error: subcategory\n");
        return 1;
    }

    p = strchr(p, ']');
    p += 1;

    if (*p == '\n' || *p == '\0') {
        strcpy(old_str, "0.0.0");
    } else {
        strncpy(old_str, p + 1, strlen(p + 1));
    }

    /* XXX:We don't accept any thing higher than our current version. */
    strcpy(new_str, xstr(EZP_PROD_VERSION));
    /* Purify new_str. e.g. 1.6.5-RC1 => 1.6.5 */
    for (i = 0; old_str[i] == '.' || isdigit(old_str[i]) ; i++);
    old_str[i]='\0';
    for (i = 0; new_str[i] == '.' || isdigit(new_str[i]) ; i++);
    new_str[i]='\0';

    /* Very likely we cannot find the matched version since our firmware might
     * be older than the config file. */
    old = 0x0FFFFFFF;

    for (w = &nvram_fw_table[0]; w->fw_str ; w++) {
        if (!strcmp(w->fw_str, old_str)) {
            old = w->fw_version;
        }
        if (!strcmp(w->fw_str, new_str)) {
            new = w->fw_version;
        }
    }

    if (old > new) {
        printf("log file format error: newer version configuration format\n");
        return 1;
    }

    while (fgets(buf, sizeof(buf), fp)) {
        if ((p = strchr(buf, '=')) == NULL)
            continue;

        /* Please the end of the string to replace "=". */
        *p = 0; 

        for (v = &nvram_ezpacket_default[0]; v->name ; v++) {
            if (!strcmp(v->name, buf)) {
                break;
            }
        }

        if (!v->name || (v->option & NVRAM_PROTECTED) || 
                (v->option & NVRAM_TEMP)) {
            /* No match or NVRAM_PROTECTED or NVRAM_TEMP. */
            printf("invalid: %s=%s\n", buf, p + 1);
            continue;
        }

        if (*(p + 1) == '\n') {
            /* "key=\n" */
            nvram_set(buf, "");
        } else {
            /* "key=value\n" */
            p++;
            /* Replace \n with \0 */
            if (*(p + strlen(p) - 1) == '\n') {
                *(p + strlen(p) - 1) = '\0';
            }
            nvram_set(buf, p);
        }
    }
    fclose(fp);

    nvram_upgrade(old_str);

    return 0;
}

int
check_action(void)
{
	char buf[80] = "";
	
	if(file_to_buf(ACTION_FILE, buf, sizeof(buf))){
		if(!strcmp(buf, "ACT_TFTP_UPGRADE")){
			cprintf("Upgrading from tftp now, quiet exit....\n");
			return ACT_TFTP_UPGRADE;
		}
		else if(!strcmp(buf, "ACT_WEBS_UPGRADE")){
			cprintf("Upgrading from web (https) now, quiet exit....\n");
			return ACT_WEBS_UPGRADE;
		}
		else if(!strcmp(buf, "ACT_WEB_UPGRADE")){
			cprintf("Upgrading from web (http) now, quiet exit....\n");
			return ACT_WEB_UPGRADE;
		}
		else if(!strcmp(buf, "ACT_SW_RESTORE")){
			cprintf("Receive restore command from web, quiet exit....\n");
			return ACT_SW_RESTORE;
		}
		else if(!strcmp(buf, "ACT_HW_RESTORE")){
			cprintf("Receive restore commond from resetbutton, quiet exit....\n");
			return ACT_HW_RESTORE;
		}
	}
	//fprintf(stderr, "Waiting for upgrading....\n");
	return ACT_IDLE;
}

int
file_to_buf(char *path, char *buf, int len)
{
	FILE *fp;

	memset(buf, 0 , len);

	if ((fp = fopen(path, "r"))) {
		fgets(buf, len, fp);
		fclose(fp);
		return 1;
	}

	return 0;
}

int fw_func_0_0_0_to_1_6_5(void)
{
    int i, j, len;
    char new[1024];
    char tmp[64];
    char *str = &new[0];
    char *rule_set = "wan_bw_rule";
    char *value = nvram_safe_get(rule_set);

    printf("fw_func_0_0_0_to_1_6_5\n");
    /* wanX_bw_mode will be added at "nvram boot" if it doesn't exist. */

    /* If empty, nvram boot will create it. */ 
    if (value && *value) {
        /* 
         * wan_bw_rule is changed in the format. 
         *
         * Old format: type^dl^ul^link_percent^global_percent^user_percent^
         * dbm_upmax^dbm_upmin^dbm_downmax^dbm_downmin 
         * New format: type^dl^ul^expert_dl^expert_ul^link_percent^
         * global_percent^user_percent^dbm_upmax^dbm_upmin^dbm_downmax^
         * dbm_downmin 
         */
        for (i = 0, j = 0, len = 0; i < WAN_NUM; i++) {
            /* Removed expert_dl/expert_ul. */
            ezplib_get_attr_val(rule_set, i, "type", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dl", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "ul", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* expert_dl */
            len = snprintf(str, 1024 - j, "0^", tmp);
            j += len;
            str += len;
            /* expert_ul */
            len = snprintf(str, 1024 - j, "0^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "expert_dl", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "expert_ul", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "link_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "global_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "user_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_upmax", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* No ending "^". */ 
            ezplib_get_attr_val(rule_set, i, "dbm_upmin", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s", tmp);
            j += len;
            str += len;

            ezplib_replace_rule(rule_set, i, new);
        }
    }
    return 0;
}

int fw_func_1_6_5_to_0_0_0(void)
{
    int i, j, len;
    char new[1024];
    char tmp[128];
    char *str = &new[0];
    char *rule_set = "wan_bw_rule";
    char *value = nvram_safe_get(rule_set);

    printf("fw_func_1_6_5_to_0_0_0\n");
    /* wanX_bw_mode */
    for (i = 0; i < WAN_NUM; i++) {
        snprintf(tmp, 64, "wan%d_bw_mode", i);
        nvram_unset(tmp);    
    }
    /* Check the existence of the value. */
    if (value && *value) {
        for (i = 0, j = 0, len = 0; i < WAN_NUM; i++) {
            /* Removed expert_dl/expert_ul. */
            ezplib_get_attr_val(rule_set, i, "type", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dl", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "ul", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "link_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "global_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "user_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_upmax", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_upmin", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_downmax", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* No ending "^". */ 
            ezplib_get_attr_val(rule_set, i, "dbm_downmin", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s", tmp);
            j += len;
            str += len;

            ezplib_replace_rule(rule_set, i, new);
        }
    }
    return 0;
}

int fw_func_1_6_5_to_1_6_6(void)
{
    int i, j, len;
    char new[1024];
    char tmp[64];
    char *str = &new[0];
    char *rule_set = "upnp_rule";
    char *value = nvram_safe_get(rule_set);

    printf("fw_func_1_6_5_to_1_6_6\n");
    /* If empty, nvram boot will create it. */ 
    /* Check the existence of the value. */
    if (value && *value) {
        /* 
         * upnp_rule is changed in the format. 
         *
         * Old: name^enable[0|1]^extif^inif^port
         * New: name^enable[0|1]^pmp_enable[0|1]^extif^inif^port
         */

        i = j = 0;
        /* Added pmp_enable. */
        ezplib_get_attr_val(rule_set, i, "name", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* pmp_enable */
        len = snprintf(str, 1024 - j, "0^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "pmp_enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "extif", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* No ending "^". */ 
        ezplib_get_attr_val(rule_set, i, "inif", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s", tmp);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, i, new);
    }
    return 0;
}

int fw_func_1_6_6_to_1_6_5(void)
{
    int i, j, len;
    char new[1024];
    char tmp[128];
    char *str = &new[0];
    char *rule_set = "upnp_rule";
    char *value = nvram_safe_get(rule_set);

    printf("fw_func_1_6_6_to_1_6_5\n");

    /* Check the existence of the value. */
    if (value && *value) {
        i = 0;
        /* Removed pmp_enable. */
        ezplib_get_attr_val(rule_set, i, "name", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "extif", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "inif", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* No ending "^". */ 
        ezplib_get_attr_val(rule_set, i, "port", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s", tmp);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, i, new);
    }
    return 0;
}
int fw_func_1_6_6_to_1_6_7(void)
{
    int i, j, len;
    char new[1024];
    char tmp[64];
    char *str = &new[0];
    char *rule_set = "wan_bw_rule";
    char *value = nvram_safe_get(rule_set);

    printf("fw_func_1_6_6_to_1_6_7\n");
    /* If empty, nvram boot will create it. */ 
    /* Check the existence of the value. */
    if (value && *value) {
        /* 
         * wan_bw_rule is changed in the value in the attribute "type". 
         *
         * e.g.,
         * Old: 2M256K
         * New: A2M256K
         */

        for (i = 0, j = 0, len = 0; i < WAN_NUM; i++) {
            /* Modified the value of "type". */
            ezplib_get_attr_val(rule_set, i, "type", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            /* Set the type default as ADSL */
            len = snprintf(str, 1024 - j, "A%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dl", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "ul", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "expert_dl", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "expert_ul", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "link_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "global_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "user_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_upmax", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_upmin", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_downmax", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_downmin", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s", tmp);
            j += len;
            str += len;

            ezplib_replace_rule(rule_set, i, new);
        }
    }
    return 0;
}

int fw_func_1_6_7_to_1_6_6(void)
{
    int i, j, k, len;
    char new[1024];
    char tmp[128];
    char *str = &new[0];
    char *rule_set = "wan_bw_rule";
    char *value = nvram_safe_get(rule_set);
    char *type;
    char tmp_dl[128], tmp_ul[128];
    char *bw_type[] = {
        "1M64K", "2M256K", "2M512K", "3M640K", "4M1M", "8M640K", "10M2M", 
        "12M1M", NULL
    };

    printf("fw_func_1_6_7_to_1_6_6\n");

    /* Check the existence of the value. */
    if (value && *value) {
        for (i = 0, j = 0, len = 0; i < WAN_NUM; i++) {
            ezplib_get_attr_val(rule_set, i, "type", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            ezplib_get_attr_val(rule_set, i, "dl", tmp_dl, sizeof(tmp),
                    EZPLIB_USE_CLI);
            ezplib_get_attr_val(rule_set, i, "ul", tmp_ul, sizeof(tmp),
                    EZPLIB_USE_CLI);
            /* Ignore the bandwidth type: ADSL, Cable or VDSL */
            if (!strcmp(tmp, "Custom")) {
                type = "Custom";
            } else {
                type = &tmp[1];
                for (k = 0; bw_type[k]; k++) {
                    if (!strcmp(type, bw_type[k])) {
                        break;
                    }
                }
                if (!bw_type[k]) {
                    /* No matched type */
                    type = "2M256K";
                    strcpy(tmp_dl, "2048");
                    strcpy(tmp_ul, "256");
                }
            }
            len = snprintf(str, 1024 - j, "%s^%s^%s^", type, tmp_dl, tmp_ul);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "expert_dl", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "expert_ul", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "link_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "global_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "user_percent", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_upmax", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_upmin", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_downmax", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "dbm_downmin", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s", tmp);
            j += len;
            str += len;

            ezplib_replace_rule(rule_set, i, new);
        }
    }
    return 0;
}
int fw_func_1_6_7_to_1_7_0(void)
{
    printf("fw_func_1_6_7_to_1_7_0\n");
    return 0;
}
int fw_func_1_7_0_to_1_6_7(void)
{
    printf("fw_func_1_7_0_to_1_6_7\n");
    return 0;
}
int fw_func_1_7_0_to_1_7_1(void)
{
    printf("fw_func_1_7_0_to_1_7_1\n");
    return 0;
}
int fw_func_1_7_1_to_1_7_0(void)
{
    printf("fw_func_1_7_1_to_1_7_0\n");
    return 0;
}
int fw_func_1_7_1_to_1_7_2(void)
{
    printf("fw_func_1_7_1_to_1_7_2\n");
    return 0;
}
int fw_func_1_7_2_to_1_7_1(void)
{
    printf("fw_func_1_7_2_to_1_7_1\n");
    return 0;
}
int fw_func_1_7_2_to_1_7_3(void)
{
    printf("fw_func_1_7_2_to_1_7_3\n");
    return 0;
}
int fw_func_1_7_3_to_1_7_2(void)
{
    printf("fw_func_1_7_3_to_1_7_2\n");
    return 0;
}
int fw_func_1_7_3_to_1_7_4(void)
{
    printf("fw_func_1_7_3_to_1_7_4\n");
    return 0;
}
int fw_func_1_7_4_to_1_7_3(void)
{
    printf("fw_func_1_7_4_to_1_7_3\n");
    return 0;
}
int fw_func_1_7_4_to_1_7_5(void)
{
    char *rule_set = "upnp_rule";
    char tmp[32];

    printf("fw_func_1_7_4_to_1_7_5\n");

    /* Disable NAT-PMP by default since now! */
    ezplib_get_attr_val(rule_set, 0, "pmp_enable", tmp, sizeof(tmp),
		    EZPLIB_USE_CLI);
    if (!strcmp(tmp, "1")) {
        ezplib_replace_attr(rule_set, 0, "pmp_enable", "0");
    } 

    /* Change UPnP listen port from 5000 to 5555.  */
    ezplib_get_attr_val(rule_set, 0, "port", tmp, sizeof(tmp), EZPLIB_USE_CLI);
    if (!strcmp(tmp, "5000")) {
        ezplib_replace_attr(rule_set, 0, "port", "5555");
    } 

    return 0;
}
int fw_func_1_7_5_to_1_7_4(void)
{
    printf("fw_func_1_7_5_to_1_7_4\n");
    /* Do nothing since we perfer NAT-PMP disabled and port 5555. */
    return 0;
}
int fw_func_1_7_5_to_1_7_6(void)
{
    int i, j, len, rule_num, max;
    char new[1024], old[1024];
    char tmp[64], tmp2[64], tmp3[64];
    char *str = &new[0];
    char *rule_set = "ntp_rule";
    char *value = nvram_safe_get(rule_set), *value2;
    char *wl_enable, *ptr;

    printf("fw_func_1_7_5_to_1_7_6\n");

    /* If empty, nvram boot will create it. */ 
    /* Check the existence of the value. */
    if (value && *value) {
        /* 
         * upnp_rule is changed in the format. 
         *
         * Old: name^enable[0|1]^pool^zone
         * New: name^enable[0|1]^type[pool|ipaddr]^pool^ipaddr^zone
         */

        i = j = 0;
        ezplib_get_attr_val(rule_set, i, "name", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* type */
        len = snprintf(str, 1024 - j, "pool^", tmp);
        j += len;
        str += len;
        /* pool: use new "type" to get old "pool". */
        ezplib_get_attr_val(rule_set, i, "type", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* ipaddr */
        len = snprintf(str, 1024 - j, "^", tmp);
        j += len;
        str += len;

        /* No ending "^". */ 
        /* zone: use new "pool" to get old "zone". */
        ezplib_get_attr_val(rule_set, i, "pool", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s", tmp);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, i, new);
    }

    rule_num = atoi(nvram_safe_get("ipsec_rule_num"));
    rule_set = "ipsec_rule";
    
    for (i = j = 0; i < rule_num; i++) {
        str = &new[0];
        ezplib_get_rule(rule_set, i, old, 1024);
        /* If empty, nvram boot will create it. */ 
        /* Check the existence of the value. */
        if (old[0]) {
            /* 
             * Old: name^enable^adv^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init^phase1_mode^phase1_auth_md5^phase1_auth_sha1^phase1_encrypt_des^phase1_encrypt_3des^phase1_encrypt_aes^phase1_id^phase2_auth_md5^phase2_auth_sha1^phase2_encrypt_des^phase2_encrypt_3des^phase2_encrypt_aes^phase1_lifetime^phase2_lifetime^pfs^ike_keymode^psk^rsa^ca
             * New: name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^phase1_mode^phase1_id^phase1_lifetime^phase2_lifetime^phase1_auth^phase1_encrypt^phase1_group^phase2_auth^phase2_encrypt^phase2_group
             */

            /* name^enable */
            ezplib_get_subrule(rule_set, i, 0, 1, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init */
            ezplib_get_subrule(rule_set, i, 3, 11, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* ike_keymode^psk^rsa^ca */
            ezplib_get_subrule(rule_set, i, 27, 30, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* adv */
            ezplib_get_subrule(rule_set, i, 2, 2, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* phase1_mode */
            ezplib_get_subrule(rule_set, i, 12, 12, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* phase1_id */
            ezplib_get_subrule(rule_set, i, 18, 18, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* phase1_lifetime^phase2_lifetime */
            ezplib_get_subrule(rule_set, i, 24, 25, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* phase1_auth */
            ezplib_get_subrule(rule_set, i, 13, 13, tmp, sizeof(tmp));
            ezplib_get_subrule(rule_set, i, 14, 14, tmp2, sizeof(tmp));
            if (tmp[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "md5");
            } else if (tmp2[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "sha1");
            } else {
                /* default MD5 */
                len = snprintf(str, 1024 - j, "%s^", "md5");
            }
            j += len;
            str += len;

            /* phase1_encrypt */
            ezplib_get_subrule(rule_set, i, 15, 15, tmp, sizeof(tmp));
            ezplib_get_subrule(rule_set, i, 16, 16, tmp2, sizeof(tmp));
            ezplib_get_subrule(rule_set, i, 17, 17, tmp3, sizeof(tmp));
            if (tmp2[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "3des");
            } else if (tmp3[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "aes");
            } else if (tmp[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "des");
            } else {
                /* default 3DES */
                len = snprintf(str, 1024 - j, "%s^", "3des");
            }
            j += len;
            str += len;

            /* phase1_group */
            len = snprintf(str, 1024 - j, "dh2^");
            j += len;
            str += len;

            /* phase2_auth */
            ezplib_get_subrule(rule_set, i, 19, 19, tmp, sizeof(tmp));
            ezplib_get_subrule(rule_set, i, 20, 20, tmp2, sizeof(tmp));
            if (tmp[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "md5");
            } else if (tmp2[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "sha1");
            } else {
                /* default MD5 */
                len = snprintf(str, 1024 - j, "%s^", "md5");
            }
            j += len;
            str += len;

            /* phase2_encrypt */
            ezplib_get_subrule(rule_set, i, 21, 21, tmp, sizeof(tmp));
            ezplib_get_subrule(rule_set, i, 22, 22, tmp2, sizeof(tmp));
            ezplib_get_subrule(rule_set, i, 23, 23, tmp3, sizeof(tmp));
            if (tmp2[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "3des");
            } else if (tmp3[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "aes");
            } else if (tmp[0] == '1') {
                len = snprintf(str, 1024 - j, "%s^", "des");
            } else {
                /* default 3DES */
                len = snprintf(str, 1024 - j, "%s^", "3des");
            }
            j += len;
            str += len;

            /* phase2_group */
            len = snprintf(str, 1024 - j, "dh2");
            j += len;
            str += len;

            ezplib_replace_rule(rule_set, i, new);
        }
    }

#if (WL_NUM > 0 )
    /* wireless */
    value = nvram_safe_get("wl0_radio");
    value2 = nvram_safe_get("wl_basic_rule");

    /* wl0_radio exists, nvram is still of old version. */
    if (*value && !*value2) {
        /* wl_basic_rule: enable^net_mode^txpower^channel */
        i = j = 0;
        rule_set = "wl_basic_rule";
        /* wl_basic_rule requires default value. */
        nvram_fset(rule_set, WL_BASIC_RULE);
        str = &new[0];

        /* enable */
        ptr = nvram_safe_get("wl0_radio");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* net_mode */
        ezplib_get_attr_val(rule_set, i, "net_mode", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* net_mode */
        ezplib_get_attr_val(rule_set, i, "txpower", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* channel */
        ezplib_get_attr_val(rule_set, i, "channel", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s", tmp);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, 0, new);

        /* wl0_basic_rule: enable^hidden^rate^wme */
        i = j = 0;
        rule_set = "wl0_basic_rule";
        /* wl0_basic_rule requires default value. */
        nvram_fset(rule_set, WL0_BASIC_RULE);
        str = &new[0];

        /* enable */
        ptr = nvram_safe_get("wl0_radio");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* hidden */
        ezplib_get_attr_val(rule_set, i, "hidden", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* rate */
        ezplib_get_attr_val(rule_set, i, "rate", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* wme */
        ezplib_get_attr_val(rule_set, i, "wme", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s", tmp);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, 0, new);

        /* wl0_ssid: ssid */
        rule_set = "wl0_ssid_rule";
        /* wl0_ssid_rule requires default value. */
        nvram_fset(rule_set, WL0_SSID_RULE);

        ptr = nvram_safe_get("wl0_ssid");
        ezplib_replace_rule(rule_set, 0, ptr);

        /* wl0_sec_rule: secmode */
        rule_set = "wl0_sec_rule";
        /* wl0_sec_rule requires default value. */
        nvram_fset(rule_set, WL0_SEC_RULE);

        ptr = nvram_safe_get("wl0_sec_mode");
        ezplib_replace_rule(rule_set, 0, ptr);

        /* wl0_sec_wep_rule: key_index^key1^key2^key3^key4 */
        i = j = 0;
        rule_set = "wl0_sec_wep_rule";
        /* wl0_sec_wep_rule requires default value. */
        nvram_fset(rule_set, WL0_SEC_WEP_RULE);
        str = &new[0];

        /* key_index */
        ptr = nvram_safe_get("wl0_key");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        ptr = nvram_safe_get("wl0_key1");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        ptr = nvram_safe_get("wl0_key2");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        ptr = nvram_safe_get("wl0_key3");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        ptr = nvram_safe_get("wl0_key4");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, 0, new);

        /* wl0_sec_wpa_rule: key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^rekey_time_interval^rekey_pkt_interval */
        i = j = 0;
        rule_set = "wl0_sec_wpa_rule";
        /* wl0_sec_wap_rule requires default value. */
        nvram_fset(rule_set, WL0_SEC_WPA_RULE);
        str = &new[0];

        /* key */ 
        ptr = nvram_safe_get("wl0_wpa_psk");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* crypto */
        ptr = nvram_safe_get("wl0_crypto");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* radius_key */
        ptr = nvram_safe_get("wl0_radius_key");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* radius_ipaddr */
        ptr = nvram_safe_get("wl0_radius_ipaddr");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* radius_port */
        ptr = nvram_safe_get("wl0_radius_port");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* rekey_mode */
        ezplib_get_attr_val(rule_set, i, "rekey_mode", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* rekey_time_interval */
        ezplib_get_attr_val(rule_set, i, "rekey_time_interval", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* rekey_pkt_interval */
        ezplib_get_attr_val(rule_set, i, "rekey_pkt_interval", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s", tmp);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, 0, new);

        /* wl0_sec_wpa2_rule: key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^rekey_time_interval^rekey_pkt_interval^preauth */
        i = j = 0;
        rule_set = "wl0_sec_wpa2_rule";
        /* wl0_sec_wap2_rule requires default value. */
        nvram_fset(rule_set, WL0_SEC_WPA2_RULE);
        str = &new[0];

        /* key */ 
        ptr = nvram_safe_get("wl0_wpa_psk");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* crypto */
        ptr = nvram_safe_get("wl0_crypto");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* radius_key */
        ptr = nvram_safe_get("wl0_radius_key");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* radius_ipaddr */
        ptr = nvram_safe_get("wl0_radius_ipaddr");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* radius_port */
        ptr = nvram_safe_get("wl0_radius_port");
        len = snprintf(str, 1024 - j, "%s^", ptr);
        j += len;
        str += len;

        /* rekey_mode */
        ezplib_get_attr_val(rule_set, i, "rekey_mode", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* rekey_time_interval */
        ezplib_get_attr_val(rule_set, i, "rekey_time_interval", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* rekey_pkt_interval */
        ezplib_get_attr_val(rule_set, i, "rekey_pkt_interval", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        /* preauth */
        ezplib_get_attr_val(rule_set, i, "preauth", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, 0, new);

        /* wl_wds_rule: mode^time */
        i = j = 0;
        rule_set = "wl_wds_rule";
        /* wl0_wds_rule requires default value. */
        nvram_fset(rule_set, WL_WDS_RULE);
        str = &new[0];

        ptr = nvram_safe_get("wl0_wds_enable");
        len = snprintf(str, 1024 - j, "%s^1",
                (ptr[0] == '0') ? "disabled" : "repeater");
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, 0, new);

        /* wl0_wds_basic_rule: mac^secmode */
        i = j = 0;
        rule_set = "wl0_wds_basic_rule";
        /* wl0_wds_basic_rule requires a default value. */
        nvram_fset(rule_set, WL0_WDS_BASIC_RULE);

        /* wl0_wds_rule_num requires a default value. */
        nvram_set("wl0_wds_rule_num", xstr(WDS_RULE_MAX));
        max = atoi(nvram_safe_get("wl0_wds_rule_num"));

        for (i = 0; i < max; i++) {
            ezplib_get_subrule("wl0_wds", 0, i, i, tmp, sizeof(tmp));
            snprintf(new, sizeof(new), "%s^disabled", tmp);
            ezplib_replace_rule(rule_set, i, new);
        }

        /* Unset the old and unused nvram variables. */
        /* XXX */
        nvram_unset("wl0_radio");
    }
#endif

    return 0;
}
int fw_func_1_7_6_to_1_7_5(void)
{
    int i, j, len, rule_num;
    char new[1024], old[1024];
    char tmp[64];
    char *str = &new[0];
    char *rule_set = "ntp_rule";
    char *value = nvram_safe_get(rule_set), *value2;

    printf("fw_func_1_7_6_to_1_7_5\n");
    /* Check the existence of the value. */
    if (value && *value) {
        i = j = 0;
        ezplib_get_attr_val(rule_set, i, "name", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "pool", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_attr_val(rule_set, i, "zone", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        len = snprintf(str, 1024 - j, "%s", tmp);
        j += len;
        str += len;

        ezplib_replace_rule(rule_set, i, new);
    }

    rule_num = atoi(nvram_safe_get("ipsec_rule_num"));
    rule_set = "ipsec_rule";
    
    for (i = j = 0; i < rule_num; i++) {
        str = &new[0];
        ezplib_get_rule(rule_set, i, old, 1024);
        /* If empty, nvram boot will create it. */ 
        /* Check the existence of the value. */
        if (old[0]) {
            /* name^enable */
            ezplib_get_subrule(rule_set, i, 0, 1, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* adv */
            ezplib_get_attr_val(rule_set, i, "adv", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init */
            ezplib_get_subrule(rule_set, i, 2, 10, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* phase1_mode */
            ezplib_get_attr_val(rule_set, i, "phase1_mode", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* phase1_auth */
            ezplib_get_attr_val(rule_set, i, "phase1_auth", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            if (!strcmp(tmp, "md5")) {
                len = snprintf(str, 1024 - j, "1^0^");
            } else if (!strcmp(tmp, "sha1")) {
                len = snprintf(str, 1024 - j, "0^1^");
            } else {
                len = snprintf(str, 1024 - j, "1^1^");
            }
            j += len;
            str += len;

            /* phase1_encrypt */
            ezplib_get_attr_val(rule_set, i, "phase1_encrypt", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            if (!strcmp(tmp, "des")) {
                len = snprintf(str, 1024 - j, "1^0^0^");
            } else if (!strcmp(tmp, "3des")) {
                len = snprintf(str, 1024 - j, "0^1^0^");
            } else if (!strcmp(tmp, "aes")) {
                len = snprintf(str, 1024 - j, "0^0^1^");
            } else {
                len = snprintf(str, 1024 - j, "1^1^1^");
            }
            j += len;
            str += len;

            ezplib_get_attr_val(rule_set, i, "phase1_id", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* phase2_auth */
            ezplib_get_attr_val(rule_set, i, "phase2_auth", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            if (!strcmp(tmp, "md5")) {
                len = snprintf(str, 1024 - j, "1^0^");
            } else if (!strcmp(tmp, "sha1")) {
                len = snprintf(str, 1024 - j, "0^1^");
            } else {
                len = snprintf(str, 1024 - j, "1^1^");
            }
            j += len;
            str += len;

            /* phase2_encrypt */
            ezplib_get_attr_val(rule_set, i, "phase2_encrypt", tmp, sizeof(tmp),
                    EZPLIB_USE_CLI);
            if (!strcmp(tmp, "des")) {
                len = snprintf(str, 1024 - j, "1^0^0^");
            } else if (!strcmp(tmp, "3des")) {
                len = snprintf(str, 1024 - j, "0^1^0^");
            } else if (!strcmp(tmp, "aes")) {
                len = snprintf(str, 1024 - j, "0^0^1^");
            } else {
                len = snprintf(str, 1024 - j, "0^0^0^");
            }
            j += len;
            str += len;

            /* phase1_lifetime^phase2_lifetime */
            ezplib_get_subrule(rule_set, i, 18, 19, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s^", tmp);
            j += len;
            str += len;

            /* pfs */
            len = snprintf(str, 1024 - j, "0^", tmp);
            j += len;
            str += len;

            /* ike_keymode^psk^rsa^ca */
            ezplib_get_subrule(rule_set, i, 11, 14, tmp, sizeof(tmp));
            len = snprintf(str, 1024 - j, "%s", tmp);
            j += len;
            str += len;

            ezplib_replace_rule(rule_set, i, new);
        }
    }

    value = nvram_safe_get("wl0_radio");
    value2 = nvram_safe_get("wl_basic_rule");

    if (!*value && *value2) {
        /* wl0_radio */
        rule_set = "wl_basic_rule";
        ezplib_get_attr_val(rule_set, 0, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        nvram_fset("wl0_radio", tmp);

        /* wl0_ssid */
        rule_set = "wl0_ssid_rule";
        ezplib_get_attr_val(rule_set, 0, "ssid", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        nvram_fset("wl0_ssid", tmp);

#if 0
        /* wl0_sec_mode */
        rule_set = "wl0_sec_rule";
        ezplib_get_attr_val(rule_set, 0, "secmode", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        nvram_set("wl0_sec_mode", tmp);

        /* wl0_key */
        rule_set = "wl0_sec_wep_rule";
        ezplib_get_attr_val(rule_set, 0, "key_index", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        nvram_set("wl0_key", tmp);

        /* wl0_key1 */
        rule_set = "wl0_sec_wep_rule";
        ezplib_get_attr_val(rule_set, 0, "key1", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        nvram_set("wl0_key1", tmp);

        /* wl0_key2 */
        rule_set = "wl0_sec_wep_rule";
        ezplib_get_attr_val(rule_set, 0, "key2", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        nvram_set("wl0_key1", tmp);

        /* wl0_key3 */
        rule_set = "wl0_sec_wep_rule";
        ezplib_get_attr_val(rule_set, 0, "key3", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        nvram_set("wl0_key1", tmp);

        /* wl0_key4 */
        rule_set = "wl0_sec_wep_rule";
        ezplib_get_attr_val(rule_set, 0, "key4", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        nvram_set("wl0_key1", tmp);

        /* wl0_wpa_psk */
#endif
        /* Unset the unused nvram variables. */
        nvram_unset("wl_basic_rule");
    }
    return 0;
}
int fw_func_1_7_6_to_1_7_7(void)
{
    char *rule_set;
    char new[1024], old[1024];
    char tmp[64];
    char *str = &new[0];
    int i, j, num, vnum;

    printf("fw_func_1_7_6_to_1_7_7\n");

#if (WL_NUM >0)
    rule_set = "wl_wds_rule";
    ezplib_get_attr_val(rule_set, 0, "mode", tmp, sizeof(tmp), EZPLIB_USE_CLI);
    if (tmp[0] == '0') {
        /* This is due to correct the error in fw_func_1_7_5_to_1_7_6(). */
        ezplib_get_attr_val(rule_set, 0, "timeout", tmp, sizeof(tmp),
                            EZPLIB_USE_CLI);
        snprintf(new, sizeof(new), "disabled^%s", tmp);
        ezplib_replace_rule(rule_set, 0, new);
    }

    rule_set = "wl0_wds_basic_rule";
    str = nvram_safe_get(rule_set);
    if (!strcmp(str, "0^1")) {
        /* This is due to correct the error in the original 
         * fw_func_1_7_5_to_1_7_6(). 
         */
        nvram_set(rule_set, WL0_WDS_BASIC_RULE);
    }

    /* wl_basic_rule:
     * From old format: enable^net_mode^txpower^channel
     * To new format: enable^net_mode^txpower^channel^bisolation
     */
    rule_set = "wl_basic_rule";
    num = atoi(nvram_safe_get("wl_num"));
    for (i = 0; i < num; i++) {
        ezplib_get_rule(rule_set, i, old, sizeof(old));
        snprintf(new, sizeof(new), "%s^0", old);
        ezplib_replace_rule(rule_set, i, new);
    }

    /* wlX_basic_rule:
     * From old format: enable^hidden^rate^wme
     * To new format: enable^hidden^rate^wme^isolation
     */
    num = atoi(nvram_safe_get("wl_num"));
    /* From 1.7.5 to 1.7.7 directly, wlv_rule_set hasn't been set. */
    vnum = atoi(xstr(WL0_RULE_NUM));
    for (i = 0; i < num; i++) {
        snprintf(tmp, sizeof(tmp), "wl%d_basic_rule", i);

        for (j = 0; j < vnum; j++) {
            ezplib_get_rule(tmp, j, old, sizeof(old));
            snprintf(new, sizeof(new), "%s^0", old);
            ezplib_replace_rule(tmp, j, new);
        }
    }
#endif

    /* Unset upnp_rule. So it falls back to default. */
    nvram_unset("upnp_rule");
}
int fw_func_1_7_7_to_1_7_6(void)
{
    char *rule_set;
    char new[1024], old[1024];
    char tmp[64];
    char *str = &new[0];
    int i, j, num, vnum;

    printf("fw_func_1_7_7_to_1_7_6\n");

    /* wl_basic_rule:
     * From new format: enable^net_mode^txpower^channel^bisolation
     * To old format: enable^net_mode^txpower^channel
     */
    rule_set = "wl_basic_rule";
    num = atoi(nvram_safe_get("wl_num"));
    for (i = 0; i < num; i++) {
        ezplib_get_subrule(rule_set, i, 0, 3, new, sizeof(new));
        ezplib_replace_rule(rule_set, i, new);
    }

    /* wlX_basic_rule:
     * From new format: enable^hidden^rate^wme^isolation
     * To old format: enable^hidden^rate^wme
     */
    num = atoi(nvram_safe_get("wl_num"));
    vnum = atoi(nvram_safe_get("wlv_rule_num"));
    for (i = 0; i < num; i++) {
        snprintf(tmp, sizeof(tmp), "wl%d_basic_rule", i);

        for (j = 0; j < vnum; j++) {
            ezplib_get_subrule(tmp, i, 0, 3, new, sizeof(new));
            ezplib_replace_rule(tmp, j, new);
        }
    }

    /* Unset upnp_rule. So it falls back to default. */
    nvram_unset("upnp_rule");

    /* Rewind the MAC address. */
    system("echo 1 > /proc/ezp_rewind");
}
int fw_func_1_7_7_to_1_7_8(void)
{
    printf("fw_func_1_7_7_to_1_7_8\n");
    return 0;
}
int fw_func_1_7_8_to_1_7_7(void)
{
    printf("fw_func_1_7_8_to_1_7_7\n");
    return 0;
}
int fw_func_1_7_8_to_1_7_9(void)
{
    char tmp[1024], name[1024], ipaddr_start[64], ipaddr_end[64];
    char new[1024], old[1024];
    char *str;
    int nrule, i, j, dbm_nrule, len, start, end, num;

    printf("fw_func_1_7_8_to_1_7_9\n");

    nvram_fset("bw_dbm_rule", "");
    nvram_fset("bw_dbm_rule_num", "0");
    nvram_fset("bw_sbm_rule", "");
    nvram_fset("bw_sbm_rule_num", "0");

    nrule = atoi(nvram_safe_get("fl_rule_num"));
    for (i = 0; i < nrule; i++) {
        /* dbm_enable */
        ezplib_get_subrule("fl_rule", i, 14, 14, tmp, sizeof(tmp));
        if (tmp[0] == '1') {
            /* The rule with DBM enabled. */
            ezplib_get_attr_val("fl_rule", i, "name", name, sizeof(name),
                    EZPLIB_USE_CLI);
            ezplib_get_attr_val("fl_rule", i, "srcipaddr_start", ipaddr_start,
                    sizeof(ipaddr_start), EZPLIB_USE_CLI);
            ezplib_get_attr_val("fl_rule", i, "srcipaddr_end", ipaddr_end,
                    sizeof(ipaddr_end), EZPLIB_USE_CLI);
            /* 1.7.8 had a bug in L4 that the dbm_num is mistaken as 32.
             * Therefore, we cannot directly use this attribute in fl_rule.
             */
            start = ntohl((int)inet_addr(ipaddr_start));
            end = ntohl((int)inet_addr(ipaddr_end));
            num = end - start + 1;
            snprintf(tmp, sizeof(tmp), "%s^1^%s^%s^%d^", name, ipaddr_start, ipaddr_end, num);
            ezplib_append_rule("bw_dbm_rule", tmp);

            /* Increase the number of DBM rules. */
            dbm_nrule = atoi(nvram_safe_get("bw_dbm_rule_num"));
            snprintf(tmp, sizeof(tmp), "%d", dbm_nrule+1);
            nvram_set("bw_dbm_rule_num", tmp);
        }

        /* Remove the attribute 'dbm_enable' and 'dbm_num' from fl_rule */
        j = 0;
        str = &new[0];

        ezplib_get_subrule("fl_rule", i, 0, 13, tmp, sizeof(tmp));
        len = snprintf(str, sizeof(new) - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_get_subrule("fl_rule", i, 16, 18, tmp, sizeof(tmp));
        len = snprintf(str, sizeof(new) - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_replace_rule("fl_rule", i, new);
    }

    return 0;
}
int fw_func_1_7_9_to_1_7_8(void)
{
    char tmp[1024], name[1024], start[64], end[64], num[64], mark[64];
    char new[1024];
    int nrule, i, j, fl_nrule, len;
    char *str;

    printf("fw_func_1_7_9_to_1_7_8\n");

    /* Add back the attribute 'dbm_enable' and 'dbm_num' into fl_rule. */
    nrule = atoi(nvram_safe_get("fl_rule_num"));
    for (i = 0; i < nrule; i++, j += atoi(num)) {
        char ipaddr_start[64], ipaddr_end[64];

        j = 0;
        str = &new[0];

        ezplib_get_subrule("fl_rule", i, 0, 13, tmp, sizeof(tmp));
        len = snprintf(str, sizeof(new) - j, "%s^", tmp);
        j += len;
        str += len;

        /* 'dbm_enable' and 'dbm_num' */
        /* dbm_num would be updated as a correct value when executing
         * save_fl().
         */
        len = snprintf(str, sizeof(new) - j, "0^0^", tmp);
        j += len;
        str += len;

        ezplib_get_subrule("fl_rule", i, 14, 16, tmp, sizeof(tmp));
        len = snprintf(str, sizeof(new) - j, "%s^", tmp);
        j += len;
        str += len;

        ezplib_replace_rule("fl_rule", i, new);
    }

    /* Move the rules in bw_dbm_rule to fl_rule. */
    nrule = atoi(nvram_safe_get("bw_dbm_rule_num"));
    for (i = 0, j = 0; i < nrule; i++, j += atoi(num)) {
        ezplib_get_attr_val("bw_dbm_rule", i, "enable", tmp, sizeof(tmp),
                EZPLIB_USE_CLI);
        if (tmp[0] == '0') {
            continue;
        }

        ezplib_get_attr_val("bw_dbm_rule", i, "name", name, sizeof(name),
                EZPLIB_USE_CLI);
        ezplib_get_attr_val("bw_dbm_rule", i, "inipaddr_start", start,
                sizeof(start), EZPLIB_USE_CLI);
        ezplib_get_attr_val("bw_dbm_rule", i, "inipaddr_end", end,
                sizeof(end), EZPLIB_USE_CLI);
        ezplib_get_attr_val("bw_dbm_rule", i, "inipaddr_num", num,
                sizeof(num), EZPLIB_USE_CLI);

        snprintf(mark, sizeof(mark), "%04X", j+1);
        snprintf(tmp, sizeof(tmp), "%s^1^^^%s^%s^^^^^^^^1^1^%s^%s^0^0", name, start,
                end, num, mark);

        ezplib_append_rule("fl_rule", tmp);

        fl_nrule = atoi(nvram_safe_get("fl_rule_num"));
        snprintf(tmp, sizeof(tmp), "%d", fl_nrule+1);

        nvram_set("fl_rule_num", tmp);
    }
        
    /* Roll back the wan_bw_rule. */
    nrule = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < nrule; i++) {
        char dl[64], ul[64], dl_max[64], dl_min[64], ul_max[64], ul_min[64];
        char *p_link_percent = "75";
        int link_percent = 75;
        int global_percent = 90;
        int user_percent = 90;
        int bw_num, used_ul, used_dl, ulmax, ulmin, dlmax, dlmin;

        bw_num = atoi(nvram_safe_get("dbm_max"));

        ezplib_get_attr_val("wan_bw_rule", i, "dl", dl, sizeof(dl),
                EZPLIB_USE_CLI);
        ezplib_get_attr_val("wan_bw_rule", i, "ul", ul, sizeof(ul),
                EZPLIB_USE_CLI);

        used_ul = atoi(ul);
        used_dl = atoi(dl);

        ulmax = used_ul * link_percent / 100;
        ulmax = ulmax * global_percent / 100;
        ulmax = ulmax * user_percent / 100;
        ulmin = ulmax / bw_num;

        dlmax = used_dl * link_percent / 100;
        dlmax = dlmax * global_percent / 100;
        dlmax = dlmax * user_percent / 100;
        dlmin = dlmax / bw_num;

        snprintf(ul_max, sizeof(ul_max), "%d", ulmax);
        snprintf(ul_min, sizeof(ul_min), "%d", ulmin);
        snprintf(dl_max, sizeof(dl_max), "%d", dlmax);
        snprintf(dl_min, sizeof(dl_min), "%d", dlmin);

        ezplib_replace_attr("wan_bw_rule", i, "link_percent", p_link_percent);
        ezplib_replace_attr("wan_bw_rule", i, "dbm_upmax", ul_max);
        ezplib_replace_attr("wan_bw_rule", i, "dbm_upmin", ul_min);
        ezplib_replace_attr("wan_bw_rule", i, "dbm_downmax", dl_max);
        ezplib_replace_attr("wan_bw_rule", i, "dbm_downmin", dl_min);
    }

    nvram_unset("bw_dbm_rule");
    nvram_unset("bw_dbm_rule_num");
    nvram_unset("bw_sbm_rule");
    nvram_unset("bw_sbm_rule_num");

    return 0;
}
int fw_func_1_7_9_to_1_7_10(void)
{
    printf("fw_func_1_7_9_to_1_7_10\n");
    return 0;
}
int fw_func_1_7_10_to_1_7_9(void)
{
    printf("fw_func_1_7_10_to_1_7_9\n");
    return 0;
}
int fw_func_1_7_10_to_1_7_11(void)
{
    printf("fw_func_1_7_10_to_1_7_11\n");
    nvram_set("bw_mask", "0xFFFF0000");
    return 0;
}
int fw_func_1_7_11_to_1_7_10(void)
{
    printf("fw_func_1_7_11_to_1_7_10\n");
    nvram_set("bw_mask", "0x00FFFFFF");
    return 0;
}
int fw_func_1_7_11_to_2_0_0(void)
{
    printf("fw_func_1_7_11_to_2_0_0\n");
    return 0;
}
int fw_func_2_0_0_to_1_7_11(void)
{
    printf("fw_func_2_0_0_to_1_7_11\n");
    return 0;
}
int fw_func_2_0_1_to_2_0_0(void)
{
    char codepage[64], swap_size[64];
    char tmp[1024];
    printf("fw_func_2_0_1_to_2_0_0\n");
    ezplib_get_attr_val("storage_rule", 0, "codepage",
            codepage, sizeof(codepage), EZPLIB_USE_CLI);
    ezplib_get_attr_val("storage_rule", 0, "swap_size",
            swap_size, sizeof(swap_size), EZPLIB_USE_CLI);
    snprintf(tmp, sizeof(tmp), "^%s^%s^",
            codepage, swap_size);
    ezplib_replace_rule("storage_rule", 0, tmp);
    nvram_unset("storage_state_rule");
    return 0;
}
int fw_func_2_0_0_to_2_0_1(void)
{
    char buf[64];
    char tmp[1024];
    printf("fw_func_2_0_0_to_2_0_1\n");
    nvram_fset("storage_state_rule", "^");
    ezplib_get_subrule("storage_rule", 0, 1, 2, buf, sizeof(buf));
    snprintf(tmp, sizeof(tmp), "%s^4096", buf);
    ezplib_replace_rule("storage_rule", 0, tmp);
    return 0;
}
int fw_func_2_0_2_to_2_0_1(void)
{
    char buf[64];
    char tmp[1024];
    printf("fw_func_2_0_2_to_2_0_1\n");
    ezplib_get_subrule("lan_dhcps_rule", 0, 0, 6, buf, sizeof(buf));
    snprintf(tmp, sizeof(tmp), "%s", buf);
    ezplib_replace_rule("lan_dhcps_rule", 0, tmp);

    /* stats_rule */
#ifdef CONFIG_EZP_ARCH_RT305X
#if ( WAN_NUM == 1 ) && ( LAN_NUM == 1 )
    ezplib_replace_rule("stats_rule", 0,
            "^1^60^vlan1 vlan2 ppp0 br0 ra0");
#elif ( WAN_NUM == 2 ) && ( LAN_NUM == 1 ) 
    ezplib_replace_rule("stats_rule", 0,
        "^1^60^vlan3 vlan1 vlan2 ppp0 ppp1 br0 ra0");
#endif
#endif
    return 0;
}
int fw_func_2_0_1_to_2_0_2(void)
{
    char buf[64];
    char tmp[1024];
    printf("fw_func_2_0_1_to_2_0_2\n");
    ezplib_get_subrule("lan_dhcps_rule", 0, 0, 6, buf, sizeof(buf));
    snprintf(tmp, sizeof(tmp), "%s^dnsrelay^^^^", buf);
    ezplib_replace_rule("lan_dhcps_rule", 0, tmp);

    /* stats_rule */
#ifdef CONFIG_EZP_ARCH_RT305X
#if ( WAN_NUM == 1 ) && ( LAN_NUM == 1 )
    ezplib_replace_rule("stats_rule", 0,
            "^1^60^vlan1 vlan2 ppp0 br0 ra0 usb0 eth0");
#elif ( WAN_NUM == 2 ) && ( LAN_NUM == 1 ) 
    ezplib_replace_rule("stats_rule", 0,
        "^1^60^vlan3 vlan1 vlan2 ppp0 ppp1 br0 ra0 usb0 eth0 eth1");
#endif
#endif
    return 0;
}
int fw_func_2_0_3_to_2_0_2(void)
{
    char buf[256], buf2[256];
    char tmp[1024];
    int i, nrule;

    printf("fw_func_2_0_3_to_2_0_2\n");

    nrule = atoi(nvram_safe_get("wan_num"));
    /* wan_pppoe_rule */
    for (i = 0; i < nrule; i++) {
        ezplib_get_subrule("wan_pppoe_rule", i, 1, 8, buf, sizeof(buf));
        ezplib_replace_rule("wan_pppoe_rule", i, buf);
    }

    /* wan_wwan_rule */
    for (i = 0; i < nrule; i++) {
        ezplib_get_subrule("wan_wwan_rule", i, 0, 5, buf, sizeof(buf));
        ezplib_get_subrule("wan_wwan_rule", i, 7, 21, buf2, sizeof(buf2));
        snprintf(tmp, sizeof(tmp), "%s^%s", buf, buf2);
        ezplib_replace_rule("wan_wwan_rule", i, tmp);
    }

    ezplib_get_subrule("ftpd_rule", 0, 0, 9, buf, sizeof(buf));
    snprintf(tmp, sizeof(tmp), "%s", buf);
    ezplib_replace_rule("ftpd_rule", 0, tmp);
    
    /* Wireless advanced rule downgrade */
    nrule = atoi(nvram_safe_get("wl_rule_num"));
    for (i = 0; i < nrule; i++) {
        ezplib_get_subrule("wl_advanced_rule", i, 0, 13, buf, sizeof(buf));
        ezplib_replace_rule("wl_advanced_rule", i, buf);
    }

    /* Routing rule */
    nrule = atoi(nvram_safe_get("rt_rule_num"));
    for (i = 0; i < nrule; i++) {
        ezplib_get_subrule("rt_rule", i, 0, 11, buf, sizeof(buf));
        ezplib_replace_rule("rt_rule", i, buf);
    }
    
    /* OpenDNS rule */
    nrule = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < nrule; i++) {
        ezplib_get_subrule("wan_opendns_rule", i, 0, 4, buf, sizeof(buf));
        ezplib_replace_rule("wan_opendns_rule", i, buf);
    }

    return 0;
}
int fw_func_2_0_2_to_2_0_3(void)
{
    char buf[256], buf2[256];
    char tmp[1024];
    int i, nrule;

    printf("fw_func_2_0_2_to_2_0_3\n");

    nrule = atoi(nvram_safe_get("wan_num"));
    /* wan_pppoe_rule */
    for (i = 0; i < nrule; i++) {
        ezplib_get_subrule("wan_pppoe_rule", i, 0, 7, buf, sizeof(buf));
        snprintf(tmp, sizeof(tmp), "chap^%s", buf);
        ezplib_replace_rule("wan_pppoe_rule", i, tmp);
    }
    /* wan_wwan_rule */
    for (i = 0; i < nrule; i++) {
        ezplib_get_subrule("wan_wwan_rule", i, 0, 5, buf, sizeof(buf));
        ezplib_get_subrule("wan_wwan_rule", i, 6, 20, buf2, sizeof(buf2));
        snprintf(tmp, sizeof(tmp), "%s^chap^%s", buf, buf2);
        ezplib_replace_rule("wan_wwan_rule", i, tmp);
    }
    
    ezplib_get_subrule("ftpd_rule", 0, 0, 9, buf, sizeof(buf));
    snprintf(tmp, sizeof(tmp), "%s^0", buf);
    ezplib_replace_rule("ftpd_rule", 0, tmp);
    nvram_unset("samba_rule");

    /* Wireless advanced rule */
    nrule = atoi(nvram_safe_get("wl_rule_num"));
    for (i = 0; i < nrule; i++) {
        ezplib_get_rule("wl_advanced_rule", i, buf, sizeof(buf));
        snprintf(tmp, sizeof(tmp), "%s^1^0^0^0^0^0^1^1^0^33^5^0^1^8^0", buf);
        ezplib_replace_rule("wl_advanced_rule", i, tmp);
    }

    /* Routing rule */
    nrule = atoi(nvram_safe_get("rt_rule_num"));
    for (i = 0; i < nrule; i++) {
        ezplib_get_rule("rt_rule", i, buf, sizeof(buf));
        snprintf(tmp, sizeof(tmp), "%s^default^", buf);
        ezplib_replace_rule("rt_rule", i, tmp);
    }

    /* Dhcp rule */
    /* Replace DNS setting to default opendns */
    ezplib_replace_attr("lan_dhcps_rule", 0, "dnstype", "opendns");
    ezplib_replace_attr("lan_dhcps_rule", 0, "dnsaddr1", "208.67.220.220");
    ezplib_replace_attr("lan_dhcps_rule", 0, "dnsaddr2", "208.67.222.222");

    /* OpenDNS rule */
    nrule = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < nrule; i++) {
        ezplib_get_rule("wan_opendns_rule", i, buf, sizeof(buf));
        snprintf(tmp, sizeof(tmp), "%s^", buf);
        ezplib_replace_rule("wan_opendns_rule", i, tmp);
    }

    return 0;
}
int fw_func_2_0_4_to_2_0_3(void)
{
    char buf[256], buf2[256];
    char tmp[1024];
    int i, nrule;
    printf("fw_func_2_0_4_to_2_0_3\n");
    /* get bt rule attr */
    /* bt rule
     * enable[0|1]^peer^max_peer^max_task^download_rate^upload_rate^directory^session^load_start^port_range_start^port_range_end^dht_port^check_hash */
    ezplib_get_subrule("bt_rule", 0, 1, 1, buf, sizeof(buf));
    ezplib_get_subrule("bt_rule", 0, 3, 12, buf2, sizeof(buf2));

    /* set rtorrent_rule */
    /*rtorrent rule
     * enable[0|1]^min_peer^max_peer^max_upload^download_rate^upload_rate^directory^session^load_start^port_range_start^port_range_end^dht_port^check_hash
     */
    snprintf(tmp, sizeof(tmp), "1^10^%s^2^%s" , buf, buf2);
    nvram_set("rtorrent_rule", tmp);
    ezplib_get_subrule("bt_torrent_max", 0, 0, 0, buf, sizeof(buf));
    nvram_set("rtorrent_torrent_max", buf);
    
    /* unset bt rule */
    nvram_unset("bt_rule");
    nvram_unset("bt_torrent_max");

    /* wan_wwan_rule */
    nrule = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < nrule; i++) {
    /* apn_type^location^isp^apn^pin^mode^auth^username^passwd^demand^
     * idletime^redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^
     * ctrltty^devname^flag^turbolink
     */
        ezplib_get_subrule("wan_wwan_rule", i, 0, 21, buf, sizeof(buf));

    /* apn_type^location^isp^apn^pin^mode^auth^username^passwd^demand^
     * idletime^redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^
     * ctrltty^devname^flag
     */
        ezplib_replace_rule("wan_wwan_rule", i, buf);
    }

   /* wan_htc_rule */
    nrule = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < nrule; i++) {
   /* hostname^domain^lease^mtu^turbolink */
        ezplib_get_subrule("wan_htc_rule", i, 0, 3, buf, sizeof(buf));

   /* hostname^domain^lease^mtu */
        ezplib_replace_rule("wan_htc_rule", i, buf);
    }
    return 0;
}
int fw_func_2_0_3_to_2_0_4(void)
{
    char buf[256], buf2[256];
    char tmp[1024];
    int i, nrule;
    printf("fw_func_2_0_3_to_2_0_4\n");
    /* get rtorrent rule attr */
    /*rtorrent rule
     * enable[0|1]^min_peer^max_peer^max_upload^download_rate^upload_rate^directory^session^load_start^port_range_start^port_range_end^dht_port^check_hash
     */
    ezplib_get_subrule("rtorrent_rule", 0, 2, 2, buf, sizeof(buf));
    ezplib_get_subrule("rtorrent_rule", 0, 4, 12, buf2, sizeof(buf2));
    /* set bt_rule */
    /* bt rule
     * enable[0|1]^peer^max_peer^max_task^download_rate^upload_rate^directory^session^load_start^port_range_start^port_range_end^dht_port^check_hash
     */
    snprintf(tmp, sizeof(tmp), "1^%s^250^10^%s" , buf, buf2);
    nvram_set("bt_rule", tmp);
    /* unset rtorrent rule */
    nvram_unset("rtorrent_rule");
    nvram_unset("rtorrent_torrent_max");

    /* wan_wwan_rule */
    nrule = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < nrule; i++) {
    /* apn_type^location^isp^apn^pin^mode^auth^username^passwd^demand^
     * idletime^redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^
     * ctrltty^devname^flag
     */
        ezplib_get_subrule("wan_wwan_rule", i, 0, 21, buf, sizeof(buf));

    /* apn_type^location^isp^apn^pin^mode^auth^username^passwd^demand^
     * idletime^redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^
     * ctrltty^devname^flag^turbolink
     */
        snprintf(tmp, sizeof(tmp), "%s^0", buf);
        ezplib_replace_rule("wan_wwan_rule", i, tmp);
    }

    /* wan_htc_rule */
    nrule = atoi(nvram_safe_get("wan_num"));
    for (i = 0; i < nrule; i++) {
    /* hostname^domain^lease^mtu */
        ezplib_get_subrule("wan_htc_rule", i, 0, 3, buf, sizeof(buf));

    /* hostname^domain^lease^mtu^turbolink */
        snprintf(tmp, sizeof(tmp), "%s^0", buf);
        ezplib_replace_rule("wan_htc_rule", i, tmp);
    }
    return 0;
}
int fw_func_2_0_5_to_2_0_4(void)
{
    char buf[256], tmp[256];
    int i, num;
    printf("fw_func_2_0_5_to_2_0_4\n");
    /* Modify storage_rule form 
     * codepage^swap_size^block_size^ext_enable^target_ip^target_dir^user^pw^gp
     * to
     * codepage^swap_size^block_size
     */
    ezplib_get_subrule("storage_rule", 0, 0, 2, tmp, sizeof(tmp));
    ezplib_replace_rule("storage_rule", 0, tmp);

    /* Modify fw_rule form 
     * enable^syn_enable^icmp_enable^icmp_redir_enable
     * ^rpfilter_enable^state_enable^bc_storm_enable
     * to
     * enable^syn_enable^icmp_enable^icmp_redir_enable
     * ^rpfilter_enable^state_enable
     */
    ezplib_get_subrule("fw_rule", 0, 0, 5, tmp, sizeof(tmp));
    ezplib_replace_rule("fw_rule", 0, tmp);
    /* Modify vlan_rule
     * name^enable[0|1]^ifname^vid^portmember 
     * to
     * name^enable[0|1]^ifname^vid^portmember^tag^flag */
    num = atoi(nvram_safe_get("vlan_rule_num"));
    for (i = 0; i < num; i++) {
        ezplib_get_subrule("vlan_rule", i, 0, 4, tmp, sizeof(tmp));
        snprintf(buf, sizeof(buf), "%s^0000001^", tmp);
        ezplib_replace_rule("vlan_rule", i, buf);
    }
    /* unset vlanport_rule vlanport_rule_num vlanport_rule_max */
    nvram_unset("vlanport_rule");
    nvram_unset("vlanport_rule_num");
    nvram_unset("vlanport_rule_max");
    return 0;

    /* Modify bw_rule_max, bw_sbm_rule_max and bw_dbm_rule_maX
     * SUBCAT 2 from 32 to 8
     * SUBCAT 3 and 4 from 64 to 32 */
    ezplib_get_rule("prod_subcat", 0, tmp, sizeof(tmp));
    if (!strcmp(tmp, "2")) {
        ezplib_replace_rule("bw_rule_max",0,"8");
        ezplib_replace_rule("bw_sbm_rule_max",0,"8");
        ezplib_replace_rule("bw_dbm_rule_max",0,"8");
    }
    if (!strcmp(tmp, "3") || !strcmp(tmp, "4")) {
        ezplib_replace_rule("bw_rule_max",0,"16");
        ezplib_replace_rule("bw_sbm_rule_max",0,"16");
        ezplib_replace_rule("bw_dbm_rule_max",0,"16");
    }
}
int fw_func_2_0_4_to_2_0_5(void)
{
    char tmp[256], buf[256];
    int i, num;
    printf("fw_func_2_0_4_to_2_0_5\n");
    /* Modify storage_rule form 
     * codepage^swap_size^block_size
     * to
     * codepage^swap_size^block_size^ext_enable^target_ip^target_dir^user^pw^gp
     */
    ezplib_get_rule("storage_rule", 0, tmp, sizeof(tmp));
    snprintf(buf, sizeof(buf), "%s^0^^^^^", tmp);
    ezplib_replace_rule("storage_rule", 0, buf);

    /* Modify fw_rule form 
     * enable^syn_enable^icmp_enable^icmp_redir_enable
     * ^rpfilter_enable^state_enable
     * to
     * enable^syn_enable^icmp_enable^icmp_redir_enable
     * ^rpfilter_enable^state_enable^bc_storm_enable
     */
    ezplib_get_rule("fw_rule", 0, tmp, sizeof(tmp));
    snprintf(buf, sizeof(buf), "%s^0", tmp);
    ezplib_replace_rule("fw_rule", 0, buf);
    /* Modify vlan_rule and vlanport_rule
     * name^enable[0|1]^ifname^vid^portmember^tag^flag
     * to
     * name^enable[0|1]^ifname^vid^portmember */
    num = atoi(nvram_safe_get("vlan_rule_num"));
    for (i = 0; i < num; i++) {
        ezplib_get_subrule("vlan_rule", i, 0, 4, tmp, sizeof(tmp));
        ezplib_replace_rule("vlan_rule", i, tmp);
    }
    /* default vlanport_rule for 2.0.5 */
    nvram_default_rule("vlanport_rule");

    /* Modify bw_rule_max, bw_sbm_rule_max and bw_dbm_rule_maX
     * SUBCAT 2 from 8 to 32 
     * SUBCAT 3 and 4 from 32 to 64 */
    ezplib_get_rule("prod_subcat", 0, tmp, sizeof(tmp));
    if (!strcmp(tmp, "2")) {
        ezplib_replace_rule("bw_rule_max",0,"32");
        ezplib_replace_rule("bw_sbm_rule_max",0,"32");
        ezplib_replace_rule("bw_dbm_rule_max",0,"32");
    }
    if (!strcmp(tmp, "3") || !strcmp(tmp, "4")) {
        ezplib_replace_rule("bw_rule_max",0,"64");
        ezplib_replace_rule("bw_sbm_rule_max",0,"64");
        ezplib_replace_rule("bw_dbm_rule_max",0,"64");
    }
     
    return 0;
}

int fw_func_2_0_5_to_2_0_6(void)
{
    char tmp[256], b64[256], buf[256];
    unsigned long n = 256;
    int i,nrule;
    printf("fw_func_2_0_5_to_2_0_6\n");
    /* base64 encode pppoe username and passwd */
    ezplib_get_attr_val("wan_pppoe_rule", 0, "username", tmp, sizeof(tmp),
            EZPLIB_USE_WEB);
    base64_encode((unsigned char *)tmp, strlen(tmp), (unsigned char *)b64, &n);
    ezplib_replace_attr("wan_pppoe_rule", 0, "username", b64);
    ezplib_get_attr_val("wan_pppoe_rule", 0, "passwd", tmp, sizeof(tmp),
            EZPLIB_USE_WEB);
    n = 256;
    base64_encode((unsigned char *)tmp, strlen(tmp), (unsigned char *)b64, &n);
    ezplib_replace_attr("wan_pppoe_rule", 0, "passwd", b64);

    /* Modify ipsec_rule from 
        name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^phase1_mode^phase1_auth^phase1_encrypt^phase1_group^phase1_id^phase2_auth^phase2_encrypt^phase2_group^phase1_lifetime^phase2_lifetime   
        to
        name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^phase1_mode^phase1_auth^phase1_encrypt^phase1_group^phase1_id^phase2_auth^phase2_encrypt^phase2_group^phase1_lifetime^phase2_lifetime^dpd_enable^dpd_interval^dpd_timeout
     */
    nrule = atoi(nvram_safe_get("ipsec_rule_num"));
    for (i = 0; i < nrule; i++) {
        ezplib_get_subrule("ipsec_rule", i, 0, 25, buf, sizeof(buf));
        /* enable=0, interval=10, timeout=60 */
        snprintf(tmp, sizeof(tmp), "%s^0^10^60", buf);
        ezplib_replace_rule("ipsec_rule", i, tmp);
    }

    return 0;
}

int fw_func_2_0_6_to_2_0_5(void)
{
    char tmp[256];
    printf("fw_func_2_0_6_to_2_0_5\n");
    /* base64 decode pppoe username and passwd */
    ezplib_get_attr_val("wan_pppoe_rule", 0, "username", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    ezplib_replace_attr("wan_pppoe_rule", 0, "username", tmp);
    ezplib_get_attr_val("wan_pppoe_rule", 0, "passwd", tmp, sizeof(tmp),
            EZPLIB_USE_CLI);
    ezplib_replace_attr("wan_pppoe_rule", 0, "passwd", tmp);

    /* Modify ipsec_rule from 
        name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^phase1_mode^phase1_auth^phase1_encrypt^phase1_group^phase1_id^phase2_auth^phase2_encrypt^phase2_group^phase1_lifetime^phase2_lifetime^dpd_enable^dpd_interval^dpd_timeout
        to
        name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^phase1_mode^phase1_auth^phase1_encrypt^phase1_group^phase1_id^phase2_auth^phase2_encrypt^phase2_group^phase1_lifetime^phase2_lifetime   
     */
    ezplib_get_subrule("ipsec_rule", 0, 0, 25, tmp, sizeof(tmp));
    ezplib_replace_rule("ipsec_rule", 0, tmp);

    return 0;
}

int fw_func_2_0_6_to_2_0_7(void)
{
    char tmp[256], buf[256];
    printf("fw_func_2_0_6_to_2_0_7\n");
    nvram_default_rule("appmgr_rule");
    ezplib_get_subrule("cron_rule", 0, 0, 2, buf, sizeof(buf));
    snprintf(tmp, sizeof(tmp), "%s^0^week^03^09^30", buf);
    ezplib_replace_rule("cron_rule", 0, tmp);
    ezplib_get_subrule("log_rule", 0, 1, 3, buf, sizeof(buf));
    snprintf(tmp, sizeof(tmp), "syslog^%s^1^1^1^1^1^1^1^1^1^1^1^0^0^192.168.1.2^tcp^514", buf);
    ezplib_replace_rule("log_rule", 0, tmp);
    /* change wan_bw_rule link percent default value to 90.
     * This value is used to set the tc class local rate at ver 2.0.7.*/
    ezplib_replace_attr("wan_bw_rule", 0, "link_percent", "90");
    /* default wan_main_rule , wan_detect_rule */
    nvram_default_rule("wan_main_rule");
    nvram_default_rule("wan_detect_rule");
    return 0;
}

int fw_func_2_0_7_to_2_0_6(void)
{
    char tmp[256];
    printf("fw_func_2_0_7_to_2_0_6\n");
    nvram_unset("appmgr_rule");
    ezplib_get_subrule("cron_rule", 0, 0, 2, tmp, sizeof(tmp));
    ezplib_replace_rule("cron_rule", 0, tmp);
    ezplib_get_subrule("log_rule", 0, 0, 3, tmp, sizeof(tmp));
    ezplib_replace_rule("log_rule", 0, tmp);
    /* change wan_bw_rule link percent default value to 75 */
    ezplib_replace_attr("wan_bw_rule", 0, "link_percent", "75");
    /* default wan_main_rule */
    /* unset wan_status_rule, wan_lb_rule, wan_upnp_status_rule and
     * lan_status_rule */
    nvram_unset("wan_main_rule");
    nvram_default_rule("wan_status_rule");
    nvram_default_rule("lan_status_rule");
    nvram_default_rule("wan_lb_rule");
    nvram_default_rule("wan_upnp_status_rule");
    return 0;
}

int fw_func_2_0_7_to_2_0_8(void)
{
    printf("fw_func_2_0_7_to_2_0_8\n");
    nvram_default_rule("wan_status_rule");
    nvram_default_rule("snmp_rule");
    nvram_default_rule("cron_rule");
    return 0;
}

int fw_func_2_0_8_to_2_0_7(void)
{
    printf("fw_func_2_0_8_to_2_0_7\n");
    nvram_default_rule("wan_status_rule");
    nvram_default_rule("snmp_rule");
    nvram_default_rule("cron_rule");
    return 0;
}
/* 2.0.9 */
int fw_func_2_0_8_to_2_0_9(void)
{
    printf("fw_func_2_0_8_to_2_0_9\n");
    return 0;
}
int fw_func_2_0_9_to_2_0_8(void)
{
    printf("fw_func_2_0_9_to_2_0_8\n");
    return 0;
}
/* 2.0.10 */
int fw_func_2_0_9_to_2_0_10(void)
{
    printf("%s\n", __func__);
    return 0;
}
int fw_func_2_0_10_to_2_0_9(void)
{
    printf("%s\n", __func__);
    return 0;
}
/* 2.0.11 */
int fw_func_2_0_10_to_2_0_11(void)
{
    printf("%s\n", __func__);
    return 0;
}
int fw_func_2_0_11_to_2_0_10(void)
{
    printf("%s\n", __func__);
    return 0;
}
/* 2.0.12 */
int fw_func_2_0_11_to_2_0_12(void)
{
    printf("%s\n", __func__);
    return 0;
}
int fw_func_2_0_12_to_2_0_11(void)
{
    printf("%s\n", __func__);
    return 0;
}
/* 2.0.13 */
int fw_func_2_0_12_to_2_0_13(void)
{
    printf("%s\n", __func__);
    return 0;
}
int fw_func_2_0_13_to_2_0_12(void)
{
    printf("%s\n", __func__);
    return 0;
}
/* 2.0.14 */
int fw_func_2_0_13_to_2_0_14(void)
{
    printf("%s\n", __func__);
    return 0;
}
int fw_func_2_0_14_to_2_0_13(void)
{
    printf("%s\n", __func__);
    return 0;
}
/* 2.0.15 */
int fw_func_2_0_14_to_2_0_15(void)
{
    printf("%s\n", __func__);
    return 0;
}
int fw_func_2_0_15_to_2_0_14(void)
{
    printf("%s\n", __func__);
    return 0;
}
/* 3.0.0 */
int fw_func_2_0_15_to_3_0_0(void)
{
    char buf[128], b64[128];
    unsigned long n = sizeof(b64);

    printf("%s\n", __func__);
    /* construct base64 encoded ssid for apcli */
    ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, sizeof(buf),
            EZPLIB_USE_CLI);
    base64_encode((unsigned char *)buf, strlen(buf), (unsigned char *)b64, &n);
    b64[n] = 0;
    ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", b64);
#ifdef PLATFORM_BAD_m
    /* switch to swap enabled wisp structure */
    ezplib_replace_attr("wl0_apcli_swap_basic_rule", 0, "enable", "1");
#endif
    return 0;
}
int fw_func_3_0_0_to_2_0_15(void)
{
    char buf[128], b64[128];
    unsigned long n = sizeof(b64);

    printf("%s\n", __func__);
    /* construct base64 encoded ssid for apcli */
    ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, sizeof(buf),
            EZPLIB_USE_CLI);
    base64_decode((unsigned char *)buf, strlen(buf), (unsigned char *)b64, &n);
    b64[n] = 0;
    ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", b64);
#ifdef PLATFORM_BAD_m
    /* switch to swap enabled wisp structure */
    ezplib_replace_attr("wl0_apcli_swap_basic_rule", 0, "enable", "0");
#endif
    return 0;
}
/* 3.0.1 */
int fw_func_3_0_0_to_3_0_1(void)
{
    printf("upgrade to 3.0.1\n");
    return 0;
}
int fw_func_3_0_1_to_3_0_0(void)
{
    printf("downgrade from 3.0.0\n");
    return 0;
}
/* 3.0.2 */
int fw_func_3_0_1_to_3_0_2(void)
{
    printf("upgrade to 3.0.2\n");
    return 0;
}
int fw_func_3_0_2_to_3_0_1(void)
{
    printf("downgrade from 3.0.1\n");
    return 0;
}
/* 3.0.3 */
int fw_func_3_0_2_to_3_0_3(void)
{
    printf("upgrade to 3.0.3\n");
    return 0;
}
int fw_func_3_0_3_to_3_0_2(void)
{
    printf("downgrade from 3.0.2\n");
    return 0;
}
/* 3.0.4 */
int fw_func_3_0_3_to_3_0_4(void)
{
    printf("upgrade to 3.0.4\n");
    return 0;
}
int fw_func_3_0_4_to_3_0_3(void)
{
    printf("downgrade from 3.0.3\n");
    return 0;
}
/* 3.0.5 */
int fw_func_3_0_4_to_3_0_5(void)
{
    printf("upgrade to 3.0.5\n");
    return 0;
}
int fw_func_3_0_5_to_3_0_4(void)
{
    printf("downgrade from 3.0.4\n");
    return 0;
}
/* 3.0.6 */
int fw_func_3_0_5_to_3_0_6(void)
{
    printf("upgrade to 3.0.6\n");
    return 0;
}
int fw_func_3_0_6_to_3_0_5(void)
{
    printf("downgrade from 3.0.5\n");
    return 0;
}
/* 3.0.7 */
int fw_func_3_0_6_to_3_0_7(void)
{
    printf("upgrade to 3.0.7\n");
    return 0;
}
int fw_func_3_0_7_to_3_0_6(void)
{
    printf("downgrade from 3.0.6\n");
    return 0;
}
/* 3.0.8 */
int fw_func_3_0_7_to_3_0_8(void)
{
#if defined (EZP_PROD_BRAND_SONY)
    printf("SONY upgrade to 3.0.8\n");
    nvram_default_rule("ntp_rule");
#else
    printf("upgrade to 3.0.8\n");
#endif
    return 0;
}
int fw_func_3_0_8_to_3_0_7(void)
{
#if defined (EZP_PROD_BRAND_SONY)
    printf("SONY downgrade to 3.0.7\n");
    nvram_fset("ntp_rule", "^1^pool^north-america.pool.ntp.org^^PST8PDT,M3.2.0/2,M11.1.0/2^1");
    nvram_commit(); 
#else
    printf("downgrade from 3.0.7\n");
#endif
    return 0;
}
/* 3.0.9 */
int fw_func_3_0_8_to_3_0_9(void)
{
    printf("upgrade to 3.0.9\n");
    return 0;
}
int fw_func_3_0_9_to_3_0_8(void)
{
    printf("downgrade from 3.0.8\n");
    return 0;
}
/* 3.0.10 */
int fw_func_3_0_9_to_3_0_10(void)
{
    printf("upgrade to 3.0.10\n");
    return 0;
}
int fw_func_3_0_10_to_3_0_9(void)
{
    printf("downgrade from 3.0.9\n");
    return 0;
}
/* 3.0.11 */
int fw_func_3_0_10_to_3_0_11(void)
{
    printf("upgrade to 3.0.11\n");
    return 0;
}
int fw_func_3_0_11_to_3_0_10(void)
{
    printf("downgrade from 3.0.10\n");
    return 0;
}
/* 3.0.12 */
int fw_func_3_0_11_to_3_0_12(void)
{
    printf("upgrade to 3.0.12\n");
    return 0;
}
int fw_func_3_0_12_to_3_0_11(void)
{
    printf("downgrade from 3.0.11\n");
    return 0;
}
/* 3.0.13 */
int fw_func_3_0_12_to_3_0_13(void)
{
    printf("upgrade to 3.0.13\n");
    return 0;
}
int fw_func_3_0_13_to_3_0_12(void)
{
    printf("downgrade from 3.0.12\n");
    return 0;
}
/* 3.0.14 */
int fw_func_3_0_13_to_3_0_14(void)
{
    printf("upgrade to 3.0.14\n");
    return 0;
}
int fw_func_3_0_14_to_3_0_13(void)
{
    printf("downgrade from 3.0.13\n");
    return 0;
}
/* 3.0.15 */
int fw_func_3_0_14_to_3_0_15(void)
{
    printf("upgrade to 3.0.15\n");
    return 0;
}
int fw_func_3_0_15_to_3_0_14(void)
{
    printf("downgrade from 3.0.14\n");
    return 0;
}
/* 3.0.16 */
int fw_func_3_0_15_to_3_0_16(void)
{
    printf("upgrade to 3.0.16\n");
    return 0;
}
int fw_func_3_0_16_to_3_0_15(void)
{
    printf("downgrade from 3.0.15\n");
    return 0;
}
/* 3.0.17 */
int fw_func_3_0_16_to_3_0_17(void)
{
    printf("upgrade to 3.0.17\n");
    return 0;
}
int fw_func_3_0_17_to_3_0_16(void)
{
    printf("downgrade from 3.0.16\n");
    return 0;
}
/* 3.0.18 */
int fw_func_3_0_17_to_3_0_18(void)
{
    printf("upgrade to 3.0.18\n");
    return 0;
}
int fw_func_3_0_18_to_3_0_17(void)
{
    printf("downgrade from 3.0.17\n");
    return 0;
}
/* 3.0.19 */
int fw_func_3_0_18_to_3_0_19(void)
{
    printf("upgrade to 3.0.19\n");
    return 0;
}
int fw_func_3_0_19_to_3_0_18(void)
{
    printf("downgrade from 3.0.18\n");
    return 0;
}
/* 3.0.20 */
int fw_func_3_0_19_to_3_0_20(void)
{
    printf("upgrade to 3.0.20\n");
    return 0;
}
int fw_func_3_0_20_to_3_0_19(void)
{
    printf("downgrade from 3.0.19\n");
    return 0;
}
/* 3.1.0 */
int fw_func_3_0_20_to_3_1_0(void)
{
    printf("upgrade to 3.1.0\n");
#if defined(PLATFORM_M2E)
    nvram_default();
    nvram_commit();
#endif
#ifdef PLATFORM_AXA
    nvram_default_rule("wl0_apcli_swap_rule");
    nvram_default_rule("wl0_apcli_rule");
    nvram_default_rule("appmgr_rule_G");
    nvram_default_rule("appmgr_rule_I");
    nvram_commit();
    /* FIXME!!!
     * check if wifi security key and admin password need to be handled. */
#endif
#if defined (EZP_PROD_BRAND_ALLIS) 
    nvram_default();
#endif   
    return 0;
}
int fw_func_3_1_0_to_3_0_20(void)
{
    printf("downgrade from 3.1.0\n");
#if defined(PLATFORM_M2E)
    nvram_default();
    nvram_commit();
#endif
#ifdef PLATFORM_AXA
    nvram_default_rule("wl0_apcli_swap_rule");
    nvram_default_rule("wl0_apcli_rule");
    nvram_default_rule("appmgr_rule_G");
    nvram_default_rule("appmgr_rule_I");
    nvram_commit();
    /* FIXME!!!
     * check if wifi security key and admin password need to be handled. */
#endif
    return 0;
}
/* 3.1.1 */
int fw_func_3_1_0_to_3_1_1(void)
{
    printf("upgrade to 3.1.1\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_1_to_3_1_0(void)
{
    printf("downgrade from 3.1.1\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}

/* 3.1.2 */
int fw_func_3_1_1_to_3_1_2(void)
{
    printf("%s\n", __func__);
#if defined(PLATFORM_M2E)
    /* Due to the model is changed in emergency firmware, in case it follow to
     * the exporting version, we have to do this upgrade modification. */
    nvram_fset("model", xstr(EZP_MODEL));
    nvram_commit();
#endif
    return 0;
}
int fw_func_3_1_2_to_3_1_1(void)
{
    printf("%s\n", __func__);
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.1.3 */
int fw_func_3_1_2_to_3_1_3(void)
{
    printf("upgrade to 3.1.3\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_3_to_3_1_2(void)
{
    printf("downgrade from 3.1.2\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.1.4 */
int fw_func_3_1_3_to_3_1_4(void)
{
    printf("upgrade to 3.1.4\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_4_to_3_1_3(void)
{
    printf("downgrade from 3.1.3\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.1.5 */
int fw_func_3_1_4_to_3_1_5(void)
{
    printf("upgrade to 3.1.5\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_5_to_3_1_4(void)
{
    printf("downgrade from 3.1.4\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.1.6 */
int fw_func_3_1_5_to_3_1_6(void)
{
    printf("upgrade to 3.1.6\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_6_to_3_1_5(void)
{
    printf("downgrade from 3.1.5\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.1.7 */
int fw_func_3_1_6_to_3_1_7(void)
{
    printf("upgrade to 3.1.7\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_7_to_3_1_6(void)
{
    printf("downgrade from 3.1.6\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.1.8 */
int fw_func_3_1_7_to_3_1_8(void)
{
    printf("upgrade to 3.1.8\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_8_to_3_1_7(void)
{
    printf("downgrade from 3.1.7\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.1.9 */
int fw_func_3_1_8_to_3_1_9(void)
{
    printf("upgrade to 3.1.9\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_9_to_3_1_8(void)
{
    printf("downgrade from 3.1.8\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.1.10 */
int fw_func_3_1_9_to_3_1_10(void)
{
    printf("upgrade to 3.1.10\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_10_to_3_1_9(void)
{
    printf("downgrade from 3.1.9\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
/* 3.2.0 */
int fw_func_3_2_0_to_3_1_9(void)
{
    printf("downgrade from 3.1.9\n");
    nvram_default();
    nvram_commit();
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_1_9_to_3_2_0(void)
{
    printf("upgrade to 3.2.0\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_2_0_to_3_2_1(void)
{
    printf("upgrade to 3.2.1\n");
    nvram_default();
    nvram_commit();
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_2_1_to_3_2_0(void)
{
    printf("downgrade from 3.2.0\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}

int fw_func_3_2_1_to_3_2_2(void)
{
    printf("upgrade to 3.2.2\n");
    nvram_default();
    nvram_commit();
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
int fw_func_3_2_2_to_3_2_1(void)
{
    printf("downgrade from 3.2.1\n");
#if defined(PLATFORM_M2E)
#endif
    return 0;
}
