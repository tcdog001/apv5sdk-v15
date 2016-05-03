/*
*                Copyright 2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/
#ifndef AP8X_FWDL_H_
#define AP8X_FWDL_H_

#include <linux/netdevice.h>
#include <linux/delay.h>
#include <linux/version.h>
#include "ap8xLnxIntf.h"

extern int wlFwDownload(struct net_device *);

extern int wlPrepareFwFile(struct net_device *);
#endif /* AP8X_FWDL_H_ */
