/*
 *  Copyright © 2005 Atheros Communications, Inc.,  All Rights Reserved.
 *
 */

#ifdef VXWORKS
#include "vxworks.h"
#endif

#ifdef __ATH_DJGPPDOS__
#define __int64 long long
typedef unsigned long DWORD;
#define Sleep   delay
#endif  // #ifdef __ATH_DJGPPDOS__

#include <errno.h>
#include <assert.h>

#include "mIds.h"
#include <stdio.h>

#include "mData.h"
#include "rate_constants.h"

#include "MyDelay.h"
#include "ParameterSelect.h"
#include "Card.h"
#include "UserPrint.h"

#include "Device.h"


#include "mCal5416.h"

#include "ar5416reg.h"

//extern void AGCDeafAr5211(unsigned long devNum);
//extern void AGCUnDeafAr5211(unsigned long devNum);
//extern void disable5211QueueClocks(unsigned long devNum);

#define MAX_TX_QUEUE 10


#define F2_Q0_STS         0x0a00 // MAC Miscellaneous QCU status
#define F2_Q_STS_PEND_FR_CNT_M		0x00000003 // Mask for Pending Frame Count

#define MDCU 10			// should we only set the first 8??
#define MQCU 10

#undef REGR

static unsigned int REGR(unsigned long devNum, unsigned long address)
{
	unsigned int value;

	devNum=0;

	MyRegisterRead(address,&value);

	return value;
}

#define MAC_PCU_STA_ADDR_L32 0x00008000
#define MAC_PCU_STA_ADDR_U16 0x00008004
#define MAC_PCU_BSSID_L32 0x00008008
#define MAC_PCU_BSSID_U16 0x0000800c


static int Ar5416BssIdSet(unsigned char *bssid)
{
	unsigned int reg;

	reg=bssid[3]<<24|bssid[2]<<16|bssid[1]<<8|bssid[0];
    MyRegisterWrite(MAC_PCU_BSSID_L32,reg);

	MyRegisterRead(MAC_PCU_BSSID_U16,&reg);
	reg &= ~(0xffff);
	reg |= (bssid[5]<<8|bssid[4]);
    MyRegisterWrite(MAC_PCU_BSSID_U16,reg);

	return 0;
}


static int Ar5416StationIdSet(unsigned char *mac)
{
	unsigned int reg;

	reg=mac[3]<<24|mac[2]<<16|mac[1]<<8|mac[0];
    MyRegisterWrite(MAC_PCU_STA_ADDR_L32,reg);

	MyRegisterRead(MAC_PCU_STA_ADDR_U16,&reg);
	reg &= ~(0xffff);
	reg |= (mac[5]<<8|mac[4]);
    MyRegisterWrite(MAC_PCU_STA_ADDR_U16,reg);

	return 0;
}


void macAPIInitAr5416
(
 unsigned long devNum
)
{
#ifdef UNUSED
    LIB_DEV_INFO *pLibDev = gLibInfo.pLibDevArray[devNum];
    if (IS_MAC_5416_2_0_UP(pLibDev->macRev)) {
        pLibDev->txDescStatus1 = FIRST_5416_2_TX_STATUS_WORD;
        pLibDev->txDescStatus2 = SECOND_5416_2_TX_STATUS_WORD;
    } else {
        pLibDev->txDescStatus1 = FIRST_5416_TX_STATUS_WORD;
        pLibDev->txDescStatus2 = SECOND_5416_TX_STATUS_WORD;
    }
    pLibDev->decryptErrMsk = VENICE_DESC_DECRYPT_ERROR;
    pLibDev->bitsToRxSigStrength = OWL_BITS_TO_RX_SIG_STRENGTH;
    pLibDev->rxDataRateMsk = 0xff;
#endif
    return;
}

unsigned long setupAntennaAr5416
(
 unsigned long devNum,
 unsigned long antenna,
 unsigned long* antModePtr   // retVal used by setDescr, createDescr etc...
)
{
#ifdef UNUSED
	LIB_DEV_INFO *pLibDev = gLibInfo.pLibDevArray[devNum];
if(isKiteSW(pLibDev->swDevID))
{
	if(antenna == (USE_DESC_ANT|DESC_ANT_A))
		MyRegisterWrite(0x8058, REGR(devNum, 0x8058) & ~(0x1<<24));
	else
		MyRegisterWrite(0x8058, REGR(devNum, 0x8058) | (1<<24));
}
    else 
#endif
	{
    /* Nothing to be done for now... use .cfg switch table */
    devNum = 0; // quiet compiler
    antenna = 0; // quiet compiler
    antModePtr = 0; // quiet compiler
}
    return 1;
}


