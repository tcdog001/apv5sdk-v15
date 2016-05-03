include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=wtpd
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
EXTRA_CFLAGS= -DAQ2010
include $(INCLUDE_DIR)/package.mk

define Package/wtpd
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=app
  TITLE:=capwap client program 
  DESCRIPTION:=\
	capwap client client running in ap
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
#	./prepare.sh $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile/Default
	echo "hi"
endef

define Build/Compile

	$(MAKE) -C $(PKG_BUILD_DIR)/wcpss/src/app/wtpd/ CC=$(TARGET_CC) AR=$(TARGET_CROSS)ar EXTRA_CFLAGS="$(EXTRA_CFLAGS)" 
endef

define Package/wtpd/install
	$(INSTALL_DIR) $(1)/sbin
#	$(INSTALL_DIR) $(1)/etc/wlan
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wcpss/src/app/wtpd/wtpd $(1)/sbin/
#	$(INSTALL_DATA) $(PKG_BUILD_DIR)/wcpss/src/res/config.wtp $(1)/etc/wlan
endef

$(eval $(call BuildPackage,wtpd))
