/*********************************************************************************************
*			Copyright(c), 2008, Autelan Technology Co.,Ltd.
*						All Rights Reserved
*
**********************************************************************************************
$RCSfile:igmp_snoop_com.c
$Author: Rock
$Revision: 1.00
$Date:2008-3-8 11:11
***********************************************************************************************/
#ifndef __IGMP_SNOOP_COM_H__
#define __IGMP_SNOOP_COM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <pthread.h>

#define CONF_FILE_MAX_LINE	50		/*max lines in the config file*/
#define CONF_FILE_MAX_ROW	100		/*max chars in the line*/

struct cfg_element{	
	char	*str;		/*string*/	
	int		min;	
	int		max;	
	int		def_value;	
	void 	(*func)(struct cfg_element *cur,void *value);	/*callback function*/
};

/********************************** timer structure****************************************/
struct timer_element{
	struct timer_element	*next;
	unsigned int	id;
	unsigned int	type;
	unsigned int	priority;
	unsigned long	expires;
	unsigned long	current;
	void			*data;
	unsigned int 	datalen;
	void			(*func)(struct timer_element *);
};

struct timer_list{
	struct timer_element *first_timer;
	unsigned int	cnt;
	unsigned int	lock;
};

#define	TIMER_TYPE_MIN	0
#define TIMER_TYPE_LOOP	1
#define TIMER_TYPE_NOLOOP	2
#define TIMER_TYPE_MAX	10

#define TIMER_PRIORI_LOW	0
#define TIMER_PRIORI_NORMAL	1
#define TIMER_PRIORI_HIGH	2

#define TIMER_LIST_MAX	256

/***********************************socket interface function***********************************/
extern int creat_servsocket(char *path);
extern int send_datasocket(char *path,char *data,int data_len);

extern int creatclientsock_stream_unblock(char *path);
extern int creatclientsock_stream(char *path);
extern int creatservsock_stream(char *path);

/***********************************Timer interface function************************************/
extern struct timer_element *create_timer(unsigned int type,unsigned int pri,
				unsigned long interval,void (*func)(struct timer_element *),void *data,unsigned int datalen);
extern int add_timer(struct timer_list *head, struct timer_element *new_timer, unsigned long *ptimer_id);
extern int del_timer(struct timer_list *head, unsigned int timer_id);
extern int del_all_timer(struct timer_list *head);

#ifdef __cplusplus
}
#endif

#endif

