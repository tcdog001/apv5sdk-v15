/*********************************************************************************************
*			Copyright(c), 2008, Autelan Technology Co.,Ltd.
*						All Rights Reserved
*
**********************************************************************************************
$RCSfile:igmp_snoop_cli.c
$Author: Rock
$Revision: 1.00
$Date:2008-3-8 10:07
***********************************************************************************************/
#include "igmp_snoop_com.h"
#include "igmp_snoop_inter.h"

#define _DEBUG_	
#ifdef _DEBUG_
#define DEBUG_OUT(format,arg...)	{	printf(format,##arg);}
#else
#define DEBUG_OUT(format,arg...)	
#endif

#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]


/***********************************declare functions**************************************/
static void *igmp_set_snp_enable(struct cfg_element *cur,void *value);
static void *igmp_set_snp_debug(struct cfg_element *cur,void *value);
static void *igmp_cfg_add_vlan(struct cfg_element *cur,void *value);
static void *igmp_cfg_add_port(struct cfg_element *cur,void *value);
static void *igmp_cfg_add_trunkport(struct cfg_element *cur,void *value);
static void *igmp_cfg_add_routeport(struct cfg_element *cur,void *value);
static INT igmp_snp_msg_proc(cmd_msg *t_msg, INT *flag);
static INT igmp_set_enable(LONG flag);
static INT igmp_del_spec_mcgroup(cmd_msg *t_msg);
static INT igmp_del_all_vlan(cmd_msg *t_msg );
static INT igmp_show_group_cnt(cmd_msg *t_msg);
static INT igmp_show_host_timerinterval(cmd_msg *t_msg);
static INT igmp_show_query_timerinterval(cmd_msg *t_msg);
static INT igmp_set_query_timerinterval(cmd_msg *t_msg);
static INT igmp_show_resp_timerinterval(cmd_msg *t_msg);
static INT igmp_set_robust_timerinterval(cmd_msg *t_msg);
static INT igmp_show_robust_timerinterval(cmd_msg *t_msg);
static INT igmp_set_grouplife_timerinterval(cmd_msg *t_msg);
static INT igmp_show_grouplife_timerinterval(cmd_msg *t_msg);
static INT igmp_set_vlanlife_timerinterval(cmd_msg *t_msg);
static INT igmp_show_vlanlife_timerinterval(cmd_msg *t_msg);
static INT igmp_add_routeport(LONG vlan_id,ULONG ifindex,ULONG group );
static INT igmp_del_routeport(LONG vlan_id,ULONG ifindex,ULONG group );
static INT igmp_show_mcroute_port(cmd_msg *t_msg);

/********************************************global value****************************************/
struct cfg_element igmp_config_element[] = {
		{"igmp_snoop_enable",0,1,1,(void *)igmp_set_snp_enable},
		{"igmp_snoop_debug",0,1,0,(void *)igmp_set_snp_debug},
		{"add vlan",0,0,0,(void *)igmp_cfg_add_vlan},
		{"add port",0,0,0,(void *)igmp_cfg_add_port},
		{"add trunk port",0,0,0,(void *)igmp_cfg_add_trunkport},
		{"test router port",0,0,0,(void *)igmp_cfg_add_routeport}
};
/************************************extern value****************************************/
extern igmp_routerport	*p_routerlist;
extern LONG	igmp_snoop_enable;	/*IGMP snoop enable or not*/
extern igmp_vlan_list *p_vlanlist;		/*system vlan information*/
extern UINT igmp_snoop_debug;
extern INT	first_vlan_idx;	/*初始值为-1*/
extern ULONG igmp_groupcount;
extern MC_group_vlan	*mcgroup_vlan_queue[IGMP_GENERAL_GUERY_MAX];
extern USHORT igmp_robust_variable;		/*default IGMP_ROBUST_VARIABLE*/
extern ULONG igmp_query_interval;			/*default	IGMP_V2_QUERY_INTERVAL*/
extern ULONG igmp_resp_interval;			/*default IGMP_V2_QUERY_RESP_INTERVAL*/
extern ULONG igmp_query_interval;			/*default	IGMP_V2_QUERY_INTERVAL*/
extern ULONG igmp_grouplife;				/*IGMP_GROUP_LIFETIME*/
extern ULONG igmp_vlanlife;				/*IGMP_VLAN_LIFE_FACTOR*/
/***************************************************************************************/
/***********************************functions ********************************************/
/***************************************************************************************/

void *igmp_set_snp_enable(struct cfg_element *cur,void *value)
{
	char *str = (char *)value + 17 + 2;
	LONG p = atoi(str);		
	if( (p>= cur->min)&&(p<= cur->max))		
		//igmp_snoop_enable = p;
		igmp_set_enable(p);
	else		
		//igmp_snoop_enable = cur->def_value;
		igmp_set_enable(cur->def_value);
}

