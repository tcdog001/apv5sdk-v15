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
 *   signed pibpeek (const byte memory []);
 *
 *   pib.h
 *
 *   print PIB identify information on stdout; 
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *		Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef PIBPEEK_SOURCE
#define PIBPEEK_SOURCE

#include <stdio.h>
#include <memory.h>

#include "../tools/memory.h"
#include "../tools/number.h"
#include "../crypt/HPAVKey.h"
#include "../crypt/keys.h"
#include "../pib/pib.h"

signed pibpeek (const byte memory []) 

{
	extern const struct key keys [KEYS];
	extern const char * CCoMode [PIB_CCOMODES];
	extern const char * MDURole [PIB_MDUROLES];
	char buffer [HPAVKEY_SHA_LEN * 3];
	struct PIB1 * PIB;
	size_t key;
	if (memory [0] == 1) 
	{
		if (memory [1] == 2) 
		{
			struct PIB1_2 * PIB = (struct PIB1_2 *)(memory);
			printf ("\tPIB %d-%d\n", PIB->FWVersion, PIB->PIBVersion);
			printf ("\tMAC %s\n", hexstring (buffer, sizeof (buffer), PIB->MAC, sizeof (PIB->MAC)));
			printf ("\tDAK %s", hexstring (buffer, sizeof (buffer), PIB->DAK, sizeof (PIB->DAK)));
			for (key = 0; key < KEYS; key++) 
			{
				if (!memcmp (keys [key].DAK, PIB->DAK, HPAVKEY_DAK_LEN)) 
				{
					printf (" (%s)", keys [key].phrase);
					break;
				}
			}
			printf ("\n");
			printf ("\tNMK %s", hexstring (buffer, sizeof (buffer), PIB->NMK, sizeof (PIB->NMK)));
			for (key = 0; key < KEYS; key++) 
			{
				if (!memcmp (keys [key].NMK, PIB->NMK, HPAVKEY_NMK_LEN)) 
				{
					printf (" (%s)", keys [key].phrase);
					break;
				}
			}
			printf ("\n");
			printf ("\tFLG %s\n", hexstring (buffer, sizeof (buffer), (uint8_t *)(&PIB->FLG), sizeof (PIB->FLG)));
			return (0);
		}
		else if (memory [1] == 3) 
		{
			struct PIB1_3 * PIB = (struct PIB1_3 *)(memory);
			printf ("\tPIB %d-%d\n", PIB->FWVersion, PIB->PIBVersion);
			printf ("\tMAC %s\n", hexstring (buffer, sizeof (buffer), PIB->MAC, sizeof (PIB->MAC)));
			printf ("\tDAK %s", hexstring (buffer, sizeof (buffer), PIB->DAK, sizeof (PIB->DAK)));
			for (key = 0; key < KEYS; key++) 
			{
				if (!memcmp (keys [key].DAK, PIB->DAK, HPAVKEY_DAK_LEN)) 
				{
					printf (" (%s)", keys [key].phrase);
					break;
				}
			}
			printf ("\n");
			printf ("\tNMK %s", hexstring (buffer, sizeof (buffer), PIB->NMK, sizeof (PIB->NMK)));
			for (key = 0; key < KEYS; key++) 
			{
				if (!memcmp (keys [key].NMK, PIB->NMK, HPAVKEY_NMK_LEN)) 
				{
					printf (" (%s)", keys [key].phrase);
					break;
				}
			}
			printf ("\n");
			printf ("\tNET \"%s\"\n", PIB->NET);
			printf ("\tMFG \"%s\"\n", PIB->MFG);
			printf ("\tUSR \"%s\"\n", PIB->USR);
			return (0);
		}
		else if (memory [1] >= 4) 
		{
			struct PIB1_4 * PIB = (struct PIB1_4 *)(memory);
			printf ("\tPIB %d-%d %d bytes\n", PIB->VersionHeader.FWVersion, PIB->VersionHeader.PIBVersion, LE16TOH (PIB->VersionHeader.PIBLength));
			printf ("\tMAC %s\n", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.MAC, sizeof (PIB->LocalDeviceConfig.MAC)));
			printf ("\tDAK %s", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.DAK, sizeof (PIB->LocalDeviceConfig.DAK)));
			for (key = 0; key < KEYS; key++) 
			{
				if (!memcmp (keys [key].DAK, PIB->LocalDeviceConfig.DAK, HPAVKEY_DAK_LEN)) 
				{
					printf (" (%s)", keys [key].phrase);
					break;
				}
			}
			printf ("\n");
			printf ("\tNMK %s", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.NMK, sizeof (PIB->LocalDeviceConfig.NMK)));
			for (key = 0; key < KEYS; key++) 
			{
				if (!memcmp (keys [key].NMK, PIB->LocalDeviceConfig.NMK, HPAVKEY_NMK_LEN)) 
				{
					printf (" (%s)", keys [key].phrase);
					break;
				}
			}
			printf ("\n");
			printf ("\tNET %s\n", PIB->LocalDeviceConfig.NET);
			printf ("\tMFG %s\n", PIB->LocalDeviceConfig.MFG);
			printf ("\tUSR %s\n", PIB->LocalDeviceConfig.USR);
			return (0);
		}
	}
	else if (memory [0] == 2) 
	{
		struct PIB2_0 * PIB = (struct PIB2_0 *)(memory);
		printf ("\tPIB %d-%d %d bytes\n", PIB->VersionHeader.FWVersion, PIB->VersionHeader.PIBVersion, LE16TOH (PIB->VersionHeader.PIBLength));
		printf ("\tMAC %s\n", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.MAC, sizeof (PIB->LocalDeviceConfig.MAC)));
		printf ("\tDAK %s", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.DAK, sizeof (PIB->LocalDeviceConfig.DAK)));
		for (key = 0; key < KEYS; key++) 
		{
			if (!memcmp (keys [key].DAK, PIB->LocalDeviceConfig.DAK, HPAVKEY_DAK_LEN)) 
			{
				printf (" (%s)", keys [key].phrase);
				break;
			}
		}
		printf ("\n");
		printf ("\tNMK %s", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.NMK, sizeof (PIB->LocalDeviceConfig.NMK)));
		for (key = 0; key < KEYS; key++) 
		{
			if (!memcmp (keys [key].NMK, PIB->LocalDeviceConfig.NMK, HPAVKEY_NMK_LEN)) 
			{
				printf (" (%s)", keys [key].phrase);
				break;
			}
		}
		printf ("\n");
		printf ("\tNID %s\n", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.PreferredNID, sizeof (PIB->LocalDeviceConfig.PreferredNID)));
		printf ("\tNET %s\n", PIB->LocalDeviceConfig.NET);
		printf ("\tMFG %s\n", PIB->LocalDeviceConfig.MFG);
		printf ("\tUSR %s\n", PIB->LocalDeviceConfig.USR);
		return (0);
	}
	else if ((memory [0] == 3) || (memory [0] == 4)) 
	{
		struct PIB3_0 * PIB = (struct PIB3_0 *)(memory);
		printf ("\tPIB %d-%d %d bytes\n", PIB->VersionHeader.FWVersion, PIB->VersionHeader.PIBVersion, LE16TOH (PIB->VersionHeader.PIBLength));
		printf ("\tMAC %s\n", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.MAC, sizeof (PIB->LocalDeviceConfig.MAC)));
		printf ("\tDAK %s", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.DAK, sizeof (PIB->LocalDeviceConfig.DAK)));
		for (key = 0; key < KEYS; key++) 
		{
			if (!memcmp (keys [key].DAK, PIB->LocalDeviceConfig.DAK, HPAVKEY_DAK_LEN)) 
			{
				printf (" (%s)", keys [key].phrase);
				break;
			}
		}
		printf ("\n");
		printf ("\tNMK %s", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.NMK, sizeof (PIB->LocalDeviceConfig.NMK)));
		for (key = 0; key < KEYS; key++) 
		{
			if (!memcmp (keys [key].NMK, PIB->LocalDeviceConfig.NMK, HPAVKEY_NMK_LEN)) 
			{
				printf (" (%s)", keys [key].phrase);
				break;
			}
		}
		printf ("\n");
		printf ("\tNID %s\n", hexstring (buffer, sizeof (buffer), PIB->LocalDeviceConfig.PreferredNID, sizeof (PIB->LocalDeviceConfig.PreferredNID)));
		printf ("\tNET %s\n", PIB->LocalDeviceConfig.NET);
		printf ("\tMFG %s\n", PIB->LocalDeviceConfig.MFG);
		printf ("\tUSR %s\n", PIB->LocalDeviceConfig.USR);
		printf ("\tCCo %s\n", CCoMode [PIB->LocalDeviceConfig.CCoSelection]);
		printf ("\tMDU %s\n", PIB->LocalDeviceConfig.MDUConfiguration? MDURole [PIB->LocalDeviceConfig.MDURole & 1]: "N/A");
		return (0);
	}
	PIB = (struct PIB1 *)(memory);
	printf ("\tPIB %d-%d %d (unsupported)\n", PIB->FWVersion, PIB->PIBVersion, LE16TOH (PIB->PIBLength));
	return (1);
}

#endif

