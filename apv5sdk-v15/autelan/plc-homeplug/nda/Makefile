#!/usr/bin/make -f
# file: nda/Makefile

# ====================================================================
# environment definitions;
# --------------------------------------------------------------------

include ../make.def

# ====================================================================
# symbols;
# --------------------------------------------------------------------

CFLAGS+=
LDFLAGS+=-lm
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9] 
FILES=Makefile *.mak *.sh *.txt *.c *.h 
TOOLS=plcecho plcinit plcmcinfo plcmod psgen ampinit int6kecho int6kp int6kprobe 
PAGES=plcecho.7 plcinit.7 plcmcinfo.7 plcmod.7 psgen.7 ampinit.7 int6kecho.7 int6kp.7 int6kprobe.7

# ====================================================================
# pseudo targets;
# --------------------------------------------------------------------

all compile: ${TOOLS}
compact: compile
	if [ ! -z ${STRIP} ]; then ${STRIP} ${TOOLS}; fi
	if [ -x /usr/local/bin/upx ]; then upx --best ${TOOLS}; fi
library:
	install -d ${BIN}
scripts:
manuals:
	install ${PAGES} ${MAN}
install: compile library
	install ${TOOLS} ${BIN}
uninstall:
	cd ${BIN}; rm -f plc ${TOOLS}
	cd ${MAN}; rm -f plc ${PAGES}
check:
	${SHELL} nda.sh
fresh: clean compile
clean:
	chmod 0644 *.c *.h *.txt *.mak
	chmod 0755 Makefile *.sh
	rm -f ${TRASH} ${TOOLS}

# ====================================================================
# rules;        
# --------------------------------------------------------------------

%: %.o
	${CC} -o ${@} ${^} ${LDFLAGS}
%.o: %.c
	${CC} -c ${<} ${CFLAGS}

# ====================================================================
# programs;     
# --------------------------------------------------------------------

