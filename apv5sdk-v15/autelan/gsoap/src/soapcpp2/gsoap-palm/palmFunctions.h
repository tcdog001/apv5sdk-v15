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
#ifndef _PALMFUNCTIONS_H
#define _PALMFUNCTIONS_H

#include <FloatMgr.h>
#include <sys_types.h>

#ifdef __cplusplus
extern "C" {
#endif


Err palmNetLibOpen();
Err palmNetLibClose();

void palmSprintf(char *str, const char *conv, ...);
#undef sprintf
#define sprintf palmSprintf
#define strncmp StrNCompare
#define strncat StrNCat 
#define strerror PalmStrerror 
char * PalmStrerror(int errnum);
double strtod(const char *s, char **endptr);
int palmRand(void);
#define rand palmRand

#ifdef __cplusplus
}
#endif

#endif
