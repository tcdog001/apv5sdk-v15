#!/usr/bin/haserl
<%in /usr/lib/web/libweb.sh %>
<%
if [ "$FORM_SERVLET" == "Apply" ]; then
 my_action_status="Login Failed"
 cgi_exec cgi-bin/as_query.cgi 
fi
%>



