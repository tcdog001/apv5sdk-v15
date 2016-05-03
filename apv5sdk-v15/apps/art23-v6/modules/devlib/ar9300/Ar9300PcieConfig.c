#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "wlantype.h"

#include "Ar9300Device.h"
#include "Ar9300PcieConfig.h"
#include "ConfigurationStatus.h"
#include "ah_osdep.h"
#include "opt_ah.h"

#include "ah.h"
#include "ah_internal.h"
#include "ar9300eep.h"
#include "ar9300reg.h"
#include "ar9300.h"

#include "Ar9300EepromSave.h"
#include "Ar9300EepromStructSet.h"
#include "ar9300EepromPrint.h"

#include "UserPrint.h"
#include "ErrorPrint.h"
#include "EepromError.h"

// 
// this is the hal pointer, 
// returned by ath_hal_attach
// used as the first argument by most (all?) HAL routines
//
extern struct ath_hal *AH;

//static PCIE_CONFIG_STRUCT pcieAddressValueData[20];
static PCIE_CONFIG_STRUCT pcieAddressValueData[MAX_pcieAddressValueData];
unsigned char pcieByteArray[6*MAX_pcieAddressValueData+2];
static int num_pcieAddressValueData;
static int num_pcieAddressValueDataFromCard;

A_INT32 Ar9300SSIDSet(A_UINT16 SSID);
A_INT32 Ar9300SubVendorSet(A_UINT16 subVendorID);
A_INT32 Ar9300deviceIDSet(A_UINT16 deviceID);
A_INT32 Ar9300vendorSet(A_UINT16 vendorID);
A_INT32 Ar9300pcieAddressValueDataSet( A_UINT16 address, A_UINT32 data);
A_INT32 Ar9300pcieAddressValueDataAdd( A_UINT16 address, A_UINT16 dataLow, A_UINT16 dataHigh);

#define MPCIE 100
#define PCIE_OTP_BASE 32
#define PCIE_OTP_TOP 200

static int PcieTop;

/* DEBUG TEST
#define Ar9300OtpWrite(ADDRESS,BUFFER,LENGTH) Ar9300EepromWrite(ADDRESS+0x30,BUFFER,LENGTH)
#define Ar9300OtpRead(ADDRESS,BUFFER,LENGTH) MyAr9300EepromRead(ADDRESS+0x30,BUFFER,LENGTH)
*/

static int WriteIt(int address, unsigned int value)
{
	unsigned char byte[4];

	byte[0]=(value&0xff);
	byte[1]=((value>>8)&0xff);
	byte[2]=((value>>16)&0xff);
	byte[3]=((value>>24)&0xff);
	return Ar9300OtpWrite(address,byte,4);
}

static int ReadIt(int address, unsigned int *value)
{
	unsigned char byte[4];
	int error;

	error=Ar9300OtpRead(address,byte,4);
	if(error==0)
	{
	    *value=byte[3];
		*value=((*value)<<8)|byte[2];
		*value=((*value)<<8)|byte[1];
		*value=((*value)<<8)|byte[0];
	}
	return error;
}


