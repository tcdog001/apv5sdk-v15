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
 *   void Failure (struct int6k * int6k, const char *format, ...) 
 *
 *   error.h
 *
 *   Inform the user that an operation failed; print the channel name, 
 *   packet source device, error message and user defined message on 
 *   stderr unless the INT6K_SILENCE flags is set;
 *
 *   This function is identical to Confirm () excep that the message
 *   status code and description is output if the status is non-zero;
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
 *   Contributor(s);
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef FAILURE_SOURCE
#define FAILURE_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../tools/memory.h"
#include "../mme/mme.h"

#ifdef __GNUC__

__attribute__ ((format (printf, 2, 3))) 

#endif

void Failure (struct int6k * int6k, const char *format, ...) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
	if (_allclr (int6k->flags, INT6K_SILENCE)) 
	{
		char address [ETHER_ADDR_LEN * 3];
		struct header_confirm 
		{
			header_eth ethernet;
			header_int intellon;
			uint8_t MSTATUS;
		}
		* header = (struct header_confirm *)(message);
		struct __packed header_confirm_request 
		{
			struct header_eth ethernet;
			struct header_int intellon;
			uint32_t REQ_ID;
			uint8_t MSTATUS;
		}
		* header_confirm_request = (struct header_confirm_request *)(message);
		hexdecode (header->ethernet.OSA, sizeof (header->ethernet.OSA), address, sizeof (address));
		fprintf (stderr, "%s %s ", channel->name, address);
		switch (header->intellon.MMTYPE) 
		{
		case VS_ADD_CONN | MMTYPE_CNF:
		case VS_MOD_CONN | MMTYPE_CNF:
		case VS_REL_CONN | MMTYPE_CNF:
		case VS_CON_INFO | MMTYPE_CNF:
			if (header_confirm_request->MSTATUS) 
			{
				fprintf (stderr, "%s (0x%02X): ", MMECode (header_confirm_request->intellon.MMTYPE, header_confirm_request->MSTATUS), header_confirm_request->MSTATUS);
			}
			break;
		default:
			if (header->MSTATUS) 
			{
				fprintf (stderr, "%s (0x%02X): ", MMECode (header->intellon.MMTYPE, header->MSTATUS), header->MSTATUS);
			}
			break;
		}
		if ((format) && (*format)) 
		{
			va_list arglist;
			va_start (arglist, format);
			vfprintf (stderr, format, arglist);
			va_end (arglist);
		}
		fprintf (stderr, "\n");
	}
	if (_anyset (int6k->flags, INT6K_BAILOUT)) 
	{
		if (_allclr (int6k->flags, INT6K_SILENCE)) 
		{
			error (1, 0, "Bailing Out!");
		}
		exit (1);
	}
	return;
}

#endif

