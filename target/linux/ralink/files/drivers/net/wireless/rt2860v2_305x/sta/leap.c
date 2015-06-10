/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	leap.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Name		Date			Modification logs
*/
#include "rt_config.h"

UCHAR   EAP_HEADER[8] = {0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00, 0x88, 0x8e};

UCHAR   SNAP_LEAP_ROGUEAP_HDR[8] = {0xAA, 0xAA, 0x03, 0x00, 0x40, 0x96, 0x00, 0x00};

UINT32   Spbox[8][64] = {
{0x01010400,0x00000000,0x00010000,0x01010404,
0x01010004,0x00010404,0x00000004,0x00010000,
0x00000400,0x01010400,0x01010404,0x00000400,
0x01000404,0x01010004,0x01000000,0x00000004,
0x00000404,0x01000400,0x01000400,0x00010400,
0x00010400,0x01010000,0x01010000,0x01000404,
0x00010004,0x01000004,0x01000004,0x00010004,
0x00000000,0x00000404,0x00010404,0x01000000,
0x00010000,0x01010404,0x00000004,0x01010000,
0x01010400,0x01000000,0x01000000,0x00000400,
0x01010004,0x00010000,0x00010400,0x01000004,
0x00000400,0x00000004,0x01000404,0x00010404,
0x01010404,0x00010004,0x01010000,0x01000404,
0x01000004,0x00000404,0x00010404,0x01010400,
0x00000404,0x01000400,0x01000400,0x00000000,
0x00010004,0x00010400,0x00000000,0x01010004}
,{0x80108020,0x80008000,0x00008000,0x00108020,
0x00100000,0x00000020,0x80100020,0x80008020,
0x80000020,0x80108020,0x80108000,0x80000000,
0x80008000,0x00100000,0x00000020,0x80100020,
0x00108000,0x00100020,0x80008020,0x00000000,
0x80000000,0x00008000,0x00108020,0x80100000,
0x00100020,0x80000020,0x00000000,0x00108000,
0x00008020,0x80108000,0x80100000,0x00008020,
0x00000000,0x00108020,0x80100020,0x00100000,
0x80008020,0x80100000,0x80108000,0x00008000,
0x80100000,0x80008000,0x00000020,0x80108020,
0x00108020,0x00000020,0x00008000,0x80000000,
0x00008020,0x80108000,0x00100000,0x80000020,
0x00100020,0x80008020,0x80000020,0x00100020,
0x00108000,0x00000000,0x80008000,0x00008020,
0x80000000,0x80100020,0x80108020,0x00108000}
,{0x00000208,0x08020200,0x00000000,0x08020008,
0x08000200,0x00000000,0x00020208,0x08000200,
0x00020008,0x08000008,0x08000008,0x00020000,
0x08020208,0x00020008,0x08020000,0x00000208,
0x08000000,0x00000008,0x08020200,0x00000200,
0x00020200,0x08020000,0x08020008,0x00020208,
0x08000208,0x00020200,0x00020000,0x08000208,
0x00000008,0x08020208,0x00000200,0x08000000,
0x08020200,0x08000000,0x00020008,0x00000208,
0x00020000,0x08020200,0x08000200,0x00000000,
0x00000200,0x00020008,0x08020208,0x08000200,
0x08000008,0x00000200,0x00000000,0x08020008,
0x08000208,0x00020000,0x08000000,0x08020208,
0x00000008,0x00020208,0x00020200,0x08000008,
0x08020000,0x08000208,0x00000208,0x08020000,
0x00020208,0x00000008,0x08020008,0x00020200}
,{0x00802001,0x00002081,0x00002081,0x00000080,
0x00802080,0x00800081,0x00800001,0x00002001,
0x00000000,0x00802000,0x00802000,0x00802081,
0x00000081,0x00000000,0x00800080,0x00800001,
0x00000001,0x00002000,0x00800000,0x00802001,
0x00000080,0x00800000,0x00002001,0x00002080,
0x00800081,0x00000001,0x00002080,0x00800080,
0x00002000,0x00802080,0x00802081,0x00000081,
0x00800080,0x00800001,0x00802000,0x00802081,
0x00000081,0x00000000,0x00000000,0x00802000,
0x00002080,0x00800080,0x00800081,0x00000001,
0x00802001,0x00002081,0x00002081,0x00000080,
0x00802081,0x00000081,0x00000001,0x00002000,
0x00800001,0x00002001,0x00802080,0x00800081,
0x00002001,0x00002080,0x00800000,0x00802001,
0x00000080,0x00800000,0x00002000,0x00802080}
,{0x00000100,0x02080100,0x02080000,0x42000100,
0x00080000,0x00000100,0x40000000,0x02080000,
0x40080100,0x00080000,0x02000100,0x40080100,
0x42000100,0x42080000,0x00080100,0x40000000,
0x02000000,0x40080000,0x40080000,0x00000000,
0x40000100,0x42080100,0x42080100,0x02000100,
0x42080000,0x40000100,0x00000000,0x42000000,
0x02080100,0x02000000,0x42000000,0x00080100,
0x00080000,0x42000100,0x00000100,0x02000000,
0x40000000,0x02080000,0x42000100,0x40080100,
0x02000100,0x40000000,0x42080000,0x02080100,
0x40080100,0x00000100,0x02000000,0x42080000,
0x42080100,0x00080100,0x42000000,0x42080100,
0x02080000,0x00000000,0x40080000,0x42000000,
0x00080100,0x02000100,0x40000100,0x00080000,
0x00000000,0x40080000,0x02080100,0x40000100}
,{0x20000010,0x20400000,0x00004000,0x20404010,
0x20400000,0x00000010,0x20404010,0x00400000,
0x20004000,0x00404010,0x00400000,0x20000010,
0x00400010,0x20004000,0x20000000,0x00004010,
0x00000000,0x00400010,0x20004010,0x00004000,
0x00404000,0x20004010,0x00000010,0x20400010,
0x20400010,0x00000000,0x00404010,0x20404000,
0x00004010,0x00404000,0x20404000,0x20000000,
0x20004000,0x00000010,0x20400010,0x00404000,
0x20404010,0x00400000,0x00004010,0x20000010,
0x00400000,0x20004000,0x20000000,0x00004010,
0x20000010,0x20404010,0x00404000,0x20400000,
0x00404010,0x20404000,0x00000000,0x20400010,
0x00000010,0x00004000,0x20400000,0x00404010,
0x00004000,0x00400010,0x20004010,0x00000000,
0x20404000,0x20000000,0x00400010,0x20004010}
,{0x00200000,0x04200002,0x04000802,0x00000000,
0x00000800,0x04000802,0x00200802,0x04200800,
0x04200802,0x00200000,0x00000000,0x04000002,
0x00000002,0x04000000,0x04200002,0x00000802,
0x04000800,0x00200802,0x00200002,0x04000800,
0x04000002,0x04200000,0x04200800,0x00200002,
0x04200000,0x00000800,0x00000802,0x04200802,
0x00200800,0x00000002,0x04000000,0x00200800,
0x04000000,0x00200800,0x00200000,0x04000802,
0x04000802,0x04200002,0x04200002,0x00000002,
0x00200002,0x04000000,0x04000800,0x00200000,
0x04200800,0x00000802,0x00200802,0x04200800,
0x00000802,0x04000002,0x04200802,0x04200000,
0x00200800,0x00000000,0x00000002,0x04200802,
0x00000000,0x00200802,0x04200000,0x00000800,
0x04000002,0x04000800,0x00000800,0x00200002}
,{0x10001040,0x00001000,0x00040000,0x10041040,
0x10000000,0x10001040,0x00000040,0x10000000,
0x00040040,0x10040000,0x10041040,0x00041000,
0x10041000,0x00041040,0x00001000,0x00000040,
0x10040000,0x10000040,0x10001000,0x00001040,
0x00041000,0x00040040,0x10040040,0x10041000,
0x00001040,0x00000000,0x00000000,0x10040040,
0x10000040,0x10001000,0x00041040,0x00040000,
0x00041040,0x00040000,0x10041000,0x00001000,
0x00000040,0x10040040,0x00001000,0x00041040,
0x10001000,0x00000040,0x10000040,0x10040000,
0x10040040,0x10000000,0x00040000,0x10001040,
0x00000000,0x10041040,0x00040040,0x10000040,
0x10040000,0x10001000,0x10001040,0x00000000,
0x10041040,0x00041000,0x00041000,0x00001040,
0x00001040,0x00040040,0x10000000,0x10041000}
};

/* Primitive function F.
 * Input is r, subkey array in keys, output is XORed into l.
 * Each round consumes eight 6-bit subkeys, one for
 * each of the 8 S-boxes, 2 longs for each round.
 * Each long contains four 6-bit subkeys, each taking up a byte.
 * The first long contains, from high to low end, the subkeys for
 * S-boxes 1, 3, 5 & 7; the second contains the subkeys for S-boxes
 * 2, 4, 6 & 8 (using the origin-1 S-box numbering in the standard,
 * not the origin-0 numbering used elsewhere in this code)
 * See comments elsewhere about the pre-rotated values of r and Spbox.
 */
#define F(l,r,key){\
    work = ((r >> 4) | (r << 28)) ^ key[0];\
    l ^= Spbox[6][work & 0x3f];\
    l ^= Spbox[4][(work >> 8) & 0x3f];\
    l ^= Spbox[2][(work >> 16) & 0x3f];\
    l ^= Spbox[0][(work >> 24) & 0x3f];\
    work = r ^ key[1];\
    l ^= Spbox[7][work & 0x3f];\
    l ^= Spbox[5][(work >> 8) & 0x3f];\
    l ^= Spbox[3][(work >> 16) & 0x3f];\
    l ^= Spbox[1][(work >> 24) & 0x3f];\
}

// Key schedule-related tables from FIPS-46

