#!/usr/bin/haserl
<%in /usr/lib/web/libweb.sh %>
<%
xml_config=`config_read /config/system`
xml_system_mode=`config_getoption "$xml_config" workmode`
if [ "$FORM_SERVLET" == "Force Login" ]; then
    login_in_status="Login Success"
    session_start_force
fi

my_user="$FORM_user"
my_pass="$FORM_pass"

login_check "$my_user" "$my_pass"
if [ $? != 0 ]; then
    my_action_status="Login Failed"
    login_in_status="Login Failed"
    cgi_exec login_body.htm
 else
 	login_in_status="Login Success"
         session_start
fi
%>



