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
 *   const char * MMECode (uint16_t MMTYPE, uint8_t MSTATUS) 
 *
 *   mme.h  
 *
 *   return text for a given message type and status code; search is
 *   performed using nested binary searchs; 
 *
 *   the bootloader and firmware share some message types but return 
 *   different status codes for the same message type and error condition;
 *   differences are enabled and disabled by constant SOFTLOADER;
 *
 *   two MMEs in particular, VS_WR_MEM and VS_ST_MAC are normally
 *   used only with the Bootloader and so we define SOFTLOADER as 1
 *   causing Bootloader/Softloader codes to replace the firmware
 *   codes;
 *
 *   this function increases program size by about 3k due to symbol 
 *   table size; define MMERRORS as 0 to suppress the table without 
 *   disturbing other code;
 *
 *   Intellon vendor specific status codes are reused; a given code
 *   may mean something different for each message type; this table
 *   maps message types and codes to strings; it is bulky but takes
 *   less space than lots of little tables;
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

#ifndef MMERROR_SOURCE
#define MMERROR_SOURCE

#include <stdio.h>
#include <stdint.h>

#include "../tools/symbol.h"

#define MMERRORS 1

#if MMERRORS

#define SOFTLOADER
#ifdef SOFTLOADER
#	define MMECODES 176
#else
#	define MMECODES 170
#endif

static struct mmecode 

{
	uint16_t type;
	uint8_t code;
	const char * text;
}

MMECodes [MMECODES] = 

