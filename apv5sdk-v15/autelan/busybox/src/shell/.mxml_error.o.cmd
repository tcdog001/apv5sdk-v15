cmd_shell/mxml_error.o := mips-linux-uclibc-gcc -Wp,-MD,shell/.mxml_error.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/autelan/busybox/src/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.4.2)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wstrict-prototypes -Wshadow -Wundef -funsigned-char -fno-builtin-strlen -finline-limit=0 -static-libgcc -Os -falign-functions=1 -falign-jumps=1 -falign-loops=1 -fomit-frame-pointer -ffunction-sections -fdata-sections -Wdeclaration-after-statement -Wno-pointer-sign -DAPV5   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mxml_error)"  -D"KBUILD_MODNAME=KBUILD_STR(mxml_error)" -c -o shell/mxml_error.o shell/mxml_error.c

deps_shell/mxml_error.o := \
  shell/mxml_error.c \
  shell/mxml.h \
  shell/mxml_defs.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/stdio.h \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/c.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/gefile.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/features.h \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/ile.h) \
    $(wildcard include/config/ntrant.h) \
    $(wildcard include/config/tify/level.h) \
    $(wildcard include/config/i.h) \
    $(wildcard include/config/ern/inlines.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_config.h \
    $(wildcard include/config/mips/o32/abi//.h) \
    $(wildcard include/config/mips/n32/abi//.h) \
    $(wildcard include/config/mips/n64/abi//.h) \
    $(wildcard include/config/mips/isa/1//.h) \
    $(wildcard include/config/mips/isa/2//.h) \
    $(wildcard include/config/mips/isa/3//.h) \
    $(wildcard include/config/mips/isa/4//.h) \
    $(wildcard include/config/mips/isa/mips32//.h) \
    $(wildcard include/config/mips/isa/mips32r2//.h) \
    $(wildcard include/config/mips/isa/mips64//.h) \
    $(wildcard include/config///.h) \
    $(wildcard include/config//.h) \
    $(wildcard include/config/link//.h) \
    $(wildcard include/config//vfprintf//.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_arch_features.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/cdefs.h \
    $(wildcard include/config/espaces.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include/stddef.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/wordsize.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/kernel_types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/typesizes.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/pthreadtypes.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sched.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_stdio.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_mutex.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/pthread.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sched.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/time.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/time.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_clk_tck.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/signal.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sigset.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/initspin.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sigthread.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_pthread.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include/stdarg.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/stdio_lim.h \

shell/mxml_error.o: $(deps_shell/mxml_error.o)

$(deps_shell/mxml_error.o):
