#ifndef CML_ETH_H
#define CML_ETH_H

//#define CML_WAN_P0		// used Port 0 as WAN if defin, used Port 4 as WAN if Not define
//-------------------------------------------------------------

#ifdef CML_WAN_P0
enum
{
	PHY_WAN_PORT = 0,
	PHY_LAN_PORT1,
	PHY_LAN_PORT2,
	PHY_LAN_PORT3,
	PHY_LAN_PORT4
	
};
#else
enum
{
	PHY_LAN_PORT1 = 0,
	PHY_LAN_PORT2,
	PHY_LAN_PORT3,
	PHY_LAN_PORT4,

	PHY_WAN_PORT
};
#endif

#ifndef __BIT
#define __BIT(_a)              (0x1 << (_a))
#endif

/*!
 * \brief The number of receive buffer descrcriptors 
*/
#ifdef	CAMELOT_ETH_RX_DESC_SZ
#define NUM_RX_DESC 	CAMELOT_ETH_RX_DESC_SZ
#else
#define NUM_RX_DESC		48	//32
#endif
/*!
 * \brief The number of transmit buffer descrcriptors 
*/
#ifdef	CAMELOT_ETH_TX_DESC_SZ
#define NUM_TX_DESC 	CAMELOT_ETH_TX_DESC_SZ
#else
#define NUM_TX_DESC		96	//32
#endif
/*!
 * \brief The number of hardware buffer descrcriptors 
*/
#define NUM_HW_DESC		4

#define ETH_ADDR_LEN		6
#define ETH_TYPE_LEN		2
#define ETH_HEAD_LEN		(2*ETH_ADDR_LEN + ETH_TYPE_LEN)
#define ETH_CRC_LEN			4
#define ETH_MIN_FRAMELEN	60
//#define	TX_LEN_PADDING		1

#define MAX_IF_NUM		5

typedef struct if_cml_eth_s {
#ifdef _ECOS
	struct eth_drv_sc *sc;
#else
	struct net_device *sc;
#endif
	unsigned short unit;
	unsigned short link_status;
	
	unsigned short flags;
	unsigned int to_vlan;
	unsigned char macaddr[ETH_ADDR_LEN];
	unsigned short phy_addr;
} if_cml_eth_t;


#define PRIVIFFG_ACTIVE			0x2
#define PRIVIFFG_MAC_SET		0x1
#define CML_ETH_NBUF_LEN		1536

/*  buf descriptor for tx/rx control  */
typedef struct bufdesc_s {
#ifdef __ECOS
	struct eth_drv_sc *sc;
#else
	struct net_device *sc;
#endif
	void *buf;
} bufdesc_t;


/*  RX descriptor  */
typedef struct rxdesc_s {
	unsigned int w0;
	unsigned int w1;
} rxdesc_t;


/*  TX descriptor  */
typedef struct txdesc_s {
	unsigned int w0;
	unsigned int w1;
} txdesc_t;

#define	MCR_LOOPBACK		(1<<0)
#define	MCR_RXMAC_EN		(1<<1)
#define	MCR_FLOWCTRL		(1<<2)
#define	MCR_LINKUP		(1<<3)
#define	MCR_RXDMA_EN		(1<<4)
#define	MCR_HNAT_EN		(1<<5)
#define	MCR_TOCPU		(1<<6)
#define	MCR_BYBASS_UPDATE	(1<<7)
#define	MCR_TXMAC_EN		(1<<8)
#define	MCR_TX_FLOWCTRL		(1<<9)
#define	MCR_BYBASS_LOOKUP	(1<<10)
#define	MCR_ROUTE_LOOPBACK	(1<<11)
#define	MCR_TXDMA_EN		(1<<12)
#define MCR_TXDMA_RETRY_EN	(1<<13)
#define MCR_NATBUCKET_SIZE	(1<<14)
#define MCR_PREFETCH_DIS	(1<<15)
#define MCR_REVERSEIDX_GEN	(1<<16)
#define MCR_HMU_EN		(1<<17)
#define MCR_CACHE_EN		(1<<18)
#define MCR_HASH_FUNC		(1<<19)
#define MCR_BWCTRL_DROP		(1<<20)
#define MCR_BWCTRL_TRIGGER	(1<<21)
#define MCR_HMU_TESTMODE	(1<<22)
#define MCR_HWDS_EXT_RAM	(1<<23)
#define	MCR_SMI_S		24

