/*
*                Copyright 2002-2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

#ifndef WL_H
#define WL_H

typedef	s8	CHAR;
typedef	s16	SHORT;
typedef	s32	LONG;
typedef u8	UCHAR;
typedef u8 BYTE;
typedef u16	USHORT;
typedef	u32	ULONG;
typedef	u32 DWORD;
//typedef long long	LARGE_INTEGER;
typedef	u8	OS_BOOL, *POS_BOOL;

typedef	s8	OS_INT8;
typedef s16	OS_INT16;
typedef s32	OS_INT32;

typedef	u8	OS_UINT8;
typedef	u16	OS_UINT16;
typedef	u32	OS_UINT32;

typedef u64	OS_UINT64, *POS_UINT64;	

#ifndef	FALSE
#define	FALSE		0
#endif

#ifndef	TRUE
#define	TRUE		1
#endif

#endif /* WL_H */

