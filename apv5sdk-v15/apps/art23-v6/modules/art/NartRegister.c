


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "mlibif.h"

#include "ParameterSelect.h"
#include "NartRegister.h"

#include "smatch.h"
#include "UserPrint.h"
#include "ParameterParse.h"
#include "CommandParse.h"
#include "Card.h"
#include "NewArt.h"
#include "Field.h"
#include "Sticky.h"
#include "Device.h"
#include "ah_osdep.h"
#include "opt_ah.h"

#include "ErrorPrint.h"
#include "ParseError.h"
#include "NartError.h"
#include "CardError.h"

#define MBUFFER 1024

enum
{
	ResponseStyleVerbose=0,		// normal nart-style response with header, data, ERROR, and DONE messages
	ResponseStyleSplit,			// writes act as for Simple, reads act as for Verbose
	ResponseStyleSimple,		// writes return nothing, reads return value only or ERROR
};

static int _ResponseStyle=ResponseStyleVerbose;

enum
{
	MemoryAddress=0,
	MemoryValue,
	MemorySize,
	MemoryResponse,
	MemoryDebug,
};

//
// parameter definitions
// these are used by multiple commands
//
static struct _ParameterList LogicalParameter[2]=
{
	{0,{"no",0,0},0,0,0,0,0,0,0,0,0},
	{1,{"yes",0,0},0,0,0,0,0,0,0,0,0},
};


static struct _ParameterList ResponseParameter[3]=
{
	{ResponseStyleVerbose,{"verbose",0,0},0,0,0,0,0,0,0,0,0},
	{ResponseStyleSplit,{"split",0,0},0,0,0,0,0,0,0,0,0},
	{ResponseStyleSimple,{"simple",0,0},0,0,0,0,0,0,0,0,0},
};

static int DebugDefault=0;
static struct _ParameterList RegisterDebugParameter[]=
{
    {MemoryDebug,{"debug",0,0},"turn register debug log on or off",'z',0,1,1,1,0,0,&DebugDefault,
		sizeof(LogicalParameter)/sizeof(LogicalParameter[0]),LogicalParameter},
    {MemoryResponse,{"response",0,0},"the response to rx commands",'z',0,1,1,1,0,0,&_ResponseStyle,
		sizeof(ResponseParameter)/sizeof(ResponseParameter[0]),ResponseParameter},
};


static struct _ParameterList RegisterReadParameter[]=
{
    {MemoryAddress,{"address",0,0},"the address",'x',0,1,1,1,0,0,0,0,0},
};

static struct _ParameterList FieldReadParameter[]=
{
    {MemoryAddress,{"address","name",0},"the field name or address",'t',0,1,1,1,0,0,0,0,0},
};

static struct _ParameterList MemoryReadParameter[]=
{
    {MemorySize,{"size","bytes",0},"the number of bytes",'d',0,1,1,1,0,0,0,0,0},
    {MemoryAddress,{"address",0,0},"the address",'x',0,1,1,1,0,0,0,0,0},
};

static struct _ParameterList RegisterWriteParameter[]=
{
    {MemoryAddress,{"address",0,0},"the address",'x',0,1,1,1,0,0,0,0,0},
    {MemoryValue,{"value",0,0},"the value",'x',0,1,1,1,0,0,0,0,0},
};

static struct _ParameterList FieldWriteParameter[]=
{
    {MemoryAddress,{"address","name",0},"the field name or address",'t',0,1,1,1,0,0,0,0,0},
    {MemoryValue,{"value",0,0},"the value",'x',0,1,0,0,0,0,0},
};

static struct _ParameterList MemoryWriteParameter[]=
{
    {MemorySize,{"size","bytes",0},"the number of bytes",'d',0,1,1,1,0,0,0,0,0},
    {MemoryAddress,{"address",0,0},"the address",'x',0,1,1,1,0,0,0,0,0},
    {MemoryValue,{"value",0,0},"the value",'x',0,1,1,1,0,0,0,0,0},
};


void RegisterDebugParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(RegisterDebugParameter)/sizeof(RegisterDebugParameter[0]);
    list->special=RegisterDebugParameter;
}

void RegisterReadParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(RegisterReadParameter)/sizeof(RegisterReadParameter[0]);
    list->special=RegisterReadParameter;
}

void FieldReadParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(FieldReadParameter)/sizeof(FieldReadParameter[0]);
    list->special=FieldReadParameter;
}

void MemoryReadParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(MemoryReadParameter)/sizeof(MemoryReadParameter[0]);
    list->special=MemoryReadParameter;
}

void RegisterWriteParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(RegisterWriteParameter)/sizeof(RegisterWriteParameter[0]);
    list->special=RegisterWriteParameter;
}

void FieldWriteParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(FieldWriteParameter)/sizeof(FieldWriteParameter[0]);
    list->special=FieldWriteParameter;
}

void MemoryWriteParameterSplice(struct _ParameterList *list)
{
    list->nspecial=sizeof(MemoryWriteParameter)/sizeof(MemoryWriteParameter[0]);
    list->special=MemoryWriteParameter;
}

void RegisterDebugCommand(int client)
{
	int ngot;
	int np;
	int ip;
	int value;
	char *name;
	int done;

	value=0;
	done=0;
	np=CommandParameterMany();
	for(ip=0; ip<np; ip++)
	{
		name=CommandParameterName(ip);
		if(Smatch(name,"d")||Smatch(name,"debug")||Smatch(name,"v")||Smatch(name,"value"))
		{
			ngot=SformatInput(CommandParameterValue(ip,0)," %d ",&value);
			if(ngot==1)
			{
				MyRegisterDebug(value);
				done++;
			}
		}
		else if(Smatch(name,"r")||Smatch(name,"response"))
		{
			ngot=SformatInput(CommandParameterValue(ip,0)," %d ",&value);
			if(ngot==1)
			{
				_ResponseStyle=value;
				done++;
			}
		}
	}
	if(done==0)
	{
		SendError(client,"no value");
	}
		
	SendDone(client);
}



void RegisterReadCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address, low, high;
	unsigned int value;
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int bad;
	int selection;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			ErrorPrint(CardNoneLoaded);
		}
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		low=0;
		high=0;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			selection=ParameterSelect(name,RegisterReadParameter,sizeof(RegisterReadParameter)/sizeof(RegisterReadParameter[0]));
			switch(selection)
			{
				case MemoryAddress:
					for(it=0; it<CommandParameterValueMany(ip); it++)
					{
						ngot=SformatInput(CommandParameterValue(ip,it)," %x : %x ",&low,&high);
						if(ngot>=1)
						{
							//
							// check alignment, registers must be addressed as 0, 4, 8, ...
							//
							low=4*(low/4);

							if(ngot<2)
							{
								high=low;
							}
							for(address=low; address<=high; address+=4)
							{
		//		                bad=MyRegisterRead(address,&value);
								bad=DeviceRegisterRead(address,&value);
								if(bad)
								{
									if(_ResponseStyle!=ResponseStyleSimple)
									{
										nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad read from address %x, ",address);
										if(nc>0)
										{
											lc+=nc;
										}
									}
									else
									{
										nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
										if(nc>0)
										{
											lc+=nc;
										}
									}
									error++;
								}
								else
								{
									if(_ResponseStyle!=ResponseStyleSimple)
									{
										SformatOutput(buffer,MBUFFER-1,"|rr|%04x|%08x|",address,value);
										if(done==0)
										{
											ErrorPrint(NartDataHeader,"|rr|address|value|");
										}
										ErrorPrint(NartData,buffer);
									}
									else
									{
										nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"%x,",value);
										if(nc>0)
										{
											lc+=nc;
										}
									}
									done++;
								}
							}
						}
						else
						{
							if(_ResponseStyle!=ResponseStyleSimple)
							{
								nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad address %s, ",CommandParameterValue(ip,it));
								if(nc>0)
								{
									lc+=nc;
								}
							}
							else
							{
								nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
								if(nc>0)
								{
									lc+=nc;
								}
							}
							error++;
						}
					}
					break;
				default:
					ErrorPrint(ParseBadParameter,name);
					break;
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid address");
				if(nc>0)
				{
					lc+=nc;
				}
				error++;
			}
			if(error>0)
			{
				SendError(client,ebuffer);
			}
		}
		else
		{
			ErrorPrint(NartOther,ebuffer);
		}
	}
	if(_ResponseStyle!=ResponseStyleSimple)
	{
		SendDone(client);
	}
}


void RegisterWriteCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address;
	unsigned int value;
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int bad;
	//
	// check if card is loaded
	//
    if(CardCheckAndLoad(client)!=0)
    {
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			ErrorPrint(CardNoneLoaded);
		}
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		address= -1;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&address);
					if(ngot!=1)
					{
						address=0;
					}
					//
					// check alignment, registers must be addressed as 0, 4, 8, ...
					//
					address=4*(address/4);
				}
			}
			if(Smatch(name,"v")||Smatch(name,"value"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&value);
					if(ngot==1 && address>=0)
					{
	//		            bad=MyRegisterWrite(address+it*sizeof(value),value);
						bad=DeviceRegisterWrite(address+it*sizeof(value),value);
						if(bad)
						{
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad write to address %lx, ",address+it*sizeof(value));
								if(nc>0)
								{
									lc+=nc;
								}
							}
							error++;
						}
						else
						{
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								SformatOutput(buffer,MBUFFER-1,"|rw|%04x|%08x|",(unsigned int)(address+it*sizeof(value)),value);
								if(done==0)
								{
									ErrorPrint(NartDataHeader,"|rw|address|value|");
								}
								ErrorPrint(NartData,buffer);
							}
							done++;
						}
					}
					else
					{
						if(_ResponseStyle==ResponseStyleVerbose)
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad address %s, ",CommandParameterValue(ip,it));
							if(nc>0)
							{
								lc+=nc;
							}
						}
						error++;
					}
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid address or no valid value");
				if(nc>0)
				{
					lc+=nc;
				}
				error++;
			}
			if(error>0)
			{
				SendError(client,ebuffer);
			}
		}
	}
	if(_ResponseStyle==ResponseStyleVerbose)
	{
		SendDone(client);
	}
}


static int BitFirst(unsigned int mask)
{
	int it;

	for(it=0; it<32; it++)
	{
		if((1<<it) & mask)
		{
			return it;
		}
	}
	return 0;
}

enum
{
    RegisterAddress=0,
    RegisterValue,
};


static void FieldReadHeaderSend(int client)
{
	ErrorPrint(NartDataHeader,"|fr|name|address|register|high|low|mask|hex|decimal|signed|");
}

static void FieldWriteHeaderSend(int client)
{
	ErrorPrint(NartDataHeader,"|fw|name|address|register|high|low|mask|hex|decimal|signed|");
}

static int FieldNameLookup(char *name, int *address, int *high, int *low)
{
    int ngot;
    int flip;
	char extra[2];
 	//
	// look up in our big list of field names
	//
	ngot=FieldFind(name,(unsigned int *)address,low,high);
	if(ngot!=1)
	{
		//
		// let's try to parse it as register[high,low]
		//
		ngot=SformatInput(name," %x [ %d , %d ] %1c ",address,high,low,extra);
		if(ngot!=3)
		{
			//
			// let's try to parse it as register[bit]
			//
			ngot=SformatInput(name," %x [ %d ] %1c ",address,high,extra);
			if(ngot==2)
			{
				*low= *high;
				ngot=3;
			}
			else
			{
				//
				// let's try to parse it as register
				//
				ngot=SformatInput(name," %x %1c ",address,extra);
				if(ngot==1)
				{
					*low=0;
					*high=31;
					ngot=3;
				}
				else
				{
					ngot= -1;
				}
			}
		}
	}
	if(ngot>0 && (*low) > (*high))
	{
		flip= *high;
		*high= *low;
		*low=flip;
	}
		
    return (ngot<=0);
}


