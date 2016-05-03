
//#ident  "ACI $Id: //depot/sw/branches/narthal_mar2011/dk/mdk/shared/TimeMillisecond.c#1 $, $Header: //depot/sw/branches/narthal_mar2011/dk/mdk/shared/TimeMillisecond.c#1 $"

#ifdef _WINDOWS
 #include <windows.h>
#endif

#if defined (LINUX) || defined(__APPLE__)
#include <sys/time.h>
#endif

#ifndef LINUX
#ifndef __APPLE__
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




#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>



#include "TimeMillisecond.h"

int TimeMillisecond()
{
#ifdef _WINDOWS
	return GetTickCount();
#elif defined(LINUX) || defined(__APPLE__)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec*1000+tv.tv_usec/1000);
	
#endif

}


