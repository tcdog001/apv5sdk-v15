/*
   Mini XML lib

   Test; reads a file from stdin and the write its to std out

   Author: Giancarlo Niccolai <gian@niccolai.ws>

   $Id: mxml_test.c,v 1.2 2003/06/30 18:20:49 jonnymind Exp $
*/

#include <mxml.h>
#include <mxml_file.h>

int main( int argc, char *argv[] )
{
   MXML_DOCUMENT *doc = mxml_document_new();
   MXML_ITERATOR iter;
   MXML_NODE *node;
   FILE *fp;

   if ( argc == 1 )
      fp = stdin;
   else {
      fp = fopen( argv[1], "r" );
      if ( fp == NULL ) {
         printf( "Error: can't open %s\n", argv[1] );
         return 1;
      }
   }

   mxml_read_file( fp, doc, 0 );

   if ( doc->status == MXML_STATUS_ERROR ) {
      printf( "ERROR while reading the document: (%d) %s\n",
         doc->error, mxml_error_desc( doc->error ) );
   }
   else if ( doc->status == MXML_STATUS_MALFORMED ) {
      printf( "Invalid XML document. Line %d: (%d) %s\n",
         doc->iLine, doc->error, mxml_error_desc( doc->error ) );
   }

   printf( "---Begin of the new document (found: %d nodes)---\n\n", doc->node_count );
   mxml_write_file( doc, stdout,
         MXML_STYLE_INDENT | MXML_STYLE_THREESPACES );
   printf( "\n\n---End of the new document---\n\n" );


   // drawing the path structure
   printf( "Path list (dynamic):\n" );
   mxml_iterator_setup( &iter, doc );
   while ( ( node = mxml_iterator_next( &iter ) ) != NULL ) {
      char *path = mxml_node_get_path_new( node );
      printf( "* %s, (depth: %d, length %d)\n", path,
         mxml_node_get_path_depth( node ),
         mxml_node_get_path_length( node ) );
      free( path );
   }
   printf( "------------------------\n\n" );

   // drawing the path structure
   printf( "Path list (static):\n" );
   mxml_iterator_setup( &iter, doc );
   while ( ( node = mxml_iterator_next( &iter ) ) != NULL ) {
      char path[1024];
      int len = mxml_node_get_path( node, path );
      printf( "* %s (%d)\n", path, len );
   }

   if ( argc > 2 ) {
      printf( "-------------\nScanning for node named: %s\n", argv[2] );
      mxml_iterator_setup( &iter, doc );
      node = mxml_iterator_scan_node( &iter, argv[2] );
      if ( node == NULL ){
         printf( "Impossible to find it!\n" );
      }
      else {
         MXML_OUTPUT out;
         MXML_SGS *sgs;

         mxml_output_setup( &out, mxml_output_func_to_sgs, 1 );

         printf( "Found it! here it is its dump:\n-----------\n" );

         while ( node != NULL ) {
            sgs = mxml_sgs_new();
            out.data = sgs;
            mxml_node_write( &out, node, MXML_STYLE_NOESCAPE );
            printf( "---------- BUFFERED (unescaped) -------\n%s", sgs->buffer );
            mxml_sgs_destroy( sgs );
            mxml_iterator_next( &iter );
            node = mxml_iterator_scan_node( &iter, argv[2] );

         }
      }
   }

   if ( argc > 3 ) {
      printf( "-------------\nClone and append: %s\n", argv[3] );
      mxml_iterator_setup( &iter, doc );
      node = mxml_iterator_scan_node( &iter, argv[3] );
      if ( node == NULL ){
         printf( "Impossible to find it!\n" );
      }
      else {
         MXML_ITERATOR it2;
         mxml_iterator_setup( &it2, doc );

         node = mxml_node_clone_tree( iter.node );

         mxml_iterator_add_below( &it2, node );

         mxml_write_file( doc, stdout, MXML_STYLE_INDENT | MXML_STYLE_THREESPACES );
      }
   }

   return 0;
}
