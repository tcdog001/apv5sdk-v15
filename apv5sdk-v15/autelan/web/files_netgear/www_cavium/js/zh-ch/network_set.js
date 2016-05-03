var overlayer_t="正在处理请求，请稍候...";
/***************************/
/*   main_menu_language   */
/***************************/
var mmenu_bs="基本设置";
var mmenu_wl="无线设置";
var mmenu_if="接口设置";
var mmenu_net="网络设置";
var mmenu_as="高级设置";
var mmenu_ac="访问控制";
var mmenu_qos="服务质量";
var mmenu_services="应用服务";
var mmenu_admin="管理信息";
var mmenu_status="状态信息";
var mmenu_help="帮助信息";
var mmenu_logout="退出"; 


/***************************/
/*  network_set_menu               */
/***************************/
var advmenu_wds="无线分布式系统";
var advmenu_stpconfig="生成树协议";
var advmenu_vlanconfig="虚拟局域网";
var advmenu_rog="流氓AP检测";
var advmenu_wm="工作模式";
var advmenu_routesetting="路由设置";
var advmenu_dhcpsnooping="DHCP SNOOPING";
var advmenu_igmpsnooping="IGMP SNOOPING";
var advmenu_mldsnooping="MLD SNOOPING";
var advmenu_dhcpserver="DHCP服务器";
var advmenu_firewallsetting="防火墙设置";

/***************************/
/*  stp_settings_language  */
/***************************/
var stp_config="STP特性";
var stp_prior="桥中AP优先级:";
var stp_delay="桥转发时延:";
var stp_hello="hello报文间隔:";
var stp_age="BPDU消息生存周期";
var stp_name="STP使能状态:";
var stp_enable="启用";
var stp_disable="禁用";
var stp_help="<a href=\"javascript:void(openWin('stp_help.htm', 650, 500));\">帮助>></a>";
var stp_apply="<input type=\"submit\" value=\"应用\">";

/***************************/
/*  vlan_settings_language  */
/***************************/
var	vlan_create="创建";
var	vlan_vid="虚拟ID";
var	vlan_name="虚拟局域网名 ";
var	vlan_ipaddr="IP地址";
var	vlan_mask="子网掩码";
var	vlan_eth0="eth0";
var	vlan_help="<a href=\"javascript:void(openWin('vlan_help.htm', 650, 500));\">帮助>></a>";
var	vlan_apply="<input type=\"submit\" value=\"应用\">";
var vlan_alert_ip="请输入正确的IP地址";
var vlan_alert_mask="请输入正确的子网掩码";
var vlan_alert_vid1="请输入正确的vid1";
var vlan_alert_vid2="请输入正确的vid2";
var vlan_alert_vid3="请输入正确的vid3";
var vlan_alert_vid4="请输入正确的vid4";
var vlan_alert_vid5="请输入正确的vid5";
var vlan_alert_vid6="请输入正确的vid6";
var vlan_alert_vid7="请输入正确的vid7";
var vlan_alert_vid8="请输入正确的vid8";
var vlan_alert_range1="vid1值超出范围";
var vlan_alert_range2="vid2值超出范围";
var vlan_alert_range3="vid3值超出范围";
var vlan_alert_range4="vid4值超出范围";
var vlan_alert_range5="vid5值超出范围";
var vlan_alert_range6="vid6值超出范围";
var vlan_alert_range7="vid7值超出范围";
var vlan_alert_range8="vid8值超出范围";
var alert_vlan1_ip="vip1与";
var alert_vlan2_ip="vip2与";
var alert_vlan3_ip="vip3与";
var alert_vlan4_ip="vip4与";
var alert_vlan5_ip="vip5与";
var alert_vlan6_ip="vip6与";
var alert_vlan7_ip="vip7与";
var alert_vlan8_ip="vip8与";
var alert_vlan_error="的ip地址冲突。";
var alert_vid_vip_error="相同vid的vlan ip地址不同。";

/***************************/
/*  route_settings_language  */
/***************************/
var bnet_alert_ip_error="请输入正确的IP。";
var bnet_alert_mask_error="请输入正确的子网掩码。";
var bnet_alert_gateway_error="请输入正确的网关。";
var	bnet_sroute="静态路由";
var	bnet_droute="默认路由";
var	bnet_dgw="网关:";
var	bnet_route2="路由1";
var	bnet_rip2="IP地址:";
var	bnet_rmask2="子网掩码:";
var	bnet_rgw2="网关:";
var	bnet_route3="路由2";
var	bnet_rip3="IP地址:";
var	bnet_rmask3="子网掩码:";
var	bnet_rgw3="网关:";
var	bnet_route4="路由3";
var	bnet_rip4="IP地址:";
var	bnet_rmask4="子网掩码:";
var	bnet_rgw4="网关:";
var	bnet_eth0="eth0接口配置";
var	bnet_eip0="IP地址:";
var	bnet_emask0="子网掩码:";
var	bnet_efw20="不参与二层转发";
var bnet_eth1="eth1接口配置";
var	bnet_eip1="IP地址:";
var	bnet_emask1="子网掩码:";
var	bnet_efw21="不参与二层转发";
var	bnet_vap1="VAP1接口配置";
var	bnet_vip1="IP地址:";
var	bnet_vmask1="子网掩码:";
var	bnet_vfw21="不参与二层转发";	
var	bnet_vap2="VAP2接口配置";
var	bnet_vip2="IP地址:";
var	bnet_vmask2="子网掩码:";
var	bnet_vfw22="不参与二层转发";
var	bnet_fw3="三层路由转发";
var	bnet_efw3="使能三层路由转发";
var bnet_efw3_alert="NAT已经启用，若关闭三层转发将同时关闭NAT，是否继续？";
var	b_lansu="<input type=\"submit\" value=\"应用\" id=\"lsetup\" >";
var t_help="<a href=\"javascript:void(openWin('lansetting_help.htm', 650, 500));\">帮助>></a>";
var rt_help="<a href=\"javascript:void(openWin('route_help.htm', 650, 500));\">帮助>></a>";



