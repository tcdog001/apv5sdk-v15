#corporation info
corporation=Autelan

#product info
BMUTIL=/usr/bin/pro_ctl_util
SHOWSYSINFO=/usr/sbin/showsysinfo
corporation=`$SHOWSYSINFO 2>/dev/null|awk -F':' '/Company Name/{print $2}' 2>/dev/null`
[ -z "$corporation" ]&&corporation="Autelan"
HOST_NAME="${corporation:-Autelan}"
productType=`$SHOWSYSINFO 2>/dev/null|awk -F':' '/Device Type/{print $2}' 2>/dev/null|cut -b 1-6 2>/dev/null`
IMG_HTML="image/logo.jpg"
if [ "$productType" == "AP2600" ];then
       IMG_HTML="image/xj-AP2600.jpg"
fi
if [ "$productType" == "AP2400" ];then
       IMG_HTML="image/xj-AP2400.jpg"
fi
if [ "$productType" == "FH-AP2" ];then
	IMG_HTML="image/xh.jpg"
fi	
#CORPORATION_NAME=`echo $corporation|grep  -o -e 'Autelan'`
#[ -z "$CORPORATION_NAME" ]&&CORPORATION_NAME=`echo $corporation|grep  -o -e 'Topshine'`
#[ -z "$CORPORATION_NAME" ]&&CORPORATION_NAME="Autelan"
#HOST_NAME="${CORPORATION_NAME:-Autelan}"

DEVICE_MODEL=`$BMUTIL -i 2>/dev/null | awk -F ":" '/product type is/{print $2}'`

#device info
DEVICE_INFO_XML=/tmp/device_info.xml
XML_GET_DATA_BY_OPTION_AWK=/usr/lib/web/xml_get_data_by_option.awk
DEV_NAME=`cat $DEVICE_INFO_XML 2>/dev/null | grep -i -o -e  'AQ[0-9]\{1,4\}'|grep -o -e '[0-9]\{1,4\}'`
DEV_wifi0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi0 option2=status`
DEVMODE_wifi0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi0 option2=mode`
DEVPOWER_wifi0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi0 option2=max_power`
DEVPOWEROFF_wifi0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi0 option2=limit`
DEV_wifi1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi1 option2=status`
DEVMODE_wifi1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi1 option2=mode`
DEVPOWER_wifi1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi1 option2=max_power`
DEVPOWEROFF_wifi1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi1 option2=limit`
DEV_eth0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=if0 option2=status`
DEV_eth1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=if1 option2=status`
WAPI_ENABLE=enable
if [ "$DEV_wifi1" == "enable" ];then
       NUM_ap="12"
else
       NUM_ap="6"
fi

#common cgi
login_cgi="login.cgi"
logout_cgi="logout.cgi"
upgrade_cgi="upgrade.cgi"
action_cgi="action.cgi"


#common files
FIRMWARE_XML=/etc/config/firmware.xml
CONFIG_XML=/etc/config/config.xml
DEFAULT_CONFIG_XML=/etc/defaults/config.xml
SESSION_XML=/tmp/session.xml
UPGRADE_XML=/tmp/upgrade.xml
ACCONFIG_XML=/etc/config/acconfig.xml
MESSAGES0_LOG=/var/log/messages.0
MESSAGES_LOG=/var/log/messages
DEFAULT_SESSION_XML=/etc/defaults/session.xml
DATA_CONF=/usr/bin/data.conf
LOCAL_CONF=/usr/bin/local.conf
CWMP_CONF=/usr/bin/cwmp.conf
TR069_CONF=/usr/bin/tr-069.conf
UDHCPD_CONF=/etc/udhcpd.conf
DNSMASQ_CONF=/etc/dnsmasq.conf
DHCPSNOOPING=/usr/sbin/dhcpsnooping
DNSMASQ=/etc/init.d/dnsmasq
OPTIONS=/etc/ppp/options
PAP_SECRETS=/etc/ppp/pap-secrets
CHAP_SECRETS=/etc/ppp/chap-secrets

#common commands
SETTXPOWER=/usr/sbin/settxpower2
IFCONFIG=/sbin/ifconfig
ROUTE=/sbin/route
WLANCONFIG=/usr/sbin/wlanconfig
IWCONFIG=/usr/sbin/iwconfig
BRCTL=/usr/sbin/brctl
UDHCPC=/sbin/udhcpc
IWPRIV=/usr/sbin/iwpriv
HOSTAPD=/usr/sbin/hostapd
NTPCLIENT=/usr/sbin/ntpclient
VCONFIG=/sbin/vconfig
SYSLOGD=/sbin/syslogd
SNMPD=/usr/sbin/snmpd
KILLALL=/usr/bin/killall
LOGGER=/usr/bin/logger
SYSCTL=/sbin/sysctl
DATAMODEL=/usr/bin/datamodel
LOCALMANAGER=/usr/bin/LocalManager
CWMP=/usr/bin/cwmp
TELNETD=/usr/sbin/telnetd
DROPBEAR=/etc/init.d/dropbear
UDHCPD=/usr/sbin/udhcpd
AUTO_CHANNEL=/usr/sbin/auto_channel
AUTO_POWER=/usr/sbin/auto_power
SET_CRONTAB=/usr/sbin/set_crontab
PERTRAP=/usr/sbin/pertrap
LINK_DETECT=/usr/sbin/link_detect
CROND=/usr/sbin/crond
SEND_TRAP=/usr/sbin/send_trap
MYDAEMON=/usr/sbin/mydaemon
START_TR069=/usr/sbin/start_tr69
STOP_TR069=/usr/sbin/stop_tr69
IPTABLES=/usr/bin/iptables
HTPASSWD=/usr/sbin/htpasswd
PPPD=/usr/sbin/pppd
WPA_SUPPLICANT=/usr/sbin/wpa_supplicant
APLOCATOR=/usr/sbin/aplocator_enable
WAPISET=/usr/sbin/wapiset
WAPID=/usr/sbin/wapid
CERT_UNITE=/usr/sbin/cert_unite
UPLINK_DETECT=/usr/sbin/uplink_detect
SETANTENNA=/usr/sbin/setantenna
BRLAN=default
SYSFACTORY_RESET=/sbin/sysfactoryreset

EBTABLES=/usr/sbin/ebtables
AUTELAN=/usr/sbin/autelan
SLEEP=/bin/sleep
IWLIST=/usr/sbin/iwlist
CRYPT=/sbin/crypt
AWK=/usr/bin/awk
GREP=/bin/grep
SED=/bin/sed
