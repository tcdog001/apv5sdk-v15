
#
#lang_check()
#
lang_check()
{
		local lang="$FORM_chalang"
    
		local xml_session=`config_read /config/session`
		xml_session=`config_setoption "$xml_session" language "$lang"`
		config_write  /config/session "$xml_session"
		if [ $? != 0 ]; then
        false
    else
        true
    fi	
}



login_check()
{
    local user="$1"
    local pass="$2"

    local xml_config=`config_read /config/system`
    local xml_username=`config_getoption "$xml_config" username`
    local xml_password=`config_getoption "$xml_config" password`

    local pass_salt=`echo -n "$xml_password" | head -c 11`
    local pass_encoded=`$CRYPT "$pass" "$pass_salt"`
    if [ "$user" == "$xml_username" -a "$pass_encoded" == "$xml_password" ]; then
        true
    else
        false
    fi
}

conf_user_setup()
{
		local xml_config=`config_read /config/system`
    local xml_username=`config_getoption "$xml_config" username`
    local user="$FORM_user_new"
    local pass_old="$FORM_pass_old"
    local pass_new="$FORM_pass_new"
    local pass_new_retype="$FORM_pass_new_retype"

    if [ "$pass_new" != "$pass_new_retype" ]; then
        false
        return
    fi

    login_check "$xml_username" "$pass_old"
    if [ $? != 0 ]; then
        false
        return
    fi
		if [ -z "$user" ]; then
        false
        return
    fi
    local xml_config=`config_read /config/system`
    local pass_encoded=`$CRYPT "$pass_new"`
    xml_config=`config_setoption "$xml_config" password "$pass_encoded"`
    xml_config=`config_setoption "$xml_config" username "$user"`
    config_write /config/system "$xml_config"
    if [ $? != 0 ]; then
        false
    else
        true
    fi

}

#
# login_update(user, pass_old, pass_new, pass_new_retype)
#
login_update()
{

    local user="$1"
    local pass_old="$2"
    local pass_new="$3"
    local pass_new_retype="$4"

    if [ "$pass_new" != "$pass_new_retype" ]; then
        false
        return
    fi

    login_check "$user" "$pass_old"
    if [ $? != 0 ]; then
        false
        return
    fi

    local xml_config=`config_read /config/system`
    local pass_encoded=`echo "$pass_new" | md5sum | head -c 32`
    xml_config=`config_setoption "$xml_config" password "$pass_encoded"`
    config_write /config/system "$xml_config"
    if [ $? != 0 ]; then
        false
    else
        true
    fi

}

#
# session_start()
#
session_start()
{
    local xml_session=`seconfig_read  /config/session`
    local xml_session_id=`seconfig_getoption "$xml_session" session_id`
    local xml_timestamp=`seconfig_getoption "$xml_session" timestamp`
    local xml_client_ip=`seconfig_getoption "$xml_session" client_ip`
    local xml_session1=`config_read  /config/session`
    local xml_timeout=`config_getoption "$xml_session1" timeout`
   
    local xml_mode=`config_read /config/system`
    local xml_mode_type=`config_getoption "$xml_mode" workmode`
    local inactive_time="0"

    # this session
    local session_id=`echo -n "$SESSIONID" | md5sum | head -c 32`
    local timestamp=`cat /proc/uptime | cut -d. -f1`
    local client_ip="$REMOTE_ADDR"

    # Should no previous session
    # Should no active session
    if [ "$xml_session_id" != "" -a "$COOKIE_SESSION_ID" != "$xml_session_id" -a "$client_ip" != "$xml_client_ip" ]; then
        inactive_time=`expr $timestamp - $xml_timestamp | sed "s/-//"`
        expr $inactive_time \>= $xml_timeout
        if [ $? != 0 ]; then
            my_action_status="Used by $xml_client_ip"
            cgi_exec session.htm
            exit
        fi
    fi

    # Update
    xml_session=`seconfig_setoption "$xml_session" session_id "$session_id"`
    xml_session=`seconfig_setoption "$xml_session" timestamp "$timestamp"`
    xml_session=`seconfig_setoption "$xml_session" client_ip "$client_ip"`
    seconfig_write /config/session "$xml_session"

    echo "Set-Cookie: SESSION_ID=$session_id; path=/;"
    if [ "$xml_mode_type" == "1" ]; then
       response_redirect lansetup.htm
    else
       response_redirect extend.htm
    fi
}

#
# session_validate()
#
session_validate()
{
    local xml_session=`seconfig_read  /config/session`
    local xml_session_id=`seconfig_getoption "$xml_session" session_id`
    local xml_timestamp=`seconfig_getoption "$xml_session" timestamp`
    local xml_client_ip=`seconfig_getoption "$xml_session" client_ip`
    local xml_session1=`config_read  /config/session`
    local xml_timeout=`config_getoption "$xml_session1" timeout`

    # SESSION_ID should match
    if [ "$COOKIE_SESSION_ID" == ""  -o "$COOKIE_SESSION_ID" != "$xml_session_id" ]; then
        my_action_status="Invalid Session"
        cgi_exec session.htm
        exit
    fi

    # Inactive time should less than timeout
    local timestamp=`cat /proc/uptime | cut -d. -f1`
    local inactive_time=`expr $timestamp - $xml_timestamp | sed "s/-//"`   
    expr $inactive_time \< $xml_timeout  
    if [ $? != 0 ]; then
        my_action_status="Session Timeout"
        cgi_exec session.htm
        exit
    fi

    # REMOTE_ADDR check
    if [ "$REMOTE_ADDR" != "$xml_client_ip" ]; then
        my_action_status="Invalid Session"
        cgi_exec session.htm
        exit
    fi

    # HTTP_REFERER check
    if [ "$1" != "no_referer_check" ]; then
        if [ "$HTTP_REFERER" == "" ]; then
            my_action_status="Invalid Session"
            cgi_exec session.htm
            exit
        fi
    fi

    # Update
    xml_session=`seconfig_setoption "$xml_session" timestamp "$timestamp"`
    seconfig_write  /config/session "$xml_session"
}

#
# session_end()
#
session_end()
{
    local xml_session=`seconfig_read  /config/session`
    xml_session=`seconfig_setoption "$xml_session" session_id ""`
    xml_session=`seconfig_setoption "$xml_session" timestamp ""`
    xml_session=`seconfig_setoption "$xml_session" client_ip ""`
    seconfig_write  /config/session "$xml_session"

    response_redirect /
}

#
# session_start_force()
#
session_start_force()
{
    local xml_session=`seconfig_read  /config/session`

    # this session
    local session_id=`echo -n "$SESSIONID" | md5sum | head -c 32`
    local timestamp=`cat /proc/uptime | cut -d. -f1`
    local client_ip="$REMOTE_ADDR"

    # Update
    xml_session=`seconfig_setoption "$xml_session" session_id "$session_id"`
    xml_session=`seconfig_setoption "$xml_session" timestamp "$timestamp"`
    xml_session=`seconfig_setoption "$xml_session" client_ip "$client_ip"`
    seconfig_write  /config/session "$xml_session"

    echo "Set-Cookie: SESSION_ID=$session_id; path=/;"
    
    local xml_mode=`config_read /config/system`
    local xml_mode_type=`config_getoption "$xml_mode" workmode`
    if [ "$xml_mode_type" == "1" ]; then
       response_redirect lansetup.htm
    else
       response_redirect extend.htm
    fi
}

#
# response_headers()
#
response_headers()
{
    echo "Content-Type: text/html"
    echo "Cache-Control: no-cache"
    echo "Pragma: no-cache"
    echo "Expires: 0"
}

#
# response_redirect(url)
#
response_redirect()
{
    echo "Location: $1"
}

#
# cgi_download(file)
#
cgi_download()
{
    echo "Content-Type: application/octet-stream;"
    echo "Content-disposition: attachment; filename=config.xml"
    echo
    cat "$1"
}

cgi_oem_download()
{
    echo "Content-Type: application/octet-stream;"
    echo "Content-disposition: attachment; filename=$2"
    echo
    cat "$1"
}



#
# cgi_download(file)
#
cgi_download1()
{
    echo "Content-Type: application/octet-stream;"
    echo "Content-disposition: attachment; filename=messages"
    echo
    cat "$1"
}

#
# cgi_download(file)
#
cgi_download2()
{
    echo "Content-Type: application/octet-stream;"
    echo "Content-disposition: attachment; filename=messages.0"
    echo
    cat "$1"
}

#
# cgi_exec(file)
#

cgi_exec()
{
    local html="$1"
    haserl --accept-none "$html"
}


#
# webui_radiobutton_check(value, check)
#
webui_radiobutton_check()
{
    local value="$1"
    local check="$2"
    if [ "$value" == "$check" ]; then
        echo -n " checked "
    fi
}

webui_checkbox_check()
{
    local value="$1"
    local check="$2"
    if [ "$value" == "$check" ]; then
        echo -n " checked "
    fi
}

webui_listbox_select()
{
    local value="$1"
    local check="$2"
    if [ "$value" == "$check" ]; then
        echo -n ' selected="selected" '
    fi
}

#
# lan_setup(proto, ipaddr, netmask, gateway)
#
lan_setup()
{
    local proto="$1"
    local ipaddr="$2"
    local netmask="$3"
    local count="$5"
    local route1="$FORM_route1"
    local vendorclassid="$FORM_vendorclassid"

    local xml_config=`config_read /config/network/lan`
    xml_config=`config_setoption "$xml_config" proto "$proto"`
    if [ "$proto" == "static" ]; then
        xml_config=`config_setoption "$xml_config" ipaddr "$ipaddr"`
        xml_config=`config_setoption "$xml_config" netmask "$netmask"`
    else
        xml_config=`config_setoption "$xml_config" dhcpcount "$count"`
        xml_config=`config_setoption "$xml_config" vendorclassid "$vendorclassid"`
    fi
    config_write /config/network/lan "$xml_config"
    
    local xml_sroute=`config_read /config/network/sroute`
    local xml_sroute_route1=`config_getoption "$xml_sroute" route1`
    [ -n "$xml_sroute_route1" ] && $ROUTE del -net default gw "$xml_sroute_route1" 2>/dev/null
    
    local xml_bnet_sroute=`config_read /config/network/sroute`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" route1 "$route1"`
    config_write /config/network/sroute "$xml_bnet_sroute"
    
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
    
       
}

#
#wired_if_setup
#
wired_if_setup()
{
    local eth0_ip="$FORM_wdip1"
    local eth0_netmask="$FORM_wdmask1"
    local eth0_fw2="${FORM_wdfw21:-yes}"  
    
    local xml_bnet_eth0=`config_read /config/network/eth0`
    xml_bnet_eth0=`config_setoption "$xml_bnet_eth0" ip "$eth0_ip"`
    xml_bnet_eth0=`config_setoption "$xml_bnet_eth0" netmask "$eth0_netmask"`
    xml_bnet_eth0=`config_setoption "$xml_bnet_eth0" forward2 "$eth0_fw2"`
    config_write /config/network/eth0 "$xml_bnet_eth0"
    
    if [ "$DEV_eth1" == "enable" ]; then    
		    local eth1_ip="$FORM_wdip2"
		    local eth1_netmask="$FORM_wdmask2"
		    local eth1_fw2="${FORM_wdfw22:-yes}"
		    
		    local xml_bnet_eth1=`config_read /config/network/eth1`
		    xml_bnet_eth1=`config_setoption "$xml_bnet_eth1" ip "$eth1_ip"`
		    xml_bnet_eth1=`config_setoption "$xml_bnet_eth1" netmask "$eth1_netmask"`
		    xml_bnet_eth1=`config_setoption "$xml_bnet_eth1" forward2 "$eth1_fw2"`
		    config_write /config/network/eth1 "$xml_bnet_eth1"
    fi
}

#
#wireless_if_setup
#
wireless_if_setup()
{
		for vap_num in `seq $NUM_ap`
		do
	    local xml_vap=`config_read /config/network/vap${vap_num}`
	    local xml_vap_enable=`config_getoption "$xml_vap" create`
	    [ "$xml_vap_enable" == "yes" ]&&{
		    eval "local vap_ip=\$FORM_wlip${vap_num}"
		    eval "local vap_netmask=\$FORM_wlmask${vap_num}"
		    eval "local vap_fw2=\${FORM_wlfw2${vap_num}:-yes}" 

		    xml_vap=`config_setoption "$xml_vap" ip "$vap_ip"`
		    xml_vap=`config_setoption "$xml_vap" netmask "$vap_netmask"`
		    xml_vap=`config_setoption "$xml_vap" forward2 "$vap_fw2"`
		    config_write /config/network/vap${vap_num} "$xml_vap"
	    }
    done
}

