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

#define IGMP_SNP_DEL_INTERTIMER( id ) { id = 0; }
/****************************thread value****************************************************/
pthread_t thread_timer;
pthread_t thread_msg;
pthread_t thread_recvskb;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	/*线程锁*/

/************************************global value*****************************************/
/************************************config value*****************************************/
LONG igmp_snoop_enable = IGMP_SNOOP_NO;	/*IGMP snoop enable or not*/
UINT igmp_snoop_debug = IGMP_SNOOP_NO;
struct timer_list igmp_timer_list;

INT		kernel_fd = 0;
ULONG igmp_genquerytime[IGMP_GENERAL_GUERY_MAX] = {0};
ULONG igmp_router_timeout = 260 ;

ULONG igmp_groupcount = 0;
ULONG igmp_global_timer = 0;
USHORT igmp_robust_variable = IGMP_ROBUST_VARIABLE;
ULONG igmp_query_interval = IGMP_V2_QUERY_INTERVAL;
ULONG igmp_rxmt_interval = IGMP_V2_LEAVE_LATENCY *1000;
ULONG igmp_resp_interval = IGMP_V2_QUERY_RESP_INTERVAL;
ULONG igmp_grouplife = IGMP_GROUP_LIFETIME;
ULONG igmp_vlanlife = IGMP_VLAN_LIFE_FACTOR;
INT 				first_vlan_idx = -1;	/*初始值为-1*/
MC_group_vlan 	*mcgroup_vlan_queue[IGMP_GENERAL_GUERY_MAX];

igmp_vlan_list		*p_vlanlist = NULL;	/*system vlan information*/

igmp_queryport	*p_queryportlist = NULL;
igmp_routerport	*p_routerlist = NULL;
igmp_reporter_port	*p_reporterlist = NULL;
/************************************extern value****************************************/
extern l2mc_list_head		p_l2mc_list_head;

/***************************************declare functions**********************************/
static INT init_igmp_snp_timer(void);
static VOID igmp_snp_global_timer_func( struct timer_element *cur );
static LONG Igmp_Event_GenQuery_Timeout( MC_group_vlan *p_vlan );
static LONG Igmp_Event_GroupLife_Timeout( MC_group_vlan *p_vlan, MC_group * p_group );
static LONG Igmp_Event_Proxy_Timeout(MC_group_vlan *p_vlan, MC_group * p_group);
static LONG Igmp_Event_GroupMember_Timeout( MC_group_vlan *p_vlan,MC_group *p_group,
							ULONG ifindex );
static VOID Igmp_Event_Resp_Timeout( struct timer_element *cur);
static VOID Igmp_Event_Rxmt_Timeout( struct timer_element *cur );
static LONG igmp_snp_addintertimer( ULONG ulSec, ULONG * pulTimerId );
static LONG Igmp_Snoop_Send_Igmp( igmp_snoop_pkt * pPkt );
static VOID igmp_snp_file_option_field( UCHAR * pOptionBuf, ULONG ulLength );
static INT inet_cksum(USHORT *addr,ULONG len);
static void Igmp_Snoop_Send_Packet(struct igmp_skb *msg_skb,UINT datalen,LONG vlan_id,ULONG ifindex);
static LONG igmp_recv_report( ULONG usIfIndex, ULONG ulGroup, LONG lVid, 
								ULONG ulType, struct igmp_info *sk_info);
static INT igmp_recv_query(ULONG ifindex, USHORT maxresptime, ULONG group,
					LONG vlan_id, struct igmp_info *sk_info);	
static LONG igmp_recv_leave( ULONG usIfIndex, ULONG ulGroup, LONG lVid,struct igmp_info *sk_info );
static LONG igmp_recv_unknown(struct igmp_info *sk_info );
static LONG Igmp_Snoop_RouterReport( LONG lVid, ULONG usIfIndex, ULONG ulGroup, struct igmp_info *sk_info );
static LONG igmp_snp_routerleave( LONG lVid, ULONG usIfIndex, ULONG ulGroup, ULONG ulSaddr );
static LONG igmp_snp_flood( struct igmp_info *sk_info, LONG lVid, ULONG usIfIndex );
/**************************************************************************************/
/*****************************************functions**************************************/
/**************************************************************************************/

static void Igmp_print_packet( char * pBuffer, long lLength )
{
	unsigned long i;
	char *p;
	unsigned long length;
	
	if ( !pBuffer )
		return ;
	
	p = pBuffer;
	length = lLength;
	IGMP_SNP_DEBUG("----------DATA----------------------------------------------------\r\n");

	for ( i = 0; i < length; i++ )
	{
		if (( i % 16 == 0)&&(i != 0 ) )
		{
			IGMP_SNP_DEBUG("\r\n");
		}
		IGMP_SNP_DEBUG(" %02x ", (unsigned char)(*( p + i )));
	}
	IGMP_SNP_DEBUG("\r\n");
}

void igmp_debug_print_skb(struct igmp_skb *msg_skb)
{
	struct iphdr *ip_addr = (struct iphdr *)(msg_skb->buf + 8 + 14);
	struct igmp *igmphd = (struct igmp *)((unsigned char *)ip_addr+ip_addr->ihl*4);
	struct pim_header *pimhd = (struct pim_header *)((unsigned char *)ip_addr + ip_addr->ihl*4);
	
	IGMP_SNP_DEBUG("********************************PACKET information*************************\r\n");
	IGMP_SNP_DEBUG("\tVlan_id:0x%04x\t\tIfindex:0x%04x\r\n",
		*(unsigned int *)(msg_skb->buf+4),*(unsigned int *)(msg_skb->buf));
	IGMP_SNP_DEBUG("\t-------------IP header----------------\r\n");
	IGMP_SNP_DEBUG("\tVer:%d\tIhl:%d\tTos:%d\tTot_len:%d\r\n",
				ip_addr->version,ip_addr->ihl,ip_addr->tos,ip_addr->tot_len);
	IGMP_SNP_DEBUG("\tFrag_off:%d\tTtl:%d\tProtocol:%d\r\n",
				ip_addr->frag_off,ip_addr->ttl,ip_addr->protocol);
	IGMP_SNP_DEBUG("\tSaddr:%u.%u.%u.%u",NIPQUAD(ip_addr->saddr));
	IGMP_SNP_DEBUG("\tDaddr:%u.%u.%u.%u\r\n",NIPQUAD(ip_addr->daddr))

	if( IPPROTO_PIM == ip_addr->protocol )
	{
		IGMP_SNP_DEBUG("\t-------------PIM header--------------\r\n");
		IGMP_SNP_DEBUG("\tPIM_vers:%d\t\tPIM_type:%d\r\n",
					pimhd->pim_vers,pimhd->pim_type);
		IGMP_SNP_DEBUG("\tPIM_Cksum:%d\tPIM_reserved:%d\r\n",
			pimhd->pim_cksum,pimhd->pim_reserved);
	}
	else
	{
		IGMP_SNP_DEBUG("\t-------------IGMP header--------------\r\n");
		IGMP_SNP_DEBUG("\tType:%d\t\tCode:%d\r\n",igmphd->igmp_type,igmphd->igmp_code);
		IGMP_SNP_DEBUG("\tCksum:%d\tGroup:%u.%u.%u.%u\r\n",
			igmphd->igmp_cksum,NIPQUAD(igmphd->igmp_group.s_addr));
	}
	Igmp_print_packet(msg_skb->buf, (8 + (8 + sizeof(struct iphdr) + sizeof(struct igmp) + 4)));
	IGMP_SNP_DEBUG("***************************************************************************\r\n\r\n");
}

void igmp_debug_print_groupvlan(MC_group_vlan *pvlan)
{
	struct MC_port_state *t_port = NULL;
	if( !igmp_snoop_debug )
		return;
	if(NULL == pvlan)
	{
		IGMP_SNP_DEBUG("igmp_debug_print_groupvlan:vlan is NULL.\r\n");
	}
	IGMP_SNP_DEBUG("@@@@@@@@@@@@@@@@@@@@@@Vlan data@@@@@@@@@@@@@@@@@@@@\r\n");
	IGMP_SNP_DEBUG("\tnext:%d\tprev:%d\tvlan_id:%d\tlast_requery_addr:%u.%u.%u.%u\r\n"
		"\tquerytimeinterval:%d\tquerytimer_id:%d\tvlanlife:%d\r\n",
		pvlan->next,pvlan->prev,
		pvlan->vlan_id,NIPQUAD(pvlan->saddr),
		pvlan->querytimeinterval,pvlan->querytimer_id,
		pvlan->vlanlife);
	if( NULL != pvlan->firstgroup )
	{
		IGMP_SNP_DEBUG("----------------------------Firstgroup data-----------------------------\r\n");
		IGMP_SNP_DEBUG("\tMC_ipadd:%u.%u.%u.%u\treport_ipadd:%u.%u.%u.%u\tvlan_id:%d\tversion:%s\r\n",
			NIPQUAD(pvlan->firstgroup->MC_ipadd),
			NIPQUAD(pvlan->firstgroup->report_ipadd),
			pvlan->firstgroup->vid,
			(pvlan->firstgroup->ver_flag?"IGMP V1":"IGMP V2"));
		t_port = pvlan->firstgroup->portstatelist;
		IGMP_SNP_DEBUG("----------------------------MC_ports----------------------------------\r\n");
		while( NULL != t_port )
		{
			IGMP_SNP_DEBUG("\t\tifindex:0x%x\tstate:0x%x\r\n",t_port->ifindex,t_port->state);
			t_port = t_port->next;
		}
	}
	IGMP_SNP_DEBUG("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n\r\n");
}


/**************************************************************************
 * igmp_snp_addr_check()
 *INPUTS:
 *Multicast group ip address
 *OUTPUTS:
 *available IGMP_SNOOP_OK	invailable IGMP_SNOOP_ERR
 * DESCRIPTION:
 *		1. 224.0.0.X resever.
 *		2. available address 224.0.0.0 ~ 239.255.255.255
 **************************************************************************/
INT igmp_snp_addr_check( ULONG Group )
{
	/* range 224.0.0.0 ~ 239.255.255.255 */
	if ( ( Group < 0xE0000000 ) || ( Group > 0xEFFFFFFF ) )
	{
		IGMP_SNP_DEBUG( "\r\nigmp_snp_addr_check: Invalid group address 0x%.8x.\r\n", Group );
		return IGMP_SNOOP_ERR;
	}

	/* range 224.0.0.0 ~ 224.0.0.255 */
	if ( ( Group >= 0xE0000000 ) && ( Group <= 0xE00000FF ) )
	{
		IGMP_SNP_DEBUG(  "igmp_snp_addr_check: reserved group address 0x%.8x.\r\n", Group );
		return IGMP_SNOOP_ERR;
	}

	return IGMP_SNOOP_OK;
}


