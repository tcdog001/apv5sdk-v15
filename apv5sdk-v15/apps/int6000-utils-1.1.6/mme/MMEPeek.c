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
 *   void MMEPeek (const void * memory, size_t length, FILE *fp);
 *
 *   mme.h
 *   
 *   print a HomePlug AV frame header on stdout in human readable 
 *   format;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual 
 *   for more information on Intellon vendor specific header formats;
 *
 *   This software and documentation is the property of Intellon 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Intellon assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Intellon customers may modify and distribute the software 
 *   without obligation to Intellon. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *   
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef MMPEEK_SOURCE
#define MMPEEK_SOURCE

#include <stdio.h>
#include <stdint.h>

#include "../int6k/homeplug.h"
#include "../int6k/message.h"
#include "../tools/memory.h"
#include "../mme/mme.h"

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6 /* normally defined in ethernet.h or if_ether.h */
#endif

void MMEPeek (const void * memory, size_t extent, FILE *fp) 

{
	char address [ETHER_ADDR_LEN * 3];
	struct packet_eth 
	{
		uint8_t ODA [ETHER_ADDR_LEN];
		uint8_t OSA [ETHER_ADDR_LEN];
		uint16_t MTYPE;
		uint8_t BODY [ETHERMTU];
	}
	* packet = (struct packet_eth *)(memory);
	fprintf (fp, "ODA=%s ", hexstring (address, sizeof (address), packet->ODA, sizeof (packet->ODA)));
	fprintf (fp, "OSA=%s ", hexstring (address, sizeof (address), packet->OSA, sizeof (packet->OSA)));
	fprintf (fp, "MTYPE=%04X ", ntohs (packet->MTYPE));
	if (ntohs (packet->MTYPE) == 0x88E1) 
	{
		struct header_mme * header = (struct header_mme *)(&packet->BODY);
		if (LE16TOH (header->MMTYPE) < (unsigned)(MMTYPE_MS)) 
		{
			fprintf (fp, "MMV=%02X ", header->MMV);
			fprintf (fp, "MMTYPE=%04X ", LE16TOH (header->MMTYPE));
			fprintf (fp, "FMI=%04X ", LE16TOH (header->FMI));
			fprintf (fp, "%s.%s\n", MMEName (header->MMTYPE), MMEMode (header->MMTYPE));
			return;
		}
		if (LE16TOH (header->MMTYPE) < (unsigned)(MMTYPE_VS)) 
		{
			struct header_std * header = (struct header_std *)(&packet->BODY);
			fprintf (fp, "MMV=%02X ", header->MMV);
			fprintf (fp, "MMTYPE=%04X ", LE16TOH (header->MMTYPE));
			fprintf (fp, "%s.%s\n", MMEName (header->MMTYPE), MMEMode (header->MMTYPE));
			return;
		}
		if (LE16TOH (header->MMTYPE) < (unsigned)(MMTYPE_XX)) 
		{
			struct header_int * header = (struct header_int *)(&packet->BODY);
			fprintf (fp, "MMV=%02X ", header->MMV);
			fprintf (fp, "MMTYPE=%04X ", LE16TOH (header->MMTYPE));
			fprintf (fp, "OUI=%s ", hexstring (address, sizeof (address), header->OUI, ETHER_ADDR_LEN/2));
			fprintf (fp, "%s.%s\n", MMEName (header->MMTYPE), MMEMode (header->MMTYPE));
			return;
		}
	}
	fprintf (fp, "NON_HOMEPLUG_FRAME\n");
	return;
}

#endif

