#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "wlantype.h"
//#include "osdep.h"
#include "NewArt.h"
#include "Interpolate.h"
#include "mCal9300.h"
#include "Ar9300Device.h"

#include "UserPrint.h"
#include "Field.h"
#include "ParameterSelect.h"
#include "Card.h"

#ifdef UNUSED

#include "default9300.h"

//static OSPREY_EEPROM default9300;
OSPREY_EEPROM currentCard9300;

#endif

//
// hal header files
//
#include "ah.h"
#include "ah_internal.h"
#include "ar9300eep.h"
#include "Ar9300EepromStructSet.h"
#include "ConfigurationStatus.h"



#ifdef UNUSEDHAL
int Ar9300EepromCalPierGet(int mode, int ipier, int ichain, 
                       int *pfrequency, int *pcorrection, int *ptemperature, int *pvoltage)
{
    A_UINT8 *pCalPier;
    OSP_CAL_DATA_PER_FREQ_OP_LOOP *pCalPierStruct;
    int is2GHz;

    if(ichain >= OSPREY_MAX_CHAINS) {
        UserPrint("Invalid chain index, must be less than %d\n", OSPREY_MAX_CHAINS);
        return -1;
    }

    if(mode) {//5GHz
        if(ipier >= OSPREY_NUM_5G_CAL_PIERS){
            UserPrint("Invalid 5GHz cal pier index, must be less than %d\n", OSPREY_NUM_5G_CAL_PIERS);
            return -1;
        }
        pCalPier = &(Ar9300EepromStructGet()->calFreqPier5G[ipier]);
        pCalPierStruct = &(Ar9300EepromStructGet()->calPierData5G[ichain][ipier]);
        is2GHz = 0;
    } else {
        if(ipier >= OSPREY_NUM_2G_CAL_PIERS){
            UserPrint("Invalid 2GHz cal pier index, must be less than %d\n", OSPREY_NUM_2G_CAL_PIERS);
            return -1;
        }

        pCalPier = &(Ar9300EepromStructGet()->calFreqPier2G[ipier]);
        pCalPierStruct = &(Ar9300EepromStructGet()->calPierData2G[ichain][ipier]);
        is2GHz = 1;
    }
    *pfrequency = FBIN2FREQ(*pCalPier, is2GHz);
    *pcorrection = pCalPierStruct->refPower;
    *ptemperature = pCalPierStruct->tempMeas;
    *pvoltage = pCalPierStruct->voltMeas;
    return 0;
}
#endif


int ar9300EepromUpdateCalPier(int pierIdx, int freq, int chain, 
                          int pwrCorrection, int voltMeas, int tempMeas)
{
    A_UINT8 *pCalPier;
    OSP_CAL_DATA_PER_FREQ_OP_LOOP *pCalPierStruct;
	int is2G;

    if(chain >= OSPREY_MAX_CHAINS) {
        UserPrint("Invalid chain index, must be less than %d\n", OSPREY_MAX_CHAINS);
        return -1;
    }

    if(freq < 3000) { /* 2GHz frequency pier */
        if(pierIdx >= OSPREY_NUM_2G_CAL_PIERS){
            UserPrint("Invalid 2GHz cal pier index, must be less than %d\n", OSPREY_NUM_2G_CAL_PIERS);
            return -1;
        }
		is2G=1;
        pCalPier = &(Ar9300EepromStructGet()->calFreqPier2G[pierIdx]);
        pCalPierStruct = &(Ar9300EepromStructGet()->calPierData2G[chain][pierIdx]);
    }
    else { /* 5GHz Freq pier */
        if(pierIdx >= OSPREY_NUM_5G_CAL_PIERS){
            UserPrint("Invalid 5GHz cal pier index, must be less than %d\n", OSPREY_NUM_5G_CAL_PIERS);
            return -1;
        }
		is2G=0;
        pCalPier = &(Ar9300EepromStructGet()->calFreqPier5G[pierIdx]);
        pCalPierStruct = &(Ar9300EepromStructGet()->calPierData5G[chain][pierIdx]);
    }

    *pCalPier = FREQ2FBIN(freq,is2G);
    pCalPierStruct->refPower = pwrCorrection;
    pCalPierStruct->tempMeas = tempMeas;
    pCalPierStruct->voltMeas = voltMeas; 
    return 0;
}


#ifdef UNUSEDHAL
A_UINT8 Ar9300EepromGetLegacyTrgtPwr(A_UINT16 rateIndex, int freq, A_BOOL is2GHz)
{
    A_UINT16 numPiers, i;
    A_UINT32 targetPowerArray[OSPREY_NUM_5G_20_TARGET_POWERS];
    A_UINT32 freqArray[OSPREY_NUM_5G_20_TARGET_POWERS]; 
    CAL_TARGET_POWER_LEG *pEepromTargetPwr;

    if (is2GHz) {
        numPiers = OSPREY_NUM_2G_20_TARGET_POWERS;    
        pEepromTargetPwr = Ar9300EepromStructGet()->calTargetPower2G;
    } else {
        numPiers = OSPREY_NUM_5G_20_TARGET_POWERS;
        pEepromTargetPwr = Ar9300EepromStructGet()->calTargetPower5G;
    }

    //create array of channels and targetpower from targetpower piers stored on eeprom
    for (i = 0; i < numPiers; i++) {
//        freqArray[i] = FBIN2FREQ(pEepromTargetPwr->bChannel, is2GHz);
        targetPowerArray[i] = pEepromTargetPwr->tPow2x[rateIndex];
    }

    //interpolate to get target power for given frequency
    return((A_UINT8)Interpolate(freq, freqArray, targetPowerArray, numPiers));
}

A_UINT8 Ar9300EepromGetHT20TrgtPwr(A_UINT16 rateIndex, int freq, A_BOOL is2GHz)
{
    A_UINT16 numPiers, i;
    A_UINT32 targetPowerArray[OSPREY_NUM_5G_20_TARGET_POWERS];
    A_UINT32 freqArray[OSPREY_NUM_5G_20_TARGET_POWERS]; 
    OSP_CAL_TARGET_POWER_HT *pEepromTargetPwr;

    if (is2GHz) {
        numPiers = OSPREY_NUM_2G_20_TARGET_POWERS;    
        pEepromTargetPwr = Ar9300EepromStructGet()->calTargetPower2GHT20;
    } else {
        numPiers = OSPREY_NUM_5G_20_TARGET_POWERS;
        pEepromTargetPwr = Ar9300EepromStructGet()->calTargetPower5GHT20;
    }

    //create array of channels and targetpower from targetpower piers stored on eeprom
    for (i = 0; i < numPiers; i++) {
//        freqArray[i] = FBIN2FREQ(pEepromTargetPwr->bChannel, is2GHz);
        targetPowerArray[i] = pEepromTargetPwr->tPow2x[rateIndex];
    }

    //interpolate to get target power for given frequency
    return((A_UINT8)Interpolate(freq, freqArray, targetPowerArray, numPiers));
}

A_UINT8 Ar9300EepromGetHT40TrgtPwr(A_UINT16 rateIndex, int freq, A_BOOL is2GHz)
{
    A_UINT16 numPiers, i;
    A_UINT32 targetPowerArray[OSPREY_NUM_5G_40_TARGET_POWERS];
    A_UINT32 freqArray[OSPREY_NUM_5G_40_TARGET_POWERS]; 
    OSP_CAL_TARGET_POWER_HT *pEepromTargetPwr;

    if (is2GHz) {
        numPiers = OSPREY_NUM_2G_40_TARGET_POWERS;    
        pEepromTargetPwr = Ar9300EepromStructGet()->calTargetPower2GHT40;
    } else {
        numPiers = OSPREY_NUM_5G_40_TARGET_POWERS;
        pEepromTargetPwr = Ar9300EepromStructGet()->calTargetPower5GHT40;
    }

    //create array of channels and targetpower from targetpower piers stored on eeprom
    for (i = 0; i < numPiers; i++) {
//        freqArray[i] = FBIN2FREQ(pEepromTargetPwr->bChannel, is2GHz);
        targetPowerArray[i] = pEepromTargetPwr->tPow2x[rateIndex];
    }

    //interpolate to get target power for given frequency
    return((A_UINT8)Interpolate(freq, freqArray, targetPowerArray, numPiers));
}

A_UINT8 Ar9300EepromGetCckTrgtPwr(A_UINT16 rateIndex, int freq)
{
    A_UINT16 numPiers = OSPREY_NUM_2G_CCK_TARGET_POWERS, i;
    A_UINT32 targetPowerArray[OSPREY_NUM_2G_CCK_TARGET_POWERS];
    A_UINT32 freqArray[OSPREY_NUM_2G_CCK_TARGET_POWERS]; 
    CAL_TARGET_POWER_LEG *pEepromTargetPwr = Ar9300EepromStructGet()->calTargetPowerCck;

    //create array of channels and targetpower from targetpower piers stored on eeprom
    for (i = 0; i < numPiers; i++) {
//        freqArray[i] = FBIN2FREQ(pEepromTargetPwr->bChannel, 1);
        targetPowerArray[i] = pEepromTargetPwr->tPow2x[rateIndex];
    }

    //interpolate to get target power for given frequency
    return((A_UINT8)Interpolate(freq, freqArray, targetPowerArray, numPiers));
}


