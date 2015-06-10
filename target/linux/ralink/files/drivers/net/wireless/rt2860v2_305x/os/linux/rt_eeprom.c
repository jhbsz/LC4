/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	eeprom.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Name		Date			Modification logs
*/
#include <rt_config.h>

#if defined(CONFIG_RALINK_RT3050AP_1T1R) || defined(CONFIG_RALINK_RT3050STA_1T1R)
#define EEPROM_DEFAULT_PATH                     "/etc/wl/RT3050_AP_1T1R_V1_0.bin"
#elif defined(CONFIG_RALINK_RT3051AP_1T2R) || defined(CONFIG_RALINK_RT3051STA_1T2R)
#define EEPROM_DEFAULT_PATH                     "/etc/wl/RT3051_AP_1T2R_V1_0.bin"
#elif defined(CONFIG_RALINK_RT3052AP_2T2R) || defined(CONFIG_RALINK_RT3052STA_2T2R)
#define EEPROM_DEFAULT_PATH                     "/etc/wl/RT3052_AP_2T2R_V1_1.bin"
#elif defined(CONFIG_RT2860V2_AP_2850) || defined(CONFIG_RT2860V2_STA_2850)
#define EEPROM_DEFAULT_PATH                     "/etc/wl/RT2880_RT2850_AP_2T3R_V1_6.bin"
#else // RFIC 2820
#define EEPROM_DEFAULT_PATH                     "/etc/wl/RT2880_RT2820_AP_2T3R_V1_6.bin"
#endif

#define EEPROM_SIZE				0x200

#ifdef CONFIG_RALINK_FLASH_API
#define NVRAM_OFFSET				0x30000
#if defined (CONFIG_RT2880_FLASH_32M) && defined (CONFIG_RALINK_RT3052_MP2)
#define RF_OFFSET				0x1FE0000
#else
#define RF_OFFSET				0x40000
#endif
#else //CONFIG_RALINK_FLASH_API
#if defined (CONFIG_RT2880_FLASH_32M) && defined (CONFIG_RALINK_RT3052_MP2)
#define MTD_NUM_FACTORY 5
#else
#define MTD_NUM_FACTORY 2
#endif
int ra_mtd_write(int num, loff_t to, size_t len, const u_char *buf);
int ra_mtd_read(int num, loff_t from, size_t len, u_char *buf);
#endif //CONFIG_RALINK_FLASH_API

static UCHAR init_flag = 0;
static PUCHAR nv_ee_start = 0;

static UCHAR EeBuffer[EEPROM_SIZE];



// IRQL = PASSIVE_LEVEL
VOID RaiseClock(
    IN	PRTMP_ADAPTER	pAd,
    IN  UINT32 *x)
{
    *x = *x | EESK;
    RTMP_IO_WRITE32(pAd, E2PROM_CSR, *x);
    RTMPusecDelay(1);				// Max frequency = 1MHz in Spec. definition 
}

// IRQL = PASSIVE_LEVEL
VOID LowerClock(
    IN	PRTMP_ADAPTER	pAd,
    IN  UINT32 *x)
{
    *x = *x & ~EESK;
    RTMP_IO_WRITE32(pAd, E2PROM_CSR, *x);
    RTMPusecDelay(1);
}

// IRQL = PASSIVE_LEVEL
USHORT ShiftInBits(
    IN	PRTMP_ADAPTER	pAd)
{
    UINT32       x,i;
	USHORT      data=0;

    RTMP_IO_READ32(pAd, E2PROM_CSR, &x);

    x &= ~( EEDO | EEDI);
    
    for(i=0; i<16; i++)
    {
        data = data << 1;
        RaiseClock(pAd, &x);

        RTMP_IO_READ32(pAd, E2PROM_CSR, &x);

        x &= ~(EEDI);
        if(x & EEDO)
            data |= 1;

        LowerClock(pAd, &x);
    }

    return data;
}

// IRQL = PASSIVE_LEVEL
VOID ShiftOutBits(
    IN	PRTMP_ADAPTER	pAd,
    IN  USHORT data,
    IN  USHORT count)
{
    UINT32       x,mask;

    mask = 0x01 << (count - 1);
    RTMP_IO_READ32(pAd, E2PROM_CSR, &x);

    x &= ~(EEDO | EEDI);

    do
    {
        x &= ~EEDI;
        if(data & mask)		x |= EEDI;

        RTMP_IO_WRITE32(pAd, E2PROM_CSR, x);

        RaiseClock(pAd, &x);
        LowerClock(pAd, &x);

        mask = mask >> 1;
    } while(mask);

    x &= ~EEDI;
    RTMP_IO_WRITE32(pAd, E2PROM_CSR, x);
}