//
// disable receive
//
static int Ar5416ReceiveDisable()
{
	unsigned long reg;

    reg=REGR(0, F2_CR);
	reg |= F2_CR_RXD;			// set disable rx bit
	reg &= (~F2_CR_RXE);		// clear enable rx bit
    MyRegisterWrite(F2_CR, reg);

	reg=REGR(0, F2_DIAG_SW);
	reg |= F2_DIAG_RX_DIS;
    MyRegisterWrite(F2_DIAG_SW, reg);
    //
    // clear unicast and broadcast and promiscuous
	//
	reg=REGR(0, F2_RX_FILTER);
	reg &= (~(F2_RX_UCAST|F2_RX_BCAST|F2_RX_PROM));
    MyRegisterWrite(F2_RX_FILTER, reg);

	return 0;
}


#define AR_DIAG_RX_ABORT            0x02000000 //  abort rx
//
// enable receive
//
static int Ar5416ReceiveEnable()
{
	unsigned long reg;

	reg=REGR(0, F2_DIAG_SW);
	reg &= (~F2_DIAG_RX_DIS);
    MyRegisterWrite(F2_DIAG_SW, reg);

	reg=REGR(0, F2_DIAG_SW);
	reg &= (~AR_DIAG_RX_ABORT);
    MyRegisterWrite(F2_DIAG_SW, reg);

    reg=REGR(0, F2_CR);
	reg &= (~F2_CR_RXD);		// clear disable rx bit
	reg |= F2_CR_RXE;			// set enable rx bit
    MyRegisterWrite(F2_CR, reg);

	return 0;
}


//
// set pointer to rx descriptor in shared memory
//
static int Ar5416ReceiveDescriptorPointer(unsigned int descriptor)
{
    MyRegisterWrite(F2_RXDP, descriptor);

	return 0;
}


//
// set or clear receive filter bit
//
static int Ar5416ReceiveFilter(int on, unsigned int mask)
{
	unsigned long reg;

		reg=REGR(0, F2_RX_FILTER);
	if(on)
	{
		reg |= mask;
	}
	else
	{
		reg &= (~mask);
 	}
        MyRegisterWrite(F2_RX_FILTER, reg);

		return 0;
}


//
// set or clear receive of unicast packets
//
static int Ar5416ReceiveUnicast(int on)
{
	return Ar5416ReceiveFilter(on,F2_RX_UCAST);
}


//
// set or clear receive of broadcast packets
//
static int Ar5416ReceiveBroadcast(int on)
{
	return Ar5416ReceiveFilter(on,F2_RX_BCAST);
}


//
// set or clear promiscuous mode
//
static int Ar5416ReceivePromiscuous(int on)
{
	return Ar5416ReceiveFilter(on,F2_RX_PROM);
}


//
// set pointer to tx descriptor in shared memory
//
static int Ar5416TransmitDescriptorPointer(int queue, unsigned int descriptor)
{
    MyRegisterWrite( F2_Q0_TXDP + (4 * queue), descriptor);

	return 0;
}


#define DISABLE_TIMEOUT 100

//
// disable transmit
//
static int Ar5416TransmitDisable(unsigned int qmask)
{
	unsigned long reg;
	int it,jt;
	int pendingCount;

	MyRegisterWrite(F2_Q_TXD, qmask);
	//
	// wait for Q_TXE to clear
	//
    for(it = 0; it < DISABLE_TIMEOUT; it++) 
	{
		reg=REGR(0, F2_Q_TXE);
		if((reg&qmask)==0)
		{
			break;
		}
		MyDelay(1);
	}
    //
	// wait for packet queue to become empty
	//
	for(jt = 0; jt < MAX_TX_QUEUE; jt++) 
	{
		if((qmask&(1<<jt))!=0)
		{
		    //wait for the pending frames to go to zero
		    for(it = 0; it < DISABLE_TIMEOUT; it++) 
			{
			    pendingCount = REGR(0, F2_Q0_STS + (4 * jt)) &  F2_Q_STS_PEND_FR_CNT_M;
			    if(pendingCount==0) 
				{
				    break;
				}
			    MyDelay(1);
			}
		    if(it >= DISABLE_TIMEOUT) 
			{
			}
		}
	}
	//
	// reset various registers?
	//
	//
	// MORE?
	//
	return 0;
}


