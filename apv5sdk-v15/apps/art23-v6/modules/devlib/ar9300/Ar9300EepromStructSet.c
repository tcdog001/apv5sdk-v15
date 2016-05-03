#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#ifdef __APPLE__
#include "opt_ah.h"
#include "osdep.h"
#endif

#include "wlantype.h"
#include "mEepStruct9300.h"
//#include "default9300.h"
#include "Sticky.h"

//
// hal header files
//
#include "ah.h"
#include "ah_internal.h"
#include "ar9300.h"
#include "ar9300eep.h"

#include "Ar9300EepromStructSet.h"
#include "Field.h"
#include "ParameterSelect.h"
#include "Card.h"
#include "Ar9300PcieConfig.h"

extern struct ath_hal *AH;

ar9300_eeprom_t *Ar9300EepromStructGet(void)
{
    struct ath_hal_9300 *ahp = AH9300(AH);
    return & (ahp->ah_eeprom);
}

int Ar9300EepromWriteEnableGpioSet(int line)
{
	ar9300_eeprom_t *ep;
	ep=Ar9300EepromStructGet();
	if(ep!=0)
	{
		ep->baseEepHeader.eepromWriteEnableGpio=line;
		return VALUE_OK;
	}
	return ERR_VALUE_BAD;
}

int Ar9300EepromWriteEnableGpioGet()
{
	return ar9300EepromWriteEnableGpioGet(AH);
}

int Ar9300RxBandSelectGpioSet(int line)
{
	ar9300_eeprom_t *ep;
	ep=Ar9300EepromStructGet();
	if(ep!=0)
	{
		ep->baseEepHeader.rxBandSelectGpio=line;
		return VALUE_OK;
	}
//	sprintf(tValue, "Ar9300EepromStructGet failed!");
	return ERR_VALUE_BAD;
}

int Ar9300RxBandSelectGpioGet()
{
	return ar9300RxBandSelectGpioGet(AH);
}

int Ar9300WlanLedGpioSet(int line)
{
	ar9300_eeprom_t *ep;
	ep=Ar9300EepromStructGet();
	if(ep!=0)
	{
		ep->baseEepHeader.wlanLedGpio=line;
		return VALUE_OK;
	}
//	sprintf(tValue, "Ar9300EepromStructGet failed!");
	return ERR_VALUE_BAD;
}

int Ar9300WlanLedGpioGet()
{
	return ar9300WlanLedGpioGet(AH);
}

int Ar9300WlanDisableGpioSet(int line)
{
	ar9300_eeprom_t *ep;
	ep=Ar9300EepromStructGet();
	if(ep!=0)
	{
		ep->baseEepHeader.wlanDisableGpio=line;
		return VALUE_OK;
	}
//	sprintf(tValue, "Ar9300EepromStructGet failed!");
	return ERR_VALUE_BAD;
}

int Ar9300WlanDisableGpioGet()
{
	return ar9300WlanDisableGpioGet(AH);
}


#ifdef HALUNUSED
OSPREY_EEPROM *Ar9300EepromStructInit(int defaultIndex) 
{
    //Ar9300EepromStructGet()=default9300;;
	memcpy(Ar9300EepromStructGet(),&default9300,Ar9300EepromStructSize());
    return(Ar9300EepromStructGet());
}
#endif

A_INT32 Ar9300ReconfigDriveStrengthApply(int value) {

        //clear sticky writes?
    while(StickyFieldClear("ch0_BIAS1.pwd_ic25adc")==0);
    while(StickyFieldClear("ch0_BIAS1.pwd_ic25fir")==0);
    while(StickyFieldClear("ch0_BIAS1.pwd_ic25dac")==0);
    while(StickyFieldClear("ch0_BIAS1.pwd_ic25bb")==0);
    while(StickyFieldClear("ch0_BIAS1.pwd_ic25v2iI")==0);
    while(StickyFieldClear("ch0_BIAS1.pwd_ic25v2iQ")==0);

    while(StickyFieldClear("ch0_BIAS2.pwd_ic25pllgm")==0);
    while(StickyFieldClear("ch0_BIAS2.pwd_ic25pllcp")==0);
    while(StickyFieldClear("ch0_BIAS2.pwd_ic25pllcp2")==0);
    while(StickyFieldClear("ch0_BIAS2.pwd_ic25pllreg")==0);
    while(StickyFieldClear("ch0_BIAS2.pwd_ic25synth")==0);
    while(StickyFieldClear("ch0_BIAS2.pwd_ic25rxrf")==0);
    while(StickyFieldClear("ch0_BIAS2.pwd_ic25txrf")==0);
    while(StickyFieldClear("ch0_BIAS2.pwd_ic25xtal")==0);
    while(StickyFieldClear("ch0_BIAS2.pwd_ic25xtalreg")==0);

    while(StickyFieldClear("ch0_BIAS4.pwd_ic25spareA")==0);
    while(StickyFieldClear("ch0_BIAS4.pwd_ic25spareB")==0);
    while(StickyFieldClear("ch0_BIAS4.pwd_ic25xpabias")==0);


    if(value) {
        StickyFieldAdd("ch0_BIAS1.pwd_ic25adc", 0x5);
        StickyFieldAdd("ch0_BIAS1.pwd_ic25fir",  0x5);
        StickyFieldAdd("ch0_BIAS1.pwd_ic25dac",  0x5);
        StickyFieldAdd("ch0_BIAS1.pwd_ic25bb",  0x5);
        StickyFieldAdd("ch0_BIAS1.pwd_ic25v2iI",  0x5);
        StickyFieldAdd("ch0_BIAS1.pwd_ic25v2iQ",  0x5);

        StickyFieldAdd("ch0_BIAS2.pwd_ic25pllgm",  0x5);
        StickyFieldAdd("ch0_BIAS2.pwd_ic25pllcp",  0x5);
        StickyFieldAdd("ch0_BIAS2.pwd_ic25pllcp2",  0x5);
        StickyFieldAdd("ch0_BIAS2.pwd_ic25pllreg",  0x5);
        StickyFieldAdd("ch0_BIAS2.pwd_ic25synth",  0x5);
        StickyFieldAdd("ch0_BIAS2.pwd_ic25rxrf",  0x5);
        StickyFieldAdd("ch0_BIAS2.pwd_ic25txrf",  0x5);
        StickyFieldAdd("ch0_BIAS2.pwd_ic25xtal",  0x5);
        StickyFieldAdd("ch0_BIAS2.pwd_ic25xtalreg",  0x5);

        StickyFieldAdd("ch0_BIAS4.pwd_ic25spareA",  0x5);
        StickyFieldAdd("ch0_BIAS4.pwd_ic25spareB",  0x5);
        StickyFieldAdd("ch0_BIAS4.pwd_ic25xpabias",  0x5);
    }

    return 0;
}

