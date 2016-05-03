#!/bin/make -f
# file: ram/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=../int6k:../tools:../crypt:../ether:../ihpapi:../nvm
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h
TOOLS=config2cfg sdram 
PAGES=config2cfg.7 sdram.7

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
	if [ ! -z ${STRIP} ]; then ${STRIP} ${TOOLS}; fi
	sudo install -m 0555 -o ${OWNER} -g ${GROUP} ${TOOLS} ${BIN}
uninstall:
	sudo cd ${BIN}; rm -f ${TOOLS}
	sudo cd ${MAN}; rm -f ${PAGES}
clean:
	rm -f ${TOOLS} ${TRASH}
fresh: clean compile
%.o: %.c
	${CC} -c ${<} ${CFLAGS}
%: %.o
	${CC} -o ${@} ${^} ${LFLAGS}

# ====================================================================
# programs; 
# --------------------------------------------------------------------

config2cfg: config2cfg.o getoptv.o putoptv.o version.o error.o checksum_32.o hexencode.o todigit.o
sdram: sdram.o checksum_32.o

# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../int6k/int6k.mak
include ../tools/tools.mak
include ../crypt/crypt.mak
include ../ram/ram.mak
include ../pib/pib.mak

