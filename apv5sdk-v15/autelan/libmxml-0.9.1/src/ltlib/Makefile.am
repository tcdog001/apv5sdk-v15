#
# Minimal XML library
#
# /src (main) subdir Makefile.am
# $Id: Makefile.am,v 1.1 2003/08/06 13:53:03 jonnymind Exp $


SUBDIRS =

include_HEADERS= \
   ../include/mxml.h \
   ../include/mxml_defs.h \
   ../include/mxml_file.h

   
#
# Dynamic library compilation
#


if BUILD_LT
libmxml_la_LDFLAGS = -version-info 1:1:0
lib_LTLIBRARIES = libmxml.la

libmxml_la_SOURCES = \
   ../src/mxml_attribute.c \
   ../src/mxml_document.c \
   ../src/mxml_error.c \
   ../src/mxml_file.c \
   ../src/mxml_index.c \
   ../src/mxml_iterator.c \
   ../src/mxml_node.c \
   ../src/mxml_output.c \
   ../src/mxml_path.c \
   ../src/mxml_refil.c \
   ../src/mxml_sgs.c

   INCLUDES = -I../include

endif

