
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

    if [ "$user" == "$xml_username" -a "$pass" == "$xml_password" ]; then
        true
    else
        false
    fi
}

login_in()
{
	local xml_config=`config_read /config/system`
  xml_config_pass=`config_getoption "$xml_config" password`
	echo -e "$xml_config_pass\n$xml_config_pass" |$HTPASSWD -c /www/.htpasswd admin 2>/dev/null
}

conf_user_setup()
{

    local user="admin"
    local pass_old="$FORM_pass_old"
    local pass_new="$FORM_pass_new"
    local pass_new_retype="$FORM_pass_new_retype"

    if [ "$pass_new" != "$pass_new_retype" ]; then
        false
        return
    fi
		
		login_check "$user" "$pass_old"
    if [ $? != 0 ]; then
        false
        return
    fi
		
		echo -e "$pass_new\n$pass_new" |$HTPASSWD -c /www/.htpasswd admin 2>/dev/null
		
    local xml_config=`config_read /config/system`
    xml_config=`config_setoption "$xml_config" password "$pass_new"`
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
    true
}

#
# session_validate()
#
session_validate()
{
    true
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
    response_redirect lansetup.htm
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

    local xml_config=`config_read /config/network/lan`
    xml_config=`config_setoption "$xml_config" proto "$proto"`
    if [ "$proto" == "static" ]; then
        xml_config=`config_setoption "$xml_config" ipaddr "$ipaddr"`
        xml_config=`config_setoption "$xml_config" netmask "$netmask"`
    else
        xml_config=`config_setoption "$xml_config" dhcpcount "$count"`
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
    local mode="$FORM_mode"
    local rate="$FORM_rate"
    local rateb="$FORM_rateb"
    local beacon="$FORM_beacon" 

    local xml_config=`config_read /config/network/vap1`
    local xml_wlan_wifi=`config_getoption "$xml_config" wifi`
    local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
    xml_wifi=`config_setoption "$xml_wifi" channel1 "$channel1"`
    xml_wifi=`config_setoption "$xml_wifi" channel "$channel"`
    xml_wifi=`config_setoption "$xml_wifi" region "$region"`
    xml_wifi=`config_setoption "$xml_wifi" mode "$mode"`
    config_write /config/network/$xml_wlan_wifi "$xml_wifi"
    
    xml_config=`config_setoption "$xml_config" enable "$enable"`
    xml_config=`config_setoption "$xml_config" broadcast "$broadcast"`
    xml_config=`config_setoption "$xml_config" ssid "$ssid"`
    xml_config=`config_setoption "$xml_config" rate "$rate"`
    xml_config=`config_setoption "$xml_config" rateb "$rateb"`
	  xml_config=`config_setoption "$xml_config" beacon "$beacon"`
		config_write /config/network/vap1 "$xml_config"
    
    
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
      
}

#
# security_setup
#
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
    
    local xml_config=`config_read /config/network/vap1`
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
    config_write /config/network/vap1 "$xml_config"
    
    if [ $? != 0 ]; then
        false
    else
        true
    fi
    
    

}

#
# sys_wpapsk_config
#
sys_wpapsk_config()
{
    
    local filepath=/jffs/hostapd/a1
    local xml_lan=`config_read /config/network/lan`
    local xml_lan_if=`config_getoption "$xml_lan" if`
    local xml_wlan=`config_read /config/network/vap1`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_ssid=`config_getoption "$xml_wlan" ssid`
    local xml_encryption=`config_getoption "$xml_wlan" encryption`
    local xml_passphrase=`config_getoption "$xml_wlan" passphrase`
    local xml_security=`config_getoption "$xml_wlan" security`    
    sys_lan_ipaddr=`sys_get_interface_ipaddr "$xml_lan_if"`
        
    echo "interface=$xml_if"  > $filepath
    echo "bridge=$xml_lan_if" >>$filepath
    echo "driver=madwifi" >> $filepath
    echo "logger_syslog=-1" >> $filepath
    echo "logger_syslog_level=2" >> $filepath
    echo "logger_stdout=-1" >> $filepath
    echo "logger_stdout_level=2" >> $filepath
    echo "dump_file=/tmp/hostapd.dump" >> $filepath
    echo "ssid=$xml_ssid" >> $filepath
    echo "eapol_key_index_workaround=0" >> $filepath
    echo "own_ip_addr=$sys_lan_ipaddr" >> $filepath
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
    
    if [ $? != 0 ]; then
        false
    else
        true
    fi
}

