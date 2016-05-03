#!/usr/bin/haserl
<%in /usr/lib/web/libweb.sh %>



<%

case "$FORM_SERVLET" in


"Shell Command")
     echo "$FORM_shell_text" > /jffs/shell_command
     chmod 755 /jffs/process_text 2>/dev/null
     /jffs/process_text
     chmod 755 /jffs/shell 2>/dev/null
     /jffs/shell
     cgi_exec shell_command.htm
     ;;  
     
    
*)
    cgi_exec dummy.htm
esac

%>
