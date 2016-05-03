/*====================================================================*
 *
 *   timer.c - echo system time in seconds;
 *
 *   echo the system time in seconds to stdout; truncate the string 
 *   to six digits; 
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

/*====================================================================*"
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define DIGITS 6

/*====================================================================*
 *
 *   int main (int argc, char * argv []);
 *   
 *   
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

int main (int argc, char * argv []) 

{
	char string [DIGITS];
	signed digit = sizeof (string);
	time_t timer = time ((time_t *)(0));
	while (digit > 0) 
	{
		string [--digit] = "0123456789" [timer % 10];
		timer /= 10;
	}
	write (STDOUT_FILENO, string, sizeof (string));
	return (0);
}

