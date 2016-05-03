
#ifndef _CTRL_IFACE_H_
#define _CTRL_IFACE_H_
#include "typedef.h"





#define CGI_PORT					9002
#define CGI_CMD_RELOAD			0x0212
#define CGI_RELOAD_RESPONSE	0x0213
#define CGI_CMD_CHECKCERT		0x0214
#define CGI_CHECK_RESPONSE	0x0215


/*****************************************************************************
	Description  : create a udp socket.
 *****************************************************************************/
#ifdef CTRL_UDP
int open_socket_for_cgi();
int Process_CGI_checkcert(u8 *mesg, int msglen);
int Process_CTRL_message(u8 *recv_buf, int readlen, int *cgicmd);
#endif
#ifdef CTRL_UNIX
int open_socket_for_ctrl(void);
int wapid_ctrl_iface_init(struct eloop_data *eloop);
void wapid_ctrl_iface_deinit(struct eloop_data *eloop);
int wapid_ctrl_handle(int sock, struct eloop_data * eloop);
#endif
#endif

