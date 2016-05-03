/*====================================================================*
 *   
 *   Copyright (c) 2010, Atheros Communications Inc.
 *   
 *   Permission to use, copy, modify, and/or distribute this software 
 *   for any purpose with or without fee is hereby granted, provided 
 *   that the above copyright notice and this permission notice appear 
 *   in all copies.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL 
 *   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED 
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL  
 *   THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR 
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 *   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
 *   NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *   
 *--------------------------------------------------------------------*/

/*====================================================================*
 * 
 *   debug.h - Debugging Definitions and Declarations;
 *
 *   this file is a subset of the original that includes only those
 *   definitions and declaration needed for toolkit programs;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef DEBUG_HEADER
#define DEBUG_HEADER

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define INTELLON_DEBUG 1
#define INTELLON_TRACE 1

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#if INTELLON_TRACE
#define EVENT(s) printk(KERN_DEBUG "---[ %s ]---\n", (s))
#define ENTER printk(KERN_DEBUG "-->[ %s:%d ]\n", __FUNCTION__,__LINE__)
#define ABORT printk(KERN_DEBUG "<--( %s:%d )\n", __FUNCTION__,__LINE__)
#define LEAVE printk(KERN_DEBUG "<--[ %s:%d ]\n", __FUNCTION__,__LINE__)
#define CRUMB printk(KERN_DEBUG "... %s:%d\n",__FUNCTION__,__LINE__) 
#else
#define EVENT {}
#define ENTER {}
#define ABORT {}
#define LEAVE {}
#define CRUMB {}
#endif

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#if INTELLON_DEBUG
#define address(e) if (!(e)) { printk ("%s(%d): address %s is null\n",__FUNCTION__,__LINE__,#e); }
#define confirm(e) if (!(e)) { printk ("%s(%d): assertion %s is wrong\n",__FUNCTION__,__LINE__, #e); }
#else
#define address(e) {}
#define confirm(e) {}
#endif

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

