
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: DataModel.c
* description:  implementation for Data model repository - Data model tree using gSoap DOM parser 
*  - Data model tree using gSoap DOM parser
*  - wait for subscription request done by manager(s)
*  - wait for request done by client(s)  
*  - wait for notify done by manager(s) 
************************************************************************************/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "StubH.h"

#include "const.h"
#include "DataModel.h"
#include "messages.h"
#include "TreeFunctions.h"

#include "FileConf.h"


#ifdef __cplusplus
extern "C" {
#endif

// gSoap context objects
struct Namespace *namespaces;
static struct soap_dom_element *gdom_tree = NULL;
static struct soap *gSoapTree;


// subscriber current Id
static short gId =-1;

// table with Id, Type, @IP, Port
static STRUCT_DATAMODEL_ROW gtable[10];
static int gnbSubscriber = 0;

// synchronization
static sem_t S_read, S_response, S_concurrent; // semaphore to lock/unlock reading messages
static short gflagResponse_OK = 0;

// Value kept during thread sequence (Request getValues + response values for instance)
static char gszValue[MAX_VALUE];
static LIST_PARAM_VALUE *gListParamValues=NULL;
static LIST_PARAM_VALUE *gtmpListParamValues=NULL;  //add by wangjr 091124


/***************************************************************************/
/***                                                                     ***/
/***                          PROGRAMME PRINCIPAL                        ***/
/***                                                                     ***/
/*** socket server listening on specific port                            ***/
/*** - wait for connection                                               ***/
/*** - create one thread for each connection                             ***/
/*** ex of execution:                                                    ***/
/*** ./datamodel --file:/var/tr-069/datamodel.conf                       ***/
/***************************************************************************/ 
void sigpipe_handle(int x) 
{ 
  dprintf(LOG_LEVEL1, "SIGPIPE caught\n");
}

void sigint_handle(int x) 
{ 
  dprintf(LOG_LEVEL1, "SIGINT caught\n");
  exit(0);
}

int main(int argc, char *argv[])
{
printf("debug_level: %d\n",DEBUG_LEVEL);
dprintf(LOG_LEVEL1,"level1\n");
dprintf(LOG_LEVEL2,"level2\n");
dprintf(LOG_LEVEL3,"level3\n");
dprintf(LOG_LEVEL4,"level4\n");
// command line arguments
int nArg;
char *ptr;

// config file values
FILE *fdConfig = NULL;
char szFileName[ARGSIZE];
char szConfigValue[BUFFERSIZE];
int nLength;
int nParameterType;
int nPortNumber;

// thread
pthread_t tid;

// soap env
struct soap soap, *tsoap;
SOAP_SOCKET m, s;
struct Namespace empty_namespaces[] = { { NULL } };

if( (argc == 3)&& (strncmp(argv[2],"-d",strlen("-d")) == 0))
{	
    if( daemon(0,0))
	{		
	    dprintf(LOG_LEVEL3, "daemon error\n");		
		return 1 ;	
	} 	
}
  dprintf(LOG_LEVEL1, "\nDATA MODEL REPOSITORY..execution\n\n");
  signal(SIGPIPE, sigpipe_handle);
  signal(SIGINT, sigint_handle);

  // Init Semaphores
  sem_init(&S_read, 0, 1);
  sem_init(&S_concurrent, 0, 1);
  sem_init(&S_response, 0, 0);

  // Command Line Arguments
  for (nArg = 1; nArg < argc; nArg++)
  {
    ptr = argv[nArg];
    if (strncmp(argv[nArg], "--file:", strlen("--file:")) == 0)
    {
      strcpy(szFileName, &ptr[strlen("--file:")]);
      dprintf(LOG_LEVEL1, "..szFileName: %s\n", szFileName);
    }
  }

  // Data model conf values
  fdConfig = fopen(szFileName, "r");
  if (fdConfig != NULL)
  {
    // Search Corresponding Value In Config File
    nLength = GetConfigValue(fdConfig, SERVER_PORT, szConfigValue,  &nParameterType);
    if (nLength > 0)
    {
      nPortNumber = atoi(szConfigValue);
      dprintf(LOG_LEVEL1, "..%s : %d\n", SERVER_PORT, nPortNumber);
    }
    fclose(fdConfig);
  }
  else 
  {
    dprintf(LOG_LEVEL1, "..file %s not found\n", szFileName);
    goto error_label;
  }

  // soap structure initialization
  InitSoapStruct(&soap, empty_namespaces);
  
  // wait for connections
  m = soap_bind(&soap, NULL, nPortNumber, BACKLOG);
  if (!soap_valid_socket(m))
  {
    dprintf(LOG_LEVEL1, "..socket connection error\n");
    exit(1);
  }
  dprintf(LOG_LEVEL1, "..socket connection successful %d\n", m);
  for (;;)
  {
    //this sleep used to help thread worker_thread get cpu time
    sleep(1);
    s = soap_accept(&soap);
    dprintf(LOG_LEVEL1, "datamodel: socket: %d\n", s);
    if (!soap_valid_socket(s))
    {
      dprintf(LOG_LEVEL1, "not valid socket\n");
      if (soap.errnum)
      {
        soap_print_fault(&soap, stderr);
        exit(1);
      }
      dprintf(LOG_LEVEL1, "..server timed out\n"); 
    }
    else
    {
      dprintf(LOG_LEVEL1, "valid socket\n");
      dprintf(LOG_LEVEL1, "\n..socket %d connection from IP %ld.%ld.%ld.%ld\n", s, (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF);
      tsoap = soap_copy(&soap); // make a safe copy
      if (!tsoap) break;
  
      // worker_thread creation
      pthread_create(&tid, NULL, (void*(*)(void*))worker_thread, (void*)tsoap);
    }
  }
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
error_label:
  return 0;
}


/***************************************************************************/
/*** Worker thread                                                       ***/
/***                                                                     ***/
/*** - Read message on socket                                            ***/
/*** - Analyze of message sent :                                         ***/
/***   method (REQUEST, NOTIFY, SUBSCRIBE, RESPONSE), Id, Function,      ***/
/***   Parameter, Type, Port                                             ***/
/***   and XML data model part (for SUBSCRIBE of manager)                ***/
/*** - Make a response                                                   ***/
/***************************************************************************/ 
void *worker_thread(void *soap)
{
struct soap_dom_element dom;

int nSoapError;
short bFoundInTable, bError;

char soap_endpoint[ARGSIZE];

STRUCT_THREAD_DATA threadargs;
STRUCT_MESSAGE message, *dupMessage;
LIST_PARAM_VALUE *list_param_value;
enum methodName eMethodName;
enum hostType eHostType;
enum functionName eFunctionName;
// thread
pthread_t tid;
int ret_sem_wait;

  bFoundInTable = 0;
  bError = 0;

  dprintf(LOG_LEVEL1, "worker_thread %lu\n", (unsigned long)pthread_self());
  dom.soap = soap;
  dprintf(LOG_LEVEL1, "worker_thread %lu..read data from socket\n", (unsigned long)pthread_self());
  nSoapError = soap_begin_recv(dom.soap);
  if (nSoapError != 0)
  {
    dprintf(LOG_LEVEL1, "worker_thread %lu.. nError: %d!\n", (unsigned long)pthread_self(), nSoapError);
    goto endthread_label;
  }

  dprintf(LOG_LEVEL1, "worker_thread %lu..parse XML message\n", (unsigned long)pthread_self());

  do
  {
    if ((ret_sem_wait=sem_wait(&S_read)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "worker_thread %lu, ERROR on sem_wait !! \n", (unsigned long)pthread_self());
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));


  // Extract data from message received and fill STRUCT_MESSAGE variable
  // list of (param,value)
  list_param_value = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
  memset(list_param_value, 0x00, sizeof(LIST_PARAM_VALUE));

  if (ExtractXMLTagsWithMultipleParams(soap, &message, list_param_value) != 0)
  {
    dprintf(LOG_LEVEL1, "worker_thread %lu ExtractXMLTags error\n", (unsigned long)pthread_self());
    goto endthread_label;
  }
  message.ListParamValue = list_param_value;
  dprintf(LOG_LEVEL1, "worker_thread: name: %s\n", message.szName);
  dprintf(LOG_LEVEL1, "worker_thread: type: %s\n", message.szType);
  // Process message to retrieve method name, host type and function name
  dprintf(LOG_LEVEL1, "message:%s\n",message.szFunction);
  if (ProcessMessage(&message, &eMethodName, &eHostType, &eFunctionName) > -1)
  {
    if (eMethodName == SUBSCRIBE) // SUBSCRIBE method
    { // Parsing of XML data model parts, create a DOM branch
      dprintf(LOG_LEVEL1, "worker_thread %lu, Method = SUBSCRIBE\n", (unsigned long)pthread_self());

      // sender (@IP + port)
      sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message.Ip >> 24)&0xFF, (message.Ip >> 16)&0xFF, (message.Ip >> 8)&0xFF, message.Ip&0xFF, message.Port);
      dprintf(LOG_LEVEL1, "worker_thread %lu, soap_endpoint : %s\n", (unsigned long)pthread_self(), soap_endpoint);

      // copy of message and copy of list of param-value
      dupMessage = (STRUCT_MESSAGE*)malloc(sizeof(STRUCT_MESSAGE));
      memcpy(dupMessage, &message, sizeof(STRUCT_MESSAGE));
      dupMessage->ListParamValue = duplicate_queue(message.ListParamValue);

      // do subscribe task (add parameter in DOM tree, add manager id in table...
      if (subscribe_task(&dom, eMethodName, eHostType, dupMessage) == 0)
      {
          dprintf(LOG_LEVEL1, "work_thread: subscribe_task\n");
          bError = 1;
      }
      else CreateResponse(soap_endpoint, gId);
    }

    // find if sender of request with its Id exists in subscriber table
    if (eMethodName != SUBSCRIBE) 
      bFoundInTable = getSubscriber(gtable, gnbSubscriber, message.Id, soap_endpoint, &message.Port);
    else bFoundInTable = 1;

    // SEMAPHORE unlock
    dprintf(LOG_LEVEL3, "worker_thread %lu, unlock S_read\n", (unsigned long)pthread_self());
    sem_post(&S_read);
    
    if (!bFoundInTable)
    {
      dprintf(LOG_LEVEL1, "worker_thread %lu, subscriber not found in table !!\n", (unsigned long)pthread_self());
      goto endthread_label;
    }

    gflagResponse_OK = 0;

    if (eMethodName == REQUEST) // REQUEST method
    {
      dprintf(LOG_LEVEL3, "worker_thread %lu, Method = REQUEST\n", (unsigned long)pthread_self());
      dprintf(LOG_LEVEL3, "worker_thread wait for S_concurrent\n");
      do {
        if ((ret_sem_wait=sem_wait(&S_concurrent)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "worker_thread %lu, ERROR on sem_wait !! \n", (unsigned long)pthread_self());
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));
      dprintf(LOG_LEVEL1, "worker_thread: wait walk through \n");
      // copy of message and copy of list of param-value
      dupMessage = (STRUCT_MESSAGE*)malloc(sizeof(STRUCT_MESSAGE));
      memcpy(dupMessage, &message, sizeof(STRUCT_MESSAGE));
      dupMessage->ListParamValue = duplicate_queue(message.ListParamValue);
      dprintf(LOG_LEVEL1, "worker_thread: copy walk through \n");
      threadargs.soap = soap;
      threadargs.msg = dupMessage;
      dprintf(LOG_LEVEL1, "worker_thread: name: %s\n",dupMessage->szName);
      dprintf(LOG_LEVEL1, "worker_thread: function: %s\n",dupMessage->szFunction);
      dprintf(LOG_LEVEL1, "worker_thread: parameter: %s\n",dupMessage->ListParamValue->szParameter);
      dprintf(LOG_LEVEL1, "worker_thread: value: %s\n",dupMessage->ListParamValue->szValue);
      
      if (eFunctionName==GET_PASSIVEPARAMETERS)
      {
        pthread_create(&tid, NULL, (void*(*)(void*))request_getpassiveparameters_task, (void*)&threadargs);
        (void) pthread_join(tid, NULL);
      }
	  else if (eFunctionName==GET_ACTIVEPARAMETERS)  //add by wangjr 091124
      {
        pthread_create(&tid, NULL, (void*(*)(void*))request_getactiveparameters_task, (void*)&threadargs);

        // wait for end of thread execution
        (void) pthread_join(tid, NULL);
      }
	  else if (eFunctionName==GET_MONITORPARAMETERS)
	  {
	  	dprintf(LOG_LEVEL1, "get parameter names\n");
		pthread_create(&tid, NULL, (void*(*)(void*))request_getmonitorparameter_task, (void*)&threadargs);
        // wait for end of thread execution
        (void) pthread_join(tid, NULL);
	  } 
	  else if (eFunctionName==GET_ALARMPARAMETERS)
	  {
	  	dprintf(LOG_LEVEL1, "get parameter names\n");
		pthread_create(&tid, NULL, (void*(*)(void*))request_getalarmparameter_task, (void*)&threadargs);
        // wait for end of thread execution
        (void) pthread_join(tid, NULL);
	  }
      else if (eFunctionName==GET_IPPARAMETERS)
      {
      }
	  else if (eFunctionName==GET_NAME)
	  {
	  	dprintf(LOG_LEVEL1, "get parameter names\n");
		pthread_create(&tid, NULL, (void*(*)(void*))request_getparameternames_task, (void*)&threadargs);

        // wait for end of thread execution
        (void) pthread_join(tid, NULL);
	  }
	  else if(eFunctionName==DELETEOBJECT)  //add by wangjr 091124
	  { 
		pthread_create(&tid, NULL, (void*(*)(void*))request_deleteobject_task, (void*)&threadargs);
        (void) pthread_join(tid, NULL);// wait for end of thread execution
        
	  }
	  else if(eFunctionName==ADDOBJECT)  //add by wangjr 091124
	  {
		pthread_create(&tid, NULL, (void*(*)(void*))request_addobject_task, (void*)&threadargs);      
        (void) pthread_join(tid, NULL); // wait for end of thread execution
	  }
	  else if(eFunctionName==GET_PARAMETERATTRIBUTES)  //add by wangjr 091124
	  {
	  		
		  dprintf(LOG_LEVEL1, "\n--------------------GET_PARAMETERATTRIBUTES------------\n");
		  pthread_create(&tid, NULL, (void*(*)(void*))request_GetParameterAttributes_task, (void*)&threadargs);		
		  (void) pthread_join(tid, NULL); // wait for end of thread execution

	  }
	  else if(eFunctionName==SET_PARAMETERATTRIBUTES)  //add by wangjr 091124
	  {
	  		
		  dprintf(LOG_LEVEL1, "\n--------------------SET_PARAMETERATTRIBUTES------------\n");
		  pthread_create(&tid, NULL, (void*(*)(void*))request_SetParameterAttributes_task, (void*)&threadargs);		
		  (void) pthread_join(tid, NULL); // wait for end of thread execution

	  }
      else if (message.ListParamValue && strcmp(message.ListParamValue->szParameter, "") != 0)
      {
        dprintf(LOG_LEVEL1, "worker_thread: request task\n");
        pthread_create(&tid, NULL, (void*(*)(void*))request_task, (void*)&threadargs);

        // wait for end of thread execution
        dprintf(LOG_LEVEL1, "worker_thread: thread jion\n");
        (void) pthread_join(tid, NULL);
      }

      // SEMAPHORE unlock S_concurrent
      dprintf(LOG_LEVEL3, "worker_thread %lu, sem_post S_concurrent\n", (unsigned long)pthread_self());
      sem_post(&S_concurrent);
    }
    else if (eMethodName == NOTIFY) // NOTIFY method
    {
      dprintf(LOG_LEVEL3, "worker_thread %lu, Method = NOTIFY\n", (unsigned long)pthread_self());

      dprintf(LOG_LEVEL1, "worker_thread wait for S_concurrent\n");

      do
      {
        if ((ret_sem_wait=sem_wait(&S_concurrent)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "worker_thread %lu, ERROR on sem_wait !! \n", (unsigned long)pthread_self());
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));

      // arguments of thread 
      // copy of message and copy of list of param-value
      dupMessage = (STRUCT_MESSAGE*)malloc(sizeof(STRUCT_MESSAGE));
      memcpy(dupMessage, &message, sizeof(STRUCT_MESSAGE));
      dupMessage->ListParamValue = duplicate_queue(message.ListParamValue);

      threadargs.soap = soap;
      threadargs.msg = dupMessage;
      pthread_create(&tid, NULL, (void*(*)(void*))notify_task, (void*)&threadargs);

      // wait for end of thread execution
      (void) pthread_join(tid, NULL);

      // SEMAPHORE unlock S_concurrent
      dprintf(LOG_LEVEL3, "worker_thread %lu, sem_post S_concurrent\n", (unsigned long)pthread_self());
      sem_post(&S_concurrent);
    }
    else if (eMethodName == RESPONSE) // RESPONSE OK
    { // response from a previous request
      dprintf(LOG_LEVEL3, "worker_thread %lu, Method = RESPONSE\n", (unsigned long)pthread_self());
      gflagResponse_OK = 1;
      if (message.ListParamValue != NULL) // For response with value
      { // copy first value of list
        strcpy(gszValue, message.ListParamValue->szValue);
      }

      // SEMAPHORE unlock S_response
      dprintf(LOG_LEVEL3, "worker_thread %lu, sem_post S_response\n", (unsigned long)pthread_self());
      sem_post(&S_response);
    }
    else if (eMethodName == ERROR) // ERROR RESPONSE
    { // error corresponding to a previous request
      dprintf(LOG_LEVEL1, "worker_thread %lu, Method = ERROR\n", (unsigned long)pthread_self());
      gflagResponse_OK = 0;

      // SEMAPHORE unlock S_response
      dprintf(LOG_LEVEL3, "worker_thread %lu, sem_post S_response\n", (unsigned long)pthread_self());
      sem_post(&S_response);
    }  
  }

  // error in sequence, send error message to sender of request
  if (bError || (eMethodName == UNKNOWN))
  {
    // @IP + port
    sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message.Ip >> 24)&0xFF, (message.Ip >> 16)&0xFF, (message.Ip >> 8)&0xFF, message.Ip&0xFF, message.Port);
    
    CreateErrorResponse(soap_endpoint, gId);
    
    dprintf(LOG_LEVEL1, "!!!\n!!! worker_thread %lu.. soap_endpoint: %s, nSoapError: %d !!!\n!!!\n", (unsigned long)pthread_self(), soap_endpoint, nSoapError);
  }

