/*
 *  Copyright © 2005 Atheros Communications, Inc.,  All Rights Reserved.
 *
 */

#include <stdio.h>

#include "wlantype.h"
//#include "mdata.h"
#include "rate_constants.h"

#include "NewArt.h"
#include "ParameterSelect.h"

#include "Card.h"
#include "smatch.h"
#include "Field.h"

#include "Device.h"

#include "mCal9300.h"
#include "mEepStruct9300.h"

#include "Ar9300CalibrationApply.h"
#include "Ar9300EepromSave.h"
//#include "Ar9300EepromRestore.h"
#include "ar9300_target_pwr.h"
#include "Ar9300Device.h"
#include "Ar9300Temperature.h"
#include "ParameterConfigDef.h"
#include "ConfigurationStatus.h"
//
// hal header files
//
#include "ah.h"
#include "ah_internal.h"
#include "ar9300.h"
#include "ar9300eep.h"
#include "ar9300desc.h"
#include "UserPrint.h"

//#include "ar9300.h"
#include "ah_osdep.h"
#include "opt_ah.h"



#define MDCU 10			// should we only set the first 8??
#define MQCU 10

#define MBUFFER 1024

#define MAC_PCU_STA_ADDR_L32 0x00008000
#define MAC_PCU_STA_ADDR_U16 0x00008004
#define MAC_PCU_BSSID_L32 0x00008008
#define MAC_PCU_BSSID_U16 0x0000800c

static int deafMode = 0;
static int undeafThresh62 = 0;
static int undeafThresh62Ext = 0;
static int undeafForceAgcClear = 0;
static int undeafCycpwrThr1 = 0;
static int undeafCycpwrThr1Ext = 0;
static int undeafRssiThr1a = 0;

//#ifdef UNUSED
int calData = CALDATA_AUTO;
//#endif

#ifdef UNUSED
{"MAC_PCU_STA_ADDR_U16", "ADDR_47_32"                           , 0x00008004, 15, 0 },
{"MAC_PCU_BSSID_L32", "ADDR"                                    , 0x00008008, 31, 0 },
{"MAC_PCU_BSSID_U16", "AID"                                     , 0x0000800c, 26, 16},
{"MAC_PCU_BSSID_U16", "ADDR"                                    , 0x0000800c, 15, 0 },
#endif

int Ar9300FlashCal(int value)
{
#ifdef MDK_AP
    calData = CALDATA_FLASH ;
    UserPrint("FlashCal = %d\n", calData);
	return 0;
#else
	printf("Error: Flash access for windows build is not supported.\n");
	return -1;
#endif
}

static int Ar9300BssIdSet(unsigned char *bssid)
{
	unsigned int reg;

	reg=bssid[3]<<24|bssid[2]<<16|bssid[1]<<8|bssid[0];
    MyRegisterWrite(MAC_PCU_BSSID_L32,reg);

//    reg=0;
	MyRegisterRead(MAC_PCU_BSSID_U16,&reg);
	reg &= ~(0xffff);
	reg |= (bssid[5]<<8|bssid[4]);
    MyRegisterWrite(MAC_PCU_BSSID_U16,reg);

	return 0;
}


static int Ar9300StationIdSet(unsigned char *mac)
{
	unsigned int reg;

	reg=mac[3]<<24|mac[2]<<16|mac[1]<<8|mac[0];
    MyRegisterWrite(MAC_PCU_STA_ADDR_L32,reg);

//    reg=0;
	MyRegisterRead(MAC_PCU_STA_ADDR_U16,&reg);
	reg &= ~(0xffff);
	reg |= (mac[5]<<8|mac[4]);
    MyRegisterWrite(MAC_PCU_STA_ADDR_U16,reg);

	return 0;
}

#define AR9300_EEPROM_SIZE 16*1024  // byte addressable


int MyAr9300EepromRead(unsigned int address, unsigned char *buffer, int many)
{
    if(!ar9300CalibrationDataReadEeprom(AH, address, (unsigned char*)buffer, many ))// return 0 if eprom read is correct; ar9300EepromRead returns 1 for success
		return 1; // bad read

    return 0;
}


int Ar9300EepromWrite(unsigned int address, unsigned char *buffer, int many)
{
    unsigned int eepAddr;
    unsigned int byteAddr;
    unsigned short svalue, word;
    int i;

	if(((address)<0)||((address+many)>AR9300_EEPROM_SIZE-1)){
		return 1; // bad address
	}

#ifdef MDK_AP
    if(calData == CALDATA_FLASH ){
        int fd, it;
        if((fd = open("/dev/caldata", O_RDWR)) < 0) {
            perror("Could not open flash\n");
            return 1 ;
        }
        //printf("Address = %x, many = %d\n", address, many);
        //for(it=0;it<many;it++)
        //    printf("Addr %x, data = %x\n", address+it, buffer[it]);
        lseek(fd, address, SEEK_SET);
        if (write(fd, buffer, many) != many) {
                perror("\nwrite\n");
                return 1;
        }
		close(fd);
        return 0;
    }
#endif
    for(i=0;i<many;i++){
        eepAddr = (unsigned short)(address+i)/2;
       	byteAddr = (unsigned short) (address+i)%2;
		if(!ar9300EepromReadWord(AH, eepAddr, &svalue ))// return 0 if eprom read is correct; ar9300EepromRead returns 1 for success
			return 1; // bad read
        word = buffer[i]<<(byteAddr*8);
        svalue = svalue & ~(0xff<<(byteAddr*8));
        svalue = svalue | word;
		if(!ar9300EepromWrite(AH, eepAddr,  svalue ))// return 0 if eprom write is correct; ar9300EepromWrite returns 1 for success
			return 1; // bad write
	}
    return 0;
}


#define OTP_SIZE 1024 // in bytes // 512 words; total 1024*8 bits

