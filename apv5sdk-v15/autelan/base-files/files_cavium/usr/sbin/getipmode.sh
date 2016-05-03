. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
local xml_lan=`config_read /config/network/lan`
local ipmode=`config_getoption "$xml_lan" proto`
echo $ipmode
