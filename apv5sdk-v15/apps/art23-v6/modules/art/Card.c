

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "wlantype.h"
//
// hal header files
//
#include "ah.h"
#include "ah_osdep.h"
#ifdef __APPLE__
#include "osdep.h"
#endif

#include "opt_ah.h"
#include "ah_devid.h"
#include "ah_internal.h"
#include "ar9300.h"
#include "ar9300eep.h"
#include "ar9300reg.h"

#include "AnwiDriverInterface.h"

#include "Ar9300EepromStructSet.h"

#include "ParameterSelect.h"
#include "Card.h"
#include "smatch.h"
#include "UserPrint.h"
#include "TimeMillisecond.h"
#include "CommandParse.h"
#include "ParameterParse.h"
#include "NewArt.h"
#include "Sticky.h"

#include "ErrorPrint.h"
#include "ParseError.h"
#include "CardError.h"
#include "NartError.h"

#include "rate_constants.h"
#include "LinkList.h"
#include "LinkTx.h"
#include "LinkRx.h"
#include "NoiseFloor.h"


#include "Device.h"
#include "DeviceLoad.h"

#include "ConfigurationGet.h"
#include "Calibrate.h"

#include "NoiseFloor.h"

#include "Ar9300EepromSave.h"
#include "ConfigurationSet.h"

//
// pick up the function definitions for Merlin
//
#ifndef __APPLE__
#include "Ar5416Device.h"
#include "Ar5416RxDescriptor.h"
#include "Ar5416TxDescriptor.h"
#include "Ar5416Field.h"
#endif
//
// pick up the function definitions for Osprey
//
#include "Ar9300Device.h"
#include "Ar9300RxDescriptor.h"
#include "Ar9300TxDescriptor.h"
#include "Ar9300Field.h"
#include "Ar9300CalibrationApply.h"
#include "mEepStruct9300.h"
#include "ar9300eep.h"
#include "papredistortion.h"

#include "templatelist.h"


#define MBUFFER 1024
#define MLABEL 30


//
// index for the ANWI driver stuff
//
static short devIndex=0;		// this has to be 0

// 
// this is the hal pointer, 
// returned by ath_hal_attach
// used as the first argument by most (all?) HAL routines
//
struct ath_hal *AH=0;

//
// keep track of whether we've done a reset. 
// there should be a way to ask the chip, but right now some things don't work (e.g. EepromWrite())
// unless a reset has been performed at least once since the last power down.
//	
static int ResetDone=0;

static int ResetForce=0;

static int DeviceValid=0;

void CardResetForce()
{
	ResetForce=1;
}

static int PapdEnable=0;
static HAL_CHANNEL channel;
static int ChannelLast=0;
static int TimeLast=0;
static int BandwidthLast=BW_AUTOMATIC;
static unsigned int TxChainLast=0;
static unsigned int RxChainLast=0;
static unsigned int PapdEnableLast=0;


#define MBUFFER 1024

void FreeMemoryPrint(void)
{
	int calibration;
	int pcie;

	calibration=Ar9300EepromUsed();
	pcie=ConfigurationUsed();
	ErrorPrint(CardFreeMemory,calibration-pcie,calibration,pcie);
}

//
// return the channel frequency used in the last chip reset
//
int CardFrequency(void)
{
	return ChannelLast;
}

int CardRemoveDo(void)
{
	AH=0;
    ResetDone=0;
    DeviceValid=0;
	StickyClear();
	CalibrateClear();
    AnwiDriverDetach();
    return 0;
}


void CardUnloadDataSend(int client)
{
	ErrorPrint(NartData,"|set|devid||");
	ErrorPrint(NartData,"|set|mac||");
	ErrorPrint(NartData,"|set|customer||");
}


int CardRemove(int client)
{	

	if(!CardValid())
	{
		ErrorPrint(CardNoneLoaded);
	}
	//
	// do it anyway
	//
	CardRemoveDo();
	//
	// send data to cart
	//
	CardUnloadDataSend(client);
	//
	// announce success
	//
	ErrorPrint(CardUnloadGood);
	//
	// say we're done
	//
	SendDone(client);
	
	return 0;
}

int CardRxIqCalComplete()
{
	HAL_BOOL isCalDone=0;
	int status;
	u_int32_t sched_cals=IQ_MISMATCH_CAL;
	u_int8_t rxchainmask=RxChainLast;
	
	status=ar9300Calibration(AH, &channel, rxchainmask,
                  0, &isCalDone, 0, &sched_cals);

    return isCalDone;
}


