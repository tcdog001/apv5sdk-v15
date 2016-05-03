include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=openssl
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/openssl
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=app
  TITLE:=a module for encryption
  DESCRIPTION:=\
	a module for encryption
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src
endef

define Package/openssl/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/libcrypto.so.0.9.8 $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/libssl.so.0.9.8 $(1)/lib/
endef

$(eval $(call BuildPackage,openssl))
