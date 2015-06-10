#include <linux/module.h>
#include <linux/version.h>
#include <linux/netdevice.h>

#include <linux/kernel.h>
#include <linux/sched.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
#include <asm/rt2880/rt_mmap.h>
#endif

#include "ra2882ethreg.h"
#include "raether.h"


#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || defined (CONFIG_RALINK_RT5350) 
#define PHY_CONTROL_0 		0xC0   
#define PHY_CONTROL_1 		0xC4   
#define MDIO_PHY_CONTROL_0	(RALINK_ETH_SW_BASE + PHY_CONTROL_0)
#define MDIO_PHY_CONTROL_1 	(RALINK_ETH_SW_BASE + PHY_CONTROL_1)

#define GPIO_MDIO_BIT		(1<<7)
#define GPIO_PURPOSE_SELECT	0x60
#define GPIO_PRUPOSE		(RALINK_SYSCTL_BASE + GPIO_PURPOSE_SELECT)

#elif defined (CONFIG_RALINK_RT6855)  || defined (CONFIG_RALINK_RT6855A)

#define PHY_CONTROL_0 		0x7004   
#define MDIO_PHY_CONTROL_0	(RALINK_ETH_SW_BASE + PHY_CONTROL_0)

#define GPIO_MDIO_BIT		(1<<7)
#define GPIO_PURPOSE_SELECT	0x60
#define GPIO_PRUPOSE		(RALINK_SYSCTL_BASE + GPIO_PURPOSE_SELECT)

#elif defined (CONFIG_RALINK_MT7620)

#define PHY_CONTROL_0 		0x7004   
#define MDIO_PHY_CONTROL_0	(RALINK_ETH_SW_BASE + PHY_CONTROL_0)

#define GPIO_MDIO_BIT		(2<<7)
#define GPIO_PURPOSE_SELECT	0x60
#define GPIO_PRUPOSE		(RALINK_SYSCTL_BASE + GPIO_PURPOSE_SELECT)

#elif defined (CONFIG_RALINK_MT7621)

#define PHY_CONTROL_0 		0x0004   
#define MDIO_PHY_CONTROL_0	(RALINK_ETH_SW_BASE + PHY_CONTROL_0)

#define GPIO_MDIO_BIT		(2<<7)
#define GPIO_PURPOSE_SELECT	0x60
#define GPIO_PRUPOSE		(RALINK_SYSCTL_BASE + GPIO_PURPOSE_SELECT)

#else 
#define PHY_CONTROL_0       	0x00
#define PHY_CONTROL_1       	0x04
#define MDIO_PHY_CONTROL_0	(RALINK_FRAME_ENGINE_BASE + PHY_CONTROL_0)
#define MDIO_PHY_CONTROL_1	(RALINK_FRAME_ENGINE_BASE + PHY_CONTROL_1)
#define enable_mdio(x)
#endif

#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || defined (CONFIG_RALINK_RT5350) || \
    defined (CONFIG_RALINK_RT6855) || defined (CONFIG_RALINK_MT7620) || defined (CONFIG_RALINK_MT7621)
void enable_mdio(int enable)
{
#if !defined (CONFIG_P5_MAC_TO_PHY_MODE) && !defined(CONFIG_GE1_RGMII_AN) && !defined(CONFIG_GE2_RGMII_AN) && \
    !defined (CONFIG_GE1_MII_AN) && !defined (CONFIG_GE2_MII_AN)
	u32 data = sysRegRead(GPIO_PRUPOSE);
	if (enable)
		data &= ~GPIO_MDIO_BIT;
	else
		data |= GPIO_MDIO_BIT;
	sysRegWrite(GPIO_PRUPOSE, data);
#endif
}
#elif defined (CONFIG_RALINK_RT6855A)
void enable_mdio(int enable)
{
	/*need to check RT6855A MII/GPIO pin share scheme*/
}
#endif

#if defined (CONFIG_RALINK_RT6855) || defined (CONFIG_RALINK_RT6855A) || \
    defined (CONFIG_RALINK_MT7620) || defined (CONFIG_RALINK_MT7621)

