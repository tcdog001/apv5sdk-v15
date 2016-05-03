#ifndef WNCQMI_H
#define WNCQMI_H

#ifndef NULL
#define NULL  0
#endif

#define NAS_SUPPORT

//#define WMS_SUPPORT

#define GPS_SUPPORT


#define WDS_DUALSTACK_SUPPORT


#define MSG_DEFAULT_TIMEOUT 5000

//#define debug

#ifdef debug
/* Debug and error messages */
#define WNC_ERR_MSG_0(str)                       fprintf (stderr,str)
#define WNC_ERR_MSG_1(str,arg1)                  fprintf (stderr,str,arg1)
#define WNC_ERR_MSG_2(str,arg1,arg2)             fprintf (stderr,str,arg1,arg2)
#define WNC_ERR_MSG_3(str,arg1,arg2,arg3)        fprintf (stderr,str,arg1,arg2,arg3)
#define WNC_ERR_MSG_4(str,arg1,arg2,arg3,arg4)   fprintf (stderr,str,arg1,arg2,arg3,arg4)

#define WNC_DEBUG_MSG_0(str)                       fprintf (stdout,str)
#define WNC_DEBUG_MSG_1(str,arg1)                  fprintf (stdout,str,arg1)
#define WNC_DEBUG_MSG_2(str,arg1,arg2)             fprintf (stdout,str,arg1,arg2)
#define WNC_DEBUG_MSG_3(str,arg1,arg2,arg3)        fprintf (stdout,str,arg1,arg2,arg3)
#define WNC_DEBUG_MSG_4(str,arg1,arg2,arg3,arg4)   fprintf (stdout,str,arg1,arg2,arg3,arg4)
#else
/* Debug and error messages */
#define WNC_ERR_MSG_0(str)                       do{} while (0)
#define WNC_ERR_MSG_1(str,arg1)                  do{} while (0)
#define WNC_ERR_MSG_2(str,arg1,arg2)             do{} while (0)
#define WNC_ERR_MSG_3(str,arg1,arg2,arg3)        do{} while (0)
#define WNC_ERR_MSG_4(str,arg1,arg2,arg3,arg4)   do{} while (0)

#define WNC_DEBUG_MSG_0(str)                       do{} while (0)
#define WNC_DEBUG_MSG_1(str,arg1)                  do{} while (0)
#define WNC_DEBUG_MSG_2(str,arg1,arg2)             do{} while (0)
#define WNC_DEBUG_MSG_3(str,arg1,arg2,arg3)        do{} while (0)
#define WNC_DEBUG_MSG_4(str,arg1,arg2,arg3,arg4)   do{} while (0)
#endif


#endif // WNCQMI_H
