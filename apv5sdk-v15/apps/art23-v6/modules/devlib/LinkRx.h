
extern unsigned int MemoryLayout(int size);


// this frees everything.
extern void MemoryFree(unsigned int buffer);

//
// Return address of first rx descriptor.
//
extern unsigned int LinkRxLoopFirst();


//
// Destroy loop of rx descriptors and free all of the memory.
//
extern int LinkRxLoopDestroy();


//
// Create a loop of rx descriptors.
// Return the address of the first one.
//
extern int LinkRxLoopCreate(int many);


extern void LinkRxSetup(unsigned char *bssid, unsigned char *macaddr);


extern void LinkRxStart(int promiscuous);


extern void LinkRxComplete(int timeout, 
	int enableCompare, unsigned char *dataPattern, int dataPatternLength, int (*done)());


extern void LinkRxOptions(int fifo, int maxdescriptor, int enablefirst);
