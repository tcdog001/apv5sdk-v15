/*====================================================================*
 *
 *   SHA256.h - SHA256 encryption declarations and definitions;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef SHA256_HEADER
#define SHA256_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define SHA256_DIGEST_LENGTH 256/8

/*====================================================================*
 *   variables;
 *--------------------------------------------------------------------*/

typedef struct sha256 

{
	uint32_t count [2];
	uint32_t state [8];
	uint8_t block [64];
	uint8_t extra [64];
}

SHA256;

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

void SHA256Reset (struct sha256 * sha256);
void SHA256Write (struct sha256 * sha256, const void * memory, size_t extent);
void SHA256Block (struct sha256 * sha256, const void * memory);
void SHA256Fetch (struct sha256 * sha256,  byte digest []);
void SHA256Print (const byte digest [], const char * string);
void SHA256Ident (int fd, byte digest []);
bool SHA256Match (int fd, const byte digest []);

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif 

