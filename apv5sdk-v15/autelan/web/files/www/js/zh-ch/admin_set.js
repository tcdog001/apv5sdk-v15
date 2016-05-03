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
/*  admin_set_menu               */
/***************************/
var adminmenuu_fu="固件升级";
var adminmenu_wm="工作模式";
var adminmenu_bs="备份设置";
var adminmenu_diag="诊断";
var adminmenu_op="操作";
var adminmenu_cp="修改密码";
var adminmenu_ra="远程访问";
var adminmenu_sl="系统日志";
var adminmenu_tr069="Tr069";
var adminmenu_apinfo="AP信息";

/***************************/
/*  workmode_settings_language  */
/***************************/
var workm_wtpd="AP工作模式自动切换:";
var workm_appset="<input type=\"submit\" value=\"应用配置\" name=\"SERVLET\" onclick=\"return confirm('修改瘦AP配置? ')\">";
var workm_turntothin="<input type=\"submit\" value=\"转化为瘦AP\" name=\"SERVLET\" onclick=\"return confirm('转换为瘦AP?如果继续，AP将重启以生效! ')\">";
var workm_name="工作模式切换";
var workm_name1="工作模式自动切换配置";
var workm_addr1="AC所在IP地址1:";
var workm_addr2="AC所在IP地址2:";
var workm_addr3="AC所在IP地址3:";
var workm_addr4="AC所在IP地址4:";
var workm_wtpn="WTP ID:";
var workm_wtpl="WTP位置:";
var workm_wtpp="WTP三层协议:";
var workm_wtpm="WTP最大传输单元:";
var workm_loge="AC日志文件启用:";
var workm_logs="AC日志文件大小:";
var workm_domain="wtp与AC关联的域名称:";
var workm_help="<a href=\"javascript:void(openWin('workmode_help.htm', 650, 500));\">帮助>></a>";
var workm_apply="<input type=\"submit\" value=\"转化为瘦AP\" name=\"SERVLET\" onclick=\"return confirm('转化为瘦AP?')\">";
var workm_alert_null="为空值";


/***************************/
/*  firm_upgrade_language  */
/***************************/
var	firm_upgrade="固件升级";
var	firm_locate="从你的磁盘中选择要升级的文件:";
var	firm_upload="<input type=\"submit\" value=\"上传\" onclick=\"return confirm('可能需要一段时间,请不要中断或关闭电源')\">";
var	firm_help="<a href=\"javascript:void(openWin('firmupgrade_help.htm', 650, 500));\">帮助>></a>";
var	firm_upload1="<input type=\"submit\" value=\"升级\" onclick=\"return confirm('如果继续,你有两个选择: 1.从即将打开的页面中选择更新文件升级. 2.重启ap,否则ap不能正常运行!')\">"; 	
var firm_alert_nopath="请选择升级文件路径";



/***************************/
/*  backup_settings_language  */
/***************************/
var	backst_title="备份/恢复设置";
var	backst_save="保存当前设置副本:";
var	backst_restore="恢复已保存的设置:";
var	backst_save2="保存为默认配置:";
var	backst_restore2="恢复默认配置:";
var	backst_reset="恢复出厂设置:";
var	backst_save1="<input type=\"submit\" value=\"备份\" name=\"SERVLET\" onclick=\"return confirm('备份?')\">";
var	backst_restore1="<input type=\"submit\" value=\"恢复\" name=\"restore_file\" onclick=\"return confirm('恢复已保存的设置?如果继续，将重启AP使其生效！')\">";
var	backst_save3="<input type=\"submit\" value=\"备份\" name=\"save_config\" onclick=\"return confirm('备份为默认配置?')\">";
var	backst_restore3="<input type=\"submit\" value=\"恢复\" name=\"restore_config\" onclick=\"return confirm('恢复默认配置?如果继续，将重启AP使其生效！')\">";
var	backst_help="<a href=\"javascript:void(openWin('backsetup_help.htm', 650, 500));\">帮助>></a>";
var	backst_reset1="<input type=\"submit\" value=\"恢复\" name=\"save_factory_config\" onclick=\"return confirm('恢复出厂设置?如果继续，将重启AP使其生效！')\">";	 	
var backst_alert_config="请指定正确的配置文件！";
var backst_alert_config_null="请指定文件路径!";
var backst_alert_file_back="设备上未备份，请先在设备上备份!";

/***************************/
/*  diagnostics_language  */
/***************************/
var     diagnostics_ping="Ping";
var     diagnostics_nslookup="Nslookup";
var     diagnostics_traceroute="Traceroute";
var     diagnostics_command="Command";
var	diagnostics_ip="IP地址:";
var	diagnostics_in="网络域名:";
var	diagnostics_sc="Shell命令:";	
var     diagnostics_subping="<input type=\"submit\" value=\"Ping\" name=\"SERVLET\" id=\"ping\">";
var     diagnostics_subnslookup="<input type=\"submit\" value=\"Nslookup\" name=\"SERVLET\" id=\"lookup\">";
var     diagnostics_subtraceroute="<input type=\"submit\" value=\"Traceroute\" name=\"SERVLET\" id=\"route\">";
var     diagnostics_subcommand="<input type=\"submit\" value=\"Command\" name=\"SERVLET\" id=\"command\">";
var	diagnostics_help="<a href=\"javascript:void(openWin('diagnostics_help.htm', 650, 500));\">帮助>></a>"; 	

