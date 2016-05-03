/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: wapicgifunc.c
* description:  implementation for some functions used as cgi. 
* 
*
* 
************************************************************************************/

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <dirent.h>
#define SEP_Q_MARK	'"'
#include "p80211def.h"
#include "wapiiofunc.h"
#include "wapiiwfunc.h"
#include "wapicgifunc.h"
#include "wapiPwlanAp.h"
#include<sys/file.h>

int save_global_conf(char type, char* filename, char *command, char *key, char *value)
{
	FILE* fidIn, *fidOut;
	char line[1024], tmp_line[1024];
    char tmp_fname[255], *index;
    char separator[2];
    int i=0;
	 
	if ((fidIn=fopen(filename,"r"))==NULL) return 0;
	strcpy(tmp_fname,filename);	
	strcat(tmp_fname,".snmp");
	
    if ((fidOut=fopen(tmp_fname,"w"))==NULL){
		AAT aat;
		time_t nowTimes;
    	memset(&aat,0,sizeof(aat));
    	time(&nowTimes);
		set_alarm( &aat,ALARM_ID_FLASH_WRITEERROR,"flash no space,not write"); //by sunyushuang0321



		send_trap_string_notification(& aat) ; //by sunyushuang0321
		return 0;
	}
 	 
    strcpy(separator,(type==SEP_EQUAL?"=":type==SEP_SPACE?" ":"\t"));
    type=separator[0];

	while(!feof(fidIn))
	{
		fgets(line,1024,fidIn);
                strcpy(tmp_line,command);
                strcat(tmp_line,key);
                if (((index=strstr(line,tmp_line))!=NULL) && (index==line)) {
                    i=1;
                    if (!feof(fidIn)) {
                        if (type != '0'){
                            fprintf(fidOut,"%s%s%c%c%s%c\n",command,key,type,SEP_Q_MARK, value, SEP_Q_MARK);
                        }
						else {
                            fprintf(fidOut,"%s%s%c%s%c\n",command,key,SEP_Q_MARK,value,SEP_Q_MARK);
						}
					}
                } else{
                    if (!feof(fidIn)) fprintf(fidOut,"%s",line);
                }
        }

        if (i==0) {
            if (type != '0') { fprintf(fidOut,"%s%s%c%s\n",command,key,type,value); }
            else fprintf(fidOut,"%s%s%s\n",command,key,value);
			 
        }
	fclose(fidIn);
	fclose(fidOut);
	unlink(filename);
	rename(tmp_fname,filename);
	return 1;
}



 
int get_pr (char type, char *line, prop_data *prop)
{
    //int index;
    char *index;
    char *keyptr;
    char *sea_str="";//wapi 2002-12-16

    if (line[0] != '#')
    {
        // modified by kWlW, separator changed to this  vvv  from "="
        keyptr = (char *)strstr(line, "setenv ");	//finds the first occurrence of the substring 'setenv' in line
        if (keyptr != NULL) line = keyptr + 7;   /* Add size of 'setenv ' string */
        sea_str=(char *)(type == SEP_EQUAL ? "=" : type == SEP_SPACE ? " " : "\t");

	index = strstr (line, sea_str);
	if (index != NULL)		
        {
            *((char *) index) = '\0';
            if ((prop->key = (char *)malloc (strlen (line) + 1)) != NULL &&
                (prop->value = (char *)malloc (strlen ((char *) (index + 1)) + 1)) != NULL)
            {
                strcpy (prop->key, line);
                strcpy (prop->value, (char *) (index + 1));
                return 1;
            }
        }
    }
    return 0;
}


