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
 *   modpib.c - modify selected pib parameters and update checksum; 
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
 *	Nathan Houghton <nathan.houghton@intellon.com>
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <arpa/inet.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/number.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../tools/files.h"
#include "../tools/chars.h"
#include "../int6k/problem.h"
#include "../crypt/HPAVKey.h"
#include "../crypt/keys.h"
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
#include "../tools/uintspec.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/hexstring.c"
#include "../tools/todigit.c"
#include "../tools/memincr.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibfile.c"
#include "../pib/pibpeek.c"
#include "../pib/pibcodes.c"
#endif

#ifndef MAKEFILE
#include "../crypt/HPAVKeyNID.c"
#include "../crypt/SHA256Reset.c"
#include "../crypt/SHA256Write.c"
#include "../crypt/SHA256Block.c"
#include "../crypt/SHA256Fetch.c"
#include "../crypt/keys.c"
#endif

/*====================================================================*
 *
 *   signed pibedit (struct _file_ * file, simple_pib *sample_pib, signed level, flag_t flags);
 * 
 *   modify selected PIB header values and compute a new checksum;
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
 *	Nathan Houghton <nathan.houghton@intellon.com>
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#define MEMBERSHIP_OFFSET 0x00001E8C
#define MEMBERSHIP_STATUS 0x00000000

static signed pibedit (struct _file_ * file, simple_pib *sample_pib, signed level, flag_t flags) 

{
	struct simple_pib simple_pib;
	memset (&simple_pib, 0, sizeof (simple_pib));
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", file->name);
	}
	if (read (file->file, &simple_pib, sizeof (simple_pib)) < sizeof (simple_pib)) 
	{
		error (1, errno, "Can't read header: %s", file->name);
	}
	if (_anyset (flags, PIB_MAC)) 
	{
		memcpy (simple_pib.MAC, sample_pib->MAC, sizeof (simple_pib.MAC));
	}
	if (_anyset (flags, PIB_MACINC)) 
	{
		memincr (simple_pib.MAC, sizeof (simple_pib.MAC));
	}
	if (_anyset (flags, PIB_DAK)) 
	{
		memcpy (simple_pib.DAK, sample_pib->DAK, sizeof (simple_pib.DAK));
	}
	if (_anyset (flags, PIB_NMK)) 
	{
		memcpy (simple_pib.NMK, sample_pib->NMK, sizeof (simple_pib.NMK));
	}
	if (_anyset (flags, PIB_NETWORK)) 
	{
		memcpy (simple_pib.NET, sample_pib->NET, sizeof (simple_pib.NET));
	}
	if (_anyset (flags, PIB_MFGSTRING)) 
	{
		memcpy (simple_pib.MFG, sample_pib->MFG, sizeof (simple_pib.MFG));
	}
	if (_anyset (flags, PIB_USER)) 
	{
		memcpy (simple_pib.USR, sample_pib->USR, sizeof (simple_pib.USR));
	}
	if (_anyset (flags, PIB_CCO_MODE)) 
	{
		simple_pib.CCoSelection = sample_pib->CCoSelection;
	}
	if (_anyset (flags, PIB_NMK | PIB_NID)) 
	{

/*
 *	clear the AVLNMembership bit whenever the NMK or NID change; this is an important
 *	step because it prevents false network association;
 */

		if (ntohs (*(uint16_t *)(&simple_pib)) > 0x0200) 
		{
			uint32_t membership = MEMBERSHIP_STATUS;
			if (lseek (file->file, MEMBERSHIP_OFFSET, SEEK_SET) != MEMBERSHIP_OFFSET) 
			{
				error (1, errno, "Can't position %s", file->name);
			}
			if (write (file->file, &membership, sizeof (membership)) < sizeof (membership)) 
			{
				error (1, errno, "Can't write %s", file->name);
			}
		}
		if (_allclr (flags, PIB_NID)) 
		{
			level = simple_pib.PreferredNID [HPAVKEY_NID_LEN-1] >> 4;
		}
		HPAVKeyNID (simple_pib.PreferredNID, simple_pib.NMK, level & 1);
	}
	simple_pib.CHECKSUM = 0;
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", file->name);
	}
	if (write (file->file, &simple_pib, sizeof (simple_pib)) < sizeof (simple_pib)) 
	{
		error (1, errno, "Can't write %s", file->name);
	}
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", file->name);
	}
	simple_pib.CHECKSUM = fdchecksum_32 (file->file, LE16TOH (simple_pib.PIBLENGTH), 0);
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", file->name);
	}
	if (write (file->file, &simple_pib, sizeof (simple_pib)) < sizeof (simple_pib)) 
	{
		error (1, errno, "Can't write %s", file->name);
	}
	if (_anyset (flags, PIB_VERBOSE)) 
	{
		pibpeek ((uint8_t *)(&simple_pib));
	}
	return (0);
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv []) 
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *;  For demonstration; Not for production use;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern const struct key keys [];
	static const char *optv [] = 
	{
		"C:D:L:M:N:pP:S:T:U:vX",
		"file [file] [...]",
		"modify selected PIB parameters and update checksum",
		"C n\tCCo Selection is n",
		"D x\tDAK as xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx",
		"L n\tsecurity level is n",
		"M x\tMAC as xx:xx:xx:xx:xx:xx",
		"N x\tNMK as xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx:xx",
		"S s\tMFG string is s",
		"T s\tNET string is s",
		"U s\tUSR string is s",
		"v\tverbose messages",
		(const char *) (0) 
	};
	struct simple_pib sample_pib;
	struct _file_ pib;
	signed level = 0;
	signed state = 0;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	memset (&sample_pib, 0, sizeof (sample_pib));
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'X':
			error (0, 0, "Option -X is deprecated. Use setpib -x instead.");
			break;
		case 'C':
			sample_pib.CCoSelection = (uint8_t)(uintspec (optarg, 0, 3));
			_setbits (flags, PIB_CCO_MODE);
			break;
		case 'D':
			_setbits (flags, PIB_DAK);
			if (!strcmp (optarg, "none")) 
			{
				memcpy (sample_pib.DAK, keys [0].DAK, sizeof (sample_pib.DAK));
				break;
			}
			if (!strcmp (optarg, "key1")) 
			{
				memcpy (sample_pib.DAK, keys [1].DAK, sizeof (sample_pib.DAK));
				break;
			}
			if (!strcmp (optarg, "key2")) 
			{
				memcpy (sample_pib.DAK, keys [2].DAK, sizeof (sample_pib.DAK));
				break;
			}
			if (!hexencode (sample_pib.DAK, sizeof (sample_pib.DAK), optarg)) 
			{
				error (1, errno, INT6K_BAD_DAK, optarg);
			}
			break;
		case 'L':
			level = (uint8_t)(uintspec (optarg, 0, 1));
			_setbits (flags, PIB_NID);
			break;
		case 'M':
			if (!strcmp (optarg, "auto")) 
			{
				_setbits (flags, PIB_MACINC);
				break;
			}
			if (!strcmp (optarg, "next")) 
			{
				_setbits (flags, PIB_MACINC);
				break;
			}
			if (!strcmp (optarg, "plus")) 
			{
				_setbits (flags, PIB_MACINC);
				break;
			}
			if (!hexencode (sample_pib.MAC, sizeof (sample_pib.MAC), optarg)) 
			{
				error (1, errno, INT6K_BAD_MAC, optarg);
			}
			_setbits (flags, PIB_MAC);
			break;
		case 'N':
			_setbits (flags, PIB_NMK);
			if (!strcmp (optarg, "key0")) 
			{
				memcpy (sample_pib.NMK, keys [0].NMK, sizeof (sample_pib.NMK));
				break;
			}
			if (!strcmp (optarg, "key1")) 
			{
				memcpy (sample_pib.NMK, keys [1].NMK, sizeof (sample_pib.NMK));
				break;
			}
			if (!strcmp (optarg, "key2")) 
			{
				memcpy (sample_pib.NMK, keys [2].NMK, sizeof (sample_pib.NMK));
				break;
			}
			if (!hexencode (sample_pib.NMK, sizeof (sample_pib.NMK), optarg)) 
			{
				error (1, errno, INT6K_BAD_NMK, optarg);
			}
			break;
		case 'S':
			if (strlen (optarg) > sizeof (sample_pib.MFG) - 1) 
			{
				error (1, 0, "Manufacturing string is at most %u chars", (unsigned)(sizeof (sample_pib.MFG) - 1));
			}
			memcpy (sample_pib.MFG, optarg, strlen (optarg));
			_setbits (flags, PIB_MFGSTRING);
			break;
		case 'T':
			if (strlen (optarg) > sizeof (sample_pib.NET) - 1) 
			{
				error (1, 0, "NET string is at most %u chars", (unsigned)(sizeof (sample_pib.NET) - 1));
			}
			memcpy (sample_pib.NET, optarg, strlen (optarg));
			_setbits (flags, PIB_NETWORK);
			break;
		case 'U':
			if (strlen (optarg) > sizeof (sample_pib.USR) - 1) 
			{
				error (1, 0, "USR string is at most %u chars", (unsigned)(sizeof (sample_pib.USR) - 1));
			}
			memcpy (sample_pib.USR, optarg, strlen (optarg));
			_setbits (flags, PIB_USER);
			break;
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
		if ((pib.file = open (pib.name, O_BINARY|O_RDWR, FILE_FILEMODE)) == -1) 
		{
			error (0, errno, "Can't open %s", pib.name);
			state = 1;
			errno = 0;
		}
		else if (pibfile (&pib)) 
		{
			error (0, errno, "Bad PIB file: %s", pib.name);
			state = 1;
		}
		else if (pibedit (&pib, &sample_pib, level, flags)) 
		{
			state = 1;
		}
		close (pib.file);
		argc--;
		argv++;
	}
	return (state);
}

