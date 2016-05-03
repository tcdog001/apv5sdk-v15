/*
   Mini XML lib

   Document routines

   Author: Giancarlo Niccolai <gian@niccolai.ws>

   $Id: mxml_document.c,v 1.2 2003/07/24 20:27:40 jonnymind Exp $
*/

#include <mxml.h>
#include <stdio.h>

MXML_DOCUMENT *mxml_document_new()
{
   MXML_DOCUMENT *doc =(MXML_DOCUMENT *) MXML_ALLOCATOR( sizeof( MXML_DOCUMENT ) );

   if ( doc != NULL )
      mxml_document_setup( doc );

   return doc;
}

MXML_STATUS mxml_document_setup( MXML_DOCUMENT *doc )
{
   doc->iLine = 0;
   doc->name = NULL;
   doc->index = mxml_index_new();
   doc->node_count = 0;
   doc->root = mxml_node_new();

   if ( doc->root != NULL && doc->index != NULL ) {
      doc->root->type = MXML_TYPE_DOCUMENT;
      doc->status = MXML_STATUS_OK;
      doc->error = MXML_ERROR_NONE;
      return MXML_STATUS_OK;
   }

   doc->status = MXML_STATUS_ERROR;
   doc->error = MXML_ERROR_NOMEM;
   return MXML_STATUS_ERROR;
}

void mxml_document_destroy( MXML_DOCUMENT *doc )
{
   mxml_index_destroy( doc->index );
   MXML_DELETOR( doc );
}


MXML_STATUS mxml_document_find( MXML_ITERATOR *it, char *path )
{
   return MXML_STATUS_OK;
}