// IRQL = PASSIVE_LEVEL
VOID EEpromCleanup(
    IN	PRTMP_ADAPTER	pAd)
{
    UINT32 x;

    RTMP_IO_READ32(pAd, E2PROM_CSR, &x);
	
    x &= ~(EECS | EEDI);
    RTMP_IO_WRITE32(pAd, E2PROM_CSR, x);

    RaiseClock(pAd, &x);
    LowerClock(pAd, &x);	
}

VOID EWEN(
	IN	PRTMP_ADAPTER	pAd)
{
    UINT32	x;

    // reset bits and set EECS
    RTMP_IO_READ32(pAd, E2PROM_CSR, &x);
    x &= ~(EEDI | EEDO | EESK);
    x |= EECS;
    RTMP_IO_WRITE32(pAd, E2PROM_CSR, x);

	// kick a pulse
	RaiseClock(pAd, &x);
	LowerClock(pAd, &x);

    // output the read_opcode and six pulse in that order    
    ShiftOutBits(pAd, EEPROM_EWEN_OPCODE, 5);
    ShiftOutBits(pAd, 0, 6);

    EEpromCleanup(pAd);    
}

VOID EWDS(
	IN	PRTMP_ADAPTER	pAd)
{
    UINT32	x;

    // reset bits and set EECS
    RTMP_IO_READ32(pAd, E2PROM_CSR, &x);
    x &= ~(EEDI | EEDO | EESK);
    x |= EECS;
    RTMP_IO_WRITE32(pAd, E2PROM_CSR, x);

	// kick a pulse
	RaiseClock(pAd, &x);
	LowerClock(pAd, &x);

    // output the read_opcode and six pulse in that order    
    ShiftOutBits(pAd, EEPROM_EWDS_OPCODE, 5);
    ShiftOutBits(pAd, 0, 6);

    EEpromCleanup(pAd);    
}

// IRQL = PASSIVE_LEVEL
USHORT _RTMP_EEPROM_READ16(
    IN	PRTMP_ADAPTER	pAd,
    IN  USHORT Offset)
{
    UINT32       x;
    USHORT      data;

    Offset /= 2;
    // reset bits and set EECS
    RTMP_IO_READ32(pAd, E2PROM_CSR, &x);
    x &= ~(EEDI | EEDO | EESK);
    x |= EECS;
    RTMP_IO_WRITE32(pAd, E2PROM_CSR, x);

	// kick a pulse
	RaiseClock(pAd, &x);
	LowerClock(pAd, &x);

    // output the read_opcode and register number in that order    
    ShiftOutBits(pAd, EEPROM_READ_OPCODE, 3);
    ShiftOutBits(pAd, Offset, pAd->EEPROMAddressNum);

    // Now read the data (16 bits) in from the selected EEPROM word
    data = ShiftInBits(pAd);

    EEpromCleanup(pAd);

    return data;
}	//ReadEEprom

VOID _RTMP_EEPROM_WRITE16(
    IN	PRTMP_ADAPTER	pAd,
    IN  USHORT Offset,
    IN  USHORT Data)
{
    UINT32	x;
	
	Offset /= 2;

	EWEN(pAd);

    // reset bits and set EECS
    RTMP_IO_READ32(pAd, E2PROM_CSR, &x);
    x &= ~(EEDI | EEDO | EESK);
    x |= EECS;
    RTMP_IO_WRITE32(pAd, E2PROM_CSR, x);

	// kick a pulse
	RaiseClock(pAd, &x);
	LowerClock(pAd, &x);

    // output the read_opcode ,register number and data in that order    
    ShiftOutBits(pAd, EEPROM_WRITE_OPCODE, 3);
    ShiftOutBits(pAd, Offset, pAd->EEPROMAddressNum);
	ShiftOutBits(pAd, Data, 16);		// 16-bit access

    // read DO status
    RTMP_IO_READ32(pAd, E2PROM_CSR, &x);

	EEpromCleanup(pAd);

	RTMPusecDelay(10000);	//delay for twp(MAX)=10ms
		
	EWDS(pAd);

    EEpromCleanup(pAd);
}



USHORT rt_ee_read(PRTMP_ADAPTER pAd, USHORT Offset)
{
	USHORT ret;
	
	if (!init_flag)
		return 0xFFFF;
		
	memcpy(&ret, nv_ee_start+ Offset, 2);
	return ret;
}


