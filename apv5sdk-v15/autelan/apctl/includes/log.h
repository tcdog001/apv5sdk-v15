#ifndef __LOG_H__
#define __LOG_H__

/* ------------------------------------------------------------------------------------------------------------------ */
/* include header files */
#include <sys/syscall.h>

/* ------------------------------------------------------------------------------------------------------------------ */
/* define macros */
#if 0
#define TMP_LOG_FILE_NAME            "../apctlt.log"
#define LOG_FILE_NAME                "../apctl.log"
#else
#define TMP_LOG_FILE_NAME            "/tmp/apctl.log"
#define LOG_FILE_NAME                "/jffs/apctl.log.txt"
#define MESSAGE_REPORT_FILE_NAME	"/tmp/message_report"	// Add by dongzw for station association message report log 2013-04-09
#endif

/* ------------------------------------------------------------------------------------------------------------------ */
/* declare variables */
extern BOOL log_enable;

extern BOOL APVLog(const char *format, ...);
extern BOOL APLog(const char *format, ...);
extern BOOL OpenLogFile();
extern BOOL OpenVLogFile();
extern BOOL CloseLogFile();
extern BOOL TouchReportSwitchFile();
FILE* LogFileInit(int *line, char *fileName);

#endif
