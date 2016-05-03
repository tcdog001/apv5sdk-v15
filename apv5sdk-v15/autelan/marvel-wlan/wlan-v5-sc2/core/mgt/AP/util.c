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


#include "wltypes.h"
#if 1
extern UINT8 util_CopyList(UINT8 *Dest_p, UINT8 *Src_p, UINT32 MaxSize)
{
   UINT32  i = 0;

   while (i < MaxSize  &&  Src_p[i] != '\0')
   {
      Dest_p[i] = Src_p[i];
      i++;
   }

   if (i < MaxSize)
   {
      Dest_p[i] = '\0';
   }
   return (i);
} // End util_CopyList()
#endif

extern UINT8 util_ListLen(UINT8 *List_p, UINT32 MaxSize)
{
   UINT32  i = 0;

   while (i < MaxSize  &&  (*List_p) != 0)
   {
      i++;
      if (i < MaxSize)
      {
         List_p++;
      }
   }
   return i;
} // End util_ListLen()

UINT8 util_GetIndexByRate(UINT8 rate)
{
    switch (rate)
    {
    case 2:
        return 0;
    case 4:
        return 1;
    case 11:
        return 2;
    case 22:
        return 3;
    case 44:
        return 4;
    case 12:
        return 5;
    case 18:
        return 6;
    case 24:
        return 7;
    case 36:
        return 8;
    case 48:
        return 9;
    case 72:
        return 10;
    case 96:
        return 11;
    case 108:
        return 12;
    case 144:
        return 13;
    default:
        return 12;
    }
}

UINT8 util_GetRateByIndex(UINT8 index)
{
    switch (index)
    {
    case 0:
        return 2;
    case 1:
    	return 4;
    case 2:
    	return 11;
    case 3:
    	return 22;
    case 4:
    	return 44;
    case 5:
    	return 12;
    case 6:
    	return 18;
    case 7:
    	return 24;
    case 8:
    	return 36;
    case 9:
    	return 48;
    case 10:
    	return 72;
    case 11:
    	return 96;
    case 12:
    	return 108;
    case 13:
    	return 144;
    default:
    	return 108;
    }        
}


