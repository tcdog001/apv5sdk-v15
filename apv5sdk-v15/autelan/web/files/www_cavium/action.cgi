#!/usr/bin/haserl --upload-limit=1024 
<%in /usr/lib/web/libweb.sh %>


<% session_validate %>

<%
my_reboot="FALSE"

# special for Restore
[ "$IPC_fileupload" == "-ERR" ] && FORM_SERVLET="Restore"

case "$FORM_SERVLET" in

"Reboot")
    my_reboot="TRUE"
    ;;

"thin Config")
    vap_create_conf
    cgi_exec wdssetup.htm
    ;; 

"thin Wlwan Apply")
    thin_wlwan_setup
    thin_wlwan_conf
    cgi_exec wdssetup.htm
    ;;
	
"thin VAPSpec WS") 
    vap_specific_conf    
    cgi_exec wdssetup.htm 
     ;; 

"ACKApply") 
    acktimeout_setup    
    cgi_exec extend.htm 
     ;; 	 
	
# LAN Network
"LAN Setup")
    lan_setup "$FORM_proto" "$FORM_ipaddr" "$FORM_netmask" "$FORM_gateway" "${FORM_count:-3}"
    sys_lan_setup
    cgi_exec lansetup.htm 
    ;; 
    
 "Dns Setup")  
    dns_setup "$FORM_nameserver_config" "$FORM_nameserver1" "$FORM_nameserver2"
    sys_dns_setup    
    cgi_exec  dnsserver.htm  
    ;;  

"NTP Setup")
     ntp_setup
     sys_ntp_setup
     cgi_exec ntpserver.htm 
     ;;  
     
# WLAN Network    
"Config")  
    vap_create_conf
    cgi_exec wlansetup.htm  
    ;; 

"Wlwan Apply") 
    wlwan_setup
    wlwan_conf
    cgi_exec wireless_setting.htm
    ;;
 
 "Locate Setup") 
    locate_setup
    locate_conf
    cgi_exec services.htm
    ;;
       
"Generate") 
   wep_setup
   cgi_exec security.htm 
   ;;  
  
"VAP Apply")  	   
    wlan_setup
    vap_wifi_alldown
    sys_vap_setup
    vap_cspecific_up
    vap_wifi_allup    
    cgi_exec wireless_setting.htm  
     ;;

# URL filter
"URL filter")
	url_filter
	sys_url_filter
	cgi_exec firewall.htm
	;;

 "VAPSpec WS")     
    vap_specific_conf    
    cgi_exec wireless_setting.htm  
     ;; 
     
 "VAPSpec AS")     
    vap_specific_conf    
    cgi_exec advanceset.htm  
     ;;   
     
"VAPSpec SEC")     
    vap_specific_conf    
    cgi_exec security.htm  
     ;; 
     
  "VAPSpec QQueue")     
    vap_specific_conf    
    cgi_exec qos_queue.htm  
     ;; 
     
  "VAPSpec QoS")     
    vap_specific_conf    
    cgi_exec qos.htm  
     ;; 
     
  "VAPSpec MF")     
    vap_specific_conf    
    cgi_exec macfilter.htm  
     ;; 
     
 "SEC Apply")     
 		if [ "$FORM_security" == "wapi" ]; then 
    	if [ "$IPC_fileupload" != "-ERR" ]; then 
    		cp -f "$FORM_uploadfileas" "/jffs/as_cert" > /dev/null 2>&1
    		if [ "$FORM_p12" == "on" ];then
    			cp -f "$FORM_uploadfileap" "/jffs/ap_cert.tmp"
    			/usr/sbin/openssl pkcs12 -in "/jffs/ap_cert.tmp" -out "/jffs/ap_cert" -nodes -noattr -password "pass:${FORM_p12_passwd}"
    			if [ $? -eq 0 ];then
    				echo "success" > /tmp/p12_res
    			else
    				echo "failure" > /tmp/p12_res
    			fi
    			rm -rf /jffs/ap_cert.tmp
    		else
    			cp -f "$FORM_uploadfileap" "/jffs/ap_cert" > /dev/null 2>&1
    		fi
    		if [ "$FORM_3cert" == "1" ];then
      		cp -f "$FORM_uploadfileca" "/jffs/ca.cer" > /dev/null 2>&1
      	fi
    	fi
    fi  
	  security_setup 
    vap_cspecific_down
    sys_security_setup
    vap_cspecific_up 
    if [ "$FORM_security" != "wapi" ]; then
    $KILLALL wapid 2>&1 > /dev/null
    fi 
    cgi_exec security.htm  
     ;;    
      
 "RADIUS Setup") 
    radius_setup 
    cgi_exec radiusserver.htm
    ;;
    
  "AP INFO")
  	ap_info "$FORM_apname"  "$FORM_aplocation" "$FORM_apdiscribe"
    cgi_exec ap_info.htm
    ;;

	"3G wan setting")
		AP_3g_wan_setup
		sys_3g_wan_setup
		cgi_exec wan_3g_settings.htm 
    ;;
    
  "wan3g refresh")
		cgi_exec wan_3g_settings.htm 
    ;;
    
   "pppconnect")
		ppp_connect_setup
		ppp_connect_conf
		cgi_exec wan_3g_settings.htm 
		;;

 "MAC Setup") 
    macfilter_setup
    sys_macfilter_setup
    vap_cspecific_down
    vap_cspecific_up
    cgi_exec macfilter.htm 
    ;;
          
