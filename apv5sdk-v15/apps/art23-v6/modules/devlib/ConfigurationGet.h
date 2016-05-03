extern int ConfigurationInit();
//
// Get the mac address. Return 0 on success.
//
extern int ConfigurationMacAddressGet(unsigned char mac[6]);


//
// Get the customer data field. Return 0 on success.
//
extern int ConfigurationCustomerDataGet(char *data, int max);

//
// Get the device id field. Return 0 on success.
//
extern int ConfigurationDeviceIdGet(int *id);


//
// Get the device id field. Return 0 on success.
//
extern int ConfigurationSubSystemIdGet(int *id);


//
// Get the device id field. Return 0 on success.
//
extern int ConfigurationVendorIdGet(int *id);


//
// Get the device id field. Return 0 on success.
//
extern int ConfigurationSubVendorIdGet(int *id);
//
// Get pcieAddress configure structure value data at address. Return 0 on success, -3 for the address not found
extern A_INT32 ConfigurationpcieAddressValueDataGet(char *sValue, int address);

extern A_INT32 ConfigurationpcieAddressValueDataOfNumGet(int num, unsigned int *value);

extern A_INT32 ConfigPCIeOnBoard(int iItem, unsigned int *value);

