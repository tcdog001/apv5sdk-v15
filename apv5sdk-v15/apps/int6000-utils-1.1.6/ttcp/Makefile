#!/usr/bin/make
# file: ttcp/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
#
# --------------------------------------------------------------------

TRASH=*~ t t.* *.[0-9][0-9][0-9]
TOOLS=ttcp
PAGES=ttcp.7
FILES=Makefile *.mak *.sh *.txt *.c *.h *.xml *.css

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
install: compile library
	sudo install -m 4555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
uninstall:
	sudo cd ${BIN}; rm -f ${TOOLS}
manuals:
	sudo install -m 0444 -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
fresh: clean compile
clean:
	rm -f ${TRASH} ${TOOLS} 
%.o: %.c
	${CC} ${CFLAGS} -c ${<} 
%: %.o
	${CC} -o ${@} ${^} ${LFLAGS}