int load_prop (char type, char *file_name, prop_data properties[])
{
	FILE *fid;
	char line[254];
	int prop_count = 0;

	if ((fid = fopen (file_name, "r")) == NULL)
		return 0;
	while (!feof (fid))
	{
		fscanf (fid, "%[^\n]\n", line);
		if (get_pr (type, line, (prop_data *) & properties[prop_count]))
		{
			prop_count++;
		}
	}
	fclose (fid);
	return prop_count;
}
#if 0
char *get_prop (char *key, char *result, prop_data properties[], int count)
{
  int index;
  for (index = 0; index < count; index++)
    {
      if (strcmp (properties[index].key, key) == 0)
	{
	  strcpy (result, properties[index].value);
	  return result;
	}
    }
  return NULL;
}
#endif
char *get_prop (char *key, char *result, prop_data properties[], int count)
{  
	int index;
  char tmp_str[256] = "";
  for (index = 0; index < count; index++)
  {
  	  /*ljy--test start*/
	  //printf("%s: properties[index].key %s, key %s.\n", __func__, properties[index].key, key);
	  /*ljy--test end*/
      if (strcmp (properties[index].key, key) == 0)
	  {
	  	strcpy(tmp_str, properties[index].value);	
	 	strcpy (result, strip(tmp_str, SEP_Q_MARK));
	 	return result;
	  }
  }
  return NULL;
}


int free_prop (prop_data properties[], int count)
{
  int index;
  for (index = 0; index < count; index++)
    {
      free (properties[index].key);
      free (properties[index].value);
    }
  return 1;
}

int kill_proc (char *proc_name, int signal)
{
  FILE *f;
  DIR *dir;
  struct dirent *entry;
  char path[32], line[LENGTH], *ind;

  dir = opendir ("/proc");
  if (!dir)
    return 1;

  while ((entry = readdir (dir)) != NULL)
    {
      if (atoi (entry->d_name) == 0)
	continue;

      sprintf (path, "/proc/%s/status", entry->d_name);
      f = fopen (path, "r");
      fgets (line, LENGTH, f);
      fclose (f);
      ind = strchr (line, SEP_TAB);
      ind++;
      ind[strlen (ind) - 1] = '\0';	/* Strips newline character */
      		 
      if (strcmp (ind, proc_name) == 0)
	{
	    kill (atoi (entry->d_name), signal);/* Kill process with signal */
	  
	  return 1;
	}
    }
  return 0;
}

char *strip (char *string, char delimiter)
{
  register int x, y;

  y = strlen (string);
  x = 0;
  while ((x < y) && (string[x] == delimiter))
    {
      x++;
    };

  while ((y > 0) && (string[y - 1] == delimiter))
    {
      y--;
    };
  string[y] = 0;
  return string + x;
}

int wapiget_ip(char *filename, char sep, char *key, char *outstr)
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char address[LENGTH]="";
	int ip;  //modify by sunyushuang0422


	prop_count=load_prop(sep,filename,properties);
	get_prop(key, address, properties, prop_count);
 	free_prop(properties,prop_count);

	ip = inet_addr(address);
	if(ip == -1)
		return -1;
	memcpy(outstr, (char *)&ip, 4);

	return 0;
}


int get_as_info(struct sockaddr_in *as_addr, char *loop, int as_port)
{

        as_addr->sin_family = AF_INET;
        as_addr->sin_port = htons(as_port);
        if(inet_aton(loop, &(as_addr->sin_addr))<0)
        {
                printf("\nas IP_addr error!!!\n\n");
                return -1;
        }
        return 0;
}


int open_as_socket()
{
        int sock;

        struct protoent *protocol;
        protocol = getprotobyname("udp");
        if (protocol == NULL)
        {
                printf("\nCreat CA_udp socket error 1!!!\n\n");
                exit(-1);
        }
        sock = socket(PF_INET, SOCK_DGRAM, protocol->p_proto);
        if (sock == -1 )
        {
                printf("\nCreat as_udp socket error 2!!!\n\n");
                exit(-1);
        }
        return sock;

}

