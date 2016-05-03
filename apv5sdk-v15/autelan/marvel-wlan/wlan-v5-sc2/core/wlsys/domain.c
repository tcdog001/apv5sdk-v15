/*
*                Copyright 2002-2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

#include "domain.h"
#include "wltypes.h"
#include "IEEE_types.h"
#include "hostcmd.h"

#ifdef IEEE80211H
extern UINT8 bcn_reg_domain;
#endif /* IEEE80211H */

#define MaxMultiDomainCapabilityEntryA 31//20

#define MaxMultiDomainCapabilityEntryG 1

typedef struct _DOMAIN
{
	UINT8 domainCode;
	UINT8 ChannelList[IEEE_80211_MAX_NUMBER_OF_CHANNELS];
}
DOMAIN;
typedef  struct _MultiDomainCapabilityEntry
{
	UINT8 FirstChannelNo;
	UINT8 NoofChannel;
	UINT8 MaxTransmitPw;   
}
PACK_END MultiDomainCapabilityEntry;


typedef  struct _DOMAIN_TXPOWER
{
	UINT8 domainCode;
	UINT8 CountryString[3];
	UINT8 GChannelLen;
	MultiDomainCapabilityEntry DomainEntryG[ MaxMultiDomainCapabilityEntryG];
	UINT8 AChannelLen;
	MultiDomainCapabilityEntry DomainEntryA[MaxMultiDomainCapabilityEntryA];
}
PACK_END DOMAIN_TXPOWER;

#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT
extern UINT8 mib_channelspacing;
#endif

/*Begin:added by pengdecai for country code*/
typedef struct _COUNTRY_DOMAIN
{
	UINT16         countryID;
	UINT8          domainCode;
}
PACK_END COUNTRY_DOMAIN;
/*Begin:added by pengdecai for country code*/
static DOMAIN_TXPOWER IEEERegionPower[]=
{
#ifdef DFS_CHNL_SUPPORT
	{DOMAIN_CODE_FCC,{'U','S',' '},3,{{1,11,30}},
    72,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {52,1,24},{56,1,24},{60,1,24},{64,1,24},
    {100,1,30},{104,1,30},{108,1,30},{112,1,30},{116,1,30},{120,1,30},{124,1,30},{128,1,30},{132,1,30},{136,1,30},{140,1,30},
    {149,1,23},{153,1,23},{157,1,23},{161,1,23},{165,1,23},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}},  /** USA **/
#else
	{DOMAIN_CODE_FCC,{'U','S',' '},3,{{1,11,30}},
    27,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {149,1,23},{153,1,23},{157,1,23},{161,1,23},{165,1,23},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}},  /** USA **/
#endif

#ifdef DFS_CHNL_SUPPORT
	{DOMAIN_CODE_IC,{'C','A',' '},3,{{1,11,30}},
    72,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {52,1,24},{56,1,24},{60,1,24},{64,1,24},
    {100,1,30},{104,1,30},{108,1,30},{112,1,30},{116,1,30},{120,1,30},{124,1,30},{128,1,30},{132,1,30},{136,1,30},{140,1,30},
    {149,1,23},{153,1,23},{157,1,23},{161,1,23},{165,1,23},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}},  /** CANADA **/
#else /*!DFS_CHNL_SUPPORT*/
	{DOMAIN_CODE_IC,{'C','A',' '},3,{{1,11,30}},
    27,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {149,1,23},{153,1,23},{157,1,23},{161,1,23},{165,1,23},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}},  /** CANADA **/
#endif

#ifdef DFS_CHNL_SUPPORT
	{DOMAIN_CODE_ETSI,{'E','U',' '},3,{{1,13,20}},
    57 ,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {52,1,24},{56,1,24},{60,1,24},{64,1,24},
    {100,1,30},{104,1,30},{108,1,30},{112,1,30},{116,1,30},{120,1,30},{124,1,30},{128,1,30},{132,1,30},{136,1,30},{140,1,30},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}	
	}},  /** EUROPE **/
#else /*!DFS_CHNL_SUPPORT*/
	{DOMAIN_CODE_ETSI,{'E','U',' '},3,{{1,13,20}},
    12 ,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}	
	}},  /** EUROPE **/
