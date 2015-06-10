/*=============================================================================+
|                                                                              |
| Copyright 2008                                                               |
| Acrospeed Inc. All right reserved.                                           |
|                                                                              |
+=============================================================================*/
/*! 
*   \file cml_eth.c
*   \brief  Camelot ethernet driver.
*   \author Acrospeed
*/

/*=============================================================================+
| Included Files							       |
+=============================================================================*/
#ifdef	__ECOS
#include <io_eth_drivers.h>
#include <sys/types.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/infra/diag.h>
#include <cyg/hal/drv_api.h>
#include <netdev.h>
#include <eth_drv.h>
#include <cml_eth.h>
#include <cml_hnat.h>

#ifdef CYGPKG_NET
#include <net/if.h>
#endif
//#include <cdb_api.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/mbuf.h>

#else

#include <linux/autoconf.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/mii.h>
#include <asm/delay.h>
#include <asm/camelot/camelot.h>
#include <asm/camelot/cml_eth.h>
#include <asm/camelot/cml_hnat.h>

#define	hal_delay_us(n) udelay(n)
#define diag_printf		printk
#define cyg_drv_interrupt_unmask(n) enable_irq(n)
#define cyg_drv_interrupt_mask(n) disable_irq(n)
#define HAL_DCACHE_FLUSH(a,l)	pci_map_single(0,a,l,PCI_DMA_FROMDEVICE)
#define HAL_DCACHE_STORE(a,l)	pci_map_single(0,a,l,PCI_DMA_TODEVICE)
#ifndef SA_INTERRUPT
#define	SA_INTERRUPT IRQF_DISABLED
#endif
#ifndef	IRQ_HANDLED
#define IRQ_HANDLED
#define irqreturn_t void
#endif 
#endif

/*=============================================================================+
| Define                                                                       |
+=============================================================================*/
/// @cond DOXYGEN
#define ETH_DBG
#define ETH_CAMELOT_ETH0_NAME 			"eth0"
#define ETH_CAMELOT_ETH1_NAME 			"eth1"
/// @endcond

/// @cond DOXYGEN
#define MCR_INIT	((0xd<<MCR_SMI_S) | MCR_BWCTRL_TRIGGER|MCR_BWCTRL_DROP| \
			MCR_BYBASS_LOOKUP|	/* bypass lookup */	\
			MCR_TXDMA_RETRY_EN|MCR_TX_FLOWCTRL|MCR_LINKUP|MCR_FLOWCTRL)
			//MCR_TXDMA_RETRY_EN|MCR_TX_FLOWCTRL|MCR_LINKUP)

#define MDROPS_INIT	0x1e0100fb	
#define	MIM_INIT	0x33
#define MRXCR_INIT	0x8201004 //defulat=0x8321004
#define ML3_INIT	0x071505dc
/// @endcond

/// @cond DOXYGEN

#ifdef	CONFIG_NET_CLUSTER_NONCACHED
#define	DC_FLUSH_TX(a,l)	\
{	\
	if ((((unsigned long)a) & 0xa0000000) != 0xa0000000)	\
	{	\
		HAL_DCACHE_STORE(a,l);	\
	}	\
}

#define	DC_FLUSH_RX(a,l)
#define	PHYTOVA(a)				nonca_addr(a)
#define	DESC_W1_TO_VBUF(w1) 	nonca_addr((w1)&0xffffc0)
#define	DESC_W1_TO_VPKT(w1) 	nonca_addr((w1<<8)>>8)
#else

#define	DC_FLUSH_TX(a,l)		HAL_DCACHE_STORE(a,l)
#define	DC_FLUSH_RX(a,l)		HAL_DCACHE_FLUSH(a,l)
#define	PHYTOVA(a)				ca_addr(a)
#define	DESC_W1_TO_VBUF(w1) 	ca_addr((w1)&0xffffc0)
#define	DESC_W1_TO_VPKT(w1) 	ca_addr((w1<<8)>>8)
#endif

#ifndef	HAL_READ_ETHMAC
#define HAL_READ_ETHMAC cdb_get_mac
#endif


/// @endcond

/// @cond DOXYGEN
#define	NUM_MBUF	 	(NUM_RX_DESC+24+2)

#define DESC_SZ			(NUM_RX_DESC*sizeof(rxdesc_t) \
				+ NUM_TX_DESC*sizeof(txdesc_t) + NUM_HW_DESC*sizeof(rxdesc_t) )

#define CML_ETH_IRQ		IRQ_MAC

#ifdef ETH_DBG
#define DBG_ERR			__BIT(0)
#define DBG_INFO		__BIT(1)
#define DBG_INIT		__BIT(2)
#define DBG_INITMORE		__BIT(3)
#define DBG_TX			__BIT(4)
#define DBG_TXMORE		__BIT(5)
#define DBG_RX			__BIT(6)
#define DBG_RXMORE		__BIT(7)
#define DBG_IOCTL		__BIT(8)
#define DBG_ISR			__BIT(9)
#define DBG_DSR			__BIT(10)
#define DBG_DELIVER		__BIT(11)
#define	DBG_PHY			__BIT(12)


//#define DBG_INFO_MASK		(DBG_ERR | DBG_INFO | DBG_INIT | DBG_IOCTL|DBG_PHY|DBG_RX|DBG_RXMORE)
#define DBG_INFO_MASK		(DBG_ERR)

#define DBGPRINTF(_type, fmt, ...)				\
	do {							\
		if ((_type) & DBG_INFO_MASK)			\
			diag_printf(fmt, ## __VA_ARGS__);	\
	} while(0)

#else

#define DBGPRINTF(_type, fmt, ...)				\
	do { } while(0)
#endif

#ifndef	CYG_ASSERT
#define CYG_ASSERTC(test) do {} while(0)
#define CYG_ASSERT(test,msg) do {} while(0)
#endif

#define MAC_SMI_CR MPHY

#define RX_CACHE_FLUSH_SIZE		1520
/// @endcond

/*=============================================================================+
| Variables						                       |
+=============================================================================*/
/*  Global variables							*/
#ifdef	__ECOS
static if_cml_eth_t cml_eth0_priv_data = { 
    unit: 0,
};

ETH_DRV_SC(cml_eth_sc0,
           &cml_eth0_priv_data,      // Driver specific data
           ETH_CAMELOT_ETH0_NAME, // Name for device
           cml_eth_start,
           cml_eth_stop,
           cml_eth_ioctl,
           cml_eth_cansend,
           cml_eth_send,
           cml_eth_recv,
           cml_eth_deliver,
           cml_eth_poll,
           cml_eth_intvector
    );

NETDEVTAB_ENTRY(cml_eth_netdev0, 
                "cml_" ETH_CAMELOT_ETH0_NAME,
                cml_eth_drv_init, 
                &cml_eth_sc0);

static if_cml_eth_t cml_eth1_priv_data = { 
    unit: 1,
};

ETH_DRV_SC(cml_eth_sc1,
           &cml_eth1_priv_data,      // Driver specific data
           ETH_CAMELOT_ETH1_NAME, // Name for device
           cml_eth_start,
           cml_eth_stop,
           cml_eth_ioctl,
           cml_eth_cansend,
           cml_eth_send,
           cml_eth_recv,
           cml_eth_deliver,
           cml_eth_poll,
           cml_eth_intvector
    );

NETDEVTAB_ENTRY(cml_eth_netdev1, 
                "cml_" ETH_CAMELOT_ETH1_NAME,
                cml_eth_drv_init, 
                &cml_eth_sc1);
#endif //__ECOS

cml_eth_t cml_eth;
cml_eth_t *pcml_eth = 0;

static bufdesc_t g_txbd[NUM_TX_DESC] __attribute__ ((aligned (0x8)));
// descriptor must be aligned to 0x20 to optimize performance
static unsigned char g_desc[DESC_SZ] __attribute__ ((aligned (0x20)));
/// @endcond
/*=============================================================================+
| Function Prototypes						       	       |
+=============================================================================*/
/// @cond DOXYGEN
/*  Interface function for uppler layer  				*/
//#define IN_SEC_IRAM __attribute__ ((section(".iram"))) 
#define IN_SEC_IRAM 

#ifdef __ECOS
static void cml_eth_start(struct eth_drv_sc *sc, unsigned char *enaddr, 
		int flags);
static void cml_eth_stop(struct eth_drv_sc *sc);
static int cml_eth_ioctl(struct eth_drv_sc *sc, unsigned long key, 
		void *data, int data_len);
static int cml_eth_cansend(struct eth_drv_sc *sc);
static void cml_eth_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
		int sg_len, int total_len, unsigned long key);
static void cml_eth_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
		int sg_len);
		
static void cml_eth_deliver(struct eth_drv_sc *sc);
static void cml_eth_poll(struct eth_drv_sc *sc);
static int cml_eth_intvector(struct eth_drv_sc *sc);

/*  Interface initialize function					*/
static bool cml_eth_drv_init(struct cyg_netdevtab_entry *pnde);

static void cml_eth_rxint(rxctrl_t *prxc) IN_SEC_IRAM;
void cml_eth_txint(txctrl_t *ptxc, int wait_free) IN_SEC_IRAM;
static void cml_eth_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
		int sg_len, int total_len, unsigned long key) IN_SEC_IRAM;
static unsigned int cml_eth_isr(cyg_vector_t vec, cyg_addrword_t data) IN_SEC_IRAM;
static void cml_eth_dsr(cyg_vector_t vec, cyg_ucount32 count, cyg_addrword_t data) IN_SEC_IRAM;
#else

#define	USED_DSR
#define ETH_DEV_INIT 2
if_cml_eth_t * if_cml_eth_g[MAX_IF_NUM];
const static char *cml_eth_ethdrv_version = "0.1.2" ;
struct tasklet_struct   tasklet;    /* tasklet for dsr */

static void cml_eth_rxint(rxctrl_t *prxc) IN_SEC_IRAM;
void cml_eth_txint(txctrl_t *ptxc, int wait_free) IN_SEC_IRAM;
static int cml_eth_start(struct net_device *dev);
static int cml_eth_stop( struct net_device *dev );
static int cml_eth_send(struct sk_buff *skb, struct net_device *dev ) IN_SEC_IRAM;
static void cml_eth_deliver(struct net_device *sc) IN_SEC_IRAM;
static irqreturn_t cml_eth_isr( int int_num, void *dev_id, struct pt_regs *regs ) IN_SEC_IRAM;
static int cml_eth_ioctl( struct net_device *dev, struct ifreq *ifr, int cmd );
int eth_drv_recv1(struct net_device *dev, unsigned char * data,  int total_len, char **newbuf, void *hw_nat) IN_SEC_IRAM;
char * eth_drv_getbuf(void);
void eth_drv_freebuf(char *buf);