#define DS0_RX_OWNBIT			(1<<31)
#define DS0_TX_OWNBIT			(1<<30)
#define DS0_END_OF_RING			(1<<29)
#define DS0_FREE_OF_PACKET		(1<<28)
#define	DS0_DROP_OF_PACKET		(1<<27)
#define	DS0_DF		(1<<27)
#define	DS0_LEN_S	16
#define	DS0_M		(1<<12)

//packet info
#define	DS0_PT_S	13		//rx packet type
#define	DS0_PT_B	3		//rx packet type, 3bits
#define	DS0_IPF		(1<<12)		//ip fragment
#define DS0_ISBC	(1<<11)		//broadcast packet
#define DS0_ISMC	(1<<10)		//multicast packet
#define	DS0_INB		(1<<9)		//inbond direction
#define	DS0_TFIN	(1<<8)		//tcp FIN flag
#define	DS0_TSYC	(1<<7)		//tcp SYN flag
#define	DS0_TRST	(1<<6)		//tcp RST flag
#define	DS0_NATH	(1<<5)		//nat table lookup hit
#define	DS0_ALG_S	(1<<0)		//ALG 4bits
//if DES_D (drop is set)
#define	DS0_ERUNT	(1<<6)		//runt packet
#define	DS0_EOL		(1<<5)		//over length
#define	DS0_ETTL	(1<<4)		//ip's ttl <2
#define	DS0_ECRC	(1<<3)		//crc error
#define	DS0_EL4CS	(1<<2)		//tcp checksum error
#define	DS0_EIPCS	(1<<1)		//ip checksum error
#define	DS0_ETSYC	(1<<0)		//tcp sync over
/* DS1 */
#define	DS1_POE		(1<<31)
#define	DS1_PSI_S	28
#define	DS1_VL		(1<<27)
#define	DS1_VID_S	24
#define	DS1_BUF_M	((1<<24)-1)
/* for tx only */
#define	DS0_DF		(1<<27)		//tx raw packet
#define	DS0_TST_S	13		//tx status
#define	DS0_TST_B	3		//tx status, 3bits
#define	DS0_TST_M 	(((1 << DS0_TST_B)-1 )<< DS0_TST_S)	
#define	DS0_M		(1<<12)		//more fragment in current packet
#define	DS0_DMI_S	0		//dest mac index

#define		MPHY	0x00
#define		MCR	0x04
#define		MAC_RXCR	0x14

#define		MIE_HMU	(1<<0)
#define		MIE_TX	(1<<1)
#define		MIE_SQE	(1<<2)
#define		MIE_HQE	(1<<3)
#define		MIE_BE	(1<<4)
#define		MIE_RX	(1<<5)

#define		MIS_HMU	(1<<0)
#define		MIS_TX	(1<<1)
#define		MIS_SQE	(1<<2)
#define		MIS_HQE	(1<<3)
#define		MIS_BE	(1<<4)
#define		MIS_RX	(1<<5)

#define		MIE_ENABLE	(0x3f)
#define		MIE	0x84
#define		MIM	0x84
#define		MIC	0x88
#define		MIS	0x8C

#define		MSWDB	0x08
#define		MHWDB	0x0c
#define		MBUFB	0x10
#define		MRXCR	0x14
#define		MPOET	0x18
#define		MVL01	0x1c
#define		MVL23	0x20
#define		MVL45	0x24
#define		MVL67	0x28
#define		MTSR	0x2c
#define		MDROPS	0x30
#define		MPKTSZ	0x34
#define		MFP	0x38
#define		MSA00	0x3c
#define		MSA01	0x40
#define		MSA10	0x44
#define		MSA11	0x48
#define         MAUTO	0x4c
#define		MFC0	0x50
#define		MFC1	0x54
#define		MDBUG	0x58
#define		MSRST	0x5c
#define		MMEMB	0x60
#define		MTXDB	0x64

#define 	MPROT	0x80
#define 	ML3	0x90

#define		MTT	0xc0
#define		MCT	0xc4
#define		MTTH	0xc8
#define		MTW	0xcc
#define		MARPB	0xD0
#define		MLBUF	0xD4

#define		MHTOBB		0x180
#define		MHTIBB		0x184
#define		MTB		0x188
#define		MSIP		0x194
#define		MECF		0x198	//NAT entry counter full status
#define		MNATEA		0x19C
#define		MCRCIV		0x1a0	//CRC10 initial value

