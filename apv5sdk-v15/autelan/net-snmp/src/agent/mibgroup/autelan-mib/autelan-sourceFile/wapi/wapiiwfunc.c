/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: wapiiwfunc.c
* description:  implementation for wireless functions to get wapi information.
* 
*
* 
************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_bridge.h>


#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "p80211def.h"
#include "wapiiofunc.h"
#include "wapiiwfunc.h"
#include "wapicgifunc.h"
//#include "wireless.h"  hyh test

extern if_info wlandev[10];
extern int wlannum;
extern int wds_status_flag;
extern int sta_status_flag;
extern int mac_status_flag;

int
iw_byte_size(int	args)
{
  int	ret = args & IW_PRIV_SIZE_MASK;

  if(((args & IW_PRIV_TYPE_MASK) == IW_PRIV_TYPE_INT) ||
     ((args & IW_PRIV_TYPE_MASK) == IW_PRIV_TYPE_FLOAT))
    ret <<= 2;

  if((args & IW_PRIV_TYPE_MASK) == IW_PRIV_TYPE_NONE)
    return 0;

  return ret;
}


static int
set_private(int		skfd,		/* Socket */
	    char *	args[],		/* Command line args */
	    int		count,		/* Args count */
	    char *	ifname,		/* Dev name */
	    char *  buffer,     /* return */
            int  *  paramnum)
{
  struct iwreq		wrq;
  int		i = 0;		/* Start with first arg */
  int		k;
  iwprivargs	priv[16];
  int		number;
  int		temp;

  /* Read the private ioctls */
  number = iw_get_priv_info(skfd, ifname, priv);

  /* Is there any ? */
  if(number <= 0)
    {
      /* Could skip this message ? */
      printf("%-8.8s  no private ioctls.\n\n",
	      ifname);
      return(-1);
    }

  /* Search the correct ioctl */
  k = -1;
  while((++k < number) && strcmp(priv[k].name, args[i]));

  /* If not found... */
  if(k == number)
    {
      printf("Invalid command : %s\n", args[i]);
      return(-1);
    }
	  
  /* Next arg */
  i++;

  /* If we have to set some data */
  if((priv[k].set_args & IW_PRIV_TYPE_MASK) &&
     (priv[k].set_args & IW_PRIV_SIZE_MASK))
    {
      switch(priv[k].set_args & IW_PRIV_TYPE_MASK)
	{
	case IW_PRIV_TYPE_BYTE:
	  /* Number of args to fetch */
	  wrq.u.data.length = count - 1;
	  if(wrq.u.data.length > (priv[k].set_args & IW_PRIV_SIZE_MASK))
	    wrq.u.data.length = priv[k].set_args & IW_PRIV_SIZE_MASK;

	  /* Fetch args */
	  for(; i < wrq.u.data.length + 1; i++) {
	    sscanf(args[i], "%d", &temp);
	    buffer[i - 1] = (char) temp;
	  }
	  break;

	case IW_PRIV_TYPE_INT:
	  /* Number of args to fetch */
	  wrq.u.data.length = count - 1;
	  if(wrq.u.data.length > (priv[k].set_args & IW_PRIV_SIZE_MASK))
	    wrq.u.data.length = priv[k].set_args & IW_PRIV_SIZE_MASK;

	  /* Fetch args */
	  for(; i < wrq.u.data.length + 1; i++)
	    sscanf(args[i], "%d", ((u_int *) buffer) + i - 1);
	  break;

	case IW_PRIV_TYPE_CHAR:
	  if(i < count)
	    {
	      /* Size of the string to fetch */
	      wrq.u.data.length = strlen(args[i]) + 1;
	      if(wrq.u.data.length > (priv[k].set_args & IW_PRIV_SIZE_MASK))
		wrq.u.data.length = priv[k].set_args & IW_PRIV_SIZE_MASK;

	      /* Fetch string */
	      memcpy(buffer, args[i], wrq.u.data.length);
	      i++;
	    }
	  else
	    {
	      wrq.u.data.length = 1;
	      buffer[0] = '\0';
	    }
	  break;

	default:
	  printf("Not yet implemented...\n");
	  return(-1);
	}
	  
      if((priv[k].set_args & IW_PRIV_SIZE_FIXED) &&
	 (wrq.u.data.length != (priv[k].set_args & IW_PRIV_SIZE_MASK)))
	{
	  printf("The command %s need exactly %d argument...\n",
		 priv[k].name, priv[k].set_args & IW_PRIV_SIZE_MASK);
	  return(-1);
	}
    }	/* if args to set */
  else
    {
      wrq.u.data.length = 0L;
    }

  strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

  if((priv[k].set_args & IW_PRIV_SIZE_FIXED) &&
     (iw_byte_size(priv[k].set_args) < IFNAMSIZ))
    memcpy(wrq.u.name, buffer, IFNAMSIZ);
  else
    {
      wrq.u.data.pointer = (caddr_t) buffer;
      wrq.u.data.flags = 0;
    }

  /* Perform the private ioctl */
  if(ioctl(skfd, priv[k].cmd, &wrq) < 0)
    {
      printf("Interface doesn't accept private ioctl...\n");
      printf("%X: %s\n", priv[k].cmd, strerror(errno));
      return(-1);
    }

  /* If we have to get some data */
  if((priv[k].get_args & IW_PRIV_TYPE_MASK) &&
     (priv[k].get_args & IW_PRIV_SIZE_MASK))
    {

      if((priv[k].get_args & IW_PRIV_SIZE_FIXED) &&
	 (iw_byte_size(priv[k].get_args) < IFNAMSIZ))
	{
	  memcpy(buffer, wrq.u.name, IFNAMSIZ);
	  *paramnum = priv[k].get_args & IW_PRIV_SIZE_MASK;
	}
      else
	*paramnum = wrq.u.data.length;


    }	/* if args to set */

  return(0);
}

