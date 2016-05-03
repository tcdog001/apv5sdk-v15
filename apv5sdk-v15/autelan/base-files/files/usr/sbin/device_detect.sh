#!/bin/sh
# device_detect.sh
# Usage: device_detect.sh
#
GREP=/bin/grep
IFCONFIG=/sbin/ifconfig
SEND_TRAP=/usr/sbin/send_trap

while true
do
$IFCONFIG | $GREP 'wifi0' >/dev/null
if [ $? != 0 ] ;then
   $SEND_TRAP apWiFiFaultAlert
fi
done

