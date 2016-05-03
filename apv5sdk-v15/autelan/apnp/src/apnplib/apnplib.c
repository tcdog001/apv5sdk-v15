#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "apnplib.h"
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <ftw.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../src/apnp_log.h"


int _system(char *cmdbuf)
{
	int stat;

    ApnpDebugLog("cmd: %s\n",cmdbuf);
	stat = system(cmdbuf);
	
	if(WIFEXITED(stat))
	{
		if(WEXITSTATUS(stat) == 0)
		{	
			return OK;
		}
		else
		{
			ApnpDebugLog("cmd err:%s %s. code = %d\n", cmdbuf,strerror(errno),WEXITSTATUS(stat)) ;
			return ERROR;
		}
	}

	ApnpDebugLog("cmd err:%s %s.\n", cmdbuf,strerror(errno)) ;
    return ERROR;
}

void _result(int ret,char *result)
{
    memset(result,0,strlen(result));

    ApnpDebugLog("ret = %d\n",ret);
    
    if(ret == OK)
    {
        memcpy(result,SUCCESS,strlen(SUCCESS));
    }
    else
    {
        memcpy(result,CMD_EXE_FAIL,strlen(CMD_EXE_FAIL));
    }
}


int get_file_name(const int fd,char *file_path)  
{  
    int rslt = 0;
    int i;
    char buf[BUFF_SIZE] = {'\0'};  
    char file_buf[MALLOC_SIZE + MALLOC_SIZE - NAME_LEN - 20] = {'\0'};

    if (0 >= fd)
    {  
        return 1;  
    }
    
    snprintf(buf, sizeof (buf), "/proc/self/fd/%d", fd); 

    rslt = readlink(buf, file_buf,sizeof(file_buf));

    if(rslt < 0 || rslt >= sizeof(file_buf))
    {
        return 1;
    }

    file_buf[rslt] = '\0';

    for (i = rslt; i >= 0; i--)
    {
        if (file_buf[i] == '/')
        {
            file_buf[i + 1] = '\0';
            break;
        }
    }

    if(strncmp(file_buf,UDISK,strlen(UDISK)) == 0)
    {
        memcpy(file_path,(file_buf + strlen(UDISK)),strlen(file_buf - strlen(UDISK)));
    }
    else
    {
        return 1;
    }
#if 0
    if(file_buf[0] == '/')
        memcpy(file_path,(file_buf + 1),strlen(file_buf - 1));
    else
        memcpy(file_path,file_buf,strlen(file_buf));
#endif
    return 0;  
}  


int IsFileExist(const char* fileName)
{
    return (access(fileName, 06));
}

/** return: -1 no exist;0 exist;1 downing;2 downing error**/
int FTP_Tmp_File_Check(char* filepath,int flag)
{
    int ret = 0,i,len = 0;
    long long file_total_size = 0;
    char * filename = NULL;
    char tmpfile[BUFF_SIZE] = {0};
    char tmppath[BUFF_SIZE] = {0};
    char cmdbuf[BUFF_SIZE] = {0};
    char tmpbuf[BUFF_SIZE] = {0};

    for(i = 0;i < strlen(filepath);i++)
    {
        if(filepath[i] == '/')
        {
            len = i + 1;
        }
    }
    filename = filepath + len;
    
    if(len != 0)
    {
        memcpy(tmppath,filepath,(len-1));
        sprintf(tmpfile, "%s/.%s",tmppath,filename);
    }
    else
    {
        sprintf(tmpfile, ".%s",filepath);
    }

    ret = access(tmpfile, 0);

    ApnpDebugLog("tmpfile = %s ret = %d flag = %d\n",tmpfile,ret,flag);

    if((flag == 1) && (ret == 0))
    {
        memset(cmdbuf,0,sizeof(cmdbuf));
        sprintf(cmdbuf, "cat '%s'",tmpfile);
        
        FILE *fp = NULL;
    	if (NULL != (fp = popen(cmdbuf, "r")))
    	{
            fgets(tmpbuf,256,fp);
    		pclose(fp);
            tmpbuf[strlen(tmpbuf) - 1] = '\0';
    	} 
    	else
    	{
    	    ret = -1;
    	}

        file_total_size = atoll(tmpbuf);

        if(file_total_size == -1)
        {
            memset(cmdbuf,0,sizeof(cmdbuf));
            sprintf(cmdbuf, "rm -rf '%s'",tmpfile);
            _system(cmdbuf);
            ret = 2;
        }
        else
        {
            ret = 1;
        }
    }

    return ret;
}

int IsDirExist(const char* filename)
{
    struct stat statbuf;
    int result;

    result = stat(filename, &statbuf);
    if( result != 0 )
    {
        perror( "Problem getting information" );
        switch (errno)
        {
            case ENOENT:
                ApnpDebugLog("File %s not found.\n", filename);
                break;
                
            case EINVAL:
                ApnpDebugLog("Invalid parameter to _stat.\n");
            break;
            
            default:
                /* Should never be reached. */
                ApnpDebugLog("Unexpected error in stat.\n");
        }   
    }
    else
    {
        if(S_ISDIR(statbuf.st_mode))
        {
            ApnpDebugLog("dir is dir\n");
        }
        if(S_ISREG(statbuf.st_mode))
        {
            ApnpDebugLog("file is file\n");
        }
    }
    return 0;
}

