
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>

#include "smatch.h"
#include "UserPrint.h"
#include "TimeMillisecond.h"

#include "Ar9300Device.h"

#include "wlantype.h"

#if !defined(LINUX) && !defined(__APPLE__)
#include "osdep.h"
#endif


#include "Ar9300EepromSave.h"

#include "ah.h"
#include "ah_internal.h"
#include "ar9300eep.h"
#include "ar9300.h"

#include "Ar9300EepromStructSet.h"
#include "Ar9300EepromParameter.h"
#include "ar9300EepromPrint.h"


#define MBLOCK 10

#define MPRINTBUFFER 1024

//
// This fun structure contains the names, sizes, and types for all of the fields in the eeprom structure.
// It exists so that we can easily compare two structures and report the differences.
// It must match the definition of ar9300_eeprom_t in ar9300eep.h.
// Fields must be defined in the same order as they occur in the structure.
//

struct _EepromPrint
{
	char *name;
	short offset;
	char size;
	short nx,ny,nz;
	char type;	// f -- floating point
				// d -- decimal
				// x -- hexadecimal
				// u -- unsigned
				// c -- character
				// t -- text
				// p -- transmit power, 0.5*value as floating point 
				// 2 -- compressed 2GHz frequency
				// 5 -- compressed 5GHz frequency
				// m -- mac address
	int interleave;
	int high;
	int low;
	int voff;
};

