#include "rt_config.h"
#include "linux/time.h"

#ifdef RALINK_ATE

#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
#define ATE_BBP_REG_NUM	168
UCHAR 	restore_BBP[ATE_BBP_REG_NUM]={0};
#endif // RT30xx //

// 802.11 MAC Header, Type:Data, Length:24bytes
UCHAR TemplateFrame[24] = {0x08,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0x00,0xAA,0xBB,0x12,0x34,0x56,0x00,0x11,0x22,0xAA,0xBB,0xCC,0x00,0x00};	
 
extern RTMP_RF_REGS RF2850RegTable[];
extern UCHAR NUM_OF_2850_CHNL;

extern FREQUENCY_ITEM FreqItems3020[];
extern UCHAR NUM_OF_3020_CHNL;



static CHAR CCKRateTable[] = {0, 1, 2, 3, 8, 9, 10, 11, -1}; /* CCK Mode. */
static CHAR OFDMRateTable[] = {0, 1, 2, 3, 4, 5, 6, 7, -1}; /* OFDM Mode. */
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
static CHAR HTMIXRateTable3T3R[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, -1}; /* HT Mix Mode for 3*3. */
#else
static CHAR HTMIXRateTable[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1}; /* HT Mix Mode. */
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //


static INT TxDmaBusy(
	IN PRTMP_ADAPTER pAd);

static INT RxDmaBusy(
	IN PRTMP_ADAPTER pAd);

static VOID RtmpDmaEnable(
	IN PRTMP_ADAPTER pAd,
	IN INT Enable);

static VOID BbpSoftReset(
	IN PRTMP_ADAPTER pAd);

static VOID RtmpRfIoWrite(
	IN PRTMP_ADAPTER pAd);

static INT ATESetUpFrame(
	IN PRTMP_ADAPTER pAd,
	IN UINT32 TxIdx);

static INT ATETxPwrHandler(
	IN PRTMP_ADAPTER pAd,
	IN char index);

static NDIS_STATUS ATESTART(
	IN PRTMP_ADAPTER pAd);

static NDIS_STATUS ATESTOP(
	IN PRTMP_ADAPTER pAd);

static NDIS_STATUS TXCARR(
	IN PRTMP_ADAPTER pAd);

static NDIS_STATUS TXCONT(
	IN PRTMP_ADAPTER pAd);

static NDIS_STATUS TXFRAME(
	IN PRTMP_ADAPTER pAd);

static NDIS_STATUS RXFRAME(
	IN PRTMP_ADAPTER pAd);

#ifdef RALINK_28xx_QA
static NDIS_STATUS TXSTOP(
	IN PRTMP_ADAPTER pAd);

static NDIS_STATUS RXSTOP(
	IN PRTMP_ADAPTER pAd);
#endif // RALINK_28xx_QA //

static NDIS_STATUS ATECmdHandler(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

static int CheckMCSValid(
	IN	PRTMP_ADAPTER	pAd, 
	IN UCHAR Mode,
	IN UCHAR Mcs);


#ifdef RTMP_MAC_PCI
static VOID ATEWriteTxWI(
	IN	PRTMP_ADAPTER	pAd,
	IN	PTXWI_STRUC 	pOutTxWI,	
	IN	BOOLEAN			FRAG,	
	IN	BOOLEAN			CFACK,
	IN	BOOLEAN			InsTimestamp,
	IN	BOOLEAN 		AMPDU,
	IN	BOOLEAN 		Ack,
	IN	BOOLEAN 		NSeq,		// HW new a sequence.
	IN	UCHAR			BASize,
	IN	UCHAR			WCID,
	IN	ULONG			Length,
	IN	UCHAR 			PID,
	IN	UCHAR			TID,
	IN	UCHAR			TxRate,
	IN	UCHAR			Txopmode,	
	IN	BOOLEAN			CfAck,	
	IN	HTTRANSMIT_SETTING	*pTransmit);
#endif // RTMP_MAC_PCI //


static VOID SetJapanFilter(
	IN	PRTMP_ADAPTER	pAd);


#ifdef RALINK_28xx_QA
/* command id with Cmd Type == 0x0008(for 28xx)/0x0005(for iNIC) */
#define RACFG_CMD_RF_WRITE_ALL		0x0000
#define RACFG_CMD_E2PROM_READ16		0x0001
#define RACFG_CMD_E2PROM_WRITE16	0x0002
#define RACFG_CMD_E2PROM_READ_ALL	0x0003
#define RACFG_CMD_E2PROM_WRITE_ALL	0x0004
#define RACFG_CMD_IO_READ			0x0005
#define RACFG_CMD_IO_WRITE			0x0006
#define RACFG_CMD_IO_READ_BULK		0x0007
#define RACFG_CMD_BBP_READ8			0x0008
#define RACFG_CMD_BBP_WRITE8		0x0009
#define RACFG_CMD_BBP_READ_ALL		0x000a
#define RACFG_CMD_GET_COUNTER		0x000b
#define RACFG_CMD_CLEAR_COUNTER		0x000c

#define RACFG_CMD_RSV1				0x000d
#define RACFG_CMD_RSV2				0x000e
#define RACFG_CMD_RSV3				0x000f

#define RACFG_CMD_TX_START			0x0010
#define RACFG_CMD_GET_TX_STATUS		0x0011
#define RACFG_CMD_TX_STOP			0x0012
#define RACFG_CMD_RX_START			0x0013
#define RACFG_CMD_RX_STOP			0x0014
#define RACFG_CMD_GET_NOISE_LEVEL	0x0015
#define RACFG_CMD_QUERY_BF_RSP		0x0016
#define RACFG_CMD_QUERY_IBF_TAG		0x0017
#define RACFG_CMD_QUERY_EBF_TAG		0x0018
#define RACFG_CMD_QUERY_IBF_PROFILE	0x0019
#define RACFG_CMD_QUERY_EBF_PROFILE	0x001a
#define RACFG_CMD_WRITE_IBF_TAG		0x001b
#define RACFG_CMD_WRITE_EBF_TAG		0x001c
#define RACFG_CMD_WRITE_IBF_PROFILE	0x001d
#define RACFG_CMD_WRITE_EBF_PROFILE	0x001e
#define RACFG_CMD_CALIBRATION_CAPTURE 0x0020

#define RACFG_CMD_ATE_START			0x0080
#define RACFG_CMD_ATE_STOP			0x0081

#define RACFG_CMD_ATE_START_TX_CARRIER		0x0100
#define RACFG_CMD_ATE_START_TX_CONT			0x0101
#define RACFG_CMD_ATE_START_TX_FRAME		0x0102
#define RACFG_CMD_ATE_SET_BW	            0x0103
#define RACFG_CMD_ATE_SET_TX_POWER0	        0x0104
#define RACFG_CMD_ATE_SET_TX_POWER1			0x0105
#define RACFG_CMD_ATE_SET_FREQ_OFFSET		0x0106
#define RACFG_CMD_ATE_GET_STATISTICS		0x0107
#define RACFG_CMD_ATE_RESET_COUNTER			0x0108
#define RACFG_CMD_ATE_SEL_TX_ANTENNA		0x0109
#define RACFG_CMD_ATE_SEL_RX_ANTENNA		0x010a
#define RACFG_CMD_ATE_SET_PREAMBLE			0x010b
#define RACFG_CMD_ATE_SET_CHANNEL			0x010c
#define RACFG_CMD_ATE_SET_ADDR1				0x010d
#define RACFG_CMD_ATE_SET_ADDR2				0x010e
#define RACFG_CMD_ATE_SET_ADDR3				0x010f
#define RACFG_CMD_ATE_SET_RATE				0x0110
#define RACFG_CMD_ATE_SET_TX_FRAME_LEN		0x0111
#define RACFG_CMD_ATE_SET_TX_FRAME_COUNT	0x0112
#define RACFG_CMD_ATE_START_RX_FRAME		0x0113
#define RACFG_CMD_ATE_E2PROM_READ_BULK	0x0114
#define RACFG_CMD_ATE_E2PROM_WRITE_BULK	0x0115
#define RACFG_CMD_ATE_IO_WRITE_BULK		0x0116
#define RACFG_CMD_ATE_BBP_READ_BULK		0x0117
#define RACFG_CMD_ATE_BBP_WRITE_BULK	0x0118
#define RACFG_CMD_ATE_RF_READ_BULK		0x0119
#define RACFG_CMD_ATE_RF_WRITE_BULK		0x011a
#define RACFG_CMD_ATE_SET_TX_POWER2	        0x011b

#define RACFG_CMD_ATE_ETH_EXT_SETTING	0x0200

static VOID memcpy_exl(PRTMP_ADAPTER pAd, UCHAR *dst, UCHAR *src, ULONG len);
static VOID memcpy_exs(PRTMP_ADAPTER pAd, UCHAR *dst, UCHAR *src, ULONG len);
static VOID RTMP_IO_READ_BULK(PRTMP_ADAPTER pAd, UCHAR *dst, UCHAR *src, UINT32 len);

#define	LEN_OF_ARG 16

static  INT ResponseToGUI(
	IN  struct ate_racfghdr *pRaCfg,
	IN	struct iwreq	*pwrq,
	IN  INT Length,
	IN  INT Status)
{
	(pRaCfg)->length = htons((Length));														
	(pRaCfg)->status = htons((Status));														
	(pwrq)->u.data.length = sizeof((pRaCfg)->magic_no) + sizeof((pRaCfg)->command_type)	
							+ sizeof((pRaCfg)->command_id) + sizeof((pRaCfg)->length)		
							+ sizeof((pRaCfg)->sequence) + ntohs((pRaCfg)->length);			
	ATEDBGPRINT(RT_DEBUG_TRACE, ("wrq->u.data.length = %d\n", (pwrq)->u.data.length));		
	if (copy_to_user((pwrq)->u.data.pointer, (UCHAR *)(pRaCfg), (pwrq)->u.data.length))	
	{																							
		ATEDBGPRINT(RT_DEBUG_ERROR, ("copy_to_user() fail in %s\n", __FUNCTION__));				
		return (-EFAULT);																		

	}																							
	else																						
	{																							
		ATEDBGPRINT(RT_DEBUG_TRACE, ("%s is done !\n", __FUNCTION__));							
	}

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_START(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_START\n"));
	
	/* Prepare feedback as soon as we can to avoid QA timeout. */
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);
#ifdef	CONFIG_RT2880_ATE_CMD_NEW
	Set_ATE_Proc(pAdapter, "ATESTART");
#else	
	Set_ATE_Proc(pAdapter, "APSTOP");
#endif
	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_STOP(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	INT32 ret;

	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_STOP\n"));

	/*
		Distinguish this command came from QA(via ate agent)
		or ate agent according to the existence of pid in payload.

		No need to prepare feedback if this cmd came directly from ate agent,
		not from QA.
	*/
	pRaCfg->length = ntohs(pRaCfg->length);

	if (pRaCfg->length == sizeof(pAdapter->ate.AtePid))
	{
		/*
			This command came from QA.
			Get the pid of ATE agent.
		*/
		memcpy((UCHAR *)&pAdapter->ate.AtePid,
						(&pRaCfg->data[0]) - 2/* == sizeof(pRaCfg->status) */,
						sizeof(pAdapter->ate.AtePid));					

		/* Prepare feedback as soon as we can to avoid QA timeout. */
		ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

		/*
			Kill ATE agent when leaving ATE mode.

			We must kill ATE agent first before setting ATESTOP,
			or Microsoft will report sth. wrong. 
		*/
#ifdef LINUX
		ret = KILL_THREAD_PID(pAdapter->ate.AtePid, SIGTERM, 1);

		if (ret)
		{
			ATEDBGPRINT(RT_DEBUG_ERROR, ("%s: unable to kill ate thread\n", pAdapter->net_dev->name));
		}
#endif // LINUX //
	}


	/* AP/STA might have in ATE_STOP mode due to cmd from QA. */
	if (ATE_ON(pAdapter))
	{
		/* Someone has killed ate agent while QA GUI is still open. */
#ifdef	CONFIG_RT2880_ATE_CMD_NEW
		Set_ATE_Proc(pAdapter, "ATESTOP");
#else
		Set_ATE_Proc(pAdapter, "APSTART");
#endif
		ATEDBGPRINT(RT_DEBUG_TRACE, ("RACFG_CMD_AP_START is done !\n"));
	}

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_RF_WRITE_ALL(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UINT32 R1, R2, R3, R4;
	USHORT channel;
	
	memcpy(&R1, pRaCfg->data-2, 4);
	memcpy(&R2, pRaCfg->data+2, 4);
	memcpy(&R3, pRaCfg->data+6, 4);
	memcpy(&R4, pRaCfg->data+10, 4);
	memcpy(&channel, pRaCfg->data+14, 2);		
	
	pAdapter->LatchRfRegs.R1 = ntohl(R1);
	pAdapter->LatchRfRegs.R2 = ntohl(R2);
	pAdapter->LatchRfRegs.R3 = ntohl(R3);
	pAdapter->LatchRfRegs.R4 = ntohl(R4);
	pAdapter->LatchRfRegs.Channel = ntohs(channel);

	RTMP_RF_IO_WRITE32(pAdapter, (UINT32)pAdapter->LatchRfRegs.R1);
	RTMP_RF_IO_WRITE32(pAdapter, (UINT32)pAdapter->LatchRfRegs.R2);
	RTMP_RF_IO_WRITE32(pAdapter, (UINT32)pAdapter->LatchRfRegs.R3);
	RTMP_RF_IO_WRITE32(pAdapter, (UINT32)pAdapter->LatchRfRegs.R4);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return  NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_E2PROM_READ16(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UINT16	offset=0, value=0;
	USHORT  tmp=0;				

	offset = ntohs(pRaCfg->status);

	/* "tmp" is especially for some compilers... */
	RT28xx_EEPROM_READ16(pAdapter, offset, tmp);
	value = tmp;
	value = htons(value);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("EEPROM Read offset = 0x%04x, value = 0x%04x\n", offset, value));
	memcpy(pRaCfg->data, &value, 2);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+2, NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_E2PROM_WRITE16(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT	offset, value;
	
	offset = ntohs(pRaCfg->status);
	memcpy(&value, pRaCfg->data, 2);
	value = ntohs(value);
	RT28xx_EEPROM_WRITE16(pAdapter, offset, value);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_E2PROM_READ_ALL(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT buffer[EEPROM_SIZE/2];

	rt_ee_read_all(pAdapter,(USHORT *)buffer);
	memcpy_exs(pAdapter, pRaCfg->data, (UCHAR *)buffer, EEPROM_SIZE);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+EEPROM_SIZE, NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_E2PROM_WRITE_ALL(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT buffer[EEPROM_SIZE/2];

	NdisZeroMemory((UCHAR *)buffer, EEPROM_SIZE);
	memcpy_exs(pAdapter, (UCHAR *)buffer, (UCHAR *)&pRaCfg->status, EEPROM_SIZE);
	rt_ee_write_all(pAdapter,(USHORT *)buffer);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_IO_READ(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UINT32	offset;
	UINT32	value;
	
	memcpy(&offset, &pRaCfg->status, 4);
	offset = ntohl(offset);

	/*
		We do not need the base address.
		So just extract the offset out.
	*/
	if ((offset & 0xFFFF0000) == 0x10000000)
	{
		RTMP_SYS_IO_READ32(offset | 0xa0000000, &value);
	}
	else
	{
		offset &= 0x0000FFFF;
		RTMP_IO_READ32(pAdapter, offset, &value);
	}
	value = htonl(value);
	memcpy(pRaCfg->data, &value, 4);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+4, NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_IO_WRITE(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UINT32	offset, value;
					
	memcpy(&offset, pRaCfg->data-2, 4);
	memcpy(&value, pRaCfg->data+2, 4);

	offset = ntohl(offset);

	/*
		We do not need the base address.
		So just extract the offset out.
	*/
	offset &= 0x0000FFFF;
	value = ntohl(value);
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_IO_WRITE: offset = %x, value = %x\n", offset, value));
	RTMP_IO_WRITE32(pAdapter, offset, value);
	
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_IO_READ_BULK(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UINT32	offset;
	USHORT	len;
	
	memcpy(&offset, &pRaCfg->status, 4);
	offset = ntohl(offset);

	/*
		We do not need the base address.
		So just extract the offset out.
	*/
	offset &= 0x0000FFFF;
	memcpy(&len, pRaCfg->data+2, 2);
	len = ntohs(len);

	if (len > 371)
	{
		ATEDBGPRINT(RT_DEBUG_TRACE,("length requested is too large, make it smaller\n"));
		pRaCfg->length = htons(2);
		pRaCfg->status = htons(1);
		return -EFAULT;
	}

	RTMP_IO_READ_BULK(pAdapter, pRaCfg->data, (UCHAR *)offset, len*4);// unit in four bytes

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+(len*4), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_BBP_READ8(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT	offset;
	UCHAR	value;
	
	value = 0;
	offset = ntohs(pRaCfg->status);

	if (ATE_ON(pAdapter))
	{
		ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, offset, &value);
	}
	else
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, offset, &value);	
	}

	pRaCfg->data[0] = value;
	
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+1, NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_BBP_WRITE8(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT	offset;
	UCHAR	value;
	
	offset = ntohs(pRaCfg->status);
	memcpy(&value, pRaCfg->data, 1);

	if (ATE_ON(pAdapter))
	{
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, offset, value);
	}
	else
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, offset, value);
	}

	if ((offset == BBP_R1) || (offset == BBP_R3))
	{
		SyncTxRxConfig(pAdapter, offset, value);
	}
	
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_BBP_READ_ALL(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT bbp_reg_index;
	
	for (bbp_reg_index = 0; bbp_reg_index < MAX_BBP_ID+1; bbp_reg_index++)
	{
		pRaCfg->data[bbp_reg_index] = 0;
		
		if (ATE_ON(pAdapter))
		{
			ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, bbp_reg_index, &pRaCfg->data[bbp_reg_index]);
		}
		else
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, bbp_reg_index, &pRaCfg->data[bbp_reg_index]);
		}
	}
	
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+MAX_BBP_ID+1, NDIS_STATUS_SUCCESS);
	
	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_GET_NOISE_LEVEL(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UCHAR	channel;
	INT32   buffer[3][10];/* 3 : RxPath ; 10 : no. of per rssi samples */

	channel = (ntohs(pRaCfg->status) & 0x00FF);
	CalNoiseLevel(pAdapter, channel, buffer);
	memcpy_exl(pAdapter, (UCHAR *)pRaCfg->data, (UCHAR *)&(buffer[0][0]), (sizeof(INT32)*3*10));

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+(sizeof(INT32)*3*10), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
static  INT DO_RACFG_CMD_QUERY_BF_RSP(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN struct ate_racfghdr *pRaCfg
)
{
	USHORT value, value0, value1, value2, soundingRespSize;
	int i;
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_QUERY_BF_RSP\n"));

	if ((pAdapter->ate.sounding == 2) && ((jiffies - pAdapter->ate.sounding_jiffies) < TIME_ONE_SECOND))
	{
		//printk("Sending feedback: got sounding response\n");
		value = htons(1);
		value0 = htons(pAdapter->ate.soundingSNR[0]);
		value1 = htons(pAdapter->ate.soundingSNR[1]);
		value2 = htons(pAdapter->ate.soundingSNR[2]);
		soundingRespSize = 0;	// 0=>sounding response is not returned
		//soundingRespSize = pAdapter->ate.soundingRespSize;  // entire sounding response is returned
	}
	else
	{
		//printk("Sending feedback: did not get sounding response\n");
		value = 0;
		value0 = value1 = value2 = 0;
		soundingRespSize = 0;
	}

	// prepare feedback
	pRaCfg->length = htons(4+6+2+soundingRespSize);
	pRaCfg->status = htons(0);
	memcpy(pRaCfg->data, &value, 2);
	memcpy(pRaCfg->data+2, &value0, 2);
	memcpy(pRaCfg->data+4, &value1, 2);
	memcpy(pRaCfg->data+6, &value2, 2);
	value2 = htons(soundingRespSize);
	memcpy(pRaCfg->data+8, &value2, 2);
	for (i=0; i<soundingRespSize && i<sizeof(pAdapter->ate.soundingResp) && i<sizeof(pRaCfg->data)-10; i++)
		pRaCfg->data[10+i] = pAdapter->ate.soundingResp[i];

	ResponseToGUI(pRaCfg, wrq, 4+6+2+soundingRespSize, NDIS_STATUS_SUCCESS);
	return NDIS_STATUS_SUCCESS;


#if 0
	USHORT value;
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_QUERY_BF_RSP\n"));

	if ((pAdapter->ate.sounding == 2) && ((jiffies - pAdapter->ate.sounding_jiffies) < TIME_ONE_SECOND))
	{
		printk("Sending feedback: got sounding response\n");
		value = htons(1);
	}
	else
	{
		printk("Sending feedback: did not get sounding response\n");
		value = 0;
	}

	// prepare feedback
	pRaCfg->length = htons(4);
	pRaCfg->status = htons(0);
	memcpy(pRaCfg->data, &value, 2);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+2, NDIS_STATUS_SUCCESS);
	return NDIS_STATUS_SUCCESS;
#endif
}

#define DO_RACFG_CMD_QUERY_EBF_TAG DO_RACFG_CMD_QUERY_IBF_TAG

static  INT DO_RACFG_CMD_QUERY_IBF_TAG(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN struct ate_racfghdr *pRaCfg
)
{
			USHORT profileNum;
			int byteIndex, di;
			UCHAR byteVal;
			BOOLEAN eProfile = ntohs(pRaCfg->command_id)==RACFG_CMD_QUERY_EBF_TAG;
			

			profileNum = ntohs(pRaCfg->status);

			ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_QUERY_BF_TAG %d\n", profileNum));

			// Select Explicit/Implicit profile
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R179, eProfile? 0x04: 0x00);

			pRaCfg->length = htons(2+18);
			pRaCfg->status = htons(0);

			// Read the tagfield and write to data buffer
			di = 0;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R181, 0x80);
			for (byteIndex=0; byteIndex<18; byteIndex++) {
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R180, (profileNum<<5) | byteIndex);
				RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, BBP_R182, &byteVal);
				pRaCfg->data[di++] = byteVal;
			}
	ResponseToGUI(pRaCfg, wrq, 2+18, NDIS_STATUS_SUCCESS);
	return NDIS_STATUS_SUCCESS;
}

#define DO_RACFG_CMD_QUERY_EBF_PROFILE DO_RACFG_CMD_QUERY_IBF_PROFILE

static  INT DO_RACFG_CMD_QUERY_IBF_PROFILE(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN struct ate_racfghdr *pRaCfg
)
{
			USHORT profileNum, startCarrier;
			USHORT bytesPerRow, numCarriers;
			int byteIndex, carrierIndex, di;
			UCHAR byteVal;
			BOOLEAN eProfile = ntohs(pRaCfg->command_id)==RACFG_CMD_QUERY_EBF_PROFILE;			
			struct timeval tval1, tval2;

			profileNum = ntohs(pRaCfg->status);
			memcpy(&startCarrier, pRaCfg->data, 2);
			startCarrier = ntohs(startCarrier);
			memcpy(&numCarriers, pRaCfg->data+2, 2);
			numCarriers = ntohs(numCarriers);

			// Older version have no numCarriers field so length is 4. If so, numCarriers=64 
			if (ntohs(pRaCfg->length)==4)
				numCarriers = 64;

			if (startCarrier+numCarriers > 128)
				numCarriers = 128 - startCarrier;

			ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_QUERY_BF_PROFILE\n"));

			do_gettimeofday(&tval1);
			bytesPerRow = (eProfile? 18: 14);

			// Select Explicit/Implicit profile
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R179, eProfile? 0x04: 0x00);

			pRaCfg->length = htons(2+bytesPerRow*numCarriers);
			pRaCfg->status = htons(0);

			// Read the data for each carrier and write to data buffer
			di = 0;
			for (carrierIndex=startCarrier; carrierIndex<startCarrier+numCarriers; carrierIndex++) {
				// Read a row of data
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R181, carrierIndex);
			
				for (byteIndex=0; byteIndex<bytesPerRow; byteIndex++) {
					RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R180, (profileNum<<5) | byteIndex);
					RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, BBP_R182, &byteVal);
					pRaCfg->data[di++] = byteVal;
				}
			}

			do_gettimeofday(&tval2);
            ATEDBGPRINT(RT_DEBUG_WARN, ("BF Read elasped = %ld usec\n", tval2.tv_usec - tval1.tv_usec));


	ResponseToGUI(pRaCfg, wrq, 2+bytesPerRow*numCarriers, NDIS_STATUS_SUCCESS);
	return NDIS_STATUS_SUCCESS;
}

#define DO_RACFG_CMD_WRITE_EBF_TAG DO_RACFG_CMD_WRITE_IBF_TAG
static  INT DO_RACFG_CMD_WRITE_IBF_TAG(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN struct ate_racfghdr *pRaCfg
)
{
			USHORT profileNum;
			int byteIndex, di;
			BOOLEAN eProfile = ntohs(pRaCfg->command_id)==RACFG_CMD_WRITE_EBF_TAG;

			profileNum = ntohs(pRaCfg->status);

			ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_WRITE_EBF_TAG %d\n", profileNum));

			// Select Explicit/Implicit profile
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R179, eProfile? 0x04: 0x00);

			pRaCfg->length = htons(2);
			pRaCfg->status = htons(0);

			// Read the tagfield and write to data buffer
			di = 0;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R181, 0x80);
			for (byteIndex=0; byteIndex<18; byteIndex++) {
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R180, (profileNum<<5) | byteIndex);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R182, pRaCfg->data[2+di]);
				di++;
			}
	ResponseToGUI(pRaCfg, wrq, 2, NDIS_STATUS_SUCCESS);
	return NDIS_STATUS_SUCCESS;
}

#define DO_RACFG_CMD_WRITE_EBF_PROFILE DO_RACFG_CMD_WRITE_IBF_PROFILE
static  INT DO_RACFG_CMD_WRITE_IBF_PROFILE(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN struct ate_racfghdr *pRaCfg
)
{
			USHORT profileNum, startCarrier;
			USHORT bytesPerRow, numCarriers;
			int byteIndex, carrierIndex, di;
			BOOLEAN eProfile = ntohs(pRaCfg->command_id)==RACFG_CMD_WRITE_EBF_PROFILE;
			struct timeval tval1, tval2;
			
			bytesPerRow = (eProfile? 18: 14);

			profileNum = ntohs(pRaCfg->status);
			memcpy(&startCarrier, pRaCfg->data, 2);
			startCarrier = ntohs(startCarrier);

			// Calculate number of carriers from length of data
			numCarriers = (ntohs(pRaCfg->length)-4)/bytesPerRow;

			if (startCarrier+numCarriers > 128)
				numCarriers = 128 - startCarrier;

			ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_WRITE_BF_PROFILE-%d %d\n", startCarrier, numCarriers));

			do_gettimeofday(&tval1);

			// Select Explicit/Implicit profile
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R179, eProfile? 0x04: 0x00);

			pRaCfg->length = htons(2);
			pRaCfg->status = htons(0);

			// Write the data for each carrier from data buffer
			di = 0;
			for (carrierIndex=startCarrier; carrierIndex<startCarrier+numCarriers; carrierIndex++) {
				// Write a row of data
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R181, carrierIndex);
			
				for (byteIndex=0; byteIndex<bytesPerRow; byteIndex++) {
					RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R180, (profileNum<<5) | byteIndex);
					RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R182, pRaCfg->data[2+di]);
					di++;
				}
			}

			do_gettimeofday(&tval2);
            ATEDBGPRINT(RT_DEBUG_WARN, ("BF Write elasped = %ld usec\n", tval2.tv_usec - tval1.tv_usec));

	ResponseToGUI(pRaCfg, wrq, 2, NDIS_STATUS_SUCCESS);
	return NDIS_STATUS_SUCCESS;
}

static  INT DO_RACFG_CALIBRATION_CAPTURE(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN struct ate_racfghdr *pRaCfg
)
{
	UINT32 capCtrl, macCtrl;
	UINT8 r25Value, r65Value, r66Value;
	UINT8 rfValue0;
	struct timeval tval0, tval1, tval2;
	int i=0;

	// Read CAP CTRL, MAC CTRL and RF R25
	RTMP_IO_READ32(pAdapter, PBF_CAP_CTRL, &capCtrl);
	RTMP_IO_READ32(pAdapter, MAC_SYS_CTRL, &macCtrl);
	ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, BBP_R25, &r25Value);

	// Enable Capture and RX
	RTMP_IO_WRITE32(pAdapter, PBF_CAP_CTRL, capCtrl | 0x40000000);
	RTMP_IO_WRITE32(pAdapter, MAC_SYS_CTRL, macCtrl | 0x08);
	
	do_gettimeofday(&tval0);

	// Set BBP R65, R66
	ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, BBP_R65, &r65Value);
	//ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R65, 0x2C);
	// Set BBP R66 - VGA
	ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, BBP_R66, &r66Value);
	//ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R66, pAdapter->ate.Channel<=14? 0x10: 0x18);

	// Enable RF Loopback
	ATE_RF_IO_READ8_BY_REG_ID(pAdapter, RF_R36, &rfValue0);
	//ATE_RF_IO_WRITE8_BY_REG_ID(pAdapter, RF_R36, rfValue0 | 0x70);

	if (pAdapter->CommonCfg.DebugFlags & DBF_UNUSED0800) {	 //gaa
		// Start capture before RF loopback
		RTMP_IO_WRITE32(pAdapter, PBF_CAP_CTRL, capCtrl | 0x20000000);
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R25, r25Value | 0x50);
	}
	else {
		// Start RF Loopback calibration then delay
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R25, r25Value | 0x50);

		// Debug flag controls delay 0x400=>100usec, 0x800=>0 usec, else 10 usec
		RTMPusecDelay(10);
		if (pAdapter->CommonCfg.DebugFlags & DBF_UNUSED0400)	 //gaa
			RTMPusecDelay(100);

		// Start Capture and poll for completion
		RTMP_IO_WRITE32(pAdapter, PBF_CAP_CTRL, capCtrl | 0x20000000);
	}

	do_gettimeofday(&tval2);

	do {
		RTMPusecDelay(75);
		RTMP_IO_READ32(pAdapter, PBF_CAP_CTRL, &capCtrl);
	} while ((capCtrl & 0x40000000)!=0 && ++i<12);
	
	// Restore RF_R36, BBP R65 and R66
	ATE_RF_IO_WRITE8_BY_REG_ID(pAdapter, RF_R36, rfValue0);
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R65, r65Value);
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, BBP_R66, r66Value);

	do_gettimeofday(&tval1);

    ATEDBGPRINT(RT_DEBUG_WARN, ("Cal Cap c=%x, t1=%ld t2=%ld\n", capCtrl,
				tval1.tv_usec - tval0.tv_usec, tval2.tv_usec - tval0.tv_usec));

	ResponseToGUI(pRaCfg, wrq, 2, NDIS_STATUS_SUCCESS);
	return NDIS_STATUS_SUCCESS;
}

