

//
// save any information required to support calibration
//
extern int Ar9300EepromSave(void);


//
// check saved calibration information
//
extern int Ar9300EepromReport(void (*print)(char *format, ...), int all);

//
// specify which templates may be used in compression.
// if many=0, all templates are allowed.
//
extern void Ar9300EepromTemplateAllowed(unsigned int *value, unsigned int many);

//
// turn compression on/off.
//
extern void Ar9300EepromCompress(unsigned int value);

//
// turn overwrite on/off.
//
extern void Ar9300EepromOverwrite(unsigned int value);

//
// override default address for saving 
//
extern void Ar9300EepromSaveAddressSet(int address);

//
// override default memory type for saving 
//
extern void Ar9300EepromSaveMemorySet(int memory);


extern int Ar9300EepromSaveMemory(void);


//
// returns lowest free memory address
//
int Ar9300EepromUsed(void);