int CardResetDo(int frequency, int txchain, int rxchain, int bandwidth)
{	
	int start,end;
	int error;
	HAL_OPMODE opmode;
	HAL_HT_MACMODE htmode;
    HAL_HT_EXTPROTSPACING exprotspacing;
	HAL_BOOL bChannelChange;
	int isscan;
	int tnf[1];
	//
	// do it
	//
	start=TimeMillisecond();
	//
	// station card
	// how do we do AP??
	//
	opmode=HAL_M_STA;
    //
	// channel parameters
	// are we really allowed to (have to) set all of this?
	//
    channel.channel=frequency;        /* setting in Mhz */
#ifdef UNUSED
/* channelFlags */
#define CHANNEL_CW_INT    0x00002 /* CW interference detected on channel */
#define CHANNEL_TURBO     0x00010 /* Turbo Channel */
#define CHANNEL_CCK       0x00020 /* CCK channel */
#define CHANNEL_OFDM      0x00040 /* OFDM channel */
#define CHANNEL_2GHZ      0x00080 /* 2 GHz spectrum channel. */
#define CHANNEL_5GHZ      0x00100 /* 5 GHz spectrum channel */
#define CHANNEL_PASSIVE   0x00200 /* Only passive scan allowed in the channel */
#define CHANNEL_DYN       0x00400 /* dynamic CCK-OFDM channel */
#define CHANNEL_XR        0x00800 /* XR channel */
#define CHANNEL_STURBO    0x02000 /* Static turbo, no 11a-only usage */
#define CHANNEL_HALF      0x04000 /* Half rate channel */
#define CHANNEL_QUARTER   0x08000 /* Quarter rate channel */
#define CHANNEL_HT20      0x10000 /* HT20 channel */
#define CHANNEL_HT40PLUS  0x20000 /* HT40 channel with extention channel above */
#define CHANNEL_HT40MINUS 0x40000 /* HT40 channel with extention channel below */
#define CHANNEL_A           (CHANNEL_5GHZ|CHANNEL_OFDM)
#define CHANNEL_B           (CHANNEL_2GHZ|CHANNEL_CCK)
#define CHANNEL_PUREG       (CHANNEL_2GHZ|CHANNEL_OFDM)
#endif
	if(bandwidth==BW_QUARTER || bandwidth==BW_HALF || bandwidth==BW_HT20 || bandwidth==BW_OFDM)
	{
		htmode=HAL_HT_MACMODE_20;
	}
	else
	{
		htmode=HAL_HT_MACMODE_2040;
	}
	if(frequency<4000)		// let's presume this is B/G
	{
	    if(bandwidth==BW_AUTOMATIC || bandwidth==BW_HT40_PLUS)
		{
		    channel.channelFlags=CHANNEL_2GHZ|CHANNEL_HT40PLUS;
		}
	    else if(bandwidth==BW_HT40_MINUS)
		{
		    channel.channelFlags=CHANNEL_2GHZ|CHANNEL_HT40MINUS;
		}
		else
		{
            channel.channelFlags=CHANNEL_2GHZ|CHANNEL_HT20; 
		}
	}
	else
	{
	    if(bandwidth==BW_HALF)
		{
			channel.channelFlags=CHANNEL_5GHZ|CHANNEL_HALF|CHANNEL_OFDM;
		}
		else if(bandwidth==BW_QUARTER)
		{
			channel.channelFlags=CHANNEL_5GHZ|CHANNEL_QUARTER|CHANNEL_OFDM;
		}
		else if (bandwidth==BW_OFDM) {
			channel.channelFlags = CHANNEL_5GHZ|CHANNEL_OFDM;
		}
	    else if(bandwidth==BW_AUTOMATIC || bandwidth==BW_HT40_PLUS)
		{
		    channel.channelFlags=CHANNEL_5GHZ|CHANNEL_HT40PLUS;
		}
	    else if(bandwidth==BW_HT40_MINUS)
		{
		    channel.channelFlags=CHANNEL_5GHZ|CHANNEL_HT40MINUS;
		}
		else
		{
            channel.channelFlags=CHANNEL_5GHZ|CHANNEL_HT20; 
		}
	}
#ifdef UNUSED
#define CHANNEL_INTERFERENCE    0x01 /* Software use: channel interference 
                                        used for as AR as well as RADAR 
                                        interference detection */
#define CHANNEL_DFS             0x02 /* DFS required on channel */
#define CHANNEL_4MS_LIMIT       0x04 /* 4msec packet limit on this channel */
#define CHANNEL_DFS_CLEAR       0x08 /* if channel has been checked for DFS */
#define CHANNEL_DISALLOW_ADHOC  0x10 /* ad hoc not allowed on this channel */
#define CHANNEL_PER_11D_ADHOC   0x20 /* ad hoc support is per 11d */
#define CHANNEL_EDGE_CH         0x40 /* Edge Channel */
#endif
    channel.privFlags=CHANNEL_DFS_CLEAR;

    channel.maxRegTxPower=27;  /* max regulatory tx power in dBm */
    channel.maxTxPower=2*27;     /* max true tx power in 0.5 dBm */
    channel.minTxPower=0;     /* min true tx power in 0.5 dBm */
    channel.regClassId=0;     /* regulatory class id of this channel */
#ifdef UNUSED
	typedef enum {
        HAL_HT_MACMODE_20       = 0,            /* 20 MHz operation */
        HAL_HT_MACMODE_2040     = 1,            /* 20/40 MHz operation */
} HAL_HT_MACMODE;
#endif
#ifdef UNUSED
typedef enum {
    HAL_HT_EXTPROTSPACING_20    = 0,            /* 20 MHZ spacing */
    HAL_HT_EXTPROTSPACING_25    = 1,            /* 25 MHZ spacing */
} HAL_HT_EXTPROTSPACING;
#endif
    exprotspacing=HAL_HT_EXTPROTSPACING_20;

	bChannelChange=0;           // fast channel change is broken in HAL/osprey
	if(ResetForce)
	{
		bChannelChange=0;
	}

	error=0;
	isscan=0;
	if(AH!=0 && AH->ah_reset!=0)
	{
	    (*AH->ah_reset)(AH,opmode,&channel,htmode,txchain,rxchain,exprotspacing,bChannelChange,(HAL_STATUS*)&error,isscan);

		if(error==HAL_FULL_RESET)
		{
			error=0;
		}

        if(error==0)
		{
			StickyExecute();

#ifdef ATH_SUPPORT_PAPRD 
			// Call PAPD routine
			if(Ar9300EepromPaPredistortionGet() && Ar9300EepromCalibrationValid())
			{
				int modebit=0;
				if(frequency>4000)modebit=1;
				if((Ar9300PapdRateMaskHt20Get(modebit)&0xffffff)!=0){
			    	struct ath_hal_9300 *ahp = AH9300(AH);
				    u_int8_t   txrxMask;

				    txrxMask = ahp->ah_eeprom.baseEepHeader.txrxMask;

				    papredistortionSingleTable(AH, &channel, (txrxMask&0xF0)>>4);
			    }
	        }
#endif
			ResetDone=1;
		}
	}
	else
	{
		error= -1;
	}
	end=TimeMillisecond();

	
	return error;
}