//
// enable transmit
//
static int Ar5416TransmitEnable(unsigned int qmask)
{
	unsigned long reg;

    //improve performance
    MyRegisterWrite(0x1230, 0x10);

	// set preserve seqNum bit if aggregation is requested
//	if(pLibDev->yesAgg == 1) 
	{
		MyRegisterWrite(0x8004, 0x20000000 | REGR(0, 0x8004));
	}

	MyRegisterWrite(F2_Q_TXD, ~qmask);		// or maybe 0???
	reg=REGR(0, F2_Q_TXE);
	reg|=qmask;
    MyRegisterWrite(F2_Q_TXE, reg);

	return 0;
}


static void enable5416QueueClocks( unsigned long qcu, unsigned long dcu)
{
	unsigned long reg;

    reg=REGR(0, F2_QDCKLGATE);
	reg &= (~((0x01 << qcu) | 0x10000 << dcu));
    MyRegisterWrite(F2_QDCKLGATE, reg);
}



//
// Set contention window.
// cwmin and cwmax are limited to values that are powers of two minus 1: 0, 1, 3, 7, ....
//
static int Ar5416TransmitContentionWindow(int dcu, int cwmin, int cwmax)
{
    unsigned long reg;
	static int allowed[]={0x0,0x1,0x3,0x7,0xf,0x1f,0x3f,0x7f,0xff,0x1ff,0x3ff};
	int nallowed;
	int it;
	//
	// default values
	//
	nallowed=sizeof(allowed)/sizeof(int);

	if(cwmin<0)
	{
		cwmin=0xf;
	}
	else if(cwmin>=allowed[nallowed-1])
	{
		cwmin=allowed[nallowed-1];
	}
	else 
	{
		for(it=0; it<sizeof(allowed)/sizeof(int); it++)
		{
			if(cwmin>allowed[it])
			{
				cwmin=allowed[it];
				break;
			}
		}
	}

	if(cwmax<0)
	{
		cwmax=0x3ff;
	}
	else if(cwmax>=allowed[nallowed-1])
	{
		cwmax=allowed[nallowed-1];
	}
	else 
	{
		for(it=0; it<sizeof(allowed)/sizeof(int); it++)
		{
			if(cwmax>allowed[it])
			{
				cwmax=allowed[it];
				break;
			}
		}
	}

	if(cwmin>cwmax)
	{
		cwmin=cwmax;
	}

        reg = REGR(0, F2_D0_LCL_IFS + (4 * dcu ));
	    //
	    // set cwmin = cwmax = 0
	    //
        reg &= ~(F2_D_LCL_IFS_CWMIN_M | F2_D_LCL_IFS_CWMAX_M);
		//
		// set cwmax
		//
		reg |= ((cwmax << F2_D_LCL_IFS_CWMAX_S) & F2_D_LCL_IFS_CWMAX_M);
		//
		// set cwmin
		//
		reg |= ((cwmin << F2_D_LCL_IFS_CWMIN_S) & F2_D_LCL_IFS_CWMIN_M);

        MyRegisterWrite(F2_D0_LCL_IFS +  ( 4 * dcu ), reg);

	return 0;
}


static int Ar5416TransmitInterframeSpacing(int dcu, int spacing)
{
    unsigned long reg;
	//
	// default values
	//
	if(spacing<0)
	{
		spacing=2;
    }

        reg = REGR(0, F2_D0_LCL_IFS + (4 * dcu ));
	    //
	    // clear current ifs value
	    //
		reg &= ( ~F2_D_LCL_IFS_AIFS_M);
		//
		// set new ifs value
		//
		reg |= ((spacing << F2_D_LCL_IFS_AIFS_S) & F2_D_LCL_IFS_AIFS_M);

        MyRegisterWrite(F2_D0_LCL_IFS +  ( 4 * dcu ), reg);

	return 0;
}


