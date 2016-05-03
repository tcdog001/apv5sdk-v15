if [ -n "$1" ] ; then
    if [ -b /dev/$1 ]; then
	if [ $1 == "sda1" ]; then
		DEVPATH=/tmp/udisk
	fi
	if [ $1 == "sdb1" ]; then
        	DEVPATH=/tmp/udisk1
	fi
	if [ $1 == "sdc1" ]; then
        	DEVPATH=/tmp/udisk2
	fi
	if [ $1 == "sdd1" ]; then
        	DEVPATH=/tmp/udisk3
	fi
	if [ ! -d $DEVPATH ]; then
		mkdir -p $DEVPATH
	fi

        mount /dev/$1 $DEVPATH
	if [ $? -ne 0 ]; then
		rm -rf $DEVPATH
	fi
    fi
fi
