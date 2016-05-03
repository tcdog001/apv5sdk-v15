# 
# Copyright (C) 2006-2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 9907 2007-12-25 01:59:55Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=net-snmp
PKG_VERSION:=5.1.2
PKG_RELEASE:=2.3

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/net-snmp
PKG_MD5SUM:=8080555ab3f90011f25d5122042d9a8d

include $(INCLUDE_DIR)/package.mk

define Package/net-snmp/Default
  SECTION:=net
  CATEGORY:=Network
  URL:=http://www.net-snmp.org/
endef

define Package/net-snmp/Default/description
 Simple Network Management Protocol (SNMP) is a widely used protocol for 
 monitoring the health and welfare of network equipment (eg. routers), 
 computer equipment and even devices like UPSs. Net-SNMP is a suite of 
 applications used to implement SNMP v1, SNMP v2c and SNMP v3 using both 
 IPv4 and IPv6.
endef


define Package/libnetsnmp
$(call Package/net-snmp/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libelf
  TITLE:=Open source SNMP implementation (librairies)
endef

define Package/libnetsnmp/description
$(call Package/net-snmp/Default/description)
 .
 This package contains shared libraries, needed by other programs.
endef


define Package/snmp-utils
$(call Package/net-snmp/Default)
  DEPENDS:=+libnetsnmp
  TITLE:=Open source SNMP implementation (utilities)
endef

define Package/snmp-utils/description
$(call Package/net-snmp/Default/description)
 .
 This package contains SNMP client utilities.
endef


define Package/snmpd
$(call Package/net-snmp/Default)
  DEPENDS:=+libnetsnmp +aplib
  TITLE:=Open source SNMP implementation (daemon)
endef

define Package/snmpd/description
$(call Package/net-snmp/Default/description)
 .
 This package contains the SNMP agent, dynamically linked.
endef


define Package/snmpd-static
$(call Package/net-snmp/Default)
  TITLE:=Open source SNMP implementation (daemon)
endef

define Package/snmpd-static/description
$(call Package/net-snmp/Default/description)
 .
 This package contains the SNMP agent, statically linked.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	chmod 777 $(PKG_BUILD_DIR)/configure
endef


SNMP_MIB_MODULES_INCLUDED = \
	host/hr_device \
	host/hr_disk \
	host/hr_filesys \
	host/hr_network \
	host/hr_partition \
	host/hr_proc \
	host/hr_storage \
	host/hr_system \
	mibII/at \
	mibII/icmp \
	mibII/interfaces \
	mibII/ip \
	mibII/snmp_mib \
	mibII/sysORTable \
	mibII/system_mib \
	mibII/tcp \
	mibII/udp \
	mibII/vacm_context \
	mibII/vacm_vars \
	snmpv3/snmpEngine \
	snmpv3/snmpMPDStats \
	snmpv3/usmStats \
	snmpv3/usmUser \
	snmpv3mibs \
	tunnel \
	ucd-snmp/disk \
	ucd-snmp/dlmod \
	ucd-snmp/extensible \
	ucd-snmp/loadave \
	ucd-snmp/memory \
	ucd-snmp/pass \
	ucd-snmp/proc \
	ucd-snmp/vmstat \
	util_funcs \
	utilities/execute \
	iwncomm	\
	iwncomm/wireless/iwlib	\
	autelan/ieee802dot11 	\
	autelan/autelanAp	\
	autelan/accessAp	\
	autelan/sourceFile/apAttributeInfo	\
	autelan/sourceFile/apCounterInfo	\
	autelan/sourceFile/apGlobalInfo	\
	autelan/sourceFile/apNetworkInfo	\
	autelan/sourceFile/apPropertyTable	\
	autelan/sourceFile/apStatisticsTable	\
	autelan/sourceFile/apCommonInfo	\
	autelan/sourceFile/apWholeInfo	\
	autelan/sourceFile/apGeneralInfo	\
	autelan/sourceFile/apFileConfigInfo	\
	autelan/sourceFile/attackLogTable	\


SNMP_MIB_MODULES_EXCLUDED = \
	if-mib/ifTable \
	agent_mibs \
	agentx \
	host \
	mibII \
	ip-mib/data_access/ipaddress_linux \
        iwncomm/iwniofunc \
	notification \
	target \
	ucd_snmp \
	utilities \

SNMP_TRANSPORTS_INCLUDED = Callback UDP

SNMP_TRANSPORTS_EXCLUDED = TCP TCPv6 UDPv6 Unix

CONFIGURE_ARGS += \
	--enable-shared \
	--enable-static \
	--with-endianness=little \
	--with-logfile=/var/log/snmpd.log \
	--with-persistent-directory=/usr/lib/snmp/ \
	--with-default-snmp-version=1 \
	--with-sys-contact=root@localhost \
	--with-sys-location=Unknown \
	--enable-applications \
	--disable-debugging \
	--disable-ipv6 \
	--disable-manuals \
	--disable-mibs \
	--disable-scripts \
	--with-out-mib-modules="$(SNMP_MIB_MODULES_EXCLUDED)" \
	--with-mib-modules="$(SNMP_MIB_MODULES_INCLUDED)" \
	--with-out-transports="$(SNMP_TRANSPORTS_EXCLUDED)" \
	--with-transports="$(SNMP_TRANSPORTS_INCLUDED)" \
	--with-openssl="$(PKG_BUILD_DIR)/../openssl/src" \
	--without-libwrap \
	--without-rpm \
	--without-zlib \

CONFIGURE_VARS += \
	CPPFLAGS="-I$(STAGING_DIR)/usr/include" 

EXTRA_LDFLAGS = -L$(STAGING_DIR)/usr/lib -lapfunc


define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)" \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		all install
	( cd $(PKG_INSTALL_DIR); mv ./usr/sbin/snmpd ./usr/sbin/snmpd-shared; )
	#ifneq ($(CONFIG_PACKAGE_snmpd-static),)
	( cd $(PKG_BUILD_DIR); rm -f agent/snmpd; )
	$(MAKE) -C $(PKG_BUILD_DIR) \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS) -static" \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		all install
	( cd $(PKG_INSTALL_DIR); mv ./usr/sbin/snmpd ./usr/sbin/snmpd-static; )
	#endif
