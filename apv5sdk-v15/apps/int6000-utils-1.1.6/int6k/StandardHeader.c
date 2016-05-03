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
 *   signed StandardHeader (struct header_std * header, uint16_t MMTYPE);
 *
 *   message.h
 *
 *   encode memory with a standard HomePlug AV header having HomePlug 
 *   protocol version (HOMEPLUG_MMV) and HomePlug message type (MMTYPE); 
 *   return the number of bytes actually encoded;
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
 *	Charles Maier, charles.maier@intellon.com
 *
 *--------------------------------------------------------------------*/

#ifndef STANDARDHEADER_SOURCE
#define STANDARDHEADER_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/message.h"
#include "../int6k/homeplug.h"
#include "../tools/memory.h"

signed StandardHeader (struct header_std * header, uint16_t MMTYPE) 

{
	header->MMV = HOMEPLUG_MMV;
	header->MMTYPE = HTOLE16 (MMTYPE);
	return (sizeof (struct header_std));
}

#endif

