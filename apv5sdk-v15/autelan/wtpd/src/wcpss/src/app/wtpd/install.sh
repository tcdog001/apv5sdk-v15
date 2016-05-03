SRCDIR=../../res
if [ -d $SRCDIR ]
then
if [ -f $SRCDIR/config.wtp ]
then
	cp -f $SRCDIR/config.wtp $TOPDIR/rootdir/etc/wlan
	cp -f $SRCDIR/config.wtp $TOPDIR/tftpboot
fi
if [ -f $SRCDIR/config.board ]
then
    cp -f $SRCDIR/config.board $TOPDIR/rootdir/etc/wlan
	cp -f $SRCDIR/config.board $TOPDIR/tftpboot
fi
if [ -f $SRCDIR/settings.wtp.txt ]
then 
	cp -f $SRCDIR/settings.wtp.txt $TOPDIR/rootdir/etc/wlan
	cp -f $SRCDIR/settings.wtp.txt $TOPDIR/tftpboot
fi
if [ -f $SRCDIR/wtpd ]
then
	cp -f $SRCDIR/wtpd $TOPDIR/rootdir/sbin
	cp -f $SRCDIR/wtpd $TOPDIR/tftpboot
fi

fi
