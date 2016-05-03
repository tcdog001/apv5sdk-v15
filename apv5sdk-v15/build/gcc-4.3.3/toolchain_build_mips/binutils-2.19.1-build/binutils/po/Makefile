# Makefile for program source directory in GNU NLS utilities package.
# Copyright (C) 1995, 1996, 1997 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
# Copyright 2003, 2006 Free Software Foundation, Inc.
#
# This file may be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.

PACKAGE = binutils
VERSION = 2.19.1

SHELL = /bin/sh


srcdir = /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/po
top_srcdir = /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils
VPATH = /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/po
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
MKINSTALLDIRS = /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/../mkinstalldirs

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
POFILES =  $(srcdir)/fr.po $(srcdir)/tr.po $(srcdir)/ja.po $(srcdir)/es.po $(srcdir)/sv.po $(srcdir)/da.po $(srcdir)/zh_CN.po $(srcdir)/ru.po $(srcdir)/ro.po $(srcdir)/rw.po $(srcdir)/zh_TW.po $(srcdir)/fi.po $(srcdir)/vi.po $(srcdir)/uk.po $(srcdir)/sk.po
GMOFILES =  $(srcdir)/fr.gmo $(srcdir)/tr.gmo $(srcdir)/ja.gmo $(srcdir)/es.gmo $(srcdir)/sv.gmo $(srcdir)/da.gmo $(srcdir)/zh_CN.gmo $(srcdir)/ru.gmo $(srcdir)/ro.gmo $(srcdir)/rw.gmo $(srcdir)/zh_TW.gmo $(srcdir)/fi.gmo $(srcdir)/vi.gmo $(srcdir)/uk.gmo $(srcdir)/sk.gmo
DISTFILES = ChangeLog Makefile.in.in POTFILES.in $(PACKAGE).pot \
stamp-cat-id $(POFILES) $(GMOFILES) $(SOURCES)

POTFILES = \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/addr2line.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/ar.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/arsup.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/arsup.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/bin2c.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/binemul.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/binemul.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/bucomm.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/bucomm.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/budbg.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/coffdump.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/coffgrok.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/coffgrok.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/cxxfilt.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/debug.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/debug.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/debug.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/dlltool.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/dlltool.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/dllwrap.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/dwarf.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/dwarf.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/emul_aix.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/emul_vanilla.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/filemode.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/ieee.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/ieee.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/is-ranlib.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/is-strip.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/maybe-ranlib.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/maybe-strip.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/mclex.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/nlmconv.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/nlmconv.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/nm.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/not-ranlib.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/not-strip.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/objcopy.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/objdump.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/prdbg.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/rclex.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/rdcoff.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/rdcoff.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/rddbg.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/rddbg.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/readelf.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/rename.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/resbin.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/rescoff.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/resrc.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/resres.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/size.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/srconv.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/stabs.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/stabs.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/strings.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/sysdep.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/sysdump.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/unwind-ia64.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/unwind-ia64.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/version.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/windint.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/windmc.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/windmc.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/windres.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/windres.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/winduni.c \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/winduni.h \
     /home/zhangsiyu/pc018work-v15-at/apv5sdk-v15/build/gcc-4.3.3/toolchain_build_mips/binutils-2.19.1/binutils/wrstabs.c

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
