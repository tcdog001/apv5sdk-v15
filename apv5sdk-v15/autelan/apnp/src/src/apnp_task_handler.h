#ifndef _APNP_TASK_HANDLER_H
#define _APNP_TASK_HANDLER_H

int
apnp_task_ip_change(void *data, size_t data_len,void **data_addr, size_t *data_addr_len);

int
apnp_task_tcp_send(void *data, size_t data_len,void **data_addr, size_t *data_addr_len);
int
apnp_task_udp_send(void *data, size_t data_len,void **data_addr, size_t *data_addr_len);


#endif
