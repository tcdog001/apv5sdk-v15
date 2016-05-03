#include "global.h"

/* ------------------------------------------------------------------------------------------------------------------ */
/* define variables */


#ifndef SINGLE_THREAD
	pthread_key_t LastErr_Key;
#else
	static HANDLE_ERR_INFO *LastErr;
#endif

/* ------------------------------------------------------------------------------------------------------------------ */
/* define functions */
BOOL init_handle_err() 
{		
	#ifndef SINGLE_THREAD
		if(pthread_key_create(&LastErr_Key, NULL) != 0)
		{
			return FALSE;
		}
	#else
		HANDLE_ERR_INFO *pLastErr = NULL;
		CREATE_OBJECT_ERR(pLastErr, HANDLE_ERR_INFO, return FALSE;);
		pLastErr->code = ERROR_NONE;
		LastErr = pLastErr;
	#endif
	return TRUE;
}

BOOL raise_err(ErrorCode code, const char *msg, const char *filename, int line) 
{
	HANDLE_ERR_INFO *pLastErr = NULL;
#ifndef SINGLE_THREAD
	pLastErr = pthread_getspecific(LastErr_Key);
	if(pLastErr == NULL)
	{
		CREATE_OBJECT_ERR(pLastErr, HANDLE_ERR_INFO, return FALSE;);
		pLastErr->code = ERROR_NONE;
		if(!pthread_setspecific(LastErr_Key, pLastErr))
		{
			APVLog("Critical Error, closing the process...\n"); 
		}
	}
#else
	pLastErr = LastErr;
#endif
	
	if(pLastErr == NULL) 
	{
		APVLog("Critical Error: something strange has happened, closing the process...\n"); 
	}
	
	pLastErr->code = code;
	if(msg != NULL) 
		strcpy(pLastErr->msg, msg);
	else 
		pLastErr->msg[0]='\0';
	if(filename != NULL)
	{
		strcpy(pLastErr->fileName, filename);
	}
	pLastErr->line = line;
	return FALSE;
}
/* */
ErrorCode get_last_err_code() 
{
	HANDLE_ERR_INFO *pLastErr = NULL;
	
	#ifndef SINGLE_THREAD
		pLastErr = pthread_getspecific(LastErr_Key);
	#else
		pLastErr = LastErr;
	#endif
	
	if(pLastErr == NULL) 
		return ERROR_GENERAL;
	
	return pLastErr->code;
}
void print_last_err(HANDLE_ERR_INFO *pLastErr, const char *err, const char *fileName, int line) 
{
	if(pLastErr == NULL) 
		return;
	
	if((pLastErr->msg != NULL) && (pLastErr->msg[0] != '\0')) 
	{
		APLog("Error: %s. %s .\n", err, pLastErr->msg);
	} 
	else 
	{
		APLog("Error: %s\n", err);
	}
	APLog("(occurred at line %d in file %s, catched at line %d in file %s).\n",pLastErr->line, pLastErr->fileName, line, fileName);
}
/* */
BOOL handle_last_err(const char *fileName, int line) 
{
	HANDLE_ERR_INFO *pLastErr = NULL;
	
	#ifndef SINGLE_THREAD
		pLastErr = pthread_getspecific(LastErr_Key);
	#else
		pLastErr = LastErr;
	#endif
	/* */
	if(pLastErr == NULL) 
	{
		APVLog("No Error Pending\n");
		return FALSE;
	}
	
	#define __CW_ERROR_PRINT(str)	print_last_err(pLastErr, (str), fileName, line)
	
	switch(pLastErr->code) 
	{
		case ERROR_SUCCESS:
		case ERROR_NONE:
			return TRUE;
			break;
		case ERROR_OUT_OF_MEMORY:
			__CW_ERROR_PRINT("Out of Memory");
			#ifndef SINGLE_THREAD
				pthread_exit(NULL); // note: we can manage this on per-thread basis: ex. we can
								    // kill some other thread if we are a manager thread.
			#else
				exit(1);
			#endif
			break;
			
		case ERROR_WRONG_ARG:
			__CW_ERROR_PRINT("Wrong Arguments in Function");
			break;
			
		case ERROR_NEED_RESOURCE:
			__CW_ERROR_PRINT("Missing Resource");
			break;
			
		case ERROR_GENERAL:
			__CW_ERROR_PRINT("Error Occurred");
			break;
		
		case ERROR_CREATING:
			__CW_ERROR_PRINT("Error Creating Resource");
			break;
			
		case ERROR_SENDING:
			__CW_ERROR_PRINT("Error Sending");
			break;
		
		case ERROR_RECEIVING:
			__CW_ERROR_PRINT("Error Receiving");
			break;
			
		case ERROR_INVALID_FORMAT:
			__CW_ERROR_PRINT("Invalid Format");
			break;
				
		case ERROR_INTERRUPTED:
		default:
			break;
	}
	
	return FALSE;
}