#define MCHAIN 3
int ar9300ChainMany()
{
	unsigned int ah_enterprise_mode;

    if (AR_SREV_HORNET(AH)) 
	{
       return 1;
    } 
	else if (AR_SREV_WASP(AH))
	{
       return 2;
    } 
	else 
	{
        /* Osprey needs to be configured for 3-chain mode before running AGC/TxIQ cals */
		int ah_enterprise_mode;

		DeviceRegisterRead(AR_ENT_OTP,&ah_enterprise_mode);
        if(ah_enterprise_mode&AR_ENT_OTP_CHAIN2_DISABLE)
		{
			return 2;
		}
		else
		{
			return 3;
		}
    }
}


int CardRxChainMany()
{
	return ar9300ChainMany();
}

int CardTxChainMany()
{
	return ar9300ChainMany();
}

int CardTxChainMask()
{
	switch(CardTxChainMany())
	{
		case 1:
			return 1;
		case 2:
			return 3;
		case 3:
			return 7;
	}
	return 0;
}

int CardRxChainMask()
{
	switch(CardRxChainMany())
	{
		case 1:
			return 1;
		case 2:
			return 3;
		case 3:
			return 7;
	}
	return 0;
}
		

int NoiseFloorBad(int rxchain, int bandwidth)
{
	int it;
	int nfc[MCHAIN],nfe[MCHAIN];

	if(NoiseFloorFetchWait(nfc,nfe,MCHAIN,1000)!=0)
	{
		ErrorPrint(CardNoiseFloorBad, "NosieFloorFetch Timeout");
		return 1;
	}
	for(it=0; it<3; it++)
	{
		if(rxchain & (0x1<<it)){
			if(nfc[it]< -130)
			{
				ErrorPrint(CardNoiseFloorBad,nfc[0],nfe[0],nfc[1],nfe[1],nfc[2],nfe[2]);
				return 1;
			}
			if((bandwidth==BW_AUTOMATIC||bandwidth==BW_HT40_PLUS||bandwidth==BW_HT40_MINUS) && (nfe[it]< -130))
			{
				ErrorPrint(CardNoiseFloorBad,nfc[0],nfe[0],nfc[1],nfe[1],nfc[2],nfe[2]);
				return 1;
			}
		}
	}
	return 0;
}

#define MCHANNEL 2000
static HAL_CHANNEL Channel[MCHANNEL];
static unsigned int ChannelMany;
//
// look though the channel list and return the first channel that matches.
// frequency<=0 matches anything
//
extern int CardChannelFind(int fmin, int fmax, int bw, int *rfrequency, int *rbw)
{
	int it;
	*rfrequency=-100;
	*rbw=BW_AUTOMATIC;
	//
	// if we didn't find anything that works for ht40 and the setting is automatic,
	// try again just looking for ht20
	//
	if(bw==BW_AUTOMATIC)
	{
		if(CardChannelFind(fmin,fmax,BW_HT40_PLUS,rfrequency,rbw))
		{
			if(CardChannelFind(fmin,fmax,BW_HT40_MINUS,rfrequency,rbw))
			{
				if(CardChannelFind(fmin,fmax,BW_HT20,rfrequency,rbw))
				{
					if(CardChannelFind(fmin,fmax,BW_HALF,rfrequency,rbw))
					{
						if(CardChannelFind(fmin,fmax,BW_QUARTER,rfrequency,rbw))
						{
							if(CardChannelFind(fmin,fmax,BW_OFDM,rfrequency,rbw))
							{
								return -1;
							}
						}
					}
				}
			}
		}
		return 0;
	}
	else
	{
		for(it=0; it<ChannelMany; it++)
		{
			if(fmin<=Channel[it].channel && Channel[it].channel<=fmax)
			{
				switch(bw)
				{
					case BW_HT40_PLUS:
						if(Channel[it].channelFlags&CHANNEL_HT40PLUS)
						{
							*rfrequency=Channel[it].channel;
							*rbw=BW_HT40_PLUS;
							return 0;
						}
						break;
					case BW_HT40_MINUS:
						if(Channel[it].channelFlags&CHANNEL_HT40MINUS)
						{
							*rfrequency=Channel[it].channel;
							*rbw=BW_HT40_MINUS;
							return 0;
						}
						break;
					case BW_HT20:
						if(Channel[it].channelFlags&CHANNEL_HT20)
						{
							*rfrequency=Channel[it].channel;
							*rbw=BW_HT20;
							return 0;
						}
						break;
					case BW_OFDM:
						if(Channel[it].channelFlags&CHANNEL_OFDM)
						{
							*rfrequency=Channel[it].channel;
							*rbw=BW_OFDM;
							return 0;
						}
						break;
					case BW_HALF:
						if(Channel[it].channelFlags&CHANNEL_HALF)
						{
							*rfrequency=Channel[it].channel;
							*rbw=BW_HALF;
							return 0;
						}
						break;
					case BW_QUARTER:
						if(Channel[it].channelFlags&CHANNEL_QUARTER)
						{
							*rfrequency=Channel[it].channel;
							*rbw=BW_QUARTER;
							return 0;
						}
						break;
				}
			}
		}
	}
	return -1;
}

