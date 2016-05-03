#!/usr/bin/make -f
# file: coqos/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_ 
LFLAGS=
VPATH=../int6k:../tools:../ether:../mme
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h 
TOOLS=coqos_add coqos_info coqos_man coqos_mod coqos_rel 
PAGES=coqos_add.7 coqos_info.7 coqos_man.7 coqos_mod.7 coqos_rel.7 

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
manuals:
	sudo install -m 0444 -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
install: compile library
#	sudo install -m 4555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
	if [ ! -z ${STRIP} ]; then ${STRIP} ${TOOLS}; fi
	sudo install -m 4555 -o ${OWNER} -g ${GROUP} ${TOOLS} ${BIN}
uninstall:
	sudo cd ${BIN}; rm -f int6k ${TOOLS}
	sudo cd ${MAN}; rm -f int6k ${PAGES}
clean:
	rm -f ${TRASH} ${TOOLS}
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<}
%: %.o
	${CC} ${LFLAGS} -o ${@} ${^} 

# ====================================================================
# programs;     
# --------------------------------------------------------------------

coqos_add: coqos_add.o getoptv.o putoptv.o version.o error.o devices.o bytespec.o ipv4spec.o ipv6spec.o basespec.o bytespec.o endian.o openchannel.o closechannel.o synonym.o hexencode.o Request.o SendMME.o Failure.o Confirm.o hexstring.o ReadMME.o todigit.o hexdecode.o MMECode.o readpacket.o hexdump.o sendpacket.o uintspec.o channel.o Display.o lookup.o expect.o assist.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

coqos_info: coqos_info.o getoptv.o putoptv.o version.o error.o devices.o sendpacket.o readpacket.o ReadMME.o SendMME.o Request.o Confirm.o Failure.o hexstring.o openchannel.o hexencode.o closechannel.o synonym.o hexdump.o hexdecode.o MMECode.o todigit.o channel.o Display.o lookup.o assist.o expect.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

coqos_mod: coqos_mod.o getoptv.o putoptv.o version.o error.o devices.o openchannel.o closechannel.o synonym.o Request.o SendMME.o Failure.o Confirm.o ReadMME.o readpacket.o hexdump.o sendpacket.o hexencode.o hexstring.o hexdecode.o MMECode.o todigit.o channel.o Display.o lookup.o assist.o expect.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

coqos_rel: coqos_rel.o getoptv.o putoptv.o version.o error.o devices.o SendMME.o ReadMME.o Request.o Confirm.o Failure.o hexstring.o MMECode.o readpacket.o sendpacket.o hexdecode.o openchannel.o closechannel.o synonym.o hexencode.o hexdump.o todigit.o channel.o Display.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

coqos_man: coqos_man.o getoptv.o putoptv.o version.o error.o devices.o SendMME.o ReadMME.o Request.o Confirm.o Failure.o hexstring.o MMECode.o readpacket.o sendpacket.o hexdecode.o openchannel.o closechannel.o synonym.o hexencode.o hexdump.o todigit.o channel.o Display.o uintspec.o devices.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../int6k/int6k.mak
include ../tools/tools.mak
include ../crypt/crypt.mak
include ../ether/ether.mak
include ../nodes/nodes.mak
include ../ram/ram.mak
include ../pib/pib.mak
include ../nvm/nvm.mak
include ../mme/mme.mak
