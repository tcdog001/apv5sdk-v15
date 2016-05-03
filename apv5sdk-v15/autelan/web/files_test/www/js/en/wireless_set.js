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
/*  wireless_set_menu               */
/***************************/
var wiremenu_wi="WirelessInfo";
var wiremenu_ws="WirelessSet";
var wiremenu_sec="Security";
var wiremenu_rs="RADIUS";
var wiremenu_mf="MacFilter";
var wiremenu_qos="QoS";
var wiremenu_wds="WDS";
var wiremenu_rog="RogueAPDetect";
var wiremenu_as="Advance";
var wiremenu_is="Intelligent";

/***************************/
/*  wireless_info_language  */
/***************************/
var winfo_wifi="WIFI";
var winfo_vap="VAP";
var winfo_ssid="SSID";
var winfo_enable="ENABLE";
var winfo_chan="CHANNEL";
var winfo_mode="MODE";
var winfo_wmode="TYPE";
var winfo_help="<a href=\"javascript:void(openWin('wirelessinfoen_help.htm', 650, 500));\">Help>></a>";
var winfo_apply="<input type=\"submit\" value=\"Establish\" >"; 
var winfo_del_button0="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath0\" value=\"Delete\">";
var winfo_del_button1="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath1\" value=\"Delete\">";
var winfo_del_button2="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath2\" value=\"Delete\">";
var winfo_del_button3="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath3\" value=\"Delete\">";
var winfo_del_button4="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath4\" value=\"Delete\">";
var winfo_del_button5="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath5\" value=\"Delete\">";
var winfo_del_button6="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath6\" value=\"Delete\">";
var winfo_del_button7="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath7\" value=\"Delete\">";
var winfo_del_button8="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath8\" value=\"Delete\">";
var winfo_del_button9="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath9\" value=\"Delete\">";
var winfo_del_button10="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath10\" value=\"Delete\">";
var winfo_del_button11="<input type=\"button\" onClick=\"delvap(this)\" name=\"ath11\" value=\"Delete\">";
var winfo_vap_create="VAP Create:";
var winfo_vap_name="VAP Name:";
var winfo_vap_parent="Dependent wifi:";
var winfo_wlan_mode="WLAN MODE:";
var winfo_vap_create_alert1=" exsists,do you want to rebuild it? After click 'confirm' ";
var winfo_vap_create_alert2=" will be rebuilded.";

/***************************/
/*  wireless_set_language  */
/***************************/
var	wset_wap="Wireless Access Point";
var	wset_selectap="VAP Select";
var	wset_currentap="Current VAP:";
var	wset_staset="STA Settings";
var	wset_stastate="STA State:";
var	wset_associated_ssid="Associated ssid:";
var	wset_find_button="<input type=\"button\" value=\"Find\" name=\"scanning\" onclick=\"javascript:g_OpenWindow('scan_vap.htm','650','500')\">";
var	wset_security="Security:";
var	wset_wep_mode="Mode:";
var	wset_encrypt_type="Encryption Type:";
var	wset_wep_key1="WEP Key 1:";
var	wset_wep_key2="WEP Key 2:";
var	wset_wep_key3="WEP Key 3:";
var	wset_wep_key4="WEP Key 4:";
var	wset_wpa_with="WPA With:";
var	wset_wpa_key="Passphrase:";
var	wset_wifi_set="wifi Parameter Settings";
var	wset_enable="Enable Wireless Access Point";
var	wset_broad="Allow Broadcast of Name (SSID)";
var	wset_network="Wireless Network";
var	wset_name="SSID Name:";
var	wset_region="Region:";
var	wset_chan="Channel:";
var	wset_mode="Mode:";
var	wset_basic_set="VAP Basic Settings";
var	wset_rate="Transmission Rate:";
var	wset_beacon_interval="Beacon Interval:";
var	wset_label_range="(Default: 100, Range: 25 - 1000)";
var	wset_help="<a href=\"javascript:void(openWin('wirelesssettingen_help.htm', 650, 500));\">Help>></a>";
var	wset_apply="<input type=\"submit\" value=\"Apply\" >";
var	wset_sta_apply="<input type=\"button\" name=\"sta_conf\" value=\"Apply\" onclick=\"submit_sta()\">";
var wset_alert_channel="if you select it,you must restart ap to take effect!";
var wset_selectchan="The channel of selected AP£º";
var wset_assocfail="Select AP by the weight of associate fail£º";
var wset_assocrssi="Select AP by the max rate£º";
var wset_alert_illchar=" have illegal characters,must be [0-9] .";
var wset_alert_range=" value is out of range .";
var wset_alert_c1213="If you want channel 12 and 13 valid,please restart later.";
var wset_alert_ssid="Please input ssid properly";