int fd_timed_read(int fd, char *buf, int length,
   		int microseconds)
{
	fd_set readfds;
	struct timeval tv;
	int bytes_read;
	/* First, setup a select() statement to poll for the data comming in */
	FD_ZERO(&readfds);
	FD_SET(fd,&readfds);
	
	tv.tv_sec = microseconds;
	tv.tv_usec = 0;
        select(fd+1, &readfds, NULL, NULL, &tv);
        if (FD_ISSET(fd,&readfds))
	{
#ifndef WAPI_PWLAN_ID
		bytes_read = recv(fd, buf, length, MSG_WAITALL);
#else 
		bytes_read = recv(fd, buf, length,0);
#endif
		return(bytes_read);
	}
 	else
	{
		/* No bytes read */
		return 0;
	}
	return 0;
}
int hex2int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return -1;
}

int macstr2addr(char *macstr, unsigned char *addr)
{
	int i, val, val2;
	char *pos = macstr;

	for (i = 0; i < 6; i++) {
		val = hex2int(*pos++);
		if (val < 0)
			return -1;
		val2 = hex2int(*pos++);
		if (val2 < 0)
			return -1;
		addr[i] = (val * 16 + val2) & 0xff;

		if (i < 5 && *pos++ != ':')
			return -1;
	}

	return 0;
}

int save_cert(char *filename, char *buff, int bufflen)
{
	FILE *stream;
	int result;

	if(filename == NULL || strcmp(filename, "")==0)
		return -1;

	stream = fopen(filename,"w+b");
	if(stream == NULL)
		return -1;
	result = fwrite(buff, bufflen, 1, stream);

	fclose(stream);
	return 0;
}


int download_cert(char *filename, char* buff, int *bufflen)
{

	FILE *stream;
	int result;
  	char c1;
	int i = 0;

	if(filename == NULL || strcmp(filename, "")==0)
		return -1;

	stream = fopen(filename,"rb");
	if(stream == NULL)
		return -1;

	while(1)
	{
		c1 = fgetc(stream);

		if(feof(stream))
			break;

		buff[i] = c1;
		i++;
	}
	*bufflen = i;

	
	fclose(stream);
	return 0;
}

int low2upper(char *str)
{
	int n=0;
	int i=0;
	char ctmp, *cp;
	
	n=strlen(str);
	for(cp=str;(*cp)!='\0';cp++)
	{
		if(isgraph(*cp)&&(!isupper(*cp)))
		{
			ctmp=toupper(*cp);
			*cp=ctmp;
		}
	}

	return 0;
}

int save_mac1(char *filename, char *macaddr, char *startkey, char *endkey)
{
	FILE* fidIn=NULL;
        FILE* fidOut=NULL;
	int sw = 0;
	char line[255];
        char tmp_fname[255],tmp_key[255], tmp_key1[255];
	char tempmac[20];

	if ((fidIn=fopen(filename,"r"))==NULL) return -1;
	strcpy(tmp_fname,filename);	
	strcat(tmp_fname,".snmp");
	
        if ((fidOut=fopen(tmp_fname,"w"))==NULL)
	{
		fclose(fidIn);
		return -1;
	}
	
	strcpy(tempmac, macaddr);
	low2upper(tempmac);
	strcat(tempmac,"\n");

	strcpy(tmp_key,startkey);
	strcat(tmp_key,"\n");
	strcpy(tmp_key1,endkey);
	strcat(tmp_key1,"\n");

	while(!feof(fidIn))
	{
		fgets(line,255,fidIn);
                if (strcmp(line,tmp_key)==0&& !feof(fidIn))
		{
			fprintf(fidOut,"%s",line);
			sw = 1;
			break;
		}
	       	else if(!feof(fidIn)) 
			fprintf(fidOut,"%s",line);
        }
	if(sw == 0)
	{
		fprintf(fidOut,"%s",tmp_key);
		fprintf(fidOut,"%s",tempmac);
		fprintf(fidOut,"%s",tmp_key1);
	}
	else
	{
		fprintf(fidOut,"%s",tempmac);

		while(!feof(fidIn))
		{
			fgets(line,255,fidIn);
	            if (strcmp(line,tempmac)!=0 && !feof(fidIn))
			{
				fprintf(fidOut,"%s",line);
			}
		}
	}

	fclose(fidIn);
	fclose(fidOut);
	unlink(filename);
	rename(tmp_fname,filename);
	return 0;
}

