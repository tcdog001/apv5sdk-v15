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

 
#include "CWWTP.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

/*____________________________________________________________________________*/
/*  *****************************___ASSEMBLE___*****************************  */
CWBool CWAssembleMsgElemACName(CWProtocolMessage *msgPtr) 
{
	char *name = NULL;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	name = CWWTPGetACName();
	
	// create message
	//CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, strlen(name), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	/*zengmin modify: create string len by Coverity 2013-06-09*/
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, strlen(name)+1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStoreStr(msgPtr, name);
				
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_AC_NAME_CW_TYPE);
}

CWBool CWAssembleMsgElemACNameWithIndex(CWProtocolMessage *msgPtr) 
{
	const int ac_Index_length=1;
	CWACNamesWithIndex ACsinfo;
	CWProtocolMessage *msgs;
	int len = 0;
	int i;
	int j;
	int k;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(!CWWTPGetACNameWithIndex(&ACsinfo)){
		return CW_FALSE;
	}
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, ACsinfo.count, {CW_FREE_OBJECT(ACsinfo.ACNameIndex[0].ACName);CW_FREE_OBJECT(ACsinfo.ACNameIndex[1].ACName);CW_FREE_OBJECT(ACsinfo.ACNameIndex);return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	// create message
	for(i = 0; i < ACsinfo.count; i++) {
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], ac_Index_length+strlen(ACsinfo.ACNameIndex[i].ACName), {for(k = 0; k < ACsinfo.count; k++) CW_FREE_OBJECT(ACsinfo.ACNameIndex[k].ACName);CW_FREE_OBJECT(ACsinfo.ACNameIndex);for(k = i-1; k >= 0; k--) { CW_FREE_PROTOCOL_MESSAGE(msgs[k]);}CW_FREE_OBJECT(msgs);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		CWProtocolStore8(&(msgs[i]), ACsinfo.ACNameIndex[i].index); // ID of the AC
		CWProtocolStoreStr(&(msgs[i]), ACsinfo.ACNameIndex[i].ACName); // name of the AC
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE))) {
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			for(i = 0; i < ACsinfo.count; i++) CW_FREE_OBJECT(ACsinfo.ACNameIndex[i].ACName);
			CW_FREE_OBJECT(ACsinfo.ACNameIndex);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		len += msgs[i].offset;
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, {for(k = 0; k < ACsinfo.count; k++) CW_FREE_OBJECT(ACsinfo.ACNameIndex[k].ACName);CW_FREE_OBJECT(ACsinfo.ACNameIndex);for(k = 0; k < ACsinfo.count; k++) { CW_FREE_PROTOCOL_MESSAGE(msgs[k]);}CW_FREE_OBJECT(msgs);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < ACsinfo.count; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);

 	/*
         * They free ACNameIndex, which is an array of CWACNameWithIndexValues,
         * but nobody cares to free the actual strings that were allocated as fields
         * of the CWACNameWithIndexValues structures in the CWWTPGetACNameWithIndex() 
         * function.. Here we take care of this.
         *
         * BUG ML06
         * 16/10/2009 - Donato Capitella 
         */
	CW_FREE_OBJECT(ACsinfo.ACNameIndex[0].ACName);
	CW_FREE_OBJECT(ACsinfo.ACNameIndex[1].ACName);


	CW_FREE_OBJECT(ACsinfo.ACNameIndex);
	
	return CW_TRUE;
}

CWBool CWAssembleMsgElemDiscoveryType(CWProtocolMessage *msgPtr) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	

	CWProtocolStore8(msgPtr, CWWTPGetDiscoveryType());

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE);
}

CWBool CWAssembleMsgElemLocationData(CWProtocolMessage *msgPtr) {
	char *location = NULL;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	location = CWWTPGetLocation();
	
	// create message
	//zengmin add strlen(location)+1 by Coverity 2013-06-09
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, strlen(location)+1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStoreStr(msgPtr, location);
					
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE);	
}

CWBool CWAssembleMsgElemStatisticsTimer(CWProtocolMessage *msgPtr)
{	
	const int statistics_timer_length= 2;
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, statistics_timer_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, CWWTPGetStatisticsTimer());
	
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_STATISTICS_TIMER_CW_TYPE);	
}

CWBool CWAssembleMsgElemWTPBoardData(CWProtocolMessage *msgPtr) 
{
	const int VENDOR_ID_LENGTH = 4; 	//Vendor Identifier is 4 bytes long
	const int TLV_HEADER_LENGTH = 4; 	//Type and Length of a TLV field is 4 byte long 
	CWWTPVendorInfos infos;
	int i, size = 0;
	/*csc test*/
	//unsigned char mac_buf[6]={0x00,0x03,0x7F,0x07,0xD8,0x5E};
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	 // get infos
	if(!CWWTPGetBoardData(&infos)) {
		return CW_FALSE;
	}
	
	//Calculate msg elem size
	size = VENDOR_ID_LENGTH;
	for(i = 0; i < infos.vendorInfosCount; i++) 
		{size += (TLV_HEADER_LENGTH + ((infos.vendorInfos)[i]).length);}
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(msgPtr, ((infos.vendorInfos)[0].vendorIdentifier));
	for(i = 0; i < infos.vendorInfosCount; i++) 
	{
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].type));
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].length));
#if 0 /*old 1.0.11  pei*/
		if((infos.vendorInfos)[i].length == 4) {
			*((infos.vendorInfos)[i].valuePtr) = htonl(*((infos.vendorInfos)[i].valuePtr));
		}
		if((infos.vendorInfos)[i].length == 4)
		CWProtocolStoreRawBytes(msgPtr, (char*) ((infos.vendorInfos)[i].valuePtr), (infos.vendorInfos)[i].length);
		else if((infos.vendorInfos)[i].length == 6)
		CWProtocolStoreRawBytes(msgPtr, (char *)WTPBaseMAC, (infos.vendorInfos)[i].length); //pei 0917 del warning
		else if((infos.vendorInfos)[i].length == 20)
		CWProtocolStoreRawBytes(msgPtr, (char *)WTPBoardID, (infos.vendorInfos)[i].length); //pei 0917 del warning
#endif
		if((infos.vendorInfos)[i].type == CW_WTP_MODEL_NUMBER)
		{
			//zengmin add strlen(WTPModelNum)+1 by Coverity 2013-06-09
			char model_temp[strlen(WTPModelNum)+1];
			strncpy(model_temp, WTPModelNum, strlen(WTPModelNum));
//			printf("strlen(WTPModelNum) = %d\n", strlen(WTPModelNum));
			CWProtocolStoreRawBytes(msgPtr, model_temp, strlen(WTPModelNum)); //pei 0925
		}
		else if((infos.vendorInfos)[i].type == CW_WTP_SERIAL_NUMBER)
		{
			CWProtocolStoreRawBytes(msgPtr, (char *)WTPBoardID, (infos.vendorInfos)[i].length); //pei 0925
		}
		else if((infos.vendorInfos)[i].type == CW_BOARD_MAC_ADDRESS)
		{
			CWProtocolStoreRawBytes(msgPtr, (char *)WTPBaseMAC, (infos.vendorInfos)[i].length); //pei 0925
		}
		else if((infos.vendorInfos)[i].type == CW_WTP_REAL_MODEL_NUMBER)
		{
			CWProtocolStoreRawBytes(msgPtr, (char *)WTPRealModelNum, (infos.vendorInfos)[i].length); //pei 0925
		}
		else if((infos.vendorInfos)[i].type == CW_WTP_CODE_VERSION)
		{
			CWProtocolStoreRawBytes(msgPtr, (char *)gWTPCodeVersion, (infos.vendorInfos)[i].length); //pei 0925
		}
//#if (defined(APXJ)||defined(XHAP93)||defined(XHPB44))
		if(gManuOption!=0){
			if((infos.vendorInfos)[i].type == CW_WTP_MANUFACTURE_OPTION)
			{
				//printf("gManuOption:%d    gManuOption<<16:%d\n", gManuOption, gManuOption<<16);
				CWProtocolStore32(msgPtr, gManuOption<<16);
			}
		}
//#endif		

	}
	CWWTPDestroyVendorInfos(&infos);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPDescriptor(CWProtocolMessage *msgPtr) {
	const int GENERIC_RADIO_INFO_LENGTH = 4;//First 4 bytes for Max Radios, Radios In Use and Encryption Capability 
	const int VENDOR_ID_LENGTH = 4; 	//Vendor Identifier is 4 bytes long
	const int TLV_HEADER_LENGTH = 4; 	//Type and Length of a TLV field is 4 byte long 
	CWWTPVendorInfos infos;
	int i, size = 0;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// get infos
	if(!CWWTPGetVendorInfos(&infos)) { 
		return CW_FALSE;
	}
	
	//Calculate msg elem size
	size = GENERIC_RADIO_INFO_LENGTH;
	for(i = 0; i < infos.vendorInfosCount; i++) 
		{size += (VENDOR_ID_LENGTH + TLV_HEADER_LENGTH + ((infos.vendorInfos)[i]).length);}
		
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, CWWTPGetMaxRadios()); // number of radios supported by the WTP
	CWProtocolStore8(msgPtr, CWWTPGetRadiosInUse()); // number of radios present in the WTP
	CWProtocolStore16(msgPtr, CWWTPGetEncCapabilities()); // encryption capabilities
 
	for(i = 0; i < infos.vendorInfosCount; i++) {
		CWProtocolStore32(msgPtr, ((infos.vendorInfos)[i].vendorIdentifier));
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].type));
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].length));
#if 1
		if((infos.vendorInfos)[i].type == CW_WTP_HARDWARE_VERSION)
		{
			CWProtocolStoreRawBytes(msgPtr, gWTPHardWareVersion, (infos.vendorInfos)[i].length);
			//printf("hardware version len %d.version :%s\n",(infos.vendorInfos)[i].length, gWTPHardWareVersion);
		}
		else if((infos.vendorInfos)[i].type == CW_WTP_SOFTWARE_VERSION)
		{
			CWProtocolStoreRawBytes(msgPtr, gWTPSoftWareVersion, (infos.vendorInfos)[i].length);
			//printf("software version len %d.version :%s\n",(infos.vendorInfos)[i].length, gWTPSoftWareVersion);
		}
		else if((infos.vendorInfos)[i].type == CW_BOOT_VERSION)
		{
			//printf("boot version len %d\n", (infos.vendorInfos)[i].length);
			*((infos.vendorInfos)[i].valuePtr) = autelan_htonl(*((infos.vendorInfos)[i].valuePtr));
			CWProtocolStoreRawBytes(msgPtr, (char*) ((infos.vendorInfos)[i].valuePtr), (infos.vendorInfos)[i].length);
		}
#else
		if((infos.vendorInfos)[i].length == 4) {
			*((infos.vendorInfos)[i].valuePtr) = htonl(*((infos.vendorInfos)[i].valuePtr));
		}
		if((infos.vendorInfos)[i].length == strlen(gWTPSoftWareVersion)){
			CWProtocolStoreRawBytes(msgPtr,gWTPSoftWareVersion, (infos.vendorInfos)[i].length);
		}
		else
		CWProtocolStoreRawBytes(msgPtr, (char*) ((infos.vendorInfos)[i].valuePtr), (infos.vendorInfos)[i].length);
#endif

	}
	
	CWWTPDestroyVendorInfos(&infos);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE);
}

