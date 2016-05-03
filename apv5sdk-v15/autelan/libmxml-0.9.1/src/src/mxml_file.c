/*
   Mini XML lib

   Opening and reading an xml

   Author: Giancarlo Niccolai <gian@niccolai.ws>

   $Id: mxml_file.c,v 1.3 2003/07/24 20:27:40 jonnymind Exp $
*/

#include <mxml.h>
#include <mxml_file.h>

/*
   Parses a whole document in a DOM field
*/
MXML_DOCUMENT *mxml_read( char *filename, int style )
{
   FILE *fp;
   MXML_DOCUMENT *doc = mxml_document_new();

   if ( doc->status != MXML_STATUS_OK )
      return doc;

   fp = fopen( filename, "r" );
   if ( fp == NULL ) {
      doc->status = MXML_STATUS_ERROR;
      doc->error = MXML_ERROR_IO;
      return doc;
   }

   mxml_read_file( fp, doc, style );

   /* A common error is not to check file close return */
   if ( fclose( fp ) != 0 )
      doc->status = MXML_STATUS_UNDEFINED;

   return doc;
}


MXML_STATUS mxml_read_file( FILE *fp, MXML_DOCUMENT *doc, int iStyle )
{
   MXML_REFIL *refil;
   MXML_STATUS ret;
   char *buf;

   doc->iLine = 1;

   buf = (char *) MXML_ALLOCATOR( MXML_FILE_BLOCK_SIZE );
   refil = mxml_refil_new(mxml_refill_from_stream_func, buf, 0, MXML_FILE_BLOCK_SIZE);
   refil->data = fp;

   ret = mxml_node_read( refil, doc->root, doc, iStyle );

   mxml_refil_destroy( refil );
   MXML_DELETOR( buf );

   return ret;
}


MXML_STATUS mxml_write( MXML_DOCUMENT *doc, char *filename, int style )
{
   FILE *fp;

   fp = fopen( filename, "w" );
   if ( fp == NULL ) {
      return MXML_STATUS_ERROR;
   }

   mxml_write_file( doc, fp, style );

   /* A common error is not to check file close return */
   if ( fclose( fp ) != 0 )
      return MXML_STATUS_ERROR;

   return MXML_STATUS_OK;
}

MXML_STATUS mxml_write_file( MXML_DOCUMENT *doc, FILE *fp, int style )
{
   MXML_OUTPUT out;
   mxml_output_setup( &out, mxml_output_func_to_stream, doc->node_count );
   out.data = fp;

   return mxml_node_write( &out, doc->root, style );
}
