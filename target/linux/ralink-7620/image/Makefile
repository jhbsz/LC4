#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
include $(INCLUDE_DIR)/target.mk

IMGNAME:=$(BIN_DIR)/ezp-$(BOARD)

define imgname
$(IMGNAME)-$(2)-$(patsubst jffs2-%,jffs2,$(1))
endef

define add_jffs2_mark
	echo -ne '\xde\xad\xc0\xde' >> $(1)
endef

VMLINUX:=$(IMGNAME)-vmlinux
ifeq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),y)
	VMLINUX:=$(IMGNAME)-vmlinux-initramfs
endif

ifeq ($(CONFIG_FLASH_128K_SECTOR),y)
	KERNEL_SECTOR:=128k
	ROOTFS_OFFSET:=0x160000
else
	KERNEL_SECTOR:=64k
	ROOTFS_OFFSET:=0x1D0000
endif

define Image/BuildKernel
	cp $(KDIR)/vmlinux $(VMLINUX).bin
	$(STAGING_DIR_HOST)/bin/lzma e $(KDIR)/vmlinux -lc1 -lp2 -pb2 $(KDIR)/vmlinux.bin.l7
	dd if=$(KDIR)/vmlinux.bin.l7 of=$(VMLINUX).lzma conv=sync
endef

define Image/Build/squashfs
    $(call prepare_generic_squashfs,$(KDIR)/root.squashfs,$(KERNEL_SECTOR))
endef

# Add deadc0de after mkimage is done.
# The firmware is also round-up of 64KB in terms of size.
define Image/Build
	$(call Image/Build/$(1))
	dd if=$(KDIR)/root.$(1) of=$(IMGNAME)-root.$(1) bs=$(KERNEL_SECTOR) conv=sync
	if [ -n $(CONFIG_EZP_PRODUCT_SUPERCATEGORY) ]; then \
		mkimage -A mips -O linux -T kernel -a 0x80000000 -C lzma \
			-e $(shell readelf -h $(LINUX_DIR)/vmlinux | grep "Entry" | awk '{print $$4}') -p \
			-n 'EZPacket MIPS Linux-$(LINUX_VERSION)&$(call qstrip,$(CONFIG_TARGET_PLATFORM))' \
			-d $(VMLINUX).lzma \
			-c $(CONFIG_EZP_PRODUCT_SUPERCATEGORY) -s $(CONFIG_EZP_PRODUCT_SUBCATEGORY) \
			-w $(EZP_VERSION_MAJOR) -y $(EZP_VERSION_MINOR) \
			-z $(EZP_VERSION_SUBMINOR) -o $(ROOTFS_OFFSET) \
			-f $(KDIR)/root.squashfs $(IMGNAME)-$(KERNEL)-$(1).bix ; \
	else \
		mkimage -A mips -O linux -T kernel -a 0x80000000 -C lzma \
			-e $(shell readelf -h $(LINUX_DIR)/vmlinux | grep "Entry" | awk '{print $$4}') -p \
			-n 'EZPacket MIPS Linux-$(LINUX_VERSION)&$(call qstrip,$(CONFIG_TARGET_PLATFORM))' \
			-d $(VMLINUX).lzma \
			-c $(CONFIG_EZP_PRODUCT_CATEGORY) -s $(CONFIG_EZP_PRODUCT_SUBCATEGORY) \
			-w $(EZP_VERSION_MAJOR) -y $(EZP_VERSION_MINOR) \
			-z $(EZP_VERSION_SUBMINOR) -o $(ROOTFS_OFFSET) \
			-f $(KDIR)/root.squashfs $(IMGNAME)-$(KERNEL)-$(1).bix ; \
	fi
	$(call add_jffs2_mark,$(IMGNAME)-$(KERNEL)-$(1).bix)
	mv $(IMGNAME)-$(KERNEL)-$(1).bix $(IMGNAME)-$(KERNEL)-$(1)-deadc0de.bix
	dd if=$(IMGNAME)-$(KERNEL)-$(1)-deadc0de.bix of=$(IMGNAME)-$(KERNEL)-$(1).bix bs=$(KERNEL_SECTOR) conv=sync
	rm -rf $(IMGNAME)-$(KERNEL)-$(1)-deadc0de.bix
endef


$(eval $(call BuildImage))
