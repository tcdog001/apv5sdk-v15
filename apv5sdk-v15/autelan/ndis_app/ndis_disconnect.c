#include     <stdio.h>      
#include     <stdlib.h>     
#include     <unistd.h>     
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>  
#include     <sys/ioctl.h>
#include     <net/if.h>
#include     <sys/socket.h>

#include     "ndis_qmi_service.h"

#define PC_DEBUG
#ifdef PC_DEBUG
    #define cprintf  printf
#else
    #define cprintf(fmt, args...) do { \
        FILE *fp = fopen("/dev/console", "w"); \
        if (fp) { \
            fprintf(fp, fmt, ## args); \
            fclose(fp); \
        } \
    } while (0)  
#endif
#define DISCONNECT_TIME_OUT  3    //seconds

int main()
{
	char version[64] = {0};
	int ret = -1;
	int option = -1;
	ndis_ipinfo pipinfo;
	int disnnecting_timeout = DISCONNECT_TIME_OUT;
	int fd = ndis_open();
	
	
	if(fd == -1)
	{
		cprintf("ndis_open failed.\n");
		return -1;
	}
	ret = ndis_get_status(fd,&pipinfo);	

	if(ret<0)
	{
		cprintf("init failed.\n");
		ndis_close(fd);
		return -1;
	}
	cprintf("ndis disconnect test! \n");
	// disconnect to internet	
	{
		ret = ndis_disconnect(fd);
		if(ret!=0)
		{
			cprintf("ndis_disconnect failed,ret=%d.\n",ret);	
            goto EXIT_PROGRAM;		
		}else
		{
			cprintf("ndis_disconnect success.\n");
		}		
	}
	
	// to release resource.
	while(disnnecting_timeout--)
	{
		ret = ndis_get_status(fd,&pipinfo);
		if(ret!=0)
		{
			cprintf("ndis_get_status failed,ret=%d.\n",ret);					
		}
		usleep(50000);

	}

EXIT_PROGRAM:	
	ndis_close(fd);
	return 0;
}

