#!/usr/bin/haserl --upload-limit=24096 
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
    #echo $FORM_ng > /tmp/upgrade
    if [ "$FORM_ng" == "ng"  ]; then
    	writeflash
    	rm -rf /jffs/thinapflag
	#echo "write flash OK."
	cgi_exec reboot.htm
	cp /usr/sbin/ngupgrade /tmp/ > /dev/null 2>&1
    	/tmp/ngupgrade "$FORM_uploadfile" > /dev/null 2>&1
    	#echo aa >> /tmp/upgrade
    else
		cgi_exec reboot.htm
    	cp /usr/sbin/sysupgrade /tmp/ > /dev/null 2>&1
    	/tmp/sysupgrade "$FORM_uploadfile" > /dev/null 2>&1
    fi
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
