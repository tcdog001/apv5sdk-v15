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
 *   itln_subagent -- demo subagent for net-snmpd.
 *
 *   This code is based on the example provided in the net-snmpd
 *   tutorial section, with bits of the linux toolkit used to provide
 *   necessary functionality. It is intended for demonstration
 *   purposes only.
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
 *
 *--------------------------------------------------------------------*/

/*====================================================================*
 *   system header files
 *--------------------------------------------------------------------*/

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/utilities.h>
#include <signal.h>
#include <errno.h>

/*====================================================================*
 *   custom header files
 *--------------------------------------------------------------------*/

#include "../snmp/intellon-snmp.h"
#include "../snmp/DeviceList.h"
#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../tools/getoptv.h"
#include "../tools/memory.h"

/*====================================================================*
 *   program constants; 
 *--------------------------------------------------------------------*/

#define SUBAGENT_STR "itln_subagent"
#define UPDATE_INTERVAL 3 /* time between SNMP table updates (seconds) */

/*====================================================================*
 *   program variables; 
 *--------------------------------------------------------------------*/

/* global so that it can be accessed in SNMP callback functions */

extern struct channel channel;

#include "../int6k/int6k-struct.c"

/*====================================================================*
 *   program functions; 
 *--------------------------------------------------------------------*/

void update_snmp (struct int6k *);
static int keep_running;
void timer (unsigned int, void *);

/*====================================================================*
 *
 *   RETSIGTYPE stop_server (int a);
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

RETSIGTYPE stop_server (int a) 

{
	keep_running = 0;
}

/*====================================================================*
 *
 *   int main (int argc, const char * argv []);
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	int c;
	int r;
	static const char *optv [] = 
	{
		"i:qv",
		"device [device] [...]",
		"Intellon INT6x00 NET-SNMPD SubAgent",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number is n [2]",

#else

		"i s\thost interface name is s [" CHANNEL_ETHDEVICE "]",

#endif

		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};
	if (getenv (PLCDEVICE)) 
	{
		channel.name = strdup (getenv (PLCDEVICE));
	}
	optmin = 0;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (int6k.flags, INT6K_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (geteuid ()) 
	{
		error (1, EPERM, INT6K_NEEDROOT);
	}
	openchannel (&channel);
	if (!(int6k.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}

/* use syslog instead of stderr */

	snmp_enable_calllog ();
	netsnmp_ds_set_boolean (NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);

/* initialize tcpip, if necessary */

	SOCK_STARTUP;

/* initialize the agent library and register with snmpd */

	r = init_agent (SUBAGENT_STR);
	if (r) error (1, 0, "could not initialize agent.");
	init_intellon ();

/* init snmp / read config */

	init_snmp (SUBAGENT_STR);

/* set up termination signal */

	keep_running = 1;
	signal (SIGTERM, stop_server);
	signal (SIGINT, stop_server);

/* register our update timer with snmpd */

	snmp_alarm_register (UPDATE_INTERVAL, SA_REPEAT, timer, &int6k);
	while (keep_running) 
	{

/* block for snmp messages */

		r = agent_check_and_process (1);
	}
	snmp_shutdown (SUBAGENT_STR);
	SOCK_CLEANUP;
	free (int6k.message);
	closechannel (&channel);
	return 0;
}

/*====================================================================*
 *
 *   struct operationsTable_entry * table_find_device (struct operationsTable_entry *head, char *mac);
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

struct operationsTable_entry * table_find_device (struct operationsTable_entry *head, char *mac) 

{
	struct operationsTable_entry *entry;
	entry = head;
	while (entry) 
	{
		if (!strcmp (mac, entry->deviceMacAddress)) return entry;
		entry = entry->next;
	}
	return NULL;
}

/*====================================================================*
 *
 *   int table_find_empty (struct operationsTable_entry *head);
 *
 *   find first un-used deviceIndex 
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int table_find_empty (struct operationsTable_entry *head) 

{
	struct operationsTable_entry *entry;
	int i;

/* search for first hole */

/* this is absolutely horrible and slow... */

	for (i = 0; i >= 0; ++i) 
	{
		entry = head;
		while (entry) 
		{
			if (entry->deviceIndex == i) break;
			entry = entry->next;
		}
		if (entry) continue;
		else break;
	}

/* positive if index found, negative otherwise */

	return i;
}

/*====================================================================*
 *
 *   struct device * list_find_device (struct device *head, char *mac);
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

struct device * list_find_device (struct device *head, char *mac) 

{
	struct device *d;
	d = head;
	while (d) 
	{
		if (!strcmp (mac, d->mac)) return d;
		d = d->next;
	}
	return NULL;
}

/*====================================================================*
 *
 *   void timer (unsigned int reg, void *clientarg);
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

void timer (unsigned int reg, void *clientarg) 

{
	update_snmp ((struct int6k *) clientarg);
}

/*====================================================================*
 *
 *   void update_snmp (struct int6k *int6k);
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

void update_snmp (struct int6k *int6k) 

{
	static int index;
	struct device *list_head;
	struct device *d;
	extern struct operationsTable_entry *operationsTable_head;
	struct operationsTable_entry *entry;
	list_head = DeviceList (int6k);
	if (list_head == NULL) printf ("no devices found...\n");
	d = list_head;
	while (d) 
	{

/* see if the device is already in the snmp table */

		entry = table_find_device (operationsTable_head, d->mac);

/* MAC address is not in table */

		if (!entry) 
		{

/* find empty index */

			index = table_find_empty (operationsTable_head);
			if (index < 0) 
			{
				error (0, 0, "error: device table full!");
				return;
			}
			operationsTable_createEntry (index, d);
			printf ("A %s\n", d->mac);
		}
		else 
		{
			printf ("U %s\n", d->mac);
			memcpy (entry->deviceID, d->id, strlen (d->id) + 1);
			entry->deviceID_len = strlen (entry->deviceID);
			memcpy (entry->deviceFirmwareRevision, d->version, strlen (d->version) + 1);
			entry->deviceFirmwareRevision_len = strlen (entry->deviceFirmwareRevision);
		}
		d = d->next;
	}

/* remove devices no longer present */

	entry = operationsTable_head;
	while (entry) 
	{
		if (list_find_device (list_head, entry->deviceMacAddress) == NULL) 
		{
			printf ("R %s\n", entry->deviceMacAddress);
			operationsTable_remove (entry);
		}
		entry = entry->next;
	}
	printf ("---------\n");
}

