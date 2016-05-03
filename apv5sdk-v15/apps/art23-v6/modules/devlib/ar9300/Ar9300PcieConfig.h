#define PCI_EXPRESS_VENDOR_DEVICE_ADDR	          0x5000
#define PCI_EXPRESS_SUBSYS_VENDOR_DEVICE_ADDR     0x502c

typedef struct pcieConfigStruct {
    A_UINT16 address;
    A_UINT16 dataLow;
    A_UINT16 dataHigh;
} PCIE_CONFIG_STRUCT;

extern A_INT32 Ar9300SSIDSet(A_UINT16 SSID);
extern A_INT32 Ar9300SSIDGet(A_UINT16 *SSID);
extern A_INT32 Ar9300SubVendorSet(A_UINT16 subVendorID);
extern A_INT32 Ar9300SubVendorGet(A_UINT16 *subVendorID);
extern A_INT32 Ar9300deviceIDSet(A_UINT16 deviceID);
extern A_INT32 Ar9300deviceIDGet(A_UINT16 *deviceID);
extern A_INT32 Ar9300vendorSet(A_UINT16 vendorID);
extern A_INT32 Ar9300vendorGet(A_UINT16 *vendorID);
extern A_INT32 Ar9300ConfigSpaceCommit();
extern A_INT32 Ar9300ConfigSpaceUsed();
extern A_INT32 Ar9300pcieAddressValueDataInit(void);
extern A_INT32 Ar9300pcieAddressValueDataSet( A_UINT16 address, A_UINT32 data);
extern A_INT32 Ar9300pcieAddressValueDataGet( A_UINT16 address, A_UINT32 *data);
extern A_INT32 Ar9300pcieAddressValueDataRemove( A_UINT16 address);
extern A_INT32 Ar9300pcieAddressValueDataOfNumGet( int num, A_UINT16 *address, A_UINT32 *data);
extern A_INT32 Ar9300ConfigPCIeOnBoard(int iItem, A_UINT16 *addr, A_UINT32 *data);

#define MAX_pcieAddressValueData  340	// 16Kb->2kByte/6



