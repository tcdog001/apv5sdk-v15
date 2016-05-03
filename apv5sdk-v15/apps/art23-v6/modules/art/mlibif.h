/*
 *  Copyright © 2001 Atheros Communications, Inc.,  All Rights Reserved.
 */
/* MLIBif.h - Exported functions and defines for the manufacturing lib */

#ifndef	__INCmlibifh
#define	__INCmlibifh
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//  "ACI $Id: //depot/sw/branches/narthal_mar2011/dk/mdk/art/mlibif.h#1 $, $Header: //depot/sw/branches/narthal_mar2011/dk/mdk/art/mlibif.h#1 $"

#include "dk_structures.h"

// Environment/device setup funcitons
A_BOOL   initializeEnvironment(A_BOOL remote);
void     closeEnvironment(void);

#ifndef __ATH_DJGPPDOS__
A_INT32  setupDevice(A_UINT32 whichDevice, DK_DEV_INFO *pdkInfo, A_UINT16 remoteLib);
#endif
void     teardownDevice(A_UINT32 devNum);

// Some print functions
void     txPrintStats(A_UINT32 devNum, A_UINT32 rateInMb, A_UINT32 remote);
void     rxPrintStats(A_UINT32 devNum, A_UINT32 rateInMb, A_UINT32 remote);

// Basic device I/O functions
A_UINT32 OSregRead(A_UINT32 devNum, A_UINT32 regOffset);
void     OSregWrite(A_UINT32 devNum, A_UINT32 regOffset, A_UINT32 regValue);
A_UINT32 OScfgRead(A_UINT32 devNum, A_UINT32 regOffset);
void     OScfgWrite(A_UINT32 devNum, A_UINT32 regOffset, A_UINT32 regValue);
void     OSmemRead(A_UINT32 devNum, A_UINT32 physAddr, A_UCHAR  *bytesRead, A_UINT32 length);
void     OSmemWrite(A_UINT32 devNum, A_UINT32 physAddr, A_UCHAR  *bytesWrite, A_UINT32 length);

// Dev to driverDev mapping table.  devNum must be in range 0 to LIB_MAX_DEV
extern  A_UINT32   devNum2driverTable[];
#define dev2drv(x) (devNum2driverTable[(x)])

// Macros to devMap defined functions
#define REGR(x, y) (OSregRead((x), (y) + (globDrvInfo.pDevInfoArray[dev2drv(x)]->pdkInfo->f2MapAddress)))
#define REGW(x, y, z) (OSregWrite((x), (y) + (globDrvInfo.pDevInfoArray[dev2drv(x)]->pdkInfo->f2MapAddress), (z)))

void changePciWritesFlag
(
	A_UINT32 devNum,
	A_UINT32 flag
);

#ifdef __cplusplus
}
#endif

#endif // #define __INCmlibifh