A_INT32 Ar9300pcieAddressValueDataInitEeprom(void)
// read eep from card, 
// if it's blank, wirte Atheros header, SSID... and hardcoded reg addr and value into eep structure
// if it's not blank, read the eep context from card, compare with atheros default setting,
// if not the same update structure data to the default, will write them into eep when runs commit
{
	int status = VALUE_OK;	
	int i;
	A_UINT addr;

	A_UINT8 pcieConfigHeader[6] = {0x5a, 0xa5, 0, 0, 3, 0};
	A_UINT8 pcieConfigHeader0[6], pcieConfigDataSet[6];

	// first read eep back to see if it's a blank card
	if (MyAr9300EepromRead(0, pcieConfigHeader0, 6))	
		return ERR_EEP_READ;	// bad eep reading

	num_pcieAddressValueDataFromCard = 0;
	num_pcieAddressValueData = 0;
	if (!(pcieConfigHeader0[0] == 0	||			// for blank EEPROM 
		  pcieConfigHeader0[0] == 0xFF)) {	// for blank OTP
		// not a blank eep card, read back card's eep contents
		for (i=0; i<6; i++) {
			if (pcieConfigHeader0[i] != pcieConfigHeader[i])
				return ERR_VALUE_BAD;		// eep header is not match atheros standard.
		}
		// from last 2 byte header to figure out the eep start address.
		addr = ((A_UINT)(pcieConfigHeader[4]) + (A_UINT)(pcieConfigHeader[5] <<8)) * 2;		
		while(1) {
			if (MyAr9300EepromRead(addr, pcieConfigDataSet, 6))	
				return -1;	// bad eep reading	
			if ( (pcieConfigDataSet[0]==0 && pcieConfigDataSet[1]==0) ||	// for blank EEPROM
				  (pcieConfigDataSet[0]==0xFF && pcieConfigDataSet[1]==0xFF)) {	// for blank OTP
				break;	// found the end of eep data on card
			} else {
				addr+=6;
				pcieAddressValueData[num_pcieAddressValueData].address = (A_UINT16)(pcieConfigDataSet[0]) + (A_UINT16)(pcieConfigDataSet[1] <<8);
				pcieAddressValueData[num_pcieAddressValueData].dataLow = (A_UINT16)(pcieConfigDataSet[2]) + (A_UINT16)(pcieConfigDataSet[3] <<8);
				pcieAddressValueData[num_pcieAddressValueData].dataHigh = (A_UINT16)(pcieConfigDataSet[4]) + (A_UINT16)(pcieConfigDataSet[5] <<8);
				num_pcieAddressValueData++;
				if (num_pcieAddressValueData==MAX_pcieAddressValueData)
					return ERR_MAX_REACHED;	
			}
		}
	}
	return VALUE_OK;
}


A_INT32 Ar9300pcieAddressValueDataInitOtp(void)
{
	int address;
	unsigned int eregister,evalue;

	num_pcieAddressValueDataFromCard = 0;
	num_pcieAddressValueData = 0;

	for(address=PCIE_OTP_BASE; address<PCIE_OTP_TOP; address+=8)
	{
		ReadIt(address,&eregister);
		//
		// are we done?
		//
		if(eregister==0 /*|| eregister==0xffffffff*/)			// DEBUG TEST 
		{
//			UserPrint("end of exisiting at %x out=%d\n",address,out);
			break;
		}
		if((eregister&0x3)==0)
		{
			ReadIt(address+4,&evalue);
//				UserPrint("old=%08x new=%08x",evalue,nvalue[out]);
			pcieAddressValueData[num_pcieAddressValueData].address = (A_UINT16)(eregister&0xffff);
			pcieAddressValueData[num_pcieAddressValueData].dataLow = (A_UINT16)(evalue&0xffff);
			pcieAddressValueData[num_pcieAddressValueData].dataHigh = (A_UINT16)((evalue>>16)&0xffff);
			num_pcieAddressValueData++;
			if (num_pcieAddressValueData==MAX_pcieAddressValueData)
				return ERR_MAX_REACHED;	
		}
	}
	return VALUE_OK;
}


A_INT32 Ar9300pcieAddressValueDataInitDefault(void)
{
	int status = VALUE_OK;	

	num_pcieAddressValueDataFromCard = num_pcieAddressValueData;
	// init to Atheros's hard coded EEPROM values
	//status = Ar9300SSIDSet(0x3112);
	//status = Ar9300SubVendorSet(0x168c);
	if(AR_SREV_AR9580(AH)) {
		status = Ar9300deviceIDSet(0x33);
	} else {
		status = Ar9300deviceIDSet(0x30);
	}
	status = Ar9300vendorSet(0x168c);
	if (num_pcieAddressValueData==0)
		num_pcieAddressValueData = 2;	// first 2 is for ssid, vid...
	if (Ar9300pcieAddressValueDataAdd( 0x5008, 0x0001, 0x0280)== ERR_MAX_REACHED)
		return ERR_MAX_REACHED;
	if (Ar9300pcieAddressValueDataAdd( 0x407c, 0x0001, 0x0000)== ERR_MAX_REACHED)
		return ERR_MAX_REACHED;
	if (Ar9300pcieAddressValueDataAdd( 0x4004, 0x021b, 0x0102)== ERR_MAX_REACHED)
		return ERR_MAX_REACHED;
	//initialize pcie serdes config
	if(AR_SREV_AR9580(AH)) {
		if (Ar9300pcieAddressValueDataAdd( 0x4040, 0x2e5e, 0x0831)== ERR_MAX_REACHED)
			return ERR_MAX_REACHED;
	} else {
		if (Ar9300pcieAddressValueDataAdd( 0x4040, 0x2e5e, 0x0821)== ERR_MAX_REACHED)
		        return ERR_MAX_REACHED;
	}
	if (Ar9300pcieAddressValueDataAdd( 0x4040, 0x003b, 0x0008)== ERR_MAX_REACHED)
            return ERR_MAX_REACHED;
	if (Ar9300pcieAddressValueDataAdd( 0x4044, 0x0000, 0x0000)== ERR_MAX_REACHED)
            return ERR_MAX_REACHED;            
	//end pcie serdes config

    if (Ar9300pcieAddressValueDataAdd( 0x570C, 0x3f01, 0x173f)== ERR_MAX_REACHED)
            return ERR_MAX_REACHED;
	return VALUE_OK;
}


