/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: mii_app.h
* description:  implementation for the header file of mii_app.c.
* 
*
* 
************************************************************************************/


#ifndef MII_APP_H
#define MII_APP_H





static unsigned long  g_ulMiiSpeed[]=
{
	0,
	100000000,
	10000000,
};




int get_dev_link(char *ifname);








#endif //MII_APP_H
