include $(TOPDIR)/rules.mk

PKG_NAME:=aplib
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/aplib
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=lib
  DEPENDS:=+libxml
  TITLE:=ap function support
  DESCRIPTION:=\
	a  shared library for ap 
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		STAGING_DIR=$(STAGING_DIR) \
		CC=$(TARGET_CC) \
		AR=$(AR)
endef

define Package/aplib/install
	$(CP) $(PKG_BUILD_DIR)/include/* $(STAGING_DIR)/usr/include/ 
	mkdir -p $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/*.so $(STAGING_DIR)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/*.so $(1)/usr/lib
endef

$(eval $(call BuildPackage,aplib))
