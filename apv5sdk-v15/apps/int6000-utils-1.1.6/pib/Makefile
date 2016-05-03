#!/usr/bin/make -f
# file: pib/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_
LFLAGS=
VPATH=../int6k:../tools:../crypt:../nodes
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9] *.test
FILES=Makefile *.mak *.sh *.txt *.c *.h *.xml *.css
#TOOLS=chkpib getpib modpib mrgpib psin psout pskey psgraph psnotch pxe setpib pibrump pibruin
TOOLS=chkpib getpib modpib setpib
#PAGES=chkpib.7 getpib.7 modpib.7 mrgpib.7 pibrump.7 pibruin.7 psin.7 psout.7 pskey.7 psgraph.7 psnotch.7 pxe.7 setpib.7 
PAGES=chkpib.7 getpib.7 modpib.7 setpib.7

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
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<} 
%: %.o
	${CC} ${LFLAGS} -o ${@} ${^} 

# ====================================================================
# Build C version;
# --------------------------------------------------------------------

chkpib: chkpib.o getoptv.o putoptv.o version.o pibpeek.o fdchecksum_32.o keys.o hexstring.o alert.o error.o hexdecode.o SHA256.o HPAVKeyNID.o pibcodes.o 
modpib: modpib.o getoptv.o putoptv.o version.o hexencode.o hexdecode.o todigit.o memincr.o fdchecksum_32.o keys.o uintspec.c error.o alert.o pibpeek.o hexstring.o pibfile.o SHA256.o HPAVKeyNID.o pibcodes.o 
getpib: getpib.o getoptv.o putoptv.o version.o uintspec.o basespec.o todigit.o hexout.o error.o 
mrgpib: mrgpib.o getoptv.o putoptv.o version.o error.o fdchecksum_32.o pibfile.o
pxe: pxe.o getoptv.o putoptv.o version.o emalloc.o checksum_32.o fdchecksum_32.o xmlattribute.o xmlvalue.o xmlcontent.o xmlnode.o xmlopen.o xmlscan.o xmledit.o xmlfree.o xmlschema.o pibfile.o keys.o alert.o error.o todigit.o output.o 
setpib: setpib.o getoptv.o putoptv.o version.o hexdump.o dataspec.o basespec.o error.o alert.o todigit.o uintspec.o bytespec.o pibfile.o endian.o checksum_32.o fdchecksum_32.o
psin: psin.o getoptv.o putoptv.o version.o hexdecode.o hexstring.o filepart.o todigit.o error.o pibfile.o piblock.o fdchecksum_32.o
pskey: pskey.o getoptv.o putoptv.o version.o error.o hexdecode.o hexstring.o filepart.o hexout.o pibfile.o SHA256Reset.o SHA256Write.o SHA256Block.o SHA256Fetch.o keys.o fdchecksum_32.o
psout: psout.o getoptv.o putoptv.o version.o error.o filepart.o pibfile.o fdchecksum_32.o
psnotch: psnotch.o getoptv.o putoptv.o version.o error.o todigit.o uintspec.o
psgraph: psgraph.o getoptv.o putoptv.o version.o error.o todigit.o uintspec.o 
pibrump: pibrump.o getoptv.o putoptv.o version.o error.o pibfile.o fdchecksum_32.o reword.o hexstring.o hexdecode.o memout.o rules.o
pibruin: pibruin.o getoptv.o putoptv.o version.o error.o pibfile.o fdchecksum_32.o reword.o hexstring.o hexdecode.o memout.o lookup.o assist.o bytespec.o basespec.o ipv4spec.o ipv6spec.o todigit.o expect.o endian.o emalloc.o rules.o ParseRule.o piblock.o filepart.o

# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../tools/tools.mak
include ../nodes/nodes.mak
include ../crypt/crypt.mak
include ../pib/pib.mak

