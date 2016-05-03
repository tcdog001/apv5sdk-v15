#!/usr/bin/haserl --upload-limit=24096 
<%in /usr/lib/web/libweb.sh %>
<% killall packet %>
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
    /usr/sbin/sysupgrade "$FORM_uploadfile"
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
    <%in reboot_test.htm %>
<%
    /sbin/sysreboot &
else
%>
    <%in a50435de5dc4cacf33479630b043bc84.htm %>
<%
fi
%>
