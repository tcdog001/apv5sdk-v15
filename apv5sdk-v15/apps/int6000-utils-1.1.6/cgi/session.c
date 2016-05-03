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
 *   session.c - global session variable declaration;
 *
 *   session.h
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef SESSION_SOURCE
#define SESSION_SOURCE

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/number.h"
#include "../tools/memory.h"
#include "../tools/files.h"
#include "../cgi/session.h"
#include "../cgi/page.h"

/*====================================================================*
 *  
 *--------------------------------------------------------------------*/

struct fileinfo pib;
struct fileinfo nvm;

/*====================================================================*
 *  
 *--------------------------------------------------------------------*/

struct message message;

/*====================================================================*
 *   custom variables;
 *--------------------------------------------------------------------*/

struct session session = 

{
	OPTION_TOPOLOGY,
	"",
	SESSION_NIC,
	{
		{
			{
				0x00,
				0xB0,
				0x52,
				0x00,
				0x00,
				0x01 
			}
		}
	},
	0,
	{
	},
	{
		"HomePlugAV" 
	},
	{
		"HomePlugAV" 
	},
	{
		{
			2000000,
			2000000,
			3000000,
			3000000 
		},
		{
			1,
			1 
		},
		{
			1,
			2,
			3,
			0,
			0,
			0,
			0,
			0 
		},
		{
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0 
		},
	}
};

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

