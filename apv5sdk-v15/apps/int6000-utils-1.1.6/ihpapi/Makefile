#!/usr/bin/bash -f
# file: ihpapi/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
#
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h *.xml *.css

# ====================================================================
# 
# --------------------------------------------------------------------

.PHONY: all compile install scripts manuals clean

all compile install scripts manuals:
clean:
	rm -f ${TRASH} 