void Ar9300SetTargetPowerFromEeprom(A_UINT16 freq)
{
    A_UINT8 targetPowerValT2[NUM_TRGT_PWR_REGISTERS];
    A_UINT8 ht40PowerIncForPdadc = 0; //hard code for now, need to get from eeprom struct
    A_BOOL  is2GHz = 0;

    if(freq < 4000) {
        is2GHz = 1;
    }

    targetPowerValT2[ALL_TARGET_LEGACY_6_24] = Ar9300EepromGetLegacyTrgtPwr(LEGACY_TARGET_RATE_6_24, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_LEGACY_36] = Ar9300EepromGetLegacyTrgtPwr(LEGACY_TARGET_RATE_36, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_LEGACY_48] = Ar9300EepromGetLegacyTrgtPwr(LEGACY_TARGET_RATE_48, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_LEGACY_54] = Ar9300EepromGetLegacyTrgtPwr(LEGACY_TARGET_RATE_54, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_LEGACY_1L_5L] = Ar9300EepromGetCckTrgtPwr(LEGACY_TARGET_RATE_1L_5L, freq);
    targetPowerValT2[ALL_TARGET_LEGACY_5S] = Ar9300EepromGetCckTrgtPwr(LEGACY_TARGET_RATE_5S, freq);
    targetPowerValT2[ALL_TARGET_LEGACY_11L] = Ar9300EepromGetCckTrgtPwr(LEGACY_TARGET_RATE_11L, freq);
    targetPowerValT2[ALL_TARGET_LEGACY_11S] = Ar9300EepromGetCckTrgtPwr(LEGACY_TARGET_RATE_11S, freq);
    targetPowerValT2[ALL_TARGET_HT20_0_8_16] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_0_8_16, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_1_3_9_11_17_19] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_1_3_9_11_17_19, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_4] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_4, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_5] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_5, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_6] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_6, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_7] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_7, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_12] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_12, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_13] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_13, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_14] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_14, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_15] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_15, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_20] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_20, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_21] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_21, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_22] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_22, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT20_23] = Ar9300EepromGetHT20TrgtPwr(HT_TARGET_RATE_23, freq, is2GHz);
    targetPowerValT2[ALL_TARGET_HT40_0_8_16] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_0_8_16, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_1_3_9_11_17_19] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_1_3_9_11_17_19, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_4] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_4, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_5] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_5, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_6] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_6, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_7] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_7, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_12] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_12, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_13] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_13, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_14] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_14, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_15] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_15, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_20] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_20, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_21] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_21, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_22] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_22, freq, is2GHz) + ht40PowerIncForPdadc;
    targetPowerValT2[ALL_TARGET_HT40_23] = Ar9300EepromGetHT40TrgtPwr(HT_TARGET_RATE_23, freq, is2GHz) + ht40PowerIncForPdadc;

    //Write target power array to registers
    Ar9300TransmitPowerRegWrite(targetPowerValT2);
} 
#endif

#ifdef UNUSEDHAL
A_UINT16 Ar9300EepromStructSize(void) 
{
    return sizeof(OSPREY_EEPROM);
}
#endif // UNUSEDHAL


#ifdef UNUSEDHAL
OSPREY_EEPROM *Ar9300EepromStructDefault(int defaultIndex) 
{
    //return one and only default for now
    //fill9300DefaultStruct(&default9300);
    if(defaultIndex==2)
    {
        return(&default9300);
    }
    else
    {
        return 0;
    }
}
#endif // UNUSEDHAL

int Ar9300eepromVersionGet()
{
	A_UINT8  value;
	value = (u_int8_t) Ar9300EepromStructGet()->eepromVersion;
    return value; 
}
int Ar9300templateVersionGet()
{
	A_UINT8  value;
	value = (u_int8_t) Ar9300EepromStructGet()->templateVersion;
    return value; 
}
int Ar9300FutureGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, max=MAX_MODAL_FUTURE;
	if (iBand==-1)
		max=MAX_BASE_EXTENSION_FUTURE;
	if (ix<0 || ix>=max) {
		for (i=0; i<max; i++) {
			if (iBand==band_BG)
				value[iv++] =  (A_UINT8)Ar9300EepromStructGet()->modalHeader2G.futureModal[i];
			else if (iBand==band_A)
				value[iv++] =  (A_UINT8)Ar9300EepromStructGet()->modalHeader5G.futureModal[i];
			else
				value[iv++] =  (A_UINT8)Ar9300EepromStructGet()->base_ext1.future[i];
			*num=max;
		}
	} else {
			if (iBand==band_BG)
				value[0] =  (A_UINT8)Ar9300EepromStructGet()->modalHeader2G.futureModal[ix];
			else if (iBand==band_A)
				value[0] =  (A_UINT8)Ar9300EepromStructGet()->modalHeader5G.futureModal[ix];
			else
				value[0] =  (A_UINT8)Ar9300EepromStructGet()->base_ext1.future[ix];
		*num=1;
	}
    return 0; 
}

int Ar9300AntDivCtrlGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->base_ext1.ant_div_control;
    return value; 
}

/*
 *Function Name:Ar9300pwrTuningCapsParamsGet
 *Parameters: returned string value for get 2 uint8 value delim with ,
 *Description: Get TuningCapsParams values from field of eeprom struct 2 uint8
 *Returns: zero
 */
int Ar9300pwrTuningCapsParamsGet(int *value, int ix, int *num)
{
	int i, iv=0;
	if (ix<0 || ix>2) {
		value[0] = Ar9300EepromStructGet()->baseEepHeader.params_for_tuning_caps[0];
		value[1] = Ar9300EepromStructGet()->baseEepHeader.params_for_tuning_caps[1];
		*num = 2;
	} else {
		value[0] = Ar9300EepromStructGet()->baseEepHeader.params_for_tuning_caps[1];
		*num = 1;
	}
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300regDmnGet
 *Parameters: returned string value for get value
 *Description: Get regDmn value from field of eeprom struct uint16*2 
 *Returns: zero
 */
int Ar9300regDmnGet(int *value, int ix, int *num)
{
	int i, iv=0;
	if (ix<0 || ix>2) {
		value[0] = Ar9300EepromStructGet()->baseEepHeader.regDmn[0];
		value[1] = Ar9300EepromStructGet()->baseEepHeader.regDmn[1];
		*num = 2;
	} else {
		value[0] = Ar9300EepromStructGet()->baseEepHeader.regDmn[1];
		*num = 1;
	}
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300txMaskGet
 *Parameters: returned string value for get value
 *Description: Get txMask value from field of eeprom struct uint8 (up 4 bit???)
 *Returns: zero
 */
int Ar9300txrxMaskGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.txrxMask;
    return value; 
}

/*
 *Function Name:Ar9300txMaskGet
 *Parameters: returned string value for get value
 *Description: Get txMask value from field of eeprom struct uint8 (up 4 bit???)
 *Returns: zero
 */
int Ar9300txMaskGet(void)
{
	A_UINT8  value;
	value = (Ar9300EepromStructGet()->baseEepHeader.txrxMask & 0xf0) >> 4;
    return value; 
}
/*
 *Function Name:Ar9300rxMaskGet
 *Parameters: returned string value for get value
 *Description: Get rxMask value from field of eeprom struct uint8 (low 4bits??)
 *Returns: zero
 */
int Ar9300rxMaskGet(void)
{
	A_UINT8  value;
	value = (Ar9300EepromStructGet()->baseEepHeader.txrxMask & 0x0f);
    return value; 
}
/*
 *Function Name:Ar9300opFlagsGet
 *Parameters: returned string value for get value
 *Description: Get opFlags value from field of eeprom struct uint8
 *Returns: zero
 */
int Ar9300opFlagsGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.opCapFlags.opFlags;
    return value; 
}/*
 *Function Name:Ar9300eepMiscGet
 *Parameters: returned string value for get value
 *Description: Get eepMisc value from field of eeprom struct uint8
 *Returns: zero
 */
int Ar9300eepMiscGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.opCapFlags.eepMisc;
    return value; 
}/*
 *Function Name:Ar9300rfSilentGet
 *Parameters: returned string value for get value
 *Description: Get rfSilent value from field of eeprom struct uint8
 *Returns: zero
 */
