/*
*                Copyright 2003, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

/*!
* \file    hal_mib.c
* \brief   Routines to initialize MIB values by reading hardware registers
*/


#include "wltypes.h"
#include "wl_macros.h"
#include "IEEE_types.h"
#include "wl_mib.h"
#include "wl_hal.h"
#include "mib.h"
#include "qos.h"
#include "wlmac.h"

UINT8 dataRates[IEEEtypes_MAX_DATA_RATES_G] = 
{ 2, 4, 11, 22, 12, 18, 24, 36, 48, 72, 96, 108, 44, 144 };

static void hal_InitPhyMIB(MIB_802DOT11 * mib)
{
}

BOOLEAN hal_InitApMIB(MIB_802DOT11 * mib)
{
	hal_InitPhyMIB(mib);
	return (TRUE);
}

BOOLEAN hal_InitStaMIB(MIB_802DOT11 * mib)
{
	MIB_OP_DATA *mibOpData;

	mibOpData = mib->OperationTable;

	hal_InitPhyMIB(mib);
	return (TRUE);
}


