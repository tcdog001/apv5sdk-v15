#!/usr/bin/make -f
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
VPATH=../pib:../nvm:../ram:../int6k:../tools:../ether:../mme:../crypt:../nodes
TRASH=*.o *.obj *.exe t t.* *~ *.[0-9][0-9][0-9]
FILES=Makefile *.mak *.sh *.txt *.c *.h 
TOOLS=int6k int6kboot int6kecho int6keth int6kf int6khost int64host int6kid int6klist int6klog int6km int6kmod int6kprobe int6kstat int6krate int6krule int6ktone int6kwait CMEncrypt sada int6ktest
#TOOLS=int6k int6kboot
#PAGES=int6k.7 int6kboot.7 int6kecho.7 int6keth.7 int6kf.7 int6kid.7 int6khost.7 int64host.7 int6klist.7 int6klog.7 int6km.7 int6kmod.7 int6kprobe.7 int6kstat.7 int6ktest.7 int6ktone.7 int6krate.7 int6krule.7 int6kwait.7 CMEncrypt.7 sada.7   
#PAGES=int6k.7 int6kboot.7

# ====================================================================
# targets;
# --------------------------------------------------------------------

.PHONY: all compile compact install scripts manuals uninstall library clean fresh

# ====================================================================
# targets;
# --------------------------------------------------------------------

all compile: ${TOOLS}
compact: compile
	@echo "albert said strip eq "$(STRIP)
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
	chmod 0644 *.c *.h *.7 *.txt
	chmod 0755 Makefile *.mak *.sh
test:
	@ ./test.sh
size:
	filesize ${TOOLS}
fresh: clean compile
%.o: %.c
	${CC} ${CFLAGS} -c ${<}
%: %.o
	${CC} ${LFLAGS} -o ${@} ${^} 

# ====================================================================
# programs;     
# --------------------------------------------------------------------

CMEncrypt: CMEncrypt.o getoptv.o putoptv.o version.o uintspec.o hexencode.o hexdump.o error.o channel.o openchannel.o closechannel.o readpacket.o sendpacket.o SHA256Reset.o SHA256Write.o SHA256Block.o SHA256Fetch.o todigit.o synonym.o channel.o Display.o hexdecode.o devices.o EthernetHeader.o StandardHeader.o UnwantedPacket.o

int6k: int6k.o getoptv.o putoptv.o version.o error.o devices.o filepart.o uintspec.o todigit.o checkfilename.o hexencode.o hexdecode.o hexdump.o checksum_32.o fdchecksum_32.o keys.o Attributes.o Confirm.o Display.o FlashDevice.o FlashNVM.o Identity.o Failure.o NetworkInfo.o NVRAMInfo.o nvrampeek.o nvram.o SDRAMInfo.o sdramfile.o sdrampeek.o ReadNVM.o ReadMME.o ReadMFG.o ReadPIB.o Request.o ResetDevice1.o FactoryDefaults.o nvmfile.o nvmpeek.o pibfile.o PushButton.o SendMME.o SetNMK.o WriteCFG.o StartFirmware.o VersionInfo.o WriteNVM.o WritePIB.o channel.o openchannel.o closechannel.o readpacket.o sendpacket.o EthernetHeader.o StandardHeader.o IntellonHeader.o UnwantedPacket.o MMECode.o typename.o chipset.o WaitForStart.o WaitForReset.o strfbits.o alert.o pibpeek.o WatchdogReport.o hexstring.o synonym.o pibcodes.o 

int6kboot: int6kboot.o getoptv.o putoptv.o version.o alert.o error.o filepart.o uintspec.o todigit.o checkfilename.o hexdecode.o hexdump.o checksum_32.o fdchecksum_32.o nvram.o keys.o BootDevice.o UpgradeDevice.o FlashDevice.o FlashNVM.o Confirm.o Display.o Failure.o WriteCFG.o nvmfile.o nvmpeek.o pibfile.o sdramfile.o ReadMME.o Request.o SendMME.o WriteMEM.o WriteNVM.o WritePIB.o StartFirmware.o VersionInfo.o channel.o openchannel.o closechannel.o readpacket.o sendpacket.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o MMECode.o chipset.o WaitForStart.o WaitForReset.o strfbits.o



