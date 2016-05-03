#!/bin/sh

#
#include_js(js_name)
#
include_js()
{
   local xml_language=`config_read /config/session 2>/dev/null`
   local xml_language_type=`config_getoption "$xml_language" language`
   if [ -n "$xml_language_type" ];then
        echo "<script language=\"javascript\" src=\"js/$xml_language_type/$1\"></script>"
	 else
	 			echo "<script language=\"javascript\" src=\"js/en/$1\"></script>"
	 fi
	 echo "<script language=\"javascript\" src=\"js/display.js\"></script>"
}

#
# sys_get_interface_mac(interface)
#
sys_get_interface_mac()
{
    local interface="$1"
    $IFCONFIG $interface | grep -o -i -e "hwaddr [0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}" | cut -d" " -f2
}

#
# sys_get_interface_ipaddr(interface)
#
sys_get_interface_ipaddr()
{
    local interface="$1"
    $IFCONFIG $interface | grep -o -i -e "inet addr:[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}" | cut -d: -f2
}

#
# sys_get_interface_netmask(interface)
#
sys_get_interface_netmask()
{
    local interface="$1"
    $IFCONFIG $interface | grep -o -i -e "mask:[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}" | cut -d: -f2
}

#
# sys_get_interface_gateway(interface)
#
sys_get_interface_gateway()
{
    local interface="$1"
    local default_routes=`$ROUTE -n | grep $interface | grep -e "^0.0.0.0"`
    echo "$default_routes" | grep -o -i -e "[1-9][0-9]\{1,2\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}"
}

#
#sys_user_setup
#
sys_user_setup()
{
    local xml_config=`config_read /config/system`
    local xml_username=`config_getoption "$xml_config" username`
    local xml_password=`config_getoption "$xml_config" password`

    local awk_program="BEGIN{FS=\":\"; OFS=\":\";} /^$xml_username:/{\$2=\"$xml_password\";} {print;}"
    cat /etc/passwd | awk "$awk_program" > /tmp/passwd
    mv /tmp/passwd /etc/passwd
}

#
#sys_lan_start
#
sys_lan_start()
{
    local xml_lan=`config_read /config/network/lan`
    local xml_if=`config_getoption "$xml_lan" if`
    local xml_ipaddr=`config_getoption "$xml_lan" ipaddr`
    local xml_netmask=`config_getoption "$xml_lan" netmask`
    local xml_gateway=`config_getoption "$xml_lan" gateway`
    
    local xml_sroute=`config_read /config/network/sroute`
    local xml_sroute_route1=`config_getoption "$xml_sroute" route1`
    
    [ -n "$xml_sroute_route1" ] && $ROUTE add -net default gw "$xml_sroute_route1" > /dev/null 2>&1
    
    $IFCONFIG $1 "$xml_ipaddr" netmask "$xml_netmask" > /dev/null 2>&1
}

#
#sys_lan_setup
#
sys_lan_setup()
{
    local xml_lan=`config_read /config/network/lan`
    local xml_if=`config_getoption "$xml_lan" if`
    local xml_type=`config_getoption "$xml_lan" type`
    local xml_proto=`config_getoption "$xml_lan" proto`
    local xml_ipaddr=`config_getoption "$xml_lan" ipaddr`
    local xml_netmask=`config_getoption "$xml_lan" netmask`
    local xml_lan_dhcpcount=`config_getoption "$xml_lan" dhcpcount`
    
    local xml_sroute=`config_read /config/network/sroute`
    local xml_sroute_route1=`config_getoption "$xml_sroute" route1`

    if [ "$xml_proto" == "static" ]; then
        $IFCONFIG "$xml_if" "$xml_ipaddr" netmask "$xml_netmask" > /dev/null 2>&1
    elif [ "$xml_proto" == "dhcp" ]; then
        $KILLALL udhcpc >/dev/null
        $UDHCPC -i "$xml_if" -t $xml_lan_dhcpcount -b > /dev/null 2>&1
    fi
    
    [ -n "$xml_sroute_route1" ] && $ROUTE add -net default gw "$xml_sroute_route1" > /dev/null 2>&1    
    
}

#
#wired_if_conf
#
wired_if_conf()
{
    local xml_eth0=`config_read /config/network/eth0`
    local xml_eth0_ip=`config_getoption "$xml_eth0" ip`
    local xml_eth0_netmask=`config_getoption "$xml_eth0" netmask`
    local xml_eth0_fw2=`config_getoption "$xml_eth0" forward2`
    
    local xml_eth1=`config_read /config/network/eth1`
    local xml_eth1_ip=`config_getoption "$xml_eth1" ip`
    local xml_eth1_netmask=`config_getoption "$xml_eth1" netmask`
    local xml_eth1_fw2=`config_getoption "$xml_eth1" forward2`

    [ -n "$xml_eth0_ip" ] && [ -n "$xml_eth0_netmask" ] && $IFCONFIG eth0 "$xml_eth0_ip" netmask "$xml_eth0_netmask" > /dev/null 2>&1
    if [ $? == 0 ]; then
		    if [ "$xml_eth0_fw2" == "yes" ]; then
					    $BRCTL addif $BRLAN eth0 > /dev/null 2>&1
			  else
			       $BRCTL delif $BRLAN eth0 > /dev/null 2>&1
			  fi
	  fi
    
    if [ "$DEV_eth1" == "enable" ];then
      [ -n "$xml_eth1_ip" ] && [ -n "$xml_eth1_netmask" ] && $IFCONFIG eth1 "$xml_eth1_ip" netmask "$xml_eth1_netmask" > /dev/null 2>&1
      if [ $? == 0 ]; then 
	       if [ "$xml_eth1_fw2" == "yes" ]; then
				    $BRCTL addif $BRLAN eth1 > /dev/null 2>&1
				 else
				    $BRCTL delif $BRLAN eth1 > /dev/null 2>&1
				 fi
			fi
    fi
  
}


