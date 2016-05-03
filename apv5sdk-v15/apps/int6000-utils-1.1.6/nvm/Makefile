#!/usr/bin/make -f
# file: nvm/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=../int6k:../tools:../crypt:../ether:../ihpapi:../ram
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9] *.test
FILES=Makefile *.mak *.sh *.txt *.c *.h *.xml *.css
TOOLS=chknvm chknvm2 setnvm nvmsplit nvmmerge
PAGES=chknvm.7 chknvm2.7 setnvm.7 nvmsplit.7 nvmmerge.7

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
scripts:
manuals:
	sudo install -m 0444 -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
library:
	sudo install -m 0755 -o ${OWNER} -g ${GROUP} -d ${BIN}
install: compile library
#	sudo install -m 0555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
	if [ ! -z ${STRIP} ]; then ${STRIP} ${TOOLS}; fi
	sudo install -m 0555 -o ${OWNER} -g ${GROUP} ${TOOLS} ${BIN}
uninstall:
	sudo cd ${BIN}; rm -f ${TOOLS}
clean:
	rm -f ${TRASH} ${TOOLS}
test:
	@ ./test.sh
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<} 
%: %.o
	${CC} -o ${@} ${^} ${LFLAGS}

# ====================================================================
# programs; 
# --------------------------------------------------------------------

chknvm: chknvm.o getoptv.o putoptv.o version.o error.o checksum_32.o fdchecksum_32.o nvmpeek.o sdrampeek.o alert.o strfbits.o filepart.o
chknvm2: chknvm2.o getoptv.o putoptv.o version.o checksum_32.o strfbits.o filepart.o error.o chknvm2.o nvmpeek.o 
setnvm: setnvm.o getoptv.o putoptv.o version.o error.o checksum_32.o fdchecksum_32.o nvmpeek.o strfbits.o basespec.o todigit.o
nvmsplit: nvmsplit.o getoptv.o putoptv.o version.o error.o checksum_32.o
nvmmerge: nvmmerge.o getoptv.o putoptv.o version.o error.o checksum_32.o 

# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../int6k/int6k.mak
include ../tools/tools.mak
include ../crypt/crypt.mak
include ../ether/ether.mak
include ../ram/ram.mak
include ../nvm/nvm.mak