#endif

static  INT DO_RACFG_CMD_GET_COUNTER(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	memcpy_exl(pAdapter, &pRaCfg->data[0], (UCHAR *)&pAdapter->ate.U2M, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[4], (UCHAR *)&pAdapter->ate.OtherData, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[8], (UCHAR *)&pAdapter->ate.Beacon, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[12], (UCHAR *)&pAdapter->ate.OtherCount, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[16], (UCHAR *)&pAdapter->ate.TxAc0, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[20], (UCHAR *)&pAdapter->ate.TxAc1, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[24], (UCHAR *)&pAdapter->ate.TxAc2, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[28], (UCHAR *)&pAdapter->ate.TxAc3, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[32], (UCHAR *)&pAdapter->ate.TxHCCA, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[36], (UCHAR *)&pAdapter->ate.TxMgmt, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[40], (UCHAR *)&pAdapter->ate.RSSI0, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[44], (UCHAR *)&pAdapter->ate.RSSI1, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[48], (UCHAR *)&pAdapter->ate.RSSI2, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[52], (UCHAR *)&pAdapter->ate.SNR0, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[56], (UCHAR *)&pAdapter->ate.SNR1, 4);
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	if (IS_RT2883(pAdapter) || IS_RT3883(pAdapter) )
	{
		memcpy_exl(pAdapter, &pRaCfg->data[60], (UCHAR *)&pAdapter->ate.SNR2, 4);
		memcpy_exl(pAdapter, &pRaCfg->data[64], (UCHAR *)&pAdapter->ate.LastRxRate, 4);
		memcpy_exl(pAdapter, &pRaCfg->data[68], (UCHAR *)&pAdapter->MacTab.Content[BSSID_WCID].HTPhyMode.word, 4);
		memcpy_exl(pAdapter, &pRaCfg->data[72], (UCHAR *)&pAdapter->ate.BF_SNR[0], 4);
		memcpy_exl(pAdapter, &pRaCfg->data[76], (UCHAR *)&pAdapter->ate.BF_SNR[1], 4);
		memcpy_exl(pAdapter, &pRaCfg->data[80], (UCHAR *)&pAdapter->ate.BF_SNR[2], 4);
		ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+84, NDIS_STATUS_SUCCESS);
	}
	else
	{
		ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+60, NDIS_STATUS_SUCCESS);
	}
#else
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+60, NDIS_STATUS_SUCCESS);
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_CLEAR_COUNTER(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	pAdapter->ate.U2M = 0;
	pAdapter->ate.OtherData = 0;
	pAdapter->ate.Beacon = 0;
	pAdapter->ate.OtherCount = 0;
	pAdapter->ate.TxAc0 = 0;
	pAdapter->ate.TxAc1 = 0;
	pAdapter->ate.TxAc2 = 0;
	pAdapter->ate.TxAc3 = 0;
	pAdapter->ate.TxHCCA = 0;
	pAdapter->ate.TxMgmt = 0;
	pAdapter->ate.TxDoneCount = 0;
	
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_TX_START(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT *p;
	USHORT	err = 1;
	UCHAR   Bbp22Value = 0, Bbp24Value = 0;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	HEADER_802_11 *pHeader_802_11;
	PHT_CONTROL pHTC;
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

	if ((pAdapter->ate.TxStatus != 0) && (pAdapter->ate.Mode & ATE_TXFRAME))
	{
		ATEDBGPRINT(RT_DEBUG_TRACE,("Ate Tx is already running, to run next Tx, you must stop it first\n"));
		err = 2; 
		goto TX_START_ERROR;
	}
	else if ((pAdapter->ate.TxStatus != 0) && !(pAdapter->ate.Mode & ATE_TXFRAME))
	{
		int i = 0;

		while ((i++ < 10) && (pAdapter->ate.TxStatus != 0))
		{
			RTMPusecDelay(5000);
		}

		/* force it to stop */
		pAdapter->ate.TxStatus = 0;
		pAdapter->ate.TxDoneCount = 0;
		pAdapter->ate.bQATxStart = FALSE;
	}

	/*
		If pRaCfg->length == 0, this "RACFG_CMD_TX_START"
		is for Carrier test or Carrier Suppression.
	*/
	if (ntohs(pRaCfg->length) != 0)
	{
		/* get frame info */

		NdisMoveMemory(&pAdapter->ate.TxWI, pRaCfg->data + 2, 16);						
#ifdef RT_BIG_ENDIAN
		RTMPWIEndianChange((PUCHAR)&pAdapter->ate.TxWI, TYPE_TXWI);
#endif // RT_BIG_ENDIAN //

		NdisMoveMemory(&pAdapter->ate.TxCount, pRaCfg->data + 18, 4);
		pAdapter->ate.TxCount = ntohl(pAdapter->ate.TxCount);

		p = (USHORT *)(&pRaCfg->data[22]);

		/* always use QID_AC_BE */
		pAdapter->ate.QID = 0;

		p = (USHORT *)(&pRaCfg->data[24]);
		pAdapter->ate.HLen = ntohs(*p);

		if (pAdapter->ate.HLen > 32)
		{
			ATEDBGPRINT(RT_DEBUG_ERROR,("pAdapter->ate.HLen > 32\n"));
			err = 3;
			goto TX_START_ERROR;
		}

		NdisMoveMemory(&pAdapter->ate.Header, pRaCfg->data + 26, pAdapter->ate.HLen);
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
		if (IS_RT2883(pAdapter) || IS_RT3883(pAdapter) )
		{			
			// check Sounding frame
			pAdapter->ate.sounding = 0;
			pHeader_802_11 = (HEADER_802_11	*) pAdapter->ate.Header;
			pHTC = (PHT_CONTROL) &pHeader_802_11->Octet[2];
			if ((pAdapter->ate.TxWI.Sounding== 1)
					|| ((pHeader_802_11->FC.SubType & 0x08) && (pHeader_802_11->FC.Order == 1) && (pHTC->NDPAnnouce)) )
			{
				printk("Sending sounding frame\n");
				pAdapter->ate.sounding = 1;
				pAdapter->ate.sounding_jiffies = jiffies; // TIME_ONE_SECOND timeout
			}
		}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

		pAdapter->ate.PLen = ntohs(pRaCfg->length) - (pAdapter->ate.HLen + 28);

		if (pAdapter->ate.PLen > 32)
		{
			ATEDBGPRINT(RT_DEBUG_ERROR,("pAdapter->ate.PLen > 32\n"));
			err = 4;
			goto TX_START_ERROR;
		}

		NdisMoveMemory(&pAdapter->ate.Pattern, pRaCfg->data + 26 + pAdapter->ate.HLen, pAdapter->ate.PLen);
		pAdapter->ate.DLen = pAdapter->ate.TxWI.MPDUtotalByteCount - pAdapter->ate.HLen;

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
		if (IS_RT2883(pAdapter) || IS_RT3883(pAdapter) )
		{	
			if ((pHeader_802_11->FC.SubType & 0x08) && (pHeader_802_11->FC.Order == 1))
				pAdapter->ate.DLen += 2; // compensation for header padding
		}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

	}

	ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, BBP_R22, &Bbp22Value);

	switch (Bbp22Value)
	{
		case BBP22_TXFRAME:
			{
				if (pAdapter->ate.TxCount == 0)
				{
#ifdef RTMP_MAC_PCI
					pAdapter->ate.TxCount = 0xFFFFFFFF;
#endif // RTMP_MAC_PCI //
				}
				ATEDBGPRINT(RT_DEBUG_TRACE,("START TXFRAME\n"));
				pAdapter->ate.bQATxStart = TRUE;
				Set_ATE_Proc(pAdapter, "TXFRAME");
			}
			break;

		case BBP22_TXCONT_OR_CARRSUPP:
			{
				ATEDBGPRINT(RT_DEBUG_TRACE,("BBP22_TXCONT_OR_CARRSUPP\n"));
				ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, BBP_R24, &Bbp24Value);

				switch (Bbp24Value)
				{
					case BBP24_TXCONT:
						{
							ATEDBGPRINT(RT_DEBUG_TRACE,("START TXCONT\n"));
							pAdapter->ate.bQATxStart = TRUE;
							Set_ATE_Proc(pAdapter, "TXCONT");
						}
						break;

					case BBP24_CARRSUPP:
						{
							ATEDBGPRINT(RT_DEBUG_TRACE,("START TXCARRSUPP\n"));
							pAdapter->ate.bQATxStart = TRUE;
							pAdapter->ate.Mode |= ATE_TXCARRSUPP;
						}
						break;

					default:
						{
							ATEDBGPRINT(RT_DEBUG_ERROR,("Unknown TX subtype !"));
						}
						break;
				}
			}
			break;	

		case BBP22_TXCARR:
			{
				ATEDBGPRINT(RT_DEBUG_TRACE,("START TXCARR\n"));
				pAdapter->ate.bQATxStart = TRUE;
				Set_ATE_Proc(pAdapter, "TXCARR");
			}
			break;							

		default:
			{
				ATEDBGPRINT(RT_DEBUG_ERROR,("Unknown Start TX subtype !"));
			}
			break;
	}

	if (pAdapter->ate.bQATxStart == TRUE)
	{
		ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);
		return NDIS_STATUS_SUCCESS;
	}

TX_START_ERROR:
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), err);

	return err;
}


static  INT DO_RACFG_CMD_GET_TX_STATUS(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UINT32 count=0;
	
	count = htonl(pAdapter->ate.TxDoneCount);
	NdisMoveMemory(pRaCfg->data, &count, 4);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+4, NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_TX_STOP(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_TX_STOP\n"));

	Set_ATE_Proc(pAdapter, "TXSTOP");

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_RX_START(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_RX_START\n"));

	pAdapter->ate.bQARxStart = TRUE;
	Set_ATE_Proc(pAdapter, "RXFRAME");

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}	


static  INT DO_RACFG_CMD_RX_STOP(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_RX_STOP\n"));

	Set_ATE_Proc(pAdapter, "RXSTOP");

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_START_TX_CARRIER(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_START_TX_CARRIER\n"));

	Set_ATE_Proc(pAdapter, "TXCARR");

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_START_TX_CONT(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_START_TX_CONT\n"));

	Set_ATE_Proc(pAdapter, "TXCONT");

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_START_TX_FRAME(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_START_TX_FRAME\n"));

	Set_ATE_Proc(pAdapter, "TXFRAME");

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}	


static  INT DO_RACFG_CMD_ATE_SET_BW(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_BW\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);

	Set_ATE_TX_BW_Proc(pAdapter, str);
	
	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_TX_POWER0(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);

	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_TX_POWER0\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_TX_POWER0_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_TX_POWER1(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_TX_POWER1\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_TX_POWER1_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}

#if defined(RT2883) || defined(RT3883)
static  INT DO_RACFG_CMD_ATE_SET_TX_POWER2(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_TX_POWER2\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_TX_POWER2_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}
#endif // RT2883 || RT3883 //


static  INT DO_RACFG_CMD_ATE_SET_FREQ_OFFSET(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);

	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_FREQ_OFFSET\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_TX_FREQOFFSET_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_GET_STATISTICS(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_GET_STATISTICS\n"));

	memcpy_exl(pAdapter, &pRaCfg->data[0], (UCHAR *)&pAdapter->ate.TxDoneCount, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[4], (UCHAR *)&pAdapter->WlanCounters.RetryCount.u.LowPart, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[8], (UCHAR *)&pAdapter->WlanCounters.FailedCount.u.LowPart, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[12], (UCHAR *)&pAdapter->WlanCounters.RTSSuccessCount.u.LowPart, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[16], (UCHAR *)&pAdapter->WlanCounters.RTSFailureCount.u.LowPart, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[20], (UCHAR *)&pAdapter->WlanCounters.ReceivedFragmentCount.QuadPart, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[24], (UCHAR *)&pAdapter->WlanCounters.FCSErrorCount.u.LowPart, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[28], (UCHAR *)&pAdapter->Counters8023.RxNoBuffer, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[32], (UCHAR *)&pAdapter->WlanCounters.FrameDuplicateCount.u.LowPart, 4);
	memcpy_exl(pAdapter, &pRaCfg->data[36], (UCHAR *)&pAdapter->RalinkCounters.OneSecFalseCCACnt, 4);
	
	if (pAdapter->ate.RxAntennaSel == 0)
	{
		INT32 RSSI0 = 0;
		INT32 RSSI1 = 0;
		INT32 RSSI2 = 0;

		RSSI0 = (INT32)(pAdapter->ate.LastRssi0 - pAdapter->BbpRssiToDbmDelta);
		RSSI1 = (INT32)(pAdapter->ate.LastRssi1 - pAdapter->BbpRssiToDbmDelta);
		RSSI2 = (INT32)(pAdapter->ate.LastRssi2 - pAdapter->BbpRssiToDbmDelta);
		memcpy_exl(pAdapter, &pRaCfg->data[40], (UCHAR *)&RSSI0, 4);
		memcpy_exl(pAdapter, &pRaCfg->data[44], (UCHAR *)&RSSI1, 4);
		memcpy_exl(pAdapter, &pRaCfg->data[48], (UCHAR *)&RSSI2, 4);
		ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+52, NDIS_STATUS_SUCCESS);
	}
	else
	{
		INT32 RSSI0 = 0;
	
		RSSI0 = (INT32)(pAdapter->ate.LastRssi0 - pAdapter->BbpRssiToDbmDelta);
		memcpy_exl(pAdapter, &pRaCfg->data[40], (UCHAR *)&RSSI0, 4);
		ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+44, NDIS_STATUS_SUCCESS);
	}

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_RESET_COUNTER(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 1;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);

	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_RESET_COUNTER\n"));				

	sprintf((char *)str, "%d", value);
	Set_ResetStatCounter_Proc(pAdapter, str);

	pAdapter->ate.TxDoneCount = 0;

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SEL_TX_ANTENNA(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)	
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SEL_TX_ANTENNA\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_TX_Antenna_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SEL_RX_ANTENNA(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SEL_RX_ANTENNA\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_RX_Antenna_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_PREAMBLE(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_PREAMBLE\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_TX_MODE_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_CHANNEL(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_CHANNEL\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_CHANNEL_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_ADDR1(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_ADDR1\n"));

	/*
		Addr is an array of UCHAR,
		so no need to perform endian swap.
	*/
	memcpy(pAdapter->ate.Addr1, (PUCHAR)(pRaCfg->data - 2), MAC_ADDR_LEN);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_ADDR2(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_ADDR2\n"));

	/*
		Addr is an array of UCHAR,
		so no need to perform endian swap.
	*/
	memcpy(pAdapter->ate.Addr2, (PUCHAR)(pRaCfg->data - 2), MAC_ADDR_LEN);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_ADDR3(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_ADDR3\n"));

	/*
		Addr is an array of UCHAR,
		so no need to perform endian swap.
	*/
	memcpy(pAdapter->ate.Addr3, (PUCHAR)(pRaCfg->data - 2), MAC_ADDR_LEN);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_RATE(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_RATE\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_TX_MCS_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_TX_FRAME_LEN(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	SHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_TX_FRAME_LEN\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);
	sprintf((char *)str, "%d", value);
	Set_ATE_TX_LENGTH_Proc(pAdapter, str);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_SET_TX_FRAME_COUNT(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT    value = 0;
	STRING    str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_SET_TX_FRAME_COUNT\n"));				

	memcpy((PUCHAR)&value, (PUCHAR)&(pRaCfg->status), 2);
	value = ntohs(value);

#ifdef RTMP_MAC_PCI
	/* TX_FRAME_COUNT == 0 means tx infinitely */
	if (value == 0)
	{
		/* Use TxCount = 0xFFFFFFFF to approximate the infinity. */
		pAdapter->ate.TxCount = 0xFFFFFFFF;
		ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_COUNT_Proc (TxCount = %d)\n", pAdapter->ate.TxCount));
		ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_COUNT_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //

	}
	else
#endif // RTMP_MAC_PCI //
	{
		sprintf((char *)str, "%d", value);
		Set_ATE_TX_COUNT_Proc(pAdapter, str);
	}

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_START_RX_FRAME(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_RX_START\n"));

	Set_ATE_Proc(pAdapter, "RXFRAME");

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_E2PROM_READ_BULK(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT offset;
	USHORT len;
	USHORT buffer[EEPROM_SIZE/2];
	
	offset = ntohs(pRaCfg->status);
	memcpy(&len, pRaCfg->data, 2);
	len = ntohs(len);
	
	rt_ee_read_all(pAdapter, (USHORT *)buffer);

	if (offset + len <= EEPROM_SIZE)
		memcpy_exs(pAdapter, pRaCfg->data, (UCHAR *)buffer+offset, len);
	else
		ATEDBGPRINT(RT_DEBUG_ERROR, ("exceed EEPROM size\n"));

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+len, NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_E2PROM_WRITE_BULK(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT offset;
	USHORT len;
	USHORT buffer[EEPROM_SIZE/2];
	
	offset = ntohs(pRaCfg->status);
	memcpy(&len, pRaCfg->data, 2);
	len = ntohs(len);

	rt_ee_read_all(pAdapter,(USHORT *)buffer);
	memcpy_exs(pAdapter, (UCHAR *)buffer + offset, (UCHAR *)pRaCfg->data + 2, len);
	rt_ee_write_all(pAdapter,(USHORT *)buffer);

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_IO_WRITE_BULK(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UINT32 offset, i, value;
	USHORT len;
	
	memcpy(&offset, &pRaCfg->status, 4);
	offset = ntohl(offset);
	memcpy(&len, pRaCfg->data+2, 2);
	len = ntohs(len);
	
	for (i = 0; i < len; i += 4)
	{
		memcpy_exl(pAdapter, (UCHAR *)&value, pRaCfg->data+4+i, 4);
		ATEDBGPRINT(RT_DEBUG_TRACE,("Write %x %x\n", offset + i, value));
		RTMP_IO_WRITE32(pAdapter, ((offset+i) & (0xffff)), value);
	}

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_BBP_READ_BULK(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT offset;
	USHORT len;
	USHORT j;
	
	offset = ntohs(pRaCfg->status);
	memcpy(&len, pRaCfg->data, 2);
	len = ntohs(len);
	
	for (j = offset; j < (offset+len); j++)
	{
		pRaCfg->data[j - offset] = 0;
		
		if (pAdapter->ate.Mode == ATE_STOP)
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAdapter, j, &pRaCfg->data[j - offset]);
		}
		else
		{
			ATE_BBP_IO_READ8_BY_REG_ID(pAdapter, j, &pRaCfg->data[j - offset]);
		}
	}

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+len, NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_BBP_WRITE_BULK(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT offset;
	USHORT len;
	USHORT j;
	UCHAR *value;
	
	offset = ntohs(pRaCfg->status);
	memcpy(&len, pRaCfg->data, 2);
	len = ntohs(len);
					
	for (j = offset; j < (offset+len); j++)
	{
		value = pRaCfg->data + 2 + (j - offset);
		if (pAdapter->ate.Mode == ATE_STOP)
		{
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAdapter, j,  *value);
		}
		else
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAdapter, j,  *value);
		}
	}

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}

#ifdef CONFIG_RALINK_RT3883
static  INT DO_RACFG_CMD_ATE_ETH_EXT_SETTING(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	UINT32    value = 0;
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("RACFG_CMD_ATE_ETH_EXT_SETTING\n"));
	memcpy((PUCHAR)&value, pRaCfg->data-2, 4);
	value = ntohl(value);
	
	if (value == 0)
	{ // GPIO poll low
		RTMPRT3883ABandSel(36);
	}
	else
	{ // GPIO poll high
		RTMPRT3883ABandSel(1);
	}

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}
#endif // CONFIG_RALINK_RT3883 //

#ifdef RTMP_RF_RW_SUPPORT
static  INT DO_RACFG_CMD_ATE_RF_READ_BULK(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT offset;
	USHORT len;
	USHORT j;
	
	offset = ntohs(pRaCfg->status);
	memcpy(&len, pRaCfg->data, 2);
	len = ntohs(len);

	for (j = offset; j < (offset+len); j++)
	{
		pRaCfg->data[j - offset] = 0;
		ATE_RF_IO_READ8_BY_REG_ID(pAdapter, j,  &pRaCfg->data[j - offset]);
	}

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status)+len, NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}


static  INT DO_RACFG_CMD_ATE_RF_WRITE_BULK(
	IN	PRTMP_ADAPTER	pAdapter,
	IN	struct iwreq	*wrq,
	IN  struct ate_racfghdr *pRaCfg)
{
	USHORT offset;
	USHORT len;
	USHORT j;
	UCHAR *value;
	
	offset = ntohs(pRaCfg->status);
	memcpy(&len, pRaCfg->data, 2);
	len = ntohs(len);

	for (j = offset; j < (offset+len); j++)
	{
		value = pRaCfg->data + 2 + (j - offset);
		ATE_RF_IO_WRITE8_BY_REG_ID(pAdapter, j,  *value);
	}

	ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);

	return NDIS_STATUS_SUCCESS;
}
#endif // RTMP_RF_RW_SUPPORT //

#endif // RALINK_28xx_QA //


#ifdef RTMP_MAC_PCI
static INT TxDmaBusy(
	IN PRTMP_ADAPTER pAd)
{
	INT result;
	WPDMA_GLO_CFG_STRUC GloCfg;

	RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);	// disable DMA
	if (GloCfg.field.TxDMABusy)
		result = 1;
	else
		result = 0;

	return result;
}


static INT RxDmaBusy(
	IN PRTMP_ADAPTER pAd)
{
	INT result;
	WPDMA_GLO_CFG_STRUC GloCfg;

	RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);	// disable DMA
	if (GloCfg.field.RxDMABusy)
		result = 1;
	else
		result = 0;

	return result;
}


static VOID RtmpDmaEnable(
	IN PRTMP_ADAPTER pAd,
	IN INT Enable)
{
	BOOLEAN value;
	ULONG WaitCnt;
	WPDMA_GLO_CFG_STRUC GloCfg;
	
	value = Enable > 0 ? 1 : 0;

	// check DMA is in busy mode.
	WaitCnt = 0;

	while (TxDmaBusy(pAd) || RxDmaBusy(pAd))
	{
		RTMPusecDelay(10);
		if (WaitCnt++ > 100)
			break;
	}
	
	RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);	// disable DMA
	GloCfg.field.EnableTxDMA = value;
	GloCfg.field.EnableRxDMA = value;
	RTMP_IO_WRITE32(pAd, WPDMA_GLO_CFG, GloCfg.word);	// abort all TX rings
	RTMPusecDelay(5000);

	return;
}
#endif // RTMP_MAC_PCI //


static VOID BbpSoftReset(
	IN PRTMP_ADAPTER pAd)
{
	UCHAR BbpData = 0;
	// Soft reset, set BBP R21 bit0=1->0
	ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R21, &BbpData);
	BbpData |= 0x00000001; //set bit0=1
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, BbpData);

	ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R21, &BbpData);
	BbpData &= ~(0x00000001); //set bit0=0
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R21, BbpData);

	return;
}


static VOID RtmpRfIoWrite(
	IN PRTMP_ADAPTER pAd)
{
	// Set RF value 1's set R3[bit2] = [0]
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R1);
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R2);
	RTMP_RF_IO_WRITE32(pAd, (UINT32)(pAd->LatchRfRegs.R3 & (~0x04)));
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R4);

	RTMPusecDelay(200);

	// Set RF value 2's set R3[bit2] = [1]
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R1);
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R2);
	RTMP_RF_IO_WRITE32(pAd, (UINT32)(pAd->LatchRfRegs.R3 | 0x04));
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R4);

	RTMPusecDelay(200);

	// Set RF value 3's set R3[bit2] = [0]
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R1);
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R2);
	RTMP_RF_IO_WRITE32(pAd, (UINT32)(pAd->LatchRfRegs.R3 & (~0x04)));
	RTMP_RF_IO_WRITE32(pAd, (UINT32)pAd->LatchRfRegs.R4);

	return;
}


static int CheckMCSValid(
	IN	PRTMP_ADAPTER	pAd, 
	IN UCHAR Mode,
	IN UCHAR Mcs)
{
	int i;
	PCHAR pRateTab=NULL;

	switch (Mode)
	{
		case 0:
			pRateTab = CCKRateTable;
			break;
		case 1:
			pRateTab = OFDMRateTable;
			break;
		case 2:
		case 3:
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
			if (IS_RT2883(pAd) || IS_RT3883(pAd))
				pRateTab = HTMIXRateTable3T3R;
#else
			pRateTab = HTMIXRateTable;
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
			break;
		default: 
			ATEDBGPRINT(RT_DEBUG_ERROR, ("unrecognizable Tx Mode %d\n", Mode));
			return -1;
			break;
	}

	i = 0;
	while (pRateTab[i] != -1)
	{
		if (pRateTab[i] == Mcs)
			return 0;
		i++;
	}

	return -1;
}


static INT ATETxPwrHandler(
	IN PRTMP_ADAPTER pAd,
	IN char index)
{
	ULONG R;
	CHAR TxPower = 0;
	UCHAR Bbp94 = 0;
	BOOLEAN bPowerReduce = FALSE;
#ifdef RTMP_RF_RW_SUPPORT
	UCHAR RFValue = 0;
#endif // RTMP_RF_RW_SUPPORT //

#ifdef RALINK_28xx_QA
	if ((pAd->ate.bQATxStart == TRUE) || (pAd->ate.bQARxStart == TRUE))
	{
		/* 
			When QA is used for Tx, pAd->ate.TxPower0/1 and real tx power
			are not synchronized.
		*/
		return 0;
	}
	else
#endif // RALINK_28xx_QA //

	if (index == 0)
	{
		TxPower = pAd->ate.TxPower0;
	}
	else if (index == 1)
	{
		TxPower = pAd->ate.TxPower1;
	}
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	else if (index == 2)
	{
		if (IS_RT2883(pAd) || IS_RT3883(pAd) )
			TxPower = pAd->ate.TxPower2;
		else
		{ 	
			ATEDBGPRINT(RT_DEBUG_ERROR, ("Only TxPower0 and TxPower1 are adjustable !\n"));
			ATEDBGPRINT(RT_DEBUG_ERROR, ("TxPower%d is out of range !\n", index));	
		}	
	}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
	else
	{
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Only TxPower0, TxPower1, and TxPower2 are adjustable !\n"));
#else
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Only TxPower0 and TxPower1 are adjustable !\n"));
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		ATEDBGPRINT(RT_DEBUG_ERROR, ("TxPower%d is out of range !\n", index));
	}

#ifdef RT3883 //wayne_note 20091005_add
	if (IS_RT3883(pAd))
	{
		if (pAd->ate.Channel <= 14)
		{
			if (index == 0)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R53, TxPower);
			else if (index == 1)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R54, TxPower);
			else if (index == 2)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R55, TxPower);
		}
		else 
		{
			CHAR power; 
			power = 0x50 | ((TxPower & 0x10) << 1) | (TxPower & 0xf);
			if (index == 0)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R53, power);
			else if (index == 1)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R54, power);
			else if (index == 2)		
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R55, power);
		}
	}else
#endif 		
#ifdef RT305x
		if (1)
		{
			if (index == 0)
			{
				// Set Tx0 Power
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R12, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0xE0) | TxPower;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R12, (UCHAR)RFValue);
			}
			else if (index == 1)
			{
				// Set Tx1 Power
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R13, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0xE0) | TxPower;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R13, (UCHAR)RFValue);
			}
			else
			{
				ATEDBGPRINT(RT_DEBUG_ERROR, ("Only TxPower0 and TxPower1 are adjustable !\n"));
				ATEDBGPRINT(RT_DEBUG_ERROR, ("TxPower%d is out of range !\n", index));
			}
			ATEDBGPRINT(RT_DEBUG_TRACE, ("3050 or 3052:%s (TxPower%d=%d, RFValue=%x)\n", __FUNCTION__, index, TxPower, RFValue));
		}
		else
#endif // RT305x //		


	{
		if (pAd->ate.Channel <= 14)
		{
			if (TxPower > 31)
			{
				// R3, R4 can't large than 31 (0x24), 31 ~ 36 used by BBP 94
				R = 31;
				if (TxPower <= 36)
					Bbp94 = BBPR94_DEFAULT + (UCHAR)(TxPower - 31);		
			}
			else if (TxPower < 0)
			{
				// R3, R4 can't less than 0, -1 ~ -6 used by BBP 94
				R = 0;
				if (TxPower >= -6)
					Bbp94 = BBPR94_DEFAULT + TxPower;
			}
			else
			{  
				// 0 ~ 31
				R = (ULONG) TxPower;
				Bbp94 = BBPR94_DEFAULT;
			}

			ATEDBGPRINT(RT_DEBUG_TRACE, ("%s (TxPower=%d, R=%ld, BBP_R94=%d)\n", __FUNCTION__, TxPower, R, Bbp94));
		}
		else /* 5.5 GHz */
		{
			if (TxPower > 15)
			{
				// R3, R4 can't large than 15 (0x0F)
				R = 15;
			}
			else if (TxPower < 0)
			{
				// R3, R4 can't less than 0
				// -1 ~ -7
				ASSERT((TxPower >= -7));
				R = (ULONG)(TxPower + 7);
				bPowerReduce = TRUE;
			}
			else
			{  
				// 0 ~ 15
				R = (ULONG) TxPower;
			}

			ATEDBGPRINT(RT_DEBUG_TRACE, ("%s (TxPower=%d, R=%lu)\n", __FUNCTION__, TxPower, R));
		}

		if (pAd->ate.Channel <= 14)
		{
			if (index == 0)
			{
				// shift TX power control to correct RF(R3) register bit position
				R = R << 9;		
				R |= (pAd->LatchRfRegs.R3 & 0xffffc1ff);
				pAd->LatchRfRegs.R3 = R;
			}
			else
			{
				// shift TX power control to correct RF(R4) register bit position
				R = R << 6;		
				R |= (pAd->LatchRfRegs.R4 & 0xfffff83f);
				pAd->LatchRfRegs.R4 = R;
			}
		}
		else /* 5.5GHz */
		{
			if (bPowerReduce == FALSE)
			{
				if (index == 0)
				{
					// shift TX power control to correct RF(R3) register bit position
					R = (R << 10) | (1 << 9);		
					R |= (pAd->LatchRfRegs.R3 & 0xffffc1ff);
					pAd->LatchRfRegs.R3 = R;
				}
				else
				{
					// shift TX power control to correct RF(R4) register bit position
					R = (R << 7) | (1 << 6);		
					R |= (pAd->LatchRfRegs.R4 & 0xfffff83f);
					pAd->LatchRfRegs.R4 = R;
				}
			}
			else
			{
				if (index == 0)
				{
					// shift TX power control to correct RF(R3) register bit position
					R = (R << 10);		
					R |= (pAd->LatchRfRegs.R3 & 0xffffc1ff);

					/* Clear bit 9 of R3 to reduce 7dB. */ 
					pAd->LatchRfRegs.R3 = (R & (~(1 << 9)));
				}
				else
				{
					// shift TX power control to correct RF(R4) register bit position
					R = (R << 7);		
					R |= (pAd->LatchRfRegs.R4 & 0xfffff83f);

					/* Clear bit 6 of R4 to reduce 7dB. */ 
					pAd->LatchRfRegs.R4 = (R & (~(1 << 6)));
				}
			}
		}
		RtmpRfIoWrite(pAd);
	}

	return 0;
}


