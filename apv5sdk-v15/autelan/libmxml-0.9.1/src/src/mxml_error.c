/*
   Mini XML lib

   Traslation from error code to error description 

   Author: Giancarlo Niccolai <gian@niccolai.ws>

   $Id: mxml_error.c,v 1.2 2003/06/30 18:20:49 jonnymind Exp $
*/

#include <mxml.h>
#include <stdio.h>

static char *edesc[] =
{
   "Input/output error",
   "Not enough memory",
   "Character outside tags",
   "Invalid character as tag name",
   "Invalid character as attribute name",
   "Malformed attribute definition",
   "Invalid character",
   "Name of tag too long",
   "Name of attribute too long",
   "Value of attribute too long",
   "Unbalanced tag opening",
   "Unbalanced entity opening",
   "Escape/entity '&;' found"
};

char *mxml_error_desc( MXML_ERROR_CODE code )
{
   code --;
   if ( code < 0 || code > sizeof( edesc ) / sizeof( char * ) )
      return NULL;

   return edesc[ code ];
}
