. /usr/lib/web/constant.sh
. /usr/lib/web/xmlconf.sh
. /usr/lib/web/function.sh
#
#changepassword.sh username password password
#
    local user="$1"
    local pass_new="$2"
    local pass_new_retype="$3"

    if [ "$pass_new" != "$pass_new_retype" ]; then
        false
        return
    fi

    if [ $? != 0 ]; then
        false
        return
    fi

    local xml_config=`config_read /config/system`
    local pass_encoded=`$CRYPT "$pass_new"`
    xml_config=`config_setoption "$xml_config" password "$pass_encoded"`
    config_write /config/system "$xml_config"
    local xml_config=`config_read /config/system`
    local xml_username=`config_getoption "$xml_config" username`
    local xml_password=`config_getoption "$xml_config" password`
local awk_program="BEGIN{FS=\":\"; OFS=\":\";} /^$xml_username:/{\$2=\"$xml_password\";} {print;}"

        cat /etc/passwd | awk "$awk_program" > /tmp/passwd
            mv /tmp/passwd /etc/passwd
