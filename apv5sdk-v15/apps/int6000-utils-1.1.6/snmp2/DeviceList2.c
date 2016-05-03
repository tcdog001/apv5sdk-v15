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
 *   struct device *DeviceList (struct int6k * int6k);
 *   
 *   int6k.h
 * 
 *   Retrieve a list of local devices using ihpapi_GetVersionInfo
 *   api function calls.
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
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef DEVICELIST2_SOURCE
#define DEVICELIST2_SOURCE

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BROADCAST "FF:FF:FF:FF:FF:FF"

#include "../ihpapi/ihp.h"
#include "../ihpapi/ihpapi.h"
#include "../int6k/chipset.h"
#include "../tools/error.h"
#include "../tools/memory.h"

#include "../snmp2/DeviceList2.h"

extern const char * chipset [CHIPSETS+1];
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
	static struct device *head;
	struct device d;
	printf ("1\n");
	if (head) 
	{
		free_list (head);
		head = NULL;
	}
	hexencode (int6k->channel->peer, sizeof (int6k->channel->peer), BROADCAST);
	int6k->packetsize = (int)(ihpapi_GetVersionInfo (int6k->channel->host, int6k->channel->peer, sizeof (struct message), (uint8_t *)(int6k->message)));
	if (int6k->packetsize == 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "wrong packet size");
		return NULL;
	}
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
		return NULL;
	}
	while (ReadMME2 (int6k) > 0) 
	{
		int i = (int6k->result->data.version.mdeviceid > CHIPSETS)? 0:int6k->result->data.version.mdeviceid;
		if (int6k->result->opStatus.status) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			continue;
		}
		bzero (&d, sizeof (struct device));
		hexdecode (int6k->message->ethernet.OSA, sizeof (int6k->message->ethernet.OSA), d.mac, sizeof (d.mac));
		d.id = chipset [i];
		strlcpy (d.version, int6k->result->data.version.mversion, sizeof (d.version));
		append_list (&head, &d);
	}
	return head;
}

#endif

