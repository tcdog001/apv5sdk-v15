/*
 *  Copyright ?2005 Atheros Communications, Inc.,  All Rights Reserved.
 *
 */



#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "smatch.h"
#include "UserPrint.h"
#include "Device.h"
#include "DeviceLoad.h"


#define MBUFFER 1024


static HINSTANCE hinstLib;

//static int (*_DeviceAttach)(int devid, int ssid);
//static int (*_DeviceReset)(int frequency, int ht40, unsigned char txchain, unsigned char rxchain);
static int (*_DeviceBssIdSet)(unsigned char *bssid);
typedef int (*_DeviceBssIdSetFunction)(unsigned char *bssid);

static int (*_DeviceStationIdSet)(unsigned char *macaddr);
typedef int (*_DeviceStationIdSetFunction)(unsigned char *macaddr);

static int (*_DeviceReceiveDescriptorPointer)(unsigned int descriptor);
typedef int (*_DeviceReceiveDescriptorPointerFunction)(unsigned int descriptor);

static int (*_DeviceReceiveUnicast)(int on);
typedef int (*_DeviceReceiveUnicastFunction)(int on);

static int (*_DeviceReceiveBroadcast)(int on);
typedef int (*_DeviceReceiveBroadcastFunction)(int on);

static int (*_DeviceReceivePromiscuous)(int on);
typedef int (*_DeviceReceivePromiscuousFunction)(int on);

static int (*_DeviceReceiveEnable)(void);
typedef int (*_DeviceReceiveEnableFunction)(void);

static int (*_DeviceReceiveDisable)(void);
typedef int (*_DeviceReceiveDisableFunction)(void);

static int (*_DeviceReceiveDeafMode)(int deaf);
typedef int (*_DeviceReceiveDeafModeFunction)(int deaf);

static int (*_DeviceTransmitDescriptorStatusPointer)(unsigned int first, unsigned int last);
typedef int (*_DeviceTransmitDescriptorStatusPointerFunction)(unsigned int first, unsigned int last);

static int (*_DeviceTransmitDescriptorPointer)(int qcu, unsigned int descriptor);
typedef int (*_DeviceTransmitDescriptorPointerFunction)(int qcu, unsigned int descriptor);

static int (*_DeviceTransmitRetryLimit)(int dcu, int retry);
typedef int (*_DeviceTransmitRetryLimitFunction)(int dcu, int retry);

static int (*_DeviceTransmitQueueSetup)(int qcu, int dcu);
typedef int (*_DeviceTransmitQueueSetupFunction)(int qcu, int dcu);

static int (*_DeviceTransmitRegularData)(void);		// normal
typedef int (*_DeviceTransmitRegularDataFunction)(void);		// normal

static int (*_DeviceTransmitFrameData)(int ifs);	// tx99
typedef int (*_DeviceTransmitFrameDataFunction)(int ifs);	// tx99

static int (*_DeviceTransmitContinuousData)(void);		// tx100
typedef int (*_DeviceTransmitContinuousDataFunction)(void);		// tx100

static int (*_DeviceTransmitCarrier)(int frequency, unsigned int txchain);				// carrier only
typedef int (*_DeviceTransmitCarrierFunction)(int frequency, unsigned int txchain);				// carrier only

static int (*_DeviceTransmitEnable)(unsigned int qmask);
typedef int (*_DeviceTransmitEnableFunction)(unsigned int qmask);

static int (*_DeviceTransmitDisable)(unsigned int qmask);
typedef int (*_DeviceTransmitDisableFunction)(unsigned int qmask);

static int (*_DeviceTransmitPowerSet)(int mode, double txp);
typedef int (*_DeviceTransmitPowerSetFunction)(int mode, double txp);

static int (*_DeviceTransmitGainSet)(int mode, int pcdac);
typedef int (*_DeviceTransmitGainSetFunction)(int mode, int pcdac);

static int (*_DeviceTransmitGainRead)(int entry, unsigned int *rvalue, int *value, int max);
typedef int (*_DeviceTransmitGainReadFunction)(int entry, unsigned int *rvalue, int *value, int max);

static int (*_DeviceTransmitGainWrite)(int entry, int *value, int nvalue);
typedef int (*_DeviceTransmitGainWriteFunction)(int entry, int *value, int nvalue);

static int (*_DeviceEepromRead)(unsigned int address, unsigned char *value, int count);
typedef int (*_DeviceEepromReadFunction)(unsigned int address, unsigned char *value, int count);

