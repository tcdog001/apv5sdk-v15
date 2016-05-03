

#include <stdio.h>
#include <stdlib.h>

#include "wlantype.h"
#include "smatch.h"
#include "UserPrint.h"
#include "TimeMillisecond.h"
#include "CommandParse.h"
#include "ParameterSelect.h"	// 10/30/09 cz
#include "ParameterParse.h"		// 10/30/09 cz
#include "NewArt.h"
#include "MyDelay.h"
#include "Card.h"
#include "Link.h"
#include "rate_constants.h"

#include "LinkList.h"

#include "Device.h"

#include "LinkStat.h"

#include "LinkRxStat.h"
#include "LinkRx.h"

//#include "LinkTxStat.h"
#include "LinkTx.h"

#include "Carrier.h"

#include "Field.h"

#include "Calibrate.h"
#include "NoiseFloor.h"
#include "Ar9300Device.h"

#include "ah.h"
#include "ah_internal.h"
#include "ar9300eep.h"
#include "ar9300.h"
#include "Ar9300EepromStructSet.h"
#include "mEepStruct9300.h"

#include "ErrorPrint.h"
#include "ParseError.h"
#include "CardError.h"
#include "NartError.h"
#include "LinkError.h"

extern struct ath_hal *AH;


#define MBUFFER 1024
#define MRATE 200

static int _LinkClient;

static int _LinkCalibrateCount=0;

enum {TpcmTxGain=0, TpcmTxPower, TpcmTargetPower};


static int CalibratePowerMean=11;
static int CalibratePowerDeviation=1;
static int CalibrateTxGainMinimum=0;
static int CalibrateTxGainMaximum=100;
static int CalibrateAttemptMaximum=5;
static int CalibratePowerAdjust[MCHAIN];
static int CalibrateAttempt[MCHAIN];

//
// control parameters
//
static int _LinkFrequency=2412;
static int _LinkTxChain=0x7;
static int _LinkRxChain=0x7;
static int _LinkShortGI=0;
static int _LinkStat=3;
static int _LinkIss=0;
static int _LinkAtt=0;
static int _LinkIfs= -1;				// -1 means regular mode, 0 means tx100, >0 means tx99
static int _LinkDeafMode=0;	
static int _LinkCWmin= -1;
static int _LinkCWmax= -1;
static int _LinkPromiscuous=0;
static int _LinkNdump=0;
static int _LinkRetryMax=15;
static int _LinkPacketLength=1000;
static int _LinkPacketMany=100;
static int _LinkDuration= -1;
static int _LinkBc=1;
static int _LinkIr=0;
static int _LinkAgg=0;
static int _LinkReset;					// 1=reset before op, 0=no reset before op, -1=auto
static int _Papd_reset=0;
static int _LinkCarrier=0;				// if set, output carrier only

#define MPATTERN 100
static int _LinkPatternLength=0;
static unsigned char _LinkPattern[MPATTERN];	

static int _LinkAntenna=0;					// nothing sets this

static int _LinkTpcm=TpcmTargetPower;
static double _LinkTransmitPower= -1;		// -1 means use target power, otherwise tx power in dBm, only if _LinkTpcm=TpcmClosed
static int _LinkPcdac=30;
static int _LinkPdgain=3;

static int _LinkTxGainIndex= -1;
static int _LinkTxDacGain= -1;

static int _LinkCalibrate=0;		// 0=none, 1=combined; 2=individual chains
static int _LinkCalibrateChain=0;

static int _LinkNoiseFloor;
static int _LinkNoiseFloorControl[MCHAIN],_LinkNoiseFloorExtension[MCHAIN];
static int _LinkRssiCalibrate=0;

static int _LinkRxIqCal=0;

static int _LinkXtalCal=0;
static double _LinkFrequencyActual=0;

static int _LinkChipTemperature=0;


static int _LinkRate[MRATE];
static int _LinkRateMany=0;
static int _LinkBandwidth=BW_AUTOMATIC;

int temperature_before_reading=0;

//
// these are the default values
//
static unsigned char bssID[6]={0x50,0x55,0x55,0x55,0x55,0x05};
static unsigned char rxStation[6]={0x10,0x11,0x11,0x11,0x11,0x01};   // DUT
static unsigned char txStation[6]={0x20,0x22,0x22,0x22,0x22,0x02};    // Golden

static unsigned char _LinkBssId[6]     ;
static unsigned char _LinkRxStation[6] ;   // DUT
static unsigned char _LinkTxStation[6];    // Golden
static unsigned short _LinkAssocId=0;		// i don't know what this is???

    static enum {NO_MODE, TX_MODE, RX_MODE, WAIT_MODE, LOOP_MODE, EXIT_MODE, CARRIER_MODE} LinkMode;




