/****************************
 *
 * to use in the user space 
 * with the module wrcpu.o
 *autelan
 *2007.1.10
 ****************************/
#include <auteos.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>   
#include <arpa/inet.h>

#include "pro_ctl_mod_ap83.h"
unsigned int network_arg = 0;
wrcpu_op_args wrcpu_u_op_args;
//product_info network_ioctl_usr;
unsigned char ProductID_buff[PRO_BUF];

unsigned char HWVer_buff[HW_BUF]; 

unsigned char basemac_buff[MAC_BUF];
unsigned char serno_buff[SN_BUF];
unsigned char oeminfo_buff[OEM_BUF];
unsigned char buffer[max_mtd];
unsigned char buffer_read[max_mtd];

unsigned char network_mac_buff[6];
unsigned char network_ip_buff[4];
//ghy add for WNAP550 network_info
#define MAGIC_OFFSET 57616
#define IP_ADDR_OFFSET 57620
#define SUBMASK_OFFSET 57624
#define GW_OFFSET 57628
#define DNS_OFFSET 57632
#define VLAN_OFFSET 57636
#define DHCP_OFFSET 57640

#define KERNEL_NUM_OFFSET 40196
int num_pro;
int get_boardconfig_app(wrcpu_op_args * ops)
{
 	int i;
  /*AQ2000_X added by dengpanhong*/
	#if (defined (AQ2000_X) || defined ( PC018 ) || defined ( AE5000ENT )) && !defined( XH9344 ) && !defined ( XJ9344 )
	int fd_val = autelan_open("/dev/mtdblock7", O_RDWR, 0666);
	#else
#ifdef WNAP550
	int fd_val = autelan_open("/dev/mtdblock8", O_RDWR, 0666);
#else
#ifdef AQ2000N
	int fd_val = autelan_open("/dev/mtdblock7", O_RDWR, 0666);
#else
#if defined(XJPB44_02) || defined(TDPB44_02)
	int fd_val = autelan_open("/dev/mtdblock1", O_RDWR, 0666);
#else
#if defined( ARMADA370 ) || defined ( AP2400 )
	int fd_val = autelan_open("/dev/mtdblock4", O_RDWR, 0666);
#else
#ifdef APM82181
    int fd_val = autelan_open("/dev/mtdblock3", O_RDWR, 0666);
#else
	int fd_val = autelan_open("/dev/mtdblock5", O_RDWR, 0666);
#endif
#endif
#endif
#endif
#endif
#endif
	memset(buffer_read, 0, sizeof(buffer_read));
	autelan_read(fd_val, buffer_read, sizeof(buffer_read));
	autelan_lseek(fd_val, 0, SEEK_SET);

#if (defined(AQ2000N) || defined ( AE5000ENT ) || defined (AQ2000_X) || defined(AQ2000SNH) ||defined (APM82181)|| defined (ARMADA370)|| defined(PC018)) && !defined( XH9344 )&& !defined( XJ9344 )
	char *our_company_name = "Beijing AUTELAN Technology Co., Ltd.";
#else
#if defined(XHPB44) || defined(XHAP93)|| defined(XH9344)
	char *our_company_name = "WUHAN HONGXIN TELECOMMUNICATION TECHNOLOGIES CO.,LTD";
#else
#if defined(AP2400) || defined(XJAP93) || defined(XJAP93_02) || defined(XJPB44_02)|| defined( XJ9344 )
	char *our_company_name = "COMBA TELECOM SYSTEMS LTD ";
#else
#if defined(TDPB44_02)
	char *our_company_name = "DATANG TELECOM TECHNOLOGY CO.,LTD";
	char *our_product_type = "R2000 AP-IN(D)";
#else	
#if defined(WNAP550)
	char *our_company_name = "Netgear Inc.";
	char *our_product_type = "WNAP550";
#else
	char *our_company_name = "Netgear Inc.";
	char *our_product_type = "WNAP210";
#endif
#endif
#endif
#endif
#endif
	autelan_printf("\nBase MAC:");
#if !defined(AP2400) && !defined(WNAP550)
//wangyu add for product information
	#if defined(XJAP93_02) || defined(XHPB44) || defined(XJPB44_02) || defined(TDPB44_02)
		if (buffer_read[MAC_OFFSET]==0xff){
		  autelan_printf(" ");
		}
		else{
		for(i=0;i<6;i++){
		  if(i<5)
			autelan_printf("%02x:",buffer_read[i+MAC_OFFSET]);
		  else
			autelan_printf("%02x",buffer_read[i+MAC_OFFSET]);
		  }
		}
	#else
//wangyu add end
	for(i=0;i<12;i++){
		if(i%2){
			if(i==11)
				autelan_printf("%c",buffer_read[i+MAC_OFFSET]);
			else
				autelan_printf("%c:",buffer_read[i+MAC_OFFSET]);
		}
		else{
				autelan_printf("%c",buffer_read[i+MAC_OFFSET]);
			}
	}
	#endif
#else
	for(i=0;i<6;i++){
		if(i<5)
			autelan_printf("%02x:",buffer_read[i+MAC_OFFSET]);
		else
			autelan_printf("%02x",buffer_read[i+MAC_OFFSET]);
	}
#endif
#ifndef WNAP210 
#ifndef AP2400
 	autelan_printf("\nHD version:");	
	if ( buffer_read[HW_OFFSET]==0xff ){
//		printf("");
	}else{
		for(i=0; i<HW_BUF; i++){
		autelan_printf("%c", buffer_read[i+HW_OFFSET]);
		}
	}
#endif
#ifdef XHPB44
	if (buffer_read[SN_OFFSET]!= 0xff){
		autelan_printf("\nSN:");
		for(i=0; i<SN_BUF; i++){
			autelan_printf("%c", buffer_read[i+SN_OFFSET]);
		}
	}
#else
#ifdef WNAP550
#ifdef AQ2000SNH
	autelan_printf("\nSN:");
	if ( buffer_read[SN_OFFSET+12]==0xff ||buffer_read[SN_OFFSET+13]==0xff ||buffer_read[SN_OFFSET+14]==0xff||buffer_read[SN_OFFSET+15]==0xff ){
		for(i=0; i<13; i++){
		autelan_printf("%c", buffer_read[i+SN_OFFSET]);
		}
	}else{
		for(i=0; i<SN_BUF; i++){
		autelan_printf("%c", buffer_read[i+SN_OFFSET]);
		}
	}
#else	
	autelan_printf("\nSN:");
	for(i=0; i<13; i++){
	autelan_printf("%c", buffer_read[i+SN_OFFSET]);
	}
#endif
#else
autelan_printf("\nSN:");
	if ( buffer_read[SN_OFFSET] != 0xff ){
		for(i=0; i<SN_BUF; i++){
			autelan_printf("%c", buffer_read[i+SN_OFFSET]);
		}
	}
#endif
#endif	
#if defined(XJAP93)||defined(XHAP93) || defined(AQ2000N) 
	autelan_printf("\nOeminfo:");
	if ( buffer_read[OEM_OFFSET] != 0xff ){
		for(i=0; i<OEM_BUF; i++){
			autelan_printf("%c", buffer_read[i+OEM_OFFSET]);
		}
	}
#endif
#ifdef AQ2000SNH
	autelan_printf("\nproduct type is:");
	if (buffer_read[PRO_OFFSET] != 0xff){
		for(i=0;i<PRO_BUF;i++){
			autelan_printf("%c", buffer_read[i+PRO_OFFSET]);
		}
	}
	autelan_printf("\nOeminfo:");
	if(buffer_read[OEM_OFFSET] ==0xff){
		autelan_printf("");
	}else{
		for(i=0;i<OEM_BUF;i++){
			autelan_printf("%c", buffer_read[i+OEM_OFFSET]);
		}
	}
	autelan_printf("\n");
	//printf("\nproduct type is:%s\ncompany name is:%s\n",our_product_type,our_company_name);
	autelan_printf("company name is:%s\n",our_company_name);
#else
#ifdef WNAP550
	autelan_printf("\nproduct type is:");
	if (buffer_read[PRO_OFFSET]==0xff || buffer_read[PRO_OFFSET]==0x00){
		autelan_printf("WNAP550");
	}
	else{
		for(i=0;i<PRO_BUF;i++){
			autelan_printf("%c", buffer_read[i+PRO_OFFSET]);
		}
	}
	autelan_printf("\ncompany name is:%s\n",our_company_name);
#else
#ifdef XHPB44
	if(buffer_read[PRO_OFFSET] ==0xff){
		autelan_printf("\nproduct type is:");
		autelan_printf("");
		autelan_printf("\ncompany name is:%s\n",our_company_name);
	}
	else{
		autelan_printf("\nproduct type is:");
		for(i=0;i<PRO_BUF;i++){
			autelan_printf("%c",buffer_read[i+PRO_OFFSET]);
		}
		autelan_printf("\ncompany name is:%s\n",our_company_name);
	}
#else
	autelan_printf("\nproduct type is:");
	if(buffer_read[PRO_OFFSET] != 0xff){
		for(i=0;i<PRO_BUF;i++){
			autelan_printf("%c",buffer_read[i+PRO_OFFSET]);
		}
	}
	autelan_printf("\ncompany name is:%s\n",our_company_name);
#endif
#endif
#endif
#else
	autelan_printf("\nSN:");
	autelan_printf("\nproduct type is:%s\ncompany name is:%s\n",our_product_type,our_company_name);

#endif
/*************************************************************************/ 
	autelan_close(fd_val);
	return 0;

}
#if defined(XJPB44_02) || defined(TDPB44_02)||defined( AP2400 )
int read_entry_addr(void)
{
	int i;
	unsigned char buffer_entry[65536];
	int fd_tmp = autelan_open("/dev/mtdblock7", O_RDONLY);
	memset(buffer_entry, 0, sizeof(buffer_entry));
	int rdsize = autelan_read(fd_tmp, buffer_entry, sizeof(buffer_entry));
	autelan_printf("entry:");
	for(i=0;i<4;i++){
		autelan_printf("%02x",buffer_entry[i + ENTRY_OFFSET]);
	}
	autelan_printf("\n");
	return 0;
	//printf("rdsize=%d\n",rdsize);
 	//lseek(fd, 0, SEEK_SET);
	
}
#endif

