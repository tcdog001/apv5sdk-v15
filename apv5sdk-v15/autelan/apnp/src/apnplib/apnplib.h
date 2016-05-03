
#include <sys/socket.h>
#include <netinet/in.h>


/********************************************************************/
/*                          return define                           */ 
/*  0 : SUCCESS                                                     */
/*  1 : Command execution fail                                      */
/*  2 : The input parameters are wrong                              */
/*  3 : The directory name should be less than NAME_LEN(30)         */
/*  4 : The same file name already exists                           */
/*  5 : The same directory name already exists                      */
/*  6 : The file does not exist                                     */
/*  7 : The directory does not exist                                */
/*  8 : The directory is empty                                      */
/*  9 : Too many files,cannot traverse                              */
/* 10 : The directory depth is too high                             */
/* 11 : FTP is being downloaded,can not operation file              */
/* 12 :                                                             */
/* 13 :                                                             */
/* 14 :                                                             */
/* 15 :                                                             */
/* 16 :                                                             */
/* 17 :                                                             */
/* 18 :                                                             */
/* 19 :                                                             */
/* 20 :                                                             */
/* 21 : FTP connect error                                           */
/* 22 : FTP username or password error                              */
/* 23 : FTP server not found file                                   */
/* 24 : FTP server down file error                                  */
/*                                                                  */
/********************************************************************/

#define SUCCESS                 "0"
#define CMD_EXE_FAIL            "1"
#define PARAMETERS_ERROR        "2"
#define NAME_IS_TOO_LONG        "3"
#define FILE_EXISTS             "4"
#define DIRECTORY_EXISTS        "5"
#define FILE_NOT_EXISTS         "6"
#define DIRECTORY_NOT_EXISTS    "7"
#define DIRECTORY_IS_EMPTY      "8"
#define FILE_IS_TOO_MANY        "9"
#define DEPTH_IS_TOO_HIGH       "10"
#define FILE_IS_DONWING         "11"

#define FTP_INIT_FAIL           "21"
#define FTP_LOGIN_FAIL          "22"
#define FTP_NOT_FOUND_FLIE      "23"
#define FTP_DOWNLOAD_FAIL       "24"

#define FTP_DOWNLOAD_FINISH     "/100"

#define OK 0
#define ERROR    1
#define BUFF_SIZE 256
#define MALLOC_SIZE 128
#define NAME_LEN  30
#define FILE_MAX_COUNT 500


#define SERV_PORT 21
#define MAXSIZE 1024

#define SHOWDEVINFO "./showdevinfo"

#define UDISK "/tmp/udisk"

#define SA struct sockaddr

typedef struct ftp_host_info_s {
    char *ip;
	char *user;
	char *password;
    char *local_path;
    char *server_path;
    char *tmp_path;
	struct sockaddr_in serv_addr;
} ftp_host_info_t;



#define CREATE_OBJECT_SIZE_ERR(obj_name, obj_size,on_err)	{obj_name = (malloc(obj_size)); if(obj_name == NULL) {on_err} else { memset(obj_name, 0, obj_size); }}

#define COPY_MEMORY_RETURN(dst, src)       {memcpy(dst, src, sizeof(src));return dst;}