{

#ifdef SOFTLOADER

	{
		0xA005,
		0x14,
		"Bad Checksum" 
	},
	{
		0xA005,
		0x1C,
		"Bad Length" 
	},
	{
		0xA005,
		0x38,
		"Bad Address" 
	},
	{
		0xA005,
		0x3C,
		"Bad Data Alignment" 
	},

#else

	{
		0xA005,
		0x10,
		"Bad Address" 
	},
	{
		0xA005,
		0x14,
		"Bad Length" 
	},

#endif

	{
		0xA009,
		0x10,
		"Bad Offset" 
	},
	{
		0xA009,
		0x14,
		"Bad Length" 
	},
	{
		0xA00D,
		0x10,
		"Bad Module ID" 
	},

#ifdef SOFTLOADER 

	{
		0xA00D,
		0x14,
		"Bad Image Checksum" 
	},
	{
		0xA00D,
		0x1C,
		"Bad Image Length" 
	},
	{
		0xA00D,
		0x38,
		"Bad Image Load Address" 
	},
	{
		0xA00D,
		0x3C,
		"Bad Data Alignment" 
	},
	{
		0xA00D,
		0x40,
		"Bad Start Address" 
	},

#else

	{
		0xA00D,
		0x14,
		"Bad Command" 
	},

#endif

	{
		0xA011,
		0x10,
		"No NVRAM" 
	},
	{
		0xA01D,
		0x01,
		"Device Failed to Reset" 
	},
	{
		0xA01D,
		0x02,
		"Device Busy" 
	},
	{
		0xA021,
		0x10,
		"Bad Module" 
	},
	{
		0xA021,
		0x12,
		"Bad Length" 
	},
	{
		0xA021,
		0x14,
		"Bad Checksum" 
	},
	{
		0xA021,
		0x20,
		"Bad Offset" 
	},
	{
		0xA021,
		0x40,
		"Operation Blocked" 
	},
	{
		0xA021,
		0x50,
		"Fail to lock NVM" 
	},
	{
		0xA025,
		0x10,
		"Bad Module" 
	},
	{
		0xA025,
		0x12,
		"Bad Length" 
	},
	{
		0xA025,
		0x14,
		"Bad Checksum" 
	},
	{
		0xA025,
		0x20,
		"Unexpected Offset" 
	},
	{
		0xA025,
		0x50,
		"Fail to lock NVM" 
	},
	{
		0xA025,
		0x58,
		"DAK Mismatch" 
	},
	{
		0xA029,
		0x10,
		"Bad Module" 
	},
	{
		0xA029,
		0x14,
		"No NVRAM" 
	},
	{
		0xA029,
		0x18,
		"Not enough NVRAM" 
	},
	{
		0xA029,
		0x1C,
		"Bad Header Checksum" 
	},
	{
		0xA029,
		0x20,
		"Bad Image Checksum" 
	},
	{
		0xA029,
		0x24,
		"Bad PIB" 
	},
	{
		0xA029,
		0x28,
		"Softloader Too Large" 
	},
	{
		0xA029,
		0x2C,
		"Firmware Too Large" 
	},
	{
		0xA029,
		0x42,
		"Firmware without PIB" 
	},
	{
		0xA029,
		0x44,
		"Bad PIB Checksum" 
	},
	{
		0xA029,
		0x46,
		"DAK Not Zero" 
	},
	{
		0xA029,
		0x48,
		"DAC Mismatch" 
	},
	{
		0xA029,
		0x50,
		"MFG HFID Mismatch" 
	},
	{
		0xA029,
		0x52,
		"Bad Bind Factory Defaults" 
	},
	{
		0xA029,
		0x54,
		"Bad Bind Template PIB" 
	},
	{
		0xA029,
		0x56,
		"Bad Bind Working PIB" 
	},
	{
		0xA029,
		0x58,
		"Error Computing PIB Checksum" 
	},
	{
		0xA029,
		0x5A,
		"Bad Bind Scratch PIB" 
	},
	{
		0xA029,
		0x5C,
		"No Firmware Version" 
	},
	{
		0xA029,
		0x5E,
		"Version Mismatch" 
	},
	{
		0xA029,
		0x60,
		"Bad 01PIB Checksum" 
	},
	{
		0xA029,
		0x62,
		"Bad 02PIB Checksum" 
	},
	{
		0xA029,
		0x64,
		"Bad WPIB Checksum" 
	},
	{
		0xA029,
		0x66,
		"Illegal Firmware Revision" 
	},
	{
		0xA031,
		0x01,
		"Bad Control" 
	},
	{
		0xA031,
		0x02,
		"Bad Direction" 
	},
	{
		0xA031,
		0x10,
		"Bad Link ID" 
	},
	{
		0xA031,
		0x20,
		"Bad MAC Address" 
	},
	{
		0xA035,
		0x10,
		"Bad Control" 
	},
	{
		0xA049,
		0x20,
		"Invalid Duration" 
	},
	{
		0xA049,
		0x12,
		"Invalid Length" 
	},
	{
		0xA049,
		0x24,
		"Already Set" 
	},
	{
		0xA051,
		0x10,
		"Bad PEKS" 
	},
	{
		0xA051,
		0x11,
		"Bad PIB" 
	},
	{
		0xA051,
		0x12,
		"Bad PEKS Encrypted Payload" 
	},
	{
		0xA051,
		0x13,
		"Remote Failure" 
	},
	{
		0xA051,
		0x14,
		"Bad Remote Response" 
	},
	{
		0xA051,
		0x15,
		"Remote Decryption Failed" 
	},
	{
		0xA059,
		0x10,
		"No NVRAM" 
	},
	{
		0xA05D,
		0x30,
		"Bad Checksum" 
	},
	{
		0xA05D,
		0x34,
		"BIST Failed" 
	},
	{
		0xA065,
		0x10,
		"No NVRAM" 
	},
	{
		0xA069,
		0x02,
		"Not Supported" 
	},
	{
		0xA06D,
		0x01,
		"Bad Control" 
	},
	{
		0xA06D,
		0x02,
		"Success Write" 
	},
	{
		0xA06D,
		0x03,
		"Bad Write Parameter" 
	},
	{
		0xA071,
		0x01,
		"Bad MAC Address" 
	},
	{
		0xA071,
		0x02,
		"Bad TMSLOT" 
	},
	{
		0xA081,
		0x10,
		"Bad Module ID" 
	},
	{
		0xA081,
		0x14,
		"No Flash Memory" 
	},
	{
		0xA081,
		0x18,
		"Not Enough Flash Memory" 
	},
	{
		0xA081,
		0x1C,
		"Bad Image Header Checksum" 
	},
	{
		0xA081,
		0x20,
		"Bad Image Checksum" 
	},
	{
		0xA081,
		0x24,
		"Invalid PIB" 
	},
	{
		0xA081,
		0x28,
		"Softloader Too Large" 
	},
	{
		0xA081,
		0x2C,
		"Firmware File Too Large" 
	},
	{
		0xA081,
		0x42,
		"Firmware without PIB" 
	},
	{
		0xA081,
		0x44,
		"Bad PIB Checksum" 
	},
	{
		0xA081,
		0x46,
		"DAK Not Zero" 
	},
	{
		0xA081,
		0x48,
		"DAC Mismatch" 
	},
	{
		0xA081,
		0x50,
		"MFG HFID Mismatch" 
	},
	{
		0xA081,
		0x52,
		"Bad Bind Factory Defaults" 
	},
	{
		0xA081,
		0x54,
		"Bad Bind Template PIB" 
	},
	{
		0xA081,
		0x56,
		"Bad Bind Working PIB" 
	},
	{
		0xA081,
		0x58,
		"DAK Mismatch" 
	},
	{
		0xA081,
		0x5A,
		"Failed to Lock NVM" 
	},
	{
		0xA089,
		0x01,
		"Bad Control" 
	},
	{
		0xA089,
		0x02,
		"Bad Volatility" 
	},
	{
		0xA089,
		0x03,
		"Bad Classifier" 
	},
	{
		0xA089,
		0x04,
		"Classifier table full" 
	},
	{
		0xA089,
		0x05,
		"Classifier exists with different action" 
	},
	{
		0xA089,
		0x06,
		"Classifier not found" 
	},
	{
		0xA08D,
		0x10,
		"Refused" 
	},
	{
		0xA08D,
		0x11,
		"Refused" 
	},
	{
		0xA08F,
		0x10,
		"Error" 
	},
	{
		0xA091,
		0x01,
		"Bad MAC" 
	},
	{
		0xA091,
		0x02,
		"Bad Slot" 
	},
	{
		0xA099,
		0x01,
		"Misaligned Address" 
	},
	{
		0xA099,
		0x02,
		"Misaligned Entry Point" 
	},
	{
		0xA099,
		0x04,
		"Foreign Entry Point" 
	},
	{
		0xA099,
		0x08,
		"Bad Checksum" 
	},
	{
		0xA099,
		0x10,
		"Out of Memory" 
	},
	{
		0xA099,
		0x20,
		"Offset/Length Mismatch" 
	},
	{
		0xA099,
		0x40,
		"Memory not Operational" 
	},
	{
		0xA099,
		0x80,
		"Bad Length" 
	},
	{
		0xA0B1,
		0x10,
		"Invalid Number of Module Operations" 
	},
	{
		0xA0B1,
		0x11,
		"Invalid Module Operation ID" 
	},
	{
		0xA0B1,
		0x12,
		"Invalid Session ID" 
	},
	{
		0xA0B1,
		0x13,
		"Invalid Num Module Operation Specific Data" 
	},
	{
		0xA0B1,
		0x14,
		"Module ID not found" 
	},
	{
		0xA0B1,
		0x15,
		"Invalid Module Length" 
	},
	{
		0xA0B1,
		0x16,
		"Invalid Module Index" 
	},
	{
		0xA0B1,
		0x17,
		"Invalid Data Length" 
	},
	{
		0xA0B1,
		0x18,
		"Unexpected Offset" 
	},
	{
		0xA0B1,
		0x19,
		"Invalid Commit Action Code" 
	},
	{
		0xA0B1,
		0x1A,
		"Operation Block by previous Commit" 
	},
	{
		0xA0B1,
		0x1B,
		"Duplicate Module ID/SubID" 
	},
	{
		0xA0B1,
		0x1C,
		"Invalid Module ID/SubID" 
	},
	{
		0xA0B1,
		0x20,
		"NVM not Present" 
	},
	{
		0xA0B1,
		0x21,
		"NVM too small" 
	},
	{
		0xA0B1,
		0x22,
		"Unsupported Flash" 
	},
	{
		0xA0B1,
		0x23,
		"Fail to lock NVM" 
	},
	{
		0xA0B1,
		0x24,
		"Unsupported Flash" 
	},
	{
		0xA0B1,
		0x30,
		"Invalid Module checksum" 
	},
	{
		0xA0B1,
		0x31,
		"Individual Module Error" 
	},
	{
		0xA0B1,
		0x32,
		"Expected module not available in NVM or memory" 
	},
	{
		0xA0B1,
		0x40,
		"Invalid Header Checksum" 
	},
	{
		0xA0B1,
		0x41,
		"Invalid FW Checksum" 
	},
	{
		0xA0B1,
		0x42,
		"Invalid FW Checksum" 
	},
	{
		0xA0B1,
		0x43,
		"Soft-loader is too large" 
	},
	{
		0xA0B1,
		0x44,
		"Invalid PIB Checksum" 
	},
	{
		0xA0B1,
		0x45,
		"No Firmware Version" 
	},
	{
		0xA0B1,
		0x46,
		"FW Commit but no PIB" 
	},
	{
		0xA0B1,
		0x47,
		"Version Mismatch" 
	},
	{
		0xA0B1,
		0x50,
		"Invalid PIB" 
	},
	{
		0xA0B1,
		0x51,
		"DAK not zero" 
	},
	{
		0xA0B1,
		0x52,
		"MAC mismatch" 
	},
	{
		0xA0B1,
		0x53,
		"DAK mismatch" 
	},
	{
		0xA0B1,
		0x54,
		"Mfg HFID mismatch" 
	},
	{
		0xA0B1,
		0x55,
		"Bad bind to FAC. Default PIB" 
	},
	{
		0xA0B1,
		0x56,
		"Bad bind to template PIB" 
	},
	{
		0xA0B1,
		0x57,
		"Bad bind to working PIB" 
	},
	{
		0xA0B1,
		0x58,
		"Bad Bind Scratch PIB" 
	},
	{
		0xA0B1,
		0x59,
		"Error Generating Checksum Scratch PIB" 
	},
	{
		0xA0B1,
		0x5a,
		"Checksum Error O1 PIB" 
	},
	{
		0xA0B1,
		0x5b,
		"Checksum Error O2 PIB" 
	},
	{
		0xA0B1,
		0x5c,
		"Checksum Error Working PIB" 
	},
	{
		0xA0C1,
		0x03,
		"Invalid Classifier" 
	},
	{
		0xA0C1,
		0x04,
		"Already too many Classifiers" 
	},
	{
		0xA0C1,
		0x05,
		"Classifier exists with different Classification Action" 
	},
	{
		0xA0C1,
		0x07,
		"Link Already in Progress" 
	},
	{
		0xA0C1,
		0x08,
		"Invalid Configuration" 
	},
	{
		0xA0C1,
		0x10,
		"Invalid CSPEC Version" 
	},
	{
		0xA0C1,
		0x11,
		"Invalid CSPEC" 
	},
	{
		0xA0C1,
		0x20,
		"Out of Local Resources" 
	},
	{
		0xA0C1,
		0x30,
		"Invalid Peer" 
	},
	{
		0xA0C1,
		0x31,
		"Peer Confirm Timed out" 
	},
	{
		0xA0C1,
		0x40,
		"Peer rejection" 
	},
	{
		0xA0C5,
		0x01,
		"Invalid Modification Control" 
	},
	{
		0xA0C5,
		0x06,
		"CID Not Found" 
	},
	{
		0xA0C5,
		0x07,
		"Link Update in Progress, try again later" 
	},
	{
		0xA0C9,
		0x06,
		"CID Not Found" 
	},
	{
		0xA0C9,
		0x07,
		"Link Update in Progress. Try again later" 
	},
	{
		0xA0C9,
		0x31,
		"Peer Confirm Timed out" 
	},
	{
		0xA0CD,
		0x01,
		"Invalid Request Type" 
	},
	{
		0xA0CD,
		0x04,
		"Too Many Requests",
	},
	{
		0xA0CD,
		0x06,
		"CID Not Found" 
	},
	{
		0xA0CD,
		0x10,
		"Invalid CSPEC Version" 
	},
	{
		0xA0CD,
		0x31,
		"Peer Confirm Timed out" 
	}
};