static NDIS_STATUS ATESTART(
	IN PRTMP_ADAPTER pAd)
{
	UINT32			MacData=0, atemode=0, temp=0;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;
#ifdef RTMP_MAC_PCI
	UINT32			ring_index=0;
	PTXD_STRUC		pTxD = NULL;
#ifdef RT_BIG_ENDIAN
    PTXD_STRUC      pDestTxD = NULL;
    TXD_STRUC       TxD;
#endif // RT_BIG_ENDIAN //
#endif // RTMP_MAC_PCI //
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
	UINT32			bbp_index=0;
	UCHAR			RestoreRfICType=pAd->RfIcType;
#endif // RT3xxx //
	UCHAR			BbpData = 0;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : ===> %s\n", __FUNCTION__));

#if defined(LINUX) || defined(VXWORKS)
#ifdef RTMP_MAC_PCI
#ifndef RTMP_RBUS_SUPPORT
	/* check if we have removed the firmware */
	if (!(ATE_ON(pAd)))
	{
		NICEraseFirmware(pAd);
	}
#endif // RTMP_RBUS_SUPPORT //
#endif // RTMP_MAC_PCI //
#endif // defined(LINUX) || defined(VXWORKS) //

	atemode = pAd->ate.Mode;
	pAd->ate.Mode = ATE_START;

	/* Disable Rx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);
	
	/* Disable auto responder */
	RTMP_IO_READ32(pAd, AUTO_RSP_CFG, &temp);
	temp = temp & 0xFFFFFFFE;
	RTMP_IO_WRITE32(pAd, AUTO_RSP_CFG, temp);

	/* clean bit4 to stop continuous Tx production test */
	/* Disable or cancel pending irp first ??? */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= 0xFFFFFFEF; 
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	if (atemode == ATE_TXCARR)
	{
		/* RT35xx ATE will reuse this code segment. */
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
		// Hardware Reset BBP
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &temp);
		temp = temp | 0x00000002;
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, temp);
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &temp);
		temp = temp & ~(0x00000002);
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, temp);

		// Restore All BBP Value
		for (bbp_index=0;bbp_index<ATE_BBP_REG_NUM;bbp_index++)
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd,bbp_index,restore_BBP[bbp_index]);
		/*
		 	The RfIcType will be reset to zero after the hardware reset bbp command.
			Therefore, we must restore the original RfIcType.
		*/
		pAd->RfIcType=RestoreRfICType;
#endif // RT2883 || RT3883 || RT30xx //

		/* No Carrier Test set BBP R22 bit7=0, bit6=0, bit[5~0]=0x0 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
		BbpData &= 0xFFFFFF00; //clear bit7, bit6, bit[5~0]	
	    ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);
	}
	else if (atemode == ATE_TXCARRSUPP)
	{
		/* RT35xx ATE will reuse this code segment. */
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
		// Hardware Reset BBP
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &temp);
		temp = temp | 0x00000002;
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, temp);
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &temp);
		temp = temp & ~(0x00000002);
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, temp);

		/* Restore All BBP Value */
		for (bbp_index=0;bbp_index<ATE_BBP_REG_NUM;bbp_index++)
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd,bbp_index,restore_BBP[bbp_index]);

		/*
			The RfIcType will be reset to zero after the hardware reset bbp command.
			Therefore, we must restore the original RfIcType.
		*/
		pAd->RfIcType=RestoreRfICType;			
#endif // RT2883 || RT3883 || RT30xx //

		/* No Cont. TX set BBP R22 bit7=0 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
		BbpData &= ~(1 << 7); //set bit7=0
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);

		/* No Carrier Suppression set BBP R24 bit0=0 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R24, &BbpData);
		BbpData &= 0xFFFFFFFE; //clear bit0	
	    ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R24, BbpData);
	}		

	/*
		We should free some resource which was allocated
		when ATE_TXFRAME, ATE_STOP, and ATE_TXCONT.
	*/
	else if ((atemode & ATE_TXFRAME) || (atemode == ATE_STOP))
	{
#ifdef RTMP_MAC_PCI
		PRTMP_TX_RING pTxRing = &pAd->TxRing[QID_AC_BE];
#endif // RTMP_MAC_PCI //
		if (atemode == ATE_TXCONT)
		{
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
			// Hardware Reset BBP
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &temp);
			temp = temp | 0x00000002;
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, temp);
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &temp);
			temp = temp & ~(0x00000002);
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, temp);
			
			//printk("write restore_BBP[] in  ATE_TXFRAME. ATE_STOP \n\r");
			/* Restore All BBP Value */
			for (bbp_index=0;bbp_index<ATE_BBP_REG_NUM;bbp_index++)
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd,bbp_index,restore_BBP[bbp_index]);
			/*
				The RfIcType will be reset to zero after the hardware reset bbp command.
				Therefore, we must restore the original RfIcType.
			*/
			pAd->RfIcType=RestoreRfICType;			
#endif // RT2883 || RT3883 || RT30xx //


			/* Not Cont. TX anymore, so set BBP R22 bit7=0 */
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
			BbpData &= ~(1 << 7); //set bit7=0
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);
		}

		/* Abort Tx, Rx DMA. */
		RtmpDmaEnable(pAd, 0);
#ifdef RTMP_MAC_PCI
		for (ring_index=0; ring_index<TX_RING_SIZE; ring_index++)
		{
			PNDIS_PACKET  pPacket;

#ifndef RT_BIG_ENDIAN
		    pTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[ring_index].AllocVa;
#else
    		pDestTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[ring_index].AllocVa;
    		TxD = *pDestTxD;
    		pTxD = &TxD;
    		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif // !RT_BIG_ENDIAN //
			pTxD->DMADONE = 0;
			pPacket = pTxRing->Cell[ring_index].pNdisPacket;

			if (pPacket)
			{
				PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr0, pTxD->SDLen0, PCI_DMA_TODEVICE);
				RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
			}

			/* Always assign pNdisPacket as NULL after clear */
			pTxRing->Cell[ring_index].pNdisPacket = NULL;

			pPacket = pTxRing->Cell[ring_index].pNextNdisPacket;

			if (pPacket)
			{
				PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr1, pTxD->SDLen1, PCI_DMA_TODEVICE);
				RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
			}

			/* Always assign pNextNdisPacket as NULL after clear */
			pTxRing->Cell[ring_index].pNextNdisPacket = NULL;
#ifdef RT_BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
			WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif // RT_BIG_ENDIAN //
		}
#endif // RTMP_MAC_PCI //

		/* Start Tx, RX DMA */
		RtmpDmaEnable(pAd, 1);
	}

	/* reset Rx statistics. */
	pAd->ate.LastSNR0 = 0;
	pAd->ate.LastSNR1 = 0;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	pAd->ate.LastSNR2 = 0;
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
	pAd->ate.LastRssi0 = 0;
	pAd->ate.LastRssi1 = 0;
	pAd->ate.LastRssi2 = 0;
	pAd->ate.AvgRssi0 = 0;
	pAd->ate.AvgRssi1 = 0;
	pAd->ate.AvgRssi2 = 0;
	pAd->ate.AvgRssi0X8 = 0;
	pAd->ate.AvgRssi1X8 = 0;
	pAd->ate.AvgRssi2X8 = 0;
	pAd->ate.NumOfAvgRssiSample = 0;

#ifdef RALINK_28xx_QA
	/* Tx frame */
	pAd->ate.bQATxStart = FALSE;
	pAd->ate.bQARxStart = FALSE;
	pAd->ate.seq = 0; 

	/* counters */
	pAd->ate.U2M = 0;
	pAd->ate.OtherData = 0;
	pAd->ate.Beacon = 0;
	pAd->ate.OtherCount = 0;
	pAd->ate.TxAc0 = 0;
	pAd->ate.TxAc1 = 0;
	pAd->ate.TxAc2 = 0;
	pAd->ate.TxAc3 = 0;
	pAd->ate.TxHCCA = 0;
	pAd->ate.TxMgmt = 0;
	pAd->ate.RSSI0 = 0;
	pAd->ate.RSSI1 = 0;
	pAd->ate.RSSI2 = 0;
	pAd->ate.SNR0 = 0;
	pAd->ate.SNR1 = 0;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	pAd->ate.SNR2 = 0;
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

	/* control */
	pAd->ate.TxDoneCount = 0;
	// TxStatus : 0 --> task is idle, 1 --> task is running
	pAd->ate.TxStatus = 0;
#endif // RALINK_28xx_QA //

	// Soft reset BBP.
	BbpSoftReset(pAd);

#ifdef CONFIG_AP_SUPPORT 
#ifdef RTMP_MAC_PCI
#if defined(LINUX) || defined(VXWORKS)
	RTMP_OS_NETDEV_STOP_QUEUE(pAd->net_dev);
#endif // defined(LINUX) || defined(VXWORKS) //
#endif // RTMP_MAC_PCI //

   	/* pAd->ate.Mode must HAVE been ATE_START prior to call ATEAPStop(pAd) */
	/* We must disable DFS and Radar Detection, or 8051 will modify BBP registers. */
	ATEAPStop(pAd);
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT 
	/* LinkDown() has "AsicDisableSync();" and "RTMP_BBP_IO_R/W8_BY_REG_ID();" inside. */
//	LinkDown(pAd, FALSE);
//	AsicEnableBssSync(pAd);
	AsicDisableSync(pAd);
#ifdef RTMP_MAC_PCI
#if defined(LINUX) || defined(VXWORKS)
	RTMP_OS_NETDEV_STOP_QUEUE(pAd->net_dev);
#endif // defined(LINUX) || defined(VXWORKS) //
#endif // RTMP_MAC_PCI //
	/* 
		If we skip "LinkDown()", we should disable protection
		to prevent from sending out RTS or CTS-to-self.
	*/
	ATEDisableAsicProtect(pAd);
	RTMPStationStop(pAd);
#endif // CONFIG_STA_SUPPORT //

#ifdef RTMP_MAC_PCI
	/* Disable Tx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 2);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* Disable Rx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

#ifdef RT3883
	if (IS_RT3883(pAd))
	{
		int i;
		UCHAR RFValue = 0, RFValue2 = 0;
		USHORT FreqOffset = 0;

		// Set RF offset  RF_R17=RF_R23 (RT30xx)
		RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET, FreqOffset);
		ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R17, (PUCHAR)&RFValue);
		RFValue2 = (FreqOffset & 0x80) | (FreqOffset & 0x7f);
		if (RFValue2 > RFValue)
		{
			for (i = 1; i <= (RFValue2 - RFValue); i++)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R17, (UCHAR)(RFValue + i));
		}	
		else
		{
			for (i = 1; i <= (RFValue - RFValue2); i++)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R17, (UCHAR)(RFValue - i));

		}
	}
	else
#else
#if 0 /*mark for QA ATE START ERROR*/
	/* Change rf23 before QA running up. */
	{
	    USHORT FreqOffset = 0;
	    UINT32 RfReg = 0;
	    RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET, FreqOffset);

	    RT30xxReadRFRegister(pAd, RF_R23, (PUCHAR)&RfReg);
	    RfReg &= 0x80;
	    RfReg |= (FreqOffset & 0x7f);
	    RT30xxWriteRFRegister(pAd, RF_R23, (UCHAR)RfReg);
	}
#endif // RT305x //
#endif
#endif // RTMP_MAC_PCI //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : <=== %s\n", __FUNCTION__));
	return Status;
}


static NDIS_STATUS ATESTOP(
	IN PRTMP_ADAPTER pAd)
{
	UINT32			MacData=0, ring_index=0;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;
#ifdef RTMP_MAC_PCI
	PRXD_STRUC		pRxD = NULL;
#endif // RTMP_MAC_PCI //
	UCHAR			BbpData = 0;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : ===> %s\n", __FUNCTION__));

	/*
		Abort Tx, RX DMA.
		Q   : How to do the following I/O if Tx, Rx DMA is aborted ?
		Ans : Bulk endpoints are aborted, while the control endpoint is not.
	*/

	/* Default value in BBP R22 is 0x0. */
	BbpData = 0;
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);

	/* Clean bit4 to stop continuous Tx production test. */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= 0xFFFFFFEF;
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData); 
	
	/* Disable Rx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);
	
	/* Abort Tx, RX DMA */
	RtmpDmaEnable(pAd, 0);

	/* Disable Tx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 2);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

#ifdef RTMP_MAC_PCI
#if defined(LINUX) || defined(VXWORKS)
#ifndef RTMP_RBUS_SUPPORT
	pAd->ate.bFWLoading = TRUE;

	Status = NICLoadFirmware(pAd);

	if (Status != NDIS_STATUS_SUCCESS)
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("NICLoadFirmware failed, Status[=0x%08x]\n", Status));
		return Status;
	}
#endif // RTMP_RBUS_SUPPORT //
#endif // defined(LINUX) || defined(VXWORKS) //
	pAd->ate.Mode = ATE_STOP;

	/*
		Even the firmware has been loaded, 
		we still could use ATE_BBP_IO_READ8_BY_REG_ID(). 
		But this is not suggested.
	*/
	BbpSoftReset(pAd);

	RTMP_ASIC_INTERRUPT_DISABLE(pAd);
	
	NICInitializeAdapter(pAd, TRUE);
	
	/*
		Reinitialize Rx Ring before Rx DMA is enabled.
		>>>RxCoherent<<< was gone !
	*/
	for (ring_index = 0; ring_index < RX_RING_SIZE; ring_index++)
	{
		pRxD = (PRXD_STRUC) pAd->RxRing.Cell[ring_index].AllocVa;
		pRxD->DDONE = 0;
	}

	/* We should read EEPROM for all cases. */  
	NICReadEEPROMParameters(pAd, NULL);
	NICInitAsicFromEEPROM(pAd); 

	AsicSwitchChannel(pAd, pAd->CommonCfg.Channel, FALSE);

	/* empty function */
	AsicLockChannel(pAd, pAd->CommonCfg.Channel);		

	/* clear garbage interrupts */
	RTMP_IO_WRITE32(pAd, INT_SOURCE_CSR, 0xffffffff);
	/* Enable Interrupt */
	RTMP_ASIC_INTERRUPT_ENABLE(pAd);
#endif // RTMP_MAC_PCI //

	/* restore RX_FILTR_CFG */
#ifdef CONFIG_AP_SUPPORT 
	RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, APNORMAL);
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT 
	/* restore RX_FILTR_CFG due to that QA maybe set it to 0x3 */
	RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, STANORMAL);
#endif // CONFIG_STA_SUPPORT //

	/* Enable Tx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData |= (1 << 2);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* move to above */

	/* Enable Tx, Rx DMA. */
	RtmpDmaEnable(pAd, 1);

	/* Enable Rx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData |= (1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

#ifdef RTMP_MAC_PCI
#ifdef CONFIG_AP_SUPPORT 
	APStartUp(pAd);
#endif // CONFIG_AP_SUPPORT // 

#ifdef CONFIG_STA_SUPPORT 
	RTMPStationStart(pAd);
#endif // CONFIG_STA_SUPPORT //
#endif // RTMP_MAC_PCI

#if defined(LINUX) || defined(VXWORKS)
	RTMP_OS_NETDEV_START_QUEUE(pAd->net_dev);
#endif // defined(LINUX) || defined(VXWORKS) //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : <=== %s\n", __FUNCTION__));
	return Status;
}


static NDIS_STATUS TXCARR(
	IN PRTMP_ADAPTER pAd)
{
	UINT32			MacData=0;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
	UINT32			bbp_index=0;	 
#endif // RT30xx //
	UCHAR			BbpData = 0;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : ===> %s\n", __FUNCTION__));

	pAd->ate.Mode = ATE_TXCARR;

	/* RT35xx ATE will reuse this code segment. */
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
	for (bbp_index=0;bbp_index<ATE_BBP_REG_NUM;bbp_index++)
		restore_BBP[bbp_index]=0;

	// Record All BBP Value
	for (bbp_index=0; bbp_index<ATE_BBP_REG_NUM; bbp_index++)
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, bbp_index, &restore_BBP[bbp_index]);		
#endif // RT2883 || RT3883 || RT30xx //

	/* QA has done the following steps if it is used. */
	if (pAd->ate.bQATxStart == FALSE) 
	{
		/* Soft reset BBP. */
		//BbpSoftReset(pAd);

		/* Carrier Test set BBP R22 bit7=1, bit6=1, bit[5~0]=0x01 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
		BbpData &= 0xFFFFFF00; //clear bit7, bit6, bit[5~0]
		BbpData |= 0x000000C1; //set bit7=1, bit6=1, bit[5~0]=0x01
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);

		/* set MAC_SYS_CTRL(0x1004) Continuous Tx Production Test (bit4) = 1 */
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
		MacData = MacData | 0x00000010;
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);
	}

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : <=== %s\n", __FUNCTION__));
	return Status;
}


static NDIS_STATUS TXCONT(
	IN PRTMP_ADAPTER pAd)
{
	UINT32			MacData=0;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;
#ifdef RTMP_MAC_PCI
	UINT32			ring_index=0;
	PTXD_STRUC		pTxD = NULL;
	PRTMP_TX_RING 	pTxRing = &pAd->TxRing[QID_AC_BE];
#ifdef RT_BIG_ENDIAN
    PTXD_STRUC      pDestTxD = NULL;
    TXD_STRUC       TxD;
#endif // RT_BIG_ENDIAN //
#endif // RTMP_MAC_PCI //
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
	UINT32			bbp_index=0;
#endif // RT30xx //
	UCHAR			BbpData = 0;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : ===> %s\n", __FUNCTION__));

	if (pAd->ate.bQATxStart == TRUE)
	{
		/*
			set MAC_SYS_CTRL(0x1004) bit4(Continuous Tx Production Test)
			and bit2(MAC TX enable) back to zero.
		*/ 
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
		MacData &= 0xFFFFFFEB;
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

		/* set BBP R22 bit7=0 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
		BbpData &= 0xFFFFFF7F; //set bit7=0
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);
	}

	/* RT35xx ATE will reuse this code segment. */
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3350)|| defined (CONFIG_RALINK_RT3352)|| defined (CONFIG_RALINK_RT3883)
	for(bbp_index=0;bbp_index<ATE_BBP_REG_NUM;bbp_index++)
		restore_BBP[bbp_index]=0;
	
	// Record All BBP Value
	for (bbp_index=0; bbp_index<ATE_BBP_REG_NUM; bbp_index++)
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, bbp_index, &restore_BBP[bbp_index]);		
#endif // RT2883 || RT3883 || RT30xx //

	/* 
		for TxCont mode.
		Step 1: Send 50 packets first then wait for a moment.
		Step 2: Send more 50 packet then start continue mode.
	*/


	/* Step 1: send 50 packets first. */
	pAd->ate.Mode = ATE_TXCONT;
	pAd->ate.TxCount = 50;


	/* Soft reset BBP. */
	//BbpSoftReset(pAd);

	/* Abort Tx, RX DMA. */
	RtmpDmaEnable(pAd, 0);

#ifdef RTMP_MAC_PCI
	/* Fix can't smooth kick */
	{
		RTMP_IO_READ32(pAd, TX_DTX_IDX0 + QID_AC_BE * 0x10,  &pTxRing->TxDmaIdx);
		pTxRing->TxSwFreeIdx = pTxRing->TxDmaIdx;
		pTxRing->TxCpuIdx = pTxRing->TxDmaIdx;
		RTMP_IO_WRITE32(pAd, TX_CTX_IDX0 + QID_AC_BE * 0x10, pTxRing->TxCpuIdx);
	}
#endif // RTMP_MAC_PCI //

	/* Do it after Tx/Rx DMA is aborted. */
	pAd->ate.TxDoneCount = 0;
	
	/* Only needed if we have to send some normal frames. */
	SetJapanFilter(pAd);

#ifdef RTMP_MAC_PCI
	for (ring_index = 0; (ring_index < TX_RING_SIZE-1) && (ring_index < pAd->ate.TxCount); ring_index++)
	{
		PNDIS_PACKET pPacket;
		UINT32 TxIdx = pTxRing->TxCpuIdx;

#ifndef RT_BIG_ENDIAN
		pTxD = (PTXD_STRUC)pTxRing->Cell[TxIdx].AllocVa;
#else
		pDestTxD = (PTXD_STRUC)pTxRing->Cell[TxIdx].AllocVa;
		TxD = *pDestTxD;
		pTxD = &TxD;
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif // !RT_BIG_ENDIAN //

		/* Clean current cell. */
		pPacket = pTxRing->Cell[TxIdx].pNdisPacket;

		if (pPacket)
		{
			PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr0, pTxD->SDLen0, PCI_DMA_TODEVICE);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
		}

		/* Always assign pNdisPacket as NULL after clear */
		pTxRing->Cell[TxIdx].pNdisPacket = NULL;

		pPacket = pTxRing->Cell[TxIdx].pNextNdisPacket;

		if (pPacket)
		{
			PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr1, pTxD->SDLen1, PCI_DMA_TODEVICE);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
		}

		/* Always assign pNextNdisPacket as NULL after clear */
		pTxRing->Cell[TxIdx].pNextNdisPacket = NULL;

#ifdef RT_BIG_ENDIAN
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
		WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif // RT_BIG_ENDIAN //

		if (ATESetUpFrame(pAd, TxIdx) != 0)
			return NDIS_STATUS_FAILURE;

		INC_RING_INDEX(pTxRing->TxCpuIdx, TX_RING_SIZE);
	}

	ATESetUpFrame(pAd, pTxRing->TxCpuIdx);
#endif // RTMP_MAC_PCI //

	/* Enable Tx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData |= (1 << 2);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* Disable Rx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* Start Tx, RX DMA. */
	RtmpDmaEnable(pAd, 1);

#ifdef RALINK_28xx_QA
	if (pAd->ate.bQATxStart == TRUE)
	{
		pAd->ate.TxStatus = 1;
	}
#endif // RALINK_28xx_QA //

#ifdef RTMP_MAC_PCI
	/* kick Tx Ring */
	RTMP_IO_WRITE32(pAd, TX_CTX_IDX0 + QID_AC_BE * RINGREG_DIFF, pAd->TxRing[QID_AC_BE].TxCpuIdx);

	RTMPusecDelay(5000);
#endif // RTMP_MAC_PCI //

	/* Step 2: send more 50 packets then start continue mode. */
	/* Abort Tx, RX DMA. */
	RtmpDmaEnable(pAd, 0);

	/* Cont. TX set BBP R22 bit7=1 */
	ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
	BbpData |= 0x00000080; //set bit7=1
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);

	pAd->ate.TxCount = 50;
#ifdef RTMP_MAC_PCI
	/* Fix can't smooth kick */
	{
		RTMP_IO_READ32(pAd, TX_DTX_IDX0 + QID_AC_BE * 0x10,  &pTxRing->TxDmaIdx);
		pTxRing->TxSwFreeIdx = pTxRing->TxDmaIdx;
		pTxRing->TxCpuIdx = pTxRing->TxDmaIdx;
		RTMP_IO_WRITE32(pAd, TX_CTX_IDX0 + QID_AC_BE * 0x10, pTxRing->TxCpuIdx);					
	}
#endif // RTMP_MAC_PCI //

	pAd->ate.TxDoneCount = 0;
	SetJapanFilter(pAd);

#ifdef RTMP_MAC_PCI
	for (ring_index = 0; (ring_index < TX_RING_SIZE-1) && (ring_index < pAd->ate.TxCount); ring_index++)
	{
		PNDIS_PACKET pPacket;
		UINT32 TxIdx = pTxRing->TxCpuIdx;

#ifndef RT_BIG_ENDIAN
		pTxD = (PTXD_STRUC)pTxRing->Cell[TxIdx].AllocVa;
#else
		pDestTxD = (PTXD_STRUC)pTxRing->Cell[TxIdx].AllocVa;
		TxD = *pDestTxD;
		pTxD = &TxD;
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif // !RT_BIG_ENDIAN //
		/* clean current cell */
		pPacket = pTxRing->Cell[TxIdx].pNdisPacket;

		if (pPacket)
		{
			PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr0, pTxD->SDLen0, PCI_DMA_TODEVICE);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
		}

		/* Always assign pNdisPacket as NULL after clear. */
		pTxRing->Cell[TxIdx].pNdisPacket = NULL;

		pPacket = pTxRing->Cell[TxIdx].pNextNdisPacket;

		if (pPacket)
		{
			PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr1, pTxD->SDLen1, PCI_DMA_TODEVICE);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
		}

		/* Always assign pNextNdisPacket as NULL after clear. */
		pTxRing->Cell[TxIdx].pNextNdisPacket = NULL;

#ifdef RT_BIG_ENDIAN
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
		WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif // RT_BIG_ENDIAN //

		if (ATESetUpFrame(pAd, TxIdx) != 0)
			return NDIS_STATUS_FAILURE;

		INC_RING_INDEX(pTxRing->TxCpuIdx, TX_RING_SIZE);
	}

	ATESetUpFrame(pAd, pTxRing->TxCpuIdx);
#endif // RTMP_MAC_PCI //

	/* Enable Tx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData |= (1 << 2);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* Disable Rx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* Start Tx, Rx DMA. */
	RtmpDmaEnable(pAd, 1);

#ifdef RALINK_28xx_QA
	if (pAd->ate.bQATxStart == TRUE)
	{
		pAd->ate.TxStatus = 1;
	}
#endif // RALINK_28xx_QA //

#ifdef RTMP_MAC_PCI
	/* kick Tx Ring */
	RTMP_IO_WRITE32(pAd, TX_CTX_IDX0 + QID_AC_BE * RINGREG_DIFF, pAd->TxRing[QID_AC_BE].TxCpuIdx);
#endif // RTMP_MAC_PCI //

	RTMPusecDelay(500);

	/* enable continuous tx production test */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData |= 0x00000010;
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);							

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : <=== %s\n", __FUNCTION__));
	return Status;
}


static NDIS_STATUS TXFRAME(
	IN PRTMP_ADAPTER pAd)
{
	UINT32			MacData=0;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;
#ifdef RTMP_MAC_PCI
	UINT32			ring_index=0;
	PRTMP_TX_RING 	pTxRing = &pAd->TxRing[QID_AC_BE];
	PTXD_STRUC		pTxD = NULL;
#ifdef RT_BIG_ENDIAN
    PTXD_STRUC      pDestTxD = NULL;
    TXD_STRUC       TxD;
#endif // RT_BIG_ENDIAN //
#endif // RTMP_MAC_PCI //
	UCHAR			BbpData = 0;


	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : ===> %s(Count=%d)\n", __FUNCTION__, pAd->ate.TxCount));
	pAd->ate.Mode |= ATE_TXFRAME;

#ifdef RTMP_MAC_PCI
	/* Default value in BBP R22 is 0x0. */
	BbpData = 0;
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);

	/* Soft reset BBP. */
	//BbpSoftReset(pAd);

	/* clean bit4 to stop continuous Tx production test */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= 0xFFFFFFEF; 
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* Abort Tx, RX DMA. */
	RtmpDmaEnable(pAd, 0);

	/* Fix can't smooth kick */
	{
		RTMP_IO_READ32(pAd, TX_DTX_IDX0 + QID_AC_BE * 0x10,  &pTxRing->TxDmaIdx);
		pTxRing->TxSwFreeIdx = pTxRing->TxDmaIdx;
		pTxRing->TxCpuIdx = pTxRing->TxDmaIdx;
		RTMP_IO_WRITE32(pAd, TX_CTX_IDX0 + QID_AC_BE * 0x10, pTxRing->TxCpuIdx);					
	}

	pAd->ate.TxDoneCount = 0;

	SetJapanFilter(pAd);
	
	for (ring_index = 0; (ring_index < TX_RING_SIZE-1) && (ring_index < pAd->ate.TxCount); ring_index++)
	{
		PNDIS_PACKET pPacket;
		UINT32 TxIdx = pTxRing->TxCpuIdx;

#ifndef RT_BIG_ENDIAN
		pTxD = (PTXD_STRUC)pTxRing->Cell[TxIdx].AllocVa;
#else
		pDestTxD = (PTXD_STRUC)pTxRing->Cell[TxIdx].AllocVa;
		TxD = *pDestTxD;
		pTxD = &TxD;
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif // !RT_BIG_ENDIAN //
		/* Clean current cell. */
		pPacket = pTxRing->Cell[TxIdx].pNdisPacket;

		if (pPacket)
		{
			PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr0, pTxD->SDLen0, PCI_DMA_TODEVICE);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
		}

		/* Always assign pNdisPacket as NULL after clear */
		pTxRing->Cell[TxIdx].pNdisPacket = NULL;

		pPacket = pTxRing->Cell[TxIdx].pNextNdisPacket;

		if (pPacket)
		{
			PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr1, pTxD->SDLen1, PCI_DMA_TODEVICE);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
		}

		/* Always assign pNextNdisPacket as NULL after clear */
		pTxRing->Cell[TxIdx].pNextNdisPacket = NULL;

#ifdef RT_BIG_ENDIAN
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
		WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif // RT_BIG_ENDIAN //

		if (ATESetUpFrame(pAd, TxIdx) != 0)
			return NDIS_STATUS_FAILURE;

		INC_RING_INDEX(pTxRing->TxCpuIdx, TX_RING_SIZE);

	}

	ATESetUpFrame(pAd, pTxRing->TxCpuIdx);

	/* Start Tx, Rx DMA. */
	RtmpDmaEnable(pAd, 1);

	/* Enable Tx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData |= (1 << 2);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);
#endif // RTMP_MAC_PCI //

#ifdef RALINK_28xx_QA
	/* add this for LoopBack mode */
	if (pAd->ate.bQARxStart == FALSE)  
	{
		/* Disable Rx */
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
		MacData &= ~(1 << 3);
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);
	}

	if (pAd->ate.bQATxStart == TRUE)  
	{
		pAd->ate.TxStatus = 1;
	}
#else
	/* Disable Rx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);
#endif // RALINK_28xx_QA //

#ifdef RTMP_MAC_PCI
	RTMP_IO_READ32(pAd, TX_DTX_IDX0 + QID_AC_BE * RINGREG_DIFF, &pAd->TxRing[QID_AC_BE].TxDmaIdx);
	/* kick Tx Ring */
	RTMP_IO_WRITE32(pAd, TX_CTX_IDX0 + QID_AC_BE * RINGREG_DIFF, pAd->TxRing[QID_AC_BE].TxCpuIdx);

	pAd->RalinkCounters.KickTxCount++;