void get_udisk_path(char *path,char *filepath)
{

    if(filepath[0] == '/')
    {
        sprintf(path, "%s%s",UDISK,filepath);
    }
    else
    {
        sprintf(path, "%s/%s",UDISK,filepath);
    }

    ApnpDebugLog("get_udisk_path:%s\n",path);
    
    return;
}


int get_path_name_len(char *dirpath)
{
    int i,len = 0,ret = 0,name_len = 0;

    if(dirpath[strlen(dirpath) - 1] == '/')
    {
        name_len = strlen(dirpath) - 1;
    }
    else
    {
        name_len = strlen(dirpath);
    }
    
    for(i = 0;i < name_len;i++)
    {
        if(dirpath[i] == '/')
        {
            len = i + 1;
        }
    }

    ret = strlen(dirpath) - len;

    return ret;
}

int apnp_ftp_cmd(int control_sockfd,char *sendline,char *recvline)
{

    int ret = 0;
    ApnpDebugLog("send :%s\n",sendline);
    memset(recvline,0,MAXSIZE);
    
    ret = send(control_sockfd,sendline,strlen(sendline),0);
    if(ret == -1)
    {
        ApnpDebugLog("send is wrong\n");
        return 1;
    }
    
    ret = recv(control_sockfd,recvline,MAXSIZE,0);
    if(ret == -1)
    {
        ApnpDebugLog("recv is wrong\n");
        return 2;
    }
    ApnpDebugLog("ret = %d recv :%s\n",ret,recvline);
    return 0;
}

int apnp_ftp_init(int *control_sockfd,ftp_host_info_t *ftp_host_info)
{
    char recvdate;
    char recvline[MAXSIZE];

    ApnpDebugLog("apnp_ftp_init\n");
    
    *control_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(*control_sockfd<0)
    {
        ApnpDebugLog("socket is error\n");
        return -1;
    }

    bzero(&(ftp_host_info->serv_addr),sizeof(ftp_host_info->serv_addr));
    ftp_host_info->serv_addr.sin_family = AF_INET;
    ftp_host_info->serv_addr.sin_port = htons(SERV_PORT);
    ftp_host_info->serv_addr.sin_addr.s_addr = inet_addr(ftp_host_info->ip);

    if((connect(*control_sockfd,(SA*)&(ftp_host_info->serv_addr),sizeof(ftp_host_info->serv_addr)))<0)
    {
        ApnpDebugLog("connect is error\n");
        return -1;
    }

    ApnpDebugLog("connect to %s\n",ftp_host_info->ip);
    
    recvdate = recv(*control_sockfd,recvline,sizeof(recvline),0);

    if(recvdate == -1)
    {
        ApnpDebugLog("recvdate is connect error\n");
        return -1;
    }
    else if(strncmp(recvline,"220",3)==0)
    {
        ApnpDebugLog("connect success,pelase enter username\n");
    }
    else 
    {
        ApnpDebugLog("220 connect is error!");
        return -1;
    }
    
    return 0;
}

int apnp_ftp_login_in(int control_sockfd,ftp_host_info_t ftp_host_info)
{

    char sendline[MAXSIZE],recvline[MAXSIZE];

    ApnpDebugLog("apnp_ftp_login_in\n");

    memset(sendline,0,MAXSIZE);
    sprintf(sendline, "USER %s\r\n",ftp_host_info.user);
    if((apnp_ftp_cmd(control_sockfd,sendline,recvline)) != 0)
    {
        ApnpDebugLog("ftp connect is error!");
        return -1;
    }

    if(strncmp(recvline,"331",3)==0)
    {
        ApnpDebugLog("331 please specify the password.\n");
    }
    else
    {
        ApnpDebugLog("recv date is error.\n");
        return -1;
    }

    memset(sendline,0,MAXSIZE);
    sprintf(sendline, "PASS %s\r\n",ftp_host_info.password);

    if((apnp_ftp_cmd(control_sockfd,sendline,recvline)) != 0)
    {
        ApnpDebugLog("ftp connect is error!");
        return -1;
    }

    if(strncmp(recvline,"230",3)==0)
    {
        ApnpDebugLog("login success!\n");
    }
    else
    {
        ApnpDebugLog("FTP password error\n");
        return -1;
    }

    return 0;
}

