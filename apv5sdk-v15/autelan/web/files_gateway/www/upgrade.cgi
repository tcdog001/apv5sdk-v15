#!/usr/bin/haserl --upload-limit=24096 --upload-target=/tmp
<%in /usr/lib/web/libweb.sh %>
<% session_validate %>

<%
echo $IPC_fileupload >/tmp/aaa
if [ "$IPC_fileupload" == "-ERR" ]; then
    my_action_status="File Upload Failed, Firmware Upgrade Cancelled"
    my_ret="FALSE"
else
    my_ret="TRUE"
fi
%>

<%
if [ "$my_ret" == "TRUE" ]; then
    cgi_exec upgrade.htm
    /usr/sbin/sysupgrade "$FORM_uploadfile" > /dev/null 2>&1
    echo "aa"
    if [ $? == 0 ]; then
        my_action_status="Firmware Upgrade Successful"
        my_ret="TRUE"
    else
        my_action_status="Firmware Upgrade Failed"
        my_ret="FALSE"
    fi
fi
%>

<%in upgrade.htm %>

<%
if [ "$my_ret" == "TRUE" ]; then
		/sbin/sysreboot &
fi
%>
