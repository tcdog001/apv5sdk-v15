var overlayer_t="The system is dealing with your request,please wait";
/***************************/
/*   main_menu_language   */
/***************************/
var mmenu_bs="BasicSetting";
var mmenu_wl="Wireless";
var mmenu_if="Interface";
var mmenu_net="Network";
var mmenu_as="AdvanceSetting";
var mmenu_ac="AccessControl";
var mmenu_qos="QoS";
var mmenu_services="Service";
var mmenu_admin="Adminstration";
var mmenu_status="Status";
var mmenu_help="Help";
var mmenu_logout="Logout";



/***************************/
/*  network_set_menu               */
/***************************/
var advmenu_wds="WDS";
var advmenu_stpconfig="STP";
var advmenu_vlanconfig="VLAN";
var advmenu_rog="Rogue AP Detection";
var advmenu_wm="work Mode";
var advmenu_routesetting="Route Setting";
var advmenu_dhcpsnooping="DHCP Snooping";
var advmenu_dhcpserver="DHCP Server";
var advmenu_igmpsnooping="IGMP Snooping";
var advmenu_mldsnooping="MLD Snooping";
var advmenu_firewallsetting="Firewall";

/***************************/
/*  stp_settings_language  */
/***************************/
var stp_config="Configuring STP Features";
var	stp_prior="set bridge priority:";
var	stp_delay="set bridge forward delay:";
var	stp_hello="set hello time:";
var	stp_age="set max message age:";
var	stp_name="STP :";
var	stp_enable="Enable";
var	stp_disable="Disable";
var	stp_help="<a href=\"javascript:void(openWin('stpen_help.htm', 650, 500));\">Help>></a>";
var	stp_apply="<input type=\"submit\" value=\"Apply\">";

/***************************/
/*  vlan_settings_language  */
/***************************/
var	vlan_create="create";
var	vlan_vid="vid";
var	vlan_name="vlanname ";
var	vlan_ipaddr="IP";
var	vlan_mask="MASK";
var	vlan_eth0="eth0";
var	vlan_help="<a href=\"javascript:void(openWin('vlanen_help.htm', 650, 500));\">Help>></a>";
var	vlan_apply="<input type=\"submit\" value=\"Apply\">";
var vlan_alert_ip="Please input IP correctly.";
var vlan_alert_mask="Please input NetMask correctly.";
var vlan_alert_vid1="Please input vid1 correctly.";
var vlan_alert_vid2="Please input vid2 correctly.";
var vlan_alert_vid3="Please input vid3 correctly.";
var vlan_alert_vid4="Please input vid4 correctly.";
var vlan_alert_vid5="Please input vid5 correctly.";
var vlan_alert_vid6="Please input vid6 correctly.";
var vlan_alert_vid7="Please input vid7 correctly.";
var vlan_alert_vid8="Please input vid8 correctly.";
var vlan_alert_range1="vid1 is out of range.";
var vlan_alert_range2="vid2 is out of range.";
var vlan_alert_range3="vid3 is out of range.";
var vlan_alert_range4="vid4 is out of range.";
var vlan_alert_range5="vid5 is out of range.";
var vlan_alert_range6="vid6 is out of range.";
var vlan_alert_range7="vid7 is out of range.";
var vlan_alert_range8="vid8 is out of range.";