int Ar9300rfSilentGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.rfSilent;
    return value; 
}
int Ar9300rfSilentB0Get(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.rfSilent & 0x01;
    return value; 
}
int Ar9300rfSilentB1Get(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.rfSilent >>1;
    return value; 
}
int Ar9300rfSilentGPIOGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.rfSilent>>2;
    return value; 
}
/*
 *Function Name:Ar9300blueToothOptionsGet
 *Parameters: returned string value for get value
 *Description: Get blueToothOptions value from field of eeprom struct uint8
 *Returns: zero
 */
int Ar9300blueToothOptionsGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.blueToothOptions;
    return value; 
}/*
 *Function Name:Ar9300deviceCapGet
 *Parameters: returned string value for get value
 *Description: Get deviceCap value from field of eeprom struct uint8
 *Returns: zero
 */
int Ar9300deviceCapGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.deviceCap;
    return value; 
}/*
 *Function Name:Ar9300deviceTypeGet
 *Parameters: returned string value for get value
 *Description: Get deviceType value from field of eeprom struct uint8
 *Returns: zero
 */
int Ar9300deviceTypeGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.deviceType;
    return value; 
}
/*
 *Function Name:Ar9300pwrTableOffsetGet
 *Parameters: returned string value for get value
 *Description: Get pwrTableOffset value from field of eeprom struct int8
 *Returns: zero
 */
int Ar9300pwrTableOffsetGet(void)
{
	A_INT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.pwrTableOffset;
    return value; 
}

/*
 *Function Name:Ar9300TempSlopeGet
 *Parameters: returned string value for get value
 *Description: Get TempSlope value from field of eeprom struct int8
 *Returns: zero
 */
int Ar9300TempSlopeGet(int *value, int iBand)
{
	if (iBand==band_BG) {
		value[0] = Ar9300EepromStructGet()->modalHeader2G.tempSlope;
	} else {
		value[0] = Ar9300EepromStructGet()->modalHeader5G.tempSlope;
	}
    return VALUE_OK; 
}
int Ar9300TempSlopeLowGet(int *value)
{
	value[0] = Ar9300EepromStructGet()->base_ext2.tempSlopeLow;
    return VALUE_OK; 
}
int Ar9300TempSlopeHighGet(int *value)
{
	value[0] = Ar9300EepromStructGet()->base_ext2.tempSlopeHigh;
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300VoltSlopeGet
 *Parameters: returned string value for get value
 *Description: Get VoltSlope value from field of eeprom struct int8
 *Returns: zero
 */
int Ar9300VoltSlopeGet(int iBand)
{
	A_INT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.voltSlope;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.voltSlope;
	}	
    return value; 
}

/*
 *Function Name:Ar9300ReconfigMiscGet
 *Description: Get miscConfiguration all bits
 */
int Ar9300ReconfigMiscGet(void)
{
	A_INT32  bit;
	bit = Ar9300EepromStructGet()->baseEepHeader.miscConfiguration;
    return bit; 
}
/*
 *Function Name:Ar9300reconfigDriveStrengthGet
 *Description: Get reconfigDriveStrength flag from miscConfiguration 
 *             field of eeprom struct (bit 0)
 */
int Ar9300ReconfigDriveStrengthGet(void)
{
	A_INT32  bit;
	bit = Ar9300EepromStructGet()->baseEepHeader.miscConfiguration & 0x01;
    return bit; 
}
// bit 4 - enable quick drop
int Ar9300ReconfigQuickDropGet(void)
{
	A_INT32  bit;
	bit = Ar9300EepromStructGet()->baseEepHeader.miscConfiguration >>4;
    return bit; 
}
/*
 *Function Name:Ar9300TxGainGet
 *Parameters: returned string value for get bit
 *Description: Get TxGain flag in txrxgain field of eeprom struct's upper 4bits
 *Returns: zero
 */
int Ar9300TxGainGet(void)
{
	A_UINT8  value;
	value = (Ar9300EepromStructGet()->baseEepHeader.txrxgain  & 0xf0) >> 4;
    return value; 
}
/*
 *Function Name:Ar9300RxGainGet
 *Parameters: returned string value for get bit
 *Description: Get RxGain flag in txrxgain field of eeprom struct's upper 4bits
 *Returns: zero
 */
int Ar9300RxGainGet(void)
{
	A_UINT8  value;
	value = Ar9300EepromStructGet()->baseEepHeader.txrxgain  & 0x0f;
    return value; 
}

/*
 *Function Name:Ar9300EnableFeatureGet
 *Description: get all featureEnable 8bits
 *             field of eeprom struct (bit 0)
 * enable tx temp comp 
 */
int Ar9300EnableFeatureGet(void)
{
	int value;
    value = Ar9300EepromStructGet()->baseEepHeader.featureEnable;
    return value; 
}
/*
 *Function Name:Ar9300EnableTempCompensationGet
 *Description: get EnableTempCompensation flag from featureEnable 
 *             field of eeprom struct (bit 0)
 * enable tx temp comp 
 */
int Ar9300EnableTempCompensationGet(void)
{
	int value;
    value = Ar9300EepromStructGet()->baseEepHeader.featureEnable & 0x01;
    return value; 
}

/*
 *Function Name:Ar9300EnableVoltCompensationGet
 *Description: get rEnableVoltCompensation flag from featureEnable 
 *             field of eeprom struct (bit 1)
 * enable tx volt comp
 */
int Ar9300EnableVoltCompensationGet(void)
{
	int value;
    value = (Ar9300EepromStructGet()->baseEepHeader.featureEnable & 0x02) >> 1;
    return value; 
}

/*
 *Function Name:Ar9300EnableFastClockGet
 *Description: Get reconfigDriveStrength flag from featureEnable 
 *             field of eeprom struct (bit 2)
 * enable fastClock - default to 1
 */
int Ar9300EnableFastClockGet(void)
{
	int value;
    value = (Ar9300EepromStructGet()->baseEepHeader.featureEnable & 0x04) >> 2;
    return value; 
}

/*
 *Function Name:Ar9300EnableDoublingGet
 *Description: get EnableDoubling flag from featureEnable 
 *             field of eeprom struct (bit 3)
 * enable doubling - default to 1
 */
int Ar9300EnableDoublingGet(void)
{
	int value;
    value = (Ar9300EepromStructGet()->baseEepHeader.featureEnable & 0x08) >> 3;
    return value; 
}

/*
 *Function Name:Ar9300EnableTuningCapsGet
 *Description: get EnableTuningCaps flag from featureEnable 
 *             field of eeprom struct (bit 6)
 * enable TuningCaps - default to 0
 */
int Ar9300EnableTuningCapsGet(void)
{
	int value;
    value = (Ar9300EepromStructGet()->baseEepHeader.featureEnable & 0x40) >> 6;
    return value; 
}

/*
 *Function Name:Ar9300InternalRegulatorGet
 *Description: get internal regulator flag from featureEnable 
 *             field of eeprom struct (bit 4)
 * enable internal regulator - default to 1
 */
int Ar9300InternalRegulatorGet()
{
	int value;
    value = (Ar9300EepromStructGet()->baseEepHeader.featureEnable & 0x10) >> 4;
    return value; 
}

/*
 *Function Name:Ar9300PapdGet
 *Description: get PA predistortion enable flag from featureEnable 
 *             field of eeprom struct (bit 5)
 * enable paprd - default to 0 
 */
int Ar9300PapdGet(void)
{
	int value;
    value = (Ar9300EepromStructGet()->baseEepHeader.featureEnable & 0x20) >> 5;
    return value; 
}

int Ar9300PapdRateMaskHt20Get(int iBand)
{
	u_int32_t  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.papdRateMaskHt20;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.papdRateMaskHt20;
	}
    return value; 
}

int Ar9300PapdRateMaskHt40Get(int iBand)
{
	u_int32_t  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.papdRateMaskHt40;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.papdRateMaskHt40;
	}
    return value; 
}

int Ar9300_SWREG_Get(void)
{
	return Ar9300EepromStructGet()->baseEepHeader.swreg;
/*
	int ngot;
	unsigned int address;
	int low, high;
	int status=VALUE_OK;
	char regName[100];
	A_UINT32 mask, reg;

	sprintf(regName, "REG_CONTROL0.%s",sValue); 
	if (strcmp(sValue, "swreg")==0) 
		sprintf(regName, "REG_CONTROL0.swreg_pwd"); 
	ngot=FieldFind(regName,&address,&low,&high);
	if (ngot==1) {
		mask = MaskCreate(low, high);
		reg = Ar9300EepromStructGet()->baseEepHeader.swreg; 
		if (strcmp(sValue, "swreg")!=0) { 
			reg &= mask;
			reg = reg>>low;
		}
		sprintf(sValue, "0x%x", reg);
	} else {
		status = ERR_VALUE_BAD;
		sprintf(sValue, "Can't find reg name: %s", regName);
	}

    return status; */
}

/*
 *Function Name:Ar9300MacAdressGet
 *
 *Parameters: mac -- pointer to output pointer
 *
 *Description: Returns MAC address from eeprom structure.
 *
 *Returns: zero
 *
 */

