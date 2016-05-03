#!/usr/bin/make -f
# file: hpav/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=../tools:../ether:../mme:../int6k         
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h *.xml *.css
TOOLS=arpc hpav 
PAGES=hpav.7 hpavd.7

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
clean:
	rm -f ${TRASH} ${TOOLS}
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<} 
%: %.o
	${CC} ${LFLAGS} -o ${@} ${^} 

# ====================================================================
# programs; 
# --------------------------------------------------------------------

hpav: hpav.o getoptv.o putoptv.o version.o hexdump.o hexdecode.o hexstring.o error.o channel.o openchannel.o closechannel.o sendpacket.o readpacket.o MMEPeek.o MMEName.o MMEMode.o endian.h version.h ether.h 

hpavd: hpavd.o getoptv.o putoptv.o version.o hexdump.o hexdecode.o hexstring.o error.o channel.o openchannel.o closechannel.o sendpacket.o readpacket.o MMEPeek.o MMEName.o MMEMode.o endian.h version.h ether.h 

arpc: arpc.o getoptv.o putoptv.o version.o hexdump.o uintspec.o todigit.o error.o channel.o openchannel.o closechannel.o sendpacket.o readpacket.o hexwrite.c


# ====================================================================
# includes; 
# --------------------------------------------------------------------

include ../tools/tools.mak
include ../int6k/int6k.mak
include ../hpav/hpav.mak
include ../mme/mme.mak

