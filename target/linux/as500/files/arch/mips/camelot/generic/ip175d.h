/******************************************************************************
*
*   Name:           switch.h
*
*   Description:    SIOC data structure + switch Header
*
*   Copyright:      (c) 2005-2050    IC Plus Corp.
*                   All rights reserved.  By DeNa + Chance
*
*******************************************************************************/
#ifndef _IP175D_H_
#define _IP175D_H_

#ifndef SIOCDEVPRIV
#define SIOCDEVPRIV		0x89f0
#define SIOCVLAN		SIOCDEVPRIV+0x0
#define SIOCQOS			SIOCDEVPRIV+0x1
#define SIOCMIRROR		SIOCDEVPRIV+0x2
#define SIOCDATARATE	SIOCDEVPRIV+0x3

#define SIOCACLSET		SIOCDEVPRIV+0xd
#define SIOCVOIPSET		SIOCDEVPRIV+0xe
#define SIOCNATSET		SIOCDEVPRIV+0xf
#endif

#define CMD_WRITE		0x10
#define CMD_READ		0x00


#define MAX_PORT_NUM	6
#define MAX_VLAN_NUM	16
#define TBASE_VLAN_NUM	MAX_VLAN_NUM
#define PBASE_VLAN_NUM	6

struct _VLAN_entry
{
  unsigned char	valid;	//0->disable  !0->Enable
  unsigned int	VID;	//1~4093
  unsigned char	FID;	//0~15
  unsigned char	member;	// bit[5:0] -> P5~P0
  unsigned char	AddTag;	// bit[5:0] -> P5~P0
  unsigned char	RmvTag;	// bit[5:0] -> P5~P0
  unsigned int	PVID;	//1~4093
};


#define VLAN_CMD_WRITE	CMD_WRITE
#define VLAN_CMD_READ	CMD_READ
#define VLAN_CMD_TBASE	0x01
#define VLAN_CMD_PBASE	0x00
#define VLAN_CMD_OKMASK	0x3f
#define VLAN_CMD_OK	0x80
struct _VLAN_setting
{
  unsigned char	cmd;	//bit4->1:write 0:read , bit0->1:TagVLAN 0:PortBaseVLAN
  unsigned char	vmode;	//bit[5:0] in write mode:0->PortBaseVLAN !0->TagBase VLAN
			//in read mode :0->PBaseVLAN !0->TBaseVLAN (Register22.0[5:0])
  struct _VLAN_entry VLAN_entry[MAX_VLAN_NUM];
};


#define QoS_QUEUE_0				0x00
#define QoS_QUEUE_1				0x01
#define QoS_QUEUE_2				0x02
#define QoS_QUEUE_3				0x03
#define QoS_MAX_QUEUE			4
struct _protocol_entry
{
  unsigned char  enable;		// 0: disable  !0: enable
  unsigned short port;			// TCP/UDP port number
  unsigned char  queue;			// queue0~3
};

struct _range_entry
{
  unsigned char  enable;		// 0: disable  !0: enable
  unsigned short s_port;		// TCP/UDP port range start number
  unsigned short e_port;		// TCP/UDP port range end number
  unsigned char  queue;			// queue0~3
};

#define QoS_TYPE_DISABLE		0x00
#define QoS_TYPE_SOURCE			0x01
#define QoS_TYPE_DESTINATION	0x02
#define QoS_TYPE_SOURCEnDEST	0x03

#define QoS_MODE_WRR			0x00
#define QoS_MODE_WFQ			0x01
#define QoS_MODE_SPM			0x02
#define QoS_MODE_SP 			0x03
#define QoS_MODE_HIGH_PASS		QoS_MODE_SP

#define MAX_PROTOCOL_NUM		4
#define MAX_RANGE_NUM			2

#define QoS_CMD_WRITE	CMD_WRITE
#define QoS_CMD_READ	CMD_READ
struct _QoS_setting
{
  unsigned char	cmd;	//bit4->1:write 0:read 
  unsigned char enable;	// 0:disable !0:enable
  unsigned char type;	// 0:disable 1:SA 2:DA 3:DA+SA
  unsigned char mode;	// 0:WRR	3:High Priority first
  unsigned int  queue_weight[QoS_MAX_QUEUE];//weight for q0(low)~q3(high) (unit:packet)
  struct _protocol_entry protocol[MAX_PROTOCOL_NUM];
  struct _range_entry range[MAX_RANGE_NUM];
};

#define MAX_MIRROR_PORT		MAX_PORT_NUM

#define MIRROR_CMD_WRITE	CMD_WRITE
#define MIRROR_CMD_READ		CMD_READ

