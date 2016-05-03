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
 *   signed DivisionHeader (signed margin, const char * style);
 *   
 *   page.h
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef DIVISIONHEADER_SOURCE
#define DIVISIONHEADER_SOURCE

#include <stdio.h>

#include "../tools/markup.h"
#include "../tools/format.h"
#include "../cgi/page.h"

signed DivisionHeader (signed margin, const char * style) 

{
	output (margin, "content-type: %s\n", ISO_CONTENT);
	output (margin, "<?xml version='%s' encoding='%s'?>", XML_VERSION, XML_CHARSET);
	output (margin, "<!doctype html public '%s' '%s'>", W3C_STD, W3C_DTD);
	output (margin++, "<html xmlns='%s' lang='%s'>", XML_NAMESPACE, XML_LANGUAGE);
	output (margin++, "<head>");
	output (margin++, "<title>");
	output (margin, "%s", PAGE_NAME);
	output (margin--, "</title>");
	output (margin, "<meta http-equiv='content-type' content='%s'/>", HTML_CONTENT);
	output (margin, "<meta name='generator' content='%s'/>", HTML_PROGRAM);
	output (margin, "<meta name='author' content='%s'/>", HTML_AUTHOR);
	output (margin, "<link href='%s' rel='stylesheet' type='text/css'/>", PAGE_STYLESHEET);
	output (margin--, "</head>");
	output (margin++, "<body>");
	output (margin++, "<form action='%s' method='POST' enctype='multipart/form-data'>", PAGE_PATH);
	output (margin++, "<div class='%s'>", style);
	output (margin, "<img class='%s' src='%s' alt='%s'/>", style, PAGE_LOGO_PATH, PAGE_LOGO_TEXT);
	output (margin--, "</div>");
	return (margin);
}

#endif

