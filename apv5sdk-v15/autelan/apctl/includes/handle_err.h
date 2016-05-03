#ifndef __HANDLE_ERR_H__
#define __HANDLE_ERR_H__


typedef enum 
{
	ERROR_SUCCESS             = 1,
	ERROR_OUT_OF_MEMORY       = 2,
	ERROR_WRONG_ARG           = 3,
	ERROR_INTERRUPTED         = 4,
	ERROR_NEED_RESOURCE       = 5,
	ERROR_COMUNICATING        = 6,
	ERROR_CREATING            = 7,
	ERROR_GENERAL             = 8,
	ERROR_OPERATION_ABORTED   = 9,
	ERROR_SENDING             = 10,
	ERROR_RECEIVING           = 11,
	ERROR_INVALID_FORMAT      = 12,
	ERROR_TIME_EXPIRED        = 13,
	ERROR_NONE                = 14
}ErrorCode;


/* declare structures */
typedef struct 
{
	ErrorCode code;
	char msg[256];
	int line;
	char fileName[64];
}HANDLE_ERR_INFO;

/* */
#define RAISE_SYS_ERR(error)	 {					               \
							        char buf[256] = {0};	       \
							        strerror_r(errno, buf, 256);   \
									RAISE_ERROR(error, buf);	   \
									return FALSE;		           \
								 }

#define RAISE_ERROR(code, msg)    raise_err(code, msg, __FILE__, __LINE__)
#define HANDLE_LAST_ERROR(filename, line)   handle_last_err(filename, line) 
#define IS_ERR(arg)  ((arg) && (HANDLE_LAST_ERROR(__FILE__, __LINE__)))



extern BOOL raise_err(ErrorCode code, const char *msg, const char *filename, int line);



#endif