u32 mii_mgr_read(u32 phy_addr, u32 phy_register, u32 *read_data)
{
	u32 volatile status = 0;
	u32 rc = 0;
	unsigned long volatile t_start = jiffies;
	u32 volatile data = 0;

	/* We enable mdio gpio purpose register, and disable it when exit. */
	enable_mdio(1);

	// make sure previous read operation is complete
	while (1) {
			// 0 : Read/write operation complete
		if(!( sysRegRead(MDIO_PHY_CONTROL_0) & (0x1 << 31))) 
		{
			break;
		}
		else if (time_after(jiffies, t_start + 5*HZ)) {
			enable_mdio(0);
			printk("\n MDIO Read operation is ongoing !!\n");
			return rc;
		}
	}

	data  = (0x01 << 16) | (0x02 << 18) | (phy_addr << 20) | (phy_register << 25);
	sysRegWrite(MDIO_PHY_CONTROL_0, data);
	data |= (1<<31);
	sysRegWrite(MDIO_PHY_CONTROL_0, data);
	//printk("\n Set Command [0x%08X] to PHY !!\n",MDIO_PHY_CONTROL_0);


	// make sure read operation is complete
	t_start = jiffies;
	while (1) {
		if (!(sysRegRead(MDIO_PHY_CONTROL_0) & (0x1 << 31))) {
			status = sysRegRead(MDIO_PHY_CONTROL_0);
			*read_data = (u32)(status & 0x0000FFFF);

			enable_mdio(0);
			return 1;
		}
		else if (time_after(jiffies, t_start+5*HZ)) {
			enable_mdio(0);
			printk("\n MDIO Read operation is ongoing and Time Out!!\n");
			return 0;
		}
	}
}

u32 mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data)
{
	unsigned long volatile t_start=jiffies;
	u32 volatile data;

	enable_mdio(1);

	// make sure previous write operation is complete
	while(1) {
		if (!(sysRegRead(MDIO_PHY_CONTROL_0) & (0x1 << 31))) 
		{
			break;
		}
		else if (time_after(jiffies, t_start + 5 * HZ)) {
			enable_mdio(0);
			printk("\n MDIO Write operation ongoing\n");
			return 0;
		}
	}

	data = (0x01 << 16)| (1<<18) | (phy_addr << 20) | (phy_register << 25) | write_data;
	sysRegWrite(MDIO_PHY_CONTROL_0, data);
	data |= (1<<31);
	sysRegWrite(MDIO_PHY_CONTROL_0, data); //start operation
	//printk("\n Set Command [0x%08X] to PHY !!\n",MDIO_PHY_CONTROL_0);

	t_start = jiffies;

	// make sure write operation is complete
	while (1) {
		if (!(sysRegRead(MDIO_PHY_CONTROL_0) & (0x1 << 31))) //0 : Read/write operation complete
		{
			enable_mdio(0);
			return 1;
		}
		else if (time_after(jiffies, t_start + 5 * HZ)) {
			enable_mdio(0);
			printk("\n MDIO Write operation Time Out\n");
			return 0;
		}
	}
}

#else // not rt6855

u32 mii_mgr_read(u32 phy_addr, u32 phy_register, u32 *read_data)
{
	u32 volatile status = 0;
	u32 rc = 0;
	unsigned long volatile t_start = jiffies;
#if !defined (CONFIG_RALINK_RT3052) && !defined (CONFIG_RALINK_RT3352) && !defined (CONFIG_RALINK_RT5350)
	u32 volatile data = 0;
#endif

	/* We enable mdio gpio purpose register, and disable it when exit. */
	enable_mdio(1);

	// make sure previous read operation is complete
	while (1) {
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || defined (CONFIG_RALINK_RT5350)
		// rd_rdy: read operation is complete
		if(!( sysRegRead(MDIO_PHY_CONTROL_1) & (0x1 << 1))) 
#else
			// 0 : Read/write operation complet
		if(!( sysRegRead(MDIO_PHY_CONTROL_0) & (0x1 << 31))) 
#endif
		{
			break;
		}
		else if (time_after(jiffies, t_start + 5*HZ)) {
			enable_mdio(0);
			printk("\n MDIO Read operation is ongoing !!\n");
			return rc;
		}
	}

#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || defined (CONFIG_RALINK_RT5350)
	sysRegWrite(MDIO_PHY_CONTROL_0 , (1<<14) | (phy_register << 8) | (phy_addr));
#else
	data  = (phy_addr << 24) | (phy_register << 16);
	sysRegWrite(MDIO_PHY_CONTROL_0, data);
	data |= (1<<31);
	sysRegWrite(MDIO_PHY_CONTROL_0, data);
#endif
	//printk("\n Set Command [0x%08X] to PHY !!\n",MDIO_PHY_CONTROL_0);


	// make sure read operation is complete
	t_start = jiffies;
	while (1) {
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || defined (CONFIG_RALINK_RT5350)
		if (sysRegRead(MDIO_PHY_CONTROL_1) & (0x1 << 1)) {
			status = sysRegRead(MDIO_PHY_CONTROL_1);
			*read_data = (u32)(status >>16);

			enable_mdio(0);
			return 1;
		}
#else
		if (!(sysRegRead(MDIO_PHY_CONTROL_0) & (0x1 << 31))) {
			status = sysRegRead(MDIO_PHY_CONTROL_0);
			*read_data = (u32)(status & 0x0000FFFF);

			enable_mdio(0);
			return 1;
		}
#endif
		else if (time_after(jiffies, t_start+5*HZ)) {
			enable_mdio(0);
			printk("\n MDIO Read operation is ongoing and Time Out!!\n");
			return 0;
		}
	}
}


