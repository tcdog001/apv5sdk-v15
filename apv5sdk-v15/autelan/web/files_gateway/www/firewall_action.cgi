#!/usr/bin/haserl
<%in /usr/lib/web/libweb.sh %>


<% session_validate %>

<%
my_reboot="FALSE"

case "$FORM_SERVLET" in

# DOS filter
"DOS filter")
	dos_filter
	sys_dos_filter
	cgi_exec dos.htm
	;;
	
# IP filter
"IP filter")
	ip_filter
	sys_ip_filter
	cgi_exec firewall.htm
	;;

# MAC filter
"MAC filter")
	mac_filter
	sys_mac_filter
	cgi_exec mac.htm
	;;
	
# URL filter
"URL filter")
	url_filter
	sys_url_filter
	cgi_exec url.htm
	;;

# virtual server
"virtual server")
	virtual_server
	sys_virtual_server
	cgi_exec port_forwarding.htm
	;;
	
# DMZ
"DMZ")
	dmz_host
	sys_dmz_host
	cgi_exec ip_forwarding.htm
	;;
esac	
%>