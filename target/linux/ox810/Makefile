#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=ox810
BOARDNAME:=Oxford NAS 810
SUBTARGETS:=
LINUX_VERSION:=2.6.26

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += 

define Target/Description
	Build firmware images for Oxford NAS 810 based boards
endef
# kernel-default.mk does obj-copy with some parameters which don't work well here.
define Kernel/CompileImage
	$(if $(CONFIG_TARGET_ROOTFS_INITRAMFS),,rm -f $(TARGET_DIR)/init)
	+$(MAKE) $(KERNEL_MAKEOPTS) $(KERNELNAME)
	echo "$(KERNEL_CROSS)"
	$(KERNEL_CROSS)objcopy -O binary -R .note -R .note.gnu.build-id -R .comment -S $(LINUX_DIR)/vmlinux $(LINUX_KERNEL)
endef

$(eval $(call BuildTarget))
