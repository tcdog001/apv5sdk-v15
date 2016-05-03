#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/* ------------------------------------------------------------------------------------------------------------------ */
/* include header files */
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <dirent.h>
#include "iconv.h"

/* ------------------------------------------------------------------------------------------------------------------ */
/* define structures */
typedef enum
{
	FALSE           = 0,
	TRUE            = 1
}BOOL;
typedef enum
{
	DISABLED        = 0,
	ENABLED         = 1
}TSTATE;
typedef enum
{
	UP        	 	= 0,
	DOWN         	= 1
}WIFI_IF_STATE;

typedef enum
{
	SUCCESSED       = 0,
	FAILED          = 1
}RESULT_CODE;
typedef enum
{
	IPV4           = 0,
	IPV6           = 1
}PROTO_FAMILY;
typedef enum
{
	MODE_11B       = 0x01,
	MODE_11A       = 0x02,
	MODE_11G       = 0x04,
	MODE_11N_5     = 0x08, 
	MODE_11N_24    = 0x10
}WIFI_MODE;

typedef int   sock_t;

/* Begin: Modified by dongzw for signal strength message report 2013-04-11 */
typedef signed char			s8;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;
/* End: Modified by dongzw for signal strength message report 2013-04-11 */

/* ------------------------------------------------------------------------------------------------------------------ */
/* define macros */
#define FORMAT_STR_LEN             64
#define MAC_ADDR_LEN               6
#define	IEEE80211_ADDR_LEN		   6
#define MAX_WLAN_NUM               32
#define MAX_STA_NUM                32
#define	MAX_SEQ_NUM				   255
#define VENDOR_ID                  31656
#define MAX_BUF_SIZE               65536

/* Begin: Modified by dongzw for update report_switch file when system upgrade 2013-04-10 */
/* if file '/jffs/report_switch' add or del line in func 'TouchReportSwitchFile()', change the line num */
#define REPORT_SWITCH_LINE	10
/* End: Modified by dongzw for update report_switch file when system upgrade 2013-04-10 */

#define	IEEE80211_RATE_VAL		   0x7F

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

#ifdef AR11N
#define	IEEE80211_RATE_MAXSIZE	30
#else
#define	IEEE80211_RATE_MAXSIZE	36
#endif

/* Begin: Modified by dongzw for fix apctl exit and restart bug 2013-04-10 */
#define	MALLOC_ERR_RET(ret) {printf_d("%s,%d,malloc failed\n", __func__,__LINE__); APVLog("%s,%d,malloc failed\n", __func__,__LINE__);return ret;}
#define	MALLOC_ERR_GOTO(label) {printf_d("%s,%d,malloc failed\n", __func__,__LINE__); APVLog("%s,%d,malloc failed\n", __func__,__LINE__);goto label;}
#define	CREATE_OBJECT_ERR(obj_name, obj_type, on_err)	{obj_name = (obj_type*)(malloc(sizeof(obj_type))); if(!obj_name) {on_err} else {memset(obj_name, 0, sizeof(obj_type));}}
#define	CREATE_OBJECT_SIZE_ERR(obj_name, obj_size,on_err)	{obj_name = (malloc(obj_size)); if(!obj_name) {on_err} else { memset(obj_name, 0, obj_size); }}
#define	CREATE_ARRAY_ERR(ar_name, ar_size, ar_type, on_err)	{ar_name = (ar_type*) (malloc(sizeof(ar_type) * (ar_size))); if(!(ar_name)) {on_err} else {memset(ar_name, 0, sizeof(ar_type) * (ar_size)); }}
#define	CREATE_STRING_ERR(str_name, str_length, on_err)	{str_name = (char*) (malloc(sizeof(char) * ((str_length)+1) ) ); if(!(str_name)) {on_err} else {memset(str_name, 0, sizeof(char) * ((str_length) + 1)); }}
#define CREATE_STRING_FROM_STRING_ERR(str_name, str, on_err)	{CREATE_STRING_ERR(str_name, strlen(str), on_err); strcpy((str_name), str);}
/* End: Modified by dongzw for fix apctl exit and restart bug 2013-04-10 */

#define FREE_OBJ(obj)              { if(obj != NULL) { free(obj); obj = NULL; } }
#define	FREE_OBJS_ARRAY(ar_name, ar_size)			{int _i = 0; for(_i = ((ar_size)-1); _i >= 0; _i--) {if(((ar_name)[_i]) != NULL){ free((ar_name)[_i]);}} free(ar_name); (ar_name) = NULL; }

#define COPY_MEMORY(dst, src, len)			bcopy(src, dst, len)

#define PRINT_LINE_NUM             {printf_d("%-32s %06d\n", __FILE__, __LINE__);}
#define SYSTEM(cmd)                {printf_d("CMD: %s\n", cmd); APVLog("CMD: %s\n", cmd);system(cmd);}
/* ------------------------------------------------------------------------------------------------------------------ */
/* declare variables */

/* global.c */
extern BOOL debugf;
extern pthread_mutex_t seq_mutex;
/* APCTL.c */
extern sock_t    ctl_sd;
/* ------------------------------------------------------------------------------------------------------------------ */
/* declare functions */

/* global.c */
extern __inline__ void printf_d(const char *format, ...);
extern __inline__ void dprintff(const char *str, const char c, const int len);
extern BOOL low_to_upper(char *str);
extern char *popen_fgets(char *cmd, char *str, int len);
extern char *popen_fread(char *cmd, char *buf, int len);
extern void print_packet(u8 *buf, int len);
extern char *trim(char *str);
extern char *inet_inttostr(char *ipstr, int len, u32 ip);
extern int inet_strtoint(char *ipstr);
extern u8 get_sequeue_num();
extern char *fgetsc(char *buf, int len, FILE *fp);
extern char *get_format_str(const char *str, const char c, const int len);
/* log.c */



/* timelib.c */
//extern BOOL get_current_time();
#include "APCTL.h"
#include "NetWork.h"
#include "log.h"
#include "handle_err.h"
#include "messages.h"
#include "timerlib.h"


/* network.c */
extern BOOL send_message(BOOL (*assemble_func)(void *arg, TMSG *msg), void *arg1);

extern int heartbeat_timer;
extern int echotime;

//for mac debug print, add by diaowq@20120916
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]

extern u32 wireless_if_updown_count[RADIO_NUM];
#define		CWNetworkCloseSocket(x)		{ shutdown(x, SHUT_RDWR); close(x); }

#endif
