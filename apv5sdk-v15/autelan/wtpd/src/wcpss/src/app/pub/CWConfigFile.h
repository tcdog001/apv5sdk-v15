/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	       *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.                *
 *                                                                                         *
 * You should have received a copy of the GNU General Public License along with this       *
 * program; if not, write to the:                                                          *
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,                    *
 * MA  02111-1307, USA.                                                                    *
 *                                                                                         *
 * --------------------------------------------------------------------------------------- *
 * Project:  Capwap                                                                        *
 *                                                                                         *
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *******************************************************************************************/


#ifndef __CAPWAP_CWConfigFile_HEADER__
#define __CAPWAP_CWConfigFile_HEADER__
 
#define RADIO_TOTAL_COUNT 4
typedef char **CWStringArray;

typedef struct {
	enum {
		CW_INTEGER,
		CW_STRING,
		CW_STRING_ARRAY
	} type;
	
	union {
		int int_value;
		char *str_value;
		char **str_array_value;
	} value;
	
	char *code;
	char *endCode;
	
	int count;
} CWConfigValue;

struct radio_info_type{
	char radio_type;
	char radio_id;
	char bss_count;
	char reserved;
	
};

struct CWConfigVersionInfo_
{
	char *str_ap_model; //for oem change
	char *str_ap_version_name;
	char *str_ap_version_path;
	unsigned char radio_num;
	unsigned char bss_num;
	CWBool ischanged;
	CWBool ismodelchanged;
	struct radio_info_type radio_info[RADIO_TOTAL_COUNT];
	char *str_ap_code;// for model match
	struct CWConfigVersionInfo_ *next;
};

typedef struct CWConfigVersionInfo_ CWConfigVersionInfo;

extern CWConfigVersionInfo *gConfigVersionInfo;
extern CWConfigValue *gConfigValues;
extern int gConfigValuesCount;
extern int gModelCount;



CWBool CWParseConfigFile();
CWBool CWParseConfigVersionInfo();//added by weiay 20080618
CWBool CWParseConfigVersionInfoXML();//added by weianying 20090408
CWBool CWSaveConfigVersionInfo();
CWBool CWParseConfigFile1();
char * CWGetCommand(FILE *configFile);
CWBool CWConfigFileInitLib(void);
CWBool CWConfigFileInitLib1(void);
CWBool CWConfigFileDestroyLib(void);

#endif
