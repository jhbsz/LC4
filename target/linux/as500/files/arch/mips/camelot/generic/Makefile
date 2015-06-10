#
# Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved.
#
obj-y				:= init.o setup.o reset.o \
				   wbflush.o prom.o time.o int.o \
				   serial.o c-camelot.o gpio.o
obj-y += cml_eth.o cml_hnat.o cml_hnat_sdmz.o
obj-y += nf_hnat.o
# 175d drive
obj-y += 175d.o

ifeq "" "$(wildcard $(srctree)/$(src)/cml_hnat.c)"
$(obj)/cml_hnat.o: $(src)/cml_hnat.o.gz
	gzip -d -c $< > $@
endif

#obj-$(CONFIG_EARLY_PRINTK)	+=

EXTRA_CFLAGS += -Werror
#
# 	Uncomment to use Switch Port0 as WAN port
#
#EXTRA_CFLAGS += -DCML_ETH_WAN_P0
