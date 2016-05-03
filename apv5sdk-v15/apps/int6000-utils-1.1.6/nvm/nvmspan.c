/*====================================================================*
 *   
 *   Copyright (c) 2010, Atheros Communications Inc.
 *   
 *   Permission to use, copy, modify, and/or distribute this software 
 *   for any purpose with or without fee is hereby granted, provided 
 *   that the above copyright notice and this permission notice appear 
 *   in all copies.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL 
 *   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED 
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL  
 *   THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR 
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 *   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
 *   NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *   
 *--------------------------------------------------------------------*/

/*====================================================================*
 *
 *   signed nvmspan (const void * memory);
 *
 *   nvm.h
 *
 *   return the extent of an NVM file image in memory by walking the
 *   NVM header chain to the end;
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
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef NVMSPAN_SOURCE
#define NVMSPAN_SOURCE

#include <memory.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include "../tools/memory.h"
#include "../tools/error.h"
#include "../nvm/nvm.h"

signed nvmspan (const void * memory) 

{
	struct header_nvm * header_nvm;
	const byte * origin = (byte *)(memory);
	const byte * offset = (byte *)(memory);
	unsigned image = 0;
	do 
	{
		header_nvm = (struct header_nvm *)(offset);
		if (LE32TOH (header_nvm->HEADERVERSION) != NVM_HEADER_VERSION) 
		{
			return (0);
		}
		if (checksum_32 (header_nvm, sizeof (struct header_nvm), 0)) 
		{
			return (0);
		}
		offset += sizeof (struct header_nvm);
		if (checksum_32 (offset, LE32TOH (header_nvm->IMAGELENGTH), header_nvm->IMAGECHECKSUM)) 
		{
			return (0);
		}
		offset += header_nvm->IMAGELENGTH;
		image++;
	}
	while (header_nvm->NEXTHEADER);
	return (offset - origin);
}

/*====================================================================*
 *   demo/test program;
 *--------------------------------------------------------------------*/

#if 0
#include <stdio.h>
#include "../tools/checksum32.c"
#include "../tools/error.c"

#ifndef WIN32
#define O_BINARY 0
#endif

const char * program_name = "nvmspan";
int main (int argc, const char * argv []) 

{
	const char * buffer;
	const char * extent;
	signed length;
	signed fd;
	argc--;
	argv++;
	while ((argc) && (*argv)) 
	{
		if ((fd = open (*argv, O_BINARY|O_RDONLY)) == -1) 
		{
			error (1, errno, "Can't open %s", *argv);
		}
		else if ((length = lseek (fd, 0, SEEK_END)) == -1) 
		{
			error (1, errno, "Can't size %s", *argv);
		}
		else if (!(buffer = malloc (length))) 
		{
			error (1, errno, "Can't allocate buffer");
		}
		else if (lseek (fd, 0, SEEK_SET)) 
		{
			error (1, errno, "Can't home %s", *argv);
		}
		else if (read (fd, buffer, length) < length) 
		{
			error (1, errno, "Can't read %s", *argv);
		}
		else if (!(extent = nvmspan (buffer))) 
		{
			error (1, 0, "%s is corrupt", *argv);
		}
		printf ("length=%d %s\n", extent - buffer, *argv);
		close (fd);
		argc--;
		argv++;
	}
	return (0);
}

#endif

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

