#if defined(MODVERSIONS)
#include <linux/modversions.h>
#endif
#include <linux/autoconf.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <asm/dma.h>

#include "ip175d.h"

#define DRV_NAME	"ip175d"
#define DRV_VERSION	"1.03"
#define DRV_RELDATE	"2008-12-10"

static struct net_device* pNet_dev=NULL;

#define CAMELOT
void cml_eth_mdio(short phy,short reg,unsigned short *pval,int wr);

//#define TEST_REG
#ifdef TEST_REG
u16 RegList[32][32];
#endif

#define Write_Reg(phy,reg,val)	phy_write(phy,reg,val)	//modify this to MII interface
#define Read_Reg(phy,reg)	phy_read(phy,reg)			//modify this to MII interface
void phy_write(int phyaddr, int regaddr, int value)		//modify this to MII interface
{  
#ifdef TEST_REG
	RegList[phyaddr][regaddr]=value; 
#endif
#ifdef	CAMELOT
	unsigned short val=(unsigned short)value; 
	cml_eth_mdio((short)phyaddr,(short)regaddr,&val,1);
#endif
}
u16 phy_read(int phyaddr, int regaddr)					//modify this to MII interface
{
#ifdef TEST_REG
	return RegList[phyaddr][regaddr];
#endif
#ifdef	CAMELOT
	unsigned short val;
	cml_eth_mdio((short)phyaddr,(short)regaddr,&val,0);
	return val;
#endif
	return 0; 
}


void VLAN_Set( struct _VLAN_setting *VLAN_setting )
{	u16 Valid=0x0,FVID=0x0,Reg_Val,reg_setting;
	int i,len=PBASE_VLAN_NUM;
//	printk("\nVLAN Mode=%d",VLAN_setting->cmd);
	if(VLAN_setting->cmd&VLAN_CMD_TBASE){len=TBASE_VLAN_NUM;}

	Reg_Val=Read_Reg(PHY22,VLAN_CLASS);
	if(VLAN_setting->vmode)
	{	Reg_Val |= 0x003f;	}
	else
	{	Reg_Val &=~0x003f;	}
	Write_Reg(PHY22,VLAN_CLASS,Reg_Val);	//set vlan mode

	for(i=0;i<len;i++)
	{
		if(VLAN_setting->VLAN_entry[i].valid)
		{ Valid |= (0x0001<<i); }
		FVID = (VLAN_setting->VLAN_entry[i].VID&0x0fff);
		FVID|=((VLAN_setting->VLAN_entry[i].FID&0x000f)<<12);
		Write_Reg(PHY22, VLAN_FID_VID0+i ,FVID); 	//set FID&VID

		reg_setting  =VLAN_setting->VLAN_entry[i].member;
		reg_setting &=0x003f;
		reg_setting<<=(8*(i%2));
		Reg_Val = ( Read_Reg(PHY23,VLAN_MEMBER_00+i/2) );
		Reg_Val&= ~(0x00ff<< (8*(i%2)) );
		Reg_Val|=reg_setting;
		Write_Reg(PHY23,VLAN_MEMBER_00+i/2,Reg_Val) ;	//set VLAN member

		reg_setting  =VLAN_setting->VLAN_entry[i].AddTag;
		reg_setting &=0x003f;
		reg_setting<<=(8*(i%2));
		Reg_Val = ( Read_Reg(PHY23,VLAN_ADDTAG_00+i/2) );
		Reg_Val&= ~(0x00ff<< (8*(i%2)) );
		Reg_Val|=reg_setting;
		Write_Reg(PHY23,VLAN_ADDTAG_00+i/2,Reg_Val) ;	//set AddTag

		reg_setting  =VLAN_setting->VLAN_entry[i].RmvTag;
		reg_setting &=0x003f;
		reg_setting<<=(8*(i%2));
		Reg_Val = ( Read_Reg(PHY23,VLAN_REMOVETAG_00+i/2) );
		Reg_Val&= ~(0x00ff<< (8*(i%2)) );
		Reg_Val|=reg_setting;
		Write_Reg(PHY23,VLAN_REMOVETAG_00+i/2,Reg_Val);	//set RemoveTag

		if(i<PBASE_VLAN_NUM)
		{	Reg_Val=(VLAN_setting->VLAN_entry[i].PVID & 0x0fff);
			Write_Reg(PHY22,VLAN_INFO_0+i, Reg_Val );
		}						//set PVID
	}
	Write_Reg(PHY22,VLAN_VALID,Valid);			//set valid

	VLAN_setting->cmd &= VLAN_CMD_OKMASK;
	VLAN_setting->cmd |= VLAN_CMD_OK;	//command complete
}