int checkFreq(int value, int iBand)
{
	int i;
	if (iBand==band_BG) {
		if (value!=0 && (value >MAX_G_BAND_FREQ || value<MIN_G_BAND_FREQ)) {
//			sprintf(tValue,"%d is not BG band freq.", value);
			return ERR_VALUE_BAD;
		}
	} else {
		if (value!=0 && (value >MAX_A_BAND_FREQ || value<MIN_A_BAND_FREQ)) {
//			sprintf(tValue,"%d is not A band freq.", value);
			return ERR_VALUE_BAD;
		}
	}
	return VALUE_OK;
}

A_INT32 Ar9300eepromVersion(int value)
{
	Ar9300EepromStructGet()->eepromVersion = (u_int8_t) value;
    return 0;
}

A_INT32 Ar9300templateVersion(int value)
{
	Ar9300EepromStructGet()->templateVersion = (u_int8_t) value;
    return 0;
}

A_INT32 Ar9300FutureSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	if (iBand==-1) {
		for (i=ix; i<MAX_BASE_EXTENSION_FUTURE; i++) {
			if (iv>=num)
				break;
			Ar9300EepromStructGet()->base_ext1.future[i] = value[iv++];
		}
	} else {
		for (i=ix; i<MAX_MODAL_FUTURE; i++) {
			if (iv>=num)
				break;
			if (iBand==band_BG) 
				Ar9300EepromStructGet()->modalHeader2G.futureModal[i] = value[iv++];
			else if (iBand==band_A) 
				Ar9300EepromStructGet()->modalHeader5G.futureModal[i] = value[iv++];
		}
	}
	return 0;
}

A_INT32 Ar9300AntDivCtrlSet(int value)
{
	Ar9300EepromStructGet()->base_ext1.ant_div_control = (u_int8_t)value;
    return 0;
}

/*
 *Function Name:Ar9300AntCtrlCommonSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set AntCtrlCommon flag in field of eeprom struct (u_int32_t)
 *Returns: zero
 */
A_INT32 Ar9300AntCtrlCommonSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.antCtrlCommon = (u_int32_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.antCtrlCommon = (u_int32_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300AntCtrlCommon2Set
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set AntCtrlCommon2 flag in field of eeprom struct (u_int32_t)
 *Returns: zero
 */
A_INT32 Ar9300AntCtrlCommon2Set(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.antCtrlCommon2 = (u_int32_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.antCtrlCommon2 = (u_int32_t)value;
	}
    return 0;
}

/*
 *Function Name:Ar9300TempSlopeSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set TempSlope flag in field of eeprom struct (int8_t)
 *Returns: zero
 */
A_INT32 Ar9300TempSlopeSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.tempSlope = (int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.tempSlope  = (int8_t)value;
	}
    return 0;
}
A_INT32 Ar9300TempSlopeLowSet(int value)
{
	Ar9300EepromStructGet()->base_ext2.tempSlopeLow= (int8_t)value;
    return 0;
}

A_INT32 Ar9300TempSlopeHighSet(int value)
{
	Ar9300EepromStructGet()->base_ext2.tempSlopeHigh= (int8_t)value;
    return 0;
}

/*
 *Function Name:Ar9300VoltSlopeSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set voltSlope flag in field of eeprom struct (int8_t)
 *Returns: zero
 */
A_INT32 Ar9300VoltSlopeSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.voltSlope = (int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.voltSlope = (int8_t)value;
	}
    return 0;
}

/*
 *Function Name:Ar9300reconfigDriveStrengthSet
 *
 *Parameters: value
 *
 *Description: set reconfigDriveStrength flag in miscConfiguration 
 *             field of eeprom struct (bit 0)
 *
 *Returns: zero
 *
 */
A_INT32 Ar9300ReconfigDriveStrengthSet(int value)
{
    if (value) {
        Ar9300EepromStructGet()->baseEepHeader.miscConfiguration |= 0x01;
    }
    else {
        Ar9300EepromStructGet()->baseEepHeader.miscConfiguration &= 0xfe;
    }
    Ar9300ReconfigDriveStrengthApply(value);
    return 0;
}

A_INT32 Ar9300ThermometerGet()
{
	return ar9300ThermometerGet(AH);
}

A_INT32 Ar9300ThermometerSet(int value)
{
	unsigned long misc;
	value++;
    misc=Ar9300EepromStructGet()->baseEepHeader.miscConfiguration;
	misc&=(~0x6);
	misc|=((value&0x3)<<1);
    Ar9300EepromStructGet()->baseEepHeader.miscConfiguration=misc;
    return 0;
}

A_INT32 Ar9300ChainMaskReduceGet()
{
	return ar9300EepromGet(AH9300(AH),EEP_CHAIN_MASK_REDUCE);
}

A_INT32 Ar9300ChainMaskReduceSet(int value)
{
	unsigned int misc;
    misc=Ar9300EepromStructGet()->baseEepHeader.miscConfiguration;
	misc&=(~0x8);
	misc|=((value&0x1)<<3);
    Ar9300EepromStructGet()->baseEepHeader.miscConfiguration=misc;
    return 0;
}
// bit 4 - enable quick drop
A_INT32 Ar9300ReconfigQuickDropSet(int value)		
{
    if (value) {
        Ar9300EepromStructGet()->baseEepHeader.miscConfiguration |= 0x10;
    }
    else {
        Ar9300EepromStructGet()->baseEepHeader.miscConfiguration &= (~0x10);
    }
    return 0;
}


/*
 *Function Name:Ar9300TxGainSet
 *Parameters: value
 *Description: set TxGain flag in txrxgain field of eeprom struct's upper 4bits
 *Returns: zero
 */
A_INT32 Ar9300TxGainSet(int value)
{
	u_int8_t  value4;
	value4 = (u_int8_t)((value & 0x0f) << 4);
	Ar9300EepromStructGet()->baseEepHeader.txrxgain &= (0x0f);
	Ar9300EepromStructGet()->baseEepHeader.txrxgain |= value4;

	ar9300TxGainTableApply(AH);
	CardResetForce();
    return 0;
}
/*
 *Function Name:Ar9300RxGainSet
 *Parameters: value
 *Description: set RxGain flag in txrxgain field of eeprom struct's lower 4bits
 *Returns: zero
 */
A_INT32 Ar9300RxGainSet(int value)
{
	u_int8_t  value4;
	value4 = (u_int8_t)(value & 0x0f);
	Ar9300EepromStructGet()->baseEepHeader.txrxgain &= (0xf0);
	Ar9300EepromStructGet()->baseEepHeader.txrxgain |= value4;

	ar9300RxGainTableApply(AH);
	CardResetForce();

    return 0;
}


