#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>


#define SET_BIT(VALUE, BIT) ((VALUE) |= (1 << (BIT)))
#define CLEAR_BIT(VALUE, BIT) ((VALUE) &= (~(1 << (BIT))))

#define BIT_IS_SET(VALUE, BIT)  ((VALUE) & (1 << BIT))
#define BIT_IS_CLEAR(VALUE, BIT)  (!((VALUE) & (1 << BIT)))



int PopenFile(const char *cmd_str,char *str,int len)
{
   if(cmd_str == NULL ||str == NULL)
	   return 0;
	   
	memset(str, 0, len);		   
	
	FILE *fp = NULL; 

	fp = popen(cmd_str, "r");
	if(fp) 
	{	   
		fgets(str, len, fp);	   
		if(str[strlen(str)-1] == '\n')	   
		{		   
		   str[strlen(str)-1] = '\0';	   
		}	   
		pclose(fp); 	   
		return 1;    
	}  
	else
	{	   
		printf("cmd:%s error!%s\n",cmd_str,strerror(errno)); 	   
		return 0;   
	}
}



static char help_msg[] = "\
Usage: config_eth {-r|-s {10|100|1000} |-d {0|1} |-h }\n";
static void iw_usage(void)
{
    printf("%s", help_msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	int opt = 0;
    char cmd[128] = {'\0'};
    char result[16] = {'\0'};
    
    unsigned int value = 0;
    unsigned int parm = 0;
    
    unsigned short  rate = 0;
    unsigned char duplex = 0;
    unsigned char link = 0;


    
    system("echo 0 > /proc/sys/kernel/printk");
    if((opt = getopt(argc,argv,"rlhs:d:")) != -1) {
        
        /*get the value of the reg*/
        if(!PopenFile("/usr/sbin/ethreg -p 0x00 0x00 | awk -F\"= 0x\" '{print$2}'", result, sizeof(result))){
            printf("/usr/sbin/ethreg -p 0x00 0x00 | awk -F\"= 0x\" '{print$2}' is error\n");
        }
        

        /*form the value*/
        sscanf(result , "%x", &value);

        switch (opt)
        {
            case 'h':
                iw_usage();
            break;
        	case 'r':   // reset phy chip   match to the 15th bit
        	
            	/*check parm*/
                if(argc > 2){
                    goto FAIL;
                }
            	SET_BIT(value,15);
                
        	break;
            
        	case 's':   //set rate  00:10M/s, 01:100M/s, 10:1000M/s, 11:negotiation-0
        	            
            	/*check parm*/
                if(argc > 3){
                    goto FAIL;
                }
                parm = atoi(argv[argc - 1]);
                if( 0 != parm && 10 != parm && 100 != parm && 1000 != parm ){
                    goto FAIL;
                }

                
                /*  
            	            enable auto-negotiation process match to the 8th bit
            	            00:10M/s, 01:100M/s, 10:1000M/s
            	            high bit match to the 6th bit, low bit match to the 13th bit
            	        */     

                if( 0 == parm){
                    SET_BIT(value, 12);
                    CLEAR_BIT(value, 13);                
                    CLEAR_BIT(value, 6);
                }else if( 10 == parm ){
                    CLEAR_BIT(value, 12);
                    CLEAR_BIT(value, 13);                
                    CLEAR_BIT(value, 6);
                }else if( 100 == parm ){
                    CLEAR_BIT(value, 12);
                    SET_BIT(value, 13);
                    CLEAR_BIT(value, 6);
                }else if( 1000 == parm ){
                    CLEAR_BIT(value, 12);
                    CLEAR_BIT(value, 13);
                    SET_BIT(value, 6);
                }

                
        	break;
            
        	case 'd':  

                /*set the duplex    0:half, 1:full  match to the 8th bit*/
                /*check parm*/
                if(argc > 3){
                    goto FAIL;
                }
                
                parm = atoi(argv[argc - 1]);
                if(0 != parm && 1 != parm){
                    goto FAIL;
                }
                
                if(parm){
                    SET_BIT(value, 8);
                }else{
                    CLEAR_BIT(value, 8);
                }
                
        	break;

        	case 'l':  

                if(argc > 2){
                    goto FAIL;
                }

                /*read the status of the reg*/
                if(!PopenFile("/usr/sbin/ethreg -p 0x00 0x11 | awk -F\"= 0x\" '{print$2}'", result, sizeof(result))){
                    printf("/usr/sbin/ethreg -p 0x00 0x11 | awk -F\"= 0x\" '{print$2}' is error\n");
                }

                
                /*form the value*/
                sscanf(result , "%x", &value);
                
                /*get the link status*/
                if( BIT_IS_SET(value, 10) ){
                    printf("the link status is up\n");
                }else{
                    printf("the link status is down\n");
                }
                
                /*get the duplex*/
                if( BIT_IS_SET(value, 13) ){
                    printf("the work mode is full-duplex\n");
                }else{
                    printf("the work mode is half-duplex\n");
                }

                /*get the rate
                
            	                00:10M/s, 01:100M/s, 10:1000M/s
            	                high bit match to the 15th bit, low bit match to the 14th bit
            	                */  
                if(BIT_IS_CLEAR(value, 15) && BIT_IS_CLEAR(value, 14)){ //00:10M/s
                    printf("the rate is 10M/s\n");
                }else if(BIT_IS_CLEAR(value, 15) && BIT_IS_SET(value, 14)){ //01:100M/s
                    printf("the rate is 100M/s\n");
                }else if(BIT_IS_SET(value, 15) && BIT_IS_CLEAR(value, 14)){ //10:1000M/s
                    printf("the rate is 1000M/s\n");
                }else{
                    printf("the rate is error\n");
                }      
        	break;

            
            
        	default: 
                goto FAIL;
        }
        
        /*set the value of the reg*/
        if( 's' == opt || 'd' == opt || 'r' == opt){
            sprintf(cmd, "/usr/sbin/ethreg -p 0x00 0x00=0x%x >> /dev/null 2>&1", value);    
            system(cmd);
        }
    }else{
        goto FAIL;
    }
    
    system("echo 7 > /proc/sys/kernel/printk");
    return 0;
FAIL:
    
    system("echo 7 > /proc/sys/kernel/printk");
    iw_usage();
    return -1;
    
}

