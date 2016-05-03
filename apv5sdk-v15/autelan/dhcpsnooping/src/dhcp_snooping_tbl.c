 
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: dhcp_snooping_tbl.c
* description:  implementation for dhcp_snooping table for update ,delete ,add and so on
* 
*
* 
************************************************************************************/

#include <sys/types.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <linux/if_ether.h>
#include <time.h>


#include "dhcp_snooping_if.h"
#include "dhcp_snooping_options.h"
#include "dhcp_snooping_pkt.h"
#include "dhcp_snooping_tbl.h"
#include "dhcp_snooping_main.h"

extern struct interface *int_list;
extern struct interface *int_last;

extern int g_sockfd;  /*add by sundaolian*/
dhcp_snooping_tbl_item_t* g_DHCP_Snooping_Hash_Table[DHCP_SNOOPING_HASH_TABLE_SIZE] = {0};

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_tbl_initialize(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   initialize DHCP Snooping bind table
 ******************************************************************************
 */
u_int32_t dhcp_snoop_tbl_initialize(void)
{
    
    memset(g_DHCP_Snooping_Hash_Table, 0, DHCP_SNOOPING_HASH_TABLE_SIZE*4);
    return DHCP_OK;
}

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_tbl_hash(uchar *mac)
 *  Input      :   mac address of user
 *  Output     :   NULL
 *  Return     :   hash value
 *  Description:   get the hash value according the user mac address
 ******************************************************************************
 */
u_int32_t dhcp_snoop_tbl_hash(u_int8_t *mac)
{
    u_int32_t h = 0;
	int i;
	
	for(i = 0; i < 6; i++)
	{
		h = h + mac[i];
	}	
    h = (h) % DHCP_SNOOPING_HASH_TABLE_SIZE;
    return h;
}

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_tbl_destroy(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   release DHCP Snooping bind table momery
 ******************************************************************************
 */
u_int32_t dhcp_snoop_tbl_destroy(void)
{
	int i;
	dhcp_snooping_tbl_item_t* nextItem;
	dhcp_snooping_tbl_item_t* item;
	for(i=0; i<DHCP_SNOOPING_HASH_TABLE_SIZE; i++)
	{
		item = g_DHCP_Snooping_Hash_Table[i];
		while( item != NULL )
		{
			nextItem = item->next;
			free(item);
			item = nextItem;
		}
	}
    return DHCP_OK;
}

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_tbl_item_find(dhcp_snooping_user_item_t *user,
                              dhcp_snooping_tbl_item_t *item)
 *  Input      :   user
 *  Output     :   item
 *  Return     :   OK/FAIL
 *  Description:   Get the item of specifical user
 ******************************************************************************
 */


void *dhcp_snoop_tbl_item_find(dhcp_snooping_user_item_t *user)
{
	u_int32_t key;
	int match = 0;
	dhcp_snooping_tbl_item_t *item = NULL;
	dhcp_snooping_tbl_item_t* nextItem;
	dhcp_snooping_tbl_item_t* tempItem;

	if ( user == NULL )
	{
		printf("user pointer is NULL\r\n");
		return item;
	}
	key = dhcp_snoop_tbl_hash(user->chaddr);

	if(key > DHCP_SNOOPING_HASH_TABLE_SIZE)
	{
		printf("error in calculate the hash value\r\n");
		return NULL;
	} 
	
	tempItem = g_DHCP_Snooping_Hash_Table[key];
	while((tempItem != NULL)&&(!match)) 
	{
		if (memcmp(tempItem->chaddr, user->chaddr, 6) == 0)
		{
			match = 1;
			item = tempItem;
		}
		tempItem = tempItem->next;
	}		
	if (!match)
	{
		printf("do not find the special entry\r\n");
		//item = NULL;
		return NULL;
	}
	return item;
}

void dhcp_snoop_update_logfile(void)
{
	FILE *fp;
	int ifcount;
	int i;
	dhcp_snooping_tbl_item_t* preItem;
	dhcp_snooping_tbl_item_t* nextItem;
	dhcp_snooping_tbl_item_t* item;
	static int currentTime;
	int counter;	
	char str[256];
	struct interface *ife;
	int flag = 0;

	printf("enter the log update routine\r\n");
	if (!(fp = fopen("/tmp/dhcpsnooping.log", "w"))) 
	{
		printf("Unable to open log for writing");
		return ;
	}	

	for(i=0; i<DHCP_SNOOPING_HASH_TABLE_SIZE; i++)
	{
		item = g_DHCP_Snooping_Hash_Table[i];
		while( item != NULL )
		{
		    flag = 1;
			/*scan all the bind table, age the expired items*/
			time_t tmp_time=item->sys_escape;
			strftime(str,sizeof(str),"%Y-%m-%d %H:%M:%S",localtime(&tmp_time));
			//sprintf(str, "Time:%08d", item->sys_escape);
			fwrite(str, 1, strlen(str), fp);

			sprintf(str, " MAC:%02x:%02x:%02x:%02x:%02x:%02x", item->chaddr[0], 
				item->chaddr[1], item->chaddr[2],item->chaddr[3],item->chaddr[4],item->chaddr[5]);
			fwrite(str, 1, strlen(str), fp);

			//sprintf(str, " VID:%04d", item->vlanId);
			//fwrite(str, 1, strlen(str), fp);

			char ip_buf[16];
		//	printf("\nitem->ip_addr:%s",item->ip_addr);
			sprintf(str, " IP:%s", inet_ntop(AF_INET,&(item->ip_addr),ip_buf,sizeof(ip_buf)));
			fwrite(str, 1, strlen(str), fp);

				
			for (ife = int_list; ife; ife = ife->next) 
			{
				if (ife->ifindex == item->ifindex)
				{
					if (!strncmp(ife->name,"ath",3))
					{
						sprintf(str, "%s", ife->name);
						ifcount = str[3]-'0';
						printf("name:%s,str[3]:%c ,ifcount:%d ,str[4]:%c\n",ife->name,str[3],ifcount,str[4]);
						
						if(str[4] != '\0')
							ifcount = ifcount * 10+(str[4]-'0');
						ifcount++;
					}	
					sprintf(str, " IF:vap%d", ifcount);
					fwrite(str, 1, strlen(str), fp);
				}
			}

			sprintf(str, " Lease:%dh", item->lease_time/3600);
			fwrite(str, 1, strlen(str), fp);

			fwrite("\r\n", 1, 2, fp);			
			item = item->next;
		}
	}
	if(!flag)
		printf("\nno content write to dhcpsnooping.log");
	fclose(fp);
}

/*
 ******************************************************************************
 *  Function   :   void dhcp_snoop_tbl_fill_item(dhcp_snooping_user_info_t *user,
                        dhcp_snooping_database_item_t *item)
 *  Input      :   user
 *  Output     :   item
 *  Return     :   OK/FAIL
 *  Description:   fill the dhcp bind table according user information
 ******************************************************************************
 */
u_int32_t dhcp_snoop_tbl_fill_item(dhcp_snooping_user_item_t *user,
        dhcp_snooping_tbl_item_t *item)
{
    if((user == NULL) || (item == NULL))
    {
		printf("error pointer NULL\r\n");
        return DHCP_FAIL;
    }
    memcpy(item->chaddr, user->chaddr, 6);
    item->haddr_len     = user->haddr_len;	
    item->bind_type     = user->bind_type;
    item->state         = user->state;
    item->ifindex       = user->ifindex;
    item->ip_addr       = user->ip_addr;
    item->lease_time    = user->lease_time;
    item->vlanId        = user->vlanId;		
    item->sys_escape    = time(0);
    //dhcp_snoop_update_logfile();
    return DHCP_OK;
}


/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_tbl_item_insert(dhcp_snooping_user_item_t *user,dhcp_snooping_tbl_item_t *item)
 *  Input      :   user
 *  Output     :   item
 *  Return     :   OK/FAIL
 *  Description:   insert the user bind information into the bind table
 ******************************************************************************
 */
void *dhcp_snoop_tbl_item_insert(dhcp_snooping_user_item_t *user)
{
	int key;
	dhcp_snooping_tbl_item_t* item = NULL;
	dhcp_snooping_tbl_item_t* nextItem;
	dhcp_snooping_tbl_item_t* tempItem;
	if ( user == NULL )
	{
		printf("user error pointer NULL");
		return NULL;
	}
	item = malloc(sizeof(dhcp_snooping_tbl_item_t));
	if (!item)
	{
		printf("can not malloc the memory\r\n");			
		return NULL;
	}
	printf("\ndhcp_snoop_tbl_item_insert: user->ifindex:%u",user->ifindex);
	printf("\ndhcp_snoop_tbl_item_insert: user->ip_addr:%u",user->ip_addr);
	if (dhcp_snoop_tbl_fill_item(user, item)!= DHCP_OK)
	{
		printf("dhcp snooping table fill error\r\n");			
		free(item);
		return NULL;
	}
	key = dhcp_snoop_tbl_hash(user->chaddr);
	if(key > DHCP_SNOOPING_HASH_TABLE_SIZE)
	{
		printf("error in calculate the hash value\r\n");
		return NULL;
	}	
	item->next = g_DHCP_Snooping_Hash_Table[key];
	g_DHCP_Snooping_Hash_Table[key] = item;
	dhcp_snoop_update_logfile();
    return item;
}

/*
*
*function:
* update the interface index.eg set ath.0-1 ifindex  9
*add by sundaolian
*/
u_int32_t updateinterfaceindex(int sock)
{
   struct interface *ife;
   for(ife = int_list;ife;ife=ife->next)
   {
	    struct ifreq ifr;
		strncpy(ifr.ifr_name,ife->name,strlen(ife->name)+1);
		if((ioctl(sock,SIOCGIFINDEX,&ifr) == -1))
		{
			printf("Couldn't retrive ifindex for the interface %s\r\n", ife->name);
			continue;  // find the false interface ,and don't update the interface,go next
		}
		ife->ifindex = ifr.ifr_ifindex;
		printf("update interface %s ifindex is ::%d\r\n",ife->name, ife->ifindex);
	}
   return DHCP_OK;
}
/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_tbl_item_delete(dhcp_snooping_tbl_item_t *item)
 *  Input      :   user
 *  Output     :   item
 *  Return     :   OK/FAIL
 *  Description:   delete the user bind item from the bind table
 ******************************************************************************
 */
u_int32_t dhcp_snoop_tbl_item_delete(dhcp_snooping_tbl_item_t *item)
{
    int key;
    dhcp_snooping_tbl_item_t* tempItem;
    /*********wangjr ******20100311***************/
	char cmd_buf[100]={0};
	struct interface *ife;
	char wlan_id[10] ={0};
	unsigned char radio_id = 0;
	#if 0
	printf("\n\n -----------------------item->chaddr = %d",item->chaddr[0]);
	printf("\n\n -----------------------item->chaddr = %d",item->chaddr[1]);
	printf("\n\n -----------------------item->chaddr = %d",item->chaddr[2]);
	printf("\n\n -----------------------item->chaddr = %d",item->chaddr[3]);
	printf("\n\n -----------------------item->chaddr = %d",item->chaddr[4]);
	printf("\n\n -----------------------item->chaddr = %d",item->chaddr[5]);
	printf("\n\n -----------------------item->ip_addr = %u",item->ip_addr);
	#endif
		  
   /*sundaolian add*/
   set_all_interfaces_promisc(g_sockfd);
   updateinterfaceindex(g_sockfd);  
   /*add end*/
   
	printf("\n------------------------------1---------------------------------\n");
	for (ife=int_list;ife;ife=ife->next)
	{
	  printf("int_list ifindex:   %d,int_list name:%s \n",ife->ifindex,ife->name );
	}
	 printf("item->ifindex :%d\n",item->ifindex );
	 printf("\n------------------------------2---------------------------------\n");

	for (ife = int_list; ife; ife = ife->next) 
	{
		if (ife->ifindex == item->ifindex)
		{
			if (!strncmp(ife->name,"ath",3))
			{
			   if(strlen(ife->name) == 7 )
					{
					radio_id = ife->name[4];
					sprintf(wlan_id, "%c",ife->name[6]);
			        }
			   else if(strlen(ife->name) == 8 )
					{
					radio_id = ife->name[4];
					sprintf(wlan_id, "%c%c",ife->name[6],ife->name[7]);
			        }
			   else			   	 
					{
					 radio_id = ife->name[4];
			          strncpy(wlan_id,ife->name + 6,strlen(ife->name)-6); /*ath.0-128*/
					}
			}	
		}
	}

	if(strlen(wlan_id)==0)
		printf("\n\nNoooooo! wlan is zeroooooooo----delete\n");

		//sprintf(cmd_buf,"/usr/sbin/wtpd_alarm 127.0.0.1 1 11 %02x%02x%02x%02x%02x%02x %c %03d %04d %u",item->chaddr[0], item->chaddr[1], item->chaddr[2],item->chaddr[3],item->chaddr[4],item->chaddr[5],radio_id,atoi(wlan_id),item->vlanId,item->ip_addr);
		sprintf(cmd_buf,"/usr/sbin/wtpd_alarm_all 127.0.0.1 1 11 %02x%02x%02x%02x%02x%02x %c %03d %04d %u",item->chaddr[0], item->chaddr[1], item->chaddr[2],item->chaddr[3],item->chaddr[4],item->chaddr[5],radio_id,atoi(wlan_id),item->vlanId,item->ip_addr);

		system(cmd_buf);
	printf("\n\n -----------------------cmd_buf=%s",cmd_buf);

	key = dhcp_snoop_tbl_hash(item->chaddr);
	if(key > DHCP_SNOOPING_HASH_TABLE_SIZE)
	{
		printf("error in calculate the hash value\r\n");
		return DHCP_FAIL;
	} 
	
	if(g_DHCP_Snooping_Hash_Table[key] == item)
	{
		g_DHCP_Snooping_Hash_Table[key] = item->next;
		free(item);
		dhcp_snoop_update_logfile();
		return DHCP_OK;
	}
	tempItem = g_DHCP_Snooping_Hash_Table[key];
	while((tempItem->next != NULL)) 
	{
		if (tempItem->next == item)
		{
			tempItem->next = item->next;
			free(item);
			dhcp_snoop_update_logfile();
			return DHCP_OK;
		}
		tempItem = tempItem->next;
	}	
	//printf("do not find the special entry\r\n");	
	return DHCP_FAIL;
}


/*
 ******************************************************************************
 *  Function   :   void dhcp_tbl_item(dhcp_snooping_user_info_t *user,
                        dhcp_snooping_database_item_t *item)
 *  Input      :   item
 *  Output     :   user
 *  Return     :   OK/FAIL
 *  Description:   get the dhcp bind table item information
 ******************************************************************************
 */
u_int32_t dhcp_snoop_tbl_get_item(dhcp_snooping_user_item_t *user,
        dhcp_snooping_tbl_item_t *item)
{
    if((user  == NULL) || (item == NULL))
    {
		printf("error pointer NULL");
        return DHCP_FAIL;
    }
     
    memset(user,0,sizeof(dhcp_snooping_user_item_t));
    user->bind_type    = item->bind_type;
    user->state        = item->state;
    user->haddr_len    = item->haddr_len;
    user->vlanId       = item->vlanId;
    user->ip_addr      = item->ip_addr;
    user->lease_time   = item->lease_time;
    user->sys_escape   = item->sys_escape;
    user->cur_expire   = item->cur_expire;
    user->ifindex      = item->ifindex;
    user->flags        = item->flags;    
    memcpy(user->chaddr,item->chaddr,item->haddr_len);
    return DHCP_OK;
}


/*
 ******************************************************************************
 *  Function   :   void dhcp_snoop_database_fill_bind(dhcp_snooping_user_info_t *user,
                        dhcp_snooping_database_item_t *item)
 *  Input      :   user
 *  Output     :   item
 *  Return     :   OK/FAIL
 *  Description:   fill the bind table according user information
 ******************************************************************************
 */
void dhcp_snoop_tbl_fill_bind(dhcp_snooping_user_item_t *user,
        dhcp_snooping_tbl_item_t *item)
{
    if((user == NULL) || (item == NULL))
    {
		printf("error pointer NULL\r\n");        
		return;
    }
    
    item->state         = user->state;
    item->ip_addr       = user->ip_addr;
    item->lease_time    = user->lease_time;
    item->sys_escape    = time(0);    
    //dhcp_snoop_update_logfile();
    return ;
}



/*
 ******************************************************************************
 *  函数格式:   UINT32 dhcp_snoop_database_identity_item(void)
 *  输入参数:   无
 *  输出参数:   无
 *  返 回 值:   无
 *  功能描述:   
 ******************************************************************************
 */
u_int32_t dhcp_snoop_tbl_identity_item(dhcp_snooping_tbl_item_t *item,
    dhcp_snooping_user_item_t *user)
{

    if((item == NULL) || (user  == NULL))
    {
		printf("error pointer value NULL\r\n");        
		return DHCP_FAIL;
    }
    if((item->vlanId != user->vlanId )||(item->haddr_len != user->haddr_len))
    {
        printf("vid is not the same\r\n");
		return DHCP_FAIL;
    }
    if((user->ifindex!= 0) && (item->ifindex!= user->ifindex))
    {
		printf("have different ifindex\r\n");        
		return DHCP_FAIL;
    }
    if(memcmp(item->chaddr,user->chaddr,item->haddr_len) != 0)
    {
		printf("have different chaddr value\r\n");        
		return DHCP_FAIL;
    }
    
    return DHCP_OK;
}


/*
 ******************************************************************************
 *  Function   :   void dhcp_snoop_database_fill_bind(dhcp_snooping_user_info_t *user,
                        dhcp_snooping_database_item_t *item)
 *  Input      :   user
 *  Output     :   item
 *  Return     :   OK/FAIL
 *  Description:   fill the bind table according user information
 ******************************************************************************
 */
u_int32_t dhcp_snoop_tbl_refresh_bind(dhcp_snooping_user_item_t *user,
        dhcp_snooping_tbl_item_t *item)
{
	
    if((item == NULL) || (user  == NULL))
    {
        return DHCP_FAIL;
    }
	
	if(dhcp_snoop_tbl_identity_item(item,user) == DHCP_FAIL)
	{
		return DHCP_FAIL;
	}
	
	/* 填充数据库表项参数。 */
	dhcp_snoop_tbl_fill_bind(user,item);
	 /*********wangjr ******20100311*** for register************/
	 char cmd_buf[100]={0};
    struct interface *ife;
	unsigned char wlan_id[10] ={0};
	unsigned char radio_id = 0;
	 #if 0
	 printf("\n\n 11111111111-----------------------item->chaddr = %d",item->chaddr[0]);
	 printf("\n\n 111111111111-----------------------item->chaddr = %d",item->chaddr[1]);
	 printf("\n\n 11111111111-----------------------item->chaddr = %d",item->chaddr[2]);
	 printf("\n\n 111111111111-----------------------item->chaddr = %d",item->chaddr[3]);
	 printf("\n\n 111111111111111-----------------------item->chaddr = %d",item->chaddr[4]);
	 printf("\n\n111111111111 -----------------------item->chaddr = %d",item->chaddr[5]);
	 printf("\n\n11111111111111 -----------------------item->ip_addr = %u",item->ip_addr);
	#endif

	 /*sundaolian add*/
	 set_all_interfaces_promisc(g_sockfd);
     updateinterfaceindex(g_sockfd);  
	 /*add end*/

	printf("\n------------------------------1---------------------------------\n");
	for (ife=int_list;ife;ife=ife->next)
	{
	  printf("int_list ifindex:   %d,int_list name:%s \n",ife->ifindex,ife->name );
	}
	 printf("item->ifindex :%d\n",item->ifindex );
	 printf("\n------------------------------2---------------------------------\n");

	for (ife = int_list; ife; ife = ife->next) 
	{
		if (ife->ifindex == item->ifindex)
		{
			if (!strncmp(ife->name,"ath",3))
			{
			   if(strlen(ife->name) == 7 )
				{
				    radio_id = ife->name[4];
					sprintf(wlan_id, "%c",ife->name[6]);
			    }
			   else if(strlen(ife->name) == 8 )
				{
					radio_id = ife->name[4];
					sprintf(wlan_id, "%c%c",ife->name[6],ife->name[7]);
			    }
			   else			   	 
			   {
			    radio_id = ife->name[4];
			    strncpy(wlan_id,ife->name + 6,strlen(ife->name)-6); /*ath.0-128*/
			   }
			}	
		}
	}

//	if(strlen(wlan_id))	
	if(strlen(wlan_id)==0)	
		printf("\n\nNoooooo! wlan is zeroooooooo-----bind\n");
	
	//	 sprintf(cmd_buf,"/usr/sbin/wtpd_alarm 127.0.0.1 1 10 %02x%02x%02x%02x%02x%02x %c %03d %04d %u",item->chaddr[0], item->chaddr[1], item->chaddr[2],item->chaddr[3],item->chaddr[4],item->chaddr[5],radio_id,atoi(wlan_id),item->vlanId,item->ip_addr);
		 sprintf(cmd_buf,"/usr/sbin/wtpd_alarm_all 127.0.0.1 1 10 %02x%02x%02x%02x%02x%02x %c %03d %04d %u",item->chaddr[0], item->chaddr[1], item->chaddr[2],item->chaddr[3],item->chaddr[4],item->chaddr[5],radio_id,atoi(wlan_id),item->vlanId,item->ip_addr);
		 system(cmd_buf);
	 printf("\n\n 11111111111111-----------------------cmd_buf=%s",cmd_buf); 
	dhcp_snoop_update_logfile();
	
	return DHCP_OK;
}
/*
 ******************************************************************************
 *  Function   :   void dhcp_snoop_scan_tbl(void) 
 *  Input      :    NULL
 *  Output     :   NULL
 *  Return     :   OK/FAIL
 *  Description:   scan the bind table, delete the expire table
 ******************************************************************************
 */
void dhcp_snoop_scan_tbl(void)
{
  
	int i;
	dhcp_snooping_tbl_item_t* preItem;
	dhcp_snooping_tbl_item_t* nextItem;
	dhcp_snooping_tbl_item_t* item;
	static int currentTime;
	int changeflag = 0;

	if ((time(0)-currentTime) > 0)
	{
		currentTime = time(0);
	}
	else
	{
		return;
	}

	for(i=0; i<DHCP_SNOOPING_HASH_TABLE_SIZE; i++)
	{
		preItem = item = g_DHCP_Snooping_Hash_Table[i];
		while( item != NULL )
		{
			/*scan all the bind table, age the expired items*/
			if ((item->sys_escape + item->lease_time)<= currentTime)
			{
				/*expire, and delete it*/
				if(item == g_DHCP_Snooping_Hash_Table[i]) 
				{
					g_DHCP_Snooping_Hash_Table[i] = item->next;
					nextItem = item->next;
					free(item);					
					item = nextItem;
					changeflag = 1;
				}else
				{
					nextItem = item->next;
					preItem->next = item->next;
					free(item);
					item = nextItem;
					changeflag = 1;
				}
				continue;
			}
			preItem = item;
			item = item->next;
		}
	}
	if (changeflag)
	{
		printf("enter the age process\r\n");
		dhcp_snoop_update_logfile();
	}
	return;
}