/*
 *Function Name:Ar9300EnableTempCompensationSet
 *
 *Parameters: value
 *
 *Description: set reconfigDriveStrength flag in featureEnable 
 *             field of eeprom struct (bit 0)
 *
 *Returns: zero
 *
 */
A_INT32 Ar9300EnableTempCompensationSet(int value)
{
    if (value) {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable |= 0x01;
    }
    else {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable &= 0xfe;
    }
    return 0;
}

/*
 *Function Name:Ar9300EnableVoltCompensationSet
 *
 *Parameters: value
 *
 *Description: set reconfigDriveStrength flag in featureEnable 
 *             field of eeprom struct (bit 1)
 *
 *Returns: zero
 *
 */
A_INT32 Ar9300EnableVoltCompensationSet(int value)
{
    if (value) {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable |= 0x02;
    }
    else {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable &= 0xfd;
    }
    return 0;
}

/*
 *Function Name:Ar9300MacAdressSet
 *
 *Parameters: mac -- pointer to input mac address. 
 *
 *Description: Saves mac address in the eeprom structure.
 *
 *Returns: zero.
 *
 */

A_INT32 Ar9300MacAdressSet(A_UINT8 *mac)
{
    A_INT16 i;
    for(i=0; i<6; i++)
        Ar9300EepromStructGet()->macAddr[i] = mac[i];

    return 0;
}
/*
 *Function Name:Ar9300CustomerDataSet
 *
 *Parameters: data -- Pointer to input array. 
 *            len -- length of the array. 
 *
 *Description: Saves input array in the customer data array of eeprom structure.
 *
 *Returns: -1 on error condition
 *          0 on success.
 */

A_INT32 Ar9300CustomerDataSet(A_UCHAR *data, A_INT32 len)
{
    A_INT16 i;

    if(len>OSPREY_CUSTOMER_DATA_SIZE) {
        len=OSPREY_CUSTOMER_DATA_SIZE;
    }

    for(i=0; i<len; i++)
        Ar9300EepromStructGet()->custData[i]=data[i];

    return 0;
}

/*
 *Function Name:Ar9300pwrTuningCapsParamsSet
 *Parameters: value0, value1
 *Description: Set TuningCapsParams values of field of eeprom struct 2 uint8
 *Returns: zero
 */
A_INT32 Ar9300pwrTuningCapsParamsSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	for (i=ix; i<2; i++) {
		if (iv>=num)
			break;
		Ar9300EepromStructGet()->baseEepHeader.params_for_tuning_caps[i] = (u_int8_t)value[iv++];
	}
	ar9300TuningCapsApply(AH);
	return 0;
}

/*
 *Function Name:Ar9300regDmnSet
 *Parameters: value
 *Description: set regDmn field of eeprom struct (u_int16_t *2) 
 *Returns: zero
 */
A_INT32 Ar9300regDmnSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	for (i=ix; i<2; i++) {
		if (iv>=num)
			break;
		Ar9300EepromStructGet()->baseEepHeader.regDmn[i] = (u_int16_t)value[iv++];
	}
    return 0;
}
/*
 *Function Name:Ar9300txMaskSet
 *Parameters: value
 *Description: set txrxMask field of eeprom struct (u_int8_t) 4bit tx (upper 4)
 *Returns: zero
 */
A_INT32 Ar9300txMaskSet(int value)
{
	u_int8_t  value4;
	value4 = (u_int8_t)(value << 4);
	Ar9300EepromStructGet()->baseEepHeader.txrxMask &= (0x0f);
	Ar9300EepromStructGet()->baseEepHeader.txrxMask += value4;
    return 0;
}
/*
 *Function Name:Ar9300rxMaskSet
 *Parameters: value
*Description: set txrxMask field of eeprom struct (u_int8_t) 4bit tx (lower 4)
 *Returns: zero
 */
A_INT32 Ar9300rxMaskSet(int value)
{
	u_int8_t  value4;
	value4 = (u_int8_t)(value & 0x0f);
	Ar9300EepromStructGet()->baseEepHeader.txrxMask &= (0xf0);
	Ar9300EepromStructGet()->baseEepHeader.txrxMask += value4;
    return 0;
}
/*
 *Function Name:Ar9300opFlagsSet
 *Parameters: value
 *Description: set opFlags field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300opFlagsSet(int value)
{
	Ar9300EepromStructGet()->baseEepHeader.opCapFlags.opFlags = (u_int8_t) value;
    return 0;
}
/*
 *Function Name:Ar9300eepMiscSet
 *Parameters: value
 *Description: set eepMisc field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300eepMiscSet(int value)
{
	Ar9300EepromStructGet()->baseEepHeader.opCapFlags.eepMisc = (u_int8_t) value;
    return 0;
}
/*
 *Function Name:Ar9300rfSilentSet
 *Parameters: value
 *Description: set rfSilent field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300rfSilentSet(int value)
{
	Ar9300EepromStructGet()->baseEepHeader.rfSilent = (u_int8_t) value;
    return 0;
}
A_INT32 Ar9300rfSilentB0Set(int value)
{
	if (value)
		Ar9300EepromStructGet()->baseEepHeader.rfSilent |= 0x1;
	else
		Ar9300EepromStructGet()->baseEepHeader.rfSilent &= (~0x1);
    return 0;
}
A_INT32 Ar9300rfSilentB1Set(int value)
{
	if (value)
		Ar9300EepromStructGet()->baseEepHeader.rfSilent |= 0x2;
	else
		Ar9300EepromStructGet()->baseEepHeader.rfSilent &= (~0x2);
    return 0;
}
A_INT32 Ar9300rfSilentGPIOSet(int value)
{
	//clear out the field before setting
	Ar9300EepromStructGet()->baseEepHeader.rfSilent &= (~0xfc);
	if (value)
		//set the field
		Ar9300EepromStructGet()->baseEepHeader.rfSilent |= (value << 2);
    return 0;
}

/*
 *Function Name:Ar9300deviceCapSet
 *Parameters: value
 *Description: set deviceCap field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300deviceCapSet(int value)
{
	Ar9300EepromStructGet()->baseEepHeader.deviceCap = (u_int8_t) value;
    return 0;
}/*
 *Function Name:Ar9300blueToothOptionsSet
 *Parameters: value
 *Description: set blueToothOptions field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300blueToothOptionsSet(int value)
{
	Ar9300EepromStructGet()->baseEepHeader.blueToothOptions = (u_int8_t) value;
    return 0;
}/*
 *Function Name:Ar9300deviceTypetSet
 *Parameters: value
 *Description: set deviceType field of eeprom struct (u_int8_t) (lower byte in EEP)
 *Returns: zero
 */