"Advanceset Apply")
   advanceset_setup
   vap_alldown
   sys_advanceset_setup
   vap_allup
   cgi_exec advanceset.htm 
   ;;
   
"Intelligence setup")
   intelligence_setup
   intelligence_conf
   cgi_exec auto_adjust.htm 
   ;;
   
"QoS Apply")
   wmm_setup
   wmm_conf
   cgi_exec qos.htm 
   ;;

"QQueue Setup")
   bandwidth_setup
   bandwidth_conf
   cgi_exec qos_queue.htm 
   ;;
   
# ADVANCE SETUP
"WDS Setup")
    wds_setup
    sys_wds_setup
    cgi_exec wds.htm 
    ;;
     
 "STP Setup")
    stp_setup 
    sys_stp_setup
    cgi_exec stp.htm 
    ;;      

 "VLAN Setup") 
    vlan_setup
    sys_vlan_setup
    cgi_exec vlan.htm 
    ;; 
 "rogueap")
    RogueAp_detection_conf
    cgi_exec rogueap_detection.htm 
    ;; 
      
  "rogue refresh")
    cgi_exec rogueap_detection.htm 
    ;;
      
 "ApplySetup")
 	 lan_setup "$FORM_proto" "$FORM_ipaddr" "$FORM_netmask" "$FORM_gateway" "${FORM_count:-3}"
   ac_setup
   cgi_exec reboot.htm
     /sbin/sysreboot &
   ;;    
    
"TurnMode")
     fatapmode_setup
    cgi_exec reboot.htm
     /sbin/sysreboot &
     ;;
         
"Apply Setup")
    ac_setup
    cp /jffs/config.wtp /tmp/config.wtp
    wtpd_enable
    cgi_exec workmode.htm
    #cgi_exec reboot.htm
    #/sbin/sysreboot &
    ;; 
    
"应用配置")
    ac_setup
    cp /jffs/config.wtp /tmp/config.wtp
    wtpd_enable
    cgi_exec workmode.htm
    #cgi_exec reboot.htm
    #/sbin/sysreboot &
    ;; 
  
"TurnToThin")
    thinapmode_setup 
    cgi_exec reboot.htm
     /sbin/sysreboot &
    ;; 
    
 "转化为瘦AP")
    thinapmode_setup 
    cgi_exec reboot.htm
     /sbin/sysreboot &
    ;; 
    
 # Administration
"Backup")
    if [ -f /jffs/.OEM/.cfg_name ]; then
	cfg=`cat /jffs/.OEM/.cfg_name`
	cp /jffs/config.xml /tmp/$cfg
	CONFIG_XML="/tmp/$cfg"
	cgi_oem_download "$CONFIG_XML" "$cfg"
    else
        cgi_download "$CONFIG_XML"
    fi
    ;;
    
   "备份")
    if [ -f /jffs/.OEM/.cfg_name ]; then
        cfg=`cat /jffs/.OEM/.cfg_name`
        cp /jffs/config.xml /tmp/$cfg
        CONFIG_XML="/tmp/$cfg" 
	cgi_oem_download "$CONFIG_XML" "$cfg"
    else
        cgi_download "$CONFIG_XML"
    fi
    ;;
    
   "backup in device")
    cp /jffs/config.xml /jffs/config.xml_bak
    cgi_exec backsetup.htm
    ;;
    
    "restore in device")
    if [ -f /jffs/config.xml_bak ]; then
	    cp /jffs/config.xml_bak /jffs/config.xml 
	    my_action_status="Restore Successful"
	    my_reboot="TRUE"
    else
    	my_action_status="File cannot restore"
    	cgi_exec backsetup.htm
    fi
    ;;

