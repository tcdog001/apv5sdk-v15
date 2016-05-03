. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 xml_config_autopower=`config_read /config/network/intelligence/autopower`
 local xml_autopower_power_wifi0=`config_getoption "$xml_config_autopower" power_wifi0`
 echo $xml_autopower_power_wifi0