void VLAN_Query( struct _VLAN_setting *VLAN_setting )
{	u16 Valid=0x0,FVID=0x0,Reg_Val;
	int i,len=PBASE_VLAN_NUM;
	if(VLAN_setting->cmd&VLAN_CMD_TBASE){len=TBASE_VLAN_NUM;}
	VLAN_setting->vmode=( Read_Reg(PHY22,VLAN_CLASS)&0x3f );
	for(i=0;i<len;i++)
	{	Valid=Read_Reg(PHY22,VLAN_VALID);
		if(Valid & (0x0001<<i))
		{ VLAN_setting->VLAN_entry[i].valid=1; }	//query valid port

		FVID=Read_Reg(PHY22, VLAN_FID_VID0+i);
		VLAN_setting->VLAN_entry[i].VID= (FVID&0x0fff);	//query VID
		VLAN_setting->VLAN_entry[i].FID=((FVID&0xf000)>>12);//query FID

		Reg_Val = ( Read_Reg(PHY23,VLAN_MEMBER_00+i/2) >> (8*(i%2)) );
		Reg_Val&= 0x00ff;
		VLAN_setting->VLAN_entry[i].member = Reg_Val;	//query VLAM member

		Reg_Val = ( Read_Reg(PHY23,VLAN_ADDTAG_00+i/2) >> (8*(i%2)) );
		Reg_Val&= 0x00ff;
		VLAN_setting->VLAN_entry[i].AddTag = Reg_Val;	//query add tag

		Reg_Val = ( Read_Reg(PHY23,VLAN_REMOVETAG_00+i/2) >> (8*(i%2)) );
		Reg_Val&= 0x00ff;
		VLAN_setting->VLAN_entry[i].RmvTag = Reg_Val;	//query remove tag

		Reg_Val = Read_Reg(PHY22,VLAN_INFO_0);
		Reg_Val&= 0x0fff;				//query PVID 
		if(i<PBASE_VLAN_NUM)				//(only 6 entries)
		{	VLAN_setting->VLAN_entry[i].PVID  = Reg_Val;	}
		else
		{	VLAN_setting->VLAN_entry[i].PVID  = 0x0;	}
	}

	VLAN_setting->cmd &= VLAN_CMD_OKMASK;
	VLAN_setting->cmd |= VLAN_CMD_OK;	//command complete
}

#define exchange(a,b)	{		  \
							u16 t;\
							t = a;\
							a = b;\
							b = t;\
						}

void QoS_Set(struct _QoS_setting *QoS_setting)
{	u16 Reg2522=0,Reg2523=0,Reg2511=0,Reg2520=0;
	u16 i;
	
	if(!(QoS_setting->enable))	//disable QoS function
	{	Write_Reg(PHY25,QOS_CTRL,0x0);	return;	}
	else
	{	Write_Reg(PHY25,QOS_CTRL,0xff80);		}

	QoS_setting->mode&=0x3;
//printk(KERN_ERR"\nQoS mode:%01x",QoS_setting->mode);
	Reg2522=0x2000;
	for(i=0;i<6;i++)
	{	Reg2522|= ( QoS_setting->mode<<(2*i) );	}

	for(i=0;i<QoS_MAX_QUEUE;i++)
	{	QoS_setting->queue_weight[i]&=0xf;
		Reg2523|= ( QoS_setting->queue_weight[i]<<(4*i) );
	}

	QoS_setting->type&=0x3;
	Reg2511|=(QoS_setting->type<<6);
	for(i=0;i<MAX_PROTOCOL_NUM;i++)
	{	if( QoS_setting->protocol[i].enable )
		{	Reg2511|= (0x01<<(0+i));		}
	}
	for(i=0;i<MAX_RANGE_NUM;i++)
	{	if( QoS_setting->range[i].enable )
		{	Reg2511|= (0x01<<(4+i));		}
	}

	for(i=0;i<MAX_PROTOCOL_NUM;i++)
	{	Write_Reg(PHY25,QOS_PRE_LOGI_0+i, QoS_setting->protocol[i].port );	}
	for(i=0;i<MAX_RANGE_NUM;i++)
	{	if(QoS_setting->range[i].s_port>QoS_setting->range[i].e_port)
		{ exchange(QoS_setting->range[i].s_port,QoS_setting->range[i].e_port); }
		Write_Reg(PHY25,QOS_USER_RG_LOW_0+(2*i),QoS_setting->range[i].s_port);
		Write_Reg(PHY25,QOS_USER_RG_HI_0 +(2*i),QoS_setting->range[i].e_port);
	}
	
	for(i=0;i<MAX_PROTOCOL_NUM;i++)
	{	QoS_setting->protocol[i].queue&=0x3;
		Reg2520|= ( QoS_setting->protocol[i].queue<<(0+i*2) );
	}
	for(i=0;i<MAX_RANGE_NUM;i++)
	{	QoS_setting->range[i].queue&=0x3;
		Reg2520|= ( QoS_setting->range[i].queue<<(8+i*2) );
	}

//printk(KERN_ERR" reg2522:%04x",Reg2522);
	Write_Reg(PHY25,QOS_SCH_CONFIG	,Reg2522);
	Write_Reg(PHY25,QOS_SCH_WEIGHT	,Reg2523);
	Write_Reg(PHY25,QOS_TCP_CTRL	,Reg2511);
	Write_Reg(PHY25,QOS_TCP_QU_MAP	,Reg2520);
}

