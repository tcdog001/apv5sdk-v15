
// 
// link parameters
//
enum 
{
	LinkParameterFrequency=0,
	LinkParameterTx,
	LinkParameterRx,
	LinkParameterRxVSG,
	LinkParameterRate,
	LinkParameterTemperature,
	LinkParameterBroadcast,
	LinkParameterRetry,
	LinkParameterAttenuation,
	LinkParameterInputSignalStrength,
	LinkParameterTransmitPower,
	LinkParameterPcdac,
	LinkParameterPdgain,
	LinkParameterPacketCount,
	LinkParameterPacketLength,
	LinkParameterInterleaveRates,
	LinkParameterChain,
	LinkParameterTxChain,
	LinkParameterRxChain,
	LinkParameterStatistic,
	LinkParameterLog,
	LinkParameterLogFile,
	LinkParameterDelay,
	LinkParameterReportType,
	LinkParameterReportFile,
	LinkParameterThreshold,
	LinkParameterPowerMeter,
	LinkParameterEVM,
	LinkParameterSpectralMask,
	LinkParameterHt40,
	LinkParameterTx99,
	LinkParameterTx100,
	LinkParameterInterframeSpacing,
	LinkParameterStop,
	LinkParameterDuration,
	LinkParameterDump,
	LinkParameterPromiscuous,
	LinkParameterBssId,
	LinkParameterMacTx,
	LinkParameterMacRx,
	LinkParameterContentionWindowMinimum,
	LinkParameterContentionWindowMaximum,
	LinkParameterDeafMode,
	LinkParameterReset,
	LinkParameterAggregate,
    LinkParameterGuardInterval,
	LinkParameterAverage,
	LinkParameterCalibrate,
	LinkParameterBlocker,
	LinkParameterBlockerDeltaFrequency,
	LinkParameterBlockerFrequency,
	LinkParameterBlockerInputSignalStrength,
	LinkParameterBlockerTransmitPower,
	LinkParameterRepeat,
	LinkParameterCurrentMeter,
	LinkParameterPattern,
	LinkParameterNoiseFloor,
	LinkParameterRssiCalibrate,
	LinkParameterChipTemperature,
	LinkParameterCalibrateGoal,
	LinkParameterCalibrateTxGainMinimum,
	LinkParameterCalibrateTxGainMaximum,
	LinkParameterRxIqCal,
	LinkParameterCarrier,
	LinkParameterFrequencyAccuracy,
	LinkParameterXtalCal,
	LinkParameterDatabaseClear,
	LinkParameterBandwidth,
};


