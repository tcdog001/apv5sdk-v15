. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
local xml_dhcp=`config_read /config/network/dhcp/dhcpserver`
local xml_dhcp_leasetime=`config_getoption "$xml_dhcp" leasetime`
echo $xml_dhcp_leasetime