void QoS_Query(struct _QoS_setting *QoS_setting)
{}

void Mirror_Set(struct _Mirror_setting *Mirror_setting)
{	u16 Reg2020=Read_Reg(PHY20,PORT_MIRROR_0),Reg2021=0;
	if(!(Mirror_setting->enable))	//disable Mirror function
	{	Reg2020 &= ~0x8000;
		Write_Reg(PHY20,PORT_MIRROR_0,Reg2020);
		return;
	}
	else
	{	Reg2020 = 0x8000;
	}
	
	Mirror_setting->mode&=0x3;
	Reg2020 |= (Mirror_setting->mode<<13);

	Mirror_setting->rx_from%=MAX_MIRROR_PORT;	
	Reg2020 |= (0x01<<Mirror_setting->rx_from);

	Mirror_setting->tx_from%=MAX_MIRROR_PORT;	
	Reg2021 |= (0x01<<Mirror_setting->tx_from);

	Mirror_setting->to%=MAX_MIRROR_PORT;	
	Reg2021 |= (Mirror_setting->to << 12);

	Write_Reg(PHY20,PORT_MIRROR_0,Reg2020);
	Write_Reg(PHY20,PORT_MIRROR_1,Reg2021);
}

void Mirror_Query(struct _Mirror_setting *Mirror_setting)
{	u16 Reg2020=Read_Reg(PHY20,PORT_MIRROR_0);
	u16 Reg2021=Read_Reg(PHY20,PORT_MIRROR_1);
	u8  i;
	if(Reg2020 & 0x8000)
	{	Mirror_setting->enable=1;	}
	
	Mirror_setting->mode=  ((Reg2020>>13)&0x3) ;

	for(i=0;i<MAX_MIRROR_PORT;i++)
	{
		if( Reg2020 & (0x01<<i) )
		{ Mirror_setting->rx_from=i; }
		if( Reg2021 & (0x01<<i) )
		{ Mirror_setting->tx_from=i; }
	}
	
	Mirror_setting->to = ((Reg2021>>12)&0x7);

}

void Datarate_Set(struct _Datarate_setting *Datarate_setting)
{	u16 Reg_cmd,Reg_BW_MBS=0xf000,i;
	
	for(i=0;i<MAX_PORT_NUM;i++)
	{
		if(Datarate_setting->tx_rate[i]>DATARATE_MAX_RATE) 
		{Datarate_setting->tx_rate[i]=0;}
		Write_Reg(PHY21, BW_TI, 0x0001);
		Write_Reg(PHY21, BW_CREDIT_SIZE, Datarate_setting->tx_rate[i]*(32/8));//32/8bit
		Write_Reg(PHY21, BW_MBS, Reg_BW_MBS);
		Reg_cmd= (0x308|i) ; // ( start | write | tx | port )
		Write_Reg(PHY21, BW_SETTING, Reg_cmd);

		if(Datarate_setting->rx_rate[i]>DATARATE_MAX_RATE) 
		{Datarate_setting->rx_rate[i]=0;}
		Write_Reg(PHY21, BW_TI, 0x0001);
		Write_Reg(PHY21, BW_CREDIT_SIZE, Datarate_setting->rx_rate[i]*(32/8));//32/8bit
		Write_Reg(PHY21, BW_MBS, Reg_BW_MBS);
		Reg_cmd= (0x300|i) ; // ( start | write | rx | port )
		Write_Reg(PHY21, BW_SETTING, Reg_cmd);
	}
}

