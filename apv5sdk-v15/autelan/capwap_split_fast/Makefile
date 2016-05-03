
#
# Copyright (C) 2008 autelan.com
#
# this module for trasmit packet of the capwap data channel in aplide mac mode
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=capwap_split_fast
PKG_RELEASE:=1

PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
ifeq ($(DUMP),)
	include $(LINUX_DIR)/.config
endif

define KernelPackage/capwapsplidefast
	SECTION:=autelan
	CATEGORY:=Autelan
	SUBMENU:=modules
    	TITLE:=capwap split fast
	DEPENDS:=@LINUX_2_6
    	DESCRIPTION:=\
		this module for trasmit packet of the capwap data channel in aplide mac mode
	VERSION:=$(PKG_RELEASE)+$(LINUX_VERSION)-$(BOARD)-$(LINUX_RELEASE)
	FILES:= \
		$(PKG_BUILD_DIR)/capwap_split_fast.$(LINUX_KMOD_SUFFIX)
	AUTOLOAD:=$(call AutoLoad,60,capwapsplidefast)
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	echo "----------------------------------"
	$(MAKE) -C $(PKG_BUILD_DIR)
endef
#define Build/InstallDev
#	echo "install"

#endef
#define Build/UninstallDev
#	echo "unstall"
#endef
$(eval $(call KernelPackage,capwapsplidefast))
