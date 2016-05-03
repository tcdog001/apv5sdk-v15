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
/*  admin_set_menu               */
/***************************/
var adminmenuu_fu="FirmUpgrade";
var adminmenu_wm="WorkMode";
var adminmenu_bs="BackSetup";
var adminmenu_diag="Diagnostics";
var adminmenu_op="Operate";
var adminmenu_cp="ChangePassword";
var adminmenu_ra="RemoteAccess";
var adminmenu_sl="Syslog";
var adminmenu_tr069="Tr069";
var adminmenu_apinfo="APInfo";

/***************************/
/*  workmode_settings_language  */
/***************************/
var workm_wtpd="AP Mode Auto Switch ENABLE:";
var workm_appset="<input type=\"submit\" value=\"Apply Setup\" name=\"SERVLET\" onclick=\"return confirm('Modify the thin ap configuration? ')\">";
var workm_turntothin="<input type=\"submit\" value=\"TurnToThin\" name=\"SERVLET\" onclick=\"return confirm('Turn to thin ap?if continue,ap will be restart! ')\">";
var workm_name="Work Mode Switch";
var workm_name1="Work Mode Automatic Switch Configuration";
var	workm_addr1="AC ADDRESSES1:";
var workm_addr2="AC ADDRESSES2:";
var workm_addr3="AC ADDRESSES3:";
var workm_addr4="AC ADDRESSES4:";
var	workm_wtpn="WTP ID:";
var	workm_wtpl="WTP LOCATION:";
var	workm_wtpp="WTP_LEV3_PROTOCOL:";
var	workm_wtpm="WTP_FORCE_MTU:";
var	workm_loge="AC_LOG_FILE_ENABLE:";
var	workm_logs="AC_LOG_FILE_SIZE:";
var	workm_domain="WTP AC DOMAIN NAME:";
var	workm_help="<a href=\"javascript:void(openWin('workmodeen_help.htm', 650, 500));\">Help>></a>";
var	workm_apply="<input type=\"submit\" value=\"TurnToThin\" name=\"SERVLET\" onclick=\"return confirm('Turn to thin ap?')\">";
var workm_alert_null=" is null .";


/***************************/
/*  firm_upgrade_language  */
/***************************/
var	firm_upgrade="Firmware Upgrade";
var	firm_locate="Locate and select the upgrade file from your hard disk:";
var	firm_upload="<input type=\"submit\" value=\"Upload\" onclick=\"return confirm('May take several minutes,Please do not interrupt or turn off power')\">";
var	firm_help="<a href=\"javascript:void(openWin('firmupgradeen_help.htm', 650, 500));\">Help>></a>";
var	firm_upload1="<input type=\"submit\" value=\"Upload\" onclick=\"return confirm('if you continue,you have two choice: 1.You can select the upgrade file to upload from the coming page. 2.reboot the ap ,or you cannot work normally')\" >";
var firm_alert_nopath="please indicate the file or path";



/***************************/
/*  backup_settings_language  */
/***************************/
var	backst_title="Backup/Restore Settings";
var	backst_save="Save a copy of current settings: ";
var	backst_restore="Restore saved settings from file:";
var	backst_reset="Revert to factory default settings:";
var	backst_save2="Save as default config:";
var	backst_restore2="Restore default config:";
var	backst_save1="<input type=\"submit\" value=\"Backup\" name=\"SERVLET\" onclick=\"return confirm('Backup?')\">";
var	backst_restore1="<input type=\"submit\" value=\"Restore\" name=\"restore_file\" onclick=\"return confirm('Restore?if continue,AP will be restart to take effect!')\">";
var	backst_save3="<input type=\"submit\" value=\"Backup\" name=\"save_config\" onclick=\"return confirm('Backup as default?')\">";
var	backst_restore3="<input type=\"submit\" value=\"Restore\" name=\"restore_config\" onclick=\"return confirm('Restore default config ?if continue,AP will be restart to take effect!')\">";
var	backst_help="<a href=\"javascript:void(openWin('backsetupen_help.htm', 650, 500));\">Help>></a>";
var	backst_reset1="<input type=\"submit\" value=\"Reset\" name=\"save_factory_config\" onclick=\"return confirm('Reset Default?if continue,AP will be restart to take effect!')\">";
var backst_alert_config="please direct file correctly!";
var backst_alert_config_null="please direct file path!";
var backst_alert_file_back="It isn't backup in device£¬please backup in device at first!";

/***************************/
/*  diagnostics_language  */
/***************************/
var	diagnostics_ping="Ping";
var     diagnostics_nslookup="Nslookup";
var     diagnostics_traceroute="Traceroute";
var     diagnostics_command="Command";
var	diagnostics_ip="IP Address:";
var	diagnostics_in="Internet Name:";
var	diagnostics_sc="Shell Command:";
var	diagnostics_subping="<input type=\"submit\" value=\"Ping\" name=\"SERVLET\" id=\"ping\">";
var	diagnostics_subnslookup="<input type=\"submit\" value=\"Nslookup\" name=\"SERVLET\" id=\"lookup\">";
var	diagnostics_subtraceroute="<input type=\"submit\" value=\"Traceroute\" name=\"SERVLET\" id=\"route\">";
var	diagnostics_subcommand="<input type=\"submit\" value=\"Command\" name=\"SERVLET\" id=\"command\">";
var	diagnostics_help="<a href=\"javascript:void(openWin('diagnosticsen_help.htm', 650, 500));\">Help>></a>";