#endif

	{DOMAIN_CODE_SPAIN,{'E','S',' '},3,{{1,13,20}},
    57 ,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {52,1,24},{56,1,24},{60,1,24},{64,1,24},
    {100,1,30},{104,1,30},{108,1,30},{112,1,30},{116,1,30},{120,1,30},{124,1,30},{128,1,30},{132,1,30},{136,1,30},{140,1,30},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}	
	}},  /** SPAIN **/

	{DOMAIN_CODE_FRANCE,{'F','R',' '},3,{{10,4,20}},
    57 ,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {52,1,24},{56,1,24},{60,1,24},{64,1,24},
    {100,1,30},{104,1,30},{108,1,30},{112,1,30},{116,1,30},{120,1,30},{124,1,30},{128,1,30},{132,1,30},{136,1,30},{140,1,30},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}},  /** FRANCE **/
	
	{DOMAIN_CODE_MKK,{'J','P',' '},3,{{1,14,23}},
    39,{{36,1,23},{40,1,23},{44,1,23},{48,1,23},
    {52,1,23},{56,1,23},{60,1,23},{64,1,23},
	{100,1,30},{104,1,30},{108,1,30},{112,1,30},{116,1,30},{120,1,30},{124,1,30},{128,1,30},{132,1,30},{136,1,30},{140,1,30},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}	
	}},  /** JAPAN W53 & W56**/
#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT
	{DOMAIN_CODE_MKK2,{'J','P',' '},3,{{1,14,23}},
    33 ,{{7,1,23},{8,1,23},{11,1,23},{183,1,23},{184,1,23},{185,1,23},
	{187,1,23},{188,1,23},{189,1,23},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}},  /** JAPAN with channel spacing 10**/
#endif

	{DOMAIN_CODE_DGT,{'T','W',' '},3,{{1,11,30}},
    57 ,{{52,1,23},{56,1,23},{60,1,23},{64,1,23},
    {100,1,23},{104,1,23},{108,1,23},{112,1,23},{116,1,23},{120,1,23},{124,1,23},{128,1,23},{132,1,23},{136,1,23},{140,1,23},
    {149,1,23},{153,1,23},{157,1,23},{161,1,23},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}},  /** Taiwan **/
	
#ifdef DFS_CHNL_SUPPORT
	{DOMAIN_CODE_AUS,{'A','U',' '},3,{{1,13,30}},
    72,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {52,1,24},{56,1,24},{60,1,24},{64,1,24},
    {100,1,30},{104,1,30},{108,1,30},{112,1,30},{116,1,30},{120,1,30},{124,1,30},{128,1,30},{132,1,30},{136,1,30},{140,1,30},
    {149,1,23},{153,1,23},{157,1,23},{161,1,23},{165,1,23},
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}},  /** Australia new code with 1-13, 32-48, 52-64, 100-140 and 149-165 **/
#else
	{DOMAIN_CODE_AUS,{'A','U',' '},3,{{1,13,20}},
    12,{{149,1,23},{153,1,23},{157,1,23},{161,1,23},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} 
	}},  /** Australia **/
#endif

       /*Begin:added by pengdecai for China country code*/
	{DOMAIN_CODE_CHI,{'C','N',' '},3,{{1,13,27}},    /*2.4G channel,from channel 1, and the max txpower is 27,3 is the length*/
	    15 ,{{149,1,23},{153,1,23},{157,1,23},{161,1,23},{165,1,23},   /*15 is the length(3*5, for 5 channel), 23 is the maxtxpower*/
	    {0,0,0},{0,0,0},{0,0,0},
	    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
	    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0} 
	}},  /** China **/
	/*End:added by pengdecai for China country code*/
	
	{DOMAIN_CODE_ALL,{'A','L','L'},3,{{1,14,30}},
	90,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
	{52,1,24},{56,1,24},{60,1,24},{64,1,24},
	{100,1,23},{104,1,23},{108,1,23},{112,1,23},{116,1,23},{120,1,23},{124,1,23},{128,1,23},{132,1,23},{136,1,23},{140,1,23},
	{149,1,23},{153,1,23},{157,1,23},{161,1,23},{165,1,23},
	{183,1,23},{184,1,23},{185,1,23},{187,1,23},{188,1,23},{189,1,23},
	{0,0,0}}},  /** DEFAULT **/ 

	{0,{'X','X',' '},3,{{1,14,30}},
    27,{{36,1,17},{40,1,17},{44,1,17},{48,1,17},
    {149,1,23},{153,1,23},{157,1,23},{161,1,23},{165,1,23},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},    
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	}}  /** DEFAULT **/ 
};

