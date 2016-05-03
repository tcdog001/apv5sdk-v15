

#include <stdio.h>
#include <stdlib.h>


#include "smatch.h"
#include "UserPrint.h"
#include "TimeMillisecond.h"
#include "ParameterSelect.h"
#include "Card.h"
#include "Device.h"
#include "Carrier.h"
#include "MyDelay.h"

extern void ar5416CarrierTransmit(struct ath_hal *ah, unsigned char mode, unsigned char tx_chain_mask);
extern void ar9300CarrierTransmit(struct ath_hal *ah, unsigned char mode, unsigned char tx_chain_mask);


void CarrierStart(int frequency, unsigned char txchain, int timeout, int (*ison)(), int (*done)()) 
{
	unsigned int ctime, startTime, endTime;
	int it;

	DeviceTransmitCarrier(frequency,txchain);

	if(ison!=0)
	{
		(*ison)();
	}

    //
	// Loop timeout condition.
	// This number can be large since it is only used for
	// catastrophic failure of cart. The normal terminating
	// condition is a message from cart saying STOP.
	//
    startTime=TimeMillisecond();
	ctime=startTime;
	if(timeout<=0)
	{
		timeout=60000;
	}
	endTime=startTime+timeout; 
	//
	// loop looking for descriptors with transmitted packets.
	//
	for(it = 0; ; it++) 
	{
		//
		// check for message from cart telling us to stop
		// this is the normal terminating condition
		//
		if(done!=0)
		{
			if((*done)())
			{
				UserPrint("Stop message received.\n");
				if(txchain&1){
					FieldWrite("ch0_rxtx2.paon",0x1);
					FieldWrite("ch0_rxtx2.txon",0x1);
				}
				if(txchain&2){
					FieldWrite("ch0_rxtx2.paon",0x1);
					FieldWrite("ch0_rxtx2.txon",0x1);
				}
				if(txchain&4){
					FieldWrite("ch0_rxtx2.paon",0x1);
					FieldWrite("ch0_rxtx2.txon",0x1);
				}
				FieldWrite("ch0_rxtx2.synthon",0x0);
				break;
			}
		}
		//
		// check for timeout
		// rare terminating condition when cart crashes or disconnects
		//
		ctime=TimeMillisecond();
		if((endTime>startTime && (ctime>endTime || ctime<startTime)) || (endTime<startTime && ctime>endTime && ctime<startTime))
		{
			UserPrint("Timeout.\n");
				if(txchain&1){
					FieldWrite("ch0_rxtx2.paon",0x1);
					FieldWrite("ch0_rxtx2.txon",0x1);
				}
				if(txchain&2){
					FieldWrite("ch0_rxtx2.paon",0x1);
					FieldWrite("ch0_rxtx2.txon",0x1);
				}
				if(txchain&4){
					FieldWrite("ch0_rxtx2.paon",0x1);
					FieldWrite("ch0_rxtx2.txon",0x1);
				}
			FieldWrite("ch0_rxtx2.synthon",0x0);
			break;
		}
		//
		// sleep every other time, need to keep up with fast rates
		//
	    UserPrint(".");
	    MyDelay(100);
   } 
}

