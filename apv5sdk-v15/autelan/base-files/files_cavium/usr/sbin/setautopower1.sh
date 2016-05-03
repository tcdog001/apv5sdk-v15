. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 local xml_config_autopower=`config_read /config/network/intelligence/autopower`
 xml_config_autopower=`config_setoption "$xml_config_autopower" power_wifi0 "$1"`
 config_write /config/network/intelligence/autopower "$xml_config_autopower"