int get_priv_int( int tok, char* ifname )
{
    char *args[2];
    char buffer[1024];
    char tokbuff[10];
    int goterr;
    int num;
    int skfd = socket(AF_INET, SOCK_DGRAM, 0);

    if ( skfd == -1 ) 
    {
	return -1;
    }

    args[0] = "getprism2_param";
    sprintf(tokbuff,"%d",tok);
    args[1] = tokbuff;
    goterr = set_private(skfd, args, 2, ifname, buffer, &num);
    close(skfd);
    if(goterr)
        return -1;
    return ((int *) buffer)[0];
}

int set_priv_int( int tok, char* ifname, long value )
{
    char *args[3];
    char buffer[1024];
    char tokbuff[10];
    char valuebuff[20];
    int goterr;
    int num;
    int skfd = socket(AF_INET, SOCK_DGRAM, 0);

    if ( skfd == -1 ) 
    {
	return -1;
    }

    args[0] = "prism2_param";
    sprintf(tokbuff,"%d",tok);
    args[1] = tokbuff;
    sprintf(valuebuff, "%d", value);
    args[2] = valuebuff;

    goterr = set_private(skfd, args, 3, ifname, buffer, &num);
    close(skfd);
    if(goterr)
        return -1;
    return ((int *) buffer)[0];
}


int wapiset_essid(u_char *var_val, size_t var_val_len, char *ifname)
{
	struct iwreq		wrq;
	int result;
	char temp[34];
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);

	if ( skfd == -1 ) 
	{
		return -1;
	}

	memcpy(temp, var_val, var_val_len);
	temp[var_val_len] = '\0';

	wrq.u.essid.flags = 1;
	wrq.u.essid.pointer = (caddr_t) temp;
	wrq.u.essid.length = var_val_len + 1;
	result = iw_set_ext(skfd, ifname, SIOCSIWESSID, &wrq);
	close(skfd);
	return result;

}

int wapiset_bssid(u_char *var_val, size_t var_val_len, char *ifname)
{
	struct iwreq  wrq;
	int result;
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);

	if ( skfd == -1 ) 
	{
		return -1;
	}

	wrq.u.ap_addr.sa_family = ARPHRD_ETHER;
	memcpy(wrq.u.ap_addr.sa_data, var_val, 6);
	result = iw_set_ext(skfd, ifname, SIOCSIWAP, &wrq);
	close(skfd);
	return result;

}

int wapiset_name(u_char *var_val, size_t var_val_len, char *ifname)
{
	struct iwreq		wrq;
	int result;
	char temp[34];
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);

	if ( skfd == -1 ) 
	{
		return -1;
	}

	memcpy(temp, var_val, var_val_len);
	temp[var_val_len] = '\0';

	wrq.u.essid.pointer = (caddr_t) temp;
	wrq.u.essid.length = var_val_len + 1;
	result = iw_set_ext(skfd, ifname, SIOCSIWNICKN, &wrq);
	close(skfd);
	if(result >= 0)
		return 0;
	else
		return -1;

}

