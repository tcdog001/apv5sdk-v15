/*====================================================================*
 *
 *   memory.h - memory encode/decode definitions and delcaration;
 *
 *   this file is a subset of the original that includes only those
 *   definitions and declaration needed for toolkit programs;
 *
 *.  Intellon Linux Toolkit; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration and evaluation only; Not for production use;
 *
 *   Contributor (s):
 *      Charles Maier <charles.maier@intellon.com>
 *	Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef MEMORY_HEADER
#define MEMORY_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"
#include "../tools/endian.h"

#define IPv4_LEN 4
#define IPv6_LEN 16

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#ifndef offset
#define offset(struct, member) (signed)(&struct.member)-(signed)(&struct)
#endif

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#define HEXDUMP_HEADER "-------- 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ----------------\n"
#define REGDUMP_HEADER "REGISTER CONTENTS 31----24 23----16 15----08 07----00\n"

/*====================================================================*
 *   macro expansions;
 *--------------------------------------------------------------------*/

#define NEW(object) (object *)(emalloc(sizeof(object))) 
#define STR(length) (char *)(emalloc((length)+1)) 

/*====================================================================*
 *   memory increment/decrement functions;
 *--------------------------------------------------------------------*/

signed strincr (void * memory, size_t extent, byte min, byte max);
signed strdecr (void * memory, size_t extent, byte min, byte max);
signed memincr (void * memory, size_t extent);
signed memdecr (void * memory, size_t extent);

/*====================================================================*
 *   memory allocation functions;
 *--------------------------------------------------------------------*/

void * emalloc (size_t length);

/*====================================================================*
 *   memory movement functions;
 *--------------------------------------------------------------------*/

void memswap (void *,  void *, size_t extent);
void endian (void * memory, size_t extent);

/*====================================================================*
 *   memory validation functions (deprecated);
 *--------------------------------------------------------------------*/

uint32_t checksum32 (const uint32_t memory [], size_t extent, uint32_t checksum);
uint32_t fdchecksum32 (int fd, size_t extent, uint32_t checksum);

/*====================================================================*
 *   memory validation functions;
 *--------------------------------------------------------------------*/

uint32_t checksum_32 (const void * memory, size_t extent, uint32_t checksum);
uint32_t fdchecksum_32 (int fd, size_t extent, uint32_t checksum);

/*====================================================================*
 *   memory encode functions;
 *--------------------------------------------------------------------*/

size_t enhex (char buffer [], size_t length, void const * memory, size_t extent);
size_t dehex (char const buffer [], size_t length, void * memory, size_t extent);
size_t hexencode (void * memory, size_t extent, const char *string);
size_t decencode (void * memory, size_t extent, const char *string);
size_t hexdecode (const void * memory, size_t extent,  char buffer [], size_t length);
size_t decdecode (const void * memory, size_t extent,  char buffer [], size_t length);
char * hexstring (char buffer [], size_t length, const void * memory, size_t extent);
char * decstring (char buffer [], size_t length, const void * memory, size_t extent);
size_t bytespec (const char * string,  void * memory, size_t extent);
size_t dataspec (const char * string,  void * memory, size_t extent);
size_t ipv4spec (const char * string,  void * memory);
size_t ipv6spec (const char * string,  void * memory);

/*====================================================================*
 *   memory input functions;
 *--------------------------------------------------------------------*/

size_t hexin (void * memory, size_t extent, FILE *fp);

/*====================================================================*
 *   memory display functions;
 *--------------------------------------------------------------------*/

void hexdump (const void * memory, size_t offset, size_t extent, FILE *fp);
void bindump (const void * memory, size_t offset, size_t extent, FILE *fp);
void hexview (const void * memory, size_t offset, size_t extent, FILE *fp);
void regview (const void * memory, size_t offset, size_t extent, FILE *fp);
void hexout (const void * memory, size_t extent, signed c, FILE *fp);
void decout (const void * memory, size_t extent, signed c, FILE *fp);
void binout (const void * memory, size_t extent, signed c, FILE *fp);
void memout (const void * memory, size_t extent, const char * format, unsigned group, signed c, FILE *fp);
void hexwrite (signed fd, const void * memory, size_t extent);

/*====================================================================*
 *   end definitions;
 *--------------------------------------------------------------------*/

#endif