#
#sys_get_currenttime
#
sys_get_currenttime()
{ 
    local xml_config=`config_read /config/system`
    local xml_timezone=`config_getoption "$xml_config" timezone`   
    export TZ="$xml_timezone"
    date -R 
}

#
#sys_vap_startup()
#
sys_vap_startup()
{
		$WLANCONFIG ath0 create wlandev wifi0 wlanmode ap > /dev/null 2>&1
		$WLANCONFIG ath1 create wlandev wifi0 wlanmode sta nosbeacon > /dev/null 2>&1
		$IFCONFIG ath1 down > /dev/null 2>&1

}

#
#sys_vap_setup
#
sys_vap_setup()
{
    local xml_wlan=`config_read /config/network/vap1`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_enable=`config_getoption "$xml_wlan" enable`
    local xml_ssid=`config_getoption "$xml_wlan" ssid`
    local xml_broadcast=`config_getoption "$xml_wlan" broadcast`
    local xml_wlanmode=`config_getoption "$xml_wlan" wlanmode`
    local xml_wlan_rate=`config_getoption "$xml_wlan" rate`
    local xml_wlan_rateb=`config_getoption "$xml_wlan" rateb`
    local xml_wlan_beacon=`config_getoption "$xml_wlan" beacon`
    
    local xml_wlan_wifi=`config_getoption "$xml_wlan" wifi`
		local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
		local xml_wifi_channel=`config_getoption "$xml_wifi" channel`
		local xml_wifi_channel1=`config_getoption "$xml_wifi" channel1`
		local xml_mode=`config_getoption "$xml_wifi" mode`
		local xml_region=`config_getoption "$xml_wifi" region`

        if [ "$xml_enable" == "yes" ]; then
        		$IFCONFIG "$xml_if" down > /dev/null 2>&1
            [ -n "$xml_ssid" ] && $IWCONFIG "$xml_if" essid "$xml_ssid" > /dev/null 2>&1
            $IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
            $IWPRIV "$xml_if" pureg 0 > /dev/null 2>&1
		        if [ "$xml_mode" == "11b" ]; then
		            $IWPRIV "$xml_if" mode "$xml_mode" > /dev/null 2>&1
		        elif [ "$xml_mode" == "bg" ]; then
		            $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
		        else
		            $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
		            $IWPRIV "$xml_if" pureg 1 > /dev/null 2>&1
		        fi
		        if [ "$xml_region" == "Asia" -o "$xml_region" == "Europe" ]; then       
		            $IWCONFIG "$xml_if" channel "$xml_wifi_channel1" > /dev/null 2>&1
		        elif [ "$xml_region" == "America" ]; then    
		            $IWCONFIG "$xml_if" channel "$xml_wifi_channel" > /dev/null 2>&1
		        else     
		            $IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
		        fi
		        if [ "$xml_mode" == "11b"  ];then
								if [ "$xml_wlan_rateb" == "auto" ]; then
								   $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
								else
								   $IWCONFIG "$xml_if" rate  "$xml_wlan_rateb" > /dev/null 2>&1
								fi
						else
								if [ "$xml_wlan_rate" == "auto" ]; then
								   $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
								else
								   $IWCONFIG "$xml_if" rate  "$xml_wlan_rate" > /dev/null 2>&1
								fi
						fi
		        
		        $IFCONFIG "$xml_if" up > /dev/null 2>&1
            $BRCTL addif $BRLAN $xml_if > /dev/null 2>&1
					  $IWPRIV "$xml_if" bintval "$xml_wlan_beacon" > /dev/null 2>&1
    
            if [ "$xml_broadcast" == "yes" ]; then
                $IWPRIV "$xml_if" hide_ssid 0 > /dev/null 2>&1
            elif [ "$xml_broadcast" == "no" ]; then
                $IWPRIV "$xml_if" hide_ssid 1 > /dev/null 2>&1
            fi
						sys_security_setup
         elif [ "$xml_enable" == "no" ]; then
            $IFCONFIG "$xml_if" down > /dev/null 2>&1
         fi

				

}