int CardResetIfNeeded(int frequency, int txchain, int rxchain, int force, int bandwidth)
{
	int start;
	int error;
	int bandwidthUse,bandwidthTemp;
	int rxchainuse,txchainuse;
	int tfrequency;
	int nchain;

	error=0;
    if(!CardValid())
	{
		return -1;
	}

	if(bandwidth == BW_AUTOMATIC)
	{
		CardChannelFind(frequency,frequency,bandwidth,&frequency,&bandwidthUse);
	}
	else
	{
		bandwidthUse=bandwidth;
	}
	//
	// if something in the code has signalled that the current configuration is invalid
	// then we force a reset even if the current command doesn't want one.
	//
	if(ResetForce)
	{
		force=1;
	}

	//
	// get maximum chain mask
	//
	nchain=ar9300ChainMany();
	if(nchain==1)
	{
		txchain=0x1;
		rxchainuse=0x1;
	}
	else if(nchain==2)
	{
		txchain=0x3;
		rxchainuse=0x3;
	}
	else
	{
		txchain=0x7;
		rxchainuse=0x7;
	}

	start=TimeMillisecond();
	if(!ResetDone || force>0 || 
		(force<0 && 
		    (
		    ChannelLast!=frequency || 
			bandwidth!=BandwidthLast ||
			TxChainLast!=txchain ||
			RxChainLast!=rxchain ||
		    start-TimeLast>=600000 
			)
		)
	)
	{	
		//
		// reset the device using HAL, always reset with all chains on.
		//
        error=CardResetDo(frequency, txchain, rxchainuse,bandwidthUse);
		if(error!=0)
		{
			ErrorPrint(CardResetBad,error,frequency,bandwidthUse,txchain,rxchain);
		}      
		if(error!=0 || NoiseFloorBad(rxchain, bandwidthUse))		// ####need to get correct chain mask
		{
			//
			// sometimes reset fails, we don't know why, but it appears that resetting
			// to another channel may fix it. so we're going to try this experiment.
			//
     		if(!CardChannelFind(frequency+1,10000,bandwidth,&tfrequency,&bandwidthTemp))
			{
				CardChannelFind(0,frequency-1,bandwidth,&tfrequency,&bandwidthTemp);	
			}

			//
			// reset the device using HAL, always reset with all chains on.
			//
			error=CardResetDo(tfrequency, txchain, rxchainuse, bandwidthTemp);
//			printf("*********** freq:%d, txchain: %d, rxchainuse: %d,  bandwidthTemp:%d,  error: %d\n", 
//				tfrequency, txchain, rxchainuse, bandwidthTemp, error);	
			//
			// Now we have done the reset to alternative channel now try the original channel
			//
			error=CardResetDo(frequency, txchain, rxchainuse, bandwidthUse);
//			printf("*********** freq:%d, txchain: %d, rxchainuse: %d,  bandwidthUse:%d,  error: %d\n", 
//				frequency, txchain, rxchainuse, bandwidthUse, error);	
			if(error!=0 || NoiseFloorBad(nchain, bandwidthUse))		// ####need to get correct chain mask
			{
				error=1;
			}
		}
		//
		// record the parameters we used
		//
		if(error==0)
		{
			TimeLast=start;
			ChannelLast=frequency;
			BandwidthLast=bandwidthUse;
			TxChainLast=txchain;
			RxChainLast=rxchain;
			ResetForce=0;
			//
			// reset is done, now set the rx chain mask registers.
			//
			Ar9300RxChainSet(rxchain);

			ErrorPrint(CardResetSuccess,frequency,bandwidthUse,txchain,rxchain);
		}
		else
		{
			TimeLast=0;
			ChannelLast=0;
			BandwidthLast=BW_AUTOMATIC;
			TxChainLast=0;
			RxChainLast=0;
			ResetForce=1;
			ErrorPrint(CardResetFail,error,frequency,bandwidthUse,txchain,rxchain);
		}
	}
	return error;
}



static struct _ParameterList ResetParameter[]=
{
	LINK_FREQUENCY(1),
	LINK_CHAIN(1),
	LINK_TX_CHAIN(1), 
	LINK_RX_CHAIN,
	LINK_HT40,
	LINK_RESET,
	LINK_BANDWIDTH,
};

void CardResetParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(ResetParameter)/sizeof(ResetParameter[0]);
    list->special=ResetParameter;
}





static int ChannelMessageHeader(int client)
{
//    return ErrorPrint(NartDataHeader,"|channel|frequency|channelFlag|privFlag|max_reg_tx_power|max_tx_power|mix_tx_power|regClass|");
    return ErrorPrint(NartDataHeader,"|channel|frequency|CCK|OFDM|ht20|ht40p|ht40m|TURBO|STURBO|HALF|QUARTER|");
}


static int ChannelMessageSend(int client, HAL_CHANNEL *channel)
{
	char buffer[MBUFFER];
	int lc;

	lc=SformatOutput(buffer,MBUFFER-1,"|channel|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|",
        channel->channel,
		((channel->channelFlags&CHANNEL_2GHZ&CHANNEL_CCK)==0? 0:1),
        ((channel->channelFlags&CHANNEL_OFDM)==0? 0:1),
        ((channel->channelFlags&CHANNEL_HT20)==0? 0:1),
        ((channel->channelFlags&CHANNEL_HT40PLUS)==0? 0:1),
        ((channel->channelFlags&CHANNEL_HT40MINUS)==0? 0:1),
        ((channel->channelFlags&CHANNEL_TURBO)==0? 0:1),
        ((channel->channelFlags&CHANNEL_STURBO)==0? 0:1),
        ((channel->channelFlags&CHANNEL_HALF)==0? 0:1),
        ((channel->channelFlags&CHANNEL_QUARTER)==0? 0:1));

    return ErrorPrint(NartData,buffer);
}


int CardChannel(int client)
{
	unsigned int it;

	ChannelMessageHeader(client);
	//
	// return channel information
	//
	for(it=0; it<ChannelMany; it++)
	{
		ChannelMessageSend(client,&Channel[it]);
	}
	return 0;
}


