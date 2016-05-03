
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: FileConf.h  
* description: the header file  for the function declaration of file functions for TR-69 client application 
* 
*
* 
************************************************************************************/

#define TR69_CONFIG_FILE          "./tr-069.conf"
#define TR69_CONFIG_FACTORY_FILE  "/var/tr-069/tr-069.factory.conf"
#define TR69_CONFIG_TMP_FILE      "/tmp/tr-069.conf"
#define COMMANDKEY_FILE           "/var/tr-069/commandkey.conf"
#define LOCK_FILE                 "/tmp/tr69-lock-file"

#define BUFFERSIZE 256

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************/
/* writeStringInFile                          */
/* write string in a file                     */
/**********************************************/
int writeStringInFile(FILE *_fd, char *_szName, char *_szValue);

/**********************************************/
/* SetConfigValue                             */
/* search and replace value in a file         */
/* or add value if not exists                 */
/* return -1 if error                         */
/* else length                                */
/**********************************************/
int SetConfigValue(FILE *_fdSrc, FILE *_fdDest, char *_szName, char *_szValue);


/**********************************************/
/* ChangeConfFileValue                        */
/* replace config file value with new one     */
/* return -1 if error                         */
/**********************************************/
int ChangeConfFileValue(char *_szName, char *_szValue);


/**********************************************/
/* GetConfigValue                             */
/* search value in a file and fill string     */
/* return -1 if value not found               */
/*                                            */
/* exemple d'une ligne du fichier tr-069.conf:*/ 
/* Device.DeviceInfo.Manufacturer=Th0ms0n\n   */
/* Device.ManagementServer.InformEnable=1\n   */
/**********************************************/
int GetConfigValue(FILE *_fd, char *_szName, char *_szValue, int *_type);


/*********************************************/
/* WriteBootCommandKey                       */
/* Write CommandKey value received from      */
/* Reboot method                             */
/* and to send in next inform                */
/*********************************************/
int WriteBootCommandKey(FILE *_fd, char *_szValue);


/*********************************************/
/* ReadBootCommandKey                        */
/* Read CommandKey value                     */
/*********************************************/
int ReadBootCommandKey(FILE *_fd, char *_szValue);


#ifdef __cplusplus
}
#endif