int red_register(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd, WRCPU_IOC_O_, ops);
	if (0 ==retval) {
		autelan_printf("\nioctl success\n");
		return 0;
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	autelan_close(fd);
	return retval;
}


int read_addr(wrcpu_op_args * ops){
	int fd = 0;
	int retval = 0;
	fd = autelan_open("/dev/wrcpu",0);
	
	retval = ioctl (fd,WRCPU_IOC_G_,ops);
	if (0 == retval) {
		autelan_printf("\nioctl success\n");
		ops->op_value = (unsigned long) ops->op_value;
	}
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}
	autelan_printf("FD is %d,addr [0x%08lx] Return value is [0x%08lx][%lu]\n",fd,ops->op_addr,ops->op_value,ops->op_value);
	autelan_close(fd);
	return retval;
}
int write_addr(wrcpu_op_args * ops){
        int fd; 
        int retval;
   	wrcpu_op_args *ops1=NULL;
	fd = autelan_open("/dev/wrcpu",0);
    	//ioctl (fd,WRCPU_IOC_G_,ops1);
	//ops1->op_value |= ops->op_value;	
        retval = ioctl (fd,WRCPU_IOC_X_,ops);
        if (0 ==retval) {
                autelan_printf("\nioctl success\n");
                ops->op_value = (unsigned long) ops->op_value;
        }   
        else {
                autelan_printf("Read failed return [%d]\n",retval);
        }   

        autelan_printf("FD is %d,addr [0x%08lx] write value is [0x%08lx]\n",fd,ops->op_addr,ops->op_value);
        autelan_close(fd);
        return retval;
}
//zhengkun add new  listen_factory_reset function for apv6
#ifdef APM82181
int listen_factory_reset(struct hlds_ctrl_t *ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd, WRCPU_IOC_V6_, ops);
	if (0 ==retval) {
		autelan_printf("\nioctl success\n");
		autelan_printf("factory reset : %d\n",ops->reset);	
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}		
	autelan_close(fd);
	return retval;

}
#else
int listen_factory_reset(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu",0);
	retval = ioctl (fd,WRCPU_IOC_L_,ops);
	if (0 ==retval) {
     	autelan_printf("\nioctl success\n");
 		autelan_printf("factory reset : %d\n",ops->factory_reset_get);
	}
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}
	autelan_close(fd);
	return retval;	
}
#endif
//zhengkun add end

/*ghy add for ap83 led*/
int ap83_led_on(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd, WRCPU_IOC_B_, ops);
	if (0 ==retval) {
		autelan_printf("\nioctl success\n");
		return 0;
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	autelan_close(fd);
	return retval;
}

int ap83_led_down(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd, NETGEAR_DOWN_LED, ops);
	if (0 ==retval) {
		autelan_printf("\nioctl success\n");
		return 0;
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	autelan_close(fd);
	return retval;
}

int up_ap83_led_blink(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd,  NETGEAR_LED_BLINK, ops);
	if (0 ==retval) {
		autelan_printf("\nioctl success\n");
		return 0;
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	autelan_close(fd);
	return retval;	
}
int up_xjpb44_led_blink(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd,  WRCPU_IOC_O_, ops);
	if (0 ==retval) {
		autelan_printf("\nioctl success\n");
		return 0;
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	autelan_close(fd);
	return retval;	
}

