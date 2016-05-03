/*
 * Copyright (c) 2002-2006 Atheros Communications, Inc.
 * All rights reserved.
 *
 */

// "ACI $Id: //depot/sw/branches/narthal_mar2011/dk/mdk/devlib/ar9300/mEepStruct9300.h#2 $, $Header: //depot/sw/branches/narthal_mar2011/dk/mdk/devlib/ar9300/mEepStruct9300.h#2 $"

#ifndef _OSPREY_EEPROM_STRUCT_H_
#define _OSPREY_EEPROM_STRUCT_H_


#ifdef UNUSEDHAL
extern A_UINT8 Ar9300EepromGetLegacyTrgtPwr(A_UINT16 rateIndex, int freq, A_BOOL is2GHz);
extern A_UINT8 Ar9300EepromGetHT20TrgtPwr(A_UINT16 rateIndex, int freq, A_BOOL is2GHz);
extern A_UINT8 Ar9300EepromGetHT40TrgtPwr(A_UINT16 rateIndex, int freq, A_BOOL is2GHz);
extern A_UINT8 Ar9300EepromGetCckTrgtPwr(A_UINT16 rateIndex, int freq);
#endif

extern A_INT32 Ar9300MacAdressGet(unsigned char *mac);
extern A_INT32 Ar9300CustomerDataGet(A_UCHAR *data, A_INT32 len);

#ifdef HALUNUSED
#if defined(WIN32) || defined(WIN64)
#pragma pack (push, 1)
#endif

#define OSPREY_EEP_VER               0xD000
#define OSPREY_EEP_VER_MINOR_MASK    0xFFF
#define OSPREY_EEP_MINOR_VER_1       0x1
#define OSPREY_EEP_MINOR_VER         OSPREY_EEP_MINOR_VER_1

// 16-bit offset location start of calibration struct
#define OSPREY_EEP_START_LOC         256
#define OSPREY_NUM_5G_CAL_PIERS      8
#define OSPREY_NUM_2G_CAL_PIERS      3
#define OSPREY_NUM_5G_20_TARGET_POWERS  8
#define OSPREY_NUM_5G_40_TARGET_POWERS  8
#define OSPREY_NUM_2G_CCK_TARGET_POWERS 2
#define OSPREY_NUM_2G_20_TARGET_POWERS  3
#define OSPREY_NUM_2G_40_TARGET_POWERS  3
//#define OSPREY_NUM_CTLS              21
#define OSPREY_NUM_CTLS_5G           9
#define OSPREY_NUM_CTLS_2G           12
#define OSPREY_CTL_MODE_M            0xF
#define OSPREY_NUM_BAND_EDGES_5G     8
#define OSPREY_NUM_BAND_EDGES_2G     4
#define OSPREY_NUM_PD_GAINS          4
#define OSPREY_PD_GAINS_IN_MASK      4
#define OSPREY_PD_GAIN_ICEPTS        5
#define OSPREY_EEPROM_MODAL_SPURS    5
#define OSPREY_MAX_RATE_POWER        63
#define OSPREY_NUM_PDADC_VALUES      128
#define OSPREY_NUM_RATES             16
#define OSPREY_BCHAN_UNUSED          0xFF
#define OSPREY_MAX_PWR_RANGE_IN_HALF_DB 64
#define OSPREY_OPFLAGS_11A           0x01
#define OSPREY_OPFLAGS_11G           0x02
#define OSPREY_OPFLAGS_5G_HT40       0x04
#define OSPREY_OPFLAGS_2G_HT40       0x08
#define OSPREY_OPFLAGS_5G_HT20       0x10
#define OSPREY_OPFLAGS_2G_HT20       0x20
#define OSPREY_EEPMISC_BIG_ENDIAN    0x01
#define OSPREY_EEPMISC_WOW           0x02
#define OSPREY_CUSTOMER_DATA_SIZE    20

#define FREQ2FBIN(x,y) ((y) ? ((x) - 2300) : (((x) - 4800) / 5))
#define FBIN2FREQ(x,y) ((y) ? (2300 + x) : (4800 + 5 * x))
#define OSPREY_MAX_CHAINS            3
#define OSPREY_ANT_16S               25
#define OSPREY_FUTURE_MODAL_SZ       6

