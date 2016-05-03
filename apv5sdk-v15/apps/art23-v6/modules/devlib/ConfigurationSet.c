

#include <stdio.h>

//
// hal header files
//
#include "ah.h"
#include "ah_internal.h"
#include "ar9300.h"
#include "ar9300eep.h"


#include "wlantype.h"

#include "UserPrint.h"
#include "ParameterConfigDef.h"

#include "mEepStruct9300.h"
#include "Ar9300PcieConfig.h"
#include "Ar9300EepromStructSet.h"
#include "ConfigurationStatus.h"
#include "ConfigurationSet.h"

//
// Set the mac address. Return 0 on success.
//
int ConfigurationMacAddressSet(unsigned char mac[6])
{
    UserPrint("mac address = %02x:%02x:%02x:%02x:%02x:%02x\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    return Ar9300MacAdressSet(mac);
}


//
// Set the customer data field. Return 0 on success.
//
int ConfigurationCustomerDataSet(unsigned char *data, int len)
{
    UserPrint("customer data = %s\n",data);
    return Ar9300CustomerDataSet(data, len);
}

//
// Set the device id field. Return 0 on success.
//
// set config=HexValue; a=hexAddress;
int ConfigurationConfigSet(A_UINT16 address, A_UINT32 data)
{
	int status=Ar9300pcieAddressValueDataSet(address, data);
	if (status==VALUE_SAME || status==VALUE_UPDATED || status==VALUE_NEW)
		status = VALUE_OK;
    return status;
}

//
// Set the device id field. Return 0 on success.
//
int ConfigurationDeviceIdSet(int id)
{
	int status=Ar9300deviceIDSet((unsigned short) id);
	if (status==VALUE_SAME || status==VALUE_UPDATED)
		status = VALUE_OK;
    return status;
}


//
// Set the device id field. Return 0 on success.
//
int ConfigurationSubSystemIdSet(int id)
{
	int status=Ar9300SSIDSet((unsigned short)id);
	if (status==VALUE_SAME || status==VALUE_UPDATED)
		status = VALUE_OK;
    return status;
}


//
// Set the device id field. Return 0 on success.
//
int ConfigurationVendorIdSet(int id)
{
	int status=Ar9300vendorSet((unsigned short)id);
	if (status==VALUE_SAME || status==VALUE_UPDATED)
		status = VALUE_OK;
    return status;
}


//
// Set the device id field. Return 0 on success.
//
int ConfigurationSubVendorIdSet(int id)
{
	int status=Ar9300SubVendorSet((unsigned short)id);
	if (status==VALUE_SAME || status==VALUE_UPDATED)
		status = VALUE_OK;
    return status;
}


//
// report how much memory is used by the configuration space
//
int ConfigurationUsed(void)
{
    return Ar9300ConfigSpaceUsed();
}


//
// Commit PCIE config space changes to the card
//
int ConfigurationCommit(void)
{
    return Ar9300ConfigSpaceCommit();
}


int ConfigurationRemove(void)
{
    return Ar9300pcieAddressValueDataInit();
}
