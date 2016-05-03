#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include "apnp_log.h"

#define		APNP_MALLOC(str_name, str_length, on_err)	{str_name = (char*) (malloc(sizeof(char) * ((str_length)+1) ) ); if(str_name == NULL) {on_err} else { memset(str_name, 0, sizeof(char) * ((str_length)+1)); }}
#define 	APNP_LOG_FREE(p)  {if(p) {free((p));p = NULL;}}

#define gMaxWTPLogFileSize  30000

#define gEnabledLog		1
#define debug_print		0

static FILE *gApnpLogFile = NULL;


void ApnpLogInitFile(char * fileName)
{
	if(fileName == NULL){
		printf("Wrong File Name for log file\n");
		return;
	}
	if((gApnpLogFile = fopen(fileName,"a")) == NULL){
		printf("Can't open log file:%s\n",strerror(errno));
		return;
	}
	return;
}

void ApnpLogCloseFile()
{
	fclose(gApnpLogFile);
	return;
}
int checkApnpResetFile()
{
	long fileSize=0;
	
	if((fileSize=ftell(gApnpLogFile))==-1)
	{
		printf("An error with log file occurred: %s", strerror(errno));
		return 0;
	}
	if (fileSize>=gMaxWTPLogFileSize)
	{
		fclose(gApnpLogFile);
		if((gApnpLogFile = fopen(gApnpLogFileName, "w")) == NULL) 
		{
			printf("Can't open log file: %s", strerror(errno));
			return 0;
		}
	}
	return 1;
}

__inline__ void ApnpVlog(const char *format,va_list args)
{
	char *logstr = NULL;
	time_t now;
	char *nowstr = NULL;

	if(format == NULL) return ;

	now = time(NULL);
	nowstr = ctime(&now);
	nowstr[strlen(nowstr)-1] = '\0';

	APNP_MALLOC(logstr,(strlen(format) + strlen(nowstr) + 100),return;);

	sprintf(logstr,"[APNP::%s]\t %s\n",nowstr,format);

	if(gApnpLogFile != NULL){
	char fileLine[256];
	vsnprintf(fileLine,255,logstr,args);

	if(!checkApnpResetFile()){
		printf("Reset error\n");
		exit(1);
	 }
	fwrite(fileLine,strlen(fileLine),1,gApnpLogFile);
	fflush(gApnpLogFile);
	}
	APNP_LOG_FREE(logstr);
}

__inline__ void ApnpDebugLog(const char *format,...)
{
	va_list args,argslog;
		
	va_start(args, format);
	if(debug_print){
		vprintf(format,args);
		printf("\n");
	}
	va_end(args);
	va_start(argslog, format);
	if (gEnabledLog)
	{
		ApnpVlog(format, argslog);
	}
	va_end(argslog);


}

