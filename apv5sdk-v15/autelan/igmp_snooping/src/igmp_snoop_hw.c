/*********************************************************************************************
*			Copyright(c), 2008, Autelan Technology Co.,Ltd.
*						All Rights Reserved
*
**********************************************************************************************
$RCSfile:igmp_snoop_main.c
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

/********************************************global value****************************************/
l2mc_list_head		p_l2mc_list_head;
USHORT	igmp_snp_pfm_mode = 0;

/********************************************extern value***************************************/
extern INT kernel_fd;
extern igmp_vlan_list *p_vlanlist;
extern UINT igmp_snoop_debug;
/***********************************declare functions**************************************/
static LONG L2mc_Entry_Insert( igmp_snoop_pkt * pPkt );
static LONG L2mc_Entry_Update(  ULONG ulVlanId,char * pMac,ULONG ulVlanIdx );
static LONG L2mc_Entry_GetVidx(  ULONG ulVlanId,char * pMac );
static LONG L2mc_Entry_Delete( igmp_snoop_pkt * pPkt );
static LONG igmp_notify_msg(igmp_snoop_pkt * pPkt,ULONG ulArg );
static LONG send_msg(VOID *data,INT datalen,ULONG msg_type);
/***************************************************************************************/
/***********************************functions ********************************************/
/***************************************************************************************/

VOID igmp_debug_print_notify(struct igmp_skb *send)
{
	IGMP_SNP_DEBUG("##########################NOTIFY message#######################\r\n");
	if( IGMP_SNP_FLAG_ADDR_MOD == send->nlh.nlmsg_flags )
	{
		igmp_notify_mod_pkt *send_noti = (igmp_notify_mod_pkt *)send->buf;
		
		IGMP_SNP_DEBUG("\tMessage Type:%d[%s]\r\n",
			send->nlh.nlmsg_flags,"IGMP_SNP_FLAG_ADDR_MOD");
		IGMP_SNP_DEBUG("\t-----------------------------------------------------------------\r\n");
		switch( send_noti->mod_type)
		{
			case IGMP_ADDR_ADD:
				IGMP_SNP_DEBUG("\tMod_type:0x%x[%s]\r\n",
						send_noti->mod_type,"IGMP_ADDR_ADD");
				IGMP_SNP_DEBUG("\tReserve:0x%x[%s]\r\n",
						send_noti->reserve,
						(send_noti->mod_type == send_noti->reserve)?"IGMP_ADDR_ADD":"UNKNOWN");
				break;
			case IGMP_ADDR_DEL:
				IGMP_SNP_DEBUG("\tMod_type:0x%x[%s]\r\n",
						send_noti->mod_type,"IGMP_ADDR_DEL");
				IGMP_SNP_DEBUG("\tReserve:0x%x[%s]\r\n",
						send_noti->reserve,
						(send_noti->mod_type == send_noti->reserve)?"IGMP_ADDR_DEL":"UNKNOWN");
				break;
			case IGMP_ADDR_RMV:
				IGMP_SNP_DEBUG("\tMod_type:0x%x[%s]\r\n",
						send_noti->mod_type,"IGMP_ADDR_RMV");
				IGMP_SNP_DEBUG("\tReserve:0x%x[%s]\r\n",
						send_noti->reserve,
						(send_noti->mod_type == send_noti->reserve)?"IGMP_ADDR_RMV":"UNKNOWN");
				break;
			case IGMP_SYS_SET:
				IGMP_SNP_DEBUG("\tMod_type:0x%x[%s]\r\n",
						send_noti->mod_type,"IGMP_SYS_SET");
				IGMP_SNP_DEBUG("\tReserve:0x%x[%s]\r\n",
						send_noti->reserve,
						(IGMP_SYS_SET_STOP == send_noti->reserve)?"IGMP_SYS_SET_STOP":"IGMP_SYS_SET_INIT");
				break;
			case IGMP_TRUNK_UPDATE:
				IGMP_SNP_DEBUG("\tMod_type:0x%x[%s]\r\n",
						send_noti->mod_type,"IGMP_TRUNK_UPDATE");
				IGMP_SNP_DEBUG("\tReserve:0x%x[%s]\r\n",
						send_noti->reserve,
						(send_noti->mod_type == send_noti->reserve)?"IGMP_TRUNK_UPDATE":"UNKNOWN");
				break;
			default:
				IGMP_SNP_DEBUG("\tMod_type:0x%x[%s]\r\n",
						send_noti->mod_type,"UNKNOWN");
				IGMP_SNP_DEBUG("\tReserve:0x%x[%s]\r\n",
						send_noti->reserve,"UNKNOWN");
				break;
		}
		IGMP_SNP_DEBUG("\tVlan_id:0x%04x\t\tIfindex:0x%04x\r\n",
				send_noti->vlan_id,send_noti->ifindex);
		IGMP_SNP_DEBUG("\tGroup:%u.%u.%u.%u\r\n",NIPQUAD(send_noti->groupadd));
	}
	else
	{
		IGMP_SNP_DEBUG("\tMessage Type:%d[%s]\r\n",
			send->nlh.nlmsg_flags,"Unknown");
	}
	IGMP_SNP_DEBUG("#############################################################\r\n\r\n");
}


