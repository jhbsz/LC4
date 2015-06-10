/*
 * EZ Packet product default NVRAM values
 *
 * Copyright 2006, 2007 EZ Packet Technologies
 * All Rights Reserved.
 */

/*
 * Router default NVRAM values
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: defaults.c,v 1.104 2005/05/25 12:31:20 honor Exp $
 */

#ifndef _NVRAM_EZPACKET_P_H
#define _NVRAM_EZPACKET_P_H

#if ( WAN_NUM_P == 1 )
#define WAN0_PROTO_P "wwan"
#define WAN_MAIN_RULE_P "WAN1^1^1500^1^20"
#else
#error "WAN_NUM_P is not valid!!"
#endif

#endif /*_NVRAM_EZPACKET_H */