void *igmp_set_snp_debug(struct cfg_element *cur,void *value)
{
	char *str = (char *)value + 16 +2;
	int p = atoi(str);		
	if( (p>= cur->min)	&&(p<= cur->max))		
		igmp_snoop_debug = p;	
	else		
		igmp_snoop_debug = cur->def_value;
}

void *igmp_cfg_add_vlan(struct cfg_element *cur,void *value)
{
	char *str = (char *)value + 9;
	int p = atoi(str);
	igmp_vlan_list *t_vlan = NULL;

	if( p > 4094 )	/*max value*/
	{
		IGMP_SNP_DEBUG("igmp_cfg_add_vlan:parameter error.\r\n");
		return;
	}
	else
	{
		if( !p_vlanlist )	/*first*/
		{
			t_vlan = (igmp_vlan_list *)malloc(sizeof(igmp_vlan_list));
			if(NULL == t_vlan )
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_vlan:malloc memory failed.\r\n");
				return;
			}
			memset(t_vlan,0,sizeof(igmp_vlan_list));
			t_vlan->next = NULL;
			t_vlan->vlan_id = p;
			t_vlan->first_port = NULL;
			p_vlanlist = t_vlan;
		}
		else
		{
			igmp_vlan_list *new_vlan =NULL;
			t_vlan = p_vlanlist;
			while((t_vlan->vlan_id != p)&&(NULL == t_vlan->next ))
			{
				t_vlan = t_vlan->next;
			}
			if( t_vlan->vlan_id == p)
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_vlan:vlan_id alread existence.\r\n");
				return;
			}
			
			new_vlan= (igmp_vlan_list *)malloc(sizeof(igmp_vlan_list));
			if(NULL == new_vlan )
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_vlan:malloc memory failed.\r\n");
				return;
			}
			memset(new_vlan,0,sizeof(igmp_vlan_list));
			new_vlan->next = NULL;
			new_vlan->vlan_id = p;
			new_vlan->first_port = NULL;
			t_vlan->next = new_vlan;
		}
	}
}

void *igmp_cfg_add_port(struct cfg_element *cur,void *value)
{
	char *str = (char *)value + 9;
	char buf[12];
	int i,vlan_id,ifindex;
	igmp_vlan_list *t_vlan = NULL;
	
	i=0;
	while(*(str+i) !='\t')
		++i;
	memset(buf,0,sizeof(char)*12);
	memcpy(buf,str,sizeof(char)*i);
	vlan_id = atoi(buf);

	str +=i+1;
	ifindex = atoi(str);

	if( !p_vlanlist )
	{
		IGMP_SNP_DEBUG("igmp_cfg_add_port: can not find any vlan.\r\n");
		return;
	}
	else
	{
		t_vlan = p_vlanlist;
		while((t_vlan->vlan_id != vlan_id )&&(NULL == t_vlan))
			t_vlan = t_vlan->next;

		if( NULL == t_vlan )
		{
			IGMP_SNP_DEBUG("igmp_cfg_add_port:can not find vlan.\r\n");
			return;
		}
		{
		igmp_vlan_port_list *t_port = t_vlan->first_port;
		if( NULL == t_port )		/*first*/
		{
			t_port = (igmp_vlan_port_list *)malloc(sizeof(igmp_vlan_port_list));
			if( NULL == t_port)
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_port:malloc memory failed.\r\n");
				return;
			}
			t_port->next = NULL;
			t_port->ifindex = ifindex;
			t_port->trunkflag = 0;		
			t_vlan->first_port = t_port;
		}
		else
		{
			igmp_vlan_port_list *new_port = NULL;
			while( t_port->ifindex != ifindex )
			{
				if( NULL == t_port->next)
					break;
				t_port = t_port->next;
			}
			if( t_port->ifindex == ifindex)
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_port:ifindex has alread existence.\r\n");
				return;
			}
			new_port = (igmp_vlan_port_list *)malloc(sizeof(igmp_vlan_port_list));
			if(NULL == new_port)
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_port:malloc memory failed.\r\n");
				return;
			}
			new_port->next = NULL;
			new_port->ifindex = ifindex;
			new_port->trunkflag = 0;	
			t_port->next = new_port;
		}
		}
	}
}

