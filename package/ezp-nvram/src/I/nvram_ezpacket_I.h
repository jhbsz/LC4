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

#ifndef _NVRAM_EZPACKET_I_H
#define _NVRAM_EZPACKET_I_H
#if ( WAN_NUM_I == 1 )
#define WAN0_PROTO_I "wwan"
#define WAN_MAIN_RULE_I "WAN1^1^1500^1^20"
#elif ( WAN_NUM_I == 2 )
#define WAN0_PROTO_I "dhcp"
#define WAN1_PROTO_I "wwan"
#define WAN_MAIN_RULE_I "WAN1^1^1500^1^10|WAN2^1^1500^1^20"
#elif ( WAN_NUM_I == 3 )
#define WAN0_PROTO_I "dhcp"
#define WAN1_PROTO_I "dhcp"
#define WAN2_PROTO_I "wwan"
#define WAN_MAIN_RULE_I "WAN1^1^1500^1^10|WAN2^1^1500^1^10|WAN3^1^1500^1^20"
#elif ( WAN_NUM_I == 4 )
#define WAN0_PROTO_I "dhcp"
#define WAN1_PROTO_I "dhcp"
#define WAN2_PROTO_I "dhcp"
#define WAN3_PROTO_I "wwan"
#define WAN_MAIN_RULE_I "WAN1^1^1500^1^10|WAN2^1^1500^1^10|WAN3^1^1500^1^10|WAN4^1^1500^1^20"
#elif ( WAN_NUM_I == 5 )
#define WAN0_PROTO_I "dhcp"
#define WAN1_PROTO_I "dhcp"
#define WAN2_PROTO_I "dhcp"
#define WAN3_PROTO_I "dhcp"
#define WAN4_PROTO_I "wwan"
#define WAN_MAIN_RULE_I "WAN1^1^1500^1^10|WAN2^1^1500^1^10|WAN3^1^1500^1^10|WAN4^1^1500^1^10|WAN5^1^1500^1^20"
#else
#error "WAN_NUM_I is not valid!!"
#endif

#endif /*_NVRAM_EZPACKET_H */
