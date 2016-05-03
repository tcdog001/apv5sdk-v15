/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: readKey.c
* description:  implementation for operations to get and analyze the key of wep.
* 
*
* 
************************************************************************************/

//output interface:
//	int GetWepKey( int iIndex, char *strPara ); 
//		strPara: IN_OUT	in: para name; out:para value

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "wapiPwlanAp.h"
#include "../../../mibII/interfaces.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readKey.h"


extern char g_strMsg[256];


struct WepTypeKey{ 
//    int index; 
    char name[256];
    int type; 
    char key[4][100];
    char para5[4][5]; 
//    struct WepTypeKey *next; 
};

static struct WepTypeKey g_sList[2];


static void GetLine( FILE *fp, char *strLine )
{
    memset( strLine, 0, 256 );
    fgets( strLine, 256, fp);
    if( '\n' == strLine[strlen(strLine)-1] ){
	strLine[strlen(strLine)-1] = '\0';	
    }

}


/*
static void PrintList(){

    struct WepTypeKey* pCurr = g_psList;
    for(; pCurr; pCurr=pCurr->next ){
	printf( "index: %d, name: %s, type: %d, key:%s,%s,%s,%s, para5: %s, %s, %s, %s\n", pCurr->index, pCurr->name, pCurr->type, pCurr->key[0],pCurr->key[1], pCurr->key[2], pCurr->key[3], pCurr->para5[0], pCurr->para5[1], pCurr->para5[2], pCurr->para5[3] );
    }
}

static void FreeWep()
{
    struct WepTypeKey *pHead = g_psList; 
    
    for( ; g_psList; pHead = g_psList ){
	g_psList = g_psList->next;
	free( pHead );
    }
}


static struct WepTypeKey* FindWepPtr( int iIndex ) 
{
    struct WepTypeKey *pCurr = g_psList;

    //find in g_psWepList;
    for( ; pCurr; pCurr=pCurr->next ){
	if( pCurr ){
	    if( iIndex == pCurr->index ){
		return pCurr;
	    }
	}
    }
    return NULL;
} 

static struct WepTypeKey* GetWepPtr( int iIndex ) 
{
    struct WepTypeKey *pCurr = FindWepPtr( iIndex );

    if( pCurr ){
	return pCurr;
    }

    //else Alloc a new one
    if( !pCurr ){
    	pCurr = (struct WepTypeKey *) malloc(sizeof(struct WepTypeKey));
	if( !pCurr ){
	    return NULL;
	}
	pCurr->type = 1;
	pCurr->index = iIndex;
	memset( pCurr->key, 0, 400 );
	pCurr->next = NULL;
    }

    return pCurr;
}
*/

static int AnalyzeKeyLine( char *strLine )
{
    struct WepTypeKey* pCurr = NULL;

    char *pKey=NULL;
    int iIndex;
    int iPos;
    char strMsg[100];
    char str1[100];
    char str2[100];
    char str3[100];
    char str4[100];
    char str5[100];
 
    sscanf( strLine, "%s %s %s %s %s", str1, str2, str3, str4, str5 );

	if( strcmp("", g_sList[0].name) == 0 || strcmp(str2, g_sList[0].name)==0 ) {
		pCurr = &g_sList[0];
	}
	else {
		pCurr = &g_sList[1];
	}

    if( str4[0] == 's' && str4[1] == ':' ){
	pCurr->type = 2;
	pKey = str4 + 2;
    }
    else {  
	pCurr->type = 1;
	pKey = str4;
    }
 
    for( iPos=0; iPos<4; iPos++ ){
	if( strcmp(pCurr->key[iPos],"") == 0 ){	
	    strcpy( pCurr->key[iPos], pKey );
	    strcpy( pCurr->para5[iPos], str5 );
	    strcpy( pCurr->name, str2 );
	    break; 
	}
    }
 
    return 0;
}

int ReadKeyConf(){

    char strLine[256];
    FILE *fp = fopen( WEPKEY_CONF_FILE, "r" );
    int iStart = 0;

    memset( g_sList, 0, sizeof(g_sList) );
    strcpy( g_sList[0].name, "" );
    strcpy( g_sList[1].name, "" );

    if( !fp ){
	return -1;
    }

    while( !feof(fp) ){	//find string: "#[WEP_BEGIN]"

	GetLine( fp, strLine );
	if ( NULL != strstr( strLine, "#[WEP_BEGIN]")) {	   
 	    break; 
	}
    }

    while( !feof(fp) ){ //Analyze line

	GetLine (fp, strLine);
	if ( NULL != strstr( strLine, "#[WEP_END]")) {
	    break;
	}
	if( '#' == strLine[0] ){
	    continue;
	}
	if( 0 != AnalyzeKeyLine( strLine ) ){
	    fclose( fp );
	    return -1;
	}
	
    }

    fclose( fp );

    if( strcmp("", g_sList[0].name)==0 ) {
		return 0;
    }

    return 1;

}

int GetWepPara( char *strPara )
{
    int iHaveWepOrType=0;
    struct WepTypeKey *pCurr = NULL;

	iHaveWepOrType = ReadKeyConf();

	if( ! strPara ) {
		return iHaveWepOrType;    
	}
	
    pCurr = &g_sList[0];

    if( NULL != strstr(strPara, "type") ){
	iHaveWepOrType = pCurr->type;
    }
    else if( NULL != strstr(strPara, "key1") ){
	strcpy( strPara, pCurr->key[0] );
    }
    else if( NULL != strstr(strPara, "key2") ){
	strcpy( strPara, pCurr->key[1] );
    }
    else if( NULL != strstr(strPara, "key3") ){
	strcpy( strPara, pCurr->key[2] );
    }
    else if( NULL != strstr(strPara, "key4") ){
	strcpy( strPara, pCurr->key[3] );
    }
	else{}
  


    return iHaveWepOrType
		;
}

