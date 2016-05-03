/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	       *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.                *
 *                                                                                         *
 * You should have received a copy of the GNU General Public License along with this       *
 * program; if not, write to the:                                                          *
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,                    *
 * MA  02111-1307, USA.                                                                    *
 *                                                                                         *
 * --------------------------------------------------------------------------------------- *
 * Project:  Capwap                                                                        *
 *                                                                                         *
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *******************************************************************************************/

 
#ifndef __CAPWAP_CWCommon_HEADER__
#define __CAPWAP_CWCommon_HEADER__


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include "wireless_copy.h"
#include "../wtpdlib/common.h"
#include "../wtpdlib/wtpd_mib.h"
#include "../wtpdlib/wtpd_platform.h"
#include "../wtpdlib/wtpd_product.h"

// make sure the types really have the right sizes
#define CW_COMPILE_TIME_ASSERT(name, x)               typedef int CWDummy_ ## name[(x) * 2 - 1]

// if you get a compile error, change types (NOT VALUES!) according to your system
CW_COMPILE_TIME_ASSERT(int_size, sizeof(int) == 4);
CW_COMPILE_TIME_ASSERT(char_size, sizeof(char) == 1);


#define DEFAULT_LOG_SIZE					1000000


typedef enum {
	CW_ENTER_SULKING,
	CW_ENTER_DISCOVERY,
	CW_ENTER_JOIN,
	CW_ENTER_CONFIGURE,
	CW_ENTER_DATA_CHECK,
	CW_ENTER_RUN,
	CW_ENTER_RESET,
	CW_QUIT,
	CW_ENTER_IMAGE_DATA,
	CW_BAK_RUN
} CWStateTransition;

extern const char *CW_CONFIG_FILE;
extern int gCWForceMTU;
extern int gCWRetransmitTimer;
extern int gCWNeighborDeadInterval;
extern int gCWMaxRetransmit; 
extern int gMaxLogFileSize;
extern int gMaxWTPLogFileSize;
extern int gEnabledLog;

#include "CWStevens.h"
#include "config.h"
#include "CWLog.h"
#include "CWErrorHandling.h"

#include "CWRandom.h"
//#include "CWTimer.h"
#include "timerlib.h"
#include "CWThread.h"
#include "CWNetwork.h"
#include "CWList.h"
#include "CWSafeList.h"

#include "CWProtocol.h"
#include "CWSecurity.h"
#include "CWConfigFile.h"

int CWTimevalSubtract(struct timeval *res, const struct timeval *x, const struct timeval *y);
CWBool CWParseSettingsFile();
void CWErrorHandlingInitLib();

extern CWThreadMutex gCreateIDMutex;
#define RADIO_ID_OFFSET 1

#if 0
int sem_timedwait(CWThreadTimedSem *, struct timespec *);
#endif

#endif
