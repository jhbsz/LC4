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

#ifndef _NVRAM_EZPACKET_G_H
#define _NVRAM_EZPACKET_G_H

#if ( WAN_NUM_G == 1 )
#define WAN0_PROTO_G "dhcp"
#define WAN_MAIN_RULE_G "WAN1^1^1500^1^10"
#elif ( WAN_NUM_G == 2 )
#define WAN0_PROTO_G "dhcp"
#define WAN1_PROTO_G "dhcp"
#define WAN_MAIN_RULE_G "WAN1^1^1500^1^10|WAN2^1^1500^1^10"
#elif ( WAN_NUM_G == 3 )
#define WAN0_PROTO_G "dhcp"
#define WAN1_PROTO_G "dhcp"
#define WAN2_PROTO_G "dhcp"
#define WAN_MAIN_RULE_G "WAN1^1^1500^1^10|WAN2^1^1500^1^10|WAN3^1^1500^1^10"
#elif ( WAN_NUM_G == 4 )
#define WAN0_PROTO_G "dhcp"
#define WAN1_PROTO_G "dhcp"
#define WAN2_PROTO_G "dhcp"
#define WAN3_PROTO_G "dhcp"
#define WAN_MAIN_RULE_G "WAN1^1^1500^1^10|WAN2^1^1500^1^10|WAN3^1^1500^1^10|WAN4^1^1500^1^10"
#else
#error "WAN_NUM_G is not valid!!"
#endif


#endif /*_NVRAM_EZPACKET_G_H */
