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
 *   signed DivisionOption (signed margin, const char * style);
 *
 *   page.h 
 * 
 *   print the menu division on stdout; automatically disable the tab
 *   control for the current session state;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef DIVISIONOPTION_SOURCE
#define DIVISIONOPTION_SOURCE

#include <string.h>

#include "../tools/format.h"
#include "../cgi/session.h"
#include "../cgi/page.h"

signed DivisionOption (signed margin, const char * style) 

{
	extern struct session session;
	extern struct tab tabs [TABS];
	unsigned tab;
	output (margin++, "<div class='%s'>", style);
	for (tab = 0; tab < TABS; tab++) 
	{
		if (!strcmp (tabs [tab].name, session.option)) 
		{
			output (margin, "<input class='%s' type='submit' name='%s' value='%s' title='%s' disabled>", style, SUBMIT_OPTION, tabs [tab].name, tabs [tab].text);
		}
		else
		{
			output (margin, "<input class='%s' type='submit' name='%s' value='%s' title='%s'>", style, SUBMIT_OPTION, tabs [tab].name, tabs [tab].text);
		}
	}
	output (margin--, "</div>");
	return (margin);
}

#endif

