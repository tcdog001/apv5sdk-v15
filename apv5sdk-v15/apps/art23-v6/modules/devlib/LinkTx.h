



extern struct txStats *LinkTxStatFetch(int rate);


extern struct txStats *LinkTxStatTotalFetch();


extern int LinkTxStart();


extern int LinkTxComplete(int timeout, int (*ison)(), int (*done)(), int chipTemperature);

//
// get all of the packets descriptors ready to run
//
extern int LinkTxSetup(int *rate, int nrate, int interleave,
	unsigned char *bssid, unsigned char *source, unsigned char *destination, 
	int PacketMany, int PacketLength,
	int RetryMax, int Antenna, int Bc, int Ifs,
	int shortGi, unsigned int txchain,
	int naggregate,
	unsigned char *pattern, int npattern);


extern void LinkTxOptions(int controlfifo, int splitdescriptor, int aggstatus, int enablefirst);

extern void LinkTxForPAPD(int ChainNum);

