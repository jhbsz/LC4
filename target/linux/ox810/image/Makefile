#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
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

#define Image/BuildKernel
#	cp $(KDIR)/vmlinux $(VMLINUX).bin
#	$(STAGING_DIR_HOST)/bin/lzma e $(KDIR)/vmlinux -lc1 -lp2 -pb2 $(KDIR)/vmlinux.bin.l7
#	dd if=$(KDIR)/vmlinux.bin.l7 of=$(VMLINUX).lzma conv=sync
#endef

define Image/Build/squashfs
    $(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
endef

# Add deadc0de after mkimage is done.
# The firmware is also round-up of 64KB in terms of size.
define Image/Build
	$(call Image/Build/$(1))
	dd if=$(KDIR)/root.$(1) of=$(IMGNAME)-root.$(1) bs=64k conv=sync

	mkimage -A arm -O linux -T kernel -a 0x48008000 -C none -e \
		0x48008000 \
		-n 'EZPacket ARM Linux-$(LINUX_VERSION)' \
		-d $(KDIR)/vmlinux 
		
#	mkimage -A arm -O linux -T kernel -a 0x48008000 -C lzma -e \
#		0x48008000 -p \
#		-n 'EZPacket ARM Linux-$(LINUX_VERSION)' \
#		-d $(VMLINUX).lzma \
#		-c $(CONFIG_EZP_PRODUCT_CATEGORY) -s $(CONFIG_EZP_PRODUCT_SUBCATEGORY) -w $(EZP_VERSION_MAJOR) -y $(EZP_VERSION_MINOR) -z $(EZP_VERSION_SUBMINOR) -o 0xE0000 -f $(KDIR)/root.squashfs $(IMGNAME)-$(KERNEL)-$(1).bix
#	$(call add_jffs2_mark,$(IMGNAME)-$(KERNEL)-$(1).bix)
#	mv $(IMGNAME)-$(KERNEL)-$(1).bix $(IMGNAME)-$(KERNEL)-$(1)-deadc0de.bix
#	dd if=$(IMGNAME)-$(KERNEL)-$(1)-deadc0de.bix of=$(IMGNAME)-$(KERNEL)-$(1).bix bs=64k conv=sync
#	rm -rf $(IMGNAME)-$(KERNEL)-$(1)-deadc0de.bix
endef


$(eval $(call BuildImage))
