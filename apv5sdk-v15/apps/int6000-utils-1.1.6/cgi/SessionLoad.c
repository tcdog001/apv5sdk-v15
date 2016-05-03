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
 *   void SessionLoad (const char * filename);
 *
 *   session.h
 *
 *   read session variable from named file; initialize the variable
 *   and try to save it if the file cannot be read;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef SESSIONLOAD_SOURCE
#define SESSIONLOAD_SOURCE

#include <unistd.h>    
#include <memory.h>
#include <fcntl.h>

#include "../cgi/page.h"
#include "../cgi/session.h"
#include "../ether/ether.h"

void SessionLoad (const char * filename) 

{
	extern struct session session;
	extern const byte localcast [ETHER_ADDR_LEN];
	int fd = open (filename, O_RDONLY);
	if (read (fd, &session, sizeof (session)) != sizeof (session)) 
	{
		memset (&session, 0, sizeof (session));
		strcpy (session.option, OPTION_TOPOLOGY);
		anynic (session.source, sizeof (session.source));
		memcpy (session.bridge [0].MAC, localcast, ETHER_ADDR_LEN);
		memcpy (session.device.MAC, localcast, ETHER_ADDR_LEN);
		session.QoS.TTL [0] = session.QoS.TTL [1] = 20000000;
		session.QoS.TTL [2] = session.QoS.TTL [3] = 30000000;
		strcpy (session.DPW, SESSION_DPW);
		strcpy (session.NPW, SESSION_NPW);
		SessionSave (filename);
	}
	close (fd);
	return;
}

#endif

