/*
*                Copyright 2002-2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

/** include files **/
#include "wllog.h"
#include <stdarg.h>

extern UINT32 debug_level;

void WlLogPrint(UINT32 loglevel,const char *func, const char *format, ... )
{
    unsigned char debugString[1024] = "";
    
    va_list a_start;

    if ((loglevel > debug_level) || (debug_level == MARVEL_DEBUG_DEFAULT))
    {
        return;
    }

    if (format != NULL)
    {
        va_start(a_start, format);
        vsprintf(debugString, format, a_start);
        va_end(a_start);
    }

    switch (loglevel)
    {
        case MARVEL_DEBUG_PANIC:
            printk("PANIC:");
            break;
        case MARVEL_DEBUG_ERROR:
            printk("ERROR:");
            break;
        case MARVEL_DEBUG_WARNING:
            printk("WARNING:");
            break;
        case MARVEL_DEBUG_DEBUG:
            printk("DEBUG:");
            break;
        case MARVEL_DEBUG_ALL:
            printk("ALL:");
            break;
        default:
            break;
    }
    
    if (strlen(debugString) > 0)
    {
        if (debugString[strlen(debugString)-1] == '\n')
            debugString[strlen(debugString)-1] = '\0';
        printk("%s()- %s\n",func, debugString);
    }
}

/*** printk packet ***/
void CWCaptrue(int n ,unsigned char *buffer){
		int t=0;
        printk("packet len = %d\n",n);
		while((n-t)>=16)
		{
			int i;
			printk("[");
			for(i=0;i<16;i++)
				printk("%02x ",buffer[t+i]);
			printk("]\t[");
		/*
            for(i=0;i<16;i++)
			{
				char ch=buffer[t+i];
			}
        */
			printk("]\n");
			t+=16;
		}

		if(n>t)
		{
			int i=t;
			printk("[");
			while(i<n)
				printk("%02x ",buffer[i++]);
			printk("]");
			i=n-t;
			i=16-i;
			while(i--)
				printk("   ");
			printk("\t[");
			i=t;
		/*	while(i<n)
			{
				char ch=buffer[i++];
			}
        */
			printk("]\n");
		}
		printk("\n\n");
}


