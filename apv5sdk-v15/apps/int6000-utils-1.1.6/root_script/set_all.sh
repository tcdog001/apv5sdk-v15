#!/bin/sh
. /tmp/.apcfg
if [ ! -f /tmp/.apcfg ];then
echo "config file does not exist"
else
MAC0=${REM_MAC0}
MAC1=${REM_MAC1}
MAC2=${REM_MAC2}
MAC3=${REM_MAC3}
MAC4=${REM_MAC4}
MAC5=${REM_MAC5}
MAC6=${REM_MAC6}
MAC7=${REM_MAC7}
DAKEY0=$(hpavkey -D ${PASSWD0})
NMKEY=$(hpavkey -M ${NMK})
FORWARD=$(cat /proc/sys/net/ipv4/ip_forward)
echo ${MAC0}
echo ${MAC1}
echo ${MAC2}
echo ${MAC3}
echo ${MAC4}
echo ${MAC5}
echo ${MAC6}
echo ${MAC7}
echo ${DAKEY}
echo ${NMKEY}
if [ 0 = ${FORWARD} ]; then
IF=br0
else
IF=eth0
fi
if [ ! -z ${MAC0} ]; then
int6k -i ${IF} -J ${MAC0} -K ${NMKEY} -D ${DAKEY0}
fi
if [ ! -z ${MAC1} ]; then
int6k -i ${IF} -J ${MAC1} -K ${NMKEY} -D ${DAKEY1}
fi
if [ ! -z ${MAC2} ]; then
int6k -i ${IF} -J ${MAC2} -K ${NMKEY} -D ${DAKEY2}
fi
if [ ! -z ${MAC3} ]; then
int6k -i ${IF} -J ${MAC3} -K ${NMKEY} -D ${DAKEY3}
fi
if [ ! -z ${MAC4} ]; then
int6k -i ${IF} -J ${MAC4} -K ${NMKEY} -D ${DAKEY4}
fi
if [ ! -z ${MAC5} ]; then
int6k -i ${IF} -J ${MAC5} -K ${NMKEY} -D ${DAKEY5}
fi
if [ ! -z ${MAC6} ]; then
int6k -i ${IF} -J ${MAC6} -K ${NMKEY} -D ${DAKEY6}
fi
if [ ! -z ${MAC7} ]; then
int6k -i ${IF} -J ${MAC7} -K ${NMKEY} -D ${DAKEY7}
fi
int6k -i ${IF} -MK ${NMKEY}
 
fi