CWBool CWAssembleMsgElemImageIdentifier(CWProtocolMessage *msgPtr) {        //pei add 0616
	int IMAGEIDENTIFIER_LENGTH = 8+gImageIdentifier_ACSupported->length;

	int ModelLen = strlen(WTPModelNum);
	if(gImageIdentifier_ACSupported->type == 1)
	{
		IMAGEIDENTIFIER_LENGTH +=2+ModelLen;
	}

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, IMAGEIDENTIFIER_LENGTH, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(msgPtr, gImageIdentifier_ACSupported->NumOfVersion);

	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore16(msgPtr, 0);

	CWProtocolStore16(msgPtr, gImageIdentifier_ACSupported->type);

	if(gImageIdentifier_ACSupported->type == 1)
	{
		CWProtocolStore16(msgPtr, ModelLen);
		CWProtocolStoreRawBytes(msgPtr, WTPModelNum, ModelLen);
	}

	CWProtocolStore16(msgPtr, gImageIdentifier_ACSupported->length);
	CWProtocolStoreRawBytes(msgPtr, (char *)(gImageIdentifier_ACSupported->values), gImageIdentifier_ACSupported->length); //pei 0917 del warning

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE);
}
CWBool CWAssembleMsgElemWTPFrameTunnelMode(CWProtocolMessage *msgPtr) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, CWWTPGetFrameTunnelMode()); // frame encryption

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPIPv4Address(CWProtocolMessage *msgPtr) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 4, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	CWProtocolStore32(msgPtr, CWWTPGetIPv4Address());

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPMACType(CWProtocolMessage *msgPtr) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, CWWTPGetMACType()); // mode of operation of the WTP (local, split, ...)
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPName(CWProtocolMessage *msgPtr) {
	char *name = NULL;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	name = CWWTPGetName();
	
	// create message
	//zengmin add strlen(name)+1 by Coverity 2013-06-09
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, strlen(name)+1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStoreStr(msgPtr, name);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_NAME_CW_TYPE);	
}
CWBool CWGetAPOnlineTime(unsigned short *OnTime)  //pei add 0213
{	
	char cmd_time[128];
	char str_time[24];
	*OnTime = 0;
/*
	memset(cmd_time, 0, 128);
	sprintf(cmd_time, "date | awk '{print $4}' | awk -F \":\" '{print $1}'");
	fp_time = popen(cmd_time, "r");
	if(fp_time)
	{
		memset(str_time, 0, 10);
		fgets(str_time, sizeof(str_time), fp_time);
		*OnTime += atoi(str_time)*60*60;
		pclose(fp_time);
	}
	memset(cmd_time, 0, 128);
	sprintf(cmd_time, "date | awk '{print $4}' | awk -F \":\" '{print $2}'");
	fp_time = popen(cmd_time, "r");
	if(fp_time)
	{
		memset(str_time, 0, 10);
		fgets(str_time, sizeof(str_time), fp_time);
		*OnTime += atoi(str_time)*60;
		pclose(fp_time);
	}
	memset(cmd_time, 0, 128);
	sprintf(cmd_time, "date | awk '{print $4}' | awk -F \":\" '{print $3}'");
	fp_time = popen(cmd_time, "r");
	if(fp_time)
	{
		memset(str_time, 0, 10);
		fgets(str_time, sizeof(str_time), fp_time);
		*OnTime += atoi(str_time);
		pclose(fp_time);
	}
*/
	
	memset(cmd_time, 0, 128);
	autelan_sprintf(cmd_time, "cat /proc/uptime | awk -F ' ' '{print $1}' | awk -F '.' '{print $1}'");
	PopenFile(cmd_time, str_time, sizeof(str_time));
	*OnTime = atoi(str_time);
	//printf("sum of *OnTime:%d\n", *OnTime);
	
	return CW_TRUE;
}
/*Begin:zengmin add for ap cold or hot boot 2013-03-21*/
CWBool CWGetAPIsColdBoot(unsigned char *IsColdBoot,unsigned char *isWTPRestart)
{
	if(0 ==access("/tmp/kes_load_failed", 0)){
		if(0 ==access("/jffs/sysrebootflag", 0))
		{
			*IsColdBoot = 0;
			_system("rm /jffs/sysrebootflag");
		}
		else if(0 ==access("/tmp/sysrebootflag", 0))
		{
			*IsColdBoot = 0;
			*isWTPRestart = 1;
			_system("rm /tmp/sysrebootflag");
		}
		else
		{
			*IsColdBoot = 1;
		}
	}
	else
	{
		char tmp_str[100];
		char  str[20];
		memset(tmp_str,0,100);
		memset(str,0,20);
		autelan_sprintf(tmp_str, "cat /proc/kes_debug_flag");
		PopenFile(tmp_str, str, sizeof(str));
		switch(str[0]) {
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':	/*add for factoryreset */
			case 'g':	/*add for watchdog  */
			case 'h':	/*add for syscoldreboot */
			case 'i':	/*add for ap83-firmware-upgrade-stage2*/
		//	case 'j':	/*add for ac-lose */
			case 'k':	/*add for boot_upgrade */
			case 'l':	/*add for ac-get */
			case 'm':	/*add for wtpd_check*/
			case 'n':	/*add for redboot-upgrde */
			case 'o':	/*add for ap_monitor */
			case 'p':	/*add for mod_check */
			case 'q':	/*add for sync_version */
			case 'r':	/*add for sysupgrade*/
			case 's':	/*add for upgrade */
			case 't':	/*add for ngupgrade */
			case 'u':	/*add for reset_manufacturer_config */
			case 'v':	/*add for eoc_monitor */
			case 'w':	/*add for upgrade_cbat_stage2 */
			case 'x':	/*add for zt_monitor */
			case 'y':	/*add for autelan-firmware-upgrade-stage2 */
			case 'z':	/*add for watchdog timeout*/
			case 'A':	/*add for bus error*/
			case 'B':	/*add for woc_upgrade*/
			case 'C':	/*add for radio_reset*/
			case 'D':	/*add for boot-firmware-upgrade-stage2*/
			case 'E':	/*add for stuck check*/
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				*IsColdBoot = 0;
				break;
			default:
				*IsColdBoot = 1;
				break;	
		}
		CWWTPDebugLog("kes_debug_flag: %s ", str);
		_system("cp -f /proc/kes_debug_flag /tmp/kes_debug_flag");
		_system("echo 0 > /proc/kes_debug_flag");
		if(0 ==access("/tmp/sysrebootflag", 0))
		{
			*IsColdBoot = 0;
			*isWTPRestart = 1;
			_system("rm /tmp/sysrebootflag");
		}
		if(0 ==access("/jffs/sysrebootflag", 0))
		{
			*IsColdBoot = 0;
			_system("rm /jffs/sysrebootflag");
		}		
	}	
	return CW_TRUE;
}
/*End:zengmin add for ap cold or hot boot 2013-03-21*/

CWBool CWGetAPIPInfo(APIPInfoValues *ipInfo)
{
	char cmd_str[128];
	char str[24];
	char str_tmp[24];
	unsigned int netmask, gw, dnsNum, dns;
	
	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str, "/sbin/ifconfig default | awk -F \" \" '/Mask/ {print $4}' | awk -F \":\" '{print $2}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
	if(inet_atoi(str, &netmask) == 1)
		ipInfo->netmask = netmask;
	CWWTPDebugLog("netmask: %s len:%d", str, (int)strlen(str));

	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str, "route -n | awk -F \" \" '/UG/ {print $2}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
	if(inet_atoi(str, &gw) == 1)
		ipInfo->gateway = gw;
	CWWTPDebugLog("gw: %s len:%d", str, (int)strlen(str));

	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str, "cat /etc/resolv.conf | grep nameserver | wc -l");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
	dnsNum = atoi(str);
	
	dns = 0;
	if(dnsNum >= 1)
	{
		memset(str, 0, 24);
		memset(cmd_str, 0, 128);
		autelan_sprintf(cmd_str, "cat /etc/resolv.conf | grep nameserver | awk '{if(NR==1){print $2}}'");
		PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
		
		strcpy(str, trim(str_tmp));
		if(inet_atoi(str, &dns) == 1)
			ipInfo->dns = dns;
		CWWTPDebugLog("dns: %s len:%d", str, (int)strlen(str));
	}
	dns = 0;
	if(dnsNum == 2)
	{
		memset(str, 0, 24);
		memset(cmd_str, 0, 128);
		autelan_sprintf(cmd_str, "cat /etc/resolv.conf | grep nameserver | awk '{if(NR==2){print $2}}'");
		PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
		strcpy(str, trim(str_tmp));
		if(inet_atoi(str, &dns) == 1)
			ipInfo->vice_dns = dns;
		CWWTPDebugLog("vice_dns: %s len:%d", str, (int)strlen(str));
	}
	return CW_TRUE;
}
CWBool CWGetAPhardInfo(ApHardInfoValues *hardInfo)
{
	char cmd_str[128];
	char str_tmp[32];
	hardInfo->eth_num=1;
	hardInfo->eth_up_speed=100;
	
	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str, "/usr/sbin/get_hardinfo | awk -F \":\" '/cpu_type/ {print $2}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	memcpy(hardInfo->WTPCPUTYPE,str_tmp,strlen(str_tmp));
	dpf("cpu_type: %s len:%d   \n", str_tmp, (int)strlen(str_tmp));
	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str, "/usr/sbin/get_hardinfo | awk -F \":\" '/flash_type/ {print $2}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	memcpy(hardInfo->WTPFLASHTYPE,str_tmp,strlen(str_tmp));
	dpf("fp_flash_type: %s len:%d  \n", str_tmp, (int)strlen(str_tmp));
#if 0
		memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \"[:M]\" '/flash_size/ {print $2}'");
	fp_flash_info = popen(cmd_str, "r");
	if(fp_flash_info)
		memset(str_tmp, 0, 32);
		fgets(str_tmp, sizeof(str_tmp), fp_flash_info);
		str_tmp[strlen(str_tmp)-1] = '\0';
		hardInfo->flashsize=atoi(str_tmp);
		dpf("flash_size: %d  \n", hardInfo->flashsize);
		pclose(fp_flash_info);
#endif

	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \":\" '/mem_type/ {print $2}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	memcpy(hardInfo->WTPMEMTYPE,str_tmp,strlen(str_tmp));
	dpf("fp_mem_type: %s len:%d    \n", str_tmp, (int)strlen(str_tmp));
#if 0	
	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \"[:M]\" '/mem_size/ {print $2}'");
	fp_mem_info = popen(cmd_str, "r");
	if(fp_mem_info)
		memset(str_tmp, 0, 32);
		fgets(str_tmp, sizeof(str_tmp), fp_mem_info);
		str_tmp[strlen(str_tmp)-1] = '\0';
		hardInfo->memsize=atoi(str_tmp);
		dpf("mem_size: %d \n", hardInfo->memsize);
		pclose(fp_mem_info);
#endif
	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \":\" '/eth_num/ {print $2}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	hardInfo->eth_num=atoi(str_tmp);
	dpf("eth_num: %d  \n", hardInfo->eth_num);

	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str,  "/usr/sbin/get_eth_speed| awk -F \"[:M]\" '{print $2}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	hardInfo->eth_up_speed=atoi(str_tmp);
	dpf("eth_up_speed: %d  \n", hardInfo->eth_up_speed);
#ifdef AP2400_IFM
	int speed_tmp=0;
	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str,  "/usr/bin/ethreg eth0 |awk -F \"x\" '{print $3}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	speed_tmp=atoi(str_tmp);
	switch(speed_tmp){
		case 0:
			hardInfo->eth_up_speed=10;
			break;
		case 1:
			hardInfo->eth_up_speed=100;
			break;
		case 2:
			hardInfo->eth_up_speed=1000;
			break;
		default:
			hardInfo->eth_up_speed=1000;
			break;
	}
	dpf("eth_up_speed: %d  \n", hardInfo->eth_up_speed);