// permuted choice table (key)
UCHAR pc1[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,

    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

// number left rotations of pc1
UCHAR totrot[] = {
    1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28
};

// permuted choice key (table)
UCHAR pc2[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

/* End of DES-defined tables */

int Asmversion = 0;
/* bit 0 is left-most in byte */
static int bytebit[] = {
    0200,0100,040,020,010,04,02,01
};

/*
    ========================================================================

    Routine Description:
        Classify LEAP EAP message type

    Arguments:
        EAPType     Value of EAP message type
        MsgType     Internal Message definition for MLME state machine

    Return Value:
        TRUE        Found appropriate message type
        FALSE       No appropriate message type

    Note:

    ========================================================================
*/
BOOLEAN LeapMsgTypeSubst(
    IN  UCHAR   EAPType,
    OUT ULONG   *MsgType)
{
    *MsgType = LEAP_MSG;
    return TRUE;
}

/*
    ========================================================================

    Routine Description:
        LEAP Machine, Process LEAP Message. 

    Arguments:
        pAd         Pointer to our adapter
        *S          Current LEAP state machine
        Elem        Queue message
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapMachinePerformAction(
    IN  PRTMP_ADAPTER   pAd,
    IN  STATE_MACHINE   *S,
    IN  MLME_QUEUE_ELEM *Elem)
{
    PLEAP_EAPOL_HEADER          pLeapHeader = NULL;
    PLEAP_EAPOL_PACKET          pLeapPacket = NULL;
    MLME_QUEUE_ELEM             ElemDeauththenticationMsg;
    MLME_DEAUTH_REQ_STRUCT      *pInfo;
    INT                         MsgType;
    BOOLEAN                     TimerCancelled;
    
#ifdef RELASE_EXCLUDE
	DBGPRINT(RT_DEBUG_INFO, ("==> LeapMachinePerformAction\n"));	
#endif // RELASE_EXCLUDE //
    // EAP header first
    pLeapHeader = (PLEAP_EAPOL_HEADER) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H )];

    if (pLeapHeader->Length != 0)
    {
        pLeapPacket = (PLEAP_EAPOL_PACKET) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H + sizeof(LEAP_EAPOL_HEADER))];
    }

    if ((pLeapPacket != NULL) && (pLeapHeader != NULL))
    {
        if ((pLeapPacket->Code == 0x01) && (pLeapPacket->Type == 0x01))
            MsgType = LEAP_MSG_REQUEST_IDENTITY;
        else if ((pLeapPacket->Code == 0x01) && (pLeapPacket->Type == 0x11))
            MsgType = LEAP_MSG_REQUEST_LEAP;
        else if (pLeapPacket->Code == 0x03)
            MsgType = LEAP_MSG_SUCCESS;
        else if (pLeapPacket->Code == 0x04)
            MsgType = LEAP_MSG_FAILED;
        else if ((pLeapPacket->Code == 0x02) && (pLeapPacket->Type == 0x11))
            MsgType = LEAP_MSG_RESPONSE_LEAP;
        else if (pLeapHeader->Type == 0x03)
            MsgType = LEAP_MSG_EAPOLKEY;
        else 
            MsgType = LEAP_MSG_UNKNOWN;
    }

    switch(pAd->Mlme.LeapMachine.CurrState)
    {
        case LEAP_IDLE:
            if (MsgType == LEAP_MSG_EAPOLKEY)
            {
                if ((pAd->StaCfg.LeapAuthInfo.CCKM == TRUE) || (pAd->CommonCfg.WepStatus == Ndis802_11Encryption2Enabled))
                    WpaEAPOLKeyAction(pAd, Elem);
                else
                    LeapEapolKeyAction(pAd, Elem);
            }
            else
            LeapStartAction(pAd, Elem);
            break;
        case LEAP_WAIT_IDENTITY_REQUEST:
            if (MsgType == LEAP_MSG_REQUEST_IDENTITY)
                LeapIdentityAction(pAd, Elem);
            break;
        case LEAP_WAIT_CHANLLENGE_REQUEST:
            if (MsgType == LEAP_MSG_REQUEST_LEAP)
                LeapPeerChallengeAction(pAd, Elem);
            break;
        case LEAP_WAIT_SUCCESS:
            if (MsgType == LEAP_MSG_SUCCESS)
                LeapNetworkChallengeAction(pAd, Elem);
            else if (MsgType == LEAP_MSG_FAILED)
            {
                RTMPCancelTimer(&pAd->StaCfg.LeapAuthTimer, &TimerCancelled);
                RogueApTableSetEntry(pAd, &pAd->StaCfg.RogueApTab, pAd->CommonCfg.Bssid, LEAP_REASON_CHALLENGE_FROM_AP_FAILED);
                pInfo = (MLME_DEAUTH_REQ_STRUCT *) ElemDeauththenticationMsg.Msg;
                pInfo->Reason = REASON_DEAUTH_STA_LEAVING;
                COPY_MAC_ADDR(pInfo->Addr, pAd->CommonCfg.Bssid);
                MlmeDeauthReqAction(pAd, &ElemDeauththenticationMsg);
                if (INFRA_ON(pAd))
                {
                    LinkDown(pAd, FALSE);
                    pAd->Mlme.AssocMachine.CurrState = ASSOC_IDLE;
                }
                pAd->Mlme.LeapMachine.CurrState = LEAP_IDLE;
            }
            break;
        case LEAP_WAIT_CHANLLENGE_RESPONSE:
            if (MsgType == LEAP_MSG_RESPONSE_LEAP)
                LeapNetworkChallengeResponse(pAd, Elem);
            break;
        case LEAP_WAIT_EAPOLKEY:
            if (MsgType == LEAP_MSG_EAPOLKEY)
            {
                if ((pAd->StaCfg.LeapAuthInfo.CCKM == TRUE) || (pAd->CommonCfg.WepStatus == Ndis802_11Encryption2Enabled))
                    WpaEAPOLKeyAction(pAd, Elem);
                else
                    LeapEapolKeyAction(pAd, Elem);
            }
            else if (MsgType == LEAP_MSG_REQUEST_IDENTITY)
                LeapIdentityAction(pAd, Elem);
            break;
        default:
            DBGPRINT(RT_DEBUG_TRACE, (" LEAP EAP Message Unknown\n"));
            break;
    }
#ifdef RELASE_EXCLUDE
	DBGPRINT(RT_DEBUG_INFO, ("<== LeapMachinePerformAction\n"));
#endif // RELASE_EXCLUDE //
    
}

/*
    ========================================================================

    Routine Description:
        Init Leap 802.11 Header

    Arguments:
        pAd         Pointer to our adapter
        pHdr80211   Pointer to 802.11 Header
        wep         Use Wep or not.
        pAddr1      Pointer to Addr1
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapMacHeaderInit(
    IN  PRTMP_ADAPTER       pAd,
    IN  OUT PHEADER_802_11  pHdr80211,
    IN  UCHAR               wep,
    IN  PUCHAR              pAddr3)
{
    NdisZeroMemory(pHdr80211, sizeof(HEADER_802_11));
    pHdr80211->FC.Type = BTYPE_DATA;
    pHdr80211->FC.ToDs = 1;
    if (wep == 1)
        pHdr80211->FC.Wep = 1;

    // 
    // To Ds
    // Addr1: BSSID, Addr2: SA, Addr3: DA  
    // Spec Page 44. Table 4--Address field contents
    //
    COPY_MAC_ADDR(pHdr80211->Addr1, pAd->CommonCfg.Bssid);
    COPY_MAC_ADDR(pHdr80211->Addr2, pAd->CurrentAddress);
    COPY_MAC_ADDR(pHdr80211->Addr3, pAddr3);
    pHdr80211->Sequence = pAd->Sequence;
}

/*
    ========================================================================

    Routine Description:
        Send EAPOL-Start Packet to Authenticator

    Arguments:
        pAd         Pointer to our adapter
        *Elem       Pointer queue message
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapStartAction(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem)
{
    HEADER_802_11       Header_802_11;
    ULONG               FrameLen = 0;
//  UCHAR               ApAddr[6];
    PUCHAR              pOutBuffer = NULL;
    NDIS_STATUS         NStatus;
    LEAP_EAPOL_HEADER   Leap_EAPOL_Header;

    DBGPRINT(RT_DEBUG_TRACE, ("==> LeapStartAction\n"));
    // allocate for send out EAPOL-Start frame
    NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);  //Get an unused nonpaged memory
    if (NStatus != NDIS_STATUS_SUCCESS)
    {
        DBGPRINT(RT_DEBUG_TRACE, ("Send EAPOL-Start Frame failed, cause by not enough memory\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("<== LeapStartAction\n"));
        return;
    }

    LeapMacHeaderInit(pAd, &Header_802_11, 0, pAd->CommonCfg.Bssid);
    Leap_EAPOL_Header.Version = 0x01;
    Leap_EAPOL_Header.Type = 0x01;
    Leap_EAPOL_Header.Length[0] = 0;
    Leap_EAPOL_Header.Length[1] = 0;

#ifdef RELASE_EXCLUDE
	DBGPRINT(RT_DEBUG_INFO, ("LEAP - Send EAPOL-Start to %02x:%02x:%02x:%02x:%02x:%02x\n", 
                pAd->CommonCfg.Bssid[0],pAd->CommonCfg.Bssid[1],pAd->CommonCfg.Bssid[2],
                pAd->CommonCfg.Bssid[3],pAd->CommonCfg.Bssid[4],pAd->CommonCfg.Bssid[5]));
#endif // RELASE_EXCLUDE //
    
    MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
                      sizeof(HEADER_802_11),            &Header_802_11,
                      sizeof(EAP_HEADER),               EAP_HEADER,
                      sizeof(LEAP_EAPOL_HEADER),        &Leap_EAPOL_Header,
                      END_OF_ARGS);
    MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
	MlmeFreeMemory(pAd, pOutBuffer);

    pAd->Mlme.LeapMachine.CurrState = LEAP_WAIT_IDENTITY_REQUEST;
    pAd->StaCfg.PortSecured = WPA_802_1X_PORT_NOT_SECURED;
    RTMPSetTimer(&pAd->StaCfg.LeapAuthTimer, LEAP_AUTH_TIMEOUT);
    DBGPRINT(RT_DEBUG_TRACE, ("<== LeapStartAction\n"));
}

/*
    ========================================================================

    Routine Description:
        Send Identity Response Packet to Authenticator

    Arguments:
        pAd         Pointer to our adapter
        *Elem       Pointer queue message
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapIdentityAction(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem)
{
    HEADER_802_11       Header_802_11;
    ULONG               FrameLen = 0;
//  UCHAR               ApAddr[6];
    PUCHAR              pOutBuffer = NULL;
    PUCHAR              pLeapOutBuffer;
    NDIS_STATUS         NStatus;
    UCHAR               Identifier;
    PLEAP_EAPOL_HEADER  pLeap_EAPOL_Header = NULL;
    PLEAP_EAPOL_PACKET  pLeap_EAPOL_Packet = NULL;
    ULONG              EapLen = 0;

    DBGPRINT(RT_DEBUG_TRACE, ("==> LeapIdentityAction\n"));
    // allocate for send out EAPOL-Identify Response frame
    NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);  //Get an unused nonpaged memory
    if (NStatus != NDIS_STATUS_SUCCESS)
    {
        DBGPRINT(RT_DEBUG_TRACE, ("Send Identify response failed, cause by not enough memory\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("<== LeapIdentityAction\n"));
        return;
    }

    // allocate for EAPOL-Identify frame;
    NStatus = MlmeAllocateMemory(pAd, &pLeapOutBuffer);  //Get an unused nonpaged memory
    if (NStatus != NDIS_STATUS_SUCCESS)
    {
        MlmeFreeMemory(pAd, pOutBuffer);
        DBGPRINT(RT_DEBUG_TRACE, ("Send Identify response failed, cause by not enough memory\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("<== LeapIdentityAction\n"));
        return;
    }

    pLeap_EAPOL_Header = (PLEAP_EAPOL_HEADER) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H)];
    pLeap_EAPOL_Packet = (PLEAP_EAPOL_PACKET) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H + sizeof(LEAP_EAPOL_HEADER))];

    //record the original identifier
    if ((pLeap_EAPOL_Header != NULL) && (pLeap_EAPOL_Packet != NULL))
        Identifier = pLeap_EAPOL_Packet->Identifier;
    else
    {
        MlmeFreeMemory(pAd, pOutBuffer);
        MlmeFreeMemory(pAd, pLeapOutBuffer);
        DBGPRINT(RT_DEBUG_TRACE, ("EAPOL Frame can't be NULL\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("<== LeapIdentityAction\n"));
        return;
    }

    pLeap_EAPOL_Header = (PLEAP_EAPOL_HEADER) pLeapOutBuffer;
    pLeap_EAPOL_Packet = (PLEAP_EAPOL_PACKET) (pLeapOutBuffer+sizeof(LEAP_EAPOL_HEADER));

    NdisZeroMemory(pLeap_EAPOL_Header, sizeof(LEAP_EAPOL_HEADER));
    NdisZeroMemory(pLeap_EAPOL_Packet, sizeof(LEAP_EAPOL_PACKET));

    pLeap_EAPOL_Packet->Code = 0x02; //Response
    pLeap_EAPOL_Packet->Identifier = Identifier;
    pLeap_EAPOL_Packet->Type = 0x01; //Identity;
    EapLen += sizeof(LEAP_EAPOL_PACKET);
    NdisMoveMemory((PUCHAR) pLeap_EAPOL_Packet + sizeof(LEAP_EAPOL_PACKET),  (PUCHAR)&pAd->StaCfg.LeapAuthInfo.UserName, pAd->StaCfg.LeapAuthInfo.UserNameLen);
	
    EapLen += pAd->StaCfg.LeapAuthInfo.UserNameLen;
    pLeap_EAPOL_Packet->Length[0] = (UCHAR) (EapLen / 256);
    pLeap_EAPOL_Packet->Length[1] = (UCHAR) (EapLen % 256);
    pLeap_EAPOL_Header->Length[0] = (UCHAR) (EapLen / 256);
    pLeap_EAPOL_Header->Length[1] = (UCHAR) (EapLen % 256);

    EapLen += sizeof(LEAP_EAPOL_HEADER);
    LeapMacHeaderInit(pAd, &Header_802_11, 0, pAd->CommonCfg.Bssid);
    pLeap_EAPOL_Header->Version = 0x01;
    pLeap_EAPOL_Header->Type = 0x00;  //EAP-Packet
    MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
                      sizeof(HEADER_802_11),            &Header_802_11,
                      sizeof(EAP_HEADER),               EAP_HEADER,
                      EapLen,                           pLeap_EAPOL_Header,
                      END_OF_ARGS);
    MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
	MlmeFreeMemory(pAd, pOutBuffer);

    pAd->Mlme.LeapMachine.CurrState = LEAP_WAIT_CHANLLENGE_REQUEST;
    MlmeFreeMemory(pAd, pLeapOutBuffer);

    DBGPRINT(RT_DEBUG_TRACE, ("<== LeapIdentityAction\n"));
}

/*
    ========================================================================

    Routine Description:
        Send Authticator Challenge Response Packet to Authenticator

    Arguments:
        pAd         Pointer to our adapter
        *Elem       Pointer queue message
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapPeerChallengeAction(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem)
{
    HEADER_802_11       Header_802_11;
    ULONG               FrameLen = 0;
//  UCHAR               ApAddr[6];
    PUCHAR              pOutBuffer = NULL;
    PUCHAR              pLeapOutBuffer;
    NDIS_STATUS         NStatus;
    UCHAR               Identifier;
    PUCHAR              pData = NULL;
    PLEAP_EAPOL_HEADER  pLeap_EAPOL_Header = NULL;
    PLEAP_EAPOL_PACKET  pLeap_EAPOL_Packet = NULL;
    PLEAP_EAP_CONTENTS  pLeap_EAP_Contents = NULL;

    UCHAR               hash1[16];
    UCHAR               ChallengeResponseToRadius[24];
    USHORT              EapLen = 0;

    DBGPRINT(RT_DEBUG_TRACE, ("==> LeapPeerChallengeAction\n"));
    // allocate for send out EAPOL-Identify Response frame
    NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);  //Get an unused nonpaged memory
    if (NStatus != NDIS_STATUS_SUCCESS)
    {
        DBGPRINT(RT_DEBUG_TRACE, ("Send Challenge response failed, cause by not enough memory\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("==> LeapPeerChallengeAction\n"));
        return;
    }

    // allocate for EAPOL-Identify frame;
    NStatus = MlmeAllocateMemory(pAd, &pLeapOutBuffer);  //Get an unused nonpaged memory
    if (NStatus != NDIS_STATUS_SUCCESS)
    {
        MlmeFreeMemory(pAd, pOutBuffer);
        DBGPRINT(RT_DEBUG_TRACE, ("Send Challenge response failed, cause by not enough memory\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("==> LeapPeerChallengeAction\n"));
        return;
    }

    pLeap_EAPOL_Header = (PLEAP_EAPOL_HEADER) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H)];
    pLeap_EAPOL_Packet = (PLEAP_EAPOL_PACKET) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H + sizeof(LEAP_EAPOL_HEADER))];

    //keep original identifier
    if ((pLeap_EAPOL_Header != NULL) && (pLeap_EAPOL_Packet != NULL))
        Identifier = pLeap_EAPOL_Packet->Identifier;
    else
    {
        MlmeFreeMemory(pAd, pOutBuffer);
        MlmeFreeMemory(pAd, pLeapOutBuffer);
        DBGPRINT(RT_DEBUG_TRACE, ("EAPOL Frame can't be NULL\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("==> LeapPeerChallengeAction\n"));
        return;
    }

    pData = (PUCHAR) pLeap_EAPOL_Packet;
    pLeap_EAP_Contents = (PLEAP_EAP_CONTENTS) (pData + sizeof(LEAP_EAPOL_PACKET));
    //if (pLeap_EAP_Contents->Length != 8) return Failed;
    NdisMoveMemory(pAd->StaCfg.PeerChallenge, (PUCHAR)pLeap_EAP_Contents + sizeof(LEAP_EAP_CONTENTS), 8);

    HashPwd(pAd->StaCfg.LeapAuthInfo.Password, pAd->StaCfg.LeapAuthInfo.PasswordLen, hash1);  //7 ==> Passwd Len
    NdisMoveMemory(pAd->StaCfg.HashPwd, hash1, 16);
 
    PeerChallengeResponse((PUCHAR) pLeap_EAP_Contents + sizeof(LEAP_EAP_CONTENTS), hash1, ChallengeResponseToRadius);
    NdisMoveMemory(pAd->StaCfg.PeerChallengeResponse, ChallengeResponseToRadius, LEAP_CHALLENGE_RESPONSE_LENGTH);

    pLeap_EAPOL_Header = (PLEAP_EAPOL_HEADER) pLeapOutBuffer;
    pLeap_EAPOL_Packet = (PLEAP_EAPOL_PACKET) (pLeapOutBuffer + sizeof(LEAP_EAPOL_HEADER));

    NdisZeroMemory(pLeap_EAPOL_Header, sizeof(LEAP_EAPOL_HEADER));
    NdisZeroMemory(pLeap_EAPOL_Packet, sizeof(LEAP_EAPOL_PACKET));

    pData = (PUCHAR) pLeap_EAPOL_Packet;
    pLeap_EAP_Contents = (PLEAP_EAP_CONTENTS) (pData + sizeof(LEAP_EAPOL_PACKET));

    NdisZeroMemory(pLeap_EAP_Contents, sizeof(LEAP_EAP_CONTENTS));
    pLeap_EAP_Contents->Version = 0x01;
    pLeap_EAP_Contents->Reserved = 0x00;
    pLeap_EAP_Contents->Length = LEAP_CHALLENGE_RESPONSE_LENGTH;   
    EapLen += sizeof(LEAP_EAP_CONTENTS);
    NdisMoveMemory((PUCHAR)pLeap_EAP_Contents + sizeof(LEAP_EAP_CONTENTS), ChallengeResponseToRadius, LEAP_CHALLENGE_RESPONSE_LENGTH);
    EapLen += LEAP_CHALLENGE_RESPONSE_LENGTH;
    NdisMoveMemory((PUCHAR)pLeap_EAP_Contents + sizeof(LEAP_EAP_CONTENTS) + LEAP_CHALLENGE_RESPONSE_LENGTH, pAd->StaCfg.LeapAuthInfo.UserName, pAd->StaCfg.LeapAuthInfo.UserNameLen);
    EapLen += (USHORT) pAd->StaCfg.LeapAuthInfo.UserNameLen;

    pLeap_EAPOL_Packet->Code = 0x02; //Response
    pLeap_EAPOL_Packet->Identifier = Identifier;
    pLeap_EAPOL_Packet->Type = 0x11; //LEAP
    EapLen += sizeof(LEAP_EAPOL_PACKET);
    pLeap_EAPOL_Packet->Length[0] = (UCHAR) (EapLen / 256);
    pLeap_EAPOL_Packet->Length[1] = (UCHAR) (EapLen % 256);

    pLeap_EAPOL_Header->Version = 0x01;
    pLeap_EAPOL_Header->Type = 0x00; //EAP - Packet
    pLeap_EAPOL_Header->Length[0] = (UCHAR) (EapLen / 256);
    pLeap_EAPOL_Header->Length[1] = (UCHAR) (EapLen % 256);
    EapLen += sizeof(LEAP_EAPOL_HEADER);

    LeapMacHeaderInit(pAd, &Header_802_11, 0, pAd->CommonCfg.Bssid);
    MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
                      sizeof(HEADER_802_11),            &Header_802_11,
                      sizeof(EAP_HEADER),               EAP_HEADER,
                      EapLen,                           pLeap_EAPOL_Header,
                      END_OF_ARGS);
    MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
	MlmeFreeMemory(pAd, pOutBuffer);

    pAd->Mlme.LeapMachine.CurrState = LEAP_WAIT_SUCCESS;
    MlmeFreeMemory(pAd, pLeapOutBuffer);

    DBGPRINT(RT_DEBUG_TRACE, ("<== LeapPeerChallengeAction\n"));
}

/*
    ========================================================================

    Routine Description:
        Hash Password

    Arguments:
        pwd         Pointer password
        pwdlen      Password length
        hash        Pointer password hashed
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID HashPwd(
    IN  PUCHAR  pwd,
    IN  INT     pwdlen,
    OUT PUCHAR  hash)
{
    MD4_CTX     md4Context;
    UCHAR       unicodepwd[256 * 2];
    INT         i;

    NdisZeroMemory(hash, 16);
    NdisZeroMemory(unicodepwd, sizeof(unicodepwd));
    for (i=0 ; i < pwdlen; i++)
        unicodepwd[i*2] = *pwd++;
    MD4Init(&md4Context);
    MD4Update(&md4Context, unicodepwd, pwdlen * 2);
    MD4Final(hash, &md4Context);
}

/*
    ========================================================================

    Routine Description:
        Build Challenge Response frame

    Arguments:
        szChallenge     Pointer to Challenge frame
        smbPasswd       Pointer to Hashed Password
        szResponse      Pointer to Response frame
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID PeerChallengeResponse(
    IN  PUCHAR  szChallenge,
    IN  PUCHAR  smbPasswd,
    OUT PUCHAR  szResponse)
{
    CHAR    szMD4[21];

    /* initialize hash string */
    NdisZeroMemory(szMD4, 21);

    NdisMoveMemory(szMD4, smbPasswd, 16);
    DesEncrypt(szChallenge, szMD4, szResponse);
    DesEncrypt(szChallenge, szMD4 + 7, szResponse + 8);
    DesEncrypt(szChallenge, szMD4 + 14, szResponse + 16);
}

VOID ParityKey(
    OUT PUCHAR  szOut,
    IN  PUCHAR  szIn)
{
    INT     i;
    UCHAR   cNext = 0;
    UCHAR   cWorking = 0;

    for (i = 0; i < 7; i++)
    {
        cWorking = szIn[i];
        szOut[i] = (cWorking >> i) | cNext | 1;
        cWorking = szIn[i];
        cNext = (cWorking << (7 - i));
    }
    szOut[i] = cNext | 1;
}

/* Generate key schedule for encryption or decryption
 * depending on the value of "decrypt"
 */
VOID DesKey(
    OUT ULONG   k[16][2],   // Key schedule array
    IN  PUCHAR  key,        // 64 bits (will use only 56)
    IN  INT     decrypt)    // 0 = encrypt, 1 = decrypt
{
    UCHAR   pc1m[56]; // place to modify pc1 into
    UCHAR   pcr[56];  // place to rotate pc1 into
    INT     i,j,l;
    INT     m;
    UCHAR   ks[8];

    for (j = 0; j < 56; j++) 
    {                           // convert pc1 to bits of key
        l = pc1[j]-1;           // integer bit location
        m = l & 07;             // find bit
         // find which key byte l is in and which bit of that byte and store 1-bit result
        pc1m[j]=(key[l>>3] & bytebit[m]) ? 1 : 0;
    }

    for ( i = 0; i < 16; i++) 
    {                                   // key chunk for each iteration
        NdisZeroMemory(ks, sizeof(ks)); // Clear key schedule
        for ( j = 0; j < 56; j++)       // rotate pc1 the right amount
            pcr[j] = pc1m[(l=j+totrot[decrypt? 15-i : i])<(j<28? 28 : 56) ? l: l-28];
        // rotate left and right halves independently
        for (j=0; j<48; j++)
        {   // select bits individually
            // check bit that goes to ks[j]
            if (pcr[pc2[j]-1])
            {
                // mask it in if it's there
                l= j % 6;
                ks[j/6] |= bytebit[l] >> 2;
            }
        }
        // Now convert to packed odd/even interleaved form
        k[i][0] = ((long)ks[0] << 24) | ((long)ks[2] << 16) | ((long)ks[4] << 8) | ((long)ks[6]);
        k[i][1] = ((long)ks[1] << 24) | ((long)ks[3] << 16) | ((long)ks[5] << 8) | ((long)ks[7]);
        if(Asmversion)
        {
            // The assembler versions pre-shift each subkey 2 bits
            // so the Spbox indexes are already computed
            k[i][0] <<= 2;
            k[i][1] <<= 2;
        }
    }
}

// Encrypt or decrypt a block of data in ECB mode 
VOID Des(
    IN  ULONG   ks[16][2], // Key schedule
    OUT UCHAR   block[8])  // Data block 
{
    UINT32 left,right,work;

    // Read input block and place in left/right in big-endian order
    left = ((unsigned long)block[0] << 24)
     | ((unsigned long)block[1] << 16)
     | ((unsigned long)block[2] << 8)
     | (unsigned long)block[3];
    right = ((unsigned long)block[4] << 24)
     | ((unsigned long)block[5] << 16)
     | ((unsigned long)block[6] << 8)
     | (unsigned long)block[7];

    // Hoey's clever initial permutation algorithm, from Outerbridge
    // (see Schneier p 478)
    //
    // The convention here is the same as Outerbridge: rotate each
    // register left by 1 bit, i.e., so that "left" contains permuted
    // input bits 2, 3, 4, ... 1 and "right" contains 33, 34, 35, ... 32
    // (using origin-1 numbering as in the FIPS). This allows us to avoid
    // one of the two rotates that would otherwise be required in each of
    // the 16 rounds.
    //
    work = ((left >> 4) ^ right) & 0x0f0f0f0f;
    right ^= work;
    left ^= work << 4;
    work = ((left >> 16) ^ right) & 0xffff;
    right ^= work;
    left ^= work << 16;
    work = ((right >> 2) ^ left) & 0x33333333;
    left ^= work;
    right ^= (work << 2);
    work = ((right >> 8) ^ left) & 0xff00ff;
    left ^= work;
    right ^= (work << 8);
    right = (right << 1) | (right >> 31);
    work = (left ^ right) & 0xaaaaaaaa;
    left ^= work;
    right ^= work;
    left = (left << 1) | (left >> 31);

    // Now do the 16 rounds
    F(left,right,ks[0]);
    F(right,left,ks[1]);
    F(left,right,ks[2]);
    F(right,left,ks[3]);
    F(left,right,ks[4]);
    F(right,left,ks[5]);
    F(left,right,ks[6]);
    F(right,left,ks[7]);
    F(left,right,ks[8]);
    F(right,left,ks[9]);
    F(left,right,ks[10]);
    F(right,left,ks[11]);
    F(left,right,ks[12]);
    F(right,left,ks[13]);
    F(left,right,ks[14]);
    F(right,left,ks[15]);

    // Inverse permutation, also from Hoey via Outerbridge and Schneier
    right = (right << 31) | (right >> 1);
    work = (left ^ right) & 0xaaaaaaaa;
    left ^= work;
    right ^= work;
    left = (left >> 1) | (left  << 31);
    work = ((left >> 8) ^ right) & 0xff00ff;
    right ^= work;
    left ^= work << 8;
    work = ((left >> 2) ^ right) & 0x33333333;
    right ^= work;
    left ^= work << 2;
    work = ((right >> 16) ^ left) & 0xffff;
    left ^= work;
    right ^= work << 16;
    work = ((right >> 4) ^ left) & 0x0f0f0f0f;
    left ^= work;
    right ^= work << 4;

    // Put the block back into the user's buffer with final swap
    block[0] = (UCHAR) (right >> 24);
    block[1] = (UCHAR) (right >> 16);
    block[2] = (UCHAR) (right >> 8);
    block[3] = (UCHAR) (right);
    block[4] = (UCHAR) (left >> 24);
    block[5] = (UCHAR) (left >> 16);
    block[6] = (UCHAR) (left >> 8);
    block[7] = (UCHAR) (left);
}

VOID DesEncrypt(
    IN  PUCHAR  szClear,
    IN  PUCHAR  szKey,
    OUT PUCHAR  szOut)
{
    CHAR    szParityKey[9];
    ULONG   ulK[16][2];

    ParityKey(szParityKey, szKey); //Insert parity bits

    NdisMoveMemory(szOut, szClear, 8);// des encrypts in place
    DesKey(ulK, (PUCHAR)szParityKey, 0);  //generate keypair
    Des(ulK, szOut); //encrypt
}

/*
    ========================================================================

    Routine Description:
        Send Challenge Resquest Packet to Authenticator, to Verify Authticator

    Arguments:
        pAd         Pointer to our adapter
        *Elem       Pointer queue message
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapNetworkChallengeAction(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem)
{
    HEADER_802_11       Header_802_11;
    ULONG               FrameLen = 0;
//  UCHAR               ApAddr[6];
    PUCHAR              pOutBuffer = NULL;
    PUCHAR              pLeapOutBuffer;
    NDIS_STATUS         NStatus;
    PLEAP_EAPOL_HEADER  pLeap_EAPOL_Header = NULL;
    PLEAP_EAPOL_PACKET  pLeap_EAPOL_Packet = NULL;
    PLEAP_EAP_CONTENTS  pLeap_EAP_Contents = NULL;

    USHORT              EapLen = 0;
    INT                 i;
    PUCHAR              pData;
    UCHAR				Identifier = 0;

    DBGPRINT(RT_DEBUG_TRACE, ("==> LeapNetworkChallengeAction\n"));
    // allocate for send out EAPOL-Identify Response frame
    NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);  //Get an unused nonpaged memory
    if (NStatus != NDIS_STATUS_SUCCESS)
    {
        DBGPRINT(RT_DEBUG_TRACE, ("Send Challenge request failed, cause by not enough memory\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("<== LeapNetworkChallengeAction\n"));
        return;
    }

    // allocate for EAPOL-Identify frame;
    NStatus = MlmeAllocateMemory(pAd, &pLeapOutBuffer);  //Get an unused nonpaged memory
    if (NStatus != NDIS_STATUS_SUCCESS)
    {
        MlmeFreeMemory(pAd, pOutBuffer);
        DBGPRINT(RT_DEBUG_TRACE, ("Send Challenge request failed, cause by not enough memory\n"));
        DBGPRINT(RT_DEBUG_TRACE, ("<== LeapNetworkChallengeAction\n"));
        return;
    }

    //
    // Get original Identifier.
    //
    pLeap_EAPOL_Packet  = (PLEAP_EAPOL_PACKET) &Elem->Msg[LENGTH_802_11 + LENGTH_802_1_H + sizeof(LEAP_EAPOL_HEADER)];
    Identifier = pLeap_EAPOL_Packet->Identifier;

    //
    // Build new LEAP request.
    //
    pLeap_EAPOL_Header = (PLEAP_EAPOL_HEADER) pLeapOutBuffer;
    pLeap_EAPOL_Packet = (PLEAP_EAPOL_PACKET) (pLeapOutBuffer + sizeof(LEAP_EAPOL_HEADER));

    NdisZeroMemory(pLeap_EAPOL_Header, sizeof(LEAP_EAPOL_HEADER));
    NdisZeroMemory(pLeap_EAPOL_Packet, sizeof(LEAP_EAPOL_PACKET));

    pLeap_EAP_Contents = (PLEAP_EAP_CONTENTS) ((PUCHAR)pLeap_EAPOL_Packet + sizeof(LEAP_EAPOL_PACKET));
    pLeap_EAP_Contents->Version = 0x01;
    pLeap_EAP_Contents->Reserved = 0x00;
    pLeap_EAP_Contents->Length = LEAP_CHALLENGE_REQUEST_LENGTH;
    EapLen += sizeof(LEAP_EAP_CONTENTS);  //Contents header
    EapLen += pLeap_EAP_Contents->Length;  //PeerChallenge length
    pData = (PUCHAR) pLeap_EAP_Contents + sizeof(LEAP_EAP_CONTENTS);
    for (i = 0; i < LEAP_CHALLENGE_REQUEST_LENGTH; i++)
        *pData++ = RandomByte(pAd);
    NdisMoveMemory((PUCHAR)pLeap_EAP_Contents + sizeof(LEAP_EAP_CONTENTS) + LEAP_CHALLENGE_REQUEST_LENGTH, pAd->StaCfg.LeapAuthInfo.UserName, pAd->StaCfg.LeapAuthInfo.UserNameLen);
    NdisMoveMemory(pAd->StaCfg.NetworkChallenge, (PUCHAR)pLeap_EAP_Contents + sizeof(LEAP_EAP_CONTENTS), LEAP_CHALLENGE_REQUEST_LENGTH); //challenge len=8
    EapLen += (USHORT) pAd->StaCfg.LeapAuthInfo.UserNameLen; //User name length;

    pLeap_EAPOL_Packet->Code = 0x01; //Request;
    //
    // Identifier can't be Random byte.
    // compatibility issue with Funk server.
    //
    pLeap_EAPOL_Packet->Identifier = Identifier;
    pLeap_EAPOL_Packet->Type = 0x11; //LEAP frame;
    EapLen += sizeof(LEAP_EAPOL_PACKET); 
    pLeap_EAPOL_Packet->Length[0] = (UCHAR) (EapLen / 256);
    pLeap_EAPOL_Packet->Length[1] = (UCHAR) (EapLen % 256);

    pLeap_EAPOL_Header->Version = 0x01;
    pLeap_EAPOL_Header->Type = 0x00; //EAP - Packet
    pLeap_EAPOL_Header->Length[0] = (UCHAR) (EapLen / 256);
    pLeap_EAPOL_Header->Length[1] = (UCHAR) (EapLen % 256);
    EapLen += sizeof(LEAP_EAPOL_HEADER);

    LeapMacHeaderInit(pAd, &Header_802_11, 0, pAd->CommonCfg.Bssid);
    MakeOutgoingFrame(pOutBuffer,                       &FrameLen, 
                      sizeof(HEADER_802_11),            &Header_802_11,
                      sizeof(EAP_HEADER),               EAP_HEADER,
                      EapLen,                           pLeap_EAPOL_Header,
                      END_OF_ARGS);
    MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
	MlmeFreeMemory(pAd, pOutBuffer);

    pAd->Mlme.LeapMachine.CurrState = LEAP_WAIT_CHANLLENGE_RESPONSE;
    MlmeFreeMemory(pAd, pLeapOutBuffer);
    DBGPRINT(RT_DEBUG_TRACE, ("<== LeapNetworkChallengeAction\n"));
}

/*
    ========================================================================

    Routine Description:
        Verify Authenticator's Challenge response Packet, process Session key

    Arguments:
        pAd         Pointer to our adapter
        *Elem       Pointer queue message
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapNetworkChallengeResponse(
    IN  PRTMP_ADAPTER   pAd, 
    IN  MLME_QUEUE_ELEM *Elem)
{
    ULONG                       FrameLen = 0;
//  UCHAR                       ApAddr[6];
//  PUCHAR                      pOutBuffer = NULL;
//  PUCHAR                      pLeapOutBuffer;
    PLEAP_EAPOL_HEADER          pLeap_EAPOL_Header = NULL;
    PLEAP_EAPOL_PACKET          pLeap_EAPOL_Packet = NULL;
    PLEAP_EAP_CONTENTS          pLeap_EAP_Contents = NULL;
    MLME_QUEUE_ELEM             ElemDeauththenticationMsg;
    MLME_DEAUTH_REQ_STRUCT      *pInfo;
    BOOLEAN                     TimerCancelled;
    UCHAR                       hash2[16];
    UCHAR                       ChallengeResponseFromRadius[24];
    USHORT                      EapLen = 0;

    DBGPRINT(RT_DEBUG_TRACE, ("==> LeapNetworkChallengeResponse\n"));
    pLeap_EAPOL_Header = (PLEAP_EAPOL_HEADER) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H)];
    pLeap_EAPOL_Packet = (PLEAP_EAPOL_PACKET) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H + sizeof(LEAP_EAPOL_HEADER))];
    pLeap_EAP_Contents = (PLEAP_EAP_CONTENTS) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H + sizeof(LEAP_EAPOL_HEADER) + sizeof(LEAP_EAPOL_PACKET))];

    if (pLeap_EAPOL_Packet->Type != 0x11)  //LEAP
        return;
    if (pLeap_EAP_Contents->Length != LEAP_CHALLENGE_RESPONSE_LENGTH) //Length failed.
        return;

    NdisMoveMemory(pAd->StaCfg.NetworkChallengeResponse, (PUCHAR)pLeap_EAP_Contents + sizeof(LEAP_EAP_CONTENTS), pLeap_EAP_Contents->Length);

    HashpwdHash(pAd->StaCfg.HashPwd, hash2);
    // 
    // Verify Authenticator challenge response
    //
    PeerChallengeResponse(pAd->StaCfg.NetworkChallenge, hash2, ChallengeResponseFromRadius);

    if (!NdisEqualMemory(ChallengeResponseFromRadius, pAd->StaCfg.NetworkChallengeResponse, LEAP_CHALLENGE_RESPONSE_LENGTH))
    {
        // Verify Authenticator's cahllenge respone failed, possible Rogue AP. 
        // Add this AP to RogueApTable list, and Deauth it.
        DBGPRINT(RT_DEBUG_TRACE, ("Verify Authenticator's challenge response failed, possible Rogue AP\n"));
        RTMPCancelTimer(&pAd->StaCfg.LeapAuthTimer, &TimerCancelled);
        RogueApTableSetEntry(pAd, &pAd->StaCfg.RogueApTab, pAd->CommonCfg.Bssid, LEAP_REASON_CHALLENGE_TO_AP_FAILED);
        pInfo = (MLME_DEAUTH_REQ_STRUCT *) ElemDeauththenticationMsg.Msg;
        pInfo->Reason = REASON_DEAUTH_STA_LEAVING;
        COPY_MAC_ADDR(pInfo->Addr, pAd->CommonCfg.Bssid);
        MlmeDeauthReqAction(pAd, &ElemDeauththenticationMsg);
        if (INFRA_ON(pAd))
        {
            LinkDown(pAd, FALSE);
            pAd->Mlme.AssocMachine.CurrState = ASSOC_IDLE;
        }
        pAd->Mlme.LeapMachine.CurrState = LEAP_IDLE;
    }

    // 
    // Calculate Session Key.
    //
    ProcessSessionKey(pAd->StaCfg.SessionKey,
                      hash2, pAd->StaCfg.NetworkChallenge, pAd->StaCfg.NetworkChallengeResponse,
                      pAd->StaCfg.PeerChallenge, pAd->StaCfg.PeerChallengeResponse);

    pAd->Mlme.LeapMachine.CurrState = LEAP_WAIT_EAPOLKEY;

    DBGPRINT(RT_DEBUG_TRACE, ("LEAP SessionKey[%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X]\n",
        pAd->StaCfg.SessionKey[0], pAd->StaCfg.SessionKey[1], pAd->StaCfg.SessionKey[2],
        pAd->StaCfg.SessionKey[3], pAd->StaCfg.SessionKey[4], pAd->StaCfg.SessionKey[5],
        pAd->StaCfg.SessionKey[6], pAd->StaCfg.SessionKey[7], pAd->StaCfg.SessionKey[8],
        pAd->StaCfg.SessionKey[9], pAd->StaCfg.SessionKey[10], pAd->StaCfg.SessionKey[11],
        pAd->StaCfg.SessionKey[12]));
    DBGPRINT(RT_DEBUG_TRACE, ("<== LeapNetworkChallengeResponse\n"));
}

VOID HashpwdHash(
    IN  PUCHAR  hash,
    IN  PUCHAR  hashhash)
{
    MD4_CTX     md4Context;

    MD4Init(&md4Context);
    MD4Update(&md4Context, hash, 16);
    MD4Final(hashhash, &md4Context);
}

VOID ProcessSessionKey(
    OUT PUCHAR  SessionKey,
    IN  PUCHAR  hash2, 
    IN  PUCHAR  ChallengeToRadius, 
    IN  PUCHAR  ChallengeResponseFromRadius,
    IN  PUCHAR  ChallengeFromRadius, 
    IN  PUCHAR  ChallengeResponseToRadius)
{
    MD5_CTX context;
    UCHAR str[80];

    NdisMoveMemory(str, hash2, 16);
    NdisMoveMemory(str+16, ChallengeToRadius, 8);
    NdisMoveMemory(str+16+8, ChallengeResponseFromRadius, 24);
    NdisMoveMemory(str+16+8+24, ChallengeFromRadius, 8);
    NdisMoveMemory(str+16+8+24+8, ChallengeResponseToRadius, 24);

    MD5Init (&context);
    MD5Update (&context, str, 80);
    MD5Final (SessionKey, &context);
}
/*
    ========================================================================

    Routine Description:
        Process EAPOL-key

    Arguments:
        pAd         Pointer to our adapter
        *Elem       Pointer queue message
        
    Return Value:
        None 
        
    Note:
        After Get the EAPOL-Key set the following
          pAd->StaCfg.PortSecured = WPA_802_1X_PORT_SECURED
        
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapEapolKeyAction(
    IN PRTMP_ADAPTER    pAd, 
    IN MLME_QUEUE_ELEM  *Elem)
{
    PEAPOL_KEY_HEADER       pKeyHeader = NULL;
    PLEAP_EAPOL_HEADER      pLeap_EAPOL_Header = NULL;
    UCHAR                   Signature[16];
    USHORT                  KeypacketLen;
    USHORT                  KeyLen;
    UCHAR                   digest[16];
    UCHAR                   Enckey[16];
    BOOLEAN                 BroadCastKey = FALSE;
    ARCFOURCONTEXT          Ctx;
    UCHAR                   RC4Key[32];
    UCHAR                   KeyIdx;
    BOOLEAN          TimerCancelled;
	UCHAR                   CipherAlg;

    DBGPRINT(RT_DEBUG_TRACE, ("==> LeapEapolKeyAction\n"));
    RTMPCancelTimer(&pAd->StaCfg.LeapAuthTimer, &TimerCancelled);

    pLeap_EAPOL_Header = (PLEAP_EAPOL_HEADER) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H)];
    pKeyHeader = (PEAPOL_KEY_HEADER) &Elem->Msg[(LENGTH_802_11 + LENGTH_802_1_H + sizeof(LEAP_EAPOL_HEADER))];

    KeypacketLen = (pLeap_EAPOL_Header->Length[0] << 8) + pLeap_EAPOL_Header->Length[1] + sizeof(LEAP_EAPOL_HEADER);
    NdisMoveMemory(Signature, pKeyHeader->Signature, 16);
    NdisMoveMemory(RC4Key, pKeyHeader->IV, 16);
    NdisMoveMemory(RC4Key + 16, pAd->StaCfg.SessionKey, 16);
    NdisZeroMemory(pKeyHeader->Signature, 16);
    KeyLen = ((pKeyHeader->Length[0]) << 8) + pKeyHeader->Length[1];
    if ((KeypacketLen - sizeof(LEAP_EAPOL_HEADER) - sizeof(EAPOL_KEY_HEADER)) > 0)
    {
        BroadCastKey = TRUE;
        NdisMoveMemory(Enckey, (PUCHAR)pKeyHeader + sizeof(EAPOL_KEY_HEADER), KeyLen);
    }

    hmac_md5(pAd->StaCfg.SessionKey, 16, (PUCHAR)pLeap_EAPOL_Header, KeypacketLen, digest);

    if (!NdisEqualMemory(Signature, digest, 16))
    {
        DBGPRINT(RT_DEBUG_TRACE, ("Signature verify failed \n"));
        return;
    }

    KeyIdx = pKeyHeader->Index & 0x7F;
    if (KeyIdx >= 4)
        return; //Bad key

    if (pKeyHeader->Index & 0x80)
        pAd->CommonCfg.DefaultKeyId = (UCHAR) KeyIdx;

    if (BroadCastKey)
    { //Decrypt Key
        ARCFOUR_INIT(&Ctx, RC4Key, 32);
        ARCFOUR_DECRYPT(&Ctx, Enckey, Enckey, KeyLen);
        //Set Key.
        pAd->SharedKey[BSS0][KeyIdx].KeyLen = (UCHAR) KeyLen;
		//
		// Check CKIP property
		//
		if (pAd->StaCfg.CkipFlag & 0x10) // Cisco CKIP KP is on
		{
			CipherAlg = (pAd->SharedKey[BSS0][KeyIdx].KeyLen == 5)? CIPHER_CKIP64 : CIPHER_CKIP128;
		}
		else 
		{
			CipherAlg = (pAd->SharedKey[BSS0][KeyIdx].KeyLen == 5)? CIPHER_WEP64 : CIPHER_WEP128;
		}

        NdisMoveMemory(pAd->SharedKey[BSS0][KeyIdx].Key, Enckey, KeyLen);
		pAd->SharedKey[BSS0][KeyIdx].CipherAlg = CipherAlg;		

		AsicAddKeyEntry(pAd, 0, BSS0, (UCHAR)KeyIdx, &pAd->SharedKey[BSS0][KeyIdx], FALSE, FALSE);
    }
    else
    { //Session Key.
        //Set Key.
        pAd->SharedKey[BSS0][KeyIdx].KeyLen = (UCHAR) KeyLen;
		//
		// Check CKIP property
		//
		if (pAd->StaCfg.CkipFlag & 0x10) // Cisco CKIP KP is on
		{
			CipherAlg = (pAd->SharedKey[BSS0][KeyIdx].KeyLen == 5)? CIPHER_CKIP64 : CIPHER_CKIP128;
		}
		else 
		{
			CipherAlg = (pAd->SharedKey[BSS0][KeyIdx].KeyLen == 5)? CIPHER_WEP64 : CIPHER_WEP128;
		}

        NdisMoveMemory(pAd->SharedKey[BSS0][KeyIdx].Key, pAd->StaCfg.SessionKey, KeyLen);
		pAd->SharedKey[BSS0][KeyIdx].CipherAlg = CipherAlg;		

#if 0
		AsicAddSharedKeyEntry(pAd, BSS0, (UCHAR)KeyIdx, CipherAlg, pAd->SharedKey[BSS0][KeyIdx].Key, NULL, NULL);
#endif
		AsicAddKeyEntry(pAd, BSSID_WCID, BSS0, KeyIdx, &pAd->SharedKey[BSS0][KeyIdx], FALSE, TRUE);
		AsicUpdateRxWCIDTable(pAd, BSSID_WCID, pAd->CommonCfg.Bssid);

#if 0
		AsicAddPairwiseKeyEntry(pAd, 
								pAd->CommonCfg.Bssid, 
								BSSID_WCID, 
								&pAd->SharedKey[BSS0][KeyIdx],
								FALSE);
#endif
        LeapSendRogueAPReport(pAd);
    }

    pAd->StaCfg.PortSecured = WPA_802_1X_PORT_SECURED;

    DBGPRINT(RT_DEBUG_TRACE, ("<== LeapEapolKeyAction\n"));
}

/*
    ========================================================================

    Routine Description:
        Init Cisco CCX RogueAP Table

    Arguments:
        *Tab         Pointer to RogueAP Table
        
    Return Value:
        None 
        
    Note:
      
        This function implement Cisco CCX 1.X Rogue AP
    ========================================================================
*/
VOID RogueApTableInit(
    IN PROGUEAP_TABLE    pTab) 
{
    int i;

    pTab->RogueApNr = 0;
    for (i = 0; i < MAX_LEN_OF_BSS_TABLE; i++) 
    {
        NdisZeroMemory(&pTab->RogueApEntry[i], sizeof(ROGUEAP_ENTRY));
    }
}

/*
    ========================================================================

    Routine Description:
        Search Cisco CCX RogueAP Table

    Arguments:
        *Tab         Pointer to RogueAP Table
        pAddr        Pointer to RogueAp Mac address
        
    Return Value:
        None 
        
    Note:
      
        This function implement Cisco CCX 1.X Rogue AP
    ========================================================================
*/
ULONG RogueApTableSearch(
    IN PROGUEAP_TABLE   pTab, 
    IN PUCHAR           pAddr) 
{
    UCHAR i;
    
    for (i = 0; i < pTab->RogueApNr; i++) 
    {
        //printf("comparinpg %s and %s\n", p_tab->bss[i].ssid, ssid);
        if (MAC_ADDR_EQUAL(pTab->RogueApEntry[i].Addr, pAddr)) 
        { 
            return i;
        }
    }
    return (ULONG)BSS_NOT_FOUND;
}
/*
    ========================================================================

    Routine Description:
        Set Cisco CCX RogueAP to RogueAP Table list 

    Arguments:
        pAd          Pointer to our adapter
        *pRogueAp    Pointer to RogueAP Entry Table
        pAddr        Pointer to RogueAp Mac address
        FailCode     RogueAP type
        
    Return Value:
        None 
        
    Note:
      
        This function implement Cisco CCX 1.X Rogue AP
    ========================================================================
*/
VOID RogueApEntrySet(
    IN  PRTMP_ADAPTER   pAd,
    OUT ROGUEAP_ENTRY   *pRogueAp,
    IN  PUCHAR          pAddr,
    IN  UCHAR           FaileCode)
{
    COPY_MAC_ADDR(pRogueAp->Addr, pAddr);
    
    pRogueAp->ErrorCode[0] = 0x00;
    pRogueAp->ErrorCode[1] = FaileCode;

}

/*
    ========================================================================

    Routine Description:
        Set Cisco CCX RogueAP to RogueAP Table list 

    Arguments:
        pAd          Pointer to our adapter
        *Tab         Pointer to RogueAP Table
        pAddr        Pointer to RogueAp Mac address
        FailCode     RogueAP type
        
    Return Value:
        None 
        
    Note:
      
        This function implement Cisco CCX 1.X Rogue AP
    ========================================================================
*/	
ULONG RogueApTableSetEntry(
    IN  PRTMP_ADAPTER   pAd,
    OUT ROGUEAP_TABLE   *Tab,
    IN  PUCHAR          pAddr,
    IN  UCHAR           FaileCode)
{
    ULONG   Idx;

    Idx = RogueApTableSearch(Tab, pAddr);
    if (Idx == BSS_NOT_FOUND) 
    {
        if (Tab->RogueApNr >= MAX_LEN_OF_BSS_TABLE)
            return BSS_NOT_FOUND;
        Idx = Tab->RogueApNr;
        RogueApEntrySet(pAd, &Tab->RogueApEntry[Idx], pAddr, FaileCode);
        Tab->RogueApNr++;
    }
    else
    {
        RogueApEntrySet(pAd, &Tab->RogueApEntry[Idx], pAddr, FaileCode);
    }

    DBGPRINT(RT_DEBUG_TRACE, ("Add:Rogue AP[%02X:%02X:%02X:%02X:%02X:%02X] Code:%d\n",
		pAddr[0], pAddr[1], pAddr[2],
		pAddr[3], pAddr[4], pAddr[5], FaileCode));

    return Idx;
}

/*
    ========================================================================

    Routine Description:
        delete Cisco CCX RogueAP from RogueAP Table list 

    Arguments:
        *Tab         Pointer to RogueAP Table
        pAddr        Pointer to RogueAp Mac address
        
    Return Value:
        None 
        
    Note:
      
        This function implement Cisco CCX 1.X Rogue AP
    ========================================================================
*/

VOID RogueApTableDeleteEntry(
    IN OUT  ROGUEAP_TABLE   *Tab, 
    IN      PUCHAR          pAddr) 
{
    UCHAR i, j;
    
    for (i = 0; i < Tab->RogueApNr; i++) 
    {
        if (MAC_ADDR_EQUAL(Tab->RogueApEntry[i].Addr, pAddr)) 
        {
            for (j = i; j < Tab->RogueApNr - 1; j++)
            {
                NdisMoveMemory(&(Tab->RogueApEntry[j]), &(Tab->RogueApEntry[j + 1]), sizeof(ROGUEAP_ENTRY));
            }
            Tab->RogueApNr -= 1;
            return;
        }
    }
}

/*
    ========================================================================

    Routine Description:
        Leap Authentication timeout call backup function

    Arguments:
        FunctionContext   Pointer to our adapter
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X LEAP Support.
    ========================================================================
*/
VOID LeapAuthTimeout(
    IN PVOID    SystemSpecific1, 
    IN PVOID    FunctionContext, 
    IN PVOID    SystemSpecific2, 
    IN PVOID    SystemSpecific3) 
{
    MLME_QUEUE_ELEM             ElemDeauththenticationMsg;
    MLME_DEAUTH_REQ_STRUCT      *pInfo;

    RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)FunctionContext;

	
	// Do nothing if the driver is starting halt state.
	// This might happen when timer already been fired before cancel timer with mlmehalt
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST))
		return;
	
    DBGPRINT(RT_DEBUG_TRACE, ("==> LeapAuthTimeout\n"));
    RogueApTableSetEntry(pAd, &pAd->StaCfg.RogueApTab, pAd->CommonCfg.Bssid, LEAP_REASON_AUTH_TIMEOUT);
    //
    // Deauth it, for try next bssid.
    //
    pInfo = (MLME_DEAUTH_REQ_STRUCT *) ElemDeauththenticationMsg.Msg;
    pInfo->Reason = REASON_DEAUTH_STA_LEAVING;
    COPY_MAC_ADDR(pInfo->Addr, pAd->CommonCfg.Bssid);
    MlmeDeauthReqAction(pAd, &ElemDeauththenticationMsg);
    if (INFRA_ON(pAd))
    {
        LinkDown(pAd, FALSE);
        pAd->Mlme.AssocMachine.CurrState = ASSOC_IDLE;
    }
    pAd->Mlme.LeapMachine.CurrState = LEAP_IDLE;
    DBGPRINT(RT_DEBUG_TRACE, ("<== LeapAuthTimeout\n"));
}
/*
    ========================================================================

    Routine Description:
        Send Rogue AP to Authenticator

    Arguments:
        pAd         Pointer to our adapter
        
    Return Value:
        None 
        
    Note:
        This function implement Cisco CCX 1.X Rogue AP
    ========================================================================
*/
VOID LeapSendRogueAPReport(
    IN  PRTMP_ADAPTER   pAd)
{
    HEADER_802_11               Header_802_11;
    ULONG                       FrameLen = 0;	
    NDIS_STATUS                 NStatus;
    PUCHAR                      pOutBuffer = NULL;
    ULONG                       Length = 40;
    UCHAR                       MsgType = 0x40;
    UCHAR                       FunctionCode = 0x8E;
    UCHAR                       RogueAPName[16];
    UCHAR                       DDPLength[2];	
    UCHAR                       ckip_ck[16];
    UCHAR                       ckip_pk[16];     /* permuted key */	
    PUCHAR                      Ptr;
    INT                         i,j;

    DBGPRINT(RT_DEBUG_TRACE, ("==> LeapSendRogueAPReport\n"));
    //Delete Current Bssid (not Rogue AP) from Rogue AP list for remove False Rogue AP.
    RogueApTableDeleteEntry(&pAd->StaCfg.RogueApTab, pAd->CommonCfg.Bssid);

    for( i = 0; i < pAd->StaCfg.RogueApTab.RogueApNr; i++)
    {
        //Report RogueAP, if not reported in the RogueAP list.
        if (pAd->StaCfg.RogueApTab.RogueApEntry[i].Reported != TRUE)
        {
            NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);  //Get an unused nonpaged memory
            if (NStatus != NDIS_STATUS_SUCCESS)
            {
                DBGPRINT(RT_DEBUG_TRACE, ("Send DDP RogueAP list failed, cause not enough memory\n"));
                return; 
            }

            NdisZeroMemory(RogueAPName, 16);
            NdisZeroMemory(pOutBuffer, sizeof(HEADER_802_11) + 8 + 40);
            LeapMacHeaderInit(pAd, &Header_802_11, 1, pAd->CommonCfg.Bssid);
            MakeOutgoingFrame(pOutBuffer,                     &FrameLen, 
                              sizeof(HEADER_802_11),          &Header_802_11,
                              END_OF_ARGS);

            // Encrypt Rogue AP Detection DDP Packet 
            if (pAd->StaCfg.CkipFlag & 0x10)
            {//Cisco CKIP KP is on.
#ifdef RELASE_EXCLUDE
				DBGPRINT(RT_DEBUG_INFO, ("Using Ckip encrypt DDP packet\n"));
#endif // RELASE_EXCLUDE //
                if (pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen == 0)
                {
                    MlmeFreeMemory(pAd, pOutBuffer);                
                    DBGPRINT(RT_DEBUG_ERROR, ("LeapSendRogueAPReport#1588: Can't process Ckip key, keylen can't be zero\n"));
                    return;
                }

                for(j=0; j< (16 / pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen); j++)
                    NdisMoveMemory(ckip_ck+j*pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen, 
                                    pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].Key, 
                                    pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen);
                NdisMoveMemory(ckip_ck+j *pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen, 
                                pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].Key, 
                                16-(j * pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen));

                j = 2; 
                while (++pAd->StaCfg.GIV[j] == 0x0)
                {
                    j--;
                    if (j < 0)
                        break;
                }

                Ptr = (PUCHAR)&Header_802_11;
                CKIP_key_permute(ckip_pk, ckip_ck, *(Ptr+1) & 3, pAd->StaCfg.GIV);

                Ptr = pOutBuffer;
                Ptr += sizeof(HEADER_802_11);

                RTMPInitWepEngine(pAd,
                                  ckip_pk,
                                  pAd->CommonCfg.DefaultKeyId,
                                  16,
                                  Ptr);
            }
            else
            {
                Ptr = pOutBuffer;
                Ptr += sizeof(HEADER_802_11);

                RTMPInitWepEngine(pAd, 
                                  pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].Key,
                                  pAd->CommonCfg.DefaultKeyId,
                                  pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen, 
                                  Ptr);
            }

            Ptr += 4;
            FrameLen += 4; //IV
            RTMPEncryptData(pAd, SNAP_LEAP_ROGUEAP_HDR, Ptr, sizeof(SNAP_LEAP_ROGUEAP_HDR));
            Ptr += sizeof(SNAP_LEAP_ROGUEAP_HDR);
            FrameLen += sizeof(SNAP_LEAP_ROGUEAP_HDR);
            DDPLength[0] = (UCHAR) (Length / 256);
            DDPLength[1] = (UCHAR) (Length % 256);
            RTMPEncryptData(pAd, (PUCHAR) DDPLength, Ptr, 2);
            Ptr += 2;
            FrameLen += 2;
            RTMPEncryptData(pAd, (PUCHAR) &MsgType, Ptr, 1);
            Ptr += 1;
            FrameLen += 1;
            RTMPEncryptData(pAd, (PUCHAR) &FunctionCode, Ptr, 1);
            Ptr += 1;
            FrameLen +=1;
            RTMPEncryptData(pAd, pAd->CommonCfg.Bssid, Ptr, MAC_ADDR_LEN);
            Ptr += MAC_ADDR_LEN;
            FrameLen += MAC_ADDR_LEN;
            RTMPEncryptData(pAd, pAd->CurrentAddress, Ptr, MAC_ADDR_LEN);
            Ptr += MAC_ADDR_LEN;
            FrameLen += MAC_ADDR_LEN;
            RTMPEncryptData(pAd, (PUCHAR) &pAd->StaCfg.RogueApTab.RogueApEntry[i].ErrorCode, Ptr, 2);
            Ptr += 2;
            FrameLen += 2;
            RTMPEncryptData(pAd, (PUCHAR) &pAd->StaCfg.RogueApTab.RogueApEntry[i].Addr, Ptr, MAC_ADDR_LEN);
            Ptr += MAC_ADDR_LEN;
            FrameLen += MAC_ADDR_LEN;
            RTMPEncryptData(pAd, RogueAPName, Ptr, 16);
            Ptr += 16;
            FrameLen += 16;
            RTMPSetICV(pAd, Ptr);
            FrameLen += 4;  //ICV

			DBGPRINT(RT_DEBUG_TRACE, ("Report RogueAP:[%02X:%02X:%02X:%02X:%02X:%02X]Reson:%d\n", 
				pAd->StaCfg.RogueApTab.RogueApEntry[i].Addr[0], pAd->StaCfg.RogueApTab.RogueApEntry[i].Addr[1],
				pAd->StaCfg.RogueApTab.RogueApEntry[i].Addr[2], pAd->StaCfg.RogueApTab.RogueApEntry[i].Addr[3],
				pAd->StaCfg.RogueApTab.RogueApEntry[i].Addr[4], pAd->StaCfg.RogueApTab.RogueApEntry[i].Addr[5], 
				pAd->StaCfg.RogueApTab.RogueApEntry[i].ErrorCode[1]));

            MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
        	MlmeFreeMemory(pAd, pOutBuffer);
            pAd->StaCfg.RogueApTab.RogueApEntry[i].Reported = TRUE;
        }
    }

    //Clear RogueAP list Table for recording another report on next association.
    RogueApTableInit(&pAd->StaCfg.RogueApTab);
    DBGPRINT(RT_DEBUG_TRACE, ("<== LeapSendRogueAPReport\n"));
}