/*
int LoadWepPara( int iIndex, char *strPara, char *strVal )
{
    int iRet;
    struct WepTypeKey *pCurr = NULL;

    if( iIndex == 1 ){
	strcpy( strPara, "" );
	return -1;
    }

    ReadKeyConf();    
    pCurr = FindWepPtr( iIndex );    
    //pCurr = GetWepPtr( iIndex );
    if( !pCurr ){       	
	FreeWep( g_psList );
	strcpy( strPara, "" );
	return -1;    
    }

    if( NULL != strstr(strPara, "type") ){
	pCurr->type = atoi(strVal);
    }
    else if( NULL != strstr(strPara, "key1") ){
	strcpy( pCurr->key[0], strVal );
    }
    else if( NULL != strstr(strPara, "key2") ){
	strcpy( pCurr->key[1], strVal );
    }
    else if( NULL != strstr(strPara, "key3") ){
	strcpy( pCurr->key[2], strVal );
    }
    else if( NULL != strstr(strPara, "key4") ){
	strcpy( pCurr->key[3], strVal );
    }
    else{
	return -1;
    }

    return 0;
}
*/

static int ChangeLine( char *strLine, char *strPara, char *strVal )
{
    int iType;
	char strTmp[100];
    char str1[100];
    char str2[100];
    char str3[100];
    char str4[100];
    char str5[100];
 
    sscanf( strLine, "%s %s %s %s %s", str1, str2, str3, str4, str5 );


    if( str4[0] == 's' && str4[1] == ':' ){
	iType = 2;
    }
    else {  
	iType = 1;
    }
 
     if( NULL != strstr(strPara, "type") && atoi(strVal)!=iType ){
	 	if( iType == 1 ) {
			strcpy( strTmp, str4 );
			sprintf( str4, "s:" );
			strcat( str4, strTmp );
	 	}
		else {
			strcpy( str4, str4+2 );
		}
    }
    else if( NULL!=strstr(strPara, "key") ) 	{
		if( strcmp(strPara,"key1")==0 && strcmp(str5,"[1]")==0
			|| strcmp(strPara,"key2")==0 && strcmp(str5,"[2]")==0
			|| strcmp(strPara,"key3")==0 && strcmp(str5,"[3]")==0
			|| strcmp(strPara,"key4")==0 && strcmp(str5,"[4]")==0 ) 
		{
			if(iType==1){
				strcpy( str4, strVal );
			}
			else {
				strcpy( str4+2, strVal );
			}
		}
    	}
    else{
	return 0;
    }

	sprintf( strLine, "%s %s %s %s %s", str1, str2, str3, str4, str5 );
	
	
    return 0;
}

int SaveWepPara( char *strParaName, char *strValue )
{
    int i;
    FILE *fidOut;
   FILE *fidIn;
    char tmp_fname[255];
    struct WepTypeKey *pList=NULL;
    char strPara[256];
    char strVal[256];
	char strLine[256];

    strcpy( strPara, strParaName );
    strcpy( strVal, strValue );

	if( strstr( strPara,"type" ) != NULL ){
		return 0;
	}

	if(  ReadKeyConf() == 0 ) {
		return 0;
	}

//    LoadWepPara( iIndex, strPara, strVal );

    strcpy(tmp_fname,WEPKEY_CONF_FILE);	
    strcat(tmp_fname,".snmp");	

    if ((fidIn=fopen(WEPKEY_CONF_FILE,"r"))==NULL)
    {
	return 0;
    }
	if ((fidOut=fopen(tmp_fname,"w"))==NULL)
    {
	return 0;
    }


    while( !feof(fidIn) ){
	GetLine( fidIn, strLine );
	fprintf(fidOut,"%s\n",strLine);
	if ( NULL != strstr( strLine, "#[WEP_BEGIN]")) {	   
		//Analyze line
	    while( !feof(fidIn) ){ 
			GetLine (fidIn, strLine);
			if ( NULL != strstr( strLine, "#[WEP_END]")) {
				fprintf(fidOut,"%s\n",strLine);
			    	break;
			}
			if( '#' == strLine[0] ){
				fprintf(fidOut,"%s\n",strLine);
			    continue;
			}
			if( strcmp(strLine,"" )==0 ){
				continue;
				}
			ChangeLine( strLine, strPara, strVal );
			fprintf(fidOut,"%s\n",strLine);	
	    	}
	}
    	

    	}

    fclose(fidIn);
    fclose(fidOut);
    unlink(WEPKEY_CONF_FILE);
    rename(tmp_fname,WEPKEY_CONF_FILE);

    return 0;

}



 
/* write value
int main( int argv, char **argc )
{
    int iRet;

    if( argv < 2 ){
	return -1;
    }

    SaveWepPara( atoi(argc[1]), argc[2], argc[3] );

    return 0;
}
//*/

/* read value:
int main( int argv, char **argc )
{

    if( argv < 3 ){
	printf( "para less of 3!\n" );
	return -1;
    }

    printf( "%d: %s\n", GetWepPara( atoi(argc[1]), argc[2]), argc[2] );

    return 0;
}
//*/
