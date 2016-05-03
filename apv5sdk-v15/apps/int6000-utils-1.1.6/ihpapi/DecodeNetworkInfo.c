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
 *   int ihp_DecodeNetworkInfo (const uint8_t buffer [], size_t length, ihpapi_result_t * result);
 *
 *   ihp.h
 *
 *   decode buffer as a VS_LNK_STATS Confirm MME; update the structure 
 *   named by the caller; 
 *
 *   return 0 on decode success and -1 on decode error;
 * 
 *   EFAULT if buffer/result address or size are invalid or illegal;
 *
 *   See the INT6000 Firmware Technical Reference Manual
 *   for more information;
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
 *.  Intellon HomePlug AV Application Programming Interface;
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration and evaluation only; Not for production use;
 *
 *   Contributor(s): 
 *	Charles Maier, charles.maier@intellon.com
 *	Alex Vasquez, alex.vasquez@intellon.com
 *
 *--------------------------------------------------------------------*/

#ifndef DECODENETWORKINFO_SOURCE
#define DECODENETWORKINFO_SOURCE

#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "../ihpapi/ihp.h"
 
/*====================================================================*
 *   single station structure;
 *--------------------------------------------------------------------*/
 
typedef struct __packed station 

{
	uint8_t DA [IHPAPI_ETHER_ADDR_LEN];
	uint8_t TEI;
	uint8_t BDA [IHPAPI_ETHER_ADDR_LEN];
	uint8_t AVGTX;
	uint8_t AVGRX;
}

station;

/*====================================================================*
 *   single network structure;
 *--------------------------------------------------------------------*/
 
typedef struct __packed network 

{
	uint8_t NID [7];
	uint8_t SNID;
	uint8_t TEI;
	uint8_t ROLE;
	uint8_t CCO_MACADDR [6];
	uint8_t CCO_TEI;
	uint8_t NUMSTAS;
	struct station station [1];
}

network;
int ihp_DecodeNetworkInfo (const uint8_t buffer [], size_t length, ihpapi_result_t * result) 

{
	struct vs_nw_info_cnf 
	{
		struct header_vs header;
		int8_t NUMAVLNS;
		int8_t NWINFO [IHPAPI_ETH_FRAME_LEN - sizeof (header_mme) - 1];
	}
	*confirm = (struct vs_nw_info_cnf *)(buffer);
	ihpapi_getNetworkInfoData_t * message = & result->data.netInfo;
	if (length < sizeof (struct network)) 
	{
		errno = EFAULT;
		return (-1);
	}
	if (result == (ihpapi_result_t *)(0)) 
	{
		errno = EFAULT;
		return (-1);
	}
	message->numavlns = confirm->NUMAVLNS;
	memcpy (message->nwinfo, confirm->NWINFO, sizeof (confirm->NWINFO));
	result->dataLen = sizeof (ihpapi_getNetworkInfoData_t);
	result->validData = true;
	result->opStatus.status = 0;
	return (0);
}

#endif
 