static struct _EepromPrint _Ar9300EepromList[]=
{
	{Ar9300EepromVersion,offsetof(ar9300_eeprom_t,eepromVersion),sizeof(u_int8_t),1,1,1,'u',1,-1,-1,0},
	{Ar9300EepromTemplateVersion,offsetof(ar9300_eeprom_t,templateVersion),sizeof(u_int8_t),1,1,1,'u',1,-1,-1,0},
	{Ar9300EepromMacAddress,offsetof(ar9300_eeprom_t,macAddr),6,sizeof(u_int8_t),1,1,'m',1,-1,-1,0},
	{Ar9300EepromCustomerData,offsetof(ar9300_eeprom_t,custData),OSPREY_CUSTOMER_DATA_SIZE,sizeof(u_int8_t),1,1,'t',1,-1,-1,0},
	{Ar9300EepromRegulatoryDomain,offsetof(ar9300_eeprom_t,baseEepHeader.regDmn),sizeof(u_int16_t),2,1,1,'x',1,-1,-1,0},
	{Ar9300EepromTxRxMask,offsetof(ar9300_eeprom_t,baseEepHeader.txrxMask),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
		{Ar9300EepromTxRxMaskTx,offsetof(ar9300_eeprom_t,baseEepHeader.txrxMask),sizeof(u_int8_t),1,1,1,'x',1,7,4,0},
		{Ar9300EepromTxRxMaskRx,offsetof(ar9300_eeprom_t,baseEepHeader.txrxMask),sizeof(u_int8_t),1,1,1,'x',1,3,0,0},
	{Ar9300EepromOpFlags,offsetof(ar9300_eeprom_t,baseEepHeader.opCapFlags.opFlags),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromEepMisc,offsetof(ar9300_eeprom_t,baseEepHeader.opCapFlags.eepMisc),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromRfSilent,offsetof(ar9300_eeprom_t,baseEepHeader.rfSilent),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
		{Ar9300EepromRfSilentB0,offsetof(ar9300_eeprom_t,baseEepHeader.rfSilent),sizeof(u_int8_t),1,1,1,'x',1,0,0,0},
		{Ar9300EepromRfSilentB1,offsetof(ar9300_eeprom_t,baseEepHeader.rfSilent),sizeof(u_int8_t),1,1,1,'x',1,1,1,0},
		{Ar9300EepromRfSilentGpio,offsetof(ar9300_eeprom_t,baseEepHeader.rfSilent),sizeof(u_int8_t),1,1,1,'x',1,7,2,0},
	{Ar9300EepromBlueToothOptions,offsetof(ar9300_eeprom_t,baseEepHeader.blueToothOptions),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromDeviceCapability,offsetof(ar9300_eeprom_t,baseEepHeader.deviceCap),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromDeviceType,offsetof(ar9300_eeprom_t,baseEepHeader.deviceType),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromPowerTableOffset,offsetof(ar9300_eeprom_t,baseEepHeader.pwrTableOffset),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300EepromTuningCaps,offsetof(ar9300_eeprom_t,baseEepHeader.params_for_tuning_caps),sizeof(u_int8_t),2,1,1,'x',1,-1,-1,0},
	{Ar9300EepromFeatureEnable,offsetof(ar9300_eeprom_t,baseEepHeader.featureEnable),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
		{Ar9300EepromFeatureEnableTemperatureCompensation,offsetof(ar9300_eeprom_t,baseEepHeader.featureEnable),sizeof(u_int8_t),1,1,1,'x',1,0,0,0},
		{Ar9300EepromFeatureEnableVoltageCompensation,offsetof(ar9300_eeprom_t,baseEepHeader.featureEnable),sizeof(u_int8_t),1,1,1,'x',1,1,1,0},
		{Ar9300EepromFeatureEnableFastClock,offsetof(ar9300_eeprom_t,baseEepHeader.featureEnable),sizeof(u_int8_t),1,1,1,'x',1,2,2,0},
		{Ar9300EepromFeatureEnableDoubling,offsetof(ar9300_eeprom_t,baseEepHeader.featureEnable),sizeof(u_int8_t),1,1,1,'x',1,3,3,0},
		{Ar9300EepromFeatureEnableInternalSwitchingRegulator,offsetof(ar9300_eeprom_t,baseEepHeader.featureEnable),sizeof(u_int8_t),1,1,1,'x',1,4,4,0},
		{Ar9300EepromFeatureEnablePaPredistortion,offsetof(ar9300_eeprom_t,baseEepHeader.featureEnable),sizeof(u_int8_t),1,1,1,'x',1,5,5,0},
		{Ar9300EepromFeatureEnableTuningCaps,offsetof(ar9300_eeprom_t,baseEepHeader.featureEnable),sizeof(u_int8_t),1,1,1,'x',1,6,6,0},
	{Ar9300EepromMiscellaneous,offsetof(ar9300_eeprom_t,baseEepHeader.miscConfiguration),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
		{Ar9300EepromMiscellaneousDriveStrength,offsetof(ar9300_eeprom_t,baseEepHeader.miscConfiguration),sizeof(u_int8_t),1,1,1,'x',1,0,0,0},
		{Ar9300EepromMiscellaneousThermometer,offsetof(ar9300_eeprom_t,baseEepHeader.miscConfiguration),sizeof(u_int8_t),1,1,1,'d',1,2,1,-1},
		{Ar9300EepromMiscellaneousChainMaskReduce,offsetof(ar9300_eeprom_t,baseEepHeader.miscConfiguration),sizeof(u_int8_t),1,1,1,'x',1,3,3,0},
		{Ar9300EepromMiscellaneousQuickDropEnable,offsetof(ar9300_eeprom_t,baseEepHeader.miscConfiguration),sizeof(u_int8_t),1,1,1,'x',1,4,4,0},
	{Ar9300EepromEepromWriteEnableGpio,offsetof(ar9300_eeprom_t,baseEepHeader.eepromWriteEnableGpio),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromWlanDisableGpio,offsetof(ar9300_eeprom_t,baseEepHeader.wlanDisableGpio),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromWlanLedGpio,offsetof(ar9300_eeprom_t,baseEepHeader.wlanLedGpio),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromRxBandSelectGpio,offsetof(ar9300_eeprom_t,baseEepHeader.rxBandSelectGpio),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromTxRxGain,offsetof(ar9300_eeprom_t,baseEepHeader.txrxgain),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
		{Ar9300EepromTxRxGainTx,offsetof(ar9300_eeprom_t,baseEepHeader.txrxgain),sizeof(u_int8_t),1,1,1,'x',1,7,4,0},
		{Ar9300EepromTxRxGainRx,offsetof(ar9300_eeprom_t,baseEepHeader.txrxgain),sizeof(u_int8_t),1,1,1,'x',1,3,0,0},
	{Ar9300EepromSwitchingRegulator,offsetof(ar9300_eeprom_t,baseEepHeader.swreg),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},

	{Ar9300Eeprom2GHzAntennaControlCommon,offsetof(ar9300_eeprom_t,modalHeader2G.antCtrlCommon),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzAntennaControlCommon2,offsetof(ar9300_eeprom_t,modalHeader2G.antCtrlCommon2),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzAntennaControlChain,offsetof(ar9300_eeprom_t,modalHeader2G.antCtrlChain),sizeof(u_int16_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzAttenuationDb,offsetof(ar9300_eeprom_t,modalHeader2G.xatten1DB),sizeof(u_int8_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzAttenuationMargin,offsetof(ar9300_eeprom_t,modalHeader2G.xatten1Margin),sizeof(u_int8_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzTemperatureSlope,offsetof(ar9300_eeprom_t,modalHeader2G.tempSlope),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom2GHzVoltageSlope,offsetof(ar9300_eeprom_t,modalHeader2G.voltSlope),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom2GHzSpur,offsetof(ar9300_eeprom_t,modalHeader2G.spurChans),sizeof(u_int8_t),OSPREY_EEPROM_MODAL_SPURS,1,1,'2',1,-1,-1,0},
	{Ar9300Eeprom2GHzNoiseFloorThreshold,offsetof(ar9300_eeprom_t,modalHeader2G.noiseFloorThreshCh),sizeof(int8_t),OSPREY_MAX_CHAINS,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom2GHzReserved,offsetof(ar9300_eeprom_t,modalHeader2G.reserved),sizeof(u_int8_t),11,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzQuickDrop,offsetof(ar9300_eeprom_t,modalHeader2G.quickDrop),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom2GHzXpaBiasLevel,offsetof(ar9300_eeprom_t,modalHeader2G.xpaBiasLvl),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzTxFrameToDataStart,offsetof(ar9300_eeprom_t,modalHeader2G.txFrameToDataStart),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzTxFrameToPaOn,offsetof(ar9300_eeprom_t,modalHeader2G.txFrameToPaOn),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzTxClip,offsetof(ar9300_eeprom_t,modalHeader2G.txClip),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzAntennaGain,offsetof(ar9300_eeprom_t,modalHeader2G.antennaGain),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom2GHzSwitchSettling,offsetof(ar9300_eeprom_t,modalHeader2G.switchSettling),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzAdcSize,offsetof(ar9300_eeprom_t,modalHeader2G.adcDesiredSize),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom2GHzTxEndToXpaOff,offsetof(ar9300_eeprom_t,modalHeader2G.txEndToXpaOff),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzTxEndToRxOn,offsetof(ar9300_eeprom_t,modalHeader2G.txEndToRxOn),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzTxFrameToXpaOn,offsetof(ar9300_eeprom_t,modalHeader2G.txFrameToXpaOn),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzThresh62,offsetof(ar9300_eeprom_t,modalHeader2G.thresh62),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzPaPredistortionHt20,offsetof(ar9300_eeprom_t,modalHeader2G.papdRateMaskHt20),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzPaPredistortionHt40,offsetof(ar9300_eeprom_t,modalHeader2G.papdRateMaskHt40),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzFuture,offsetof(ar9300_eeprom_t,modalHeader2G.futureModal),sizeof(u_int8_t),MAX_MODAL_FUTURE,1,1,'x',1,-1,-1,0},

	{Ar9300EepromAntennaDiversityControl,offsetof(ar9300_eeprom_t,base_ext1.ant_div_control),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300EepromFuture,offsetof(ar9300_eeprom_t,base_ext1.future),sizeof(u_int8_t),MAX_BASE_EXTENSION_FUTURE,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzQuickDropLow,offsetof(ar9300_eeprom_t,base_ext1.quickDropLow),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzQuickDropHigh,offsetof(ar9300_eeprom_t,base_ext1.quickDropHigh),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},

	{Ar9300Eeprom2GHzCalibrationFrequency,offsetof(ar9300_eeprom_t,calFreqPier2G),sizeof(u_int8_t),OSPREY_NUM_2G_CAL_PIERS,1,1,'2',1,-1,-1,0},
//	{Ar9300Eeprom2GHzCalibrationData,offsetof(ar9300_eeprom_t,calPierData2G),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_2G_CAL_PIERS,6,'u',1,-1,-1,0},
	{Ar9300Eeprom2GHzPowerCorrection,offsetof(ar9300_eeprom_t,calPierData2G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,refPower),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_2G_CAL_PIERS,1,'d',6,-1,-1,0},
	{Ar9300Eeprom2GHzCalibrationVoltage,offsetof(ar9300_eeprom_t,calPierData2G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,voltMeas),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_2G_CAL_PIERS,1,'u',6,-1,-1,0},
	{Ar9300Eeprom2GHzCalibrationTemperature,offsetof(ar9300_eeprom_t,calPierData2G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,tempMeas),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_2G_CAL_PIERS,1,'u',6,-1,-1,0},
	{Ar9300Eeprom2GHzNoiseFloor,offsetof(ar9300_eeprom_t,calPierData2G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,rxNoisefloorCal),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_2G_CAL_PIERS,1,'d',6,-1,-1,0},
	{Ar9300Eeprom2GHzNoiseFloorPower,offsetof(ar9300_eeprom_t,calPierData2G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,rxNoisefloorPower),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_2G_CAL_PIERS,1,'d',6,-1,-1,0},
	{Ar9300Eeprom2GHzNoiseFloorTemperature,offsetof(ar9300_eeprom_t,calPierData2G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,rxTempMeas),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_2G_CAL_PIERS,1,'u',6,-1,-1,0},
	{Ar9300Eeprom2GHzTargetFrequencyCck,offsetof(ar9300_eeprom_t,calTarget_freqbin_Cck),sizeof(u_int8_t),OSPREY_NUM_2G_CCK_TARGET_POWERS,1,1,'2',1,-1,-1,0},
	{Ar9300Eeprom2GHzTargetFrequency,offsetof(ar9300_eeprom_t,calTarget_freqbin_2G),sizeof(u_int8_t),OSPREY_NUM_2G_20_TARGET_POWERS,1,1,'2',1,-1,-1,0},
	{Ar9300Eeprom2GHzTargetFrequencyHt20,offsetof(ar9300_eeprom_t,calTarget_freqbin_2GHT20),sizeof(u_int8_t),OSPREY_NUM_2G_20_TARGET_POWERS,1,1,'2',1,-1,-1,0},
	{Ar9300Eeprom2GHzTargetFrequencyHt40,offsetof(ar9300_eeprom_t,calTarget_freqbin_2GHT40),sizeof(u_int8_t),OSPREY_NUM_2G_40_TARGET_POWERS,1,1,'2',1,-1,-1,0},
	{Ar9300Eeprom2GHzTargetPowerCck,offsetof(ar9300_eeprom_t,calTargetPowerCck),sizeof(u_int8_t),OSPREY_NUM_2G_CCK_TARGET_POWERS,4,1,'p',1,-1,-1,0},
	{Ar9300Eeprom2GHzTargetPower,offsetof(ar9300_eeprom_t,calTargetPower2G),sizeof(u_int8_t),OSPREY_NUM_2G_20_TARGET_POWERS,4,1,'p',1,-1,-1,0},
	{Ar9300Eeprom2GHzTargetPowerHt20,offsetof(ar9300_eeprom_t,calTargetPower2GHT20),sizeof(u_int8_t),OSPREY_NUM_2G_20_TARGET_POWERS,14,1,'p',1,-1,-1,0},
	{Ar9300Eeprom2GHzTargetPowerHt40,offsetof(ar9300_eeprom_t,calTargetPower2GHT40),sizeof(u_int8_t),OSPREY_NUM_2G_40_TARGET_POWERS,14,1,'p',1,-1,-1,0},
	{Ar9300Eeprom2GHzCtlIndex,offsetof(ar9300_eeprom_t,ctlIndex_2G),sizeof(u_int8_t),OSPREY_NUM_CTLS_2G,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom2GHzCtlFrequency,offsetof(ar9300_eeprom_t,ctl_freqbin_2G),sizeof(u_int8_t),OSPREY_NUM_CTLS_2G,OSPREY_NUM_BAND_EDGES_2G,1,'2',1,-1,-1,0},
 	{Ar9300Eeprom2GHzCtlPower,offsetof(ar9300_eeprom_t,ctlPowerData_2G),sizeof(u_int8_t),OSPREY_NUM_CTLS_2G,OSPREY_NUM_BAND_EDGES_2G,1,'p',1,-1,-1,0},
 	{Ar9300Eeprom2GHzCtlBandEdge,offsetof(ar9300_eeprom_t,ctlPowerData_2G),sizeof(u_int8_t),OSPREY_NUM_CTLS_2G,OSPREY_NUM_BAND_EDGES_2G,1,'x',1,7,6,0},

	{Ar9300Eeprom5GHzAntennaControlCommon,offsetof(ar9300_eeprom_t,modalHeader5G.antCtrlCommon),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAntennaControlCommon2,offsetof(ar9300_eeprom_t,modalHeader5G.antCtrlCommon2),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAntennaControlChain,offsetof(ar9300_eeprom_t,modalHeader5G.antCtrlChain),sizeof(u_int16_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAttenuationDb,offsetof(ar9300_eeprom_t,modalHeader5G.xatten1DB),sizeof(u_int8_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAttenuationMargin,offsetof(ar9300_eeprom_t,modalHeader5G.xatten1Margin),sizeof(u_int8_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzTemperatureSlope,offsetof(ar9300_eeprom_t,modalHeader5G.tempSlope),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzVoltageSlope,offsetof(ar9300_eeprom_t,modalHeader5G.voltSlope),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzSpur,offsetof(ar9300_eeprom_t,modalHeader5G.spurChans),sizeof(u_int8_t),OSPREY_EEPROM_MODAL_SPURS,1,1,'5',1,-1,-1,0},
	{Ar9300Eeprom5GHzNoiseFloorThreshold,offsetof(ar9300_eeprom_t,modalHeader5G.noiseFloorThreshCh),sizeof(int8_t),OSPREY_MAX_CHAINS,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzReserved,offsetof(ar9300_eeprom_t,modalHeader5G.reserved),sizeof(u_int8_t),11,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzQuickDrop,offsetof(ar9300_eeprom_t,modalHeader5G.quickDrop),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzXpaBiasLevel,offsetof(ar9300_eeprom_t,modalHeader5G.xpaBiasLvl),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzTxFrameToDataStart,offsetof(ar9300_eeprom_t,modalHeader5G.txFrameToDataStart),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzTxFrameToPaOn,offsetof(ar9300_eeprom_t,modalHeader5G.txFrameToPaOn),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzTxClip,offsetof(ar9300_eeprom_t,modalHeader5G.txClip),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAntennaGain,offsetof(ar9300_eeprom_t,modalHeader5G.antennaGain),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzSwitchSettling,offsetof(ar9300_eeprom_t,modalHeader5G.switchSettling),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAdcSize,offsetof(ar9300_eeprom_t,modalHeader5G.adcDesiredSize),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzTxEndToXpaOff,offsetof(ar9300_eeprom_t,modalHeader5G.txEndToXpaOff),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzTxEndToRxOn,offsetof(ar9300_eeprom_t,modalHeader5G.txEndToRxOn),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzTxFrameToXpaOn,offsetof(ar9300_eeprom_t,modalHeader5G.txFrameToXpaOn),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzThresh62,offsetof(ar9300_eeprom_t,modalHeader5G.thresh62),sizeof(u_int8_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzPaPredistortionHt20,offsetof(ar9300_eeprom_t,modalHeader5G.papdRateMaskHt20),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzPaPredistortionHt40,offsetof(ar9300_eeprom_t,modalHeader5G.papdRateMaskHt40),sizeof(u_int32_t),1,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzFuture,offsetof(ar9300_eeprom_t,modalHeader5G.futureModal),sizeof(u_int8_t),10,1,1,'x',1,-1,-1,0},

	{Ar9300Eeprom5GHzTemperatureSlopeLow,offsetof(ar9300_eeprom_t,base_ext2.tempSlopeLow),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzTemperatureSlopeHigh,offsetof(ar9300_eeprom_t,base_ext2.tempSlopeHigh),sizeof(int8_t),1,1,1,'d',1,-1,-1,0},
	{Ar9300Eeprom5GHzAttenuationDbLow,offsetof(ar9300_eeprom_t,base_ext2.xatten1DBLow),sizeof(u_int8_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAttenuationMarginLow,offsetof(ar9300_eeprom_t,base_ext2.xatten1MarginLow),sizeof(u_int8_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAttenuationDbHigh,offsetof(ar9300_eeprom_t,base_ext2.xatten1DBHigh),sizeof(u_int8_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzAttenuationMarginHigh,offsetof(ar9300_eeprom_t,base_ext2.xatten1MarginHigh),sizeof(u_int8_t),OSPREY_MAX_CHAINS,1,1,'x',1,-1,-1,0},

	{Ar9300Eeprom5GHzCalibrationFrequency,offsetof(ar9300_eeprom_t,calFreqPier5G),sizeof(u_int8_t),OSPREY_NUM_5G_CAL_PIERS,1,1,'5',1,-1,-1,0},
// 	{Ar9300Eeprom5GHzCalibrationData,offsetof(ar9300_eeprom_t,calPierData5G),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_5G_CAL_PIERS,6,'u',1,-1,-1,0},
	{Ar9300Eeprom5GHzPowerCorrection,offsetof(ar9300_eeprom_t,calPierData5G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,refPower),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_5G_CAL_PIERS,1,'d',6,-1,-1,0},
	{Ar9300Eeprom5GHzCalibrationVoltage,offsetof(ar9300_eeprom_t,calPierData5G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,voltMeas),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_5G_CAL_PIERS,1,'u',6,-1,-1,0},
	{Ar9300Eeprom5GHzCalibrationTemperature,offsetof(ar9300_eeprom_t,calPierData5G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,tempMeas),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_5G_CAL_PIERS,1,'u',6,-1,-1,0},
	{Ar9300Eeprom5GHzNoiseFloor,offsetof(ar9300_eeprom_t,calPierData5G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,rxNoisefloorCal),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_5G_CAL_PIERS,1,'d',6,-1,-1,0},
	{Ar9300Eeprom5GHzNoiseFloorPower,offsetof(ar9300_eeprom_t,calPierData5G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,rxNoisefloorPower),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_5G_CAL_PIERS,1,'d',6,-1,-1,0},
	{Ar9300Eeprom5GHzNoiseFloorTemperature,offsetof(ar9300_eeprom_t,calPierData5G)+offsetof(OSP_CAL_DATA_PER_FREQ_OP_LOOP,rxTempMeas),sizeof(u_int8_t),OSPREY_MAX_CHAINS,OSPREY_NUM_5G_CAL_PIERS,1,'u',6,-1,-1,0},
	{Ar9300Eeprom5GHzTargetFrequency,offsetof(ar9300_eeprom_t,calTarget_freqbin_5G),sizeof(u_int8_t),OSPREY_NUM_5G_20_TARGET_POWERS,1,1,'5',1,-1,-1,0},
	{Ar9300Eeprom5GHzTargetFrequencyHt20,offsetof(ar9300_eeprom_t,calTarget_freqbin_5GHT20),sizeof(u_int8_t),OSPREY_NUM_5G_20_TARGET_POWERS,1,1,'5',1,-1,-1,0},
	{Ar9300Eeprom5GHzTargetFrequencyHt40,offsetof(ar9300_eeprom_t,calTarget_freqbin_5GHT40),sizeof(u_int8_t),OSPREY_NUM_5G_40_TARGET_POWERS,1,1,'5',1,-1,-1,0},
	{Ar9300Eeprom5GHzTargetPower,offsetof(ar9300_eeprom_t,calTargetPower5G),sizeof(u_int8_t),OSPREY_NUM_5G_20_TARGET_POWERS,4,1,'p',1,-1,-1,0},
	{Ar9300Eeprom5GHzTargetPowerHt20,offsetof(ar9300_eeprom_t,calTargetPower5GHT20),sizeof(u_int8_t),OSPREY_NUM_5G_20_TARGET_POWERS,14,1,'p',1,-1,-1,0},
	{Ar9300Eeprom5GHzTargetPowerHt40,offsetof(ar9300_eeprom_t,calTargetPower5GHT40),sizeof(u_int8_t),OSPREY_NUM_5G_40_TARGET_POWERS,14,1,'p',1,-1,-1,0},
	{Ar9300Eeprom5GHzCtlIndex,offsetof(ar9300_eeprom_t,ctlIndex_5G),sizeof(u_int8_t),OSPREY_NUM_CTLS_5G,1,1,'x',1,-1,-1,0},
	{Ar9300Eeprom5GHzCtlFrequency,offsetof(ar9300_eeprom_t,ctl_freqbin_5G),sizeof(u_int8_t),OSPREY_NUM_CTLS_5G,OSPREY_NUM_BAND_EDGES_5G,1,'5',1,-1,-1,0},
 	{Ar9300Eeprom5GHzCtlPower,offsetof(ar9300_eeprom_t,ctlPowerData_5G),sizeof(u_int8_t),OSPREY_NUM_CTLS_5G,OSPREY_NUM_BAND_EDGES_5G,1,'p',1,5,0,0},
 	{Ar9300Eeprom5GHzCtlBandEdge,offsetof(ar9300_eeprom_t,ctlPowerData_5G),sizeof(u_int8_t),OSPREY_NUM_CTLS_5G,OSPREY_NUM_BAND_EDGES_5G,1,'x',1,7,6,0},
};

static unsigned int Mask(int low, int high)
{
	unsigned int mask;
	int ib;

	mask=0;
	if(low<=0)
	{
		low=0;
	}
	if(high>=31)
	{
		high=31;
	}
	for(ib=low; ib<=high; ib++)
	{
		mask|=(1<<ib);
	}
	return mask;
}


static int Ar9300EepromPrintIt(unsigned char *data, int type, int size, int length, int high, int low, int voff,
	char *buffer, int max)
{
	char *vc;
	short *vs;
	int *vl;
	double *vd;
	float *vf;
	int lc, nc;
	int it;
	char text[MPRINTBUFFER];
	int vuse;
	unsigned int mask;
	int iuse;

	lc=0;
	switch(size)
	{
		case 1:
			vc=(char *)data;
			if(type=='t')
			{
				//
				// make sure there is a null
				//
				if(length>MPRINTBUFFER)
				{
					length=MPRINTBUFFER;
				}
				strncpy(text,vc,length);
				text[length]=0;

				nc=SformatOutput(&buffer[lc],max-lc-1,"%s",text);
				if(nc>0)
				{
					lc+=nc;
				}
			}
			else
			{
				for(it=0; it<length; it++)
				{
					vuse=vc[it]&0xff;
					if(high>=0 && low>=0)
					{
						mask=Mask(low,high);
						vuse=(vuse&mask)>>low;
					}
					switch(type)
					{
						case 'p':
#ifdef UNUSED
							if(vuse&0xc0)
							{
								nc=SformatOutput(&buffer[lc],max-lc-1,"%.1lf-%d",0.5*((int)(vuse&0x3f)),(int)(vuse>>6));
							}
							else
#endif
							{
								nc=SformatOutput(&buffer[lc],max-lc-1,"%.1lf",0.5*(voff+(int)(vuse&0x3f)));
							}
							break;
						case '2':
							if(vuse>0)
							{
								nc=SformatOutput(&buffer[lc],max-lc-1,"%d",2300+(voff+(int)vuse));
							}
							else
							{
								nc=SformatOutput(&buffer[lc],max-lc-1,"%d",0);
							}
							break;
						case '5':
							if(vuse>0)
							{
								nc=SformatOutput(&buffer[lc],max-lc-1,"%d",4800+5*(voff+(int)vuse));
							}
							else
							{
								nc=SformatOutput(&buffer[lc],max-lc-1,"%d",0);
							}
							break;
						case 'd':
							iuse=(int)vuse;
							if(vuse&0x80)
							{
								iuse=((int)vuse)|0xffffff00;
							}
							else
							{
								iuse=(int)vuse;
							}
							nc=SformatOutput(&buffer[lc],max-lc-1,"%+d",voff+iuse);
							break;
						case 'u':
							nc=SformatOutput(&buffer[lc],max-lc-1,"%u",(voff+(unsigned int)vuse)&0xff);
							break;
						case 'c':
							nc=SformatOutput(&buffer[lc],max-lc-1,"%1c",(voff+(unsigned int)vuse)&0xff);
							break;
						default:
						case 'x':
							nc=SformatOutput(&buffer[lc],max-lc-1,"0x%02x",(voff+(unsigned int)vuse)&0xff);
							break;
					}
					if(nc>0)
					{
						lc+=nc;
					}
					if(it<length-1)
					{
						nc=SformatOutput(&buffer[lc],max-lc-1,",");
						if(nc>0)
						{
							lc+=nc;
						}
					}
				}
			}
			break;
		case 2:
			vs=(short *)data;
			for(it=0; it<length; it++)
			{
				vuse=vs[it]&0xffff;
				if(high>=0 && low>=0)
				{
					mask=Mask(low,high);
					vuse=(vuse&mask)>>low;
				}
				switch(type)
				{
		            case 'd':
						iuse=(int)vuse;
						if(vuse&0x8000)
						{
							iuse=((int)vuse)|0xffff0000;
						}
						else
						{
							iuse=(int)vuse;
						}
			            nc=SformatOutput(&buffer[lc],max-lc-1,"%+d",voff+iuse);
						break;
		            case 'u':
			            nc=SformatOutput(&buffer[lc],max-lc-1,"%u",(voff+(unsigned int)vuse)&0xffff);
						break;
		            case 'c':
			            nc=SformatOutput(&buffer[lc],max-lc-1,"%2c",(voff+(unsigned int)vuse)&0xff,((voff+(unsigned int)vuse)>>8)&0xff);
						break;
					default:
		            case 'x':
			            nc=SformatOutput(&buffer[lc],max-lc-1,"0x%04x",(voff+(unsigned int)vuse)&0xffff);
						break;
				}
				if(nc>0)
				{
					lc+=nc;
				}
				if(it<length-1)
				{
			        nc=SformatOutput(&buffer[lc],max-lc-1,",");
					if(nc>0)
					{
						lc+=nc;
					}
				}
			}
			break;
		case 4:
			vl=(int *)data;
			vf=(float *)data;
			for(it=0; it<length; it++)
			{
				vuse=vl[it]&0xffffffff;
				if(high>=0 && low>=0)
				{
					mask=Mask(low,high);
					vuse=(vuse&mask)>>low;
				}
				switch(type)
				{
		            case 'd':
			            nc=SformatOutput(&buffer[lc],max-lc-1,"%+d",voff+vuse);
						break;
		            case 'u':
			            nc=SformatOutput(&buffer[lc],max-lc-1,"%u",voff+(vuse&0xffffffff));
						break;
		            case 'f':
			            nc=SformatOutput(&buffer[lc],max-lc-1,"%g",voff+vf[it]);
						break;
		            case 'c':
			            nc=SformatOutput(&buffer[lc],max-lc-1,"%4c",(voff+vuse)&0xff,((voff+(unsigned int)vuse)>>8)&0xff,((voff+(unsigned int)vuse)>>16)&0xff,((voff+(unsigned int)vuse)>>24)&0xff);
						break;
					default:
		            case 'x':
			            nc=SformatOutput(&buffer[lc],max-lc-1,"0x%08x",(voff+vuse)&0xffffffff);
						break;
				}
				if(nc>0)
				{
					lc+=nc;
				}
				if(it<length-1)
				{
			        nc=SformatOutput(&buffer[lc],max-lc-1,",");
					if(nc>0)
					{
						lc+=nc;
					}
				}
			}
			break;
		default:
			vc=(char *)data;
			if(type=='t')
			{
				//
				// make sure there is a null
				//
				if(size>MPRINTBUFFER)
				{
					size=MPRINTBUFFER;
				}
				strncpy(text,vc,size);
				text[size]=0;
				nc=SformatOutput(&buffer[lc],max-lc-1,"%s",text);
				if(nc>0)
				{
					lc+=nc;
				}
			}
			else if(type=='m')
			{
				nc=SformatOutput(&buffer[lc],max-lc-1,"%02x:%02x:%02x:%02x:%02x:%02x",
					vc[0]&0xff,vc[1]&0xff,vc[2]&0xff,vc[3]&0xff,vc[4]&0xff,vc[5]&0xff);
				if(nc>0)
				{
					lc+=nc;
				}
			}
			else
			{
				for(it=0; it<length; it++)
				{
					vuse=vc[it]&0xff;
					nc=SformatOutput(&buffer[lc],max-lc-1,"0x%02x",(voff+(unsigned int)vuse)&0xff);
					if(nc>0)
					{
						lc+=nc;
					}
				}
			}
			break;
    }
	return lc;
}


static void Ar9300EepromPrintEntry(void (*print)(char *format, ...), 
	char *name, int offset, int size, int high, int low, int voff,
	int nx, int ny, int nz, int interleave,
	char type, ar9300_eeprom_t *mptr, int mcount, int all)
{
	int im;
	int lc, nc;
	char buffer[MPRINTBUFFER],fullname[MPRINTBUFFER];
	int it, iuse;
	int different;
	int length;
	int ix, iy, iz;

	length=nx*ny*nz;

	for(it=0; it<length; it++)
	{
#ifdef WRONG
		iy=it%(nx*ny);
		iz=it/(nx*ny);
		ix=iy%nx;
		iy=iy/nx;
#else
		iz=it%nz;
		iy=it/nz;
		ix=iy/ny;
		iy=iy%ny;
#endif
		if(nz>1)
		{
			SformatOutput(fullname,MPRINTBUFFER-1,"%s[%d][%d][%d]",name,ix,iy,iz);
		}
		else if(ny>1)
		{
			SformatOutput(fullname,MPRINTBUFFER-1,"%s[%d][%d]",name,ix,iy);
		}
		else if(nx>1)
		{
			SformatOutput(fullname,MPRINTBUFFER-1,"%s[%d]",name,ix);
		}
		else
		{
			SformatOutput(fullname,MPRINTBUFFER-1,"%s",name);
		}
		fullname[MPRINTBUFFER-1]=0;
		lc=0;
		nc=SformatOutput(&buffer[lc],MPRINTBUFFER-lc-1,"|ecv|%s|%d|%d|%d|%d|%d|%d|%d|%d|%c|",
			fullname,it,offset+it*interleave,size,high,low,nx,ny,nz,type);
		if(nc>0)
		{
			lc+=nc;
		}
		//
		// put value from mptr[0]
		//
		nc=Ar9300EepromPrintIt(((unsigned char *)&mptr[0])+offset+it*interleave*size, type, size, 1, high, low, voff, &buffer[lc], MPRINTBUFFER-lc-1);
		if(nc>0)
		{
			lc+=nc;
		}
		nc=SformatOutput(&buffer[lc],MPRINTBUFFER-lc-1,"|");
		if(nc>0)
		{
			lc+=nc;
		}
		//
		// loop over subsequent iterations
		// add value only if different than previous value
		//
		different=0;
		for(im=1; im<mcount; im++)
		{
			if(memcmp(((unsigned char *)&mptr[im-1])+offset+it*interleave*size,((unsigned char *)&mptr[im])+offset+it*interleave*size,size)!=0)
			{
				nc=Ar9300EepromPrintIt(((unsigned char *)&mptr[im])+offset+it*interleave*size, type, size, 1, high, low, voff, &buffer[lc], MPRINTBUFFER-lc-1);
				if(nc>0)
				{
					lc+=nc;
				}
				different++;
			}
			else
			{
				nc=SformatOutput(&buffer[lc], MPRINTBUFFER-lc-1,".");
				if(nc>0)
				{
					lc+=nc;
				}
			}
			nc=SformatOutput(&buffer[lc],MPRINTBUFFER-lc-1,"|");
			if(nc>0)
			{
				lc+=nc;
			}
		}
		//
		// fill in up to the maximum number of blocks
		//
		for( ; im<MBLOCK; im++)
		{
			nc=SformatOutput(&buffer[lc],MPRINTBUFFER-lc-1,"|");
			if(nc>0)
			{
				lc+=nc;
			}
		}
		//
		// print it
		//
		if(different>0 || all)
		{
			(*print)("%s",buffer);
		}
	}
}


void Ar9300EepromDifferenceAnalyze(void (*print)(char *format, ...), ar9300_eeprom_t *mptr, int mcount, int all)
{
	int msize;
	int offset;
	int length;
	int im;
	int lc, nc;
	char buffer[MPRINTBUFFER];
	int it, nt;
	int io;
    //
	// make header
	//
	lc=SformatOutput(buffer,MPRINTBUFFER-1,"|ecv|name|index|offset|size|high|low|nx|ny|nz|type|");
	//
	// fill in up to the maximum number of blocks
	//
	for(im=0; im<MBLOCK; im++)
	{
		nc=SformatOutput(&buffer[lc],MPRINTBUFFER-lc-1,"b%d|",im);
		if(nc>0)
		{
			lc+=nc;
		}
	}
	(*print)("%s",buffer);

	offset=0;
	nt=sizeof(_Ar9300EepromList)/sizeof(_Ar9300EepromList[0]);
	for(it=0; it<nt; it++)
	{
		//
		// first we do any bytes that are not associated with a field name
		//
		for(io=offset; io<_Ar9300EepromList[it].offset; io++)
		{
            Ar9300EepromPrintEntry(print, "unknown", io, 1, -1, -1, 0, 1, 1, 1, 1, 'x', mptr, mcount, all);
		}
		//
		// do the field
		//
        Ar9300EepromPrintEntry(print,
			_Ar9300EepromList[it].name, _Ar9300EepromList[it].offset, _Ar9300EepromList[it].size, _Ar9300EepromList[it].high, _Ar9300EepromList[it].low, _Ar9300EepromList[it].voff,
			_Ar9300EepromList[it].nx, _Ar9300EepromList[it].ny, _Ar9300EepromList[it].nz, _Ar9300EepromList[it].interleave,
			_Ar9300EepromList[it].type, 
			mptr, mcount, all);
        if(_Ar9300EepromList[it].interleave==1 || (it<nt-1 && _Ar9300EepromList[it].interleave!=_Ar9300EepromList[it+1].interleave))
		{
			offset=_Ar9300EepromList[it].offset+
				(_Ar9300EepromList[it].size*_Ar9300EepromList[it].nx*_Ar9300EepromList[it].ny*_Ar9300EepromList[it].nz*_Ar9300EepromList[it].interleave)-
				(_Ar9300EepromList[it].interleave-1); 
		}
		else
		{
			offset=_Ar9300EepromList[it].offset+_Ar9300EepromList[it].size; 
		}
	}
	//
	// do any trailing bytes not associated with a field name
	//
	for(io=offset; io<sizeof(ar9300_eeprom_t); io++)
	{
        Ar9300EepromPrintEntry(print, "unknown", io, 1, -1, -1, 0, 1, 1, 1, 1, 'x', mptr, mcount, all);
	}
}
