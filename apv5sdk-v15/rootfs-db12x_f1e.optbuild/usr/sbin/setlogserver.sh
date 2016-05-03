. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
local xml_syslogserver=`config_read /config/system/log`
xml_syslogserver=`config_setoption "$xml_syslogserver" sip "$1"`
config_write /config/system/log "$xml_syslogserver"

