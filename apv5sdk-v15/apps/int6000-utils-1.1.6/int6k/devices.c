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
 *   devices.c - device address synonym tables
 *
 *   int6k.h
 *
 *   some device addresses are so common or significant that we give
 *   them names; users can enter the name instead of the address; all
 *   toolkit programs reference this table via function synonym();
 *
 *   function synonym() uses a binary search to lookup symbols and so
 *   they must be entered in alphabetical order or lookup errors will 
 *   occur;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef DEVICES_SOURCE
#define DEVICES_SOURCE

/*====================================================================*
 *   custom include files;
 *--------------------------------------------------------------------*/

#include "../int6k/int6k.h"
#include "../tools/types.h"

/*====================================================================*
 *   variables;
 *--------------------------------------------------------------------*/

const struct _term_ devices [DEVICES] = 

{
	{
		"all",
		BROADCAST 
	},
	{
		"broadcast",
		BROADCAST 
	},
	{
		"local",
		LOCALCAST 
	}
};

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#endif

