#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

static int i64c(int i)
{
	i &= 0x3f;
	if (i == 0)
		return '.';
	if (i == 1)
		return '/';
	if (i < 12)
		return ('0' - 2 + i);
	if (i < 38)
		return ('A' - 12 + i);
	return ('a' - 38 + i);
}

static void crypt_make_salt(char *p, int cnt)
{
	unsigned x = x; /* it's pointless to initialize it anyway :) */

	x += getpid() + time(NULL) + clock();
	do {
		/* x = (x*1664525 + 1013904223) % 2^32 generator is lame
		 * (low-order bit is not "random", etc...),
		 * but for our purposes it is good enough */
		x = x*1664525 + 1013904223;
		/* BTW, Park and Miller's "minimal standard generator" is
		 * x = x*16807 % ((2^31)-1)
		 * It has no problem with visibly alternating lowest bit
		 * but is also weak in cryptographic sense + needs div,
		 * which needs more code (and slower) on many CPUs */
		*p++ = i64c(x >> 16);
		*p++ = i64c(x >> 22);
	} while (--cnt);
	*p = '\0';
}

int main(int argc, char *argv[])
{
char salt[sizeof("$N$XXXXXXXX")]; /* "$N$XXXXXXXX" or "XX" */

    if (argc < 2)
    {
        printf("Usage: crypt <clear> [salt]\n");
        return 1;
    }

    if (argc > 2)
    {
        strcpy(salt, argv[2]);
    }
    else
    {
        strcpy(salt, "$1$");
        crypt_make_salt(salt + 3, 4);
    }
    printf("%s", crypt(argv[1], salt));

    return 0;
}
