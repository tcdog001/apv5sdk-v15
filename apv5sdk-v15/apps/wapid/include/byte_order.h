/*-M- byte_order.h --  compatibility file
*  This fixes compilation using e.g.
*       apps/gateway/../../wlan/net80211/ieee80211.h
*  which expects _BYTE_ORDER to be defined as either _BIG_ENDIAN or otherwise.
*
*  Just to make life complicated, there are two competing schemes
*  for __LITTLE_ENDIAN and __BIG_ENDIAN .
*  /usr/include/endian.h defines both of them, setting __BYTE_ORDER to
*  one; this file is included from various other headers.
*  By contrast, <asm/byteorder.h> defines just one of them....
*  but relying on that won't work because endian.h is probably included
*  from somewhere for the same compilation.
*       (above comment by Ted Merrill)       
*/

#ifndef __BYTE_ORDER_H__
#define __BYTE_ORDER_H__

#ifndef _BYTE_ORDER

#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define _BYTE_ORDER _LITTLE_ENDIAN
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
#define _BYTE_ORDER _BIG_ENDIAN
#endif

#endif  // _BYTE_ORDER

#ifndef _BYTE_ORDER
#error "Could not define _BYTE_ORDER in byte_order.h"
#endif

// The following is at least non-portable, if not broken:
//was: #include <include/compat.h>
//was: #define _LITTLE_ENDIAN  1234    /* LSB first: i386, vax */
//was: #define _BIG_ENDIAN 4321    /* MSB first: 68000, ibm, net */
//was: #include <asm/byteorder.h>
//was: #if defined(__LITTLE_ENDIAN)
//was: #define _BYTE_ORDER _LITTLE_ENDIAN
//was: #elif defined(__BIG_ENDIAN)
//was: #define _BYTE_ORDER _BIG_ENDIAN
//was: #else
//was: #error "Please fix asm/byteorder.h"
//was: #endif

#endif