int cml_hnat_sdmz_rx_check(if_cml_eth_t *pifcml_eth, char *buf, char *pkt, unsigned int w0, unsigned int w1, int len);
void cml_hnat_sdmz_tx_check(char *hwadrp);
#endif //__ECOS

void cml_eth_flowctrl(int onoff);
void cml_eth_macatable_wr(unsigned char *mac, char fid, char num, unsigned short info, unsigned short flag);

/// @endcond
/*=============================================================================+
| Extern Function/Variables						       |
+=============================================================================*/
#ifdef	__ECOS
extern int eth_drv_recv1(struct eth_drv_sc *sc, char * data,  int total_len, char **clbuf, void *hw_nat);
#endif

/*=============================================================================+
| Functions  						   	 	       |
+=============================================================================*/

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_mdio:
 *      \brief read/write phy register.
 *	\param phy: 
 *	\param reg: 
 *=============================================================*/
void cml_eth_mdio(short phy,short reg,unsigned short *pval,int wr)
{
    unsigned long v;
    
    v=(2<<30)|(phy<<24)|(reg<<16);
    if (wr)
	v |= (1<<30)|*pval;
    ETH_REG32(MAC_SMI_CR) = v;
    hal_delay_us(1); /* delay for h/w, DON'T remove*/
    for (;;)
    {
	if ((1<<31)&(v=ETH_REG32(MAC_SMI_CR)) )
	{
		break;
	}
    }
    if (!wr)
    	*pval=(v&0xffff);
}
/// @endcond

/// @cond DOXYGEN
/*!-----------------------------------------------------------------------------
 * function: cml_eth_mdio_rd
 *
 *      \brief read phy register.
 *	\param phy: 
 *	\param reg: 
 *      \return 0 is ready, -1 is failed.
 +----------------------------------------------------------------------------*/
unsigned short cml_eth_mdio_rd(short phy,short reg)
{
	unsigned short rd;
	cml_eth_mdio(phy,reg,&rd,0);
	return rd;
}
/// @endcond
/*=============================================================*
 *  cml_eth_mdio_wr:
 *      \brief write phy register.
 *	\param phy: 
 *	\param reg: 
 *=============================================================*/
void cml_eth_mdio_wr(short phy,short reg,unsigned short val)
{
	cml_eth_mdio(phy,reg,&val,1);
	if (DBG_INFO_MASK & DBG_PHY)
	{
		unsigned short rd;
		rd=cml_eth_mdio_rd(phy,reg);

        	DBGPRINTF(DBG_PHY, "phy%02x,reg%02x,wr:%04x\n", phy,(int)reg,(unsigned int)val);
        	if (rd!=val)
	    		diag_printf("cml_eth_mdio_wr(%02x,%02x,%04x)==> not match, rd:%x\n\n", phy, (int)reg, (unsigned int) val, (unsigned int)rd);
	}
}
/// @endcond

/*=============================================================*
 *  cml_eth_preset_175d
 *=============================================================*/
/*!
 * \brief disable cpu port for safe
 * \param void
 * \return None
 */
void cml_eth_preset_175d(void)
{
  	ETH_REG32(MCR) = (MCR_INIT&0xff000000);
	cml_eth_mdio_wr(20, 6 , 0x0f3f);
}

struct phycfg
{
	char phyr;
	char reg; 
	unsigned short val;
};

//#define	CONFIG_IP175D_P01
#ifdef	CONFIG_IP175D_P01	// P0: LAN, P1: WAN
static struct phycfg ip175d_cfg[]=
{
	/* indepent vlan learning */
	{20,13,0x9},  /* splite address table into 1K table for unicast frame and 1K table for multicast frame */
	/* tag-based vlan */
	{22,0,0x7ff},
	/* vlan enable */
	{22,10,0x3},	/* valid entry 0-1 */
	/* port0,PVID=1 */
	{22,14,0x1001},
	{22,4,1},
	/* port1,PVID=2 */
	{22,15,0x2002},
	{22,5,2},
	/* vlan1 member: 100010, 00- vlan0 member: 100001 */
	{23,0,0x2221},
	/* port5, add tag vid1,vid0 */
	{23,8,0x2020},
	/* port0~4, remove tag for vid0,vid1 */
	{23,0x10,0x1f1f},
	/* enable wan port forwarding*/
	{20, 6 , 0x3f3f},
	{-1, -1, -1},
};
#else
static struct phycfg ip175d_cfg[]=
{
	/* indepent vlan learning */
	{20,13,0x9},  /* splite address table into 1K table for unicast frame and 1K table for multicast frame */
	/* tag-based vlan */
	{22,0,0x7ff},
	/* vlan enable */
	{22,10,0x1f},
#ifdef CML_ETH_WAN_P0
	/* port0,PVID=2 */
	{22,14,0x2002},
	{22,4,2},
	/* port1,PVID=1 */
	{22,15,0x1001},
	{22,5,1},
	/* port2,PVID=1 */
	{22,16,0x1001},
	{22,6,1},
	/* port3,PVID=1 */
	{22,17,0x1001},
	{22,7,1},
	/* port4,PVID=1 */
	{22,18,0x1001},
	{22,8,1},
	/* vlan1 member: 111110, vlan0 member: 100001 */
	{23,0,0x3e21},
	/* vlan3 member: 111110, vlan2 member: 111110 */
	{23,1,0x3e3e},
	/* vlan4 member: 111110 */
	{23,2,0x003e},
#else
	/* port0,PVID=1 */
	{22,14,0x1001},
	{22,4,1},
	/* port1,PVID=1 */
	{22,15,0x1001},
	{22,5,1},
	/* port2,PVID=1 */
	{22,16,0x1001},
	{22,6,1},
	/* port3,PVID=1 */
	{22,17,0x1001},
	{22,7,1},
	/* port4,PVID=2 */
	{22,18,0x2002},
	{22,8,2},
	/* vlan1 member: 101111, vlan0 member: 101111 */
	{23,0,0x0b2f},
	/* vlan3 member: 101111, vlan2 member: 101111 */
	{23,1,0x0b2f},
	/* vlan4 member: 110000 */
	{23,2,0x0030},
#endif	
	/* port5, add tag vid1,vid0 */
	{23,8,0x2020},
	/* port5, add tag vid3,vid2 */
	{23,9,0x2020},
	/* port5, add tag vid4 */
	{23,0x0a,0x0020},
	/* port0~4, remove tag for vid0,vid1 */
	{23,0x10,0x1f1f},
	/* port0~4, remove tag for vid2,vid3 */
	{23,0x11,0x1f1f},
	/* port0~4, remove tag for vid4,vid5 */
	{23,0x12,0x1f1f},
	/* enable wan port forwarding*/
	{20, 6 , 0x3f3f},
	{-1, -1, -1},
};
#endif
static struct phycfg ip175c_cfg[]=
{
	{29,23,0x07c2},
	{29,24,1},	//port0, PVID=1
	{29,25,1},	//port1, PVID=1
	{29,26,1},	//port2, PVID=1
	{29,27,1},	//port3, PVID=1
	{29,28,2},	//port4, PVID=2
	{29,30,2},	//mii0,  PVID=2
	{30, 1,0x2f30},	//VLAN1=mii,port0~port3 VLAN0=mii0,port4
	{30, 2,0x3f30}, //VLAN3=mii,port0~port4 VLAN2=mii0,port4
	{30, 9,0x1089},	//WAN:port4, VLAN:enable, VID:0, Router:enable, LAN: VLAN1
	{29,18,0xfc08},	//flow-control
	{-1, -1, -1},
};
/*=============================================================*
 *  cml_eth_switch_init:
 *=============================================================*/
/*!
 * \brief Initialize the switch and configure the VLAN.
 *
 * \param void
 *
 * \return None
 */
void cml_eth_switch_init(void)
{
	struct phycfg *regset;
	short val;
	// 175D
	if (0x175d == cml_eth_mdio_rd(20,0))
	{
		DBGPRINTF(DBG_INIT, "Init ip175d switch\n");
		regset = & ip175d_cfg[0];
	}
	else
	{
		DBGPRINTF(DBG_INIT, "Init ip175c switch\n");
		regset = & ip175c_cfg[0];
	}
	while (regset->phyr!=-1)
	{
		cml_eth_mdio_wr(regset->phyr, regset->reg, regset->val);
		regset++;
	}
	/* Broadcast Storm Protection enable */
	{
		val = cml_eth_mdio_rd(20, 16);
		cml_eth_mdio_wr(20, 16 , val|0x1f00);
		cml_eth_mdio_wr(20, 17 , 0xffff);
		cml_eth_mdio_wr(20, 18 , 0xffff);

		val = cml_eth_mdio_rd(20, 19);
		cml_eth_mdio_wr(20, 19, val|0x0ff);
	}

}

/*=============================================================*
 *  cml_eth_setmac:
 *=============================================================*/
/*!
 * \brief Set the MAC address for a specified Ethernet interface.
 *
 * \param unit: the number for specified interface
 * \param mac: Pointer to a 6-byte array which contain the address
 *
 * \return None
 */
static void cml_eth_setmac(int unit, unsigned char *mac)
{
	char fid;

	if (unit==0)
	{
		ETH_REG32(MSA00) = (mac[0] << 8) | mac[1] ;
  		ETH_REG32(MSA01) = (mac[2] << 24) | (mac[3] << 16) | (mac[4] << 8) | mac[5];
		fid = 1;
	}
	else
	{
		ETH_REG32(MSA10) = (mac[0] << 8) | mac[1] ;
  		ETH_REG32(MSA11) = (mac[2] << 24) | (mac[3] << 16) | (mac[4] << 8) | mac[5];
		fid = 2;
	}
	DBGPRINTF(DBG_INFO, "ifcml_eth%d: %s mac %02x:%02x:%02x:%02x:%02x:%02x\n",
			unit, "add", 
			mac[0], mac[1],	mac[2], mac[3], mac[4], mac[5]);

	/* configure myself's mac address in switch to avoid mac address land attack */
	cml_eth_macatable_wr((char *)mac, fid, 0, (0x6<<3)|(fid<<6), 0x2);

}

/*=============================================================*
 *  cml_eth_macatable_rd:
 *=============================================================*/
/*!
 * \brief Get the information from switch address table.
 *
 * \param mac: Pointer to a 6-byte array which contain the address
 * \param fid: the number of fid
 * \param num: entry number
 * \param databuf: data pointer for storing entry information
 *
 * \return None
 */