#define		MHIP		0x1d8
#define		MHPORT		0x1dc
#define		MHPROT		0x1e0
#define		MHKEY		0x1e4
#define		MTXCT		0x204
#define		NHM		0x208	//HMU hash mode b0: inbond idx using sip xor dip
#define		HWDESC		0x1C0

#define RX_SPORT(_status)	(((_status) << 5) >> 29)
#define RX_FRAMELEN(_status)	(((_status) << 5) >> (16+5))
#define RX_ERROR(_status)	(((_status) << 17) >> 17)
#define RX_PROTYPE(_status)	(((_status) << 16) >> (13+16))

/* line status */
#define	GEN_STATUS_RESTART	0x80	// restart phy setting
#define GEN_STATUS_AN           0x10    // doing the Auto Nego
#define GEN_STATUS_FORCE        0x08    // force the link status, no AN
#define GEN_STATUS_FDX          0x04    // 1 = full duplex, 0 = half
#define GEN_STATUS_100MBPS      0x02    // 1 = 100 Mbps, 0 = 10 Mbps
#define GEN_STATUS_LINK         0x01    // 1 = link up, 0 = link down

/*  Receive machine  */
typedef struct rxctrl_s {
	rxdesc_t *prxd;
	unsigned short num_rxd;
	unsigned short num_freerxd;
	unsigned short rx_head;
	unsigned short rx_tail;

	unsigned int rx_pktcnt;
	unsigned int rx_bytecnt;
	unsigned int rx_mcpkt;
	unsigned int rx_bcpkt;
	unsigned int rx_err;
	unsigned int rx_errcode[16];
} rxctrl_t;



/*  Transmit machine  */
typedef struct txctrl_s {
	txdesc_t *ptxd;
	unsigned short num_txd;
	unsigned short num_freetxd;
	unsigned short tx_head;
	unsigned short tx_tail;
	
	bufdesc_t *ptxbuf;
	unsigned int tx_pktcnt;
	unsigned int tx_ok;
	unsigned int tx_ubuf;
	unsigned int tx_sglist;
	unsigned int tx_drop;
	unsigned int tx_err;
	unsigned int tx_bytecnt;
} txctrl_t;

typedef struct cml_eth_s {
	unsigned int    vector;		// interrupt vector
#ifdef	__ECOS
	unsigned int    int_hdl;	// handle for int.handler
	cyg_interrupt   int_obj;	// interrupt object
	struct eth_drv_sc *sc_main;
#else
	struct net_device *sc_main;
#endif //_ECOS
	unsigned short flags;
	unsigned short if_cnt;
	unsigned short act_ifmask;
	unsigned int int_mask;
	unsigned char flowctrl_mode;
	unsigned char flowctrl_state;

	txctrl_t tx;
	rxctrl_t rx;
	rxctrl_t hw;

	rxctrl_t *act_rx;
	if_cml_eth_t *if_array[MAX_IF_NUM];
	
} cml_eth_t;


#define		ETH_REG32(ofs)	((volatile unsigned int*)MAC_BASE)[ofs>>2]
#define 	TX_MAX_BUFCNT	3	//6


#define		virtophy(va) 	(((unsigned int)va)&0xfffffff)
#define		phytovirt(pa)	(((unsigned int)pa)|0xa0000000)
#define		nonca_addr(va)	(((unsigned int)va)|0xa0000000)
#define		ca_addr(va)		(((unsigned int)va)|0x80000000)
#ifndef		virt_to_bus
#define 	virt_to_bus(a) 	(((unsigned int)a)&0xfffffff)
#endif


#define	BUF_IPOFF 	0x40
#define	BUF_IPOMSK 	(~(BUF_IPOFF-1))
#define	BUF_MSK		(~((1<<11)-1))
#define BUF_MSB_OFS	0x0	//to save MSB16 of buffer pointer
#define BUF_HW_OFS	0x4	//where hw use it as the first byte
#define	BUF_SW_OFS  0x0 //where sw use it as the software pointer

#define pkt_to_buf(pkt) \
	(((unsigned long)pkt)  & 0xffffc0)
 
#define pkt_to_vbuf(pkt) \
	phytovirt(((unsigned long)pkt) & 0xffffc0) 

#define pkt_to_ip(pkt) \
	(phytovirt(((unsigned long)pkt) & 0x00ffffc0)+ BUF_IPOFF+BUF_HW_OFS)

#endif	/*  CML_ETH_H  */
