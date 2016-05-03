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
 *   void status (int status, errno_t number, const char *format, ...);
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef OOPS_SOURCE
#define OOPS_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../tools/types.h"
#include "../tools/format.h"
#include "../cgi/page.h"

#ifdef __GNUC__

__attribute__ ((format (printf, 3, 4))) 

#endif

signed report (signed margin, errno_t number, const char *format, ...) 

{
	output (margin++, "<div class='%s'>", STYLE_STATUS);
	output (margin++, "<h2 class='%s'>", STYLE_STATUS);
	if (number) 
	{
		printf ("%s: ", strerror (number));
	}
	if ((format) && (*format)) 
	{
		va_list arglist;
		va_start (arglist, format);
		vfprintf (stdout, format, arglist);
		va_end (arglist);
	}
	output (margin--, "</h2>");
	output (margin--, "</div");
	return (margin);
}

void status (int status, errno_t number, const char *format, ...) 

{
	signed margin = 0;
	margin = DivisionHeader (margin, STYLE_HEADER);
	margin = DivisionOption (margin, STYLE_OPTION);
	output (margin++, "<div class='%s'>", STYLE_STATUS);
	output (margin++, "<h2 class='%s'>", STYLE_STATUS);
	if (number) 
	{
		printf ("%s: ", strerror (number));
	}
	if ((format) && (*format)) 
	{
		va_list arglist;
		va_start (arglist, format);
		vfprintf (stdout, format, arglist);
		va_end (arglist);
	}
	output (margin, "<input type='submit' class='%s' name='%s' value='%s' title=' Return to Operation '>", STYLE_STATUS, SUBMIT_ACTION, ACTION_RETURN);
	output (margin--, "</h2>");
	output (margin--, "</div");
	margin = DivisionDevice (margin, STYLE_DEVICE);
	margin = DivisionFooter (margin, STYLE_FOOTER);
	exit (1);
}

#endif

