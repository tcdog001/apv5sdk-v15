. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 local xml_config_autochannel=`config_read /config/network/intelligence/autochannel`
 xml_config_autochannel=`config_setoption "$xml_config_autochannel" interval "$1"`
config_write /config/network/intelligence/autochannel "$xml_config_autochannel"

