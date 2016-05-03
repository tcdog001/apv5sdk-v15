
#include <stdio.h>
#include <stdlib.h>

#include "smatch.h"
#include "UserPrint.h"
#include "ErrorPrint.h"
#include "ParseError.h"
#include "NewArt.h"

#define MBUFFER 1024


int main(int narg, char *arg[]) 
{
	int iarg;
	int console;
    int instance=0;
	FILE *lf;
    int port;
	char startfile[MBUFFER];
    //
	// temporarily turn on console output while we parse the command line arguments
	//
	UserPrintConsole(1);
	//
	// process command line options
	//
	SformatOutput(startfile,MBUFFER-1,"%s","nart.art");
	startfile[MBUFFER-1]=0;
    console=0;
    port = -1;
	for(iarg=1; iarg<narg; iarg++)
	{
        if(Smatch(arg[iarg],"-port"))
        {
            if(iarg+1<narg)
            {
                SformatInput(arg[iarg+1]," %d ",&port);
                iarg++;
            }
        }
        else if(Smatch(arg[iarg],"-console"))
		{
			console=1;
		}
		else if(Smatch(arg[iarg],"-instance"))
		{
			if(iarg<narg-1)
			{
				iarg++;
				SformatInput(arg[iarg]," %d ",&instance); 
			}
			else
			{
				ErrorPrint(ParseBadValue,"NULL","-instance");
			}
		}
		else if(Smatch(arg[iarg],"-start"))
		{
			if(iarg<narg-1)
			{
				iarg++;
				SformatOutput(startfile,MBUFFER-1,"%s",arg[iarg]);
				startfile[MBUFFER-1]=0;
			}
			else
			{
				ErrorPrint(ParseBadValue,"NULL","-start");
			}
		}
		else if(Smatch(arg[iarg],"-log"))
		{
			if(iarg<narg-1)
			{
				iarg++;
				lf=fopen(arg[iarg],"a+");
				if(lf==0)
				{
					UserPrint("Can't open log file %s.\n",arg[iarg]);
				}
				else
				{
					UserPrintFile(lf);
				}
			}
			else
			{
				ErrorPrint(ParseBadValue,"NULL","-log");
			}
		}
		else if(Smatch(arg[iarg],"-help"))
		{
			ErrorPrint(ParseHelp,"-console");
			ErrorPrint(ParseHelp,"-log [log file name]");
			ErrorPrint(ParseHelp,"-port [port number]");
			ErrorPrint(ParseHelp,"-instance [device index]");
			ErrorPrint(ParseHelp,"-start [startup command file]");
			exit(0);
		}
		else
		{
			ErrorPrint(ParseBadParameter,arg[iarg]);
		}
	}

	UserPrintConsole(console);
    if(instance == 0)
	{
		if(port<0)
		{
			port=2390;
		} 
	}
    else if(instance == 1)
	{
		if(port<0)
		{
#ifndef __APPLE__	
			port=2391;
#else
			port=2390;
#endif			
		}
    }
    else
    {
#ifndef __APPLE__	
        printf("Only instance 0 and 1 are supported\n");
		exit(1);
#else
		if(port<0)
		{
			port=2390;
		} 	
#endif	
    }
    NewArt(instance,port,startfile);
	exit(0);
}


