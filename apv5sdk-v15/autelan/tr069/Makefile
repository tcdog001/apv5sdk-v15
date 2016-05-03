
include $(TOPDIR)/rules.mk

PKG_NAME:=tr069
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/tr069/Default
	SECTION:=autelan
	CATEGORY:=Autelan
	SUBMENU:=app
	DEPENDS:=+gsoap
endef

define Package/tr069
  $(call Package/tr069/Default)
  TITLE:= TR069 implementation
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		STAGING_DIR="$(STAGING_DIR)" \
		BUILD_DIR="$(BUILD_DIR)" \
		TARGET_CC="$(TARGET_CC)" \
		TARGET_LD="$(TARGET_CROSS)ld"
endef

define Build/InstallDev
	
endef

define Package/tr069/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cwmp $(1)/usr/bin 
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/datamodel $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/LocalManager $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/DownloadManager  $(1)/usr/bin
	$(INSTALL_DATA) ./files/* $(1)/usr/bin
endef


$(eval $(call BuildPackage,tr069))
