. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
local xml_syslogserver=`config_read /config/system/log`
local xml_syslogserver_sip=`config_getoption "$xml_syslogserver" sip`
echo $xml_syslogserver_sip