/***************************/
/*  wireless_security_language  */
/***************************/
var	sec_wst="Wireless Security Type";
var	sec_wep="WEP";
var	sec_mod="Mode:";
var	sec_wepencry="Encryption:";
var	sec_etype="Encryption Type:";
var	sec_weppass="WEP Passphrase:";
var	sec_gen="<input type=\"button\" value=\"Generate\" name=\"Generate\" onclick=\"genstr()\">";
var	sec_key1="WEP Key 1:";
var	sec_key2="WEP Key 2:";
var	sec_key3="WEP Key 3:";
var	sec_key4="WEP Key 4:";
var	sec_wpa="WPA";
var	sec_wpawith="WPA With:";
var	sec_wpaencry="Encryption:";
var	sec_psk="PSK Settings";
var	sec_wpapass="Passphrase:";
var	sec_cert_type="Certificate Type:";
var	sec_sec_type="Security Type:";
var	sec_cert_serverip="Certificate Server IP Address:";
var	sec_cert_as="AS Certificate:";
var	sec_cert_ap="AP Certificate:";
var sec_cert_ca="CA Certificate:";
var sec_alert_wapi_fail="wapi erro,please retry!";
var sec_alert_cert_file="please input Certificate file!";
var	sec_alert_p12_fail="p12 password wrong,please retry!";
var sec_p12="p12 enable:";
var sec_p12_pass="password:";
var	sec_generate_button="<input type=\"button\" value=\"Generate\" name=\"Generate\" onclick=\"typeselect()\">";
var	sec_help="<a href=\"javascript:void(openWin('wsecurityen_help.htm', 650, 500));\">Help>></a>";
var	sec_apply="<input type=\"submit\" value=\"Apply\" >";
var sec_alert_key_phrase="Key phrases should not be less than 8.";
var sec_alert_wepk1="Please input WEP Key 1 correctly.";
var sec_alert_wepk2="Please input WEP Key 2 correctly.";
var sec_alert_wepk3="Please input WEP Key 3 correctly.";
var sec_alert_wepk4="Please input WEP Key 4 correctly.";
var sec_alert_length="Please enter the appropriate password length.";

/***************************/
/*  radius_language  */
/***************************/
var	radius_rss="Radius Authentication  Server ";
var	radius_arss="Radius Account Server";
var	radius_sip="Primary Auth Server IP:";
var	radiusc_authport="Authentication Port:";
var	radius_sk="Shared Key:";
var	radius_sip1="Second Auth Server IP:";
var	radiusc_authport1="Authentication Port:";
var	radius_sk1="Shared Key:";
var	radius_aip="Primary Account IP:";
var	radiusc_ap="Account Port:";
var	radius_ask="Shared Key:";
var	radius_aip1="Second Account IP:";
var	radiusc_ap1="Account Port:";
var	radius_ask1="Shared Key:";
var	radius_help="<a href=\"javascript:void(openWin('radiusserveren_help.htm', 650, 500));\">Help>></a>";
var	radius_apply="<input type=\"submit\" value=\"Apply\" >";
var radius_alert_ill=" may have illegal characters or its size great than 5 .";
var radius_alert_range=" value is out of range .";
var radius_alert_ip="Please input IP correctly.";

/***************************/
/*  macfilter_language  */
/***************************/
var	mac_use="Use Filter";
var	mac_filter="MAC Filter";
var	mac_enable="Enable";
var	mac_disable="Disable";
var	mac_mode="Filter Mode";
var	mac_prevent="Prevent clients listed from accessing the wireless network ";
var	mac_permit="Permit only clients listed to access the wireless network";
var	mac_add_button="<input id=\"but\" type=\"button\" name=\"insert_one_row\" value=\"Add\" onclick='addrulecount();urltable_insRow(\"click\");'/>";
var	mac_del_button="<input id=\"but\" type=\"button\" name=\"delete_one_row\" value=\"Delete\" onclick=\"delrulecount();urltable_delRow();\"/>";
var	mac_help="<a href=\"javascript:void(openWin('macfilteren_help.htm', 650, 500));\">Help>></a>";
var	mac_apply="<input type=\"submit\" value=\"Apply\" >";
var mac_alert_max="Max number of filter MAC is 20!";
var mac_alert_atleast="There must be at least one mac-filter entry!";