"Restore")
    if [ "$IPC_fileupload" == "-ERR" ]; then
        my_action_status="File Upload Failed, Restore Cancelled"
        cgi_exec backsetup.htm
    else
        # TODO: config.xml integrity check, size & structure
        cp -f "$FORM_uploadfile" "$CONFIG_XML"
        my_action_status="Restore Successful"
        my_reboot="TRUE"
	     			$SEND_TRAP apFileTransAlarm > /dev/null 2>&1
    fi
    ;;    
    
    "恢复")
    if [ "$IPC_fileupload" == "-ERR" ]; then
        my_action_status="File Upload Failed, Restore Cancelled"
        cgi_exec backsetup.htm
    else
        # TODO: config.xml integrity check, size & structure
        cp -f "$FORM_uploadfile" "$CONFIG_XML"
        my_action_status="Restore Successful"
        my_reboot="TRUE"
	     			$SEND_TRAP apFileTransAlarm > /dev/null 2>&1
        
    fi
    ;;    

"Reset Default")
#    cp -f "$DEFAULT_CONFIG_XML" "$CONFIG_XML"
    ${FACTORY_RESET}
    my_action_status="Reset Default Successful"
    my_reboot="TRUE"
    ;;
    
    "恢复默认")
    ${FACTORY_RESET}
#    cp -f "$DEFAULT_CONFIG_XML" "$CONFIG_XML"
    my_action_status="Reset Default Successful"
    my_reboot="TRUE"
    ;;

    
"Ping")
    cgi_exec ping.htm
    ;;

"Nslookup")
    cgi_exec nslookup.htm
    ;;       

 "Traceroute") 
    cgi_exec traceroute.htm 
    ;; 
 
 "Command")
    cgi_exec shell.htm
    ;;
    
 "Logout") 
    session_end
    cgi_exec index.htm 
    ;;
    
"Reboot")
    my_reboot="TRUE"
    ;; 

"重启")
    my_reboot="TRUE"
    ;;      
    
 "Change Password") 
       conf_user_setup
    if [ $? != 0 ]; then
        my_action_status="Change User or Password Failed"
    else
        my_action_status="Change User and Password Successful"
         sys_user_setup
    fi
    cgi_exec change_password.htm 
    ;;   
"RA Apply") 
    snmp_setup
    snmp_conf
    cgi_exec adminaccess.htm 
    ;;  

"TR069 Apply") 
    tr069_conf
    cgi_exec tr069.htm 
    ;;

"DHCP Setup") 
		dhcpserver_setup
    dhcpserver_conf
    cgi_exec dhcpserver.htm
    ;;
    
"DHCP Snooping") 
		dhcpsnooping_setup
    dhcpsnooping_conf
    cgi_exec dhcp_snooping.htm
    ;;

"IGMP Snooping") 
		igmpsnooping_setup
    igmpsnooping_conf
    cgi_exec igmp_snooping.htm
    ;;
    
"MLD Snooping") 
		mldsnooping_setup
    mldsnooping_conf
    cgi_exec mld_snooping.htm
    ;;
"Route Setup") 
    route_forward_setup
    route_forward_conf
    cgi_exec route_forward.htm
    ;; 

"Wlif Setup") 
    wireless_if_setup
    wireless_if_conf
    cgi_exec wireless_if.htm
    ;;
    
"Wdif Setup") 
    wired_if_setup
    wired_if_conf
    cgi_exec wired_if.htm 
    ;;

"Adminap Setup") 
    adminsec_setup
    adminsec_conf
    cgi_exec admin_security.htm
    ;; 
    
"Netfilter Setup") 
    netfilter_setup
    netfilter_conf
    cgi_exec netfilter.htm 
    ;;    
    
"Apply") 
    syslogserver_setup
    SysLogServer_conf
    cgi_exec syslog.htm 
    ;; 
    
   "应用") 
    syslogserver_setup
    SysLogServer_conf
    cgi_exec syslog.htm 
    ;; 
    
"SaveAs")
    if [ -f "/var/logmsg/messages.0" ]; then
             cgi_download2 "$MESSAGES0_LOG"
    fi
    cgi_download1 "$MESSAGES_LOG"
    ;;  
 
 "另存为")
    if [ -f "/var/logmsg/messages.0" ]; then
             cgi_download2 "$MESSAGES0_LOG"
    fi
    cgi_download1 "$MESSAGES_LOG"
    ;; 
   
"refresh") 
    cgi_exec syslog.htm 
    ;; 
     
 "刷新") 
    cgi_exec syslog.htm 
    ;;   
    
# Dummy
*)
    cgi_exec dummy.htm
esac

if [ "$my_reboot" == "TRUE" ]; then
    cgi_exec reboot.htm
    /sbin/sysreboot &
fi
%>