A_INT32 Ar9300MacAdressGet(A_UINT8 *mac)
{
    A_INT16 i;
    for(i=0; i<6; i++)
        mac[i] = Ar9300EepromStructGet()->macAddr[i];
    return 0;
}

/*
 *Function Name:Ar9300CustomerDataGet
 *
 *Parameters: data -- pointer to output array. 
 *            len -- size of output array. 
 *
 *Description: Returns customer data array from eeprom structure.
 *
 *Returns: -1 on error condition
 *          0 on success.
 *
 */

A_INT32 Ar9300CustomerDataGet(A_UCHAR *data, A_INT32 len)
{
    A_INT16 i;

    if(len>OSPREY_CUSTOMER_DATA_SIZE) {
        UserPrint("Error:: Can't get %d char in the customer data array\n", len);
        return -1;
    }

    for(i=0; i<len; i++)
        data[i] = Ar9300EepromStructGet()->custData[i];

    return 0;
}
/*
A_INT32 Ar9300CalTgtPwrGet(int *pwrArr, int band, int htMode, int iFreqNum)
{
	int i, j;
	if (band==band_BG) {
		if (htMode==legacy_CCK) {
			for (i=0; i<NUM_CALTGT_FREQ_CCK;i++) {
				if (iFreqNum ==i) {
					for (j=0; j<NUM_TGT_DATARATE_LEGACY; j++) 
						pwrArr[j] = (int)(Ar9300EepromStructGet()->calTargetPowerCck[i].tPow2x[j]/2);
					break;
				}
			}
		} else {
			for (i=0; i<NUM_CALTGT_FREQ_2G;i++) {
				if (iFreqNum ==i) {
					if (htMode==legacy_OFDM) {
						for (j=0; j<NUM_TGT_DATARATE_LEGACY; j++) {
							pwrArr[j] = (int)(Ar9300EepromStructGet()->calTargetPower2G[i].tPow2x[j]/2);
						}
					} else if (htMode==HT20) {
						for (j=0; j<NUM_TGT_DATARATE_HT; j++)  {
							pwrArr[j] = (int)(Ar9300EepromStructGet()->calTargetPower2GHT20[i].tPow2x[j]/2);
						}
					} else if (htMode==HT40) {
						for (j=0; j<NUM_TGT_DATARATE_HT; j++)  {
							pwrArr[j] = (int)(Ar9300EepromStructGet()->calTargetPower2GHT40[i].tPow2x[j]/2);
						}
					}
					break;
				}
			}
		}
	} 

	if (band==band_A) {
		for (i=0; i<NUM_CALTGT_FREQ_5G;i++) {
			if (iFreqNum ==i) {
				if (htMode==legacy_OFDM) {
					for (j=0; j<NUM_TGT_DATARATE_LEGACY; j++) {
//						tmp = (int)Ar9300EepromStructGet()->calTargetPower5G[i].tPow2x[j];
						pwrArr[j] = (int)(Ar9300EepromStructGet()->calTargetPower5G[i].tPow2x[j]/2);
					}
				} else if (htMode==HT20) {
					for (j=0; j<NUM_TGT_DATARATE_HT; j++) {
						pwrArr[j] = (int)(Ar9300EepromStructGet()->calTargetPower5GHT20[i].tPow2x[j]/2);
					}
				} else if (htMode==HT40) {
					for (j=0; j<NUM_TGT_DATARATE_HT; j++) {
						pwrArr[j] = (int)(Ar9300EepromStructGet()->calTargetPower5GHT40[i].tPow2x[j]/2);
					}
				}
				break;
			}
		}
	}
	return 0;
}
*/
int setFBIN2FREQ(int bin, int iBand)
{
	int freq;
	if (bin==0)
		return 0;
	if (iBand==band_BG)
		freq = FBIN2FREQ(bin,1);
	else
		freq = FBIN2FREQ(bin,0);
	return freq;
}

A_INT32 Ar9300antCtrlChainGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iCMaxChain=OSPREY_MAX_CHAINS;
	int val;
	if (ix<0 || ix>iCMaxChain) {
		for (i=0; i<iCMaxChain; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->modalHeader2G.antCtrlChain[i];
			} else {
				val = Ar9300EepromStructGet()->modalHeader5G.antCtrlChain[i];
			}
			value[i] = val;
		}
		*num = iCMaxChain;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->modalHeader2G.antCtrlChain[ix];
		} else {
			val = Ar9300EepromStructGet()->modalHeader5G.antCtrlChain[ix];
		}
		value[0] = val;
		*num = 1;
	}
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300AntCtrlCommonGet
 *Parameters: returned string value for get value
 *Description: Get AntCtrlCommon value from field of eeprom struct u_int32
 *Returns: zero
 */
int Ar9300AntCtrlCommonGet(int iBand)
{
	A_UINT32  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.antCtrlCommon;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.antCtrlCommon;
	}	
    return value; 
}
/*
 *Function Name:Ar9300AntCtrlCommon2Get
 *Parameters: returned string value for get value
 *Description: Get AntCtrlCommon2 value from field of eeprom struct u_int32
 *Returns: zero
 */
int Ar9300AntCtrlCommon2Get(int iBand)
{
	A_UINT32  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.antCtrlCommon2;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.antCtrlCommon2;
	}	
    return value; 
}


/*
 *Function Name:Ar9300xatten1DBGet
 *Parameters: returned string value for get value
 *Description: Get xatten1DB flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300xatten1DBGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iCMaxChain=OSPREY_MAX_CHAINS;
	int val;
	if (ix<0 || ix>iCMaxChain) {
		for (i=0; i<iCMaxChain; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->modalHeader2G.xatten1DB[i];
			} else {
				val = Ar9300EepromStructGet()->modalHeader5G.xatten1DB[i];
			}
			value[i] = val;
		}
		*num = iCMaxChain;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->modalHeader2G.xatten1DB[ix];
		} else {
			val = Ar9300EepromStructGet()->modalHeader5G.xatten1DB[ix];
		}
		value[0] = val;
		*num = 1;
	}
    return VALUE_OK; 
} 

A_INT32 Ar9300xatten1DBLowGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iCMaxChain=OSPREY_MAX_CHAINS;
	int val;
	if (iBand==band_BG) {
		*num=0;
		return 0;
	}
	if (ix<0 || ix>iCMaxChain) {
		for (i=0; i<iCMaxChain; i++) {
			value[i] = Ar9300EepromStructGet()->base_ext2.xatten1DBLow[i];
		}
		*num = iCMaxChain;
	} else {
		value[0] = Ar9300EepromStructGet()->base_ext2.xatten1DBLow[ix];
		*num = 1;
	}
    return VALUE_OK; 
}

A_INT32 Ar9300xatten1DBHighGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iCMaxChain=OSPREY_MAX_CHAINS;
	int val;
	if (iBand==band_BG) {
		*num=0;
		return 0;
	}
	if (ix<0 || ix>iCMaxChain) {
		for (i=0; i<iCMaxChain; i++) {
			value[i] = Ar9300EepromStructGet()->base_ext2.xatten1DBHigh[i];
		}
		*num = iCMaxChain;
	} else {
		value[0] = Ar9300EepromStructGet()->base_ext2.xatten1DBHigh[ix];
		*num = 1;
	}
    return VALUE_OK; 
}


/*
 *Function Name:Ar9300xatten1MarginGet
 *Parameters: returned string value for get value
 *Description: Get xatten1Margin flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300xatten1MarginGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iCMaxChain=OSPREY_MAX_CHAINS;
	int val;
	if (ix<0 || ix>iCMaxChain) {
		for (i=0; i<iCMaxChain; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->modalHeader2G.xatten1Margin[i];
			} else {
				val = Ar9300EepromStructGet()->modalHeader5G.xatten1Margin[i];
			}
			value[i] = val;
		}
		*num = iCMaxChain;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->modalHeader2G.xatten1Margin[ix];
		} else {
			val = Ar9300EepromStructGet()->modalHeader5G.xatten1Margin[ix];
		}
		value[0] = val;
		*num = 1;
	}
    return VALUE_OK; 
}

A_INT32 Ar9300xatten1MarginLowGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iCMaxChain=OSPREY_MAX_CHAINS;
	int val;
	if (ix<0 || ix>iCMaxChain) {
		for (i=0; i<iCMaxChain; i++) {
			value[i] = Ar9300EepromStructGet()->base_ext2.xatten1MarginLow[i];
		}
		*num = iCMaxChain;
	} else {
		value[0] = Ar9300EepromStructGet()->base_ext2.xatten1MarginLow[ix];
		*num = 1;
	}
    return VALUE_OK; 
}

A_INT32 Ar9300xatten1MarginHighGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iCMaxChain=OSPREY_MAX_CHAINS;
	int val;
	if (ix<0 || ix>iCMaxChain) {
		for (i=0; i<iCMaxChain; i++) {
			value[i] = Ar9300EepromStructGet()->base_ext2.xatten1MarginHigh[i];
		}
		*num = iCMaxChain;
	} else {
		value[0] = Ar9300EepromStructGet()->base_ext2.xatten1MarginHigh[ix];
		*num = 1;
	}
    return VALUE_OK; 
}

/*
 *Function Name:Ar9300spurChansGet
 *Parameters: returned string value for get value
 *Description: Get spurChans flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300spurChansGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0;
	int val;
	if (ix<0 || ix>OSPREY_EEPROM_MODAL_SPURS) {
		for (i=0; i<OSPREY_EEPROM_MODAL_SPURS; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->modalHeader2G.spurChans[i];
			} else {
				val = Ar9300EepromStructGet()->modalHeader5G.spurChans[i];
			}
			value[i] = setFBIN2FREQ(val, iBand);
		}
		*num = OSPREY_EEPROM_MODAL_SPURS;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->modalHeader2G.spurChans[ix];
		} else {
			val = Ar9300EepromStructGet()->modalHeader5G.spurChans[ix];
		}
		value[0] = setFBIN2FREQ(val, iBand);
		*num = 1;
	}
    return VALUE_OK; 
}

/*
 *Function Name:Ar9300noiseFloorThreshChGet
 *Parameters: returned string value for get value
 *Description: Get noiseFloorThreshCh flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300noiseFloorThreshChGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iCMaxChain=OSPREY_MAX_CHAINS;
	int val;
	if (ix<0 || ix>iCMaxChain) {
		for (i=0; i<iCMaxChain; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->modalHeader2G.noiseFloorThreshCh[i];
			} else {
				val = Ar9300EepromStructGet()->modalHeader5G.noiseFloorThreshCh[i];
			}
			value[i] = val;
		}
		*num = iCMaxChain;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->modalHeader2G.noiseFloorThreshCh[ix];
		} else {
			val = Ar9300EepromStructGet()->modalHeader5G.noiseFloorThreshCh[ix];
		}
		value[0] = val;
		*num = 1;
	}
    return VALUE_OK; 
}

A_INT32 Ar9300ReservedGet(int *value, int ix, int *num, int iBand)
{
	int i, iv=0, iMax=MAX_MODAL_RESERVED;
	int val;
	if (ix<0 || ix>iMax) {
		for (i=0; i<iMax; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->modalHeader2G.reserved[i];
			} else {
				val = Ar9300EepromStructGet()->modalHeader5G.reserved[i];
			}
			value[i] = val;
		}
		*num = iMax;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->modalHeader2G.reserved[ix];
		} else {
			val = Ar9300EepromStructGet()->modalHeader5G.reserved[ix];
		}
		value[0] = val;
		*num = 1;
	}
    return VALUE_OK; 
}


int Ar9300QuickDropGet(int iBand)
{
	int8_t  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.quickDrop;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.quickDrop;
	}	
    return value; 
}

int Ar9300QuickDropLowGet()
{
	int8_t  value;
	value = Ar9300EepromStructGet()->base_ext1.quickDropLow;
    return value; 
}

int Ar9300QuickDropHighGet()
{
	int8_t  value;
	value = Ar9300EepromStructGet()->base_ext1.quickDropHigh;
    return value; 
}

/*
 *Function Name:Ar9300xpaBiasLvlGet
 *Parameters: returned string value for get value
 *Description: Get xpaBiasLvl value from field of eeprom struct u_int8_t
 *Returns: zero
 */
