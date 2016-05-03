/*====================================================================*
 *   
 *   void SHA256Print (const byte digest [], const char * string);
 *
 *   HPAVKey.h
 *
 *   print a digest in hexadecimal on stdout followed by string if
 *   string is non-empty;
 *
 *   This software and documentation is the property of Intellon 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Intellon assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Intellon customers may modify and distribute the software 
 *   without obligation to Intellon. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef SHA256PRINT_SOURCE
#define SHA256PRINT_SOURCE

#include <stdio.h>

#include "../crypt/SHA256.h"
#include "../tools/number.h"

void SHA256Print (const byte digest [], const char * string) 

{
	unsigned length = SHA256_DIGEST_LENGTH;
	while (length--) 
	{
		putc (DIGITS_HEX [(* digest >> 4) & 0x0F], stdout);
		putc (DIGITS_HEX [(* digest >> 0) & 0x0F], stdout);
		digest++;
	}
	if (string) for (putc (' ', stdout); *string; string++) 
	{
		putc (*string, stdout);
	}
	printf ("\n");
	return;
}

#endif

