#include <stdio.h>
#include <stdlib.h>

#include "link.h"
#include "ar9300papd.h"
#include "UserPrint.h"

unsigned int gain_table_entries[32], gain_vs_table_index[32];

int papredistortionSingleTable(struct ath_hal *ah, HAL_CHANNEL *chan, int txChainMask)
{
	int chainNum;
	unsigned int PA_table[24], smallSignalGain; 
    int status = 0, disable_papd=0, chain_fail[3]={0,0,0};

	UserPrint("Run PA predistortion algorithm\n");

	LinkTransmitPAPDWarmUp(txChainMask);
	status=ar9300PAPDInitTable(ah, chan);
	if(status==-1)
	{
		ar9300EnablePAPD(ah, AH_FALSE, chan);
		UserPrint("Warning:: PA predistortion failed in InitTable\n");
		return -1;
	}
    {
        struct ath_hal_9300 *ahp = AH9300(ah);
        UserPrint("Training power_x2 is %d, channel %d\n", ahp->paprd_training_power, chan->channel);
    }

	for(chainNum=0; chainNum<3; chainNum++)
	{
		unsigned int i, desired_gain, gain_index;
		if(txChainMask&(1<<chainNum)) 
		{
			ar9300PaprdSetupGainTable(ah,chainNum);
			LinkTransmitPAPD(chainNum);
			ar9300PAPRDisDone(ah);
			status = ar9300PAPRDCreateCurve(ah, chan, chainNum);
			if(status==0) 
            {
                ar9300PopulatePaprdSingleTable(ah, chan, chainNum);
            }
            else
            {
                disable_papd = 1;
                chain_fail[chainNum] = 1;
            }
		}
	}
	if(disable_papd==0)
    {
		ar9300EnablePAPD(ah, AH_TRUE, chan);
    }
	else
    {
		ar9300EnablePAPD(ah, AH_FALSE, chan);
        UserPrint("Warning:: PA predistortion failed. chain_fail_flag %d %d %d\n", chain_fail[0], chain_fail[1], chain_fail[2]);
        return -1;
    }

	
	return 0;
}


