/*=============================================================================+
|                                                                              |
| Copyright 2008                                                               |
| Acrospeed Inc. All right reserved.                                           |
|                                                                              |
+=============================================================================*/
/*! 
*   \file nf_hnat_sdmz.c
*   \brief  netfilter hook for Camelot hnat
*   \author Acrospeed
*/

#include <linux/autoconf.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
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

#define ARP_REQUEST	1
#define ARP_REPLY	2

#define IPA(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|d)
extern unsigned int nf_hnat_wan_ip;
unsigned int cml_hnat_sdmz_fakeip=IPA(192,168,0,254);
char cml_hnat_sdmz_mac[6]={0x00,0x0e,0x2e,0x3e,0x9e,0x09};

struct iphdr
{
#if 1
  unsigned char version:4, ihl:4;
#else
  unsigned char ihl:4, version:4;
#endif
  char service;
  unsigned short len;
  unsigned short ident;
  unsigned short frags;
  char ttl;
  char protocol;
  unsigned short chksum;
  unsigned long src;
  unsigned long dst;
} __attribute__ ((aligned(2), packed));

struct tcphdr
{
  unsigned short src;
  unsigned short dst;
  unsigned int seqno;
  unsigned int ackno;
  unsigned char tcpoffset;
  unsigned char flags;
  unsigned short window;
  unsigned short chksum;
  unsigned short urgp;
} __attribute__ ((aligned(2), packed));

struct udphdr {
	unsigned short src;		/* source port */
	unsigned short dst;		/* destination port */
	unsigned short len;		/* udp length */
	unsigned short chksum;		/* udp checksum */
} __attribute__ ((aligned(2), packed)) ;

struct icmphdr
{
  unsigned char type;		/* ICMP type field */
  unsigned char code;		/* ICMP code field */
  unsigned short chksum;	/* checksum */
  unsigned short part1;		/* depends on type and code */
  unsigned short part2;
};

struct arprequest
{
  unsigned short hwtype;
  unsigned short protocol;
  char hwlen;
  char protolen;
  unsigned short opcode;
  char shwaddr[6];
  char sipaddr[4];
  char thwaddr[6];
  char tipaddr[4];
};

#define FIX_CHECKSUM(diff, cksum) { \
	diff += (cksum); \
	if (diff < 0) \
	{ \
		diff = -diff; \
		diff = (diff >> 16) + (diff & 0xffff); \
		diff += diff >> 16; \
		(cksum) = (u_short) ~diff; \
	} \
	else \
	{ \
		diff = (diff >> 16) + (diff & 0xffff); \
		diff += diff >> 16; \
		(cksum) = (u_short) diff; \
	} \
}

extern cml_eth_t *pcml_eth;
unsigned short ip_cksum(unsigned short *p, unsigned int len);

/*!-----------------------------------------------------------------------------
 * function: cml_hnat_sdmz_rx_check
 *
 *      \brief 	response the arp request (arp proxy)
 *      		translate super DMZ ip (wan ip) to dmz ip
 *		\param 	
 *      \return 
 +----------------------------------------------------------------------------*/