#
#sys_security_setup
#
sys_security_setup()
{
    local xml_wlan=`config_read /config/network/vap1`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_enable=`config_getoption "$xml_wlan" enable`
    
    if [ "$xml_enable" == "yes" ]; then
		    local xml_security=`config_getoption "$xml_wlan" security`
		    local xml_authmode=`config_getoption "$xml_wlan" authmode`
		    local xml_wpamode=`config_getoption "$xml_wlan" wpamode` 
		    local xml_wepkey=`config_getoption "$xml_wlan" wepkey`
		    local xml_wepkeytype=`config_getoption "$xml_wlan" wepkeytype`
		    
		    local  xml_passkey1=`config_getoption "$xml_wlan" passkey1`
		    local  xml_passkey2=`config_getoption "$xml_wlan" passkey2`
		    local  xml_passkey3=`config_getoption "$xml_wlan" passkey3`
		    local  xml_passkey4=`config_getoption "$xml_wlan" passkey4`
    
        $IFCONFIG "$xml_if" down > /dev/null 2>&1
        $IWCONFIG "$xml_if" key open off > /dev/null 2>&1
        local a="a1"
		    local b="b1"
		    local cmd_hostapda="ps|awk '/jffs\/hostapd\/$a/{print \$1}'"
		    local cmd_hostapdb="ps|awk '/jffs\/hostapd\/$b/{print \$1}'"
		    local en1=`eval $cmd_hostapda`
		    local en2=`eval $cmd_hostapdb`
		    if [ "$en1" != "" ]; then
		       kill $en1 > /dev/null 2>&1
		    fi
		    if [ "$en2" != "" ]; then
		       kill $en2 > /dev/null 2>&1
		    fi
		    
        if [ "$xml_security" == "wep" ]; then
            $IWPRIV "$xml_if" authmode 1
            if [ "$xml_authmode" == "3" ]; then
                    sys_wparadius_config
                    $HOSTAPD -B /jffs/hostapd/b1 2>&1 >/dev/null &
                else
		            if [ "$xml_wepkeytype" == "hex" ]; then
				            if [ "$xml_wepkey" == "1" ]; then
				                $IWCONFIG "$xml_if" key [1] "$xml_passkey1" open > /dev/null 2>&1
					          elif [ "$xml_wepkey" == "2" ]; then
					             $IWCONFIG "$xml_if" key [2] "$xml_passkey2" open > /dev/null 2>&1
					          elif [ "$xml_wepkey" == "3" ]; then
					             $IWCONFIG "$xml_if" key [3] "$xml_passkey3" open > /dev/null 2>&1
					          else	     
					             $IWCONFIG "$xml_if" key [4] "$xml_passkey4" open > /dev/null 2>&1
					          fi
					      else
					          if [ "$xml_wepkey" == "1" ]; then
				               $IWCONFIG "$xml_if" key [1] s:$xml_passkey1 open > /dev/null 2>&1
					          elif [ "$xml_wepkey" == "2" ]; then
					             $IWCONFIG "$xml_if" key [2] s:$xml_passkey2 open > /dev/null 2>&1
					          elif [ "$xml_wepkey" == "3" ]; then
					             $IWCONFIG "$xml_if" key [3] s:$xml_passkey3 open > /dev/null 2>&1
					          else	     
					             $IWCONFIG "$xml_if" key [4] s:$xml_passkey4 open > /dev/null 2>&1
					          fi
					      fi
			          if [ "$xml_authmode" == "2" ];then
		               $IWPRIV "$xml_if" authmode "$xml_authmode" > /dev/null 2>&1
					      fi
					fi
        elif [ "$xml_security" == "wpa" ]; then
            if [ "$xml_wpamode" == "PSK" ]; then
	              sys_wpapsk_config
                $HOSTAPD -B /jffs/hostapd/a1 2>&1 > /dev/null &
	          else
	              sys_wparadius_config
	              $HOSTAPD -B /jffs/hostapd/b1 2>&1 > /dev/null &
	           fi
        elif [ "$xml_security" == "wpa2" ]; then
            if [ "$xml_wpamode" == "PSK" ]; then
                sys_wpapsk_config
                $HOSTAPD -B /jffs/hostapd/a1 2>&1 > /dev/null &
            else
                sys_wparadius_config
                $HOSTAPD -B /jffs/hostapd/b1 2>&1 > /dev/null &
            fi
        fi
        $IFCONFIG "$xml_if" up > /dev/null 2>&1
    fi
}

#
#dhcpserver_conf
#
dhcpserver_conf()
{
	 local xml_dhcp=`config_read /config/network/dhcp/dhcpserver`
	 local xml_dhcp_enable=`config_getoption "$xml_dhcp" enable`
	 local xml_dhcp_interface=`config_getoption "$xml_dhcp" interface`
	 local xml_dhcp_leasetime=`config_getoption "$xml_dhcp" leasetime`
	 local xml_dhcp_ipstart=`config_getoption "$xml_dhcp" ipstart`
	 local xml_dhcp_ipend=`config_getoption "$xml_dhcp" ipend`
	 local xml_dhcp_subnet=`config_getoption "$xml_dhcp" subnet`
	 local xml_dhcp_route=`config_getoption "$xml_dhcp" route`
	 
	 local xml_lan=`config_read /config/network/lan`
	 local xml_lan_if=`config_getoption "$xml_lan" if`
	 local xml_lan_ipaddr=`config_getoption "$xml_lan" ipaddr`
	 local xml_lan_netmask=`config_getoption "$xml_lan" netmask`
	 
	 [ -z $xml_lan_if ]||[ -z $xml_lan_ipaddr ]||[ -z $xml_lan_netmask ]||$IFCONFIG $xml_lan_if $xml_lan_ipaddr netmask $xml_lan_netmask > /dev/null 2>&1
	 
	 if [ "$xml_dhcp_enable" == "yes" ];then
	 	 echo "resolv-file=/etc/resolv.conf" > $DNSMASQ_CONF
		 echo "interface=default" >> $DNSMASQ_CONF 
		 echo "no-hosts" >> $DNSMASQ_CONF
		 echo "dhcp-range=$xml_dhcp_ipstart,$xml_dhcp_ipend,$xml_dhcp_subnet,${xml_dhcp_leasetime}h" >> $DNSMASQ_CONF
		 echo "dhcp-option=option:router,$xml_dhcp_route" >> $DNSMASQ_CONF
		 echo "dhcp-lease-max=150" >> $DNSMASQ_CONF
		 $KILLALL dnsmasq > /dev/null 2>&1
		 $DNSMASQ start > /dev/null 2>&1
	 else
	 	 $KILLALL dnsmasq > /dev/null 2>&1
	 fi
	 
	 
	 
}