int down_ap83_led_blink(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd,  NETGEAR_DOWN_LED, ops);
	if (0 ==retval) {
		autelan_printf("\nioctl success\n");
		return 0;
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	autelan_close(fd);
	return retval;
}

static int read_gpio(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu",0);
//	printf("the fd_wrcpu is %d\n",fd);
	
	retval = ioctl (fd,WRCPU_IOC_S_,ops);
	if (0 == retval) {
		//printf("\nioctl success\n");
		if(ops->num == AP83_BUTTON){
			if(ops->status == 0){
				autelan_fprintf(stdout, "\nreset button pushed\n");
			}
			else{
				autelan_fprintf(stdout, "\nreset button released\n");
			}
			
		//ops->op_value = (unsigned long) ops->op_value;
		}
	}
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}

	autelan_close(fd);
	return retval;
}
static int run_watchdog(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu",0);
	
	retval = ioctl (fd,WRCPU_IOC_U_,ops);
	if (0 == retval) {
		autelan_printf("\nioctl success\n");
		//ops->op_value = (unsigned long) ops->op_value;
	}
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}

	autelan_close(fd);
	return retval;

}
static int stop_watchdog(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu",0);
	
	retval = ioctl (fd,WRCPU_IOC_D_,ops);
	if (0 == retval) {
		autelan_printf("\nioctl success\n");
		//ops->op_value = (unsigned long) ops->op_value;
	}
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}

	autelan_close(fd);
	return retval;

}
/*XJ free irq when stop_monitor*/
static int freeirq(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu",0);
	
	retval = ioctl (fd, FREE_IRQ, ops);
	if (0 == retval) {
		autelan_printf("\nioctl success\n");
		//ops->op_value = (unsigned long) ops->op_value;
	}
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}

	autelan_close(fd);
	return retval;
}
static int setirp(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu",0);
	
	retval = ioctl (fd, SET_IRQ, ops);
	if (0 == retval) {
		autelan_printf("%d\n",ops->product_test_flag_app);
		//printf("\nioctl success\n");
		//ops->op_value = (unsigned long) ops->op_value;
	}
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}

	autelan_close(fd);
	return retval;
}

static int read_network_info()
{
	int fd,i;
	fd = autelan_open("/dev/mtdblock8",O_RDWR, 0666);
	memset(buffer_read, 0, sizeof(buffer_read));
	autelan_read(fd, buffer_read, sizeof(buffer_read));
	autelan_lseek(fd, 0, SEEK_SET);
	unsigned int ip_val = *(unsigned int*)(buffer_read+IP_ADDR_OFFSET);
	unsigned int submask_val = *(unsigned int*)(buffer_read+SUBMASK_OFFSET);
	unsigned int gw_val = *(unsigned int*)(buffer_read+GW_OFFSET); 
	unsigned int dns_val = *(unsigned int*)(buffer_read+DNS_OFFSET); 
	autelan_printf("\n");
	autelan_printf("magic      : %d\n", *(unsigned int*)(buffer_read+MAGIC_OFFSET));
	autelan_printf("ip         : %d.%d.%d.%d\n", ip_val>>24,(ip_val>>16)&0xff,(ip_val>>8)&0xff,(ip_val)&0xff);
	autelan_printf("submask    : %d.%d.%d.%d\n",submask_val>>24,(submask_val>>16)&0xff,(submask_val>>8)&0xff,(submask_val)&0xff);
	autelan_printf("gw         : %d.%d.%d.%d\n", gw_val>>24,(gw_val>>16)&0xff,(gw_val>>8)&0xff,(gw_val)&0xff);
	autelan_printf("dns        : %d.%d.%d.%d\n" ,dns_val>>24,(dns_val>>16)&0xff,(dns_val>>8)&0xff,(dns_val)&0xff);
	//printf("ip         : %d.%d.%d.%d\n", buffer_read[57620],buffer_read[57621],buffer_read[57622],buffer_read[57623]);
	autelan_printf("vlan       : %d\n", *(unsigned int*)(buffer_read+VLAN_OFFSET));
	autelan_printf("dhcp       : %d\n", *(unsigned int*)(buffer_read+DHCP_OFFSET));
	return 0;
}

/*wangyu defined the functions for buffer operations*/
int get_the_buff(int fd)
{
	fd = autelan_open(product_mtdblock, O_RDWR, 0666);
	memset(buffer, 0, sizeof(buffer));
	int rdsize = autelan_read(fd , buffer, sizeof(buffer));
	//printf("rdsize=%d\n",rdsize);
	autelan_lseek(fd, 0, SEEK_SET);

	return fd;
}
/*wangyu defined the functions for buffer operations end*/

void copy_to_product(unsigned char *product_str, int fd_tmp)
{
	memset(buffer+PRO_OFFSET+strlen(product_str), 0, PRO_BUF-strlen(product_str));
	//printf("%d\n", PRO_BUFF-strlen(product_str));
	memcpy(buffer+PRO_OFFSET, product_str,strlen(product_str));
	int wrsize = autelan_write(fd_tmp , buffer, sizeof(buffer));
	//printf("wrsize=%d\n",wrsize);
}
void copy_to_hwver(unsigned char *hwver, int fd_tmp)
{
	memset(buffer+HW_OFFSET+strlen(hwver), 0, HW_BUF-strlen(hwver));
	memcpy(buffer+HW_OFFSET, hwver,strlen(hwver));
	autelan_write(fd_tmp, buffer, sizeof(buffer));
}
void hexctohex(unsigned char *hexchar,int n)
{
    int i;
    unsigned char hex[12];
    bzero(hex,sizeof(hex));
    if (n%2!=0)
    {    
        strcat((char *)hexchar,"0");
        n++; 
    }    
//	printf("n=%d\n",n);
	    for(i=0;i<n;i++)
    {    
        /*if(hexchar[i] > 0x39)
            hexchar[i] -= 0x37;
        else
            hexchar[i] -= 0x30;
        if(hexchar[i]>0xf)
            hexchar[i] &=0xf;*/
           // printf("hexchar=%02x   ", hexchar[i]);
           	//printf("********hexchar=0x%x   ", hexchar[i]);

		//printf("#####hexchar=0x%02x   ", hexchar[i]);
        if((hexchar[i] > 0x40)&&(hexchar[i] < 0x47))
            hexchar[i] -= 0x37;
        else if((hexchar[i] > 0x60)&&(hexchar[i] < 0x67))
            hexchar[i] -= 0x57;
        else 
            hexchar[i] -= 0x30;
        if(hexchar[i]>0xf)
            hexchar[i] &=0xf;
	//printf("********hexchar=0x%x   ", hexchar[i]);
	
	//printf("##hexchar=0x%02x   ", hexchar[i]);
    }    
//printf("\n");
    for (i=0;i<n;i+=2)
    {    
        hex[i/2]=hexchar[i];
        hex[i/2]<<=4;
        hex[i/2]|=hexchar[i+1];
//		printf("hex=%02x   ", hex[i/2]);
    }  
//	printf("\n");
    n=n/2;

    memcpy(hexchar,hex,n);
}

