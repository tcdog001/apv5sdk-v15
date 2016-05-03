


#include "TxDescriptor.h"

#define MBUFFER 1024

static int (*_TxDescriptorLinkPtrSet)(void *block, unsigned int ptr);
static int (*_TxDescriptorTxRateSet)(void *block, unsigned int rate);

static unsigned char (*_TxDescriptorBaStatus)(void *block);
static unsigned int (*_TxDescriptorAggLength)(void *block);
static unsigned int (*_TxDescriptorBaBitmapLow)(void *block);
static unsigned int (*_TxDescriptorBaBitmapHigh)(void *block);
static unsigned char (*_TxDescriptorFifoUnderrun)(void *block);
static unsigned char (*_TxDescriptorExcessiveRetries)(void *block);
static unsigned int (*_TxDescriptorRtsFailCount)(void *block);
static unsigned int (*_TxDescriptorDataFailCount)(void *block);

static unsigned int (*_TxDescriptorLinkPtr)(void *block);
static unsigned int (*_TxDescriptorBufPtr)(void *block);
static unsigned int (*_TxDescriptorBufLen)(void *block);
static unsigned char (*_TxDescriptorIntReq)(void *block);
static unsigned int (*_TxDescriptorRssiCombined)(void *block);
static unsigned int (*_TxDescriptorRssiAnt00)(void *block);
static unsigned int (*_TxDescriptorRssiAnt01)(void *block);
static unsigned int (*_TxDescriptorRssiAnt02)(void *block);
static unsigned int (*_TxDescriptorRssiAnt10)(void *block);
static unsigned int (*_TxDescriptorRssiAnt11)(void *block);
static unsigned int (*_TxDescriptorRssiAnt12)(void *block);
static unsigned int (*_TxDescriptorTxRate)(void *block);
static unsigned int (*_TxDescriptorDataLen)(void *block);
static unsigned char (*_TxDescriptorMore)(void *block);
static unsigned int (*_TxDescriptorNumDelim)(void *block);
static unsigned int (*_TxDescriptorSendTimestamp)(void *block);
static unsigned char (*_TxDescriptorGi)(void *block);
static unsigned char (*_TxDescriptorH2040)(void *block);
static unsigned char (*_TxDescriptorDuplicate)(void *block);
static unsigned int (*_TxDescriptorTxAntenna)(void *block);
static double (*_TxDescriptorEvm0)(void *block);
static double (*_TxDescriptorEvm1)(void *block);
static double (*_TxDescriptorEvm2)(void *block);
static unsigned char (*_TxDescriptorDone)(void *block);
static unsigned char (*_TxDescriptorFrameTxOk)(void *block);
static unsigned char (*_TxDescriptorCrcError)(void *block);
static unsigned char (*_TxDescriptorDecryptCrcErr)(void *block);
static unsigned char (*_TxDescriptorPhyErr)(void *block);
static unsigned char (*_TxDescriptorMicError)(void *block);
static unsigned char (*_TxDescriptorPreDelimCrcErr)(void *block);
static unsigned char (*_TxDescriptorKeyIdxValid)(void *block);
static unsigned int (*_TxDescriptorKeyIdx)(void *block);
static unsigned char (*_TxDescriptorMoreAgg)(void *block);
static unsigned char (*_TxDescriptorAggregate)(void *block);
static unsigned char (*_TxDescriptorPostDelimCrcErr)(void *block);
static unsigned char (*_TxDescriptorDecryptBusyErr)(void *block);
static unsigned char (*_TxDescriptorKeyMiss)(void *block);

static void (*_TxDescriptorSetup)(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, int buf_len,
	int broadcast, int retry,
	int rate, int ht40, int shortGi, unsigned int txchain,
	int isagg, int moreagg,
	int id);

static void (*_TxDescriptorStatusSetup)(void *block); 

static void (*_TxDescriptorReset)(void *block);

static int (*_TxDescriptorSize)();

static int (*_TxDescriptorStatusSize)();

static int (*_TxDescriptorPrint)(void *block, char *buffer, int max);

static void  (*_TxDescriptorPAPDSetup)(void *block, int chainNum);

