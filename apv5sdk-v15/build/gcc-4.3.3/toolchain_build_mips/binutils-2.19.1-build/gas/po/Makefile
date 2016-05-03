# Makefile for program source directory in GNU NLS utilities package.
# Copyright (C) 1995, 1996, 1997 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
# Copyright 2003, 2006 Free Software Foundation, Inc.
#
# This file may be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.

PACKAGE = gas
VERSION = 2.19.1

SHELL = /bin/sh


srcdir = /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/po
top_srcdir = /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas
VPATH = /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/po
top_builddir = ../

prefix = /usr
exec_prefix = ${prefix}
datadir = $(prefix)/
localedir = $(datadir)/locale
gnulocaledir = $(prefix)/share/locale
gettextsrcdir = $(prefix)/share/gettext/po
subdir = po

DESTDIR =

INSTALL = /usr/bin/install -c
INSTALL_DATA = /usr/bin/install -c -m 644
MKINSTALLDIRS = /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/../mkinstalldirs

CC = /usr/bin/gcc -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/host_dir/include -I/home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/build_mips/host_dir/usr/include
GENCAT = 
GMSGFMT = PATH=../src:$$PATH /usr/bin/msgfmt
MSGFMT = /usr/bin/msgfmt
XGETTEXT = PATH=../src:$$PATH /usr/bin/xgettext
MSGMERGE = PATH=../src:$$PATH msgmerge

DEFS = -DHAVE_CONFIG_H
CFLAGS = -g -O2
CPPFLAGS = 

INCLUDES = -I.. -I$(top_srcdir)/intl

COMPILE = $(CC) -c $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(XCFLAGS)

SOURCES = cat-id-tbl.c
POFILES =  $(srcdir)/fr.po $(srcdir)/tr.po $(srcdir)/es.po $(srcdir)/rw.po
GMOFILES =  $(srcdir)/fr.gmo $(srcdir)/tr.gmo $(srcdir)/es.gmo $(srcdir)/rw.gmo
DISTFILES = ChangeLog Makefile.in.in POTFILES.in $(PACKAGE).pot \
stamp-cat-id $(POFILES) $(GMOFILES) $(SOURCES)

POTFILES = \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/app.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/as.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/as.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/asintl.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/atof-generic.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/bignum.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/bit_fix.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/cgen.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/cgen.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/cond.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/atof-ieee.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/atof-vax.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/e-crisaout.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/e-criself.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/e-i386aout.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/e-i386coff.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/e-i386elf.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/e-mipsecoff.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/e-mipself.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-aout.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-aout.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-coff.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-coff.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-ecoff.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-ecoff.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-elf.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-elf.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-evax.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-evax.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-fdpicelf.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-fdpicelf.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-som.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/obj-som.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-alpha.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-alpha.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-arc.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-arc.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-arm.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-arm.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-avr.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-avr.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-bfin.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-bfin.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-cr16.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-cr16.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-cris.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-cris.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-crx.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-crx.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-d10v.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-d10v.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-d30v.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-d30v.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-dlx.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-dlx.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-fr30.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-fr30.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-frv.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-frv.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-h8300.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-h8300.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-hppa.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-hppa.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-i370.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-i370.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-i386.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-i386.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-i860.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-i860.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-i960.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-i960.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-ia64.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-ia64.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-ip2k.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-ip2k.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-iq2000.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-iq2000.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-m32c.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-m32c.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-m32r.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-m32r.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-m68hc11.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-m68hc11.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-m68k.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-m68k.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-maxq.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-maxq.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mcore.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mcore.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mep.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mep.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mips.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mips.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mmix.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mmix.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mn10200.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mn10200.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mn10300.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mn10300.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-msp430.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-msp430.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mt.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-mt.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-ns32k.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-ns32k.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-openrisc.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-openrisc.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-or32.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-or32.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-pdp11.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-pdp11.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-pj.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-pj.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-ppc.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-ppc.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-s390.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-s390.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-score.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-score.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-sh.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-sh.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-sh64.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-sh64.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-sparc.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-sparc.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-spu.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-spu.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-tic30.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-tic30.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-tic4x.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-tic4x.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-tic54x.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-tic54x.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-v850.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-v850.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-vax.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-vax.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-xc16x.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-xc16x.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-xstormy16.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-xstormy16.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-xtensa.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-xtensa.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-z80.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-z80.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-z8k.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/tc-z8k.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-386bsd.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-aix5.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-armeabi.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-armlinuxeabi.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-dynix.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-epoc-pe.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-freebsd.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-generic.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-gnu.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-go32.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-hppa.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-hppa64.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-hppalinux64.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-i386aix.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-ia64aix.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-interix.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-lnews.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-lynx.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-mach.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-macos.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-nbsd.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-nbsd532.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-netware.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-pc532mach.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-pe.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-psos.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-riscix.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-sparcaout.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-sun3.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-svr4.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-symbian.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-tmips.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-vxworks.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/te-wince-pe.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/xtensa-relax.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/config/xtensa-relax.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/depend.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/dw2gencfi.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/dw2gencfi.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/dwarf2dbg.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/dwarf2dbg.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/ecoff.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/ecoff.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/ehopt.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/emul-target.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/emul.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/expr.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/expr.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/flonum-copy.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/flonum-konst.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/flonum-mult.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/flonum.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/frags.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/frags.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/hash.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/hash.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/input-file.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/input-file.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/input-scrub.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/itbl-lex.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/itbl-ops.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/itbl-ops.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/listing.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/listing.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/literal.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/macro.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/macro.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/messages.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/obj.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/output-file.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/output-file.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/read.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/read.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/remap.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/sb.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/sb.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/stabs.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/struc-symbol.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/subsegs.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/subsegs.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/symbols.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/symbols.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/tc.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/write.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/gas/write.h