void strtounchar_addr(unsigned char *str,unsigned char *mac)
{
    int i;
    unsigned char temp_str[20];

    memset(temp_str,0,20*sizeof(char));
    memcpy(temp_str,str,8);

    hexctohex(temp_str,sizeof(temp_str));
    for(i = 0;i < 4;i++)
    {
        mac[i] = temp_str[i];
    }

}

void strtounchar(unsigned char *str,unsigned char *mac)
{
    int i;
    unsigned char temp_str[20];

    memset(temp_str,0,20*sizeof(char));

    memcpy(temp_str,str,12);
    hexctohex(temp_str,strlen(temp_str));
    for(i = 0;i < 6;i++)
    {
        mac[i] = temp_str[i];
    }
}

void strtounchar_bak(unsigned char *str,unsigned char *mac)
{
    int i;
    unsigned char temp_str[20];

    memset(temp_str,0,20*sizeof(char));
    memcpy(temp_str,str,12);

    hexctohex(temp_str,sizeof(temp_str));
    for(i = 0;i < 6;i++)
    {
        mac[i] = temp_str[i];
    }

}

void str_to_mac_buff(unsigned char *mac_str)
{
#ifdef WNAP550
	strtounchar(mac_str,network_ip_buff);
	strtounchar(mac_str,network_mac_buff);
#else 
#if defined(AP2400) || defined(XJPB44_02) || defined(TDPB44_02)
	strtounchar_addr(mac_str,network_ip_buff);
	strtounchar(mac_str,network_mac_buff);
#else 
	strtounchar(mac_str,network_mac_buff);
#endif
#endif	
}
#if defined(AP2400) || defined(XJPB44_02) || defined(TDPB44_02)
int copy_to_addr(unsigned char *addr_str)
{
	int fd = autelan_open("/dev/mtdblock7", O_RDWR, 0666);
	memset(buffer, 0, sizeof(buffer));
	int rdsize = autelan_read(fd , buffer, sizeof(buffer));
	//printf("rdsize=%d\n",rdsize);
	autelan_lseek(fd, 0, SEEK_SET);

	str_to_mac_buff(addr_str);
	autelan_printf("ENTRY_OFFSET=%d\n",ENTRY_OFFSET);
	//memset(buffer+ENTRY_OFFSET+sizeof(network_mac_buff), 0, ENTRY_BUF - sizeof(network_mac_buff));
	memset(buffer+ENTRY_OFFSET, 0, ENTRY_BUF);
	memcpy(buffer+ENTRY_OFFSET, network_ip_buff, sizeof(network_ip_buff));
	int opt = autelan_write(fd, buffer, sizeof(buffer));
	
	if(opt<0){
		autelan_printf("write entry point wrror\n");
		autelan_close(fd);
		return -1;
	}
	autelan_close(fd);
	return 0;
}
#endif
void copy_to_ap2400basemac(unsigned char *basemac, int fd_tmp)
{

	str_to_mac_buff(basemac);
	memset(buffer+MAC_OFFSET+sizeof(network_mac_buff), 0, MAC_BUF - sizeof(network_mac_buff));
	memcpy(buffer+MAC_OFFSET, network_mac_buff, sizeof(network_mac_buff));
	autelan_write(fd_tmp, buffer, sizeof(buffer));

}
void copy_to_basemac(unsigned char *basemac, int fd_tmp)
{
	#ifdef XJAP93_02
	unsigned char mac[6];
//	printf("basemac= %s\n",basemac);
	strtounchar(basemac,mac);
	memset(buffer+MAC_OFFSET+sizeof(mac), 0, MAC_BUF-sizeof(mac));
	memcpy(buffer+MAC_OFFSET, mac, sizeof(mac));
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	#else
	memset(buffer+MAC_OFFSET+strlen(basemac), 0, MAC_BUF-strlen(basemac));
	memcpy(buffer+MAC_OFFSET, basemac, strlen(basemac));
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	#endif
}
void copy_to_serno(unsigned char *serno, int fd_tmp)
{
	memset(buffer+SN_OFFSET+strlen(serno), 0, SN_BUF-strlen(serno));
	memcpy(buffer+SN_OFFSET,serno,strlen(serno));
	autelan_write(fd_tmp, buffer, sizeof(buffer));
}
void copy_to_oeminfo(unsigned char *oemdata, int fd_tmp)
{
	memset(buffer+OEM_OFFSET+strlen(oemdata), 0, OEM_BUF-strlen(oemdata));
	memcpy(buffer+OEM_OFFSET,oemdata,strlen(oemdata));	
	autelan_write(fd_tmp, buffer, sizeof(buffer));
}

