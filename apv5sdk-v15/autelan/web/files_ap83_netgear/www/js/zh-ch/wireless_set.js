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
/*  wireless_set_menu               */
/***************************/
var wiremenu_wi="无线信息";
var wiremenu_ws="无线配置";
var wiremenu_sec="安全";
var wiremenu_rs="Radius服务器";
var wiremenu_mf="MAC地址过滤";
var wiremenu_qos="QoS";
var wiremenu_wds="WDS设置";
var wiremenu_rog="流氓AP检测";
var wiremenu_as="高级设置";
var wiremenu_is="智能设置";

/***************************/
/*  wireless_info_language  */
/***************************/
var winfo_wifi="无线设备";
var winfo_vap="虚拟AP";
var winfo_ssid="SSID";
var winfo_enable="启用";
var winfo_chan="频段";
var winfo_mode="模式";
var winfo_wmode="WLAN模式";
var winfo_help="<a href=\"javascript:void(openWin('wirelessinfo_help.htm', 650, 500));\">帮助>></a>";
var winfo_apply="<input type=\"submit\" value=\"创建\" >";
var winfo_del_button0="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath0\" value=\"删除\">";
var winfo_del_button1="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath1\" value=\"删除\">";
var winfo_del_button2="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath2\" value=\"删除\">";
var winfo_del_button3="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath3\" value=\"删除\">";
var winfo_del_button4="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath4\" value=\"删除\">";
var winfo_del_button5="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath5\" value=\"删除\">";
var winfo_del_button6="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath6\" value=\"删除\">";
var winfo_del_button7="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath7\" value=\"删除\">";
var winfo_del_button8="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath8\" value=\"删除\">";
var winfo_del_button9="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath9\" value=\"删除\">";
var winfo_del_button10="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath10\" value=\"删除\">";
var winfo_del_button11="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath11\" value=\"删除\">";
var winfo_vap_create="VAP创建：";
var winfo_vap_name="VAP名：";
var winfo_vap_parent="所在wifi：";
var winfo_wlan_mode="WLAN MODE：";

/***************************/
/*  wireless_set_language  */
/***************************/
var	wset_wap="无线AP";
var	wset_selectap="VAP指定";
var	wset_currentap="当前指定VAP：";
var	wset_staset="STA接口配置";
var	wset_stastate="STA接口状态：";
var	wset_associated_ssid="关联的SSID：";
var	wset_find_button="<input type=\"button\" value=\"查找\" name=\"scanning\" onclick=\"javascript:g_OpenWindow('scan_vap.htm','650','500')\">";
var	wset_security="安全：";
var	wset_wep_mode="模式：";
var	wset_encrypt_type="加密方式：";
var	wset_wep_key1="WEP密钥1：";
var	wset_wep_key2="WEP密钥2：";
var	wset_wep_key3="WEP密钥3：";
var	wset_wep_key4="WEP密钥4：";
var	wset_wpa_with="WPA关联：";
var	wset_wpa_key="密钥：";
var	wset_wifi_set="wifi参数配置";
var	wset_enable="使能状态";
var	wset_broad="允许广播SSID";
var	wset_network="无线网络";
var	wset_name="SSID名: ";
var	wset_region="地区:";
var	wset_chan="频段:";
var	wset_mode="工作模式:";
var	wset_nonoverlapping="非重叠1、6、11信道自选择：";
var	wset_basic_set="VAP基本配置";
var	wset_rate="传输速率：";
var	wset_beacon_interval="Beacon发送间隔：";
var	wset_label_range="（默认：100，范围：25 - 1000）";
var	wset_help="<a href=\"javascript:void(openWin('wirelesssetting_help.htm', 650, 500));\">帮助>></a>";
var	wset_apply="<input type=\"submit\" value=\"应用\" >";
var	wset_sta_apply="<input type=\"button\" name=\"sta_conf\" value=\"应用\" onclick=\"submit_sta()\">";
var wset_alert_channel="如果你选择这个选项，你必须重启才能使之生效!";
var wset_selectchan="关联AP的信道：";
var wset_assocfail="根据关联失败的权值选择AP：";
var wset_assocrssi="信号接近时根据最大速率值选择AP：";