#ifdef UNUSED
static void (*_TxDescriptorWrite)(void *block, unsigned int physical);
static void (*_TxDescriptorRead)(void *block, unsigned int physical);
#endif


int TxDescriptorLinkPtrSet(void *block, unsigned int ptr)
{
	if(_TxDescriptorLinkPtrSet!=0)
	{
		return _TxDescriptorLinkPtrSet(block,ptr);
	}
	return 0;
}

int TxDescriptorTxRateSet(void *block, unsigned int rate)
{
	if(_TxDescriptorTxRateSet!=0)
	{
		return _TxDescriptorTxRateSet(block,rate);
	}
	return 0;
}

int TxDescriptorPrint(void *block, char *buffer, int max)
{
	if(_TxDescriptorPrint!=0)
	{
		return _TxDescriptorPrint(block,buffer,max);
	}
	return 0;
}

unsigned char TxDescriptorBaStatus(void *block)
{
	if(_TxDescriptorBaStatus!=0)
	{
		return _TxDescriptorBaStatus(block);
	}
	return 0;
}

unsigned int TxDescriptorAggLength(void *block)
{
	if(_TxDescriptorAggLength!=0)
	{
		return _TxDescriptorAggLength(block);
	}
	return 0;
}

unsigned int TxDescriptorBaBitmapLow(void *block)
{
	if(_TxDescriptorBaBitmapLow!=0)
	{
		return _TxDescriptorBaBitmapLow(block);
	}
	return 0;
}

unsigned int TxDescriptorBaBitmapHigh(void *block)
{
	if(_TxDescriptorBaBitmapHigh!=0)
	{
		return _TxDescriptorBaBitmapHigh(block);
	}
	return 0;
}

unsigned char TxDescriptorFifoUnderrun(void *block)
{
	if(_TxDescriptorFifoUnderrun!=0)
	{
		return _TxDescriptorFifoUnderrun(block);
	}
	return 0;
}

unsigned char TxDescriptorExcessiveRetries(void *block)
{
	if(_TxDescriptorExcessiveRetries!=0)
	{
		return _TxDescriptorExcessiveRetries(block);
	}
	return 0;
}

unsigned int TxDescriptorRtsFailCount(void *block)
{
	if(_TxDescriptorRtsFailCount!=0)
	{
		return _TxDescriptorRtsFailCount(block);
	}
	return 0;
}

unsigned int TxDescriptorDataFailCount(void *block)
{
	if(_TxDescriptorDataFailCount!=0)
	{
		return _TxDescriptorDataFailCount(block);
	}
	return 0;
}

unsigned int TxDescriptorLinkPtr(void *block)
{
	if(_TxDescriptorLinkPtr!=0)
	{
		return _TxDescriptorLinkPtr(block);
	}
	return 0;
}

unsigned int TxDescriptorBufPtr(void *block)
{
	if(_TxDescriptorBufPtr!=0)
	{
		return _TxDescriptorBufPtr(block);
	}
	return 0;
}

unsigned int TxDescriptorBufLen(void *block)
{
	if(_TxDescriptorBufLen!=0)
	{
		return _TxDescriptorBufLen(block);
	}
	return 0;
}

unsigned char TxDescriptorIntReq(void *block)
{
	if(_TxDescriptorIntReq!=0)
	{
		return _TxDescriptorIntReq(block);
	}
	return 0;
}

unsigned int TxDescriptorRssiCombined(void *block)
{
	if(_TxDescriptorRssiCombined!=0)
	{
		return _TxDescriptorRssiCombined(block);
	}
	return 0;
}

unsigned int TxDescriptorRssiAnt00(void *block)
{
	if(_TxDescriptorRssiAnt00!=0)
	{
		return _TxDescriptorRssiAnt00(block);
	}
	return 0;
}

unsigned int TxDescriptorRssiAnt01(void *block)
{
	if(_TxDescriptorRssiAnt01!=0)
	{
		return _TxDescriptorRssiAnt01(block);
	}
	return 0;
}

unsigned int TxDescriptorRssiAnt02(void *block)
{
	if(_TxDescriptorRssiAnt02!=0)
	{
		return _TxDescriptorRssiAnt02(block);
	}
	return 0;
}