void cml_eth_macatable_rd(unsigned char *mac, char fid, char num, unsigned short *databuf)
{
	unsigned short idx;

	if(cml_eth_mdio_rd(20, 13) & 0x8)
	{
		idx = fid ^ mac[0] ^ mac[1] ^ mac[2] ^ mac[3] ^ mac[4] ^ mac[5];
		if(mac[0] & 1)
			idx = idx<<2|num|(1<<11);
		else
			idx = idx<<2|num;
	}
	else
	{
		idx =	(fid<<3 | (mac[0]>>5)) ^ 
		((((short)mac[0]&0x1f) << 4)|((((short)mac[1]) >> 4))) ^  
		((((short)mac[1]&0xf) << 5)|((((short)mac[2]) >> 3))) ^
		((((short)mac[2]&0x7) << 6)|((((short)mac[3]) >> 2))) ^
		((((short)mac[3]&0x3) << 7)|((((short)mac[4]) >> 1))) ^
		((((short)mac[4]&0x1) << 8)|(((short)mac[5])));

		idx = idx<<2|num;
	}

	cml_eth_mdio_wr(21, 14, idx|(2<<11)|(1<<15));
	while((cml_eth_mdio_rd(21, 14) & 0x2000) == 0);

	databuf[0] = cml_eth_mdio_rd(21, 15);
	databuf[1] = cml_eth_mdio_rd(21, 16);
	databuf[2] = cml_eth_mdio_rd(21, 17);
	databuf[3] = cml_eth_mdio_rd(21, 18);
	databuf[4] = cml_eth_mdio_rd(21, 19);
}

/*=============================================================*
 *  cml_eth_macatable_wr:
 *=============================================================*/
/*!
 * \brief Set the information to switch address table.
 *
 * \param mac: Pointer to a 6-byte array which contain the address
 * \param fid: the number of fid
 * \param num: entry number
 * \param info: reg 21.18
 * \param flag: reg 21.19
 *
 * \return None
 */
void cml_eth_macatable_wr(unsigned char *mac, char fid, char num, unsigned short info, unsigned short flag)
{
	unsigned short idx;
	if(cml_eth_mdio_rd(20, 13) & 0x8)
	{
		idx = fid ^ mac[0] ^ mac[1] ^ mac[2] ^ mac[3] ^ mac[4] ^ mac[5];
		if(mac[0] & 1)
			idx = idx<<2|num|(1<<11);
		else
			idx = idx<<2|num;
	}
	else
	{
		idx =	(fid<<3 | (mac[0]>>5)) ^ 
		((((short)mac[0]&0x1f) << 4)|((((short)mac[1]) >> 4))) ^  
		((((short)mac[1]&0xf) << 5)|((((short)mac[2]) >> 3))) ^
		((((short)mac[2]&0x7) << 6)|((((short)mac[3]) >> 2))) ^
		((((short)mac[3]&0x3) << 7)|((((short)mac[4]) >> 1))) ^
		((((short)mac[4]&0x1) << 8)|(((short)mac[5])));

		idx = idx<<2|num;
	}
	cml_eth_mdio_wr(21, 15, *(unsigned short *)&mac[4]);
	cml_eth_mdio_wr(21, 16, *(unsigned short *)&mac[2]);
	cml_eth_mdio_wr(21, 17, *(unsigned short *)&mac[0]);
	cml_eth_mdio_wr(21, 18, info);
	cml_eth_mdio_wr(21, 19, flag);

	cml_eth_mdio_wr(21, 14, idx|(1<<11)|(1<<15));
}

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_hwreset:
 *=============================================================*/
static void cml_eth_hwreset(void)
{
	DBGPRINTF(DBG_INIT, "cml_eth_hwreset\n");
	ETH_REG32(MSRST) = 0xfff;
	ETH_REG32(MSRST) = 0;
}
/// @endcond

/*=============================================================*
 *  cml_eth_txd_init:
 *=============================================================*/
/*!
 * \brief Initialize the transmit buffer descriptors.
 *
 * \param ptxc: pointer of the transmit descriptor control
 *
 * \return None
 */
static void cml_eth_txd_init(txctrl_t *ptxc)
{

	DBGPRINTF(DBG_INIT, "ifcml_eth: init %d txd at %08x\n", ptxc->num_txd,  (unsigned int) ptxc->ptxd);
	/* zero all tx desc */
	memset((void *) ptxc->ptxd, 0, ptxc->num_txd * sizeof(txdesc_t));
	/* Init buffer descriptor  */
	memset((void *) ptxc->ptxbuf, 0, ptxc->num_txd * sizeof(bufdesc_t));
	
	/*  Mark end of ring at the last tx descriptor  */
	ptxc->ptxd[ptxc->num_txd-1].w0 = DS0_END_OF_RING;
	
	ptxc->num_freetxd = ptxc->num_txd;
	ptxc->tx_head = ptxc->tx_tail = 0;
}


/*=============================================================*
 *  cml_eth_rxd_init:
 *=============================================================*/
/*!
 * \brief Initialize the receive buffer descriptors.
 *
 * \param prxc: pointer of the receive descriptor control.
 *
 * \return None
 */
static void cml_eth_rxd_init(rxctrl_t *prxc)
{
	int i;
	rxdesc_t *prxd;
		
	DBGPRINTF(DBG_INIT, "ifcml_eth: init %d rxd at %08x\n",
			prxc->num_rxd, (unsigned int) prxc->prxd);

	prxd = prxc->prxd;
	for (i = prxc->num_rxd; i>0; i--, prxd++)
	{
		prxd->w1 = 0;
		prxd->w0 = DS0_RX_OWNBIT;
	}
	(--prxd)->w0 = DS0_END_OF_RING|DS0_RX_OWNBIT;

	prxc->num_freerxd = prxc->num_rxd;
	prxc->rx_head = prxc->rx_tail = 0;
}
/// @cond DOXYGEN

/*=============================================================*
 *  cml_eth_txd_free:
 *=============================================================*/
static void cml_eth_txd_free(txctrl_t *ptxc)
{
	int i;
	void *buf;
	bufdesc_t *ptxbuf = &ptxc->ptxbuf[0];
	
	/*  free buffer */
	for(i=0; i < ptxc->num_txd; i++,ptxbuf++)
	{
		if ((buf=ptxbuf->buf))
		{
#ifdef __ECOS
			struct eth_drv_sc *sc;
			if ((sc=ptxbuf->sc))
				(sc->funs->eth_drv->tx_done)( sc, (CYG_ADDRESS)buf, 0);
			else
#endif
				eth_drv_freebuf(buf);
		}
	}	
}


/*=============================================================*
 *  cml_eth_rxd_free:
 *=============================================================*/
#if 0
static void cml_eth_rxd_free(rxctrl_t *prxc)
{
	//TODO 
	// free according to w1's info
}
#endif
/// @endcond

/*=============================================================*
 *  cml_eth_rx_start:
 *=============================================================*/
