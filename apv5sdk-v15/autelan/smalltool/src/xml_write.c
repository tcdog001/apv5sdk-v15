#include <auteos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int _system(char *cmdbuf)
{
	int stat;
	
	stat = autelan_system(cmdbuf);
	if(WIFEXITED(stat))
	{
		if(WEXITSTATUS(stat) == 0)
		{	
			autelan_printf("cmd ok:  %s\n",cmdbuf) ;
			return 0;
		}
		else
		{
			autelan_printf("cmd err!wexitstaus [0x%x],cmd: %s\n",WEXITSTATUS(stat),cmdbuf) ;
			return -1;
		}
	}

	autelan_printf("cmd err !errno:%d,%s,cmd: %s\n",errno,strerror(errno),cmdbuf) ;
	return -1;																
}

#define USAGE_WTPWriteXML "\n\
Usage:\n\
    xml_write <file> <name> <value>\n\
    xml_write <file> <radio id> <name> <value>\n\
    xml_write <file> <radio id> <wlan index> <name> <value>\n\
    xml_write <file> <radio id> <min wlan index> <max wlan index> <name> <value>\n\
\n"
int  usage()
{
	autelan_fprintf(stderr,USAGE_WTPWriteXML);
	return -1;
}
int main(int argc,char **argv)
{
	char file[128] = {0}, cmd[256]={0};

	if(argc != 4 && argc != 5 && argc != 6 && argc != 7){
		autelan_printf("argc:%d,no support this cmd.\n",argc);	
		usage();
		return -1;
	}
	
	strcpy(file,argv[1]);
	if (argc == 4)	
	{
		/* xml_write <file> <name> <value>  */
		char name[64]={0},value[64]={0};
		strcpy(name,argv[2]);
		strcpy(value,argv[3]);
		
		autelan_printf("argc:%d,file:%s,name:%s,value:%s\n",
			argc,file,name,value);	
		
		autelan_sprintf(cmd,"sed -i  '/<system>/,/<\\/system>/ s/%s>.*<\\/%s/%s>%s<\\/%s/' %s",name,name,name,value,name,file);
		return _system(cmd);
	}
	else if (argc == 5)	
	{
		/* xml_write <file> <radio id> <name> <value> */
		char name[64]={0},value[64]={0};
		int radio_id = -1;
		
		radio_id = atoi(argv[2]);
		strcpy(name,argv[3]);
		strcpy(value,argv[4]);
		
		autelan_printf("argc:%d,file:%s,radio id:%d,name:%s,value:%s\n",
			argc,file,radio_id,name,value);	
		
		if(radio_id < 0 || radio_id > 3) return usage();
		autelan_sprintf(cmd,"sed -i  '/<wifi%d>/,/<\\/wifi%d>/ s/%s>.*<\\/%s/%s>%s<\\/%s/' %s",radio_id,radio_id,name,name,name,value,name,file);
		return _system(cmd);
	}
	else if (argc == 6)	
	{
		/* xml_write <file> <radio id> <wlan index> <name> <value> */
		char name[64]={0},value[64]={0};
		int radio_id = -1,wlan_idx = -1,vap_idx = -1;
		
		radio_id = atoi(argv[2]);
		wlan_idx = atoi(argv[3]);
		strcpy(name,argv[4]);
		strcpy(value,argv[5]);
		
		autelan_printf("argc:%d,file:%s,radio id:%d,wlan idx:%d,name:%s,value:%s\n",
			argc,file,radio_id,wlan_idx,name,value);	
		
		if(radio_id < 0 || radio_id > 2) return usage();
		if(wlan_idx < 1 || wlan_idx > 8) return usage();
		vap_idx = 8*radio_id + wlan_idx;
		autelan_sprintf(cmd,"sed -i  '/<vap%d>/,/<\\/vap%d>/ s/%s>.*<\\/%s/%s>%s<\\/%s/' %s",vap_idx,vap_idx,name,name,name,value,name,file);
		return _system(cmd);
	}
	else if (argc == 7) 
	{
		/* xml_write <file> <radio id> <min wlan index> <max wlan index> <name> <value> */
		char name[64]={0},value[64]={0};
		int radio_id = -1,min_wlan_idx = -1,max_wlan_idx = -1,min_vap_idx = -1,max_vap_idx = -1;
		
		radio_id = atoi(argv[2]);
		min_wlan_idx = atoi(argv[3]);
		max_wlan_idx = atoi(argv[4]);
		strcpy(name,argv[5]);
		strcpy(value,argv[6]);
		
		autelan_printf("argc:%d,file:%s,radio id:%d,min wlan idx:%d,max wlan idx:%d,name:%s,value:%s\n",
			argc,file,radio_id,min_wlan_idx,max_wlan_idx,name,value);	

		if(radio_id < 0 || radio_id > 2) return usage();
		if(min_wlan_idx < 1 || min_wlan_idx > 8) return usage();
		if(max_wlan_idx < 1 || max_wlan_idx > 8) return usage();
		min_vap_idx = 8*radio_id + min_wlan_idx;
		max_vap_idx = 8*radio_id + max_wlan_idx;
		autelan_sprintf(cmd,"sed -i  '/<vap%d>/,/<\\/vap%d>/ s/%s>.*<\\/%s/%s>%s<\\/%s/' %s",min_vap_idx,max_vap_idx,name,name,name,value,name,file);
		return _system(cmd);
	}

	return 0;

}

