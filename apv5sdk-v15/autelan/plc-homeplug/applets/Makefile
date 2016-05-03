#!/bin/make -f
# file: applets/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

TRASH=t t.* *~ *.[0-9][0-9][0-9]
FILES=*.c *.h *.txt *.mak Makefile Makefile.*
TOOLS=
PAGES=memctl.7a AR7420-goodbye.7a AR7420-hello.7a 
PAGES+=INT6400-sdramtest.7a AR7420-flashtest.7a QCA6410-flashtest.7a

# ====================================================================
# pseudo targets;
# --------------------------------------------------------------------

all compile:
manuals:
	install -m ${MAN_PERM} -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
scripts:
library:
install:
uninstall:
#	cd ${BIN}; rm -f int6k ${TOOLS}
	cd ${MAN}; rm -f int6k ${PAGES}
fresh: clean compile
check:
	${SHELL} applets.sh
clean:
	chmod 0755 Makefile *.sh 
	chmod 0444 *.md5 *.nvm *.7a
	rm -f ${TOOLS} ${TRASH}

# ====================================================================
#
# --------------------------------------------------------------------

