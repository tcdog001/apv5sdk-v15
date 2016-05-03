/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: readRadius.c
* description:  implementation for operation to get and analyze the Radius parameters.
* 
*
* 
************************************************************************************/

//output:
//	int GetRadiusPara( char *strPara ); //strPara: IN_OUT





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readRadius.h"
extern int SaveLogger( char *strPara );
/*
enum radiusParaSuffix{
    authPrimaryServerIP,
    authPrimaryServerPort,
    authPrimaryServerSecret,
    authSecondaryServerIP,
    authSecondaryServerPort,
    authSecondaryServerSecret,
    accountPrimaryServerIP,
    accountPrimaryServerPort,
    accountPrimaryServerSecret,
    accountSecondaryServerIP,
    accountSecondaryServerPort,
    accountSecondaryServerSecret,
    wpaPassPhrase
};
//*/

static char g_strMsg[256];

static struct RadiusAndWepPara{
    char radiusPara[13][100];
    int wpaReauthTime;
    int wpaGroupKeyUpdateCondition;
    int wpaGroupKeyUpdateIntervalSecond;
    int wpaGroupKeyUpdateIntervalPacket;
};


static void DisplayRadiusPara( struct RadiusAndWepPara *prw ){

    int i;

    printf( "radiusServerPara:\n" );
    for( i=0; i<13; i++){
    	printf( prw->radiusPara[i] );
    	printf("\n");
    }
    printf( "%d\n", prw->wpaReauthTime );
    printf( "%d\n", prw->wpaReauthTime );
    printf( "%d\n", prw->wpaGroupKeyUpdateIntervalSecond );
    printf( "%d\n", prw->wpaGroupKeyUpdateIntervalPacket );

    printf( "end\n" );	    
}

static void GetLine( FILE* fp, char *strLine )
{
    memset( strLine, 0, 256 );
    fgets( strLine, 256, fp);
    if( '\n' == strLine[strlen(strLine)-1] ){
	strLine[strlen(strLine)-1] = '\0';	
    }

}

static int AnalyzeRadiusAndWepParaLine( struct RadiusAndWepPara *prw, char *strLine )
{
    char strValue[100];
    char *pVal = NULL;

    memset( strValue, 0, 100 );

    sscanf( strLine, "%s ", strValue );

    pVal = strchr( strValue, '=' );
    if( !pVal ){
	return 1;
    }
    pVal++; //point to the next char of '='

    if( strstr(strLine, "auth_server_addr=") != NULL ){
	if( prw->radiusPara[0][0] == 0){
	    strcpy( prw->radiusPara[0], pVal );
	}
	else if( prw->radiusPara[3][0] == 0){
	    strcpy( prw->radiusPara[3], pVal );
	}
    }
    else if( strstr(strLine, "auth_server_port=") != NULL ){
	if( prw->radiusPara[1][0] == 0){
	    strcpy( prw->radiusPara[1], pVal );
	}
	else if( prw->radiusPara[4][0] == 0){
	    strcpy( prw->radiusPara[4], pVal );
	}
    }
    else if( strstr(strLine, "auth_server_shared_secret=") != NULL ){
	if( prw->radiusPara[2][0] == 0){
	    strcpy( prw->radiusPara[2], pVal );
	}
	else if( prw->radiusPara[5][0] == 0){
	    strcpy( prw->radiusPara[5], pVal );
	}
    }
    else if( strstr(strLine, "acct_server_addr=") != NULL ){
	if( prw->radiusPara[6][0] == 0){
	    strcpy( prw->radiusPara[6], pVal );
	}
	else if( prw->radiusPara[9][0] == 0){
	    strcpy( prw->radiusPara[9], pVal );
	}
    }
    else if( strstr(strLine, "acct_server_port=") != NULL ){
	if( prw->radiusPara[7][0] == 0){
	    strcpy( prw->radiusPara[7], pVal );
	}
	else if( prw->radiusPara[10][0] == 0){
	    strcpy( prw->radiusPara[10], pVal );
	}
    }
    else if( strstr(strLine, "acct_server_shared_secret=") != NULL ){
	if( prw->radiusPara[8][0] == 0){
	    strcpy( prw->radiusPara[8], pVal );
	}
	else if( prw->radiusPara[11][0] == 0){
	    strcpy( prw->radiusPara[11], pVal );
	}
    }
    else if( strstr(strLine, "wpa_passphrase=") != NULL ){	
    	strcpy( prw->radiusPara[12], pVal );
    }
    else if( strstr(strLine, "wpa_group_rekey=") != NULL ){
	prw->wpaGroupKeyUpdateIntervalSecond = atoi(pVal);    	
    }
    else{
    
    }
    

    return 0;
}