int apnp_ftp_get_file_size(int control_sockfd,ftp_host_info_t ftp_host_info)
{
    int ret = 0;
    char cmdbuf[BUFF_SIZE] = {0};
    char sendline[MAXSIZE],recvline[MAXSIZE];

    ApnpDebugLog("apnp_ftp_get_file_size\n");

    memset(sendline,0,MAXSIZE);
    
    sprintf(sendline, "SIZE %s\r\n",ftp_host_info.server_path);

    if((apnp_ftp_cmd(control_sockfd,sendline,recvline)) != 0)
    {
        ApnpDebugLog("ftp connect is error!");
        return -1;
    }

    if(strncmp(recvline,"213",3) != 0)
    {
        ApnpDebugLog("FTP server error\n");
        return -1;
    }

    if((recvline[4] < 48) || (recvline[4] > 57))
    {
        ApnpDebugLog("FTP server not found file %s\n",ftp_host_info.server_path);
        return 2;
        
    }

    ApnpDebugLog("file %s size : %s\n",ftp_host_info.server_path,recvline+4);

    if(IsFileExist(ftp_host_info.tmp_path) == 0)
    {
        sprintf(cmdbuf, "rm -rf '%s'",ftp_host_info.tmp_path);
        _system(cmdbuf);
    }
    
    recvline[strlen(recvline)-1] = '\0';
    
    memset(cmdbuf,0,sizeof(cmdbuf));
    sprintf(cmdbuf, "touch '%s';echo %s > '%s'",ftp_host_info.tmp_path,(recvline+4),ftp_host_info.tmp_path);
    ret = _system(cmdbuf);

    return ret;
}


int apnp_ftp_quit(int control_sockfd)
{

    char sendline[MAXSIZE],recvline[MAXSIZE];

    ApnpDebugLog("apnp_ftp_quit\n");
    
    memset(sendline,0,MAXSIZE);
    sprintf(sendline, "QUIT\r\n");

    if((apnp_ftp_cmd(control_sockfd,sendline,recvline)) != 0)
    {
        ApnpDebugLog("ftp connect is error!");
        return -1;
    }

    if(strncmp(recvline,"221",3)==0)
    {
        ApnpDebugLog("221 bye!^_^\n");
    }
    else
    {
        ApnpDebugLog("quit recv is error!\n");
    }
    
    close(control_sockfd);
    return 0;
}

void *apnp_ftp_down(void *arg)
{
    int ret = 0;
    char cmdbuf[BUFF_SIZE] = {0};
    char tmpbuf[MAXSIZE] = {0};
    
    ftp_host_info_t *ftp_host_info = (ftp_host_info_t *)arg;

    ApnpDebugLog("apnp_ftp_down\n");

    memcpy(tmpbuf,ftp_host_info->tmp_path,strlen(ftp_host_info->tmp_path));

    sprintf(cmdbuf, "ftpget -v -u %s -p %s %s %s %s",ftp_host_info->user,\
        ftp_host_info->password,ftp_host_info->ip,ftp_host_info->local_path,ftp_host_info->server_path);

    ApnpDebugLog("cmdbuf: %s\n",cmdbuf);
    
    ret = _system(cmdbuf);

    if(ret != 0)
    {
        memset(cmdbuf,0,sizeof(cmdbuf));
        sprintf(cmdbuf, "echo -1 > '%s'",tmpbuf);
        ret = _system(cmdbuf);
        ApnpDebugLog("ftp download fail\n");
    }
    else
    {
        memset(cmdbuf,0,sizeof(cmdbuf));
        sprintf(cmdbuf, "rm -rf '%s'",tmpbuf);
        ret = _system(cmdbuf);
        ApnpDebugLog("ftp download finish\n");
    }

    return (void*)0;
}

// stores len bytes in the message, increments the current offset in bytes.
int ProtocolStoreStr(char *msgPtr, char *str,int buff_size) 
{
    int i,len;
    
    len = strlen(str);

    for(i = 0; i < buff_size; i++)
    {
        if(msgPtr[i] == '\0')
            break;
    }
    
    //ApnpDebugLog("buf: %s,i = %d ,str: %s ,len:%d\n",msgPtr,i,str,len);

    if(i + len > buff_size)
    {
        memset(msgPtr,0,sizeof(msgPtr));
        memcpy(msgPtr,DEPTH_IS_TOO_HIGH,sizeof(DEPTH_IS_TOO_HIGH));
        ApnpDebugLog("buff is small,i = %d,str = %s,len = %d,size = %d\n",i,str,len,buff_size);
        return 1;
    }

    if(i == 0)
	    bcopy(str+2,&(msgPtr[i]),len-2);
    else
        bcopy(str,&(msgPtr[i]),len);
    
    msgPtr[strlen(msgPtr)] = '\0';

    return 0;
}

int prasefilecount(char *dir,int depth,int flag)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int count = 0;
    
    if((dp = opendir(dir)) == NULL)
    {
        return 0;
    }

    chdir(dir);
    while((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name,&statbuf);
        
        if(S_ISDIR(statbuf.st_mode)) 
        {
            /**//* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;
            /**//* Recurse at a new indent level */  

            if(strncmp(entry->d_name,".",1) == 0)
                continue;

            if(flag == 1)
            {
                count += prasefilecount(entry->d_name,depth+4,flag);
            }
            count ++;
        }
        else
        {
            if(strncmp(entry->d_name,".",1) == 0)
                continue;
            
            count ++;
        }
    }
    chdir("..");
    closedir(dp);
    return count;
}

