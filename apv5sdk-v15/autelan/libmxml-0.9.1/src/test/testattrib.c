/*
   Mini XML lib

   Testattrib; test to verify attribute reading/writing

   Author: Giancarlo Niccolai <gian@niccolai.ws>

   $Id: testattrib.c,v 1.1 2003/08/09 16:46:30 jonnymind Exp $
*/

#include <mxml.h>
#include <mxml_file.h>

int main( int argc, char *argv[] )
{
   MXML_DOCUMENT *doc = mxml_document_new();
   char *buf = (char *) MXML_ALLOCATOR( MXML_FILE_BLOCK_SIZE );
   MXML_REFIL *refil = mxml_refil_new(mxml_refill_from_stream_func, buf, 0, MXML_FILE_BLOCK_SIZE);
   refil->data = stdin;
   MXML_OUTPUT out;
   MXML_ATTRIBUTE *ret;

   printf( "Please, enter a valid attribute description\n" );

   ret = mxml_attribute_read( refil, doc, 0 );

   if ( doc->status == MXML_STATUS_OK ) {
      printf( "\nResulting parse:\n" );
      mxml_output_setup( &out, mxml_output_func_to_stream, 0 );
      out.data = stdout;

      mxml_attribute_write( &out, ret, 0 );
      mxml_attribute_destroy( ret );
      printf("\n");
   }
   else {
      printf("\nSome errors while reading: %s\n", mxml_error_desc( doc->error) );
   }

   mxml_refil_destroy( refil );
   MXML_DELETOR( buf );

   return 0;
}