int Ar9300xpaBiasLvlGet(int iBand)
{
	u_int8_t  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.xpaBiasLvl;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.xpaBiasLvl;
	}	
    return value; 
}

/*
 *Function Name:Ar9300txFrameToDataStartGet
 *Parameters: returned string value for get value
 *Description: Get txFrameToDataStart value from field of eeprom struct u_int8_t
 *Returns: zero
 */
int Ar9300txFrameToDataStartGet( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.txFrameToDataStart;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.txFrameToDataStart;
	}	
    return value; 
}

/*
 *Function Name:Ar9300txFrameToPaOnGet
 *Parameters: returned string value for get value
 *Description: Get txFrameToPaOn value from field of eeprom struct u_int8_t
 *Returns: zero
 */
int Ar9300txFrameToPaOnGet( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.txFrameToPaOn;
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.txFrameToPaOn;
	}	
    return value; 
}
/*
 *Function Name:Ar9300txClipGet
 *Parameters: returned string value for get value
 *Description: Get txClip value from field of eeprom struct u_int8_t (4 bits tx_clip)
 *Returns: zero
 */
int Ar9300txClipGet( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.txClip & 0x0f;		// which 4 bits are for tx_clip???
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.txClip & 0x0f;
	}	
    return value; 
}
/*
 *Function Name:Ar9300dac_scale_cckGet
 *Parameters: returned string value for get value
 *Description: Get txClip(dac_scale_cck) value from field of eeprom struct u_int8_t (4 bits tx_clip)
 *Returns: zero
 */
int Ar9300dac_scale_cckGet( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = (Ar9300EepromStructGet()->modalHeader2G.txClip & 0xf0) >> 4;		// which 4 bits are for dac_scale_cck???
	} else {
		value = (Ar9300EepromStructGet()->modalHeader5G.txClip & 0xf0) >> 4;
	}	
    return value; 
}
/*
 *Function Name:Ar9300antennaGainGet
 *Parameters: returned string value for get value
 *Description: Get antennaGain value from field of eeprom struct int8_t 
 *Returns: zero
 */
int Ar9300antennaGainGet( int iBand)
{
	A_INT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.antennaGain;		
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.antennaGain;
	}	
    return value; 
}
/*
 *Function Name:Ar9300adcDesiredSizeGet
 *Parameters: returned string value for get value
 *Description: Get adcDesiredSize value from field of eeprom struct int8_t 
 *Returns: zero
 */
int Ar9300adcDesiredSizeGet( int iBand)
{
	A_INT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.adcDesiredSize;		
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.adcDesiredSize;
	}	
    return value; 
}
/*
 *Function Name:Ar9300switchSettlingGet
 *Parameters: returned string value for get value
 *Description: Get switchSettling value from field of eeprom struct u_int8_t 
 *Returns: zero
 */
int Ar9300switchSettlingGet( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.switchSettling;		
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.switchSettling;
	}	
    return value; 
}
/*
 *Function Name:Ar9300txEndToXpaOffGet
 *Parameters: returned string value for get value
 *Description: Get txEndToXpaOff value from field of eeprom struct u_int8_t 
 *Returns: zero
 */
int Ar9300txEndToXpaOffGet( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.txEndToXpaOff;		
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.txEndToXpaOff;
	}	
    return value; 
}
/*
 *Function Name:Ar9300txEndToRxOnGet
 *Parameters: returned string value for get value
 *Description: Get txEndToRxOn value from field of eeprom struct u_int8_t 
 *Returns: zero
 */
int Ar9300txEndToRxOnGet( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.txEndToRxOn;		
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.txEndToRxOn;
	}	
    return value; 
}
/*
 *Function Name:Ar9300txFrameToXpaOnGet
 *Parameters: returned string value for get value
 *Description: Get txFrameToXpaOn value from field of eeprom struct u_int8_t 
 *Returns: zero
 */
int Ar9300txFrameToXpaOnGet( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.txFrameToXpaOn;		
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.txFrameToXpaOn;
	}	
    return value; 
}
/*
 *Function Name:Ar9300thresh62Get
 *Parameters: returned string value for get value
 *Description: Get thresh62 value from field of eeprom struct u_int8_t 
 *Returns: zero
 */
int Ar9300thresh62Get( int iBand)
{
	A_UINT8  value;
	if (iBand==band_BG) {
		value = Ar9300EepromStructGet()->modalHeader2G.thresh62;		
	} else {
		value = Ar9300EepromStructGet()->modalHeader5G.thresh62;
	}	
    return value; 
}

