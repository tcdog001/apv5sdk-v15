

//
// clear all device control function pointers and set to default behavior
//
extern void Ar9300DeviceSelect();


extern int MyAr9300EepromRead(unsigned int address, unsigned char *buffer, int many);


extern int Ar9300EepromWrite(unsigned int address, unsigned char *buffer, int many);

extern int Ar9300OtpRead(unsigned int address, unsigned char *buffer, int many);

extern int Ar9300OtpWrite(unsigned int address, unsigned char *buffer, int many);

extern void Ar9300RxChainSet(int rxChain);

extern int Ar9300Deaf(int deaf); 