/***************************/
/*  route_settings_language  */
/***************************/
var bnet_alert_ip_error="Please input IP correctly.";
var bnet_alert_mask_error="Please input NetMask correctly.";
var bnet_alert_gateway_error="Please input Gateway correctly.";
var	bnet_sroute="Static Route";
var bnet_droute="Default Route";
var bnet_dgw="Gateway:";
var bnet_route2="Route 1";
var bnet_rip2="IP Address:";
var bnet_rmask2="Netmask:";
var bnet_rgw2="Gateway:";
var bnet_route3="Route 2";
var bnet_rip3="IP Address:";
var bnet_rmask3="Netmask:";
var bnet_rgw3="Gateway:";
var bnet_route4="Route 3";
var bnet_rip4="IP Address:";
var bnet_rmask4="Netmask:";
var bnet_rgw4="Gateway:";
var bnet_eth0="eth0 Config";
var bnet_eip0="IP Address:";
var bnet_emask0="Netmask:";
var bnet_efw20="Not participate two-layer forwarding";
var bnet_eth1="eth1 Config";
var bnet_eip1="IP Address:";
var bnet_emask1="Netmask:";
var bnet_efw21="Not participate two-layer forwarding";
var bnet_vap1="vap1 Config";
var bnet_vip1="IP Address:";
var bnet_vmask1="Netmask:";
var bnet_vfw21="Not participate two-layer forwarding";	
var bnet_vap2="vap2 Config";
var bnet_vip2="IP Address:";
var bnet_vmask2="Netmask:";
var bnet_vfw22="Not participate two-layer forwarding";
var bnet_fw3="the three-layer forwarding";
var bnet_efw3="Enable the three-layer forwarding of route";		
var bnet_efw3_alert="NAT has already been turned on,if do this NAT would be turned down,continue?";
var b_lansu="<input type=\"submit\" value=\"Apply\" id=\"lsetup\" >";
var t_help="<a href=\"javascript:void(openWin('lansettingen_help.htm', 650, 500));\">Help>></a>";
var rt_help="<a href=\"javascript:void(openWin('routeen_help.htm', 650, 500));\">Help>></a>";
/***************************/
/*  Firewall_settings_language  */
/***************************/
var firewall_natenable_alert="Three-layer forwarding of route is not turned on,if do this Three-layer forwarding of route will be turned on,continue?";
var	firewall_natchange="NAT Conversion";
var	firewall_natchangestate="NAT Conversion Enable State"; 
var 	firewall_natenable="Enable";
var 	firewall_natdisable="Disable";
var	firewall_urlfilter="URL Filter";
var	firewall_urlfilterstatus="URL Filter";
var 	firewall_urlenable="Enable";
var 	firewall_urldisable="Disable";
var	firewall_settings_add="<input id=\"but\" type=\"button\" name=\"insert_one_row\" value=\"Add\" onclick='addrulecount();urltable_insRow(\"click\");'/>";
var	firewall_settings_delete="<input id=\"but\" type=\"button\" name=\"delete_one_row\" value=\"Delete\" onclick=\"delrulecount();urltable_delRow();\"/>";
var	firewall_settings_apply="<input type=\"submit\" value=\"Apply\" id=\"lsetup\" >";
var	firewall_settings_help="<a href=\"javascript:void(openWin('firewallen_help.htm', 650, 500));\">Help>></a>";
var firewall_alert_max="Max number of filter URL is 10!";
var firewall_alert_atleast="There must be at least one url-filter entry!";



/*****************************************/
/*  DHCP_SNOOPING_settings_language      */
/****************************************/
var	dhcpsnooping_title="DHCP SNOOPING";
var	dhcpsnooping_enablestate="DHCP SNOOPING Enable State";
var	dhcpsnooping_enable="Enable";
var	dhcpsnooping_disable="Disable";
var	dhcpsnooping_trustif="Trust Interface";
var	dhcpsnooping_option82field="Option82Field";
var	dhcpsnooping_usersleaseinfo="Users Lease Infomation";
var	dhcpsnooping_setting_apply="<input type=\"submit\" value=\"Apply\" id=\"lsetup\" >";
var	dhcpsnooping_settings_help="<a href=\"javascript:void(openWin('dhcpsnoopingen_help.htm', 650, 500));\">Help>></a>";

/*****************************************/
/*  IGMP_SNOOPING_settings_language      */
/****************************************/
var	igmpsnooping_title="IGMP SNOOPING";
var	igmpsnooping_enablestate="IGMP SNOOPING Enable State";
var	igmpsnooping_enable="Enable";
var	igmpsnooping_disable="Disable";
var	igmpsnooping_setting_apply="<input type=\"submit\" value=\"Apply\" id=\"lsetup\" >";
var	igmpsnooping_settings_help="<a href=\"javascript:void(openWin('igmpsnoopingen_help.htm', 650, 500));\">Help>></a>";


/*****************************************/
/*  MLD_SNOOPING_settings_language      */
/****************************************/
var	mldsnooping_title="MLD SNOOPING";
var	mldsnooping_enablestate="MLD SNOOPING Enable State";
var	mldsnooping_enable="Enable";
var	mldsnooping_disable="Disable";
var	mldsnooping_setting_apply="<input type=\"submit\" value=\"Apply\" id=\"lsetup\" >";
var	mldsnooping_settings_help="<a href=\"javascript:void(openWin('mldsnoopingen_help.htm', 650, 500));\">Help>></a>";

/*********************************/
/*  DHCP_settings_language      */
/********************************/
var	dhcp_server="DHCP Server";
var	dhcp_enablestate="DHCP Server Enable State";
var	dhcp_enable="Enable";
var	dhcp_disable="Disable";
var	dhcp_rentperiod="DHCP Rent Period";
var dhcp_time="hour";
var	dhcp_subnetmask="Subnet Mask";
var	dhcp_gateway="Gateway";
var	dhcp_ipaddrange="Ip Address Range";
var	dhcp_settings_apply="<input type=\"submit\" value=\"Apply\" id=\"lsetup\" >";
var	dhcp_settings_help="<a href=\"javascript:void(openWin('dhcpserveren_help.htm', 650, 500));\">Help>></a>";
