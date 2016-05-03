#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "wlantype.h"
#include "smatch.h"
#include "UserPrint.h"
#include "TimeMillisecond.h"
#include "CommandParse.h"
#include "NewArt.h"
#include "MyDelay.h"
#include "ParameterSelect.h"
#include "Card.h"
//#include "Field.h"


#include "Device.h"

#include "ParameterParse.h"
#include "ParameterParseNart.h"
#include "Link.h"
//#include "Calibrate.h"
#include "configCmd.h"
#include "ConfigurationStatus.h"
#include "rate_constants.h"

#define MBUFFER 1024
#define MRATE 3
#define MLOOP 200

void ConfigCmdHelpPrint()
{

}

void ConfigCmdStatus(int status, int *error, char *cmd, char *name, char *tValue, int client)
{
	char buffer[MBUFFER];
    if(status==VALUE_OK)
    {
		if (strlen(cmd)>0 && strlen(name)>0) 
			// ex: "|get|ssid|1234|"
			SformatOutput(buffer,MBUFFER-1,"|%s|%s|%s|",cmd, name, tValue);
		else if (strlen(name)>0)
			SformatOutput(buffer,MBUFFER-1,"|%s|%s|", name, tValue);
		else
			SformatOutput(buffer,MBUFFER-1,"|%s|", tValue);
		buffer[MBUFFER-1]=0;
        SendIt(client,buffer);
	} else if (status==ERR_VALUE_BAD) {
		if (strlen(tValue)>0)
			SformatOutput(buffer,MBUFFER-1,"%s",tValue);
//			SformatOutput(buffer,MBUFFER-1,"bad value %s for parameter %s",tValue,name);
		else 
			SformatOutput(buffer,MBUFFER-1,"bad value for %s %s",name,tValue);
		buffer[MBUFFER-1]=0;
		SendError(client,buffer);
	    (*error)++;
	} else {
        SformatOutput(buffer,MBUFFER-1,"error %s %s",cmd, name);
        buffer[MBUFFER-1]=0;
        SendError(client,buffer);
	    (*error)++;
    }
}

//
// link parameters
//
enum ConfigCmd 
{
    ConfigCmdHelp=0,
	ConfigCmdFreq,
	ConfigCmdDataRate,
};

struct _ParameterList cpl[]=
{
    {ConfigCmdHelp,"help","?",0}, 
	{ConfigCmdFreq,"frequency","f",0}, 
	{ConfigCmdDataRate,"r","rate",0}, 
};

int parseConfigCmd(int *nFreq, unsigned int *nRate, double *tp, int client, char *cmd)
{
	int np, ip;
	char *name, tValue[MBUFFER];
	int code;
	int ngot=0, error=0;
	int status = VALUE_OK;
	int foundRatesByName;
	unsigned int nrate, rate[MLOOP];


	strcpy(tValue, "");
	np=CommandParameterMany();
	if(np<=0 || np>2)
		ConfigCmdHelpPrint();
	if (np==1) {
		ConfigCmdStatus(ERR_VALUE_BAD, &error, cmd, "", "You need to specify freq and rate", client);
		ConfigCmdHelpPrint();
	}
	for(ip=0; ip<np; ip++)
	{
		name=CommandParameterName(ip);
		code=ParameterSelect(name,cpl,sizeof(cpl)/sizeof(struct _ParameterList));
		switch(code)
		{
			case ConfigCmdHelp:
				ConfigCmdHelpPrint();
				break;
			case ConfigCmdFreq:
				strcpy(tValue, CommandParameterValue(ip,0));
				ngot=SformatInput(tValue," %d ",nFreq);
				if(ngot!=1)
					ConfigCmdStatus(ERR_VALUE_BAD, &error, cmd, name, tValue, client);
				break;
			case ConfigCmdDataRate:
				foundRatesByName=ParseStringAndSetRates(ip,name,MRATE,(int*)rate);//first preference to the rates by name
				if(!foundRatesByName)
				{
					nrate=ParseHex(ip,name,MRATE,rate);
					if(nrate<3)
					{
						rate[2]=0;
						if(nrate<2)
						{
							rate[1]=0;
							if(nrate<1)
							{
								rate[0]=1;	
							}
						}	
					}
				}
				nRate[0] = rate[0];
				nRate[1] = rate[1];
				nRate[2] = rate[2];
				break;
			default:
				UserPrint("ERROR: invalid parameter %s\n",name);
		}	// switch(code)
	}	// for(it=0; it<nt; it++)

	if (error>0)
		return ERR_VALUE_BAD;
	return VALUE_OK;
}


void ConfigGetTPCommand(int client)
{
	int status = VALUE_OK, error=0;
	int freq;
	double tp=0;
	int Rate[numRateCodes], RateMany, ir;
	unsigned int nRate[3];
	char sRate[10], buffer[MBUFFER];
	int nc=0, lc=0;

	nRate[0]=1<<7;		// 54Mbps
	nRate[1]=0;			// HT20
	nRate[2]=0;			// HT40
	freq = 2412;

	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		SendError(client,"no card loaded");
    }
	else
	{
		if (parseConfigCmd(&freq, nRate, &tp, client, "getTP")!=VALUE_OK) {
			ConfigCmdHelpPrint();
			return;
		}
		RateMany = RateCount(nRate[0], nRate[1], nRate[2], Rate);
		for (ir=0; ir<RateMany; ir++) {
			strcpy(sRate,rateStrAll[Rate[ir]]);
			nc = SformatOutput(&buffer[lc],MBUFFER-lc-1,"|%s|", sRate);
			if(nc>0) lc+=nc;
		}
		ConfigCmdStatus(VALUE_OK, &error, "", "", buffer, client);
		lc=0;
		for (ir=0; ir<RateMany; ir++) {
			DeviceTargetPowerGet(freq,Rate[ir],&tp);
			nc = SformatOutput(&buffer[lc],MBUFFER-lc-1,"|%.1f|", tp);
			if(nc>0) lc+=nc;
		}
		ConfigCmdStatus(VALUE_OK, &error, "", "", buffer, client);

		SendDone(client);
	}

}

void ConfigSetTPCommand(int client)
{
}