int wapiset_mode(u_char *var_val, size_t var_val_len, char *ifname)
{
	struct iwreq  wrq;
	int result;
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);

	if ( skfd == -1 ) 
	{
		return -1;
	}

	wrq.u.mode = *(int *) var_val;
	result = iw_set_ext(skfd, ifname, SIOCSIWMODE, &wrq);
	close(skfd);
	if(result >= 0)
		return 0;
	else
		return -1;
}

int wapiset_dens(u_char *var_val, size_t var_val_len, char *ifname)
{
	struct iwreq  wrq;
	int result;
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);

	wrq.u.sens.value = *(int *) var_val;
	result = iw_set_ext(skfd, ifname, SIOCSIWSENS, &wrq);
	close(skfd);
	if(result >= 0)
		return 0;
	else
		return -1;

}

int wapiset_rts(u_char *var_val, size_t var_val_len, char *ifname)
{
	struct iwreq  wrq;
	int result;
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);

	wrq.u.rts.disabled = 0;
	wrq.u.rts.value = *(unsigned long *) var_val;
	result = iw_set_ext(skfd, ifname, SIOCSIWRTS, &wrq);
	close(skfd);
	if(result >= 0)
		return 0;
	else
		return -1;

}

int wapiset_frag(u_char *var_val, size_t var_val_len, char *ifname)
{
	struct iwreq  wrq;
	int result;
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	wrq.u.frag.disabled = 0;
	wrq.u.frag.value = *(unsigned long *) var_val;
	result = iw_set_ext(skfd, ifname, SIOCSIWFRAG, &wrq);
	close(skfd);
	if(result >= 0)
		return 0;
	else
		return -1;

}




int  notify_to_APdriver(int ioctl_fd, unsigned short ioflag, void *buf)
{
	int             result = 0;
	char            devname[] = DEVICE_NAME;/***wlan0***/
	p80211ioctl_req_t       req;
	
	req.len = MSG_BUFF_LEN;         /* TODO: need to fix the length */
	req.magic = P80211_IOCTL_MAGIC;
	req.data = (void*)buf;         //msg;
	strcpy(req.name, devname);
	result = ioctl(ioctl_fd, ioflag, &req);
	if ( result < 0 )
	{
		fprintf(stderr, "%s", strerror(errno));
	}
	return result;
}

int send_data(char *data_buf, int data_buf_len, const unsigned short packet_type)
{
	int ioctl_fd = -1;
	int ret = 0; 
	struct ioctl_drv ioctl_drv_data;
	
	ioctl_fd = socket_open_for_ioctl();
	
	if(ioctl_fd < 0) return -1;
	memset(&ioctl_drv_data, 0, sizeof(struct ioctl_drv));
	
	ioctl_drv_data.io_packet = packet_type;
	memcpy(ioctl_drv_data.iodata.pbData, data_buf, data_buf_len);
	ioctl_drv_data.iodata.wDataLen = data_buf_len;
	ret = notify_to_APdriver(ioctl_fd, P80211_IOCTL_PRIV, &ioctl_drv_data);
	close(ioctl_fd);
	return ret;
}
int set_ap_hanter_begin( )
{
	int wds_becon_control_stop = 1;
	int ret = 0;
	ret = send_data((char * )(&wds_becon_control_stop), sizeof(unsigned char), P80211_PACKET_AP_HUNTRER);
	return ret;
	
}

int set_ap_hanter_end( )
{
	int wds_becon_control_start = 0;
	int ret = 0;
	ret = send_data((char * )(&wds_becon_control_start), sizeof(unsigned char), P80211_PACKET_AP_HUNTRER);
	return ret;
	
}


int set_ap_channel(char *devname, unsigned short channel)
{
	
	struct iwreq	  wrq;
	double 	   freq;
	int ret = 0;
	int ioctl_fd = -1;
	
	
	if((channel > 11 ) || (channel == 0) )
	{
		printf("channel = %d out range\n", channel);
		return -1;
	}
	
	ioctl_fd = socket_open_for_ioctl();
	if(ioctl_fd < 0) return -1;

	freq = (double)channel;
	iw_float2freq(freq, &(wrq.u.freq));
	
	ret = iw_set_ext(ioctl_fd, devname, SIOCSIWFREQ, &wrq);
	close(ioctl_fd);
	if(ret < 0 )
	{
		fprintf(stderr, " SET failed on device %-1.8s ; %s.\n",
			devname, strerror(errno)); 
	}

	return ret; 
}