int CardChannelCalculate(void)
{
	u_int32_t modeSelect;
    HAL_BOOL enableOutdoor;
	HAL_BOOL enableExtendedChannels;
	u_int16_t *regDmn;
    u_int8_t regclassids[MCHANNEL];
    u_int maxchans, maxregids, nregids;
    HAL_CTRY_CODE cc;
	int error;
	//
	// try calling this here
	//
	//Fcain need to handle this better, should not be accessing struct values directly
	regDmn = ar9300RegulatoryDomainGet(AH);
	if( regDmn[0] != 0) 
	{
		UserPrint("RegulatoryDomainOverride: 0x%x -> 0\n",regDmn[0]);
        ar9300RegulatoryDomainOverride(AH, 0);
	}		
	if(regDmn[0] & 0x8000)
		cc=regDmn[0] & 0x3ff;
	else
		cc=CTRY_DEBUG;		//CTRY_DEFAULT;			// changed from CTRY_DEBUG since that doesn't work if regdmn is set
	modeSelect=0xffffffff;
    enableOutdoor=0;
	enableExtendedChannels=0;
    maxchans=sizeof(Channel)/sizeof(Channel[0]);
	maxregids=sizeof(regclassids)/sizeof(regclassids[0]);

    error=ath_hal_init_channels(AH,
              Channel, maxchans, &ChannelMany,
              regclassids, maxregids, &nregids,
              cc, modeSelect,
              enableOutdoor, enableExtendedChannels);

	if(error==0)
	{
//		AH=0;
		ErrorPrint(CardLoadNoChannel);
//		return -5;
	}

	return ChannelMany;
}


int CardReset(int client)
{
    int frequency;
	int ht40;
	unsigned int txchain;
	int rxchain;
	int reset;
	int error;
	int bandwidth;
	char buffer[MBUFFER];
	int ip, np;
	int ngot;
	char *name;
	int code;
	int index;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		ErrorPrint(CardNoneLoaded);
		error= -1;
    }
	else
	{
		error=0;
		frequency= -1;
		txchain=CardTxChainMask();
		rxchain=CardRxChainMask();
		ht40=2;			// ht40 is on, either high or low as appropriate
		reset=LinkResetDefault;		// default is force reset
		bandwidth=LinkBandwidthDefault;
		//
		// prepare beginning of error message in case we need to use it
		//
		error=0;
		//
		// parse parameters and values
		//
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			index=ParameterSelectIndex(name,ResetParameter,sizeof(ResetParameter)/sizeof(ResetParameter[0]));
			if(index>=0)
			{
				code=ResetParameter[index].code;
				switch(code) 
				{
					case LinkParameterFrequency:
						ngot=ParseIntegerList(ip,name,&frequency,&ResetParameter[index]);
						if(ngot<=0)
						{
							error++;
						}
						break;
					case LinkParameterHt40:
						ngot=ParseIntegerList(ip,name,&ht40,&ResetParameter[index]);
						if(ngot<=0)
						{
							error++;
						}
						else
						{
							switch(ht40){
							case 0:
								bandwidth=BW_HT20;
								break;
							case 1:
								bandwidth=BW_HT40_PLUS;
								break;
							case -1:
								bandwidth=BW_HT40_MINUS;
								break;
							case 2:
								bandwidth=BW_AUTOMATIC;
								break;
							default:
								error++;
								break;
							}
						}
						break;
					case LinkParameterBandwidth:
						ngot=ParseIntegerList(ip,name,&bandwidth,&ResetParameter[index]);
						if(ngot<=0)
						{
							error++;
						}
						break;
					case LinkParameterChain:
						ngot=ParseHexList(ip,name,&txchain,&ResetParameter[index]);
						if(ngot<=0)
						{
							error++;
						}
						else
						{
							rxchain=txchain;
						}
						break;
					case LinkParameterTxChain:
						ngot=ParseHexList(ip,name,&txchain,&ResetParameter[index]);
						if(ngot<=0)
						{
							error++;
						}
						break;
					case LinkParameterRxChain:
						ngot=ParseHexList(ip,name,&txchain,&ResetParameter[index]);
						if(ngot<=0)
						{
							error++;
						}
						break;
					case LinkParameterReset:
						ngot=ParseIntegerList(ip,name,&reset,&ResetParameter[index]);
						if(ngot<=0)
						{
							error++;
						}
						break;
					default:
						error++;
						ErrorPrint(ParseBadParameter,name);
						break;
				}
			}
			else
			{
				error++;
				ErrorPrint(ParseBadParameter,name);
			}
		}

		if(error==0)
		{
			SendOn(client);

			if(frequency<0)
			{
				if(CardChannelFind(0,10000,bandwidth,&frequency,&bandwidth)==0)
				{
					CardResetIfNeeded(frequency,txchain,rxchain,reset,bandwidth);
				}
			}
			else
			{
				error=CardResetIfNeeded(frequency,txchain,rxchain,reset,bandwidth);
			}

			SendOff(client);
		}
	}
	//
	// say we're done
	//
    SendDone(client);

	return error;
}


#define MLABEL 30

#define CHIP_MAC_ID (0x4020)

static int ChipIdentify(void)
{
	unsigned int macid;

	macid=0;
	if(MyRegisterRead(CHIP_MAC_ID,&macid)==0)
	{
		macid = (macid & AR_SREV_VERSION2) >> AR_SREV_TYPE2_S;
        switch(macid)
		{
			case AR_SREV_VERSION_OSPREY:
				return AR9300_DEVID_AR9380_PCIE;
			case AR_SREV_VERSION_POSEIDON:
				return AR9300_DEVID_AR9485_PCIE;
//			case AR_SREV_VERSION_JUPITER:
//				return AR9300_DEVID_AR9380_PCIE;
			case AR_SREV_VERSION_HORNET:
				return AR9300_DEVID_AR9380_PCIE;
			default:
				return AR9300_DEVID_AR9340;
		}
	}
	return -1;
}


