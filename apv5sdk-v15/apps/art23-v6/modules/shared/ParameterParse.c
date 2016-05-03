
//  "ACI $Id: //depot/sw/branches/narthal_mar2011/dk/mdk/shared/ParameterParse.c#1 $, $Header: //depot/sw/branches/narthal_mar2011/dk/mdk/shared/ParameterParse.c#1 $"


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "smatch.h"
#include "ErrorPrint.h"

#include "ParseError.h"
#include "CommandParse.h"
#include "ParameterSelect.h"
#include "ParameterParse.h"

#define MBUFFER 1024

static struct _Error _ParseError[]=
{
    {ParseBadParameter,ErrorFatal,ParseBadParameterFormat},
    {ParseBadValue,ErrorFatal,ParseBadValueFormat},
    {ParseTooMany,ErrorFatal,ParseTooManyFormat},
    {ParseNegativeIncrement,ErrorFatal,ParseNegativeIncrementFormat},
    {ParsePositiveIncrement,ErrorFatal,ParsePositiveIncrementFormat},
    {ParseMinimumDecimal,ErrorFatal,ParseMinimumDecimalFormat},
    {ParseMaximumDecimal,ErrorFatal,ParseMaximumDecimalFormat},
    {ParseMinimumHex,ErrorFatal,ParseMinimumHexFormat},
    {ParseMaximumHex,ErrorFatal,ParseMaximumHexFormat},
    {ParseMinimumDouble,ErrorFatal,ParseMinimumDoubleFormat},
    {ParseMaximumDouble,ErrorFatal,ParseMaximumDoubleFormat},
    {ParseError,ErrorFatal,ParseErrorFormat},
    {ParseHelp,ErrorInformation,ParseHelpFormat},
    {ParseHelpStart,ErrorControl,ParseHelpStartFormat},
    {ParseHelpEnd,ErrorControl,ParseHelpEndFormat},
    {ParseMinimumUnsigned,ErrorFatal,ParseMinimumUnsignedFormat},
    {ParseMaximumUnsigned,ErrorFatal,ParseMaximumUnsignedFormat},
	{ParseMinimumMac,ErrorFatal,ParseMinimumMacFormat},
    {ParseMaximumMac,ErrorFatal,ParseMaximumMacFormat},
	{ParseHelpSynopsisStart,ErrorControl,ParseHelpSynopsisStartFormat},
	{ParseHelpSynopsisEnd,ErrorControl,ParseHelpSynopsisEndFormat},
	{ParseHelpParametersStart,ErrorControl,ParseHelpParametersStartFormat},
	{ParseHelpParametersEnd,ErrorControl,ParseHelpParametersEndFormat},
	{ParseHelpDescriptionStart,ErrorControl,ParseHelpDescriptionStartFormat},
	{ParseHelpDescriptionEnd,ErrorControl,ParseHelpDescriptionEndFormat},
	{ParseHelpUnknown,ErrorInformation,ParseHelpUnknownFormat},
    {ParseBadCommand,ErrorFatal,ParseBadCommandFormat},
	{ParseBadArrayIndex,ErrorFatal,ParseBadArrayIndexFormat},
	{ParseArrayIndexBound,ErrorFatal,ParseArrayIndexBoundFormat},
	{ParseArrayIndexBound2,ErrorFatal,ParseArrayIndexBound2Format},
	{ParseArrayIndexBound3,ErrorFatal,ParseArrayIndexBound3Format},
};

static int _ErrorFirst=1;

void ParseErrorInit()
{
    if(_ErrorFirst)
    {
        ErrorHashCreate(_ParseError,sizeof(_ParseError)/sizeof(_ParseError[0]));
    }
    _ErrorFirst=0;
}

enum 
{
	 code_all=0,
	 code_all_legacy,
	 code_all_mcs_ht_20,  
	 code_all_mcs_ht_40,  
	 code_all_dvt,  
	 code_11s,  
	 code_11l,  
	 code_5s,   
	 code_5l,
	 code_2s,
	 code_2l,
	 code_1l,
	 code_6,
	 code_9,
	 code_12,
	 code_18,
	 code_24,
	 code_36,
	 code_48,
	 code_54,
	 code_t0,
	 code_t1,
	 code_t2,
	 code_t3,
	 code_t4,
	 code_t5,
	 code_t6,
	 code_t7,
	 code_t8,
	 code_t9,
	 code_t10,
	 code_t11,
	 code_t12,
	 code_t13,
	 code_t14,
	 code_t15,
	 code_t16,
	 code_t17,
	 code_t18,
	 code_t19,
	 code_t20,
	 code_t21,
	 code_t22,
	 code_t23,
	 code_f0,
	 code_f1,
	 code_f2,
	 code_f3,
	 code_f4,
	 code_f5,
	 code_f6,
	 code_f7,
	 code_f8,
	 code_f9,
	 code_f10,
	 code_f11,
	 code_f12,
	 code_f13,
	 code_f14,
	 code_f15,
	 code_f16,
	 code_f17,
	 code_f18,
	 code_f19,
	 code_f20,
	 code_f21,
	 code_f22,
	 code_f23,
};


