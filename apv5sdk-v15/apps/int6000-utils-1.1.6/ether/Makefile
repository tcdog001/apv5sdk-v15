#!/usr/bin/make -f
# file: ether/Makefile

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=-lpcap
VPATH=../int6k:../tools
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h 
TOOLS=pcapdevs
PAGES=pcapdevs.7

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
#	sudo install -m 0555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
uninstall:
#	sudo cd ${BIN}; rm -f ${TOOLS}
clean:
	rm -f ${TRASH} ${TOOLS}
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<} 
%: %.o
	${CC} -o ${@} ${^} ${LFLAGS}

# ====================================================================
# Build C version;
# --------------------------------------------------------------------

pcapdevs: pcapdevs.o getoptv.o putoptv.o version.o error.o hexdecode.o gethwaddr.o

# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../tools/tools.mak
include ../ether/ether.mak
include ../make.def