u32 mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data)
{
	unsigned long volatile t_start=jiffies;
	u32 volatile data;

	enable_mdio(1);

	// make sure previous write operation is complete
	while(1) {
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || defined (CONFIG_RALINK_RT5350)
		if (!(sysRegRead(MDIO_PHY_CONTROL_1) & (0x1 << 0)))
#else
		if (!(sysRegRead(MDIO_PHY_CONTROL_0) & (0x1 << 31))) 
#endif
		{
			break;
		}
		else if (time_after(jiffies, t_start + 5 * HZ)) {
			enable_mdio(0);
			printk("\n MDIO Write operation ongoing\n");
			return 0;
		}
	}

#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || defined (CONFIG_RALINK_RT5350)
	data = ((write_data & 0xFFFF) << 16);
	data |= (phy_register << 8) | (phy_addr);
	data |= (1<<13);
	sysRegWrite(MDIO_PHY_CONTROL_0, data);
#else
	data = (1<<30) | (phy_addr << 24) | (phy_register << 16) | write_data;
	sysRegWrite(MDIO_PHY_CONTROL_0, data);
	data |= (1<<31);
	sysRegWrite(MDIO_PHY_CONTROL_0, data); //start operation
#endif
	//printk("\n Set Command [0x%08X] to PHY !!\n",MDIO_PHY_CONTROL_0);

	t_start = jiffies;

	// make sure write operation is complete
	while (1) {
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || defined (CONFIG_RALINK_RT5350)
		if (sysRegRead(MDIO_PHY_CONTROL_1) & (0x1 << 0)) //wt_done ?= 1
#else
		if (!(sysRegRead(MDIO_PHY_CONTROL_0) & (0x1 << 31))) //0 : Read/write operation complete
#endif
		{
			enable_mdio(0);
			return 1;
		}
		else if (time_after(jiffies, t_start + 5 * HZ)) {
			enable_mdio(0);
			printk("\n MDIO Write operation Time Out\n");
			return 0;
		}
	}
}




#endif

#if defined(CONFIG_AR8316) || defined(CONFIG_AR8328)
//#define outw(address, value)    *((volatile uint32_t *)(address)) = cpu_to_le32(value)
//#define inw(address)            le32_to_cpu(*(volatile u32 *)(address))
uint32_t athrs16_reg_read(uint32_t reg_addr)
{
    	uint32_t reg_word_addr;
    	uint32_t phy_addr, tmp_val, reg_val;
    	uint32_t phy_val;
    	uint32_t phy_reg;
	//uint32_t mo_reg;

    	/* change reg_addr to 16-bit word address, 32-bit aligned */
    	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

    	/* configure register high address */
    	phy_addr = 0x18;
    	phy_reg = 0x0;
    	phy_val = ((reg_word_addr >> 8) & 0x3ff);  /* A18-A9 of reg address */
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_write(phy_addr, phy_reg, phy_val);

    	/* For some registers such as MIBs, since it is read/clear, we should */
    	/* read the lower 16-bit register then the higher one */

    	/* read register in lower address */
    	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* A8-A6 of reg address */
   	phy_reg = (reg_word_addr & 0x1f);   /* A5-A1 of reg address */
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_read(phy_addr, phy_reg, &reg_val);

    	/* read register in higher address */
    	reg_word_addr++;
    	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* A8-A6 of reg address */
    	phy_reg = (reg_word_addr & 0x1f);   /* A5-A1 of reg address */
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_read(phy_addr, phy_reg, &tmp_val);
    	reg_val |= (tmp_val << 16);

    return reg_val;   
}