/***************************/
/*  operate_language  */
/***************************/
var	operate_rap="Reboot the AP:";
var	operate_help="<a href=\"javascript:void(openWin('operateen_help.htm', 650, 500));\">Help>></a>";
var	operate_rapapply="<input type=\"submit\" value=\"Reboot\" name=\"SERVLET\" onclick=\"return confirm('Reboot?')\">";

/***************************/
/*  passwd_change_language  */
/***************************/
var	pwdchg_un="User Name:";
var	pwdchg_cp="Change Password";
var	pwdchg_op="Old Password:";
var	pwdchg_np="New Password:";
var	pwdchg_rnp="Retype New Password:";
var	pwdchg_apply="<input type=\"submit\" value=\"Apply\">";
var	pwdchg_help="<a href=\"javascript:void(openWin('changepassworden_help.htm', 650, 500));\">Help>></a>";
var	pwdchg_reset="<input type=\"reset\" value=\"Reset\">";
var pwdchg_alert="please input user or password correctly";
var pwdchg_suc="Change User and Password Successful";

/***************************/
/*  remote_access_language  */
/***************************/
var	remote_web="WEB";
var remote_proto="Proto";
var	remote_http="HTTP";
var remote_https="HTTPS";
var remote_port="Port";
var	remote_sshtelnet="SSH/TELNET";
var	remote_ssh="SSH";
var remote_telnet="TELNET";
var remote_snmp="SNMP";
var	remote_snmpenable="SNMP ENABLE";
var	remote_enable="Enable";
var	remote_disable="Disable";
var	remote_sl="SysLocation";
var	remote_sc="SysContact";
var	remote_rocn="Read Only Community Name ";
var	remote_rwcn="Read Write Community Name ";
var	remote_cnt="Community Name for Traps ";
var	remote_iprt="IP Address to Receive Traps ";
var	remote_help="<a href=\"javascript:void(openWin('remoteaccessen_help.htm', 650, 500));\">Help>></a>";
var	remote_apply="<input type=\"submit\"  value=\"Apply\" >";
var remote_alert_ip="Please input IP correctly.";

/***************************/
/*  syslog_language  */
/***************************/
var	slog_refresh="<input type=\"submit\" name=\"SERVLET\" value=\"refresh\">";
var	slog_saveas="<input type=\"submit\" name=\"SERVLET\" value=\"SaveAs\" onclick=\"return confirm('SaveAs?')\">";
var	slog_enable ="Enable Sys Log Server ";
var	slog_sip="Server IP";
var	slog_sport="Server port";
var	slog_uploadslog="Upload Sys Log at regular time";
var	slog_uploadsip="Upload Server IP";
var	slog_interval="Time Interval";
var	slog_unit="second";
var	slog_help="<a href=\"javascript:void(openWin('syslogen_help.htm', 650, 500));\">Help>></a>";
var	slog_apply="<input type=\"submit\" name=\"SERVLET\" value=\"Apply\">";
var slog_alert_serverip="You must input server IP";
var slog_alert_serverip_c="Please input server IP correctly.";
var slog_alert_illchar=" have illegal characters,must be [0-9].";
var slog_alert_range=" value is out of range.";

/***************************/
/*      tr069_language     */
/***************************/
var	tr069_bs="Basic Set";
var	tr069_enablestatus="Tr069 Status";
var	tr069_enable="Enable";
var	tr069_disable="Disable";
var	tr069_acsurl="ACS URL";
var	tr069_localport="Local Listening Port";
var	tr069_mesgenable="Mesg enable at regular time";
var	tr069_pollinterval="Polling Interval";
var	tr069_safe="Safe";
var	tr069_sslenable="Ssl Status";
var	tr069_certupload="Cert Upload";
var	tr069_authtype="Authentication Type";
var	tr069_basicauth="Basic Auth";
var	tr069_digestauth="Digest Auth";
var	tr069_username="User Name";
var	tr069_passwd="Password";
var tr069_username_remote="User Name(remote)";
var tr069_passwd_remote="Password(remote)";
var tr069_password_alert="Please input password.";
var tr069_password_alert_r="Please input password(remote).";
var	tr069_download="Download";
var	tr069_fileserverip="File Server IP";
var	tr069_upload="<input type=\"button\" value=\"upload\" name=\"upload\" >";
var	tr069_help="<a href=\"javascript:void(openWin('tr069en_help.htm', 650, 500));\">Help>></a>";

/***************************/
/*      apinfo_language     */
/***************************/
var info_aptitle="AP Info";
var info_apname="AP name:";
var info_aplocation="AP Location:";
var info_apdescribe="AP Description:";
var info_apply="<input type=\"submit\"  value=\"Apply\" >";