#endif	
#if (defined(AQ2000N)||defined(APXJ))
	int speed_tmp=0;
	memset(cmd_str, 0, 128);
	autelan_sprintf(cmd_str,  "/usr/bin/ethreg -p 2 offset |awk -F \"x\" '{print $3}'");
	PopenFile(cmd_str, str_tmp, sizeof(str_tmp));
	speed_tmp=atoi(str_tmp);
	switch(speed_tmp){
		case 1000:
			hardInfo->eth_up_speed=100;
			break;
		default:
			hardInfo->eth_up_speed=10;
			break;
	}
	dpf("eth_up_speed: %d  \n", hardInfo->eth_up_speed);
#endif	
	return CW_TRUE;
}
#if 0
CWBool CWAssembleMsgElemWTPOperationalStatistics(CWProtocolMessage *msgPtr, int radio)
{
	const int operational_statistics_length= 4;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(radio<0 || radio>=gRadiosInfo.radioCount) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, operational_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, radio);
	CWProtocolStore8(msgPtr, gRadiosInfo.radiosInfo[radio].TxQueueLevel);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].wirelessLinkFramesPerSec);


	CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE);
	
	return CW_TRUE;	
}
#endif
CWBool CWAssembleMsgElemWTPOperationalStatistics(CWProtocolMessage *msgPtr, int radio)
{
	const int operational_statistics_length= 7+16+96+5;
	unsigned short on_time = 100;
	unsigned char cold_boot = 0;
	unsigned char isWTPRestart = 0; //zeng add 20130110
	APIPInfoValues ipInfo;
	ApHardInfoValues hardInfo;
	
	on_time = gPowerOnUntilRuntime;
	CWGetAPIsColdBoot(&cold_boot,&isWTPRestart);
	/*Begin:zeng add save log to jffs  2013-01-10*/
	if((cold_boot == 0) && (isWTPRestart == 0)){
		CWLog("AP is hot boot.");
	}
	else if((cold_boot == 0) && (isWTPRestart == 1)){
		CWLog("WTPD Restart.");
		}
	else if(cold_boot == 1){
		CWLog("AP is cold boot.");
		}
	/*End:zeng add save log to jffs 2013-01-10*/
	memset(&ipInfo, 0, sizeof(APIPInfoValues));
	memset(&hardInfo, 0, sizeof(ApHardInfoValues));
	CWGetAPIPInfo(&ipInfo);
	CWGetAPhardInfo(&hardInfo);
	
	CWWTPDebugLog("on_time:%d cold_boot:%d", on_time, cold_boot);

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, operational_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore16(msgPtr, 0);
	CWProtocolStore16(msgPtr, on_time);
	CWProtocolStore8(msgPtr, cold_boot);
	CWProtocolStore32(msgPtr, ipInfo.netmask);
	CWProtocolStore32(msgPtr, ipInfo.gateway);
	CWProtocolStore32(msgPtr, ipInfo.dns);
	CWProtocolStore32(msgPtr, ipInfo.vice_dns);
	CWProtocolStoreRawBytes(msgPtr, hardInfo.WTPCPUTYPE, 32);
	CWProtocolStoreRawBytes(msgPtr, hardInfo.WTPFLASHTYPE, 32);
	CWProtocolStoreRawBytes(msgPtr, hardInfo.WTPMEMTYPE, 32);
	CWProtocolStore8(msgPtr, hardInfo.eth_num);
	CWProtocolStore32(msgPtr, hardInfo.eth_up_speed);

	CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE);
	
	return CW_TRUE;	
}


CWBool CWAssembleMsgElemWTPRadioStatistics(CWProtocolMessage *msgPtr, int radio)
{
	const int radio_statistics_length= 20;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(radio<0 || radio>gRadiosInfo.radioCount) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, radio_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	CWProtocolStore8(msgPtr, radio);
	CWProtocolStore8(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.lastFailureType);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.resetCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.SWFailureCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.HWFailuireCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.otherFailureCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.unknownFailureCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.configUpdateCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.channelChangeCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.bandChangeCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.currentNoiseFloor);


	//return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE);

	CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE);
	
	return CW_TRUE;
}

CWBool CWAssembleMsgElemWTPRebootStatistics(CWProtocolMessage *msgPtr)
{
	const int reboot_statistics_length= 15;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, reboot_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.rebootCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.ACInitiatedCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.linkFailurerCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.SWFailureCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.HWFailuireCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.otherFailureCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.unknownFailureCount);
	CWProtocolStore8(msgPtr, gWTPRebootStatistics.lastFailureType);


	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE);
}

CWBool CWAssembleMsgElemDeleteStation(CWProtocolMessage *msgPtr, DeleteSTAValues *valuesPtr)             //pei add 0703
{
//	int i;
	const int delete_station_length = 8;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, delete_station_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
/*
	if(debug_print)
	{
		printf("-----------------assembling WTP Event Request Delete Station Request\n");
		printf("valuesPtr->radio_id = %d\n", valuesPtr->radio_id);
		printf("valuesPtr->mac_length = %d\n", valuesPtr->mac_length);
		printf("valuesPtr->mac_addr = ");
		for (i = 0; i < 6; i++){
		printf("%02x", valuesPtr->mac_addr[i]);
		}
		printf("\n");
	}
*/	
	CWProtocolStore8(msgPtr, valuesPtr->radio_id);
	CWProtocolStore8(msgPtr, valuesPtr->mac_length);
	CWProtocolStoreRawBytes(msgPtr, (char *)(valuesPtr->mac_addr), 6); //pei 0917 del warning
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE);
}

#if 0
CWBool CWAssembleMsgElemVendorSpecificPayload(CWProtocolMessage *msgPtr, int interface_count, wlan_stats_info *valuesPtr)             //pei add 0205
{
	int k;
	const int vendor_specific_payload_length = 1+137*interface_count;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, interface_count);
	for(k=0;k<interface_count;k++)
	{
		if(debug_print)
		{
			int i=0;
			printf("---assembling WTP Event Request Vendor Specific Payload Request.\n");
			printf("type:%d\n", valuesPtr[k].type);
			printf("radioId:%d\n", valuesPtr[k].radioId);
			printf("wlanId:%d\n", valuesPtr[k].wlanId);
			printf("mac:");
			for (i = 0; i < 6; i++){
				printf("%02x ", valuesPtr[k].mac[i]);
			}
			printf("\n");
			printf("rx_packets:%d\n", valuesPtr[k].rx_packets);
			printf("tx_packets:%d\n", valuesPtr[k].tx_packets);
			printf("rx_bytes:%llu\n", valuesPtr[k].rx_bytes);
			printf("tx_bytes:%llu\n", valuesPtr[k].tx_bytes);
			printf("rx_errors:%d\n", valuesPtr[k].rx_errors);
			printf("tx_errors:%d\n", valuesPtr[k].tx_errors);
			printf("rx_rate:%d\n", valuesPtr[k].rx_rate);
			printf("tx_rate:%d\n", valuesPtr[k].tx_rate);
			printf("ast_rx_crcerr:%d\n", valuesPtr[k].ast_rx_crcerr);
			printf("ast_rx_badcrypt:%d\n", valuesPtr[k].ast_rx_badcrypt);
			printf("ast_rx_badmic:%d\n", valuesPtr[k].ast_rx_badmic);
			printf("ast_rx_phyerr:%d\n", valuesPtr[k].ast_rx_phyerr);
			printf("rx_dropped:%d\n", valuesPtr[k].rx_dropped);
			printf("tx_dropped:%d\n", valuesPtr[k].tx_dropped);
			printf("rx_multicast:%d\n", valuesPtr[k].rx_multicast);
			printf("tx_multicast:%d\n", valuesPtr[k].tx_multicast);
			printf("rx_broadcast:%d\n", valuesPtr[k].rx_broadcast);			
			printf("tx_broadcast:%d\n", valuesPtr[k].tx_broadcast);
			printf("rx_unicast:%d\n", valuesPtr[k].rx_unicast);
			printf("tx_unicast:%d\n", valuesPtr[k].tx_unicast);
			printf("rx_multicast_bytes:%d\n", valuesPtr[k].rx_multicast_bytes);
			printf("tx_multicast_bytes:%d\n", valuesPtr[k].tx_multicast_bytes);
			printf("rx_unicast_bytes:%d\n", valuesPtr[k].rx_unicast_bytes);
			printf("tx_unicast_bytes:%d\n", valuesPtr[k].tx_unicast_bytes);
			printf("rx_retry_bytes:%d\n", valuesPtr[k].rx_retry_bytes);
			printf("tx_retry_bytes:%d\n", valuesPtr[k].tx_retry_bytes);
			printf("rx_retry:%d\n", valuesPtr[k].rx_retry);
			printf("tx_retry:%d\n", valuesPtr[k].tx_retry);	
			printf("rx_mgmt:%d\n", valuesPtr[k].rx_mgmt);
			printf("tx_mgmt:%d\n\n", valuesPtr[k].tx_mgmt);
		}
		CWProtocolStore8(msgPtr, valuesPtr[k].type);
		CWProtocolStore8(msgPtr, valuesPtr[k].radioId);
		CWProtocolStore8(msgPtr, valuesPtr[k].wlanId);
		CWProtocolStoreRawBytes(msgPtr, (char *)(valuesPtr[k].mac), 6);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_packets);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_packets);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_errors);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_errors);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_bytes);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_bytes);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_rate);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_rate);
		CWProtocolStore32(msgPtr, valuesPtr[k].ast_rx_crcerr);
		CWProtocolStore32(msgPtr, valuesPtr[k].ast_rx_badcrypt);
		CWProtocolStore32(msgPtr, valuesPtr[k].ast_rx_badmic);
		CWProtocolStore32(msgPtr, valuesPtr[k].ast_rx_phyerr);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_dropped);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_dropped);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_multicast);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_multicast);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_broadcast);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_broadcast);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_unicast);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_unicast);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_multicast_bytes);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_multicast_bytes);
		CWProtocolStore32(msgPtr, 0); /*save for broadcast_bytes*/
		CWProtocolStore32(msgPtr, 0);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_unicast_bytes);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_unicast_bytes);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_retry_bytes);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_retry_bytes);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_retry);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_retry);
		CWProtocolStore32(msgPtr, valuesPtr[k].rx_mgmt);
		CWProtocolStore32(msgPtr, valuesPtr[k].tx_mgmt);
	}
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}
#endif

CWBool CWAssembleMsgElemIEEE80211DirectSequenceControl(CWProtocolMessage *msgPtr)               //pei add 0623
{
	const int direct_sequence_control_length = 8;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, direct_sequence_control_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, 0);     //radio ID  ***
	CWProtocolStore8(msgPtr, 0);     //reserved
	CWProtocolStore8(msgPtr, 0);	    //current channel ***
	CWProtocolStore8(msgPtr, 0);     //current cca
	CWProtocolStore32(msgPtr, 0);   //

	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_DIRECT_SEQUEUE_CONTROL);
}

CWBool CWAssembleMsgElemIEEE80211TxPower(CWProtocolMessage *msgPtr)               //pei add 0623
{
	const int tx_power_length = 4;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, tx_power_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, 0);     //radio ID ***
	CWProtocolStore8(msgPtr, 0);     //reserved
	CWProtocolStore16(msgPtr, 0);	    //current tx power ***

	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_TX_POWER);
}