#
#route_forward_setup
#
route_forward_setup()
{
    local route2="$FORM_route2"
    local rtip2="$FORM_rtip2"
    local rtmask2="$FORM_rtmask2"
    
    local route3="$FORM_route3"
    local rtip3="$FORM_rtip3"
    local rtmask3="$FORM_rtmask3"
    
    local route4="$FORM_route4" 
    local rtip4="$FORM_rtip4"
    local rtmask4="$FORM_rtmask4"

    local fw3="${FORM_fw3:-0}"
    
    local xml_sroute=`config_read /config/network/sroute`
    local xml_sroute_route2=`config_getoption "$xml_sroute" route2`
    local xml_sroute_rtip2=`config_getoption "$xml_sroute" rtip2`
    local xml_sroute_rtmask2=`config_getoption "$xml_sroute" rtmask2`
    local xml_sroute_route3=`config_getoption "$xml_sroute" route3`
    local xml_sroute_rtip3=`config_getoption "$xml_sroute" rtip3`
    local xml_sroute_rtmask3=`config_getoption "$xml_sroute" rtmask3`
    local xml_sroute_route4=`config_getoption "$xml_sroute" route4`
    local xml_sroute_rtip4=`config_getoption "$xml_sroute" rtip4`
    local xml_sroute_rtmask4=`config_getoption "$xml_sroute" rtmask4`
    
    [ -n "$xml_sroute_route2" ] && [ -n "$xml_sroute_rtip2" ] && [ -n "$xml_sroute_rtmask2" ] && $ROUTE del -net $xml_sroute_rtip2 netmask $xml_sroute_rtmask2 gw $xml_sroute_route2 2>/dev/null
    [ -n "$xml_sroute_route3" ] && [ -n "$xml_sroute_rtip3" ] && [ -n "$xml_sroute_rtmask3" ] && $ROUTE del -net $xml_sroute_rtip3 netmask $xml_sroute_rtmask3 gw $xml_sroute_route3 2>/dev/null
    [ -n "$xml_sroute_route4" ] && [ -n "$xml_sroute_rtip4" ] && [ -n "$xml_sroute_rtmask4" ] && $ROUTE del -net $xml_sroute_rtip4 netmask $xml_sroute_rtmask4 gw $xml_sroute_route4 2>/dev/null
    if [ -n "$rtip2" ];then
	ip21=`echo $rtip2 | awk -F'[.]' '{print $1}'`
	ip22=`echo $rtip2 | awk -F'[.]' '{print $2}'`
	ip23=`echo $rtip2 | awk -F'[.]' '{print $3}'`
	ip24=`echo $rtip2 | awk -F'[.]' '{print $4}'`

	mask21=`echo $rtmask2 | awk -F'[.]' '{print $1}'`
	mask22=`echo $rtmask2 | awk -F'[.]' '{print $2}'`
	mask23=`echo $rtmask2 | awk -F'[.]' '{print $3}'`
	mask24=`echo $rtmask2 | awk -F'[.]' '{print $4}'`

	new_ip21=$(($ip21&$mask21))
	new_ip22=$(($ip22&$mask22))
	new_ip23=$(($ip23&$mask23))
	new_ip24=$(($ip24&$mask24))
	
	new_rtip2=$new_ip21.$new_ip22.$new_ip23.$new_ip24
    fi	
    if [ -n "$rtip3" ];then
	ip31=`echo $rtip3 | awk -F'[.]' '{print $1}'`
	ip32=`echo $rtip3 | awk -F'[.]' '{print $2}'`
	ip33=`echo $rtip3 | awk -F'[.]' '{print $3}'`
	ip34=`echo $rtip3 | awk -F'[.]' '{print $4}'`

	mask31=`echo $rtmask3 | awk -F'[.]' '{print $1}'`
	mask32=`echo $rtmask3 | awk -F'[.]' '{print $2}'`
	mask33=`echo $rtmask3 | awk -F'[.]' '{print $3}'`
	mask34=`echo $rtmask3 | awk -F'[.]' '{print $4}'`

	new_ip31=$(($ip31&$mask31))
	new_ip32=$(($ip32&$mask32))
	new_ip33=$(($ip33&$mask33))
	new_ip34=$(($ip34&$mask34))
	
	new_rtip3=$new_ip31.$new_ip32.$new_ip33.$new_ip34
    fi
    if [ -n "$rtip4" ];then
	ip41=`echo $rtip4 | awk -F'[.]' '{print $1}'`
	ip42=`echo $rtip4 | awk -F'[.]' '{print $2}'`
	ip43=`echo $rtip4 | awk -F'[.]' '{print $3}'`
	ip44=`echo $rtip4 | awk -F'[.]' '{print $4}'`

	mask41=`echo $rtmask4 | awk -F'[.]' '{print $1}'`
	mask42=`echo $rtmask4 | awk -F'[.]' '{print $2}'`
	mask43=`echo $rtmask4 | awk -F'[.]' '{print $3}'`
	mask44=`echo $rtmask4 | awk -F'[.]' '{print $4}'`

	new_ip41=$(($ip41&$mask41))
	new_ip42=$(($ip42&$mask42))
	new_ip43=$(($ip43&$mask43))
	new_ip44=$(($ip44&$mask44))
	
	new_rtip4=$new_ip41.$new_ip42.$new_ip43.$new_ip44
    fi
    
    local xml_bnet_sroute=`config_read /config/network/sroute`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" route2 "$route2"`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" rtip2 "$new_rtip2"`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" rtmask2 "$rtmask2"`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" route3 "$route3"`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" rtip3 "$new_rtip3"`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" rtmask3 "$rtmask3"`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" route4 "$route4"`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" rtip4 "$new_rtip4"`
    xml_bnet_sroute=`config_setoption "$xml_bnet_sroute" rtmask4 "$rtmask4"`
    config_write /config/network/sroute "$xml_bnet_sroute"
    
    local xml_bnet_forward3=`config_read /config/network`
    xml_bnet_forward3=`config_setoption "$xml_bnet_forward3" forward3 "$fw3"`
    config_write /config/network "$xml_bnet_forward3"
    
    if [ "$fw3" == "0" ]; then
        local xml_nat=`config_read /config/firewall/nat`
        xml_nat=`config_setoption "$xml_nat" enable "no"`
        config_write /config/firewall/nat "$xml_nat"
    fi
    
}

#
# dns_setup( nameserver_config, nameserver1, nameserver2)
#
dns_setup()
{
    local nameserver_config="$1"
    local nameserver1="$2"
    local nameserver2="$3"

    local xml_config=`config_read /config/network/lan`
    xml_config=`config_setoption "$xml_config" nameserver_config "$nameserver_config"`
    if [ "$nameserver_config" == "static" ]; then
        xml_config=`config_setoption "$xml_config" nameserver1 "$nameserver1"`
        xml_config=`config_setoption "$xml_config" nameserver2 "$nameserver2"`
    fi
    config_write /config/network/lan "$xml_config"
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
   

}

#
#radius_setup
#
radius_setup()
{
    local radiusip="$FORM_radiusip"  
    local authport="$FORM_authport"
    local sharekey="$FORM_sharekey"
    
    local radiusip1="$FORM_radiusip1"  
    local authport1="$FORM_authport1"
    local sharekey1="$FORM_sharekey1"
    
    local radiusaip="$FORM_radiusaip"
    local acceptport="$FORM_acceptport"
    local asharekey="$FORM_asharekey"
    
    local radiusaip1="$FORM_radiusaip1"
    local acceptport1="$FORM_acceptport1"
    local asharekey1="$FORM_asharekey1"

    local xml_config=`config_read /config/network/radius`

    xml_config=`config_setoption "$xml_config" radiusip "$radiusip"`    
    xml_config=`config_setoption "$xml_config" authport "$authport"`
    xml_config=`config_setoption "$xml_config" sharekey "$sharekey"`
    
    xml_config=`config_setoption "$xml_config" radiusip1 "$radiusip1"`    
    xml_config=`config_setoption "$xml_config" authport1 "$authport1"`
    xml_config=`config_setoption "$xml_config" sharekey1 "$sharekey1"`
    
    xml_config=`config_setoption "$xml_config" radiusaip "$radiusaip"`
    xml_config=`config_setoption "$xml_config" acceptport "$acceptport"`
    xml_config=`config_setoption "$xml_config" asharekey "$asharekey"`
    
    xml_config=`config_setoption "$xml_config" radiusaip1 "$radiusaip1"`
    xml_config=`config_setoption "$xml_config" acceptport1 "$acceptport1"`
    xml_config=`config_setoption "$xml_config" asharekey1 "$asharekey1"`

    config_write /config/network/radius "$xml_config"
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
    
}

#
#ntp_setup
#

ntp_setup()
{

    local timezone="$FORM_timezone"
    local usedefault="$FORM_usedefault"
    local timeserver1="$FORM_timeserver1"
    local timeserver2="$FORM_timeserver2"
    local timeupdate="$FORM_update"

    local xml_config=`config_read /config/system`
    xml_config=`config_setoption "$xml_config" timezone "$timezone"`
    xml_config=`config_setoption "$xml_config" timeupdate "$timeupdate"`
    xml_config=`config_setoption "$xml_config" timeserver_usedefault "$usedefault"`
    if [ "$usedefault" == "no" ]; then
        xml_config=`config_setoption "$xml_config" timeserver1 "$timeserver1"`
        xml_config=`config_setoption "$xml_config" timeserver2 "$timeserver2"`
    fi
    config_write /config/system "$xml_config"
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
    

}


#
# wlan_setup
#
wlan_setup()
{
    local enable="${FORM_enable:-no}"
    local broadcast="${FORM_broadcast:-no}"
    local ssid="$FORM_ssid"
    local region="$FORM_region"
    local channel="$FORM_channel"
    local channel1="$FORM_channel1"
    local channela="$FORM_channela"
                  
    local mode="$FORM_mode"
    local modea="$FORM_modea"
    local moden="$FORM_moden"
    local modeng="$FORM_modeng"
    local rate="$FORM_rate"
    local rateb="$FORM_rateb"
    local ratea="$FORM_ratea"
    local rate_n_ht20ds="$FORM_rate_n_ht20ds"
    local rate_n_ht40ds="$FORM_rate_n_ht40ds"
    local rate_n_ht40ds_s="$FORM_rate_n_ht40ds_s"
    local beacon="$FORM_beacon" 

    local vapname="$FORM_vap_name"
    local xml_num_str="echo $vapname | awk -F'p' '{print \$2}'"
		local xml_num=`eval $xml_num_str`
    local xml_vapnum=`config_read /config/network`
    xml_vapnum=`config_setoption "$xml_vapnum" vapnum $xml_num`
 		config_write /config/network "$xml_vapnum"

    local xml_config=`config_read /config/network/$vapname`
    
    local xml_wlan_wifi=`config_getoption "$xml_config" wifi`
    local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
    
    local shortgi=`config_getoption "$xml_wifi" shortgi`
    
    xml_wifi=`config_setoption "$xml_wifi" channel1 "$channel1"`
    xml_wifi=`config_setoption "$xml_wifi" channel "$channel"`
    xml_wifi=`config_setoption "$xml_wifi" channela "$channela"`
    xml_wifi=`config_setoption "$xml_wifi" region "$region"`
    xml_wifi=`config_setoption "$xml_wifi" mode "$mode"`
    xml_wifi=`config_setoption "$xml_wifi" modea "$modea"`
    
    eval wifi_devmode="$"DEVMODE_$xml_wlan_wifi
    if [ "$wifi_devmode" == "an" ]; then
       xml_wifi=`config_setoption "$xml_wifi" moden "$moden"`
    fi   
    if [ "$wifi_devmode" == "n" ]; then
       xml_wifi=`config_setoption "$xml_wifi" moden "$modeng"`
    fi
    
    config_write /config/network/$xml_wlan_wifi "$xml_wifi"
    
    local config_country_code=`config_read /config/network/country`
		config_country_code=`config_setoption "$config_country_code" region "$region"`
    config_write /config/network/country "$config_country_code"
    
    xml_config=`config_setoption "$xml_config" enable "$enable"`
    xml_config=`config_setoption "$xml_config" broadcast "$broadcast"`
    xml_config=`config_setoption "$xml_config" ssid "$ssid"`
    xml_config=`config_setoption "$xml_config" rate "$rate"`
    xml_config=`config_setoption "$xml_config" rateb "$rateb"`
    xml_config=`config_setoption "$xml_config" ratea "$ratea"`
    xml_config=`config_setoption "$xml_config" rate_n_ht20ds "$rate_n_ht20ds"`
    if [ "$shortgi" == "0" ];then
       xml_config=`config_setoption "$xml_config" rate_n_ht40ds "$rate_n_ht40ds"`
    fi
    if [ "$shortgi" == "1" ];then
       xml_config=`config_setoption "$xml_config" rate_n_ht40ds "$rate_n_ht40ds_s"`
    fi
	  xml_config=`config_setoption "$xml_config" beacon "$beacon"`

		config_write /config/network/$vapname "$xml_config"
    
    
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
      
}

security_setup()
{
		local authmode="$FORM_authmode"
    local security="$FORM_security"
    local wpamode="$FORM_wpamode"
    local passphrase="$FORM_passphrase"
    local encryption="$FORM_encryption"
    local wepkey="$FORM_wepkey"
    local etype="$FORM_etype"
    local wepbit="$FORM_wepbit"
    
    
    local passkey1="$FORM_passkey1"
    local passkey2="$FORM_passkey2"
    local passkey3="$FORM_passkey3"
    local passkey4="$FORM_passkey4"
    
    local authtype="$FORM_authtype"
    local sec_type="$FORM_sec_type"
    local enc_type="$FORM_enc_type"
    local enc_pass="$FORM_enc_pass"
    local server_ip="$FORM_server_ip"
    local server_port="$FORM_server_port"
    
    local if3cert="${FORM_3cert:-0}"
    
    local vapname="$FORM_vap_name"
    local xml_num_str="echo $vapname | awk -F'p' '{print \$2}'"
		local xml_num=`eval $xml_num_str`
    local xml_vapnum=`config_read /config/network`
    xml_vapnum=`config_setoption "$xml_vapnum" vapnum $xml_num`
 		config_write /config/network "$xml_vapnum"

    local xml_config=`config_read /config/network/$vapname`
    xml_config=`config_setoption "$xml_config" security "$security"`
    xml_config=`config_setoption "$xml_config" authmode "$authmode"`
    xml_config=`config_setoption "$xml_config" wpamode "$wpamode"`
    xml_config=`config_setoption "$xml_config" passphrase "$passphrase"`
    xml_config=`config_setoption "$xml_config" encryption "$encryption"`

    xml_config=`config_setoption "$xml_config" wepkey "$wepkey"`
    xml_config=`config_setoption "$xml_config" wepkeytype "$etype"`
    xml_config=`config_setoption "$xml_config" wepbit "$wepbit"`
     
    xml_config=`config_setoption "$xml_config" passkey1 "$passkey1"`
    xml_config=`config_setoption "$xml_config" passkey2 "$passkey2"`
    xml_config=`config_setoption "$xml_config" passkey3 "$passkey3"`
    xml_config=`config_setoption "$xml_config" passkey4 "$passkey4"`
    
    xml_config=`config_setoption "$xml_config" wapibook "$authtype"`
		xml_config=`config_setoption "$xml_config" wapiauth "$sec_type"`
		xml_config=`config_setoption "$xml_config" wapienc "$enc_type"`
		xml_config=`config_setoption "$xml_config" wapipass "$enc_pass"`
		xml_config=`config_setoption "$xml_config" wapisip "$server_ip"`
		xml_config=`config_setoption "$xml_config" wapisport "$server_port"`
		
		xml_config=`config_setoption "$xml_config" if3cert "$if3cert"`
    config_write /config/network/$vapname "$xml_config"
    
    

    
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
    

}

generate_64bit_wepkey()
{
    dd if=/dev/random bs=1 count=5 2>/dev/null |hexdump|grep -o -i -e " [0-9a-f]\{1,4\} [0-9a-f]\{1,4\} [0-9a-f]\{1,2\}"|sed -e 's/ //g'
}

generate_128bit_wepkey()
{
    dd if=/dev/random bs=1 count=13 2>/dev/null |hexdump|grep -o -i -e " [0-9a-f ]\{1,4\} [0-9a-f]\{1,4\} [0-9a-f]\{1,4\} [0-9a-f]\{1,4\} [0-9    a-f]\{1,4\} [0-9a-f]\{1,4\} [0-9a-f]\{1,2\}"|sed -e 's/ //g'    
}

generate_152bit_wepkey()
{
    dd if=/dev/random bs=1 count=16 2>/dev/null |hexdump|grep -o -i -e " [0-9a-f ]\{1,4\} [0-9a-f]\{1,4\} [0-9a-f]\{1,4\} [0-9a-f]\{1,4\} [0-9    a-f]\{1,4\} [0-9a-f]\{1,4\} [0-9a-f]\{1,4\} [0-9a-f]\{1,4\}"|sed -e 's/ //g'
}

#
#vap_specific_conf
#
vap_specific_conf()
{
		local vapname="$FORM_vapname"
    local xml_num=`echo $vapname | awk -F'p' '{print $2}'`
    local xml_vapnum=`config_read /config/network`
    xml_vapnum=`config_setoption "$xml_vapnum" vapnum $xml_num`
 		config_write /config/network "$xml_vapnum"
}

#
#vap_create_conf
#
vap_create_conf()
{


		local vapname="$FORM_vapname"
		local dvap="$FORM_dvap"
		local xml_vap_vapdel="$FORM_vapdel"
		local xml_vap_wifi="$FORM_wifiname"
		local xml_vap_wlanmode="$FORM_wlanmode"
		local xml_vap=`config_read /config/network/$vapname`
		local xml_vap_if=`config_getoption "$xml_vap" if`
		local xml_vap_forward2=`config_getoption "$xml_vap" forward2`
		local xml_isolation=`config_getoption "$xml_vap" isolation`
		
		#create a vap
    if [ "$dvap" != "dvap" ];then
		    xml_vap=`config_setoption "$xml_vap" wifi "$xml_vap_wifi"`
				xml_vap=`config_setoption "$xml_vap" wlanmode "$xml_vap_wlanmode"`
				xml_vap=`config_setoption "$xml_vap" create "yes"`
				xml_vap=`config_setoption "$xml_vap" enable "no"`
				if [ "$xml_vap_wlanmode" == "root" ];then
            xml_vap=`config_setoption "$xml_vap" isolation "0"`        	
        fi
				config_write /config/network/$vapname "$xml_vap"
    
    		if [ "$xml_vap_wlanmode" == "ap" -o "$xml_vap_wlanmode" == "root" ];then
    				$WLANCONFIG $xml_vap_if destroy > /dev/null 2>&1
        		$WLANCONFIG $xml_vap_if create wlandev $xml_vap_wifi wlanmode ap > /dev/null 2>&1
        		if [ "$xml_vap_wlanmode" == "root" ];then
            	$IWPRIV $xml_vap_if wds 1 > /dev/null 2>&1          	
        		fi
        		if [ "$xml_vap_forward2" == "yes" ]; then
        				$BRCTL addif $BRLAN $xml_vap_if > /dev/null 2>&1
        		fi
        elif [ "$xml_vap_wlanmode" == "sta" -o "$xml_vap_wlanmode" == "client" ];then
        		$WLANCONFIG $xml_vap_if destroy > /dev/null 2>&1
        		$WLANCONFIG $xml_vap_if create wlandev $xml_vap_wifi wlanmode sta nosbeacon > /dev/null 2>&1
        		if [ "$xml_vap_forward2" == "yes" ]; then
        			$BRCTL addif $BRLAN $xml_vap_if > /dev/null 2>&1
        		fi
        		if [ "$xml_vap_wlanmode" == "client" ];then
            	$IWPRIV $xml_vap_if wds 1 > /dev/null 2>&1
            	$IWPRIV $xml_vap_if hostroaming 1 > /dev/null 2>&1
        		fi
        fi
    else
		    #delete the vap you selected
		    DEL_VAP=`echo $xml_vap_vapdel | awk -F'%' '{num=NF-2;print num}'`
		    for del_vapnum in `seq $DEL_VAP`
		    do
		    	del_athnum=`expr $del_vapnum + 1`
			    eval "local vap_del=\`echo $xml_vap_vapdel | awk -F'%' '{print \$$del_athnum}'\`"
			    [ -n "$vap_del" ]&&{
			        $WLANCONFIG $vap_del destroy  > /dev/null 2>&1
			        local xml_ath_del=`echo $vap_del | awk -F'h' '{print $2}'`
			        xml_ath_del=`expr $xml_ath_del + 1`
			        local xml_vap_del=`config_read /config/network/vap$xml_ath_del`
					    xml_vap_del=`config_setoption "$xml_vap_del" create "no"`
					    xml_vap_del=`config_setoption "$xml_vap_del" enable "no"`
					    xml_vap_del=`config_setoption "$xml_vap_del" security "none"`
							config_write /config/network/vap$xml_ath_del "$xml_vap_del"
					}
		    done
		fi
				
				local xml_allvap=`config_read /config/network`
				local cvapnum=`config_getoption "$xml_allvap" vapnum`
				local cvapname="vap$cvapnum"
				local xml_config_cvapname=`config_read /config/network/${cvapname}`
				local xml_config_cvapname_enable=`config_getoption "$xml_config_cvapname" create`
				
				[ "$xml_config_cvapname_enable" != "yes" ]&&{
					for vap_allnum in `seq $NUM_ap`
					do
							local xml_wlan_config_vap=`config_read /config/network/vap${vap_allnum}`
							local xml_wlan_vap_enable=`config_getoption "$xml_wlan_config_vap" create`
							[ "$xml_wlan_vap_enable" == "yes" ]&&{
								cvapname="vap$vap_allnum"
								xml_allvap=`config_setoption "$xml_allvap" vapnum "$vap_allnum"`
								config_write /config/network "$xml_allvap"
								break
							}
					done
				}
		
}


#
# macfilter_setup
#
macfilter_setup()
{
    
    local macfilter="$FORM_macfilter"
    local filtermode="$FORM_filtermode"
    local rule_num="${FORM_rule_num:-1}"
    local mac1="$FORM_mac1"
    local mac2="$FORM_mac2"
    local mac3="$FORM_mac3"
    local mac4="$FORM_mac4"
    local mac5="$FORM_mac5"
    local mac6="$FORM_mac6"
    local mac7="$FORM_mac7"
    local mac8="$FORM_mac8"
    local mac9="$FORM_mac9"
    local mac10="$FORM_mac10"
    local mac11="$FORM_mac11"
    local mac12="$FORM_mac12"
    local mac13="$FORM_mac13"
    local mac14="$FORM_mac14"
    local mac15="$FORM_mac15"
    local mac16="$FORM_mac16"
    local mac17="$FORM_mac17"
    local mac18="$FORM_mac18"
    local mac19="$FORM_mac19"
    local mac20="$FORM_mac20"
    local vapname="$FORM_vap_name"
    local xml_num_str="echo $vapname | awk -F'p' '{print \$2}'"
		local xml_num=`eval $xml_num_str`
    local xml_vapnum=`config_read /config/network`
    xml_vapnum=`config_setoption "$xml_vapnum" vapnum $xml_num`
 		config_write /config/network "$xml_vapnum"
		
    local xml_config=`config_read /config/network/$vapname`
    xml_config=`config_setoption "$xml_config" macfilter "$macfilter"`
    xml_config=`config_setoption "$xml_config" filtermode "$filtermode"`
    xml_config=`config_setoption "$xml_config" mac_count "$rule_num"`
    xml_config=`config_setoption "$xml_config" mac1 "$mac1"`
    xml_config=`config_setoption "$xml_config" mac2 "$mac2"`    
    xml_config=`config_setoption "$xml_config" mac3 "$mac3"`    
    xml_config=`config_setoption "$xml_config" mac4 "$mac4"`    
    xml_config=`config_setoption "$xml_config" mac5 "$mac5"`    
    xml_config=`config_setoption "$xml_config" mac6 "$mac6"`    
    xml_config=`config_setoption "$xml_config" mac7 "$mac7"`    
    xml_config=`config_setoption "$xml_config" mac8 "$mac8"`    
    xml_config=`config_setoption "$xml_config" mac9 "$mac9"`    
    xml_config=`config_setoption "$xml_config" mac10 "$mac10"`    
    xml_config=`config_setoption "$xml_config" mac11 "$mac11"`    
    xml_config=`config_setoption "$xml_config" mac12 "$mac12"`    
    xml_config=`config_setoption "$xml_config" mac13 "$mac13"`    
    xml_config=`config_setoption "$xml_config" mac14 "$mac14"`    
    xml_config=`config_setoption "$xml_config" mac15 "$mac15"`    
    xml_config=`config_setoption "$xml_config" mac16 "$mac16"`    
    xml_config=`config_setoption "$xml_config" mac17 "$mac17"`    
    xml_config=`config_setoption "$xml_config" mac18 "$mac18"`    
    xml_config=`config_setoption "$xml_config" mac19 "$mac19"`    
    xml_config=`config_setoption "$xml_config" mac20 "$mac20"`
    config_write /config/network/$vapname "$xml_config"
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
    
}

wep_setup()
{

    local enable="${FORM_enable:-no}"
    local broadcast="${FORM_broadcast:-no}"
    local ssid="$FORM_ssid"
    local region="$FORM_region"
    local channel="$FORM_channel"
    local mode="$FORM_mode"
    local authmode="$FORM_authmode"
    local security="$FORM_security"
    local wepkey="$FORM_wepkey"
    local wepbit="$FORM_wepbit"
    
    if [ "$FORM_wepbit" == 64 ]; then
      local passkey1=`generate_64bit_wepkey`
      local passkey2=`generate_64bit_wepkey`
      local passkey3=`generate_64bit_wepkey`
      local passkey4=`generate_64bit_wepkey`
   elif [ "$FORM_wepbit" == 128 ]; then
     local passkey1=`generate_128bit_wepkey`
     local passkey2=`generate_128bit_wepkey`
     local passkey3=`generate_128bit_wepkey`
     local passkey4=`generate_128bit_wepkey`
   else
    local passkey1=`generate_152bit_wepkey`
    local passkey2=`generate_152bit_wepkey`
    local passkey3=`generate_152bit_wepkey`
    local passkey4=`generate_152bit_wepkey`
   fi
   
    xml_vap=`config_read /config/network`
    vapnum=`config_getoption "$xml_vap" vapnum `
    
    local xml_config=`config_read /config/network/vap$vapnum`
    xml_config=`config_setoption "$xml_config" enable "$enable"`
    xml_config=`config_setoption "$xml_config" broadcast "$broadcast"`
    xml_config=`config_setoption "$xml_config" ssid "$ssid"`
    xml_config=`config_setoption "$xml_config" region "$region"`
    xml_config=`config_setoption "$xml_config" channel "$channel"`
    xml_config=`config_setoption "$xml_config" mode "$mode"`

    xml_config=`config_setoption "$xml_config" security "$security"`
    xml_config=`config_setoption "$xml_config" authmode "$authmode"`
    xml_config=`config_setoption "$xml_config" wepkey "$wepkey"`
    xml_config=`config_setoption "$xml_config" wepbit "$wepbit"`

    xml_config=`config_setoption "$xml_config" passkey1 "$passkey1"`
    xml_config=`config_setoption "$xml_config" passkey2 "$passkey2"`
    xml_config=`config_setoption "$xml_config" passkey3 "$passkey3"`
    xml_config=`config_setoption "$xml_config" passkey4 "$passkey4"`
    
    config_write /config/network/vap$vapnum "$xml_config"
    if [ $? != 0 ]; then
       false
    else
       true
    fi
}

sys_wpapsk_startup()
{
		local filepath=/jffs/hostapd/a$vapnum
    local xml_lan=`config_read /config/network/lan`
    local xml_lan_if=`config_getoption "$xml_lan" if`
    local xml_wlan=`config_read /config/network/vap$vapnum`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_ssid=`config_getoption "$xml_wlan" ssid`
    local xml_encryption=`config_getoption "$xml_wlan" encryption`
    local xml_passphrase=`config_getoption "$xml_wlan" passphrase`
    local xml_security=`config_getoption "$xml_wlan" security` 
    
    #获取指定的桥
    local xml_vlan=`config_read /config/network/vap$vapnum/vlan`
    local xml_vlan_enable=`config_getoption "$xml_vlan" enable`
    local xml_vlan_vid=`config_getoption "$xml_vlan" vid`
       
    sys_lan_ipaddr=`sys_get_interface_ipaddr "$xml_lan_if"`
        
    echo "interface=$xml_if"  > $filepath
    if [ "$xml_vlan_enable" == "1" ]; then
    	[ -z $xml_vlan_vid ]||echo "bridge=br$xml_vlan_vid" >>$filepath
    else
    	echo "bridge=$xml_lan_if" >>$filepath
    fi
    echo "driver=madwifi" >> $filepath
    echo "logger_syslog=-1" >> $filepath
    echo "logger_syslog_level=2" >> $filepath
    echo "logger_stdout=-1" >> $filepath
    echo "logger_stdout_level=2" >> $filepath
    echo "dump_file=/tmp/hostapd.dump" >> $filepath
    echo "ssid=$xml_ssid" >> $filepath
    echo "eapol_key_index_workaround=0" >> $filepath
    if [ "$sys_lan_ipaddr" == "" ];then
    	echo "own_ip_addr=192.168.100.100" >> $filepath
    else
    	echo "own_ip_addr=$sys_lan_ipaddr" >> $filepath
    fi
    if [ "$xml_security" == "wpa" ]; then
        echo "wpa=1" >> $filepath
    else
        echo "wpa=2" >> $filepath
    fi
    echo "wpa_passphrase=$xml_passphrase" >> $filepath
    echo "wpa_key_mgmt=WPA-PSK " >> $filepath
    if [ "$xml_encryption" == "TKIP" ]; then
        echo "wpa_pairwise=TKIP" >> $filepath
    else
        echo "wpa_pairwise=CCMP" >> $filepath
    fi
}

sys_wpapsk_config()
{
    
    
    xml_vap=`config_read /config/network`
    vapnum=`config_getoption "$xml_vap" vapnum `
    local filepath=/jffs/hostapd/a$vapnum
    local xml_lan=`config_read /config/network/lan`
    local xml_lan_if=`config_getoption "$xml_lan" if`
    local xml_wlan=`config_read /config/network/vap$vapnum`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_ssid=`config_getoption "$xml_wlan" ssid`
    local xml_encryption=`config_getoption "$xml_wlan" encryption`
    local xml_passphrase=`config_getoption "$xml_wlan" passphrase`
    local xml_security=`config_getoption "$xml_wlan" security` 
    
    #获取指定的桥
    local xml_vlan=`config_read /config/network/vap$vapnum/vlan`
    local xml_vlan_enable=`config_getoption "$xml_vlan" enable`
    local xml_vlan_vid=`config_getoption "$xml_vlan" vid`
       
    sys_lan_ipaddr=`sys_get_interface_ipaddr "$xml_lan_if"`
        
    echo "interface=$xml_if"  > $filepath
    if [ "$xml_vlan_enable" == "1" ]; then
    	[ -z $xml_vlan_vid ]||echo "bridge=br$xml_vlan_vid" >>$filepath
    else
    	echo "bridge=$xml_lan_if" >>$filepath
    fi
    echo "driver=madwifi" >> $filepath
    echo "logger_syslog=-1" >> $filepath
    echo "logger_syslog_level=2" >> $filepath
    echo "logger_stdout=-1" >> $filepath
    echo "logger_stdout_level=2" >> $filepath
    echo "dump_file=/tmp/hostapd.dump" >> $filepath
    echo "ssid=$xml_ssid" >> $filepath
    echo "eapol_key_index_workaround=0" >> $filepath
    if [ "$sys_lan_ipaddr" == "" ];then
    	echo "own_ip_addr=192.168.100.100" >> $filepath
    else
    	echo "own_ip_addr=$sys_lan_ipaddr" >> $filepath
    fi
    if [ "$xml_security" == "wpa" ]; then
        echo "wpa=1" >> $filepath
    else
        echo "wpa=2" >> $filepath
    fi
    echo "wpa_passphrase=$xml_passphrase" >> $filepath
    echo "wpa_key_mgmt=WPA-PSK " >> $filepath
    if [ "$xml_encryption" == "TKIP" ]; then
        echo "wpa_pairwise=TKIP" >> $filepath
    else
        echo "wpa_pairwise=CCMP" >> $filepath
    fi

}

sys_wparadius_startup()
{

    local filepath=/jffs/hostapd/b$vapnum
    local xml_lan=`config_read /config/network/lan`
    local xml_lan_if=`config_getoption "$xml_lan" if`
    local xml_wlan=`config_read /config/network/vap$vapnum`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_ssid=`config_getoption "$xml_wlan" ssid`
    local xml_encryption=`config_getoption "$xml_wlan" encryption`
    local xml_wlan_authmode=`config_getoption "$xml_wlan" authmode`
    local xml_security=`config_getoption "$xml_wlan" security`
    
    #获取指定的桥
    local xml_vlan=`config_read /config/network/vap$vapnum/vlan`
    local xml_vlan_enable=`config_getoption "$xml_vlan" enable`
    local xml_vlan_vid=`config_getoption "$xml_vlan" vid`
    
    local xml_radius=`config_read /config/network/radius`
    local xml_radiusip=`config_getoption "$xml_radius" radiusip`
		local xml_authport=`config_getoption "$xml_radius" authport`
		local xml_sharekey=`config_getoption "$xml_radius" sharekey`
		
		local xml_radiusip1=`config_getoption "$xml_radius" radiusip1`
		local xml_authport1=`config_getoption "$xml_radius" authport1`
		local xml_sharekey1=`config_getoption "$xml_radius" sharekey1`
		
		local xml_radiusaip=`config_getoption "$xml_radius" radiusaip`
		local xml_acceptport=`config_getoption "$xml_radius" acceptport`
		local xml_asharekey=`config_getoption "$xml_radius" asharekey`
		
		local xml_radiusaip1=`config_getoption "$xml_radius" radiusaip1`
		local xml_acceptport1=`config_getoption "$xml_radius" acceptport1`
		local xml_asharekey1=`config_getoption "$xml_radius" asharekey1`
    local sys_lan_ipaddr=`sys_get_interface_ipaddr "$xml_lan_if"`

    echo "interface=$xml_if"  >$filepath
    if [ "$xml_vlan_enable" == "1" ]; then
    	[ -z $xml_vlan_vid ]||echo "bridge=br$xml_vlan_vid" >>$filepath
    else
    	echo "bridge=$xml_lan_if" >>$filepath
    fi
    echo "driver=madwifi" >>$filepath
    echo "logger_syslog=-1" >>$filepath
    echo "logger_syslog_level=2" >>$filepath
    echo "logger_stdout=-1" >>$filepath
    echo "logger_stdout_level=2" >>$filepath
    echo "debug=0" >>$filepath
    echo "dump_file=/tmp/hostapd.dump" >>$filepath
    echo "ssid=$xml_ssid" >>$filepath
    echo "ieee8021x=1" >>$filepath
    echo "eapol_key_index_workaround=0" >>$filepath
    if [ "$sys_lan_ipaddr" == "" ];then
    	echo "own_ip_addr=192.168.100.100" >> $filepath
    else
    	echo "own_ip_addr=$sys_lan_ipaddr" >> $filepath
    fi
    
    if [ "$xml_radiusip" != "" ]; then
		    echo "auth_server_addr=$xml_radiusip"  >>$filepath
		    echo "auth_server_port=$xml_authport" >>$filepath
		    echo "auth_server_shared_secret=$xml_sharekey" >>$filepath
		fi 
		
		if [ "$xml_radiusaip" != "" ]; then  
		    echo "acct_server_addr=$xml_radiusaip "  >>$filepath
		    echo "acct_server_port=$xml_acceptport" >>$filepath
		    echo "acct_server_shared_secret=$xml_asharekey" >>$filepath
    fi
		
		if [ "$xml_radiusip1" != "" ]; then  
		    echo "auth_server_addr=$xml_radiusip1 "  >>$filepath
		    echo "auth_server_port=$xml_authport1" >>$filepath
		    echo "auth_server_shared_secret=$xml_sharekey1" >>$filepath
    fi 
    
    if [ "$xml_radiusaip1" != "" ]; then  
		    echo "acct_server_addr=$xml_radiusaip1 "  >>$filepath
		    echo "acct_server_port=$xml_acceptport1" >>$filepath
		    echo "acct_server_shared_secret=$xml_asharekey1" >>$filepath
    fi

    if [ "$xml_wlan_authmode" == "3" -a "$xml_security" == "wep" ]; then
        echo "wep_key_len_broadcast=5" >>$filepath
        echo "wep_key_len_unicast=5" >>$filepath
        echo "wep_rekey_period=300"  >>$filepath
    else
		    if [ "$xml_security" == "wpa" ]; then
		        echo "wpa=1" >> $filepath
		    else
		        if [ "$xml_security" == "wpa2" ]; then
		          echo "wpa=2" >> $filepath
		        fi
		    fi
		    echo "wpa_key_mgmt=WPA-EAP " >>$filepath
		    if [ "$xml_encryption" == "TKIP" ]; then
		        echo "wpa_pairwise=TKIP" >> $filepath
		    else
			      if [ "$xml_encryption" == "AES" ]; then
			        echo "wpa_pairwise=CCMP" >> $filepath
			      fi
		    fi
		fi
}

sys_wparadius_config()
{
    

    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum `
    local filepath=/jffs/hostapd/b$vapnum
    local xml_lan=`config_read /config/network/lan`
    local xml_lan_if=`config_getoption "$xml_lan" if`
    local xml_wlan=`config_read /config/network/vap$vapnum`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_ssid=`config_getoption "$xml_wlan" ssid`
    local xml_encryption=`config_getoption "$xml_wlan" encryption`
    local xml_wlan_authmode=`config_getoption "$xml_wlan" authmode`
    local xml_security=`config_getoption "$xml_wlan" security`
    #获取指定的桥
    local xml_vlan=`config_read /config/network/vap$vapnum/vlan`
    local xml_vlan_enable=`config_getoption "$xml_vlan" enable`
    local xml_vlan_vid=`config_getoption "$xml_vlan" vid`
    
    local xml_radius=`config_read /config/network/radius`
    local xml_radiusip=`config_getoption "$xml_radius" radiusip`
		local xml_authport=`config_getoption "$xml_radius" authport`
		local xml_sharekey=`config_getoption "$xml_radius" sharekey`
		
		local xml_radiusip1=`config_getoption "$xml_radius" radiusip1`
		local xml_authport1=`config_getoption "$xml_radius" authport1`
		local xml_sharekey1=`config_getoption "$xml_radius" sharekey1`
		
		local xml_radiusaip=`config_getoption "$xml_radius" radiusaip`
		local xml_acceptport=`config_getoption "$xml_radius" acceptport`
		local xml_asharekey=`config_getoption "$xml_radius" asharekey`
		
		local xml_radiusaip1=`config_getoption "$xml_radius" radiusaip1`
		local xml_acceptport1=`config_getoption "$xml_radius" acceptport1`
		local xml_asharekey1=`config_getoption "$xml_radius" asharekey1`
    local sys_lan_ipaddr=`sys_get_interface_ipaddr "$xml_lan_if"`

    echo "interface=$xml_if"  >$filepath
    if [ "$xml_vlan_enable" == "1" ]; then
    	[ -z $xml_vlan_vid ]||echo "bridge=br$xml_vlan_vid" >>$filepath
    else
    	echo "bridge=$xml_lan_if" >>$filepath
    fi
    echo "driver=madwifi" >>$filepath
    echo "logger_syslog=-1" >>$filepath
    echo "logger_syslog_level=2" >>$filepath
    echo "logger_stdout=-1" >>$filepath
    echo "logger_stdout_level=2" >>$filepath
    echo "debug=0" >>$filepath
    echo "dump_file=/tmp/hostapd.dump" >>$filepath
    echo "ssid=$xml_ssid" >>$filepath
    echo "ieee8021x=1" >>$filepath
    echo "eapol_key_index_workaround=0" >>$filepath
    if [ "$sys_lan_ipaddr" == "" ];then
    	echo "own_ip_addr=192.168.100.100" >> $filepath
    else
    	echo "own_ip_addr=$sys_lan_ipaddr" >> $filepath
    fi
    
    if [ "$xml_radiusip" != "" ]; then
		    echo "auth_server_addr=$xml_radiusip"  >>$filepath
		    echo "auth_server_port=$xml_authport" >>$filepath
		    echo "auth_server_shared_secret=$xml_sharekey" >>$filepath
		fi 
		
		if [ "$xml_radiusaip" != "" ]; then  
		    echo "acct_server_addr=$xml_radiusaip "  >>$filepath
		    echo "acct_server_port=$xml_acceptport" >>$filepath
		    echo "acct_server_shared_secret=$xml_asharekey" >>$filepath
    fi
		
		if [ "$xml_radiusip1" != "" ]; then  
		    echo "auth_server_addr=$xml_radiusip1 "  >>$filepath
		    echo "auth_server_port=$xml_authport1" >>$filepath
		    echo "auth_server_shared_secret=$xml_sharekey1" >>$filepath
    fi 
    
    if [ "$xml_radiusaip1" != "" ]; then  
		    echo "acct_server_addr=$xml_radiusaip1 "  >>$filepath
		    echo "acct_server_port=$xml_acceptport1" >>$filepath
		    echo "acct_server_shared_secret=$xml_asharekey1" >>$filepath
    fi

    if [ "$xml_wlan_authmode" == "3" -a "$xml_security" == "wep" ]; then
        echo "wep_key_len_broadcast=5" >>$filepath
        echo "wep_key_len_unicast=5" >>$filepath
        echo "wep_rekey_period=300"  >>$filepath
    else
		    if [ "$xml_security" == "wpa" ]; then
		        echo "wpa=1" >> $filepath
		    else
		        if [ "$xml_security" == "wpa2" ]; then
		          echo "wpa=2" >> $filepath
		        fi
		    fi
		    echo "wpa_key_mgmt=WPA-EAP " >>$filepath
		    if [ "$xml_encryption" == "TKIP" ]; then
		        echo "wpa_pairwise=TKIP" >> $filepath
		    else
			      if [ "$xml_encryption" == "AES" ]; then
			        echo "wpa_pairwise=CCMP" >> $filepath
			      fi
		    fi
		fi
}



