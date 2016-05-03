. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 local xml_config_system=`config_read /config/system`
 xml_config_system=`config_setoption "$xml_config_system" timeserver1 "$1"`
 config_write /config/system "$xml_config_system"

