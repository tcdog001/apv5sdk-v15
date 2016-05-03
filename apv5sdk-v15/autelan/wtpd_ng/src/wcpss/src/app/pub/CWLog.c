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

#include "CWCommon.h"
#include <syslog.h>
#include <assert.h>


//#define WRITE_STD_OUTPUT 1 

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

static FILE *gLogFile = NULL;
static FILE *gWTPLogFile = NULL;

#ifndef CW_SINGLE_THREAD
	CWThreadMutex gFileMutex;
	CWThreadMutex gWTPFileMutex;
#endif
int gWIDLogdebugLevel = WID_SYSLOG_DEBUG_DEFAULT;
int gWIDLOGLEVEL = WID_SYSLOG_DEFAULT;
extern int debug_print;		

void CWLogInitFile(char *fileName) {
	if(fileName == NULL) {
		CWLog("Wrong File Name for Log File");
	}
	
	if((gLogFile = fopen(fileName, "a")) == NULL) {
		CWLog("%s,Can't open log file:%s.", strerror(errno),fileName);
		//exit(1);
	}
	
	#ifndef CW_SINGLE_THREAD
		if(!CWCreateThreadMutex(&gFileMutex)) {
			CWLog("Can't Init File Mutex for Log");
			//exit(1);
		}
	#endif
}


CWBool checkResetFile()
{
	long fileSize=0;

	if((fileSize=ftell(gLogFile))==-1)
	{
		CWLog("An error with log file occurred: %s", strerror(errno));
		return 0;
	}
	if (fileSize>=gMaxLogFileSize)
	{
		fclose(gLogFile);
		if((gLogFile = fopen(gLogFileName, "w")) == NULL) 
		{
			CWLog("%s,Can't open log file: %s", strerror(errno),gLogFileName);
			return 0;
		}
	}
	return 1;
}


void CWLogCloseFile() {
	#ifndef CW_SINGLE_THREAD
		CWDestroyThreadMutex(&gFileMutex);
	#endif
	
	fclose(gLogFile);
}

__inline__ void CWVLog(const char *format, va_list args) {
	char *logStr = NULL;
	time_t now;
	char *nowReadable = NULL;
		
	if(format == NULL) return;
	
	now = time(NULL);
	nowReadable = ctime(&now);
	
	nowReadable[strlen(nowReadable)-1] = '\0';
	
	// return in case of memory err: we're not performing a critical task
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);
	
	//sprintf(logStr, "[CAPWAP::%s]\t\t %s\n", nowReadable, format);
	sprintf(logStr, "[CAPWAP::%s]\t%08x\t %s\n", nowReadable, (unsigned int)CWThreadSelf(), format);
	char fileLine[256];
	vsnprintf(fileLine, 255, logStr, args);

	if(gLogFile != NULL) {
		
		#ifndef CW_SINGLE_THREAD
			CWThreadMutexLock(&gFileMutex);
			fseek(gLogFile, 0L, SEEK_END);
		#endif
		
	
		if(!checkResetFile()) 
		{
			CWThreadMutexUnlock(&gFileMutex);
			exit (1);
		}
		
		fwrite(fileLine, strlen(fileLine), 1, gLogFile);
		fflush(gLogFile);
		
		#ifndef CW_SINGLE_THREAD
			CWThreadMutexUnlock(&gFileMutex);
		#endif
	}
#ifdef WRITE_STD_OUTPUT
	vprintf(fileLine, args);
#endif	
	
	CW_FREE_OBJECT(logStr);
}

__inline__ void CWLog(const char *format, ...) {
	va_list args;
	
	va_start(args, format);
	if (gEnabledLog)
		{CWVLog(format, args);}
	va_end(args);
}

__inline__ void CWDebugLog(const char *format, ...) {
	#ifdef CW_DEBUGGING
		char *logStr = NULL;
		va_list args;
		time_t now;
		char *nowReadable = NULL;
		
		if (!gEnabledLog) {return;}

		if(format == NULL) {
#ifdef WRITE_STD_OUTPUT
			printf("\n");
#endif
			return;
		}
		
		now = time(NULL);
		nowReadable = ctime(&now);
		
		nowReadable[strlen(nowReadable)-1] = '\0';
		
		// return in case of memory err: we're not performing a critical task
		CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);
		
		//sprintf(logStr, "[[CAPWAP::%s]]\t\t %s\n", nowReadable, format);
		sprintf(logStr, "[CAPWAP::%s]\t%08x\t %s\n", nowReadable, (unsigned int)CWThreadSelf(), format);

		va_start(args, format);
		
		if(gLogFile != NULL) {
			char fileLine[256];
			
			#ifndef CW_SINGLE_THREAD
				CWThreadMutexLock(&gFileMutex);
				fseek(gLogFile, 0L, SEEK_END);
			#endif
			
			vsnprintf(fileLine, 255, logStr, args);

			if(!checkResetFile()) 
			{
				CWThreadMutexUnlock(&gFileMutex);
				//exit (1);
			}

			fwrite(fileLine, strlen(fileLine), 1, gLogFile);
			
			fflush(gLogFile);
			
			#ifndef CW_SINGLE_THREAD
			CWThreadMutexUnlock(&gFileMutex);
			#endif
		}