static int MessageWait(int timeout)
{
	int nread;
	char buffer[MBUFFER];
	int start,end;
	int client;

	start=TimeMillisecond();
//	UserPrint("MessageWait start\n");

	while(1)
	{
		nread=CommandNext(buffer,MBUFFER-1,&client);
	    //
	    // Got data. Process it.
		//
		if(nread>0)
		{
			if(Smatch(buffer,"START"))
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else if(nread<0)
		{
			return -1;
		}
		//
		// dont wait forever
		//
		end=TimeMillisecond();
		if(end-start>timeout)
		{
			UserPrint("%9d message wait timeout\n",end);
			break;
		}
		UserPrint("+");
		MyDelay(50);
	}
	return -1;
}


static int LinkPowerRequest(int client, int txchain)
{
	char buffer[MBUFFER];

	SformatOutput(buffer,MBUFFER-1,"cal txchain=%x;power=?",txchain);
	buffer[MBUFFER-1]=0;
	ErrorPrint(NartRequest,buffer);

	return 0;
}


static int LinkPowerRequestAll(int client, int isolated, int txchain)
{
	int ich;

	if(isolated)
	{
		for(ich=0; ich<MCHAIN; ich++)
		{
			if(txchain&(1<<ich))
			{
				LinkPowerRequest(client,(1<<ich));
			}
		}
	}
	else
	{
		LinkPowerRequest(client,txchain);
	}
    return 0;
}

static void XtalCalibrateStatsHeader()
{
	ErrorPrint(NartDataHeader,"|xtal|frequency|txchain|txgain|txpower||factual|ppm|cap|");
}

static void XtalCalibrateStatsReturn(int frequency, int txchain, int txgain, double txpower, double factual, int ppm, int cap)
{
    char buffer[MBUFFER];

    SformatOutput(buffer,MBUFFER-1,"|xtal|%d|%d|%d|%.1lf||%.6lf|%d|%d|",
        frequency,txchain,txgain,txpower,factual,ppm,cap);
    ErrorPrint(NartData,buffer);
}

static int CarrierFrequencyRequest(int client, int txchain)
{
	char buffer[MBUFFER];

	SformatOutput(buffer,MBUFFER-1,"xtal txchain=%x; frequency=?",txchain);
	buffer[MBUFFER-1]=0;
	ErrorPrint(NartRequest,buffer);

	return 0;
}

static int CarrierFrequencyRequestAll(int client, int isolated, int txchain)
{
	int ich;

	if(isolated)
	{
		for(ich=0; ich<MCHAIN; ich++)
		{
			if(txchain&(1<<ich))
			{
				CarrierFrequencyRequest(client,(1<<ich));
			}
		}
	}
	else
	{
		CarrierFrequencyRequest(client,txchain);
	}
    return 0;
}


static int LinkTxIsOn()
{
    SendOn(_LinkClient);
	//
	// if we're doing calibration, request power readings
	// answers come back in cal messages which are handled by LinkCalibrate()
	//
	if(_LinkCalibrate)
	{
        temperature_before_reading=DeviceTemperatureGet(1);
		LinkPowerRequestAll(_LinkClient,_LinkCalibrate,_LinkCalibrateChain);
	}
	if(_LinkXtalCal)
	{
		CarrierFrequencyRequestAll(_LinkClient,0,1);
	}
	return 0;
}


static int ChainIdentify(unsigned long chmask)
{
    int it;
    int good;

    good= -1;
    for(it=0; it<MCHAIN; it++)
    {
        if((chmask>>it)&0x1)
        {
            if(good>=0)
            {
                return -1;
            }
            good=it;
        }
    }
    return good;
}

int LinkCalibrate()
{
    int ip, np;
    double power;
    char *name;
    int lc;
    int ngot;
    int error;
	unsigned int txchain;
	int ichain;
	//
	// parse parameters and values
	//
    error=0;
    lc=0;
	np=CommandParameterMany();
	txchain=_LinkTxChain;
	power=-1000;
	for(ip=0; ip<np; ip++)
	{
		name=CommandParameterName(ip);
		if(Smatch(name,"power") || Smatch(name,"pow") || Smatch(name,"p"))
		{
			ngot=SformatInput(CommandParameterValue(ip,0)," %lg ",&power);
			if(ngot!=1)
			{	
	            ErrorPrint(ParseBadValue,CommandParameterValue(ip,0),name);
			    error++;
			}
		}
		else if(Smatch(name,"txch") || Smatch(name,"ch") || Smatch(name,"chain") || Smatch(name,"txchain"))
		{
			ngot=SformatInput(CommandParameterValue(ip,0)," %x ",&txchain);
			if(ngot!=1)
			{	
	            ErrorPrint(ParseBadValue,CommandParameterValue(ip,0),name);
			    error++;
			}
		}
    }
    //
    // send header
    //
    if(_LinkCalibrateCount==0)
    {
        CalibrateStatsHeader(_LinkClient);
    }
    //
    // go off and get all the other data we need
    //
    if(error==0)
	{
		//
		// one shot calibration
		//
		if(_LinkCalibrate==1 || _LinkCalibrate==2)
		{
			CalibrateRecord(_LinkClient,_LinkFrequency,txchain,_LinkPcdac,power);
		}
		//
		// iteration to find good value
		//
		else
		{
			ichain=ChainIdentify(txchain);
			if(power>=CalibratePowerMean-CalibratePowerDeviation && power<=CalibratePowerMean+CalibratePowerDeviation)
			{
				CalibratePowerAdjust[ichain]=0;
				CalibrateRecord(_LinkClient,_LinkFrequency,txchain,_LinkPcdac,power);
			}
			else
			{
				CalibrateRecord(_LinkClient,_LinkFrequency,txchain,_LinkPcdac,power);
				CalibrateAttempt[ichain]++;
				if(CalibrateAttempt[ichain]>CalibrateAttemptMaximum)
				{
					CalibratePowerAdjust[ichain]=0;
					ErrorPrint(CalibrateFail,txchain,_LinkPcdac,power);
				}
				else
				{
					CalibratePowerAdjust[ichain]=_LinkPcdac+2.0*(CalibratePowerMean-power);
					if(CalibratePowerAdjust[ichain]>CalibrateTxGainMaximum)
					{
						if(_LinkPcdac>=CalibrateTxGainMaximum)
						{
							CalibratePowerAdjust[ichain]=0;
							ErrorPrint(CalibrateFail,txchain,_LinkPcdac,power);
						}
						else
						{
							CalibratePowerAdjust[ichain]=CalibrateTxGainMaximum;
						}
					}
					if(CalibratePowerAdjust[ichain]<CalibrateTxGainMinimum)
					{
						if(_LinkPcdac<=CalibrateTxGainMinimum)
						{
							CalibratePowerAdjust[ichain]=0;
							ErrorPrint(CalibrateFail,txchain,_LinkPcdac,power);
						}
						else
						{
							CalibratePowerAdjust[ichain]=CalibrateTxGainMinimum;
						}
					}
				}
			}
		}
		//
		// check off this chain as being done. if there are no more to do
		// then abort transmission
		//
		_LinkCalibrateChain &= ~(txchain);
		if(_LinkCalibrateChain==0)
		{
			return 1;			// signal to stop transmission
		}
	}

    return 0;
}

int LinkXtalCal()
{
    int ip, np;
    double ff;
    char *name;
    int lc;
    int ngot;
    int error;
	unsigned long txchain;
	int ichain;
	//
	// parse parameters and values
	//
    error=0;
    lc=0;
	np=CommandParameterMany();
	txchain=_LinkTxChain;
	ff=0;
	for(ip=0; ip<np; ip++)
	{
		name=CommandParameterName(ip);
		if(Smatch(name,"frequency"))
		{
			ngot=SformatInput(CommandParameterValue(ip,0)," %lg ",&ff);
			if(ngot!=1)
			{	
	            ErrorPrint(ParseBadValue,CommandParameterValue(ip,0),name);
			    error++;
			}
		}
		else if(Smatch(name,"txch") || Smatch(name,"ch") || Smatch(name,"chain") || Smatch(name,"txchain"))
		{
			ngot=SformatInput(CommandParameterValue(ip,0)," %x ",&txchain);
			if(ngot!=1)
			{	
	            ErrorPrint(ParseBadValue,CommandParameterValue(ip,0),name);
			    error++;
			}
		}
    }
    //
    // go off and get all the other data we need
    //
    if(error==0)
	{
		_LinkFrequencyActual = ff;
	}
//	UserPrint("_LinkFrequencyActual=%f\n",_LinkFrequencyActual);

    return 1;			// signal to stop transmission
}

//
// Called from inside LinkRxComplete.
// Checks for messages from cart.
// Returns 0 to indicate that the operation should continue.
//
static int LinkStopIt()
{
	int nread;
	char buffer[MBUFFER];
	int client;
    char *word;
	int caldone;

	if(_LinkRxIqCal)
	{
		caldone=CardRxIqCalComplete();
		if(caldone)
		{
			return 2;		// immediate stop
		}
	}

	nread=CommandNext(buffer,MBUFFER-1,&client);
	//
	// Got data. Process it.
	//
	if(nread>0)
	{
		//
		// Dispatch
		//
		CommandParse(buffer);
		word=CommandWord();

		if(Smatch(word,"STOP"))
		{
			return 1;	// read remaining packets
		}
		else if(Smatch(word,"cal"))
		{
            return LinkCalibrate();
		}
		else if(Smatch(word,"xtal")||Smatch(word,"xtalcal"))
		{
            return LinkXtalCal();
		}
	}
    return 0;
}


static void LinkRateCheck()
{
	int ir,im;
	int rfrequency,rbandwidth;

	//
	// if frequency is 5GHz, eliminate any legacy rates 
	//
	if(_LinkFrequency>4000)
	{
		for(ir=0; ir<_LinkRateMany; ir++)
		{
			if(IS_11B_RATE_INDEX(_LinkRate[ir]))
			{
				ErrorPrint(LinkRateRemove11B,rateStrAll[_LinkRate[ir]]);
				_LinkRateMany--;
				for(im=ir; im<_LinkRateMany; im++)
				{
					_LinkRate[im]=_LinkRate[im+1];
				}
				ir--;
			}
		}
	}
	//
	// Figure out bandwidth from frequency
	//
	if (_LinkBandwidth==BW_AUTOMATIC)
	{
		CardChannelFind(_LinkFrequency,_LinkFrequency,_LinkBandwidth,&rfrequency,&rbandwidth);
	}
	else 
	{
		rbandwidth=_LinkBandwidth;
	}

	//
	// if ht40 is forced off, or half or quarter rates enabled, eliminate any ht40 rates
	//
	if(rbandwidth==BW_HT20 || rbandwidth==BW_QUARTER || rbandwidth==BW_HALF)
	{
		for(ir=0; ir<_LinkRateMany; ir++)
		{
			if(IS_HT40_RATE_INDEX(_LinkRate[ir]))
			{
				ErrorPrint(LinkRateRemoveHt40,rateStrAll[_LinkRate[ir]]);
				_LinkRateMany--;
				for(im=ir; im<_LinkRateMany; im++)
				{
					_LinkRate[im]=_LinkRate[im+1];
				}
				ir--;
			}
		}
	}
	//
	// if or half or quarter rates enabled, eliminate any ht20 rates
	//
	if(rbandwidth==BW_QUARTER || rbandwidth==BW_HALF)
	{
		for(ir=0; ir<_LinkRateMany; ir++)
		{
			if(IS_HT20_RATE_INDEX(_LinkRate[ir]))
			{
				ErrorPrint(LinkRateRemoveHt20,rateStrAll[_LinkRate[ir]]);
				_LinkRateMany--;
				for(im=ir; im<_LinkRateMany; im++)
				{
					_LinkRate[im]=_LinkRate[im+1];
				}
				ir--;
			}
		}
	}
	//
	// ###### SHOULD CHECK 2 STREAM and 3 STREAM RATES AGAINST CHAIN MASK HERE.
	//
	// if autoconfiguring ht40, see if any ht40 rates are requested
	//
	if(_LinkBandwidth==BW_AUTOMATIC && (rbandwidth==BW_HT40_PLUS||rbandwidth==BW_HT40_MINUS ))
	{
		for(ir=0; ir<_LinkRateMany; ir++)
		{
			if(IS_HT40_RATE_INDEX(_LinkRate[ir]))
			{
				break;
			}
		}
		if(ir>=_LinkRateMany)
		{
			_LinkBandwidth=BW_HT20;
		}
		else 
		{
			_LinkBandwidth=rbandwidth;
		}
	}
	else 
	{
		_LinkBandwidth=rbandwidth;
	}
}


static int MacCompare(unsigned char *m1, unsigned char *m2)
{
	int it;

	for(it=0; it<6; it++)
	{
		if(m1[it]!=m2[it])
		{
			return 0;
		}
	}
	return 1;
}


static void MacSave(unsigned char *dest, unsigned char *source)
{
	int it;

	for(it=0; it<6; it++)
	{
		dest[it]=source[it];
	}
}


static double HistogramMean(int *data, int ndata)
{
	int it;
	int count;
	double mean;
	//
	// compute mean
	//
	count=0;
	mean=0;
	for(it=0; it<ndata; it++)
	{
		if(data[it]>0)
		{
			//
			// accumulate data for mean
			//
			count+=data[it];
			mean+=(it*data[it]);
		}
	}
	//
	// finish computing mean value
	//
	if(count>0)
	{
		mean/=count;
	}
	return mean;
}


static int HistogramPrint(char *buffer, int max, int *data, int ndata)
{
	int it, is;
	int dmin,dmax;
	int count;
	double mean;
	int lc, nc;
    int ndup;
	//
	// find minimum and maximum nonzero data
	// also compute mean
	//
	dmin= -1;
	dmax= -1;
	count=0;
	mean=0;
	for(it=0; it<ndata; it++)
	{
		if(data[it]>0)
		{
			if(dmin<0)
			{
				dmin=it;
			}
			dmax=it;
			//
			// accumulate data for mean
			//
			count+=data[it];
			mean+=(it*data[it]);
		}
	}
	//
	// finish computing mean value
	//
	if(count>0)
	{
		mean/=count;
	}
	lc=SformatOutput(buffer,max-1,"%.1lf",mean);
	if(count>0)	//dmin>=0 && dmin!=dmax)
	{
		nc=SformatOutput(&buffer[lc],max-lc-1,":%d",data[dmin]);
		if(nc>0)
		{
			lc+=nc;
		}
	    for(it=dmin+1; it<=dmax; it++)
		{
            //
            // count number of duplicate values
            //
            for(is=it+1; is<=dmax; is++)
            {
                if(data[it]!=data[is])
                {
                    break;
                }
            }
            ndup=is-it;
            if(ndup<=1)
            {
		        nc=SformatOutput(&buffer[lc],max-lc-1,",%d",data[it]);
		        if(nc>0)
			    {
			        lc+=nc;
			    }
            }
            else
            {
		        nc=SformatOutput(&buffer[lc],max-lc-1,",%d*%d",data[it],ndup);
		        if(nc>0)
			    {
			        lc+=nc;
			    }
                it=is-1;
            }
		}
	}
	buffer[max-1]=0;
	return lc;
}


static int HistogramPrintWithBar(char *buffer, int max, int *data, int ndata)
{
	int lc, nc;

	lc=HistogramPrint(buffer,max,data,ndata);
	nc=SformatOutput(&buffer[lc],max-lc-1,"|");
	if(nc>0)
	{
		lc+=nc;
	}
	return lc;
}


//
// exported to test.c to complete menu structure
//
static struct _ParameterList LinkParameter[]=
{	
	LINK_FREQUENCY(1),
	LINK_RATE(MRATE), 
	LINK_INTERLEAVE,
	LINK_HT40,
	LINK_PACKET_COUNT(1), 
	LINK_AGGREGATE(1),
	LINK_DURATION,
	LINK_PACKET_LENGTH(1), 
	LINK_TRANSMIT_POWER(1),
	LINK_TXGAIN(1),
	LINK_BROADCAST,
	LINK_RETRY,
	LINK_TX99,
	LINK_TX100,
	LINK_CARRIER,
	LINK_CHAIN(1),
	LINK_TX_CHAIN(1), 
	LINK_RX_CHAIN,
	LINK_DUMP,
	LINK_PROMISCUOUS,
	LINK_BSSID,
	LINK_MAC_TX,
	LINK_MAC_RX,
	LINK_ATTENUATION(1),		// #### shouldn't be passed to nart
	LINK_ISS(1),				// #### shouldn't be passed to nart
	LINK_CALIBRATE,
	LINK_CALIBRATE_GOAL,
	LINK_CALIBRATE_TX_GAIN_MINIMUM,
	LINK_CALIBRATE_TX_GAIN_MAXIMUM,
	LINK_NOISE_FLOOR,
	LINK_RSSI_CALIBRATE,
	LINK_RX_IQ_CAL,
	LINK_AVERAGE,
	LINK_RESET,
	LINK_PDGAIN,
	LINK_STATISTIC,
	LINK_GUARD_INTERVAL,
	LINK_INTERFRAME_SPACING,
	LINK_DEAF_MODE,
	LINK_PATTERN(1),
	LINK_CHIP_TEMPERATURE,
	LINK_BANDWIDTH,
	LINK_XTAL_CAL,
};


void LinkParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(LinkParameter)/sizeof(LinkParameter[0]);
    list->special=LinkParameter;
}


