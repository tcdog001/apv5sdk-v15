

#define NO_FUNCTION_DEFINED (-1)

//
// The following functions control the device.
//
// unless otherwise noted, all of these functions return 0 on success
//

//
// attach to the device. setup any internal data structures.
//
extern int DeviceAttach(int devid, int ssid);

//
// reset the device
//
extern int DeviceReset(int frequency, int ht40, unsigned char txchain, unsigned char rxchain);

//
// set the bssid asscoiated with the device
//
extern int DeviceBssIdSet(unsigned char *bssid);

//
// set the station id
//
extern int DeviceStationIdSet(unsigned char *macaddr);

//
// set the receive descriptor pointer on the device
//
extern int DeviceReceiveDescriptorPointer(unsigned int descriptor);

//
// allow/disallow unicast packets
//
extern int DeviceReceiveUnicast(int on);

//
// allow/disallow broadcast packets
//
extern int DeviceReceiveBroadcast(int on);

//
// allow/disallow any type of packets. turning off promiscuous mode does
// not affect any packet types that are explicitly allowed
//
extern int DeviceReceivePromiscuous(int on);

//
// turn on the receiver
//
extern int DeviceReceiveEnable();

//
// turn off the receiver
//
extern int DeviceReceiveDisable();

//
// put the receiver in deaf mode
//
extern int DeviceReceiveDeafMode(int on);

//
// set the transmit descriptor pointer on the device
//
extern int DeviceTransmitDescriptorStatusPointer(unsigned int first, unsigned int last);

//
// set the transmit descriptor pointer on the device
//
extern int DeviceTransmitDescriptorPointer(int queue, unsigned int descriptor);

//
// set the retry limit. retry limit can also be set in the descriptor for particular packets
//
extern int DeviceTransmitRetryLimit(int dcu, int retry);

//
// asscoiate a qcu with a dcu
//
extern int DeviceTransmitQueueSetup(int qcu, int dcu);

//
// use regular data trasnmsission mode. packets, interframe spacing, contention window, etc.
// use DeviceTransmitEnable() to start. use DeviceTransmitDisable() to stop.
//
extern int DeviceTransmitRegularData();	

//
// use tx99 mode. packets, fixed interframe spacing, no contention window, 
// use DeviceTransmitEnable() to start. use DeviceTransmitDisable() to stop.
//	
extern int DeviceTransmitFrameData(int ifs);

//
// use continuous data mode, one continuous data stream, no interframe spacing, no contention window, just data
// use DeviceTransmitEnable() to start. use DeviceReset() to stop.
//
extern int DeviceTransmitContinuousData();

//
// transmit the carrier frequency. 
// starts itself. use DeviceReset() to stop.
//			
extern int DeviceTransmitCarrier(int frequency, unsigned int txchain);				

//
// start the transmitter
//
extern int DeviceTransmitEnable(unsigned int qmask);

//
// stop the transmitter
//
extern int DeviceTransmitDisable(unsigned int qmask);

//
// set the transmit power. dBm
//
extern int DeviceTransmitPowerSet(int mode, double txp);


//
// set the pcdac and pdgain values
//
extern int DeviceTransmitGainSet(int mode, int pcdac);

extern int DeviceTransmitGainRead(int entry, unsigned int *rvalue, int *value, int max);

extern int DeviceTransmitGainWrite(int entry, int *value, int nvalue);

//
// read/write the eeprom
//
extern int DeviceEepromRead(unsigned int address, unsigned char *value, int count);

extern int DeviceEepromWrite(unsigned int address, unsigned char *value, int count);

//
// read/write the eeprom
//
extern int DeviceOtpRead(unsigned int address, unsigned char *value, int count);

extern int DeviceOtpWrite(unsigned int address, unsigned char *value, int count);


//
// read/write the shared memory
//
extern int DeviceMemoryRead(unsigned int address, unsigned int *value, int count);

extern int DeviceMemoryWrite(unsigned int address, unsigned int *value, int count);

extern int DeviceRegisterRead(unsigned int address, unsigned int *value);

extern int DeviceRegisterWrite(unsigned int address, unsigned int value);


//
// set/save/restore/use calibration and other configuration information
//
extern int DeviceConfigurationSave(void);

extern int DeviceConfigurationRestore(void);

//
// override the normal power control settings with these newly caluclated ones.
// used to test new calibration data, before installing the values in the main structure.
//
extern int DevicePowerControlOverride(int frequency, int *correction, int *voltage, int *temperature);