static int Ar5416TransmitEarlyTermination(int qcu, int on)
{
    unsigned long reg;

    reg = REGR(0, F2_Q0_MISC + (4 * qcu));
	if(on)
	{
        reg |= 0x800;
	}
	else
	{
        reg &= (~0x800);
	}
    MyRegisterWrite(F2_Q0_MISC + (4 * qcu ), reg);

	return 0;
}

 
static int Ar5416TransmitOtherFastStuff(int ht40, int on)
{
	unsigned long reg;

    //Increase the interframe spacing to improve the TX EVM
	if(on)
	{
        reg=REGR(0, F2_D_GBL_IFS_SIFS);
        if(ht40) 
		{
            reg=0x400;	//sifs, 0x64
		}
        else 
		{
            reg=0x200;	//sifs, 0x64
		}
		MyRegisterWrite(F2_D_GBL_IFS_SIFS, reg); 

        reg=REGR(0, F2_D_GBL_IFS_EIFS);
        reg=100;
        MyRegisterWrite(F2_D_GBL_IFS_EIFS, reg); //eifs

        reg=REGR(0, F2_D_FPCTL);
        reg=0x10;
        MyRegisterWrite(F2_D_FPCTL, reg); //eifs

        reg=REGR(0, F2_TIME_OUT);
        reg=0x2;
        MyRegisterWrite(F2_TIME_OUT, reg); //eifs
	}

	return 0;
}


static int Ar5416ContinuousDataMode(int on)
{
	unsigned long reg;

	if(on)
	{
        // Put PCU and DMA in continuous data mode
        reg=REGR(0, 0x8054);
	    reg |= 1;
		MyRegisterWrite(0x8054, reg);

        //disable encryption since packet has no header
		reg=REGR(0, F2_DIAG_SW);
		reg |= F2_DIAG_ENCRYPT_DIS;
        MyRegisterWrite(F2_DIAG_SW,  reg);
	}
	else
	{
        // Put PCU and DMA in continuous data mode
        reg=REGR(0, 0x8054);
	    reg &= (~1);
		MyRegisterWrite(0x8054, reg);
	}
	return 0;
}


//
// map the qcu to the dcu and enable the clocks for both
//
static int Ar5416TransmitQueueSetup(int qcu, int dcu)
{
	unsigned long reg;
    // 
	// program the queue
	//
    reg=REGR(0, F2_D0_QCUMASK + (4 * dcu ));
	reg|=(F2_QCU_0<<qcu);
    MyRegisterWrite(F2_D0_QCUMASK +  ( 4 * dcu ), reg);
    //
	// enable the clocks
	//
    enable5416QueueClocks(qcu, dcu);

	return 0;
}

//
// set transmit retry limit
//
static int Ar5416TransmitRetryLimit(int dcu, int retry)
{
	unsigned long reg;

	if(retry<=0)
	{
		retry=1;
	}
	if(retry>15)
	{
		retry=15;
	}
	reg = REGR(0, F2_D0_RETRY_LIMIT + ( 4 * dcu ) );
	reg=((reg & 0xfffffff0) | retry);
//	UserPrint(" %x\n",reg);
	MyRegisterWrite(F2_D0_RETRY_LIMIT + ( 4 * dcu ), reg);

	return 0;
}

static int Ar5416TransmitRegularData()			// normal
{
	int qcu, dcu;

	for(dcu=0; dcu<MDCU; dcu++)
	{
        Ar5416TransmitInterframeSpacing(dcu, -1);
        Ar5416TransmitContentionWindow(dcu, -1, -1);
	}
	Ar5416TransmitOtherFastStuff(1,0);
	for(qcu=0; qcu<MQCU; qcu++)
	{
	    Ar5416TransmitEarlyTermination(qcu,0);
	}
	Ar5416ContinuousDataMode(0);

	return 0;
}


static int Ar5416TransmitFrameData(int ifs)	// tx99
{
	int qcu, dcu;

	for(dcu=0; dcu<MDCU; dcu++)
	{
        Ar5416TransmitInterframeSpacing(dcu, ifs);
        Ar5416TransmitContentionWindow(dcu, 0, 0);
        Ar5416TransmitRetryLimit(dcu, 15);
	}
	Ar5416TransmitOtherFastStuff(1,0);
	for(qcu=0; qcu<MQCU; qcu++)
	{
	    Ar5416TransmitEarlyTermination(qcu,1);
	}
	Ar5416ContinuousDataMode(0);

	return 0;
}