void copy_to_magic(int magic, int fd_tmp)
{
	int i;
	*(unsigned int*)(buffer+MAGIC_OFFSET) = magic;
	autelan_printf("write the flash over now read\n");
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	for(i=0;i<4;i++){
	autelan_printf("%d.",buffer[i+MAGIC_OFFSET]);
	}
	autelan_printf("\n");
}
void copy_to_ipaddr(int ip_addr,int fd_tmp)
{	
	int i;
	autelan_printf("read the value from the flash\n");
	for(i=0;i<4;i++){
	autelan_printf("%d.",buffer[i+IP_ADDR_OFFSET]);
	}
	*(unsigned int*)(buffer+IP_ADDR_OFFSET) = ip_addr;
	autelan_printf("write the flash over now read\n");
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	for(i=0;i<4;i++){
	autelan_printf("%d.",buffer[i+IP_ADDR_OFFSET]);
	}
	autelan_printf("\n");
}
void copy_to_submask(int submask,int fd_tmp)
{
	int i;
	*(unsigned int*)(buffer+SUBMASK_OFFSET) = submask;
	autelan_printf("write the flash over now read\n");
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	for(i=0;i<4;i++){
	autelan_printf("%d.",buffer[i+SUBMASK_OFFSET]);
	}
	autelan_printf("\n");
}
void copy_to_gw(int gw,int fd_tmp)
{
	int i;
	*(unsigned int*)(buffer+GW_OFFSET) = gw;
	autelan_printf("write the flash over now read\n");
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	for(i=0;i<4;i++){
	autelan_printf("%d.",buffer[i+GW_OFFSET]);
	}
	autelan_printf("\n");
}
void copy_to_dns(int dns,int fd_tmp)
{
	int i;
	*(unsigned int*)(buffer+DNS_OFFSET) = dns;
	autelan_printf("write the flash over now read\n");
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	for(i=0;i<4;i++){
	autelan_printf("%d.",buffer[i+DNS_OFFSET]);
	}
	autelan_printf("\n");
}
void copy_to_vlan(int vlan,int fd_tmp)
{
	int i;
	*(unsigned int*)(buffer+VLAN_OFFSET) = vlan;
	autelan_printf("write the flash over now read\n");
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	for(i=0;i<4;i++){
	autelan_printf("%d.",buffer[i+VLAN_OFFSET]);
	}
	autelan_printf("\n");
}
void copy_to_dhcp(int dhcp,int fd_tmp)
{
	int i;
	*(unsigned int*)(buffer+DHCP_OFFSET) = dhcp;
	autelan_printf("write the flash over now read\n");
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	for(i=0;i<4;i++){
	autelan_printf("%d.",buffer[i+DHCP_OFFSET]);
	}
	autelan_printf("\n");
}
int write_flash_char(wrcpu_op_args * ops, int fd_tmp) 
{
	autelan_printf("wrcpu_u_op_args.op_value=%x\n", *value_char);
	autelan_printf("wrcpu_u_op_args.offset=%d\n",ops->offset);
	//wrcpu_u_op_args.op_value;
	*(unsigned char*)(buffer+ops->offset) = *value_char;
	memset(buffer+ops->offset+strlen(value_char), 0, 8-strlen(value_char));
	memcpy(buffer+ops->offset,value_char,strlen(value_char));
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	return 0;
}
int write_flash(wrcpu_op_args * ops, int fd_tmp)
{
	autelan_printf("wrcpu_u_op_args.op_value=%lu\n", value);
	autelan_printf("wrcpu_u_op_args.offset=%d\n",ops->offset);
	//printf("sizeof = %d\n",sizeof(unsigned long));
	//wrcpu_u_op_args.op_value;
	*(unsigned long *)(buffer+ops->offset) = value;
	//memset(buffer+ops->offset+strlen(value), 0, 16-strlen(value));
	//memcpy(buffer+ops->offset,value,strlen(value));
	autelan_write(fd_tmp, buffer, sizeof(buffer));
	return 0;
}

 int compare_to_version(void)
{
	/*40176=flag offset 180=version offset 188=version offset */
	int i;
	unsigned long int opt =*(unsigned long int *)(buffer+40176);
	autelan_printf("opt:%lu\n",opt);
	if(opt == 1){
		autelan_printf("current:");
		for(i=0;i<8;i++){
			autelan_printf("%c",buffer[i+40180]);
		}
		autelan_printf("\n");
		autelan_printf("next:");
		for(i=0;i<8;i++){
		autelan_printf("%c",buffer[i+40188]);
		}
		autelan_printf("\n");
	}else{
		autelan_printf("current:");
		for(i=0;i<8;i++){
			autelan_printf("%c",buffer[i+40188]);
		}
		autelan_printf("\n");
		autelan_printf("next:");
		for(i=0;i<8;i++){
		autelan_printf("%c",buffer[i+40180]);
		}
		autelan_printf("\n");
	}
	return 0;
}
int read_kernel1(void)
{
	autelan_printf("kernel 1 content:\n");
	unsigned long int crc_kernel =*(unsigned long int *)(buffer+40144);
	unsigned long int crc_rootfs =*(unsigned long int *)(buffer+40148);
	unsigned long int kernel_lenth =*(unsigned long int *)(buffer+40160);
	unsigned long int rootfs_lenth =*(unsigned long int *)(buffer+40164);
	autelan_printf("crc_kernel:%lu\n",crc_kernel);
	autelan_printf("crc_rootfs:%lu\n",crc_rootfs);
	autelan_printf("kernel_lenth:%lu\n",kernel_lenth);
	autelan_printf("rootfs_lenth:%lu\n",rootfs_lenth);
	return 0;
}
int read_kernel2(void)
{
	autelan_printf("kernel 2 content:\n");
	unsigned long int crc_kernel =*(unsigned long int *)(buffer+40152);
	unsigned long int crc_rootfs =*(unsigned long int *)(buffer+40156);
	unsigned long int kernel_lenth =*(unsigned long int *)(buffer+40168);
	unsigned long int rootfs_lenth =*(unsigned long int *)(buffer+40172);
	autelan_printf("crc_kernel:%lu\n",crc_kernel);
	autelan_printf("crc_rootfs:%lu\n",crc_rootfs);
	autelan_printf("kernel_lenth:%lu\n",kernel_lenth);
	autelan_printf("rootfs_lenth:%lu\n",rootfs_lenth);
	return 0;
}
int read_kernel_num(void)
{
	unsigned int kernel_num = *(unsigned int *)(buffer+KERNEL_NUM_OFFSET);
	autelan_printf("kernel_num:%u\n",kernel_num);
	return 0;
}
#if defined( AQ2000N )|| defined( PC018 )&& !defined( XH9344 )&& !defined( XJ9344 )
int get_temperature(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu",0);
	
	retval = ioctl (fd, WOC_GET_TEMPUTER, ops);
	autelan_printf("temperature=%d\n",ops->temperature);
	if (0 == retval) {
		//printf("\nioctl success\n");
		;
	}
	else {
		autelan_printf("Read failed return [%d]\n",retval);
	}

	autelan_close(fd);
	return retval;	
}

#endif
//wangyu add the function of pc018
//zhengkun add the function for apv6
int read_temp(void)
{    autelan_system("cat /sys/class/hwmon/hwmon0/device/temp1_input | xargs echo board");
     autelan_system("cat /sys/class/hwmon/hwmon0/device/temp2_input | xargs echo cpu");
     return 0;
}
int apv6_io_fun(struct hlds_ctrl_t *ops)
{int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	//printf("\nfd=%d",fd);
	retval = ioctl(fd, WRCPU_IOC_V6_, ops);
	if (0 ==retval) {
		autelan_printf("\nioctl success\n");
	}else{
		autelan_printf("Read failed return [%d]\n",retval);
		}
		autelan_close(fd);
		return -1;
}
//zhengkun add end
#if defined( PC018 )&& !defined( XH9344 )&& !defined( XJ9344 )
//# if 1
int read_power_in(wrcpu_op_args * ops)
{
	int fd;
		int retval;
		fd = autelan_open("/dev/wrcpu",0);
		
		retval = ioctl (fd, WRCPU_POWER_IN, ops);
		if (0 == retval) {
			//printf("\nioctl success\n");
			if(ops->status == 0){
				autelan_fprintf(stdout, "\nIt's POE power\n");
			}
			else{
				autelan_fprintf(stdout, "\nIt's a adapter\n");
			}
		}
		else {
			autelan_printf("Read failed return [%d]\n",retval);
		}
	
		autelan_close(fd);
		return retval;	

}
int read_power_out(wrcpu_op_args * ops)
{
	int fd;
		int retval;
		fd = autelan_open("/dev/wrcpu",0);
		
		retval = ioctl (fd, WRCPU_POWER_OUT, ops);
		if (0 == retval) {
			//printf("\nioctl success\n");
			if(ops->status == 0){
				autelan_fprintf(stdout, "\nThe power is 3.1v.\n");
			}
			else{
				autelan_fprintf(stdout, "\nThe power is 3.3V .\n");
			}
		}
		else {
			autelan_printf("Read failed return [%d]\n",retval);
		}
	
		autelan_close(fd);
		return retval;	

}

