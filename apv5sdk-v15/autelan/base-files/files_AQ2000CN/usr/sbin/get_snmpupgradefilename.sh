. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
    local xml_upgrade=`config_read /config/administrator/snmp/eoc/upgradeserver`
    local xml_filename=`config_getoption "$xml_upgrade" filename`
    echo $xml_filename