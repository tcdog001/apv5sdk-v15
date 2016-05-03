


#include "RxDescriptor.h"

#define MBUFFER 1024


static unsigned int (*_RxDescriptorLinkPtr)(void *block);
static unsigned int (*_RxDescriptorBufPtr)(void *block);
static unsigned int (*_RxDescriptorBufLen)(void *block);
static unsigned char (*_RxDescriptorIntReq)(void *block);
static unsigned int (*_RxDescriptorRssiCombined)(void *block);
static unsigned int (*_RxDescriptorRssiAnt00)(void *block);
static unsigned int (*_RxDescriptorRssiAnt01)(void *block);
static unsigned int (*_RxDescriptorRssiAnt02)(void *block);
static unsigned int (*_RxDescriptorRssiAnt10)(void *block);
static unsigned int (*_RxDescriptorRssiAnt11)(void *block);
static unsigned int (*_RxDescriptorRssiAnt12)(void *block);
static unsigned int (*_RxDescriptorRxRate)(void *block);
static unsigned int (*_RxDescriptorDataLen)(void *block);
static unsigned char (*_RxDescriptorMore)(void *block);
static unsigned int (*_RxDescriptorNumDelim)(void *block);
static unsigned int (*_RxDescriptorRcvTimestamp)(void *block);
static unsigned char (*_RxDescriptorGi)(void *block);
static unsigned char (*_RxDescriptorH2040)(void *block);
static unsigned char (*_RxDescriptorDuplicate)(void *block);
static unsigned int (*_RxDescriptorRxAntenna)(void *block);
static double (*_RxDescriptorEvm0)(void *block);
static double (*_RxDescriptorEvm1)(void *block);
static double (*_RxDescriptorEvm2)(void *block);
static unsigned char (*_RxDescriptorDone)(void *block);
static unsigned char (*_RxDescriptorFrameRxOk)(void *block);
static unsigned char (*_RxDescriptorCrcError)(void *block);
static unsigned char (*_RxDescriptorDecryptCrcErr)(void *block);
static unsigned char (*_RxDescriptorPhyErr)(void *block);
static unsigned char (*_RxDescriptorMicError)(void *block);
static unsigned char (*_RxDescriptorPreDelimCrcErr)(void *block);
static unsigned char (*_RxDescriptorKeyIdxValid)(void *block);
static unsigned int (*_RxDescriptorKeyIdx)(void *block);
static unsigned char (*_RxDescriptorMoreAgg)(void *block);
static unsigned char (*_RxDescriptorFirstAgg)(void *block);
static unsigned char (*_RxDescriptorAggregate)(void *block);
static unsigned char (*_RxDescriptorPostDelimCrcErr)(void *block);
static unsigned char (*_RxDescriptorDecryptBusyErr)(void *block);
static unsigned char (*_RxDescriptorKeyMiss)(void *block);

static void (*_RxDescriptorSetup)(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, unsigned int buf_len);
static void (*_RxDescriptorReset)(void *block);

static int (*_RxDescriptorSize)();

static int (*_RxDescriptorPrint)(void *block, char *buffer, int max);

#ifdef UNUSED
static void (*_RxDescriptorWrite)(void *block, unsigned int physical);
static void (*_RxDescriptorRead)(void *block, unsigned int physical);
#endif


int RxDescriptorPrint(void *block, char *buffer, int max)
{
	if(_RxDescriptorPrint!=0)
	{
		return _RxDescriptorPrint(block,buffer,max);
	}
	return 0;
}

unsigned int RxDescriptorLinkPtr(void *block)
{
	if(_RxDescriptorLinkPtr!=0)
	{
		return _RxDescriptorLinkPtr(block);
	}
	return 0;
}

unsigned int RxDescriptorBufPtr(void *block)
{
	if(_RxDescriptorBufPtr!=0)
	{
		return _RxDescriptorBufPtr(block);
	}
	return 0;
}

unsigned int RxDescriptorBufLen(void *block)
{
	if(_RxDescriptorBufLen!=0)
	{
		return _RxDescriptorBufLen(block);
	}
	return 0;
}

unsigned char RxDescriptorIntReq(void *block)
{
	if(_RxDescriptorIntReq!=0)
	{
		return _RxDescriptorIntReq(block);
	}
	return 0;
}

unsigned int RxDescriptorRssiCombined(void *block)
{
	if(_RxDescriptorRssiCombined!=0)
	{
		return _RxDescriptorRssiCombined(block);
	}
	return 0;
}

unsigned int RxDescriptorRssiAnt00(void *block)
{
	if(_RxDescriptorRssiAnt00!=0)
	{
		return _RxDescriptorRssiAnt00(block);
	}
	return 0;
}