BOOLEAN CCKMAssocRspSanity(
    IN PRTMP_ADAPTER    pAd,
    IN VOID             *Msg,
    IN ULONG            MsgLen)
{
    NDIS_STATUS                 Status;
    PFRAME_802_11               pFrame = (PFRAME_802_11)Msg;
    PEID_STRUCT                 pEid;
    PUCHAR                      Ptr;
    UCHAR                       CalcMicBuffer[80];
    ULONG                       CalcMicBufferLen = 0;
    UCHAR                       MicAP[16];
    UCHAR                       PTK[80];
    UCHAR                       CalcBTK[80];
    UCHAR                       RatesLen;
    PNDIS_802_11_KEY            pPeerKey;
    UCHAR                       GTK[32];
    UCHAR                       DescriptGroupKey[20];
    UCHAR                       UnicastKeyID;
    UCHAR                       MulticastKeyID;
    UCHAR                       MulticastKeyLen;
    INT                         i;

    DBGPRINT(RT_DEBUG_TRACE, ("==> CCKMAssocRspSanity\n"));
    RatesLen = pFrame->Octet[7];
    pEid = (PEID_STRUCT) &pFrame->Octet[8 + RatesLen];

    // get variable fields from payload and advance the pointer
    while (((UCHAR*)pEid + pEid->Len + 1) < ((UCHAR*)pFrame + MsgLen))
    {
        if (pEid->Eid == IE_AIRONET_CCKMREASSOC)
        {
            Ptr = pEid->Octet;

            UnicastKeyID = *(Ptr + 8);
            MulticastKeyID = *(Ptr + 9);
            MulticastKeyLen = *(Ptr + 18);
            //
            // 1. Check the reassociation request number
            //
            if (!NdisEqualMemory((PUCHAR) &pAd->StaCfg.CCKMRN, Ptr + 4, 4))
                return FALSE;
            //
            // 2. Update PTK
            //
            NdisMoveMemory(CalcBTK, (PUCHAR) &pAd->StaCfg.CCKMRN, sizeof(pAd->StaCfg.CCKMRN));
            NdisMoveMemory(CalcBTK + sizeof(pAd->StaCfg.CCKMRN), pAd->CommonCfg.Bssid, MAC_ADDR_LEN);

            CCKMPRF(pAd->StaCfg.BTK, 32, CalcBTK, 10, PTK, LEN_PTK);
            NdisMoveMemory(pAd->StaCfg.PTK, PTK, LEN_PTK);

            //
            // 3. Check MIC_AP
            //
            COPY_MAC_ADDR(CalcMicBuffer, pAd->CurrentAddress);
            CalcMicBufferLen += MAC_ADDR_LEN;
            NdisMoveMemory(CalcMicBuffer + CalcMicBufferLen, CipherSuiteCiscoCCKM, CipherSuiteCiscoCCKMLen);
            CalcMicBufferLen += CipherSuiteCiscoCCKMLen;
            NdisMoveMemory(CalcMicBuffer + CalcMicBufferLen, Ptr + 4, 16);
            CalcMicBufferLen += 16;
            NdisMoveMemory(CalcMicBuffer + CalcMicBufferLen, Ptr + 28, MulticastKeyLen);
            CalcMicBufferLen += MulticastKeyLen;
            hmac_md5(pAd->StaCfg.PTK, LEN_EAP_MICK, CalcMicBuffer, CalcMicBufferLen, MicAP);

            if (!NdisEqualMemory(MicAP, Ptr+20, 8))
            {
                // cisco new firmware has different CipherSuiteCiscoCCKM
                DBGPRINT(RT_DEBUG_TRACE, ("==> 1st time CCKM MIC check fail\n"));
                CalcMicBufferLen = 0;
                // 2nd time check MIC, using different CipherSuite
                NdisMoveMemory(CalcMicBuffer, pAd->CurrentAddress, 6);
                CalcMicBufferLen += 6;
                NdisMoveMemory(CalcMicBuffer + CalcMicBufferLen, CipherSuiteCiscoCCKM24, CipherSuiteCiscoCCKM24Len);
                CalcMicBufferLen += CipherSuiteCiscoCCKM24Len;
                NdisMoveMemory(CalcMicBuffer + CalcMicBufferLen, Ptr + 4, 16);
                CalcMicBufferLen += 16;
                NdisMoveMemory(CalcMicBuffer + CalcMicBufferLen, Ptr + 28, MulticastKeyLen);
                CalcMicBufferLen += MulticastKeyLen;
                hmac_md5(pAd->StaCfg.PTK, LEN_EAP_MICK, CalcMicBuffer, CalcMicBufferLen, MicAP);
                
                if (!RTMPEqualMemory(MicAP, Ptr+20, 8))      
                {
                    DBGPRINT(RT_DEBUG_TRACE, ("==> 2nd time CCKM MIC check fail\n"));
                    return FALSE;
                }
                    DBGPRINT(RT_DEBUG_TRACE, ("==> 2nd time CCKM MIC check success\n"));
            }

            //
            // 4. Update 802.11 Encrypt Key
            //

            Status = MlmeAllocateMemory(pAd, &pPeerKey);  //Get an unused nonpaged memory
            if (Status != NDIS_STATUS_SUCCESS) 
                return FALSE;

            NdisZeroMemory(pPeerKey, sizeof(NDIS_802_11_KEY) + LEN_EAP_KEY);
            pPeerKey->Length    = sizeof(NDIS_802_11_KEY) + LEN_EAP_KEY;
            pPeerKey->KeyIndex  = 0xe0000000 | UnicastKeyID;
            pPeerKey->KeyLength = 16;
            COPY_MAC_ADDR(pPeerKey->BSSID, pAd->CommonCfg.Bssid);
            NdisMoveMemory(&pPeerKey->KeyRSC, Ptr + 10, LEN_KEY_DESC_RSC);
            NdisMoveMemory(pPeerKey->KeyMaterial, &pAd->StaCfg.PTK[32], LEN_EAP_KEY);
            // Before Call Add peer key function
			// we need to Remove all WPA keys
			RTMPWPARemoveAllKeys(pAd);

            RTMPWPAAddKeyProc(pAd, pPeerKey);
            MlmeFreeMemory(pAd, pPeerKey);

            //
            // 5. Get GTK = RC4(RN | PTK-802.1X-Encrypt-Key, EGTK)
            //
            NdisMoveMemory(DescriptGroupKey, (PUCHAR)&pAd->StaCfg.CCKMRN, 4);
            NdisMoveMemory(DescriptGroupKey + 4, &pAd->StaCfg.PTK[16], 16);
            ARCFOUR_INIT(&pAd->PrivateInfo.WEPCONTEXT, DescriptGroupKey, 20);
            //discard first 256 bytes specified in TGi
            for (i = 0; i < 256; i++)
                ARCFOUR_BYTE(&pAd->PrivateInfo.WEPCONTEXT);
            // Decrypt GTK. Becareful, there is no ICV to check the result is correct or not
            ARCFOUR_DECRYPT(&pAd->PrivateInfo.WEPCONTEXT, GTK, Ptr + 28, MulticastKeyLen);	

            //
            // 6. Update Group Key
            //
            pAd->SharedKey[BSS0][MulticastKeyID].KeyLen = MulticastKeyLen;
            pAd->SharedKey[BSS0][MulticastKeyID].Type = GROUP_KEY;
            NdisMoveMemory(pAd->SharedKey[BSS0][MulticastKeyID].Key, GTK, MulticastKeyLen);
    		DBGPRINT(RT_DEBUG_TRACE, ("<== CCKMAssocRspSanity OK\n"));

            return TRUE;
        }
        else
            pEid = (PEID_STRUCT)((UCHAR*)pEid + 2 + pEid->Len);
    }

    DBGPRINT(RT_DEBUG_TRACE, ("<== CCKMAssocRspSanity Failed\n"));
    return FALSE;
}

