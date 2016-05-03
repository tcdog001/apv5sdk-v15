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
 *   void SessionEdit ();
 *
 *   session.h
 *   
 *   read a multipart/form-data message from stdin and update the 
 *   cgi session variable; 
 *
 *   the entire message is read into the control scanner and parsed
 *   into lines; lines are then copied into the content scanner and
 *   parsed into keyword and value tokens; content lines are parsed
 *   and processed by function MultipartContent;
 *
 *   See RFC-2338 "Returning Values from Forms: multipart/form-data"
 *   for an explanation of the multipart form-data message format;
 *   
 *   environment variable CONTENT_LENGTH contains the length of the
 *   HTTP_POST frame; read the entire frame into the control buffer
 *   for scanning;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef SESSIONEDIT_SOURCE
#define SESSIONEDIT_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>
#include <ctype.h>

#include "../tools/error.h"
#include "../tools/files.h"
#include "../tools/format.h"
#include "../tools/memory.h"
#include "../ether/ether.h"
#include "../scan/scan.h"
#include "../cgi/session.h"
#include "../cgi/page.h"
#include "../pib/pib.h"
#include "../nvm/nvm.h"

void SessionEdit () 

{
	extern struct session session;
	extern struct fileinfo pib;
	extern struct fileinfo nvm;
	char address [ETHER_ADDR_LEN * 3];
	struct _scan_ control;
	struct _scan_ content;
	char dataname [CGI_DATANAME_LENGTH];
	char filename [CGI_FILENAME_LENGTH];
	char * string = getenv ("CONTENT_LENGTH");
	signed length = 0;
	if (!string) 
	{
		string = "";
	}
	while (isdigit (*string)) 
	{
		length *= 10;
		length += *string - '0';
		string ++;
	}
	string = malloc (length);
	if (!string) 
	{
		string = "";
	}
	scaninput (&control, string, length);
	while (control.index < control.limit) 
	{
		control.index += read (STDIN_FILENO, (char *)(control.index), CGI_DATA_LENGTH);
	}

#if 1

	signed fd = open (SESSION_LOG, O_CREAT|O_APPEND|O_WRONLY);
	write (fd, control.start, control.index - control.start);
	write (fd, "\n", 1);
	close (fd);

#endif

	while (isready (&control)) 
	{
		scanflush (&control);
		scanchunk (&control);
		if (istoken (&control, "\r\n")) 
		{
			scanflush (&control);
			scanchunk (&control);
			if (istoken (&control, "\r\n")) 
			{
				continue;
			}
			control.final--;
			control.final--;
			if (!strcmp (dataname, SUBMIT_RETURN)) 
			{
			}
			else if (!strcmp (dataname, SUBMIT_OPTION)) 
			{
				copytoken (&control, session.option, sizeof (session.option));
			}
			else if (!strcmp (dataname, SUBMIT_ACTION)) 
			{
				copytoken (&control, session.action, sizeof (session.action));
			}
			else if (!strcmp (dataname, SUBMIT_SOURCE)) 
			{
				copytoken (&control, session.source, sizeof (session.source));
			}
			else if (!strcmp (dataname, SUBMIT_DEVICE)) 
			{
				copytoken (&control, address, sizeof (address));
				hexencode (session.device.MAC, sizeof (session.device.MAC), address);
			}
			else if (!strcmp (dataname, FIELD_DPW)) 
			{
				copytoken (&control, session.DPW, sizeof (session.DPW));
				HPAVKeyDAK (session.device.DAK, session.DPW);
			}
			else if (!strcmp (dataname, FIELD_NPW)) 
			{
				copytoken (&control, session.NPW, sizeof (session.NPW));
				HPAVKeyNMK (session.device.NMK, session.NPW);
				HPAVKeyNID (session.device.NID, session.device.NMK, 0);
			}
			else if (!strcmp (dataname, FIELD_VLAN "0")) 
			{
				session.QoS.VLAN [0] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_VLAN "1")) 
			{
				session.QoS.VLAN [1] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_VLAN "2")) 
			{
				session.QoS.VLAN [2] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_VLAN "3")) 
			{
				session.QoS.VLAN [3] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_VLAN "4")) 
			{
				session.QoS.VLAN [4] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_VLAN "5")) 
			{
				session.QoS.VLAN [5] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_VLAN "6")) 
			{
				session.QoS.VLAN [6] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_VLAN "7")) 
			{
				session.QoS.VLAN [7] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TOS "0")) 
			{
				session.QoS.ToS [0] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TOS "1")) 
			{
				session.QoS.ToS [1] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TOS "2")) 
			{
				session.QoS.ToS [2] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TOS "3")) 
			{
				session.QoS.ToS [3] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TOS "4")) 
			{
				session.QoS.ToS [4] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TOS "5")) 
			{
				session.QoS.ToS [5] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TOS "6")) 
			{
				session.QoS.ToS [6] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TOS "7")) 
			{
				session.QoS.ToS [7] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TTL "0")) 
			{
				session.QoS.TTL [0] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TTL "1")) 
			{
				session.QoS.TTL [1] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TTL "2")) 
			{
				session.QoS.TTL [2] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_TTL "3")) 
			{
				session.QoS.TTL [3] = tovalue (&control);
			}
			else if (!strcmp (dataname, FIELD_PRI "0")) 
			{
				if (istoken (&control, "on")) 
				{
					session.QoS.method [0] |= 2;
				}
			}
			else if (!strcmp (dataname, FIELD_PRI "1")) 
			{
				if (istoken (&control, "on")) 
				{
					session.QoS.method [1] |= 2;
				}
			}
			else if (!strcmp (dataname, FIELD_PIB)) 
			{
				struct header_pib * header_pib = (struct header_pib *)(control.first);
				strcpy (pib.name, filepart (filename));
				pib.size = LE16TOH (header_pib->PIBLENGTH);
				pib.data = control.first;
				if (pib.size != tokensize (&control)) 
				{
					status (0, EPERM, "PIB file %s has wrong size", pib.name);
				}
				if (checksum_32 (header_pib, pib.size, 0)) 
				{
					status (0, EPERM, "PIB file %s has bad checksum", pib.name);
				}
				control.final = control.first + pib.size;
			}
			else if (!strcmp (dataname, FIELD_NVM)) 
			{
				struct header_nvm * header_nvm = (struct header_nvm *)(control.first);
				strcpy (nvm.name, filepart (filename));
				nvm.size = nvmspan (header_nvm);
				nvm.data = control.first;
				if (!nvm.size) 
				{
					status (0, EPERM, "NVM file %s has wrong structure", nvm.name);
				}
				control.final = control.first + nvm.size;
			}
			control.final++;
			control.final++;
			continue;
		}
		scaninput (&content, control.first, control.final - control.first);
		scantoken (&content);
		if (istoken (&content, "--")) 
		{
			scanflush (&content);
			scanident (&content);
			continue;
		}
		if (havetoken (&content, "Content-Disposition")) 
		{
			memset (dataname, 0, sizeof (dataname));
			memset (filename, 0, sizeof (filename));
			if (!havetoken (&content, ":")) 
			{
				continue;
			}
			if (!havetoken (&content, "form-data")) 
			{
				error (0, EINVAL, "expected form-data content type");
				continue;
			}
			if (!havetoken (&content, ";")) 
			{
				continue;
			}
			if (!havetoken (&content, "name")) 
			{
				error (0, EINVAL, "expected control name");
				continue;
			}
			if (!havetoken (&content, "=")) 
			{
				continue;
			}
			copyquote (&content, dataname, sizeof (dataname));
			nexttoken (&content);
			if (!havetoken (&content, ";")) 
			{
				continue;
			}
			if (!havetoken (&content, "filename")) 
			{
				error (0, EINVAL, "expected file name");
				continue;
			}
			if (!havetoken (&content, "=")) 
			{
				continue;
			}
			copyquote (&content, filename, sizeof (filename));
			continue;
		}
		if (havetoken (&content, "Content-Type")) 
		{
			if (!havetoken (&content, ":")) 
			{
				continue;
			}
			if (!havetoken (&content, "application")) 
			{
				continue;
			}
			if (!havetoken (&content, "/")) 
			{
				continue;
			}
			if (!havetoken (&content, "octet-stream")) 
			{
				continue;
			}
			continue;
		}
	}
	return;
}

#endif