void *igmp_cfg_add_trunkport(struct cfg_element *cur,void *value)
{
	char *str = (char *)value + 15;
	char buf[12];
	int i,vlan_id,ifindex,trunkifindex;
	igmp_vlan_list *t_vlan = NULL;
	
	i=0;
	while(*(str+i) !='\t')
		++i;
	memset(buf,0,sizeof(char)*12);
	memcpy(buf,str,sizeof(char)*i);
	vlan_id = atoi(buf);
	str +=i +1;
	
	i = 0;
	while(*(str+i)!='\t')
		++i;
	memset(buf,0,sizeof(char)*12);
	memcpy(buf,str,sizeof(char)*i);
	ifindex = atoi(buf);
	
	str +=i+1;
	trunkifindex = atoi(str);

	if( !p_vlanlist )
	{
		IGMP_SNP_DEBUG("igmp_cfg_add_port: can not find any vlan.\r\n");
		return;
	}
	else
	{
		t_vlan = p_vlanlist;
		while((t_vlan->vlan_id != vlan_id )&&(NULL == t_vlan))
			t_vlan = t_vlan->next;

		if( NULL == t_vlan )
		{
			IGMP_SNP_DEBUG("igmp_cfg_add_port:can not find vlan.\r\n");
			return;
		}
		{
		igmp_vlan_port_list *t_port = t_vlan->first_port;
		if( NULL == t_port )		/*first*/
		{
			t_port = (igmp_vlan_port_list *)malloc(sizeof(igmp_vlan_port_list));
			if( NULL == t_port)
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_port:malloc memory failed.\r\n");
				return;
			}
			t_port->next = NULL;
			t_port->ifindex = ifindex;
			t_port->trunkflag = trunkifindex;		
			t_vlan->first_port = t_port;
		}
		else
		{
			igmp_vlan_port_list *new_port = NULL;
			while( t_port->ifindex != ifindex )
			{
				if( NULL == t_port->next)
					break;
				t_port = t_port->next;
			}
			if( t_port->ifindex == ifindex)
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_port:ifindex has alread existence.\r\n");
				return;
			}
			new_port = (igmp_vlan_port_list *)malloc(sizeof(igmp_vlan_port_list));
			if(NULL == new_port)
			{
				IGMP_SNP_DEBUG("igmp_cfg_add_port:malloc memory failed.\r\n");
				return;
			}
			new_port->next = NULL;
			new_port->ifindex = ifindex;
			new_port->trunkflag = trunkifindex;	
			t_port->next = new_port;
		}
		}
	}
}


void *igmp_cfg_add_routeport(struct cfg_element *cur,void *value)
{	/*test, create pim packet*/
	char *str = (char *)value + 17;
	char buf[12];
	int i,vlan_id,ifindex;
	igmp_vlan_list *t_vlan = NULL;
	igmp_routerport *new_routeport = NULL;
	
	i=0;
	while(*(str+i) !='\t')
		++i;
	memset(buf,0,sizeof(char)*12);
	memcpy(buf,str,sizeof(char)*i);
	vlan_id = atoi(buf);

	str +=i+1;
	ifindex = atoi(str);

	igmp_snoop_searchrouterlist(vlan_id,ifindex,IGMP_PORT_ADD,&new_routeport);
}

