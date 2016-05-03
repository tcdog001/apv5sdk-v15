include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=hostapd0.6.6
PKG_RELEASE:=1

PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/hostapd0.6.6
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=app
  DEPENDS:=kmod-madwifi-trunk-r3314-wapi
  TITLE:=authenticator for wpa and 802.1x
  DESCRIPTION:=\
	authenticator for wpa and 802.1x
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./hostapd-0.6.6/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/hostapd \
	$(TARGET_CONFIGURE_OPTS) \
	LIBS="$(TARGET_LDFLAGS) -L$(STAGING_DIR)/usr/lib " \
	MADWIFI_H=$(KERNEL_BUILD_DIR)/madwifi-trunk-r3314-wapi/ \
	SSL_H=$(STAGING_DIR)/usr/include/ \
	TLS_FUNCS=1  \
	CONFIG_TLS="internal" \
	CONFIG_INTERNAL_LIBTOMMATH=1 \
		hostapd hostapd_cli
endef

define Package/hostapd0.6.6/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hostapd/hostapd $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,hostapd0.6.6))