int channel_hande_timer( unsigned long interval, int channel_used)
{
	int i = 0;
	int ret = 0;	
	for(i = 11; i > 0; i-= 5)
	{
		ret = set_ap_channel(DEVICE_NAME, i);
		if(ret < 0) break;
		usleep(interval);
	}
	set_ap_channel(DEVICE_NAME, (unsigned short)channel_used);
	return ret;
}

/**/
int wapi_get_ap_info(unsigned short ioflag, struct print_info_ap *print_info)
{
	struct iwreq          wrq;
	int ioctl_fd = -1;
	int ret = 0;
	char   devname[] = DEVICE_NAME;

	char buffer[IW_MAX_AP_IWN * (sizeof(struct wapi_ap_info) +
				sizeof(struct sockaddr) +
				sizeof(struct iw_quality))];
	char *tmp_buf = buffer;
	int buff_len = 0;

	struct wapi_ap_info *ap_info;
	struct sockaddr * hwa;
	struct iw_quality *qual;
	iwrange   range;
	int 	  has_range = 0;
	int 	  has_qual = 0;
	int 	  n = 0;
	int 	  i;	

	
	buff_len = IW_MAX_AP_IWN * (sizeof(struct wapi_ap_info)+
						sizeof(struct sockaddr) +
						sizeof(struct iw_quality));

	memset(buffer, 0 , buff_len);
	
	memset(&wrq, 0 ,sizeof(struct iwreq));

	ioctl_fd = socket_open_for_ioctl();
	if(ioctl_fd < 0) 
	{
		ret = -1;
		goto exit;
	}
	
	/* Collect stats */
	strncpy(wrq.ifr_name, devname, IFNAMSIZ);
	wrq.u.data.pointer = (caddr_t) buffer;
	wrq.u.data.length = IW_MAX_AP_WAPI;
	wrq.u.data.flags = 0;

	ret = ioctl(ioctl_fd, ioflag, &wrq);
	if(ret < 0)
	{
		fprintf(stderr, "%-8.8s  Interface doesn't have a list of Access Points\n\n", devname);
		ret = -1;
		goto exit;
	  }
	
	/* Number of addresses */
	n = wrq.u.data.length;
	has_qual = wrq.u.data.flags;
	/* The two lists */
	ap_info = (struct wapi_ap_info *)tmp_buf;
	tmp_buf += (sizeof(struct wapi_ap_info)*n);
	
	hwa = (struct sockaddr *) tmp_buf;
	tmp_buf += (sizeof(struct sockaddr) * n);
	
	qual = (struct iw_quality *)tmp_buf;
	
	/* Check if we have valid mac address type */
	ret = iw_check_mac_addr_type(ioctl_fd, devname); 
	if(ret < 0)
	{
		fprintf(stderr, "%-8.8s  Interface doesn't support MAC addresses\n\n", devname);
		ret = -1;	
		goto exit;
	}
	
	/* Get range info if we can */
	if(iw_get_range_info(ioctl_fd, devname, &(range)) >= 0)
	  has_range = 1;
	
	for(i = 0; i < n; i++)
	{
		if(has_qual)
	  	{
			/* Print stats for this address */
			iw_pr_ap_info(print_info[i].ssid, ap_info[i].ap_ssid);
			sprintf(print_info[i].channel, "%d", ap_info[i].ap_channel);
			iw_pr_ether(print_info[i].mac, hwa[i].sa_data);
			iw_print_stats(&(print_info[i]), &qual[i], &range, has_range);
		}
		else
	 	{ //Only print the address 
	  		iw_pr_ether(print_info[i].mac, hwa[i].sa_data);
		}
	}
	
exit:
	return (ret == 0? n:ret);	
}

