/*====================================================================*
 *
 *   signed pibseek (struct _file_ const * file, flag_t flags);
 *
 *   position a panther/lynx PIB file to the start of the parameter
 *   block image; return 0 on success or 1 for missing image; print 
 *   a message and exit the program if the image chain is not valid;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *   
 *--------------------------------------------------------------------*/

#ifndef PIBSEEK_SOURCE
#define PIBSEEK_SOURCE

#include <unistd.h>

#include "../tools/memory.h"
#include "../tools/endian.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../pib/pib.h"
#include "../nvm/nvm.h"

signed pibseek (struct _file_ const * file, flag_t flags) 

{
	struct nvm_header2 nvm_header;
	size_t origin = ~0;
	size_t offset = 0;
	signed module = 0;
	do 
	{
		if (read (file->file, &nvm_header, sizeof (nvm_header)) != sizeof (nvm_header)) 
		{
			if (_allclr (flags, PIB_SILENCE)) 
			{
				error (0, errno, NVM_HDR_CANTREAD, file->name, module);
			}
			return (-1);
		}
		if (LE16TOH (nvm_header.MajorVersion) != 1) 
		{
			if (_allclr (flags, PIB_SILENCE)) 
			{
				error (0, 0, NVM_HDR_VERSION, file->name, module);
			}
			return (-1);
		}
		if (LE16TOH (nvm_header.MinorVersion) != 1) 
		{
			if (_allclr (flags, PIB_SILENCE)) 
			{
				error (0, 0, NVM_HDR_VERSION, file->name, module);
			}
			return (-1);
		}
		if (checksum32 (&nvm_header, sizeof (nvm_header), 0)) 
		{
			if (_allclr (flags, PIB_SILENCE)) 
			{
				error (0, 0, NVM_HDR_CHECKSUM, file->name, module);
			}
			return (-1);
		}
		if (LE32TOH (nvm_header.PrevHeader) != origin) 
		{
			if (_allclr (flags, PIB_SILENCE)) 
			{
				error (0, 0, NVM_HDR_LINK, file->name, module);
			}
			return (-1);
		}
		if (LE32TOH (nvm_header.ImageType) == NVM_IMAGE_PIB) 
		{
			return (1);
		}
		if (fdchecksum32 (file->file, LE32TOH (nvm_header.ImageLength), nvm_header.ImageChecksum)) 
		{
			if (_allclr (flags, PIB_SILENCE)) 
			{
				error (0, 0, NVM_IMG_CHECKSUM, file->name, module);
			}
			return (-1);
		}
		origin = offset;
		offset = LE32TOH (nvm_header.NextHeader);
		module++;
	}
	while (~nvm_header.NextHeader);
	return (0);
}

#endif