static int ParseInput(int client)
{
 	int ip,np;
	char *name;
	int lc, nc;
	char buffer[MBUFFER];
	int error;
	int it;
	int ngot;
	int index;
	int code;			
	int ipattern[MPATTERN];
	int pattern[MPATTERN];
	int rlegacy,rht20,rht40,extra;
	int nvalue;
	int rerror;
	double dpcdac;
	int tx99;
	int tx100;
	int _LinkHt40;

    _LinkCalibrateCount=0;
	_LinkChipTemperature=0;
	_LinkNoiseFloor=0;
	_LinkRssiCalibrate=0;
	_LinkPatternLength=0;
	_LinkReset= -1;
	_LinkDuration= -1;
	_LinkPacketLength=1000;
	_LinkPacketMany=100;
	_LinkFrequency=2412;
	_LinkTxChain=0x7;
	_LinkRxChain=0x7;
	_LinkRateMany=1;
	_LinkRate[0]=RATE_INDEX_6;
	_LinkTpcm=TpcmTargetPower;
	_LinkPcdac=30;
	_LinkPdgain=3;
	_LinkTransmitPower= -1;
	_LinkAtt= -1;
	_LinkIss= -1;
	_LinkBc=1;
	_LinkRetryMax=0;
	_LinkStat=3;	
	_LinkIr=0;
	_LinkHt40=2;
	_LinkAgg=1;
	_LinkIfs= -1;
	_LinkDeafMode=0;
	_LinkCWmin= -1;
	_LinkCWmax= -1;
	_LinkNdump=0;
	_LinkPromiscuous=0;
    _LinkShortGI=0;
	_LinkCalibrate=0;
	_LinkCarrier=0;
	_LinkBandwidth=BW_AUTOMATIC;
	_LinkXtalCal=0;
	CalibratePowerMean= -100;
	CalibrateTxGainMinimum=0;
	CalibrateTxGainMaximum=100;
	for(it=0; it<6; it++)
	{
		_LinkBssId[it]=bssID[it];
		_LinkTxStation[it]=txStation[it];
		_LinkRxStation[it]=rxStation[it];
	}
	_LinkRxIqCal=0;
	tx99=0;
	tx100=0;
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
		index=ParameterSelectIndex(name,LinkParameter,sizeof(LinkParameter)/sizeof(LinkParameter[0]));
		if(index>=0)
		{
			code=LinkParameter[index].code;
			switch(code) 
			{
				case LinkParameterFrequency:
					ngot=ParseIntegerList(ip,name,&_LinkFrequency,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterChipTemperature:
					ngot=ParseIntegerList(ip,name,&_LinkChipTemperature,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterTransmitPower:
					ngot=ParseDoubleList(ip,name,&_LinkTransmitPower,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					else if(_LinkTransmitPower<= -100)
					{
						_LinkTpcm=TpcmTargetPower;
					}
					else
					{
						_LinkTpcm=TpcmTxPower;
					}
					break;
				case LinkParameterPacketLength:
					ngot=ParseIntegerList(ip,name,&_LinkPacketLength,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterDuration:
					ngot=ParseIntegerList(ip,name,&_LinkDuration,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterNoiseFloor:
					ngot=ParseIntegerList(ip,name,&_LinkNoiseFloor,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterRssiCalibrate:
					ngot=ParseIntegerList(ip,name,&_LinkRssiCalibrate,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterRxIqCal:
					ngot=ParseIntegerList(ip,name,&_LinkRxIqCal,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterCalibrate:
					ngot=ParseIntegerList(ip,name,&_LinkCalibrate,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterCalibrateGoal:
					ngot=ParseIntegerList(ip,name,&CalibratePowerMean,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterCalibrateTxGainMinimum:
					ngot=ParseIntegerList(ip,name,&CalibrateTxGainMinimum,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterCalibrateTxGainMaximum:
					ngot=ParseIntegerList(ip,name,&CalibrateTxGainMaximum,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterInputSignalStrength:
					ngot=ParseIntegerList(ip,name,&_LinkIss,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterAttenuation:
					ngot=ParseIntegerList(ip,name,&_LinkAtt,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterPdgain:
					ngot=ParseIntegerList(ip,name,&_LinkPdgain,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterPcdac:
					//
					// check for double precision because that is what litepoint sends, 8/3/2010
					//
					ngot=sscanf(CommandParameterValue(ip,0)," %lg %1c",&dpcdac,&extra);
					if(ngot==1)
					{
						_LinkPcdac=(int)dpcdac;
						_LinkTpcm=TpcmTxGain;
					}
					else
					{
						ngot=ParseIntegerList(ip,name,&_LinkPcdac,&LinkParameter[index]);
						if(ngot<=0)
						{
							error++;
						}
						else
						{	
							_LinkTpcm=TpcmTxGain;
						}
					}
					break;
				case LinkParameterHt40:
					ngot=ParseIntegerList(ip,name,&_LinkHt40,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					else
					{
						switch(_LinkHt40){
						case 0:
							_LinkBandwidth=BW_HT20;
							break;
						case 1:
							_LinkBandwidth=BW_HT40_PLUS;
							break;
						case -1:
							_LinkBandwidth=BW_HT40_MINUS;
							break;
						case 2:
							_LinkBandwidth=BW_AUTOMATIC;
							break;
						default:
							error++;
							break;
						}
					}
					break;
				case LinkParameterBandwidth:
					ngot=ParseIntegerList(ip,name,&_LinkBandwidth,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterInterleaveRates:
					ngot=ParseIntegerList(ip,name,&_LinkIr,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterBroadcast:
					ngot=ParseIntegerList(ip,name,&_LinkBc,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterRetry:
					ngot=ParseIntegerList(ip,name,&_LinkRetryMax,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterAggregate:
					ngot=ParseIntegerList(ip,name,&_LinkAgg,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterGuardInterval:
					ngot=ParseIntegerList(ip,name,&_LinkShortGI,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterChain:
					ngot=ParseIntegerList(ip,name,&_LinkTxChain,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					else
					{
						_LinkRxChain=_LinkTxChain;
					}
					break;
				case LinkParameterTxChain:
					ngot=ParseIntegerList(ip,name,&_LinkTxChain,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterRxChain:
					ngot=ParseIntegerList(ip,name,&_LinkRxChain,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterStatistic:
					ngot=ParseIntegerList(ip,name,&_LinkStat,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterReset:
					ngot=ParseIntegerList(ip,name,&_LinkReset,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterPromiscuous:
					ngot=ParseIntegerList(ip,name,&_LinkPromiscuous,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterDump:
					ngot=ParseIntegerList(ip,name,&_LinkNdump,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterDeafMode:
					ngot=ParseIntegerList(ip,name,&_LinkDeafMode,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterInterframeSpacing:
					ngot=ParseIntegerList(ip,name,&_LinkIfs,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					break;
				case LinkParameterCarrier:
					ngot=ParseIntegerList(ip,name,&_LinkCarrier,&LinkParameter[index]);		
					if(ngot<=0)
					{
						error++;
					}
					else
					{
						if(_LinkCarrier!=0)
						{
							tx100=1;
							tx99=0;
							_LinkIfs=0;
						}
					}
					break;
				case LinkParameterTx99:
					ngot=ParseIntegerList(ip,name,&tx99,&LinkParameter[index]);		
					if(ngot<=0)
					{
						error++;
					}
					else
					{
						if(tx99!=0)
						{
							tx100=0;
							_LinkIfs=1;
						}
						else
						{
							_LinkIfs= -1;
						}
						}
					break;
				case LinkParameterTx100:
					ngot=ParseIntegerList(ip,name,&tx100,&LinkParameter[index]);		
					if(ngot<=0)
					{
						error++;
					}
					else
					{
						if(tx100!=0)
						{
							tx99=0;
							_LinkIfs=0;
						}
						else
						{
							_LinkIfs= -1;
						}
					}
					break;
				case LinkParameterPacketCount:
					ngot=ParseIntegerList(ip,name,&_LinkPacketMany,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
//					else if(_LinkPacketMany<0)
//					{
//						_LinkPacketMany=0;
//					}
					break;
				case LinkParameterPattern:
					ngot=ParseHexList(ip,name,pattern,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					else
					{
						for(it=0; it<ngot; it++)
						{
							_LinkPattern[it]=pattern[it];
						}
					}
					break;
				case LinkParameterRate:
					nvalue=CommandParameterValueMany(ip);
					//
					// check if it might be the old mask codes
					//
					rerror=1;
					if(nvalue==3)
					{
						rerror=0;
						rlegacy=0;
						rht20=0;
						rht40=0;
						ngot=SformatInput(CommandParameterValue(ip,0)," %x %1c",&rlegacy,&extra);
						if(ngot!=1)
						{
							rerror++;
						}
						if(nvalue>=2)
						{
							ngot=SformatInput(CommandParameterValue(ip,1)," %x %1c",&rht20,&extra);
							if(ngot!=1)
							{
								rerror++;
							}
						}
						if(nvalue>=3)
						{
							ngot=SformatInput(CommandParameterValue(ip,2)," %x %1c",&rht40,&extra);
							if(ngot!=1)
							{
								rerror++;
							}
						}
						if(rerror<=0)
						{
							_LinkRateMany=RateCount(rlegacy,rht20,rht40,_LinkRate);
						}
					}
					if(rerror!=0)
					{
						_LinkRateMany=ParseIntegerList(ip,name,_LinkRate,&LinkParameter[index]);
						if(_LinkRateMany<=0)
						{
							error++;
						}
						else
						{
							_LinkRateMany=RateExpand(_LinkRate,_LinkRateMany);
						}
					}
					break;
				case LinkParameterBssId:
					if(ParseMacAddress(CommandParameterValue(ip,0),_LinkBssId))
					{
						ErrorPrint(ParseBadValue,CommandParameterValue(ip,0),name);
						error++;
					}
					break;
				case LinkParameterMacTx:
					if(ParseMacAddress(CommandParameterValue(ip,0),_LinkTxStation))
					{
						ErrorPrint(ParseBadValue,CommandParameterValue(ip,0),name);
						error++;
					}
					break;
				case LinkParameterMacRx:
					if(ParseMacAddress(CommandParameterValue(ip,0),_LinkRxStation))
					{
						ErrorPrint(ParseBadValue,CommandParameterValue(ip,0),name);
						error++;
					}
					break;
				case LinkParameterXtalCal:
					ngot=ParseIntegerList(ip,name,&_LinkXtalCal,&LinkParameter[index]);
					if(ngot<=0)
					{
						error++;
					}
					else
					{
						if(_LinkXtalCal)
						{
							_LinkCarrier=1;
						}
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

	if(error!=0)
	{
		return error;
	}

	//
	// if there's no card loaded, return error
	//
	if(CardCheckAndLoad(client)!=0)
	{
		ErrorPrint(CardNoneLoaded);
		return -CardNoneLoaded;
	}

	//
	//  restrict chain mask to maximum supported by chip
	//
	_LinkRxChain&=CardRxChainMask();
	_LinkTxChain&=CardTxChainMask();

	LinkRateCheck();

	if(_LinkRateMany<=0 && !_LinkCarrier)
	{
		return -1;
	}

	if(_Papd_reset)
	{
		_LinkReset = 1;
		_Papd_reset = 0;
	}


	return 0;
}

//
// code for transmit side of link test
//

static void LinkTransmitStatsHeader()
{
	ErrorPrint(NartDataHeader,"|tx|frequency|tp|txchain|iss|att|pdgain|txgain|rate|pl|pc|agg||correct|throughput|error|fifo|excess|retry|dretry|rssi|rssi00|rssi01|rssi02|rssi10|rssi11|rssi12|txgi|dacg|byte|duration|temp|volt|");
}


static void LinkTransmitStatsReturn(int channel, 
	double pout, int txchain,
    int iss, int att, int pdgain, int pcdac, 
	char *rate, int plength, int expected, int agg,
	int correct, 
	int tput, 
	int other, int under, 
	int excess, 
	int *retry, int mretry,
	int *dretry, int mdretry,
	int *rssi, int mrssi,
	int *rssi00, int mrssi00,
	int *rssi01, int mrssi01,
	int *rssi02, int mrssi02,
    int *rssi10, int mrssi10,
    int *rssi11, int mrssi11,
    int *rssi12, int mrssi12,
	int txgi, int dacg,
    int byte, int duration,
	int temp, int volt)
{
	char buffer[MBUFFER];
	int lc, nc;
    //
	// format message
	//
	lc=SformatOutput(buffer,MBUFFER-1,"|tx|%d|%.1f|%d|%d|%d|%d|%d|%s|%d|%d|%d||%d|%d|%d|%d|%d|",
        channel,
		pout,
        txchain,
		iss,
		att,
		pdgain,
		pcdac,
        rate,
		plength,
        expected,
		agg,
        correct,
        tput,
		other,
		under,
		excess);
	//
	// add retry histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,retry,mretry);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// add dretry histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,dretry,mdretry);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// add rssi histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi,mrssi);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// add rssi histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi00,mrssi00);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// add rssi histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi01,mrssi01);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// add rssi histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi02,mrssi02);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// add rssi histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi10,mrssi10);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// add rssi histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi11,mrssi11);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// add rssi histogram
	//
    nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi12,mrssi12);
	if(nc>0)
	{
		lc+=nc;
	}
    nc=SformatOutput(&buffer[lc],MBUFFER-lc,"%d|%d|",txgi,dacg);
	if(nc>0)
	{
		lc+=nc;
	}
    nc=SformatOutput(&buffer[lc],MBUFFER-lc,"%d|%d|",byte,duration);
	if(nc>0)
	{
		lc+=nc;
	}
    nc=SformatOutput(&buffer[lc],MBUFFER-lc,"%d|%d|",temp,volt);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// send it
	//
	ErrorPrint(NartData,buffer);
}


static void LinkTransmitStatPrint()
{
    int j;
    struct txStats *tStats;
    double tp;
	int temp, volt;
	int nf[MCHAIN],nfc[MCHAIN],nfe[MCHAIN],nfcal[MCHAIN],nfpower[MCHAIN];
	int it;

	NoiseFloorFetch(nfc,nfe,MCHAIN);
	for(it=0; it<MCHAIN; it++)
	{
		nf[it]=(nfc[it]+nfe[it])/2;
		nfcal[it]=ar9300NoiseFloorGet(AH, _LinkFrequency, it);
		nfpower[it]=ar9300NoiseFloorPowerGet(AH, _LinkFrequency, it);
		UserPrint("%d: %5d %5d %5d %5d %5d\n",it,nfc[it],nfe[it],nf[it],nfcal[it],nfpower[it]);
	}
    //
    // make return messages
   	// received vs correct, chain1rssi, chain2rssi, chain3rssi???????
   	//
   	if(_LinkStat&2)
   	{               
		temp=DeviceTemperatureGet(1);
		volt=DeviceVoltageGet();
    	LinkTransmitStatsHeader();
   		//
   		// these stats aren't accurate
   		// it looks like the library does not compute and store startTime or endTime
   		// so we made up our own. not accurate if more than 1 rate used
   		//
        for (j = 0; j < _LinkRateMany; j++) 
   		{
            tStats=LinkTxStatFetch(_LinkRate[j]);
            if(_LinkTpcm==TpcmTargetPower)
            {
                DeviceTargetPowerGet(_LinkFrequency,_LinkRate[j],&tp);
            }
            else
            {
                tp=_LinkTransmitPower;
            }
            LinkTransmitStatsReturn(_LinkFrequency,
   				tp,_LinkTxChain,
                _LinkIss,_LinkAtt,_LinkPdgain,_LinkPcdac,
				rateStrAll[_LinkRate[j]],
				_LinkPacketLength,_LinkPacketMany,_LinkAgg,
				tStats->goodPackets,tStats->newThroughput,
				tStats->otherError,tStats->underruns,tStats->excessiveRetries,
				tStats->shortRetry, MRETRY,
				tStats->longRetry, MRETRY,
				tStats->rssi, MRSSI,
				tStats->rssic[0], MRSSI,
				tStats->rssic[1], MRSSI,
				tStats->rssic[2], MRSSI,
				tStats->rssie[0], MRSSI,
				tStats->rssie[1], MRSSI,
				tStats->rssie[2], MRSSI,
				_LinkTxGainIndex,_LinkTxDacGain,
                tStats->byteCount,tStats->endTime-tStats->startTime,
				temp,volt);
		}
#ifdef DOALL
        tStats=LinkTxStatTotalFetch();
        LinkTransmitStatsReturn(_LinkFrequency,
			_LinkTransmitPower,_LinkTxChain,
            _LinkIss,_LinkAtt,_LinkPdgain,_LinkPcdac,
			"ALL",
			_LinkPacketLength,_LinkPacketMany,_LinkAgg,
			tStats->goodPackets,tStats->newThroughput,
			tStats->otherError,tStats->underruns,tStats->excessiveRetries,
			tStats->shortRetry, MRETRY,
			tStats->longRetry, MRETRY,
			tStats->rssi, MRSSI,
			tStats->rssic[0], MRSSI,
			tStats->rssic[1], MRSSI,
			tStats->rssic[2], MRSSI,
			tStats->rssie[0], MRSSI,
			tStats->rssie[1], MRSSI,
			tStats->rssie[2], MRSSI,
			_LinkTxGainIndex,_LinkTxDacGain,
            tStats->byteCount,tStats->endTime-tStats->startTime);
#endif
	}
}



static void LinkTransmitDoIt()
{
    LinkTxStart();

    LinkTxComplete(_LinkDuration, LinkTxIsOn, LinkStopIt, _LinkChipTemperature);	

	SendOff(_LinkClient);
}


static int doReset()
{
	int error;

	error=CardResetIfNeeded(_LinkFrequency,_LinkTxChain,_LinkRxChain,_LinkReset,_LinkBandwidth);
	if(error!=0)
	{
		return error;
	}

	return 0;
}



static int LinkTransmitFirstTime()
{
	int error;
	//
	// should we do a reset?
	// yes, if the user explicitly asked for it (_LinkReset>0) or if
	// the user asked for auto mode (_LinkReset<0) and any important parameters have changed
	//
    error=doReset();

	if(error!=0)
	{
		return error;
	}

	//
    // set power control mode
    //
    _LinkTxGainIndex= -1;
	_LinkTxDacGain= -1;

	if(_LinkTpcm==TpcmTxGain)
	{
		_LinkTxGainIndex=DeviceTransmitGainSet(_LinkFrequency>=4000,_LinkPcdac);
		FieldRead("forced_dac_gain", (unsigned int *)&_LinkTxDacGain);	
//		TpcmLast=TpcmTxGain;
		UserPrint("TxGainIndex=%d TxDacGain=%d\n",_LinkTxGainIndex,_LinkTxDacGain);
	}
	else if(_LinkTpcm==TpcmTxPower)
	{
		DeviceTransmitPowerSet(_LinkFrequency>=4000,_LinkTransmitPower);
//        TpcmLast=TpcmTxPower;
		UserPrint("tx power=%lg\n",_LinkTransmitPower);
	}
    else
    {
        DeviceTargetPowerApply(_LinkFrequency);
//        TpcmLast=TpcmTargetPower;
 		UserPrint("TARGET POWER\n");
   }
	//
	// get all of the packets descriptors ready to run
	//
    LinkTxSetup(_LinkRate, _LinkRateMany, _LinkIr,
		_LinkBssId, _LinkTxStation, _LinkRxStation, 
		_LinkPacketMany, _LinkPacketLength, _LinkRetryMax, _LinkAntenna, _LinkBc, _LinkIfs,
		_LinkShortGI, _LinkTxChain, _LinkAgg,
		_LinkPattern, _LinkPatternLength);

	return 0;
}


static void TargetPowerGoal()
{
	int ir;
	double tp,tptotal;
	int count;

	tptotal=0;
	count=0;
	//
	// default target power goal is average over ht20 mcs rates 
	//
	for(ir=RATE_INDEX_HT20_MCS0; ir<=RATE_INDEX_HT20_MCS23; ir++)
	{
		if(DeviceTargetPowerGet(_LinkFrequency,ir,&tp)==0)
		{
			tptotal+=tp;
			count++;
		}
	}
	if(count>0)
	{
		CalibratePowerMean=(int)(0.5+(tptotal/((double)count)));
	}
	else
	{
		CalibratePowerMean=10.0;
	}
}


static void Ar9300CarrierOnly(int txchain)
{
	if(txchain&0x1)
	{
		FieldWrite("ch0_rxtx3.dacfullscale",1);
	}
	if(txchain&0x2)
	{
		FieldWrite("ch1_rxtx3.dacfullscale",1);
	}
	if(txchain&0x4)
	{
		FieldWrite("ch2_rxtx3.dacfullscale",1);
	}
}

static void Ar9300CarrierOnlyClear(void)
{
	FieldWrite("ch0_rxtx3.dacfullscale",0);
	FieldWrite("ch1_rxtx3.dacfullscale",0);
	FieldWrite("ch2_rxtx3.dacfullscale",0);
}

static void Ar9300SetXtalValue(int caps_val)
{
    FieldWrite("ch0_XTAL.xtal_capindac", caps_val);
    FieldWrite("ch0_XTAL.xtal_capoutdac", caps_val);
}

void LinkTransmit(int client)
{
	int pap;
	int ich;
	int caps=0x40;
	int ppm;
	int step=0x20;

	//
	// parse the input and if it is good
	// get ready to do the receive
	//
	if(ParseInput(client)==0)
	{
		pap=0;
		//
		// if there's no card loaded, return error
		//
		if(CardCheckAndLoad(client)!=0)
		{
			ErrorPrint(CardNoneLoaded);
		}
		else
		{
			//
			// Save client handle so we can send messages back to the right client
			//
			_LinkClient=client;

			if(_LinkCalibrate!=0)
			{
				if(CalibratePowerMean<=-100)
				{
					TargetPowerGoal();
				}

				for(ich=0; ich<MCHAIN; ich++)
				{
					CalibrateAttempt[ich]=0;
					CalibratePowerAdjust[ich]=0;
				}
				_LinkCalibrateChain=_LinkTxChain;

				pap=Ar9300EepromPaPredistortionGet();
				Ar9300EepromPaPredistortionSet(0);
				CardResetForce();
			}
			if (_LinkCarrier!=0)
			{
				pap=Ar9300EepromPaPredistortionGet();
				Ar9300EepromPaPredistortionSet(0);
				CardResetForce();
			}
			if(_LinkXtalCal!=0)
			{
				caps=0x40;
				step=0x20;
				XtalCalibrateStatsHeader();
			}
			LinkMode = TX_MODE;
			//
			// Setup the descriptors for receive
			//
			if(LinkTransmitFirstTime()==0)
			{
				//
				// Acknowledge command from client
				//
				SendOk(client);
				//
				// wait for "START" message
				//
				if(MessageWait(60000)==0)
				{
					//
					// this is where we really do it.
					// turn on the receiver and get the packets
					//
CalibrateAgain:
					if(_LinkCarrier)
					{
						Ar9300CarrierOnly(_LinkTxChain);
					}
					if(_LinkXtalCal)
					{
						Ar9300SetXtalValue(caps);
					}
					LinkTransmitDoIt();	

					if(_LinkCarrier)
					{
						Ar9300CarrierOnlyClear();
					}
					//
					// need to reset to turn off tx100 mode
					//
					if(_LinkIfs==0)
					{
						_LinkReset=1;
						doReset();
					}
					else
					{
						//
						// send statistics back to client
						//
						LinkTransmitStatPrint();
					}

				}
				else
				{
					//
					// need to reset to turn off tx100 mode
					//
					if(_LinkIfs==0)
					{
						_LinkReset=1;
						doReset();
					}
					ErrorPrint(TransmitCancel);
				}
			}
			else 
			{
			    ErrorPrint(TransmitSetup);
			}

			if(_LinkCalibrate!=0)
			{
				Ar9300EepromPaPredistortionSet(pap);
				//
				// did we get good values for all chains?
				//
				if(_LinkCalibrate==3 || _LinkCalibrate==4)
				{
					for(ich=0; ich<MCHAIN; ich++)
					{
						if(CalibratePowerAdjust[ich]!=0)
						{
							_LinkCalibrateChain=(1<<ich);
							_LinkPcdac=CalibratePowerAdjust[ich];
							UserPrint("redo calibration for chain mask=%x at txgain=%d\n",_LinkCalibrateChain,_LinkPcdac);
							LinkTransmitFirstTime();
							goto CalibrateAgain;
						}
					}
				}
				//
				// yes. save it.
				//
				CalibrateSave();
			}
			if(_LinkXtalCal)
			{
				//
				// did we get a good value?
				//
				ppm=(int)(((_LinkFrequencyActual/_LinkFrequency)-1)*1e6);
				XtalCalibrateStatsReturn(_LinkFrequency,_LinkTxChain,_LinkPcdac,_LinkTransmitPower,_LinkFrequencyActual,ppm,caps);
				if(step>0 && (ppm<-2 || ppm>2))
				{
					if(ppm<0)
					{
						caps-=step;
					}
					else if(ppm>0)
					{
						caps+=step;
					}
					step/=2;
					LinkTransmitFirstTime();
					goto CalibrateAgain;
				}
				//
				// yes. save it.
				//
				Ar9300pwrTuningCapsParamsSet(&caps, 0, 0, 0, 1, 0);
			}
			if (_LinkCarrier!=0)
			{
				Ar9300EepromPaPredistortionSet(pap);
			}
		}
	}
	//
	// tell the client we are done
	//
    SendDone(client);
}


void CarrierTransmit(int client)
{
	int error;

	if(ParseInput(client)==0)
	{
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
			_LinkClient=client;

			SendOk(client);
	 
			error=0;
			if(_LinkReset!=0)
			{
				error=doReset();
			}

			if(error==0)
			{
				if(MessageWait(60000)==0)
				{
					//
					// start transmission
					//
					CarrierStart(_LinkFrequency, _LinkTxChain, _LinkDuration, LinkTxIsOn, LinkStopIt);
					SendOff(client);
					//
					// need to reset to turn off carrier mode
					//
					_LinkReset=1;
					doReset();
				}
				else
				{
					ErrorPrint(CarrierCancel);
				}
			}
		}
	}
	SendDone(client);
}


//
// code to run the receiver in a link test
//



static void LinkReceiveStatsHeader()
{
	ErrorPrint(NartDataHeader,"|rx|frequency|tp|iss|att|pdgain|txgain|rate|pl|pc|agg||correct|throughput|error|crc|psr|rssi|rssi00|rssi01|rssi02|rssi10|rssi11|rssi12|evm0|evm1|evm2|byte|duration|rxchain|");
}


static void LinkReceiveStatsReturn(int channel, 
	double pout, int iss, int att, int pdgain, int pcdac, 
	char *rate, int plength, int expected, int agg,
	int error, int crc, int correct, int tput,
	int *rssi, int mrssi,
	int *rssi1, int mrssi1, 
	int *rssi2, int mrssi2, 
	int *rssi3, int mrssi3,
	int *rssi11, int mrssi11, 
	int *rssi12, int mrssi12, 
	int *rssi13, int mrssi13,
	int *evm1, int mevm1,
	int *evm2, int mevm2,
	int *evm3, int mevm3,
    int byte, int duration, int rxchain)
 {
	char buffer[MBUFFER];
	double psr;
	int lc, nc;
	int denom;
    //
	// compute PSR
	//
	if(agg>1)
	{
		denom=agg*(expected/agg);
	}
	else
	{
		denom=expected;
	}
	if(denom>0)
	{
		psr=((100.0*correct)/((double)denom));
	}
	else
	{
		psr=0;
	}
    //
	// format message
	//
	lc=SformatOutput(buffer,MBUFFER-1,"|rx|%d|%.1f|%d|%d|%d|%d|%s|%d|%d|%d||%d|%d|%d|%d|%.3lf|",
        channel,
		pout,
		iss,
		att,
		pdgain,
		pcdac,
        rate,
		plength,
        denom,
		agg,
        correct,
		tput,
        error,
		crc,
		psr);
	
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi,mrssi);
	    if(nc>0)
		{
			lc+=nc;
		}
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi1,mrssi1);
	    if(nc>0)
		{
			lc+=nc;
		}
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi2,mrssi2);
	    if(nc>0)
		{
			lc+=nc;
		}
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi3,mrssi3);
	    if(nc>0)
		{
			lc+=nc;
		}
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi11,mrssi11);
	    if(nc>0)
		{
			lc+=nc;
		}
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi12,mrssi12);
	    if(nc>0)
		{
			lc+=nc;
		}
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,rssi13,mrssi13);
	    if(nc>0)
		{
			lc+=nc;
		}
         nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,evm1,mevm1);
	    if(nc>0)
		{
			lc+=nc;
		}
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,evm2,mevm2);
	    if(nc>0)
		{
			lc+=nc;
		}
        nc=HistogramPrintWithBar(&buffer[lc],MBUFFER-lc,evm3,mevm3);
	    if(nc>0)
		{
			lc+=nc;
		}
    nc=SformatOutput(&buffer[lc],MBUFFER-lc,"%d|%d|%d|",byte,duration, rxchain);
	if(nc>0)
	{
		lc+=nc;
	}
	//
	// send it
	//
	ErrorPrint(NartData,buffer);
}


static void LinkReceiveStatPrint()
{
    int j;
    struct rxStats *rStats;
	int nf[MCHAIN],nfc[MCHAIN],nfe[MCHAIN],nfcal[MCHAIN],nfpower[MCHAIN];
	int it;

	NoiseFloorFetch(nfc,nfe,MCHAIN);
	for(it=0; it<MCHAIN; it++)
	{
		nf[it]=(nfc[it]+nfe[it])/2;
		nfcal[it]=ar9300NoiseFloorGet(AH, _LinkFrequency, it);
		nfpower[it]=ar9300NoiseFloorPowerGet(AH, _LinkFrequency, it);
		UserPrint("%d: %5d %5d %5d %5d %5d\n",it,nfc[it],nfe[it],nf[it],nfcal[it],nfpower[it]);
	}
    //
    // make return messages
	// received vs correct, chain1rssi, chain2rssi, chain3rssi???????
	//
	it=0;
	if(_LinkStat&1)
	{
		LinkReceiveStatsHeader();
        for (j = 0; j < _LinkRateMany; j++) 
		{
            rStats=LinkRxStatFetch(_LinkRate[j]);
            LinkReceiveStatsReturn(_LinkFrequency,
				_LinkTransmitPower,
				_LinkIss,
				_LinkAtt,
				_LinkPdgain,
				_LinkPcdac,
				rateStrAll[_LinkRate[j]],
				_LinkPacketLength,
				_LinkPacketMany,
				_LinkAgg,
				rStats->otherError,
				rStats->crcPackets,
				rStats->goodPackets,
				rStats->rxThroughPut,
				rStats->rssi,MRSSI,
				rStats->rssic[0],MRSSI,
				rStats->rssic[1],MRSSI,
				rStats->rssic[2],MRSSI,
				rStats->rssie[0],MRSSI,
				rStats->rssie[1],MRSSI,
				rStats->rssie[2],MRSSI,
				rStats->evm[0],MEVM,
				rStats->evm[1],MEVM,
				rStats->evm[2],MEVM,
                rStats->byteCount,rStats->endTime-rStats->startTime, _LinkRxChain);
		}
#ifdef DOALL
        rStats=LinkRxStatTotalFetch();
         LinkReceiveStatsReturn(_LinkFrequency,
			_LinkTransmitPower,
			_LinkIss,
			_LinkAtt,
			_LinkPdgain,
			_LinkPcdac,
			"ALL",
			_LinkPacketLength,
			_LinkPacketMany,
			_LinkAgg,
			rStats->otherError,
			rStats->crcPackets,
			rStats->goodPackets,
			rStats->rxThroughPut,
			rStats->rssi,MRSSI,
			rStats->rssic[0],MRSSI,
			rStats->rssic[1],MRSSI,
			rStats->rssic[2],MRSSI,
			rStats->rssie[0],MRSSI,
			rStats->rssie[1],MRSSI,
			rStats->rssie[2],MRSSI,
			rStats->evm[0],MEVM,
			rStats->evm[1],MEVM,
			rStats->evm[2],MEVM,
            rStats->byteCount,rStats->endTime-rStats->startTime);
#endif
	}
}


static void LinkReceiveDoIt()
{
	int timeout;
    //
	// make the timeout a little bit longer, just in case
	//
	timeout=_LinkDuration;
	if(timeout>0)
	{
		timeout+=100;
	}
    //
	// start the receiver
	//
	LinkRxStart(_LinkPromiscuous);
	//
	// tell the client the receiver is on
	//
    SendOn(_LinkClient);
	//
	// track the descriptors as they arrive
	//
	LinkRxComplete(timeout, _LinkNdump, _LinkPattern, 2, LinkStopIt);
    //
	// tell the client the receiver is off
	//
	SendOff(_LinkClient);
}


static void LinkRxIqCalibrationSave()
{
	ErrorPrint(NartDataHeader,"|rxiqcal|frequency|chain|ce|iss||rssi|nf|offset|");
	//
	// get data and return to client
	//
}


static void LinkRssiCalibrationSave()
{
    int j;
    struct rxStats *rStats;
	double rssic[MCHAIN],rssie[MCHAIN];
	int count;
	int countHt40;
	int it;
	char buffer[MBUFFER];
	int nfcal, nfpower;

	count=0;
	countHt40=0;
	for(it=0; it<MCHAIN; it++)
	{
		rssic[it]=0;
		rssie[it]=0;
	}

    for (j = 0; j < _LinkRateMany; j++) 
	{
        rStats=LinkRxStatFetch(_LinkRate[j]);
		for(it=0; it<MCHAIN; it++)
		{
			if(_LinkRxChain&(1<<it))
			{
				rssic[it] += HistogramMean(rStats->rssic[it],MRSSI);
				if(IS_HT40_RATE_INDEX(_LinkRate[j]))
				{
					rssie[it] += HistogramMean(rStats->rssie[it],MRSSI);
				}
			}
		}
		count++;
		if(IS_HT40_RATE_INDEX(_LinkRate[j]))
		{
			countHt40++;
		}
	}

	ErrorPrint(NartDataHeader,"|rssi|frequency|chain|ce|iss||rssi|nf|offset|");
	if(count>0)
	{
		for(it=0; it<MCHAIN; it++)
		{
			if(_LinkRxChain&(1<<it))
			{
				rssic[it] /= count;
				SformatOutput(buffer,MBUFFER-1,"|rssi|%d|%d|c|%d||%.1lf|%d|%.1lf|",
					_LinkFrequency,it,_LinkIss,rssic[it],_LinkNoiseFloorControl[it],_LinkIss-rssic[it]-_LinkNoiseFloorControl[it]);
				buffer[MBUFFER-1]=0;
				ErrorPrint(NartData,buffer);
				nfcal= (int)_LinkNoiseFloorControl[it];
				nfpower= (int)(_LinkIss-rssic[it]);

				if(countHt40>0)
				{
					rssie[it] /= countHt40;
					SformatOutput(buffer,MBUFFER-1,"|rssi|%d|%d|e|%d||%.1lf|%d|%.1lf|",
						_LinkFrequency,it,_LinkIss,rssie[it],_LinkNoiseFloorExtension[it],_LinkIss-rssie[it]-_LinkNoiseFloorExtension[it]);
					buffer[MBUFFER-1]=0;
					ErrorPrint(NartData,buffer);
					nfcal= (int)(0.5*(_LinkNoiseFloorControl[it]+_LinkNoiseFloorExtension[it]));
					nfpower= (int)(0.5*((_LinkIss-3-rssic[it])+(_LinkIss-3-rssie[it])));
				}

				Ar9300NoiseFloorSet(_LinkFrequency, it, nfcal);
				Ar9300NoiseFloorPowerSet(_LinkFrequency, it, nfpower);
			}
		}
	}
}


static void LinkReceiveNoiseFloor()
{
	int nf;
	//
	// install noise floor values
	//
	if(_LinkNoiseFloor<0)
	{
        NoiseFloorDo(_LinkFrequency, &_LinkNoiseFloor, 1, 0, 0, 0, 0, _LinkNoiseFloorControl, _LinkNoiseFloorExtension, MCHAIN);
	}
	//
	// or compute noise floor values
	//
	else if(_LinkNoiseFloor>0)
	{
		nf= -_LinkNoiseFloor;
        NoiseFloorDo(_LinkFrequency, &nf, 1, 20, 11, 100, 0, _LinkNoiseFloorControl, _LinkNoiseFloorExtension, MCHAIN);
	}
}


static int LinkReceiveFirstTime()
{
	int error;
	//
	// should we do a reset?
	// yes, if the user explicitly asked for it (_LinkReset>0) or if
	// the user asked for auto mode (_LinkReset<0) and any important parameters have changed
	//
    error=doReset();
	if(error!=0)
	{
		return error;
	}
	//
	// if we are doing rssi calibration, force noise floor
	//
	if(_LinkRssiCalibrate!=0)
	{
		if(_LinkNoiseFloor==0)
		{
			_LinkNoiseFloor= 110;
		}
	}
	//
	// if we are supposed to do noise floor calibration, do it here
	// after reset and before setting up descriptors
	//
	if(_LinkNoiseFloor!=0)
	{
		LinkReceiveNoiseFloor();
	}
    //
	// get all of the packets descriptors ready to run
	//
    LinkRxSetup(_LinkBssId,_LinkRxStation);

	return 0;
}


void LinkReceive(int client)
{
	//
	// parse the input and if it is good
	// get ready to do the receive
	//
	if(ParseInput(client)==0)
	{
		//
		// if there's no card loaded, return error
		//
		if(CardCheckAndLoad(client)!=0)
		{
			ErrorPrint(CardNoneLoaded);
		}
		else
		{
			_LinkClient=client;

			LinkMode = RX_MODE;
			//
			// Setup the descriptors for receive
			//
			if(LinkReceiveFirstTime()==0)
			{
				//
				// Acknowledge command from client
				//
				SendOk(client);
				//
				// wait for "START" message
				//
				if(MessageWait(60000)==0)
				{
					//
					// this is where we really do it.
					// turn on the receiver and get the packets
					//
					LinkReceiveDoIt();	
					//
					// if we are doing rssi calibration, save the data here
					//
					if(_LinkRssiCalibrate)
					{
						LinkRssiCalibrationSave();
					}
					//
					// if we are doing rx iq calibration, save the data here
					//
					if(_LinkRxIqCal)
					{
						LinkRxIqCalibrationSave();
					}
					//
					// send statistics back to client
					//
					LinkReceiveStatPrint();
				}
				else
				{
					ErrorPrint(ReceiveCancel);
				}
			} 
			else 
			{
			    ErrorPrint(ReceiveSetup);
			}
		}
	}
	SendDone(client);
}

//void LinkTransmitPAPD(int freq, int ht40, unsigned int TxChain, unsigned int RxChain, int chainNum )
void LinkTransmitPAPD(int chainNum )
{
	int rate=32;
	char junkAddr[6] = {0,0,0,0,0,0};

	//UserPrint("LinkTransmitPAPD:: freq=%d, ht40=%d, TxChain=%d, RxChain=%d, chainNum=%d\n", freq, ht40, TxChain, RxChain, chainNum);

	//
	// get all of the packets descriptors ready to run
	//
	LinkTxForPAPD(chainNum);
//    LinkTxSetup(_LinkRate, _LinkRateMcs20, _LinkRateMcs40, _LinkIr,
//		_LinkBssId, _LinkTxStation, _LinkRxStation, 
//		_LinkPacketMany, _LinkPacketLength, _LinkRetryMax, _LinkAntenna, _LinkBc, _LinkIfs,
//		_LinkShortGI, _LinkTxChain, _LinkAgg,
//		_LinkPattern, _LinkPatternLength);

	LinkTxSetup(&rate, 1, _LinkIr,
		_LinkBssId, _LinkTxStation, junkAddr, 
		 5, 2000, 1, 0, 0, -1,
		0, 1<<chainNum, 1,
		_LinkPattern, 0);

	LinkTxStart();
	LinkTxComplete(1000, NULL, NULL, _LinkChipTemperature);	
	
	// Clear PAPD ChainNum variable in LinkTx.c
	LinkTxForPAPD(-1);

	//LinkTransmitStatPrint();
}

void LinkTransmitPAPDWarmUp( int txchain)
{
	int rate=32;
	char junkAddr[6] = {0,0,0,0,0,0};

	//UserPrint("LinkTransmitPAPD:: freq=%d, ht40=%d, TxChain=%d, RxChain=%d, chainNum=%d\n", freq, ht40, TxChain, RxChain, chainNum);

	//
	// get all of the packets descriptors ready to run
	//
//    LinkTxSetup(_LinkRate, _LinkRateMcs20, _LinkRateMcs40, _LinkIr,
//		_LinkBssId, _LinkTxStation, _LinkRxStation, 
//		_LinkPacketMany, _LinkPacketLength, _LinkRetryMax, _LinkAntenna, _LinkBc, _LinkIfs,
//		_LinkShortGI, _LinkTxChain, _LinkAgg,
//		_LinkPattern, _LinkPatternLength);

	LinkTxSetup(&rate, 1, _LinkIr,
		_LinkBssId, _LinkTxStation, junkAddr, 
		 2, 2000, 1, 0, 1, -1,
		0, txchain, 1,
		_LinkPattern, 0);

	LinkTxStart();
	LinkTxComplete(1000, NULL, NULL, _LinkChipTemperature);	
	
	//LinkTransmitStatPrint();
}

void paramSetPAPD()
{
    _LinkCalibrateCount=0;
	_LinkChipTemperature=0;
	_LinkNoiseFloor=0;
	_LinkRssiCalibrate=0;
	_LinkPatternLength=0;
	_LinkReset= -1;
	_LinkDuration=0;
	_LinkPacketLength=1000;
	_LinkPacketMany=100;
	_LinkFrequency=2412;
	_LinkTxChain=0x7;
	_LinkRxChain=0x7;
	_LinkRate[0]=32;
	_LinkRateMany=1;
	_LinkTpcm=TpcmTargetPower;
	_LinkPcdac=30;
	_LinkPdgain=3;
	_LinkTransmitPower= -1;
	_LinkAtt= -1;
	_LinkIss= -1;
	_LinkBc=1;
	_LinkRetryMax=0;
	_LinkStat=3;	
	_LinkIr=0;
	_LinkBandwidth=BW_AUTOMATIC;
	_LinkAgg=1;
	_LinkIfs= -1;
	_LinkDeafMode=0;
	_LinkCWmin= -1;
	_LinkCWmax= -1;
	_LinkNdump=0;
	_LinkPromiscuous=0;
    _LinkShortGI=0;
	_LinkCalibrate=0;

}

void ForceLinkReset()
{
	_Papd_reset = 1;
}

