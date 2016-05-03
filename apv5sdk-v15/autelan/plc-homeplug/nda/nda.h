/*====================================================================*
 *   
 *   Copyright (c) 2011 by Qualcomm Atheros.
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
 *   nda.h - Atheros restricted definitions and declarations;
 *
 *   this file contains definitions and declarations that require
 *   a formal non-disclosure agreement with Atheros Communications;
 *
 *   this file, and associated nda folder, are omitted from standard
 *   toolkit releases;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

#ifndef NDA_HEADER
#define NDA_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../plc/plc.h"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define MID_FIRMWARE 0
#define MID_SOFTLOADER 1
#define MID_USERPIB 2
#define MID_FACTPIB 3
#define MID_USERPIBALT 4
#define MID_FACTPIBALT 5
#define MID_CUSTMODULE 6

#define ACCESS_LEVEL_KEY_TYPE_DAK 1
#define ACCESS_LEVEL_PTS 1

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

signed EraseFlashMemory (struct plc *);
signed EraseFlashMemory1 (struct plc *);
signed EraseFlashMemory2 (struct plc *);
signed ModuleErase (struct plc * plc, struct vs_module_spec * vs_module_spec);
signed EraseFlashSector (struct plc *);
signed ReadFlashMemory (struct plc *);
signed ReadFlashMemory1 (struct plc *);
signed ReadFlashMemory2 (struct plc *);
signed ReadFlashParameters (struct plc *);
signed ReadFlashFirmware (struct plc *);
signed RandomAddress (struct plc *);
signed FlashPTS (struct plc *);
signed NetworkProbe (struct plc *);
signed Sniffer (struct plc *);
signed Loopback (struct plc *, void * memory, size_t extent);
signed Monitor (struct plc *, signed colon, signed space);
signed AccessLevelPTS (struct plc *);

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