INT igmp_vlanportrelation(ULONG ifindex,LONG vlan_id,ULONG *targged)
{
	*targged = 0;		/*untargged*/
	return IGMP_SNOOP_OK;
}
INT igmp_is_vlan_ifindex(LONG vlan_id, ULONG ifindex)
{
	igmp_vlan_list *t_vlan = NULL;
	igmp_vlan_port_list *t_port = NULL;
	
	if( !p_vlanlist )
	{
		IGMP_SNP_DEBUG("igmp_is_vlan_ifindex:can not find any vlan.\r\n");
		return IGMP_SNOOP_ERR;
	}
	DEBUG_OUT("igmp_is_vlan_ifindex:vlan_id:0x%x\tifindex:0x%x\r\n",
				vlan_id,ifindex);
	t_vlan = p_vlanlist;
	while(NULL != t_vlan)
	{
		if( t_vlan->vlan_id == vlan_id)
		{
			if( NULL != t_vlan->first_port )
			{
				t_port = t_vlan->first_port;
				while(t_port)
				{
					if( t_port->ifindex == ifindex )
						return 1;		/*find*/
				}
				t_port = t_port->next;
			}
		}
		t_vlan = t_vlan->next;
	}
	return 0;
}

int igmp_getifindex_byvlanid(LONG vlan_id,ULONG *first_ifindex)
{
	igmp_vlan_list *t_vlan = NULL;
	
	if(!p_vlanlist)
	{
		IGMP_SNP_DEBUG("igmp_getifindex_byvlanid:can not find any vlan.\r\n");
		return IGMP_SNOOP_ERR;
	}

	t_vlan = p_vlanlist;
	while(NULL != t_vlan)
	{
		if( t_vlan->vlan_id == vlan_id)
		{
			if( NULL != t_vlan->first_port )
			{
				*first_ifindex = t_vlan->first_port->ifindex;
				return IGMP_SNOOP_OK;
			}
			else
			{
				*first_ifindex  = 0;
				IGMP_SNP_DEBUG("igmp_getifindex_byvlanid:this vlan is empty.\r\n");
				return IGMP_SNOOP_ERR;
			}
		}
	}
	*first_ifindex = 0;
	IGMP_SNP_DEBUG("igmp_getifindex_byvlanid:can not find vlan.\r\n");
	return IGMP_SNOOP_ERR;
}


ULONG igmp_get_nextifindex_byifindex(LONG vlan_id, ULONG ifindex)
{
	ULONG next_if = 0;
	igmp_vlan_list *t_vlan = NULL;
	
	if(!p_vlanlist)
	{
		IGMP_SNP_DEBUG("igmp_get_nextifindex_byifindex:can not find any vlan.\r\n");
		return IGMP_SNOOP_ERR;
	}
	
	t_vlan = p_vlanlist;
	while(NULL != t_vlan)
	{
		if( t_vlan->vlan_id == vlan_id)
		{
			if( NULL != t_vlan->first_port )
			{
				igmp_vlan_port_list *t_port = t_vlan->first_port;
				while(NULL != t_port)
				{
					if( t_port->ifindex == ifindex)
					{
						if( NULL != t_port->next )
							return (t_port->next->ifindex);
						else
							return IGMP_SNOOP_OK;
					}
					t_port = t_port->next;
				}
				IGMP_SNP_DEBUG("igmp_get_nextifindex_byifindex:can not find ifindex.\r\n");
				return IGMP_SNOOP_OK;
			}
			else
			{
				IGMP_SNP_DEBUG("igmp_get_nextifindex_byifindex:this vlan is empty.\r\n");
				return IGMP_SNOOP_OK;
			}
		}
	}
	IGMP_SNP_DEBUG("igmp_get_nextifindex_byifindex:can not find vlan.\r\n");
	return IGMP_SNOOP_OK;

}