endthread_label:
  nSoapError = soap_end_recv(soap);
  if (nSoapError != 0) dprintf(LOG_LEVEL1, "worker_thread %lu.. nError: %d!\n", (unsigned long)pthread_self(), nSoapError);
  soap_closesock(soap);

  dprintf(LOG_LEVEL3, "worker_thread %lu, pthread_detach\n", (unsigned long)pthread_self());
  pthread_detach(pthread_self());

  if (list_param_value != NULL) free_queue(list_param_value); // free list of param-value

  soap_destroy((struct soap*)soap); // dealloc C++ data
  soap_end((struct soap*)soap); // dealloc data and clean up
  soap_done((struct soap*)soap); // detach soap struct
  free(soap); // !!

  dprintf(LOG_LEVEL2, "worker_thread %lu, before return !\n", (unsigned long)pthread_self());
  return NULL;
}


/***************************************************************************/
/*** notify task                                                         ***/
/***                                                                     ***/
/*** - analyze request                                                   ***/
/*** - search clients in table                                           ***/
/*** - for each client                                                   ***/
/***     send notify message                                             ***/
/***     wait for response                                               ***/
/*** - send response to manager who send notify message                  ***/
/***************************************************************************/
void *notify_task(void *_threadArg)
{
STRUCT_THREAD_DATA *args;
STRUCT_MESSAGE *message;
struct soap *soap;

char soap_endpoint[ARGSIZE];
TR69_ERROR error = FUNCTION_ERROR;
int i;
int ret_sem_wait;

  dprintf(LOG_LEVEL1, "notify_task\n");
  args = (STRUCT_THREAD_DATA *) _threadArg;
  soap = args->soap;
  message = args->msg;

  print(message->ListParamValue);

  // search clients in global table
  for (i = 0; i < gnbSubscriber; i++)
  {
    if (gtable[i].Type == TYPE_CLIENT)
    {
      // send notify message to client
      sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (gtable[i].Ip >> 24)&0xFF, (gtable[i].Ip >> 16)&0xFF, (gtable[i].Ip >> 8)&0xFF, gtable[i].Ip&0xFF, gtable[i].Port);

      dprintf(LOG_LEVEL3, "notify_task, message->ListParamValue: %p\n", message->ListParamValue);
      //dprintf(LOG_LEVEL1, "notify_task, message->ListParamValue->szParameter: %s", message->ListParamValue->szParameter);

      if (message->ListParamValue)
      CreateNotify(soap_endpoint, gtable[i].Id, message->ListParamValue->szParameter, message->ListParamValue->szValue);

      // wait for response
      do {
        if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));

      error = OK;
    }
  }

  // send response to manager who sent the notify message
  sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);
  if ((error == OK) && (gflagResponse_OK == 1))
  {
    CreateResponse(soap_endpoint, message->Id);
  }
  else
  {
    dprintf(LOG_LEVEL1, "notify_task, %s\n", getErrorString(error));
    CreateErrorResponse(soap_endpoint, message->Id);
  }

  dprintf(LOG_LEVEL3, "notify_task, pthread_detach\n");
  pthread_detach(pthread_self());

  // free copy of message param-value list
  if (message->ListParamValue != NULL)
  {
    free_queue(message->ListParamValue);
    message->ListParamValue = NULL;
  }

  // free message
  if (message != NULL) free(message);

  dprintf(LOG_LEVEL2, "notify_task, before return !\n");
  return NULL;
}

/**************************************************************************/
/***
/***wangjr  string to other   091124
/****
/**************************************************************************/
char* sting_to_singlestring(char *src,char *dest)
{
    int i=0;
	for(i=0;i<=strlen(src);i++)
	{
	   if(src[i]>='0' && src[i]<='9')
	   {
	   	    dest[i]='1';
	   }
	   else
	   {
	        dest[i]=src[i];
	   }
	}
		
	return dest;
}