CWBool CWAssembleMsgElemIEEE80211WTPRadioInfo(CWProtocolMessage *msgPtr)               //pei add 0623
{
	const int radio_info_length = 5;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, radio_info_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, 0);     //radio ID
	CWProtocolStore32(msgPtr, 0);     //radio type

	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_IEEE80211_RADIO_INFO);
}
//test version
CWBool CWAssembleMsgElemDuplicateIPv4Address(CWProtocolMessage *msgPtr) {
	const int duplicate_ipv4_length= 11;
	char *macAddress;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, duplicate_ipv4_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	
	CWProtocolStore32(msgPtr, CWWTPGetIPv4Address());

	CWProtocolStore8(msgPtr, CWWTPGetIPv4StatusDuplicate());

	CWProtocolStore8(msgPtr, 6);

	CW_CREATE_ARRAY_ERR(macAddress, 6, char, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	macAddress[0] = 103;
	macAddress[1] = 204;
	macAddress[2] = 204;
	macAddress[3] = 190;
	macAddress[4] = 180;
	macAddress[5] = 0;
	
	CWProtocolStoreRawBytes(msgPtr, macAddress, 6);
	CW_FREE_OBJECT(macAddress);

	//CWProtocolStore8(msgPtr, CWWTPGetIPv4StatusDuplicate());

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE);
}

//test version
CWBool CWAssembleMsgElemDuplicateIPv6Address(CWProtocolMessage *msgPtr) {
	const int duplicate_ipv6_length= 23;
	char *macAddress;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, duplicate_ipv6_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	
	struct sockaddr_in6 myAddr;
	CWWTPGetIPv6Address(&myAddr);
	CWProtocolStoreRawBytes(msgPtr, (char*)myAddr.sin6_addr.s6_addr, 16);

	CWProtocolStore8(msgPtr, CWWTPGetIPv6StatusDuplicate());
	
	CWProtocolStore8(msgPtr, 6);

	CW_CREATE_ARRAY_ERR(macAddress, 6, char, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	macAddress[0] = 103;
	macAddress[1] = 204;
	macAddress[2] = 204;
	macAddress[3] = 190;
	macAddress[4] = 180;
	macAddress[5] = 0;
	
	CWProtocolStoreRawBytes(msgPtr, macAddress, 6);
	CW_FREE_OBJECT(macAddress);

	//CWProtocolStore8(msgPtr, CWWTPGetIPv6StatusDuplicate());

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE);
}



CWBool CWAssembleMsgElemRadioAdminState(CWProtocolMessage *msgPtr) 
{
	const int radio_Admin_State_Length=2;
	CWRadiosAdminInfo infos;
	CWProtocolMessage *msgs;
	int len = 0;
	int i;
	int j;
	int k;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(!CWGetWTPRadiosAdminState(&infos)) {
		return CW_FALSE;
	}
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, (infos.radiosCount), {CW_FREE_OBJECT(infos.radios);return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], radio_Admin_State_Length, {for(k = i-1; k >= 0; k--) { CW_FREE_PROTOCOL_MESSAGE(msgs[k]);}CW_FREE_OBJECT(msgs);CW_FREE_OBJECT(infos.radios);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].state); // state of the radio
		//CWProtocolStore8(&(msgs[i]), infos.radios[i].cause);
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE))) {
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(infos.radios);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, {for(k = 0; k < infos.radiosCount; k++) { CW_FREE_PROTOCOL_MESSAGE(msgs[k]);}CW_FREE_OBJECT(msgs);CW_FREE_OBJECT(infos.radios);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);
	CW_FREE_OBJECT(infos.radios);

	return CW_TRUE;
}

//if radioID is negative return Radio Operational State for all radios
CWBool CWAssembleMsgElemRadioOperationalState(int radioID, CWProtocolMessage *msgPtr) 
{
	const int radio_Operational_State_Length=3;
	CWRadiosOperationalInfo infos;
	CWProtocolMessage *msgs;
	int len = 0;
	int i;
	int k;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(!(CWGetWTPRadiosOperationalState(radioID,&infos))) {
		return CW_FALSE;
	}

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, (infos.radiosCount), {CW_FREE_OBJECT(infos.radios);return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], radio_Operational_State_Length, {for(k = i-1; k >= 0; k--) { CW_FREE_PROTOCOL_MESSAGE(msgs[k]);}CW_FREE_OBJECT(msgs);CW_FREE_OBJECT(infos.radios);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].state); // state of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].cause);
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE))) {
			int j;
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(infos.radios);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, {for(k = 0; k < infos.radiosCount; k++) { CW_FREE_PROTOCOL_MESSAGE(msgs[k]);}CW_FREE_OBJECT(msgs);CW_FREE_OBJECT(infos.radios);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);
	CW_FREE_OBJECT(infos.radios);

	return CW_TRUE;
}

CWBool CWAssembleMsgElemDecryptErrorReport(CWProtocolMessage *msgPtr, int radioID) 
{
	int decrypy_Error_Report_Length=0;
	CWDecryptErrorReportInfo infos;
	CWProtocolMessage *msgs;
	int len = 0;
	int i;
	int k;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(!(CWGetDecryptErrorReport(radioID,&infos))) {
		return CW_FALSE;
	}

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, (infos.radiosCount), {for(k=0; k<infos.radiosCount; k++) {CW_FREE_OBJECT(infos.radios[k].decryptErrorMACAddressList);}CW_FREE_OBJECT(infos.radios);return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		decrypy_Error_Report_Length = 2 + sizeof(CWMACAddress)*(infos.radios[i].numEntries); 
		
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], decrypy_Error_Report_Length, {for(k=0; k<infos.radiosCount; k++) {CW_FREE_OBJECT(infos.radios[k].decryptErrorMACAddressList);}for(k = i-1; k >= 0; k--) { CW_FREE_PROTOCOL_MESSAGE(msgs[k]);}CW_FREE_OBJECT(msgs);CW_FREE_OBJECT(infos.radios);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].numEntries); // state of the radio

		CWProtocolStore8(&(msgs[i]), (unsigned char)sizeof(CWMACAddress)*(infos.radios[i].numEntries));

		CWProtocolStoreRawBytes(&(msgs[i]), (char*)*(infos.radios[i].decryptErrorMACAddressList), sizeof(CWMACAddress)*(infos.radios[i].numEntries));
		
		
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE))) {
			int j;
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			for(j=0; j<infos.radiosCount; j++) {CW_FREE_OBJECT(infos.radios[j].decryptErrorMACAddressList);}
			CW_FREE_OBJECT(infos.radios);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, {for(k=0; k<infos.radiosCount; k++) {CW_FREE_OBJECT(infos.radios[k].decryptErrorMACAddressList);}for(k = 0; k < infos.radiosCount; k++) { CW_FREE_PROTOCOL_MESSAGE(msgs[k]);}CW_FREE_OBJECT(msgs);CW_FREE_OBJECT(infos.radios);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	for(i = 0; i < infos.radiosCount; i++) {
		CW_FREE_OBJECT(infos.radios[i].decryptErrorMACAddressList);
	}
	
	CW_FREE_OBJECT(msgs);
	CW_FREE_OBJECT(infos.radios);

	return CW_TRUE;
	
}

/*
CWBool CWAssembleMsgElemWTPRadioInformation(CWProtocolMessage *msgPtr) {
	CWProtocolMessage *msgs;
	CWRadiosInformation infos;
	
	int len = 0;
	int i;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	if(!CWWTPGetRadiosInformation(&infos)) {
		return CW_FALSE;
	}
	
	// create one message element for each radio
	
	CW_CREATE_ARRAY_ERR(msgs, (infos.radiosCount), CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], 5, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore32(&(msgs[i]), infos.radios[i].type); // type of the radio
		
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_WTP_RADIO_INFO_CW_TYPE))) {
			int j;
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(infos.radios);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
	}
	
	// return all the messages as one big message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);
	CW_FREE_OBJECT(infos.radios);
	
	return CW_TRUE;
}
*/

/*_________________________________________________________________________*/
/*  *****************************___PARSE___*****************************  */
CWBool CWParseACDescriptor(CWProtocolMessage *msgPtr, int len, CWACInfoValues *valPtr) {
	int i=0, theOffset=0;
		
	CWParseMessageElementStart();
	
	valPtr->stations= CWProtocolRetrieve16(msgPtr);
	
	valPtr->limit	= CWProtocolRetrieve16(msgPtr);
	
	valPtr->activeWTPs= CWProtocolRetrieve16(msgPtr);
	
	valPtr->maxWTPs	= CWProtocolRetrieve16(msgPtr);
	
	valPtr->security= CWProtocolRetrieve8(msgPtr);
	
	valPtr->RMACField= CWProtocolRetrieve8(msgPtr);

//	valPtr->WirelessField= CWProtocolRetrieve8(msgPtr);

	CWProtocolRetrieve8(msgPtr);			//Reserved

	valPtr->DTLSPolicy= CWProtocolRetrieve8(msgPtr); // DTLS Policy

	valPtr->vendorInfos.vendorInfosCount = 0;
	
	theOffset = msgPtr->offset;
	
	// see how many vendor ID we have in the message
	while((msgPtr->offset-oldOffset) < len) {	// oldOffset stores msgPtr->offset's value at the beginning of this function.
							// See the definition of the CWParseMessageElementStart() macro.
		int tmp, id=0, type=0;		


		id=CWProtocolRetrieve32(msgPtr);
		
		type=CWProtocolRetrieve16(msgPtr);
		
		tmp = CWProtocolRetrieve16(msgPtr);
		msgPtr->offset += tmp; // len
		valPtr->vendorInfos.vendorInfosCount++;
	}
	
	msgPtr->offset = theOffset;
	
	// actually read each vendor ID
	CW_CREATE_ARRAY_ERR(valPtr->vendorInfos.vendorInfos, valPtr->vendorInfos.vendorInfosCount, CWACVendorInfoValues,
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	for(i = 0; i < valPtr->vendorInfos.vendorInfosCount; i++) {
		(valPtr->vendorInfos.vendorInfos)[i].vendorIdentifier = CWProtocolRetrieve32(msgPtr);
		(valPtr->vendorInfos.vendorInfos)[i].type = CWProtocolRetrieve16(msgPtr);																
		(valPtr->vendorInfos.vendorInfos)[i].length = CWProtocolRetrieve16(msgPtr);
		(valPtr->vendorInfos.vendorInfos)[i].valuePtr = (int*) (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos.vendorInfos)[i].length));
		
		if((valPtr->vendorInfos.vendorInfos)[i].valuePtr == NULL) return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
		
		if((valPtr->vendorInfos.vendorInfos)[i].length == 4) {
			*((valPtr->vendorInfos.vendorInfos)[i].valuePtr) = autelan_ntohl(*((valPtr->vendorInfos.vendorInfos)[i].valuePtr));
		}
		
	}		
	CWParseMessageElementEnd();
}

CWBool CWParseACIPv4List(CWProtocolMessage *msgPtr, int len, ACIPv4ListValues *valPtr) {
	int i;
	CWParseMessageElementStart();
	
	if(len == 0 || ((len % 4) != 0)) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Malformed AC IPv4 List Messame Element");
	
	valPtr->ACIPv4ListCount = (len/4);
	
	CW_CREATE_ARRAY_ERR(valPtr->ACIPv4List, valPtr->ACIPv4ListCount, int, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < valPtr->ACIPv4ListCount; i++) {
		struct sockaddr_in addr;
		(valPtr->ACIPv4List)[i] = CWProtocolRetrieve32(msgPtr);
		addr.sin_addr.s_addr = (valPtr->ACIPv4List)[i];
		addr.sin_family = AF_INET;
		addr.sin_port = 1024;
	}
	
	CWParseMessageElementEnd();
}