uint32_t athrs17_reg_read(uint32_t reg_addr)
{
    	uint32_t reg_word_addr;
    	uint32_t phy_addr, tmp_val, reg_val;
    	uint32_t phy_val;
    	uint32_t phy_reg;
	//uint32_t mo_reg;

    	/* change reg_addr to 16-bit word address, 32-bit aligned */
    	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

    	/* configure register high address */
    	phy_addr = 0x18;
    	phy_reg = 0x0;
    	phy_val = ((reg_word_addr >> 8) & 0x3ff);  /* A18-A9 of reg address */
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_write(phy_addr, phy_reg, phy_val);

    	/* For some registers such as MIBs, since it is read/clear, we should */
    	/* read the lower 16-bit register then the higher one */

    	/* read register in lower address */
    	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* A8-A6 of reg address */
   	phy_reg = (reg_word_addr & 0x1f);   /* A5-A1 of reg address */
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_read(phy_addr, phy_reg, &reg_val);

    	/* read register in higher address */
    	reg_word_addr++;
    	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* A8-A6 of reg address */
    	phy_reg = (reg_word_addr & 0x1f);   /* A5-A1 of reg address */
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_read(phy_addr, phy_reg, &tmp_val);
    	reg_val |= (tmp_val << 16);

    return reg_val;   
}

void athrs16_reg_write(uint32_t reg_addr, uint32_t reg_val)
{
    	uint32_t reg_word_addr;
    	uint32_t phy_addr;
    	uint32_t phy_val;
    	uint32_t phy_reg;
	//uint32_t mo_reg;
    	
	/* change reg_addr to 16-bit word address, 32-bit aligned */
    	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

    	/* configure register high address */
    	phy_addr = 0x18;
    	phy_reg = 0x0;
    	phy_val = ((reg_word_addr >> 8) & 0x3ff);  /* A18-A9 of reg address */
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_write(phy_addr, phy_reg, phy_val);

    	/* For some registers such as ARL and VLAN, since they include BUSY bit */
    	/* in lower address, we should write the higher 16-bit register then the */
    	/* lower one */

    	/* read register in higher address */
    	reg_word_addr++;
    	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* A8-A6 of reg address */
    	phy_reg = (reg_word_addr & 0x1f);   /* A5-A1 of reg address */
    	phy_val = ((reg_val >> 16) & 0xffff);
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_write(phy_addr, phy_reg, phy_val);

    	/* write register in lower address */
    	reg_word_addr--;
    	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* A8-A6 of reg address */
    	phy_reg = (reg_word_addr & 0x1f);   /* A5-A1 of reg address */
    	phy_val = (reg_val & 0xffff);
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_write(phy_addr, phy_reg, phy_val); 
}

