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
 *   void SessionTask ();
 *
 *   session.h 
 *
 *   inspect session.action and perform indicated action only if the
 *   action is valid; clear session.action on exit;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef SESSIONTASK_SOURCE
#define SESSIONTASK_SOURCE

#include <errno.h>

#include "../cgi/session.h"
#include "../cgi/page.h"

void SessionTask () 

{
	extern struct session session;
	extern struct channel channel;
	extern struct fileinfo pib;
	extern struct fileinfo nvm;
	CGILocalPLD ();
	if (!strcmp (session.action, ACTION_SOURCE)) 
	{
		closechannel (&channel);
		channel.name = session.source;
		openchannel (&channel);
		CGILocalPLD ();
	}
	else if (!strcmp (session.action, ACTION_DEVICE)) 
	{
	}
	else if (!strcmp (session.action, ACTION_NETWORK)) 
	{
		CGISetKey ();
	}
	else if (!strcmp (session.action, ACTION_PRIORITIZE)) 
	{
		status (1, ENOSYS, "This is demonstration software.");
	}
	else if (!strcmp (session.action, ACTION_CONFIGURE)) 
	{
		if ((!pib.data) || (!pib.size)) 
		{
			status (1, ECANCELED, "No PIB file selected");
		}
		CGIWriteMOD (VS_MODULE_PIB, pib.data, pib.size);
		CGIFlashMOD (VS_MODULE_PIB);
		memset (&pib, 0, sizeof (pib));
		memset (&nvm, 0, sizeof (nvm));
	}
	else if (!strcmp (session.action, ACTION_UPGRADE)) 
	{
		if ((!pib.data) && (!pib.size)) 
		{
			status (1, ECANCELED, "No PIB file selected");
		}
		if ((!nvm.data) && (!nvm.size)) 
		{
			status (1, ECANCELED, "No NVM file selected");
		}
		CGIWriteMOD (VS_MODULE_PIB, pib.data, pib.size);
		CGIWriteMOD (VS_MODULE_MAC, nvm.data, nvm.size);
		CGIFlashMOD (VS_MODULE_MAC|VS_MODULE_PIB);
		memset (&pib, 0, sizeof (pib));
		memset (&nvm, 0, sizeof (nvm));
	}
	else if (!strcmp (session.action, ACTION_DEFAULT)) 
	{
		CGIRestore ();
	}
	memset (session.action, 0, sizeof (session.action));
	return;
}

#endif