#
#acconfig_setup
#
acconfig_setup()
{
     local addr="$FORM_ac_adress"
     local name="$FORM_ac_wtpname "
     local location="$FORM_ac_location"
     local proto="$FORM_protocol"
     local mtu="$FORM_forcemtu "
     local fileable="$FORM_fileable "
     local filesize="$FORM_filesize "
     local domain="$FORM_domain"
     local xml_config=`config_read /config/network/acconfig`
     xml_config=`config_setoption "$xml_config" AC_ADDRESSES "$addr"`
     xml_config=`config_setoption "$xml_config" WTP_NAME "$name"`
     xml_config=`config_setoption "$xml_config" WTP_LOCATION "$location"`
     xml_config=`config_setoption "$xml_config" WTP_LEV3_PROTOCOL "$proto"`
     xml_config=`config_setoption "$xml_config" WTP_FORCE_MTU "$mtu"`
     xml_config=`config_setoption "$xml_config" AC_LOG_FILE_ENABLE "$fileable"`
     xml_config=`config_setoption "$xml_config" AC_LOG_FILE_SIZE "$filesize"`
     xml_config=`config_setoption "$xml_config" WTP_AC_DOMAIN_NAME "$domain"`
     config_write /config/network/acconfig "$xml_config"
     if [ $? != 0 ]; then
         false
     else
         true
    fi
 }

