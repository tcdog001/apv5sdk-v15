#include <auteos.h>
#include <stdio.h>
#include <string.h>


static int usage(void)
{
	autelan_fprintf(stderr,
		"\n"
		"usage: daemon [cmd] [para1] ... [paran]"
		"\n"
		"   shellcmd	shell cmd need executed in backgroud,full PATH required!\n"
		"   para1   	first parameter\n"
		"   paran   	No. n parameter\n");
	
}

int main(int argc, char **argv){
	int i ;
	char cmdstr[255] ;
	char* pcmd ;
	
	if( argc < 2 ){
		usage() ;
		return 0;	
	}
	
	memset(cmdstr,0,255) ;
	pcmd=cmdstr ;
	
	for(i=1; i<argc ; i++){
			autelan_printf( "arg[%d]=%s\r\n",i,argv[i] ) ;
			autelan_sprintf(pcmd,"%s ", argv[i] ) ;
			pcmd += strlen(argv[i])+1 ;
	}
	
	autelan_printf( "autelan_system cmdline:%s\r\n", cmdstr ) ;
	autelan_printf( "move to backgroud \r\n") ;
	
	if( daemon(0,0)){
		autelan_fprintf(stderr, "daemon error") ;
		return 1 ;
	} 	

	autelan_system( cmdstr) ;
	
	return 0 ;
}
