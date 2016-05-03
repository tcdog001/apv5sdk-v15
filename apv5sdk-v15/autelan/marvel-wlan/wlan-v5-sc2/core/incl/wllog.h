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

#ifndef WL_LOG_H
#define WL_LOG_H
#include "wltypes.h"

#define MARVEL_DEBUG_DEFAULT            0
#define MARVEL_DEBUG_PANIC              1
#define MARVEL_DEBUG_ERROR              2
#define MARVEL_DEBUG_WARNING            3
#define MARVEL_DEBUG_DEBUG              4
#define MARVEL_DEBUG_ALL                5
#define MARVEL_DEBUG_RECV_PACKET        6
#define MARVEL_DEBUG_SEND_PACKET        7

void WlLogPrint(UINT32 loglevel,const char *func, const char *format, ... );
void CWCaptrue(int n ,unsigned char *buffer);

#endif /* WL_LOG_H */

