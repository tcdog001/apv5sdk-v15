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
 *   intellon.c - intellon powerline device manager;
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdlib.h>
#include <errno.h> 
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#if defined (__APPLE__)
#include <sys/types.h>
#endif
#include <net/if_arp.h>
#include <netinet/in.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../int6k/int6k.h"
#include "../tools/files.h"
#include "../tools/format.h"
#include "../tools/error.h"
#include "../ether/channel.h"
#include "../scan/scan.h"
#include "../cgi/session.h"
#include "../cgi/page.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../cgi/DivisionHeader.c"
#include "../cgi/DivisionOption.c"
#include "../cgi/DivisionDevice.c"
#include "../cgi/DivisionFooter.c"
#include "../cgi/status.c"
#include "../cgi/session.c"
#include "../cgi/SessionLoad.c"
#include "../cgi/SessionEdit.c"
#include "../cgi/SessionTask.c"
#include "../cgi/SessionPeek.c"
#include "../cgi/SessionSave.c"
#include "../cgi/stylesheet.c"
#include "../cgi/OptionTopology.c"
#include "../cgi/OptionIdentity.c"
#include "../cgi/OptionSecurity.c"
#include "../cgi/OptionPriority.c"
#include "../cgi/OptionFirmware.c"
#include "../cgi/CGILocalPLD.c"
#include "../cgi/CGIWriteMOD.c"
#include "../cgi/CGIFlashMOD.c"
#include "../cgi/CGISetKey.c"
#include "../cgi/CGIRestore.c"
#endif

#ifndef MAKEFILE
#include "../int6k/chipset.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#endif

#ifndef MAKEFILE
#include "../crypt/SHA256Reset.c"
#include "../crypt/SHA256Write.c"
#include "../crypt/SHA256Block.c"
#include "../crypt/SHA256Fetch.c"
#include "../crypt/HPAVKeyDAK.c"
#include "../crypt/HPAVKeyNMK.c"
#include "../crypt/HPAVKeyNID.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibcodes.c"
#endif

#ifndef MAKEFILE
#include "../nvm/nvmspan.c"
#endif

#ifndef MAKEFILE
#include "../tools/output.c"
#include "../tools/hexstring.c"
#include "../tools/hexdecode.c"
#include "../tools/hexencode.c"
#include "../tools/checksum_32.c"
#include "../tools/hexdump.c"
#include "../tools/todigit.c"
#include "../tools/filepart.c"
#endif

#ifndef MAKEFILE
#include "../ether/channel.c"
#include "../ether/openchannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#include "../ether/closechannel.c"
#include "../ether/anynic.c"
#endif

#ifndef MAKEFILE
#include "../scan/scan.c"
#endif

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

const char * program_name = "";

/*====================================================================*
 *
 *   struct tab tabs [TABS]; 
 *
 *   page.h
 *
 *   each tab consists of a function pointer, name and text; you can
 *   add or remove tabs here and adjust TABS in page.h;each tab needs
 *   it's own function;
 *  
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

struct tab tabs [TABS] = 

{
	{
		OptionTopology,
		OPTION_TOPOLOGY,
		"Network Topology" 
	},
	{
		OptionIdentity,
		OPTION_IDENTITY,
		"Device Identity" 
	},
	{
		OptionSecurity,
		OPTION_SECURITY,
		"Device Security" 
	},
	{
		OptionPriority,
		OPTION_PRIORITY,
		"Channel Priority" 
	},
	{
		OptionFirmware,
		OPTION_FIRMWARE,
		"Device Firmware and Configuration" 
	}
};

/*====================================================================*
 *
 *   int main (int argc, const char * argv []);
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv [], const char * envp []) 

{
	extern struct session session;
	extern struct channel channel;
	extern struct tab tabs [TABS];
	signed margin = 0;
	signed tab = 0;
	program_name = filepart (*argv);
	while ((--argc) && (*++argv));
	SessionLoad (SESSION_DAT);
	channel.name = session.source;
	openchannel (&channel);
	SessionEdit ();
	SessionTask ();
	margin = DivisionHeader (margin, STYLE_HEADER);
	margin = DivisionOption (margin, STYLE_OPTION);
	for (tab = 0; tab < TABS; tab++) 
	{
		if (!strcmp (tabs [tab].name, session.option)) 
		{
			margin = tabs [tab].form (margin, STYLE_ACTION);
		}
	}
	margin = DivisionDevice (margin, STYLE_DEVICE);
	margin = DivisionFooter (margin, STYLE_FOOTER);
	SessionSave (SESSION_DAT);
//	SessionPeek ();
	closechannel (&channel);
	return (0);
}