/*
 *Function Name:Ar9300calFreqPierGet
 *Parameters: returned string value for get value
 *Description: Get calFreqPier flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calFreqPierGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, iv=0, iMaxPier;
	int val;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_CAL_PIERS;
	else
		iMaxPier=OSPREY_NUM_5G_CAL_PIERS;
	if (ix<0 || ix>iMaxPier) {
		for (i=0; i<iMaxPier; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->calFreqPier2G[i];
			} else {
				val = Ar9300EepromStructGet()->calFreqPier5G[i];
			}
			value[i] = setFBIN2FREQ(val, iBand);
		}
		*num = OSPREY_NUM_2G_CAL_PIERS;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->calFreqPier2G[ix];
		} else {
			val = Ar9300EepromStructGet()->calFreqPier5G[ix];
		}
		value[0] = setFBIN2FREQ(val, iBand);
		*num = 1;
	}
	return VALUE_OK;
}
/*
 *Function Name:Ar9300calPierDataRefPowerGet
 *Parameters: returned string value for get value
 *Description: Get calPierData.RefPower flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataRefPowerGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iMaxPier, iv=0;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_CAL_PIERS;
	else
		iMaxPier=OSPREY_NUM_5G_CAL_PIERS;
	if (iy<0 || iy>=iMaxPier) {
		if (ix<0 || ix>=OSPREY_MAX_CHAINS) {
			// get all i, all j
			for (i=0; i<OSPREY_MAX_CHAINS; i++) {
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[i][j].refPower;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[i][j].refPower;
					}
				}
				*num = OSPREY_MAX_CHAINS*iMaxPier;
			}
		} else { // get all j for ix chain
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[ix][j].refPower;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[ix][j].refPower;
					}
				}
				*num = iMaxPier;
		}
	} else {
		if (iBand==band_BG) {
			value[0] = Ar9300EepromStructGet()->calPierData2G[ix][iy].refPower;
		} else {
			value[0] = Ar9300EepromStructGet()->calPierData5G[ix][iy].refPower;
		}
		*num = 1;
	}
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300calPierDataVoltMeasGet
 *Parameters: returned string value for get value
 *Description: Get calPierData.voltMeas flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataVoltMeasGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iMaxPier, iv=0;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_CAL_PIERS;
	else
		iMaxPier=OSPREY_NUM_5G_CAL_PIERS;
	if (iy<0 || iy>=iMaxPier) {
		if (ix<0 || ix>=OSPREY_MAX_CHAINS) {
			// get all i, all j
			for (i=0; i<OSPREY_MAX_CHAINS; i++) {
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[i][j].voltMeas;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[i][j].voltMeas;
					}
				}
				*num = OSPREY_MAX_CHAINS*iMaxPier;
			}
		} else { // get all j for ix chain
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[ix][j].voltMeas;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[ix][j].voltMeas;
					}
				}
				*num = iMaxPier;
		}
	} else {
		if (iBand==band_BG) {
			value[0] = Ar9300EepromStructGet()->calPierData2G[ix][iy].voltMeas;
		} else {
			value[0] = Ar9300EepromStructGet()->calPierData5G[ix][iy].voltMeas;
		}
		*num = 1;
	}
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300calPierDataTempMeasGet
 *Parameters: returned string value for get value
 *Description: Get calPierData.tempMeas flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataTempMeasGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iMaxPier, iv=0;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_CAL_PIERS;
	else
		iMaxPier=OSPREY_NUM_5G_CAL_PIERS;
	if (iy<0 || iy>=iMaxPier) {
		if (ix<0 || ix>=OSPREY_MAX_CHAINS) {
			// get all i, all j
			for (i=0; i<OSPREY_MAX_CHAINS; i++) {
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[i][j].tempMeas;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[i][j].tempMeas;
					}
				}
				*num = OSPREY_MAX_CHAINS*iMaxPier;
			}
		} else { // get all j for ix chain
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[ix][j].tempMeas;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[ix][j].tempMeas;
					}
				}
				*num = iMaxPier;
		}
	} else {
		if (iBand==band_BG) {
			value[0] = Ar9300EepromStructGet()->calPierData2G[ix][iy].tempMeas;
		} else {
			value[0] = Ar9300EepromStructGet()->calPierData5G[ix][iy].tempMeas;
		}
		*num = 1;
	}
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300calPierDataRxNoisefloorCalGet
 *Parameters: returned string value for get value
 *Description: Get calPierData.rxNoisefloorCal flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataRxNoisefloorCalGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iMaxPier, iv=0;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_CAL_PIERS;
	else
		iMaxPier=OSPREY_NUM_5G_CAL_PIERS;
	if (iy<0 || iy>=iMaxPier) {
		if (ix<0 || ix>=OSPREY_MAX_CHAINS) {
			// get all i, all j
			for (i=0; i<OSPREY_MAX_CHAINS; i++) {
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[i][j].rxNoisefloorCal;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[i][j].rxNoisefloorCal;
					}
				}
				*num = OSPREY_MAX_CHAINS*iMaxPier;
			}
		} else { // get all j for ix chain
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[ix][j].rxNoisefloorCal;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[ix][j].rxNoisefloorCal;
					}
				}
				*num = iMaxPier;
		}
	} else {
		if (iBand==band_BG) {
			value[0] = Ar9300EepromStructGet()->calPierData2G[ix][iy].rxNoisefloorCal;
		} else {
			value[0] = Ar9300EepromStructGet()->calPierData5G[ix][iy].rxNoisefloorCal;
		}
		*num = 1;
	}
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300calPierDataRxNoisefloorPowerGet
 *Parameters: returned string value for get value
 *Description: Get calPierData.rxNoisefloorPower flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataRxNoisefloorPowerGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iMaxPier, iv=0;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_CAL_PIERS;
	else
		iMaxPier=OSPREY_NUM_5G_CAL_PIERS;
	if (iy<0 || iy>=iMaxPier) {
		if (ix<0 || ix>=OSPREY_MAX_CHAINS) {
			// get all i, all j
			for (i=0; i<OSPREY_MAX_CHAINS; i++) {
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[i][j].rxNoisefloorPower;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[i][j].rxNoisefloorPower;
					}
				}
				*num = OSPREY_MAX_CHAINS*iMaxPier;
			}
		} else { // get all j for ix chain
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[ix][j].rxNoisefloorPower;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[ix][j].rxNoisefloorPower;
					}
				}
				*num = iMaxPier;
		}
	} else {
		if (iBand==band_BG) {
			value[0] = Ar9300EepromStructGet()->calPierData2G[ix][iy].rxNoisefloorPower;
		} else {
			value[0] = Ar9300EepromStructGet()->calPierData5G[ix][iy].rxNoisefloorPower;
		}
		*num = 1;
	}
    return VALUE_OK; 
}
/*
 *Function Name:Ar9300calPierDataRxTempMeasGet
 *Parameters: returned string value for get value
 *Description: Get calPierData.rxTempMeas flag in field of eeprom struct in OSPREY_MODAL_EEP_HEADER (u_int8_t) 
 *Returns: zero
 */
A_INT32 Ar9300calPierDataRxTempMeasGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iMaxPier, iv=0;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_CAL_PIERS;
	else
		iMaxPier=OSPREY_NUM_5G_CAL_PIERS;
	if (iy<0 || iy>=iMaxPier) {
		if (ix<0 || ix>=OSPREY_MAX_CHAINS) {
			// get all i, all j
			for (i=0; i<OSPREY_MAX_CHAINS; i++) {
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[i][j].rxTempMeas;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[i][j].rxTempMeas;
					}
				}
				*num = OSPREY_MAX_CHAINS*iMaxPier;
			}
		} else { // get all j for ix chain
				for (j=0; j<iMaxPier; j++) {
					if (iBand==band_BG) {
						value[iv++] = Ar9300EepromStructGet()->calPierData2G[ix][j].rxTempMeas;
					} else {
						value[iv++] = Ar9300EepromStructGet()->calPierData5G[ix][j].rxTempMeas;
					}
				}
				*num = iMaxPier;
		}
	} else {
		if (iBand==band_BG) {
			value[0] = Ar9300EepromStructGet()->calPierData2G[ix][iy].rxTempMeas;
		} else {
			value[0] = Ar9300EepromStructGet()->calPierData5G[ix][iy].rxTempMeas;
		}
		*num = 1;
	}
    return VALUE_OK; 
}

A_INT32 Ar9300calFreqTGTcckGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, iv=0, iMaxPier;
	int val;
	iMaxPier=OSPREY_NUM_2G_CCK_TARGET_POWERS;
	if (ix<0 || ix>iMaxPier) {
		for (i=0; i<iMaxPier; i++) {
			val = Ar9300EepromStructGet()->calTarget_freqbin_Cck[i];
			value[i] = setFBIN2FREQ(val, iBand);
		}
		*num = iMaxPier;
	} else {
		val = Ar9300EepromStructGet()->calTarget_freqbin_Cck[ix];
		value[0] = setFBIN2FREQ(val, iBand);
		*num = 1;
	}
	return VALUE_OK;
}

A_INT32 Ar9300calFreqTGTLegacyOFDMGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, iv=0, iMaxPier;
	int val;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		iMaxPier=OSPREY_NUM_5G_20_TARGET_POWERS;
	if (ix<0 || ix>iMaxPier) {
		for (i=0; i<iMaxPier; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->calTarget_freqbin_2G[i];
			} else {
				val = Ar9300EepromStructGet()->calTarget_freqbin_5G[i];
			}
			value[i] = setFBIN2FREQ(val, iBand);
		}
		*num = iMaxPier;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->calTarget_freqbin_2G[ix];
		} else {
			val = Ar9300EepromStructGet()->calTarget_freqbin_5G[ix];
		}
		value[0] = setFBIN2FREQ(val, iBand);
		*num = 1;
	}
	return VALUE_OK;
}