int del_mac1(char *filename, char *macaddr, char *startkey, char *endkey)
{
	FILE* fidIn=NULL;
        FILE* fidOut=NULL;
	int sw = 0;
	char line[255];
        char tmp_fname[255],tmp_key[255], tmp_key1[255];
	char tempmac[20];

	if ((fidIn=fopen(filename,"r"))==NULL) return -1;
	strcpy(tmp_fname,filename);	
	strcat(tmp_fname,".snmp");
	
        if ((fidOut=fopen(tmp_fname,"w"))==NULL)
	{
		fclose(fidIn);
		return -1;
	}
	
	strcpy(tempmac, macaddr);
	strcat(tempmac,"\n");

	strcpy(tmp_key,startkey);
	strcat(tmp_key,"\n");
	strcpy(tmp_key1,endkey);
	strcat(tmp_key1,"\n");

	while(!feof(fidIn))
	{
		fgets(line,255,fidIn);
                if (strcmp(line,tmp_key)==0 && !feof(fidIn))
		{
			fprintf(fidOut,"%s",line);
			break;
		}
	       	else if(!feof(fidIn))
			fprintf(fidOut,"%s",line);
        }

	while(!feof(fidIn))
	{
		fgets(line,255,fidIn);
                if (strcmp(line,tmp_key1)==0 && !feof(fidIn))
		{
			fprintf(fidOut,"%s",line);
			break;
		}

		low2upper(line);
            	if (strcmp(line,tempmac)!=0 && !feof(fidIn))
		{
			fprintf(fidOut,"%s",line);
		}
		else
			sw = 1;

	}

	while(!feof(fidIn))
	{
		fgets(line,255,fidIn);
		if(!feof(fidIn))
			fprintf(fidOut,"%s",line);
        }

	fclose(fidIn);
	fclose(fidOut);
	unlink(filename);
	rename(tmp_fname,filename);
	return 0;
}


int del_macall(char *filename, char *startkey, char *endkey)
{
	FILE* fidIn=NULL;
        FILE* fidOut=NULL;
	int sw = 0;
	char line[255];
        char tmp_fname[255],tmp_key[255], tmp_key1[255];

	if ((fidIn=fopen(filename,"r"))==NULL) return -1;
	strcpy(tmp_fname,filename);	
	strcat(tmp_fname,".snmp");
	
        if ((fidOut=fopen(tmp_fname,"w"))==NULL)
	{
		fclose(fidIn);
		return -1;
	}
	
	strcpy(tmp_key,startkey);
	strcat(tmp_key,"\n");
	strcpy(tmp_key1,endkey);
	strcat(tmp_key1,"\n");

	while(!feof(fidIn))
	{
		fgets(line,255,fidIn);
                if (strcmp(line,tmp_key)==0 && !feof(fidIn))
		{
			fprintf(fidOut,"%s",line);
			break;
		}
	       	else if(!feof(fidIn))
			fprintf(fidOut,"%s",line);
        }

	while(!feof(fidIn))
	{
		fgets(line,255,fidIn);

                if (strcmp(line,tmp_key1)==0 && !feof(fidIn))
		{
			fprintf(fidOut,"%s",line);
			break;
		}
	}

	while(!feof(fidIn))
	{
		fgets(line,255,fidIn);
		if(!feof(fidIn))
			fprintf(fidOut,"%s",line);
        }

	fclose(fidIn);
	fclose(fidOut);
	unlink(filename);
	rename(tmp_fname,filename);
	return 0;
}

int  load_conf_file(char *file_name, char *be_key,char *end_key)
{
	char line[255];
	FILE *fid;
	
	int i = 0;
	int found = -1;
	
	if((fid = fopen(file_name, "r")) == NULL)
	{
		printf("Open read config file error!\n");
		return -1;
	}
	while (!feof(fid))
	{
		
		fscanf(fid, "%[^\n]\n", line);
		if(strcmp(line, be_key) == 0)
		 {
		   found = 0;
		   continue;
		 }	
		else if((found == 0) && (strcmp(line, end_key) != 0))
		{
			i++;
		}	
	}
	fclose(fid);
	return i;
}