#endif // RTMP_MAC_PCI //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : <=== %s\n", __FUNCTION__));
	return Status;
}


static NDIS_STATUS RXFRAME(
	IN PRTMP_ADAPTER pAd)
{
	UINT32			MacData=0;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;
	UCHAR			BbpData = 0;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : ===> %s\n", __FUNCTION__));

	/* Disable Rx of MAC block */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* Default value in BBP R22 is 0x0. */
	BbpData = 0;
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);

#ifdef RT3883 
	if (pAd->ate.TxWI.BW == BW_20)
	{
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R103, 0xC0); 
	}
	else
	{
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R103, 0x00);
	}
#endif // RT3883 //

	/* Clean bit4 to stop continuous Tx production test. */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= 0xFFFFFFEF;
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	pAd->ate.Mode |= ATE_RXFRAME;

	/* Disable Tx of MAC block. */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 2);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	/* Enable Rx of MAC block. */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData |= (1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : <=== %s\n", __FUNCTION__));
	return Status;
}


#ifdef RALINK_28xx_QA
static NDIS_STATUS TXSTOP(
	IN PRTMP_ADAPTER pAd)
{
	UINT32			MacData=0, atemode=0;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;
#ifdef RTMP_MAC_PCI
	UINT32			ring_index=0;
	PTXD_STRUC		pTxD = NULL;
#ifdef RT_BIG_ENDIAN
    PTXD_STRUC      pDestTxD = NULL;
    TXD_STRUC       TxD;
#endif // RT_BIG_ENDIAN //
#endif // RTMP_MAC_PCI //
	UCHAR			BbpData = 0;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : ===> %s\n", __FUNCTION__));
	
	atemode = pAd->ate.Mode;
	pAd->ate.Mode &= ATE_TXSTOP;
	pAd->ate.bQATxStart = FALSE;

	if (atemode == ATE_TXCARR)
	{
		/* No Carrier Test set BBP R22 bit7=0, bit6=0, bit[5~0]=0x0 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
		BbpData &= 0xFFFFFF00; //clear bit7, bit6, bit[5~0]	
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);
	}
	else if (atemode == ATE_TXCARRSUPP)
	{
		/* No Cont. TX set BBP R22 bit7=0 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
		BbpData &= ~(1 << 7); //set bit7=0
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);

		/* No Carrier Suppression set BBP R24 bit0=0 */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R24, &BbpData);
		BbpData &= 0xFFFFFFFE; //clear bit0	
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R24, BbpData);
	}		

	/*
		We should free some resource which was allocated
		when ATE_TXFRAME, ATE_STOP, and ATE_TXCONT.
	*/
	else if ((atemode & ATE_TXFRAME) || (atemode == ATE_STOP))
#ifdef RTMP_MAC_PCI
	{
		PRTMP_TX_RING pTxRing = &pAd->TxRing[QID_AC_BE];

		if (atemode == ATE_TXCONT)
		{
			// No Cont. TX set BBP R22 bit7=0
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R22, &BbpData);
			BbpData &= ~(1 << 7); //set bit7=0
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R22, BbpData);
		}

		/* Abort Tx, Rx DMA. */
		RtmpDmaEnable(pAd, 0);

		for (ring_index=0; ring_index<TX_RING_SIZE; ring_index++)
		{
			PNDIS_PACKET  pPacket;

#ifndef RT_BIG_ENDIAN
		    pTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[ring_index].AllocVa;
#else
    		pDestTxD = (PTXD_STRUC)pAd->TxRing[QID_AC_BE].Cell[ring_index].AllocVa;
    		TxD = *pDestTxD;
    		pTxD = &TxD;
    		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#endif // !RT_BIG_ENDIAN //
			pTxD->DMADONE = 0;
			pPacket = pTxRing->Cell[ring_index].pNdisPacket;

			if (pPacket)
			{
				PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr0, pTxD->SDLen0, PCI_DMA_TODEVICE);
				RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
			}

			/* Always assign pNdisPacket as NULL after clear */
			pTxRing->Cell[ring_index].pNdisPacket = NULL;

			pPacket = pTxRing->Cell[ring_index].pNextNdisPacket;

			if (pPacket)
			{
				PCI_UNMAP_SINGLE(pAd, pTxD->SDPtr1, pTxD->SDLen1, PCI_DMA_TODEVICE);
				RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_SUCCESS);
			}

			/* Always assign pNextNdisPacket as NULL after clear */
			pTxRing->Cell[ring_index].pNextNdisPacket = NULL;
#ifdef RT_BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
			WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif // RT_BIG_ENDIAN //
		}
		/* Enable Tx, Rx DMA */
		RtmpDmaEnable(pAd, 1);
	}
#endif // RTMP_MAC_PCI //

	/* task Tx status : 0 --> task is idle, 1 --> task is running */
	pAd->ate.TxStatus = 0;

	/* Soft reset BBP. */
	//BbpSoftReset(pAd);

	/* Disable Tx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 2);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : <=== %s\n", __FUNCTION__));
	return Status;
}


static NDIS_STATUS RXSTOP(
	IN PRTMP_ADAPTER pAd)
{
	UINT32			MacData=0;
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : ===> %s\n", __FUNCTION__));

	/* Disable Rx */
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacData);
	MacData &= ~(1 << 3);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacData);

	pAd->ate.Mode &= ATE_RXSTOP;
	pAd->ate.bQARxStart = FALSE;
//	pAd->ate.TxDoneCount = pAd->ate.TxCount;

	/* Soft reset BBP. */
	//BbpSoftReset(pAd);

	/* move to above */

	ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE : <=== %s\n", __FUNCTION__));
	return Status;
}
#endif // RALINK_28xx_QA //


/*
==========================================================================
    Description:
        Set ATE operation mode to
        0. ATESTART  = Start ATE Mode
        1. ATESTOP   = Stop ATE Mode
        2. TXCARR    = Transmit Carrier
        3. TXCONT    = Continuous Transmit
        4. TXFRAME   = Transmit Frames
        5. RXFRAME   = Receive Frames
#ifdef RALINK_28xx_QA
        6. TXSTOP    = Stop Any Type of Transmition
        7. RXSTOP    = Stop Receiving Frames        
#endif // RALINK_28xx_QA //

    Return:
        NDIS_STATUS_SUCCESS if all parameters are OK.
==========================================================================
*/
static NDIS_STATUS	ATECmdHandler(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	NDIS_STATUS		Status = NDIS_STATUS_SUCCESS;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("===> %s\n", __FUNCTION__));

	ATEAsicSwitchChannel(pAd);

	/* AsicLockChannel() is empty function so far in fact */
	AsicLockChannel(pAd, pAd->ate.Channel);

	RTMPusecDelay(5000);

#ifdef	CONFIG_RT2880_ATE_CMD_NEW
	if (!strcmp(arg, "ATESTART")) 		
	{
		Status = ATESTART(pAd);
	}
	else if (!strcmp(arg, "ATESTOP")) 
	{
		Status = ATESTOP(pAd);
	}
#else
	if (!strcmp(arg, "APSTOP")) 		//Enter ATE mode and set Tx/Rx Idle
	{
		Status = ATESTART(pAd);
	}
	else if (!strcmp(arg, "APSTART")) 
	{
		Status = ATESTOP(pAd);
	}
#endif
	else if (!strcmp(arg, "TXCARR"))	
	{
		Status = TXCARR(pAd);
	}
	else if (!strcmp(arg, "TXCONT"))	
	{
		Status = TXCONT(pAd);
	}
	else if (!strcmp(arg, "TXFRAME")) 
	{
		Status = TXFRAME(pAd);
	}
	else if (!strcmp(arg, "RXFRAME")) 
	{
		Status = RXFRAME(pAd);
	}
#ifdef RALINK_28xx_QA
	/* Enter ATE mode and set Tx/Rx Idle */
	else if (!strcmp(arg, "TXSTOP"))
	{
		Status = TXSTOP(pAd);
	}
	else if (!strcmp(arg, "RXSTOP"))
	{
		Status = RXSTOP(pAd);
	}
#endif // RALINK_28xx_QA //
	else
	{	
		ATEDBGPRINT(RT_DEBUG_TRACE, ("ATE: Invalid arg!\n"));
		Status = NDIS_STATUS_INVALID_DATA;
	}
	RTMPusecDelay(5000);

	ATEDBGPRINT(RT_DEBUG_TRACE, ("<=== %s\n", __FUNCTION__));

	return Status;
}


INT	Set_ATE_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	// Handle ATEACTIVE and ATEPASSIVE commands as a special case
	if (!strcmp(arg, "ATEACTIVE"))
	{
		pAd->ate.PassiveMode = FALSE;
		return TRUE;
	}

	if (!strcmp(arg, "ATEPASSIVE"))
	{
		pAd->ate.PassiveMode = TRUE;
		return TRUE;
	}

	// Disallow all other ATE commands in passive mode
	if (pAd->ate.PassiveMode)
		return TRUE;

#ifdef COC_SUPPORT
#ifdef  CONFIG_RT2880_ATE_CMD_NEW
        if (!strcmp(arg, "ATESTART"))
        {
#else
        if (!strcmp(arg, "APSTOP"))
        {
#endif
                AsicSetCoC(pAd, 0);
	}
#endif // COC_SUPPORT

	if (ATECmdHandler(pAd, arg) == NDIS_STATUS_SUCCESS)
	{
		ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //

		return TRUE;
	}
	else
	{
		ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_Proc Failed\n"));
		return FALSE;
	}
}


/* 
==========================================================================
    Description:
        Set ATE ADDR1=DA for TxFrame(AP  : To DS = 0 ; From DS = 1)
        or
        Set ATE ADDR3=DA for TxFrame(STA : To DS = 1 ; From DS = 0)        
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_DA_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	PSTRING				value;
	INT					i;

	// Mac address acceptable format 01:02:03:04:05:06 length 17	
	if (strlen(arg) != 17)  
		return FALSE;

    for (i = 0, value = rstrtok(arg, ":"); value; value = rstrtok(NULL, ":")) 
	{
		/* sanity check */
		if ((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))))
		{
			return FALSE;  
		}
#ifdef CONFIG_AP_SUPPORT
		AtoH(value, &pAd->ate.Addr1[i++], 1);
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
		AtoH(value, &pAd->ate.Addr3[i++], 1);
#endif // CONFIG_STA_SUPPORT //
	}

	/* sanity check */
	if (i != 6)
	{
		return FALSE;  
	}
#ifdef CONFIG_AP_SUPPORT		
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_DA_Proc (DA = %2X:%2X:%2X:%2X:%2X:%2X)\n", pAd->ate.Addr1[0],
		pAd->ate.Addr1[1], pAd->ate.Addr1[2], pAd->ate.Addr1[3], pAd->ate.Addr1[4], pAd->ate.Addr1[5]));

#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_DA_Proc (DA = %2X:%2X:%2X:%2X:%2X:%2X)\n", pAd->ate.Addr3[0],
		pAd->ate.Addr3[1], pAd->ate.Addr3[2], pAd->ate.Addr3[3], pAd->ate.Addr3[4], pAd->ate.Addr3[5]));
#endif // CONFIG_STA_SUPPORT //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_DA_Proc Success\n"));
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE ADDR3=SA for TxFrame(AP  : To DS = 0 ; From DS = 1)
        or
        Set ATE ADDR2=SA for TxFrame(STA : To DS = 1 ; From DS = 0)
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_SA_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	PSTRING				value;
	INT					i;

	// Mac address acceptable format 01:02:03:04:05:06 length 17	
	if (strlen(arg) != 17)  
		return FALSE;

    for (i=0, value = rstrtok(arg, ":"); value; value = rstrtok(NULL, ":")) 
	{
		/* sanity check */
		if ((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))))
		{
			return FALSE;  
		}
#ifdef CONFIG_AP_SUPPORT
		AtoH(value, &pAd->ate.Addr3[i++], 1);
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
		AtoH(value, &pAd->ate.Addr2[i++], 1);
#endif // CONFIG_STA_SUPPORT //
	}

	/* sanity check */
	if (i != 6)
	{
		return FALSE;
	}
#ifdef CONFIG_AP_SUPPORT		
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_SA_Proc (SA = %2X:%2X:%2X:%2X:%2X:%2X)\n", pAd->ate.Addr3[0],
		pAd->ate.Addr3[1], pAd->ate.Addr3[2], pAd->ate.Addr3[3], pAd->ate.Addr3[4], pAd->ate.Addr3[5]));
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_SA_Proc (SA = %2X:%2X:%2X:%2X:%2X:%2X)\n", pAd->ate.Addr2[0],
		pAd->ate.Addr2[1], pAd->ate.Addr2[2], pAd->ate.Addr2[3], pAd->ate.Addr2[4], pAd->ate.Addr2[5]));
#endif // CONFIG_STA_SUPPORT //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_SA_Proc Success\n"));

	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE ADDR2=BSSID for TxFrame(AP  : To DS = 0 ; From DS = 1)
        or
        Set ATE ADDR1=BSSID for TxFrame(STA : To DS = 1 ; From DS = 0)

    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_BSSID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	PSTRING				value;
	INT					i;

	// Mac address acceptable format 01:02:03:04:05:06 length 17	
	if (strlen(arg) != 17)  
		return FALSE;

    for (i=0, value = rstrtok(arg, ":"); value; value = rstrtok(NULL, ":")) 
	{
		/* sanity check */
		if ((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))))
		{
			return FALSE;  
		}
#ifdef CONFIG_AP_SUPPORT
		AtoH(value, &pAd->ate.Addr2[i++], 1);
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
		AtoH(value, &pAd->ate.Addr1[i++], 1);
#endif // CONFIG_STA_SUPPORT //
	}

	/* sanity check */
	if(i != 6)
	{
		return FALSE;
	}
#ifdef CONFIG_AP_SUPPORT		
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_BSSID_Proc (BSSID = %2X:%2X:%2X:%2X:%2X:%2X)\n",	pAd->ate.Addr2[0],
		pAd->ate.Addr2[1], pAd->ate.Addr2[2], pAd->ate.Addr2[3], pAd->ate.Addr2[4], pAd->ate.Addr2[5]));
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_BSSID_Proc (BSSID = %2X:%2X:%2X:%2X:%2X:%2X)\n",	pAd->ate.Addr1[0],
		pAd->ate.Addr1[1], pAd->ate.Addr1[2], pAd->ate.Addr1[3], pAd->ate.Addr1[4], pAd->ate.Addr1[5]));
#endif // CONFIG_STA_SUPPORT //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_BSSID_Proc Success\n"));

	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Tx Channel

    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_CHANNEL_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR channel;

	channel = simple_strtol(arg, 0, 10);

	// to allow A band channel : ((channel < 1) || (channel > 14))
	if ((channel < 1) || (channel > 216))
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_CHANNEL_Proc::Out of range, it should be in range of 1~14.\n"));
		return FALSE;
	}
	pAd->ate.Channel = channel;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_CHANNEL_Proc (ATE Channel = %d)\n", pAd->ate.Channel));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_CHANNEL_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Tx Power0
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_POWER0_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg)
{
	CHAR TxPower;
	
	TxPower = simple_strtol(arg, 0, 10);

	
	if (pAd->ate.Channel <= 14)
	{
		if ((TxPower > 31) || (TxPower < 0))
		{
			ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER0_Proc::Out of range (Value=%d)\n", TxPower));
			return FALSE;
		}
	}
	else/* 5.5 GHz */
	{
#ifdef RT3883
		if (IS_RT3883(pAd))
		{
			if ((TxPower > 31) || (TxPower < 0))
			{
				ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER0_Proc::Out of range (Value=%d)\n", TxPower));
				return FALSE;
			}
		}
#else
		if ((TxPower > 15) || (TxPower < -7))
		{
			ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER0_Proc::Out of range (Value=%d)\n", TxPower));
			return FALSE;
		}
#endif // RT3883 //
	}

	pAd->ate.TxPower0 = TxPower;
	ATETxPwrHandler(pAd, 0);

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
	{
		ATEAsicSwitchChannel(pAd);
	}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_POWER0_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Tx Power1
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_POWER1_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg)
{
	CHAR TxPower;
	
	TxPower = simple_strtol(arg, 0, 10);

	if (pAd->ate.Channel <= 14)
	{
		if ((TxPower > 31) || (TxPower < 0))
		{
			ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER1_Proc::Out of range (Value=%d)\n", TxPower));
			return FALSE;
		}
	}
	else
	{
#ifdef RT3883
		if (IS_RT3883(pAd))
		{
			if ((TxPower > 31) || (TxPower < 0))
			{
				ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER0_Proc::Out of range (Value=%d)\n", TxPower));
				return FALSE;
			}
		}
#else
		if ((TxPower > 15) || (TxPower < -7))
		{
			ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER0_Proc::Out of range (Value=%d)\n", TxPower));
			return FALSE;
		}
#endif // RT3883 //
	}

	pAd->ate.TxPower1 = TxPower;
	ATETxPwrHandler(pAd, 1);

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
	{
		ATEAsicSwitchChannel(pAd);
	}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_POWER1_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
/* 
==========================================================================
    Description:
        Set ATE Tx Power2
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_POWER2_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg)
{
	CHAR TxPower;
	
	TxPower = simple_strtol(arg, 0, 10);

	if (pAd->ate.Channel <= 14)
	{
		if ((TxPower > 31) || (TxPower < 0))
		{
			ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER2_Proc::Out of range (Value=%d)\n", TxPower));
			return FALSE;
		}
	}
	else
	{
#ifdef RT3883
		if (IS_RT3883(pAd))
		{
			if ((TxPower > 31) || (TxPower < 0))
			{
				ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER0_Proc::Out of range (Value=%d)\n", TxPower));
				return FALSE;
			}
		}
#else
		if ((TxPower > 15) || (TxPower < -7))
		{
			ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_POWER0_Proc::Out of range (Value=%d)\n", TxPower));
			return FALSE;
		}
#endif // RT3883 //
	}

	pAd->ate.TxPower2 = TxPower;
	ATETxPwrHandler(pAd, 2);

	if (IS_RT2883(pAd) || IS_RT3883(pAd))
	{
		ATEAsicSwitchChannel(pAd);
	}

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_POWER2_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	return TRUE;
}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //


/* 
==========================================================================
    Description:
        Set ATE Tx Antenna
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_Antenna_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg)
{
	CHAR value;
	
	value = simple_strtol(arg, 0, 10);

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	if( ((IS_RT2883(pAd) || IS_RT3883(pAd) ) && ((value > 3) || (value < 0)))   || 
		 ((!IS_RT2883(pAd) && !IS_RT3883(pAd)) && ((value > 2) || (value < 0))))
#else
	if ((value > 2) || (value < 0))
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_Antenna_Proc::Out of range (Value=%d)\n", value));
		return FALSE;
	}

	pAd->ate.TxAntennaSel = value;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_Antenna_Proc (Antenna = %d)\n", pAd->ate.TxAntennaSel));
	ATEDBGPRINT(RT_DEBUG_TRACE,("Ralink: Set_ATE_TX_Antenna_Proc Success\n"));

	/* calibration power unbalance issues */
	ATEAsicSwitchChannel(pAd);

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Rx Antenna
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_RX_Antenna_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg)
{
	CHAR value;
	
	value = simple_strtol(arg, 0, 10);

	if ((value > 3) || (value < 0))
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_RX_Antenna_Proc::Out of range (Value=%d)\n", value));
		return FALSE;
	}

	pAd->ate.RxAntennaSel = value;

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_RX_Antenna_Proc (Antenna = %d)\n", pAd->ate.RxAntennaSel));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_RX_Antenna_Proc Success\n"));

	/* calibration power unbalance issues */
	ATEAsicSwitchChannel(pAd);

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


//peter ++
#ifdef CONFIG_RALINK_RT3350
/* 
==========================================================================
    Description:
        Set ATE PA bias to improve EVM
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT Set_ATE_PA_Bias_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR PABias = 0;
	UCHAR RFValue;
	
	PABias = simple_strtol(arg, 0, 10);

	if (PABias >= 16)
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_PA_Bias_Proc::Out of range, it should be in range of 0~15.\n"));
		return FALSE;
	}

	pAd->ate.PABias = PABias;

    ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R19, (PUCHAR)&RFValue);
    RFValue = (((RFValue & 0x0F) | (pAd->ate.PABias << 4)));
    ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R19, (UCHAR)RFValue);

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_PA_Bias_Proc (PABias = %d)\n", pAd->ate.PABias));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_PA_Bias_Proc Success\n"));

	return TRUE;
}
#endif // CONFIG_RALINK_RT3350 //
//peter --
/* 
==========================================================================
    Description:
        Set ATE RF frequence offset
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_FREQOFFSET_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR RFFreqOffset = 0;
	ULONG R4 = 0;
#ifdef RTMP_RF_RW_SUPPORT
	UCHAR RFValue = 0;
#endif // RTMP_RF_RW_SUPPORT //
	
	RFFreqOffset = simple_strtol(arg, 0, 10);

	/* RT35xx ATE will reuse this code segment. */
#ifdef RTMP_RF_RW_SUPPORT
//2008/08/06: KH modified the limit of offset value from 64 to 96(0x5F + 0x01)
	if (RFFreqOffset >= 96)
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_FREQOFFSET_Proc::Out of range, it should be in range of 0~95.\n"));
		return FALSE;
	}
#else
	if (RFFreqOffset >= 64)
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_FREQOFFSET_Proc::Out of range, it should be in range of 0~63.\n"));
		return FALSE;
	}
#endif // RTMP_RF_RW_SUPPORT //
	pAd->ate.RFFreqOffset = RFFreqOffset;

#ifdef RTMP_RF_RW_SUPPORT
#ifdef RT305x
	if (1)
	{
		ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R23, (PUCHAR)&RFValue);
		RFValue = ((RFValue & 0x80) | pAd->ate.RFFreqOffset);
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R23, (UCHAR)RFValue);
	}
	else
#endif // RT305x //
#if defined (CONFIG_RALINK_RT3883)
	if (IS_RT3883(pAd))
	{
		int i;
		UCHAR RFValue2 = 0;

		// Set RF offset  RF_R17=RF_R23 (RT30xx)
		ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R17, (PUCHAR)&RFValue);
		RFValue2 = (RFValue & 0x80) | pAd->ate.RFFreqOffset;
		if (RFValue2 > RFValue)
		{
			for (i = 1; i <= (RFValue2 - RFValue); i++)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R17, (UCHAR)(RFValue + i));
		}	
		else
		{
			for (i = 1; i <= (RFValue - RFValue2); i++)
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R17, (UCHAR)(RFValue - i));
		}
	}
	else
#endif // CONFIG_RALINK_RT3883 //
#endif // RTMP_RF_RW_SUPPORT //
	{
		// RT28xx
		// shift TX power control to correct RF register bit position
		R4 = pAd->ate.RFFreqOffset << 15;		
		R4 |= (pAd->LatchRfRegs.R4 & ((~0x001f8000)));
		pAd->LatchRfRegs.R4 = R4;
		
		RtmpRfIoWrite(pAd);
	}
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_FREQOFFSET_Proc (RFFreqOffset = %d)\n", pAd->ate.RFFreqOffset));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_FREQOFFSET_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE RF BW
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_BW_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	INT i;
	UCHAR value = 0;
	UCHAR BBPCurrentBW;
	
	BBPCurrentBW = simple_strtol(arg, 0, 10);

	if ((BBPCurrentBW == 0)
		)
	{
		pAd->ate.TxWI.BW = BW_20;
	}
	else
	{
		pAd->ate.TxWI.BW = BW_40;
 	}

#ifndef CONFIG_RALINK_RT3350
	/* RT35xx ATE will reuse this code segment. */
	// Fix the error spectrum of CCK-40MHZ.
	// Turn on BBP 20MHz mode by request here.
	if ((pAd->ate.TxWI.PHYMODE == MODE_CCK) && (pAd->ate.TxWI.BW == BW_40))
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_BW_Proc!! Warning!! CCK only supports 20MHZ!!\nBandwidth switch to 20\n"));
		pAd->ate.TxWI.BW = BW_20;
	}
#endif
	if (pAd->ate.TxWI.BW == BW_20)
	{
		if (pAd->ate.Channel <= 14)
		{
			for(i=0; i<MAX_TXPOWER_ARRAY_SIZE; i++)
			{
				if (pAd->Tx20MPwrCfgGBand[i] == 0xffffffff)
					continue;

				/* TX_PWR_CFG_1 ~ TX_PWR_CFG_4 */
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
				if ((i == 5) && (IS_RT2883(pAd) || IS_RT3883(pAd)))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, pAd->Tx20MPwrCfgGBand[i]);
				}
				else if ((i == 6) && (IS_RT2883(pAd) || IS_RT3883(pAd)))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, pAd->Tx20MPwrCfgGBand[i]);
				}
#if defined (CONFIG_RALINK_RT3883)
				else if (IS_RT3883(pAd) && (i == 7))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_7, pAd->Tx20MPwrCfgGBand[i]);
				}
				else if (IS_RT3883(pAd) && (i == 8))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_8, pAd->Tx20MPwrCfgGBand[i]);
				}
				else if (IS_RT3883(pAd) && (i == 9))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_9, pAd->Tx20MPwrCfgGBand[i]);
				}
#endif // CONFIG_RALINK_RT3883 //
				else
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, pAd->Tx20MPwrCfgGBand[i]);
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
					if (IS_RT2883(pAd) || IS_RT3883(pAd))
					{
						RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + i*4, (pAd->Tx20MPwrCfgGBand[i] & 0xf0f0f0f0) >> 4);
					}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
				}
				RTMPusecDelay(5000);				
			}
		}
		else
		{
			for(i=0; i<MAX_TXPOWER_ARRAY_SIZE; i++)
			{
				if (pAd->Tx20MPwrCfgABand[i] == 0xffffffff)
					continue;

				/* TX_PWR_CFG_1 ~ TX_PWR_CFG_4 */
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
				if ((i == 5) && (IS_RT2883(pAd) || IS_RT3883(pAd)))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, pAd->Tx20MPwrCfgABand[i]);
				}
				else if ((i == 6) && (IS_RT2883(pAd) || IS_RT3883(pAd)))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, pAd->Tx20MPwrCfgABand[i]);
				}
#if defined (CONFIG_RALINK_RT3883)
				else if (IS_RT3883(pAd) && (i == 7))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_7, pAd->Tx20MPwrCfgABand[i]);
				}
				else if (IS_RT3883(pAd) && (i == 8))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_8, pAd->Tx20MPwrCfgABand[i]);
				}
				else if (IS_RT3883(pAd) && (i == 9))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_9, pAd->Tx20MPwrCfgABand[i]);
				}
#endif // CONFIG_RALINK_RT3883 //
				else
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, pAd->Tx20MPwrCfgABand[i]);
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
					if (IS_RT2883(pAd) || IS_RT3883(pAd))
					{
						RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + i*4, (pAd->Tx20MPwrCfgABand[i] & 0xf0f0f0f0) >> 4);
					}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
				}
				RTMPusecDelay(5000);				
			}
		}
 
		// Set BBP R4 bit[4:3]=0:0
 		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &value);
 		value &= (~0x18);
 		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);



#ifndef CONFIG_RALINK_RT3883
		// Set BBP R66=0x3C
		value = 0x3C;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
		if (IS_RT2883(pAd) || IS_RT3883(pAd))
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, 0x0);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, value);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, 0x20);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, value);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, 0x40);
		}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, value);
#endif

		// Set BBP R68=0x0B
		// to improve Rx sensitivity.
		value = 0x0B;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R68, value);
		// Set BBP R69=0x16
		value = 0x16;
 		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, value);
		// Set BBP R70=0x08
		value = 0x08;
 		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, value);
		// Set BBP R73=0x11
		value = 0x11;
 		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, value);

	    /*
			If Channel=14, Bandwidth=20M and Mode=CCK, Set BBP R4 bit5=1
			(to set Japan filter coefficients).
			This segment of code will only works when ATETXMODE and ATECHANNEL
			were set to MODE_CCK and 14 respectively before ATETXBW is set to 0.
	    */
		if (pAd->ate.Channel == 14)
		{
			INT TxMode = pAd->ate.TxWI.PHYMODE;

			if (TxMode == MODE_CCK)
			{
				// when Channel==14 && Mode==CCK && BandWidth==20M, BBP R4 bit5=1
 				ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &value);
				value |= 0x20; //set bit5=1
 				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);				
			}
		}

#ifdef RT305x
		if (1)
		{
#ifndef CONFIG_RALINK_RT3350
			// set BW
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R24, (PUCHAR)&value);
			value &= 0xDF;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R24, (UCHAR)value);

			// Rx filter
			RT30xxWriteRFRegister(pAd, RF_R31, 0x0F);
#else
			// set RF_R24 Simba optimize the spectrum flatness
			if(pAd->ate.TxWI.PHYMODE == MODE_CCK)
				value = 0x1F;
			else if(pAd->ate.TxWI.PHYMODE == MODE_OFDM)
				value = 0x18;
		        else
				value = 0x10;

			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R24, (UCHAR)value);

			// Rx filter
			RT30xxWriteRFRegister(pAd, RF_R31, 0x48);
#endif // CONFIG_RALINK_RT3350 //
		}
		else
#endif // RT305x //
		// set BW = 20 MHz
		{
			pAd->LatchRfRegs.R4 &= ~0x00200000;
			RtmpRfIoWrite(pAd);
		}
	}
	// If bandwidth = 40M, set RF Reg4 bit 21 = 0.
	else if (pAd->ate.TxWI.BW == BW_40)
	{
		if (pAd->ate.Channel <= 14)
		{
			for(i=0; i<MAX_TXPOWER_ARRAY_SIZE; i++)
			{
				if (pAd->Tx40MPwrCfgGBand[i] == 0xffffffff)
					continue;

				/* TX_PWR_CFG_1 ~ TX_PWR_CFG_4 */
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
				if ((i == 5) && (IS_RT2883(pAd) || IS_RT3883(pAd)))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, pAd->Tx40MPwrCfgGBand[i]);
				}
				else if ((i == 6) && (IS_RT2883(pAd) || IS_RT3883(pAd)))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, pAd->Tx40MPwrCfgGBand[i]);
				}
#if defined (CONFIG_RALINK_RT3883)
				else if (IS_RT3883(pAd) && (i == 7))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_7, pAd->Tx40MPwrCfgGBand[i]);
				}
				else if (IS_RT3883(pAd) && (i == 8))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_8, pAd->Tx40MPwrCfgGBand[i]);
				}
				else if (IS_RT3883(pAd) && (i == 9))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_9, pAd->Tx40MPwrCfgGBand[i]);
				}