static int FieldParse(char *name, 
    unsigned int *address, int *high, int *low, int max) 
{
	char *first,*last;
    unsigned int faddress, laddress;
    int flow, fhigh, llow, lhigh;
    int error;
    int it;
    //
    // see if this is a range specification
    //
    last=strchr(name,':');
    if(last)
    {
        *last=0;
        first=name;
        last++;
    }
    else
    {
        first=name;
        last=0;
    }
    error=FieldNameLookup(first,(int *)&faddress,&fhigh,&flow);
    if(last!=0 && !Smatch(last,""))
    {
        error+=FieldNameLookup(last,(int *)&laddress,&lhigh,&llow);
    }
    else
    {
        laddress=faddress;
        lhigh=fhigh;
        llow=flow;
    }
    if(last==0 )
    {
        last="";
    }

    it=1;
    address[0]=faddress;
    low[0]=flow;
    high[0]=fhigh;
    for(it=1, faddress+=4; it<max && faddress<laddress; it++, faddress+=4)
    {
        address[it]=faddress;
        high[it]=31;
        low[it]=0;
    }
    if(it<max && faddress<=laddress)
    {
        address[it]=laddress;
        high[it]=lhigh;
        low[it]=llow;
        it++;
    }

    return it;
}


#define MFIELD 200

static int FieldParseLookupAndReturn(int client, char *name)
{
	unsigned int mask;
	unsigned int reg;
	unsigned int value;
    int sign;
	char fname[MBUFFER],buffer[MBUFFER];
    unsigned int address[MFIELD];
    int low[MFIELD], high[MFIELD];
    int nfield;
    int ia;
	int nc;
	int bad;
    char *registerName,*fieldName;

    nfield=FieldParse(name,address,high,low,MFIELD);
	//
	// now do the read operation
	//
    if(nfield>0)
    {
        for(ia=0; ia<nfield; ia++)
        {
			//
			// check alignment, registers must be addressed as 0, 4, 8, ...
			//
			address[ia]=4*(address[ia]/4);

		    bad=DeviceRegisterRead(address[ia],&reg);
		    if(bad)
		    {
		        nc=SformatOutput(buffer,MBUFFER-1,"bad read from address %x for %s",address[ia],name);
                SendError(client,buffer);
		    }
		    else
		    {	
			    mask=MaskCreate(low[ia],high[ia]);
			    value=(reg&mask)>>low[ia];
                if(value&(1<<(high[ia]-low[ia])))
                {
                    sign=(0xffffffff^(mask>>low[ia]))|value;
                }
                else
                {
                    sign=value;
                }

                if(!FieldFindByAddress(address[ia], low[ia], high[ia], &registerName, &fieldName))
                {
                    SformatOutput(fname,MBUFFER-1,"%s.%s",registerName,fieldName);
                    fname[MBUFFER-1]=0;
                }
                else
                {
                    SformatOutput(fname,MBUFFER-1,"%04x[%d,%d]",address[ia],high[ia],low[ia]);
                    fname[MBUFFER-1]=0;
                }

		        SformatOutput(buffer,MBUFFER-1,"|fr|%s|%04x|%08x|%d|%d|%08x|%08x|%u|%d|",
                    fname,address[ia],reg,high[ia],low[ia],mask,value,value,sign);
		        ErrorPrint(NartData,buffer);
		    }
	    }
    }
	else
	{
		nc=SformatOutput(buffer,MBUFFER-1,"bad field name %s",name);
        SendError(client,buffer);
	}
    return 0;
}


void FieldReadCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	char *name;
    int done;
    int code;
    int nvalue;
    static struct _ParameterList rr[]=
    {
	    {RegisterAddress,"address","name",0},
    };
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			ErrorPrint(CardNoneLoaded);
		}	
    }
	else
	{
		done=0;
		//
		// parse arguments and do it
		//
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			//
			// see if this is a valid parameter name
			//
			code=ParameterSelect(name,rr,sizeof(rr)/sizeof(struct _ParameterList));
			switch(code)
			{
				case RegisterAddress:
					if(done==0)
					{
						FieldReadHeaderSend(client);
						done=1;
					}
					nvalue=CommandParameterValueMany(ip);
					for(it=0; it<nvalue; it++)
					{	
						name=CommandParameterValue(ip,it);
						ngot=FieldParseLookupAndReturn(client,name);
					}
					break;
				default:
					if(done==0)
					{
						FieldReadHeaderSend(client);
						done=1;
					}
					ngot=FieldParseLookupAndReturn(client,name);
				break;
			}
		}
	}
	//
	// send DONE
	//
	if(_ResponseStyle!=ResponseStyleSimple)
	{
		SendDone(client);
	}
}


static int SetAndReturn(int client, unsigned int address, int high, int low, unsigned int value)
{
    char buffer[MBUFFER];
    int done;
    int error;
    int bad;
    unsigned int mask;
    unsigned int reg;
    int sign;
    char *registerName,*fieldName;
    char fname[MBUFFER];

    error=0;
    done=0;
	//
	// check alignment, registers must be addressed as 0, 4, 8, ...
	//
	address=4*(address/4);

	bad=DeviceRegisterRead(address,&reg);
	if(bad)
	{
		SformatOutput(buffer,MBUFFER-1,"bad read from address %x",address);
        SendError(client,buffer);
		error++;
	}
	else
	{
		mask=MaskCreate(low,high);
		reg &= ~(mask);							// clear bits
		reg |= ((value<<low)&mask);				// set new value
        UserPrint("|=%08x  ",reg);
		DeviceRegisterWrite(address,reg);
        if(value&(1<<(high-low)))
        {
            sign=(0xffffffff^(mask>>low))|value;
        }
        else
        {
            sign=value;
        }

        if(!FieldFindByAddress(address, low, high, &registerName, &fieldName))
        {
            SformatOutput(fname,MBUFFER-1,"%s.%s",registerName,fieldName);
            fname[MBUFFER-1]=0;
        }
        else {
        
            SformatOutput(fname,MBUFFER-1,"%04x[%d,%d]",address,high,low);
            fname[MBUFFER-1]=0;
        }

		SformatOutput(buffer,MBUFFER-1,"|fw|%s|%04x|%08x|%d|%d|%08x|%08x|%u|%d|",
            fname,address,reg,high,low,mask,value,value,sign);
		ErrorPrint(NartData,buffer);
		done++;
	}
    return done+error;
}


