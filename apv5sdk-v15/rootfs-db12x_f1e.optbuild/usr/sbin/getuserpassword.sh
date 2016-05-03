. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
    local xml_config=`config_read /config/system`
    local xml_username=`config_getoption "$xml_config" username`
    local xml_password=`config_getoption "$xml_config" password`
    echo $xml_username 
    echo $xml_password
