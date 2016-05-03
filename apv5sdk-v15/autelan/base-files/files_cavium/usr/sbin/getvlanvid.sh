. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 xml_config_vlan=`config_read /config/network/vap$1/vlan`
 local xml_vap_vlanid=`config_getoption "$xml_config_vlan" vid`  
 echo $xml_vap_vlanid
