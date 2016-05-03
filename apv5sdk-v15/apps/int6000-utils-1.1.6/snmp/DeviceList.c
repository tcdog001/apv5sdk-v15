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
 *   signed DeviceList (struct int6k * int6k);
 *   
 *   int6k.h
 * 
 *   Retrieve a list of local devices.
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
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef DEVICELIST_SOURCE
#define DEVICELIST_SOURCE

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/memory.h"
#include "../snmp/DeviceList.h"

void free_list (struct device *head) 

{
	struct device *n;
	struct device *d;
	d = head;
	while (d) 
	{
		n = d->next;
		free (d);
		d = n;
	}
}

/*
 * Add an item d to the end of the list. must pass in head.
 */

void append_list (struct device **head, struct device *d) 

{
	struct device *n;
	struct device *i;
	n = malloc (sizeof (struct device));
	if (n == NULL) error (1, errno, INT6K_ERR_MEMORY);
	memcpy (n, d, sizeof (struct device));
	n->next = NULL;
	i = *head;
	while (i) 
	{
		if (i->next == NULL) break;
		i = i->next;
	}

/* was not an empty list */

	if (i != NULL) i->next = n;
	else *head = n;
}

void dump_list (struct device *d) 

{
	while (d) 
	{
		printf ("mac: %s\n", d->mac);
		printf ("id: %s\n", d->id);
		printf ("fw version: %s\n", d->version);
		d = d->next;
		printf ("----\n");
	}
}

struct device *DeviceList (struct int6k * int6k) 

{
	extern const char * chipset [CHIPSETS+1];
	static struct device *head;
	struct device d;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_sw_ver_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MDEVICEID;
		uint8_t MVERLENGTH;
		char MVERSION [INT6K_VERSTRING];
		uint8_t UPGRADEABLE;
		uint32_t MCOOKIE;
	}
	* confirm = (struct vs_sw_ver_confirm *) (int6k->message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	if (head) 
	{
		free_list (head);
		head = NULL;
	}
	memset (int6k->message, 0, sizeof (struct message));
	hexencode (int6k->channel->peer, sizeof (int6k->channel->peer), BROADCAST);
	EthernetHeader (&int6k->message->ethernet, int6k->channel->peer, int6k->channel->host);
	IntellonHeader (&int6k->message->intellon, (VS_SW_VER | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (NULL);
	}
	while (ReadMME (int6k, (VS_SW_VER | MMTYPE_CNF)) > 0) 
	{
		if (confirm->MSTATUS) 
		{
			continue;
		}
		if (confirm->MDEVICEID > CHIPSETS) 
		{
			confirm->MDEVICEID = 0x00;
		}
		bzero (&d, sizeof (struct device));
		hexdecode (int6k->message->ethernet.OSA, sizeof (int6k->message->ethernet.OSA), d.mac, sizeof (d.mac));
		d.id = chipset [confirm->MDEVICEID];
		strlcpy (d.version, confirm->MVERSION, sizeof (d.version));
		append_list (&head, &d);
	}

/* NULL if no devices found */

	return head;
}

#endif

