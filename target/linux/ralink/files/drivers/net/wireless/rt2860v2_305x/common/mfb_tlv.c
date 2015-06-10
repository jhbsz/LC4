/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	mfb_tlv.c

	Abstract:

  
*/
#include "rt_config.h"

VOID
MFB_PerPareMRQ(
	IN	PRTMP_ADAPTER	pAd,
	OUT	VOID* pBuf,
	IN	PMAC_TABLE_ENTRY pEntry)
{
	PHT_CONTROL pHT_Control;

//	DBGPRINT(RT_DEBUG_TRACE, ("-----> MFB_PerPareMRQ\n"));
	if (pEntry->HTCapability.ExtHtCapInfo.MCSFeedback >= MCSFBK_MRQ)
	{
		pHT_Control = (HT_CONTROL *)pBuf;

		pHT_Control->MRQ = 1;
		
		if (pEntry->msiToTx == MSI_TOGGLE_BF)
			if (pEntry->mrqCnt == 0)
				pEntry->mrqCnt = TOGGLE_BF_PKTS;
			else 
			{
				(pEntry->mrqCnt)--;
				if (pEntry->mrqCnt == 0)
					pEntry->msiToTx = 0;
			}
		pHT_Control->MSI = pEntry->msiToTx;

		//update region
		if (pEntry->msiToTx == MSI_TOGGLE_BF-1)//MSI_TOGGLE_BF==6 is used to indicate the TxBF status is inverted for this packet
			pEntry->msiToTx = 0;
		else if (pEntry->msiToTx !=  MSI_TOGGLE_BF)
			pEntry->msiToTx++;

	}

//	DBGPRINT(RT_DEBUG_TRACE, ("<----- MFB_PerPareMRQ\n"));
}
VOID
MFB_PerPareMFB(//to be completed!!!!!!!!!!!!!!!!!
	IN	PRTMP_ADAPTER	pAd,
	OUT	VOID* pBuf,
	IN	PMAC_TABLE_ENTRY pEntry)
{
	PHT_CONTROL pHT_Control;

//	DBGPRINT(RT_DEBUG_TRACE, ("-----> MFB_PerPareMRQ\n"));
/*	if (pAd->CommonCfg.HtCapability.ExtHtCapInfo.MCSFeedback >= MCSFBK_MRQ)
	{
		pHT_Control = (HT_CONTROL *)pBuf;

		pHT_Control-> = 1;
		
		if (pEntry->msiToTx == MSI_TOGGLE_BF)
			if (pEntry->mrqCnt == 0)
				pEntry->mrqCnt = TOGGLE_BF_PKTS;
			else 
			{
				(pEntry->mrqCnt)--;
				if (pEntry->mrqCnt == 0)
					pEntry->msiToTx = 0;
			}
		pHT_Control->MFBorASC = ;
		pHT_Control->MFSI = 

		//update region
		if (pEntry->msiToTx == MSI_TOGGLE_BF-1)//MSI_TOGGLE_BF==6 is used to indicate the TxBF status is inverted for this packet
			pEntry->msiToTx = 0;
		else if (pEntry->msiToTx !=  MSI_TOGGLE_BF)
			pEntry->msiToTx++;

	}
*/
//	DBGPRINT(RT_DEBUG_TRACE, ("<----- MFB_PerPareMRQ\n"));
}