############################################firewall#######################################
# firewall module
# sys_default_NAT
# added by tangsiqi
sys_default_NAT()
{
	iptables -t nat -F POSTROUTING > /dev/null 2>&1
	iptables -t nat -A POSTROUTING -o $1 -j MASQUERADE > /dev/null 2>&1
}


# firewall module
# sys_dos_filter
# added by tangsiqi
sys_dos_filter()
{
	local xml_dos=`config_read /config/firewall/dosfilter`
	local xml_status=`config_getoption "$xml_dos" status`
	if [ "$xml_status" == "enable" ];then
		echo 1 >/proc/sys/net/ipv4/tcp_syncookies
	else
		echo 0 >/proc/sys/net/ipv4/tcp_syncookies
	fi
	
}

# firewall module
# sys_ip_filter
# added by tangsiqi
sys_ip_filter()
{
	iptables -N AP_IP_FILTER
	iptables -F AP_IP_FILTER
	iptables -D FORWARD -j AP_IP_FILTER
	iptables -A FORWARD -j AP_IP_FILTER
	
	local xml_ip_father=`config_read /config/firewall/ipfilter`
	local xml_status=`config_getoption "$xml_ip_father" status`
	
	local xml_ip_start1=`config_getoption "$xml_ip_father" ipaddr1_start`
	local xml_ip_end1=`config_getoption "$xml_ip_father" ipaddr1_end`
	local xml_ip_start2=`config_getoption "$xml_ip_father" ipaddr2_start`
	local xml_ip_end2=`config_getoption "$xml_ip_father" ipaddr2_end`
	local xml_ip_start3=`config_getoption "$xml_ip_father" ipaddr3_start`
	local xml_ip_end3=`config_getoption "$xml_ip_father" ipaddr3_end`
	local xml_ip_start4=`config_getoption "$xml_ip_father" ipaddr4_start`
	local xml_ip_end4=`config_getoption "$xml_ip_father" ipaddr4_end`
	local xml_ip_start5=`config_getoption "$xml_ip_father" ipaddr5_start`
	local xml_ip_end5=`config_getoption "$xml_ip_father" ipaddr5_end`
	local xml_ip_start6=`config_getoption "$xml_ip_father" ipaddr6_start`
	local xml_ip_end6=`config_getoption "$xml_ip_father" ipaddr6_end`
	local xml_ip_start7=`config_getoption "$xml_ip_father" ipaddr7_start`
	local xml_ip_end7=`config_getoption "$xml_ip_father" ipaddr7_end`
	local xml_ip_start8=`config_getoption "$xml_ip_father" ipaddr8_start`
	local xml_ip_end8=`config_getoption "$xml_ip_father" ipaddr8_end`
	local xml_ip_start9=`config_getoption "$xml_ip_father" ipaddr9_start`
	local xml_ip_end9=`config_getoption "$xml_ip_father" ipaddr9_end`
	local xml_ip_start10=`config_getoption "$xml_ip_father" ipaddr10_start`
	local xml_ip_end10=`config_getoption "$xml_ip_father" ipaddr10_end`
	
	if [ "$xml_status" == "enable" ];then
		if [ "$xml_ip_start1" != "" -a "$xml_ip_end1" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start1-$xml_ip_end1" -j DROP 
		fi
		
		if [ "$xml_ip_start2" != "" -a "$xml_ip_end2" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start2-$xml_ip_end2" -j DROP 
		fi
		
		if [ "$xml_ip_start3" != "" -a "$xml_ip_end3" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start3-$xml_ip_end3" -j DROP 
		fi
		
		if [ "$xml_ip_start4" != "" -a "$xml_ip_end4" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start4-$xml_ip_end4" -j DROP 
		fi
		
		if [ "$xml_ip_start5" != "" -a "$xml_ip_end5" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start5-$xml_ip_end5" -j DROP 
		fi
		
		if [ "$xml_ip_start6" != "" -a "$xml_ip_end6" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start6-$xml_ip_end6" -j DROP 
		fi
		
		if [ "$xml_ip_start7" != "" -a "$xml_ip_end7" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start7-$xml_ip_end7" -j DROP 
		fi
		
		if [ "$xml_ip_start8" != "" -a "$xml_ip_end8" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start8-$xml_ip_end8" -j DROP 
		fi
		
		if [ "$xml_ip_start9" != "" -a "$xml_ip_end9" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start9-$xml_ip_end9" -j DROP 
		fi
		
		if [ "$xml_ip_start10" != "" -a "$xml_ip_end10" != "" ];then
			iptables -A AP_IP_FILTER -m iprange --src-range "$xml_ip_start10-$xml_ip_end10" -j DROP 
		fi
	elif [ "$xml_status" == "disable" ];then
		iptables -F AP_IP_FILTER

	fi
	
	iptables -D AP_IP_FILTER -j RETURN 
	iptables -A AP_IP_FILTER -j RETURN
	
}

# firewall module
# sys_mac_filter
# added by tangsiqi
sys_mac_filter()
{
	iptables -N AP_MAC_FILTER
	iptables -F AP_MAC_FILTER
	iptables -D FORWARD -j AP_MAC_FILTER
	iptables -A FORWARD -j AP_MAC_FILTER
	
	local xml_mac_filter=`config_read /config/firewall/macfilter`
	local xml_status=`config_getoption "$xml_mac_filter" status`
	
	local xml_mac1=`config_getoption "$xml_mac_filter" mac1`
	local xml_mac2=`config_getoption "$xml_mac_filter" mac2`
	local xml_mac3=`config_getoption "$xml_mac_filter" mac3`
	local xml_mac4=`config_getoption "$xml_mac_filter" mac4`
	local xml_mac5=`config_getoption "$xml_mac_filter" mac5`
	local xml_mac6=`config_getoption "$xml_mac_filter" mac6`
	local xml_mac7=`config_getoption "$xml_mac_filter" mac7`
	local xml_mac8=`config_getoption "$xml_mac_filter" mac8`
	local xml_mac9=`config_getoption "$xml_mac_filter" mac9`
	local xml_mac10=`config_getoption "$xml_mac_filter" mac10`
	
	if [ "$xml_status" == "enable" ];then
		if [ "$xml_mac1" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac1" -j DROP
		fi
		
		if [ "$xml_mac2" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac2" -j DROP
		fi
		
		if [ "$xml_mac3" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac3" -j DROP
		fi
		
		if [ "$xml_mac4" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac4" -j DROP
		fi
		
		if [ "$xml_mac5" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac5" -j DROP
		fi
		
		if [ "$xml_mac6" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac6" -j DROP
		fi
		
		if [ "$xml_mac7" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac7" -j DROP
		fi
		
		if [ "$xml_mac8" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac8" -j DROP
		fi
		
		if [ "$xml_mac9" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac9" -j DROP
		fi
		
		if [ "$xml_mac10" != "" ];then
			iptables -A AP_MAC_FILTER -m mac --mac-source "$xml_mac10" -j DROP
		fi
	elif [ "$xml_status" == "disable" ];then
		iptables -F AP_MAC_FILTER
	fi
	
	iptables -D AP_MAC_FILTER -j RETURN 
	iptables -A AP_MAC_FILTER -j RETURN 
}

# firewall module
# sys_url_filter
# added by tangsiqi
sys_url_filter()
{
	iptables -N AP_URL_FILTER	
	
	iptables -F AP_URL_FILTER
	iptables -D FORWARD -j AP_URL_FILTER
	iptables -A FORWARD -j AP_URL_FILTER
	
	local xml_url_filter=`config_read /config/firewall/urlfilter`
	local xml_status=`config_getoption "$xml_url_filter" status`
	
	local xml_url1=`config_getoption "$xml_url_filter" url1`
	local xml_url2=`config_getoption "$xml_url_filter" url2`
	local xml_url3=`config_getoption "$xml_url_filter" url3`
	local xml_url4=`config_getoption "$xml_url_filter" url4`
	local xml_url5=`config_getoption "$xml_url_filter" url5`
	local xml_url6=`config_getoption "$xml_url_filter" url6`
	local xml_url7=`config_getoption "$xml_url_filter" url7`
	local xml_url8=`config_getoption "$xml_url_filter" url8`
	local xml_url9=`config_getoption "$xml_url_filter" url9`
	local xml_url10=`config_getoption "$xml_url_filter" url10`
	
	if [ "$xml_status" == "enable" ];then
		if [ "$xml_url1" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url1" -j DROP
		fi
		
		if [ "$xml_url2" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url2" -j DROP
		fi
		
		if [ "$xml_url3" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url3" -j DROP
		fi
		
		if [ "$xml_url4" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url4" -j DROP
		fi
		
		if [ "$xml_url5" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url5" -j DROP
		fi
		
		if [ "$xml_url6" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url6" -j DROP
		fi
		
		if [ "$xml_url7" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url7" -j DROP
		fi
		
		if [ "$xml_url8" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url8" -j DROP
		fi
		
		if [ "$xml_url9" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url9" -j DROP
		fi
		
		if [ "$xml_url10" != "" ];then
			iptables -A AP_URL_FILTER -d "$xml_url10" -j DROP
		fi
	elif [ "$xml_status" == "disable" ];then
		iptables -F AP_URL_FILTER
	fi
	
	iptables -D AP_URL_FILTER -j RETURN 
	iptables -A AP_URL_FILTER -j RETURN 
}

# firewall module
# sys_dmz_host
# added by tangsiqi
sys_dmz_host()
{
	iptables -t nat -F PREROUTING
	
	local xml_dmz_filter=`config_read /config/firewall/dmz`
	local xml_status=`config_getoption "$xml_dmz_filter" status`
	local xml_dmz_ip=`config_getoption "$xml_dmz_filter" dmz_ip`
	
	if [ "$xml_status" == "enable" -a "$xml_dmz_ip" != "" ];then
		iptables -t nat -A PREROUTING -j DNAT --to "$xml_dmz_ip"
	elif [ "$xml_status" == "disable" ];then
		iptables -t nat -F PREROUTING
	fi
}

# firewall module
# sys_virtual_server
# added by tangsiqi
sys_virtual_server()
{
	iptables -t nat -F PREROUTING
	local xml_virtual_server=`config_read /config/firewall/virtual_server`
	
	for count in `seq 10`
	do
	eval xml_server_state$count=`config_getoption "$xml_virtual_server" enable$count`
	eval xml_server_private_ip$count=`config_getoption "$xml_virtual_server" private_ip$count`
	eval xml_server_private_port$count=`config_getoption "$xml_virtual_server" private_port$count`
	eval xml_server_protocal_type$count=`config_getoption "$xml_virtual_server" protocal_type$count`
	eval xml_server_public_port$count=`config_getoption "$xml_virtual_server" public_port$count`
	eval tmp="\$"xml_server_state${count}
	eval private_ip="\$"xml_server_private_ip${count}
	eval private_port="\$"xml_server_private_port${count}
	eval protocal_type="\$"xml_server_protocal_type${count}
	eval public_port="\$"xml_server_public_port${count}
	
	if [ "$public_port" != "tcp_udp" ];then
		if [ "$tmp" == "enable" ];then
			iptables -t nat -A PREROUTING -p "$protocal_type" --dport "$public_port" -j DNAT --to "$private_ip":"$private_port"
		else
			iptables -t nat -F PREROUTING
		fi
	else
		if [ "$tmp" == "enable" ];then
			iptables -t nat -A PREROUTING -p tcp --dport "$public_port" -j DNAT --to "$private_ip":"$private_port"
			iptables -t nat -A PREROUTING -p udp --dport "$public_port" -j DNAT --to "$private_ip":"$private_port"
		else
			iptables -t nat -F PREROUTING
		fi
	fi
	done
	
	
}

#
#wlwan_conf
#
wlwan_conf()
{
	local xml_wlwan=`config_read /config/network/wlwan`
	local xml_wlwan_enable=`config_getoption "$xml_wlwan" enable`
	local xml_wlwan_ssid=`config_getoption "$xml_wlwan" ssid`
	local xml_wlwan_security=`config_getoption "$xml_wlwan" sectype`
	
	local xml_wep=`config_read /config/network/wlwan/wep`
	local xml_wlwan_authmode=`config_getoption "$xml_wep" wepmode`
	local xml_wlwan_keytype=`config_getoption "$xml_wep" wepkeytype`
	local xml_wlwan_cwepkey=`config_getoption "$xml_wep" cwepkey`
	local xml_wlwan_passkey1=`config_getoption "$xml_wep" wepkey1`
	local xml_wlwan_passkey2=`config_getoption "$xml_wep" wepkey2`
	local xml_wlwan_passkey3=`config_getoption "$xml_wep" wepkey3`
	local xml_wlwan_passkey4=`config_getoption "$xml_wep" wepkey4`
	
	local xml_wpa=`config_read /config/network/wlwan/wpa`
	local xml_wlwan_passphrase=`config_getoption "$xml_wpa" wpapass`
	
	local xml_ip=`config_read /config/network/wlwan/ip`
	local xml_wlwan_ip=`config_getoption "$xml_ip" iptype`
	local xml_wlwan_ipaddr=`config_getoption "$xml_ip" ipaddr`
	local xml_wlwan_netmask=`config_getoption "$xml_ip" netmask`
	local xml_wlwan_gateway=`config_getoption "$xml_ip" gateway`
	
	local xml_dns=`config_read /config/network/wlwan/dns`
	local xml_wlwan_dns=`config_getoption "$xml_dns" dnstype`
	local xml_wlwan_dnsprimary=`config_getoption "$xml_dns" primarydns`
	local xml_wlwan_dnsecondary=`config_getoption "$xml_dns" seconddns`
	
	local CONF_FILE=/etc/wpa_supplicant.conf
	
	if [ "$xml_wlwan_enable" == "yes" ];then
		local cwepkey=`expr $xml_wlwan_cwepkey + 1`
		eval passkey="$"xml_wlwan_passkey$cwepkey
		if [ "$xml_wlwan_keytype" == "hex" ];then
					local str_wepkey="wep_key$xml_wlwan_cwepkey=$passkey"
		else
					local str_wepkey="wep_key$xml_wlwan_cwepkey=\"$passkey\""
		fi
		
		case "$xml_wlwan_security" in
		"none")
				echo "ctrl_interface=/var/run/wpa_supplicant" > $CONF_FILE
				echo "network={" >> $CONF_FILE
				echo "	ssid=\"$xml_wlwan_ssid\"" >> $CONF_FILE
				echo "	key_mgmt=NONE" >> $CONF_FILE
				echo "}" >> $CONF_FILE
		;;
		
		"wep")
				echo "ctrl_interface=/var/run/wpa_supplicant" > $CONF_FILE
				echo "network={" >> $CONF_FILE
				echo "	ssid=\"$xml_wlwan_ssid\"" >> $CONF_FILE
				echo "	key_mgmt=NONE" >> $CONF_FILE
				echo "	$str_wepkey" >> $CONF_FILE
				echo "	wep_tx_keyidx=0" >> $CONF_FILE
				if [ "$xml_wlwan_authmode" == "2" ]; then
                                        echo "  auth_alg=SHARED" >> $CONF_FILE
                                fi
				echo "}" >> $CONF_FILE
		;;
		
		"wpa")
				echo "ctrl_interface=/var/run/wpa_supplicant" > $CONF_FILE
				echo "network={" >> $CONF_FILE
				echo "	ssid=\"$xml_wlwan_ssid\"" >> $CONF_FILE
				echo "	key_mgmt=WPA-PSK" >> $CONF_FILE
				echo "	psk=\"$xml_wlwan_passphrase\"" >> $CONF_FILE
				echo "}" >> $CONF_FILE
		;;
		
		*)
				echo "ctrl_interface=/var/run/wpa_supplicant" > $CONF_FILE
				echo "network={" >> $CONF_FILE
				echo "	ssid=\"$xml_wlwan_ssid\"" >> $CONF_FILE
				echo "	key_mgmt=WPA-PSK" >> $CONF_FILE
				echo "	psk=\"$xml_wlwan_passphrase\"" >> $CONF_FILE
				echo "	proto=wpa2" >> $CONF_FILE
				echo "}" >> $CONF_FILE
		esac
		$KILLALL wpa_supplicant > /dev/null 2>&1
		$WPA_SUPPLICANT -B -c /etc/wpa_supplicant.conf -i ath1 -D madwifi > /dev/null 2>&1
	else
		$KILLALL wpa_supplicant > /dev/null 2>&1
		$IFCONFIG ath1 down > /dev/null 2>&1
	fi
	
	if [ $? == 0 ];then
		if [ "$xml_wlwan_ip" == "auto" ];then
			 $UDHCPC -i ath1 -t 3 -b > /dev/null 2>&1
		else
			 $IFCONFIG ath1 $xml_wlwan_ipaddr netmask $xml_wlwan_netmask > /dev/null 2>&1
			 echo "nameserver $xml_wlwan_dnsprimary" >/etc/resolv.conf
			 echo "nameserver $xml_wlwan_dnsecondary" >>/etc/resolv.conf
			 $ROUTE add -net default gw "$xml_wlwan_gateway" > /dev/null 2>&1
		fi
	fi
}

#
#wanselect_conf
#
wanselect_conf()
{
	local xml_wan=`config_read /config/network/wan`
  xml_wan_select=`config_getoption "$xml_wan" wan_select`
	if [ "$xml_wan_select" == "3g" ]; then
			$IFCONFIG ath1 down > /dev/null 2>&1
			$ROUTE del default > /dev/null 2>&1
			sys_default_NAT ppp0
			sys_3g_wan_setup
	elif [ "$xml_wan_select" == "80211" ]; then
			$KILLALL pppd > /dev/null 2>&1
			$ROUTE del default > /dev/null 2>&1
			sys_default_NAT ath1
			wlwan_conf
	else
			$IFCONFIG ath1 down > /dev/null 2>&1
			$ROUTE del default > /dev/null 2>&1
			sys_default_NAT ppp0
			sys_3g_wan_setup
	fi
}

#
#sys_3g_wan_setup
#
sys_3g_wan_setup()
{
	local xml_3g_PPP_Connection=`config_read /config/network/ap_3g_wan/PPP_Connection_Mode`
	local xml_connect_mode=`config_getoption "$xml_3g_PPP_Connection" connect_mode`
	local xml_connect_status=`config_getoption "$xml_3g_PPP_Connection" connect_status`

	local xml_3g_apn=`config_read /config/network/ap_3g_wan/APN`
	local xml_apn_mode=`config_getoption "$xml_3g_apn" apn_mode`
	local xml_apn_content=`config_getoption "$xml_3g_apn" apn_content`

	local xml_3g_Dial_setting=`config_read /config/network/ap_3g_wan/Dial_setting`
	local xml_Dial_NO=`config_getoption "$xml_3g_Dial_setting" Dial_NO`

	local xml_3g_PDP=`config_read /config/network/ap_3g_wan/PDP`
	local xml_PDP_type=`config_getoption "$xml_3g_PDP" PDP_type`
	local xml_PDP_mode=`config_getoption "$xml_3g_PDP" PDP_mode`
	local xml_PDP_address=`config_getoption "$xml_3g_PDP" PDP_address`

	local xml_3g_PPP_Auth=`config_read /config/network/ap_3g_wan/PPP_Auth`
	local xml_auth_mode=`config_getoption "$xml_3g_PPP_Auth" auth_mode`
	local xml_user_name=`config_getoption "$xml_3g_PPP_Auth" user_name`
	local xml_passwd=`config_getoption "$xml_3g_PPP_Auth" passwd`

	local xml_3g_PPP_setting=`config_read /config/network/ap_3g_wan/PPP_setting`
	local xml_idle_time_enable=`config_getoption "$xml_3g_PPP_setting" idle_time_enable`
	local xml_max_idle_time=`config_getoption "$xml_3g_PPP_setting" max_idle_time`
	local xml_MTU=`config_getoption "$xml_3g_PPP_setting" MTU`

	local xml_3g_DNS=`config_read /config/network/ap_3g_wan/DNS`
	local xml_dns_mode=`config_getoption "$xml_3g_DNS" dns_mode`
	local xml_Primary_dns=`config_getoption "$xml_3g_DNS" Primary_dns`
	local xml_second_dns=`config_getoption "$xml_3g_DNS" second_dns`
	
	#
	echo -n "" > $OPTIONS
	if [ "$xml_connect_mode" == "Demand_connct" ]; then
		echo "demand" > $OPTIONS
	#	echo "ipcp-accept-remote" >> $OPTIONS
	#else
	#	echo "ipcp-accept-remote" > $OPTIONS
	fi
	
	#echo "ipcp-accept-local" >> $OPTIONS
	#
	
	echo "holdoff 10" >> $OPTIONS
	echo "connect \"/usr/sbin/chat -v -t6 -f /etc/ppp/chat_script\"" >> $OPTIONS
	if [ "$xml_idle_time_enable" == "enable" ]; then
		echo "idle $xml_max_idle_time" >> $OPTIONS
	fi
	if [ -f /jffs/3g-card-vendor ]; then
        	g3_card_provider="`cat /jffs/3g-card-vendor`"
        	if [ "$g3_card_provider" == "huawei" ]; then
			echo "ttyUSB0" >> $OPTIONS
		else
			echo "ttyUSB3" >> $OPTIONS
        	fi
	else
		echo "zhongxing" > /jffs/3g-card-vendor
		echo "ttyUSB3" >> $OPTIONS
	fi

	echo "115200" >> $OPTIONS
	echo "debug" >> $OPTIONS
	echo "logfile /var/log/ppplog" >> $OPTIONS
	echo "mtu $xml_MTU" >> $OPTIONS
	echo "-detach" >> $OPTIONS
	echo "noauth" >> $OPTIONS
	echo "noipdefault" >> $OPTIONS
	echo "defaultroute" >> $OPTIONS
	echo "crtscts" >> $OPTIONS
	echo "lock" >> $OPTIONS
	echo "lcp-echo-failure 4" >> $OPTIONS
	echo "-ccp" >> $OPTIONS
	echo "-vj" >> $OPTIONS
	
	if [ "$xml_dns_mode" == "auto_dns" ];then
			 echo "usepeerdns" >> $OPTIONS
	else
			 echo $xml_Primary_dns >/etc/ppp/resolv.conf
			 echo $xml_second_dns >>/etc/ppp/resolv.conf
	fi
	
	if [ "$xml_auth_mode" == "pap" ]; then
		echo "$xml_user_name	*	$xml_passwd" > $PAP_SECRETS
	#	echo "+pap" >> $OPTIONS
		echo "-chap" >> $OPTIONS
	else
		echo "$xml_user_name	*	$xml_passwd" > $CHAP_SECRETS
		echo "-pap" >> $OPTIONS
	#	echo "+chap" >> $OPTIONS
	fi
	
	echo "user" >> $OPTIONS
	echo "hide-password" >> $OPTIONS
	
	CHAT_SCRIPT=/etc/ppp/chat_script
	if [ "$xml_Dial_NO" != "" ]; then
		echo "\"\" AT" > ${CHAT_SCRIPT}
		echo "OK ATD${xml_Dial_NO}" >> ${CHAT_SCRIPT}
		echo "CONNECT \"\"" >> ${CHAT_SCRIPT}
	fi
	
	
	
	#apn
	if [ "$xml_apn_mode" == "auto_apn"  ]; then
		if [ "$xml_PDP_mode" == "auto_pdp" ]; then
			/usr/sbin/ttycrl -f /dev/ttyUSB2 "AT+CGDCONT=1,\"${xml_PDP_type}\",\"\",\"\",0,0" > /dev/null 2>&1
		else
			/usr/sbin/ttycrl -f /dev/ttyUSB2 "AT+CGDCONT=1,\"${xml_PDP_type}\",\"\",\"${xml_PDP_address}\",0,0" > /dev/null 2>&1
		fi
	else
		if [ "$xml_PDP_mode" == "auto_pdp" ]; then
			/usr/sbin/ttycrl -f /dev/ttyUSB2 "AT+CGDCONT=1,\"${xml_PDP_type}\",\"${xml_apn_content}\",\"\",0,0" > /dev/null 2>&1
		else
			/usr/sbin/ttycrl -f /dev/ttyUSB2 "AT+CGDCONT=1,\"${xml_PDP_type}\",\"${xml_apn_content}\",\"${xml_PDP_address}\",0,0" > /dev/null 2>&1
		fi
	fi
	

	
	
	
	if [ "$xml_connect_mode" == "Auto_connect" -o "$xml_connect_mode" == "Demand_connct" ]; then
		$KILLALL pppd > /dev/null 2>&1
		sleep 3 > /dev/null 2>&1
	 	$PPPD & > /dev/null 2>&1
	else
		$KILLALL pppd > /dev/null 2>&1
 	fi
}

#
#ppp_connect_conf()
#
ppp_connect_conf()
{
	local xml_3g_PPP_Connection=`config_read /config/network/ap_3g_wan/PPP_Connection_Mode`
	local xml_connect_status=`config_getoption "$xml_3g_PPP_Connection" connect_status`
	if [ "$xml_connect_status" == "Connected" ]; then
		$KILLALL pppd > /dev/null 2>&1
	 	$PPPD & > /dev/null 2>&1
	else
		$KILLALL pppd > /dev/null 2>&1
 	fi
}

#
#voip_conf()
#
voip_conf()
{
	local xml_voip=`config_read /config/voip`
	local xml_voip_register_server=`config_getoption "$xml_voip" register_server`
	local xml_voip_proxy_server=`config_getoption "$xml_voip" proxy_server`
	local xml_voip_port=`config_getoption "$xml_voip" port`
	local xml_voip_auth_user=`config_getoption "$xml_voip" auth_user`
	local xml_voip_auth_pass=`config_getoption "$xml_voip" auth_pass`
	local xml_voip_user=`config_getoption "$xml_voip" user`
	local xml_voip_status=`config_getoption "$xml_voip" status`


}

#
#set_3g_conf()
#
set_3g_conf()
{
	local xml_set3g=`config_read /config/network/set_3g`
	local xml_set3g_mode=`config_getoption "$xml_set3g" mode`
	
	
	if [ "$xml_set3g_mode" = "WCDMA_pre" ]; then
		/usr/sbin/ttycrl -f /dev/ttyUSB2 "AT+ZSNT=0,0,2" > /dev/null 2>&1
	elif [ "$xml_set3g_mode" = "only_WCDMA" ]; then 
		/usr/sbin/ttycrl -f /dev/ttyUSB2 "AT+ZSNT=2,0,0" > /dev/null 2>&1
	elif [ "$xml_set3g_mode" = "GSM_pre" ]; then
		/usr/sbin/ttycrl -f /dev/ttyUSB2 "AT+ZSNT=0,0,1" > /dev/null 2>&1
	elif [ "$xml_set3g_mode" = "only_GSM" ]; then 
		/usr/sbin/ttycrl -f /dev/ttyUSB2 "AT+ZSNT=1,0,0" > /dev/null 2>&1
	fi
		
	
}