/*
    ========================================================================

    Routine Description:
        Report Adjacent AP informaion for CCX 

    Arguments:
        pAd      Pointer to our adapter
        
    Return Value:
        None 
        
    Note:
        When Romaming to the new ap, a client will always send an 
        IAPP information packet to the new AP about the previous AP's 
        information.
    ========================================================================
*/
VOID CCXAdjacentAPReport(
    IN	PRTMP_ADAPTER	pAd)
{
    NDIS_STATUS            Status;
    CISCO_IAPP_CONTENT     CCXAdjacentAPInfo;
	ULONG                  Now32;
    HEADER_802_11          Header_802_11;
    ULONG                  FrameLen = 0;
    PUCHAR                 pOutBuffer = NULL;
    UCHAR                  ckip_ck[16];
    UCHAR                  ckip_pk[16];     /* permuted key */	
    PUCHAR                 Ptr;
    INT                    j;
    USHORT                 Seconds;
	ULONG				   tmp;

	if (pAd->StaCfg.CCXAdjacentAPReportFlag != TRUE)
		return;

    //
    // Check Ssid.
    //
    if (!SSID_EQUAL(pAd->CommonCfg.Ssid, pAd->CommonCfg.SsidLen, pAd->StaCfg.CCXAdjacentAPSsid, pAd->StaCfg.CCXAdjacentAPSsidLen))
        return;

    if (MAC_ADDR_EQUAL(pAd->CommonCfg.Bssid, pAd->StaCfg.CCXAdjacentAPBssid))
        return;

    if (RogueApTableSearch(&pAd->StaCfg.RogueApTab, pAd->StaCfg.CCXAdjacentAPBssid) != BSS_NOT_FOUND)
        return;

    NdisZeroMemory(&CCXAdjacentAPInfo, sizeof(CISCO_IAPP_CONTENT));
    // Change IAPP length endian format to Big endian
    CCXAdjacentAPInfo.Length = cpu2be16(sizeof(CISCO_IAPP_CONTENT));
	CCXAdjacentAPInfo.MessageType = 0x30;
	CCXAdjacentAPInfo.FunctionCode = 0x00;
	COPY_MAC_ADDR(CCXAdjacentAPInfo.DestinaionMAC, pAd->CommonCfg.Bssid);
	COPY_MAC_ADDR(CCXAdjacentAPInfo.SourceMAC, pAd->CurrentAddress);
    CCXAdjacentAPInfo.Tag = cpu2be16(0x9B);
	NdisMoveMemory(CCXAdjacentAPInfo.OUI, CISCO_OUI, 3);
    CCXAdjacentAPInfo.TagLength = cpu2be16(48);
	COPY_MAC_ADDR(CCXAdjacentAPInfo.PreviousAP, pAd->StaCfg.CCXAdjacentAPBssid);
    // Change Channel number endian format to Big endian
    CCXAdjacentAPInfo.Channel = cpu2be16(pAd->StaCfg.CCXAdjacentAPChannel);
    // Change SSID Length endian format to Big endian
    CCXAdjacentAPInfo.SsidLen = cpu2be16(pAd->StaCfg.CCXAdjacentAPSsidLen);
	NdisMoveMemory(CCXAdjacentAPInfo.Ssid, pAd->StaCfg.CCXAdjacentAPSsid, pAd->StaCfg.CCXAdjacentAPSsidLen);
	NdisGetSystemUpTime(&Now32);
    Seconds =  (USHORT) (Now32 - pAd->StaCfg.CCXAdjacentAPLinkDownTime) / 1000;  //Get Seconds
    // Change lost Seconds time endian format to Big endian
    CCXAdjacentAPInfo.Seconds = cpu2be16(Seconds);

    Status = MlmeAllocateMemory(pAd, &pOutBuffer);  //Get an unused nonpaged memory
    if (Status != NDIS_STATUS_SUCCESS)
        return;

    if (pAd->CommonCfg.WepStatus == Ndis802_11EncryptionDisabled)		
    	LeapMacHeaderInit(pAd, &Header_802_11, 0, pAd->CommonCfg.Bssid);
	else
    	LeapMacHeaderInit(pAd, &Header_802_11, 1, pAd->CommonCfg.Bssid);
	
    MakeOutgoingFrame(pOutBuffer,            &FrameLen, 
                      sizeof(HEADER_802_11), &Header_802_11,
                      END_OF_ARGS);
 
    if (pAd->CommonCfg.WepStatus == Ndis802_11Encryption1Enabled)
    {
    
	    if (pAd->StaCfg.CkipFlag & 0x10)
	    { // Ckip is ON
	        if (pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen == 0)
	        {
	            MlmeFreeMemory(pAd, pOutBuffer);                
	            DBGPRINT(RT_DEBUG_ERROR, ("CCXAdjacentAPReport#1918: Can't process Ckip key, keylen can't be zero\n"));
	            return;
	        }

	        for(j=0; j< (16 / pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen); j++)
	            NdisMoveMemory(ckip_ck+j*pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen, 
	                            pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].Key, 
	                            pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen);
	        NdisMoveMemory(ckip_ck+j *pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen, 
	                        pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].Key, 
	                        16-(j * pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen));

	        j = 2; 
	        while (++pAd->StaCfg.GIV[j] == 0x0)
	        {
	            j--;
	            if (j < 0)
	                break;
	        }

	        Ptr = (PUCHAR)&Header_802_11;
	        CKIP_key_permute(ckip_pk, ckip_ck, *(Ptr+1) & 3, pAd->StaCfg.GIV);

	        Ptr = pOutBuffer;
	        Ptr += sizeof(HEADER_802_11);

	        RTMPInitWepEngine(pAd,
	                          ckip_pk,
	                          pAd->CommonCfg.DefaultKeyId,
	                          16,
	                          Ptr);
	 
	    }
		else
		{
			Ptr = pOutBuffer;
			Ptr += sizeof(HEADER_802_11);

			RTMPInitWepEngine(pAd, 
    			pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].Key,
	    		pAd->CommonCfg.DefaultKeyId,
		    	pAd->SharedKey[BSS0][pAd->CommonCfg.DefaultKeyId].KeyLen, 
			    Ptr);	
		}

		Ptr += 4;
		FrameLen += 4; //IV
		RTMPEncryptData(pAd, SNAP_LEAP_ROGUEAP_HDR, Ptr, sizeof(SNAP_LEAP_ROGUEAP_HDR));
		Ptr += sizeof(SNAP_LEAP_ROGUEAP_HDR);
		FrameLen += sizeof(SNAP_LEAP_ROGUEAP_HDR);

		RTMPEncryptData(pAd, (PUCHAR) &CCXAdjacentAPInfo, Ptr, sizeof(CISCO_IAPP_CONTENT));

		Ptr += sizeof(CISCO_IAPP_CONTENT);
		FrameLen += sizeof(CISCO_IAPP_CONTENT);
		RTMPSetICV(pAd, Ptr);
		FrameLen += 4;  //ICV

    }
	else if (pAd->CommonCfg.WepStatus == Ndis802_11EncryptionDisabled)
	{ //None WEP
	
    	MakeOutgoingFrame(pOutBuffer + FrameLen,              &tmp, 
			              sizeof(SNAP_LEAP_ROGUEAP_HDR),      SNAP_LEAP_ROGUEAP_HDR,
                          sizeof(CISCO_IAPP_CONTENT),         &CCXAdjacentAPInfo,
                          END_OF_ARGS);

		FrameLen += tmp;

	}
    DBGPRINT(RT_DEBUG_TRACE, ("Report Adjacent Previous AP[%02X:%02X:%02X:%02X:%02X:%02X][Lost seconds: %d]\n",
        CCXAdjacentAPInfo.PreviousAP[0], CCXAdjacentAPInfo.PreviousAP[1],
        CCXAdjacentAPInfo.PreviousAP[2], CCXAdjacentAPInfo.PreviousAP[3], 
        CCXAdjacentAPInfo.PreviousAP[4], CCXAdjacentAPInfo.PreviousAP[5], CCXAdjacentAPInfo.Seconds));
    MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);  
	MlmeFreeMemory(pAd, pOutBuffer);
	
    pAd->StaCfg.CCXAdjacentAPReportFlag = FALSE;
}




