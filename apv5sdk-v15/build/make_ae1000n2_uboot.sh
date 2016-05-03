if [ `env|grep -o -e 'LD_LIBRARY_PATH'` ];then
	export LD_LIBRARY_PATH=:/usr/lib32
else
        export LD_LIBRARY_PATH=/usr/lib32
fi

make BOARD_TYPE=db12x BUILD_TYPE=jffs2 ETH_CONFIG=_s27 BUILD_CONFIG=ap123 uboot_ae1000n2
