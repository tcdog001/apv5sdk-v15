

/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: mxml_file.h
* description:  the header files for file oriented operations on mxml libs
* 
*
* 
************************************************************************************/

#ifndef MXML_FILE_H
#define MXML_FILE_H

#include <stdio.h>

/* Operations on the whole doc strucuture */
MXML_DOCUMENT *mxml_read( char *filename, int style );
MXML_STATUS mxml_read_file( FILE *fp, MXML_DOCUMENT *doc, int style );

MXML_STATUS mxml_write( MXML_DOCUMENT *doc, char *filename, int style );
MXML_STATUS mxml_write_file( MXML_DOCUMENT *doc, FILE *fp, int style );

#endif