static DOMAIN IEEERegionChannel[]=
{
    /* US FCC */
#ifdef DFS_CHNL_SUPPORT
	{DOMAIN_CODE_FCC, {1,2,3,4,5,6,7,8,9,10,11,0,0,0,                   /*US FCC 2.4GHz*/
	36,40,44,48,                                                        /*US FCC 5GHz UNII-1*/
	52,56,60,64,                                                        /*US FCC UNII-2*/
#ifdef FCC_15E_INTERIM_PLAN
	100,104,108,112,116,132,136,140,                                    /*US FCC UNII-2 EXT*/
	149,153,157,161,                                                    /*US FCC UNII-2 */
    0,0,0,                                                              /*Filler for ch 120,124,128*/
#else
	100,104,108,112,116,120,124,128,132,136,140,                        /*US FCC UNII-2 EXT*/
	149,153,157,161,                                                    /*US FCC UNII-2 */
#endif
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0
	}},
#else /*!DFS_CHNL_SUPPORT*/
	{DOMAIN_CODE_FCC, {1,2,3,4,5,6,7,8,9,10,11,0,0,0,                   /*US FCC 2.4GHz*/
	36,40,44,48,                                                        /*US FCC 5GHz UNII-1*/
	149,153,157,161,                                                    /*US FCC UNII-2 */
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
	}},
#endif

    /* Canada IC */
#ifdef DFS_CHNL_SUPPORT
	{DOMAIN_CODE_IC, {1,2,3,4,5,6,7,8,9,10,11,0,0,0,                    /*Canada IC 2.4GHZ*/
	36,40,44,48,                                                        /*Canada IC 5GHz UNII-1*/
	52,56,60,64,                                                        /*Canada IC UNII-2*/
#ifdef FCC_15E_INTERIM_PLAN
	100,104,108,112,116,132,136,140,                                    /*Canada IC UNII-2 EXT*/
	149,153,157,161,                                                    /*Canada IC UNII-2*/
    0,0,0,                                                              /*Filler for ch 120,144,128*/
#else
	100,104,108,112,116,120,124,128,132,136,140,                        /*Canada IC UNII-2 EXT*/
	149,153,157,161,                                                    /*Canada IC UNII-2*/
#endif
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0
	}},
#else /*!DFS_CHNL_SUPPORT*/
	{DOMAIN_CODE_IC, {1,2,3,4,5,6,7,8,9,10,11,0,0,0,                    /*Canada IC 2.4GHZ*/
	36,40,44,48,                                                        /*Canada IC 5GHz UNII-1*/
	149,153,157,161,                                                    /*Canada IC UNII-2*/
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
	}},
#endif

    /* EU ETSI */
#ifdef DFS_CHNL_SUPPORT
	{DOMAIN_CODE_ETSI, {1,2,3,4,5,6,7,8,9,10,11,12,13,0,                   /*EU 2.4GHz*/
	36,40,44,48,                                                           /*EU 5GHz UNII-1*/
	52,56,60,64,                                                           /*EU 5GHz UNII-2*/
	100,104,108,112,116,120,124,128,132,136,140,                           /*EU 5GHz UNII-2 EXT*/ 
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0
	}},
#else /*!DFS_CHNL_SUPPORT*/
	{DOMAIN_CODE_ETSI, {1,2,3,4,5,6,7,8,9,10,11,12,13,0,                   /*EU 2.4GHz*/
	36,40,44,48,                                                           /*EU 5GHz UNII-1*/
	0,0,0,0,                                           
	0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0
	}},
#endif

    /* SPAIN */
	{DOMAIN_CODE_SPAIN, {10,11,0,0,0,0,0,0,0,0,0,0,0,0,                    /*2.4GHZ */
	36,40,44,48,                                                           /*5GHz UNII-1*/
	52,56,60,64,                                                           /*5GHz UNII-2*/
	100,104,108,112,116,120,124,128,132,136,140,                           /*5GHz UNII-2 EXT*/ 
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	}},

    /*FRANCE*/
	{DOMAIN_CODE_FRANCE, {10,11,12,13, 0,0,0,0,0,0,0,0,0,0,                /*2.4 GHz*/  
	36,40,44,48,                                                           /*5GHz UNII-1*/ 
	52,56,60,64,                                                           /*5GHz UNII-2*/ 
	100,104,108,112,116,120,124,128,132,136,140,                           /*5GHz UNII-2 EXT*/  
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	}},

    /* JAPAN MKK */
	{DOMAIN_CODE_MKK, {1,2,3,4,5,6,7,8,9,10,11,12,13, 14,                  /*2.4 GHz*/
	36,40,44,48,                                                           /*5GHz UNII-1*/ 
	52,56,60,64,                                                           /*5GHz UNII-2*/ 
	100,104,108,112,116,120,124,128,132,136,140,                           /*5GHz UNII-2 EXT*/   
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	}},
#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT /** currently not support on A7c chip **/
	{DOMAIN_CODE_MKK, {1,2,3,4,5,6,7,8,9,10,11,12,13,14,
	7,8,11,183,                                     
	184,185,187,188,189,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	}},
#endif

    /* DGT */
	{DOMAIN_CODE_DGT, {1,2,3,4,5,6,7,8,9,10,11,0,0,0,                       /*2.4 GHz*/
	52,56,60,64,                                                            /*5GHz UNII-2*/ 
	100,104,108,112,116,120,124,128,132,136,140,                            /*5GHz UNII-2 EXT*/ 
	149,153,157,161,                                                        /*5GHz UNII-2*/ 
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	}},

    /* AU */
#ifdef DFS_CHNL_SUPPORT
	{DOMAIN_CODE_AUS, {1,2,3,4,5,6,7,8,9,10,11,12,13,0,                    /*2.4 GHz*/ 
	36,40,44,48,                                                           /*5GHz UNII-1*/   
	52,56,60,64,                                                           /*5GHz UNII-2*/  
#ifdef FCC_15E_INTERIM_PLAN
	100,104,108,112,116,132,136,140,                                       /*5GHz UNII-2 EXT*/  
	149,153,157,161,                                                       /*5GHz UNII-2*/   
    0,0,0,
#else
	100,104,108,112,116,120,124,128,132,136,140,                           /*5GHz UNII-2 EXT*/  
	149,153,157,161,                                                       /*5GHz UNII-2*/   
#endif
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0
	}},
#else /*!DOMAIN_CODE_AUS*/
	{DOMAIN_CODE_AUS, {1,2,3,4,5,6,7,8,9,10,11,12,13,0,                    /*2.4 GHz*/  
	149,153,157,161,                                                       /*5GHz UNII-2*/    
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0
	}},
#endif
    /*Begin:added by pengdecai for China country code*/
    {DOMAIN_CODE_CHI, {1,2,3,4,5,6,7,8,9,10,11,12,13,0,         /*2.4 GHz*/  
	149,153,157,161,165,                                                  /*5GHz UNII-2*/    
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0
	}},     /* CHINA */
	/*End:added by pengdecai for China country code*/
    
    /* ALL*/
	{DOMAIN_CODE_ALL, { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13, 14,                        
	36,40,44,48,
	52,56,60,64,                                                  
	100,104,108,112,116,120,124,128,132,136,140,
	149,153,157,161,165,
	183,184,185,187,188,189,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
	}},

    /* Default set */
    {0, {1,2,3,4,5,6,7,8,9,10,11,0,0,0,
	36,40,44,48,
	149,153,157,161,165,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0
	}}
};

 
/*Begin:Added by pengdecai for country code*/
enum CountryID
{
	CTRY_DEFAULT               =0,          /*default*/
	CTRY_AUSTRALIA            = 36,      /* Australia */
	CTRY_CANADA               = 124,     /* Canada */
	CTRY_CHINA                = 156,     /* People's Republic of China */
	CTRY_FRANCE               = 250,     /* France */
	CTRY_SPAIN                = 724,     /* Spain */
	CTRY_UNITED_STATES        = 840,     /* United States */
	CTRY_TAIWAN               = 158,     /* Taiwan */
	CTRY_UNITED_STATES_FCC49  = 842,     /* United States (Public Safety)*/
	CTRY_JAPAN1               = 393,     /* Japan (JP1) */
};

