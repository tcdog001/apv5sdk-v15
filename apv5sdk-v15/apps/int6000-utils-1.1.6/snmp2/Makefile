#!/usr/bin/make -f
# file: snmp2/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS=-Wall -DMAKEFILE -D__GETOPT_H__ -D_GETOPT_DEFINED_ `net-snmp-config --cflags`
LFLAGS=`net-snmp-config --libs` `net-snmp-config --agent-libs`
VPATH=../pib:../nvm:../ram:../int6k:../tools:../ether:../mme:../crypt:../nodes:../int6k2:../ihpapi
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h 
TOOLS=itln_subagent2
PAGES=
MIBS=mibs/INTELLON-MIB.txt mibs/IHPAV-OPERATIONS-MIB.txt
MIB_DIR=/usr/local/share/snmp/mibs/

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
#library:
#	sudo install -m 0755 -o ${OWNER} -g ${GROUP} -d ${BIN}
scripts:
#manuals:
#	sudo install -m 0444 -o ${OWNER} -g ${GROUP} ${PAGES} ${MAN}
install: compile
	sudo install -m 4555 -o ${OWNER} -g ${GROUP} -s ${TOOLS} ${BIN}
	sudo install -m 755 -o ${OWNER} -g ${GROUP} -d ${MIB_DIR}
	sudo install -m 444 -o ${OWNER} -g ${GROUP} ${MIBS} ${MIB_DIR}
#uninstall:
#	sudo cd ${BIN}; rm -f int6k ${TOOLS}
#	sudo cd ${MAN}; rm -f int6k ${PAGES}
clean:
	rm -f ${TRASH} ${TOOLS}
test:
	@ ./test.sh
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<}
%: %.o
	${CC} ${LFLAGS} -o ${@} ${^} 

# ====================================================================
# programs;     
# --------------------------------------------------------------------

itln_subagent2: intellon-snmp2.o itln_subagent2.o chipset.o getoptv.o putoptv.o version.o error.o channel.o openchannel.o closechannel.o readpacket.o sendpacket.o EthernetHeader.o IntellonHeader.o ReadMME2.o SendMME.o DeviceList2.o hexdump.o Display.o hexdecode.o hexencode.o todigit.o ResetDevice.o ResetDevice2.o Request.o Failure.o Confirm.o MMECode.o TxFrame.o RxFrame.o EncodeEthernetHeader.o EncodeIntellonHeader.o DecodeEthernetPHYSettings.o DecodeResetToFactoryDefaults.o DecodeGetToneMap.o DecodeHostAction.o DecodeSetSDRAMCfg.o SequenceReadModule.o DecodeMfgString.o DecodeSetKey.o DecodeResetDevice.o DecodeGetRxToneMap.o DecodeReadModule.o DecodeNetworkInfo.o DecodeFlashNVM.o DecodeLinkStats.o DecodeStartMAC.o SequenceWriteModule.o SequenceWriteMemory.o DecodeWriteMemory.o bopen.o bclose.o bgets.o bgetnexts.o bgeto.o beof.o GetVersionInfo.o DecodeGetVersion.o checksum32.o DecodeWriteModule.o


# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../ihpapi/ihpapi.mak
include ../int6k/int6k.mak
include ../tools/tools.mak
include ../crypt/crypt.mak
include ../ether/ether.mak
include ../nodes/nodes.mak
include ../ram/ram.mak
include ../pib/pib.mak
include ../nvm/nvm.mak
include ../mme/mme.mak

