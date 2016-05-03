
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: Manager.c  
* description:  implementation for Generic functions used by managers (local manager, component manager ..) 
* 
*
* 
************************************************************************************/
#include <stdio.h>
#include "Manager.h"
#include "const.h"
#include "tools.h"
/***************************************************************************/
/*** subscribe Data model message part                                   ***/
/*** recursive function                                                  ***/
/***************************************************************************/ 
char *CreateDataModelPart(struct soap *_soap, char *_szDataModelSyntax, NOTIFICATION notificationType, ACCESSCONDITION accessConditions,int isExist)//update by wangr 091124
{
char *ptr;
char sep = '.'; // Separator
char szNotification[10];
char szAccessCondition[10];
char szline[ARGSIZE];
char szSubString[ARGSIZE];

  dprintf(LOG_LEVEL3, "CreateDataModelPart, _szDataModelSyntax: %s, sep: %c\n", _szDataModelSyntax, sep);

  memset(szSubString, 0x00, ARGSIZE);
  ptr = strExtractWord(_szDataModelSyntax, sep, szSubString);
  if (ptr != NULL)
  {
    dprintf(LOG_LEVEL3, "CreateDataModelPart, ptr != NULL\n");
    soap_element_begin_out(_soap, szSubString, 0, NULL);
    CreateDataModelPart(_soap, ptr, notificationType, accessConditions,isExist);//update by wangjr 091124
    soap_element_end_out(_soap, szSubString);
  }
  else
  {
    switch (notificationType)
    {
      case ACTIVE:
        sprintf(szNotification, SZ_ACTIVE);
        break;
      case PASSIVE:
        sprintf(szNotification, SZ_PASSIVE);
        break;
      case NONE:
        sprintf(szNotification, SZ_NONE);
        break;
    }

    switch (accessConditions)
    {
      case READ:
        sprintf(szAccessCondition, SZ_READ);
        break;
      case WRITE:
        sprintf(szAccessCondition, SZ_WRITE);
        break;
      case READWRITE:
        sprintf(szAccessCondition, SZ_READWRITE);
        break;
    }

    // last word  update by wangjr 091124
    //sprintf(szline, "<%s %s=\"%s\" %s=\"%s\" />", szSubString, SZ_ACCESSCONDITION, szAccessCondition, SZ_NOTIFICATION, szNotification,SZ_ISEXIST,isExist);

     sprintf(szline, "<%s %s=\"%s\" %s=\"%s\"  %s=\"%d\" />", szSubString, SZ_ACCESSCONDITION, szAccessCondition, SZ_NOTIFICATION, szNotification,SZ_ISEXIST,isExist);//update by wangjr

	 dprintf(3,"\n\nszline=%s\n\n",szline);

    soap_send_raw(_soap, szline, strlen(szline));
  }

  return ptr;
}


/***************************************************************************/
/***                          SUBSCRIBE MESSAGE CREATION                 ***/
/***                                                                     ***/
/***************************************************************************/ 
int CreateSubscribeMessage(STRUCT_MANAGER_INTERNALVALUES row, struct soap *_soap)
{
	char *ptr;
	int nRet = 0;

  dprintf(LOG_LEVEL1, "CreateSubscribeMessage\n");

  // Parse row.szDataModelSyntax
  ptr = CreateDataModelPart(_soap, row.szDataModelSyntax, row.notificationType, row.accessConditions,row.isExist);  //add by wangjr 091124

  dprintf(LOG_LEVEL3, "CreateSubscribeMessage after CreateDataModelPart, ptr : %s\n", ptr);

  return nRet;

}