/***************************/
/*  advance_set_language  */
/***************************/
var adv_shortgi="ShortGI";
var adv_extoffset="Direction of the channel expansion";
var adv_extoffset_0="no expansion";
var adv_extoffset_1="down";
var adv_extoffset_2="up";
var	wset_nonoverlapping="Nonoverlapping 1,6,11 Channel Autoselect";
var	adv_as="Advanced Settings";
var	adv_rate="Rate";
var	adv_rates="(Default: Auto)";
var	adv_beacon="Beacon Interval";
var	adv_beacons="(Default: 100ms, Range: 25 - 1000)";
var	adv_dtim="DTIM Interval";
var	adv_dtims="(Default: 1, Range: 1 - 15)";
var	adv_frag="Fragmentation Threshold";
var	adv_frags="(Default: 2346, Range: 256 - 2346)";
var	adv_diversity="diversity";
var	adv_diversitys="(Defaul: true)";
var	adv_tx="TX Antenna";
var	adv_txs="(Default: Auto)";
var	adv_rx="RX Antenna";
var	adv_rxs="(Default: Auto)";
var	adv_pream="Preamble";
var	adv_preams="(Default: Short)";
var	adv_power="TX Power Condition";
var	adv_power_big="TX Power";
var	adv_powers="(Range: -10 - 0)";
var	adv_powers_big="(Range: 1 - 27dbm)";
var	adv_rts="RTS Threshold";
var	adv_rtss="(Default: 2346, Range: 1 - 2346)";
var	wset_count="Max User Count";
var	wset_isa="User Isolation";
var	adv_wmm="Wireless Multimedia Support Settings ";
var	adv_wmmsupp="WMM Support ";
var	adv_ack="No-Acknowledgement";
var	adv_enable="Enable";
var	adv_disable="Disable";
var	adv_disables="(Default: Disable)";
var	adv_vlan="Vlan";
var	adv_vlans="vlan ";
var	adv_vid="VID";
var	adv_user_set="Associated User Settings";
var	adv_cast_iso="Unicast/Multicast Isolation";
var	adv_help="<a href=\"javascript:void(openWin('advanceseten_help.htm', 650, 500));\">Help>></a>";
var	adv_apply="<input type=\"submit\" value=\"Apply\" >";
var adv_cwwmode="Channel Bandwidth";
var adv_alert_illchar=" have illegal characters,must be [0-9] .";
var adv_alert_range=" value is out of range .";
var adv_must_reboot="Must reboot to make the option become effective!";
/***************************/
/*  wds_set_language  */
/***************************/
var	wds_name="WDS";
var	wds_prop="WDS:";
var	wds_enable="Enable";
var	wds_disable="Disable";
var	wds_role="WDS ROLE:";
var	wds_root="Root";
var	wds_client="Client";
var	wds_ssid="WDS SSID:";
var	wds_chan="WDS Channel:";
var	wds_derived="WDS Derived Interface:";
var	wds_setmac="WDS MAC Setting:";
var wds_attention="Please ensure the channel of vap is same with the other";
var	wds_help="<a href=\"javascript:void(openWin('wdsen_help.htm', 650, 500));\">Help>></a>";
var	wds_apply="<input type=\"submit\" value=\"Apply\"  >";

/***************************/
/*  rogue_set_language  */
/***************************/
var	rogue_create="RogueAP Dectection";
var	rogue_vid="Enable RogueAP Dectection";
var	rogue_select_if="Select Scan Interface:";
var	rogue_refresh="<input type=\"button\"  value=\"Refresh\" name=\"refresh\" onclick=\"myrefresh()\" >";
var	rogue_help="<a href=\"javascript:void(openWin('rogueapen_help.htm', 650, 500));\">Help>></a>";
var	rogue_apply="<input type=\"submit\"  value=\"Apply\"  >";

/***************************/
/*  intelligent_set_language  */
/***************************/
var iset_uplink_detect="Up Link Detection";
var iset_eth0_disable="When eth0 disable:";
var iset_eth1_disable="When eth1 disable:";
var iset_detect_interval="Auto Detect Interval:";
var iset_minutes="&nbsp;Minutes&nbsp;(0-60Minutes)";
var iset_uplink_ip="Up Link IP Address:";
var iset_load_balance="Load Balance";
var iset_lb_enable="Enable:";
var iset_lb_threshold="Threshold:";
var iset_channel_auto="Channel Auto Adjust";
var iset_wifi0_chanauto="Auto Adjust Channel On wifi0:";
var iset_wifi1_chanauto="Auto Adjust Channel On wifi1:";
var iset_adjust_interval="Auto Adjust Interval:";
var iset_power_auto="Power Auto Adjust";
var iset_wifi0_powerauto="Auto Adjust Power On wifi0:";
var iset_wifi1_powerauto="Auto Adjust Power On wifi1:";
var	iset_help="<a href=\"javascript:void(openWin('intelligenceen_help.htm', 650, 500));\">Help>></a>";
