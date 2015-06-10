define KernelPackage/rt2860v2_ap
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RT3052 AP support
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rt2860v2_ap/rt2860v2_ap.$(LINUX_KMOD_SUFFIX)
#  AUTOLOAD:=$(call AutoLoad,60,rt2860v2_ap)
endef
define KernelPackage/rt2860v2_ap/description
 Kernel modules for RT305X AP wireless support
endef
$(eval $(call KernelPackage,rt2860v2_ap))

define KernelPackage/raeth
  SUBMENU:=$(NETWORK_DEVICES_MENU)
  TITLE:=RT305X Ethernet support
  FILES:=$(LINUX_DIR)/drivers/net/raeth/raeth.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,60,raeth)
endef
define KernelPackage/raeth/description
 Kernel modules for RT305X ethernet support
endef
$(eval $(call KernelPackage,raeth))

define KernelPackage/rt2860v2_sta
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=RT3052 STA support
  FILES:=$(LINUX_DIR)/drivers/net/wireless/rt2860v2_sta/rt2860v2_sta.$(LINUX_KMOD_SUFFIX)
#  AUTOLOAD:=$(call AutoLoad,60,rt2860v2_sta)
endef
define KernelPackage/rt2860v2_sta/description
 Kernel modules for RT305X STA wireless support
endef
$(eval $(call KernelPackage,rt2860v2_sta))

define KernelPackage/dwc_otg
  SUBMENU:=$(USB_MENU)
  TITLE:=RT305X DWC OTG
  FILES:=$(LINUX_DIR)/drivers/usb/dwc_otg/dwc_otg.$(LINUX_KMOD_SUFFIX) \
    $(LINUX_DIR)/arch/mips/rt2880/lm.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,70,lm dwc_otg)
endef
define KernelPackage/dwc_otg/description
 Kernel modules for RT305X DWC OTG
endef
$(eval $(call KernelPackage,dwc_otg))
