

enum
{
    NartOk=7500,
    NartOn,
	NartOff,
	NartDataHeader,
	NartData,

	NartError, //7505
	NartDone,
	NartDebug,
	NartProcess,
	NartActive,

	NartOther, //7510
	NartRequest,
};


#define NartOkFormat "OK"
#define NartOnFormat "ON"
#define NartOffFormat "OFF"
#define NartDataHeaderFormat "%s"
#define NartDataFormat "%s"
#define NartErrorFormat "ERROR %s"
#define NartDoneFormat "DONE %s"
#define NartDebugFormat "%s"
#define NartProcessFormat "BEGIN %s"
#define NartActiveFormat "Please look for another active nart."
#define NartOtherFormat "%s"
#define NartRequestFormat "%s"


extern void NartErrorInit(void);