void igmp_getifstatus(ULONG ifindex,ULONG *portstate)
{
	DEBUG_OUT("igmp_getifstatus:excuted successful.\r\n");
	*portstate = 1;
}

void igmp_getmaxgroupcnt(ULONG ifindex,ULONG *maxgroupcnt)
{
	DEBUG_OUT("igmp_getmaxgroupcnt:excuted successful.\r\n");
	*maxgroupcnt = IGMP_SNP_GRP_MAX;
}


INT igmp_getvlan_addr(ULONG vlan_id, ULONG *vlan_addr)
{
	DEBUG_OUT("igmp_getvlan_addr:excuted successful.\r\n");
	return IGMP_SNOOP_ERR;
}

int IFM_PhyIsMerged2TrunkApi(LONG vlan_id,ULONG ifindex,ULONG *trunkifindex)
{	/*check port is member of trunck*/
	igmp_vlan_list *t_vlan = NULL;
	igmp_vlan_port_list *t_port = NULL;
	
	DEBUG_OUT("Entry IFM_PhyIsMerged2TrunkApi\r\n");
	if( !p_vlanlist )
	{
		IGMP_SNP_DEBUG("IFM_PhyIsMerged2TrunkApi:can not find any vlan.\r\n");
		return IGMP_SNOOP_ERR;
	}
	while(NULL != t_vlan)
	{
		if( t_vlan->vlan_id == vlan_id)
		{
			if( NULL != t_vlan->first_port )
			{
				t_port = t_vlan->first_port;
				while(t_port)
				{
					if( t_port->ifindex == ifindex )
						if(t_port->trunkflag != 0 )
						{
							*trunkifindex = t_port->trunkflag;
							return IGMP_SNOOP_OK;		/*trunk*/
						}
						else
							return IGMP_SNOOP_ERR;
				}
				t_port = t_port->next;
			}
		}
		t_vlan = t_vlan->next;
	}
	return IGMP_SNOOP_ERR;
}

int IFM_isTrunkIf(LONG vlan_id, ULONG ifindex)
{
	igmp_vlan_list *t_vlan = NULL;
	igmp_vlan_port_list *t_port = NULL;
	
	if( !p_vlanlist )
	{
		IGMP_SNP_DEBUG("igmp_is_vlan_ifindex:can not find any vlan.\r\n");
		return IGMP_SNOOP_ERR;
	}
	DEBUG_OUT("igmp_is_vlan_ifindex:vlan_id:0x%x\tifindex:0x%x\r\n",
				vlan_id,ifindex);
	t_vlan = p_vlanlist;
	while(NULL != t_vlan)
	{
		if( t_vlan->vlan_id == vlan_id)
		{
			if( NULL != t_vlan->first_port )
			{
				t_port = t_vlan->first_port;
				while(t_port)
				{
					if( t_port->ifindex == ifindex )
						if(t_port->trunkflag != 0 )
							return 1;		/*trunk*/
						else
							return 0;
				}
				t_port = t_port->next;
			}
		}
		t_vlan = t_vlan->next;
	}
	return 0;
}


LONG igmp_vlanisused( LONG vlan_id, ULONG * ret )
{
	ULONG vlanifidx;
	igmp_vlan_list *t_vlan = NULL;

	if( !p_vlanlist )
	{
		IGMP_SNP_DEBUG("igmp_vlanisused:can not find any vlan.\r\n");
		return IGMP_SNOOP_ERR;
	}
	DEBUG_OUT("igmp_vlanisused:vlan_id:0x%x\r\n",
				vlan_id);
	t_vlan = p_vlanlist;
	while(NULL != t_vlan)
	{
		if( t_vlan->vlan_id == vlan_id)
		{
			*ret = 1;
			return IGMP_SNOOP_OK;
		}
		t_vlan = t_vlan->next;
	}
	*ret = 0;
	return IGMP_SNOOP_OK;
}