A_INT32 Ar9300calFreqTGTHT20Get(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, iv=0, iMaxPier;
	int val;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		iMaxPier=OSPREY_NUM_5G_20_TARGET_POWERS;
	if (ix<0 || ix>iMaxPier) {
		for (i=0; i<iMaxPier; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->calTarget_freqbin_2GHT20[i];
			} else {
				val = Ar9300EepromStructGet()->calTarget_freqbin_5GHT20[i];
			}
			value[i] = setFBIN2FREQ(val, iBand);
		}
		*num = iMaxPier;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->calTarget_freqbin_2GHT20[ix];
		} else {
			val = Ar9300EepromStructGet()->calTarget_freqbin_5GHT20[ix];
		}
		value[0] = setFBIN2FREQ(val, iBand);
		*num = 1;
	}
	return VALUE_OK;
}

A_INT32 Ar9300calFreqTGTHT40Get(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, iv=0, iMaxPier;
	int val;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_40_TARGET_POWERS;
	else
		iMaxPier=OSPREY_NUM_5G_40_TARGET_POWERS;
	if (ix<0 || ix>iMaxPier) {
		for (i=0; i<iMaxPier; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->calTarget_freqbin_2GHT40[i];
			} else {
				val = Ar9300EepromStructGet()->calTarget_freqbin_5GHT40[i];
			}
			value[i] = setFBIN2FREQ(val, iBand);
		}
		*num = iMaxPier;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->calTarget_freqbin_2GHT40[ix];
		} else {
			val = Ar9300EepromStructGet()->calTarget_freqbin_5GHT40[ix];
		}
		value[0] = setFBIN2FREQ(val, iBand);
		*num = 1;
	}
	return VALUE_OK;
}

A_INT32 Ar9300calTGTPwrCCKGet(double *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iv=0, iMaxPier, jMaxRate=4;
	int val;
	iMaxPier=OSPREY_NUM_2G_CCK_TARGET_POWERS;
	if (iy<0 || iy>=jMaxRate) {
		if (ix<0 || ix>=iMaxPier) {
			// get all i, all j
			for (i=0; i<iMaxPier; i++) {
				for (j=0; j<jMaxRate; j++) {
					value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPowerCck[i].tPow2x[j])/2.0;
				}
			}
			*num = jMaxRate*iMaxPier;
		} else { // get all j for ix chain
				for (j=0; j<jMaxRate; j++) {
					value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPowerCck[ix].tPow2x[j])/2.0;
				}
				*num = jMaxRate;
		}
	} else {
		if (ix<0 || ix>=iMaxPier) {
			for (i=0; i<iMaxPier; i++) {
				value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPowerCck[i].tPow2x[iy])/2.0;
				*num = iMaxPier;
			}
		} else {
			value[0] = ((double)Ar9300EepromStructGet()->calTargetPowerCck[ix].tPow2x[iy])/2.0;
			*num = 1;
		}
	}	
	return VALUE_OK; 
}
A_INT32 Ar9300calTGTPwrLegacyOFDMGet(double *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iv=0, iMaxPier, jMaxRate=4;
	int val;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		iMaxPier=OSPREY_NUM_5G_20_TARGET_POWERS;
	if (iy<0 || iy>=jMaxRate) {
		if (ix<0 || ix>=iMaxPier) {
			// get all i, all j
			for (i=0; i<iMaxPier; i++) {
				for (j=0; j<jMaxRate; j++) {
					if (iBand==band_BG) {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2G[i].tPow2x[j])/2.0;
					} else {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5G[i].tPow2x[j])/2.0;
					}
				}
			}
			*num = jMaxRate*iMaxPier;
		} else { // get all j for ix chain
				for (j=0; j<jMaxRate; j++) {
					if (iBand==band_BG) {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2G[ix].tPow2x[j])/2.0;
					} else {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5G[ix].tPow2x[j])/2.0;
					}
				}
				*num = jMaxRate;
		}
	} else {
		if (ix<0 || ix>=iMaxPier) {
			for (i=0; i<iMaxPier; i++) {
				if (iBand==band_BG) {
					value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2G[i].tPow2x[iy])/2.0;
				} else {
					value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5G[i].tPow2x[iy])/2.0;
				}
				*num = iMaxPier;
			}
		} else {
			if (iBand==band_BG) {
				value[0] =((double) Ar9300EepromStructGet()->calTargetPower2G[ix].tPow2x[iy])/2.0;
			} else {
				value[0] = ((double)Ar9300EepromStructGet()->calTargetPower5G[ix].tPow2x[iy])/2.0;
			}
			*num = 1;
		}
	}
    return VALUE_OK; 
}
A_INT32 Ar9300calTGTPwrHT20Get(double *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iv=0, iMaxPier, jMaxRate=14;
	int val;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_20_TARGET_POWERS;
	else
		iMaxPier=OSPREY_NUM_5G_20_TARGET_POWERS;
	if (iy<0 || iy>=jMaxRate) {
		if (ix<0 || ix>=iMaxPier) {
			// get all i, all j
			for (i=0; i<iMaxPier; i++) {
				for (j=0; j<jMaxRate; j++) {
					if (iBand==band_BG) {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2GHT20[i].tPow2x[j])/2.0;
					} else {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5GHT20[i].tPow2x[j])/2.0;
					}
				}
			}
			*num = jMaxRate*iMaxPier;
		} else { // get all j for ix chain
				for (j=0; j<jMaxRate; j++) {
					if (iBand==band_BG) {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2GHT20[ix].tPow2x[j])/2.0;
					} else {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5GHT20[ix].tPow2x[j])/2.0;
					}
				}
				*num = jMaxRate;
		}
	} else {
		if (ix<0 || ix>=iMaxPier) {
			for (i=0; i<iMaxPier; i++) {
				if (iBand==band_BG) {
					value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2GHT20[i].tPow2x[iy])/2.0;
				} else {
					value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5GHT20[i].tPow2x[iy])/2.0;
				}
				*num = iMaxPier;
			}
		} else {
			if (iBand==band_BG) {
				value[0] = ((double)Ar9300EepromStructGet()->calTargetPower2GHT20[ix].tPow2x[iy])/2.0;
			} else {
				value[0] = ((double)Ar9300EepromStructGet()->calTargetPower5GHT20[ix].tPow2x[iy])/2.0;
			}
			*num = 1;
		}
	}
    return VALUE_OK; 
}
A_INT32 Ar9300calTGTPwrHT40Get(double *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iv=0, iMaxPier, jMaxRate=14;
	int val;
	if (iBand==band_BG) 
		iMaxPier=OSPREY_NUM_2G_40_TARGET_POWERS;
	else
		iMaxPier=OSPREY_NUM_5G_40_TARGET_POWERS;
	if (iy<0 || iy>=jMaxRate) {
		if (ix<0 || ix>=iMaxPier) {
			// get all i, all j
			for (i=0; i<iMaxPier; i++) {
				for (j=0; j<jMaxRate; j++) {
					if (iBand==band_BG) {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2GHT40[i].tPow2x[j])/2.0;
					} else {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5GHT40[i].tPow2x[j])/2.0;
					}
				}
			}
			*num = jMaxRate*iMaxPier;
		} else { // get all j for ix chain
				for (j=0; j<jMaxRate; j++) {
					if (iBand==band_BG) {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2GHT40[ix].tPow2x[j])/2.0;
					} else {
						value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5GHT40[ix].tPow2x[j])/2.0;
					}
				}
				*num = jMaxRate;
		}
	} else {
		if (ix<0 || ix>=iMaxPier) {
			for (i=0; i<iMaxPier; i++) {
				if (iBand==band_BG) {
					value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower2GHT40[i].tPow2x[iy])/2.0;
				} else {
					value[iv++] = ((double)Ar9300EepromStructGet()->calTargetPower5GHT40[i].tPow2x[iy])/2.0;
				}
				*num = iMaxPier;
			}
		} else {
			if (iBand==band_BG) {
				value[0] = ((double)Ar9300EepromStructGet()->calTargetPower2GHT40[ix].tPow2x[iy])/2.0;
			} else {
				value[0] = ((double)Ar9300EepromStructGet()->calTargetPower5GHT40[ix].tPow2x[iy])/2.0;
			}
			*num = 1;
		}
	}
    return VALUE_OK; 
}

A_INT32 Ar9300ctlIndexGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, iv=0, iMaxCtl;
	int val;
	if (iBand==band_BG) 
		iMaxCtl=OSPREY_NUM_CTLS_2G;
	else
		iMaxCtl=OSPREY_NUM_CTLS_5G;
	if (ix<0 || ix>iMaxCtl) {
		for (i=0; i<iMaxCtl; i++) {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->ctlIndex_2G[i];
			} else {
				val = Ar9300EepromStructGet()->ctlIndex_5G[i];
			}
			value[i] = val;
		}
		*num = iMaxCtl;
	} else {
		if (iBand==band_BG) {
			val = Ar9300EepromStructGet()->ctlIndex_2G[ix];
		} else {
			val = Ar9300EepromStructGet()->ctlIndex_5G[ix];
		}
		value[0] = val;
		*num = 1;
	}	
    return VALUE_OK; 
}