#
#ac_setup()
#
ac_setup()
{
     local addr1="$FORM_ac_adress1"
     local addr2="$FORM_ac_adress2"
     local addr3="$FORM_ac_adress3"
     local addr4="$FORM_ac_adress4"
     local name="$FORM_ac_wtpname "
     local location="$FORM_ac_location"
     local proto="$FORM_protocol"
     local mtu="$FORM_forcemtu "
     local fileable="$FORM_fileable "
     local filesize="$FORM_filesize "
     local domain="$FORM_domain"
     local filepath=/jffs/config.wtp
     local wtpenable="${FORM_wtpenable:-no}"
     local xml_wtp=`config_read /config/system`
		 xml_wtp=`config_setoption "$xml_wtp" wtpenable "$wtpenable"`
		 config_write /config/system "$xml_wtp"

     echo "<AC_ADDRESSES>" > $filepath
     if [ "$addr1" ]; then
     	echo "$addr1" >> $filepath
     fi
     if [ "$addr2" ]; then
     	echo "$addr2" >> $filepath
     fi
     if [ "$addr3" ]; then
     	echo "$addr3" >> $filepath
     fi
     if [ "$addr4" ]; then
     	echo "$addr4" >> $filepath
     fi
     echo "</AC_ADDRESSES>" >> $filepath
     echo "</WTP_NAME> $name" >> $filepath
     echo "</WTP_LOCATION> $location" >> $filepath
     echo "</WTP_LEV3_PROTOCOL> $proto" >> $filepath
     echo "</WTP_FORCE_MTU> $mtu" >> $filepath
     echo "</AC_LOG_FILE_ENABLE> $fileable" >> $filepath
     echo "</AC_LOG_FILE_SIZE> $filesize" >> $filepath
     echo "</WTP_AC_DOMAIN_NAME> $domain" >> $filepath
}


