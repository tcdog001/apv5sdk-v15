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
 *   ihpapi.c - HomePlug AV Application Programming Interface;
 *
 *   ihpapi.h
 *
 *   compile one monolithic module from all Intellon HomePlug AV API
 *   modules; this can be used as, or instead of, a link library;
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

#ifndef IHPAPI_SOURCE
#define IHPAPI_SOURCE
 
/*====================================================================*
 *   external source files; see ihpapi.h;
 *--------------------------------------------------------------------*/

#include "../ihpapi/GetConnectionInfo.c"
#include "../ihpapi/GetManufacturerInfo.c"
#include "../ihpapi/GetNetworkInfo.c"
#include "../ihpapi/GetVersionInfo.c"
#include "../ihpapi/ResetDevice.c"
#include "../ihpapi/ResetToFactoryDefaults.c"
#include "../ihpapi/InitializeDevice.c"
#include "../ihpapi/SetKey.c"
#include "../ihpapi/UpdateDevice.c"
#include "../ihpapi/RxFrame.c"
#include "../ihpapi/TxFrame.c"
#include "../ihpapi/SequenceInProgress.c"
#include "../ihpapi/SequenceTerminate.c"
#include "../ihpapi/ManageEnetPHY.c"
#include "../ihpapi/GetToneMap.c"
#include "../ihpapi/GetRxToneMap.c"
#include "../ihpapi/SetSDRAMConfig.c"
 
/*====================================================================*
 *   internal source files; see ihp.h;
 *--------------------------------------------------------------------*/

#include "../ihpapi/EncodeEthernetHeader.c"
#include "../ihpapi/EncodeIntellonHeader.c"
#include "../ihpapi/DecodeEthernetPHYSettings.c"
#include "../ihpapi/DecodeFlashNVM.c"
#include "../ihpapi/DecodeGetNVM.c"
#include "../ihpapi/DecodeGetVersion.c"
#include "../ihpapi/DecodeHostAction.c"
#include "../ihpapi/DecodeLinkStats.c"
#include "../ihpapi/DecodeMfgString.c"
#include "../ihpapi/DecodeNetworkInfo.c"
#include "../ihpapi/DecodeReadModule.c"
#include "../ihpapi/DecodeResetDevice.c"
#include "../ihpapi/DecodeResetToFactoryDefaults.c"
#include "../ihpapi/DecodeSetKey.c"
#include "../ihpapi/DecodeStartMAC.c"
#include "../ihpapi/DecodeWriteModule.c"
#include "../ihpapi/DecodeWriteMemory.c"
#include "../ihpapi/DecodeGetToneMap.c"
#include "../ihpapi/DecodeGetRxToneMap.c"
#include "../ihpapi/DecodeSetSDRAMCfg.c"
#include "../ihpapi/SequenceReadModule.c"
#include "../ihpapi/SequenceWriteModule.c"
#include "../ihpapi/SequenceWriteMemory.c"
#include "../ihpapi/FindFWHeader.c"
 
/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif
 

