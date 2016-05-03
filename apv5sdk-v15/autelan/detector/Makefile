include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=detector
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/detector
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=app
  TITLE:=a tool to get the sta location
  DESCRIPTION:=\
	a tool to use detector to find the location of sta
endef

define Build/Prepare
#gengzongjie added for rfid scan
	rm -rf $(PKG_BUILD_DIR)
#gengzongjie added end 

	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)"
endef

define Package/detector/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/detector $(1)/usr/bin
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nvram $(1)/usr/sbin
endef

$(eval $(call BuildPackage,detector))