A_INT32 Ar9300pcieAddressValueDataInit(void)
{
	int error;

	error=VALUE_OK;
	switch(ar9300CalibrationDataGet(AH))
	{
		case CalibrationDataEeprom:
			error=Ar9300pcieAddressValueDataInitEeprom();
			break;
		case CalibrationDataFlash:
		case CalibrationDataOtp:
			error=Ar9300pcieAddressValueDataInitOtp();
			break;
	}
	if(error!=VALUE_OK || num_pcieAddressValueData==0)
	{
		error=Ar9300pcieAddressValueDataInitDefault();
	}
	return error;
}



#ifdef _UNUSED
A_INT32 Ar9300pcieAddressValueDataUpdate( A_UINT16 address, A_UINT16 dataLow, A_UINT16 dataHigh)
{
	int i;
	int status = VALUE_NEW;		
	
	for (i=0; i< num_pcieAddressValueData; i++) {
		if (pcieAddressValueData[i].address == address) {
			if ( pcieAddressValueData[i].dataHigh == dataHigh &&
				 pcieAddressValueData[i].dataLow == dataLow )
				return VALUE_SAME;		
			else {
				pcieAddressValueData[i].dataHigh = dataHigh;
				pcieAddressValueData[i].dataLow = dataLow;
				return VALUE_UPDATED;		
			}
		}
	}
	if (status == VALUE_NEW) {
		if (num_pcieAddressValueData>=MAX_pcieAddressValueData)
				return ERR_MAX_REACHED;		
		else {
			pcieAddressValueData[num_pcieAddressValueData].address = address;
			pcieAddressValueData[num_pcieAddressValueData].dataLow = dataLow;
			pcieAddressValueData[num_pcieAddressValueData].dataHigh = dataHigh;
			num_pcieAddressValueData++;
		}
	}
    return status;
}
#endif

A_INT32 Ar9300pcieAddressValueDataAdd( A_UINT16 address, A_UINT16 dataLow, A_UINT16 dataHigh)
{
	int i;
	int status = VALUE_NEW;		
	
	for (i=0; i< num_pcieAddressValueData; i++) {
		if (pcieAddressValueData[i].address == address) {
			if ( pcieAddressValueData[i].dataHigh == dataHigh &&
				 pcieAddressValueData[i].dataLow == dataLow )
				return VALUE_SAME;		
		}
	}
	if (status == VALUE_NEW) {
		if (num_pcieAddressValueData>=MAX_pcieAddressValueData)
				return ERR_MAX_REACHED;		
		else {
			pcieAddressValueData[num_pcieAddressValueData].address = address;
			pcieAddressValueData[num_pcieAddressValueData].dataLow = dataLow;
			pcieAddressValueData[num_pcieAddressValueData].dataHigh = dataHigh;
			num_pcieAddressValueData++;
		}
	}
    return status;
}


A_INT32 Ar9300pcieAddressValueDataSet( A_UINT16 address, A_UINT32 data)
{
	A_UINT16 dataLow, dataHigh;
	dataLow  = (A_UINT16)(data & 0xFFFF);
	dataHigh = (A_UINT16)(data >> 16);
    return Ar9300pcieAddressValueDataAdd(address, dataLow, dataHigh);
}

A_INT32 Ar9300pcieAddressValueDataGet( A_UINT16 address, A_UINT32 *data)
{
	int i;
	int status = ERR_NOT_FOUND;	
	for (i=0; i< num_pcieAddressValueData; i++) {
		if (pcieAddressValueData[i].address == address) {
			*data = (A_UINT32) pcieAddressValueData[i].dataLow +
					(A_UINT32) (pcieAddressValueData[i].dataHigh << 16);
			status = VALUE_OK;
			break;
		}
	}
    return status;
}


