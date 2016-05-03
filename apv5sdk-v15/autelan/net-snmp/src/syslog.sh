

start(){
    syslogd -f /etc/IWNCOMM/snmp/syslog.conf
    return 0
}
stop(){
    killall syslogd
    return 0
}
restart(){
    stop
    start
}

case "$1" in
    start)
	start
	;;
    stop)
	stop
	;;
    restart)	
	stop
	start
	;;
    *)    
	echo $"Usage: syslog.sh start | stop | restart"
	exit 1

esac

exit $?