LONG igmp_check_ip_from_mac(ULONG group, char *groupmac)
{
	if (( groupmac[0] != 0x1) || (groupmac[1] != 0x0) ||(groupmac[2] != 0x5e))
		return IGMP_SNOOP_ERR;
	if (groupmac[3] != ((group >> 16) & 0x7f) )
		return IGMP_SNOOP_ERR;
	if (groupmac[4] != ((group >> 8) & 0xff) )
		return IGMP_SNOOP_ERR;
	if (groupmac[5] != ((group >> 0) & 0xff) )
		return IGMP_SNOOP_ERR;
	return IGMP_SNOOP_OK;
}

LONG L2mc_Entry_Insert( igmp_snoop_pkt * pPkt )
{
	l2mc_list * pstL2mcEntry = NULL;
	l2mc_list * pstTemp = NULL;
	LONG lCount = 0;
	ULONG EntryExist = 0;
	
	if ( NULL == pPkt )
	{
		return IGMP_SNOOP_ERR;
	}
	if( p_l2mc_list_head.stListHead )
	{
		pstTemp = p_l2mc_list_head.stListHead;
		while(pstTemp)
		{
			if ( lCount >= IGMP_SNP_GRP_MAX + 1 )
			{
				return IGMP_SNOOP_ERR;
			}
			lCount++;
			if ( ( pstTemp ->ulGroup == pPkt->groupadd )
			 			&& ( pstTemp->ulVlanId == pPkt->vlan_id) )
			{
				EntryExist = 1;
				break;
			}
			pstTemp = pstTemp->next;
			if( pstTemp == p_l2mc_list_head.stListHead )	/*can not find*/
				break;
		}
		if ( EntryExist != 1 )
		{
			if ( p_l2mc_list_head.ulListNodeCount >= IGMP_SNP_GRP_MAX + 1 )
			{
				return IGMP_SNOOP_ERR;
			}
			pstL2mcEntry = malloc( sizeof( l2mc_list ));
			if ( pstL2mcEntry == NULL )
			{
				return IGMP_SNOOP_ERR;
			}
			pstL2mcEntry ->ulIndex = pPkt->ifindex;
			pstL2mcEntry ->ulGroup = pPkt->groupadd;
			pstL2mcEntry ->ulVlanId = pPkt->vlan_id;
			pstL2mcEntry->ulVlanIdx = pPkt->group_id;
			pstL2mcEntry->next = p_l2mc_list_head.stListHead;
			pstL2mcEntry->prev = p_l2mc_list_head.stListHead->prev;
			(p_l2mc_list_head.stListHead->prev)->next = pstL2mcEntry;
			p_l2mc_list_head.stListHead->prev = pstL2mcEntry;
			p_l2mc_list_head.ulListNodeCount++;
			IGMP_SNP_DEBUG(" L2mc_Entry_Insert: insert new entry.\r\n");
		}
	}
	else
	{
		pstL2mcEntry = malloc( sizeof( l2mc_list ));
		if ( pstL2mcEntry == NULL )
		{
			return IGMP_SNOOP_ERR;
		}
		pstL2mcEntry->next = pstL2mcEntry;
		pstL2mcEntry->prev = pstL2mcEntry;
		pstL2mcEntry ->ulIndex = pPkt->ifindex;
		pstL2mcEntry ->ulGroup = pPkt->groupadd;
		pstL2mcEntry ->ulVlanId = pPkt->vlan_id;
		pstL2mcEntry->ulVlanIdx = pPkt->group_id;
		p_l2mc_list_head.stListHead = pstL2mcEntry;
		p_l2mc_list_head.ulListNodeCount = 1;
	}
	return IGMP_SNOOP_OK;
}