static int (*_DeviceEepromWrite)(unsigned int address, unsigned char *value, int count);
typedef int (*_DeviceEepromWriteFunction)(unsigned int address, unsigned char *value, int count);

static int (*_DeviceOtpRead)(unsigned int address, unsigned char *value, int count);
typedef int (*_DeviceOtpReadFunction)(unsigned int address, unsigned char *value, int count);

static int (*_DeviceOtpWrite)(unsigned int address, unsigned char *value, int count);
typedef int (*_DeviceOtpWriteFunction)(unsigned int address, unsigned char *value, int count);

static int (*_DeviceMemoryRead)(unsigned int address, unsigned int *value, int count);
typedef int (*_DeviceMemoryReadFunction)(unsigned int address, unsigned int *value, int count);

static int (*_DeviceMemoryWrite)(unsigned int address, unsigned int *value, int count);
typedef int (*_DeviceMemoryWriteFunction)(unsigned int address, unsigned int *value, int count);

static int (*_DeviceRegisterRead)(unsigned int address, unsigned int *value);
typedef int (*_DeviceRegisterReadFunction)(unsigned int address, unsigned int *value);

static int (*_DeviceRegisterWrite)(unsigned int address, unsigned int value);
typedef int (*_DeviceRegisterWriteFunction)(unsigned int address, unsigned int value);

static int (*_DeviceConfigurationRestore)(void);
typedef int (*_DeviceConfigurationRestoreFunction)(void);

static int (*_DeviceConfigurationSave)(void);
typedef int (*_DeviceConfigurationSaveFunction)(void);

static int (*_DeviceCalibrationPierSet)(int pier, int frequency, int chain, int correction, int voltage, int temperature);
typedef int (*_DeviceCalibrationPierSetFunction)(int pier, int frequency, int chain, int correction, int voltage, int temperature);

static int (*_DevicePowerControlOverride)(int *correction, int *voltage, int *temperature);
typedef int (*_DevicePowerControlOverrideFunction)(int *correction, int *voltage, int *temperature);

static int (*_DeviceTargetPowerSet)(/* arguments ???? */);
typedef int (*_DeviceTargetPowerSetFunction)(/* arguments ???? */);

static int (*_DeviceTargetPowerGet)(int frequency, int rate, double *power);
typedef int (*_DeviceTargetPowerGetFunction)(int frequency, int rate, double *power);

static int (*_DeviceTargetPowerApply)(int frequency);
typedef int (*_DeviceTargetPowerApplyFunction)(int frequency);

//typedef int (*_DeviceReceiveEnableFunction)(void);
//static int (*_DeviceReceiveEnable)(void);

typedef char * (*_DeviceNameFunction)(void);
static char * (*_DeviceName)(void);

typedef char * (*_DevicePrefixFunction)(void);
static char * (*_DevicePrefix)(void);

//
// Returns a pointer to the specified function.
//
static void *DeviceFunctionFind(char *prefix, char *function)
{
    void *f;
    char buffer[MBUFFER];

    if(hinstLib!=0)
    {
        f=GetProcAddress(hinstLib, function);
        //
        // try adding the prefix in front of the name
        //
        if(f==0)
        {
            SformatOutput(buffer,MBUFFER-1,"%s%s",prefix,function);
            buffer[MBUFFER-1]=0;
            f=GetProcAddress(hinstLib, buffer);
        }
        return f;
    }
    return 0;
}


//
// unload the dll
//
void DeviceUnload()
{
    if(hinstLib!=0)
    {
	    FreeLibrary(hinstLib);
        hinstLib=0;
    }
}


