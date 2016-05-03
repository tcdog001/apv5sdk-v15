
//
// This file contains the functions that control the radio chip set and make it do the correct thing.
//
// These are the only functions that should be used by higher level software. These functions, in turn, call
// the appropriate device dependent functions based on the currently selected chip set.
//


#include "Device.h"


//
// These are the function pointers to the device dependent control functions
//
static int (*_DeviceAttach)(int devid, int ssid);
static int (*_DeviceReset)(int frequency, int ht40, unsigned char txchain, unsigned char rxchain);
static int (*_DeviceBssIdSet)(unsigned char *bssid);
static int (*_DeviceStationIdSet)(unsigned char *macaddr);

static int (*_DeviceReceiveDescriptorPointer)(unsigned int descriptor);
static int (*_DeviceReceiveUnicast)(int on);
static int (*_DeviceReceiveBroadcast)(int on);
static int (*_DeviceReceivePromiscuous)(int on);
static int (*_DeviceReceiveEnable)();
static int (*_DeviceReceiveDisable)();
static int (*_DeviceReceiveDeafMode)(int deaf);

static int (*_DeviceTransmitDescriptorStatusPointer)(unsigned int first, unsigned int last);
static int (*_DeviceTransmitDescriptorPointer)(int qcu, unsigned int descriptor);
static int (*_DeviceTransmitRetryLimit)(int dcu, int retry);
static int (*_DeviceTransmitQueueSetup)(int qcu, int dcu);
static int (*_DeviceTransmitRegularData)();		// normal
static int (*_DeviceTransmitFrameData)(int ifs);	// tx99
static int (*_DeviceTransmitContinuousData)();		// tx100
static int (*_DeviceTransmitCarrier)(int frequency, unsigned int txchain);				// carrier only
static int (*_DeviceTransmitEnable)(unsigned int qmask);
static int (*_DeviceTransmitDisable)(unsigned int qmask);

static int (*_DeviceTransmitPowerSet)(int mode, double txp);
static int (*_DeviceTransmitGainSet)(int mode, int pcdac);
static int (*_DeviceTransmitGainRead)(int entry, unsigned int *rvalue, int *value, int max);
static int (*_DeviceTransmitGainWrite)(int entry, int *value, int nvalue);

static int (*_DeviceEepromRead)(unsigned int address, unsigned char *value, int count);
static int (*_DeviceEepromWrite)(unsigned int address, unsigned char *value, int count);

static int (*_DeviceOtpRead)(unsigned int address, unsigned char *value, int count);
static int (*_DeviceOtpWrite)(unsigned int address, unsigned char *value, int count);

static int (*_DeviceMemoryRead)(unsigned int address, unsigned int *value, int count);
static int (*_DeviceMemoryWrite)(unsigned int address, unsigned int *value, int count);

static int (*_DeviceRegisterRead)(unsigned int address, unsigned int *value);
static int (*_DeviceRegisterWrite)(unsigned int address, unsigned int value);

static int (*_DeviceConfigurationRestore)(void);
static int (*_DeviceConfigurationSave)(void);
static int (*_DeviceCalibrationPierSet)(int pier, int frequency, int chain, int correction, int voltage, int temperature);
static int (*_DevicePowerControlOverride)(int frequency, int *correction, int *voltage, int *temperature);
static int (*_DeviceTargetPowerSet)(/* arguments ???? */);
static int (*_DeviceTargetPowerGet)(int frequency, int rate, double *power);
static int (*_DeviceTargetPowerApply)(int frequency);

static int (*_DeviceTemperatureGet)(int);
static int (*_DeviceVoltageGet)(void);



extern int DeviceReset(int frequency, int ht40, unsigned char txchain, unsigned char rxchain)
{
	if(_DeviceReset!=0)
	{
		return _DeviceReset(frequency,ht40,txchain,rxchain);
	}
	return NO_FUNCTION_DEFINED;
}


