
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "wlantype.h"
#include "NewArt.h"
#include "smatch.h"
#include "ErrorPrint.h"
#include "NartError.h"

#include "ah.h"
#include "ah_internal.h"
#include "ar9300eep.h"

#include "mEepStruct9300.h"
#include "Ar9300EepromStructSet.h"

#define MBUFFER 1024

void print9300BaseHeader(int client, const OSPREY_BASE_EEP_HEADER    *pBaseEepHeader)
{
	char  buffer[MBUFFER];

	SformatOutput(buffer, MBUFFER-1, " | RegDomain 1              0x%04X   |  RegDomain 2             0x%04X   |",
		pBaseEepHeader->regDmn[0],
		pBaseEepHeader->regDmn[1]
		);
	ErrorPrint(NartOther, buffer);

	SformatOutput(buffer, MBUFFER-1, " | TX Mask                  0x%X      |  RX Mask                 0x%X      |",
		(pBaseEepHeader->txrxMask&0xF0)>>4,
		pBaseEepHeader->txrxMask&0x0F
		);
	ErrorPrint(NartOther, buffer);
	SformatOutput(buffer, MBUFFER-1," | OpFlags: 5GHz            %d        |  2GHz                    %d        |",
             (pBaseEepHeader->opCapFlags.opFlags & OSPREY_OPFLAGS_11A) || 0,
			 (pBaseEepHeader->opCapFlags.opFlags & OSPREY_OPFLAGS_11G) || 0
			 );
 	ErrorPrint(NartOther, buffer);
	SformatOutput(buffer, MBUFFER-1," | Disable 5G HT20          %d        |  Disable 2G HT20         %d        |",
		      (pBaseEepHeader->opCapFlags.opFlags & OSPREY_OPFLAGS_5G_HT20) || 0,
			  (pBaseEepHeader->opCapFlags.opFlags & OSPREY_OPFLAGS_2G_HT20) || 0
				);
    ErrorPrint(NartOther, buffer);

	SformatOutput(buffer, MBUFFER-1," | Disable 5G HT40          %d        |  Disable 2G HT40         %d        |",
		      (pBaseEepHeader->opCapFlags.opFlags & OSPREY_OPFLAGS_5G_HT40) || 0,
			  (pBaseEepHeader->opCapFlags.opFlags & OSPREY_OPFLAGS_2G_HT40) || 0
				);
    ErrorPrint(NartOther, buffer);


    SformatOutput(buffer, MBUFFER-1," | Big Endian               %d        |  Wake On Wireless        %d        |",
			 (pBaseEepHeader->opCapFlags.eepMisc & OSPREY_EEPMISC_BIG_ENDIAN) || 0, (pBaseEepHeader->opCapFlags.eepMisc & OSPREY_EEPMISC_WOW) || 0);
	ErrorPrint(NartOther, buffer);

	SformatOutput(buffer, MBUFFER-1, " | RF Silent                0x%X      |  Bluetooth               0x%X      |",
		pBaseEepHeader->rfSilent,
		pBaseEepHeader->blueToothOptions
		);
	ErrorPrint(NartOther, buffer);

	SformatOutput(buffer, MBUFFER-1, " | Device Cap               %d        |  DeviceType              %s  |",
		pBaseEepHeader->deviceCap,
		sDeviceType[pBaseEepHeader->deviceType]
		);
	ErrorPrint(NartOther, buffer);
		SformatOutput(buffer, MBUFFER-1 ," | Tuning Caps(0,1):        (%02x,%02x)  |  Enbl Compensation       0x%02x     |",
		pBaseEepHeader->params_for_tuning_caps[0],
		pBaseEepHeader->params_for_tuning_caps[1],
		pBaseEepHeader->featureEnable
		);
	ErrorPrint(NartOther, buffer);
	SformatOutput(buffer, MBUFFER-1, " | GPIO eepWrite Enable     %d        |  GPIO rxBand Selection   0x%02x     |",
		pBaseEepHeader->eepromWriteEnableGpio, pBaseEepHeader->rxBandSelectGpio);
	ErrorPrint(NartOther, buffer);
	SformatOutput(buffer, MBUFFER-1, " | GPIO wlan Disable        %d        |  GPIO wlan LED           0x%02x     |",
		pBaseEepHeader->wlanDisableGpio, pBaseEepHeader->wlanLedGpio);
	ErrorPrint(NartOther, buffer);
	SformatOutput(buffer, MBUFFER-1, " | txrxGain                 0x%02x     |  pwrTableOffset          %d        |",
		pBaseEepHeader->txrxgain, pBaseEepHeader->pwrTableOffset);
	ErrorPrint(NartOther, buffer);
	SformatOutput(buffer, MBUFFER-1, " | internal regulator     0x%08x                                     |",
		pBaseEepHeader->swreg);
	ErrorPrint(NartOther,buffer);
}



