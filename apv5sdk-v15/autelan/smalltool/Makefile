include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=smalltool
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/smalltool
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=app
  TITLE:=some small tool
  DESCRIPTION:=\
	small tool running in ap
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile/Default
	echo "hi"
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/ CC=$(TARGET_CC)
endef

define Package/smalltool/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rate-stat $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mydaemon $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/packet $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/loopback $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cpu_util_rate $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cpu_util_check $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wtpd_alarm $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/capture $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/myping $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wtpd_alarm_all $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/iwlist_scan/iwlist_scan $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,smalltool))