#define OSPREY_NUM_ANT_CHAIN_FIELDS     7
#define OSPREY_NUM_ANT_COMMON_FIELDS    4
#define OSPREY_SIZE_ANT_CHAIN_FIELD     3
#define OSPREY_SIZE_ANT_COMMON_FIELD    4
#define OSPREY_ANT_CHAIN_MASK           0x7
#define OSPREY_ANT_COMMON_MASK          0xf
#define OSPREY_CHAIN_0_IDX              0
#define OSPREY_CHAIN_1_IDX              1
#define OSPREY_CHAIN_2_IDX              2

#define AR928X_NUM_ANT_CHAIN_FIELDS     6
#define AR928X_SIZE_ANT_CHAIN_FIELD     2
#define AR928X_ANT_CHAIN_MASK           0x3

/* Delta from which to start power to pdadc table */
/* This offset is used in both open loop and closed loop power control
 * schemes. In open loop power control, it is not really needed, but for
 * the "sake of consistency" it was kept.
 * For certain AP designs, this value is overwritten by the value in the flag
 * "pwrTableOffset" just before writing the pdadc vs pwr into the chip registers.
 */
#define OSPREY_PWR_TABLE_OFFSET  0

//enable flags for voltage and temp compensation
#define ENABLE_TEMP_COMPENSATION 0x01
#define ENABLE_VOLT_COMPENSATION 0x02

typedef struct eepFlags {
    A_UINT8  opFlags;
    A_UINT8  eepMisc;
} __ATTRIB_PACK EEP_FLAGS;

typedef enum targetPowerHTRates {
    HT_TARGET_RATE_0_8_16,
    HT_TARGET_RATE_1_3_9_11_17_19,
    HT_TARGET_RATE_4,
    HT_TARGET_RATE_5,
    HT_TARGET_RATE_6,
    HT_TARGET_RATE_7,
    HT_TARGET_RATE_12,
    HT_TARGET_RATE_13,
    HT_TARGET_RATE_14,
    HT_TARGET_RATE_15,
    HT_TARGET_RATE_20,
    HT_TARGET_RATE_21,
    HT_TARGET_RATE_22,
    HT_TARGET_RATE_23
};

#endif


    const static char *sRatePrintHT[24] = 
	{
		"MCS0 ",
		"MCS1 ", 
		"MCS2 ",
		"MCS3 ",
		"MCS4 ",
		"MCS5 ",
		"MCS6 ",
		"MCS7 ",
		"MCS8 ",
		"MCS9 ",
		"MCS10",
		"MCS11",
		"MCS12",
		"MCS13",
		"MCS14",
		"MCS15",
		"MCS16",
		"MCS17",
		"MCS18",
		"MCS19",
		"MCS20",
		"MCS21",
		"MCS22",
		"MCS23"
	};

#ifdef HALUNUSED

	const static int mapRate2Index[24]=
	{
		0,1,1,1,2,
		3,4,5,0,1,
		1,1,6,7,8,
		9,0,1,1,1,
		10,11,12,13
	};

typedef enum targetPowerLegacyRates {
    LEGACY_TARGET_RATE_6_24,
    LEGACY_TARGET_RATE_36,
    LEGACY_TARGET_RATE_48,
    LEGACY_TARGET_RATE_54
};

#endif

    const static char *sRatePrintLegacy[4] = 
	{
		"6-24",
		" 36 ", 
		" 48 ", 
		" 54 "
	};

#ifdef HALUNUSED


typedef enum targetPowerCckRates {
    LEGACY_TARGET_RATE_1L_5L,
    LEGACY_TARGET_RATE_5S,
    LEGACY_TARGET_RATE_11L,
    LEGACY_TARGET_RATE_11S
};

#endif

    const static char *sRatePrintCck[4] = 
	{
		"1L-5L",
		" 5S  ", 
		" 11L ", 
		" 11S "
	};


#ifdef HALUNUSED

    const static char *sRatePrint[3][8] = {
      {"     6-24     ", "      36      ", "      48      ", "      54      ", "", "", "", ""},
      {"       1      ", "       2      ", "     5.5      ", "      11      ", "", "", "", ""},
      {"   HT MCS0    ", "   HT MCS1    ", "   HT MCS2    ", "   HT MCS3    ",
       "   HT MCS4    ", "   HT MCS5    ", "   HT MCS6    ", "   HT MCS7    "},
    };

    const static char *sTargetPowerMode[7] = {
      "5G OFDM ", "5G HT20 ", "5G HT40 ", "2G CCK  ", "2G OFDM ", "2G HT20 ", "2G HT40 ",
    };