unsigned int RxDescriptorRssiAnt01(void *block)
{
	if(_RxDescriptorRssiAnt01!=0)
	{
		return _RxDescriptorRssiAnt01(block);
	}
	return 0;
}

unsigned int RxDescriptorRssiAnt02(void *block)
{
	if(_RxDescriptorRssiAnt02!=0)
	{
		return _RxDescriptorRssiAnt02(block);
	}
	return 0;
}

unsigned int RxDescriptorRssiAnt10(void *block)
{
	if(_RxDescriptorRssiAnt10!=0)
	{
		return _RxDescriptorRssiAnt10(block);
	}
	return 0;
}

unsigned int RxDescriptorRssiAnt11(void *block)
{
	if(_RxDescriptorRssiAnt11!=0)
	{
		return _RxDescriptorRssiAnt11(block);
	}
	return 0;
}

unsigned int RxDescriptorRssiAnt12(void *block)
{
	if(_RxDescriptorRssiAnt12!=0)
	{
		return _RxDescriptorRssiAnt12(block);
	}
	return 0;
}

unsigned int RxDescriptorRxRate(void *block)
{
	if(_RxDescriptorRxRate!=0)
	{
		return _RxDescriptorRxRate(block);
	}
	return 0;
}

unsigned int RxDescriptorDataLen(void *block)
{
	if(_RxDescriptorDataLen!=0)
	{
		return _RxDescriptorDataLen(block);
	}
	return 0;
}

unsigned char RxDescriptorMore(void *block)
{
	if(_RxDescriptorMore!=0)
	{
		return _RxDescriptorMore(block);
	}
	return 0;
}

unsigned int RxDescriptorNumDelim(void *block)
{
	if(_RxDescriptorNumDelim!=0)
	{
		return _RxDescriptorNumDelim(block);
	}
	return 0;
}

unsigned int RxDescriptorRcvTimestamp(void *block)
{
	if(_RxDescriptorRcvTimestamp!=0)
	{
		return _RxDescriptorRcvTimestamp(block);
	}
	return 0;
}

unsigned char RxDescriptorGi(void *block)
{
	if(_RxDescriptorGi!=0)
	{
		return _RxDescriptorGi(block);
	}
	return 0;
}

unsigned char RxDescriptorH2040(void *block)
{
	if(_RxDescriptorH2040!=0)
	{
		return _RxDescriptorH2040(block);
	}
	return 0;
}

unsigned char RxDescriptorDuplicate(void *block)
{
	if(_RxDescriptorDuplicate!=0)
	{
		return _RxDescriptorDuplicate(block);
	}
	return 0;
}

unsigned int RxDescriptorRxAntenna(void *block)
{
	if(_RxDescriptorRxAntenna!=0)
	{
		return _RxDescriptorRxAntenna(block);
	}
	return 0;
}

double RxDescriptorEvm0(void *block)
{
	if(_RxDescriptorEvm0!=0)
	{
		return _RxDescriptorEvm0(block);
	}
	return 0;
}

double RxDescriptorEvm1(void *block)
{
	if(_RxDescriptorEvm1!=0)
	{
		return _RxDescriptorEvm1(block);
	}
	return 0;
}

double RxDescriptorEvm2(void *block)
{
	if(_RxDescriptorEvm2!=0)
	{
		return _RxDescriptorEvm2(block);
	}
	return 0;
}

unsigned char RxDescriptorDone(void *block)
{
	if(_RxDescriptorDone!=0)
	{
		return _RxDescriptorDone(block);
	}
	return 0;
}

unsigned char RxDescriptorFrameRxOk(void *block)
{
	if(_RxDescriptorFrameRxOk!=0)
	{
		return _RxDescriptorFrameRxOk(block);
	}
	return 0;
}

unsigned char RxDescriptorCrcError(void *block)
{
	if(_RxDescriptorCrcError!=0)
	{
		return _RxDescriptorCrcError(block);
	}
	return 0;
}

unsigned char RxDescriptorDecryptCrcErr(void *block)
{
	if(_RxDescriptorDecryptCrcErr!=0)
	{
		return _RxDescriptorDecryptCrcErr(block);
	}
	return 0;
}

unsigned char RxDescriptorPhyErr(void *block)
{
	if(_RxDescriptorPhyErr!=0)
	{
		return _RxDescriptorPhyErr(block);
	}
	return 0;
}

unsigned char RxDescriptorMicError(void *block)
{
	if(_RxDescriptorMicError!=0)
	{
		return _RxDescriptorMicError(block);
	}
	return 0;
}

unsigned char RxDescriptorPreDelimCrcErr(void *block)
{
	if(_RxDescriptorPreDelimCrcErr!=0)
	{
		return _RxDescriptorPreDelimCrcErr(block);
	}
	return 0;
}