int6kecho: int6kecho.o EthernetHeader.o IntellonHeader.o Failure.o Display.o ReadMME.o SendMME.o UnwantedPacket.o Loopback.o devices.o alert.o assist.o error.o expect.o getoptv.o putoptv.o version.o uintspec.o hexdump.o hexencode.o hexdecode.o hexstring.o todigit.o checkfilename.o checksum32.o fdchecksum32.o strfbits.o filepart.o typename.o lookup.o synonym.o openchannel.o closechannel.o readpacket.o sendpacket.o channel.o MMECode.o MMEName.o hexin.o Traffic.o

int6keth: int6keth.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o devices.o synonym.o alert.o assist.o error.o expect.o getoptv.o lookup.o putoptv.o version.o hexdump.o hexencode.o hexdecode.o todigit.o openchannel.o closechannel.o readpacket.o sendpacket.o channel.o MMECode.o hexstring.o

int6kf: int6kf.o getoptv.o putoptv.o version.o alert.o error.o filepart.o uintspec.o todigit.o checkfilename.o hexdecode.o hexdump.o checksum_32.o fdchecksum_32.o nvram.o keys.o StartDevice.o UpgradeDevice.o FlashDevice.o FlashNVM.o Confirm.o Display.o Failure.o WriteCFG.o nvmfile.o nvmpeek.o pibfile.o sdramfile.o ReadMME.o Request.o SendMME.o WriteMEM.o WriteNVM.o WritePIB.o StartFirmware.o VersionInfo.o channel.o openchannel.o closechannel.o readpacket.o sendpacket.o MMECode.o chipset.o WaitForStart.o WaitForReset.o strfbits.o pibcodes.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

int6klog: int6klog.o getoptv.o putoptv.o version.o error.o alert.o devices.o Confirm.o Display.o Request.o Failure.o SendMME.o ReadMME.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o uintspec.o hexdump.o hexencode.o hexdecode.o hexstring.o todigit.o checkfilename.o checksum_32.o strfbits.o filepart.o typename.o synonym.o openchannel.o closechannel.o readpacket.o sendpacket.o channel.o MMECode.o b64dump.o 

int6kmod: int6kmod.o devices.o Confirm.o Display.o Request.o Failure.o ReadMME.o SendMME.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o synonym.o alert.o getoptv.o putoptv.o version.o error.o uintspec.o hexdump.o hexencode.o hexdecode.o hexstring.o todigit.o checkfilename.o checksum_32.o fdchecksum_32.o strfbits.o filepart.o typename.o markup.o openchannel.o closechannel.o readpacket.o sendpacket.o channel.o MMECode.o basespec.o filepart.o


int6krate: int6krate.o getoptv.o putoptv.o version.o alert.o uintspec.o hexdump.o hexencode.o hexdecode.o todigit.o checkfilename.o checksum_32.o fdchecksum_32.o error.o PHYRates.o ResetDevice1.o VersionInfo.o Request.o Confirm.o Failure.o SendMME.o ReadMME.o keys.o openchannel.o closechannel.o readpacket.o sendpacket.o MMECode.o Display.o chipset.o synonym.o channel.o devices.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o NetworkTraffic.o Traffic.o ProtocolHeader.o

int6khost: int6khost.o getoptv.o putoptv.o version.o uintspec.o checkfilename.o hexdecode.o todigit.o hexdump.o hexview.o checksum_32.o fdchecksum_32.o error.o filepart.o Confirm.o Failure.o FlashDevice.o FlashNVM.o HostActionResponse.o EmulateHost.o Request.o ReadMME.o SendMME.o StartFirmware.o WaitForReset.o WaitForStart.o WriteCFG.o WriteMEM.o WriteNVM.o WritePIB.o ReadNVM.o ReadPIB.o ResetDevice1.o FactoryDefaults.o MMECode.o sdramfile.o nvmfile.o nvmpeek.o pibfile.o keys.o openchannel.o closechannel.o readpacket.o sendpacket.o strfbits.o alert.o channel.o Display.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

int64host: int64host.o getoptv.o putoptv.o version.o uintspec.o checkfilename.o hexdecode.o todigit.o hexdump.o checksum_32.o fdchecksum_32.o error.o filepart.o Confirm.o Failure.o FlashDevice.o FlashNVM.o HostActionResponse.o EmulateHost64.o Request.o ReadMME.o SendMME.o StartFirmware.o WaitForReset.o WaitForStart.o WriteCFG.o WriteMEM.o WriteNVM.o WritePIB.o ReadNVM.o ReadPIB.o ResetDevice1.o FactoryDefaults.o MMECode.o sdramfile.o nvmfile.o nvmpeek.o pibfile.o keys.o openchannel.o closechannel.o readpacket.o sendpacket.o strfbits.o alert.o InitDevice.o BootDevice.o channel.o Display.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

