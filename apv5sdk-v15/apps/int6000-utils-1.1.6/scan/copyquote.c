/*====================================================================*
 *
 *   void copyquote (SCAN * content, char buffer [], signed length);
 *
 *   scan.h
 *
 *   copy the current token to a user supplied buffer of specified 
 *   length; the token is assumed to be enclosed in quotes of some
 *   kind which are discarded before the token is copied;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef COPYQUOTE_SOURCE
#define COPYQUOTE_SOURCE

# include "../scan/scan.h"

void copyquote (SCAN * content, char buffer [], signed length) 

{
	content->first++;
	content->final--;
	copytoken (content, buffer, length);
	content->first--;
	content->final++;
	return;
}

#endif