/***************************************************************************/
/*** request task                                                        ***/
/***                                                                     ***/
/*** - Analyze request                                                   ***/
/*** - search parameter in DOM tree for Id of manager                    ***/
/*** - send the request to corresponding component manager               ***/
/***************************************************************************/
void *request_task(void *_threadArg)
{
short bFound=0;
STRUCT_THREAD_DATA *args;
STRUCT_MESSAGE *message;
struct soap *soap;
struct soap_dom_element *e=NULL;
struct soap_dom_attribute *attr;
char soap_endpoint[ARGSIZE];
short IdManager = -1;
ACCESSCONDITION accessCond;
TR69_ERROR error = FUNCTION_ERROR;
char szParameterName[ARGSIZE];
char szOrigParameterName[ARGSIZE];
char szSingleParameterName[ARGSIZE];//add by wangjr 091124
char szSubString[ARGSIZE];
char *ptr;
int NumberOfEntries;
int ret_sem_wait;

  dprintf(LOG_LEVEL1, "request_task\n");
  args = (STRUCT_THREAD_DATA *) _threadArg;
  soap = args->soap;
  message = args->msg;
  //dprintf(LOG_LEVEL3, "xxxxxxxxxxxxxxxxxxxx: %s\n", message->ListParamValue->szParameter);
  // search parameter in DOM tree
  if (message->ListParamValue != NULL)
  {
    sprintf(szParameterName, "%s", message->ListParamValue->szParameter);

    // ! specific handling of NumberOfEntries !!
    // if szParameterName ends with NumberOfEntries, retrieve NumberOfEntries attribute in its object
    if (((ptr = strstr(szParameterName, SZ_NUMBEROFENTRIES)) != NULL) && 0)
    {
      error = VARIABLE_ERROR;
      memset(szSubString, 0x00, ARGSIZE);
      if (strncpy(szSubString, szParameterName, (unsigned long)(ptr - &szParameterName[0])))
      {
        dprintf(LOG_LEVEL3, "request_task, szSubString: %s\n", szSubString);

        // Find parameter object
        e = search_entire_element(gdom_tree, szSubString);

        // retrieve "NumberOfEntries" attribute
        if (e != NULL) {
          attr = search_attribute(e, SZ_NUMBEROFENTRIES); // retrieve "NumberOfEntries" in attribute list
          if (attr == NULL) {
            error = PARAMETER_ERROR;
          }
          else {
            NumberOfEntries = atoi(attr->data);
            dprintf(LOG_LEVEL3, "NumberOfEntries: %d\n", NumberOfEntries);
            gflagResponse_OK = 1;
            sprintf(gszValue, "%d", NumberOfEntries);
            error = OK;
          }
        }
      }
      goto label_answers;
    }
    else {
      e = search_entire_element(gdom_tree, szParameterName);
      //dprintf(LOG_LEVEL1, "worker_thread: entire: %x\n",e);
      //dprintf(LOG_LEVEL1, "worker_thread: gdom_tree: %x\n",gdom_tree);
    }
  }

  if (e != NULL)
  {
    attr = search_attribute(e, "Id"); // retrieve Id of component manager in attribute list
    if (attr == NULL) {
      error = ID_ERROR;
    }
    else
    {
      IdManager = atoi(attr->data);
      attr = search_attribute(e, SZ_ACCESSCONDITION); // retrieve Access conditions in attribute list
      if (attr == NULL) {
        error = DOM_ERROR;
      }
      else 
      {
///////////////////////////update by wangjr 091124///////////////////////////////////////////
         attr = search_attribute(e, SZ_ISEXIST);
		 if(attr == NULL)
		 {
		   	  error = DOM_ERROR;
		 }
		 else
		 {
			if(atoi(attr->data) == 0)
		   {
		   		error = DOM_ERROR;
		   }
		   else
		   {	attr = search_attribute(e, SZ_ACCESSCONDITION); // retrieve Access conditions in attribute list
      			if (attr == NULL) {
        				error = DOM_ERROR;
      			} 
				else
				{
        dprintf(LOG_LEVEL3, "attr->data: %s\n", attr->data);
        accessCond = getAccessCondition(attr->data);
        dprintf(LOG_LEVEL3, "accessCond: %d\n", accessCond);
          		}
///////////////////////////update by wangjr 091124///////////////////////////////////////////
        // if path of parameter contains instance object, replace path string with original INSTANCE
        // ex.: Device.LANDevice.1.LANHostConfigManagement ==> Device.LANDevice.INSTANCE1.LANHostConfigManagement
        memset(szOrigParameterName, 0x00, ARGSIZE);
        e = str_parameter_path_original(gdom_tree, szParameterName, szOrigParameterName);
        if (e != NULL) {
          strcpy(szParameterName, szOrigParameterName);
          dprintf(LOG_LEVEL3, "ORIGINAL STRING !!\n\n");
          dprintf(LOG_LEVEL3, "szParameterName: %s\n", szParameterName);
        }
        error = OK;
      }
    }
  }
    }
  }
  else error = VARIABLE_ERROR;

  // retrieve @IP:port of component manager
  if (error == OK)
  {
    bFound = getSubscriber(gtable, gnbSubscriber, IdManager, soap_endpoint, NULL);
    if (!bFound)
      error = ID_ERROR;

    // test access conditions and functions
    switch(accessCond)
    {
      case READ:
        if (strcmp(message->szFunction, CONST_FUNCTIONSETVALUE) == 0) 
          error = ACCESS_ERROR;
        break;
      case WRITE:
        if (strcmp(message->szFunction, CONST_FUNCTIONGETVALUE) == 0)
          error = ACCESS_ERROR;
        break;
      default:
        break;
    }
  }

  if (error==OK)
  {
    // send request to component manager
    CreateRequest(soap_endpoint, atoi(attr->data), message->szFunction, szParameterName, message->ListParamValue->szValue);

    // SEMAPHORE wait for response message
    dprintf(LOG_LEVEL3, "request_task.. wait for S_response\n");
    do {
      if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
          dprintf(LOG_LEVEL1, "request_task, ERROR on sem_wait !! \n");
    }
    while ((ret_sem_wait != 0) && (errno == EINTR));
  }

label_answers:
  // send response or error to manager who sent the notify message
  sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);
  if (error != OK) //update by wangjr  091124
  {
    dprintf(LOG_LEVEL1, "request_task, CreateErrorResponse\n");
    // send error to client sender
    CreateErrorResponse(soap_endpoint, message->Id);
  }
  else
  {
    dprintf(LOG_LEVEL3, "request_task, CreateResponse\n");

    // send response with values to client sender
    CreateResponseWithValues(soap_endpoint, message->Id, gszValue);
  }
  FILE *fp;
  dprintf(4,"message:%s\n", message->szFunction);
  if((strcmp(message->szFunction, CONST_FUNCTIONSETVALUE) == 0) && (fp=fopen("/jffs/alarmflag","r")))
  {
  	dprintf(LOG_LEVEL3, "request_task, %d\n", __LINE__);
  	fclose(fp);
	dprintf(LOG_LEVEL3, "request_task, %d\n", __LINE__);
	char szSetTemp[256];  
	memset(szSetTemp, 0x00, 256);
	dprintf(LOG_LEVEL3, "request_task, %d\n", __LINE__);
  	fp = fopen("/jffs/alarm/alarmpara1","r");
	dprintf(LOG_LEVEL3, "request_task, %d\n", __LINE__);
	fgets(szSetTemp, 256, fp);
	dprintf(LOG_LEVEL3, "request_task, %d\n", __LINE__);
	szSetTemp[strlen(szSetTemp)-1] = '\0';
	dprintf(LOG_LEVEL3, "request_task, %d\n", __LINE__);
	fclose(fp);
	dprintf(LOG_LEVEL3, "request_task, %d\n", __LINE__);
	dprintf(LOG_LEVEL3, "request_task, %s\n", szSetTemp);
	dprintf(LOG_LEVEL3, "request_task, %s\n", message->ListParamValue->szParameter);
  	if(strncmp(message->ListParamValue->szParameter, szSetTemp, strlen(message->ListParamValue->szParameter)) == 0){
		char min[8];
		char max[8];
		fp = fopen("/jffs/alarm/alarmmin1","r");
		fgets(min, 256, fp);
		min[strlen(min)-1] = '\0';
		fclose(fp);
		fp = fopen("/jffs/alarm/alarmmax1","r");
		fgets(max, 256, fp);
		max[strlen(max)-1] = '\0';
		fclose(fp);

		if((strncmp(max, message->ListParamValue->szValue, strlen(message->ListParamValue->szValue))<0 ) || (strncmp(message->ListParamValue->szValue, min, strlen(message->ListParamValue->szValue))<0))
		{
			system("touch /jffs/alarm/bAlarm1");
		}
  	}
  }
  dprintf(LOG_LEVEL3, "request_task, pthread_detach\n");
  pthread_detach(pthread_self());

  // free copy of message param-value list
  if (message->ListParamValue != NULL)
  {
    free_queue(message->ListParamValue);
    message->ListParamValue = NULL;
  }

  // free message
  if (message != NULL) free(message);

  dprintf(LOG_LEVEL2, "request_task, before return !\n");

  return NULL;
}


/***************************************************************************/
/*** request getparameternames task									 ***/
/***																	 ***/
/*** - search in DOM tree for the right path parameter names			 ***/
/*** - for all parameter found											 ***/
/*** - construct a global list with (parameter, value)					 ***/
/***************************************************************************/

TR69_ERROR get_branches_name(struct soap_dom_element *_node, char *_xpath, LIST_PARAM_VALUE ** _res_result_list, int _getFlag)
{
    dprintf(LOG_LEVEL1, "level:%d\n",_getFlag);
	dprintf(LOG_LEVEL1, "level:%s\n",_xpath);
	struct soap_dom_element *itera=NULL;
	struct soap_dom_attribute *attr=NULL;
	LIST_PARAM_VALUE *iter_list=NULL;
	char szParameterName[MAX_PARAMETER];
	char szParameterTemp[MAX_PARAMETER];
	TR69_ERROR nError = OK;
	memset(szParameterName, 0x00, MAX_PARAMETER);
	if(!_res_result_list)
	{
		return PARAMETER_ERROR;
	}
	if(!_node)
	{
		return OK;
	}
//	printf("branch:%d\n",__LINE__);
	if(*_res_result_list == NULL){
//		printf("branch:%d\n",__LINE__);
		*_res_result_list = (LIST_PARAM_VALUE *)malloc( sizeof(LIST_PARAM_VALUE) );
		iter_list = *_res_result_list;
		iter_list->next = NULL;
	}
	else{
	//	printf("branch:%d\n",__LINE__);
		iter_list = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
        add_in_queue(*_res_result_list, iter_list);
	}
//	printf("branch:%d\n",__LINE__);
	strcpy(iter_list->szParameter, _xpath);
	if(_node->elts != NULL)
		strcat(iter_list->szParameter, ".");
	dprintf(LOG_LEVEL1, "level:%s\n",iter_list->szParameter);
	/*hxh:we shoud get this value from attr*/
//	printf("branch:%d\n",__LINE__);
	attr = search_attribute(_node, SZ_ACCESSCONDITION);
//	printf("branch:%d\n",__LINE__);
	if(attr && strcmp(attr->data,SZ_READ))
	{
//	printf("branch:%d\n",__LINE__);
		strcpy(iter_list->szValue, "TRUE");
	}
	else
	{
//	printf("branch:%d\n",__LINE__);
		strcpy(iter_list->szValue, "FALSE");
	}
	dprintf(LOG_LEVEL1, "name:%s\n",_node->name);
	for( itera=_node->elts; itera; itera=tree_next_element(itera))
	{
//	printf("branch:%d\n",__LINE__);
		dprintf(LOG_LEVEL1, "name:%s\n",itera->name);
		iter_list = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
		memset(iter_list, 0x00, sizeof(LIST_PARAM_VALUE));
        add_in_queue(*_res_result_list, iter_list); 
		dprintf(LOG_LEVEL1, "level:%s\n",_xpath);
		strcpy(iter_list->szParameter, _xpath);
		strcat(iter_list->szParameter, ".");
		dprintf(LOG_LEVEL1, "level:%s\n",iter_list->szParameter);
		strcat(iter_list->szParameter, itera->name);
		dprintf(LOG_LEVEL1, "level:%s\n",iter_list->szParameter);
		strcpy(szParameterTemp, iter_list->szParameter);
		if(itera->elts != NULL)
		{
			strcat(iter_list->szParameter, ".");
			strcat(szParameterTemp, ".");
			strcat(szParameterTemp, itera->elts->name);
		}
		
		/*hxh:we shoud get this value from attr*/
		attr = search_attribute(itera, SZ_ACCESSCONDITION);
		if(attr && strcmp(attr->data,SZ_READ))
		{
			strcpy(iter_list->szValue, "TRUE");
		}
		else
		{
			strcpy(iter_list->szValue, "FALSE");
		}
//		printf("branch:%d\n",__LINE__);
		if(_getFlag && itera->elts)
			nError += get_branches_name(itera->elts, szParameterTemp, _res_result_list, _getFlag);
	}
	return nError;
}


