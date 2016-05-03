. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 xml_config_autochannel=`config_read /config/network/intelligence/autochannel`
 local xml_autochannel_interval=`config_getoption "$xml_config_autochannel" interval`
 echo $xml_autochannel_interval