#endif

    const static char *sDeviceType[] = {
      "UNKNOWN",
      "Cardbus",
      "PCI    ",
      "MiniPCI",
      "AP     ",
      "PCIE   ",
      "UNKNOWN",
      "UNKNOWN",
    };

    const static char *sCtlType[] = {
        "[ 11A base mode ]",
        "[ 11B base mode ]",
        "[ 11G base mode ]",
        "[ INVALID       ]",
        "[ INVALID       ]",
        "[ 2G HT20 mode  ]",
        "[ 5G HT20 mode  ]",
        "[ 2G HT40 mode  ]",
        "[ 5G HT40 mode  ]",
    };

#ifdef HALUNUSED

#define OSPREY_CHECKSUM_LOCATION (OSPREY_EEP_START_LOC + 1)
//
typedef struct osprey_BaseEepHeader {
    A_UINT16  regDmn[2]; //Does this need to be outside of this structure, if it gets written after calibration
    A_UINT8   txrxMask;  //4 bits tx and 4 bits rx
    EEP_FLAGS opCapFlags;
    A_UINT8   rfSilent;
    A_UINT8   blueToothOptions;
    A_UINT8   deviceCap;
    A_UINT8   deviceType; // takes lower byte in eeprom location
    A_INT8    pwrTableOffset; // offset in dB to be added to beginning of pdadc table in calibration
	A_UINT8   params_for_tuning_caps[2];  //placeholder, get more details from Don
    A_UINT8   featureEnable; //bit0 - enable tx temp comp 
                             //bit1 - enable tx volt comp
                             //bit2 - enable fastClock - default to 1
                             //bit3 - enable doubling - default to 1
    A_UINT8   miscConfiguration; //misc flags: bit0 - turn down drivestrength
    A_UINT8   futureBase[9];
} __ATTRIB_PACK OSPREY_BASE_EEP_HEADER;



typedef struct spurChanStruct {
    A_UINT16 spurChan;
    A_UINT8  spurRangeLow;
    A_UINT8  spurRangeHigh;
} __ATTRIB_PACK SPUR_CHAN;


//Note the order of the fields in this structure has been optimized to put all fields likely to change together
typedef struct ospreyModalEepHeader {
    A_UINT32  antCtrlCommon;                         // 4   idle, t1, t2, b (4 bits per setting)
    A_UINT32  antCtrlCommon2;                        // 4    ra1l1, ra2l1, ra1l2, ra2l2, ra12
    A_UINT16  antCtrlChain[OSPREY_MAX_CHAINS];       // 6   idle, t, r, rx1, rx12, b (2 bits each)
    A_UINT8   xatten1DB[OSPREY_MAX_CHAINS];           // 3  //xatten1_db for merlin (0xa20c/b20c 5:0)
    A_UINT8   xatten1Margin[OSPREY_MAX_CHAINS];          // 3  //xatten1_margin for merlin (0xa20c/b20c 16:12
	A_INT8    tempSlope;
	A_INT8    voltSlope;
    A_UINT8 spurChans[OSPREY_EEPROM_MODAL_SPURS];  // spur channels in usual fbin coding format
    A_INT8    noiseFloorThreshCh[OSPREY_MAX_CHAINS]; // 3    //Check if the register is per chain
    A_UINT8   ob[OSPREY_MAX_CHAINS];                 // 3 
    A_UINT8   db_stage2[OSPREY_MAX_CHAINS];          // 3 
    A_UINT8   db_stage3[OSPREY_MAX_CHAINS];          // 3 
    A_UINT8   db_stage4[OSPREY_MAX_CHAINS];          // 3 
    A_UINT8   xpaBiasLvl;                            // 1
    A_UINT8   txFrameToDataStart;                    // 1
    A_UINT8   txFrameToPaOn;                         // 1
    A_UINT8   txClip;                                     // 4 bits tx_clip, 4 bits dac_scale_cck
    A_INT8    antennaGain;                           // 1
    A_UINT8   switchSettling;                        // 1
    A_INT8    adcDesiredSize;                        // 1
    A_UINT8   txEndToXpaOff;                         // 1
    A_UINT8   txEndToRxOn;                           // 1
    A_UINT8   txFrameToXpaOn;                        // 1
    A_UINT8   thresh62;                              // 1
    A_UINT8    futureModal[32];
} __ATTRIB_PACK OSPREY_MODAL_EEP_HEADER;                    // == 100 B

