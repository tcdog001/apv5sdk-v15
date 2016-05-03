/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：debug.c
* 摘    要：调试函数的实现
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#include <stdio.h>
#include <string.h>
#include "include/debug.h"
#include "include/structure.h"

void wapi_hex_dump(char *name, void *_str,int len)
{
	int i;
	unsigned char *str = (unsigned char *)_str;

	if(debug < 2)
		return;
	
	if(name != NULL)
		printf("[WAPID]:: %s(length=%d):", name, len);
	if(len >16)
		printf("	\n");
	for(i=0;i<len;i++)
	{	
		if((i)%16 == 0) printf("	");
		printf("%02X:",*str++);
		if(((i+1)%16==0) && ((i+1) <len))
			printf("	\n");
	}
	printf("\n");
}



void wapi_str_dump(char *name, void *_str,int len)
{
	int i;
	if(debug == 0)
		return;
	if(name != NULL)
		printf("[WAPID]:: %s(length=%d) : \n", name, len);
	for(i=0; i<len; i++){
		if((i)%64 == 0) printf("	");
		 printf("%c", *((unsigned char *)_str + i));
		 if((i+1)%64==0)
		 	printf("	\n");
	}
	printf("\n");
}