extern int DeviceAttach(int devid, int ssid)
{
	if(_DeviceAttach!=0)
	{
		return _DeviceAttach(devid, ssid);
	}
	return NO_FUNCTION_DEFINED;
}


extern int DeviceBssIdSet(unsigned char *bssid)
{
	if(_DeviceBssIdSet!=0)
	{
		return _DeviceBssIdSet(bssid);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceStationIdSet(unsigned char *macaddr)
{
	if(_DeviceStationIdSet!=0)
	{
		return _DeviceStationIdSet(macaddr);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceReceiveDescriptorPointer(unsigned int descriptor)
{
	if(_DeviceReceiveDescriptorPointer!=0)
	{
		return _DeviceReceiveDescriptorPointer(descriptor);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceReceiveUnicast(int on)
{
	if(_DeviceReceiveUnicast!=0)
	{
		return _DeviceReceiveUnicast(on);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceReceiveBroadcast(int on)
{
	if(_DeviceReceiveBroadcast!=0)
	{
		return _DeviceReceiveBroadcast(on);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceReceivePromiscuous(int on)
{
	if(_DeviceReceivePromiscuous!=0)
	{
		return _DeviceReceivePromiscuous(on);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceReceiveEnable()
{
	if(_DeviceReceiveEnable!=0)
	{
		return _DeviceReceiveEnable();
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceReceiveDisable()
{
	if(_DeviceReceiveDisable!=0)
	{
		return _DeviceReceiveDisable();
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitDescriptorStatusPointer(unsigned int first, unsigned int last)
{
	if(_DeviceTransmitDescriptorStatusPointer!=0)
	{
		return _DeviceTransmitDescriptorStatusPointer(first,last);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitDescriptorPointer(int queue, unsigned int descriptor)
{
	if(_DeviceTransmitDescriptorPointer!=0)
	{
		return _DeviceTransmitDescriptorPointer(queue,descriptor);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitRetryLimit(int dcu, int retry)
{
	if(_DeviceTransmitRetryLimit!=0)
	{
		return _DeviceTransmitRetryLimit(dcu,retry);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitQueueSetup(int qcu, int dcu)
{
	if(_DeviceTransmitQueueSetup!=0)
	{
		return _DeviceTransmitQueueSetup(qcu,dcu);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceReceiveDeafMode(int deaf)
{
	if(_DeviceReceiveDeafMode!=0)
	{
		return _DeviceReceiveDeafMode(deaf);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitRegularData()			// normal
{
	if(_DeviceTransmitRegularData!=0)
	{
		return _DeviceTransmitRegularData();
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitFrameData(int ifs)	// tx99
{
	if(_DeviceTransmitFrameData!=0)
	{
		return _DeviceTransmitFrameData(ifs);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitContinuousData()		// tx100
{
	if(_DeviceTransmitContinuousData!=0)
	{
		return _DeviceTransmitContinuousData();
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitCarrier(int frequency, unsigned int txchain)					// carrier only
{
	if(_DeviceTransmitCarrier!=0)
	{
		return _DeviceTransmitCarrier(frequency,txchain);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitEnable(unsigned int qmask)
{
	if(_DeviceTransmitEnable!=0)
	{
		return _DeviceTransmitEnable(qmask);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceTransmitDisable(unsigned int qmask)
{
	if(_DeviceTransmitDisable!=0)
	{
		return _DeviceTransmitDisable(qmask);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceTransmitPowerSet(int mode, double txp)
{
	if(_DeviceTransmitPowerSet!=0)
	{
		return _DeviceTransmitPowerSet(mode, txp);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceTransmitGainRead(int entry, unsigned int *rvalue, int *value, int max)
{
	if(_DeviceTransmitGainRead!=0)
	{
		return _DeviceTransmitGainRead(entry,rvalue,value,max);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceTransmitGainWrite(int entry, int *value, int nvalue)
{
	if(_DeviceTransmitGainWrite!=0)
	{
		return _DeviceTransmitGainWrite(entry,value,nvalue);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceTransmitGainSet(int mode, int pcdac)
{
	if(_DeviceTransmitGainSet!=0)
	{
		return _DeviceTransmitGainSet(mode,pcdac);
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceEepromRead(unsigned int address, unsigned char *value, int count)
{
	if(_DeviceEepromRead!=0)
	{
		return _DeviceEepromRead(address, value, count);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceEepromWrite(unsigned int address, unsigned char *value, int count)
{
	if(_DeviceEepromWrite!=0)
	{
		return _DeviceEepromWrite(address, value, count);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceConfigurationRestore(void)
{
	if(_DeviceConfigurationRestore!=0)
	{
		return _DeviceConfigurationRestore();
	}
	return NO_FUNCTION_DEFINED;
}


int DeviceConfigurationSave(void)
{
	if(_DeviceConfigurationSave!=0)
	{
		return _DeviceConfigurationSave();
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceCalibrationPierSet(int pier, int frequency, int chain, int correction, int voltage, int temperature)
{
	if(_DeviceCalibrationPierSet!=0)
	{
		return _DeviceCalibrationPierSet(pier,frequency,chain,correction,voltage,temperature);
	}
	return NO_FUNCTION_DEFINED;
}

int DevicePowerControlOverride(int frequency, int *correction, int *voltage, int *temperature)
{
	if(_DevicePowerControlOverride!=0)
	{
		return _DevicePowerControlOverride(frequency, correction,voltage,temperature);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceTargetPowerGet(int frequency, int rate, double *power)
{
	if(_DeviceTargetPowerGet!=0)
	{
		return _DeviceTargetPowerGet(frequency, rate, power);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceTargetPowerApply(int frequency)
{
	if(_DeviceTargetPowerApply!=0)
	{
		return _DeviceTargetPowerApply(frequency);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceOtpRead(unsigned int address, unsigned char *value, int count)
{
	if(_DeviceOtpRead!=0)
	{
		return _DeviceOtpRead(address, value, count);
	}
	return NO_FUNCTION_DEFINED;
}

int DeviceOtpWrite(unsigned int address, unsigned char *value, int count)
{
	if(_DeviceOtpWrite!=0)
	{
		return _DeviceOtpWrite(address, value, count);
	}
	return NO_FUNCTION_DEFINED;
}
//
// read the shared memory
//
int DeviceMemoryRead(unsigned int address, unsigned int *value, int count)
{
	if(_DeviceMemoryRead!=0)
	{
		return _DeviceMemoryRead(address, value, count);
	}
	return NO_FUNCTION_DEFINED;
}


//
// write the shared memory
//
int DeviceMemoryWrite(unsigned int address, unsigned int *value, int count)
{
	if(_DeviceMemoryWrite!=0)
	{
		return _DeviceMemoryWrite(address, value, count);
	}
	return NO_FUNCTION_DEFINED;
}


//
// read a register
//
int DeviceRegisterRead(unsigned int address, unsigned int *value)
{
	if(_DeviceRegisterRead!=0)
	{
		return _DeviceRegisterRead(address, value);
	}
	return NO_FUNCTION_DEFINED;
}

//
// write a register
//
int DeviceRegisterWrite(unsigned int address, unsigned int value)
{
	if(_DeviceRegisterWrite!=0)
	{
		return _DeviceRegisterWrite(address, value);
	}
	return NO_FUNCTION_DEFINED;
}


//
// read temperature from chip
//
int DeviceTemperatureGet(int forceTempRead)
{
	if(_DeviceTemperatureGet!=0)
	{
		return _DeviceTemperatureGet(forceTempRead);
	}
	return NO_FUNCTION_DEFINED;
}

//
// read voltgae from chip
//
int DeviceVoltageGet(void)
{
	if(_DeviceVoltageGet!=0)
	{
		return _DeviceVoltageGet();
	}
	return NO_FUNCTION_DEFINED;
}
//
//
//
// the following functions are used to set the pointers to the correct functions for a specific chip.
//
// When implementing support for a new chip a function performing each of these operations
// must be produced. See Ar5416Device.c for an example.
//
//
//

//
// clear all device control function pointers and set to default behavior
//
void DeviceFunctionReset()
{
    _DeviceAttach=0;
    _DeviceReset=0;
    _DeviceBssIdSet=0;
    _DeviceStationIdSet=0;

    _DeviceReceiveDescriptorPointer=0;
    _DeviceReceiveUnicast=0;
    _DeviceReceiveBroadcast=0;
    _DeviceReceivePromiscuous=0;
    _DeviceReceiveEnable=0;
    _DeviceReceiveDisable=0;
    _DeviceReceiveDeafMode=0;

    _DeviceTransmitDescriptorStatusPointer=0;
    _DeviceTransmitDescriptorPointer=0;
    _DeviceTransmitRetryLimit=0;
    _DeviceTransmitQueueSetup=0;
    _DeviceTransmitRegularData=0;			// normal
    _DeviceTransmitFrameData=0;				// tx99
    _DeviceTransmitContinuousData=0;		// tx100
    _DeviceTransmitCarrier=0;				// carrier only
    _DeviceTransmitEnable=0;
    _DeviceTransmitDisable=0;

	_DeviceTransmitPowerSet=0;
	_DeviceTransmitGainSet=0;

	_DeviceEepromRead=0;
	_DeviceEepromWrite=0;

    _DeviceConfigurationRestore=0;
    _DeviceConfigurationSave=0;
    _DeviceCalibrationPierSet=0;
    _DevicePowerControlOverride=0;
    _DeviceTargetPowerSet=0;
    _DeviceTargetPowerGet=0;
    _DeviceTargetPowerApply=0;

    _DeviceMemoryRead=0;
    _DeviceMemoryWrite=0;

    _DeviceRegisterRead=0;
    _DeviceRegisterWrite=0;
}

void DeviceBssIdSetFunction(int (*f)(unsigned char *bssid))
{
	_DeviceBssIdSet=f;
}

void DeviceStationIdSetFunction(int (*f)(unsigned char *macaddr))
{
	_DeviceStationIdSet=f;
}

void DeviceReceiveDescriptorPointerFunction(int (*f)(unsigned int descriptor))
{
	_DeviceReceiveDescriptorPointer=f;
}

void DeviceReceiveUnicastFunction(int (*f)(int on))
{
	_DeviceReceiveUnicast=f;
}

void DeviceReceiveBroadcastFunction(int (*f)(int on))
{
	_DeviceReceiveBroadcast=f;
}

void DeviceReceivePromiscuousFunction(int (*f)(int on))
{
	_DeviceReceivePromiscuous=f;
}

void DeviceReceiveEnableFunction(int (*f)())
{
	_DeviceReceiveEnable=f;
}

void DeviceReceiveDisableFunction(int (*f)())
{
	_DeviceReceiveDisable=f;
}

void DeviceTransmitDescriptorStatusPointerFunction(int (*f)(unsigned int first, unsigned int last))
{
	_DeviceTransmitDescriptorStatusPointer=f;
}

void DeviceTransmitDescriptorPointerFunction(int (*f)(int qcu, unsigned int descriptor))
{
	_DeviceTransmitDescriptorPointer=f;
}

void DeviceTransmitRetryLimitFunction(int (*f)(int dcu, int retry))
{
	_DeviceTransmitRetryLimit=f;
}

void DeviceTransmitQueueSetupFunction(int (*f)(int qcu, int dcu))
{
	_DeviceTransmitQueueSetup=f;
}

void DeviceReceiveDeafModeFunction(int (*f)(int on))
{
	_DeviceReceiveDeafMode=f;
}

void DeviceTransmitRegularDataFunction(int (*f)())
{
	_DeviceTransmitRegularData=f;
}

void DeviceTransmitFrameDataFunction(int (*f)(int ifs))
{
	_DeviceTransmitFrameData=f;
}

void DeviceTransmitContinuousDataFunction(int (*f)())
{
	_DeviceTransmitContinuousData=f;
}

void DeviceTransmitCarrierFunction(int (*f)(int frequency, unsigned int txchain))
{
	_DeviceTransmitCarrier=f;
}

void DeviceTransmitEnableFunction(int (*f)(unsigned int qmask))
{
	_DeviceTransmitEnable=f;
}

void DeviceTransmitDisableFunction(int (*f)(unsigned int qmask))
{
	_DeviceTransmitDisable=f;
}

void DeviceTransmitPowerSetFunction(int (*f)(int mode, double txp))
{
	_DeviceTransmitPowerSet=f;
}

void DeviceTransmitGainReadFunction(int (*f)(int entry, unsigned int *rvalue, int *value, int max))
{
	_DeviceTransmitGainRead=f;
}

void DeviceTransmitGainWriteFunction(int (*f)(int entry, int *value, int nvalue))
{
	_DeviceTransmitGainWrite=f;
}

void DeviceTransmitGainSetFunction(int (*f)(int mode, int pcdac))
{
	_DeviceTransmitGainSet=f;
}

void DeviceEepromReadFunction(int (*f)(unsigned int address, unsigned char *value, int count))
{
	_DeviceEepromRead=f;
}

void DeviceEepromWriteFunction(int (*f)(unsigned int address, unsigned char *value, int count))
{
	_DeviceEepromWrite=f;
}

void DeviceConfigurationRestoreFunction(int (*f)(void))
{
	_DeviceConfigurationRestore=f;
}

void DeviceConfigurationSaveFunction(int (*f)(void))
{
	_DeviceConfigurationSave=f;
}

void DeviceCalibrationPierSetFunction(int (*f)(int pier, int frequency, int chain, int correction, int voltage, int temperature))
{
    _DeviceCalibrationPierSet=f;
}

void DevicePowerControlOverrideFunction(int (*f)(int frequency, int *correction, int *voltage, int *temperature))
{
    _DevicePowerControlOverride=f;
}

void DeviceTargetPowerSetFunction(int (*f)(void))
{
    _DeviceTargetPowerSet=f;
}

void DeviceTargetPowerGetFunction(int (*f)(int frequency, int rate, double *power))
{
    _DeviceTargetPowerGet=f;
}

void DeviceTargetPowerApplyFunction(int (*f)(int frequency))
{
    _DeviceTargetPowerApply=f;
}

void DeviceOtpReadFunction(int (*f)(unsigned int address, unsigned char *value, int count))
{
	_DeviceOtpRead=f;
}

void DeviceOtpWriteFunction(int (*f)(unsigned int address, unsigned char *value, int count))
{
	_DeviceOtpWrite=f;
}

void DeviceMemoryReadFunction(int (*f)(unsigned int address, unsigned int *value, int count))
{
	_DeviceMemoryRead=f;
}

void DeviceMemoryWriteFunction(int (*f)(unsigned int address, unsigned int *value, int count))
{
	_DeviceMemoryWrite=f;
}

void DeviceRegisterReadFunction(int (*f)(unsigned int address, unsigned int *value))
{
	_DeviceRegisterRead=f;
}

void DeviceRegisterWriteFunction(int (*f)(unsigned int address, unsigned int value))
{
	_DeviceRegisterWrite=f;
}

void DeviceTemperatureGetFunction(int (*f)(int))
{
	_DeviceTemperatureGet=f;
}

void DeviceVoltageGetFunction(int (*f)(void))
{
	_DeviceVoltageGet=f;
}
