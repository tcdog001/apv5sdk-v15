#include <stdio.h>
#include <string.h>
//
// hal header files
//
#include "ah.h"
#include "ah_internal.h"
#include "ar9300.h"
#include "ar9300eep.h"


#include "wlantype.h"

#include "ParameterConfigDef.h"

#include "mEepStruct9300.h"
#include "Ar9300PcieConfig.h"
#include "Ar9300EepromStructSet.h"
#include "ConfigurationStatus.h"


int ConfigurationInit()
{
	return Ar9300pcieAddressValueDataInit();
}
//
// Set the mac address. Return 0 on success.
//
int ConfigurationMacAddressGet(unsigned char mac[6])
{
    return Ar9300MacAdressGet(mac);
}


//
// Set the customer data field. Return 0 on success.
//
int ConfigurationCustomerDataGet(unsigned char *data, int max)
{
    return Ar9300CustomerDataGet(data, 20);  // TO DO
}

//
// Get the device id field. Return 0 on success.
//
extern int ConfigurationDeviceIdGet(int *id)
{
	A_UINT16 iID;
	int status;
	status = Ar9300deviceIDGet(&iID);
	*id = iID;
    return status; 
}


//
// Get the device id field. Return 0 on success.
//
extern int ConfigurationSubSystemIdGet(int *id)
{
	A_UINT16 SSID;
	int status;
	status = Ar9300SSIDGet(&SSID);
	*id = SSID;
    return status; 
}


//
// Get the device id field. Return 0 on success.
//
extern int ConfigurationVendorIdGet(int *id)
{
	A_UINT16 iID;
	int status;
	status = Ar9300vendorGet(&iID);
	*id = iID;
    return status; 
}


//
// Get the device id field. Return 0 on success.
//
extern int ConfigurationSubVendorIdGet(int *id)
{
	A_UINT16 iID;
	int status;
	status = Ar9300SubVendorGet(&iID);
	*id = iID;
    return status; 
}

//
// Get the device id field. Return 0 on success.
//
A_INT32 ConfigurationpcieAddressValueDataGet(char *sValue, int address)
// address is A_UINT16
{
	A_UINT32 data;
	A_INT32 status=VALUE_OK;

	if(address<0 || address>(int)(0xffff)) {
		sprintf(sValue, "address supposed to be a UINT16, you entered 0x%x", address);		
		status = ERR_VALUE_BAD;
	} else {
		status = Ar9300pcieAddressValueDataGet(address, &data);
		if (status==VALUE_OK)
			sprintf(sValue, "0x%08x",data);
	}
    return status; 
}
/*
A_INT32 ConfigurationpcieAddressValueDataOfNumGet(int num, char *sValue)
// address is A_UINT16
{
	A_UINT16 address;
	A_UINT32 data;

	int status;
	status =  Ar9300pcieAddressValueDataOfNumGet(num, &address, &data);
	sprintf(sValue, "addr:%x, data:%x", address, data);

	return status;
} */

A_INT32 ConfigurationpcieAddressValueDataOfNumGet(int num, unsigned int *value)
// address is A_UINT16
{
	A_UINT16 address;
	A_UINT32 data;

	int status;
	status =  Ar9300pcieAddressValueDataOfNumGet(num, &address, &data);
	value[0] = address;
	value[1] = data;
	return status;
}

A_INT32 ConfigPCIeOnBoard(int iItem, unsigned int *value)
{
	int status;
	A_UINT16 address;
	A_UINT32 data;
	status =  Ar9300ConfigPCIeOnBoard(iItem, &address, &data);
	value[0] = address;
	value[1] = data;
	return status;
}