A_INT32 Ar9300deviceTypetSet(int value)
{
	Ar9300EepromStructGet()->baseEepHeader.deviceType = (u_int8_t) value;
    return 0;
}
/*
 *Function Name:Ar9300pwrTableOffsetSet
 *Parameters: value
 *Description: set pwrTableOffset field of eeprom struct (int8_t)
 *Returns: zero
 */
A_INT32 Ar9300pwrTableOffsetSet(int value)
{
	Ar9300EepromStructGet()->baseEepHeader.pwrTableOffset = (int8_t) value;
    return 0;
}

A_INT32 Ar9300EnableFeatureSet(int value)
{
    value = Ar9300EepromStructGet()->baseEepHeader.featureEnable=(unsigned char)value;
	return 0;
}

A_INT32 Ar9300MiscConfigurationSet(int value)
{
    value = Ar9300EepromStructGet()->baseEepHeader.miscConfiguration=(unsigned char)value;
	return 0;
}
/*
 *Function Name:Ar9300EnableFastClockSet
 *
 *Parameters: value
 *
 *Description: set Fast Clock flag in featureEnable 
 *             field of eeprom struct (bit 2)
 *
 *Returns: zero
 *
 */
A_INT32 Ar9300EnableFastClockSet(int value)
{
    if (value) {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable |= 0x04;
    }
    else {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable &= 0xfb;
    }
    return 0;
}

/*
 *Function Name:Ar9300EnableDoublingSet
 *Parameters: value
 *Description: set EnableDoubling flag in featureEnable 
 *             field of eeprom struct (bit 3)
 *Returns: zero
 */
A_INT32 Ar9300EnableDoublingSet(int value)
{
    if (value) {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable |= 0x08;
    }
    else {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable &= 0xf7;
    }
    return 0;
}
/*
 *Function Name:Ar9300EnableTuningCapsSet
 *Parameters: value
 *Description: set EnableTuningCaps flag in featureEnable 
 *             field of eeprom struct (bit 3)
 *Returns: zero
 */
A_INT32 Ar9300EnableTuningCapsSet(int value)
{
    if (value) {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable |= 0x40;
    }
    else {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable &= 0xbf;
    }
	ar9300TuningCapsApply(AH);
    return 0;
}
/*
 *Function Name:Ar9300InternalRegulatorSet
 *
 *Parameters: value
 *
 *Description: set internal regulator flag in featureEnable 
 *             field of eeprom struct (bit 4).
 *             Add an entry in PCIE config space
 *
 *Returns: zero
 *
 */
A_INT32 Ar9300InternalRegulatorSet(int value)
{
    if (value) {
	// internal regulator is ON. This default setting.
        Ar9300EepromStructGet()->baseEepHeader.featureEnable |= 0x10;  // set the bit
    } else {
	// Internal regulator is OFF. We should write 4 to 0x7048. This write is necessary for non-calibrated board.
        Ar9300EepromStructGet()->baseEepHeader.featureEnable &= 0xef; // clear the bit
    }
    ar9300InternalRegulatorApply(AH);
    return 0;
}
/*
 *Function Name:Ar9300PapdSet
 *
 *Parameters: value
 *
 *Description: set internal regulator flag in featureEnable 
 *             field of eeprom struct (bit 4).
 *             Add an entry in PCIE config space
 *
 *Returns: zero
 *
 */
A_INT32 Ar9300PapdSet(int value)
{
	if (value) {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable |= 0x20;  // set the papd bit
    } else {
        Ar9300EepromStructGet()->baseEepHeader.featureEnable &= 0xdf; // clear the papd bit
    }
    return 0;
}

A_INT32 Ar9300PapdRateMaskHt20Set(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.papdRateMaskHt20 = (u_int32_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.papdRateMaskHt20 = (u_int32_t)value;
	}
    return 0;
}

A_INT32 Ar9300PapdRateMaskHt40Set(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.papdRateMaskHt40 = (u_int32_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.papdRateMaskHt40 = (u_int32_t)value;
	}
    return 0;
}

A_INT32 Ar9300_SWREG_Set(int value)
{
	Ar9300EepromStructGet()->baseEepHeader.swreg =  (u_int32_t)value;

	/*
#set internalregulator=1;
#set SWREG_PROGRAM=0;
#set SWREG=32bit_hex;
#set SWREG_PROGRAM=1;

	int ngot;
	unsigned int address;
	int low, high;
	char regName[100];
	A_UINT32 mask, reg;

	sprintf(regName, "REG_CONTROL0.%s",tValue);		// tValue is the name set reg_Field_name = 
	ngot=FieldFind(regName,&address,&low,&high);
	if (ngot==1) {
		mask = MaskCreate(low, high);
		reg = Ar9300EepromStructGet()->baseEepHeader.swreg; 
		reg &= ~(mask);						// clear bits
		reg |= ((value<<low)&mask);			// set new value
		Ar9300EepromStructGet()->baseEepHeader.swreg = reg; 
	} else
		return ERR_VALUE_BAD;
*/
    return VALUE_OK;
}

A_INT32 Ar9300_SWREG_PROGRAM_Set(int value)
{
	int status=VALUE_OK;
	unsigned int address, swregAddr;
	int low, high;
	int ngot;
	char regName[100];
	A_UINT32 reg;

	if (value==1) {
		ngot=FieldFind("REG_CONTROL0.SWREG_pwd",&swregAddr,&low,&high);
		if (ngot<1)
			return ERR_VALUE_BAD;

		sprintf(regName, "REG_CONTROL1.SWREG_PROGRAM"); 
		ngot=FieldFind(regName,&address,&low,&high);
		if (ngot==1) {
			// disable internal regulator program write.
			MyFieldWrite(address,low,high,0);	
			// set swreg from eep structure to HW
			reg = Ar9300EepromStructGet()->baseEepHeader.swreg; 
			MyRegisterWrite(swregAddr, reg);
			// apply internal regulator program write.
			MyFieldWrite(address,low,high,1);	
		} else 
			return ERR_VALUE_BAD;
	}
    return 0;
}

int setFREQ2FBIN(int freq, int iBand)
{
	int bin;
	if (freq==0)
		return 0;
	if (iBand==band_BG)
		bin = FREQ2FBIN(freq,1);
	else
		bin = FREQ2FBIN(freq,0);
	return bin;
}