CATALOGS = 
CATOBJEXT = 
INSTOBJEXT = 

.SUFFIXES:
.SUFFIXES: .c .o .po .pox .gmo .mo .msg .cat

.c.o:
	$(COMPILE) $<

.po.pox:
	$(MAKE) $(PACKAGE).pot
	$(MSGMERGE) $< $(srcdir)/$(PACKAGE).pot -o $*.pox

.po.mo:
	$(MSGFMT) -o $@ $<

.po.gmo:
	file=`echo $* | sed 's,.*/,,'`.gmo \
	  && rm -f $$file && $(GMSGFMT) -o $$file $<

.po.cat:
	sed -f ../intl/po2msg.sed < $< > $*.msg \
	  && rm -f $@ && $(GENCAT) $@ $*.msg


all: all-no

all-yes: $(CATALOGS) # $(PACKAGE).pot
all-no:

$(srcdir)/$(PACKAGE).pot: $(POTFILES)
	$(XGETTEXT) --default-domain=$(PACKAGE) --directory=$(top_srcdir) \
	  --add-comments --keyword=_ --keyword=N_ \
	  --msgid-bugs-address=bug-binutils@gnu.org \
	  --files-from=$(srcdir)/POTFILES.in
	rm -f $(srcdir)/$(PACKAGE).pot
	mv $(PACKAGE).po $(srcdir)/$(PACKAGE).pot

$(srcdir)/cat-id-tbl.c: stamp-cat-id; @:
$(srcdir)/stamp-cat-id: $(PACKAGE).pot
	rm -f cat-id-tbl.tmp
	sed -f ../intl/po2tbl.sed $(srcdir)/$(PACKAGE).pot \
		| sed -e "s/@PACKAGE NAME@/$(PACKAGE)/" > cat-id-tbl.tmp
	if cmp -s cat-id-tbl.tmp $(srcdir)/cat-id-tbl.c; then \
	  rm cat-id-tbl.tmp; \
	else \
	  echo cat-id-tbl.c changed; \
	  rm -f $(srcdir)/cat-id-tbl.c; \
	  mv cat-id-tbl.tmp $(srcdir)/cat-id-tbl.c; \
	fi
	cd $(srcdir) && rm -f stamp-cat-id && echo timestamp > stamp-cat-id


