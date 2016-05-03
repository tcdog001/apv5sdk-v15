/*********************************************************************************************
*			Copyright(c), 2008, Autelan Technology Co.,Ltd.
*						All Rights Reserved
*
**********************************************************************************************
$RCSfile:igmp_snoop_com.c
$Author: Rock
$Revision: 1.00
$Date:2008-3-8 09:12
***********************************************************************************************/
#include "igmp_snoop_com.h"

int creat_servsocket(char *path)
{
	int sock,len,oldmask;
	struct sockaddr_un serv;
	
	if(!path)
		return -1;
	
	unlink(path);
	oldmask =umask(0077);
	if((sock = socket(AF_UNIX,SOCK_DGRAM,0))<0)
	{
		printf("Creat socket failed.\r\n");
		perror(NULL);
		return -1;
	}
	
	memset(&serv,0,sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	strncpy(serv.sun_path,path,strlen(path));
	len = strlen(serv.sun_path) + sizeof(serv.sun_family);
	if(bind(sock,(struct sockaddr*)&serv,len)<0)
	{
		printf("Bind failed.\r\n");
		perror(NULL);
		close(sock);
		return -1;
	}
	umask(oldmask);
	return sock;
}

int send_datasocket(char *path,char *data,int data_len)
{
	int sock,send_len;
	struct sockaddr_un send;
	char buf[1024];
	
	if( !path||!data)
		return -1;
	if(1024<data_len)
		return -1;
		
	if((sock = socket(AF_UNIX,SOCK_DGRAM,0))<0)
	{
		printf("Creat socket failed.\r\n");
		perror(NULL);
		return -1;
	}
	
	memset(&send,0,sizeof(struct sockaddr_un));
	send.sun_family = AF_UNIX;
	strncpy(send.sun_path,path,strlen(path));
	memset(buf,0,sizeof(char)*1024);
	memcpy(buf,data,data_len);
	send_len = sendto(sock,buf,data_len,0,(struct sockaddr*)&send,
					sizeof(struct sockaddr_un));
	if(send_len<0)
	{
		printf("Send data failed.\r\n");
		perror(NULL);
		close(sock);
		return -1;
	}
	close(sock);
	return 0;
}

int creatclientsock_stream(char *path)
{
	int sock,len;
	struct sockaddr_un serv;
	
	if(!path)
		return -1;
	
	if((sock = socket(AF_UNIX,SOCK_STREAM,0))<0)
	{
		printf("Creat socket failed.\r\n");
		perror(NULL);
		return -1;
	}
	
	memset(&serv,0,sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	strncpy(serv.sun_path,path,strlen(path));
	len = strlen(serv.sun_path)+sizeof(serv.sun_family);
	
	if(connect(sock,(struct sockaddr*)&serv,len)<0)
	{
		printf("Connect failed.\r\n");
		perror(NULL);
		close(sock);
		return -1;
	}
	return sock;
}

int creatclientsock_stream_unblock(char *path)
{
	int sock,len,flags;
	struct sockaddr_un serv;
	
	if(!path)
		return -1;
	
	if((sock = socket(AF_UNIX,SOCK_STREAM,0))<0)
	{
		printf("Creat socket failed.\r\n");
		perror(NULL);
		return -1;
	}
	
	flags = fcntl(sock,F_GETFL,0);
	fcntl(sock,F_SETFL,flags|O_NONBLOCK);
	
	memset(&serv,0,sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	strncpy(serv.sun_path,path,strlen(path));
	len = strlen(serv.sun_path)+sizeof(serv.sun_family);
	
	if(connect(sock,(struct sockaddr*)&serv,len)<0)
	{
		if( errno != EINPROGRESS )
		{
			printf("Connect failed.\r\n");
			perror(NULL);
			close(sock);
			return -1;
		}
	}
	return sock;
}

int creatservsock_stream(char *path)
{
	int sock,len;
	struct sockaddr_un serv;
	
	if( !path )
		return -1;
	
	unlink(path);
	
	if((sock = socket(AF_UNIX,SOCK_STREAM,0))<0)
	{
		printf("Creat socket failed.\r\n");
		perror(NULL);
		return -1;
	}
	
	memset(&serv,0,sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	strncpy(serv.sun_path,path,strlen(path));
	len = sizeof(serv.sun_family)+strlen(serv.sun_path);
	
	if( bind(sock,(struct sockaddr*)&serv,len)<0)
	{
		printf("Bind failed.\r\n");
		perror(NULL);
		close(sock);
		return -1;
	}
	
	if( listen(sock,5)<0 )
	{
		printf("Listen failed.\r\n");
		perror(NULL);
		close(sock);
		return -1;
	}
	
	return sock;
}

/*读取配置文件的默认函数*/
void *def_func(struct cfg_element *cur,void *value)
{	
	return;
}

/*************************************建立定时器************************/
struct timer_element *create_timer(unsigned int type,unsigned int pri,
						unsigned long interval, 
						void (*func)(struct timer_element *),void *data,unsigned int datalen)
{
	struct timer_element *new_timer = NULL;
	
	if( TIMER_TYPE_MAX < type )
	{
		printf("Create_timer: type error.\r\n");
		return 0;
	}
	if( TIMER_PRIORI_HIGH < pri )
	{
		printf("Create_timer: priority error.\r\n");
		return 0;
	}
	if( NULL == func)
	{
		printf("Create_timer: handle function is not existence.\r\n");
		return 0;
	}
	
	new_timer = (struct timer_element *)malloc(sizeof(struct timer_element));
	if( NULL == new_timer )
	{
		printf("Create_timer: malloc memory failed.\r\n");
		return 0;
	}
	memset(new_timer,0,sizeof(struct timer_element));
	new_timer->next = NULL;
	new_timer->type = type;
	new_timer->priority = pri;
	new_timer->expires = interval;
	new_timer->current = 0;
	new_timer->data = data;
	new_timer->datalen = datalen;
	new_timer->func = func;
	
	return new_timer;
}

/*********************************增加定时器***************************/
int add_timer(struct timer_list *head, struct timer_element *new_timer, unsigned long *ptimer_id)
{
	int i,timer_id,flag;
	struct timer_element *tnext = NULL;
	struct timer_element *tprev = NULL;
	
	if( !head || !new_timer )
	{
		printf("add_timer:parameter error.\r\n");
		return -1;
	}
	timer_id = rand();
	while(head->lock)
		head->lock = 0;
	
	if( head->cnt >= (TIMER_LIST_MAX - 1) )
	{
		head->lock = 1;
		printf("add_timer:timer element too many.\r\n");
		return -1;
	}
	flag = 1;
	do{
		tnext = head->first_timer;
		while(tnext)
		{
			if( tnext->id == timer_id )
			{
				timer_id = rand();
				break;
			}
			tnext = tnext->next;
		}
	}while(tnext);
	new_timer->id = timer_id;
	if( NULL != ptimer_id )
		*ptimer_id = timer_id;
	if( NULL == head->first_timer )
	{
		head->first_timer = new_timer;
		head->cnt = 1;
	}
	else
	{
		tnext = head->first_timer;
		tprev = head->first_timer;
		
		while(tnext->type > new_timer->type)
		{
			tprev = tnext;
			tnext = tnext->next;
			if( NULL == tnext )
				break;
		}
		if( NULL == tnext )	/*链表尾*/
		{
			tprev->next = new_timer;
		}
		else
		{
			while( tnext->priority >= new_timer->priority )
			{
				tprev = tnext;
				tnext = tnext->next;
				if( NULL == tnext )
					break;
			}
			tprev->next = new_timer;
			new_timer->next = tnext;
		}
		head->cnt++;
	}
	head->lock = 1;
	return 0;
}

/*********************************删除定时器***************************/
int del_timer(struct timer_list *head, unsigned int timer_id)
{
	struct timer_element *tnext = NULL;
	struct timer_element *tprev = NULL;
	
	if( !head || !timer_id )
	{
		printf("del_timer:parameter failed.\r\n");
		return -1;
	}
	
	while(head->lock)
		head->lock = 0;
		
	tnext = head->first_timer;
	tprev = head->first_timer;
	while(tnext)
	{
		if( tnext->id == timer_id )
		{
			if( tprev != tnext )
			{
				tprev->next = tnext->next;
			}
			else
			{
				head->first_timer = tnext->next;
			}
			head->lock = 1;
			free(tnext);
			head->cnt--;
			if( 0 == head->cnt )
				head->first_timer = NULL;
			return 0;
		}
		tprev = tnext;
		tnext = tnext->next;
	}
	printf("del_timer:can not find timer.\r\n");
	return -1;
}

/**********************************删除所有定时器***************************/
int del_all_timer(struct timer_list *head)
{
	int id,cnt;
	
	if( !head )
	{
		printf("add_timer:parameter error.\r\n");
		return -1;
	}
	
	cnt = head->cnt;
	while(cnt)
	{
		if( NULL != head->first_timer )
		{
			id = head->first_timer->id;
			del_timer(head,id);
		}
		cnt--;
	}
	return 0;
}

