#!/bin/sh
# upload.sh
# Usage: upload.sh [ipaddr] [timer]
#
TAR=/bin/tar
TFTP=/usr/bin/tftp
TARGET=

if [ $# != 2 ];then 
	echo "input correct ipaddr and timer" 
	exit 1
fi

while true
do
    	TARGET=log.$(date +%y%m%d%H%M).tar.gz
	cd /var
    	$TAR -zcf $TARGET logmsg
    	$TFTP -p -l  $TARGET  $1
	rm -f $TARGET
    	sleep $2
done
 