int Ar9300OtpRead(unsigned int address, unsigned char *buffer, int many)
{
    if(!ar9300CalibrationDataReadOtp(AH, address, (unsigned char*)buffer, many ))// return 0 if eprom read is correct; ar9300EepromRead returns 1 for success
		return 1; // bad read

    return 0;
}

int Ar9300OtpWrite(unsigned int address, unsigned char *buffer, int many)
{
#ifdef OLDCODE
	char value[4];
    unsigned short otpAddr;
    unsigned short byteAddr;
    unsigned int *value_4;
    int i;

	if(((address)<0)||((address+many)>OTP_SIZE-1)){
		return 1; // bad address
	}

    for(i=0;i<many;i++){
        otpAddr =(unsigned short) (address+i)/4;
        byteAddr =(unsigned short) (address+i)%4;
        value_4 = (unsigned int *)value;
		*value_4=0;
		value[byteAddr]=buffer[i];

		if(!ar9300OtpWrite(AH, otpAddr,  *value_4 ))// return 0 if eprom write is correct; ar9300EepromWrite returns 1 for success
			return 1; // bad write
	}

    return 0;
#else
    unsigned int eepAddr;
    unsigned int byteAddr;
    unsigned int word;
    unsigned int svalue;
    int i;

	if(((address)<0)||((address+many)>OTP_SIZE)){
		return 1; // bad address
	}

     for(i=0;i<many;i++){
        eepAddr = (unsigned short)(address+i)/4;
       	byteAddr = (unsigned short) (address+i)%4;
		if(!ar9300OtpRead(AH, eepAddr, &svalue ))// return 0 if eprom read is correct; ar9300EepromRead returns 1 for success
			return 1; // bad read
        word = buffer[i]<<(byteAddr*8);
        svalue = svalue & ~(0xff<<(byteAddr*8));
        svalue = svalue | word;
		if(!ar9300OtpWrite(AH, eepAddr,  svalue ))// return 0 if eprom write is correct; ar9300EepromWrite returns 1 for success
			return 1; // bad write
	}
    return 0;
#endif
}


