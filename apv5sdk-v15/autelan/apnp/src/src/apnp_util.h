#ifndef _APNP_UTIL_H
#define _APNP_UTIL_H

#include "ithread.h"
#include "ixml.h" /* for IXML_Document, IXML_Element */
#include "upnp.h" /* for Upnp_EventType */
#include "upnptools.h"
#include "upnpdebug.h"

/* for public head file */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <stdarg.h>

#define LOG_EMERG		0       /* system is unusable */
#define LOG_ALERT		1       /* action must be taken immediately */
#define LOG_CRIT			2       /* critical conditions */
#define LOG_ERR			3       /* error conditions */
#define LOG_WARNING	4       /* warning conditions */
#define LOG_NOTICE		5       /* normal but significant condition */
#define LOG_INFO		6       /* informational */
#define LOG_DEBUG		7       /* debug-level messages */


#endif