unsigned int TxDescriptorRssiAnt10(void *block)
{
	if(_TxDescriptorRssiAnt10!=0)
	{
		return _TxDescriptorRssiAnt10(block);
	}
	return 0;
}

unsigned int TxDescriptorRssiAnt11(void *block)
{
	if(_TxDescriptorRssiAnt11!=0)
	{
		return _TxDescriptorRssiAnt11(block);
	}
	return 0;
}

unsigned int TxDescriptorRssiAnt12(void *block)
{
	if(_TxDescriptorRssiAnt12!=0)
	{
		return _TxDescriptorRssiAnt12(block);
	}
	return 0;
}

unsigned int TxDescriptorTxRate(void *block)
{
	if(_TxDescriptorTxRate!=0)
	{
		return _TxDescriptorTxRate(block);
	}
	return 0;
}

unsigned int TxDescriptorDataLen(void *block)
{
	if(_TxDescriptorDataLen!=0)
	{
		return _TxDescriptorDataLen(block);
	}
	return 0;
}

unsigned char TxDescriptorMore(void *block)
{
	if(_TxDescriptorMore!=0)
	{
		return _TxDescriptorMore(block);
	}
	return 0;
}

unsigned int TxDescriptorNumDelim(void *block)
{
	if(_TxDescriptorNumDelim!=0)
	{
		return _TxDescriptorNumDelim(block);
	}
	return 0;
}

unsigned int TxDescriptorSendTimestamp(void *block)
{
	if(_TxDescriptorSendTimestamp!=0)
	{
		return _TxDescriptorSendTimestamp(block);
	}
	return 0;
}

unsigned char TxDescriptorGi(void *block)
{
	if(_TxDescriptorGi!=0)
	{
		return _TxDescriptorGi(block);
	}
	return 0;
}

unsigned char TxDescriptorH2040(void *block)
{
	if(_TxDescriptorH2040!=0)
	{
		return _TxDescriptorH2040(block);
	}
	return 0;
}

unsigned char TxDescriptorDuplicate(void *block)
{
	if(_TxDescriptorDuplicate!=0)
	{
		return _TxDescriptorDuplicate(block);
	}
	return 0;
}

unsigned int TxDescriptorTxAntenna(void *block)
{
	if(_TxDescriptorTxAntenna!=0)
	{
		return _TxDescriptorTxAntenna(block);
	}
	return 0;
}

double TxDescriptorEvm0(void *block)
{
	if(_TxDescriptorEvm0!=0)
	{
		return _TxDescriptorEvm0(block);
	}
	return 0;
}

double TxDescriptorEvm1(void *block)
{
	if(_TxDescriptorEvm1!=0)
	{
		return _TxDescriptorEvm1(block);
	}
	return 0;
}

double TxDescriptorEvm2(void *block)
{
	if(_TxDescriptorEvm2!=0)
	{
		return _TxDescriptorEvm2(block);
	}
	return 0;
}

unsigned char TxDescriptorDone(void *block)
{
	if(_TxDescriptorDone!=0)
	{
		return _TxDescriptorDone(block);
	}
	return 0;
}

unsigned char TxDescriptorFrameTxOk(void *block)
{
	if(_TxDescriptorFrameTxOk!=0)
	{
		return _TxDescriptorFrameTxOk(block);
	}
	return 0;
}

unsigned char TxDescriptorCrcError(void *block)
{
	if(_TxDescriptorCrcError!=0)
	{
		return _TxDescriptorCrcError(block);
	}
	return 0;
}

unsigned char TxDescriptorDecryptCrcErr(void *block)
{
	if(_TxDescriptorDecryptCrcErr!=0)
	{
		return _TxDescriptorDecryptCrcErr(block);
	}
	return 0;
}

unsigned char TxDescriptorPhyErr(void *block)
{
	if(_TxDescriptorPhyErr!=0)
	{
		return _TxDescriptorPhyErr(block);
	}
	return 0;
}

unsigned char TxDescriptorMicError(void *block)
{
	if(_TxDescriptorMicError!=0)
	{
		return _TxDescriptorMicError(block);
	}
	return 0;
}

unsigned char TxDescriptorPreDelimCrcErr(void *block)
{
	if(_TxDescriptorPreDelimCrcErr!=0)
	{
		return _TxDescriptorPreDelimCrcErr(block);
	}
	return 0;
}