#endif // CONFIG_RALINK_RT3883 //
				else
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, pAd->Tx40MPwrCfgGBand[i]);
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
					if (IS_RT2883(pAd) || IS_RT3883(pAd))
					{
						RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + i*4, (pAd->Tx40MPwrCfgGBand[i] & 0xf0f0f0f0) >> 4);
					}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
				}
				RTMPusecDelay(5000);				
			}
		}
		else
		{
			for(i=0; i<MAX_TXPOWER_ARRAY_SIZE; i++)
			{
				if (pAd->Tx40MPwrCfgABand[i] == 0xffffffff)
					continue;

				/* TX_PWR_CFG_1 ~ TX_PWR_CFG_4 */
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
				if ((i == 5) && (IS_RT2883(pAd) || IS_RT3883(pAd)))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, pAd->Tx40MPwrCfgABand[i]);
				}
				else if ((i == 6) && (IS_RT2883(pAd) || IS_RT3883(pAd)))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, pAd->Tx40MPwrCfgABand[i]);
				}
#if defined (CONFIG_RALINK_RT3883)
				else if (IS_RT3883(pAd) && (i == 7))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_7, pAd->Tx40MPwrCfgABand[i]);
				}
				else if (IS_RT3883(pAd) && (i == 8))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_8, pAd->Tx40MPwrCfgABand[i]);
				}
				else if (IS_RT3883(pAd) && (i == 9))
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_9, pAd->Tx40MPwrCfgABand[i]);
				}
#endif // CONFIG_RALINK_RT3883 //
				else
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, pAd->Tx40MPwrCfgABand[i]);
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
					if (IS_RT2883(pAd) || IS_RT3883(pAd))
					{
						RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + i*4, (pAd->Tx40MPwrCfgABand[i] & 0xf0f0f0f0) >> 4);
					}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
				}
				RTMPusecDelay(5000);				
			}
#ifdef DOT11_N_SUPPORT
			if ((pAd->ate.TxWI.PHYMODE >= MODE_HTMIX) && (pAd->ate.TxWI.MCS == 7))
			{
    			value = 0x28;
    			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R67, value);
			}
#endif // DOT11_N_SUPPORT //
		}

		// Set BBP R4 bit[4:3]=1:0
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &value);
		value &= (~0x18);
		value |= 0x10;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);


#ifndef CONFIG_RALINK_RT3883
		// Set BBP R66=0x3C
		value = 0x3C;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
		if (IS_RT2883(pAd) || IS_RT3883(pAd))
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, 0x0);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, value);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, 0x20);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, value);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, 0x40);
		}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, value);
#endif

		// Set BBP R68=0x0C
		// to improve Rx sensitivity
		value = 0x0C;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R68, value);
		// Set BBP R69=0x1A
		value = 0x1A;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, value);
		// Set BBP R70=0x0A
		value = 0x0A;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, value);
		// Set BBP R73=0x16
		value = 0x16;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, value);

		// If bandwidth = 40M, set RF Reg4 bit 21 = 1.
#ifdef RT305x
		if (1)
		{
#ifndef CONFIG_RALINK_RT3350
			// set BW
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R24, (PUCHAR)&value);
			value &= 0xDF;
			value |= 0x20;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R24, (UCHAR)value);

			// Rx filter
			RT30xxWriteRFRegister(pAd, RF_R31, 0x2F);
#else
			// set RF_R24
			if(pAd->ate.TxWI.PHYMODE == MODE_CCK)
				value = 0x3F;
			else
				value = 0x28;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R24, (UCHAR)value);

			// Rx filter
			RT30xxWriteRFRegister(pAd, RF_R31, 0x68);
#endif // CONFIG_RALINK_RT3350 //
		}
		else
#endif // RT305x //
		// set BW = 40 MHz
		{
		pAd->LatchRfRegs.R4 |= 0x00200000;
		RtmpRfIoWrite(pAd);
		}
	}

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_BW_Proc (BBPCurrentBW = %d)\n", pAd->ate.TxWI.BW));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_BW_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Tx frame length
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_LENGTH_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	pAd->ate.TxLength = simple_strtol(arg, 0, 10);

	if ((pAd->ate.TxLength < 24) || (pAd->ate.TxLength > (MAX_FRAME_SIZE - 34/* == 2312 */)))
	{
		pAd->ate.TxLength = (MAX_FRAME_SIZE - 34/* == 2312 */);
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_LENGTH_Proc::Out of range, it should be in range of 24~%d.\n", (MAX_FRAME_SIZE - 34/* == 2312 */)));
		return FALSE;
	}

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_LENGTH_Proc (TxLength = %d)\n", pAd->ate.TxLength));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_LENGTH_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Tx frame count
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_COUNT_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	pAd->ate.TxCount = simple_strtol(arg, 0, 10);

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_COUNT_Proc (TxCount = %d)\n", pAd->ate.TxCount));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_COUNT_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Tx frame MCS
        
        Return:
        	TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_MCS_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg)
{
	UCHAR MCS;
	INT result;

	MCS = simple_strtol(arg, 0, 10);
	result = CheckMCSValid(pAd, pAd->ate.TxWI.PHYMODE, MCS);

	/* RT35xx ATE will reuse this code segment. */
		

	if (result != -1)
	{
		pAd->ate.TxWI.MCS = (UCHAR)MCS;
	}
	else
	{
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_MCS_Proc::Out of range, refer to rate table.\n"));
		return FALSE;
	}

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_MCS_Proc (MCS = %d)\n", pAd->ate.TxWI.MCS));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_MCS_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Tx frame Mode
        0: MODE_CCK
        1: MODE_OFDM
        2: MODE_HTMIX
        3: MODE_HTGREENFIELD
        
        Return:
        	TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_MODE_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg)
{
	UCHAR BbpData = 0;

	pAd->ate.TxWI.PHYMODE = simple_strtol(arg, 0, 10);

	if (pAd->ate.TxWI.PHYMODE > 3)
	{
		pAd->ate.TxWI.PHYMODE = 0;
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_MODE_Proc::Out of range.\nIt should be in range of 0~3\n"));
		ATEDBGPRINT(RT_DEBUG_ERROR, ("0: CCK, 1: OFDM, 2: HT_MIX, 3: HT_GREEN_FIELD.\n"));
		return FALSE;
	}

	// Turn on BBP 20MHz mode by request here.
	if (pAd->ate.TxWI.PHYMODE == MODE_CCK)
	{
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BbpData);
		BbpData &= (~0x18);
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, BbpData);
		pAd->ate.TxWI.BW = BW_20;
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_MODE_Proc::CCK Only support 20MHZ. Switch to 20MHZ.\n"));
	}


#ifdef CONFIG_RALINK_RT3350
	if (pAd->ate.TxWI.PHYMODE == MODE_CCK)
	{
		USHORT i;
	        USHORT value;
		UCHAR  rf_offset;
		UCHAR  rf_value;

		RT28xx_EEPROM_READ16(pAd, 0x126, value);
		rf_value = value & 0x00FF;
                rf_offset = (value & 0xFF00) >> 8;

		if(rf_offset == 0xff)
		    rf_offset = RF_R21;
		if(rf_value == 0xff)
		    rf_value = 0x4F;
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, rf_offset, (UCHAR)rf_value);
	
		RT28xx_EEPROM_READ16(pAd, 0x12a, value);
		rf_value = value & 0x00FF;
                rf_offset = (value & 0xFF00) >> 8;

		if(rf_offset == 0xff)
		    rf_offset = RF_R29;
		if(rf_value == 0xff)
		    rf_value = 0x07;
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, rf_offset, (UCHAR)rf_value);
	

		// set RF_R24
		if (pAd->ate.TxWI.BW == BW_40)
		{    
			value = 0x3F;
		}
		else
		{
			value = 0x1F;
		}
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R24, (UCHAR)value);


	}
	else
	{
		USHORT i;
	        USHORT value;
		UCHAR  rf_offset;
		UCHAR  rf_value;

		RT28xx_EEPROM_READ16(pAd, 0x124, value);
		rf_value = value & 0x00FF;
                rf_offset = (value & 0xFF00) >> 8;

		if(rf_offset == 0xff)
		    rf_offset = RF_R21;
		if(rf_value == 0xff)
		    rf_value = 0x6F;
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, rf_offset, (UCHAR)rf_value);
	
		RT28xx_EEPROM_READ16(pAd, 0x128, value);
		rf_value = value & 0x00FF;
                rf_offset = (value & 0xFF00) >> 8;

		if(rf_offset == 0xff)
		    rf_offset = RF_R29;
		if(rf_value == 0xff)
		    rf_value = 0x07;
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, rf_offset, (UCHAR)rf_value);
	
		// set RF_R24
		if (pAd->ate.TxWI.BW == BW_40)
		{    
			value = 0x28;
		}
		else
		{
			if(pAd->ate.TxWI.PHYMODE == MODE_OFDM)
			    value = 0x18;
			else
			    value = 0x10;
		}
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R24, (UCHAR)value);

	}
#endif


	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_MODE_Proc (TxMode = %d)\n", pAd->ate.TxWI.PHYMODE));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_MODE_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


/* 
==========================================================================
    Description:
        Set ATE Tx frame GI
        
        Return:
        	TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_TX_GI_Proc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			arg)
{
	pAd->ate.TxWI.ShortGI = simple_strtol(arg, 0, 10);

	if (pAd->ate.TxWI.ShortGI > 1)
	{
		pAd->ate.TxWI.ShortGI = 0;
		ATEDBGPRINT(RT_DEBUG_ERROR, ("Set_ATE_TX_GI_Proc::Out of range\n"));
		return FALSE;
	}

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_TX_GI_Proc (GI = %d)\n", pAd->ate.TxWI.ShortGI));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_TX_GI_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


INT	Set_ATE_RX_FER_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	pAd->ate.bRxFER = simple_strtol(arg, 0, 10);

	if (pAd->ate.bRxFER == 1)
	{
		pAd->ate.RxCntPerSec = 0;
		pAd->ate.RxTotalCnt = 0;
	}

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_RX_FER_Proc (bRxFER = %d)\n", pAd->ate.bRxFER));
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_RX_FER_Proc Success\n"));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	
	return TRUE;
}


INT Set_ATE_Read_RF_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
#ifdef RTMP_RF_RW_SUPPORT
	/* modify by WY for Read RF Reg. error */
	UCHAR RFValue;
	INT index=0;

//2008/07/10:KH add to support RT30xx ATE<--
	if (IS_RT30xx(pAd) || IS_RT3572(pAd))
	{
		for (index = 0; index < 32; index++)
		{
			ATE_RF_IO_READ8_BY_REG_ID(pAd, index, (PUCHAR)&RFValue);
			ate_print("R%d=%d\n",index,RFValue);
		}		
	}
	else
//2008/07/10:KH add to support RT30xx ATE-->
#ifdef RT305x
	if (1)
	{
		for (index = 0; index < 32; index++)
		{
			ATE_RF_IO_READ8_BY_REG_ID(pAd, index, (PUCHAR)&RFValue);
			ate_print("R%d=%d\n",index,RFValue);
		}		
	}
	else
#endif // RT305x //
#endif // RTMP_RF_RW_SUPPORT //
	{
		ate_print(KERN_EMERG "R1 = %lx\n", pAd->LatchRfRegs.R1);
		ate_print(KERN_EMERG "R2 = %lx\n", pAd->LatchRfRegs.R2);
		ate_print(KERN_EMERG "R3 = %lx\n", pAd->LatchRfRegs.R3);
		ate_print(KERN_EMERG "R4 = %lx\n", pAd->LatchRfRegs.R4);
	}
	return TRUE;
}


#ifndef RTMP_RF_RW_SUPPORT
INT Set_ATE_Write_RF1_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UINT32 value = (UINT32) simple_strtol(arg, 0, 16);	

		pAd->LatchRfRegs.R1 = value;
		RtmpRfIoWrite(pAd);

	return TRUE;
}


INT Set_ATE_Write_RF2_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UINT32 value = (UINT32) simple_strtol(arg, 0, 16);

		pAd->LatchRfRegs.R2 = value;
		RtmpRfIoWrite(pAd);

	return TRUE;
}


INT Set_ATE_Write_RF3_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UINT32 value = simple_strtol(arg, 0, 16);

		pAd->LatchRfRegs.R3 = value;
		RtmpRfIoWrite(pAd);

	return TRUE;
}


INT Set_ATE_Write_RF4_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UINT32 value = (UINT32) simple_strtol(arg, 0, 16);

		pAd->LatchRfRegs.R4 = value;
		RtmpRfIoWrite(pAd);

	return TRUE;
}
#endif // RTMP_RF_RW_SUPPORT //

/* 
==========================================================================
    Description:
        Load and Write EEPROM from a binary file prepared in advance.
        
        Return:
        	TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
#if defined(LINUX) || defined(VXWORKS)
INT Set_ATE_Load_E2P_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	BOOLEAN		    	ret = FALSE;
	PSTRING			src = EEPROM_DEFAULT_FILE_PATH;
	RTMP_OS_FD		srcf;
	INT32 			retval;
	USHORT 			WriteEEPROM[(EEPROM_SIZE/2)];
	INT				FileLength = 0;
	UINT32 			value = (UINT32) simple_strtol(arg, 0, 10);
	RTMP_OS_FS_INFO	osFSInfo;

	ATEDBGPRINT(RT_DEBUG_ERROR, ("===> %s (value=%d)\n\n", __FUNCTION__, value));

	if (value > 0)
	{
		/* zero the e2p buffer */
		NdisZeroMemory((PUCHAR)WriteEEPROM, EEPROM_SIZE);

		RtmpOSFSInfoChange(&osFSInfo, TRUE);

		do
		{
			/* open the bin file */
			srcf = RtmpOSFileOpen(src, O_RDONLY, 0);

			if (IS_FILE_OPEN_ERR(srcf)) 
			{
				ate_print("%s - Error opening file %s\n", __FUNCTION__, src);
				break;
			}

			/* read the firmware from the file *.bin */
			FileLength = RtmpOSFileRead(srcf, (PSTRING)WriteEEPROM, EEPROM_SIZE);

			if (FileLength != EEPROM_SIZE)
			{
				ate_print("%s: error file length (=%d) in e2p.bin\n",
					   __FUNCTION__, FileLength);
				break;
			}
			else
			{
				/* write the content of .bin file to EEPROM */
#if defined(RTMP_MAC_PCI) && defined(RTMP_PCI_SUPPORT)
                {
                    USHORT index=0;
                    USHORT value=0;

                    INT32 e2p_size=512;/* == 0x200 for PCI interface */
                    USHORT tempData=0;
                    for (index = 0 ; index < (e2p_size / 2); )
                    {
                        /* "value" is especially for some compilers... */
                        tempData = le2cpu16(WriteEEPROM[index]);
                        value = tempData;
                        RT28xx_EEPROM_WRITE16(pAd, index*2, value);
                        index ++;
                    }
                }
#else
				rt_ee_write_all(pAd, WriteEEPROM);
#endif /* defined(RTMP_MAC_PCI) && defined(RTMP_PCI_SUPPORT) */
				ret = TRUE;
			}
			break;
		} while(TRUE);

		/* close firmware file */
		if (IS_FILE_OPEN_ERR(srcf))
		{
				;
		}
		else
		{
			retval = RtmpOSFileClose(srcf);			

			if (retval)
			{
				ATEDBGPRINT(RT_DEBUG_ERROR, ("--> Error %d closing %s\n", -retval, src));
				
			} 
		}

		/* restore */
		RtmpOSFSInfoChange(&osFSInfo, FALSE);		
	}

    ATEDBGPRINT(RT_DEBUG_ERROR, ("<=== %s (ret=%d)\n", __FUNCTION__, ret));

    return ret;
	
}
#endif // defined(LINUX) || defined(VXWORKS) //




INT Set_ATE_Read_E2P_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	USHORT buffer[EEPROM_SIZE/2];
	USHORT *p;
	int i;
	
	rt_ee_read_all(pAd, (USHORT *)buffer);
	p = buffer;
	for (i = 0; i < (EEPROM_SIZE/2); i++)
	{
		ate_print("%4.4x ", *p);
		if (((i+1) % 16) == 0)
			ate_print("\n");
		p++;
	}
	return TRUE;
}


INT	Set_ATE_IPG_Proc(//peter
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
    UINT32           data, value;

	pAd->ate.IPG = simple_strtol(arg, 0, 10);
	value = pAd->ate.IPG;

	RTMP_IO_READ32(pAd, XIFS_TIME_CFG, &data);

    if (value <= 0)
	{
		ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_IPG_Proc::IPG is disabled(IPG == 0).\n"));
		return TRUE;
	}

	ASSERT(value > 0);

    if (0 < value < 256)
    {               
        RTMP_IO_READ32(pAd, EDCA_AC0_CFG, &data);
        data &= 0x0;
        RTMP_IO_WRITE32(pAd, EDCA_AC0_CFG, data);

        RTMP_IO_READ32(pAd, EDCA_AC1_CFG, &data);
        data &= 0x0;
        RTMP_IO_WRITE32(pAd, EDCA_AC1_CFG, data);

        RTMP_IO_READ32(pAd, EDCA_AC2_CFG, &data);
        data &= 0x0;
        RTMP_IO_WRITE32(pAd, EDCA_AC2_CFG, data);

        RTMP_IO_READ32(pAd, EDCA_AC3_CFG, &data);
        data &= 0x0;
        RTMP_IO_WRITE32(pAd, EDCA_AC3_CFG, data);
    }
    else
    {
        UINT32 aifsn, slottime;

        RTMP_IO_READ32(pAd, BKOFF_SLOT_CFG, &slottime);
        slottime &= 0x000000FF;

        aifsn = value / slottime;                  
        value = value % slottime;

        RTMP_IO_READ32(pAd, EDCA_AC0_CFG, &data);
        data &= 0x0;
        data |= (aifsn << 8);
        RTMP_IO_WRITE32(pAd, EDCA_AC0_CFG, data);

        RTMP_IO_READ32(pAd, EDCA_AC1_CFG, &data);
        data &= 0x0;
        data |= (aifsn << 8);
        RTMP_IO_WRITE32(pAd, EDCA_AC1_CFG, data);

        RTMP_IO_READ32(pAd, EDCA_AC2_CFG, &data);
        data &= 0x0;
        data |= (aifsn << 8);
        RTMP_IO_WRITE32(pAd, EDCA_AC2_CFG, data);

        RTMP_IO_READ32(pAd, EDCA_AC3_CFG, &data);
        data &= 0x0;
        data |= (aifsn << 8);
        RTMP_IO_WRITE32(pAd, EDCA_AC3_CFG, data);
    }

    data = (value & 0xFFFF0000) | value | (value << 8);
	RTMP_IO_WRITE32(pAd, XIFS_TIME_CFG, data);
	
	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_IPG_Proc (IPG = %u)\n", pAd->ate.IPG));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_IPG_Proc Success\n"));
	
	return TRUE;
}

/* 
==========================================================================
    Description:
        Set ATE payload pattern for TxFrame
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	Set_ATE_Payload_Proc(//peter
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	PSTRING				value;

	value = arg;

	// only one octet acceptable	
	if (strlen(value) != 2)  
		return FALSE;

	AtoH(value, &(pAd->ate.Payload), 1);

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Set_ATE_Payload_Proc (repeated pattern = 0x%2x)\n", pAd->ate.Payload));

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("Ralink: Set_ATE_Payload_Proc Success\n"));
	
	return TRUE;
}
INT	Set_ATE_Show_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ate_print("Mode=%u\n", pAd->ate.Mode);
#ifdef CONFIG_RALINK_RT3350
	ate_print("PABias=%u\n", pAd->ate.PABias);//peter
#endif // CONFIG_RALINK_RT3350 //
	ate_print("TxPower0=%d\n", pAd->ate.TxPower0);
	ate_print("TxPower1=%d\n", pAd->ate.TxPower1);
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	ate_print("TxPower2=%d\n", pAd->ate.TxPower2);
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
	ate_print("TxAntennaSel=%d\n", pAd->ate.TxAntennaSel);
	ate_print("RxAntennaSel=%d\n", pAd->ate.RxAntennaSel);
	ate_print("BBPCurrentBW=%u\n", pAd->ate.TxWI.BW);
	ate_print("GI=%u\n", pAd->ate.TxWI.ShortGI);
	ate_print("MCS=%u\n", pAd->ate.TxWI.MCS);
	ate_print("TxMode=%u\n", pAd->ate.TxWI.PHYMODE);
	ate_print("Addr1=%02x:%02x:%02x:%02x:%02x:%02x\n",
		pAd->ate.Addr1[0], pAd->ate.Addr1[1], pAd->ate.Addr1[2], pAd->ate.Addr1[3], pAd->ate.Addr1[4], pAd->ate.Addr1[5]);
	ate_print("Addr2=%02x:%02x:%02x:%02x:%02x:%02x\n",
		pAd->ate.Addr2[0], pAd->ate.Addr2[1], pAd->ate.Addr2[2], pAd->ate.Addr2[3], pAd->ate.Addr2[4], pAd->ate.Addr2[5]);
	ate_print("Addr3=%02x:%02x:%02x:%02x:%02x:%02x\n",
		pAd->ate.Addr3[0], pAd->ate.Addr3[1], pAd->ate.Addr3[2], pAd->ate.Addr3[3], pAd->ate.Addr3[4], pAd->ate.Addr3[5]);
	ate_print("Channel=%u\n", pAd->ate.Channel);
	ate_print("TxLength=%u\n", pAd->ate.TxLength);
	ate_print("TxCount=%u\n", pAd->ate.TxCount);
	ate_print("RFFreqOffset=%u\n", pAd->ate.RFFreqOffset);
	ate_print("IPG=%u\n", pAd->ate.IPG);//peter
	ate_print("Payload=0x%02x\n", pAd->ate.Payload);//peter
	ate_print(KERN_EMERG "Set_ATE_Show_Proc Success\n");
#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //
	return TRUE;
}


INT	Set_ATE_Help_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
#ifdef	CONFIG_RT2880_ATE_CMD_NEW
	ate_print("ATE=ATESTART, ATESTOP, TXCONT, TXCARR, TXFRAME, RXFRAME\n");
#else
	ate_print("ATE=APSTOP, APSTART, TXCONT, TXCARR, TXFRAME, RXFRAME\n");
#endif
	ate_print("ATEDA\n");
	ate_print("ATESA\n");
	ate_print("ATEBSSID\n");
	ate_print("ATECHANNEL, range:0~14(unless A band !)\n");
	ate_print("ATETXPOW0, set power level of antenna 1.\n");
	ate_print("ATETXPOW1, set power level of antenna 2.\n");
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
	{
		ate_print("ATETXPOW2, set power level of antenna 3.\n");
		ate_print("ATETXANT, set TX antenna. 0:all, 1:antenna one, 2:antenna two, 3:antenna three.\n");
	}
	else
	{
		ate_print("ATETXANT, set TX antenna. 0:all, 1:antenna one, 2:antenna two.\n");
	}
#else
	ate_print("ATETXANT, set TX antenna. 0:all, 1:antenna one, 2:antenna two.\n");
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
	ate_print("ATERXANT, set RX antenna.0:all, 1:antenna one, 2:antenna two, 3:antenna three.\n");

#ifdef CONFIG_RALINK_RT3350
	ate_print("ATEPABIAS, set power amplifier bias for EVM, range 0~15\n");//peter
#endif // CONFIG_RALINK_RT3350 //
#ifdef RTMP_RF_RW_SUPPORT
	ate_print("ATETXFREQOFFSET, set frequency offset, range 0~95\n");
#else
	ate_print("ATETXFREQOFFSET, set frequency offset, range 0~63\n");
#endif // RTMP_RF_RW_SUPPORT //
	ate_print("ATETXBW, set BandWidth, 0:20MHz, 1:40MHz.\n");
	ate_print("ATETXLEN, set Frame length, range 24~%d\n", (MAX_FRAME_SIZE - 34/* == 2312 */));
	ate_print("ATETXCNT, set how many frame going to transmit.\n");
	ate_print("ATETXMCS, set MCS, reference to rate table.\n");
	ate_print("ATETXMODE, set Mode 0:CCK, 1:OFDM, 2:HT-Mix, 3:GreenField, reference to rate table.\n");
	ate_print("ATETXGI, set GI interval, 0:Long, 1:Short\n");
	ate_print("ATERXFER, 0:disable Rx Frame error rate. 1:enable Rx Frame error rate.\n");
	ate_print("ATERRF, show all RF registers.\n");
#ifndef RTMP_RF_RW_SUPPORT
	ate_print("ATEWRF1, set RF1 register.\n");
	ate_print("ATEWRF2, set RF2 register.\n");
	ate_print("ATEWRF3, set RF3 register.\n");
	ate_print("ATEWRF4, set RF4 register.\n");
#endif // !RTMP_RF_RW_SUPPORT //
	ate_print("ATELDE2P, load EEPROM from .bin file.\n");
	ate_print("ATERE2P, display all EEPROM content.\n");
	ate_print("ATEIPG, set ATE Tx frame IPG.\n");//peter
	ate_print("ATEPAYLOAD, set ATE payload pattern for TxFrame.\n");//peter
	ate_print("ATESHOW, display all parameters of ATE.\n");
	ate_print("ATEHELP, online help.\n");

	return TRUE;
}



/*
==========================================================================
    Description:

	AsicSwitchChannel() dedicated for ATE.
    
==========================================================================
*/
#ifdef RT3883 
extern UCHAR NUM_OF_3883_CHNL; 
extern FREQUENCY_ITEM FreqItems3883[];
#endif // RT3883 //

VOID ATEAsicSwitchChannel(
    IN PRTMP_ADAPTER pAd) 
{
	UINT32 R2 = 0, R3 = DEFAULT_RF_TX_POWER, R4 = 0, Value = 0;
	CHAR TxPwer = 0, TxPwer2 = 0;
	UCHAR index = 0, BbpValue = 0, R66 = 0x30;
	RTMP_RF_REGS *RFRegTable = NULL;
	UCHAR Channel = 0;
#ifdef RTMP_RF_RW_SUPPORT
	/* added to prevent RF register reading error */
	UCHAR RFValue = 0, RFValue2 = 0;
#endif // RTMP_RF_RW_SUPPORT //

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	CHAR TxPwer3 = 0;
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

#ifdef RALINK_28xx_QA
	// for QA mode, TX power values are passed from UI
	if ((pAd->ate.bQATxStart == TRUE) || (pAd->ate.bQARxStart == TRUE))
	{
		if (pAd->ate.Channel != pAd->LatchRfRegs.Channel)			
		{
			pAd->ate.Channel = pAd->LatchRfRegs.Channel;
		}
		return;
	}
	else
#endif // RALINK_28xx_QA //
	Channel = pAd->ate.Channel;

#ifdef ANT_DIVERSITY_SUPPORT
	// select antenna for RT3090
	AsicAntennaSelect(pAd, Channel);
#endif // ANT_DIVERSITY_SUPPORT //

	// fill Tx power value
	TxPwer = pAd->ate.TxPower0;
	TxPwer2 = pAd->ate.TxPower1;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
		TxPwer3 = pAd->ate.TxPower2;
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

#ifdef RT3883
	if (IS_RT3883(pAd))
	{
		RTMPRT3883ABandSel(Channel);
		for (index = 0; index < NUM_OF_3883_CHNL; index++)
		{
			if (Channel == FreqItems3883[index].Channel)
			{
				int i;

				// programming channel parameters
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R08, FreqItems3883[index].N);
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R09, FreqItems3883[index].K);
		
#if 0
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R11, &RFValue);
				RFValue &= 0xF0;
				RFValue |= (FreqItems3883[index].R & 0x0F);				
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R11, RFValue);
#else
				if  (Channel <= 14)
					RFValue = 0x46;
				else
					RFValue = 0x48;
				RT30xxWriteRFRegister(pAd, RF_R11, (UCHAR)RFValue);