CWBool CWParseACIPv6List(CWProtocolMessage *msgPtr, int len, ACIPv6ListValues *valPtr) 
{
	int i;
	CWParseMessageElementStart();
	
	if(len == 0 || ((len % 16) != 0)) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Malformed AC IPv6 List Messame Element");
	
	valPtr->ACIPv6ListCount = (len/16);
	
	CW_CREATE_ARRAY_ERR(valPtr->ACIPv6List, valPtr->ACIPv6ListCount, struct in6_addr, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < valPtr->ACIPv6ListCount; i++) {
		struct sockaddr_in6 addr;
//		CW_COPY_MEMORY(&((valPtr->ACIPv6List)[i]), CWProtocolRetrieveRawBytes(msgPtr, 16), 16); /* del for fix bug, capwap0.93.3 */
		/*
		 * BUG ML09
		 * 19/10/2009 - Donato Capitella
		 */
		void *ptr = NULL;
		ptr =  CWProtocolRetrieveRawBytes(msgPtr, 16);
		CW_COPY_MEMORY(&((valPtr->ACIPv6List)[i]), ptr, 16);
		CW_FREE_OBJECT(ptr);
		CW_COPY_MEMORY(&(addr.sin6_addr), &((valPtr->ACIPv6List)[i]), 16);
		addr.sin6_family = AF_INET6;
		addr.sin6_port = autelan_htons(CW_CONTROL_PORT);
		
	}
	
	CWParseMessageElementEnd();
}

CWBool CWParseCWControlIPv4Addresses(CWProtocolMessage *msgPtr, int len, CWProtocolIPv4NetworkInterface *valPtr) {
	CWParseMessageElementStart();

	valPtr->addr.sin_addr.s_addr = autelan_htonl(CWProtocolRetrieve32(msgPtr));
	valPtr->addr.sin_family = AF_INET;
	valPtr->addr.sin_port = autelan_htons(CW_CONTROL_PORT);
	
	
	valPtr->WTPCount = CWProtocolRetrieve16(msgPtr);
	
	CWParseMessageElementEnd();
}

CWBool CWParseCWControlIPv6Addresses(CWProtocolMessage *msgPtr, int len, CWProtocolIPv6NetworkInterface *valPtr) {
	CWParseMessageElementStart();

//	CW_COPY_MEMORY(&(valPtr->addr.sin6_addr), CWProtocolRetrieveRawBytes(msgPtr, 16), 16);
	/*
	 * BUG ML09
	 * 19/10/2009 - Donato Capitella
	 * 14/3/2011 - Pei Wenhui
	 */
	void *ptr = NULL;
	ptr =  CWProtocolRetrieveRawBytes(msgPtr, 16);
	CW_COPY_MEMORY(&(valPtr->addr.sin6_addr), ptr, 16);
	CW_FREE_OBJECT(ptr);
	
	valPtr->addr.sin6_family = AF_INET6;
	valPtr->addr.sin6_port = autelan_htons(CW_CONTROL_PORT);
	
	
	valPtr->WTPCount = CWProtocolRetrieve16(msgPtr);
	
	CWParseMessageElementEnd();
}

CWBool CWParseCWTimers (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->discoveryTimer = CWProtocolRetrieve8(msgPtr);
	valPtr->echoRequestTimer = CWProtocolRetrieve8(msgPtr);
	
	CWParseMessageElementEnd();
}

CWBool CWParseDecryptErrorReportPeriod (CWProtocolMessage *msgPtr, int len, WTPDecryptErrorReportValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->radioID = CWProtocolRetrieve8(msgPtr);
	valPtr->reportInterval = CWProtocolRetrieve16(msgPtr);
	
	CWParseMessageElementEnd();
}

CWBool CWParseIdleTimeout (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->idleTimeout = CWProtocolRetrieve32(msgPtr);	
		
	CWParseMessageElementEnd();
}

CWBool CWParseWTPFallback (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->fallback = CWProtocolRetrieve8(msgPtr);	
		
	CWParseMessageElementEnd();
}

CWBool CWParseIeee80211DirectSequenceControl (CWProtocolMessage *msgPtr, int len, DirectSequeueControlValues *valPtr)        //pei add 0623
{
	CWParseMessageElementStart();
	
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	msgPtr->offset +=1;  //skip reserved
	valPtr->current_channel = CWProtocolRetrieve8(msgPtr);
//	printf("valPtr->current_channel = %d\n", valPtr->current_channel);
	valPtr->currend_CCA = CWProtocolRetrieve8(msgPtr);
	valPtr->energy_detect_threshold = CWProtocolRetrieve32(msgPtr);
	
	CWParseMessageElementEnd();
}

