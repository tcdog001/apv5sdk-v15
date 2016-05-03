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
 *   signed OptionSecurity (signed margin, const char * style);
 * 
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef OPTIONSECURITY_SOURCE
#define OPTIONSECURITY_SOURCE

#include "../cgi/page.h"
#include "../cgi/session.h"
#include "../tools/memory.h"
#include "../crypt/HPAVKey.h"
#include "../tools/format.h"

#define PASSWORD "HomePlugAV"
#define DISABLED "disabled value='not required'"

static signed passwords (signed margin, const char * style) 

{
	extern struct session session;

#ifdef STYLE_KEY

	char string [HPAVKEY_DAK_LEN * 3];

#endif

	unsigned bridge;
	for (bridge = 0; bridge < session.bridges; bridge++) 
	{
		if (!memcmp (session.device.MAC, session.bridge [bridge].MAC, ETHER_ADDR_LEN)) 
		{
			break;
		}
	}
	output (margin++, "<fieldset class='%s'>", style);
	output (margin++, "<legend class='%s'>", style);
	output (margin, "%s", "Passwords");
	output (margin--, "</legend>");
	output (margin++, "<table class='%s'>", style);
	if (bridge == session.bridges) 
	{
		output (margin++, "<tr class='%s'>", style);
		output (margin++, "<th class='%s'>", style);
		output (margin, "%s", "Station");
		output (margin--, "</th>");
		output (margin++, "<td class='%s'>", style);
		output (margin, "<input type='text' maxlength='64' class='%s' name='%s'>", STYLE_PWD, FIELD_DPW);
		output (margin--, "</td>");

#ifdef STYLE_KEY

		output (margin++, "<td class='%s'>", style);
		output (margin, "<input type='text' class='%s' name='%s' value='%s' readonly>", STYLE_KEY, FIELD_DAK, hexstring (string, sizeof (string), session.device.DAK, sizeof (session.device.DAK)));
		output (margin--, "</td>");

#endif

		output (margin--, "</tr>");
	}
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<th class='%s'>", style);
	output (margin, "%s", "Network");
	output (margin--, "</th>");
	output (margin++, "<td class='%s'>", style);
	output (margin, "<input type='text' maxlength='64' class='%s' name='%s'>", STYLE_PWD, FIELD_NPW);
	output (margin--, "</td>");

#ifdef STYLE_KEY

	output (margin++, "<td class='%s'>", style);
	output (margin, "<input type='text' class='%s' name='%s' value='%s' readonly>", STYLE_KEY, FIELD_NMK, hexstring (string, sizeof (string), session.device.NMK, sizeof (session.device.NMK)));
	output (margin--, "</td>");

#endif

	output (margin--, "</tr>");
	output (margin--, "</table>");
	output (margin--, "</fieldset>");
	return (margin);
}

/*====================================================================*
 *   
 *   signed OptionSecurity (signed margin, const char * style);
 *   
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

signed OptionSecurity (signed margin, const char * style) 

{
	output (margin++, "<div class='%s'>", style);
	output (margin++, "<table class='%s'>", style);
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<td class='%s'>", style);
	margin = passwords (margin, FIELD_KEY);
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<td class='%s'>", style);
	output (margin, "<input type='submit' class='%s' name='%s' value='%s' title=' Change Network Membership '>", style, SUBMIT_ACTION, ACTION_NETWORK);
	output (margin--, "</td>");
	output (margin++, "<td class='%s'>", style);
	output (margin, "%s", "&nbsp;");
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin--, "</table>");
	output (margin--, "</div>");
	return (margin);
}

#endif