typedef struct ospCalDataPerFreqOpLoop {
    A_INT8 refPower;    /*   */
    A_UINT8 voltMeas; /* pdadc voltage at power measurement */
    A_UINT8 tempMeas;  /* pcdac used for power measurement   */
    A_UINT8 rxNoisefloorCal; /*range is -60 to -127 create a mapping equation 1db resolution */
    A_UINT8 rxNoisefloorPower; /*range is same as noisefloor */
    A_UINT8 rxTempMeas; /*temp measured when noisefloor cal was performed */
} __ATTRIB_PACK OSP_CAL_DATA_PER_FREQ_OP_LOOP;

typedef struct CalTargetPowerLegacy {
    A_UINT8  tPow2x[4];
} __ATTRIB_PACK CAL_TARGET_POWER_LEG;

typedef struct ospCalTargetPowerHt {
    A_UINT8  tPow2x[14];
} __ATTRIB_PACK OSP_CAL_TARGET_POWER_HT;

#ifdef ARCH_BIG_ENDIAN
    typedef struct CalCtlEdgePwr {
        A_UINT8  flag    :2,
                 tPower  :6;
    } __ATTRIB_PACK CAL_CTL_EDGE_PWR;
#else
    typedef struct CalCtlEdgePwr {
        A_UINT8  tPower :6,
                 flag   :2;
    } __ATTRIB_PACK CAL_CTL_EDGE_PWR;
#endif

typedef struct ospCalCtlData_5G {
    CAL_CTL_EDGE_PWR  ctlEdges[OSPREY_NUM_BAND_EDGES_5G];
} __ATTRIB_PACK OSP_CAL_CTL_DATA_5G;

typedef struct ospCalCtlData_2G {
    CAL_CTL_EDGE_PWR  ctlEdges[OSPREY_NUM_BAND_EDGES_2G];
} __ATTRIB_PACK OSP_CAL_CTL_DATA_2G;


typedef struct ospreyEeprom {
    A_UINT8  eepromVersion;
    A_UINT8  templateVersion;
    A_UINT8  macAddr[6];
    A_UINT8  custData[OSPREY_CUSTOMER_DATA_SIZE];

    OSPREY_BASE_EEP_HEADER    baseEepHeader;

    OSPREY_MODAL_EEP_HEADER   modalHeader2G;
    A_UINT8            calFreqPier2G[OSPREY_NUM_2G_CAL_PIERS];
    OSP_CAL_DATA_PER_FREQ_OP_LOOP calPierData2G[OSPREY_MAX_CHAINS][OSPREY_NUM_2G_CAL_PIERS];
	A_UINT8 calTarget_freqbin_Cck[OSPREY_NUM_2G_CCK_TARGET_POWERS];
    A_UINT8 calTarget_freqbin_2G[OSPREY_NUM_2G_20_TARGET_POWERS];
    A_UINT8 calTarget_freqbin_2GHT20[OSPREY_NUM_2G_20_TARGET_POWERS];
    A_UINT8 calTarget_freqbin_2GHT40[OSPREY_NUM_2G_40_TARGET_POWERS];
    CAL_TARGET_POWER_LEG calTargetPowerCck[OSPREY_NUM_2G_CCK_TARGET_POWERS];
    CAL_TARGET_POWER_LEG calTargetPower2G[OSPREY_NUM_2G_20_TARGET_POWERS];
    OSP_CAL_TARGET_POWER_HT  calTargetPower2GHT20[OSPREY_NUM_2G_20_TARGET_POWERS];
    OSP_CAL_TARGET_POWER_HT  calTargetPower2GHT40[OSPREY_NUM_2G_40_TARGET_POWERS];
    A_UINT8   ctlIndex_2G[OSPREY_NUM_CTLS_2G];
    A_UINT8   ctl_freqbin_2G[OSPREY_NUM_CTLS_2G][OSPREY_NUM_BAND_EDGES_2G];
    OSP_CAL_CTL_DATA_2G   ctlPowerData_2G[OSPREY_NUM_CTLS_2G];


    OSPREY_MODAL_EEP_HEADER   modalHeader5G;
    A_UINT8            calFreqPier5G[OSPREY_NUM_5G_CAL_PIERS];
    OSP_CAL_DATA_PER_FREQ_OP_LOOP calPierData5G[OSPREY_MAX_CHAINS][OSPREY_NUM_5G_CAL_PIERS];
    A_UINT8 calTarget_freqbin_5G[OSPREY_NUM_5G_20_TARGET_POWERS];
    A_UINT8 calTarget_freqbin_5GHT20[OSPREY_NUM_5G_20_TARGET_POWERS];
    A_UINT8 calTarget_freqbin_5GHT40[OSPREY_NUM_5G_40_TARGET_POWERS];
    CAL_TARGET_POWER_LEG calTargetPower5G[OSPREY_NUM_5G_20_TARGET_POWERS];
    OSP_CAL_TARGET_POWER_HT  calTargetPower5GHT20[OSPREY_NUM_5G_20_TARGET_POWERS];
    OSP_CAL_TARGET_POWER_HT  calTargetPower5GHT40[OSPREY_NUM_5G_40_TARGET_POWERS];
    A_UINT8   ctlIndex_5G[OSPREY_NUM_CTLS_5G];
    A_UINT8   ctl_freqbin_5G[OSPREY_NUM_CTLS_5G][OSPREY_NUM_BAND_EDGES_5G];
    OSP_CAL_CTL_DATA_5G   ctlPowerData_5G[OSPREY_NUM_CTLS_5G];
} __ATTRIB_PACK OSPREY_EEPROM;


