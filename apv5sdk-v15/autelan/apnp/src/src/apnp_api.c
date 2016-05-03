#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <dlfcn.h>

#include <pthread.h>

#include <libxml/xmlsave.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <libxml/xpathInternals.h>


/** include apnp **/
#include "apnp_util.h"
#include "apnp_type.h"
#include "apnp_transport.h"
#include "apnp_function.h"
#include "apnp_log.h"

#include "apnp_api.h"
#include "apnp_list.h"

extern char *ifname;
extern char *pbuf;
extern int apnp_running;
void *dl_handle ;
struct sockaddr_in ser_addr;


typedef struct apnp_task_s	apnp_task;
static void *
_sess_add(apnp_session *in_session, 
				apnp_transport *transport);


static pthread_mutex_t session_mutex;
static pthread_mutex_t task_mutex;
static pthread_cond_t	task_cond;
static u_long global_sessid 	= 0;
static u_long global_taskid		= 0;

static int init_tdomain_init_done = 0;

static struct session_list *Sessions = NULL;
static struct list_head sessions_head = LIST_HEAD_INIT(sessions_head);

static struct list_head task_head = LIST_HEAD_INIT(task_head);


struct session_list {
	//struct session_list *next;
	apnp_session *session;
	apnp_transport *transport;
	//struct manage_internal_session *internal;
	struct list_head session_node;
};


/*
struct apnp_message {
	u_char	m_type;
	
	u_char 	m_errno;
	
	u_short	method_id;

	size_t 	data_length;

	u_int	m_check;
};*/

struct apnp_task_s {
	u_long  task_id;     			/* task id */
	
	u_long	message_type;
	task_method		method;				/* user method process task */

	void		*transport_data;
	size_t     transport_data_length;

	void		*para_data;				/* user para data for process task */
	size_t	para_data_length;

	void			*method_data;		/* user method data process task */
	size_t		method_data_length;
	
	u_long          timeout;        		/* length to wait for timeout */
	struct timeval  time;   				/* Time this task was made */
	struct timeval  expire; 			/* time this task is due to expire */

	struct session_list 	*sessp;
	struct list_head task_node;
};




struct TLV{

	short int type;
	int length;
	char value[0];

}__attribute((packed));

int create_beat_xmltobuff(char *ipstr,char *mac,char **out_buff)
{
	ApnpDebugLog("enter create_beat_xmltobuff\n");
	int size;
	xmlDocPtr doc;
	doc = xmlNewDoc(BAD_CAST"1.0");

	if(doc){
		xmlNodePtr beat = xmlNewNode(NULL,BAD_CAST"beat");

		xmlDocSetRootElement(doc,beat);
		xmlNewTextChild(beat,NULL,BAD_CAST"mac",BAD_CAST mac);
		xmlNewTextChild(beat,NULL,BAD_CAST"ip",BAD_CAST ipstr);

		xmlDocDumpMemory(doc,out_buff,&size);
		//xmlSaveFile("/tmp/test_test.xml",doc);
		
		return size;
	}
	else {
		ApnpDebugLog("xmlNewDoc error\n");
		return -1;
		
	}
	
}

