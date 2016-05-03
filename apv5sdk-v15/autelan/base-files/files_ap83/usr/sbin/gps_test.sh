#!/bin/sh


head -n 60 /dev/ttyS1 > /tmp/gps_info 

GPS_MESSAGE=`awk -F',' '{if (NR >=6 && NR <= 26) {print $1} }' /tmp/gps_info  | sort -k1.2`

for i in $GPS_MESSAGE
do
	if [ $i = "\$GPGGA" -o $i = "\$GPGLL" -o $i = "\$GPGSA" -o $i = "\$GPGSV" -o $i = "\$GPRMC" -o $i = "\$GPVTG" ]
	then
		continue
	else
		echo -e "*** GPS MODULE TEST FAILED. ***"
		echo -e "*** GPS INFO: ***"
		cat /tmp/gps_info
		exit 0
	fi
done

echo -e "*** GPS MODULE TEST SUCCESSFULLY. ***"

echo -e "*** GPS INFO: ***"
cat /tmp/gps_info