void print9300_5GHzHeader(int client, const OSPREY_MODAL_EEP_HEADER   *pModalHeader, const OSPREY_BASE_EXTENSION_2 *base_ext2)
{
	char  buffer[MBUFFER];

	SformatOutput(buffer, MBUFFER-1," |                                                                       |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," |===========================5GHz Modal Header===========================|");
	ErrorPrint(NartOther, buffer);

	SformatOutput(buffer, MBUFFER-1," |                                                                       |");
	ErrorPrint(NartOther,buffer);

    SformatOutput(buffer, MBUFFER-1," | Antenna Common        0x%08X  |  Antenna Common2       0x%08X |",
        pModalHeader->antCtrlCommon,
		pModalHeader->antCtrlCommon2
		);
	ErrorPrint(NartOther, buffer);

    SformatOutput(buffer, MBUFFER-1," | Ant Chain 0              0x%04X   |  Ant Chain 1             0x%04X   |",
        pModalHeader->antCtrlChain[0],
		pModalHeader->antCtrlChain[1]
		);
	ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | Ant Chain 2              0x%04X   |                                   |",
                 pModalHeader->antCtrlChain[2]);
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1DB Ch0 Low      %2x        |  xatten 1DB Ch1 Low     %2x        |",
                 base_ext2->xatten1DBLow[0],
				 base_ext2->xatten1DBLow[1]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1DB Ch2 Low      %2x        |  xatten 1DB Ch0         %2x        |",
                 base_ext2->xatten1DBLow[2],
				 pModalHeader->xatten1DB[0]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1DB Ch1          %2x        |  xatten 1DB Ch2         %2x        |",
                 pModalHeader->xatten1DB[1],
				 pModalHeader->xatten1DB[2]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1DB Ch0 High     %2x        |  xatten 1DB Ch1 High    %2x        |",
                 base_ext2->xatten1DBHigh[0],
				 base_ext2->xatten1DBHigh[1]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1DB Ch2 high     %2x        |                                   |",
				 base_ext2->xatten1DBHigh[2]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1Margin0 Low     %2x        |  xatten 1Margin1 Low    %2x        |",
                 base_ext2->xatten1MarginLow[0],
				 base_ext2->xatten1MarginLow[1]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1Margin2 Low     %2x        |  xatten 1Margin0        %2x        |",
                 base_ext2->xatten1MarginLow[2],
                 pModalHeader->xatten1Margin[0]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1Margin1         %2x        |  xatten 1Margin2        %2x        |",
                 pModalHeader->xatten1Margin[1],
				 pModalHeader->xatten1Margin[2]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1Margin0 High    %2x        |  xatten 1Margin1 High   %2x        |",
                 base_ext2->xatten1MarginHigh[0],
				 base_ext2->xatten1MarginHigh[1]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1Margin2 High    %2x        |                                   |",
				 base_ext2->xatten1MarginHigh[2]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | TempL Slope            %3d        |  Temp Slope            %3d        |",
				base_ext2->tempSlopeLow,
				 pModalHeader->tempSlope
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | TempH Slope            %3d        |  Volt Slope            %3d        |",
				base_ext2->tempSlopeHigh,
				 pModalHeader->voltSlope
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | Spur Channels:           %04d, %04d, %04d, %04d, %04d                 |",
                 FBIN2FREQ(pModalHeader->spurChans[0],0),
				 FBIN2FREQ(pModalHeader->spurChans[1],0),
				 FBIN2FREQ(pModalHeader->spurChans[2],0),
				 FBIN2FREQ(pModalHeader->spurChans[3],0),
				 FBIN2FREQ(pModalHeader->spurChans[4],0)
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | NoiseFloor Thres0      %3d        |  NoiseFloorThres1      %3d        |",
                 pModalHeader->noiseFloorThreshCh[0],
				 pModalHeader->noiseFloorThreshCh[1]
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | NoiseFloor Thres2      %3d        |                                   |",
                 pModalHeader->noiseFloorThreshCh[2]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xpaBiasLvl              %2x        |  txFrame2DataStart      %2x        |",
                 pModalHeader->xpaBiasLvl,
				 pModalHeader->txFrameToDataStart
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | txFrameToPaOn           %2x        |  txClip                 %2x        |",
                 pModalHeader->txFrameToPaOn,
				 pModalHeader->txClip
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | antennaGain            %3d        |  switchSettling          %2x       |",
                 pModalHeader->antennaGain,
				 pModalHeader->switchSettling
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | adcDesiredSize          %3d       |  txEndToXpaOff          %2x        |",
                 pModalHeader->adcDesiredSize,
				 pModalHeader->txEndToXpaOff
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | txEndToRxOn             %2x        |  txFrameToXpaOn         %2x        |",
                 pModalHeader->txEndToRxOn,
				 pModalHeader->txFrameToXpaOn
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | thresh62                 %2d       |                                   |",
                 pModalHeader->thresh62
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," |                                   |                                   |");
		ErrorPrint(NartOther,buffer);
}