A_INT32 Ar9300pcieAddressValueDataOfNumGet( int num, A_UINT16 *address, A_UINT32 *data)
{
	if (num <0 || num>=num_pcieAddressValueData)
		return ERR_RETURN;
	*address = pcieAddressValueData[num].address;
	*data = (A_UINT32) pcieAddressValueData[num].dataLow +
			(A_UINT32) (pcieAddressValueData[num].dataHigh << 16);
	return VALUE_OK;
}

A_INT32 Ar9300pcieAddressValueDataRemove( A_UINT16 address)
{
	int i, iFound=ERR_NOT_FOUND;
	
	for (i=0; i< num_pcieAddressValueData; i++) {
		if (pcieAddressValueData[i].address == address) 
			iFound = i;
	}
	if (iFound!=ERR_NOT_FOUND) {
		for (i=iFound; i<num_pcieAddressValueData-1; i++) {
			pcieAddressValueData[i].address = pcieAddressValueData[i+1].address;
			pcieAddressValueData[i].dataLow = pcieAddressValueData[i+1].dataLow;
			pcieAddressValueData[i].dataHigh = pcieAddressValueData[i+1].dataHigh;
		}
		pcieAddressValueData[num_pcieAddressValueData].address = 0xFFFF;
		pcieAddressValueData[num_pcieAddressValueData].dataLow = 0xFFFF;
		pcieAddressValueData[num_pcieAddressValueData].dataHigh = 0xFFFF;
		num_pcieAddressValueData--;
	}
    return iFound;
}

A_INT32 Ar9300SSIDSet(A_UINT16 SSID) 
{
    //Need to make this smarter, look for address in list, if don't find it, add to next avail space
    pcieAddressValueData[1].address = PCI_EXPRESS_SUBSYS_VENDOR_DEVICE_ADDR;
	if (pcieAddressValueData[1].dataHigh == SSID)
		return VALUE_SAME;
	else {
		pcieAddressValueData[1].dataHigh = SSID;
		return VALUE_UPDATED;
	}
}

A_INT32 Ar9300SSIDGet(A_UINT16 *SSID) 
{
	if (pcieAddressValueData[1].address == PCI_EXPRESS_SUBSYS_VENDOR_DEVICE_ADDR) {
		*SSID = pcieAddressValueData[1].dataHigh;
		return VALUE_OK;
	} else if (pcieAddressValueData[1].address == 0	||			// for EEPROM
				pcieAddressValueData[1].address == 0xFFFF) {	// for OTP
		*SSID = pcieAddressValueData[1].dataHigh;
		return VALUE_NEW;
	}
	*SSID = 0xDEAD;
	return ERR_RETURN;
}

A_INT32 Ar9300SubVendorSet(A_UINT16 subVendorID) 
{
    //Need to make this smarter, look for address in list, if don't find it, add to next avail space
    pcieAddressValueData[1].address = PCI_EXPRESS_SUBSYS_VENDOR_DEVICE_ADDR;
	if (pcieAddressValueData[1].dataLow == subVendorID)
		return VALUE_SAME;
	else {
	    pcieAddressValueData[1].dataLow = subVendorID;
		return VALUE_UPDATED;
	}
}

A_INT32 Ar9300SubVendorGet(A_UINT16 *subVendorID) 
{
    //Need to make this smarter, look for address in list, if don't find it, add to next avail space
	if (pcieAddressValueData[1].address == PCI_EXPRESS_SUBSYS_VENDOR_DEVICE_ADDR) {
		*subVendorID = pcieAddressValueData[1].dataLow;
		return VALUE_OK;
	} else if (pcieAddressValueData[1].address == 0	||			// for EEPROM
				pcieAddressValueData[1].address == 0xFFFF) {	// for OTP
		*subVendorID = pcieAddressValueData[1].dataLow;
		return VALUE_NEW;
	}
	*subVendorID = 0xDEAD;
	return ERR_RETURN;
}


A_INT32 Ar9300deviceIDSet(A_UINT16 deviceID) 
{  
    //Need to make this smarter, look for address in list, if don't find it, add to next avail space
    pcieAddressValueData[0].address = PCI_EXPRESS_VENDOR_DEVICE_ADDR;
	if (pcieAddressValueData[0].dataHigh == deviceID)
		return VALUE_SAME;
	else {
	    pcieAddressValueData[0].dataHigh = deviceID;
		return VALUE_UPDATED;
	}
}