void athrs17_reg_write(uint32_t reg_addr, uint32_t reg_val)
{
    	uint32_t reg_word_addr;
    	uint32_t phy_addr;
    	uint32_t phy_val;
    	uint32_t phy_reg;
	//uint32_t mo_reg;
    	
	/* change reg_addr to 16-bit word address, 32-bit aligned */
    	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

    	/* configure register high address */
    	phy_addr = 0x18;
    	phy_reg = 0x0;
    	phy_val = ((reg_word_addr >> 8) & 0x3ff);  /* A18-A9 of reg address */
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_write(phy_addr, phy_reg, phy_val);

    	/* For some registers such as ARL and VLAN, since they include BUSY bit */
    	/* in lower address, we should write the higher 16-bit register then the */
    	/* lower one */

    	/* read register in higher address */
    	reg_word_addr++;
    	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* A8-A6 of reg address */
    	phy_reg = (reg_word_addr & 0x1f);   /* A5-A1 of reg address */
    	phy_val = ((reg_val >> 16) & 0xffff);
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_write(phy_addr, phy_reg, phy_val);

    	/* write register in lower address */
    	reg_word_addr--;
    	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* A8-A6 of reg address */
    	phy_reg = (reg_word_addr & 0x1f);   /* A5-A1 of reg address */
    	phy_val = (reg_val & 0xffff);
    	//change P5 external PHY address before operation
	//mo_reg = inw(0xb01100c8);
	//mo_reg &= 0xe0ffffff;
	//mo_reg |= (phy_addr << 24); 
	//outw(0xb01100c8, mo_reg);
    	mii_mgr_write(phy_addr, phy_reg, phy_val); 
}
void athrs17_init(void)
{
    static int athr17_init_flag=0;
    int phy_addr = 0; 
    uint32_t po_reg_val; 
    if (athr17_init_flag){
        printk("AR8327 Already Supported!!\n");
        return -1;
    }

    //Use power on strapping resistor to config the SW settings.
    //Bit30 is not specified in the data sheet, it is a hidden value.

//    athrs16_reg_write(0x010, 0x40000000);// for AR8327 only    
    athrs17_reg_write(0x624, 0x007f7f7f);
    athrs17_reg_write(0x004, 0x07600000);
    //It is PAD6 control reg. Should not relevant...But the FAQ note write
    //this done.
    athrs17_reg_write(0x00c, 0x01000000);

    /*The MAC0 speed/duplex is enabled by CPU. The link status of MAC1~MAC5 */
    /*are controlled by PHY0~PHY4*/
    //MAC config controlled by software
    //force P0 1000M; TX/RX MAC enable; TX/RX flow control enable; P0 full duplex;
    athrs17_reg_write(0x07c, 0x0000007e);    

    // New by YH 20121219 
    for (phy_addr = 0x0; phy_addr <= 4; phy_addr++){
        mii_mgr_write(phy_addr, 0x1d, 0x3d);
        mii_mgr_write(phy_addr, 0x1e, 0x48a0);
    }
    printk("AR8327/AR8328 Support!!\n");
    athr17_init_flag = 1;
}
void athrs17_LAN_WAN_Par(void)
{
#warning "AR8327 PORT init as P1P2P3P4P5 ==> WLLLL"
    
    //Setting the port-based forwarding domain
    athrs17_reg_write(0x660, 0x0014007E); //CPU PORT:0111_1110
    athrs17_reg_write(0x66C, 0x00140005); //LAN1:0000_0101
    athrs17_reg_write(0x678, 0x00140003); //LAN2:0000_0011
    athrs17_reg_write(0x684, 0x00140001); //WAN0:0000_0001
    athrs17_reg_write(0x690, 0x00140021); //GMN LAN:0010_0001    
    athrs17_reg_write(0x69C, 0x00140011); //ARM LAN:0001_0001

    /*Max Frame Size*/
    /*Allow 1518+4 length for single vlan*/
	//athrs16_reg_write(0x78, 0x5F2);
  
    /*PORT VLAN CTRL REG*/

    //P0 tag:CPU, should tagged in linux...
    athrs17_reg_write(0x420, 0x00000000);
    athrs17_reg_write(0x424, 0x00002040);
    
    /* All port untag at boot time */ 
    //P1 untag:LAN1 PVID=1
    athrs17_reg_write(0x428, 0x00010001);
    athrs17_reg_write(0x42C, 0x00001040);
    //P3 untag:WAN0 PVID=2
    athrs17_reg_write(0x438, 0x00020002);
    athrs17_reg_write(0x43C, 0x80001040);
    //P2 untag:LAN2 PVID=1
    athrs17_reg_write(0x430, 0x00010001);
    athrs17_reg_write(0x434, 0x00001040);
    //P4 untag:LAN3 PVID=3
    athrs17_reg_write(0x440, 0x00030003);
    athrs17_reg_write(0x444, 0x00001040);
    //P5 untag:LAN4 PVID=3
    athrs17_reg_write(0x448, 0x00030003);
    athrs17_reg_write(0x44C, 0x00001040);
}