A_INT32 Ar9300antCtrlChainSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	for (i=ix; i<OSPREY_MAX_CHAINS; i++) {
		if (iv>=num)
			break;
		if (iBand==band_BG) {
			Ar9300EepromStructGet()->modalHeader2G.antCtrlChain[i] = (u_int16_t)value[iv++];
		} else {
			Ar9300EepromStructGet()->modalHeader5G.antCtrlChain[i] = (u_int16_t)value[iv++];
		}
	}
    return 0;
}
/*
 *Function Name:Ar9300xatten1DBSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set xatten1DB flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300xatten1DBSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	for (i=ix; i<OSPREY_MAX_CHAINS; i++) {
		if (iv>=num)
			break;
		if (iBand==band_BG) {
			Ar9300EepromStructGet()->modalHeader2G.xatten1DB[i] = (u_int8_t)value[iv++];
		} else {
			Ar9300EepromStructGet()->modalHeader5G.xatten1DB[i] = (u_int8_t)value[iv++];
		}
	}
	CardResetForce();
    return 0;
}
/*
 *Function Name:Ar9300xatten1DBLowSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set xatten1DBLow flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300xatten1DBLowSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	if (iBand==band_BG)
		return 0;
	for (i=ix; i<OSPREY_MAX_CHAINS; i++) {
		if (iv>=num)
			break;
		Ar9300EepromStructGet()->base_ext2.xatten1DBLow[i] = (u_int8_t)value[iv++];
	}
	CardResetForce();
    return 0;
}
/*
 *Function Name:Ar9300xatten1DBHighSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set xatten1DBHigh flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300xatten1DBHighSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	if (iBand==band_BG)
		return 0;
	for (i=ix; i<OSPREY_MAX_CHAINS; i++) {
		if (iv>=num)
			break;
		Ar9300EepromStructGet()->base_ext2.xatten1DBHigh[i] = (u_int8_t)value[iv++];
	}
	CardResetForce();
    return 0;
}

/*
 *Function Name:Ar9300xatten1MarginSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set xatten1Margin flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300xatten1MarginSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	for (i=ix; i<OSPREY_MAX_CHAINS; i++) {
		if (iv>=num)
			break;
		if (iBand==band_BG) {
			Ar9300EepromStructGet()->modalHeader2G.xatten1Margin[i] = (u_int8_t)value[iv++];
		} else {
			Ar9300EepromStructGet()->modalHeader5G.xatten1Margin[i] = (u_int8_t)value[iv++];
		}
	}
	CardResetForce();
    return 0;
}
/*
 *Function Name:Ar9300xatten1MarginLowSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set xatten1MarginLow flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300xatten1MarginLowSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	if (iBand==band_BG)
		return 0;
	for (i=ix; i<OSPREY_MAX_CHAINS; i++) {
		if (iv>=num)
			break;
		Ar9300EepromStructGet()->base_ext2.xatten1MarginLow[i] = (u_int8_t)value[iv++];
	}
	CardResetForce();
    return 0;
}
/*
 *Function Name:Ar9300xatten1MarginHighSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set xatten1MarginHigh flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300xatten1MarginHighSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	if (iBand==band_BG)
		return 0;
	for (i=ix; i<OSPREY_MAX_CHAINS; i++) {
		if (iv>=num)
			break;
		Ar9300EepromStructGet()->base_ext2.xatten1MarginHigh[i] = (u_int8_t)value[iv++];
	}
	CardResetForce();
    return 0;
}

/*
 *Function Name:Ar9300spurChansSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set spurChans flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300spurChansSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	char buff[1024];
	A_UINT8 bin;
	int i, iv=0;
	for (i=ix; i<OSPREY_EEPROM_MODAL_SPURS; i++) {
		if (iv>=num)
			break;
		bin = setFREQ2FBIN(value[iv++], iBand);
		if (iBand==band_BG)
			Ar9300EepromStructGet()->modalHeader2G.spurChans[i] = bin;
		else
			Ar9300EepromStructGet()->modalHeader5G.spurChans[i] = bin;
	}
	return VALUE_OK;
}
/*
 *Function Name:Ar9300noiseFloorThreshChSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set noiseFloorThreshCh flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300noiseFloorThreshChSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	for (i=ix; i<OSPREY_MAX_CHAINS; i++) {
		if (iv>=num)
			break;
		if (iBand==band_BG) {
			Ar9300EepromStructGet()->modalHeader2G.noiseFloorThreshCh[i] = (int8_t)value[iv++];
		} else {
			Ar9300EepromStructGet()->modalHeader5G.noiseFloorThreshCh[i] = (int8_t)value[iv++];
		}
	}
    return 0;
}

A_INT32 Ar9300ReservedSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, iv=0;
	for (i=ix; i<MAX_MODAL_RESERVED; i++) {
		if (iv>=num)
			break;
		if (iBand==band_BG) {
			Ar9300EepromStructGet()->modalHeader2G.reserved[i] = (int8_t)value[iv++];
		} else {
			Ar9300EepromStructGet()->modalHeader5G.reserved[i] = (int8_t)value[iv++];
		}
	}
    return 0;
}

A_INT32 Ar9300QuickDropSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.quickDrop = value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.quickDrop = value;
	}
    return 0;
}
A_INT32 Ar9300QuickDropLowSet(int value)
{
	Ar9300EepromStructGet()->base_ext1.quickDropLow = value;
    return 0;
}
A_INT32 Ar9300QuickDropHighSet(int value)
{
	Ar9300EepromStructGet()->base_ext1.quickDropHigh = value;
    return 0;
}

/*
 *Function Name:Ar9300xpaBiasLvlSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set xpaBiasLvl flag in field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300xpaBiasLvlSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.xpaBiasLvl = (u_int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.xpaBiasLvl = (u_int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300txFrameToDataStartSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set txFrameToDataStart flag in field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300txFrameToDataStartSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.txFrameToDataStart = (u_int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.txFrameToDataStart = (u_int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300txFrameToPaOnSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set txFrameToPaOn flag in field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300txFrameToPaOnSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.txFrameToPaOn = (u_int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.txFrameToPaOn = (u_int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300txClipSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set txClip flag in field of eeprom struct (u_int8_t) (4 bits tx_clip)
 *Returns: zero
 */
A_INT32 Ar9300txClipSet(int value, int iBand)
{
	u_int8_t  value4;
	value4 = (u_int8_t)(value & 0x0f);
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.txClip &= (0xf0);				// which 4 bits are for tx_clip???
		Ar9300EepromStructGet()->modalHeader2G.txClip += value4;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.txClip &= (0xf0);
		Ar9300EepromStructGet()->modalHeader5G.txClip += value4;
	}
    return 0;
}
/*
 *Function Name:Ar9300dac_scale_cckSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set dac_scale_cck flag in field of eeprom struct (u_int8_t) (4 bits tx_clip)
 *Returns: zero
 */
