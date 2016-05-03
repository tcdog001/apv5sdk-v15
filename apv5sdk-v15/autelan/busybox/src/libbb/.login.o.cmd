cmd_libbb/login.o := mips-linux-uclibc-gcc -Wp,-MD,libbb/.login.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/autelan/busybox/src/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.4.2)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wstrict-prototypes -Wshadow -Wundef -funsigned-char -fno-builtin-strlen -finline-limit=0 -static-libgcc -Os -falign-functions=1 -falign-jumps=1 -falign-loops=1 -fomit-frame-pointer -ffunction-sections -fdata-sections -Wdeclaration-after-statement -Wno-pointer-sign -DAPV5   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(login)"  -D"KBUILD_MODNAME=KBUILD_STR(login)" -c -o libbb/login.o libbb/login.c

deps_libbb/login.o := \
  libbb/login.c \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/param.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include-fixed/limits.h \
    $(wildcard include/config/.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include-fixed/syslimits.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/limits.h \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/en.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/features.h \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/gefile.h) \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/c.h) \
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
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/posix1_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/local_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/linux/limits.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_local_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/posix2_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/xopen_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/stdio_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/linux/param.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm/param.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/wordsize.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include/stddef.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/kernel_types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/typesizes.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/pthreadtypes.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sched.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/time.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/endian.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/endian.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/select.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/select.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sigset.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/time.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/sysmacros.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/stdio.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_stdio.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_mutex.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/pthread.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sched.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_clk_tck.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/signal.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/initspin.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sigthread.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_pthread.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include/stdarg.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/unistd.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/posix_opt.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/environments.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/confname.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/getopt.h \
  include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/getopt/long.h) \
    $(wildcard include/config/ktop.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/byteswap.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/byteswap.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/arpa/inet.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/netinet/in.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/stdint.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/socket.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/uio.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uio.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/socket.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sockaddr.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm/socket.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm/sockios.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm/ioctl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm-generic/ioctl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/in.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/mount.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/ioctl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/ioctls.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm/ioctls.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/ioctl-types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/ttydefaults.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/ctype.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_touplow.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/dirent.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/dirent.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/errno.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/errno.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/linux/errno.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm/errno.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm-generic/errno-base.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/syscall.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sysnum.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/fcntl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/fcntl.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sgidefs.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/stat.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/stat.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/inttypes.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/malloc.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/netdb.h \
    $(wildcard include/config/3/ascii/rules.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/rpc/netdb.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/siginfo.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/netdb.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/setjmp.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/setjmp.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/signum.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sigaction.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sigcontext.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/sigstack.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/ucontext.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/ucontext.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/stdlib.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/waitflags.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/waitstatus.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/alloca.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/string.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/strings.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/mman.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/mman.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/statfs.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/statfs.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/time.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/wait.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/resource.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/resource.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/termios.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/termios.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/utime.h \
  include/pwd_.h \
    $(wildcard include/config/use/bb/pwd/grp.h) \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/pwd.h \
  include/grp_.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/grp.h \
  include/xatonum.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/utsname.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/utsname.h \

libbb/login.o: $(deps_libbb/login.o)

$(deps_libbb/login.o):
