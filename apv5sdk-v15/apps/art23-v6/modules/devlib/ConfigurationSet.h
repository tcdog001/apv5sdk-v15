#include "ParameterConfigDef.h"
//
// Set the mac address. Return 0 on success.
//
extern int ConfigurationMacAddressSet(unsigned char mac[6]);
//
// Set the customer data field. Return 0 on success.
//
extern int ConfigurationCustomerDataSet(unsigned char *data, int len);
//
// Set the device id field. Return 0 on success.
//
extern int ConfigurationDeviceIdSet(int id);
//
// Set the device id field. Return 0 on success.
//
extern int ConfigurationConfigSet(A_UINT16 address, A_UINT32 data);
//
// Set the device id field. Return 0 on success.
//
extern int ConfigurationSubSystemIdSet(int id);
//
// Set the device id field. Return 0 on success.
//
extern int ConfigurationVendorIdSet(int id);
//
// Set the device id field. Return 0 on success.
//
extern int ConfigurationSubVendorIdSet(int id);

//
// Commit PCIE config space changes to the card
//
extern int ConfigurationCommit(void);

//
// report how much memory is used by the configuration space
//
extern int ConfigurationUsed(void);



