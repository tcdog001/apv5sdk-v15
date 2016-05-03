. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
    local xml_lan=`config_read /config/network/lan`
    local xml_gateway=`config_getoption "$xml_lan" gateway`
    echo $xml_gateway