#
#thinapmode_setup
#
thinapmode_setup()
{
	local xml_config=`config_read /config/system`
	xml_config=`config_setoption "$xml_config" workmode "0"`
	config_write /config/system "$xml_config"


	if [ $? != 0 ]; then
	   false
 	else
	   true
	fi
}
#
#fatapmode_setup
#
fatapmode_setup()
{
    local xml_config=`config_read /config/system`
    xml_config=`config_setoption "$xml_config" workmode "1"`
    config_write /config/system "$xml_config"
    if [ $? != 0 ]; then
        false
    else
       true
    fi
}

#
#vapnum_setup
#
vapnum_setup()
{
    local vap="$FORM_vapnum"
    local xml_config=`config_read /config/network`
    xml_config=`config_setoption "$xml_config" vapnum "$vap"`
    config_write /config/network "$xml_config"
    if [ $? != 0 ]; then
        false
    else
       true
    fi
}

#
#stp_setup
#
stp_setup()
{

    local portprio="$FORM_portprio"
    local setfd="$FORM_setfd"
    local sethello="$FORM_sethello"
    local maxage="$FORM_setmaxage"
    local stpenable="$FORM_stpenable"

    local xml_config=`config_read /config/network/advanset`
    xml_config=`config_setoption "$xml_config" portprio "$portprio"`
    xml_config=`config_setoption "$xml_config" setfd "$setfd"`
    xml_config=`config_setoption "$xml_config" sethello "$sethello"`
    xml_config=`config_setoption "$xml_config" maxage "$maxage"`
    xml_config=`config_setoption "$xml_config" stpenable "$stpenable"`

    config_write /config/network/advanset "$xml_config"
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
    
}

#
#wds_setup
#
wds_setup()
{

    local wdsenable="$FORM_wdsenable"
    local wdsif="$FORM_vapname"
    local wdsmac="$FORM_wds_mac"

    local xml_config=`config_read /config/network/wds`
    xml_config=`config_setoption "$xml_config" wdsenable "$wdsenable"`
    xml_config=`config_setoption "$xml_config" wdsif "$wdsif"`
    xml_config=`config_setoption "$xml_config" wdsmac "$wdsmac"`
    config_write /config/network/wds "$xml_config"
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
    
}

#
#vlan_setup
#
vlan_setup()
{

    for vlan in `seq 8`
    do
        eval create="$"FORM_create$vlan
        eval vid="$"FORM_vid$vlan
        eval vlanname="$"FORM_vlanname$vlan
        eval ifeth="$"FORM_ifeth$vlan
        eval ifeth1="$"FORM_ifeth$vlan$vlan
        eval ipaddr="$"FORM_vlanip$vlan
        eval netmask="$"FORM_vlanmask$vlan
        xml_config=`config_read /config/network/vlan$vlan`
        xml_config=`config_setoption "$xml_config" create "$create"`
        xml_config=`config_setoption "$xml_config" vid "$vid"`
        xml_config=`config_setoption "$xml_config" vlanname "$vlanname"`
        xml_config=`config_setoption "$xml_config" ipaddr "$ipaddr"`
        xml_config=`config_setoption "$xml_config" netmask "$netmask"`
        xml_config=`config_setoption "$xml_config" ifeth "$ifeth"`
        xml_config=`config_setoption "$xml_config" ifeth1 "$ifeth1"`
        config_write /config/network/vlan$vlan "$xml_config"
        
        
    done
    if [ $? != 0 ]; then
        false
    else
        true
    fi
}

#
#wmm_setup
#
wmm_setup()
{ 

    local vapname="$FORM_vap_name"
    local xml_num_str="echo $vapname | awk -F'p' '{print \$2}'"
		local xml_num=`eval $xml_num_str`
    local xml_vapnum=`config_read /config/network`
    xml_vapnum=`config_setoption "$xml_vapnum" vapnum $xml_num`
 		config_write /config/network "$xml_vapnum"
    
    local wmmenable="$FORM_wmmenable"
    local wtenable="$FORM_wtenable"
    local wvenable="$FORM_wvenable"
    
    local bgmint="$FORM_bgmint"
    local bgmaxt="$FORM_bgmaxt"
    local bgaift="$FORM_bgaift"
    local bgopbt="$FORM_bgopbt"
    local bgnoack="${FORM_bgnoack:-0}"

    
    local bemint="$FORM_bemint"
    local bemaxt="$FORM_bemaxt"
    local beaift="$FORM_beaift"
    local beopbt="$FORM_beopbt"
    local benoack="${FORM_benoack:-0}"

    
    local vdmint="$FORM_vdmint"
    local vdmaxt="$FORM_vdmaxt"
    local vdaift="$FORM_vdaift"
    local vdopbt="$FORM_vdopbt"
    local vdnoack="${FORM_vdnoack:-0}"

    
    local vcmint="$FORM_vcmint"
    local vcmaxt="$FORM_vcmaxt"
    local vcaift="$FORM_vcaift"
    local vcopbt="$FORM_vcopbt"
    local vcnoack="${FORM_vcnoack:-0}"

    
    local bgminf="$FORM_bgminf"
    local bgmaxf="$FORM_bgmaxf"
    local bgaiff="$FORM_bgaiff"
    local bgopbf="$FORM_bgopbf"

    
    local beminf="$FORM_beminf"
    local bemaxf="$FORM_bemaxf"
    local beaiff="$FORM_beaiff"
    local beopbf="$FORM_beopbf"

    
    local vdminf="$FORM_vdminf"
    local vdmaxf="$FORM_vdmaxf"
    local vdaiff="$FORM_vdaiff"
    local vdopbf="$FORM_vdopbf"

    
    local vcminf="$FORM_vcminf"
    local vcmaxf="$FORM_vcmaxf"
    local vcaiff="$FORM_vcaiff"
    local vcopbf="$FORM_vcopbf"

    local wtegress8="$FORM_wtegress8"
    local wtegress32="$FORM_wtegress32"
    local wtegress40="$FORM_wtegress40"
    local wtegress48="$FORM_wtegress48"
    local wtegress136="$FORM_wtegress136"
    local wtegress160="$FORM_wtegress160"
    local wtegress184="$FORM_wtegress184"
    local wtegress224="$FORM_wtegress224"
    
    local twegress8="$FORM_twegress8"
    local twegress32="$FORM_twegress32"
    local twegress40="$FORM_twegress40"
    local twegress48="$FORM_twegress48"
    local twegress136="$FORM_twegress136"
    local twegress160="$FORM_twegress160"
    local twegress184="$FORM_twegress184"
    local twegress224="$FORM_twegress224"
    
    local wvingress0="$FORM_wvingress0"
    local wvingress1="$FORM_wvingress1"
    local wvingress2="$FORM_wvingress2"
    local wvingress3="$FORM_wvingress3"
    
    local wvegress0="$FORM_wvegress0"
    local wvegress1="$FORM_wvegress1"
    local wvegress2="$FORM_wvegress2"
    local wvegress3="$FORM_wvegress3"
    local wvegress4="$FORM_wvegress4"
    local wvegress5="$FORM_wvegress5"
    local wvegress6="$FORM_wvegress6"
    local wvegress7="$FORM_wvegress7"
    
    local xml_config_wmm=`config_read /config/network/$vapname/wmm`
	    xml_config_wmm=`config_setoption "$xml_config_wmm" enable "$wmmenable"`
	    
	  if [ "$wmmenable" == "1" ];then
	    xml_config_wmm=`config_setoption "$xml_config_wmm" wtenable "$wtenable"`
	    xml_config_wmm=`config_setoption "$xml_config_wmm" wvenable "$wvenable"`
	    xml_config_wmm=`config_setoption "$xml_config_wmm" bgmint "$bgmint"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bgmaxt "$bgmaxt"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bgaift "$bgaift"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bgopbt "$bgopbt"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bgnoack "$bgnoack"`
      
      xml_config_wmm=`config_setoption "$xml_config_wmm" bemint "$bemint"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bemaxt "$bemaxt"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" beaift "$beaift"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" beopbt "$beopbt"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" benoack "$benoack"`
      
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdmint "$vdmint"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdmaxt "$vdmaxt"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdaift "$vdaift"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdopbt "$vdopbt"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdnoack "$vdnoack"`
      
      xml_config_wmm=`config_setoption "$xml_config_wmm" vcmint "$vcmint"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vcmaxt "$vcmaxt"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vcaift "$vcaift"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vcopbt "$vcopbt"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vcnoack "$vcnoack"`
      
      xml_config_wmm=`config_setoption "$xml_config_wmm" bgminf "$bgminf"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bgmaxf "$bgmaxf"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bgaiff "$bgaiff"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bgopbf "$bgopbf"`
 
      xml_config_wmm=`config_setoption "$xml_config_wmm" beminf "$beminf"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" bemaxf "$bemaxf"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" beaiff "$beaiff"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" beopbf "$beopbf"`
 
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdminf "$vdminf"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdmaxf "$vdmaxf"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdaiff "$vdaiff"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vdopbf "$vdopbf"`

      xml_config_wmm=`config_setoption "$xml_config_wmm" vcminf "$vcminf"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vcmaxf "$vcmaxf"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vcaiff "$vcaiff"`
      xml_config_wmm=`config_setoption "$xml_config_wmm" vcopbf "$vcopbf"` 
      if [ "$wtenable" == "1" ];then 
        xml_config_wmm=`config_setoption "$xml_config_wmm" wtegress8 "$wtegress8"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wtegress32 "$wtegress32"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wtegress40 "$wtegress40"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wtegress48 "$wtegress48"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wtegress136 "$wtegress136"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wtegress160 "$wtegress160"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wtegress184 "$wtegress184"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wtegress224 "$wtegress224"`
	      
	      xml_config_wmm=`config_setoption "$xml_config_wmm" twegress8 "$twegress8"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" twegress32 "$twegress32"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" twegress40 "$twegress40"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" twegress48 "$twegress48"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" twegress136 "$twegress136"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" twegress160 "$twegress160"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" twegress184 "$twegress184"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" twegress224 "$twegress224"`
      fi
      if [ "$wvenable" == "1" ];then
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvingress0 "$wvingress0"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvingress1 "$wvingress1"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvingress2 "$wvingress2"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvingress3 "$wvingress3"` 
	      
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvegress0 "$wvegress0"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvegress1 "$wvegress1"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvegress2 "$wvegress2"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvegress3 "$wvegress3"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvegress4 "$wvegress4"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvegress5 "$wvegress5"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvegress6 "$wvegress6"`
	      xml_config_wmm=`config_setoption "$xml_config_wmm" wvegress7 "$wvegress7"`
      fi
    fi
      
	    config_write /config/network/$vapname/wmm "$xml_config_wmm"
	    
	    
	    
	    
	    
	    if [ $? != 0 ]; then
		      false
	    else
		      true
	    fi
}

