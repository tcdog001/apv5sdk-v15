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
 *   signed OptionPriority (signed margin, const char * style);
 *   
 *   page.h
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef OPTIONPRIORITY_SOURCE
#define OPTIONPRIORITY_SOURCE

#include <stdio.h>

#include "../tools/format.h"
#include "../cgi/page.h"
#include "../cgi/session.h"

static const char * labels [CAPS] = 

{
	"CAP 0",
	"CAP 1",
	"CAP 2",
	"CAP 3" 
};

static const signed values [CAPS] = 

{
	0,
	1,
	2,
	3 
};

/*====================================================================*
 *   
 *   signed checkbox (signed margin, const char * style, const char * label, signed check);
 *   
 *   page.h
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

signed checkbox (signed margin, const char * style, const char * label, signed check) 

{
	output (margin, "<input type='checkbox' class='%s' name='%s' %s/>", style, label, (check)? "checked": "");
	return (margin);
}

/*====================================================================*
 *   
 *   signed dropbox (signed margin, const char * label, signed value, const char * labels [], const signed values [], signed count);
 *
 *   page.h
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   margin is the indent level;
 *   label is the control name as a string;
 *   value is the control value in the range 0 through count-1;
 *   labels [] is a list of lables to display;
 *   count is the number of dropbox items;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

static signed dropbox (signed margin, const char * style, const char * label, signed value, const char * labels [], const signed values [], signed count) 

{
	unsigned index = 0;
	output (margin++, "<select class='%s' name='%s'>", style, label);
	while (index < count) 
	{
		char * state;
		if (index == value) 
		{
			state = "selected";
		}
		else 
		{
			state = "";
		}
		output (margin++, "<option class='%s' value='%d' %s>", style, values [index], state);
		output (margin, "%s", labels [index]);
		output (margin--, "</option>");
		index++;
	}
	output (margin--, "</select>");
	return (margin);
}

/*====================================================================*
 *   
 *   signed VLANTags (signed margin, const char * style);
 * 
 *   print a vertcal array of 8 VLAN tag selectors with title;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

static signed VLANTags (signed margin, const char * style) 

{
	extern struct session session;
	unsigned tag = 0;
	output (margin++, "<fieldset class='%s'>", style);
	output (margin++, "<legend class='%s'>", style);
	output (margin, "%s", "VLAN Tags");
	output (margin--, "</legend>");
	output (margin++, "<table class='%s'>", style);

#if 1

	while (tag < TAGS) 
	{
		char label [36];
		output (margin++, "<tr class='%s'>", style);
		snprintf (label, sizeof (label), "%s%d", FIELD_VLAN, tag);
		output (margin++, "<th class='%s'>", style);
		output (margin, "Tag%d", tag);
		output (margin--, "</th>");
		output (margin++, "<td class='%s'>", style);
		margin = dropbox (margin, style, label, session.QoS.VLAN [tag], labels, values, CAPS);
		output (margin--, "</td>");
		output (margin--, "</tr>");
		tag++;
	}

#else

	while (tag < (TAGS>>1)) 
	{
		char label [36];
		output (margin++, "<tr class='%s'>", style);
		snprintf (label, sizeof (label), "%s%d", FIELD_VLAN, tag);
		output (margin++, "<th class='%s'>", style);
		output (margin, "Tag%d", tag);
		output (margin--, "</th>");
		output (margin++, "<td class='%s'>", style);
		margin = dropbox (margin, style, label, session.tag [tag], labels, values, CAPS);
		output (margin--, "</td>");
		snprintf (label, sizeof (label), "%s%d", style, tag + (TAGS>>1));
		output (margin++, "<th class='%s'>", style);
		output (margin, "Tag%d", tag + (TAGS>>1));
		output (margin--, "</th>");
		output (margin++, "<td class='%s'>", style);
		margin = dropbox (margin, style, label, session.tag [tag + (TAGS>>1)], labels, values, CAPS);
		output (margin--, "</td>");
		output (margin--, "</tr>");
		tag++;
	}

#endif

	output (margin--, "</table>");
	output (margin--, "</fieldset>");
	return (margin);
}

/*====================================================================*
 *   
 *   signed TOSBits (signed margin, const char * style);
 *
 *   print a vertical array of 8 TOS bit selectors with title;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed TOSBits (signed margin, const char * style) 

{
	extern struct session session;
	unsigned bit = 0;
	output (margin++, "<fieldset class='%s'>", style);
	output (margin++, "<legend class='%s'>", style);
	output (margin, "%s", "TOS Bits");
	output (margin--, "</legend>");
	output (margin++, "<table class='%s'>", style);

#if 1

	while (bit < BITS) 
	{
		char label [36];
		output (margin++, "<tr class='%s'>", style);
		snprintf (label, sizeof (label), "%s%d", FIELD_TOS, bit);
		output (margin++, "<th class='%s'>", style);
		output (margin, "Bit%d", bit);
		output (margin--, "</th>");
		output (margin++, "<td class='%s'>", style);
		margin = dropbox (margin, style, label, session.QoS.ToS [bit], labels, values, CAPS);
		output (margin--, "</td>");
		output (margin--, "</tr>");
		bit++;
	}

#else

	while (bit < (BITS>>1)) 
	{
		char label [36];
		output (margin++, "<tr class='%s'>", style);
		snprintf (label, sizeof (label), "%s%d", FIELD_TOS, bit);
		output (margin++, "<th class='%s'>", style);
		output (margin, "Bit%d", bit);
		output (margin--, "</th>");
		output (margin++, "<td class='%s'>", style);
		margin = dropbox (margin, style, label, session.tos [bit], labels, values, CAPS);
		output (margin--, "</td>");
		snprintf (label, sizeof (label), "%s%d", style, bit + (BITS>>1));
		output (margin++, "<th class='%s'>", style);
		output (margin, "Bit%d", bit + (BITS>>1));
		output (margin--, "</th>");
		output (margin++, "<td class='%s'>", style);
		margin = dropbox (margin, style, label, session.tos [bit + (BITS>>1)], labels, values, CAPS);
		output (margin--, "</td>");
		output (margin--, "</tr>");
		bit++;
	}

#endif

	output (margin--, "</table>");
	output (margin--, "</fieldset>");
	return (margin);
}

/*====================================================================*
 *   
 *   signed TimeToLive (signed margin, const char * style);
 *
 *   print a vertical array of 8 TOS bit selectors with title;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed TimeToLive (signed margin, const char * style) 

{
	extern struct session session;
	unsigned ttl = 0;
	output (margin++, "<fieldset class='%s'>", style);
	output (margin++, "<legend class='%s' title='10-65000msec'>", style);
	output (margin, "%s", "Priority TTL");
	output (margin--, "</legend>");
	output (margin++, "<table class='%s'>", style);
	while (ttl < TTLS) 
	{
		char label [36];
		snprintf (label, sizeof (label), "%s%d", FIELD_TTL, ttl);
		output (margin++, "<tr class='%s'>", style);
		output (margin++, "<th class='%s'>", style);
		output (margin, "CA%d", ttl);
		output (margin--, "</th>");
		output (margin++, "<td class='%s'>", style);
		output (margin, "<input type='text' class='%s' name='%s' value='%d'/>", style, label, session.QoS.TTL [ttl]);
		output (margin--, "</td>");
		output (margin--, "</tr>");
		ttl++;
	}
	output (margin--, "</table>");
	output (margin--, "</fieldset>");
	return (margin);
}

/*====================================================================*
 *   
 *   signed ChannelPriority (signed margin, const char * style);
 *
 *   print a vertical array of 8 TOS bit selectors with title;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed ChannelPriority (signed margin, const char * style) 

{
	extern struct session session;
	session.QoS.method [0] >>= 1;
	session.QoS.method [1] >>= 1;
	output (margin++, "<fieldset class='%s'>", style);
	output (margin++, "<legend class='%s'>", style);
	output (margin, "%s", "Prioritize Using");
	output (margin--, "</legend>");
	output (margin++, "<table class='%s'>", style);
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<td class='%s'>", style);
	margin = checkbox (margin, style, FIELD_PRI "0", session.QoS.method [0]);
	output (margin++, "<label class='%s'>", style);
	output (margin, "%s", "VLAN Tags");
	output (margin--, "</label>");
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<td class='%s'>", style);
	margin = checkbox (margin, style, FIELD_PRI "1", session.QoS.method [1]);
	output (margin++, "<label class='%s'>", style);
	output (margin, "%s", "Traffic Class");
	output (margin--, "</label>");
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin--, "</table>");
	output (margin--, "</fieldset>");
	return (margin);
}

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

signed OptionPriority (signed margin, const char * style) 

{
	output (margin++, "<div class='%s'>", style);
	output (margin++, "<table class='%s'>", style);
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<td class='%s'>", style);
	margin = VLANTags (margin, FIELD_VLAN);
	output (margin--, "</td>");
	output (margin++, "<td class='%s'>", style);
	margin = TOSBits (margin, FIELD_TOS);
	output (margin--, "</td>");
	output (margin++, "<td class='%s'>", style);
	margin = TimeToLive (margin, FIELD_TTL);
	margin = ChannelPriority (margin, FIELD_PRI);
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin++, "<tr class='%s'>", style);
	output (margin++, "<td class='%s'>", style);
	output (margin, "<input type='submit' class='%s' name='%s' value='%s' title=' Apply QoS Parameters '/>", style, SUBMIT_ACTION, ACTION_PRIORITIZE);
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

