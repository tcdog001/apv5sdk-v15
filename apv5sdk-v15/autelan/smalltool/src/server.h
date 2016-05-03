#ifndef _SERVER_H
#define _SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_ap_table{
    unsigned int m_ip_addr;
    unsigned int m_port;
    unsigned int m_user_count;
    int m_access_flag;
    unsigned int m_ap_flow;
}AP_TAB;



#ifdef __cplusplus
}
#endif

#endif
