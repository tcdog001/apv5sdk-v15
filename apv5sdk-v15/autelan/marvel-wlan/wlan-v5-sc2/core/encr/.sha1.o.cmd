cmd_/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/sha1.o := /home/shark/apv7_marvell/apv7sdk/build/../build/armv7-marvell-linux-gnueabi-softfp/bin/arm-marvell-linux-gnueabi-gcc -Wp,-MD,/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/.sha1.o.d  -nostdinc -isystem /home/shark/apv7_marvell/apv7sdk/build/armv7-marvell-linux-gnueabi-softfp/bin/../lib/gcc/arm-marvell-linux-gnueabi/4.4.5/include -I/home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-armada370/include -Iarch/arm/plat-armada/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -ffunction-sections -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -I/home/shark/apv7_marvell/apv7sdk/build/../linux/kernels/marvell-linux-2.6.35.9 -DWL_KERNEL_26 -DUSE_TASKLET -O2 -funroll-loops -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2 -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2 -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/driver/linux -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/api -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/incl -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/mgt/STA/incl -IW8764 -IW8764/Common -IW8764/Soc/idma -IW8764/Soc/ctrlEnv -IW8764/mv88f5181 -IW8764/osServices -IW8764/osServices/linux/ArchARM -IW8764/Board/boardEnv -IW8764/Board/boardEnv/DB_88FXX81 -IW8764/Soc/ctrlEnv/MV_88FXX81 -IW8764/osServices/linux -DMV_CPU_LE -DMV_LINUX -DMV_ARM -DUDP_API -DAP_MAC_LINUX -DIEEE80211H -DWPA -DAP_WPA2 -DQOS_FEATURE -DSTA_INFO_DB -DBT_COEXISTENCE -DAVOID_MIS_ACC -DQOS_WSM_FEATURE -DEXPORT_SYMTAB -DRELEASE_11N -DAGG_QUE -DAP8X_STATISTICS -DENABLE_WLSNDEVT -DAP8X_DUMP -DAUTOCHANNEL -DENABLE_WLSYSLOG -DUAPSD_SUPPORT -DAMPDU_SUPPORT -DMRV_8021X -DINTEROP -DAMSDU_BYTE_REORDER -DIEEE_HTIE -DWDS_FEATURE -DBRS_SUPPORT -DIEEE80211_DH -DMFG_SUPPORT -DPWRFRAC -DMRVL_WSC -DMRVL_DFS -DIEEE80211N_MIMOPSD110 -DCOEXIST_20_40_SUPPORT -DGENERIC_GETIE -DMRVL_WPS2 -DEXPLICIT_BF -DEWB -DWLAN_INCLUDE_TSO -DMBSS -DCOMMON_PHYDSSS -DMIMO_PS_HT -DNPROTECTION -DCLIENT_SUPPORT -DWPA_STA -DWPA2 -DRSN_RESOLVE -DAMPDU_SUPPORT_TX_CLIENT -DSOC_W8764 -DWLMEM_DISABLED -DMPRXY -DMPRXY_SNOOP -DMPRXY_IGMP_QUERY -DNUMOFAPS=8 -DMRVL_WPS_CLIENT -DDFS_PASSIVE_SCAN -DDFS_CHNL_SUPPORT -DFCC_15E_INTERIM_PLAN -DBA_WATCHDOG -DV6FW -DATTN_RSSI -DAMPDU_SUPPORT_SBA -DDYNAMIC_BA_SUPPORT -DAMSDUOVERAMPDU -DWFA_TKIP_NEGATIVE  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sha1)"  -D"KBUILD_MODNAME=KBUILD_STR(ap8x)"  -c -o /home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/sha1.o /home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/sha1.c

deps_/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/sha1.o := \
  /home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/sha1.c \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/posix_types.h \
  /home/shark/apv7_marvell/apv7sdk/build/armv7-marvell-linux-gnueabi-softfp/bin/../lib/gcc/arm-marvell-linux-gnueabi/4.4.5/include/stdarg.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/string.h \
  /home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/incl/sha1.h \

/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/sha1.o: $(deps_/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/sha1.o)

$(deps_/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/encr/sha1.o):
