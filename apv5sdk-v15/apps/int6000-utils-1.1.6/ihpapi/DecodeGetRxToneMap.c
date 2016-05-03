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
 *   int ihp_DecodeGetRxToneMapInfo (const uint8_t buffer [], size_t length, ihpapi_result_t * result);
 *
 *   ihp.h
 *
 *   decode buffer as a VS_RX_TONE_MAP_CHAR Confirm MME; update the structure
 *   named by the caller; 
 *
 *   return 0 on decode success or -1 on decode error;
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
 *	Alex Vasquez, alex.vasquez@intellon.com
 *
 *--------------------------------------------------------------------*/

#ifndef DECODEGETRXTONEMAPINFO_SOURCE
#define DECODEGETRXTONEMAPINFO_SOURCE

#include <stdint.h>
#include <string.h>

#include "../ihpapi/ihp.h"
 
int ihp_DecodeGetRxToneMapInfo (const uint8_t buffer [], size_t length, ihpapi_result_t * result) 

{
	struct __packed vs_rx_tone_map_char_cnf 
	{
		struct header_vs header;
		uint8_t MSTATUS;
		uint8_t TMSLOT;
		uint8_t NUMTMS;
		uint16_t TMNUMACTCARRIERS;
		uint8_t MOD_CARRIER [MOD_CARRIER_MAX_TUPLE_NUM];
		uint8_t GIL;
		uint16_t AVG_AGC_GAIN;
	}
	* confirm = (struct vs_rx_tone_map_char_cnf *)(buffer);

#if INTELLON_SAFEMODE
 
	if (length < sizeof (struct vs_rx_tone_map_char_cnf)) 
	{
		return (-1);
	}
	if (result == (ihpapi_result_t *)(0)) 
	{
		return (-1);
	}

#endif
 
	result->opStatus.status = confirm->MSTATUS;
	result->data.rxToneMap.tmslot = confirm->TMSLOT;
	result->data.rxToneMap.numtms = confirm->NUMTMS;
	result->data.rxToneMap.tmnumactcarriers = intohs(confirm->TMNUMACTCARRIERS);
	result->data.rxToneMap.gil = confirm->GIL;
	result->data.rxToneMap.avg_agc_gain = intohs(confirm->AVG_AGC_GAIN);
	memcpy (result->data.rxToneMap.mod_carrier, confirm->MOD_CARRIER, MOD_CARRIER_MAX_TUPLE_NUM);
	return (0);
}

#endif
 