#ifdef WRITE_STD_OUTPUT	
		vprintf(logStr, args);
#endif
		
		va_end(args);
		CW_FREE_OBJECT(logStr);
	#endif
}

__inline__ void WIDVLog(int level,const char *format, va_list args) {
	char *logStr = NULL;
	time_t now;
	char *nowReadable = NULL;
	char widloglevel[20];	
	if(format == NULL) return;
	
	now = time(NULL);
	nowReadable = ctime(&now);
	
	nowReadable[strlen(nowReadable)-1] = '\0';
	
	// return in case of memory err: we're not performing a critical task
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);
	
	switch(level){
		case WID_SYSLOG_EMERG:
			strcpy(widloglevel, "Emerg");
			break;
			
		case WID_SYSLOG_DEBUG_INFO:
			strcpy(widloglevel, "Info");
			break;
			
		case WID_SYSLOG_CRIT:
			strcpy(widloglevel, "Crit");
			break;

		case WID_SYSLOG_ERR:
			strcpy(widloglevel, "Err");
			break;

		case WID_SYSLOG_WARNING:
			strcpy(widloglevel, "Warning");
			break;

		case WID_SYSLOG_NOTICE:
			strcpy(widloglevel, "Notice");
			break;
			
		case WID_SYSLOG_INFO:
			strcpy(widloglevel, "Info");
			break;
			
		case WID_SYSLOG_DEBUG:
			strcpy(widloglevel, "Debug");
			break;

		case WID_SYSLOG_DEBUG_DEBUG:
			strcpy(widloglevel, "Debug");
			break;
			
		default :
			strcpy(widloglevel, "All");
			break;
	}
	sprintf(logStr, "[CAPWAP::%s]\t%08x\t %s\t%s\n", nowReadable, (unsigned int)CWThreadSelf(),widloglevel,format);

	if(gLogFile != NULL) {
		char fileLine[256];
		
		#ifndef CW_SINGLE_THREAD
			CWThreadMutexLock(&gFileMutex);
			fseek(gLogFile, 0L, SEEK_END);
		#endif
		
		vsnprintf(fileLine, 255, logStr, args);
	
		if(!checkResetFile()) 
		{
			CWThreadMutexUnlock(&gFileMutex);
			exit (1);
		}
		
		fwrite(fileLine, strlen(fileLine), 1, gLogFile);
		fflush(gLogFile);
		
		#ifndef CW_SINGLE_THREAD
			CWThreadMutexUnlock(&gFileMutex);
		#endif
	}
#ifdef WRITE_STD_OUTPUT
	vprintf(logStr, args);
#endif	
	
	CW_FREE_OBJECT(logStr);
}
__inline__ void WID_Log(int level,const char *format, ...) 
{
	va_list args;
	va_start(args, format);
	if (gEnabledLog)
		{
		if ( level == WID_SYSLOG_DEBUG_DEBUG )
			{
			if (gWIDLogdebugLevel & level)
				{
				level = WID_SYSLOG_DEBUG;
				WIDVLog(level,format, args);
				}
			}
		else if ( level == WID_SYSLOG_DEBUG_INFO)
			{
			if (gWIDLogdebugLevel & level)
				{
				level = WID_SYSLOG_DEBUG;
				WIDVLog(level,format, args);}
			}
		else 
			WIDVLog(level,format, args);
		}
	va_end(args);

}
//sz20080927




void wid_syslog_emerg(char *format,...)
	
{
    //char *ident = "WID_EME";
	//int wid_log_level = WID_SYSLOG_EMERG;
	char buf[2048] = {0};

	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	
	
	//openlog(ident, 0, LOG_DAEMON); 
		sprintf(buf,"WID_EME ");
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+8,format,ptr);
		va_end(ptr);
		syslog(LOG_EMERG,buf);

	
	//closelog();
	
}
void wid_syslog_alert(char *format,...)
	
