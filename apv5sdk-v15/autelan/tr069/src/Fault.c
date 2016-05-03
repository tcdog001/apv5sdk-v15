
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: Fault.c 
* description:  implementation for TR-069 error definition ,Retrieve error string from error code value 
* 
*
* 
************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fault.h"

TR_069_ERROR FaultErrors[] = {
                              {0, ""}, 
                              {9000, "Method not supported"}, 
                              {9001, "Request denied"}, 
                              {9002, "Internal error"}, 
                              {9003, "Invalid arguments"},
                              {9004, "Resources exceeded"},
                              {9005, "Invalid parameter name"},
                              {9006, "Invalid parameter type"},
                              {9007, "Invalid parameter value"},
                              {9008, "Attempt to set a non-writable parameter"},
                              {9009, "Notification request rejected"},
                              {9010, "Download failure"},
							{9012, "File transfer server authentication failure"}, //add by wangjr 
							{9013, "Unsupported protocol for file transfer"}, //add by wangjr 
                              {9015, "Download failure: unable to contact file server"},  //add by wangjr for unable to contact file server
                              {9016, "Download failure: unable to access file"},  //add by wangjr unable to access file
								{9019, "Download failure: file authentication failure"},  //add by wangjr 091203
                              {9011, "Upload failure"},
                              {9012, "File transfer server authentication failure"},
                              {9013, "Unsupported protocol for file transfer"}
                             };

void getTR069ErrorString(unsigned int _FaultCode, char **_szFaultString)
{
unsigned int nbError,i;

  nbError = sizeof(FaultErrors) / sizeof(TR_069_ERROR);
  for (i=0; i < nbError; i++)
  {
    if (_FaultCode == FaultErrors[i].code)
    {
      (*_szFaultString) = (char *)malloc(strlen(FaultErrors[i].szString)+1);
      if ((*_szFaultString) != NULL) strcpy(*_szFaultString, FaultErrors[i].szString);
      break;
    }
  }
}