INT inet_cksum(USHORT *addr,ULONG len)
{	/*len = num Bytes*/
	register INT nleft = ( int ) len;
	register USHORT *w = addr;
	USHORT answer = 0;
	register INT sum = 0;

	/*
	*  Our algorithm is simple, using a 32 bit accumulator (sum),
	*  we add sequential 16 bit words to it, and at the end, fold
	*  back all the carry bits from the top 16 bits into the lower
	*  16 bits.
	*/
	while ( nleft > 1 )
	{
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if ( nleft == 1 )
	{
		*( UCHAR * ) ( &answer ) = *( UCHAR * ) w ;
		sum += answer;
	}

	/*
	* add back carry outs from top 16 bits to low 16 bits
	*/
	sum = ( sum >> 16 ) + ( sum & 0xffff );	/* add hi 16 to low 16 */
	sum += ( sum >> 16 );			/* add carry */
	answer = ~( USHORT )sum;				/* truncate to 16 bits */
	return ( answer );
}


/**************************************************************************
* igmp_snp_groupcheck()
*
* DESCRIPTION:
*		igmp_snp_groupcheck() check Vlan-group chain. If vlan was destroyed
*		delete the groups and the chain.
**************************************************************************/
LONG igmp_snp_groupcheck( LONG lVid )
{
	ULONG ulRet = 0;

	igmp_vlanisused( lVid, &ulRet );
	/* if lVid does not exist, delete all groups belongs to this vlan node */
	if ( 0 == ulRet )
	{
		igmp_snp_delvlan( lVid );
		return IGMP_SNOOP_ERR;
	}

	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_creat_group_node()
*
* INPUTS:
*		 usIfIndex - Interface Index. The multicast router port. 
*		 ulGroup  - multicast group address. 
*		 lVid	-  vlan id
*		 ppPrevGroupVlan - list head.As a input arg, it contains the pointer refer to
*						   the list head.
*
* OUTPUTS:
*		ppMcGroup - 
*		ppPrevGroupVlan - as a output arg, it contains the vlan node the group belongs to.
* RETURN VALUE:
*		IGMP_SNOOP_OK - on success.
*		IGMP_SNOOP_ERR - on error
*
* DESCRIPTION:
*		This function creates a new node in group vlan list.
*		Notice: group node is created in 2 condition:
*				1. recv'd a group specific query
*				2. recv'd a report and there is no group of that group address.
*
*
**************************************************************************/
LONG igmp_creat_group_node( LONG lVid, ULONG ulGroup, ULONG usIfIndex,
					MC_group ** ppMcGroup, MC_group_vlan ** ppPrevGroupVlan )
{
	int tmp;
	MC_group_vlan * pVlanNode = NULL;
	MC_group *pGroup = NULL;
	USHORT usIsCreate = IGMP_SNOOP_NO;   /* 0 - indicate the vlan node is created. 1 - new vlan node */
	ULONG ulSlot = 0;
	ULONG ulMaxGroup = 0;

	IGMP_SNP_DEBUG( "\r\nigmp_creat_group_node: vid %d group 0x%x If 0x%x pVlan 0x%x\r\n",
						lVid, ulGroup, usIfIndex, ( *ppPrevGroupVlan ) );
#if 0
	if ( SYS_IS_TRUNK_IF( usIfIndex ) )
	{
		/*ulSlot = IGMP_SNOOP_DEFAULT_SLOT;  /*can not get slot by trunk ifindex*/
		IFM_GetTrunkSlotIdApi( usIfIndex, &ulSlot, FALSE, TRUE );
	}
	if ( SYS_IS_PHYSICAL_IF( usIfIndex ) )
	{
		ulSlot = SYS_IF_SLOT_ID( usIfIndex );
	}
	ulMaxGroup = IGMP_SNP_GRP_MAX;
	if ( ( MODULE_B_2GBIC_6GTX_48FE == SYS_MODULE_TYPE( ulSlot ) )
				|| ( MODULE_B_4GBIC_24FMM == SYS_MODULE_TYPE( ulSlot ) ) )
	{
		ulMaxGroup = IGMP_SNP_GRP_MAX - 1;
	}
#endif
	igmp_getmaxgroupcnt(usIfIndex,&ulMaxGroup);
	/* if the total group count is more than the max group number supported, return error*/
	if ( igmp_groupcount > ulMaxGroup )
	{
		IGMP_SNP_DEBUG( "igmp_creat_group_node: the group count reachs max %d\r\n", ulMaxGroup);
		IGMP_SNP_DEBUG("\r\n Group full!  GroupCount %d, lVid = %d usIfIndex = 0x%x \r\n",
								igmp_groupcount, lVid, usIfIndex);
		return IGMP_SNOOP_ERR;
	}

	pGroup = ( MC_group * )malloc( sizeof( MC_group ));
	if ( NULL == pGroup )
	{
		IGMP_SNP_DEBUG("igmp_creat_group_node: pMcGroup alloc failed.\r\n ");
		return IGMP_SNOOP_ERR;
	}
	memset( pGroup,0, sizeof( MC_group ) );

	pGroup->MC_ipadd= ulGroup;
	pGroup->vid = ( USHORT ) lVid;
	pGroup->lifetime = igmp_grouplife;
	pGroup->queryresposetime = IGMP_V2_QUERY_RESP_INTERVAL;
	pGroup->memberinterval = igmp_robust_variable * igmp_query_interval
								+ igmp_resp_interval;
	pGroup->sedquerytime = igmp_rxmt_interval;
	igmp_snp_addintertimer( pGroup->lifetime, &( pGroup->lifetimer_id ) );
	
	IGMP_SNP_DEBUG("igmp_creat_group_node: add group life timer %d.\r\n", pGroup->lifetime);

	/*	Create vlan node */
	pVlanNode = ( MC_group_vlan * ) malloc( sizeof( MC_group_vlan ));
	if ( NULL == pVlanNode )
	{
		IGMP_SNP_DEBUG( "igmp_creat_group_node: pVlanNode alloc failed\r\n");
		if ( NULL != pGroup )
			free( pGroup );
		return IGMP_SNOOP_ERR;
	}
	memset( pVlanNode,0, sizeof( MC_group_vlan ) );

	/* If the router port is configured, add the router port */
	{
	igmp_routerport *pRouter = NULL;

	igmp_snoop_searchrouterlist( lVid, 0, IGMP_PORT_QUERY, &pRouter );
	if ( pRouter != NULL )
	{
		igmp_router_entry * Igmp_snoop_router_temp = NULL;
		Igmp_snoop_router_temp = ( igmp_router_entry * )malloc( sizeof( igmp_router_entry ));
		if ( NULL == Igmp_snoop_router_temp )
		{
			IGMP_SNP_DEBUG( ( "igmp_creat_group_node: alloc mem fail. \r\n" ) );
			return IGMP_SNOOP_ERR;
		}
		memset( Igmp_snoop_router_temp,0, sizeof( igmp_routerport ) );               
		Igmp_snoop_router_temp->mroute_ifindex = pRouter->ifindex;
		pVlanNode->routerlist = Igmp_snoop_router_temp;
	}
	else
	{
		pVlanNode->routerlist = NULL;
	}
	}
	
	if ( *ppPrevGroupVlan != NULL )                          /* vlan node exists */
	{
		if ( ( *ppPrevGroupVlan ) ->vlan_id == lVid )       /* vlan node ( lvid ) exists, no need to create a vlan node */
		{
			free( pVlanNode );
			pVlanNode = *ppPrevGroupVlan;
			usIsCreate = IGMP_SNOOP_NO;
		}
		else
		{
			tmp = 0;
			while(tmp<IGMP_GENERAL_GUERY_MAX&&mcgroup_vlan_queue[tmp])
				tmp++;

			if( tmp < IGMP_GENERAL_GUERY_MAX )
				mcgroup_vlan_queue[tmp] = pVlanNode;
			if ( GET_VLAN_POINT_BY_INDEX(( *ppPrevGroupVlan ) ->next) != NULL )
				( GET_VLAN_POINT_BY_INDEX( (*ppPrevGroupVlan ) ->next )) ->prev = tmp;

			pVlanNode->next = ( *ppPrevGroupVlan ) ->next;
			pVlanNode->prev= GET_VLAN_POINT_BY_INDEX((*ppPrevGroupVlan)->prev)->next;
			( *ppPrevGroupVlan ) ->next = tmp;
			usIsCreate = IGMP_SNOOP_YES;
		}
	}
	else	/* The first vlan node */
	{
		first_vlan_idx = 0;
		mcgroup_vlan_queue[first_vlan_idx]= pVlanNode;
		pVlanNode->next = -1;		/*-1 --- no vlan node*/
		pVlanNode->prev = -1;
		usIsCreate = IGMP_SNOOP_YES;
	}
	*ppPrevGroupVlan = pVlanNode;  /* we need return this pointer */

	pGroup->next = pVlanNode->firstgroup;
	if ( pVlanNode->firstgroup != NULL )
	{
		( pVlanNode->firstgroup ) ->prev = pGroup;
	}
	pGroup->prev= NULL;
	pVlanNode->firstgroup = pGroup;

	/* Group count increase */
	igmp_groupcount++;

	if ( usIsCreate )
	{
		pVlanNode->vlan_id = lVid;
		pVlanNode->querytimeinterval = igmp_query_interval;
		pVlanNode->vlanlife = igmp_vlanlife;
	}

	if ( pVlanNode->querytimer_id == 0 )
	{
		/* Add gen query timer */
		igmp_snp_addintertimer( igmp_query_interval, \
						&( pVlanNode->querytimer_id ) );
	}
	*ppMcGroup = pGroup; /* Output */

	return IGMP_SNOOP_OK;
}



/**************************************************************************
* igmp_searchvlangroup()
*
* INPUTS:		
*		 group  - MC group ip address. 
*		 vlan_id	-  vlan id
*		 pregroupvlan - list head
*		 mcgroup - MC group struct pointer
*
* OUTPUTS:
*		mcgroup - NULL:can not find MC_group. 			
*		pregroupvlan - NULL: can not find vlan node
*						  
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		This function finds a node in group vlan list.
*
*		
**************************************************************************/
LONG igmp_searchvlangroup( LONG lVid, ULONG ulGroup,
			MC_group **ppMcGroup,
			MC_group_vlan **ppPrevGroupVlan )

{
	MC_group_vlan * pCurrent = NULL;
	MC_group *pGroupTemp = NULL;

	pCurrent = GET_VLAN_POINT_BY_INDEX(first_vlan_idx);
	*ppPrevGroupVlan = pCurrent;

	/* check vlan */
	if ( 0 == lVid )
	{
		IGMP_SNP_DEBUG("igmp_searchvlangroup: vlan %d doesn't exist.\r\n", lVid );
	}
	if ( IGMP_SNOOP_OK != igmp_snp_groupcheck( lVid ) )
	{
		IGMP_SNP_DEBUG( "igmp_searchvlangroup: vlan %d doesn't exist.\r\n", lVid  );
		return IGMP_SNOOP_ERR;
	}

	while ( NULL != pCurrent )
	{
		*ppPrevGroupVlan = pCurrent;
		if ( pCurrent->vlan_id != lVid )
		{
			pCurrent = GET_VLAN_POINT_BY_INDEX(pCurrent->next);
			continue;
		}
		else
		{
			pGroupTemp = pCurrent->firstgroup;
			while ( NULL != pGroupTemp )
			{
				if ( pGroupTemp->MC_ipadd != ulGroup )
				{
					pGroupTemp = pGroupTemp->next;
					continue;
				}
				else
				{
					*ppMcGroup = pGroupTemp;
					return IGMP_SNOOP_OK;
				}
			}
			return IGMP_SNOOP_OK;
		}
	}

	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_snoop_searchrouterlist()
*
* Input: usIfIndex - the port index which to be searched 
*		  lVid 		-	vlan id
*		  enflag		-  indicate the action to be taken 
* Output: ppRouter	- pointer to pointer of router node 
*
* DESCRIPTION:
*		This function will search the router list. If an element with the 
*		same port index to the input ifindex, then return the pointer to the
*		router node. If no router is found, there is a choice
*		whether create a new element and add to the list, the usFlag determine
*		how to do. If the usIfIndex == NULL and enFlag == IGMP_PORT_DEL, delete
*		any port in the vlan.
*	 
**************************************************************************/
LONG igmp_snoop_searchrouterlist( LONG lVid, ULONG usIfIndex, igmpportstate enFlag,
									igmp_routerport **ppRouter )

{
	igmp_routerport * pTemp = NULL;
	igmp_routerport *pPort = NULL;
	igmp_routerport *pPrev = NULL;

	IGMP_SNP_DEBUG("igmp_snoop_searchrouterlist: vid %d port 0x%x, flag %d \r\n",
							lVid, usIfIndex, enFlag );
	pTemp = p_routerlist;
	pPrev = pTemp;
	while ( NULL != pTemp )
	{
		if ( ( pTemp->vlan_id== lVid ) )
			break;
		pPrev = pTemp;
		pTemp = pTemp->next;
	}

	if ( NULL != pTemp )             /* Found the vlan */
	{
		if ( IGMP_PORT_DEL == enFlag )
		{
			if ( ( pTemp->ifindex != usIfIndex ) && ( 0 != usIfIndex ) )
			{
				*ppRouter = pTemp;
				return IGMP_SNOOP_ERR;
			}
			if ( NULL != pPrev )
			{
				pPrev->next = pTemp->next;
			}
			if ( pTemp == p_routerlist )         	/* if it is the list head, set the list head to NULL */
			{
				p_routerlist = pTemp->next;
			}
			free( pTemp );
			pTemp = NULL;
		}
		else if ( IGMP_PORT_ADD == enFlag )
		{
			pTemp->ifindex = usIfIndex;
			*ppRouter = pTemp;
		}
		else
		{
			*ppRouter = pTemp;
		}
		return IGMP_SNOOP_OK;
	}

	/* If NOT FOUND, look at the usflag */
	switch ( enFlag )
	{
		case IGMP_PORT_QUERY:
			*ppRouter = NULL;
			break;
		case IGMP_PORT_ADD:
			{
			/* create a new list element and add to nod*/
			pPort = ( igmp_routerport *) malloc(sizeof( igmp_routerport ));
			if ( NULL == pPort )
			{
				IGMP_SNP_DEBUG( "igmp_snoop_searchrouterlist: alloc mem fail. \r\n" );
				return IGMP_SNOOP_ERR;
			}
			memset( pPort, 0, sizeof( igmp_routerport ) );
			pPort->vlan_id= lVid;
			pPort->ifindex = usIfIndex;
			pPort->next = p_routerlist;

			p_routerlist = pPort;
			*ppRouter = pPort;
			}
			break;
		case IGMP_PORT_DEL:
			return IGMP_SNOOP_ERR;       /* not found */
		default :
			break;
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_snp_searchportlist()
*
* Input: usIfIndex - the port index which to be searched 
*		  pPortList -  port list head
*		  enflag		-  indicate the action to be taken 
* Output: ppPortState	- pointer to pointer of struct MC_port_state 
*		   pPortList - list head 
*
* DESCRIPTION:
*		This function will search a portstate list. If an element with the 
*		same port index to the input ifindex, then return the pointer to the
*		portState block. If no portstate element is found, there is a choice
*		whether create a new element and add to the list, the usFlag determine
*		how to do.
*	 
**************************************************************************/
LONG igmp_snp_searchportlist( struct MC_port_state ** ppPortList,
					ULONG usIfIndex, igmpportstate enFlag,
					struct MC_port_state **ppPortState )
{
	struct MC_port_state *pTemp = NULL;
	struct MC_port_state *pPort = NULL;
	struct MC_port_state *pPrev = NULL;

	IGMP_SNP_DEBUG( "igmp_snp_searchportlist: port 0x%x, flag %d List 0x%x\r\n",
						usIfIndex, enFlag, *ppPortList );
	pTemp = *ppPortList;
	pPrev = pTemp;
	while ( NULL != pTemp )
	{
		if ( pTemp->ifindex == usIfIndex )
			break;
		pPrev = pTemp;
		pTemp = pTemp->next;
	}

	if ( NULL != pTemp )             /* Found the port */
	{
		if ( IGMP_PORT_DEL == enFlag )
		{
			if ( NULL != pPrev )
			{
				pPrev->next = pTemp->next;
			}
			if ( pTemp == *ppPortList )         	/* if it is the list head, set the list head to NULL */
			{
				*ppPortList = pTemp->next;
			}
			free( pTemp );
		}
		else
		{
			*ppPortState = pTemp;
		}
		return IGMP_SNOOP_OK;
	}

	/* If NOT FOUND, look at the usflag */
	switch ( enFlag )
	{
		case IGMP_PORT_QUERY:
			*ppPortState = NULL;
			break;
		case IGMP_PORT_ADD:
			{
			/* create a new list element and add to nod*/
			pPort = ( struct MC_port_state * ) malloc( sizeof( struct MC_port_state ));
			if ( NULL == pPort )
			{
				IGMP_SNP_DEBUG( ( "igmp_snp_searchportlist: alloc mem fail. \r\n" ) );
				return IGMP_SNOOP_ERR;
			}
			memset( pPort,0, sizeof( struct MC_port_state ) );
			pPort->ifindex = usIfIndex;
			pPort->state = IGMP_SNP_GROUP_NOMEMBER;
			pPort->next = *ppPortList;

			*ppPortList = pPort;
			*ppPortState = pPort;
			}
			break;
		case IGMP_PORT_DEL:
			return IGMP_SNOOP_ERR;       /* not found */
		default :
			break;
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_snp_searchreporterlist()
*
* Input: ulIfIndex - the port index which to be searched 
*		  lVid 		-	vlan id
*		  enflag		-  indicate the action to be taken 
* Output: ppReporter	- pointer to pointer of reporter node 
*
* DESCRIPTION:
*		This function will search the reporter list. If an element with the 
*		same port index to the input ifindex, then return the pointer to the
*		reporter node. If no reporter is found, there is a choice
*		whether create a new element and add to the list, the usFlag determine
*		how to do. If the ulIfIndex == NULL and enFlag == IGMP_PORT_DEL, delete
*		any port in the vlan.
*	 
**************************************************************************/
LONG igmp_snp_searchreporterlist( LONG lVid, ULONG ulIfIndex, igmpportstate enFlag,
					igmp_reporter_port **ppReporter )
{
	igmp_reporter_port * pTemp = NULL;
	igmp_reporter_port *pPort = NULL;
	igmp_reporter_port *pPrev = NULL;
	member_port_list *pMemberPort = NULL, *pTempMemberPort = NULL, *pPreTempMemberPort = NULL;

	IGMP_SNP_DEBUG("igmp_snp_searchreporterlist: vid %d port 0x%x, flag %d \r\n",
									lVid, ulIfIndex, enFlag);
	/*Find the vlan node*/
	pTemp = p_reporterlist;
	pPrev = pTemp;
	while ( NULL != pTemp )
	{
		if ( pTemp->vlan_id == lVid )
			break;
		pPrev = pTemp;
		pTemp = pTemp->next;
	}

	/*Found the vlan*/
	if ( NULL != pTemp )
	{
		IGMP_SNP_DEBUG( "igmp_snp_searchreporterlist:found vlan\r\n");
		if ( IGMP_PORT_DEL == enFlag )
		{
			pTempMemberPort = pTemp->portlist;
			pPreTempMemberPort = pTempMemberPort;

			while ( NULL != pTempMemberPort )
			{
				if ( ulIfIndex == pTempMemberPort->ifindex )
				{
					if ( pPreTempMemberPort == pTempMemberPort && NULL == pTempMemberPort->next )
					{ /*The last member port is deleted from vlan,so del the vlan node*/
						free( pTempMemberPort );

						if ( pPrev == pTemp && NULL == pTemp->next )
						{ /*The last vlan node in member port list*/
							free( pTemp );
							p_reporterlist = NULL;
						}
						else if ( pPrev == pTemp )
						{ /*del the first vlan node in portlist*/
							p_reporterlist = pTemp->next;
							free( pTemp );
						}
						else
						{
							pPrev->next = pTemp->next;
							free( pTemp );
						}
					}
					else if ( pPreTempMemberPort == pTempMemberPort )
					{ /*del the first member port in portlist*/
						pTemp->portlist = pTempMemberPort->next;
						free( pTempMemberPort );
					}
					else
					{
						pPreTempMemberPort->next = pTempMemberPort->next;
						free( pTempMemberPort );
					}
					break;
				}
				pPreTempMemberPort = pTempMemberPort;
				pTempMemberPort = pTempMemberPort->next;
			}
			if ( NULL == pTempMemberPort )
			{
				return IGMP_SNOOP_ERR;
			}
		}
		else if ( IGMP_PORT_ADD == enFlag )  /*The member port is already in portlist when add member port*/
		{
			pTempMemberPort = pTemp->portlist;
			pPreTempMemberPort = pTempMemberPort;

			while ( NULL != pTempMemberPort )
			{
				if ( ulIfIndex == pTempMemberPort->ifindex )
				{ /*This port is in portlist already*/
					return IGMP_SNOOP_ERR;
				}

				pPreTempMemberPort = pTempMemberPort;
				pTempMemberPort = pTempMemberPort->next;
			}

			if ( NULL == pTempMemberPort )
			{
				pMemberPort = ( member_port_list * )malloc( sizeof( member_port_list ));
				if ( NULL == pMemberPort )
				{
					IGMP_SNP_DEBUG( ( "igmp_snp_searchreporterlist: alloc mem fail. \r\n" ) );
					return IGMP_SNOOP_ERR;
				}
				pMemberPort->ifindex = ulIfIndex;
				pMemberPort->next = NULL;
				pPreTempMemberPort->next = pMemberPort;
			}
		}
		else if ( IGMP_PORT_QUERY == enFlag )
		{
			*ppReporter = pTemp;
		}
		return IGMP_SNOOP_OK;
	}

	IGMP_SNP_DEBUG( ( "igmp_snp_searchreporterlist:no found vlan\r\n" ) );

	/* If NOT FOUND, look at the usflag */
	switch ( enFlag )
	{
		case IGMP_PORT_QUERY:
			*ppReporter = NULL;
			break;
		case IGMP_PORT_ADD:
			{
			/* create a new vlan node and add to list*/
			pPort = ( igmp_reporter_port * )malloc( sizeof( igmp_reporter_port ));
			if ( NULL == pPort )
			{
				IGMP_SNP_DEBUG( ( "igmp_snp_searchreporterlist: alloc mem fail. \r\n" ) );
				return IGMP_SNOOP_ERR;
			}
			memset( pPort,0, sizeof( igmp_reporter_port ) );

			pMemberPort = ( member_port_list * )malloc( sizeof( member_port_list ));
			memset( pMemberPort, 0,sizeof( member_port_list ) );
			pPort->vlan_id= lVid;

			if ( NULL == pMemberPort )
			{
				IGMP_SNP_DEBUG( ( "igmp_snp_searchreporterlist: alloc mem fail. \r\n" ) );
				return IGMP_SNOOP_ERR;
			}

			pMemberPort->ifindex = ulIfIndex;
			pMemberPort->next = NULL;

			pPort->portlist = pMemberPort;
			pPort->next = p_reporterlist;

			p_reporterlist = pPort;
			*ppReporter = pPort;
			}
			break;

		case IGMP_PORT_DEL:
			return IGMP_SNOOP_ERR;       /* not found */
		default :
			break;
	}
	return IGMP_SNOOP_OK;
}

LONG Igmp_Snoop_Del_Reporter_ByVlan( USHORT lVid )
{
	igmp_reporter_port * pTemp = NULL;
	igmp_reporter_port *pPrev = NULL;

	IGMP_SNP_DEBUG( "Igmp_Snoop_Del_Reporter_ByVlan: vid %d \r\n", lVid);

	/*Find the vlan node*/
	pTemp = p_reporterlist;
	pPrev = pTemp;
	while ( NULL != pTemp )
	{
		if ( pTemp->vlan_id== lVid )
			break;
		pPrev = pTemp;
		pTemp = pTemp->next;
	}

	if ( NULL != pTemp )
	{
		if ( pPrev == pTemp && NULL == pTemp->next )
		{ /*The last node in member port list*/
			free( pTemp );
			p_reporterlist = NULL;
		}
		else
		{
			if ( pTemp == pPrev )
			{
				p_reporterlist = pTemp->next;
			}
			else
			{
				pPrev->next = pTemp->next;
			}
			free( pTemp );
		}
	}
	else
	{
		IGMP_SNP_DEBUG("Igmp_Snoop_Del_Reporter_ByVlan:  no this vlan in member port list\r\n");
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* Igmp_Event_DelVlan()
*
* INPUTS:
*		pPkt - Igmp_Snoop_pkt_t structure pointer 	
*
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		Igmp_Event_VlanDelPort handles the vlan deleting event. It will
*		delete the vlan node and all group node belongs to this vlan.
*		
*
**************************************************************************/
LONG Igmp_Event_DelVlan( igmp_snoop_pkt * pPkt )
{
	IGMP_SNP_DEBUG( "\r\nEnter Igmp_Event_DelVlan. \r\n");

	/* search the vlan node
	Notice: DO NOT use the group search routine, because it will validate the vlan id 
	and it is possible that the vlan didn't exist at that time */
	igmp_snp_delvlan( pPkt->vlan_id);
	Igmp_Snoop_Del_Reporter_ByVlan( pPkt->vlan_id);

	free( pPkt );
	return IGMP_SNOOP_OK;
}


LONG Igmp_Event_PortDown( igmp_snoop_pkt * pPkt )
{
	MC_group * pMcGroup = NULL;
	MC_group_vlan *pPrevGroupVlan = NULL;

	LONG lRet;
	LONG lVid;
	struct MC_port_state *pstPort = NULL;
	igmp_snoop_pkt stPkt;
	igmp_router_entry * Igmp_snoop_router_temp;
	igmp_router_entry ** Igmp_snoop_router_pre;

	IGMP_SNP_DEBUG("\r\nEnter Igmp_Event_PortDown. \r\n" );

	pPrevGroupVlan = GET_VLAN_POINT_BY_INDEX(first_vlan_idx);
	while (pPrevGroupVlan != NULL)
	{     
		lVid= pPrevGroupVlan->vlan_id;
		if( pPkt->vlan_id&& lVid != pPkt->vlan_id)
		{
			if( pPrevGroupVlan->next == -1)	/*到队列尾*/
				break;	
			pPrevGroupVlan = GET_VLAN_POINT_BY_INDEX(pPrevGroupVlan->next);
			continue;
		}
		/* delete vlan router port */
		/* if router port is deleted from vlan, delete it from port list */
		IGMP_SNP_DEBUG(" Igmp_Event_PortDown:Del vlan router port.\r\n");
		Igmp_snoop_router_temp = pPrevGroupVlan->routerlist;
		Igmp_snoop_router_pre = &( pPrevGroupVlan->routerlist );
		while ( Igmp_snoop_router_temp != NULL )
		{
			if ( Igmp_snoop_router_temp->mroute_ifindex == pPkt->ifindex )
			{
				*Igmp_snoop_router_pre = Igmp_snoop_router_temp->next;
				free( Igmp_snoop_router_temp );
				Igmp_snoop_router_temp = NULL;
				break;
			}
			Igmp_snoop_router_pre = &( Igmp_snoop_router_temp->next );
			Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
		}

		IGMP_SNP_DEBUG(" Igmp_Event_PortDown:Del group member.\r\n");
		pMcGroup = pPrevGroupVlan->firstgroup;
		while ( pMcGroup != NULL )
		{
			if ( Igmp_Snoop_IF_Recv_Is_Report_Discard( lVid, pPkt->ifindex ) == IGMP_SNOOP_ERR )
			{
				IGMP_SNP_DEBUG("\r\n This is a reporter port. \r\n");
				free( pPkt );
				return IGMP_SNOOP_OK;
			}
			/* scan portlist */
			lRet = igmp_snp_searchportlist( &( pMcGroup->portstatelist ), pPkt->ifindex,
			                        IGMP_PORT_DEL, &pstPort );
			if ( NULL == pMcGroup->portstatelist )
			{
				igmp_snp_addintertimer( igmp_grouplife, &( pMcGroup->lifetimer_id ) );
			}
			IGMP_SNP_DEBUG( " Igmp_Event_PortDown:Enter Del group member.\r\n");

			if ( IGMP_SNOOP_OK == lRet )
			{
				stPkt.type = IGMP_ADDR_DEL;
				stPkt.ifindex = pPkt->ifindex;
				stPkt.groupadd= pMcGroup->MC_ipadd;
				stPkt.vlan_id= lVid;
				if ( IGMP_SNOOP_OK != ( lRet = igmp_snp_mod_addr( &stPkt , IGMP_ADDR_DEL ) ) )
				{
					IGMP_SNP_DEBUG("Igmp_Event_PortDown: failed in setting L2 MC table. Slot/port: 0x%x \r\n", pPkt->ifindex);
				}
			}
			pMcGroup = pMcGroup->next;
			}
			if( pPrevGroupVlan->next == -1)	/*到队列尾*/
				break;	
			pPrevGroupVlan = GET_VLAN_POINT_BY_INDEX(pPrevGroupVlan->next);
		}
	/*
	An IGMP snooping switch should be aware of link layer topology
	changes caused by Spanning Tree operation. When a port is
	enabled or disabled by Spanning Tree, a General Query may be
	sent on all active non-router ports in order to reduce network
	convergence time. */
	Igmp_Event_GenQuery_Timeout( pPrevGroupVlan );
	free( pPkt );
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* Igmp_Event_VlanDelPort()
*
* INPUTS:
*		pPkt - igmp_snoop_pkt structure pointer 	
*
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		Igmp_Event_VlanDelPort handles the vlan port deleting event. It will
*	iterate vlan group list, search the port and delete from group member .
*
* CALLED BY:
*		Igmp_TimerExp_Proc()
* CALLS:
*		
* Modify Log:  
*		Create by Lideer 03/24/2003
*
**************************************************************************/
LONG Igmp_Event_VlanDelPort( igmp_snoop_pkt * pPkt )
{
	MC_group * pMcGroup = NULL;
	MC_group_vlan *pPrevGroupVlan = NULL;

	LONG lRet;
	struct MC_port_state *pstPort = NULL;
	igmp_snoop_pkt stPkt;
	igmp_router_entry * Igmp_snoop_router_temp;
	igmp_router_entry ** Igmp_snoop_router_pre;

	IGMP_SNP_DEBUG("\r\nEnter Igmp_Event_VlanDelPort. \r\n");
	/* delete port from user configured router port list */
	{
		igmp_routerport *pRouter = NULL;
		igmp_snoop_searchrouterlist( pPkt->vlan_id, pPkt->ifindex, IGMP_PORT_DEL, &pRouter );
	}

	/* delete port from user configured member port list */
	{
		igmp_reporter_port *pReporter = NULL;
		igmp_snp_searchreporterlist( pPkt->vlan_id, pPkt->ifindex, IGMP_PORT_DEL, &pReporter );
	}

	/* check vlan and group existence */
	if ( IGMP_SNOOP_OK != ( lRet = igmp_searchvlangroup( pPkt->vlan_id, 0, &pMcGroup, &pPrevGroupVlan ) ) )
	{
		IGMP_SNP_DEBUG("Igmp_Event_VlanDelPort: search failed\r\n");
		free( pPkt );
		return IGMP_SNOOP_ERR;
	}

	if ( NULL == pPrevGroupVlan )
	{
		IGMP_SNP_DEBUG(" Igmp_Event_VlanDelPort:No vlan node found.\r\n");
		free( pPkt );
		return IGMP_SNOOP_ERR;
	}

	if ( pPrevGroupVlan->vlan_id!= pPkt->vlan_id)
	{
		IGMP_SNP_DEBUG(" Igmp_Event_VlanDelPort:vlan:%d does not exist.\r\n", pPkt->vlan_id);
		free( pPkt );
		return IGMP_SNOOP_ERR;
	}
	/* delete vlan router port */
	/* if router port is deleted from vlan, delete it from port list */
	IGMP_SNP_DEBUG( " Igmp_Event_VlanDelPort:Del vlan router port.\r\n");
	Igmp_snoop_router_temp = pPrevGroupVlan->routerlist;
	Igmp_snoop_router_pre = &( pPrevGroupVlan->routerlist );
	while ( Igmp_snoop_router_temp != NULL )
	{
		if ( Igmp_snoop_router_temp->mroute_ifindex == pPkt->ifindex )
		{
			*Igmp_snoop_router_pre = Igmp_snoop_router_temp->next;
			free( Igmp_snoop_router_temp );
			Igmp_snoop_router_temp = NULL;
			break;
		}
		Igmp_snoop_router_pre = &( Igmp_snoop_router_temp->next );
		Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
	}
	IGMP_SNP_DEBUG(" Igmp_Event_VlanDelPort:Del group member.\r\n");
	pMcGroup = pPrevGroupVlan->firstgroup;
	while ( pMcGroup != NULL )
	{
		/* scan portlist */
		lRet = igmp_snp_searchportlist( &( pMcGroup->portstatelist ), pPkt->ifindex,
		                IGMP_PORT_DEL, &pstPort );
		if ( NULL == pMcGroup->portstatelist )
		{
			igmp_snp_addintertimer( igmp_grouplife, &( pMcGroup->lifetimer_id ) );
		}
		if ( IGMP_SNOOP_OK == lRet )
		{
			stPkt.vlan_id= pPkt->vlan_id;
			stPkt.type = IGMP_ADDR_DEL;
			stPkt.ifindex = pPkt->ifindex;
			stPkt.groupadd= pMcGroup->MC_ipadd;
			
			if ( IGMP_SNOOP_OK != ( lRet = igmp_snp_mod_addr( &stPkt , IGMP_ADDR_DEL ) ) )
			{
				IGMP_SNP_DEBUG("Igmp_Event_VlanDelPort: failed in setting L2 MC table. Slot/port: 0x%x \r\n", pPkt->ifindex );
			}
		}
		pMcGroup = pMcGroup->next;
	}
	free( pPkt );
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_snp_delvlan()
*
* DESCRIPTION:
*		igmp_snp_delvlan() delete a vlan node and its group nodes. 
* 
**************************************************************************/
LONG igmp_snp_delvlan( LONG vlan_id )
{
	MC_group * pMcGroup = NULL;
	MC_group *pNext = NULL;
	MC_group_vlan *pVlan = NULL;
	igmp_router_entry* Igmp_snoop_router_temp;
	igmp_router_entry* Igmp_snoop_router_pre;
	static ULONG ulCount=0;
	INT	vlan_idx;

	vlan_idx = first_vlan_idx;
	pVlan = GET_VLAN_POINT_BY_INDEX(vlan_idx);
	while ( pVlan != NULL )
	{
		if ( vlan_id == pVlan->vlan_id )
		{
			pMcGroup = pVlan->firstgroup;
			while ( pMcGroup != NULL )
			{
				ulCount++;
				igmp_snp_delgroup( pVlan, pMcGroup, &pNext );
				pMcGroup = pNext;
			}

			/*zgm add delete router port*/
			if ( pVlan->routerlist != NULL )
			{
				Igmp_snoop_router_temp = pVlan->routerlist;
				while ( Igmp_snoop_router_temp != NULL )
				{
					Igmp_snoop_router_pre = Igmp_snoop_router_temp;
					Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
					free( Igmp_snoop_router_pre );
				}
				pVlan->routerlist = NULL;
			}

			/* delete general query timer */
			IGMP_SNP_DEL_INTERTIMER( pVlan->querytimer_id );

			if ( ( pVlan->next == -1 ) && ( pVlan->prev== -1 ) )
			{
				CLEAR_VLAN_POINT_BY_INDEX(vlan_idx);
				first_vlan_idx = -1;
			}
			else
			{
				CLEAR_VLAN_POINT_BY_INDEX(vlan_idx);
				if ( pVlan->next != -1 )
				{
					GET_VLAN_POINT_BY_INDEX(pVlan->next)->prev = pVlan->prev;
				}
				if ( pVlan->prev != -1 )
				{
					GET_VLAN_POINT_BY_INDEX(pVlan->prev)->next = pVlan->next;
				}
				else
				{
					first_vlan_idx = pVlan->next;
				}
			}
			free( pVlan );
			IGMP_SNP_DEBUG( "igmp_snp_delvlan: Vlan node %d deleted.\r\n", vlan_id );
			break;
		}
		else
		{
			pVlan = GET_VLAN_POINT_BY_INDEX(pVlan->next);
		}
	}

	return IGMP_SNOOP_OK;
}



/**************************************************************************
* igmp_snp_delgroup()
*
* DESCRIPTION:
*		igmp_snp_delgroup() delete a group node. At first, the group
*		should be searched. When return, output the next group.
**************************************************************************/
LONG igmp_snp_delgroup( MC_group_vlan * pVlan, MC_group * pGroup, MC_group **ppNextGroup )
{
	/*ULONG ulTimerId = 0;*/
	igmp_snoop_pkt pkt ;
	struct MC_port_state *pstPort = NULL;

	if ( NULL == pGroup )
	{
		return IGMP_SNOOP_ERR;
	}
	*ppNextGroup = pGroup->next;

	pkt.vlan_id = pGroup->vid;
	pkt.groupadd= pGroup->MC_ipadd;
	pkt.type = IGMP_ADDR_RMV;
	pkt.ifindex = 0;


	/* remove group from hardware table*/
	if ( IGMP_SNOOP_OK != ( igmp_snp_mod_addr( ( VOID * ) & pkt , IGMP_ADDR_RMV ) ) )
	{
		IGMP_SNP_DEBUG("igmp_snp_delgroup: failed in setting L2 MC table. 0x%x \r\n", pkt.ifindex);
	}

	while ( NULL != pGroup->portstatelist )
	{
		pstPort = pGroup->portstatelist;
		pGroup->portstatelist = pstPort->next;
		free( pstPort );
	}

	/* delete group node  */
	if ( !( pGroup->next== NULL && pGroup->prev== NULL ) )
	{
		if ( pGroup->next != NULL )
		{
			pGroup->next->prev= pGroup->prev;
		}
		if ( pGroup->prev!= NULL )
		{
			pGroup->prev->next= pGroup->next;
		}
		else
		{
			pVlan->firstgroup= pGroup->next;
		}
	}
	else
	{
		pVlan->firstgroup= NULL;
		/* don't delete vlan node, because we want to save the user configured router port,
			So just delete the general query timer */
		IGMP_SNP_DEL_INTERTIMER( pVlan->querytimer_id );
	}
	free( pGroup );

	/* Group count decrease */
	if ( igmp_groupcount > 0 )
	{
		igmp_groupcount--;
	}

	return IGMP_SNOOP_OK;
}

/**************************************************************************
* igmp_snp_file_option_field()
*
* pOptionBuf - Ip option buffer
* ulLength - Ip option buffer length
* DESCRIPTION:
*		igmp_snp_file_option_field() fill ip option field
* Created by yuanzhiyong on 2003/7/26 14:38:52 
**************************************************************************/
VOID igmp_snp_file_option_field( UCHAR * pOptionBuf, ULONG ulLength )
{
	if ( ( !pOptionBuf )
	   || ( ulLength != IGMP_ROUTER_ALERT_LEN ) )
		return ;

	pOptionBuf[ 0 ] = 0x94;
	pOptionBuf[ 1 ] = 0x04;
	pOptionBuf[ 2 ] = 0x00;
	pOptionBuf[ 3 ] = 0x00;
}

void Igmp_Snoop_Send_Packet(struct igmp_skb *msg_skb,UINT datalen,LONG vlan_id,ULONG ifindex)
{		/*目前测试阶段只是写到socket中*/
	*(( ULONG *)msg_skb->buf +1) = vlan_id;
	*((LONG *)msg_skb->buf ) = ifindex;
	IGMP_SNP_DEBUG("Send Packet:\r\n");
	igmp_debug_print_skb(msg_skb);
	if( 0 != kernel_fd )
		write(kernel_fd,(char *)(msg_skb),datalen);
}


/**************************************************************************
* igmp_snp_routerleave()
*
* Input: usIfIndex - the router port 
*
* DESCRIPTION:
*		When a group is added or a router port is configured to a vlan,
*		send a leave to let the router del the member
*		port.
**************************************************************************/
LONG igmp_snp_routerleave( LONG lVid, ULONG usIfIndex, ULONG ulGroup, ULONG ulSaddr )
{
	igmp_snoop_pkt stPkt;

	IGMP_SNP_DEBUG(" igmp_snp_routerleave: lVid 0x%x, usIfIndex 0x%x, ulGroup 0x%x \r\n" ,
	                  lVid, usIfIndex, ulGroup) ;
	stPkt.vlan_id= lVid;
	stPkt.ifindex = usIfIndex;
	stPkt.groupadd= ulGroup;
	stPkt.type = IGMP_MSG_LEAVE;
	stPkt.saddr = ulSaddr;
	return Igmp_Snoop_Send_Igmp( &stPkt );
}

/**************************************************************************
* Igmp_Snoop_RouterReport()
*
* Input: usIfIndex - the router port 
*
* DESCRIPTION:
*		When a group is added or a router port is configured to a vlan,
*		send a report ( or a report + leave ) to let the router add the member
*		port.
**************************************************************************/
LONG Igmp_Snoop_RouterReport( LONG lVid, ULONG usIfIndex, ULONG ulGroup, struct igmp_info *sk_info )
{
	igmp_snoop_pkt stPkt;
	struct iphdr *pIphd = NULL;
	IGMP_SNP_DEBUG( " Igmp_Snoop_RouterReport: lVid 0x%x, usIfIndex 0x%x, ulGroup 0x%x \r\n" ,
	      			lVid, usIfIndex, ulGroup ) ;
	pIphd = sk_info->ip_hdr;
	stPkt.vlan_id = lVid;
	stPkt.ifindex = usIfIndex;
	stPkt.groupadd= ulGroup;
	stPkt.type = IGMP_MSG_REPORT;
	stPkt.saddr = pIphd->saddr;
	return Igmp_Snoop_Send_Igmp( &stPkt );
}


/**************************************************************************
* igmp_snp_flood()
*
* pSkbuf - struct sk_buff header + ip PDU
* usIfIndex - is the input port, do not send packet from this port
* DESCRIPTION:
*		igmp_snp_flood() flood Igmp packets in a specific vlan 
**************************************************************************/
LONG igmp_snp_flood( struct igmp_info *sk_info, LONG lVid, ULONG usIfIndex )
{
	ULONG unIfIndex = 0;
	UCHAR *pBuf = NULL;
	LONG lBufSize;
	ULONG ulportstatus;
	igmp_queryport *PTemp = NULL;

	lBufSize = sizeof(struct igmp_skb);
	pBuf = ( UCHAR * )malloc( lBufSize);
	if ( NULL == pBuf )
	{
		IGMP_SNP_DEBUG( "igmp_snp_flood: failed in alloc mem for pBuf.\r\n" );
		return IGMP_SNOOP_ERR;
	}
	memset( pBuf,0, lBufSize );
	memcpy( pBuf, ( UCHAR* )( sk_info->data), lBufSize );
	IGMP_SNP_DEBUG(" data: 0x%x  %d\r\n", pBuf, lBufSize );
//	igmp_debug_print_skb(sk_info->data);
	if(IGMP_SNOOP_OK !=igmp_getifindex_byvlanid(lVid,&unIfIndex) )
	{
		IGMP_SNP_DEBUG("igmp_snp_flood: can not get ifindex by vlan_id.\r\n");
		return IGMP_SNOOP_ERR;
	}
	while(0 != unIfIndex )
	{
		if ( unIfIndex == usIfIndex )
		{
			unIfIndex = igmp_get_nextifindex_byifindex(lVid,unIfIndex);
			continue;
		}
		if ( p_queryportlist != NULL )
		{
			PTemp = p_queryportlist;
			while ( PTemp != NULL )
			{
				if ( PTemp->ifindex == unIfIndex )
					break;
				PTemp = PTemp->next;
			}
			if ( PTemp != NULL )
			{
				igmp_getifstatus( unIfIndex, &ulportstatus );
				if ( ulportstatus == 1 )	/*UP*/
					Igmp_Snoop_Send_Packet( (struct igmp_skb *)pBuf, lBufSize, lVid, unIfIndex );
			}
		}
		else
		{
			igmp_getifstatus( unIfIndex, &ulportstatus );
			if ( ulportstatus == 1 )		/*UP*/
				Igmp_Snoop_Send_Packet( (struct igmp_skb *)pBuf, lBufSize, lVid, unIfIndex );
		}
		unIfIndex = igmp_get_nextifindex_byifindex(lVid, unIfIndex);
	}
}


/**************************************************************************
* Igmp_Snoop_Send_Igmp()
*
* DESCRIPTION:
*		Igmp_Snoop_Send_Igmp() send Igmp packet
**************************************************************************/
LONG Igmp_Snoop_Send_Igmp( igmp_snoop_pkt * pPkt )
{
	UINT	 ethlen;
	ULONG taged = 0;
	struct iphdr * pIphd = NULL;
	struct igmp *pIgmp = NULL;
	ULONG unVlanIf;
	LONG lRet;
	ULONG unIfIndex = 0;
	UCHAR *pBuf = NULL;		/* IP PDU */
	ULONG ulBufLen = 0;
	ULONG ulIpAddr, ulIpMask;
	ULONG ulportstatus;
	igmp_queryport *PTemp = NULL;
	
	IGMP_SNP_DEBUG( "\r\nEnter Igmp_Snoop_Send_Igmp: vlan %d, group 0x%x, type %d, If 0x%x\r\n",
								pPkt->vlan_id, pPkt->groupadd, pPkt->type, pPkt->ifindex);

	unIfIndex = pPkt->ifindex;

	/*for igmp report/leave packet , need add ip option: router alert*/
	switch ( pPkt->type )
	{
		case IGMP_MSG_GEN_QUERY:
		case IGMP_MSG_GS_QUERY:
		case IGMP_MSG_V1_REPORT:
			ulBufLen = sizeof( struct iphdr ) + sizeof( struct igmp );
			break;
		case IGMP_MSG_REPORT:
		case IGMP_MSG_LEAVE:
			ulBufLen = sizeof( struct iphdr ) + sizeof( struct igmp ) + IGMP_ROUTER_ALERT_LEN;
			break;
		default:
			/*It seems that igmp v3 report need add */
			ulBufLen = sizeof( struct iphdr ) + sizeof( struct igmp ) + IGMP_ROUTER_ALERT_LEN;
			break;
	}

	if(IGMP_SNOOP_OK != igmp_vlanportrelation(pPkt->ifindex,pPkt->vlan_id,&taged))
	{
		IGMP_SNP_DEBUG( ( "Igmp_Snoop_Send_Igmp:get ifindex targged failed.\r\n" ) );
		return IGMP_SNOOP_ERR;
	}
	if( taged )		/*targged*/
		ethlen = 18;
	else
		ethlen = 14;
	
	pBuf = ( UCHAR * )malloc( ethlen + ulBufLen + 8 + sizeof(struct nlmsghdr));
	if ( NULL == pBuf )
	{
		IGMP_SNP_DEBUG( ( "Igmp_Snoop_Send_Igmp: alloc mem for pBuf failed.\r\n" ) );
		return IGMP_SNOOP_ERR;
	}
	memset( pBuf,0, (ethlen + ulBufLen + 8 + sizeof(struct nlmsghdr)) );

	pIphd = ( struct iphdr * ) (pBuf + 8 + ethlen + sizeof(struct nlmsghdr));
	/*for igmp report/leave packet , need add ip option: router alert*/
	switch ( pPkt->type )
	{
		case IGMP_MSG_GEN_QUERY:
		case IGMP_MSG_GS_QUERY:
		case IGMP_MSG_V1_REPORT:
			pIgmp = ( struct igmp * ) ( pIphd + 1 );
			break;
		case IGMP_MSG_REPORT:
		case IGMP_MSG_LEAVE:
			igmp_snp_file_option_field( ( UCHAR * ) ( pIphd + 1 ), IGMP_ROUTER_ALERT_LEN );
			pIgmp = ( struct igmp * ) ( ( UCHAR * ) ( pIphd + 1 ) + IGMP_ROUTER_ALERT_LEN );
			break;
		default:
			/*It seems that igmp v3 report need add */
			igmp_snp_file_option_field( ( UCHAR * ) ( pIphd + 1 ), IGMP_ROUTER_ALERT_LEN );
			pIgmp = ( struct igmp * ) ( ( UCHAR * ) ( pIphd + 1 ) + IGMP_ROUTER_ALERT_LEN );
			break;
	}
	/* generate IGMP PDU */
	pIgmp->igmp_code = 0;
	pIgmp->igmp_group.s_addr = pPkt->groupadd;

	switch ( pPkt->type )
	{
		case IGMP_MSG_GEN_QUERY:
			/* If a general query, send to all port except the router port in the pkt->ifindex */
			pIgmp->igmp_code = igmp_resp_interval * IGMP_V2_SEC_2_MILL;
			pIgmp->igmp_group.s_addr = 0;  /* general query */
			pIgmp->igmp_type = IGMP_MEMSHIP_QUERY;
			pIphd->daddr = htonl( IGMP_ALL_SYSTEM_ADDR );
			break;
		case IGMP_MSG_GS_QUERY:
			/* If a g-s query, send to the specific port */
			pIgmp->igmp_code = igmp_rxmt_interval / IGMP_V2_TIME_SCALE;
			pIgmp->igmp_type = IGMP_MEMSHIP_QUERY;
			pIphd->daddr = htonl( pPkt->groupadd );
			break;
		case IGMP_MSG_REPORT:
			/* If a report, send to the router port */
			pIgmp->igmp_type = IGMP_V2_MEMSHIP_REPORT;
			pIphd->daddr = htonl( pPkt->groupadd);
			break;
		case IGMP_MSG_V1_REPORT:
			/* If a V1 report, send to the router port */
			pIgmp->igmp_type = IGMP_V1_MEMSHIP_REPORT;
			pIphd->daddr = htonl( pPkt->groupadd );
			break;
		case IGMP_MSG_LEAVE:
			pIgmp->igmp_type = IGMP_V2_LEAVE_GROUP;
			pIphd->daddr = htonl( IGMP_ALL_ROUTER_ADDR );
			break;
		default:
			break;
	}
	pIgmp->igmp_cksum = 0;
	pIgmp->igmp_cksum = inet_cksum( ( USHORT * ) pIgmp, sizeof( struct igmp ) );

	/* generate IP header */
	pIphd->ttl = IGMP_TTL; /* IGMP packet ttl = 1 */
	/*pIphd->ihl = sizeof( struct iphdr ) >> 2; */ /* numbers of 32-bit block*/
	pIphd->protocol = IPPROTO_IGMP;
	pIphd->tos = IGMP_TOS;
	pIphd->version = IPVERSION;
	/*pIphd->tot_len = sizeof( struct iphdr ) + sizeof( struct igmp );*/
	pIphd->id = 1;
	pIphd->frag_off = 0;
	pIphd->check = 0;

	/*for igmp report/leave packet , need add ip option: router alert*/
	switch ( pPkt->type )
	{
		case IGMP_MSG_GEN_QUERY:
		case IGMP_MSG_GS_QUERY:
		case IGMP_MSG_V1_REPORT:
			pIphd->ihl = sizeof( struct iphdr ) >> 2;
			pIphd->tot_len = sizeof( struct iphdr ) + sizeof( struct igmp );
			break;
		case IGMP_MSG_REPORT:
		case IGMP_MSG_LEAVE:
			pIphd->ihl = ( sizeof( struct iphdr ) + IGMP_ROUTER_ALERT_LEN ) >> 2;
			pIphd->tot_len = sizeof( struct iphdr ) + sizeof( struct igmp ) + IGMP_ROUTER_ALERT_LEN;
			break;
		default:
			/*It seems that igmp v3 report need add */
			pIphd->ihl = ( sizeof( struct iphdr ) + IGMP_ROUTER_ALERT_LEN ) >> 2;
			pIphd->tot_len = sizeof( struct iphdr ) + sizeof( struct igmp ) + IGMP_ROUTER_ALERT_LEN;
			break;
	}

	/* Get vlan's IP address. If no ip addr, use 0.0.0.0 as SIP */
#if 0
	SYS_CREATE_VLAN_IF( unVlanIf, pPkt->lVid );
	lRet = IFM_Get3LayerIpAddr( unVlanIf, &ulIpAddr, &ulIpMask, 0 );
#endif
	lRet = igmp_getvlan_addr(pPkt->vlan_id,&ulIpAddr);
	if ( ( lRet != IGMP_SNOOP_OK ) )
	{
		pIphd->saddr = pPkt->saddr;
	}
	else
	{
		pIphd->saddr = htonl( ulIpAddr );
	}
	pIphd->check = inet_cksum( ( USHORT * ) pIphd, ( ULONG ) ( pIphd->ihl ) << 2 );

	/* If it is the geneal query, should send packet to vlan except the router port
	If the other types, send the packet directly to unIfIndex.
	*/
	if ( pPkt->type == IGMP_MSG_GEN_QUERY )        /* general query, send pkt to all port except the unIfIndex port */
	{
		IGMP_SNP_DEBUG( ( "Send General query.\r\n" ) );

		if(IGMP_SNOOP_OK !=igmp_getifindex_byvlanid(pPkt->vlan_id,&unIfIndex) )
		{
			IGMP_SNP_DEBUG("igmp_snp_flood: can not get ifindex by vlan_id.\r\n");
			return IGMP_SNOOP_ERR;
		}
		//IFM_PORTONVLANSTART( unIfIndex, ( USHORT ) ( pPkt->lVid ) )
		while(0 !=unIfIndex)
		{

			if ( unIfIndex == pPkt->ifindex )
			{
				unIfIndex = igmp_get_nextifindex_byifindex(pPkt->vlan_id,unIfIndex);
				continue;
			}
			
			igmp_getifstatus( unIfIndex, &ulportstatus );
			if ( ulportstatus == 1 )	/*将来要修改pBuf结构*/
				Igmp_Snoop_Send_Packet( (struct igmp_skb *)pBuf, 
								ethlen + ulBufLen + 8 + sizeof(struct nlmsghdr), 
								pPkt->vlan_id, unIfIndex );
			unIfIndex = igmp_get_nextifindex_byifindex(pPkt->vlan_id, unIfIndex);
		}
		//IFM_PORTONVLANEND;
	}
	/* IGMP_SNP_DEBUG(("Send to If 0x%x\r\n",unIfIndex.usIfIndex)); */
	else if ( pPkt->type == IGMP_MSG_GS_QUERY )
	{
		if ( p_queryportlist != NULL )
		{
			PTemp = p_queryportlist;
			while ( PTemp != NULL )
			{
				if ( PTemp->ifindex == unIfIndex )
					break;
				PTemp = PTemp->next;
			}
			if ( PTemp != NULL )
			{
				igmp_getifstatus( unIfIndex, &ulportstatus );
				if ( ulportstatus == 1 )
				{
					Igmp_Snoop_Send_Packet( (struct igmp_skb *)pBuf, 
									ethlen + ulBufLen + 8 + sizeof(struct nlmsghdr),
									pPkt->vlan_id, unIfIndex );
					IGMP_SNP_DEBUG("Configure:Send to If 0x%x\r\n", unIfIndex);
				}
			}
		}
		else
		{
			igmp_getifstatus( unIfIndex, &ulportstatus );
			if ( ulportstatus == 1 )
			{
				Igmp_Snoop_Send_Packet( (struct igmp_skb *)pBuf, 
									ethlen + ulBufLen + 8 + sizeof(struct nlmsghdr), 
									pPkt->vlan_id, unIfIndex );
				IGMP_SNP_DEBUG( "Send  to If 0x%x\r\n", unIfIndex);
			}
		}
	}
	else
	{
		igmp_getifstatus( unIfIndex, &ulportstatus );
		if ( ulportstatus == 1 )
			Igmp_Snoop_Send_Packet( (struct igmp_skb *)pBuf, 
									ethlen + ulBufLen + 8 + sizeof(struct nlmsghdr), 
									pPkt->vlan_id, pPkt->ifindex );
	}
	free( pBuf );
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_snp_addintertimer()
*
* Input: ulSec -  the time value in seconds
*		  ulFlag  -  Timer flag: HOS_TIMER_LOOP, HOS_TIMER_NO_LOOP etc.
* Output: 
*		  if returns ok, it is the timer id
* DESCRIPTION:
*		igmp_snp_addintertimer() add a local timer. 
**************************************************************************/
LONG igmp_snp_addintertimer( ULONG ulSec, ULONG * pulTimerId )
{
	*pulTimerId = ulSec;
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* Igmp_Event_GenQuery_Timeout()
*
* INPUTS:
*		pPkt - igmp_snoop_pkt structure pointer 	
*
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		Igmp_Event_GenQuery_Timeout handles the general query interval timeout
*		message. Every vlan has a general query interval timer. When the timer
*		expired, general query message should be sent to this vlan.
*		
*		Notice: Because some of the normal host doesn't reply general query, so we should
*		send g-s query here instead.
* CALLED BY:
*		Igmp_TimerExp_Proc()
* CALLS:
*		
**************************************************************************/
LONG Igmp_Event_GenQuery_Timeout( MC_group_vlan *p_vlan )
{
	MC_group_vlan *pPrevGroupVlan = NULL;
	igmp_snoop_pkt stData;
	struct igmp_router_entry * Igmp_snoop_router_temp;  /*zgm add router port */

	IGMP_SNP_DEBUG( ( "\r\nEnter Igmp_Event_GenQuery_Timeout. \r\n" ) );

	if( !p_vlan )
	{
		IGMP_SNP_DEBUG("Igmp_Event_GenQuery_Timeout: parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}

	pPrevGroupVlan = p_vlan;
	/* send general query to all ports except the router port */
	Igmp_snoop_router_temp = pPrevGroupVlan->routerlist;
	if ( Igmp_snoop_router_temp != NULL )
	{
		while ( Igmp_snoop_router_temp != NULL )
		{

			/*send general skb*/
			stData.vlan_id = p_vlan->vlan_id;
			stData.groupadd= 0;
			stData.ifindex = Igmp_snoop_router_temp->mroute_ifindex;
			stData.saddr = Igmp_snoop_router_temp->saddr;
			stData.type = IGMP_MSG_GEN_QUERY;

			if ( IGMP_SNOOP_OK != ( Igmp_Snoop_Send_Igmp( &stData ) ) )
			{
				IGMP_SNP_DEBUG( ( "Igmp_Event_GenQuery_Timeout: failed in sending query packet\r\n" ) );
			}

			Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
		}
	}
	else
	{
		/*send general skb*/
		stData.vlan_id= p_vlan->vlan_id;
		stData.groupadd= 0;
		stData.ifindex = 0;
		stData.saddr = 0;
		stData.type = IGMP_MSG_GEN_QUERY;

		if ( IGMP_SNOOP_OK != ( Igmp_Snoop_Send_Igmp( &stData ) ) )
		{
			IGMP_SNP_DEBUG( ( "Igmp_Event_GenQuery_Timeout: failed in sending query packet\r\n" ) );
		} 
	}
	return IGMP_SNOOP_OK;
}

/**************************************************************************
* Igmp_Event_GroupLife_Timeout()
*
* INPUTS:
*		pPkt - igmp_snoop_pkt structure pointer 	
*
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		Igmp_Event_GroupLife_Timeout handles the group life timeout message.
*		A group haven't received query message for a long time, it will be 
*		taken to thought that it was dead. So delete the group.
*
*
* CALLED BY:
*		Igmp_Timer_Exp_Proc()
* CALLS:
*		
* Modify Log:  
*
**************************************************************************/
LONG Igmp_Event_GroupLife_Timeout( MC_group_vlan *p_vlan, MC_group * p_group )
{
	MC_group * pNextGroup = NULL;
	LONG lRet = IGMP_SNOOP_OK;

	IGMP_SNP_DEBUG( ( "\r\nEnter Igmp_Event_GroupLife_Timeout \r\n" ) );

	if ( NULL == p_group )
	{
		IGMP_SNP_DEBUG( " Igmp_Event_GroupLife_Timeout: parameter.\r\n" );
		return IGMP_SNOOP_ERR;
	}

	/* delete group node */
	lRet = igmp_snp_delgroup( p_vlan, p_group, &pNextGroup );
	if( lRet == IGMP_SNOOP_OK )
		p_group = NULL;		/*release seccess, set p_group = NULL*/
	/* if the group is the last group in the vlan, delete vlan node */
	if ( NULL == p_vlan )
	{
		IGMP_SNP_DEBUG( " Igmp_Event_GroupLife_Timeout: vlan node does not exist.\r\n" ); 
		return IGMP_SNOOP_ERR;
	}
	else if ( NULL == p_vlan->firstgroup)
	{
		if( IGMP_SNOOP_OK == igmp_snp_delvlan( p_vlan->vlan_id))
			p_vlan = NULL;
	}
	return lRet;
}
/**************************************************************************
* Igmp_Event_Proxy_Timeout()
*
* INPUTS:
*		pPkt - igmp_snoop_pkt structure pointer 	
*
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		Igmp_Event_Proxy_Timeout handles the group life timeout message.
*		A group haven't received query message for a long time, it will be 
*		taken to thought that it was dead. So delete the group.
*
*
* CALLED BY:
*		Igmp_Timer_Exp_Proc()
* CALLS:
*		
* Modify Log:  
*
**************************************************************************/
LONG Igmp_Event_Proxy_Timeout(MC_group_vlan *p_vlan, MC_group * p_group)
{
	//ULONG unIfIndex;
	igmp_router_entry * Igmp_snoop_router_temp;
	igmp_snoop_pkt stPkt;
	
	//IGMP_SNP_DEBUG( ( "\r\nEnter Igmp_Event_Proxy_Timeout()! \r\n" ) );
	if ( NULL == p_vlan )
	{
		IGMP_SNP_DEBUG("Igmp_Event_Proxy_Timeout: parameter error.\r\n");
		return IGMP_SNOOP_ERR;
	}

	if ( NULL != p_group->portstatelist )                                                                                             /* has member, send report */
	{
		if ( NULL != p_vlan->routerlist )
		{
			Igmp_snoop_router_temp = p_vlan->routerlist;
			while ( Igmp_snoop_router_temp != NULL )
			{
				/*向个个端口发送report报文，可以改为直接构造skb*/
				stPkt.vlan_id= p_group->vid;
				stPkt.groupadd= p_group->MC_ipadd;

				stPkt.ifindex = Igmp_snoop_router_temp->mroute_ifindex;
				stPkt.type = ( p_group->ver_flag == IGMP_SNOOP_NO ) ? IGMP_MSG_REPORT : IGMP_MSG_V1_REPORT ;
				stPkt.saddr = p_group->report_ipadd;

				IGMP_SNP_DEBUG( "Igmp_Event_Proxy_Timeout: send report. 0x%8x %d\r\n",
						p_group->MC_ipadd, stPkt.ifindex );

				if ( IGMP_SNOOP_OK != Igmp_Snoop_Send_Igmp( &stPkt ) )
				{
					IGMP_SNP_DEBUG( ( "Igmp_Event_Proxy_Timeout: send report failed.\r\n" ) );
					/* if failed in send report, doesn't return, and go on */
				}

				Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
			}
		}
	}
	/*delete timer in here is not right but simple */
	else
	{
		p_group->router_reporttimer = 0;
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* Igmp_Event_GroupMember_Timeout()
*
* INPUTS:
*		MC_group_vlan *p_vlan
*		MC_group *p_group
*		ULONG ifindex 		need delete port index
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		Igmp_Event_GroupMember_Timeout handles the IGMP group member timeout
*		message. A group haven't received membership message for the specific time,
*		it will be thought that the port has no member now. So change the port
*		state to NoMember state( 00 ).
*		If the member is the last member of this group, we should start the group
*		life timer. If no report received before the group life timer expired,
*		The group should be deleted.
*
**************************************************************************/
LONG Igmp_Event_GroupMember_Timeout( MC_group_vlan *p_vlan,MC_group *p_group,
							ULONG ifindex  )
{ 
	struct MC_port_state *t_port = NULL;
	igmp_snoop_pkt 	stPkt;
	
	IGMP_SNP_DEBUG("\r\nEnter Igmp_Event_GroupMember_Timeout. vid %d group 0x%.8x If 0x%x\r\n", p_vlan->vlan_id, p_group->MC_ipadd, ifindex);
	if ( !p_group ||!p_vlan)
	{
		IGMP_SNP_DEBUG( " Igmp_Event_Rxmt_Timeout: parameter error.\r\n") ;
		return IGMP_SNOOP_ERR;
	}

	/* if the group membership timer is timeout, delete the port from member list
	*/
	igmp_snp_searchportlist( &( p_group->portstatelist ), ifindex,
						IGMP_PORT_DEL, &t_port );

	/* del L2 L3 MC address */
	stPkt.vlan_id= p_vlan->vlan_id;
	stPkt.ifindex = ifindex;


	stPkt.type = IGMP_ADDR_DEL;
	stPkt.groupadd= p_group->MC_ipadd;

	if ( IGMP_SNOOP_OK != ( igmp_snp_mod_addr ( &stPkt , IGMP_ADDR_DEL ) ) )
	{
		IGMP_SNP_DEBUG("Igmp_Event_GroupMember_Timeout: failed in setting L2 MC table. 0x%x \r\n", stPkt.ifindex);
	}

	if ( NULL == p_group->portstatelist )       /* if no member present, start group life timer */
	{
		/* Add group life timer */
		igmp_snp_addintertimer( igmp_grouplife, &( p_group->lifetimer_id ) );
		IGMP_SNP_DEBUG("Igmp_Event_GroupMember_Timeout: add group life timer.%d\r\n", p_group->lifetimer_id);
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* Igmp_Event_Resp_Timeout()
*
* INPUTS:
*		pPkt - igmp_snoop_pkt structure pointer 	
*
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		Igmp_Event_Resp_Timeout handles the response timeout message.When the 
*		Igmp Snooping switch receives a query packet, it will start the response
*		timer to delay the report packet sending process. So while the reponse 
*		timeout send the report message.
*
*
* CALLED BY:
*		timer chains
* CALLS: 
*		
*
**************************************************************************/
VOID Igmp_Event_Resp_Timeout( struct timer_element *cur)
{
	igmp_snoop_pkt * pPkt = NULL;
	MC_group * pMcGroup = NULL;
	MC_group_vlan *pPrevGroupVlan = NULL;
	ULONG unIfIndex;
	ULONG unRouterIfIndex;
	igmp_snoop_pkt stPkt;
	MC_group_vlan *pMcVlan = NULL;
	igmp_router_entry * Igmp_snoop_router_temp;

	unRouterIfIndex = pPkt->ifindex;

	IGMP_SNP_DEBUG("\r\nEnter Igmp_Event_Resp_Timeout()! \r\n");

	if( NULL == cur )
	{
		IGMP_SNP_DEBUG("Igmp_Event_Resp_Timeout: cur is null\r\n");
		return;
	}
	pPkt = (igmp_snoop_pkt *)cur->data;
	if ( (NULL == pPkt)||(cur->datalen < sizeof(igmp_snoop_pkt)) )
	{
		IGMP_SNP_DEBUG("Igmp_Event_Resp_Timeout: pPkt is null\r\n");
		return;
	}

	/* search vlan */
	pMcVlan = GET_VLAN_POINT_BY_INDEX(first_vlan_idx);
	while ( pMcVlan != NULL )
	{
		if ( pMcVlan->vlan_id == pPkt->vlan_id )
		{
			break;
		}
		else
		{
			pMcVlan = GET_VLAN_POINT_BY_INDEX(pMcVlan->next);
		}
	}

	if (	 pMcVlan ==NULL)
	{
		IGMP_SNP_DEBUG("Igmp_Event_Resp_Timeout: No such vlan node %d.\r\n", pPkt->vlan_id);
		free( pPkt );
		cur->data = NULL;
		cur->datalen = 0;
		return;
	}

	/* if a general query, search all group, reponse*/
	if ( 0 == pPkt->groupadd)                     /* general query response timeout */
	{
		/* go through mc groups, send report */
		pMcGroup = pMcVlan->firstgroup;
		while ( pMcGroup != NULL )
		{
			if ( NULL != pMcGroup->portstatelist )                    /* has member, send report */
			{
				if ( NULL != pMcVlan->routerlist )
				{
					Igmp_snoop_router_temp = pMcVlan->routerlist;
					while ( Igmp_snoop_router_temp != NULL ) 
					{
						if (unRouterIfIndex == Igmp_snoop_router_temp->mroute_ifindex)
						{
							stPkt.vlan_id = pMcGroup->vid;
							stPkt.groupadd= pMcGroup->MC_ipadd;

							stPkt.ifindex = Igmp_snoop_router_temp->mroute_ifindex;
							stPkt.type = ( pMcGroup->ver_flag== IGMP_SNOOP_NO ) ? IGMP_MSG_REPORT : IGMP_MSG_V1_REPORT ;
							stPkt.saddr = pMcGroup->report_ipadd;

							unIfIndex = stPkt.ifindex;

							IGMP_SNP_DEBUG( "Igmp_Event_Resp_Timeout: send report. 0x%8x %d\r\n",
									pMcGroup->MC_ipadd,  unIfIndex);

							if ( IGMP_SNOOP_OK != Igmp_Snoop_Send_Igmp( &stPkt ) )
							{
								IGMP_SNP_DEBUG("Igmp_Event_Resp_Timeout: send report failed.\r\n");
								/* if failed in send report, doesn't return, and go on */
							}
						}
						Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
					}
				}
			}
			else
			{
				/* add group life timer */
				if ( pMcGroup->lifetimer_id == 0 )
				{
					/* Add group life timer */
					igmp_snp_addintertimer( igmp_grouplife, &( pMcGroup->lifetimer_id ) );
					IGMP_SNP_DEBUG("Igmp_Event_Resp_Timeout: start group life timer. time:%d\r\n", pMcGroup->lifetimer_id);
				}
			}
			pMcGroup = pMcGroup->next;
		}
	}
	else		/* g-s query response timeout */
	/* search group */
	{
		if ( IGMP_SNOOP_OK != ( igmp_searchvlangroup( pPkt->vlan_id, pPkt->groupadd, &pMcGroup, &pPrevGroupVlan ) ) )
		{
			IGMP_SNP_DEBUG("Igmp_Event_Resp_Timeout: search failed\r\n");
			free( pPkt );
			cur->data = NULL;
			cur->datalen = 0;
			return;
		}
		if ( NULL == pMcGroup )
		{
			IGMP_SNP_DEBUG( " Igmp_Event_Resp_Timeout: group:%d does not exist.\r\n", pPkt->groupadd)
			free( pPkt );
			cur->data = NULL;
			cur->datalen = 0;
			return;
		}
		/* send reports */
		if ( NULL != pMcGroup->portstatelist )                                                                                             /* has member, send report */
		{
			if ( NULL != pMcVlan->routerlist )
			{
			Igmp_snoop_router_temp = pMcVlan->routerlist;
			while ( Igmp_snoop_router_temp != NULL )
			{
				if (unRouterIfIndex == Igmp_snoop_router_temp->mroute_ifindex)
				{
					stPkt.vlan_id = pMcGroup->vid;
					stPkt.groupadd= pMcGroup->MC_ipadd;
					stPkt.ifindex = Igmp_snoop_router_temp->mroute_ifindex;
					stPkt.type = ( pMcGroup->ver_flag== IGMP_SNOOP_NO ) ? IGMP_MSG_REPORT : IGMP_MSG_V1_REPORT ;
					stPkt.saddr = pMcGroup->report_ipadd;
					IGMP_SNP_DEBUG("Igmp_Event_Resp_Timeout: send report. 0x%8x  port 0x%x\r\n",
							pMcGroup->MC_ipadd, stPkt.ifindex);

					if ( IGMP_SNOOP_OK != Igmp_Snoop_Send_Igmp( &stPkt ) )
					{
						IGMP_SNP_DEBUG("Igmp_Event_Resp_Timeout: send report failed.\r\n");
						/* if failed in send report, doesn't return, and go on */
					}
				}
				Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
			}
			}
		}
		else
		{
			/* add group life timer */
			if ( pMcGroup->lifetimer_id == 0 )
			{
				/* Add group life timer */
				igmp_snp_addintertimer( igmp_grouplife, &( pMcGroup->lifetimer_id ) );
				IGMP_SNP_DEBUG( "Igmp_Event_Resp_Timeout: start group life timer. time:%d\r\n", pMcGroup->lifetimer_id);
			}
		}
		/* No need to delete timer */
	}
	free( pPkt );
	cur->data = NULL;
	cur->datalen = 0;
	return;
}


/**************************************************************************
* Igmp_Event_Rxmt_Timeout()
*
* INPUTS:
*		 struct timer_element *cur 	timer 	
*
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK -  on sucess
*		IGMP_SNOOP_ERR - not found
*
* DESCRIPTION:
*		Igmp_Event_Rxmt_Timeout handles the retransmit query response interval 
*		timeout	message. when report was received from the port, the timer should
*		be deleted. Notice: it should retransmit g-s query for Robustnss variable
*		times.
* 
*
**************************************************************************/
VOID Igmp_Event_Rxmt_Timeout( struct timer_element *cur )
{
	 igmp_snoop_pkt * pPkt = NULL;
	MC_group * pMcGroup = NULL;
	MC_group_vlan *pPrevGroupVlan = NULL;
	LONG lRet;
	ULONG ulTimerId = 0;
	struct MC_port_state *pstPort = NULL;
	igmp_snoop_pkt stPkt;
	igmp_router_entry * Igmp_snoop_router_temp;

	IGMP_SNP_DEBUG("\r\nEnter Igmp_Event_Rxmt_Timeout. \r\n");

	if( NULL == cur )
	{
		IGMP_SNP_DEBUG("Igmp_Event_Rxmt_Timeout: cur is null\r\n");
		return;
	}
	pPkt = (igmp_snoop_pkt *)cur->data;
	if ( (NULL == pPkt)||(cur->datalen < sizeof(igmp_snoop_pkt)) )
	{
		IGMP_SNP_DEBUG("Igmp_Event_Rxmt_Timeout: pPkt is null\r\n");
		return;
	}
	/* search group */
	if ( IGMP_SNOOP_OK != ( lRet = igmp_searchvlangroup( pPkt->vlan_id, pPkt->groupadd, &pMcGroup, &pPrevGroupVlan ) ) )
	{
		IGMP_SNP_DEBUG("Igmp_Recv_Query: search failed\r\n");
		free( pPkt );
		cur->data = NULL;
		cur->datalen = 0;
		return;
	}
	if ( NULL == pMcGroup )
	{
		IGMP_SNP_DEBUG(" Igmp_Event_Rxmt_Timeout: group:0x%x does not exist.\r\n", pPkt->groupadd)
		free( pPkt );
		cur->data = NULL;
		cur->datalen = 0;
		return;
	}

	/* if port state is not check member, no need to send g-s query or do anything */
	igmp_snp_searchportlist( &( pMcGroup->portstatelist ), pPkt->ifindex,IGMP_PORT_QUERY, &pstPort );
	if ( pstPort == NULL )
	{
		free( pPkt );
		cur->data = NULL;
		cur->datalen = 0;
		return;
	}

	if ( pstPort->state != IGMP_SNP_CHECK_MEMBER )
	{
		free( pPkt );
		cur->data = NULL;
		cur->datalen = 0;
		return;
	}

	if ( pPkt->retranscnt != 0 )
	/* if retransmit times >0 , send g-s query, reset rxmt timer, rxmtcount-- */
	{
		stPkt.vlan_id= pPkt->vlan_id;
		stPkt.groupadd= pPkt->groupadd;
		stPkt.type = IGMP_MSG_GS_QUERY;
		stPkt.ifindex = pPkt->ifindex;
		stPkt.saddr = pPrevGroupVlan->saddr;
		if ( IGMP_SNOOP_OK != ( lRet = Igmp_Snoop_Send_Igmp( &stPkt ) ) )
		{
			IGMP_SNP_DEBUG(" Igmp_Event_Rxmt_Timeout: send g-s query failed.\r\n")
		}

		pPkt->retranscnt --;
		if ( pPkt->retranscnt )
		{	/*continue timer*/
			struct timer_element *new_timer = NULL;
			new_timer = create_timer(TIMER_TYPE_NOLOOP, TIMER_PRIORI_NORMAL,
									igmp_rxmt_interval,
									(void *)Igmp_Event_Rxmt_Timeout,
									(void *)pPkt,sizeof(igmp_snoop_pkt));
			if(  NULL == new_timer )
			{
				IGMP_SNP_DEBUG("Igmp_recv_query:create timer failed.\r\n");
				return;
			}
			if( -1 == add_timer(&igmp_timer_list, new_timer,&(ulTimerId)) )
			{
				IGMP_SNP_DEBUG("Igmp_recv_query:add timer failed.\r\n");
				return;
			}
		}
	}

	/* if rxmtcount == 0 or send g-s query failed or creat timer failed,
	modify the checking member state port to no member state  */
	if ( ( 0 == pPkt->retranscnt ) || ( lRet != IGMP_SNOOP_OK ) || ( 0 == ulTimerId ) )
	{
		if ( pstPort->state == IGMP_SNP_CHECK_MEMBER )
		{
			stPkt.vlan_id= pPkt->vlan_id;
			stPkt.groupadd= pPkt->groupadd;
			stPkt.type = IGMP_ADDR_DEL;
			stPkt.ifindex = pPkt->ifindex;
			
			if ( IGMP_SNOOP_OK != ( lRet = igmp_snp_mod_addr( &stPkt , IGMP_ADDR_DEL ) ) )
			{
				IGMP_SNP_DEBUG("Igmp_Event_Rxmt_Timeout: failed in setting L2 MC table. 0x%x \r\n", stPkt.ifindex);
			}
			/* delete port from port list */
			igmp_snp_searchportlist( &( pMcGroup->portstatelist), pPkt->ifindex,IGMP_PORT_DEL, &pstPort );
			/* if find router port,send leave to its upstream switch ,else flood leave in vlan,only when it is last member .add by wangjingqi ,2003/10/07 */
			/*add begin by wangjingqi */
			if ( NULL == pMcGroup->portstatelist )
			{
				if ( pPrevGroupVlan->routerlist != NULL )
				{
					Igmp_snoop_router_temp = pPrevGroupVlan->routerlist;
					while ( Igmp_snoop_router_temp != NULL )
					{
						igmp_snp_routerleave( pPkt->vlan_id, Igmp_snoop_router_temp->mroute_ifindex, pPkt->groupadd, pPkt->saddr );
						Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
					}
				}
				else
				{
					ULONG unIfIndex;
					ULONG ulportstatus;
					//IFM_PORTONVLANSTART( unIfIndex, ( USHORT ) pPkt->vlan_id )
					if(IGMP_SNOOP_OK !=igmp_getifindex_byvlanid(pPkt->vlan_id,&unIfIndex) )
					{
						IGMP_SNP_DEBUG("igmp_snp_flood: can not get ifindex by vlan_id.\r\n");
						return;
					}
					while(0 != unIfIndex )
					{
						if ( unIfIndex == pPkt->ifindex )
						{
							unIfIndex = igmp_get_nextifindex_byifindex(pPkt->vlan_id,unIfIndex);
							continue;
						}
						/* send report here */
						igmp_getifstatus( unIfIndex, &ulportstatus );
						if ( ulportstatus == 1 )
						{
							igmp_snp_routerleave( pPkt->vlan_id, unIfIndex, pPkt->groupadd, pPkt->saddr );
						}
						unIfIndex = igmp_get_nextifindex_byifindex(pPkt->vlan_id,unIfIndex);
						//IFM_PORTONVLANEND
					}
				}
				igmp_snp_addintertimer( igmp_grouplife, &( pMcGroup->lifetimer_id ) );
				IGMP_SNP_DEBUG(" Igmp_Event_Rxmt_Timeout: add group life timer.%d\r\n", pMcGroup->lifetimer_id);
			}
			/* start Group life timer */
		}
		if( 0 != ulTimerId )
			del_timer(&igmp_timer_list, ulTimerId );
		free( pPkt );
		cur->data = NULL;
		cur->datalen = 0;
	}
	igmp_debug_print_groupvlan(pPrevGroupVlan);
	return;
}

/**********************************************************************************
*igmp_enable_init()
*INPUTS:
*none
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	enable igmp snooping init function
***********************************************************************************/
INT igmp_enable_init(void)
{
	INT ret;	
	igmp_snoop_pkt t_pkt;
	
	t_pkt.vlan_id = 0;
	t_pkt.groupadd = 0;
	t_pkt.ifindex = 0;
	t_pkt.type = IGMP_SYS_SET_INIT;		/*init*/

	if( IGMP_SNOOP_OK != igmp_snp_mod_addr(&t_pkt,IGMP_SYS_SET))
	{
		IGMP_SNP_DEBUG("Igmp_snp_mod_addr:failed in set port pfm.\r\n");
		goto error;
	}

	if( IGMP_SNOOP_OK != init_igmp_snp_timer())
	{
		IGMP_SNP_DEBUG("Igmp_timer_init:fail in set global timer.\r\n");
		goto error;
	}

	return IGMP_SNOOP_OK;
error:
	return IGMP_SNOOP_ERR;
}


/**********************************************************************************
*igmp_snp_stop()
*INPUTS:
*none
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	stop IGMP SNOOPING，release data
*
***********************************************************************************/
INT igmp_snp_stop(VOID)
{			
	INT tmp = 0;
	INT ret = IGMP_SNOOP_OK;
	MC_group_vlan *tvlan = NULL;
	igmp_snoop_pkt t_pkt;
	
	if(!IGMP_SNP_ISENABLE())
	{
		IGMP_SNP_DEBUG("Fail to stop IGMP snooping:task is not started.\r\n");
		return IGMP_SNOOP_OK;
	}
	igmp_snoop_enable = IGMP_SNOOP_NO;
	
	t_pkt.vlan_id = 0;
	t_pkt.groupadd = 0;
	t_pkt.ifindex = 0;
	t_pkt.type = IGMP_SYS_SET_STOP;	

	if( IGMP_SNOOP_OK != igmp_snp_mod_addr(&t_pkt,IGMP_SYS_SET))
	{
		IGMP_SNP_DEBUG("Igmp_snp_mod_addr:failed in set port pfm.\r\n");
		goto error;
	}

	/*delete timer*/
	if(igmp_timer_list.cnt)
	{
		if( IGMP_SNOOP_OK != del_all_timer(&igmp_timer_list))
		{
			IGMP_SNP_DEBUG("Igmp_timer_delete:fail in delete global timer.\r\n");
			goto error;
		}
	}

	/*删除消息队列*/
	/*暂时未添加*/
	
	/*delete mc group*/
	tvlan = mcgroup_vlan_queue[tmp];
	while(NULL != tvlan)
	{
		if(NULL != tvlan )
			igmp_snp_delvlan(tvlan->vlan_id);
		mcgroup_vlan_queue[tmp] = NULL;
		tmp++;
		tvlan = mcgroup_vlan_queue[tmp];
	}
	first_vlan_idx = -1;
	igmp_snoop_debug = IGMP_SNOOP_NO;
	return IGMP_SNOOP_OK;
error:
	igmp_snoop_enable = IGMP_SNOOP_YES;
	return IGMP_SNOOP_ERR;
}


LONG igmp_recv_pktdata_proc(struct igmp_skb *msg_skb )
{
	USHORT	*tmp;
	struct iphdr * pIphd = NULL;
	ULONG usIfIndex = 0;
	ULONG ulGroup;
	MC_group * pMcGroup = NULL;
	MC_group_vlan *pPrevGroupVlan = NULL;
	LONG lRet = IGMP_SNOOP_OK;
	LONG lVid = *((LONG *)(msg_skb->buf) +1);
	if( !msg_skb )
	{
		IGMP_SNP_DEBUG("igmp_recv_pktdata_proc: parameter is NULL.\r\n");
		return IGMP_SNOOP_ERR;
	}
	tmp = (USHORT *)(msg_skb->buf + 8 + 12 );
	if( 0x8100 == *tmp )
		pIphd = (struct iphdr *)(msg_skb->buf + 8 + 18);
	else
		pIphd = (struct iphdr *)(msg_skb->buf + 8 + 14);
	ulGroup = pIphd->daddr;

	if ( pIphd->ttl < 1 )
	{
		IGMP_SNP_DEBUG("igmp_recv_pktdata_proc: IP header ttl:%d is less than 1.\r\n", pIphd->ttl);
		return IGMP_SNOOP_ERR;
	}
	/* check header checksum */
	if ( inet_cksum( ( USHORT * ) pIphd, ( ULONG ) ( pIphd->ihl ) << 2 ) )
	{
		IGMP_SNP_DEBUG("igmp_recv_pktdata_proc: IP header checksum error\r\n");
		return IGMP_SNOOP_ERR;
	}

	lRet = igmp_searchvlangroup( lVid, ulGroup, &pMcGroup, &pPrevGroupVlan );
	if ( IGMP_SNOOP_OK != lRet )
	{
		IGMP_SNP_DEBUG("igmp_recv_pktdata_proc: search failed\r\n");
		return IGMP_SNOOP_ERR;
	}
	if ( NULL != pMcGroup )
	{
		return IGMP_SNOOP_OK;
	}
	else
	{
		if ( IGMP_SNOOP_OK != igmp_creat_group_node( lVid, ulGroup, usIfIndex, &pMcGroup, &pPrevGroupVlan ) )
		{
			IGMP_SNP_DEBUG("Igmp_Recv_Report: create group error. pVlan 0x%x\r\n", pPrevGroupVlan);
			return IGMP_SNOOP_ERR;
		}
	}
	
	{
	igmp_snoop_pkt stPkt ;

	stPkt.vlan_id = lVid;
	stPkt.group_id = 0;
	stPkt.groupadd = ulGroup;
	stPkt.ifindex = usIfIndex;
	stPkt.type = IGMP_ADDR_ADD;

	lRet = igmp_snp_mod_addr( &stPkt, IGMP_ADDR_ADD );
	if ( IGMP_SNOOP_OK != lRet )
	{
		IGMP_SNP_DEBUG("Igmp_Recv_Report: failed in add address.\r\n");
		return IGMP_SNOOP_ERR;
	}
	}

	/* add group member timer */
	igmp_snp_addintertimer( pMcGroup->lifetime, &( pMcGroup->lifetimer_id ) );
	return IGMP_SNOOP_OK;
}


/**********************************************************************************
*create_recvskb_thread()
*INPUTS:
*struct igmp_skb *msg_skb 	receive message pointer
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	init igmp_snoop receive messsage distribute function
***********************************************************************************/
INT igmp_message_proc(struct igmp_skb *msg_skb)
{
	if( !IGMP_SNP_ISENABLE() )
	{
		IGMP_SNP_DEBUG("IGMP Snooping is switch-off.\r\n");
		return;
	}

	if( NULL == msg_skb)
	{
		IGMP_SNP_DEBUG("igmp_message_proc:parameter is NULL.\r\n");
		return;
	}
	
	switch(msg_skb->nlh.nlmsg_type)
	{
		case IGMP_SNP_TYPE_DEVICE_EVENT:
			{
			ULONG event;
			dev_notify_msg *dev_info = NULL;

			if(msg_skb->nlh.nlmsg_len < (sizeof(dev_notify_msg)+sizeof(struct nlmsghdr)))
			{
				IGMP_SNP_DEBUG("igmp_skb_proc:msg_skb error.\r\n");
				return IGMP_SNOOP_ERR;
			}
			event = *(ULONG *)(msg_skb->buf);
			dev_info = (dev_notify_msg *)(msg_skb->buf);
			igmp_snp_device_event(event,dev_info);
			}
			break;
		case IGMP_SNP_TYPE_PACKET_MSG:
			switch(msg_skb->nlh.nlmsg_flags)
			{
				case IGMP_SNP_FLAG_PKT_UNKNOWN:
					igmp_recv_pktdata_proc(msg_skb);
					break;
				case IGMP_SNP_FLAG_PKT_IGMP:
					igmp_skb_proc(msg_skb);
					break;
				default:
					IGMP_SNP_DEBUG("igmp_skb_proc:unknown msg flag.\r\n");
					break;
			}
			break;
		default:
			IGMP_SNP_DEBUG("igmp_skb_proc:unknown msg type.\r\n");
			break;
	}
	return IGMP_SNOOP_OK;
}


INT igmp_snp_pim_msghandle( ULONG ulGoup, ULONG ulIfindex, LONG lVid, ULONG ulSrcAddr )
{
	LONG result = 0;
	ULONG trunkIfIndex = 0;      
	igmp_routerport *pRouter = NULL;
	MC_group *pMcGroup = NULL;
	MC_group_vlan *pPrevGroupVlan = NULL;
	igmp_router_entry * Igmp_snoop_router_temp;
	igmp_router_entry ** Igmp_snoop_router_pre;


	IGMP_SNP_DEBUG("igmp_snp_pim_msghandle: Enter\r\n")
	/* search vlan-group list, try to find a node */
	if ( IGMP_SNOOP_OK != ( igmp_searchvlangroup( lVid, ulGoup, &pMcGroup, &pPrevGroupVlan ) ) )
	{
		IGMP_SNP_DEBUG("igmp_snp_pim_msghandle: search failed\r\n");
		return IGMP_SNOOP_ERR;
	}

	/*check if source port is a member of trunk*/
	result = IFM_PhyIsMerged2TrunkApi(lVid,ulIfindex,&trunkIfIndex );
	if ( result == IGMP_SNOOP_OK ) /*trunk member*/
	{
		DEBUG_OUT("igmp_snp_pim_msghandle:trunkIfindex:0x%x\r\n",trunkIfIndex);
		ulIfindex = trunkIfIndex;
	}
	else
	{
		ulIfindex = ulIfindex;
	}

	if  ( NULL == pPrevGroupVlan ) 
	{
		return IGMP_SNOOP_OK;
	}

	/* If a query message came from a multicasting router(it's SIP not 0.0.0.0),
	we should change the multicast router port to this port whether it is a 
	designated port */
 	igmp_snoop_searchrouterlist(lVid,ulIfindex,IGMP_PORT_QUERY,&pRouter);

	if ( ( ulSrcAddr != 0 ) && ( pRouter == NULL ) )       /* no router port configured */
	{
		Igmp_snoop_router_pre = &( pPrevGroupVlan->routerlist );

		while ( *Igmp_snoop_router_pre != NULL )
		{
			if ( ( *Igmp_snoop_router_pre ) ->mroute_ifindex == ulIfindex )
			{
				break;
			}
			Igmp_snoop_router_pre = &( ( *Igmp_snoop_router_pre ) ->next );
		}
		if ( *Igmp_snoop_router_pre == NULL )
		{
			Igmp_snoop_router_temp = ( igmp_router_entry * )malloc(sizeof( igmp_router_entry ));
			if ( NULL == Igmp_snoop_router_temp )
			{
				IGMP_SNP_DEBUG("igmp_snp_pim_msghandle: alloc mem fail. \r\n");
				return IGMP_SNOOP_ERR;
			}
			memset( Igmp_snoop_router_temp,0, sizeof( igmp_router_entry ) );
			Igmp_snoop_router_temp->saddr = ulSrcAddr;
			Igmp_snoop_router_temp->mroute_ifindex = ulIfindex;
			igmp_snp_addintertimer( igmp_router_timeout, &( Igmp_snoop_router_temp->timer_id ) );
			*Igmp_snoop_router_pre = Igmp_snoop_router_temp;
			IGMP_SNP_DEBUG( "\r\nigmp_snp_pim_msghandle:  add router port vlan =%d,ifindex=%d\r\n", lVid, ulIfindex);
		}
		else
		{
			( *Igmp_snoop_router_pre ) ->saddr = ulSrcAddr;
			igmp_snp_addintertimer( igmp_router_timeout, &( ( *Igmp_snoop_router_pre ) ->timer_id ) );
		}
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* pim_recv_msg()
*
*When igmp_snoop rev pim hello packet add route port 
**************************************************************************/
LONG pim_recv_msg(struct igmp_skb *msg_skb)
{
	struct igmp_info *msg_info = NULL;
	struct iphdr	*pIphd = NULL;
	pim_header_t *pPimphd = NULL;
	ULONG	ulPimLen;
	ULONG lRet = IGMP_SNOOP_OK;
	LONG	lVid = 0;
	ULONG ifindex = 0;
	ULONG ulGroup;
	ULONG ulSrcaddr;

	pIphd = (struct iphdr *)(msg_skb->buf + 8 + 14);

	ulSrcaddr = pIphd->saddr;
	/*得到PIM头*/
	pPimphd = ( struct pim_header * ) ( ( UCHAR * ) pIphd + sizeof(   struct iphdr ) );

	IGMP_SNP_DEBUG( "\r\npim_recv_msg: ver 0x%x,type 0x%x, cksum %d\r\n",
	      				pPimphd->pim_vers, pPimphd->pim_type, pPimphd->pim_cksum);

	/* check header checksum */
	if ( inet_cksum( ( USHORT * ) pIphd, ( ULONG ) ( pIphd->ihl ) << 2 ) )
	{
		IGMP_SNP_DEBUG( ( "pim_recv_msg: IP header checksum error\r\n" ) );
		return IGMP_SNOOP_ERR;
	}
	/*check pim checksum*/    
	//ulPimLen = (ULONG)pIphd->tot_len - (ULONG) (( pIphd->ihl ) << 2);
	ulPimLen = sizeof(struct pim_header );
	IGMP_SNP_DEBUG("IP header len: %d Pim data len: %d\r\n",( pIphd->ihl << 2 ), ulPimLen);

	if ( 0 != ( lRet = inet_cksum( ( USHORT * ) pPimphd, ( ULONG ) ulPimLen ) ) )                                                                                                   /* check IGMP packet */
	{
		IGMP_SNP_DEBUG("pim_recv_msg: Pim packet checksum error. Cksum: %d\r\n", lRet);
		return IGMP_SNOOP_ERR;
	}
	/* check port/trunk-vlan relation */
	ifindex = *((ULONG *)(msg_skb->buf));
	lVid = *((LONG *)(msg_skb->buf) +1);
	/* distribute message */
	switch ( pPimphd->pim_type )
	{
		/* Pim Hello  packet process */
		case 0x0:          /*PIM_HELLO =0*/
			if ( pPimphd->pim_vers == 1 )
			{
				ulGroup = 0;
			}
			else if ( pPimphd->pim_vers == 2 )
			{
				ulGroup = 0;
			}
			else
			{
				return IGMP_SNOOP_ERR;
			}
			igmp_snp_pim_msghandle( ulGroup, ifindex, lVid, ulSrcaddr );
			break;
		default:
			break;
	}
	return IGMP_SNOOP_OK;
}


/**********************************************************************************
*igmp_recv_report()
*INPUTS:
*	ULONG usIfIndex	ULONG ulGroup	LONG lVid		ULONG ulType	struct igmp_info *sk_info
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	IGMP snoop report packet handle
*
***********************************************************************************/
LONG igmp_recv_report( ULONG usIfIndex, ULONG ulGroup, LONG lVid, 
								ULONG ulType, struct igmp_info *sk_info)
{
	ULONG group_addr = htonl(ulGroup);
	LONG result = 0;
	ULONG trunkIfIndex = 0;
	ULONG ulIfIndex = 0;
	MC_group * pMcGroup = NULL;
	MC_group_vlan *pPrevGroupVlan = NULL;
	LONG lRet = IGMP_SNOOP_OK;
	struct MC_port_state *pstPort = NULL;
	struct iphdr * pIphd = NULL;
	igmp_router_entry * Igmp_snoop_router_temp;
	igmp_reporter_port *pReporter = NULL;
	member_port_list *pMemberPort = NULL;
	
	pIphd = sk_info->ip_hdr;
	IGMP_SNP_DEBUG("\r\nEnter igmp_recv_report: vid %d group %u.%u.%u.%u if 0x%x\r\n", lVid, NIPQUAD(group_addr), usIfIndex);

	DEBUG_OUT("Receive report packet:\tvlan_id:%d\tgroup:%u.%u.%u.%u\ttype:%d\tifindex:0x%x\r\n",
			lVid,NIPQUAD(group_addr),ulType,usIfIndex);
  	/* validate Group address */
	if ( IGMP_SNOOP_OK != ( lRet = igmp_snp_addr_check( ulGroup ) ) )
	{
		IGMP_SNP_DEBUG( "igmp_recv_report: group address invalid. 0x%-8x \r\n", ulGroup );
		return IGMP_SNOOP_OK;
	}

	if ( IGMP_SNOOP_OK != ( lRet = igmp_searchvlangroup( lVid, ulGroup, &pMcGroup, &pPrevGroupVlan ) ) )
	{
		IGMP_SNP_DEBUG( "igmp_recv_report: search failed\r\n" );
		return IGMP_SNOOP_ERR;
	}
	/*check if the report port is a member of trunk */
	result = IFM_PhyIsMerged2TrunkApi( lVid,usIfIndex,&trunkIfIndex );
	if ( result == IGMP_SNOOP_OK ) /*trunk member*/
	{
		DEBUG_OUT("igmp_recv_report:trunkIfindex:0x%x\r\n",trunkIfIndex);
		ulIfIndex = trunkIfIndex;
	}
	else
	{
		ulIfIndex = usIfIndex;
	}
	/* if report recv'd from the router port in this vlan, it is a invalid report, discard it */

	if ( NULL == pMcGroup )
	{
		DEBUG_OUT("McGroup == NULL:Create McGroup and GroupVlan.\r\n");
		if ( IGMP_SNOOP_OK != igmp_creat_group_node( lVid, ulGroup, ulIfIndex, &pMcGroup, &pPrevGroupVlan ) )
		{
			IGMP_SNP_DEBUG("igmp_recv_report: create group error. pVlan 0x%x\r\n", pPrevGroupVlan);
			return IGMP_SNOOP_ERR;
		}
	}

	/*zgm add for if the first group report ,forward report to all router port*/
	if ( pMcGroup ->portstatelist == NULL )
	{
		DEBUG_OUT("Portstatelist == NULL:Send report to router port.\r\n");
		if ( pPrevGroupVlan ->routerlist != NULL )
		{
			Igmp_snoop_router_temp = pPrevGroupVlan ->routerlist;
			while ( Igmp_snoop_router_temp != NULL )
			{
				DEBUG_OUT("Send report to router port:\tvlan:%d\tgroup:%u.%u.%u.%u\tifindex:0x%x\r\n",
							lVid,NIPQUAD(group_addr),Igmp_snoop_router_temp->mroute_ifindex);
				Igmp_Snoop_RouterReport( lVid, Igmp_snoop_router_temp->mroute_ifindex, ulGroup, sk_info );
				Igmp_snoop_router_temp = Igmp_snoop_router_temp->next;
			}
		}
		else
		{
			ULONG unIfIndex;
			ULONG ulportstatus;

			if(IGMP_SNOOP_OK !=igmp_getifindex_byvlanid(lVid,&unIfIndex) )
			{
				IGMP_SNP_DEBUG("igmp_snp_flood: can not get ifindex by vlan_id.\r\n");
				return IGMP_SNOOP_ERR;
			}
			while(0 != unIfIndex)
			{
				if ( unIfIndex == ulIfIndex )   
				{
					unIfIndex = igmp_get_nextifindex_byifindex(lVid,unIfIndex);
					continue;
				}
				/* send report here */
				igmp_getifstatus( unIfIndex, &ulportstatus );
				if ( ulportstatus == 1 )
				{
					DEBUG_OUT("Send report to router port:\tvlan:%d\tgroup:%u.%u.%u.%u\tifindex:0x%x\r\n",
							lVid,NIPQUAD(group_addr),unIfIndex);
					Igmp_Snoop_RouterReport( lVid, unIfIndex, ulGroup, sk_info);
				}
				unIfIndex = igmp_get_nextifindex_byifindex(lVid,unIfIndex);
			}
			//IFM_PORTONVLANEND
		}
		igmp_snp_addintertimer( 10, &( pMcGroup->router_reporttimer ) );
	}



	/*search the member port list*/
	if ( IGMP_SNOOP_OK != igmp_snp_searchreporterlist( lVid, 0, IGMP_PORT_QUERY, &pReporter ) )
	{
		IGMP_SNP_DEBUG("igmp_recv_report: search reporter list failed\r\n");
		return IGMP_SNOOP_ERR;
	}

	if ( NULL != pReporter ) 	/*There is some member port in vlan*/
	{       
		pMemberPort = pReporter->portlist;

		while ( NULL != pMemberPort )
		{
			if ( ulIfIndex != pMemberPort->ifindex ) 	/*There is a reporter port in this vlan,add this port in outport list*/
			{
				IGMP_SNP_DEBUG( "igmp_recv_report: reporter port = 0x%x\r\n", pMemberPort->ifindex);                   
				/* search portlist, if not found create one */
				lRet = igmp_snp_searchportlist( &( pMcGroup->portstatelist ), pMemberPort->ifindex, \
				                      IGMP_PORT_ADD, &pstPort );
				if ( ( IGMP_SNOOP_OK != lRet ) || ( NULL == pstPort ) )
				{
					IGMP_SNP_DEBUG("Igmp_Snoop_Add_Reporter: search port failed. 0x%x\r\n", pstPort);
					return IGMP_SNOOP_ERR;
				}                   
				if ( IGMP_SNP_GROUP_NOMEMBER == pstPort->state )
				{
					igmp_snoop_pkt stPkt ;

					stPkt.vlan_id = lVid;
					stPkt.group_id=0;
					stPkt.groupadd= ulGroup;
					stPkt.ifindex = pMemberPort->ifindex;
					stPkt.type = IGMP_ADDR_ADD;                  
					if ( IGMP_SNOOP_OK != ( lRet = igmp_snp_mod_addr( &stPkt, IGMP_ADDR_ADD ) ) )
					{
						IGMP_SNP_DEBUG( "igmp_recv_report: failed in add address.\r\n");
						return IGMP_SNOOP_ERR;
					}
				}
				/* delete group life timer*/
				IGMP_SNP_DEL_INTERTIMER( pMcGroup->lifetimer_id );

				/* Set port state */
				if ( IGMP_SNP_V1_MEMBER != pstPort->state )
					pstPort->state = IGMP_SNP_HAS_MEMBER;

			}
			pMemberPort = pMemberPort->next;
		}
	}

	/* begin handle report message. */
	/* The difference between v1 and v2 report process is that :
	1. we need to change the port state to v1 member state whether the previous state is.
	2. we should add a v1 host timer to this port, and also the group member timer should 
	be added either.
	*/
	/* search portlist, if not found create one */
	lRet = igmp_snp_searchportlist( &( pMcGroup->portstatelist ), ulIfIndex, \
	                    IGMP_PORT_ADD, &pstPort );
	if ( ( IGMP_SNOOP_OK != lRet ) || ( NULL == pstPort ) )
	{
		IGMP_SNP_DEBUG("igmp_recv_report: search port failed. 0x%x\r\n", pstPort);
		return IGMP_SNOOP_ERR;
	}

	/* Add L2 L3 mc address to hardware */
	if ( IGMP_SNP_GROUP_NOMEMBER == pstPort->state )
	{
		igmp_snoop_pkt stPkt ;

		stPkt.vlan_id = lVid;
		stPkt.group_id=0;
		stPkt.groupadd= ulGroup;
		stPkt.ifindex = ulIfIndex;
		stPkt.type = IGMP_ADDR_ADD;

		if ( IGMP_SNOOP_OK != ( lRet = igmp_snp_mod_addr( &stPkt, IGMP_ADDR_ADD ) ) )
		{
			IGMP_SNP_DEBUG("igmp_recv_report: failed in add address.\r\n");
			return IGMP_SNOOP_ERR;
		}
	}

	/* Set port state */
	if ( IGMP_SNP_V1_MEMBER != pstPort->state )
		pstPort->state = IGMP_SNP_HAS_MEMBER;

	/* delete group life timer*/
	IGMP_SNP_DEL_INTERTIMER( pMcGroup->lifetimer_id );
	/*is report port*/
	if ( Igmp_Snoop_IF_Recv_Is_Report_Discard( lVid, ulIfIndex ) == IGMP_SNOOP_ERR )
	{
		IGMP_SNP_DEBUG( "\r\n This is a reporter port. \r\n");  
		return IGMP_SNOOP_ERR;
	}
	/* add group member timer */
	igmp_snp_addintertimer( igmp_robust_variable * igmp_query_interval
	               	 + igmp_resp_interval, &( pstPort->membertimer_id ) );
	IGMP_SNP_DEBUG("igmp_recv_report: start group member timer. 0x%x\r\n", pstPort->membertimer_id);

	/* if this is a v1 report, do some extra process */
	if ( IGMP_V1_MEMSHIP_REPORT == ulType )
	{
		/* change member state */
		pstPort->state = IGMP_SNP_V1_MEMBER;
		/* Set v1 host timer */
		igmp_snp_addintertimer( IGMP_V1_ROUTER_PRESENT_TIMEOUT, &( pstPort->hosttimer_id ) );
	}
	pIphd = sk_info->ip_hdr;
	pMcGroup->report_ipadd= pIphd->saddr;
	igmp_debug_print_groupvlan(pPrevGroupVlan);
	IGMP_SNP_DEBUG( "Out igmp_recv_report.\r\n");
	return IGMP_SNOOP_OK;
}


/**********************************************************************************
*igmp_recv_query()
*INPUTS:
*	ULONG ifindex,	USHORT maxresptime,  ULONG group
*	LONG vlan_id,  struct igmp_skb *msg_skb
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	IGMP snoop query packet handdle
*
***********************************************************************************/
INT igmp_recv_query(ULONG ifindex, USHORT maxresptime, ULONG group,
					LONG vlan_id, struct igmp_info *sk_info)
{
	LONG result = 0;
	ULONG t_ifindex = 0;
	ULONG trunkifindex = 0;
	struct iphdr *ip_hdr = NULL;
	igmp_routerport *t_router = NULL;
	MC_group *t_mcgroup = NULL;
	MC_group_vlan *t_mcgroupvlan = NULL;
	igmp_router_entry *igmp_snp_router_temp;
	igmp_router_entry **igmp_snp_router_pre;
	
	IGMP_SNP_DEBUG("\r\nEnter igmp_recv_query:vlan_id %d group %u.%u.%u.%u maxresptime 0x%x if 0x%x\r\n",
			vlan_id,NIPQUAD(group),maxresptime,ifindex);

	DEBUG_OUT("Receive query packet:\tvlan_id:%d\tgroup:%u.%u.%u.%u\tifindex:0x%x\r\n",
				vlan_id,NIPQUAD(group),ifindex);
	/*mc group address check*/
	if( 0 != group)
	{
		if( IGMP_SNOOP_OK != igmp_snp_addr_check(group))
		{
			IGMP_SNP_DEBUG("Igmp_recv_query: group address invalid.0x%-8x\r\n",group);
			return IGMP_SNOOP_OK;
		}
	}
	
	/*search mc group*/
	if( IGMP_SNOOP_OK != igmp_searchvlangroup(vlan_id,group,&t_mcgroup,&t_mcgroupvlan))
	{
		IGMP_SNP_DEBUG("Igmp_recv_query:search failed\r\n");
		return IGMP_SNOOP_ERR;
	}

	/*is trunk port*/
	result = IFM_PhyIsMerged2TrunkApi(vlan_id,ifindex,&trunkifindex );
	if ( result == IGMP_SNOOP_OK ) /*trunk member*/
	{
		DEBUG_OUT("igmp_recv_query:trunkIfindex:0x%x\r\n",trunkifindex);
		t_ifindex = trunkifindex;
	}
	else
	{
		t_ifindex = ifindex;
	}
	
	if( NULL == t_mcgroupvlan)
	{
		DEBUG_OUT("FLOOD:mcgroupvlan = NULL :\tvlan_id:%d\tifindex:%d\r\n",
					vlan_id,t_ifindex);
		igmp_snp_flood(sk_info,vlan_id,t_ifindex);
		return IGMP_SNOOP_OK;
	}
	
	if( (t_mcgroupvlan->vlan_id != vlan_id)&&(0 == group))
	{
		DEBUG_OUT("FLOOD:mcgroupvlan->vlan_id != vlan&&0==group:\tmcgroupvlan->vlan_id:%d\tvlan_id:%d\tifindex:%d\r\n",
					t_mcgroupvlan->vlan_id,vlan_id,t_ifindex);
		igmp_snp_flood(sk_info,vlan_id,t_ifindex);
		return IGMP_SNOOP_OK;
	}
	
	if( (NULL == t_mcgroup)&&(0 == group) )
	{
		DEBUG_OUT("FLOOD:mcgroup ==NULL&&0==group:\tvlan_id:%d\tifindex:%d\r\n",
					t_mcgroupvlan->vlan_id,vlan_id,t_ifindex);
		igmp_snp_flood(sk_info,vlan_id,t_ifindex);
		return IGMP_SNOOP_OK;
	}
	/*If a query message came from a multicasting router(it's SIP not 0.0.0.0),
	* we should change the multicast router port to this port whether it is a designated port*/
	igmp_snoop_searchrouterlist(vlan_id,t_ifindex,IGMP_PORT_QUERY,&t_router);

	ip_hdr = sk_info->ip_hdr;
	if((0 != ip_hdr)&&(NULL == t_router))
	{
		igmp_snp_router_pre = &(t_mcgroupvlan->routerlist);
		
		while(NULL != * igmp_snp_router_pre)
		{
			if((*igmp_snp_router_pre)->mroute_ifindex == t_ifindex )
			{
				break;
			}
			igmp_snp_router_pre = &((*igmp_snp_router_pre)->next);
		}
		if(NULL == *igmp_snp_router_pre)
		{
			igmp_snp_router_temp = (igmp_router_entry *)malloc(sizeof(igmp_router_entry));
			if( NULL == igmp_snp_router_temp)
			{
				IGMP_SNP_DEBUG("Igmp_recv_query: malloc memory failed.\r\n");
				return IGMP_SNOOP_ERR;
			}
			memset(igmp_snp_router_temp,0,sizeof(igmp_router_entry));
			igmp_snp_router_temp->saddr = ip_hdr->saddr;
			igmp_snp_router_temp->mroute_ifindex = t_ifindex;
			
			/*add router timer*/
			igmp_snp_addintertimer(igmp_router_timeout,&(igmp_snp_router_temp->timer_id));
			
			*igmp_snp_router_pre = igmp_snp_router_temp;
			IGMP_SNP_DEBUG("\r\nEnter igmp_recv_query:add router port vlan = %d,port = %d \r\n",
					vlan_id,t_ifindex);
		}
		else
		{
			(*igmp_snp_router_pre)->saddr = ip_hdr->saddr;
			/*add router timer*/
			igmp_snp_addintertimer(igmp_router_timeout,&((*igmp_snp_router_pre)->timer_id));
		}
	}
	
	/*start response timer.(A random number between[0,maxresptime])*/
	if((NULL != t_mcgroup)&&(0 != group))
	{
		if(NULL != t_mcgroup->portstatelist)	/*only response when group has member*/
		{
			t_mcgroup->resposetime = (maxresptime>0)?(maxresptime * IGMP_V2_TIME_SCALE)
						:(IGMP_V2_QUERY_RESP_INTERVAL * 1000);
			t_mcgroup->resposetime = rand()%((LONG)t_mcgroup->resposetime);
			/*Add Resp Timer*/
			{
				igmp_snoop_pkt *pkt = NULL;
				struct timer_element *new_timer = NULL;

				pkt = (igmp_snoop_pkt *)malloc(sizeof(igmp_snoop_pkt));
				if(NULL == pkt )
				{
					IGMP_SNP_DEBUG("Igmp_recv_query:malloc timer data memory failed.\r\n");
					return IGMP_SNOOP_ERR;
				}

				pkt->vlan_id = vlan_id;
				pkt->groupadd = group;
				pkt->type = IGMP_TIMEOUT_RESP;
				pkt->ifindex = t_ifindex;

				new_timer = create_timer(TIMER_TYPE_NOLOOP, TIMER_PRIORI_NORMAL,
										t_mcgroup->resposetime,
										(void *)Igmp_Event_Resp_Timeout,
										(void *)pkt,sizeof(igmp_snoop_pkt));
				if(  NULL == new_timer )
				{
					IGMP_SNP_DEBUG("Igmp_recv_query:create timer failed.\r\n");
					return IGMP_SNOOP_ERR;
				}

				if( -1 == add_timer(&igmp_timer_list, new_timer,&(t_mcgroup->resposetime_id)))
				{
					IGMP_SNP_DEBUG("Igmp_recv_query:add timer failed.\r\n");
					return IGMP_SNOOP_ERR;
				}
			}
			IGMP_SNP_DEBUG("Igmp_recv_query:start resp timer for g-s query: time:%d\r\n",
							t_mcgroup->resposetime);
			return IGMP_SNOOP_OK;
		}
	}
	else if((0==group)&&(t_mcgroupvlan->vlan_id== vlan_id))
	{
		/*add Resp timer*/
		if( NULL != t_mcgroupvlan->firstgroup )
		{
			igmp_snoop_pkt *pkt = NULL;
			struct timer_element *new_timer = NULL;
			ULONG resptime;
			ULONG resptimer_id;

			resptime = (maxresptime>0)?(maxresptime * IGMP_V2_TIME_SCALE)
						:(IGMP_V2_QUERY_RESP_INTERVAL * 1000);
			resptime = rand()%((LONG)t_mcgroup->resposetime);
			IGMP_SNP_DEBUG("Igmp_recv_query:resp time 0x%x\r\n",resptime);
			pkt = (igmp_snoop_pkt *)malloc(sizeof(igmp_snoop_pkt));
			if(NULL == pkt )
			{
				IGMP_SNP_DEBUG("Igmp_recv_query:malloc timer data memory failed.\r\n");
				return IGMP_SNOOP_ERR;
			}

			pkt->vlan_id = vlan_id;
			pkt->groupadd = group;
			pkt->type = IGMP_TIMEOUT_RESP;
			pkt->ifindex = t_ifindex;

			new_timer = create_timer(TIMER_TYPE_NOLOOP, TIMER_PRIORI_NORMAL,
									t_mcgroup->resposetime,
									(void *)Igmp_Event_Resp_Timeout,
									(void *)pkt,sizeof(igmp_snoop_pkt));
			if(  NULL == new_timer )
			{
				IGMP_SNP_DEBUG("Igmp_recv_query:create timer failed.\r\n");
				return IGMP_SNOOP_ERR;
			}

			if( -1 == add_timer(&igmp_timer_list, new_timer,&(resptimer_id)) )
			{
				IGMP_SNP_DEBUG("Igmp_recv_query:add timer failed.\r\n");
				return IGMP_SNOOP_ERR;
			}
		}
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_recv_leave()
*
* INPUTS:
*		 usIfIndex - Interface Index. 
*		 ulGroup  - multicast group address. 
*		 lVid	-  vlan id
*		 struct igmp_info *sk_info
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK - on success.
*		IGMP_SNOOP_ERR - on error
*
* DESCRIPTION:
*		This function handles IGMP v1 Leave message. If the port received 
*		a Leave message, chang the port state to  checking member state.
*		send g-s query and start rxmt timer. 
*
**************************************************************************/
LONG igmp_recv_leave( ULONG usIfIndex, ULONG ulGroup, LONG lVid,struct igmp_info *sk_info )
{
	MC_group * pMcGroup = NULL;
	MC_group_vlan *pPrevGroupVlan = NULL;
	struct MC_port_state * pstPort = NULL;
	igmp_snoop_pkt stPkt;
	struct iphdr *pIphd = NULL;
	ULONG ulRxmtTimerId = 0;
	/*
	ULONG unIfIndex;
	*/
	LONG result = 0;
	ULONG trunkIfIndex = 0;
	ULONG ulIfIndex = 0;

	IGMP_SNP_DEBUG("\r\nEnter igmp_recv_leave: vid %d group 0x%.8x if 0x%x\r\n", lVid, ulGroup, usIfIndex);

	if ( IGMP_SNOOP_OK != ( igmp_snp_addr_check( ulGroup ) ) )
	{
		IGMP_SNP_DEBUG("igmp_recv_leave: group address invalid. 0x%-8x \r\n", ulGroup);
		return IGMP_SNOOP_OK;
	}

	/* check vlan and group (inlcuded in search group process ) */
	if ( IGMP_SNOOP_OK != ( igmp_searchvlangroup( lVid, ulGroup, &pMcGroup, &pPrevGroupVlan ) ) )
	{
		IGMP_SNP_DEBUG( "Igmp_Recv_Query: search failed\r\n" );
		return IGMP_SNOOP_ERR;
	}

	if ( NULL == pMcGroup )
	{
		IGMP_SNP_DEBUG(" igmp_recv_leave: group:0x%.8x does not exist.\r\n", ulGroup);
		return IGMP_SNOOP_ERR;
	}

	/*check if the report port is a member of trunk */
	result = IFM_PhyIsMerged2TrunkApi( lVid,usIfIndex,&trunkIfIndex);
	if ( result == IGMP_SNOOP_OK ) /*trunk member*/
	{
		DEBUG_OUT("igmp_recv_leave:trunkIfindex:0x%x\r\n",trunkIfIndex);
		ulIfIndex = trunkIfIndex;
	}
	else
	{
		ulIfIndex = usIfIndex;
	}

	if ( Igmp_Snoop_IF_Recv_Is_Report_Discard( lVid, ulIfIndex ) == IGMP_SNOOP_ERR )
	{
		IGMP_SNP_DEBUG( "\r\n This is a reporter port. \r\n");
		return IGMP_SNOOP_ERR;
	}

	/* search port list */
	igmp_snp_searchportlist( &( pMcGroup->portstatelist ), ulIfIndex,IGMP_PORT_QUERY, &pstPort );
	if ( pstPort == NULL )           /* no such port */
	{
		IGMP_SNP_DEBUG( "igmp_recv_leave: port 0x%x is not a member.\r\n", ulIfIndex);
		return IGMP_SNOOP_OK;
	}
	DEBUG_OUT("find port:\tport:0x%x\tstate:0x%x\r\n",ulIfIndex,pstPort->state);
	/* change port state */
	pstPort->state = IGMP_SNP_CHECK_MEMBER;

	IGMP_SNP_DEBUG("igmp_recv_leave: %x state %d \r\n", usIfIndex, pstPort->state);

	/* send g-s query to this port */
	stPkt.vlan_id = lVid;
	stPkt.groupadd= ulGroup;
	stPkt.ifindex = ulIfIndex;
	stPkt.type = IGMP_MSG_GS_QUERY;

	stPkt.saddr = pPrevGroupVlan->saddr;
	if ( IGMP_SNOOP_OK != ( Igmp_Snoop_Send_Igmp( &stPkt ) ) )
	{
		IGMP_SNP_DEBUG( "igmp_recv_leave: send leave message failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	
	/* set retransmit timer*/
	{
	igmp_snoop_pkt *pkt = NULL;
	struct timer_element *new_timer = NULL;

	pkt = (igmp_snoop_pkt *)malloc(sizeof(igmp_snoop_pkt));
	if(NULL == pkt )
	{
		IGMP_SNP_DEBUG("Igmp_recv_query:malloc timer data memory failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	pkt->vlan_id = lVid;
	pkt->groupadd = ulGroup;
	pkt->type = IGMP_TIMEOUT_RXMT;
	pkt->retranscnt = igmp_robust_variable -1;
	pkt->ifindex = ulIfIndex;
	pIphd = sk_info->ip_hdr;
	pkt->saddr = pIphd->saddr;

	new_timer = create_timer(TIMER_TYPE_NOLOOP, TIMER_PRIORI_NORMAL,
									igmp_rxmt_interval,
									(void *)Igmp_Event_Rxmt_Timeout,
									(void *)pkt,sizeof(igmp_snoop_pkt));
	if(  NULL == new_timer )
	{
		IGMP_SNP_DEBUG("Igmp_recv_query:create timer failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( -1 == add_timer(&igmp_timer_list, new_timer,&(ulRxmtTimerId)) )
	{
		IGMP_SNP_DEBUG("Igmp_recv_query:add timer failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	DEBUG_OUT("Set leave timer.\r\n");
	}
	return IGMP_SNOOP_OK;
}


/**************************************************************************
* igmp_recv_unknown()
*
* INPUTS:
*		pSkbuf - struct sk_buf 
*
* OUTPUTS:
*
* RETURN VALUE:
*		IGMP_SNOOP_OK - on success.
*		IGMP_SNOOP_ERR - on error
*
* DESCRIPTION:
*		This function hanles unknown types of IGMP message. Because there may
*		be some higher version IGMP message which the IGMP snooping  process
*		do not support. We flood these packet in the vlan and don't use any 
*		information in this message.
*
* CALLED BY:
*		Igmp_Snoop_MsgRecv_proc()
* CALLS:
*		
* Modify Log:  
*
**************************************************************************/
LONG igmp_recv_unknown(struct igmp_info *sk_info )
{
	LONG lVid;
	ULONG usIfIndex;
	
	IGMP_SNP_DEBUG("Enter igmp_recv_unknown. \r\n" );

	lVid = sk_info->vlan_id;
	usIfIndex = sk_info->ifindex;
	igmp_snp_flood( sk_info, lVid, usIfIndex );
	return IGMP_SNOOP_OK;
}

/**********************************************************************************
*igmp_skb_proc()
*INPUTS:
*struct igmp_skb *msg_skb	receive igmp packet
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	IGMP/PIM  packet handle
*
***********************************************************************************/
INT igmp_skb_proc(struct igmp_skb *msg_skb)
{
	struct iphdr *ip_addr = NULL;
	struct igmp *igmphd = NULL;
	ULONG igmplen;
	INT ret = IGMP_SNOOP_OK;
	LONG vlan_id = 0;
	ULONG group_ip;
	ULONG taged = 0;
	ULONG ifindex;
	ULONG targged;
	struct igmp_info *msg_info = NULL;

	msg_info = (struct igmp_info *)malloc(sizeof(struct igmp_info));
	if( NULL == msg_info)
	{
		IGMP_SNP_DEBUG("igmp_skb_proc:malloc memory failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	memset(msg_info,0,sizeof(struct igmp_info));
	msg_info->ifindex = *((ULONG *)(msg_skb->buf));
	msg_info->vlan_id = *((LONG *)(msg_skb->buf) +1);

	if( IGMP_SNOOP_OK !=igmp_vlanportrelation(msg_info->ifindex,msg_info->vlan_id,&targged))
	{
		IGMP_SNP_DEBUG("igmp_skb_proc:get ifindex targged failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	if( 1 == targged )	/*targged*/
		msg_info->ip_hdr = (struct iphdr *)(msg_skb->buf + 8 + 18);
	else				/*targged*/
		msg_info->ip_hdr = (struct iphdr *)(msg_skb->buf + 8 + 14);

	/*handle PIM packet*/
	if( IPPROTO_PIM == msg_info->ip_hdr->protocol )
	{
		DEBUG_OUT("igmp_skb_proc:receive PIM packet.\r\n");
		pim_recv_msg(msg_skb);
		return IGMP_SNOOP_OK;
	}
	
	msg_info->igmp_hdr = (struct igmp *)((ULONG *)msg_info->ip_hdr + msg_info->ip_hdr->ihl);
	msg_info->data = msg_skb;
	ip_addr = msg_info->ip_hdr;
	igmphd = msg_info->igmp_hdr;
	group_ip = ntohl(igmphd->igmp_group.s_addr);
	IGMP_SNP_DEBUG("\r\nigmp_skb_proc:type 0x%x, time:%d, group:0x%x cksum:%d\r\n",
		igmphd->igmp_type,igmphd->igmp_code,group_ip,igmphd->igmp_cksum);
	
	if(1 != ip_addr->ttl)
	{
		IGMP_SNP_DEBUG("igmp_skb_proc:IP header ttl:%d is not equal to 1.\r\n",ip_addr->ttl);
		return IGMP_SNOOP_ERR;
	}
	IGMP_SNP_DEBUG("Receive IGMP Packet:\r\n");
	igmp_debug_print_skb(msg_skb);
	/*checksum*/
	if(inet_cksum((USHORT *)ip_addr,(ULONG)(ip_addr->ihl)<<2))
	{
		IGMP_SNP_DEBUG("igmp_skb_proc:IP header checksum error\r\n");
		return IGMP_SNOOP_ERR;
	}
	igmplen = sizeof(struct igmp);
	IGMP_SNP_DEBUG("IP header len:%d Igmp data len:%d\r\n",(ip_addr->ihl)<<2,igmplen);
	if( 0 != (ret = inet_cksum((USHORT *)igmphd,(ULONG)(igmplen))))
	{
		IGMP_SNP_DEBUG("igmp_skb_proc:IGMP packet checksum error:cksum:%d\r\n",ret);
		return IGMP_SNOOP_ERR;
	}

	ifindex = msg_info->ifindex;
	vlan_id = msg_info->vlan_id;
	/*if port/trunk not belong to the vlan,not handle this msg*/
	//if(IFM_VlanPortRelationApi(sk_info->ifindex,vlan_id,&taged))
	taged = 1;
	if(0)
	{
		IGMP_SNP_DEBUG("igmp_skb_proc:check the recv port %d and vlan %d.\r\n",ifindex,vlan_id);
		return IGMP_SNOOP_ERR;
	}
	else if(!taged)
	{
		IGMP_SNP_DEBUG("igmp_skb_proc:the recv port %d and vlan %d.\r\n",ifindex,vlan_id);
		return IGMP_SNOOP_ERR;
	}
	switch(igmphd->igmp_type)
	{
		case IGMP_MEMSHIP_QUERY:		/*IGMP Query */
			igmp_recv_query(ifindex,igmphd->igmp_code,group_ip,vlan_id,msg_info);
			break;
		case IGMP_V1_MEMSHIP_REPORT:
		case IGMP_V2_MEMSHIP_REPORT:	/*IGMP report*/
			igmp_recv_report(ifindex,group_ip,vlan_id,igmphd->igmp_type,msg_info);
			break;
		case IGMP_V2_LEAVE_GROUP:	/*IGMP leave*/
			igmp_recv_leave(ifindex,group_ip,vlan_id,msg_info);
			break;
		default:
			igmp_recv_unknown(msg_info);
			break;	
	}
	free(msg_info);
	msg_info = NULL;
	DEBUG_OUT("End for skb handle!\r\n")
}


/**********************************************************************************
*create_recvskb_thread()
*INPUTS:
*none
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	init igmp_snoop receive skb thread
***********************************************************************************/
static void *create_recvskb_thread(void)
{
	int ret,write_flag;
	int sock,accept_sock,len;
	struct igmp_skb *msg_skb = NULL; 
	struct sockaddr_un client;
	fd_set rfds;
#if 0	
	if( !IGMP_SNP_ISENABLE() )
	{
		IGMP_SNP_DEBUG("IGMP Snooping is switch-off.\r\n");
		return;
	}
#endif
#if 0	
	if(0 >(sock = creatservsock_stream(IGMP_SNOOP_SKB_SOCK)))
	{
		IGMP_SNP_DEBUG("Create msg socket failed.\r\n");
		return;
	}
#else
	if(0 >(sock = creatclientsock_stream(IGMP_SNOOP_SKB_SOCK)))
	{
		IGMP_SNP_DEBUG("Create msg socket failed.\r\n");
		return;
	}
#endif
	kernel_fd = sock;
	//igmp_enable_init();
	memset(&client,0,sizeof(struct sockaddr_un));
	len = sizeof(struct sockaddr_un);
	while(1)
	{
#if 0
		if((accept_sock = accept(sock,(struct sockaddr *)&client,&len))<0)
		{
			IGMP_SNP_DEBUG("Accept failed:errno %d [%s].\r\n",errno,strerror(errno));
			close(sock);
			/*需要增加对其他线程的退出信号*/
			return;
		}
#endif
		accept_sock = sock;
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
					INT recvlen=0;
					msg_skb = (struct igmp_skb *)malloc(sizeof(struct igmp_skb));
					if( NULL == msg_skb )
					{
						IGMP_SNP_DEBUG("create_recvskb_thread:malloc igmp_skb memory failed.\r\n");
						return;
					}
					do{
						memset(msg_skb,0,sizeof(struct igmp_skb));
						recvlen = recv(accept_sock,(void *)msg_skb,sizeof(struct igmp_skb),0);
						IGMP_SNP_DEBUG("Read data successful!\r\n");
						/*handle skb*/
						if(recvlen > 0&&recvlen>=sizeof(struct igmp_skb))
						{
//							igmp_debug_print_skb(msg_skb);
							igmp_message_proc(msg_skb);
						}
					}while(recvlen > 0);
					free(msg_skb);
					msg_skb = NULL;
					break;
				}
		}
		close(accept_sock);
		accept_sock = 0;
		kernel_fd = accept_sock;
	}
	IGMP_SNP_DEBUG("Create msg thread success!\r\n");
	IGMP_SNP_DEBUG("Create recvskb thread success!\r\n");

}

/**********************************************************************************
*init_igmp_snp_timer()
*INPUTS:
*struct timer_element *cur 	mc group timer
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	create mc group timeout handle
***********************************************************************************/
VOID igmp_snp_global_timer_func( struct timer_element *cur )
{
	INT i;
	INT next = 0;
	MC_group_vlan *p_vlan;
	igmp_router_entry *p_router_entry;
	igmp_router_entry *p_prev_router_entry;
	MC_group		*p_group;
	struct MC_port_state		*p_port;
	
	for(i = 0; i<IGMP_GENERAL_GUERY_MAX; ++i)
	{
		if(0 < igmp_genquerytime[i])
			igmp_genquerytime[i]--;
	}
	
	p_vlan = GET_VLAN_POINT_BY_INDEX(first_vlan_idx);
	while(NULL != p_vlan)
	{
		next = p_vlan->next;		/*get next index firstly*/
		if( 0 != p_vlan->vlan_id )
		{
			if( p_vlan->querytimer_id == 1 && igmp_genquerytime[p_vlan->vlan_id] <=1)
			{
				Igmp_Event_GenQuery_Timeout(p_vlan);
				p_vlan->querytimer_id = igmp_query_interval;
				igmp_genquerytime[p_vlan->vlan_id] = igmp_query_interval;
			}
			p_vlan->querytimer_id--;
		}

		if(NULL != p_vlan->routerlist)
		{
			p_router_entry = p_vlan->routerlist;
			p_prev_router_entry = p_vlan->routerlist;
			while( NULL != p_router_entry )
			{
				if( 0 < p_router_entry->timer_id )
				{
					if( 1 == p_router_entry->timer_id )
					{
						p_prev_router_entry = p_router_entry->next;
						if( p_vlan->routerlist == p_router_entry )	/*The first route port,need update vlan->routerlist*/
							p_vlan->routerlist = p_prev_router_entry;	
						free(p_router_entry);
						p_router_entry = p_prev_router_entry;	
						continue;
					}
					else
					{
						p_router_entry->timer_id--;
					}
				}
				p_prev_router_entry = p_router_entry;
				p_router_entry = p_router_entry->next;
			}
		}

		p_group = p_vlan->firstgroup;
		while(NULL != p_group)
		{
			if( 0 < p_group->lifetimer_id )
			{
				if( 1 == p_group->lifetimer_id )
				{
					MC_group *next_group = p_group->next;
					if( IGMP_SNOOP_OK !=Igmp_Event_GroupLife_Timeout(p_vlan,p_group))
					{
						IGMP_SNP_DEBUG("igmp_snp_global_timer_func:delete group failed.\r\n");
						return;
					}
					DEBUG_OUT("Delete group and vlan successful.\r\n");
					if( NULL ==p_vlan )
						break;	/*vlan is already released */
					p_group = next_group;
					continue;
				}
				p_group->lifetimer_id--;
			}

			if( 0 < p_group->router_reporttimer )
			{
				if( 1 == p_group->router_reporttimer )
				{	/*proxy send report*/
					Igmp_Event_Proxy_Timeout(p_vlan,p_group);
					if( 0 != p_group->router_reporttimer )
						p_group->router_reporttimer = 11;
				}
				if( 0 != p_group->router_reporttimer )
					p_group->router_reporttimer--;
			}

			p_port = p_group->portstatelist;
			while( NULL != p_port )
			{
				if( 0 != p_port->membertimer_id )
				{
					if( 1 == p_port->membertimer_id )
					{		/*Release this port state*/
						struct MC_port_state *next_port = p_port->next;
						Igmp_Event_GroupMember_Timeout(p_vlan,p_group,p_port->ifindex);
						p_port = next_port;
						continue;
					}
					p_port->membertimer_id--;
				}

				if( 0 != p_port->hosttimer_id )
				{
					if( 1 == p_port->hosttimer_id )
					{
						p_port->hosttimer_id = 0;	/*time expire, set state*/
						p_port->state = IGMP_SNP_HAS_MEMBER;
						p_port = p_port->next;
						continue;
					}
					p_port->hosttimer_id--;
				}
				p_port = p_port->next;
			}
			p_group = p_group->next;
		}
		p_vlan = GET_VLAN_POINT_BY_INDEX(next);
	}
}

/**********************************************************************************
*init_igmp_snp_timer()
*INPUTS:
*none
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	create mc group timer
***********************************************************************************/
INT init_igmp_snp_timer(void)
{
	struct timer_element *new_timer = NULL;
	
	new_timer = create_timer(TIMER_TYPE_LOOP,TIMER_PRIORI_NORMAL,
							1,(void *)igmp_snp_global_timer_func,
							NULL,0);
	if( NULL == new_timer )
	{
		IGMP_SNP_DEBUG("init_igmp_snp_timer:create timer failed.\r\n");
		return IGMP_SNOOP_ERR;
	}

	if( -1 == add_timer(&igmp_timer_list,new_timer,NULL))
	{
		IGMP_SNP_DEBUG("init_igmp_snp_timer:add timer failed.\r\n");
		return IGMP_SNOOP_ERR;
	}
	return IGMP_SNOOP_OK;
}

 
/**********************************************************************************
*create_timer_thread()
*INPUTS:
*none
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	init igmp_snoop timer thread
***********************************************************************************/
static void *create_timer_thread(void)
{
	INT ret = 0;
	int timer_id;
	struct timer_element *tnext = NULL;
		
	while(1)
	{
		ret = usleep(1000);
		if( !IGMP_SNP_ISENABLE() )	/*igmp snoop disable*/
			continue;

		if( -1 == ret )
		{
			IGMP_SNP_DEBUG("create_timer_thread:usleep error.\r\n");
			return;
		}
		pthread_mutex_trylock(&mutex);
		while( igmp_timer_list.lock )
			igmp_timer_list.lock = 0;

		if( igmp_timer_list.first_timer )
		{
			tnext = igmp_timer_list.first_timer;
			while( tnext )
			{
				tnext->current++;
				if(tnext->current == tnext->expires )
				{
					if( NULL != tnext->func )
						tnext->func(tnext);

					if( TIMER_TYPE_NOLOOP == tnext->type )
					{		/*NO LOOP*/
						timer_id = tnext->id;
						tnext = tnext->next;
						igmp_timer_list.lock = 1;
						del_timer(&igmp_timer_list,timer_id);
						igmp_timer_list.lock = 0;
						continue;
					}
					else
					{		/*LOOP*/
						tnext->current = 0;
					}
				}
				tnext = tnext->next;
			}
		}
		igmp_timer_list.lock = 1;
		pthread_mutex_unlock(&mutex);
	}	
	IGMP_SNP_DEBUG("Create timer thread success!\r\n");
}


/**********************************************************************************
*igmp_config_init()
*INPUTS:
*none
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*	init igmp_snoop according to config file
***********************************************************************************/
static INT igmp_config_init(void)
{	
	CHAR buf[CONF_FILE_MAX_ROW];	
	FILE *fd=NULL;
	
	/*默认配置*/
	igmp_snoop_debug = 1;
	//igmp_snoop_enable = IGMP_SNOOP_YES;

	p_l2mc_list_head.stListHead = NULL;
	p_l2mc_list_head.ulListNodeCount = 0;
	memset(&igmp_timer_list,0,sizeof(struct timer_list));
	igmp_timer_list.lock = 1;
	
	IGMP_SNP_DEBUG("init igmp\r\n");

	if(NULL ==(fd = fopen(IGMP_SNOOP_CONFIG_PATH,"rt")) )
	{
		IGMP_SNP_DEBUG("Config file is not existence, we will create it.\r\n");
		creat_config_file(IGMP_SNOOP_CONFIG_PATH);
	}
	
	memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
	//while(fgets(buf,CONF_FILE_MAX_ROW-1,fd))
	while(!feof(fd))
	{
		fgets(buf,CONF_FILE_MAX_ROW - 1,fd);
		if('#' == buf[0])
		{
			memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
			continue;
		}
		read_config(buf);
		memset(buf,0,sizeof(char)*CONF_FILE_MAX_ROW);
	}
	return IGMP_SNOOP_OK;
}


/**********************************************************************************
*main()
*INPUTS:
*none
*OUTPUTS:
*RETURN VALUE:
*	0 - success
*	!=0 - error
*DESCRIPTION:
*
***********************************************************************************/
int main(VOID)
{
	int ret = 0;
#if 0
	if(IGMP_SNOOP_OK != igmp_config_init())
	{
		IGMP_SNP_DEBUG("Init igmp snooping failed.\r\n");
		goto error;
	}
#endif
    printf("create timer\n");
	ret = pthread_create(&thread_timer,NULL,(void *)create_timer_thread,NULL);
	if( 0 != ret )
	{
		IGMP_SNP_DEBUG("Create timer thread fail.\r\n");
		goto error;
	}
    printf("create messager\n");
	ret = pthread_create(&thread_msg,NULL,(void *)create_msg_thread,NULL);
	if( 0 != ret )
	{
		IGMP_SNP_DEBUG("Create message thread fail.\r\n");
		pthread_join(thread_timer,NULL);
		goto error;
	}
    printf("create reciver\n");
	ret = pthread_create(&thread_recvskb,NULL,(void *)create_recvskb_thread,NULL);
	if( 0 != ret )
	{
		IGMP_SNP_DEBUG("Create message thread fail.\r\n");
		pthread_join(thread_msg,NULL);
		pthread_join(thread_timer,NULL);
		goto error;
	}

	if(IGMP_SNOOP_OK != igmp_config_init())
	{
		IGMP_SNP_DEBUG("Init igmp snooping failed.\r\n");
		goto error;
	}

	pthread_join(thread_recvskb,NULL);
	pthread_join(thread_msg,NULL);
	pthread_join(thread_timer,NULL);
	IGMP_SNP_DEBUG("igmp snoop task start!");
	
	return IGMP_SNOOP_OK;
error:
	return IGMP_SNOOP_ERR;
}

