#!/bin/sh
. /tmp/.apcfg
NMKEY=$(hpavkey -M ${NMK})
FORWARD=$(cat /proc/sys/net/ipv4/ip_forward)
#echo ${NMKEY}
#echo ${FORWARD}
if [ 0 = ${FORWARD} ]; then
IF=br0
else
IF=eth0
fi
if [ ! -z ${NMKEY} ]; then
int6k -i ${IF} -MK ${NMKEY}
fi