#define N(a) (sizeof(a)/sizeof(a[0]))

static COUNTRY_DOMAIN  CountryDomain[]=
{
	{CTRY_UNITED_STATES,DOMAIN_CODE_FCC},
	{CTRY_CANADA,DOMAIN_CODE_IC},
	{CTRY_DEFAULT,DOMAIN_CODE_ETSI}, //no country code yet
	{CTRY_SPAIN,DOMAIN_CODE_SPAIN},
	{CTRY_FRANCE,DOMAIN_CODE_FRANCE},
	{CTRY_JAPAN1,DOMAIN_CODE_MKK},
	{CTRY_TAIWAN,DOMAIN_CODE_DGT},
	{CTRY_AUSTRALIA,DOMAIN_CODE_AUS},
	{CTRY_CHINA,DOMAIN_CODE_CHI},  
	{CTRY_DEFAULT,DOMAIN_CODE_ALL}
};

/*get domain by countryID*/
UINT8  CountryID_TO_Domain(UINT16 CountryID)
{
	int i;
	for (i = 0; i < N(CountryDomain) ;i ++)
	{
		if(CountryID == CountryDomain[i].countryID)
		{
			domainSetDomain(IEEERegionPower[i].domainCode);
			return CountryDomain[i].domainCode;
		}
	}
	printk("The CountryID may be error or the device don't support this country!");
	domainSetDomain(0);
	return 0; //enable all channel.
}