#
#bandwidth_setup
#
bandwidth_setup()
{
    local vapname="$FORM_vap_name"
    local xml_num_str="echo $vapname | awk -F'p' '{print \$2}'"
		local xml_num=`eval $xml_num_str`
    local xml_vapnum=`config_read /config/network`
    xml_vapnum=`config_setoption "$xml_vapnum" vapnum $xml_num`
 		config_write /config/network "$xml_vapnum"
    
    local apbw="$FORM_apbw"
    local apbw_send="$FORM_apbw_send"
    local apbwenable="$FORM_apbwenable"
    local estabwenable="$FORM_estabwenable"
    local stabwenable="$FORM_stabwenable"
    local estabw="$FORM_estabw"
    local estabw_send="$FORM_estabw_send"
    local sta1="$FORM_sta1"
    local stabw1="$FORM_stabw1"
    local stabw1_send="$FORM_stabw1_send"
    local sta2="$FORM_sta2"
    local stabw2="$FORM_stabw2"
    local stabw2_send="$FORM_stabw2_send"
    local sta3="$FORM_sta3"
    local stabw3="$FORM_stabw3"
    local stabw3_send="$FORM_stabw3_send"
    local sta4="$FORM_sta4"
    local stabw4="$FORM_stabw4"
    local stabw4_send="$FORM_stabw4_send"
    local sta5="$FORM_sta5"
    local stabw5="$FORM_stabw5"
    local stabw5_send="$FORM_stabw5_send"
    local sta6="$FORM_sta6"
    local stabw6="$FORM_stabw6"
    local stabw6_send="$FORM_stabw6_send"
    local sta7="$FORM_sta7"
    local stabw7="$FORM_stabw7"
    local stabw7_send="$FORM_stabw7_send"
    local sta8="$FORM_sta8"
    local stabw8="$FORM_stabw8"
    local stabw8_send="$FORM_stabw8_send"
    local sta9="$FORM_sta9"
    local stabw9="$FORM_stabw9"
    local stabw9_send="$FORM_stabw9_send"

    
    local xml_config_apbw=`config_read /config/network/$vapname/bandwidth/apbw`
    xml_config_apbw=`config_setoption "$xml_config_apbw" apbwenable "$apbwenable"`
    xml_config_apbw=`config_setoption "$xml_config_apbw" maxclientrate "$apbw"`
    xml_config_apbw=`config_setoption "$xml_config_apbw" maxclientrate_send "$apbw_send"`
	  config_write /config/network/$vapname/bandwidth/apbw "$xml_config_apbw"
	  
	  local xml_config_estabw=`config_read /config/network/$vapname/bandwidth/estabw`
    xml_config_estabw=`config_setoption "$xml_config_estabw" estabwenable "$estabwenable"`
    xml_config_estabw=`config_setoption "$xml_config_estabw" stabwenable "$stabwenable"`
    xml_config_estabw=`config_setoption "$xml_config_estabw" maxbw "$estabw"`
    xml_config_estabw=`config_setoption "$xml_config_estabw" maxbw_send "$estabw_send"`
	  config_write /config/network/$vapname/bandwidth/estabw "$xml_config_estabw"

	  if [ "$stabwenable" == "1" ];then
			  local xml_config_stabw1=`config_read /config/network/$vapname/bandwidth/stabw1`
		    xml_config_stabw1=`config_setoption "$xml_config_stabw1" mac "$sta1"`
		    xml_config_stabw1=`config_setoption "$xml_config_stabw1" maxbw "$stabw1"`
		    xml_config_stabw1=`config_setoption "$xml_config_stabw1" maxbw_send "$stabw1_send"`
			  config_write /config/network/$vapname/bandwidth/stabw1 "$xml_config_stabw1"
			  
			  local xml_config_stabw2=`config_read /config/network/$vapname/bandwidth/stabw2`
		    xml_config_stabw2=`config_setoption "$xml_config_stabw2" mac "$sta2"`
		    xml_config_stabw2=`config_setoption "$xml_config_stabw2" maxbw "$stabw2"`
		    xml_config_stabw2=`config_setoption "$xml_config_stabw2" maxbw_send "$stabw2_send"`
			  config_write /config/network/$vapname/bandwidth/stabw2 "$xml_config_stabw2"
			  
			  local xml_config_stabw3=`config_read /config/network/$vapname/bandwidth/stabw3`
		    xml_config_stabw3=`config_setoption "$xml_config_stabw3" mac "$sta3"`
		    xml_config_stabw3=`config_setoption "$xml_config_stabw3" maxbw "$stabw3"`
		    xml_config_stabw3=`config_setoption "$xml_config_stabw3" maxbw_send "$stabw3_send"`
			  config_write /config/network/$vapname/bandwidth/stabw3 "$xml_config_stabw3"
			  
			  local xml_config_stabw4=`config_read /config/network/$vapname/bandwidth/stabw4`
		    xml_config_stabw4=`config_setoption "$xml_config_stabw4" mac "$sta4"`
		    xml_config_stabw4=`config_setoption "$xml_config_stabw4" maxbw "$stabw4"`
		    xml_config_stabw4=`config_setoption "$xml_config_stabw4" maxbw_send "$stabw4_send"`
			  config_write /config/network/$vapname/bandwidth/stabw4 "$xml_config_stabw4"
			  
			  local xml_config_stabw5=`config_read /config/network/$vapname/bandwidth/stabw5`
		    xml_config_stabw5=`config_setoption "$xml_config_stabw5" mac "$sta5"`
		    xml_config_stabw5=`config_setoption "$xml_config_stabw5" maxbw "$stabw5"`
		    xml_config_stabw5=`config_setoption "$xml_config_stabw5" maxbw_send "$stabw5_send"`
			  config_write /config/network/$vapname/bandwidth/stabw5 "$xml_config_stabw5"
			  
			  local xml_config_stabw6=`config_read /config/network/$vapname/bandwidth/stabw6`
		    xml_config_stabw6=`config_setoption "$xml_config_stabw6" mac "$sta6"`
		    xml_config_stabw6=`config_setoption "$xml_config_stabw6" maxbw "$stabw6"`
		    xml_config_stabw6=`config_setoption "$xml_config_stabw6" maxbw_send "$stabw6_send"`
			  config_write /config/network/$vapname/bandwidth/stabw6 "$xml_config_stabw6"
			  
			  local xml_config_stabw7=`config_read /config/network/$vapname/bandwidth/stabw7`
		    xml_config_stabw7=`config_setoption "$xml_config_stabw7" mac "$sta7"`
		    xml_config_stabw7=`config_setoption "$xml_config_stabw7" maxbw "$stabw7"`
		    xml_config_stabw7=`config_setoption "$xml_config_stabw7" maxbw_send "$stabw7_send"`
			  config_write /config/network/$vapname/bandwidth/stabw7 "$xml_config_stabw7"
			  
			  local xml_config_stabw8=`config_read /config/network/$vapname/bandwidth/stabw8`
		    xml_config_stabw8=`config_setoption "$xml_config_stabw8" mac "$sta8"`
		    xml_config_stabw8=`config_setoption "$xml_config_stabw8" maxbw "$stabw8"`
		    xml_config_stabw8=`config_setoption "$xml_config_stabw8" maxbw_send "$stabw8_send"`
			  config_write /config/network/$vapname/bandwidth/stabw8 "$xml_config_stabw8"
			  
			  local xml_config_stabw9=`config_read /config/network/$vapname/bandwidth/stabw9`
		    xml_config_stabw9=`config_setoption "$xml_config_stabw9" mac "$sta9"`
		    xml_config_stabw9=`config_setoption "$xml_config_stabw9" maxbw "$stabw9"`
		    xml_config_stabw9=`config_setoption "$xml_config_stabw9" maxbw_send "$stabw9_send"`
			  config_write /config/network/$vapname/bandwidth/stabw9 "$xml_config_stabw9"
	  fi
	  if [ $? != 0 ]; then
		      false
	    else
		      true
	    fi
}


#
# advanceset_setup
#
advanceset_setup()
{   
    local autochannel="${FORM_autochannel:-0}"
    local shortgi="${FORM_shortgi:-0}"
    local extoffset="${FORM_extoffset:-0}"
    local dtim="$FORM_DTIM"
    local frag="$FORM_frag"
    local diversity="$FORM_diversity"
    local txantenna="$FORM_txantenna"
    local rxantenna="$FORM_rxantenna"
    local preamble="$FORM_preamble"
    local txpower="$FORM_txpower"
    local rts="$FORM_rts"
    local vlanenable="$FORM_vlanenable"
    local vid="$FORM_VID"
    local count="$FORM_count"
    local isolation="${FORM_isolation:-1}"
    local bcisolation="${FORM_bcisolation:-1}"
    local cwwmode="${FORM_cwwmode:-1}"

    local vapname="$FORM_vap_name"
    local xml_num_str="echo $vapname | awk -F'p' '{print \$2}'"
		local xml_num=`eval $xml_num_str`
    local xml_vapnum=`config_read /config/network`
    xml_vapnum=`config_setoption "$xml_vapnum" vapnum $xml_num`
 		config_write /config/network "$xml_vapnum"

    local xml_config=`config_read /config/network/$vapname`
    local xml_enable=`config_getoption "$xml_config" enable`
    local xml_if=`config_getoption "$xml_config" if`
    
    
    
    local xml_wlan_wifi=`config_getoption "$xml_config" wifi`
    local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
      xml_wifi=`config_setoption "$xml_wifi" autochannel "$autochannel"`
	    xml_wifi=`config_setoption "$xml_wifi" diversity "$diversity"`
	    xml_wifi=`config_setoption "$xml_wifi" txantenna "$txantenna"`
	     
	    local xml_txrt=`config_getoption "$xml_wifi" rxantenna` 
      if [ "$xml_txrt" == "$rxantenna" ];then	     
         xml_wifi=`config_setoption "$xml_wifi" rxantenna "$txantenna"`
      else
         xml_wifi=`config_setoption "$xml_wifi" rxantenna "$rxantenna"`
      fi

	    xml_wifi=`config_setoption "$xml_wifi" txpoff "$txpower"`
	    xml_wifi=`config_setoption "$xml_wifi" cwwmode "$cwwmode"`
	    xml_wifi=`config_setoption "$xml_wifi" extoffset "$extoffset"`
	    xml_wifi=`config_setoption "$xml_wifi" shortgi "$shortgi"`
      config_write /config/network/$xml_wlan_wifi "$xml_wifi"
      
      local rate_n_ht40ds=`config_getoption "$xml_config" rate_n_ht40ds`
      if [ "$shortgi" == "0" -a "$rate_n_ht40ds" == "300M" ]||[ "$shortgi" == "0" -a "$rate_n_ht40ds" == "150M" ];then
         xml_config=`config_setoption "$xml_config" rate_n_ht40ds "270M"`
      fi        
      
	    xml_config=`config_setoption "$xml_config" dtim "$dtim"`
	    xml_config=`config_setoption "$xml_config" frag "$frag"`
	    xml_config=`config_setoption "$xml_config" preamble "$preamble"`
	    xml_config=`config_setoption "$xml_config" rts "$rts"`
	    xml_config=`config_setoption "$xml_config" count "$count"`
      xml_config=`config_setoption "$xml_config" isolation "$isolation"`
      xml_config=`config_setoption "$xml_config" bcisolation "$bcisolation"`
	    config_write /config/network/$vapname "$xml_config"

	    

	    local xml_config_vlan=`config_read /config/network/$vapname/vlan`
	    local old_vid=`config_getoption "$xml_config_vlan" vid`
	    xml_config_vlan=`config_setoption "$xml_config_vlan" enable "$vlanenable"`
	    if [ "$vlanenable" == "0" ];then
	    		xml_config_vlan=`config_setoption "$xml_config_vlan" vid ""`
	    else
	    		xml_config_vlan=`config_setoption "$xml_config_vlan" vid "$vid"`
	    fi
	    config_write /config/network/$vapname/vlan "$xml_config_vlan"
      
      if [ "$xml_enable" == "yes" ];then
    	$BRCTL delif $BRLAN "$xml_if" > /dev/null 2>&1
    	$BRCTL delif br$old_vid "$xml_if" > /dev/null 2>&1
      fi
      
      if [ $? != 0 ]; then
		      false
	    else
		      true
	    fi
	    
	    

}