CWBool CWParseIeee80211TxPower (CWProtocolMessage *msgPtr, int len, TxPowerValues *valPtr)        //pei add 0623
{
	CWParseMessageElementStart();
	
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
//	msgPtr->offset +=1;  //skip reserved
	valPtr->reserved = CWProtocolRetrieve8(msgPtr);
	valPtr->current_power = CWProtocolRetrieve16(msgPtr);
	CWWTPDebugLog("radio_id:%d Txpower:%d", valPtr->radio_id, valPtr->current_power);
	
	CWParseMessageElementEnd();
}
CWBool CWParseRadioAdminState (CWProtocolMessage *msgPtr, int len, AdminStateValues *valPtr)        //pei add 0724
{
	CWParseMessageElementStart();
	
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->admin_state = CWProtocolRetrieve8(msgPtr);
//	printf("valPtr->radio_id = %d valPtr->admin_state = %d\n", valPtr->radio_id, valPtr->admin_state);
	
	CWParseMessageElementEnd();
}
CWBool CWParseWTPStaticIP (CWProtocolMessage *msgPtr, int len, WTPStaticIPValues *valPtr)        //pei add 0214
{
	CWParseMessageElementStart();

	valPtr->IP_addr = CWProtocolRetrieve32(msgPtr);
	valPtr->netmask = CWProtocolRetrieve32(msgPtr);
	valPtr->gateway = CWProtocolRetrieve32(msgPtr);
	valPtr->static_IP_enable = CWProtocolRetrieve8(msgPtr);

//	printf("static_IP_enable:%d IP_addr:%d netmask:%d gateway:%d\n", valPtr->static_IP_enable, valPtr->IP_addr, valPtr->netmask, valPtr->gateway);
	CWParseMessageElementEnd();
}
CWBool CWParseWTPStaticIPV2 (CWProtocolMessage *msgPtr, int len, WTPStaticIPValues *valPtr)
{
	CWParseMessageElementStart();

	valPtr->IP_addr = CWProtocolRetrieve32(msgPtr);
	valPtr->netmask = CWProtocolRetrieve32(msgPtr);
	valPtr->gateway = CWProtocolRetrieve32(msgPtr);
	valPtr->firstdns = CWProtocolRetrieve32(msgPtr);
	valPtr->secdns = CWProtocolRetrieve32(msgPtr);
	valPtr->static_IP_enable = CWProtocolRetrieve8(msgPtr);
	msgPtr->offset +=3;  //skip reserved

	CWWTPDebugLog("IP_addr:%u netmask:%u gateway:%u firstdns:%u secdns:%u", valPtr->IP_addr, valPtr->netmask, valPtr->gateway, valPtr->firstdns, valPtr->secdns);
	CWParseMessageElementEnd();
}
CWBool acwparse_eth_config_mtu (CWProtocolMessage *msgPtr, int len, set_eth_mtu_vlaues*valPtr)
{
	CWParseMessageElementStart();
	msgPtr->offset +=1; //skip optype
	valPtr->eth_id= CWProtocolRetrieve8(msgPtr);
	valPtr->mtu= CWProtocolRetrieve32(msgPtr);
	msgPtr->offset +=len-6;  //skip reserved
	
	
	CWParseMessageElementEnd();
}
CWBool acwparse_time_config(CWProtocolMessage *msgPtr, int len, set_all_time_vlaues*valPtr)
{
	CWParseMessageElementStart();
	msgPtr->offset +=1; //skip optype
	msgPtr->offset +=1; //skip Reserve
	valPtr->timeinterval= CWProtocolRetrieve16(msgPtr);
	msgPtr->offset +=len-4;  //skip reserved
	
	
	CWParseMessageElementEnd();
}
CWBool CWParseWTPModeNPara(CWProtocolMessage *msgPtr, int len, WTPModeNParaValues *valPtr)
{
	CWParseMessageElementStart();

	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->guard_interval = CWProtocolRetrieve16(msgPtr);
	valPtr->mcs = CWProtocolRetrieve16(msgPtr);
	valPtr->cwmmode = CWProtocolRetrieve16(msgPtr);
	msgPtr->offset += len-7;  //skip reserved

	CWWTPDebugLog("len:%d radio_id:%d guard_interval:%d mcs:%d cwmmode:%d", len, valPtr->radio_id, valPtr->guard_interval, valPtr->mcs, valPtr->cwmmode);
	
	CWParseMessageElementEnd();
}
CWBool CWParseApScanningSet (CWProtocolMessage *msgPtr, int len, ApScanningSetValues *valPtr)        //pei add 1118
{
	CWParseMessageElementStart();
	
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->reportinterval = CWProtocolRetrieve16(msgPtr);
	CWWTPDebugLog("***value:%d  opstate:%d  reportinterval:%d", valPtr->value, valPtr->opstate, valPtr->reportinterval);
	
	CWParseMessageElementEnd();
}
CWBool CWParseStaThroughputSet(CWProtocolMessage *msgPtr, int len, StaThroughputSetValues*valPtr)
{
	CWParseMessageElementStart();

	valPtr->optype= CWProtocolRetrieve8(msgPtr);
	valPtr->len = CWProtocolRetrieve16(msgPtr);
	msgPtr->offset += 1;  //skip reserved
	valPtr->radioid= CWProtocolRetrieve8(msgPtr);
	valPtr->wlanid= CWProtocolRetrieve8(msgPtr);
	valPtr->opstate= CWProtocolRetrieve16(msgPtr);
	valPtr->stainterval= CWProtocolRetrieve32(msgPtr);
	valPtr->throughput_threshold= CWProtocolRetrieve32(msgPtr);

	if(valPtr->len>13)
		msgPtr->offset += valPtr->len-13;  //skip reserved

	CWWTPDebugLog("***optype:%d  opstate:%d  stainterval:%d  throughput_threshold:%d", valPtr->optype, valPtr->opstate, valPtr->stainterval,
		valPtr->throughput_threshold);

	CWParseMessageElementEnd();
}
CWBool CWParseCollectApThroughputInfoSet (CWProtocolMessage *msgPtr, int len, ApThroughputInfoValues *valPtr)        //pei add 0204
{
	CWParseMessageElementStart();
	
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	CWWTPDebugLog("***value:%d  opstate:%d", valPtr->value, valPtr->opstate);
	
	CWParseMessageElementEnd();
}
CWBool CWParseMaxThroughput (CWProtocolMessage *msgPtr, int len, ApMaxThroughputValues *valPtr)        //pei add 0204
{
	unsigned char value;
	int i = 0;
	
	CWParseMessageElementStart();
	value = CWProtocolRetrieve8(msgPtr);
	valPtr->apMaxThroughput = CWProtocolRetrieve8(msgPtr);
	valPtr->wlanCount = CWProtocolRetrieve8(msgPtr);
	CWWTPDebugLog("value:%d runState maxThroughput:%d wlanCount:%d", value, valPtr->apMaxThroughput, valPtr->wlanCount);
	CW_CREATE_ARRAY_ERR(valPtr->wlanMaxThroughput, valPtr->wlanCount, WlanMaxThroughputValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	//pei add 0302
	for(i=0;i<valPtr->wlanCount;i++)
	{
		valPtr->wlanMaxThroughput[i].wlanId = CWProtocolRetrieve8(msgPtr);
		valPtr->wlanMaxThroughput[i].maxThroughput = CWProtocolRetrieve8(msgPtr);
		CWWTPDebugLog("wlanId:%d maxThroughput:%d", valPtr->wlanMaxThroughput[i].wlanId, valPtr->wlanMaxThroughput[i].maxThroughput);
	}
	
	CWParseMessageElementEnd();
}
CWBool CWParseCmdFromAC (CWProtocolMessage *msgPtr, int len, CWProtocolConfigurationUpdateRequestValues *valPtr)        //pei add 0204
{
	unsigned char value;
	/*Begin: caidh add for root wlan wds 2013-03-21*/
	#ifdef MESH_SWITCH
	char tmp_str[100] = {0};
	char  str[20] = {0};
	char cmdbuf[100]={0};
	unsigned char wds=0,radio_id=0,wlan_id=0;
	char *wds_cmd="wds";
	CWWTPWlan *ptr;
    #endif
	/*End: caidh add for root wlan wds 2013-03-21*/
	CWParseMessageElementStart();
	value = CWProtocolRetrieve8(msgPtr);
	valPtr->cmdLength = CWProtocolRetrieve8(msgPtr);
	valPtr->cmd = (unsigned char *)CWProtocolRetrieveStr(msgPtr, valPtr->cmdLength);
	CWWTPDebugLog("***value:%d cmdLength:%d  cmd:%s", value, valPtr->cmdLength, valPtr->cmd);
	/*Begin: caidh add for root wlan wds 2013-03-21*/
	#ifdef MESH_SWITCH	
	if(strstr(valPtr->cmd,wds_cmd)!=NULL)
	{
		autelan_sprintf(tmp_str,"echo %s|awk -F \"wds\" '{print$2}'",valPtr->cmd);
		PopenFile(tmp_str,str,sizeof(str));
		wds=atoi(str);
		autelan_sprintf(tmp_str,"echo %s|awk -F \".\" '{print$2}'|awk -F \"-\" '{print$1}'",valPtr->cmd);
		PopenFile(tmp_str,str,sizeof(str));
		radio_id=atoi(str);
		autelan_sprintf(tmp_str,"echo %s|awk -F \"wds\" '{print$1}'|awk -F \"-\" '{print$2}'",valPtr->cmd);
		PopenFile(tmp_str,str,sizeof(str));
		wlan_id=atoi(str);
		autelan_printf("###wds is %d,radio id is %d,wlan id is %d###\n",wds,radio_id,wlan_id);
		if(wds==1){
			for(ptr=wtp_wlan_list;ptr!=NULL;ptr=ptr->next){
				if(radio_id==ptr->radio_id && wlan_id == ptr->wlan_id)
					ptr->wds=wds;
				if(ptr->vlan_id !=0){
					autelan_sprintf(cmdbuf,"ath.%d-%d.%d",radio_id,wlan_id,ptr->vlan_id);
					if(Check_Interface_State(cmdbuf)!=0)
					{
						autelan_printf("begin,add btr at %s,by caidh\n",__func__);
						autelan_sprintf(cmdbuf,"vconfig add ath.%d-%d %d",radio_id,wlan_id,ptr->vlan_id);
						_system(cmdbuf);
						autelan_sprintf(cmdbuf,"brctl addif br%d ath.%d-%d.%d",ptr->vlan_id,radio_id,wlan_id,ptr->vlan_id);
						_system(cmdbuf);
						autelan_sprintf(cmdbuf,"/sbin/ifconfig ath.%d-%d.%d up",radio_id,wlan_id,ptr->vlan_id);
						_system(cmdbuf);
						autelan_printf("end,add btr at %s,by caidh\n",__func__);
					}
				}
			}
		}
		if(wds==0){
		/*Begin:caidh add for root wlan wds 2013-03-21*/
				for(ptr=wtp_wlan_list;ptr!=NULL;ptr=ptr->next){
					if(radio_id==ptr->radio_id && wlan_id == ptr->wlan_id)
						ptr->wds=wds;
					if(ptr->vlan_id !=0){
						autelan_sprintf(cmdbuf,"ath.%d-%d.%d",radio_id,wlan_id,ptr->vlan_id);
						if(Check_Interface_State(cmdbuf) ==0)
						{
							autelan_printf("begin,delete btr at %s,by caidh\n",__func__);
							autelan_sprintf(cmdbuf,"brctl delif br%d ath.%d-%d.%d",ptr->vlan_id,radio_id,wlan_id,ptr->vlan_id);
							_system(cmdbuf);
							autelan_sprintf(cmdbuf,"/sbin/ifconfig ath.%d-%d.%d down",radio_id,wlan_id,ptr->vlan_id);
							_system(cmdbuf);
							autelan_sprintf(cmdbuf, "vconfig rem ath.%d-%d.%d", radio_id,wlan_id,ptr->vlan_id);
							_system(cmdbuf);
							autelan_printf("end,deletebtr at %s,by caidh\n",__func__);
						}
					}
				}
		/*End:caidh add for root wlan wds 2013-03-21*/
		}
	}
	#endif
	/*End: caidh add for root wlan wds 2013-03-21*/
	CWParseMessageElementEnd();
}

/* Begin: gengzj added for wifipos 2013-11-25 */
void CWReleaseWifiScanAndPosParam(WifiScanAndPosParam** valPtr)
{
    if ((WifiScanAndPosParam**)NULL == valPtr)
    {
        return;
    }
    if ((WifiScanAndPosParam*)NULL == *valPtr)
    {
        return;
    }
    
    CW_FREE_OBJECT((*valPtr)->val);
    CW_FREE_OBJECT(*valPtr);
    *valPtr = (WifiScanAndPosParam*)NULL;
    return;
}

CWBool CWParseWifiScanAndPosParam(
    CWProtocolMessage* msgPtr, 
    int len, 
    WifiScanAndPosParam* valPtr
)
{
    CWParseMessageElementStart();
    
    valPtr->len = len;
    valPtr->val = CWProtocolRetrieveRawBytes(msgPtr, len);
    CWWTPDebugLog("parse Wifi Scanning and Positioning message: len = %d", len);
    printmsg(valPtr->len, valPtr->val);
    
    CWParseMessageElementEnd();
}
/* End: gengzj added end */

CWBool CWParseTwoRadio5gPriorityAccess(CWProtocolMessage *msgPtr, int len, TwoRadio5GPriorityAccess*valPtr)
{
	CWParseMessageElementStart();
	valPtr->switch_ebable = CWProtocolRetrieve8(msgPtr);
	valPtr ->discard_count =  CWProtocolRetrieve8(msgPtr);
	valPtr->scantime_thr = CWProtocolRetrieve16(msgPtr);
	valPtr->agingtime_thr = CWProtocolRetrieve32(msgPtr);
	valPtr->stacount_thr= CWProtocolRetrieve8(msgPtr);
	valPtr->sta_signal_strength_thr=CWProtocolRetrieve8(msgPtr);
	CWWTPDebugLog("5G radio access  switch_ebable:%d,discard_count:%d,scantime_thr:%d,agingtime_thr:%d,stacount_thr:%d sta_signal_strength_thr:%d\n",
		valPtr->switch_ebable,valPtr ->discard_count,valPtr->scantime_thr,valPtr->agingtime_thr,valPtr->stacount_thr,valPtr->sta_signal_strength_thr);
	CWParseMessageElementEnd();
}
CWBool CWParseLongCmdForDHCPOption60FromAC (CWProtocolMessage *msgPtr, unsigned short len, CWProtocolConfigurationUpdateRequestValues *valPtr)       
{
	
	unsigned short element_id,cmdLength;
	unsigned char *cmd = NULL;
	char *str = NULL;
	/*zengmin add by Coverity 2013-06-13*/
	CWParseMessageElementStart();
	CW_CREATE_OBJECT_SIZE_ERR(cmd,(len + 30) , return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
    memset(cmd,0,(len + 30));	
	strcpy((char *)cmd,"/usr/sbin/set_vendorclassid ");
	(msgPtr->offset)-=4;
	element_id = CWProtocolRetrieve16(msgPtr);
    (msgPtr->offset)+=2;
	cmdLength = len ;
	valPtr->cmdLength = (cmdLength > 255)? 255 : ((unsigned char)cmdLength);
	//strcat((char *)cmd, CWProtocolRetrieveStr(msgPtr, cmdLength));
	str = CWProtocolRetrieveStr(msgPtr, cmdLength);
	strcat((char *)cmd, str);
	CW_FREE_OBJECT(str);
	
	valPtr->cmd = cmd;
	CWWTPDebugLog("***element_id:%d cmdLength:%d  cmd:%s", element_id, cmdLength, valPtr->cmd);
	
	CWParseMessageElementEnd();
}
CWBool CWParseInterfaceUpdownTime (CWProtocolMessage *msgPtr, int len, ExtraInfoValues *valPtr)        //pei add 0224
{
	CWParseMessageElementStart();
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->reportinterval = CWProtocolRetrieve16(msgPtr);
	CWWTPDebugLog("value:%d opstate:%d reportinterval:%d", valPtr->value, valPtr->opstate, valPtr->reportinterval);
	
	CWParseMessageElementEnd();
}
CWBool CWParseMonitorSet (CWProtocolMessage *msgPtr, int len, ApMonitorSetValues *valPtr)        //pei add 0226
{
	CWParseMessageElementStart();
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->reportinterval = CWProtocolRetrieve8(msgPtr);
	CWWTPDebugLog("value:%d opstate:%d reportinterval:%d", valPtr->value, valPtr->opstate, valPtr->reportinterval);
	
	CWParseMessageElementEnd();
}
CWBool CWParseGetStaInfo (CWProtocolMessage *msgPtr, int len, GetStaInfoValues *valPtr)        //pei add 0417
{
	CWParseMessageElementStart();
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->reportinterval = CWProtocolRetrieve16(msgPtr);
	CWWTPDebugLog("value:%d opstate:%d reportinterval:%d", valPtr->value, valPtr->opstate, valPtr->reportinterval);
	
	CWParseMessageElementEnd();
}
CWBool CWParseGetIfState (CWProtocolMessage *msgPtr, int len, GetIfStateValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->reportinterval = CWProtocolRetrieve16(msgPtr);
	CWWTPDebugLog("value:%d opstate:%d reportinterval:%d", valPtr->value, valPtr->opstate, valPtr->reportinterval);
	
	CWParseMessageElementEnd();
}
CWBool CWParseGetAttackDetectInfo (CWProtocolMessage *msgPtr, int len, GetAttackDetectInfoValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->floodingDetectEnable = CWProtocolRetrieve8(msgPtr);
	valPtr->spoofingDetectEnable = CWProtocolRetrieve8(msgPtr);
	valPtr->weakIVDetectEnable = CWProtocolRetrieve8(msgPtr);
	valPtr->flood_attack_cnt = CWProtocolRetrieve8(msgPtr);
	valPtr->flood_probe_cnt = CWProtocolRetrieve8(msgPtr);
	valPtr->wids_intval = CWProtocolRetrieve8(msgPtr);
	
	CWWTPDebugLog("value:%d floodingDetectEnable:%d spoofingDetectEnable:%d weakIVDetectEnable:%d flood_attack_cnt:%d flood_probe_cnt:%d wids_intval:%d", 
			valPtr->value, valPtr->floodingDetectEnable, valPtr->spoofingDetectEnable, valPtr->weakIVDetectEnable, valPtr->flood_attack_cnt, 
			valPtr->flood_probe_cnt, valPtr->wids_intval);
	
	CWParseMessageElementEnd();
}
CWBool CWParseGetStaWapiInfo (CWProtocolMessage *msgPtr, int len, GetStaWapiInfoValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->reportinterval = CWProtocolRetrieve16(msgPtr);
	
	dpf("value:%d opstate:%d reportinterval:%d\n", valPtr->value, valPtr->opstate, valPtr->reportinterval);
	
	CWParseMessageElementEnd();
}
CWBool CWParseGetNtpInfo (CWProtocolMessage *msgPtr, int len, GetNtpValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump reserve 
	valPtr->ipv4_v6= CWProtocolRetrieve8(msgPtr);
	valPtr->state= CWProtocolRetrieve8(msgPtr);
	valPtr->ntp_intval= CWProtocolRetrieve32(msgPtr);
	if(valPtr->ipv4_v6==0)
		valPtr->ipv4= CWProtocolRetrieve32(msgPtr);
	/*if(valPtr->ipv4_v6==1)
		valPtr->ipv6= CWProtocolRetrieveStr(msgPtr,16);*/
	(msgPtr->offset) += 4; //jump reserve 
	
	CWWTPDebugLog("value:%d ipv4_v6:%d state:%d ntp_intval:%u ipv4:%u ", 
			valPtr->value, valPtr->ipv4_v6, valPtr->state, valPtr->ntp_intval, valPtr->ipv4);
	CWParseMessageElementEnd();
}
CWBool CWParseGetTerminalDisturbInfo (CWProtocolMessage *msgPtr, int len, GetTerminalDisturbValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->terDisturbStaCnt = CWProtocolRetrieve16(msgPtr);
	valPtr->terDisturbPkt = CWProtocolRetrieve16(msgPtr);
	(msgPtr->offset) += 4; //jump reserve
	
	CWWTPDebugLog("value:%d opstate:%d terDisturbStaCnt:%d terDisturbPkt:%d", valPtr->value, valPtr->opstate, valPtr->terDisturbStaCnt, valPtr->terDisturbPkt);
	
	CWParseMessageElementEnd();
}

CWBool CWParseGetUserandPasswdinfo (CWProtocolMessage *msgPtr, int len, WtpUserPasswdValues *valPtr)
{
	int type=0,length = 0;
	int username_length = 0;
	int passwd_length     = 0;
	valPtr->username = NULL;
	valPtr->passwd = NULL;
	
	CWParseMessageElementStart();
	type = CWProtocolRetrieve8(msgPtr);
	length= CWProtocolRetrieve8(msgPtr);
	username_length= CWProtocolRetrieve8(msgPtr);
	valPtr->username = CWProtocolRetrieveStr(msgPtr,username_length);
	passwd_length = CWProtocolRetrieve8(msgPtr);
	valPtr->passwd= CWProtocolRetrieveStr(msgPtr,passwd_length);

	CWWTPDebugLog("username:%s passwd:%s", valPtr->username,valPtr->passwd);
	CWParseMessageElementEnd();
}

CWBool CWParseGetDisauthDisassocvalues(CWProtocolMessage *msgPtr, int len, DisassocDeauthValues *valPtr)
{
	int opstate = 0;
	CWParseMessageElementStart();
	opstate= CWProtocolRetrieve8(msgPtr);
	valPtr->value = 24;
	valPtr->length = len;
	valPtr->opstate= opstate;

	CWWTPDebugLog("parse disauth and disassoc switch  type:%d  length:%d opstate:%d", valPtr->value,len,opstate);
	CWParseMessageElementEnd();
}


CWBool CWParseGetAllstaLeavDetailIValues(CWProtocolMessage *msgPtr, int len,DetailAllStaInfoValues *valPtr)
{
	int opstate = 0;
	CWParseMessageElementStart();
	opstate= CWProtocolRetrieve8(msgPtr);

	valPtr->value = 25;
	valPtr->length = len;
	valPtr->opstate= opstate;

	CWWTPDebugLog("parse all sta leav detail info switch type:%d  length:%d opstate:%d", valPtr->value,len,opstate);
	CWParseMessageElementEnd();
}
CWBool CWParseGetMultiUserPerformance (CWProtocolMessage *msgPtr, int len, GetMultiUserPerformanceValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->op_type = CWProtocolRetrieve8(msgPtr);
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);
	valPtr->state = CWProtocolRetrieve8(msgPtr);
	valPtr->value = CWProtocolRetrieve8(msgPtr);
	CWWTPDebugLog("op_type:%d radio_id:%d wlan_id:%d state:%d value:%d", valPtr->op_type, valPtr->radio_id, valPtr->wlan_id, valPtr->state, valPtr->value);
	
	CWParseMessageElementEnd();
}

CWBool CWParseGetMcsMask (CWProtocolMessage *msgPtr, int len, GetMcsMaskValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->value = CWProtocolRetrieve32(msgPtr);
	(msgPtr->offset) += 12; //jump reserve
	CWWTPDebugLog("radio_id:%d value:0x%x", valPtr->radio_id, valPtr->value);
	
	CWParseMessageElementEnd();
}
CWBool CWParseGetStaPktsStatisticRpt(CWProtocolMessage *msgPtr, int len, GetStaPktsStatisticRptValues *valPtr,unsigned short elementid)
{
	CWParseMessageElementStart();
	valPtr->elem_type = elementid;
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->reportinterval = CWProtocolRetrieve16(msgPtr);
	CWWTPDebugLog("GetStaPktsStatisticRpt elem_type:%d opstate:%d reportinterval:%d", valPtr->elem_type, valPtr->opstate, valPtr->reportinterval);
	if(debug_print)
		autelan_printf("GetStaPktsStatisticRpt elem_type:%d opstate:%d reportinterval:%d\n", valPtr->elem_type, valPtr->opstate, valPtr->reportinterval);
	
	CWParseMessageElementEnd();
}

/*Begin:caidh add this function is to parse message 30 we receive from AC,like the report opstate ,report interval2013.02.16*/
CWBool CWParseGetApPktsStatisticRpt(CWProtocolMessage *msgPtr, int len, GetApPktsStatisticRptValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->elem_type = VSP_T_AP_PACKET_STATISTIC_REPORT;
	valPtr->length = len;
	valPtr->opstate = CWProtocolRetrieve8(msgPtr);
	valPtr->reportinterval = CWProtocolRetrieve16(msgPtr);
	CWWTPDebugLog("GetApPktsStatisticRpt elem_type:%d length:%d opstate:%d reportinterval:%d", valPtr->elem_type, valPtr->length,valPtr->opstate, valPtr->reportinterval);
	CWParseMessageElementEnd();
}
/*End:caidh add  2012.02.16*/
CWBool CWParseGetACLList(CWProtocolMessage *msgPtr, int len, GetACLList *valPtr)
{
	CWParseMessageElementStart();

	int mac_count;
	
	valPtr->list_class = CWProtocolRetrieve8(msgPtr);
	valPtr->level	= CWProtocolRetrieve8(msgPtr);
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);
	valPtr->operate= CWProtocolRetrieve8(msgPtr);
	valPtr->mac_count = CWProtocolRetrieve16(msgPtr);

	mac_count = valPtr->mac_count;
	CW_CREATE_ARRAY_ERR(valPtr->mac_list,mac_count*MAC_ADDR_LEN,unsigned char,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	void *ptr = NULL;
	ptr =  (unsigned char *)CWProtocolRetrieveRawBytes(msgPtr,mac_count*MAC_ADDR_LEN);
	CW_COPY_MEMORY(valPtr->mac_list, ptr, mac_count*MAC_ADDR_LEN);
	CW_FREE_OBJECT(ptr);

	CWWTPDebugLog("%s,class:%d,level:%d,radio id:%d,wlan id:%d,operate:%d,mac count:%d",
					__func__, valPtr->list_class,valPtr->level,valPtr->radio_id,valPtr->wlan_id,valPtr->operate,valPtr->mac_count);
	if(valPtr->operate == 3/*ACL_OP_CLEAR*/){
		valPtr->list_class = 0;
		CWWTPDebugLog("%s,class:%d,level:%d,radio id:%d,wlan id:%d,operate:%d,mac count:%d",
						__func__, valPtr->list_class,valPtr->level,valPtr->radio_id,valPtr->wlan_id,valPtr->operate,valPtr->mac_count);

	}
	/*for(i = 0; i < mac_count; i++){
		CWWTPDebugLog("idx:%d,"MAC_FMT,i+1,MAC2STR(valPtr->mac_list +  i*MAC_ADDR_LEN));
	}*/
	CWParseMessageElementEnd();
}

CWBool CWParseEMenuConf(CWProtocolMessage *msgPtr, int len, EMenuConf *valPtr)
{
	CWParseMessageElementStart();

	valPtr->e_swtich= CWProtocolRetrieve8(msgPtr);
	valPtr->level	= CWProtocolRetrieve8(msgPtr);
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);

	CWWTPDebugLog("%s, e_switch=%d, level=%d, radio id=%d, wlan id=%d",
					__func__, valPtr->e_swtich, valPtr->level, valPtr->radio_id, valPtr->wlan_id);

	CWParseMessageElementEnd();
}

