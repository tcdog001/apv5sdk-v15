include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=libxml
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/libxml
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=lib
  TITLE:=libxml support
  DESCRIPTION:=\
	lib for xml
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC=$(TARGET_CC) 
endef

define Build/InstallDev
endef
define Package/libxml/install
	mkdir -p $(STAGING_DIR)/usr/include/libmxml
	mkdir -p $(STAGING_DIR)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/lib/*.a $(STAGING_DIR)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/include/*.h $(STAGING_DIR)/usr/include/libmxml
endef

$(eval $(call BuildPackage,libxml))