int socket_open_for_ioctl()
{
	int ipx_sock = -1;                /* IPX socket */
	int ax25_sock = -1;               /* AX.25 socket */
	int inet_sock = -1;               /* INET socket*/
	int ddp_sock = -1;                /* Appletalk DDP socket*/

	inet_sock=socket(AF_INET, SOCK_DGRAM, 0);
	if(inet_sock!=-1)
	{
		return inet_sock;
	}
	ipx_sock=socket(AF_IPX, SOCK_DGRAM, 0);
	if(ipx_sock!=-1)
	{
		return ipx_sock;
	}
	ax25_sock=socket(AF_AX25, SOCK_DGRAM, 0);
	if(ax25_sock!=-1)
	{
		return ax25_sock;
	}
	
	ddp_sock=socket(AF_APPLETALK, SOCK_DGRAM, 0);
	return ddp_sock;		
					
}
char * iw_pr_ether(char *buffer,  unsigned char *	ptr)
{
  sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
	  (ptr[0] & 0xFF), (ptr[1] & 0xFF), (ptr[2] & 0xFF),
	  (ptr[3] & 0xFF), (ptr[4] & 0xFF), (ptr[5] & 0xFF)
  );
  return(buffer);
}
char *iw_pr_ap_info(char *buffer,  unsigned char *	ptr)
{
	sprintf(buffer, "%s ", ptr);
	return buffer;
}

char *free_mem(char *buffer, int buff_len)
{
	if(buffer != NULL)
	{
		memset(buffer, 0, buff_len);
		free(buffer);
		buffer = NULL;
	}
	return NULL;
}


int br_addif_dev(char *br_name, char *dev_name)
{
	int ret = 0;
	int ifindex;
	
	ifindex = if_nametoindex(dev_name);
	if(ifindex == 0)
	{
		printf("interface %s does not exist\n", dev_name);
		return -1;	
	}
	ret =  br_device_ioctl(br_name, BRCTL_ADD_IF, ifindex, 0, 0);
	
	if(ret == 0) return 0;
	switch(ret)
	{
		case EBUSY:
			printf("device %s is already a member of a bridge; "
							"can't enslave it to bridge %s.\n", dev_name,
							br_name);
			break;
		default:
			fprintf(stderr, "br_add_interface\n");
			break;
	
	}
	return -1;
}

int br_device_ioctl(char *br_name, unsigned long arg0, 
					unsigned long arg1,unsigned long arg2,
					unsigned long arg3)
{
	unsigned long args[4];
	struct ifreq ifr;
	int ret = 0;
	int fd = -1;
	
	fd = socket(AF_INET,SOCK_STREAM, 0);
	if(fd < 0) 	return errno;
	
	args[0] = arg0;
	args[1] = arg1;
	args[2] = arg2;
	args[3] = arg3;
	strncpy(ifr.ifr_name, br_name, IFNAMSIZ);
	((unsigned long *)(&ifr.ifr_data))[0] = (unsigned long)args;
	ret = ioctl(fd, SIOCDEVPRIVATE,&ifr);
	if(ret < 0)
	{
        	fprintf(stderr, "%s\n", strerror(errno));
        	fprintf(stderr, "The ioctl_flag is = 0x%02x\n", SIOCDEVPRIVATE);

	}
	return ret;
}

/*添加或删除时发送MAC地址到驱动*/
int send_mac(unsigned short ioflag, char *mac, int *mac_len)
{
	struct iwreq          wrq;
	int ioctl_fd = -1;
	int ret = 0;
	char   devname[] = DEVICE_NAME;
	u_char buffer[1024];
		
	ioctl_fd = socket_open_for_ioctl();
	if(ioctl_fd < 0) return -1;
	
	memset(&wrq, 0 ,sizeof(struct iwreq));
	if(IW_IS_SET(ioflag))
	{	
		wrq.u.data.length = *mac_len + 1;
	
		memcpy(buffer, mac, wrq.u.data.length);
	
		buffer[sizeof(buffer) - 1] = '\0';
	}
	else if(IW_IS_GET(ioflag))
	{
		wrq.u.data.length = 0L;
			
	}
	strncpy(wrq.ifr_name, devname, IFNAMSIZ);
  	wrq.u.data.pointer = (caddr_t) buffer;
	wrq.u.data.flags = 0;
      	
    	
	ret = ioctl(ioctl_fd, ioflag, &wrq);
	if(ret < 0)
	{
		fprintf(stderr, "Interface doesn't accept private ioctl...\n");
		fprintf(stderr, "%X: %s\n", ioflag, strerror(errno));
	}
	if(IW_IS_GET(ioflag))
	{
		int       n = 0;
		n = wrq.u.data.length;
		buffer[wrq.u.data.length] = '\0';
		memcpy(mac, buffer, n);
		
		*mac_len = n;
				
	}
	return ret;	
}
/*获取自身无线网卡地址*/
int get_mac(unsigned short ioflag, char *mac, int *mac_len)
{
	
	int ret = 0;
	ret =  send_mac(ioflag, mac, mac_len);
	return ret;
}
/*取得wds设备名*/
int get_dev_name(void  *set_data, int *data_len, unsigned long io_type)
{
	struct iwreq		  wrq;
	int ioctl_fd = -1;
	int ret = 0;
	char   devname[] = DEVICE_NAME;
	char get_data[30]={0,};
	unsigned short int name_len = 0;
	
	ioctl_fd = socket_open_for_ioctl();
	if(ioctl_fd < 0) return  -1;
			
	memset(&wrq, 0 ,sizeof(struct iwreq));
	strncpy(wrq.ifr_name, devname, IFNAMSIZ);
	
	memcpy(get_data, set_data, *data_len);
	get_data[*data_len + 1] = '\0';
	
	wrq.u.data.length = 0L;
	wrq.u.data.pointer = (caddr_t)get_data;
	wrq.u.data.flags = 0;
			
			
	ret = ioctl(ioctl_fd, io_type, &wrq);
	if(ret < 0)
	{
		printf("Interface doesn't accept private ioctl...\n");
		printf( "%s\n", strerror(errno));
		return -1;
	}
	name_len = *(unsigned short *)get_data;		
	if(name_len == 0)
 	{
 			printf("get data error\n");
 			return -1;
 	}
	memcpy(set_data, get_data + 2, name_len);
	((char *)set_data)[ name_len] = '\0';
	*data_len = name_len;
	return 0;
}

