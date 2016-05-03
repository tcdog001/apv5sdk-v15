. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/sysinit.sh
. /usr/lib/web/function.sh
local xml_config=`config_read /config/network/lan`
xml_config=`config_setoption "$xml_config" proto "$1"`
config_write /config/network/lan "$xml_config"
