#!/bin/sh

#
#include_js(js_name)
#
include_js()
{
    local xml_language=`config_read /config/session 2>/dev/null`
    local xml_language_type=`config_getoption "$xml_language" language`
    if [ -n "$xml_language_type" ];then
        echo "<script language=\"JavaScript\" src=\"js/$xml_language_type/$1?temp=4\" type=\"text/javascript\"></script>"
    else
        echo "<script language=\"JavaScript\" src=\"js/en/$1?temp=4\" type=\"text/javascript\"></script>"
    fi
}

#
#ap_info(apname,aplocation,apdescribe)
#
ap_info()
{
    CONF_PATH=/jffs/apinfo.conf
    echo "APNAME=\"$1\"" > $CONF_PATH
    echo "APLOCATION=\"$2\"" >> $CONF_PATH
    echo "APDESCR=\"$3\"" >> $CONF_PATH
}


#
#wtpd_enable
#
wtpd_enable()
{
    local xml_wtp=`config_read /config/system`
    local xml_wtp_enable=`config_getoption "$xml_wtp" wtpenable`
    if [ "$xml_wtp_enable" == "yes" ]; then
        $KILLALL wtpd >/dev/null  2>&1
        /sbin/wtpd  -B  & >/dev/null  2>&1
    else
        $KILLALL wtpd >/dev/null  2>&1
    fi
}


#
#sys_user_setup
#
sys_user_setup()
{
    local xml_config=`config_read /config/system`
    local xml_username=`config_getoption "$xml_config" username`
    local xml_password=`config_getoption "$xml_config" password`

    local awk_program="BEGIN{FS=\":\"; OFS=\":\";}{if(\$0~/:admin:/){\$1=\"$xml_username\"; \$2=\"$xml_password\";} print \$0;}"
    cat /tmp/passwd | awk "$awk_program" > /tmp/passwd_tmp
    cp /tmp/passwd_tmp /etc/passwd
    mv /tmp/passwd_tmp /tmp/passwd
}

#
#sys_lan_start(interface) 
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
    local xml_lan_vendorclassid=`config_getoption "$xml_lan" vendorclassid`


    local xml_sroute=`config_read /config/network/sroute`
    local xml_sroute_route1=`config_getoption "$xml_sroute" route1`

    $KILLALL udhcpc >/dev/null

    if [ "$xml_proto" == "static" ]; then
        $IFCONFIG "$xml_if" "$xml_ipaddr" netmask "$xml_netmask" > /dev/null 2>&1
    elif [ "$xml_proto" == "dhcp" ]; then
       if [ ! -z "${xml_lan_vendorclassid}" ];then
          $UDHCPC -i "$xml_if" -V "${xml_lan_vendorclassid}" -t $xml_lan_dhcpcount -b > /dev/null 2>&1
       else
          $UDHCPC -i "$xml_if" -t $xml_lan_dhcpcount -b > /dev/null 2>&1 
       fi
    fi

    [ -n "$xml_sroute_route1" ] && $ROUTE add -net default gw "$xml_sroute_route1" > /dev/null 2>&1    

}

#
#wired_if_conf
#
wired_if_conf()
{
    for wdif_num in `seq 2`
    do
        local eth_num=`expr $wdif_num - 1`
        eval "local xml_eth${eth_num}=\`config_read /config/network/eth${eth_num}\`"
        eval "xml_eth${eth_num}_ip=\`config_getoption \"\$xml_eth${eth_num}\" ip\`"
        eval "xml_eth${eth_num}_netmask=\`config_getoption \"\$xml_eth${eth_num}\" netmask\`"
        eval "xml_eth${eth_num}_fw2=\`config_getoption \"\$xml_eth${eth_num}\" forward2\`"
    done
    [ -z "$xml_eth0_netmask" ]||xml_eth0_netmask="netmask $xml_eth0_netmask"
    if [  "$xml_eth0_ip" == "" ]; then
        $IFCONFIG eth0 0.0.0.0 > /dev/null 2>&1
    else
        $IFCONFIG eth0 $xml_eth0_ip $xml_eth0_netmask > /dev/null 2>&1
    fi
    [ $? == 0 ]&&{
    if [ "$xml_eth0_fw2" == "yes" ]; then
        $BRCTL addif $BRLAN eth0 > /dev/null 2>&1
    else
        $BRCTL delif $BRLAN eth0 > /dev/null 2>&1
    fi
}

[ "$DEV_eth1" == "enable" ]&&{
[ -z "$xml_eth1_netmask" ]||xml_eth1_netmask="netmask $xml_eth1_netmask"
if [  "$xml_eth1_ip" == "" ]; then
    $IFCONFIG eth1 0.0.0.0 > /dev/null 2>&1
else
    $IFCONFIG eth1 $xml_eth1_ip $xml_eth1_netmask > /dev/null 2>&1
fi
[ $? == 0 ]&&{
if [ "$xml_eth1_fw2" == "yes" ]; then
    $BRCTL addif $BRLAN eth1 > /dev/null 2>&1
else
    $BRCTL delif $BRLAN eth1 > /dev/null 2>&1
fi
            }
        }

    }

    #
    #wireless_if_conf
    #
    wireless_if_conf()
    {
        for vap_num in `seq $NUM_ap`
        do
            local xml_vap=`config_read /config/network/vap${vap_num}`
            local xml_vap_enable=`config_getoption "$xml_vap" create`
            [ "$xml_vap_enable" == "yes" ]&&{
            local xml_vap_ip=`config_getoption "$xml_vap" ip`
            local xml_vap_netmask=`config_getoption "$xml_vap" netmask`
            local xml_vap_fw2=`config_getoption "$xml_vap" forward2`
            local xml_vap_vlan=`config_read /config/network/vap${vap_num}/vlan`
            local xml_vap_vlanenable=`config_getoption "$xml_vap_vlan" enable`
            local xml_vap_vlanid=`config_getoption "$xml_vap_vlan" vid`

            local WLIF=`expr $vap_num - 1`
            [ -z "$xml_vap_netmask" ]||xml_vap_netmask="netmask $xml_vap_netmask"
            if [ "$xml_vap_ip" == "" ]; then
                $IFCONFIG ath$WLIF 0.0.0.0 > /dev/null 2>&1
            else
                $IFCONFIG ath$WLIF $xml_vap_ip $xml_vap_netmask > /dev/null 2>&1
            fi
            if [ "$xml_vap_vlanenable" == "0" ];then
                if [ "$xml_vap_fw2" == "yes" ]; then
                    $BRCTL addif $BRLAN ath$WLIF > /dev/null 2>&1
                else
                    $BRCTL delif $BRLAN ath$WLIF > /dev/null 2>&1
                fi
            else
                if [ "$xml_vap_fw2" == "yes" ]; then
                    [ ! -z "$xml_vap_vlanid" ] && $BRCTL delif br$xml_vap_vlanid ath$WLIF > /dev/null 2>&1
                    $BRCTL addif br$xml_vap_vlanid ath$WLIF > /dev/null 2>&1
                else
                    $BRCTL delif br$xml_vap_vlanid ath$WLIF > /dev/null 2>&1
                fi
            fi
        }
    done
}

#
#route_forward_conf
#
route_forward_conf()
{

    local xml_sroute=`config_read /config/network/sroute`
    for route_num in `seq 3`
    do
        local TMP_NUM=`expr $route_num + 1`
        eval "local xml_sroute_route$TMP_NUM=\`config_getoption \"\$xml_sroute\" route$TMP_NUM\`"
        eval "local xml_sroute_rtip$TMP_NUM=\`config_getoption \"\$xml_sroute\" rtip$TMP_NUM\`"
        eval "local xml_sroute_rtmask$TMP_NUM=\`config_getoption \"\$xml_sroute\" rtmask$TMP_NUM\`"
    done


    local xml_bnet=`config_read /config/network`
    local xml_bnet_forward3=`config_getoption "$xml_bnet" forward3`
    [ -n "$xml_bnet_forward3" ] && $SYSCTL -w net.ipv4.ip_forward=$xml_bnet_forward3 > /dev/null 2>&1
    [ -n "$xml_sroute_route2" ] && [ -n "$xml_sroute_rtip2" ] && [ -n "$xml_sroute_rtmask2" ] && $ROUTE add -net $xml_sroute_rtip2 netmask $xml_sroute_rtmask2 gw $xml_sroute_route2 > /dev/null 2>&1    
    [ -n "$xml_sroute_route3" ] && [ -n "$xml_sroute_rtip3" ] && [ -n "$xml_sroute_rtmask3" ] && $ROUTE add -net $xml_sroute_rtip3 netmask $xml_sroute_rtmask3 gw $xml_sroute_route3 > /dev/null 2>&1 
    [ -n "$xml_sroute_route4" ] && [ -n "$xml_sroute_rtip4" ] && [ -n "$xml_sroute_rtmask4" ] && $ROUTE add -net $xml_sroute_rtip4 netmask $xml_sroute_rtmask4 gw $xml_sroute_route4 > /dev/null 2>&1


    if [ "$xml_bnet_forward3" == "0" ]; then
        $IPTABLES -t nat -F POSTROUTING > /dev/null 2>&1
    fi

}


#
#sys_dns_setup
#
sys_dns_setup()
{
    local xml_lan=`config_read /config/network/lan`
    local xml_proto=`config_getoption "$xml_lan" proto`
    local xml_nameserver1=`config_getoption "$xml_lan" nameserver1`
    local xml_nameserver2=`config_getoption "$xml_lan" nameserver2`
    if [ "$xml_proto" == "static" ];then
        [ -n "$xml_nameserver1" ] && echo "nameserver $xml_nameserver1" > /etc/resolv.conf 
        [ -n "$xml_nameserver2" ] && echo "nameserver $xml_nameserver2" >> /etc/resolv.conf
    fi 
}