void Datarate_Query(struct _Datarate_setting *Datarate_setting)
{	u16 Reg_cmd,Reg_CREDIT,i;
	for(i=0;i<MAX_PORT_NUM;i++)
	{
		Reg_cmd= (0x208|i) ; // ( start | read | tx | port )
		Write_Reg(PHY21, BW_SETTING, Reg_cmd);		
		Reg_CREDIT=Read_Reg(PHY21, BW_CREDIT_SIZE);
		Datarate_setting->tx_rate[i]= ( Reg_CREDIT/(32/8) );
		
		Reg_cmd= (0x200|i) ; // ( start | read | tx | port )
		Write_Reg(PHY21, BW_SETTING, Reg_cmd);		
		Reg_CREDIT=Read_Reg(PHY21, BW_CREDIT_SIZE);
		Datarate_setting->rx_rate[i]= ( Reg_CREDIT/(32/8) );
	}
}

void case1_VOIP(void)
{
	Write_Reg(PHY22,VLAN_CLASS 		,0x8000);//
	Write_Reg(PHY21,3 			,0x9cf0);//
	Write_Reg(PHY20,4 			,0xa000);////clear VLAN setting

	Write_Reg(PHY22,VLAN_CLASS 		,0x08a3);//
	Write_Reg(PHY22,VLAN_INFO_1 		,0x0014);//
	Write_Reg(PHY22,VLAN_INFO_5 		,0x000a);//
	Write_Reg(PHY22,VLAN_VALID		,0x0003);//
	Write_Reg(PHY22,VLAN_FID_VID0		,0x000a);//
	Write_Reg(PHY22,VLAN_FID_VID1		,0x0014);//
	Write_Reg(PHY23,VLAN_MEMBER_00 		,0x0321);//
	Write_Reg(PHY23,VLAN_ADDTAG_00 		,0x0101);//
	Write_Reg(PHY23,VLAN_REMOVETAG_00	,0x0220);//
}
void case2_NAT(void)
{
	Write_Reg(PHY22,VLAN_CLASS 			,0x8000);//
	Write_Reg(PHY21,3 					,0x8ef0);//
	Write_Reg(PHY20,4 					,0xf000);////clear VLAN setting

	Write_Reg(PHY20,LEARNING_CTRL_REG	,0x0021);//
	Write_Reg(PHY22,VLAN_CLASS 			,0x0cf3);//
	Write_Reg(PHY22,VLAN_INFO_0 		,0x0001);//

	Write_Reg(PHY22,VLAN_INFO_1 		,0x0002);//
	Write_Reg(PHY22,VLAN_INFO_4 		,0x0002);//
	Write_Reg(PHY22,VLAN_INFO_5 		,0x0001);//

	Write_Reg(PHY22,VLAN_VALID			,0x0003);//
	Write_Reg(PHY22,VLAN_FID_VID0		,0x1001);//
	Write_Reg(PHY22,VLAN_FID_VID1		,0x2002);//
	Write_Reg(PHY23,VLAN_MEMBER_00 		,0x1221);//
}

//please modify for each application
void case_ACL(void)
{	
	Write_Reg(PHY25,QOS_CTRL 		,0xff80);// enable CoS and User define priority
	Write_Reg(PHY25,QOS_SCH_CONFIG	,0x2fff);// enable all range[5:4]+ portocol[3:0] =1
	                                 // both source and dest port take effecting [7:6]
	Write_Reg(PHY25,QOS_TCP_CTRL,0x00ff);// protocol-0~3 and range0~1 enable
	Write_Reg(PHY25,QOS_PRE_LOGI_0,22) ;	// protocol-0 set SSH   (port 22)
	Write_Reg(PHY25,QOS_PRE_LOGI_1,443);	// protocol-1 set HTTPs (port 443)
	Write_Reg(PHY25,QOS_PRE_LOGI_2,3389);	// protocol-2 set RDP   (port 3389)
	Write_Reg(PHY25,QOS_PRE_LOGI_3,6000);	// protocol-3 set XWIN  (port 6000)
	Write_Reg(PHY25,QOS_USER_RG_LOW_0,23);	// range-0 set from 23
	Write_Reg(PHY25,QOS_USER_RG_HI_0 ,23);	// ranfe-0 set to   23  (only for TELNET)
	Write_Reg(PHY25,QOS_USER_RG_LOW_1,5800);// range-1 set from 5800
	Write_Reg(PHY25,QOS_USER_RG_HI_1,5800);	// range-1 set to   2500(only for VNC)
	Write_Reg(PHY25,QOS_TCP_QU_MAP,0x0fff);	// set protocol 0~3 and range 0~1 to queue3
}


