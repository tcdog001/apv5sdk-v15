#!/bin/make -f
# file: scan/Makefile

# ====================================================================
# environment;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE
LFLAGS=
VPATH=../scan:../tools.h
TRASH=*.exe *.obj *.o t t.c *~ *.[0-9][0-9][0-9] 
FILES=Makefile *.mak *.sh *.txt *.c *.h
TOOLS=libscan.a
PAGES=scan.7

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
	install -m 0444 -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
install: compile library
	install -m 0755 -o ${OWNER} -g ${GROUP} ${TOOLS} ${BIN}
library: 
	install -m 0755 -o ${OWNER} -g ${GROUP} -d ${BIN}
cygwin: compile
	cp ${TOOLS} ${BIN}
uninstall:
	cd ${BIN}; rm -fv ${TOOLS}
	cd ${MAN}; rm -fv ${PAGES}
fresh: clean compile
clean:
	rm -f ${TOOLS} ${TRASH}

# ====================================================================
# rules;
# --------------------------------------------------------------------

%.o: %.c           
	${CC} ${CFLAGS} -c ${<} 
%: %.o
	${CC} ${LFLAGS} -o ${@} ${^}

# ====================================================================
# files;
# --------------------------------------------------------------------

libscan.a: copytoken.o havebreak.o haveclass.o havematch.o havetoken.o isbreak.o isclass.o ismatch.o isready.o istoken.o nextbreak.o nexttoken.o scanalnum.o scanalpha.o scanblock.o scanblockmatch.o scanbreak.o scanbreakmatch.o scancntrl.o scandigit.o scanflush.o scangroup.o scanident.o scaninput.o scanlimit.o scanprint.o scanquote.o scanquotematch.o scanreset.o scanspace.o scanstart.o scantoken.o scanuntil.o scanuntilmatch.o scanwhile.o scanwhilematch.o scanwrite.o showtoken.o tokensize.o tokentext.o tokentype.o tovalue.o
	rm -f ${@}
	$(AR) rc ${@} ${^}
	$(RANLIB) ${@}

# ====================================================================
# files;
# --------------------------------------------------------------------

include ../scan/scan.mak
include ../tools/tools.mak

