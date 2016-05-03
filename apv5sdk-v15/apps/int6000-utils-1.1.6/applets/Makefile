#!/bin/make -f
# file: applets/Makefile

# ====================================================================
#
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
#
# --------------------------------------------------------------------

TRASH=t t.* *~ *.[0-9][0-9][0-9]
FILES=*.c *.h *.txt *.mak Makefile Makefile.*
TOOLS=
PAGES=applets.7a burstadrinadrtestloop.7a burstreadtest6400.7a burstwrite6400.7a burstwritereadtest6400.7a connection.7a memctl.7a flashtest.7a marchloop.7a linesynctest.7a nvramtest.7a sdramtest.7a

# ====================================================================
#
# --------------------------------------------------------------------

.PHONY: compile library scripts manuals install uninstall clean fresh

# ====================================================================
#
# --------------------------------------------------------------------

all compile:
manuals:
	sudo install -m 0444 -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
scripts library install:
uninstall:
#	sudo cd ${BIN}; rm -f int6k ${TOOLS}
	sudo cd ${MAN}; rm -f int6k ${PAGES}
clean:
	rm -f ${TOOLS} ${TRASH}
fresh: clean compile

# ====================================================================
#
# --------------------------------------------------------------------

