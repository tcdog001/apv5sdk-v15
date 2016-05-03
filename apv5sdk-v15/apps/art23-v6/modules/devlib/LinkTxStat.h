

extern void LinkTxStatFinish();


extern struct txStats *LinkTxStatTotalFetch();


extern struct txStats *LinkTxStatFetch(int rate);


extern void LinkTxStatClear();
			  

extern void LinkTxStatExtract(unsigned int *descriptor, unsigned int *control, int nagg, int np);