void beat_handle(void)
{
	ApnpDebugLog("send beat frame ...\n");
	
	int beat_fd=-1;
	int sdata;
	char bframe[45]={0};

	
	struct session_list *slp=NULL;
	struct session_list *next=NULL;
	pthread_mutex_lock(&session_mutex);
	list_for_each_entry_safe(slp,next,&sessions_head,session_node)
	{
		if(APNP_FLAGS_UDP_SOCKET & slp->transport->flags )
			beat_fd = slp->transport->sock;
	}	
	/*
	for(slp=Sessions;slp;slp=slp->next)
	{
		if(APNP_FLAGS_UDP_SOCKET & slp->transport->flags )
			beat_fd = slp->transport->sock;
	}*/
	pthread_mutex_unlock(&session_mutex);

	uint32_t ipaddr;
	char ipstr[32];
	char mac[18];
	char *beat_buff;
	int len;

	ipaddr = do_ioctl_get_ipaddress(ifname);
	
	ip2str(ipaddr, ipstr, sizeof(ipstr));
	ApnpDebugLog("host ipastr:%s\n",ipstr);

	do_ioctl_get_mac(ifname,mac);
	ApnpDebugLog("host mac:%s\n",mac);

	//sprintf(bframe,"mac:%s-ip:%s",mac,ipstr);

	if(beat_fd < 0){
		ApnpDebugLog("beat_handle: sock search ERROR!\n");
		return NULL;
	}
	len = create_beat_xmltobuff(ipstr,mac,&beat_buff);
	if(len < 0){
		ApnpDebugLog("beat_handle: create beat frame error\n");
		return NULL;
	}
#if 0
	memset(&ser_addr,0,sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(38800);
	ser_addr.sin_addr.s_addr = inet_addr("192.168.24.12");
	ApnpDebugLog("udp server port:%d~~~~~ip:%s\n",ser_addr.sin_port,inet_ntoa(((ser_addr.sin_addr))));
#endif
	while(apnp_running){
		//ApnpDebugLog("sending ...\n");
		sdata = sendto(beat_fd,beat_buff,len,0,&ser_addr,sizeof(ser_addr));
		
		if( sdata < 0 ){
			ApnpDebugLog("beat_handle: sendto error!\n");
			//close(beat_fd); // 
		}
	
		sleep(5);
	}
	pthread_exit(0);
}

 void 
	apnp_dlysm_init()
{

	
	char *dl_path = "/lib/libapnplib.so";
	
	
	dl_handle = dlopen(dl_path,RTLD_LAZY);

	if(!dl_handle){
		ApnpDebugLog("dlysm_functon: dlerror()->%s\n",dlerror());
		return NULL;
	}
	ApnpDebugLog("init apnp_dlysm_init sucess\n");
	
}
#if 0
static char *parse_string(const char *msg,int total)
{


	char *str=msg;
	int type=-1;
	
	int ilength=0;
	char *arg=NULL;
	char *tmp=NULL;
	int va_len=0;
	int str_len = total;

	printf("msg:%s----total:%d\n",msg,total);
	for(;str_len>0;)
	{
		type = ((struct TLV*)str)->type;
		
		
		//if(0 == type)
		{
			ilength = ((struct TLV*)str)->length;
			va_len += (ilength -sizeof(short int)-sizeof(int)+1);
			printf("ilength:%d\n",ilength);
			printf("va_len:%d\n",va_len);
			arg = (char *)realloc(tmp,va_len);
			if(!arg){
				printf("parse_string:realloc error\n");
				free(tmp);
				return NULL;
			}
			tmp=arg;
			printf("str->value:%s\n",((struct TLV*)str)->value);
			strncat(arg,((struct TLV*)str)->value,(ilength -sizeof(short int)-sizeof(int)));
			strncat(arg,",",1);

		}
		str_len -= ilength;
		str += ilength;

	}
	str = NULL;
	arg = (char *)realloc(arg,va_len+4);
	strncat(arg,"NULL",4);
	
	return arg;

	/*
	printf("enter parse_string\n");
	int data_len = total;
	int i;
	int ilength;
	int ilength_tmp=0;
	char *type = (char *)malloc(sizeof(char)*1);
	memset(type,0,1);
	char *slength = (char *)malloc(sizeof(char)*4);
	memset(slen`	gth,0,4);
	char *str_fun=NULL;
	char *str_arg=NULL;	
	char *tmp=NULL;
	
	strncpy(type,msg,1);
	msg += 1;
	data_len -= 1;
	strncpy(slength,msg,4);
	msg += 4;
	data_len -= 4;
	ilength = atoi(slength);
	str_fun = (char *)malloc(sizeof(char)*(ilength-1));
	strncpy(str_fun,msg,ilength-1);
	data_len -= ilength;
	printf("type:%s --- slength:%s --- str_fun:%s\n",type,slength,str_fun);
	printf("fill in astr_arg\n");
	ilength = 0;
	for(;data_len > 0;)
	{
		strncpy(type,msg,1);
		msg += 1;
		data_len -= 1;
		printf("for type%s\n",type);
		strncpy(slength,msg,4);
		printf("for slength:%s\n",slength);
		msg += 4;
		data_len -= 4;
		ilength = atoi(slength);
		ilength_tmp += ilength;
		
		str_arg = realloc(str_arg,sizeof(char)*(ilength_tmp));
		//str_arg = tmp;
		strncat(str_arg,msg,ilength);
		strncat(str_arg,",",1);
		msg += (ilength+1);
		data_len -= (ilength+1);
	}
	strncat(str_arg,"NULL",4);

	printf("str_arg:%s\n",str_arg);
	
	return str_arg;
*/

}
#endif

static inline apnp_task *
_task_create(void *sessp, void *opaque, u_long olength) {

	struct session_list *slp = (struct session_list *)sessp;
	apnp_task *task = (apnp_task *)calloc(1, sizeof(apnp_task));
	if(NULL == task) {
		apnp_syslog(LOG_ERR, "_task_create: can't malloc space for task\n");
		return NULL;
	}

	task->task_id = ++global_taskid;
	task->transport_data = opaque;
	task->transport_data_length = olength;
	task->sessp = slp;
	
	gettimeofday(&task->time, NULL);
		
	return task;
}

static int
_task_parse(apnp_task *task, u_char *data, size_t length) {
	task->method = task->sessp->session->callback;
	task->para_data_length = length;
	task->para_data = (void *)calloc(1, task->para_data_length);

	if(NULL == task->para_data) {
		apnp_syslog(LOG_ERR, "_task_parse: task (%d), can't malloc space for task para data\n", task->task_id);
		//task->message.m_errno = (u_char)APNP_ERR_MALLOC_FAIL;
		return APNP_ERR_MALLOC_FAIL;
	}
	memcpy(task->para_data, data, task->para_data_length);
	return APNP_ERR_SUCCESS;
}

static inline void
_task_add(apnp_task *task) {

	int queue_empty = 0;
	
	pthread_mutex_lock(&task_mutex);	

	if(list_empty(&task_head)){
		queue_empty = 1;
	}
	list_add(&(task->task_node),&task_head);

	pthread_mutex_unlock(&task_mutex);	
	if(queue_empty) {
		pthread_cond_broadcast(&task_cond);
	}	
	
	return ;
}

static inline void
_task_free(apnp_task **task) {
	if(NULL == task || NULL == *task) {
		return ;
	}
	APNP_FREE((*task)->transport_data);
	APNP_FREE((*task)->para_data);
	APNP_FREE((*task)->method_data);
	APNP_FREE(*task);
	
	return ;
}

static inline apnp_task *
_task_get(void){
	ApnpDebugLog("enter _task_get\n");
	
	apnp_task *task = NULL;	
	
	pthread_mutex_lock(&task_mutex);	
	while(list_empty(&task_head)) {
		pthread_cond_wait(&task_cond, &task_mutex) ;		/** wait for task cond **/
	} 
/*
	task = task_queue;
	task_queue = task_queue->next;
	if(NULL == task_queue) {
		task_tail = NULL;
	}
*/
	task = list_first_entry(&task_head,apnp_task,task_node);
	list_del(&(task->task_node));//need?
	pthread_mutex_unlock(&task_mutex);	

	ApnpDebugLog("exit _task_get, task (%p) id (%d)\n", task, task->task_id);
	return task;
}

static inline int
_task_process(apnp_task *task) {
	
	if(NULL == task) {
		ApnpDebugLog( "_task_process: task is NULL\n");
		return APNP_ERR_BAD_INPUT;
	}
	ApnpDebugLog("enter _task_process, task (%p) id (%d)\n", task, task->task_id);
	
	if(!task->method){
		ApnpDebugLog("_task_process: task->method is NULL\n");
		return APNP_ERR_BAD_INPUT;
	}
	
	struct session_list *slp = (struct session_list *)(task->sessp);
	apnp_transport *transport = slp->transport;

#if 0

/*  parse command with TLV*/


	struct TLV *tmp = NULL;
	char * data=(char *)malloc(sizeof(char) * 21);
	memset(data,0,22);
	
	tmp = (struct TVL *)malloc(sizeof(struct TLV)+5);
	memset(tmp,0,sizeof(struct TLV)+5);
	tmp->type = 0;
	tmp->length =11;
	memcpy(tmp->value,"abcde",5);
	memcpy(data,tmp,sizeof(struct TLV)+5);
	free(tmp);
	tmp = NULL;
	printf("sizeof(stuct TLV):%d --sizeof(int):%d--sizeof(short int):%d\n",sizeof(struct TLV),sizeof(int),sizeof(short int));
	printf("value:%c,%c,%c,%c,%c\n",((struct TLV*)data)->value[0],
					((struct TLV*)data)->value[1],
					((struct TLV*)data)->value[2],
					((struct TLV*)data)->value[3],
					((struct TLV*)data)->value[4]);

	
	tmp = (struct TVL *)malloc(sizeof(struct TLV)+4);
	memset(tmp,0,sizeof(struct TLV)+4);
	tmp->type = 0;
	tmp->length =10;
	memcpy(tmp->value ,"1234",4);
	memcpy(data+sizeof(short int)+sizeof(int)+5,tmp,10);
	
	free(tmp);
	tmp =NULL;

	
	//printf("\nparse_string():%s\n",parse_string(data,21));
	char *ss = parse_string(data,21);
	printf("ss:%s\n",ss);
	dlsym_function(5,ss);
#endif

	if(slp->session->message_type == APNP_MESSAGE_TYPE_RET)
	{	
		ApnpDebugLog("messae_type == APNP_MESSAGE_TYPE_RET\n");
		xmlChar*dbuf = read_mem_xml((char *)(task->para_data),task->para_data_length);

		if(dbuf == NULL){
			ApnpDebugLog("dbuf null\n");
			return -1;
		}
		ApnpDebugLog("strlen(dbuf):%d -- (UDP)dbuf:\n%s\n",strlen(dbuf),dbuf);
		
		if(transport->f_send(&transport->sock,(char *)dbuf,strlen(dbuf),&ser_addr,sizeof(struct sockaddr_in))< 0){
				ApnpDebugLog("_task_process(UDP): f_send error\n");
		}
		if(dbuf != NULL){
			xmlFree(dbuf);
			dbuf=NULL;
		}
		dbuf=NULL;
	}
	if(slp->transport->flags & APNP_FLAGS_STREAM_SOCKET)
	{
		ApnpDebugLog("transport->flags == APNP_FLAGS_STREAM_SOCKET\n");
		xmlChar*dbuf = read_mem_xml((char *)(task->para_data),task->para_data_length);
		if(dbuf == NULL){
			ApnpDebugLog("dbuf null\n");
			return -1;
		}
		ApnpDebugLog("strlen(dbuf):%d -- (TCP)dbuf:%s\n",strlen(dbuf),dbuf);

		if(transport->f_send(&transport->sock,(char *)dbuf,strlen(dbuf),NULL,0)< 0){
				ApnpDebugLog("_task_process(TCP): f_send error\n");
		}
		if(dbuf != NULL){
			xmlFree(dbuf);
			dbuf=NULL;
		}
		dbuf = NULL;
	}
	else
	{
		ApnpDebugLog("run into task\n");
		int ret;
		ret = (u_char)task->method(task->para_data, task->para_data_length, 
											&(task->transport_data), &(task->transport_data_length));
	}
}

void* 
_task_thread(void) {
	
	while(1) {
		apnp_task *task = _task_get();
		_task_process(task);
		_task_free(&task);
	}

	return ;
}


static int
_sess_process_packet(void *sessp,
							u_char *packetptr, size_t length,
							void *opaque, size_t olength) {
	int op_ret;
	apnp_task	*task = NULL;
	struct session_list *slp = (struct session_list *)sessp;
	
	//struct manage_internal_session *isp = slp ? slp->internal : NULL;
	
	ApnpDebugLog( "enter _sess_process_packet\n");

	if(!slp) {
		ApnpDebugLog("_sess_process_packet: slp or isp is NULL\n");
		op_ret = APNP_ERR_BAD_INPUT;
		goto PROCESS_ERROR;
	}

	task = _task_create(slp, opaque, olength);
	

	if(NULL == task) {
		ApnpDebugLog("sess_process_packet: task create fail\n");
		op_ret = APNP_ERR_MALLOC_FAIL;
		goto PROCESS_ERROR;
	}

	//there can change the way to call the function to callback
	op_ret = _task_parse(task, packetptr, length);

	if(APNP_ERR_SUCCESS != op_ret) {
		ApnpDebugLog( "sess_process_packet: task prase fail\n");
		_task_free(&task);
		return APNP_ERR_BAD_PARSE;
	}


	_task_add(task);
	
	return APNP_ERR_SUCCESS;


PROCESS_ERROR:
	APNP_FREE(opaque);
	return op_ret;
}


static int
apnp_sess_read(void *sessp, fd_set *fdset) {
	struct session_list *slp = (struct session_list *)sessp;
	apnp_session *sp = slp ? slp->session : NULL;
	apnp_transport *transport = slp ? slp->transport : NULL;
	static u_char *rxbuf = NULL;
	void *opaque = NULL;
	size_t rxbuf_len = 65536, olength = 0;
	int length = 0, rc = 0;

	ApnpDebugLog("enter apnp_sess_read\n");	
	
	if(!slp || !transport) {
		ApnpDebugLog("apnp_sess_read : read fail: closing...\n");
		return 0;
	}
	
	/*  */ 
	if(transport->sock < 0) { 
		ApnpDebugLog("apnp_sess_read: transport->sock got negative fd value %d\n", transport->sock);
		return 0; 
	}

	if(!fdset || !FD_ISSET(transport->sock, fdset)) {
		ApnpDebugLog("apnp_sess_read: not reading %d (fdset %p set %d)\n",
				            transport->sock, fdset, fdset ? FD_ISSET(transport->sock, fdset) : -9);
		return 0;
	}
	
	ApnpDebugLog( "apnp_sess_read:  reading %d (fdset %p set %d)\n",
								transport->sock, fdset, fdset ? FD_ISSET(transport->sock, fdset) : -9);


	if(rxbuf) {
		ApnpDebugLog("apnp_sess_read: the buf is already malloc\n");
	} else {
		if(NULL == (rxbuf = (u_char *) malloc(rxbuf_len))) {
			 ApnpDebugLog("apnp_sess_read: can't malloc %u bytes for rxbuf\n", rxbuf_len);
			return 0;
		}
	}
	
//	char paddr[20];
	//inet_ntop(AF_INET,(void *) &(((struct sockaddr_in * )(transport->addr))->sin_addr),paddr,16);
	//printf("apnp_sess_read:%s\n",paddr);
	
	if(transport->flags & APNP_FLAGS_LISTEN_SOCKET){
		int data_sock = transport->f_accept(transport);
		if(data_sock >= 0){
			apnp_transport *new_transport = app_transport_copy(transport);
			if(new_transport != NULL){
				struct session_list * nslp = NULL;
				new_transport->sock = data_sock;
				new_transport->flags &= ~APNP_FLAGS_LISTEN_SOCKET;

				nslp = (struct session_list *)_sess_add(sp,new_transport);

				if(nslp != NULL){
					//nslp->next = Sessions;
					//Sessions = nslp;
					list_add(&(nslp->session_node),&sessions_head);
				}
				return 0;
			}else {
				sp->s_errno = APNP_ERR_MALLOC_FAIL;
				return -1;
			}
		}else{
			sp->s_errno = APNP_ERR_ACCEPT;
			return -1;
		}

	}

	
    length = transport->f_recv(transport, rxbuf, rxbuf_len, &opaque, &olength);

	if(length <= 0 && transport->flags & APNP_FLAGS_STREAM_SOCKET){

		ApnpDebugLog("apnp_sess_read :tcp close sock\n");
		transport->f_close(transport);

		//dlclose(dl_handle);
		return -1;
	}

	if(length <= 0 && transport->flags & APNP_FLAGS_UDP_SOCKET){
		ApnpDebugLog("apnp_sess_read : udp close sock\n");
		transport->f_close(transport);
		//dlclose(dl_handle);

	}
	ApnpDebugLog("apnp_sess_read :rxbuf:%s\n",rxbuf);
	ApnpDebugLog("apnp_sess_read: transport recv %d\n", length);	

	//opaque = (void * )transport->addr;
	//olength = sizeof(struct sockaddr);
		
	rc = _sess_process_packet(sessp, rxbuf, length, opaque, olength);
	return rc;
}
  

void
apnp_read(fd_set *fdset) {
	struct session_list *slp;
	struct session_list *next;
	pthread_mutex_lock(&session_mutex);	
//	for(slp = Sessions; slp; slp = slp->next) {
//		apnp_sess_read(slp, fdset);
//	}
	list_for_each_entry_safe(slp,next,&sessions_head,session_node){
		apnp_sess_read(slp, fdset);
	}
	pthread_mutex_unlock(&session_mutex);	

	return ;
}

void
apnp_timeout()
{
	struct session_list *slp;
	slp = list_first_entry(&sessions_head,typeof(*slp),session_node);
		ApnpDebugLog("slp->session:%p",slp->session);
		ApnpDebugLog("slp->transport:%p",slp->transport);
}

static void
apnp_sess_free(apnp_session *s) {
	if(NULL == s) {
		ApnpDebugLog("apnp_sess_free: input *s of apnp_session point is NULL\n");
		return ;
	}

	APNP_FREE(s->local);
	APNP_FREE(s->remote);
	APNP_FREE(s);
	return ;
}



static int
apnp_sess_close(void *sessp) {
	struct session_list *slp = (struct session_list *) sessp;
	//struct manage_internal_session *isp = NULL;

	if(NULL == slp) {
		ApnpDebugLog( "apnp_sess_close: input *sessp is NULL\n");
		return APNP_ERR_BAD_INPUT;
	}

	//isp = slp->internal;
	//slp->internal = NULL;

	ApnpDebugLog("free transport\n");
	if(slp->transport) {
		slp->transport->f_close(slp->transport);
		apnp_transport_free(slp->transport);
		slp->transport = NULL;
	}
	ApnpDebugLog("free session\n");
	if(slp->session) {
		apnp_sess_free(slp->session);
		slp->session = NULL;
	}

	list_del(&(slp->session_node));//repeat??
		
	APNP_FREE(slp);
	
	return APNP_ERR_SUCCESS;
}



int
apnp_select_info(int *numfds, fd_set *fdset,
					struct timeval *timeout, int *block) {
	struct session_list *slp;
	struct session_list *next;
	
	*numfds = 0;
	*block = 1;

	FD_ZERO(fdset);

	pthread_mutex_lock(&session_mutex);
	
	//for(slp = Sessions; slp; slp = slp->next) {
	list_for_each_entry_safe(slp,next,&sessions_head,session_node) {
	sess_select:
		
		if(NULL == slp->transport) {
			/*
	 		 * Close in progress -- skip this one.  
	 		 */
	 		ApnpDebugLog("apnp_sess_select_info:  skip\n");
			continue;
		}

		if(slp->transport->sock < 0) {
			/*
			 * This session was marked for deletion.  
			 */
	 		ApnpDebugLog("apnp_sess_select_info:  delete session\n");
			
//			struct session_list *next = slp->next;
			apnp_sess_close(slp);
//			slp = next;
//			Sessions = next;

//			if(slp) {
//				goto sess_select;
//			}
			
//			break;
			continue;
		}
		
		if((slp->transport->sock + 1) > *numfds) {
		    *numfds = (slp->transport->sock + 1);
		}
	 	ApnpDebugLog("apnp_sess_select_info:  fd_set %d, numfds = %d\n", 
												slp->transport->sock, *numfds);
		FD_SET(slp->transport->sock, fdset);
	}	
	pthread_mutex_unlock(&session_mutex);	

    return APNP_ERR_SUCCESS;
}


static struct session_list *
_sess_copy(apnp_session * in_session) {
	struct session_list *slp = NULL;
	//struct manage_internal_session *isp = NULL;

	in_session->s_manage_errno = 0;
	in_session->s_errno = 0;

	/*
	 * Copy session structure and link into list 
	 */
	slp = (struct session_list *) calloc(1, sizeof(struct session_list));
	if(NULL == slp) {
		in_session->s_manage_errno = APNP_ERR_MALLOC_FAIL;
		return NULL;
	}

	slp->transport = NULL;
	slp->session = (apnp_session *)calloc(1, sizeof(apnp_session));
	if(NULL == slp->session) {
		apnp_sess_close(slp);
		in_session->s_manage_errno = APNP_ERR_MALLOC_FAIL;
		return NULL;
	}

	slp->session->message_type = in_session->message_type;
	slp->session->sessid = ++global_sessid;
	slp->session->flags = in_session->flags;
	slp->session->callback = in_session->callback;
	//slp->session->Method_s = in_session->Method_s;
	
	//if(in_session->local && in_session->local_len) {
	//	slp->session->local = calloc(1, in_session->local_len);
	//	if(NULL == slp->session->local) {
	//		apnp_sess_close(slp);
	//		in_session->s_manage_errno = APNP_ERR_MALLOC_FAIL;
	//		return NULL;
	//	}
	//	memcpy(slp->session->local, in_session->local, in_session->local_len);
	//}
	
	//if(in_session->remote && in_session->remote_len) {
	//	slp->session->remote = calloc(1, in_session->remote_len);
	//	if(NULL == slp->session->remote) {
	//		manage_sess_close(slp);
	//		in_session->s_manage_errno = MANAGEERR_MALLOC_FAIL;
	//		return NULL;
	//	}
	//	memcpy(slp->session->remote, in_session->remote, in_session->remote_len);
	//}
	
	return slp;
}

static void *
_sess_add(apnp_session *in_session, 
				apnp_transport *transport) {
	struct session_list *slp = NULL;
	
	//_init_manage();

	if(NULL == transport) {
		ApnpDebugLog("_sess_add: input transport is NULL\n");
		return NULL;
	}
	
	if(NULL == in_session) {
		ApnpDebugLog("_sess_add: input in_session is NULL\n");
		transport->f_close(transport);
		apnp_transport_free(transport);
		return NULL;
	}

	if(NULL == (slp = _sess_copy(in_session))) {
		ApnpDebugLog("_sess_add: _sess_copy in_session fail\n");
		transport->f_close(transport);
		apnp_transport_free(transport);
		return NULL;
	}
	
	slp->transport = transport;
	
	
	return (void *)slp;
}


static void *
apnp_sess_open(apnp_session *in_session) {
	apnp_transport *transport = NULL;
//	struct apnp_internal_session *interval = NULL;

	in_session->s_manage_errno = 0;
	in_session->s_errno = 0;

	//_init_manage();

	ApnpDebugLog("apnp_sess_open: in_session->flags (%x)\n", in_session->flags);

	if(in_session->flags & APNP_FLAGS_NETLINK_SOCKET){
		ApnpDebugLog("apnp_sess_open: create netlink transport\n");
		transport = apnp_transport_open(APNP_NETLINK_DOMAIN, in_session->flags);
	}/*add by dt*/
	else if(in_session->flags & APNP_FLAGS_STREAM_SOCKET){
		ApnpDebugLog("apnp_sess_open: create tcp transport\n");
		transport = apnp_transport_open(APNP_TCP_DOMAIN, in_session->flags);
	}
	else if(in_session->flags & APNP_FLAGS_UDP_SOCKET){
		ApnpDebugLog("apnp_sess_open: create udp transport\n");
		transport = apnp_transport_open(APNP_UDP_DOMAIN, in_session->flags);
						

	}

	if(transport == NULL) {
		ApnpDebugLog("_sess_open: can not interpret local\n");
		in_session->s_manage_errno = APNP_ERR_BAD_ADDRESS;
		in_session->s_errno = errno;
		return NULL;
	}
	
	return _sess_add(in_session, transport);
}

/*
static void *
apnp_sess_udp_open(apnp_session *in_session) {
	apnp_transport *transport = NULL;
//	struct apnp_internal_session *interval = NULL;

	in_session->s_manage_errno = 0;
	in_session->s_errno = 0;

	//_init_manage();

	fprintf(stdout,"apnp_sess_open: in_session->flags (%x)\n", in_session->flags);

	if(in_session->flags & APNP_FLAGS_UDP_SOCKET){
		printf("apnp_sess_open: create netlink transport\n");
		transport = apnp_transport_open(APNP_UDP_DOMAIN, in_session->flags);
	}

	if(transport == NULL) {
		printf("_sess_open: can not interpret local\n");
		in_session->s_manage_errno = APNP_ERR_OPEN_FAIL;
		in_session->s_errno = errno;
		return NULL;
	}
	
	return _sess_add(in_session, transport);
}

*/
apnp_session *
apnp_open(apnp_session * session) {
	struct session_list *slp = NULL;

	if(NULL == session) {
		ApnpDebugLog("apnp_open: input session is NULL\n");
		return NULL;
	}
	
	slp = (struct session_list *)apnp_sess_open(session);
	if(NULL == slp) {
		ApnpDebugLog("apnp_open: apnp_sess_open fail\n");
		return NULL;
	}
	
	pthread_mutex_lock(&session_mutex);	
	//slp->next = Sessions;
	//Sessions = slp;
	list_add(&(slp->session_node),&sessions_head);
	pthread_mutex_unlock(&session_mutex);	
	
	return (slp->session);
}
/*
apnp_session *
apnp_udp_open(apnp_session * session) {
	struct session_list *slp = NULL;

	if(NULL == session) {
		printf("apnp_open: input session is NULL\n");
		return NULL;
	}
	
	slp = (struct session_list *)apnp_sess_udp_open(session);
	if(NULL == slp) {
		printf("apnp_open: apnp_sess_open fail\n");
		return NULL;
	}
	
	pthread_mutex_lock(&session_mutex);	
	slp->next = Sessions;
	Sessions = slp;
	pthread_mutex_unlock(&session_mutex);	
	
	return (slp->session);
}
*/

int
apnp_service_restart(uint32_t ip_addr)
{
	char ipstr[32] = { 0 };
	int ret = UPNP_E_SUCCESS;
	device_stop();
	ApnpDebugLog("apnp_service_restart...\n");
	ip2str(ip_addr, ipstr, sizeof(ipstr));
		
	ret = device_start(ipstr, 49900);
	
	if (UPNP_E_SUCCESS != ret) {
		return ret;
	}
	return ret;
}


void
_apnp_tdomain_init(){
	if (init_tdomain_init_done) {
        return;
    }

    init_tdomain_init_done = 1;
	apnp_tdomain_init();
}


