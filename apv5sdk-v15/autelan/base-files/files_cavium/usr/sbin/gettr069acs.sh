. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 xml_config_tr069=`config_read /config/administrator/tr069`
 local xml_tr069_acs=`config_getoption "$xml_config_tr069" acs`
 echo $xml_tr069_acs
