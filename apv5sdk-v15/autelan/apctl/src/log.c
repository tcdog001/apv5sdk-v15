#include "global.h"

/* ------------------------------------------------------------------------------------------------------------------ */
/* define variables */
BOOL log_enable;
FILE *fp;
FILE *vfp;
FILE *message_report_fp;	// Add by dongzw for station association message report log 2013-04-09
int tmp_log_max;
int log_max;
int message_report_max;		// Add by dongzw for station association message report log 2013-04-09
int tmp_ln;
int ln;
int message_report_ln;		// Add by dongzw for station association message report log 2013-04-09
/* ------------------------------------------------------------------------------------------------------------------ */
/* define functions */

FILE* LogFileInit(int *line, char *fileName) {
	FILE *fp = NULL;
	if(log_enable == TRUE)
	{
		char cmd[64]={0},str[32]={0};
		sprintf(cmd,"cat %s |wc -l",fileName);
		if(popen_fgets(cmd,str,sizeof(str)) != NULL){
			*line = atoi(str);
			printf("%s,%s,has %d lines\n",__func__,fileName,*line);
		}else{
			return NULL;
		}
		
		if(fileName == NULL) {
			printf("Wrong File Name for Log File\n");
		}
		
		if((fp = fopen(fileName, "a")) == NULL) {
			printf("%s,Can't open log file:%s.\n", strerror(errno),fileName);
			//exit(1);
		}
#if 0
#ifndef CW_SINGLE_THREAD
			if(!CWCreateThreadMutex(&gFileMutex)) {
				CWLog("Can't Init File Mutex for Log");
				//exit(1);
			}
#endif
#endif

	}
	return fp;
}
/* open log file */
BOOL OpenLogFile()
{
	if(log_enable == TRUE)
	{
		fp = NULL;
		fp = fopen(LOG_FILE_NAME, "w");
		if(fp == NULL)
		{
			printf_d("fopen log file failed: %s\n", strerror(errno));
			return FALSE;
		}
		ln = 0;
	}
	return TRUE;
}
/* open log file */
BOOL OpenVLogFile()
{
	if(log_enable == TRUE)
	{
		vfp = NULL;
		vfp = fopen(TMP_LOG_FILE_NAME, "w");
		if(vfp == NULL)
		{
			printf_d("fopen log file failed: %s\n", strerror(errno));
			return FALSE;
		}
		tmp_ln = 0;
	}
	return TRUE;
}

/* Begin: Add by dongzw for station association message report log 2013-04-09 */
BOOL open_message_report_file()
{
	if(log_enable == TRUE)
	{
		message_report_fp= NULL;
		message_report_fp= fopen(MESSAGE_REPORT_FILE_NAME, "w");
		if(message_report_fp== NULL)
		{
			printf_d("fopen log file failed: %s\n", strerror(errno));
			return FALSE;
		}
		message_report_ln= 0;
	}
	return TRUE;
}
/* End: Add by dongzw for station association message report log 2013-04-09 */

/* close log file */
BOOL CloseLogFile()
{
	if(fp == NULL)
		return FALSE;
	if(log_enable == TRUE)
	{
		if(fclose(fp) != 0)
		{
			printf_d("fclose log file failed: %s\n", strerror(errno));
			return FALSE;
		}
		if(fclose(vfp) != 0)
		{
			printf_d("fclose vlog file failed: %s\n", strerror(errno));
			return FALSE;
		}
		/* Begin: Add by dongzw for station association message report log 2013-04-09 */
		if(fclose(message_report_fp) != 0)
		{
			printf_d("fclose message report file failed: %s\n", strerror(errno));
			return FALSE;
		}
		/* End: Add by dongzw for station association message report log 2013-04-09 */

	}
	return TRUE;
}

/* write log to /tmp/apctl.log */
BOOL APVLog(const char *format, ...)
{
	char str[512] = {0};
	time_t now;
	char *ct = NULL;
	if(log_enable == TRUE && vfp != NULL)
	{
		time(&now);
		va_list args;
		va_start(args, format);
		ct = ctime(&now);
		ct[strlen(ct)-1] = '\0';
		snprintf(str, 512, "[%s]  %-12u %s", ct, (unsigned int)syscall(SYS_gettid), format);
		vfprintf(vfp, str, args);
		va_end(args);
		fflush(vfp);
		tmp_ln++;
		if(tmp_ln > tmp_log_max)
		{
			fclose(vfp);
			OpenVLogFile();
		}
	}
	return TRUE;
}

/* Begin: Add by dongzw for station association message report log 2013-04-09 */
BOOL message_report_log(const char *format, ...)
{
	char str[512] = {0};
	time_t now;
	char *ct = NULL;
	if(log_enable == TRUE && message_report_fp!= NULL)
	{
		time(&now);
		va_list args;
		va_start(args, format);
		ct = ctime(&now);
		ct[strlen(ct)-1] = '\0';
		snprintf(str, 512, "[%s]  %-12u %s", ct, (unsigned int)syscall(SYS_gettid), format);
		vfprintf(message_report_fp, str, args);
		va_end(args);
		fflush(message_report_fp);
		message_report_ln++;
		if(message_report_ln> message_report_max)
		{
			fclose(message_report_fp);
			open_message_report_file();
		}
	}
	return TRUE;
}
/* End: Add by dongzw for station association message report log 2013-04-09 */

/* write log to /jffs/apctl.log */
BOOL APLog(const char *format, ...)
{
	char str[512] = {0};
	char *ct = NULL;
	time_t now;
	if(log_enable == TRUE && fp != NULL)
	{
		time(&now);
		va_list args;
		va_start(args, format);
		ct = ctime(&now);
		ct[strlen(ct)-1] = '\0';
		snprintf(str, 512, "[%s]  %-12u %s", ct, (unsigned int)syscall(SYS_gettid), format);
		vfprintf(fp, str, args);
		va_end(args);
		fflush(fp);
		ln++;
		if(ln > log_max)
		{
			fclose(fp);
			OpenLogFile();
		}
	}
	return TRUE;
}


BOOL TouchReportSwitchFile()
{
/* Begin: Modified by dongzw for update report_switch file when system upgrade 2013-04-10 */
	SYSTEM("touch /jffs/report_switch");
	FILE *re_fp;
	re_fp = fopen("/jffs/report_switch", "w");
	fprintf(re_fp, "total_report_enable=1\n");
	fprintf(re_fp, "ap_report_enable=1\n");
	fprintf(re_fp, "radio_report_enable=1\n");
	fprintf(re_fp, "sta_report_enable=1\n");
	fprintf(re_fp, "eth_report_enable=1\n");
	fprintf(re_fp, "ath_report_enable=1\n");
	fprintf(re_fp, "rrm_report_enable=1\n");
	fprintf(re_fp, "ap_report_interval=180\n");
	fprintf(re_fp, "eth_report_interval=180\n");
	fprintf(re_fp, "report_switch_line=%d\n", REPORT_SWITCH_LINE);
	fclose(re_fp);
/* End: Modified by dongzw for update report_switch file when system upgrade 2013-04-10 */
	return TRUE;
}