/**********************************************************************************
*igmp_snp_msg_proc()
*INPUTS:
*command message handle function
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	IGMP SNOOP 
*
***********************************************************************************/
INT igmp_snp_msg_proc(cmd_msg *t_msg, INT *flag)
{
	INT ret;

	if( !t_msg)
	{
		IGMP_SNP_DEBUG("igmp_snp_msg_proc:Msg error.\r\n");
		return IGMP_SNOOP_ERR;
	}

	if( (CMD_MSG_TYPE_BEGIN>= t_msg->msgtype)||
		(CMD_MSG_TYPE_END<= t_msg->msgtype))
	{
		IGMP_SNP_DEBUG("igmp_snp_msg_proc:Msg type error.\r\n");
		return IGMP_SNOOP_ERR;
	}

	switch(t_msg->msgtype)
	{
		case CMD_MSG_TYPE_KERNEL:
			{
			}
			break;
		case CMD_MSG_TYPE_SHOW:
			{
				switch(t_msg->msgcode)
				{
					case CMD_SHOW_GROUPCNT:
						igmp_show_group_cnt(t_msg);
						ret = IGMP_SNOOP_DUMP;
						break;
					case CMD_SHOW_HOSTTIMEOUT:
						igmp_show_host_timerinterval(t_msg);
						ret = IGMP_SNOOP_DUMP;
						break;
					case CMD_SHOW_QUERYTIMEOUT:
						igmp_show_query_timerinterval(t_msg);
						ret = IGMP_SNOOP_DUMP;
						break;
					case CMD_SHOW_RESPTIMEOUT:
						igmp_show_resp_timerinterval(t_msg);
						ret = IGMP_SNOOP_DUMP;
						break;
					case CMD_SHOW_ROBUSTTIMEOUT:
						igmp_show_robust_timerinterval(t_msg);
						ret = IGMP_SNOOP_DUMP;
						break;
					case CMD_SHOW_GROUPLIFETIME:
						igmp_show_grouplife_timerinterval(t_msg);
						ret = IGMP_SNOOP_DUMP;
						break;
					case CMD_SHOW_VLANLIFETIME:
						igmp_show_vlanlife_timerinterval(t_msg);
						ret = IGMP_SNOOP_DUMP;
						break;
					case CMP_SHOW_MCROUTE:
						igmp_show_mcroute_port(t_msg);
						ret = IGMP_SNOOP_DUMP;
						break;
					default:
						IGMP_SNP_DEBUG("igmp_snp_msg_proc:error msg_code.\r\n");
						break;
				}
			}
			break;
		case CMD_MSG_TYPE_NOTIFY:
			{
			}
			break;
		case CMD_MSG_TYPE_CONF:
			{
				switch( t_msg->msgcode )
				{
					case CMD_CONF_ENABLE:
						if(IGMP_SNOOP_OK == igmp_set_enable(IGMP_SNOOP_YES))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_DISABLE:
						if(IGMP_SNOOP_OK == igmp_set_enable(IGMP_SNOOP_NO))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_DEBUG_ON:
						igmp_snoop_debug = 1;
						break;
					case CMD_CONF_DEBUG_OFF:
						igmp_snoop_debug = 0;
						break;
					case CMD_CONF_DEL_SPEC:
						if( IGMP_SNOOP_OK == igmp_del_spec_mcgroup(t_msg))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_DEL_ALL:
						if( IGMP_SNOOP_OK == igmp_del_all_vlan(t_msg))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_SET_QUERYTIME:
						if( IGMP_SNOOP_OK == igmp_set_query_timerinterval(t_msg))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_SET_ROBUSTTIME:
						if( IGMP_SNOOP_OK == igmp_set_robust_timerinterval(t_msg))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_SET_GROUPLIFE:
						if( IGMP_SNOOP_OK == igmp_set_grouplife_timerinterval(t_msg))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_SET_VLANLIFE:
						if( IGMP_SNOOP_OK == igmp_set_vlanlife_timerinterval(t_msg))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_ADD_MCROUTE:
						if( IGMP_SNOOP_OK == igmp_add_mcroute_port(t_msg))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					case CMD_CONF_DEL_MCROUTE:
						if( IGMP_SNOOP_OK == igmp_del_mcroute_port(t_msg))
							t_msg->msgext = CMD_EXT_RETURN_SUCCESS;
						else
							t_msg->msgext = CMD_EXT_RETURN_FAILED;
						ret = IGMP_SNOOP_RETURN;
						break;
					default:
						IGMP_SNP_DEBUG("igmp_snp_msg_proc:error msg_code.\r\n");
						break;
				}
			}
			break;
		case CMD_MSG_TYPE_REQUEST:
			{
			}
			break;
		case CMD_MSG_TYPE_ACK:
			{
			}
			break;
		default:
			IGMP_SNP_DEBUG("igmp_snp_msg_proc:Unknown msg type.\r\n");
			break;
	}
	return ret;
}

/**********************************************************************************
*create_msg_thread()
*INPUTS:
*
*OUTPUTS:
*RETURN VALUE:
*	
*DESCRIPTION:
*	IGMP SNOOP command message handle thread
*
***********************************************************************************/
void *create_msg_thread(void)
{
	int ret,write_flag;
	int sock,accept_sock,len;
	int recv_len = 0;
	cmd_msg *msg;
	struct sockaddr_un client;
	char buf[IGMP_MSG_MAX_SIZE];
	fd_set rfds;

	if(0 >(sock = creatservsock_stream(IGMP_SNOOP_MSG_SOCK)))
	{
		IGMP_SNP_DEBUG("Create msg socket failed.\r\n");
		return;
	}

	memset(&client,0,sizeof(struct sockaddr_un));
	len = sizeof(struct sockaddr_un);

	msg = (cmd_msg *)buf;
	while(1)
	{
		if((accept_sock = accept(sock,(struct sockaddr *)&client,&len))<0)
		{
			IGMP_SNP_DEBUG("Accept failed:errno %d [%s].\r\n",errno,strerror(errno));
			close(sock);
			/*需要增加对其他线程的退出信号*/
			return;
		}
		DEBUG_OUT("create_msg_thread:accept command.\r\n");
		while(1)
		{
			memset(buf,0,sizeof(char)*IGMP_MSG_MAX_SIZE);
			FD_ZERO(&rfds);
			FD_SET(accept_sock,&rfds);
			switch(select(accept_sock+1,&rfds,NULL,NULL,NULL))
			{
				case -1:
					break;
				case 0:
					break;
				default:
					if(FD_ISSET(accept_sock,&rfds))
					{
						recv_len = read(accept_sock,buf,IGMP_MSG_MAX_SIZE);
						if( 0 == recv_len )
							break;
						ret = igmp_snp_msg_proc(msg,&write_flag);
						if( (IGMP_SNOOP_DUMP == ret )
							||(IGMP_SNOOP_RETURN == ret) )	/*need send message*/
						{
							write(accept_sock,buf,IGMP_MSG_MAX_SIZE);
						}
						break;
					}
			}
			if(0 == recv_len )
				break;
		}
		close(accept_sock);
		accept_sock = 0;
	}
	IGMP_SNP_DEBUG("Create msg thread success!\r\n");
}