static struct _ParameterList rates[]=
{
	{code_all, "all"},
	{code_all_legacy, "all_legacy","legacy_all"},
	{code_all_mcs_ht_20, "all_mcs_ht_20","all_mcs_ht20","ht20_mcs_all"},
	{code_all_mcs_ht_40, "all_mcs_ht_40","all_mcs_ht40","ht40_mcs_all"},
	{code_all_dvt, "all_dvt","dvt_all"},
	{code_11s, "11s"},
	{code_11l, "11l"},
	{code_5s, "5s"},
	{code_5l, "5l"},
	{code_2s, "2s"},
	{code_2l, "2l"},
	{code_1l, "1l"},
	{code_6, "6"},
	{code_9, "9"},
	{code_12, "12"},
	{code_18, "18"},
	{code_24, "24"},
	{code_36, "36"},
	{code_48, "48"},
	{code_54, "54"},
	{code_t0, "t0", "mcs0"},
	{code_t1, "t1", "mcs1"},
	{code_t2, "t2", "mcs2"},
	{code_t3, "t3", "mcs3"},
	{code_t4, "t4", "mcs4"},
	{code_t5, "t5", "mcs5"},
	{code_t6, "t6", "mcs6"},
	{code_t7, "t7", "mcs7"},
	{code_t8, "t8", "mcs8"},
	{code_t9, "t9", "mcs9"},
	{code_t10, "t10", "mcs10"},
	{code_t11, "t11", "mcs11"},
	{code_t12, "t12", "mcs12"},
	{code_t13, "t13", "mcs13"},
	{code_t14, "t14", "mcs14"},
	{code_t15, "t15", "mcs15"},
	{code_t16, "t16", "mcs16"},
	{code_t17, "t17", "mcs17"},
	{code_t18, "t18", "mcs18"},
	{code_t19, "t19", "mcs19"},
	{code_t20, "t20", "mcs20"},
	{code_t21, "t21", "mcs21"},
	{code_t22, "t22", "mcs22"},
	{code_t23, "t23", "mcs23"},
	{code_f0, "f0", "mcs0/40"},
	{code_f1, "f1", "mcs1/40"},
	{code_f2, "f2", "mcs2/40"},
	{code_f3, "f3", "mcs3/40"},
	{code_f4, "f4", "mcs4/40"},
	{code_f5, "f5", "mcs5/40"},
	{code_f6, "f6", "mcs6/40"},
	{code_f7, "f7", "mcs7/40"},
	{code_f8, "f8", "mcs8/40"},
	{code_f9, "f9", "mcs9/40"},
	{code_f10, "f10", "mcs10/40"},
	{code_f11, "f11", "mcs11/40"},
	{code_f12, "f12", "mcs12/40"},
	{code_f13, "f13", "mcs13/40"},
	{code_f14, "f14", "mcs14/40"},
	{code_f15, "f15", "mcs15/40"},
	{code_f16, "f16", "mcs16/40"},
	{code_f17, "f17", "mcs17/40"},
	{code_f18, "f18", "mcs18/40"},
	{code_f19, "f19", "mcs19/40"},
	{code_f20, "f20", "mcs20/40"},
	{code_f21, "f21", "mcs21/40"},
	{code_f22, "f22", "mcs22/40"},
	{code_f23, "f23", "mcs23/40"},
};


//
// this function massages the input by replacing any occurence of $NAME with the value
//
static char * (*_ValueReplacement)(char *name, char *buffer, int max);


void ParseParameterReplacement(char * (*f)(char *name, char *buffer, int max))
{
    _ValueReplacement=f;
}

static int TryOtherFormats(char *text, int *value)
{
	int extra;
	int ngot;

	ngot=0;
	if(ngot!=1)
	{
		ngot=sscanf(text, " 0x%x %1c",value,&extra);
	}
	if(ngot!=1)
	{
		ngot=sscanf(text, " x%x %1c",value,&extra);
	}
	if(ngot!=1)
	{
		ngot=sscanf(text, " h%x %1c",value,&extra);
	}
	if(ngot!=1)
	{
		ngot=sscanf(text, " d%d %1c",value,&extra);
	}
	if(ngot!=1)
	{
		ngot=sscanf(text, " t%d %1c",value,&extra);
	}
	if(ngot!=1)
	{
		ngot=sscanf(text, " +%d %1c",value,&extra);
	}
	if(ngot!=1)
	{
		ngot=sscanf(text, " -%d %1c",value,&extra);
		if(ngot==1)
		{
			*value = -(*value);
		}
	}
	if(ngot!=1)
	{
		ngot=sscanf(text, " u%u %1c",value,&extra);
	}

	return ngot;
}