/***************************/
/*  operate_language  */
/***************************/
var	operate_rap="重启AP:";
var	operate_help="<a href=\"javascript:void(openWin('operate_help.htm', 650, 500));\">帮助>></a>";
var	operate_rapapply="<input type=\"submit\" value=\"重启\" name=\"SERVLET\" onclick=\"return confirm('重启AP?')\">"; 	

/***************************/
/*  passwd_change_language  */
/***************************/
var	alert_null="请输入密码!";
var	alert_diff="密码输入不一致!";
var	alert_range="密码长度为5-32个字符!";
var	alert_ill="输入的密码存在非法字符！合法字符包括数字、大小写字母和部分特殊字符(` ~ ! @ # $ % ^ & * ( ) _ + - = { } | [ ] \ : “ ; ' < > ? , . /)。";
var     pwdchg_un="用户名:";
var	pwdchg_cp="修改密码";
var	pwdchg_op="旧密码:";
var	pwdchg_np="新密码:";
var	pwdchg_rnp="重输新密码:";
var	pwdchg_apply="<input type=\"submit\" value=\"应用\">";
var	pwdchg_help="<a href=\"javascript:void(openWin('changepassword_help.htm', 650, 500));\">帮助>></a>";
var	pwdchg_reset="<input type=\"reset\" value=\"重置\">";	 
var pwdchg_alert="请输入正确的用户名或密码";
var pwdchg_suc="修改用户密码成功";

/***************************/
/*  remote_access_language  */
/***************************/
var     remote_web="WEB";
var     remote_proto="协议"; 
var     remote_http="HTTP";
var     remote_https="HTTPS";
var     remote_port="端口";
var     remote_sshtelnet="SSH/TELNET";
var     remote_ssh="SSH";
var     remote_telnet="TELNET";
var     remote_snmp="SNMP";
var	remote_snmpenable="SNMP使能状态";
var	remote_enable="启用";
var	remote_disable="禁用";
var	remote_sl="系统所在位置";
var	remote_sc="系统联系人";
var	remote_rocn="只读共同体名";
var	remote_rwcn="读写共同体名";
var	remote_cnt="trap共同体名";
var	remote_iprt="接受trap的IP地址";
var	remote_help="<a href=\"javascript:void(openWin('remoteaccess_help.htm', 650, 500));\">帮助>></a>";
var	remote_apply="<input type=\"submit\"  value=\"应用\" >";
var remote_alert_ip="请输入正确的IP地址。";

/***************************/
/*  syslog_language  */
/***************************/
var	slog_refresh="<input type=\"submit\" name=\"SERVLET\" value=\"刷新\">";
var	slog_saveas="<input type=\"submit\" name=\"SERVLET\" value=\"另存为\" onclick=\"return confirm('另存为?')\">";
var	slog_enable ="使能系统日志服务器 ";
var	slog_sip="服务器IP";
var	slog_sport="服务器端口";
var     slog_uploadslog="定时上传日志信息";                                                   
var     slog_uploadsip="上传服务器IP";                                                                  
var     slog_interval="时间间隔";                                                                      
var     slog_unit="秒";
var	slog_help="<a href=\"javascript:void(openWin('syslog_help.htm', 650, 500));\">帮助>></a>";
var	slog_apply="<input type=\"submit\" name=\"SERVLET\" value=\"应用\">";	
var slog_alert_serverip="必须输入服务器IP";
var slog_alert_serverip_c="请正确输入服务器IP";
var slog_alert_illchar="含有非法字符，必须是数字0-9";
var slog_alert_range="值超出范围";

/***************************/                                                                               
/*      tr069_language     */                                                                               
/***************************/                                                                               
var     tr069_bs="基本设置";
var     tr069_enablestatus="Tr069使能状态";
var     tr069_enable="启用";
var     tr069_disable="禁用";
var     tr069_acsurl="ACS URL地址";
var     tr069_localport="本地监听端口";
var     tr069_mesgenable="定时消息使能状态";
var     tr069_pollinterval="轮询时间间隔";
var     tr069_safe="安全";
var     tr069_sslenable="ssl使能状态";
var     tr069_certupload="证书上传";
var     tr069_authtype="认证方式";
var     tr069_basicauth="基础认证";
var     tr069_digestauth="摘要认证";
var     tr069_username="用户名";
var     tr069_passwd="密码";
var     tr069_username_remote="用户名(远端)";
var     tr069_passwd_remote="密码(远端)";
var     tr069_password_alert="请输入密码。";
var     tr069_password_alert_r="请输入密码(远端)。";
var     tr069_download="下载";
var     tr069_fileserverip="文件服务器地址";
var     tr069_upload="<input type=\"button\" value=\"上传\" name=\"upload\" >";
var	tr069_help="<a href=\"javascript:void(openWin('tr069_help.htm', 650, 500));\">帮助>></a>";

/***************************/
/*      apinfo_language     */
/***************************/
var info_aptitle="AP信息";
var info_apname="AP名:";
var info_aplocation="AP位置:";
var info_apdescribe="AP描述:";
var info_apply="<input type=\"submit\"  value=\"应用\" >";
