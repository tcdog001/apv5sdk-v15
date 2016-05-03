#
# Minimal XML library
#
# /src (main) subdir Makefile.in (this is for configure)
# $Id: Makefile.am,v 1.1 2003/07/08 22:52:51 jonnymind Exp $

SUBDIRS=
EXTRADIST=Doxyfile 

all: doctag

doctag: $(wildcard ../src/*.c)
	if test x@enable_doc@ = xyes; then\
	   doxygen && touch doctag;\
	fi

clean:
	rm -rf html
	rm -rf *~
	rm -rf doctag

distclean: clean
	rm -f Makefile

install:
	echo "Install of documentation not supported"

