
/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：init.h
* 摘    要： 系统初始化相关函数的原型
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#include "auth.h"
#define COMMTYPE_GROUP	8
#define PROC_NET_WIRELESS	"/proc/net/wireless"
#define PROC_SYS_DEV_WIFI	"/proc/net/dev"


int ap_initialize_wie(apdata_info *papdata);
#ifdef LSDK6
int ap_setdriver_wie(apdata_info *papdata, int mgmttype);
#endif
int ap_initialize_alg(apdata_info *papdata);

int ap_initialize(apdata_info *pap);

void ap_initialize_sta_table(apdata_info *pap);

/*初始化鉴别与密钥管理套间*/
int ap_initialize_akm(apdata_info *apdata);

int set_wapi(apdata_info *pap);

int check_file_valid(char *filename);
 int open_socket_for_asu() ;
int socket_open_for_ioctl();

int socket_open_for_netlink();
int rtnl_open(struct rtnl_handle *rth, unsigned subscriptions);
void rtnl_close(struct rtnl_handle *rth);

void free_one_interface(struct wapid_interfaces *interfaces );
void free_all_interfaces(struct eloop_data *eloop);

#ifdef DOT11_WAPI_MIB
int ap_initialize_mib(apdata_info *pap);
int ap_setup_mib(apdata_info *pap);
#endif