static int Ar9300TransmitCarrier(int frequency, unsigned int txchain)
{
	if(frequency<4000)
	{
    FieldWrite("ch0_rxtx2.synthon",0x1);
    FieldWrite("ch0_rxtx2.synthon_ovr",0x1);
    FieldWrite("ch0_pllclkmoda2.rst_warm_int_l",0x0);
    FieldWrite("ch0_top2.local_addacpwd",0x1);
    FieldWrite("ch0_top2.dacpwd",0x0);
    FieldWrite("ch0_top2.adcpwd",0x7);

    if(txchain&1)
	{
		FieldWrite("ch0_txrf2.ob2g_paloff",0x3);
		FieldWrite("ch0_txrf2.db2g",0x5);
		FieldWrite("ch0_rxtx3.dacfullscale",0x1);
		FieldWrite("ch0_rxtx1.mantxgain",0x1);
		FieldWrite("ch0_rxtx1.tx1db_biquad",0x7);
		FieldWrite("ch0_rxtx1.tx6db_biquad",0x2);
		FieldWrite("ch0_rxtx1.txbb_gc",0x7);
		FieldWrite("ch0_rxtx1.padrv2gn",0x15);
		FieldWrite("ch0_rxtx2.txon",0x1);
		FieldWrite("ch0_rxtx2.paon",0x1);
		FieldWrite("ch0_rxtx2.rxon",0x0);
		FieldWrite("ch0_rxtx2.txon_ovr",0x1);
		FieldWrite("ch0_rxtx2.paon_ovr",0x1);
		FieldWrite("ch0_rxtx2.rxon_ovr",0x1);
		FieldWrite("ch0_rxtx3.loforcedon",0x1);
		FieldWrite("ch0_txrf1.lodiv2gforced",0x1);
		FieldWrite("ch0_rxtx3.dacpwd",0x0);
		FieldWrite("ch0_rxtx3.dacpwd_ovr",0x1);
		FieldWrite("ch0_rxtx3.adcpwd",0x1);
		FieldWrite("ch0_rxtx3.adcpwd_ovr",0x1);
		FieldWrite("ch0_txrf1.pdlodiv2g",0x0);
		FieldWrite("ch0_bb2.switch_override",0x1);
		FieldWrite("ch0_bb1.force_i2v",0x1);
		FieldWrite("ch0_bb1.force_v2i",0x1);
		FieldWrite("ch0_bb1.force_osdac",0x1);
		FieldWrite("ch0_bb1.force_biquad",0x1);
		FieldWrite("ch0_bb1.force_notch",0x1);
		FieldWrite("ch0_bb1.force_loq",0x1);
		FieldWrite("ch0_bb1.enable_v2i",0x1);
		FieldWrite("ch0_bb1.enable_biquad",0x1);
		FieldWrite("ch0_bb1.enable_notch",0x1);
		FieldWrite("ch0_rxtx3.caltx",0x0);
		FieldWrite("ch0_rxtx3.calpa",0x0);
		FieldWrite("ch0_rxtx3.caltx_ovr",0x1);
		FieldWrite("ch0_rxtx3.calpa_ovr",0x1);
		FieldWrite("ch0_rxtx3.calfc",0x0);
		FieldWrite("ch0_rxtx3.calfc_ovr",0x1);
	}
#ifdef NOREPEAT
    FieldWrite("a28c",0x11111;
    FieldWrite("a288",0x111;

    FieldWrite("ch0_rxtx2.synthon",0x1
    FieldWrite("ch0_rxtx2.synthon_ovr",0x1
    FieldWrite("ch0_pllclkmoda2.rst_warm_int_l",0x0
    FieldWrite("ch0_top2.localaddacpwd",0x1
    FieldWrite("ch0_top2.dacpwd",0x0
    FieldWrite("ch0_top2.adcpwd",0x7
#endif

	if(txchain&2)
	{
		FieldWrite("ch1_txrf2.ob2g_paloff",0x3);
		FieldWrite("ch1_txrf2.db2g",0x5);
		FieldWrite("ch1_rxtx3.dacfullscale",0x1);
		FieldWrite("ch1_rxtx1.mantxgain",0x1);
		FieldWrite("ch1_rxtx1.tx1db_biquad",0x7);
		FieldWrite("ch1_rxtx1.tx6db_biquad",0x2);
		FieldWrite("ch1_rxtx1.txbb_gc",0x7);
		FieldWrite("ch1_rxtx1.padrv2gn",0x15);
		FieldWrite("ch1_rxtx2.txon",0x1);
		FieldWrite("ch1_rxtx2.paon",0x1);
		FieldWrite("ch1_rxtx2.rxon",0x0);
		FieldWrite("ch1_rxtx2.txon_ovr",0x1);
		FieldWrite("ch1_rxtx2.paon_ovr",0x1);
		FieldWrite("ch1_rxtx2.rxon_ovr",0x1);
		FieldWrite("ch1_rxtx3.loforcedon",0x1);
		FieldWrite("ch1_txrf1.lodiv2gforced",0x1);
		FieldWrite("ch1_rxtx3.dacpwd",0x0);
		FieldWrite("ch1_rxtx3.dacpwd_ovr",0x1);
		FieldWrite("ch1_rxtx3.adcpwd",0x1);
		FieldWrite("ch1_rxtx3.adcpwd_ovr",0x1);
		FieldWrite("ch1_txrf1.pdlodiv2g",0x0);
		FieldWrite("ch1_bb2.switch_override",0x1);
		FieldWrite("ch1_bb1.force_i2v",0x1);
		FieldWrite("ch1_bb1.force_v2i",0x1);
		FieldWrite("ch1_bb1.force_osdac",0x1);
		FieldWrite("ch1_bb1.force_biquad",0x1);
		FieldWrite("ch1_bb1.force_notch",0x1);
		FieldWrite("ch1_bb1.force_loq",0x1);
		FieldWrite("ch1_bb1.enable_v2i",0x1);
		FieldWrite("ch1_bb1.enable_biquad",0x1);
		FieldWrite("ch1_bb1.enable_notch",0x1);
		FieldWrite("ch1_rxtx3.caltx",0x0);
		FieldWrite("ch1_rxtx3.calpa",0x0);
		FieldWrite("ch1_rxtx3.caltx_ovr",0x1);
		FieldWrite("ch1_rxtx3.calpa_ovr",0x1);
		FieldWrite("ch1_rxtx3.calfc",0x0);
		FieldWrite("ch1_rxtx3.calfc_ovr",0x1);
	}

#ifdef NOREPEAT
    FieldWrite("a28c",0x11111;
    FieldWrite("a288",0x111;

reset f=2447;ht40=0;ch=4
    FieldWrite("ch0_rxtx2.synthon",0x1
    FieldWrite("ch0_rxtx2.synthon_ovr",0x1
    FieldWrite("ch0_pllclkmoda2.rst_warm_int_l",0x0
    FieldWrite("ch0_top2.localaddacpwd",0x1
    FieldWrite("ch0_top2.dacpwd",0x0
    FieldWrite("ch0_top2.adcpwd",0x7
#endif

	if(txchain&4)
	{
		FieldWrite("ch2_txrf2.ob2g_paloff",0x3);
		FieldWrite("ch2_txrf2.db2g",0x5);
		FieldWrite("ch2_rxtx3.dacfullscale",0x1);
		FieldWrite("ch2_rxtx1.mantxgain",0x1);
		FieldWrite("ch2_rxtx1.tx1db_biquad",0x7);
		FieldWrite("ch2_rxtx1.tx6db_biquad",0x2);
		FieldWrite("ch2_rxtx1.txbb_gc",0x7);
		FieldWrite("ch2_rxtx1.padrv2gn",0x15);
		FieldWrite("ch2_rxtx2.txon",0x1);
		FieldWrite("ch2_rxtx2.paon",0x1);
		FieldWrite("ch2_rxtx2.rxon",0x0);
		FieldWrite("ch2_rxtx2.txon_ovr",0x1);
		FieldWrite("ch2_rxtx2.paon_ovr",0x1);
		FieldWrite("ch2_rxtx2.rxon_ovr",0x1);
		FieldWrite("ch2_rxtx3.loforcedon",0x1);
		FieldWrite("ch2_txrf1.lodiv2gforced",0x1);
		FieldWrite("ch2_rxtx3.dacpwd",0x0);
		FieldWrite("ch2_rxtx3.dacpwd_ovr",0x1);
		FieldWrite("ch2_rxtx3.adcpwd",0x1);
		FieldWrite("ch2_rxtx3.adcpwd_ovr",0x1);
		FieldWrite("ch2_txrf1.pdlodiv2g",0x0);
		FieldWrite("ch2_bb2.switch_override",0x1);
		FieldWrite("ch2_bb1.force_i2v",0x1);
		FieldWrite("ch2_bb1.force_v2i",0x1);
		FieldWrite("ch2_bb1.force_osdac",0x1);
		FieldWrite("ch2_bb1.force_biquad",0x1);
		FieldWrite("ch2_bb1.force_notch",0x1);
		FieldWrite("ch2_bb1.force_loq",0x1);
		FieldWrite("ch2_bb1.enable_v2i",0x1);
		FieldWrite("ch2_bb1.enable_biquad",0x1);
		FieldWrite("ch2_bb1.enable_notch",0x1);
		FieldWrite("ch2_rxtx3.caltx",0x0);
		FieldWrite("ch2_rxtx3.calpa",0x0);
		FieldWrite("ch2_rxtx3.caltx_ovr",0x1);
		FieldWrite("ch2_rxtx3.calpa_ovr",0x1);
		FieldWrite("ch2_rxtx3.calfc",0x0);
		FieldWrite("ch2_rxtx3.calfc_ovr",0x1);
	}

    MyRegisterWrite(0xa28c,0x11111);
    MyRegisterWrite(0xa288,0x220);
	}
	else
	{
		if(txchain&1)
		{
			FieldWrite("ch0_rxtx1.padrv3gn5g",0x70);

			FieldWrite("ch0_rxtx2.txon",0x1);
			FieldWrite("ch0_rxtx1.tx1db_biquad",0x0);
			FieldWrite("ch0_rxtx1.tx6db_biquad",0x1);
			FieldWrite("ch0_rxtx1.padrv2gn",0xf);
			FieldWrite("ch0_rxtx1.padrv4gn5g",0x2);
			FieldWrite("ch0_rxtx1.txbb_gc",0x3);

			FieldWrite("ch2_rxtx2.bmode",0x0);
			FieldWrite("ch1_rxtx2.bmode",0x0);
			FieldWrite("ch0_rxtx2.bmode",0x0);
			FieldWrite("ch2_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch1_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch0_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch2_rxtx3.dacfullscale",0x1);
			FieldWrite("ch1_rxtx3.dacfullscale",0x1);
			FieldWrite("ch0_rxtx3.dacfullscale",0x1);
			FieldWrite("ch2_rxtx3.loforcedon",0x1);
			FieldWrite("ch1_rxtx3.loforcedon",0x1);
			FieldWrite("ch0_rxtx3.loforcedon",0x1);
			FieldWrite("ch2_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch1_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch0_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch0_rxtx2.synthon",0x1);
			FieldWrite("ch0_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch1_rxtx2.synthon",0x1);
			FieldWrite("ch1_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch2_rxtx2.synthon",0x1);
			FieldWrite("ch2_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch2_top2.local_addacpwd",0x1);
			FieldWrite("ch1_top2.local_addacpwd",0x1);
			FieldWrite("ch0_top2.local_addacpwd",0x1);
			FieldWrite("ch2_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch1_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch0_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch2_top2.dacpwd",0x3);
			FieldWrite("ch1_top2.dacpwd",0x3);
			FieldWrite("ch0_top2.dacpwd",0x3);
			FieldWrite("ch2_top2.adcpwd",0x7);
			FieldWrite("ch1_top2.adcpwd",0x7);
			FieldWrite("ch0_top2.adcpwd",0x7);

			FieldWrite("ch0_txrf3.ob5g",0x3);
			FieldWrite("ch0_txrf3.d2b5g",0x3);
			FieldWrite("ch0_txrf2.d3b5g",0x3);
			FieldWrite("ch0_txrf2.d4b5g",0x3);
			FieldWrite("ch0_txrf3.ocas5g",0x0);
			FieldWrite("ch0_txrf3.d2cas5g",0x2);
			FieldWrite("ch0_txrf3.d3cas5g",0x2);
			FieldWrite("ch0_txrf3.d4cas5g",0x2);
			FieldWrite("ch0_rxtx1.mantxgain",0x1);
			FieldWrite("ch0_rxtx2.paon",0x1);
			FieldWrite("ch0_rxtx2.rxon",0x0);
			FieldWrite("ch0_rxtx2.txon_ovr",0x1);
			FieldWrite("ch0_rxtx2.paon_ovr",0x1);
			FieldWrite("ch0_rxtx2.rxon_ovr",0x1);
			FieldWrite("ch0_rxtx3.dacpwd",0x0);
			FieldWrite("ch0_rxtx3.dacpwd_ovr",0x1);
			FieldWrite("ch0_rxtx3.adcpwd",0x1);
			FieldWrite("ch0_rxtx3.adcpwd_ovr",0x1);
			FieldWrite("ch0_txrf1.pdlodiv2g",0x0);
			FieldWrite("ch0_bb2.switch_override",0x1);
			FieldWrite("ch0_bb1.force_i2v",0x1);
			FieldWrite("ch0_bb1.force_v2i",0x1);
			FieldWrite("ch0_bb1.force_osdac",0x1);
			FieldWrite("ch0_bb1.force_biquad",0x1);
			FieldWrite("ch0_bb1.force_notch",0x1);
			FieldWrite("ch0_bb1.force_loq",0x1);
			FieldWrite("ch0_bb1.enable_v2i",0x1);
			FieldWrite("ch0_bb1.enable_biquad",0x1);
			FieldWrite("ch0_bb1.enable_notch",0x1);
			FieldWrite("ch0_rxtx3.caltx",0x0);
			FieldWrite("ch0_rxtx3.calpa",0x0);
			FieldWrite("ch0_rxtx3.caltx_ovr",0x1);
			FieldWrite("ch0_rxtx3.calpa_ovr",0x1);
			FieldWrite("ch0_rxtx3.calfc",0x0);
			FieldWrite("ch0_rxtx3.calfc_ovr",0x1);

			MyRegisterWrite(0xa28c,0x22222);
			MyRegisterWrite(0xa288,0x222);
		}
		if(txchain&2)
		{
			FieldWrite("ch1_rxtx1.padrv3gn5g",0x9);

			FieldWrite("ch1_rxtx2.txon",0x1);
			FieldWrite("ch1_rxtx1.tx1db_biquad",0x0);
			FieldWrite("ch1_rxtx1.tx6db_biquad",0x1);
			FieldWrite("ch1_rxtx1.padrv2gn",0xf);
			FieldWrite("ch1_rxtx1.padrv4gn5g",0x2);
			FieldWrite("ch1_rxtx1.txbb_gc",0x3);

			FieldWrite("ch2_rxtx2.bmode",0x0);
			FieldWrite("ch1_rxtx2.bmode",0x0);
			FieldWrite("ch0_rxtx2.bmode",0x0);
			FieldWrite("ch2_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch1_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch0_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch2_rxtx3.dacfullscale",0x1);
			FieldWrite("ch1_rxtx3.dacfullscale",0x1);
			FieldWrite("ch0_rxtx3.dacfullscale",0x1);
			FieldWrite("ch2_rxtx3.loforcedon",0x1);
			FieldWrite("ch1_rxtx3.loforcedon",0x1);
			FieldWrite("ch0_rxtx3.loforcedon",0x1);
			FieldWrite("ch2_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch1_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch0_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch0_rxtx2.synthon",0x1);
			FieldWrite("ch0_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch1_rxtx2.synthon",0x1);
			FieldWrite("ch1_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch2_rxtx2.synthon",0x1);
			FieldWrite("ch2_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch2_top2.local_addacpwd",0x1);
			FieldWrite("ch1_top2.local_addacpwd",0x1);
			FieldWrite("ch0_top2.local_addacpwd",0x1);
			FieldWrite("ch2_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch1_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch0_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch2_top2.dacpwd",0x3);
			FieldWrite("ch1_top2.dacpwd",0x3);
			FieldWrite("ch0_top2.dacpwd",0x3);
			FieldWrite("ch2_top2.adcpwd",0x7);
			FieldWrite("ch1_top2.adcpwd",0x7);
			FieldWrite("ch0_top2.adcpwd",0x7);

			FieldWrite("ch1_txrf3.ob5g",0x3);
			FieldWrite("ch1_txrf3.d2b5g",0x3);
			FieldWrite("ch1_txrf2.d3b5g",0x3);
			FieldWrite("ch1_txrf2.d4b5g",0x3);
			FieldWrite("ch1_txrf3.ocas5g",0x0);
			FieldWrite("ch1_txrf3.d2cas5g",0x2);
			FieldWrite("ch1_txrf3.d3cas5g",0x2);
			FieldWrite("ch1_txrf3.d4cas5g",0x2);
			FieldWrite("ch1_rxtx1.mantxgain",0x1);
			FieldWrite("ch1_rxtx2.paon",0x1);
			FieldWrite("ch1_rxtx2.rxon",0x0);
			FieldWrite("ch1_rxtx2.txon_ovr",0x1);
			FieldWrite("ch1_rxtx2.paon_ovr",0x1);
			FieldWrite("ch1_rxtx2.rxon_ovr",0x1);
			FieldWrite("ch1_rxtx3.dacpwd",0x0);
			FieldWrite("ch1_rxtx3.dacpwd_ovr",0x1);
			FieldWrite("ch1_rxtx3.adcpwd",0x1);
			FieldWrite("ch1_rxtx3.adcpwd_ovr",0x1);
			FieldWrite("ch1_txrf1.pdlodiv2g",0x0);
			FieldWrite("ch1_bb2.switch_override",0x1);
			FieldWrite("ch1_bb1.force_i2v",0x1);
			FieldWrite("ch1_bb1.force_v2i",0x1);
			FieldWrite("ch1_bb1.force_osdac",0x1);
			FieldWrite("ch1_bb1.force_biquad",0x1);
			FieldWrite("ch1_bb1.force_notch",0x1);
			FieldWrite("ch1_bb1.force_loq",0x1);
			FieldWrite("ch1_bb1.enable_v2i",0x1);
			FieldWrite("ch1_bb1.enable_biquad",0x1);
			FieldWrite("ch1_bb1.enable_notch",0x1);
			FieldWrite("ch1_rxtx3.caltx",0x0);
			FieldWrite("ch1_rxtx3.calpa",0x0);
			FieldWrite("ch1_rxtx3.caltx_ovr",0x1);
			FieldWrite("ch1_rxtx3.calpa_ovr",0x1);
			FieldWrite("ch1_rxtx3.calfc",0x0);
			FieldWrite("ch1_rxtx3.calfc_ovr",0x1);

			MyRegisterWrite(0xa28c,0x22222);
			MyRegisterWrite(0xa288,0x222);
		}
		if(txchain&4)
		{
			FieldWrite("ch2_rxtx1.padrv3gn5g",0x7);

			FieldWrite("ch2_rxtx2.txon",0x1);
			FieldWrite("ch2_rxtx1.tx1db_biquad",0x0);
			FieldWrite("ch2_rxtx1.tx6db_biquad",0x1);
			FieldWrite("ch2_rxtx1.padrv2gn",0xf);
			FieldWrite("ch2_rxtx1.padrv4gn5g",0x2);
			FieldWrite("ch2_rxtx1.txbb_gc",0x3);

			FieldWrite("ch2_rxtx2.bmode",0x0);
			FieldWrite("ch1_rxtx2.bmode",0x0);
			FieldWrite("ch0_rxtx2.bmode",0x0);
			FieldWrite("ch2_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch1_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch0_rxtx2.bmode_ovr",0x1);
			FieldWrite("ch2_rxtx3.dacfullscale",0x1);
			FieldWrite("ch1_rxtx3.dacfullscale",0x1);
			FieldWrite("ch0_rxtx3.dacfullscale",0x1);
			FieldWrite("ch2_rxtx3.loforcedon",0x1);
			FieldWrite("ch1_rxtx3.loforcedon",0x1);
			FieldWrite("ch0_rxtx3.loforcedon",0x1);
			FieldWrite("ch2_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch1_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch0_txrf1.lodiv2gforced",0x1);
			FieldWrite("ch0_rxtx2.synthon",0x1);
			FieldWrite("ch0_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch1_rxtx2.synthon",0x1);
			FieldWrite("ch1_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch2_rxtx2.synthon",0x1);
			FieldWrite("ch2_rxtx2.synthon_ovr",0x1);
			FieldWrite("ch2_top2.local_addacpwd",0x1);
			FieldWrite("ch1_top2.local_addacpwd",0x1);
			FieldWrite("ch0_top2.local_addacpwd",0x1);
			FieldWrite("ch2_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch1_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch0_pllclkmoda2.rst_warm_int_l",0x0);
			FieldWrite("ch2_top2.dacpwd",0x3);
			FieldWrite("ch1_top2.dacpwd",0x3);
			FieldWrite("ch0_top2.dacpwd",0x3);
			FieldWrite("ch2_top2.adcpwd",0x7);
			FieldWrite("ch1_top2.adcpwd",0x7);
			FieldWrite("ch0_top2.adcpwd",0x7);

			FieldWrite("ch2_txrf3.ob5g",0x3);
			FieldWrite("ch2_txrf3.d2b5g",0x3);
			FieldWrite("ch2_txrf2.d3b5g",0x3);
			FieldWrite("ch2_txrf2.d4b5g",0x3);
			FieldWrite("ch2_txrf3.ocas5g",0x0);
			FieldWrite("ch2_txrf3.d2cas5g",0x2);
			FieldWrite("ch2_txrf3.d3cas5g",0x2);
			FieldWrite("ch2_txrf3.d4cas5g",0x2);
			FieldWrite("ch2_rxtx1.mantxgain",0x1);
			FieldWrite("ch2_rxtx2.paon",0x1);
			FieldWrite("ch2_rxtx2.rxon",0x0);
			FieldWrite("ch2_rxtx2.txon_ovr",0x1);
			FieldWrite("ch2_rxtx2.paon_ovr",0x1);
			FieldWrite("ch2_rxtx2.rxon_ovr",0x1);
			FieldWrite("ch2_rxtx3.dacpwd",0x0);
			FieldWrite("ch2_rxtx3.dacpwd_ovr",0x1);
			FieldWrite("ch2_rxtx3.adcpwd",0x1);
			FieldWrite("ch2_rxtx3.adcpwd_ovr",0x1);
			FieldWrite("ch2_txrf1.pdlodiv2g",0x0);
			FieldWrite("ch2_bb2.switch_override",0x1);
			FieldWrite("ch2_bb1.force_i2v",0x1);
			FieldWrite("ch2_bb1.force_v2i",0x1);
			FieldWrite("ch2_bb1.force_osdac",0x1);
			FieldWrite("ch2_bb1.force_biquad",0x1);
			FieldWrite("ch2_bb1.force_notch",0x1);
			FieldWrite("ch2_bb1.force_loq",0x1);
			FieldWrite("ch2_bb1.enable_v2i",0x1);
			FieldWrite("ch2_bb1.enable_biquad",0x1);
			FieldWrite("ch2_bb1.enable_notch",0x1);
			FieldWrite("ch2_rxtx3.caltx",0x0);
			FieldWrite("ch2_rxtx3.calpa",0x0);
			FieldWrite("ch2_rxtx3.caltx_ovr",0x1);
			FieldWrite("ch2_rxtx3.calpa_ovr",0x1);
			FieldWrite("ch2_rxtx3.calfc",0x0);
			FieldWrite("ch2_rxtx3.calfc_ovr",0x1);

			MyRegisterWrite(0xa28c,0x22222);
			MyRegisterWrite(0xa288,0x222);
		}
	}

	return 0;
}


#undef REGR

static unsigned int REGR(unsigned int devNum, unsigned int address)
{
	unsigned int value;

	devNum=0;

	MyRegisterRead(address,&value);

	return value;
}

#define MAC_PCU_RX_FILTER (0x803c)
#define BROADCAST (0x4)
#define UNICAST (0x1)
#define MULTICAST (0x2)
#define PROMISCUOUS (0x20)

#define MAC_PCU_DIAG_SW (0x8048)

#define MAC_DMA_CR (0x0008)
#define RXE_LP (0x4)	

#define MAC_DMA_RX_QUEUE_LP_RXDP (0x0078)

#define MAC_QCU_TXDP (0x0800)

#define MAC_QCU_STATUS_RING_START (0x0830)
#define MAC_QCU_STATUS_RING_STOP  (0x0834)

#define MAC_DCU_QCUMASK (0x1000)
//
// disable receive
//
static int Ar9300ReceiveDisable()
{
    FieldWrite("MAC_DMA_CR.RXD",1);
	return 0;
}


//
// enable receive
//
static int  Ar9300ReceiveEnable()
{
	MyRegisterWrite(MAC_PCU_DIAG_SW,0);
    FieldWrite("MAC_DMA_CR.RXD",0);
    FieldWrite("MAC_DMA_CR.RXE_LP",1);
	return 0;
}


//
// set pointer to rx descriptor in shared memory
//
static int Ar9300ReceiveDescriptorPointer(unsigned int descriptor)
{
    MyRegisterWrite(MAC_DMA_RX_QUEUE_LP_RXDP, descriptor);

	return 0;
}


//
// set or clear receive filter bit
//
static int Ar9300ReceiveFilter(int on, unsigned int mask)
{
	unsigned int reg;

		reg=REGR(0, MAC_PCU_RX_FILTER);
//	    UserPrint("Ar9300ReceiveFilter(on,%x): MAC_PCU_RX_FILTER %x ->",mask,reg);
	if(on)
	{
		reg |= mask;
	}
	else
	{
		reg &= (~mask);
 	}
//    	UserPrint(" %x\n",reg);
        MyRegisterWrite(MAC_PCU_RX_FILTER, reg);

	return 0;
}


//
// set or clear receive of unicast packets
//
static int Ar9300ReceiveUnicast(int on)
{
	return Ar9300ReceiveFilter(on,UNICAST);
}


//
// set or clear receive of broadcast packets
//
static int Ar9300ReceiveBroadcast(int on)
{
	return Ar9300ReceiveFilter(on,BROADCAST);
}


//
// set or clear promiscuous mode
//
static int Ar9300ReceivePromiscuous(int on)
{
	return Ar9300ReceiveFilter(on,PROMISCUOUS);
}


//
// Set contention window.
// cwmin and cwmax are limited to values that are powers of two minus 1: 0, 1, 3, 7, ....
//
static int Ar9300TransmitContentionWindow(int dcu, int cwmin, int cwmax)
{
	static int allowed[]={0x0,0x1,0x3,0x7,0xf,0x1f,0x3f,0x7f,0xff,0x1ff,0x3ff};
	int nallowed;
	int it;
	char buffer[MBUFFER];
	//
	// default values
	//
//	UserPrint("Ar9300ContentionWindow(%d,%d,%d): ",dcu,cwmin,cwmax);
	nallowed=sizeof(allowed)/sizeof(int);

	if(cwmin<0)
	{
		cwmin=0xf;
	}
	else if(cwmin>=allowed[nallowed-1])
	{
		cwmin=allowed[nallowed-1];
	}
	else 
	{
		for(it=0; it<sizeof(allowed)/sizeof(int); it++)
		{
			if(cwmin>allowed[it])
			{
				cwmin=allowed[it];
				break;
			}
		}
	}

	if(cwmax<0)
	{
		cwmax=0x3ff;
	}
	else if(cwmax>=allowed[nallowed-1])
	{
		cwmax=allowed[nallowed-1];
	}
	else 
	{
		for(it=0; it<sizeof(allowed)/sizeof(int); it++)
		{
			if(cwmax>allowed[it])
			{
				cwmax=allowed[it];
				break;
			}
		}
	}

	if(cwmin>cwmax)
	{
		cwmin=cwmax;
	}

	SformatOutput(buffer,MBUFFER-1,"MAC_DCU_LCL_IFS[%d].CW_MIN",dcu);
    FieldWrite(buffer,0);

	SformatOutput(buffer,MBUFFER-1,"MAC_DCU_LCL_IFS[%d].CW_MAX",dcu);
    FieldWrite(buffer,0);

	return 0;
}

 
static int Ar9300TransmitOtherFastStuff(int on)
{
	if(on)
	{
		FieldWrite("MAC_DCU_GBL_IFS_SIFS.DURATION", 0x400);
	}
	return 0;
}

static int Ar9300ContinuousDataMode(int on)
{
	unsigned int reg;

	if(on)
	{
        // Put PCU and DMA in continuous data mode
        reg=REGR(0, 0x8054);
//		UserPrint("Ar5416ContinuousDataMode(%d) 0x8054: %x ->",on,reg);
	    reg |= 1;
//		UserPrint(" %x\n",reg);
		MyRegisterWrite(0x8054, reg);

        //disable encryption since packet has no header
		reg=REGR(0, 0x8048);
//		UserPrint("Ar5416ContinuousDataMode(%d) F2_DIAG_SW: %x ->",on,reg);
		reg |= 0x8;
//		UserPrint(" %x\n",reg);
        MyRegisterWrite(0x8048,  reg);
	}
	else
	{
        // Put PCU and DMA in continuous data mode
        reg=REGR(0, 0x8054);
//		UserPrint("Ar5416ContinuousDataMode(%d) 0x8054: %x ->",on,reg);
	    reg &= (~1);
//		UserPrint(" %x\n",reg);
		MyRegisterWrite(0x8054, reg);
	}
	return 0;
}

static int Ar9300TransmitRegularData()			// normal
{
	int dcu;

	for(dcu=0; dcu<MDCU; dcu++)
	{
        Ar9300TransmitContentionWindow(dcu, -1, -1);
	}
	Ar9300TransmitOtherFastStuff(1);
	Ar9300ContinuousDataMode(0);
	return 0;
}


static int Ar9300TransmitFrameData(int ifs)	// tx99
{
	int dcu;

	for(dcu=0; dcu<MDCU; dcu++)
	{
        Ar9300TransmitContentionWindow(dcu, 0, 0);
	}
	Ar9300TransmitOtherFastStuff(1);
	Ar9300ContinuousDataMode(0);
	return 0;
}


int Ar9300TransmitContinuousData()		// tx100
{
	int dcu;

	for(dcu=0; dcu<MDCU; dcu++)
	{
        Ar9300TransmitContentionWindow(dcu, -1, -1);
	}
	Ar9300TransmitOtherFastStuff(1);
	Ar9300ContinuousDataMode(1);

    return 0;
}


//
// set pointer to tx descriptor in shared memory
//
int Ar9300TransmitDescriptorPointer(int queue, unsigned int descriptor)
{
    MyRegisterWrite(MAC_QCU_TXDP + (4 * queue), descriptor);
	return 0;
}

//
// set pointer to tx descriptor in shared memory
//
int Ar9300TransmitDescriptorStatusPointer(unsigned int start, unsigned int stop)
{
    MyRegisterWrite(MAC_QCU_STATUS_RING_START, start);
    MyRegisterWrite(MAC_QCU_STATUS_RING_STOP, stop);
	return 0;
}

//
// map the qcu to the dcu and enable the clocks for both
//
int Ar9300TransmitQueueSetup(int qcu, int dcu)
{
	unsigned int reg;

	MyRegisterWrite(MAC_PCU_DIAG_SW,0);
    // 
	// program the queue
	//
    reg=REGR(0,MAC_DCU_QCUMASK + (4 * dcu ));
//	UserPrint("Ar9300QueueSetup(%d,%d): F2_D0_QCUMASK %x ->",qcu,dcu,reg);
	reg|=(1<<qcu);
//	UserPrint(" %x\n",reg);
    MyRegisterWrite(MAC_DCU_QCUMASK +  ( 4 * dcu ), reg);

	return 0;
}

static int Ar9300TransmitDisable(unsigned int mask)
{
    ar9300AbortTxDma(AH);
    return 0;
}

static int Ar9300TargetPowerApply(int frequency)
{
    A_UINT8 targetPowerValT2[ar9300RateSize];

	/* make sure forced gain is not set - HAL function will not do this */
    FieldWrite("force_dac_gain", 0);
	FieldWrite("force_tx_gain", 0);
	
	ar9300SetTargetPowerFromEeprom(AH, (short)frequency, targetPowerValT2);
	//
	// Write target power array to registers
	//
	ar9300TransmitPowerRegWrite(AH, targetPowerValT2);

   return 0;
}


//set the registers for the selected rx chain mask
void Ar9300RxChainSet(int rxChain)
{
    if(rxChain == 0x5) {
        FieldWrite("BB_analog_swap.swap_alt_chn", 0x1); 
    }
 
    FieldWrite("BB_multichain_enable.rx_chain_mask", rxChain & 0x7);
    FieldWrite("BB_cal_chain_mask.cal_chain_mask", rxChain & 0x7);
    return;
}

//
//enable deaf mode
//
int Ar9300Deaf(int deaf) 
{
    //
    //If not currently in deaf mode Store off the existing field values so that can go back to undeaf mode
    //
    if (deafMode == 0) {
        FieldRead("BB_cca_b0.cf_thresh62", (unsigned int *)&undeafThresh62);
        FieldRead("BB_ext_chan_pwr_thr_1.thresh62_ext", (unsigned int *) &undeafThresh62Ext);
        FieldRead("BB_test_controls.force_agc_clear", (unsigned int *) &undeafForceAgcClear);
        FieldRead("BB_timing_control_5.cycpwr_thr1", (unsigned int *) &undeafCycpwrThr1);
        FieldRead("BB_ext_chan_pwr_thr_2_b0.cycpwr_thr1_ext", (unsigned int *) &undeafCycpwrThr1Ext);
        FieldRead("BB_timing_control_5.rssi_thr1a", (unsigned int *) &undeafRssiThr1a);
    }

    if(deaf) {
        FieldWrite("BB_cca_b0.cf_thresh62", 0x7f);
        FieldWrite("BB_ext_chan_pwr_thr_1.thresh62_ext", 0x7f);
        FieldWrite("BB_test_controls.force_agc_clear", 1);
        FieldWrite("BB_timing_control_5.cycpwr_thr1", 0x7f);
        FieldWrite("BB_ext_chan_pwr_thr_2_b0.cycpwr_thr1_ext", 0x7f);
        FieldWrite("BB_timing_control_5.rssi_thr1a", 0x7f);
        deafMode=1;
    } else {
        FieldWrite("BB_cca_b0.cf_thresh62", undeafThresh62);
        FieldWrite("BB_ext_chan_pwr_thr_1.thresh62_ext", undeafThresh62Ext);
        FieldWrite("BB_test_controls.force_agc_clear", undeafForceAgcClear);
        FieldWrite("BB_timing_control_5.cycpwr_thr1", undeafCycpwrThr1);
        FieldWrite("BB_ext_chan_pwr_thr_2_b0.cycpwr_thr1_ext", undeafCycpwrThr1Ext);
        FieldWrite("BB_timing_control_5.rssi_thr1a", undeafRssiThr1a);
        deafMode=0;
    }
    return 0;
}

//
// clear all device control function pointers and set to default behavior
//
void Ar9300DeviceSelect()
{
	DeviceFunctionReset();
    //
	// read/write memory
	//
	DeviceMemoryReadFunction(MyMemoryRead);
	DeviceMemoryWriteFunction(MyMemoryWrite);
	//
	// read/write registers
	//
	DeviceRegisterReadFunction(MyRegisterRead);
	DeviceRegisterWriteFunction(MyRegisterWrite);

    DeviceEepromReadFunction(MyAr9300EepromRead);
	DeviceEepromWriteFunction(Ar9300EepromWrite);

	DeviceOtpReadFunction(Ar9300OtpRead);
	DeviceOtpWriteFunction(Ar9300OtpWrite);

    DeviceBssIdSetFunction(Ar9300BssIdSet);
    DeviceStationIdSetFunction(Ar9300StationIdSet);

    DeviceReceiveDescriptorPointerFunction(Ar9300ReceiveDescriptorPointer);
    DeviceReceiveUnicastFunction(Ar9300ReceiveUnicast);
    DeviceReceiveBroadcastFunction(Ar9300ReceiveBroadcast);
    DeviceReceivePromiscuousFunction(Ar9300ReceivePromiscuous);
    DeviceReceiveEnableFunction(Ar9300ReceiveEnable);
    DeviceReceiveDisableFunction(Ar9300ReceiveDisable);

    DeviceTransmitCarrierFunction(Ar9300TransmitCarrier);
	
    DeviceTransmitDescriptorStatusPointerFunction(Ar9300TransmitDescriptorStatusPointer);
    DeviceTransmitDescriptorPointerFunction(Ar9300TransmitDescriptorPointer);
    DeviceTransmitQueueSetupFunction(Ar9300TransmitQueueSetup);
    DeviceTransmitRegularDataFunction(Ar9300TransmitRegularData);
    DeviceTransmitFrameDataFunction(Ar9300TransmitFrameData);
#ifdef UNUSED
    DeviceTransmitRetryLimitFunction(Ar9300TransmitRetryLimit);
    DeviceTransmitEnableFunction(Ar9300TransmitEnable);
#endif
    DeviceTransmitDisableFunction(Ar9300TransmitDisable);

    DeviceTransmitContinuousDataFunction(Ar9300TransmitContinuousData);
	DeviceTransmitGainReadFunction(Ar9300TransmitGainRead);
	DeviceTransmitGainWriteFunction(Ar9300TransmitGainWrite);
	DeviceTransmitGainSetFunction(Ar9300TransmitGainSet);
	DeviceTransmitPowerSetFunction(Ar9300TransmitPowerSet);
	DeviceTargetPowerApplyFunction(Ar9300TargetPowerApply);
    DeviceTargetPowerGetFunction(Ar9300EepromGetTargetPwr);

//    DeviceConfigurationRestoreFunction(Ar9300EepromRestore);
    DeviceConfigurationSaveFunction(Ar9300EepromSave);

    DeviceCalibrationPierSetFunction(ar9300EepromUpdateCalPier);
    DevicePowerControlOverrideFunction(nartAr9300PowerControlOverride);

	DeviceTemperatureGetFunction(Ar9300TemperatureGet);
	DeviceVoltageGetFunction(Ar9300VoltageGet);

//    DeviceTargetPowerSetFunction(/* arguments????*/)
//    DeviceTargetPowerApplyFunction(void)

    DeviceReceiveDeafModeFunction(Ar9300Deaf);
}