void FieldWriteCommand(int client)
{
    int ngot;
	int nt;
	int np;
	int ip, it;

    int naddress;
	unsigned int address[MFIELD];
	int low[MFIELD], high[MFIELD];
    int nvalue;
    unsigned int value[MFIELD];

	char *name;
	char buffer[MBUFFER];
	int error;
	int done;
	int lc;
    int code;

    static struct _ParameterList rr[]=
    {
	    {RegisterAddress,"address","name",0},
	    {RegisterValue,"value",0,0},
    };
	//
	// check if card is loaded
	//
	if(CardValid())
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		naddress=0;
		nvalue=0;
		//
		// parse arguments and do it
		//
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			//
			// see if this is a valid parameter name
			//
			code=ParameterSelect(name,rr,sizeof(rr)/sizeof(struct _ParameterList));
			nt=CommandParameterValueMany(ip);
			switch(code)
			{
				case RegisterAddress:
					for(it=0; it<nt; it++)
					{	
						name=CommandParameterValue(ip,it);
						ngot=FieldParse(name,&address[naddress],&high[naddress],&low[naddress],MFIELD-naddress);
						if(ngot>0)
						{
							naddress+=ngot;
						}
						else
						{
							SformatOutput(buffer,MBUFFER-1,"bad field name %s",name);
							SendError(client,buffer);
							error++;
						}
					}
					break;
				case RegisterValue:
					nvalue=ParseHex(ip, name, MFIELD, (unsigned int *)value);
					if(nvalue>0)
					{
						if(done==0)
						{
							FieldWriteHeaderSend(client);
							done=1;
						}
						for(it=0; it<naddress; it++)
						{
							done+=SetAndReturn(client,address[it],high[it],low[it],value[it%nvalue]);
						}
					}
					break;
				default:
					naddress=0;
					ngot=FieldParse(name,&address[naddress],&high[naddress],&low[naddress],MFIELD-naddress);
					if(ngot>0)
					{
						naddress+=ngot;
					}
					else
					{
						SformatOutput(buffer,MBUFFER-1,"bad field name %s",name);
						SendError(client,buffer);
						error++;
					}

					nvalue=ParseHex(ip, name, MFIELD, (unsigned int *)value);
					if(nvalue>0)
					{
						if(done==0)
						{
							FieldWriteHeaderSend(client);
							done=1;
						}
						for(it=0; it<naddress; it++)
						{
							done+=SetAndReturn(client,address[it],high[it],low[it],value[it%nvalue]);
						}
					}
					break;
			}
		}
		//
		// send DONE or ERROR
		//
		if(done==0 && error==0)
		{
			SformatOutput(buffer,MBUFFER-1,"no valid field name or no valid value");
			SendError(client,buffer);
			error++;
		}
	}
	else
	{
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			ErrorPrint(CardNoneLoaded);
		}
	}
	if(_ResponseStyle==ResponseStyleVerbose)
	{
		SendDone(client);
	}
}


void FieldStickyCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	unsigned int address;
	int low, high;
	unsigned int value;
	char *name;
	char ebuffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int gotaddress;
	int gotvalue;
	int doit;
	int multiple;

	address=0;
	low=0;
	high=0;
	value=0;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		ErrorPrint(CardNoneLoaded);
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		doit=1;
		multiple=1;
		//
		//parse arguments and do it
		//
		gotaddress=0;
		gotvalue=0;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"a")||Smatch(name,"address")||Smatch(name,"n")||Smatch(name,"name"))
			{
	//			for(it=0; it<CommandParameterValueMany(ip); it++)
				it=0;
				{
					//
					// look up in our big list of field names
					//
					ngot=FieldFind(CommandParameterValue(ip,it),&address,&low,&high);
					if(ngot!=1)
					{
						//
						// let's try to parse it as register[low:high]
						//
						ngot=SformatInput(CommandParameterValue(ip,it)," %x [ %d : %d ]",&address,&low,&high);
						if(ngot>0)
						{
							if(ngot==1)
							{
								low=0;
								high=31;
								ngot=3;
							}
							if(ngot==2)
							{
								high=31;
								ngot=3;
							}
						}
					}
					//
					// now do the read operation
					//
					if(ngot<=0 || address<0)
					{
						nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad field name %s, ",CommandParameterValue(ip,it));
						if(nc>0)
						{
							lc+=nc;
						}
						error++;
					}
					else
					{
						gotaddress=1;
					}
				}
			}
			else if(Smatch(name,"v")||Smatch(name,"value"))
			{
	//			for(it=0; it<CommandParameterValueMany(ip); it++)
				it=0;
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&value);
					if(ngot==1)
					{
						gotvalue=1;	
					}
					else
					{
						nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad field name %s, ",CommandParameterValue(ip,it));
						if(nc>0)
						{
							lc+=nc;
						}
						error++;
					}
				}
			}
			else if(Smatch(name,"do")||Smatch(name,"d"))
			{
				it=0;
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %d ",&doit);
				}
			}
			else if(Smatch(name,"multiple")||Smatch(name,"m"))
			{
				it=0;
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %d ",&multiple);
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(gotaddress && gotvalue)
		{
			if(multiple)
			{
				StickyInternalAdd(address,low,high,value);
			}
			else
			{
				StickyInternalChange(address,low,high,value);
			}
			if(doit)
			{
				MyFieldWrite(address,low,high,value);
			}
		}
		else if(error>0)
		{
			SendError(client,ebuffer);
		}
		else
		{
			lc=0;
			nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid field name or no valid value");
			if(nc>0)
			{
				lc+=nc;
				SendError(client,ebuffer);
			}
		}
	}
	SendDone(client);
}


