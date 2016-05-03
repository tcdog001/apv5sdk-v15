/*====================================================================*
 *
 *   signed pibtype (struct _file_ const * file, flag_t flags);
 * 
 *   pib.h
 *
 *   determine if file is a PIB file and position to the start of the
 *   PIB block within the file; return -1 on non-PIB or file error;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *   
 *--------------------------------------------------------------------*/

#ifndef PIBTYPE_SOURCE
#define PIBTYPE_SOURCE

#include <unistd.h>

#include "../tools/files.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../tools/endian.h"
#include "../pib/pib.h"

signed pibtype (struct _file_ const * file, flag_t flags) 

{
	uint32_t version;
	if (read (file->file, &version, sizeof (version)) != sizeof (version)) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			error (0, errno, FILE_CANTREAD, file->name);
		}
		return (-1);
	}
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			error (0, errno, FILE_CANTHOME, file->name);
		}
		return (-1);
	}
	if (LE32TOH (version) == 0x60000000) 
	{
		if (_allclr (flags, PIB_SILENCE)) 
		{
			error (0, 0, FILE_WONTREAD, file->name);
		}
		return (-1);
	}
	if (LE32TOH (version) == 0x00010001) 
	{
		pibseek (file, flags);
	}
	return (0);
}


#endif

