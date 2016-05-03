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
 *   weeder.c - Weeder Solid State Relay Control program;
 *
 *.  Intellon Linux Toolkit;
 *:  Snapshot 2006-2008 by Intellon Corporation; charles.maier@intellon.com;
 *;  For demonstration or evaluation only; Not for production use;
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#if defined (__linux__)
#	include <sys/io.h>
#	include <termios.h>
#elif defined (__APPLE__)
#	include <sys/io.h>
#	include <termios.h>
#elif defined (WIN32)
#	include <windows.h>
#else
#error "Unknown Environment"
#endif

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/version.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/files.h"
#include "../tools/flags.h"
#include "../tools/error.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/synonym.c"
#include "../tools/todigit.c"
#include "../tools/error.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define MODES 2
#define UNITS "BACDE"
#define CHARS 7
#define WAIT 10 

#ifdef WIN32
#	define DEVICE "com1:"
#	define SLEEP(n) Sleep(n)
#else
#	define DEVICE "/dev/ttyS0"
#	define SLEEP(n) usleep((n)*1000)
#endif

#define WEEDER_SILENCE (1 << 0)
#define WEEDER_VERBOSE (1 << 1)

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

static const struct _term_ modes [MODES] = 

{
	{
		"off",
		"0" 
	},
	{
		"on",
		"1" 
	}
};

/*====================================================================*
 *
 *   void function0 (struct _file_ * port, const char * units, unsigned wait, unsigned echo);
 *
 *   send echo command to Weeder Solid State Relay modules B then A;
 *   Standard Intellon relay modules were wired in reverse order for 
 *   some reason;
 *
 *.  Intellon Linux Toolkit;
 *:  Snapshot 2006-2008 by Intellon Corporation; charles.maier@intellon.com;
 *;  For demonstration or evaluation only; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void function0 (struct _file_ * port, const char * units, unsigned wait, unsigned echo) 

{
	char buffer [10];
	signed length = 0;
	buffer [length++] = *units++;
	buffer [length++] = 'X';
	buffer [length++] = '0' + (echo & 1);
	buffer [length++] = '\r';
	if (write (port->file, buffer, length) < length) 
	{
		error (1, errno, "Can't write to %s", port->name);
	}
	SLEEP (wait);
	length = 0;
	buffer [length++] = *units++;
	buffer [length++] = 'X';
	buffer [length++] = '0' + (echo & 1);
	buffer [length++] = '\r';
	if (write (port->file, buffer, length) < length) 
	{
		error (1, errno, "Can't write to %s", port->name);
	}
	SLEEP (wait);
	return;
}

/*====================================================================*
 *
 *   void function1 (struct _file_ * port, const char * units, unsigned wait, unsigned mode, unsigned data);
 *
 *   send write command to Weeder Solid State Relay modules B then A;
 *   Standard Intellon relay modules were wired in reverse order for 
 *   some reason;
 *
 *.  Intellon Linux Toolkit;
 *:  Snapshot 2006-2008 by Intellon Corporation; charles.maier@intellon.com;
 *;  For demonstration or evaluation only; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void function1 (struct _file_ * port, const char * units, unsigned wait, unsigned mode, unsigned data) 

{
	char buffer [10];
	signed length = 0;
	buffer [length++] = *units++;
	buffer [length++] = 'W';
	buffer [length++] = '0' + (mode & 1);
	buffer [length++] = '0';
	buffer [length++] = '0';
	while (length < CHARS) 
	{
		buffer [length++] = '0' + (data & 1);
		data >>= 1;
	}
	buffer [length++] = '\r';
	if (write (port->file, buffer, length) < length) 
	{
		error (1, errno, "Can't write to %s", port->name);
	}
	SLEEP (wait);
	length = 0;
	buffer [length++] = *units++;
	buffer [length++] = 'W';
	while (length < CHARS) 
	{
		buffer [length++] = '0' + (data & 1);
		data >>= 1;
	}
	buffer [length++] = '\r';
	if (write (port->file, buffer, length) < length) 
	{
		error (1, errno, "Can't write to %s", port->name);
	}
	SLEEP (wait);
	return;
}

/*====================================================================*
 *
 *   int main (int argc, const char * argv []);
 *
 *
 *
 *.  Intellon Linux Toolkit;
 *:  Snapshot 2006-2008 by Intellon Corporation; charles.maier@intellon.com;
 *;  For demonstration or evaluation only; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"e:m:o:p:iqvw:",
		"",
		"control Weeder programmable attenuator module",
		"e n\techo is (n) [0]",
		"m n\tmode is (n) [1]",
		"o s\tunit order is (s) [" UNITS "]",
		"p f\tport is (f) [" DEVICE "]",
		"q\tquiet mode",
		"v\tverbose mode",
		"w n\twait (n) millseconds [10]",
		(const char *) (0) 
	};
	struct _file_ port = 
	{
		-1,
		DEVICE 
	};

#if defined (__linux__)

	struct termios termios;

#elif defined (WIN32)

	HANDLE hSerial;
	DCB dcbSerial = 
	{
		0 
	};

#else
#error "Unknown environment"
#endif

	const char * units = UNITS;
	unsigned wait = WAIT;
	unsigned echo = 0;
	unsigned mode = 1;
	unsigned data = 0;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'e':
			echo = (unsigned)(uintspec (synonym (optarg, modes, MODES), 0, 1));
			break;
		case 'm':
			mode = (unsigned)(uintspec (synonym (optarg, modes, MODES), 0, 1));
			break;
		case 'o':
			units = optarg;
			break;
		case 'p':
			port.name = optarg;
			break;
		case 'w':
			wait = (unsigned)(uintspec (optarg, 5, 100));
			break;
		case 'q':
			_setbits (flags, WEEDER_SILENCE);
			break;
		case 'v':
			_setbits (flags, WEEDER_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if ((argc) && (*argv)) 
	{
		data = (unsigned)(uintspec (*argv, 0, 0x7F));
	}

#if defined (WIN32)

	hSerial = CreateFile (port.name, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hSerial == INVALID_HANDLE_VALUE) 
	{
		error (1, 0, "Can't open %s", port.name);
	}
	dcbSerial.DCBlength = sizeof (dcbSerial);
	if (!GetCommState (hSerial, &dcbSerial)) 
	{
		error (1, 0, "Can't read %s state", port.name);
	}
	dcbSerial.BaudRate = CBR_9600;
	dcbSerial.ByteSize = 8;
	dcbSerial.StopBits = ONESTOPBIT;
	dcbSerial.Parity = NOPARITY;
	if (!SetCommState (hSerial, &dcbSerial)) 
	{
		error (1, 0, "Can't save %s state", port.name);
	}
	CloseHandle (hSerial);
	if ((port.file = open (port.name, O_RDWR)) == -1) 
	{
		error (1, errno, "Can't open %s", port.name);
	}

#elif defined (__linux__)

	if ((port.file = open (port.name, O_RDWR|O_NOCTTY|O_NDELAY)) == -1) 
	{
		error (1, 0, "Can't open %s", port.name);
	}
	tcgetattr (port.file, &termios);
	termios.c_cflag = CS8;
	cfsetospeed (&termios, B9600);
	tcsetattr (port.file, TCSANOW, &termios);

#else
#error "Unknown environment"
#endif

	function0 (&port, units, wait, echo);
	function1 (&port, units, wait, mode, data);
	close (port.file);
	exit (0);
}