unsigned char TxDescriptorKeyIdxValid(void *block)
{
	if(_TxDescriptorKeyIdxValid!=0)
	{
		return _TxDescriptorKeyIdxValid(block);
	}
	return 0;
}

unsigned int TxDescriptorKeyIdx(void *block)
{
	if(_TxDescriptorKeyIdx!=0)
	{
		return _TxDescriptorKeyIdx(block);
	}
	return 0;
}

unsigned char TxDescriptorMoreAgg(void *block)
{
	if(_TxDescriptorMoreAgg!=0)
	{
		return _TxDescriptorMoreAgg(block);
	}
	return 0;
}

unsigned char TxDescriptorAggregate(void *block)
{
	if(_TxDescriptorAggregate!=0)
	{
		return _TxDescriptorAggregate(block);
	}
	return 0;
}

unsigned char TxDescriptorPostDelimCrcErr(void *block)
{
	if(_TxDescriptorPostDelimCrcErr!=0)
	{
		return _TxDescriptorPostDelimCrcErr(block);
	}
	return 0;
}

unsigned char TxDescriptorDecryptBusyErr(void *block)
{
	if(_TxDescriptorDecryptBusyErr!=0)
	{
		return _TxDescriptorDecryptBusyErr(block);
	}
	return 0;
}

unsigned char TxDescriptorKeyMiss(void *block)
{
	if(_TxDescriptorKeyMiss!=0)
	{
		return _TxDescriptorKeyMiss(block);
	}
	return 0;
}
	
//
// setup a descriptor with the standard required fields
//
void TxDescriptorSetup(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, int buf_len,
	int broadcast, int retry,
	int rate, int ht40, int shortGi, unsigned int txchain,
	int isagg, int moreagg,
	int id)
{
	if(_TxDescriptorSetup!=0)
	{
	    _TxDescriptorSetup(block,link_ptr,buf_ptr,buf_len,
			broadcast,retry,rate,ht40,shortGi,txchain,
			isagg,moreagg,
			id);
	}
}

//
// setup a descriptor with the standard required fields
//
void TxDescriptorStatusSetup(void *block)
{
	if(_TxDescriptorStatusSetup!=0)
	{
	    _TxDescriptorStatusSetup(block);
	}
}

//
// reset the descriptor so that it can be used again
//
void TxDescriptorReset(void *block)
{  
	if(_TxDescriptorReset!=0)
	{
		_TxDescriptorReset(block);
	}
}

//
// return the size of a descriptor 
//
int TxDescriptorSize()
{
	if(_TxDescriptorSize!=0)
	{
		return _TxDescriptorSize();
	}
	return 0;
}

//
// return the size of a descriptor 
//
int TxDescriptorStatusSize()
{
	if(_TxDescriptorStatusSize!=0)
	{
		return _TxDescriptorStatusSize();
	}
	return 0;
}

// Setup PA predistortion training chain number
void TxDescriptorPAPDSetup(void *block, int chainNum)
{
	if(_TxDescriptorPAPDSetup!=0)
	{
			_TxDescriptorPAPDSetup(block, chainNum);
	}

}

#ifdef UNUSED
//
// copy the descriptor from application memory to the shared memory
//
void TxDescriptorWrite(void *block, unsigned int physical)
{
	if(_TxDescriptorWrite!=0)
	{
		_TxDescriptorWrite(block,physical);
	}
}

//
// copy the descriptor from the shared memory to application memory
//
void TxDescriptorRead(void *block, unsigned int physical)
{
	if(_TxDescriptorRead!=0)
	{
		_TxDescriptorRead(block,physical);
	}
}
#endif