endef

define Build/InstallDev
	mkdir -p $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/net-snmp-config $(1)/usr/bin/
	mkdir -p $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/net-snmp $(1)/usr/include/
	mkdir -p $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnetsnmp{,agent,helpers,mibs}.{a,so*} $(1)/usr/lib/
	$(SED) 's,-I/usr/include,,g' $(1)/usr/bin/net-snmp-config
	$(SED) 's,-L/usr/lib,,g' $(1)/usr/bin/net-snmp-config
endef

define Package/libnetsnmp/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnetsnmp{,agent,helpers,mibs}.so.* $(1)/usr/lib/
endef

define Package/snmpd/conffiles
/etc/default/snmpd
/etc/snmp/snmpd.conf
endef

define Package/snmpd/install
	$(INSTALL_DIR) $(1)/etc/snmp
	$(INSTALL_DATA) ./files/snmpd.conf $(1)/etc/snmp/snmpd.conf
	$(INSTALL_DIR) $(1)/etc/default
	$(INSTALL_DATA) ./files/snmpd.default $(1)/etc/default/snmpd
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/snmpd.init $(1)/etc/init.d/snmpd
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/snmpd-shared $(1)/usr/sbin/snmpd
endef

define Package/snmpd-static/conffiles
/etc/default/snmpd
/etc/snmp/snmpd.conf
/etc/snmp/syslog.conf
endef

define Package/snmpd-static/install
	$(INSTALL_DIR) $(1)/etc/snmp
	$(INSTALL_DIR) $(1)/etc/wapi/config
	$(INSTALL_DATA) ./files/snmpd.conf $(1)/etc/snmp/snmpd.conf
	$(INSTALL_DATA) ./files/snmp/syslog.conf $(1)/etc/snmp/syslog.conf
	$(INSTALL_DATA) ./files/snmp/ac_snmp.conf $(1)/etc/snmp/ac_snmp.conf
	$(INSTALL_DATA) ./files/snmp/qos.conf $(1)/etc/snmp/qos.conf
	$(INSTALL_DATA) ./files/snmp/snmp_agent.conf $(1)/etc/snmp/snmp_agent.conf
	$(INSTALL_DATA) ./files/wapi/config/* $(1)/etc/wapi/config/
#start, pei add for 2010v2 snmp
	$(INSTALL_DIR) $(1)/etc/files.conf/snmp
	$(INSTALL_DIR) $(1)/etc/files.conf/wapi/config
	$(INSTALL_DATA) ./files/snmpd.conf $(1)/etc/files.conf/snmp/snmpd.conf
	$(INSTALL_DATA) ./files/snmp/syslog.conf $(1)/etc/files.conf/snmp/syslog.conf
	$(INSTALL_DATA) ./files/snmp/ac_snmp.conf $(1)/etc/files.conf/snmp/ac_snmp.conf
	$(INSTALL_DATA) ./files/snmp/qos.conf $(1)/etc/files.conf/snmp/qos.conf
	$(INSTALL_DATA) ./files/snmp/snmp_agent.conf $(1)/etc/files.conf/snmp/snmp_agent.conf
	$(INSTALL_DATA) ./files/wapi/config/* $(1)/etc/files.conf/wapi/config/
	mkdir -p /tmp/snmp
	mkdir -p /tmp/wapi/config
	cp -rf $(1)/etc/files.conf/snmp/* /tmp/snmp/
	cp -rf $(1)/etc/files.conf/wapi/config/ /tmp/wapi/config/
	ln -sf /tmp/snmp/snmpd.conf $(1)/etc/snmp/snmpd.conf
	ln -sf /tmp/snmp/snmp_agent.conf $(1)/etc/snmp/snmp_agent.conf
	ln -sf /tmp/snmp/syslog.conf $(1)/etc/snmp/syslog.conf
	ln -sf /tmp/snmp/ac_snmp.conf $(1)/etc/snmp/ac_snmp.conf
	ln -sf /tmp/snmp/qos.conf $(1)/etc/snmp/qos.conf
	ln -sf /tmp/wapi/config/ap_sysinfo.conf $(1)/etc/wapi/config/ap_sysinfo.conf
	ln -sf /tmp/wapi/config/basic.conf $(1)/etc/wapi/config/basic.conf
	ln -sf /tmp/wapi/config/ntp.conf $(1)/etc/wapi/config/ntp.conf
	ln -sf /tmp/wapi/config/wireless.conf $(1)/etc/wapi/config/wireless.conf
#end, pei add for 2010v2 snmp
	$(INSTALL_DIR) $(1)/etc/default
	$(INSTALL_DATA) ./files/snmpd.default $(1)/etc/default/snmpd
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/snmpd.init $(1)/etc/init.d/snmpd
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/snmpd-static $(1)/usr/sbin/snmpd
endef

define Package/snmp-utils/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/snmp{get,set,status,test,trap,walk} $(1)/usr/bin/
endef

$(eval $(call BuildPackage,libnetsnmp))
$(eval $(call BuildPackage,snmp-utils))
$(eval $(call BuildPackage,snmpd))
$(eval $(call BuildPackage,snmpd-static))