A_INT32 Ar9300deviceIDGet(A_UINT16 *deviceID) 
{  
    //Need to make this smarter, look for address in list, if don't find it, add to next avail space
	if (pcieAddressValueData[0].address == PCI_EXPRESS_VENDOR_DEVICE_ADDR) {
		*deviceID = pcieAddressValueData[0].dataHigh;
		return VALUE_OK;
	} else if (pcieAddressValueData[0].address == 0	||			// for EEPROM
				pcieAddressValueData[0].address == 0xFFFF) {	// for OTP
		*deviceID = pcieAddressValueData[0].dataHigh;
		return VALUE_NEW;
	}
	*deviceID = 0xDEAD;
	return ERR_RETURN;
}

A_INT32 Ar9300vendorSet(A_UINT16 vendorID)
{   
    //Need to make this smarter, look for address in list, if don't find it, add to next avail space
    pcieAddressValueData[0].address = PCI_EXPRESS_VENDOR_DEVICE_ADDR;
	if (pcieAddressValueData[0].dataLow == vendorID)
		return VALUE_SAME;
	else {
	    pcieAddressValueData[0].dataLow = vendorID;
		return VALUE_UPDATED;
	}
}

A_INT32 Ar9300vendorGet(A_UINT16 *vendorID)
{   
	//Need to make this smarter, look for address in list, if don't find it, add to next avail space
	if (pcieAddressValueData[0].address == PCI_EXPRESS_VENDOR_DEVICE_ADDR) {
		*vendorID = pcieAddressValueData[0].dataLow;
		return VALUE_OK;
	} else if (pcieAddressValueData[0].address == 0	||			// for EEPROM
				pcieAddressValueData[0].address == 0xFFFF) {	// for OTP
		*vendorID = pcieAddressValueData[0].dataLow;
		return VALUE_NEW;
	}
	*vendorID = 0xDEAD;
	return ERR_RETURN;
}

A_INT32 Ar9300ConfigPCIeOnBoard(int iItem, A_UINT16 *addr, A_UINT32 *data) 
{
	// blank card
	if (num_pcieAddressValueDataFromCard==0)
		return ERR_MAX_REACHED;

	if (iItem==0) {
		*addr = 0xa55a;
		*data = 0x00000003;
		return VALUE_OK;
	} else {
		if (iItem<=num_pcieAddressValueDataFromCard) {
			*addr = pcieAddressValueData[iItem-1].address;
			*data = pcieAddressValueData[iItem-1].dataLow + (pcieAddressValueData[iItem-1].dataHigh<<16);
		} else
			return ERR_MAX_REACHED;
	}
	return VALUE_OK;
}

A_INT32 Ar9300ConfigSpaceCommitEeprom() 
{
	int limit;
	A_UINT addr;
    int i;
    A_UINT8 pcieConfigHeader[6] = {0x5a, 0xa5, 0, 0, 3, 0};

	addr = ((A_UINT)(pcieConfigHeader[4]) + (A_UINT)(pcieConfigHeader[5] <<8)) * 2;		

	limit=Ar9300EepromUsed();
	if(limit<=addr+num_pcieAddressValueData*6+2)
	{
		ErrorPrint(PcieWontFit,addr+num_pcieAddressValueData*6+2,limit);
		return-1;
	}

    Ar9300EepromWrite(0, pcieConfigHeader, 6);

	// need tp add FFFF at the end of value for mark
	// [4] is the lsb for eep addr start, [5] is the msb for eep addr start
	if (num_pcieAddressValueData+1<MAX_pcieAddressValueData) {	// add 0xFFFF to mark as the end
		pcieAddressValueData[num_pcieAddressValueData].address =0xFFFF;
	}

    // Convert to byte array
    // Fill up pcie Byte Array with default values (0xff)
    for(i=0; i<(6*MAX_pcieAddressValueData+2); i++)
        pcieByteArray[i] = 0xff;
    for(i=0; i<num_pcieAddressValueData; i++)
    {
        pcieByteArray[6*i+0] = pcieAddressValueData[i].address & 0xff;
        pcieByteArray[6*i+1] = (pcieAddressValueData[i].address >> 8) & 0xff;
        pcieByteArray[6*i+2] = pcieAddressValueData[i].dataLow & 0xff;
        pcieByteArray[6*i+3] = (pcieAddressValueData[i].dataLow >> 8) & 0xff;
        pcieByteArray[6*i+4] = pcieAddressValueData[i].dataHigh & 0xff;
        pcieByteArray[6*i+5] = (pcieAddressValueData[i].dataHigh >> 8) & 0xff;
    }
	Ar9300EepromWrite(addr, pcieByteArray, num_pcieAddressValueData*6+2);
    
    return 0;
}

