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
 *   void Confirm (struct int6k *int6k, const char * message);
 *   
 *   int6k.h
 *
 *   Inform the user that an operation has completed; print the channel 
 *   name, frame source address and user message on stdout unless the
 *   INT6K_SILENCE flag is set;
 *
 *   this function is identical to Request () except that it prints the
 *   packet source address instead of the template destination address;
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
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef CONFIRM_SOURCE
#define CONFIRM_SOURCE

#include <stdio.h>
#include <stdarg.h>

#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/flags.h"

#ifdef __GNUC__

__attribute__ ((format (printf, 2, 3))) 

#endif

void Confirm (struct int6k * int6k, const char *format, ...) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
	if (_allclr (int6k->flags, INT6K_SILENCE)) 
	{
		char address [ETHER_ADDR_LEN * 3];
		hexdecode (message->ethernet.OSA, sizeof (message->ethernet.OSA), address, sizeof (address));
		fprintf (stderr, "%s %s ", channel->name, address);
		if ((format) && (*format)) 
		{
			va_list arglist;
			va_start (arglist, format);
			vfprintf (stderr, format, arglist);
			va_end (arglist);
		}
		fprintf (stderr, "\n");
	}
	return;
}

#endif