int setioctl_ifaddr(char *ifaddr, char *ifname, int ioctl_type)
{
	struct ifreq ifr;
	struct sockaddr_in sa_in;
	int fd;
	
	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) > -1) 
	{
		int ret = 0;
		
		strcpy(ifr.ifr_name, ifname);
		sa_in.sin_family = AF_INET;
		ret = inet_aton(ifaddr, &(sa_in.sin_addr));
		if(ret == 0) 
		{
			printf("the ifaddr invalid\n");
			return -1;
		}
		memcpy(&ifr.ifr_addr, (struct sockaddr *)(&sa_in), 
				sizeof(struct sockaddr));
		if(ioctl(fd, ioctl_type, &ifr) == 0)
		{
			return 0;	
		}
		else
		{
			printf("send to dev=%s ", ifname);
			fprintf(stderr, "%s ", strerror(errno));
			printf("The ioctl_flag is = 0x%02x\n", ioctl_type);
		}
	}
	return -1;	
}

int setioctl_dev_ifflag(short ifflag, char *ifname, int ioctl_type)
{
	struct ifreq ifr;                    /* Socket stuff */
	int fd;
	
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) > -1)
	{
		strcpy(ifr.ifr_name, ifname);
		ifr.ifr_flags = ifflag;
		if (ioctl(fd, ioctl_type, &ifr) == 0)
		{
			return 0;
		}
		else
		{
			printf("send to dev=%s ", ifname);
			fprintf(stderr, "%s ", strerror(errno));
			printf("The ioctl_flag is = 0x%02x\n", ioctl_type);
		}
	}
	return -1;
}



int macstr2byte(char *macstr, unsigned char *des_byte, int byte_len)
{
	int i, val, val2;
	char *pos = macstr;
	
	for (i = 0; i < byte_len; i++) {
		val = hex2int(*pos++);
		if (val < 0)
			return -1;
		val2 = hex2int(*pos++);
		if (val2 < 0)
			return -1;
		des_byte[i] = (val * 16 + val2) & 0xff;
		
	}
	return 0;
}

int send_key(char *key_buf, int key_buf_len)
{
	int ioctl_fd = -1;
	int ret = 0; 
	struct ioctl_drv ioctl_drv_data;
	
	ioctl_fd = socket_open_for_ioctl();
	
	if(ioctl_fd < 0) return -1;
	memset(&ioctl_drv_data, 0, sizeof(struct ioctl_drv));
	
	ioctl_drv_data.io_packet = P80211_PACKET_B2410_SEC;
	memcpy(ioctl_drv_data.iodata.pbData, key_buf, key_buf_len);
	ioctl_drv_data.iodata.wDataLen = key_buf_len;
	ret = notify_to_APdriver(ioctl_fd, P80211_IOCTL_PRIV, &ioctl_drv_data);
	return ret;
}
















