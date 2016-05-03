
/**************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：alg_comm.c
* 摘    要：定义更新AP证书的状态的函数
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
*************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "proc.h"
#include "certupdate.h"
#include "debug.h"

/*更新AP证书的状态*/
int update_cert_status(char *fileconfig)
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char get_cert_status[255] ={0,};
	int res = 0;

	/*从配置文件中读取证书状态*/
	prop_count=load_prop(SEP_EQUAL,fileconfig,properties);
	get_prop("CERT_STATUS", get_cert_status, properties, prop_count);
	free_prop(properties,prop_count);

	/*判断证书状态是否是无效*/
	if(atoi(get_cert_status) != 1)
	{
		/*将状态修改为有效*/
		res = !save_cert_status(fileconfig, "1");
	}
	return res;
}

/*保存证书状态*/
int save_cert_status(char *fileconfig, char *cert_flag)
{
	int res = 0;
	res = !save_global_conf(SEP_EQUAL,fileconfig,"",  "CERT_STATUS",cert_flag);
	if(res != 0)
	{
		DPrintf("open file %s error\n", fileconfig);
	}
	return res;
}

