#
# Minimal XML library
#
# /test subdir Makefile.am
# $Id: Makefile.am,v 1.2 2003/08/09 16:46:30 jonnymind Exp $

SUBDIRS=

#
# Check if we must build the tests
#
if BUILD_TESTS

noinst_PROGRAMS = mxml_test testattrib

mxml_test_SOURCES = mxml_test.c
mxml_test_LDADD=../lib/libmxml.a

testattrib_SOURCES = testattrib.c
testattrib_LDADD=../lib/libmxml.a

endif

INCLUDES = -I../include
