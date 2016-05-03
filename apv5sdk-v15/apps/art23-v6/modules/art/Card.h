


extern struct ath_hal *AH;
//
// check is a card is loaded.
// if not, try to load using the default id from the bus
// returns 0 if there is a good card.
//
extern int CardCheckAndLoad(int client);

//
// Returns 1 is there is a valid card loaded and ready for operation.
//
extern int CardValid(void);

//
// Returns 1 is there is a valid card loaded and ready for operation.
//
extern int CardValidReset(void);

//
// returns 0 on success
//
extern int CardRemove(int client);

//
// Force the code to perform a reset even if it looks like it isn't required.
//
extern void CardResetForce();

//
// returns 0 on success
//
extern int CardLoad(int client);

extern void CardLoadParameterSplice(struct _ParameterList *list);


//
// returns 0 on success
//
extern int CardReset(int client);

extern void CardResetParameterSplice(struct _ParameterList *list);


//
// return a list of valid channel descriptors
//
extern int CardChannel(int client);

extern int CardChannelCalculate(void);


//
// return card specific information
// call on connect from cart to send information on cards that may have been loaded previously
//
extern int CardDataSend(int client);



extern int CardResetDo(int frequency, int txchain, int rxchain, int bandwidth);


extern int CardResetIfNeeded(int frequency, int txchain, int rxchain, int force, int bandwidth);

extern int CardChannelFind(int fmin, int fmax, int bw, int *rfrequency, int *rbw);

extern int CardRxIqCalComplete();

//
// return the channel frequency used in the last chip reset
//
extern int CardFrequency(void);


extern int CardRxChainMany(void);


extern int CardTxChainMany(void);


extern int CardTxChainMask(void);


extern int CardRxChainMask(void);



extern void MyRegisterDebug(int state);


extern int MyRegisterRead(unsigned int address, unsigned int *value);


extern int MyRegisterWrite(unsigned int address, unsigned int value);


unsigned int MaskCreate(int low, int high);


extern int MyFieldRead(unsigned int address, int low, int high, unsigned int *value);


extern int MyFieldWrite(unsigned int address, int low, int high, unsigned int value);

#ifdef __APPLE__
extern uintptr_t MyMemoryBase(void);
#else
extern int MyMemoryBase(void);
#endif 

unsigned char *MyMemoryPtr(unsigned int address);


extern int MyMemoryRead(unsigned int address, unsigned int *buffer, int many);


extern int MyMemoryWrite(unsigned int address, unsigned int *buffer, int many);


extern void FreeMemoryPrint(void);