int  Domain_TO_CountryID(void)
{
	int i ;
	UINT8 domainCode=0;
#ifdef IEEE80211H      
    if (bcn_reg_domain)
	domainCode = bcn_reg_domain;          
#else
	domainCode = GetDomainCode();
#endif /* IEEE80211H */

	for (i = 0; i < N(CountryDomain) ;i ++)
	{
		if(domainCode == CountryDomain[i].domainCode)
		{
			return CountryDomain[i].countryID;
		}
	}
	printk("Error: can not get the Country ID,now use the default domain!\n");
	return CTRY_DEFAULT;
}

/*get domain from country code */
UINT8 CountryCode_TO_Domain(char *countrycode)
{	
	int i = 0;

      if(2== strlen(countrycode))
	  	countrycode[2]=' ';
	while (IEEERegionPower[i].domainCode)
	{
		if (0 == memcmp(countrycode,&IEEERegionPower[i].CountryString[0],3))
		{
#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT
			if(mib_channelspacing==10)  /** hack for Japan channel spacing of 10 **/
			{
				i++;  /** use the next channel list instead **/
			}
#endif
			domainSetDomain(IEEERegionPower[i].domainCode);
			return IEEERegionPower[i].domainCode;
		}
		i++;
	}
	printk("The CountryID may be error or the device don't support this country!\n");
	domainSetDomain(0);
	return 0;

}

/*get country code from domain*/
int  Domain_TO_CountryCode(UINT8 * code_buf)
{
	int i = 0;
	UINT8 domainCode=0;
#ifdef IEEE80211H      
    if (bcn_reg_domain)
	domainCode = bcn_reg_domain;          
#else
	domainCode = GetDomainCode();
#endif /* IEEE80211H */


	while (IEEERegionPower[i].domainCode)
	{
		if (domainCode ==IEEERegionPower[i].domainCode)
		{
#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT
			if(mib_channelspacing==10)  /** hack for Japan channel spacing of 10 **/
			{
				i++;  /** use the next channel list instead **/
			}
#endif
			memcpy(code_buf, IEEERegionPower[i].CountryString,sizeof(IEEERegionPower[i].CountryString));
			return 1;
		}
		i++;
	}
	printk("Error: can not get the Country code,now use the default domain!\n");
	memcpy(code_buf, IEEERegionPower[i].CountryString,sizeof(IEEERegionPower[i].CountryString));
	return 0;

}
/*End:Added by pengdecai for country code*/