static struct _ParameterList LinkLogicalParameter[2]=
{
	{0,{"no",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{1,{"yes",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkFrequencyMinimum=2400;
static int LinkFrequencyMaximum=6000;
static int LinkFrequencyDefault=2412;

static int LinkRetryMinimum=0;
static int LinkRetryMaximum=15;
static int LinkRetryDefault=0;

static int LinkPacketCountMinimum= -1;
static int LinkPacketCountMaximum=0x7fffffff;		
static int LinkPacketCountDefault=100;
static struct _ParameterList LinkPacketCountParameter[1]=
{
	{0,{"infinite",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkPacketLengthMinimum=30;
static int LinkPacketLengthMaximum=4092;
static int LinkPacketLengthDefault=1000;

static int LinkDurationMinimum= -1;
static int LinkDurationMaximum=0x7fffffff;			
static int LinkDurationDefault=60000;
static struct _ParameterList LinkDurationParameter[1]=
{
	{-1,{"forever",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkBroadcastDefault=1;

static int LinkDumpMinimum=0;
static int LinkDumpMaximum=4000;
static int LinkDumpDefault=0;

static int LinkPromiscuousDefault=0;

static int LinkInterleaveDefault=0;
static struct _ParameterList LinkInterleaveParameter[]=
{
	{0,{"no",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{1,{"yes",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{2,{"cart",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkRxDelayMinimum=0;
static int LinkRxDelayMaximum=10000;
static int LinkRxDelayDefault=0;

static int LinkTxGainMinimum=0;
static int LinkTxGainMaximum=100;
static int LinkTxGainDefault=30;

static int LinkAttenuationMinimum=0;
static int LinkAttenuationMaximum=110;
static int LinkAttenuationDefault=0;

static int LinkIssMinimum=-120;
static int LinkIssMaximum=0;
static int LinkIssDefault=0;

static int LinkAggregateMinimum=0;
static int LinkAggregateMaximum=32;
static int LinkAggregateDefault=1;

static int LinkChainMinimum=0x1;
static int LinkChainMaximum=0x7;
static int LinkChainDefault=0x7;

#define LinkUseTargetPower (-100)

static double LinkTransmitPowerMinimum=-100.0;
static double LinkTransmitPowerMaximum=31.5;
static double LinkTransmitPowerDefault= LinkUseTargetPower;
static struct _ParameterList LinkTransmitPowerParameter[1]=
{
	{LinkUseTargetPower,{"target",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static double LinkBlockerTransmitPowerMinimum=0.0;
static double LinkBlockerTransmitPowerMaximum=31.5;
static double LinkBlockerTransmitPowerDefault= -1.0;

static int LinkBlockerDeltaMinimum=-1000;
static int LinkBlockerDeltaMaximum=1000;
static int LinkBlockerDeltaDefault= 25;

static int LinkBlockerFreqMinimum=2000;
static int LinkBlockerFreqMaximum=8000;
static int LinkBlockerFreqDefault= 2437;

static int LinkBlockerIssMinimum=-120;
static int LinkBlockerIssMaximum=0;
static int LinkBlockerIssDefault=0;

#ifdef UNUSED
static int LinkNartNone= -1;
static int LinkRxDefault=ArtDut;
static int LinkTxDefault=ArtGolden;
static int LinkBlockerDefault= -1;
static struct _ParameterList LinkNartParameter[4]=
{
	{-1,{"none",0,0},0,0,0,0,0,0,0,0,0},
	{ArtDut,{"dut",0,0},0,0,0,0,0,0,0,0,0},
	{ArtGolden,{"golden",0,0},0,0,0,0,0,0,0,0,0},
	{ArtBlocker,{"blocker",0,0},0,0,0,0,0,0,0,0,0},
};
#endif

static unsigned char LinkBssidDefault[6]={0x50,0x55,0x55,0x55,0x55,0x05};
static unsigned char LinkMacTxDefault[6]={0x20,0x22,0x22,0x22,0x22,0x02};
static unsigned char LinkMacRxDefault[6]={0x10,0x11,0x11,0x11,0x11,0x01};

//
// things we can do with the power meter
//
enum PowerMeterAction
{
    PowerMeterNone=0,
    PowerMeterCombined=1,
    PowerMeterIsolated=2,
	PowerMeterCombinedIterate=3,
	PowerMeterIsolatedIterate=4,
};


static int LinkPmDefault=PowerMeterNone;

static int LinkSmDefault=PowerMeterNone;

static int LinkFaDefault=PowerMeterNone;

static int LinkCmDefault=PowerMeterNone;

static int LinkEvmDefault=PowerMeterNone;

static int LinkCalibrateDefault=PowerMeterNone;

static int LinkRxvsgDefault=PowerMeterNone;

static struct _ParameterList LinkPowerMeterParameter[]=
{
	{PowerMeterNone,{"none",0,0},"nothing is measured",0,0,0,0,0,0,0,0,0,0},
	{PowerMeterCombined,{"combined",0,0},"the combined output signal is measured",0,0,0,0,0,0,0,0},
	{PowerMeterIsolated,{"isolated",0,0},"attenuators are used to isolate and measure each chain separately",0,0,0,0,0,0,0,0},
};

static struct _ParameterList LinkCalibrateParameter[]=
{
	{PowerMeterNone,{"none",0,0},"nothing is measured",0,0,0,0,0,0,0,0,0,0},
	{PowerMeterCombined,{"combined",0,0},"the combined output signal is measured",0,0,0,0,0,0,0,0,0,0},
	{PowerMeterIsolated,{"isolated",0,0},"attenuators are used to isolate and measure each chain separately",0,0,0,0,0,0,0,0,0,0},
	{PowerMeterCombinedIterate,{"iterate-combined","ic",0},"the combined output signal is measured with iteration to reach the power goal",0,0,0,0,0,0,0,0,0,0},
	{PowerMeterIsolatedIterate,{"iterate-isolated","ii",0},"attenuators are used to isolate and measure each chain separately with iteration to reach the power goal",0,0,0,0,0,0,0,0,0,0},
};

static int LinkNfMinimum=-200;
static int LinkNfMaximum=200;
static int LinkNfDefault=0;
static struct _ParameterList LinkNfParameter[2]=
{
	{0,{"current",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{1,{"calculate",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkRssiDefault=0;

static int LinkAverageMinimum= -1;
static int LinkAverageMaximum=1000;
static int LinkAverageDefault= -1;
static struct _ParameterList LinkAverageParameter[1]=
{
	{-1,{"automatic",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkTemperatureMinimum= -1;
static int LinkTemperatureMaximum=100;
static int LinkTemperatureDefault= -1;
static struct _ParameterList LinkTemperatureParameter[1]=
{
	{-1,{"none",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkResetDefault= -1;
static struct _ParameterList LinkResetParameter[3]=
{
	{0,{"no",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{1,{"yes",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{-1,{"automatic",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkPdgainMinimum=0;
static int LinkPdgainMaximum=3;
static int LinkPdgainDefault=0;

static int LinkStatMinimum=0;
static int LinkStatMaximum=3;
static int LinkStatDefault=3;

static int LinkLogDefault=0;

static int LinkHt40Default=2;
static struct _ParameterList LinkHt40Parameter[4]=
{
	{0,{"none",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{-1,{"low",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{1,{"high",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{2,{"automatic","dynamic",0},0,0,0,0,0,0,0,0,0,0,0},
};

//OLD DEFINITIONS
//#define BW_QUARTER   5
//#define BW_HALF      10
//#define BW_STATIC_20 20
//#define BW_AUTOMATIC 40
//#define BW_40_EX_LOW -1
//#define BW_40_EX_HI   1

//being replaced by below:
#define BW_AUTOMATIC (0)
#define BW_HT40_PLUS (40)
#define BW_HT40_MINUS (-40)
#define BW_HT20 (20)
#define BW_OFDM (19)
#define BW_HALF (10)
#define BW_QUARTER (5)


static int LinkBandwidthDefault=BW_AUTOMATIC;
static struct _ParameterList LinkBandwidthParameter[]=
{
	{BW_QUARTER,{"quarter",0,0},"5MHz bandwidth, quarter rate speed",0,0,0,0,0,0,0,0,0,0},
	{BW_HALF,{"half",0,0},"10MHz bandwidth,half rate speed",0,0,0,0,0,0,0,0,0,0},
	{BW_HT20,{"ht20",0,0},"20MHz bandwidth, regular legacy or HT20 rates (ie HT40=0)",0,0,0,0,0,0,0,0,0,0},
	{BW_OFDM,{"ofdm",0,0},"Legacy OFDM rates only (6-54)",0,0,0,0,0,0,0,0,0,0},
	{BW_AUTOMATIC,{"automatic","dynamic",0},"Use 20MHz or 40 MHz (extension low or high) as appropriate (ie HT40=2)",0,0,0,0,0,0,0,0,0,0},
	{BW_HT40_MINUS,{"ht40minus",0,0},"40MHz bandwidth, extension channel low (ie HT40=-1)",0,0,0,0,0,0,0,0,0,0},
	{BW_HT40_PLUS,{"ht40plus",0,0},"40MHz bandwidth, extension channel high (ie HT40=1)",0,0,0,0,0,0,0,0,0,0},
};

static int LinkSgiDefault=0;

static int LinkTx99Default=0;

static int LinkTx100Default=0;

static int LinkIfsMinimum= -1;
static int LinkIfsMaximum=1;
static int LinkIfsDefault= -1;
static struct _ParameterList LinkIfsParameter[3]=
{
	{-1,{"regular",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{0,{"tx100",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{1,{"tx99",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkDeafDefault=0;

static int LinkRepeatMinimum=1;
static int LinkRepeatMaximum=1000000;
static int LinkRepeatDefault=1;

static unsigned int LinkPatternMinimum=0;
static unsigned int LinkPatternMaximum=255;
static unsigned int LinkPatternDefault=0;

static unsigned int LinkChipTemperatureMinimum=0;
static unsigned int LinkChipTemperatureMaximum=255;
static unsigned int LinkChipTemperatureDefault=0;


static int LinkRateDefault=32;
static struct _ParameterList LinkRateParameter[]=
{
	{RATE_INDEX_6, {"6",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_9, {"9",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_12, {"12",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_18, {"18",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_24, {"24",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_36, {"36",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_48, {"48",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_54, {"54",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_1L, {"1l",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_2L, {"2l",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_2S, {"2s",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_5L, {"5l",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_5S, {"5s",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_11L, {"11l",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_11S, {"11s",0,0},0,0,0,0,0,0,0,0,0,0,0},

	{RATE_INDEX_HT20_MCS0, {"t0", "mcs0", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS1, {"t1", "mcs1", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS2, {"t2", "mcs2", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS3, {"t3", "mcs3", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS4, {"t4", "mcs4", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS5, {"t5", "mcs5", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS6, {"t6", "mcs6", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS7, {"t7", "mcs7", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS8, {"t8", "mcs8", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS9, {"t9", "mcs9", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS10, {"t10", "mcs10", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS11, {"t11", "mcs11", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS12, {"t12", "mcs12", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS13, {"t13", "mcs13", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS14, {"t14", "mcs14", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS15, {"t15", "mcs15", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS16, {"t16", "mcs16", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS17, {"t17", "mcs17", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS18, {"t18", "mcs18", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS19, {"t19", "mcs19", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS20, {"t20", "mcs20", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS21, {"t21", "mcs21", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS22, {"t22", "mcs22", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT20_MCS23, {"t23", "mcs23", 0},0,0,0,0,0,0,0,0,0,0,0},

	{RATE_INDEX_HT40_MCS0, {"f0", "mcs0/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS1, {"f1", "mcs1/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS2, {"f2", "mcs2/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS3, {"f3", "mcs3/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS4, {"f4", "mcs4/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS5, {"f5", "mcs5/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS6, {"f6", "mcs6/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS7, {"f7", "mcs7/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS8, {"f8", "mcs8/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS9, {"f9", "mcs9/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS10, {"f10", "mcs10/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS11, {"f11", "mcs11/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS12, {"f12", "mcs12/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS13, {"f13", "mcs13/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS14, {"f14", "mcs14/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS15, {"f15", "mcs15/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS16, {"f16", "mcs16/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS17, {"f17", "mcs17/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS18, {"f18", "mcs18/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS19, {"f19", "mcs19/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS20, {"f20", "mcs20/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS21, {"f21", "mcs21/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS22, {"f22", "mcs22/40", 0},0,0,0,0,0,0,0,0,0,0,0},
	{RATE_INDEX_HT40_MCS23, {"f23", "mcs23/40", 0},0,0,0,0,0,0,0,0,0,0,0},

	{RateAll, {"all",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RateLegacy, {"legacy",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RateHt20, {"ht20",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{RateHt40, {"ht40",0,0},0,0,0,0,0,0,0,0,0,0,0},
//	{RateDvt, {"dvt",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkCalibrateGoalMinimum= -100;
static int LinkCalibrateGoalMaximum=35;
static int LinkCalibrateGoalDefault= -100;
static struct _ParameterList LinkCalibrateGoalParameter[3]=
{
	{-100,{"mean",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int LinkCalibrateTxGainMinimum=0;
static int LinkCalibrateTxGainMaximum=100;
static int LinkCalibrateTxGainMinimumDefault= 0;
static int LinkCalibrateTxGainMaximumDefault= 100;

static int LinkRxIqCalDefault=0;

static int LinkCarrierDefault=0;
static int LinkXtalCalDefault=0;

static int LinkDatabaseClearDefault=1;

//
// the following definition are shared by nart and cart.
//
#define LINK_FREQUENCY(MLOOP) {LinkParameterFrequency,{"frequency","f",0},"the channel carrier frequency",'u',"MHz",MLOOP,1,1,\
	&LinkFrequencyMinimum,&LinkFrequencyMaximum,&LinkFrequencyDefault,0,0}
#define LINK_RETRY {LinkParameterRetry,{"retry",0,0},"the number of times a packet is retransmitted",'u',0,1,1,1,\
	&LinkRetryMinimum,&LinkRetryMaximum,&LinkRetryDefault,0,0}
#ifdef UNUSED
#define LINK_RX	{LinkParameterRx,{"rx",0,0},"which device is the receiver",'z',0,1,\
	0,0,&LinkRxDefault,sizeof(LinkNartParameter)/sizeof(LinkNartParameter[0]),LinkNartParameter}
#define LINK_TX	{LinkParameterTx,{"tx",0,0},"which device is the transmitter",'z',0,1,
	0,0,&LinkTxDefault,sizeof(LinkNartParameter)/sizeof(LinkNartParameter[0]),LinkNartParameter}
#define LINK_BLOCKER {LinkParameterBlocker,{"blocker",0,0},"which device is the blocker",'z',0,1,\
	0,0,&LinkBlockerDefault,sizeof(LinkNartParameter)/sizeof(LinkNartParameter[0]),LinkNartParameter}
#endif
#define LINK_RATE(MLOOP) {LinkParameterRate,{"rate","r",0},"the data rates used",'z',0,MLOOP,1,1,\
	0,0,&LinkRateDefault,sizeof(LinkRateParameter)/sizeof(LinkRateParameter[0]),LinkRateParameter}
#define LINK_BROADCAST {LinkParameterBroadcast,{"broadcast","bc",0},"if set to 1 the packets are broadcast, if set to 0 the packets are unicast",'z',0,1,1,1,\
	0,0,&LinkBroadcastDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_PACKET_COUNT(MLOOP) {LinkParameterPacketCount,{"packetCount","pc","np"},"the number of packets sent",'d',0,MLOOP,1,1,\
	&LinkPacketCountMinimum,&LinkPacketCountMaximum,&LinkPacketCountDefault,sizeof(LinkPacketCountParameter)/sizeof(LinkPacketCountParameter[0]),LinkPacketCountParameter}
#define LINK_PACKET_LENGTH(MLOOP) {LinkParameterPacketLength,{"packetLength","pl",0},"the length of the packets",'u',"Byte",MLOOP,1,1,\
	&LinkPacketLengthMinimum,&LinkPacketLengthMaximum,&LinkPacketLengthDefault,0,0}
#define LINK_CHAIN(MLOOP) {LinkParameterChain,{"chain","ch",0},"the chain mask used for both transmit and receive",'x',0,MLOOP,1,1,\
	&LinkChainMinimum,&LinkChainMaximum,&LinkChainDefault,0,0}
#define LINK_TX_CHAIN(MLOOP) {LinkParameterTxChain,{"txChain",0,0},"the chain mask used for transmit",'x',0,MLOOP,1,1,\
	&LinkChainMinimum,&LinkChainMaximum,&LinkChainDefault,0,0}
#define LINK_RX_CHAIN {LinkParameterRxChain,{"rxChain",0,0},"the chain mask used for receive",'x',0,1,1,1,\
	&LinkChainMinimum,&LinkChainMaximum,&LinkChainDefault,0,0}
#define LINK_TRANSMIT_POWER(MLOOP) {LinkParameterTransmitPower,{"transmitPower","tp","txp"},"the transmit power used",'f',"dBm",MLOOP,1,1,\
	&LinkTransmitPowerMinimum,&LinkTransmitPowerMaximum,&LinkTransmitPowerDefault,sizeof(LinkTransmitPowerParameter)/sizeof(LinkTransmitPowerParameter[0]),LinkTransmitPowerParameter}
#define LINK_DURATION {LinkParameterDuration,{"duration",0,0},"the maximum duration of the operation",'d',"ms",1,1,1,\
	&LinkDurationMinimum,&LinkDurationMaximum,&LinkDurationDefault,sizeof(LinkDurationParameter)/sizeof(LinkDurationParameter[0]),LinkDurationParameter}
#define LINK_DELAY {LinkParameterDelay,{"delay",0,0},"delay between receiver ready and transmitter start",'d',"ms",1,1,1,\
	&LinkRxDelayMinimum,&LinkRxDelayMaximum,&LinkRxDelayDefault,0,0}
#define LINK_DUMP {LinkParameterDump,{"dump",0,0},"the number of bytes of each packet displayed in the nart log",'u',0,1,1,1,\
	&LinkDumpMinimum,&LinkDumpMaximum,&LinkDumpDefault,0,0}
#define LINK_PROMISCUOUS {LinkParameterPromiscuous,{"promiscuous",0,0},"if set to 1, all packet types are received",'z',0,1,1,1,\
	0,0,&LinkPromiscuousDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_BSSID {LinkParameterBssId,{"bssid",0,0},"the bssid used by the transmitter and receiver",'a',0,1,1,1,\
	0,0,&LinkBssidDefault,0,0}
#define LINK_MAC_TX {LinkParameterMacTx,{"mactx",0,0},"the mac address used by the transmitter",'a',0,1,1,1,\
	0,0,&LinkMacTxDefault,0,0}
#define LINK_MAC_RX {LinkParameterMacRx,{"macrx",0,0},"the mac address used by the receiver",'a',0,1,1,1,\
	0,0,&LinkMacRxDefault,0,0}
#define LINK_TEMPERATURE(MLOOP) {LinkParameterTemperature,{"temperature",0,0},"the temperature at which the test is run",'d',"deg C",MLOOP,1,1,\
	&LinkTemperatureMinimum,&LinkTemperatureMaximum,&LinkTemperatureDefault,sizeof(LinkTemperatureParameter)/sizeof(LinkTemperatureParameter[0]),LinkTemperatureParameter}
#define LINK_ATTENUATION(MLOOP) {LinkParameterAttenuation,{"attenuation",0,0},"the attenuation between the golden unit and the dut",'d',"dB",MLOOP,1,1,\
	&LinkAttenuationMinimum,&LinkAttenuationMaximum,&LinkAttenuationDefault,0,0}
#define LINK_ISS(MLOOP) {LinkParameterInputSignalStrength,{"inputSignalStrength","iss",0},"the expected input signal strength at the dut",'d',"dB",MLOOP,1,1,\
	&LinkIssMinimum,&LinkIssMaximum,&LinkIssDefault,0,0}
#define LINK_TXGAIN(MLOOP) {LinkParameterPcdac,{"pcdac","txgain","txg"},"the tx gain used by the transmitter",'d',0,MLOOP,1,1,\
	&LinkTxGainMinimum,&LinkTxGainMaximum,&LinkTxGainDefault,0,0}
#define LINK_INTERLEAVE_CART {LinkParameterInterleaveRates,{"interleaveRates","ir",0},"interleave packets from different rates?",'z',0,1,1,1,\
	0,0,&LinkInterleaveDefault,sizeof(LinkInterleaveParameter)/sizeof(LinkInterleaveParameter[0]),LinkInterleaveParameter}
#define LINK_INTERLEAVE {LinkParameterInterleaveRates,{"interleaveRates","ir",0},"interleave packets from different rates?",'z',0,1,1,1,\
	0,0,&LinkInterleaveDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_AGGREGATE(MLOOP) {LinkParameterAggregate,{"aggregate",0,0},"the number of aggregated packets",'d',0,MLOOP,1,1,\
	&LinkAggregateMinimum,&LinkAggregateMaximum,&LinkAggregateDefault,0,0}
#define LINK_POWER_METER {LinkParameterPowerMeter,{"powerMeter","pm",0},"measure power",'z',0,1,1,1,\
	0,0,&LinkPmDefault,sizeof(LinkPowerMeterParameter)/sizeof(LinkPowerMeterParameter[0]),LinkPowerMeterParameter}
#define LINK_EVM {LinkParameterEVM,{"evm",0,0},"measure evm",'z',0,1,1,1,\
	0,0,&LinkEvmDefault,sizeof(LinkPowerMeterParameter)/sizeof(LinkPowerMeterParameter[0]),LinkPowerMeterParameter}
#define LINK_MASK {LinkParameterSpectralMask,{"spectralMask","mask",0},"measure spectral mask",'z',0,1,1,1,\
	0,0,&LinkSmDefault,sizeof(LinkPowerMeterParameter)/sizeof(LinkPowerMeterParameter[0]),LinkPowerMeterParameter}
#define LINK_FREQUENCY_ACCURACY {LinkParameterFrequencyAccuracy,{"fa","ppm",0},"measure frequency accuracy",'z',0,1,1,1,\
	0,0,&LinkFaDefault,sizeof(LinkPowerMeterParameter)/sizeof(LinkPowerMeterParameter[0]),LinkPowerMeterParameter}
#define LINK_CALIBRATE {LinkParameterCalibrate,{"calibrate",0,0},"calibrate transmit power",'z',0,1,1,1,\
	0,0,&LinkCalibrateDefault,sizeof(LinkCalibrateParameter)/sizeof(LinkCalibrateParameter[0]),LinkCalibrateParameter}
#define LINK_CURRENT {LinkParameterCurrentMeter,{"current","cm",0},"measure current consumption",'z',0,1,1,1,\
	0,0,&LinkCmDefault,sizeof(LinkPowerMeterParameter)/sizeof(LinkPowerMeterParameter[0]),LinkPowerMeterParameter}
#define LINK_RXVSG {LinkParameterRxVSG,{"rxvsg",0,0},"measure rxvsg",'z',0,1,1,1,\
	0,0,&LinkRxvsgDefault,sizeof(LinkPowerMeterParameter)/sizeof(LinkPowerMeterParameter[0]),LinkPowerMeterParameter}
#define LINK_NOISE_FLOOR {LinkParameterNoiseFloor,{"nf",0,0},"noise floor value",'d',0,1,1,1,\
	&LinkNfMinimum,&LinkNfMaximum,&LinkNfDefault,sizeof(LinkNfParameter)/sizeof(LinkNfParameter[0]),LinkNfParameter}
#define LINK_RSSI_CALIBRATE {LinkParameterRssiCalibrate,{"rssical",0,0},"measure and calibrate rssi",'z',0,1,1,1,\
	0,0,&LinkRssiDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_AVERAGE {LinkParameterAverage,{"average","avg",0},"number of measurements taken and averaged",'d',0,1,1,1,\
	&LinkAverageMinimum,&LinkAverageMaximum,&LinkAverageDefault,sizeof(LinkAverageParameter)/sizeof(LinkAverageParameter[0]),LinkAverageParameter}
#define LINK_RESET {LinkParameterReset,{"reset",0,0},"reset device before operation",'z',0,1,1,1,\
	0,0,&LinkResetDefault,sizeof(LinkResetParameter)/sizeof(LinkResetParameter[0]),LinkResetParameter}
#define LINK_PDGAIN {LinkParameterPdgain,{"pdgain",0,0},"pdgain",'d',0,1,1,1,\
	&LinkPdgainMinimum,&LinkPdgainMaximum,&LinkPdgainDefault,0,0}
#define LINK_STATISTIC {LinkParameterStatistic,{"statistic",0,0},"statistic",'d',0,1,1,1,\
	&LinkStatMinimum,&LinkStatMaximum,&LinkStatDefault,0,0}
#define LINK_LOG {LinkParameterLog,{"log",0,0},"log data",'z',0,1,1,1,\
	0,0,&LinkLogDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_LOG_FILE {LinkParameterLogFile,{"logFile","lf",0},"log file name",'t',0,1,1,1,\
	0,0,"$LogFileName",0,0}
#define LINK_HT40 {LinkParameterHt40,{"ht40",0},"use 40MHz channel",'z',0,1,1,1,\
	0,0,&LinkHt40Default,sizeof(LinkHt40Parameter)/sizeof(LinkHt40Parameter[0]),LinkHt40Parameter}
#define LINK_GUARD_INTERVAL	{LinkParameterGuardInterval,{"gi","sgi",0},"use short guard interval",'z',0,1,1,1,\
	0,0,&LinkSgiDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_TX99 {LinkParameterTx99,{"tx99",0,0},"use tx99 mode, small, constant interframe spacing",'z',0,1,1,1,\
	0,0,&LinkTx99Default,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_TX100 {LinkParameterTx100,{"tx100",0,0},"use tx100 mode, continuous data transmission",'z',0,1,1,1,\
	0,0,&LinkTx100Default,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_INTERFRAME_SPACING {LinkParameterInterframeSpacing,{"interFrameSpacing","ifs","fs"},"spacing between frames",'d',0,1,1,1,\
	&LinkIfsMinimum,&LinkIfsMaximum,&LinkIfsDefault,sizeof(LinkIfsParameter)/sizeof(LinkIfsParameter[0]),LinkIfsParameter}
#define LINK_DEAF_MODE {LinkParameterDeafMode,{"deafMode",0,0},"disable receiver during transmission",'z',0,1,1,1,\
	0,0,&LinkDeafDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_BLOCKER_DELTA(MLOOP) {LinkParameterBlockerDeltaFrequency,{"delta",0,0},"frequency delta for blocker unit",'d',"MHz",MLOOP,1,1,\
	&LinkBlockerDeltaMinimum,&LinkBlockerDeltaMaximum,&LinkBlockerDeltaDefault,0,0}
#define LINK_BLOCKER_FREQUENCY(MLOOP) {LinkParameterBlockerFrequency,{"bfrequency","bfreq","bf"},"frequency for blocker unit",'d',"MHz",MLOOP,1,1,\
	&LinkBlockerFreqMinimum,&LinkBlockerFreqMaximum,&LinkBlockerFreqDefault,0,0}
#define LINK_BLOCKER_ISS(MLOOP) {LinkParameterBlockerInputSignalStrength,{"biss",0,0},"input signal strength from blocker unit",'d',"dB",MLOOP,1,1,\
	&LinkBlockerIssMinimum,&LinkBlockerIssMaximum,&LinkBlockerIssDefault,0,0}
#define LINK_BLOCKER_TRANSMIT_POWER(MLOOP) {LinkParameterBlockerTransmitPower,{"btp",0,0},"transmit power for blocker unit",'f',"dBm",MLOOP,1,1,\
	&LinkBlockerTransmitPowerMinimum,&LinkBlockerTransmitPowerMaximum,&LinkBlockerTransmitPowerDefault,sizeof(LinkTransmitPowerParameter)/sizeof(LinkTransmitPowerParameter[0]),LinkTransmitPowerParameter}
#define LINK_REPEAT {LinkParameterRepeat,{"repeat",0,0},"number of times the operation is repeated",'d',0,1,1,1,\
	&LinkRepeatMinimum,&LinkRepeatMaximum,&LinkRepeatDefault,0,0}
#define LINK_PATTERN(MPATTERN) {LinkParameterPattern,{"pattern",0,0},"data pattern",'x',0,MPATTERN,1,1,\
	&LinkPatternMinimum,&LinkPatternMaximum,&LinkPatternDefault,0,0}
#define LINK_CHIP_TEMPERATURE {LinkParameterChipTemperature,{"chipTemperature",0,0},"wait for chip temperature to exceed this value",'u',0,1,1,1,\
	&LinkChipTemperatureMinimum,&LinkChipTemperatureMaximum,&LinkChipTemperatureDefault,0,0}
#define LINK_CALIBRATE_GOAL {LinkParameterCalibrateGoal,{"goal",0,0},"target output power for calibration",'d',0,1,1,1,\
	&LinkCalibrateGoalMinimum,&LinkCalibrateGoalMaximum,&LinkCalibrateGoalDefault,sizeof(LinkCalibrateGoalParameter)/sizeof(LinkCalibrateGoalParameter[0]),LinkCalibrateGoalParameter}
#define LINK_CALIBRATE_TX_GAIN_MINIMUM {LinkParameterCalibrateTxGainMinimum,{"txgminimum",0,0},"minimum txgain for calibration search",'d',0,1,1,1,\
	&LinkCalibrateTxGainMinimum,&LinkCalibrateTxGainMaximum,&LinkCalibrateTxGainMinimumDefault,0,0}
#define LINK_CALIBRATE_TX_GAIN_MAXIMUM {LinkParameterCalibrateTxGainMaximum,{"txgmaximum",0,0},"maximum txgain for calibration search",'d',0,1,1,1,\
	&LinkCalibrateTxGainMinimum,&LinkCalibrateTxGainMaximum,&LinkCalibrateTxGainMaximumDefault,0,0}
#define LINK_RX_IQ_CAL {LinkParameterRxIqCal,{"rxiqcal","iqcal",0},"perform rx iq calibration",'z',0,1,1,1,\
	0,0,&LinkRxIqCalDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_CARRIER {LinkParameterCarrier,{"carrier",0,0},"transmit carrier only",'z',0,1,1,1,\
	0,0,&LinkCarrierDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_XTAL_CAL {LinkParameterXtalCal,{"xtalcal","xtal",0},"turning caps calibration",'z',0,1,1,1,\
	0,0,&LinkXtalCalDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_STOP(MLOOP) {LinkParameterStop,{"stop",0,0},"early termination condition",'t',0,MLOOP,1,1,\
	0,0,0,0,0}
#define LINK_DATABASE_CLEAR {LinkParameterDatabaseClear,{"clear","dbclear",0},"clear the internal database before performing the command",'z',0,1,1,1,\
	0,0,&LinkDatabaseClearDefault,sizeof(LinkLogicalParameter)/sizeof(LinkLogicalParameter[0]),LinkLogicalParameter}
#define LINK_BANDWIDTH {LinkParameterBandwidth,{"bandwidth","bw",0},"select bandwidth",'z',0,1,1,1,\
	0,0,&LinkBandwidthDefault,sizeof(LinkBandwidthParameter)/sizeof(LinkBandwidthParameter[0]),LinkBandwidthParameter}


#define LINK_REPORT {LinkParameterReportType,{"report",0,0},"does anyone use this?",'t',0,1,1,1,0,0,0,0,0}
#define LINK_THRESHOLD {LinkParameterThreshold,{"threshold","th",0},"does anyone use this?",'d',0,1,0,0,0,0,0}
#define LINK_CONTENTION_WINDOW_MINIMUM {LinkParameterContentionWindowMinimum,{"cwMinimum",0,0},"does anyone use this?",'d',0,1,1,1,0,0,0,0,0}
#define LINK_CONTENTION_WINDOW_MAXIMUM {LinkParameterContentionWindowMaximum,{"cwMaximum",0,0},"does anyone use this?",'d',0,1,1,1,0,0,0,0,0}