/***************************/
/*  wireless_security_language  */
/***************************/
var	sec_wst="无线安全类型";
var	sec_wep="有线等效加密(WEP)";
var	sec_mod="工作模式:";
var	sec_wepencry="密钥长度:";
var	sec_etype="密钥类型:";
var	sec_weppass="密钥短语 :";
var	sec_gen="<input type=\"button\" value=\"生成\" name=\"Generate\" onclick=\"genstr()\">";
var	sec_key1="WEP键1:";
var	sec_key2="WEP键2:";
var	sec_key3="WEP键3:";
var	sec_key4="WEP键4:";
var	sec_wpa="WPA";
var	sec_wpawith="WPA关联:";
var	sec_wpaencry="加密方式:";
var	sec_psk="PSK设置";
var	sec_wpapass="密钥 :";
var	sec_cert_type="证书类型：";
var	sec_sec_type="安全类型：";
var	sec_cert_serverip="证书服务器IP地址：";
var	sec_cert_as="AS证书：";
var	sec_cert_ap="AP证书：";
var	sec_generate_button="<input type=\"button\" value=\"生成\" name=\"Generate\" onclick=\"typeselect()\">";
var	sec_help="<a href=\"javascript:void(openWin('wsecurityzh_help.htm', 650, 500));\">帮助>></a>";
var	sec_apply="<input type=\"submit\" value=\"应用\" >";

/***************************/
/*  radius_language  */
/***************************/
var	radius_rss="Radius认证服务器";
var	radius_arss="Radius计费服务器";
var	radius_sip="主认证服务器IP:";
var	radiusc_authport="认证端口:";
var	radius_sk="共享密钥:";
var	radius_sip1="备用认证服务器IP:";
var	radiusc_authport1="认证端口:";
var	radius_sk1="共享密钥:";
var	radius_aip="主计费服务器IP:";
var	radiusc_ap="认证端口:";
var	radius_ask="共享密钥:";
var	radius_aip1="备用计费服务器IP:";
var	radiusc_ap1="计费端口:";
var	radius_ask1="共享密钥:";
var	radius_sip="认证服务器IP:";
var	radius_aip="计费服务器IP:";
var	radiusc_authport="认证端口:";
var	radiusc_ap="计费端口:";
var	radius_sk="共享密钥:";
var	radius_help="<a href=\"javascript:void(openWin('radiusserver_help.htm', 650, 500));\">帮助>></a>";
var	radius_apply="<input type=\"submit\" value=\"应用\" >";

/***************************/
/*  macfilter_language  */
/***************************/
var	mac_use="过滤";
var	mac_filter="MAC过滤";
var	mac_enable="启用";
var	mac_disable="禁用";
var	mac_mode="过滤模式";
var	mac_prevent="阻止被列出来的MAC地址所在主机访问网络 ";
var	mac_permit="仅允许被列出来的MAC地址所在主机访问网络";
var	mac_add_button="<input id=\"but\" type=\"button\" name=\"insert_one_row\" value=\"添加\" onclick='addrulecount();urltable_insRow(\"click\");'/>";
var	mac_del_button="<input id=\"but\" type=\"button\" name=\"delete_one_row\" value=\"删除\" onclick=\"delrulecount();urltable_delRow();\"/>";
var	mac_help="<a href=\"javascript:void(openWin('macfilter_help.htm', 650, 500));\">帮助>></a>";
var	mac_apply="<input type=\"submit\" value=\"应用\" >";

