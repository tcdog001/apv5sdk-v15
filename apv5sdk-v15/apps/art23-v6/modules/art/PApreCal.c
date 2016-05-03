#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "wlantype.h"
#include "smatch.h"
#include "UserPrint.h"
#include "TimeMillisecond.h"
#include "CommandParse.h"
#include "NewArt.h"
//#include "MyDelay.h"
#include "ParameterSelect.h"
#include "Card.h"
#include "Field.h"
#include "NartRegister.h"
//#include "osprey_reg_map_ART_template.h"

//#include "Device.h"

#include "ParameterParse.h"
#include "ConfigurationStatus.h"
#include "PApreCal.h"

#define MBUFFER 1024

void CmdStatus(int status, unsigned char *name, unsigned char *tvalue, int client)
{

}

int regRead(unsigned char *buffer, int client)
{
//	long address, low, high;
//	unsigned long value;

	CommandParse(buffer);
	RegisterReadCommand(client);

	return VALUE_OK;
}

enum 
{
    PAprecalAll=0,
	PAprecalGain,
	PAprecalHelp,
};

struct _ParameterList paprecalpl[]=
{
    {PAprecalAll,"all",0,0}, 
	{PAprecalGain,"gain",0,0}, 
	{PAprecalHelp,"help",0},		
};

void PAPreCalCommand(int client)
{
	int np, ip;
	unsigned char *name;	
	unsigned char buffer[MBUFFER],tvalue[MBUFFER];
//	int error;
    int status, parseStatus=0;
//	int done;
	int lc=0, nc;
    int code;
    unsigned long value; 

	strcpy(buffer, "");

	//
	// check if card is loaded
	//
	if(!CardValid())
	{
		SendError(client,"no card loaded");
		return;
	}

	//
	// prepare beginning of error message in case we need to use it
	//
//	error=0;
//	done=0;
	//
	//parse arguments and do it
	//
	np=CommandParameterMany();
	for(ip=0; ip<np; ip++)
	{
		status=ERR_VALUE_BAD;		parseStatus=0;
		name=CommandParameterName(ip);
		if (strlen(name)==0) continue;	// sometime there are tab or space at the end count as np with name as "", skip it
		Stolower(name, name);
		strcpy(tvalue, "");
		code=ParameterSelect(name,paprecalpl,sizeof(paprecalpl)/sizeof(struct _ParameterList));
		switch(code)
		{
            case PAprecalGain:
				parseStatus=ParseInteger(ip, name, 1, (int *)&value);

				nc=SformatOutput(&buffer[lc],MBUFFER-lc-1,"rr a=1234");
				if(nc>0) lc+=nc;
				regRead(buffer, client);
				//status = ConfigCmdSetValue(name, ip, tvalue, IsHEX, Ar9300eepromVersion);
				//ConfigurationCmdStatus(status, &done, &error, "set", name, tvalue, client);
                break;
			case PAprecalHelp:
//				SetGetHelp("set", client);
//				done++;
                break;
            default:
//				ConfigurationCmdStatus(ERR_VALUE_BAD, &done, &error, "set", name, "bad parameter name", client); 
                break;
		}
	}
	//
	// send DONE or ERROR
	//
	//if(done==0 && error==0)
	//{
//		ConfigurationCmdStatus(ERR_VALUE_BAD, &done, &error, "get", "", "no valid parameter name", client); 
	//	error++;
	//}

//	SendDone(client);
}
