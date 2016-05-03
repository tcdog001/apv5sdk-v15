include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=xmlconf
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/xmlconf
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=app
  TITLE:=xmlconf support
  DEPENDS:=+libxml
  DESCRIPTION:=\
	a  moudle for xmlconf service
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC=$(TARGET_CC) 
endef

define Package/xmlconf/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/xmlconf $(1)/usr/bin/
endef

$(eval $(call BuildPackage,xmlconf))
