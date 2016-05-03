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
 *   signed FactoryDefaults2 (struct int6k * int6k);
 * 
 *   int6k2.h
 *
 *   Reset an INT6000 using a ihpapi_ResetToFactoryDefaults API function call then 
 *   print all responses;
 *
 *   See the HomePlugAV Firmware Technical Reference Manual
 *   for more information;
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
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef FACTORYDEFAULTS2_SOURCE
#define FACTORYDEFAULTS2_SOURCE

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../tools/error.h"
#include "../ihpapi/ihpapi.h"
#include "../int6k/int6k.h"

signed FactoryDefaults2 (struct int6k * int6k) 

{
	Request (int6k, "Restore Factory Defaults");
	int6k->packetsize = (int)(ihpapi_ResetToFactoryDefaults (int6k->channel->host, int6k->channel->peer, sizeof (struct message), (uint8_t *)(int6k->message)));
	if (int6k->packetsize == 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "wrong packet size");
		return (-1);
	}
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME2 (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
		return (-1);
	}
	if (int6k->result->opStatus.status) 
	{
		Failure (int6k, "Restore failed");
		return (-1);
	}
	Confirm (int6k, "Restored.");
	return (0);
}

#endif