int athrs17_reset()
{
    static int athrs17_rst_flag=0;
    uint32_t reg_val;
    
    if (athrs17_rst_flag){
        printk("AR8328 Already Reset!\n");
        return -1;
    }
    /* Method #1: set the bit31 of 0x0000 to software reset the switch */
    //Read the Value of reg addr 0x0000
    /*
    reg_val = athrs17_reg_read(0x00);
    reg_val = reg_val | 0x80000000;
    athrs17_reg_write(0x00  , reg_val);
    */
    /* Method #2: set the bit15 of 0x0000 of phy to software reset the phy */
    
    mii_mgr_write(0x0, 0x0, 0x8000);
    mii_mgr_write(0x1, 0x0, 0x8000);
    mii_mgr_write(0x2, 0x0, 0x8000);
    mii_mgr_write(0x3, 0x0, 0x8000);
    mii_mgr_write(0x4, 0x0, 0x8000);
    
    //print out the message when all init values are written to AR8327 
    printk("AR8328 Reset...\n");
    athrs17_rst_flag = 1;
}

void athrs16_init(void)
{
    	static int athr16_init_flag=0;
    	uint32_t po_reg_val; 
	/* if using header for register configuration, we have to     */
    	/* configure s16 register after frame transmission is enabled */
    	if (athr16_init_flag)
        	return;

    	/*Power on strip mode setup*/
    	/*Configure the MAC0 RGMII PHY mode, and enable the input clock delay*/
   	/*AR8316 only support RGMII MAC0 PHY mode. MACO in MAC mode is only supported when us GMII*/
    	/*RT3052 only have RGMII interface, hence MAC0 of AR8316 is set to PHY mode in this case*/
    	athrs16_reg_write(0x8, 0x012e1be2);
    	/*The MAC0 speed/duplex is enabled by CPU. The link status of MAC1~MAC5 */
    	/*are controlled by PHY0~PHY4*/
    	athrs16_reg_write(0x100, 0x7e);
    	athrs16_reg_write(0x200, 0x0270);
    	athrs16_reg_write(0x300, 0x0270);
    	athrs16_reg_write(0x400, 0x0270);
    	athrs16_reg_write(0x500, 0x0270);
    	athrs16_reg_write(0x600, 0x0270);
    	/*unitcast and multicast flood destination port*/
    	athrs16_reg_write(0x2c, 0x003f003f);
        
    	/*allow single vlan tag to be transmitted and received for all port*/
    	/*Enable learning and forwarding*/
    	athrs16_reg_write(0x104, 0x6004);
    	athrs16_reg_write(0x204, 0x6004);
    	athrs16_reg_write(0x304, 0x6004);
    	athrs16_reg_write(0x404, 0x6004);
    	athrs16_reg_write(0x504, 0x6004);    
    	athrs16_reg_write(0x604, 0x6004);
	
	//get Power-On Strapping Reg Value to verify we indeed write the correct value.
    	po_reg_val = athrs16_reg_read(0x8);
	//printk("\n AR8316's Power-On Strapping Reg Value is 0x%08X \n", po_reg_val);
	//printk("\n AR8316's MAC0 started as PHY mode in RGMII.\n");
	printk("AR8316 Support!!\n");

    	athr16_init_flag = 1;
}
void athrs16_LAN_WAN_Par(void)
{
//port arrangement is LLLLW, P1~P5    
//port member for each port
//P0:111110 = 0x3E
//P1:011101 = 0x1D
//P2:011011 = 0x1B
//P3:010111 = 0x17
//P4:001111 = 0x0F
//P5:000001 = 0x01

//PVID setting
//P0:1
//P1:1
//P2:1
//P3:1
//P4:1
//P5:2

//Egress Port Tagging
//P0:tag/unmodified
//P1:untag
//P2:untag
//P3:untag
//P4:untag
//P5:untag

//reg value for Port-based VLAN register(0x108,0x208,0x308,0x408,0x508,0x608)
//0x108 = 0x003E0001
//0x208 = 0x001D0001
//0x308 = 0x001B0001
//0x408 = 0x00170001
//0x508 = 0x000F0001
//0x608 = 0x00010002

//reg value for Port Control register(0x104, 0x204, 0x304, 0x404, 0x504, 0x604)
//the initial value for this is 00, means unmodifying the tag. So, we don't need to set this.

    	athrs16_reg_write(0x108, 0x003e0001);
    	athrs16_reg_write(0x208, 0x001d0001);
    	athrs16_reg_write(0x308, 0x001b0001);
    	athrs16_reg_write(0x408, 0x00170001);
    	athrs16_reg_write(0x508, 0x000f0001);
    	athrs16_reg_write(0x608, 0x00010002);
    	

	//P0 tag,
    	athrs16_reg_write(0x104, 0x00006204);
	//P1 untag
    	athrs16_reg_write(0x204, 0x00006104);
	//P2 untag
    	athrs16_reg_write(0x304, 0x00006104);
	//P3 untag
    	athrs16_reg_write(0x404, 0x00006104);
	//P4 untag
    	athrs16_reg_write(0x504, 0x00006104);
	//P5 untag
    	athrs16_reg_write(0x604, 0x00006104);
/*
// check VT is busy
    	u32 temp=0;
	temp = athrs16_reg_read(0x0040);
	temp = temp&0x00000004;
	while (temp == 0x4){
    	athrs16_reg_write(0x0044, 0x0000080e);
    	athrs16_reg_write(0x0040, 0x0001000a);
	}
	temp = athrs16_reg_read(0x0040);
	temp = temp&0x00000004;
	while (temp == 0x4){
    	athrs16_reg_write(0x0044, 0x00000818);
    	athrs16_reg_write(0x0040, 0x0002000a);
	}
*/
/*
// This is a test !! We test the tag is outting from P3
	u32 temp;

	//P0: PVID=1, port_mem=(P0,P5), ingress=port_vlan, egress=untag
    	athrs16_reg_write(0x108, 0x00200001);
    	temp = athrs16_reg_read(0x108);
	printf("\n Port Settin 0xn08 is %08x \n", temp);
	
	//P1: PVID=2, port_mem=(P1, P2, P3), ingress=port_vlan, egress=untag	
    	athrs16_reg_write(0x208, 0x000c0002);
    	temp = athrs16_reg_read(0x208);
	printf("\n Port Settin 0xn08 is %08x \n", temp);
	
	//P2: PVID=2, port_mem=(P1, P2, P3), ingress=port_vlan, egress=untag
    	athrs16_reg_write(0x308, 0x000a0002);
    	temp = athrs16_reg_read(0x308);
	printf("\n Port Settin 0xn08 is %08x \n", temp);

	//P3: PVID=5, port_mem=(P1, P2, P3, P4), ingress=port_vlan, egress=tag
    	athrs16_reg_write(0x408, 0x00160005);
    	temp = athrs16_reg_read(0x408);
	printf("\n Port Settin 0xn08 is %08x \n", temp);

	//P4: PVID=3, port_mem=(P3, P4), ingress=port_vlan, egress=untag
    	athrs16_reg_write(0x508, 0x00080003);
    	temp = athrs16_reg_read(0x508);
	printf("\n Port Settin 0xn08 is %08x \n", temp);

	//P5: PVID=1, port_mem=(P0,P5), ingress=port_vlan, egress=untag
    	athrs16_reg_write(0x608, 0x00010001);
    	temp = athrs16_reg_read(0x608);
	printf("\n Port Settin 0xn08 is %08x \n", temp);


//force P3 add tag on egress to see if there is a tag exit
	//P0 untag
    	athrs16_reg_write(0x104, 0x00006104);
    	temp = athrs16_reg_read(0x104);
	printf("\n Port Settin 0xn04 is %08x \n", temp);
	//P1 untag
    	athrs16_reg_write(0x204, 0x00006104);
    	temp = athrs16_reg_read(0x204);
	printf("\n Port Settin 0xn04 is %08x \n", temp);
	//P2 untag
    	athrs16_reg_write(0x304, 0x00006104);
    	temp = athrs16_reg_read(0x304);
	printf("\n Port Settin 0xn04 is %08x \n", temp);
	//P3 tag
    	athrs16_reg_write(0x404, 0x00006304);
    	temp = athrs16_reg_read(0x404);
	printf("\n Port Settin 0xn04 is %08x \n", temp);
	//P4 untag
    	athrs16_reg_write(0x504, 0x00006104);
    	temp = athrs16_reg_read(0x504);
	printf("\n Port Settin 0xn04 is %08x \n", temp);
	//P5 untag
    	athrs16_reg_write(0x604, 0x00006104);
    	temp = athrs16_reg_read(0x604);
	printf("\n Port Settin 0xn04 is %08x \n", temp);

	printf("\n AR8316 VLAN Support Now.\n");

*/
}
#endif



EXPORT_SYMBOL(mii_mgr_write);
EXPORT_SYMBOL(mii_mgr_read);
#ifdef CONFIG_AR8316
EXPORT_SYMBOL(athrs16_reg_write);
EXPORT_SYMBOL(athrs16_reg_read);
#endif
