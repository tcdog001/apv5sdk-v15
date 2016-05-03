/*
*******************************************************
* FileName:detector.c
* Athor:GengZongJie
* Date:2013-7-22
*******************************************************
*/



#include "detector.h"
#include <errno.h>

#ifndef __cplusplus
#define __cdecl
#endif

/*
global variable declaration
*/
scan_locate_parm_detector_common detector_common_params_11g;
scan_locate_parm_detector_common detector_common_params_11a;
scan_locate_parm_detector_rfid detector_rfid_params_11g;
scan_locate_parm_detector_rfid detector_rfid_params_11a;
scan_locate_parm_detector_wifipos detector_wifipos_params_11g;
scan_locate_parm_detector_wifipos detector_wifipos_params_11a;
scan_locate_parm_detector_rrm detector_rrm_params_11g;
scan_locate_parm_detector_rrm detector_rrm_params_11a;

scan_locate_parm_driver_common driver_common_params;
scan_locate_parm_driver_rfid driver_rfid_params;
scan_locate_parm_driver_wifipos driver_wifipos_params;
scan_locate_parm_driver_rrm driver_rrm_params;

detector_nlmsg config_parm_from_wtpd;
detector_nlmsg config_parm_to_scanner;
detector_nlmsg scan_result_from_scanner;
detector_nlmsg scan_result_to_wtpd;

scan_locate_res_wifipos_chain_node *g_wifipos_result_chain_head = NULL;
u_int16_t g_wifipos_result_chain_node_count = 0;

scan_locate_chan_node *g_11g_chan_head = NULL;
scan_locate_chan_node *g_11a_chan_head = NULL;

int nlsock_to_wtpd = 0;
int nlsock_to_scanner = 0;
int wifipos_udp_sockfd = 0;
int rfid_tcp_sockfd = 0;

pthread_mutex_t detector_mutex;
pthread_mutex_t detector_mutex_parm;

void __cdecl signal_handler(int);

pthread_t hand_scan_task_11g;	//2.4G
pthread_t hand_scan_task_11a;	//5.8G
pthread_t send_report_tid;

int detector_debug_lvl = 0;
int debug_print_screen = 0;

//static DETECTOR_STATE detector_state = INIT;
DETECTOR_STATE detector_state = INIT;

u_int8_t ap_mac[MAC_ADDR_LEN];

unsigned long scanner_result_counter = 0;
unsigned long server_result_counter = 0;

#define DETECTOR_LOG_FILENAME "/tmp/detector.log"
int g_max_detector_logfile_size = 300000;
static FILE* g_detector_logfile = NULL;
#ifndef DETECTOR_SINGLE_THREAD
pthread_mutex_t g_logfile_mutex;
#endif

#define DET_TRUE 1
#define DET_FALSE 0
int isHotRestart = DET_FALSE;
int detector_state_bak = INIT;
#define DETECTOR_BAKINFO_11G "/tmp/detector_bakup_11G.bin"
#define DETECTOR_BAKINFO_11A "/tmp/detector_bakup_11A.bin"

#define RADIO_11G 0
#define RADIO_11A 1
u_int8_t params_changed_11g = 0;	//params update flag on 11g
u_int8_t params_changed_11a = 0; //params update flag on 11a


