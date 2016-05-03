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
 *   signed stylesheet (signed margin) ;
 *
 *   print CSS2 style block on stdout; this ensures that class names
 *   match those used in the output;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef STYLESHEET_SOURCE
#define STYLESHEET_SOURCE

#include <stdio.h>

#include "../tools/format.h"
#include "../tools/markup.h"
#include "../cgi/page.h"

signed stylesheet (signed margin) 

{
	output (margin, "body { background: white; font: normal 10pt monospace; }");
	output (margin, "form { border: solid 1px black; margin: 2px; }");
	output (margin, "button { border: none; margin: 0px; background: silver; width: 100px; }");
	output (margin, "fieldset { border: solid 1px silver; }");
	output (margin, "div { border: solid 1px silver; margin: 2px; }");
	output (margin, "legend { font: bold 10pt sanserif; }");
	output (margin, "table { font: normal 10pt monospace; }");
	output (margin, "tr { vertical-align: top; }");
	output (margin, "th, td { padding: 2px 5px; }");
	output (margin, "td.lite { background: aqua; }");
	output (margin, "td.dark { background: silver; }");
	output (margin, "div.%s { text-align: center; }", STYLE_HEADER);
	output (margin, "div.%s { min-height: 325px; }", STYLE_ACTION);
	output (margin, "div.%s { min-height: 325px; }", STYLE_STATUS);
	output (margin, "div.%s { text-align: center; }", STYLE_FOOTER);
	output (margin, "h2.%s { margin: 20px 20px; }", STYLE_STATUS);
	output (margin, "img.%s { border: none; align: center; margin: 5px; }", STYLE_HEADER);
	output (margin, "img.%s { border: none; align: center; width: 100%%; }", STYLE_FOOTER);
	output (margin, "input { border:none; background: beige; }");
	output (margin, "input.%s { width: 150px; }", FIELD_QOS);
	output (margin, "input.%s { width: 75px; }", FIELD_VLAN);
	output (margin, "input.%s { width: 75px; }", FIELD_TOS);
	output (margin, "input.%s { width: 75px; }", FIELD_TTL);
	output (margin, "input.%s { width: 300px; padding: 2px 5px; font: normal 10pt monospace; }", STYLE_PWD);

#ifdef STYLE_KEY

	output (margin, "input.%s { width: 400px; font: normal 10pt monospace; }", STYLE_KEY);

#endif

	output (margin, "input.%s { min-width: 600px; }", STYLE_FILE);
	output (margin, "input.%s { background: silver; width: 100px; }", STYLE_OPTION);
	output (margin, "input.%s { background: silver; width: 100px; }", STYLE_ACTION);
	output (margin, "input.%s { background: silver; width: 100px; }", STYLE_DEVICE);
	output (margin, "input.%s { background: silver; width: 100px; }", STYLE_SOURCE);
	output (margin, "select.%s { font: normal 10pt monospace; }", STYLE_SOURCE);
	output (margin, "select.%s { font: normal 10pt monospace; }", STYLE_DEVICE);
	output (margin, "input.%s { background: none; }", STYLE_PEEK);
	return (margin);
}

#endif

