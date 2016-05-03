. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh

 local xml_config_tr069=`config_read /config/administrator/tr069`
 xml_config_tr069=`config_setoption "$xml_config_tr069" suser "$1"`
 config_write /config/administrator/tr069 "$xml_config_tr069"