#
#sys_ntp_setup
#
sys_ntp_setup()
{
    local xml_config=`config_read /config/system`
    local xml_timeupdate=`config_getoption "$xml_config" timeupdate`
    local xml_timezone=`config_getoption "$xml_config" timezone`
    local xml_usedefault=`config_getoption "$xml_config" timeserver_usedefault`
    local xml_timeserverdefault=`config_getoption "$xml_config" timeserverdefault`
    local xml_timeserver1=`config_getoption "$xml_config" timeserver1`
    local xml_timeserver2=`config_getoption "$xml_config" timeserver2`


    export TZ="$xml_timezone"

    if [ "$xml_usedefault" == "yes" ]; then
        if [ "$xml_timeupdate" == "-1" ]; then
            $SET_CRONTAB $NTPCLIENT m0 > /dev/null 2>&1 
            killall crond > /dev/null 2>&1
            $CROND start > /dev/null 2>&1   
        elif [ "$xml_timeupdate" == "0" ]; then
            $SET_CRONTAB $NTPCLIENT m0 > /dev/null 2>&1 
            $NTPCLIENT  -h  "$xml_timeserverdefault" -s & > /dev/null 2>&1
            killall crond > /dev/null 2>&1
            $CROND start > /dev/null 2>&1
        else
            $NTPCLIENT  -h  "$xml_timeserverdefault" -s & > /dev/null 2>&1
            $SET_CRONTAB $NTPCLIENT h$xml_timeupdate "-h $xml_timeserverdefault -s & > /dev/null 2>&1" > /dev/null 2>&1
            killall crond > /dev/null 2>&1
            $CROND start > /dev/null 2>&1 
        fi
    else
        if [ "$xml_timeupdate" == "-1" ]; then
            $SET_CRONTAB $NTPCLIENT m0 > /dev/null 2>&1 
            killall crond > /dev/null 2>&1
            $CROND start > /dev/null 2>&1
        elif [ "$xml_timeupdate" == "0" ]; then
            $SET_CRONTAB $NTPCLIENT m0 > /dev/null 2>&1 
            $NTPCLIENT  -h  "$xml_timeserver1" -s &
            [ $? != 0 ] && $NTPCLIENT -h  "$xml_timeserver2" -s & > /dev/null 2>&1
            # fail safe
            [ $? != 0 ] && $NTPCLIENT -h  "$xml_timeserverdefault" -s & > /dev/null 2>&1
            killall crond > /dev/null 2>&1
            $CROND start > /dev/null 2>&1
        else
            $SET_CRONTAB $NTPCLIENT h$xml_timeupdate "-h $xml_timeserver1 -s & > /dev/null 2>&1" > /dev/null 2>&1
            $NTPCLIENT  -h  "$xml_timeserver1" -s & > /dev/null 2>&1
            [ $? != 0 ] && $SET_CRONTAB $NTPCLIENT h$xml_timeupdate "-h $xml_timeserver2 -s & > /dev/null 2>&1" > /dev/null 2>&1 && $NTPCLIENT -h "$xml_timeserver2" -s &
            # fail safe
            [ $? != 0 ] && $SET_CRONTAB $NTPCLIENT h$xml_timeupdate "-h $xml_timeserverdefault -s & > /dev/null 2>&1" > /dev/null 2>&1 && $NTPCLIENT -h "$xml_timeserverdefault" -s &
            killall crond > /dev/null 2>&1
            $CROND start > /dev/null 2>&1
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
#sys_wlan_setup
#
sys_wlan_setup()
{
    eval first="0"
    eval pre_wifi="first"
    eval wifi0_done="0"
    eval wifi1_done="0"
    for vapnum in `seq $NUM_ap`  
    do
        #wlan_startup
        local xml_wlan=`config_read /config/network/vap$vapnum`
        local xml_if=`config_getoption "$xml_wlan" if`
        local xml_enable=`config_getoption "$xml_wlan" create`
        local xml_ssid=`config_getoption "$xml_wlan" ssid`
        local xml_broadcast=`config_getoption "$xml_wlan" broadcast`
        local xml_wlanmode=`config_getoption "$xml_wlan" wlanmode`
        local xml_wlan_rate=`config_getoption "$xml_wlan" rate`
        local xml_wlan_rateb=`config_getoption "$xml_wlan" rateb`
        local xml_wlan_ratea=`config_getoption "$xml_wlan" ratea`
        local xml_wlan_rate_n_ofdm=`config_getoption "$xml_wlan" rate_n_ofdm`
        local xml_wlan_rate_n_ht20ds=`config_getoption "$xml_wlan" rate_n_ht20ds`
        local xml_wlan_rate_n_ht40ds=`config_getoption "$xml_wlan" rate_n_ht40ds`
        local xml_wlan_beacon=`config_getoption "$xml_wlan" beacon`

        local xml_wlan_wifi=`config_getoption "$xml_wlan" wifi`
        eval wifi_devmode="$"DEVMODE_$xml_wlan_wifi
        local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
        local xml_wifi_channel=`config_getoption "$xml_wifi" channel`
        local xml_wifi_channel1=`config_getoption "$xml_wifi" channel1`
        local xml_wifi_channela=`config_getoption "$xml_wifi" channela`
        local xml_wlan_autochannel=`config_getoption "$xml_wifi" autochannel`
        local xml_mode=`config_getoption "$xml_wifi" mode`
        local xml_modea=`config_getoption "$xml_wifi" modea`
        local xml_moden=`config_getoption "$xml_wifi" moden`
        local xml_cwwmode=`config_getoption "$xml_wifi" cwwmode`				
        local xml_extoffset=`config_getoption "$xml_wifi" extoffset`
        local xml_shortgi=`config_getoption "$xml_wifi" shortgi`
        local config_country_code=`config_read /config/network/country`
        local xml_region=`config_getoption "$config_country_code" region`

        local xml_config_vlan=`config_read /config/network/vap$vapnum/vlan`
        local xml_wlan_vlan_enable=`config_getoption "$xml_config_vlan" enable`
        local xml_wlan_vlan_vid=`config_getoption "$xml_config_vlan" vid`

        if [ "$xml_wlanmode" == "ap" ]||[ "$xml_wlanmode" == "root" ]&&[ "$xml_enable" == "yes"  ]; then
            echo "$vapnum $xml_wlanmode $xml_enable" >>/tmp/bbb
            #vap_wifi_alldown
            $WLANCONFIG "$xml_if" create wlandev "$xml_wlan_wifi" wlanmode ap > /dev/null 2>&1
            if [ "$xml_wlanmode" == "root" ];then
                $IWPRIV $xml_if wds 1 > /dev/null 2>&1
            fi
            [ -n "$xml_ssid" ] && $IWCONFIG "$xml_if" essid "$xml_ssid" > /dev/null 2>&1
            eval first="0"
            eval wifi_done="$"${xml_wlan_wifi}_done
            if [ ${pre_wifi} != ${xml_wlan_wifi} ]&&[ ${wifi_done} == "0" ];then
                eval first="1"
                eval wifi_done="1"
            fi
            eval pre_wifi=${xml_wlan_wifi}

            if [ ${first} == "1" ];then
                $IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
                $IWPRIV "$xml_if" pureg 0 > /dev/null 2>&1
                if [ "$wifi_devmode" == "bg" ];then
                    if [ "$xml_mode" == "11b" ]; then
                        $IWPRIV "$xml_if" mode 11b > /dev/null 2>&1
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
                    echo "$xml_wlan_autochannel" > /proc/sys/dev/$xml_wlan_wifi/nonoverlapping
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
                elif [ "$wifi_devmode" == "abg" ];then
                    if [ "$xml_modea" == "11b" ]; then
                        $IWPRIV "$xml_if" mode 11b > /dev/null 2>&1
                    elif [ "$xml_modea" == "bg" ]; then
                        $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
                    elif [ "$xml_modea" == "11g" ];then
                        $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
                        $IWPRIV "$xml_if" pureg 1 > /dev/null 2>&1
                    else
                        $IWPRIV "$xml_if" mode 11a > /dev/null 2>&1
                    fi
                    if [ "$xml_modea" != "11a" ];then
                        if [ "$xml_region" == "Asia" -o "$xml_region" == "Europe" ]; then       
                            $IWCONFIG "$xml_if" channel "$xml_wifi_channel1" > /dev/null 2>&1
                        elif [ "$xml_region" == "America" ]; then    
                            $IWCONFIG "$xml_if" channel "$xml_wifi_channel" > /dev/null 2>&1
                        else     
                            $IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
                        fi
                    else
                        $IWCONFIG "$xml_if" channel "$xml_wifi_channela" > /dev/null 2>&1
                    fi
                    echo "$xml_wlan_autochannel" > /proc/sys/dev/$xml_wlan_wifi/nonoverlapping
                    if [ "$xml_modea" == "11b"  ];then
                        if [ "$xml_wlan_rateb" == "auto" ]; then
                            $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                        else
                            $IWCONFIG "$xml_if" rate  "$xml_wlan_rateb" > /dev/null 2>&1
                        fi
                    elif [ "$xml_modea" == "11g" -o "$xml_modea" == "bg" ];then
                        if [ "$xml_wlan_rate" == "auto" ]; then
                            $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                        else
                            $IWCONFIG "$xml_if" rate  "$xml_wlan_rate" > /dev/null 2>&1
                        fi
                    else
                        if [ "$xml_wlan_ratea" == "auto" ]; then
                            $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                        else
                            $IWCONFIG "$xml_if" rate  "$xml_wlan_ratea" > /dev/null 2>&1
                        fi
                    fi
                else
                    if [ "$xml_moden" == "11b" ]; then
                        $IWPRIV "$xml_if" mode 11b > /dev/null 2>&1
                    elif [ "$xml_moden" == "bg" ]; then
                        $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
                        $IWPRIV "$xml_if" countryie 1  > /dev/null 2>&1
                        $IWPRIV "$xml_if" doth 0  > /dev/null 2>&1
                    elif [ "$xml_moden" == "11g" ];then
                        $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
                        $IWPRIV "$xml_if" pureg 1 > /dev/null 2>&1
                        $IWPRIV "$xml_if" countryie 1  > /dev/null 2>&1
                        $IWPRIV "$xml_if" doth 0  > /dev/null 2>&1
                    elif [ "$xml_moden" == "11a" ];then
                        $IWPRIV "$xml_if" mode 11a > /dev/null 2>&1
                        $IWPRIV "$xml_if" countryie 0  > /dev/null 2>&1
                        $IWPRIV "$xml_if" doth 1  > /dev/null 2>&1
                    elif [ "$xml_moden" == "11na" ]; then
                        $IWPRIV "$xml_if" bgscan 0  > /dev/null 2>&1
                        $IFCONFIG "$xml_if" txqueuelen 1000  > /dev/null 2>&1
                        $IFCONFIG "$xml_wlan_wifi" txqueuelen 1000  > /dev/null 2>&1
                        $IWPRIV "$xml_if" shortgi "$xml_shortgi"  > /dev/null 2>&1

                        if [ "$xml_cwwmode" == "0" ];then
                            $IWPRIV "$xml_if" mode 11NAHT20 > /dev/null 2>&1
                        fi
                        if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "0" ];then
                            $IWPRIV "$xml_if" mode 11NAHT20 > /dev/null 2>&1
                        fi
                        if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "1" ];then
                            $IWPRIV "$xml_if" mode 11NAHT40PLUS > /dev/null 2>&1
                        fi
                        if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "-1" ];then
                            $IWPRIV "$xml_if" mode 11NAHT40MINUS > /dev/null 2>&1
                        fi

                        #$IWPRIV "$xml_wlan_wifi" ForBiasAuto 1  > /dev/null 2>&1
                        $IWPRIV "$xml_if" extoffset "$xml_extoffset" > /dev/null 2>&1
                        $IWPRIV "$xml_if" cwmmode "$xml_cwwmode" > /dev/null 2>&1
                        $IWPRIV "$xml_wlan_wifi" AMPDU 1  > /dev/null 2>&1
                        $IWPRIV "$xml_wlan_wifi" AMPDUFrames 32  > /dev/null 2>&1
                        $IWPRIV "$xml_wlan_wifi" AMPDULim 50000  > /dev/null 2>&1
                        $IWPRIV "$xml_if" pureg 0 > /dev/null 2>&1
                        $IWPRIV "$xml_if" puren 0 > /dev/null 2>&1
                        eval txc="$"DEVTXC_$xml_wlan_wifi
                        $IWPRIV "$xml_wlan_wifi" txchainmask $txc  > /dev/null 2>&1
                        $IWPRIV "$xml_wlan_wifi" rxchainmask 3  > /dev/null 2>&1
                        echo 1 > /proc/sys/dev/ath/htdupieenable  > /dev/null 2>&1
                    elif [ "$xml_moden" == "11ng" ]; then
                        $IWPRIV "$xml_if" bgscan 0  > /dev/null 2>&1
                        $IFCONFIG "$xml_if" txqueuelen 1000  > /dev/null 2>&1
                        $IFCONFIG "$xml_wlan_wifi" txqueuelen 1000  > /dev/null 2>&1
                        $IWPRIV "$xml_if" shortgi "$xml_shortgi"  > /dev/null 2>&1

                        if [ "$xml_cwwmode" == "0" ];then
                            $IWPRIV "$xml_if" mode 11NGHT20 > /dev/null 2>&1
                        fi
                        if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "0" ];then
                            $IWPRIV "$xml_if" mode 11NGHT20 > /dev/null 2>&1
                        fi
                        if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "1" ];then
                            $IWPRIV "$xml_if" mode 11NGHT40PLUS > /dev/null 2>&1
                        fi
                        if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "-1" ];then
                            $IWPRIV "$xml_if" mode 11NGHT40MINUS > /dev/null 2>&1
                        fi

                        $IWPRIV "$xml_wlan_wifi" ForBiasAuto 1  > /dev/null 2>&1
                        $IWPRIV "$xml_if" extoffset "$xml_extoffset" > /dev/null 2>&1
                        $IWPRIV "$xml_if" cwmmode "$xml_cwwmode" > /dev/null 2>&1
                        $IWPRIV "$xml_wlan_wifi" AMPDU 1  > /dev/null 2>&1
                        $IWPRIV "$xml_wlan_wifi" AMPDUFrames 32  > /dev/null 2>&1
                        $IWPRIV "$xml_wlan_wifi" AMPDULim 50000  > /dev/null 2>&1
                        $IWPRIV "$xml_if" pureg 0 > /dev/null 2>&1
                        $IWPRIV "$xml_if" puren 0 > /dev/null 2>&1
                        eval txc="$"DEVTXC_$xml_wlan_wifi
                        $IWPRIV "$xml_wlan_wifi" txchainmask $txc  > /dev/null 2>&1
                        $IWPRIV "$xml_wlan_wifi" rxchainmask 3  > /dev/null 2>&1
                        echo 1 > /proc/sys/dev/ath/htdupieenable  > /dev/null 2>&1
                    fi

                    if [ "$xml_moden" == "11a" -o "$xml_moden" == "11na"  ];then
                        $IWCONFIG "$xml_if" channel "$xml_wifi_channela" > /dev/null 2>&1
                    else
                        if [ "$xml_region" == "Asia" -o "$xml_region" == "Europe" ]; then       
                            $IWCONFIG "$xml_if" channel "$xml_wifi_channel1" > /dev/null 2>&1
                        elif [ "$xml_region" == "America" ]; then    
                            $IWCONFIG "$xml_if" channel "$xml_wifi_channel" > /dev/null 2>&1
                        else     
                            $IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
                        fi
                    fi
                    if [ "$xml_moden" == "11ng" -o "$xml_moden" == "11na" ];then
                        if [ "$xml_wlan_rate_n_ht20ds" == "auto" ]; then
                            $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                        else
                            $IWCONFIG "$xml_if" rate  "$xml_wlan_rate_n_ht20ds" > /dev/null 2>&1
                        fi
                    elif [ "$xml_moden" == "11g" -o "$xml_moden" == "bg" ];then
                        if [ "$xml_wlan_rate" == "auto" ]; then
                            $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                        else
                            $IWCONFIG "$xml_if" rate  "$xml_wlan_rate" > /dev/null 2>&1
                        fi
                    elif [ "$xml_moden" == "11b" ];then
                        if [ "$xml_wlan_rateb" == "auto" ]; then
                            $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                        else
                            $IWCONFIG "$xml_if" rate  "$xml_wlan_rateb" > /dev/null 2>&1
                        fi
                    else
                        if [ "$xml_wlan_ratea" == "auto" ]; then
                            $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                        else
                            $IWCONFIG "$xml_if" rate  "$xml_wlan_ratea" > /dev/null 2>&1
                        fi
                    fi
                fi
            fi
            #$IFCONFIG "$xml_if" up > /dev/null 2>&1

            #vap_wifi_allup


            $IWPRIV "$xml_if" bintval "$xml_wlan_beacon" > /dev/null 2>&1



            if [ "$xml_broadcast" == "yes" ]; then
                $IWPRIV "$xml_if" hide_ssid 0 > /dev/null 2>&1
            elif [ "$xml_broadcast" == "no" ]; then
                $IWPRIV "$xml_if" hide_ssid 1 > /dev/null 2>&1
            fi

            #	$IFCONFIG "$xml_if" down > /dev/null 2>&1
            #


            echo "$DEVICE_MODEL" | grep -q 'N'
            if [ $? -eq 0 ];then
                echo "11n"
                #bandwidth_startup
                local vapname="vap$vapnum"
                local xml_config_apbw=`config_read /config/network/$vapname/bandwidth/apbw`
                local xml_apbw_maxclient=`config_getoption "$xml_config_apbw" maxclientrate`
                local xml_apbw_maxclient_send=`config_getoption "$xml_config_apbw" maxclientrate_send`
                local xml_apbw_enable=`config_getoption "$xml_config_apbw" apbwenable`

                local xml_config_estabw=`config_read /config/network/$vapname/bandwidth/estabw`
                local xml_estabw_enable=`config_getoption "$xml_config_estabw" estabwenable`
                local xml_stabw_enable=`config_getoption "$xml_config_estabw" stabwenable`
                local xml_estabw_maxbw=`config_getoption "$xml_config_estabw" maxbw`
                local xml_estabw_maxbw_send=`config_getoption "$xml_config_estabw" maxbw_send`

                local xml_config_stabw1=`config_read /config/network/$vapname/bandwidth/stabw1`
                local xml_stabw1_mac=`config_getoption "$xml_config_stabw1" mac`
                local xml_stabw1_maxbw=`config_getoption "$xml_config_stabw1" maxbw`
                local xml_stabw1_maxbw_send=`config_getoption "$xml_config_stabw1" maxbw_send`

                local xml_config_stabw2=`config_read /config/network/$vapname/bandwidth/stabw2`
                local xml_stabw2_mac=`config_getoption "$xml_config_stabw2" mac`
                local xml_stabw2_maxbw=`config_getoption "$xml_config_stabw2" maxbw`
                local xml_stabw2_maxbw_send=`config_getoption "$xml_config_stabw2" maxbw_send`

                local xml_config_stabw3=`config_read /config/network/$vapname/bandwidth/stabw3`
                local xml_stabw3_mac=`config_getoption "$xml_config_stabw3" mac`
                local xml_stabw3_maxbw=`config_getoption "$xml_config_stabw3" maxbw`
                local xml_stabw3_maxbw_send=`config_getoption "$xml_config_stabw3" maxbw_send`

                local xml_config_stabw4=`config_read /config/network/$vapname/bandwidth/stabw4`
                local xml_stabw4_mac=`config_getoption "$xml_config_stabw4" mac`
                local xml_stabw4_maxbw=`config_getoption "$xml_config_stabw4" maxbw`
                local xml_stabw4_maxbw_send=`config_getoption "$xml_config_stabw4" maxbw_send`

                local xml_config_stabw5=`config_read /config/network/$vapname/bandwidth/stabw5`
                local xml_stabw5_mac=`config_getoption "$xml_config_stabw5" mac`
                local xml_stabw5_maxbw=`config_getoption "$xml_config_stabw5" maxbw`
                local xml_stabw5_maxbw_send=`config_getoption "$xml_config_stabw5" maxbw_send`

                local xml_config_stabw6=`config_read /config/network/$vapname/bandwidth/stabw6`
                local xml_stabw6_mac=`config_getoption "$xml_config_stabw6" mac`
                local xml_stabw6_maxbw=`config_getoption "$xml_config_stabw6" maxbw`
                local xml_stabw6_maxbw_send=`config_getoption "$xml_config_stabw6" maxbw_send`

                local xml_config_stabw7=`config_read /config/network/$vapname/bandwidth/stabw7`
                local xml_stabw7_mac=`config_getoption "$xml_config_stabw7" mac`
                local xml_stabw7_maxbw=`config_getoption "$xml_config_stabw7" maxbw`
                local xml_stabw7_maxbw_send=`config_getoption "$xml_config_stabw7" maxbw_send`

                local xml_config_stabw8=`config_read /config/network/$vapname/bandwidth/stabw8`
                local xml_stabw8_mac=`config_getoption "$xml_config_stabw8" mac`
                local xml_stabw8_maxbw=`config_getoption "$xml_config_stabw8" maxbw`
                local xml_stabw8_maxbw_send=`config_getoption "$xml_config_stabw8" maxbw_send`

                local xml_config_stabw9=`config_read /config/network/$vapname/bandwidth/stabw9`
                local xml_stabw9_mac=`config_getoption "$xml_config_stabw9" mac`
                local xml_stabw9_maxbw=`config_getoption "$xml_config_stabw9" maxbw`
                local xml_stabw9_maxbw_send=`config_getoption "$xml_config_stabw9" maxbw_send`

                $AUTELAN traffic_limit $xml_if set_vap_flag $xml_apbw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_every_node_flag $xml_estabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw_enable > /dev/null 2>&1

                if [ "$xml_apbw_enable" == "1" ];then
                    [ -n "$xml_apbw_maxclient" ] && $AUTELAN traffic_limit $xml_if set_vap $xml_apbw_maxclient > /dev/null 2>&1
                    [ -n "$xml_apbw_maxclient_send" ] && $AUTELAN traffic_limit $xml_if set_vap_send $xml_apbw_maxclient_send > /dev/null 2>&1
                fi

                if [ "$xml_estabw_enable" == "1" ];then
                    [ -n "$xml_estabw_maxbw" ] && $AUTELAN traffic_limit $xml_if set_every_node $xml_estabw_maxbw > /dev/null 2>&1
                    [ -n "$xml_estabw_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_every_node_send $xml_estabw_maxbw_send > /dev/null 2>&1
                fi


                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw1_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw2_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw3_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw4_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw5_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw6_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw7_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw8_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw9_mac $xml_stabw_enable > /dev/null 2>&1
                if [ "$xml_stabw_enable" == "1" ];then
                    [ -n "$xml_stabw1_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw1_mac $xml_stabw1_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw2_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw2_mac $xml_stabw2_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw3_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw3_mac $xml_stabw3_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw4_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw4_mac $xml_stabw4_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw5_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw5_mac $xml_stabw5_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw6_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw6_mac $xml_stabw6_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw7_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw7_mac $xml_stabw7_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw8_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw8_mac $xml_stabw8_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw9_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw9_mac $xml_stabw9_maxbw > /dev/null 2>&1

                    [ -n "$xml_stabw1_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw1_mac $xml_stabw1_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw2_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw2_mac $xml_stabw2_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw3_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw3_mac $xml_stabw3_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw4_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw4_mac $xml_stabw4_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw5_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw5_mac $xml_stabw5_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw6_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw6_mac $xml_stabw6_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw7_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw7_mac $xml_stabw7_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw8_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw8_mac $xml_stabw8_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw9_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw9_mac $xml_stabw9_maxbw_send > /dev/null 2>&1

                fi



            else
                #wmm_conf
                local vapname="vap$vapnum"  
                local xml_config_wmm=`config_read /config/network/$vapname/wmm`
                local xml_wlan_wmm_enable=`config_getoption "$xml_config_wmm" enable`
                $IWPRIV $xml_if wmm $xml_wlan_wmm_enable > /dev/null 2>&1
                if [ "$xml_wlan_wmm_enable" == "1" ];then

                    local xml_wmm_wtenable=`config_getoption "$xml_config_wmm" wtenable`
                    local xml_wmm_wvenable=`config_getoption "$xml_config_wmm" wvenable`
                    local xml_wmm_bgmint=`config_getoption "$xml_config_wmm" bgmint`
                    local xml_wmm_bgmaxt=`config_getoption "$xml_config_wmm" bgmaxt`
                    local xml_wmm_bgaift=`config_getoption "$xml_config_wmm" bgaift`
                    local xml_wmm_bgopbt=`config_getoption "$xml_config_wmm" bgopbt`
                    local xml_wmm_bgnoack=`config_getoption "$xml_config_wmm" bgnoack`

                    local xml_wmm_bemint=`config_getoption "$xml_config_wmm" bemint`
                    local xml_wmm_bemaxt=`config_getoption "$xml_config_wmm" bemaxt`
                    local xml_wmm_beaift=`config_getoption "$xml_config_wmm" beaift`
                    local xml_wmm_beopbt=`config_getoption "$xml_config_wmm" beopbt`
                    local xml_wmm_benoack=`config_getoption "$xml_config_wmm" benoack`

                    local xml_wmm_vdmint=`config_getoption "$xml_config_wmm" vdmint`
                    local xml_wmm_vdmaxt=`config_getoption "$xml_config_wmm" vdmaxt`
                    local xml_wmm_vdaift=`config_getoption "$xml_config_wmm" vdaift`
                    local xml_wmm_vdopbt=`config_getoption "$xml_config_wmm" vdopbt`
                    local xml_wmm_vdnoack=`config_getoption "$xml_config_wmm" vdnoack`

                    local xml_wmm_vcmint=`config_getoption "$xml_config_wmm" vcmint`
                    local xml_wmm_vcmaxt=`config_getoption "$xml_config_wmm" vcmaxt`
                    local xml_wmm_vcaift=`config_getoption "$xml_config_wmm" vcaift`
                    local xml_wmm_vcopbt=`config_getoption "$xml_config_wmm" vcopbt`
                    local xml_wmm_vcnoack=`config_getoption "$xml_config_wmm" vcnoack`

                    local xml_wmm_bgminf=`config_getoption "$xml_config_wmm" bgminf`
                    local xml_wmm_bgmaxf=`config_getoption "$xml_config_wmm" bgmaxf`
                    local xml_wmm_bgaiff=`config_getoption "$xml_config_wmm" bgaiff`
                    local xml_wmm_bgopbf=`config_getoption "$xml_config_wmm" bgopbf`

                    local xml_wmm_beminf=`config_getoption "$xml_config_wmm" beminf`
                    local xml_wmm_bemaxf=`config_getoption "$xml_config_wmm" bemaxf`
                    local xml_wmm_beaiff=`config_getoption "$xml_config_wmm" beaiff`
                    local xml_wmm_beopbf=`config_getoption "$xml_config_wmm" beopbf`

                    local xml_wmm_vdminf=`config_getoption "$xml_config_wmm" vdminf`
                    local xml_wmm_vdmaxf=`config_getoption "$xml_config_wmm" vdmaxf`
                    local xml_wmm_vdaiff=`config_getoption "$xml_config_wmm" vdaiff`
                    local xml_wmm_vdopbf=`config_getoption "$xml_config_wmm" vdopbf`

                    local xml_wmm_vcminf=`config_getoption "$xml_config_wmm" vcminf`
                    local xml_wmm_vcmaxf=`config_getoption "$xml_config_wmm" vcmaxf`
                    local xml_wmm_vcaiff=`config_getoption "$xml_config_wmm" vcaiff`
                    local xml_wmm_vcopbf=`config_getoption "$xml_config_wmm" vcopbf`


                    $SLEEP 5 > /dev/null 2>&1
                    $AUTELAN wmm tos_flag $xml_if $xml_wmm_wtenable > /dev/null 2>&1
                    $AUTELAN wmm 1p_flag $xml_if $xml_wmm_wvenable > /dev/null 2>&1

                    $IWPRIV $xml_if cwmin 1 0 $xml_wmm_bgmint > /dev/null 2>&1
                    $IWPRIV $xml_if cwmax 1 0 $xml_wmm_bgmaxt > /dev/null 2>&1
                    $IWPRIV $xml_if aifs 1 0 $xml_wmm_bgaift > /dev/null 2>&1
                    $IWPRIV $xml_if txoplimit 1 0 $xml_wmm_bgopbt > /dev/null 2>&1
                    $IWPRIV $xml_if noackpolicy 1 0 $xml_wmm_bgnoack > /dev/null 2>&1

                    $IWPRIV $xml_if cwmin 0 0 $xml_wmm_bemint > /dev/null 2>&1
                    $IWPRIV $xml_if cwmax 0 0 $xml_wmm_bemaxt > /dev/null 2>&1
                    $IWPRIV $xml_if aifs 0 0 $xml_wmm_beaift > /dev/null 2>&1
                    $IWPRIV $xml_if txoplimit 0 0 $xml_wmm_beopbt > /dev/null 2>&1
                    $IWPRIV $xml_if noackpolicy 0 0 $xml_wmm_benoack > /dev/null 2>&1

                    $IWPRIV $xml_if cwmin 2 0 $xml_wmm_vdmint > /dev/null 2>&1
                    $IWPRIV $xml_if cwmax 2 0 $xml_wmm_vdmaxt > /dev/null 2>&1
                    $IWPRIV $xml_if aifs 2 0 $xml_wmm_vdaift > /dev/null 2>&1
                    $IWPRIV $xml_if txoplimit 2 0 $xml_wmm_vdopbt > /dev/null 2>&1
                    $IWPRIV $xml_if noackpolicy 2 0 $xml_wmm_vdnoack > /dev/null 2>&1

                    $IWPRIV $xml_if cwmin 3 0 $xml_wmm_vcmint > /dev/null 2>&1
                    $IWPRIV $xml_if cwmax 3 0 $xml_wmm_vcmaxt > /dev/null 2>&1
                    $IWPRIV $xml_if aifs 3 0 $xml_wmm_vcaift > /dev/null 2>&1
                    $IWPRIV $xml_if txoplimit 3 0 $xml_wmm_vcopbt > /dev/null 2>&1
                    $IWPRIV $xml_if noackpolicy 3 0 $xml_wmm_vcnoack > /dev/null 2>&1

                    $IWPRIV $xml_if cwmin 1 1 $xml_wmm_bgminf > /dev/null 2>&1
                    $IWPRIV $xml_if cwmax 1 1 $xml_wmm_bgmaxf > /dev/null 2>&1
                    $IWPRIV $xml_if aifs 1 1 $xml_wmm_bgaiff > /dev/null 2>&1
                    $IWPRIV $xml_if txoplimit 1 1 $xml_wmm_bgopbf > /dev/null 2>&1

                    $IWPRIV $xml_if cwmin 0 1 $xml_wmm_beminf > /dev/null 2>&1
                    $IWPRIV $xml_if cwmax 0 1 $xml_wmm_bemaxf > /dev/null 2>&1
                    $IWPRIV $xml_if aifs 0 1 $xml_wmm_beaiff > /dev/null 2>&1
                    $IWPRIV $xml_if txoplimit 0 1 $xml_wmm_beopbf > /dev/null 2>&1

                    $IWPRIV $xml_if cwmin 2 1 $xml_wmm_vdminf > /dev/null 2>&1
                    $IWPRIV $xml_if cwmax 2 1 $xml_wmm_vdmaxf > /dev/null 2>&1
                    $IWPRIV $xml_if aifs 2 1 $xml_wmm_vdaiff > /dev/null 2>&1
                    $IWPRIV $xml_if txoplimit 2 1 $xml_wmm_vdopbf > /dev/null 2>&1

                    $IWPRIV $xml_if cwmin 3 1 $xml_wmm_vcminf > /dev/null 2>&1
                    $IWPRIV $xml_if cwmax 3 1 $xml_wmm_vcmaxf > /dev/null 2>&1
                    $IWPRIV $xml_if aifs 3 1 $xml_wmm_vcaiff > /dev/null 2>&1
                    $IWPRIV $xml_if txoplimit 3 1  $xml_wmm_vcopbf > /dev/null 2>&1

                    if [ "$xml_wmm_wvenable" == "1" ];then
                        local xml_wmm_wvingress0=`config_getoption "$xml_config_wmm" wvingress0`
                        local xml_wmm_wvingress1=`config_getoption "$xml_config_wmm" wvingress1`
                        local xml_wmm_wvingress2=`config_getoption "$xml_config_wmm" wvingress2`
                        local xml_wmm_wvingress3=`config_getoption "$xml_config_wmm" wvingress3` 

                        local xml_wmm_wvegress0=`config_getoption "$xml_config_wmm" wvegress0`
                        local xml_wmm_wvegress1=`config_getoption "$xml_config_wmm" wvegress1`
                        local xml_wmm_wvegress2=`config_getoption "$xml_config_wmm" wvegress2`
                        local xml_wmm_wvegress3=`config_getoption "$xml_config_wmm" wvegress3`
                        local xml_wmm_wvegress4=`config_getoption "$xml_config_wmm" wvegress4`
                        local xml_wmm_wvegress5=`config_getoption "$xml_config_wmm" wvegress5`
                        local xml_wmm_wvegress6=`config_getoption "$xml_config_wmm" wvegress6`
                        local xml_wmm_wvegress7=`config_getoption "$xml_config_wmm" wvegress7`

                        $AUTELAN wmm 1p_ingress_map $xml_if 0 $xml_wmm_wvingress0 > /dev/null 2>&1
                        $AUTELAN wmm 1p_ingress_map $xml_if 1 $xml_wmm_wvingress1 > /dev/null 2>&1
                        $AUTELAN wmm 1p_ingress_map $xml_if 2 $xml_wmm_wvingress2 > /dev/null 2>&1
                        $AUTELAN wmm 1p_ingress_map $xml_if 3 $xml_wmm_wvingress3 > /dev/null 2>&1

                        $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress0 0 > /dev/null 2>&1
                        $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress1 1 > /dev/null 2>&1
                        $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress2 2 > /dev/null 2>&1
                        $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress3 3 > /dev/null 2>&1
                        $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress4 4 > /dev/null 2>&1
                        $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress5 5 > /dev/null 2>&1
                        $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress6 6 > /dev/null 2>&1
                        $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress7 7 > /dev/null 2>&1
                    fi

                    if [ "$xml_wmm_wtenable" == "1" ];then
                        local xml_wmm_wtegress8=`config_getoption "$xml_config_wmm" wtegress8`
                        local xml_wmm_wtegress32=`config_getoption "$xml_config_wmm" wtegress32`
                        local xml_wmm_wtegress40=`config_getoption "$xml_config_wmm" wtegress40`
                        local xml_wmm_wtegress48=`config_getoption "$xml_config_wmm" wtegress48`
                        local xml_wmm_wtegress136=`config_getoption "$xml_config_wmm" wtegress136`
                        local xml_wmm_wtegress160=`config_getoption "$xml_config_wmm" wtegress160`
                        local xml_wmm_wtegress184=`config_getoption "$xml_config_wmm" wtegress184`
                        local xml_wmm_wtegress224=`config_getoption "$xml_config_wmm" wtegress224`

                        local xml_wmm_twegress8=`config_getoption "$xml_config_wmm" twegress8`
                        local xml_wmm_twegress32=`config_getoption "$xml_config_wmm" twegress32`
                        local xml_wmm_twegress40=`config_getoption "$xml_config_wmm" twegress40`
                        local xml_wmm_twegress48=`config_getoption "$xml_config_wmm" twegress48`
                        local xml_wmm_twegress136=`config_getoption "$xml_config_wmm" twegress136`
                        local xml_wmm_twegress160=`config_getoption "$xml_config_wmm" twegress160`
                        local xml_wmm_twegress184=`config_getoption "$xml_config_wmm" twegress184`
                        local xml_wmm_twegress224=`config_getoption "$xml_config_wmm" twegress224`

                        $AUTELAN wmm tos_egress_map $xml_if 0 $xml_wmm_twegress8 $xml_wmm_wtegress8 > /dev/null 2>&1
                        $AUTELAN wmm tos_egress_map $xml_if 1 $xml_wmm_twegress32 $xml_wmm_wtegress32 > /dev/null 2>&1
                        $AUTELAN wmm tos_egress_map $xml_if 2 $xml_wmm_twegress40 $xml_wmm_wtegress40 > /dev/null 2>&1
                        $AUTELAN wmm tos_egress_map $xml_if 3 $xml_wmm_twegress48 $xml_wmm_wtegress48 > /dev/null 2>&1
                        $AUTELAN wmm tos_egress_map $xml_if 4 $xml_wmm_twegress136 $xml_wmm_wtegress136 > /dev/null 2>&1
                        $AUTELAN wmm tos_egress_map $xml_if 5 $xml_wmm_twegress160 $xml_wmm_wtegress160 > /dev/null 2>&1
                        $AUTELAN wmm tos_egress_map $xml_if 6 $xml_wmm_twegress184 $xml_wmm_wtegress184 > /dev/null 2>&1
                        $AUTELAN wmm tos_egress_map $xml_if 7 $xml_wmm_twegress224 $xml_wmm_wtegress224 > /dev/null 2>&1
                    fi
                fi



                #bandwidth_startup
                local vapname="vap$vapnum"

                local xml_config_apbw=`config_read /config/network/$vapname/bandwidth/apbw`
                local xml_apbw_maxclient=`config_getoption "$xml_config_apbw" maxclientrate`
                local xml_apbw_maxclient_send=`config_getoption "$xml_config_apbw" maxclientrate_send`
                local xml_apbw_enable=`config_getoption "$xml_config_apbw" apbwenable`

                local xml_config_estabw=`config_read /config/network/$vapname/bandwidth/estabw`
                local xml_estabw_enable=`config_getoption "$xml_config_estabw" estabwenable`
                local xml_stabw_enable=`config_getoption "$xml_config_estabw" stabwenable`
                local xml_estabw_maxbw=`config_getoption "$xml_config_estabw" maxbw`
                local xml_estabw_maxbw_send=`config_getoption "$xml_config_estabw" maxbw_send`

                local xml_config_stabw1=`config_read /config/network/$vapname/bandwidth/stabw1`
                local xml_stabw1_mac=`config_getoption "$xml_config_stabw1" mac`
                local xml_stabw1_maxbw=`config_getoption "$xml_config_stabw1" maxbw`
                local xml_stabw1_maxbw_send=`config_getoption "$xml_config_stabw1" maxbw_send`

                local xml_config_stabw2=`config_read /config/network/$vapname/bandwidth/stabw2`
                local xml_stabw2_mac=`config_getoption "$xml_config_stabw2" mac`
                local xml_stabw2_maxbw=`config_getoption "$xml_config_stabw2" maxbw`
                local xml_stabw2_maxbw_send=`config_getoption "$xml_config_stabw2" maxbw_send`

                local xml_config_stabw3=`config_read /config/network/$vapname/bandwidth/stabw3`
                local xml_stabw3_mac=`config_getoption "$xml_config_stabw3" mac`
                local xml_stabw3_maxbw=`config_getoption "$xml_config_stabw3" maxbw`
                local xml_stabw3_maxbw_send=`config_getoption "$xml_config_stabw3" maxbw_send`

                local xml_config_stabw4=`config_read /config/network/$vapname/bandwidth/stabw4`
                local xml_stabw4_mac=`config_getoption "$xml_config_stabw4" mac`
                local xml_stabw4_maxbw=`config_getoption "$xml_config_stabw4" maxbw`
                local xml_stabw4_maxbw_send=`config_getoption "$xml_config_stabw4" maxbw_send`

                local xml_config_stabw5=`config_read /config/network/$vapname/bandwidth/stabw5`
                local xml_stabw5_mac=`config_getoption "$xml_config_stabw5" mac`
                local xml_stabw5_maxbw=`config_getoption "$xml_config_stabw5" maxbw`
                local xml_stabw5_maxbw_send=`config_getoption "$xml_config_stabw5" maxbw_send`

                local xml_config_stabw6=`config_read /config/network/$vapname/bandwidth/stabw6`
                local xml_stabw6_mac=`config_getoption "$xml_config_stabw6" mac`
                local xml_stabw6_maxbw=`config_getoption "$xml_config_stabw6" maxbw`
                local xml_stabw6_maxbw_send=`config_getoption "$xml_config_stabw6" maxbw_send`

                local xml_config_stabw7=`config_read /config/network/$vapname/bandwidth/stabw7`
                local xml_stabw7_mac=`config_getoption "$xml_config_stabw7" mac`
                local xml_stabw7_maxbw=`config_getoption "$xml_config_stabw7" maxbw`
                local xml_stabw7_maxbw_send=`config_getoption "$xml_config_stabw7" maxbw_send`

                local xml_config_stabw8=`config_read /config/network/$vapname/bandwidth/stabw8`
                local xml_stabw8_mac=`config_getoption "$xml_config_stabw8" mac`
                local xml_stabw8_maxbw=`config_getoption "$xml_config_stabw8" maxbw`
                local xml_stabw8_maxbw_send=`config_getoption "$xml_config_stabw8" maxbw_send`

                local xml_config_stabw9=`config_read /config/network/$vapname/bandwidth/stabw9`
                local xml_stabw9_mac=`config_getoption "$xml_config_stabw9" mac`
                local xml_stabw9_maxbw=`config_getoption "$xml_config_stabw9" maxbw`
                local xml_stabw9_maxbw_send=`config_getoption "$xml_config_stabw9" maxbw_send`

                $AUTELAN traffic_limit $xml_if set_vap_flag $xml_apbw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_every_node_flag $xml_estabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw_enable > /dev/null 2>&1

                if [ "$xml_apbw_enable" == "1" ];then
                    [ -n "$xml_apbw_maxclient" ] && $AUTELAN traffic_limit $xml_if set_vap $xml_apbw_maxclient > /dev/null 2>&1
                    [ -n "$xml_apbw_maxclient_send" ] && $AUTELAN traffic_limit $xml_if set_vap_send $xml_apbw_maxclient_send > /dev/null 2>&1
                fi

                if [ "$xml_estabw_enable" == "1" ];then
                    [ -n "$xml_estabw_maxbw" ] && $AUTELAN traffic_limit $xml_if set_every_node $xml_estabw_maxbw > /dev/null 2>&1
                    [ -n "$xml_estabw_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_every_node_send $xml_estabw_maxbw_send > /dev/null 2>&1
                fi


                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw1_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw2_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw3_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw4_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw5_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw6_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw7_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw8_mac $xml_stabw_enable > /dev/null 2>&1
                $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw9_mac $xml_stabw_enable > /dev/null 2>&1
                if [ "$xml_stabw_enable" == "1" ];then
                    [ -n "$xml_stabw1_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw1_mac $xml_stabw1_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw2_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw2_mac $xml_stabw2_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw3_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw3_mac $xml_stabw3_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw4_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw4_mac $xml_stabw4_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw5_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw5_mac $xml_stabw5_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw6_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw6_mac $xml_stabw6_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw7_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw7_mac $xml_stabw7_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw8_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw8_mac $xml_stabw8_maxbw > /dev/null 2>&1
                    [ -n "$xml_stabw9_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw9_mac $xml_stabw9_maxbw > /dev/null 2>&1

                    [ -n "$xml_stabw1_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw1_mac $xml_stabw1_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw2_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw2_mac $xml_stabw2_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw3_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw3_mac $xml_stabw3_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw4_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw4_mac $xml_stabw4_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw5_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw5_mac $xml_stabw5_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw6_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw6_mac $xml_stabw6_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw7_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw7_mac $xml_stabw7_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw8_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw8_mac $xml_stabw8_maxbw_send > /dev/null 2>&1
                    [ -n "$xml_stabw9_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw9_mac $xml_stabw9_maxbw_send > /dev/null 2>&1

                fi
            fi
        elif [ "$xml_wlanmode" == "sta" ]||[ "$xml_wlanmode" == "client" ]&&[ "$xml_enable" == "yes"  ]; then
            echo "$vapnum $xml_wlanmode $xml_enable" >>/tmp/bbb
            $WLANCONFIG "$xml_if" create wlandev "$xml_wlan_wifi" wlanmode sta nosbeacon > /dev/null 2>&1 

            if [ "$xml_wlanmode" == "client" ];then
                $IWPRIV $xml_if wds 1 > /dev/null 2>&1
                $IWPRIV $xml_if hostroaming 1 > /dev/null 2>&1
            fi 
            local xml_wlwan=`config_read /config/network/vap$vapnum`
            local xml_wlwan_enable=`config_getoption "$xml_wlwan" enable`
            local xml_wlwan_forward2=`config_getoption "$xml_wlwan" forward2`
            local xml_wlwan_wlanmode=`config_getoption "$xml_wlwan" wlanmode`
            local xml_wlwan_if=`config_getoption "$xml_wlwan" if`
            local xml_wlwan_ssid=`config_getoption "$xml_wlwan" ssid`
            local xml_wlwan_security=`config_getoption "$xml_wlwan" sectype`
            local xml_wlan_assocchan=`config_getoption "$xml_wlwan" assocchan`
            local xml_wlan_assocweight=`config_getoption "$xml_wlwan" assocweight`
            local xml_wlan_assocrssi=`config_getoption "$xml_wlwan" assocrssi`

            local xml_wep=`config_read /config/network/vap$vapnum/wep`
            local xml_wlwan_authmode=`config_getoption "$xml_wep" wepmode`
            local xml_wlwan_keytype=`config_getoption "$xml_wep" wepkeytype`
            local xml_wlwan_cwepkey=`config_getoption "$xml_wep" cwepkey`
            local xml_wlwan_passkey1=`config_getoption "$xml_wep" wepkey1`
            local xml_wlwan_passkey2=`config_getoption "$xml_wep" wepkey2`
            local xml_wlwan_passkey3=`config_getoption "$xml_wep" wepkey3`
            local xml_wlwan_passkey4=`config_getoption "$xml_wep" wepkey4`

            local xml_wpa=`config_read /config/network/vap$vapnum/wpa`
            local xml_wlwan_passphrase=`config_getoption "$xml_wpa" wpapass`

            if [ "$xml_wlwan_forward2" == "yes" ]; then
                $BRCTL addif $BRLAN $xml_if > /dev/null 2>&1
            fi

            local CONF_FILE=/etc/wpa_supplicant.conf

            if [ "$xml_wlwan_enable" == "yes" ];then
                local cwepkey=`expr $xml_wlwan_cwepkey + 1`
                eval passkey="$"xml_wlwan_passkey$cwepkey
                $IWPRIV "$xml_wlwan_if" select_weight  $xml_wlan_assocweight > /dev/null 2>&1
                $IWPRIV "$xml_wlwan_if" select_chan  $xml_wlan_assocchan > /dev/null 2>&1
                $IWPRIV "$xml_wlwan_if" near_rssi  $xml_wlan_assocrssi > /dev/null 2>&1
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
                            echo "	auth_alg=SHARED" >> $CONF_FILE
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
                        echo "	proto=wpa2" >> $CONF_FILE
                        echo "	psk=\"$xml_wlwan_passphrase\"" >> $CONF_FILE
                        echo "}" >> $CONF_FILE
                esac


                sleep 2 > /dev/null 2>&1
                if [ "$xml_wlwan_forward2" == "yes" ]; then
                    $WPA_SUPPLICANT -B -c /etc/wpa_supplicant.conf -i $xml_wlwan_if -b ${BRLAN} -D madwifi > /dev/null 2>&1
                else
                    $WPA_SUPPLICANT -B -c /etc/wpa_supplicant.conf -i $xml_wlwan_if -D madwifi > /dev/null 2>&1
                fi
            else
                $IFCONFIG $xml_wlwan_if down > /dev/null 2>&1
            fi        
        fi
    done

}

#
#sys_security_startsetup
#
sys_security_startsetup()
{
    for vapnum in `seq $NUM_ap`
    do
        local xml_wlan=`config_read /config/network/vap$vapnum`
        local xml_enable=`config_getoption "$xml_wlan" create`
        local xml_wlanmode=`config_getoption "$xml_wlan" wlanmode`

        if [ "$xml_wlanmode" == "ap" ]||[ "$xml_wlanmode" == "root" ]&&[ "$xml_enable" == "yes"  ]; then
            local xml_if=`config_getoption "$xml_wlan" if`
            local xml_security=`config_getoption "$xml_wlan" security`
            local xml_authmode=`config_getoption "$xml_wlan" authmode`
            local xml_wpamode=`config_getoption "$xml_wlan" wpamode`
            local xml_wepkey=`config_getoption "$xml_wlan" wepkey`
            local xml_wepkeytype=`config_getoption "$xml_wlan" wepkeytype`

            local  xml_passkey1=`config_getoption "$xml_wlan" passkey1`
            local  xml_passkey2=`config_getoption "$xml_wlan" passkey2`
            local  xml_passkey3=`config_getoption "$xml_wlan" passkey3`
            local  xml_passkey4=`config_getoption "$xml_wlan" passkey4`

            local  xml_wapi_authtype=`config_getoption "$xml_wlan" wapibook`
            local  xml_wapi_sectype=`config_getoption "$xml_wlan" wapiauth`
            local  xml_wapi_enctype=`config_getoption "$xml_wlan" wapienc`
            local  xml_wapi_passphrase=`config_getoption "$xml_wlan" wapipass`
            local  xml_wlan_serverip=`config_getoption "$xml_wlan" wapisip`
            local  xml_wlan_serverport=`config_getoption "$xml_wlan" wapisport`

            local  xml_wapi_if3cert=`config_getoption "$xml_wlan" if3cert`

            if [ "$xml_security" == "wep" ]; then 
                if [ "$xml_authmode" == "3" ]; then
                    sys_wparadius_startup
                    $HOSTAPD -B /jffs/hostapd/b$vapnum 2>&1 >/dev/null &
                else
                    if [ "$xml_authmode" == "2" ];then
                        $IWPRIV "$xml_if" authmode "$xml_authmode" > /dev/null 2>&1
                    fi
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
                fi
            elif [ "$xml_security" == "none" ]; then
                $IWCONFIG "$xml_if" key open off > /dev/null 2>&1
            elif [ "$xml_security" == "wpa" ]; then
                if [ "$xml_wpamode" == "PSK" ]; then
                    sys_wpapsk_startup
                    $HOSTAPD -B /jffs/hostapd/a$vapnum 2>&1 >/dev/null &
                else
                    sys_wparadius_startup
                    $HOSTAPD -B /jffs/hostapd/b$vapnum 2>&1 >/dev/null &
                fi
            elif [ "$xml_security" == "wpa2" ]; then
                if [ "$xml_wpamode" == "PSK" ]; then
                    sys_wpapsk_startup
                    $HOSTAPD -B /jffs/hostapd/a$vapnum 2>&1 >/dev/null &
                else
                    sys_wparadius_startup
                    $HOSTAPD -B /jffs/hostapd/b$vapnum 2>&1 >/dev/null &
                fi
            elif [ "$xml_security" == "wapi" ]; then
                $WAPISET wlan $xml_if  add 2>&1 > /dev/null
                if [ "$xml_wapi_sectype" == "0" ]; then
                    $WAPISET wlan $xml_if set mode open 2>&1 > /dev/null
                elif [ "$xml_wapi_sectype" == "7" ]; then
                    $WAPISET wlan $xml_if set mode psk 2>&1 > /dev/null
                    $WAPISET wlan $xml_if set psktype $xml_wapi_enctype 2>&1 > /dev/null
                    $WAPISET wlan $xml_if set psk  $xml_wapi_passphrase 2>&1 > /dev/null
                elif [ "$xml_wapi_sectype" == "11" ]; then
                    $WAPISET wlan $xml_if set mode cert 2>&1 > /dev/null
                    $WAPISET certname /jffs/user.cer 2>&1 > /dev/null
                    $WAPISET asuip  $xml_wlan_serverip  2>&1 > /dev/null
                    $WAPISET authmode 2 2>&1 > /dev/null
                    #$CERT_UNITE /jffs/as_cert /jffs/ap_cert /jffs/user.cer 2>&1 > /dev/null
                    if [ "$xml_wapi_if3cert" == "1" ];then
                        $WAPISET cacert /jffs/ca.cer 2>&1 > /dev/null
                        $WAPISET authmode 3 2>&1 > /dev/null
                    fi
                fi
                $KILLALL wapid 2>&1 > /dev/null
                /usr/sbin/mydaemon $WAPID  -c /usr/sbin/wlan.conf 2>&1 > /dev/null
            fi
        fi
    done
}

#
# sys_macfilter_startsetup
#
sys_macfilter_startsetup()
{
    for vapnum in `seq $NUM_ap`
    do
        local xml_config=`config_read /config/network/vap$vapnum`
        local xml_enable=`config_getoption "$xml_config" create`
        local xml_wlanmode=`config_getoption "$xml_config" wlanmode`

        if [ "$xml_wlanmode" == "ap" ]||[ "$xml_wlanmode" == "root" ]&&[ "$xml_enable" == "yes"  ]; then
            local xml_if=`config_getoption "$xml_config" if`
            local xml_macfilter=`config_getoption "$xml_config" macfilter`
            local xml_filtermode=`config_getoption "$xml_config" filtermode`
            local xml_macount=`config_getoption "$xml_config" mac_count`
            local  xml_mac1=`config_getoption "$xml_config" mac1`
            local  xml_mac2=`config_getoption "$xml_config" mac2`
            local  xml_mac3=`config_getoption "$xml_config" mac3`
            local  xml_mac4=`config_getoption "$xml_config" mac4`
            local  xml_mac5=`config_getoption "$xml_config" mac5`
            local  xml_mac6=`config_getoption "$xml_config" mac6`
            local  xml_mac7=`config_getoption "$xml_config" mac7`
            local  xml_mac8=`config_getoption "$xml_config" mac8`
            local  xml_mac9=`config_getoption "$xml_config" mac9`
            local  xml_mac10=`config_getoption "$xml_config" mac10`
            local  xml_mac11=`config_getoption "$xml_config" mac11`
            local  xml_mac12=`config_getoption "$xml_config" mac12`
            local  xml_mac13=`config_getoption "$xml_config" mac13`
            local  xml_mac14=`config_getoption "$xml_config" mac14`
            local  xml_mac15=`config_getoption "$xml_config" mac15`
            local  xml_mac16=`config_getoption "$xml_config" mac16`
            local  xml_mac17=`config_getoption "$xml_config" mac17`
            local  xml_mac18=`config_getoption "$xml_config" mac18`
            local  xml_mac19=`config_getoption "$xml_config" mac19`
            local  xml_mac20=`config_getoption "$xml_config" mac20`

            $IWPRIV "$xml_if" maccmd 3 > /dev/null 2>&1
            if [ "$xml_macfilter" == "yes" ]; then
                if [ "$xml_filtermode" == "permit" ]; then
                    $IWPRIV "$xml_if" maccmd 1 > /dev/null 2>&1
                elif [ "$xml_filtermode" == "prevent" ]; then
                    $IWPRIV "$xml_if" maccmd 2 > /dev/null 2>&1
                fi
                for mac in `seq $xml_macount`
                do
                    eval macaddr="$"xml_mac$mac
                    echo "$macaddr"|grep "[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}"
                    if [ $? == 0 ]; then
                        $IWPRIV "$xml_if" addmac $macaddr > /dev/null 2>&1
                    fi
                done
            elif [ "$xml_macfilter" == "no" ]; then
                $IWPRIV "$xml_if" maccmd 0 > /dev/null 2>&1
            fi

        fi
    done
}

sys_advanceset_startsetup()
{

    for vapnum in `seq $NUM_ap`
    do
        local xml_config=`config_read /config/network/vap$vapnum`
        local xml_enable=`config_getoption "$xml_config" create`
        local xml_wlanmode=`config_getoption "$xml_config" wlanmode`


        if [ "$xml_wlanmode" == "ap" ]||[ "$xml_wlanmode" == "root" ]&&[ "$xml_enable" == "yes"  ]; then
            local xml_if=`config_getoption "$xml_config" if`
            local xml_wlan_dtim=`config_getoption "$xml_config" dtim`
            local xml_wlan_frag=`config_getoption "$xml_config" frag`
            local xml_forward2=`config_getoption "$xml_config" forward2`

            local xml_wlan_preamble=`config_getoption "$xml_config" preamble`
            local xml_wlan_rts=`config_getoption "$xml_config" rts`
            local xml_isolation=`config_getoption "$xml_config" isolation`
            local xml_bcisolation=`config_getoption "$xml_config" bcisolation`
            local xml_count=`config_getoption "$xml_config" count`
            #xml_count=`expr $xml_count + 1`
            local xml_puren=`config_getoption "$xml_config" puren`
            
            local xml_wlan_wifi=`config_getoption "$xml_config" wifi`
            local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
            local xml_wifi_txpower=`config_getoption "$xml_wifi" txpoff`
            local xml_wlan_diversity=`config_getoption "$xml_wifi" diversity`
            local xml_wlan_txantenna=`config_getoption "$xml_wifi" txantenna`
            local xml_wlan_rxantenna=`config_getoption "$xml_wifi" rxantenna`
            local xml_wifi_cwwmode=`config_getoption "$xml_wifi" cwwmode`
            local xml_wifi_extoffset=`config_getoption "$xml_wifi" extoffset`

            local xml_config_vlan=`config_read /config/network/vap$vapnum/vlan`
            local xml_wlan_vlan_enable=`config_getoption "$xml_config_vlan" enable`
            local xml_wlan_vlan_vid=`config_getoption "$xml_config_vlan" vid`  

            #$IFCONFIG "$xml_if" down > /dev/null 2>&1
            $SYSCTL -w net.$xml_if.maxaid=$xml_count > /dev/null 2>&1
            $IWPRIV $xml_if ap_bridge $xml_isolation > /dev/null 2>&1
            $IWPRIV $xml_if switch $xml_bcisolation > /dev/null 2>&1
            $IWPRIV $xml_if puren $xml_puren > /dev/null 2>&1

            #$IFCONFIG "$xml_if" up > /dev/null 2>&1

            $IWPRIV "$xml_if" dtim_period "$xml_wlan_dtim" > /dev/null 2>&1
            $IWCONFIG "$xml_if" frag "$xml_wlan_frag" > /dev/null 2>&1

            if [ "$xml_wlan_preamble" == "Short" ]; then
                $IWPRIV "$xml_if" shpreamble 1 > /dev/null 2>&1
            else
                $IWPRIV "$xml_if" shpreamble 0 > /dev/null 2>&1
            fi

            #txpower set after vap up
            $IFCONFIG "$xml_if" up > /dev/null 2>&1

            #      eval cur_power="$"DEVPOWER_$xml_wlan_wifi
            #      eval cur_poweroff="$"DEVPOWEROFF_$xml_wlan_wifi

            #modify 20090717 start
            #       [ $cur_power -lt $xml_wifi_txpower ]&& {
            #      		xml_wifi_txpower=$cur_power
            #     		xml_wifi=`config_setoption "$xml_wifi" txpower "$xml_wifi_txpower"`
            #    		config_write /config/network/$xml_wlan_wifi "$xml_wifi"
            #   }

            $SETTXPOWER "$xml_if" "$xml_wifi_txpower" > /dev/null 2>&1



            $IWCONFIG "$xml_if" rts "$xml_wlan_rts" > /dev/null 2>&1
            if [ "$xml_forward2" == "yes" ]; then
                if [ "$xml_wlan_vlan_enable" == "1" ]; then
                    $BRCTL delif $BRLAN "$xml_if" > /dev/null 2>&1
                    $BRCTL addif "br$xml_wlan_vlan_vid" "$xml_if" > /dev/null 2>&1	 
                elif [ "$xml_wlan_vlan_enable" == "0" ]; then	    
                    $BRCTL delif "br$xml_wlan_vlan_vid" "$xml_if" > /dev/null 2>&1
                    $BRCTL addif $BRLAN "$xml_if" > /dev/null 2>&1
                fi
            fi

            #if [ "$xml_wlan_diversity" == "0" ]; then
            echo "$DEVICE_MODEL" | grep -q 'N'
            if [ $? -eq 0 ];then
                echo "11n"

                $IWPRIV "$xml_if" cwmmode "$xml_wifi_cwwmode" > /dev/null 2>&1
                $IWPRIV "$xml_if" extoffset "$xml_wifi_extoffset" > /dev/null 2>&1

            else
                $SETANTENNA $xml_wlan_wifi diversity $xml_wlan_diversity 2>/dev/null
                #echo $xml_wlan_diversity > /proc/sys/dev/wifi0/diversity 
                #fi

                $SETANTENNA $xml_wlan_wifi tx $xml_wlan_txantenna 2>/dev/null
                $SETANTENNA $xml_wlan_wifi rx $xml_wlan_rxantenna 2>/dev/null
                #echo $xml_wlan_txantenna > /proc/sys/dev/wifi0/txantenna
                #echo $xml_wlan_rxantenna > /proc/sys/dev/wifi0/rxantenna  
            fi
        fi
    done

    vap_allup
}

#
#vap_wifi_alldown
#
vap_wifi_alldown()
{
    local xml_vap=`config_read /config/network`
    local vap_cnum=`config_getoption "$xml_vap" vapnum`
    local	xml_wlan_cvap=`config_read /config/network/vap$vap_cnum`
    local	xml_wlan_cvap_wifi=`config_getoption "$xml_wlan_cvap" wifi`
    local	xml_wlan_cvap_enable=`config_getoption "$xml_wlan_cvap" enable`
    local vapname="vap$vap_cnum"

    [ "$xml_wlan_cvap_enable" == "yes" ]&&{
    for vapnum in `seq $NUM_ap`
    do 
        ath_num=`expr $vapnum - 1`
        local	xml_wlan_config_vap=`config_read /config/network/vap$vapnum`
        local	xml_wlan_vap_create=`config_getoption "$xml_wlan_config_vap" create`
        local	xml_wlan_vap_enable=`config_getoption "$xml_wlan_config_vap" enable`
        local	xml_wlan_vap_wifi=`config_getoption "$xml_wlan_config_vap" wifi`
        if [ "$xml_wlan_vap_create" == "yes" ];then
            if [ "$xml_wlan_vap_enable" == "yes"  ];then
                if [ "$xml_wlan_vap_wifi" == "$xml_wlan_cvap_wifi" ];then
                    $IFCONFIG ath$ath_num down > /dev/null 2>&1
                fi
            fi
        fi
    done
}

}

#
#vap_wifi_allup
#
vap_wifi_allup()
{
    local xml_vap=`config_read /config/network`
    local vap_cnum=`config_getoption "$xml_vap" vapnum`
    local	xml_wlan_cvap=`config_read /config/network/vap$vap_cnum`
    local	xml_wlan_cvap_wifi=`config_getoption "$xml_wlan_cvap" wifi`
    local	xml_wlan_cvap_enable=`config_getoption "$xml_wlan_cvap" enable`
    local vapname="vap$vap_cnum"

    [ "$xml_wlan_cvap_enable" == "yes" ]&&{
    for vapnum in `seq $NUM_ap`
    do 
        ath_num=`expr $vapnum - 1`
        local	xml_wlan_config_vap=`config_read /config/network/vap$vapnum`
        local	xml_wlan_vap_create=`config_getoption "$xml_wlan_config_vap" create`
        local	xml_wlan_vap_enable=`config_getoption "$xml_wlan_config_vap" enable`
        local	xml_wlan_vap_wifi=`config_getoption "$xml_wlan_config_vap" wifi`
        if [ "$xml_wlan_vap_create" == "yes" ];then
            if [ "$xml_wlan_vap_enable" == "yes"  ];then
                if [ "$xml_wlan_vap_wifi" == "$xml_wlan_cvap_wifi" ];then
                    sleep 2 > /dev/null 2>&1
                    $IFCONFIG ath$ath_num up > /dev/null 2>&1

                fi
            fi
        fi
    done
}

}

#
#vap_alldown
#
vap_alldown()
{
    for vapnum in `seq $NUM_ap`
    do 
        ath_num=`expr $vapnum - 1`
        local	xml_wlan_config_vap=`config_read /config/network/vap$vapnum`
        local	xml_wlan_vap_create=`config_getoption "$xml_wlan_config_vap" create`
        local	xml_wlan_vap_enable=`config_getoption "$xml_wlan_config_vap" enable`
        if [ "$xml_wlan_vap_create" == "yes" ];then
            if [ "$xml_wlan_vap_enable" == "yes"  ];then
                echo "all down" >/tmp/down
                $IFCONFIG ath$ath_num down > /dev/null 2>&1
            fi
        fi
    done


}

#
#vap_allup
#
vap_allup()
{

    for vapnum in `seq $NUM_ap`
    do 
        ath_num=`expr $vapnum - 1`
        local	xml_wlan_config_vap=`config_read /config/network/vap$vapnum`
        local	xml_wlan_vap_create=`config_getoption "$xml_wlan_config_vap" create`
        local	xml_wlan_vap_enable=`config_getoption "$xml_wlan_config_vap" enable`
        if [ "$xml_wlan_vap_create" == "yes" ];then
            if [ "$xml_wlan_vap_enable" == "yes"  ];then
                sleep 2 > /dev/null 2>&1
                $IFCONFIG ath$ath_num up > /dev/null 2>&1
            fi
        fi
    done

}




#
#sys_vap_setup
#
sys_vap_setup()
{
    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum `

    local xml_wlan=`config_read /config/network/vap$vapnum`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_enable=`config_getoption "$xml_wlan" enable`
    local xml_ssid=`config_getoption "$xml_wlan" ssid`
    local xml_broadcast=`config_getoption "$xml_wlan" broadcast`
    local xml_wlanmode=`config_getoption "$xml_wlan" wlanmode`
    local xml_wlan_rate=`config_getoption "$xml_wlan" rate`
    local xml_wlan_rateb=`config_getoption "$xml_wlan" rateb`
    local xml_wlan_ratea=`config_getoption "$xml_wlan" ratea`
    local xml_wlan_rate_n_ofdm=`config_getoption "$xml_wlan" rate_n_ofdm`
    local xml_wlan_rate_n_ht20ds=`config_getoption "$xml_wlan" rate_n_ht20ds`
    local xml_wlan_rate_n_ht40ds=`config_getoption "$xml_wlan" rate_n_ht40ds`
    local xml_wlan_beacon=`config_getoption "$xml_wlan" beacon`

    local xml_wlan_wifi=`config_getoption "$xml_wlan" wifi`
    eval wifi_devmode="$"DEVMODE_$xml_wlan_wifi
    local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
    local xml_wifi_channel=`config_getoption "$xml_wifi" channel`
    local xml_wifi_channel1=`config_getoption "$xml_wifi" channel1`
    local xml_wifi_channela=`config_getoption "$xml_wifi" channela`
    #	local xml_wlan_autochannel=`config_getoption "$xml_wifi" autochannel`
    local xml_mode=`config_getoption "$xml_wifi" mode`
    local xml_modea=`config_getoption "$xml_wifi" modea`
    local xml_moden=`config_getoption "$xml_wifi" moden`
    local xml_cwwmode=`config_getoption "$xml_wifi" cwwmode`
    local xml_extoffset=`config_getoption "$xml_wifi" extoffset`
    local xml_shortgi=`config_getoption "$xml_wifi" shortgi`
    local config_country_code=`config_read /config/network/country`
    local xml_region=`config_getoption "$config_country_code" region`

    local xml_config_vlan=`config_read /config/network/vap$vapnum/vlan`
    local xml_wlan_vlan_enable=`config_getoption "$xml_config_vlan" enable`
    local xml_wlan_vlan_vid=`config_getoption "$xml_config_vlan" vid`

    if [ "$xml_enable" == "yes" ]; then
        [ -n "$xml_ssid" ] && $IWCONFIG "$xml_if" essid "$xml_ssid" > /dev/null 2>&1
        $IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
        $IWPRIV "$xml_if" pureg 0 > /dev/null 2>&1
        if [ "$wifi_devmode" == "bg" ];then
            if [ "$xml_mode" == "11b" ]; then
                $IWPRIV "$xml_if" mode 11b > /dev/null 2>&1
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
            #	            echo "$xml_wlan_autochannel" > /proc/sys/dev/$xml_wlan_wifi/nonoverlapping
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
        elif [ "$wifi_devmode" == "abg" ];then
            if [ "$xml_modea" == "11b" ]; then
                $IWPRIV "$xml_if" mode 11b > /dev/null 2>&1
            elif [ "$xml_modea" == "bg" ]; then
                $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
            elif [ "$xml_modea" == "11g" ];then
                $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
                $IWPRIV "$xml_if" pureg 1 > /dev/null 2>&1
            else
                $IWPRIV "$xml_if" mode 11a > /dev/null 2>&1
            fi
            if [ "$xml_modea" != "11a" ];then
                if [ "$xml_region" == "Asia" -o "$xml_region" == "Europe" ]; then       
                    $IWCONFIG "$xml_if" channel "$xml_wifi_channel1" > /dev/null 2>&1
                elif [ "$xml_region" == "America" ]; then    
                    $IWCONFIG "$xml_if" channel "$xml_wifi_channel" > /dev/null 2>&1
                else     
                    $IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
                fi
            else
                $IWCONFIG "$xml_if" channel "$xml_wifi_channela" > /dev/null 2>&1
            fi
            #            echo "$xml_wlan_autochannel" > /proc/sys/dev/$xml_wlan_wifi/nonoverlapping
            if [ "$xml_modea" == "11b"  ];then
                if [ "$xml_wlan_rateb" == "auto" ]; then
                    $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                else
                    $IWCONFIG "$xml_if" rate  "$xml_wlan_rateb" > /dev/null 2>&1
                fi
            elif [ "$xml_modea" == "11g" -o "$xml_modea" == "bg" ];then
                if [ "$xml_wlan_rate" == "auto" ]; then
                    $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                else
                    $IWCONFIG "$xml_if" rate  "$xml_wlan_rate" > /dev/null 2>&1
                fi
            else
                if [ "$xml_wlan_ratea" == "auto" ]; then
                    $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                else
                    $IWCONFIG "$xml_if" rate  "$xml_wlan_ratea" > /dev/null 2>&1
                fi
            fi
        else
            if [ "$xml_moden" == "11b" ]; then
                $IWPRIV "$xml_if" mode 11b > /dev/null 2>&1
            elif [ "$xml_moden" == "bg" ]; then
                $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
                $IWPRIV "$xml_if" countryie 1  > /dev/null 2>&1
                $IWPRIV "$xml_if" doth 0  > /dev/null 2>&1
            elif [ "$xml_moden" == "11g" ];then
                $IWPRIV "$xml_if" mode 11g > /dev/null 2>&1
                $IWPRIV "$xml_if" pureg 1 > /dev/null 2>&1
                $IWPRIV "$xml_if" countryie 1  > /dev/null 2>&1
                $IWPRIV "$xml_if" doth 0  > /dev/null 2>&1
            elif [ "$xml_moden" == "11a" ];then
                $IWPRIV "$xml_if" mode 11a > /dev/null 2>&1
                $IWPRIV "$xml_if" countryie 0  > /dev/null 2>&1
                $IWPRIV "$xml_if" doth 1  > /dev/null 2>&1
            elif [ "$xml_moden" == "11na" ]; then
                $IWPRIV "$xml_if" bgscan 0  > /dev/null 2>&1
                $IFCONFIG "$xml_if" txqueuelen 1000  > /dev/null 2>&1
                $IFCONFIG "$xml_wlan_wifi" txqueuelen 1000  > /dev/null 2>&1
                $IWPRIV "$xml_if" shortgi "$xml_shortgi"  > /dev/null 2>&1

                if [ "$xml_cwwmode" == "0" ];then
                    $IWPRIV "$xml_if" mode 11NAHT20 > /dev/null 2>&1
                fi
                if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "0" ];then
                    $IWPRIV "$xml_if" mode 11NAHT20 > /dev/null 2>&1
                fi
                if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "1" ];then
                    $IWPRIV "$xml_if" mode 11NAHT40PLUS > /dev/null 2>&1
                fi
                if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "-1" ];then
                    $IWPRIV "$xml_if" mode 11NAHT40MINUS > /dev/null 2>&1
                fi

                #$IWPRIV "$xml_wlan_wifi" ForBiasAuto 1  > /dev/null 2>&1
                $IWPRIV "$xml_if" extoffset "$xml_extoffset" > /dev/null 2>&1
                $IWPRIV "$xml_if" cwmmode "$xml_cwwmode" > /dev/null 2>&1
                $IWPRIV "$xml_wlan_wifi" AMPDU 1  > /dev/null 2>&1
                $IWPRIV "$xml_wlan_wifi" AMPDUFrames 32  > /dev/null 2>&1
                $IWPRIV "$xml_wlan_wifi" AMPDULim 50000  > /dev/null 2>&1
                $IWPRIV "$xml_if" pureg 0 > /dev/null 2>&1
                $IWPRIV "$xml_if" puren 0 > /dev/null 2>&1
                eval txc="$"DEVTXC_$xml_wlan_wifi
                $IWPRIV "$xml_wlan_wifi" txchainmask $txc  > /dev/null 2>&1
                $IWPRIV "$xml_wlan_wifi" rxchainmask 3  > /dev/null 2>&1
                echo 1 > /proc/sys/dev/ath/htdupieenable  > /dev/null 2>&1
            elif [ "$xml_moden" == "11ng" ]; then
                $IWPRIV "$xml_if" bgscan 0  > /dev/null 2>&1
                $IFCONFIG "$xml_if" txqueuelen 1000  > /dev/null 2>&1
                $IFCONFIG "$xml_wlan_wifi" txqueuelen 1000  > /dev/null 2>&1
                $IWPRIV "$xml_if" shortgi "$xml_shortgi"  > /dev/null 2>&1

                if [ "$xml_cwwmode" == "0" ];then
                    $IWPRIV "$xml_if" mode 11NGHT20 > /dev/null 2>&1
                fi
                if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "0" ];then
                    $IWPRIV "$xml_if" mode 11NGHT20 > /dev/null 2>&1
                fi
                if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "1" ];then
                    $IWPRIV "$xml_if" mode 11NGHT40PLUS > /dev/null 2>&1
                fi
                if [ "$xml_cwwmode" != "0" -a "$xml_extoffset" == "-1" ];then
                    $IWPRIV "$xml_if" mode 11NGHT40MINUS > /dev/null 2>&1
                fi

                $IWPRIV "$xml_wlan_wifi" ForBiasAuto 1  > /dev/null 2>&1
                $IWPRIV "$xml_if" extoffset "$xml_extoffset" > /dev/null 2>&1
                $IWPRIV "$xml_if" cwmmode "$xml_cwwmode" > /dev/null 2>&1
                $IWPRIV "$xml_wlan_wifi" AMPDU 1  > /dev/null 2>&1
                $IWPRIV "$xml_wlan_wifi" AMPDUFrames 32  > /dev/null 2>&1
                $IWPRIV "$xml_wlan_wifi" AMPDULim 50000  > /dev/null 2>&1
                $IWPRIV "$xml_if" pureg 0 > /dev/null 2>&1
                $IWPRIV "$xml_if" puren 0 > /dev/null 2>&1
                eval txc="$"DEVTXC_$xml_wlan_wifi
                $IWPRIV "$xml_wlan_wifi" txchainmask $txc  > /dev/null 2>&1
                $IWPRIV "$xml_wlan_wifi" rxchainmask 3  > /dev/null 2>&1
                echo 1 > /proc/sys/dev/ath/htdupieenable  > /dev/null 2>&1
            fi

            if [ "$xml_moden" == "11a" -o "$xml_moden" == "11na" ];then
                $IWCONFIG "$xml_if" channel "$xml_wifi_channela" > /dev/null 2>&1
            else
                if [ "$xml_region" == "Asia" -o "$xml_region" == "Europe" ]; then       
                    $IWCONFIG "$xml_if" channel "$xml_wifi_channel1" > /dev/null 2>&1
                elif [ "$xml_region" == "America" ]; then    
                    $IWCONFIG "$xml_if" channel "$xml_wifi_channel" > /dev/null 2>&1
                else     
                    $IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
                fi
            fi
            if [ "$xml_moden" == "11ng" -o "$xml_moden" == "11na" ];then    
                if [ "$xml_cwwmode" == "0" ];then
                    if [ "$xml_wlan_rate_n_ht20ds" == "auto" ]; then
                        $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                    else
                        $IWCONFIG "$xml_if" rate  "$xml_wlan_rate_n_ht20ds" > /dev/null 2>&1
                    fi

                elif [ "$xml_cwwmode" == "1" ];then

                    if [ "$xml_wlan_rate_n_ht40ds" == "auto" ]; then
                        $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                    else
                        $IWCONFIG "$xml_if" rate  "$xml_wlan_rate_n_ht40ds" > /dev/null 2>&1

                    fi

                elif [ "$xml_cwwmode" == "2" ];then

                    if [ "$xml_wlan_rate_n_ht40ds" == "auto" -a "$xml_cwwmode" == "2" ]; then
                        $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                    else
                        $IWCONFIG "$xml_if" rate  "$xml_wlan_rate_n_ht40ds" > /dev/null 2>&1
                    fi
                fi

            elif [ "$xml_moden" == "11g" -o "$xml_moden" == "bg" ];then
                if [ "$xml_wlan_rate" == "auto" ]; then
                    $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                else
                    $IWCONFIG "$xml_if" rate  "$xml_wlan_rate" > /dev/null 2>&1
                fi
            elif [ "$xml_moden" == "11b" ];then
                if [ "$xml_wlan_rateb" == "auto" ]; then
                    $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                else
                    $IWCONFIG "$xml_if" rate  "$xml_wlan_rateb" > /dev/null 2>&1
                fi
            else
                if [ "$xml_wlan_ratea" == "auto" ]; then
                    $IWCONFIG "$xml_if" rate  auto > /dev/null 2>&1
                else
                    $IWCONFIG "$xml_if" rate  "$xml_wlan_ratea" > /dev/null 2>&1
                fi
            fi
        fi

        $IWPRIV "$xml_if" bintval "$xml_wlan_beacon" > /dev/null 2>&1



        if [ "$xml_broadcast" == "yes" ]; then
            $IWPRIV "$xml_if" hide_ssid 0 > /dev/null 2>&1
        elif [ "$xml_broadcast" == "no" ]; then
            $IWPRIV "$xml_if" hide_ssid 1 > /dev/null 2>&1
        fi

        sys_security_setup
        sys_macfilter_setup
        sys_advanceset_setup
    elif [ "$xml_enable" == "no" ]; then
        $IFCONFIG "$xml_if" down > /dev/null 2>&1
    fi



}

#
#vap_cspecific_down
#
vap_cspecific_down()
{
    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum ` 
    local xml_wlan=`config_read /config/network/vap$vapnum`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_enable=`config_getoption "$xml_wlan" enable`
    [ "$xml_enable" == "yes" ]&&$IFCONFIG $xml_if down > /dev/null 2>&1
}

#
#vap_cspecific_up
#
vap_cspecific_up()
{
    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum ` 
    local xml_wlan=`config_read /config/network/vap$vapnum`
    local xml_if=`config_getoption "$xml_wlan" if`
    local xml_enable=`config_getoption "$xml_wlan" enable`
    [ "$xml_enable" == "yes" ]&&$IFCONFIG $xml_if up > /dev/null 2>&1

}

#
# sys_stp_setup
#
sys_stp_setup()
{

    local xml_lan=`config_read /config/network/lan`
    local xml_if=`config_getoption "$xml_lan" if`

    xml_config=`config_read /config/network/advanset`
    xml_portprio=`config_getoption "$xml_config" portprio`
    xml_setfd=`config_getoption "$xml_config" setfd`
    xml_sethello=`config_getoption "$xml_config" sethello`
    xml_setmaxage=`config_getoption "$xml_config" maxage`
    xml_stpenable=`config_getoption "$xml_config" stpenable`

    $BRCTL setbridgeprio "$xml_if" "$xml_portprio" > /dev/null 2>&1
    $BRCTL setfd "$xml_if"  "$xml_setfd" > /dev/null 2>&1
    $BRCTL setmaxage "$xml_if" "$xml_setmaxage" > /dev/null 2>&1 
    $BRCTL sethello  "$xml_if" "$xml_sethello" > /dev/null 2>&1
    if [ "$xml_stpenable" == "yes" ]; then 
        $BRCTL stp "$xml_if" on > /dev/null 2>&1
    elif [ "$xml_stpenable" == "no" ]; then
        $BRCTL stp "$xml_if" off > /dev/null 2>&1
    fi
}

#
#sys_wds_setup
#
sys_wds_setup()
{  
    local xml_config=`config_read /config/network/wds`
    local xml_enable=`config_getoption "$xml_config" wdsenable`
    local xml_wds_if=`config_getoption "$xml_config" wdsif`
    local xml_wds_mac=`config_getoption "$xml_config" wdsmac`

    local tmp_num=`echo $xml_wds_if|awk -F'p' '{print $2}'`
    tmp_num=`expr $tmp_num - 1 2>/dev/null`
    if [ "$xml_enable" == "yes" ]; then
        for wds_num in `seq 8`
        do
            tmp_wds_num=`expr $wds_num - 1 2>/dev/null`
            $WLANCONFIG "ath${tmp_num}.wds${tmp_wds_num}" destroy > /dev/null 2>&1
        done
        for wds_num in `seq 8`
        do
            eval "mac_tmp=\`echo \"${xml_wds_mac}\"|awk -F';' '{print \$${wds_num}}'\`"
            if [ "$mac_tmp" != "" ]; then
                tmp_wds_num=`expr $wds_num - 1 2>/dev/null`
                $IWPRIV ath${tmp_num} wds_add ${mac_tmp} > /dev/null 2>&1
                $BRCTL addif $BRLAN "ath${tmp_num}.wds${tmp_wds_num}" > /dev/null 2>&1
                $IFCONFIG "ath${tmp_num}.wds${tmp_wds_num}" up > /dev/null 2>&1
            fi
        done
    elif [ "$xml_enable" == "no" ]; then
        for wds_num in `seq 8`
        do
            tmp_wds_num=`expr $wds_num - 1 2>/dev/null`
            $IFCONFIG "ath${tmp_num}.wds${tmp_wds_num}" down > /dev/null 2>&1
            $WLANCONFIG "ath${tmp_num}.wds${tmp_wds_num}" destroy > /dev/null 2>&1
        done
    fi

}

#
#sys_vlan_startsetup
#
sys_vlan_startsetup()
{
    local eth0_mtu="1500"
    local eth1_mtu="1500"

    for vlan in `seq 8`
    do   
        local  xml_config=`config_read /config/network/vlan$vlan`
        local  xml_vlan_create=`config_getoption "$xml_config" create`
        local  xml_vlan_vid=`config_getoption "$xml_config" vid`
        local  xml_vlan_vlanname=`config_getoption "$xml_config" vlanname`
        local  xml_vlan_ipaddr=`config_getoption "$xml_config" ipaddr`
        local  xml_vlan_mask=`config_getoption "$xml_config" netmask`
        local  xml_vlan_ifeth=`config_getoption "$xml_config" ifeth`
        local  xml_vlan_ifeth1=`config_getoption "$xml_config" ifeth1`

        if [ "$xml_vlan_create" == "yes" ]; then
            $BRCTL addbr br$xml_vlan_vid > /dev/null 2>&1
            $IFCONFIG br$xml_vlan_vid $xml_vlan_ipaddr netmask $xml_vlan_mask > /dev/null 2>&1
            if [ "$xml_vlan_ifeth" == "yes" ]; then
                eth0_mtu="1600"
                $VCONFIG add eth0 $xml_vlan_vid > /dev/null 2>&1

                $IFCONFIG eth0.$xml_vlan_vid up > /dev/null 2>&1
                $BRCTL addif br$xml_vlan_vid eth0.$xml_vlan_vid > /dev/null 2>&1
            fi
            if [ "$xml_vlan_ifeth1" == "yes" ]; then
                eth1_mtu="1600" 
                $VCONFIG add eth1 $xml_vlan_vid > /dev/null 2>&1
                $IFCONFIG eth1.$xml_vlan_vid up > /dev/null 2>&1
                $BRCTL addif br$xml_vlan_vid eth1.$xml_vlan_vid > /dev/null 2>&1
            fi  
            $IFCONFIG br$xml_vlan_vid up > /dev/null 2>&1
        fi
    done
    [ "$eth0_mtu" != "1500" ] && $IFCONFIG eth0 mtu $eth0_mtu > /dev/null 2>&1
    [ "$eth1_mtu" != "1500" ] && $IFCONFIG eth1 mtu $eth1_mtu > /dev/null 2>&1

}


#
#sys_vlan_setup
#
sys_vlan_setup()
{
    local eth0_mtu="1500"
    local eth1_mtu="1500"

    for vlan in `seq 8`
    do   
        local  xml_config=`config_read /config/network/vlan$vlan`
        local  xml_vlan_create=`config_getoption "$xml_config" create`
        local  xml_vlan_vid=`config_getoption "$xml_config" vid`
        local  xml_vlan_vlanname=`config_getoption "$xml_config" vlanname`
        local  xml_vlan_ipaddr=`config_getoption "$xml_config" ipaddr`
        local  xml_vlan_mask=`config_getoption "$xml_config" netmask`
        local  xml_vlan_ifeth=`config_getoption "$xml_config" ifeth`
        local  xml_vlan_ifeth1=`config_getoption "$xml_config" ifeth1`
        if [ "$xml_vlan_create" == "yes" ]; then
            $BRCTL addbr br$xml_vlan_vid > /dev/null 2>&1
            $IFCONFIG br$xml_vlan_vid $xml_vlan_ipaddr netmask $xml_vlan_mask > /dev/null 2>&1
            if [ "$xml_vlan_ifeth" == "yes" ]; then
                eth0_mtu="1600"
                $VCONFIG add eth0 $xml_vlan_vid > /dev/null 2>&1
                $IFCONFIG eth0.$xml_vlan_vid up > /dev/null 2>&1
                $BRCTL addif br$xml_vlan_vid eth0.$xml_vlan_vid > /dev/null 2>&1
            else
                $BRCTL  delif br$xml_vlan_vid eth0.$xml_vlan_vid > /dev/null 2>&1
                $VCONFIG rem eth0.$xml_vlan_vid > /dev/null 2>&1
            fi
            if [ "$xml_vlan_ifeth1" == "yes" ]; then
                eth1_mtu="1600"
                $VCONFIG add eth1 $xml_vlan_vid > /dev/null 2>&1
                $IFCONFIG eth1.$xml_vlan_vid up > /dev/null 2>&1
                $BRCTL addif br$xml_vlan_vid eth1.$xml_vlan_vid > /dev/null 2>&1
            else
                $BRCTL  delif br$xml_vlan_vid eth1.$xml_vlan_vid > /dev/null 2>&1
                $VCONFIG rem eth1.$xml_vlan_vid > /dev/null 2>&1
            fi
            $IFCONFIG br$xml_vlan_vid up > /dev/null 2>&1
        else
            $BRCTL  delif br$xml_vlan_vid eth0.$xml_vlan_vid > /dev/null 2>&1
            $VCONFIG rem eth0.$xml_vlan_vid > /dev/null 2>&1
            $BRCTL  delif br$xml_vlan_vid eth1.$xml_vlan_vid > /dev/null 2>&1
            $VCONFIG rem eth1.$xml_vlan_vid > /dev/null 2>&1
            $IFCONFIG br$xml_vlan_vid down > /dev/null 2>&1
            $BRCTL  delbr br$xml_vlan_vid > /dev/null 2>&1
        fi    
    done
    $IFCONFIG eth0 mtu $eth0_mtu > /dev/null 2>&1
    $IFCONFIG eth1 mtu $eth1_mtu > /dev/null 2>&1

}

#
# sys_macfilter_setup
#
sys_macfilter_setup()
{
    xml_vap=`config_read /config/network`
    vapnum=`config_getoption "$xml_vap" vapnum `


    local xml_config=`config_read /config/network/vap$vapnum`
    local xml_enable=`config_getoption "$xml_config" enable`
    local xml_if=`config_getoption "$xml_config" if`
    local xml_macfilter=`config_getoption "$xml_config" macfilter`
    local xml_filtermode=`config_getoption "$xml_config" filtermode`
    local xml_macount=`config_getoption "$xml_config" mac_count`
    local  xml_mac1=`config_getoption "$xml_config" mac1`
    local  xml_mac2=`config_getoption "$xml_config" mac2`
    local  xml_mac3=`config_getoption "$xml_config" mac3`
    local  xml_mac4=`config_getoption "$xml_config" mac4`
    local  xml_mac5=`config_getoption "$xml_config" mac5`
    local  xml_mac6=`config_getoption "$xml_config" mac6`
    local  xml_mac7=`config_getoption "$xml_config" mac7`
    local  xml_mac8=`config_getoption "$xml_config" mac8`
    local  xml_mac9=`config_getoption "$xml_config" mac9`
    local  xml_mac10=`config_getoption "$xml_config" mac10`
    local  xml_mac11=`config_getoption "$xml_config" mac11`
    local  xml_mac12=`config_getoption "$xml_config" mac12`
    local  xml_mac13=`config_getoption "$xml_config" mac13`
    local  xml_mac14=`config_getoption "$xml_config" mac14`
    local  xml_mac15=`config_getoption "$xml_config" mac15`
    local  xml_mac16=`config_getoption "$xml_config" mac16`
    local  xml_mac17=`config_getoption "$xml_config" mac17`
    local  xml_mac18=`config_getoption "$xml_config" mac18`
    local  xml_mac19=`config_getoption "$xml_config" mac19`
    local  xml_mac20=`config_getoption "$xml_config" mac20`
    if [ "$xml_enable" == "yes" ]; then
        $IWPRIV "$xml_if" maccmd 3 > /dev/null 2>&1
        if [ "$xml_macfilter" == "yes" ]; then
            if [ "$xml_filtermode" == "permit" ]; then
                $IWPRIV "$xml_if" maccmd 1 > /dev/null 2>&1             
            elif [ "$xml_filtermode" == "prevent" ]; then
                $IWPRIV "$xml_if" maccmd 2 > /dev/null 2>&1
            fi
            for mac in `seq $xml_macount`
            do
                eval macaddr="$"xml_mac$mac
                echo "$macaddr"|grep "[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}" 
                if [ $? == 0 ]; then
                    $IWPRIV "$xml_if" addmac $macaddr > /dev/null 2>&1
                fi
            done
        elif [ "$xml_macfilter" == "no" ]; then
            $IWPRIV "$xml_if" maccmd 0 > /dev/null 2>&1    
        fi
    fi
}




#
# sys_get_interface_ipaddr(interface)
#
sys_get_interface_mac()
{
    local interface="$1"
    $IFCONFIG $interface | grep -o -i -e "hwaddr [0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}" | cut -d" " -f2
}

sys_get_interface_ipaddr()
{
    local interface="$1"
    $IFCONFIG $interface | grep -o -i -e "inet addr:[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}" | cut -d: -f2
}

sys_get_interface_netmask()
{
    local interface="$1"
    $IFCONFIG $interface | grep -o -i -e "mask:[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}" | cut -d: -f2
}

sys_get_interface_gateway()
{
    local interface="$1"
    local default_routes=`$ROUTE -n | grep $interface | grep -e "^0.0.0.0"`
    echo "$default_routes" | grep -o -i -e "[1-9][0-9]\{1,2\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}"
}

sys_get_nameserver1()
{
    local nameservers=`cat /etc/resolv.conf | grep nameserver | grep -n nameserver`
    echo "$nameservers" | grep 1:nameserver | grep -o -i -e "[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}"
}

sys_get_nameserver2()
{
    local nameservers=`cat /etc/resolv.conf | grep nameserver | grep -n nameserver`
    echo "$nameservers" | grep 2:nameserver | grep -o -i -e "[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}"
}
#
#sys_get_interface_essid(interface)
#i
sys_get_interface_essid()
{
    local wlan_interface="$1"
    $IWCONFIG $wlan_interface| grep -o -i -e "ESSID:\"[a-zA-Z0-9]\{1,30\}\""| cut -d\" -f2
}


sys_extend_setup()
{
    if [ $? != 0 ]; then
        false
    else
        true
    fi
}



sys_security_setup()
{
    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum `


    local xml_wlan=`config_read /config/network/vap$vapnum`
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

        local  xml_wapi_authtype=`config_getoption "$xml_wlan" wapibook`
        local  xml_wapi_sectype=`config_getoption "$xml_wlan" wapiauth`
        local  xml_wapi_enctype=`config_getoption "$xml_wlan" wapienc`
        local  xml_wapi_passphrase=`config_getoption "$xml_wlan" wapipass`
        local  xml_wlan_serverip=`config_getoption "$xml_wlan" wapisip`
        local  xml_wlan_serverport=`config_getoption "$xml_wlan" wapisport`

        local  xml_wapi_if3cert=`config_getoption "$xml_wlan" if3cert`

        echo "$DEVICE_MODEL" | grep -q 'N'
        if [ $? -eq 0 ];then
            $IWPRIV "$xml_if" cwmmode 1 > /dev/null 2>&1
            $IWPRIV "$xml_if" extoffset 1 > /dev/null 2>&1
        fi
        local a="a$vapnum"
        local b="b$vapnum"
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

        $IWCONFIG "$xml_if" key open off > /dev/null 2>&1
        $WAPISET wlan $xml_if  del 2>&1 > /dev/null
        $IWPRIV "$xml_if" authmode 1

        if [ "$xml_security" == "wep" ]; then
            $IWPRIV "$xml_if" authmode 1
            if [ "$xml_authmode" == "3" ]; then
                sys_wparadius_config
                $HOSTAPD -B /jffs/hostapd/b$vapnum 2>&1 >/dev/null &
            else
                if [ "$xml_authmode" == "2" ];then
                    $IWPRIV "$xml_if" authmode "$xml_authmode" > /dev/null 2>&1
                fi
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
            fi
        elif [ "$xml_security" == "wpa" ]; then
            if [ "$xml_wpamode" == "PSK" ]; then
                sys_wpapsk_config
                $HOSTAPD -B /jffs/hostapd/a$vapnum 2>&1 > /dev/null &
            else
                sys_wparadius_config
                $HOSTAPD -B /jffs/hostapd/b$vapnum 2>&1 > /dev/null &
            fi
        elif [ "$xml_security" == "wpa2" ]; then
            if [ "$xml_wpamode" == "PSK" ]; then
                sys_wpapsk_config
                $HOSTAPD -B /jffs/hostapd/a$vapnum 2>&1 > /dev/null &
            else
                sys_wparadius_config
                $HOSTAPD -B /jffs/hostapd/b$vapnum 2>&1 > /dev/null &
            fi
        elif [ "$xml_security" == "wapi" ]; then
            $WAPISET wlan $xml_if  add 2>&1 > /dev/null
            if [ "$xml_wapi_sectype" == "0" ]; then
                $WAPISET wlan $xml_if set mode open 2>&1 > /dev/null
            elif [ "$xml_wapi_sectype" == "7" ]; then
                $WAPISET wlan $xml_if set mode psk 2>&1 > /dev/null
                $WAPISET wlan $xml_if set psktype $xml_wapi_enctype 2>&1 > /dev/null
                $WAPISET wlan $xml_if set psk  $xml_wapi_passphrase 2>&1 > /dev/null
            elif [ "$xml_wapi_sectype" == "11" ]; then
                $WAPISET wlan $xml_if set mode cert 2>&1 > /dev/null
                $WAPISET certname /jffs/user.cer 2>&1 > /dev/null
                $WAPISET asuip  $xml_wlan_serverip  2>&1 > /dev/null
                $WAPISET authmode 2 2>&1 > /dev/null
                $CERT_UNITE /jffs/as_cert /jffs/ap_cert /jffs/user.cer 2>&1 > /dev/null
                if [ "$xml_wapi_if3cert" == "1" ];then
                    $WAPISET cacert /jffs/ca.cer 2>&1 > /dev/null
                    $WAPISET authmode 3 2>&1 > /dev/null
                fi
            fi
            $KILLALL wapid 2>&1 > /dev/null
            /usr/sbin/mydaemon $WAPID  -c /usr/sbin/wlan.conf 2>&1 > /dev/null 
            sleep 3
        fi
    fi
}


RogueAp_detection_conf()
{
    local RogueAPEnable="${FORM_RogueAPEnable:-no}"
    local xml_rogueapdetection_if="${FORM_vapname:-vap1}"
    xml_num=`echo $xml_rogueapdetection_if | awk -F'p' '{print $2}'`
    ath_num=`expr $xml_num - 1`

    local xml_rogueapdetection=`config_read /config/network/rogueapdetection`
    xml_rogueapdetection=`config_setoption "$xml_rogueapdetection" if "$xml_rogueapdetection_if"`
    xml_rogueapdetection=`config_setoption "$xml_rogueapdetection" enable "$RogueAPEnable"`
    config_write /config/network/rogueapdetection "$xml_rogueapdetection"

    local xml_rogueap=`config_read /config/network/$xml_rogueapdetection_if`
    xml_rogueap=`config_setoption "$xml_rogueap" enable "$RogueAPEnable"`
    config_write /config/network/$xml_rogueapdetection_if "$xml_rogueap"

    if [ "$RogueAPEnable" == "no" ];then
        $IFCONFIG ath$ath_num down > /dev/null 2>&1
    else
        sleep 2 > /dev/null 2>&1
        $IFCONFIG ath$ath_num up > /dev/null 2>&1	    
    fi
}


sys_advanceset_setup()
{

    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum`

    local xml_config=`config_read /config/network/vap$vapnum`
    local xml_if=`config_getoption "$xml_config" if`
    local xml_enable=`config_getoption "$xml_config" enable`
    local xml_forward2=`config_getoption "$xml_config" forward2`


    local xml_wlan_dtim=`config_getoption "$xml_config" dtim`

    local xml_wlan_frag=`config_getoption "$xml_config" frag`
    local xml_wlan_preamble=`config_getoption "$xml_config" preamble`
    local xml_wlan_rts=`config_getoption "$xml_config" rts`
    local xml_isolation=`config_getoption "$xml_config" isolation`
    local xml_bcisolation=`config_getoption "$xml_config" bcisolation`
    local xml_count=`config_getoption "$xml_config" count`
    #xml_count=`expr $xml_count + 1` 
    local xml_puren=`config_getoption "$xml_config" puren`
    
    local xml_wlan_wifi=`config_getoption "$xml_config" wifi`
    local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
    local xml_wifi_txpower=`config_getoption "$xml_wifi" txpoff`
    local xml_wifi_pretxp=`config_getoption "$xml_wifi" pretxp`
    local xml_wifi_cwwmode=`config_getoption "$xml_wifi" cwwmode`
    local xml_wifi_extoffset=`config_getoption "$xml_wifi" extoffset`
    local xml_wifi_shortgi=`config_getoption "$xml_wifi" shortgi`


    local xml_wlan_diversity=`config_getoption "$xml_wifi" diversity`
    local xml_wlan_txantenna=`config_getoption "$xml_wifi" txantenna`
    local xml_wlan_rxantenna=`config_getoption "$xml_wifi" rxantenna`

    local xml_wlan_autochannel=`config_getoption "$xml_wifi" autochannel`
    eval wifi_devmode="$"DEVMODE_$xml_wlan_wifi

    local xml_config_vlan=`config_read /config/network/vap$vapnum/vlan`
    local xml_wlan_vlan_enable=`config_getoption "$xml_config_vlan" enable`
    local xml_wlan_vlan_vid=`config_getoption "$xml_config_vlan" vid` 



    if [ "$xml_enable" == "yes" ]; then

        if [ "$wifi_devmode" == "bg" ];then
            echo "$xml_wlan_autochannel" > /proc/sys/dev/$xml_wlan_wifi/nonoverlapping
        elif [ "$wifi_devmode" == "abg" ];then
            echo "$xml_wlan_autochannel" > /proc/sys/dev/$xml_wlan_wifi/nonoverlapping
        fi


        $SYSCTL -w net.$xml_if.maxaid=$xml_count > /dev/null 2>&1
        $IWPRIV $xml_if ap_bridge $xml_isolation > /dev/null 2>&1
        $IWPRIV $xml_if switch $xml_bcisolation > /dev/null 2>&1
        $IWPRIV $xml_if puren $xml_puren > /dev/null 2>&1

        $IWPRIV "$xml_if" dtim_period "$xml_wlan_dtim" > /dev/null 2>&1
        $IWCONFIG "$xml_if" frag "$xml_wlan_frag" > /dev/null 2>&1

        if [ "$xml_wlan_preamble" == "Short" ]; then
            $IWPRIV "$xml_if" shpreamble 1 > /dev/null 2>&1
        else
            $IWPRIV "$xml_if" shpreamble 0 > /dev/null 2>&1
        fi

        #     eval cur_power="$"DEVPOWER_$xml_wlan_wifi
        #     eval cur_poweroff="$"DEVPOWEROFF_$xml_wlan_wifi
        #     [ $cur_power -lt $xml_wifi_txpower ]&& xml_wifi_txpower=$cur_power

        #txpower set after vap up
        $IFCONFIG "$xml_if" up > /dev/null 2>&1


        $SETTXPOWER "$xml_if" "$xml_wifi_txpower" > /dev/null 2>&1
        if [ $? -eq 0 ];then        
            xml_wifi=`config_setoption "$xml_wifi" pretxp "$xml_wifi_txpower"`
        else
            xml_wifi=`config_setoption "$xml_wifi" txpoff "$xml_wifi_pretxp"`
        fi
        config_write /config/network/$xml_wlan_wifi "$xml_wifi"  



        $IWCONFIG "$xml_if" rts "$xml_wlan_rts" > /dev/null 2>&1

        if [ "$xml_forward2" == "yes" ]; then     
            if [ "$xml_wlan_vlan_enable" == "1" ]; then
                $BRCTL delif $BRLAN "$xml_if" > /dev/null 2>&1
                $BRCTL addif "br$xml_wlan_vlan_vid" "$xml_if" > /dev/null 2>&1	 
            elif [ "$xml_wlan_vlan_enable" == "0" ]; then	    
                $BRCTL delif "br$xml_wlan_vlan_vid" "$xml_if" > /dev/null 2>&1
                $BRCTL addif $BRLAN "$xml_if" > /dev/null 2>&1
                local xml_config_vlan1=`config_read /config/network/vap$vapnum/vlan`
                xml_config_vlan1=`config_setoption "$xml_config_vlan1" vid ""`	
                config_write /config/network/vap$vapnum/vlan "$xml_config_vlan1"
            fi 
        fi

        #if [ "$xml_wlan_diversity" == "0" ]; then
        echo "$DEVICE_MODEL" | grep -q 'N'
        if [ $? -eq 0 ];then
            echo "11n"

            $IWPRIV "$xml_if" cwmmode "$xml_wifi_cwwmode" > /dev/null 2>&1
            $IWPRIV "$xml_if" extoffset "$xml_wifi_extoffset" > /dev/null 2>&1
            $IWPRIV "$xml_if" shortgi "$xml_wifi_shortgi"  > /dev/null 2>&1

        else

            $SETANTENNA $xml_wlan_wifi diversity $xml_wlan_diversity 2>/dev/null
            #echo $xml_wlan_diversity > /proc/sys/dev/wifi0/diversity 
            #fi

            $SETANTENNA $xml_wlan_wifi tx $xml_wlan_txantenna 2>/dev/null
            $SETANTENNA $xml_wlan_wifi rx $xml_wlan_rxantenna 2>/dev/null
            #echo $xml_wlan_txantenna > /proc/sys/dev/wifi0/txantenna
            #echo $xml_wlan_rxantenna > /proc/sys/dev/wifi0/rxantenna   
        fi
    fi
}

#
#wmm_conf
#
wmm_conf()
{
        local xml_vap=`config_read /config/network`
        local vapnum=`config_getoption "$xml_vap" vapnum`
        local vapname="vap$vapnum"

        local xml_config=`config_read /config/network/$vapname`
        local xml_if=`config_getoption "$xml_config" if`
        local xml_enable=`config_getoption "$xml_config" enable`

        if [ "$xml_enable" == "yes" ];then

            local xml_config_wmm=`config_read /config/network/$vapname/wmm`
            local xml_wlan_wmm_enable=`config_getoption "$xml_config_wmm" enable`
            $IWPRIV $xml_if wmm $xml_wlan_wmm_enable > /dev/null 2>&1
            if [ "$xml_wlan_wmm_enable" == "1" ];then

                local xml_wmm_wtenable=`config_getoption "$xml_config_wmm" wtenable`
                local xml_wmm_wvenable=`config_getoption "$xml_config_wmm" wvenable`
                local xml_wmm_bgmint=`config_getoption "$xml_config_wmm" bgmint`
                local xml_wmm_bgmaxt=`config_getoption "$xml_config_wmm" bgmaxt`
                local xml_wmm_bgaift=`config_getoption "$xml_config_wmm" bgaift`
                local xml_wmm_bgopbt=`config_getoption "$xml_config_wmm" bgopbt`
                local xml_wmm_bgnoack=`config_getoption "$xml_config_wmm" bgnoack`

                local xml_wmm_bemint=`config_getoption "$xml_config_wmm" bemint`
                local xml_wmm_bemaxt=`config_getoption "$xml_config_wmm" bemaxt`
                local xml_wmm_beaift=`config_getoption "$xml_config_wmm" beaift`
                local xml_wmm_beopbt=`config_getoption "$xml_config_wmm" beopbt`
                local xml_wmm_benoack=`config_getoption "$xml_config_wmm" benoack`

                local xml_wmm_vdmint=`config_getoption "$xml_config_wmm" vdmint`
                local xml_wmm_vdmaxt=`config_getoption "$xml_config_wmm" vdmaxt`
                local xml_wmm_vdaift=`config_getoption "$xml_config_wmm" vdaift`
                local xml_wmm_vdopbt=`config_getoption "$xml_config_wmm" vdopbt`
                local xml_wmm_vdnoack=`config_getoption "$xml_config_wmm" vdnoack`

                local xml_wmm_vcmint=`config_getoption "$xml_config_wmm" vcmint`
                local xml_wmm_vcmaxt=`config_getoption "$xml_config_wmm" vcmaxt`
                local xml_wmm_vcaift=`config_getoption "$xml_config_wmm" vcaift`
                local xml_wmm_vcopbt=`config_getoption "$xml_config_wmm" vcopbt`
                local xml_wmm_vcnoack=`config_getoption "$xml_config_wmm" vcnoack`

                local xml_wmm_bgminf=`config_getoption "$xml_config_wmm" bgminf`
                local xml_wmm_bgmaxf=`config_getoption "$xml_config_wmm" bgmaxf`
                local xml_wmm_bgaiff=`config_getoption "$xml_config_wmm" bgaiff`
                local xml_wmm_bgopbf=`config_getoption "$xml_config_wmm" bgopbf`

                local xml_wmm_beminf=`config_getoption "$xml_config_wmm" beminf`
                local xml_wmm_bemaxf=`config_getoption "$xml_config_wmm" bemaxf`
                local xml_wmm_beaiff=`config_getoption "$xml_config_wmm" beaiff`
                local xml_wmm_beopbf=`config_getoption "$xml_config_wmm" beopbf`

                local xml_wmm_vdminf=`config_getoption "$xml_config_wmm" vdminf`
                local xml_wmm_vdmaxf=`config_getoption "$xml_config_wmm" vdmaxf`
                local xml_wmm_vdaiff=`config_getoption "$xml_config_wmm" vdaiff`
                local xml_wmm_vdopbf=`config_getoption "$xml_config_wmm" vdopbf`

                local xml_wmm_vcminf=`config_getoption "$xml_config_wmm" vcminf`
                local xml_wmm_vcmaxf=`config_getoption "$xml_config_wmm" vcmaxf`
                local xml_wmm_vcaiff=`config_getoption "$xml_config_wmm" vcaiff`
                local xml_wmm_vcopbf=`config_getoption "$xml_config_wmm" vcopbf`

                $SLEEP 5 > /dev/null 2>&1
                $AUTELAN wmm tos_flag $xml_if $xml_wmm_wtenable > /dev/null 2>&1
                $AUTELAN wmm 1p_flag $xml_if $xml_wmm_wvenable > /dev/null 2>&1

                $IWPRIV $xml_if cwmin 1 0 $xml_wmm_bgmint > /dev/null 2>&1
                $IWPRIV $xml_if cwmax 1 0 $xml_wmm_bgmaxt > /dev/null 2>&1
                $IWPRIV $xml_if aifs 1 0 $xml_wmm_bgaift > /dev/null 2>&1
                $IWPRIV $xml_if txoplimit 1 0 $xml_wmm_bgopbt > /dev/null 2>&1
                $IWPRIV $xml_if noackpolicy 1 0 $xml_wmm_bgnoack > /dev/null 2>&1

                $IWPRIV $xml_if cwmin 0 0 $xml_wmm_bemint > /dev/null 2>&1
                $IWPRIV $xml_if cwmax 0 0 $xml_wmm_bemaxt > /dev/null 2>&1
                $IWPRIV $xml_if aifs 0 0 $xml_wmm_beaift > /dev/null 2>&1
                $IWPRIV $xml_if txoplimit 0 0 $xml_wmm_beopbt > /dev/null 2>&1
                $IWPRIV $xml_if noackpolicy 0 0 $xml_wmm_benoack > /dev/null 2>&1

                $IWPRIV $xml_if cwmin 2 0 $xml_wmm_vdmint > /dev/null 2>&1
                $IWPRIV $xml_if cwmax 2 0 $xml_wmm_vdmaxt > /dev/null 2>&1
                $IWPRIV $xml_if aifs 2 0 $xml_wmm_vdaift > /dev/null 2>&1
                $IWPRIV $xml_if txoplimit 2 0 $xml_wmm_vdopbt > /dev/null 2>&1
                $IWPRIV $xml_if noackpolicy 2 0 $xml_wmm_vdnoack > /dev/null 2>&1

                $IWPRIV $xml_if cwmin 3 0 $xml_wmm_vcmint > /dev/null 2>&1
                $IWPRIV $xml_if cwmax 3 0 $xml_wmm_vcmaxt > /dev/null 2>&1
                $IWPRIV $xml_if aifs 3 0 $xml_wmm_vcaift > /dev/null 2>&1
                $IWPRIV $xml_if txoplimit 3 0 $xml_wmm_vcopbt > /dev/null 2>&1
                $IWPRIV $xml_if noackpolicy 3 0 $xml_wmm_vcnoack > /dev/null 2>&1

                $IWPRIV $xml_if cwmin 1 1 $xml_wmm_bgminf > /dev/null 2>&1
                $IWPRIV $xml_if cwmax 1 1 $xml_wmm_bgmaxf > /dev/null 2>&1
                $IWPRIV $xml_if aifs 1 1 $xml_wmm_bgaiff > /dev/null 2>&1
                $IWPRIV $xml_if txoplimit 1 1 $xml_wmm_bgopbf > /dev/null 2>&1

                $IWPRIV $xml_if cwmin 0 1 $xml_wmm_beminf > /dev/null 2>&1
                $IWPRIV $xml_if cwmax 0 1 $xml_wmm_bemaxf > /dev/null 2>&1
                $IWPRIV $xml_if aifs 0 1 $xml_wmm_beaiff > /dev/null 2>&1
                $IWPRIV $xml_if txoplimit 0 1 $xml_wmm_beopbf > /dev/null 2>&1

                $IWPRIV $xml_if cwmin 2 1 $xml_wmm_vdminf > /dev/null 2>&1
                $IWPRIV $xml_if cwmax 2 1 $xml_wmm_vdmaxf > /dev/null 2>&1
                $IWPRIV $xml_if aifs 2 1 $xml_wmm_vdaiff > /dev/null 2>&1
                $IWPRIV $xml_if txoplimit 2 1 $xml_wmm_vdopbf > /dev/null 2>&1

                $IWPRIV $xml_if cwmin 3 1 $xml_wmm_vcminf > /dev/null 2>&1
                $IWPRIV $xml_if cwmax 3 1 $xml_wmm_vcmaxf > /dev/null 2>&1
                $IWPRIV $xml_if aifs 3 1 $xml_wmm_vcaiff > /dev/null 2>&1
                $IWPRIV $xml_if txoplimit 3 1  $xml_wmm_vcopbf > /dev/null 2>&1

                if [ "$xml_wmm_wvenable" == "1" ];then
                    local xml_wmm_wvingress0=`config_getoption "$xml_config_wmm" wvingress0`
                    local xml_wmm_wvingress1=`config_getoption "$xml_config_wmm" wvingress1`
                    local xml_wmm_wvingress2=`config_getoption "$xml_config_wmm" wvingress2`
                    local xml_wmm_wvingress3=`config_getoption "$xml_config_wmm" wvingress3` 

                    local xml_wmm_wvegress0=`config_getoption "$xml_config_wmm" wvegress0`
                    local xml_wmm_wvegress1=`config_getoption "$xml_config_wmm" wvegress1`
                    local xml_wmm_wvegress2=`config_getoption "$xml_config_wmm" wvegress2`
                    local xml_wmm_wvegress3=`config_getoption "$xml_config_wmm" wvegress3`
                    local xml_wmm_wvegress4=`config_getoption "$xml_config_wmm" wvegress4`
                    local xml_wmm_wvegress5=`config_getoption "$xml_config_wmm" wvegress5`
                    local xml_wmm_wvegress6=`config_getoption "$xml_config_wmm" wvegress6`
                    local xml_wmm_wvegress7=`config_getoption "$xml_config_wmm" wvegress7`

                    $AUTELAN wmm 1p_ingress_map $xml_if 0 $xml_wmm_wvingress0 > /dev/null 2>&1
                    $AUTELAN wmm 1p_ingress_map $xml_if 1 $xml_wmm_wvingress1 > /dev/null 2>&1
                    $AUTELAN wmm 1p_ingress_map $xml_if 2 $xml_wmm_wvingress2 > /dev/null 2>&1
                    $AUTELAN wmm 1p_ingress_map $xml_if 3 $xml_wmm_wvingress3 > /dev/null 2>&1

                    $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress0 0 > /dev/null 2>&1
                    $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress1 1 > /dev/null 2>&1
                    $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress2 2 > /dev/null 2>&1
                    $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress3 3 > /dev/null 2>&1
                    $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress4 4 > /dev/null 2>&1
                    $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress5 5 > /dev/null 2>&1
                    $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress6 6 > /dev/null 2>&1
                    $AUTELAN wmm 1p_egress_map $xml_if $xml_wmm_wvegress7 7 > /dev/null 2>&1
                fi

                if [ "$xml_wmm_wtenable" == "1" ];then
                    local xml_wmm_wtegress8=`config_getoption "$xml_config_wmm" wtegress8`
                    local xml_wmm_wtegress32=`config_getoption "$xml_config_wmm" wtegress32`
                    local xml_wmm_wtegress40=`config_getoption "$xml_config_wmm" wtegress40`
                    local xml_wmm_wtegress48=`config_getoption "$xml_config_wmm" wtegress48`
                    local xml_wmm_wtegress136=`config_getoption "$xml_config_wmm" wtegress136`
                    local xml_wmm_wtegress160=`config_getoption "$xml_config_wmm" wtegress160`
                    local xml_wmm_wtegress184=`config_getoption "$xml_config_wmm" wtegress184`
                    local xml_wmm_wtegress224=`config_getoption "$xml_config_wmm" wtegress224`

                    local xml_wmm_twegress8=`config_getoption "$xml_config_wmm" twegress8`
                    local xml_wmm_twegress32=`config_getoption "$xml_config_wmm" twegress32`
                    local xml_wmm_twegress40=`config_getoption "$xml_config_wmm" twegress40`
                    local xml_wmm_twegress48=`config_getoption "$xml_config_wmm" twegress48`
                    local xml_wmm_twegress136=`config_getoption "$xml_config_wmm" twegress136`
                    local xml_wmm_twegress160=`config_getoption "$xml_config_wmm" twegress160`
                    local xml_wmm_twegress184=`config_getoption "$xml_config_wmm" twegress184`
                    local xml_wmm_twegress224=`config_getoption "$xml_config_wmm" twegress224`

                    $AUTELAN wmm tos_egress_map $xml_if 0 $xml_wmm_twegress8 $xml_wmm_wtegress8 > /dev/null 2>&1
                    $AUTELAN wmm tos_egress_map $xml_if 1 $xml_wmm_twegress32 $xml_wmm_wtegress32 > /dev/null 2>&1
                    $AUTELAN wmm tos_egress_map $xml_if 2 $xml_wmm_twegress40 $xml_wmm_wtegress40 > /dev/null 2>&1
                    $AUTELAN wmm tos_egress_map $xml_if 3 $xml_wmm_twegress48 $xml_wmm_wtegress48 > /dev/null 2>&1
                    $AUTELAN wmm tos_egress_map $xml_if 4 $xml_wmm_twegress136 $xml_wmm_wtegress136 > /dev/null 2>&1
                    $AUTELAN wmm tos_egress_map $xml_if 5 $xml_wmm_twegress160 $xml_wmm_wtegress160 > /dev/null 2>&1
                    $AUTELAN wmm tos_egress_map $xml_if 6 $xml_wmm_twegress184 $xml_wmm_wtegress184 > /dev/null 2>&1
                    $AUTELAN wmm tos_egress_map $xml_if 7 $xml_wmm_twegress224 $xml_wmm_wtegress224 > /dev/null 2>&1
                fi
            fi



        fi
}

#
#bandwidth_conf
#
bandwidth_conf()
{
    #echo "$DEVICE_MODEL" | grep -q 'N'
    #if [ $? -eq 0 ];then
    #	echo "11n"
    #else
    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum`
    local vapname="vap$vapnum"

    local xml_config=`config_read /config/network/$vapname`
    local xml_if=`config_getoption "$xml_config" if`
    local xml_enable=`config_getoption "$xml_config" enable`

    if [ "$xml_enable" == "yes" ];then
        local xml_config_apbw=`config_read /config/network/$vapname/bandwidth/apbw`
        local xml_apbw_maxclient=`config_getoption "$xml_config_apbw" maxclientrate`
        local xml_apbw_maxclient_send=`config_getoption "$xml_config_apbw" maxclientrate_send`
        local xml_apbw_enable=`config_getoption "$xml_config_apbw" apbwenable`

        local xml_config_estabw=`config_read /config/network/$vapname/bandwidth/estabw`
        local xml_estabw_enable=`config_getoption "$xml_config_estabw" estabwenable`
        local xml_stabw_enable=`config_getoption "$xml_config_estabw" stabwenable`
        local xml_estabw_maxbw=`config_getoption "$xml_config_estabw" maxbw`
        local xml_estabw_maxbw_send=`config_getoption "$xml_config_estabw" maxbw_send`

        local xml_config_stabw1=`config_read /config/network/$vapname/bandwidth/stabw1`
        local xml_stabw1_mac=`config_getoption "$xml_config_stabw1" mac`
        local xml_stabw1_maxbw=`config_getoption "$xml_config_stabw1" maxbw`
        local xml_stabw1_maxbw_send=`config_getoption "$xml_config_stabw1" maxbw_send`

        local xml_config_stabw2=`config_read /config/network/$vapname/bandwidth/stabw2`
        local xml_stabw2_mac=`config_getoption "$xml_config_stabw2" mac`
        local xml_stabw2_maxbw=`config_getoption "$xml_config_stabw2" maxbw`
        local xml_stabw2_maxbw_send=`config_getoption "$xml_config_stabw2" maxbw_send`

        local xml_config_stabw3=`config_read /config/network/$vapname/bandwidth/stabw3`
        local xml_stabw3_mac=`config_getoption "$xml_config_stabw3" mac`
        local xml_stabw3_maxbw=`config_getoption "$xml_config_stabw3" maxbw`
        local xml_stabw3_maxbw_send=`config_getoption "$xml_config_stabw3" maxbw_send`

        local xml_config_stabw4=`config_read /config/network/$vapname/bandwidth/stabw4`
        local xml_stabw4_mac=`config_getoption "$xml_config_stabw4" mac`
        local xml_stabw4_maxbw=`config_getoption "$xml_config_stabw4" maxbw`
        local xml_stabw4_maxbw_send=`config_getoption "$xml_config_stabw4" maxbw_send`

        local xml_config_stabw5=`config_read /config/network/$vapname/bandwidth/stabw5`
        local xml_stabw5_mac=`config_getoption "$xml_config_stabw5" mac`
        local xml_stabw5_maxbw=`config_getoption "$xml_config_stabw5" maxbw`
        local xml_stabw5_maxbw_send=`config_getoption "$xml_config_stabw5" maxbw_send`

        local xml_config_stabw6=`config_read /config/network/$vapname/bandwidth/stabw6`
        local xml_stabw6_mac=`config_getoption "$xml_config_stabw6" mac`
        local xml_stabw6_maxbw=`config_getoption "$xml_config_stabw6" maxbw`
        local xml_stabw6_maxbw_send=`config_getoption "$xml_config_stabw6" maxbw_send`

        local xml_config_stabw7=`config_read /config/network/$vapname/bandwidth/stabw7`
        local xml_stabw7_mac=`config_getoption "$xml_config_stabw7" mac`
        local xml_stabw7_maxbw=`config_getoption "$xml_config_stabw7" maxbw`
        local xml_stabw7_maxbw_send=`config_getoption "$xml_config_stabw7" maxbw_send`

        local xml_config_stabw8=`config_read /config/network/$vapname/bandwidth/stabw8`
        local xml_stabw8_mac=`config_getoption "$xml_config_stabw8" mac`
        local xml_stabw8_maxbw=`config_getoption "$xml_config_stabw8" maxbw`
        local xml_stabw8_maxbw_send=`config_getoption "$xml_config_stabw8" maxbw_send`

        local xml_config_stabw9=`config_read /config/network/$vapname/bandwidth/stabw9`
        local xml_stabw9_mac=`config_getoption "$xml_config_stabw9" mac`
        local xml_stabw9_maxbw=`config_getoption "$xml_config_stabw9" maxbw`
        local xml_stabw9_maxbw_send=`config_getoption "$xml_config_stabw9" maxbw_send`

        $AUTELAN traffic_limit $xml_if set_vap_flag $xml_apbw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_every_node_flag $xml_estabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw_enable > /dev/null 2>&1

        if [ "$xml_apbw_enable" == "1" ];then
            [ -n "$xml_apbw_maxclient" ] && $AUTELAN traffic_limit $xml_if set_vap $xml_apbw_maxclient > /dev/null 2>&1
            [ -n "$xml_apbw_maxclient_send" ] && $AUTELAN traffic_limit $xml_if set_vap_send $xml_apbw_maxclient_send > /dev/null 2>&1
        fi

        if [ "$xml_estabw_enable" == "1" ];then
            [ -n "$xml_estabw_maxbw" ] && $AUTELAN traffic_limit $xml_if set_every_node $xml_estabw_maxbw > /dev/null 2>&1
            [ -n "$xml_estabw_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_every_node_send $xml_estabw_maxbw_send > /dev/null 2>&1
        fi


        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw1_mac $xml_stabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw2_mac $xml_stabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw3_mac $xml_stabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw4_mac $xml_stabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw5_mac $xml_stabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw6_mac $xml_stabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw7_mac $xml_stabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw8_mac $xml_stabw_enable > /dev/null 2>&1
        $AUTELAN traffic_limit $xml_if set_specific_node_flag $xml_stabw9_mac $xml_stabw_enable > /dev/null 2>&1
        if [ "$xml_stabw_enable" == "1" ];then
            [ -n "$xml_stabw1_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw1_mac $xml_stabw1_maxbw > /dev/null 2>&1
            [ -n "$xml_stabw2_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw2_mac $xml_stabw2_maxbw > /dev/null 2>&1
            [ -n "$xml_stabw3_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw3_mac $xml_stabw3_maxbw > /dev/null 2>&1
            [ -n "$xml_stabw4_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw4_mac $xml_stabw4_maxbw > /dev/null 2>&1
            [ -n "$xml_stabw5_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw5_mac $xml_stabw5_maxbw > /dev/null 2>&1
            [ -n "$xml_stabw6_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw6_mac $xml_stabw6_maxbw > /dev/null 2>&1
            [ -n "$xml_stabw7_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw7_mac $xml_stabw7_maxbw > /dev/null 2>&1
            [ -n "$xml_stabw8_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw8_mac $xml_stabw8_maxbw > /dev/null 2>&1
            [ -n "$xml_stabw9_maxbw" ] && $AUTELAN traffic_limit $xml_if set_specific_node $xml_stabw9_mac $xml_stabw9_maxbw > /dev/null 2>&1

            [ -n "$xml_stabw1_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw1_mac $xml_stabw1_maxbw_send > /dev/null 2>&1
            [ -n "$xml_stabw2_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw2_mac $xml_stabw2_maxbw_send > /dev/null 2>&1
            [ -n "$xml_stabw3_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw3_mac $xml_stabw3_maxbw_send > /dev/null 2>&1
            [ -n "$xml_stabw4_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw4_mac $xml_stabw4_maxbw_send > /dev/null 2>&1
            [ -n "$xml_stabw5_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw5_mac $xml_stabw5_maxbw_send > /dev/null 2>&1
            [ -n "$xml_stabw6_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw6_mac $xml_stabw6_maxbw_send > /dev/null 2>&1
            [ -n "$xml_stabw7_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw7_mac $xml_stabw7_maxbw_send > /dev/null 2>&1
            [ -n "$xml_stabw8_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw8_mac $xml_stabw8_maxbw_send > /dev/null 2>&1
            [ -n "$xml_stabw9_maxbw_send" ] && $AUTELAN traffic_limit $xml_if set_specific_node_send $xml_stabw9_mac $xml_stabw9_maxbw_send > /dev/null 2>&1

        fi

    fi
    # fi

}

SysLogServer_conf()
{

    local xml_syslogserver=`config_read /config/system/log` 
    local xml_syslogserver_enable=`config_getoption "$xml_syslogserver" enable`
    local xml_syslogserver_sip=`config_getoption "$xml_syslogserver" sip`
    local xml_syslogserver_sport=`config_getoption "$xml_syslogserver" sport`
    #############tmp code#########
    local xml_syslogserver_upenable=`config_getoption "$xml_syslogserver" upenable`
    local xml_syslogserver_upip=`config_getoption "$xml_syslogserver" upip`
    local xml_syslogserver_uptime=`config_getoption "$xml_syslogserver" uptime`
    ##############################
    if [ "$xml_syslogserver" != "" ]; then
        if [ "$xml_syslogserver_enable" == "yes" ];then
            if [ "$xml_syslogserver_sip" != "" ]; then
                killall syslogd	 > /dev/null 2>&1		
                $SYSLOGD -s 2 -O /tmp/logmsg/messages -R $xml_syslogserver_sip:$xml_syslogserver_sport  > /dev/null 2>&1
            fi
        else
            killall syslogd  > /dev/null 2>&1
            $SYSLOGD -s 2 -O /tmp/logmsg/messages > /dev/null 2>&1
        fi
    fi
    #############tmp code#########
    [ "$xml_syslogserver_upenable" == "yes" ]&&/usr/sbin/mydaemon "/usr/sbin/upload.sh" "$xml_syslogserver_upip" "$xml_syslogserver_uptime" 2>/dev/null
    [ "$xml_syslogserver_upenable" == "" ]&&killall upload.sh 2>/dev/null
    ##############################
}

#
#snmp_conf
#
snmp_conf()
{

    local xml_snmp=`config_read /config/administrator/snmp`

    local xml_snmp_enable=`config_getoption "$xml_snmp" enable`
    local xml_snmp_syslocation=`config_getoption "$xml_snmp" syslocation`
    local xml_snmp_syscontact=`config_getoption "$xml_snmp" syscontact`
    local xml_snmp_publicCN=`config_getoption "$xml_snmp" publicCN`
    local xml_snmp_privateCN=`config_getoption "$xml_snmp" privateCN`
    local xml_snmp_TCN=`config_getoption "$xml_snmp" TCN`
    local xml_snmp_snmpip=`config_getoption "$xml_snmp" snmpip`
    local xml_http=`config_read /config/administrator/http`
    local xml_http_enable=`config_getoption "$xml_http" enable`

    local xml_https=`config_read /config/administrator/https`
    local xml_https_enable=`config_getoption "$xml_https" enable`

    local xml_ssh=`config_read /config/administrator/ssh`
    local xml_ssh_enable=`config_getoption "$xml_ssh" enable`

    local xml_telnet=`config_read /config/administrator/telnet`
    local xml_telnet_enable=`config_getoption "$xml_telnet" enable`

    local OEM_PATH="/jffs/.OEM"
    local LOCATION="${OEM_PATH}/.location"
    local CONTACT="${OEM_PATH}/.contact"
    if [ "$xml_snmp_syslocation" == "default" ]; then
        if [ -f ${LOCATION} ]; then
            xml_snmp_syslocation="`cat ${LOCATION}`"
            xml_snmp=`config_setoption "$xml_snmp" syslocation "$xml_snmp_syslocation"`
        fi
        if [ -f ${CONTACT} ]; then
            xml_snmp_syscontact="`cat ${CONTACT}`"
            xml_snmp=`config_setoption "$xml_snmp" syscontact "$xml_snmp_syscontact"`
        fi
        config_write /config/administrator/snmp "$xml_snmp"
    fi

    if [ "$xml_ssh_enable" == "yes" ];then
        $KILLALL dropbear  > /dev/null 2>&1
        $DROPBEAR start  > /dev/null 2>&1
    else
        $KILLALL dropbear > /dev/null 2>&1
    fi

    if [ "$xml_telnet_enable" == "yes" ];then
        $KILLALL telnetd > /dev/null 2>&1
        $TELNETD > /dev/null 2>&1
    else
        $KILLALL telnetd > /dev/null 2>&1
    fi

    if [ "$xml_snmp_enable" == "1" ];then
        echo "com2sec  ro  default   $xml_snmp_publicCN">/etc/snmp/snmpd.conf
        echo "com2sec  rw  default   $xml_snmp_privateCN">>/etc/snmp/snmpd.conf
        echo "">>/etc/snmp/snmpd.conf
        echo "group  $xml_snmp_publicCN  v1   ro">>/etc/snmp/snmpd.conf
        echo "group  $xml_snmp_publicCN  v2c   ro">>/etc/snmp/snmpd.conf
        echo "group  $xml_snmp_publicCN  usm   ro">>/etc/snmp/snmpd.conf
        echo "group  $xml_snmp_privateCN  v1   rw">>/etc/snmp/snmpd.conf
        echo "group  $xml_snmp_privateCN  v2c   rw">>/etc/snmp/snmpd.conf
        echo "group  $xml_snmp_privateCN  usm   rw">>/etc/snmp/snmpd.conf
        echo "">>/etc/snmp/snmpd.conf
        echo "">>/etc/snmp/snmpd.conf
        echo "view  all  included  .1">>/etc/snmp/snmpd.conf
        echo "access  $xml_snmp_publicCN  \"\"  any  noauth  exact  all  none  none">>/etc/snmp/snmpd.conf
        echo "access  $xml_snmp_privateCN  \"\"  any  noauth  exact  all  all  all">>/etc/snmp/snmpd.conf
        echo "">>/etc/snmp/snmpd.conf
        echo "syslocation $xml_snmp_syslocation">>/etc/snmp/snmpd.conf
        echo "syscontact $xml_snmp_syscontact">>/etc/snmp/snmpd.conf
        echo "">>/etc/snmp/snmpd.conf
        echo "trapcommunity $xml_snmp_TCN">>/etc/snmp/snmpd.conf
        echo "trapsink $xml_snmp_snmpip">>/etc/snmp/snmpd.conf
        echo "trap2sink $xml_snmp_snmpip">>/etc/snmp/snmpd.conf
        echo "informsink $xml_snmp_snmpip">>/etc/snmp/snmpd.conf
        killall snmpd		
        $SNMPD -c /etc/snmp/snmpd.conf -C
        $SET_CRONTAB $PERTRAP m30 > /dev/null 2>&1
    else
        killall snmpd > /dev/null 2>&1
    fi

    /usr/sbin/test

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

    if [ "$xml_dhcp_enable" == "yes" ];then
        echo "resolv-file=/etc/resolv.conf" > $DNSMASQ_CONF
        echo "dhcp-leasefile=/var/lib/misc/dnsmasq.leases" >> $DNSMASQ_CONF
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

#
#dhcpsnooping_conf
#
dhcpsnooping_conf()
{
    local xml_dhcpsnoop=`config_read /config/network/dhcp/dhcpsnooping`
    local xml_dhcpsnoop_enable=`config_getoption "$xml_dhcpsnoop" enable`
    local xml_dhcpsnoop_interface=`config_getoption "$xml_dhcpsnoop" interface`
    local xml_dhcpsnoop_info=`config_getoption "$xml_dhcpsnoop" info`

    [ -z $xml_dhcpsnoop_interface ]||xml_dhcpsnoop_argument="-t $xml_dhcpsnoop_interface"
    [ -z $xml_dhcpsnoop_interface ]||[ -z $xml_dhcpsnoop_info ]||xml_dhcpsnoop_argument="-t $xml_dhcpsnoop_interface -o $xml_dhcpsnoop_info"
    if [ "$xml_dhcpsnoop_enable" == "yes" ];then
        $KILLALL dhcpsnooping > /dev/null 2>&1
        $EBTABLES -D FORWARD -p IPv4 --ip-proto udp --ip-sport 67:68 --ip-dport 67:68 -j DROP > /dev/null 2>&1
        $EBTABLES -A FORWARD -p IPv4 --ip-proto udp --ip-sport 67:68 --ip-dport 67:68 -j DROP > /dev/null 2>&1
        /usr/sbin/mydaemon $DHCPSNOOPING $xml_dhcpsnoop_argument > /dev/null 2>&1
    else
        $KILLALL dhcpsnooping > /dev/null 2>&1
        $EBTABLES -D FORWARD -p IPv4 --ip-proto udp --ip-sport 67:68 --ip-dport 67:68 -j DROP > /dev/null 2>&1
    fi

}

#
#igmpsnooping_conf
#
igmpsnooping_conf()
{
    local xml_igmpsnoop=`config_read /config/network/igmp/igmpsnooping`
    local xml_igmpsnoop_enable=`config_getoption "$xml_igmpsnoop" enable`

    if [ "$xml_igmpsnoop_enable" == "yes" ];then
        /usr/sbin/igmp_snoop start > /dev/null 2>&1
    else
        /usr/sbin/igmp_snoop stop > /dev/null 2>&1
    fi
}

#
#mldsnooping_conf
#
mldsnooping_conf()
{
    local xml_mldsnoop=`config_read /config/network/mld/mldsnooping`
    local xml_mldsnoop_enable=`config_getoption "$xml_mldsnoop" enable`

    if [ "$xml_mldsnoop_enable" == "yes" ];then
        /usr/sbin/mld_snoop start > /dev/null 2>&1
    else
        /usr/sbin/mld_snoop stop > /dev/null 2>&1
    fi
}

#
#autelogger(flag,string)
#
autelogger()
{
    local flag=$1
    local string=$2
    if [ "$flag" == "enable" ]||[ "$flag" == "1" ]||[ "$flag" == "yes" ]; then
        $LOGGER "$string"
    fi
}

#
#adminsec_conf
#
adminsec_conf()
{  
    local xml_netrule=`config_read /config/netsecury/netfilter`
    local xml_netrule_count=`config_getoption "$xml_netrule" icount`
    local xml_netrule_idefault=`config_getoption "$xml_netrule" inputdefault`

    $EBTABLES -F INPUT > /dev/null 2>&1

    if [ "$xml_netrule_idefault" == "ACCEPT" -o "$xml_netrule_idefault" == "DROP" ];then
        $EBTABLES -P INPUT $xml_netrule_idefault > /dev/null 2>&1
    fi

    if [ "$xml_netrule_count" != "0" ]; then
        for rc in `seq $xml_netrule_count`
        do
            eval LINE$rc=\"`config_getoption "$xml_netrule" "inrule$rc" 2>/dev/null`\"
            for cellnum in `seq 4`
            do
                eval "cell$cellnum=\` echo \$LINE$rc | awk -F \"%\" '{print \$$cellnum}' \`"
            done
            if [ "$cell1" == "-" ];then
                cell1=""
            else
                cell1="-i $cell1"
            fi
            if [ "$cell2" == "-" ];then
                cell2=""
            else
                cell2="-s $cell2"
            fi
            if [ "$cell3" == "-" ];then
                cell3=""
            else
                cell3="-p IPv4 --ip-src $cell3"
            fi
            if [ "$cell4" == "-" ];then
                cell4=""
            else
                cell4="-j $cell4"
            fi

            $EBTABLES -A INPUT $cell1 $cell2 $cell3 $cell4  > /dev/null 2>&1
        done
    fi
}

#
#netfilter_conf
#
netfilter_conf()
{   

    local xml_netrule=`config_read /config/netsecury/netfilter`
    local xml_netrule_count=`config_getoption "$xml_netrule" fcount`
    local xml_netrule_fdefault=`config_getoption "$xml_netrule" forwardefault`

    $EBTABLES -F FORWARD > /dev/null 2>&1
    local xml_dhcpsnoop=`config_read /config/network/dhcp/dhcpsnooping`
    local xml_dhcpsnoop_enable=`config_getoption "$xml_dhcpsnoop" enable`
    if [ "$xml_dhcpsnoop_enable" == "yes" ]; then
        $EBTABLES -A FORWARD -p IPv4 --ip-proto udp --ip-sport 67:68 --ip-dport 67:68 -j DROP > /dev/null 2>&1
    fi
    if [ "$xml_netrule_fdefault" == "ACCEPT" -o "$xml_netrule_fdefault" == "DROP" ];then
        $EBTABLES -P FORWARD $xml_netrule_fdefault > /dev/null 2>&1
    fi

    if [ "$xml_netrule_count" != "0" ]; then
        for rc in `seq $xml_netrule_count`
        do
            eval LINE$rc=\"`config_getoption "$xml_netrule" "forule$rc" 2>/dev/null`\"
            for cellnum in `seq 8`
            do
                eval "cell$cellnum=\` echo \$LINE$rc | awk -F \"%\" '{print \$$cellnum}' \`"
            done
            if [ "$cell1" == "-" ];then
                cell1=""
            else
                cell1="-i $cell1"
            fi
            if [ "$cell2" == "-" ];then
                cell2=""
            else
                cell2="-o $cell2"
            fi
            if [ "$cell3" == "-" ];then
                cell3=""
            else
                cell3="-s $cell3"
            fi
            if [ "$cell4" == "-" ];then
                cell4=""
            else
                cell4="-d $cell4"
            fi
            if [ "$cell5" == "-" ];then
                cell5=""
            else
                cell5="--ip-src $cell5"
            fi
            if [ "$cell6" == "-" ];then
                cell6=""
            else
                cell6="--ip-dst $cell6"
            fi
            if [ "$cell7" == "---" ];then
                cell7=""
            else
                local atr1=`echo $cell7 | awk -F'-' '{print $1}'`
                local atr2=`echo $cell7 | awk -F'-' '{print $2}'`
                local atr3=`echo $cell7 | awk -F'-' '{print $3}'`
                if [ "$atr2" != "" -a "$atr3" != "" ]; then
                    cell7="--ip-proto $atr1 --ip-sport $atr2 --ip-dport $atr3"
                elif [ "$atr2" == "" -a "$atr3" != "" ]; then
                    cell7="--ip-proto $atr1 --ip-dport $atr3"
                elif [ "$atr2" != "" -a "$atr3" == "" ]; then
                    cell7="--ip-proto $atr1 --ip-sport $atr2"
                else
                    cell7="--ip-proto $atr1"
                fi
            fi
            if [ "$cell8" == "-" ];then
                cell8=""
            else
                cell8="-j $cell8"
            fi
            if [ "$cell7" == "" -a "$cell5" == "" -a "$cell6" == "" ]; then
                $EBTABLES -A FORWARD $cell1 $cell2 $cell3 $cell4 $cell5 $cell6 $cell7 $cell8 > /dev/null 2>&1
            else
                $EBTABLES -A FORWARD $cell1 $cell2 $cell3 $cell4 -p IPv4 $cell5 $cell6 $cell7 $cell8 > /dev/null 2>&1
            fi
        done
    fi

}

#
#tr069_conf
#
tr069_conf()
{
    local tr069enable="$FORM_tr069enable"
    local acs="$FORM_acs"
    local lport="$FORM_lport"
    local tr069menable="$FORM_tr069menable"
    local poll="$FORM_poll"
    local suser="$FORM_suser"
    local spass="$FORM_spass"
    local suser_r="$FORM_suser_r"
    local spass_r="$FORM_spass_r"

    if [ "$tr069enable" != "" -o "$tr069menable" != "" ];then
        local xml_tr069=`config_read /config/administrator/tr069`
        xml_tr069=`config_setoption "$xml_tr069" enable $tr069enable`
        xml_tr069=`config_setoption "$xml_tr069" acs $acs`
        xml_tr069=`config_setoption "$xml_tr069" port $lport`
        xml_tr069=`config_setoption "$xml_tr069" pollenable $tr069menable`
        xml_tr069=`config_setoption "$xml_tr069" pollinterval $poll`
        xml_tr069=`config_setoption "$xml_tr069" suser $suser`
        xml_tr069=`config_setoption "$xml_tr069" spass $spass`
        xml_tr069=`config_setoption "$xml_tr069" suser_r $suser_r`
        xml_tr069=`config_setoption "$xml_tr069" spass_r $spass_r`
        config_write /config/administrator/tr069 "$xml_tr069"
    fi

    local xml_tr069=`config_read /config/administrator/tr069`
    local xml_tr069_enable=`config_getoption "$xml_tr069" enable`
    local xml_tr069_acs=`config_getoption "$xml_tr069" acs`
    local xml_tr069_port=`config_getoption "$xml_tr069" port`
    local xml_tr069_pollenable=`config_getoption "$xml_tr069" pollenable`
    local xml_tr069_pollinterval=`config_getoption "$xml_tr069" pollinterval`
    local xml_tr069_suser=`config_getoption "$xml_tr069" suser`
    local xml_tr069_spass=`config_getoption "$xml_tr069" spass`
    local xml_tr069_suser_r=`config_getoption "$xml_tr069" suser_r`
    local xml_tr069_spass_r=`config_getoption "$xml_tr069" spass_r`

    echo "InternetGatewayDevice.ManagementServer.PeriodicInformEnable=$xml_tr069_pollenable" > $TR069_CONF
    if [ "$xml_tr069_pollenable" == "1" ];then
        echo "InternetGatewayDevice.ManagementServer.PeriodicInformInterval=$xml_tr069_pollinterval" >> $TR069_CONF
    fi

    echo "Port=$xml_tr069_port" > $CWMP_CONF
    echo "ConnectionTimeout=60" >> $CWMP_CONF

    if [ "$xml_tr069_suser" != "" ];then
        echo "UserName=$xml_tr069_suser" >> $CWMP_CONF
        echo "Password=$xml_tr069_spass" >> $CWMP_CONF
    fi
    if [ "$xml_tr069_suser_r" != "" ];then
        echo "WWW-UserName=$xml_tr069_suser_r" >> $CWMP_CONF
        echo "WWW-Password=$xml_tr069_spass_r" >> $CWMP_CONF
    fi


    local DATA_PORT=`cat $DATA_CONF|awk -F'=' '/Port/{print $2}' `

    local TIMER=`date|egrep -o -e '[0-9]{1,2}:[0-9]{1,2}'`
    local MINUTE=`echo $TIMER | awk -F':' '{print $2}'`
    local HOUR=`echo $TIMER | awk -F':' '{print $1}'`

    if [ $MINUTE -eq 59 ];then
        if [ $HOUR -eq 23 ];then
            HOUR=0
        else
            HOUR=`expr $HOUR + 1`
        fi
        MINUTE=0
    else
        MINUTE=`expr $MINUTE + 1`
    fi

    if [ "$xml_tr069_enable" == "1" ];then
        $START_TR069 $xml_tr069_acs
    else
        $STOP_TR069
    fi


}

#
#intelligence_conf
#
intelligence_conf()
{
    local xml_linkdetect=`config_read /config/network/intelligence/linkdetect`
    local xml_linkdetect_eth0=`config_getoption "$xml_linkdetect" vap_eth0`
    local xml_linkdetect_eth1=`config_getoption "$xml_linkdetect" vap_eth1`
    local xml_linkdetect_interval=`config_getoption "$xml_linkdetect" interval`
    local xml_linkdetect_upip=`config_getoption "$xml_linkdetect" upip`
    local xml_linkdetect_uptime=`config_getoption "$xml_linkdetect" uptime`

    local xml_autochannel=`config_read /config/network/intelligence/autochannel`
    local xml_autochannel_wifi0=`config_getoption "$xml_autochannel" channel_wifi0`
    local xml_autochannel_wifi1=`config_getoption "$xml_autochannel" channel_wifi1`
    local xml_autochannel_interval=`config_getoption "$xml_autochannel" interval`

    local xml_autopower=`config_read /config/network/intelligence/autopower`
    local xml_autopower_wifi0=`config_getoption "$xml_autopower" power_wifi0`
    local xml_autopower_wifi1=`config_getoption "$xml_autopower" power_wifi1`
    local xml_autopower_interval=`config_getoption "$xml_autopower" interval`

    local xml_load=`config_read /config/network/intelligence/loadbalance`
    local xml_load_enable=`config_getoption "$xml_load" enable`
    local xml_load_threshold=`config_getoption "$xml_load" wifi0threshold`
    if [ "$xml_linkdetect_upip" == "" ]; then
        $SET_CRONTAB $UPLINK_DETECT m0 > /dev/null 2>&1
    else
        $SET_CRONTAB $UPLINK_DETECT m$xml_linkdetect_uptime "$xml_linkdetect_upip > /dev/null 2>&1" > /dev/null 2>&1
    fi	

    if [ "$xml_linkdetect_eth0" == "ignore" ];then
        $SET_CRONTAB "$LINK_DETECT eth0" m0 > /dev/null 2>&1
    else
        $SET_CRONTAB "$LINK_DETECT eth0" m$xml_linkdetect_interval "$xml_linkdetect_eth0 > /dev/null 2>&1" > /dev/null 2>&1
    fi

    if [ "$xml_linkdetect_eth1" == "ignore" ];then
        $SET_CRONTAB "$LINK_DETECT eth1" m0 > /dev/null 2>&1
    else
        $SET_CRONTAB "$LINK_DETECT eth1" m$xml_linkdetect_interval "$xml_linkdetect_eth1 > /dev/null 2>&1" > /dev/null 2>&1
    fi

    if [ "$xml_autochannel_wifi0" == "wifi0" ];then
        $SET_CRONTAB "$AUTO_CHANNEL wifi0" m$xml_autochannel_interval " > /dev/null 2>&1" > /dev/null 2>&1
    else
        $SET_CRONTAB "$AUTO_CHANNEL wifi0" m0 > /dev/null 2>&1
    fi

    if [ "$xml_autochannel_wifi1" == "wifi1" ];then
        $SET_CRONTAB "$AUTO_CHANNEL wifi1" m$xml_autochannel_interval " > /dev/null 2>&1" > /dev/null 2>&1
    else
        $SET_CRONTAB "$AUTO_CHANNEL wifi1" m0 > /dev/null 2>&1
    fi

    if [ "$xml_autopower_wifi0" == "ignore" ];then
        $SET_CRONTAB "$AUTO_POWER wifi0" m0 > /dev/null 2>&1
    else
        $SET_CRONTAB "$AUTO_POWER wifi0" m$xml_autopower_interval "$xml_autopower_wifi0 > /dev/null 2>&1" > /dev/null 2>&1
    fi

    if [ "$xml_autopower_wifi1" == "ignore" ];then
        $SET_CRONTAB "$AUTO_POWER wifi1" m0 > /dev/null 2>&1
    else
        $SET_CRONTAB "$AUTO_POWER wifi1" m$xml_autopower_interval "$xml_autopower_wifi1 > /dev/null 2>&1" > /dev/null 2>&1
    fi

    killall crond > /dev/null 2>&1
    $CROND start > /dev/null 2>&1

    if [ "$DEV_wifi1" == "enable" ];then
        $SYSCTL -w dev.wifi0.load_balancing=$xml_load_enable
        $SYSCTL -w dev.wifi0.balance_threshold=$xml_load_threshold
    fi
}



# firewall module
# sys_url_filter
# added by tangsiqi
sys_url_filter()
{
    local xml_nat=`config_read /config/firewall/nat`
    local xml_nat_enable=`config_getoption "$xml_nat" enable`

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




    #$IPTABLES -X FORWARD > /dev/null 2>&1
    #$IPTABLES -N FORWARD > /dev/null 2>&1
    $IPTABLES -F FORWARD > /dev/null 2>&1
    #$IPTABLES -D FORWARD -j FORWARD > /dev/null 2>&1

    if [ "$xml_status" == "enable" ];then
        if [ "$xml_url1" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url1" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url2" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url2" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url3" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url3" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url4" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url4" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url5" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url5" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url6" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url6" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url7" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url7" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url8" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url8" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url9" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url9" -j DROP > /dev/null 2>&1
        fi

        if [ "$xml_url10" != "" ];then
            $IPTABLES -A FORWARD -d "$xml_url10" -j DROP > /dev/null 2>&1
        fi
    elif [ "$xml_status" == "disable" ];then
        $IPTABLES -F FORWARD
    fi

    if [ "$xml_nat_enable" == "yes" ];then
        $SYSCTL -w net.ipv4.ip_forward="1" > /dev/null 2>&1
        $IPTABLES -t nat -A POSTROUTING -o eth0 -j MASQUERADE > /dev/null 2>&1
    else
        $IPTABLES -t nat -F POSTROUTING > /dev/null 2>&1
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

    if [ "$xml_connect_mode" == "Demand_connct" ]; then
        echo "demand" > $OPTIONS
        echo "ipcp-accept-remote" >> $OPTIONS
    else
        echo "ipcp-accept-remote" > $OPTIONS
    fi

    echo "ipcp-accept-local" >> $OPTIONS
    echo "holdoff 10" >> $OPTIONS
    echo "connect \"/usr/sbin/chat -t 3 -V -v -f /etc/ppp/chat_script\"" >> $OPTIONS
    if [ "$xml_idle_time_enable" == "enable" ]; then
        echo "idle $xml_max_idle_time" >> $OPTIONS
    fi
    echo "ttyUSB0" >> $OPTIONS
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

    echo "user	root" >> $OPTIONS
    echo "hide-password" >> $OPTIONS

    if [ "$xml_connect_mode" == "Auto_connect" -o "$xml_connect_mode" == "Demand_connct" ]; then
        $KILLALL pppd > /dev/null 2>&1
        $PPPD & > /dev/null 2>&1
    else
        $KILLALL pppd > /dev/null 2>&1
    fi
}


#
#wlwan_conf
#
wlwan_conf()
{
    $KILLALL wpa_supplicant > /dev/null 2>&1

    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum`

    local xml_wlwan=`config_read /config/network/vap$vapnum`
    local xml_wlwan_enable=`config_getoption "$xml_wlwan" enable`
    local xml_wlwan_forward2=`config_getoption "$xml_wlwan" forward2`
    local xml_wlwan_wlanmode=`config_getoption "$xml_wlwan" wlanmode`
    local xml_wlwan_if=`config_getoption "$xml_wlwan" if`
    local xml_wlwan_ssid=`config_getoption "$xml_wlwan" ssid`
    local xml_wlwan_security=`config_getoption "$xml_wlwan" sectype`
    local xml_wlan_assocchan=`config_getoption "$xml_wlwan" assocchan`
    local xml_wlan_assocweight=`config_getoption "$xml_wlwan" assocweight`
    local xml_wlan_assocrssi=`config_getoption "$xml_wlwan" assocrssi`

    local xml_wep=`config_read /config/network/vap$vapnum/wep`
    local xml_wlwan_authmode=`config_getoption "$xml_wep" wepmode`
    local xml_wlwan_keytype=`config_getoption "$xml_wep" wepkeytype`
    local xml_wlwan_cwepkey=`config_getoption "$xml_wep" cwepkey`
    local xml_wlwan_passkey1=`config_getoption "$xml_wep" wepkey1`
    local xml_wlwan_passkey2=`config_getoption "$xml_wep" wepkey2`
    local xml_wlwan_passkey3=`config_getoption "$xml_wep" wepkey3`
    local xml_wlwan_passkey4=`config_getoption "$xml_wep" wepkey4`

    local xml_wpa=`config_read /config/network/vap$vapnum/wpa`
    local xml_wlwan_passphrase=`config_getoption "$xml_wpa" wpapass`

    local CONF_FILE=/etc/wpa_supplicant.conf

    if [ "$xml_wlwan_enable" == "yes" ];then
        local cwepkey=`expr $xml_wlwan_cwepkey + 1`
        eval passkey="$"xml_wlwan_passkey$cwepkey
        $IWPRIV "$xml_wlwan_if" select_weight  $xml_wlan_assocweight > /dev/null 2>&1
        $IWPRIV "$xml_wlwan_if" select_chan  $xml_wlan_assocchan > /dev/null 2>&1
        $IWPRIV "$xml_wlwan_if" near_rssi  $xml_wlan_assocrssi > /dev/null 2>&1
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
                    echo "	auth_alg=SHARED" >> $CONF_FILE
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
                echo "	proto=wpa2" >> $CONF_FILE
                echo "	psk=\"$xml_wlwan_passphrase\"" >> $CONF_FILE
                echo "}" >> $CONF_FILE
        esac

        #$IFCONFIG $xml_wlwan_if up > /dev/null 2>&1
        #sleep 2 > /dev/null 2>&1

        sleep 2 > /dev/null 2>&1
        if [ "$xml_wlwan_forward2" == "yes" ]; then
            $WPA_SUPPLICANT -B -c /etc/wpa_supplicant.conf -i $xml_wlwan_if -b ${BRLAN} -D madwifi > /dev/null 2>&1
        else
            $WPA_SUPPLICANT -B -c /etc/wpa_supplicant.conf -i $xml_wlwan_if -D madwifi > /dev/null 2>&1
        fi
    else
        $IFCONFIG $xml_wlwan_if down > /dev/null 2>&1
    fi
}

#
#thin_wlwan_conf
#
thin_wlwan_conf()
{
    $KILLALL wpa_supplicant > /dev/null 2>&1

    local xml_vap=`config_read /config/network`
    local vapnum=`config_getoption "$xml_vap" vapnum`

    local xml_wlwan=`config_read /config/network/vap$vapnum`
    local xml_wlwan_enable=`config_getoption "$xml_wlwan" enable`
    local xml_wlwan_wlanmode=`config_getoption "$xml_wlwan" wlanmode`
    local xml_wlwan_if=`config_getoption "$xml_wlwan" if`
    local xml_wlwan_ssid=`config_getoption "$xml_wlwan" ssid`
	local xml_wlwan_bssid=`config_getoption "$xml_wlwan" bssid`
	local xml_wlwan_txpower=`config_getoption "$xml_wlwan" txpower`
	local xml_wlan_wifi=`config_getoption "$xml_wlwan" wifi`

	if [ "$xml_wlwan_wlanmode" == "client" ];then	
		if [ "$xml_wlwan_enable" == "yes" ];then
		#	$BRCTL delif default eth0 > /dev/null 2>&1
			echo 1 > /proc/sys/net/capwap/wds_tunnel 
			$WLANCONFIG $xml_wlwan_if create wlandev $xml_wlan_wifi wlanmode sta nosbeacon  > /dev/null 2>&1
			$BRCTL addif default $xml_wlwan_if > /dev/null 2>&1
			$IWCONFIG  $xml_wlwan_if txpower $xml_wlwan_txpower > /dev/null 2>&1
			$IWCONFIG  $xml_wlwan_if essid "$xml_wlwan_ssid" > /dev/null 2>&1
			$IWCONFIG  $xml_wlwan_if ap $xml_wlwan_bssid > /dev/null 2>&1
			$IWPRIV $xml_wlwan_if wds 1 > /dev/null 2>&1
			 $IFCONFIG $xml_wlwan_if up > /dev/null 2>&1
		else
			$IFCONFIG $xml_wlwan_if down > /dev/null 2>&1
		fi
	fi
}

#
#thin_wlwan_startsetup
#
thin_wlwan_startsetup()
{
	for vapnum in `seq $NUM_ap`  
    do
		local xml_wlwan=`config_read /config/network/vap$vapnum`	
		local xml_wlwan_enable=`config_getoption "$xml_wlwan" enable`
		local xml_wlwan_if=`config_getoption "$xml_wlwan" if`
		local xml_wlwan_wlanmode=`config_getoption "$xml_wlwan" wlanmode`
		local xml_wlwan_ssid=`config_getoption "$xml_wlwan" ssid`
		local xml_wlwan_txpower=`config_getoption "$xml_wlwan" txpower`
		local xml_wlan_wifi=`config_getoption "$xml_wlwan" wifi`
		
		if [ "$xml_wlwan_wlanmode" == "client" ];then	
			if [ "$xml_wlwan_enable" == "yes" ];then
			#	$BRCTL delif default eth0 > /dev/null 2>&1
				echo 1 > /proc/sys/net/capwap/wds_tunnel 
				$WLANCONFIG $xml_wlwan_if create wlandev $xml_wlan_wifi wlanmode sta nosbeacon  > /dev/null 2>&1
				$BRCTL addif default $xml_wlwan_if > /dev/null 2>&1
				$IWCONFIG  $xml_wlwan_if txpower $xml_wlwan_txpower > /dev/null 2>&1
				$IWCONFIG  $xml_wlwan_if essid $xml_wlwan_ssid > /dev/null 2>&1
				$IWPRIV $xml_wlwan_if wds 1 > /dev/null 2>&1
				$IWPRIV $xml_wlan_wifi set_rate_cap 0x0 > /dev/null 2>&1
				$IWPRIV $xml_wlan_wifi set_mcs_cap 0x0 > /dev/null 2>&1
				$IFCONFIG $xml_wlwan_if up > /dev/null 2>&1
			else
				$IFCONFIG $xml_wlwan_if down > /dev/null 2>&1
			fi
		fi
	done
}

#
#locate_conf
#
locate_conf()
{
    local	xml_locate=`config_read /config/locate`
    local   xml_locate_enable=`config_getoption "$xml_locate" enable`
    local	xml_macfilter_enable=`config_getoption "$xml_locate" macfilter`
    local	xml_locate_macaddr=`config_getoption "$xml_locate" macaddr`
    local	xml_locate_region=`config_getoption "$xml_locate" region`
    local	xml_locate_scantype=`config_getoption "$xml_locate" scantype`
    local	xml_locate_scansize=`config_getoption "$xml_locate" scansize`
    local	xml_locate_interval=`config_getoption "$xml_locate" interval`
    local	xml_locate_ipaddr=`config_getoption "$xml_locate" ipaddr`
    local	xml_locate_port=`config_getoption "$xml_locate" port`

    local conf_path=/jffs/nvram_config
    echo "wl0_hwaddr <00:12:34:56:78:90>" > $conf_path
    echo "time_zone <$xml_locate_region>" >> $conf_path
    echo "aplocator_scan_type <$xml_locate_scantype>" >> $conf_path
    if [ "$xml_macfilter_enable" == "yes" ]; then
        echo "aplocator_mac_filter <1>" >> $conf_path
        echo "aplocator_mac_filter_data <$xml_locate_macaddr>" >> $conf_path
    else
        echo "aplocator_mac_filter <0>" >> $conf_path
        echo "aplocator_mac_filter_data <$xml_locate_macaddr>" >> $conf_path
    fi
    echo "aplocator_scan_time <$xml_locate_interval>" >> $conf_path
    echo "aplocator_file_size <$xml_locate_scansize>" >> $conf_path
    echo "aplocator_server_ip <$xml_locate_ipaddr>" >> $conf_path
    echo "aplocator_server_port <$xml_locate_port>" >> $conf_path

    if [ "$xml_locate_enable" == "yes" ]; then
        $APLOCATOR start > /dev/null 2>&1
    else
        $APLOCATOR stop > /dev/null 2>&1 
    fi

}

#
#sys_acktimeout_setup
#
sys_acktimeout_setup()
{
    local xml_wifi0=`config_read /config/network/wifi0`
	local xml_wifi0_distance=`config_getoption "$xml_wifi0" distance`

	local xml_wifi1=`config_read /config/network/wifi1`
	local xml_wifi1_distance=`config_getoption "$xml_wifi1" distance`

	$ACKTIMEOUT wifi0 set $xml_wifi0_distance > /dev/null 2>&1
	$ACKTIMEOUT wifi1 set $xml_wifi1_distance > /dev/null 2>&1

}

#
#sys_emenu_wlan_setup
#
sys_emenu_wlan_setup()
{
    for vapnum in `seq $NUM_ap`  
    do
        local xml_wlan=`config_read /config/network/vap$vapnum`
        local xml_if=`config_getoption "$xml_wlan" if`
        local xml_enable=`config_getoption "$xml_wlan" create`
        local xml_ssid=`config_getoption "$xml_wlan" ssid`

        local xml_wlan_wifi=`config_getoption "$xml_wlan" wifi`
        local xml_wifi=`config_read /config/network/$xml_wlan_wifi`
        local xml_wifi_channel=`config_getoption "$xml_wifi" channel`
        local xml_wifi_mode=`config_getoption "$xml_wifi" mode`
        local xml_wifi_cwmmode=`config_getoption "$xml_wifi" cwwmode`
        local xml_wifi_extoffset=`config_getoption "$xml_wifi" extoffset`
	    local xml_wifi_txpower=`config_getoption "$xml_wifi" txpower`

        if [ "$xml_enable" == "yes" ]; then
		$WLANCONFIG "$xml_if" create wlandev "$xml_wlan_wifi" wlanmode ap > /dev/null 2>&1
		[ -n "$xml_ssid" ] && $IWCONFIG "$xml_if" essid "$xml_ssid" > /dev/null 2>&1
		$IWCONFIG "$xml_if" channel "$xml_wifi_channel" > /dev/null 2>&1
		$IFCONFIG $xml_if up > /dev/null 2>&1
        $IFCONFIG $xml_if down > /dev/null 2>&1
		$SETTXPOWER "$xml_if" "$xml_wifi_txpower" > /dev/null 2>&1
		$IWCONFIG "$xml_if" channel 0 > /dev/null 2>&1
                $SET_MODE "$xml_if" "$xml_wifi_mode" "$xml_wifi_cwmmode" "$xml_wifi_extoffset"
		$IWCONFIG "$xml_if" channel "$xml_wifi_channel" > /dev/null 2>&1
            	$BRCTL addif default $xml_if > /dev/null 2>&1
                $IFCONFIG $xml_if up > /dev/null 2>&1
	fi
     done
}
#
#sys_emenu_macfilter_setup
#
sys_emenu_macfilter_setup()
{
    for vapnum in `seq $NUM_ap`
    do
        local xml_config=`config_read /config/network/vap$vapnum`
        local xml_enable=`config_getoption "$xml_config" create`
        local xml_wlanmode=`config_getoption "$xml_config" wlanmode`
        local xml_config_vap=`config_read /config/network/vap1`


        if [ "$xml_wlanmode" == "ap" ]||[ "$xml_wlanmode" == "root" ]&&[ "$xml_enable" == "yes"  ]; then
            local xml_if=`config_getoption "$xml_config" if`
            local xml_macfilter=`config_getoption "$xml_config_vap" macfilter`
            local xml_filtermode=`config_getoption "$xml_config_vap" filtermode`
            local xml_macount=`config_getoption "$xml_config_vap" mac_count`
            for emac in `seq $xml_macount`
            do
            	local  xml_mac$emac=`$AWK 'NR=='$emac'' /jffs/whitelist`
            done

            $IWPRIV "$xml_if" maccmd 3 > /dev/null 2>&1
            if [ "$xml_macfilter" == "yes" ]; then
                if [ "$xml_filtermode" == "permit" ]; then
                    $IWPRIV "$xml_if" maccmd 1 > /dev/null 2>&1
                elif [ "$xml_filtermode" == "prevent" ]; then
                    $IWPRIV "$xml_if" maccmd 2 > /dev/null 2>&1
                fi
                for mac in `seq $xml_macount`
                do
                    eval macaddr="$"xml_mac$mac
                    echo "$macaddr"|grep "[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}:[0-9a-fA-F]\{1,2\}"
                    if [ $? == 0 ]; then
                        $IWPRIV "$xml_if" addmac $macaddr > /dev/null 2>&1
                    fi
                done
            elif [ "$xml_macfilter" == "no" ]; then
                $IWPRIV "$xml_if" maccmd 0 > /dev/null 2>&1
            fi

        fi
    done
}
