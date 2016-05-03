# ====================================================================
# source files;
# --------------------------------------------------------------------

Attributes.o: Attributes.c int6k.h error.h memory.h format.h
chipset.o: chipset.c chipset.h
CMEncrypt.o: CMEncrypt.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h SHA256.h 
Confirm.o: Confirm.c int6k.h memory.h flags.h
DiagnosticNetworkProbe.o: DiagnosticNetworkProbe.c int6k.h memory.h number.h error.h
devices.o: devices.c int6k.h types.h
Display.o: Display.c int6k.h memory.h 
FlashNVM.o: FlashNVM.c int6k.h error.h memory.h
FlashPTS.o: FlashPTS.c int6k.h error.h memory.h
FactoryDefaults.o: FactoryDefaults.c int6k.h memory.h error.h
FlashDevice.o: FlashDevice.c int6k.h 
EmulateHost.o: EmulateHost.c int6k.h error.h sdram.h nvm.h pib.h
EmulateHost64.o: EmulateHost64.c int6k.h error.h sdram.h nvm.h pib.h
Failure.o: Failure.c int6k.h memory.h flags.h mme.h
Identity.o: Identity.c int6k.h error.h memory.h pib.h
FirmwarePacket.o: FirmwarePacket.c int6k.h channel.h error.h memory.h flags.h
UnwantedPacket.o: UnwantedPacket.c intellon.h homeplug.h message.h endian.h types.h 
int6k.o: int6k.c getoptv.h putoptv.h homeplug.h intellon.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h sdram.h keys.h pib.h nvm.h keys.h
int6kecho.o: int6kecho.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h intellon.h channel.h mme.h 
int6keth.o: int6keth.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h channel.h 
int6kf.o: int6kf.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h ether.h nvram.h sdram.h nvm.h pib.h keys.h
int6klog.o: int6klog.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h channel.h
int6kmod.o: int6kmod.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h channel.h int6k.h 
int6kfp.o: int6kfp.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h ether.h nvram.h sdram.h nvm.h pib.h keys.h
int6kid.o: int6kid.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h ether.h nvram.h sdram.h nvm.h pib.h mme.h 
int6klist.o: int6klist.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h 
int6km.o: int6km.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h ether.h nvram.h sdram.h nvm.h pib.h 
int6kprobe.o: int6kprobe.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h 
int6kp.o: int6kp.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h sdram.h keys.h pib.h nvm.h keys.h
int6khost.o: int6khost.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h ether.h nvram.h sdram.h nvm.h pib.h 
int64host.o: int64host.c getoptv.h putoptv.h memory.h types.h flags.h files.h error.h int6k.h ether.h nvram.h sdram.h nvm.h pib.h 
int6krate.o: int6krate.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h 
int6kwait.o: int6kwait.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h 
int6kstat.o: int6kstat.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h 
int6ktest.o: int6ktest.c getoptv.h putoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h ether.h nvram.h sdram.h nvm.h pib.h 
int6ktone.o: int6ktone.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h 
int6krule.o: int6krule.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h 
sada.o: sada.c getoptv.h memory.h number.h symbol.h types.h flags.h files.h error.h int6k.h HPAVKey.h sdram.h pib.h nvm.h nodes.h 
LinkStats.o: LinkStats.c memory.h error.h int6k.h
LinkStatistics.o: LinkStatistics.c memory.h error.h int6k.h
MfgString.o: MfgString.c int6k.h error.h memory.h
NVRAMInfo.o: NVRAMInfo.c int6k.h error.h memory.h symbol.h nvram.h
NetworkInfo.o: NetworkInfo.c int6k.h error.h memory.h
NetworkInformation.o: NetworkInformation.c int6k.h error.h memory.h
NetworkTopology.o: NetworkTopology.c int6k.h channel.h memory.h error.h 
NetworkTraffic.o: NetworkTraffic.c message.h intellon.h homeplug.h problem.h channel.h memory.h error.h flags.h types.h endian.h
PHYRates.o: PHYRates.c int6k.h error.h memory.h
PushButton.o: PushButton.c int6k.h error.h memory.h
ReadNVM.o: ReadNVM.c int6k.h memory.h error.h nvm.h
ReadMFG.o: ReadMFG.c int6k.h channel.h error.h memory.h flags.h
ReadMME.o: ReadMME.c int6k.h channel.h error.h memory.h flags.h
ReadPIB.o: ReadPIB.c int6k.h error.h flags.h pib.h
Request.o: Request.c int6k.h memory.h flags.h
ResetDevice1.o: ResetDevice1.c int6k.h error.h memory.h
SDRAMInfo.o: SDRAMInfo.c int6k.h error.h memory.h sdram.h nvm.h
SendMME.o: SendMME.c int6k.h channel.h
SetNMK.o: SetNMK.c int6k.h error.h flags.h memory.h HPAVKey.h
StartDevice.c.o: StartDevice.c.c int6k.h error.h nvm.h pib.h
StartFirmware.o: StartFirmware.c int6k.h error.h memory.h
ToneMaps.o: ToneMaps.c int6k.h error.h 
Topology.o: Topology.c int6k.h channel.h memory.h error.h 
UpgradeDevice.o: UpgradeDevice.c int6k.h error.h
VersionInfo.o: VersionInfo.c int6k.h error.h memory.h
WatchdogReport: WatchdogReport.c int6k.h error.h memory.h
WaitForAssoc.o: WaitForAssoc.c int6k.h error.h memory.h
WaitForReset.o: WaitForReset.c int6k.h error.h memory.h
WaitForStart.o: WaitForStart.c int6k.h error.h memory.h
WaitForStop.o: WaitForStop.c int6k.h error.h memory.h
WriteCFG.o: WriteCFG.c int6k.h error.h memory.h sdram.h
WriteNVM.o: WriteNVM.c int6k.h error.h memory.h files.h
WriteMEM.o: WriteMEM.c int6k.h memory.h error.h
WritePIB.o: WritePIB.c int6k.h error.h memory.h files.h
WriteMOD.o: WriteMOD.c int6k.h channel.h ether.h 
FlashMOD.o: FlashMOD.c int6k.h channel.h ether.h 
rules.o: rules.c rules.h
ParseRule.o: ParseRule.c rules.h
EthernetHeader.o: EthernetHeader.c homeplug.h message.h
FragmentHeader.o: FragmentHeader.c message.h homeplug.h memory.h types.h endian.h
StandardHeader.o: StandardHeader.c message.h homeplug.h memory.h types.h endian.h
IntellonHeader.o: IntellonHeader.c intellon.h message.h memory.h types.h endian.h
ProtocolHeader.o: ProtocolHeader.c homeplug.h message.h


# ====================================================================
# header files;
# --------------------------------------------------------------------

chipset.h homeplug.h intellon.h message.h:
	touch ../int6k/${@}
int6k.h: homeplug.h intellon.h channel.h chipset.h message.h types.h HPAVKey.h nvm.h
	touch ../int6k/${@}