unsigned char RxDescriptorKeyIdxValid(void *block)
{
	if(_RxDescriptorKeyIdxValid!=0)
	{
		return _RxDescriptorKeyIdxValid(block);
	}
	return 0;
}

unsigned int RxDescriptorKeyIdx(void *block)
{
	if(_RxDescriptorKeyIdx!=0)
	{
		return _RxDescriptorKeyIdx(block);
	}
	return 0;
}

unsigned char RxDescriptorFirstAgg(void *block)
{
	if(_RxDescriptorFirstAgg!=0)
	{
		return _RxDescriptorFirstAgg(block);
	}
	return 0;
}

unsigned char RxDescriptorMoreAgg(void *block)
{
	if(_RxDescriptorMoreAgg!=0)
	{
		return _RxDescriptorMoreAgg(block);
	}
	return 0;
}

unsigned char RxDescriptorAggregate(void *block)
{
	if(_RxDescriptorAggregate!=0)
	{
		return _RxDescriptorAggregate(block);
	}
	return 0;
}

unsigned char RxDescriptorPostDelimCrcErr(void *block)
{
	if(_RxDescriptorPostDelimCrcErr!=0)
	{
		return _RxDescriptorPostDelimCrcErr(block);
	}
	return 0;
}

unsigned char RxDescriptorDecryptBusyErr(void *block)
{
	if(_RxDescriptorDecryptBusyErr!=0)
	{
		return _RxDescriptorDecryptBusyErr(block);
	}
	return 0;
}

unsigned char RxDescriptorKeyMiss(void *block)
{
	if(_RxDescriptorKeyMiss!=0)
	{
		return _RxDescriptorKeyMiss(block);
	}
	return 0;
}
	
//
// setup a descriptor with the standard required fields
//
void RxDescriptorSetup(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, unsigned int buf_len)
{
	if(_RxDescriptorSetup!=0)
	{
	    _RxDescriptorSetup(block,link_ptr,buf_ptr,buf_len);
	}
}

//
// reset the descriptor so that it can be used again
//
void RxDescriptorReset(void *block)
{  
	if(_RxDescriptorReset!=0)
	{
		_RxDescriptorReset(block);
	}
}

//
// return the size of a descriptor 
//
int RxDescriptorSize()
{
	if(_RxDescriptorSize!=0)
	{
		return _RxDescriptorSize();
	}
	return 0;
}

#ifdef UNUSED
//
// copy the descriptor from application memory to the shared memory
//
void RxDescriptorWrite(void *block, unsigned int physical)
{
	if(_RxDescriptorWrite!=0)
	{
		_RxDescriptorWrite(block,physical);
	}
}

//
// copy the descriptor from the shared memory to application memory
//
void RxDescriptorRead(void *block, unsigned int physical)
{
	if(_RxDescriptorRead!=0)
	{
		_RxDescriptorRead(block,physical);
	}
}
#endif

//
// clear all rx descriptor function pointers and set to default behavior
//
void RxDescriptorFunctionReset()
{
	//
	// install generic behavior
	//
#ifdef UNUSED
	_RxDescriptorRead=0;
	_RxDescriptorWrite=0;
#endif
	_RxDescriptorSize=0;
	_RxDescriptorSetup=0;
	_RxDescriptorReset=0;
	_RxDescriptorPrint=0;
    _RxDescriptorLinkPtr=0;
    _RxDescriptorBufPtr=0;
    _RxDescriptorBufLen=0;
    _RxDescriptorIntReq=0;
    _RxDescriptorRssiCombined=0;
    _RxDescriptorRssiAnt00=0;
    _RxDescriptorRssiAnt01=0;
    _RxDescriptorRssiAnt02=0;
    _RxDescriptorRssiAnt10=0;
    _RxDescriptorRssiAnt11=0;
    _RxDescriptorRssiAnt12=0;
    _RxDescriptorRxRate=0;
    _RxDescriptorDataLen=0;
    _RxDescriptorMore=0;
    _RxDescriptorNumDelim=0;
    _RxDescriptorRcvTimestamp=0;
    _RxDescriptorGi=0;
    _RxDescriptorH2040=0;
    _RxDescriptorDuplicate=0;
    _RxDescriptorRxAntenna=0;
    _RxDescriptorEvm0=0;
    _RxDescriptorEvm1=0;
    _RxDescriptorEvm2=0;
    _RxDescriptorDone=0;
    _RxDescriptorFrameRxOk=0;
    _RxDescriptorCrcError=0;
    _RxDescriptorDecryptCrcErr=0;
    _RxDescriptorPhyErr=0;
    _RxDescriptorMicError=0;
    _RxDescriptorPreDelimCrcErr=0;
    _RxDescriptorKeyIdxValid=0;
    _RxDescriptorKeyIdx=0;
    _RxDescriptorMoreAgg=0;
    _RxDescriptorAggregate=0;
    _RxDescriptorPostDelimCrcErr=0;
    _RxDescriptorDecryptBusyErr=0;
    _RxDescriptorKeyMiss=0;

    _RxDescriptorSetup=0, 
    _RxDescriptorReset=0;

    _RxDescriptorSize=0;

    _RxDescriptorPrint=0;
#ifdef UNUSED
    _RxDescriptorWrite=0;
    _RxDescriptorRead=0;
#endif
}

