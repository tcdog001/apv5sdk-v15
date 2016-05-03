/***************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：typedef.h
* 摘    要：自定义类型说明
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
***************************************************************/

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__
#ifdef __linux__
#include <endian.h>
#include <byteswap.h>
#endif
#include <stdint.h>
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned char   BOOL;

#endif