int  save_rec_conf(char *file_name, char *key, char *buf)
{
	char line[255];
	FILE *fid, *fidw;
	char tmp_file[255]= "";
	
	
	strcpy(tmp_file, file_name);
	strcat(tmp_file,".snmp");
	
	if((fid = fopen(file_name, "r")) == NULL)
	{
		printf("Open read config file error!\n");
		return -1;
	}
	if((fidw = fopen(tmp_file, "w")) == NULL)
	{
		fclose(fid);
		return -1;
	}
	
	while (!feof(fid))
	{
		
		fscanf(fid, "%[^\n]\n", line);
		if(!strcmp(line, key))
		 {
		   fprintf(fidw, "%s\n", line);
		   fprintf(fidw, "%s\n", buf);
		 }	
		else
		{
			fprintf(fidw, "%s\n", line);
		}	
			
	}
	fclose(fid);
	fclose(fidw);
	unlink(file_name);
	rename(tmp_file, file_name);
	return 0;	    

}


int init_srv_info(struct srv_info *WAI_srv, const char *ip_addr)
{
	int ret = 0;

	memset(&(WAI_srv->addr), 0, sizeof(struct sockaddr_in));
	WAI_srv ->fd = socket(AF_INET, SOCK_DGRAM, 0);
	WAI_srv->addr.sin_family = AF_INET;
	WAI_srv->addr.sin_port = htons(WAI_srv->port);
	ret = inet_aton(ip_addr, &(WAI_srv->addr.sin_addr));
	if(ret == 0)
	{
	        printf("\nas IP_addr error!!!\n\n");
	        
	}
	return ret;
}

int wapi_send_data(struct srv_info *WAI_srv, struct _packet_reset_srv *packet_reset_srv)
{
	int sendlen = 0;
	int data_len = 0;
	int ret = 0;
	
	data_len = packet_reset_srv->head.data_len + sizeof(struct _head_info);
	packet_reset_srv->head.data_len = htons(packet_reset_srv->head.data_len);
	sendlen = sendto(	WAI_srv->fd, 
					(char *)packet_reset_srv, 
					data_len,0,
					(struct sockaddr *)&(WAI_srv->addr),
			  		sizeof(struct sockaddr_in));
	if(sendlen != data_len)
		ret = -1;
		
	return ret;
}
int recv_data(struct srv_info *WAI_srv, struct _packet_reset_srv *recv_from_srv)
{
	int readlen = 0;
	int ret = 0;
	
	readlen= fd_timed_read(WAI_srv->fd, (char *)recv_from_srv,
									RECVFROM_LEN, RECVTIMEOUT);
	if(readlen == 0)
		ret = -1;
	return ret;
}

void ntoh_data(struct _packet_reset_srv *recv_from_srv)
{
	recv_from_srv->head.ver = ntohs(recv_from_srv->head.ver);
	recv_from_srv->head.cmd = ntohs(recv_from_srv->head.cmd);
	recv_from_srv->head.data_len = ntohs(recv_from_srv->head.data_len);
}
void hton_data(struct _packet_reset_srv *senf_to_srv)
{
	senf_to_srv->head.ver = htons(senf_to_srv->head.ver);
	senf_to_srv->head.cmd = htons(senf_to_srv->head.cmd);
	senf_to_srv->head.data_len = htons(senf_to_srv->head.data_len);

}

int WAI_msg_handle(struct _packet_reset_srv *recv_from_WAI)
{
	int ret = 0;
	unsigned short CMD = 0;
	unsigned char check_result = 0;
	
	ntoh_data(recv_from_WAI);
	if(recv_from_WAI->head.ver != VERSIONNOW)
	{
		printf("Version error in data from , The Ver is %d\n", recv_from_WAI->head.ver );
		ret = -1;
		goto up_error;
	}
	if(recv_from_WAI->head.data_len !=2)
	{
		printf("data_len error in data from , The Ver is %d\n", recv_from_WAI->head.data_len );
		ret = -1;	
		goto up_error;
	}
	CMD = recv_from_WAI->head.cmd;
	check_result = *((unsigned short *)recv_from_WAI->data);
	switch(CMD)
	{
		case CHECK_CERT_RESPNOSE:
		case AP_RELOAD_RESPONSE:
			if(check_result != 0)
			{
				ret = -1;
			}
			break;		
		default:
			ret = -1;
			break;
	}
up_error:
	return ret;

}