A_INT32 Ar9300dac_scale_cckSet(int value, int iBand)
{
	u_int8_t  value4;
	value4 = (u_int8_t)(value << 4);
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.txClip &= (0x0f);				// which 4 bits are for tx_clip???
		Ar9300EepromStructGet()->modalHeader2G.txClip += value4;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.txClip &= (0x0f);
		Ar9300EepromStructGet()->modalHeader5G.txClip += value4;
	}
    return 0;
}
/*
 *Function Name:Ar9300antennaGainSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set antennaGain flag in field of eeprom struct (int8_t)
 *Returns: zero
 */
A_INT32 Ar9300antennaGainSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.antennaGain = (int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.antennaGain = (int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300adcDesiredSizeSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set adcDesiredSize flag in field of eeprom struct (int8_t)
 *Returns: zero
 */
A_INT32 Ar9300adcDesiredSizeSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.adcDesiredSize = (int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.adcDesiredSize = (int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300switchSettlingSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set switchSettling flag in field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300switchSettlingSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.switchSettling = (u_int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.switchSettling = (u_int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300txEndToXpaOffSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set txEndToXpaOff flag in field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300txEndToXpaOffSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.txEndToXpaOff = (u_int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.txEndToXpaOff = (u_int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300txEndToRxOnSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set txEndToRxOn flag in field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300txEndToRxOnSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.txEndToRxOn = (u_int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.txEndToRxOn = (u_int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300txFrameToXpaOnSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set txFrameToXpaOn flag in field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300txFrameToXpaOnSet(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.txFrameToXpaOn = (u_int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.txFrameToXpaOn = (u_int8_t)value;
	}
    return 0;
}
/*
 *Function Name:Ar9300thresh62Set
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA
 *Description: set thresh62 flag in field of eeprom struct (u_int8_t)
 *Returns: zero
 */
A_INT32 Ar9300thresh62Set(int value, int iBand)
{
	if (iBand==band_BG) {
		Ar9300EepromStructGet()->modalHeader2G.thresh62 = (u_int8_t)value;
	} else {
		Ar9300EepromStructGet()->modalHeader5G.thresh62 = (u_int8_t)value;
	}
    return 0;
}

/*
 *Function Name:Ar9300calFreqPierSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set calFreqPier flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */

A_INT32 Ar9300calFreqPierSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	char buff[1024];
	A_UINT8 bin;
	int i, maxnum, iv=0;
	if (iBand==band_BG)
		maxnum = NUM_2G_CAL_PIERS;
	else
		maxnum = NUM_5G_CAL_PIERS;
	for (i=ix; i<maxnum; i++) {
		if (iv>=num)
			break;
		bin = setFREQ2FBIN(value[iv++], iBand);
		if (iBand==band_BG)
			Ar9300EepromStructGet()->calFreqPier2G[i] = bin;
		else
			Ar9300EepromStructGet()->calFreqPier5G[i] = bin;
	}
	return VALUE_OK;
}

/*
 *Function Name:Ar9300calPierDataRefPowerSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set calPierData.refPower flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataRefPowerSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iMaxChain, iMaxPier, iv=0;
	iMaxChain = OSPREY_MAX_CHAINS;
	if (iBand==band_BG) 
		iMaxPier = NUM_2G_CAL_PIERS;
	else 
		iMaxPier = NUM_5G_CAL_PIERS;
	for (i=ix; i<iMaxChain; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iMaxPier; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calPierData2G[i][j].refPower = (int8_t)value[iv++];
			} else {
				Ar9300EepromStructGet()->calPierData5G[i][j].refPower = (int8_t)value[iv++];
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}
/*
 *Function Name:Ar9300calPierDataVoltMeasSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set calPierData.refPower flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataVoltMeasSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iMaxChain, iMaxPier, iv=0;
	iMaxChain = OSPREY_MAX_CHAINS;
	if (iBand==band_BG) 
		iMaxPier = NUM_2G_CAL_PIERS;
	else 
		iMaxPier = NUM_5G_CAL_PIERS;
	for (i=ix; i<iMaxChain; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iMaxPier; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calPierData2G[i][j].voltMeas = (int8_t)value[iv++];
			} else {
				Ar9300EepromStructGet()->calPierData5G[i][j].voltMeas = (int8_t)value[iv++];
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}
/*
 *Function Name:Ar9300calPierDataTempMeasSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set calPierData.refPower flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataTempMeasSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iMaxChain, iMaxPier, iv=0;
	iMaxChain = OSPREY_MAX_CHAINS;
	if (iBand==band_BG) 
		iMaxPier = NUM_2G_CAL_PIERS;
	else 
		iMaxPier = NUM_5G_CAL_PIERS;
	for (i=ix; i<iMaxChain; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iMaxPier; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calPierData2G[i][j].tempMeas = (int8_t)value[iv++];
			} else {
				Ar9300EepromStructGet()->calPierData5G[i][j].tempMeas = (int8_t)value[iv++];
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}
/*
 *Function Name:Ar9300calPierDataRxNoisefloorCalSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set calPierData.refPower flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataRxNoisefloorCalSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iMaxChain, iMaxPier, iv=0;
	iMaxChain = OSPREY_MAX_CHAINS;
	if (iBand==band_BG) 
		iMaxPier = NUM_2G_CAL_PIERS;
	else 
		iMaxPier = NUM_5G_CAL_PIERS;
	for (i=ix; i<iMaxChain; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iMaxPier; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calPierData2G[i][j].rxNoisefloorCal = (int8_t)value[iv++];
			} else {
				Ar9300EepromStructGet()->calPierData5G[i][j].rxNoisefloorCal = (int8_t)value[iv++];
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}
/*
 *Function Name:Ar9300calPierDataRxNoisefloorPowerSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set calPierData.refPower flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataRxNoisefloorPowerSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iMaxChain, iMaxPier, iv=0;
	iMaxChain = OSPREY_MAX_CHAINS;
	if (iBand==band_BG) 
		iMaxPier = NUM_2G_CAL_PIERS;
	else 
		iMaxPier = NUM_5G_CAL_PIERS;
	for (i=ix; i<iMaxChain; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iMaxPier; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calPierData2G[i][j].rxNoisefloorPower = (int8_t)value[iv++];
			} else {
				Ar9300EepromStructGet()->calPierData5G[i][j].rxNoisefloorPower = (int8_t)value[iv++];
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}
/*
 *Function Name:Ar9300calPierDataRxTempMeaSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		iChain: 0,1,2
 *Description: set calPierData.refPower flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataRxTempMeaSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iMaxChain, iMaxPier, iv=0;
	iMaxChain = OSPREY_MAX_CHAINS;
	if (iBand==band_BG) 
		iMaxPier = NUM_2G_CAL_PIERS;
	else 
		iMaxPier = NUM_5G_CAL_PIERS;
	for (i=ix; i<iMaxChain; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iMaxPier; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calPierData2G[i][j].rxTempMeas = (int8_t)value[iv++];
			} else {
				Ar9300EepromStructGet()->calPierData5G[i][j].rxTempMeas = (int8_t)value[iv++];
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}

A_INT32 Ar9300calFreqTGTcckSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	A_UINT8 bin;
	int i, iv=0;
	for (i=ix; i<OSPREY_NUM_2G_CCK_TARGET_POWERS; i++) {
		if (iv>=num)
			break;
		bin = setFREQ2FBIN(value[iv++], iBand);
		Ar9300EepromStructGet()->calTarget_freqbin_Cck[i] = bin;
	}
	return VALUE_OK;
}

A_INT32 Ar9300calFreqTGTLegacyOFDMSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	char buff[1024];
	A_UINT8 bin;
	int i, maxnum, iv=0;
	if (iBand==band_BG)
		maxnum = OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		maxnum = OSPREY_NUM_5G_20_TARGET_POWERS;
	for (i=ix; i<maxnum; i++) {
		if (iv>=num)
			break;
		bin = setFREQ2FBIN(value[iv++], iBand);
		if (iBand==band_BG)
			Ar9300EepromStructGet()->calTarget_freqbin_2G[i] = bin;
		else
			Ar9300EepromStructGet()->calTarget_freqbin_5G[i] = bin;
	}
	return VALUE_OK;
}
A_INT32 Ar9300calFreqTGTHT20Set(int *value, int ix, int iy, int iz, int num, int iBand)
{
	A_UINT8 bin;
	int i, maxnum, iv=0;
	if (iBand==band_BG)
		maxnum = OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		maxnum = OSPREY_NUM_5G_20_TARGET_POWERS;
	for (i=ix; i<maxnum; i++) {
		if (iv>=num)
			break;
		bin = setFREQ2FBIN(value[iv++], iBand);
		if (iBand==band_BG)
			Ar9300EepromStructGet()->calTarget_freqbin_2GHT20[i] = bin;
		else
			Ar9300EepromStructGet()->calTarget_freqbin_5GHT20[i] = bin;
	}
	return VALUE_OK;
}
A_INT32 Ar9300calFreqTGTHT40Set(int *value, int ix, int iy, int iz, int num, int iBand)
{
	A_UINT8 bin;
	int i, maxnum, iv=0;
	if (iBand==band_BG)
		maxnum = OSPREY_NUM_2G_40_TARGET_POWERS;
	else
		maxnum = OSPREY_NUM_5G_40_TARGET_POWERS;
	for (i=ix; i<maxnum; i++) {
		if (iv>=num)
			break;
		bin = setFREQ2FBIN(value[iv++], iBand);
		if (iBand==band_BG)
			Ar9300EepromStructGet()->calTarget_freqbin_2GHT40[i] = bin;
		else
			Ar9300EepromStructGet()->calTarget_freqbin_5GHT40[i] = bin;
	}
	return VALUE_OK;
}
/*
 *Function Name:Ar9300calTGTPwrLegacyOFDMSet
 *Parameters: value
 *			  iBand: 0-bandBG, 1-bandA,		
 *Description: set calTargetPowerxx flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calTGTPwrLegacyOFDMSet(double *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iv=0;
	int iMaxPier;
	if (iBand==band_BG) 
		iMaxPier = OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		iMaxPier = OSPREY_NUM_5G_20_TARGET_POWERS;
	for (i=ix; i<iMaxPier; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<4; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calTargetPower2G[i].tPow2x[j] = (u_int8_t)(value[iv++]*2);	// eep value is double of entered value
			} else {
				Ar9300EepromStructGet()->calTargetPower5G[i].tPow2x[j] = (u_int8_t)(value[iv++]*2);
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}

A_INT32 Ar9300calTGTPwrCCKSet(double *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iv=0;
	for (i=ix; i<OSPREY_NUM_2G_CCK_TARGET_POWERS; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<4; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calTargetPowerCck[i].tPow2x[j] = (u_int8_t)(value[iv++]*2);	// eep value is double of entered value
			} else 
				return ERR_VALUE_BAD;
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}

A_INT32 Ar9300calTGTPwrHT20Set(double *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iv=0;
	int iMaxPier;
	if (iBand==band_BG) 
		iMaxPier = OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		iMaxPier = OSPREY_NUM_5G_20_TARGET_POWERS;
	for (i=ix; i<iMaxPier; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<14; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calTargetPower2GHT20[i].tPow2x[j] = (u_int8_t)(value[iv++]*2);	// eep value is double of entered value
			} else {
				Ar9300EepromStructGet()->calTargetPower5GHT20[i].tPow2x[j] = (u_int8_t)(value[iv++]*2);
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}
A_INT32 Ar9300calTGTPwrHT40Set(double *value, int ix, int iy, int iz, int num, int iBand)
{
	int i, j, j0, iv=0;
	int iMaxPier;
	if (iBand==band_BG) 
		iMaxPier = OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		iMaxPier = OSPREY_NUM_5G_20_TARGET_POWERS;
	for (i=ix; i<iMaxPier; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<14; j++) {
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->calTargetPower2GHT40[i].tPow2x[j] = (u_int8_t)(value[iv++]*2);	// eep value is double of entered value
			} else {
				Ar9300EepromStructGet()->calTargetPower5GHT40[i].tPow2x[j] = (u_int8_t)(value[iv++]*2);
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}
/*
A_INT32 Ar9300calFreqTGTSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	if (imode == legacy_CCK)
		return Ar9300calFreqTGTcckSet(value, iBand);
	else if (imode == legacy_OFDM)
		return Ar9300calFreqTGTLegacyOFDMSet(value, iBand);
	else if (imode == HT20)
		return Ar9300calFreqTGTHT20Set(value, iBand);
	else
		return Ar9300calFreqTGTHT40Set(value, iBand);
	return VALUE_OK; 
}

A_INT32 Ar9300calTGTPwrSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	if (imode == legacy_CCK)
		return Ar9300calTGTPwrCCKSet(value, iBand, iIndex);
	else if (imode == legacy_OFDM)
		return Ar9300calTGTPwrLegacyOFDMSet(value, iBand, iIndex);
	else if (imode == HT20)
		return Ar9300calTGTPwrHT20Set(value, iBand, iIndex);
	else
		return Ar9300calTGTPwrHT40Set(value, iBand, iIndex);
	return VALUE_OK; 
} */

A_INT32 Ar9300ctlIndexSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	int maxnum, i, iv=0;
	if (iBand==band_BG) 
		maxnum = OSPREY_NUM_CTLS_2G;
	else
		maxnum = OSPREY_NUM_CTLS_5G;

	for (i=ix; i<maxnum; i++) {
		if (iv>=num)
			break;
		if (iBand==band_BG) 
			Ar9300EepromStructGet()->ctlIndex_2G[i] = (u_int8_t)(value[iv++]);
		else 
			Ar9300EepromStructGet()->ctlIndex_5G[i] = (u_int8_t)(value[iv++]);
	}
    return VALUE_OK;
}

A_INT32 Ar9300ctlFreqSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	A_UINT8 bin;
	int i, j, j0, iCtl, iEdge, iv=0;
	if (iBand==band_BG) {
		iCtl = OSPREY_NUM_CTLS_2G;
		iEdge = OSPREY_NUM_BAND_EDGES_2G;
	} else {
		iCtl = OSPREY_NUM_CTLS_5G;
		iEdge = OSPREY_NUM_BAND_EDGES_5G;
	}
	for (i=ix; i<iCtl; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iEdge; j++) {
			bin = setFREQ2FBIN(value[iv++], iBand);
			if (iBand==band_BG)
				Ar9300EepromStructGet()->ctl_freqbin_2G[i][j] = bin;
			else
				Ar9300EepromStructGet()->ctl_freqbin_5G[i][j] = bin;
			if (iv>=num)
				break;
		}
	}
    return 0;
}
A_INT32 Ar9300ctlPowerSet(double *value, int ix, int iy, int iz, int num, int iBand)
{
	u_int8_t  value6;
	A_UINT8 bin;
	int i, j, j0, iCtl, iEdge, iv=0;
	if (iBand==band_BG) {
		iCtl = OSPREY_NUM_CTLS_2G;
		iEdge = OSPREY_NUM_BAND_EDGES_2G;
	} else {
		iCtl = OSPREY_NUM_CTLS_5G;
		iEdge = OSPREY_NUM_BAND_EDGES_5G;
	}
	for (i=ix; i<iCtl; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iEdge; j++) {
			value6 = ((u_int8_t)(value[iv++]*2.0)) & 0x3f;
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->ctlPowerData_2G[i].ctlEdges[j].tPower = value6;;
			} else {
				Ar9300EepromStructGet()->ctlPowerData_5G[i].ctlEdges[j].tPower = value6;;
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}


A_INT32 Ar9300ctlFlagSet(int *value, int ix, int iy, int iz, int num, int iBand)
{
	u_int8_t  value2;
	A_UINT8 bin;
	int i, j, j0, iCtl, iEdge, iv=0;
	if (iBand==band_BG) {
		iCtl = OSPREY_NUM_CTLS_2G;
		iEdge = OSPREY_NUM_BAND_EDGES_2G;
	} else {
		iCtl = OSPREY_NUM_CTLS_5G;
		iEdge = OSPREY_NUM_BAND_EDGES_5G;
	}
	for (i=ix; i<iCtl; i++) {
		if (iv>=num)
			break;
		if (i==ix)
			j0=iy;
		else
			j0=0;	
		for (j=j0; j<iEdge; j++) {
			value2 = (u_int8_t)(value[iv++] & 0x3) ;
			if (iBand==band_BG) {
				Ar9300EepromStructGet()->ctlPowerData_2G[i].ctlEdges[j].flag = value2;
			} else {
				Ar9300EepromStructGet()->ctlPowerData_5G[i].ctlEdges[j].flag = value2;
			}
			if (iv>=num)
				break;
		}
	}
    return VALUE_OK;
}


//
// returns 0 on success, negative error code on problem
//
int Ar9300NoiseFloorSet(int frequency, int ichain, int nf)
{
	int fx;
	int it;
    ar9300_eeprom_t *eep;
    u_int8_t *pCalPier;
    OSP_CAL_DATA_PER_FREQ_OP_LOOP *pCalPierStruct;
    int is2GHz;
	int ipier,npier;

	eep=Ar9300EepromStructGet();
	if(eep==0)
	{
		return -1;
	}
	//
	// check chain value
	//
	if(ichain<0 || ichain>=OSPREY_MAX_CHAINS)
	{
		return -2;
	}
   //
	// figure out which band we're using
	//
    is2GHz=(frequency<4000);
    if(is2GHz)
    {
        npier=OSPREY_NUM_2G_CAL_PIERS;
        pCalPier = eep->calFreqPier2G;
        pCalPierStruct = eep->calPierData2G[ichain];
    }
    else
    {
        npier=OSPREY_NUM_5G_CAL_PIERS;
        pCalPier = eep->calFreqPier5G;
        pCalPierStruct = eep->calPierData5G[ichain];
    }
    //
	// look for correct frequency pier
	//
	for(ipier=0; ipier<npier; ipier++)
	{
		fx = FBIN2FREQ(pCalPier[ipier], is2GHz);
		if(fx==frequency)
		{
			pCalPierStruct[ipier].rxNoisefloorCal=nf;
			return 0;
		}
	}

    return -3;
}


//
// returns 0 on success, negative error code on problem
//
int Ar9300NoiseFloorPowerSet(int frequency, int ichain, int nfpower)
{
	int fx;
	int it;
    ar9300_eeprom_t *eep;
    u_int8_t *pCalPier;
    OSP_CAL_DATA_PER_FREQ_OP_LOOP *pCalPierStruct;
    int is2GHz;
	int ipier,npier;

	eep=Ar9300EepromStructGet();
	if(eep==0)
	{
		return -1;
	}
	//
	// check chain value
	//
	if(ichain<0 || ichain>=OSPREY_MAX_CHAINS)
	{
		return -2;
	}
   //
	// figure out which band we're using
	//
    is2GHz=(frequency<4000);
    if(is2GHz)
    {
        npier=OSPREY_NUM_2G_CAL_PIERS;
        pCalPier = eep->calFreqPier2G;
        pCalPierStruct = eep->calPierData2G[ichain];
    }
    else
    {
        npier=OSPREY_NUM_5G_CAL_PIERS;
        pCalPier = eep->calFreqPier5G;
        pCalPierStruct = eep->calPierData5G[ichain];
    }
    //
	// look for correct frequency pier
	//
	for(ipier=0; ipier<npier; ipier++)
	{
		fx = FBIN2FREQ(pCalPier[ipier], is2GHz);
		if(fx==frequency)
		{
			pCalPierStruct[ipier].rxNoisefloorPower=nfpower;
			return 0;
		}
	}

    return -3;
}
