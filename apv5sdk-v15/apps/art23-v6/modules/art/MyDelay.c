
//  "ACI $Id: //depot/sw/branches/narthal_mar2011/dk/mdk/art/MyDelay.c#1 $, $Header: //depot/sw/branches/narthal_mar2011/dk/mdk/art/MyDelay.c#1 $"

#ifdef _WINDOWS
 #include <windows.h>
#endif


#ifndef LINUX
#include <conio.h>
#include <io.h>
#endif

//#define OSPREY 1

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


#include "MyDelay.h"


//#define OSPREY 1


void MyDelay(int ms)
{
#ifdef OSPREY
	if(ms<=0) ms=1;
#endif
#ifdef MDK_AP
	milliSleep(ms);
#else
	Sleep(ms);
#endif
}

