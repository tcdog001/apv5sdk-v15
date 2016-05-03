//===================================================================================
//
//    (C) COPYRIGHT International Business Machines Corp., 2002 All Rights Reserved
//    Licensed Materials - Property of IBM
//    US Government Users Restricted Rights - Use, duplication or
//    disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//
//    IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
//    ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//    PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
//    CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
//    USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
//    OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
//    OR PERFORMANCE OF THIS SOFTWARE. 
//
//    The program may be used, executed, copied, modified, and distributed
//    without royalty for the purpose of developing, using, marketing, or distributing.
//
//=======================================================================================

/*
Copyright (C) 2000-2004, Robert van Engelen, Genivia, Inc., All Rights Reserved.
*/

#include "stdlib.h"
#include <string.h>
#include <va_list.h>
#include <stdarg.h>
#include <StringMgr.h>
#include "PalmFunctions.h"

#ifdef __cplusplus
extern "C" {
#endif

int AppNetOpenCount = 0;
extern UInt16 AppNetRefnum;
extern	Int32 AppNetTimeout;
extern Err 	NetUInit (void);

char * PalmStrerror(int err)
{
    static char str[40];

    SysErrString(err,str,40);

    return str;
}

double strtod(const char *s, char **endptr) {

	char *p = StrChr(s,'E');
   	FlpCompDouble result;
   	*endptr = ((char*)s)+StrLen(s);
   	if (p)       // change E to e and remove '+'
   	{
    	*p='e';
    	if (*(p+1)=='+')
      	StrCopy(p+1,p+2);
   	}
   	FlpBufferAToF (&result.fd, s);

   return result.d;
}

void palmSprintf(char *str, const char *conv, ...)
{
    int retValue = 1;
    size_t n = strlen(conv);
    va_list args;
    va_start(args, conv);

    // Catch the float 

    if(n < 6 && conv[0] == '%' && (conv[n-1] == 'g' || conv[n-1] == 'G' || conv[n-1] == 'f' || conv[n-1] == 'F'))
    {  
        FlpCompDouble flp;
        flp.d = (double) va_arg(args, double);
        FlpFToA(flp.fd,str);
        goto end;
    }
    StrVPrintF(str,(const Char *)conv,args);
    end:
      va_end(args);
}

Err palmNetLibOpen() 
{
	Err error = 0;
	UInt16 ifErrs;
	
	if (!AppNetOpenCount)
	{
		
		error = NetUInit();
		if (error) return error;
		
		error = NetLibOpen(AppNetRefnum, &ifErrs);
		
		if (error == netErrAlreadyOpen)  {
		error = 0;
		}
		
	}
	
	if (!error) AppNetOpenCount++;
	return error;

}
Err palmNetLibClose()
{

	Err error = 0;
	
	if (AppNetOpenCount)
	{
		
		AppNetOpenCount--;
		error = NetLibClose(AppNetRefnum, false);
	}
	
	return error;

}

int palmRand()
{
/* Compute x = (7^5 * x) mod (2^31 - 1)
 * without overflowing 31 bits:
 *      (2^31 - 1) = 127773 * (7^5) + 2836
 * From "Random number generators: good ones are hard to find",
 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
 * October 1988, p. 1195.
 */
       static int seed = 0x19610910;
       int hi = seed / 127773L;
       int lo = seed % 127773L;
       int x = 16807L * lo - 2836L * hi;
       if (x <= 0)
               x += 0x7fffffffL;
       return ((seed = x) % 0x8FFFFFFF);
}

#ifdef __cplusplus
}
#endif
