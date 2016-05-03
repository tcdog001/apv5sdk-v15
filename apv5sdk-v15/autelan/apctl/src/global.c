#include "global.h"

/* ------------------------------------------------------------------------------------------------------------------ */
/* define variables */
BOOL debugf;
pthread_mutex_t seq_mutex;

/* ------------------------------------------------------------------------------------------------------------------ */
/* define functions */
/* print debug info */
__inline__ void printf_d(const char *format, ...)
{
	if(debugf == TRUE)
	{
		if(format == NULL)
			return ;
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}
char *get_format_str(const char *str, const char c, const int len)
{
	char *fstr = NULL;
	int str_len = 0;
	int h = 0, t = 0;
	//fstr = (char *)malloc(len+1);
	CREATE_STRING_ERR(fstr,len,MALLOC_ERR_RET(NULL));
/* Begin: Modified by dongzw for fix apctl exit and restart bug 2013-04-10 */
//	if(fstr == NULL)
//		return NULL;
//	memset(fstr, 0, len+1);
/* End: Modified by dongzw for fix apctl exit and restart bug 2013-04-10 */
	str_len = strlen(str);
	if(str_len >= len )
	{
		strncpy(fstr, str, len);
	}
	else
	{
		h = (len-str_len)/2;
		t = h+(len-str_len)%2;

		memset(fstr, c, h);
		memcpy(fstr+h, str, str_len);
		memset(fstr+h+str_len, c, t);
		fstr[h+str_len+t] = '\0';
	}
	return fstr;
}
/* print format debug info */
__inline__ void dprintff(const char *str, const char c, const int len)
{
	char *fstr = NULL;
	if(debugf == TRUE)
	{
		if((str == NULL) || (len < 1))
			return ;
		fstr = get_format_str(str, c, len);
		
		if(fstr == NULL)
		{
			return ;
		}
		printf("\n%s\n", fstr);
		FREE_OBJ(fstr);
	}
}
void print_packet(u8 *buf, int len)
{
	int i = 0;
	int j = 0;
	int m = 0;
	if(debugf == TRUE)
	{
		for(i = 0; i < len; i++)
		{
			printf("%02X ", buf[i]);
			if((i+1)%16 == 0)
			{
				printf("\t[ ");
				for(; j <= i; j++)
				{
					if((isgraph(buf[j]) == 0) || (isalnum(buf[j]) == 0))
						printf(".");
						
					else
						printf("%c", buf[j]);
				}
				printf(" ]\n");
			}
		}
		if(i%16 != 0)
		{
			for(m = i%16; m < 16; m++)
			{
				printf("   ");
			}
			printf("\t[ ");
			for(; j < i; j++)
			{
				if((isgraph(buf[j]) == 0) || (isalnum(buf[j]) == 0))
					printf(".");
					
				else
					printf("%c", buf[j]);
			}
			printf(" ]\n");
		}
	}
}
BOOL low_to_upper(char *str)
{
	int i = 0;
	int str_len = strlen(str);
	if(str == NULL)
		return FALSE;
	for(i = 0; i < str_len; i++)
	{
		if((str[i] >= 'a') && (str[i] <= 'z'))
		{
			str[i] = toupper(str[i]);
		}
		else
			str[i] = str[i];
	}
	str[str_len] = '\0';
	return TRUE;
}
char *popen_fread(char *cmd, char *buf, int len)
{
	char *str_tmp = NULL;
	char *p = NULL;
	FILE *fp = NULL;
	
	if((cmd == NULL) || (buf == NULL))
		return NULL;
	//str_tmp = malloc(len);
	CREATE_STRING_ERR(str_tmp,len,MALLOC_ERR_RET(NULL));
	if(str_tmp == NULL)
	{
		goto err;
	}
	printf_d("CMD: %s\n", cmd);
	fp = popen(cmd, "r");
	if(fp)
	{
		memset(str_tmp, 0, len);
		if(fread(str_tmp, 1, len, fp) > 0)
		{
			if(strlen(str_tmp) < 1)
			{
				pclose(fp);
				goto err;
			}
			if(str_tmp[strlen(str_tmp)-1] == '\n')
			{
				str_tmp[strlen(str_tmp)-1] = '\0';
			}
			else
			{
				str_tmp[strlen(str_tmp)] = '\0';
			}
			p = trim(str_tmp);
			if(p == NULL)
			{
				buf[0] = 0;
			}
			else
				strcpy(buf, p);
			pclose(fp);
		}
		else
		{
			pclose(fp);
			goto err;
		}
	}
	else
	{
		goto err;
	}
	FREE_OBJ(str_tmp);
	return buf;
err:
	FREE_OBJ(str_tmp);
	return NULL;
}

char *popen_fgets(char *cmd, char *str, int len)
{
	FILE *fp=NULL; 
	if((cmd == NULL) || (str == NULL))
	{
		RAISE_ERROR(ERROR_WRONG_ARG, NULL);
	   return NULL;
	}
	printf_d("CMD: %s\t", cmd);
	memset(str, 0, len);
	fp = popen(cmd, "r");
	if(fp == NULL)
	{
		printf_d("cmd:%s error!%s\n", cmd, strerror(errno));	   
		APVLog("cmd:%s error!%s\n", cmd, strerror(errno)); 	   
		return NULL;   
	}
	   
	fgets(str, len, fp);	   
	if(str[strlen(str)-1] == '\n')	   
	{		   
	   str[strlen(str)-1] = '\0';	   
	}	   
	pclose(fp);
	printf_d("result:%s\n", str);	   
	return str;
}
char *trim(char *str)
{
	char *str_dst1 = str;
	char *str_dst2 = str+strlen(str)-1; 
	if(NULL == str)
	{
		printf_d("parameter error");
	}
	while( *str_dst1 == ' ' || *str_dst1 == '\t' )
	{
		str_dst1++;
		if(*str_dst1 == '\0')
		return NULL;
	}
	while( *str_dst2 == ' ' || *str_dst2 == '\t' )
	{
		str_dst2--;
	}
	*(str_dst2+1) = '\0';
	return str_dst1;
}
char *inet_inttostr(char *ipstr, int len, u32 ip)
{
	struct in_addr addr;
	addr.s_addr = htonl(ip);
	if(inet_ntop(AF_INET, &addr, ipstr, len) == NULL)
	{
		printf_d("inet_ntop error: %s\n", strerror(errno));
		return NULL;
	}
	return ipstr;
}
int inet_strtoint(char *ipstr)
{
	struct in_addr addr;
	int ret = 0;
	ret = inet_pton(AF_INET, (const char *)ipstr, (void *)&addr);
	if( ret == -1)
	{
		printf_d("inet_ntop error: %s\n", strerror(errno));
		return 0;
	}
	else if(ret == 0)
	{
		printf_d("inet_pton second argument is NULL\n");
		return 0;
	}
	return ntohl(addr.s_addr);
}
u8 get_sequeue_num()
{
	u8 n = 0;
	static u8 seq_num = 0;
	pthread_mutex_lock(&seq_mutex);
	n = seq_num++;
	pthread_mutex_unlock(&seq_mutex);
	return n;
}
char *fgetsc(char *buf, int len, FILE *fp) 
{
	int i = -1;
	if((buf == NULL) || (fp == NULL) || (len <= 0)) 
	{
		return NULL;
	}
	
	bzero(buf, len);
	
	do 
	{
		if(!feof(fp)) 
		{
			i++;
			buf[i] = getc(fp);
		}
		else  //reach the end of file
			break;
	} while ((i < len) && (buf[i] != '\n') && (buf[i] != '\r'));
	
	if(i == -1) 
	{
		return NULL;
	}
	i++;
	buf[i] = '\0';
	
	return buf;
}

#if 0
BOOL get_current_time()
{
	time_t now;
	struct tm *ctm = NULL;
	struct timeval tv;
	if(time(&now) == -1)
	{
		printf_d("[ time ] get current time failed: %s\n", strerror(errno));
		return FALSE;
	}
	ctm = gmtime(&now);
	if(ctm == NULL)
	{
		printf_d("[gmtime] get current time failed: %s\n", strerror(errno));
		return FALSE;
	}
	if(gettimeofday(&tv, NULL) == -1)
	{
		printf_d("[gettimeofday] get current time failed: %s\n", strerror(errno));
		return FALSE;
	}
	printf_d("Current time: %s\n", ctime(&now)); 
	
	return TRUE;
}
#endif

