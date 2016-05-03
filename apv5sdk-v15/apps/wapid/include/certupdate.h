/**************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：alg_comm.c
* 摘    要：定义更新AP证书的状态的函数原型
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
*************************************************************/

#ifndef __CERTUPDATE_H_
#define __CERTUPDATE_H_

int update_cert_status(char *filename);
int save_cert_status(char *filename, char *cert_flag);

#endif