A_INT32 Ar9300ConfigSpaceCommitOtp() 
{
	int it;
	int address;
	unsigned int eregister,evalue;
	int nmany;
	unsigned int nregister[MPCIE],nvalue[MPCIE];
	unsigned int check;
	unsigned int invalidate=0xffff;
	unsigned int readit=0x4;
	int out;
	int limit;

	limit=Ar9300EepromUsed();
	limit-=8;		// becuase we do the check on the address field (4 bytes) and the value follows (another 4 bytes)
	limit-=4;		// to allow 4 bytes of zeros before calibration structure
	//
	// form new value array and check whether it is already in the existing array
	//
	nmany=num_pcieAddressValueData;
    for(it=0; it<nmany; it++)
    {
        nregister[it] = pcieAddressValueData[it].address;
		nvalue[it]=(pcieAddressValueData[it].dataLow&0xffff)|((pcieAddressValueData[it].dataHigh&0xffff)<<16);
    }
//	UserPrint("need to write %d pairs\n",nmany);
	//
	// read the existing pcie space
	// if the register is the same as the new one, check the value
	//     if equal, go on to the next
	//     if not equal, see if we can fix it
	//         if not invalidate existing, and go on to next exisitng pair
	// if not the same register, invalidate it and go on to the next exisiting
	//
	out=0;
	for(address=PCIE_OTP_BASE; address<=limit; address+=8)
	{
		ReadIt(address,&eregister);
		//
		// are we done?
		//
		if(eregister==0 /*|| eregister==0xffffffff*/)			// DEBUG TEST 
		{
//			UserPrint("end of exisiting at %x out=%d\n",address,out);
			break;
		}
		if((eregister&0x3)==0)
		{
			//
			// if no more new registers, then we invalidate all of the rest
			//
			if(out>=nmany)
			{
				WriteIt(address,invalidate);
				ReadIt(address+4,&check);
				if((check&0x3)==0)
				{
					//
					// otp error, reject chip
					//
					ErrorPrint(PcieVerify,address,check,invalidate);
					return -1;
				}
			}
			//
			// if registers are the same
			//
			else if(eregister==nregister[out])
			{
				ReadIt(address+4,&evalue);
//				UserPrint("old=%08x new=%08x",evalue,nvalue[out]);
				//
				// register same, value same
				//
				if(evalue==nvalue[out])
				{
//					UserPrint("register same,value same, address=%x out=%d",address,out);
					out++;
					continue;
				}
				else if((evalue&(~nvalue[out]))==0)
				{
//					UserPrint("register same,value fixable, address=%x out=%d old=%08x new=%08x",address,out,evalue,nvalue[out]);
					//
					// register same, value fixable
					//
					WriteIt(address,nvalue[out]);
					ReadIt(address,&check);
					if((check!=nvalue[out])==0)
					{
						WriteIt(address,invalidate);
						ReadIt(address,&check);
						if((check&0x3)==0)
						{
							//
							// otp error, reject chip
							//
							ErrorPrint(PcieVerify,address,check,invalidate);
							return -1;
						}
					}
				}
				else
				{
//					UserPrint("register same,value different, address=%x out=%d",address,out);
					//
					// register same, value bad
					//
					WriteIt(address,invalidate);
					ReadIt(address,&check);
					if((check&0x3)==0)
					{
						//
						// otp error, reject chip
						//
						ErrorPrint(PcieVerify,address,check,invalidate);
						return -1;
					}
				}
			}
			//
			// registers are different
			//
			else
			{
//				UserPrint("register different, address=%x out=%d",address,out);
				//
				// register same, value bad
				//
				WriteIt(address,invalidate);
				ReadIt(address,&check);
				if((check&0x3)==0)
				{
					//
					// otp error, reject chip
					//
					ErrorPrint(PcieVerify,address,check,invalidate);
					return -1;
				}
			}
		}
	}
	//
	// if any new pairs are left over, add them to the end
	//
	for( ; out<nmany; out++)
	{
		if(address>limit)
		{
			//
			// otp error, reject chip
			//
			ErrorPrint(PcieWontFit,address,limit);
			return -1;
		}
		//
		// write and verify register address
		//
//		UserPrint("new, address=%x out=%d",address,out);
		WriteIt(address,nregister[out]);
		ReadIt(address,&check);
		if(check!=nregister[out])
		{
			WriteIt(address,invalidate);
			ReadIt(address,&check);
			if((check&0x3)==0)
			{
				//
				// otp error, reject chip
				//
				ErrorPrint(PcieVerify,address,check,invalidate);
				return -1;
			}
			else
			{
				//
				// try again
				//
				out--;
			}
		}
		//
		// write and verify register value
		//
		WriteIt(address+4,nvalue[out]);
		ReadIt(address+4,&check);
		if(check!=nvalue[out])
		{
			WriteIt(address,invalidate);
			ReadIt(address,&check);
			if((check&0x3)==0)
			{
				//
				// otp error, reject chip
				//
				ErrorPrint(PcieVerify,address,check,invalidate);
				return -1;
			}
			else
			{
				//
				// try again
				//
				out--;
			}
		}

		address+=8;
	}
	//
	// check that next word is all zero
	//
//	UserPrint("check, address=%x",address);
	ReadIt(address,&check);
	if(check!=0 /*&& check!=0xffffffff*/)				// DEBUG TEST
	{
		//
		// otp error, reject chip
		//
		ErrorPrint(PcieVerify,address,check,0);
		return -1;
	}
	//
	// Save highest address
	//
	PcieTop=address+8;
	//
	// and we have to write bit 2 of byte 0 to get the chip to look at the initialization data
	//
	ReadIt(0, &check);
	if((check&readit)==0)
	{
 		WriteIt(0, readit);
		ReadIt(0, &check);
		//
		// just check the bit we tried to write, we don't care about other bits
		//
		if(readit!=(check&readit))
		{
			//
			// otp error, reject chip
			//
			ErrorPrint(PcieVerify,address,check,readit);
			return -1;
		}
	}
   
    return PcieTop;
}