/*read config file for init igmp snooping*/
void read_config(char *line)
{	
	int i;	
	int tmp;		

	if(!line)		
		return;			
	for(i=0;i<6;++i)	
	{		
		tmp = strlen(igmp_config_element[i].str);		
		if(!strncmp(line,igmp_config_element[i].str,tmp))		
		{			
			igmp_config_element[i].func(igmp_config_element+i,(void *)line);			
			break;		
		}	
	}
}

void write_handle(FILE *fd,FILE *tmp_fd)
{
	int tmp = 0,i;
	char buf[CONF_FILE_MAX_ROW];
	
	memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
	while(fgets(buf,CONF_FILE_MAX_ROW-1,fd))
	{
		if('#' == buf[0])
		{
			fputs(buf,tmp_fd);
			memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
			continue;
		}
		for(i=0;i<6;++i)
		{
			if(!strncmp(buf,igmp_config_element[i].str,strlen(igmp_config_element[i].str)))
			{
				break;
			}
		}
		
		switch(i)
		{
			case 0:
				memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
				sprintf(buf,"igmp_snoop_enable	=%d\n",igmp_snoop_enable);
				fputs(buf,tmp_fd);
				break;
			case 1:
				memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
				sprintf(buf,"igmp_snoop_debug	=%d\n",igmp_snoop_debug);
				fputs(buf,tmp_fd);
				break;
			case 2:
				memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
				//sprintf(buf,"cfgdatalen	=%d\n",test_conf.datalen);
				fputs(buf,tmp_fd);
				break;
			case 3:
				memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
				//sprintf(buf,"cfgtime	=%d\n",test_conf.time);
				fputs(buf,tmp_fd);
				break;
			default:
				fputs(buf,tmp_fd);
				memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
				break;
		}
	}
}

void write_config(char *path,void (*handle)(FILE *,FILE *))
{	
	FILE *fd = NULL;	
	FILE *tmp_fd = NULL;	
	char tmp_path[120];	
	struct stat file_stat;		

	if( !path )		
		return;		
	stat(path,&file_stat);	
	if((fd=fopen(path,"rt"))==NULL)	{		
		fprintf(stderr,"Can not open file.errno:%d [%s]\r\n",errno,strerror(errno));		
		return;	
	}		
	memset(tmp_path,0,120);	
	strncpy(tmp_path,path,strlen(path));	
	strcat(tmp_path,"_1");	
	if( (tmp_fd = fopen(tmp_path,"ab+")) == NULL )	{		
		fprintf(stderr,"Open file failed. errno %d [%s].\r\n",errno,strerror(errno));		
		return;	
		}
	if( handle)		
		handle(fd,tmp_fd);			
	fclose(fd);
	remove(path);
	fclose(tmp_fd);
	rename(tmp_path,path);
	chmod(path,file_stat.st_mode);	
	return;
}

int creat_config_file(char *path)
{
}


INT igmp_set_enable(LONG flag)
{
	if( igmp_snoop_enable == flag )
	{
		DEBUG_OUT("igmp_set_enable:the same state. cur_state:%ld\tset_state:%ld\r\n",
			igmp_snoop_enable,flag);
		return IGMP_SNOOP_OK;
	}
	igmp_snoop_enable = flag;
	if( flag )
	{
		igmp_enable_init();
	}
	else
	{
		igmp_snp_stop();
	}
	return IGMP_SNOOP_OK;
}