extern int DeviceCalibrationPierSet(int pier, int frequency, int chain, int correction, int voltage, int temperature);

//
// set and remember the target powers
//
extern int DeviceTargetPowerSet(void);

//
// looks up the target power for a given frequency and rate
//
extern int DeviceTargetPowerGet(int frequency, int rate, double *power);

//
// apply the target powers
//
extern int DeviceTargetPowerApply(int frequency);

extern int DeviceTemperatureGet(int);

extern int DeviceVoltageGet(void);




//
// The following functions setup the device control functions for a specific device type.
//


//
// clear all device control function pointers and set to default behavior
//
extern void DeviceFunctionReset();

extern void DeviceReceiveAttachFunction(int (*f)(int devid, int ssid));

extern void DeviceReceiveResetFunction(int (*f)(int frequency, int ht40, unsigned int txchain, unsigned int rxchain));

extern void DeviceBssIdSetFunction(int (*f)(unsigned char *bssid));

extern void DeviceStationIdSetFunction(int (*f)(unsigned char *macaddr));

extern void DeviceReceiveDescriptorPointerFunction(int (*f)(unsigned int descriptor));

extern void DeviceReceiveUnicastFunction(int (*f)(int on));

extern void DeviceReceiveBroadcastFunction(int (*f)(int on));

extern void DeviceReceivePromiscuousFunction(int (*f)(int on));

extern void DeviceReceiveEnableFunction(int (*f)());

extern void DeviceReceiveDisableFunction(int (*f)());

extern void DeviceReceiveDeafModeFunction(int (*f)(int on));

extern void DeviceTransmitDescriptorStatusPointerFunction(int (*f)(unsigned int first, unsigned int last));

extern void DeviceTransmitDescriptorPointerFunction(int (*f)(int qcu, unsigned int descriptor));

extern void DeviceTransmitRetryLimitFunction(int (*f)(int dcu, int retry));

extern void DeviceTransmitQueueSetupFunction(int (*f)(int qcu, int dcu));

extern void DeviceTransmitRegularDataFunction(int (*f)());

extern void DeviceTransmitFrameDataFunction(int (*f)(int ifs));

extern void DeviceTransmitContinuousDataFunction(int (*f)());

extern void DeviceTransmitCarrierFunction(int (*f)(int frequency, unsigned int txchain));

extern void DeviceTransmitEnableFunction(int (*f)(unsigned int qmask));

extern void DeviceTransmitDisableFunction(int (*f)(unsigned int qmask));

extern void DeviceTransmitPowerSetFunction(int (*f)(int mode, double txp));

extern void DeviceTransmitGainReadFunction(int (*f)(int entry, unsigned int *rvalue, int *value, int max));

extern void DeviceTransmitGainWriteFunction(int (*f)(int entry, int *value, int nvalue));

extern void DeviceTransmitGainSetFunction(int (*f)(int mode, int pcdac));

extern void DeviceEepromReadFunction(int (*f)(unsigned int address, unsigned char *value, int count));

extern void DeviceEepromWriteFunction(int (*f)(unsigned int address, unsigned char *value, int count));

extern void DeviceConfigurationRestoreFunction(int (*f)(void));

extern void DeviceConfigurationSaveFunction(int (*f)(void));

extern void DeviceCalibrationPierSetFunction(int (*f)(int pier, int frequency, int chain, int correction, int voltage, int temperature));

extern void DevicePowerControlOverrideFunction(int (*f)(int frequency, int *correction, int *voltage, int *temperature));

extern void DeviceTargetPowerSetFunction(int (*f)(void));

extern void DeviceTargetPowerGetFunction(int (*f)(int frequency, int rate, double *power));

extern void DeviceTargetPowerApplyFunction(int (*f)(int frequency));

extern void DeviceOtpReadFunction(int (*f)(unsigned int address, unsigned char *value, int count));

extern void DeviceOtpWriteFunction(int (*f)(unsigned int address, unsigned char *value, int count));

extern void DeviceMemoryReadFunction(int (*f)(unsigned int address, unsigned int *value, int count));

extern void DeviceMemoryWriteFunction(int (*f)(unsigned int address, unsigned int *value, int count));

extern void DeviceRegisterReadFunction(int (*f)(unsigned int address, unsigned int *value));

extern void DeviceRegisterWriteFunction(int (*f)(unsigned int address, unsigned int value));

extern void DeviceTemperatureGetFunction(int (*f)(int));

extern void DeviceVoltageGetFunction(int (*f)(void));