ampinit: ampinit.o Attributes1.o Confirm.o Devices.o Display.o EraseFlashMemory1.o EthernetHeader.o FactoryDefaults.o Failure.o FlashPTS.o FragmentHeader.o HostActionResponse.o Identity1.o MMECode.o NVRAMInfo.o NetInfo2.o PushButton.o QualcommHeader.o ReadFirmware1.o ReadFlashFirmware.o ReadFlashParameters.o ReadFMI.o ReadMFG.o ReadMME.o ReadParameters1.o RemoteHosts.o Request.o ResetDevice.o SDRAMInfo.o SendMME.o SetNMK.o StationRole.o UnwantedMessage.o VersionInfo1.o WaitForReset.o WaitForStart.o WatchdogReport.o WriteCFG.o WriteNVM.o WritePIB.o basespec.o channel.o checkfilename.o checksum32.o chipset.o closechannel.o error.o fdchecksum32.o getoptv.o hexdecode.o hexdump.o hexencode.o hexout.o hexstring.o keys.o nvmfile1.o nvram.o openchannel.o pibfile1.o pibpeek1.o putoptv.o readpacket.o sdramfile.o sdrampeek.o sendpacket.o strfbits.o synonym.o todigit.o typename.o uintspec.o version.o
int6kecho: int6kecho.o Devices.o Display.o EthernetHeader.o Failure.o Loopback.o MMECode.o QualcommHeader.o ReadMME.o SendMME.o UnwantedMessage.o assist.o channel.o checkfilename.o checksum32.o closechannel.o error.o codelist.o fdchecksum32.o getoptv.o hexdecode.o hexdump.o hexencode.o hexload.o hexstring.o lookup.o openchannel.o putoptv.o readpacket.o sendpacket.o strfbits.o synonym.o todigit.o typename.o uintspec.o version.o
int6kp: int6kp.o Attributes1.o Confirm.o Devices.o Display.o EraseFlashMemory1.o EthernetHeader.o FactoryDefaults.o Failure.o FlashPTS.o FragmentHeader.o HostActionResponse.o Identity1.o MMECode.o NVRAMInfo.o NetInfo1.o PLCSelect.o PushButton.o QualcommHeader.o ReadFirmware1.o ReadFlashFirmware.o ReadFlashParameters.o ReadFMI.o ReadMFG.o ReadMME.o ReadParameters1.o Request.o ResetDevice.o SDRAMInfo.o SendMME.o SetNMK.o StationRole.o UnwantedMessage.o VersionInfo1.o WaitForReset.o WaitForStart.o WatchdogReport.o WriteCFG.o WriteNVM.o WritePIB.o basespec.o channel.o checkfilename.o checksum32.o chipset.o closechannel.o error.o fdchecksum32.o getoptv.o hexdecode.o hexdump.o hexencode.o hexstring.o keys.o nvmfile1.o nvram.o openchannel.o pibfile1.o pibpeek1.o putoptv.o readpacket.o sdramfile.o sdrampeek.o sendpacket.o strfbits.o synonym.o todigit.o typename.o uintspec.o version.o
int6kprobe: int6kprobe.o Confirm.o Devices.o Display.o EthernetHeader.o Failure.o MMECode.o NetworkProbe.o QualcommHeader.o ReadMME.o Request.o SendMME.o UnwantedMessage.o channel.o closechannel.o error.o getoptv.o hexdecode.o hexdump.o hexencode.o hexstring.o openchannel.o putoptv.o readpacket.o sendpacket.o synonym.o todigit.o uintspec.o version.o
plcecho: plcecho.o Devices.o Display.o EthernetHeader.o Failure.o Loopback.o MMECode.o QualcommHeader.o ReadMME.o SendMME.o UnwantedMessage.o assist.o channel.o checkfilename.o checksum32.o closechannel.o error.o codelist.o fdchecksum32.o getoptv.o hexdecode.o hexdump.o hexencode.o hexload.o hexstring.o lookup.o openchannel.o putoptv.o readpacket.o sendpacket.o strfbits.o synonym.o todigit.o typename.o uintspec.o version.o
plcinit: plcinit.o AccessLevelPTS.o Attributes2.o Confirm.o Devices.o Display.o EraseFlashMemory2.o EraseFlashSector.o EthernetHeader.o FactoryDefaults.o Failure.o FlashDevice2.o FlashFirmware.o FlashNVM.o FlashParameters.o FlashSoftloader.o FragmentHeader.o HostActionResponse.o Identity2.o MMECode.o ModuleCommit.o ModuleErase.o ModuleRead.o ModuleSession.o ModuleSpec.o ModuleWrite.o NVRAMInfo.o NetInfo2.o PLCSelect.o PushButton.o QualcommHeader.o ReadFirmware1.o ReadFlashMemory2.o ReadFMI.o ReadMFG.o ReadMME.o ReadParameters2.o RemoteHosts.o Request.o ResetDevice.o SDRAMInfo.o SendMME.o SetNMK.o StartFirmware1.o StationRole.o UnwantedMessage.o VersionInfo2.o WaitForReset.o WaitForStart.o WatchdogReport.o WriteNVM.o WritePIB.o basespec.o channel.o checkfilename.o checksum32.o chipset.o closechannel.o error.o fdchecksum32.o getoptv.o hexdecode.o hexdump.o hexencode.o hexout.o hexstring.o keys.o nvmfile2.o nvram.o openchannel.o pibfile2.o pibpeek2.o putoptv.o readpacket.o sdramfile.o sdrampeek.o sendpacket.o strfbits.o synonym.o todigit.o typename.o uintspec.o version.o
plcmcinfo: plcmcinfo.o Confirm.o Devices.o Display.o EthernetHeader.o Failure.o FragmentHeader.o MMECode.o MulticastInfo1.o MulticastInfo2.o QualcommHeader.o ReadFMI.o ReadMME.o Request.o SendMME.o UnwantedMessage.o basespec.o channel.o checksum32.o chipset.o closechannel.o decdecode.o decstring.o error.o getoptv.o hexdecode.o hexdump.o hexencode.o hexout.o hexstring.o nvram.o openchannel.o putoptv.o readpacket.o sdramfile.o sdrampeek.o sendpacket.o strfbits.o synonym.o todigit.o typename.o uintspec.o version.o
plcmod: plcmod.o AccessLevelPTS.o Confirm.o Devices.o Display.o EthernetHeader.o Failure.o keys.o MMECode.o ModuleCommit.o ModuleDump.o ModuleRead.o ModuleSession.o ModuleSpec.o ModuleWrite.o QualcommHeader.o ReadMME.o Request.o SendMME.o UnwantedMessage.o basespec.o channel.o checkfilename.o checksum32.o closechannel.o error.o fdchecksum32.o getoptv.o hexdecode.o hexdump.o hexencode.o hexstring.o hexview.o openchannel.o putoptv.o readpacket.o sendpacket.o strfbits.o synonym.o todigit.o typename.o uintspec.o version.o
plcsnif: plcsnif.o Confirm.o Devices.o Display.o EthernetHeader.o Failure.o MMECode.o Monitor.o QualcommHeader.o ReadMME.o Request.o SendMME.o Sniffer.o UnwantedMessage.o channel.o chipset.o closechannel.o error.o getoptv.o hexdecode.o hexdump.o hexencode.o hexout.o hexstring.o openchannel.o putoptv.o readpacket.o sendpacket.o synonym.o todigit.o typename.o uintspec.o version.o
psgen: psgen.o error.o fdchecksum32.o getoptv.o lookup.o pibfile1.o piblock.o pibscalers.o psgen_util.o putoptv.o version.o

# ====================================================================
# includes;
# --------------------------------------------------------------------

include ../tools/tools.mak
include ../ether/ether.mak
include ../ram/ram.mak
include ../pib/pib.mak
include ../nvm/nvm.mak
include ../mme/mme.mak
include ../nda/nda.mak
include ../plc/plc.mak
include ../key/key.mak