int ParseIntegerList(int input, char *name, int *value, struct _ParameterList *list)
{
	int ip, np;
	int ngot;
	int nvalue;
	int vmin,vmax,vinc,iv;
	char *text, tcopy[MBUFFER];
	char buffer[MBUFFER];
	char *tmin, *tmax, *tinc;
	int index;
	int extra;

    ParseErrorInit();

    nvalue=0;		
	
	np=CommandParameterValueMany(input);
	for(ip=0; ip<np; ip++)
	{
		if(ParameterValueMaximum(list)>0 && nvalue>ParameterValueMaximum(list))
		{
			ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
            nvalue= -1;
			break;
		}

        text=CommandParameterValue(input,ip);
		if(_ValueReplacement!=0)
		{
			(*_ValueReplacement)(text,buffer,MBUFFER);
			text=buffer;
		}

		SformatOutput(tcopy,MBUFFER-1,"%s",text);
		tcopy[MBUFFER-1]=0;
		tinc=0;
		tmax=0;
		tmin=tcopy;
		tmax=strchr(tcopy,':');
		if(tmax!=0)
		{
			*tmax=0;
			tmax++;
			tinc=strchr(tmax,':');
			if(tinc!=0)
			{
				*tinc=0;
				tinc++;
			}
		}
        //
		// parse the minimum value
		//
		ngot= -1;
		if(list->nspecial>0)
		{
			index=ParameterSelectIndex(tmin,list->special,list->nspecial);
			if(index>=0)
			{
				vmin=list->special[index].code;
				ngot=1;
			}
		}
		if(ngot<=0)
		{
			ngot=sscanf(tmin, " %d %1c",&vmin,&extra);
			//
			// also try hex
			//
			if(ngot!=1)
			{
				ngot=TryOtherFormats(tmin,&vmin);
			}
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
			if(list!=0 && list->minimum!=0 && vmin< *((int *)list->minimum))
			{
				ErrorPrint(ParseMinimumDecimal,vmin,*((int *)list->minimum),name);
				break;
			}
			if(list!=0 && list->maximum!=0 && vmin> *((int *)list->maximum))
			{
				ErrorPrint(ParseMaximumDecimal,vmin,*((int *)list->maximum),name);
				break;
			}
		}
		//
		// now parse the maximum value
		//
		if(tmax!=0)
		{
			ngot= -1;
			if(list!=0 && list->nspecial>0)
			{
				index=ParameterSelectIndex(tmin,list->special,list->nspecial);
				if(index>=0)
				{
					vmax=list->special[index].code;
					ngot=1;
				}
			}
			if(ngot<=0)
			{
				ngot=sscanf(tmax, " %d %1c",&vmax,&extra);
				if(ngot!=1)
				{
					ngot=TryOtherFormats(tmax,&vmax);
				}
				if(ngot!=1) 
				{
					ErrorPrint(ParseBadValue,text,name);
					break;
				}
				if(list!=0 && list->minimum!=0 && vmax< *((int *)list->minimum))
				{
					ErrorPrint(ParseMinimumDecimal,vmax, *((int *)list->minimum),name);
					break;
				}
				if(list!=0 && list->maximum!=0 && vmax> *((int *)list->maximum))
				{
					ErrorPrint(ParseMaximumDecimal,vmax, *((int *)list->maximum),name);
					break;
				}
			}
		}
		else
		{
			vmax=vmin;
		}
		//
		// parse the increment
		//
		if(tinc!=0)
		{
			ngot=sscanf(tinc, " %d %1c",&vinc,&extra);
			if(ngot!=1)
			{
				ngot=TryOtherFormats(tinc,&vinc);
			}
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
		}
		else
		{
			vinc=1;
		}

		if(vinc>0)
		{
			if(vmax>=vmin)
			{
		        for(iv=vmin; iv<=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParsePositiveIncrement,name);
			}
		}
		else if(vinc<0)
		{
			if(vmax<=vmin)
			{
		        for(iv=vmin; iv>=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParseNegativeIncrement,name);
				continue;
			}
		}
		else
		{
		    value[nvalue]=vmin;
		    nvalue++;
		}
	}

	return nvalue;
}


int ParseInteger(int input, char *name, int max, int *value)
{
	struct _ParameterList list;

	list.nspecial=0;
	list.nx=max;
	list.ny=1;
	list.nz=1;
	list.minimum=0;
	list.maximum=0;

	return ParseIntegerList(input,name,value,&list);
}


