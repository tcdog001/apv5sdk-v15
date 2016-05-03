#!/usr/bin/haserl
<%in /usr/lib/web/libweb.sh %>
<% 
case "$FORM_SERVLET" in

"lang")
   lang_check
   cgi_exec index.htm
   ;;
esac 
%>