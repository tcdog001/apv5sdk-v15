#
# Makefile for MPI library

# ***** BEGIN LICENSE BLOCK *****
# Version: MPL 1.1/GPL 2.0/LGPL 2.1
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
#
# The Original Code is the MPI Arbitrary Precision Integer Arithmetic library.
#
# The Initial Developer of the Original Code is
# Michael J. Fromberger <sting@linguist.dartmouth.edu>.
# Portions created by the Initial Developer are Copyright (C) 1998
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#   Netscape Communications Corporation
#   Richard C. Swift        (swift@netscape.com)
#   Douglas Stebila <douglas@stebila.ca>, Sun Microsystems Laboratories
#
# Alternatively, the contents of this file may be used under the terms of
# either the GNU General Public License Version 2 or later (the "GPL"), or
# the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
# in which case the provisions of the GPL or the LGPL are applicable instead
# of those above. If you wish to allow use of your version of this file only
# under the terms of either the GPL or the LGPL, and not to allow others to
# use your version of this file under the terms of the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice
# and other provisions required by the GPL or the LGPL. If you do not delete
# the provisions above, a recipient may use your version of this file under
# the terms of any one of the MPL, the GPL or the LGPL.
#
# ***** END LICENSE BLOCK *****

#
# $Id: //depot/sw/releases/Aquila_9.2.0/apps/wapid/lib/ECC/Makefile#1 $
#

## Define CC to be the C compiler you wish to use.  The GNU cc
## compiler (gcc) should work, at the very least
#CC=cc
#include $(prefix)/Rules.elinux
#CC=$(CROSS)gcc 
#AR=$(CROSS)ar
#RANLIB=$(CROSS)ranlib
CC = $(TOOLPREFIX)gcc
LD = $(TOOLPREFIX)ld
AR = $(TOOLPREFIX)ar
STRIP = $(TOOLPREFIX)strip
## 
## Define PERL to point to your local Perl interpreter.  It
## should be Perl 5.x, although it's conceivable that Perl 4
## might work ... I haven't tested it.
##
#PERL=/usr/bin/perl
PERL=perl

#include target.mk

CFLAGS+= -I. -DMP_API_COMPATIBLE -DMP_IOFUNC
#$(XCFLAGS)

##
## Define LIBS to include any libraries you need to link against.
## If NO_TABLE is define, LIBS should include '-lm' or whatever is
## necessary to bring in the math library.  Otherwise, it can be
## left alone, unless your system has other peculiar requirements.
##
LIBS=#-lmalloc#-lefence#-lm

## 
## Define RANLIB to be the library header randomizer; you might not
## need this on some systems (just set it to 'echo' on these systems,
## such as IRIX)
##

##
## This is the version string used for the documentation and 
## building the distribution tarball.  Don't mess with it unless
## you are releasing a new version
VERS=1.7p6

## ----------------------------------------------------------------------
## You probably don't need to change anything below this line...
##

##
## This is the list of source files that need to be packed into
## the distribution file
SRCS=   mpi.c mpprime.c mplogic.c  mpmontg.c \
	ec.c ecl.c ecl-exp.h ecl_mult.c ecp_aff.c ecp_jac.c \
	ecl_curve.c  ecl_gf.c ecp_jm.c  ecp_mont.c ec_naf.c\
	secitem.c random.c \
	mpi-config.h mpprime.h 	logtab.h mpi.h mplogic.h \
	ecc_impl.h  ec.h  ecl-curve.h  ecl-exp.h  ecl.h  ecl-priv.h  ecp_fp.h  ecp.h\
	make-logtab 

## These are the header files that go into the distribution file
HDRS=mpi-config.h mpprime.h logtab.h mpi.h mplogic.h \
	ecc_impl.h  ec.h  ecl-curve.h  ecl-exp.h  ecl.h  ecl-priv.h  ecp_fp.h  ecp.h
	
## These are the documentation files that go into the distribution file

LIBOBJS = mpmontg.o mplogic.o mpi.o \
	ecl.o ecl_curve.o ecl_mult.o ecl_gf.o \
	ecp_aff.o ecp_jac.o ecp_mont.o \
	ec_naf.o ecp_jm.o \
	ec.o random.o secitem.o

LIBHDRS = mpi-config.h mpi-priv.h mpi.h\
	ecc_impl.h  ec.h  ecl-curve.h  ecl-exp.h  ecl.h  ecl-priv.h  ecp_fp.h  ecp.h

all: libs

help:
	@ echo ""
	@ echo "The following targets can be built with this Makefile:"
	@ echo ""
	@ echo "libecl.a     - arithmetic and prime testing library"
	@ echo "clean        - clean up objects and such"
	@ echo ""

.SUFFIXES: .c .o .i

.c.i:
	$(CC) $(CFLAGS) -E $< > $@

#.c.o: $*.h $*.c
#	$(CC) $(CFLAGS) -c $<

#---------------------------------------

all: clean libecl.a

$(LIBOBJS): $(LIBHDRS)

logtab.h: make-logtab
	$(PERL) make-logtab > logtab.h

mpi.o: mpi.c logtab.h $(LIBHDRS)
mplogic.o: mplogic.c mpi-priv.h mplogic.h $(LIBHDRS)
mpmontg.o: mpmontg.c mpi-priv.h mplogic.h mpprime.h $(LIBHDRS)
ec.o: ec.c $(LIBHDRS)
ecl.o: ecl.c $(LIBHDRS)
ecl_curve.o: ecl_curve.c $(LIBHDRS)
ecl_mult.o: ecl_mult.c $(LIBHDRS)
ecl_gf.o: ecl_gf.c $(LIBHDRS)
ecp_aff.o: ecp_aff.c $(LIBHDRS)
ecp_jac.o: ecp_jac.c $(LIBHDRS)
ecp_jm.o: ecp_jm.c $(LIBHDRS)
ecp_mont.o: ecp_mont.c $(LIBHDRS)
ec_naf.o: ec_naf.c $($LIBHDRS)  
random.o: random.c $($LIBHDRS)  
secitem.o: secitem.c $($LIBHDRS)  

libecl.a: $(LIBOBJS)
	$(AR) -cvr libecl.a $(LIBOBJS)
#	$(RANLIB) libecl.a
	cp libecl.a ../
	#$(CROSS)gcc $(CFLAGS)  -shared -o libecl.so -Wl -fPIC,-soname=libecl.so -Wl,-Bsymbolic -Wl,--whole-archive libecl.a -Wl,--no-whole-archive -L.
	#cp libecl.so ../
lib libs: libecl.a

mpi.i: mpi.h
ecl.i: ecl.h


#---------------------------------------

clean:
	rm -f *.o *.a *.so *.i
	rm -f core
	rm -f *~ .*~

# END
