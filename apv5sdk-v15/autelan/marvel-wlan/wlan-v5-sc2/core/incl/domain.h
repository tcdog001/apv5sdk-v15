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

#ifndef _DOMAIN_H_
#define _DOMAIN_H_
#include "wltypes.h" //added by pengdecai
#define DOMAIN_CODE_FCC     0x10
#define DOMAIN_CODE_IC      0x20
#define DOMAIN_CODE_ETSI    0x30
#define DOMAIN_CODE_SPAIN   0x31
#define DOMAIN_CODE_FRANCE  0x32
#define DOMAIN_CODE_MKK     0x40
#define DOMAIN_CODE_DGT    0x80
#define DOMAIN_CODE_AUS    0x81
#define DOMAIN_CODE_CHI     0x82 //added by pengdecai

#ifdef JAPAN_CHANNEL_SPACING_10_SUPPORT
#define DOMAIN_CODE_MKK2     0x41  /** for japan channel with spacing 10 **/
#endif
#define DOMAIN_CODE_MKK3     0x41  /** for japan channel - 5450-5725 MHz */
#define DOMAIN_CODE_ALL		0xff
int domainGetInfo(unsigned char *ChannelList/* NULL Terminate*/);
int domainChannelValid(unsigned char channel, unsigned char band);

unsigned char domainGetDomain(void);
extern int domainSetDomain(unsigned char domain);
extern int domainGetPowerInfo(unsigned char *info);
int  Domain_TO_CountryID(void);
UINT8  CountryID_TO_Domain(UINT16 CountryID);
int  Domain_TO_CountryCode(UINT8 * code_buf);
UINT8 CountryCode_TO_Domain(char *countrycode); 
#endif /*_DOMAIN_H_*/
