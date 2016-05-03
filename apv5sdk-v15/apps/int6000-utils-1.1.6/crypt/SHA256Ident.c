/*====================================================================*
 *
 *   void SHA256Ident (int fd,  byte digest []);
 *
 *   SHA256.h
 *
 *   compute the SHA256 digest of file content; the digest becomes
 *   the fingerprint that can be used to identify the file despite 
 *   filename changes;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef SHA256IDENT_SOURCE
#define SHA256IDENT_SOURCE

#include <unistd.h>

#include "../crypt/SHA256.h"

void SHA256Ident (int fd,  byte digest []) 

{
	struct sha256 sha256;
	byte buffer [1024];
	signed length;
	SHA256Reset (&sha256);
	while ((length = read (fd, buffer, sizeof (buffer))) > 0) 
	{
		SHA256Write (&sha256, buffer, length);
	}
	SHA256Fetch (&sha256, digest);
	return;
}

#endif