static int Ar5416TransmitContinuousData()		// tx100
{
	int qcu, dcu;

	for(dcu=0; dcu<MDCU; dcu++)
	{
        Ar5416TransmitInterframeSpacing(dcu, -1);
        Ar5416TransmitContentionWindow(dcu, -1, -1);
	}
	Ar5416TransmitOtherFastStuff(1,0);
	for(qcu=0; qcu<MQCU; qcu++)
	{
	    Ar5416TransmitEarlyTermination(qcu,0);
	}
	Ar5416ContinuousDataMode(1);

	return 0;
}




/**************************************************************************
* rxCleanupConfig - program register ready for rx begin
*
*/
void rxCleanupConfigAr5416
(
 unsigned long devNum
)
{
    //write RX disable bit
    MyRegisterWrite(F2_CR, REGR(devNum, F2_CR) | F2_CR_RXD);

    //set the receive disable register
    MyRegisterWrite(F2_DIAG_SW, REGR(devNum, F2_DIAG_SW) | F2_DIAG_RX_DIS);

    //clear rx DESC_INT
    MyRegisterWrite(F2_IMR, (REGR(devNum, F2_IMR) & ~F2_IMR_RXDESC));

    //disable IER
    MyRegisterWrite(F2_IER, F2_IER_DISABLE);

    //clear unicast and broadcast reception
    MyRegisterWrite(F2_RX_FILTER, REGR(devNum, F2_RX_FILTER) & ~(F2_RX_UCAST | F2_RX_BCAST));

    return;
}



// i dont know if this is correct for ar5416. works on Merlin
/** 
 * - Function Name: ar928xSetSingleCarrier
 * - Description  :
 * - Arguments
 *     - 
 * - Returns      :                                                                
 *******************************************************************************/
#define COMMON_SW_TABLE_REG (0x9964)

