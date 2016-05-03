#include <ah_osdep.h>

extern int AnwiDriverDetach(void);


//
// Grab the device id from the pci bus
// is there anything else we should check, like an Atheros id?
// how do we get this for other bus types?
//
extern int DeviceIdGet(void);

	
extern int AnwiDriverAttach(int devid);

extern uintptr_t AnwiDriverMemoryMap(void);


extern int AnwiDriverMemorySize(void);

extern uintptr_t AnwiDriverRegisterMap(void);

//
// returns 1 if the Anwi driver is successfully loaded
//
extern int AnwiDriverValid(void);