int scandirectory(char *dir,int depth,char *buf,int buff_size,int flag)
{
    char cmdbuf[BUFF_SIZE] = {0};
    char filepath[BUFF_SIZE] = {0};
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    struct tm *mtime;

    ApnpDebugLog("dir = %s depth = %d\n",dir,depth);

    if((dp = opendir(dir)) == NULL)
    {
        return 1;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL)
    {
        memset(cmdbuf,0,sizeof(cmdbuf));
        memset(filepath,0,sizeof(filepath));
        lstat(entry->d_name,&statbuf);

        /**//* Found a directory, but ignore . and .. */
        if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
            continue;
        /**//* Recurse at a new indent level */  

        if(strncmp(entry->d_name,".",1) == 0)
            continue;

        mtime = localtime(&statbuf.st_mtime);
 
        if(dir[strlen(dir) - 1] != '/')
            sprintf(filepath,"%s/%s",dir,entry->d_name);
        else
            sprintf(filepath,"%s%s",dir,entry->d_name);

        ApnpDebugLog("filepath = %s\n",filepath);
        
        if(S_ISDIR(statbuf.st_mode)) 
        {

            sprintf(cmdbuf,"/%s/1/%ld/%d-%d-%d %d:%d",(filepath+strlen(UDISK)),statbuf.st_size,\
            mtime->tm_year+1900,mtime->tm_mon+1,mtime->tm_mday,mtime->tm_hour,mtime->tm_min);

            if(ProtocolStoreStr(buf,cmdbuf,buff_size) != 0)
            {
                chdir("..");
                closedir(dp);
                return 1;  
            }
            if(flag == 1)
            {
                if(scandirectory(filepath,depth+4,buf,buff_size,flag) != 0)
                {
                    return 1;
                }
            }
        }
        else
        {
            sprintf(cmdbuf,"/%s/0/%ld/%d-%d-%d %d:%d",(filepath+strlen(UDISK)),statbuf.st_size,\
            mtime->tm_year+1900,mtime->tm_mon+1,mtime->tm_mday,mtime->tm_hour,mtime->tm_min);
            
            if(ProtocolStoreStr(buf,cmdbuf,buff_size) != 0)
            {
                chdir("..");
                closedir(dp);
                return 1; 
            }
        }
    }
    chdir("..");
    closedir(dp);
    return 0;
}

//char *GetList(const char *dirname,const char *flag)
char *GetList(va_list argp)
{
    char *buf,*para,*dirname = NULL,*flag = NULL;
    int file_count = 0,mode = 0,buff_size = 0,argno = 0;
    char pwdpath[BUFF_SIZE] = {0};
    char udisk_path[MAXSIZE] = {0};
    
    ApnpDebugLog("GetList\n");

    if(argp == NULL)
    {
        CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }
    
    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %s\n",argno,para); 
        argno++; 

        if(argno > 2)
        {
            CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);
            COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
        }
        
        if(argno == 1)
            dirname = para;
        if(argno == 2)
            flag = para;
    } 

    if(dirname == NULL || flag == NULL)
    {
        CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }

    get_udisk_path(udisk_path,dirname);

    DIR *dirp;
    if((dirp = opendir(udisk_path)) == NULL)
    {
        CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);
        COPY_MEMORY_RETURN(buf,DIRECTORY_NOT_EXISTS);
    }

    closedir(dirp);

    if((strlen(flag) == 1) && (flag[0] == '1'))
    {
        mode = 1;
    }
    else if((strlen(flag) == 1) && (flag[0] == '0'))
    {
        mode = 0;
    }
    else
    {
        CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }
    
    ApnpDebugLog("mode = %d\n",mode);

    FILE *fp = NULL;
	if (NULL != (fp = popen("pwd", "r")))
	{
        fgets(pwdpath,256,fp);
		pclose(fp);
        pwdpath[strlen(pwdpath) - 1] = '\0';
	} 
	else
	{
	    COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
	}
    
    file_count = prasefilecount(udisk_path,0,mode);

    ApnpDebugLog("file count = %d\n",file_count);

    if(file_count == 0)
    {
        CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);
        COPY_MEMORY_RETURN(buf,DIRECTORY_IS_EMPTY);
    }

    if(file_count > FILE_MAX_COUNT)
    {
        CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);
        COPY_MEMORY_RETURN(buf,FILE_IS_TOO_MANY);
    }

    buff_size = file_count * MALLOC_SIZE;
    ApnpDebugLog("buff_size = %d dirname =%s\n",buff_size,dirname);
    CREATE_OBJECT_SIZE_ERR(buf, buff_size, return NULL;);
    
    ApnpDebugLog("******* directory scan of %s\n",dirname);
    buf[0] = '\0';
    scandirectory(udisk_path,0,buf,buff_size,mode);
    
    ApnpDebugLog("******* done. bufsize = %d\n",strlen(buf));

    chdir(pwdpath);

    return buf;
}


//char *CreatDir(const char *dirname)
char *CreatDir(va_list argp)
{
    char *buf,*para,*dirpath;
    int argno = 0;
    char udisk_path[BUFF_SIZE] = {0};
    
    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("CreatDir\n");

    if(argp == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }
    
    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %s\n",argno,para); 
        argno++; 

        if(argno > 1)
        {
            COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
        }

        if(argno == 1)
            dirpath = para;
    } 

    if(dirpath == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }

    if(get_path_name_len(dirpath) > NAME_LEN)
    {
        COPY_MEMORY_RETURN(buf,NAME_IS_TOO_LONG);
    }

    get_udisk_path(udisk_path,dirpath);

    FILE *fp;
    if((fp = fopen(udisk_path,"r")) != NULL)
    {
        fclose(fp);
        COPY_MEMORY_RETURN(buf,FILE_EXISTS);
    }

    int ret = mkdir(udisk_path,777);
    _result(ret,buf);
    
    return buf;
}