#if defined(WIN32) || defined(WIN64)
#pragma pack (pop)
#endif


extern A_UINT16 Ar9300EepromStructSize(void);
extern OSPREY_EEPROM *Ar9300EepromStructInit(int defaultIndex);
extern OSPREY_EEPROM *Ar9300EepromStructGet();
extern OSPREY_EEPROM *Ar9300EepromStructDefault(int defaultIndex);
extern int Ar9300EepromUpdateCalPier(int pierIdx, int freq, int chain,
                          int pwrCorrection, int voltMeas, int tempMeas)
extern Ar9300EepromCalPierGet(int mode, int ipier, int ichain,
                       int *pfrequency, int *pcorrection, int *ptemperature, int *pvoltage);

extern void Ar9300EepromDisplayCalData(int for2GHz);
extern void Ar9300EepromDisplayAll(void);
extern void Ar9300SetTargetPowerFromEeprom(A_UINT16 freq);

extern A_INT32 Ar9300EnableTempCompensationGet();
extern A_INT32 Ar9300EnableVoltCompensationGet();
#endif
extern int Ar9300FutureGet(int *value, int ix, int *num, int iBand);
extern int Ar9300AntDivCtrlGet(void);
extern int Ar9300ReconfigMiscGet(void);
extern int Ar9300ReconfigDriveStrengthGet(void);
extern int Ar9300ReconfigQuickDropGet(void);
extern int Ar9300EnableFeatureGet(void);
extern int Ar9300EnableTempCompensationGet(void);
extern int Ar9300EnableVoltCompensationGet(void);
extern int Ar9300EnableFastClockGet(void);
extern int Ar9300EnableDoublingGet(void);
extern int Ar9300InternalRegulatorGet(void);
extern int Ar9300PapdGet(void);
extern int Ar9300EnableTuningCapsGet(void);
extern int Ar9300PapdRateMaskHt20Get(int iBand);
extern int Ar9300PapdRateMaskHt40Get(int iBand);

extern int Ar9300_SWREG_Get(void);

extern int Ar9300eepromVersionGet(void);
extern int Ar9300templateVersionGet(void);
extern int Ar9300regDmnGet(int *value, int ix, int *num);
extern int Ar9300txrxMaskGet(void);
extern int Ar9300txMaskGet(void);
extern int Ar9300rxMaskGet(void);
extern int Ar9300opFlagsGet(void);
extern int Ar9300eepMiscGet(void);
extern int Ar9300rfSilentGet(void);
extern int Ar9300rfSilentB0Get(void);
extern int Ar9300rfSilentB1Get(void);
extern int Ar9300rfSilentGPIOGet(void);
extern int Ar9300blueToothOptionsGet(void);
extern int Ar9300deviceCapGet(void);
extern int Ar9300deviceTypeGet(void);
extern int Ar9300pwrTableOffsetGet(void);
extern int Ar9300pwrTuningCapsParamsGet(int *value, int ix, int *num);
extern int Ar9300TxGainGet(void);
extern int Ar9300RxGainGet(void);