int ParseFloatList(int input, char *name, float *value, struct _ParameterList *list)
{
	int ip, np;
	int ngot;
	int nvalue;
	double vmin,vmax,vinc,iv;
	char *text, tcopy[MBUFFER];
	char buffer[MBUFFER];
	char *tmin, *tmax, *tinc;
	int index;
	int extra;

    ParseErrorInit();

    nvalue=0;		
	
	np=CommandParameterValueMany(input);
	for(ip=0; ip<np; ip++)
	{
		if(ParameterValueMaximum(list)>0 && nvalue>ParameterValueMaximum(list))
		{
			ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
            nvalue= -1;
			break;
		}

        text=CommandParameterValue(input,ip);
		if(_ValueReplacement!=0)
		{
			(*_ValueReplacement)(text,buffer,MBUFFER);
			text=buffer;
		}

		SformatOutput(tcopy,MBUFFER-1,"%s",text);
		tcopy[MBUFFER-1]=0;
		tinc=0;
		tmax=0;
		tmin=tcopy;
		tmax=strchr(tcopy,':');
		if(tmax!=0)
		{
			*tmax=0;
			tmax++;
			tinc=strchr(tmax,':');
			if(tinc!=0)
			{
				*tinc=0;
				tinc++;
			}
		}
        //
		// parse the minimum value
		//
		ngot= -1;
		if(list->nspecial>0)
		{
			index=ParameterSelectIndex(tmin,list->special,list->nspecial);
			if(index>=0)
			{
				vmin=list->special[index].code;
				ngot=1;
			}
		}
		if(ngot<=0)
		{
			ngot=sscanf(tmin, " %lg %1c",&vmin,&extra);
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
			if(list!=0 && list->minimum!=0 && vmin< *((float *)list->minimum))
			{
				ErrorPrint(ParseMinimumDouble,(double)vmin, (double)*((float *)list->minimum),name);
				break;
			}
			if(list!=0 && list->maximum!=0 && vmin> *((float *)list->maximum))
			{
				ErrorPrint(ParseMaximumDouble,(double)vmin, (double)*((float *)list->maximum),name);
				break;
			}
		}
		//
		// now parse the maximum value
		//
		if(tmax!=0)
		{
			ngot= -1;
			if(list!=0 && list->nspecial>0)
			{
				index=ParameterSelectIndex(tmin,list->special,list->nspecial);
				if(index>=0)
				{
					vmax=(float)list->special[index].code;
					ngot=1;
				}
			}
			if(ngot<=0)
			{
				ngot=sscanf(tmax, " %lg %1c",&vmax,&extra);
				if(ngot!=1) 
				{
					ErrorPrint(ParseBadValue,text,name);
					break;
				}
				if(list!=0 && list->minimum!=0 && vmax< *((float *)list->minimum))
				{
					ErrorPrint(ParseMinimumDouble,(double)vmax, (double)*((float *)list->minimum),name);
					break;
				}
				if(list!=0 && list->maximum!=0 && vmax> *((float *)list->maximum))
				{
					ErrorPrint(ParseMaximumDouble,(double)vmax, (double)*((float *)list->maximum),name);
					break;
				}
			}
		}
		else
		{
			vmax=vmin;
		}
		//
		// parse the increment
		//
		if(tinc!=0)
		{
			ngot=sscanf(tinc, " %lg %1c",&vinc,&extra);
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
		}
		else
		{
			vinc=1;
		}

		if(vinc>0)
		{
			if(vmax>=vmin)
			{
		        for(iv=vmin; iv<=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=(float)iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParsePositiveIncrement,name);
			}
		}
		else if(vinc<0)
		{
			if(vmax<=vmin)
			{
		        for(iv=vmin; iv>=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=(float)iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParseNegativeIncrement,name);
				continue;
			}
		}
		else
		{
		    value[nvalue]=(float)vmin;
		    nvalue++;
		}
	}

	return nvalue;
}


int ParseFloat(int input, char *name, int max, float *value)
{
	struct _ParameterList list;

	list.nspecial=0;
	list.nx=max;
	list.ny=1;
	list.nz=1;
	list.minimum=0;
	list.maximum=0;

	return ParseFloatList(input,name,value,&list);
}


int ParseDoubleList(int input, char *name, double *value, struct _ParameterList *list)
{
	int ip, np;
	int ngot;
	int nvalue;
	double vmin,vmax,vinc,iv;
	char *text, tcopy[MBUFFER];
	char buffer[MBUFFER];
	char *tmin, *tmax, *tinc;
	int index;
	int extra;

    ParseErrorInit();

    nvalue=0;		
	
	np=CommandParameterValueMany(input);
	for(ip=0; ip<np; ip++)
	{
		if(ParameterValueMaximum(list)>0 && nvalue>ParameterValueMaximum(list))
		{
			ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
            nvalue= -1;
			break;
		}

        text=CommandParameterValue(input,ip);
		if(_ValueReplacement!=0)
		{
			(*_ValueReplacement)(text,buffer,MBUFFER);
			text=buffer;
		}

		SformatOutput(tcopy,MBUFFER-1,"%s",text);
		tcopy[MBUFFER-1]=0;
		tinc=0;
		tmax=0;
		tmin=tcopy;
		tmax=strchr(tcopy,':');
		if(tmax!=0)
		{
			*tmax=0;
			tmax++;
			tinc=strchr(tmax,':');
			if(tinc!=0)
			{
				*tinc=0;
				tinc++;
			}
		}
        //
		// parse the minimum value
		//
		ngot= -1;
		if(list->nspecial>0)
		{
			index=ParameterSelectIndex(tmin,list->special,list->nspecial);
			if(index>=0)
			{
				vmin=(double)list->special[index].code;
				ngot=1;
			}
		}
		if(ngot<=0)
		{
			ngot=sscanf(tmin, " %lg %1c",&vmin,&extra);
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
			if(list!=0 && list->minimum!=0 && vmin< *((double *)list->minimum))
			{
				ErrorPrint(ParseMinimumDouble,vmin, *((double *)list->minimum),name);
				break;
			}
			if(list!=0 && list->maximum!=0 && vmin> *((double *)list->maximum))
			{
				ErrorPrint(ParseMaximumDouble,vmin, *((double *)list->maximum),name);
				break;
			}
		}
		//
		// now parse the maximum value
		//
		if(tmax!=0)
		{
			ngot= -1;
			if(list!=0 && list->nspecial>0)
			{
				index=ParameterSelectIndex(tmin,list->special,list->nspecial);
				if(index>=0)
				{
					vmax=(double)list->special[index].code;
					ngot=1;
				}
			}
			if(ngot<=0)
			{
				ngot=sscanf(tmax, " %lg %1c",&vmax,&extra);
				if(ngot!=1) 
				{
					ErrorPrint(ParseBadValue,text,name);
					break;
				}
				if(list!=0 && list->minimum!=0 && vmax< *((double *)list->minimum))
				{
					ErrorPrint(ParseMinimumDouble,vmax, *((double *)list->minimum),name);
					break;
				}
				if(list!=0 && list->maximum!=0 && vmax> *((double *)list->maximum))
				{
					ErrorPrint(ParseMaximumDouble,vmax, *((double *)list->maximum),name);
					break;
				}
			}
		}
		else
		{
			vmax=vmin;
		}
		//
		// parse the increment
		//
		if(tinc!=0)
		{
			ngot=sscanf(tinc, " %lg %1c",&vinc,&extra);
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
		}
		else
		{
			vinc=1;
		}

		if(vinc>0)
		{
			if(vmax>=vmin)
			{
		        for(iv=vmin; iv<=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParsePositiveIncrement,name);
			}
		}
		else if(vinc<0)
		{
			if(vmax<=vmin)
			{
		        for(iv=vmin; iv>=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParseNegativeIncrement,name);
				continue;
			}
		}
		else
		{
		    value[nvalue]=vmin;
		    nvalue++;
		}
	}

	return nvalue;
}


