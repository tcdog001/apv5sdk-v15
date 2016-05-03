/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: syslogService.h
* description:  implementation for the header file of syslogService.c.
* 
*
* 
************************************************************************************/

#ifndef SYSLOGSERVICE_H
#define SYSLOGSERVICE_H

/*
*---------add by piyongping 2008-10-30---------------------------------
*/
#if 1
#define SYSLOGENABLE		1
#define SYSLOGMSGSEND		2
#define SYSLOGMSGLEVEL		3
#define SYSLOGINDEX		4
#define SYSLOGSERVERPATH		5
#define SYSLOGLEVEL		6
#define SYSLOGFACILITY		7
#define SYSLOGCONFINFO		8
#define SYSLOGUPDATAINTERVALTIME		9
#define SYSLOGSAVELOGBAKPATH		10
#define SYSLOGSERVERPORT		11

#endif


// function declarations 
    void init_syslogService(void);
    
    	FindVarMethod var_syslogService;
		FindVarMethod var_wapiSyslogSettingTable ;
#if 1
//add by piyongping 2008-10-30
		WriteMethod write_syslogEnable;
		WriteMethod write_syslogMsgSend;
		WriteMethod	write_syslogMsgLevel;
		WriteMethod write_syslogServerPath;
    	WriteMethod write_syslogLevel;
    	WriteMethod write_syslogFacility;
		WriteMethod write_syslogConfInfo;
		WriteMethod write_syslogServerPort;
		WriteMethod	write_cityhotspotsname;
		WriteMethod write_apattributionac;
		WriteMethod write_syslogUpdataIntervalTime;
		WriteMethod write_syslogSaveLogBakPath;
#endif




/*
*--------add by piyongping 2008-10-30-------------
*/
#if 1

//priority of syslog
static char *syslog_Alarm_Level[] = { 
		"emerg", 
		"alert", 
		"crit", 
		"err", 
		"warning", 
		"notice", 
		"info", 
		"debug" 
};

static char *facility_Names[] = {
	"*",		  	//表示所有设备来源
	"auth",       	//认证系统：login、su、getty等 
	"authpriv",   	//同LOG_AUTH，但只登录到所选择的单个用户可读的文件中 
	"cron",       	//守护进程     
	"daemon",     	//其他系统守护进程，如routed  
	"kern",       	//内核产生的消息
	"security",   	//安全原因
	"syslog",   	//由syslogd（8）产生的内部消息     
	"user",    		//随机用户进程产生的消息 
	"local0",  		//为本地使用保留 
	"local1"  		//为本地使用保留 
};

struct syslogConfpara{
    long dEnable;	
    char strLevel[20];
    char strPath[64];
	char strFacility[20];
    char strMsg[128];
    long dMsgLevel;
}syslogConfpara;

typedef struct syslogConf{
	char dataconf[104];
	struct syslogConf *next;
}syslogConf;
#endif


#if 1
void loadSyslogPara( void );
int loadSyslogConf( const char *strFileName, char *fac , char *level,char *path);
int loadSyslogConf1( const char *strFileName, struct syslogConf **head);
int  syslogScript( char *strType );
long getSyslogLevel( const char *strLevel );
int sendLoggerMsg( int iLevel, char *strMsg );
void free_Syslog_table( struct syslogConf **head);
char *readSyslogFlag(void);
int creatSyslogConf( const char *strFileName);
int copySyslogConf( char *srcPath,  char *desPath );
int saveSyslogConf1(const char *strFileName,char *syslogType,char *key,int index);
int showSyslogConfInfo( struct syslogConf *head,char *fac,char *level,char *path );
int getSyslogFacility( const char *strIndex );
int killallSyslogd( void );
void do_restart_syslog();
#endif


#endif // SYSLOGSERVICE_H 