LONG L2mc_Entry_Update(  ULONG ulVlanId,char * pMac,ULONG ulVlanIdx )
{
	l2mc_list * pstTemp = NULL;
	LONG lCount = 0;     

	pstTemp = p_l2mc_list_head.stListHead;
	while(pstTemp)
	{
		if ( lCount >= IGMP_SNP_GRP_MAX + 1 )
		{
			return IGMP_SNOOP_ERR;
		}
		lCount++;
		
		if ((  pstTemp->ulVlanId== ulVlanId) 
			&&( IGMP_SNOOP_OK == igmp_check_ip_from_mac (pstTemp->ulGroup,pMac)))	
		{
			pstTemp->ulVlanIdx = ulVlanIdx;
			break;
		}
		pstTemp = pstTemp->next;
		if( pstTemp == p_l2mc_list_head.stListHead )	/*can not find*/
			break;
	}
	return IGMP_SNOOP_OK;
}

LONG L2mc_Entry_GetVidx(  ULONG ulVlanId,char * pMac )
{
	l2mc_list * pstTemp = NULL;
	LONG lCount = 0;  
	ULONG ulVlanIdx=0;

	pstTemp = p_l2mc_list_head.stListHead;
	while(pstTemp)
	{
		if( lCount >= IGMP_SNP_GRP_MAX + 1 )
		{
			return 0;
		}
		lCount++;
		
		if((  pstTemp->ulVlanId== ulVlanId)
			&&( IGMP_SNOOP_OK == igmp_check_ip_from_mac (pstTemp->ulGroup,pMac)))
		{
			ulVlanIdx= pstTemp->ulVlanIdx;
			break;
		}
		pstTemp = pstTemp->next;
		if( pstTemp == p_l2mc_list_head.stListHead )	/*can not find*/
			break;
	}
	return ulVlanIdx;
}