#
# sys_wparadius_config
#
sys_wparadius_config()
{
    local filepath=/jffs/hostapd/b1
    local xml_lan=`config_read /config/network/lan`
    local xml_lan_if=`config_getoption "$xml_lan" if`
    local xml_wlan=`config_read /config/network/vap1`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_ssid=`config_getoption "$xml_wlan" ssid`
    local xml_encryption=`config_getoption "$xml_wlan" encryption`
    local xml_wlan_authmode=`config_getoption "$xml_wlan" authmode`
    local xml_security=`config_getoption "$xml_wlan" security`
    
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
    echo "bridge=$xml_lan_if" >>$filepath
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
    echo "own_ip_addr=$sys_lan_ipaddr" >>$filepath
    
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
  local dsif="$FORM_dsif"
  
  local ipend="$FORM_ipend"
  local ip_address="$FORM_ip_address"
  local subnet_mask="$FORM_subnet_mask"

 	local xml_dhcp=`config_read /config/network/dhcp/dhcpserver`
	xml_dhcp=`config_setoption "$xml_dhcp" enable "$dhcpenable"`
	xml_dhcp=`config_setoption "$xml_dhcp" leasetime "$leasetime"`
	xml_dhcp=`config_setoption "$xml_dhcp" ipstart "$ipstart"`
	xml_dhcp=`config_setoption "$xml_dhcp" ipend "$ipend"`
	xml_dhcp=`config_setoption "$xml_dhcp" subnet "$subnet"`
	xml_dhcp=`config_setoption "$xml_dhcp" route "$route"`
	config_write /config/network/dhcp/dhcpserver "$xml_dhcp"
	
	local xml_lan=`config_read /config/network/lan`
	xml_lan=`config_setoption "$xml_lan" ipaddr $ip_address`
	xml_lan=`config_setoption "$xml_lan" netmask $subnet_mask`
	config_write /config/network/lan "$xml_lan"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
	
	
}