void FieldStickyClear(int client)
{
	int ngot;
	int np;
	int ip, it;
	unsigned int address;
	int low, high;
	char *name;
	char ebuffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int multiple;
	int serror;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		ErrorPrint(CardNoneLoaded);
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		multiple=1;
		//
		//parse arguments and do it
		//
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"a")||Smatch(name,"address")||Smatch(name,"n")||Smatch(name,"name"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					if(Smatch(CommandParameterValue(ip,it),"all") || Smatch(CommandParameterValue(ip,it),"*"))
					{
						StickyClear();
						done++;
					}
					else
					{
						//
						// look up in our big list of field names
						//
						ngot=FieldFind(CommandParameterValue(ip,it),&address,&low,&high);
						if(ngot!=1)
						{
							//
							// let's try to parse it as register:low:high
							//
							ngot=SformatInput(CommandParameterValue(ip,it)," %x [ %d : %d ]",&address,&low,&high);
							if(ngot>0)
							{
								if(ngot==1)
								{
									low=0;
									high=31;
									ngot=3;
								}
								if(ngot==2)
								{
									high=31;
									ngot=3;
								}
							}
						}
						//
						// now do the read operation
						//
						if(ngot<=0 || address<0)
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad field name %s, ",CommandParameterValue(ip,it));
							if(nc>0)
							{
								lc+=nc;
							}
							error++;
						}
						else
						{
							done++;
							serror=StickyInternalClear(address,low,high);
							if(multiple==0)
							{
								while(serror==0)
								{
									serror=StickyInternalClear(address,low,high);
								}
							}
							done++;
						}
					}
				}
			}
			else if(Smatch(name,"multiple")||Smatch(name,"m"))
			{
				it=0;
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %d ",&multiple);
				}
			}
		}
		//
		// send ERROR
		//
		if(done<=0 && error<=0)
		{
			lc=0;
			nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid field name or no valid value");
			if(nc>0)
			{
				lc+=nc;
				error++;
			}
		}
		if(error>0)
		{
			SendError(client,ebuffer);
		}
	}
	SendDone(client);
}

static void StickyListHeaderSend(int client)
{
	ErrorPrint(NartDataHeader,"|sl|name|address|high|low|mask|hex|decimal|signed|");
}


void FieldStickyList(int client)
{
	unsigned int address;
	int low, high;
	unsigned int value;
	char buffer[MBUFFER];
	int error;
    int sign;
	char fname[MBUFFER];
    char *registerName,*fieldName;
    unsigned int mask;
	//
	// check if card is loaded
	//
	if(!CardValid())
	{
		ErrorPrint(CardNoneLoaded);
		return;
	}

    StickyListHeaderSend(client);

	for(error=StickyHead(&address,&low,&high,&value); error==0; error=StickyNext(&address,&low,&high,&value))
	{
		mask=MaskCreate(low,high);
        if(value&(1<<(high-low)))
        {
            sign=(0xffffffff^(mask>>low))|value;
        }
        else
        {
            sign=value;
        }

        if(!FieldFindByAddress(address, low, high, &registerName, &fieldName))
        {
            SformatOutput(fname,MBUFFER-1,"%s.%s",registerName,fieldName);
            fname[MBUFFER-1]=0;
        }
        else
        {
            SformatOutput(fname,MBUFFER-1,"%04x[%d,%d]",address,high,low);
            fname[MBUFFER-1]=0;
        }

		SformatOutput(buffer,MBUFFER-1,"|sl|%s|%04x|%d|%d|%08x|%08x|%u|%d|",
					  fname,address,high,low,mask,value,value,sign);
		ErrorPrint(NartData,buffer);
	}
	
	SendDone(client);
}


void ConfigReadCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address, low, high;
	unsigned int value;
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		ErrorPrint(CardNoneLoaded);
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		low=0;
		high=0;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x : %x ",&low,&high);
					if(ngot>=1)
					{
						if(ngot<2)
						{
							high=low;
						}
						for(address=low; address<=high; address+=4)
						{
							value=OScfgRead(0,address);
							{
								if(_ResponseStyle!=ResponseStyleSimple)
								{
									SformatOutput(buffer,MBUFFER-1,"|cr|%04x|%08x|",address,value);
									if(done==0)
									{
										ErrorPrint(NartDataHeader,"|cr|address|value|");
									}
									ErrorPrint(NartData,buffer);
								}
								else
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"%x,",value);
									if(nc>0)
									{
										lc+=nc;
									}
								}
								done++;
							}
						}
					}
					else
					{
						if(_ResponseStyle!=ResponseStyleSimple)
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad address %s, ",CommandParameterValue(ip,it));
							if(nc>0)
							{
								lc+=nc;
							}
						}
						else
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
							if(nc>0)
							{
								lc+=nc;
							}
						}
						error++;
					}
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid address");
				if(nc>0)
				{
					lc+=nc;
				}
				error++;
			}
			if(error>0)
			{
				SendError(client,ebuffer);
			}
		}
		else
		{
			ErrorPrint(NartOther,ebuffer);
		}
	}
	if(_ResponseStyle!=ResponseStyleSimple)
	{
		SendDone(client);
	}
}

static void FieldListHeaderSend(void)
{
	ErrorPrint(NartDataHeader,"|fl|name|address|high|low|mask|");
}


static void FieldListPrint(char *name, unsigned int address, int low, int high)
{
	char buffer[MBUFFER];
	unsigned int mask;

	mask=MaskCreate(low,high);
	SformatOutput(buffer,MBUFFER-1,"|fl|%s|%x|%d|%d|%x|",name,address,high,low,mask);
	ErrorPrint(NartData,buffer);
}


void FieldListCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	char *name;
	int error;
	int done;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		ErrorPrint(CardNoneLoaded);
    }
	else
	{
		//
		// parse arguments and do it
		//
		FieldListHeaderSend();
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"a")||Smatch(name,"address")||Smatch(name,"n")||Smatch(name,"name"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=FieldList(CommandParameterValue(ip,it),FieldListPrint);
				}
			}
			else
			{
				ErrorPrint(ParseBadParameter,name);
			}
		}
	}
	SendDone(client);
}


void ConfigWriteCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address;
	unsigned int value;
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	//
	// check if card is loaded
	//
	if(!CardValid())
	{
		if(_ResponseStyle==ResponseStyleVerbose)
		{
		    ErrorPrint(CardNoneLoaded);
		}
	}
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		address= -1;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&address);
					if(ngot!=1)
					{
						address=0;
					}
				}
			}
			if(Smatch(name,"v")||Smatch(name,"value"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&value);
					if(ngot==1 && address>=0)
					{
						OScfgWrite(0,address,value);
						{
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								SformatOutput(buffer,MBUFFER-1,"|cw|%04x|%08x|",(unsigned int)(address+it*sizeof(value)),value);
								if(done==0)
								{
									ErrorPrint(NartDataHeader,"|cw|address|value|");
								}
								ErrorPrint(NartData,buffer);
							}
							done++;
						}
					}
					else
					{
						if(_ResponseStyle==ResponseStyleVerbose)
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad address %s, ",CommandParameterValue(ip,it));
							if(nc>0)
							{
								lc+=nc;
							}
						}
						error++;
					}
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid address or no valid value");
				if(nc>0)
				{
					lc+=nc;
				}
				error++;
			}
			if(error>0)
			{
				SendError(client,ebuffer);
			}
		}
	}
	if(_ResponseStyle==ResponseStyleVerbose)
	{
		SendDone(client);
	}
}


void MemoryReadCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address, low, high;
	unsigned int value;
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int bad;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			ErrorPrint(CardNoneLoaded);
		}
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		low=0;
		high=0;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x : %x ",&low,&high);
					if(ngot>=1)
					{
						if(ngot<2)
						{
							high=low;
						}
						for(address=low; address<=high; address+=4)
						{
	//		                bad=MyMemoryRead(address,&value,sizeof(unsigned int));
							bad=DeviceMemoryRead(address,&value,sizeof(unsigned int));
							if(bad)
							{
								if(_ResponseStyle!=ResponseStyleSimple)
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad read from address %x, ",address);
									if(nc>0)
									{
										lc+=nc;
									}
								}
								else
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
									if(nc>0)
									{
										lc+=nc;
									}
								}
								error++;
							}
							else
							{
								if(_ResponseStyle!=ResponseStyleSimple)
								{
									SformatOutput(buffer,MBUFFER-1,"|mr|%04x|%08x|",address,value);
									if(done==0)
									{
										ErrorPrint(NartDataHeader,"|mr|address|value|");
									}
									ErrorPrint(NartData,buffer);
								}
								else
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"%x,",value);
									if(nc>0)
									{
										lc+=nc;
									}
								}
								done++;
							}
						}
					}
					else
					{
						if(_ResponseStyle!=ResponseStyleSimple)
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad address %s, ",CommandParameterValue(ip,it));
							if(nc>0)
							{
								lc+=nc;
							}
						}
						else
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
							if(nc>0)
							{
								lc+=nc;
							}
						}
						error++;
					}
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid address");
				if(nc>0)
				{
					lc+=nc;
				}
				error++;
			}
			if(error>0)
			{
				SendError(client,ebuffer);
			}
		}
		else
		{
			ErrorPrint(NartOther,ebuffer);
		}
	}
	if(_ResponseStyle!=ResponseStyleSimple)
	{
		SendDone(client);
	}
}


void MemoryWriteCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address;
	unsigned int value;
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int bad;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			ErrorPrint(CardNoneLoaded);
		}
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		address= -1;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&address);
					if(ngot!=1)
					{
						address=0;
					}
				}
			}
			if(Smatch(name,"v")||Smatch(name,"value"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&value);
					if(ngot==1 && address>=0)
					{
	//		            bad=MyMemoryWrite(address+it*sizeof(value),&value,sizeof(value));
						bad=DeviceMemoryWrite(address+it*sizeof(value),&value,sizeof(value));
						if(bad)
						{
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad write to address %lx, ",address+it*sizeof(value));
								if(nc>0)
								{
									lc+=nc;
								}
							}
							error++;
						}
						else
						{
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								SformatOutput(buffer,MBUFFER-1,"|mw|%04x|%08x|",(unsigned int)(address+it*sizeof(value)),value);
								if(done==0)
								{
									ErrorPrint(NartDataHeader,"|mw|address|value|");
								}
								ErrorPrint(NartData,buffer);
							}
							done++;
						}
					}
					else
					{
						if(_ResponseStyle==ResponseStyleVerbose)
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad address %s, ",CommandParameterValue(ip,it));
							if(nc>0)
							{
								lc+=nc;
							}
						}
						error++;
					}
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid address or no valid value");
				if(nc>0)
				{
					lc+=nc;
				}
				error++;
			}
			if(error>0)
			{
				SendError(client,ebuffer);
			}
		}
	}
	if(_ResponseStyle==ResponseStyleVerbose)
	{
		SendDone(client);
	}
}

void EepromReadCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address, low, high;
	int s;
	unsigned char val[4];
	unsigned char value_1; // 1 byte value
	unsigned short value_2; // 2 byte value
	unsigned int value_4; // 2 byte value
	int size=2; //default 2 bytes for eprom read/write value
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int bad;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			ErrorPrint(CardNoneLoaded);
		}
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		// parse arguments and do it
		//
		low=0;
		high=0;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"s")||Smatch(name,"size"))
			{
				ngot=SformatInput(CommandParameterValue(ip,0)," %d ",&size);
				if(CommandParameterValueMany(ip)!=1 || ngot!=1 || (!((size==1)||(size==2)||(size==4))))
				{
					SformatOutput(buffer,MBUFFER-1,"bad value for parameter size");
					buffer[MBUFFER-1]=0;
					SendError(client,buffer);
					size=2;
				}
			}
			else if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x : %x ",&low,&high);
					if(ngot>=1)
					{
						if(ngot<2)
						{
							high=low;
						}
						for(address=low; address<=high; address+=size)
						{
							bad=0;
							for(s=0;s<size;s++)
							{
								bad|=DeviceEepromRead(address+s,&val[s],1); // reads 8 bits
							}
							for(s=size; s<4; s++)
							{
								val[s]=0;
							}
							
							value_4 = val[0] | (val[1]<<8) | (val[2]<<16) | (val[3]<<24);
							value_2 = val[0] | (val[1]<<8);
							value_1 = val[0];

							if(bad)
							{
								if(_ResponseStyle!=ResponseStyleSimple)
								{
									nc=SformatOutput(buffer,MBUFFER-1,"bad read from eeprom address %x",address);
									buffer[MBUFFER-1]=0;
									SendError(client,buffer);
								}
								else
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
									if(nc>0)
									{
										lc+=nc;
									}
								}
								error++;
							}
							else
							{
								if(_ResponseStyle!=ResponseStyleSimple)
								{
									
									if(size==1){
										SformatOutput(buffer,MBUFFER-1,"|er|%04x|%02x|",address,value_1);
									}
									if(size==2){
										SformatOutput(buffer,MBUFFER-1,"|er|%04x|%04x|",address,value_2);
									}
									if(size==4){
										SformatOutput(buffer,MBUFFER-1,"|er|%04x|%08x|",address,value_4);
									}
									if(done==0)
									{
										ErrorPrint(NartDataHeader,"|er|address|value|");
									}
									ErrorPrint(NartData,buffer);
								}
								else
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"%x,",value_4);
									if(nc>0)
									{
										lc+=nc;
									}
								}
								done++;
							}
						}
					}
					else
					{
						if(_ResponseStyle!=ResponseStyleSimple)
						{
							nc=SformatOutput(buffer,MBUFFER-1,"bad address %s",CommandParameterValue(ip,it));
							buffer[MBUFFER-1]=0;
							SendError(client,buffer);
						}
						else
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
							if(nc>0)
							{
								lc+=nc;
							}
						}
						error++;
					}
				}
			}							
			else
			{
				if(_ResponseStyle!=ResponseStyleSimple)
				{
					nc=SformatOutput(buffer,MBUFFER-1,"unknown parameter %s",name);
					buffer[MBUFFER-1]=0;
					SendError(client,buffer);
				}
				error++;
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(buffer,MBUFFER-1,"no valid address");
				buffer[MBUFFER-1]=0;
				SendError(client,buffer);
				error++;
			}
		}
		else
		{
			ebuffer[MBUFFER-1]=0;
			ErrorPrint(NartOther,ebuffer);
		}
	}
	if(_ResponseStyle!=ResponseStyleSimple)
	{
		SendDone(client);
	}
}


void EepromWriteCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address;
	char *name;
	char buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int bad;
	unsigned char value_1; // 1 byte value
	unsigned int value_4; // 4 byte value
	int s,size=2; //default 2 bytes for eprom read/write value
	//
	// check if card is loaded
	//
	if(!CardValidReset())
	{
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			ErrorPrint(CardNoLoadOrReset);
		}
	}
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		address= -1;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"s")||Smatch(name,"size"))
			{
				ngot=SformatInput(CommandParameterValue(ip,0)," %d ",&size);
				if(CommandParameterValueMany(ip)!=1 || ngot!=1 || (!((size==1)||(size==2)||(size==4))))
				{
					if(_ResponseStyle==ResponseStyleVerbose)
					{
						SformatOutput(buffer,MBUFFER-1,"bad value for parameter size");
						buffer[MBUFFER-1]=0;
						SendError(client,buffer);
					}
					size=2;
				}
			}
			else if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&address);
					if(ngot!=1)
					{
						address= -1;// address zero present in Eeprom
					}
				}
			}
			else if(Smatch(name,"v")||Smatch(name,"value"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&value_4);
					if(ngot==1 && address>=0)
					{
						bad=0;
						for(s=0;s<size;s++)
						{
							value_1=(unsigned char)(value_4>>(s*8));
							bad|=DeviceEepromWrite(address+(it*size)+s,&value_1,1);//writes 8 bits at a time..
						}

						if(bad)
						{
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								nc=SformatOutput(buffer,MBUFFER-1,"bad write to eeprom address %x",address+it);
								buffer[MBUFFER-1]=0;
								SendError(client,buffer);
							}
							error++;
						}
						else
						{
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								SformatOutput(buffer,MBUFFER-1,"|ew|%04x|%02x|",address+it,value_4);
								buffer[MBUFFER-1]=0;
								if(done==0)
								{
									ErrorPrint(NartDataHeader,"|ew|address|value|");
								}
								ErrorPrint(NartData,buffer);
							}
							done++;
						}
					}
					else
					{
						if(_ResponseStyle==ResponseStyleVerbose)
						{
							nc=SformatOutput(buffer,MBUFFER-1,"bad address %s",CommandParameterValue(ip,it));
							buffer[MBUFFER-1]=0;
							SendError(client,buffer);
						}
						error++;
					}
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(buffer,MBUFFER-1,"no valid address or no valid value");
				buffer[MBUFFER-1]=0;
				SendError(client,buffer);		
				error++;
			}
		}
	}
	if(_ResponseStyle==ResponseStyleVerbose)
	{
		SendDone(client);
	}
}


void OtpReadCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address, low, high;
	int s;
	unsigned char val[4];
	unsigned char *value_1; // 1 byte value
	unsigned short *value_2; // 2 byte value
	unsigned int *value_4; // 2 byte value
	int size=2; //default 2 bytes for eprom read/write value
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int bad;
	//
	// if there's no card loaded, return error
	//
    if(CardCheckAndLoad(client)!=0)
    {
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			ErrorPrint(CardNoneLoaded);
		}
    }
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		low=0;
		high=0;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"s")||Smatch(name,"size"))
			{
				ngot=CommandParameterValueMany(ip);
				if(ngot=!1){
					SformatOutput(buffer,MBUFFER-1,"or: can not take more than one 'size'paramters; goes for default size 2");
					ErrorPrint(NartData,buffer);
					size=2;
				}else{
				
					ngot=SformatInput(CommandParameterValue(ip,0)," %x ",&size);
					if(ngot==1){
						if(!((size==1)||(size==2)||(size==4))){
							SformatOutput(buffer,MBUFFER-1,"or::: default size 2");
							ErrorPrint(NartData,buffer);
							size=2;
						}
					}else{
						SformatOutput(buffer,MBUFFER-1,"or: default size 2");
						ErrorPrint(NartData,buffer);
						size=2;
					}
				}

			}
			if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x : %x ",&low,&high);
					if(ngot>=1)
					{
						if(ngot<2)
						{
							high=low;
						}
						for(address=low; address<=high; address+=size)
						{
	//		                bad=MyMemoryRead(address,&value,sizeof(unsigned int));
							bad=0;

							for(s=0;s<size;s++){
								bad|=DeviceOtpRead(address+s,&val[s],1); // reads 8 bits
							}
							value_4=(unsigned int *)val;
							value_2=(unsigned short *)val;
							value_1=(unsigned char *)val;

							if(bad)
							{
								if(_ResponseStyle!=ResponseStyleSimple)
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad Otp read from address %x, ",address);
									if(nc>0)
									{
										lc+=nc;
									}
								}
								else
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
									if(nc>0)
									{
										lc+=nc;
									}
								}
								error++;
							}
							else
							{
								if(_ResponseStyle!=ResponseStyleSimple)
								{
									
									if(size==1){
										SformatOutput(buffer,MBUFFER-1,"|or|%04x|%02x|",address,*value_1);
									}
									if(size==2){
										SformatOutput(buffer,MBUFFER-1,"|or|%04x|%04x|",address,*value_2);
									}
									if(size==4){
										SformatOutput(buffer,MBUFFER-1,"|or|%04x|%08x|",address,*value_4);
									}
									if(done==0)
									{
										ErrorPrint(NartDataHeader,"|or|address|value|");
									}
									ErrorPrint(NartData,buffer);
								}
								else
								{
									nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"%x,",*value_4);
									if(nc>0)
									{
										lc+=nc;
									}
								}
								done++;
							}
						}
					}
					else
					{
						if(_ResponseStyle!=ResponseStyleSimple)
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad address %s, ",CommandParameterValue(ip,it));
							if(nc>0)
							{
								lc+=nc;
							}
						}
						else
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"ERROR,");
							if(nc>0)
							{
								lc+=nc;
							}
						}
						error++;
					}
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle!=ResponseStyleSimple)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid address");
				if(nc>0)
				{
					lc+=nc;
				}
				error++;
			}
			if(error>0)
			{
				SendError(client,ebuffer);
			}
		}
		else
		{
			ErrorPrint(NartOther,ebuffer);
		}
	}
	if(_ResponseStyle!=ResponseStyleSimple)
	{
		SendDone(client);
	}
}


void OtpWriteCommand(int client)
{
	int ngot;
	int np;
	int ip, it;
	int address;
	char *name;
	char ebuffer[MBUFFER],buffer[MBUFFER];
	int error;
	int done;
	int lc, nc;
	int bad;
	unsigned char value_1; // 1 byte value
	unsigned int value_4; // 4 byte value
	int s,size=2; //default 2 bytes for eprom read/write value
	//
	// check if card is loaded
	//
	if(!CardValidReset())
	{
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			ErrorPrint(CardNoLoadOrReset);
		}
	}
	else
	{
		//
		// prepare beginning of error message in case we need to use it
		//
		lc=0;
		error=0;
		done=0;
		//
		//parse arguments and do it
		//
		address= -1;
		np=CommandParameterMany();
		for(ip=0; ip<np; ip++)
		{
			name=CommandParameterName(ip);
			if(Smatch(name,"s")||Smatch(name,"size"))
			{
				ngot=CommandParameterValueMany(ip);
				if(ngot=!1){
					SformatOutput(buffer,MBUFFER-1,"ow: can not take more than one 'size'paramters; goes for default size 2");
					ErrorPrint(NartData,buffer);
					size=2;
				}else{
				
					ngot=SformatInput(CommandParameterValue(ip,0)," %x ",&size);
					if(ngot==1){
						if(!((size==1)||(size==2)||(size==4))){
							SformatOutput(buffer,MBUFFER-1,"ow::: default size 2");
							ErrorPrint(NartData,buffer);
							size=2;
						}
					}else{
						SformatOutput(buffer,MBUFFER-1,"ow: default size 2");
						ErrorPrint(NartData,buffer);
						size=2;
					}
				}

			}
			if(Smatch(name,"a")||Smatch(name,"address"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&address);
					if(ngot!=1)
					{
						address=-1;// address zero present in Eeprom
					}
				}
			}
			if(Smatch(name,"v")||Smatch(name,"value"))
			{
				for(it=0; it<CommandParameterValueMany(ip); it++)
				{
					ngot=SformatInput(CommandParameterValue(ip,it)," %x ",&value_4);
					if(ngot==1 && address>=0)
					{
						bad=0;
						for(s=0;s<size;s++){
							value_1=(unsigned char)(value_4>>(s*8));
							bad|=DeviceOtpWrite(address+(it*size)+s,&value_1,1);//writes 8 bits at a time..
						}

						if(bad)
						{
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad Otp write to address %x, ",address+it);
								if(nc>0)
								{
									lc+=nc;
								}
							}
							error++;
						}
						else
						{
						
							if(_ResponseStyle==ResponseStyleVerbose)
							{
								SformatOutput(buffer,MBUFFER-1,"|ow|%04x|%02x|",address+it,value_4);
								if(done==0)
								{
									ErrorPrint(NartDataHeader,"|ow|address|value|");
								}
								ErrorPrint(NartData,buffer);
							}
							done++;
						}
					}
					else
					{
						if(_ResponseStyle==ResponseStyleVerbose)
						{
							nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"bad address %s, ",CommandParameterValue(ip,it));
							if(nc>0)
							{
								lc+=nc;
							}
						}
						error++;
					}
				}
			}
		}
		//
		// send DONE or ERROR
		//
		if(_ResponseStyle==ResponseStyleVerbose)
		{
			if(done==0 && error==0)
			{
				nc=SformatOutput(&ebuffer[lc],MBUFFER-lc-1,"no valid address or no valid value");
				if(nc>0)
				{
					lc+=nc;
				}
				error++;
			}
			if(error>0)
			{
				SendError(client,ebuffer);
			}
		}
	}
	if(_ResponseStyle==ResponseStyleVerbose)
	{
		SendDone(client);
	}
}