static int Ar5416TransmitCarrier(int frequency, unsigned int tx_chain_mask)
{
//	A_UINT32     temp_reg;

    int mode;

	mode=(frequency>=4000);


    MyRegisterWrite(    0x983c, ((0x7ff<<11) | 0x7ff));

    MyRegisterWrite(    0x9808, ((1<<7) | (1<<1)));
    MyRegisterWrite(    0x982c, 0x80008000);

    /* set up 2GHz single carrier */
    if(mode == 0)			// 2.4ghz
	{
#ifdef UNUSED
        /* chain zero */
		if(isKiteSW(pLibDev->swDevID))
		{
            MyRegisterWrite(    0x7868, (0x1 << 29) | (0x1 << 27) | (0x1 << 17) | (0x1 << 16) |
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
                MyRegisterWrite(    0x78ac, (0x1 << 29) | (0x1 << 20) | 
                     (0x1 << 18) | (0x1 << 16) | 
                     (0x1 << 12) | (0x1 << 10) | 
                     (0x1 << 7)  | (0x1 << 5)  | 
                     (0x1 << 3)); 
            }
            /* chain one */
            if ((tx_chain_mask & 0x02) == 0x02 ) 
            {
                MyRegisterWrite(    0x78ac, (0x1 << 29) | (0x1 << 20) | 
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
				MyRegisterWrite(    0x788c, (0x1 << 29) | (0x1 << 20) | 
					 (0x1 << 18) | (0x1 << 16) | 
					 (0x1 << 12) | (0x1 << 10) | 
					 (0x1 << 7)  | (0x1 << 4)  | 
					 (0x1 << 2)); 
			}
			/* chain one */
			if ((tx_chain_mask & 0x02) == 0x02 ) 
			{
				MyRegisterWrite(    0x788c, (0x1 << 29) | (0x1 << 20) | 
					 (0x3 << 18) | (0x1 << 16) | 
					 (0x3 << 12) | (0x3 << 10) | 
					 (0x1 << 7)  | (0x1 << 4)  | 
					 (0x1 << 2)); 
			}
		}
    }

    else					// 5GHz
	{

        /* turning the xpa on requires controlling 
         * the swcom lines for mini-PCI cards */
//        if( pLibDev->hwDevID == MERLIN_DEV_AR5416_PCI ) 
		{
            MyRegisterWrite(COMMON_SW_TABLE_REG, 0x00000dd0);
        }

        /* chain zero */
        if( (tx_chain_mask & 0x01) == 0x01 )
        {
            MyRegisterWrite(    0x788c, (0x1 << 29) | (0x0 << 20) |
                 (0x0 << 18) | (0x1 << 16) | 
                 (0x1 << 12) | (0x1 << 10) |
                 (0x1 << 7)  | (0x1 << 4)  |
                 (0x1 << 2)); 
        }
        /* chain one */
        if ((tx_chain_mask & 0x02) == 0x02 ) 
        {
            MyRegisterWrite(    0x788c, (0x1 << 29) | (0x0 << 20) | 
                 (0x0 << 18) | (0x1 << 16) |
                 (0x3 << 12) | (0x3 << 10) | 
                 (0x1 << 7)  | (0x1 << 4)  | 
                 (0x1 << 2)); 
        }
    }

    /* force xpa on */
    if( (tx_chain_mask & 0x01) == 0x01 ) 
	{
        MyRegisterWrite(    0xa3d8, ((0x1<<1) | 1)); /* for chain zero */
    }
    if( (tx_chain_mask & 0x10) == 0x10 ) 
	{
        MyRegisterWrite(    0xa3d8, ((0x3<<1) | 1)); /* for chain one */
    }

    return 0;
}


//
// clear all device control function pointers and set to default behavior
//
void Ar5416DeviceSelect()
{
	DeviceFunctionReset();
    //
	// read/write memory
	//
	DeviceMemoryReadFunction(MyMemoryRead);
	DeviceMemoryWriteFunction(MyMemoryWrite);
	//
	// read/write registers
	//
	DeviceRegisterReadFunction(MyRegisterRead);
	DeviceRegisterWriteFunction(MyRegisterWrite);

    DeviceBssIdSetFunction(Ar5416BssIdSet);
    DeviceStationIdSetFunction(Ar5416StationIdSet);

    DeviceReceiveDescriptorPointerFunction(Ar5416ReceiveDescriptorPointer);
    DeviceReceiveUnicastFunction(Ar5416ReceiveUnicast);
    DeviceReceiveBroadcastFunction(Ar5416ReceiveBroadcast);
    DeviceReceivePromiscuousFunction(Ar5416ReceivePromiscuous);
    DeviceReceiveEnableFunction(Ar5416ReceiveEnable);
    DeviceReceiveDisableFunction(Ar5416ReceiveDisable);
//    DeviceReciveDeafModeFunction(Ar5416Deaf);

    DeviceTransmitDescriptorPointerFunction(Ar5416TransmitDescriptorPointer);
    DeviceTransmitRetryLimitFunction(Ar5416TransmitRetryLimit);
    DeviceTransmitQueueSetupFunction(Ar5416TransmitQueueSetup);
    DeviceTransmitRegularDataFunction(Ar5416TransmitRegularData);
    DeviceTransmitFrameDataFunction(Ar5416TransmitFrameData);
    DeviceTransmitContinuousDataFunction(Ar5416TransmitContinuousData);
    DeviceTransmitCarrierFunction(Ar5416TransmitCarrier);	
    DeviceTransmitEnableFunction(Ar5416TransmitEnable);
    DeviceTransmitDisableFunction(Ar5416TransmitDisable);

	DeviceTransmitGainReadFunction(Ar5416TransmitGainRead);
	DeviceTransmitGainWriteFunction(Ar5416TransmitGainWrite);
	DeviceTransmitGainSetFunction(Ar5416TransmitGainSet);
	DeviceTransmitPowerSetFunction(Ar5416TransmitPowerSet);

//    DeviceEepromRead=0;
//    DeviceEepromReadAll=0;
//    DeviceEepromWrite=0;
//    DeviceEepromWriteAll=0;

    DeviceMemoryReadFunction(MyMemoryRead);
    DeviceMemoryWriteFunction(MyMemoryWrite);

    DeviceRegisterReadFunction(MyRegisterRead);
    DeviceRegisterWriteFunction(MyRegisterWrite);
}


