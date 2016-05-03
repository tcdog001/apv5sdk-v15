
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: dhcp_snooping_main.h
* description:  the header file of dhcp_snooping command
* 
*
* 
************************************************************************************/
#ifndef _DHCP_SNOOPING_MAIN_H
#define _DHCP_SNOOPING_MAIN_H


#define DHCP_FAIL 0
#define DHCP_OK   1

#define DHCP_SNOOPING_REQUEST_TIMEOUT                 60


typedef struct dhcp_snooping_config {
	char trusted_interface[32];/* trusted interface name string */
	int  trusted_ifindex;
	char option82[256];
}dhcp_snooping_config_t;
#endif /*_DHCP_SNOOPING_MAIN_H*/