int notify_WAPI_policy_to_APdriver(void  *flag)
{
	int	result = 0;
	int	fd;
	int 	ret = 0;

	p80211ioctl_req_t       req;
	char                    devname[] = DEVICE_NAME;//=prism0;
	
	/* get a socket */
	fd = socket_open_for_ioctl();

	if(fd < 0) 		return -1;
	req.len = MSG_BUFF_LEN;         /* TODO: need to fix the length */
	req.magic = P80211_IOCTL_MAGIC;
	req.data = flag;         //msg;
	strcpy(req.name,devname);
	result = ioctl(fd,P80211_IF_AUTH_FLAG,&req);
	if ( result < 0 ) 
	{
	      fprintf(stderr, "%s\n", strerror(errno));
		ret = -1;
	}
	close(fd);
	return ret;	
}
void print_string(void *str, int len)
{
	char *data = str;
	int i = 0;
	for(i=0; i<len; i++)
	{
		printf("%02x ", data[i]);
		if((i + 1)%16 == 0)
			printf("\n");
	}
}
int packet_policy(struct ioctl_drv *ioctl_drv_data, UINT16 packed_type, 
		UINT8 policy)
{
        int ret = -1;
	ioctl_drv_data->io_packet = packed_type;
	memcpy(ioctl_drv_data->iodata.pbData, &policy, sizeof(policy));
	ioctl_drv_data->iodata.wDataLen = sizeof(policy);
	print_string((unsigned char  *)ioctl_drv_data, sizeof(struct ioctl_drv));
	ret = notify_WAPI_policy_to_APdriver(ioctl_drv_data);		
	return ret;	
}

int WAPI_sendmsg(unsigned char wai_policy, unsigned char wpi_policy)
{
	struct ioctl_drv ioctl_drv_data ;
	int ret = 0;
	
	memset(&ioctl_drv_data, 0, sizeof(struct ioctl_drv));
	ret = packet_policy(&ioctl_drv_data, PACKET_AUTH_FLAG, wai_policy);
	if(ret == 0)
	{
		memset(&ioctl_drv_data, 0 ,sizeof(struct ioctl_drv));
		ret = packet_policy(&ioctl_drv_data, PACKET_ENCRYPT_POLICY, wpi_policy);
	}
	return ret;
}

#ifndef WAPI_PWLAN_ID
int send_ap_reload_msg(struct srv_info *WAI_srv)
{
	int ret = 0;
	struct _packet_reset_srv packet_reset_srv;
	struct _packet_reset_srv recv_from_srv;

	packet_reset_srv.head.ver = htons(VERSIONNOW);
	packet_reset_srv.head.cmd = htons(AP_RELOAD);
	//packet_reset_srv.data =NULL;
	packet_reset_srv.head.data_len = 0;
	ret = wapi_send_data(WAI_srv, &packet_reset_srv);
	if(ret != 0)
	{
		printf("sendto WAI error\n");
		goto err_exit;
	}
	ret = recv_data(WAI_srv, &recv_from_srv);

	if(ret != 0)
	{
		printf("recvfrom WAI error\n");
		goto err_exit;
	}
	ntoh_data(&recv_from_srv);
	if(recv_from_srv.head.ver != VERSIONNOW)
	{
		printf("Version error in data from , The Ver is %d\n", recv_from_srv.head.ver );
		goto err_exit;
	}
err_exit:
	return ret;
}