{
    //char *ident = "WID_ALE";
	//int wid_log_level = WID_SYSLOG_ALERT;
	char buf[2048] = {0};

	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	
	//openlog(ident, 0, LOG_DAEMON); 
		sprintf(buf,"WID_ALE ");
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+8,format,ptr);
		va_end(ptr);
		syslog(LOG_ALERT,buf);

	
	//closelog();
	
}
void wid_syslog_crit(char *format,...)
	
{
    //char *ident = "WID_CRI";
	//int wid_log_level = WID_SYSLOG_CRIT;
	char buf[2048] = {0};

	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	
	
	//openlog(ident, 0, LOG_DAEMON); 
		sprintf(buf,"WID_CRI ");
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+8,format,ptr);
		
		va_end(ptr);
		syslog(LOG_CRIT,buf);

	
	//closelog();

}
void wid_syslog_err(char *format,...)
	
{
    //char *ident = "WID_ERR";
	//int wid_log_level = WID_SYSLOG_ERR;
	char buf[2048] = {0};

	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	
	//openlog(ident, 0, LOG_DAEMON); 
		sprintf(buf,"WID_ERR ");
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+8,format,ptr);
		va_end(ptr);
		syslog(LOG_ERR,buf);

	
	//closelog();

}
void wid_syslog_warning(char *format,...)
	
{
    //char *ident = "WID_WAR";
	//int wid_log_level = WID_SYSLOG_WARNING;
	char buf[2048] = {0};

	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	
	//openlog(ident, 0, LOG_DAEMON); 
		sprintf(buf,"WID_WAR ");
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+8,format,ptr);
		va_end(ptr);
		syslog(LOG_WARNING,buf);

	
	//closelog();

}
void wid_syslog_notice(char *format,...)
	
{
    //char *ident = "WID_NOT";
	//int wid_log_level = WID_SYSLOG_NOTICE;
	char buf[2048] = {0};

	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
		

	//openlog(ident, 0, LOG_DAEMON); 
		sprintf(buf,"WID_NOT ");
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+8,format,ptr);
		va_end(ptr);
		syslog(LOG_NOTICE,buf);

	
	//closelog();
	
}
void wid_syslog_info(char *format,...)
	
{
    //char *ident = "WID_INF";
	//int wid_log_level = WID_SYSLOG_INFO;
	char buf[2048] = {0};

	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	
	//openlog(ident, 0, LOG_DAEMON); 
		sprintf(buf,"WID_INF ");
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+8,format,ptr);
		va_end(ptr);
		syslog(LOG_INFO,buf);

	
	//closelog();

}
void wid_syslog_debug_debug(char *format,...)
	
{
    //char *ident = "WID_DEB";
	int wid_log_level = WID_SYSLOG_DEBUG;
	char buf[2048] = {0};

	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	if(gWIDLOGLEVEL & wid_log_level)
	{
		//openlog(ident, 0, LOG_DAEMON);
		sprintf(buf,"WID_DEB ");
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf+8,format,ptr);
		va_end(ptr);
		syslog(LOG_DEBUG,buf);
		//closelog();
	}

}

void wid_syslog_debug_info(char *format,...)
	
{
    char *ident = "WID_DBI";
	int wid_log_level = WID_SYSLOG_DEBUG_INFO;
	char buf[2048] = {0};
	
	
	//first,write to 'ac.log.txt' which is belong to wid itself
	//if 'ac.log.txt' will be removed,delete this step
	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	
	//start syslog writing
	if(gWIDLOGLEVEL & wid_log_level)
	{
		openlog(ident, 0, LOG_DAEMON);
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf,format,ptr);
		va_end(ptr);
		syslog(LOG_DEBUG,buf);
		closelog();
	}
}
void wid_syslog_debug(char *format,...)
	
{
    char *ident = "WID_DBD";
	int wid_log_level = WID_SYSLOG_DEBUG;
	char buf[2048] = {0};

	//first,write to 'ac.log.txt' which is belong to wid itself
	//if 'ac.log.txt' will be removed,delete this step
	//va_list args;
	//va_start(args,format);
	//WIDVLog(wid_log_level,format,args);
	//va_end(args);
	
	//start syslog writing
	if(gWIDLOGLEVEL & wid_log_level)
	{ 
		openlog(ident, 0, LOG_DAEMON);
		va_list ptr;
		va_start(ptr,format);
		vsprintf(buf,format,ptr);
		va_end(ptr);
		syslog(LOG_DEBUG,buf);
		closelog();
	}
}


void CWWTPLogInitFile(char *fileName) {
	if(fileName == NULL) {
		printf("Wrong File Name for Log File");
	}
	
	if((gWTPLogFile = fopen(fileName, "a")) == NULL) {
		printf("Can't open log file: %s", strerror(errno));
//		exit(1);
	}
	
	#ifndef CW_SINGLE_THREAD
		if(!CWCreateThreadMutex(&gWTPFileMutex)) {
			printf("Can't Init File Mutex for Log");
//			exit(1);
		}
	#endif
}