int ParseDouble(int input, char *name, int max, double *value)
{
	struct _ParameterList list;

	list.nspecial=0;
	list.nx=max;
	list.ny=1;
	list.nz=1;
	list.minimum=0;
	list.maximum=0;

	return ParseDoubleList(input,name,value,&list);
}

int ParseHexList(int input, char *name, unsigned int *value, struct _ParameterList *list)
{
	int ip, np;
	int ngot;
	int nvalue;
	unsigned int vmin,vmax,vinc,iv;
	char *text, tcopy[MBUFFER];
	char buffer[MBUFFER];
	char *tmin, *tmax, *tinc;
	int index;
	int extra;
	char *tplus;

    ParseErrorInit();

    nvalue=0;		
	
	np=CommandParameterValueMany(input);
	for(ip=0; ip<np; ip++)
	{
		if(ParameterValueMaximum(list)>0 && nvalue>ParameterValueMaximum(list))
		{
			ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
            nvalue= -1;
			break;
		}

        text=CommandParameterValue(input,ip);
		if(_ValueReplacement!=0)
		{
			(*_ValueReplacement)(text,buffer,MBUFFER);
			text=buffer;
		}

		SformatOutput(tcopy,MBUFFER-1,"%s",text);
		tcopy[MBUFFER-1]=0;
		tinc=0;
		tmax=0;
		tmin=tcopy;
		tmax=strchr(tcopy,':');
		if(tmax!=0)
		{
			*tmax=0;
			tmax++;
			tinc=strchr(tmax,':');
			if(tinc!=0)
			{
				*tinc=0;
				tinc++;
			}
		}
        //
		// parse the minimum value
		//
		ngot= -1;
		if(list->nspecial>0)
		{
			index=ParameterSelectIndex(tmin,list->special,list->nspecial);
			if(index>=0)
			{
				vmin=(unsigned int)list->special[index].code;
				ngot=1;
			}
		}
		if(ngot<=0)
		{
			//
			// scan text for + or -. we don't allow those in hex although sscanf does.
			//
			for(tplus=tmin; *tplus!=0 && *tplus!='+' && *tplus!='-'; tplus++);
			if(*tplus==0)
			{
				ngot=sscanf(tmin, " %x %1c",&vmin,&extra);
			}
			else
			{
				ngot= -1;
			}
			if(ngot!=1)
			{
				ngot=TryOtherFormats(tmin,&vmin);
			}
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
			if(list!=0 && list->minimum!=0 && vmin< *((unsigned int *)list->minimum))
			{
				ErrorPrint(ParseMinimumHex,vmin, *((unsigned int *)list->minimum),name);
				break;
			}
			if(list!=0 && list->maximum!=0 && vmin> *((unsigned int *)list->maximum))
			{
				ErrorPrint(ParseMaximumHex,vmin, *((unsigned int *)list->maximum),name);
				break;
			}
		}
		//
		// now parse the maximum value
		//
		if(tmax!=0)
		{
			ngot= -1;
			if(list!=0 && list->nspecial>0)
			{
				index=ParameterSelectIndex(tmin,list->special,list->nspecial);
				if(index>=0)
				{
					vmax=(unsigned int)list->special[index].code;
					ngot=1;
				}
			}
			if(ngot<=0)
			{
				//
				// scan text for + or -. we don't allow those in hex although sscanf does.
				//
				for(tplus=tmax; *tplus!=0 && *tplus!='+' && *tplus!='-'; tplus++);
				if(*tplus==0)
				{
					ngot=sscanf(tmax, " %x %1c",&vmax,&extra);
				}
				else
				{
					ngot= -1;
				}
				if(ngot!=1)
				{
					ngot=TryOtherFormats(tmax,&vmax);
				}
				if(ngot!=1) 
				{
					ErrorPrint(ParseBadValue,text,name);
					break;
				}
				if(list!=0 && list->minimum!=0 && vmax< *((unsigned int *)list->minimum))
				{
					ErrorPrint(ParseMinimumHex,vmax, *((unsigned int *)list->minimum),name);
					break;
				}
				if(list!=0 && list->maximum!=0 && vmax> *((unsigned int *)list->maximum))
				{
					ErrorPrint(ParseMaximumHex,vmax, *((unsigned int *)list->maximum),name);
					break;
				}
			}
		}
		else
		{
			vmax=vmin;
		}
		//
		// parse the increment
		//
		if(tinc!=0)
		{
			//
			// scan text for + or -. we don't allow those in hex although sscanf does.
			//
			for(tplus=tinc; *tplus!=0 && *tplus!='+' && *tplus!='-'; tplus++);
			if(*tplus==0)
			{
				ngot=sscanf(tinc, " %x %1c",&vinc,&extra);
			}
			else
			{
				ngot= -1;
			}
			if(ngot!=1)
			{
				ngot=TryOtherFormats(tinc,&vinc);
			}
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
		}
		else
		{
			vinc=1;
		}

		if(vinc>0)
		{
			if(vmax>=vmin)
			{
		        for(iv=vmin; iv>=vmin && iv<=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParsePositiveIncrement,name);
			}
		}
		else if(vinc<0)
		{
			if(vmax<=vmin)
			{
		        for(iv=vmin; iv<=vmin && iv>=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParseNegativeIncrement,name);
				continue;
			}
		}
		else
		{
		    value[nvalue]=vmin;
		    nvalue++;
		}
	}

	return nvalue;
}