void *request_getparameternames_task(void *_threadArg)
{
	dprintf(LOG_LEVEL1,"get parameter names task \n");
	short bFound=0;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	struct soap *soap;
	struct soap_dom_element *itera=NULL;
	struct soap_dom_attribute *attr;
	char soap_endpoint[ARGSIZE];
	short IdManager = -1;

	char szParameterName[MAX_PARAMETER];  // parameter
	char szTemp[MAX_PARAMETER];
	TR69_ERROR error = FUNCTION_ERROR;

	dprintf(LOG_LEVEL1, "request_getparameternames_task\n");
	args = (STRUCT_THREAD_DATA *) _threadArg;
	soap = args->soap;
	message = args->msg;

	if (gListParamValues != NULL)
	{
		free_queue(gListParamValues);
		gListParamValues = NULL;
	}

	dprintf(LOG_LEVEL3, "request_getparameternames_task, message: %p\n", message);

	// search parameter with the right path in DOM tree
	if (message->ListParamValue != NULL)
	{
    	sprintf(szParameterName, "%s", message->ListParamValue->szParameter);
		dprintf(LOG_LEVEL1,"xpath:%s\n",szParameterName);
		dprintf(LOG_LEVEL1,"xpath:%s\n",message->ListParamValue->szParameter);
		dprintf(LOG_LEVEL1,"--->%d<---\n",strlen(szParameterName));

		if(szParameterName[strlen(szParameterName)-1] == '.')
			szParameterName[strlen(szParameterName)-1] = '\0';
		dprintf(LOG_LEVEL1,"xpath:%s\n",szParameterName);
		itera = search_entire_element(gdom_tree, szParameterName);
		if( itera )
		{
			dprintf(LOG_LEVEL1,"itera name:%s\n",itera->name);
			dprintf(LOG_LEVEL1,"itera value:%s\n",message->ListParamValue->szValue);
			if(strncmp(message->ListParamValue->szValue,"TRUE",strlen("TRUE"))== 0)  //add by wangjr for GetParameterNames
				error = get_branches_name(itera, szParameterName, &gListParamValues, 0);//add by wangjr 091113
			else
				error = get_branches_name(itera, szParameterName, &gListParamValues, 1);//add by wangjr 091113
		}
	}

	sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);
	if ( error != OK )
	{
	  dprintf(LOG_LEVEL1, "request_getparameternames_task, CreateErrorResponse\n");
	  // send error to client sender
	  CreateErrorResponse(soap_endpoint, message->Id);
	}
	else
	{
	  // response to request getParameterNames
	  dprintf(LOG_LEVEL3, "request_parameternames_task, Before CreateResponseWithMultipleValues, gListParamValues: %p\n", gListParamValues);
	  // send response with values to client sender
	  CreateResponseWithMultipleValues(soap_endpoint, message->Id, gListParamValues);
	  if (gListParamValues != NULL)
	  {
	    dprintf(LOG_LEVEL3, "free glist\n");
		//free_queue(gListParamValues); // free global list of param-value
		gListParamValues = NULL;
	  }
	}
	
	dprintf(LOG_LEVEL3, "request_getpassiveparameters_task, pthread_detach\n");
	pthread_detach(pthread_self());

	// free copy of message param-value list
	if (message->ListParamValue != NULL)
	{
		dprintf(LOG_LEVEL3, "free mlist\n");
		free_queue(message->ListParamValue);
		message->ListParamValue = NULL;
	}

	// free message
	dprintf(LOG_LEVEL3, "free message\n");
	if (message != NULL) 
		free(message);
	
	dprintf(LOG_LEVEL2, "request_getparameternames_task, before return !\n");

	return NULL;

}



/***************************************************************************/
/*** request getalarmparameter task                                   ***/
/***                                                                     ***/
/*** - search in DOM tree for alarm notification parameters            ***/
/*** - for all parameter found                                           ***/
/***   send a request getValues to component manager found (Id)          ***/
/***   wait for response                                                 ***/
/*** - construct a global list with (parameter, value)                   ***/
/***************************************************************************/
void *request_getalarmparameter_task(void *_threadArg)
{
	short bFound=0;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	struct soap *soap;
	struct soap_dom_element *itera=NULL, *iterb=NULL;
	struct soap_dom_attribute *attr;
	char soap_endpoint[ARGSIZE];
	short IdManager = -1;
	struct soap_dom_element *e=NULL;
	ACCESSCONDITION accessCond;
	
	char szParameter[MAX_PARAMETER];  // parameter
	char szTemp[MAX_PARAMETER];
	char szMonitorName[MAX_PARAMETER];
	char szOrigParameterName[ARGSIZE];
	
	LIST_PARAM_VALUE *elt;
	TR69_ERROR error = FUNCTION_ERROR;
	int ret_sem_wait;
	FILE *fp;
	int count = 0;
	  dprintf(LOG_LEVEL1, "request_getalarmparameters_task\n");
	  args = (STRUCT_THREAD_DATA *) _threadArg;
	  soap = args->soap;
	  message = args->msg;
	
	  if (gListParamValues != NULL)
	  {
		free_queue(gListParamValues);
		gListParamValues = NULL;
	  }
	
	  dprintf(LOG_LEVEL3, "request_getalarmparameter_task, message: %p\n", message);
	
	  if(fp = popen("ls -l /jffs/alarm/ | grep -c alarmpara","r"))
	  {
		fgets(szTemp, MAX_PARAMETER, fp);
		count = atoi(szTemp);
		pclose(fp);
	  }
	  int i;
	  dprintf(4,"--<%d>--\n",count);
	  for(i=1; i<=count; i++)
	  {
		sprintf(szTemp, "/jffs/alarm/alarmpara%d", i);
		if(fp=fopen(szTemp, "r"))
		{
			fgets(szMonitorName, MAX_PARAMETER, fp);
			szMonitorName[strlen(szMonitorName)-1] = '\0';
			dprintf(4,"--<%s>--\n",szMonitorName);
			fclose(fp);
			
			// search parameter with monitor notification in DOM tree
			e = search_entire_element(gdom_tree, szMonitorName);
			if (e != NULL)
			{
			dprintf(4,"<%d>\n",__LINE__);
			  attr = search_attribute(e, "Id"); // retrieve Id of component manager in attribute list
			  if (attr == NULL) {
				error = ID_ERROR;
			  }
			  else
			  {
				IdManager = atoi(attr->data);
				dprintf(4,"<%d>\n",__LINE__);
				attr = search_attribute(e, SZ_ACCESSCONDITION); // retrieve Access conditions in attribute list
				if (attr == NULL) {
					dprintf(4,"<%d>\n",__LINE__);
				  error = DOM_ERROR;
				}
				else 
				{/*
				  dprintf(LOG_LEVEL3, "attr->data: %s\n", attr->data);
				  accessCond = getAccessCondition(attr->data);
				  dprintf(LOG_LEVEL3, "accessCond: %d\n", accessCond);
			
				  // if path of parameter contains instance object, replace path string with original INSTANCE
				  // ex.: Device.LANDevice.1.LANHostConfigManagement ==> Device.LANDevice.INSTANCE1.LANHostConfigManagement
				  memset(szOrigParameterName, 0x00, ARGSIZE);
				  e = str_parameter_path_original(gdom_tree, szMonitorName, szOrigParameterName);
				  if (e != NULL) {
					strcpy(szMonitorName, szOrigParameterName);
					dprintf(LOG_LEVEL3, "ORIGINAL STRING !!\n\n");
					dprintf(LOG_LEVEL3, "szParameterName: %s\n", szMonitorName);*/
				  }
				  error = OK;
				}
			  }
			}
			else error = VARIABLE_ERROR;
			
			// retrieve @IP:port of component manager
			if (error == OK)
			{
			dprintf(4,"<%d>\n",__LINE__);
			  bFound = getSubscriber(gtable, gnbSubscriber, IdManager, soap_endpoint, NULL);
			  dprintf(4,"<%d>-<%d>\n",__LINE__,bFound);
			  if (!bFound)
				error = ID_ERROR;
			  // test access conditions and functions
			  strcpy(message->szFunction, CONST_FUNCTIONGETVALUE);
			  switch(accessCond)
			  {
				case READ:
				  if (strcmp(message->szFunction, CONST_FUNCTIONSETVALUE) == 0) {
					error = ACCESS_ERROR;
					dprintf(4,"<%d>\n",__LINE__);
				  	}
				  break;
				case WRITE:
				  if (strcmp(message->szFunction, CONST_FUNCTIONGETVALUE) == 0){
				  	dprintf(4,"<%d>\n",__LINE__);
					error = ACCESS_ERROR;
				  	}
				  break;
				default:
				  break;
			  }
			}
			dprintf(4,"<%s>\n",message->szFunction);
			if (error==OK)
			{
			  // send request to component manager
			  CreateRequest(soap_endpoint, atoi(attr->data), message->szFunction, szMonitorName, message->ListParamValue->szValue);
			
			  // SEMAPHORE wait for response message
			  dprintf(LOG_LEVEL3, "request_task.. wait for S_response\n");
			  do {
				if ((ret_sem_wait=sem_wait(&S_response)) != 0)	// SEMAPHORE lock
					dprintf(LOG_LEVEL1, "request_task, ERROR on sem_wait !! \n");
			  }
			  while ((ret_sem_wait != 0) && (errno == EINTR));
	
			  if (gflagResponse_OK == 1) // reponse ok
			  {
				error = OK;
		
				// add (parameter, value) in list
				if (gListParamValues == NULL)
				{
					gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
					memset(gListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
					strcpy(gListParamValues->szParameter, szMonitorName);
					strcpy(gListParamValues->szValue, gszValue);
				}
				else
				{
					elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
					strcpy(elt->szParameter, szMonitorName);
					strcpy(elt->szValue, gszValue);
					add_in_queue(gListParamValues, elt);
				}
			  }
	  }
	  	}
	  // send response or error to manager who sent the notify message
	  sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);
	  if ((gflagResponse_OK != 1) && (error != OK))
	  {
		dprintf(LOG_LEVEL1, "request_getalarmparameters_task, CreateErrorResponse\n");
		// send error to client sender
		CreateErrorResponse(soap_endpoint, message->Id);
	  }
	  else
	  {
		// response to request getPassiveParameters
		dprintf(LOG_LEVEL3, "request_getpassiveparameters_task, Before CreateResponseWithMultipleValues, gListParamValues: %p\n", gListParamValues);
		// send response with values to client sender

//		elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
//	    strcpy(elt->szParameter, "InternetGatewayDevice.DeviceInfo.SoftwareVersion");
//	    strcpy(elt->szValue, "v1.1.9.8");
//	    add_in_queue(gListParamValues, elt);

//		elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
//	    strcpy(elt->szParameter, "InternetGatewayDevice.DeviceInfo.HardwareVersion");
//	    strcpy(elt->szValue, "1.20");
//	    add_in_queue(gListParamValues, elt);

//		elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
//	    strcpy(elt->szParameter, "InternetGatewayDevice.ManagementServer.ConnectionRequestURL");
//	    strcpy(elt->szValue, "192.168.17.76:7547");
//	    add_in_queue(gListParamValues, elt);
				
		CreateResponseWithMultipleValues(soap_endpoint, message->Id, gListParamValues);
		if (gListParamValues != NULL)
		{
		  free_queue(gListParamValues); // free global list of param-value
		  gListParamValues = NULL;
		}
	  }
	
	  dprintf(LOG_LEVEL3, "request_getpassiveparameters_task, pthread_detach\n");
	  pthread_detach(pthread_self());
	
	  // free copy of message param-value list
	  if (message->ListParamValue != NULL)
	  {
		free_queue(message->ListParamValue);
		message->ListParamValue = NULL;
	  }
	
	  // free message
	  if (message != NULL) free(message);
	
	  dprintf(LOG_LEVEL2, "request_getalarmparameter_task, before return !\n");
	
	  return NULL;

}