VOID rt_ee_write(PRTMP_ADAPTER pAd, USHORT Offset, USHORT Data)
{
	if (!init_flag)
		return;
	memcpy(nv_ee_start+ Offset, &Data, 2);
	//rt_nv_commit();
	//rt_cfg_commit();
#ifdef CONFIG_RALINK_FLASH_API
	FlashWrite(EeBuffer, RF_OFFSET, EEPROM_SIZE);
#else //CONFIG_RALINK_FLASH_API
	ra_mtd_write(MTD_NUM_FACTORY, 0, EEPROM_SIZE, EeBuffer);
#endif //CONFIG_RALINK_FLASH_API
}

NDIS_STATUS rt_ee_reset(PUCHAR start)
{
	PUCHAR					src;
	struct file				*srcf;
	INT 					retval, orgfsuid, orgfsgid;
   	mm_segment_t			orgfs;

   	src = EEPROM_DEFAULT_PATH;

	// Save uid and gid used for filesystem access.
	// Set user and group to 0 (root)	
	orgfsuid = current->fsuid;
	orgfsgid = current->fsgid;
	current->fsuid=current->fsgid = 0;
    orgfs = get_fs();
    set_fs(KERNEL_DS);

	if (src && *src)
	{
		srcf = filp_open(src, O_RDONLY, 0);
		if (IS_ERR(srcf)) 
		{
			DBGPRINT(RT_DEBUG_TRACE, ("--> Error %ld opening %s\n", -PTR_ERR(srcf),src));
		}
		else 
		{
			// The object must have a read method
			if (srcf->f_op && srcf->f_op->read)
			{
				memset(start, 0, EEPROM_SIZE);
				retval=srcf->f_op->read(srcf, start, EEPROM_SIZE, &srcf->f_pos);
				if (retval < 0)
				{
					DBGPRINT(RT_DEBUG_TRACE, ("--> Read %s error %d\n", src, -retval));
				}
				else
				{
					DBGPRINT(RT_DEBUG_TRACE, ("--> rt_ee_reset copy %s to eeprom buffer\n", src));
				}
			}
			else
			{
				DBGPRINT(RT_DEBUG_TRACE, ("--> %s does not have a write method\n", src));
			}
			
			retval=filp_close(srcf,NULL);
			
			if (retval)
			{
				DBGPRINT(RT_DEBUG_TRACE, ("--> Error %d closing %s\n", -retval, src));
			}
		}
	}

	set_fs(orgfs);
	current->fsuid = orgfsuid;
	current->fsgid = orgfsgid;

	return NDIS_STATUS_SUCCESS;
}



NDIS_STATUS rt_ee_init(PRTMP_ADAPTER pAd, PUCHAR start)
{
	init_flag = 1;
	

	nv_ee_start = start;
#if defined(CONFIG_RALINK_RT3052)
	if ((rt_ee_read(pAd, 0) != 0x3052) && (rt_ee_read(pAd, 0) != 0x3051) && (rt_ee_read(pAd, 0) != 0x3050))
#else
	if ((rt_ee_read(pAd, 0) != 0x2880))
#endif
	{
		if (rt_ee_reset(start) != NDIS_STATUS_SUCCESS)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("rt_ee_init: rt_ee_reset failed\n"));
			return NDIS_STATUS_FAILURE;
		}
		
		// Random number for the last bytes of MAC address
		{
			USHORT  Addr45;
			
			Addr45 = rt_ee_read(pAd, 0x08);
			Addr45 = Addr45 & 0xff;
			Addr45 = Addr45 | (RandomByte(pAd)&0xf8) << 8;
			
			rt_ee_write(pAd, 0x08, Addr45);
		}
		
#if defined(CONFIG_RALINK_RT3052)
		if ((rt_ee_read(pAd, 0) != 0x3052) && (rt_ee_read(pAd, 0) != 0x3051) && (rt_ee_read(pAd, 0) != 0x3050))
#else
		if ((rt_ee_read(pAd, 0) != 0x2880))
#endif
		{
			DBGPRINT(RT_DEBUG_ERROR, ("rt_ee_init: invalid eeprom\n"));
			return NDIS_STATUS_FAILURE;
		}
		
	}
	
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS rt_nv_init(PRTMP_ADAPTER pAd)
{
	DBGPRINT(RT_DEBUG_TRACE, ("--> rt_nv_init\n"));
	
#ifdef CONFIG_RALINK_FLASH_API
	FlashRead(EeBuffer, RF_OFFSET, EEPROM_SIZE);
#else //CONFIG_RALINK_FLASH_API
	ra_mtd_read(MTD_NUM_FACTORY, 0, EEPROM_SIZE, EeBuffer);
#endif //CONFIG_RALINK_FLASH_API
	return rt_ee_init(pAd, EeBuffer);
}