/*********************************/
/*  Firewall_settings_language   */
/********************************/
var firewall_natenable_alert="三层转发未启用，若开启NAT将同时开启三层转发，是否继续？";
var	firewall_natchange="NAT转换";
var	firewall_natchangestate="NAT转换使能状态"; 
var 	firewall_natenable="启用";
var 	firewall_natdisable="禁用";
var	firewall_urlfilter="URL过滤";
var	firewall_urlfilterstatus="URL过滤";
var 	firewall_urlenable="启用";
var 	firewall_urldisable="禁用";
var	firewall_settings_add="<input id=\"but\" type=\"button\" name=\"insert_one_row\" value=\"添加\" onclick='addrulecount();urltable_insRow(\"click\");'/>";
var	firewall_settings_delete="<input id=\"but\" type=\"button\" name=\"delete_one_row\" value=\"删除\" onclick=\"delrulecount();urltable_delRow();\"/>";
var	firewall_settings_apply="<input type=\"submit\" value=\"应用\" id=\"lsetup\" >";
var	firewall_settings_help="<a href=\"javascript:void(openWin('firewall_help.htm', 650, 500));\">帮助>></a>";
var firewall_alert_max="最多设置10个URL过滤";
var firewall_alert_atleast="最少要设置一个URL过滤";


/*****************************************/
/*  DHCP_SNOOPING_settings_language      */
/****************************************/
var	dhcpsnooping_title="DHCP SNOOPING";
var	dhcpsnooping_enablestate="DHCPSNOOPING使能状态";
var	dhcpsnooping_enable="启用";
var	dhcpsnooping_disable="禁用";
var	dhcpsnooping_trustif="信任接口";
var	dhcpsnooping_option82field="Option82字段";
var	dhcpsnooping_usersleaseinfo="用户租用信息";
var	dhcpsnooping_settings_apply="<input type=\"submit\" value=\"应用\" id=\"lsetup\" >";
var	dhcpsnooping_settings_help="<a href=\"javascript:void(openWin('dhcpsnooping_help.htm', 650, 500));\">帮助>></a>";
/*****************************************/
/*  IGMP_SNOOPING_settings_language      */
/****************************************/
var	igmpsnooping_title="IGMP SNOOPING";
var	igmpsnooping_enablestate="IGMPSNOOPING使能状态";
var	igmpsnooping_enable="启用";
var	igmpsnooping_disable="禁用";
var	igmpsnooping_settings_apply="<input type=\"submit\" value=\"应用\" id=\"lsetup\" >";
var	igmpsnooping_settings_help="<a href=\"javascript:void(openWin('igmpsnooping_help.htm', 650, 500));\">帮助>></a>";

/*****************************************/
/*  MLD_SNOOPING_settings_language      */
/****************************************/
var	mldsnooping_title="MLD SNOOPING";
var	mldsnooping_enablestate="MLDSNOOPING使能状态";
var	mldsnooping_enable="启用";
var	mldsnooping_disable="禁用";
var	mldsnooping_settings_apply="<input type=\"submit\" value=\"应用\" id=\"lsetup\" >";
var	mldsnooping_settings_help="<a href=\"javascript:void(openWin('mldsnooping_help.htm', 650, 500));\">帮助>></a>";

/*********************************/
/*  DHCP_settings_language      */
/********************************/
var	dhcp_server="DHCP服务器";
var	dhcp_enablestate="DHCP服务器使能状态";
var	dhcp_enable="启用";
var	dhcp_disable="禁用";
var	dhcp_rentperiod="DHCP租期";
var dhcp_time="小时";
var	dhcp_subnetmask="子网掩码";
var	dhcp_gateway="网关";
var	dhcp_ipaddrange="Ip地址范围";
var	dhcp_settings_apply="<input type=\"submit\" value=\"应用\" id=\"lsetup\" >";
var	dhcp_settings_help="<a href=\"javascript:void(openWin('dhcpserver_help.htm', 650, 500));\">帮助>></a>";
