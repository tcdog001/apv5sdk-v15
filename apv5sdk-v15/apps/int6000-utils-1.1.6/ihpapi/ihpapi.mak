# ====================================================================
# files;
# --------------------------------------------------------------------

DecodeEthernetHeader.o: DecodeEthernetHeader.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeEthernetPHYSettings.o: DecodeEthernetPHYSettings.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeFlashNVM.o: DecodeFlashNVM.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeGetNVM.o: DecodeGetNVM.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeGetToneMap.o: DecodeGetToneMap.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeGetRxToneMap.o: DecodeGetRxToneMap.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeGetVersion.o: DecodeGetVersion.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeHostAction.o: DecodeHostAction.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeLinkStats.o: DecodeLinkStats.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeMfgString.o: DecodeMfgString.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeNetworkInfo.o: DecodeNetworkInfo.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeReadModule.o: DecodeReadModule.c ihp.h memory.h ihpapi.h types.h flags.h buffer.h
DecodeResetDevice.o: DecodeResetDevice.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeResetToFactoryDefaults.o: DecodeResetToFactoryDefaults.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeSetKey.o: DecodeSetKey.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeSetSDRAMCfg.o: DecodeSetSDRAMCfg.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeStartMAC.o: DecodeStartMAC.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeWriteMemory.o: DecodeWriteMemory.c ihp.h ihpapi.h types.h flags.h buffer.h
DecodeWriteModule.o: DecodeWriteModule.c ihp.h ihpapi.h types.h flags.h buffer.h
EncodeEthernetHeader.o: EncodeEthernetHeader.c ihp.h types.h ihpapi.h flags.h buffer.h
EncodeIntellonHeader.o: EncodeIntellonHeader.c ihp.h types.h ihpapi.h flags.h buffer.h
EncodeSetSDRAMCfg.o: EncodeSetSDRAMCfg.c ihpapi.h ihp.h memory.h types.h flags.h buffer.h
FindFWHeader.o: FindFWHeader.c ihp.h ihpapi.h memory.h types.h flags.h buffer.h
GetConnectionInfo.o: GetConnectionInfo.c ihpapi.h ihp.h types.h flags.h buffer.h
GetManufacturerInfo.o: GetManufacturerInfo.c ihpapi.h ihp.h types.h flags.h buffer.h
GetNetworkInfo.o: GetNetworkInfo.c ihpapi.h ihp.h types.h flags.h buffer.h
GetToneMap.o: GetToneMap.c ihpapi.h ihp.h types.h flags.h buffer.h
GetRxToneMap.o: GetRxToneMap.c ihpapi.h ihp.h types.h flags.h buffer.h
GetVersionInfo.o: GetVersionInfo.c ihpapi.h ihp.h types.h flags.h buffer.h
InitializeDevice.o: InitializeDevice.c ihpapi.h ihp.h memory.h flags.h types.h buffer.h
ManageEnetPHY.o: ManageEnetPHY.c ihpapi.h ihp.h types.h flags.h buffer.h
ResetDevice.o: ResetDevice.c ihpapi.h ihp.h types.h flags.h buffer.h
ResetToFactoryDefaults.o: ResetToFactoryDefaults.c ihpapi.h ihp.h types.h flags.h buffer.h
RxFrame.o: RxFrame.c memory.h ihpapi.h ihp.h types.h flags.h buffer.h
SequenceInProgress.o: SequenceInProgress.c ihpapi.h ihp.h flags.h types.h buffer.h
SequenceReadModule.o: SequenceReadModule.c ihpapi.h ihp.h memory.h flags.h types.h buffer.h
SequenceTerminate.o: SequenceTerminate.c ihpapi.h ihp.h types.h flags.h buffer.h
SequenceWriteMemory.o: SequenceWriteMemory.c ihpapi.h ihp.h memory.h flags.h types.h buffer.h
SequenceWriteModule.o: SequenceWriteModule.c ihpapi.h ihp.h memory.h flags.h types.h buffer.h
SetKey.o: SetKey.c ihpapi.h ihp.h types.h flags.h buffer.h
SetSDRAMConfig.o: SetSDRAMConfig.c ihpapi.h ihp.h memory.h types.h flags.h buffer.h
TxFrame.o: TxFrame.c ihpapi.h ihp.h types.h flags.h buffer.h
UpdateDevice.o: UpdateDevice.c ihpapi.h ihp.h flags.h types.h buffer.h

# ====================================================================
# files;
# --------------------------------------------------------------------

ihp.h: ihpapi.h types.h flags.h buffer.h
	touch ../ihpapi/${@}
ihpapi.h: types.h
	touch ../ihpapi/${@}