/*begin:wangyu add for apv5 temperature control*/
int set_temperature_up(wrcpu_op_args * ops)
{
	int fd;
		int retval;
		fd = autelan_open("/dev/wrcpu",0);
		
		retval = ioctl (fd, SET_TEMP_UP, ops);
		if (0 ==retval) {
			autelan_printf("\nioctl success\n");
			return 0;
		}	
		else {
			autelan_printf("Read failed return [%d]\n",retval);
			return -1;
		}		
	
		autelan_close(fd);
		return retval;	

}
int set_temperature_down(wrcpu_op_args * ops)
{
	int fd;
		int retval;
		fd = autelan_open("/dev/wrcpu",0);
		
		retval = ioctl (fd, SET_TEMP_DOWN, ops);
		if (0 ==retval) {
			autelan_printf("\nioctl success\n");
			return 0;
		}	
		else {
			autelan_printf("Read failed return [%d]\n",retval);
			return -1;
		}		
	
		autelan_close(fd);
		return retval;	

}
/*end:wangyu add for apv5 temperature control*/

#endif
//wangyu add end
/*Begin:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/
#if defined(AE5000ENT)
int turn_off_all_lights(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd,	TURN_OFF_ALL_LED, ops);
	if (0 ==retval) {
		autelan_printf("\ioctl success\n");
		return 0;
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	autelan_close(fd);
	return retval;
}

int turn_on_all_lights(wrcpu_op_args * ops)
{
	int fd;
	int retval;
	fd = autelan_open("/dev/wrcpu", 0);
	retval = ioctl(fd,	TURN_ON_ALL_LED, ops);
	if (0 ==retval) {
		autelan_printf("\ioctl success\n");
		return 0;
	}	
	else {
		autelan_printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	autelan_close(fd);
	return retval;
}
#endif
/*End:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/

int main(int argc, char **argv) {
	int optype=0; // 1for read, 2 for write;
	int opt;
	int value_to_set = 0;
	int ret;
	int pro_num;
	memset(&wrcpu_u_op_args,0,sizeof(wrcpu_u_op_args));
	wrcpu_u_op_args.op_len = 32;
#ifdef AQ2000N
	int fd_tmp;
	int retval_val;
	fd_tmp = autelan_open("/dev/wrcpu",0);
//	printf("the fs_wrcpu is %d\n",fd_tmp);
	retval_val = ioctl (fd_tmp, SET_IRQ, &wrcpu_u_op_args);
	autelan_close(fd_tmp);
#endif	
	int fd = 0;//wangyu add for all product	

	while ((opt = getopt(argc,argv,"AXYxyRrCwTJKMeBkcgftQqO:I:p:a:H:m:o:v:n:s:d:D:eu:S:G:F:D:V:P:M:iLb:h")) != -1)
	{
		switch (opt) {
			case 'r':
				optype=1;
				break;
			case 'w':
				optype=2;
				break;
			case 'a':
				wrcpu_u_op_args.op_addr = strtoul(optarg,NULL,16);
				break;
			case 'v':
				wrcpu_u_op_args.op_value= strtoul(optarg,NULL,16);
				value_to_set = 1;
				//printf("Input value [%s] got [0x%08x]\n",optarg,wrcpu_u_op_args.op_value);
				break;
			case 'n':/*which led is on 0/1*/
				#ifdef APM82181
				wrcpu_args.device = strtoul(optarg,NULL,10);
			    wrcpu_args.ctrl=1;
				optype=3;
				break;
				#else
				#if defined( ARMADA370 )
				wrcpu_args.device = strtoul(optarg,NULL,10);
			    wrcpu_args.ctrl=2;
				optype=3;
				break;
				#else
				wrcpu_u_op_args.num = strtoul(optarg,NULL,10);
				optype=3;
				break;
				#endif
				#endif
			//case 's':
			//	wrcpu_u_op_args.status = strtoul(optarg,NULL,10);
			//	break;
			case 'i':
				optype = 6;/*read sysinf*/
				break;
			case 'L':
				#ifdef APM82181
				optype = 12;//listen factory reset signal
				wrcpu_args.ctrl=2;
				break;
				#else
				#if defined( ARMADA370 )
				optype = 12;//listen factory reset signal
				wrcpu_args.ctrl=3;
				break;
				#else
				optype = 12;//listen factory reset signal
				break;
				#endif
				#endif

			case 'b':
				#if defined ( APM82181 ) || defined( ARMADA370 )
				wrcpu_args.device = strtoul(optarg,NULL,10);
			    wrcpu_args.ctrl=1;
				optype = 14;
				break;
				#else
				wrcpu_u_op_args.num = strtoul(optarg,NULL,10);
				optype = 14;
				break;
				#endif
			case 'd':
				#if defined ( APM82181 ) || defined( ARMADA370 )
                wrcpu_args.device = strtoul(optarg,NULL,10);
				wrcpu_args.ctrl=0;
				optype = 4;/*down the bilnd*/
				break;
				#else
				wrcpu_u_op_args.num = strtoul(optarg,NULL,10);
				optype = 4;/*down the bilnd*/
				break;
				#endif
			case 'u':
				wrcpu_u_op_args.num = strtoul(optarg,NULL,10);
				optype = 15;
				break;
			case 'f':
				optype = 17;
				break;
			case 'k':
				optype = 18;
				break;
			case 't' :  //stop the watch dog
				optype = 8;
				break;
#if defined(AQ2000N) || defined(PC018) ||defined(APM82181)||defined( XH9344 )||defined( XJ9344 ) || defined (	AQ2000_X) || defined(AE5000ENT)
			case 'p':	
				fd=get_the_buff(fd);
				copy_to_product(optarg,fd);
				//num_pro =  strlen(optarg);
				//printf("pro_num=%d\n", num_pro);
				break;
			case 'H':
				fd=get_the_buff(fd);
				copy_to_hwver(optarg,fd);
				break;
			case 'm':
				fd=get_the_buff(fd);
				copy_to_basemac(optarg,fd);
				break;
			case 's':
				fd=get_the_buff(fd);
				copy_to_serno(optarg,fd);
				break;
			case 'o':
				fd=get_the_buff(fd);
				copy_to_oeminfo(optarg,fd);
				break;
			case 'e':
				if(wrcpu_u_op_args.product_test_flag_app){
					fd=get_the_buff(fd);
					memset(buffer,0,sizeof(buffer));
					autelan_write(fd,buffer,sizeof(buffer));
				}else
				;
				break;
			case 'V':
				value = strtoul(optarg,NULL,10);
				//write_flash(&wrcpu_u_op_args,fd);
				//value = optarg;
				//printf("wrcpu_u_op_args.value=%u\n",value);
				break;
			case 'O':
				fd=get_the_buff(fd);
				wrcpu_u_op_args.offset = strtoul(optarg,NULL,10);
				//printf("offset=%d\n",wrcpu_u_op_args.offset );
				//write_flash(&wrcpu_u_op_args,fd);
				write_flash_char(&wrcpu_u_op_args,fd);
				break;