/***************************************************************************/
/*** request getmonitorparameter task                                   ***/
/***                                                                     ***/
/*** - search in DOM tree for monitor notification parameters            ***/
/*** - for all parameter found                                           ***/
/***   send a request getValues to component manager found (Id)          ***/
/***   wait for response                                                 ***/
/*** - construct a global list with (parameter, value)                   ***/
/***************************************************************************/
void *request_getmonitorparameter_task(void *_threadArg)
{
short bFound=0;
STRUCT_THREAD_DATA *args;
STRUCT_MESSAGE *message;
struct soap *soap;
struct soap_dom_element *itera=NULL, *iterb=NULL;
struct soap_dom_attribute *attr;
char soap_endpoint[ARGSIZE];
short IdManager = -1;
struct soap_dom_element *e=NULL;
ACCESSCONDITION accessCond;

char szParameter[MAX_PARAMETER];  // parameter
char szTemp[MAX_PARAMETER];
char szMonitorName[MAX_PARAMETER];
char szOrigParameterName[ARGSIZE];

LIST_PARAM_VALUE *elt;
TR69_ERROR error = FUNCTION_ERROR;
int ret_sem_wait;
FILE *fp;
int count = 0;
  dprintf(LOG_LEVEL1, "request_getpassiveparameters_task\n");
  args = (STRUCT_THREAD_DATA *) _threadArg;
  soap = args->soap;
  message = args->msg;

  if (gListParamValues != NULL)
  {
    free_queue(gListParamValues);
    gListParamValues = NULL;
  }

  dprintf(LOG_LEVEL3, "request_getmonitorparameter_task, message: %p\n", message);

  if(fp = popen("ls -l /jffs/mon/ | grep -c monpara","r"))
  {
  	fgets(szTemp, MAX_PARAMETER, fp);
	count = atoi(szTemp);
  	pclose(fp);
  }
  int i;
  for(i=1; i<=count; i++)
  {
  	sprintf(szTemp, "/jffs/mon/monpara%d", i);
  	if(fp=fopen(szTemp, "r"))
  	{
		fgets(szMonitorName, MAX_PARAMETER, fp);
		szMonitorName[strlen(szMonitorName)-1] = '\0';
		
		fclose(fp);
		
		// search parameter with monitor notification in DOM tree
		e = search_entire_element(gdom_tree, szMonitorName);
		if (e != NULL)
		{
		  attr = search_attribute(e, "Id"); // retrieve Id of component manager in attribute list
		  if (attr == NULL) {
			error = ID_ERROR;
		  }
		  else
		  {
			IdManager = atoi(attr->data);
			attr = search_attribute(e, SZ_ACCESSCONDITION); // retrieve Access conditions in attribute list
			if (attr == NULL) {
			  error = DOM_ERROR;
			}
			else 
			{/*
			  dprintf(LOG_LEVEL3, "attr->data: %s\n", attr->data);
			  accessCond = getAccessCondition(attr->data);
			  dprintf(LOG_LEVEL3, "accessCond: %d\n", accessCond);
		
			  // if path of parameter contains instance object, replace path string with original INSTANCE
			  // ex.: Device.LANDevice.1.LANHostConfigManagement ==> Device.LANDevice.INSTANCE1.LANHostConfigManagement
			  memset(szOrigParameterName, 0x00, ARGSIZE);
			  e = str_parameter_path_original(gdom_tree, szMonitorName, szOrigParameterName);
			  if (e != NULL) {
				strcpy(szMonitorName, szOrigParameterName);
				dprintf(LOG_LEVEL3, "ORIGINAL STRING !!\n\n");
				dprintf(LOG_LEVEL3, "szParameterName: %s\n", szMonitorName);*/
			  }
			  error = OK;
			}
		  }
		}
		else error = VARIABLE_ERROR;
		
		// retrieve @IP:port of component manager
		if (error == OK)
		{
		  bFound = getSubscriber(gtable, gnbSubscriber, IdManager, soap_endpoint, NULL);
		  if (!bFound)
			error = ID_ERROR;
		
		  // test access conditions and functions
		  strcpy(message->szFunction, CONST_FUNCTIONGETVALUE);
		  switch(accessCond)
		  {
			case READ:
			  if (strcmp(message->szFunction, CONST_FUNCTIONSETVALUE) == 0) 
				error = ACCESS_ERROR;
			  break;
			case WRITE:
			  if (strcmp(message->szFunction, CONST_FUNCTIONGETVALUE) == 0)
				error = ACCESS_ERROR;
			  break;
			default:
			  break;
		  }
		}
  	
		if (error==OK)
		{
		  // send request to component manager
		  CreateRequest(soap_endpoint, atoi(attr->data), message->szFunction, szMonitorName, message->ListParamValue->szValue);
		
		  // SEMAPHORE wait for response message
		  dprintf(LOG_LEVEL3, "request_task.. wait for S_response\n");
		  do {
			if ((ret_sem_wait=sem_wait(&S_response)) != 0)	// SEMAPHORE lock
				dprintf(LOG_LEVEL1, "request_task, ERROR on sem_wait !! \n");
		  }
		  while ((ret_sem_wait != 0) && (errno == EINTR));

    	  if (gflagResponse_OK == 1) // reponse ok
    	  {
    		error = OK;
    
    		// add (parameter, value) in list
    		if (gListParamValues == NULL)
    		{
	    		gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
	    		memset(gListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
	    		strcpy(gListParamValues->szParameter, szMonitorName);
	    		strcpy(gListParamValues->szValue, gszValue);
    		}
    		else
    		{
	    		elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
	    		strcpy(elt->szParameter, szMonitorName);
	    		strcpy(elt->szValue, gszValue);
	    		add_in_queue(gListParamValues, elt);
    		}
		  }
  }
 }
  // send response or error to manager who sent the notify message
  sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);
  if ((gflagResponse_OK != 1) && (error != OK))
  {
    dprintf(LOG_LEVEL1, "request_getpassiveparameters_task, CreateErrorResponse\n");
    // send error to client sender
    CreateErrorResponse(soap_endpoint, message->Id);
  }
  else
  {
    // response to request getPassiveParameters
    dprintf(LOG_LEVEL3, "request_getpassiveparameters_task, Before CreateResponseWithMultipleValues, gListParamValues: %p\n", gListParamValues);
    // send response with values to client sender
//	elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
//	strcpy(elt->szParameter, "InternetGatewayDevice.DeviceInfo.SoftwareVersion");
//	strcpy(elt->szValue, "v1.1.9.8");
//	add_in_queue(gListParamValues, elt);
	
//	elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
//	strcpy(elt->szParameter, "InternetGatewayDevice.DeviceInfo.HardwareVersion");
//	strcpy(elt->szValue, "1.20");
//	add_in_queue(gListParamValues, elt);
	
//	elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
//	strcpy(elt->szParameter, "InternetGatewayDevice.ManagementServer.ConnectionRequestURL");
//	strcpy(elt->szValue, "192.168.17.76:7547");
//	add_in_queue(gListParamValues, elt);

	
    CreateResponseWithMultipleValues(soap_endpoint, message->Id, gListParamValues);
    if (gListParamValues != NULL)
    {
      free_queue(gListParamValues); // free global list of param-value
      gListParamValues = NULL;
    }
  }

  dprintf(LOG_LEVEL3, "request_getpassiveparameters_task, pthread_detach\n");
  pthread_detach(pthread_self());

  // free copy of message param-value list
  if (message->ListParamValue != NULL)
  {
    free_queue(message->ListParamValue);
    message->ListParamValue = NULL;
  }

  // free message
  if (message != NULL) free(message);

  dprintf(LOG_LEVEL2, "request_getmonitorparameter_task, before return !\n");

  return NULL;


}
/***************************************************************************/
/*** deleteobject_task                                                       ***/
/***        add by wangjr 091124                                                       ***/
/*** - Analyze request                                                   ***/
/*** - search parameter in DOM tree for Id of manager                    ***/
/*** - send the request to corresponding component manager               ***/
/***************************************************************************/
void *request_deleteobject_task(void *_threadArg)
{
	short bFound=0;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	struct soap *soap;
	struct soap_dom_element *e=NULL;
	struct soap_dom_attribute *attr;
	char soap_endpoint[ARGSIZE];
	short IdManager = -1;
	ACCESSCONDITION accessCond;
	TR69_ERROR error = FUNCTION_ERROR;
	char szParameterName[ARGSIZE];
	char szOrigParameterName[ARGSIZE];
	char szSingleParameterName[ARGSIZE];//wangjr
	char szSubString[ARGSIZE];
	char *ptr;
	int NumberOfEntries;
	int ret_sem_wait;

	args = (STRUCT_THREAD_DATA *) _threadArg;
	soap = args->soap;
	message = args->msg;
	LIST_PARAM_VALUE *iter_paramlist;

	struct soap_dom_element *itera=NULL;

   	if (message->ListParamValue != NULL)
	{
    	sprintf(szParameterName, "%s", message->ListParamValue->szParameter);
		if(szParameterName[strlen(szParameterName)-1] == '.')
			szParameterName[strlen(szParameterName)-1] = '\0';
		itera = search_entire_element(gdom_tree, szParameterName);
		if( itera )
		{
			error = get_branches_name(itera, szParameterName, &gListParamValues, 1);  //error = get_branches_name(itera, szParameterName, &gListParamValues, 0);  //update by wangjr 091204 
		}
	}

	 for(iter_paramlist=gListParamValues; iter_paramlist; iter_paramlist=iter_paramlist->next)
	{	
	    // if szParameterName ends with NumberOfEntries, retrieve NumberOfEntries attribute in its object   
	     e = search_entire_element(gdom_tree, iter_paramlist->szParameter);
		  if (e != NULL)
		  {
			attr = search_attribute(e, SZ_ISEXIST); // retrieve Id of component manager in attribute list
			if (attr == NULL) 
			{
		      error = ID_ERROR;
		    }
			else
			{
				attr =update_isexist_no(e, SZ_ISEXIST);
			 }
		  }
		  else error = VARIABLE_ERROR;

	}
	print_tree(gdom_tree, NULL, 0);

	dprintf(LOG_LEVEL3, "\n\nerror: %d\n\n", error);

label_answers:
	// send response or error to manager who sent the notify message
	sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);

	dprintf(LOG_LEVEL3, "deleteobject, pthread_detach\n");
	pthread_detach(pthread_self());
	// free copy of message param-value list
	if (message->ListParamValue != NULL)
	{
		free_queue(message->ListParamValue);
		message->ListParamValue = NULL;
	}
	// free message
	if (message != NULL) free(message);
	dprintf(LOG_LEVEL2, "deleteobject, before return !\n");
	return NULL;
}

