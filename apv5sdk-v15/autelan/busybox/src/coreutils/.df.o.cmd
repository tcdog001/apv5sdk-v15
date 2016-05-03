cmd_coreutils/df.o := mips-linux-uclibc-gcc -Wp,-MD,coreutils/.df.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/autelan/busybox/src/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.4.2)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wstrict-prototypes -Wshadow -Wundef -funsigned-char -fno-builtin-strlen -finline-limit=0 -static-libgcc -Os -falign-functions=1 -falign-jumps=1 -falign-loops=1 -fomit-frame-pointer -ffunction-sections -fdata-sections -Wdeclaration-after-statement -Wno-pointer-sign -DAPV5   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(df)"  -D"KBUILD_MODNAME=KBUILD_STR(df)" -c -o coreutils/df.o coreutils/df.c

deps_coreutils/df.o := \
  coreutils/df.c \
    $(wildcard include/config/feature/human/readable.h) \
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
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/stdlib.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/waitflags.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/waitstatus.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/endian.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/endian.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/types.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/select.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/select.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/sysmacros.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/alloca.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/string.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/unistd.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/posix_opt.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/environments.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/confname.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/getopt.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/mntent.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/paths.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/vfs.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/statfs.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/statfs.h \
  include/busybox.h \
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
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include-fixed/limits.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/bin-ccache/../lib/gcc/mips-linux-uclibc/4.3.3/include-fixed/syslimits.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/limits.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/posix1_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/local_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/linux/limits.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/uClibc_local_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/posix2_lim.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/xopen_lim.h \
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
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/strings.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/mman.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/bits/mman.h \
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
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/sys/param.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/linux/param.h \
  /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/staging_dir/usr/include/asm/param.h \
  include/xatonum.h \
  include/applets.h \
    $(wildcard include/config/feature/verbose/usage.h) \
    $(wildcard include/config/install/no/usr.h) \
    $(wildcard include/config/t.h) \
    $(wildcard include/config/group.h) \
    $(wildcard include/config/user.h) \
    $(wildcard include/config/timex.h) \
    $(wildcard include/config/applet.h) \
    $(wildcard include/config/ing.h) \
    $(wildcard include/config/ename.h) \
    $(wildcard include/config/onfig.h) \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/zip2.h) \
    $(wildcard include/config/v.h) \
    $(wildcard include/config/ttr.h) \
    $(wildcard include/config/rp.h) \
    $(wildcard include/config/od.h) \
    $(wildcard include/config/wn.h) \
    $(wildcard include/config/st.h) \
    $(wildcard include/config/oot.h) \
    $(wildcard include/config/um.h) \
    $(wildcard include/config/ar.h) \
    $(wildcard include/config/m.h) \
    $(wildcard include/config/o.h) \
    $(wildcard include/config/nd.h) \
    $(wildcard include/config/ntab.h) \
    $(wildcard include/config/e.h) \
    $(wildcard include/config/llocvt.h) \
    $(wildcard include/config/fsd.h) \
    $(wildcard include/config//dhcprelay.h) \
    $(wildcard include/config/f.h) \
    $(wildcard include/config/name.h) \
    $(wildcard include/config/sg.h) \
    $(wildcard include/config/2unix.h) \
    $(wildcard include/config/g.h) \
    $(wildcard include/config/g/deb.h) \
    $(wildcard include/config/pkmap.h) \
    $(wildcard include/config//dumpleases.h) \
    $(wildcard include/config/sck.h) \
    $(wildcard include/config/abel.h) \
    $(wildcard include/config/ture/grep/egrep/alias.h) \
    $(wildcard include/config/ct.h) \
    $(wildcard include/config/dir.h) \
    $(wildcard include/config/uidgid.h) \
    $(wildcard include/config/er/wake.h) \
    $(wildcard include/config/r.h) \
    $(wildcard include/config/eidentd.h) \
    $(wildcard include/config/se.h) \
    $(wildcard include/config/et.h) \
    $(wildcard include/config/lush.h) \
    $(wildcard include/config/ormat.h) \
    $(wildcard include/config/sk.h) \
    $(wildcard include/config/ture/grep/fgrep/alias.h) \
    $(wildcard include/config/dfs.h) \
    $(wildcard include/config/eramdisk.h) \
    $(wildcard include/config/k.h) \
    $(wildcard include/config/k/minix.h) \
    $(wildcard include/config/get.h) \
    $(wildcard include/config/put.h) \
    $(wildcard include/config/er.h) \
    $(wildcard include/config/opt.h) \
    $(wildcard include/config/ty.h) \
    $(wildcard include/config/p.h) \
    $(wildcard include/config/zip.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/dump.h) \
    $(wildcard include/config/tid.h) \
    $(wildcard include/config/tname.h) \
    $(wildcard include/config/pd.h) \
    $(wildcard include/config/lock.h) \
    $(wildcard include/config/pdown.h) \
    $(wildcard include/config/td.h) \
    $(wildcard include/config/mod.h) \
    $(wildcard include/config/tall.h) \
    $(wildcard include/config/ddr.h) \
    $(wildcard include/config/alc.h) \
    $(wildcard include/config/rm.h) \
    $(wildcard include/config/s.h) \
    $(wildcard include/config/ink.h) \
    $(wildcard include/config/oute.h) \
    $(wildcard include/config/ule.h) \
    $(wildcard include/config/unnel.h) \
    $(wildcard include/config/l.h) \
    $(wildcard include/config/lall.h) \
    $(wildcard include/config/lall5.h) \
    $(wildcard include/config/gd.h) \
    $(wildcard include/config/gth.h) \
    $(wildcard include/config/arch.h) \
    $(wildcard include/config/ture/initrd.h) \
    $(wildcard include/config/dfont.h) \
    $(wildcard include/config/dkmap.h) \
    $(wildcard include/config/ger.h) \
    $(wildcard include/config/in.h) \
    $(wildcard include/config/read.h) \
    $(wildcard include/config/etup.h) \
    $(wildcard include/config/zma.h) \
    $(wildcard include/config/edevs.h) \
    $(wildcard include/config/sum.h) \
    $(wildcard include/config/ir.h) \
    $(wildcard include/config/2fs.h) \
    $(wildcard include/config/ifo.h) \
    $(wildcard include/config/s/minix.h) \
    $(wildcard include/config/wap.h) \
    $(wildcard include/config/emp.h) \
    $(wildcard include/config/probe.h) \
    $(wildcard include/config/nt.h) \
    $(wildcard include/config/sktop.h) \
    $(wildcard include/config/ntpoint.h) \
    $(wildcard include/config/eif.h) \
    $(wildcard include/config/msg.h) \
    $(wildcard include/config/stat.h) \
    $(wildcard include/config/ter.h) \
    $(wildcard include/config/up.h) \
    $(wildcard include/config/ookup.h) \
    $(wildcard include/config/nvt.h) \
    $(wildcard include/config/swd.h) \
    $(wildcard include/config/ch.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/g6.h) \
    $(wildcard include/config/e/progress.h) \
    $(wildcard include/config/ot/root.h) \
    $(wildcard include/config/ntenv.h) \
    $(wildcard include/config/ntf.h) \
    $(wildcard include/config/dautorun.h) \
    $(wildcard include/config/te.h) \
    $(wildcard include/config/dahead.h) \
    $(wildcard include/config/dlink.h) \
    $(wildcard include/config/dprofile.h) \
    $(wildcard include/config/lpath.h) \
    $(wildcard include/config/ice.h) \
    $(wildcard include/config/ize.h) \
    $(wildcard include/config/2cpio.h) \
    $(wildcard include/config//parts.h) \
    $(wildcard include/config/level.h) \
    $(wildcard include/config/sv.h) \
    $(wildcard include/config/svdir.h) \
    $(wildcard include/config/console.h) \
    $(wildcard include/config/keycodes.h) \
    $(wildcard include/config/logcons.h) \
    $(wildcard include/config/sid.h) \
    $(wildcard include/config/ture/sh/is/ash.h) \
    $(wildcard include/config/ture/sh/is/hush.h) \
    $(wildcard include/config/ture/sh/is/lash.h) \
    $(wildcard include/config/ture/sh/is/msh.h) \
    $(wildcard include/config/1sum.h) \
    $(wildcard include/config/ep.h) \
    $(wildcard include/config/tlimit.h) \
    $(wildcard include/config/rt/stop/daemon.h) \
    $(wildcard include/config/ings.h) \
    $(wildcard include/config/y.h) \
    $(wildcard include/config/ogin.h) \
    $(wildcard include/config/ogd.h) \
    $(wildcard include/config/ponoff.h) \
    $(wildcard include/config/tch/root.h) \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/logd.h) \
    $(wildcard include/config/kset.h) \
    $(wildcard include/config/net.h) \
    $(wildcard include/config/netd.h) \
    $(wildcard include/config/feature/tftp/get.h) \
    $(wildcard include/config/feature/tftp/put.h) \
    $(wildcard include/config/ceroute.h) \
    $(wildcard include/config/e2fs.h) \
    $(wildcard include/config//udhcpc.h) \
    $(wildcard include/config//udhcpd.h) \
    $(wildcard include/config/unt.h) \
    $(wildcard include/config/me.h) \
    $(wildcard include/config/ompress.h) \
    $(wildcard include/config/q.h) \
    $(wildcard include/config/x2dos.h) \
    $(wildcard include/config/ip.h) \
    $(wildcard include/config/ime.h) \
    $(wildcard include/config/eep.h) \
    $(wildcard include/config/ecode.h) \
    $(wildcard include/config/ncode.h) \
    $(wildcard include/config/nfig.h) \
    $(wildcard include/config/ck.h) \
    $(wildcard include/config/chdog.h) \
    $(wildcard include/config/ami.h) \
    $(wildcard include/config/gs.h) \

coreutils/df.o: $(deps_coreutils/df.o)

$(deps_coreutils/df.o):
