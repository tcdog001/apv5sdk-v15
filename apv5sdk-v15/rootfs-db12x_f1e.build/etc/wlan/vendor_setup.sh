
#
#vendor_user_setup(user, passwd)
#
vendor_user_setup()
{
	(test -z "$1" || test -z "$2") && exit 1
	local user="$1"
	local pass_new="$2"
    local xml_config=`config_read /config/system`
    local pass_encoded=`$CRYPT "$pass_new"`
    xml_config=`config_setoption "$xml_config" password "$pass_encoded"`
    xml_config=`config_setoption "$xml_config" username "$user"`
    config_write /config/system "$xml_config"
}

#
#vendor_wmode_setup(work_mode)
#
vendor_wmode_setup()
{
	(test -z "$1") && exit 1
	local workmode="$1"
	local xml_config=`config_read /config/system`
	xml_config=`config_setoption "${xml_config}" workmode "${workmode}"`
	config_write /config/system "${xml_config}"	
}

#
#vendor_ssid_setup(ssid)
#
XPATH_VAP=/config/network/vap
MAX_VAP_NUM=16
vendor_ssid_setup()
{
	(test -z "$1") && exit 1
	local ssid="$1"
	
    for num in `seq ${MAX_VAP_NUM}`
    do
        local vap_str=`config_read ${XPATH_VAP}${num} 2>/dev/null`
        local old_ssid=`config_getoption "$vap_str" ssid 2>/dev/null`
        if [ -n "${old_ssid}" ];then
			vap_str=`config_setoption "$vap_str" ssid "${ssid}"`
			config_write ${XPATH_VAP}${num} "${vap_str}"
        fi
    done
}

#
#vendor_cf_setup()
#
vendor_cf_setup()
{
	vendor_flag="comba"
	vendor_file=/jffs/.OEM/.company_name
	cfg_path=/etc/wlan/.OEMINFO/cfg
	if [ -f ${vendor_file} ];then
		vendor_name=`cat ${vendor_file}`
	else
		return 1
	fi

	if [ ! -z "${vendor_name}" ];then
		for vendor in "${vendor_flag}"
		do
			echo "${vendor_name}" | grep -o -i -q -e "${vendor_flag}"
			if [ $? -eq 0 ] && [ -f ${cfg_path}/config.xml.${vendor_flag} ];then
				cp ${cfg_path}/config.xml.${vendor_flag} /jffs/config.xml
			fi
		done
	fi

}


