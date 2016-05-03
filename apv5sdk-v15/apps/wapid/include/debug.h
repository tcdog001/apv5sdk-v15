/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：debug.c
* 摘    要：调试函数原型定义
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int debug;
#define DPrintf if (!debug) ; else printf
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

void wapi_str_dump(char *name, void *_str,int len);
void wapi_hex_dump(char *name, void *_str,int len);

#endif //  #ifndef _MY_TRACEFUNC_H__

