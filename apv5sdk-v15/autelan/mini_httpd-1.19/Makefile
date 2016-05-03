include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mini-httpd
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/mini-httpd
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=app
  TITLE:=mini-httpd
  DESCRIPTION:=\
	a  httpd server
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC=$(TARGET_CC) -f Makefile.ap2010
endef

define Package/mini-httpd/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mini_httpd $(1)/sbin/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/mini_httpd.pem $(1)/sbin/
endef

$(eval $(call BuildPackage,mini-httpd))