extern int Ar9300TempSlopeGet(int *value, int iBand);
extern int Ar9300TempSlopeLowGet(int *value);
extern int Ar9300TempSlopeHighGet(int *value);
extern int Ar9300VoltSlopeGet(int iBand);
extern int Ar9300QuickDropGet(int iBand);
extern int Ar9300QuickDropLowGet();
extern int Ar9300QuickDropHighGet();
extern int Ar9300xpaBiasLvlGet(int iBand);
extern int Ar9300txFrameToDataStartGet(int iBand);
extern int Ar9300txFrameToPaOnGet(int iBand);
extern int Ar9300txClipGet(int iBand);
extern int Ar9300dac_scale_cckGet(int iBand);
extern int Ar9300antennaGainGet(int iBand);
extern int Ar9300adcDesiredSizeGet(int iBand);
extern int Ar9300switchSettlingGet(int iBand);
extern int Ar9300txEndToXpaOffGet(int iBand);
extern int Ar9300txEndToRxOnGet(int iBand);
extern int Ar9300txFrameToXpaOnGet(int iBand);
extern int Ar9300thresh62Get(int iBand);

//extern A_INT32 Ar9300CalTgtPwrGet(int *pwrArr, int band, int htMode, int iFreqNum);
//extern A_INT32 Ar9300CalTgtFreqGet(int *freqArr, int band, int htMode);

extern A_INT32 Ar9300calFreqTGTcckGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calFreqTGTLegacyOFDMGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calFreqTGTHT20Get(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calFreqTGTHT40Get(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calTGTPwrCCKGet(double *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calTGTPwrLegacyOFDMGet(double *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calTGTPwrHT20Get(double *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calTGTPwrHT40Get(double *value, int ix, int iy, int iz, int *num, int iBand);

extern A_INT32 Ar9300calFreqPierGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calPierDataRefPowerGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calPierDataVoltMeasGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calPierDataTempMeasGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calPierDataRxNoisefloorCalGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calPierDataRxNoisefloorPowerGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300calPierDataRxTempMeasGet(int *value, int ix, int iy, int iz, int *num, int iBand);

extern A_INT32 Ar9300ctlIndexGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300ctlFreqGet(int *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300ctlPowerGet(double *value, int ix, int iy, int iz, int *num, int iBand);
extern A_INT32 Ar9300ctlFlagGet(int *value, int ix, int iy, int iz, int *num, int iBand);

extern A_INT32 Ar9300noiseFloorThreshChGet(int *value, int ix, int *num, int iBand);
extern A_INT32 Ar9300ReservedGet(int *value, int ix, int *num, int iBand);
extern A_INT32 Ar9300spurChansGet(int *value, int ix, int *num, int iBand);
extern int Ar9300AntCtrlCommonGet(int iBand);
extern int Ar9300AntCtrlCommon2Get(int iBand);
extern A_INT32 Ar9300antCtrlChainGet(int *value, int ix, int *num, int iBand);
extern A_INT32 Ar9300xatten1DBGet(int *value, int ix, int *num, int iBand);
extern A_INT32 Ar9300xatten1MarginGet(int *value, int ix, int *num, int iBand);
extern A_INT32 Ar9300xatten1DBLowGet(int *value, int ix, int *num, int iBand);
extern A_INT32 Ar9300xatten1MarginLowGet(int *value, int ix, int *num, int iBand);
extern A_INT32 Ar9300xatten1DBHighGet(int *value, int ix, int *num, int iBand);
extern A_INT32 Ar9300xatten1MarginHighGet(int *value, int ix, int *num, int iBand);

extern void Ar9300EepromPaPredistortionSet(int value);
extern int Ar9300EepromPaPredistortionGet(void);
extern int Ar9300EepromCalibrationValid(void);

#endif //_OSPREY_EEPROM_STRUCT_H_
