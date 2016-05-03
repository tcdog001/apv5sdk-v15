#!/bin/sh
LOG=/jffs/lte_upgrade_log
echo -n "" > $LOG

usage ()
{
echo "
**************************************************
This shell file needs two parameters:
First  param:	The image name
Second param:	IP of the tftp server
For example: upgrade_lte firmware.tar.bz2 10.1.1.1
**************************************************
"
}

up_grade()
{
backup_flag=0
for i in `seq 2`
do
echo "`adb shell upgrader backup | grep -o 'successfully' | wc -l`" > num
if [ `cat num` -ne 3 ];then
	echo "backup error" >> $LOG
	let backup_flag=backup_flag+1
	if [ $backup_flag -eq 2 ];then
		return 1
	fi
	continue
fi
break
done

if [ -e /tmp/firmware.tar.bz2 ];then
	adb push /tmp/firmware.tar.bz2 /usr >/dev/null
	if [ $? -ne 0 ];then
		echo "adb push error" >> $LOG
		return 1
	fi
else
	echo "img not exist" >> $LOG
	return 1
fi

adb shell tar -xjvf /usr/firmware.tar.bz2 --to-command=upgrader >/dev/null 2>&1
if [ $? -ne 0 ];then
	echo "tar error" >> $LOG
	return 1
fi

set_flag="`adb shell upgrader set | grep 'successfully' | echo $?`"
if [ $set_flag -ne 0 ];then
	echo "upgrader set error">>$LOG
	return 0
fi

adb shell reboot >/dev/null 2>&1
sleep 10
reboot_flag=0
while :
do
ifconfig wan0 up
if [ $? -eq 0 ];then
	adb shell upgrader get
	let reboot_flag=reboot_flag+1
	if [ $reboot_flag -eq 3 ];then
		break
	else
		sleep 10
		continue
	fi
fi
sleep 10
done

up_flag="`adb shell upgrader get | cut -c 1-8`"
echo $up_flag
if [ $(($up_flag)) -ne 0 ];then
	echo "upgrader error!" >> $LOG
	return 0
else
	echo "upgrader success."
	rm $LOG
	return 1
fi
}

if [ $# -ne 2 ];then
	usage
	exit
fi

tftp -r $1 -l /tmp/firmware.tar.bz2 -g $2 >/dev/null 2>&1 && touch /tmp/tftp_tmp
if [ ! -e /tmp/tftp_tmp ];then
	echo "tftp error" >> $LOG
	exit
fi

stop_monitor
killall wtpd

ath=`ifconfig | grep ath* | awk '{print $1}'`
for interface in $ath
do
	wlanconfig $interface destroy
done

up_grade
case $? in
0) reboot;;
1) wtpd & >/dev/null 2>&1;;
*) echo "something error" >> $LOG
	reboot;;
esac
