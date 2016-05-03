/*====================================================================*
 *
 *   html.h - html related definitions and declarations;
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef HTML_HEADERER
#define HTML_HEADERER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/version.h"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define ISO_CHARSET "iso-8859-1"
#define ISO_CONTENT "text/html"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define W3C_STD "-//W3C//DTD XHTML 1.0 Strict//EN"
#define W3C_DTD "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define XML_VERSION "1.0"
#define XML_CHARSET ISO_CHARSET
#define XML_NAMESPACE "http://www.w3c.org/1999/xhtml"
#define XML_LANGUAGE "en-us"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define CSS_STYLESHEET "/styles/cmassoc.css"
#define CSS_CONTENT "text/css"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define LABEL_DATA "data"
#define LABEL_BASE "base"
#define LABEL_SIZE "size"
#define LABEL_NAME "name"
#define LABEL_TYPE "type"
#define LABEL_TEXT "text"

#define LABEL_TOPLINK "headerlink"
#define LABEL_BOTLINK "footerlink"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define HTML_CONTENT ISO_CONTENT ";" ISO_CHARSET
#define HTML_PROGRAM PACKAGE " " VERSION "." RELEASE
#define HTML_AUTHOR CONTACT
#define HTML_ROBOTS "noindex,nofollow"

#define HTML_PREV "PREV"
#define HTML_HOME "HOME"
#define HTML_NEXT "NEXT"

#define HTML_PATH "index.html"
#define HTML_NAME "Index Page"

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

const char * strenv (const char * vector [], const char *string, unsigned c);

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#endif

