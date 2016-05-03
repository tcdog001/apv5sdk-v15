

//  "ACI $Id: //depot/sw/branches/narthal_mar2011/dk/mdk/shared/EepromError.c#2 $, $Header: //depot/sw/branches/narthal_mar2011/dk/mdk/shared/EepromError.c#2 $"

#ifdef UNUSED
#ifdef _WINDOWS
 #include <windows.h>
#endif


#ifndef LINUX
#include <conio.h>
#include <io.h>
#endif
#endif


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "ErrorPrint.h"
#include "EepromError.h"

static struct _Error _EepromError[]=
{
	{EepromNoRoom,ErrorFatal,EepromNoRoomFormat},
	{EepromVerify,ErrorFatal,EepromVerifyFormat},
	{EepromAlgorithm,ErrorInformation,EepromAlgorithmFormat},
	{EepromTooMany,ErrorFatal,EepromTooManyFormat},
	{EepromWrite,ErrorFatal,EepromWriteFormat},
	{EepromFatal,ErrorFatal,EepromFatalFormat},
	{EepromWontFit,ErrorFatal,EepromWontFitFormat},
	{EepromNotEmpty,ErrorFatal,EepromNotEmptyFormat},

	{PcieVerify,ErrorFatal,PcieVerifyFormat},
	{PcieTooMany,ErrorFatal,PcieTooManyFormat},
	{PcieWrite,ErrorFatal,PcieWriteFormat},
	{PcieFatal,ErrorFatal,PcieFatalFormat},
	{PcieWontFit,ErrorFatal,PcieWontFitFormat},
};

static int _ErrorFirst=1;

void EepromErrorInit(void)
{
    if(_ErrorFirst)
    {
        ErrorHashCreate(_EepromError,sizeof(_EepromError)/sizeof(_EepromError[0]));
    }
    _ErrorFirst=0;
}