#if !defined(APM82181)//added  by zhaoenjuan for the question WBS-57 2013-05-08
			case 'S':			
				fd=get_the_buff(fd);
				wrcpu_u_op_args.offset = strtoul(optarg,NULL,10);
                		autelan_printf("offset=%d\n",wrcpu_u_op_args.offset);
				write_flash(&wrcpu_u_op_args,fd);
				break;   
#endif
			case 'C':
				optype = 10;
				break;
			case 'J':
				optype = 23;
				break;
			case 'K':
				optype = 20;
				break;
			case 'P':
				value_char = optarg;
				//write_flash_char(&wrcpu_u_op_args,fd);
				break;
			case 'T':
				optype = 13;
				break;
#endif
/*Begin:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/
#if  defined(AE5000ENT)
            case 'Q':
				optype = 28;
			    break;
			case 'q':
				optype = 29;
			    break;
#endif
/*End:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/

#if  defined(AQ2000N) || defined(APM82181) || defined(PC018) && !defined( XH9344 )&& !defined( XJ9344 )&& !defined( LTEFI_V2 )
			case 'M':
				optype = 21;
				break;
#endif
#if defined(PC018) && !defined( XH9344 )&& !defined( XJ9344 ) //wangyu add the function of PC018
			case 'X':
				optype = 24;
				break;
			case 'Y':
				optype = 25;
				break;
				/*begin:wangyu add for apv5 temperature control*/
			case 'x':
				optype = 26;
				break;
			case 'y':
				optype = 27;
				break;
				/*begin:wangyu add for apv5 temperature control*/
#endif
				//wangyu add end
#if  defined(XJAP93)||defined(XHAP93)||defined( XJAP93_02 )|| defined (ARMADA370)
			case 'p':
				fd=get_the_buff(fd);
				copy_to_product(optarg,fd);
				//num_pro =  strlen(optarg);
				//printf("pro_num=%d\n", num_pro);
				break;
			case 'H':
				fd=get_the_buff(fd);
				copy_to_hwver(optarg,fd);
				break;
			case 'm':
				fd=get_the_buff(fd);
				copy_to_basemac(optarg,fd);
				break;
			case 's':
				fd=get_the_buff(&fd);
				copy_to_serno(optarg,fd);
				break;
			case 'o':
				fd=get_the_buff(fd);
				copy_to_oeminfo(optarg,fd);
				break;
			case 'e':
				fd=get_the_buff(fd);
				memset(buffer,0,sizeof(buffer));
				autelan_write(fd,buffer,sizeof(buffer));
#endif

#if defined( AP2400 ) || defined(XJPB44_02) || defined(TDPB44_02)
			case 'p':
				fd=get_the_buff(fd);
				copy_to_product(optarg,fd);
				//num_pro =  strlen(optarg);
				//printf("pro_num=%d\n", num_pro);
				break;
			case 'm':
				fd=get_the_buff(fd);
				//network_arg = strtoul(optarg,NULL,10);
				copy_to_ap2400basemac(optarg,fd);
				break;
			case 'F' :
				copy_to_addr(optarg);
				break;
			case 'B' :
				optype = 19;
				break;
			case 's':
				fd=get_the_buff(fd);
				copy_to_serno(optarg,fd);
				break;
			case 'g':  //run the watchdog
				optype = 7;
				break;
			case 'c' :
				optype = 16;
				break;
#endif
#ifdef WNAP550
			case 'M':      //write magic
				fd=get_the_buff(fd);
				network_arg = strtoul(optarg,NULL,10);
				copy_to_magic(network_arg,fd);
				break;
			case 'I':     //write ip address
				fd=get_the_buff(fd);
				network_arg = inet_addr(optarg);
				copy_to_ipaddr(network_arg,fd);
				autelan_printf("%x\n",network_arg);
				break;
			case 'S':   //write submask
				fd=get_the_buff(fd);
				network_arg = inet_addr(optarg);
				copy_to_submask(network_arg,fd);
				break;
			case 'G': //write gw
				fd=get_the_buff(fd);
				network_arg = inet_addr(optarg);
				copy_to_gw(network_arg,fd);
				break;
			case 'D':    //write dns
				fd=get_the_buff(fd);
				network_arg = inet_addr(optarg);
				copy_to_dns(network_arg,fd);
				break;
			case 'V':   //write vlan
				fd=get_the_buff(fd);
				network_arg = strtoul(optarg,NULL,10);
				copy_to_vlan(network_arg,fd);
				break;
			case 'P':    //write dhcp
				fd=get_the_buff(fd);
				network_arg = strtoul(optarg,NULL,10);
				copy_to_dhcp(network_arg,fd);
				break;
			case 'R':
				optype = 9;
				break;
			case 'p':
				fd=get_the_buff(fd);
				copy_to_product(optarg,fd);
				//num_pro =  strlen(optarg);
				//printf("pro_num=%d\n", num_pro);
				break;
			case 'm':
				//network_arg = strtoul(optarg,NULL,10);
				fd=get_the_buff(fd);
				copy_to_ap2400basemac(optarg,fd);
				break;
			case 's':
				fd=get_the_buff(fd);
				copy_to_serno(optarg,fd);
				break;
			case 'H':
				fd=get_the_buff(fd);
				copy_to_hwver(optarg,fd);
				break;
			case 'o':
				fd=get_the_buff(fd);
				copy_to_oeminfo(optarg,fd);
				break;
#endif
			case 'h':
#ifndef APM82181
				autelan_printf("pro_ctl_util -r(read register)\n");
				autelan_printf("          -r -a addr (read the addr)\n");
				autelan_printf("          -w -a addr -v value(write value to addr)\n");
				autelan_printf("          -n(read the gpio n's state)\n");
				autelan_printf("          -u(up the led blink)\n");
#endif
				autelan_printf("          -d(down the status led)\n");
				autelan_printf("          -b(up the status led)\n");
				autelan_printf("          -i(get the sysinfo)\n");
				autelan_printf("          -L(listen the button reset)\n");