install: install-exec install-data
install-exec:
install-info:
install-html:
install-data: install-data-no
install-data-no: all
install-data-yes: all
	if test -r $(MKINSTALLDIRS); then \
	  $(MKINSTALLDIRS) $(DESTDIR)$(datadir); \
	else \
	  $(top_srcdir)/mkinstalldirs $(DESTDIR)$(datadir); \
	fi
	@catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  case "$$cat" in \
	    *.gmo) destdir=$(gnulocaledir);; \
	    *)     destdir=$(localedir);; \
	  esac; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  dir=$(DESTDIR)$$destdir/$$lang/LC_MESSAGES; \
	  if test -r $(MKINSTALLDIRS); then \
	    $(MKINSTALLDIRS) $$dir; \
	  else \
	    $(top_srcdir)/mkinstalldirs $$dir; \
	  fi; \
	  if test -r $$cat; then \
	    $(INSTALL_DATA) $$cat $$dir/$(PACKAGE)$(INSTOBJEXT); \
	    echo "installing $$cat as $$dir/$(PACKAGE)$(INSTOBJEXT)"; \
	  else \
	    $(INSTALL_DATA) $(srcdir)/$$cat $$dir/$(PACKAGE)$(INSTOBJEXT); \
	    echo "installing $(srcdir)/$$cat as" \
		 "$$dir/$(PACKAGE)$(INSTOBJEXT)"; \
	  fi; \
	  if test -r $$cat.m; then \
	    $(INSTALL_DATA) $$cat.m $$dir/$(PACKAGE)$(INSTOBJEXT).m; \
	    echo "installing $$cat.m as $$dir/$(PACKAGE)$(INSTOBJEXT).m"; \
	  else \
	    if test -r $(srcdir)/$$cat.m ; then \
	      $(INSTALL_DATA) $(srcdir)/$$cat.m \
		$$dir/$(PACKAGE)$(INSTOBJEXT).m; \
	      echo "installing $(srcdir)/$$cat as" \
		   "$$dir/$(PACKAGE)$(INSTOBJEXT).m"; \
	    else \
	      true; \
	    fi; \
	  fi; \
	done
	if test "$(PACKAGE)" = "gettext"; then \
	  if test -r $(MKINSTALLDIRS); then \
	    $(MKINSTALLDIRS) $(DESTDIR)$(gettextsrcdir); \
	  else \
	    $(top_srcdir)/mkinstalldirs $(DESTDIR)$(gettextsrcdir); \
	  fi; \
	  $(INSTALL_DATA) $(srcdir)/Makefile.in.in \
			  $(DESTDIR)$(gettextsrcdir)/Makefile.in.in; \
	else \
	  : ; \
	fi

# Define this as empty until I found a useful application.
installcheck:

uninstall:
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  rm -f $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(PACKAGE)$(INSTOBJEXT); \
	  rm -f $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(PACKAGE)$(INSTOBJEXT).m; \
	  rm -f $(DESTDIR)$(gnulocaledir)/$$lang/LC_MESSAGES/$(PACKAGE)$(INSTOBJEXT); \
	  rm -f $(DESTDIR)$(gnulocaledir)/$$lang/LC_MESSAGES/$(PACKAGE)$(INSTOBJEXT).m; \
	done
	rm -f $(DESTDIR)$(gettextsrcdir)/po-Makefile.in.in

check: all

cat-id-tbl.o: ../intl/libgettext.h

html dvi pdf ps info tags TAGS ID:

mostlyclean:
	rm -f core core.* *.pox $(PACKAGE).po *.old.po cat-id-tbl.tmp
	rm -fr *.o

clean: mostlyclean

distclean: clean
	rm -f Makefile Makefile.in POTFILES *.mo *.msg *.cat *.cat.m

maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f $(GMOFILES)

distdir = ../$(PACKAGE)-$(VERSION)/$(subdir)
dist distdir: update-po $(DISTFILES)
	dists="$(DISTFILES)"; \
	for file in $$dists; do \
	  ln $(srcdir)/$$file $(distdir) 2> /dev/null \
	    || cp -p $(srcdir)/$$file $(distdir); \
	done

update-po: Makefile
	$(MAKE) $(PACKAGE).pot
	PATH=`pwd`/../src:$$PATH; \
	cd $(srcdir); \
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
	  cat=`basename $$cat`; \
	  lang=`echo $$cat | sed 's/\$(CATOBJEXT)$$//'`; \
	  mv $$lang.po $$lang.old.po; \
	  echo "$$lang:"; \
	  if $(MSGMERGE) $$lang.old.po $(PACKAGE).pot -o $$lang.po; then \
	    rm -f $$lang.old.po; \
	  else \
	    echo "msgmerge for $$cat failed!"; \
	    rm -f $$lang.po; \
	    mv $$lang.old.po $$lang.po; \
	  fi; \
	done

POTFILES: POTFILES.in
	( if test 'x$(srcdir)' != 'x.'; then \
	    posrcprefix='$(top_srcdir)/'; \
	  else \
	    posrcprefix="../"; \
	  fi; \
	  rm -f $@-t $@ \
	    && (sed -e '/^#/d' -e '/^[ 	]*$$/d' \
		    -e "s@.*@	$$posrcprefix& \\\\@" < $(srcdir)/$@.in \
		| sed -e '$$s/\\$$//') > $@-t \
	    && chmod a-w $@-t \
	    && mv $@-t $@ )

POTFILES.in: # ../Makefile
	cd .. && $(MAKE) po/POTFILES.in

Makefile: Make-in ../config.status POTFILES
	cd .. \
	  && CONFIG_FILES=$(subdir)/Makefile.in:$(subdir)/Make-in \
	     CONFIG_HEADERS= $(SHELL) ./config.status

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
