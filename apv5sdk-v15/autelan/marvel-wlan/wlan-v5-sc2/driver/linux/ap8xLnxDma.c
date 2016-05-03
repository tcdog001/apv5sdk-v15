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
#include "mvTypes.h"
#include "mvIdma.h"
#include "wltypes.h"
#include "wl_mib.h"
#include "wl_hal.h"
#include "wldebug.h"
#ifdef DMA_ENABLE

#ifdef WL_DEBUG
int mvDmaCopy(const char *func, int line,  struct net_device *dev, void *dst, void *src, MV_U32 byteCount)
#else
int mvDmaCopy(struct net_device *dev, void *dst, void *src, MV_U32 byteCount)
#endif
{
	MV_U32 chan=0, ctrlLo;
	MV_U32 pSrc, pDst;
	MV_DMA_DEC_WIN win;
	if( (byteCount > 0xffffff) || (byteCount == 0) )
		return 0;
	ctrlLo = ICCLR_BLOCK_MODE | ICCLR_NON_CHAIN_MODE | ICCLR_SRC_INC | ICCLR_DST_INC;
	if (byteCount > _64K)
	{
		ctrlLo |= ICCLR_DESC_MODE_16M;
	}
	/* set data transfer limit */
	ctrlLo |= (ICCLR_DST_BURST_LIM_32BYTE | ICCLR_SRC_BURST_LIM_32BYTE);
	/* wait for previous transfer completion */
	while( mvDmaStateGet(chan) != MV_IDLE );

	/* issue the transfer */

	mvDmaCtrlLowSet( chan, ctrlLo );
	pSrc = pci_map_single(dev, src, byteCount, PCI_DMA_TODEVICE);
	pDst = virt_to_phys(dst);
	mvDmaTransfer( chan, ((MV_U32)pSrc) , ((MV_U32)pDst) , byteCount, 0 );

	/* wait for completion */
	while( mvDmaStateGet(chan) != MV_IDLE );
	pci_unmap_single(dev, (pDst), byteCount, PCI_DMA_FROMDEVICE);

	return 1;

}
#endif