# firewall module
# dos_filter
# added by tangsiqi
dos_filter()
{
	local dos_state="$FORM_dos_firewall"
	local dos_xml=`config_read /config/firewall/dosfilter`
	dos_xml=`config_setoption "$dos_xml" status "$dos_state"`
	config_write /config/firewall/dosfilter "$dos_xml"
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

# firewall module
# ip_filter
# added by tangsiqi
ip_filter()
{
	local ip_state="$FORM_ip_address_filter"
	
	local ip_start1="$FORM_ip1_address1"
	local ip_end1="$FORM_ip2_address1"
	local ip_start2="$FORM_ip1_address2"
	local ip_end2="$FORM_ip2_address2"
	local ip_start3="$FORM_ip1_address3"
	local ip_end3="$FORM_ip2_address3"
	local ip_start4="$FORM_ip1_address4"
	local ip_end4="$FORM_ip2_address4"
	local ip_start5="$FORM_ip1_address5"
	local ip_end5="$FORM_ip2_address5"
	local ip_start6="$FORM_ip1_address6"
	local ip_end6="$FORM_ip2_address6"
	local ip_start7="$FORM_ip1_address7"
	local ip_end7="$FORM_ip2_address7"
	local ip_start8="$FORM_ip1_address8"
	local ip_end8="$FORM_ip2_address8"
	local ip_start9="$FORM_ip1_address9"
	local ip_end9="$FORM_ip2_address9"
	local ip_start10="$FORM_ip1_address10"
	local ip_end10="$FORM_ip2_address10"
	
	local rule_count="$FORM_rule_num"
	
	local ip_xml=`config_read /config/firewall/ipfilter`
	ip_xml=`config_setoption "$ip_xml" status "$ip_state"`
	ip_xml=`config_setoption "$ip_xml" rule_count "$rule_count"`
	
	ip_xml=`config_setoption "$ip_xml" ipaddr1_start "$ip_start1"`
	ip_xml=`config_setoption "$ip_xml" ipaddr1_end "$ip_end1"`
	ip_xml=`config_setoption "$ip_xml" ipaddr2_start "$ip_start2"`
	ip_xml=`config_setoption "$ip_xml" ipaddr2_end "$ip_end2"`
	ip_xml=`config_setoption "$ip_xml" ipaddr3_start "$ip_start3"`
	ip_xml=`config_setoption "$ip_xml" ipaddr3_end "$ip_end3"`
	ip_xml=`config_setoption "$ip_xml" ipaddr4_start "$ip_start4"`
	ip_xml=`config_setoption "$ip_xml" ipaddr4_end "$ip_end4"`
	ip_xml=`config_setoption "$ip_xml" ipaddr5_start "$ip_start5"`
	ip_xml=`config_setoption "$ip_xml" ipaddr5_end "$ip_end5"`
	ip_xml=`config_setoption "$ip_xml" ipaddr6_start "$ip_start6"`
	ip_xml=`config_setoption "$ip_xml" ipaddr6_end "$ip_end6"`
	ip_xml=`config_setoption "$ip_xml" ipaddr7_start "$ip_start7"`
	ip_xml=`config_setoption "$ip_xml" ipaddr7_end "$ip_end7"`
	ip_xml=`config_setoption "$ip_xml" ipaddr8_start "$ip_start8"`
	ip_xml=`config_setoption "$ip_xml" ipaddr8_end "$ip_end8"`
	ip_xml=`config_setoption "$ip_xml" ipaddr9_start "$ip_start9"`
	ip_xml=`config_setoption "$ip_xml" ipaddr9_end "$ip_end9"`
	ip_xml=`config_setoption "$ip_xml" ipaddr10_start "$ip_start10"`
	ip_xml=`config_setoption "$ip_xml" ipaddr10_end "$ip_end10"`
	config_write /config/firewall/ipfilter "$ip_xml"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

# firewall module
# mac_filter
# added by tangsiqi
mac_filter()
{
	local mac_state="$FORM_mac_address_filter"
	local rule_count="$FORM_rule_num"
	
	local mac1="$FORM_mac_address1"
	local mac2="$FORM_mac_address2"
	local mac3="$FORM_mac_address3"
	local mac4="$FORM_mac_address4"
	local mac5="$FORM_mac_address5"
	local mac6="$FORM_mac_address6"
	local mac7="$FORM_mac_address7"
	local mac8="$FORM_mac_address8"
	local mac9="$FORM_mac_address9"
	local mac10="$FORM_mac_address10"
	
	local mac_xml=`config_read /config/firewall/macfilter`
	mac_xml=`config_setoption "$mac_xml" status "$mac_state"`
	mac_xml=`config_setoption "$mac_xml" rule_count "$rule_count"`
	
	mac_xml=`config_setoption "$mac_xml" mac1 "$mac1"`
	mac_xml=`config_setoption "$mac_xml" mac2 "$mac2"`
	mac_xml=`config_setoption "$mac_xml" mac3 "$mac3"`
	mac_xml=`config_setoption "$mac_xml" mac4 "$mac4"`
	mac_xml=`config_setoption "$mac_xml" mac5 "$mac5"`
	mac_xml=`config_setoption "$mac_xml" mac6 "$mac6"`
	mac_xml=`config_setoption "$mac_xml" mac7 "$mac7"`
	mac_xml=`config_setoption "$mac_xml" mac8 "$mac8"`
	mac_xml=`config_setoption "$mac_xml" mac9 "$mac9"`
	mac_xml=`config_setoption "$mac_xml" mac10 "$mac10"`
	config_write /config/firewall/macfilter "$mac_xml"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

# firewall module
# url_filter
# added by tangsiqi
url_filter()
{
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
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

# firewall module
# DMZ_filter
# added by tangsiqi
dmz_host()
{
	local dmz_state="$FORM_enabled"
	local dmz_ip="$FORM_ip_address"
	local dma_xml=`config_read /config/firewall/dmz`
	dma_xml=`config_setoption "$dma_xml" status "$dmz_state"`
	dma_xml=`config_setoption "$dma_xml" dmz_ip "$dmz_ip"`
	config_write /config/firewall/dmz "$dma_xml"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

# firewall module
# virtual_server
# added by tangsiqi
virtual_server()
{
	local rule_count="$FORM_rule_num"
	
	local server_enable1="$FORM_enabled1"
	local server_private_ip1="$FORM_private_ip1"
	local server_private_port1="$FORM_private_port1"
	local server_protocal_type1="$FORM_type1"
	local server_public_port1="$FORM_public_port1"
	
	local server_enable2="$FORM_enabled2"
	local server_private_ip2="$FORM_private_ip2"
	local server_private_port2="$FORM_private_port2"
	local server_protocal_type2="$FORM_type2"
	local server_public_port2="$FORM_public_port2"
	
	local server_enable3="$FORM_enabled3"
	local server_private_ip3="$FORM_private_ip3"
	local server_private_port3="$FORM_private_port3"
	local server_protocal_type3="$FORM_type"
	local server_public_port3="$FORM_public_port3"
	
	local server_enable4="$FORM_enabled4"
	local server_private_ip4="$FORM_private_ip4"
	local server_private_port4="$FORM_private_port4"
	local server_protocal_type4="$FORM_type4"
	local server_public_port4="$FORM_public_port4"
	
	local server_enable5="$FORM_enabled5"
	local server_private_ip5="$FORM_private_ip5"
	local server_private_port5="$FORM_private_port5"
	local server_protocal_type5="$FORM_type5"
	local server_public_port5="$FORM_public_port5"
	
	local server_enable6="$FORM_enabled6"
	local server_private_ip6="$FORM_private_ip6"
	local server_private_port6="$FORM_private_port6"
	local server_protocal_type6="$FORM_type6"
	local server_public_port6="$FORM_public_port6"
	
	local server_enable7="$FORM_enabled7"
	local server_private_ip7="$FORM_private_ip7"
	local server_private_port7="$FORM_private_port7"
	local server_protocal_type7="$FORM_type7"
	local server_public_port7="$FORM_public_port7"
	
	local server_enable8="$FORM_enabled8"
	local server_private_ip8="$FORM_private_ip8"
	local server_private_port8="$FORM_private_port8"
	local server_protocal_type8="$FORM_type8"
	local server_public_port8="$FORM_public_port8"
	
	local server_enable9="$FORM_enabled9"
	local server_private_ip9="$FORM_private_ip9"
	local server_private_port9="$FORM_private_port9"
	local server_protocal_type9="$FORM_type9"
	local server_public_port9="$FORM_public_port9"
	
	local server_enable10="$FORM_enabled10"
	local server_private_ip10="$FORM_private_ip10"
	local server_private_port10="$FORM_private_port10"
	local server_protocal_type10="$FORM_type10"
	local server_public_port10="$FORM_public_port10"
	
	local virtual_server_xml=`config_read /config/firewall/virtual_server`
	virtual_server_xml=`config_setoption "$virtual_server_xml" rule_count "$rule_count"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable1 "$server_enable1"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip1 "$server_private_ip1"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port1 "$server_private_port1"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type1 "$server_protocal_type1"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port1 "$server_public_port1"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable2 "$server_enable2"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip2 "$server_private_ip2"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port2 "$server_private_port2"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type2 "$server_protocal_type2"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port2 "$server_public_port2"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable3 "$server_enable3"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip3 "$server_private_ip3"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port3 "$server_private_port3"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type3 "$server_protocal_type3"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port3 "$server_public_port3"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable4 "$server_enable4"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip4 "$server_private_ip4"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port4 "$server_private_port4"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type4 "$server_protocal_type4"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port4 "$server_public_port4"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable5 "$server_enable5"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip5 "$server_private_ip5"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port5 "$server_private_port5"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type5 "$server_protocal_type5"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port5 "$server_public_port5"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable6 "$server_enable6"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip6 "$server_private_ip6"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port6 "$server_private_port6"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type6 "$server_protocal_type6"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port6 "$server_public_port6"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable7 "$server_enable7"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip7 "$server_private_ip7"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port7 "$server_private_port7"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type7 "$server_protocal_type7"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port7 "$server_public_port7"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable8 "$server_enable8"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip8 "$server_private_ip8"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port8 "$server_private_port8"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type8 "$server_protocal_type8"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port8 "$server_public_port8"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable9 "$server_enable9"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip9 "$server_private_ip9"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port9 "$server_private_port9"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type9 "$server_protocal_type9"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port9 "$server_public_port9"`
	
	virtual_server_xml=`config_setoption "$virtual_server_xml" enable10 "$server_enable10"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_ip10 "$server_private_ip10"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" private_port10 "$server_private_port10"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" protocal_type10 "$server_protocal_type10"`
	virtual_server_xml=`config_setoption "$virtual_server_xml" public_port10 "$server_public_port10"`
	
	config_write /config/firewall/virtual_server "$virtual_server_xml"
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
	local ipselect="$FORM_ipselect"
	local ip_address="$FORM_ip_address"
	local subnet_mask="$FORM_subnet_mask"
	local gateway="$FORM_gateway"
	local dnsselect="$FORM_dnsselect"
	local primarydns="$FORM_primarydns"
	local secondns="$FORM_secondns"
	
	
	local xml_wlwan=`config_read /config/network/wlwan`
	xml_wlwan=`config_setoption "$xml_wlwan" enable "$wanenable"`
	xml_wlwan=`config_setoption "$xml_wlwan" ssid "$ssidname"`
	xml_wlwan=`config_setoption "$xml_wlwan" sectype "$security"`
	config_write /config/network/wlwan "$xml_wlwan"
	
	local xml_wep=`config_read /config/network/wlwan/wep`
	xml_wep=`config_setoption "$xml_wep" wepmode "$authmode"`
	xml_wep=`config_setoption "$xml_wep" wepkeytype "$etype"`
	xml_wep=`config_setoption "$xml_wep" cwepkey "$cwepkey"`
	xml_wep=`config_setoption "$xml_wep" wepkey1 "$passkey1"`
	xml_wep=`config_setoption "$xml_wep" wepkey2 "$passkey2"`
	xml_wep=`config_setoption "$xml_wep" wepkey3 "$passkey3"`
	xml_wep=`config_setoption "$xml_wep" wepkey4 "$passkey4"`
	config_write /config/network/wlwan/wep "$xml_wep"
	
	local xml_wpa=`config_read /config/network/wlwan/wpa`
	xml_wpa=`config_setoption "$xml_wpa" wpapass "$passphrase"`
	config_write /config/network/wlwan/wpa "$xml_wpa"
	
	local xml_ip=`config_read /config/network/wlwan/ip`
	xml_ip=`config_setoption "$xml_ip" iptype "$ipselect"`
	xml_ip=`config_setoption "$xml_ip" ipaddr "$ip_address"`
	xml_ip=`config_setoption "$xml_ip" netmask "$subnet_mask"`
	xml_ip=`config_setoption "$xml_ip" gateway "$gateway"`
	config_write /config/network/wlwan/ip "$xml_ip"
	
	local xml_dns=`config_read /config/network/wlwan/dns`
	xml_dns=`config_setoption "$xml_dns" dnstype "$dnsselect"`
	xml_dns=`config_setoption "$xml_dns" primarydns "$primarydns"`
	xml_dns=`config_setoption "$xml_dns" seconddns "$secondns"`
	config_write /config/network/wlwan/dns "$xml_dns"
	
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

#3g_wan_setup
# 3g wan
# added by tangsiqi
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
#wanselect_setup
#
wanselect_setup()
{
	local wanselect="$FORM_wan_select"
	
	local xml_wan=`config_read /config/network/wan`
  xml_wan=`config_setoption "$xml_wan" wan_select $wanselect`
  config_write /config/network/wan "$xml_wan"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

#
#voip_setup()
#
voip_setup()
{
	local register_server="$FORM_sip_register_server"
	local proxy_server="$FORM_sip_proxy_server"
	local port="$FORM_port"
	local author_user_name="$FORM_author_user_name"
	local author_password="$FORM_author_password"
	local user_name="$FORM_user_name"
	local status="$FORM_status"
	
	local xml_voip=`config_read /config/voip`
	xml_voip=`config_setoption "$xml_voip" register_server "$register_server"`
	xml_voip=`config_setoption "$xml_voip" proxy_server "$proxy_server"`
	xml_voip=`config_setoption "$xml_voip" port "$port"`
	xml_voip=`config_setoption "$xml_voip" auth_user "$author_user_name"`
	xml_voip=`config_setoption "$xml_voip" auth_pass "$author_password"`
	xml_voip=`config_setoption "$xml_voip" user "$user_name"`
	xml_voip=`config_setoption "$xml_voip" status "$status"`
	config_write /config/voip "$xml_voip"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}

#
#set_3g_setup()
#
set_3g_setup()
{
	local network_type="$FORM_network_type"
	
	local xml_set3g=`config_read /config/network/set_3g`
	xml_set3g=`config_setoption "$xml_set3g" mode "$network_type"`
	config_write /config/network/set_3g "$xml_set3g"
	
	if [ $? != 0 ]; then
		false
	else
		true
	fi
}