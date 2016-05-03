/*====================================================================*
 *
 *   flags.h - bitmap flagword definitions and declarations;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef FLAGS_HEADER
#define FLAGS_HEADER

/*====================================================================*
 *   define macros for common, but error prone, bitmap operations; 
 *--------------------------------------------------------------------*/

#define _getbits(map,pos,cnt) (((map)>>((pos)-(cnt)+1))&~(~(0)<<(cnt)))

#define _bitmask(bits) ~(~(0) << bits)

#define _setbits(flag,mask) flag |=  (mask)
#define _clrbits(flag,mask) flag &= ~(mask)
#define _toggle(flag,mask)  flag = ~(flag) & ~(mask)

#define _anyset(flag,mask) ((flag) & (mask)) != (0)
#define _anyclr(flag,mask) ((flag) & (mask)) != (mask)
#define _allset(flag,mask) ((flag) & (mask)) == (mask)
#define _allclr(flag,mask) ((flag) & (mask)) == (0)

#define _notset(flag,mask) ((flag) & (mask)) == (0)

#define _anybits(flag,mask) ((flag) & (mask)) != (0)
#define _allbits(flag,mask) ((flag) & (mask)) == (mask)

#define _clean(flag,mask) ((flag) & ~(mask)) == (0)
#define _dirty(flag,mask) ((flag) & ~(mask)) != (0)

/*====================================================================*
 *   end definitions and declarations;
 *--------------------------------------------------------------------*/

#endif