int cml_hnat_sdmz_rx_check(if_cml_eth_t *pifcml_eth, char *buf, char *pkt, unsigned int w0, unsigned int w1, int len)
{
		unsigned short ether_type = *((unsigned short*)(pkt+16));
		unsigned int *wbuf=(unsigned int *)buf;
		txdesc_t *ptxd;

		#if 0
		printk("----ether_type=%d---\n",ether_type);
		{
			int i;
			for(i=0;i<20;i++)
				printk("[%02x]",pkt[i]);
			printk("\n");	
			
		}	
		#endif	
		if (ether_type ==  0x0806)
	    {
	      	struct arprequest *arpreply = (struct arprequest *) &pkt[18];
	      	
	      	if (arpreply->opcode == htons (ARP_REQUEST) && !memcmp(&pkt[6],cml_hnat_sdmz_mac,6) && 
	      	(*(int *)(arpreply->sipaddr) == nf_hnat_wan_ip))
	      	{
	      		unsigned long tmp;
	      		struct sk_buff *skb;
	  
	      		// query for me 
	      		memcpy (&tmp, arpreply->tipaddr, sizeof (int));
	      		if ((tmp & 0xffffff00)!= (ETH_REG32(MSIP)&0xffffff00) && (tmp != nf_hnat_wan_ip))
				{
					txctrl_t *ptxc = &pcml_eth->tx;

					memcpy(&pkt[4], arpreply->shwaddr, 6);
					memcpy(&pkt[10], pifcml_eth->macaddr, 6);
					pkt[16] = 0x08;
					pkt[17] = 0x06;
		  			arpreply->opcode = htons (ARP_REPLY);
		  			memcpy (arpreply->tipaddr,
				  	arpreply->sipaddr, sizeof (int));
		  			memcpy (arpreply->thwaddr, arpreply->shwaddr, 6);
		  			memcpy (arpreply->sipaddr,
			  			&tmp, sizeof (int));
		  			memcpy (arpreply->shwaddr,
			  			pifcml_eth->macaddr, 6);
					skb = (struct sk_buff*) wbuf[BUF_SW_OFS/4];

					if (ptxc->num_freetxd <= 0)
					{
						kfree_skb(skb);
						return 1;
					}

					#if 0
					{
						int i;
						for(i=0;i<40;i++)
							printk("[%02x]",pkt[i+4]);
						printk("\n");	
					}	
					#endif				
					HAL_DCACHE_FLUSH(pkt+4,len);	
					ptxc->tx_pktcnt++;
					ptxc->tx_ubuf++;
					ptxc->num_freetxd--;
					/*  Fill the Tx descriptor  */
					ptxd = &ptxc->ptxd[ptxc->tx_head];
					ptxd->w1 = virtophy(pkt+4)| (pifcml_eth->to_vlan);
					ptxd->w0 = (ptxd->w0 & DS0_END_OF_RING)|
						(len << DS0_LEN_S) | DS0_TX_OWNBIT | DS0_DF;
					ptxc->ptxbuf[ptxc->tx_head].buf = (void *)skb;
					ptxc->ptxbuf[ptxc->tx_head].sc = 0;

					if (++ptxc->tx_head == ptxc->num_txd)
						ptxc->tx_head = 0;
					/*  Kick the hardware to start transmitting  */
					ETH_REG32(MTT) = 1;
					return 1;
		  		}
		 	}
	    }
		
		
		if(!(w0 & DS0_INB) && !memcmp(cml_hnat_sdmz_mac,pkt+6,6))
		{
			int diff, hlen;
			struct iphdr *ip;
			struct tcphdr *tcp;
			struct udphdr *udp;
			struct icmphdr *icmp;

			ip = (struct iphdr *)(buf + BUF_IPOFF + BUF_HW_OFS);
			
			if(ip->src!=nf_hnat_wan_ip)
				return 0;
			hlen = ip->ihl << 2;
			tcp = (struct tcphdr *)((char *)ip + hlen);
			diff = (ip->src >> 16);
			diff += (ip->src & 0x0ffff);
			ip->src = cml_hnat_sdmz_fakeip;
			diff -= (cml_hnat_sdmz_fakeip>>16);
			diff -= (cml_hnat_sdmz_fakeip & 0x0ffff);
			if((ip->protocol) == 0x6)
			{
				FIX_CHECKSUM(diff, tcp->chksum);
			}	
			else if((ip->protocol) == 0x11)
			{
				udp = (struct udphdr *)tcp;
				FIX_CHECKSUM(diff, udp->chksum);
			}
			else if((ip->protocol) == 0x01)
			{
				//printk("===icmp====\n");
				icmp = (struct icmphdr *)tcp;
				//FIX_CHECKSUM(diff, icmp->chksum);
			}	
			ip->chksum = 0;
			ip->chksum = ip_cksum((unsigned short *)ip, hlen);
		}	
		return 0;
}

/*!-----------------------------------------------------------------------------
 * function: cml_hnat_sdmz_tx_check
 *
 *      \brief translate the dmz host ip to super DMZ ip (wan ip)
 *		\param phy: 
 *      \return 
 +----------------------------------------------------------------------------*/
void cml_hnat_sdmz_tx_check(char *hwadrp)
{
		struct iphdr *ip;
		
		ip = (struct iphdr *)(hwadrp + 14);

		#if 0
		{
			int i;
			for(i=0;i<20;i++)
				printk("[%02x]",hwadrp[i+14]);
			printk("\n");	
			
		}	
		#endif
		if(ip->dst==cml_hnat_sdmz_fakeip)
		{
			int diff, hlen;
			struct tcphdr *tcp;
			struct udphdr *udp;
			struct icmphdr *icmp;
			unsigned int dst = nf_hnat_wan_ip ;

			hlen = ip->ihl << 2;
			tcp = (struct tcphdr *)((char *)ip + hlen);
			
			diff = (ip->dst >> 16);
			diff += (ip->dst & 0x0ffff);
			ip->dst = dst;
			diff -= (dst>>16);
			diff -= (dst & 0x0ffff);
			if((ip->protocol) == 0x6)
			{
				FIX_CHECKSUM(diff, tcp->chksum);
			}	
			else if((ip->protocol) == 0x11)
			{
				udp = (struct udphdr *)tcp;
				FIX_CHECKSUM(diff, udp->chksum);
			}	
			else if((ip->protocol) == 0x01)
			{
				icmp = (struct icmphdr *)tcp;
				//FIX_CHECKSUM(diff, icmp->chksum);
			}	
			ip->chksum = 0;
			ip->chksum = ip_cksum((unsigned short *)ip, hlen);
		}	
}