//char *DelDir(const char *dirname)
char *DelDir(va_list argp)
{
    char *buf,*para,*dirname;
    char cmdbuf[BUFF_SIZE] = {0};
    char udisk_path[BUFF_SIZE] = {0};
    int argno = 0; 

    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("DelDir\n");

    if(argp == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }
    
    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %s\n",argno,para); 
        argno++; 

        if(argno > 1)
        {
            COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
        }
        
        if(argno == 1)
            dirname = para;
    } 
    
    if(dirname == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }

    get_udisk_path(udisk_path,dirname);
    
    DIR *dirp;
    if((dirp = opendir(udisk_path)) == NULL)
    {
        COPY_MEMORY_RETURN(buf,DIRECTORY_NOT_EXISTS);
    }
    else
        closedir(dirp);

    sprintf(cmdbuf, "rm -rf '%s'", udisk_path);
    int ret = _system(cmdbuf);
    _result(ret,buf);
    
    return buf;
}

//char *DelFile(const char *filename)
char *DelFile(va_list argp)
{
    char *buf,*para,*filename;
    char udisk_path[BUFF_SIZE] = {0};
    int argno = 0; 

    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("DelFile\n");

    if(argp == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }
    
    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %s\n",argno,para); 
        argno++; 

        if(argno > 1)
        {
            COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
        }
  
        if(argno == 1)
            filename = para;
    } 
    
    if(filename == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }

    get_udisk_path(udisk_path,filename);

    if(IsFileExist(udisk_path) != 0)
    {
        COPY_MEMORY_RETURN(buf,FILE_NOT_EXISTS);
    }

    if(FTP_Tmp_File_Check(udisk_path,1) == 1)
    {
        COPY_MEMORY_RETURN(buf,FILE_IS_DONWING);
    }

    int ret = unlink(udisk_path);
    _result(ret,buf);
    
    return buf;
}


//char *CopyFile(const char *filename,const char *dirname)
char *CopyFile(va_list argp)
{
    char *para,*buf,*filename,*dirname;
    int ret = OK,argno = 0;
    char cmdbuf[BUFF_SIZE] = {0};
    char udisk_file[BUFF_SIZE] = {0};
    char udisk_path[BUFF_SIZE] = {0};

    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("CopyFile\n");

    if(argp == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }
    
    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %s\n",argno,para); 
        argno++; 

        if(argno > 2)
        {
            COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
        }
        
        if(argno == 1)
            filename = para;
        if(argno == 2)
            dirname = para;
    } 

    if((filename == NULL)||(dirname == NULL))
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }

    get_udisk_path(udisk_file,filename);
    get_udisk_path(udisk_path,dirname);

    if(IsFileExist(udisk_file) != 0)
    {
        COPY_MEMORY_RETURN(buf,FILE_NOT_EXISTS);
    }

    if(FTP_Tmp_File_Check(udisk_file,0) >= 0)
    {
        COPY_MEMORY_RETURN(buf,FILE_IS_DONWING);
    }

    DIR *dirp;
    if((dirp = opendir(udisk_path)) == NULL)
    {
        COPY_MEMORY_RETURN(buf,DIRECTORY_NOT_EXISTS);
    }
    else
        closedir(dirp);
    
    sprintf(cmdbuf, "cp '%s' '%s'",udisk_file,udisk_path);
    ret = _system(cmdbuf);
    _result(ret,buf);
    
    return buf;
}


