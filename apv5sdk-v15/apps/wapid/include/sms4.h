/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：sms4.h
* 摘    要：SMS4算法相关函数的原型
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/
#ifndef __SMS4_H__
#define __SMS4_H__
/*加密函数*/
int wpi_encrypt(unsigned char * pofbiv_in,
				unsigned char * pbw_in,
				unsigned int plbw_in,
				unsigned char * pkey,
				unsigned char * pcw_out);
#endif
