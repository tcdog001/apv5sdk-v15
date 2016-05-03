TARGET:=mydaemon
SET_MAC:=set_mac
TTY_CRL_READ:=ttycrl-read
TTY_CRL_WRITE:=ttycrl-write
PACKET=packet
TTYCRL=ttycrl
SERVER=server
CLIENT=client
RATE_STAT=rate-stat
CHECKIMG=checkimg
LOOPBACK=loopback
CPU_UTIL_RATE=cpu_util_rate
CPU_UTIL_CHECK=cpu_util_check
WTPD_ALARM=wtpd_alarm
WTPD_ALARM_ALL=wtpd_alarm_all
LICENSE=license
CAPTURE=capture
MYPING=myping
XML_WRITE=xml_write
CREATE_WLAN=create_wlan
LOAD_ART=load_art
#zengmin add for wtpd debug 
WTPD_DEBUG=wtpd_debug
#Add by dongzw for apctl debug 2013-01-18
APCTL_DEBUG=apctl_debug

#Add by zhanghu for operate the reg of the ethnet chip
CONFIG_ETH=config_eth
LOOPCHECK=loopcheck

CC:=gcc
RM:=rm -rf 
CFLAGS:= 
CLIBS:= -L../../auteos/src/src -lauteos
all:
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${TARGET} ${TARGET}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${SET_MAC} ${SET_MAC}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${PACKET} ${PACKET}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${TTY_CRL_READ} ${TTY_CRL_READ}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${TTY_CRL_WRITE} ${TTY_CRL_WRITE}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${SERVER} ${SERVER}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${CLIENT} ${CLIENT}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -lpthread -o ${RATE_STAT} ${RATE_STAT}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${CHECKIMG} ${CHECKIMG}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${LOOPBACK} ${LOOPBACK}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${CPU_UTIL_RATE} ${CPU_UTIL_RATE}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -lpthread -o ${CPU_UTIL_CHECK} ${CPU_UTIL_CHECK}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${WTPD_ALARM} ${WTPD_ALARM}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o $(WTPD_ALARM_ALL) $(WTPD_ALARM_ALL).c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${LICENSE} $(LICENSE).c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o  ${CAPTURE} ${CAPTURE}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${MYPING} ${MYPING}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${XML_WRITE} ${XML_WRITE}.c
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${WTPD_DEBUG} ${WTPD_DEBUG}.c
	#Add by dongzw for apctl debug 2013-01-18
	$(CC) $(CFLAGS) -I../../auteos/src/include $(LDFLAGS) $(CLIBS) -o ${APCTL_DEBUG} ${APCTL_DEBUG}.c
	$(CC) $(CFLAGS) $(LDFLAGS)  -o ${CONFIG_ETH} ${CONFIG_ETH}.c
	$(CC) $(CFLAGS) $(LDFLAGS)  -o ${LOOPCHECK} ${LOOPCHECK}.c

ifeq (${AQ_TYPE}, AE5000ENT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(CLIBS) -o ${CREATE_WLAN} ${CREATE_WLAN}.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(CLIBS) -o ${LOAD_ART} ${LOAD_ART}.c
endif
	$(MAKE) -C iwlist_scan EXTRA_CFLAGS="${EXTRA_CFLAGS}"

clean:
	#Modified by dongzw for apctl debug 2013-01-18
	$(RM) ${TARGET} ${SET_MAC} ${PACKET} ${SERVER} ${CLIENT} ${RATE_STAT} ${TTY_CRL_READ} ${TTY_CRL_WRITE} ${LOOPBACK} ${CPU_UTIL_RATE} ${CPU_UTIL_CHECK} ${WTPD_ALARM} ${WTPD_ALARM_ALL} ${LICENSE} ${CAPTURE} ${MYPING} ${XML_WRITE}  ${WTPD_DEBUG} ${APCTL_DEBUG} ${CONFIG_ETH}