#define MIRROR_MODE_RX		0x0
#define MIRROR_MODE_TX		0x1
#define MIRROR_MODE_TX2RX	0x2
#define MIRROR_MODE_TXnRX	0x3
struct _Mirror_setting
{
  unsigned char	cmd;	// bit4->1:write 0:read 
  unsigned char enable;	// 0:disable !0:enable
  unsigned char mode;	// 0:rx 1:tx 2:tx_to_rx 3:tx nad rx
  unsigned char tx_from;// mirror from which port's Tx (p0~p5)
  unsigned char rx_from;// mirror from which port's Rx (p0~p5)
  unsigned char to;		// mirror to which port p0~p5
};

#define DATARATE_CMD_WRITE	CMD_WRITE
#define DATARATE_CMD_READ	CMD_READ
#define DATARATE_MAX_RATE	3124
struct _Datarate_setting
{
  unsigned char	cmd;	// bit4->1:write 0:read 
  unsigned short tx_rate[MAX_PORT_NUM];// per unit is 32Kbps (0~3124 0:full speed)
  unsigned short rx_rate[MAX_PORT_NUM];// per unit is 32Kbps (0~3124 0:full speed)
};
//////////////////////////under is switch ip175d register map/////////////

//PHY Control
#define		PHY0			0
#define		PHY_CTRL		0
#define		PHY_STATUS		1
#define		PHY_AN_ADV		4
#define		PHY_AN_LPA		5


//HU Table
#define 	PHY20 			20
#define		CONG_CTRL		5
#define		PORT_STATE		6
#define 	LEARNING_CTRL_REG 	13
#define		BF_STM_CTRL		16
#define		BF_STM_THR_0		17
#define		PORT_MIRROR_0		20
#define		PORT_MIRROR_1		21

//Rate Control
#define		PHY21			21//0x0B//21 original 0x11 for FPGA
#define 	BW_TI			8
#define 	BW_MBS			9
#define 	BW_CREDIT_SIZE		10
#define 	BW_TIME			11
#define 	BW_SETTING		12
#define		LEARN_CONSTRAIN		13
#define		MAC_COMMAND		14
#define		MAC_ADDR0		15
#define		MAC_ADDR1		16
#define		MAC_ADDR2		17
#define		MAC_CONTROL		18
#define		MAC_STATIC		19
#define		CRC_COUNTER		25
//VLAN Control
#define 	PHY22			22

#define     VLAN_CLASS                  0
#define     VLAN_INGRESS                1
#define     VLAN_EGRESS                 2
#define     VLAN_INFO_0                 4
#define     VLAN_INFO_1                 VLAN_INFO_0+1
#define     VLAN_INFO_2                 VLAN_INFO_0+2
#define     VLAN_INFO_3                 VLAN_INFO_0+3
#define     VLAN_INFO_4                 VLAN_INFO_0+4
#define     VLAN_INFO_5                 VLAN_INFO_0+5
#define     VLAN_VALID                  10
#define     VLAN_QU_NUM_EN              11
#define     VLAN_STP_IDX_EN             12
#define     VLAN_REW_VALN_PRI_EN        13

#define     VLAN_FID_VID0               14
#define     VLAN_FID_VID1               VLAN_FID_VID0+1
#define     VLAN_FID_VID2               VLAN_FID_VID0+2
#define     VLAN_FID_VID3               VLAN_FID_VID0+3
#define     VLAN_FID_VID4               VLAN_FID_VID0+4
#define     VLAN_FID_VID5               VLAN_FID_VID0+5
#define     VLAN_FID_VID6               VLAN_FID_VID0+6
#define     VLAN_FID_VID7               VLAN_FID_VID0+7
#define     VLAN_FID_VID8               VLAN_FID_VID0+8
#define     VLAN_FID_VID9               VLAN_FID_VID0+9
#define     VLAN_FID_VID10              VLAN_FID_VID0+10
#define     VLAN_FID_VID11              VLAN_FID_VID0+11
#define     VLAN_FID_VID12              VLAN_FID_VID0+12
#define     VLAN_FID_VID13              VLAN_FID_VID0+13
#define     VLAN_FID_VID14              VLAN_FID_VID0+14
#define     VLAN_FID_VID15              VLAN_FID_VID0+15

