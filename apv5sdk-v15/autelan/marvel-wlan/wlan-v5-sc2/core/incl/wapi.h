/*
*                Copyright 2012, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/
#ifndef _WAPI_H_
#define _WAPI_H_
#define WAPI_PN_DW_LEN  4   //PN DWord length

#define INSERT_WAPIHDR(pWapiHdr, pPn)   \
    *pWapiHdr = *pPn;                   \
    *(pWapiHdr+1) = *(pPn + 1);         \
    *(pWapiHdr+2) = *(pPn + 2);         \
    *(pWapiHdr+3) = *(pPn + 3)

#define INCREASE_WAPI_PN(PN, inc)       \
{                                       \
    int        i;                       \
    UINT32     inc1=inc;                \
    UINT32     temp;                    \
    for(i = 0; i < WAPI_PN_DW_LEN; i++) \
    {                                   \
        temp = *(PN+i);                 \
        *(PN+i) += inc;                 \
        if (*(PN+i) < temp)             \
        {                               \
            inc1 = 1;                   \
        }                               \
        else                            \
        {                               \
            break;                      \
        }                               \
    }                                   \
}
#endif  //_WAPI_H_