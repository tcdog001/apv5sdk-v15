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
 *   page.h - page related definitions and declarations;
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef SUBMIT_HEADER
#define SUBMIT_HEADER
 
/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdint.h>
 
/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"
 
/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define PAGE_PATH "/cgi-bin/intellon.cgi"
#define PAGE_NAME "Intellon Embedded Device Manager"
#define PAGE_STYLESHEET "/styles/cgi.css"
#define PAGE_LOGO_PATH "/images/intellon.gif"
#define PAGE_LOGO_TEXT "Intellon Corporation, Ocala FL USA"
 
#define SUBMIT_OPTION "option"
#define SUBMIT_ACTION "action"
#define SUBMIT_SOURCE "source"
#define SUBMIT_DEVICE "device"
#define SUBMIT_RETURN "return"

#define OPTION_TOPOLOGY "Topology"
#define OPTION_IDENTITY "Identity"
#define OPTION_SECURITY "Security"
#define OPTION_PRIORITY "Priority"
#define OPTION_FIRMWARE "Firmware"

#define ACTION_NETWORK "Change"
#define ACTION_PRIORITIZE "Prioritize"
#define ACTION_CONFIGURE "Configure"
#define ACTION_UPGRADE "Upgrade"
#define ACTION_DEFAULT "Restore"
#define ACTION_RETURN "Return"
#define ACTION_SOURCE "Source"
#define ACTION_DEVICE "Device"

#define STYLE_HEADER "header"
#define STYLE_OPTION "option"
#define STYLE_ACTION "action"
#define STYLE_STATUS "status"
#define STYLE_DEVICE "device"
#define STYLE_SOURCE "source"
#define STYLE_FOOTER "footer"
#define STYLE_PEEK "peek"
#define STYLE_FILE "file"
#define STYLE_PWD "pwd"

#define FIELD_DPW "dpw"
#define FIELD_DAK "dak"
#define FIELD_NPW "npw"
#define FIELD_NMK "nmk"
#define FIELD_PIB "pib"
#define FIELD_NVM "nvm"
#define FIELD_VLAN "vlan"
#define FIELD_QOS "qos"
#define FIELD_TOS "tos"
#define FIELD_TTL "ttl"
#define FIELD_PRI "pri"
#define FIELD_KEY "key"

#define TABS 5
#define CAPS 4
#define TTLS 4
#define TAGS 8
#define BITS 8
 
/*====================================================================*
 *   option tab structure;
 *--------------------------------------------------------------------*/
 
typedef struct tab 

{
	signed (* form) (signed margin, const char * style);
	const char * name;
	const char * text;
}

FORM;

/*====================================================================*
 *   generic form division functions;
 *--------------------------------------------------------------------*/

#ifdef __GNUC__
 
__attribute__ ((format (printf, 3, 4)))

#endif
 
void status (int status, errno_t number, const char *format, ...);
signed report (signed margin, errno_t number, const char * format, ...);

/*====================================================================*
 *   standard form division;
 *--------------------------------------------------------------------*/
 
signed DivisionHeader (signed margin, const char * style);
signed DivisionOption (signed margin, const char * style);
signed DivisionDevice (signed margin, const char * style);
signed DivisionFooter (signed margin, const char * style);

/*====================================================================*
 *   special form divisions;
 *--------------------------------------------------------------------*/
 
signed OptionTopology (signed margin, const char * style);
signed OptionIdentity (signed margin, const char * style);
signed OptionSecurity (signed margin, const char * style);
signed OptionPriority (signed margin, const char * style);
signed OptionFirmware (signed margin, const char * style);

/*====================================================================*
 *   generic form control functions;
 *--------------------------------------------------------------------*/
 
signed stylesheet (signed margin);

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif
 

