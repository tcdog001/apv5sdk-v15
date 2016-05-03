
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: Fault.h
* description:  implementation for Fault header definition TR-069  fault code
* 
*
* 
************************************************************************************/

#ifndef _FAULT_H
#define _FAULT_H

#include "const.h"

typedef struct {
  unsigned int code;
  char szString[MAX_VALUE];
} TR_069_ERROR;


void getTR069ErrorString(unsigned int _FaultCode, char **_szFaultString);

#endif // _FAULT_H