INT igmp_del_spec_mcgroup(cmd_msg *t_msg)
{
	cmd_opt_info *opt = NULL;
	MC_group * t_group = NULL;
	MC_group *t_nextgroup = NULL;
	MC_group_vlan *t_gpvlan = NULL;

	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_del_spec_mcgroup:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	
	if( (NULL == t_msg )
		||((sizeof(cmd_opt_info)+sizeof(cmd_msg) )> t_msg->msglen ))
	{
		IGMP_SNP_DEBUG("igmp_del_spec_mcgroup:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	opt = (cmd_opt_info *)(t_msg + 1);

	if( 0 == opt->groupadd )	/*delete all group in this vlan*/
	{
		igmp_snp_delvlan(opt->vlan_id);
	}
	else
	{
		DEBUG_OUT("igmp_del_spec_mcgroup:vlan_id:0x%x\tgroup:%u.%u.%u.%u\r\n",
				opt->vlan_id,NIPQUAD(opt->groupadd));
		if( IGMP_SNOOP_OK !=igmp_searchvlangroup(opt->vlan_id,opt->groupadd,&t_group,&t_gpvlan))
		{
			IGMP_SNP_DEBUG("igmp_del_spec_mcgroup:searchgroupvlan failed.\r\n");
			return IGMP_SNOOP_ERR;
		}

		if( NULL == t_gpvlan )
		{
			IGMP_SNP_DEBUG("igmp_del_spec_mcgroup:this vlan %d is not exist.\r\n",
							opt->vlan_id);
			return IGMP_SNOOP_ERR;
		}
		
		if( NULL == t_group )
		{
			IGMP_SNP_DEBUG("igmp_del_spec_mcgroup:group:%u.%u.%u.%u. is not exist in vlan %d.\r\n",
							NIPQUAD(opt->groupadd),opt->vlan_id);
			return IGMP_SNOOP_ERR;
		}
		igmp_snp_delgroup(t_gpvlan,t_group,&t_nextgroup);
		if( NULL == t_gpvlan->firstgroup )
		{
			igmp_snp_delvlan(t_gpvlan->vlan_id);
		}
		return IGMP_SNOOP_OK;
	}
}

INT igmp_del_all_vlan(cmd_msg *t_msg )
{
	MC_group_vlan *t_gpvlan = NULL;
	INT next = 0;
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_del_all_vlan:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}

	t_gpvlan = GET_VLAN_POINT_BY_INDEX(first_vlan_idx);
	next = first_vlan_idx;
	while(t_gpvlan)
	{
		DEBUG_OUT("igmp_del_all_vlan:del vlan_id:%d",t_gpvlan->vlan_id);
		next = t_gpvlan->next;
		igmp_snp_delvlan(t_gpvlan->vlan_id);
		t_gpvlan = GET_VLAN_POINT_BY_INDEX(next);
	}
	first_vlan_idx = -1;
	return IGMP_SNOOP_OK;
}

INT igmp_show_group_cnt(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_show_group_cnt:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_show_group_cnt:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	t_msg->msgext = igmp_groupcount;
	return IGMP_SNOOP_OK;
}

INT igmp_show_host_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_show_host_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_show_host_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	t_msg->msgext = igmp_robust_variable * igmp_query_interval + igmp_resp_interval;
	return IGMP_SNOOP_OK;
}

INT igmp_show_query_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_show_query_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_show_query_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	t_msg->msgext = igmp_query_interval;
	return IGMP_SNOOP_OK;
}

INT igmp_set_query_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_set_query_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_set_query_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if((10 <= t_msg->msgext )&&(300>=t_msg->msgext))
		igmp_query_interval = t_msg->msgext;
	else
		igmp_query_interval = IGMP_V2_QUERY_INTERVAL;
	return IGMP_SNOOP_OK;
}

INT igmp_show_resp_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_show_resp_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_show_resp_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	t_msg->msgext = igmp_resp_interval;
	return IGMP_SNOOP_OK;
}

INT igmp_show_robust_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_show_robust_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_show_robust_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	t_msg->msgext = igmp_robust_variable;
	return IGMP_SNOOP_OK;
}

INT igmp_set_robust_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_set_robust_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_set_robust_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if((1 <= t_msg->msgext )&&(100>=t_msg->msgext))
		igmp_robust_variable = t_msg->msgext;
	else
		igmp_robust_variable = IGMP_ROBUST_VARIABLE;
	return IGMP_SNOOP_OK;
}

INT igmp_set_grouplife_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_set_grouplife_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_set_grouplife_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if((10 <= t_msg->msgext )&&(1000>=t_msg->msgext))
		igmp_grouplife = t_msg->msgext;
	else
		igmp_grouplife = IGMP_GROUP_LIFETIME;
	return IGMP_SNOOP_OK;
}

INT igmp_show_grouplife_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_show_grouplife_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_show_grouplife_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	t_msg->msgext = igmp_grouplife;
	return IGMP_SNOOP_OK;
}


