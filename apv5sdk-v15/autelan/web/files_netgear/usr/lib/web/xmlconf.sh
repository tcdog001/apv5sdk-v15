#
# upconfig_read(xpath)
#
upconfig_read()
{
    local xpath="$1"

    xmlconf readconf "$UPGRADE_XML" "$xpath" 2>/dev/null
}

#
# upconfig_setoption(config, optname, optvalue)
#
upconfig_setoption()
{
    local xml_config="$1"
    local optname="$2"
    local optvalue="$3"

    echo "$xml_config" | xmlconf setoption - "$optname" "$optvalue" 2>/dev/null
}

#
# upconfig_getoption(config, optname)
upconfig_getoption()
{
    local xml_config="$1"
    local optname="$2"

    echo "$xml_config" | xmlconf getoption - "$optname" 2>/dev/null
}

#
# upconfig_write(xpath, config)
#
upconfig_write()
{
    local xpath="$1"
    local xml_config="$2"

    echo "$xml_config" | xmlconf writeconf "$UPGRADE_XML" "$xpath" - 2>/dev/null
}

#
# seconfig_read(xpath)
#
seconfig_read()
{
    local xpath="$1"

    xmlconf readconf "$SESSION_XML" "$xpath" 2>/dev/null
}

#
# seconfig_setoption(config, optname, optvalue)
#
seconfig_setoption()
{
    local xml_config="$1"
    local optname="$2"
    local optvalue="$3"

    echo "$xml_config" | xmlconf setoption - "$optname" "$optvalue" 2>/dev/null
}

#
# seconfig_getoption(config, optname)
#
seconfig_getoption()
{
    local xml_config="$1"
    local optname="$2"

    echo "$xml_config" | xmlconf getoption - "$optname" 2>/dev/null
}

#
# seconfig_write(xpath, config)
#
seconfig_write()
{
    local xpath="$1"
    local xml_config="$2"

    echo "$xml_config" | xmlconf writeconf "$SESSION_XML" "$xpath" - 2>/dev/null
}

#
# firmware_read()
#
firmware_read()
{
    xmlconf readconf "$FIRMWARE_XML" /config/firmware 2>/dev/null
}

#
# firmware_getoption(optname)
#
firmware_getoption()
{
    local optname="$1"

    echo "$xml_firmware" | xmlconf getoption - "$optname" 2>/dev/null
}

#
# config_read(xpath)
#
config_read()
{
    local xpath="$1"

    xmlconf readconf "$CONFIG_XML" "$xpath" 2>/dev/null
}

#
# config_read_subset(xml_config, xpath)
#
config_read_subset()
{
    local xml_config="$1"
    local xpath="$2"

    echo "$xml_config" | xmlconf readconf - "$xpath" 2>/dev/null
}

MD5SUM=/usr/bin/md5sum
CHECKSUM=/jffs/checksum
#
# config_write(xpath, config)
#
config_write()
{
    local xpath="$1"
    local xml_config="$2"
    echo "$xml_config" | xmlconf writeconf "$CONFIG_XML" "$xpath" - 2>/dev/null
    ${MD5SUM} ${CONFIG_XML} | awk '{print $1}' > ${CHECKSUM}
}

#
# config_getoption(config, optname)
#
config_getoption()
{
    local xml_config="$1"
    local optname="$2"

    echo "$xml_config" | xmlconf getoption - "$optname" 2>/dev/null
}

#
# config_setoption(config, optname, optvalue)
#
config_setoption()
{
    local xml_config="$1"
    local optname="$2"
    local optvalue="$3"

    echo "$xml_config" | xmlconf setoption - "$optname" "$optvalue" 2>/dev/null
}

#
# acconfig_read()
#
acconfig_read()
{
    xmlconf readconf "$ACCONFIG_XML" /config/acset 2>/dev/null
}
#
# acconfig_getoption(optname)
#
acconfig_getoption()
{
       local optname="$1"
          echo "$xml_acconfig" | xmlconf getoption - "$optname" 2>/dev/null
}
#
# acconfig_write(xpath, config)
 #
acconfig_write()
{
	      local xpath="$1"
	          local xml_config="$2"
		      echo "$xml_config" | xmlconf writeconf "$ACCONFIG_XML" "$xpath" - 2>/dev/null
}

