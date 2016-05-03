. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 xml_config_tr069=`config_read /config/administrator/tr069`
 local xml_tr069_suser=`config_getoption "$xml_config_tr069" suser`
 local xml_tr069_spass=`config_getoption "$xml_config_tr069" spass`
 local xml_tr069_suser_r=`config_getoption "$xml_config_tr069" suser_r`
 local xml_tr069_spass_r=`config_getoption "$xml_config_tr069" spass_r`
 echo $xml_tr069_suser
 echo $xml_tr069_spass
 echo $xml_tr069_suser_r
 echo $xml_tr069_spass_r

