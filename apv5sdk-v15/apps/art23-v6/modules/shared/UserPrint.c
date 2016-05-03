


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Socket.h"
#include "ErrorPrint.h"


#define MBUFFER 1024

static int _UserPrintConsole=0;
static FILE *_UserPrintFile=0;
static struct _Socket *_UserPrintSocket=0;
static void (*_UserPrintFunction)(char *buffer);


void UserPrintIt(char *buffer)
{
	if(_UserPrintConsole)
	{
		printf("%s",buffer);
	}
	if(_UserPrintFile)
	{
		fprintf(_UserPrintFile,"%s",buffer);
		fflush(_UserPrintFile);
	}
	if(_UserPrintSocket)
	{
		SocketWrite(_UserPrintSocket,buffer,strlen(buffer));
	}
	if(_UserPrintFunction)
	{
		(*_UserPrintFunction)(buffer);
	}
}


void UserPrint(char *format, ...)
{
    va_list ap;
    char buffer[MBUFFER];

	if(_UserPrintConsole || _UserPrintFile || _UserPrintSocket || _UserPrintFunction)
	{
		va_start(ap, format);

		#if defined(Linux) || defined(__APPLE__)
			vsnprintf(buffer,MBUFFER,format,ap);
		#else
			_vsnprintf(buffer,MBUFFER,format,ap);
		#endif

		va_end(ap);
#ifdef NOTYET
        ErrorPrint(ErrorNone,buffer);
#else
        UserPrintIt(buffer);
#endif
    }
}



void UserPrintConsole(int onoff)
{
	_UserPrintConsole=onoff;
}


void UserPrintFile(FILE *file)
{
	_UserPrintFile=file;
}


void UserPrintSocket(struct _Socket *socket)
{
	_UserPrintSocket=socket;
}

void UserPrintFunction(void (*print)(char *buffer))
{
    _UserPrintFunction=print;
}