#endif

				if  (Channel <= 14)
					RFValue = 0x4E;
				else
					RFValue = 0x52;
				RT30xxWriteRFRegister(pAd, RF_R12, (UCHAR)RFValue);

				RFValue = 0x12;
				RT30xxWriteRFRegister(pAd, RF_R13, (UCHAR)RFValue);

				// antenna selection
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R01, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0x03) | 0x01;
				if (pAd->ate.RxAntennaSel == 1)
				{
					RFValue = RFValue | 0x04;
				}
				else if (pAd->ate.RxAntennaSel == 2)
				{
					RFValue = RFValue | 0x10;
				}
				else if (pAd->ate.RxAntennaSel == 3)
				{
					RFValue = RFValue | 0x40;
				}
				else 
				{
					RFValue = RFValue | 0x54;
				}
		
				if (pAd->ate.TxAntennaSel == 1)
				{
					RFValue = RFValue | 0x08;
				}
				else if (pAd->ate.TxAntennaSel == 2)
				{
					RFValue = RFValue | 0x20;
				}
				else if (pAd->ate.TxAntennaSel == 3)
				{
					RFValue = RFValue | 0x80;
				}
				else
				{
					RFValue = RFValue | 0xA8;
				}
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R01, (UCHAR)RFValue);

				ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
				BbpValue = BbpValue & 0xE4; // clear bit 0,1,3,4
				if (pAd->ate.RxAntennaSel == 1)
				{
					BbpValue |= 0x0;
				}
				else if (pAd->ate.RxAntennaSel == 2)
				{
					BbpValue |= 0x1;
				}
				else if (pAd->ate.RxAntennaSel == 3)
				{
					BbpValue |= 0x2;
				}
				else
				{
					// assume that all RxAntenna are enabled
					BbpValue |= 0x3;
					BbpValue |= 0x10;
				}
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);

				/* calibration power unbalance issues */
				if (pAd->Antenna.field.TxPath >= 2)
				{
					if (pAd->ate.TxAntennaSel == 1)
					{
						ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R164, &BbpValue);
						BbpValue &= 0xF3;
						ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R164, BbpValue);
					}
					else if (pAd->ate.TxAntennaSel == 2)
					{
						ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R164, &BbpValue);
						BbpValue &= 0xF3;	
						BbpValue |= 0x04;
						ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R164, BbpValue);
					}
					else if (pAd->ate.TxAntennaSel == 3)
					{
						ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R164, &BbpValue);
						BbpValue &= 0xF3;	
						BbpValue |= 0x08;
						ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R164, BbpValue);
					}
					else
					{
						ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R164, &BbpValue);
						BbpValue &= 0xF3;
						BbpValue |= 0x0C;
						ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R164, BbpValue);
					}
				}
					
				// set RF offset  RF_R17=RF_R23 (RT30xx)
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R17, (PUCHAR)&RFValue);
				RFValue2 = (RFValue & 0x80) | pAd->ate.RFFreqOffset;
				if (RFValue2 > RFValue)
				{
					for (i = 1; i <= (RFValue2 - RFValue); i++)
						ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R17, (UCHAR)(RFValue + i));
				}	
				else
				{
					for (i = 1; i <= (RFValue - RFValue2); i++)
						ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R17, (UCHAR)(RFValue - i));
				}

				// different default setting for A/BG bands
				RT30xxReadRFRegister(pAd, RF_R30, (PUCHAR)&RFValue);
				if (pAd->ate.TxWI.BW == BW_20)
					RFValue &= ~(0x06); // 20MBW Bit[2:1]=0,0
				else
					RFValue |= 0x06;
				RT30xxWriteRFRegister(pAd, RF_R30, (UCHAR)RFValue);
	
				if  (Channel <= 14)
					RFValue = 0x3C;
				else
					RFValue = 0x20;
				RT30xxWriteRFRegister(pAd, RF_R34, (UCHAR)RFValue);

				// loopback RF_BS
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R36, (PUCHAR)&RFValue);
				RFValue &= ~(0x1 << 7);
				if (Channel <= 14)
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R36, (UCHAR)(RFValue  | (0x1 << 7)));
				else
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R36, (UCHAR)RFValue);

				// RF_R39 for A-Band L:0x3e M:0x34 H:0x3a
				if (Channel > 14)
				{
					if (Channel <= 64)
					{
						RFValue = 0x3E;
					}
					else if (Channel <= 128)
					{
						RFValue = 0x34;
					}
					else
					{
						RFValue = 0x3A;
					}
					RT30xxWriteRFRegister(pAd, RF_R39, (UCHAR)RFValue);
				}
		
				// loopback RF_BS
				if  (Channel <= 14)
					RFValue = 0x93;
				else
					RFValue = 0x9F;
				RT30xxWriteRFRegister(pAd, RF_R44, (UCHAR)RFValue);


				// RF_R45 for A-Band L:0xEB M:0xBB H:0xBB
				if (Channel > 14)
				{
					if (Channel <= 64)
					{
						RFValue = 0xEB;
					}
					else
					{
						RFValue = 0xBB;
					}
					RT30xxWriteRFRegister(pAd, RF_R45, (UCHAR)RFValue);
				}
				
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R49, (PUCHAR)&RFValue);
				if  (Channel <= 14)
					RFValue = 0x8E;
				else
					RFValue = 0x8A;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R49, (UCHAR)RFValue);
			
				RFValue = 0x86;
				RT30xxWriteRFRegister(pAd, RF_R50, (UCHAR)RFValue);
		
				// tx_mx1_ic
				RT30xxReadRFRegister(pAd, RF_R51, (PUCHAR)&RFValue);
				if  (Channel <= 14)
					RFValue = 0x75;
				else
					RFValue = 0x51;
				RT30xxWriteRFRegister(pAd, RF_R51, (UCHAR)RFValue);

				RT30xxReadRFRegister(pAd, RF_R52, (PUCHAR)&RFValue);
				if  (Channel <= 14)
					RFValue = 0x45;
				else
					RFValue = 0x05;
				RT30xxWriteRFRegister(pAd, RF_R52, (UCHAR)RFValue);

				// tx0, tx1, tx2 (0.5db)
				if (Channel <= 14)
				{
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R53, pAd->ate.TxPower0);
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R54, pAd->ate.TxPower1);
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R55, pAd->ate.TxPower2);
				}
				else
				{
					CHAR power = 0x50 | ((pAd->ate.TxPower0 & 0x10) << 1) | (pAd->ate.TxPower0 & 0xf);
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R53, power);
					power = 0x50 | ((pAd->ate.TxPower1 & 0x10) << 1) | (pAd->ate.TxPower1 & 0xf);
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R54, power);
					power = 0x50 | ((pAd->ate.TxPower2 & 0x10) << 1) | (pAd->ate.TxPower2 & 0xf);
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R55, power);
				}

#if 0
				// tx0, tx1 (0.1db)
				BbpValue = (pAd->TxPower[index].Power >> 5) | ((pAd->TxPower[index].Power2 & 0xE0) >> 1);
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R109, BbpValue);

				// tx2 (0.1db)
				ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R110, &BbpValue);
				BbpValue = ((pAd->TxPower[index].Power3 & 0xE0) >> 1) | (BbpValue & 0x0F);
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R110, BbpValue);
#endif

				RT30xxReadRFRegister(pAd, RF_R57, (PUCHAR)&RFValue);
				if  (Channel <= 14)
					RFValue = 0x6E;
				else
					RFValue = 0x3E;
				RT30xxWriteRFRegister(pAd, RF_R57, (UCHAR)RFValue);

				// Enable RF tuning, this must be in the last, RF_R03=RF_R07 (RT30xx).
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R03, (PUCHAR)&RFValue);
				RFValue = RFValue | 0x80; // bit 7=vcocal_en
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R03, (UCHAR)RFValue);

				RTMPusecDelay(2000);

				// latch channel for future usage
				pAd->LatchRfRegs.Channel = Channel;
				
				break;
			}
		}
	}
	else
#endif // RT3883 //

#ifdef RT305x
	if ((pAd->RfIcType == RFIC_3022) || (pAd->RfIcType == RFIC_3021) || (pAd->RfIcType == RFIC_3020) || (pAd->RfIcType == RFIC_3320))
	{
		/* added to prevent RF register reading error */
		UCHAR RFValue = 0;

		for (index = 0; index < NUM_OF_3020_CHNL; index++)
		{
			if (Channel == FreqItems3020[index].Channel)
			{
				// programming channel parameters
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R02, FreqItems3020[index].N);
#ifndef CONFIG_RALINK_RT3350
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R03, FreqItems3020[index].K);
#else
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R03, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0xF0) | (FreqItems3020[index].K & 0x0F);
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R03, (UCHAR)RFValue);
#endif // CONFIG_RALINK_RT3350 //
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R06, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0xFC) | FreqItems3020[index].R;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R06, (UCHAR)RFValue);

				// set tx power0
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R12, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0xE0) | TxPwer;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R12, (UCHAR)RFValue);

				// set tx power1
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R13, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0xE0) | TxPwer2;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R13, (UCHAR)RFValue);

				// set RF offset
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R23, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0x80) | pAd->ate.RFFreqOffset;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R23, (UCHAR)RFValue);

#ifndef CONFIG_RALINK_RT3350
				// set BW
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R24, (PUCHAR)&RFValue);
				RFValue &= 0xDF;
				if (pAd->ate.TxWI.BW == BW_40)
				{
					RFValue |= 0x20;
				}
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R24, (UCHAR)RFValue);
#else
				// set RF_R24

				if (pAd->ate.TxWI.BW == BW_40)
				{    
					if(pAd->ate.TxWI.PHYMODE == MODE_CCK)
						RFValue = 0x3F;
					else
						RFValue = 0x28;
				}
				else
				{
					if(pAd->ate.TxWI.PHYMODE == MODE_CCK)
						RFValue = 0x1F;
					else if(pAd->ate.TxWI.PHYMODE == MODE_OFDM)
						RFValue = 0x18;
					else  
					        RFValue = 0x10;
				}
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R24, (UCHAR)RFValue);
#endif
				// enable RF tuning
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R07, (PUCHAR)&RFValue);
				RFValue = RFValue | 0x1;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R07, (UCHAR)RFValue);

				// antenna selection
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R01, (PUCHAR)&RFValue);
				RFValue = (RFValue & 0x03) | 0xC1;//peter
				if (pAd->ate.RxAntennaSel == 1)
				{
					RFValue = RFValue | 0x10;
				}
				else if (pAd->ate.RxAntennaSel == 2)
				{
					RFValue = RFValue | 0x04;
				}
				
				if (pAd->ate.TxAntennaSel == 1)
				{
					RFValue = RFValue | 0x20;
				}
				else if (pAd->ate.TxAntennaSel == 2)
				{
					RFValue = RFValue | 0x08;
				}
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R01, (UCHAR)RFValue);

				if (pAd->ate.RxAntennaSel == 1)
				{
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, 0x0);
				}
				else if (pAd->ate.RxAntennaSel == 2)
				{
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, 0x1);
				}
				else
				{
					// RXANT_NUM >= 2, must turn on Bit 0 and 1 for all ADCs
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, 0xB);
				}

#ifndef CONFIG_RALINK_RT3350
				/* calibration power unbalance issues */
				if (pAd->Antenna.field.TxPath == 2)
				{
					if (pAd->ate.TxAntennaSel == 1)
					{
						ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
						BbpValue &= 0xE7;		// 11100111B
						ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
					}
					else if (pAd->ate.TxAntennaSel == 2)
					{
						ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
						BbpValue &= 0xE7;	
						BbpValue |= 0x08;
						ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
					}
					else
					{
						ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
						BbpValue &= 0xE7;
						BbpValue |= 0x10;
						ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
					}
				}
#endif // CONFIG_RALINK_RT3350	//					

				// enable RF tuning
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R07, (PUCHAR)&RFValue);
				RFValue = RFValue | 0x1;
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R07, (UCHAR)RFValue);
			}
		}
		
		pAd->LatchRfRegs.Channel = Channel;
	}
	else
#endif // RT305x //
	{
		/* RT28xx */
		RFRegTable = RF2850RegTable;

		switch (pAd->RfIcType)
		{
			/* But only 2850 and 2750 support 5.5GHz band... */
			case RFIC_2820:
			case RFIC_2850:
			case RFIC_2720:
			case RFIC_2750:
#ifdef RT2883
			case RFIC_2853:
#endif // RT2883 //
				for (index = 0; index < NUM_OF_2850_CHNL; index++)
				{
					if (Channel == RFRegTable[index].Channel)
					{
						R2 = RFRegTable[index].R2;

						// If TX path is 1, bit 14 = 1;
						if (pAd->Antenna.field.TxPath == 1)
						{
							R2 |= 0x4000;	
						}

						if (pAd->Antenna.field.RxPath == 2)
						{
							switch (pAd->ate.RxAntennaSel)
							{
								case 1:
									R2 |= 0x20040;
									ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
									BbpValue &= 0xE4;
									BbpValue |= 0x00;
									ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								
									break;
								case 2:
									R2 |= 0x10040;
									ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
									BbpValue &= 0xE4;
									BbpValue |= 0x01;
									ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);									
									break;
								default:	
									R2 |= 0x40;
									ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
									BbpValue &= 0xE4;
									/* Only enable two Antenna to receive. */
									BbpValue |= 0x08;
									ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								
									break;
							}
						}
						else if (pAd->Antenna.field.RxPath == 1)
						{
							// write 1 to off RxPath
							R2 |= 0x20040;	
						}
#ifdef RT2883
					if (IS_RT2883(pAd))
					{
						if (pAd->Antenna.field.TxPath < 3)
						{
							R2 |= 0x2000;// write 1 to bit  13
						}
					}
#endif // RT2883 //
						if (pAd->Antenna.field.TxPath == 2)
						{
							if (pAd->ate.TxAntennaSel == 1)
							{
								// If TX Antenna select is 1 , bit 14 = 1; Disable Ant 2
								R2 |= 0x4000;	
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
								BbpValue &= 0xE7;		// 11100111B
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
							}
							else if (pAd->ate.TxAntennaSel == 2)
							{
								// If TX Antenna select is 2 , bit 15 = 1; Disable Ant 1
								R2 |= 0x8000;	
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
								BbpValue &= 0xE7;	
								BbpValue |= 0x08;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
							}
							else
							{
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
								BbpValue &= 0xE7;
								BbpValue |= 0x10;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
							}
						}
					
					
					if (IS_RT2883(pAd))
					{
						if (pAd->Antenna.field.TxPath == 3)
						{
							if (pAd->ate.TxAntennaSel == 1)
							{
								// If TX Antenna select is 1 , bit 14 = 1; Disable Ant 2
								R2 |= 0x4000;	
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
								BbpValue &= 0xE7;		//11100111B
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
							}
							else if (pAd->ate.TxAntennaSel == 2)
							{
								// If TX Antenna select is 2 , bit 15 = 1; Disable Ant 1
								R2 |= 0x8000;	
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
								BbpValue &= 0xE7;	
								BbpValue |= 0x08;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
							}
							else if (pAd->ate.TxAntennaSel == 3)
							{
								// If TX Antenna select is 2 , bit 15 = 1; Disable Ant 1
								R2 |= 0x8000;	
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
								BbpValue &= 0xE7;	
								BbpValue |= 0x08;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
							}
							else
							{
								ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
								BbpValue &= 0xE7;
								BbpValue |= 0x10;
								ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);
							}
						}
					}
					
					
					
						if (pAd->Antenna.field.RxPath == 3)
						{
							switch (pAd->ate.RxAntennaSel)
							{
								case 1:
									R2 |= 0x20040;
									ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
									BbpValue &= 0xE4;
									BbpValue |= 0x00;
									ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								
									break;
								case 2:
									R2 |= 0x10040;
									ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
									BbpValue &= 0xE4;
									BbpValue |= 0x01;
									ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);									
									break;
								case 3:	
									R2 |= 0x30000;
									ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
									BbpValue &= 0xE4;
									BbpValue |= 0x02;
									ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);
									break;								
								default:	
									ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
									BbpValue &= 0xE4;
									BbpValue |= 0x10;
									ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								
									break;
							}
						}
						
						if (Channel > 14)
						{
							// initialize R3, R4
							R3 = (RFRegTable[index].R3 & 0xffffc1ff);
							R4 = (RFRegTable[index].R4 & (~0x001f87c0)) | (pAd->ate.RFFreqOffset << 15);

		                    /*
			                    According the Rory's suggestion to solve the middle range issue.

								5.5G band power range : 0xF9~0X0F, TX0 Reg3 bit9/TX1 Reg4 bit6="0"
														means the TX power reduce 7dB.
							*/
							// R3
							if ((TxPwer >= -7) && (TxPwer < 0))
							{
								TxPwer = (7+TxPwer);
								TxPwer = (TxPwer > 0xF) ? (0xF) : (TxPwer);
								R3 |= (TxPwer << 10);
								ATEDBGPRINT(RT_DEBUG_TRACE, ("ATEAsicSwitchChannel: TxPwer=%d \n", TxPwer));
							}
							else
							{
								TxPwer = (TxPwer > 0xF) ? (0xF) : (TxPwer);
								R3 |= (TxPwer << 10) | (1 << 9);
							}

							// R4
							if ((TxPwer2 >= -7) && (TxPwer2 < 0))
							{
								TxPwer2 = (7+TxPwer2);
								TxPwer2 = (TxPwer2 > 0xF) ? (0xF) : (TxPwer2);
								R4 |= (TxPwer2 << 7);
								ATEDBGPRINT(RT_DEBUG_TRACE, ("ATEAsicSwitchChannel: TxPwer2=%d \n", TxPwer2));
							}
							else
							{
								TxPwer2 = (TxPwer2 > 0xF) ? (0xF) : (TxPwer2);
								R4 |= (TxPwer2 << 7) | (1 << 6);
							}
#ifdef RT2883
						if (IS_RT2883(pAd))
							{
								RFRegTable[index].R1 &= 0xff8fffcf; // clear bit 4,5,20,21,22
								// R1
								if ((TxPwer3 >= -7) && (TxPwer3 < 0))
								{
									TxPwer3 = (7+TxPwer3);
									TxPwer3 = (TxPwer3 > 0xF) ? (0xF) : (TxPwer3);
									RFRegTable[index].R1 |= (((TxPwer3 & 0xe) << 19) | ((TxPwer3 & 0x1) << 5));
								}
								else
								{
									TxPwer3 = (TxPwer3 > 0xF) ? (0xF) : (TxPwer3);
									RFRegTable[index].R1 |= (((TxPwer3 & 0xe) << 19) | ((TxPwer3 & 0x1) << 5)) | (1 << 4);
								}
								RFRegTable[index].R1 &= 0xfffffdff;
							}
#endif // RT2883 //
						}
						else
						{
							// Set TX power0.
							R3 = (RFRegTable[index].R3 & 0xffffc1ff) | (TxPwer << 9);
							// Set frequency offset and TX power1.
							R4 = (RFRegTable[index].R4 & (~0x001f87c0)) | (pAd->ate.RFFreqOffset << 15) | (TxPwer2 <<6);

#ifdef RT2883
						if (IS_RT2883(pAd))
							{
								RFRegTable[index].R1 &= 0xff8fffcf; // clear bit 4,5,20,21,22
								// R1
								RFRegTable[index].R1 |= (((TxPwer3 & 0x1c) << 18) | ((TxPwer3 & 0x3) << 4));
								RFRegTable[index].R1 &= 0xfffffdff;
							}
#endif // RT2883 //
						}

						// based on BBP current mode before changing RF channel
						if (pAd->ate.TxWI.BW == BW_40)
						{
							R4 |=0x200000;
						}
						
						// Update variables.
						pAd->LatchRfRegs.Channel = Channel;
						pAd->LatchRfRegs.R1 = RFRegTable[index].R1;
						pAd->LatchRfRegs.R2 = R2;
						pAd->LatchRfRegs.R3 = R3;
						pAd->LatchRfRegs.R4 = R4;

						RtmpRfIoWrite(pAd);
						
						break;
					}
				}
				break;

			default:
				break;
		}
	}

	// Change BBP setting during siwtch from a->g, g->a
	if (Channel <= 14)
	{
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	    ULONG	TxPinCfg = 0x32050F0A;//Gary 2007/08/09 0x050A0A
#else
	    ULONG	TxPinCfg = 0x00050F0A;//Gary 2007/08/09 0x050A0A
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, (0x37 - GET_LNA_GAIN(pAd)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R63, (0x37 - GET_LNA_GAIN(pAd)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, (0x37 - GET_LNA_GAIN(pAd)));
#if defined (CONFIG_RALINK_RT3883)
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R86, 0x46);	// for peak throughput, Henry 2009-12-23
#else
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R86, 0);//(0x44 - GET_LNA_GAIN(pAd)));	// According the Rory's suggestion to solve the middle range issue.
#endif
		//ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x62);

		// Rx High power VGA offset for LNA select
		if (pAd->NicConfig2.field.ExternalLNAForG)
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x62);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R75, 0x46);
		}
		else
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x84);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R75, 0x50);
		}

		// 5G band selection PIN, bit1 and bit2 are complement
		RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
		Value &= (~0x6);
		Value |= (0x04);
		RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);

		// Turn off unused PA or LNA when only 1T or 1R
		if (pAd->Antenna.field.TxPath == 1)
		{
			TxPinCfg &= 0xFFFFFFF3;
		}
		if (pAd->Antenna.field.RxPath == 1)
		{
			TxPinCfg &= 0xFFFFF3FF;
		}


		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);

	}
	else
	{
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	    ULONG	TxPinCfg = 0x31050F05;//Gary 2007/8/9 0x050505
#else
	    ULONG	TxPinCfg = 0x00050F05;//Gary 2007/8/9 0x050505
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, (0x37 - GET_LNA_GAIN(pAd)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R63, (0x37 - GET_LNA_GAIN(pAd)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, (0x37 - GET_LNA_GAIN(pAd)));
#if defined (CONFIG_RALINK_RT3883)
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R86, 0x46);	// Henry 2009-12-16
#else
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R86, 0);//(0x44 - GET_LNA_GAIN(pAd)));   // According the Rory's suggestion to solve the middle range issue.     
#endif
#if defined (CONFIG_RALINK_RT2883)
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x62);
#elif defined (CONFIG_RALINK_RT3883)
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0x82);	// Henry 2009-12-16
#else
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R82, 0xF2);
#endif

		// Rx High power VGA offset for LNA select
		if (pAd->NicConfig2.field.ExternalLNAForA)
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R75, 0x46);
		}
		else
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R75, 0x50);
		}

		// 5G band selection PIN, bit1 and bit2 are complement
		RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
		Value &= (~0x6);
		Value |= (0x02);
		RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);

		// Turn off unused PA or LNA when only 1T or 1R
		if (pAd->Antenna.field.TxPath == 1)
		{
			TxPinCfg &= 0xFFFFFFF3;
		}
		if (pAd->Antenna.field.RxPath == 1)
		{
			TxPinCfg &= 0xFFFFF3FF;
		}		


		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);

	}

	// R66 should be set according to Channel and use 20MHz when scanning
	if (Channel <= 14)
	{	
		// BG band
#if defined (CONFIG_RALINK_RT3883)
		if (pAd->ate.TxWI.BW == BW_20)
			R66 = 0x30;
		else
			R66 = 0x38;
#else
		R66 = 0x2E + GET_LNA_GAIN(pAd);
#endif

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
		if (IS_RT2883(pAd) || IS_RT3883(pAd))
		{
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &BbpValue);
			BbpValue &= 0x9f;
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, BbpValue);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (BbpValue | (1 << 5)));
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (BbpValue | (1 << 6)));
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);			
		}
		else
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
	}
	else
	{	
		// 5.5 GHz band
		if (pAd->ate.TxWI.BW == BW_20)
		{
#ifdef CONFIG_RALINK_RT3883
			//R66 = (UCHAR)(0x20 + (GET_LNA_GAIN(pAd)*5)/3);
			R66 = 0x30;
#else
			R66 = (UCHAR)(0x32 + (GET_LNA_GAIN(pAd)*5)/3);
#endif // CONFIG_RALINK_RT3883 //

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
			if (IS_RT2883(pAd) || IS_RT3883(pAd))
			{
				ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &BbpValue);
				BbpValue &= 0x9f;
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, BbpValue);
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (BbpValue | (1 << 5)));
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (BbpValue | (1 << 6)));
	    		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
			}
			else
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
    		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
		}
		else
		{
#ifdef CONFIG_RALINK_RT3883
			//R66 = (UCHAR)(0x20 + (GET_LNA_GAIN(pAd)*5)/3);
			R66 = 0x30;
#else
			R66 = (UCHAR)(0x3A + (GET_LNA_GAIN(pAd)*5)/3);
#endif // CONFIG_RALINK_RT3883 //
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
			if (IS_RT2883(pAd) || IS_RT3883(pAd))
			{
				ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &BbpValue);
				BbpValue &= 0x9f;
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, BbpValue);
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (BbpValue | (1 << 5)));
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (BbpValue | (1 << 6)));
				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
			}
			else
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, R66);
		}
	}

	/*
		On 11A, We should delay and wait RF/BBP to be stable
		and the appropriate time should be 1000 micro seconds. 

		2005/06/05 - On 11G, We also need this delay time. Otherwise it's difficult to pass the WHQL.
	*/
	RTMPusecDelay(1000);  

#ifdef RTMP_RF_RW_SUPPORT
	ATEDBGPRINT(RT_DEBUG_TRACE, ("SwitchChannel#%d(RF=%d, Pwr0=%d, Pwr1=%d, %dT), N=0x%02X, K=0x%02X, R=0x%02X\n",
		Channel, 
		pAd->RfIcType, 
		TxPwer,
		TxPwer2,
		pAd->Antenna.field.TxPath,
		FreqItems3020[index].N, 
		FreqItems3020[index].K, 
		FreqItems3020[index].R));
#endif // RTMP_RF_RW_SUPPORT //
#ifndef RTMP_RF_RW_SUPPORT
	if (Channel > 14)
	{
		// When 5.5GHz band the LSB of TxPwr will be used to reduced 7dB or not.
		ATEDBGPRINT(RT_DEBUG_TRACE, ("SwitchChannel#%d(RF=%d, %dT) to , R1=0x%08lx, R2=0x%08lx, R3=0x%08lx, R4=0x%08lx\n",
								  Channel, 
								  pAd->RfIcType, 
								  pAd->Antenna.field.TxPath,
								  pAd->LatchRfRegs.R1, 
								  pAd->LatchRfRegs.R2, 
								  pAd->LatchRfRegs.R3, 
								  pAd->LatchRfRegs.R4));
	}
	else
	{
		ATEDBGPRINT(RT_DEBUG_TRACE, ("SwitchChannel#%d(RF=%d, Pwr0=%u, Pwr1=%u, %dT) to , R1=0x%08lx, R2=0x%08lx, R3=0x%08lx, R4=0x%08lx\n",
								  Channel, 
								  pAd->RfIcType, 
								  (R3 & 0x00003e00) >> 9,
								  (R4 & 0x000007c0) >> 6,
								  pAd->Antenna.field.TxPath,
								  pAd->LatchRfRegs.R1, 
								  pAd->LatchRfRegs.R2, 
								  pAd->LatchRfRegs.R3, 
								  pAd->LatchRfRegs.R4));
    }
#endif // RTMP_RF_RW_SUPPORT //
}


/*
==========================================================================
	Description:
		Gives CCK TX rate 2 more dB TX power.
		This routine works only in ATE mode.

		calculate desired Tx power in RF R3.Tx0~5,	should consider -
		0. if current radio is a noisy environment (pAd->DrsCounters.fNoisyEnvironment)
		1. TxPowerPercentage
		2. auto calibration based on TSSI feedback
		3. extra 2 db for CCK
		4. -10 db upon very-short distance (AvgRSSI >= -40db) to AP

	NOTE: Since this routine requires the value of (pAd->DrsCounters.fNoisyEnvironment),
		it should be called AFTER MlmeDynamicTxRateSwitching()
==========================================================================
*/
VOID ATEAsicAdjustTxPower(
	IN PRTMP_ADAPTER pAd) 
{
	INT			i, j;
	CHAR		DeltaPwr = 0;
	BOOLEAN		bAutoTxAgc = FALSE;
	UCHAR		TssiRef, *pTssiMinusBoundary, *pTssiPlusBoundary, TxAgcStep;
	UCHAR		BbpR49 = 0, idx;
	PCHAR		pTxAgcCompensate;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	ULONG		TxPwr[7];
#else
	ULONG		TxPwr[5];
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
	CHAR		Value;

	/* no one calls this procedure so far */
	if (pAd->ate.TxWI.BW == BW_40)
	{
		if (pAd->ate.Channel > 14)
		{
			TxPwr[0] = pAd->Tx40MPwrCfgABand[0];
			TxPwr[1] = pAd->Tx40MPwrCfgABand[1];
			TxPwr[2] = pAd->Tx40MPwrCfgABand[2];
			TxPwr[3] = pAd->Tx40MPwrCfgABand[3];
			TxPwr[4] = pAd->Tx40MPwrCfgABand[4];
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
			if (IS_RT2883(pAd) || IS_RT3883(pAd) )
			{	
				TxPwr[5] = pAd->Tx40MPwrCfgABand[5];
				TxPwr[6] = pAd->Tx40MPwrCfgABand[6];
			}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		}
		else
		{
			TxPwr[0] = pAd->Tx40MPwrCfgGBand[0];
			TxPwr[1] = pAd->Tx40MPwrCfgGBand[1];
			TxPwr[2] = pAd->Tx40MPwrCfgGBand[2];
			TxPwr[3] = pAd->Tx40MPwrCfgGBand[3];
			TxPwr[4] = pAd->Tx40MPwrCfgGBand[4];
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
			if (IS_RT2883(pAd) || IS_RT3883(pAd) )
			{	
				TxPwr[5] = pAd->Tx40MPwrCfgGBand[5];
				TxPwr[6] = pAd->Tx40MPwrCfgGBand[6];
			}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		}
	}
	else
	{
		if (pAd->ate.Channel > 14)
		{
			TxPwr[0] = pAd->Tx20MPwrCfgABand[0];
			TxPwr[1] = pAd->Tx20MPwrCfgABand[1];
			TxPwr[2] = pAd->Tx20MPwrCfgABand[2];
			TxPwr[3] = pAd->Tx20MPwrCfgABand[3];
			TxPwr[4] = pAd->Tx20MPwrCfgABand[4];
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883) 
			if (IS_RT2883(pAd) || IS_RT3883(pAd) )
			{	
				TxPwr[5] = pAd->Tx20MPwrCfgABand[5];
				TxPwr[6] = pAd->Tx20MPwrCfgABand[6];
			}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		}
		else
		{
			TxPwr[0] = pAd->Tx20MPwrCfgGBand[0];
			TxPwr[1] = pAd->Tx20MPwrCfgGBand[1];
			TxPwr[2] = pAd->Tx20MPwrCfgGBand[2];
			TxPwr[3] = pAd->Tx20MPwrCfgGBand[3];
			TxPwr[4] = pAd->Tx20MPwrCfgGBand[4];
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883) 
			if (IS_RT2883(pAd) || IS_RT3883(pAd) )
			{	
				TxPwr[5] = pAd->Tx20MPwrCfgGBand[5];
				TxPwr[6] = pAd->Tx20MPwrCfgGBand[6];
			}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		}
	}

	// TX power compensation for temperature variation based on TSSI.
	// Do it per 4 seconds.
	if (pAd->Mlme.OneSecPeriodicRound % 4 == 0)
	{
		if (pAd->ate.Channel <= 14)
		{
			/* bg channel */
			bAutoTxAgc         = pAd->bAutoTxAgcG;
			TssiRef            = pAd->TssiRefG;
			pTssiMinusBoundary = &pAd->TssiMinusBoundaryG[0];
			pTssiPlusBoundary  = &pAd->TssiPlusBoundaryG[0];
			TxAgcStep          = pAd->TxAgcStepG;
			pTxAgcCompensate   = &pAd->TxAgcCompensateG;
		}
		else
		{
			/* a channel */
			bAutoTxAgc         = pAd->bAutoTxAgcA;
			TssiRef            = pAd->TssiRefA;
			pTssiMinusBoundary = &pAd->TssiMinusBoundaryA[0];
			pTssiPlusBoundary  = &pAd->TssiPlusBoundaryA[0];
			TxAgcStep          = pAd->TxAgcStepA;
			pTxAgcCompensate   = &pAd->TxAgcCompensateA;
		}

		if (bAutoTxAgc)
		{
			/* BbpR49 is unsigned char. */
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &BbpR49);

			/* (p) TssiPlusBoundaryG[0] = 0 = (m) TssiMinusBoundaryG[0] */
			/* compensate: +4     +3   +2   +1    0   -1   -2   -3   -4 * steps */
			/* step value is defined in pAd->TxAgcStepG for tx power value */

			/* [4]+1+[4]   p4     p3   p2   p1   o1   m1   m2   m3   m4 */
			/* ex:         0x00 0x15 0x25 0x45 0x88 0xA0 0xB5 0xD0 0xF0
			   above value are examined in mass factory production */
			/*             [4]    [3]  [2]  [1]  [0]  [1]  [2]  [3]  [4] */

			/* plus is 0x10 ~ 0x40, minus is 0x60 ~ 0x90 */
			/* if value is between p1 ~ o1 or o1 ~ s1, no need to adjust tx power */
			/* if value is 0x65, tx power will be -= TxAgcStep*(2-1) */

			if (BbpR49 > pTssiMinusBoundary[1])
			{
				// Reading is larger than the reference value.
				// Check for how large we need to decrease the Tx power.
				for (idx = 1; idx < 5; idx++)
				{
					// Found the range.
					if (BbpR49 <= pTssiMinusBoundary[idx])  
						break;
				}

				// The index is the step we should decrease, idx = 0 means there is nothing to compensate.
//				if (R3 > (ULONG) (TxAgcStep * (idx-1)))
					*pTxAgcCompensate = -(TxAgcStep * (idx-1));
//				else
//					*pTxAgcCompensate = -((UCHAR)R3);
				
				DeltaPwr += (*pTxAgcCompensate);
				ATEDBGPRINT(RT_DEBUG_TRACE, ("-- Tx Power, BBP R1=%x, TssiRef=%x, TxAgcStep=%x, step = -%d\n",
					BbpR49, TssiRef, TxAgcStep, idx-1));                    
			}
			else if (BbpR49 < pTssiPlusBoundary[1])
			{
				// Reading is smaller than the reference value.
				// Check for how large we need to increase the Tx power.
				for (idx = 1; idx < 5; idx++)
				{
					// Found the range.
					if (BbpR49 >= pTssiPlusBoundary[idx])   
						break;
				}

				// The index is the step we should increase, idx = 0 means there is nothing to compensate.
				*pTxAgcCompensate = TxAgcStep * (idx-1);
				DeltaPwr += (*pTxAgcCompensate);
				ATEDBGPRINT(RT_DEBUG_TRACE, ("++ Tx Power, BBP R1=%x, TssiRef=%x, TxAgcStep=%x, step = +%d\n",
					BbpR49, TssiRef, TxAgcStep, idx-1));
			}
			else
			{
				*pTxAgcCompensate = 0;
				ATEDBGPRINT(RT_DEBUG_TRACE, ("   Tx Power, BBP R1=%x, TssiRef=%x, TxAgcStep=%x, step = +%d\n",
					BbpR49, TssiRef, TxAgcStep, 0));
			}
		}
	}
	else
	{
		if (pAd->ate.Channel <= 14)
		{
			bAutoTxAgc         = pAd->bAutoTxAgcG;
			pTxAgcCompensate   = &pAd->TxAgcCompensateG;
		}
		else
		{
			bAutoTxAgc         = pAd->bAutoTxAgcA;
			pTxAgcCompensate   = &pAd->TxAgcCompensateA;
		}

		if (bAutoTxAgc)
			DeltaPwr += (*pTxAgcCompensate);
	}

	/* Calculate delta power based on the percentage specified from UI. */
	// E2PROM setting is calibrated for maximum TX power (i.e. 100%)
	// We lower TX power here according to the percentage specified from UI.
	if (pAd->CommonCfg.TxPowerPercentage == 0xffffffff)       // AUTO TX POWER control
		;
	else if (pAd->CommonCfg.TxPowerPercentage > 90)  // 91 ~ 100% & AUTO, treat as 100% in terms of mW
		;
	else if (pAd->CommonCfg.TxPowerPercentage > 60)  // 61 ~ 90%, treat as 75% in terms of mW
	{
		DeltaPwr -= 1;
	}
	else if (pAd->CommonCfg.TxPowerPercentage > 30)  // 31 ~ 60%, treat as 50% in terms of mW
	{
		DeltaPwr -= 3;
	}
	else if (pAd->CommonCfg.TxPowerPercentage > 15)  // 16 ~ 30%, treat as 25% in terms of mW
	{
		DeltaPwr -= 6;
	}
	else if (pAd->CommonCfg.TxPowerPercentage > 9)   // 10 ~ 15%, treat as 12.5% in terms of mW
	{
		DeltaPwr -= 9;
	}
	else                                           // 0 ~ 9 %, treat as MIN(~3%) in terms of mW
	{
		DeltaPwr -= 12;
	}

	/* Reset different new tx power for different TX rate. */
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	for (i=0; i<7; i++)
#else
	for (i=0; i<5; i++)
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
	{
		if (TxPwr[i] != 0xffffffff)
		{
			for (j=0; j<8; j++)
			{
				Value = (CHAR)((TxPwr[i] >> j*4) & 0x0F); /* 0 ~ 15 */

				if ((Value + DeltaPwr) < 0)
				{
					Value = 0; /* min */
				}
				else if ((Value + DeltaPwr) > 0xF)
				{
					Value = 0xF; /* max */
				}
				else
				{
					Value += DeltaPwr; /* temperature compensation */
				}

				/* fill new value to CSR offset */
				TxPwr[i] = (TxPwr[i] & ~(0x0000000F << j*4)) | (Value << j*4);
			}

			/* write tx power value to CSR */
			/* TX_PWR_CFG_0 (8 tx rate) for	TX power for OFDM 12M/18M
											TX power for OFDM 6M/9M
											TX power for CCK5.5M/11M
											TX power for CCK1M/2M */
			/* TX_PWR_CFG_1 ~ TX_PWR_CFG_4 */
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
			if (IS_RT2883(pAd) || IS_RT3883(pAd) )
			{	
				if (i == 5)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_5, TxPwr[i]);
				}
				else if (i == 6)
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_6, TxPwr[i]);
				}
				else
				{
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, TxPwr[i]);
					RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0_EXT + i*4, (TxPwr[i] & 0xf0f0f0f0) >> 4);
				}
			}
			else
			{
				RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, TxPwr[i]);
			}	
