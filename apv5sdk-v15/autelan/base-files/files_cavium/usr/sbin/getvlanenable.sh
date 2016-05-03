. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 xml_config_vlan=`config_read /config/network/vap$1/vlan`
 xml_wlan_vlan_enable=`config_getoption "$xml_config_vlan" enable`
  
echo $xml_wlan_vlan_enable
