

#include "ar5416/ar5416.h"
#include "ar5416/ar5416reg.h"
//#include "ar5416/ar5416desc.h"
#include "ar5416/ar5416phy.h"



// i dont know if this is correct for ar5416. works on Merlin
/** 
 * - Function Name: ar928xSetSingleCarrier
 * - Description  :
 * - Arguments
 *     - 
 * - Returns      :                                                                
 *******************************************************************************/
#define COMMON_SW_TABLE_REG (0x9964)

void ar9300CarrierTransmit(unsigned int devNum, int frequency, unsigned int tx_chain_mask)
{
//	A_UINT32     temp_reg;
    unsigned char mode;
	
	mode=(frequency>=4000);

    MyRegisterWrite(     0x98a4, ((0x7ff<<11) | 0x7ff));		// osprey 98a4
    MyRegisterWrite(     0xa364, ((1<<7) | (1<<1)));			// osprey a364
    MyRegisterWrite(     0xa350, 0x80008000);					// osprey a350

	MyRegisterWrite(     0x7854,	0x1202580b);				// SYNTH4
//	MyRegisterWrite(     0x7860,	0xc3c00000);				// SYNTH7

    /* set up 2GHz single carrier */
    if(mode == 0)			// 2.4ghz
	{
#ifdef UNUSED
        /* chain zero */
		if(isKiteSW(pLibDev->swDevID))
		{
            MyRegisterWrite(     0x7868, (0x1 << 29) | (0x1 << 27) | (0x1 << 17) | (0x1 << 16) |
				(0x1 << 14) | (0x1 << 12) | (0x1 << 11) |
				(0x1 << 7) | (0x1 << 5));
			
			OS_DELAY(10);	
			
			temp_reg = REGR(devNum, 0x7864) & 0xc007ffff;
			temp_reg |= getFieldForMode(devNum, "bb_forced_txldBloqgain", pLibDev->mode, pLibDev->turbo) << 27;
			temp_reg |= getFieldForMode(devNum, "bb_forced_txV2Igain", pLibDev->mode, pLibDev->turbo) << 25;
			temp_reg |= getFieldForMode(devNum, "bb_forced_padrvgn", pLibDev->mode, pLibDev->turbo) << 22;
			temp_reg |= getFieldForMode(devNum, "bb_forced_paout2gn", pLibDev->mode, pLibDev->turbo) << 19;
			
			REGW(devNum, 0x7864, temp_reg);
		}
        else if(isKiwi(pLibDev->swDevID))
        {
            if((tx_chain_mask & 0x01) == 0x01)
            {
                MyRegisterWrite(     0x78ac, (0x1 << 29) | (0x1 << 20) | 
                     (0x1 << 18) | (0x1 << 16) | 
                     (0x1 << 12) | (0x1 << 10) | 
                     (0x1 << 7)  | (0x1 << 5)  | 
                     (0x1 << 3)); 
            }
            /* chain one */
            if ((tx_chain_mask & 0x02) == 0x02 ) 
            {
                MyRegisterWrite(     0x78ac, (0x1 << 29) | (0x1 << 20) | 
                     (0x3 << 18) | (0x1 << 16) | 
                     (0x3 << 12) | (0x3 << 10) | 
                     (0x1 << 7)  | (0x1 << 5)  | 
                     (0x1 << 3)); 
            }
        }
        else
#endif
		{
			if((tx_chain_mask & 0x01) == 0x01)
			{
				MyRegisterWrite(     0x788c, (0x1 << 29) | (0x1 << 20) | 
					 (0x1 << 18) | (0x1 << 16) | 
					 (0x1 << 12) | (0x1 << 10) | 
					 (0x1 << 7)  | (0x1 << 4)  | 
					 (0x1 << 2)); 
			}
			/* chain one */
			if ((tx_chain_mask & 0x02) == 0x02 ) 
			{
				MyRegisterWrite(     0x788c, (0x1 << 29) | (0x1 << 20) | 
					 (0x3 << 18) | (0x1 << 16) | 
					 (0x3 << 12) | (0x3 << 10) | 
					 (0x1 << 7)  | (0x1 << 4)  | 
					 (0x1 << 2)); 
			}
		}
    }

    else					// 5GHz
	{

#ifdef UNUSED
	/* turning the xpa on requires controlling 
         * the swcom lines for mini-PCI cards */
        if( pLibDev->hwDevID == MERLIN_DEV_AR5416_PCI ) 
		{
            MyRegisterWrite( COMMON_SW_TABLE_REG, 0x00000dd0);
        }
#endif

        /* chain zero */
        if( (tx_chain_mask & 0x01) == 0x01 )
        {
            MyRegisterWrite(     0x788c, (0x1 << 29) | (0x0 << 20) |			// osprey is same
                 (0x0 << 18) | (0x1 << 16) | 
                 (0x1 << 12) | (0x1 << 10) |
                 (0x1 << 7)  | (0x1 << 4)  |
                 (0x1 << 2)); 
        }
        /* chain one */
        if ((tx_chain_mask & 0x02) == 0x02 ) 
        {
            MyRegisterWrite(     0x788c, (0x1 << 29) | (0x0 << 20) | 
                 (0x0 << 18) | (0x1 << 16) |
                 (0x3 << 12) | (0x3 << 10) | 
                 (0x1 << 7)  | (0x1 << 4)  | 
                 (0x1 << 2)); 
        }
    }

    /* force xpa on */
    if( (tx_chain_mask & 0x01) == 0x01 ) 
	{
        MyRegisterWrite(     0xa358, ((0x1<<1) | 1)); /* for chain zero */		// osprey a358
    }
    if( (tx_chain_mask & 0x10) == 0x10 ) 
	{
        MyRegisterWrite(     0xa358, ((0x3<<1) | 1)); /* for chain one */
    }


}