/***************************************************************************/
/*** request_addobject_task                                                       ***/
/***             add by wangjr 091124                                  ***/
/*** - Analyze request                                                   ***/
/*** - search parameter in DOM tree for Id of manager                    ***/
/*** - send the request to corresponding component manager               ***/
/***************************************************************************/
void *request_addobject_task(void *_threadArg)
{
	short bFound=0;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	struct soap *soap;
	struct soap_dom_element *e=NULL;
	struct soap_dom_attribute *attr;
	char soap_endpoint[ARGSIZE];
	short IdManager = -1;
	ACCESSCONDITION accessCond;
	TR69_ERROR error = FUNCTION_ERROR;
	char szParameterName[ARGSIZE];
	char szOrigParameterName[ARGSIZE];
	char szSingleParameterName[ARGSIZE];//wangjr
	char szSubString[ARGSIZE];
	char *ptr;
	int NumberOfEntries;
	int ret_sem_wait;
	int flag=0;
	int i=0;
	LIST_PARAM_VALUE *abc;
	LIST_PARAM_VALUE *elt;

	if (gListParamValues != NULL)
	{
		free_queue(gListParamValues);
		gListParamValues = NULL;
	}
	dprintf(LOG_LEVEL1, "request_task\n");
	args = (STRUCT_THREAD_DATA *) _threadArg;
	soap = args->soap;
	message = args->msg;
	LIST_PARAM_VALUE *iter_paramlist;

	struct soap_dom_element *itera=NULL;

   	if (message->ListParamValue != NULL)
	{
    	sprintf(szParameterName, "%s", message->ListParamValue->szParameter);
		if(szParameterName[strlen(szParameterName)-1] == '.')
			szParameterName[strlen(szParameterName)-1] = '\0';
        for(i=1;i<=4;i++)
        {
			sprintf(szSingleParameterName,"%s.%d",szParameterName,i);
			dprintf(LOG_LEVEL3,"\n\nszSingleParameterName=%s\n\n",szSingleParameterName);
			itera = search_entire_element(gdom_tree, szSingleParameterName);
			if( itera )
			{
				error = get_branches_name(itera, szSingleParameterName, &gListParamValues, 1); //error = get_branches_name(itera, szSingleParameterName, &gListParamValues, 0); //update by wangjr 091204
			}	
			for(iter_paramlist=gListParamValues; iter_paramlist; iter_paramlist=iter_paramlist->next)
			{
			   dprintf(LOG_LEVEL3," datamodel addobject-----szParameterName=%s\n", gListParamValues->szParameter); 
				e = search_entire_element(gdom_tree, iter_paramlist->szParameter);
				if (e != NULL)
				{
					attr = search_attribute(e, SZ_ISEXIST); // retrieve Id of component manager in attribute list
					if (attr == NULL) 
					{
						error = ID_ERROR;
					}
					else
					{
						if(atoi(attr->data)==0)
						{
						  flag=1;
						  attr =update_isexist_yes(e, SZ_ISEXIST);						  
						}
						else
						{
						  flag=0;
						}
					}
				}
				else error = VARIABLE_ERROR;
			}
			if(flag==1) break;
		}
	}          		

	    // if szParameterName ends with NumberOfEntries, retrieve NumberOfEntries attribute in its object   
    print_tree(gdom_tree, NULL, 0);

	if (gListParamValues != NULL)
	{
		free_queue(gListParamValues);
		gListParamValues = NULL;
	}
	if (gListParamValues == NULL)
	{
		gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
		memset(gListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
		strcpy(gListParamValues->szParameter, "InternetGatewayDevice.LANDevice.1.WLANConfiguration");
		sprintf(gListParamValues->szValue, "%d",i);
	}
    
	sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);

	CreateResponseWithMultipleValues(soap_endpoint, message->Id, gListParamValues);

	if (gListParamValues != NULL)
	{
		free_queue(gListParamValues); // free global list of param-value
		gListParamValues = NULL;
	}

	// send response or error to manager who sent the notify message
	dprintf(LOG_LEVEL3, "deleteobject, pthread_detach\n");
	pthread_detach(pthread_self());

	// free copy of message param-value list
	if (message->ListParamValue != NULL)
	{
		free_queue(message->ListParamValue);
		message->ListParamValue = NULL;
	}

	// free message
	if (message != NULL) free(message);
	dprintf(LOG_LEVEL2, "deleteobject, before return !\n");
	return NULL;
}


/***************************************************************************/
/*** request_SetParameterAttributes_task                                                     ***/
/***            add by wangjr 091124                                      ***/
/*** - Analyze request                                                   ***/
/*** - search parameter in DOM tree for Id of manager                    ***/
/*** - send the request to corresponding component manager               ***/
/***************************************************************************/
void *request_SetParameterAttributes_task(void *_threadArg)
{
	short bFound=0;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	struct soap *soap;
	struct soap_dom_element *e=NULL;
	struct soap_dom_attribute *attr;
	char soap_endpoint[ARGSIZE];
	short IdManager = -1;
	ACCESSCONDITION accessCond;
	TR69_ERROR error = FUNCTION_ERROR;
	char szParameterName[ARGSIZE];
	char szOrigParameterName[ARGSIZE];
	char szSingleParameterName[ARGSIZE];//wangjr
	char szSubString[ARGSIZE];
	char *ptr;
	int NumberOfEntries;
	int ret_sem_wait;

	args = (STRUCT_THREAD_DATA *) _threadArg;
	soap = args->soap;
	message = args->msg;
	LIST_PARAM_VALUE *iter_paramlist;

	struct soap_dom_element *itera=NULL;

   	if (message->ListParamValue != NULL)
	{
    	sprintf(szParameterName, "%s", message->ListParamValue->szParameter);
		if(szParameterName[strlen(szParameterName)-1] == '.')
		{			
			szParameterName[strlen(szParameterName)-1] = '\0';
			itera = search_entire_element(gdom_tree, szParameterName);
			if( itera )
			{
				error = get_branches_name(itera, szParameterName, &gListParamValues, 0);
			}

			for(iter_paramlist=gListParamValues; iter_paramlist; iter_paramlist=iter_paramlist->next)
			{	
				// if szParameterName ends with NumberOfEntries, retrieve NumberOfEntries attribute in its object   
				e = search_entire_element(gdom_tree, iter_paramlist->szParameter);
				if (e != NULL)
				{
					attr = search_attribute(e, SZ_NOTIFICATION); // retrieve Id of component manager in attribute list
					if (attr == NULL) 
					{
						error = ID_ERROR;
					}
					else
					{
						attr =update_notification(e, SZ_NOTIFICATION, message->ListParamValue->szValue);
					}
				}
				else error = VARIABLE_ERROR;

			}
		}
		else
		{
		     dprintf(LOG_LEVEL3, "\n\ message->ListParamValue->szValue: %s\n\n",  message->ListParamValue->szValue);
			  e = search_entire_element(gdom_tree, szParameterName);   //modify by wangjr 
			if (e != NULL)
			{
				attr = search_attribute(e, SZ_NOTIFICATION); // retrieve Access conditions in attribute list
				if (attr == NULL) {
				  error = ID_ERROR;
				}
				else 
				{
				  attr =update_notification(e, SZ_NOTIFICATION, atoi(message->ListParamValue->szValue));
			
				}
			}
			else error = VARIABLE_ERROR;
		}
	}

	print_tree(gdom_tree, NULL, 0);

	dprintf(LOG_LEVEL3, "\n\nerror: %d\n\n", error);

label_answers:

	sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);
	dprintf(LOG_LEVEL3, "SetParameterAttributes, pthread_detach\n");
	pthread_detach(pthread_self());
	// free copy of message param-value list
	if (message->ListParamValue != NULL)
	{
		free_queue(message->ListParamValue);
		message->ListParamValue = NULL;
	}
	// free message
	if (message != NULL) free(message);
	dprintf(LOG_LEVEL2, "SetParameterAttributes, before return !\n");
	return NULL;
}

