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
 *   int nvmfile (struct _file_ * file);
 *
 *   nvm.h
 *
 *   open an NVM file and validate it by walking the header and image
 *   chain and validating all checksums; rewind valid files; return a
 *   file descriptor on success or terminate the program on error;
 *
 *   the checksum of the entire header, including header checksum, is
 *   always 0 for valid headers; similarly, the checksum of the image
 *   and image checksum is always 0 for valid images;
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

#ifndef NVMFILE_SOURCE
#define NVMFILE_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>

#include "../tools/memory.h"
#include "../tools/error.h"
#include "../nvm/nvm.h"

int nvmfile (struct _file_ * file) 

{
	struct header_nvm header_nvm;
	unsigned image = 0;
	memset (&header_nvm, 0, sizeof (header_nvm));
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind file: %s", file->name);
	}
	do 
	{
		if (lseek (file->file, 0, SEEK_CUR) != LE32TOH (header_nvm.NEXTHEADER)) 
		{
			error (1, 0, "Bad header chain: %s (%d)", file->name, image);
		}
		if (read (file->file, &header_nvm, sizeof (struct header_nvm)) < sizeof (struct header_nvm)) 
		{
			error (1, errno, "Can't read header: %s (%d)", file->name, image);
		}
		if (LE32TOH (header_nvm.HEADERVERSION) != NVM_HEADER_VERSION) 
		{
			error (1, errno, "Bad header version: %s (%d)", file->name, image);
		}
		if (checksum_32 (&header_nvm, sizeof (struct header_nvm), 0)) 
		{
			error (1, errno, "Bad header checksum: %s (%d)", file->name, image);
		}
		if (fdchecksum_32 (file->file, LE32TOH (header_nvm.IMAGELENGTH), header_nvm.IMAGECHECKSUM)) 
		{
			error (1, errno, "Bad image checksum: %s (%d)", file->name, image);
		}
		image++;
	}
	while (header_nvm.NEXTHEADER);
	if (lseek (file->file, 0, SEEK_CUR) != lseek (file->file, 0, SEEK_END)) 
	{
		error (1, errno, "Bad header chain: %s (%d)", file->name, image);
	}
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind file: %s", file->name);
	}
	return (0);
}

#endif

