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
 *   int6k-struct.c - int6x00 data structure;
 *
 *   this structure contains information needed to perform operations
 *   on INT6x00 devices; it represents one instance of a device and 
 *   could easily be converted to an object in the future;  
 *
 *   this file is included by Intellon tools that use vendor specific
 *   management message to access INT6x00 devices; the structure is 
 *   defined in int6k.h;
 *
 *   one critical element of this structure is the channel structure
 *   that varies depending on the runtime platform; it permits raw
 *   socket I/O and is defined in channel.h;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef INT6K_STRUCT_SOURCE
#define INT6K_STRUCT_SOURCE

struct int6k int6k = 

{
	(struct channel *)(&channel),
	(struct message *) (0),
	0,

/*
 * Local Device Address (LDA) buffer needed by all operations;
 */

	{
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00 
	},

/*
 * Remote Device Address (RDA) buffer needed by selected operations;
 */

	{
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00 
	},

/*
 *   Network Membership Key (NMK) needed by VS_SET_KEY operations;
 *   The default is an encrypted version of password "HomePlugAV";
 */

	{
		0x50,
		0xD3,
		0xE4,
		0x93,
		0x3F,
		0x85,
		0x5B,
		0x70,
		0x40,
		0x78,
		0x4D,
		0xF8,
		0x15,
		0xAA,
		0x8D,
		0xB7 
	},

/*
 *   Device Access Key (DAK) needed for VS_SET_KEY operations;
 *   The default is an encrypted version of password "HomePlugAV";
 */

	{
		0x68,
		0x9F,
		0x07,
		0x4B,
		0x8B,
		0x02,
		0x75,
		0xA2,
		0x71,
		0x0B,
		0x0B,
		0x57,
		0x79,
		0xAD,
		0x16,
		0x30 
	},

/*
 * struct _file_ CFG; MAC software will be read from this file and 
 * written to RAM for each device specified on the command line;
 */

	{
		(file_t) (-1),
		(const char *)(0) 
	},

/*
 * struct _file_ cfg; SDRAM configuration will be read from flash 
 * on the specified device and written to this file; 
 */

	{
		(file_t) (-1),
		(const char *)(0) 
	},

/*
 * struct _file_ NVM; runtime firmware will be read from this file 
 * and written to RAM for each device specified on the command line;
 */

	{
		(file_t) (-1),
		(const char *)(0) 
	},

/*
 * struct _file_ nvm; MAC software will be read from SDRAM on the 
 * specified device and written to this file; interlocks elsewhere 
 * in the code should prevent this file from being overwritten 
 * multiple times, by accident, and ensure that it is created before 
 * and new MAC software is written to device RAM;
 */

	{
		(file_t) (-1),
		(const char *)(0) 
	},

/*
 * struct _file_ PIB; PIB information will be read from this file 
 * and written to RAM for each device specified on the command line;
 */

	{
		(file_t) (-1),
		(const char *)(0) 
	},

/*
 * struct _file_ pib; PIB information will be read from SDRAM on 
 * the specified device and written to this file; interlocks elsewhere 
 * in the code should prevent this file from being overwritten 
 * multiple times, by accident, and ensure that it is created before 
 * any new PIB software is written to device RAM;
 */

	{
		(file_t) (-1),
		(const char *)(0) 
	},

/*
 * struct _file_ XML; optional XML PIB edit instructions; this is an
 * advanced feature;
 */

	{
		(file_t) (-1),
		(const char *)(0) 
	},

/*
 * struct _file_ rpt; Watchdog Report data will be read from the device
 * and written to this file;
 */

	{
		(file_t) (-1),
		(const char *)(0) 
	},

/*
 *   miscellaneous small integers used as needed when arguments 
 *   are required for an MME; 
 */

	HARDWAREID,
	HOSTACTION,
	MODULECODE,
	PUSHBUTTON,
	READACTION,
	TIMEPERIOD,

/*
 *   various boolean flags; bailout causes exit on first failure;
 *   timeout indicates a real timeout; count is the loop counter;
 *   pause is a loop wait timer; values are declared as constants
 *   above;
 */

	INT6K_STATE,
	INT6K_PAUSE,
	INT6K_TIMER,
	INT6K_RETRY,
	INT6K_INDEX,
	INT6K_FLAGS,

#ifdef IHPAPI_HEADER

/*
 *   ihpapi_result_t *result; function call returns information  
 *   in this structure by decoding the incoming Ethernet frame as 
 *   Intellon Vendor Specific MME;
 */

	(ihpapi_result_t *)(0),

/*
 *   unsigned options; Specify whether the device shall be updated or 
 *   the firmware shall be started from SDRAM based on the type of device. 
 */

	NOINT6K_NOFW 

#endif

};

#endif

