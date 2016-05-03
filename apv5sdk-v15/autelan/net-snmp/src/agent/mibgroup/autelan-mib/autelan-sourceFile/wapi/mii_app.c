/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: mii_app.c
* description:  implementation for operations to get information from the device.
* 
*
* 
************************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <time.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#if !defined(__GLIBC__) && !defined(__UC_LIBC__)
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#endif
#include <linux/types.h>

static struct ifreq mii_ifr;

int new_ioctl_nums = 1;
int mdio_read(int skfd, int phy_id, int location)
{
#define SIOCGPARAMS (SIOCDEVPRIVATE+1) 		/* Read operational parameters. */
	
	__u16 *data = (__u16 *)(&mii_ifr.ifr_data);

	data[0] = phy_id;
	data[1] = location;

	if (ioctl(skfd, SIOCGPARAMS, &mii_ifr) < 0) {
		fprintf(stderr, "SIOCGMIIREG on %s failed: %s\n", mii_ifr.ifr_name,
				strerror(errno));
		return -1;
	}
	return data[3];
}

int get_dev_link(char *ifname)
{
#define MII_BMCR		0x00
#define  MII_BMCR_100MBIT	0x2000
#define  MII_BMCR_AN_ENA	0x1000
#define MII_BMSR		0x01
#define  MII_BMSR_LINK_VALID	0x0004

	int skfd = -1;
	__u16 *data = (__u16 *)(&mii_ifr.ifr_data);
	unsigned phy_id = data[0];
	
	int mii_reg;
	__u16 mii_val[32];
	__u16 bmcr, bmsr, lkpar;

	int res = 0;
	
	if (ifname == NULL) {
		ifname = "eth0";
	}

	
	strncpy(mii_ifr.ifr_name, ifname, IFNAMSIZ);

	if ((skfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		perror("socket");
		goto exit;
	}

	for (mii_reg = 0; mii_reg < 8; mii_reg++)
		mii_val[mii_reg] = mdio_read(skfd, phy_id, mii_reg);

	bmcr = mii_val[MII_BMCR]; bmsr = mii_val[MII_BMSR];
	lkpar = mii_val[5];
	if (mii_val[0] == 0xffff  ||  mii_val[1] == 0x0000) {
		goto exit;
	}
	if((bmsr & MII_BMSR_LINK_VALID) == 0) 
	{
		res = 0;
	}
	if(lkpar & 0x00A0)
	{
		if(lkpar & 0x0080)
			res = 1;
		else
			res = 2;
	}
#if 0	
	if(bmcr & MII_BMCR_AN_ENA)
	{
		if(lkpar & 0x0080)
			res = 1;
		else
			res = 2;
	}
#endif			

exit:
	close(skfd);
	return res;
}

