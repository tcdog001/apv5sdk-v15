#!/bin/make -f
# file: tools/Makefile

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=../tools:../crypt:../nodes
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h *.xml *.css
TOOLS=libmtools.a argv odc odd odx timer grab
PAGES=odc.7 odd.7 odx.7 ilt.7 grab.7

# ====================================================================
# targets;
# --------------------------------------------------------------------

.PHONY: all compile compact install scripts manuals uninstall library clean fresh

# ====================================================================
# targets;
# --------------------------------------------------------------------

all compile: ${TOOLS}
compact: compile
	if [ ! -z ${STRIP} ]; then ${STRIP} ${TOOLS}; fi
	if [ -x /usr/local/bin/upx ]; then upx --best ${TOOLS}; fi
library:
#	sudo install -m 0755 -o ${OWNER} -g ${GROUP} -d ${BIN}
scripts manuals:
manuals:
	sudo rm -f /usr/share/man/*/{ilt,tools}.*
	sudo install -m 0444 -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
install: compile library
	sudo install -m 0555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
uninstall:
	sudo cd ${BIN}; rm -f ${TOOLS}
	sudo cd ${MAN}; rm -f ${PAGES}
clean:
	rm -f ${TOOLS} ${TRASH}
	chmod 0644 *.c *.h *.7
	chmod 0755 Makefile *.mak *.sh
test:
	@ ./test.sh
fresh: clean compile
%.o: %.c
	${CC} -c ${<} ${CFLAGS}
%: %.o
	${CC} -o ${@} ${^} ${LFLAGS}

# ====================================================================
#
# --------------------------------------------------------------------

argv: argv.o
grab: grab.o getoptv.o putoptv.o version.o basespec.o todigit.o error.o efreopen.o
odd: odd.o getoptv.o putoptv.o version.o hexview.o emalloc.o error.o 
odc: odc.o getoptv.o putoptv.o version.o hexview.o emalloc.o error.o alert.o 
odx: odx.o getoptv.o putoptv.o version.o hexview.o error.o output.o xmlschema.o
timer: timer.o getoptv.o putoptv.o version.o error.o

# ====================================================================
#
# --------------------------------------------------------------------

libmtools.a: alert.o binout.o bytespec.o checkfilename.o checksum32.o dataspec.o decdecode.o decout.o emalloc.o endian.o error.o fdchecksum32.o filepart.o getoptv.o hexdecode.o hexdump.o hexencode.o hexin.o hexout.o hexstring.o hexview.o regview.o ipv4spec.o ipv6spec.o memdecr.o memincr.o memswap.o output.o putoptv.o strdecr.o strfbits.o strincr.o synonym.o timer.o todigit.o typename.o basespec.o uintspec.o version.o
	rm -f ${@}
	$(AR) rc ${@} ${^}
	$(RANLIB) ${@}

# ====================================================================
#
# --------------------------------------------------------------------

include ../nodes/nodes.mak
include ../make.def

