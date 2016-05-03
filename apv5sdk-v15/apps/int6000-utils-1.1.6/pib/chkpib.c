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
 *   chkpib.c - validate pib files using internal checksums;
 *
 *   validate one or more PIB files using header checksum and length;
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
 *		Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../tools/files.h"
#include "../crypt/HPAVKey.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/checksum_32.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/hexstring.c"
#include "../tools/hexdecode.c"
#include "../tools/error.c"
#include "../tools/alert.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibpeek.c"
#include "../pib/pibcodes.c"
#endif 

#ifndef MAKEFILE
#include "../crypt/SHA256Reset.c"
#include "../crypt/SHA256Block.c"
#include "../crypt/SHA256Write.c"
#include "../crypt/SHA256Fetch.c"
#include "../crypt/HPAVKeyNID.c"
#include "../crypt/keys.c"
#endif

/*====================================================================*
 *
 *   signed chkpib (struct _file_ * file, flag_t flags);
 *
 *   pib.h
 *
 *   open an PIB file and validate it by checking the version number,
 *   filesize and file checksum; return 0 on success or -1 on error;
 *
 *   the computed checksum of the file should be 0 because the file
 *   includes the original checksum;
 *
 *   there are three types of error: bad version, bad checksum and
 *   I/O error and they all return -1; errno will be EINVAL if the
 *   pib version is bad, 0 if the checksum is bad or the I/O error
 *   code;
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

signed chkpib (struct _file_ * file, flag_t flags) 

{
	uint8_t NID [HPAVKEY_NID_LEN];
	struct simple_pib header_pib = 
	{
		0 
	};
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			error (0, errno, "Can't rewind %s", file->name);
		}
		return (-1);
	}
	if (read (file->file, &header_pib, sizeof (struct simple_pib)) < sizeof (struct simple_pib)) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			alert ("Can't read %s", file->name);
		}
		return (-1);
	}
	if (_anyset (flags, PIB_VERBOSE)) 
	{
		printf ("------- %s -------\n", file->name);
		if (pibpeek ((byte *)(&header_pib))) 
		{
			if (_allclr (flags, PIB_SILENCE)) 
			{
				error (0, 0, "Bad version: %s", file->name);
			}
			return (-1);
		}
	}
	if (lseek (file->file, 0, SEEK_END) != LE16TOH (header_pib.PIBLENGTH)) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			error (0, errno, "Bad length: %s", file->name);
		}
		return (-1);
	}
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			error (0, errno, "Can't rewind %s", file->name);
		}
		return (-1);
	}
	if (fdchecksum_32 (file->file, LE16TOH (header_pib.PIBLENGTH), 0)) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			alert ("Bad checksum: %s", file->name);
		}
		return (-1);
	}
	HPAVKeyNID (NID, header_pib.NMK, header_pib.PreferredNID [HPAVKEY_NID_LEN-1]>>4);
	if (memcmp (NID, header_pib.PreferredNID, sizeof (NID))) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			alert ("Bad network identifier: %s", file->name);
		}
		return (-1);
	}
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			error (0, errno, "Can't rewind %s", file->name);
		}
		return (-1);
	}
	if (_allclr (flags, PIB_SILENCE)) 
	{
		alert ("%s looks good", file->name);
	}
	return (0);
}

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"qv",
		"file [file] [...]",
		"validate PIB files based on internal checksums",
		"q\tquiet",
		"v\tverbose messages",
		(const char *) (0) 
	};
	signed state = 0;
	struct _file_ pib;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'q':
			_setbits (flags, PIB_SILENCE);
			break;
		case 'v':
			_setbits (flags, PIB_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	while ((argc) && (*argv)) 
	{
		pib.name = *argv;
		if ((pib.file = open (pib.name, O_BINARY|O_RDONLY)) == -1) 
		{
			error (0, errno, "can't open %s", pib.name);
			state = 1;
			errno = 0;
		}
		else if (chkpib (&pib, flags)) 
		{
			state = 1;
		}
		close (pib.file);
		argc--;
		argv++;
	}
	return (state);
}