#
#syslogserver_setup
#
syslogserver_setup()
{

    local enable_sls="$FORM_enablesls"
    local ServerIP="$FORM_ServerIP"
   	local ServerPort="$FORM_ServerPort"
   	#############tmp code#########
   	local upip="$FORM_upip"
   	local uptime="$FORM_uptime"
   	local enableup="$FORM_enableup"
   	##############################
   	
    local xml_syslogserver=`config_read /config/system/log` 
    
    xml_syslogserver=`config_setoption "$xml_syslogserver" enable "$enable_sls"`
    
    if [ "$enable_sls" == "yes" ]; then			
	      xml_syslogserver=`config_setoption "$xml_syslogserver" sip "$ServerIP"`
	      xml_syslogserver=`config_setoption "$xml_syslogserver" sport "$ServerPort"`
	  fi
	  #############tmp code#########
	  xml_syslogserver=`config_setoption "$xml_syslogserver" upenable "$enableup"`
	  xml_syslogserver=`config_setoption "$xml_syslogserver" upip "$upip"`
	  xml_syslogserver=`config_setoption "$xml_syslogserver" uptime "$uptime"`
	  ##############################
    config_write /config/system/log "$xml_syslogserver"
	
	  if [ $? != 0 ]; then
		   false
	  else
		   true
	  fi
	
}
															  
#
#snmp_setup
#
snmp_setup()
{

  local snmpenable="$FORM_snmpenable"
  local syslocation="$FORM_syslocation"
  local syscontact="$FORM_syscontact"
  local publicCN="$FORM_publicCN"
  local privateCN="$FORM_privateCN"
  local TCN="$FORM_TCN"
  local snmpip="$FORM_snmpip"
  
  local htt="${FORM_htt}"
  local porthttp="$FORM_porthttp"
  local porthttps="$FORM_porthttps"
  
  local sshenable="${FORM_sshenable:-no}"
  local telnetenable="${FORM_telnetenable:-no}"
  
  local xml_snmp=`config_read /config/administrator/snmp` 
	xml_snmp=`config_setoption "$xml_snmp" enable "$snmpenable"`
	if [ "$snmpenable" == "1" ]; then
	    xml_snmp=`config_setoption "$xml_snmp" syslocation "$syslocation"`
	    xml_snmp=`config_setoption "$xml_snmp" syscontact "$syscontact"`
	    xml_snmp=`config_setoption "$xml_snmp" publicCN "$publicCN"`
	    xml_snmp=`config_setoption "$xml_snmp" privateCN "$privateCN"`
	    xml_snmp=`config_setoption "$xml_snmp" TCN "$TCN"`
	    xml_snmp=`config_setoption "$xml_snmp" snmpip "$snmpip"`
	fi
 	config_write /config/administrator/snmp "$xml_snmp"
 	
 	local xml_accessever=`config_read /config/administrator/accessever`
  xml_accessever=`config_setoption "$xml_accessever" enable "$htt"`
  xml_accessever=`config_setoption "$xml_accessever" porthttp "$porthttp"`
  xml_accessever=`config_setoption "$xml_accessever" porthttps "$porthttps"`
  config_write /config/administrator/accessever "$xml_accessever"

  local xml_ssh=`config_read /config/administrator/ssh`
  xml_ssh=`config_setoption "$xml_ssh" enable "$sshenable"`
  config_write /config/administrator/ssh "$xml_ssh"
  
  local xml_telnet=`config_read /config/administrator/telnet`
  xml_telnet=`config_setoption "$xml_telnet" enable "$telnetenable"`
  config_write /config/administrator/telnet "$xml_telnet"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
	
	
}


#
#dhcpserver_setup
#
dhcpserver_setup()
{

  local dhcpenable="$FORM_dhcpenable"
  local leasetime="$FORM_leasetime"
  local subnet="$FORM_subnet"
  local route="$FORM_route"
  local ipstart="$FORM_ipstart"
  local ipend="$FORM_ipend"

 	local xml_dhcp=`config_read /config/network/dhcp/dhcpserver`
	xml_dhcp=`config_setoption "$xml_dhcp" enable "$dhcpenable"`
	xml_dhcp=`config_setoption "$xml_dhcp" leasetime "$leasetime"`
	xml_dhcp=`config_setoption "$xml_dhcp" ipstart "$ipstart"`
	xml_dhcp=`config_setoption "$xml_dhcp" ipend "$ipend"`
	xml_dhcp=`config_setoption "$xml_dhcp" subnet "$subnet"`
	xml_dhcp=`config_setoption "$xml_dhcp" route "$route"`
	config_write /config/network/dhcp/dhcpserver "$xml_dhcp"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
	
	
}

#
#dhcpsnooping_setup
#
dhcpsnooping_setup()
{
  local dhcpsnoopenable="$FORM_dhcpsnoopenable"
  local dsif="$FORM_dsif"
  local info="$FORM_dsinfo"
	
	local xml_dhcpsnoop=`config_read /config/network/dhcp/dhcpsnooping`
	xml_dhcpsnoop=`config_setoption "$xml_dhcpsnoop" enable "$dhcpsnoopenable"`
	xml_dhcpsnoop=`config_setoption "$xml_dhcpsnoop" interface "$dsif"`
	xml_dhcpsnoop=`config_setoption "$xml_dhcpsnoop" info  "$info"`
	config_write /config/network/dhcp/dhcpsnooping "$xml_dhcpsnoop"
	if [ $? != 0 ]; then
		false
	else
		true
	fi
	
	
}



