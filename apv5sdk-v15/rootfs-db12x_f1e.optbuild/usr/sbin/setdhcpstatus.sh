. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
local xml_dhcp=`config_read /config/network/dhcp/dhcpserver`
xml_dhcp=`config_setoption "$xml_dhcp" enable "$1"`
config_write /config/network/dhcp/dhcpserver "$xml_dhcp"
