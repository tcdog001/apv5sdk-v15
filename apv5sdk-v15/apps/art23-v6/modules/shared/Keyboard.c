
// "ACI $Id: //depot/sw/branches/narthal_mar2011/dk/mdk/shared/Keyboard.c#1 $, $Header: //depot/sw/branches/narthal_mar2011/dk/mdk/shared/Keyboard.c#1 $"




#ifdef _WINDOWS
 #include <windows.h>
#endif


#if !defined(LINUX) && !defined(__APPLE__)
#include <conio.h>
#include <io.h>
#endif


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include <ctype.h>

#include "smatch.h"
#include "Keyboard.h"


#ifdef __APPLE__
//#include <darwin_ansi.h>
#include <sys/types.h>
#endif

int KeyboardHasInput(void)
{
#if (defined(LINUX ) && !defined(SOC_LINUX)) || defined(__APPLE__)
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);	
#else
	return _kbhit();
#endif	
}


void KeyboardBeep(void)
{
#ifdef WINDOWS
    Beep(750,300);
#endif
}


//
// get line of input from keyboard
//
int KeyboardReadWait(char *buffer, int maxlen)
{
	int length;
	//
	// get input from terminal
	//
	buffer[0]=0;
    fgets(buffer,maxlen,stdin);
	length=StrimEnd(buffer);

	return length;
}

//
// get line of input from keyboard
//
int KeyboardRead(char *buffer, int maxlen)
{
	//
	// get input from terminal
	//
	if(KeyboardHasInput())
	{
	    return KeyboardReadWait(buffer,maxlen);
	}
	return -1;
}

