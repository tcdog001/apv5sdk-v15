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
/*  interface_set_menu               */
/***************************/
var ifmenu_wireif="有线接口";
var ifmenu_wirelessif="无线接口";
var ifmenu_3gset="3G设置";
/***************************/
/*  if_settings_language  */
/***************************/
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
var	bnet_vap3="VAP3接口配置";
var	bnet_vip3="IP地址:";
var	bnet_vmask3="子网掩码:";
var	bnet_vfw23="不参与二层转发";	
var	bnet_vap4="VAP4接口配置";
var	bnet_vip4="IP地址:";
var	bnet_vmask4="子网掩码:";
var	bnet_vfw24="不参与二层转发";
var	bnet_vap5="VAP5接口配置";
var	bnet_vip5="IP地址:";
var	bnet_vmask5="子网掩码:";
var	bnet_vfw25="不参与二层转发";	
var	bnet_vap6="VAP6接口配置";
var	bnet_vip6="IP地址:";
var	bnet_vmask6="子网掩码:";
var	bnet_vfw26="不参与二层转发";
var	bnet_vap7="VAP7接口配置";
var	bnet_vip7="IP地址:";
var	bnet_vmask7="子网掩码:";
var	bnet_vfw27="不参与二层转发";	
var	bnet_vap8="VAP8接口配置";
var	bnet_vip8="IP地址:";
var	bnet_vmask8="子网掩码:";
var	bnet_vfw28="不参与二层转发";
var	bnet_vap9="VAP9接口配置";
var	bnet_vip9="IP地址:";
var	bnet_vmask9="子网掩码:";
var	bnet_vfw29="不参与二层转发";	
var	bnet_vap10="VAP10接口配置";
var	bnet_vip10="IP地址:";
var	bnet_vmask10="子网掩码:";
var	bnet_vfw210="不参与二层转发";
var	bnet_vap11="VAP11接口配置";
var	bnet_vip11="IP地址:";
var	bnet_vmask11="子网掩码:";
var	bnet_vfw211="不参与二层转发";	
var	bnet_vap12="VAP12接口配置";
var	bnet_vip12="IP地址:";
var	bnet_vmask12="子网掩码:";
var	bnet_vfw212="不参与二层转发";
var	bnet_fw3="三层路由转发";
var	bnet_efw3="使能三层路由转发";
var	b_lansu="<input type=\"submit\" value=\"应用\" id=\"lsetup\" >";
var t_help="<a href=\"javascript:void(openWin('lansetting_help.htm', 650, 500));\">帮助>></a>";
var wl_help="<a href=\"javascript:void(openWin('wirelessif_help.htm', 650, 500));\">帮助>></a>";
var wd_help="<a href=\"javascript:void(openWin('wiredif_help.htm', 650, 500));\">帮助>></a>";
var w3g_help="<a href=\"javascript:void(openWin('wan3g_help.htm', 650, 500));\">帮助>></a>";
/***************************/
/*  3G_subsetting  */
/***************************/
var ppp_mode="PPP连接模式";
var ppp_auto_connect="自动连接";
var ppp_connect_by_need="按需连接";
var ppp_manual_connect="手动连接";
var ppp_connect_status="连接状态";
var connect_button="连接";
var APN_set="APN设置";
var APN_get_mode="APN获取方式";
var auto_APN="动态获取APN";
var manual_APN="手动获取APN";
var dial_set="拨号设置";
var dial_number="拨号号码";
var PDP_set="PDP设置";
var PDP_type="PDP类型";
var PDP_get_mode="PDP获取方式";
var auto_PDP="动态获取PDP";
var manual_PDP="手动获取PDP";
var PDP_addr="PDP地址";
var PPP_auth="PPP认证";
var PPP_auth_type="认证方式";
var PPP_usr="用户名";
var PPP_passwd="密码";
var PPP_set="PPP设置";
var idle_time_set="空闲时间设置";
var idle_enable="启用";
var idle_disable="禁用";
var idle_time_max="最大空闲时间";
var MTU_set="最大传输单元";
var DNS_set="DNS设置";
var DNS_enable="DNS使能";
var auto_get_DNS="动态获取DNS";
var manual_get_DNS="手动获取DNS";
var first_DNS="首选DNS";
var second_DNS="备用DNS";
var ppp_disconnect="断开";
var ppp_connect="连接";
var ppp_disconstatus="已断开";
var ppp_constatus="已连接";
var ppp_connectstatus="已断开";
