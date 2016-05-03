#!/bin/sh
# file: ihpapi/ihpapi.sh

# ====================================================================
#
# --------------------------------------------------------------------

gcc -Wall -c DecodeEthernetHeader.c
gcc -Wall -c DecodeEthernetPHYSettings.c
gcc -Wall -c DecodeFlashNVM.c
gcc -Wall -c DecodeGetNVM.c
gcc -Wall -c DecodeGetRxToneMap.c
gcc -Wall -c DecodeGetToneMap.c
gcc -Wall -c DecodeGetVersion.c
gcc -Wall -c DecodeHostAction.c
gcc -Wall -c DecodeLinkStats.c
gcc -Wall -c DecodeMfgString.c
gcc -Wall -c DecodeNetworkInfo.c
gcc -Wall -c DecodeReadModule.c
gcc -Wall -c DecodeResetDevice.c
gcc -Wall -c DecodeResetToFactoryDefaults.c
gcc -Wall -c DecodeSetKey.c
gcc -Wall -c DecodeSetSDRAMCfg.c
gcc -Wall -c DecodeStartMAC.c
gcc -Wall -c DecodeWriteMemory.c
gcc -Wall -c DecodeWriteModule.c
gcc -Wall -c EncodeEthernetHeader.c
gcc -Wall -c EncodeIntellonHeader.c
gcc -Wall -c EncodeSetSDRAMCfg.c
gcc -Wall -c FindFWHeader.c
gcc -Wall -c GetConnectionInfo.c
gcc -Wall -c GetManufacturerInfo.c
gcc -Wall -c GetNetworkInfo.c
gcc -Wall -c GetRxToneMap.c
gcc -Wall -c GetToneMap.c
gcc -Wall -c GetVersionInfo.c
gcc -Wall -c InitializeDevice.c
gcc -Wall -c ManageEnetPHY.c
gcc -Wall -c ResetDevice.c
gcc -Wall -c ResetToFactoryDefaults.c
gcc -Wall -c RxFrame.c
gcc -Wall -c SequenceInProgress.c
gcc -Wall -c SequenceReadModule.c
gcc -Wall -c SequenceTerminate.c
gcc -Wall -c SequenceWriteMemory.c
gcc -Wall -c SequenceWriteModule.c
gcc -Wall -c SetKey.c
gcc -Wall -c SetSDRAMConfig.c
gcc -Wall -c TxFrame.c
gcc -Wall -c UpdateDevice.c
gcc -Wall -c ihpapi.c

