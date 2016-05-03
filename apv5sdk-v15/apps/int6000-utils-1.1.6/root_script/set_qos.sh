#!/bin/sh
. /tmp/.apcfg
FORWARD=$(cat /proc/sys/net/ipv4/ip_forward)
if [ 0 = ${FORWARD} ]; then
IF=br0
else
IF=eth0
fi
if [ -f /tmp/.apcfg ];then
if [ ! -z ${DEST_MAC} ]; then
#echo "MAC set"
int6krule ${DEST_MAC_PRI} any ethda is ${DEST_MAC} add temp -i ${IF}
fi
if [ ! -z ${DEST_IPADDR} ]; then
#echo "IP set"
int6krule ${DEST_IPADDR_PRI} any IPv4DA is ${DEST_IPADDR} add temp -i ${IF}
fi
if [ ! -z ${DEST_PORT} ]; then
#echo "port set"
int6krule ${DEST_PORT_PRI} any IPDP is ${DEST_PORT} add temp -i ${IF}
fi
if [ 1 = ${VLAN_MODE} ]; then
	VAL=0
	for VLAN in ${VLAN0_PRI} ${VLAN1_PRI} ${VLAN2_PRI} ${VLAN3_PRI} ${VLAN4_PRI} ${VLAN5_PRI} ${VLAN6_PRI} ${VLAN7_PRI}
	do
		int6krule ${VLAN} any VLANUP is ${VAL} add temp -i ${IF}
		VAL=$(($VAL+1))
	done
fi
if [ 1 = ${TOS_MODE} ]; then
	VAL=0
	for TOS in ${TOS0_PRI} ${TOS1_PRI} ${TOS2_PRI} ${TOS3_PRI} ${TOS4_PRI} ${TOS5_PRI} ${TOS6_PRI} ${TOS7_PRI}
	do
		int6krule ${TOS} any VLANUP is ${VAL} add temp -i ${IF}
		VAL=$(($VAL+1))
	done
fi
fi