#else
			RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, TxPwr[i]);
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

#ifdef RELASE_EXCLUDE
			ATEDBGPRINT(RT_DEBUG_INFO, ("ATEAsicAdjustTxPower - DeltaPwr=%d, offset=0x%x, TxPwr=%lx, BbpR1=%x, round=%ld, pTxAgcCompensate=%d \n",
				DeltaPwr, TX_PWR_CFG_0 + i*4, TxPwr[i], BbpR49, pAd->Mlme.OneSecPeriodicRound, *pTxAgcCompensate));
#endif // RELASE_EXCLUDE //
			
		}
	}

#ifdef RELASE_EXCLUDE
	ATEDBGPRINT(RT_DEBUG_INFO, ("<-- ATEAsicAdjustTxPower, DeltaPwr=%d\n", DeltaPwr));
#endif // RELASE_EXCLUDE //	
}


/*
========================================================================
	Routine Description:
		Write TxWI for ATE mode.
		
	Return Value:
		None
========================================================================
*/
#ifdef RTMP_MAC_PCI
static VOID ATEWriteTxWI(
	IN	PRTMP_ADAPTER	pAd,
	IN	PTXWI_STRUC 	pOutTxWI,	
	IN	BOOLEAN			FRAG,	
	IN	BOOLEAN			CFACK,
	IN	BOOLEAN			InsTimestamp,
	IN	BOOLEAN 		AMPDU,
	IN	BOOLEAN 		Ack,
	IN	BOOLEAN 		NSeq,		// HW new a sequence.
	IN	UCHAR			BASize,
	IN	UCHAR			WCID,
	IN	ULONG			Length,
	IN	UCHAR 			PID,
	IN	UCHAR			TID,
	IN	UCHAR			TxRate,
	IN	UCHAR			Txopmode,	
	IN	BOOLEAN			CfAck,	
	IN	HTTRANSMIT_SETTING	*pTransmit)
{
	TXWI_STRUC 		TxWI;
	PTXWI_STRUC 	pTxWI;

	//
	// Always use Long preamble before verifiation short preamble functionality works well.
	// Todo: remove the following line if short preamble functionality works
	//
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_SHORT_PREAMBLE_INUSED);
	NdisZeroMemory(&TxWI, TXWI_SIZE);
	pTxWI = &TxWI;

	pTxWI->FRAG= FRAG;

	pTxWI->CFACK = CFACK;
	pTxWI->TS= InsTimestamp;
	pTxWI->AMPDU = AMPDU;
	pTxWI->ACK = Ack;
	pTxWI->txop= Txopmode;
	
	pTxWI->NSEQ = NSeq;

	// John tune the performace with Intel Client in 20 MHz performance
	if ( BASize >7 )
		BASize =7;
		
	pTxWI->BAWinSize = BASize;	
	pTxWI->WirelessCliID = WCID;
	pTxWI->MPDUtotalByteCount = Length; 
	pTxWI->PacketId = PID; 
	
	// If CCK or OFDM, BW must be 20
	pTxWI->BW = (pTransmit->field.MODE <= MODE_OFDM) ? (BW_20) : (pTransmit->field.BW);
	pTxWI->ShortGI = pTransmit->field.ShortGI;
	pTxWI->STBC = pTransmit->field.STBC;
	
	pTxWI->MCS = pTransmit->field.MCS;
	pTxWI->PHYMODE = pTransmit->field.MODE;
	pTxWI->CFACK = CfAck;
	pTxWI->MIMOps = 0;
	pTxWI->MpduDensity = 0;

	pTxWI->PacketId = pTxWI->MCS;
	NdisMoveMemory(pOutTxWI, &TxWI, sizeof(TXWI_STRUC));

    return;
}
#endif // RTMP_MAC_PCI //


/*
========================================================================

	Routine Description:
		Disable protection for ATE.
========================================================================
*/
VOID ATEDisableAsicProtect(
	IN		PRTMP_ADAPTER	pAd)
{
	PROT_CFG_STRUC	ProtCfg, ProtCfg4;
	UINT32 Protect[6];
	USHORT			offset;
	UCHAR			i;
	UINT32 MacReg = 0;

	// Config ASIC RTS threshold register
	RTMP_IO_READ32(pAd, TX_RTS_CFG, &MacReg);
	MacReg &= 0xFF0000FF;
	MacReg |= (0xFFF << 8);
	RTMP_IO_WRITE32(pAd, TX_RTS_CFG, MacReg);

	// Initial common protection settings
	RTMPZeroMemory(Protect, sizeof(Protect));
	ProtCfg4.word = 0;
	ProtCfg.word = 0;
	ProtCfg.field.TxopAllowGF40 = 1;
	ProtCfg.field.TxopAllowGF20 = 1;
	ProtCfg.field.TxopAllowMM40 = 1;
	ProtCfg.field.TxopAllowMM20 = 1;
	ProtCfg.field.TxopAllowOfdm = 1;
	ProtCfg.field.TxopAllowCck = 1;
	ProtCfg.field.RTSThEn = 1;
	ProtCfg.field.ProtectNav = ASIC_SHORTNAV;

	// Handle legacy(B/G) protection
	ProtCfg.field.ProtectRate = pAd->CommonCfg.RtsRate;
	ProtCfg.field.ProtectCtrl = 0;
	Protect[0] = ProtCfg.word;
	Protect[1] = ProtCfg.word;
	/* CTS-self is not used */
	pAd->FlgCtsEnabled = 0; 

	// NO PROTECT 
	// 1.All STAs in the BSS are 20/40 MHz HT
	// 2. in ai 20/40MHz BSS
	// 3. all STAs are 20MHz in a 20MHz BSS
	// Pure HT. no protection.

	// MM20_PROT_CFG
	//	Reserved (31:27)
	// 	PROT_TXOP(25:20) -- 010111
	//	PROT_NAV(19:18)  -- 01 (Short NAV protection)
	//  PROT_CTRL(17:16) -- 00 (None)
	// 	PROT_RATE(15:0)  -- 0x4004 (OFDM 24M)
	Protect[2] = 0x01744004;	

	// MM40_PROT_CFG
	//	Reserved (31:27)
	// 	PROT_TXOP(25:20) -- 111111
	//	PROT_NAV(19:18)  -- 01 (Short NAV protection)
	//  PROT_CTRL(17:16) -- 00 (None) 
	// 	PROT_RATE(15:0)  -- 0x4084 (duplicate OFDM 24M)
	Protect[3] = 0x03f44084;

	// CF20_PROT_CFG
	//	Reserved (31:27)
	// 	PROT_TXOP(25:20) -- 010111
	//	PROT_NAV(19:18)  -- 01 (Short NAV protection)
	//  PROT_CTRL(17:16) -- 00 (None)
	// 	PROT_RATE(15:0)  -- 0x4004 (OFDM 24M)
	Protect[4] = 0x01744004;

	// CF40_PROT_CFG
	//	Reserved (31:27)
	// 	PROT_TXOP(25:20) -- 111111
	//	PROT_NAV(19:18)  -- 01 (Short NAV protection)
	//  PROT_CTRL(17:16) -- 00 (None)
	// 	PROT_RATE(15:0)  -- 0x4084 (duplicate OFDM 24M)
	Protect[5] = 0x03f44084;

	pAd->CommonCfg.IOTestParm.bRTSLongProtOn = FALSE;
	
	offset = CCK_PROT_CFG;
	for (i = 0;i < 6;i++)
		RTMP_IO_WRITE32(pAd, offset + i*4, Protect[i]);

}


/* There are two ways to convert Rssi */
/* the way used with GET_LNA_GAIN() */
CHAR ATEConvertToRssi(
	IN PRTMP_ADAPTER pAd,
	IN	CHAR	Rssi,
	IN  UCHAR   RssiNumber)
{
	UCHAR	RssiOffset, LNAGain;

	// Rssi equals to zero should be an invalid value
	if (Rssi == 0)
		return -99;
	
	LNAGain = GET_LNA_GAIN(pAd);
	if (pAd->LatchRfRegs.Channel > 14)
	{
		if (RssiNumber == 0)
			RssiOffset = pAd->ARssiOffset0;
		else if (RssiNumber == 1)
			RssiOffset = pAd->ARssiOffset1;
		else
			RssiOffset = pAd->ARssiOffset2;
	}
	else
	{
		if (RssiNumber == 0)
			RssiOffset = pAd->BGRssiOffset0;
		else if (RssiNumber == 1)
			RssiOffset = pAd->BGRssiOffset1;
		else
			RssiOffset = pAd->BGRssiOffset2;
	}

	return (-12 - RssiOffset - LNAGain - Rssi);
}


/*
========================================================================

	Routine Description:
		Set Japan filter coefficients if needed.
	Note:
		This routine should only be called when
		entering TXFRAME mode or TXCONT mode.
				
========================================================================
*/
static VOID SetJapanFilter(
	IN		PRTMP_ADAPTER	pAd)
{
	UCHAR			BbpData = 0;
	
	//
	// If Channel=14 and Bandwidth=20M and Mode=CCK, set BBP R4 bit5=1
	// (Japan Tx filter coefficients)when (TXFRAME or TXCONT).
	//
	ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BbpData);

    if ((pAd->ate.TxWI.PHYMODE == MODE_CCK) && (pAd->ate.Channel == 14) && (pAd->ate.TxWI.BW == BW_20))
    {
        BbpData |= 0x20;    // turn on
        ATEDBGPRINT(RT_DEBUG_TRACE, ("SetJapanFilter!!!\n"));
    }
    else
    {
		BbpData &= 0xdf;    // turn off
		ATEDBGPRINT(RT_DEBUG_TRACE, ("ClearJapanFilter!!!\n"));
    }

	ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, BbpData);
}


VOID ATESampleRssi(
	IN PRTMP_ADAPTER	pAd,
	IN PRXWI_STRUC		pRxWI)
{
	/* There are two ways to collect RSSI. */
//	pAd->LastRxRate = (USHORT)((pRxWI->MCS) + (pRxWI->BW <<7) + (pRxWI->ShortGI <<8)+ (pRxWI->PHYMODE <<14)) ;
	if (pRxWI->RSSI0 != 0)
	{
		pAd->ate.LastRssi0	= ATEConvertToRssi(pAd, (CHAR) pRxWI->RSSI0, RSSI_0);
		pAd->ate.AvgRssi0X8	= (pAd->ate.AvgRssi0X8 - pAd->ate.AvgRssi0) + pAd->ate.LastRssi0;
		pAd->ate.AvgRssi0  	= pAd->ate.AvgRssi0X8 >> 3;
	}
	if (pRxWI->RSSI1 != 0)
	{
		pAd->ate.LastRssi1	= ATEConvertToRssi(pAd, (CHAR) pRxWI->RSSI1, RSSI_1);
		pAd->ate.AvgRssi1X8	= (pAd->ate.AvgRssi1X8 - pAd->ate.AvgRssi1) + pAd->ate.LastRssi1;
		pAd->ate.AvgRssi1	= pAd->ate.AvgRssi1X8 >> 3;
	}
	if (pRxWI->RSSI2 != 0)
	{
		pAd->ate.LastRssi2	= ATEConvertToRssi(pAd, (CHAR) pRxWI->RSSI2, RSSI_2);
		pAd->ate.AvgRssi2X8	= (pAd->ate.AvgRssi2X8 - pAd->ate.AvgRssi2) + pAd->ate.LastRssi2;
		pAd->ate.AvgRssi2	= pAd->ate.AvgRssi2X8 >> 3;
	}

	pAd->ate.LastSNR0 = (CHAR)(pRxWI->SNR0);// CHAR ==> UCHAR ?
	pAd->ate.LastSNR1 = (CHAR)(pRxWI->SNR1);// CHAR ==> UCHAR ?
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	if (IS_RT2883(pAd) || IS_RT3883(pAd) )
		pAd->ate.LastSNR2 = (CHAR)(pRxWI->SNR2);// CHAR ==> UCHAR ?
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

	pAd->ate.NumOfAvgRssiSample ++;
}


#ifdef CONFIG_STA_SUPPORT
VOID RTMPStationStop(
    IN  PRTMP_ADAPTER   pAd)
{
//	BOOLEAN       Cancelled;
	
    ATEDBGPRINT(RT_DEBUG_TRACE, ("==> RTMPStationStop\n"));

	// For rx statistics, we need to keep this timer running.
//	RTMPCancelTimer(&pAd->Mlme.PeriodicTimer,      &Cancelled);

    ATEDBGPRINT(RT_DEBUG_TRACE, ("<== RTMPStationStop\n"));
}


VOID RTMPStationStart(
    IN  PRTMP_ADAPTER   pAd)
{
    ATEDBGPRINT(RT_DEBUG_TRACE, ("==> RTMPStationStart\n"));

#ifdef RTMP_MAC_PCI
	pAd->Mlme.CntlMachine.CurrState = CNTL_IDLE;

	/* We did not cancel this timer when entering ATE mode. */
//	RTMPSetTimer(&pAd->Mlme.PeriodicTimer, MLME_TASK_EXEC_INTV);
#endif // RTMP_MAC_PCI //

	ATEDBGPRINT(RT_DEBUG_TRACE, ("<== RTMPStationStart\n"));
}
#endif // CONFIG_STA_SUPPORT //


/* 
==========================================================================
	Description:
		Setup Frame format.
	NOTE:
		This routine should only be used in ATE mode.
==========================================================================
*/
#ifdef RTMP_MAC_PCI
static INT ATESetUpFrame(
	IN PRTMP_ADAPTER pAd,
	IN UINT32 TxIdx)
{
	UINT j;
	PTXD_STRUC pTxD;
#ifdef RT_BIG_ENDIAN
    PTXD_STRUC      pDestTxD;
    TXD_STRUC       TxD;
#endif
	PNDIS_PACKET pPacket;
	PUCHAR pDest;
	PVOID AllocVa;
	NDIS_PHYSICAL_ADDRESS AllocPa;
	HTTRANSMIT_SETTING	TxHTPhyMode;

	PRTMP_TX_RING pTxRing = &pAd->TxRing[QID_AC_BE];
	PTXWI_STRUC pTxWI = (PTXWI_STRUC) pTxRing->Cell[TxIdx].DmaBuf.AllocVa;
	PUCHAR pDMAHeaderBufVA = (PUCHAR) pTxRing->Cell[TxIdx].DmaBuf.AllocVa;

#ifdef RALINK_28xx_QA
	PHEADER_802_11	pHeader80211;
#endif // RALINK_28xx_QA //

	if (pAd->ate.bQATxStart == TRUE) 
	{
		// always use QID_AC_BE and FIFO_EDCA

		// fill TxWI
		TxHTPhyMode.field.BW = pAd->ate.TxWI.BW;
		TxHTPhyMode.field.ShortGI = pAd->ate.TxWI.ShortGI;
		TxHTPhyMode.field.STBC = pAd->ate.TxWI.STBC;
		TxHTPhyMode.field.MCS = pAd->ate.TxWI.MCS;
		TxHTPhyMode.field.MODE = pAd->ate.TxWI.PHYMODE;

		ATEWriteTxWI(pAd, pTxWI, pAd->ate.TxWI.FRAG, pAd->ate.TxWI.CFACK,
			pAd->ate.TxWI.TS,  pAd->ate.TxWI.AMPDU, pAd->ate.TxWI.ACK, pAd->ate.TxWI.NSEQ, 
			pAd->ate.TxWI.BAWinSize, 0, pAd->ate.TxWI.MPDUtotalByteCount, pAd->ate.TxWI.PacketId, 0, 0,
			pAd->ate.TxWI.txop/*IFS_HTTXOP*/, pAd->ate.TxWI.CFACK/*FALSE*/, &TxHTPhyMode);

#ifdef TXBF_SUPPORT
		if (IS_RT2883(pAd) || IS_RT3883(pAd))
		{
			// Must copy rsv bits to actual TxWI
			pTxWI->rsv = pAd->ate.TxWI.rsv;
			//		pTxWI->rsv2 = pAd->ate.TxWI.rsv2;
			pTxWI->iTxBF = pAd->ate.TxWI.iTxBF;	
			pTxWI->Sounding = pAd->ate.TxWI.Sounding;
			pTxWI->eTxBF = pAd->ate.TxWI.eTxBF;
			pTxWI->Autofallback = pAd->ate.TxWI.Autofallback;
			pTxWI->NDPSndBW = pAd->ate.TxWI.NDPSndBW;
			pTxWI->NDPSndRate = pAd->ate.TxWI.NDPSndRate;
		}
#endif // TXBF_SUPPORT //
	}
	else
	{
		TxHTPhyMode.field.BW = pAd->ate.TxWI.BW;
		TxHTPhyMode.field.ShortGI = pAd->ate.TxWI.ShortGI;
		TxHTPhyMode.field.STBC = pAd->ate.TxWI.STBC;
		TxHTPhyMode.field.MCS = pAd->ate.TxWI.MCS;
		TxHTPhyMode.field.MODE = pAd->ate.TxWI.PHYMODE;
		ATEWriteTxWI(pAd, pTxWI, FALSE, FALSE, FALSE,  FALSE, FALSE, FALSE, 
			4, 0, pAd->ate.TxLength, 0, 0, 0, IFS_HTTXOP, FALSE, &TxHTPhyMode);
	}
	
	// fill 802.11 header
#ifdef RALINK_28xx_QA
	if (pAd->ate.bQATxStart == TRUE) 
	{
		NdisMoveMemory(pDMAHeaderBufVA+TXWI_SIZE, pAd->ate.Header, pAd->ate.HLen);
	}
	else
#endif // RALINK_28xx_QA //
	{
		NdisMoveMemory(pDMAHeaderBufVA+TXWI_SIZE, TemplateFrame, LENGTH_802_11);
		NdisMoveMemory(pDMAHeaderBufVA+TXWI_SIZE+4, pAd->ate.Addr1, ETH_LENGTH_OF_ADDRESS);
		NdisMoveMemory(pDMAHeaderBufVA+TXWI_SIZE+10, pAd->ate.Addr2, ETH_LENGTH_OF_ADDRESS);
		NdisMoveMemory(pDMAHeaderBufVA+TXWI_SIZE+16, pAd->ate.Addr3, ETH_LENGTH_OF_ADDRESS);
	}

#ifdef RT_BIG_ENDIAN
	RTMPFrameEndianChange(pAd, (((PUCHAR)pDMAHeaderBufVA)+TXWI_SIZE), DIR_READ, FALSE);
#endif // RT_BIG_ENDIAN //

	/* alloc buffer for payload */
#ifdef RALINK_28xx_QA
	if (pAd->ate.bQATxStart == TRUE) 
	{
		pPacket = RTMP_AllocateRxPacketBuffer(pAd, pAd->ate.DLen + 0x100, FALSE, &AllocVa, &AllocPa);
	}
	else
#endif // RALINK_28xx_QA //
	{
		pPacket = RTMP_AllocateRxPacketBuffer(pAd, pAd->ate.TxLength, FALSE, &AllocVa, &AllocPa);
	}

	if (pPacket == NULL)
	{
		pAd->ate.TxCount = 0;
		ATEDBGPRINT(RT_DEBUG_TRACE, ("%s fail to alloc packet space.\n", __FUNCTION__));
		return -1;
	}
	pTxRing->Cell[TxIdx].pNextNdisPacket = pPacket;

	pDest = (PUCHAR) AllocVa;

#ifdef RALINK_28xx_QA
	if (pAd->ate.bQATxStart == TRUE) 
	{
		GET_OS_PKT_LEN(pPacket) = pAd->ate.DLen;
	}
	else
#endif // RALINK_28xx_QA //
	{
		GET_OS_PKT_LEN(pPacket) = pAd->ate.TxLength - LENGTH_802_11;
	}

	// prepare frame payload
#ifdef RALINK_28xx_QA
	if (pAd->ate.bQATxStart == TRUE) 
	{
		// copy pattern
		if ((pAd->ate.PLen != 0))
		{
			int j;
			
			for (j = 0; j < pAd->ate.DLen; j+=pAd->ate.PLen)
			{
				memcpy(GET_OS_PKT_DATAPTR(pPacket) + j, pAd->ate.Pattern, pAd->ate.PLen);
			}
		}
	}
	else
#endif // RALINK_28xx_QA //
	{
		for(j = 0; j < GET_OS_PKT_LEN(pPacket); j++)
			pDest[j] = pAd->ate.Payload;/*kurtis: 0xAA ATE test EVM will be positive*/
	}

	/* build Tx Descriptor */
#ifndef RT_BIG_ENDIAN
	pTxD = (PTXD_STRUC) pTxRing->Cell[TxIdx].AllocVa;
#else
    pDestTxD  = (PTXD_STRUC)pTxRing->Cell[TxIdx].AllocVa;
    TxD = *pDestTxD;
    pTxD = &TxD;
#endif // !RT_BIG_ENDIAN //

#ifdef RALINK_28xx_QA
	if (pAd->ate.bQATxStart == TRUE)
	{
		// prepare TxD
		NdisZeroMemory(pTxD, TXD_SIZE);
		RTMPWriteTxDescriptor(pAd, pTxD, FALSE, FIFO_EDCA);
		// build TX DESC
		pTxD->SDPtr0 = RTMP_GetPhysicalAddressLow(pTxRing->Cell[TxIdx].DmaBuf.AllocPa);
		pTxD->SDLen0 = TXWI_SIZE + pAd->ate.HLen;
		pTxD->LastSec0 = 0;
		pTxD->SDPtr1 = AllocPa;
		pTxD->SDLen1 = GET_OS_PKT_LEN(pPacket);
		pTxD->LastSec1 = 1;

		pDest = (PUCHAR)pTxWI;
		pDest += TXWI_SIZE;
		pHeader80211 = (PHEADER_802_11)pDest;
		
		// modify sequence number...
		if (pAd->ate.TxDoneCount == 0)
		{
			pAd->ate.seq = pHeader80211->Sequence;
		}
		else
			pHeader80211->Sequence = ++pAd->ate.seq;
	}
	else
#endif // RALINK_28xx_QA //
	{
		NdisZeroMemory(pTxD, TXD_SIZE);
		RTMPWriteTxDescriptor(pAd, pTxD, FALSE, FIFO_EDCA);
		// build TX DESC
		pTxD->SDPtr0 = RTMP_GetPhysicalAddressLow (pTxRing->Cell[TxIdx].DmaBuf.AllocPa);
		pTxD->SDLen0 = TXWI_SIZE + LENGTH_802_11;
		pTxD->LastSec0 = 0;
		pTxD->SDPtr1 = AllocPa;
		pTxD->SDLen1 = GET_OS_PKT_LEN(pPacket);
		pTxD->LastSec1 = 1;
	}

#ifdef RT_BIG_ENDIAN
	RTMPWIEndianChange((PUCHAR)pTxWI, TYPE_TXWI);
	RTMPFrameEndianChange(pAd, (((PUCHAR)pDMAHeaderBufVA)+TXWI_SIZE), DIR_WRITE, FALSE);
    RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
    WriteBackToDescriptor((PUCHAR)pDestTxD, (PUCHAR)pTxD, FALSE, TYPE_TXD);
#endif // RT_BIG_ENDIAN //

	return 0;
}

/*=======================End of RTMP_MAC_PCI =======================*/
#endif // RTMP_MAC_PCI //




VOID rt_ee_read_all(PRTMP_ADAPTER pAd, USHORT *Data)
{
	USHORT i;
	USHORT value;

#ifdef RTMP_RBUS_SUPPORT
	if (pAd->infType == RTMP_DEV_INF_RBUS)
	{	
		rtmp_ee_flash_read_all(pAd, Data);
		return;
	}
#endif // RTMP_RBUS_SUPPORT //

	for (i = 0 ; i < EEPROM_SIZE/2 ; )
	{
		/* "value" is especially for some compilers... */
		RT28xx_EEPROM_READ16(pAd, i*2, value);
		Data[i] = value;
		i++;
	}
}


VOID rt_ee_write_all(PRTMP_ADAPTER pAd, USHORT *Data)
{
	USHORT i;
	USHORT value;

#ifdef RTMP_RBUS_SUPPORT
	if (pAd->infType == RTMP_DEV_INF_RBUS)
	{
		rtmp_ee_flash_write_all(pAd, Data);
		return;
	}
#endif // RTMP_RBUS_SUPPORT //

	for (i = 0 ; i < EEPROM_SIZE/2 ; )
	{
		/* "value" is especially for some compilers... */
		value = Data[i];
		RT28xx_EEPROM_WRITE16(pAd, i*2, value);
		i++;
	}
}


#ifdef RALINK_28xx_QA
VOID ATE_QA_Statistics(
	IN PRTMP_ADAPTER			pAd,
	IN PRXWI_STRUC				pRxWI,
	IN PRT28XX_RXD_STRUC		pRxD,
	IN PHEADER_802_11			pHeader)
{
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	UINT32 lastRxRate = 0;
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
	// update counter first
	if (pHeader != NULL)
	{
		if (pHeader->FC.Type == BTYPE_DATA)
		{
			if (pRxD->U2M)
			{
				pAd->ate.U2M++;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
				if (pRxWI->MPDUtotalByteCount >= 32)
					lastRxRate = (pRxWI->MCS) + (pRxWI->BW <<7) + (pRxWI->ShortGI <<8) + (pRxWI->STBC <<9) + (pRxWI->PHYMODE <<14) + (pRxWI->MPDUtotalByteCount<<16);
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
			}
			else
				pAd->ate.OtherData++;
		}
		else if (pHeader->FC.Type == BTYPE_MGMT)
		{
			if (pHeader->FC.SubType == SUBTYPE_BEACON)
				pAd->ate.Beacon++;
			else
				pAd->ate.OtherCount++;
		}
		else if (pHeader->FC.Type == BTYPE_CNTL)
		{
			pAd->ate.OtherCount++;
		}
	}
	pAd->ate.RSSI0 = pRxWI->RSSI0; 
	pAd->ate.RSSI1 = pRxWI->RSSI1; 
	pAd->ate.RSSI2 = pRxWI->RSSI2; 
	pAd->ate.SNR0 = pRxWI->SNR0;
	pAd->ate.SNR1 = pRxWI->SNR1;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
	{
		pAd->ate.SNR2 = pRxWI->SNR2;
		pAd->ate.LastRxRate = lastRxRate;
		pAd->ate.BF_SNR[0] = BF_SNR_OFFSET + pRxWI->BF_SNR0;
		pAd->ate.BF_SNR[1] = BF_SNR_OFFSET + pRxWI->BF_SNR1;
		pAd->ate.BF_SNR[2] = BF_SNR_OFFSET + pRxWI->BF_SNR2;
	}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
}


