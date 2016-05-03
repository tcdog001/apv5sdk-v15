/*====================================================================*
 *
 *   signed nvmseek1 (struct _file_ const * file, struct nvm_header1 * nvm_header, uint32_t imagetype);
 *
 *   nvm.h
 *
 *   search a thunderbolt/lightning PIB file for the next image of a
 *   given type; return 0 on success or -1 on failure to find another
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

#ifndef NVMSEEK1_SOURCE
#define NVMSEEK1_SOURCE

#include <unistd.h>

#include "../tools/endian.h"
#include "../tools/error.h"
#include "../pib/pib.h"
#include "../nvm/nvm.h"

signed nvmseek1 (struct _file_ const * file, struct nvm_header1 * nvm_header, uint32_t imagetype) 

{
	signed module = 0;
	do 
	{
		if (read (file->file, nvm_header, sizeof (* nvm_header)) != sizeof (* nvm_header)) 
		{
			error (1, errno, NVM_HDR_CANTREAD, file->name, module);
		}
		if (LE32TOH (nvm_header->HEADERVERSION) != 0x60000000) 
		{
			error (1, errno, NVM_HDR_VERSION, file->name, module);
		}
		if (checksum32 (nvm_header, sizeof (* nvm_header), 0)) 
		{
			error (1, 0, NVM_HDR_CHECKSUM, file->name, module);
		}
		if (nvm_header->IMAGETYPE == imagetype) 
		{
			return (0);
		}
		if (fdchecksum32 (file->file, LE32TOH (nvm_header->IMAGELENGTH), nvm_header->IMAGECHECKSUM)) 
		{
			error (1, ECANCELED, NVM_IMG_CHECKSUM, file->name, module);
		}
		module++;
	}
	while (~nvm_header->NEXTHEADER);
	return (-1);
}

#endif

