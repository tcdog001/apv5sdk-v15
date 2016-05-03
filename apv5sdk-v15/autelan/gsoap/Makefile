
include $(TOPDIR)/rules.mk

PKG_NAME:=gsoap
PKG_VERSION:=2.7
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/gsoap/Default
	SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=lib
endef

define Package/gsoap
  $(call Package/gsoap/Default)
  TITLE:=A static library of RPC(remote process call)
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); \
		$(TARGET_CONFIGURE_OPTS) \
		CPPFLAGS="-I$(STAGING_DIR)/usr/include" \
		CFLAGS="-g -O2 -DWITH_OPENSSL -DHAVE_OPENSSL_SSL_H -I$(STAGING_DIR)/usr/include"  \
		./configure \
			--prefix="$(STAGING_DIR)/usr/$(PKG_NAME)" \
			--host=mips-linux \
	);
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		STAGING_DIR="$(STAGING_DIR)"
endef

define Build/InstallDev
	
endef

define Package/gsoap/install
	{ $(INSTALL_DIR) $(STAGING_DIR)/usr/include/gsoap \
	&& $(CP) $(PKG_BUILD_DIR)/soapcpp2/stdsoap2.h $(STAGING_DIR)/usr/include/gsoap/ \
	&& $(CP) $(PKG_BUILD_DIR)/soapcpp2/import $(STAGING_DIR)/usr/include/gsoap/  \
	&& $(CP) $(PKG_BUILD_DIR)/soapcpp2/*.a $(STAGING_DIR)/usr/lib ; \
	}
#	$(INSTALL_BIN) ./bin/* $(STAGING_DIR)/usr/bin
endef


$(eval $(call BuildPackage,gsoap))