static int CardLoadDo(int client, int devid, int calmem)
{
    HAL_ADAPTER_HANDLE osdev;
	HAL_SOFTC sc;
	HAL_BUS_TAG st;
	HAL_BUS_HANDLE sh;
	HAL_BUS_TYPE bustype;
    struct hal_reg_parm hal_conf_parm;
	HAL_STATUS error;
	int start,end;
	int it;
    char buffer[MBUFFER];
	int caluse;
	int eepsize;
    int status;
	//
	// connect to the ANWI driver
	//
    status=AnwiDriverAttach(devid); 
    if(status<0) 
	{
		ErrorPrint(CardLoadAnwi);
        return -2;
    }
	//
	// try to automatically identify chip
	//
	if(devid<0)
	{
		devid=ChipIdentify();
		if(devid<0)
		{
			ErrorPrint(CardChipUnknown);
			return -CardChipUnknown;
		}
	}
    if(devid==AR9300_DEVID_EMU_PCIE)
    { 
        devid=AR9300_DEVID_AR9380_PCIE;
    }
	//
    //
    // AND THEN WE CONFIGURE THE HAL CODE.
    // THIS SHOULD REALLY BE IN DEVICE SPECIFC FUNCTIONS UNDER THE NART CODE
    //
	//
	// attach to the HAL, pass it the register memory address from ANWI
	//
	osdev=0;
	sc=0;																// wmi_handle???
	st=0;																// bsd only?
	sh=(HAL_BUS_HANDLE)AnwiDriverRegisterMap();	// register map
	bustype=0;															// 5212 only?
    //
	// this stuff shouldn't be here. move into the hal as default values if conf_parm=0
	//
	hal_conf_parm.forceBias=0;
    hal_conf_parm.forceBiasAuto=0;
    hal_conf_parm.halPciePowerSaveEnable=0;
    hal_conf_parm.halPcieL1SKPEnable=0;
    hal_conf_parm.halPcieClockReq=0;
    hal_conf_parm.halPciePowerReset=0x100;
    hal_conf_parm.halPcieWaen=0;
    hal_conf_parm.halPcieRestore=0;
    hal_conf_parm.htEnable=1;
//    hal_conf_parm.disableTurboG=0;
    hal_conf_parm.ofdmTrigLow=200;
    hal_conf_parm.ofdmTrigHigh=500;
    hal_conf_parm.cckTrigHigh=200;
    hal_conf_parm.cckTrigLow=100;
    hal_conf_parm.enableANI=1;
    hal_conf_parm.noiseImmunityLvl=4;
    hal_conf_parm.ofdmWeakSigDet=1;
    hal_conf_parm.cckWeakSigThr=0;
    hal_conf_parm.spurImmunityLvl=2;
    hal_conf_parm.firStepLvl=0;
    hal_conf_parm.rssiThrHigh=40;
    hal_conf_parm.rssiThrLow=7;
    hal_conf_parm.diversityControl=0;
    hal_conf_parm.antennaSwitchSwap=0;
//    for (it=0; it< AR_EEPROM_MODAL_SPURS; it++) 
//	{
//        hal_conf_parm.ath_hal_spurChans[it][0] = 0;
//        hal_conf_parm.ath_hal_spurChans[it][1] = 0;
//    }
    hal_conf_parm.serializeRegMode=0;
    hal_conf_parm.defaultAntCfg=0;
    hal_conf_parm.fastClockEnable=1;
    hal_conf_parm.hwMfpSupport=0;
//    hal_conf_parm.ath_hal_enableMSI=0;

    {
#ifdef UNUSED
        // THIS NEEDS TO BE FIXED
        // PROVIDE AN ACCESS FUNcTION, NOT A GLOBAL SYMBOL
        extern int calData;
        calData=calmem;
#else
		ar9300CalibrationDataSet(0,calmem);
#endif
    }

	start=TimeMillisecond();
	error=0;
	AH=ath_hal_attach((unsigned short)devid, osdev, sc, st, sh, bustype, NULL /* amem_handle */, &hal_conf_parm, &error);
	if(error!=0)
	{
		ErrorPrint(CardLoadAttach,error);
		return error;
	}
	if(AH==0)
	{
		ErrorPrint(CardLoadHal);
		return -4;
	}
	end=TimeMillisecond();
	UserPrint("ath_hal_attach duration: %d=%d-%d ms\n",end-start,end,start);
	//
	// Calculate list of legal channels.
	//
	CardChannelCalculate();
	//
	// figure out where the calibration memory really is
	//
    caluse=ar9300CalibrationDataGet(AH);
	switch(caluse)
	{
		case CalibrationDataNone:
			ErrorPrint(CardLoadCalibrationNone);
			eepsize=ar9300EepromSize(AH);
			if(eepsize>0)
			{
				ar9300CalibrationDataSet(AH,CalibrationDataEeprom);
			}
			else
			{
				ar9300CalibrationDataSet(AH,CalibrationDataOtp);
			}
			break;
		case CalibrationDataFlash:
			ErrorPrint(CardLoadCalibrationFlash);
			break;
		case CalibrationDataEeprom:
			ErrorPrint(CardLoadCalibrationEeprom,ar9300CalibrationDataAddressGet(AH));
			break;
		case CalibrationDataOtp:
			ErrorPrint(CardLoadCalibrationOtp,ar9300CalibrationDataAddressGet(AH));
			break;
	}

	if (ConfigurationInit()!=0) 
	{
		ErrorPrint(CardLoadPcie);
		return -3;
    }
	//
    //
    // FROM HERE ON WE CONFIGURE THE NART CODE
    //
	CalibrateClear();
	//
	// THIS LIST NEEDS TO BE COMPLETE AND ACCURATE
	// WOULD LIKE IT TO BE INSIDE THE HAL ATTACH (OR REPLACE WITH EQUIVALENT HAL FUNCTIONS)
	//
    switch (devid) 
	{
#ifdef AH_SUPPORT_AR5212
    case AR5212_DEVID_IBM:
    case AR5212_AR2413:
    case AR5212_AR5413:
    case AR5212_AR5424:
    case AR5212_DEVID_FF19: /* XXX PCI Express extra */
        devid = AR5212_DEVID;
        /* fall thru... */
    case AR5212_AR2417:
    case AR5212_DEVID:
    case AR5212_FPGA:
    case AR5212_DEFAULT:
		Ar5212DeviceSelect();
		Ar5212FieldSelect();
		Ar5212RxDescriptorSelect();
		Ar5212TxDescriptorSelect();
        DeviceValid=1;
        break;
#endif

#ifdef AH_SUPPORT_AR5416
	case AR5416_DEVID_PCI:
	case AR5416_DEVID_PCIE:
	case AR5416_DEVID_AR9160_PCI:
	case AR5416_AR9100_DEVID:
    case AR5416_DEVID_AR9280_PCI:
    case AR5416_DEVID_AR9280_PCIE:
	case AR5416_DEVID_AR9285_PCIE:
	case AR5416_DEVID_AR9287_PCI:
	case AR5416_DEVID_AR9287_PCIE:
		Ar5416DeviceSelect();
		Ar5416FieldSelect();
		Ar5416RxDescriptorSelect();
		Ar5416TxDescriptorSelect();
        LinkTxOptions(0,0,1,0);
        LinkRxOptions(0,-1,0);
        DeviceValid=1;
        break;
#endif

#ifdef AH_SUPPORT_AR9300
    case AR9300_DEVID_AR9380_PCIE:
	case AR9300_DEVID_AR9580_PCIE:
    case AR9300_DEVID_EMU_PCIE:
		Ar9300DeviceSelect();
#ifndef __APPLE__
		if(AR_SREV_HORNET(AH))
		{
			Ar9330_FieldSelect();
			LinkRxOptions(1,128,1);
		}
		else
#endif
		{
			Ar9300_2_0_FieldSelect();
	        LinkRxOptions(1,128,1);
		}
		Ar9300RxDescriptorSelect();
		Ar9300TxDescriptorSelect();
        LinkTxOptions(1,1,0,1);
        DeviceValid=1;
        break;
		
case AR9300_DEVID_AR9340:
        UserPrint("Wasp 1.0\n\n");
        OS_DELAY(30);
        Ar9300DeviceSelect();
        Ar9340FieldSelect();
        LinkRxOptions(1,128,1);
        Ar9300RxDescriptorSelect();
        Ar9300TxDescriptorSelect();
        LinkTxOptions(1,1,0,1);
        DeviceValid=1;
        OS_DELAY(30);
        break; 
#endif

    default:
        SformatOutput(buffer,MBUFFER-1,"%04x",devid);
        buffer[MBUFFER-1]=0;
	
		#ifndef MD_AP
			UserPrint("Error:: Dynamic loading is not support on AP in this build\n");
		#else
			#ifndef __APPLE__
        	error=DeviceLoad(buffer);
		#endif
		#endif
        if(error==0)
        {
            DeviceValid=1;
        }
        else
        {
            ErrorPrint(CardLoadDevid,devid);
            DeviceValid=0;
        }
        break;
    }
    return 0;
}
 