#endif

const char * MMECode (uint16_t MMTYPE, uint8_t MSTATUS) 

{

#if MMERRORS

	size_t lower = 0;
	size_t upper = MMECODES;
	while (lower < upper) 
	{
		size_t index = (lower + upper) >> 1;
		signed order = MMTYPE - MMECodes [index].type;
		if (order < 0) 
		{
			upper = index - 0;
			continue;
		}
		if (order > 0) 
		{
			lower = index + 1;
			continue;
		}
		for (lower = index; lower > 0; lower--) 
		{
			if (MMECodes [lower - 1].type != MMECodes [index].type) 
			{
				break;
			}
		}
		for (upper = index; upper < MMECODES; upper++) 
		{
			if (MMECodes [upper + 0].type != MMECodes [index].type) 
			{
				break;
			}
		}
		while (lower < upper) 
		{
			index = (lower + upper) >> 1;
			order = MSTATUS - MMECodes [index].code;
			if (order < 0) 
			{
				upper = index - 0;
				continue;
			}
			if (order > 0) 
			{
				lower = index + 1;
				continue;
			}
			return (MMECodes [index].text);
		}
	}

#endif

	return ((MSTATUS)? ("Failure"):("Success"));
}

/*====================================================================*
 *   print error codes and text in .csv and .html format;
 *--------------------------------------------------------------------*/

