#!/usr/bin/haserl --upload-limit=24096 --upload-target=/tmp
<%in /usr/lib/web/libweb.sh %>
<% session_validate %>

<% . /usr/sbin/clearmemory %>

<%
if [ "$IPC_fileupload" == "-ERR" ]; then
    my_action_status="File Upload Failed, Firmware Upgrade Cancelled"
    my_ret="FALSE"
else
    my_ret="TRUE"
fi
%>

<%
if [ "$my_ret" == "TRUE" ]; then
        cgi_exec reboot.htm
    	cp /usr/sbin/sysupgrade /tmp/ > /dev/null 2>&1
    	/tmp/sysupgrade "$FORM_uploadfile" > /dev/null 2>&1
    if [ $? == 0 ]; then
        my_action_status="Firmware Upgrade Successful"
        my_ret="TRUE"
    else
        my_action_status="Firmware Upgrade Failed"
        my_ret="FALSE"
    fi
fi
%>

<%
if [ "$my_ret" == "TRUE" ]; then
%>
    <%in reboot.htm %>
<%
    /sbin/sysreboot &
else
%>
    <%in firmwareupgrade1.htm %>
<%
fi
%>
