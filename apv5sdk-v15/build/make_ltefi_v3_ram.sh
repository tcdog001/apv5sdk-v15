#
#wangyu add for v5 mkfs.jffs2 shared library
if [ `env|grep -o -e 'LD_LIBRARY_PATH'` ];then
#       export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/emul/ia32-linux/usr/lib
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib32
else
#       export LD_LIBRARY_PATH=/emul/ia32-linux/usr/lib
        export LD_LIBRARY_PATH=/usr/lib32
fi
##ltefi CS-VIC-2000-C for Production test


make BOARD_TYPE=db12x BUILD_TYPE=jffs2 ETH_CONFIG=_s27 OS_TYPE=SINGLE_OS ENTR=7 lzma_uimage
