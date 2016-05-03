 /*
*                Copyright 2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/
#ifndef AP8X_DMA_H_
#define AP8X_DMA_H_


#ifdef DMA_ENABLE
	#ifdef WL_DEBUG
		extern int mvDmaCopy(const char *func, int line,  void *dst, void *src, int byteCount);
		#define MEMCPY mvDmaCopy(__FUNCTION__,__LINE__,
	#else
		extern int mvDmaCopy(void *dst, void *src, int byteCount);
		#define MEMCPY mvDmaCopy(
	#endif
#else
	#define MEMCPY memcpy(
#endif

#endif /*AP8X_DMA_H_*/