int domainSetDomain(UINT8 domain)
{
#ifdef IEEE80211H    
	bcn_reg_domain = domain;    
#endif /* IEEE80211H */
	return 0;
}
UINT8 domainGetDomain(void)
{
#ifdef IEEE80211H    
	return bcn_reg_domain;    
#endif /* IEEE80211H */	
}
static int isInChannelList(UINT8 channel, UINT8 band,  UINT8 *ChannelList)
{
	int i;
	if(band==FREQ_BAND_5GHZ)
	{
		for (i =14; i <IEEE_80211_MAX_NUMBER_OF_CHANNELS; i++)
		{
			if(channel == ChannelList[i])
				return 1;
		}
	}
	else
	{
		for (i =0; i <14; i++)
		{
			if(channel == ChannelList[i])
				return 1;
		}
	}
	return 0;
}
int domainChannelValid(UINT8 channel, UINT8 band)
{
	int i = 0;
	UINT8 domainCode=0;
#ifdef IEEE80211H    
    if (bcn_reg_domain)
	domainCode = bcn_reg_domain;    
#else
	domainCode = GetDomainCode();
#endif /* IEEE80211H */
	while (IEEERegionChannel[i].domainCode)
	{
		if (domainCode == IEEERegionChannel[i].domainCode)
		{
#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT
			if(mib_channelspacing==10)  /** hack for Japan channel spacing of 10 **/
			{
				i++;  /** use the next channel list instead **/
			}
#endif
			return isInChannelList(channel, band, IEEERegionChannel[i].ChannelList);
		}
		i++;
	}
	return isInChannelList(channel, band, IEEERegionChannel[i].ChannelList);
}
/*
** domainGetInfo
*
*  FILENAME: D:\jshen\802.11_MAC-DRIVER_SW\AP\Src\domain.c
*
*  PARAMETERS:
*
*  DESCRIPTION:
*
*  RETURNS:
*
*/
int domainGetInfo(UINT8 *ChannelList/* NULL Terminate*/)
{
	int i = 0;
	UINT8 domainCode=0;
#ifdef IEEE80211H    
    if (bcn_reg_domain)
	domainCode = bcn_reg_domain;    
#else
	domainCode = GetDomainCode();
#endif /* IEEE80211H */

	while (IEEERegionChannel[i].domainCode)
	{
		if (domainCode == IEEERegionChannel[i].domainCode)
		{
#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT
			if(mib_channelspacing==10)  /** hack for Japan channel spacing of 10 **/
			{
				i++;  /** use the next channel list instead **/
			}
#endif
			memcpy(ChannelList, IEEERegionChannel[i].ChannelList, IEEE_80211_MAX_NUMBER_OF_CHANNELS);
			return 1;
		}
		i++;
	}
	memcpy(ChannelList, IEEERegionChannel[i].ChannelList, IEEE_80211_MAX_NUMBER_OF_CHANNELS);
	return 0;
}

/*
** domainGetPowerInfo
*
* 
*  PARAMETERS:
*
*  DESCRIPTION:
*
*  RETURNS:
*
*/
int domainGetPowerInfo(UINT8 *info)
{
	int i = 0;
	UINT8 domainCode=0;
#ifdef IEEE80211H      
    if (bcn_reg_domain)
	domainCode = bcn_reg_domain;          
#else
	domainCode = GetDomainCode();
#endif /* IEEE80211H */



	while (IEEERegionPower[i].domainCode)
	{
		if (domainCode ==IEEERegionPower[i].domainCode)
		{
#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT
			if(mib_channelspacing==10)  /** hack for Japan channel spacing of 10 **/
			{
				i++;  /** use the next channel list instead **/
			}
#endif
			memcpy(info, IEEERegionPower[i].CountryString,sizeof(DOMAIN_TXPOWER)-sizeof(IEEERegionPower[i].domainCode));
			return 1;
		}
		i++;
	}

	memcpy(info, IEEERegionPower[i].CountryString, sizeof(DOMAIN_TXPOWER)-sizeof(IEEERegionPower[i].domainCode));
	return 0;
}


