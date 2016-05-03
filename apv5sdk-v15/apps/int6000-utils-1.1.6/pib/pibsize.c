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
 *   void pibsize ();
 *
 *   pib.h
 *
 *   display PIB section offsets and lengths in bytes; 
 *
 *   this function is not needed for proper toolkit operation; it is
 *   used for testing and maintenance and may be deleted;
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
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef PIBSIZE_SOURCE
#define PIBSIZE_SOURCE

#include <stdio.h>

#include "../pib/pib.h"

#ifndef offset
#define offset(struct, member) (signed)(&struct.member)-(signed)(&struct)
#endif

int main (int argc, const char * argv []) 

{
	struct PIB3_3 PIB;
	printf ("offset length\n");
	printf ("0x%04X %6d PIB.VersionHeader\n", offset (PIB, VersionHeader), sizeof (PIB.VersionHeader));
	printf ("0x%04X %6d PIB.LocalDeviceConfig\n", offset (PIB, LocalDeviceConfig), sizeof (PIB.LocalDeviceConfig));
	printf ("0x%04X %6d PIB.StaticNetworkConfig\n", offset (PIB, StaticNetworkConfig), sizeof (PIB.StaticNetworkConfig));
	printf ("0x%04X %6d PIB.InterfaceConfig\n", offset (PIB, InterfaceConfig), sizeof (PIB.InterfaceConfig));
	printf ("0x%04X %6d PIB.IGMPConfig\n", offset (PIB, IGMPConfig), sizeof (PIB.IGMPConfig));
	printf ("0x%04X %6d PIB.QoSParameters\n", offset (PIB, QoSParameters), sizeof (PIB.QoSParameters));
	printf ("0x%04X %6d PIB.ToneNotchParameters\n", offset (PIB, ToneNotchParameters), sizeof (PIB.ToneNotchParameters));
	printf ("0x%04X %6d PIB.FeatureConfiguration\n", offset (PIB, FeatureConfiguration), sizeof (PIB.FeatureConfiguration));
	printf ("0x%04X %6d PIB.V3_0Configuration\n", offset (PIB, V3_0Configuration), sizeof (PIB.V3_0Configuration));
	printf ("0x%04X %6d PIB.V3_1Configuration\n", offset (PIB, V3_1Configuration), sizeof (PIB.V3_1Configuration));
	printf ("0x%04X %6d PIB.V3_3Configuration\n", offset (PIB, V3_3Configuration), sizeof (PIB.V3_3Configuration));

// printf ("0x%04X %6d PIB.FeatureGroupEnablement\n", offset (PIB, FeatureGroupEnablement), sizeof (PIB.FeatureGroupEnablement));

	printf ("0x%04X %6d PIB\n", sizeof (PIB), sizeof (PIB));
	return (0);
}

#endif