static int os_do_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
//	struct mii_ioctl_data *mii = (struct mii_ioctl_data*)&ifr->ifr_data;	
//	printk(KERN_ERR "\n IP175d IOCTL called! (%08x)\n",cmd);
	switch(cmd)
	{
	case	SIOCSMIIREG:	//WriteMDC
		{
		struct mii_ioctl_data *mii = (struct mii_ioctl_data*)&ifr->ifr_data;	
		//printk(KERN_ERR"Phy:%d Reg:%d D:%04x",mii->phy_id,mii->reg_num,mii->val_in);
		Write_Reg(mii->phy_id,mii->reg_num,mii->val_in);
		}
		return 0;
		break;
	case	SIOCGMIIREG:	//ReadMDC
		{
		struct mii_ioctl_data *mii = (struct mii_ioctl_data*)&ifr->ifr_data;	
		//printk(KERN_ERR"Phy:%d Reg:%d",mii->phy_id,mii->reg_num);
		mii->val_out=Read_Reg(mii->phy_id,mii->reg_num);
		//printk(KERN_ERR"D:%04x",mii->val_out);
		}
		return 0;
		break;
	case	SIOCVLAN:	//VLAN Setting
		{
		struct _VLAN_setting *VLAN_setting=ifr->ifr_data;
		if(VLAN_setting->cmd&VLAN_CMD_WRITE)	//write
		 {	VLAN_Set(VLAN_setting);		 }
		else
		 {	VLAN_Query(VLAN_setting);	 }

		}
		return 0;
	case	SIOCQOS:	//QoS Setting
		{
		struct _QoS_setting *QoS_setting=ifr->ifr_data;
//		printk(KERN_ERR" QoS(0x%04x) called!!",SIOCQOS);
		if(QoS_setting->cmd&QoS_CMD_WRITE)	//write
		 {	QoS_Set(QoS_setting);		 }
		else
		 {	/*QoS_Query(QoS_setting);	*/		 }
		}
		return 0;
	case	SIOCMIRROR:	//Mirror Setting
		{
		struct _Mirror_setting *Mirror_setting=ifr->ifr_data;
		if(Mirror_setting->cmd&MIRROR_CMD_WRITE)	//write
		 {	Mirror_Set(Mirror_setting);	 }
		else
		 {	Mirror_Query(Mirror_setting);	 }
		}
		return 0;
	case	SIOCDATARATE://Datarate Setting
		{
		struct _Datarate_setting *Datarate_setting=ifr->ifr_data;
		if(Datarate_setting->cmd&DATARATE_CMD_WRITE)	//write
		 {	Datarate_Set(Datarate_setting);	 }
		else
		 {	Datarate_Query(Datarate_setting);	 }
		}
		return 0;

	case	SIOCACLSET:	//ACL setting
		case_ACL();
		return 0;
	case	SIOCVOIPSET:	//VOIP setting
		case1_VOIP();
		return 0;
	case	SIOCNATSET:	//NAT setting
		case2_NAT();
		return 0;
	};
	return -EOPNOTSUPP;
}


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
static int linux_probe(struct net_device *dev)
#else
struct net_device * __init linux_probe(void)
#endif
{	u8 MACID[6]={0x00,0x00,0x00,0x00,0x17,0x5d};
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
	struct net_device* dev; 
	u16 register_err;
#endif


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
	dev=init_etherdev(dev,0);	//get device name eth0
#else
	dev= alloc_etherdev(sizeof(*dev));	//get device name eth0
#endif
	if(!dev)
	{	printk("\ndriver allocation fail");
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
		return -ENOMEM;
#else
		return ERR_PTR(-ENOMEM);
#endif
	}
	pNet_dev=dev;
	strcpy(dev->name,DRV_NAME);
	dev->do_ioctl		= &os_do_ioctl;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	SET_MODULE_OWNER(dev);
#endif
	memcpy(dev->dev_addr,MACID,sizeof(MACID));

	printk("\nModule load!");
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
	return 0;
#else
	register_err=register_netdev(dev);
	if( register_err )	//register netdevice fail
	{	printk("\nregister netdev fail");
		//release_region(dev->base_addr,2);
		free_netdev(dev);
		return(ERR_PTR(register_err));
	}
	return dev;
#endif
}
int __init ip175d_init_module(void)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
	return linux_probe(0);
#else
	linux_probe();
	return 0;
#endif
}
void __exit ip175d_cleanup_module(void)
{	
	printk(KERN_ERR "\nModule");
//	printk("\nModule(%s)",pNet_dev->name);
	unregister_netdev(pNet_dev);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
	kfree(pNet_dev);
#else
	free_netdev(pNet_dev);
#endif
	printk(KERN_ERR " removed!\n");
}

#define CAMELOT
#ifdef CAMELOT
module_init(ip175d_init_module);
module_exit(ip175d_cleanup_module);
#endif
