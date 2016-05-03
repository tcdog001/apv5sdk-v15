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
 *   session.h - session variable definitions and declarations;
 *
 *   the session structure stores all information needed to conduct
 *   an HTTP session; the structure is read from a file at the start
 *   of each session and written back at the end; because it is read
 *   and written so frequently, it is essentially self initializing;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef SESSION_HEADER
#define SESSION_HEADER
 
/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
 
/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../int6k/int6k.h"
#include "../tools/types.h"
#include "../ether/ether.h"
#include "../crypt/HPAVKey.h"
 
/*====================================================================*
 *   constant definitions;
 *--------------------------------------------------------------------*/

#define CGI_DATA_LENGTH 4096
#define CGI_DATANAME_LENGTH 36
#define CGI_FILENAME_LENGTH 256
 
#define SESSION_NIC "eth3"
#define SESSION_DAT "/var/state/intellon.dat"
#define SESSION_LOG "/var/state/intellon.log"
#define SESSION_DBG "/var/state/intellon.dbg"
#define SESSION_DPW "HomePlugAV"
#define SESSION_NPW "HomePlugAV"

#define TABS 5
#define CAPS 4
#define TTLS 4
#define TAGS 8
#define BITS 8
 
/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/
 
typedef struct __packed QoS 

{
	uint32_t TTL [TTLS];
	uint8_t method [2];
	uint8_t VLAN [TAGS];
	uint8_t ToS [BITS];
}

QOS;

/*====================================================================*
 *
 *--------------------------------------------------------------------*/
 
typedef struct __packed fileinfo 

{
	char name [FILENAME_MAX];
	const void * data;
	signed size;
}

fileinfo;

/*====================================================================*
 *   session variable declaration;
 *--------------------------------------------------------------------*/
 
typedef struct __packed session 

{
	char option [IF_NAMESIZE];
	char action [16];
	char source [16];
	struct bridge
	{
		byte MAC [ETHER_ADDR_LEN];
	} bridge [255];
	unsigned bridges;
	struct 
	{
		byte PLATFORM;
		byte FIRMWARE [INT6K_VERSTRING];
		byte MAC [ETHER_ADDR_LEN];
		byte DAK [HPAVKEY_DAK_LEN];
		byte NMK [HPAVKEY_NMK_LEN];
		byte NID [HPAVKEY_NID_LEN];
	}
	device;
	char DPW [HPAVKEY_PHRASE_MAX];
	char NPW [HPAVKEY_PHRASE_MAX];
	struct QoS QoS;
}

SESSION;

/*====================================================================*
 *   session management functions;
 *--------------------------------------------------------------------*/
 
void SessionLoad (const char * filename);
void SessionSave (const char * filename);
void SessionEdit ();
void SessionTask ();
void SessionPeek ();
void CGIWriteMOD (uint8_t module, const void * memory, signed extent);
void CGIFlashMOD (uint8_t module);
void CGILocalPLD ();
void CGISetKey ();
void CGIRestore ();

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif
 