int ParseHex(int input, char *name, int max, unsigned int *value)
{
	struct _ParameterList list;

	list.nspecial=0;
	list.nx=max;
	list.ny=1;
	list.nz=1;
	list.minimum=0;
	list.maximum=0;

	return ParseHexList(input,name,value,&list);
}

int ParseUnsignedList(int input, char *name, unsigned int *value, struct _ParameterList *list)
{
	int ip, np;
	int ngot;
	int nvalue;
	unsigned int vmin,vmax,vinc,iv;
	char *text, tcopy[MBUFFER];
	char buffer[MBUFFER];
	char *tmin, *tmax, *tinc;
	int index;
	int extra;

    ParseErrorInit();

    nvalue=0;		
	
	np=CommandParameterValueMany(input);
	for(ip=0; ip<np; ip++)
	{
		if(ParameterValueMaximum(list)>0 && nvalue>ParameterValueMaximum(list))
		{
			ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
            nvalue= -1;
			break;
		}

        text=CommandParameterValue(input,ip);
		if(_ValueReplacement!=0)
		{
			(*_ValueReplacement)(text,buffer,MBUFFER);
			text=buffer;
		}

		SformatOutput(tcopy,MBUFFER-1,"%s",text);
		tcopy[MBUFFER-1]=0;
		tinc=0;
		tmax=0;
		tmin=tcopy;
		tmax=strchr(tcopy,':');
		if(tmax!=0)
		{
			*tmax=0;
			tmax++;
			tinc=strchr(tmax,':');
			if(tinc!=0)
			{
				*tinc=0;
				tinc++;
			}
		}
        //
		// parse the minimum value
		//
		ngot= -1;
		if(list->nspecial>0)
		{
			index=ParameterSelectIndex(tmin,list->special,list->nspecial);
			if(index>=0)
			{
				vmin=(unsigned int)list->special[index].code;
				ngot=1;
			}
		}
		if(ngot<=0)
		{
			ngot=sscanf(tmin, " %u %1c",&vmin,&extra);
			if(ngot!=1)
			{
				ngot=TryOtherFormats(tmin,&vmin);
			}
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
			if(list!=0 && list->minimum!=0 && vmin< *((unsigned int *)list->minimum))
			{
				ErrorPrint(ParseMinimumUnsigned,vmin, *((unsigned int *)list->minimum),name);
				break;
			}
			if(list!=0 && list->maximum!=0 && vmin> *((unsigned int *)list->maximum))
			{
				ErrorPrint(ParseMaximumUnsigned,vmin, *((unsigned int *)list->maximum),name);
				break;
			}
		}
		//
		// now parse the maximum value
		//
		if(tmax!=0)
		{
			ngot= -1;
			if(list!=0 && list->nspecial>0)
			{
				index=ParameterSelectIndex(tmin,list->special,list->nspecial);
				if(index>=0)
				{
					vmax=(unsigned int)list->special[index].code;
					ngot=1;
				}
			}
			if(ngot<=0)
			{
				ngot=sscanf(tmax, " %u %1c",&vmax,&extra);
				if(ngot!=1)
				{
					ngot=TryOtherFormats(tmax,&vmax);
				}
				if(ngot!=1) 
				{
					ErrorPrint(ParseBadValue,text,name);
					break;
				}
				if(list!=0 && list->minimum!=0 && vmax< *((unsigned int *)list->minimum))
				{
					ErrorPrint(ParseMinimumUnsigned,vmax, *((unsigned int *)list->minimum),name);
					break;
				}
				if(list!=0 && list->maximum!=0 && vmax> *((unsigned int *)list->maximum))
				{
					ErrorPrint(ParseMaximumUnsigned,vmax, *((unsigned int *)list->maximum),name);
					break;
				}
			}
		}
		else
		{
			vmax=vmin;
		}
		//
		// parse the increment
		//
		if(tinc!=0)
		{
			ngot=sscanf(tinc, " %u %1c",&vinc,&extra);
			if(ngot!=1)
			{
				ngot=TryOtherFormats(tinc,&vinc);
			}
			if(ngot!=1) 
			{
				ErrorPrint(ParseBadValue,text,name);
				break;
			}
		}
		else
		{
			vinc=1;
		}

		if(vinc>0)
		{
			if(vmax>=vmin)
			{
		        for(iv=vmin; iv>=vmin && iv<=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParsePositiveIncrement,name);
			}
		}
		else if(vinc<0)
		{
			if(vmax<=vmin)
			{
		        for(iv=vmin; iv<=vmin && iv>=vmax; iv+=vinc)
				{
			        if(ParameterValueMaximum(list)>0 && nvalue>=ParameterValueMaximum(list))
					{
				        ErrorPrint(ParseTooMany,name,ParameterValueMaximum(list));
                        nvalue= -1;
				        break;
					}
		            value[nvalue]=iv;
		            nvalue++;
				}
			}
			else
			{
				ErrorPrint(ParseNegativeIncrement,name);
				continue;
			}
		}
		else
		{
		    value[nvalue]=vmin;
		    nvalue++;
		}
	}

	return nvalue;
}


