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
 *   void  MACPasswords (uint32_t vendor, uint32_t device, unsigned number, unsigned count, unsigned bunch, unsigned space, flag_t flags);
 *
 *   keys.h
 *
 *   print a range of device address/password pairs on stdout; print 
 *   an optional usage flag in the first column for PTS compatability;
 *
 *   vendor is the 24-bit OUI expressed as an integer; device is the 
 *   24-bit starting unit address expressed as an integer; number is 
 *   the number of address/password pairs to generate; count is the 
 *   number of letters in the password excluding delimiters; 
 *
 *   passwords consists of letters arranged in groups separated by
 *   spaces; count is the number of letters; bunch is the number of 
 *   letters in each group; space is the character that separates 
 *   each group;
 *   
 *   vendor is used to seed the random number generator and create 
 *   a character set having the 256 random upper case letters used 
 *   for all vendor passwords; most letters will appear more than 
 *   once in the character set;
 *
 *   device is used to seed the random number generator again and
 *   select count random letters from the character set until the 
 *   password has been constructed;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef MACPASSWORDS_SOURCE
#define MACPASSWORDS_SOURCE

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <netinet/in.h>

#include "../tools/types.h"
#include "../tools/flags.h"
#include "../crypt/keys.h"

static uint64_t our_seed = 0;
static void our_srand (uint32_t seed) 

{
	our_seed = (uint64_t)(seed);
	return;
}

static uint32_t our_rand () 

{
	our_seed *= 0x41C64E6D;
	our_seed += 0x00003029;
	return ((uint32_t)((our_seed >> 0x10) & 0x7FFFFFFF));
}

static void MACPassword (uint32_t device, const char charset [], unsigned members, unsigned letters, unsigned bunch, char space) 

{
	unsigned letter = 0;
	our_srand (device);
	while (letter < letters) 
	{
		unsigned member = our_rand () % members;
		if ((letter) && (bunch) && !(letter%bunch)) 
		{
			putc (space, stdout);
		}
		putc (charset [member], stdout);
		letter++;
	}
	return;
}

void MACPasswords (uint32_t vendor, uint32_t device, unsigned number, unsigned count, unsigned bunch, unsigned space, flag_t flags) 

{
	char charset [UCHAR_MAX + 1];
	unsigned members = 0;

#if INTELLON_SAFEMODE 

	if (vendor >> 24) 
	{
		return;
	}
	if (device >> 24) 
	{
		return;
	}
	if (number >> 24) 
	{
		return;
	}

#endif

	our_srand (vendor);
	while (members < sizeof (charset)) 
	{
		unsigned c = our_rand () % (SCHAR_MAX + 1);
		if (isupper (c)) 
		{
			charset [members++] = c;
		}
	}
	while (number--) 
	{
		if (_anyset (flags, PASSWORD_VERBOSE)) 
		{
			putc ('0', stdout);
			putc (' ', stdout);
		}
		if (_allclr (flags, PASSWORD_SILENCE)) 
		{
			printf ("%06X", vendor & VENDOR_MAX);
			printf ("%06X", device & DEVICE_MAX);
			putc (' ', stdout);
		}
		MACPassword (device, charset, members, count, bunch, space);
		putc ('\n', stdout);
		device++;
	}
	return;
}

#endif