//char *UpFile(const char *ip,const char *username,const char *password,const char *filename,const char *dirname)
char *UpFile(va_list argp)
{

    int argno = 0,len = 0,i = 0,ret = OK; 
    char *para,*buf,*ip,*username,*password,*filename,*dirname,*server_path;
    char local_flie[BUFF_SIZE] = {0};
    char tmp_file[BUFF_SIZE] = {0};
    char cmdbuf[BUFF_SIZE] = {0};
    char udisk_path[BUFF_SIZE] = {0};
    DIR *dirp;

    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("UpFile\n");

    if(argp == NULL)
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);

    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %s\n",argno,para); 
        argno++; 

        if(argno > 5)
        {
            COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
        }
 
        if(argno == 1)
            ip = para;
        if(argno == 2)
            username = para;
        if(argno == 3)
            password = para;
        if(argno == 4)
            server_path = para;
        if(argno == 5)
            dirname = para;
    } 
    
    if((ip == NULL)||(username == NULL)||(password == NULL)||(server_path == NULL)||(dirname == NULL))
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }

    for(i = 0;i < strlen(server_path) ;i++)
    {
        if(server_path[i] == '/')
        {
            len = i + 1;
        }
    }

    filename = server_path + len;

    if(strlen(filename) > NAME_LEN)
    {
        COPY_MEMORY_RETURN(buf,NAME_IS_TOO_LONG);
    }

    get_udisk_path(udisk_path,dirname);
    
    if((dirp = opendir(udisk_path)) == NULL)
    {
        if(get_path_name_len(dirname) > NAME_LEN)
        {
            COPY_MEMORY_RETURN(buf,NAME_IS_TOO_LONG);
        }
        
        if(mkdir(udisk_path,777) < 0)
        {
            COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
        }
        //COPY_MEMORY_RETURN(buf,DIRECTORY_NOT_EXISTS);
    }
    else
        closedir(dirp);

    if(udisk_path[strlen(udisk_path) - 1] == '/')
    {
        sprintf(local_flie, "%s%s",udisk_path,filename);
        sprintf(tmp_file, "%s.%s",udisk_path,filename);
    }
    else
    {
        sprintf(local_flie, "%s/%s",udisk_path,filename);
        sprintf(tmp_file, "%s/.%s",udisk_path,filename);
    }
    
    if(FTP_Tmp_File_Check(local_flie,0) >= 0)
    {
        COPY_MEMORY_RETURN(buf,FILE_IS_DONWING);
    }

    /************** ftp check **************/
    int control_sockfd;
    ftp_host_info_t ftp_host_info;
    
    ftp_host_info.ip = ip;
    ftp_host_info.user = username;
    ftp_host_info.password = password;
    ftp_host_info.local_path = local_flie;
    ftp_host_info.server_path = server_path;
    ftp_host_info.tmp_path = tmp_file;

    ApnpDebugLog("ip = %s\n",ftp_host_info.ip);
    ApnpDebugLog("user = %s\n",ftp_host_info.user);
    ApnpDebugLog("password = %s\n",ftp_host_info.password);
    ApnpDebugLog("local_path = %s\n",ftp_host_info.local_path);
    ApnpDebugLog("server_path = %s\n",ftp_host_info.server_path);
    ApnpDebugLog("tmp_path = %s\n",ftp_host_info.tmp_path);
    
    if((apnp_ftp_init(&control_sockfd,&ftp_host_info)) != 0)
    {
        COPY_MEMORY_RETURN(buf,FTP_INIT_FAIL);
    }

    if((apnp_ftp_login_in(control_sockfd,ftp_host_info)) != 0)
    {
        COPY_MEMORY_RETURN(buf,FTP_LOGIN_FAIL);
    }

    ret = apnp_ftp_get_file_size(control_sockfd,ftp_host_info);
    if(ret < 0)
    {
        apnp_ftp_quit(control_sockfd);
        COPY_MEMORY_RETURN(buf,FTP_INIT_FAIL);
    }
    else if(ret == 2)
    {
        apnp_ftp_quit(control_sockfd);
        COPY_MEMORY_RETURN(buf,FTP_NOT_FOUND_FLIE);
    }

    apnp_ftp_quit(control_sockfd);

    /************** thread ***************/
    pthread_t thread;

    if(pthread_create(&thread, NULL, apnp_ftp_down, &ftp_host_info) != 0)
    {
        memset(cmdbuf,0,sizeof(cmdbuf));
        sprintf(cmdbuf, "rm -rf '%s'",ftp_host_info.tmp_path);
        _system(cmdbuf);
        COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
    }
    else
    {
        COPY_MEMORY_RETURN(buf,SUCCESS);
    }
    
    return buf;
}

//char *GetUpFileProgress(const char *filepath,const char *filesize)
char *GetUpFileProgress(va_list argp)
{
    struct stat statbuf;
    int result,i,len = 0,argno = 0;
    char *filepath = NULL,*filename = NULL,*buf = NULL,*para = NULL;
    long long file_size = 0,file_total_size = 0;
    char tmpfile[BUFF_SIZE] = {0};
    char tmppath[BUFF_SIZE] = {0};
    char cmdbuf[BUFF_SIZE] = {0};
    char tmpbuf[BUFF_SIZE] = {0};
    char udisk_path[BUFF_SIZE] = {0};
    char udisk_tmp_path[BUFF_SIZE] = {0};
    
    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("GetUpFileProgress\n");

    if(argp == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }
    
    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %s\n",argno,para); 
        argno++; 

        if(argno > 1)
        {
            COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
        }
        
        if(argno == 1)
            filepath = para;
    }
    
    if(filepath == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }

    get_udisk_path(udisk_path,filepath);
    if(IsFileExist(udisk_path) != 0)
    {
        COPY_MEMORY_RETURN(buf,FILE_NOT_EXISTS);
    }

    for(i = 0;i < strlen(filepath);i++)
    {
        if(filepath[i] == '/')
        {
            len = i + 1;
        }
    }

    filename = filepath + len;

    if(strlen(filename) > NAME_LEN)
    {
        COPY_MEMORY_RETURN(buf,NAME_IS_TOO_LONG);
    }

    if(len != 0)
    {
        memcpy(tmppath,filepath,(len-1));
        sprintf(tmpfile, "%s/.%s",tmppath,filename);
    }
    else
    {
        sprintf(tmpfile, ".%s",filepath);
    }

    get_udisk_path(udisk_tmp_path,tmpfile);
    
    ApnpDebugLog("tmpfile = %s\n",udisk_tmp_path);

    if(IsFileExist(udisk_tmp_path) != 0)
    {
        COPY_MEMORY_RETURN(buf,FTP_DOWNLOAD_FINISH);
    }

    sprintf(cmdbuf, "cat '%s'",udisk_tmp_path);
    
    FILE *fp = NULL;
	if (NULL != (fp = popen(cmdbuf, "r")))
	{
        fgets(tmpbuf,256,fp);
		pclose(fp);
        tmpbuf[strlen(tmpbuf) - 1] = '\0';
	} 
	else
	{
	    COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
	}