//
// set the chip specific function
//

void RxDescriptorLinkPtrFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorLinkPtr=f;
}

void RxDescriptorBufPtrFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorBufPtr=f;
}

void RxDescriptorBufLenFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorBufLen=f;
}

void RxDescriptorIntReqFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorIntReq=f;
}

void RxDescriptorRssiCombinedFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorRssiCombined=f;
}

void RxDescriptorRssiAnt00Function(unsigned int (*f)(void *block))
{
	_RxDescriptorRssiAnt00=f;
}

void RxDescriptorRssiAnt01Function(unsigned int (*f)(void *block))
{
	_RxDescriptorRssiAnt01=f;
}

void RxDescriptorRssiAnt02Function(unsigned int (*f)(void *block))
{
	_RxDescriptorRssiAnt02=f;
}

void RxDescriptorRssiAnt10Function(unsigned int (*f)(void *block))
{
	_RxDescriptorRssiAnt10=f;
}

void RxDescriptorRssiAnt11Function(unsigned int (*f)(void *block))
{
	_RxDescriptorRssiAnt11=f;
}

void RxDescriptorRssiAnt12Function(unsigned int (*f)(void *block))
{
	_RxDescriptorRssiAnt12=f;
}

void RxDescriptorRxRateFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorRxRate=f;
}

void RxDescriptorDataLenFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorDataLen=f;
}

void RxDescriptorMoreFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorMore=f;
}

void RxDescriptorNumDelimFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorNumDelim=f;
}

void RxDescriptorRcvTimestampFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorRcvTimestamp=f;
}

void RxDescriptorGiFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorGi=f;
}

void RxDescriptorH2040Function(unsigned char (*f)(void *block))
{
	_RxDescriptorH2040=f;
}

void RxDescriptorDuplicateFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorDuplicate=f;
}

void RxDescriptorRxAntennaFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorRxAntenna=f;
}

void RxDescriptorEvm0Function(double (*f)(void *block))
{
	_RxDescriptorEvm0=f;
}

void RxDescriptorEvm1Function(double (*f)(void *block))
{
	_RxDescriptorEvm1=f;
}

void RxDescriptorEvm2Function(double (*f)(void *block))
{
	_RxDescriptorEvm2=f;
}

void RxDescriptorDoneFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorDone=f;
}

void RxDescriptorFrameRxOkFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorFrameRxOk=f;
}

void RxDescriptorCrcErrorFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorCrcError=f;
}

void RxDescriptorDecryptCrcErrFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorDecryptCrcErr=f;
}

void RxDescriptorPhyErrFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorPhyErr=f;
}

void RxDescriptorMicErrorFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorMicError=f;
}

void RxDescriptorPreDelimCrcErrFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorPreDelimCrcErr=f;
}

void RxDescriptorKeyIdxValidFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorKeyIdxValid=f;
}

void RxDescriptorKeyIdxFunction(unsigned int (*f)(void *block))
{
	_RxDescriptorKeyIdx=f;
}

void RxDescriptorFirstAggFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorFirstAgg=f;
}

void RxDescriptorMoreAggFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorMoreAgg=f;
}

void RxDescriptorAggregateFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorAggregate=f;
}

void RxDescriptorPostDelimCrcErrFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorPostDelimCrcErr=f;
}

void RxDescriptorDecryptBusyErrFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorDecryptBusyErr=f;
}

void RxDescriptorKeyMissFunction(unsigned char (*f)(void *block))
{
	_RxDescriptorKeyMiss=f;
}


void RxDescriptorSetupFunction(void (*f)(void *block, 
	unsigned int link_ptr, unsigned int buf_ptr, unsigned int buf_len))
{
	_RxDescriptorSetup=f;
}

void RxDescriptorResetFunction(void (*f)(void *block))
{
	_RxDescriptorReset=f;
}

void RxDescriptorSizeFunction(int (*f)())
{
	_RxDescriptorSize=f;
}

void RxDescriptorPrintFunction(int (*f)(void *block, char *buffer, int max))
{
	_RxDescriptorPrint=f;
}

#ifdef UNUSED
void RxDescriptorReadFunction(void (*f)(void *block, unsigned int physical))
{
	_RxDescriptorRead=f;
}

void RxDescriptorWriteFunction(void (*f)(void *block, unsigned int physical))
{
	_RxDescriptorWrite=f;
}
#endif