void print9300_2GHzHeader(int client, const OSPREY_MODAL_EEP_HEADER   *pModalHeader)
{
	char  buffer[MBUFFER];


	SformatOutput(buffer, MBUFFER-1," |                                                                       |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," |===========================2GHz Modal Header===========================|");
	ErrorPrint(NartOther, buffer);

	SformatOutput(buffer, MBUFFER-1," |                                                                       |");
	ErrorPrint(NartOther,buffer);

    SformatOutput(buffer, MBUFFER-1," | Antenna Common        0x%08X  |  Antenna Common2       0x%08X |",
        pModalHeader->antCtrlCommon,
		pModalHeader->antCtrlCommon2
		);
	ErrorPrint(NartOther, buffer);

    SformatOutput(buffer, MBUFFER-1," | Ant Chain 0              0x%04X   |  Ant Chain 1             0x%04X   |",
        pModalHeader->antCtrlChain[0],
		pModalHeader->antCtrlChain[1]
		);
	ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | Ant Chain 2              0x%04X   |                                   |",
                 pModalHeader->antCtrlChain[2]);
		ErrorPrint(NartOther, buffer);


        SformatOutput(buffer, MBUFFER-1," | xatten 1DB Ch0          %2x        |  xatten 1DB Ch1         %2x        |",
                 pModalHeader->xatten1DB[0],
				 pModalHeader->xatten1DB[1]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1DB Ch2          %2x        |                                   |",
                 pModalHeader->xatten1DB[2]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1Margin0         %2x        |  xatten 1Margin1        %2x        |",
                 pModalHeader->xatten1Margin[0],
				 pModalHeader->xatten1Margin[1]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xatten 1Margin2         %2x        |                                   |",
                 pModalHeader->xatten1Margin[2]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | Temp Slope             %3d        |  Volt Slope            %3d        |",
                 pModalHeader->tempSlope,
				 pModalHeader->voltSlope
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | Spur Channels:           %04d, %04d, %04d, %04d, %04d                 |",
                 FBIN2FREQ(pModalHeader->spurChans[0],1),
				 FBIN2FREQ(pModalHeader->spurChans[1],1),
				 FBIN2FREQ(pModalHeader->spurChans[2],1),
				 FBIN2FREQ(pModalHeader->spurChans[3],1),
				 FBIN2FREQ(pModalHeader->spurChans[4],1)
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | NoiseFloor Thres0      %3d        |  NoiseFloorThres1      %3d        |",
                 pModalHeader->noiseFloorThreshCh[0],
				 pModalHeader->noiseFloorThreshCh[1]
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | NoiseFloor Thres2      %3d        |                                   |",
                 pModalHeader->noiseFloorThreshCh[2]
				 );
		ErrorPrint(NartOther, buffer);

        SformatOutput(buffer, MBUFFER-1," | xpaBiasLvl              %2x        |  txFrame2DataStart      %2x        |",
                 pModalHeader->xpaBiasLvl,
				 pModalHeader->txFrameToDataStart
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | txFrameToPaOn           %2x        |  txClip                 %2x        |",
                 pModalHeader->txFrameToPaOn,
				 pModalHeader->txClip
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | antennaGain            %3d        |  switchSettling          %2x       |",
                 pModalHeader->antennaGain,
				 pModalHeader->switchSettling
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | adcDesiredSize          %3d       |  txEndToXpaOff          %2x        |",
                 pModalHeader->adcDesiredSize,
				 pModalHeader->txEndToXpaOff
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | txEndToRxOn             %2x        |  txFrameToXpaOn         %2x        |",
                 pModalHeader->txEndToRxOn,
				 pModalHeader->txFrameToXpaOn
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," | thresh62                 %2d       |                                   |",
                 pModalHeader->thresh62
				 );
		ErrorPrint(NartOther, buffer);

		SformatOutput(buffer, MBUFFER-1," |                                   |                                   |");
		ErrorPrint(NartOther,buffer);
}

void print9300_5GHzCalPiers(int client, A_UINT8 *pPiers)
{
	int i=0;
	char  buffer[MBUFFER];
	SformatOutput(buffer, MBUFFER-1,"5G Cal Freqncy Piers:");
	ErrorPrint(NartOther,buffer);
	for(i=0;i<8;i++){
		SformatOutput(buffer, MBUFFER-1,"[%d]: %d",i,FBIN2FREQ(pPiers[i], 0));
		ErrorPrint(NartOther,buffer);
	}
}

void print9300_2GHzCalPiers(int client, A_UINT8 *pPiers)
{
	int i=0;
	char  buffer[MBUFFER];
	SformatOutput(buffer, MBUFFER-1,"2G Cal Freqncy Piers:");
	ErrorPrint(NartOther,buffer);
	for(i=0;i<3;i++){
		SformatOutput(buffer, MBUFFER-1,"[%d]: %d",i,FBIN2FREQ(pPiers[i], 1));
		ErrorPrint(NartOther,buffer);
	}
}

