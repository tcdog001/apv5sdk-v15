#!/usr/bin/make -f
# file: mme/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=../int6k:../tools:../sdram
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h *.xml *.css
TOOLS=

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
scripts manuals:
install: compile library
#	sudo install -m 0555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
uninstall:
#	sudo cd ${BIN}; rm -f ${TOOLS}
clean:
	rm -f ${TOOLS} ${TRASH}
test:
	@ ./test.sh
fresh: clean compile
%.o: %.c
	${CC} -c ${<} ${CFLAGS}
 : %.o
	${CC} -o ${@} ${^} ${LFLAGS}

# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../mme/mme.mak

