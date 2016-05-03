SRCDIR=../../res
#TOPDIR=~/openwrt/kamikaze_7.09/package/base-files/files
TOPDIR1=~/svn/openwrt/package/base-files/files
TOPDIR=~/svn/openwrt/build_mips/root
if [ -d $SRCDIR ]
then
if [ -f $SRCDIR/config.wtp ]
then
	cp -f $SRCDIR/config.wtp $TOPDIR/etc/wlan/
	cp -f $SRCDIR/config.wtp $TOPDIR1/etc/wlan/
fi
if [ -f $SRCDIR/config.board ]
then
    cp -f $SRCDIR/config.board $TOPDIR/etc/wlan
	cp -f $SRCDIR/config.board $TOPDIR1/etc/wlan
fi
if [ -f $SRCDIR/settings.wtp.txt ]
then 
	cp -f $SRCDIR/settings.wtp.txt $TOPDIR/etc/wlan
	cp -f $SRCDIR/settings.wtp.txt $TOPDIR1/etc/wlan
fi
if [ -f $SRCDIR/wtpd ]
then
	cp -f $SRCDIR/wtpd $TOPDIR/sbin
	cp -f $SRCDIR/wtpd $TOPDIR1/sbin
fi

fi
