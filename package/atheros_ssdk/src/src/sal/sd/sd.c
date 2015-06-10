/*
 * Copyright (c) 2007-2008 Atheros Communications, Inc.
 * All rights reserved.
 *
 */

#include "sw.h"
#include "ssdk_init.h"
#include "sd.h"
#include "sw_api.h"
#include "sw_api_us.h"
/* Add includes here for RALINK IOCTL */
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include "ra_ioctl.h"

/* Add the mdio access func of RT3883 here to hook up the Atheros ssdk */ 
# warning "this is sd, if you see me means we have hooked up the mii_mgr"
static sw_error_t
mii_mgr_set(a_uint32_t dev_id, a_uint32_t phy_addr, a_uint32_t reg_addr, a_uint16_t data);
static sw_error_t 
mii_mgr_get(a_uint32_t dev_id, a_uint32_t phy_addr, a_uint32_t reg_addr, a_uint16_t * data);

mdio_reg_set ssdk_mdio_set    = mii_mgr_set;
mdio_reg_get ssdk_mdio_get    = mii_mgr_get;

/* We do not use Atheros Header as control of Atheros SW, hence we left the
 * following two as NULL */
hdr_reg_set  ssdk_hdr_reg_set = NULL;
hdr_reg_get  ssdk_hdr_reg_get = NULL;

#define SWITCH_IFNAME "eth2"

static int esw_fd = -1;
static void switch_init(void)
{
    /* socket has already opened */
	if (esw_fd >= 0) {
        return;
    }
    esw_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (esw_fd < 0) {
		perror("socket");
		exit(0);
	}
}

void switch_fini(void)
{
	if (esw_fd >= 0) {
	    close(esw_fd);
        esw_fd = -1;
    }
}

static sw_error_t
mii_mgr_set(a_uint32_t dev_id, a_uint32_t phy_addr, a_uint32_t reg_addr, a_uint16_t data)
{
    
    sw_error_t rv = SW_OK;
	ra_mii_ioctl_data mii_w_param;
    struct ifreq ifr;

    switch_init();
	if (esw_fd < 0) {
        /* switch fd open failed */
		switch_fini();
		return SW_FAIL;
    }
    mii_w_param.phy_id = phy_addr;
    mii_w_param.reg_num = reg_addr;
    mii_w_param.val_in = data;
    mii_w_param.val_out = 0;
	
    strncpy(ifr.ifr_name, SWITCH_IFNAME, sizeof(SWITCH_IFNAME));
	ifr.ifr_data = &mii_w_param;

	if (-1 == ioctl(esw_fd, RAETH_RGMII_WRITE, &ifr)) {
		perror("ioctl");
		switch_fini();
		return SW_FAIL;
	}
    switch_fini();
    return rv;
}

static sw_error_t 
mii_mgr_get(a_uint32_t dev_id, a_uint32_t phy_addr, a_uint32_t reg_addr, a_uint16_t * data)
{
    sw_error_t rv = SW_OK;
	ra_mii_ioctl_data mii_r_param;
    struct ifreq ifr;

    mii_r_param.phy_id = phy_addr;
    mii_r_param.reg_num = reg_addr;
    
    switch_init();
	if (esw_fd < 0) {
        /* switch fd open failed */
		switch_fini();

		return SW_FAIL;
        }

	strncpy(ifr.ifr_name, SWITCH_IFNAME, sizeof(SWITCH_IFNAME));
	ifr.ifr_data = &mii_r_param;
	if (-1 == ioctl(esw_fd, RAETH_RGMII_READ, &ifr)) {
		perror("ioctl");
		switch_fini();

		return SW_FAIL;
	}
	*data = mii_r_param.val_out;
    switch_fini();

    return rv;
}

sw_error_t
sd_reg_mdio_set(a_uint32_t dev_id, a_uint32_t phy, a_uint32_t reg,
                a_uint16_t data)
{
    sw_error_t rv = SW_OK;

    if (NULL != ssdk_mdio_set) {
        rv = ssdk_mdio_set(dev_id, phy, reg, data);
    } else {
        #if ((!defined(KERNEL_MODULE)) && defined(UK_IF))
        {
        a_uint32_t args[SW_MAX_API_PARAM];

        args[0] = SW_API_PHY_SET;
        args[1] = (a_uint32_t) & rv;
        args[2] = dev_id;
        args[3] = phy;
        args[4] = reg;
        args[5] = data;
        if (SW_OK != sw_uk_if(args)) {
            return SW_FAIL;
        }
        }
        #else
        return SW_NOT_SUPPORTED;
        #endif
    }

    return rv;
}

sw_error_t
sd_reg_mdio_get(a_uint32_t dev_id, a_uint32_t phy, a_uint32_t reg, a_uint16_t * data)
{
    sw_error_t rv = SW_OK;

    if (NULL != ssdk_mdio_get) {
        rv = ssdk_mdio_get(dev_id, phy, reg, data);
    } else {
        #if ((!defined(KERNEL_MODULE)) && defined(UK_IF))
        {
        a_uint32_t args[SW_MAX_API_PARAM];
        a_uint32_t tmp;

        args[0] = SW_API_PHY_GET;
        args[1] = (a_uint32_t) & rv;
        args[2] = dev_id;
        args[3] = phy;
        args[4] = reg;
        args[5] = (a_uint32_t) & tmp;
        if (SW_OK != sw_uk_if(args)) {
            return SW_FAIL;
        }
        *data = *((a_uint16_t *)&tmp);
        }
        #else
        return SW_NOT_SUPPORTED;
        #endif
    }

    return rv;
}

sw_error_t
sd_reg_hdr_set(a_uint32_t dev_id, a_uint32_t reg_addr, a_uint8_t * reg_data, a_uint32_t len)
{
    sw_error_t rv;

    if (NULL != ssdk_hdr_reg_set) {
        rv = ssdk_hdr_reg_set(dev_id, reg_addr, reg_data, len);
    } else {
        return SW_NOT_SUPPORTED;
    }

    return rv;
}

sw_error_t
sd_reg_hdr_get(a_uint32_t dev_id, a_uint32_t reg_addr, a_uint8_t * reg_data, a_uint32_t len)
{
    sw_error_t rv;

    if (NULL != ssdk_hdr_reg_get) {
        rv = ssdk_hdr_reg_get(dev_id, reg_addr, reg_data, len);
    } else {
        return SW_NOT_SUPPORTED;
    }

    return rv;
}

sw_error_t
sd_init(a_uint32_t dev_id, ssdk_init_cfg * cfg)
{
    if (NULL != cfg->reg_func.mdio_set) {
        ssdk_mdio_set = mii_mgr_set;
    }

    if (NULL != cfg->reg_func.mdio_get) {
        ssdk_mdio_get = mii_mgr_get;
    }

    if (NULL != cfg->reg_func.header_reg_set) {
        ssdk_hdr_reg_set = cfg->reg_func.header_reg_set;
    }

    if (NULL != cfg->reg_func.header_reg_get) {
        ssdk_hdr_reg_get = cfg->reg_func.header_reg_get;
    }

    return SW_OK;
}