void print9300_5GHzCalData(int client, const ar9300_eeprom_t *p9300)
{
	A_UINT16 numPiers = OSPREY_NUM_5G_CAL_PIERS;
    A_UINT16 pc; //pier count
    char  buffer[MBUFFER];
    const A_UINT8 *pPiers = &p9300->calFreqPier5G[0];
    const OSP_CAL_DATA_PER_FREQ_OP_LOOP *pData = &p9300->calPierData5G[0][0];

	SformatOutput(buffer, MBUFFER-1, " |=================5G Power Calibration Information =====================|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                   Chain 0                          Chain 1            |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                     rxNF rxNF rx  |                   rxNF rxNF   rx  |");
	ErrorPrint(NartOther,buffer);
    SformatOutput(buffer, MBUFFER-1, " | Freq  Pwr Volt Temp  Cal pwr Temp |     Pwr Volt Temp  Cal  pwr  Temp |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
    for(pc = 0; pc < numPiers; pc++) {
        SformatOutput(buffer, MBUFFER-1, " | %04d %4d %3d  %3d  %04d %4d %3d |    %4d %3d  %3d  %04d %4d  %3d  |",
                FBIN2FREQ(pPiers[pc], 0),
			pData->refPower, pData->voltMeas, pData->tempMeas, pData->rxNoisefloorCal, pData->rxNoisefloorPower, pData->rxTempMeas,
				(pData+1*OSPREY_NUM_5G_CAL_PIERS)->refPower, (pData+1*OSPREY_NUM_5G_CAL_PIERS)->voltMeas, (pData+1*OSPREY_NUM_5G_CAL_PIERS)->tempMeas,
			(pData+1*OSPREY_NUM_5G_CAL_PIERS)->rxNoisefloorCal, (pData+1*OSPREY_NUM_5G_CAL_PIERS)->rxNoisefloorPower, (pData+1*OSPREY_NUM_5G_CAL_PIERS)->rxTempMeas
			);
		ErrorPrint(NartOther,buffer);
        pData++;
	}
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                   Chain 2                                             |");
	ErrorPrint(NartOther,buffer);	
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
	pData = &p9300->calPierData5G[0][0];
	for(pc = 0; pc < numPiers; pc++) {
        SformatOutput(buffer, MBUFFER-1, " | %04d %4d %3d  %3d  %04d %4d %3d |                                   |",
            FBIN2FREQ(pPiers[pc], 0),
			(pData+2*OSPREY_NUM_5G_CAL_PIERS)->refPower, (pData+2*OSPREY_NUM_5G_CAL_PIERS)->voltMeas, (pData+2*OSPREY_NUM_5G_CAL_PIERS)->tempMeas,
			(pData+2*OSPREY_NUM_5G_CAL_PIERS)->rxNoisefloorCal, (pData+2*OSPREY_NUM_5G_CAL_PIERS)->rxNoisefloorPower, (pData+2*OSPREY_NUM_5G_CAL_PIERS)->rxTempMeas
				);
			ErrorPrint(NartOther,buffer);
            pData++;
    }
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);

	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
}

