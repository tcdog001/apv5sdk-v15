#!/bin/make -f
# file: int6k/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_ 
LFLAGS=
VPATH=../pib:../nvm:../ram:../int6k:../tools:../ether:../ihpapi:../mme:../crypt:../nodes
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.c *.h *.xml *.css
TOOLS=int6k2 
PAGES=int6k2.7 

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
	sudo install -m 4555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
uninstall:
	sudo cd ${BIN}; rm -f int6k ${TOOLS}
clean:
	rm -f ${TRASH} ${TOOLS}
test:
	@ ./test.sh
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<}
%: %.o
	${CC} -o ${@} ${^} ${LFLAGS}

# ====================================================================
# programs;     
# --------------------------------------------------------------------

int6k2: int6k2.o getoptv.o putoptv.o version.o uintspec.o alert.o error.o emalloc.o fdchecksum32.o checksum32.o checkfilename.o hexencode.o hexdecode.o todigit.o hexdump.o nvmfile.o nvmpeek.o pibfile.o pibpeek.o keys.o bopen.o bclose.o bgets.o bgetnexts.o bgeto.o beof.o ReadMAC2.o ReadPIB2.o Identity2.o VersionInfo2.o ResetDevice2.o NetworkInfo2.o MfgString2.o SetKey2.o SetSDRAM2.o FactoryDefaults2.o Upgrade2.o HostActionWait2.o SendMME.o ReadMME2.o Request.o Confirm.o Failure.o GetManufacturerInfo.o GetNetworkInfo.o GetVersionInfo.o InitializeDevice.o ResetDevice.o ResetToFactoryDefaults.o RxFrame.o SequenceTerminate.o SetKey.o SetSDRAMConfig.o TxFrame.o UpdateDevice.o EncodeEthernetHeader.o EncodeIntellonHeader.o DecodeEthernetPHYSettings.o DecodeFlashNVM.o DecodeGetToneMap.o DecodeGetRxToneMap.o DecodeGetVersion.o DecodeHostAction.o DecodeLinkStats.o DecodeMfgString.o DecodeNetworkInfo.o DecodeResetDevice.o DecodeResetToFactoryDefaults.o DecodeSetKey.o DecodeSetSDRAMCfg.o DecodeStartMAC.o DecodeReadModule.o DecodeWriteMemory.o DecodeWriteModule.o FindFWHeader.o SequenceReadModule.o SequenceWriteMemory.o SequenceWriteModule.o openchannel.o closechannel.o readpacket.o sendpacket.o RdModWrPib2.o xmlopen.o xmlfree.o xmledit.o xmlscan.o xmlattribute.o xmlcontent.o xmlnode.o xmlvalue.o MMECode.o chipset.o strfbits.o hexstring.o pibcodes.o channel.o checksum_32.o fdchecksum_32.o

# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../ihpapi/ihpapi.mak
include ../int6k/int6k.mak
include ../tools/tools.mak
include ../nodes/nodes.mak
include ../crypt/crypt.mak
include ../ether/ether.mak
include ../pib/pib.mak
include ../ram/ram.mak
include ../nvm/nvm.mak
include ../mme/mme.mak