#if defined(XJAP93)||defined(XJAP93_02)||defined(XHAP93) || defined(AQ2000N) || defined( PC018 ) || defined ( APM82181)|| defined (ARMADA370)|| defined( XH9344 )|| defined( XJ9344 )|| defined (AQ2000_X) || defined( AE5000ENT ) 
				autelan_printf("          -p AQ2000(write productID into the flash)\n");
				autelan_printf("          -H 2.0(write HWversion into the flash)\n");
				autelan_printf("          -m 001B2C3D4E5F(write the basemac into the flash)\n");
				autelan_printf("          -s 0123456(write SN number into the flash)\n");
				autelan_printf("          -o(write OEM info into the flash)\n");
#endif
#if defined(AQ2000N) || defined( APM82181 ) || defined( PC018 ) && !defined( XH9344 )&& !defined( XJ9344 )
				autelan_printf("          -M(read the temperature)\n");
#endif
#if defined(PC018) && !defined( XH9344 )&& !defined( XJ9344 )
				autelan_printf("          -X(read the POE )\n");
				autelan_printf("          -Y(read the power choice)\n");
				autelan_printf("          -y(set_temperature_up )\n");
				autelan_printf("          -z(set_temperature_down )\n");
#endif

#if defined(AP2400) || defined(XJPB44_02) || defined(TDPB44_02)
				autelan_printf("          -p AP2400-IFM(write productID into the flash)\n");
				autelan_printf("          -m 00271f2e3009(write the basemac into the flash)\n");
				autelan_printf("          -s ML2400IFM0000090(write SN number into the flash)\n");
				autelan_printf("          -B(get the entry addr)\n");
				autelan_printf("          -F(Set the entry addr)\n");
#endif
#ifdef WNAP550
				autelan_printf("          -R(read the network_info)\n");
				autelan_printf("     write the net_work info\n");
				autelan_printf("          -M 57474150(read the magic into network_info)\n");	
				autelan_printf("          -I 192.168.6.100 (write the ip addr into network_info)\n");
				autelan_printf("          -S 192.168.6.255 (write the submask into network_info)\n");
				autelan_printf("          -G 192.168.6.1 (write the gw into network_info)\n");
				autelan_printf("          -D  (write the dns into network_info)\n");
				autelan_printf("          -V(write the vlan into network_info)\n");	
				autelan_printf("          -P(write the dhcp into network_info)\n");	
				autelan_printf("     write the product info\n");
				autelan_printf("          -m 001B2C3D4E5F(write the basemac into the flash)\n");
				autelan_printf("          -s 0123456(write SN number into the flash)\n");
				autelan_printf("          -o ""(write OEM info into the flash)\n");
				autelan_printf("          -p AQ2000-SNH(write productID into the flash)\n");
#endif
#if defined (LTEFI)
				autelan_printf("          -t(stop watchdog)\n");
				autelan_printf("          -T(read kernel_num)\n");
#endif
				autelan_printf("\n");
				break;
			case 'D':
				break;
			default :
				autelan_printf("wrong input\n");
		}
	}
	if (1 == optype){
		ret = read_addr(&wrcpu_u_op_args);
	}
	if (2 == optype){
               ret = write_addr(&wrcpu_u_op_args);
        }
	if (3 == optype){
		#if defined ( APM82181 ) || defined( ARMADA370 )//apv7 ioctl use the same logic as apv6
        ret = apv6_io_fun(&wrcpu_args);
		#else
		ret = read_gpio(&wrcpu_u_op_args);
		#endif
	}
	if(4 == optype){
		#if defined ( APM82181 ) || defined( ARMADA370 )//apv7 ioctl use the same logic as apv6
        ret = apv6_io_fun(&wrcpu_args);
		#else
		ret = down_ap83_led_blink(&wrcpu_u_op_args);
        #endif
	}
	if(6 == optype)
		ret = get_boardconfig_app(&wrcpu_u_op_args);
	if(7 == optype){
		ret = run_watchdog(&wrcpu_u_op_args);
	}
	if(8 == optype)
		ret = stop_watchdog(&wrcpu_u_op_args);
	if(12==optype){
		#if defined ( APM82181 ) || defined( ARMADA370 )
		ret = listen_factory_reset(&wrcpu_args);
		#else
		ret = listen_factory_reset(&wrcpu_u_op_args);
		#endif
	}
	/*ghy add for ap83 led*/
	if(14 == optype){
		#if defined ( APM82181 ) || defined( ARMADA370 )
        ret = apv6_io_fun(&wrcpu_args);
		#else
		ret = ap83_led_on(&wrcpu_u_op_args);
        #endif
	}
	if(15 == optype){
		ret = up_ap83_led_blink(&wrcpu_u_op_args);
	}
	if(16 == optype){
		ret = up_xjpb44_led_blink(&wrcpu_u_op_args);
	}
	if(17 == optype){
		ret = freeirq(&wrcpu_u_op_args);
	}
	if(18 == optype){
		ret = setirp(&wrcpu_u_op_args);
	}
	if(9 == optype){
		ret = read_network_info();
	}
#if defined(XJPB44_02) || defined(TDPB44_02)||defined( AP2400 )
	if(19 == optype){
		ret = read_entry_addr();
	}
#endif
#if (defined(AQ2000N) || defined (PC018)|| defined (AQ2000_X) || defined(APM82181) || defined(AE5000ENT)) && !defined( XH9344 )&& !defined( XJ9344 )
	if(10 == optype){
		fd=get_the_buff(fd);
		ret = compare_to_version();
		//printf("ret = %d\n",ret);
	}
	if(23 == optype){
		fd=get_the_buff(fd);
		ret = read_kernel1();
	}
	if(20 == optype){
		fd=get_the_buff(fd);
		ret = read_kernel2();
	}
	if(21 == optype){
		#ifdef APM82181
		ret = read_temp();
		#else
		#if defined(PC018) && !defined( XH9344 )&& !defined( XJ9344 )
		ret = get_temperature(&wrcpu_u_op_args);
        #endif
        #endif
	}
	if(13 == optype){		
		fd=get_the_buff(fd);
		ret = read_kernel_num();
	}
#endif
#if defined(PC018) && !defined( XH9344 )&& !defined( XJ9344 )
	if(24 == optype){
		ret = read_power_in(&wrcpu_u_op_args);
	}
	if(25 == optype){
		ret = read_power_out(&wrcpu_u_op_args);
	}
	/*begin:wangyu add for apv5 temperature control*/
	if(26 == optype){
		ret = set_temperature_up(&wrcpu_u_op_args);
	}
	if(27 == optype){
		ret = set_temperature_down(&wrcpu_u_op_args);
	}
	/*end:wangyu add for apv5 temperature control*/
#endif
    /*Begin:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/
#if defined(AE5000ENT)
    if(28 == optype){
		ret == turn_off_all_lights(&wrcpu_u_op_args);
    }
	if(29 == optype){
		ret == turn_on_all_lights(&wrcpu_u_op_args);
	}
#endif
/*End:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/
	if(fd)
		autelan_close(fd);
	return 0;
}
