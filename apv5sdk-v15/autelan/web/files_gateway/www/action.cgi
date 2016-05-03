#!/usr/bin/haserl
<%in /usr/lib/web/libweb.sh %>


<% session_validate %>

<%
my_reboot="FALSE"

case "$FORM_SERVLET" in

"Reboot")
    my_reboot="TRUE"
    ;;

"VAP Apply")     
    wlan_setup
    sys_vap_setup    
    cgi_exec wlan_set.htm  
     ;;
 
"SEC Apply")     
    security_setup 
    sys_security_setup   
    cgi_exec security_settings.htm  
     ;;    
      
"RADIUS Setup") 
    radius_setup 
    cgi_exec radius_settings.htm
    ;;
 
  "wan3g refresh")
		cgi_exec 3g_wan_setting.htm
    ;;
    
   "wanstatus refresh")
   cgi_exec wan_status.htm
    ;;
    
"Reset Default")
    cp -f "$DEFAULT_CONFIG_XML" "$CONFIG_XML" 2>/dev/null
    if [ $? == 0 ]; then
    	my_action_status="Reset Default Successful"
    	my_reboot="TRUE"
    else
    	my_action_status="Reset Default Failed"
    fi
    cgi_exec restore.htm
    ;;
    
"Logout") 
    session_end
    cgi_exec index.htm 
    ;;
    
"Reboot")
    my_reboot="TRUE"
    ;; 

"Change Password") 
    conf_user_setup
    if [ $? != 0 ]; then
        my_action_status="Change Password Failed"
    else
        my_action_status="Change Password Successful"
         sys_user_setup
    fi
    cgi_exec manage.htm 
    ;;   

"DHCP Setup") 
		dhcpserver_setup
    dhcpserver_conf
    cgi_exec lan_settings.htm
    ;;

"Wdif Setup") 
    wired_if_setup
    wired_if_conf
    cgi_exec wired_if.htm 
    ;;

"Wlwan Apply") 
    wlwan_setup
    wlwan_conf
    cgi_exec 802.11_wan_setting.htm 
    ;;
 
"3G wan setting")
		AP_3g_wan_setup
		sys_3g_wan_setup
		cgi_exec 3g_wan_setting.htm 
    ;;

"pppconnect")
		ppp_connect_setup
		ppp_connect_conf
		cgi_exec 3g_wan_setting.htm
		;;

"Wan Apply") 
    wanselect_setup
    wanselect_conf
    cgi_exec wan_selection.htm 
    ;;

"VOIP Apply") 
    voip_setup
    voip_conf
    cgi_exec voip.htm 
    ;;
    
"Set_3G Apply") 
    set_3g_setup
    set_3g_conf
    cgi_exec network_select.htm 
    ;;

"refresh") 
    cgi_exec syslog.htm 
    ;; 
     
# Dummy
*)
    cgi_exec dummy.htm
esac

if [ "$my_reboot" == "TRUE" ]; then
    /sbin/sysreboot &
fi
%>