int ParseUnsigned(int input, char *name, int max, unsigned int *value)
{
	struct _ParameterList list;

	list.nspecial=0;
	list.nx=max;
	list.ny=1;
	list.nz=1;
	list.minimum=0;
	list.maximum=0;

	return ParseIntegerList(input,name,value,&list);
}


int ParseStringAndSetRates(int input, char *name, int max, int *value)
{
	int ip, np;
	int ngot=0;
	int nvalue;
	int code;
	char *text;
	char buffer[MBUFFER];

    ParseErrorInit();

    nvalue=0;		
	value[0]=0;
	value[1]=0;
	value[2]=0;

	np=CommandParameterValueMany(input);
	for(ip=0; ip<np; ip++)
	{
		if(nvalue>max)
		{
			ErrorPrint(ParseTooMany,name,max);
            nvalue= -1;
			break;
		}

        text=CommandParameterValue(input,ip);
		if(_ValueReplacement!=0)
		{
			(*_ValueReplacement)(text,buffer,MBUFFER);
			text=buffer;
		}

        code=ParameterSelect(text,rates,sizeof(rates)/sizeof(struct _ParameterList));

		switch (code)
		{
			case code_all:
				value[0]=0x7fff;
				value[1]=0xffffff;
				value[2]=0xffffff;
				ngot=1;
			    break;
			case code_all_legacy:
				value[0]=0x7fff;
				ngot=1;
			    break;
			case code_all_mcs_ht_20:  
				value[1]=0xffffff;
				ngot=1;
			    break;
			case code_all_mcs_ht_40:  
				value[2]=0xffffff;
				ngot=1;
			    break;
			case code_all_dvt:  
				value[0]|=0x4181;
				value[1]|=0x8181;
				value[2]|=0x8181;
				ngot=1;
			    break;
			case code_11s:  
				value[0]|=0x4000;
				ngot=1;
			    break;
			case code_11l:  
				value[0]|=0x2000;
				ngot=1;
			    break;
			case code_5s:  
				value[0]|=0x1000;
				ngot=1;
			    break;
			case code_5l:  
				value[0]|=0x0800;
				ngot=1;
			    break;
			case code_2s:  
				value[0]|=0x0400;
				ngot=1;
			    break;
			case code_2l:  
				value[0]|=0x0200;
				ngot=1;
			    break;
			case code_1l:  
				value[0]|=0x0100;
				ngot=1;
			    break;
			case code_6:  
				value[0]|=0x0001;
				ngot=1;
			    break;
			case code_9:  
				value[0]|=0x0002;
				ngot=1;
			    break;
			case code_12:  
				value[0]|=0x0004;
				ngot=1;
			    break;
			case code_18:  
				value[0]|=0x0008;
				ngot=1;
			    break;
			case code_24:  
				value[0]|=0x0010;
				ngot=1;
			    break;
			case code_36:  
				value[0]|=0x0020;
				ngot=1;
			    break;
			case code_48:  
				value[0]|=0x0040;
				ngot=1;
			    break;
			case code_54:  
				value[0]|=0x0080;
				ngot=1;
			    break;
			case code_t0:  
				value[1]|=0x0001;
				ngot=1;
			    break;
			case code_t1:  
				value[1]|=0x0002;
				ngot=1;
			    break;
			case code_t2:  
				value[1]|=0x0004;
				ngot=1;
			    break;
			case code_t3:  
				value[1]|=0x0008;
				ngot=1;
			    break;
			case code_t4:  
				value[1]|=0x0010;
				ngot=1;
			    break;
			case code_t5:  
				value[1]|=0x0020;
				ngot=1;
			    break;
			case code_t6:  
				value[1]|=0x0040;
				ngot=1;
			    break;
			case code_t7:  
				value[1]|=0x0080;
				ngot=1;
			    break;
			case code_t8:  
				value[1]|=0x0100;
				ngot=1;
			    break;
			case code_t9:  
				value[1]|=0x0200;
				ngot=1;
			    break;
			case code_t10:  
				value[1]|=0x0400;
				ngot=1;
			    break;
			case code_t11:  
				value[1]|=0x0800;
				ngot=1;
			    break;
			case code_t12:  
				value[1]|=0x1000;
				ngot=1;
			    break;
			case code_t13:  
				value[1]|=0x2000;
				ngot=1;
			    break;
			case code_t14:  
				value[1]|=0x4000;
				ngot=1;
			    break;
			case code_t15:  
				value[1]|=0x8000;
				ngot=1;
			    break;
			case code_t16:  
				value[1]|=0x10000;
				ngot=1;
			    break;
			case code_t17:  
				value[1]|=0x20000;
				ngot=1;
			    break;
			case code_t18:  
				value[1]|=0x40000;
				ngot=1;
			    break;
			case code_t19:  
				value[1]|=0x80000;
				ngot=1;
			    break;
			case code_t20:  
				value[1]|=0x100000;
				ngot=1;
			    break;
			case code_t21:  
				value[1]|=0x200000;
				ngot=1;
			    break;
			case code_t22:  
				value[1]|=0x400000;
				ngot=1;
			    break;
			case code_t23:  
				value[1]|=0x800000;
				ngot=1;
			    break;
			case code_f0:  
				value[2]|=0x0001;
				ngot=1;
			    break;
			case code_f1:  
				value[2]|=0x0002;
				ngot=1;
			    break;
			case code_f2:  
				value[2]|=0x0004;
				ngot=1;
			    break;
			case code_f3:  
				value[2]|=0x0008;
				ngot=1;
			    break;
			case code_f4:  
				value[2]|=0x0010;
				ngot=1;
			    break;
			case code_f5:  
				value[2]|=0x0020;
				ngot=1;
			    break;
			case code_f6:  
				value[2]|=0x0040;
				ngot=1;
			    break;
			case code_f7:  
				value[2]|=0x0080;
				ngot=1;
			    break;
			case code_f8:  
				value[2]|=0x0100;
				ngot=1;
			    break;
			case code_f9:  
				value[2]|=0x0200;
				ngot=1;
			    break;
			case code_f10:  
				value[2]|=0x0400;
				ngot=1;
			    break;
			case code_f11:  
				value[2]|=0x0800;
				ngot=1;
			    break;
			case code_f12:  
				value[2]|=0x1000;
				ngot=1;
			    break;
			case code_f13:  
				value[2]|=0x2000;
				ngot=1;
			    break;
			case code_f14:  
				value[2]|=0x4000;
				ngot=1;
			    break;
			case code_f15:  
				value[2]|=0x8000;
				ngot=1;
			    break;
			case code_f16:  
				value[2]|=0x10000;
				ngot=1;
			    break;
			case code_f17:  
				value[2]|=0x20000;
				ngot=1;
			    break;
			case code_f18:  
				value[2]|=0x40000;
				ngot=1;
			    break;
			case code_f19:  
				value[2]|=0x80000;
				ngot=1;
			    break;
			case code_f20:  
				value[2]|=0x100000;
				ngot=1;
			    break;
			case code_f21:  
				value[2]|=0x200000;
				ngot=1;
			    break;
			case code_f22:  
				value[2]|=0x400000;
				ngot=1;
			    break;
			case code_f23:  
				value[2]|=0x800000;
				ngot=1;
			    break;
		}

	}
	return ngot;
}


//
// parses mac address value.
// returns 0 on success. nonzero error on failure
//
int ParseMacAddress(char *buffer, unsigned char *cmac)
{
	int ngot;
	int mac[6];
	int it;

	for(it=0; it<6; it++)
	{
		cmac[it]=0;
	}
    //
	// try with dots
	//
	ngot=SformatInput(buffer," %x.%x.%x.%x.%x.%x ",
		&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
	if(ngot!=6)
	{
        //
	    // try with colons
	    //
	    ngot=SformatInput(buffer," %x:%x:%x:%x:%x:%x ",
		    &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
	    if(ngot!=6)
		{
            //
	        // try with nothing
	        //
	        ngot=SformatInput(buffer," %2x%2x%2x%2x%2x%2x ",
		        &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
		}
	}
	if(ngot==6)
	{
		for(it=0; it<6; it++)
		{
			cmac[it]=mac[it];
		}
	}
	return (ngot!=6);
}