#if 0
    for(i = 0; i < (strlen(tmpbuf) - 1); i++)
    {
        if((tmpbuf[i] < 48) || (tmpbuf[i] > 57))
        {
            COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
        }
    }
#endif

    file_total_size = atoll(tmpbuf);

    ApnpDebugLog("file total size = %lld\n",file_total_size);

    if(file_total_size == -1)
    {
        ApnpDebugLog("ftp down %s fail\n",filepath);

        memset(cmdbuf,0,sizeof(cmdbuf));
        sprintf(cmdbuf, "rm -rf '%s'",udisk_tmp_path);
         _system(cmdbuf);

        COPY_MEMORY_RETURN(buf,FTP_DOWNLOAD_FAIL);
    }

    result = stat(udisk_path, &statbuf);
    
    if(result != 0)
    {
        ApnpDebugLog("Problem getting information");
        switch (errno)
        {
            case ENOENT:
                COPY_MEMORY_RETURN(buf,FILE_NOT_EXISTS);
                break;
                
            case EINVAL:
                //COPY_MEMORY_RETURN(buf,"Invalid parameter to stat.\n");
                ApnpDebugLog("Invalid parameter to stat\n"); 
                COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
            break;
            
            default:
                /* Should never be reached. */
                //COPY_MEMORY_RETURN(buf,"Unexpected error in stat.\n");
                ApnpDebugLog("Unexpected error in stat.\n"); 
                COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
        }   
    }
    else
    {
        ApnpDebugLog("up_size = %ld file_total_size = %lld\n",statbuf.st_size,file_total_size);

        file_size = (long long)statbuf.st_size;

        ApnpDebugLog("***** %d\n",(int)((file_size * 100) / file_total_size));

        //todo
        sprintf(buf, "/%d",(int)((file_size * 100) / file_total_size));

    }

    return buf;
}


//char *GetFileInfo(const char *filename)
char *GetFileInfo(va_list argp)
{
    struct stat statbuf;
    struct tm *mtime;
    int result,filelen,i,len = 0,argno = 0,fp;
    char *filepath = NULL,*filename = NULL,*buf = NULL,*para = NULL;
    char file_path[BUFF_SIZE] = {'0'};
    char udisk_path[BUFF_SIZE] = {0};
    
    CREATE_OBJECT_SIZE_ERR(buf, (MALLOC_SIZE * 2), return NULL;);

    ApnpDebugLog("GetFileInfo\n");

    if(argp == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }
    
    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %s\n",argno,para); 
        argno++; 

        if(argno > 1)
        {
            COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
        }
        
        if(argno == 1)
            filepath = para;
    }
    
    if(filepath == NULL)
    {
        COPY_MEMORY_RETURN(buf,PARAMETERS_ERROR);
    }

    filelen = strlen(filepath);

    for(i = 0;i < filelen ;i++)
    {
        if(filepath[i] == '/')
        {
            len = i + 1;
        }
    }

    filename = filepath + len;

    if(strlen(filename) > NAME_LEN)
    {
        COPY_MEMORY_RETURN(buf,NAME_IS_TOO_LONG);
    }

    get_udisk_path(udisk_path,filepath);
    
    result = stat(udisk_path, &statbuf);
    
    if(result != 0)
    {
        ApnpDebugLog("Problem getting information");
        switch (errno)
        {
            case ENOENT:
                COPY_MEMORY_RETURN(buf,FILE_NOT_EXISTS);
                break;
                
            case EINVAL:
                //COPY_MEMORY_RETURN(buf,"Invalid parameter to stat.\n");
                ApnpDebugLog("Invalid parameter to stat\n"); 
                COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
            break;
            
            default:
                /* Should never be reached. */
                //COPY_MEMORY_RETURN(buf,"Unexpected error in stat.\n");
                ApnpDebugLog("Unexpected error in stat.\n"); 
                COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
        }   
    }
    else
    {
        if((fp = open(udisk_path,O_RDONLY,0644)) > 0)
        {
        
            if(get_file_name(fp,file_path)) //file_path should be less than BUFF_SIZE - 40
            {
                close(fp);
                ApnpDebugLog("get file name error\n");
                COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
            }
            close(fp);
        }
        else
        {
            //COPY_MEMORY_RETURN(buf,"Problem getting information\n");
            ApnpDebugLog("Problem getting information\n"); 
            COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
        }

        mtime = localtime(&statbuf.st_atime);

        //todo
        sprintf(buf, "%s/%ld/%d-%d-%d %d:%d/%s",filename,statbuf.st_size,\
        mtime->tm_year+1900,mtime->tm_mon+1,mtime->tm_mday,mtime->tm_hour,mtime->tm_min,file_path);

    }

    return buf;
}

