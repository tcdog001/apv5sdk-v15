include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=dhcpsnooping
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/dhcpsnooping
	SECTION:=autelan
	CATEGORY:=Autelan
	SUBMENU:=app
	TITLE:=a tool to snooping dhcp packages
	DESCRIPTION:=\
		a tool to snooping dhcp packages
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)  CC=$(TARGET_CC) 
endef

define Package/dhcpsnooping/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dhcpsnooping $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,dhcpsnooping))