/***************************/
/*  advance_set_language  */
/***************************/
var	adv_as="高级设置";
var	adv_rate="传输速率";
var	adv_rates="(默认: Auto)";
var	adv_beacon="Beacon帧发送时间间隔";
var	adv_beacons="(默认: 100ms, 范围: 25 - 1000)";
var	adv_dtim="DTIM帧发送时间间隔";
var	adv_dtims="(默认: 1, 范围: 1 - 15)";
var	adv_frag="分片门限值";
var	adv_frags="(默认: 2346, 范围: 256 - 2346)";
var	adv_diversity="天线转换模式";
var	adv_diversitys="(默认: true)";
var	adv_tx="发送天线";
var	adv_txs="(默认: Auto)";
var	adv_rx="接收天线";
var	adv_rxs="(默认: Auto)";
var	adv_pream="前导长度";
var	adv_preams="(默认: Short)";
var	adv_power="发射功率";
var	adv_power_big="发射功率";
var	adv_powers="(范围: 1 - 20dbm)";
var	adv_powers_big="(范围: 1 - 27dbm)";
var	adv_rts="RTS门限值";
var	adv_rtss="(默认: 2346, 范围: 1 - 2346)";
var	wset_count="最大连接用户数";
var	wset_isa="用户隔离";
var	adv_wmm="WMM设置 ";
var	adv_wmmsupp="WMM使能状态 ";
var	adv_ack="不回复ACK";
var	adv_enable="启用";
var	adv_disable="禁用";
var	adv_disables="(默认: 禁用)";
var	adv_vlan="虚拟局域网(vlan)";
var	adv_vlans="虚拟局域网 ";
var	adv_vid="虚拟局域网ID";
var	adv_user_set="关联用户配置";
var	adv_cast_iso="单播/广播隔离";
var	adv_help="<a href=\"javascript:void(openWin('advanceset_help.htm', 650, 500));\">帮助>></a>";
var	adv_apply="<input type=\"submit\" value=\"应用\" >";
var adv_cwwmode="信道带宽";
/***************************/
/*  wds_set_language  */
/***************************/
var	wds_name="无线分布式系统(WDS)";
var	wds_prop="WDS使能状态:";
var	wds_enable="启用";
var	wds_disable="禁用";
var	wds_role="角色:";
var	wds_root="Root模式";
var	wds_client="Client模式";
var	wds_ssid="桥接端口的SSID:";
var	wds_chan="WDS频段:";
var	wds_derived="WDS派生接口：";
var	wds_setmac="WDS MAC设定：";
var	wds_help="<a href=\"javascript:void(openWin('wds_help.htm', 650, 500));\">帮助>></a>";
var	wds_apply="<input type=\"submit\" value=\"应用\"  >";

/***************************/
/*  rogue_set_language  */
/***************************/
var	rogue_create="流氓AP检测";
var	rogue_vid="流氓AP检测";
var	rogue_select_if="指定扫描接口：";
var	rogue_refresh="<input type=\"button\"  value=\"刷新\" name=\"refresh\" onclick=\"myrefresh()\" >";
var	rogue_help="<a href=\"javascript:void(openWin('rogueapdetection_help.htm', 650, 500));\">帮助>></a>";
var	rogue_apply="<input type=\"submit\"  value=\"应用\"  >";

/***************************/
/*  intelligent_set_language  */
/***************************/
var iset_uplink_detect="上行链路检测";
var iset_eth0_disable="当eth0去能时：";
var iset_eth1_disable="当eth1去能时：";
var iset_detect_interval="自动检测间隔：";
var iset_minutes="&nbsp;分钟&nbsp;（0-60分钟）";
var iset_uplink_ip="上行链路IP地址：";
var iset_load_balance="负载均衡";
var iset_lb_enable="使能状态：";
var iset_lb_threshold="阀值：";
var iset_channel_auto="信道自动调整";
var iset_wifi0_chanauto="自动调整wifi0的信道：";
var iset_wifi1_chanauto="自动调整wifi1的信道：";
var iset_adjust_interval="自动调整间隔：";
var iset_power_auto="功率自动调整";
var iset_wifi0_powerauto="自动调整wifi0的功率：";
var iset_wifi1_powerauto="自动调整wifi1的功率：";
var	iset_help="<a href=\"javascript:void(openWin('intelligence_help.htm', 650, 500));\">帮助>></a>";
