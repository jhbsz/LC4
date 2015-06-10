INTERFACE   := Interface/usb/
COMMON      := Common/
EXTRA_CFLAGS := -Idrivers/usb/drxvi314/Include

obj-$(CONFIG_USB_BECEEM_WIMAX) = drxvi314.o

drxvi314-objs := $(INTERFACE)InterfaceDld.o \
		$(INTERFACE)InterfaceIdleMode.o $(INTERFACE)InterfaceInit.o\
 		$(INTERFACE)InterfaceRx.o\
		$(INTERFACE)InterfaceIsr.o\
		$(INTERFACE)InterfaceMisc.o $(INTERFACE)InterfaceTx.o\
		$(COMMON)Arp.o $(COMMON)CmHost.o $(COMMON)Debug.o\
		$(COMMON)IPv6Protocol.o $(COMMON)Qos.o $(COMMON)Transmit.o\
		$(COMMON)Bcmnet.o $(COMMON)DDRInit.o $(COMMON)HandleControlPacket.o\
		$(COMMON)LeakyBucket.o $(COMMON)Misc.o $(COMMON)sort.o\
		$(COMMON)Bcmchar.o $(COMMON)hostmibs.o $(COMMON)PHSModule.o\
	 	$(COMMON)Osal_Misc.o $(COMMON)led_control.o $(COMMON)nvm.o $(COMMON)vendorspecificextn.o