A_INT32 Ar9300ctlFreqGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iv=0, iMaxCtl, jMaxEdge;
	int val;
	if (iBand==band_BG) {
		jMaxEdge=OSPREY_NUM_BAND_EDGES_2G;
		iMaxCtl=OSPREY_NUM_CTLS_2G;
	} else {
		jMaxEdge=OSPREY_NUM_BAND_EDGES_5G;
		iMaxCtl=OSPREY_NUM_CTLS_5G;
	}
	if (iy<0 || iy>=jMaxEdge) {
		if (ix<0 || ix>=iMaxCtl) {
			// get all i, all j
			for (i=0; i<iMaxCtl; i++) {
				for (j=0; j<jMaxEdge; j++) {
					if (iBand==band_BG) {
						val = Ar9300EepromStructGet()->ctl_freqbin_2G[i][j];
					} else {
						val = Ar9300EepromStructGet()->ctl_freqbin_5G[i][j];
					}
					value[iv++]=setFBIN2FREQ(val, iBand);
				}
			}
			*num = jMaxEdge*iMaxCtl;
		} else { // get all j for ix chain
				for (j=0; j<jMaxEdge; j++) {
					if (iBand==band_BG) {
						val = Ar9300EepromStructGet()->ctl_freqbin_2G[ix][j];
					} else {
						val = Ar9300EepromStructGet()->ctl_freqbin_5G[ix][j];
					}
					value[iv++]=setFBIN2FREQ(val, iBand);
				}
				*num = jMaxEdge;
		}
	} else {
		if (ix<0 || ix>=iMaxCtl) {
			for (i=0; i<iMaxCtl; i++) {
				if (iBand==band_BG) {
					val = Ar9300EepromStructGet()->ctl_freqbin_2G[i][iy];
				} else {
					val = Ar9300EepromStructGet()->ctl_freqbin_5G[i][iy];
				}
				value[iv++]=setFBIN2FREQ(val, iBand);
				*num = iMaxCtl;
			}
		} else {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->ctl_freqbin_2G[ix][iy];
			} else {
				val = Ar9300EepromStructGet()->ctl_freqbin_5G[ix][iy];
			}
			value[0]=setFBIN2FREQ(val, iBand);
			*num = 1;
		}
	}
    return VALUE_OK; 
}
A_INT32 Ar9300ctlPowerGet(double *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iv=0, iMaxCtl, jMaxEdge;
	int val;
	if (iBand==band_BG) {
		jMaxEdge=OSPREY_NUM_BAND_EDGES_2G;
		iMaxCtl=OSPREY_NUM_CTLS_2G;
	} else {
		jMaxEdge=OSPREY_NUM_BAND_EDGES_5G;
		iMaxCtl=OSPREY_NUM_CTLS_5G;
	}
	if (iy<0 || iy>=jMaxEdge) {
		if (ix<0 || ix>=iMaxCtl) {
			// get all i, all j
			for (i=0; i<iMaxCtl; i++) {
				for (j=0; j<jMaxEdge; j++) {
					if (iBand==band_BG) {
						val = Ar9300EepromStructGet()->ctlPowerData_2G[i].ctlEdges[j].tPower;
					} else {
						val = Ar9300EepromStructGet()->ctlPowerData_5G[i].ctlEdges[j].tPower;
					}
					value[iv++]=val/2.0;
				}
			}
			*num = jMaxEdge*iMaxCtl;
		} else { // get all j for ix chain
				for (j=0; j<jMaxEdge; j++) {
					if (iBand==band_BG) {
						val = Ar9300EepromStructGet()->ctlPowerData_2G[ix].ctlEdges[j].tPower;
					} else {
						val = Ar9300EepromStructGet()->ctlPowerData_5G[ix].ctlEdges[j].tPower;
					}
					value[iv++]=val/2.0;
				}
				*num = jMaxEdge;
		}
	} else {
		if (ix<0 || ix>=iMaxCtl) {
			for (i=0; i<iMaxCtl; i++) {
				if (iBand==band_BG) {
					val = Ar9300EepromStructGet()->ctlPowerData_2G[i].ctlEdges[iy].tPower;
				} else {
					val = Ar9300EepromStructGet()->ctlPowerData_5G[i].ctlEdges[iy].tPower;
				}
				value[iv++]=val/2.0;
				*num = iMaxCtl;
			}
		} else {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->ctlPowerData_2G[ix].ctlEdges[iy].tPower;
			} else {
				val = Ar9300EepromStructGet()->ctlPowerData_5G[ix].ctlEdges[iy].tPower;
			}
			value[0]=val/2.0;
			*num = 1;
		}
	}
    return VALUE_OK; 
}
A_INT32 Ar9300ctlFlagGet(int *value, int ix, int iy, int iz, int *num, int iBand)
{
	int i, j, iv=0, iMaxCtl, jMaxEdge;
	int val;
	if (iBand==band_BG) {
		jMaxEdge=OSPREY_NUM_BAND_EDGES_2G;
		iMaxCtl=OSPREY_NUM_CTLS_2G;
	} else {
		jMaxEdge=OSPREY_NUM_BAND_EDGES_5G;
		iMaxCtl=OSPREY_NUM_CTLS_5G;
	}
	if (iy<0 || iy>=jMaxEdge) {
		if (ix<0 || ix>=iMaxCtl) {
			// get all i, all j
			for (i=0; i<iMaxCtl; i++) {
				for (j=0; j<jMaxEdge; j++) {
					if (iBand==band_BG) {
						val = Ar9300EepromStructGet()->ctlPowerData_2G[i].ctlEdges[j].flag;
					} else {
						val = Ar9300EepromStructGet()->ctlPowerData_5G[i].ctlEdges[j].flag;
					}
					value[iv++]=val;
				}
			}
			*num = jMaxEdge*iMaxCtl;
		} else { // get all j for ix chain
				for (j=0; j<jMaxEdge; j++) {
					if (iBand==band_BG) {
						val = Ar9300EepromStructGet()->ctlPowerData_2G[ix].ctlEdges[j].flag;
					} else {
						val = Ar9300EepromStructGet()->ctlPowerData_5G[ix].ctlEdges[j].flag;
					}
					value[iv++]=val;
				}
				*num = jMaxEdge;
		}
	} else {
		if (ix<0 || ix>=iMaxCtl) {
			for (i=0; i<iMaxCtl; i++) {
				if (iBand==band_BG) {
					val = Ar9300EepromStructGet()->ctlPowerData_2G[i].ctlEdges[iy].flag;
				} else {
					val = Ar9300EepromStructGet()->ctlPowerData_5G[i].ctlEdges[iy].flag;
				}
				value[iv++]=val;
				*num = iMaxCtl;
			}
		} else {
			if (iBand==band_BG) {
				val = Ar9300EepromStructGet()->ctlPowerData_2G[ix].ctlEdges[iy].flag;
			} else {
				val = Ar9300EepromStructGet()->ctlPowerData_5G[ix].ctlEdges[iy].flag;
			}
			value[0]=val;
			*num = 1;
		}
	}	
    return VALUE_OK; 
}

void Ar9300EepromPaPredistortionSet(int value)
{
	if(value)
	{
	    Ar9300EepromStructGet()->baseEepHeader.featureEnable |= (1<<5);
	}
	else
	{
	    Ar9300EepromStructGet()->baseEepHeader.featureEnable &= ~(1<<5);
	}
}

int Ar9300EepromPaPredistortionGet(void)
{
	return ((Ar9300EepromStructGet()->baseEepHeader.featureEnable>>5)&1);
}

int Ar9300EepromCalibrationValid(void)
{
	int ic, ip;

	for(ic=0; ic<OSPREY_MAX_CHAINS; ic++)
	{
		for(ip=0; ip<OSPREY_NUM_2G_CAL_PIERS; ip++)
		{
			if(Ar9300EepromStructGet()->calPierData2G[ic][ip].refPower!=0 && Ar9300EepromStructGet()->calPierData2G[ic][ip].tempMeas!=0)
			{
				return 1;
			}
		}
	}
	for(ic=0; ic<OSPREY_MAX_CHAINS; ic++)
	{
		for(ip=0; ip<OSPREY_NUM_5G_CAL_PIERS; ip++)
		{
			if(Ar9300EepromStructGet()->calPierData5G[ic][ip].refPower!=0 && Ar9300EepromStructGet()->calPierData5G[ic][ip].tempMeas!=0)
			{
				return 1;
			}
		}
	}
    return 0;
}


