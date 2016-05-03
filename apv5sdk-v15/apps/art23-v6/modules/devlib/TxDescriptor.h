
//
// set the link pointer in a descriptor
//
extern int TxDescriptorLinkPtrSet(void *block, unsigned int ptr);

//
// set the transmit rate in a descriptor
//
extern int TxDescriptorTxRateSet(void *block, unsigned int rate);



extern int TxDescriptorPrint(void *block, char *buffer, int max);

extern unsigned char TxDescriptorBaStatus(void *block);

extern unsigned int TxDescriptorAggLength(void *block);

extern unsigned int TxDescriptorBaBitmapLow(void *block);

extern unsigned int TxDescriptorBaBitmapHigh(void *block);

extern unsigned char TxDescriptorFifoUnderrun(void *block);

extern unsigned char TxDescriptorExcessiveRetries(void *block);

extern unsigned int TxDescriptorRtsFailCount(void *block);

extern unsigned int TxDescriptorDataFailCount(void *block);

extern unsigned int TxDescriptorLinkPtr(void *block);

extern unsigned int TxDescriptorBufPtr(void *block);

extern unsigned int TxDescriptorBufLen(void *block);

extern unsigned char TxDescriptorIntReq(void *block);

extern unsigned int TxDescriptorRssiCombined(void *block);

extern unsigned int TxDescriptorRssiAnt00(void *block);

extern unsigned int TxDescriptorRssiAnt01(void *block);

extern unsigned int TxDescriptorRssiAnt02(void *block);

extern unsigned int TxDescriptorRssiAnt10(void *block);

extern unsigned int TxDescriptorRssiAnt11(void *block);

extern unsigned int TxDescriptorRssiAnt12(void *block);

extern unsigned int TxDescriptorTxRate(void *block);

extern unsigned int TxDescriptorDataLen(void *block);

extern unsigned char TxDescriptorMore(void *block);

extern unsigned int TxDescriptorNumDelim(void *block);

extern unsigned int TxDescriptorSendTimestamp(void *block);

extern unsigned char TxDescriptorGi(void *block);

extern unsigned char TxDescriptorH2040(void *block);

extern unsigned char TxDescriptorDuplicate(void *block);

extern unsigned int TxDescriptorTxAntenna(void *block);

extern double TxDescriptorEvm0(void *block);

extern double TxDescriptorEvm1(void *block);

extern double TxDescriptorEvm2(void *block);

extern unsigned char TxDescriptorDone(void *block);

extern unsigned char TxDescriptorFrameTxOk(void *block);

extern unsigned char TxDescriptorCrcError(void *block);

extern unsigned char TxDescriptorDecryptCrcErr(void *block);

extern unsigned char TxDescriptorPhyErr(void *block);

extern unsigned char TxDescriptorMicError(void *block);

extern unsigned char TxDescriptorPreDelimCrcErr(void *block);

extern unsigned char TxDescriptorKeyIdxValid(void *block);

extern unsigned int TxDescriptorKeyIdx(void *block);

extern unsigned char TxDescriptorMoreAgg(void *block);

extern unsigned char TxDescriptorAggregate(void *block);

extern unsigned char TxDescriptorPostDelimCrcErr(void *block);

extern unsigned char TxDescriptorDecryptBusyErr(void *block);

extern unsigned char TxDescriptorKeyMiss(void *block);
	
//
// setup a descriptor with the standard required fields
//
extern void TxDescriptorSetup(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, int buf_len,
	int broadcast, int retry,
	int rate, int ht40, int shortGi, unsigned int txchain,
	int isagg, int moreagg,
	int id);


//
// setup a descriptor with the standard required fields
//
extern void TxDescriptorStatusSetup(void *block);



//
// reset the descriptor so that it can be used again
//
extern void TxDescriptorReset(void *block);


//
// return the size of a descriptor 
//
extern int TxDescriptorSize();


//
// return the size of a descriptor 
//
extern int TxDescriptorStatusSize();

// set descritop bit PA predistortion chain num
extern void TxDescriptorPAPDSetup(void *block, int chainNum);


//
// clear all Tx descriptor function pointers and set to default behavior
//
extern void TxDescriptorFunctionReset();


//
// set the chip specific function
//


extern void TxDescriptorLinkPtrSetFunction(int (*f)(void *block, unsigned int ptr));

extern void TxDescriptorTxRateSetFunction(int (*f)(void *block, unsigned int rate));

extern void TxDescriptorBaStatusFunction(unsigned char (*f)(void *block));

extern void TxDescriptorAggLengthFunction(unsigned int (*f)(void *block));

