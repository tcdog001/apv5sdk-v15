# Script to install net-snmpd package.

PKG_NAME="net-snmp-5.4.2.1.tar.gz"
PKG_DIR="net-snmp-5.4.2.1"

SYS_CONTACT="nobody@example.com"
SYS_LOCATION="Nowhere"

PREFIX="/usr/local/share/snmp/"
SNMPD_CFG="${PREFIX}snmpd.conf"
OUR_CFG="snmpd-example.conf"

if [ ! -f ${PKG_NAME} ]; then
	echo Missing ${PKG_NAME}.
	echo
	echo Download this file into the int6000-utils snmp directory
	echo from http://net-snmp.sourceforge.net/ .
	echo
	exit 1
fi

rm -rf ${PKG_DIR}
tar -xzf ${PKG_NAME}

if [ ${?} -ne 0 ]; then
	echo Error extracting ${PKG_NAME}.
	exit 1
fi

cd ${PKG_DIR}

./configure \
	--with-logfile="/var/log/snmpd.log" \
	--with-default-snmp-version="2" \
	--with-persistent-directory="/var/net-snmp" \
	--with-sys-contact=${SYS_CONTACT} \
	--with-sys-location=${SYS_LOCATION} \
	--disable-embedded-perl \

#	--prefix=PREFIX \
#	--exec-prefix=EPREFIX \
#	--bindir=DIR \
#	--sbindir=DIR \
#	--libexecdir=DIR \
#	--datadir=DIR \
#	--sysconfdir=DIR \
#	--sharedstatedir=DIR \
#	--localstatedir=DIR \
#	--libdir=DIR \
#	--includedir=DIR \
#	--oldincludedir=DIR \
#	--infodir=DIR \
#	--mandir=DIR \
#	--build=BUILD \
#	--host=HOST \
#	--target=TARGET \
#	--disable-FEATURE \
#	--enable-FEATURE[=ARG] \
#	--enable-as-needed \
#	--enable-silent-libtool \
#	--enable-mini-agent \
#	--disable-agent \
#	--disable-applications \
#	--disable-manuals \
#	--disable-scripts \
#	--disable-mibs \
#	--disable-mib-loading \
#	--disable-snmpv1 \
#	--disable-snmpv2c \
#	--disable-set-support \
#	--disable-des \
#	--disable-privacy \
#	--disable-md5 \
#	--enable-internal-md5 \
#	--enable-ipv6 \
#	--enable-local-smux \
#	--enable-debugging \
#	--disable-debugging \
#	--enable-developer \
#	--enable-agentx-dom-sock-only \
#	--disable-snmptrapd-subagent \
#	--enable-mib-config-checking \
#	--enable-mib-config-debug \
#	--enable-mfd-rewrites \
#	--enable-testing-code \
#	--enable-reentrant \
#	--disable-perl-cc-checks \
#	--enable-ucd-snmp-compatibility \
#	--enable-shared[=PKGS] \
#	--enable-static[=PKGS] \
#	--enable-fast-install[=PKGS] \
#	--disable-libtool-lock \
#	--with-PACKAGE[=ARG] \
#	--without-PACKAGE \
#	--with-cc=CC \
#	--with-linkcc=CC \
#	--with-ar=AR \
#	--with-endianness=big|little \
#	--with-cflags=CFLAGS \
#	--with-ldflags=LDFLAGS \
#	--with-libs=LIBS \
#	--with-install-prefix=PATH \
#	--with-dmalloc=PATH \
#	--with-efence \
#	--with-rsaref=PATH \
#	--with-openssl=PATH \
#	--with-krb5=PATH \
#	--without-rpm \
#	--without-opaque-special-types \
#	--with-mibdirs="dir1:dir2:" \
#	--with-mibs="item1:item2:" \
#	--with-mibfiles="file1:file2" \
#	--with-pkcs=PATH \
#	--without-root-access \
#	--without-kmem-usage \
#	--with-dummy-values \
#	--with-persistent-mask="mask" \
#	--with-copy-persistent-files="no" \
#	--with-temp-file-pattern=PATTERN \
#	--with-agentx-socket=FILE \
#	--with-transports="t1 t2 ..." \
#	--with-out-transports="t1 ..." \
#	--with-security-modules="s1 s2 ..." \
#	--with-mib-modules="item1 ..." \
#	--with-out-mib-modules="list" \
#	--with-enterprise-oid \
#	--with-enterprise-sysoid \
#	--with-enterprise-notification-oid \
#	--with-perl-modules=ARGS \
#	--with-python-modules=ARGS \
#	--with-server-send-buf=ARG \
#	--with-server-recv-buf=ARG \
#	--with-client-send-buf=ARG \
#	--with-client-recv-buf=ARG \
#	--with-defaults \
#	--with-gnu-ld \
#	--with-pic \
#	--with-tags[=TAGS] \
#	--without-elf \
#	--with-libwrap=LIBPATH \
#	--with-zlib=DIR \
#	--with-bzip2=DIR \

if [ ${?} -ne 0 ]; then
	echo Configure failed.
	exit 1
fi

make
if [ ${?} -ne 0 ]; then
	echo Build failed.
	exit 1
fi

sudo make install
if [ ${?} -ne 0 ]; then
	echo Install failed.
	exit 1
fi

sudo ldconfig

if [ ! -f ${SNMPD_CFG} ]; then
	echo
	echo ------------------------------------------------------
	echo No snmpd.conf file is currently installed.
	echo Install one into ${SNMPD_CFG}
	echo before starting snmpd.
	echo
	echo For the Intellon subagent to function, the snmp daemon
	echo must be configured to enable 'agentx' support. This is
	echo accomplished by adding the line:
	echo
	echo "    master agentx"
	echo
	echo to ${SNMPD_CFG}.
	echo
	echo You will also need to configure other basic snmpd
	echo options in this file.
	echo
	echo Run \"man snmpd.conf\" for more details.
	echo ------------------------------------------------------
	echo
fi