static int ReadRadiusAndWepPara( struct RadiusAndWepPara *prw ){

    char strLine[256];
    FILE *fp = fopen( RADIUS_CONF_FILE, "r" );
    if( !fp ){
	return -1;
    }

    memset( prw, 0, sizeof(struct RadiusAndWepPara) );

    while( !feof(fp) ){	//find string: "#[WEP_BEGIN]"

	GetLine( fp, strLine );
	if( '#'==strLine[0] || 0==strcmp(strLine, "") ){
	    continue;
	}
	if( 0 > AnalyzeRadiusAndWepParaLine( prw, strLine ) ){
	    fclose( fp );
	    return -1;
	}
	
    }
    fclose( fp );

    return 0;
}


int GetRadiusPara( char *strPara )
{
    struct RadiusAndWepPara rw;

    ReadRadiusAndWepPara( &rw );

    
    if( strcmp(strPara, "authPrimaryServerIP") == 0 ){

	strcpy( strPara, rw.radiusPara[0] );
    }
    if( strcmp(strPara, "authPrimaryServerPort") == 0 ){

	strcpy( strPara, rw.radiusPara[1] );
    }
    if( strcmp(strPara, "authPrimaryServerSecret") == 0 ){

	strcpy( strPara, rw.radiusPara[2] );
    }
    if( strcmp(strPara, "authSecondaryServerIP") == 0 ){

	strcpy( strPara, rw.radiusPara[3] );
    }
    if( strcmp(strPara, "authSecondaryServerPort") == 0 ){

	strcpy( strPara, rw.radiusPara[4] );
    }
    if( strcmp(strPara, "authSecondaryServerSecret") == 0 ){

	strcpy( strPara, rw.radiusPara[5] );
    }
    if( strcmp(strPara, "accountPrimaryServerIP") == 0 ){

	strcpy( strPara, rw.radiusPara[6] );
    }
    if( strcmp(strPara, "accountPrimaryServerPort") == 0 ){

	strcpy( strPara, rw.radiusPara[7] );
    }
    if( strcmp(strPara, "accountPrimaryServerSecret") == 0 ){

	strcpy( strPara, rw.radiusPara[8] );
    }
    if( strcmp(strPara, "accountSecondaryServerIP") == 0 ){

	strcpy( strPara, rw.radiusPara[9] );
    }
    if( strcmp(strPara, "accountSecondaryServerPort") == 0 ){

	strcpy( strPara, rw.radiusPara[10] );
    }
    if( strcmp(strPara, "accountSecondaryServerSecret") == 0 ){

	strcpy( strPara, rw.radiusPara[11] );
    }
    if( strcmp(strPara, "wpaPassPhrase") == 0 ){

	strcpy( strPara, rw.radiusPara[12] );
    }
    if( strcmp(strPara, "wpaReauthTime") == 0 ){
	return rw.wpaReauthTime;
    }
    if( strcmp(strPara, "wpaGroupKeyUpdateCondition") == 0 ){
	return rw.wpaGroupKeyUpdateCondition;
    }
    if( strcmp(strPara, "wpaGroupKeyUpdateIntervalSecond") == 0 ){
	return rw.wpaGroupKeyUpdateIntervalSecond;
    }
    if( strcmp(strPara, "wpaGroupKeyUpdateIntervalPacket") == 0 ){
	return rw.wpaGroupKeyUpdateIntervalPacket;
    }

    return 0;
}