//char *GetCapacity()
char *GetCapacity(va_list argp)
{
    char *buf;
    char cmdbuf[BUFF_SIZE] = {0};
    FILE *fp = NULL;
    //int argno = 0; 
    //char *para;
    
    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("GetCapacity\n");

#if 0
    if(argp != NULL)
    {
        COPY_MEMORY_RETURN(buf,"The input parameters are wrong");
    }
#endif

    sprintf(cmdbuf, "df -h| awk -F \" \" '/%s/ {print $3 \"-\" $2}'",UDISK+5);

	if (NULL != (fp = popen(cmdbuf, "r")))
	{
        fgets(buf,256,fp);
		pclose(fp);
	} 
	else
	{
	    COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
	}

    return buf;
}

char *FreeBuffer(va_list argp)
{

    char *buf;
    int argno = 0; 
    char *para;
    
    while (1) 
    { 
        para = va_arg(argp,char *); 
        if (para == NULL) 
            break; 
        ApnpDebugLog("Parameter #%d is: %p\n",argno,para); 
        argno++; 

        if(argno > 1)
        {
            return NULL;
        }

        if(argno == 1)
            buf = para;
    } 

    if(buf)
        free(buf);

    return NULL;
}

char *GetApType(va_list argp)
{
    char *buf;
    char cmdbuf[BUFF_SIZE] = {0};
    
    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("GetApType\n");

#if 0
    if(argp != NULL)
    {
        COPY_MEMORY_RETURN(buf,"The input parameters are wrong");
    }
#endif
    sprintf(cmdbuf, "/usr/sbin/showsysinfo | awk -F \":\" '/Device Type/ {print $2}'");

    FILE *fp = NULL;
	if (NULL != (fp = popen(cmdbuf, "r")))
	{
        fgets(buf,256,fp);
		pclose(fp);
        buf[strlen(buf) - 1] = '\0';
	} 
	else
	{
	    COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
	}

    return buf;
}


char *GetApSN(va_list argp)
{
    char *buf;
    char cmdbuf[BUFF_SIZE] = {0};
    
    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("GetApSN\n");

#if 0
    if(argp != NULL)
    {
        COPY_MEMORY_RETURN(buf,"The input parameters are wrong");
    }
#endif
    sprintf(cmdbuf, "/usr/sbin/showsysinfo | awk -F \":\" '/SN/ {print $2}'");

    FILE *fp = NULL;
	if (NULL != (fp = popen(cmdbuf, "r")))
	{
        fgets(buf,256,fp);
		pclose(fp);
        buf[strlen(buf) - 1] = '\0';
	} 
	else
	{
	    COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
	}

    return buf;
}


char *GetApIP(va_list argp)
{
    char *buf;
    char cmdbuf[BUFF_SIZE] = {0};
    
    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("GetApIP\n");

#if 0
    if(argp != NULL)
    {
        COPY_MEMORY_RETURN(buf,"The input parameters are wrong");
    }
#endif
    sprintf(cmdbuf, "ifconfig default | awk -F \" \" '/inet addr/ {print $2}' | awk -F \":\" '{print $2}'");

    FILE *fp = NULL;
	if (NULL != (fp = popen(cmdbuf, "r")))
	{
        fgets(buf,256,fp);
		pclose(fp);
        buf[strlen(buf) - 1] = '\0';
	} 
	else
	{
	    COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
	}

    return buf;
}


char *GetApMAC(va_list argp)
{
    char *buf;
    char cmdbuf[BUFF_SIZE] = {0};
    
    CREATE_OBJECT_SIZE_ERR(buf, MALLOC_SIZE, return NULL;);

    ApnpDebugLog("GetApMAC\n");

#if 0
    if(argp != NULL)
    {
        COPY_MEMORY_RETURN(buf,"The input parameters are wrong");
    }
#endif
    sprintf(cmdbuf, "/usr/sbin/showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");

    FILE *fp = NULL;
	if (NULL != (fp = popen(cmdbuf, "r")))
	{
        fgets(buf,256,fp);
		pclose(fp);
        buf[strlen(buf) - 1] = '\0';
	} 
	else
	{
	    COPY_MEMORY_RETURN(buf,CMD_EXE_FAIL);
	}

    return buf;
}




#if 0
int ApnpConfigurationApRequest(const char *dev,const char *cmd,const char *opt1,const char *opt2,char buf[])
{

	char str[256] = {0};
    if(opt1 == NULL)
    {
        snprintf(str, sizeof(str)-1, 
    			 "%s %s %s",SHOWDEVINFO,dev,cmd);
    }
    else
    {
    	snprintf(str, sizeof(str)-1, 
    			 "%s %s %s %s",SHOWDEVINFO,dev,cmd,opt1);
    }
    
    ApnpDebugLog("cmd : %s\n",str);
    
	FILE *fp = NULL;
	if (NULL != (fp = popen(str, "r")))
	{
		fgets(buf,256,fp);
		pclose(fp);
        return SUCCESS;
	} 
	else
	{
		ApnpDebugLog("Failed to %s !\n",str);
        return FAIL;
	}

}

#endif

