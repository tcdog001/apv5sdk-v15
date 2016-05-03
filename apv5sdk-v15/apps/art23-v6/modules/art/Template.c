#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "CommandParse.h"
#include "NewArt.h"
#include "ParameterSelect.h"
#include "ParameterParse.h"
#include "Template.h"
#include "ErrorPrint.h"
#include "ParseError.h"
#include "Card.h"

#include "Ar9300EepromSave.h"

//
// hal header files
//
#include "ah.h"
#include "ah_internal.h"
#include "ar9300.h"
#include "ar9300eep.h"

// 
// this is the hal pointer, 
// returned by ath_hal_attach
// used as the first argument by most (all?) HAL routines
//
struct ath_hal *AH;



#define MBUFFER 1024

#define MLOOP 100

#include "templatelist.h"

static struct _ParameterList TemplateParameter[]=
{
	TEMPLATE_PREFERENCE,
	TEMPLATE_ALLOWED,
	TEMPLATE_MEMORY,
	TEMPLATE_SIZE,
	TEMPLATE_COMPRESS,
	TEMPLATE_OVERWRITE,
	TEMPLATE_INSTALL,
};


void TemplateParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(TemplateParameter)/sizeof(TemplateParameter[0]);
    list->special=TemplateParameter;
}


//
// interpolates into the stored data structure and returns the value that will be used
//
void TemplateCommand(int client)
{
	int np, ip;
	char *name;	
	int error;
	int index;
    int code;
	int preference,npreference;
	int allowed[MALLOWED],nallowed=0;
	int overwrite=0,noverwrite;
	int compress=0,ncompress;
	int memory=0,nmemory;
	int address=0,naddress;
	int install=0,ninstall=0;
	//
	// install default parameter values
	//
	error=0;
	preference= -1;
	npreference= -1;
	noverwrite= -1;
	ncompress= -1;
	nmemory= -1;
	naddress= -1;
	//
	// parse arguments and do it
	//
	np=CommandParameterMany();
	for(ip=0; ip<np; ip++)
	{
		name=CommandParameterName(ip);
		index=ParameterSelectIndex(name,TemplateParameter,sizeof(TemplateParameter)/sizeof(TemplateParameter[0]));
		if(index>=0)
		{
			code=TemplateParameter[index].code;
			switch(code) 
			{
				case TemplatePreference:
					npreference=ParseIntegerList(ip,name,&preference,&TemplateParameter[index]);
					if(npreference<=0)
					{
						error++;
					}
					break;
				case TemplateAllowed:
					nallowed=ParseIntegerList(ip,name,allowed,&TemplateParameter[index]);		
					if(nallowed<=0)
					{
						error++;
					}
					break;
				case TemplateCompress:
					ncompress=ParseIntegerList(ip,name,&compress,&TemplateParameter[index]);		
					if(ncompress<=0)
					{
						error++;
					}
					break;
				case TemplateOverwrite:
					noverwrite=ParseIntegerList(ip,name,&overwrite,&TemplateParameter[index]);		
					if(noverwrite<=0)
					{
						error++;
					}
					break;
				case TemplateMemory:
					nmemory=ParseIntegerList(ip,name,&memory,&TemplateParameter[index]);
					if(nmemory<=0)
					{
						error++;
					}
					break;
				case TemplateSize:
					naddress=ParseIntegerList(ip,name,&address,&TemplateParameter[index]);
					if(naddress<=0)
					{
						error++;
					}
					break;
				case TemplateInstall:
					ninstall=ParseIntegerList(ip,name,&install,&TemplateParameter[index]);
					if(ninstall<=0)
					{
						error++;
					}
					break;
				default:
					error++;
					ErrorPrint(ParseBadParameter,name);
					break;
			}
		}
		else
		{
			error++;
			ErrorPrint(ParseBadParameter,name);
		}
	}

	if(error<=0)
	{
		if(npreference==1)
		{
			ar9300EepromTemplatePreference(preference);
		}
		if(nallowed>=0)
		{
			Ar9300EepromTemplateAllowed((unsigned int*)allowed,(unsigned int)nallowed);
		}
		if(ncompress==1)
		{
			Ar9300EepromCompress(compress);
		}
		if(noverwrite==1)
		{
			Ar9300EepromOverwrite(overwrite);
		}
		if(naddress==1)
		{
			if(address>0)
			{
				address--;
			}
			ar9300CalibrationDataAddressSet(0,address);
		}
		if(nmemory==1)
		{
			ar9300CalibrationDataSet(0,memory);
		}
		if(ninstall==1 && npreference==1)
		{
			if(install==1 || (install==2 && ar9300CalibrationDataGet(AH)==CalibrationDataNone))
			{
				ar9300EepromTemplateInstall(AH, (int32_t)preference);
				CardChannelCalculate();
			}
		}
	}
	
	SendDone(client);
}





