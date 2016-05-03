#!/usr/bin/make -f
# file: efsu/Makefile

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=../tools:../ether:../pcap
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h *.xml *.css
TOOLS=efsu
PAGES=efsu.7

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
	sudo install -m 0755 -o ${OWNER} -g ${GROUP} -d ${BIN}
scripts:
manuals:
	sudo install -m 0444 -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
install: compile library
#	sudo install -m 4555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
	if [ ! -z ${STRIP} ]; then ${STRIP} ${TOOLS}; fi
	sudo install -m 4555 -o ${OWNER} -g ${GROUP} ${TOOLS} ${BIN}
uninstall:
	sudo cd ${BIN}; rm -f ${TOOLS}
	sudo cd ${MAN}; rm -f ${PAGES}
fresh: clean compile
clean:
	rm -f ${TRASH} ${TOOLS} 
	chmod 0644 *.c *.7
	chmod 0755 Makefile *.mak *.sh
	chmod 0444 *.hex *.REQ *.RSP
%.o: %.c
	${CC} ${CFLAGS} -c ${<} 
%: %.o
	${CC} -o ${@} ${^} ${LFLAGS}

# ====================================================================
# programs;
# --------------------------------------------------------------------

efsu: efsu.o getoptv.o putoptv.o version.o error.o todigit.o hexencode.o uintspec.o hexdump.o hexin.o channel.o openchannel.o sendpacket.o readpacket.o closechannel.o todigit.o basespec.o channel.o synonym.o

# ====================================================================
# 
# --------------------------------------------------------------------

include ../tools/tools.mak
include ../ether/ether.mak
include ../efsu/efsu.mak
include ../make.def