/// @cond DOXYGEN
static void cml_eth_rx_start(void)
{
  ETH_REG32(MCR) |= (MCR_RXDMA_EN | MCR_RXMAC_EN);
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_tx_start:
 *=============================================================*/
static void cml_eth_tx_start(void)
{
  ETH_REG32(MCR) |= (MCR_TXDMA_EN | MCR_TXMAC_EN);
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_hwd_init:
 *=============================================================*/
#define	HWD_RDY	(1<<31)
#define	HWD_WR	(1<<28)
#define	HWD_IDX_S 18 	// which entry (0~23)
#define	HWD_HWI_S 16 	//which half-word (0~3)

static void cml_eth_hwd_init(void)
{
	unsigned short i,j;
	unsigned long x;

	for(i=0;i<24;i++)
	{
		for (j=0; j < 4; j++)
		{
			while (HWD_RDY & ETH_REG32(HWDESC));
			x = HWD_RDY|HWD_WR |((unsigned long)i<<HWD_IDX_S) | ((unsigned long)j<<HWD_HWI_S) ;
			if (j==0)
			{
				x |= ((unsigned long)DS0_RX_OWNBIT >>16)  |
					((i==23)?  (DS0_END_OF_RING >>16) : 0 ) ;
			}
			//diag_printf("x=%08x\n", x);
			ETH_REG32(HWDESC) = x; 
		}
	}
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_hwinit:
 *=============================================================*/
static void cml_eth_hwinit(void)
{
	DBGPRINTF(DBG_INIT, "%s\n", __func__);

	cml_eth_hwd_init();
	/*  descriptor base  */
	ETH_REG32(MHWDB) = (unsigned int) pcml_eth->hw.prxd;
	ETH_REG32(MTXDB) = (unsigned int) pcml_eth->tx.ptxd;
	ETH_REG32(MSWDB) = (unsigned int) pcml_eth->rx.prxd;
	
	ETH_REG32(MAC_RXCR) = 0x08401004;

#ifdef	CONFIG_CML_FPGA
	ETH_REG32(MFP) = 0x14ffff;
#else
	//ETH_REG32(MFP) = 0x2dffff;
	ETH_REG32(MFP) = 0x10ffff;
#endif

	ETH_REG32(MVL01) = 0x10020001;
	ETH_REG32(MVL23) = 0x10040003;
	ETH_REG32(MVL45) = 0x10060005;
	ETH_REG32(MVL67) = 0x10080007;	

	ETH_REG32(MCR) = MCR_INIT ;
  	ETH_REG32(MRXCR) = MRXCR_INIT ;
  	ETH_REG32(MAUTO) = 0x81ff0030 ;
	ETH_REG32(MDROPS) = MDROPS_INIT ;
	ETH_REG32(ML3) = ML3_INIT ;
	
	/* Clear all pending interrupts */
	ETH_REG32(MIC)= ETH_REG32(MIS);
	
	/*  Enable interrupt  */
	// enable in cml_eth_if_start
	//ETH_REG32(MIM) &= ~MIM_INIT;
	
   	//using icplus 175d
	DBGPRINTF(DBG_INIT, "\nInit switch.\n");
	cml_eth_switch_init();	
	
	/* HW NAT initialize */
	cml_hnat_init();

}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_buf_free:
 *=============================================================*/
int cml_eth_buf_free (void)
{
	unsigned long cb,nb,*p;

	if (0 ==(cb = ETH_REG32(MBUFB)))
	{
		DBGPRINTF(DBG_INIT, "%s MBUF=0,can't feee\n",__func__);
		return -1;
	}
	for (;cb;)
	{
		if (3 & (unsigned long)cb )
		{
			DBGPRINTF(DBG_INIT, "%s unaligned BUF=%08lx,can't free\n",__func__, cb);
			break;
		}
		p = (unsigned long*)nonca_addr((cb<<8)>>8);
		nb = *p; // next
		eth_drv_freebuf( (void*)( PHYTOVA((cb<<8)>>8)-BUF_HW_OFS)) ;
		cb = nb;	
	}
	return 0;
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_buf_init:
 *=============================================================*/
int cml_eth_buf_init (void)
{
	unsigned long first, cb, nb;
	int i;

	if (!(cb = first = (unsigned long)eth_drv_getbuf()))
	{
		goto no_buf;
	}
	ETH_REG32(MBUFB) = virtophy(first)+BUF_HW_OFS ;
	for (i = 0; i < NUM_MBUF-1; i++)
	{
		if (!(nb = (unsigned long)eth_drv_getbuf()))
		{
			goto free;
		}
		/* current buf's head point to next buf's head */
		*(unsigned long *)(nonca_addr(cb)+BUF_HW_OFS) = virtophy(nb)+BUF_HW_OFS;
		//*(unsigned char*)(nonca_addr(cb)+BUF_MSB_OFS) = ((unsigned int)cb)>>24 ;	//saved the pointer (MSB 16bits) of buffer
		cb = nb ;
	}
	*(unsigned long *)(nonca_addr(cb)+BUF_HW_OFS) = 0;
	ETH_REG32(MLBUF) = virtophy(cb)+BUF_HW_OFS;
	DBGPRINTF(DBG_INIT, "%s: MBUF=%x, MLBUF=%x\n", __func__, ETH_REG32(MBUFB), ETH_REG32(MLBUF));

	return 0;

free:
	cml_eth_buf_free();
no_buf:
	DBGPRINTF(DBG_INIT, "%s:no buf\n",__func__);
	return -1;
}
/// @endcond


/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_init:
 *=============================================================*/
static int cml_eth_init(void)
{
	pcml_eth = &cml_eth;
	
	DBGPRINTF(DBG_INIT, "%s\n", __func__);

	/*  Make sure the uncached memory area is not cached!!  */
	HAL_DCACHE_FLUSH(g_desc, DESC_SZ);

	cml_eth_preset_175d();
	/*  Reset switch hardware  */
	cml_eth_hwreset();

	//cml_eth_phyinit();
	pcml_eth->tx.ptxd = (txdesc_t *) nonca_addr((unsigned int)g_desc);
	pcml_eth->rx.prxd = (rxdesc_t *)(pcml_eth->tx.ptxd + NUM_TX_DESC);
	pcml_eth->hw.prxd = (rxdesc_t *)(pcml_eth->rx.prxd + NUM_RX_DESC);
	
	pcml_eth->tx.ptxbuf = &g_txbd[0];
	pcml_eth->tx.num_txd = NUM_TX_DESC;
	pcml_eth->rx.num_rxd = NUM_RX_DESC;
	pcml_eth->hw.num_rxd = NUM_HW_DESC;
	
	if (cml_eth_buf_init())
		return -1;
	
	cml_eth_txd_init(&pcml_eth->tx);
	cml_eth_rxd_init(&pcml_eth->rx);
	cml_eth_rxd_init(&pcml_eth->hw);

	pcml_eth->vector = CML_ETH_IRQ;
#ifdef	__ECOS	
	cyg_drv_interrupt_create(
		pcml_eth->vector,
		0,                  	// Priority - unused
		(CYG_ADDRWORD)pcml_eth, // Data item passed to ISR & DSR
		cml_eth_isr,            // ISR
		cml_eth_dsr,            // DSR
		&pcml_eth->int_hdl, 	// handle to intr obj
		&pcml_eth->int_obj ); 	// space for int obj

	cyg_drv_interrupt_attach(pcml_eth->int_hdl);

	cyg_drv_interrupt_configure(pcml_eth->vector, 1, 0 );
#else
#ifdef	USED_DSR
	tasklet_init(&tasklet, (void *)cml_eth_deliver, 0);
#endif
    if( request_irq(pcml_eth->vector, (irq_handler_t)&cml_eth_isr, SA_INTERRUPT, "cml_eth_eth", pcml_eth) != 0 )     /* request non-shared interrupt */
    {
       	diag_printf( "%s: irq %d request fail\n", __func__, pcml_eth->vector );
       	return -ENODEV;
    }
    disable_irq(pcml_eth->vector);   // create IRQ handler with IRQ disabled, mimic the eCos behavior
#endif
	/*  Initialize hardware  */
	cml_eth_hwinit();

	cml_eth_mdio_wr(4,5,0x480);

	return 0;
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_if_start:
 *=============================================================*/
static void cml_eth_if_start(int unit)
{
	if_cml_eth_t *pifcml_eth;
	unsigned short if_bit;

	pifcml_eth = pcml_eth->if_array[unit];
	
	CYG_ASSERTC(pifcml_eth != 0);

	if_bit = 1 << unit;

	DBGPRINTF(DBG_INIT, "if321x_if_start(%d)\n", unit );
	if (pcml_eth->act_ifmask & if_bit)
		return;

	if (pcml_eth->act_ifmask == 0)
	{
		pcml_eth->sc_main = pifcml_eth->sc;
		//TODO , add initial code 

		/*  Enable interrupt  */
		ETH_REG32(MIM) &= ~MIM_INIT;
	
		cyg_drv_interrupt_unmask(pcml_eth->vector);
	}
	pcml_eth->act_ifmask |= if_bit;
	DBGPRINTF(DBG_INIT, "if321x_if_start: act_ifmask=%x\n", pcml_eth->act_ifmask);

}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_rx_stop:
 *=============================================================*/
static void cml_eth_rx_stop(rxctrl_t *prxc)
{
	int i;
	rxdesc_t *prxd;
	void *bufp;
	
  	ETH_REG32(MCR) &= ~(MCR_RXDMA_EN | MCR_RXMAC_EN);
  	for (i = 0; i < prxc->num_rxd; i++)
  	{
    		prxd = prxc->prxd + i;
    		if(((DS0_RX_OWNBIT | DS0_TX_OWNBIT) >> 16) & *(unsigned short *) prxd)
    		{
    			prxd->w0 &= ~(DS0_RX_OWNBIT | DS0_TX_OWNBIT);
      			prxd->w0 |= DS0_DROP_OF_PACKET;
			if (prxd->w1&0xffffff)
			{
				bufp = (void*)DESC_W1_TO_VBUF(prxd->w1);
				eth_drv_freebuf(bufp);
			}
    		}		
    	}	
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_tx_stop:
 *=============================================================*/
static void cml_eth_tx_stop(txctrl_t *ptxc)
{
	int i;
	txdesc_t *ptxd;
	
	ETH_REG32(MCR) &= ~(MCR_TXDMA_EN | MCR_TXMAC_EN);
  	for (i = 0; i < ptxc->num_txd; i++)
  	{
    		ptxd = ptxc->ptxd + i;
    		if(((DS0_RX_OWNBIT | DS0_TX_OWNBIT) >> 16) & *(unsigned short *) ptxd)
    		{
    			ptxd->w0 &= ~(DS0_RX_OWNBIT | DS0_TX_OWNBIT);
      			ptxd->w0 |= DS0_DROP_OF_PACKET;
    		}
    	}	
}	
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_if_stop:
 *=============================================================*/
static void cml_eth_if_stop(int unit)
{
	if_cml_eth_t *pifcml_eth;
	unsigned short if_bit;

	pifcml_eth = pcml_eth->if_array[unit];
	if_bit = 1 << unit;

	DBGPRINTF(DBG_INIT, "cml_eth_if_stop(%d) ifp=%x act_ifmask=%x\n", unit, (int) pifcml_eth, (int) pcml_eth->act_ifmask);
	CYG_ASSERTC(pifcml_eth != 0);
	if (!(pcml_eth->act_ifmask & if_bit))
		return;
	
	if ((pcml_eth->act_ifmask &= ~if_bit) == 0)
	{
		/*  Disable interrupt  */
		ETH_REG32(MIM)|= MIM_INIT;
		
		cyg_drv_interrupt_mask(pcml_eth->vector);
#if 1
		if (0)
		{
		cml_eth_rx_stop(&pcml_eth->rx);
		cml_eth_tx_stop(&pcml_eth->tx);
		cml_eth_txd_free(&pcml_eth->tx);
		}
		diag_printf("Don't free NOW!\n");
#endif
		pcml_eth->sc_main = 0;
		return;
	}
	DBGPRINTF(DBG_INIT, "if321x_if_stop: return act_ifmask=%x\n", pcml_eth->act_ifmask);
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_add_if:
 *=============================================================*/
static int cml_eth_add_if(int unit, if_cml_eth_t *pifcml_eth)
{
	int status = -1;
	
	if (unit <0 || unit >= MAX_IF_NUM)
		return -1;
	
	if (pcml_eth->if_array[unit] == 0)
	{
		pcml_eth->if_array[unit] = pifcml_eth;
		pifcml_eth->to_vlan = ((unit|0x8) << 24);
		pcml_eth->if_cnt++;
		status = 0;
	}
	
	return status;
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_del_if:
 *=============================================================*/
static int cml_eth_del_if(int unit)
{
	if_cml_eth_t *pifcml_eth;

	if (unit < 0 || unit >= MAX_IF_NUM)
		return -1;

	DBGPRINTF(DBG_INIT, "cml_eth_del_if(%d)\n", unit);
	
	pifcml_eth = pcml_eth->if_array[unit];
	CYG_ASSERTC(pifcml_eth != 0);

	cml_eth_setmac(unit, pifcml_eth->macaddr);
	
	pcml_eth->if_array[unit] = 0;
	
	return 0;
}
/// @endcond

/// @cond DOXYGEN
/*======================================================================*/
/*  Interrupt handling  */

#ifdef __ECOS
/*=============================================================*
 *  cml_eth_isr:
 *=============================================================*/
static unsigned int cml_eth_isr(cyg_vector_t vec, cyg_addrword_t data)
{
	/*  Disable futher interrupt from the device.  */
	ETH_REG32(MIM)|= MIM_INIT;
	DBGPRINTF(DBG_ISR, "cml_eth_isr\n");

	cyg_drv_interrupt_acknowledge(vec);
	return CYG_ISR_CALL_DSR;	/* schedule DSR*/	
}
#else
static irqreturn_t cml_eth_isr( int int_num, void *dev_id, struct pt_regs *regs )  /* device interrupt handler */
{
//    struct net_device *dev = (struct net_device *)dev_id;
	ETH_REG32(MIM)|= MIM_INIT;
#ifdef	USED_DSR
	{
		tasklet_hi_schedule(&tasklet);
		return IRQ_HANDLED;
	}
#else
	cml_eth_deliver(0);
	return IRQ_HANDLED;
#endif
}
#endif //__ECOS
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_dsr:
 *=============================================================*/
#ifdef __ECOS
static void cml_eth_dsr(cyg_vector_t vec, cyg_ucount32 count, cyg_addrword_t data)
{
	DBGPRINTF(DBG_DSR, "cml_eth_dsr main=%x\n", pcml_eth->sc_main);

	if (pcml_eth->sc_main)
	{
		/*  schedule deliver routine  */
		eth_drv_dsr(vec, count, (cyg_addrword_t) pcml_eth->sc_main);
	}

}
#else

#endif //__ECOS
/// @endcond

/*=============================================================*
 *  cml_eth_rxint:
 *=============================================================*/
 /*!
 * \brief The receive interrupt service handler.
 *
 * \param prxc: pointer of the receive descriptor control.
 *
 * \return None
 */
static void cml_eth_rxint(rxctrl_t *prxc)
{
	register unsigned int w0, w1;
	unsigned char *nbufp, *bufp;
	unsigned char *pkt;
	rxdesc_t *prxd;
	short frame_len=0;
	if_cml_eth_t *pifcml_eth;
	int loop;

	prxd = prxc->prxd;

	DBGPRINTF(DBG_RXMORE, "rxint, rxc=%x\n", (unsigned int) prxc);

	prxd = &prxc->prxd[prxc->rx_head];
	w0 = prxd->w0;
	w1 = prxd->w1;
	loop=0;

	while (!(w0 & (DS0_RX_OWNBIT)) && (loop++ < (NUM_RX_DESC/1)))
	{
		DBGPRINTF(DBG_RXMORE, "rxint, DS0 w0=%x w1=%x\n",  w0, w1);
		pkt = 0;
		/* For statistics, locate the interface first */
		DBGPRINTF(DBG_RXMORE, "vl=%x\n", RX_SPORT(w1));
		if ((pifcml_eth = pcml_eth->if_array[RX_SPORT(w1)]) == 0)
		{
			/*  The port has been released.  */
			/*  Drop the packet  */
			prxd->w1=((w1>>6)<<6) + BUF_HW_OFS;
 
			goto next;
		}

		if(w0 & DS0_DROP_OF_PACKET)
		{
			if(w0 & (DS0_EIPCS|DS0_EL4CS))
			{
				prxc->rx_err++;
				prxc->rx_errcode[RX_ERROR(w0)]++;
				/*  Drop the packet  */
				prxd->w1=((w1>>6)<<6) + BUF_HW_OFS;
				goto next;
			}
			else
			{
#if 0				
				diag_printf("Drop!!!, cause:%x\n", RX_ERROR(w0));
				bufp = (unsigned char*)nonca_addr(w1);
				bufp = (unsigned char*)((0xa0<<24) | ((((unsigned int)bufp)<<8)>>8));
				diag_dump_buf_16bit(bufp,64);
#endif				
			}
		}

		frame_len = RX_FRAMELEN(w0);
		prxc->rx_bytecnt += frame_len;
		
		/* check broadcast or multicast packet */
		if(w0 & DS0_ISBC)
			prxc->rx_bcpkt++;
		else if(w0 & DS0_ISMC)
			prxc->rx_mcpkt++;

		prxc->rx_pktcnt++;
		/*  Indicate the packet to the upper layer */
		if ((pifcml_eth->flags & PRIVIFFG_ACTIVE) 
			&& (0!=(nbufp=(char*)eth_drv_getbuf())) )
		{
			pkt = (unsigned char*)DESC_W1_TO_VPKT(w1);
			bufp =  (unsigned char*)DESC_W1_TO_VBUF(w1);
			nbufp += BUF_HW_OFS ;
			prxd->w1 = (unsigned long)nbufp ; 
			DC_FLUSH_RX(nbufp, RX_CACHE_FLUSH_SIZE);
		}
		else
		{
			prxd->w1=((w1>>6)<<6) + BUF_HW_OFS;
		}
next:
		/*  Release the buffer to hardware  */
		if (++prxc->rx_head == prxc->num_rxd)
		{
			prxd->w0 = DS0_RX_OWNBIT | DS0_END_OF_RING;
			prxc->rx_head = 0;
		}
		else
			prxd->w0 = DS0_RX_OWNBIT;

		/* dispatch to upper layer */
		if(pkt != 0)
		{
			void *hw_nat;
			//diag_dump_buf_16bit(bufp+0x40,64);
			//printf("hash[0]:%x, hash[1]:%x\n", *(int *)(bufp+4), *(int *)(bufp+12));
				
			hw_nat = (void*)cml_hnat_rx_check(bufp, pkt, w0, w1);
			if(cml_hnat_sdmz_rx_check(pifcml_eth, bufp, pkt, w0, w1, frame_len-4) == 0)
				eth_drv_recv1(pifcml_eth->sc, pkt, (short)frame_len-4, (char**)&bufp, hw_nat); //4byte VLAN
		}
		prxd = &prxc->prxd[prxc->rx_head];
		w0 = prxd->w0;
		w1 = prxd->w1;
	}
	
	DBGPRINTF(DBG_RXMORE, "rxint end, prxc=%x\n", (unsigned int)prxc);
}


/*=============================================================*
 *  cml_eth_txint:
 *=============================================================*/
 /*!
 * \brief The transmit interrupt service handler.
 *
 * \param ptxc: pointer of the transmit descriptor control.
 *
 * \return None
 */
void cml_eth_txint(txctrl_t *ptxc, int wait_free)
{
	volatile txdesc_t *ptxd;
	bufdesc_t *ptxbuf;
	unsigned int tx_ctrl;
	
	DBGPRINTF(DBG_TXMORE, "txint, txc=%x\n", (unsigned int) ptxc);

	ptxd = ptxc->ptxd;
	ptxbuf = ptxc->ptxbuf;
	tx_ctrl = ptxd[ptxc->tx_tail].w0;

#if 1
	if(wait_free && (ptxd[ptxc->tx_tail].w0 & DS0_TX_OWNBIT))
	{
		int i = 0;
		int j;
		diag_printf("txbusy %s(%d) tail=%d head=%d\n", __func__, __LINE__, ptxc->tx_head, ptxc->tx_tail);
		for (j=0;j < ptxc->num_txd;j++)
		{
				if ((j&3)==0)
					diag_printf("\n");
				diag_printf("%08x-%08x ", ptxd[j].w0, ptxd[j].w1 );
		}
		while((ptxd[ptxc->tx_tail].w0&DS0_TX_OWNBIT))
		{
			i++;
		}
		diag_printf("i=%d\n", i);
	}
#endif
	while ((!(ptxd[ptxc->tx_tail].w0 & DS0_TX_OWNBIT)) && (ptxc->num_freetxd < ptxc->num_txd)) 
	{

		DBGPRINTF(DBG_TX, "TD: %d,%x,%x", ptxc->tx_tail,ptxd[ptxc->tx_tail].w0, ptxd[ptxc->tx_tail].w1);
		DBGPRINTF(DBG_TX, ",%x\n", (unsigned int)ptxbuf[ptxc->tx_tail].buf);
		if (ptxbuf[ptxc->tx_tail].buf != 0)
		{
			ptxc->tx_ok++;
#ifdef __ECOS
			if (ptxbuf[ptxc->tx_tail].sc != 0)
			{
				(ptxbuf[ptxc->tx_tail].sc->funs->eth_drv->tx_done)
					(ptxbuf[ptxc->tx_tail].sc, 
					(unsigned int) ptxbuf[ptxc->tx_tail].buf, 0);
			}
			else
			{
				CYG_ASSERTC((unsigned int)ptxbuf[ptxc->tx_tail].buf > 0x80000000);
				eth_drv_freebuf((void *)ptxbuf[ptxc->tx_tail].buf);
			}
#else

			if ((unsigned int)ptxbuf[ptxc->tx_tail].buf >= 0x80000000)
			{
				dev_kfree_skb_irq(ptxbuf[ptxc->tx_tail].buf);
			}
#endif //__ECOS

			ptxbuf[ptxc->tx_tail].sc = 0;
			ptxbuf[ptxc->tx_tail].buf = 0;
		}

		ptxc->num_freetxd++;
		if (++ptxc->tx_tail == ptxc->num_txd)
			ptxc->tx_tail = 0;

		//tx_ctrl = ptxd[ptxc->tx_tail].w0;
	}
	DBGPRINTF(DBG_TXMORE, "txint end, txc=%x\n", (unsigned int) ptxc);
}

/// @cond DOXYGEN
#if 0
/*=============================================================*
 *  cml_eth_swint:
 *=============================================================*/
static void cml_eth_swint(cml_eth_t *pdev)
{
}
#endif
/// @endcond


/*======================================================================*/
/*  Interface functions							*/

/*=============================================================*
 *  cml_eth_init:
 *=============================================================*/
 /*!
 * \brief This function attaches the Ethernet interface to the OS. It also calls various driver 
 *        initialization functions to allocate drivers resources and initialize the device.
 *
 * \param pnde: the pointer of the network device.
 *
 * \return TRUE: Success
 * \return FALSE: Failure
 */
#ifdef __ECOS
static bool cml_eth_drv_init(struct cyg_netdevtab_entry *pnde)
{
	struct eth_drv_sc *sc;
	if_cml_eth_t *pifcml_eth;
	int unit;

	sc = (struct eth_drv_sc *)(pnde->device_instance);
	pifcml_eth = (if_cml_eth_t *) sc->driver_private;
	unit = pifcml_eth->unit;
	
	DBGPRINTF(DBG_INFO, "cml_eth_drv_init: pnde=%x, sc=%x, unit=%d\n",
			(unsigned int) pnde, (unsigned int) sc, unit);
	
	if ((pcml_eth == 0)  && cml_eth_init() != 0)
	{
		DBGPRINTF(DBG_ERR, "cml_eth_drv_init: failed!\n");
		return false;
	}
	pifcml_eth->sc = sc;

	if (cml_eth_add_if(unit, pifcml_eth) != 0)
		goto err_out1;

	if (!(pifcml_eth->flags & PRIVIFFG_MAC_SET))
	{
		/*  Read ethernet MAC address from HAL  */
		HAL_READ_ETHMAC(unit, pifcml_eth->macaddr);
	}
	
	cml_eth_setmac(unit, pifcml_eth->macaddr);
	
	(sc->funs->eth_drv->init)(sc, pifcml_eth->macaddr);
	return true;
	
err_out2:
	cml_eth_del_if(unit);

err_out1:
	DBGPRINTF(DBG_ERR, "cml_eth_drv_init: failed 2!\n");
	return false;
}
#else //!__ECOS

static void cml_eth_rx_mode(struct net_device *dev)
{
	 if (dev->flags & IFF_PROMISC )
  		ETH_REG32(MDROPS) |= (1<<7);	//received include address un-matched
	return ;	
}
/*
static struct net_device_stats *net_get_stats(struct net_device *dev)
{
	return 0;
}
static int set_mac_address(struct net_device *dev, void *p)
{
	return 0;
}
*/
static int cml_eth_drv_init(int unit)
{
	if_cml_eth_t *pifcml_eth;
	char mac_addr[6] = { 0, 0 , 0x32, 0x10, 0x00, 0xaa };
	int rc;
    struct net_device *dev;

	/* dev and priv zeroed in alloc_etherdev */
	dev = alloc_etherdev (sizeof (if_cml_eth_t));
	if (dev == NULL) {
		printk("%s Unable to alloc new net device\n",__func__);
		return -ENOMEM;
	}
	sprintf(dev->name, "eth%d", unit);
	pifcml_eth = (if_cml_eth_t *) dev->priv;
	dev->base_addr = MAC_BASE;
	dev->irq = IRQ_MAC;
	pifcml_eth->unit=unit;
	
	//printk("####### %s init ++++++++\n\n", dev->name);
	DBGPRINTF(DBG_INFO, "cml_eth_drv_init: dev=%x, unit=%d\n", (unsigned int) dev, unit);
	if ((pcml_eth == 0)  && cml_eth_init() != 0)
	{
		DBGPRINTF(DBG_ERR, "cml_eth_drv_init: failed!\n");
		return -1;
	}
	pifcml_eth->sc = dev;
	rc = register_netdev (dev);
	if (rc)
		goto err_out1;

	if (cml_eth_add_if(unit, pifcml_eth) != 0)
		goto err_out2;

    memcpy( pifcml_eth->macaddr, mac_addr, 6 );       /* Set MAC address */
    pifcml_eth->macaddr[5] += (char) unit ;

	cml_eth_setmac(unit, pifcml_eth->macaddr);
    memcpy( dev->dev_addr, pifcml_eth->macaddr, dev->addr_len );       /* Set MAC address */

    dev->open = &cml_eth_start;
    dev->hard_start_xmit = &cml_eth_send;
    dev->stop = &cml_eth_stop;
    dev->do_ioctl = &cml_eth_ioctl;
//  dev->get_stats = &cml_eth_get_stats;
    dev->set_multicast_list = &cml_eth_rx_mode;
//	dev->get_stats		= net_get_stats;
//	dev->set_mac_address 	= set_mac_address;
	return 0;
	
err_out2:
	cml_eth_del_if(unit);
err_out1:
	DBGPRINTF(DBG_ERR, "cml_eth_drv_init: failed 2!\n");
	return -1;
}
#endif
/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_start:
 *=============================================================*/
#ifdef	__ECOS
static void cml_eth_start(struct eth_drv_sc *sc, unsigned char *enaddr,
		int flags)
{
	if_cml_eth_t *pifcml_eth;
	
	pifcml_eth = (if_cml_eth_t *)sc->driver_private;
	
	CYG_ASSERTC(pifcml_eth->sc == sc);
	
	DBGPRINTF(DBG_INFO, "ifcml_eth%d: start\n", pifcml_eth->unit);

	if (!(pifcml_eth->flags & PRIVIFFG_ACTIVE))
	{
		pifcml_eth->flags |= PRIVIFFG_ACTIVE;
		cml_eth_if_start(pifcml_eth->unit);
	}
	
	cml_eth_rx_start();
	cml_eth_tx_start();
}
#else
//open
static int cml_eth_start(struct net_device *dev)
{
	if_cml_eth_t *pifcml_eth;
	
	pifcml_eth = (if_cml_eth_t *)dev->priv;
	
	CYG_ASSERTC(pifcml_eth->sc == dev);
	
	DBGPRINTF(DBG_INFO, "cml_eth_start(%d)\n", pifcml_eth->unit);

	if (!(pifcml_eth->flags & PRIVIFFG_ACTIVE))
	{
		pifcml_eth->flags |= PRIVIFFG_ACTIVE;
		cml_eth_if_start(pifcml_eth->unit);
	}
	
	cml_eth_rx_start();
	cml_eth_tx_start();

    memcpy(pifcml_eth->macaddr, dev->dev_addr, 6 );
	cml_eth_setmac(pifcml_eth->unit, pifcml_eth->macaddr);

    netif_start_queue (dev);

	DBGPRINTF(DBG_INFO, "cml_eth_start(%d): done\n", pifcml_eth->unit);
    return 0;
}
#endif
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_stop:
 *=============================================================*/
#ifdef __ECOS
static void cml_eth_stop(struct eth_drv_sc *sc)
{
	if_cml_eth_t *pifcml_eth;
	
	pifcml_eth = (if_cml_eth_t *)sc->driver_private;
	
	CYG_ASSERTC(pifcml_eth->sc == sc);

	DBGPRINTF(DBG_INFO, "cml_eth_stop(%d)\n", pifcml_eth->unit);

	if (pifcml_eth->flags & PRIVIFFG_ACTIVE)
	{
		pifcml_eth->flags &= ~PRIVIFFG_ACTIVE;
		cml_eth_if_stop(pifcml_eth->unit);
	}
}
#else
static int cml_eth_stop( struct net_device *dev )
{
	if_cml_eth_t *pifcml_eth;
	
	pifcml_eth = (if_cml_eth_t *)dev->priv;
	
	CYG_ASSERTC(pifcml_eth->sc == sc);

	DBGPRINTF(DBG_INFO, "cml_eth_stop(%d)\n", pifcml_eth->unit);

    netif_stop_queue(dev);

	if (pifcml_eth->flags & PRIVIFFG_ACTIVE)
	{
		pifcml_eth->flags &= ~PRIVIFFG_ACTIVE;
		cml_eth_if_stop(pifcml_eth->unit);
	}

	DBGPRINTF(DBG_INFO, "cml_eth_stop(%d): done\n", pifcml_eth->unit);
    return 0;
}
#endif
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_ioctl:
 *=============================================================*/
#ifdef __ECOS
static int cml_eth_ioctl(struct eth_drv_sc *sc, unsigned long key, 
		void *data, int data_len)
{
	if_cml_eth_t *pifcml_eth;
	
	pifcml_eth = (if_cml_eth_t *) sc->driver_private;
	
	CYG_ASSERTC(pifcml_eth->sc == sc);

	//DBGPRINTF(DBG_IOCTL, "ifcml_eth%d: ioctl=%08lx\n", pifcml_eth->unit, key);

	switch(key)
	{
	case ETH_DRV_SET_MAC_ADDRESS:
		if (data_len != 6)
			return -2;
		cml_eth_setmac(pifcml_eth->unit, (unsigned char *)data);
		return 0;

#ifdef ETH_DRV_GET_IF_STATS_UD
	case ETH_DRV_GET_IF_STATS_UD:
#endif	/*  ETH_DRV_GET_IF_STATS_UD  */

#ifdef ETH_DRV_GET_IF_STATS
	case ETH_DRV_GET_IF_STATS:
#endif /*  ETH_DRV_GET_IF_STATS  */

#if defined(ETH_DRV_GET_IF_STATS_UD) || defined(ETH_DRV_GET_IF_STATS)
	{
//		struct ether_drv_stats *p = (struct ether_drv_stats *)data;

	}

	return -1;
#endif	/*  ETH_DRV_GET_IF_STATS_UD || ETH_DRV_GET_IF_STATS  */

#ifdef ETH_DRV_SET_MC_ALL
	case ETH_DRV_SET_MC_ALL:
// TODO
		return 0;
#endif /*  ETH_DRV_SET_MC_ALL  */

#ifdef ETH_DRV_SET_MC_LIST
	case ETH_DRV_SET_MC_LIST:
		/*  Accept all multicast frame  */
// TODO
		return 0;
#endif /*  ETH_DRV_SET_MC_LIST  */

#ifdef SIOCGIFPHY
    	case SIOCGIFPHY:
		{
			struct ifreq *ifr= (struct ifreq *)data;
			unsigned int *p = (unsigned int *)ifr->ifr_ifru.ifru_data;
			short ls=cml_eth_mdio_rd(PHY_WAN_PORT, 1);
			
			DBGPRINTF(DBG_PHY, "phy status=%x\n", (unsigned int)ls);
			if (ls&(1<<2))
				*p = 1;		// LINK UP
			else
            			*p = 0;     // LINK DOWN
			return 0;
		}
		break;
#endif
#ifdef SIOCSIFPHY
    case SIOCSIFPHY:
		{
			struct ifreq *ifr= (struct ifreq *)data;
			unsigned int *p = (unsigned int *)ifr->ifr_ifru.ifru_data;
			int act=p_cml_eth_eth->active; //save for refer later
			unsigned char mode=*p;
			unsigned char cmd;

			if (p_cml_eth_eth->phy_mode==mode)
			{
				diag_printf("SIOCSIFPHY SAME\n");
				return 0;
			}
        	cml_eth_eth_stop( sc );
			p_cml_eth_eth->phy_mode=mode;

			p_cml_eth_eth->line_status|=GEN_STATUS_RESTART;

			if (act)
            	cml_eth_eth_start( sc, 0, 0 );
			return 0;
		}
		break;
#endif
#ifdef  SIOCSFC
	case SIOCSFC:
		{
			struct ifreq *ifr= (struct ifreq *)data;

			if((int)ifr->ifr_ifru.ifru_data)
			{
				diag_printf("~~~~~~~~SIOCSFC on\n");
				pcml_eth->flowctrl_mode = 1;
				pcml_eth->flowctrl_state = 0;
				ETH_REG32(MFC1) = 1;
			}
			else
			{
				diag_printf("~~~~~~~~SIOCSFC off\n");
				pcml_eth->flowctrl_mode = 0;
				pcml_eth->flowctrl_state = 0;
				/* toggle the bit */
				ETH_REG32(MFC1) = 1;
			}
		}
		break;
#endif
	default:
		return -1;
	}
	return 0;
}
#else

static int cml_eth_ioctl( struct net_device *dev, struct ifreq *ifr, int cmd )
{
	if_cml_eth_t *pdp= (if_cml_eth_t*)dev->priv;
        struct mii_ioctl_data *data_ptr = (struct mii_ioctl_data *) &(ifr->ifr_data);
	//DBGPRINTF(DBG_INFO, "cml_eth_ioctl(%d):cmd=%x ifr=%x\n", pifcml_eth->unit, cmd, (int)ifr);
    switch (cmd)
	{
        case SIOCGMIIPHY:
	        data_ptr->phy_id = pdp->phy_addr & 0x1f;
		break;
        case SIOCGMIIREG:
                cml_eth_mdio(pdp->phy_addr,data_ptr->reg_num & 0x1f,(&(data_ptr->val_out)),0);
                break;

        case SIOCSMIIREG:
		cml_eth_mdio(pdp->phy_addr,data_ptr->reg_num & 0x1f,&(data_ptr->val_out),1);
		break;
        default:
	                return -EOPNOTSUPP;
	}
	return 0;
}
#endif //__ECOS
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_cansend:
 *=============================================================*/
#ifdef __ECOS
static int cml_eth_cansend(struct eth_drv_sc *sc)
{
	int freecnt = 0;
	
	if_cml_eth_t *pifcml_eth;
	
	pifcml_eth = (if_cml_eth_t *) sc->driver_private;
	
	CYG_ASSERT(pifcml_eth->sc == sc, "Invalid device pointer");

	DBGPRINTF(DBG_TXMORE, "cml_eth_cansend(%d)\n", pifcml_eth->unit);

	if (pifcml_eth->flags & PRIVIFFG_ACTIVE)
		freecnt = pcml_eth->tx.num_freetxd;
		
	return freecnt;
}
#endif //__ECOS
/// @endcond

/*=============================================================*
 *  cml_eth_send:
 *=============================================================*/
 /*!
 * \brief This function transmits the given frame into the DMA engine of the 
 *				Ethernet controller. If the transmit data path is enable, the frame 
 *				will be transmit at once.
 *
 * \param sc: the pointer for the soft control of Ethernet driver.
 * \param sg_list: the packet which is specified via a ¡§scatter-gather¡¨ list.
 * \param sg_len: number of packet scatter in the list.
 * \param total_len: the packet total length.
 * \param key: the packet address for upper layer release.
 *
 * \return None
 */
#ifdef __ECOS
static void cml_eth_send(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
		int sg_len, int total_len, unsigned long key)
{
	if_cml_eth_t *pifcml_eth = (if_cml_eth_t *) sc->driver_private;
	txctrl_t *ptxc = &pcml_eth->tx;
	volatile txdesc_t *ptxd, *ptxd_start;
	void *u;
	int i, desc_cnt;
	unsigned long w0,w1;

	CYG_ASSERT(pifcml_eth->sc == sc, "Invalid device pointer");

	DBGPRINTF(DBG_TX, "cml_eth_send(%d) key=%08lx, len=%d\n",
			 pifcml_eth->unit, key, total_len);
	if(ptxc->num_freetxd == 0)
	{ 
		cml_eth_txint(ptxc, 1);
		if(ptxc->num_freetxd == 0)
			diag_printf("%s, ptxc->num_freetxd:%d\n", __func__, ptxc->num_freetxd);
	}
	if (ptxc->num_freetxd == 0 || total_len > 1518)
		goto drop_tx;

	ptxc->tx_pktcnt ++;

	if(1 == sg_len) 
		cml_hnat_learn_mac((char *)sg_list[0].buf, 1, RX_SPORT(pifcml_eth->to_vlan));

	if ((sg_len > TX_MAX_BUFCNT) || (sg_len > ptxc->num_freetxd))
	{

		DBGPRINTF(DBG_INFO, "coalesce frame key=%08lx, sg_list=%x, sg_len=%d\n",
				key, (unsigned int)sg_list, sg_len);
		/*  Too many buffer segments.  */
		/*  Coalesce the segments into a single buffer  */
		if ((u=(void*)eth_drv_getbuf()) == 0)
		{
			diag_printf("%s: no buf for joint, drop\n",__func__);
			goto drop_tx;
		}
	
		eth_drv_sg_copy(u, CML_ETH_NBUF_LEN, sg_list, sg_len);
		
#ifdef TX_LEN_PADDING
		if (total_len < ETH_MIN_FRAMELEN)
		{
			//memset( ((char*)u)+total_len, 0 , ETH_MIN_FRAMELEN - total_len);
			total_len = ETH_MIN_FRAMELEN;
		}
#endif
		
		ptxc->tx_ubuf++;
		
		ptxc->num_freetxd--;
		/*  Fill the Tx descriptor  */
		ptxd_start = &ptxc->ptxd[ptxc->tx_head];
		//diag_printf("\n\ncoalesce frame\n\n"); 
		DC_FLUSH_TX(u, total_len);
		{
		/*ptxd_start->*/ w1 = (unsigned int) virtophy(u);
		/*ptxd_start->*/ w0 = (ptxd_start->w0 & DS0_END_OF_RING) |
				(total_len << DS0_LEN_S) | DS0_TX_OWNBIT | pifcml_eth->to_vlan |DS0_DF;
			DBGPRINTF(DBG_TXMORE, "txd%d: %x,%x\n", w0, w1);
			ptxd_start->w1 = w1;
			ptxd_start->w0 = w0;
		}
			
		ptxc->ptxbuf[ptxc->tx_head].buf = (void *) u;

		if (++ptxc->tx_head == ptxc->num_txd)
			ptxc->tx_head = 0;

		/*  Release the sg buf  */
		(sc->funs->eth_drv->tx_done)( sc, key, 0 );
	}
	else
	{
		/*  Ensure the contents really is in SDRAM.  */
		for (i=0; i< sg_len; i++)
		{
			DC_FLUSH_TX(sg_list[i].buf, sg_list[i].len);
#if 0
			if (sg_len>1)
			{
				diag_printf("tx sg%d,l=%d\n", i, sg_list[i].len);
				//diag_dump_buf(sg_list[i].buf, sg_list[i].len);
			}
#endif
		}

#ifdef TX_LEN_PADDING
		if (total_len < ETH_MIN_FRAMELEN)
		{
			/* 
			 * XXX:
			 */
			//memset( sg_list[sg_len-1].buf+sg_list[sg_len-1].len, 0 , ETH_MIN_FRAMELEN - total_len);
			sg_list[sg_len - 1].len += ETH_MIN_FRAMELEN - total_len;
			total_len = ETH_MIN_FRAMELEN;
		}
#endif
		ptxc->tx_sglist++;

		ptxd_start = &ptxc->ptxd[ptxc->tx_head];


		/*  Use one buffers per descriptor  */
		ptxd_start->w1 = virtophy(sg_list[0].buf)
							| (pifcml_eth->to_vlan);
		ptxd_start->w0 = (ptxd_start->w0 & DS0_END_OF_RING)
					| (sg_list[0].len<<DS0_LEN_S);
		if (sg_len == 1)
		{
			ptxd_start->w0 |= DS0_TX_OWNBIT |DS0_DF;
		}
		else
		{
			for (i=1; i<sg_len; i++)
			{
				--ptxc->num_freetxd;
				if ( ++ptxc->tx_head == ptxc->num_txd)
					ptxc->tx_head = 0;
				ptxd = &ptxc->ptxd[ptxc->tx_head];
				ptxd->w1 = virtophy(sg_list[i].buf);
				ptxd->w0 = (ptxd->w0 & DS0_END_OF_RING)
					| (sg_list[i].len << DS0_LEN_S) | DS0_TX_OWNBIT |DS0_M |DS0_DF;
			}
			//ptxd->w0 |= DS0_TX_OWNBIT;
			ptxd->w0 &= ~DS0_M;
			
			ptxd_start->w0 |= DS0_TX_OWNBIT | DS0_M |DS0_DF ;
		}

		ptxc->ptxbuf[ptxc->tx_head].sc = sc;
		ptxc->ptxbuf[ptxc->tx_head].buf = (void *) key;		
		
		--ptxc->num_freetxd;
		if (++ptxc->tx_head == ptxc->num_txd)
			ptxc->tx_head = 0;
	}
	
	/*  Kick the hardware to start transmitting  */
	ETH_REG32(MTT) = 1;
	return;

drop_tx:
	/*  No tx resource for this frame  */
	DBGPRINTF(DBG_TX, "cml_eth_send(%d) drop frame %08lx\n",
			 pifcml_eth->unit, key);

	/*  Drop the frame now  */ 
	ptxc->tx_drop++;

	(sc->funs->eth_drv->tx_done)( sc, key, 0 );
	return;
}
#else

static int cml_eth_send(struct sk_buff *skb, struct net_device *dev )
{
	if_cml_eth_t *pifcml_eth = (if_cml_eth_t *) dev->priv;
	txctrl_t *ptxc = &pcml_eth->tx;
	volatile txdesc_t *ptxd_start;
	int total_len = skb->len; 
    /* EZP: 32MB fix */
    struct sk_buff *nskb;

	CYG_ASSERT(pifcml_eth->sc == dev, "Invalid device pointer");

	DBGPRINTF(DBG_TX, "cml_eth_send(%d) key=%08x, len=%d\n",
			 pifcml_eth->unit, (unsigned int)skb, total_len);
	if(ptxc->num_freetxd == 0)
	{ 
		cml_eth_txint(ptxc, 1);
		if(ptxc->num_freetxd == 0)
			diag_printf("E(%d,%d)",ptxc->tx_head,ptxc->tx_tail);
			//diag_printf("%s, ptxc->num_freetxd:%d\n", __func__, ptxc->num_freetxd);
	}
	if (ptxc->num_freetxd == 0 || total_len > 1518)
		goto drop_tx;

    /* EZP: 32MB fix */
    if( (virtophy(skb->data) + skb->len) >= 0x01000000 ) 
    {
           nskb = skb_copy(skb, GFP_KERNEL | GFP_DMA);
           if(!nskb)
               goto drop_tx;
           
           dev_kfree_skb(skb);
           skb = nskb;
    }

	ptxc->tx_pktcnt ++;

	cml_hnat_sdmz_tx_check(skb->data);
	cml_hnat_learn_mac(skb->data, 1, RX_SPORT(pifcml_eth->to_vlan));
	DC_FLUSH_TX((void *)skb->data, skb->len);

    dev->trans_start = jiffies;
	skb->dev = dev;
#ifdef TX_LEN_PADDING
		if (total_len < ETH_MIN_FRAMELEN)
		{
			/* 
			 * XXX:
			 */
			//memset( sg_list[sg_len-1].buf+sg_list[sg_len-1].len, 0 , ETH_MIN_FRAMELEN - total_len);
			sg_list[sg_len - 1].len += ETH_MIN_FRAMELEN - total_len;
			total_len = ETH_MIN_FRAMELEN;
		}
#endif

		ptxd_start = &ptxc->ptxd[ptxc->tx_head];

		/*  Use one buffers per descriptor  */
		ptxd_start->w1 = virtophy(skb->data) | (pifcml_eth->to_vlan);
		ptxd_start->w0 = (ptxd_start->w0 & DS0_END_OF_RING)
					| (skb->len<<DS0_LEN_S)
		 			| (DS0_TX_OWNBIT |DS0_DF);
		/*  Kick the hardware to start transmitting  */
		ETH_REG32(MTT) = 1;

		ptxc->ptxbuf[ptxc->tx_head].sc = dev;
		ptxc->ptxbuf[ptxc->tx_head].buf = (void *) skb;

		--ptxc->num_freetxd;
		if (++ptxc->tx_head == ptxc->num_txd)
			ptxc->tx_head = 0;
	
	return 0;

drop_tx:
	/*  No tx resource for this frame  */
	DBGPRINTF(DBG_TX, "cml_eth_send(%d) drop frame %08lx\n",
			 pifcml_eth->unit, (unsigned long)skb);
	/*  Drop the frame now  */ 
	ptxc->tx_drop++;
	return -EBUSY;
}
#endif //!__ECOS

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_recv:
 *=============================================================*/
#ifdef __ECOS
static void cml_eth_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list,
		int sg_len)
{
#if 0
	// SHOULD NOT BE HERE
	rxctrl_t *prxc = pcml_eth->prxc;	
	unsigned char *psrc;
	int frame_len, copylen, idx;
	struct eth_drv_sg *sg_last;
	if_cml_eth_t *pifcml_eth;
	register unsigned int w0, w1;
	
	if ( sg_len == 0 || sg_list == 0 )
	{
		/*  The frame will be dropped after the return.  */
		return;
	}

	idx = prxc->rx_head;
	prxd = &prxc->prxd[idx]
	
	frame_len = ((prxc->prxd[idx].w0<<5) >> (DS0_LEN_S+5)) ;
	pifcml_eth = pcml_eth->if_array[RX_SPORT(prxc->prxd[idx].w1)];
	
	CYG_ASSERTC(frame_len >= ETH_MIN_FRAMELEN);
	CYG_ASSERTC(pifcml_eth->sc == sc);

	DBGPRINTF(DBG_RX, "cml_eth_recv(%d), id=%d, len=%d\n", 
		pifcml_eth->unit, idx, frame_len);
	
	for(sg_last=&sg_list[sg_len]; sg_list != sg_last; sg_list++)
	{
		copylen = sg_list->len;
		if (copylen <=0 || sg_list->buf == 0)
		{
			DBGPRINTF(DBG_RXMORE, "cml_eth_recv(%d) out of buffer\n", 
				pifcml_eth->unit);
			/*  Caller was out of buffers  */
			return;
		}
		if (frame_len < copylen)
			copylen = frame_len;
		memcpy((void *) sg_list->buf, psrc, copylen);
		psrc += copylen;
		frame_len -= copylen;
	}
	//CYG_ASSERT( 0 == total_len, "total_len mismatch in rx" );
	CYG_ASSERT( sg_last == sg_list, "sg count mismatch in rx" );
#endif
}
#endif //__ECOS
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_deliver:
 *=============================================================*/
#ifdef __ECOS
static void cml_eth_deliver(struct eth_drv_sc *sc)
#else
static void cml_eth_deliver(struct net_device *sc)
#endif
{
	unsigned int int_status;
	
	int_status = ETH_REG32(MIS);
	/*  Ack all interrupts  */
	ETH_REG32(MIC) = int_status;

	DBGPRINTF(DBG_DELIVER, "cml_eth_deliver. istatus=%x\n",
			int_status);
	do {
#if 0
		if (int_status & MIS_SW)
		{
			cml_eth_swint();
		}
#endif		
		if (int_status & MIS_SQE)
		{
			//DBGPRINTF(DBG_INIT, "SQE\n\n");
			ETH_REG32(MAUTO)|=(1<<2);
		}
		if (int_status & MIS_HQE)
		{
			//DBGPRINTF(DBG_INIT, "HQE\n\n");
		}

		if (int_status & MIE_BE)
		{
			//DBGPRINTF(DBG_INIT, "BE\n\n");
			ETH_REG32(MAUTO)|=(1<<0);
		}
#if 1
		if (int_status & MIS_TX)
		{
			cml_eth_txint(&pcml_eth->tx, 0);
		}
#endif
		if (int_status & MIS_RX)
		{
			cml_eth_rxint(&pcml_eth->rx);
		}
	}while (0);
#ifdef	USED_DSR
	// in case of something pending to do,
	// re-schedule next tasklet to continue
	int_status=ETH_REG32(MIS);
	if (int_status & (MIM_INIT))
	{ // we re-schedule a tasklet later
		tasklet_hi_schedule(&tasklet);
	}
	else
#endif	
	ETH_REG32(MIM)&= ~MIM_INIT;
}
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_poll:
 *=============================================================*/
#ifdef __ECOS
static void cml_eth_poll(struct eth_drv_sc *sc)
{
	DBGPRINTF(DBG_DELIVER, "cml_eth_poll.\n");
	cml_eth_deliver(sc);
}
#endif //__ECOS
/// @endcond

/// @cond DOXYGEN
/*=============================================================*
 *  cml_eth_intvector:
 *=============================================================*/
#ifdef __ECOS
static int cml_eth_intvector(struct eth_drv_sc *sc)
{
	/*  Determine interrupt vector used by a device - for attaching GDB
	 *  stubs packet handler.
	 */
	return pcml_eth->vector;
}
#endif //__ECOS
/// @endcond

void cml_eth_flowctrl(int onoff)
{
	if(pcml_eth->flowctrl_mode == 0)
		return;
	if(onoff)
	{
		/* send pause frame */
		ETH_REG32(MFC1) = 0x2;
		pcml_eth->flowctrl_state = 1;
		diag_printf("send pause!!%x\n", ETH_REG32(MFC1));	
	}
	else if(pcml_eth->flowctrl_state)
	{
		/* send clear pause frame */
		ETH_REG32(MFC1) = 0x4;
		pcml_eth->flowctrl_state = 0;
		diag_printf("send clear pause!!\n");	
	}
}

#ifndef __ECOS
int eth_drv_recv1(struct net_device *dev, unsigned char *data, int len, char **nbufp, void *hw_nat)
{
 	struct sk_buff *skb;
	unsigned int * buf = *((unsigned int**)nbufp);
	int rc;
			
//printk("%s : buf=%x\n", __func__, (int)buf);
	skb = (struct sk_buff*) buf[BUF_SW_OFS/4];
//printk("%s : skb=%x\n", __func__, (int)skb);
#if 1 //DEBUG
    /* EZP: 32MB Fix
	if (0x80 != (((unsigned int)skb)>>24)) */
	if (0x08 != (((unsigned int)skb)>>28))
	{
				diag_printf("%s: skb is lost: %x\n",__func__, (unsigned int)skb);
				return -1;
	}
#endif
	memmove(data+4, data, 12); //remove VLAN
	skb->data = skb->tail = data +4;
	skb_put(skb,len);
#if 0
	printk("%s skb->data=%x , len=%x\n", __func__, skb->data, skb->len);
	{
		int j;
        for(j=0;j<(skb->len);j++)
        {
				if (0==(j&0x1f))
                	printk("\n%03x:", j);
                printk(" %02x",skb->data[j]&0xff);
        }
	}	
#endif
	skb->dev = dev;
	skb->protocol = eth_type_trans(skb, dev);    /* set packet type */
	rc = netif_rx(skb);
	//==NET_RX_DROP);                               /* send the packet up protocol stack */
#if 0
printk("\n%s rc=%x\n", __func__,rc);
#endif
	return rc;
}

char * eth_drv_getbuf()
{
		struct sk_buff *skb;
    /* EZP: 32MB Fix
		skb=dev_alloc_skb(1664); */
        skb = __dev_alloc_skb(1664, GFP_ATOMIC | GFP_DMA);
#if 0
		printk("skb->data=%x\n", (unsigned int)skb->data);
#endif
		skb->data = (unsigned char*)(((unsigned int)(skb->data+0x3f)) & ~0x3f);
#if 0
		printk("adjust data=%x\n", (unsigned int)skb->data);
#endif
		*((unsigned int*) skb->data) = (unsigned int)skb ;
		return skb->data;
}

void eth_drv_freebuf(char *buf)
{
		unsigned int *wbuf;
		struct sk_buff *skb;
#if 0
		printk("eth_drv_freebuf: buf=%x\n", (unsigned int)buf);
#endif
		wbuf = (unsigned int*)(((unsigned int)buf) & ~0x3f);
    /* EZP: 32MB Fix
		if (0x80 ==(wbuf[BUF_SW_OFS>>2]>>24)) */
		if (0x08 ==(wbuf[BUF_SW_OFS>>2]>>28))
		{
			skb = (struct sk_buff*)wbuf[BUF_SW_OFS>>2];
			kfree_skb(skb);
		}
		else
		{
			
			printk("%s , skb head missing:%x , buf=%x\n", __func__, wbuf[BUF_SW_OFS>>2], (unsigned int)buf);		
		}
}

static int __init cml_eth_init_module (void)
{
    static int probed = 0;
    int rc;
    int i;

    if( probed )
        return -ENODEV;
    probed = 1;

	printk(KERN_INFO "Camelot Ether driver version %s\n", cml_eth_ethdrv_version);

    for (i=0;i<ETH_DEV_INIT; i++)
    {
        rc=cml_eth_drv_init(i);
        if (rc)
        	break;
    }

    return 0;
}

static void __exit cml_eth_cleanup_module (void)
{
    int i;
    struct net_device * dev;

	printk(KERN_INFO "Camelot Ether driver unloaded\n");
    for (i=0;i< ETH_DEV_INIT; i++)
    {
		if (if_cml_eth_g[i]==0)
			continue;
        if (0==(dev=if_cml_eth_g[i]->sc))
			continue;
        
		if ( pcml_eth->sc_main==dev )
           	free_irq(dev->irq, dev );
		if (dev->priv)
			kfree(dev->priv);
		unregister_netdev(dev);
		kfree(dev);
		kfree(if_cml_eth_g[i]);
		if_cml_eth_g[i]=0;	
    }
}

module_init(cml_eth_init_module);
module_exit(cml_eth_cleanup_module);


#endif //!__ECOS