//
// clear all Tx descriptor function pointers and set to default behavior
//
void TxDescriptorFunctionReset()
{
	//
	// install generic behavior
	//
	_TxDescriptorLinkPtrSet=0;
	_TxDescriptorTxRateSet=0;
#ifdef UNUSED
	_TxDescriptorRead=0;
	_TxDescriptorWrite=0;
#endif
	_TxDescriptorSize=0;
	_TxDescriptorStatusSize=0;
	_TxDescriptorSetup=0;
	_TxDescriptorStatusSetup=0;
	_TxDescriptorReset=0;
	_TxDescriptorPrint=0;
    _TxDescriptorBaStatus=0;
    _TxDescriptorAggLength=0;
    _TxDescriptorBaBitmapLow=0;
    _TxDescriptorBaBitmapHigh=0;
    _TxDescriptorFifoUnderrun=0;
    _TxDescriptorExcessiveRetries=0;
    _TxDescriptorRtsFailCount=0;
    _TxDescriptorDataFailCount=0;
    _TxDescriptorLinkPtr=0;
    _TxDescriptorBufPtr=0;
    _TxDescriptorBufLen=0;
    _TxDescriptorIntReq=0;
    _TxDescriptorRssiCombined=0;
    _TxDescriptorRssiAnt00=0;
    _TxDescriptorRssiAnt01=0;
    _TxDescriptorRssiAnt02=0;
    _TxDescriptorRssiAnt10=0;
    _TxDescriptorRssiAnt11=0;
    _TxDescriptorRssiAnt12=0;
    _TxDescriptorTxRate=0;
    _TxDescriptorDataLen=0;
    _TxDescriptorMore=0;
    _TxDescriptorNumDelim=0;
    _TxDescriptorSendTimestamp=0;
    _TxDescriptorGi=0;
    _TxDescriptorH2040=0;
    _TxDescriptorDuplicate=0;
    _TxDescriptorTxAntenna=0;
    _TxDescriptorEvm0=0;
    _TxDescriptorEvm1=0;
    _TxDescriptorEvm2=0;
    _TxDescriptorDone=0;
    _TxDescriptorFrameTxOk=0;
    _TxDescriptorCrcError=0;
    _TxDescriptorDecryptCrcErr=0;
    _TxDescriptorPhyErr=0;
    _TxDescriptorMicError=0;
    _TxDescriptorPreDelimCrcErr=0;
    _TxDescriptorKeyIdxValid=0;
    _TxDescriptorKeyIdx=0;
    _TxDescriptorMoreAgg=0;
    _TxDescriptorAggregate=0;
    _TxDescriptorPostDelimCrcErr=0;
    _TxDescriptorDecryptBusyErr=0;
    _TxDescriptorKeyMiss=0;

    _TxDescriptorSetup=0, 
    _TxDescriptorStatusSetup=0, 
    _TxDescriptorReset=0;

    _TxDescriptorSize=0;
    _TxDescriptorStatusSize=0;

    _TxDescriptorPrint=0;
	_TxDescriptorPAPDSetup=0;
#ifdef UNUSED
    _TxDescriptorWrite=0;
    _TxDescriptorRead=0;
#endif
}

//
// set the chip specific function
//

void TxDescriptorLinkPtrSetFunction(int (*f)(void *block, unsigned int ptr))
{
	_TxDescriptorLinkPtrSet=f;
}

void TxDescriptorTxRateSetFunction(int (*f)(void *block, unsigned int rate))
{
	_TxDescriptorTxRateSet=f;
}

void TxDescriptorBaStatusFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorBaStatus=f;
}

void TxDescriptorAggLengthFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorAggLength=f;
}

void TxDescriptorBaBitmapLowFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorBaBitmapLow=f;
}

void TxDescriptorBaBitmapHighFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorBaBitmapHigh=f;
}

void TxDescriptorFifoUnderrunFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorFifoUnderrun=f;
}

void TxDescriptorExcessiveRetriesFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorExcessiveRetries=f;
}

void TxDescriptorRtsFailCountFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorRtsFailCount=f;
}

void TxDescriptorDataFailCountFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorDataFailCount=f;
}

void TxDescriptorLinkPtrFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorLinkPtr=f;
}

void TxDescriptorBufPtrFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorBufPtr=f;
}

void TxDescriptorBufLenFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorBufLen=f;
}

void TxDescriptorIntReqFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorIntReq=f;
}

void TxDescriptorRssiCombinedFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorRssiCombined=f;
}

void TxDescriptorRssiAnt00Function(unsigned int (*f)(void *block))
{
	_TxDescriptorRssiAnt00=f;
}