VOID RtmpDoAte(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	struct iwreq	*wrq)
{
	USHORT Command_Id;
	INT	Status = NDIS_STATUS_SUCCESS;
	struct ate_racfghdr *pRaCfg;
	
#ifdef RELASE_EXCLUDE
	ATEDBGPRINT(RT_DEBUG_LOUD, ("===>RtmpDoAte()\n"));
#endif // RELASE_EXCLUDE //

	if ((pRaCfg = kmalloc(sizeof(struct ate_racfghdr), GFP_KERNEL)) == NULL)
	{
		Status = -EINVAL;
		return;
	}
				
	NdisZeroMemory(pRaCfg, sizeof(struct ate_racfghdr));

    if (copy_from_user((PUCHAR)pRaCfg, wrq->u.data.pointer, wrq->u.data.length))
	{
		Status = -EFAULT;

		kfree(pRaCfg);
		return;
	}
#ifdef RELASE_EXCLUDE
    else
    {
    	ATEDBGPRINT(RT_DEBUG_LOUD, ("Success in copy_from_user()\n"));
    }
#endif // RELASE_EXCLUDE //
    
	Command_Id = ntohs(pRaCfg->command_id);
	
	ATEDBGPRINT(RT_DEBUG_TRACE,("\n%s: Command_Id = 0x%04x !\n", __FUNCTION__, Command_Id));
	
	// In passive mode only allow commands that read registers
	if (pAdapter->ate.PassiveMode) {
		int i, allowCmd = FALSE;
		static int allowedCmds[] = {
				RACFG_CMD_E2PROM_READ16, RACFG_CMD_E2PROM_READ_ALL,
				RACFG_CMD_IO_READ, RACFG_CMD_IO_READ_BULK,
				RACFG_CMD_BBP_READ8, RACFG_CMD_BBP_READ_ALL,
				RACFG_CMD_ATE_E2PROM_READ_BULK,
				RACFG_CMD_ATE_BBP_READ_BULK,
				RACFG_CMD_ATE_RF_READ_BULK,
				RACFG_CMD_QUERY_IBF_TAG, RACFG_CMD_QUERY_EBF_TAG,
				RACFG_CMD_QUERY_IBF_PROFILE, RACFG_CMD_QUERY_EBF_PROFILE};

		for (i=0; i<sizeof(allowedCmds)/sizeof(allowedCmds[0]); i++) {
			if (Command_Id==allowedCmds[i]) {
				allowCmd = TRUE;
				break;
			}
		}
		// Also allow writes to BF profile registers
		if (Command_Id==RACFG_CMD_BBP_WRITE8) {
			int offset = ntohs(pRaCfg->status);
			if (offset==BBP_R27 || (offset>=BBP_R174 && offset<=BBP_R182))
				allowCmd = TRUE;
		}

		// If not allowed then ignore the command
		if (!allowCmd) {
			ResponseToGUI(pRaCfg, wrq, sizeof(pRaCfg->status), NDIS_STATUS_SUCCESS);
			if (pRaCfg != NULL)
				kfree(pRaCfg);
			return;
		}
	}

	switch (Command_Id) 
	{
 		/* We will get this command when QA starts. */
		case RACFG_CMD_ATE_START:
			Status=DO_RACFG_CMD_ATE_START(pAdapter,wrq,pRaCfg);
			break;

 		/* We will get this command either QA is closed or ated is killed by user. */
		case RACFG_CMD_ATE_STOP:
			Status=DO_RACFG_CMD_ATE_STOP(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_RF_WRITE_ALL:
			Status=DO_RACFG_CMD_RF_WRITE_ALL(pAdapter,wrq,pRaCfg);
			break;				
			
		case RACFG_CMD_E2PROM_READ16:
			Status=DO_RACFG_CMD_E2PROM_READ16(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_E2PROM_WRITE16:
			Status=DO_RACFG_CMD_E2PROM_WRITE16(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_E2PROM_READ_ALL:
			Status=DO_RACFG_CMD_E2PROM_READ_ALL(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_E2PROM_WRITE_ALL:
			Status=DO_RACFG_CMD_E2PROM_WRITE_ALL(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_IO_READ:
			Status=DO_RACFG_CMD_IO_READ(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_IO_WRITE:
			Status=DO_RACFG_CMD_IO_WRITE(pAdapter,wrq,pRaCfg);
			break;
			
		case RACFG_CMD_IO_READ_BULK:
			Status=DO_RACFG_CMD_IO_READ_BULK(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_BBP_READ8:
			Status=DO_RACFG_CMD_BBP_READ8(pAdapter,wrq,pRaCfg);
			break;
		case RACFG_CMD_BBP_WRITE8:
			Status=DO_RACFG_CMD_BBP_WRITE8(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_BBP_READ_ALL:
			Status=DO_RACFG_CMD_BBP_READ_ALL(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_E2PROM_READ_BULK:
			Status=DO_RACFG_CMD_ATE_E2PROM_READ_BULK(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_E2PROM_WRITE_BULK:
			Status=DO_RACFG_CMD_ATE_E2PROM_WRITE_BULK(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_IO_WRITE_BULK:
			Status=DO_RACFG_CMD_ATE_IO_WRITE_BULK(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_BBP_READ_BULK:
			Status=DO_RACFG_CMD_ATE_BBP_READ_BULK(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_BBP_WRITE_BULK:
			Status=DO_RACFG_CMD_ATE_BBP_WRITE_BULK(pAdapter,wrq,pRaCfg);
			break;

#ifdef CONFIG_RALINK_RT3883
		case RACFG_CMD_ATE_ETH_EXT_SETTING:
			Status=DO_RACFG_CMD_ATE_ETH_EXT_SETTING(pAdapter,wrq,pRaCfg);
			break;
#endif // CONFIG_RALINK_RT3883 //

#ifdef RTMP_RF_RW_SUPPORT
		case RACFG_CMD_ATE_RF_READ_BULK:
			Status=DO_RACFG_CMD_ATE_RF_READ_BULK(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_RF_WRITE_BULK:
			Status=DO_RACFG_CMD_ATE_RF_WRITE_BULK(pAdapter,wrq,pRaCfg);
			break;
#endif // RTMP_RF_RW_SUPPORT //

		case RACFG_CMD_GET_NOISE_LEVEL:
			Status=DO_RACFG_CMD_GET_NOISE_LEVEL(pAdapter,wrq,pRaCfg);
			break;

#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)
		case RACFG_CMD_QUERY_BF_RSP:
			Status=DO_RACFG_CMD_QUERY_BF_RSP(pAdapter,wrq,pRaCfg);
			break;
		case RACFG_CMD_QUERY_IBF_TAG:
		case RACFG_CMD_QUERY_EBF_TAG:
			Status=DO_RACFG_CMD_QUERY_IBF_TAG(pAdapter,wrq,pRaCfg);
			break;
		case RACFG_CMD_QUERY_IBF_PROFILE:
		case RACFG_CMD_QUERY_EBF_PROFILE:
			Status=DO_RACFG_CMD_QUERY_IBF_PROFILE(pAdapter,wrq,pRaCfg);
			break;
		case RACFG_CMD_WRITE_IBF_TAG:
		case RACFG_CMD_WRITE_EBF_TAG:
			Status=DO_RACFG_CMD_WRITE_IBF_TAG(pAdapter,wrq,pRaCfg);
			break;
		case RACFG_CMD_WRITE_IBF_PROFILE:
		case RACFG_CMD_WRITE_EBF_PROFILE:
			Status=DO_RACFG_CMD_WRITE_IBF_PROFILE(pAdapter,wrq,pRaCfg);
			break;
		case RACFG_CMD_CALIBRATION_CAPTURE:
			Status=DO_RACFG_CALIBRATION_CAPTURE(pAdapter,wrq,pRaCfg);
			break;
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //

		case RACFG_CMD_GET_COUNTER:
			Status=DO_RACFG_CMD_GET_COUNTER(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_CLEAR_COUNTER:
			Status=DO_RACFG_CMD_CLEAR_COUNTER(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_TX_START:
			Status=DO_RACFG_CMD_TX_START(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_GET_TX_STATUS:
			Status=DO_RACFG_CMD_GET_TX_STATUS(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_TX_STOP:
			Status=DO_RACFG_CMD_TX_STOP(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_RX_START:
			Status=DO_RACFG_CMD_RX_START(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_RX_STOP:
			Status=DO_RACFG_CMD_RX_STOP(pAdapter,wrq,pRaCfg);
			break;

		/* The following cases are for new ATE GUI(not QA). */
		/*==================================================*/
		case RACFG_CMD_ATE_START_TX_CARRIER:
			Status=DO_RACFG_CMD_ATE_START_TX_CARRIER(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_START_TX_CONT:
			Status=DO_RACFG_CMD_ATE_START_TX_CONT(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_START_TX_FRAME:
			Status=DO_RACFG_CMD_ATE_START_TX_FRAME(pAdapter,wrq,pRaCfg);
			break;	

		case RACFG_CMD_ATE_SET_BW:
			Status=DO_RACFG_CMD_ATE_SET_BW(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_TX_POWER0:
			Status=DO_RACFG_CMD_ATE_SET_TX_POWER0(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_TX_POWER1:
			Status=DO_RACFG_CMD_ATE_SET_TX_POWER1(pAdapter,wrq,pRaCfg);
			break;

#if defined(RT2883) || defined(RT3883)
		case RACFG_CMD_ATE_SET_TX_POWER2:
			Status=DO_RACFG_CMD_ATE_SET_TX_POWER2(pAdapter,wrq,pRaCfg);
			break;
#endif

		case RACFG_CMD_ATE_SET_FREQ_OFFSET:
			Status=DO_RACFG_CMD_ATE_SET_FREQ_OFFSET(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_GET_STATISTICS:
			Status=DO_RACFG_CMD_ATE_GET_STATISTICS(pAdapter,wrq,pRaCfg);	
			break;

		case RACFG_CMD_ATE_RESET_COUNTER:
			Status=DO_RACFG_CMD_ATE_RESET_COUNTER(pAdapter,wrq,pRaCfg);	
			break;

		case RACFG_CMD_ATE_SEL_TX_ANTENNA:
			Status=DO_RACFG_CMD_ATE_SEL_TX_ANTENNA(pAdapter,wrq,pRaCfg);		
			break;
			
		case RACFG_CMD_ATE_SEL_RX_ANTENNA:
			Status=DO_RACFG_CMD_ATE_SEL_RX_ANTENNA(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_PREAMBLE:
			Status=DO_RACFG_CMD_ATE_SET_PREAMBLE(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_CHANNEL:
			Status=DO_RACFG_CMD_ATE_SET_CHANNEL(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_ADDR1:
			Status=DO_RACFG_CMD_ATE_SET_ADDR1(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_ADDR2:
			Status=DO_RACFG_CMD_ATE_SET_ADDR2(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_ADDR3:
			Status=DO_RACFG_CMD_ATE_SET_ADDR3(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_RATE:
			Status=DO_RACFG_CMD_ATE_SET_RATE(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_TX_FRAME_LEN:
			Status=DO_RACFG_CMD_ATE_SET_TX_FRAME_LEN(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_SET_TX_FRAME_COUNT:
			Status=DO_RACFG_CMD_ATE_SET_TX_FRAME_COUNT(pAdapter,wrq,pRaCfg);
			break;

		case RACFG_CMD_ATE_START_RX_FRAME:
			Status=DO_RACFG_CMD_ATE_START_RX_FRAME(pAdapter,wrq,pRaCfg);
			break;
		default:
			break;		
	}

    ASSERT(pRaCfg != NULL);

    if (pRaCfg != NULL)
	    kfree(pRaCfg);

	return;
}


VOID BubbleSort(INT32 n, INT32 a[])
{ 
	INT32 k, j, temp;

	for (k = n-1;  k>0;  k--)
	{
		for (j = 0; j<k; j++)
		{
			if (a[j] > a[j+1])
			{
				temp = a[j]; 
				a[j]=a[j+1]; 
				a[j+1]=temp;
			}
		}
	}
} 


VOID CalNoiseLevel(PRTMP_ADAPTER pAd, UCHAR channel, INT32 RSSI[3][10])
{
	INT32		RSSI0, RSSI1, RSSI2;
 	CHAR		Rssi0Offset, Rssi1Offset, Rssi2Offset;
	UCHAR		BbpR50Rssi0 = 0, BbpR51Rssi1 = 0, BbpR52Rssi2 = 0;
	UCHAR		Org_BBP66value = 0, Org_BBP69value = 0, Org_BBP70value = 0, data = 0, byteValue = 0;
	USHORT		LNA_Gain = 0;
	INT32       j = 0;
	UCHAR		Org_Channel = pAd->ate.Channel;
	USHORT	    GainValue = 0, OffsetValue = 0;

	ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R66, &Org_BBP66value);
	ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R69, &Org_BBP69value);	
	ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R70, &Org_BBP70value);

	//**********************************************************************
	// Read the value of LNA gain and Rssi offset 
	//**********************************************************************
#ifdef RT3883
	if(IS_RT3883(pAd))
		RT28xx_EEPROM_READ16(pAd, EEPROM_LNA_OFFSET, GainValue);
	else
#endif // RT3883 //		
	RT28xx_EEPROM_READ16(pAd, EEPROM_LNA_OFFSET, GainValue);

	// for Noise Level
	if (channel <= 14)
	{
		LNA_Gain = GainValue & 0x00FF;		 

#ifdef RT3883
		if(IS_RT3883(pAd))
			RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET, OffsetValue);
		else
#endif // RT3883 //	
		RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET, OffsetValue);
		Rssi0Offset = OffsetValue & 0x00FF;
		Rssi1Offset = (OffsetValue & 0xFF00) >> 8;

#ifdef RT3883
		if(IS_RT3883(pAd))
			RT28xx_EEPROM_READ16(pAd, (EEPROM_RSSI_BG_OFFSET + 2), OffsetValue);
		else
#endif // RT3883 //	
		RT28xx_EEPROM_READ16(pAd, (EEPROM_RSSI_BG_OFFSET + 2)/* 0x48 */, OffsetValue);
		Rssi2Offset = OffsetValue & 0x00FF;
	}
	else
	{
		LNA_Gain = (GainValue & 0xFF00) >> 8;
#ifdef RT3883
		if(IS_RT3883(pAd))
			RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_A_OFFSET, OffsetValue);
		else
#endif // RT3883 //
		RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_A_OFFSET, OffsetValue);
		Rssi0Offset = OffsetValue & 0x00FF;
		Rssi1Offset = (OffsetValue & 0xFF00) >> 8;
#ifdef RT3883
	if(IS_RT3883(pAd))
		RT28xx_EEPROM_READ16(pAd, (EEPROM_RSSI_A_OFFSET + 2), OffsetValue);
	else
#endif // RT3883 //			
		RT28xx_EEPROM_READ16(pAd, (EEPROM_RSSI_A_OFFSET + 2)/* 0x4C */, OffsetValue);
		Rssi2Offset = OffsetValue & 0x00FF;
	}
	//**********************************************************************	
	{
		pAd->ate.Channel = channel;
		ATEAsicSwitchChannel(pAd);
		mdelay(5);

		data = 0x10;
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)// peter : need to check it
		if (IS_RT2883(pAd) || IS_RT3883(pAd))
		{
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &byteValue);
			byteValue &= 0x9f;
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, byteValue);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, data);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (byteValue | (1 << 5)));
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, data);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (byteValue | (1 << 6)));		
		}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, data);	
		data = 0x40;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, data);
		data = 0x40;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, data);
		mdelay(5);

		// start Rx
		pAd->ate.bQARxStart = TRUE;
		Set_ATE_Proc(pAd, "RXFRAME");

		mdelay(5);

		for (j = 0; j < 10; j++)
		{
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R50, &BbpR50Rssi0);
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R51, &BbpR51Rssi1);	
			ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R52, &BbpR52Rssi2);

			mdelay(10);

			// calculate RSSI 0
			if (BbpR50Rssi0 == 0)
			{
				RSSI0 = -100;
			}
			else
			{
				RSSI0 = (INT32)(-12 - BbpR50Rssi0 - LNA_Gain - Rssi0Offset);
			}
			RSSI[0][j] = RSSI0;

			if ( pAd->Antenna.field.RxPath >= 2 ) // 2R
			{
				// calculate RSSI 1
				if (BbpR51Rssi1 == 0)
				{
					RSSI1 = -100;
				}
				else
				{
					RSSI1 = (INT32)(-12 - BbpR51Rssi1 - LNA_Gain - Rssi1Offset);
				}
				RSSI[1][j] = RSSI1;
			}

			if ( pAd->Antenna.field.RxPath >= 3 ) // 3R
			{
				// calculate RSSI 2
				if (BbpR52Rssi2 == 0)
					RSSI2 = -100;
				else
					RSSI2 = (INT32)(-12 - BbpR52Rssi2 - LNA_Gain - Rssi2Offset);

				RSSI[2][j] = RSSI2;
			}
		}

		// stop Rx
		Set_ATE_Proc(pAd, "RXSTOP");

		mdelay(5);

		BubbleSort(10, RSSI[0]);	// 1R		

		if ( pAd->Antenna.field.RxPath >= 2 ) // 2R
		{
			BubbleSort(10, RSSI[1]);
		}

		if ( pAd->Antenna.field.RxPath >= 3 ) // 3R
		{
			BubbleSort(10, RSSI[2]);
		}	
	}

	pAd->ate.Channel = Org_Channel;
	ATEAsicSwitchChannel(pAd);
	
	// restore original value
#if defined (CONFIG_RALINK_RT2883) || defined (CONFIG_RALINK_RT3883)// peter : need to check it
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
	{
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &byteValue);
		byteValue &= 0x9f;
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, byteValue);
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, Org_BBP66value);
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (byteValue | (1 << 5)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, Org_BBP66value);
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (byteValue | (1 << 6)));		
	}
#endif // CONFIG_RALINK_RT2883 || CONFIG_RALINK_RT3883 //
    ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, Org_BBP66value);
    ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, Org_BBP69value);
    ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, Org_BBP70value);

	return;
}


BOOLEAN SyncTxRxConfig(PRTMP_ADAPTER pAd, USHORT offset, UCHAR value)
{ 
	UCHAR tmp = 0, bbp_data = 0;

	if (ATE_ON(pAd))
	{
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, offset, &bbp_data);
	}
	else
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, offset, &bbp_data);
	}

	/* confirm again */
	ASSERT(bbp_data == value);

	switch (offset)
	{
		case BBP_R1:
			/* Need to synchronize tx configuration with legacy ATE. */
			tmp = (bbp_data & ((1 << 4) | (1 << 3))/* 0x18 */) >> 3;
		    switch (tmp)
		    {
				/* The BBP R1 bit[4:3] = 2 :: Both DACs will be used by QA. */
		        case 2:
					/* All */
					pAd->ate.TxAntennaSel = 0;
		            break;
				/* The BBP R1 bit[4:3] = 0 :: DAC 0 will be used by QA. */
		        case 0:
					/* Antenna one */
					pAd->ate.TxAntennaSel = 1;
		            break;
				/* The BBP R1 bit[4:3] = 1 :: DAC 1 will be used by QA. */
		        case 1:
					/* Antenna two */
					pAd->ate.TxAntennaSel = 2;
		            break;
		        default:
		            DBGPRINT(RT_DEBUG_TRACE, ("%s -- Sth. wrong!  : return FALSE; \n", __FUNCTION__));    
		            return FALSE;
		    }
			break;/* case BBP_R1 */

		case BBP_R3:
			/* Need to synchronize rx configuration with legacy ATE. */
			tmp = (bbp_data & ((1 << 1) | (1 << 0))/* 0x03 */);
		    switch(tmp)
		    {
				/* The BBP R3 bit[1:0] = 3 :: All ADCs will be used by QA. */
		        case 3:
					/* All */
					pAd->ate.RxAntennaSel = 0;
		            break;
				/*
					The BBP R3 bit[1:0] = 0 :: ADC 0 will be used by QA,
					unless the BBP R3 bit[4:3] = 2
				*/
		        case 0:
					/* Antenna one */
					pAd->ate.RxAntennaSel = 1;
					tmp = ((bbp_data & ((1 << 4) | (1 << 3))/* 0x03 */) >> 3);
					if (tmp == 2)// 3R
					{
						/* Default : All ADCs will be used by QA */
						pAd->ate.RxAntennaSel = 0;
					}
		            break;
				/* The BBP R3 bit[1:0] = 1 :: ADC 1 will be used by QA. */
		        case 1:
					/* Antenna two */
					pAd->ate.RxAntennaSel = 2;
		            break;
				/* The BBP R3 bit[1:0] = 2 :: ADC 2 will be used by QA. */
		        case 2:
					/* Antenna three */
					pAd->ate.RxAntennaSel = 3;
		            break;
		        default:
		            DBGPRINT(RT_DEBUG_ERROR, ("%s -- Impossible!  : return FALSE; \n", __FUNCTION__));    
		            return FALSE;
		    }
			break;/* case BBP_R3 */

        default:
            DBGPRINT(RT_DEBUG_ERROR, ("%s -- Sth. wrong!  : return FALSE; \n", __FUNCTION__));    
            return FALSE;
		
	}
	return TRUE;
} 


static VOID memcpy_exl(PRTMP_ADAPTER pAd, UCHAR *dst, UCHAR *src, ULONG len)
{
	ULONG i, Value = 0;
	ULONG *pDst, *pSrc;
	UCHAR *p8;
	
	p8 = src;
	pDst = (ULONG *) dst;
	pSrc = (ULONG *) src;
	
	for (i = 0 ; i < (len/4); i++)
	{
		/* For alignment issue, we need a variable "Value". */
		memmove(&Value, pSrc, 4);
		Value = htonl(Value); 
		memmove(pDst, &Value, 4);		
		pDst++;
		pSrc++;
	}
	if ((len % 4) != 0)
	{
		/* wish that it will never reach here */
		memmove(&Value, pSrc, (len % 4));
		Value = htonl(Value); 
		memmove(pDst, &Value, (len % 4));
	}
}


static VOID memcpy_exs(PRTMP_ADAPTER pAd, UCHAR *dst, UCHAR *src, ULONG len)
{
	if (IS_RT2883(pAd) || IS_RT3883(pAd))
	{		
		ULONG i;
		UCHAR *pDst, *pSrc;

		pDst = dst;
		pSrc = src;	

		for (i = 0; i < (len/2); i++)
		{
			memmove(pDst, pSrc, 2);
			*((USHORT *)pDst) = htons(*((USHORT *)pDst));
			pDst+=2;
			pSrc+=2;
		}

		if ((len % 2) != 0)
		{
			memmove(pDst, pSrc, 1);
		}
	}
	else
	{

		ULONG i;
		USHORT *pDst, *pSrc;

		pDst = (USHORT *) dst;
		pSrc = (USHORT *) src;

		for (i =0; i < (len/2); i++)
		{
			*pDst = ntohs(*pSrc);
			pDst++;
			pSrc++;
		}

		if ((len % 2) != 0)
		{
			memcpy(pDst, pSrc, (len % 2));
			*pDst = ntohs(*pDst);
		}
	}

}


static VOID RTMP_IO_READ_BULK(PRTMP_ADAPTER pAd, UCHAR *dst, UCHAR *src, UINT32 len)
{
	UINT32 i, Value;
	UINT32 *pDst, *pSrc;
	
	pDst = (UINT32 *) dst;
	pSrc = (UINT32 *) src;

	for (i = 0 ; i < (len/4); i++)
	{
		RTMP_IO_READ32(pAd, (ULONG)pSrc, &Value);
		Value = htonl(Value);
		memmove(pDst, &Value, 4);
		pDst++;
		pSrc++;
	}
	return;	
}


INT Set_TxStop_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("Set_TxStop_Proc\n"));

	if (Set_ATE_Proc(pAd, "TXSTOP"))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


INT Set_RxStop_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ATEDBGPRINT(RT_DEBUG_TRACE,("Set_RxStop_Proc\n"));

	if (Set_ATE_Proc(pAd, "RXSTOP"))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


#ifdef DBG
INT Set_EERead_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	USHORT buffer[EEPROM_SIZE/2];
	USHORT *p;
	INT i;
	
	rt_ee_read_all(pAd, (USHORT *)buffer);
	p = buffer;

	for (i = 0; i < (EEPROM_SIZE/2); i++)
	{
		ate_print(KERN_EMERG "%4.4x ", *p);
		if (((i+1) % 16) == 0)
			ate_print(KERN_EMERG "\n");
		p++;
	}

	return TRUE;
}


INT Set_EEWrite_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	USHORT offset = 0, value;
	PSTRING p2 = arg;
	
	while ((*p2 != ':') && (*p2 != '\0'))
	{
		p2++;
	}
	
	if (*p2 == ':')
	{
		A2Hex(offset, arg);
		A2Hex(value, p2 + 1);
	}
	else
	{
		A2Hex(value, arg);
	}
	
	if (offset >= EEPROM_SIZE)
	{
		ate_print(KERN_EMERG "Offset can not exceed EEPROM_SIZE( == 0x%04x)\n", EEPROM_SIZE);	
		return FALSE;
	}
	
	RT28xx_EEPROM_WRITE16(pAd, offset, value);

	return TRUE;
}


INT Set_BBPRead_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR value = 0, offset;

	A2Hex(offset, arg);	
			
	if (ATE_ON(pAd))
	{
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, offset,  &value);
	}
	else
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, offset,  &value);
	}

	ate_print(KERN_EMERG "%x\n", value);
		
	return TRUE;
}


INT Set_BBPWrite_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	USHORT offset = 0;
	PSTRING p2 = arg;
	UCHAR value;
	
	while ((*p2 != ':') && (*p2 != '\0'))
	{
		p2++;
	}
	
	if (*p2 == ':')
	{
		A2Hex(offset, arg);	
		A2Hex(value, p2 + 1);	
	}
	else
	{
		A2Hex(value, arg);	
	}

	if (ATE_ON(pAd))
	{
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, offset,  value);
	}
	else
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, offset,  value);
	}

	return TRUE;
}


INT Set_RFWrite_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	PSTRING p2, p3, p4;
	UINT32 R1, R2, R3, R4;
	
	p2 = arg;

	while ((*p2 != ':') && (*p2 != '\0'))
	{
		p2++;
	}
	
	if (*p2 != ':')
		return FALSE;
	
	p3 = p2 + 1;

	while((*p3 != ':') && (*p3 != '\0'))
	{
		p3++;
	}

	if (*p3 != ':')
		return FALSE;
	
	p4 = p3 + 1;

	while ((*p4 != ':') && (*p4 != '\0'))
	{
		p4++;
	}

	if (*p4 != ':')
		return FALSE;

		
	A2Hex(R1, arg);	
	A2Hex(R2, p2 + 1);	
	A2Hex(R3, p3 + 1);	
	A2Hex(R4, p4 + 1);	
	
	RTMP_RF_IO_WRITE32(pAd, R1);
	RTMP_RF_IO_WRITE32(pAd, R2);
	RTMP_RF_IO_WRITE32(pAd, R3);
	RTMP_RF_IO_WRITE32(pAd, R4);
	
	return TRUE;
}
#endif // DBG //
#endif // RALINK_28xx_QA //


#ifdef CONFIG_AP_SUPPORT 
/*
==========================================================================
	Description:
		Used only by ATE to disassociate all STAs and stop AP service.
	Note:
==========================================================================
*/
VOID ATEAPStop(
	IN PRTMP_ADAPTER pAd) 
{
	BOOLEAN     Cancelled;
	UINT32		Value = 0;
	INT         apidx = 0;
		
	ATEDBGPRINT(RT_DEBUG_TRACE, ("!!! ATEAPStop !!!\n"));

	/* To prevent MCU to modify BBP registers w/o indications from driver. */
#ifdef DFS_SUPPORT
#ifdef RTMP_RBUS_SUPPORT
#ifdef NEW_DFS
	if (pAd->CommonCfg.dfs_func >= 1)
		NewRadarDetectionStop(pAd);
	else
#endif // NEW_DFS //
#endif // RTMP_RBUS_SUPPORT //
	{
	RadarDetectionStop(pAd);
	BbpRadarDetectionStop(pAd);
	}
#endif // DFS_SUPPORT //


#ifdef WDS_SUPPORT
	WdsDown(pAd);
#endif // WDS_SUPPORT //

	MacTableReset(pAd);

	/* For ATE, this flag cannot be set. */
//	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);

	// disable pre-tbtt interrupt
	RTMP_IO_READ32(pAd, INT_TIMER_EN, &Value);
	Value &=0xe;
	RTMP_IO_WRITE32(pAd, INT_TIMER_EN, Value);
	// disable piggyback
	RTMPSetPiggyBack(pAd, FALSE);

	ATEDisableAsicProtect(pAd);

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
	{
//		RTMP_ASIC_INTERRUPT_DISABLE(pAd);
		AsicDisableSync(pAd);
		// set LED
		RTMPSetLED(pAd, LED_LINK_DOWN);
	}

	for (apidx = 0; apidx < MAX_MBSSID_NUM; apidx++)
	{
		if (pAd->ApCfg.MBSSID[apidx].REKEYTimerRunning == TRUE)
		{
			RTMPCancelTimer(&pAd->ApCfg.MBSSID[apidx].REKEYTimer, &Cancelled);
			pAd->ApCfg.MBSSID[apidx].REKEYTimerRunning = FALSE;
		}
	}

	if (pAd->ApCfg.CMTimerRunning == TRUE)
	{
		RTMPCancelTimer(&pAd->ApCfg.CounterMeasureTimer, &Cancelled);
		pAd->ApCfg.CMTimerRunning = FALSE;
	}
	
	
	/* Cancel the Timer, to make sure the timer was not queued. */
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);


#ifdef IDS_SUPPORT
	// if necessary, cancel IDS timer
	RTMPIdsStop(pAd);
#endif // IDS_SUPPORT //


#ifdef GREENAP_SUPPORT
	DisableAPMIMOPS(pAd);
#endif // GREENAP_SUPPORT //

}
#endif // CONFIG_AP_SUPPORT //
#endif	// RALINK_ATE //