extern void TxDescriptorBaBitmapLowFunction(unsigned int (*f)(void *block));

extern void TxDescriptorBaBitmapHighFunction(unsigned int (*f)(void *block));

extern void TxDescriptorFifoUnderrunFunction(unsigned char (*f)(void *block));

extern void TxDescriptorExcessiveRetriesFunction(unsigned char (*f)(void *block));

extern void TxDescriptorRtsFailCountFunction(unsigned int (*f)(void *block));

extern void TxDescriptorDataFailCountFunction(unsigned int (*f)(void *block));

extern void TxDescriptorLinkPtrFunction(unsigned int (*f)(void *block));

extern void TxDescriptorBufPtrFunction(unsigned int (*f)(void *block));

extern void TxDescriptorBufLenFunction(unsigned int (*f)(void *block));

extern void TxDescriptorIntReqFunction(unsigned char (*f)(void *block));

extern void TxDescriptorRssiCombinedFunction(unsigned int (*f)(void *block));

extern void TxDescriptorRssiAnt00Function(unsigned int (*f)(void *block));

extern void TxDescriptorRssiAnt01Function(unsigned int (*f)(void *block));

extern void TxDescriptorRssiAnt02Function(unsigned int (*f)(void *block));

extern void TxDescriptorRssiAnt10Function(unsigned int (*f)(void *block));

extern void TxDescriptorRssiAnt11Function(unsigned int (*f)(void *block));

extern void TxDescriptorRssiAnt12Function(unsigned int (*f)(void *block));

extern void TxDescriptorTxRateFunction(unsigned int (*f)(void *block));

extern void TxDescriptorDataLenFunction(unsigned int (*f)(void *block));

extern void TxDescriptorMoreFunction(unsigned char (*f)(void *block));

extern void TxDescriptorNumDelimFunction(unsigned int (*f)(void *block));

extern void TxDescriptorSendTimestampFunction(unsigned int (*f)(void *block));

extern void TxDescriptorGiFunction(unsigned char (*f)(void *block));

extern void TxDescriptorH2040Function(unsigned char (*f)(void *block));

extern void TxDescriptorDuplicateFunction(unsigned char (*f)(void *block));

extern void TxDescriptorTxAntennaFunction(unsigned int (*f)(void *block));

extern void TxDescriptorEvm0Function(double (*f)(void *block));

extern void TxDescriptorEvm1Function(double (*f)(void *block));

extern void TxDescriptorEvm2Function(double (*f)(void *block));

extern void TxDescriptorDoneFunction(unsigned char (*f)(void *block));

extern void TxDescriptorFrameTxOkFunction(unsigned char (*f)(void *block));

extern void TxDescriptorCrcErrorFunction(unsigned char (*f)(void *block));

extern void TxDescriptorDecryptCrcErrFunction(unsigned char (*f)(void *block));

extern void TxDescriptorPhyErrFunction(unsigned char (*f)(void *block));

extern void TxDescriptorMicErrorFunction(unsigned char (*f)(void *block));

extern void TxDescriptorPreDelimCrcErrFunction(unsigned char (*f)(void *block));

extern void TxDescriptorKeyIdxValidFunction(unsigned char (*f)(void *block));

extern void TxDescriptorKeyIdxFunction(unsigned int (*f)(void *block));

extern void TxDescriptorMoreAggFunction(unsigned char (*f)(void *block));

extern void TxDescriptorAggregateFunction(unsigned char (*f)(void *block));

extern void TxDescriptorPostDelimCrcErrFunction(unsigned char (*f)(void *block));

extern void TxDescriptorDecryptBusyErrFunction(unsigned char (*f)(void *block));

extern void TxDescriptorKeyMissFunction(unsigned char (*f)(void *block));

extern void TxDescriptorSetupFunction(void (*f)(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, int buf_len,
	int broadcast, int retry,
	int rate, int ht40, int shortGi, unsigned int txchain,
	int isagg, int moreagg,
	int id));

extern void TxDescriptorStatusSetupFunction(void (*f)(void *block));

extern void TxDescriptorResetFunction(void (*f)(void *block));

extern void TxDescriptorSizeFunction(int (*f)());

extern void TxDescriptorStatusSizeFunction(int (*f)());

extern void TxDescriptorPrintFunction(int (*f)(void *block, char *buffer, int max));

extern void TxDescriptorPAPDSetupFunction(void (*f)(void *block, int chainNum));

#ifdef UNUSED
extern void TxDescriptorReadFunction(void (*f)(void *block, unsigned int physical));

extern void TxDescriptorWriteFunction(void (*f)(void *block, unsigned int physical));
#endif