#if 0

#include <stdio.h>
#include "../mme/MMEName.c"

void MMECodeToCVS () 

{
	unsigned code = 0;
	printf ("Name,Type,Code,Text\n");
	while (code < MMECODES) 
	{
		printf ("%s,", MMEName (MMECodes [code].type));
		printf ("0x%04X,", MMECodes [code].type);
		printf ("0x%02X,", MMECodes [code].code);
		printf ("%s\n", MMECodes [code].text);
		code++;
	}
	return;
}

void MMECodeToHTML () 

{
	unsigned code = 0;
	printf ("<table>\n");
	printf ("\t<tr>\n");
	printf ("\t\t<th>Name</th>\n");
	printf ("\t\t<th>Type</th>\n");
	printf ("\t\t<th>Code</th>\n");
	printf ("\t\t<th>Text</th>\n");
	printf ("\t\t</tr>\n");
	while (code < MMECODES) 
	{
		printf ("\t<tr>\n");
		printf ("\t\t<td>%s</td>\n", MMEName (MMECodes [code].type));
		printf ("\t\t<td>0x%04X</td>\n", MMECodes [code].type);
		printf ("\t\t<td>0x%02X</td>\n", MMECodes [code].code);
		printf ("\t\t<td>%s</td>\n", MMECodes [code].text);
		printf ("\t\t</tr>\n");
		code++;
	}
	printf ("\t</table>\n");
	return;
}

int main (int argc, const char * argv []) 

{
	MMECodeToCVS ();
	MMECodeToHTML ();
	return (0);
}

#endif

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

