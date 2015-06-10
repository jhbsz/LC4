#############################################################
# Makefile for WSC upnp module
#############################################################

CFLAGS +=  -I$(STAGING_DIR)/usr/include/upnp -I$(STAGING_DIR)/usr/include/upnp/ixml/ -I$(STAGING_DIR)/usr/include/upnp/threadutil -I$(STAGING_DIR)/usr/include/upnp/inc

# For some unknow reason, we can not use dynamic link at libupnp.
LIBUPNP = $(STAGING_DIR)/usr/lib/libupnp.a
LDFLAGS += -L$(STAGING_DIR)/usr/lib -lixml -lthreadutil -lpthread 

OBJS = wsc_main.o wsc_common.o wsc_ioctl.o wsc_netlink.o wsc_upnp_device.o wsc_upnp_cp.o sample_util.o wsc_msg.o $(LIBUPNP)

EXEC = wscd

CFLAGS +=  -O2 -Wall -D_REENTRANT -DMULTIPLE_CARD_SUPPORT

all: $(EXEC)


$(EXEC):  $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o *~ *.bak $(EXEC)

cleanall: clean
	rm -f *.a
