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
 *   signed piblock (struct _file_ * pib);
 *
 *   recompute and update the PIB file checksum; assume the PIB file
 *   is valid;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef PIBLOCK_SOURCE
#define PIBLOCK_SOURCE

#include <unistd.h>
#include <errno.h>

#include "../tools/error.h"
#include "../tools/files.h"
#include "../pib/pib.h"

signed piblock (struct _file_ * pib) 

{
	struct simple_pib simple_pib;
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't home %s", filepart (pib->name));
	}
	if (read (pib->file, &simple_pib, sizeof (simple_pib)) < sizeof (simple_pib)) 
	{
		error (1, errno, "Can't read %s", filepart (pib->name));
	}
	simple_pib.CHECKSUM = 0;
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't home %s", filepart (pib->name));
	}
	if (write (pib->file, &simple_pib, sizeof (simple_pib)) < sizeof (simple_pib)) 
	{
		error (1, errno, "Can't save %s", filepart (pib->name));
	}
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't seek %s", filepart (pib->name));
	}
	simple_pib.CHECKSUM = fdchecksum_32 (pib->file, LE16TOH (simple_pib.PIBLENGTH), 0);
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't home %s", filepart (pib->name));
	}
	if (write (pib->file, &simple_pib, sizeof (simple_pib)) < sizeof (simple_pib)) 
	{
		error (1, errno, "Can't save %s", filepart (pib->name));
	}
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't home %s", filepart (pib->name));
	}
	return (0);
}

#endif

