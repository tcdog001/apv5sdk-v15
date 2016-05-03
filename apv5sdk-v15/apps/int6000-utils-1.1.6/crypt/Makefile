#!/usr/bin/make -f
# file: crypt/Makefile

# ====================================================================
# folders;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS = -Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=../tools
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
TOOLS=hpavkey hpavkeys rkey mac2pw mac2pwd libkeys.a
FILES=*.c *.h *.sh *.xml *.txt *.mak Makefile Makefile.*
PAGES=hpavkey.7 hpavkeys.7 rkey.7 mac2pw.7 mac2pwd.7

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
	sudo install -m 0755 -o 0 -g 0 -d ${BIN}
scripts:
manuals:
	sudo install -m 0755 -o 0 -g 0 ${PAGES} ${MAN}
install: compile
	sudo install -m 0555 -o 0 -g 0 ${TOOLS} ${BIN}
uninstall:
	sudo cd ${BIN}; rm -f ${TOOLS}
	sudo cd ${MAN}; rm -f ${PAGES}
clean:
	rm -f ${TOOLS} ${TRASH}
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<}
%: %.o 
	${CC} ${LFLAGS} -o ${@} ${^}

# ====================================================================
# targets;
# --------------------------------------------------------------------

hpavkey: hpavkey.o getoptv.o putoptv.o version.o HPAVKeySHA.o HPAVKeyDAK.o HPAVKeyNMK.o HPAVKeyNID.o HPAVKeyOut.o SHA256.o error.o uintspec.o todigit.o hexout.o 
hpavkeys: hpavkeys.o getoptv.o putoptv.o version.o HPAVKeySHA.o HPAVKeyDAK.o HPAVKeyNMK.o HPAVKeyNID.o HPAVKeyOut.o SHA256.o error.o uintspec.o todigit.o hexout.o
rkey: rkey.o getoptv.o putoptv.o version.o HPAVKeySHA.o HPAVKeyDAK.o HPAVKeyNMK.o HPAVKeyNID.o HPAVKeyOut.o SHA256.o error.o uintspec.c todigit.c strincr.o hexout.o
mac2pw: mac2pw.o getoptv.o putoptv.o version.o uintspec.o hexencode.o todigit.o error.o MACPasswords.o 
mac2pwd: mac2pwd.o getoptv.o putoptv.o version.o uintspec.o hexencode.o todigit.o error.o MACPasswords.o 
libkeys.a: HPAVKeySHA.o HPAVKeyDAK.o HPAVKeyNMK.o HPAVKeyNID.o HPAVKeyOut.o SHA256.o MACPasswords.o
	rm -f ${@}
	$(AR) rc ${@} ${^}
	$(RANLIB) ${@}

# ====================================================================
# 
# --------------------------------------------------------------------

include ../tools/tools.mak
include ../crypt/crypt.mak