void CWWTPResetRebootStatistics(WTPRebootStatisticsInfo *rebootStatistics)
{
	rebootStatistics->rebootCount=0;
	rebootStatistics->ACInitiatedCount=0;
	rebootStatistics->linkFailurerCount=0;
	rebootStatistics->SWFailureCount=0;
	rebootStatistics->HWFailuireCount=0;
	rebootStatistics->otherFailureCount=0;
	rebootStatistics->unknownFailureCount=0;
	rebootStatistics->lastFailureType=NOT_SUPPORTED;
}	
/***************************************************************************
 * 
 * Function:  CWSetRadioAdminState
 *
 * Purpose:  mainly used for ac set the radio admin state of wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWSetRadioAdminState(AdminStateValues *adminStateValues, unsigned char *operat_state_cause)
{
	unsigned char adminState;
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
#define ENABLE_RADIO     1
#define DISABLE_RADIO    2
	if(adminStateValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*operat_state_cause = 3;         //Administratively Set

	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == adminStateValues->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
			*operat_state_cause = 1;         //Radio Failure
			return CW_TRUE;
	}

	adminState = adminStateValues->admin_state;
	if (adminState == ENABLE_RADIO)//1&&gRadioInfoValue[adminStateValues->radio_id].gIsRadioEnable == CW_FALSE)
	{
		gRadioInfoValue[adminStateValues->radio_id].gIsRadioEnable = CW_TRUE;
		for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
			if(ptr->radio_id == adminStateValues->radio_id)
			{
				ptr->wlan_updown_time++;
				memset(ifname, 0, IFNAMSIZ);
				CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
				autelan_system(cmdbuf);
			}
		}
	}
	else if (adminState == DISABLE_RADIO)//2&&gRadioInfoValue[adminStateValues->radio_id].gIsRadioEnable == CW_TRUE)
	{
		gRadioInfoValue[adminStateValues->radio_id].gIsRadioEnable = CW_FALSE;
		for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
			if(ptr->radio_id == adminStateValues->radio_id)
			{
				ptr->wlan_updown_time++;
				memset(ifname, 0, IFNAMSIZ);
				CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
				autelan_system(cmdbuf);
			}
		}
		//disable wifi
		autelan_sprintf(cmdbuf,"/sbin/ifconfig wifi%d down > /dev/null", adminStateValues->radio_id);
		_system(cmdbuf);
		CWWTPDebugLog("wifi%d now is disable now", adminStateValues->radio_id);
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = adminStateValues->radio_id;
	strcpy(confArg.conf_arg,"gIsRadioEnable");
	strcpy(confArg.conf_value,gRadioInfoValue[adminStateValues->radio_id].gIsRadioEnable == 1?"1":"0");
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);

	return CW_TRUE;
}

CWBool CWWTPSetStaticIP(WTPStaticIPValues *wtpStaticIPValues)
{
	char cmdbuf[128];
	unsigned char ip_str[24];
	unsigned char netmask_str[24];
	unsigned char gateway_str[24];
	
	if(wtpStaticIPValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	if(!wtpStaticIPValues->static_IP_enable)
	{
		CWWTPDebugLog("WTP Static IP disable");
		dpf("WTP Static IP disable\n");
		return CW_TRUE;
	}
	CWWTPDebugLog("WTP Static IP enable");
	
	memset(ip_str, 0, 24);
	memset(netmask_str, 0, 24);
	memset(gateway_str, 0, 24);

	ip_int2str(wtpStaticIPValues->IP_addr, ip_str);
	ip_int2str(wtpStaticIPValues->netmask, netmask_str);
	ip_int2str(wtpStaticIPValues->gateway, gateway_str);

/*	if(wtpStaticIPValues->netmask == 8)
		strcpy(netmask_str, "255.0.0.0");
	else if(wtpStaticIPValues->netmask == 16)
		strcpy(netmask_str, "255.255.0.0");
	else if(wtpStaticIPValues->netmask == 24)
		strcpy(netmask_str, "255.255.255.0");
	if(0 == ip_int2str(wtpStaticIPValues->gateway, gateway_str))
		return CW_FALSE;
*/
	CWWTPDebugLog("ip:%s netmask:%s gateway:%s", ip_str, netmask_str, gateway_str);

	memset(cmdbuf, 0, 128);
	autelan_sprintf(cmdbuf, "/usr/sbin/changeip static %s %s %s", ip_str, netmask_str, gateway_str);
	_system(cmdbuf);
	CWLog("modify network config manually from AC,ip:%s,mask:%s,gw:%s.reboot to update config.", ip_str, netmask_str, gateway_str);
	kes_debug("WTP Static IP enable,reboot");
	_system("echo d > /proc/kes_debug_flag");
	_system("sysreboot");
	return CW_TRUE;
}

