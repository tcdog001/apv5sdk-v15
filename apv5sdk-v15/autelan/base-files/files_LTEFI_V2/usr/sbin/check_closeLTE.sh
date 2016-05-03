#! /bin/sh

killall check_lte
killall netconn
killall qmuxd

`ifconfig | grep -o -q usb0` >/dev/null 2>&1
   if [ $? = 0 ];then
#        echo "=====Please Watch the LTE !====="
#        echo "=====   LTE is Lighted !   ====="
#       sleep 10 
	pro_ctl_util -d 16 >/dev/null 2>&1
	sleep 5
	echo "=====Please Watch the LTE !====="
	echo "=====     LTE is OFF !     ====="
	sleep 45

        `ifconfig | grep -o -q usb0` >/dev/null 2>&1
        if [ $? = 1 ]; then
                echo "    "
                echo "=====the usb0 is down !====="
                echo "====== CLOSE LTE PASS !====="
                exit 0
        else 
                echo "=====LTE Power Failure !====="
        fi
        exit 0
   else
        echo "=====LTE mount is FAIL !====="
   fi