/*
***********************************************************************
* FuncName:main
* Description:main function for scan locate,which will send the params to scanner ,
				reciev scan locate result and send the result to ac or server.
************************************************************************
*/
int main(int argc, char **argv)
{
	if (!strcmp(argv[1], "-D"))
	{
		detector_debug_lvl = atoi(argv[2]);
	}
	if (!strcmp(argv[3], "-T"))
	{
		debug_print_screen = atoi(argv[4]);
	}
	if (0 == debug_print_screen)	//printf debug log to screen
	{
		printf("do not print debug message to screen\n");
	}

	if (7 <= argc)
	{
	   if (!strcmp(argv[5], "-R"))
	   {
			isHotRestart = atoi(argv[6]);
			if (isHotRestart != DET_FALSE)
			{
				isHotRestart = DET_TRUE;
			}
	   }
	}
	
	detector_log_init_file();

	//signal(SIGINT, &signal_handler);
	//signal(SIGKILL, &signal_handler);
	//signal(SIGSEGV, &signal_handler);
	
	get_ap_mac_addr();
	detector_debug(DET_LOG_NOTICE, 
				   "ap mac is %02x:%02x:%02x:%02x:%02x:%02x\n",
				   ap_mac[0],ap_mac[1],ap_mac[2],ap_mac[3],ap_mac[4],ap_mac[5]);
	
	if (pthread_mutex_init(&detector_mutex, NULL))
	{
		detector_debug(DET_LOG_ALERT,
			"init detector_mutex failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		exit(0);
	}

	//comming into detector finite state machine
	detector_debug(DET_LOG_ALERT,
		"detector is going into finite state machine now.\n");
	while (1)
	{
		switch (detector_state)
		{
			case INIT:
				detector_debug(DET_LOG_ALERT, 
					"detector come into init state.\n");
				sub_thread_kill();
				des_wlan_all();
				//detector go into init state
				memset(&config_parm_from_wtpd, 0, sizeof(config_parm_from_wtpd));
				memset(&config_parm_to_scanner, 0, sizeof(config_parm_to_scanner));
				memset(&scan_result_from_scanner, 0, sizeof(scan_result_from_scanner));
				memset(&scan_result_to_wtpd, 0, sizeof(scan_result_to_wtpd));
				memset(&detector_common_params_11g, 0, sizeof(detector_common_params_11g));
				memset(&detector_common_params_11a, 0, sizeof(detector_common_params_11a));
				memset(&detector_rfid_params_11g, 0, sizeof(detector_rfid_params_11g));
				memset(&detector_rfid_params_11a, 0, sizeof(detector_rfid_params_11a));
				memset(&detector_wifipos_params_11g, 0, sizeof(detector_wifipos_params_11g));
				memset(&detector_wifipos_params_11a, 0, sizeof(detector_wifipos_params_11a));
				memset(&detector_rrm_params_11g, 0, sizeof(detector_rrm_params_11g));
				memset(&detector_rrm_params_11a, 0, sizeof(detector_rrm_params_11a));
				//memset(&driver_common_params, 0, sizeof(driver_common_params));
				
				//set up sockets to wtpd 
				if (0 == nlsock_to_wtpd)
				{
					nlsock_to_wtpd = nlsock_create_to_wtpd();	
					if (nlsock_to_wtpd < 0)
					{
						detector_debug(DET_LOG_ERR, 
									   "build the netlink socket to wtpd failded! nlsock_to_wtpd: %d\n",
									   nlsock_to_wtpd);
						exit(0);
					}
					else
					{
						detector_debug(DET_LOG_INFO, 
									   "build the netlink socket to wtpd ok! nlsock_to_wtpd: %d\n",
									   nlsock_to_wtpd);
					}
				}
			
				//go to config state directly if detector is hot restart
				if (DET_TRUE == isHotRestart)
				{
					detector_state = CONFIG;
					detector_debug(DET_LOG_INFO,
						"detector state change: %d\n",
						detector_state);
					break;
				}
				
				//normal start or hot restart
				while (1)
				{
					if ((1 == listening_sockets(nlsock_to_scanner, nlsock_to_wtpd)) ||
						(DET_TRUE == isHotRestart))
					{
						detector_state = CONFIG;
						detector_debug(DET_LOG_INFO,
							"detector state change: %d\n",
							detector_state);
						break;
					}
				}
				break;
			case CONFIG:
				detector_debug(DET_LOG_ALERT,
					"detector come into config state.\n");
				if (DET_TRUE == isHotRestart)	//hot restart
				{
					detector_debug(DET_LOG_NOTICE, 
						"detector hot restart!!!.\n");
					if (det_get_bakupinfo())
					{
						detector_debug(DET_LOG_ERR,
							"hot restart, get bakup info failed.\n");
						break;
					}
					
					isHotRestart = DET_FALSE;
				}
				else 
				{
					//det_del_bakupinfo();
					if (recv_scan_locate_parm(nlsock_to_wtpd))	//normal start or config msg from wtpd
					{
						detector_debug(DET_LOG_ERR,
							"recv scan locate parm error!\n");
						break;
					}
					else
					{
						//det_del_bakupinfo();
						sub_thread_kill();
						des_wlan_all();
						det_set_bakupinfo();	//will rewrite bakupinfor file when file is exiting?
						
						detector_debug(DET_LOG_ALERT,
							"recv scan locate parm from wtpd successfully.\n");
						if ((0 == detector_common_params_11g.main_switch)&&
							(0 == detector_common_params_11a.main_switch))
						{
							detector_state = INIT;
							detector_debug(DET_LOG_INFO,
								"detector state change: %d\n",
								detector_state);
							break;
						}
					}
				}
				//setup netlink to scanner
				if (0 == nlsock_to_scanner) //nlsock to scanner has not been set up
				{
					nlsock_to_scanner = nlsock_create_to_scanner(); //build the netlink sock to scanner
					if (nlsock_to_scanner < 0)
					{
						detector_debug(DET_LOG_ERR, 
							"build the netlink socket to scanner failded!nlsock_to_scanner: %d\n",
							nlsock_to_scanner);
						exit(0);
					}
					else
						detector_debug(DET_LOG_NOTICE, 
						"build the netlink socket to scanner ok.nlsock_to_scanner: %d\n",
						nlsock_to_scanner);
				}
				//execute scan locate params
				detector_debug(DET_LOG_NOTICE, "go to execute scan locate config.\n");
				if (0 == det_exec_scan_locate_param())
				{
					//change parm change flag here
					if (DET_TRUE == params_changed_11g)
					{
						params_changed_11g = DET_FALSE;
						detector_debug(DET_LOG_ALERT, 
							"the scan locate params on radio 0 has been updated.\n");
					}
					if (DET_TRUE == params_changed_11a)
					{
						params_changed_11a = DET_FALSE;
						detector_debug(DET_LOG_ALERT, 
							"the scan locate params on radio 1 has been updated.\n");
					}
					detector_state = RUN;
					detector_debug(DET_LOG_INFO,
						"detector state change: %d\n",
						detector_state);
				}
				break;

			case RUN:
				detector_debug(DET_LOG_ALERT,
					"detector come into run state.\n");
				while (1)
				{
					#if 0
					if (2 == listening_sockets(nlsock_to_scanner, nlsock_to_wtpd))	//recieve msg from scanner
					{
						detector_debug(DET_LOG_DEBUG,
							"scan locate result comming.\n");
						recv_scan_locate_result(nlsock_to_scanner);
						detector_debug(DET_LOG_INFO, 
							"listening_sockets: Handle driver message finish. counter = %d\n", 
							scanner_result_counter);
					}
					else if (1 == listening_sockets(nlsock_to_scanner, nlsock_to_wtpd)) //recieve msg from wtpd
					#endif

					if (1 == listening_sockets(nlsock_to_scanner, nlsock_to_wtpd)) //recieve msg from wtpd
					{
						detector_debug(DET_LOG_ALERT,
							"recieve msg from wtpd.\n");

						detector_state = CONFIG;	//update wifipos params
						detector_debug(DET_LOG_INFO,
							"detector state change: %d\n",
							detector_state);
						break;
					}
				}
				break;
			default:
				detector_debug(DET_LOG_ALERT, 
					"unknown state, detector go into init state.\n");
				detector_state = INIT;
				detector_debug(DET_LOG_INFO,
					"detector state change: %d\n",
					detector_state);
				break;
		}
	}
	return 0;
}

/*
*************************************************
* FuncName:listening_sockets
* Description:listen the sockets to scanner,wtpd and server
*************************************************************
*/
int listening_sockets(int nlsock_to_scanner, int nlsock_to_wtpd)
{
	int max_sock = -1;
	int ret = 0;
	fd_set msg_fd_set;
	
	if (nlsock_to_scanner < nlsock_to_wtpd)
		max_sock = nlsock_to_wtpd;
	else 
		max_sock = nlsock_to_scanner;
	
	detector_debug(DET_LOG_DEBUG, 
		"nlsock_to_wtpd:%d, nlsock_to_scanner:%d, max_sock: %d\n", 
		nlsock_to_wtpd, nlsock_to_scanner, max_sock);

	FD_ZERO(&msg_fd_set);
	if (RUN == detector_state)
	{
		DETECTOR_FD_SET(nlsock_to_scanner, &msg_fd_set);
	}
	DETECTOR_FD_SET(nlsock_to_wtpd, &msg_fd_set);

	ret = select(max_sock+1, &msg_fd_set, NULL, NULL, NULL);
	if (ret < 0)
	{
		detector_debug(DET_LOG_ERR, 
			"recieve message error. errno:%d,%s\n",
			errno,
			strerror(errno));
	}
	detector_debug(DET_LOG_INFO, 
		"recieve message ok.\n");
	if (FD_ISSET(nlsock_to_scanner, &msg_fd_set))
	{
		scanner_result_counter++;
		detector_debug(DET_LOG_DEBUG,
			"scan locate result comming.\n");
		recv_scan_locate_result(nlsock_to_scanner);
		detector_debug(DET_LOG_INFO, 
			"listening_sockets: Handle driver message finish. counter = %d\n", 
			scanner_result_counter);
		//return 2;
	}
	if (FD_ISSET(nlsock_to_wtpd, &msg_fd_set))
	{			
		detector_debug(DET_LOG_NOTICE, 
			"recieve message from wtpd.\n");
		return 1;

	}
	return -1;
}

/*
**************************************************
* FuncName:det_exec_scan_locate_param
* Description:execute scan locate parm after recv
* Return:0 success.
**************************************************
*/
int det_exec_scan_locate_param(void)
{
	//check parm of 11g
	if (0 == detector_wifipos_params_11g.func_switch) /* disable wifi position */
	{
		detector_debug(DET_LOG_ALERT, "wifipos function on radio 0 is going to exit now!\n");
		scan_locate_system_exit(RADIO_11G);	//radio 0
	}
	else
	{
		detector_debug(DET_LOG_ALERT, "wifipos function on radio 0 is going to start now!\n");
		scan_locate_system_start(RADIO_11G); //radio 0
	}

	//check parm of 11a
	if (0 == detector_wifipos_params_11a.func_switch) /* disable wifi position */
	{
		detector_debug(DET_LOG_ALERT, "wifipos function on radio 1 is going to exit now!\n");
		scan_locate_system_exit(RADIO_11A);	//radio 1
	}
	else
	{
		detector_debug(DET_LOG_ALERT, "wifipos function on radio 1 is going to start now!\n");
		scan_locate_system_start(RADIO_11A); //radio 1
	}
	
	return 0;
}

/*
********************************************
* FuncName:scan_locate_system_start
* Description:start scan locate function here
********************************************
*/
int scan_locate_system_start(u_int8_t radio_id)
{
    int error = 0;
	scan_locate_parm_detector_wifipos wifipos_parm_changed;
	scan_locate_parm_detector_wifipos wifipos_parm_another;
	scan_locate_parm_detector_common common_parm_changed;
	scan_locate_parm_detector_common common_parm_another;
	/*
    if (RUN == detector_state)
    {
        detector_debug(DET_LOG_WARNING, "wifipos func is already enabled, ignore this enalbe message.\n");
        return 0;
    }
    */
	if (RADIO_11G == radio_id)
	{
		wifipos_parm_changed = detector_wifipos_params_11g;
		wifipos_parm_another = detector_wifipos_params_11a;
		common_parm_changed = detector_common_params_11g;
		common_parm_another = detector_common_params_11a;
	}
	else 
	{
		wifipos_parm_changed = detector_wifipos_params_11a;
		wifipos_parm_another = detector_wifipos_params_11g;
		common_parm_changed = detector_common_params_11a;
		common_parm_another = detector_common_params_11g;
	}
	if (0 == wifipos_udp_sockfd)
	{
    	error = det_init_det_to_svr_sock(wifipos_parm_changed);
	    if (error < 0)
	    {
	        detector_debug(DET_LOG_ERR, 
	        "init socket used to send message from detector to server failed!\n");
	    }
		else
		{
			detector_debug(DET_LOG_INFO,
				"init socket of detector to server ok.\n");
		}
	}
	error = setup_scan_vap(radio_id);	
	if (error < 0)
	{
		detector_debug(DET_LOG_ERR, 
			"setup scan vap failed!\n");
	}
	error = set_func_flag_to_scanner((common_parm_changed.main_switch&WIFIPOS_FUNC_MASK)>>1, radio_id);
	if (error < 0)
	{
		detector_debug(DET_LOG_ERR, "set scan locate flag to scanner failed!\n");
	}
	else
	{
		detector_debug(DET_LOG_INFO, "set scan locate flag to scanner successfully.\n"); 
	}
	
	//build two thread here to deal with scan task and send udp message task
	if (RADIO_11G == radio_id)
	{
		if( (hand_scan_task_11g != 0) && (0 == pthread_kill(hand_scan_task_11g, 0) ) ) 
		{
			pthread_cancel(hand_scan_task_11g);
		}			
		if (detector_pthread_create(&hand_scan_task_11g, deal_with_scan_task_11g, NULL))
			detector_debug(DET_LOG_ERR, 
				"pthread create deal_with_scan_task_11g failed. errno:%d,%s\n",
				errno,
				strerror(errno));
		else
		{
			detector_debug(DET_LOG_ALERT, 
				"pthread create deal_with_scan_task_11g ok.hand_scan_task_11g: %d\n",
				hand_scan_task_11g);
		}
	}
	else 
	{
		if( (hand_scan_task_11a != 0) && (0 == pthread_kill(hand_scan_task_11a, 0) ) ) 
		{
			pthread_cancel(hand_scan_task_11a);
		}			
		if (detector_pthread_create(&hand_scan_task_11a, deal_with_scan_task_11a, NULL))
			detector_debug(DET_LOG_ERR, 
			"pthread create deal_with_scan_task_11a failed. errno:%d,%s\n",
			errno,
			strerror(errno));
		else
			detector_debug(DET_LOG_ALERT, 
			"pthread create deal_with_scan_task_11a ok.hand_scan_task_11a: %d\n",
			hand_scan_task_11a);
	}

	if( (send_report_tid != 0) && (0 == pthread_kill(send_report_tid, 0) ) ) 
	{
		pthread_cancel(send_report_tid);
	}			
	if (detector_pthread_create(&send_report_tid, send_udp_msg_periodic, NULL))
		detector_debug(DET_LOG_ERR, 
		"pthread create send_udp_msg_periodic failed. errno:%d,%s\n",
		errno,
		strerror(errno));
	else
		detector_debug(DET_LOG_ALERT, 
		"pthread create send_udp_msg_periodic ok.send_report_tid: %d\n",
		send_report_tid);
	
	return 0;
}

/*
*****************************************
* FuncName:det_init_det_to_svr_sock
* Description: init the sock to server
* Return:0 success, -1 failure.
*****************************************
*/
int det_init_det_to_svr_sock(scan_locate_parm_detector_wifipos detector_wifipos_parm)
{
    if (1 == detector_wifipos_parm.server.server_type)	//send to server
	{
		if (1 == detector_wifipos_parm.server.deliver_mode)	//udp mode
		{
			wifipos_udp_sockfd = wifipos_udp_sock_create();
			if (wifipos_udp_sockfd < 0)
			{
				detector_debug(DET_LOG_ERR, 
				"create udp socket to server failed.wifipos_udp_sockfd: %d.\n",
				wifipos_udp_sockfd);
				return -1;
            }
            else
            {
			    detector_debug(DET_LOG_INFO, 
			    "create udp socket to server ok.wifipos_udp_sockfd:%d\n", 
			    wifipos_udp_sockfd);
			    return 0;
            }
		}
	}

	return -1;
}

/*
***********************************************************
* FuncName:recv_scan_locate_parm
* Description:recv scan params from wtpd,update scan params
				in detector and restart scan locate task
* Return:0 success, -1 failure.
***********************************************************
*/
int recv_scan_locate_parm(int nlsock_to_wtpd)
{
	detector_debug(DET_LOG_DEBUG, "comming into recv_scan_locate_parm.\n");
	int recv_status = 0;
	int error = 0;
	struct sockaddr from_addr;
	socklen_t addr_len;
	addr_len = sizeof(struct sockaddr_nl);
	
	/* gengzj modified for coverity check 2013-10-29 */
	memset(&config_parm_from_wtpd.buf, 0, MAX_MSG_PAYLOAD);
	/* gengzj added end */
	
	recv_status = recvfrom(nlsock_to_wtpd, &config_parm_from_wtpd, 
		sizeof(config_parm_from_wtpd), 0, (struct sockaddr *)&from_addr, &addr_len);
	if (recv_status < 0)
	{
		detector_debug(DET_LOG_ERR, 
			"recv config params from wtpd failed! socket:%d, errno:%d,%s\n",
			nlsock_to_wtpd,
			errno,
			strerror(errno));
		return -1;
	}
	else
	{
		detector_debug(DET_LOG_INFO, 
			"recv scan locate parm ok, recv_status: %d\n", recv_status);
		error = parse_netlink_msg(config_parm_from_wtpd.buf);
		if (error < 0)
		{
			detector_debug(DET_LOG_ERR, 
				"parse config msg from wtpd failed,continue.\n");
			return -1;
		}
		else
		{
		    detector_debug(DET_LOG_INFO, 
				"parse_netlink_msg ok.\n");
		    return 0;
		}
	}
	
}


/*
***********************************************************
* FuncName:deal_with_scan_task_11g
* Description:handle the scan task in a independet thread
* Return: none.
***********************************************************
*/
void* deal_with_scan_task_11g(void *arg)
{
	detector_debug(DET_LOG_DEBUG, "comming into deal_with_scan_task_11g.\n");
	scan_locate_chan_node *cur_chan_node = NULL;
	scan_locate_chan_node *tmp_chan_node = NULL;
	u_int8_t chan_to_scan = 0;
	u_int16_t scan_interval = 0;
	u_int16_t listen_time = 0;
	
	/* gengzj added for setting scan mode to passive scan 2013-10-18 */
	char cmd_buf[CMD_BUF_SIZE];
	u_int8_t radio_id = RADIO_11G;
	
	memset(cmd_buf, 0, CMD_BUF_SIZE);
	sprintf(cmd_buf, "echo 1 > /proc/sys/dev/wifi%d/iwlist_scan_mode", radio_id);
	detector_debug(DET_LOG_INFO, "echo 1 > /proc/sys/dev/wifi%d/iwlist_scan_mode\n", radio_id);
	detector_system(cmd_buf);
	/* gengzj added end */
	
	while (1)
	{
		detector_debug(DET_LOG_INFO, 
			"comming into deal_with_scan_task_11g.\n");
		cur_chan_node = detector_common_params_11g.chan_list;
		detector_debug(DET_LOG_DEBUG,
			"detector_common_params_11g.chan_list: %p\n",
			detector_common_params_11g.chan_list);
		scan_interval = detector_common_params_11g.scan_period_each_chan;	//ms
		listen_time = detector_common_params_11g.listen_time_each_chan; //ms
		if (NULL == cur_chan_node)
		{
			detector_debug(DET_LOG_CRIT,
				"11g chan list head is NULL! thread deal_with_scan_task_11g exit!\n");
			pthread_exit((void *) 0);
		}

		if (cur_chan_node != NULL)
		{
		    while (cur_chan_node != NULL)
		    {
			    chan_to_scan = cur_chan_node->node.chan;
				
			    detector_debug(DET_LOG_INFO, 
					"chan_to_scan: %d\n",
					chan_to_scan);
			    if (cur_chan_node->node.flag&WIFIPOS_FUNC_MASK)
			    {
				    hand_scan_task(chan_to_scan, listen_time, RADIO_11G);
			    }
			    cur_chan_node = cur_chan_node->next;
			    detector_debug(DET_LOG_INFO, "sleep %d\n",scan_interval);

			    //change "sleep" to "usleep"
			    usleep(scan_interval * 1000);
		    }
		}
	}
	
	return;
}

/*
***********************************************************
* FuncName:deal_with_scan_task_11a
* Description:handle the scan task in a independet thread
* Return: none.
***********************************************************
*/
void* deal_with_scan_task_11a(void *arg)
{
	detector_debug(DET_LOG_DEBUG, "comming into deal_with_scan_task_11a.\n");
	scan_locate_chan_node *cur_chan_node = NULL;
	u_int8_t chan_to_scan = 0;
	u_int16_t scan_interval = 0;
	u_int16_t listen_time = 0;
	
	/* gengzj added for setting scan mode to passive scan 2013-10-18 */
	char cmd_buf[CMD_BUF_SIZE];
	u_int8_t radio_id = RADIO_11A;
	
	memset(cmd_buf, 0, CMD_BUF_SIZE);
	sprintf(cmd_buf, "echo 1 > /proc/sys/dev/wifi%d/iwlist_scan_mode", radio_id);
	detector_debug(DET_LOG_INFO, "echo 1 > /proc/sys/dev/wifi%d/iwlist_scan_mode\n", radio_id);
	detector_system(cmd_buf);
	/* gengzj added end */

	while (1)
	{
		detector_debug(DET_LOG_INFO, 
			"comming into deal_with_scan_task_11a.\n");
		
		cur_chan_node = detector_common_params_11a.chan_list;
		scan_interval = detector_common_params_11a.scan_period_each_chan;	//ms
		listen_time = detector_common_params_11a.listen_time_each_chan; //ms
		if (NULL == cur_chan_node)
		{
			detector_debug(DET_LOG_CRIT,
				"11a chan list head is NULL! thread deal_with_scan_task_11a exit!\n");
			pthread_exit((void *) 0);
		}
		if (cur_chan_node != NULL)
		{
		    while (cur_chan_node != NULL)
		    {
			    chan_to_scan = cur_chan_node->node.chan;
			    detector_debug(DET_LOG_INFO, 
					"chan_to_scan: %d\n",
					chan_to_scan);
			    if (cur_chan_node->node.flag&WIFIPOS_FUNC_MASK)
			    {
				    hand_scan_task(chan_to_scan, listen_time, RADIO_11A);
			    }
			    cur_chan_node = cur_chan_node->next;
			    detector_debug(DET_LOG_INFO, "sleep %d\n",scan_interval);

			    //change "sleep" to "usleep"
			    usleep(scan_interval * 1000);
		    }
		}
	}
	
	return;
}


/*
**************************************************
* FuncName:get_ap_mac_addr
* Description:get the mac addr of ap
* Return:0 success, -1 failure.
**************************************************
*/
int get_ap_mac_addr(void)
{
	int tmp_i = 0;
	int get_mac = 0;
	int try_time = 3;
	for (tmp_i = 0; tmp_i < try_time; tmp_i ++)
	{
		get_mac = get_ap_mac();
		if (get_mac < 0)
		{
			detector_debug(DET_LOG_WARNING, "get ap mac failed.\n");
			continue;
		}
		else
		{
			detector_debug(DET_LOG_INFO, "get ap mac successful.\n");
			break;
		}
	}
	if (get_mac < 0)
	{
		detector_debug(DET_LOG_CRIT, "failed to get ap mac for 3 times!\n");
		return -1;
	}

	return 0;
}


/*
***************************************************
* FuncName:recv_scan_locate_result
* Description:receive scan locate result from scanner
* Return: none.
*****************************************************
*/
void recv_scan_locate_result(int nlsock_to_scanner)
{
	detector_nlmsg scan_result_from_scanner;
	int recv_status = 0;
	struct sockaddr from_addr;
	socklen_t addr_len;
	int ret = 0;

	addr_len = sizeof(struct sockaddr_nl);

	if (nlsock_to_scanner < 0)
	{
		detector_debug(DET_LOG_ERR, "netlink socket to scanner is invalid!\n");
		exit(0);
	}

	if (RUN == detector_state)
	{
		memset(&scan_result_from_scanner, 0, sizeof(scan_result_from_scanner));
		recv_status = recvfrom(nlsock_to_scanner, &scan_result_from_scanner, 
			sizeof(scan_result_from_scanner), 0, (struct sockaddr *)&from_addr, &addr_len);
		if (recv_status < 0)
		{
			detector_debug(DET_LOG_ERR, 
				"recv nlsock to scanner status is error. socket:%d, errno:%d,%s\n", 
				nlsock_to_scanner,
				errno,
				strerror(errno));
			return;
		}
		ret = parse_netlink_msg(scan_result_from_scanner.buf);
		detector_debug(DET_LOG_INFO, "parse_netlink_msg is OK.\n");
	}
	
	return;
}

/*
**************************************************
* FuncName:deal_result_from_scanner
* Description:deal with the scan locate 
				results(netlink message) 
				come from scanner
* Return:0 success, -1 failure.
**************************************************
*/
int parse_scan_locate_result(u_int8_t *ptr, u_int8_t func_subtype, u_int16_t length)
{

	if (NULL == ptr)
	{
		detector_debug(DET_LOG_CRIT, "scan result from scanner is invalid.\n");
		return -1;
	}

	if (RFID_FUNC_MASK == func_subtype)
	{
		//parse rfid netlink attribute body message
	}
	else if (WIFIPOS_FUNC_MASK == func_subtype)
	{
		//parse wifipos netlink attribute body message
		parse_wifipos_result(ptr, length);	
	}
	else if (RRM_FUNC_MASK == func_subtype)
	{
		//parse rrm netlink attribute body message
	}
	else
		detector_debug(DET_LOG_ERR,
			"unknown func subtype: %d",
			func_subtype);
	
	return 0;
}

/*
*******************************************************
* FuncName:parse_wifipos_result
* Description:parse wifipos netlink attribute body message
* Return: none.
***********************************************************
*/
void parse_wifipos_result(u_int8_t *ptr, u_int16_t length)
{
	scan_locate_res_wifipos_node *temp_node = NULL;
	int node_add_result = 0;
	int tmp_len = 0;
	int total_len = 0;
	
	if (NULL == ptr)
	{
		detector_debug(DET_LOG_CRIT, "wifipos result attribute body ptr is NULL!\n");
		return;
	}
	
	temp_node = (scan_locate_res_wifipos_node *)malloc(sizeof(scan_locate_res_wifipos_node));
	if (NULL == temp_node)
	{
		detector_debug(DET_LOG_EMERG, "malloc for scan locate result temp node failed,return.\n");
		return;
	}
	memset(temp_node, 0, sizeof(scan_locate_res_wifipos_node));
	while (total_len < length)
	{
		switch (*ptr)
		{
			case WIFIPOS_RES_NLBD_TOA_TYPE:	//toa
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				memcpy(&(temp_node->toa), ptr, sizeof(u_int64_t)); 
				detector_debug(DET_LOG_DEBUG, "toa: %llu\n", temp_node->toa);
				break;
			case WIFIPOS_RES_NLBD_SRCMAC_TYPE:	//src mac
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				memcpy(temp_node->src_mac, ptr, MAC_ADDR_LEN);
				detector_debug(DET_LOG_DEBUG, "src_mac: %02x:%02x:%02x:%02x:%02x:%02x\n", temp_node->src_mac[0],temp_node->src_mac[1],temp_node->src_mac[2],temp_node->src_mac[3],temp_node->src_mac[4],temp_node->src_mac[5]);
				break;
			case WIFIPOS_RES_NLBD_RSSI_TYPE:	//rssi
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				temp_node->rssi = *ptr;
				detector_debug(DET_LOG_DEBUG, "rssi: %d\n", temp_node->rssi);
				break;
			case WIFIPOS_RES_NLBD_STACHAN_TYPE:	//sta channel
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				temp_node->sta_chan = *ptr;
				detector_debug(DET_LOG_DEBUG, "sta_chan: %d\n", temp_node->sta_chan);
				break;
			case WIFIPOS_RES_NLBD_RADIOID_TYPE:	//radio_id
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				temp_node->radio_id = *ptr;
				detector_debug(DET_LOG_DEBUG, "radio_id: %d\n", temp_node->radio_id);
				break;
			default:
				detector_debug(DET_LOG_DEBUG, "unknown type in scan locate result message.\n");
				break;
		}
		ptr += tmp_len;
		total_len += (tmp_len + TLV_MSG_TYPE_LEN + TLV_MSG_LENGTH_LEN);
	}
		
	node_add_result = result_chain_node_add(temp_node);
	if (node_add_result < 0)
	{
		detector_debug(DET_LOG_ERR, "add node to chain failed.\n");
	}
	else
	{
	    detector_debug(DET_LOG_INFO, "add node to chain successfully.\n");
	}

	free(temp_node);
	temp_node = NULL;
	
	return;
}

/*
****************************************************
* FuncName:result_chain_node_add
* Description:add the temp node to the result chain
* Return: 0 success, -1 failure.
****************************************************
*/
int result_chain_node_add(scan_locate_res_wifipos_node *result_node)
{
	int error = 0;
	
	pthread_mutex_lock(&detector_mutex);
	error = node_add(result_node);
	if (error < 0)
	{
		detector_debug(DET_LOG_ERR, "add result node to chain result failed.\n");
	}
	pthread_mutex_unlock(&detector_mutex);
	
	return error;
}


/*
****************************************************
* FuncName:node_add
* Description:add the temp node to the result chain
* Return: 0 success, -1 failure.
****************************************************
*/
int node_add(scan_locate_res_wifipos_node *result_node)
{	
	scan_locate_res_wifipos_chain_node *head = NULL;
	scan_locate_res_wifipos_chain_node *temp_chain_node = NULL;
	head = g_wifipos_result_chain_head;
	if (!result_node)
	{
		detector_debug(DET_LOG_CRIT, "result chain node is empty,return.\n");
		return -1;
	}
	
	if (RADIO_11G == result_node->radio_id)
	{
		if (result_node->rssi < detector_wifipos_params_11g.rssilimit)
		{
			detector_debug(DET_LOG_WARNING, "rssi is less than specified.discard it.\n");
			return 0;
		}
	}
	
	else 	
	{
		if (result_node->rssi < detector_wifipos_params_11a.rssilimit)
		{
			detector_debug(DET_LOG_WARNING, "rssi is less than specified.discard it.\n");
			return 0;
		}
	}

	if (NULL == g_wifipos_result_chain_head)
	{
		temp_chain_node = (scan_locate_res_wifipos_chain_node *)malloc(sizeof(scan_locate_res_wifipos_chain_node));
		
		if (NULL == temp_chain_node)
		{
			detector_debug(DET_LOG_EMERG, "malloc for temp chain node failed ,return.\n");
			return -1;
		}
		
		memset(temp_chain_node, 0, sizeof(scan_locate_res_wifipos_chain_node));
		memcpy(&(temp_chain_node->node), result_node, sizeof(scan_locate_res_wifipos_node));
		temp_chain_node->next = NULL;
		
		g_wifipos_result_chain_head = temp_chain_node;
		g_wifipos_result_chain_node_count ++;
		detector_debug(DET_LOG_DEBUG, "g_wifipos_result_chain_head is NULL, has malloced a new chain node.\n");
		return 0;
	}
	else
	{
			/* gengzj added for too much info of the same sta */
			if (0xffff == g_wifipos_result_chain_node_count)
			{
				detector_debug(DET_LOG_WARNING,
					"the result chain node count has been 65535,return.\n");
				return 0;
			}

#if AP_SCANNER_ACL			
			while (head != NULL)
			{
				if (0 == memcmp(result_node->src_mac, head->node.src_mac, MAC_ADDR_LEN))
				{
					detector_debug(DET_LOG_INFO,
						"find the same sta info. g_wifipos_result_chain_node_count: %d\n",
						g_wifipos_result_chain_node_count);
					
					if (result_node->sta_chan == head->node.sta_chan)
					{
						detector_debug(DET_LOG_INFO,
							"the same sta info with the same channel, update it.\n");
						if(result_node->toa.tv_sec == head->node.toa.tv_sec)	//same second
						{
							if ((result_node->toa.tv_usec - head->node.toa.tv_usec) < 200*1000)
							{
								detector_debug(DET_LOG_INFO,
									"the two same sta info within 200 ms, update it.\n");
								memcpy(&(head->node), result_node, sizeof(scan_locate_res_wifipos_node));
								return 0;
							}
						}
						else
						{
							if ((result_node->toa.tv_usec + 1000000 - head->node.toa.tv_usec) < 200*1000)
							{
								detector_debug(DET_LOG_INFO,
									"the two same sta info within 200 ms, update it.\n");
								memcpy(&(head->node), result_node, sizeof(scan_locate_res_wifipos_node));
								return 0;
							}							
						}
						//find the same sta info with same channel,add to result chain when time inerval over limit of 200ms
						detector_debug(DET_LOG_INFO,
							"find the same sta info with the same channel, and time interval is over limit add node. g_wifipos_result_chain_node_count: %d\n",
							g_wifipos_result_chain_node_count);
						break;
					}
					else 
					{
						detector_debug(DET_LOG_DEBUG,
							"find the same sta, but channel is not the same, goto next node. g_wifipos_result_chain_node_count: %d\n",
							g_wifipos_result_chain_node_count);
					    detector_debug(DET_LOG_DEBUG, "head = %p\n", head);
						head = head->next;
					}
				}
				else
				{
					head = head->next;
				}
			}
#endif			
		/* gengzj added end */

		//malloc a block of memory for node
		temp_chain_node = (scan_locate_res_wifipos_chain_node *)malloc(sizeof(scan_locate_res_wifipos_chain_node));
		
		if (NULL == temp_chain_node)
		{
			detector_debug(DET_LOG_EMERG, 
				"malloc for temp chain node failed ,return.\n");
			return -1;
		}
		
		memset(temp_chain_node, 0, sizeof(scan_locate_res_wifipos_chain_node));
		memcpy(&(temp_chain_node->node), result_node, sizeof(scan_locate_res_wifipos_node));
		temp_chain_node->next = g_wifipos_result_chain_head;
		g_wifipos_result_chain_head = temp_chain_node;
		g_wifipos_result_chain_node_count ++;
		detector_debug(DET_LOG_DEBUG, 
			"g_wifipos_result_chain_head = %p, g_wifipos_result_chain_node_count: %d\n",g_wifipos_result_chain_head, g_wifipos_result_chain_node_count);
		return 0;
	}
	
	return 0;

}

/*
*************************************************
* FuncName:get_ap_mac
* Description:get ap mac through sock from default
* Return: 0 success, -1 failure.
**************************************************
*/
int get_ap_mac(void)
{
	struct sockaddr_in *addr;
	struct ifreq ifr;
	unsigned char *mac;
	char *address;
	int sock_fd;

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		detector_debug(DET_LOG_ERR, "build sock to default to get ap mac failed. errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	strncpy(ifr.ifr_name, "default", IFNAMESIZE -1);

	if (ioctl(sock_fd, SIOCGIFHWADDR, &ifr) == -1)
	{
		detector_debug(DET_LOG_ERR, "ioctl get ap mac error. errno:%d,%s\n",
			errno,
			strerror(errno));
		close(sock_fd);
		return -1;
	}
	mac = (unsigned char *)(ifr.ifr_hwaddr.sa_data);
	memcpy(ap_mac, mac, MAC_ADDR_LEN);
	close(sock_fd);
	return 0;
}


/*
*********************************************************************
* FuncName:set_func_flag_to_scanner
* Description:call "iwpriv athN locate 0/1" cmd to set function flag
* Return: 0 success, -1 failure.
**********************************************************************
*/
int set_func_flag_to_scanner(u_int8_t switch_flag, u_int8_t radio_id)
{
	//open or close scan locate by calling cmd
	
	//iwpriv athN locate	1  //open the scan locate function
	//iwpriv athN locate	0  //close the scan locate function
	
	u_int8_t ifname[IF_NAME_SIZE];
	u_int8_t cmd_buf[CMD_BUF_SIZE];
	u_int8_t flag = switch_flag;
	int error = 0;
	
	memset(ifname, 0, IF_NAME_SIZE);
	sprintf(ifname, "athlocatewifi%d", radio_id);
	detector_debug(DET_LOG_DEBUG, "ifname: %s\n", ifname);
	//check if there is a vap named "athlocatewifi0"
	//if there is, return;
	if (0 != check_interface_state(ifname))
	{
		detector_debug(DET_LOG_CRIT, "set func flag to wlan,however,athlocatewifi%d doesn't exist.\n", radio_id);
		return -1;
	}
	detector_debug(DET_LOG_DEBUG, "athlocatewifi%d exist.\n", radio_id);

	memset(cmd_buf, 0, CMD_BUF_SIZE);
	detector_debug(DET_LOG_DEBUG, "flag: %d\n", flag);
	sprintf(cmd_buf, "iwpriv %s locate %d", ifname, flag);
	detector_debug(DET_LOG_DEBUG, "cmd_buf: %s\n", cmd_buf);
	error = detector_system(cmd_buf);
	if (error < 0)
	{
		detector_debug(DET_LOG_ERR, "set flat to scan vap failed.\n");
		return -1;
	}
	
	detector_debug(DET_LOG_INFO, "set flat to scan vap successfully.\n");
	return 0;
}

/*
*****************************************************
* FuncName:setup_scan_vap
* Description:setup a scan vap to start scan
* Return:0 success, -1 failure.
*****************************************************
*/
int setup_scan_vap(u_int8_t radio_id)
{
	u_int8_t ifname[IF_NAME_SIZE];
	u_int8_t cmd_buf[CMD_BUF_SIZE];
	int error = 0;
	
	//check if there is a vap named "athlocatewifi0"
	//if there is, return;
	memset(ifname, 0, IF_NAME_SIZE);
	sprintf(ifname, "athlocatewifi%d", radio_id);
	if (0 == check_interface_state(ifname))
	{
		detector_debug(DET_LOG_WARNING, "athlocatewifi%d has already exist.\n", radio_id);
		return 0;
	}
	else
	{
		detector_debug(DET_LOG_INFO, "athlocatewifi%d doesn't exist.\n", radio_id);
		memset(cmd_buf, 0, CMD_BUF_SIZE);
		sprintf(cmd_buf, "wlanconfig %s create wlandev wifi%d wlanmode ap > /dev/null", ifname, radio_id);
		error = detector_system(cmd_buf);
		if (error < 0)
		{
			return -1;
		}
		memset(cmd_buf, 0, CMD_BUF_SIZE);
		if (RADIO_11G == radio_id)
		{
			sprintf(cmd_buf, "iwpriv %s mode 11nght20 > /dev/null", ifname);
		}
		else if (RADIO_11A == radio_id)
		{
			sprintf(cmd_buf, "iwpriv %s mode 11naht20 > /dev/null", ifname);
		}
		error = detector_system(cmd_buf);
		if (error < 0)
		{
			detector_debug(DET_LOG_ERR,
				"set wlan mode failed to athlocatewifi%d.\n", radio_id);
			return -1;
		}
		memset(cmd_buf, 0, CMD_BUF_SIZE);
#if 0
		if (RADIO_11G == radio_id)
		{
			sprintf(cmd_buf, "iwconfig %s channel 11 > /dev/null", ifname);
		}
		else if (RADIO_11A == radio_id)
		{
			sprintf(cmd_buf, "iwconfig %s channel 149 > /dev/null", ifname);
		}
		error = detector_system(cmd_buf);
		if (error < 0)
		{
			detector_debug(DET_LOG_ERR,
				"set wlan channel failed to athlocatewifi%d.\n", radio_id);
			return -1;
		}
#endif
		memset(cmd_buf, 0, CMD_BUF_SIZE);
		if (RADIO_11G == radio_id)
		{
			sprintf(cmd_buf, "iwconfig %s essid scan_vap_11g > /dev/null", ifname);
		}
		else if (RADIO_11A == radio_id)
		{
			sprintf(cmd_buf, "iwconfig %s essid scan_vap_11a > /dev/null", ifname);
		}
		error = detector_system(cmd_buf);
		if (error < 0)
		{
			detector_debug(DET_LOG_ERR,
				"set wlan essid failed to athlocatewifi%d.\n", radio_id);
			return -1;
		}
		memset(cmd_buf, 0, CMD_BUF_SIZE);
		sprintf(cmd_buf, "iwpriv %s hide_ssid 1 > /dev/null", ifname);
		error = detector_system(cmd_buf);
		if (error < 0)
		{
			detector_debug(DET_LOG_ERR,
				"set hide essid failed to athlocatewifi%d.\n", radio_id);
			return -1;
		}
		memset(cmd_buf, 0, CMD_BUF_SIZE);
		sprintf(cmd_buf, "/sbin/ifconfig %s up > /dev/null", ifname);
		error = detector_system(cmd_buf);
		if (error < 0)
		{
			detector_debug(DET_LOG_ERR,
				"set wlan up failed to athlocatewifi%d.\n", radio_id);
			return -1;
		}
		detector_debug(DET_LOG_INFO, "athlocatewifi%d has been setup successfully.\n", radio_id);
	}
	
	return 0;
}

/*
***************************************************
* FuncName:check_interface_state
* Description:check the vap state
* Return: 0 success, -1 failure.
***************************************************
*/
int check_interface_state(u_int8_t *ifname)
{
	int sock_fd;
	struct ifreq ifr;
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		detector_debug(DET_LOG_ALERT,
			"setup socket of checking interface state failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

	if (ioctl(sock_fd, SIOCGIFINDEX, &ifr) == -1)
	{
		detector_debug(DET_LOG_INFO, "%s is not exist!\n", ifname);
		close(sock_fd);
		return -1;
	}
	
	detector_debug(DET_LOG_INFO, "%s is already exist.\n", ifname);
	close(sock_fd);
	return 0;
}



/*
*****************************************************
* FuncName:hand_scan_task
* Description:call the iwlist commond to start scan
* Return: 0 success.
*****************************************************
*/
int hand_scan_task(u_int8_t chan_to_scan, u_int16_t listen_time, u_int8_t radio_id)
{
	detector_debug(DET_LOG_INFO, "comming into hand_scan_task.\n");
	char cmd_buf[CMD_BUF_SIZE];
	
	memset(cmd_buf, 0, CMD_BUF_SIZE);
	sprintf(cmd_buf, "echo %d > /proc/sys/dev/wifi%d/iwlist_listen_time_each_chan", listen_time, radio_id);
	detector_debug(DET_LOG_INFO, "echo %d > /proc/sys/dev/wifi%d/iwlist_listen_time_each_chan\n", listen_time, radio_id);
	detector_system(cmd_buf);

	memset(cmd_buf, 0, CMD_BUF_SIZE);
	sprintf(cmd_buf, "echo %d > /proc/sys/dev/wifi%d/iwlist_scan_chan", chan_to_scan, radio_id);
	detector_debug(DET_LOG_INFO, "echo %d > /proc/sys/dev/wifi%d/iwlist_scan_chan\n", chan_to_scan, radio_id);
	detector_system(cmd_buf);

	memset(cmd_buf, 0, CMD_BUF_SIZE);
	sprintf(cmd_buf, "iwlist athlocatewifi%d scanning > /dev/null", radio_id);
	detector_debug(DET_LOG_INFO, "iwlist athlocatewifi%d scanning > /dev/null\n", radio_id);
	detector_system(cmd_buf);

	return 0;
}


#if 0
/*
*****************************************************
* FuncName:assemble_config_msg
* Description:assemble the config message to scanner
*****************************************************
*/
int assemble_config_msg(u_int8_t *buf)
{
	u_int8_t *tmp_ptr = buf;
	atnetlink_header atnlhdr;
	attribute_header attr_header;

	//atnetlink header
	memset(&atnlhdr, 0, sizeof(atnetlink_header));
	atnlhdr.domain = AT_NETLINK_DOMAIN;
	atnlhdr.version = AT_NETLINK_VERSION;
	if (detector_common_params.main_switch&RFID_FUNC_MASK)
		atnlhdr.attr_count ++;
	if (detector_common_params.main_switch&WIFIPOS_FUNC_MASK)
		atnlhdr.attr_count ++;
	if (detector_common_params.main_switch&RRM_FUNC_MASK)
		atnlhdr.attr_count ++;
	memcpy(tmp_ptr, &atnlhdr, sizeof(atnlhdr));
	
	//attribute header
	attr_header.func_type = ATTR_NLHDR_FUNCTYPE_CONFIG;
	if (detector_common_params.main_switch&RFID_FUNC_MASK)
		attr_header.func_subtype = attr_header.func_subtype|RFID_FUNC_MASK;
	if (detector_common_params.main_switch&WIFIPOS_FUNC_MASK)
		attr_header.func_subtype = attr_header.func_subtype|WIFIPOS_FUNC_MASK;
	
	if (detector_common_params.main_switch&RRM_FUNC_MASK)
		attr_header.func_subtype = attr_header.func_subtype|RRM_FUNC_MASK;
	*tmp_ptr ++ = ATTR_NLHDR_FUNCTYPE_TYPE;
	*tmp_ptr ++ = 1;
	*tmp_ptr ++ = ATTR_NLHDR_FUNCTYPE_CONFIG;
	if (detector_common_params.main_switch&RFID_FUNC_MASK)
		*tmp_ptr = detector_common_params.main_switch|RFID_FUNC_MASK;	//subtype type value
	if (detector_common_params.main_switch&WIFIPOS_FUNC_MASK)
		*tmp_ptr = detector_common_params.main_switch|WIFIPOS_FUNC_MASK;
	if (detector_common_params.main_switch&RRM_FUNC_MASK)
		*tmp_ptr = detector_common_params.main_switch|RRM_FUNC_MASK;
	tmp_ptr ++;
	
	
	
	
	
	
	return 0;
}
#endif

/*
***************************************************
* FuncName:hand_wifipos_report_task
* Description:send wifipos result to server
***************************************************
*/
void* send_udp_msg_periodic(void *arg)
{
    int ret = 0;
	u_int16_t report_interval = 0;
	detector_debug(DET_LOG_DEBUG,
		"detector go into send_udp_msg_periodic.sockfd: %d.\n", 
		wifipos_udp_sockfd);

	//use the 11g server param first,if empty,use 11a server param
	report_interval = detector_wifipos_params_11g.report_interval;
	if (0 == report_interval)
	{
		detector_debug(DET_LOG_ALERT,
			"parm of report interval in 11g is empty,use 11a report interval parm.\n");
		report_interval = detector_wifipos_params_11a.report_interval;
	}
	while (1)
	{
		detector_debug(DET_LOG_DEBUG, 
			"comming into send_udp_msg_periodic.\n");
	    
		ret = hand_wifipos_report_task(wifipos_udp_sockfd);
		if (0 == ret)
		{
		    detector_debug(DET_LOG_INFO, "udp message sended to server OK.\n");
    	}
    	else
       	{
        	detector_debug(DET_LOG_WARNING, "udp message sended to server error.\n");
    	}
        
		usleep(report_interval * 1000);
	}

	return;
}

/*
***************************************************
* FuncName:hand_wifipos_report_task
* Description:send wifipos result to server
***************************************************
*/
int hand_wifipos_report_task(int sockfd)
{
	
	detector_debug(DET_LOG_INFO, "detector go into hand_wifipos_report_task.\n");
	struct sockaddr_in ser_addr;
	int result = 0, send_len = 0;
	int tmp_i = 0, build_msg = -1;
	assemble_message msg;
	static u_int32_t ser_ip = 0;
	static u_int16_t ser_port = 0;
	scan_locate_parm_detector_wifipos wifipos_parm;

	ser_ip = detector_wifipos_params_11g.server.server_ip;
	ser_port = detector_wifipos_params_11g.server.server_port;
	if ((0 ==ser_ip) || (0 == ser_port))
	{
		ser_ip = detector_wifipos_params_11a.server.server_ip;
		ser_port = detector_wifipos_params_11a.server.server_port;
	}
	
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(ser_port);
	ser_addr.sin_addr.s_addr = htonl(ser_ip);

	detector_debug(DET_LOG_DEBUG, 
	               "ser_addr.sin_addr.s_addr: %d, ser_ip: %d, server_port: %d\n", 
	               ser_addr.sin_addr.s_addr, ser_ip, ser_addr.sin_port);
	build_msg = assemble_wifipos_udp_msg(&msg);
	if (build_msg < 0)
	{
		detector_debug(DET_LOG_WARNING, 
			"assemble wifipos udp message failed, go back.\n");
		return -1;
	}
	else
	{
	    detector_debug(DET_LOG_INFO, 
			"hand_wifipos_report_task: assemble wifipos udp message OK.\n");
	}
	
	if (NULL == msg.msg)
	{
		detector_debug(DET_LOG_CRIT, 
			"hand_wifipos_report_task: msg.msg is NULL, go back.\n");
		return -1;
	}
	else
	{
        detector_debug(DET_LOG_DEBUG, 
			"udp_sock: %d\n", 
			sockfd);
		send_len = sendto(sockfd, msg.msg, msg.msg_len, 0, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
		detector_debug(DET_LOG_DEBUG, 
			"send_len: %d\n", 
			send_len);
		if (send_len < 0)
		{
			free(msg.msg);
			msg.msg = NULL;
			detector_debug(DET_LOG_ERR, 
				"send wifipos result message to server failed.Don't delete the result chain.sockfd=%d, errno=%d,%s\n",
				sockfd,
				errno,
				strerror(errno));
			return -1;
		}
        server_result_counter++;
		detector_debug(DET_LOG_INFO, 
		      "send wifipos result message to server ok. server_result_counter = %d\n", 
		      server_result_counter);
		printmsg(msg.msg_len, msg.msg);
		free(msg.msg);
		msg.msg = NULL;	
		
	    return 0;
	}

}

/*
*************************************************************
* FuncName:assemble_wifipos_udp_msg
* Description:assemble the wifipos result msg in udp  packet
*************************************************************
*/
int assemble_wifipos_udp_msg(assemble_message *msg)
{
	detector_debug(DET_LOG_DEBUG, "comming into assemble_wifipos_udp_msg.\n");

	int error = 0;

	detector_debug(DET_LOG_DEBUG, "assemble_wifipos_udp_msg: pthread_lock.\n");
	pthread_mutex_lock(&detector_mutex);
	error = assemble_msg(msg);
	pthread_mutex_unlock(&detector_mutex);
	detector_debug(DET_LOG_DEBUG, "assemble_wifipos_udp_msg: pthread_unlock.\n");
		
	return error;
}


/*
*********************************************************
* FuncName:assemble_msg
* Description:assemble a message to server
*********************************************************
*/
int assemble_msg(assemble_message *msg)
{
	u_int8_t *pos = NULL;
	u_int16_t udp_msg_body_len = 0;
	int error = 0;
	detector_debug(DET_LOG_DEBUG, "comming into assemble_msg.\n");
	udp_msg_body_len = WIFIPOS_UDPMSG_STAINFO_LEN*g_wifipos_result_chain_node_count;

	if(NULL == msg)
	{
		detector_debug(DET_LOG_CRIT, 
			"assemble_msg:input NULL pointer.\n");
		return -1;
	}

	if(NULL == g_wifipos_result_chain_head)
	{
		detector_debug(DET_LOG_WARNING, 
			"the wifipos result chain is empty.go back. counter=%d\n",
			g_wifipos_result_chain_node_count);
		return -1;
	}
	else if (g_wifipos_result_chain_node_count == 0)
	{
		detector_debug(DET_LOG_EMERG, 
			"the wifipos result chain head is not null,but the chain node count is zero!maybe leak memory.\n");
		g_wifipos_result_chain_head = NULL;
		return -1;
	}
	
	msg->msg = malloc(WIFIPOS_UDPMSG_HEADER_LEN + udp_msg_body_len);
	if (NULL == msg->msg)
	{
		detector_debug(DET_LOG_EMERG, "malloc for udp message failed! return.\n");
		return -1;
	}
	
	memset(msg->msg, 0, WIFIPOS_UDPMSG_HEADER_LEN + udp_msg_body_len);
	msg->msg_len = WIFIPOS_UDPMSG_HEADER_LEN + udp_msg_body_len;
	
	//assemble udp message header
	error = assemble_wifipos_udp_msg_head(msg->msg);
	detector_debug(DET_LOG_DEBUG, "assemble_msg:assemble_wifipos_udp_msg_head is OK \n");
	if (error < 0)
	{
		detector_debug(DET_LOG_ERR, 
			"assemble wifipos upd msg header failed.\n");		
		free(msg->msg);
		msg->msg = NULL;
		return -1;
	}
	
	//assemble udp message body
	error = assemble_wifipos_udp_msg_body(msg->msg + WIFIPOS_UDPMSG_HEADER_LEN);
	if (error < 0)
	{
		detector_debug(DET_LOG_ERR, "assemble wifipos udp msg body failed.\n");	
		free(msg->msg);
		msg->msg = NULL;
		return -1;
	}

	return 0;
}
/*
***************************************************
* FuncName:assemble_wifipos_udp_msg_head
* Description:this function will fill the wifipos
				udp	msg header
***************************************************
*/
int assemble_wifipos_udp_msg_head(u_int8_t *ptr)
{
	//protocol type
	*ptr = 0;
	ptr ++;
	*ptr = 1;
	ptr ++;
	*ptr = 0;
	ptr ++;

	//protocol version
	*ptr = 1;
	ptr ++;
	*ptr = 1;
	ptr ++;
	*ptr = 0;
	ptr ++;

	//message type
	*ptr = 2;
	ptr ++;
	*ptr = 1;
	ptr ++;
	*ptr = 0;
	ptr ++;

	//ap mac
	*ptr = 3;
	ptr ++;
	*ptr = MAC_ADDR_LEN;
	ptr ++;
	memcpy(ptr, ap_mac, MAC_ADDR_LEN);
	ptr += MAC_ADDR_LEN;

	//sta num
	*ptr = 4;
	ptr ++;
	*ptr = 1;
	ptr ++;
	*ptr = g_wifipos_result_chain_node_count;
	ptr ++;

	//message length
	*ptr = 5;
	ptr ++;
	*ptr = 2;
	ptr ++;
	*((u_int16_t *)ptr) = g_wifipos_result_chain_node_count*WIFIPOS_UDPMSG_STAINFO_LEN;
	ptr ++;

	return 0;
}


/*
***************************************************
* FuncName:assemble_wifipos_udp_msg_body
* Description:this function will fill the wifipos
				udp	msg body
***************************************************
*/
int assemble_wifipos_udp_msg_body(u_int8_t *ptr)
{
	u_int16_t sta_index = 0;
	scan_locate_res_wifipos_chain_node *tmp_node = g_wifipos_result_chain_head;
	scan_locate_res_wifipos_chain_node *node_to_del = NULL;

	while (tmp_node != NULL)
	{
		//sta type
		*ptr = 0; 
		ptr ++;
		*ptr = 1;
		ptr ++;
		*ptr = WIFIPOS_UDPMSG_STATYPE_DISASSOC;
		ptr ++;

		//sta mac
		*ptr = 1;
		ptr ++;
		*ptr = MAC_ADDR_LEN;
		ptr ++;
		memcpy(ptr, tmp_node->node.src_mac, MAC_ADDR_LEN);
		ptr += MAC_ADDR_LEN;

		//rssi
		*ptr = 2;
		ptr ++;
		*ptr = 1;
		ptr ++;
		*ptr = tmp_node->node.rssi;
		ptr ++;

		//toa
		*ptr = 3;
		ptr ++;
		*ptr = sizeof(u_int64_t);
		ptr ++;
		memcpy(ptr, &tmp_node->node.toa, sizeof(u_int64_t));
		ptr += sizeof(u_int64_t);

		//differ time
		*ptr = 4;
		ptr ++;
		*ptr = sizeof(u_int64_t);
		ptr ++;
		memcpy(ptr, &tmp_node->node.locate_interval, sizeof(u_int64_t));
		ptr += sizeof(u_int64_t);

		//channel
		*ptr = 5;
		ptr ++;
		*ptr = 1;
		ptr ++;
		*ptr = tmp_node->node.sta_chan;
		ptr ++;

		//go to next chain node
		node_to_del = tmp_node;
		tmp_node = tmp_node->next;
		//count --;
		free(node_to_del);
		g_wifipos_result_chain_node_count --;
	}

	g_wifipos_result_chain_head = tmp_node;
	
	return 0;
}


/*
****************************************************
* FuncName:wifipos_udp_sock_create
* Description:create a udp socket to server
****************************************************
*/
int wifipos_udp_sock_create()
{
	int sockfd;
	int create_try = 0;
	struct sockaddr_in server_addr;
	
	for (create_try = 0; create_try < 3; create_try ++)
	{
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd < 0)
			continue;
		else
			break;
	}
	if (sockfd < 0)
	{
		detector_debug(DET_LOG_CRIT, "create upd socket to server failed for 3 times! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	detector_debug(DET_LOG_DEBUG, "create upd socket to server ok! socket: %d\n",sockfd);

	return sockfd;
}


/*
*****************************************************
* FuncName:nlsock_create_to_wtpd
* Description:create netlink socket to wtpd
*****************************************************
*/
int nlsock_create_to_wtpd(void)
{
	int nlsock_fd = 0;
	int creat_try = 0;
	struct sockaddr_nl src_addr, dest_addr;
	struct msghdr msg;
	struct nlmsghdr *nlh = NULL;

	for (creat_try = 0; creat_try < 3; creat_try ++)
	{
		nlsock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_DETECTOR_WTPD);
		if (nlsock_fd < 0)
			continue;
		else
			break;
	}
	if (nlsock_fd < 0)
	{
		detector_debug(DET_LOG_ERR, 
			"failed to create netlink socket to wtpd for 3 times! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = MODULE_ID_DETECTOR;
	src_addr.nl_groups = 0;

	if (bind(nlsock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)))
	{
		detector_debug(DET_LOG_ERR, 
			"bind netlink socket to wtpd failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		/* gengzj modified for coverity check 2013-10-29 */
		close(nlsock_fd);
		/* gengzj modified end */
		return -1;
	}

	return nlsock_fd;
}

/*
*************************************************************
* FuncName:nlsock_create_to_scanner
* Description: create the netlink socket to scanner
*************************************************************
*/
int nlsock_create_to_scanner(void)
{
	int nlsock_fd = 0;
	int creat_try = 0;
	struct sockaddr_nl src_addr, dest_addr;
	struct msghdr msg;
	struct nlmsghdr *nlh = NULL;

	for (creat_try = 0; creat_try < 3; creat_try ++)
	{
		nlsock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_DETECTOR_SCANNER);
		if (nlsock_fd < 0)
			continue;
		else
			break;
	}
	if (nlsock_fd < 0)
	{
		detector_debug(DET_LOG_CRIT, "failed to create netlink socket to scanner for 3 times. errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = 0x101;
	src_addr.nl_groups = 0;

	if (bind(nlsock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)))
	{
		detector_debug(DET_LOG_ERR, 
			"bind netlink socket to scanner failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		/* gengzj modified for coverity check 2013-10-29 */
		close(nlsock_fd);
		/* gengzj modified end */
		return -1;
	}

	return nlsock_fd;
}

/*
*************************************************************
* FuncName:parse_netlink_msg
* Description:parse the netlink message from wtpd
*************************************************************
*/

/*
netlink message farm:

netlink header:
++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++

atnetlink header:
++++++++++++++++++++++++++++++++++++++++++++++++++
   domain  |  version  |       attr_count        |
++++++++++++++++++++++++++++++++++++++++++++++++++
                     reserved                    |
++++++++++++++++++++++++++++++++++++++++++++++++++
                   original_src_pid              |
++++++++++++++++++++++++++++++++++++++++++++++++++
                   original_des_pid              |
++++++++++++++++++++++++++++++++++++++++++++++++++

attribute header:
++++++++++++++++++++++++++++++++++++++++++++++++++
  type=0   | length=1  |   func_type |   type=1  |
++++++++++++++++++++++++++++++++++++++++++++++++++
 length=1  |func_subtype| type=2     |  length=1 |
++++++++++++++++++++++++++++++++++++++++++++++++++
  version  |  type=3   | length=1    |  length   |
++++++++++++++++++++++++++++++++++++++++++++++++++

*/
int parse_netlink_msg(u_int8_t *buf)
{
	u_int16_t tmp_len = 0;
	u_int16_t tmp_total_len = 0;
	attribute_header *attr_header;
	

	if (NULL == buf)
	{
		detector_debug(DET_LOG_CRIT, "the pointer of netlink message from wtpd is NULL!\n");
		return -1;
	}
	buf += sizeof(atnetlink_header);
	attr_header = malloc(sizeof(attribute_header));
	
	if (NULL == attr_header)
	{
		detector_debug(DET_LOG_EMERG, "malloc for attr header failed, return.\n");
		return -1;
	}
	memset(attr_header, 0, sizeof(attribute_header));
		
	while (tmp_total_len < ATTR_HDR_MSG_LEN)
	{
		switch(*buf)
		{
			case ATTR_NLHDR_FUNCTYPE_TYPE:	//func type
				buf ++;
				tmp_len = *buf;
				buf ++;
				attr_header->func_type = *buf;
				detector_debug(DET_LOG_DEBUG, "attr_header->func_type: %d\n", attr_header->func_type);
				break;
			case ATTR_NLHDR_FUNCSUBTYPE_TYPE:	//func subtype
				buf ++;
				tmp_len = *buf;
				buf ++;
				attr_header->func_subtype = *buf;
				
				detector_debug(DET_LOG_DEBUG, "attr_header->func_subtype: %d\n", attr_header->func_subtype);
				break;
			case ATTR_NLHDR_VERSION_TYPE:	//version
				buf ++;
				tmp_len = *buf;
				buf ++;
				attr_header->version = *buf;
				
				detector_debug(DET_LOG_DEBUG, "attr_header->version: %d\n", attr_header->version);
				break;
			case ATTR_NLHDR_MSGLENGTH_TYPE:	//msg length
				buf ++;
				tmp_len = *buf;
				buf ++;
				attr_header->length = *((u_int16_t *)buf);
				
				detector_debug(DET_LOG_DEBUG, "attr_header->length: %d\n", attr_header->length);
				break;
			default:
				detector_debug(DET_LOG_DEBUG, "unknown type in attribute header!\n");
				break;
				
		}
		buf += tmp_len;
		tmp_total_len += (TLV_MSG_TYPE_LEN + TLV_MSG_LENGTH_LEN + tmp_len);
		detector_debug(DET_LOG_DEBUG, "tmp_total_len: %d\n", tmp_total_len);
	}

	switch(attr_header->func_type)
	{
		case ATTR_NLHDR_FUNCTYPE_CONFIG:		//scan locate config params
			detector_debug(DET_LOG_INFO, 
				"scan locate config params!\n");
			parse_func_config_parm(buf, attr_header->func_subtype, attr_header->length);
			break;	
		case ATTR_NLHDR_FUNCTYPE_RESULT:		//scan locate result elements
			detector_debug(DET_LOG_INFO, 
				"parse scan locate result elements!\n");
			parse_scan_locate_result(buf, attr_header->func_subtype, attr_header->length);
			break;
		default:	//unknown func_type;
			detector_debug(DET_LOG_ERR, 
				"unknown func_type in attribute header!\n");
			break;
	}

	/* gengzj added for memory leak 2013-10-17 */
	free(attr_header);
	attr_header = NULL;
	/* gengzj added end */
	return 0;
}


/*
attibute body:(server_config)
++++++++++++++++++++++++++++++++++++++++++++++++++
 type=0    | length=1  |result_forward| type=1   | 
++++++++++++++++++++++++++++++++++++++++++++++++++
  length=1 |report_mode|  type=2     |  length=4 |
++++++++++++++++++++++++++++++++++++++++++++++++++
                  server_ip                      |
++++++++++++++++++++++++++++++++++++++++++++++++++
  type=3   |  length=2 |      server_port        |
++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/*
**************************************************
* FuncName:parse_func_config_parm
* Description:parse the config parmas of rfid,wifipos or rrm
**************************************************
*/
int parse_func_config_parm(u_int8_t *ptr, u_int8_t subtype, u_int16_t len)
{

	int error = 0;
	
	if (NULL == ptr)
	{
		detector_debug(DET_LOG_CRIT, "the pointer of function config message from wtpd is NULL!\n");
		return -1;
	}
	if (RFID_FUNC_MASK == subtype)	//rfid params
	{
		detector_debug(DET_LOG_INFO, "parse_rfid_config_parm\n");
	}
	else if (WIFIPOS_FUNC_MASK == subtype)	//wifipos params
	{
		
		detector_debug(DET_LOG_INFO, "parse_wifipos_config_parm\n");
		error = parse_wifipos_config_parm(ptr, len);
	}
	else if (RRM_FUNC_MASK == subtype)	//rrm params	
	{
		detector_debug(DET_LOG_INFO, "parse_rrm_config_parm\n");
	}
	else
		detector_debug(DET_LOG_ERR,
			"unknown config parm subtype: %d",
			subtype);
		
	if (error < 0)
	{
		
		detector_debug(DET_LOG_ERR, "parse config parm failed.\n");
		return -1;
	}		
	
	return 0;
}

/*
attribute body:(rfid config params)
++++++++++++++++++++++++++++++++++++++++++++++++++
   type=0  |  length=1  |func_switch | type=1    |
++++++++++++++++++++++++++++++++++++++++++++++++++
   length=1|scan_type   |  type=2    |  length=1 |
++++++++++++++++++++++++++++++++++++++++++++++++++
 chan_num  |  type=3    |length=     |  chan     |
++++++++++++++++++++++++++++++++++++++++++++++++++
          ........................               |
++++++++++++++++++++++++++++++++++++++++++++++++++
  type=4   | length=1   |scan_interval| type=5   |
++++++++++++++++++++++++++++++++++++++++++++++++++
  length=1 |listen_time |   type=6   |  length=1 |
++++++++++++++++++++++++++++++++++++++++++++++++++
mac_fil_sw |  type=7    |  length=1  |mac_fil_num|
++++++++++++++++++++++++++++++++++++++++++++++++++
  type=8   | length=    |  flag      | mac_len   |
++++++++++++++++++++++++++++++++++++++++++++++++++
                 	MAC ADDR
++++++++++++++++++++++++++++++++++++++++++++++++++
                        |  .................
++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#if 0
/*
*************************************************
* FuncName:parse_rfid_config_parm
* Description:parse rfid scan config params
*************************************************
*/
int parse_rfid_config_parm(u_int8_t *ptr, u_int16_t len)
{

	int tmp_len = 0;
	int total_len = 0;
	u_int8_t tmp_mac_filter_num = 0;
	u_int8_t radio_id;
	
	if (NULL == ptr)
	{
		detector_debug(DETECTOR_CRITICAL_DEBUG_INFO, "the pointer of rfid config message from wtpd is NULL!\n");
		return -1;
	}
	u_int8_t rfid_mac_filter_num = detector_rfid_params.mac_filter_num;
	
	while (total_len < len)
	{
		switch (*ptr)
		{
			case RFID_NLBD_FUNCSWITCH_TYPE:		//func_switch
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rfid_params.func_switch = *ptr;
				detector_common_params.main_switch = (detector_common_params.main_switch&0xfe)|detector_rfid_params.func_switch;
				break;
			case RFID_NLBD_SCANTYPE_TYPE:		//scan_type
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rfid_params.scan_type = *ptr;
				break;
			case RFID_NLBD_RADIOID_TYPE:		//radio id
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rfid_params.radio_id = *ptr;
				break;
			case RFID_NLBD_CHANBITMAP_TYPE:		//chan bit map
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				parse_config_chan_bitmap(ptr, detector_rfid_params.radio_id, RFID_FUNC_MASK);
				break;
			case RFID_NLBD_RESFORWARD_TYPE:		//result_forward
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rfid_params.server.server_type = *ptr;
				break;
			case RFID_NLBD_REPORTMODE_TYPE:		//report_mode
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rfid_params.server.deliver_mode = *ptr;
				break;
			case RFID_NLBD_SERVERIP_TYPE:		//server_ip
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				memcpy(detector_rfid_params.server.server_ip, ptr, IP_ADDR_lEN);
				break;
			case RFID_NLBD_SERVERPORT_TYPE:		//server_port
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rfid_params.server.server_port = (u_int16_t *)ptr;
				break;
			case RFID_NLBD_SCANINTERVAL_TYPE:		//scan_interval
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_common_params.scan_period_each_chan = (u_int16_t *)ptr;
				break;
			case RFID_NLBD_LISTENTIME_TYPE:		//listen_time_each_chan
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_common_params.listen_time_each_chan = (u_int16_t *)ptr;
				break;
			case RFID_NLBD_MACFILTSWITCH_TYPE:		//mac_filter_switch
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rfid_params.mac_filter_switch = *ptr;
				break;
			case RFID_NLBD_MACFILTNUM_TYPE:		//mac_filter_num
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				if (0 == rfid_mac_filter_num)
				{
					detector_rfid_params.mac_filter_num = *ptr;
					tmp_mac_filter_num = *ptr;
				}	
				else
				{
					tmp_mac_filter_num = *ptr;
				}
				break;
			case RFID_NLBD_MACFILTINFO_TYPE:		//mac filter info
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				parse_rfid_config_mac_info(*ptr, rfid_mac_filter_num, tmp_mac_filter_num);
				break;
			case RFID_NLBD_REPORTINTERVAL_TYPE:
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rfid_params.report_interval = (u_int16_t *)ptr;
			default:
				detector_debug(DETECTOR_IMPORTANT_DEBUG_INFO, "unknown type!\n");
				break;
		}
		ptr += tmp_len;
		total_len += tmp_len + TYPE_LENGTH_LEN;
	}

	return 0;
}
#endif

/*
*************************************************
* FuncName:parse_wifipos_config_parm
* Description:parse wifipos config params
*************************************************
*/
int parse_wifipos_config_parm(u_int8_t *ptr, u_int16_t len)
{
	
	int tmp_len = 0;
	int total_len = 0;
	u_int8_t tmp_chan_num = 0;	
	scan_locate_parm_detector_wifipos wifipos_parm;
	scan_locate_parm_detector_common common_parm;
	scan_locate_chan_node *tmp_chan_node = NULL;
	u_int8_t wifipos_locate_sta_num = 0;
	u_int8_t tmp_locate_sta_num = 0;
	u_int8_t tmp_radio_id = 0;
	u_int8_t tmp_i = 0;
	u_int8_t tmp_j = 0;
	
	memset(&wifipos_parm, 0, sizeof(wifipos_parm));
	memset(&common_parm, 0, sizeof(common_parm));
	//wifipos_locate_sta_num = wifipos_parm.locate_sta_num;
	
	if (NULL == ptr)
	{
		detector_debug(DET_LOG_CRIT, 
			"the pointer of wifipos config message from wtpd is NULL!\n");
		return -1;
	}
	
	while (total_len < len) 
	{
		switch (*ptr)
		{
			case WIFIPOS_NLBD_FUNCSWITCH_TYPE:		//func_switch
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				wifipos_parm.func_switch = *ptr;
				detector_debug(DET_LOG_DEBUG, 
					"wifipos_parm.func_switch: %d\n",
					wifipos_parm.func_switch);
				common_parm.main_switch = (common_parm.main_switch&0xfd)|(wifipos_parm.func_switch<<1);
				
				detector_debug(DET_LOG_DEBUG, 
					"common_parm.main_switch: %d\n",
					common_parm.main_switch);

				break;
			case WIFIPOS_NLBD_SCANTYPE_TYPE:		//scan_type
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				wifipos_parm.scan_type = *ptr;
				
				detector_debug(DET_LOG_DEBUG, 
					"wifipos_parm.scan_type: %d\n",
					wifipos_parm.scan_type);
				break;
			case WIFIPOS_NLBD_SCANINTERVAL_TYPE:		//scan_interval
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				common_parm.scan_period_each_chan = *((u_int16_t *)ptr);
				
				detector_debug(DET_LOG_DEBUG, 
					"common_parm.scan_period_each_chan: %d\n",
					common_parm.scan_period_each_chan);
				break;
			case WIFIPOS_NLBD_LISTENTIME_TYPE:		//listen_time_each_chan
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				common_parm.listen_time_each_chan = *((u_int16_t *)ptr);
				
				detector_debug(DET_LOG_DEBUG, 
					"common_parm.listen_time_each_chan: %d\n",
					common_parm.listen_time_each_chan);
				break;
			case WIFIPOS_NLBD_RESFORWARD_TYPE:		//result_forward
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				wifipos_parm.server.server_type = *ptr;
				
				detector_debug(DET_LOG_DEBUG, 
					"wifipos_parm.server.server_type: %d\n",
					wifipos_parm.server.server_type);
				break;
			case WIFIPOS_NLBD_REPORTMODE_TYPE:		//report_mode
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				wifipos_parm.server.deliver_mode = *ptr;
				
				detector_debug(DET_LOG_DEBUG, 
					"wifipos_parm.server.deliver_mode: %d\n",
					wifipos_parm.server.deliver_mode);
				break;
			case WIFIPOS_NLBD_SERVERIP_TYPE:		//server_ip
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				wifipos_parm.server.server_ip = *((u_int32_t *)ptr);
				
				detector_debug(DET_LOG_NOTICE, 
					"wifipos_parm.server.server_ip: %lu\n",
					wifipos_parm.server.server_ip);
				break;
			case WIFIPOS_NLBD_SERVERPORT_TYPE:		//server_port
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				wifipos_parm.server.server_port = *((u_int16_t *)ptr);
				
				detector_debug(DET_LOG_NOTICE, 
					"wifipos_parm.server.server_port: %d\n",
					wifipos_parm.server.server_port);
				break;
			case WIFIPOS_NLBD_STANUM_TYPE:		//locate sta num
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				#if 0
				if (0 == wifipos_locate_sta_num)
				{
					wifipos_parm.locate_sta_num = *ptr;
					tmp_locate_sta_num = *ptr;
				}
				else
				{
					tmp_locate_sta_num = *ptr;
				}
				#endif
				tmp_locate_sta_num = *ptr;	//this variable has not been used now, reserved.
				detector_debug(DET_LOG_DEBUG, 
					"tmp_locate_sta_num: %d\n",
					tmp_locate_sta_num);
				break;
			case WIFIPOS_NLBD_RADIOID_TYPE: 	//radio id
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				tmp_radio_id = *ptr;
				detector_debug(DET_LOG_CRIT, 
					"wifipos_parm.radio_id: %d\n",
					tmp_radio_id);
				break;
			case WIFIPOS_NLBD_CHANBITMAP_TYPE:	//chan bit map
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_debug(DET_LOG_DEBUG, 
					"wifipos_parm.chanbitmap: %lld\n",
					*((u_int64_t *)ptr));
				if (RADIO_11G == tmp_radio_id)
				{
					detector_debug(DET_LOG_DEBUG,
						"chan list changed in 11g\n");
					if (!parse_config_chan_bitmap(ptr, RADIO_11G, WIFIPOS_FUNC_MASK))
					{
						detector_debug(DET_LOG_INFO,
							"11g chan list parsed successfully.\n");
						common_parm.chan_num = detector_common_params_11g.chan_num;
						common_parm.chan_list = detector_common_params_11g.chan_list;
					}
				}
				else if (RADIO_11A == tmp_radio_id)
				{
					detector_debug(DET_LOG_DEBUG,
						"chan list changed in 11a\n");
					if (!parse_config_chan_bitmap(ptr, RADIO_11A, WIFIPOS_FUNC_MASK))
					{
						detector_debug(DET_LOG_INFO,
							"11a chan list parsed successfully.\n");
						common_parm.chan_num = detector_common_params_11a.chan_num;
						common_parm.chan_list = detector_common_params_11a.chan_list;
					}
				}
				break;
			case WIFIPOS_NLBD_REPORTINTERVAL_TYPE:	//report interval
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				wifipos_parm.report_interval = *((u_int16_t *)ptr);
				detector_debug(DET_LOG_DEBUG, 
					"wifipos_parm.report_interval: %d\n",
					wifipos_parm.report_interval);
				break;
			case WIFIPOS_NLBD_RSSILIMIT_TYPE:	//rssi limit
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				wifipos_parm.rssilimit = *ptr;
				detector_debug(DET_LOG_DEBUG, 
					"rssilimit: %d\n", 
					wifipos_parm.rssilimit);
				break;
			default:
				detector_debug(DET_LOG_DEBUG, 
					"unknown type!\n");
				break;
		}
		ptr += tmp_len;
		total_len += (tmp_len + TLV_MSG_TYPE_LEN + TLV_MSG_LENGTH_LEN);
	}
	
	if (RADIO_11G == tmp_radio_id)
	{
		memcpy(&detector_wifipos_params_11g, &wifipos_parm, sizeof(wifipos_parm));
		memcpy(&detector_common_params_11g, &common_parm, sizeof(common_parm));
		params_changed_11g = DET_TRUE;
		detector_debug(DET_LOG_ALERT, 
			"the scan locate params on radio 0 need to update.\n");
		//print chan list
		tmp_chan_node = detector_common_params_11g.chan_list;
		while (tmp_chan_node != NULL)
		{
			detector_debug(DET_LOG_ALERT,
				"chan: %d, flag: %d\n",
				tmp_chan_node->node.chan,
				tmp_chan_node->node.flag);
			tmp_chan_node = tmp_chan_node->next;
		}
	}
	else if (RADIO_11A == tmp_radio_id)
	{
		memcpy(&detector_wifipos_params_11a, &wifipos_parm, sizeof(wifipos_parm));
		memcpy(&detector_common_params_11a, &common_parm, sizeof(common_parm));
		params_changed_11a = DET_TRUE;
		detector_debug(DET_LOG_ALERT, 
			"the scan locate params on radio 1 need to update.\n"); 
		//print chan list
		tmp_chan_node = detector_common_params_11a.chan_list;
		while (tmp_chan_node != NULL)
		{
			detector_debug(DET_LOG_ALERT,
				"chan: %d, flag: %d\n",
				tmp_chan_node->node.chan,
				tmp_chan_node->node.flag);
			tmp_chan_node = tmp_chan_node->next;
		}
	}
	
	return 0;
}

#if 0
/*
*************************************************
* FuncName:parse_rrm_config_parm
* Description:parse rrm config params
*************************************************
*/
int parse_rrm_config_parm(u_int8_t *ptr, u_int16_t len)
{	

	int total_len = 0;
	int tmp_len = 0;
	u_int8_t tmp_chan_num = 0;
	u_int8_t common_parm_chan_num = detector_common_params.chan_num_11g;
	
	if (NULL == ptr)
	{
		detector_debug(DETECTOR_CRITICAL_DEBUG_INFO, "the pointer of rrm config message from wtpd is NULL!\n");
		return -1;
	}
	
	while (total_len < len)
	{
		switch (*ptr)
		{
			case RRM_NLBD_FUNCSWITCH_TYPE:		//func_switch
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rrm_params.func_switch = *ptr;
				detector_common_params.main_switch = (detector_common_params.main_switch&0xfb)|(detector_rfid_params.func_switch<<2);
				break;
			case RRM_NLBD_SCANTYPE_TYPE:		//scan_type
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rrm_params.scan_type = *ptr;
				break;
			case RRM_NLBD_RADIOID_TYPE:
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rrm_params.radio_id = *ptr;
				break;
			case RRM_NLBD_CHANBITMAP:		//chan bit map
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				parse_config_chan_bitmap(ptr, detector_rrm_params.radio_id, RRM_FUNC_MASK);
				break;
			case RRM_NLBD_RESFORWARD_TYPE:		//result_forward
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rrm_params.server.server_type = *ptr;
				break;
			case RRM_NLBD_REPORTMODE_TYPE:		//report_mode
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rrm_params.server.deliver_mode = *ptr;
				break;
			case RRM_NLBD_SERVERIP_TYPE:		//server_ip
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				memcpy(detector_rrm_params.server.server_ip, ptr, IP_ADDR_lEN);
				break;
			case RRM_NLBD_SERVERPORT_TYPE:		//server_port
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rrm_params.server.server_port = (u_int16_t *)ptr;
				break;
			case RRM_NLBD_SCANINTERVAL_TYPE:		//scan_interval
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_common_params.scan_period_each_chan = (u_int16_t *)ptr;
				break;
			case RRM_NLBD_LISTENTIME_TYPE:		//listen_time_each_chan
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_common_params.listen_time_each_chan = (u_int16_t *)ptr;
				break;
			case RRM_NLBD_REPORTINTERVAL_TYPE:		//report interval
				ptr ++;
				tmp_len = *ptr;
				detector_rrm_params.report_interval = (u_int16_t *)ptr;
				break;
			case RRM_NLBD_SCANMODE_TYPE:		//scan mode
				ptr ++;
				tmp_len = *ptr;
				ptr ++;
				detector_rrm_params.scan_mode = *ptr;
				break;
			default:
				detector_debug(DETECTOR_IMPORTANT_DEBUG_INFO, "unknown type!\n");
				break;
		}
		ptr += tmp_len;
		total_len += tmp_len + TYPE_LENGTH_LEN;
	}
	
	return 0;
}
#endif

/*
*******************************************************
* FuncName:parse_config_chan_info
* Description:parse the chan info in scan params
*******************************************************
*/
int parse_config_chan_bitmap(char *cur_ptr, char radio_id, char func_type)	//there is some problems in this function
{
	int tmp_i = 0;
	u_int64_t chan_bit_map;
	u_int8_t chan_num;
	u_int8_t tmp_chan;
	u_int8_t tmp_chan_id;
	int error = -1;
	scan_locate_chan_info tmp_node;
	scan_locate_chan_node *tmp_chan_node = NULL;
	scan_locate_chan_node *k = NULL;
	scan_locate_chan_node *cur_chan_node = NULL;
	scan_locate_chan_node *chan_list_head = NULL;
	//detector_debug(DET_LOG_DEBUG, "sizeof(1) : %d\n", sizeof(1)); ==4
	
	if (NULL == cur_ptr)
	{
		detector_debug(DET_LOG_CRIT, 
			"the pointer of chan config message from wtpd is NULL!\n");
		return -1;
	}
	
	if (RADIO_11G == radio_id)
	{
		chan_num = 13;	//2.4G,13 channels
	}
	else 
	{
		chan_num = 56;	//5.8G,42 channels
	}
	
	chan_bit_map = *((u_int64_t *)cur_ptr);
	memcpy(&chan_bit_map, cur_ptr, sizeof(u_int64_t));
	detector_debug(DET_LOG_DEBUG, 
		"chan_bit_map: %lld, %llx, radio_id: %d.\n", 
		chan_bit_map, 
		chan_bit_map, 
		radio_id);
	
	for (tmp_i = 0; tmp_i < chan_num; tmp_i ++)
	{
		if (chan_bit_map&(bit(tmp_i)))
		{
			tmp_chan_id = tmp_i;
			detector_debug(DET_LOG_DEBUG, "tmp_chan_id: %d\n",tmp_chan_id);
			tmp_chan = switch_channel_id(tmp_chan_id);
			detector_debug(DET_LOG_DEBUG, "tmp_chan: %d\n",tmp_chan);
			error = common_parm_chan_add(tmp_chan, radio_id, func_type);
		}
		else 
		{
			tmp_chan_id = tmp_i;
			detector_debug(DET_LOG_DEBUG, "tmp_chan_id: %d\n",tmp_chan_id);
			tmp_chan = switch_channel_id(tmp_chan_id);
			detector_debug(DET_LOG_DEBUG, "tmp_chan: %d\n",tmp_chan);
			error = common_parm_chan_delete(tmp_chan, radio_id, func_type);
		}
	}
	//rank the chan list with chan from little to big
	if (RADIO_11G == radio_id)
	{
		chan_list_head = detector_common_params_11g.chan_list;
		cur_chan_node = detector_common_params_11g.chan_list;
		detector_debug(DET_LOG_DEBUG,
			"chan_list: %p\n",
			cur_chan_node);
		
	}
	else if (RADIO_11A == radio_id)
	{
		chan_list_head = detector_common_params_11a.chan_list;
		cur_chan_node = detector_common_params_11a.chan_list;
		detector_debug(DET_LOG_DEBUG,
			"chan_list: %p\n",
			cur_chan_node);
	}
	for (cur_chan_node = chan_list_head; cur_chan_node != NULL && cur_chan_node->next != NULL; cur_chan_node = cur_chan_node->next)
	{
		k = cur_chan_node;
		tmp_chan_node = cur_chan_node->next;
		do
		{
			if (tmp_chan_node->node.chan < k->node.chan)
			{
				k = tmp_chan_node;
			}
		}
		while (tmp_chan_node = tmp_chan_node->next);
		if (k != cur_chan_node)
		{
			tmp_node = cur_chan_node->node;
			cur_chan_node->node = k->node;
			k->node = tmp_node;
		}
	}
	return error;
}

/*
************************************************
* FuncName:common_parm_chan_delete
* Description:delete tmp chan from the chan list
*************************************************
*/
int common_parm_chan_delete(u_int8_t tmp_chan,char radio_id,char func_type)
{
	scan_locate_chan_node *prev_chan_node = NULL;
	scan_locate_chan_node *node_to_delete = NULL;
	scan_locate_parm_detector_common *common_parm = NULL;
	
	if (RADIO_11G == radio_id)
	{
		common_parm = &detector_common_params_11g;
		detector_debug(DET_LOG_INFO,
			"delete chan from common parm of radio 0.\n");
	}
	else 
	{
		common_parm = &detector_common_params_11a;
		detector_debug(DET_LOG_INFO,
			"delete chan from common parm of radio 1.\n");
	}
	
	if (NULL == common_parm->chan_list)
	{
		detector_debug(DET_LOG_CRIT, 
			"common parm chan list is NULL.\n");
		return -1;
	}
	else
	{
		prev_chan_node = common_parm->chan_list;
		node_to_delete = common_parm->chan_list;
		while (node_to_delete != NULL)
		{
			if (tmp_chan == node_to_delete->node.chan)
			{
				if (RFID_FUNC_MASK == func_type)
				{
					node_to_delete->node.flag = node_to_delete->node.flag&0xfe;
				}
				else if (WIFIPOS_FUNC_MASK == func_type)
				{
					node_to_delete->node.flag = node_to_delete->node.flag&0xfd;
				}
				else if (RRM_FUNC_MASK == func_type)
				{
					node_to_delete->node.flag = node_to_delete->node.flag&0xfb;
				}
				else
					detector_debug(DET_LOG_ERR,
						"unknown func type: %d",
						func_type);
				if (!node_to_delete->node.flag) //flag is 0,indecate that this chan is useless,delete it
				{
					if (node_to_delete == common_parm->chan_list)
					{
						detector_debug(DET_LOG_INFO,
							"node_to_delete is the chan list head, let the next node to be head.\n");
						common_parm->chan_list = node_to_delete->next;
						free(node_to_delete);
						node_to_delete = NULL;
					}
					else
					{
						prev_chan_node->next = node_to_delete->next;
						common_parm->chan_num --;
						free(node_to_delete);
						node_to_delete = NULL;
					}
				}
				else
				{
					prev_chan_node = node_to_delete;
					node_to_delete = node_to_delete->next;
				}
			}
			else
			{
				prev_chan_node = node_to_delete;
				node_to_delete = node_to_delete->next;
			}
		}
	}
	
	return 0;
}

/*
**********************************************
* FuncName:common_parm_chan_add
* Description:add tmp chan to the chan list
**********************************************
*/
int common_parm_chan_add(u_int8_t tmp_chan, char radio_id, char func_type)
{
	detector_debug(DET_LOG_DEBUG, "tmp_chan:%d, radio_id: %d.\n", tmp_chan, radio_id);
	scan_locate_parm_detector_common *common_parm = NULL;
	scan_locate_chan_node *node_to_add = NULL;
	scan_locate_chan_node *cur_node =NULL;
	int tmp_i = 0;
	
	if (RADIO_11G == radio_id)
	{
		common_parm = &detector_common_params_11g;
		detector_debug(DET_LOG_INFO,
			"add chan to common parms of radio 0.\n");
	}
	else 
	{
		common_parm = &detector_common_params_11a;
		detector_debug(DET_LOG_INFO,
			"add chan to common parms of radio 1.\n");
	}
	if (NULL == common_parm->chan_list)
	{
		node_to_add = malloc(sizeof(scan_locate_chan_node));
		
		if (NULL == node_to_add)
		{
			detector_debug(DET_LOG_EMERG, 
				"malloc for chan node to add failed,return.\n");
			return -1;
		}
		detector_debug(DET_LOG_DEBUG, "malloc for chan node to add successfully.\n");
		memset(node_to_add, 0, sizeof(scan_locate_chan_node));
		node_to_add->node.chan = tmp_chan;
		node_to_add->node.flag = WIFIPOS_FUNC_MASK;
		common_parm->chan_list = node_to_add;
		common_parm->chan_num ++;
		node_to_add = NULL;
		detector_debug(DET_LOG_DEBUG, "common_parm.chan_num: %d\n", 
			common_parm->chan_num);
	}
	else
	{	
		cur_node = common_parm->chan_list;
		while (cur_node != NULL)
		{
			if (tmp_chan == cur_node->node.chan)
			{
				if (RFID_FUNC_MASK == func_type)
				{
					cur_node->node.flag = (cur_node->node.flag&0xfe)|RFID_FUNC_MASK;
					return 0;
				}
				else if (WIFIPOS_FUNC_MASK == func_type)
				{
					cur_node->node.flag = (cur_node->node.flag&0xfd)|WIFIPOS_FUNC_MASK;
					return 0;
				}
				else if (RRM_FUNC_MASK == func_type)
				{
					cur_node->node.flag = (cur_node->node.flag&0xfb)|RRM_FUNC_MASK;
					return 0;
				}
				else
					detector_debug(DET_LOG_ERR,
						"unknown func type: %d",
						func_type);
			}
			else
			{
				cur_node = cur_node->next;
			}
		}
		detector_debug(DET_LOG_DEBUG, "this chan is not in the previous chan list.\n");
		node_to_add = malloc(sizeof(scan_locate_chan_node));
		
		if (NULL == node_to_add)
		{
			detector_debug(DET_LOG_EMERG, 
				"malloc for chan node to add failed,return.\n");
			return -1;
		}
		detector_debug(DET_LOG_DEBUG, "malloc for chan node to add successfully.\n");
		memset(node_to_add, 0 ,sizeof(scan_locate_chan_node));
		node_to_add->node.chan = tmp_chan;
		if (RFID_FUNC_MASK == func_type)
		{
			node_to_add->node.flag = RFID_FUNC_MASK;
		}
		else if (WIFIPOS_FUNC_MASK == func_type)
		{
			node_to_add->node.flag = WIFIPOS_FUNC_MASK;
		}
		else if (RRM_FUNC_MASK == func_type)
		{
			node_to_add->node.flag = RRM_FUNC_MASK;
		}
		else
			detector_debug(DET_LOG_ERR,
				"unknown func type: %d",
				func_type);
		node_to_add->next = common_parm->chan_list;
		common_parm->chan_list = node_to_add;
		common_parm->chan_num ++;
		node_to_add = NULL;
		detector_debug(DET_LOG_DEBUG, "common_parm.chan_num: %d\n", 
			common_parm->chan_num);
	}
	return 0;	
}

/*
**********************************************
* FuncName:switch_channel_id
* Description:switch the channel id to channel
**********************************************
*/
char switch_channel_id(char chan_id)
{
	u_int8_t chan;
	switch(chan_id)
	{
		case 0:
			chan = 1;
			break;
		case 1:
			chan = 2;
			break;
		case 2:
			chan = 3;
			break;
		case 3:
			chan = 4;
			break;
		case 4:
			chan = 5;
			break;
		case 5:
			chan = 6;
			break;
		case 6:
			chan = 7;
			break;
		case 7:
			chan = 8;
			break;
		case 8:
			chan = 9;
			break;
		case 9:
			chan = 10;
			break;
		case 10:
			chan = 11;
			break;
		case 11:
			chan = 12;
			break;
		case 12:
			chan = 13;
			break;
		case 13:
			chan = 14;
			break;
		case 16:
			chan = 7;
			break;
		case 17:
			chan = 8;
			break;
		case 18:
			chan = 9;
			break;
		case 19:
			chan = 11;
			break;
		case 20:
			chan = 12;
			break;
		case 21:
			chan = 16;
			break;
		case 22:
			chan = 34;
			break;
		case 23:
			chan = 36;
			break;
		case 24:
			chan = 38;
			break;
		case 25:
			chan = 40;
			break;
		case 26:
			chan = 42;
			break;
		case 27:
			chan = 44;
			break;
		case 28:
			chan = 46;
			break;
		case 29:
			chan = 48;
			break;
		case 30:
			chan = 52;
			break;
		case 31:
			chan = 56;
			break;
		case 32:
			chan = 60;
			break;
		case 33:
			chan = 64;
			break;
		case 34:
			chan = 100;
			break;
		case 35:
			chan = 104;
			break;
		case 36:
			chan = 108;
			break;
		case 37:
			chan = 112;
			break;
		case 38:
			chan = 116;
			break;
		case 39:
			chan = 120;
			break;
		case 40:
			chan = 124;
			break;
		case 41:
			chan = 128;
			break;
		case 42:
			chan = 132;
			break;
		case 43:
			chan = 136;
			break;
		case 44:
			chan = 140;
			break;
		case 45:
			chan = 149;
			break;
		case 46:
			chan = 153;
			break;
		case 47:
			chan = 157;
			break;
		case 48:
			chan = 161;
			break;
		case 49:
			chan = 165;
			break;
		case 50:
			chan = 183;
			break;
		case 51:
			chan = 184;
			break;
		case 52:
			chan = 185;
			break;
		case 53:
			chan = 187;
			break;
		case 54:
			chan = 188;
			break;
		case 55:
			chan = 189;
			break;
		case 56:
			chan = 192;
			break;
		case 57:
			chan = 196;
			break;
		default:
			detector_debug(DET_LOG_ERR, "unknown chan id.\n");
			/* gengzj modified for coverity check 2013-10-29 */
			chan = 0;
			/* gengzj added end */
			break;
	}
	return chan;
}

#if 0
/*
*************************************************
* FuncName:parse_rfid_config_mac_info
* Description:parse rfid mac filter info
*************************************************
*/
int parse_rfid_config_mac_info(char * cur_ptr,u_int8_t rfid_mac_filter_num,u_int8_t tmp_mac_filter_num)
{
	int tmp_i = 0;
	int tmp_j = 0;
	u_int8_t tmp_mac_flag = 0;
	u_int8_t tmp_mac_len = 0;
	
	if (NULL == cur_ptr)
	{
		detector_debug(DETECTOR_CRITICAL_DEBUG_INFO, "the pointer of rfid mac config message from wtpd is NULL!\n");
		return -1;
	}
	
	if (0 == rfid_mac_filter_num)
	{
		for (tmp_i = 0; tmp_i < tmp_mac_filter_num; tmp_i ++)
		{
			tmp_mac_flag = *cur_ptr;
			cur_ptr ++;
			if (0 == tmp_mac_flag)		//delete mac filter info
			{
				cur_ptr ++;
				cur_ptr += MAC_ADDR_LEN;
				detector_debug(DETECTOR_COMMON_DEBUG_INFO, "the mac_filter_info is empty,don't delete any mac.\n");
			}
			else		//add mac filter info
			{
				detector_rfid_params.mac_filter[tmp_i]->mac_len = *cur_ptr;
				cur_ptr ++;
				memcpy(detector_rfid_params.mac_filter[tmp_i]->mac, cur_ptr, MAC_ADDR_LEN);
				cur_ptr += MAC_ADDR_LEN;
			}
				
		}
	}
	else
	{
		for (tmp_i = 0; tmp_i < tmp_mac_filter_num; tmp_i ++)
		{
			tmp_mac_flag = *cur_ptr;
			cur_ptr ++; 
			tmp_mac_len = *cur_ptr;
			cur_ptr ++;
			if (0 == tmp_mac_flag)	//delete mac filter info
			{
				for (tmp_j = 0; tmp_j < rfid_mac_filter_num; tmp_j ++)
				{
					if (!memcmp(detector_rfid_params.mac_filter[tmp_j]->mac, cur_ptr, detector_rfid_params.mac_filter[tmp_j]->mac_len))
					{
						memset(detector_rfid_params.mac_filter[tmp_j], 0, sizeof(detector_rfid_params.mac_filter[tmp_j]));
						detector_rfid_params.mac_filter_num --;
					}
					else
					{
						detector_debug(DETECTOR_COMMON_DEBUG_INFO, "cann't find the mac info to delete,don't delete any mac info.\n");
					}
				}
				cur_ptr += MAC_ADDR_LEN;
			}
			else		//add mac filter info
			{
				for(tmp_j = 0; tmp_j < rfid_mac_filter_num; tmp_j ++)
				{
					if(!(memcpy(detector_rfid_params.mac_filter[tmp_j]->mac, cur_ptr, detector_rfid_params.mac_filter[tmp_j]->mac_len)))
					{
						detector_debug(DETECTOR_COMMON_DEBUG_INFO, "the mac info to add has already in the mac info list,do nothing.\n");
						cur_ptr += MAC_ADDR_LEN;
					}
					else
					{
						detector_rfid_params.mac_filter_num ++;
						detector_rfid_params.mac_filter[detector_rfid_params.mac_filter_num]->mac_len = tmp_mac_len;
						memcpy(detector_rfid_params.mac_filter[detector_rfid_params.mac_filter_num]->mac, cur_ptr, MAC_ADDR_LEN);
						cur_ptr += MAC_ADDR_LEN;
					}
				}
			}
		}
	}
	return 0;
}


/*
****************************************************
* FuncName:parse_wfipos_sta_info
* Description:parse sta info in wifipos params
****************************************************
*/
int	parse_wifipos_sta_info(char *cur_ptr, char wifipos_locate_sta_num, char tmp_locate_sta_num)
{
	int tmp_i = 0;
	int tmp_j = 0;
	int tmp_k = 0;
	u_int8_t tmp_sta_flag = 0;
	u_int8_t tmp_sta_chan = 0;
	u_int8_t tmp_mac_len = 0;
	
	if (NULL == cur_ptr)
	{
		detector_debug(DETECTOR_CRITICAL_DEBUG_INFO, "the pointer of wifipos sta info message from wtpd is NULL!\n");
		return -1;
	}
	
	if (0 == wifipos_locate_sta_num)
	{
		for (tmp_i = 0; tmp_i < tmp_locate_sta_num; tmp_i ++)
		{
			tmp_sta_flag = *cur_ptr;
			cur_ptr ++;
			tmp_sta_chan = *cur_ptr;
			cur_ptr ++;
			tmp_mac_len = *cur_ptr;
			cur_ptr ++;
			if (0 == tmp_sta_flag)		//delete chan
			{
				cur_ptr += MAC_ADDR_LEN;	//sta mac
				detector_debug(DETECTOR_COMMON_DEBUG_INFO, "chan list is empty, don't delete any chan.\n");
			}
			else	//add chan
			{
				detector_wifipos_params.sta[tmp_i]->sta_chan = tmp_sta_chan;
				for (tmp_j = 0; tmp_j < detector_common_params.chan_num; tmp_j ++)
				{
					if (tmp_sta_chan == detector_common_params.channel[tmp_j]->chan)	//there is this chan in common chan list
					{
						detector_common_params.channel[tmp_j]->flag = (detector_common_params.channel[tmp_j]->flag&0xfd)|(tmp_sta_flag<<1);	//set chan flag to do wifipos
					}
					else	//there is not this chan in common chan list,add it.
					{
						detector_common_params.chan_num ++;
						detector_common_params.channel[detector_common_params.chan_num]->chan = tmp_sta_chan;
						detector_common_params.channel[detector_common_params.chan_num]->flag = (detector_common_params.channel[detector_common_params.chan_num]->flag&0xfd)|(tmp_sta_flag<<1);
					}
				}
				detector_wifipos_params.sta[tmp_i]->mac_len = tmp_mac_len;
				memcpy(detector_wifipos_params.sta[tmp_i]->mac, cur_ptr, MAC_ADDR_LEN);
				cur_ptr += MAC_ADDR_LEN;
			}
		}
	}
	else
	{
		for (tmp_i = 0; tmp_i < tmp_locate_sta_num; tmp_i ++)
		{
			tmp_sta_flag = *cur_ptr;
			cur_ptr ++;
			tmp_sta_chan = *cur_ptr;
			cur_ptr ++;
			tmp_mac_len = *cur_ptr;
			cur_ptr ++;
			if (0 == tmp_sta_flag)		//delete chan
			{
				for (tmp_j = 0; tmp_j < wifipos_locate_sta_num; tmp_j ++)
				{
					if ((tmp_sta_chan == detector_wifipos_params.sta[tmp_j]->sta_chan)&&(!memcmp(detector_wifipos_params.sta[tmp_j]->mac, cur_ptr, tmp_mac_len)))
					{
						detector_wifipos_params.sta[tmp_j]->sta_chan = 0;
						memset(detector_wifipos_params.sta[tmp_j]->mac, 0, MAC_ADDR_LEN);
						
					}
					else
					{
						detector_debug(DETECTOR_COMMON_DEBUG_INFO, "cann't find the chan to delete, don't delete any chan.\n");
					}
				}
				cur_ptr += MAC_ADDR_LEN;
			}
			else		//add chan
			{
				for(tmp_j = 0; tmp_j < wifipos_locate_sta_num; tmp_j ++)
				{
					if ((tmp_sta_chan == detector_wifipos_params.sta[tmp_j]->sta_chan)&&(!memcmp(detector_wifipos_params.sta[tmp_j]->mac, cur_ptr, tmp_mac_len)))	//sta's chan and it's mac must be the same as previous it is set

						detector_common_params.channel[tmp_j]->flag = (detector_common_params.channel[tmp_j]->flag&0xfd)|(tmp_sta_flag<<1);
					else
					{
						for (tmp_k = 0; tmp_k < detector_common_params.chan_num; tmp_k ++)
						{
							if (tmp_sta_chan == detector_common_params.channel[tmp_k]->chan)
							{
								detector_common_params.channel[tmp_k]->flag = (detector_common_params.channel[tmp_k]->flag&0xfd)|(tmp_sta_flag<<1);
								detector_wifipos_params.locate_sta_num ++;
								detector_wifipos_params.sta[detector_wifipos_params.locate_sta_num]->sta_chan = tmp_sta_chan;
								detector_wifipos_params.sta[detector_wifipos_params.locate_sta_num]->mac_len = tmp_mac_len;
								memcpy(detector_wifipos_params.sta[detector_wifipos_params.locate_sta_num]->mac, cur_ptr, MAC_ADDR_LEN);
							}
							else
							{
								detector_common_params.chan_num ++;
								detector_common_params.channel[detector_common_params.chan_num]->chan = tmp_sta_chan;
								detector_common_params.channel[detector_common_params.chan_num]->flag = (detector_common_params.channel[detector_common_params.chan_num]->flag&0xfd)|(tmp_sta_flag<<1);
							}
						}
					}
				}
				cur_ptr += MAC_ADDR_LEN;
			}
		}
	}
	return 0;
}
#endif
/*
*************************************************
* FuncName:wifipos_result_chain_delete
* Description:delete the wifipos result chain
*************************************************
*/
void wifipos_result_chain_delete(void)
{
	detector_debug(DET_LOG_DEBUG, "commming into wifipos_result_chain_delete.\n");
	pthread_mutex_lock(&detector_mutex);
	chain_delete();	
	pthread_mutex_unlock(&detector_mutex);
	return;
}


/*
*************************************************
* FuncName:
* Description:
*************************************************
*/
int chain_delete(void)
{
	
	scan_locate_res_wifipos_chain_node *tmp_node = g_wifipos_result_chain_head;
	scan_locate_res_wifipos_chain_node *next_node = NULL;
	if (NULL == tmp_node)
	{
		detector_debug(DET_LOG_CRIT, "g_wifipos_result_chain_head is NULL.\n");
		return -1;
	}
	else
	{
		while ((tmp_node != NULL)&&(g_wifipos_result_chain_node_count > 0))
		{
			next_node = tmp_node->next;
			free(tmp_node);
			tmp_node = next_node;
			g_wifipos_result_chain_node_count --;
			detector_debug(DET_LOG_DEBUG, "delete:g_wifipos_result_chain_node_count: %d\n",g_wifipos_result_chain_node_count);
		}
		g_wifipos_result_chain_head = NULL;
	}
	detector_debug(DET_LOG_DEBUG, "detelet result chain ok.\n");

	return 0;
}

/*
*************************************************
* FuncName:scan_locate_system_exit
* Description:exit the system function on 11g
*************************************************
*/
void scan_locate_system_exit(u_int8_t radio_id)
{
	detector_debug(DET_LOG_DEBUG,
		"scan_locate_system_exit, radio_id: %d.\n", radio_id);
	u_int8_t rfid_exit = 0;
	u_int8_t wifipos_exit = 0;
	u_int8_t rrm_exit = 0;
	u_int8_t file_name[IF_NAME_SIZE];
	u_int8_t cmd_buf[CMD_BUF_SIZE];
	scan_locate_parm_detector_rfid rfid_parm_changed;
	scan_locate_parm_detector_rfid rfid_parm_another;
	scan_locate_parm_detector_wifipos wifipos_parm_changed;
	scan_locate_parm_detector_wifipos wifipos_parm_another;
	scan_locate_parm_detector_rrm rrm_parm_changed;
	scan_locate_parm_detector_rrm rrm_parm_another;
	scan_locate_parm_detector_common common_parm_changed;
	scan_locate_parm_detector_common common_parm_another;
	int error = -1;
	
	if (RADIO_11G == radio_id)
	{
		rfid_parm_changed = detector_rfid_params_11g;
		rfid_parm_another = detector_rfid_params_11a;
		wifipos_parm_changed = detector_wifipos_params_11g;
		wifipos_parm_another = detector_wifipos_params_11a;
		rrm_parm_changed = detector_rrm_params_11g;
		rrm_parm_another = detector_rrm_params_11a;
		common_parm_changed = detector_common_params_11g;
		common_parm_another = detector_common_params_11a;
	}
	else
	{
		rfid_parm_changed = detector_rfid_params_11a;
		rfid_parm_another = detector_rfid_params_11g;
		wifipos_parm_changed = detector_wifipos_params_11a;
		wifipos_parm_another = detector_wifipos_params_11g;
		rrm_parm_changed = detector_rrm_params_11a;
		rrm_parm_another = detector_rrm_params_11g;
		common_parm_changed = detector_common_params_11a;
		common_parm_another = detector_common_params_11g;
	}
	rfid_exit = common_parm_changed.main_switch&RFID_FUNC_MASK;
	wifipos_exit = common_parm_changed.main_switch&WIFIPOS_FUNC_MASK;
	rrm_exit = common_parm_changed.main_switch&RRM_FUNC_MASK;

	//rfid exit
	if (0 == rfid_exit)
	{
		detector_debug(DET_LOG_ALERT, 
			"exit rfid function!\n");
		//exit rfid function

		//clear data struct at the end of exit function
		//memset(&rfid_parm_changed, 0, sizeof(rfid_parm_changed));
	}

	//wifipos exit
	if (0 == wifipos_exit)
	{
		detector_debug(DET_LOG_ALERT, 
			"exit wifipos function!\n");
		//close the socket to server when the server type is 1
		if (1 == wifipos_parm_changed.server.server_type)
		{
			if (0 == wifipos_parm_another.func_switch) //wifipos function has been closed on the other radio
			{
				close(wifipos_udp_sockfd);	//both 11g and 11a have closed wifipos function			
				detector_debug(DET_LOG_INFO, 
					"close the udp socket to server!\n");
				wifipos_result_chain_delete();
				detector_debug(DET_LOG_INFO, 
					"delete wifipos scan result chain!\n");
			}
		}
		
		if (0 == rfid_exit)
		{
			/*
		    memset(cmd_buf, 0, sizeof(cmd_buf));
		    sprintf(cmd_buf, "killall iwlist");
		    detector_system(cmd_buf);
			detector_debug(DET_LOG_INFO, 
				"cmd: killall iwlist!\n");
			*/
			error = set_func_flag_to_scanner(wifipos_parm_changed.func_switch, radio_id);	//raido0
			if (error < 0)
			{
			detector_debug(DET_LOG_ERR, 
				"set func flag to scanner failed!\n");
			}
			else
			{
				detector_debug(DET_LOG_DEBUG, 
					"set func flag to scanner ok!\n");
			}
			if (0 == rrm_exit)
			{
				if (0 == rrm_parm_another.func_switch)	//rrm on 11a has closed
				{
					scan_vap_destroy(radio_id);	//11g
					detector_debug(DET_LOG_ALERT, 
						"destroy the scan vap! radio_id: %d.\n", radio_id);
					//close(nlsock_to_scanner);
					//detector_debug(DET_LOG_INFO, "close the netlink socket to scanner!\n");
				}
			}
		}
		
		//clear data struct at the end of exit function
		//memset(&wifipos_parm_changed, 0, sizeof(wifipos_parm_changed));
	}

	//rrm exit
	if (0 == rrm_exit)
	{
		detector_debug(DET_LOG_ALERT, "exit rrm function!\n");
		//exit rrm function
		
		//clear data struct at the end of exit function
		//memset(&rrm_parm_changed, 0, sizeof(rrm_parm_changed));
	}
	
	//clear data struct at the end of exit function
	//memset(&common_parm_changed, 0, sizeof(common_parm_changed));

	return;
}

/*
*************************************************
* FuncName:scan_vap_destroy
* Description:destroy the scan vap
*************************************************
*/
int scan_vap_destroy(u_int8_t radio_id)
{
	u_int8_t ifname[IF_NAME_SIZE];
	u_int8_t cmd_buf[CMD_BUF_SIZE];
	int error = -1;
	if (RADIO_11G == radio_id)
	{
		//check if there is a vap named "athlocatewifi0"
		//if there is, return;
		if (0 != check_interface_state("athlocatewifi0"))
		{
			detector_debug(DET_LOG_WARNING, "athlocatewifi0 doesn't exist.\n");
			return 0;
		}
		else
		{
			memset(ifname, 0, IF_NAME_SIZE);
			memset(cmd_buf, 0, CMD_BUF_SIZE);
			strcpy(ifname, "athlocatewifi0");
			sprintf(cmd_buf, "wlanconfig %s destroy > /dev/null", ifname);
			error = detector_system(cmd_buf);
			if (error < 0)
			{
				detector_debug(DET_LOG_ERR,
					"excute cmd:wlanconfig athlocatewifi%d destroy failed.\n",
					radio_id);
				return -1;
			}

			}
		}	

	else if (RADIO_11A == radio_id)
	{
		//check if there is a vap named "athlocatewifi1"
		//if there is, return;
		if (0 != check_interface_state("athlocatewifi1"))
		{
			detector_debug(DET_LOG_WARNING, "athlocatewifi1 doesn't exist.\n");
			return 0;
		}
		else
		{
			memset(ifname, 0, IF_NAME_SIZE);
			memset(cmd_buf, 0, CMD_BUF_SIZE);
			strcpy(ifname, "athlocatewifi1");
			sprintf(cmd_buf, "wlanconfig %s destroy > /dev/null", ifname);
			error = detector_system(cmd_buf);
			if (error < 0)
			{
				detector_debug(DET_LOG_ERR,
					"excute cmd:wlanconfig athlocatewifi%d destroy failed.\n",
					radio_id);
				return -1;
			}

			}
		}
	return 0;
}
/*
*************************************************
* FuncName:detector_log_init_file()
* Description:create log file when it is not exist
*************************************************
*/
void detector_log_init_file(void) 
{	
	if((g_detector_logfile = fopen(DETECTOR_LOG_FILENAME, "a")) == NULL)
	{
		printf("Can't open log file: %s\n", strerror(errno));
		return;
	}
	
#ifndef CW_SINGLE_THREAD
	if(0 != pthread_mutex_init(&g_logfile_mutex, NULL)) 
	{
		printf("Can't Init File Mutex for Log");
		return;
	}
#endif

	return;
}
/*
*************************************************
* FuncName:detector_log_close_file()
* Description:close log flie
* Athor/Date: liwei 2013.08.20
*************************************************
*/
void detector_log_close_file(void)
{
#ifndef CW_SINGLE_THREAD
	if(pthread_mutex_destroy(&g_logfile_mutex))
	{
		detector_debug(DET_LOG_ALERT,
			"destroy mutex g_logfile_mutex failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return;
	}
#endif	
	fclose(g_detector_logfile);
	g_detector_logfile = (FILE *)NULL;

	return;
}
/*
*************************************************
* FuncName:check_detector_logfile_size()
* Description:check log flie
* Athor/Date: liwei 2013.08.20
*************************************************
*/
int check_detector_logfile_size(void)
{
	long fileSize = 0;

	fileSize = ftell(g_detector_logfile);
	if (-1 == fileSize)
	{
		printf("An error with log file occurred: %s\n", strerror(errno));
		return 0;
	}
	if (fileSize >= g_max_detector_logfile_size)
	{
		fclose(g_detector_logfile);
		if((g_detector_logfile = fopen(DETECTOR_LOG_FILENAME, "w")) == NULL) 
		{
			printf("Can't open log file: %s", strerror(errno));
			return 0;
		}
	}
	return 1;
}

/*
*************************************************
* FuncName:detector_log_close_file()
* Description:close log flie
* Athor/Date: liwei 2013.08.20
*************************************************
*/

inline void detector_vlog(const char *format, va_list args) 
{
    if ((char *)NULL == format)
	{
	    printf("format NULL\n");
	    return;
	}
	
	char fileLine[256] = {0};
	fileLine[255] = '\0';
	int ret = 0;

	if ((FILE *)NULL != g_detector_logfile)
	{	
#ifndef CW_SINGLE_THREAD
        ret = pthread_mutex_lock(&g_logfile_mutex);
		if (fseek(g_detector_logfile, 0L, SEEK_END))
		{
			fclose(g_detector_logfile);
			detector_debug(DET_LOG_ALERT,
				"set file pointer failed! errno:%d,%s\n",
				errno,
				strerror(errno));
			return;
		}
#endif
		vsnprintf(fileLine, 255, format, args);
		if(!check_detector_logfile_size())
		{
#ifndef CW_SINGLE_THREAD
			ret = pthread_mutex_unlock(&g_logfile_mutex);
#endif
			return;
		}
		if (1 != fwrite(fileLine, strlen(fileLine), 1, g_detector_logfile))
		{
			fclose(g_detector_logfile);
			detector_debug(DET_LOG_CRIT,
				"write detector log to detector.log failed! errno: %d,%s\n",
				errno,
				strerror(errno));
			return -1;
		}
		fflush(g_detector_logfile);
#ifndef CW_SINGLE_THREAD
		ret = pthread_mutex_unlock(&g_logfile_mutex);
#endif
	}
	
    return;
}

/*
*****************************************************
* FuncName:detector_debug_print_logtxt 
* Descritpion:print debug log to detector.log
*****************************************************
*/
void detector_debug(int debug_lvl, const char* fmt, ...)
{
    va_list args;
    va_list argslog;
        
    if (detector_debug_lvl >= debug_lvl)
    {
        if(debug_print_screen)
        {
            va_start(args, fmt);
            vprintf(fmt, args);
            va_end(args);
        }
        
        va_start(argslog, fmt);
        detector_vlog(fmt, argslog);
        va_end(argslog); 
    }
    
	return;
}

/*
**************************************************
* FuncName:signal_handler
* Description:handler special signal
**************************************************
*/
void __cdecl signal_handler(int signum)
{
	//wifipos_result_chain_delete();
	//rfid_result_chain_delete();
	//rrm_result_chain_delete();
    //pthread_exit(NULL);
    if (SIGINT == signum)
	    detector_debug(DET_LOG_ALERT,
	    	"signal: SIGINT\n");
    if (SIGKILL == signum)
	    detector_debug(DET_LOG_ALERT,
	    	"signal: SIGKILL\n");
    if (SIGSEGV == signum)
	    detector_debug(DET_LOG_ALERT,
	    	"signal: SIGSEGV\n");
    detector_debug(DET_LOG_ALERT, 
		"detector is stopped by signal, go to excute command exit(0) directly.\n");
	detector_debug(DET_LOG_EMERG, 
		"\n");
	detector_debug(DET_LOG_EMERG, 
		"\n");
	exit(0);
}

/*
******************************************


*******************************************
*/
void printmsg(int n ,unsigned char *buffer)
{
    if(debug_print_screen)
    {
        printmsg_to_screen(n, buffer);
    }

    printmsg_to_logfile(n, buffer);
	return;
}

void printmsg_to_screen(int n ,unsigned char *buffer)
{
	int t=0;
	while((n-t)>=16)    
	{
	   int i;
	   printf("[");    
	   for(i=0;i<16;i++)
	        printf("%02x ",buffer[t+i]);
	   printf("]\t[");      
	   for(i=0;i<16;i++)    
	   {
	        char ch=buffer[t+i];
	        if(isalnum(ch))
	             printf("%c",ch);
	        else
	             printf(".");
	   }
	   printf("]\n");      
	   t+=16;
	}

	if(n>t)      
	{
	   int i=t;
	   printf("[");
	   while(i<n)
	        printf("%02x ",buffer[i++]);
	   printf("]");
	   i=n-t;
	   i=16-i;
	   while(i--)
	        printf("   ");
	   printf("\t[");
	   i=t;
	   while(i<n)
	   {
	        char ch=buffer[i++];
	        if(isalnum(ch))
	             printf("%c",ch);
	        else
	             printf(".");
	   }
	   printf("]\n");
	}
	printf("\n\n");
	return;
}

void printmsg_to_logfile(int n ,unsigned char *buffer)
{
#if 1 /* del for test */
    unsigned char print_buf[256] = {0};
	unsigned char *ptr = NULL;

    detector_debug(DET_LOG_DEBUG, "\n");
    int t=0;
    while((n-t)>=16)    
    {
       int i = 0;
       ptr = print_buf;  
//	   sprintf(ptr, "\n");
//	   ptr += 1;
       sprintf(ptr, "[");
	   ptr += 1;
	   
       for(i=0;i<16;i++)
       {
            sprintf(ptr, "%02x ", buffer[t+i]);
			ptr += 3;
       }
       
       sprintf(ptr, "]\n"); 
	   ptr += 2;
	   ptr = '\0';
	   detector_debug(DET_LOG_DEBUG, "%s", print_buf);
       t+=16;
    }
     
    if(n>t)      
    {
       int i=t;
	   ptr = print_buf;
       sprintf(ptr, "[");
	   ptr += 1;
       while(i<n)
       {
            sprintf(ptr, "%02x ",buffer[i++]);
			ptr += 3;	
       }
       sprintf(ptr, "]");
	   ptr += 1;
       i=n-t;
       i=16-i;
       while(i--)
       {
            sprintf(ptr, "   ");
			ptr += 3;
       }
       sprintf(ptr, "\n\n");
	   ptr += 2;
	   ptr = '\0';
	   detector_debug(DET_LOG_DEBUG, "%s", print_buf);
    }
#endif /* del for test */
    return;
}

int write_counter_to_tmpfile(const char* filename, unsigned long counter)
{
    char cmdbuf[128] = {0};
    sprintf(cmdbuf, "echo %lu > %s", counter, filename);
    detector_system(cmdbuf);
    return 0;
}

/*
* get detector backup info: 
* det_get_param_bakupinfo()
* det_get_bak_state()
* det_get_bakupinfo()
* author/date: Li Wei 2013.08.24
*/
int det_get_param_bakupinfo(void)
{
	if(det_get_bakup_11g())
	{
		detector_debug(DET_LOG_ERR,
			"get 11g bakup info failed!\n");
		return -1;
	}
	if(det_get_bakup_11a())
	{
		detector_debug(DET_LOG_ERR,
			"get 11a bakup info failed!\n");
		return -1;
	}
	detector_debug(DET_LOG_ALERT,
		"get bakup info ok!\n");

	return 0;
}


int det_get_bakup_11a(void)
{
    FILE* fp = (FILE *)NULL;
    int readbytes = 0;
	long tmp_len = 0;
	scan_locate_chan_node *tmp_chan_node = NULL;
	scan_locate_chan_node *tmp_chan_list_head = NULL;
	scan_locate_chan_node *cur_chan_node = NULL;
	int tmp_i = 0;
	
	if (access(DETECTOR_BAKINFO_11A, F_OK))
	{
		detector_debug(DET_LOG_ALERT,
			"file detector_bakup_11A.bin doesn't exist!\n");
		return 0;
	}
    fp = fopen(DETECTOR_BAKINFO_11A, "rb");
    if ((FILE *)NULL == fp)
    {
        return -1;
    }	
	
    if (fseek(fp, tmp_len, SEEK_SET))
	{
		fclose(fp);
		detector_debug(DET_LOG_ALERT,
			"set file pointer failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
    readbytes = fread(&detector_common_params_11a, sizeof(detector_common_params_11a), 1, fp);
    if (readbytes != 1)
    {
        fclose(fp);
		detector_debug(DET_LOG_CRIT,
			"read detector_common_params_11a bakinfo failed. errno:%d,%s\n",
			errno,
			strerror(errno));
        return -1;
    }
	tmp_len += sizeof(detector_common_params_11a);
	
	//chan list
	if (detector_common_params_11a.chan_num > 0)
	{
		for (tmp_i = 0; tmp_i < detector_common_params_11a.chan_num; tmp_i ++)
		{
			tmp_chan_node = malloc(sizeof(scan_locate_chan_node));
			if (NULL == tmp_chan_node)
			{
				fclose(fp);
				detector_debug(DET_LOG_EMERG,
					"malloc for tmp chan node of 11g failed.\n");
				free(tmp_chan_list_head);
				tmp_chan_list_head = NULL;
				return -1;
			}
			else
			{
				memset(tmp_chan_node, 0, sizeof(scan_locate_chan_node));
				if (fseek(fp, tmp_len, SEEK_SET))
				{
					fclose(fp);
					detector_debug(DET_LOG_ALERT,
						"set file pointer failed! errno:%d,%s\n",
						errno,
						strerror(errno));
					free(tmp_chan_node);
					tmp_chan_node = NULL;
					free(tmp_chan_list_head);
					tmp_chan_list_head = NULL;
					return -1;
				}
				//readbytes = fread(tmp_chan_node, sizeof(scan_locate_chan_node), 1, fp);	this is wrong, the tmp_chan_node->next should not be overwrite.
				readbytes = fread(&tmp_chan_node->node, sizeof(scan_locate_chan_info), 1, fp);
				if (readbytes != 1)
				{
					fclose(fp);
					detector_debug(DET_LOG_CRIT,
						"read detector_common_params_11g.chan_list bakinfo failed. errno:%d,%s\n",
						errno,
						strerror(errno));
					free(tmp_chan_node);
					tmp_chan_node = NULL;
					free(tmp_chan_list_head);
					tmp_chan_list_head = NULL;
					return -1;
				}
				tmp_len += sizeof(scan_locate_chan_node);
			}
			if (NULL == tmp_chan_list_head) //head is null
			{
				tmp_chan_list_head = tmp_chan_node;
				tmp_chan_node = NULL;
			}
			else //new comming node to be the head
			{
				tmp_chan_node->next = tmp_chan_list_head;
				tmp_chan_list_head = tmp_chan_node;
				tmp_chan_node = NULL;
			}
		}
		detector_common_params_11a.chan_list = tmp_chan_list_head;
	}

    if (fseek(fp, tmp_len, SEEK_SET))
	{
		fclose(fp);
		detector_debug(DET_LOG_ALERT,
			"set file pointer failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
    readbytes = fread(&detector_wifipos_params_11a, sizeof(detector_wifipos_params_11a), 1, fp);
    if (readbytes != 1)
    {
        fclose(fp);
		detector_debug(DET_LOG_CRIT,
			"read detector_wifipos_params_11g bakinfo failed. errno:%d,%s\n",
			errno,
			strerror(errno));
        return -1;
    }
    detector_wifipos_params_11a.sta_list_head = NULL;
    fclose(fp);

	//test code:print server params and chan list
    detector_debug(DET_LOG_ALERT, "11a server ip = %lu\t", 
                   detector_wifipos_params_11a.server.server_ip);
    detector_debug(DET_LOG_ALERT, "11a server port = %lu\n", 
                   detector_wifipos_params_11a.server.server_port);
	
	cur_chan_node = detector_common_params_11a.chan_list;
	if (cur_chan_node != NULL)
	{
		while (cur_chan_node != NULL)
		{
			detector_debug(DET_LOG_ALERT,
				"!!!!!!!!!!!!!!!!chan: %d, flag: %d, reserved: %d, next: %p\n", 
				cur_chan_node->node.chan, 
				cur_chan_node->node.flag,
				cur_chan_node->node.reserved,
				cur_chan_node->next);
			cur_chan_node = cur_chan_node->next;
		}
	}
	else
		detector_debug(DET_LOG_CRIT,
			"chan list head of 11a is NULL!\n");
	
	//set parm changed flag
	if (DET_TRUE == detector_wifipos_params_11a.func_switch)
	{
		detector_debug(DET_LOG_ALERT,
			"wifipos function on radio 1 need to start after detector hot restart.\n");
		params_changed_11a = DET_TRUE;
	}
    return 0;
}


int det_get_bakup_11g(void)
{
    FILE* fp = (FILE *)NULL;
    int readbytes = 0;
	long tmp_len = 0;
	scan_locate_chan_node *tmp_chan_node = NULL;
	scan_locate_chan_node *tmp_chan_list_head = NULL;
	scan_locate_chan_node *cur_chan_node = NULL;
	int tmp_i = 0;

	if (access(DETECTOR_BAKINFO_11G, F_OK))
	{
		detector_debug(DET_LOG_ALERT,
			"file detector_bakup_11G.bin doesn't exist!\n");
		return 0;
	}
    fp = fopen(DETECTOR_BAKINFO_11G, "rb");
    if ((FILE *)NULL == fp)
    {
        return -1;
    }

    if (fseek(fp, tmp_len, SEEK_SET))
	{
		fclose(fp);
		detector_debug(DET_LOG_ALERT,
			"set file pointer failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
    readbytes = fread(&detector_common_params_11g, sizeof(detector_common_params_11g), 1, fp);
    if (readbytes != 1)
    {
        fclose(fp);
		detector_debug(DET_LOG_CRIT,
			"read detector_common_params_11g bakinfo failed. errno:%d,%s\n",
			errno,
			strerror(errno));
        return -1;
    }
	tmp_len += sizeof(detector_common_params_11g);
	
	//chan list
	if (detector_common_params_11g.chan_num > 0)
	{
		for (tmp_i = 0; tmp_i < detector_common_params_11g.chan_num; tmp_i ++)
		{
			tmp_chan_node = malloc(sizeof(scan_locate_chan_node));
			if (NULL == tmp_chan_node)
			{
				fclose(fp);
				detector_debug(DET_LOG_EMERG,
					"malloc for tmp chan node of 11g failed.\n");
				free(tmp_chan_list_head);
				tmp_chan_list_head = NULL;
				return -1;
			}
			else
			{
				memset(tmp_chan_node, 0, sizeof(scan_locate_chan_node));
				if (fseek(fp, tmp_len, SEEK_SET))
				{
					fclose(fp);
					detector_debug(DET_LOG_ALERT,
						"set file pointer failed! errno:%d,%s\n",
						errno,
						strerror(errno));
					free(tmp_chan_node);
					tmp_chan_node = NULL;
					free(tmp_chan_list_head);
					tmp_chan_list_head = NULL;
					return -1;
				}
				//readbytes = fread(tmp_chan_node, sizeof(scan_locate_chan_node), 1, fp);	this is wrong, the tmp_chan_node->next should not be overwrite.
				readbytes = fread(&tmp_chan_node->node, sizeof(scan_locate_chan_info), 1, fp);
				if (readbytes != 1)
				{
					fclose(fp);
					detector_debug(DET_LOG_CRIT,
						"read detector_common_params_11g.chan_list bakinfo failed. errno:%d,%s\n",
						errno,
						strerror(errno));
					free(tmp_chan_node);
					tmp_chan_node = NULL;
					free(tmp_chan_list_head);
					tmp_chan_list_head = NULL;
					return -1;
				}
				tmp_len += sizeof(scan_locate_chan_node);
				//tmp_chan_node->next = NULL;	this is redundant.
			}
			if (NULL == tmp_chan_list_head)	//head is null
			{
				tmp_chan_list_head = tmp_chan_node;
				tmp_chan_node = NULL;
			}
			else //new comming node to be the head
			{
				tmp_chan_node->next = tmp_chan_list_head;
				tmp_chan_list_head = tmp_chan_node;
				tmp_chan_node = NULL;
			}
		}
		detector_common_params_11g.chan_list = tmp_chan_list_head;
		detector_debug(DET_LOG_DEBUG,
			"detector_common_params_11g.chan_list: %p\n",
			detector_common_params_11g.chan_list);
	}

    if (fseek(fp, tmp_len, SEEK_SET))
	{
		fclose(fp);
		detector_debug(DET_LOG_ALERT,
			"set file pointer failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
    readbytes = fread(&detector_wifipos_params_11g, sizeof(detector_wifipos_params_11g), 1, fp);
    if (readbytes != 1)
    {
        fclose(fp);
		detector_debug(DET_LOG_CRIT,
			"read detector_wifipos_params_11g bakinfo failed. errno:%d,%s\n",
			errno,
			strerror(errno));
        return -1;
    }
    detector_wifipos_params_11g.sta_list_head = NULL;
	
    fclose(fp);
	
	//test code:print server params and chan list 
    detector_debug(DET_LOG_ALERT, "11g server ip = %lu\t", 
                   detector_wifipos_params_11g.server.server_ip);
    detector_debug(DET_LOG_ALERT, "11g server port = %lu\n", 
                   detector_wifipos_params_11g.server.server_port);
    detector_debug(DET_LOG_DEBUG, "11g chan num = %d\n", 
                   detector_common_params_11g.chan_num);

	cur_chan_node = detector_common_params_11g.chan_list;
	detector_debug(DET_LOG_DEBUG,
		"detector_common_params_11g.chan_list: %p\n",
		detector_common_params_11g.chan_list);
	if (cur_chan_node != NULL)
	{
	while (cur_chan_node != NULL)
		{
			detector_debug(DET_LOG_ALERT,
				"!!!!!!!!!!!!!!!!chan: %d, flag: %d, reserved: %d, next: %p\n", 
				cur_chan_node->node.chan, 
				cur_chan_node->node.flag,
				cur_chan_node->node.reserved,
				cur_chan_node->next);
			cur_chan_node = cur_chan_node->next;
		}
	}
	else
		detector_debug(DET_LOG_CRIT,
			"chan list head of 11g is NULL!\n");
	
	//set parm changed flag
	if (DET_TRUE == detector_wifipos_params_11g.func_switch)
	{
		detector_debug(DET_LOG_ALERT,
			"wifipos function on radio 0 need to start after detector hot restart.\n");
		params_changed_11g = DET_TRUE;
	}
    return 0;
}

#if 0
int det_get_bak_state(void)
{
    FILE* fp = (FILE*)NULL;
    char cmdbuf[128];
    char string[64];
    memset(cmdbuf, 0, sizeof(cmdbuf));
    sprintf(cmdbuf, "cat %s", DETECTOR_BAK_STATE);
    fp = popen(cmdbuf, "r");
    if((FILE*)NULL != fp)
	{
//	    printf("det_get_bak_state: string = %s\n", string);
        memset(string, 0, sizeof(string));
        fgets(string, sizeof(string), fp);
        if (0 >= strlen(string))
        {
            pclose(fp);
            return -1;
        }
        string[strlen(string)-1] = '\0';
        pclose(fp);
        detector_state_bak = atoi(string);
	}
	return 0;
}
#endif

int det_get_bakupinfo(void)
{ 
    int ret = 0;
    ret = det_get_param_bakupinfo();
    if (ret != 0)
    {
        detector_debug(DET_LOG_ERR, "get detector param bakup info failed.\n");
        return -1;
    }
	/*
    ret = det_get_bak_state();
    if (ret != 0)
    {
        detector_debug(DET_LOG_ERR, "get detector bakup state failed.\n");
        return -1;
    }
    */
    detector_debug(DET_LOG_DEBUG, "get detector bakup info successfully.\n");
    return 0;
}
/*
* set detector backup info: 
* det_set_param_bakupinfo()
* det_set_bak_state()
* det_set_bakupinfo()
* author/date: Li Wei 2013.08.24
*/
int det_set_param_bakupinfo(u_int8_t radio_id)
{
    FILE* fp = (FILE *)NULL;
	
	
	if (RADIO_11G == radio_id)
	{
		fp = fopen(DETECTOR_BAKINFO_11G, "ab+");
		if ((FILE *)NULL == fp)
		{
			return -1;
		}
		if (set_bakinfo_11g(fp))
		{
			detector_debug(DET_LOG_ALERT,
				"set 11g bakupinfo failed!\n");
			return -1;
		}
	}
	else
	{
		fp = fopen(DETECTOR_BAKINFO_11A, "ab+");
		if ((FILE *)NULL == fp)
		{
			return -1;
		}
		if (set_bakinfo_11a(fp))
		{
			detector_debug(DET_LOG_ALERT,
				"set 11a bakupinfo failed!\n");
			return -1;
		}
	}
    fflush(fp);
    fclose(fp);
    return 0;
}

/*
********************************************
* FuncName:set_bakinfo
* Description:
* Return:0 success, -1 failed
*******************************************
*/
int set_bakinfo_11g(FILE* fp)
{
    int writebytes = 0;
	long tmp_len = 0;
	int tmp_i = 0;
	scan_locate_chan_node *tmp_chan_node = NULL;

	if (fseek(fp, tmp_len, SEEK_SET))
	{
		fclose(fp);
		detector_debug(DET_LOG_ALERT,
			"set file pointer failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	writebytes = fwrite(&detector_common_params_11g, sizeof(detector_common_params_11g), 1, fp);
	if (writebytes != 1)
	{
		fclose(fp);
		detector_debug(DET_LOG_CRIT,
			"write detector_common_params_11g bakinfo failed. errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	tmp_len += sizeof(detector_common_params_11g);
	
	//chan list
	if (detector_common_params_11g.chan_list != NULL)
	{
		tmp_chan_node = detector_common_params_11g.chan_list;
		detector_debug(DET_LOG_DEBUG,
			"detector_common_params_11g.chan_list: %p\n",
			detector_common_params_11g.chan_list);
		while (tmp_chan_node != NULL)
		{
			if (fseek(fp, tmp_len, SEEK_SET))
			{
				fclose(fp);
				detector_debug(DET_LOG_ALERT,
					"set file pointer failed! errno:%d,%s\n",
					errno,
					strerror(errno));
				return -1;
			}
			writebytes = fwrite(tmp_chan_node, sizeof(scan_locate_chan_node), 1, fp);
			if (writebytes != 1)
			{
				fclose(fp);
				detector_debug(DET_LOG_CRIT,
					"write detector_common_params_11g.chan_list bakinfo failed. errno:%d,%s\n",
					errno,
					strerror(errno));
				return -1;
			}
			tmp_len += sizeof(scan_locate_chan_node);
			detector_debug(DET_LOG_DEBUG,
				"set parms, chan: %d, flag: %d, reserved: %d, next: %p\n",
				tmp_chan_node->node.chan,
				tmp_chan_node->node.flag,
				tmp_chan_node);
			tmp_chan_node = tmp_chan_node->next;
		}
	}

	if (fseek(fp, tmp_len, SEEK_SET))
	{
		fclose(fp);
		detector_debug(DET_LOG_ALERT,
			"set file pointer failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	writebytes = fwrite(&detector_wifipos_params_11g, sizeof(detector_wifipos_params_11g), 1, fp);
	if (writebytes != 1)
	{
		fclose(fp);
		detector_debug(DET_LOG_CRIT,
			"write detector_wifipos_params_11g bakinfo failed. errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	return 0;
}

int set_bakinfo_11a(FILE* fp)
{
    int writebytes = 0;
	long tmp_len = 0;
	int tmp_i = 0;
	scan_locate_chan_node *tmp_chan_node = NULL;
	
	if (fseek(fp, tmp_len, SEEK_SET))
	{
		fclose(fp);
		detector_debug(DET_LOG_ALERT,
			"set file pointer failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	writebytes = fwrite(&detector_common_params_11a, sizeof(detector_common_params_11a), 1, fp);
	if (writebytes != 1)
	{
		fclose(fp);
		detector_debug(DET_LOG_CRIT,
			"write detector_common_params_11a bakinfo failed. errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	tmp_len += sizeof(detector_common_params_11a);
	
	//chan list
	if (detector_common_params_11a.chan_list != NULL)
	{
		tmp_chan_node = detector_common_params_11a.chan_list;
		while (tmp_chan_node != NULL)
		{
			if (fseek(fp, tmp_len, SEEK_SET))
			{
				fclose(fp);
				detector_debug(DET_LOG_ALERT,
					"set file pointer failed! errno:%d,%s\n",
					errno,
					strerror(errno));
				return -1;
			}
			writebytes = fwrite(tmp_chan_node, sizeof(scan_locate_chan_node), 1, fp);
			if (writebytes != 1)
			{
				fclose(fp);
				detector_debug(DET_LOG_CRIT,
					"write detector_common_params_11g.chan_list bakinfo failed. errno:%d,%s\n",
					errno,
					strerror(errno));
				return -1;
			}
			tmp_len += sizeof(scan_locate_chan_node);
			tmp_chan_node = tmp_chan_node->next;
		}
	}

	if (fseek(fp, tmp_len, SEEK_SET))
	{
		fclose(fp);
		detector_debug(DET_LOG_ALERT,
			"set file pointer failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	writebytes = fwrite(&detector_wifipos_params_11a, sizeof(detector_wifipos_params_11a), 1, fp);
	if (writebytes != 1)
	{
		fclose(fp);
		detector_debug(DET_LOG_CRIT,
			"write detector_wifipos_params_11a bakinfo failed. errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	return 0;
}

#if 0
int det_set_bak_state(void)
{
    char cmdbuf[128];
    int ret = 0;
    detector_state_bak = detector_state;
    sprintf(cmdbuf, "echo %d > %s", detector_state_bak, DETECTOR_BAK_STATE);
    ret = detector_system(cmdbuf);
    if (ret < 0)
    {
        return -1;
    }
    return 0;
}
#endif

int det_set_bakupinfo(void)
{
    int ret = 0;

	if (DET_TRUE == params_changed_11g)
	{
    	ret = det_set_param_bakupinfo(RADIO_11G);
	}
	else if (DET_TRUE == params_changed_11a)
	{
		ret = det_set_param_bakupinfo(RADIO_11A);
	}
    if (ret != 0)
    {
        detector_debug(DET_LOG_ERR, "set detector param bakup info failed.\n");
        return -1;
    }
	/*
    ret = det_set_bak_state();
    if (ret != 0)
    {
        detector_debug(DET_LOG_ERR, "set detector bakup state failed.\n");
        return -1;
    }
    */
    detector_debug(DET_LOG_DEBUG, "set detector bakup info successfully.\n");
    return 0;
}

#if 0
/*
* delete detector backup info: 
* det_set_bakupinfo()
* author/date: Li Wei 2013.08.24
*/
int det_del_bakupinfo(void)
{
	//check if file is exist
	if (access(DETECTOR_BAKINFO_FILE, 0))
	{
		detector_debug(DET_LOG_ERR,
			"file detector_bakupinfo.bin is not exist, return.\n");
		return 0;
	}
    char cmdbuf[128];
    int ret = 0;
    sprintf(cmdbuf, "rm %s", DETECTOR_BAKINFO_FILE);
    ret = detector_system(cmdbuf);
    if (ret < 0)
    {
        return -1;
    }
    detector_debug(DET_LOG_ALERT, "remove detector bakup info successfully.\n");
    return 0;
}
#endif
/*
********************************************************
* FuncName:detector_pthread_create
* Description:create a thread
* Return: 0 sucess, -1 error
********************************************************
*/
int detector_pthread_create(pthread_t *new_thread, void *thread_func, void *arg) 
{
	pthread_attr_t attr;
	size_t ss;	
	int s = PTHREAD_CREATE_DETACHED;
	if(new_thread == NULL) 
		return -1;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,s);
	if(pthread_create(new_thread, NULL, thread_func, arg) != 0)
	{
		detector_debug(DET_LOG_CRIT,
			"new thread create failed! errno:%d,%s\n",
			errno,
			strerror(errno));
		return -1;
	}
	return 0;
}

/*
******************************************************
* FuncName:sub_thread_kill()
* Description:kill all sub thread
* Return:0
******************************************************
*/
int sub_thread_kill(void)
{
	if (DET_TRUE == params_changed_11g)
	{
		if( (hand_scan_task_11g != 0) && (0 == pthread_kill(hand_scan_task_11g, 0) ) ) 
		{
			pthread_cancel(hand_scan_task_11g);
			detector_debug(DET_LOG_ALERT,
				"thread hand scan task of 11g has been canceled!\n");
		}			
	}
	if (DET_TRUE == params_changed_11a)
	{
		if( (hand_scan_task_11a != 0) && (0 == pthread_kill(hand_scan_task_11a, 0) ) ) 
		{
			pthread_cancel(hand_scan_task_11a);
			detector_debug(DET_LOG_ALERT,
				"thread han scan task of 11a has been canceled!\n");
		}	
	}
	if( (send_report_tid != 0) && (0 == pthread_kill(send_report_tid, 0) ) ) 
	{
		pthread_cancel(send_report_tid);
		detector_debug(DET_LOG_ALERT,
			"thread send report to server has been canceled!\n");
	}
	return 0;
}

/*
**********************************************
* FuncName:des_wlan_all()
* Description:delete all scan vap
* Return:0
**********************************************
*/
int des_wlan_all(void)
{
	if (DET_TRUE == params_changed_11g)
	{
		scan_vap_destroy(RADIO_11G);
	}
	if (DET_TRUE == params_changed_11a)
	{
		scan_vap_destroy(RADIO_11A);
	}
	return 0;
}

/*
*************************************************
* FuncName:detector_system(char *cmdbuf)
* Description:excute system cmd
* Return:0 success, -1 failed
*************************************************
*/
int detector_system(char *cmdbuf)
{
	int stat;
	
	stat = system(cmdbuf);
	if(WIFEXITED(stat))
	{
		if(WEXITSTATUS(stat) == 0)
		{	
			detector_debug(DET_LOG_INFO,
				"cmd ok:  %s\n",
				cmdbuf) ;
			return 0;
		}
		else
		{
			detector_debug(DET_LOG_ALERT,
				"cmd err!wexitstaus [0x%x],cmd: %s\n",
				WEXITSTATUS(stat),
				cmdbuf) ;
			return -1;
		}
	}

	detector_debug(DET_LOG_ALERT,
		"cmd err !errno:%d,%s,cmd: %s\n",
		errno,
		strerror(errno),
		cmdbuf);
	return -1;																
}