CWBool checkWTPResetFile()
{
	long fileSize=0;

	if((fileSize=ftell(gWTPLogFile))==-1)
	{
		wid_syslog_crit("An error with log file occurred: %s", strerror(errno));
		return 0;
	}
	if (fileSize>=gMaxWTPLogFileSize)
	{
		fclose(gWTPLogFile);
		if((gWTPLogFile = fopen(gWTPLogFileName, "w")) == NULL) 
		{
			wid_syslog_crit("Can't open log file: %s", strerror(errno));
			return 0;
		}
	}
	return 1;
}


void CWWTPLogCloseFile() {
	#ifndef CW_SINGLE_THREAD
		CWDestroyThreadMutex(&gWTPFileMutex);
	#endif
	
	fclose(gWTPLogFile);
}
__inline__ void CWWTPVLog(const char *format, va_list args) {
	char *logStr = NULL;
	time_t now;
	char *nowReadable = NULL;
		
	if(format == NULL) return;
	
	now = time(NULL);
	nowReadable = ctime(&now);
	
	nowReadable[strlen(nowReadable)-1] = '\0';
	
	// return in case of memory err: we're not performing a critical task
	CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);
	
	//sprintf(logStr, "[CAPWAP::%s]\t\t %s\n", nowReadable, format);
	sprintf(logStr, "[CAPWAP::%s]\t%08x\t %s\n", nowReadable, (unsigned int)CWThreadSelf(), format);

	if(gWTPLogFile != NULL) {
		char fileLine[256];
		
		#ifndef CW_SINGLE_THREAD
			CWThreadMutexLock(&gWTPFileMutex);
			fseek(gWTPLogFile, 0L, SEEK_END);
		#endif
		
		vsnprintf(fileLine, 255, logStr, args);
	
		if(!checkWTPResetFile()) 
		{
			CWThreadMutexUnlock(&gWTPFileMutex);
			exit (1);
		}
		
		fwrite(fileLine, strlen(fileLine), 1, gWTPLogFile);
		fflush(gWTPLogFile);
		
		#ifndef CW_SINGLE_THREAD
			CWThreadMutexUnlock(&gWTPFileMutex);
		#endif
	}
#ifdef WRITE_STD_OUTPUT
//	vprintf(logStr, args);
#endif	
	
	CW_FREE_OBJECT(logStr);
}
__inline__ void CWWTPDebugLog(const char *format, ...) {
	va_list args;
	
	va_start(args, format);
	if(debug_print){
		vprintf(format,args);
		printf("\n");
	}
	if (gEnabledLog)
		{CWWTPVLog(format, args);}
	va_end(args);
}
#if 0
__inline__ void CWWTPDebugLog(const char *format, ...) {

		char *logStr = NULL;
		va_list args;
		time_t now;
		char *nowReadable = NULL;
		
		if (!gEnabledLog) {return;}

		if(format == NULL) {
#ifdef WRITE_STD_OUTPUT
			printf("\n");
#endif
			return;
		}
		
		now = time(NULL);
		nowReadable = ctime(&now);
		
		nowReadable[strlen(nowReadable)-1] = '\0';
		
		// return in case of memory err: we're not performing a critical task
		CW_CREATE_STRING_ERR(logStr, (strlen(format)+strlen(nowReadable)+100), return;);
		
		//sprintf(logStr, "[[CAPWAP::%s]]\t\t %s\n", nowReadable, format);
		sprintf(logStr, "[CAPWAP::%s]\t%08x\t %s\n", nowReadable, (unsigned int)CWThreadSelf(), format);

		va_start(args, format);
		
		if(gWTPLogFile != NULL) {
			char fileLine[256];
			
			#ifndef CW_SINGLE_THREAD
				CWThreadMutexLock(&gWTPFileMutex);
				fseek(gWTPLogFile, 0L, SEEK_END);
			#endif
			
			vsnprintf(fileLine, 255, logStr, args);

			if(!checkResetFile()) 
			{
				CWThreadMutexUnlock(&gWTPFileMutex);
				exit (1);
			}

			fwrite(fileLine, strlen(fileLine), 1, gWTPLogFile);
			
			fflush(gWTPLogFile);
			
			#ifndef CW_SINGLE_THREAD
			CWThreadMutexUnlock(&gWTPFileMutex);
			#endif
		}
#ifdef WRITE_STD_OUTPUT	
		vprintf(logStr, args);
#endif
		
		va_end(args);
		CW_FREE_OBJECT(logStr);

}
#endif

