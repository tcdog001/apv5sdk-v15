include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=webui
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/webui
  SECTION:=autelan
  CATEGORY:=Autelan
  SUBMENU:=app
  TITLE:=web management interface
  DEPENDS:=+xmlconf +mini-httpd +haserl +crypttool
  DESCRIPTION:=\
	web management interface
endef

define Build/Compile/Default
	echo "hi"
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

define Package/webui/install
	$(CP) ./files/* $(1)
	$(INSTALL_DIR) $(1)/etc/files.conf
	$(CP) $(1)/www/image/logo.jpg $(1)/etc/files.conf/
	mkdir -p /tmp/logo/
	mv $(1)/www/image/logo.jpg /tmp/logo/
	ln -sf /tmp/logo/logo.jpg $(1)/www/image/logo.jpg
endef


$(eval $(call BuildPackage,webui))