#define     PHY23                       23
#define     VLAN_MEMBER_00              00
#define     VLAN_MEMBER_02              VLAN_MEMBER_00+1
#define     VLAN_MEMBER_04              VLAN_MEMBER_00+2
#define     VLAN_MEMBER_06              VLAN_MEMBER_00+3
#define     VLAN_MEMBER_08              VLAN_MEMBER_00+4
#define     VLAN_MEMBER_10              VLAN_MEMBER_00+5
#define     VLAN_MEMBER_12              VLAN_MEMBER_00+6
#define     VLAN_MEMBER_14              VLAN_MEMBER_00+7
#define     VLAN_ADDTAG_00              8
#define     VLAN_ADDTAG_02              VLAN_ADDTAG_00+1
#define     VLAN_ADDTAG_04              VLAN_ADDTAG_00+2
#define     VLAN_ADDTAG_06              VLAN_ADDTAG_00+3
#define     VLAN_ADDTAG_08              VLAN_ADDTAG_00+4
#define     VLAN_ADDTAG_10              VLAN_ADDTAG_00+5
#define     VLAN_ADDTAG_12              VLAN_ADDTAG_00+6
#define     VLAN_ADDTAG_14              VLAN_ADDTAG_00+7
#define     VLAN_REMOVETAG_00           16
#define     VLAN_REMOVETAG_02           VLAN_REMOVETAG_00+1
#define     VLAN_REMOVETAG_04           VLAN_REMOVETAG_00+2
#define     VLAN_REMOVETAG_06           VLAN_REMOVETAG_00+3
#define     VLAN_REMOVETAG_08           VLAN_REMOVETAG_00+4
#define     VLAN_REMOVETAG_10           VLAN_REMOVETAG_00+5
#define     VLAN_REMOVETAG_12           VLAN_REMOVETAG_00+6
#define     VLAN_REMOVETAG_14           VLAN_REMOVETAG_00+7
#define     VLAN_MISC_00                24
#define     VLAN_MISC_02                VLAN_MISC_00+1
#define     VLAN_MISC_04                VLAN_MISC_00+2
#define     VLAN_MISC_06                VLAN_MISC_00+3
#define     VLAN_MISC_08                VLAN_MISC_00+4
#define     VLAN_MISC_10                VLAN_MISC_00+5
#define     VLAN_MISC_12                VLAN_MISC_00+6
#define     VLAN_MISC_14                VLAN_MISC_00+7

#define		PHY25			25
#define		QOS_CTRL		0
#define		QOS_PORT_PRI		1
#define		QOS_TAG_PRI		2
#define		QOS_TOS_PRI_0		3
#define		QOS_TOS_PRI_1		QOS_TOS_PRI_0+1
#define		QOS_TOS_PRI_2		QOS_TOS_PRI_0+2
#define		QOS_TOS_PRI_3		QOS_TOS_PRI_0+3
#define		QOS_TOS_PRI_4		QOS_TOS_PRI_0+4
#define		QOS_TOS_PRI_5		QOS_TOS_PRI_0+5
#define		QOS_TOS_PRI_6		QOS_TOS_PRI_0+6
#define		QOS_TOS_PRI_7		QOS_TOS_PRI_0+7
#define		QOS_TCP_CTRL		11
#define		QOS_PRE_LOGI_0		12
#define		QOS_PRE_LOGI_1		QOS_PRE_LOGI_0+1
#define		QOS_PRE_LOGI_2		QOS_PRE_LOGI_0+2
#define		QOS_PRE_LOGI_3		QOS_PRE_LOGI_0+3
#define		QOS_USER_RG_LOW_0	16
#define		QOS_USER_RG_HI_0	17
#define		QOS_USER_RG_LOW_1	QOS_USER_RG_LOW_0+2
#define		QOS_USER_RG_HI_1	QOS_USER_RG_HI_0+2
#define		QOS_TCP_QU_MAP		20
#define		QOS_SCH_CONFIG		22
#define		QOS_SCH_WEIGHT		23

#define     PHY26                       26
#define     MF_CTRL                     0
#define     MF_CLASS                    1  
#define     MF_IM_SA0                   2
#define     MF_IM_SA1                   MF_IM_SA0+1
#define     MF_IM_SA2                   MF_IM_SA0+2
#define     MF_IM_DA0                   5
#define     MF_IM_DA1                   MF_IM_DA0+1
#define     MF_IM_DA2                   MF_IM_DA0+2
#define     MF_ET_VALUE                 8
#define     MF_REG9                     9
#define     MF_TCP_UDP                  10
#define     MF_LG_SP_NUM                11
#define     MF_LG_DP_NUM                12
#define     MF_SP_PORTNUM               13
#define     MF_BEHAVIOR                 14
#define     MF_TI                       15
#define     MF_MBS                      16
#define     MF_CREDIT_SIZE              17
#define     MF_TSEL                     18
#define     MF_VALID                    19
#define     MF_ACCESS_CTRL              20
#define     MF_CNT_LSB                  21
#define     MF_CNT_MSB                  22
#define	    MF_OVERFLOW			23

#define     PHY27                       27//0x23
#define     QUEUE_MONITOR               9

#endif
