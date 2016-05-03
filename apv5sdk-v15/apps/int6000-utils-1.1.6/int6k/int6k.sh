#!/bin/sh
# file: int6k/int6k.sh

# ====================================================================
# programs;
# --------------------------------------------------------------------

gcc -Wall -o CMEncrypt CMEncrypt.c
gcc -Wall -o int6k int6k.c
gcc -Wall -o int6kboot int6kboot.c
gcc -Wall -o int6kbootp int6kbootp.c
gcc -Wall -o int6kecho int6kecho.c
gcc -Wall -o int6keth int6keth.c
gcc -Wall -o int6kf int6kf.c
gcc -Wall -o int6kfp int6kfp.c
gcc -Wall -o int6klog int6klog.c
gcc -Wall -o int6km int6km.c
gcc -Wall -o int6kmod int6kmod.c
gcc -Wall -o int6khost int6khost.c
gcc -Wall -o int6klist int6klist.c
gcc -Wall -o int6kp int6kp.c
gcc -Wall -o int6krate int6krate.c
gcc -Wall -o int6krule int6krule.c
gcc -Wall -o int6kstat int6kstat.c
gcc -Wall -o int6ktest int6ktest.c
gcc -Wall -o int6ktone int6ktone.c
gcc -Wall -o int6kwait int6kwait.c
gcc -Wall -o sada sada.c

# ====================================================================
# functions; 
# --------------------------------------------------------------------

gcc -Wall -c Attributes.c
gcc -Wall -c chipset.c
gcc -Wall -c Confirm.c
gcc -Wall -c devices.c
gcc -Wall -c DiagnosticNetworkProbe.c
gcc -Wall -c Display.c
gcc -Wall -c EmulateHost.c
gcc -Wall -c EmulateHost64.c
gcc -Wall -c EthernetHeader.c
gcc -Wall -c FactoryDefaults.c
gcc -Wall -c Failure.c
gcc -Wall -c FirmwarePacket.c
gcc -Wall -c FlashDevice.c
gcc -Wall -c FlashMOD.c
gcc -Wall -c FlashNVM.c
gcc -Wall -c FlashPTS.c
gcc -Wall -c FragmentHeader.c
gcc -Wall -c StandardHeader.c
gcc -Wall -c HostActionIndicate.c
gcc -Wall -c HostActionResponse.c
gcc -Wall -c Identity.c
gcc -Wall -c IntellonHeader.c
gcc -Wall -c InitDevice.c
gcc -Wall -c LinkStatistics.c
gcc -Wall -c NVRAMInfo.c
gcc -Wall -c NetworkBridges.c
gcc -Wall -c NetworkDevices.c
gcc -Wall -c NetworkInfo.c
gcc -Wall -c NetworkInformation.c
gcc -Wall -c NetworkTopology.c
gcc -Wall -c NetworkTraffic.c
gcc -Wall -c ParseRule.c
gcc -Wall -c PHYRates.c
gcc -Wall -c PushButton.c
gcc -Wall -c ReadMFG.c
gcc -Wall -c ReadMME.c
gcc -Wall -c ReadNVM.c
gcc -Wall -c ReadPIB.c
gcc -Wall -c Request.c
gcc -Wall -c ResetAndWait.c
gcc -Wall -c ResetDevice1.c
gcc -Wall -c rules.c
gcc -Wall -c SDRAMInfo.c
gcc -Wall -c SendMME.c
gcc -Wall -c SetNMK.c
gcc -Wall -c SignalToNoise.c
gcc -Wall -c SlaveMembership.c
gcc -Wall -c StartDevice.c
gcc -Wall -c StartFirmware.c
gcc -Wall -c ToneMaps.c
gcc -Wall -c UnwantedPacket.c
gcc -Wall -c UpgradeDevice.c
gcc -Wall -c VersionInfo.c
gcc -Wall -c WaitForAssoc.c
gcc -Wall -c WaitForReset.c
gcc -Wall -c WaitForStart.c
gcc -Wall -c WaitForStop.c
gcc -Wall -c WatchdogReport.c
gcc -Wall -c WriteCFG.c
gcc -Wall -c WriteMEM.c
gcc -Wall -c WriteMOD.c
gcc -Wall -c WriteNVM.c
gcc -Wall -c WritePIB.c
