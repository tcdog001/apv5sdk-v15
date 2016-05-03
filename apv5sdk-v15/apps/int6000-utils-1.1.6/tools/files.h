/*====================================================================*
 *
 *   files.h - custom filesystem definitions and declarations;
 *
 *   this file is a subset of the original that includes only those
 *   definitions and declaration needed for toolkit programs;
 *
 *.  Intellon Linux Toolkit; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration and evaluation only; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef FILES_HEADER 
#define FILES_HEADER 

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"

/*====================================================================*
 *   path and file extenders;
 *--------------------------------------------------------------------*/

#ifdef WIN32
#define PATH_C_EXTENDER '\\'
#define FILE_C_EXTENDER '.'
#else
#define PATH_C_EXTENDER '/'
#define FILE_C_EXTENDER '.'
#endif

/*====================================================================*
 *   file create and access modes;
 *--------------------------------------------------------------------*/

#ifdef WIN32
#define FILE_FILEMODE S_IREAD|S_IWRITE
#else
#define FILE_FILEMODE S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define O_BINARY 0
#endif

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

FILE *efopen (const char *filename, const char *openmode);
FILE *efreopen (const char *filename, const char *openmode, FILE *fp);
bool checkfilename (const char * string);
const char * filepart (const char * string);

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#endif

