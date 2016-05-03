#!/usr/bin/make -f
# file: VisualStudioNET/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# 
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=chknvm:chkpib:int6k:int6kf:modpib
TRASH=*.o t t.* *.0?? *~ *.log *.obj *.exe *.suo *.ncb
TOOLS=

# ====================================================================
# define pseudo targets;
# --------------------------------------------------------------------

.PHONY: all compile library scripts manuals install uninstall clean fresh

all compile: ${TOOLS}
library:
#	sudo install -m 0755 -o ${OWNER} -g ${GROUP} -d ${BIN}
scripts manuals:
install: compile library
#	sudo install -m 0555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
uninstall:
#	sudo cd ${BIN}; rm -f ${TOOLS}
#	sudo cd ${MAN}; rm -f ${PAGES}
clean:
	rm -fr */Debug */Release/*.obj */Release/*.pdb */Release/*.idb */Release/*.htm
	rm -f ${TOOLS} ${TRASH}
fresh: clean compile

