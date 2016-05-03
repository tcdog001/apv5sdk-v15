/*====================================================================*
 *
 *   scan.c -
 *
 *   scan.h 
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCAN_SOURCE
#define SCAN_SOURCE

#include "../scan/copytoken.c"
#include "../scan/copyquote.c"
#include "../scan/havebreak.c"
#include "../scan/haveclass.c"
#include "../scan/havematch.c"
#include "../scan/havetoken.c"
#include "../scan/isbreak.c"
#include "../scan/isclass.c"
#include "../scan/ismatch.c"
#include "../scan/isready.c"
#include "../scan/istoken.c"
#include "../scan/nextbreak.c"
#include "../scan/nexttoken.c"
#include "../scan/scanalpha.c"
#include "../scan/scanalnum.c"
#include "../scan/scanblock.c"
#include "../scan/scanblockmatch.c"
#include "../scan/scanbreak.c"
#include "../scan/scanbreakmatch.c"
#include "../scan/scandigit.c"
#include "../scan/scanchunk.c"
#include "../scan/scancntrl.c"
#include "../scan/scanflush.c"
#include "../scan/scangroup.c"
#include "../scan/scanident.c"
#include "../scan/scaninput.c"
#include "../scan/scanlimit.c"
#include "../scan/scanprint.c"
#include "../scan/scanquote.c"
#include "../scan/scanquotematch.c"
#include "../scan/scanreset.c"
#include "../scan/scanspace.c"
#include "../scan/scanstart.c"
#include "../scan/scantoken.c"
#include "../scan/scanuntil.c"
#include "../scan/scanuntilmatch.c"
#include "../scan/scanwhile.c"
#include "../scan/scanwhilematch.c"
#include "../scan/scanwrite.c"
#include "../scan/showtoken.c"
#include "../scan/tokensize.c"
#include "../scan/tokentext.c"
#include "../scan/tokentype.c"
#include "../scan/tovalue.c"

#endif