A_INT32 Ar9300ConfigSpaceCommit() 
{
	int calmem;

	calmem=Ar9300EepromSaveMemory();
	switch(calmem)
	{
		case CalibrationDataEeprom:
			return Ar9300ConfigSpaceCommitEeprom();
		case CalibrationDataFlash:
		case CalibrationDataOtp:
			return Ar9300ConfigSpaceCommitOtp();
	}
	return -1;
}


A_INT32 Ar9300ConfigSpaceUsedEeprom(void)
{
	A_UINT addr;

	A_UINT8 word[6] = {0x5a, 0xa5, 0, 0, 3, 0};

	// first read eep back to see if it's a blank card
	if (MyAr9300EepromRead(0, word, 6) || word[0]==0xff)
	{
		return 0;							// nothing on card, return minimum size
	}

	// from last 2 byte header to figure out the eep start address.
	for(addr = ((A_UINT)(word[4]) + (A_UINT)(word[5] <<8)) * 2;	addr<500; addr+=6)	
	{
		if (MyAr9300EepromRead(addr, word, 6))
		{
			break;	// bad eep reading
		}
		if (word[0]==0xFF && word[1]==0xFF) 
		{	
			break;	// found the end of eep data on card
		} 
	}

	return addr;
}


A_INT32 Ar9300ConfigSpaceUsedOtp(void)
{
	int address;
	unsigned int eregister,evalue;

	for(address=PCIE_OTP_BASE; address<PCIE_OTP_TOP; address+=8)
	{
		ReadIt(address,&eregister);
		//
		// are we done?
		//
		if(eregister==0 /*|| eregister==0xffffffff*/)			// DEBUG TEST 
		{
//			UserPrint("end of exisiting at %x out=%d\n",address,out);
			break;
		}
	}
	PcieTop=address;
	return PcieTop;
}


A_INT32 Ar9300ConfigSpaceUsed() 
{
	switch(ar9300CalibrationDataGet(AH))
	{
		case CalibrationDataEeprom:
			return Ar9300ConfigSpaceUsedEeprom();
		case CalibrationDataFlash:
		case CalibrationDataOtp:
			return Ar9300ConfigSpaceUsedOtp();
	}
	return -1;
}