/***************************************************************************/
/*** request GetParameterAttributes task                                 ***/
/***                 add    by wangjr 091124                                          ***/
/*** - search in DOM tree for passive notification parameters            ***/
/*** - for all parameter found                                           ***/
/***   send a request etValues to component manager found (Id)          ***/
/***   wait for response                                                 ***/
/*** - construct a global list with (parameter, value)                   ***/
/***************************************************************************/
void *request_GetParameterAttributes_task(void *_threadArg)
{
	short bFound=0;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	struct soap *soap;
	struct soap_dom_element *e=NULL;
	struct soap_dom_attribute *attr;
	char soap_endpoint[ARGSIZE];
	short IdManager = -1;
	TR69_ERROR error = FUNCTION_ERROR;
	char szParameterName[ARGSIZE];
	char szOrigParameterName[ARGSIZE];
	char szSingleParameterName[ARGSIZE];//wangjr
	char szSubString[ARGSIZE];
	char *ptr;
	int NumberOfEntries;
	int ret_sem_wait;
	LIST_PARAM_VALUE *iter_paramlist;
	struct soap_dom_element *itera=NULL;
	LIST_PARAM_VALUE *elt;

	
	if (gListParamValues != NULL)
	{
		free_queue(gListParamValues);
		gListParamValues = NULL;
	}


	if (gtmpListParamValues != NULL)
	{
		free_queue(gtmpListParamValues);
		gtmpListParamValues = NULL;
	}
	
	dprintf(LOG_LEVEL1, "GetParameterAttributes\n");
	args = (STRUCT_THREAD_DATA *) _threadArg;
	soap = args->soap;
	message = args->msg;
	
	dprintf(LOG_LEVEL3, "\nxxxxxxxxxxxxxxxxxxxxxrequest_GetParameterAttributes_taskxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	dprintf(LOG_LEVEL3, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	dprintf(LOG_LEVEL3, "xxxxxxxxxxxxxxxxxxxx: %s\n", message->ListParamValue->szParameter);
	dprintf(LOG_LEVEL3, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	  // search parameter in DOM tree
	if (message->ListParamValue != NULL)
	{
		sprintf(szParameterName, "%s", message->ListParamValue->szParameter);
        #if 0
		if((atoi(szParameterName[strlen(szParameterName)-1]) >= 1) && (atoi(szParameterName[strlen(szParameterName)-1]) <= 9))
		{
			//szParameterName[strlen(szParameterName)-1] = '\0';
			itera = search_entire_element(gdom_tree, szParameterName);
			if( itera )
			{
			    dprintf(LOG_LEVEL3," \n itera  !=NULL \n");
				error = get_branches_name(itera, szParameterName, &gListParamValues, 0);
			}
			for(iter_paramlist=gListParamValues; iter_paramlist; iter_paramlist=iter_paramlist->next)
			{
				e = search_entire_element(gdom_tree, iter_paramlist->szParameter);
				if (e != NULL)
				{	
					  attr = search_attribute(e, SZ_NOTIFICATION); // retrieve notifation conditions in attribute list
					  if (attr == NULL) {
						error = DOM_ERROR;
					  }
					  else 
					  {
						error = OK;
				
					  }					  
				}
				else error = VARIABLE_ERROR;

		dprintf(LOG_LEVEL3," datamodel----- error=%d\n", error);
		dprintf(LOG_LEVEL3," datamodel-----attr->data=%s\n", attr->data);
		 dprintf(LOG_LEVEL3," datamodel-----szParameterName=%s\n", gListParamValues->szParameter);
		strcpy(iter_paramlist->szValue, attr->data);				 
		// dprintf(LOG_LEVEL3," datamodel----- message->ListParamValue->szValue=%s\n", attr->data);				
			}

			
		}
		#endif
	  // else
	  // {
			e = search_entire_element(gdom_tree, szParameterName);   //modify by wangjr	
		  if (e != NULL)
		  {
			  attr = search_attribute(e, SZ_NOTIFICATION); // retrieve Access conditions in attribute list
			  if (attr == NULL) {
				error = DOM_ERROR;
			  }
			  else 
			  {
			    error = OK;
		
			  }
		  }
		  else error = VARIABLE_ERROR;
		 dprintf(LOG_LEVEL3," datamodel-----attr->data=%s\n", attr->data);
		 dprintf(LOG_LEVEL3," datamodel-----message->szFunction=%s\n", message->szFunction);
		 dprintf(LOG_LEVEL3," datamodel-----szParameterName=%s\n", szParameterName);
		 dprintf(LOG_LEVEL3," datamodel----- message->ListParamValue->szValue=%s\n", message->ListParamValue->szValue);
		 if (gListParamValues != NULL)
		 {
			 free_queue(gListParamValues);
			 gListParamValues = NULL;
		 }
		 if (gListParamValues == NULL)
		 {
			 gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
			 memset(gListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
			 strcpy(gListParamValues->szParameter,szParameterName);
			 strcpy(gListParamValues->szValue, attr->data);
		 }
	   //	}

	}

	  // send response or error to manager who sent the notify message
	sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);

	dprintf(LOG_LEVEL3, "GetParameterAttributes, CreateResponse\n");	
	// send response with values to client sender
	CreateResponseWithMultipleValues(soap_endpoint, message->Id, gListParamValues);	
	if (gListParamValues != NULL)
	{
		free_queue(gListParamValues); // free global list of param-value
		gListParamValues = NULL;
	}
	pthread_detach(pthread_self());
	
	  // free copy of message param-value list
	  if (message->ListParamValue != NULL)
	  {
		free_queue(message->ListParamValue);
		message->ListParamValue = NULL;
	  }	
	  // free message
	  if (message != NULL) free(message);
	
	  dprintf(LOG_LEVEL2, "request_task, before return !\n");
	
	  return NULL;

}
/*** request getPassiveparameters task                                   ***/
/***                                                                     ***/
/*** - search in DOM tree for passive notification parameters            ***/
/*** - for all parameter found                                           ***/
/***   send a request getValues to component manager found (Id)          ***/
/***   wait for response                                                 ***/
/*** - construct a global list with (parameter, value)                   ***/
/***************************************************************************/
void *request_getpassiveparameters_task(void *_threadArg)
{
	short bFound=0;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	struct soap *soap;
	struct soap_dom_element *itera=NULL, *iterb=NULL;
	struct soap_dom_attribute *attr;
	char soap_endpoint[ARGSIZE];
	short IdManager = -1;

	char szParameter[MAX_PARAMETER];  // parameter
	char szTemp[MAX_PARAMETER];
	LIST_PARAM_VALUE *elt;
	TR69_ERROR error = FUNCTION_ERROR;
	int ret_sem_wait;

	dprintf(LOG_LEVEL1, "request_getpassiveparameters_task\n");
	args = (STRUCT_THREAD_DATA *) _threadArg;
	soap = args->soap;
	message = args->msg;

	if (gListParamValues != NULL)
	{
	free_queue(gListParamValues);
	gListParamValues = NULL;
	}

	dprintf(LOG_LEVEL3, "request_getpassiveparameters_task, message: %p\n", message);

	// search parameter with passive notification in DOM tree
	for (itera = gdom_tree; itera; itera = soap_dom_next_element(itera))
	{
		 attr = search_attribute(itera, SZ_ISEXIST);  //update by wangjr 091124
		if(attr == NULL)
		{
			 error = DOM_ERROR;
		}
		else
		{
		
		   if(atoi(attr->data) == 0)
		  {
			   error = DOM_ERROR;
		  }
		  else
		  {
	attr = search_attribute(itera, SZ_NOTIFICATION); // retrieve notification in attribute list
	if (attr != NULL)
	{ 
	  /*hxh:change for passive inform*/
	  if (attr->data && ((strcmp(attr->data, SZ_PASSIVE)==0) ))//|| (strcmp(attr->data, SZ_ACTIVE)==0)))
	  {
	    attr = search_attribute(itera, "Id");
	    if (attr != NULL)
	    {
	      IdManager = atoi(attr->data);

	      // search @IP, port in table
	      bFound = getSubscriber(gtable, gnbSubscriber, IdManager, soap_endpoint, NULL);
	      if (!bFound)
	        error = ID_ERROR;

	      attr = search_attribute(itera, SZ_ACCESSCONDITION);
	      if (attr != NULL)
	      {
	        // test access conditions
	        if (attr->data && ((strcmp(attr->data, SZ_READ)==0) || (strcmp(attr->data, SZ_READWRITE)==0)) )
	        {
	          memset(szParameter, 0x00, sizeof(szParameter));
	          // construct datamodel parameter with complete path
	          iterb = itera;
	          while (iterb != NULL)
	          {
	            if (strcmp(szParameter, "") == 0) sprintf(szParameter, "%s", iterb->name);
	            else
	            {
	              strcpy(szTemp, szParameter);
	              sprintf(szParameter, "%s.%s", iterb->name, szTemp);
	            }
	            iterb = iterb->prnt;
	          }
	          dprintf(LOG_LEVEL3, "szParameter: %s\n", szParameter);

	          // send request to component manager
	          CreateRequest(soap_endpoint, IdManager, CONST_FUNCTIONGETVALUE, szParameter, NULL);

	          // wait for response of component manager
	          // SEMAPHORE wait for response message
	          dprintf(LOG_LEVEL3, "request_getpassiveparameters_task.. wait for semaphore\n");
	          do {
	            if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
	              dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
	          }
	          while ((ret_sem_wait != 0) && (errno == EINTR));

	          if (gflagResponse_OK == 1) // reponse ok
	          {
	            error = OK;

	            // add (parameter, value) in list
	            if (gListParamValues == NULL)
	            {
	              gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
	              memset(gListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
	              strcpy(gListParamValues->szParameter, szParameter);
	              strcpy(gListParamValues->szValue, gszValue);
	            }
	            else
	            {
	              elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
	              strcpy(elt->szParameter, szParameter);
	              strcpy(elt->szValue, gszValue);
	              add_in_queue(gListParamValues, elt);
				            }
				          }
				        }
	          }
	        }
	      }
	    }
	  }
	}
	}

	// send response or error to manager who sent the notify message
	sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);
	if ((gflagResponse_OK != 1) && (error != OK))
	{
		dprintf(LOG_LEVEL1, "request_getpassiveparameters_task, CreateErrorResponse\n");
		// send error to client sender
		CreateErrorResponse(soap_endpoint, message->Id);
	}
	else
	{
	// response to request getPassiveParameters
	dprintf(LOG_LEVEL3, "request_getpassiveparameters_task, Before CreateResponseWithMultipleValues, gListParamValues: %p\n", gListParamValues);
	// send response with values to client sender
	CreateResponseWithMultipleValues(soap_endpoint, message->Id, gListParamValues);
	if (gListParamValues != NULL)
	{
		free_queue(gListParamValues); // free global list of param-value
		gListParamValues = NULL;
	}
	}

	dprintf(LOG_LEVEL3, "request_getpassiveparameters_task, pthread_detach\n");
	pthread_detach(pthread_self());

	// free copy of message param-value list
	if (message->ListParamValue != NULL)
	{
		free_queue(message->ListParamValue);
		message->ListParamValue = NULL;
	}

	// free message
	if (message != NULL) free(message);

	dprintf(LOG_LEVEL2, "request_getpassiveparameters_task, before return !\n");

	return NULL;
}

/***************************************************************************/
/*** request getActiveparameters task                                   ***/
/***                   add by wangjr 091124                                                  ***/
/*** - search in DOM tree for passive notification parameters            ***/
/*** - for all parameter found                                           ***/
/***   send a request getValues to component manager found (Id)          ***/
/***   wait for response                                                 ***/
/*** - construct a global list with (parameter, value)                   ***/
/***************************************************************************/
void *request_getactiveparameters_task(void *_threadArg)
{
	short bFound=0;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	struct soap *soap;
	struct soap_dom_element *itera=NULL, *iterb=NULL;
	struct soap_dom_attribute *attr;
	char soap_endpoint[ARGSIZE];
	short IdManager = -1;

	char szParameter[MAX_PARAMETER];  // parameter
	char szTemp[MAX_PARAMETER];
	LIST_PARAM_VALUE *elt;
	TR69_ERROR error = FUNCTION_ERROR;
	int ret_sem_wait;

	dprintf(LOG_LEVEL1, "request_getactiveparameters_task\n");
	args = (STRUCT_THREAD_DATA *) _threadArg;
	soap = args->soap;
	message = args->msg;

	if (gListParamValues != NULL)
	{
	free_queue(gListParamValues);
	gListParamValues = NULL;
	}

	dprintf(LOG_LEVEL3, "request_getactiveparameters_task, message: %p\n", message);

	// search parameter with passive notification in DOM tree
	for (itera = gdom_tree; itera; itera = soap_dom_next_element(itera))
	{

		attr = search_attribute(itera, SZ_ISEXIST);
		if(attr == NULL)
		{
			 error = DOM_ERROR;
		}
		else
		{
		
		   if(atoi(attr->data) == 0)
		  {
			   error = DOM_ERROR;
		  }
		  else
		  {
				attr = search_attribute(itera, SZ_NOTIFICATION); // retrieve notification in attribute list
				if (attr != NULL)
				{ 
				  /*hxh:change for actiive inform*/
				  		  if (attr->data && ((strcmp(attr->data, SZ_ACTIVE)==0) ))//|| (strcmp(attr->data, SZ_ACTIVE)==0)))
						  {
						    attr = search_attribute(itera, "Id");
						    if (attr != NULL)
						    {
						      IdManager = atoi(attr->data);

						      // search @IP, port in table
						      bFound = getSubscriber(gtable, gnbSubscriber, IdManager, soap_endpoint, NULL);
						      if (!bFound)
						        error = ID_ERROR;

						      attr = search_attribute(itera, SZ_ACCESSCONDITION);
						      if (attr != NULL)
						      {
						        // test access conditions
						        if (attr->data && ((strcmp(attr->data, SZ_READ)==0) || (strcmp(attr->data, SZ_READWRITE)==0)) )
						        {
						          memset(szParameter, 0x00, sizeof(szParameter));
						          // construct datamodel parameter with complete path
						          iterb = itera;
						          while (iterb != NULL)
						          {
						            if (strcmp(szParameter, "") == 0) sprintf(szParameter, "%s", iterb->name);
						            else
						            {
						              strcpy(szTemp, szParameter);
						              sprintf(szParameter, "%s.%s", iterb->name, szTemp);
						            }
						            iterb = iterb->prnt;
						          }
						          dprintf(LOG_LEVEL3, "szParameter: %s\n", szParameter);

						          // send request to component manager
						          CreateRequest(soap_endpoint, IdManager, CONST_FUNCTIONGETVALUE, szParameter, NULL);

						          // wait for response of component manager
						          // SEMAPHORE wait for response message
						          dprintf(LOG_LEVEL3, "request_getactiveparameters_task.. wait for semaphore\n");
						          do {
						            if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
						              dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
						          }
						          while ((ret_sem_wait != 0) && (errno == EINTR));

						          if (gflagResponse_OK == 1) // reponse ok
						          {
						            error = OK;

						            // add (parameter, value) in list
						            if (gListParamValues == NULL)
						            {
						              gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
						              memset(gListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
						              strcpy(gListParamValues->szParameter, szParameter);
						              strcpy(gListParamValues->szValue, gszValue);
						            }
						            else
						            {
						              elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
						              strcpy(elt->szParameter, szParameter);
						              strcpy(elt->szValue, gszValue);
						              add_in_queue(gListParamValues, elt);
						            }
						          }
						        }
						      }
						    }

						}
				}
				}
		  }
	}

	// send response or error to manager who sent the notify message
	sprintf(soap_endpoint, "%ld.%ld.%ld.%ld:%d", (message->Ip >> 24)&0xFF, (message->Ip >> 16)&0xFF, (message->Ip >> 8)&0xFF, message->Ip&0xFF, message->Port);
	if ((gflagResponse_OK != 1) && (error != OK))
	{
		dprintf(LOG_LEVEL1, "request_getactiveparameters_task, CreateErrorResponse\n");
		// send error to client sender
		CreateErrorResponse(soap_endpoint, message->Id);
	}
	else
	{
	// response to request getactiveParameters
	dprintf(LOG_LEVEL3, "request_getactiveparameters_task, Before CreateResponseWithMultipleValues, gListParamValues: %p\n", gListParamValues);
	// send response with values to client sender
	CreateResponseWithMultipleValues(soap_endpoint, message->Id, gListParamValues);
	if (gListParamValues != NULL)
	{
		free_queue(gListParamValues); // free global list of param-value
		gListParamValues = NULL;
	}
	}

	dprintf(LOG_LEVEL3, "request_getactiveparameters_task, pthread_detach\n");
	pthread_detach(pthread_self());

	// free copy of message param-value list
	if (message->ListParamValue != NULL)
	{
		free_queue(message->ListParamValue);
		message->ListParamValue = NULL;
	}

	// free message
	if (message != NULL) free(message);

	dprintf(LOG_LEVEL2, "request_getactiveparameters_task, before return !\n");

	return NULL;
}



/***************************************************************************/
/*** subscribe_task                                                      ***/
/***                                                                     ***/
/*** analyze subscribe message                                           ***/
/*** - sender                                                            ***/
/*** - part of datamodel (local or component manager)                    ***/
/*** - copy/merge received branch in global DOM tree                     ***/
/***************************************************************************/
int subscribe_task(struct soap_dom_element *dom, enum methodName eMethodName, enum hostType eHostType, STRUCT_MESSAGE *message)
{
int i;
short bFound = 0;
char szTemp[3];
struct soap_dom_element *e;
TR69_ERROR error = OK;

  dprintf(LOG_LEVEL1, "subscribe_task\n");
  dprintf(LOG_LEVEL1, "subscribe_task, gnbSubscriber: %d\n", gnbSubscriber);
  dprintf(LOG_LEVEL1, "subscribe_task: ip: %d\n", message->Ip);
  dprintf(LOG_LEVEL1, "subscribe_task: port: %d\n", message->Port);
  if ((message->Ip == 0) || (message->Port == 0))
  {
    error = PARAMETER_ERROR;
    goto endthread_label;
  }

  // LookUp '@IP:port' value with value in table
  // find if subscriber already exists
  for (i = 0; i < gnbSubscriber; i++)
  {
    if ((gtable[i].Ip == message->Ip) && (gtable[i].Port == message->Port))
    {
      bFound = 1;
      break;
    }
  }

  if (!bFound) // add subscriber values to subscriber table
  {
    gId++;

    gtable[gnbSubscriber].Id = gId;
    gtable[gnbSubscriber].Type = eHostType;
    gtable[gnbSubscriber].Ip = message->Ip;
    gtable[gnbSubscriber].Port = message->Port;
    gnbSubscriber++;
  }

  // print subscriber table
  print_subscriber(gtable, gnbSubscriber);

  // SUBSCRIBE + data model parts of manager
  if ((eMethodName == SUBSCRIBE) && ((eHostType == TYPE_LOCAL) || (eHostType == TYPE_CMP_MGR)))
  {
    //dprintf(LOG_LEVEL1, "subscribe_task.. data model parts\n");
    dprintf(LOG_LEVEL1, "subscribe_task.. data model parts\n");

    // Decode datamodel part of XML message to DOM branch
    if  (NULL == soap_in_xsd__anyType(dom->soap, NULL, dom, NULL))
    {
      error = SYNTAX_ERROR;
      goto endthread_label;
    }

    if (error == OK)
    {
      //dprintf(LOG_LEVEL1, "subscribe_task.. before find_attributes_in_branch\n");
      dprintf(LOG_LEVEL1, "subscribe_task.. before find_attributes_in_branch\n");
      
      // add attribute 'Id' at end of attributes list of DOM branch
      dprintf(LOG_LEVEL1, "subscribe_task: dom_name: %s\n", dom->name);
      e = find_attributes_in_branch(dom);
      if (e != NULL)
      {
        sprintf(szTemp, "%d", gId);
        dprintf(LOG_LEVEL1, "subscribe_task: id: %s\n", szTemp);
        add_attributes(dom->soap, e, "Id", szTemp);
      }
      else
      {
        error = DOM_ERROR;
        goto endthread_label;
      }

      //dprintf(LOG_LEVEL3, "subscribe_task, Display DOM branch\n");
      dprintf(LOG_LEVEL1, "subscribe_task, Display DOM branch\n");
      print_tree(dom, NULL, 0);

      //dprintf(LOG_LEVEL3, "manage_instances_in_branch\n");
      dprintf(LOG_LEVEL1, "manage_instances_in_branch\n");
      manage_instances_in_branch(gSoapTree, dom, gId);
      dprintf(LOG_LEVEL1, "manage_instances_in_branch end\n");
      // Add dom branch in dom tree
      if (gdom_tree == NULL)
      {
        // First subscribe, copy dom branch in dom tree
        gSoapTree = soap_copy(dom->soap);

        gdom_tree = copy_branch(gSoapTree, dom, NULL);
      }
      else 
      {
        // merge/synchronize dom branch with dom tree
        //merge_branch(gSoapTree, gdom_tree, dom);
		dprintf(LOG_LEVEL1, "merge branch in tree\n");
        merge_branch_in_tree(gSoapTree, gdom_tree, dom);
      }

      //dprintf(LOG_LEVEL1, "subscribe_task, Display DOM tree\n");
      dprintf(LOG_LEVEL1, "subscribe_task, Display DOM tree\n");
      print_tree(gdom_tree, NULL, 0);
    }
  }
endthread_label:
  if (error != OK)
    //dprintf(LOG_LEVEL1, "subscribe_task, %s\n", getErrorString(error));
    dprintf(LOG_LEVEL1, "subscribe_task, %s\n", getErrorString(error));

  // free copy of message param-value list
  if (message->ListParamValue != NULL)
  {
    free_queue(message->ListParamValue);
    message->ListParamValue = NULL;
  }

  // free message
  if (message != NULL) free(message);

  dprintf(LOG_LEVEL2, "subscribe_task, before return\n");
  return (error==OK);
}


// Print subscriber table
//
void print_subscriber(STRUCT_DATAMODEL_ROW *tab, int nbRows)
{
int i;
  dprintf(LOG_LEVEL3, "Subscriber table\n");
  dprintf(LOG_LEVEL3, "Id \t TYPE \t @IP:port\n");
  for (i=0; i < nbRows; i++)
  {
    dprintf(LOG_LEVEL3, "%d\t", tab[i].Id);
    switch(tab[i].Type)
    {
      case TYPE_LOCAL:
        dprintf(LOG_LEVEL3, "LCL MGR\t");
        break;
      case TYPE_CLIENT:
        dprintf(LOG_LEVEL3, "CLIENT \t");
        break;
      case TYPE_CMP_MGR:
        dprintf(LOG_LEVEL3, "CMP MGR\t");
        break;
      default:
        dprintf(LOG_LEVEL3, "inconnu\t");
    }
    dprintf(LOG_LEVEL3, "%ld.%ld.%ld.%ld:%d\n", (tab[i].Ip >> 24)&0xFF, (tab[i].Ip >> 16)&0xFF, (tab[i].Ip >> 8)&0xFF, tab[i].Ip&0xFF, tab[i].Port);
  }
  dprintf(LOG_LEVEL3, "\n");
}

// Look up a value in subscriber table
// return row if value found, else -1
//
int LookUpSubscriber(STRUCT_DATAMODEL_ROW *_tab, int nbRows, enum DataModelCol _column, void *_ptrVal)
{
int *Id;
enum hostType *Type;
unsigned long *Ip;
int *Port;
int i;
  for (i = 0; i < nbRows; i++)
  {
    switch(_column)
    {
      case ID:
        Id = (int*)_ptrVal;
        dprintf(LOG_LEVEL3, "Id to search: %d, _tab[i].Id: %d\n", *Id, _tab[i].Id);

        if (*Id == _tab[i].Id) return i;
        break;
      case SUBSCRIBER:
        Type = (enum hostType*)_ptrVal;
        dprintf(LOG_LEVEL3, "Type to search: %d, _tab[i].Type: %d\n", *Type, _tab[i].Type);
      
        if (*Type == _tab[i].Type) return i;
        break;
      case IP:
        Ip = (unsigned long*)_ptrVal;
        dprintf(LOG_LEVEL3, "Ip to search: %ld.%ld.%ld.%ld, val : %ld, _tab[i].Ip: %ld\n", ((*Ip) >> 24)&0xFF, ((*Ip) >> 16)&0xFF, ((*Ip) >> 8)&0xFF, (*Ip)&0xFF, *Ip, _tab[i].Ip);

        if (*Ip == _tab[i].Ip) return i;
        break;
      case PORT:
        Port = (int*)_ptrVal;
        dprintf(LOG_LEVEL3, "Port to search: %d, _tab[i].Port: %d\n", *Port, _tab[i].Port);

        if (*Port == _tab[i].Port) return i;
        break;
    }
  }

  return -1;
}


// Find Id in subscriber table
// parameters :
// (i) table, nb of rows in table, Id to find
// (o) string var to store @IP+port
// return 1 if value found, else 0
//
short getSubscriber(STRUCT_DATAMODEL_ROW *_table, int _nbElement, short _Id, char *_address, int *_portNumber)
{
short bRet = 0;
int i;

  for (i=0; i < _nbElement; i++)
  {
    if (_table[i].Id == _Id)
    {
      bRet = 1;
      if (_address != NULL)
      {
        sprintf(_address, "%ld.%ld.%ld.%ld:%d", (_table[i].Ip >> 24)&0xFF, (_table[i].Ip >> 16)&0xFF, (_table[i].Ip >> 8)&0xFF, _table[i].Ip&0xFF, _table[i].Port);
        dprintf(LOG_LEVEL3, "_address: %s\n", _address);

        if (_portNumber != NULL) *_portNumber = _table[i].Port;
      }
      break;
    }
  }

  dprintf(LOG_LEVEL3, "getSubscriber, bRet: %d\n", bRet);
  return bRet;
}

#ifdef __cplusplus
}
#endif
