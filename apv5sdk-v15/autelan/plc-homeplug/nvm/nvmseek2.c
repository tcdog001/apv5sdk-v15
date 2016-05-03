/*====================================================================*
 *
 *   signed nvmseek2 (struct _file_ const * file, struct nvm_header2 * nvm_header, uint32_t imagetype);
 *
 *   nvm.h
 *
 *   search a panther/lynx PIB file for the next image of a given 
 *   type; return 0 on success or -1 on failure to find another
 *   image of the given type; 
 *
 *   the call must provide an image header strucuture for use while
 *   searching; on success, that structure will contain information
 *   about the image and file will be positioned to the start of
 *   the image;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *   
 *--------------------------------------------------------------------*/

#ifndef NVMSEEK2_SOURCE
#define NVMSEEK2_SOURCE

#include <unistd.h>

#include "../tools/endian.h"
#include "../tools/error.h"
#include "../pib/pib.h"
#include "../nvm/nvm.h"

signed nvmseek2 (struct _file_ const * file, struct nvm_header2 * nvm_header, uint32_t imagetype) 

{
	size_t origin = ~0;
	size_t offset = 0;
	signed module = 0;
	do 
	{
		if (read (file->file, nvm_header, sizeof (* nvm_header)) != sizeof (* nvm_header)) 
		{
			error (1, errno, NVM_HDR_CANTREAD, file->name, module);
		}
		if (LE16TOH (nvm_header->MajorVersion) != 1) 
		{
			error (1, 0, NVM_HDR_VERSION, file->name, module);
		}
		if (LE16TOH (nvm_header->MinorVersion) != 1) 
		{
			error (1, 0, NVM_HDR_VERSION, file->name, module);
		}
		if (checksum32 (nvm_header, sizeof (* nvm_header), 0)) 
		{
			error (1, 0, NVM_HDR_CHECKSUM, file->name, module);
		}
		if (LE32TOH (nvm_header->PrevHeader) != origin) 
		{
			error (1, 0, NVM_HDR_LINK, file->name, module);
		}
		if (LE32TOH (nvm_header->ImageType) == imagetype) 
		{
			return (0);
		}
		if (fdchecksum32 (file->file, LE32TOH (nvm_header->ImageLength), nvm_header->ImageChecksum)) 
		{
			error (1, ECANCELED, NVM_IMG_CHECKSUM, file->name, module);
		}
		origin = offset;
		offset = LE32TOH (nvm_header->NextHeader);
		module++;
	}
	while (~nvm_header->NextHeader);
	return (-1);
}

#endif