CWBool CWWTPSetStaticIPV2(WTPStaticIPValues *wtpStaticIPValues)
{
	char cmdbuf[128];
	unsigned char ip_str[24];
	unsigned char netmask_str[24];
	unsigned char gateway_str[24];
	unsigned char firstdns_str[24];
	unsigned char secdns_str[24];
	
	if(wtpStaticIPValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	
	memset(cmdbuf, 0, 128);
	memset(ip_str, 0, 24);
	memset(netmask_str, 0, 24);
	memset(gateway_str, 0, 24);
	memset(firstdns_str, 0, 24);
	memset(secdns_str, 0, 24);
	
	if(!wtpStaticIPValues->static_IP_enable){
		CWWTPDebugLog("WTP  use DHCP get IP");
		autelan_sprintf(cmdbuf, "/usr/sbin/changeip2 dhcp");
		_system(cmdbuf);
		CWLog("modify the method of getting ip to dhcp,reboot to update config.");
		kes_debug("WTP use DHCP get IP,reboot");
		_system("echo d > /proc/kes_debug_flag");
		_system("sysreboot");
		return CW_TRUE;
	}
	CWWTPDebugLog("WTP Static IP enable");
	ip_int2str(wtpStaticIPValues->IP_addr, ip_str);
	ip_int2str(wtpStaticIPValues->netmask, netmask_str);
	ip_int2str(wtpStaticIPValues->gateway, gateway_str);
	ip_int2str(wtpStaticIPValues->firstdns, firstdns_str);
	ip_int2str(wtpStaticIPValues->secdns, secdns_str);

	CWWTPDebugLog("ip:%s netmask:%s gateway:%s firstdns_str:%s secdns_str:%s", ip_str, netmask_str, gateway_str, firstdns_str, secdns_str);

	autelan_sprintf(cmdbuf, "/usr/sbin/changeip2 static %s %s %s %s %s", ip_str, netmask_str, gateway_str, firstdns_str, secdns_str);
	_system(cmdbuf);
	CWLog("modify network config manually from AC,ip:%s,mask:%s,gw:%s,dns1:%s,dns2:%s.reboot to update config.", ip_str, netmask_str, gateway_str,firstdns_str, secdns_str);
	kes_debug("WTP Static IP enable,reboot");
	_system("echo d > /proc/kes_debug_flag");
	_system("sysreboot");
	return CW_TRUE;
}
CWBool acw_set_eth_config_mtu(set_eth_mtu_vlaues *set_eth_values)
{
	char cmdbuf[128];
	memset(cmdbuf, 0, 128);	
	if(set_eth_values == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	CWWTPDebugLog("set eth%d mtu:%d", set_eth_values->eth_id, set_eth_values->mtu);

	autelan_sprintf(cmdbuf, "/sbin/ifconfig eth%d mtu %d",set_eth_values->eth_id,set_eth_values->mtu);
	_system(cmdbuf);
	return CW_TRUE;
}
CWBool acw_set_time_config(set_all_time_vlaues *set_time_values)
{
	dpf("set wtp_throughput_info_report interval\n");
	if(gAPThroughputInfoInterval!= set_time_values->timeinterval)
		autelan_printf("set wtp_throughput_info_report interval changed %d to %d\n", gAPThroughputInfoInterval, set_time_values->timeinterval);
	gAPThroughputInfoInterval = set_time_values->timeinterval;
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gAPThroughputInfoInterval");
	autelan_sprintf(confArg.conf_value,"%hd",gAPThroughputInfoInterval);
	CWSaveConfigInfo(CONF_WTPD,&confArg,CW_FALSE);
	return CW_TRUE;
}

CWBool CWWTPSetModeNPara(WTPModeNParaValues *wtpModeNParaValues)
{
	unsigned short GuardInterval;
	unsigned short Mcs;
	unsigned short CwmMode;
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
       char *mode = NULL;
	
	if(wtpModeNParaValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	for(ptr = wtp_wlan_list; ptr != NULL; ptr = ptr->next){
		if(ptr->radio_id == wtpModeNParaValues->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		return CW_TRUE;
	}

	/*down interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL; ptr = ptr->next){
		if(ptr->radio_id == wtpModeNParaValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}

	GuardInterval = wtpModeNParaValues->guard_interval;
	//Mcs = wtpModeNParaValues->mcs;
	CwmMode = wtpModeNParaValues->cwmmode;

	CWWTPDebugLog("setting 11n parameter: GuardInterval:%d CwmMode:%d", GuardInterval,CwmMode);

	memset(cmdbuf, 0, 128);
	autelan_sprintf(cmdbuf, "iwpriv %s shortgi %d", wlanname, GuardInterval);
	_system(cmdbuf);

	if(!get_radio_mode(gRadioInfoValue[wtpModeNParaValues->radio_id].radiotype,&mode))
	{
		CW_FREE_OBJECT(mode);
		return CW_FALSE;
	}

	memset(cmdbuf, 0, 128);
	for(ptr = wtp_wlan_list ; ptr != NULL; ptr = ptr->next){
		if(ptr->radio_id == wtpModeNParaValues->radio_id)
		{
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/usr/sbin/set_mode %s %s %d %d",ifname,mode,gModeNParaValue[wtpModeNParaValues->radio_id].extoffset,CwmMode);
			_system(cmdbuf);
			if( wtpModeNParaValues->cwmmode > 1) 
				autelan_sprintf(cmdbuf, "iwpriv %s disablecoext 1 > /dev/null", ifname);
			else
				autelan_sprintf(cmdbuf, "iwpriv %s disablecoext 0 > /dev/null", ifname);
			
				_system(cmdbuf); 
		}
	}
	if(gApScanningEnable == 1)
	{
		autelan_sprintf(ifname,"athscanwifi0");
		autelan_sprintf(cmdbuf,"/usr/sbin/set_mode %s %s %d %d",ifname,mode,gModeNParaValue[wtpModeNParaValues->radio_id].extoffset,CwmMode);
		_system(cmdbuf);
	}
	if(debug_print)
		autelan_printf("set cwmmode cmd :%s\n",cmdbuf);
	/*set channel back after setting channel 0 in set_mode*/
	autelan_sprintf(cmdbuf,"iwconfig %s channel %d",wlanname,gRadioInfoValue[wtpModeNParaValues->radio_id].channel);
	_system(cmdbuf);
	CW_FREE_OBJECT(mode);
#if 0
	Mcs = wtpModeNParaValues->mcs;
	memset(cmdbuf, 0, 128);
	memset(wlanname, 0, IFNAMSIZ);
	sprintf(wlanname, "wifi%d", wtpModeNParaValues->radio_id);
	if(Mcs<8)
		sprintf(cmdbuf, "iwpriv %s txchainmask 1", wlanname);
	else if((Mcs>7)&&(Mcs<16))
		sprintf(cmdbuf, "iwpriv %s txchainmask 3", wlanname);
	_system(cmdbuf);

#endif
	Mcs = wtpModeNParaValues->mcs;
	CWWTPDebugLog("setting 11n parameter: Mcs:%d\n",Mcs);
	gModeNParaValue[wtpModeNParaValues->radio_id].mcs = wtpModeNParaValues->mcs;


	/* up interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == wtpModeNParaValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}
	usleep(180000);
	for(ptr = wtp_wlan_list ; ptr != NULL; ptr = ptr->next){
		if(ptr->radio_id == wtpModeNParaValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == wtpModeNParaValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}

	gModeNParaValue[wtpModeNParaValues->radio_id].guardInterval = wtpModeNParaValues->guard_interval;
	gModeNParaValue[wtpModeNParaValues->radio_id].mcs = wtpModeNParaValues->mcs;
	gModeNParaValue[wtpModeNParaValues->radio_id].cwmMode = wtpModeNParaValues->cwmmode;

	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = wtpModeNParaValues->radio_id;
	strcpy(confArg.conf_arg,"cwmmode");
	autelan_sprintf(confArg.conf_value,"%hd",gModeNParaValue[wtpModeNParaValues->radio_id].cwmMode);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);	
	
	return CW_TRUE;
}

CWBool CWParseProbeRequestInhibition(CWProtocolMessage *msgPtr,int len, ProbeRequestInhibition *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr); 
	valPtr->pr_switch = CWProtocolRetrieve16(msgPtr);
	CWParseMessageElementEnd();	

}
CWBool CWSaveProbeRequestInhibition(ProbeRequestInhibition *valPtr)
{
	if(valPtr == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	char cmdbuf[128];
	memset(cmdbuf ,0, 128);
	autelan_sprintf(cmdbuf,"iwpriv ath.%d-%d probe_req %u",(int)(valPtr->radio_id), (int)(valPtr->wlan_id), (unsigned int)(valPtr->pr_switch));
	_system(cmdbuf);
	return CW_TRUE;
}


CWBool CWParseCastIsolation(CWProtocolMessage *msgPtr, int len, CastIsolation_CastRate *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr); 
	valPtr->uni_muti_cast = CWProtocolRetrieve16(msgPtr);
	valPtr->rate_value = 0;
	CWParseMessageElementEnd();
}

CWBool CWParseCastRate(CWProtocolMessage *msgPtr, int len, CastIsolation_CastRate *valPtr)
{
	CWParseMessageElementStart();
	unsigned char tmp_rate;
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr); 
	CWProtocolRetrieve16(msgPtr); 
	tmp_rate = CWProtocolRetrieve8(msgPtr);
	valPtr->rate_value = ((int)tmp_rate)*1000/2;
	valPtr->uni_muti_cast = 0;
	msgPtr->offset += 3;
	CWParseMessageElementEnd();
}


CWBool CWSaveCastIsolation_CastRate(CastIsolation_CastRate *valPtr){
	if(valPtr == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	char cmdbuf[128];
	if(valPtr->rate_value == 0){
		memset(cmdbuf ,0, 128);
		autelan_sprintf(cmdbuf, "iwpriv ath.%d-%d switch %d",(int)(valPtr->radio_id), (int)(valPtr->wlan_id), valPtr->uni_muti_cast);
		_system(cmdbuf);
	}

	if(valPtr->rate_value != 0){
		memset(cmdbuf ,0, 128);
		autelan_sprintf(cmdbuf, "iwpriv ath.%d-%d mcast_rate %d",(int)(valPtr->radio_id), (int)(valPtr->wlan_id), (int)(valPtr->rate_value));
		_system(cmdbuf);
	}
	
	return CW_TRUE;
}