void TxDescriptorRssiAnt01Function(unsigned int (*f)(void *block))
{
	_TxDescriptorRssiAnt01=f;
}

void TxDescriptorRssiAnt02Function(unsigned int (*f)(void *block))
{
	_TxDescriptorRssiAnt02=f;
}

void TxDescriptorRssiAnt10Function(unsigned int (*f)(void *block))
{
	_TxDescriptorRssiAnt10=f;
}

void TxDescriptorRssiAnt11Function(unsigned int (*f)(void *block))
{
	_TxDescriptorRssiAnt11=f;
}

void TxDescriptorRssiAnt12Function(unsigned int (*f)(void *block))
{
	_TxDescriptorRssiAnt12=f;
}

void TxDescriptorTxRateFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorTxRate=f;
}

void TxDescriptorDataLenFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorDataLen=f;
}

void TxDescriptorMoreFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorMore=f;
}

void TxDescriptorNumDelimFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorNumDelim=f;
}

void TxDescriptorSendTimestampFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorSendTimestamp=f;
}

void TxDescriptorGiFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorGi=f;
}

void TxDescriptorH2040Function(unsigned char (*f)(void *block))
{
	_TxDescriptorH2040=f;
}

void TxDescriptorDuplicateFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorDuplicate=f;
}

void TxDescriptorTxAntennaFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorTxAntenna=f;
}

void TxDescriptorEvm0Function(double (*f)(void *block))
{
	_TxDescriptorEvm0=f;
}

void TxDescriptorEvm1Function(double (*f)(void *block))
{
	_TxDescriptorEvm1=f;
}

void TxDescriptorEvm2Function(double (*f)(void *block))
{
	_TxDescriptorEvm2=f;
}

void TxDescriptorDoneFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorDone=f;
}

void TxDescriptorFrameTxOkFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorFrameTxOk=f;
}

void TxDescriptorCrcErrorFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorCrcError=f;
}

void TxDescriptorDecryptCrcErrFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorDecryptCrcErr=f;
}

void TxDescriptorPhyErrFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorPhyErr=f;
}

void TxDescriptorMicErrorFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorMicError=f;
}

void TxDescriptorPreDelimCrcErrFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorPreDelimCrcErr=f;
}

void TxDescriptorKeyIdxValidFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorKeyIdxValid=f;
}

void TxDescriptorKeyIdxFunction(unsigned int (*f)(void *block))
{
	_TxDescriptorKeyIdx=f;
}

void TxDescriptorMoreAggFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorMoreAgg=f;
}

void TxDescriptorAggregateFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorAggregate=f;
}

void TxDescriptorPostDelimCrcErrFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorPostDelimCrcErr=f;
}

void TxDescriptorDecryptBusyErrFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorDecryptBusyErr=f;
}

void TxDescriptorKeyMissFunction(unsigned char (*f)(void *block))
{
	_TxDescriptorKeyMiss=f;
}


void TxDescriptorSetupFunction(void (*f)(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, int buf_len,
	int broadcast, int retry,
	int rate, int ht40, int shortGi, unsigned int txchain,
	int isagg, int moreagg,
	int id))
{
	_TxDescriptorSetup=f;
}

void TxDescriptorStatusSetupFunction(void (*f)(void *block))
{
	_TxDescriptorStatusSetup=f;
}

void TxDescriptorResetFunction(void (*f)(void *block))
{
	_TxDescriptorReset=f;
}

void TxDescriptorSizeFunction(int (*f)())
{
	_TxDescriptorSize=f;
}

void TxDescriptorStatusSizeFunction(int (*f)())
{
	_TxDescriptorStatusSize=f;
}

void TxDescriptorPrintFunction(int (*f)(void *block, char *buffer, int max))
{
	_TxDescriptorPrint=f;
}

void TxDescriptorPAPDSetupFunction(void (*f)(void *block, int chainNum))
{
	_TxDescriptorPAPDSetup=f;
}

#ifdef UNUSED
void TxDescriptorReadFunction(void (*f)(void *block, unsigned int physical))
{
	_TxDescriptorRead=f;
}

void TxDescriptorWriteFunction(void (*f)(void *block, unsigned int physical))
{
	_TxDescriptorWrite=f;
}
#endif

