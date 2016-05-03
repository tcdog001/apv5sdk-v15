#corporation info
corporation=Autelan

#product info
BMUTIL=/usr/bin/bmutil
corporation=`$BMUTIL -p 2>/dev/null|awk -F':' '/vendor name/{print $2}' 2>/dev/null`
[ -z "$corporation" ]&&corporation="Autelan"
CORPORATION_NAME=`echo $corporation|grep  -o -e 'Autelan'`
[ -z "$CORPORATION_NAME" ]&&CORPORATION_NAME=`echo $corporation|grep  -o -e 'Topshine'`
[ -z "$CORPORATION_NAME" ]&&CORPORATION_NAME="Autelan"
LOGO_IMG="${CORPORATION_NAME:-Autelan}-logo.jpg"
HOST_NAME="${CORPORATION_NAME:-Autelan}"
IMG_HTML="<img src=\"image/$LOGO_IMG\">"
#device info
DEVICE_INFO_XML=/tmp/device_info.xml
XML_GET_DATA_BY_OPTION_AWK=/usr/lib/web/xml_get_data_by_option.awk
DEV_NAME=`cat $DEVICE_INFO_XML 2>/dev/null | grep -i -o -e  'AQ[0-9]\{1,4\}'|grep -o -e '[0-9]\{1,4\}'`
DEV_wifi0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi0 option2=status`
DEVMODE_wifi0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi0 option2=mode`
DEVPOWER_wifi0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi0 option2=max_power`
DEV_wifi1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi1 option2=status`
DEVMODE_wifi1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi1 option2=mode`
DEVPOWER_wifi1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=wifi1 option2=max_power`
DEV_eth0=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=if0 option2=status`
DEV_eth1=`cat $DEVICE_INFO_XML 2>/dev/null | awk -f $XML_GET_DATA_BY_OPTION_AWK  option1=if1 option2=status`
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
firewall_action_cgi="firewall_action.cgi"


#common files
FIRMWARE_XML=/etc/config/firmware.xml
CONFIG_XML=/etc/config/config.xml
DEFAULT_CONFIG_XML=/etc/defaults/config.xml
SESSION_XML=/tmp/session.xml
UPGRADE_XML=/etc/defaults/upgrade.xml
ACCONFIG_XML=/etc/config/acconfig.xml
MESSAGES0_LOG=/var/log/messages.0
MESSAGES_LOG=/var/log/messages
DEFAULT_SESSION_XML=/etc/defaults/session.xml
DATA_CONF=/usr/bin/data.conf
LOCAL_CONF=/usr/bin/local.conf
CWMP_CONF=/usr/bin/cwmp.conf
TR069_CONF=/usr/bin/tr-069.conf
UDHCPD_CONF=/etc/udhcpd.conf
DHCPSNOOPING=/usr/sbin/dhcpsnooping
DNSMASQ_CONF=/etc/dnsmasq.conf
DNSMASQ=/etc/init.d/dnsmasq
OPTIONS=/etc/ppp/options
PAP_SECRETS=/etc/ppp/pap-secrets
CHAP_SECRETS=/etc/ppp/chap-secrets

#common commands
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
WPA_SUPPLICANT=/usr/sbin/wpa_supplicant
HTPASSWD=/usr/sbin/htpasswd
PPPD=/usr/sbin/pppd
IPTABLES=/usr/bin/iptables
BRLAN=default
WLWAN=ath1
INTF_NAME=eth0

EBTABLES=/usr/sbin/ebtables
AUTELAN=/usr/sbin/autelan
SLEEP=/bin/sleep
IWLIST=/usr/sbin/iwlist
CRYPT=/sbin/crypt
AWK=/usr/bin/awk
GREP=/bin/grep
SED=/bin/sed