static void CardLoadDataSend(int client)
{
    int devid;
	int ssid;
	int bus;
	int error;
	int lc, nc;
	char buffer[MBUFFER];
	int ip, np;
	int ngot;
	char *name;
	unsigned char macaddr[6];
	int it;
	unsigned int address,value;
    char label[MLABEL];
    //
    // Send any other parameter that we want cart to know about.
    // |set| ... data responses are intercepted in cart and converted to $variables which are 
    // user accessible for reports and limits and other fun stuff.
    //
	devid=DeviceIdGet();
	SformatOutput(buffer,MBUFFER-1,"|set|devid|%04x|",devid);
    ErrorPrint(NartData,buffer);

    ConfigurationMacAddressGet(macaddr);
    if(macaddr[0]!=0 || macaddr[1]!=0 || macaddr[2]!=0 || macaddr[3]!=0 || macaddr[4]!=0 || macaddr[5]!=0)
    {
	    SformatOutput(buffer,MBUFFER-1,"|set|mac|%02x:%02x:%02x:%02x:%02x:%02x|",
		    macaddr[0],macaddr[1],macaddr[2],macaddr[3],macaddr[4],macaddr[5]);
        buffer[MBUFFER-1]=0;
        ErrorPrint(NartData,buffer);
    }
    else
    {
	    SformatOutput(buffer,MBUFFER-1,"|set|mac||");
        buffer[MBUFFER-1]=0;
        ErrorPrint(NartData,buffer);
    }

    ConfigurationCustomerDataGet(label, MLABEL);
    label[MLABEL-1]=0;
    if(label[0]!=0)
    {
	    SformatOutput(buffer,MBUFFER-1,"|set|customer|%s|",label);
        buffer[MBUFFER-1]=0;
        ErrorPrint(NartData,buffer);
    }
    else
    {
	    SformatOutput(buffer,MBUFFER-1,"|set|customer||");
        buffer[MBUFFER-1]=0;
        ErrorPrint(NartData,buffer);
    }

}

enum
{
	LoadHelp=0,
	LoadDevid,
	LoadSsid,
	LoadBus,
};


static int DeviceIdDefault= -1;
static int CardDevidDefault=0x30;
static struct _ParameterList CardDevidParameter[]=
{
	{0x30,{"osprey",0,0},0,0,0,0,0,0,0,0,0},
	{0x31,{"wasp",0,0},0,0,0,0,0,0,0,0,0},
};


