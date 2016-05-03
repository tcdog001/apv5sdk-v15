#!/usr/bin/make -f
# file: classes/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CXXFLAGS+=
LDFLAGS+=
VPATH=../classes
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9] 
TOOLS=
FILES=*.cpp *.hpp *.h *.sh *.mak Makefile Makefile.*
PAGES=

# ====================================================================
# pseudo targets;
# --------------------------------------------------------------------

all compile: ${TOOLS}
compact: compile
	if [ ! -z ${STRIP} ]; then ${STRIP} ${TOOLS}; fi
	if [ -x /usr/local/bin/upx ]; then upx --best ${TOOLS}; fi
library:
	install -m ${DIR_PERM} -o 0 -g 0 -d ${BIN}
scripts:
manuals:
	install -m ${MAN_PERM} -o 0 -g 0 ${PAGES} ${MAN}
install: compile
	install -m ${BIN_PERM} -o 0 -g 0 ${TOOLS} ${BIN}
uninstall:
	cd ${BIN}; rm -f ${TOOLS}
check:
	${SHELL} classes.sh
fresh: clean compile
clean:
	chmod 0644 *.cpp *.hpp *.mak
	chmod 0755 Makefile *.sh
	rm -f ${TOOLS} ${TRASH}

# ====================================================================
# rules;
# --------------------------------------------------------------------

%: %.o 
	${CXX} ${LDFLAGS} -o ${@} ${^}
%.o: %.cpp
	${CXX} ${CXXFLAGS} -c ${<}
%.hpp: %.hpp

# ====================================================================
# 
# --------------------------------------------------------------------

include ../classes/classes.mak

