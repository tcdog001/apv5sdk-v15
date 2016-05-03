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
 *   signed OptionFirmware (signed margin, const char * style);
 *   
 *   page.h
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef OPTIONFIRMWARE_SOURCE
#define OPTIONFIRMWARE_SOURCE

#include "../tools/format.h"
#include "../cgi/page.h"

signed OptionFirmware (signed margin, const char * style) 

{
	output (margin++, "<div class='%s'>", style);
	output (margin++, "<table class='%s'>", style);
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<td class='%s'>", style);
	output (margin++, "<fieldset class='%s'>", style);
	output (margin++, "<legend class='%s'>", style);
	output (margin, "%s", "File Select");
	output (margin--, "</legend>");
	output (margin++, "<table class='%s'>", style);
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<th class='%s'>", style);
	output (margin, "PIB File");
	output (margin--, "</th>");
	output (margin++, "<td class='%s'>", style);
	output (margin, "<input type='file' name='%s'/>", FIELD_PIB);
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<th class='%s'>", style);
	output (margin, "NVM File");
	output (margin--, "</th>");
	output (margin++, "<td class='%s'>", style);
	output (margin, "<input type='file' name='%s'/>", FIELD_NVM);
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin--, "</table>");
	output (margin--, "</fieldset>");
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<td class='%s'>", style);
	output (margin, "<input type='submit' class='%s' name='%s' value='%s' title=' Change Configuration '>", style, SUBMIT_ACTION, ACTION_CONFIGURE);
	output (margin, "<input type='submit' class='%s' name='%s' value='%s' title=' Change Configuration and Firmware '>", style, SUBMIT_ACTION, ACTION_UPGRADE);
	output (margin, "<input type='submit' class='%s' name='%s' value='%s' title=' Restore Factory Defaults '>", style, SUBMIT_ACTION, ACTION_DEFAULT);
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin--, "</table>");
	output (margin--, "</div>");
	return (margin);
}

#endif