static struct _ParameterList LoadParameter[]=
{
	{LoadDevid,{"devid",0,0},"device type",'x',0,0,0,0,0,0,&DeviceIdDefault,
	    sizeof(CardDevidParameter)/sizeof(CardDevidParameter[0]),CardDevidParameter},
	TEMPLATE_PREFERENCE,
	TEMPLATE_MEMORY_READ,
	TEMPLATE_SIZE_READ,
};

void CardLoadParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(LoadParameter)/sizeof(LoadParameter[0]);
    list->special=LoadParameter;
}

int CardLoad(int client)
{
    unsigned int devid;
	int ndevid;
	int ssid;
	int bus;
	int error;
	int lc, nc;
	char buffer[MBUFFER];
	int ip, np;
	int ngot;
	char *name, *value;
	int code;
	unsigned char macaddr[6];
	int it;
    char label[MLABEL];
    int calmem,ncalmem;
	int address,naddress;
	int preference,npreference;
	int index;
	//
	// prepare beginning of error message in case we need to use it
	//
	lc=0;
    error=0;
	//
	// install default values
	//
	ndevid= -1;
	ncalmem= -1;
	naddress= -1;
	npreference= -1;
	devid=DeviceIdDefault;
    calmem=TemplateMemoryDefaultRead;
	address=TemplateSizeDefaultRead;
	preference=TemplatePreferenceDefault;
	//
	// parse parameters and values
	//
	np=CommandParameterMany();
	for(ip=0; ip<np; ip++)
	{
		name=CommandParameterName(ip);
		index=ParameterSelectIndex(name,LoadParameter,sizeof(LoadParameter)/sizeof(LoadParameter[0]));
		if(index>=0)
		{
			code=LoadParameter[index].code;
			switch(code) 
			{

				case LoadDevid:
					ndevid=ParseHexList(ip,name,&devid,&LoadParameter[index]);
					if(ndevid<=0)
					{
						error++;
					}
					break;
				case TemplateMemory:
					ncalmem=ParseIntegerList(ip,name,&calmem,&LoadParameter[index]);
					if(ncalmem<=0)
					{
						error++;
					}
					break;
				case TemplateSize:
					naddress=ParseIntegerList(ip,name,&address,&LoadParameter[index]);
					if(naddress<=0)
					{
						error++;
					}
					break;
				case TemplatePreference:
					npreference=ParseIntegerList(ip,name,&preference,&LoadParameter[index]);
					if(npreference<=0)
					{
						error++;
					}
					break;
				default:
					error++;
					ErrorPrint(ParseBadParameter,name);
					break;
			}
		}
		else
		{
			error++;
			ErrorPrint(ParseBadParameter,name);
		}
	}

	if(error==0)
	{
	    if(AH==0)
	    {
		    SendOn(client);

			if(npreference==1)
			{
				ar9300EepromTemplatePreference(preference);
			}
			if(naddress==1)
			{
				if(address>0)
				{
					address--;
				}
				ar9300CalibrationDataAddressSet(0,address);
			}
			if(ncalmem==1)
			{
				ar9300CalibrationDataSet(0,calmem);
			}

		    error=CardLoadDo(client,devid,calmem);

		    SendOff(client);
	    }
	    else
	    {
		    //
		    // we do this because the perl parsing expects these messages
		    // when that is fixed, we can remove these two responses
		    //
		    SendOn(client);
            error=0;
		    SendOff(client);
	    }

        if(error==0)
        {
            ConfigurationMacAddressGet(macaddr);
	        devid=DeviceIdGet();
            //
	        // return information to cart
	        //
	        ErrorPrint(NartDataHeader,"|load|devid|memaddr|memsize|mac|");
			SformatOutput(buffer,MBUFFER-1,"|load|%x|%lx|%x|%02x:%02x:%02x:%02x:%02x:%02x|",
		        devid,
                AnwiDriverMemoryMap(),
		        AnwiDriverMemorySize(),		
		        macaddr[0],macaddr[1],macaddr[2],macaddr[3],macaddr[4],macaddr[5]);
            ErrorPrint(NartData,buffer);
            //
            // Send normal card data back to cart
            //
            CardLoadDataSend(client);
        }
        else
        {
            //
            // Send bad card data back to cart
            //
            CardUnloadDataSend(client);
        }
    }
	//
	// announce success
	//
	if(error==0)
	{
		FreeMemoryPrint();
		ErrorPrint(CardLoadGood);
	}
	//
	// say we're done
	//
	SendDone(client);

    return error;
}

//
// Returns 1 is there is a valid card loaded and ready for operation.
//
int CardValid(void)
{
    return (AnwiDriverValid() && DeviceValid);
}


//
// Returns 1 is there is a valid card loaded and ready for operation.
//
int CardValidReset(void)
{
//    return CardValid() & ResetDone;

	int frequency;
	int bandwidth;

	if(!CardValid())
	{
		return 0;
	}
	if(!ResetDone)
	{
		if(CardChannelFind(0,10000,BW_AUTOMATIC,&frequency,&bandwidth)==0)
		{
			CardResetIfNeeded(frequency,CardTxChainMask(),CardRxChainMask(),1,bandwidth);
		}
	}
	return ResetDone;
}


int CardDataSend(int client)
{
    if(CardValid())
    {
        //
        // Send normal card data back to cart
        //
        CardLoadDataSend(client);
	    return 0;
    }
    else
    {
        //
        // Send normal card data back to cart
        //
        CardUnloadDataSend(client);
        return -1;
    }
}


int CardCheckAndLoad(int client)
{
	//
	// if there's no card loaded, try to load it
    // if unsuccessful, return error
	//
	if(!CardValid())
	{
        CardLoadDo(client,-1,0);
        if(CardValid())
        {
            //
            // Send normal card data back to cart
            //
            CardLoadDataSend(client);
		    return 0;
        }
        else
        {
            //
            // Send normal card data back to cart
            //
            CardUnloadDataSend(client);
            return -1;
        }
	}
    return 0;
}