int GetItemName( char *strOld, int *piTimes )
{
    if( strstr(strOld, "Secondary")!=NULL ){
	*piTimes = 1;
    }
    else{
	*piTimes = 0;
    }

    if( strstr(strOld, "authPrimaryServerIP")!=NULL || strstr(strOld, "authSecondaryServerIP")!=NULL ){
	strcpy( strOld, "auth_server_addr" );
    }

    if( strstr(strOld, "authPrimaryServerPort")!=NULL || strstr(strOld, "authSecondaryServerPort")!=NULL ){
	strcpy( strOld, "auth_server_port" );
    }
    	
    if( strstr(strOld, "authPrimaryServerSecret")!=NULL || strstr(strOld, "authSecondaryServerSecret")!=NULL ){
	strcpy( strOld, "auth_server_shared_secret" );		   
    }

    if( strstr(strOld, "accountPrimaryServerIP")!=NULL || strstr(strOld, "accountSecondaryServerIP")!=NULL ){
	strcpy( strOld, "acct_server_addr" );
    }

    if( strstr(strOld, "accountPrimaryServerPort")!=NULL || strstr(strOld, "accountSecondaryServerPort")!=NULL ){
	strcpy( strOld, "acct_server_port" );
    }
    	
    if( strstr(strOld, "accountPrimaryServerSecret")!=NULL || strstr(strOld, "accountSecondaryServerSecret")!=NULL ){
	strcpy( strOld, "acct_server_shared_secret" );		   
    }
 
    if( strstr(strOld,"wpaPassPhrase") != NULL ){
   	strcpy( strOld, "wpa_passphrase" ); 
    } 
    
    if( strstr(strOld,"wpaGroupKeyUpdateIntervalSecond") != NULL ){
   	strcpy( strOld, "wpa_group_rekey" ); 
    } 
    return 0;
}

int SaveRadiusPara( char *strParaName, char *strVal )
{
	
    FILE* fidIn, *fidOut;
    char line[1024], tmp_line[1024];
    char tmp_fname[255], *index;
    char separator[2];
    int i=0;
    char strPara[256];
    char strValue[256];
    int iTimes;
    
    strcpy( strPara, strParaName );
    strcpy( strValue, strVal );
    
    GetItemName( strPara, &iTimes );

    if ((fidIn=fopen(RADIUS_CONF_FILE,"r"))==NULL){
       	return 0;
    }
	strcpy(tmp_fname,RADIUS_CONF_FILE);	
	strcat(tmp_fname,".snmp");
	
        if ((fidOut=fopen(tmp_fname,"w"))==NULL)
	{
	   fclose(fidIn);
	    return 0;
	}

	while(!feof(fidIn))
	{
		fgets(line,1024,fidIn);
                
		if( strstr(line,strPara) != NULL && line[0] != '#' ) {
		    if( iTimes == 0 ){                 
			fprintf(fidOut, "%s=%s\n", strPara, strValue);
		    }
		    else { 
			fprintf(fidOut,"%s",line);
		    }
		    iTimes-- ;
		}
		else{ //strstr(line,strPara) == NULL			
		    fprintf(fidOut,"%s",line);
		}
	}
            
	fclose(fidIn);
	fclose(fidOut);
	unlink(RADIUS_CONF_FILE);
	rename(tmp_fname,RADIUS_CONF_FILE);
	return 1;
}

//*/

 
/*
int main( int argv, char **argc )
{
    int iRet;

    if( argv < 2 ){
	return -1;
    }

    iRet = SaveRadiusPara( argc[1], argc[2] );

    return 0;
}
//*/