#
#igmpsnooping_setup
#
igmpsnooping_setup()
{
  local igmpsnoopenable="$FORM_igmpsnoopenable"
	
	local xml_igmpsnoop=`config_read /config/network/igmp/igmpsnooping`
	xml_igmpsnoop=`config_setoption "$xml_igmpsnoop" enable "$igmpsnoopenable"`
	config_write /config/network/igmp/igmpsnooping "$xml_igmpsnoop"
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

#
#mldsnooping_setup
#
mldsnooping_setup()
{
  local mldsnoopenable="$FORM_mldsnoopenable"
	
	local xml_mldsnoop=`config_read /config/network/mld/mldsnooping`
	xml_mldsnoop=`config_setoption "$xml_mldsnoop" enable "$mldsnoopenable"`
	config_write /config/network/mld/mldsnooping "$xml_mldsnoop"
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

#
#
#adminsec_setup
#
adminsec_setup()
{
    local idpolicy="$FORM_defpolicy"
    local icount="${FORM_rulecount:-0}"
    local xml_netrule=`config_read /config/netsecury/netfilter`
    local rule_tmp=""
  	xml_netrule=`config_setoption "$xml_netrule" inputdefault "$idpolicy"`
  	xml_netrule=`config_setoption "$xml_netrule" icount "$icount"`
  	if [ "$icount" != "0" ]; then
	  	for rulenum in `seq $icount`
	  	do
	  		eval rule_tmp="$"FORM_rule$rulenum
	  		xml_netrule=`config_setoption "$xml_netrule" "inrule$rulenum" "$rule_tmp"`
	  	done
  	fi
  	config_write /config/netsecury/netfilter "$xml_netrule"
  

}	

#
#netfilter_setup
#
netfilter_setup()
{
    local fdpolicy="$FORM_defpolicy"
    local fcount="${FORM_rulecount:-0}"
    local xml_netrule=`config_read /config/netsecury/netfilter`
    local rule_tmp="" 
  	xml_netrule=`config_setoption "$xml_netrule" forwardefault "$fdpolicy"`
  	xml_netrule=`config_setoption "$xml_netrule" fcount "$fcount"`
  	if [ "$fcount" != "0" ]; then
	  	for rulenum in `seq $fcount`
	  	do
	  		eval rule_tmp="$"FORM_rule$rulenum
	  		xml_netrule=`config_setoption "$xml_netrule" "forule$rulenum"  "$rule_tmp"`
	  	done
	  fi
  	config_write /config/netsecury/netfilter "$xml_netrule"
  

}													  

#
#intelligence_setup
#
intelligence_setup()
{
		local linkdetecteth0="$FORM_linkdetecteth0"
		local linkdetecteth1="$FORM_linkdetecteth1"
		local detectime="${FORM_detectime:-0}"
		local upip="$FORM_detectupip"
		local uptime="${FORM_detecuptime:-0}"
		local wifi0enable="${FORM_wifi0enable:-ignore}"
		local wifi1enable="${FORM_wifi1enable:-ignore}"
		local channelctime="${FORM_channelctime:-0}"
		local powercwifi0="$FORM_powercwifi0"
		local powercwifi1="$FORM_powercwifi1"
		local powerctime="${FORM_powerctime:-0}"
		local loadenable="${FORM_loadenable:-0}"
		local loadthreshold="$FORM_loadthreshold"
		
		local xml_linkdetect=`config_read /config/network/intelligence/linkdetect`
  	xml_linkdetect=`config_setoption "$xml_linkdetect" vap_eth0 $linkdetecteth0`
  	xml_linkdetect=`config_setoption "$xml_linkdetect" vap_eth1 $linkdetecteth1`
  	xml_linkdetect=`config_setoption "$xml_linkdetect" interval $detectime`
  	xml_linkdetect=`config_setoption "$xml_linkdetect" upip "$upip"`
		xml_linkdetect=`config_setoption "$xml_linkdetect" uptime "$uptime"`
  	config_write /config/network/intelligence/linkdetect "$xml_linkdetect"
  	
  	local xml_autochannel=`config_read /config/network/intelligence/autochannel`
  	xml_autochannel=`config_setoption "$xml_autochannel" channel_wifi0 $wifi0enable`
  	xml_autochannel=`config_setoption "$xml_autochannel" channel_wifi1 $wifi1enable`
  	xml_autochannel=`config_setoption "$xml_autochannel" interval $channelctime`
  	config_write /config/network/intelligence/autochannel "$xml_autochannel"
  	
  	local xml_autopower=`config_read /config/network/intelligence/autopower`
  	xml_autopower=`config_setoption "$xml_autopower" power_wifi0 $powercwifi0`
  	xml_autopower=`config_setoption "$xml_autopower" power_wifi1 $powercwifi1`
  	xml_autopower=`config_setoption "$xml_autopower" interval $powerctime`
  	config_write /config/network/intelligence/autopower "$xml_autopower"
  	
  	local xml_load=`config_read /config/network/intelligence/loadbalance`
		xml_load=`config_setoption "$xml_load" enable $loadenable`
		xml_load=`config_setoption "$xml_load" wifi0threshold $loadthreshold`
		config_write /config/network/intelligence/loadbalance "$xml_load"
		
		

}
	
# firewall module
# url_filter
# added by tangsiqi
url_filter()
{
	local nat_enable="$FORM_natable"
	
	local url_state="$FORM_url_filter"
	local rule_count="$FORM_rule_num"
	
	local url1="$FORM_url_address1"
	local url2="$FORM_url_address2"
	local url3="$FORM_url_address3"
	local url4="$FORM_url_address4"
	local url5="$FORM_url_address5"
	local url6="$FORM_url_address6"
	local url7="$FORM_url_address7"
	local url8="$FORM_url_address8"
	local url9="$FORM_url_address9"
	local url10="$FORM_url_address10"
	
        if [ "$FORM_natable" == "yes" ];then	
        	local xml_bnet=`config_read /config/network` 
                xml_bnet=`config_setoption "$xml_bnet" forward3 "1"`
        	config_write /config/network "$xml_bnet"
        fi	
	
	local url_xml=`config_read /config/firewall/urlfilter`
	url_xml=`config_setoption "$url_xml" status "$url_state"`
	url_xml=`config_setoption "$url_xml" rule_count "$rule_count"`
	
	url_xml=`config_setoption "$url_xml" url1 "$url1"`
	url_xml=`config_setoption "$url_xml" url2 "$url2"`
	url_xml=`config_setoption "$url_xml" url3 "$url3"`
	url_xml=`config_setoption "$url_xml" url4 "$url4"`
	url_xml=`config_setoption "$url_xml" url5 "$url5"`
	url_xml=`config_setoption "$url_xml" url6 "$url6"`
	url_xml=`config_setoption "$url_xml" url7 "$url7"`
	url_xml=`config_setoption "$url_xml" url8 "$url8"`
	url_xml=`config_setoption "$url_xml" url9 "$url9"`
	url_xml=`config_setoption "$url_xml" url10 "$url10"`
	config_write /config/firewall/urlfilter "$url_xml"
	
	local xml_nat=`config_read /config/firewall/nat`
	xml_nat=`config_setoption "$xml_nat" enable "$nat_enable"`
	config_write /config/firewall/nat "$xml_nat"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

#
#ppp_connect_setup()
#
ppp_connect_setup()
{
	local connect_mode="$FORM_ppp_connct_mode"
	local connect_status="${FORM_connect_status:-Disconnected}"

	local xml_3g_wan_connect_mode=`config_read /config/network/ap_3g_wan/PPP_Connection_Mode`
	local xml_3g_wan_connect_mode=`config_setoption "$xml_3g_wan_connect_mode" connect_mode "$connect_mode"`
	local xml_3g_wan_connect_mode=`config_setoption "$xml_3g_wan_connect_mode" connect_status "$connect_status"`
	config_write /config/network/ap_3g_wan/PPP_Connection_Mode "$xml_3g_wan_connect_mode"
}


#
# AP_3g_wan_setup()
# 
AP_3g_wan_setup()
{
	
	local ppp_connect_mode="$FORM_ppp_connect_mode"
	##############part2#########################
	
	local apn_mode="$FORM_apn_setting"
	local apn_content="$FORM_apn"
	
	local dial_no="$FORM_dial_num"
	
	local pdp_type="$FORM_pdp_type"
	local pdp_mode="$FORM_pdp_setting"
	local pdp_address="$FORM_pdp_address"
	
	##############part3#########################
	
	local auth_mode="$FORM_authentication"
	local user_name="$FORM_user_name"
	local passwd="$FORM_password"
	
	local idle_time_enable="$FORM_idle_time_setting"
	local max_idle_time="$FORM_max_idle_time"
	local mtu="$FORM_mtu"
	
	##############part4#########################
	
	local dns_mode="$FORM_dns"
	local primary_dns="$FORM_primary_dns"
	local secondary_dns="$FORM_secondary_dns"
	
	#####################################insert node#########################
	local xml_3g_wan_connect_mode=`config_read /config/network/ap_3g_wan/PPP_Connection_Mode`
	local xml_3g_wan_connect_mode=`config_setoption "$xml_3g_wan_connect_mode" connect_mode "$ppp_connect_mode"`
	config_write /config/network/ap_3g_wan/PPP_Connection_Mode "$xml_3g_wan_connect_mode"
	
	local xml_3g_wan_APN=`config_read /config/network/ap_3g_wan/APN`
	local xml_3g_wan_APN=`config_setoption "$xml_3g_wan_APN" apn_mode "$apn_mode"`
	local xml_3g_wan_APN=`config_setoption "$xml_3g_wan_APN" apn_content "$apn_content"`
	config_write /config/network/ap_3g_wan/APN "$xml_3g_wan_APN"
	
	local xml_3g_wan_dial=`config_read /config/network/ap_3g_wan/Dial_setting`
	local xml_3g_wan_dial=`config_setoption "$xml_3g_wan_dial" Dial_NO "$dial_no"`
	config_write /config/network/ap_3g_wan/Dial_setting "$xml_3g_wan_dial"
	
	local xml_3g_wan_PDP=`config_read /config/network/ap_3g_wan/PDP`
	local xml_3g_wan_PDP=`config_setoption "$xml_3g_wan_PDP" PDP_type "$pdp_type"`
	local xml_3g_wan_PDP=`config_setoption "$xml_3g_wan_PDP" PDP_mode "$pdp_mode"`
	local xml_3g_wan_PDP=`config_setoption "$xml_3g_wan_PDP" PDP_address "$pdp_address"`
	config_write /config/network/ap_3g_wan/PDP "$xml_3g_wan_PDP"
	
	local xml_3g_wan_PPP_Auth=`config_read /config/network/ap_3g_wan/PPP_Auth`
	local xml_3g_wan_PPP_Auth=`config_setoption "$xml_3g_wan_PPP_Auth" auth_mode "$auth_mode"`
	local xml_3g_wan_PPP_Auth=`config_setoption "$xml_3g_wan_PPP_Auth" user_name "$user_name"`
	local xml_3g_wan_PPP_Auth=`config_setoption "$xml_3g_wan_PPP_Auth" passwd "$passwd"`
	config_write /config/network/ap_3g_wan/PPP_Auth "$xml_3g_wan_PPP_Auth"
	
	local xml_3g_wan_PPP_setting=`config_read /config/network/ap_3g_wan/PPP_setting`
	local xml_3g_wan_PPP_setting=`config_setoption "$xml_3g_wan_PPP_setting" idle_time_enable "$idle_time_enable"`
	local xml_3g_wan_PPP_setting=`config_setoption "$xml_3g_wan_PPP_setting" max_idle_time "$max_idle_time"`
	local xml_3g_wan_PPP_setting=`config_setoption "$xml_3g_wan_PPP_setting" MTU "$mtu"`
	config_write /config/network/ap_3g_wan/PPP_setting "$xml_3g_wan_PPP_setting"
	
	local xml_3g_wan_DNS=`config_read /config/network/ap_3g_wan/DNS`
	local xml_3g_wan_DNS=`config_setoption "$xml_3g_wan_DNS" dns_mode "$dns_mode"`
	local xml_3g_wan_DNS=`config_setoption "$xml_3g_wan_DNS" Primary_dns "$primary_dns"`
	local xml_3g_wan_DNS=`config_setoption "$xml_3g_wan_DNS" second_dns "$secondary_dns"`
	config_write /config/network/ap_3g_wan/DNS "$xml_3g_wan_DNS"
	
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
	
}

#
#wlwan_setup
#
wlwan_setup()
{
	local wanenable="${FORM_wanenable:-no}"
	local ssidname="$FORM_ssidname"
	local security="$FORM_security"
	local authmode="$FORM_authmode"
	local etype="$FORM_etype"
	local cwepkey="$FORM_wepkey"
	local passkey1="$FORM_passkey1"
	local passkey2="$FORM_passkey2"
	local passkey3="$FORM_passkey3"
	local passkey4="$FORM_passkey4"
	local passphrase="$FORM_passphrase"
	local assocchan="${FORM_assocchan:-0}"
	local assocweight="${FORM_assocweight:-0}"
	local assocrssi="${FORM_assocrssi:-0}"
	
	local xml_vap=`config_read /config/network`
	local vapnum=`config_getoption "$xml_vap" vapnum`
	
	local xml_wlwan=`config_read /config/network/vap$vapnum`
	xml_wlwan=`config_setoption "$xml_wlwan" enable "$wanenable"`
	xml_wlwan=`config_setoption "$xml_wlwan" ssid "$ssidname"`
	xml_wlwan=`config_setoption "$xml_wlwan" sectype "$security"`
	xml_wlwan=`config_setoption "$xml_wlwan" assocchan "$assocchan"`
	xml_wlwan=`config_setoption "$xml_wlwan" assocweight "$assocweight"`
	xml_wlwan=`config_setoption "$xml_wlwan" assocrssi "$assocrssi"`
	config_write /config/network/vap$vapnum "$xml_wlwan"
	
	local xml_wep=`config_read /config/network/vap$vapnum/wep`
	xml_wep=`config_setoption "$xml_wep" wepmode "$authmode"`
	xml_wep=`config_setoption "$xml_wep" wepkeytype "$etype"`
	xml_wep=`config_setoption "$xml_wep" cwepkey "$cwepkey"`
	xml_wep=`config_setoption "$xml_wep" wepkey1 "$passkey1"`
	xml_wep=`config_setoption "$xml_wep" wepkey2 "$passkey2"`
	xml_wep=`config_setoption "$xml_wep" wepkey3 "$passkey3"`
	xml_wep=`config_setoption "$xml_wep" wepkey4 "$passkey4"`
	config_write /config/network/vap$vapnum/wep "$xml_wep"
	
	local xml_wpa=`config_read /config/network/vap$vapnum/wpa`
	xml_wpa=`config_setoption "$xml_wpa" wpapass "$passphrase"`
	config_write /config/network/vap$vapnum/wpa "$xml_wpa"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

#
#thin_wlwan_setup
#
thin_wlwan_setup()
{
	local wanenable="${FORM_wanenable:-no}"
	local ssidname="$FORM_ssidname"
	local security="$FORM_security"
	local authmode="$FORM_authmode"
	local etype="$FORM_etype"
	local cwepkey="$FORM_wepkey"
	local passkey1="$FORM_passkey1"
	local passkey2="$FORM_passkey2"
	local passkey3="$FORM_passkey3"
	local passkey4="$FORM_passkey4"
	local passphrase="$FORM_passphrase"

	local txpower="$FORM_txpower_sta"
	local region="$FORM_region"
	
	local xml_vap=`config_read /config/network`
	local vapnum=`config_getoption "$xml_vap" vapnum`
	
	local xml_wlwan=`config_read /config/network/vap$vapnum`
	xml_wlwan=`config_setoption "$xml_wlwan" enable "$wanenable"`
	xml_wlwan=`config_setoption "$xml_wlwan" txpower "$txpower"`
	xml_wlwan=`config_setoption "$xml_wlwan" ssid "$ssidname"`
	xml_wlwan=`config_setoption "$xml_wlwan" sectype "$security"`
	config_write /config/network/vap$vapnum "$xml_wlwan"
	
	local xml_wep=`config_read /config/network/vap$vapnum/wep`
	xml_wep=`config_setoption "$xml_wep" wepmode "$authmode"`
	xml_wep=`config_setoption "$xml_wep" wepkeytype "$etype"`
	xml_wep=`config_setoption "$xml_wep" cwepkey "$cwepkey"`
	xml_wep=`config_setoption "$xml_wep" wepkey1 "$passkey1"`
	xml_wep=`config_setoption "$xml_wep" wepkey2 "$passkey2"`
	xml_wep=`config_setoption "$xml_wep" wepkey3 "$passkey3"`
	xml_wep=`config_setoption "$xml_wep" wepkey4 "$passkey4"`
	config_write /config/network/vap$vapnum/wep "$xml_wep"
	
	
	local xml_wlan_wifi=`config_getoption "$xml_wlwan" wifi`
    local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
    
    xml_wifi=`config_setoption "$xml_wifi" region "$region"`
	config_write /config/network/$xml_wlan_wifi "$xml_wifi"
    
    local config_country_code=`config_read /config/network/country/$xml_wlan_wifi`
	config_country_code=`config_setoption "$config_country_code" region "$region"`
    config_write /config/network/country/$xml_wlan_wifi "$config_country_code"

	
	local xml_wpa=`config_read /config/network/vap$vapnum/wpa`
	xml_wpa=`config_setoption "$xml_wpa" wpapass "$passphrase"`
	config_write /config/network/vap$vapnum/wpa "$xml_wpa"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}


#
#locate_setup
#
locate_setup()
{
	local locate_enable="${FORM_locate_enable:-no}"
	local macfilter="${FORM_macfilter_enable:-no}"
	local macaddr="$FORM_macaddr"
	local region="$FORM_region"
	local scantype="$FORM_scantype"
	local scansize="$FORM_scansize"
	local interval="$FORM_interval"
	local ipaddr="$FORM_ipaddr"
	local port="$FORM_port"
	
	
	local xml_locate=`config_read /config/locate`
	xml_locate=`config_setoption "$xml_locate" enable "$locate_enable"`
	xml_locate=`config_setoption "$xml_locate" macfilter "$macfilter"`
	xml_locate=`config_setoption "$xml_locate" macaddr "$macaddr"`
	xml_locate=`config_setoption "$xml_locate" region "$region"`
	xml_locate=`config_setoption "$xml_locate" scantype "$scantype"`
	xml_locate=`config_setoption "$xml_locate" scansize "$scansize"`
	xml_locate=`config_setoption "$xml_locate" interval "$interval"`
	xml_locate=`config_setoption "$xml_locate" ipaddr "$ipaddr"`
	xml_locate=`config_setoption "$xml_locate" port "$port"`
	config_write /config/locate "$xml_locate"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}


#
#acktimeout_setup
#
acktimeout_setup()
{
	local wifiname="$FORM_wifiname_distance"
	local distance_wifi0="$FORM_distance_wifi0"
	local distance_wifi1="$FORM_distance_wifi1"
	
	if [ "$wifiname" == "wifi0" ];then
		local xml_wifi0=`config_read /config/network/wifi0`
		xml_wifi0=`config_setoption "$xml_wifi0" distance "$distance_wifi0"`
		config_write /config/network/wifi0 "$xml_wifi0"
		$ACKTIMEOUT wifi0 set $distance_wifi0 > /dev/null 2>&1
	fi
	
	if [ "$wifiname" == "wifi1" ];then
		local xml_wifi1=`config_read /config/network/wifi1`
		xml_wifi1=`config_setoption "$xml_wifi1" distance "$distance_wifi1"`
		config_write /config/network/wifi1 "$xml_wifi1"
		$ACKTIMEOUT wifi1 set $distance_wifi1 > /dev/null 2>&1
	fi
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}