void print9300_2GHzCalData(int client, const ar9300_eeprom_t *p9300)
{
	A_UINT16 numPiers = OSPREY_NUM_2G_CAL_PIERS;
    A_UINT16 pc; //pier count
   char  buffer[MBUFFER];
    const A_UINT8 *pPiers = &p9300->calFreqPier2G[0];
    const OSP_CAL_DATA_PER_FREQ_OP_LOOP *pData = &p9300->calPierData2G[0][0];



	SformatOutput(buffer, MBUFFER-1, " |=================2G Power Calibration Information =====================|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                   Chain 0                          Chain 1            |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                     rxNF rxNF rx  |                   rxNF rxNF   rx  |");
	ErrorPrint(NartOther,buffer);
    SformatOutput(buffer, MBUFFER-1, " | Freq  Pwr Volt Temp  Cal pwr Temp |     Pwr Volt Temp  Cal  pwr  Temp |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
    for(pc = 0; pc < numPiers; pc++) {
        SformatOutput(buffer, MBUFFER-1, " | %04d %4d %3d  %3d  %04d %4d %3d |    %4d %3d  %3d  %04d %4d  %3d  |",
                FBIN2FREQ(pPiers[pc], 1),
			pData->refPower, pData->voltMeas, pData->tempMeas, pData->rxNoisefloorCal, pData->rxNoisefloorPower, pData->rxTempMeas,
				(pData+1*OSPREY_NUM_2G_CAL_PIERS)->refPower, (pData+1*OSPREY_NUM_2G_CAL_PIERS)->voltMeas, (pData+1*OSPREY_NUM_2G_CAL_PIERS)->tempMeas,
			(pData+1*OSPREY_NUM_2G_CAL_PIERS)->rxNoisefloorCal, (pData+1*OSPREY_NUM_2G_CAL_PIERS)->rxNoisefloorPower, (pData+1*OSPREY_NUM_2G_CAL_PIERS)->rxTempMeas
				);
			ErrorPrint(NartOther,buffer);
            pData++;
    }
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                   Chain 2                                             |");
	ErrorPrint(NartOther,buffer);	
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
	pData = &p9300->calPierData2G[0][0];
	for(pc = 0; pc < numPiers; pc++) {
        SformatOutput(buffer, MBUFFER-1, " | %04d %4d %3d  %3d  %04d %4d %3d |                                   |",
            FBIN2FREQ(pPiers[pc], 1),
			(pData+2*OSPREY_NUM_2G_CAL_PIERS)->refPower, (pData+2*OSPREY_NUM_2G_CAL_PIERS)->voltMeas, (pData+2*OSPREY_NUM_2G_CAL_PIERS)->tempMeas,
			(pData+2*OSPREY_NUM_2G_CAL_PIERS)->rxNoisefloorCal, (pData+2*OSPREY_NUM_2G_CAL_PIERS)->rxNoisefloorPower, (pData+2*OSPREY_NUM_2G_CAL_PIERS)->rxTempMeas
				);
			ErrorPrint(NartOther,buffer);
            pData++;
    }
	SformatOutput(buffer, MBUFFER-1, " |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
}

void print9300_5GLegacyTargetPower(int client, const CAL_TARGET_POWER_LEG *pVals, const A_UINT8 *pFreq)
{
	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

	SformatOutput(buffer, MBUFFER-1, " |===========================5G Target Powers============================|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
			SformatOutput(buffer, MBUFFER-1," | OFDM  ");

			for (j = 0; j < 8; j++) {

                    SformatOutput(buffer2, MBUFFER-1,"| %04d  ", FBIN2FREQ(*(pFreq+j),0));
					strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
			ErrorPrint(NartOther, buffer);



            for (j = 0; j < 4; j++) {

					SformatOutput(buffer, MBUFFER-1," | %s  ",sRatePrintLegacy[j]);
					for(i=0;i<8;i++){
						SformatOutput(buffer2, MBUFFER-1,"| %2d.%d  ", pVals[i].tPow2x[j]/2, (pVals[i].tPow2x[j] % 2) * 5);
						strcat(buffer,buffer2);
                }

				strcat(buffer,"|");
				ErrorPrint(NartOther, buffer);

            }
			SformatOutput(buffer, MBUFFER-1," |========================================================================");
			ErrorPrint(NartOther, buffer);
}

void print9300_2GLegacyTargetPower(int client, const CAL_TARGET_POWER_LEG *pVals, const A_UINT8 *pFreq)
{
	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

	SformatOutput(buffer, MBUFFER-1, " |===========================2G Target Powers============================|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
			SformatOutput(buffer, MBUFFER-1," | OFDM   ");

			for (j = 0; j < 3; j++) {

                    SformatOutput(buffer2, MBUFFER-1,"|        %04d        ", FBIN2FREQ(*(pFreq+j),1));
					strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |========|====================|====================|====================|");
			ErrorPrint(NartOther, buffer);



            for (j = 0; j < 4; j++) {

					SformatOutput(buffer, MBUFFER-1," | %s   ",sRatePrintLegacy[j]);
					for(i=0;i<3;i++){
						SformatOutput(buffer2, MBUFFER-1,"|        %2d.%d        ", pVals[i].tPow2x[j]/2, (pVals[i].tPow2x[j] % 2) * 5);
						strcat(buffer,buffer2);
                }

				strcat(buffer,"|");
				ErrorPrint(NartOther, buffer);

            }
			SformatOutput(buffer, MBUFFER-1," |========================================================================");
			ErrorPrint(NartOther, buffer);
}


void print9300_2GCCKTargetPower(int client, const CAL_TARGET_POWER_LEG *pVals, const A_UINT8 *pFreq)
{
	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
			SformatOutput(buffer, MBUFFER-1," | CCK    ");

			for (j = 0; j < 2; j++) {

                    SformatOutput(buffer2, MBUFFER-1,"|        %04d        ", FBIN2FREQ(*(pFreq+j),1));
					strcat(buffer,buffer2);
            }

            strcat(buffer,"|====================|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |========|====================|====================|====================|");
			ErrorPrint(NartOther, buffer);



            for (j = 0; j < 4; j++) {

					SformatOutput(buffer, MBUFFER-1," | %s  ",sRatePrintCck[j]);
					for(i=0;i<2;i++){
						SformatOutput(buffer2, MBUFFER-1,"|        %2d.%d        ", pVals[i].tPow2x[j]/2, (pVals[i].tPow2x[j] % 2) * 5);
						strcat(buffer,buffer2);
                }

				strcat(buffer,"|====================|");
				ErrorPrint(NartOther, buffer);

            }
			SformatOutput(buffer, MBUFFER-1," |========================================================================");
			ErrorPrint(NartOther, buffer);
}


void print9300_5GHT20TargetPower(int client, const OSP_CAL_TARGET_POWER_HT *pVals, const A_UINT8 *pFreq) {

	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
			SformatOutput(buffer, MBUFFER-1," | HT20  ");

			for (j = 0; j < 8; j++) {

                    SformatOutput(buffer2, MBUFFER-1,"| %04d  ", FBIN2FREQ(*(pFreq+j),0));
					strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
			ErrorPrint(NartOther, buffer);



            for (j = 0; j < 24; j++) {

					SformatOutput(buffer, MBUFFER-1," | %s ",sRatePrintHT[j]);
					for(i=0;i<8;i++){
						SformatOutput(buffer2, MBUFFER-1,"| %2d.%d  ", pVals[i].tPow2x[mapRate2Index[j]]/2, (pVals[i].tPow2x[mapRate2Index[j]] % 2) * 5);
						strcat(buffer,buffer2);
                }

				strcat(buffer,"|");
				ErrorPrint(NartOther, buffer);

            }
			SformatOutput(buffer, MBUFFER-1," |========================================================================");
			ErrorPrint(NartOther, buffer);
}


void print9300_5GHT40TargetPower(int client, const OSP_CAL_TARGET_POWER_HT *pVals, const A_UINT8 *pFreq) {

	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
			SformatOutput(buffer, MBUFFER-1," | HT40  ");

			for (j = 0; j < 8; j++) {

                    SformatOutput(buffer2, MBUFFER-1,"| %04d  ", FBIN2FREQ(*(pFreq+j),0));
					strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
			ErrorPrint(NartOther, buffer);



            for (j = 0; j < 24; j++) {

					SformatOutput(buffer, MBUFFER-1," | %s ",sRatePrintHT[j]);
					for(i=0;i<8;i++){
						SformatOutput(buffer2, MBUFFER-1,"| %2d.%d  ", pVals[i].tPow2x[mapRate2Index[j]]/2, (pVals[i].tPow2x[mapRate2Index[j]] % 2) * 5);
						strcat(buffer,buffer2);
                }

				strcat(buffer,"|");
				ErrorPrint(NartOther, buffer);

            }
			SformatOutput(buffer, MBUFFER-1," |========================================================================");
			ErrorPrint(NartOther, buffer);
}

void print9300_2GHT20TargetPower(int client, const OSP_CAL_TARGET_POWER_HT *pVals, const A_UINT8 *pFreq) {

	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
			SformatOutput(buffer, MBUFFER-1," | HT20   ");

			for (j = 0; j < 3; j++) {

                    SformatOutput(buffer2, MBUFFER-1,"|        %04d        ", FBIN2FREQ(*(pFreq+j),1));
					strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |========|====================|====================|====================|");
			ErrorPrint(NartOther, buffer);



            for (j = 0; j < 24; j++) {

					SformatOutput(buffer, MBUFFER-1," | %s  ",sRatePrintHT[j]);
					for(i=0;i<3;i++){
						SformatOutput(buffer2, MBUFFER-1,"|        %2d.%d        ", pVals[i].tPow2x[mapRate2Index[j]]/2, (pVals[i].tPow2x[mapRate2Index[j]] % 2) * 5);
						strcat(buffer,buffer2);
                }

				strcat(buffer,"|");
				ErrorPrint(NartOther, buffer);

            }
			SformatOutput(buffer, MBUFFER-1," |========================================================================");
			ErrorPrint(NartOther, buffer);
}


void print9300_2GHT40TargetPower(int client, const OSP_CAL_TARGET_POWER_HT *pVals, const A_UINT8 *pFreq) {

	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

	SformatOutput(buffer, MBUFFER-1, " |                                                                       |");
	ErrorPrint(NartOther,buffer);
			SformatOutput(buffer, MBUFFER-1," | HT40   ");

			for (j = 0; j < 3; j++) {

                    SformatOutput(buffer2, MBUFFER-1,"|        %04d        ", FBIN2FREQ(*(pFreq+j),1));
					strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |========|====================|====================|====================|");
			ErrorPrint(NartOther, buffer);



            for (j = 0; j < 24; j++) {

					SformatOutput(buffer, MBUFFER-1," | %s  ",sRatePrintHT[j]);
					for(i=0;i<3;i++){
						SformatOutput(buffer2, MBUFFER-1,"|        %2d.%d        ", pVals[i].tPow2x[mapRate2Index[j]]/2, (pVals[i].tPow2x[mapRate2Index[j]] % 2) * 5);
						strcat(buffer,buffer2);
                }

				strcat(buffer,"|");
				ErrorPrint(NartOther, buffer);

            }
			SformatOutput(buffer, MBUFFER-1," |========================================================================");
			ErrorPrint(NartOther, buffer);
}

void print9300_5GCTLIndex(int client, const A_UINT8 *pCtlIndex){
	int i;
	char buffer[MBUFFER];
	SformatOutput(buffer, MBUFFER-1,"5G CTL Index:");
	ErrorPrint(NartOther,buffer);
	for(i=0;i<9;i++){
		SformatOutput(buffer, MBUFFER-1,"[%d] :0x%x",i,pCtlIndex[i]);
		ErrorPrint(NartOther,buffer);
	}
}

void print9300_2GCTLIndex(int client, const A_UINT8 *pCtlIndex){
	int i;
	char buffer[MBUFFER];
	SformatOutput(buffer, MBUFFER-1,"2G CTL Index:");
	ErrorPrint(NartOther,buffer);
	for(i=0;i<12;i++){
		SformatOutput(buffer, MBUFFER-1,"[%d] :0x%x",i,pCtlIndex[i]);
		ErrorPrint(NartOther,buffer);
	}
}

void print9300_5GCTLData(int client, const A_UINT8 *ctlIndex, const OSP_CAL_CTL_DATA_5G Data[OSPREY_NUM_CTLS_5G], const A_UINT8 *pFreq){

	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

	SformatOutput(buffer, MBUFFER-1," |                                                                       |");
	ErrorPrint(NartOther, buffer);

	SformatOutput(buffer, MBUFFER-1," |=======================Test Group Band Edge Power======================|");
	ErrorPrint(NartOther, buffer);
	SformatOutput(buffer, MBUFFER-1," |                                                                       |");
	ErrorPrint(NartOther, buffer);
    for (i = 0; i<OSPREY_NUM_CTLS_5G; i++) {

        SformatOutput(buffer, MBUFFER-1," |                                                                       |");
		ErrorPrint(NartOther, buffer);
        SformatOutput(buffer, MBUFFER-1," | CTL: 0x%02x %s                                           |",
                 ctlIndex[i], sCtlType[ctlIndex[i] & OSPREY_CTL_MODE_M]);
		ErrorPrint(NartOther, buffer);
        SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
		ErrorPrint(NartOther, buffer);
		//FBIN2FREQ(*(pFreq++),0),Data[i].ctlEdges[j].tPower,Data[i].ctlEdges[j].flag

            SformatOutput(buffer, MBUFFER-1," | edge  ");

            for (j = 0; j < OSPREY_NUM_BAND_EDGES_5G; j++) {
                if (*(pFreq+j) == OSPREY_BCHAN_UNUSED) {
                    SformatOutput(buffer2, MBUFFER-1,"|  --   ");
                } else {
                    SformatOutput(buffer2, MBUFFER-1,"| %04d  ", FBIN2FREQ(*(pFreq+j),0));
                }
				strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," | power ");

            for (j = 0; j < OSPREY_NUM_BAND_EDGES_5G; j++) {
                if (*(pFreq+j) == OSPREY_BCHAN_UNUSED) {
                    SformatOutput(buffer2, MBUFFER-1,"|  --   ");
                } else {
                    SformatOutput(buffer2, MBUFFER-1,"| %2d.%d  ", Data[i].ctlEdges[j].tPower / 2,
                        (Data[i].ctlEdges[j].tPower % 2) * 5);
                }
				strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," | flag  ");

            for (j = 0; j < OSPREY_NUM_BAND_EDGES_5G; j++) {
                if (*(pFreq+j) == OSPREY_BCHAN_UNUSED) {
                    SformatOutput(buffer2, MBUFFER-1,"|  --   ");

                } else {
                    SformatOutput(buffer2, MBUFFER-1,"|   %1d   ", Data[i].ctlEdges[j].flag);
                }
				strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

			pFreq+=OSPREY_NUM_BAND_EDGES_5G;

            SformatOutput(buffer, MBUFFER-1," =========================================================================");
			ErrorPrint(NartOther, buffer);

    }
}

void print9300_2GCTLData(int client,  const A_UINT8 *ctlIndex, const OSP_CAL_CTL_DATA_2G Data[OSPREY_NUM_CTLS_2G], const A_UINT8 *pFreq){

	int i,j;
	char buffer[MBUFFER],buffer2[MBUFFER];

    SformatOutput(buffer, MBUFFER-1," |                                                                       |");
	ErrorPrint(NartOther, buffer);

	SformatOutput(buffer, MBUFFER-1," |=======================Test Group Band Edge Power======================|");
	ErrorPrint(NartOther, buffer);
	SformatOutput(buffer, MBUFFER-1," |                                                                       |");
	ErrorPrint(NartOther, buffer);
    for (i = 0; i<OSPREY_NUM_CTLS_2G; i++) {

        SformatOutput(buffer, MBUFFER-1," |                                                                       |");
		ErrorPrint(NartOther, buffer);
        SformatOutput(buffer, MBUFFER-1," | CTL: 0x%02x %s                                           |",
                 ctlIndex[i], sCtlType[ctlIndex[i] & OSPREY_CTL_MODE_M]);
		ErrorPrint(NartOther, buffer);
        SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
		ErrorPrint(NartOther, buffer);
		//FBIN2FREQ(*(pFreq++),0),Data[i].ctlEdges[j].tPower,Data[i].ctlEdges[j].flag

            SformatOutput(buffer, MBUFFER-1," | edge  ");

            for (j = 0; j < OSPREY_NUM_BAND_EDGES_2G; j++) {
                if (*(pFreq+j) == OSPREY_BCHAN_UNUSED) {
                    SformatOutput(buffer2, MBUFFER-1,"|  --   ");
                } else {
                    SformatOutput(buffer2, MBUFFER-1,"| %04d  ", FBIN2FREQ(*(pFreq+j),1));
                }
				strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," | power ");

            for (j = 0; j < OSPREY_NUM_BAND_EDGES_2G; j++) {
                if (*(pFreq+j) == OSPREY_BCHAN_UNUSED) {
                    SformatOutput(buffer2, MBUFFER-1,"|  --   ");
                } else {
                    SformatOutput(buffer2, MBUFFER-1,"| %2d.%d  ", Data[i].ctlEdges[j].tPower / 2,
                        (Data[i].ctlEdges[j].tPower % 2) * 5);
                }
				strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," |=======|=======|=======|=======|=======|=======|=======|=======|=======|");
			ErrorPrint(NartOther, buffer);

            SformatOutput(buffer, MBUFFER-1," | flag  ");

            for (j = 0; j < OSPREY_NUM_BAND_EDGES_2G; j++) {
                if (*(pFreq+j) == OSPREY_BCHAN_UNUSED) {
                    SformatOutput(buffer2, MBUFFER-1,"|  --   ");

                } else {
                    SformatOutput(buffer2, MBUFFER-1,"|   %1d   ", Data[i].ctlEdges[j].flag);
                }
				strcat(buffer,buffer2);
            }

            strcat(buffer,"|");
			ErrorPrint(NartOther, buffer);

			pFreq+=OSPREY_NUM_BAND_EDGES_2G;

            SformatOutput(buffer, MBUFFER-1," =========================================================================");
			ErrorPrint(NartOther, buffer);
	}
}

void print9300Struct(int client, int useDefault)

{
	char buffer[MBUFFER],buffer2[MBUFFER];
	int i;
	const ar9300_eeprom_t *pDefault9300=ar9300EepromStructDefault(2); //calling for default 2

	if(!useDefault){
		pDefault9300=Ar9300EepromStructGet();   // prints the Current EEPROM structure
	}
    SformatOutput(buffer, MBUFFER-1,"start get all");
    ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1,"                         ----------------------                           ");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1,  " =======================| AR9300 CAL STRUCTURE |==========================");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," |                       -----------------------                         |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," |                                   |                                   |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," | CAL Version             %2d        |  Template Version       %2d        |",
		pDefault9300->eepromVersion,
		pDefault9300->templateVersion
		);
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," | MacAddress: 0x%02X:%02X:%02X:%02X:%02X:%02X                                       |",
		  pDefault9300->macAddr[0],pDefault9300->macAddr[1],pDefault9300->macAddr[2],
		  pDefault9300->macAddr[3],pDefault9300->macAddr[4],pDefault9300->macAddr[5]);
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," | Customer Data in hex                                                  |");
	SformatOutput(buffer, MBUFFER-1," |");
	for(i=0;i<OSPREY_CUSTOMER_DATA_SIZE;i++){
		SformatOutput(buffer2, MBUFFER-1," %02X", pDefault9300->custData[i] );
		strcat(buffer,buffer2);
	}
	strcat(buffer,"           |");
	ErrorPrint(NartOther,buffer);
	SformatOutput(buffer, MBUFFER-1," |-----------------------------------------------------------------------|");
	ErrorPrint(NartOther,buffer);


    print9300BaseHeader(client, &pDefault9300->baseEepHeader);

    print9300_5GHzHeader(client, &pDefault9300->modalHeader5G, &pDefault9300->base_ext2);
	print9300_5GHzCalData(client, pDefault9300);

	print9300_5GLegacyTargetPower(client, pDefault9300->calTargetPower5G, pDefault9300->calTarget_freqbin_5G);
    print9300_5GHT20TargetPower(client, pDefault9300->calTargetPower5GHT20, pDefault9300->calTarget_freqbin_5GHT20);
    print9300_5GHT40TargetPower(client, pDefault9300->calTargetPower5GHT40, pDefault9300->calTarget_freqbin_5GHT40);
    print9300_5GCTLData(client, pDefault9300->ctlIndex_5G, pDefault9300->ctlPowerData_5G, &pDefault9300->ctl_freqbin_5G[0][0]);

    print9300_2GHzHeader(client, &pDefault9300->modalHeader2G);
    print9300_2GHzCalData(client, pDefault9300);
    print9300_2GLegacyTargetPower(client, pDefault9300->calTargetPower2G, pDefault9300->calTarget_freqbin_2G);
    print9300_2GCCKTargetPower(client, pDefault9300->calTargetPowerCck, pDefault9300->calTarget_freqbin_Cck);
    print9300_2GHT20TargetPower(client, pDefault9300->calTargetPower2GHT20, pDefault9300->calTarget_freqbin_2GHT20);
    print9300_2GHT40TargetPower(client, pDefault9300->calTargetPower2GHT40, pDefault9300->calTarget_freqbin_2GHT40);

    print9300_2GCTLData(client, pDefault9300->ctlIndex_2G, pDefault9300->ctlPowerData_2G, &pDefault9300->ctl_freqbin_2G[0][0]);

}

void Ar9300EepromDisplayAll(void)
{
	print9300Struct(0,0);
}

