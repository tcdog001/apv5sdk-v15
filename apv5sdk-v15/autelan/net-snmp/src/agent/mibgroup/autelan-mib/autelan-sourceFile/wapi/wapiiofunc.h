/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: wapiiofunc.h
* description:  implementation for a set of in-out control functions.
* 
*
* 
************************************************************************************/

#ifndef P80211FUNC_H
#define P80211FUNC_H

#define USLEEP_TIME 200000

typedef struct _if_info
{
        int index;
        char name[255];
}if_info;

typedef struct _temp_file
{
	char line[128];
	struct _temp_file * next;
}TEMP_FILE_STRU;

int construct_mib_req(UINT8 *msg, UINT32 getset, UINT32 did);
int do_ioctl( UINT8 *msg, char *wlandevname );
int get_if_info(if_info *ifinfo, int *num);
int test_wlan(char *devname);
void write_info(const char * info);
int msg_process_string(char *msgbuff, unsigned char *str, size_t *varlen);
int msg_process_uint32(char *msgbuff, long *longret);
int construct_setstring_req(UINT8 *msg, u_char *val, size_t vallen);
int construct_setuint32_req(UINT8 *msg, u_char *val);
char *removetok(char *str, char *tok);
int save_param(char* filename, char* toke, char* value);
int is_digital(char *str);

int header_singleindex(struct variable *vp,  oid *name,  size_t *length, int exact,
               size_t *var_len, int *index, int tabnamelen,   int max,  WriteMethod **write_method);
int oidmatch1(oid *name, size_t name_len, size_t varlen,
		int tok, int flag, char *ifname,int *index);
int oidmatch2(oid *name, size_t name_len, size_t varlen, unsigned long maxnum, int *index);

#endif


