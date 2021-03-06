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
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/endian.h"
#include "../tools/number.h"
#include "../tools/chars.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../mdio/mdio.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/error.c"
#include "../tools/todigit.c"
#endif

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

static signed c;
static unsigned row = 1;
static unsigned col = 1;
static uint16_t count = 0;
static uint16_t instr = 0;
static uint32_t addr;
static uint32_t data;
static uint32_t mask;

/*====================================================================*
 *   
 *   signed mygetc ();
 *   
 *   return next input character after updating the cursor position;
 *
 *.  Atheros Powerline Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed mygetc () 

{
	extern unsigned row;
	extern unsigned col;
	signed c = getc (stdin);
	if (c == '\n') 
	{
		row++;
		col = 0;
	}
	else if (~c)
	{
		col++;
	}
	return (c);
}


/*====================================================================*
 *
 *   uint32_t integer (unsigned radix);
 *
 *
 *.  Atheros Powerline Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static uint32_t integer (unsigned radix) 

{
	uint32_t value = 0;
	unsigned digit = 0;
	while ((digit = todigit (c)) < radix) 
	{
		value *= radix;
		value += digit;
		c = mygetc ();
	}
	while (isspace (c)) 
	{
		c = mygetc ();
	}
	return (value);
}


/*====================================================================*
 *
 *   void assemble (flag_t flags);
 *
 *   read stdin and write stdout; convert hexadecimal input to binary
 *   MDIO register instructions;
 *
 *   the input file consists of zero or more hexadecimal instructions
 *   consisting of register address, register data and register mask;
 *   instructions are terminated with semicolon; fields are separated
 *   by white space; scriptstyle comments are permitted between 
 *   instructions but not between instruction fields;
 *
 *   the output file will consist of one 16-bit program header plus 
 *   nine 16-bit MDIO instructions for each input instruction; the
 *   output is padded to the nearest multiple of 32-bits;
 *
 *.  Atheros Powerline Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void assemble (flag_t flags) 

{
	c = mygetc ();
	while (c != EOF) 
	{
		if (isspace (c)) 
		{
			do 
			{
				c = mygetc ();
			}
			while (isspace (c));
			continue;
		}
		if ((c == '#') || (c == ';')) 
		{
			do 
			{
				c = mygetc ();
			}
			while (nobreak (c));
			continue;
		}
		addr = integer (16);
		data = integer (16);
		mask = integer (16);
		instr = MDIO16_INSTR (1, 1, (0x03 << 3), 0, 2);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		instr = HTOLE16 ((addr >> 9) & 0x03FF);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		instr = HTOLE16 (0xFFFF);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		count++;
		instr = MDIO16_INSTR (1, 1, (0x02 << 3) | ((MDIO32_LO_ADDR (addr) & 0xE0) >> 5), MDIO32_LO_ADDR (addr) & 0x1F, 2);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		instr = HTOLE16 (data & 0xFFFF);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		instr = HTOLE16 (mask & 0xFFFF);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		count++;
		instr = MDIO16_INSTR (1, 1, (0x02 << 3) | ((MDIO32_LO_ADDR (addr) & 0xE0) >> 5), (MDIO32_LO_ADDR (addr) & 0x1F) | 0x01, 2);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		instr = HTOLE16 ((data >> 16) & 0xFFFF);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		instr = HTOLE16 ((mask >> 16) & 0xFFFF);
		write (STDOUT_FILENO, &instr, sizeof (instr));
		count++;
		if (_anyset (flags, MDIO_VERBOSE)) 
		{
			fprintf (stderr, "REG=0x%08X DATA=0x%08X MASK=0x%08X\n", addr, data, mask);
		}
		if ((c == ';') || (c == EOF)) 
		{
			c = mygetc ();
			continue;
		}
		if (_allclr (flags, MDIO_SILENCE)) 
		{
			error (1, 0, "Illegal character or missing terminator: line %d col %d", row, col);
		}
	}
	return;
}


/*====================================================================*
 *
 *   int main (int argc, const char * argv []);
 *   
 *   
 *.  Atheros Powerline Toolkit for HomePlug AV; 
 *:  Published 2010 by Atheros Communication; ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char * optv [] = 
	{
		"qv",
		"file [ file ] [ ...] > file",
		"Atheros Clause 45 MDIO Instruction Block Assembler",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0)
	};
	flag_t flags = (flag_t)(0);
	optind = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'q':
			_setbits (flags, MDIO_SILENCE);
			break;
		case 'v':
			_setbits (flags, MDIO_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (isatty (STDOUT_FILENO)) 
	{
		error (1, ECANCELED, "stdout must be a file or pipe");
	}
	instr = MDIO16_START (1, 0, count);
	write (STDOUT_FILENO, &instr, sizeof (instr));
	if (!argc) 
	{
		assemble (flags);
	}
	while ((argc) && (* argv)) 
	{
		if (!freopen (* argv, "rb", stdin)) 
		{
			error (1, errno, "%s", * argv);
		}
		assemble (flags);
		argc--;
		argv++;
	}
	instr = MDIO16_START (1, 0, count);
	addr = count * sizeof (instr) + sizeof (instr);
	data = 0;
	if ((addr % sizeof (data))) 
	{
		write (STDOUT_FILENO, &data, sizeof (data) - addr % sizeof (data));
	}
	if (!lseek (STDOUT_FILENO, 0, SEEK_SET)) 
	{
		write (STDOUT_FILENO, &instr, sizeof (instr));
	}
	if (_anyset (flags, MDIO_VERBOSE)) 
	{
		fprintf (stderr, "%d instructions\n", count);
	}
	return (0);
}

