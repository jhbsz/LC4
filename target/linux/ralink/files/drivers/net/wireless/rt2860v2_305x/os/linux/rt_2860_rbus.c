#include <rt_config.h>

extern int rt2860_open(struct net_device *net_dev);
extern int rt2860_close(struct net_device *net_dev);
extern int rt2860_packet_xmit(struct sk_buff *skb);
extern int rt2860_send_packets(struct sk_buff *skb, struct net_device *net_dev);
extern INT rt2860_ioctl(struct net_device *net_dev, struct ifreq *rq, int cmd);
extern struct net_device_stats *rt2860_get_ether_stats(IN struct net_device *net_dev);
extern void rt2860_set_rx_mode(IN struct net_device *net_dev);

#ifdef CONFIG_STA_SUPPORT
extern	const struct iw_handler_def rt2860_iw_handler_def;
#if WIRELESS_EXT >= 12
// This function will be called when query /proc
struct iw_statistics *rt2860_get_wireless_stats(
    IN struct net_device *net_dev);
#endif
#endif // CONFIG_STA_SUPPORT //

static struct net_device *rt2860_dev;

extern NDIS_STATUS rt_ieee80211_if_setup(struct net_device *dev, PRTMP_ADAPTER pAd);

int rbus_module_init(PRTMP_ADAPTER **ppAd)
{
    struct  net_device      *net_dev;
    ULONG                   csr_addr;
    INT                     Status;
    PVOID		    *handle;
    PRTMP_ADAPTER           pAd;
		
    DBGPRINT(RT_DEBUG_TRACE, ("RT2860_init_module\n"));
#ifdef CONFIG_AP_SUPPORT
    printk("2860 version : %s (%s)\n", AP_DRIVER_VERSION, __DATE__);
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
    printk("2860 version : %s (%s)\n", STA_DRIVER_VERSION, __DATE__);
#endif // CONFIG_STA_SUPPORT //

#if LINUX_VERSION_CODE <= 0x20402       // Red Hat 7.1
    net_dev = alloc_netdev(sizeof(PRTMP_ADAPTER), "eth%d", ether_setup);
    DBGPRINT(RT_DEBUG_TRACE, "alloc_netdev\n");
#else
    net_dev = alloc_etherdev(sizeof(PRTMP_ADAPTER));
    DBGPRINT(RT_DEBUG_TRACE, ("alloc_etherdev\n"));
#endif
    if (net_dev == NULL) 
    {
        DBGPRINT(RT_DEBUG_TRACE, ("init_ethernet failed\n"));
        goto err_out;
    }
	
    rt2860_dev = net_dev;

	netif_stop_queue(net_dev);

#if 0 // Roger: Keep for Reference
#ifdef NETWORK_MANAGER_SUPPORT
/* for supporting Network Manager */
/* Set the sysfs physical device reference for the network logical device
 * if set prior to registration will cause a symlink during initialization.
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0))
    SET_NETDEV_DEV(net_dev, &(pci_dev->dev));
#endif
#endif // NETWORK_MANAGER_SUPPORT //
#endif // #if 0

    net_dev->irq = 4;
    net_dev->addr_len = 6;

    // map physical address to virtual address for accessing register
    csr_addr = (unsigned long) RT2860_CSR_ADDR;

    handle = kmalloc(sizeof(struct os_cookie) , GFP_KERNEL);
    ((POS_COOKIE)handle)->pci_dev = (PPCI_DEV)net_dev;
    if( RTMPAllocAdapterBlock(handle, &pAd) != NDIS_STATUS_SUCCESS) {
	printk(" RTMPAllocAdapterBlock !=  NDIS_STATUS_SUCCESS\n");
	goto err_out_free_res;
    }					        


    // Save CSR virtual address and irq to device structure
    net_dev->base_addr = csr_addr;
    net_dev->priv = (PVOID)pAd;
    pAd->CSRBaseAddress = (PUCHAR)net_dev->base_addr;
    pAd->net_dev = net_dev;
    net_dev->priv_flags = INT_MAIN;

#ifdef CONFIG_STA_SUPPORT
    pAd->StaCfg.OriDevType = net_dev->type;
#endif // CONFIG_STA_SUPPORT //

#if 0
    // The chip-specific entries in the device structure.
    net_dev->open = rt2860_open;
    //net_dev->hard_start_xmit = rt2860_packet_xmit;
    net_dev->hard_start_xmit = rt2860_send_packets;
    net_dev->stop = rt2860_close;
    //net_dev->get_stats = RT2860_get_ether_stats;

#ifdef CONFIG_AP_SUPPORT	
	net_dev->get_stats = rt2860_get_ether_stats;
#endif // CONFIG_AP_SUPPORT //
	
	net_dev->set_multicast_list = rt2860_set_rx_mode;
    net_dev->do_ioctl = rt2860_ioctl;
    net_dev->priv_flags = INT_MAIN;

#ifdef CONFIG_STA_SUPPORT
#if WIRELESS_EXT >= 12
#if IW_HANDLER_VERSION < 7
    net_dev->get_wireless_stats = rt2860_get_wireless_stats;
#endif // IW_HANDLER_VERSION < 7 //
    net_dev->wireless_handlers = (struct iw_handler_def *) &rt2860_iw_handler_def;
#endif // WIRELESS_EXT >= 12 //
    pAd->StaCfg.OriDevType = net_dev->type;
#endif // CONFIG_STA_SUPPORT //

    {// find available
	INT     i=0;
	CHAR    slot_name[IFNAMSIZ];
	struct net_device   *device;

	for (i = 0; i < 8; i++)
	{
	    sprintf(slot_name, "ra%d", i);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
			device = dev_get_by_name(slot_name);
#else
            for (device = dev_base; device != NULL; device = device->next)
 			{
                if (strncmp(device->name, slot_name, 4) == 0)
                {
                    break;
                }
            }
#endif
            if(device == NULL)  break;
        }
        if(i == 8)
        {
            DBGPRINT(RT_DEBUG_ERROR, ("No available slot name\n"));
            goto err_out_free_res;
        }

        sprintf(net_dev->name, "ra%d", i);
    }
#endif

#ifdef CONFIG_STA_SUPPORT
	pAd->OpMode = OPMODE_STA;
#endif // CONFIG_STA_SUPPORT //

#ifdef CONFIG_AP_SUPPORT
	pAd->OpMode = OPMODE_AP;
#endif // CONFIG_AP_SUPPORT //

    //ether_setup(net_dev);
    rt_ieee80211_if_setup(net_dev, pAd);

    // Register this device
    Status = register_netdev(net_dev);
    if (Status)
    {
        DBGPRINT(RT_DEBUG_ERROR, ("failed to register_netdev\n"));
        goto err_out_free_res;
    }

    DBGPRINT(RT_DEBUG_TRACE, ("%s: at CSR addr 0x%1x, IRQ %d. \n",
        net_dev->name, (unsigned int)csr_addr, net_dev->irq));

#ifdef WSC_INCLUDED
	InitializeWSCTLV();
    init_MUTEX_LOCKED(&(pAd->write_dat_file_semaphore));
    init_completion(&pAd->write_dat_file_notify);
    start_write_dat_file_thread(pAd);

    if (pAd->pHmacData)
        kfree(pAd->pHmacData);
    pAd->pHmacData = kmalloc(sizeof(CHAR)*(2048), MEM_ALLOC_FLAG);
    if (pAd->pHmacData == NULL)
    {
        DBGPRINT(RT_DEBUG_ERROR, ("Wsc HmacData memory alloc failed!\n"));
    }

#endif // WSC_INCLUDED //

#ifdef CONFIG_AP_SUPPORT
	pAd->ApCfg.MBSSID[MAIN_MBSSID].MSSIDDev = net_dev;
#endif // CONFIG_AP_SUPPORT //

    *ppAd = pAd;
    
    return 0;

err_out_free_res:
    kfree (net_dev);
err_out:
    return -ENODEV;
}

VOID rbus_module_exit(VOID)
{
	struct net_device   *net_dev = rt2860_dev;
    RTMP_ADAPTER *pAd = net_dev->priv;

	if (pAd != NULL)
	{

#ifdef WSC_INCLUDED
    	if (pAd->write_dat_file_pid >= 0)
    	{
    	    int ret;
    	    pAd->time_to_die = 1;
    	    up(&(pAd->write_dat_file_semaphore));
    	    wmb(); // need to check
			ret = kill_proc (pAd->write_dat_file_pid, SIGTERM, 1);
			if (ret)
			{
				printk (KERN_ERR "%s: unable to signal thread\n", pAd->net_dev->name);
				return;
			}
    	    wait_for_completion (&pAd->write_dat_file_notify);
    	}

		if (pAd->pHmacData)
			kfree(pAd->pHmacData);
#endif // WSC_INCLUDED //
    
#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
	// remove all AP-client virtual interfaces.
		RT28xx_ApCli_Remove(pAd);
#endif // APCLI_SUPPORT //

#ifdef WDS_SUPPORT
	// remove all WDS virtual interfaces.
		RT28xx_WDS_Remove(pAd);
#endif // WDS_SUPPORT //

#ifdef MBSS_SUPPORT
    	RT28xx_MBSS_Remove(pAd);
#endif // MBSS_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

		unregister_netdev(net_dev);

		RTMPFreeAdapter(pAd);

	}
	else
	{
		unregister_netdev(net_dev);
	}
	

    // Free pre-allocated net_device memory
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    free_netdev(net_dev);
#else
	kfree(net_dev);
#endif
}


