/*====================================================================*
 *
 *   bool UnwantedHomeplugMessage (void const * memory, uint8_t MMV, uint16_t MMTYPE);
 *
 *   message.h
 *
 *   return true if memory contains an Atheros vendor-specifc message 
 *   fragment of the specified type and version;
 *
 *   This software and documentation is the property of Atheros 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Atheros assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Atheros customers may modify and distribute the software 
 *   without obligation to Atheros. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of its use. 
 *   
 *.  Atheros Powerline Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@atheros.com>
 *
 *--------------------------------------------------------------------*/

#ifndef UNWANTEDHOMEPLUGMESSAGE_SOURCE
#define UNWANTEDHOMEPLUGMESSAGE_SOURCE

#include <stdint.h>
#include <memory.h>
#ifndef MAKEFILE
#include "../tools/hexstring.c"
#endif

#include "../tools/endian.h"
#include "../tools/error.h"
#include "../mme/mme.h"
#include "../tools/memory.h"
#include "../ether/channel.h"

signed UnwantedHomeplugMessage(void const * memory, uint8_t MMV, uint16_t MMTYPE, void const * memory1) 

{

	typedef struct __packed FMI
	{
		uint8_t NFFN;
		uint8_t FMSN;
		
	}FMI;
	
	typedef struct __packed homeplug_std
	{
		uint8_t ODA[ETHER_ADDR_LEN];
		uint8_t OSA[ETHER_ADDR_LEN];
//		uint32_t VLAN;
		uint16_t MTYPE;
		uint8_t MMV;
		uint16_t MMTYPE;
		struct FMI FMI;

	}homeplug_std;

	static uint8_t VAR = 0;
	typedef struct __packed homeplug_message
	{
		struct homeplug_std homeplugdata;
		uint8_t MMENTRY[VAR];
		uint8_t MME_PAD[46-VAR];

	}homeplug_message;

	struct homeplug_message * message = (struct homeplug_message *)(memory);
	struct channel * channel = (struct channel *)(memory1);
	char address[ETHER_ADDR_LEN*3];
	char address1[ETHER_ADDR_LEN*3];

	hexstring(address, sizeof(address), message->homeplugdata.OSA, sizeof(message->homeplugdata.OSA));	
	hexstring(address1, sizeof(address1), channel->peer, sizeof(channel->peer));

	if(strcmp(address1, "00:B0:52:00:00:01") != 0 && strcmp(address1, "FF:FF:FF:FF:FF:FF") != 0)
		if(strcmp(address, address1)!= 0)
		{
//			printf("Wrong MAC Address: Have %s but want %s\n",address, address1);
			return(-1);
		}
//	printf("MAC Address:%s\n", address1);

	if (message->homeplugdata.MTYPE != htons (HOMEPLUG_MTYPE)) 
	{

		printf("MTYPE:");
#if defined (__DEBUG__)

		error (0, 0, "Wrong Fragment Type: Have %04X but want %04X", ntohs (message->homeplugdata.MTYPE), HOMEPLUG_MTYPE);

#endif

		return (1);
	}
	if (message->homeplugdata.MMV != 1) 
	{
		printf("MMV:");

#if defined (__DEBUG__)

		error (0, 0, "Wrong Fragment Version: Have %02x but want 01", message->homeplugdata.MMV);

#endif

		return (1);
	}
	if (message->homeplugdata.MMTYPE != HTOLE16 (CM_BRG_INFO | MMTYPE_CNF)) 
	{
		printf("MMTYPE:");

#if defined (__DEBUG__)

		error (0, 0, "Wrong Fragment Type: Have %04X but want %04X", LE16TOH (message->homeplugdata.MMTYPE), (VS_TONEMAP_CHAR | MMTYPE_CNF));

#endif

		return (1);
	}

	return (0);
}

#endif