LONG L2mc_Entry_Delete( igmp_snoop_pkt * pPkt )
{

	l2mc_list * pstL2mcEntry = NULL;
	l2mc_list * pstTemp = NULL;
	LONG lCount = 0;
	ULONG EntryExist = 0;

	if ( NULL == pPkt )
	{
		return IGMP_SNOOP_ERR;
	}

	pstTemp = p_l2mc_list_head.stListHead;
	while(pstTemp)
	{

		if ( lCount >= IGMP_SNP_GRP_MAX + 1 )
		{
			return IGMP_SNOOP_ERR;
		}
		lCount++;

		if ( ( pstTemp ->ulGroup == pPkt->groupadd)
				&& ( pstTemp->ulVlanId == pPkt->vlan_id) )
		{
			EntryExist = 1;
			pstL2mcEntry = pstTemp;
			break;
		}
		pstTemp = pstTemp->next;
		if( pstTemp == p_l2mc_list_head.stListHead )	/*can not find*/
			break;
	}
	if ( EntryExist == 1 )
	{

		if ( pstL2mcEntry == NULL )
		{
			return IGMP_SNOOP_ERR;
		}

		(pstL2mcEntry->prev)->next = pstL2mcEntry->next;
		(pstL2mcEntry->next)->prev = pstL2mcEntry->prev;
		free( pstL2mcEntry );
		pstL2mcEntry = NULL;
		p_l2mc_list_head.ulListNodeCount--;
		if(0== p_l2mc_list_head.ulListNodeCount)
			p_l2mc_list_head.stListHead = NULL;
		IGMP_SNP_DEBUG(" L2mc_Entry_Delete: remove entry.\r\n");
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_snp_mod_addr()
*
* DESCRIPTION:
*		Modify L2/L3 MC address  hardware table and set port filter mode.
**************************************************************************/
LONG igmp_snp_mod_addr( igmp_snoop_pkt * pPkt, ULONG ulArg )
{
	LONG lRet = IGMP_SNOOP_OK;

	if ( NULL == pPkt )
	{
		IGMP_SNP_DEBUG("igmp_snp_mod_addr: null pPkt \r\n");
		return IGMP_SNOOP_ERR;
	}
	IGMP_SNP_DEBUG("\r\nigmp_snp_mod_addr: vid %d group 0x%.8x If 0x%x arg %d\r\n",
	      						pPkt->vlan_id, pPkt->groupadd, pPkt->ifindex, ulArg);
	switch ( ulArg )
	{
		case IGMP_ADDR_ADD:
			lRet = L2mc_Entry_Insert( ( VOID * ) pPkt );
			lRet = igmp_notify_msg(pPkt,ulArg);
			break;
		case IGMP_ADDR_DEL:
			lRet = igmp_notify_msg(pPkt,ulArg);
			break;
		case IGMP_ADDR_RMV:
			lRet = L2mc_Entry_Delete( ( VOID * ) pPkt );
			lRet = igmp_notify_msg(pPkt,ulArg);
			break;
		case IGMP_SYS_SET:
			lRet = igmp_notify_msg(pPkt,ulArg);
			break;
		default:
			lRet = IGMP_SNOOP_ERR;
	}
	
	return lRet;
}


LONG igmp_notify_msg(igmp_snoop_pkt * pPkt,ULONG ulArg )
{
	LONG ret = IGMP_SNOOP_OK;
	igmp_notify_mod_pkt *send_noti = NULL;

	if( NULL == pPkt )
	{
		IGMP_SNP_DEBUG("igmp_notify_add: null pPkt \r\n");
		return IGMP_SNOOP_ERR;
	}
	send_noti = (igmp_notify_mod_pkt *)malloc(sizeof(igmp_notify_mod_pkt));
	if( NULL == send_noti )
	{
		IGMP_SNP_DEBUG("igmp_notify_add: malloc memory failed. \r\n");
		return IGMP_SNOOP_ERR;
	}
	memset(send_noti,0,sizeof(igmp_notify_mod_pkt));
	//send_noti->mod_type = pPkt->type;
	send_noti->mod_type = ulArg;
	send_noti->reserve = pPkt->type;
	send_noti->ifindex = pPkt->ifindex;
	send_noti->vlan_id = pPkt->vlan_id;
	send_noti->groupadd = pPkt->groupadd;

	ret = send_msg((void *)send_noti,sizeof(igmp_notify_mod_pkt),IGMP_SNP_FLAG_ADDR_MOD );
	if( IGMP_SNOOP_ERR == ret )
	{
		IGMP_SNP_DEBUG("igmp_notify_add: send notify message failed. \r\n");
		free(send_noti);
		return IGMP_SNOOP_ERR;
	}
	DEBUG_OUT("Igmp_notify_add:send message successful.\r\n");
	free(send_noti);
	return ret;
}


LONG send_msg(VOID *data,INT datalen,ULONG msg_type)
{
	struct igmp_skb *send = NULL;

	if( NULL == data )
	{
		IGMP_SNP_DEBUG("send_msg: null data. \r\n");
		return IGMP_SNOOP_ERR;
	}

	send = (struct igmp_skb *)malloc(sizeof(struct igmp_skb));
	if(NULL == send)
	{
		IGMP_SNP_DEBUG("send_msg: malloc memory failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	memset(send,0,sizeof(struct igmp_skb));
	send->nlh.nlmsg_type = IGMP_SNP_TYPE_NOTIFY_MSG;
	send->nlh.nlmsg_flags = msg_type;
	send->nlh.nlmsg_len = sizeof(struct nlmsghdr)+datalen;
	memcpy(send->buf,data,datalen);
	igmp_debug_print_notify(send);
	if( 0 != kernel_fd )
		write(kernel_fd,(char *)(send),sizeof(struct nlmsghdr)+datalen);
	free(send);
	DEBUG_OUT("Send MSG:msg_type:%s\r\n",
				(msg_type == IGMP_SNP_FLAG_ADDR_MOD)?"addr_mod":"UNKNOWN");
	return IGMP_SNOOP_OK;
}

/*device event handle*/
LONG igmp_snp_device_event( ULONG event, dev_notify_msg * ptr )
{

	ULONG ulIfIndex;

	if ( NULL == ptr )
	{
		return IGMP_SNOOP_ERR;
	}
	DEBUG_OUT("igmp_snp_device_event:event:%d\r\n");
	switch ( event )
	{
		case EVENT_DEV_UP:
			break;
		case EVENT_DEV_DOWN:
			{
				igmp_snoop_pkt * pPkt = NULL;  

				if ( igmp_is_vlan_ifindex( ptr->vlan_id, ptr->ifindex ))
					return IGMP_SNOOP_ERR;

				ulIfIndex =ptr->ifindex;	       
				pPkt = ( igmp_snoop_pkt * )malloc( sizeof( igmp_snoop_pkt ));
				if( NULL == pPkt )
				{
					return IGMP_SNOOP_ERR;
				}
				memset( pPkt, 0, sizeof( igmp_snoop_pkt ) );         
				pPkt->ifindex =  ulIfIndex;
				pPkt->type = IGMP_PORT_DOWN;
				IGMP_SNP_DEBUG("igmp_snp_device_event: PORT_DOWN message enter.\r\n");
				/*调用处理函数*/
				Igmp_Event_PortDown(pPkt);
				break;
			}
		case EVENT_DEV_UNREGISTER:
			if (igmp_is_vlan_ifindex( ptr->vlan_id, ptr->ifindex ))
			{
				igmp_snoop_pkt * pPkt = NULL;

				pPkt = ( igmp_snoop_pkt * ) malloc( sizeof( igmp_snoop_pkt ));
				if ( NULL == pPkt )
				{
					IGMP_SNP_DEBUG("igmp_snp_device_event: failed in alloc mem for pPkt.\r\n");
					return IGMP_SNOOP_ERR;
				}
				memset( pPkt,0, sizeof( igmp_snoop_pkt ) );

				pPkt->vlan_id= ptr->vlan_id;
				pPkt->ifindex =  ptr->ifindex;
				IGMP_SNP_DEBUG ("igmp_snp_device_event: DEL_VLAN: ulIfIndex %x,lVid  %x\r\n", ptr->ifindex, pPkt->vlan_id);
				pPkt->type = IGMP_VLAN_DEL_VLAN;
				/*调用处理函数*/
				Igmp_Event_DelVlan(pPkt);
			}
			break;
		case EVENT_DEV_VLANADDIF:
			break;
		case EVENT_DEV_VLANDELIF:
			if ( igmp_is_vlan_ifindex( ptr->vlan_id,ptr->ifindex ))
			{
				ulIfIndex = ptr->ifindex;
				{
					igmp_snoop_pkt * pPkt = NULL;

					pPkt = ( igmp_snoop_pkt * )malloc( sizeof( igmp_snoop_pkt ));
					if ( NULL == pPkt )
					{
						return IGMP_SNOOP_ERR;
					}
					memset( pPkt, 0,sizeof( igmp_snoop_pkt ) );
					pPkt->vlan_id= ptr->vlan_id;
					pPkt->ifindex =  ulIfIndex;
					pPkt->type = IGMP_VLAN_DEL_PORT;
					IGMP_SNP_DEBUG("igmp_snp_device_event:  DEL_PORT:ulIfIndex %x,lVid  %x\r\n", pPkt->ifindex, pPkt->vlan_id);
					Igmp_Event_VlanDelPort(pPkt);
				}
			}
			break;
		default:
			break;
		}
	return IGMP_SNOOP_OK;

}


/*************************************************************************
 * Function:
 *  Igmp_Snoop_IF_Recv_Is_Router_Discard
 * Description:
 *  
 * Parameters:
 *
 * Returns:
 *  Nothing.
 ***************************************************************************/
LONG Igmp_Snoop_IF_Recv_Is_Report_Discard( LONG lVid , ULONG ulIfIndex )
{
	igmp_reporter_port * pReporter = NULL;
	member_port_list *pMemberPort = NULL;
	
	if ( IGMP_SNOOP_OK != igmp_snp_searchreporterlist( lVid, 0, IGMP_PORT_QUERY, &pReporter ) )
	{
		IGMP_SNP_DEBUG( ( "Igmp_Snoop_IF_Recv_Is_Report_Discard: search reporter list failed\r\n" ) );
		return IGMP_SNOOP_ERR;
	}

	if ( NULL != pReporter ) 	/*There is some member port in vlan*/
	{
		pMemberPort = pReporter->portlist;
		while ( NULL != pMemberPort )
		{
			if ( ulIfIndex == pMemberPort->ifindex )
			{
				return IGMP_SNOOP_ERR; /*this is a mem port,so discard the pkt*/
			}
			pMemberPort = pMemberPort->next;
		}
	}
	return IGMP_SNOOP_OK;
}