#else 
int send_ap_reload_msg(struct srv_info *WAI_srv)
{
	int ret = 0;
	struct _packet_reset_srv packet_reset_srv;
	struct _packet_reset_srv recv_from_srv;

	packet_reset_srv.head.ver = htons(VERSIONNOW);
	packet_reset_srv.head.cmd = htons(AP_RELOAD);
	//packet_reset_srv.data =NULL;
	packet_reset_srv.head.data_len = 0;
	ret = send_wapi_info(WAI_srv, &packet_reset_srv);
	if(ret != 0)
	{
		printf("sendto WAI error\n");
		goto err_exit;
	}
	ret = recv_wapi_info(WAI_srv, &recv_from_srv);

	if(ret != 0)
	{
		printf("recvfrom WAI error\n");
		goto err_exit;
	}
	ntoh_data(&recv_from_srv);
	if(recv_from_srv.head.ver != VERSIONNOW)
	{
		printf("Version error in data from , The Ver is %d\n", recv_from_srv.head.ver );
		goto err_exit;
	}
err_exit:
	return ret;
}
#endif


/********************lhh2007******************************************/

int save_certificate(const char *fname, char *fcontent, int flen)
{
	FILE *f;
	int ret = 0;

	f=fopen(fname,"wb");

	if(f == NULL)
	{
		ret = 1;
	}
	if(fwrite(fcontent, flen ,1,f) != 1)
	{
		ret = 2;
	}
	
	fclose(f);

	return ret;
}

int send_wapi_info(struct srv_info *WAI_srv, struct _packet_reset_srv *packet_reset_srv)
{
	int sendlen = 0;
	int data_len = 0;
	int ret = 0;
	
	data_len = packet_reset_srv->head.data_len + sizeof(struct _head_info);
	packet_reset_srv->head.data_len = htons(packet_reset_srv->head.data_len);
	sendlen = sendto(	WAI_srv->fd, 
					(char *)packet_reset_srv, 
					data_len,0,
					(struct sockaddr *)&(WAI_srv->addr),
			  		sizeof(struct sockaddr_in));
	if(sendlen != data_len)
		ret = -1;
		
	return ret;
}
int recv_wapi_info(struct srv_info *WAI_srv, struct _packet_reset_srv *recv_from_srv)
{
	int readlen = 0;
	int ret = 0;
	
	readlen= fd_timed_read(WAI_srv->fd, (char *)recv_from_srv,
									RECVFROM_LEN, RECVTIMEOUT);
	if(readlen == 0)
		ret = -1;
	return ret;
}

/* #0045-2 guoxd_001 20081103 add start */
int wapi_send_notify_wapid(struct srv_info *wai_srv, const void *send_buf, int len)
{
	int ret = 0;
	
	ret = sendto(	wai_srv->fd, 
					send_buf, 
					len,
					0,
					(struct sockaddr *)&(wai_srv->addr),
			  		sizeof(struct sockaddr_in));
	
	if(ret != len)
		ret = -1;
		
	return ret;
}


int wapi_recv_notify_wapid(struct srv_info *wai_srv, unsigned char *recv_buf, int buf_len)
{
	fd_set readfds;
	struct timeval tv;
	int ret = -1;

	/* check param */
	if (wai_srv == NULL || recv_buf == NULL || buf_len <= 0)
	{
		return ret;
	}
	
	/* First, setup a select() statement to poll for the data comming in */
	FD_ZERO(&readfds);
	FD_SET(wai_srv->fd, &readfds);

	/* timeout value */
	tv.tv_sec = RECVTIMEOUT;
	tv.tv_usec = 0;

	/* system wait, and timeout*/
       ret = select(wai_srv->fd+1, &readfds, NULL, NULL, &tv);
	
	/* event appear */
	if (ret > 0)
	{
	       if (FD_ISSET(wai_srv->fd, &readfds))
		{
			/* recv data from network */
			ret = recvfrom(wai_srv->fd, recv_buf, buf_len, 0, NULL, NULL);
			return ret;
		}
	}
	
	return ret;
}
/* #0045-2 guoxd_001 20081103 add end */



/************************************************************/















