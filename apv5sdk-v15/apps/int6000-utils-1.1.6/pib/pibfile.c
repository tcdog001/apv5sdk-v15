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
 *   signed pibfile (struct _file_ * pib);
 *
 *   pib.h
 *
 *   open an PIB file and validate it by checking things like file 
 *   size, checksum and selected internal parameters; return a file
 *   descriptor on success; terminate the program on error;
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

#ifndef PIBFILE_SOURCE
#define PIBFILE_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>

#include "../tools/memory.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../crypt/HPAVKey.h"
#include "../pib/pib.h"

signed pibfile (struct _file_ * pib) 

{
	struct simple_pib simple_pib;
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", pib->name);
	}
	if (read (pib->file, &simple_pib, sizeof (simple_pib)) < sizeof (simple_pib)) 
	{
		error (1, errno, "Can't read header: %s", pib->name);
	}
	if (lseek (pib->file, 0, SEEK_END) != LE16TOH (simple_pib.PIBLENGTH)) 
	{
		error (1, errno, "Bad PIB length: %s", pib->name);
	}
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", pib->name);
	}
	if (fdchecksum_32 (pib->file, LE16TOH (simple_pib.PIBLENGTH), 0)) 
	{
		error (1, errno, "Bad PIB checksum: %s", pib->name);
	}
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", pib->name);
	}
	return (0);
}

#endif

