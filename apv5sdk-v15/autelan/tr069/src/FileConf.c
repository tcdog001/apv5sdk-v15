
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: FileConf.c 
* description:  implementation for file functions for TR-69 client application  
* 
*
* 
************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include "StubH.h"

#define TR69_CONFIG_FILE          "./tr-069.conf"
#define TR69_CONFIG_FACTORY_FILE  "/var/tr-069/tr-069.factory.conf"
#define TR69_CONFIG_TMP_FILE      "/tmp/tr-069.conf"
#define COMMANDKEY_FILE           "/var/tr-069/commandkey.conf"
#define BUFFERSIZE 256

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************/
/* writeStringInFile                          */
/* write string in a file                     */
/**********************************************/
int writeStringInFile(FILE *_fd, char *_szName, char *_szValue)
{
int nRet = -1;
  // copy line in file
  nRet = fputs(_szName, _fd);
  if (nRet != EOF) nRet += fputs("=", _fd);
  if (nRet != EOF) nRet += fputs(_szValue, _fd);
  if (nRet != EOF) nRet += fputs("\n", _fd);
  return nRet;
}

/**********************************************/
/* SetConfigValue                             */
/* search and replace value in a file         */
/* or add value if not exists                 */
/* return -1 if error                         */
/* else length                                */
/**********************************************/
int SetConfigValue(FILE *_fdSrc, FILE *_fdDest, char *_szName, char *_szValue)
{
int nRet = -1;
char szline[BUFFERSIZE];
char *ptrC;
int bFound = 0;

// if Descriptor of Source File is null, add value
  if ((_fdSrc == NULL) && (_fdDest != NULL))
  {
    return writeStringInFile(_fdDest, _szName, _szValue);
  }

// search _szName in line read from file
  if ((_fdSrc != NULL) && (_fdDest != NULL))
  {
    ptrC = szline; // set to ptrC any value different to NULL
    while (ptrC != NULL)
    {
      ptrC = fgets(szline, BUFFERSIZE, _fdSrc);
      if (ptrC != NULL)
      {
        if (strncmp(szline, _szName, strlen(_szName)) == 0)
        {
  #ifdef SOAP_DEBUG
          printf("........_szName found in line : %s\n", szline);
  #endif
          nRet = writeStringInFile(_fdDest, _szName, _szValue);
          bFound = 1;
        }
        else
        {
          // copy source line in destination file
          nRet = fputs(szline, _fdDest);
        }
      }
    }

    // if string not found in file, add it
    if (!bFound) 
    {
      nRet = writeStringInFile(_fdDest, _szName, _szValue);
    }
  }

  return nRet;
}


/**********************************************/
/* ChangeConfFileValue                        */
/* replace config file value with new one     */
/* return -1 if error                         */
/**********************************************/
int ChangeConfFileValue(char *_szName, char *_szValue)
{
// config file values
FILE *fdConfigSrc = NULL;
FILE *fdConfigDest = NULL;
char szSystemCmd[BUFFERSIZE];
int nRet;

  fdConfigSrc = fopen(TR69_CONFIG_FILE, "r+");
  fdConfigDest = fopen(TR69_CONFIG_TMP_FILE, "w");
  if (fdConfigDest != NULL)
  {
    nRet = SetConfigValue(fdConfigSrc, fdConfigDest, _szName, _szValue);

    if (fdConfigSrc != NULL) fclose(fdConfigSrc);
    fclose(fdConfigDest);

    if (nRet > 0)
    {
      // delete previous config file
      if (fdConfigSrc != NULL) remove(TR69_CONFIG_FILE);
      sprintf(szSystemCmd, "mv %s %s", TR69_CONFIG_TMP_FILE, TR69_CONFIG_FILE);
      // replace previous file with new file
      nRet = system(szSystemCmd);
    }
  }
  return nRet;
}


/**********************************************/
/* GetConfigValue                             */
/* search value in a file and fill string     */
/* return -1 if value not found               */
/*                                            */
/* exemple d'une ligne du fichier tr-069.conf:*/ 
/* Device.DeviceInfo.Manufacturer=Th0ms0n\n   */
/* Device.ManagementServer.InformEnable=1\n   */
/**********************************************/
int GetConfigValue(FILE *_fd, char *_szName, char *_szValue, int *_type)
{
int nRet = -1;
char line[BUFFERSIZE];

  if (_fd != NULL)
  {
    // Return To The Beginning Of The File
    fseek(_fd, 0L, SEEK_SET);

    // search _szName in line read from file
    while(fgets(line, BUFFERSIZE, _fd))
    {
      if (strncmp(line, _szName, strlen(_szName)) == 0)
      {
#ifdef SOAP_DEBUG
        printf("........_szName found in line : %s\n", line);
#endif
        // copy strings without first ('=') and last character ('\n')

        // Particular Value
        if (strncpy(_szValue, &line[strlen(_szName)+1], strlen(line)-(strlen(_szName)+2)) != NULL)
          nRet = strlen(_szValue);
        *_type = SOAP_TYPE_string;
        break;
      }
    }
  }

  return nRet;
}


/*********************************************/
// WriteBootCommandKey 
// Write CommandKey value received from Reboot method
// and to send in next inform
//
/*********************************************/
int WriteBootCommandKey(FILE *_fd, char *_szValue)
{
int nRet = -1;

  if (_fd != NULL)  {
    nRet = fputs(_szValue, _fd);
    if (nRet != EOF) fputs("\n", _fd);
  }
  return nRet;
}


/*********************************************/
// ReadBootCommandKey 
// Read CommandKey value
//
/*********************************************/
int ReadBootCommandKey(FILE *_fd, char *_szValue)
{
int nRet = -1;
char line[BUFFERSIZE];

  if (_fd != NULL)
  {
    fgets(line, BUFFERSIZE, _fd);
    if (strlen(line) > 1)
    {
      // copy without last char ('\n')
      if (strncpy(_szValue, line, strlen(line)-1) != NULL)
        nRet = strlen(_szValue);
    }
  }
  return nRet;
}


#ifdef __cplusplus
}
#endif
