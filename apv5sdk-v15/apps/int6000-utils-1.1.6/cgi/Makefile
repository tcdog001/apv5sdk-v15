#!/bin/make -f
# file: cgi/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE 
CFLAGS=-Wall -DMAKEFILE -DEMBEDDED 
LFLAGS=
VPATH=../tools:../ether:../nvm:../scan:../int6k:../crypt:../pib
TRASH=*.exe *.obj *.o t t.c *~ *.[0-9][0-9][0-9] 
TOOLS=intellon.cgi 
IMAGES=intellon.gif 
STYLES=cgi.css

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
install: compile library 
	install -m 4555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${WWW}/cgi-bin
	install -m 0555 -o ${OWNER} -g ${GROUP} ${IMAGES} ${WWW}/images
	install -m 0555 -o ${OWNER} -g ${GROUP} ${STYLES} ${WWW}/styles
	rm -fr /var/state/intellon.* 
library: 
	install -m 2755 -o ${OWNER} -g ${GROUP} -d ${WWW}/cgi-bin
	install -m 2755 -o ${OWNER} -g ${GROUP} -d ${WWW}/images 
	install -m 2755 -o ${OWNER} -g ${GROUP} -d ${WWW}/styles
uninstall:
fresh: clean compile
clean:
	rm -f ${TOOLS} ${TRASH}
%.o: %.c           
	${CC} ${CFLAGS} -c ${<} 
%.cgi: %.o
	${CC} ${LFLAGS} -o ${@} ${^}

# ====================================================================
#  
# --------------------------------------------------------------------

intellon.cgi: intellon.o DivisionHeader.o DivisionFooter.o DivisionOption.o DivisionDevice.o status.c hexstring.o hexdecode.o hexdump.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o hexencode.o todigit.o session.o OptionTopology.o OptionIdentity.o OptionSecurity.o OptionPriority.o OptionFirmware.o channel.o openchannel.o sendpacket.o readpacket.o closechannel.o SessionLoad.c SessionEdit.o SessionTask.o SessionSave.o SessionPeek.o nvmspan.o nvmpeek.o checksum_32.o strfbits.o scan.o chipset.o filepart.o HPAVKeyDAK.o HPAVKeyNMK.o HPAVKeyNID.o SHA256Reset.o SHA256Write.o SHA256Block.o SHA256Fetch.o pibcodes.o error.c CGILocalPLD.o CGIWriteMOD.o CGIFlashMOD.o CGISetKey.o CGIRestore.o output.o anynic.o

stylesheet: stylesheet.c styles.o output.o

# ====================================================================
# files;
# --------------------------------------------------------------------

include ../tools/tools.mak
include ../int6k/int6k.mak
include ../scan/scan.mak
include ../cgi/cgi.mak

