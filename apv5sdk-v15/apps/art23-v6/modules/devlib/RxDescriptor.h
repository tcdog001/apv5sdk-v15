


extern int RxDescriptorPrint(void *block, char *buffer, int max);

extern unsigned int RxDescriptorLinkPtr(void *block);

extern unsigned int RxDescriptorBufPtr(void *block);

extern unsigned int RxDescriptorBufLen(void *block);

extern unsigned char RxDescriptorIntReq(void *block);

extern unsigned int RxDescriptorRssiCombined(void *block);

extern unsigned int RxDescriptorRssiAnt00(void *block);

extern unsigned int RxDescriptorRssiAnt01(void *block);

extern unsigned int RxDescriptorRssiAnt02(void *block);

extern unsigned int RxDescriptorRssiAnt10(void *block);

extern unsigned int RxDescriptorRssiAnt11(void *block);

extern unsigned int RxDescriptorRssiAnt12(void *block);

extern unsigned int RxDescriptorRxRate(void *block);

extern unsigned int RxDescriptorDataLen(void *block);

extern unsigned char RxDescriptorMore(void *block);

extern unsigned int RxDescriptorNumDelim(void *block);

extern unsigned int RxDescriptorRcvTimestamp(void *block);

extern unsigned char RxDescriptorGi(void *block);

extern unsigned char RxDescriptorH2040(void *block);

extern unsigned char RxDescriptorDuplicate(void *block);

extern unsigned int RxDescriptorRxAntenna(void *block);

extern double RxDescriptorEvm0(void *block);

extern double RxDescriptorEvm1(void *block);

extern double RxDescriptorEvm2(void *block);

extern unsigned char RxDescriptorDone(void *block);

extern unsigned char RxDescriptorFrameRxOk(void *block);

extern unsigned char RxDescriptorCrcError(void *block);

extern unsigned char RxDescriptorDecryptCrcErr(void *block);

extern unsigned char RxDescriptorPhyErr(void *block);

extern unsigned char RxDescriptorMicError(void *block);

extern unsigned char RxDescriptorPreDelimCrcErr(void *block);

extern unsigned char RxDescriptorKeyIdxValid(void *block);

extern unsigned int RxDescriptorKeyIdx(void *block);

extern unsigned char RxDescriptorMoreAgg(void *block);

extern unsigned char RxDescriptorFirstAgg(void *block);

extern unsigned char RxDescriptorAggregate(void *block);

extern unsigned char RxDescriptorPostDelimCrcErr(void *block);

extern unsigned char RxDescriptorDecryptBusyErr(void *block);

extern unsigned char RxDescriptorKeyMiss(void *block);
	
//
// setup a descriptor with the standard required fields
//
extern void RxDescriptorSetup(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, unsigned int buf_len);


//
// reset the descriptor so that it can be used again
//
extern void RxDescriptorReset(void *block);


//
// return the size of a descriptor 
//
extern int RxDescriptorSize();

#ifdef UNUSED
//
// copy the descriptor from application memory to the shared memory
//
extern void RxDescriptorWrite(void *block, unsigned int physical);


//
// copy the descriptor from the shared memory to application memory
//
extern void RxDescriptorRead(void *block, unsigned int physical);
#endif


//
// clear all rx descriptor function pointers and set to default behavior
//
extern void RxDescriptorFunctionReset();


//
// set the chip specific function
//

extern void RxDescriptorLinkPtrFunction(unsigned int (*f)(void *block));

extern void RxDescriptorBufPtrFunction(unsigned int (*f)(void *block));

extern void RxDescriptorBufLenFunction(unsigned int (*f)(void *block));

extern void RxDescriptorIntReqFunction(unsigned char (*f)(void *block));

extern void RxDescriptorRssiCombinedFunction(unsigned int (*f)(void *block));

extern void RxDescriptorRssiAnt00Function(unsigned int (*f)(void *block));

extern void RxDescriptorRssiAnt01Function(unsigned int (*f)(void *block));

extern void RxDescriptorRssiAnt02Function(unsigned int (*f)(void *block));

extern void RxDescriptorRssiAnt10Function(unsigned int (*f)(void *block));

extern void RxDescriptorRssiAnt11Function(unsigned int (*f)(void *block));

extern void RxDescriptorRssiAnt12Function(unsigned int (*f)(void *block));

extern void RxDescriptorRxRateFunction(unsigned int (*f)(void *block));

extern void RxDescriptorDataLenFunction(unsigned int (*f)(void *block));

extern void RxDescriptorMoreFunction(unsigned char (*f)(void *block));

extern void RxDescriptorNumDelimFunction(unsigned int (*f)(void *block));

extern void RxDescriptorRcvTimestampFunction(unsigned int (*f)(void *block));

extern void RxDescriptorGiFunction(unsigned char (*f)(void *block));

extern void RxDescriptorH2040Function(unsigned char (*f)(void *block));

extern void RxDescriptorDuplicateFunction(unsigned char (*f)(void *block));

extern void RxDescriptorRxAntennaFunction(unsigned int (*f)(void *block));

extern void RxDescriptorEvm0Function(double (*f)(void *block));

extern void RxDescriptorEvm1Function(double (*f)(void *block));

extern void RxDescriptorEvm2Function(double (*f)(void *block));

extern void RxDescriptorDoneFunction(unsigned char (*f)(void *block));

extern void RxDescriptorFrameRxOkFunction(unsigned char (*f)(void *block));

extern void RxDescriptorCrcErrorFunction(unsigned char (*f)(void *block));

extern void RxDescriptorDecryptCrcErrFunction(unsigned char (*f)(void *block));

extern void RxDescriptorPhyErrFunction(unsigned char (*f)(void *block));

extern void RxDescriptorMicErrorFunction(unsigned char (*f)(void *block));

extern void RxDescriptorPreDelimCrcErrFunction(unsigned char (*f)(void *block));

extern void RxDescriptorKeyIdxValidFunction(unsigned char (*f)(void *block));

extern void RxDescriptorKeyIdxFunction(unsigned int (*f)(void *block));

extern void RxDescriptorMoreAggFunction(unsigned char (*f)(void *block));

extern void RxDescriptorFirstAggFunction(unsigned char (*f)(void *block));

extern void RxDescriptorAggregateFunction(unsigned char (*f)(void *block));

extern void RxDescriptorPostDelimCrcErrFunction(unsigned char (*f)(void *block));

extern void RxDescriptorDecryptBusyErrFunction(unsigned char (*f)(void *block));

extern void RxDescriptorKeyMissFunction(unsigned char (*f)(void *block));

extern void RxDescriptorSetupFunction(void (*f)(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, unsigned int buf_len));

extern void RxDescriptorResetFunction(void (*f)(void *block));

extern void RxDescriptorSizeFunction(int (*f)());

extern void RxDescriptorPrintFunction(int (*f)(void *block, char *buffer, int max));

#ifdef UNUSED
extern void RxDescriptorReadFunction(void (*f)(void *block, unsigned int physical));

extern void RxDescriptorWriteFunction(void (*f)(void *block, unsigned int physical));
#endif