INT igmp_set_vlanlife_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_set_vlanlife_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_set_vlanlife_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if((10 <= t_msg->msgext )&&(100000>=t_msg->msgext))
		igmp_vlanlife = t_msg->msgext;
	else
		igmp_vlanlife = IGMP_VLAN_LIFE_FACTOR;
	return IGMP_SNOOP_OK;
}

INT igmp_show_vlanlife_timerinterval(cmd_msg *t_msg)
{
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_show_vlanlife_timerinterval:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( NULL == t_msg )
	{
		IGMP_SNP_DEBUG("igmp_show_vlanlife_timerinterval:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	t_msg->msgext = igmp_vlanlife;
	return IGMP_SNOOP_OK;
}

INT igmp_add_mcroute_port(cmd_msg *t_msg)
{
	INT	existed = 0;
	INT	ret =IGMP_SNOOP_OK;
	cmd_opt_info *opt = NULL;
	igmp_vlan_list *t_vlanlist = NULL;
	igmp_vlan_list *find_vlanlist = NULL;
	igmp_vlan_port_list	*t_port = NULL;
	
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_add_mcroute_port:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( (NULL == t_msg )
		||((sizeof(cmd_opt_info)+sizeof(cmd_msg) )> t_msg->msglen ))
	{
		IGMP_SNP_DEBUG("igmp_add_mcroute_port:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	opt = (cmd_opt_info *)(t_msg + 1);

	if( ( 0 == opt->vlan_id )||(0 == opt->ifindex ) )
	{
		IGMP_SNP_DEBUG("igmp_add_mcroute_port:data error.vlan_id:%d\tifindex:0x%x\r\n",
				opt->vlan_id,opt->ifindex);
		return IGMP_SNOOP_ERR;
	}
	t_vlanlist = p_vlanlist;
	while(t_vlanlist )
	{
		if( t_vlanlist->vlan_id == opt->vlan_id )
		{
			find_vlanlist = t_vlanlist;
			break;
		}
		t_vlanlist = t_vlanlist->next;
	}
	if( find_vlanlist )	/*find vlan*/
	{
		if( find_vlanlist->first_port )
		{
			t_port = find_vlanlist->first_port;
			while(t_port)
			{
				if(t_port->ifindex == opt->ifindex )
				{
					existed = 1;
					break;
				}
				t_port = t_port->next;
			}
		}
		else
		{
			IGMP_SNP_DEBUG("igmp_add_mcroute_port:no any ports in this vlan %d\r\n",
				opt->vlan_id);
			return IGMP_SNOOP_ERR;
		}
	}
	else
	{
		IGMP_SNP_DEBUG("igmp_add_mcroute_port:vlan %d is not exist.\r\n",
				opt->vlan_id);
		return IGMP_SNOOP_ERR;
	}

	if( existed )
	{
		ret = igmp_add_routeport(opt->vlan_id,opt->ifindex,0);
	}
	return ret;
}

INT igmp_del_mcroute_port(cmd_msg *t_msg)
{
	INT	existed = 0;
	INT	ret =IGMP_SNOOP_OK;
	cmd_opt_info *opt = NULL;
	igmp_vlan_list *t_vlanlist = NULL;
	igmp_vlan_list *find_vlanlist = NULL;
	igmp_vlan_port_list	*t_port = NULL;
	
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_del_mcroute_port:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( (NULL == t_msg )
		||((sizeof(cmd_opt_info)+sizeof(cmd_msg) )> t_msg->msglen ))
	{
		IGMP_SNP_DEBUG("igmp_del_mcroute_port:parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}
	opt = (cmd_opt_info *)(t_msg + 1);

	if( ( 0 == opt->vlan_id )||(0 == opt->ifindex ) )
	{
		IGMP_SNP_DEBUG("igmp_del_mcroute_port:data error.vlan_id:%d\tifindex:0x%x\r\n",
				opt->vlan_id,opt->ifindex);
		return IGMP_SNOOP_ERR;
	}
	t_vlanlist = p_vlanlist;
	while(t_vlanlist )
	{
		if( t_vlanlist->vlan_id == opt->vlan_id )
		{
			find_vlanlist = t_vlanlist;
			break;
		}
		t_vlanlist = t_vlanlist->next;
	}
	if( find_vlanlist )	/*find vlan*/
	{
		if( find_vlanlist->first_port )
		{
			t_port = find_vlanlist->first_port;
			while(t_port)
			{
				if(t_port->ifindex == opt->ifindex )
				{
					existed = 1;
					break;
				}
				t_port = t_port->next;
			}
		}
		else
		{
			IGMP_SNP_DEBUG("igmp_del_mcroute_port:no any ports in this vlan %d\r\n",
				opt->vlan_id);
			return IGMP_SNOOP_ERR;
		}
	}
	else
	{
		IGMP_SNP_DEBUG("igmp_del_mcroute_port:vlan %d is not exist.\r\n",
				opt->vlan_id);
		return IGMP_SNOOP_ERR;
	}

	if( existed )
	{
		ret = igmp_del_routeport(opt->vlan_id,opt->ifindex,0);
	}
	return ret;
}


INT igmp_add_routeport(LONG vlan_id,ULONG ifindex,ULONG group )
{
	MC_group * t_group = NULL;
	igmp_routerport *new_routeport = NULL;
	MC_group_vlan *t_gpvlan = NULL;
	igmp_router_entry *t_router = NULL;
	igmp_router_entry *pre_router = NULL;

	DEBUG_OUT("igmp_add_routeport:vlan_id:%d\tifindex:0x%x\tgroup:%u.%u.%u.%u\r\n",
			vlan_id,ifindex,NIPQUAD(group));
	if( IGMP_SNOOP_OK != igmp_searchvlangroup(vlan_id,group,&t_group,&t_gpvlan))
	{
		IGMP_SNP_DEBUG("igmp_add_routeport:search mc group failed.\r\n");
		return IGMP_SNOOP_ERR;
	}

	if( IGMP_SNOOP_OK !=igmp_snoop_searchrouterlist(vlan_id,ifindex,IGMP_PORT_ADD,&new_routeport))
	{
		IGMP_SNP_DEBUG("igmp_add_routeport:add router failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( 0 == group )
	{
		if( NULL == t_gpvlan )
			return IGMP_SNOOP_OK;
		if( vlan_id != t_gpvlan->vlan_id )
			return IGMP_SNOOP_OK;
		if( NULL != t_gpvlan->routerlist )
		{
			t_router = t_gpvlan->routerlist;
			while(t_router)
			{
				pre_router = t_router;
				t_router = t_router->next;
				free(pre_router);
			}
			t_gpvlan->routerlist = NULL;
		}
		t_router = NULL;
		t_router = (igmp_router_entry *)malloc(sizeof(igmp_router_entry));
		if( NULL == t_router )
		{
			IGMP_SNP_DEBUG("igmp_add_routeport:malloc memory failed.\r\n");
			return IGMP_SNOOP_ERR;
		}
		memset(t_router,0,sizeof(igmp_router_entry));
		t_router->mroute_ifindex = ifindex;
		t_gpvlan->routerlist = t_router;
		DEBUG_OUT("igmp_add_routeport:add route port success. vlan_id:%d\tifindex:0x%x\r\n",
			vlan_id,ifindex);
	}
	return IGMP_SNOOP_OK;
}

INT igmp_del_routeport(LONG vlan_id,ULONG ifindex,ULONG group )
{
	MC_group * t_group = NULL;
	igmp_routerport *new_routeport = NULL;
	MC_group_vlan *t_gpvlan = NULL;
	igmp_router_entry *t_router = NULL;
	igmp_router_entry *pre_router = NULL;

	DEBUG_OUT("igmp_del_routeport:vlan_id:%d\tifindex:0x%x\tgroup:%u.%u.%u.%u\r\n",
			vlan_id,ifindex,NIPQUAD(group));
	if( IGMP_SNOOP_OK != igmp_searchvlangroup(vlan_id,group,&t_group,&t_gpvlan))
	{
		IGMP_SNP_DEBUG("igmp_del_routeport:search mc group failed.\r\n");
		return IGMP_SNOOP_ERR;
	}

	if( IGMP_SNOOP_OK !=igmp_snoop_searchrouterlist(vlan_id,ifindex,IGMP_PORT_DEL,&new_routeport))
	{
		IGMP_SNP_DEBUG("igmp_add_routeport:add router failed.\r\n");
		return IGMP_SNOOP_ERR;
	}

	if( 0 == group )
	{
		if( NULL == t_gpvlan )
			return IGMP_SNOOP_OK;
		if( vlan_id != t_gpvlan->vlan_id )
			return IGMP_SNOOP_OK;
		if( NULL != t_gpvlan->routerlist )
		{
			t_router = t_gpvlan->routerlist;
			while(t_router)
			{
				pre_router = t_router;
				t_router = t_router->next;
				free(pre_router);
			}
			t_gpvlan->routerlist = NULL;
		}
	}
	return IGMP_SNOOP_OK;;
}


INT igmp_show_mcroute_port(cmd_msg *t_msg)
{
	igmp_routerport *t_routerport = NULL;
	cmd_opt_info *opt = NULL;
	
	if( !IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("igmp_del_mcroute_port:igmp snoop is disable.\r\n");
		return IGMP_SNOOP_ERR;
	}
}