//
// loads the device control dll 
//
int DeviceLoad(char *dllname)
{
    char buffer[MBUFFER];
    int error;
    char *name;
    char *prefix;

    if(hinstLib!=0)
    {
        DeviceUnload();
    }
    // 
    // Load DLL file
    //
	hinstLib = LoadLibrary(TEXT(dllname));
    if(hinstLib==0) 
    {
        SformatOutput(buffer,MBUFFER-1,"%s.dll",dllname);
        buffer[MBUFFER-1]=0;
	    hinstLib = LoadLibrary(TEXT(buffer));
        if(hinstLib==0) 
        {
		    UserPrint("ERROR: unable to load Equipment dll %s\n",dllname);
		    return -1;
        }
	}
    //
    // Clear all function pointers
    //
	DeviceFunctionReset();
	// 
    // Get function pointers for this device
    //
    error=0;
    prefix=dllname;
 	_DevicePrefix = (_DevicePrefixFunction)DeviceFunctionFind(prefix,"DevicePrefix");
    if(_DevicePrefix!=0)
    {
        prefix=(_DevicePrefix)();
        UserPrint("device prefix = %s\n",prefix);
    }
 	_DeviceName = (_DeviceNameFunction)DeviceFunctionFind(prefix,"DeviceName");
    if(_DeviceName==0)
    {
        UserPrint("can't find DeviceName function\n");
    }

	_DeviceMemoryRead = (_DeviceMemoryReadFunction)DeviceFunctionFind(prefix,"DeviceMemoryRead");
	if(_DeviceMemoryRead==0)
	{
		UserPrint("can't find DeviceMemoryRead function\n");
		error++;
	}
	else
	{
		DeviceMemoryReadFunction(_DeviceMemoryRead);
	}

	_DeviceMemoryWrite = (_DeviceMemoryWriteFunction)DeviceFunctionFind(prefix,"DeviceMemoryWrite");
	if(_DeviceMemoryRead==0)
	{
		UserPrint("can't find DeviceMemoryWrite function\n");
		error++;
	}
	else
	{
		DeviceMemoryWriteFunction(_DeviceMemoryWrite);
	}

	_DeviceRegisterRead = (_DeviceRegisterReadFunction)DeviceFunctionFind(prefix,"DeviceRegisterRead");
	if(_DeviceRegisterRead==0)
	{
		UserPrint("can't find DeviceRegisterRead function\n");
		error++;
	}
	else
	{
		DeviceRegisterReadFunction(_DeviceRegisterRead);
	}

	_DeviceRegisterWrite = (_DeviceRegisterWriteFunction)DeviceFunctionFind(prefix,"DeviceRegisterWrite");
	if(_DeviceRegisterWrite==0)
	{
		UserPrint("can't find DeviceRegisterWrite function\n");
		error++;
	}
	else
	{
		DeviceRegisterWriteFunction(_DeviceRegisterWrite);
	}

	_DeviceEepromRead = (_DeviceEepromReadFunction)DeviceFunctionFind(prefix,"DeviceEepromRead");
	if(_DeviceEepromRead==0)
	{
		UserPrint("can't find DeviceEepromRead function\n");
		error++;
	}
	else
	{
		DeviceEepromReadFunction(_DeviceEepromRead);
	}

	_DeviceEepromWrite = (_DeviceEepromWriteFunction)DeviceFunctionFind(prefix,"DeviceEepromWrite");
	if(_DeviceEepromWrite==0)
	{
		UserPrint("can't find DeviceEepromWrite function\n");
		error++;
	}
	else
	{
		DeviceEepromWriteFunction(_DeviceEepromWrite);
	}

	_DeviceBssIdSet = (_DeviceBssIdSetFunction)DeviceFunctionFind(prefix,"DeviceBssIdSet");
	if(_DeviceBssIdSet==0)
	{
		UserPrint("can't find DeviceBssIdSet function\n");
		error++;
	}
	else
	{
		DeviceBssIdSetFunction(_DeviceBssIdSet);
	}

	_DeviceStationIdSet = (_DeviceStationIdSetFunction)DeviceFunctionFind(prefix,"DeviceStationIdSet");
	if(_DeviceStationIdSet==0)
	{
		UserPrint("can't find DeviceStationIdSet function\n");
		error++;
	}
	else
	{
		DeviceStationIdSetFunction(_DeviceStationIdSet);
	}

	_DeviceReceiveDescriptorPointer = (_DeviceReceiveDescriptorPointerFunction)DeviceFunctionFind(prefix,"DeviceReceiveDescriptorPointer");
	if(_DeviceReceiveDescriptorPointer==0)
	{
		UserPrint("can't find DeviceReceiveDescriptorPointer function\n");
		error++;
	}
	else
	{
		DeviceReceiveDescriptorPointerFunction(_DeviceReceiveDescriptorPointer);
	}

	_DeviceReceiveUnicast = (_DeviceReceiveUnicastFunction)DeviceFunctionFind(prefix,"DeviceReceiveUnicast");
	if(_DeviceReceiveUnicast==0)
	{
		UserPrint("can't find DeviceReceiveUnicast function\n");
		error++;
	}
	else
	{
		DeviceReceiveUnicastFunction(_DeviceReceiveUnicast);
	}

	_DeviceReceiveBroadcast = (_DeviceReceiveBroadcastFunction)DeviceFunctionFind(prefix,"DeviceReceiveBroadcast");
	if(_DeviceReceiveBroadcast==0)
	{
		UserPrint("can't find DeviceReceiveBroadcast function\n");
		error++;
	}
	else
	{
		DeviceReceiveBroadcastFunction(_DeviceReceiveBroadcast);
	}

	_DeviceReceivePromiscuous = (_DeviceReceivePromiscuousFunction)DeviceFunctionFind(prefix,"DeviceReceivePromiscuous");
	if(_DeviceReceivePromiscuous==0)
	{
		UserPrint("can't find DeviceReceivePromiscuous function\n");
		error++;
	}
	else
	{
		DeviceReceivePromiscuousFunction(_DeviceReceivePromiscuous);
	}

	_DeviceReceiveEnable = (_DeviceReceiveEnableFunction)DeviceFunctionFind(prefix,"DeviceReceiveEnable");
	if(_DeviceReceiveEnable==0)
	{
		UserPrint("can't find DeviceReceiveEnable function\n");
		error++;
	}
	else
	{
		DeviceReceiveEnableFunction(_DeviceReceiveEnable);
	}

	_DeviceReceiveDisable = (_DeviceReceiveDisableFunction)DeviceFunctionFind(prefix,"DeviceReceiveDisable");
	if(_DeviceReceiveDisable==0)
	{
		UserPrint("can't find DeviceReceiveDisable function\n");
		error++;
	}
	else
	{
		DeviceReceiveDisableFunction(_DeviceReceiveDisable);
	}

	_DeviceTransmitCarrier = (_DeviceTransmitCarrierFunction)DeviceFunctionFind(prefix,"DeviceTransmitCarrier");
	if(_DeviceTransmitCarrier==0)
	{
		UserPrint("can't find DeviceTransmitCarrier function\n");
		error++;
	}
	else
	{
		DeviceTransmitCarrierFunction(_DeviceTransmitCarrier);
	}

	_DeviceTransmitDescriptorStatusPointer = (_DeviceTransmitDescriptorStatusPointerFunction)DeviceFunctionFind(prefix,"DeviceTransmitDescriptorStatusPointer");
	if(_DeviceTransmitDescriptorStatusPointer==0)
	{
		UserPrint("can't find DeviceTransmitDescriptorStatusPointer function\n");
		error++;
	}
	else
	{
		DeviceTransmitDescriptorStatusPointerFunction(_DeviceTransmitDescriptorStatusPointer);
	}

	_DeviceTransmitDescriptorPointer = (_DeviceTransmitDescriptorPointerFunction)DeviceFunctionFind(prefix,"DeviceTransmitDescriptorPointer");
	if(_DeviceTransmitDescriptorPointer==0)
	{
		UserPrint("can't find DeviceTransmitDescriptorPointer function\n");
		error++;
	}
	else
	{
		DeviceTransmitDescriptorPointerFunction(_DeviceTransmitDescriptorPointer);
	}

	_DeviceTransmitQueueSetup = (_DeviceTransmitQueueSetupFunction)DeviceFunctionFind(prefix,"DeviceTransmitQueueSetup");
	if(_DeviceTransmitQueueSetup==0)
	{
		UserPrint("can't find DeviceTransmitQueueSetup function\n");
		error++;
	}
	else
	{
		DeviceTransmitQueueSetupFunction(_DeviceTransmitQueueSetup);
	}

	_DeviceTransmitRegularData = (_DeviceTransmitRegularDataFunction)DeviceFunctionFind(prefix,"DeviceTransmitRegularData");
	if(_DeviceTransmitRegularData==0)
	{
		UserPrint("can't find DeviceTransmitRegularData function\n");
		error++;
	}
	else
	{
		DeviceTransmitRegularDataFunction(_DeviceTransmitRegularData);
	}

	_DeviceTransmitFrameData = (_DeviceTransmitFrameDataFunction)DeviceFunctionFind(prefix," DeviceTransmitFrameData");
	if(_DeviceTransmitFrameData==0)
	{
		UserPrint("can't find  DeviceTransmitFrameData function\n");
		error++;
	}
	else
	{
		 DeviceTransmitFrameDataFunction(_DeviceTransmitFrameData);
	}

	_DeviceTransmitDisable = (_DeviceTransmitDisableFunction)DeviceFunctionFind(prefix," DeviceTransmitDisable");
	if(_DeviceTransmitDisable==0)
	{
		UserPrint("can't find  DeviceTransmitDisable function\n");
		error++;
	}
	else
	{
		DeviceTransmitDisableFunction(_DeviceTransmitDisable);
	}

	_DeviceTransmitContinuousData = (_DeviceTransmitContinuousDataFunction)DeviceFunctionFind(prefix," DeviceTransmitContinuousData");
	if(_DeviceTransmitContinuousData==0)
	{
		UserPrint("can't find  DeviceTransmitContinuousData function\n");
		error++;
	}
	else
	{
		DeviceTransmitContinuousDataFunction(_DeviceTransmitContinuousData);
	}

	_DeviceTransmitGainRead = (_DeviceTransmitGainReadFunction)DeviceFunctionFind(prefix," DeviceTransmitGainRead");
	if(_DeviceTransmitGainRead==0)
	{
		UserPrint("can't find  DeviceTransmitGainRead function\n");
		error++;
	}
	else
	{
		DeviceTransmitGainReadFunction(_DeviceTransmitGainRead);
	}

	_DeviceTransmitGainWrite = (_DeviceTransmitGainWriteFunction)DeviceFunctionFind(prefix," DeviceTransmitGainWrite");
	if(_DeviceTransmitGainWrite==0)
	{
		UserPrint("can't find  DeviceTransmitGainWrite function\n");
		error++;
	}
	else
	{
		DeviceTransmitGainWriteFunction(_DeviceTransmitGainWrite);
	}

	_DeviceTransmitGainSet = (_DeviceTransmitGainSetFunction)DeviceFunctionFind(prefix," DeviceTransmitGainSet");
	if(_DeviceTransmitGainSet==0)
	{
		UserPrint("can't find  DeviceTransmitGainSet function\n");
		error++;
	}
	else
	{
		DeviceTransmitGainSetFunction(_DeviceTransmitGainSet);
	}

	_DeviceTransmitPowerSet = (_DeviceTransmitPowerSetFunction)DeviceFunctionFind(prefix," DeviceTransmitPowerSet");
	if(_DeviceTransmitPowerSet==0)
	{
		UserPrint("can't find  DeviceTransmitPowerSet function\n");
		error++;
	}
	else
	{
		DeviceTransmitPowerSetFunction(_DeviceTransmitPowerSet);
	}

	_DeviceTargetPowerGet = (_DeviceTargetPowerGetFunction)DeviceFunctionFind(prefix," DeviceTargetPowerGet");
	if(_DeviceTargetPowerGet==0)
	{
		UserPrint("can't find  DeviceTargetPowerGet function\n");
		error++;
	}
	else
	{
		DeviceTargetPowerGetFunction(_DeviceTargetPowerGet);
	}

#ifdef TODO
	DeviceOtpReadFunction(Ar9300OtpRead);
	DeviceOtpWriteFunction(Ar9300OtpWrite);

#ifdef UNUSED
    DeviceTransmitRetryLimitFunction(Ar9300TransmitRetryLimit);
    DeviceTransmitEnableFunction(Ar9300TransmitEnable);
#endif

	DeviceTargetPowerApplyFunction(Ar9300TargetPowerApply);

    DeviceConfigurationRestoreFunction(Ar9300EepromRestore);
    DeviceConfigurationSaveFunction(Ar9300EepromSave);

    DeviceCalibrationPierSetFunction(Ar9300EepromUpdateCalPier);
    DevicePowerControlOverrideFunction(Ar9300PowerControlOverride);
//    DeviceTargetPowerSetFunction(/* arguments????*/)
//    DeviceTargetPowerApplyFunction(void)

//    DeviceDeafFunction(Ar9300Deaf);

    also do everthing in RxDescriptor.c and TxDescriptor.c

#endif  // TODO
    //
    // if any of the required functions are missing, unload it and return error.
    //
    if(error>0)
    {
        DeviceUnload();
        return -1;
    }

    name=0;
    if(_DeviceName!=0)
    {
        name=(_DeviceName)();
    }
    if(name==0)
    {
        name="UNKNOWN";
    }
    UserPrint("Loaded device control functions for %s from %s.\n",name,dllname);

    return 0;
}

