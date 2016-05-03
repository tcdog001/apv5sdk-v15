/*====================================================================*
 *
 *   argv.c - display argument vector;
 *
 *   this program is a simple debugging tool that displays the argument
 *   vector argv[] on stdout for inspection.
 *
 *   use it to see how your host system processes command line arguments;
 *
 *.  published 2003 by charles maier associates limited for internal use;
 *:  modified sun 07 sep 03 by user root on host zeus;
 *;  compiled for debian gnu/linux using gcc compiler;
 *
 *--------------------------------------------------------------------*/

#include <stdio.h>

int main (int argc, const char * argv []) 

{
	const char ** strp;
	for (strp = argv; *strp; strp++) 
	{
		printf (" argv[%d] = [%s]\n", strp - argv, * strp);
	}
	printf (" argv[%d] = NULL\n", strp - argv);
	return (0);
}