int6kid: int6kid.o getoptv.o putoptv.o version.o devices.o uintspec.o hexencode.o hexdecode.o todigit.o hexdump.o hexout.o error.o channel.o openchannel.o closechannel.o readpacket.o sendpacket.o MMECode.o synonym.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o channel.o Display.o

int6klist: int6klist.o Confirm.o Display.o Failure.o Request.o ReadMME.o SendMME.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o devices.o alert.o error.o getoptv.o putoptv.o version.o uintspec.o hexdump.o hexencode.o hexdecode.o hexout.o todigit.o synonym.o openchannel.o closechannel.o readpacket.o sendpacket.o channel.o MMECode.o UnwantedPacket.o NetworkBridges.o NetworkDevices.o

int6km: int6km.o getoptv.o putoptv.o version.o uintspec.o basespec.o hexencode.o hexdecode.o todigit.o hexdump.o hexview.o regview.o error.o openchannel.o closechannel.o readpacket.o sendpacket.o MMECode.o channel.o synonym.o channel.o Display.o devices.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

int6kprobe: int6kprobe.o Confirm.o Display.o Failure.o Request.o ReadMME.o SendMME.o DiagnosticNetworkProbe.o alert.o error.o getoptv.o putoptv.o version.o uintspec.o hexdump.o hexencode.o hexdecode.o hexstring.o todigit.o synonym.o openchannel.o closechannel.o readpacket.o sendpacket.o channel.o MMECode.o devices.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

int6krule: int6krule.o getoptv.o putoptv.o version.o hexdump.o hexencode.o hexdecode.o todigit.o error.o Confirm.o Failure.o ReadMME.o Request.o SendMME.o openchannel.o closechannel.o readpacket.o sendpacket.o MMECode.o dataspec.o ipv4spec.o ipv6spec.o bytespec.o  endian.o basespec.o synonym.o channel.o Display.o lookup.o assist.o expect.o devices.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o rules.o ParseRule.o

int6kstat: int6kstat.o Confirm.o LinkStatistics.o Request.o Display.o Failure.o ReadMME.o SendMME.o alert.o error.o getoptv.o putoptv.o version.o uintspec.o hexdump.o hexencode.o hexdecode.o todigit.o openchannel.o closechannel.o readpacket.o sendpacket.o MMECode.o NetworkTopology.o NetworkInformation.o hexstring.o synonym.o channel.o devices.o chipset.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

int6ktest: int6ktest.o getoptv.o putoptv.o version.o filepart.o uintspec.o checkfilename.o hexdecode.o todigit.o hexdump.o checksum_32.o fdchecksum_32.o error.o alert.o strfbits.o channel.o openchannel.o closechannel.o readpacket.o sendpacket.o nvmfile.o nvmpeek.o MMECode.o Confirm.o Display.o Failure.o Request.o SendMME.o WriteMEM.o WaitForReset.o WaitForStart.o chipset.o WaitForStop.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o FirmwarePacket.o

int6ktone: int6ktone.o Confirm.o Request.o Failure.o ReadMME.o SendMME.o alert.o error.o getoptv.o putoptv.o version.o uintspec.o hexdump.o hexencode.o hexdecode.o hexstring.o todigit.o synonym.o openchannel.o closechannel.o readpacket.o sendpacket.o MMECode.o channel.o Display.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o SignalToNoise.o ToneMaps.o

int6kwait: int6kwait.o getoptv.o putoptv.o version.o error.o uintspec.o hexdump.o hexencode.o hexdecode.o todigit.o checkfilename.o checksum_32.o fdchecksum_32.o Request.o Confirm.o Failure.o SendMME.o ReadMME.o MMECode.o openchannel.o closechannel.o readpacket.o sendpacket.o synonym.o channel.o Display.o devices.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

sada: sada.o getoptv.o putoptv.o version.o error.o Confirm.o Request.o Failure.o ReadMME.o SendMME.o SlaveMembership.o uintspec.o hexdump.o hexencode.o hexdecode.o todigit.o alert.o channel.o openchannel.o closechannel.o readpacket.o sendpacket.o MMECode.o synonym.o Display.o devices.o EthernetHeader.o IntellonHeader.o UnwantedPacket.o

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