/* 0 -- Show ee buffer */
/* 1 -- force reset to default */
/* 2 -- Change ee settings */
int	Set_EECMD_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	USHORT i;
	
	i = simple_strtol(arg, 0, 10);
	switch(i)
	{
		case 0:
			{
				USHORT value, k;
				for (k = 0; k < EEPROM_SIZE; k+=2)
				{
					value = RTMP_EEPROM_READ16(pAd, k);
					printk("%4.4x ", value);
					if (((k+2) % 0x20) == 0)
						printk("\n");
				}
				
			}
			break;
		case 1:
			if (pAd->CSRBaseAddress == (PUCHAR)RT2860_CSR_ADDR)
			{
				printk("EEPROM reset to default......\n");
				printk("The last byte of MAC address will be re-generated...\n");
				if (rt_ee_reset(nv_ee_start) != NDIS_STATUS_SUCCESS)
				{
					DBGPRINT(RT_DEBUG_ERROR, ("Set_EECMD_Proc: rt_ee_reset failed\n"));
					return FALSE;
				}
			
				// Random number for the last bytes of MAC address
				{
					USHORT  Addr45;

					Addr45 = rt_ee_read(pAd, 0x08);
					Addr45 = Addr45 & 0xff;
					Addr45 = Addr45 | (RandomByte(pAd)&0xf8) << 8;
					printk("Addr45 = %4x\n", Addr45);
					rt_ee_write(pAd, 0x08, Addr45);
				}
			
#if defined(CONFIG_RALINK_RT3052)
				if ((rt_ee_read(pAd, 0) != 0x3052))
#else
				if ((rt_ee_read(pAd, 0) != 0x2880))
#endif
				{
					DBGPRINT(RT_DEBUG_ERROR, ("Set_EECMD_Proc: invalid eeprom\n"));
					return FALSE;
				}
			}
			break;
		case 2:
			{
				USHORT offset, value = 0;
				PUCHAR p;
				
				p = arg+2;
				offset = simple_strtol(p, 0, 10);
				p+=2;
				while (*p != '\0')
				{
					if (*p >= '0' && *p <= '9')
						value = (value << 4) + (*p - 0x30);
					else if (*p >= 'a' && *p <= 'f')
						value = (value << 4) + (*p - 0x57);
					else if (*p >= 'A' && *p <= 'F')
						value = (value << 4) + (*p - 0x37);
					p++;
				}
				RTMP_EEPROM_WRITE16(pAd, offset, value);
			}
			break;
		default:
			break;
	}

	return TRUE;
}

USHORT RTMP_EEPROM_READ16(
    IN	PRTMP_ADAPTER	pAd,
    IN  USHORT Offset)
{
	if (pAd->CSRBaseAddress == (PUCHAR)RT2860_CSR_ADDR)
	{
		return rt_ee_read(pAd, Offset);
	}
	else
	{
		return _RTMP_EEPROM_READ16(pAd, Offset);
	}
}

VOID RTMP_EEPROM_WRITE16(
    IN	PRTMP_ADAPTER	pAd,
    IN  USHORT Offset,
    IN  USHORT Data)
{
	if (pAd->CSRBaseAddress == (PUCHAR)RT2860_CSR_ADDR)
	{
		rt_ee_write(pAd, Offset, Data);
	}
	else
	{
		_RTMP_EEPROM_WRITE16(pAd, Offset, Data);
	}
}

VOID rt_ee_read_all(PRTMP_ADAPTER pAd, USHORT *Data)
{	
	if (!init_flag)
		return;
		
	memcpy(Data, nv_ee_start, EEPROM_SIZE);
}

VOID rt_ee_write_all(PRTMP_ADAPTER pAd, USHORT *Data)
{
	if (!init_flag)
		return;
	memcpy(nv_ee_start, Data, EEPROM_SIZE);
#ifdef CONFIG_RALINK_FLASH_API
	FlashWrite(EeBuffer, RF_OFFSET, EEPROM_SIZE);
#else //CONFIG_RALINK_FLASH_API
	ra_mtd_write(MTD_NUM_FACTORY, 0, EEPROM_SIZE, EeBuffer);
#endif //CONFIG_RALINK_FLASH_API
}
