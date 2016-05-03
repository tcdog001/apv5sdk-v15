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



#include "wireless_copy.h"
#include "CWWTP.h"
#include "WTPFrameReceive.h"
#include "WTPDriverInteraction.h"//yuan add for report sta rssi

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
CWBool isFirstWlan = CW_TRUE;
CWBool isWPAfirstKey = CW_TRUE;
char * trim(char *str_org);
CW_THREAD_RETURN_TYPE CWWTPReceive802_11Frame(void *arg);
CW_THREAD_RETURN_TYPE CWWTPReceive802_3Frame(void *arg);
CWBool CWSaveAddwlanConfigInfo(CWWTPWlan *wlancreating);
extern unsigned char gStaWapiInfoEnable;
extern CW_THREAD_RETURN_TYPE CWWTPGetStaWapiInfo(void *arg);
extern EMenuConf gEMenuConf;


//************************************************
int CWTranslateQueueIndex(int j)
{
	if (j==VOICE_QUEUE_INDEX) return 3;
	if (j==VIDEO_QUEUE_INDEX) return 2;
	if (j==BACKGROUND_QUEUE_INDEX) return 1;

	return 0;
}

CWBool CWWTPInitBinding(int radioIndex)
{
	bindingValues* aux;
	int i,sock;
	struct iwreq wrq;

	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) 
	{
		return CW_FALSE;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, gInterfaceName, IFNAMSIZ);

	CW_CREATE_OBJECT_ERR(aux, bindingValues, {autelan_close(sock);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});

	gRadiosInfo.radiosInfo[radioIndex].bindingValuesPtr=(void*) aux;

	CW_CREATE_ARRAY_ERR(aux->qosValues, NUM_QOS_PROFILES, WTPQosValues, {autelan_close(sock);CW_FREE_OBJECT(aux);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});

	for(i=0; i<NUM_QOS_PROFILES; i++){
		if(!get_cwmin(sock, &wrq, CWTranslateQueueIndex(i), 0)){autelan_close(sock);CW_FREE_OBJECT(aux->qosValues);CW_FREE_OBJECT(aux);return CW_FALSE;}
		aux->qosValues[i].cwMin = wrq.u.param.value;

		if(!get_cwmax(sock, &wrq, CWTranslateQueueIndex(i), 0)){autelan_close(sock);CW_FREE_OBJECT(aux->qosValues);CW_FREE_OBJECT(aux);return CW_FALSE;}
		aux->qosValues[i].cwMax = wrq.u.param.value;

		if(!get_aifs(sock, &wrq, CWTranslateQueueIndex(i), 0)){autelan_close(sock);CW_FREE_OBJECT(aux->qosValues);CW_FREE_OBJECT(aux);return CW_FALSE;}
		aux->qosValues[i].AIFS = wrq.u.param.value;

/*		aux->qosValues[i].cwMin = 2;
		aux->qosValues[i].cwMax = 4;
		aux->qosValues[i].AIFS = 3;
*/
	}
	autelan_close(sock);
	return CW_TRUE;
}

CWBool CWBindingSetQosValues(int qosCount, RadioQosValues *radioQosValues, CWProtocolResultCode *resultCode)
{
	struct iwreq wrq;
	int sock;

	if (qosCount<=0) {return CW_TRUE;}
	if (radioQosValues==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	*resultCode = CW_PROTOCOL_SUCCESS;

	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) 
	{
		CWWTPDebugLog("%s,Error Creating Socket for ioctl",__func__); 
		return CWErrorRaise(CW_ERROR_GENERAL, NULL);;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, gInterfaceName, IFNAMSIZ);
		
	int i,k,j;
	
	for(i=0;i<qosCount;i++)
	{
		for(k=0;k<gRadiosInfo.radioCount;k++)
		{
			if(radioQosValues[i].radioID==gRadiosInfo.radiosInfo[k].radioID)
			{
				bindingValues* auxPtr=(bindingValues*) gRadiosInfo.radiosInfo[k].bindingValuesPtr;	
				
				for(j=0; j<NUM_QOS_PROFILES; j++)
				{
					if(auxPtr->qosValues[j].cwMin!=radioQosValues[i].qosValues[j].cwMin)
					{
						if (set_cwmin(sock, wrq, CWTranslateQueueIndex(j), 0, radioQosValues[i].qosValues[j].cwMin))
							{auxPtr->qosValues[j].cwMin=radioQosValues[i].qosValues[j].cwMin;}
						else {*resultCode=CW_PROTOCOL_FAILURE;}
					}

					if(auxPtr->qosValues[j].cwMax!=radioQosValues[i].qosValues[j].cwMax)
					{
						if (set_cwmax(sock, wrq, CWTranslateQueueIndex(j), 0, radioQosValues[i].qosValues[j].cwMax))
							{auxPtr->qosValues[j].cwMax=radioQosValues[i].qosValues[j].cwMax;}
						else {*resultCode=CW_PROTOCOL_FAILURE;}
					}

					if(auxPtr->qosValues[j].AIFS!=radioQosValues[i].qosValues[j].AIFS)
					{
						if (set_aifs(sock, wrq, CWTranslateQueueIndex(j), 0, radioQosValues[i].qosValues[j].AIFS))
							{auxPtr->qosValues[j].AIFS=radioQosValues[i].qosValues[j].AIFS;}
						else {*resultCode=CW_PROTOCOL_FAILURE;}
					}
				}
				break;
			}
		}
	}
	//WTPQosValues* aux=radioQosValues;
	
	autelan_close(sock);
	return CW_TRUE;
}
CWBool CWBindingSetQosValues1(int qosCount, Radio_QosValues *radioQosValues, CWProtocolResultCode *resultCode)
{
	if(gRunRetry != 0){	
		CWWTPDebugLog("Get Retransmission msg of QoS! Direct Return True.");
		return CW_TRUE;
	}
	int i;
	
	if (qosCount<=0) {return CW_TRUE;}
	if (radioQosValues==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	*resultCode = CW_PROTOCOL_SUCCESS;
	
	for(i=0;i<qosCount;i++)
	{
		char cmdbuf[100] = {0};
		CWWTPWlan *ptr = NULL;
		if(wtp_wlan_count == 0)
		{
			CWWTPDebugLog("set qos to wlan,but there is no wlan left to be setted.");
			*resultCode=CW_PROTOCOL_FAILURE;
		}
		for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next)
		{
			if(radioQosValues[i].radioID == ptr->radio_id)
			{
				unsigned char wlan_id;
				char wlanname[IFNAMSIZ];
				wlan_id = ptr->wlan_id;
				CWWTPGetWlanName(wlanname, ptr->radio_id,ptr->wlan_id);
				CWWTPDebugLog("configure update msg QosEnable:%d wlan:%s", gQosEnable,wlanname);
				CWWTPWmmSetList *wmm_set_list_temp;
				CWThreadMutexLock(&wmm_set_mutex);
				for(wmm_set_list_temp = g_wmm_list;NULL != wmm_set_list_temp;wmm_set_list_temp = wmm_set_list_temp->next)
				{
					CWWTPDebugLog("==update====== list = %d, add = %d enable= %d\n",wmm_set_list_temp->radio_id,ptr->radio_id,wmm_set_list_temp->enable);
					if((wmm_set_list_temp->radio_id == ptr->radio_id)&&(1== wmm_set_list_temp->enable))
					{
						autelan_sprintf(cmdbuf, "iwpriv %s wmm 1", wlanname);
						_system(cmdbuf);
					}else if((wmm_set_list_temp->radio_id == ptr->radio_id)&&(0== wmm_set_list_temp->enable)){
						autelan_sprintf(cmdbuf, "iwpriv %s wmm 0", wlanname);
						_system(cmdbuf);
					}
				}
				CWThreadMutexUnlock(&wmm_set_mutex);
			}
		}
	}
	
	return CW_TRUE;
}
/***************************************************************************
 * 
 * Function:  CWBindingSetDirectSequenceControl
 *
 * Purpose:  mainly used forac set the channel of wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWBindingSetDirectSequenceControlValues(DirectSequeueControlValues *directSequeueControlValues, CWProtocolResultCode *resultCode)
{
	int channel;
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	int type = 0;
	//int rid = directSequeueControlValues->radio_id;
	unsigned char wlanId = 0;
	int rechannel;

	if(directSequeueControlValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	int rid = directSequeueControlValues->radio_id;
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == directSequeueControlValues->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);/*get one is OK*/
			wlanId = ptr->wlan_id;
			radio_found = CW_TRUE;
			break;
		}

	}
	if(radio_found == CW_FALSE){
		CWWTPDebugLog("set radio %d channel %d,but cannot find wlan in radio %d to be setted.",directSequeueControlValues->radio_id,directSequeueControlValues->current_channel,directSequeueControlValues->radio_id);
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	channel = (int)directSequeueControlValues->current_channel;

	if((gRunRetry == 0)||(channel !=0))
	{
		autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname);
		autelan_system(cmdbuf);
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == directSequeueControlValues->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				if(wlanId != ptr->wlan_id)
				{
					memset(ifname, 0, IFNAMSIZ);
					CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
					autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
					autelan_system(cmdbuf);
				}
			}
		}
		/*get channel and report ac when set channel 0 */
		/*if((gRadioInfoValue[directSequeueControlValues->radio_id].radiotype&IEEE80211_PARAM_MODE_11a)&&(0==channel)){
			channel = 149+4*CWRandomIntInRange(0,4);
			if(-1==gModeNParaValue[directSequeueControlValues->radio_id].extoffset)
				channel=153;
			if(-1!=gModeNParaValue[directSequeueControlValues->radio_id].extoffset)
				channel=149;
			directSequeueControlValues->current_channel = channel;
			type = 1;
			type = type<<4;
			*resultCode=directSequeueControlValues->radio_id|type;
			*resultCode=*resultCode<<16;
			*resultCode=*resultCode|channel;
		}*/
		dpf("type:%d radio_id:%d channel:%d  resultCode:%d\n", type, directSequeueControlValues->radio_id, channel, *resultCode);
		if((gRadioInfoValue[rid].radiotype&IEEE80211_PARAM_MODE_11a)!=0){
			for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
				if(ptr->radio_id == rid)
				{
						memset(ifname, 0, IFNAMSIZ);
						CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
						set_channel_cmd(ifname,channel);
				}
			}
		}
		else
			set_channel_cmd(wlanname,channel);

		/* up interface*/
		autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
		autelan_system(cmdbuf);
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == directSequeueControlValues->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				if(wlanId != ptr->wlan_id)
				{
					memset(ifname, 0, IFNAMSIZ);
					CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
					autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
					autelan_system(cmdbuf);
				}
			}
		}
	}

	if(channel == 0)
	{
		if (gRunRetry == 0)
		{
			Channelget(resultCode, directSequeueControlValues->radio_id, wlanname);
			rechannel = 0x00ff & (*resultCode);
			/*for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
				if(ptr->radio_id == directSequeueControlValues->radio_id)
				{
					ptr->wlan_updown_time++;  //pei add 0225
					memset(ifname, 0, IFNAMSIZ);
					CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
					sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
					_system(cmdbuf);
				}
			}
			set_channel_cmd(wlanname,rechannel);
			for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
				if(ptr->radio_id == directSequeueControlValues->radio_id)
				{
					ptr->wlan_updown_time++;  //pei add 0225
					memset(ifname, 0, IFNAMSIZ);
					CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
					sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
					_system(cmdbuf);
				}
			}*/
		}
		else
		{
			type = 1;
			type = type<<4;
			*resultCode=directSequeueControlValues->radio_id|type;
			*resultCode=*resultCode<<16;
			*resultCode=*resultCode|gRadioInfoValue[directSequeueControlValues->radio_id].channel;
			CWWTPDebugLog("set channel auto, but retransmit, using current channel:%d", gRadioInfoValue[directSequeueControlValues->radio_id].channel);
		}
	}
	else
	{
		gRadioInfoValue[directSequeueControlValues->radio_id].channel = directSequeueControlValues->current_channel;
	}
	
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = directSequeueControlValues->radio_id;
	strcpy(confArg.conf_arg,"channel");
	autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[directSequeueControlValues->radio_id].channel);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);

	return CW_TRUE;
}
/***************************************************************************
 * 
 * Function:  CWBindingSetTxPower
 *
 * Purpose:  mainly used for ac set the tx power of wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWBindingSetTxPower(TxPowerValues *txPowerValues, CWProtocolResultCode *resultCode)
{
	int txpower;
	unsigned char txtype;
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	int return_type;
	
	if(txPowerValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == txPowerValues->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		CWWTPDebugLog("set radio %d txpower %d,but cannot find wlan in radio %d to be setted..",txPowerValues->radio_id,txPowerValues->current_power,txPowerValues->radio_id);
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	/*down interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == txPowerValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}

	
	txpower = (int)txPowerValues->current_power;
#if 0
	if(0 == txPowerValues->radio_id)
	{
		if(access("/jffs/tx-offset", R_OK)==-1)
		{
			dpf("/jffs/tx-offset not exist!!\n");
		}
		else
		{
	/*		if(txpower>11)
				txpower = 11;
			else if(txpower<1)
				txpower = 1;
	*/
			int fd,size,txoffset;
			char txbuf[10];
			memset(txbuf, 0, 10);
			fd = open ("/jffs/tx-offset", O_RDONLY);	
			size = read(fd, txbuf, sizeof(txbuf));
			close(fd);
			txbuf[size-1] = '\0';
			txoffset = atoi(txbuf);

			dpf("/jffs/tx-offset exists, tx-power(%d) is limit to less than (27-%d=%d)!\n\n", txpower, txoffset, (27-txoffset));
			
			if(txpower>txoffset)
				txpower -= txoffset;
			else
				txpower = 1;
			
			dpf("gtxpower after tx-offset:%d\n", txpower);
		}
	}
#endif
	txtype = txPowerValues->reserved;
//	txtype = 1;//gzm test 0929
	txpower = txPowerValues->current_power;
//	printf("-------current_power:%d\n", txPowerValues->current_power);
	gRadioInfoValue[txPowerValues->radio_id].txtype = txtype;
	gRadioInfoValue[txPowerValues->radio_id].txpower = txpower;
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = txPowerValues->radio_id;
	strcpy(confArg.conf_arg,"txtype");
	autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[txPowerValues->radio_id].txtype);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);
	
	strcpy(confArg.conf_arg,"txpower");
	autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[txPowerValues->radio_id].txpower);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);

	set_txpower_cmd(wlanname, txtype, txpower);
	if(txtype==1)
		return_type=3;
	else
		return_type=2;

	/* up interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == txPowerValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}
	Txpowerget(resultCode,txPowerValues->radio_id,wlanname,return_type);
	CWWTPDebugLog("wlanname: %s ,txtype: %d ,txpower: %d ", wlanname, txtype, txpower);
	

	return CW_TRUE;
}

/***************************************************************************
 * 
 * Function:  CWBindingSetRate
 *
 * Purpose:  mainly used for ac set the rate of wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWBindingSetRate(BindingRate *rateValues, CWProtocolResultCode *resultCode)
{
	int rate;
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	
	if(rateValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == rateValues->radioID){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		CWWTPDebugLog("set radio %d rate %d,but cannot find wlan in radio %d to be setted.",rateValues->radioID,rateValues->rate,rateValues->radioID);
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	/*down interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == rateValues->radioID)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}
	
	rate = (int)rateValues->rate;
	//grate = rateValues->rate;
	gRadioInfoValue[rateValues->radioID].rate = rateValues->rate;
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == rateValues->radioID)
		{
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			set_rate_cmd(wlanname, rateValues->radioID,gRadioInfoValue[rateValues->radioID].ratemask);
		}
	}
	CWWTPDebugLog("set radio %d rate %d", rateValues->radioID,rate);
	/* up interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == rateValues->radioID)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}


	return CW_TRUE;
}

/***************************************************************************
 * 
 * Function:  CWBindingSetRadioType
 *
 * Purpose:  mainly used for ac set the radio type of wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWBindingSetRadioType(BindingRadioType *radioTypeValues, CWProtocolResultCode *resultCode)
{
	unsigned int radiotype;
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	unsigned char radio_id,wlan_id;
	unsigned int channel;
	int type = 0;

	if(radioTypeValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;

	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == radioTypeValues->radioID){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_id = ptr->radio_id;
			wlan_id = ptr->wlan_id;
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	radiotype = radioTypeValues->radiotype;
	/*down interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == radioTypeValues->radioID)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
			autelan_system(cmdbuf);
			set_radiotype_cmd(ptr->radio_id, ptr->wlan_id, radiotype);
		}
	}

	if(gApScanningEnable == 1)
	{
		set_radiotype_cmd(radioTypeValues->radioID, 0, radiotype);
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = radioTypeValues->radioID;
	strcpy(confArg.conf_arg,"radiotype");
	autelan_sprintf(confArg.conf_value,"%d",radioTypeValues->radiotype);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);
	channel = gRadioInfoValue[radioTypeValues->radioID].channel;
#if 0
	if((gRadioInfoValue[radioTypeValues->radioID].radiotype&IEEE80211_PARAM_MODE_11a)&&((radiotype&IEEE80211_PARAM_MODE_11b)||(radiotype&IEEE80211_PARAM_MODE_11g)))
	{
		channel = 1+CWRandomIntInRange(0,13);
		if(-1==gModeNParaValue[radioTypeValues->radioID].extoffset)
			channel = 5+CWRandomIntInRange(0,7);
		if(-1!=gModeNParaValue[radioTypeValues->radioID].extoffset)
			channel = 1+CWRandomIntInRange(0,7);

	}
#endif
	if((radiotype&IEEE80211_PARAM_MODE_11a)&&((gRadioInfoValue[radioTypeValues->radioID].radiotype&IEEE80211_PARAM_MODE_11b)||(gRadioInfoValue[radioTypeValues->radioID].radiotype&IEEE80211_PARAM_MODE_11g)))
	{
		channel = 149+4*CWRandomIntInRange(0,5);
		if(-1==gModeNParaValue[radioTypeValues->radioID].extoffset)
			channel=153;
		if(-1!=gModeNParaValue[radioTypeValues->radioID].extoffset)
			channel=149;
	}
	type = 1;
	type = type<<4;
	*resultCode=radioTypeValues->radioID|type;
	*resultCode=*resultCode<<16;
	*resultCode=*resultCode|channel;
	//gradiotype = (int)radioTypeValues->radiotype;
	gRadioInfoValue[radioTypeValues->radioID].radiotype = radioTypeValues->radiotype;

	/*unsigned char mode;
	if(gRadioInfoValue[radioTypeValues->radioID].radiotype&IEEE80211_PARAM_MODE_11n)
			mode = 'n';
		else
			mode = 'g';
	sprintf(cmdbuf, "/usr/sbin/ar11n_wmm %s %c",wlanname,mode);
	_system(cmdbuf);*/

	//set_radiotype_cmd(radio_id, wlan_id, radiotype);
	/*set channel back after seting channel 0 in ar11nmode*/
	autelan_sprintf(cmdbuf,"iwconfig %s channel %d",wlanname,channel);
	_system(cmdbuf);
	gRadioInfoValue[radioTypeValues->radioID].channel = channel;
	/* up interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(radio_id == ptr->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}


	return CW_TRUE;
}

/***************************************************************************
 * 
 * Function:  CWBindingSetMacOperation
 *
 * Purpose:  mainly used for ac set the fragment-threshold of wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWBindingSetMacOperation(MacOperationValues *macOperationValues, CWProtocolResultCode *resultCode)
{
	unsigned short fragthreshold;
	unsigned short rtsthreshold;
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	
	if(macOperationValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;

	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == macOperationValues->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	/*down interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == macOperationValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}

	fragthreshold = macOperationValues->fragmentation_threshold;
	rtsthreshold = macOperationValues->rts_threshold;
	gRadioInfoValue[macOperationValues->radio_id].fragThreshold = macOperationValues->fragmentation_threshold;
	gRadioInfoValue[macOperationValues->radio_id].rtsThreshold = macOperationValues->rts_threshold;
	gRadioInfoValue[macOperationValues->radio_id].shortRetry = macOperationValues->short_retry;
	gRadioInfoValue[macOperationValues->radio_id].longRetry = macOperationValues->long_retry;
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = macOperationValues->radio_id;
	strcpy(confArg.conf_arg,"fragThreshold");
	autelan_sprintf(confArg.conf_value,"%hd",gRadioInfoValue[macOperationValues->radio_id].fragThreshold);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = macOperationValues->radio_id;
	strcpy(confArg.conf_arg,"rtsThreshold");
	autelan_sprintf(confArg.conf_value,"%hd",gRadioInfoValue[macOperationValues->radio_id].rtsThreshold);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	
	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = macOperationValues->radio_id;
	strcpy(confArg.conf_arg,"shortRetry");
	autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[macOperationValues->radio_id].shortRetry);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	
	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = macOperationValues->radio_id;
	strcpy(confArg.conf_arg,"longRetry");
	autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[macOperationValues->radio_id].longRetry);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == macOperationValues->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			if(gRadioInfoValue[macOperationValues->radio_id].radiotype&IEEE80211_PARAM_MODE_11n)      //11n
			{
				if(debug_print)
					autelan_printf("Current mode fragmention must be off, value: 2346.\n");
			    set_fragthreshold_cmd(wlanname,2346); 
			  }
			else
			 set_fragthreshold_cmd(wlanname, gRadioInfoValue[macOperationValues->radio_id].fragThreshold);
			
			set_rtsthreshold_cmd(wlanname, gRadioInfoValue[macOperationValues->radio_id].rtsThreshold);
		}
	}
	CWWTPDebugLog("set radio %d fragThreshold %d rtsThreshold %d", macOperationValues->radio_id, gRadioInfoValue[macOperationValues->radio_id].fragThreshold,gRadioInfoValue[macOperationValues->radio_id].rtsThreshold);
	/* up interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == macOperationValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}

	return CW_TRUE;
}

/***************************************************************************
 * 
 * Function:  CWBindingSetRadioConfig
 *
 * Purpose:  mainly used for ac set the preamble and beacon-interval of wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWBindingSetRadioConfig(WTPRadioConfigurationValues *radioConfigurationValues, CWProtocolResultCode *resultCode)
{
	unsigned char preamble;
	unsigned char dtim_period;
	unsigned short beacon_period;
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	char CountryCode[4];
	int i=0;
	
	if(radioConfigurationValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;

	for(ptr = wtp_wlan_list; ptr != NULL; ptr = ptr->next){
		if(ptr->radio_id == radioConfigurationValues->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_found = CW_TRUE;
			break;
		}
	}
	
	memset(CountryCode, 0, 4);
	memcpy(CountryCode, radioConfigurationValues->country_code, 4);
	for(i=0;i<4;i++){
		if(CountryCode[i] == ' ')
			CountryCode[i] = '\0';
	}
	CWWTPDebugLog("wifi%d CountryCode:[%s]",radioConfigurationValues->radio_id,CountryCode);
	
	autelan_sprintf(cmdbuf, "/usr/sbin/set_countrycode %d %s >/dev/null 2>&1",radioConfigurationValues->radio_id ,CountryCode);
	_system(cmdbuf);
	
	memset(cmdbuf,0,128);
	autelan_sprintf(cmdbuf, "iwpriv wifi%d setCountry %s >/dev/null",radioConfigurationValues->radio_id,CountryCode);
	_system(cmdbuf);
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	/*down interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL; ptr = ptr->next){
		if(ptr->radio_id == radioConfigurationValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}

	preamble = radioConfigurationValues->is_short_preamble;
	dtim_period = radioConfigurationValues->dtim_period;
	beacon_period = radioConfigurationValues->beacon_period;

	gRadioInfoValue[radioConfigurationValues->radio_id].preamble = radioConfigurationValues->is_short_preamble;
	gRadioInfoValue[radioConfigurationValues->radio_id].dtim = radioConfigurationValues->dtim_period;
	gRadioInfoValue[radioConfigurationValues->radio_id].beaconInterval = radioConfigurationValues->beacon_period;
	//add by sdl@20111010:ditm_period for each ath  
	for(ptr = wtp_wlan_list; ptr != NULL; ptr = ptr->next){
	    if(ptr->radio_id == radioConfigurationValues->radio_id){
		CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);	
		set_radioconfig_cmd(wlanname, gRadioInfoValue[radioConfigurationValues->radio_id].preamble, 
			gRadioInfoValue[radioConfigurationValues->radio_id].dtim, gRadioInfoValue[radioConfigurationValues->radio_id].beaconInterval);
		}
	}
	CWWTPDebugLog("set radio %d preamble %d dtim_period %d beaconInterval %d ",radioConfigurationValues->radio_id,gRadioInfoValue[radioConfigurationValues->radio_id].preamble, 
		gRadioInfoValue[radioConfigurationValues->radio_id].dtim, gRadioInfoValue[radioConfigurationValues->radio_id].beaconInterval);
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = radioConfigurationValues->radio_id;
	strcpy(confArg.conf_arg,"preamble");
	autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioConfigurationValues->radio_id].preamble);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = radioConfigurationValues->radio_id;
	strcpy(confArg.conf_arg,"dtim");
	autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioConfigurationValues->radio_id].dtim);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	
	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = radioConfigurationValues->radio_id;
	strcpy(confArg.conf_arg,"beaconInterval");
	autelan_sprintf(confArg.conf_value,"%hd",gRadioInfoValue[radioConfigurationValues->radio_id].beaconInterval);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	
	/* up interface*/
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		if(ptr->radio_id == radioConfigurationValues->radio_id)
		{
			ptr->wlan_updown_time++;  //pei add 0225
			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
			autelan_system(cmdbuf);
		}
	}

	return CW_TRUE;
}
/***************************************************************************
 * 
 * Function:  CWBindingAddWlan
 *
 * Purpose:  add a wlan in the WTP 
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/
CWBool CWBindingAddWlan(AddWlanValues *addWlanValues, WPA_IE *wpa_ie, WlanVlanValues *wlanVlanValues, WapiValues *wapiValues, CWProtocolResultCode *resultCode)
{
	CWBool radio_found = CW_FALSE;
	int sock;
//	int i;
	int wlancreateOK;
	/*static int receiveframethread = 0;*//*if 1 the thread has created*/
	CWWTPWlan *wlancreating;
	char wlanname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	int wlan_in_same_radio = 0;
	

	if (addWlanValues==NULL||wpa_ie == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	CW_CREATE_OBJECT_ERR(wlancreating,CWWTPWlan,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););

	if(addWlanValues->radio_id<MAX_RADIO_NUM)
	{
		radio_found = CW_TRUE;
	}
	if(radio_found == CW_FALSE){
		CW_FREE_OBJECT(wlancreating);
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}
	memset(wlancreating, 0, sizeof(CWWTPWlan));
	wlancreating->radio_id = addWlanValues->radio_id;
	wlancreating->wlan_id = addWlanValues->wlan_id;
	wlancreating->capabilities = addWlanValues->capabilities;
	wlancreating->key_length = addWlanValues->key_length;
	wlancreating->wlan_key_mgmt = wpa_ie->key_mgmt;
	wlancreating->wlan_mac_mode = addWlanValues->mac_mode;
	wlancreating->wlan_tunnel_mode = addWlanValues->tunnel_mode;
	strncpy(wlancreating->key,addWlanValues->key,addWlanValues->key_length);/* caosicong 080702*/
	strncpy(wlancreating->essid ,addWlanValues->ssid,32);

	*resultCode = CW_PROTOCOL_SUCCESS;

	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) 
	{
		CWWTPDebugLog("%s,Error Creating Socket for ioctl",__func__);
		CW_FREE_OBJECT(wlancreating);
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
		//return CWErrorRaise(CW_ERROR_GENERAL, NULL);;
	}
	
	CWWTPGetWlanName(wlanname,addWlanValues->radio_id,addWlanValues->wlan_id);

	 /* pei add for no recreate wlan 0703 */
	if(Check_Interface_State(wlanname) == 0)
	{
		CWSaveAddwlanConfigInfo(wlancreating);
		autelan_close(sock);
		CW_FREE_OBJECT(wlancreating);
		return CW_TRUE;
	}

	/* if is the second wlan in wifi0/1, sleep(3) */
	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
	{
		if((ptr->wlan_id != addWlanValues->wlan_id)&&(ptr->radio_id == addWlanValues->radio_id))
		{
			wlan_in_same_radio ++;
		}
	}
	if((wlan_in_same_radio == 1)&&(isBackup == 0))
	{
		CWWTPDebugLog("Creating the second wlan on wifi%d, sleep 4s.", addWlanValues->radio_id);
		autelan_sleep(4);
	}
	
	wlancreateOK = create_wlan(sock, addWlanValues, wpa_ie, wlanVlanValues, wapiValues,wlan_in_same_radio);
	if(wlancreateOK == 1 && receiveframethread == 0){
		CWThread thread_receiveFrame_dot_11;
		CWThread thread_receiveFrame_dot_3;
		WTPWLanReceiveThreadRun = CW_TRUE ;
		if(!CWErr(CWCreateThread(&thread_receiveFrame_dot_11, CWWTPReceive802_11Frame, wlancreating,0))) {//(void*)&(wlancreating->wlan_id)
			CWWTPDebugLog("Error starting Thread that receive 802.11 binding frame");
		}
		if(!CWErr(CWCreateThread(&thread_receiveFrame_dot_3, CWWTPReceive802_3Frame, wlancreating,0))){
			CWWTPDebugLog("Error starting Thread that receive 802.3 binding frame");
		}
		receiveframethread = 1;

		/* pei add for check station aging, at 080703 */
		CWThread thread_netlink;
		CWWTPDebugLog("start to create the thread check netlink");
		if(!CWErr(CWCreateThread(&thread_netlink, CWWTPCheckNetLink, NULL,0))) {
			CWWTPDebugLog("Error starting Thread that check the netlink");
		}
	}
	/*create the send sock for wlan*/
	if(wlancreateOK == 1){
		 struct sockaddr_ll toaddr1,toaddr2,toaddr3;
		 struct ifreq ifr;
		 int sockd1,sockd2,sockd3;// 1 data 2 eap
		 if((sockd1 = socket(PF_PACKET,SOCK_RAW,autelan_htons(ETH_P_ALL))) < 0){
			CWWTPDebugLog("socket init error");
		 }	
		 if((sockd2 = socket(PF_PACKET,SOCK_RAW,autelan_htons(ETH_P_ALL))) < 0){
			CWWTPDebugLog("socket init error");
		 }
		 strncpy(ifr.ifr_name,wlanname, sizeof(ifr.ifr_name));
		 if (ioctl(sockd1, SIOCGIFINDEX, &ifr) == -1){//bind to a interface
			 CWWTPDebugLog("SIOCGIFINDEX1  failed");
		 }
		 if (ioctl(sockd2, SIOCGIFINDEX, &ifr) == -1){//bind to a interface
			CWWTPDebugLog("SIOCGIFINDEX2  failed");
		 }

		 memset(&toaddr1,'\0',sizeof(toaddr1));
		 toaddr1.sll_family = AF_PACKET;
		 toaddr1.sll_protocol = autelan_htons(0x0019);
		 toaddr1.sll_ifindex = ifr.ifr_ifindex;
		 if(sockd1 >= 0){//zengmin add by Coverity 2013-06-09
			 if(autelan_bind(sockd1,(struct sockaddr *)&toaddr1,sizeof(toaddr1)) < 0){
				CWWTPDebugLog("send sock bind  error");
			 }
		 }
		 memset(&toaddr2,'\0',sizeof(toaddr2));
		 toaddr2.sll_family = AF_PACKET;
		 toaddr2.sll_protocol = autelan_htons(ETH_P_ALL);
		 toaddr2.sll_ifindex = ifr.ifr_ifindex;
		 if(sockd2 >= 0){//zengmin add by Coverity 2013-06-09
			 if(autelan_bind(sockd2,(struct sockaddr *)&toaddr2,sizeof(toaddr2)) < 0){
				CWWTPDebugLog("send sock bind  error");
			 }
		 }
		//diaowq add for CMCC_TEST 802.3 tunnel @20110916
	  	 if((sockd3 = socket(PF_PACKET,SOCK_RAW,autelan_htons(ETH_P_ALL))) < 0){
			CWWTPDebugLog("socket sockd3 init error");
		 }
		 strncpy(ifr.ifr_name,wlanname, sizeof(ifr.ifr_name));
		 if (ioctl(sockd3, SIOCGIFINDEX, &ifr) == -1){//bind to a interface
			CWWTPDebugLog("sockd3 SIOCGIFINDEX2  failed");
		 }
		 memset(&toaddr3,'\0',sizeof(toaddr3));
		 toaddr3.sll_family = AF_PACKET;
		 toaddr3.sll_protocol = autelan_htons(0x0030);
		 toaddr3.sll_ifindex = ifr.ifr_ifindex;
		 if(sockd3 >= 0){//zengmin add by Coverity 2013-06-09
			 if(autelan_bind(sockd3,(struct sockaddr *)&toaddr3,sizeof(toaddr3)) < 0){
				CWWTPDebugLog(" sockd3 bind error");
			 }
		 }
		 wlancreating->wlan_sendsock = sockd1;
		 wlancreating->wlan_eapsock  = sockd2;
		 wlancreating->wlan_dot3sock = sockd3;

		WlanTableAdd(wlancreating, wlanVlanValues->vlanId);
		CWSaveAddwlanConfigInfo(wlancreating);
	}
	CW_FREE_OBJECT(wlancreating);
	autelan_close(sock);
	
	/* if is the first wlan in wifi0/1, sleep(3) */
/*	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
	{
		if((ptr->wlan_id != addWlanValues->wlan_id)&&(ptr->radio_id == addWlanValues->radio_id))
		{
			wlan_in_same_radio = 1;
			break;
		}
	}
	if(wlan_in_same_radio == 0)
	{
		dpf("Creating the first wlan on wifi%d, sleep 3s.\n", addWlanValues->radio_id);
		sleep(3);
	}
*/
	
	return CW_TRUE;
}

/***************************************************************************
 * 
 * Function:  CWBindingDeleteWlan
 *
 * Purpose:  delete a wlan in the WTP 
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/
CWBool CWBindingDeleteWlan(int deletewlancount,DeleteWlanValues *deleteWlanValues,CWProtocolResultCode *resultCode)
{
	int i;
//	CWBool wlanfound;
//	char wlanname[IFNAMSIZ];
	
	if (deleteWlanValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	*resultCode = CW_PROTOCOL_SUCCESS;

	for(i=0;i<deletewlancount;i++){
		CWWTPWlan *ptr = NULL;
		CWWTPWlan *oldptr = NULL;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			if((ptr->wlan_id == deleteWlanValues->wlan_id)&&(ptr->radio_id == deleteWlanValues->radio_id))
			{
				delete_wlan(&deleteWlanValues[i]);
				WLanTableDelete(&deleteWlanValues[i]);
				//remove the dir of the deleted wlan
				char cmdbuf[128];
				memset(cmdbuf,0,128);
				autelan_sprintf(cmdbuf,"rm -rf /tmp/conf_wtpd/radio%d/wlaninfo/wlan%d",deleteWlanValues->radio_id,deleteWlanValues->wlan_id);
				CWWTPDebugLog("delete wlan:ath.%d-%d",deleteWlanValues->radio_id,deleteWlanValues->wlan_id);
				_system(cmdbuf);
				if(gEMenuConf.e_swtich == 1 && gRunRetry == 0){
					CWXmlWriteWlanParam(_jffs_config_xml_,deleteWlanValues->radio_id,deleteWlanValues->wlan_id,  "create","no");
					gWlanIDMap[CWGetWlanIdxInMap(gWlanIDMap,deleteWlanValues->radio_id,deleteWlanValues->wlan_id)] = 0;
				}
				break;
			}
		}
		
		int wlan_count = 0;
		for(ptr = wtp_wlan_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next)
		{
			if(deleteWlanValues->radio_id == ptr->radio_id)
			{
				wlan_count++;
			}
		}
		ConfArg confArg;		
		memset(&confArg,0,sizeof(ConfArg));
		confArg.radioID = deleteWlanValues->radio_id;
		memcpy(confArg.conf_arg,"wlan_count",11);
		autelan_sprintf(confArg.conf_value,"%d",wlan_count);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);
/*		if(WLanTableDelete(&deleteWlanValues[i]) == CW_TRUE)
			delete_wlan(&deleteWlanValues[i]);
		else
			CWWTPDebugLog("can't find the delete wlan");*/
	}

	return CW_TRUE;

}
CWBool CWBindingUpdateWlan(int updatewlancount,UpdateWlanValues *updateWlanValues,CWProtocolResultCode *resultCode)
{
	return CW_TRUE;
}
CWBool CWBindingWlanVlan(int wlanVlancount, WlanVlanValues *wlanVlanValues, CWProtocolResultCode *resultCode)
{
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	return CW_TRUE;
}
/***************************************************************************
 * 
 * Function:  CWBindingAddSta
 *
 * Purpose:  add a sta to the wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/
 CWBool CWBindingAddSta(AddSTAValues *addStaValues,CWProtocolResultCode *resultCode)
 {
	int sock = -1;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		CWWTPDebugLog("%s,errorcode %dError Creating Socket for ioctl",__func__,CW_PROTOCOL_FAILURE); 
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}
	if(0 == add_sta(sock,addStaValues))
	{
		
		CWWTPDebugLog("sta authorize success,add to sta table...");
		/*add the sta to wtp's sta table*/
		STATableAdd(addStaValues);
		STAMacRefreshForBackup(addStaValues->radio_id);
	}
	else	 
		*resultCode = CW_PROTOCOL_FAILURE;
	autelan_close(sock);
	if(*resultCode == CW_PROTOCOL_SUCCESS)//Add by zengmin for fix sta info error 2012-11-23 ONLINEBUG-692
	{
		CWWTPGetAddedStaInfo(addStaValues);//yuan add for report added sta's info
	}
		
	return CW_TRUE;
 	
 }
/***************************************************************************
 * 
 * Function:  CWBindingDeleteSta
 *
 * Purpose:  delete a sta of the wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/
#if 0 /* pei add 0105*/
CWBool CWBindingDeleteSta(DeleteSTAValues *deleteSTAValues,CWProtocolResultCode *resultCode)
{
	dpf("delete a sta \n");
	STATableDelete(deleteSTAValues);
	*resultCode = CW_PROTOCOL_SUCCESS;
	return CW_TRUE;
}
#else
CWBool GetWlanId(DeleteSTAValues *deleteSTAValues, unsigned char *wlan_id)
{
	CWWTPSta *ptr;
	CWWTPSta *oldptr;
	CWThreadMutexLock(&sta_table_mutex);
	if(wtp_sta_list == NULL || deleteSTAValues == NULL){
		CWWTPDebugLog("no sta left");
		CWThreadMutexUnlock(&sta_table_mutex);
		return CW_FALSE;
	}
	oldptr = ptr = NULL;
	for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next){
		if(macAddrCmp(ptr->sta_mac,deleteSTAValues->mac_addr) == 1){
			*wlan_id = ptr->wlan_id;
			CWThreadMutexUnlock(&sta_table_mutex);
			return CW_TRUE;
		}
	}
	CWWTPDebugLog("can't found the sta\n");
	CWThreadMutexUnlock(&sta_table_mutex);
	return CW_FALSE;
}
CWBool CWBindingDeleteSta(DeleteSTAValues *deleteSTAValues,CWProtocolResultCode *resultCode)
{
	int sock = -1;
	unsigned char wlan_id;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		CWWTPDebugLog("%s,error code %d Error Creating Socket for ioctl",__func__,CW_PROTOCOL_FAILURE); 
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}
	/*delete the sta from wtp's sta table*/
#if 1  //open this way by liuhj for APV5 & AC3.0 
	if(GetWlanId(deleteSTAValues, &wlan_id)==0)
	{
		autelan_close(sock);
		return CW_TRUE;
	}
#else //pei modified 0306
	wlan_id = deleteSTAValues->wlan_id;
#endif
	CWWTPDebugLog("delete sta from AC,wlan:ath.%d-%d",deleteSTAValues->radio_id,wlan_id);
	STATableDelete(deleteSTAValues);
	if(0==del_sta(sock, deleteSTAValues, &wlan_id))
		;
	else
		*resultCode = CW_PROTOCOL_FAILURE;
	autelan_close(sock);
	return CW_TRUE;
}
#endif

/*
*
* Purpose:set wapi key
*
*/
int wapi_set_key( int sock, const char *alg,/*WPI-SMS4*/
		const u8 *addr, char *ifname, int key_idx,
		const u8 *key, size_t key_len,int ismcast)
{
#define	IEEE80211_KEY_XMIT	0x01		/* key used for xmit */
#define	IEEE80211_KEY_RECV	0x02		/* key used for recv */
#define	IEEE80211_KEY_DEFAULT	0x80	/* default xmit key */
#define IEEE80211_CIPHER_WAPI 4
	struct ieee80211req_key wk;
	u8 cipher;

	memset(&wk, 0, sizeof(wk));	

	if (strcmp(alg, "WPI-SMS4") == 0)
		cipher = IEEE80211_CIPHER_WAPI;
	else {
		autelan_printf("%s: unknown/unsupported algorithm %s\n", __func__, alg);
		return -1;
	}
/*
	if (key_len > sizeof(wk.ik_keydata)) {
		printf("%s: key length %lu too big\n", __func__, (unsigned long) key_len);
		return -3;
	}
*/
	memset(&wk, 0, sizeof(wk));
	wk.ik_type = cipher;
	wk.ik_flags = IEEE80211_KEY_RECV | IEEE80211_KEY_XMIT;
	
	if (ismcast) {
		wk.ik_keyix = key_idx;
		wk.ik_pad = key_idx;
		wk.ik_flags |= IEEE80211_KEY_DEFAULT;
		memset(wk.ik_macaddr, 0xFF, MAC_ADDR_LEN);
	} else {
		wk.ik_keyix = IEEE80211_KEYIX_NONE;
		//wk.ik_pad = key_idx;
		wk.ik_pad = 0;  //resolve wapi can not fit for black adapter
		memcpy(wk.ik_macaddr, addr, MAC_ADDR_LEN);
	}
	wk.ik_keylen = key_len;
	memcpy(wk.ik_keydata, key, key_len);
	return set80211priv(sock,ifname,IEEE80211_IOCTL_SETKEY, (void *)&wk, sizeof(wk));
}


CWBool CWBindingSetSTASessionKey( STASessionKeyValues *staSessionKey, Ieee80211InfoEleValues *ieee80211_InfoEleValues, CWProtocolResultCode *resultCode)
{
	int sock = -1;
	//int i;
	unsigned char broadcastmac[6];
/*	int res = 0;  */
	char wlanname[IFNAMSIZ];
	unsigned int key_idx;

	CWWTPGetWlanName(wlanname, ieee80211_InfoEleValues->radio_id, ieee80211_InfoEleValues->wlan_id);
	memset(broadcastmac, 0xff, MAC_ADDR_LEN);
	
	
//	CWWTPDebugLog("into set session key:[%02x-%02x-%02x-%02x-%02x-%02x]",staSessionKey->mac[0],staSessionKey->mac[1],
//		staSessionKey->mac[2],staSessionKey->mac[3],staSessionKey->mac[4],staSessionKey->mac[5]);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		CWWTPDebugLog("%s,Error Creating Socket for ioctl",__func__); 
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}
	//CWWTPDebugLog("pairwise_cipher = %d wlan_id = %d",ieee80211_InfoEleValues->wtp_ie->pairwise_cipher,ieee80211_InfoEleValues->wlan_id);
	if(macAddrCmp(staSessionKey->mac,broadcastmac) == 1){
//		CW_FREE_OBJECT(staSessionKey->mac);/*csc 080731*/  
//		staSessionKey->mac = NULL;
	
		if(ieee80211_InfoEleValues->wtp_ie->pairwise_cipher == WTP_WAPI_CIPHER_SMS4)
		{
			char buf[128];
			int buf_len = 100;
		//	CWWTPDebugLog("mcast-key:%s", staSessionKey->key);
		//	CWWTPDebugLog("buf_len:%d  wlanname:%s", buf_len, wlanname);
			
			
			memset(buf, 0, 128);
			/*
			* for aquia:
			* the Key negotiation with AC starts from 12th bytes.
			*/
			memcpy(buf, staSessionKey->key + 12, buf_len - 12); 

//			res = wapid_ioctl(wlanname, P80211_IOCTL_SETWAPI_INFO, buf, buf_len); 

		    key_idx = (unsigned int)((staSessionKey->flags>>12)&~(0xfffC));
			if(0==wapi_set_key(sock,"WPI-SMS4",staSessionKey->mac,wlanname,key_idx,(u8 *)buf,32/*key_len*/,1))
				;
			else
				*resultCode=CW_PROTOCOL_FAILURE;
			autelan_sleep(3);
			if(debug_print)
			{
				autelan_printf("!!!!!!1!!!!!!!!mcast-key:%s\n", staSessionKey->key);
				autelan_printf("buf_len:%d\nwlanname:%s\n", buf_len, wlanname);
			}
			
			CW_FREE_OBJECT(staSessionKey->mac);
   			staSessionKey->mac = NULL;
		}
		else{
			CW_FREE_OBJECT(staSessionKey->mac);
   			staSessionKey->mac = NULL;
			if(0==madwifi_set_key(sock,staSessionKey,ieee80211_InfoEleValues->wtp_ie->pairwise_cipher, ieee80211_InfoEleValues->radio_id,ieee80211_InfoEleValues->wlan_id))
				;
			else
				*resultCode=CW_PROTOCOL_FAILURE;
	}
#if 0
		if(isWPAfirstKey){
			sprintf(cmdbuf,"/sbin/ifconfig ath%d up",ieee80211_InfoEleValues->wlan_id);
			_system(cmdbuf);
			madwifi_set_privacy(sock, ieee80211_InfoEleValues->wlan_id, 1);
			isWPAfirstKey = CW_FALSE;
		}
#endif
	}
	else
	{
		if(ieee80211_InfoEleValues->wtp_ie->pairwise_cipher == WTP_WAPI_CIPHER_SMS4)
		{
			char buf[128];
			int buf_len = 100;
			AddSTAValues addStaValues;

		//	CWWTPDebugLog("ucast-key:%s", staSessionKey->key);
		//	CWWTPDebugLog("buf_len:%d", buf_len);
			
			memset(buf, 0, 128);
			memcpy(buf, staSessionKey->key + 12, buf_len - 12);
			
			memset(&addStaValues, 0, sizeof(addStaValues));
			addStaValues.radio_id = ieee80211_InfoEleValues->radio_id;
			addStaValues.wlan_id = ieee80211_InfoEleValues->wlan_id;
			addStaValues.mac_length = MAC_ADDR_LEN;
			char *tmp;
			CW_CREATE_STRING_ERR(tmp, MAC_ADDR_LEN, {autelan_close(sock);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
			addStaValues.mac_addr = (unsigned char *)tmp;
			memset(addStaValues.mac_addr, 0, MAC_ADDR_LEN+1);
			memcpy(addStaValues.mac_addr, staSessionKey->mac, MAC_ADDR_LEN);
			CWWTPDebugLog("add sta to table because set sta session key");
			STATableAdd(&addStaValues);
			
			//res = wapid_ioctl(wlanname, P80211_IOCTL_SETWAPI_INFO, buf, buf_len);
			
		    key_idx = (unsigned int)((staSessionKey->flags>>12)&~(0xfffC));
			if(0==wapi_set_key(sock,"WPI-SMS4",staSessionKey->mac,wlanname,key_idx,(u8 *)buf,32/*key_len*/,0))
				;
			else
				*resultCode=CW_PROTOCOL_FAILURE;
			CW_FREE_OBJECT(addStaValues.mac_addr);
		}
		else
		{
			if(0==madwifi_set_key(sock,staSessionKey,ieee80211_InfoEleValues->wtp_ie->pairwise_cipher,ieee80211_InfoEleValues->radio_id,ieee80211_InfoEleValues->wlan_id))
				;
			else
				*resultCode=CW_PROTOCOL_FAILURE;
		}
	}
	autelan_close(sock);
	return CW_TRUE;
}
/***************************************************************************
 * 
 * Function:  acw_binding_set_ampdu
 *
 * Purpose:  mainly used for ac set the ampdu of wtp
 *
 * Inputs:  values
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool acw_binding_set_ampdu(ampdu_op_type*set_ampdu_values, CWProtocolResultCode *resultCode)
{
	char wlanname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	memset(wlanname, 0, IFNAMSIZ);
	if(set_ampdu_values == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;

	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == set_ampdu_values->radio_id){
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	if(set_ampdu_values->op_type==RADIO)
	{
		/*down interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next)
		{
			if(ptr->radio_id == set_ampdu_values->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
			       CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			       autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
		CWWTPGetRadioName(wlanname,set_ampdu_values->radio_id);
		set_wifi_ampdu_cmd(wlanname,set_ampdu_values->state, set_ampdu_values->ampdulimit, set_ampdu_values->ampduframes);

		CWWTPDebugLog("wlanname=%s set_ampdu_values->state=%d set_ampdu_values->ampdulimit=%d ampduframes=%d!",wlanname,set_ampdu_values->state,set_ampdu_values->ampdulimit,set_ampdu_values->ampduframes);

		/* up interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next)
		{
			if(ptr->radio_id == set_ampdu_values->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
			    CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			    autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
	}
       
	else if(set_ampdu_values->op_type==WLAN){
		CWWTPGetWlanName(wlanname,set_ampdu_values->radio_id,set_ampdu_values->wlan_id);
		autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname); 
		autelan_system(cmdbuf);
		set_ampdu_cmd(wlanname,set_ampdu_values->state, set_ampdu_values->ampdulimit, set_ampdu_values->ampduframes);
		CWWTPDebugLog("wlanname=%s set_ampdu_values->state=%d set_ampdu_values->ampdulimit=%d ampduframes=%d!",wlanname,set_ampdu_values->state,set_ampdu_values->ampdulimit,set_ampdu_values->ampduframes);
		autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname); 
		autelan_system(cmdbuf);
		
	}
       

	return CW_TRUE;
}
CWBool acw_binding_set_amsdu(amsdu_op_type*set_amsdu_values, CWProtocolResultCode *resultCode)
{
	char wlanname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	memset(wlanname, 0, IFNAMSIZ);
	if(set_amsdu_values == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;

	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == set_amsdu_values->radio_id){
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

       if(set_amsdu_values->op_type==RADIO){
		/*down interface*/
		 for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_amsdu_values->radio_id)
			{
			    ptr->wlan_updown_time++;  //pei add 0225
			    CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			    autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname);
			    autelan_system(cmdbuf);
			}
		}
		//CWWTPGetRadioName(wlanname,set_amsdu_values->radio_id);	
		//set_wifi_amsdu_cmd(wlanname,set_amsdu_values->state, set_amsdu_values->amsdulimit);
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_amsdu_values->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				set_amsdu_cmd(wlanname,set_amsdu_values->state, set_amsdu_values->amsdulimit);

				CWWTPDebugLog("wlanname=%s set_amsdu_values->state=%d set_amsdu_values->amsdulimit=%d !",wlanname,set_amsdu_values->state,set_amsdu_values->amsdulimit);
			}
		}

		/* up interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_amsdu_values->radio_id)
			{
			    ptr->wlan_updown_time++;  //pei add 0225
			    CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			    autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
			    autelan_system(cmdbuf);
			}
		}
        }
       
	else if(set_amsdu_values->op_type==WLAN){
		CWWTPGetWlanName(wlanname,set_amsdu_values->radio_id,set_amsdu_values->wlan_id);
		autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname); 
		autelan_system(cmdbuf);
	       set_amsdu_cmd(wlanname,set_amsdu_values->state, set_amsdu_values->amsdulimit);
		CWWTPDebugLog("wlanname=%s set_amsdu_values->state=%d set_amsdu_values->amsdulimit=%d amsduframes=%d!",wlanname,set_amsdu_values->state,set_amsdu_values->amsdulimit,set_amsdu_values->amsduframes);
             autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname); 
             autelan_system(cmdbuf);
		
	}


	return CW_TRUE;
}
CWBool acw_binding_set_puren_mixed(puren_mixed_op_type*set_puren_mixed_values, CWProtocolResultCode *resultCode)
{
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	memset(wlanname, 0, IFNAMSIZ);
	if(set_puren_mixed_values == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;

	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == set_puren_mixed_values->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}
	if(set_puren_mixed_values->op_type==RADIO){
		/*down interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_puren_mixed_values->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
				autelan_system(cmdbuf);
			}
		}
	}
	if(set_puren_mixed_values->op_type==WLAN){
             CWWTPGetWlanName(wlanname,set_puren_mixed_values->radio_id,set_puren_mixed_values->wlan_id);
             autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname); 
             autelan_system(cmdbuf);
	}
        
	set_puren_cmd(wlanname, set_puren_mixed_values->state);

	CWWTPDebugLog("wlanname=%s set_puren_mixed_values->state=%d",wlanname,set_puren_mixed_values->state);
       if(set_puren_mixed_values->op_type==WLAN){
		autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname); 
		autelan_system(cmdbuf);
       }
       
       if(set_puren_mixed_values->op_type==RADIO){
		/* up interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_puren_mixed_values->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
				autelan_system(cmdbuf);
			}
		}
	}



	return CW_TRUE;
}

CWBool acw_binding_set_dhcp_snooping(dhcp_snooping_type*set_dhcp_snooping_values, CWProtocolResultCode *resultCode)
{
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	
	if(set_dhcp_snooping_values == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == set_dhcp_snooping_values->radio_id){
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	set_dhcp_snooping(set_dhcp_snooping_values->state);

	return CW_TRUE;

}

#if 0
CWBool acw_binding_set_sta_info_report(sta_info_repport_type*set_sta_info_report_values, CWProtocolResultCode *resultCode)
{
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;

	if(set_sta_info_report_values == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == set_sta_info_report_values->radio_id){
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

       if(set_sta_report_by_dhcpsnooping(set_sta_info_report_values->state)==0)
		return CW_FALSE;


	return CW_TRUE;
}
#endif

CWBool acw_binding_set_channel_extoffset(channel_extoffset_op_type *set_channel_extoffset_values, CWProtocolResultCode *resultCode)
{
	char wlanname[IFNAMSIZ];
	char ifname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	char *mode=NULL;
	int channel ;
	
	memset(wlanname, 0, IFNAMSIZ);
	
	if(set_channel_extoffset_values == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == set_channel_extoffset_values->radio_id){
			CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}
	gModeNParaValue[set_channel_extoffset_values->radio_id].extoffset=set_channel_extoffset_values->state;

	if(!get_radio_mode(gRadioInfoValue[set_channel_extoffset_values->radio_id].radiotype,&mode))
	{
		CW_FREE_OBJECT(mode);
		return CW_FALSE;
	}
	if(set_channel_extoffset_values->op_type==RADIO){
		/*down interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_channel_extoffset_values->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", ifname);
				autelan_system(cmdbuf);
				set_channel_extoffset_cmd(ifname,mode,set_channel_extoffset_values->state, gModeNParaValue[set_channel_extoffset_values->radio_id].cwmMode);
			}
		}
	}
	if(gApScanningEnable == 1)
	{
		autelan_sprintf(ifname,"athscanwifi0");
		set_channel_extoffset_cmd(ifname,mode,set_channel_extoffset_values->state, gModeNParaValue[set_channel_extoffset_values->radio_id].cwmMode);
	}
	if(set_channel_extoffset_values->op_type==WLAN){
		CWWTPGetWlanName(wlanname,set_channel_extoffset_values->radio_id,set_channel_extoffset_values->wlan_id);
		autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname);
		autelan_system(cmdbuf);
		set_channel_extoffset_cmd(wlanname,mode,set_channel_extoffset_values->state, gModeNParaValue[set_channel_extoffset_values->radio_id].cwmMode);
	}

	CW_FREE_OBJECT(mode);
	channel = gRadioInfoValue[set_channel_extoffset_values->radio_id].channel;
	memset(cmdbuf, 0, 128);
	autelan_sprintf(cmdbuf,"iwconfig %s channel %d",wlanname,channel);
	_system(cmdbuf);
	CWWTPDebugLog("wlanname=%s set_channel_extoffset_values->state=%d",wlanname,set_channel_extoffset_values->state);

       if(set_channel_extoffset_values->op_type==WLAN){
             autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
             autelan_system(cmdbuf);
       }
#if 0
	char **wlannamelist;
	int i=0,j=0;
	CW_CREATE_ARRAY_ERR(wlannamelist, wtp_wlan_count, char*,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		 
		    if(ptr->radio_id == set_channel_extoffset_values->radio_id)
		    {
				ptr->wlan_updown_time++;  
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
				printf("up ifname %s!\n",wlanname);
				_system(cmdbuf);
				CW_CREATE_STRING_FROM_STRING_ERR(wlannamelist[i], wlanname, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				i++;
		    }
	    }
	for(j=wtp_wlan_count-1;j>=0;j--){
		sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null",wlannamelist[j]);
		_system(cmdbuf);
		printf("up ifname %s!\n",wlannamelist[j]);
	}
	CW_FREE_OBJECTS_ARRAY(wlannamelist, wtp_wlan_count);
#endif
	if(set_channel_extoffset_values->op_type==RADIO){
		/* up interface*/
		autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
		
		autelan_system(cmdbuf);
		autelan_sleep(1);
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_channel_extoffset_values->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
				
				autelan_system(cmdbuf);
			}
		}
	}


	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = set_channel_extoffset_values->radio_id;
	strcpy(confArg.conf_arg,"extoffset");
	autelan_sprintf(confArg.conf_value,"%d",set_channel_extoffset_values->state);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);
	
	return CW_TRUE;
}

CWBool acw_binding_set_tx_chainmask(tx_chainmask_op_type *set_tx_chainmask_value, CWProtocolResultCode *resultCode)
{
	char wlanname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	memset(wlanname, 0, IFNAMSIZ);
	if(set_tx_chainmask_value == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == set_tx_chainmask_value->radio_id){
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	if(set_tx_chainmask_value->op_type==RADIO){
		/*down interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_tx_chainmask_value->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
		CWWTPGetRadioName(wlanname,set_tx_chainmask_value->radio_id);	
		set_wifi_tx_chainmask_cmd(wlanname, set_tx_chainmask_value->state);

		CWWTPDebugLog("wifiname=%s set_tx_chainmask_value->state=%d ",wlanname,set_tx_chainmask_value->state);

		/* up interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_tx_chainmask_value->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
	}
	else if(set_tx_chainmask_value->op_type==WLAN){
		CWWTPGetWlanName(wlanname,set_tx_chainmask_value->radio_id,set_tx_chainmask_value->wlan_id);
//		sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname); 
//		_system(cmdbuf);
		set_tx_chainmask_cmd(wlanname, set_tx_chainmask_value->state);
		CWWTPDebugLog("wlanname=%s set_tx_chainmask_value->state=%d !\n",wlanname,set_tx_chainmask_value->state);

//		sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname); 
//		_system(cmdbuf);
	}


	return CW_TRUE;
}

CWBool acw_binding_set_rx_chainmask(rx_chainmask_op_type *set_rx_chainmask_value, CWProtocolResultCode *resultCode)
{
	char wlanname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	memset(wlanname, 0, IFNAMSIZ);
	if(set_rx_chainmask_value == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == set_rx_chainmask_value->radio_id){
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		*resultCode = CW_PROTOCOL_FAILURE;
		return CW_TRUE;
	}

	if(set_rx_chainmask_value->op_type==RADIO){
		/*down interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_rx_chainmask_value->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
		CWWTPGetRadioName(wlanname,set_rx_chainmask_value->radio_id);	
		set_wifi_rx_chainmask_cmd(wlanname, set_rx_chainmask_value->state);

		CWWTPDebugLog("wifiname=%s set_rx_chainmask_value->state=%d ",wlanname,set_rx_chainmask_value->state);

		/* up interface*/
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == set_rx_chainmask_value->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
	}
	else if(set_rx_chainmask_value->op_type==WLAN){
		CWWTPGetWlanName(wlanname,set_rx_chainmask_value->radio_id,set_rx_chainmask_value->wlan_id);
//		sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname); 
//		_system(cmdbuf);
		set_rx_chainmask_cmd(wlanname, set_rx_chainmask_value->state);
		CWWTPDebugLog("wlanname=%s set_rx_chainmask_value->state=%d !\n",wlanname,set_rx_chainmask_value->state);

//		sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname); 
//		_system(cmdbuf);
	}


	return CW_TRUE;
}
CWBool acw_binding_set_ack_timeout(ack_timeout_op_type *set_ack_timeout_value, CWProtocolResultCode *resultCode)
{
	char cmdbuf[128];
	memset(cmdbuf, 0, 128);
	if(set_ack_timeout_value == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	autelan_sprintf(cmdbuf,"acktimeout wifi%d set %d", set_ack_timeout_value->radio_id, set_ack_timeout_value->distance); 
	_system(cmdbuf);
	
//	CWWTPDebugLog("wifiname=wifi%d set_ack_timeout_value->state=%d set_ack_timeout_value->distance=%d !\n",set_ack_timeout_value->radio_id,set_ack_timeout_value->state,set_ack_timeout_value->distance);
	

	return CW_TRUE;
}

CWBool CWSaveWmmInlist(unsigned char radio_id,unsigned char qos_enable)
{

	CWWTPWmmSetList *wmm_set_list_node = NULL;
	CWWTPWmmSetList * ptr = NULL;
	CWWTPWmmSetList * oldptr = NULL;
	CWThreadMutexLock(&wmm_set_mutex);
	CWWTPDebugLog("radio id %d,Qos_enable = %d\n",radio_id,qos_enable);
	for(ptr = g_wmm_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next)
	{
		if((ptr->radio_id == radio_id)&&(1 == ptr->set_flag))
		{
			ptr->enable = qos_enable;
			goto OK;
		}
	}
	CW_CREATE_OBJECT_ERR(wmm_set_list_node, CWWTPWmmSetList, {CWThreadMutexUnlock(&wmm_set_mutex);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	memset(wmm_set_list_node,0,sizeof(CWWTPWmmSetList));
	wmm_set_list_node->radio_id = radio_id;
	wmm_set_list_node->enable= qos_enable;
	wmm_set_list_node->set_flag = 1;
	wmm_set_list_node ->next = g_wmm_list;
	g_wmm_list = wmm_set_list_node;
OK:	
	CWThreadMutexUnlock(&wmm_set_mutex);
	return CW_TRUE;
}
CWBool CWParseWTPQoS (CWProtocolMessage *msgPtr, int len, unsigned char* radioID, unsigned char* tagPackets, WTP_QosValues* valPtr)
{	
	int i,k;
	int dot1p_map_wmm_num = 0;
	unsigned char Qos_enable = 0;

	CWParseMessageElementStart();
	*radioID = CWProtocolRetrieve8(msgPtr);
	*tagPackets = CWProtocolRetrieve8(msgPtr);
	
#if 0
	for(i=0; i<NUM_QOS_PROFILES; i++)
	{
		valPtr[i].queueDepth=(unsigned char)CWProtocolRetrieve8(msgPtr);
		valPtr[i].cwMin=CWProtocolRetrieve16(msgPtr);
		valPtr[i].cwMax=CWProtocolRetrieve16(msgPtr);
		valPtr[i].AIFS=(unsigned char)CWProtocolRetrieve8(msgPtr);
		valPtr[i].dot1PTag=(unsigned char)CWProtocolRetrieve8(msgPtr);
		valPtr[i].DSCPTag=(unsigned char)CWProtocolRetrieve8(msgPtr);
	}
#else
	Qos_enable = CWProtocolRetrieve8(msgPtr);
	//gQosEnable = Qos_enable;

	CWSaveWmmInlist(*radioID,Qos_enable);
	CWWTPDebugLog("QoS radio_id:%d Qos_enable:%d", *radioID, Qos_enable);
	for(i=0; i<NUM_QOS_PROFILES*2; i++)
	{
		valPtr[i].WMMObject = CWProtocolRetrieve8(msgPtr);
		valPtr[i].QosOrder = CWProtocolRetrieve8(msgPtr);
		valPtr[i].Length = CWProtocolRetrieve16(msgPtr);
		valPtr[i].QueueDepth = CWProtocolRetrieve8(msgPtr);
		valPtr[i].CWMin = CWProtocolRetrieve16(msgPtr);
		valPtr[i].CWMax = CWProtocolRetrieve16(msgPtr);
		valPtr[i].AIFS = CWProtocolRetrieve8(msgPtr);
		valPtr[i].TXOPlimit = CWProtocolRetrieve16(msgPtr);
		CWWTPDebugLog("WMMObject:%d QosOrder:%d Length:%d QueueDepth:%d CWMin:%d CWMax:%d AIFS:%d TXOPlimit:%d", valPtr[i].WMMObject, 
			valPtr[i].QosOrder, valPtr[i].Length, valPtr[i].QueueDepth, valPtr[i].CWMin, valPtr[i].CWMax, valPtr[i].AIFS, valPtr[i].TXOPlimit);
		if(valPtr[i].WMMObject == 0)
		{
			valPtr[i].ACK = CWProtocolRetrieve8(msgPtr);
			valPtr[i].mapstate = CWProtocolRetrieve8(msgPtr);
			CWWTPDebugLog("ACK:%d mapstate:%d", valPtr[i].ACK, valPtr[i].mapstate);
			if(valPtr[i].mapstate == 1)
			{
				valPtr[i].wmm_map_dot1p = CWProtocolRetrieve8(msgPtr);
				valPtr[i].dot1p_map_wmm_num = CWProtocolRetrieve8(msgPtr);
				CWWTPDebugLog("wmm_map_dot1p:%d dot1p_map_wmm_num:%d", valPtr[i].wmm_map_dot1p, valPtr[i].dot1p_map_wmm_num);
				dot1p_map_wmm_num = valPtr[i].dot1p_map_wmm_num;
				for(k=0;k<dot1p_map_wmm_num;k++)
				{
					valPtr[i].dot1p_map_wmm[k] = CWProtocolRetrieve8(msgPtr);
					CWWTPDebugLog("dot1p_map_wmm[%d]:%d", k, valPtr[i].dot1p_map_wmm[k]);
				}
			}
		}
	}
#endif
	CWParseMessageElementEnd();
}
/***************************************************************************
 * 
 * Function:  CWParseDireceSequeueControl
 *
 * Purpose:   parse binding message element of configuration update request   
 *			(directsequeuecontrol message element)
 *
 * Inputs:  msg (the control message)
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWParseDireceSequeueControl(CWProtocolMessage *msgPtr, int len,  DirectSequeueControlValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->reserved = CWProtocolRetrieve8(msgPtr);
	valPtr->current_channel = CWProtocolRetrieve8(msgPtr);
	valPtr->currend_CCA = CWProtocolRetrieve8(msgPtr);
	valPtr->energy_detect_threshold = CWProtocolRetrieve32(msgPtr);
	CWParseMessageElementEnd();
}
CWBool CWParseTxPower(CWProtocolMessage *msgPtr, int len,  TxPowerValues*valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->reserved = CWProtocolRetrieve8(msgPtr);
	valPtr->current_power= CWProtocolRetrieve16(msgPtr);
	CWWTPDebugLog("parsing----radioId:%d  reserved:%d  current_power:%d", valPtr->radio_id, valPtr->reserved, valPtr->current_power);
	CWParseMessageElementEnd();
}

CWBool CWParseAddWlan(CWProtocolMessage *msgPtr, int len, AddWlanValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);
	valPtr->capabilities = CWProtocolRetrieve16(msgPtr);
	valPtr->key_index = CWProtocolRetrieve8(msgPtr);
	valPtr->key_status = CWProtocolRetrieve8(msgPtr);
	valPtr->key_length = CWProtocolRetrieve16(msgPtr);
	valPtr->key = CWProtocolRetrieveStr(msgPtr,valPtr->key_length);/* caosicong 080702*/
	//valPtr->group_tsc =CWProtocolRetrieveStr(msgPtr,6);
	msgPtr->offset += 6;/*jump the group_tsc legth*/
	valPtr->qos = CWProtocolRetrieve8(msgPtr);
	valPtr->auth_type = CWProtocolRetrieve8(msgPtr);
	valPtr->mac_mode = CWProtocolRetrieve8(msgPtr);
	valPtr->tunnel_mode = CWProtocolRetrieve8(msgPtr);
	valPtr->suppress_ssid = CWProtocolRetrieve8(msgPtr);
	valPtr->ssid = CWProtocolRetrieveStr(msgPtr,len+4-msgPtr->offset);
	
	CWParseMessageElementEnd();
}

CWBool CWBindingParseChangeWlanTunnelMode(CWProtocolMessage *msgPtr, int len, ChangeTunnelMode*valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);
	valPtr->mac_mode=CWProtocolRetrieve8(msgPtr);
	valPtr->tunnel_mode = CWProtocolRetrieve8(msgPtr);
	CWWTPDebugLog("radio %d wlan %d tunnel mode:%s",valPtr->radio_id,valPtr->wlan_id,(valPtr->tunnel_mode==CW_802_DOT_3_TUNNEL)?"802.3":"802.11");
	CWParseMessageElementEnd();
}


CWBool CWParseInformationElement(CWProtocolMessage *msgPtr, int len, Ieee80211InfoEleValues *valPtr)       //pei add 0606
{
	CWParseMessageElementStart();
	//msgPtr->offset+=1;        //jump radio_id
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);
	msgPtr->offset+= 1;        //jump flags
	valPtr->wtp_ie->elem_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wtp_ie->len = CWProtocolRetrieve8(msgPtr);
	valPtr->wtp_ie->oui[0] = CWProtocolRetrieve8(msgPtr);
	valPtr->wtp_ie->oui[1] = CWProtocolRetrieve8(msgPtr);
	valPtr->wtp_ie->oui[2] = CWProtocolRetrieve8(msgPtr);
	valPtr->wtp_ie->oui_type = CWProtocolRetrieve8(msgPtr);
	valPtr->wtp_ie->version[0] = CWProtocolRetrieve8(msgPtr);
	valPtr->wtp_ie->version[1] = CWProtocolRetrieve8(msgPtr);
	valPtr->wtp_ie->proto = CWProtocolRetrieve32(msgPtr);
	valPtr->wtp_ie->pairwise_cipher = CWProtocolRetrieve32(msgPtr);
	valPtr->wtp_ie->group_cipher = CWProtocolRetrieve32(msgPtr);
	valPtr->wtp_ie->key_mgmt = CWProtocolRetrieve32(msgPtr);
	valPtr->wtp_ie->capabilities = CWProtocolRetrieve32(msgPtr);
	valPtr->wtp_ie->num_pmkid = CWProtocolRetrieve32(msgPtr);
	valPtr->wtp_ie->mgmt_group_cipher = CWProtocolRetrieve32(msgPtr);

	CWParseMessageElementEnd();
}
CWBool CWParseWlanVlan(CWProtocolMessage *msgPtr, int len, WlanVlanValues *valPtr)
{
	int vlanTag;
	CWParseMessageElementStart();
	
	valPtr->radioId = CWProtocolRetrieve8(msgPtr);
	valPtr->wlanId = CWProtocolRetrieve8(msgPtr);
	vlanTag = CWProtocolRetrieve32(msgPtr);
	valPtr->valueOf8021qTypeId = vlanTag>>16;
	valPtr->userPriority = (vlanTag&0xe000)>>13;
	valPtr->canonicalFormat = (vlanTag&0x1000)>>12;
	valPtr->vlanId = vlanTag&0xfff;
//		valPtr->radioId, valPtr->wlanId, vlanTag, valPtr->valueOf8021qTypeId, valPtr->userPriority,
//		valPtr->canonicalFormat, valPtr->vlanId);
	
	CWParseMessageElementEnd();
}
CWBool CWParseWapi(CWProtocolMessage *msgPtr, int len, WapiValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->cerAsType = CWProtocolRetrieve16(msgPtr);
	valPtr->cerAsLen = CWProtocolRetrieve16(msgPtr);
	valPtr->cerAsValue = CWProtocolRetrieveStr(msgPtr, valPtr->cerAsLen);
	valPtr->cerAeType = CWProtocolRetrieve16(msgPtr);
	valPtr->cerAeLen = CWProtocolRetrieve16(msgPtr);
	valPtr->cerAeValue = CWProtocolRetrieveStr(msgPtr, valPtr->cerAeLen);
	valPtr->asType = CWProtocolRetrieve16(msgPtr);
	valPtr->asLen = CWProtocolRetrieve16(msgPtr);
	valPtr->asValue = CWProtocolRetrieveStr(msgPtr, valPtr->asLen);

//		valPtr->cerAsType, valPtr->cerAsLen, valPtr->cerAsValue, valPtr->cerAeType, valPtr->cerAeLen, valPtr->cerAeValue, 
//		valPtr->asType, valPtr->asLen, valPtr->asValue);
	
	CWParseMessageElementEnd();
}
/*CWBool CWParseDeleteWlan(CWProtocolMessage *msgPtr, int len, AddWlanValues *valPtr)*/
CWBool CWParseDeleteWlan(CWProtocolMessage *msgPtr, int len, DeleteWlanValues *valPtr)

{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);
	CWParseMessageElementEnd();
}
CWBool CWParseUpdateWlan(CWProtocolMessage *msgPtr, int len, UpdateWlanValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);
	valPtr->capabilities = CWProtocolRetrieve16(msgPtr);
	valPtr->key_index = CWProtocolRetrieve8(msgPtr);
	valPtr->key_status = CWProtocolRetrieve8(msgPtr);
	valPtr->key_length = CWProtocolRetrieve16(msgPtr);
	valPtr->key = CWProtocolRetrieveStr(msgPtr,valPtr->key_length);

	CWParseMessageElementEnd();
}

CWBool CWParseAddSta(CWProtocolMessage *msgPtr, int len, AddSTAValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->mac_length = CWProtocolRetrieve8(msgPtr);
	valPtr->mac_addr =  (unsigned char *)CWProtocolRetrieveStr(msgPtr,(int)valPtr->mac_length);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);
	CWParseMessageElementEnd();	
}
CWBool CWParseDeleteSta(CWProtocolMessage *msgPtr, int len ,DeleteSTAValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->mac_length = CWProtocolRetrieve8(msgPtr);
	valPtr->mac_addr =  (unsigned char *)CWProtocolRetrieveStr(msgPtr,(int)valPtr->mac_length);
	valPtr->wlan_id = CWProtocolRetrieve8(msgPtr);  //pei add 0306
	CWParseMessageElementEnd();	

}
CWBool CWParseSTASessionKey(CWProtocolMessage *msgPtr, int len, STASessionKeyValues *valPtr)
{
	int key_idx;
//	int i;/*for test*/
	long unsigned int key_len;
//	CWCaptrue(len,msgPtr->msg);
	CWParseMessageElementStart();
	valPtr->mac = (unsigned char *)CWProtocolRetrieveStr(msgPtr,MAC_ADDR_LEN);/*6 byte*/
/*	printf("valPtr->mac = ");
	for(i=0;i<6;i++)
	{
		printf("%02x ",valPtr->mac[i]);
	}
	printf("\n");
*/
	valPtr->flags = CWProtocolRetrieve16(msgPtr);
	key_idx = ((valPtr->flags>>12)&~(0xfffC));
	key_len = (long unsigned int)((valPtr->flags>>4)&~(0xff00));
	valPtr->Pairwise_TSC = (unsigned char *)CWProtocolRetrieveStr(msgPtr,MAC_ADDR_LEN);/*6 byte*/
	valPtr->Pairwise_RSC = (unsigned char *)CWProtocolRetrieveStr(msgPtr,MAC_ADDR_LEN);/*6 byte*/
	valPtr->key =  (unsigned char *)CWProtocolRetrieveStr(msgPtr,(int)key_len);
/*	printf("key:\n");
	for(i=0;i<key_len;i++){
		printf("%02x",valPtr->key[i]);
	}
*/
	CWParseMessageElementEnd();
	
}

CWBool CWParseRateSet2(CWProtocolMessage *msgPtr, int len ,BindingRate *valPtr)	
{
	int i=0;
	unsigned char length;
	int count;
	CWParseMessageElementStart();
	valPtr->radioID = CWProtocolRetrieve8(msgPtr);
	length = CWProtocolRetrieve8(msgPtr);
	count = length/2;
	valPtr->rate = CWProtocolRetrieve16(msgPtr);

	memset(&gConfigureStateRate[valPtr->radioID],0,sizeof(CWConfigureStateRate));
	CW_CREATE_OBJECT_SIZE_ERR(gConfigureStateRate[valPtr->radioID].rate, count *sizeof(unsigned short), CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	memset(gConfigureStateRate[valPtr->radioID].rate,0,count);
	gConfigureStateRate[valPtr->radioID].count = count;
	
	gConfigureStateRate[valPtr->radioID].rate[0] = valPtr->rate;
	for(i=1;i<count;i++)
	{	
		gConfigureStateRate[valPtr->radioID].rate[i] = CWProtocolRetrieve16(msgPtr);
	}
	CWParseMessageElementEnd();

}
#if (defined(LTEFI)||defined(LTEFI_V2))
CWBool CWGetRateMask(unsigned short *rate, int count,int radioid)
{
	int i=0,m=0;
	int smallestrate = 0;
	unsigned short  support_rate[12]={10,20,55,110,60,90,120,180,240,360,480,540}; //add by caidh
	unsigned short  support_rate_a[8]={60,90,120,180,240,360,480,540}; 
	unsigned int ratemask=0, rateoffset = 0,ratenum=0;
	unsigned int basic_rate_offset=0,basic_rate_offset_skip_g= 0,basic_rate_offset_skip_b=0,basic_rate_offset_skip_a=0;//modify by caidh
	unsigned int basic_rate_mask_b = 0,basic_rate_mask_gn=0,basic_rate_mask_a=0;//modify by caidh
	unsigned int radiotype = 0;
	unsigned short *ptr=NULL;
	radiotype = gRadioInfoValue[radioid].radiotype;
	/*Begin:add by caidh for rewriting set_rate_function 2012-12-04 PLCAP11*/
	basic_rate_offset_skip_g=12;
	basic_rate_offset_skip_b=4;
	basic_rate_offset_skip_a=8;//add by caidh basic rate need to move 12bit
	ratenum=12;
	basic_rate_mask_b=0xF;
	basic_rate_mask_gn=0x150;
	basic_rate_mask_a=0x15;
	if(radiotype&IEEE80211_PARAM_MODE_11a){
		ptr = support_rate_a;
		ratenum=8;//zengmin add:out-of-bounds read by Coverity 2013-06-09
	}else{
		ptr = support_rate;
	}
	/*End:add by caidh for rewriting set_rate_function 2012-12-04 PLCAP11*/
	for(i=0;i<count;i++)
	{	
		smallestrate= rate[i];
		if(smallestrate > rate[i]) 
		{
			smallestrate=rate[i]; 	
		}
	}
	for(i=0;i<count;i++)
	{
		m = 0;
		if (debug_print) autelan_printf("%d,",rate[i]);
		while(m<ratenum)
		{
			rateoffset = 0;
			if(rate[i] == ptr[m]) //found modify by caidh
			{
				rateoffset = (rateoffset  | 0x01)<< m; //each rate offset
			}
			ratemask = ratemask |rateoffset;
			if(rate[i] == ptr[m]){ //modify by caidh
				basic_rate_offset = m;//basicoffset,the smallest one rate  as a basic rate
				break;//add by caidh 
			}
			m++;
		}
	}
	if (debug_print)  autelan_printf("\n");
	/*Begin:modify by caidh for rewriting set_rate_function 2012-12-04 PLCAP11*/
	if(radiotype&IEEE80211_PARAM_MODE_11b){
		if(ratemask&basic_rate_mask_b){
			if((radiotype&IEEE80211_PARAM_MODE_11g) ==0){
			ratemask = ratemask | ((ratemask & basic_rate_mask_b)  << basic_rate_offset_skip_b);
			}else
			ratemask = ratemask | ((ratemask & basic_rate_mask_b)  << basic_rate_offset_skip_g);
		}
		else
			ratemask = ratemask | (0x01<<(basic_rate_offset+ basic_rate_offset_skip_g));
	}
	else if(radiotype&IEEE80211_PARAM_MODE_11g){
		if(ratemask&basic_rate_mask_gn)
			ratemask = ratemask | ((ratemask & basic_rate_mask_gn)  << basic_rate_offset_skip_g);
		else
			ratemask = ratemask | (0x01<<(basic_rate_offset+ basic_rate_offset_skip_g));
	}
	else if(radiotype&IEEE80211_PARAM_MODE_11a){
		if(ratemask&basic_rate_mask_a)
			ratemask = ratemask | ((ratemask & basic_rate_mask_a)<< basic_rate_offset_skip_a);
		else
			ratemask = ratemask | (0x01<<(basic_rate_offset+ basic_rate_offset_skip_a));
	}
	/*end:modify by caidh for rewriting set_rate_function 2012-12-04 PLCAP11*/
	CWWTPDebugLog("radio %d,rate mask:0x%x radiotype:%d",radioid,ratemask,radiotype);
	gRadioInfoValue[radioid].ratemask = ratemask;
	return CW_TRUE;

}
#else
/*Begin:ligh modify the ratemask function on 2013.10.15.
**Sometimes,the rate sets may don't match the radiotype,then ratemask could be invalid.
**So I add some filters on the beginning,when an invalid
**ratemask is set,change it to the default.
****************************************/
CWBool CWGetRateMask(unsigned short *rate, int count,int radioid)
{
    unsigned short support_rate_a[]={60,90,120,180,240,360,480,540};
    unsigned short support_rate_g[]={10,20,55,110,60,90,120,180,240,360,480,540};
    unsigned short support_rate_b[]={10,20,55,110};
    int basic_rate_offset=-1;
    unsigned int ratemask=0;
    const unsigned int basic_rate_offset_skip_g=12;
    const unsigned int basic_rate_offset_skip_a=8;
    const unsigned int basic_rate_offset_skip_b=4;
    const unsigned int basic_rate_mask_b=0xF;
	const unsigned int basic_rate_mask_gn=0x150;
	const unsigned int basic_rate_mask_a=0x15;
    unsigned int radiotype=0,radio_type_a=0,radio_type_b=0,radio_type_g=0;
    int i,m,len;
    unsigned short *ptr=NULL;
    
    radiotype = gRadioInfoValue[radioid].radiotype;
    radio_type_a = radiotype&IEEE80211_PARAM_MODE_11a;
    radio_type_b = radiotype&IEEE80211_PARAM_MODE_11b;
    radio_type_g = radiotype&IEEE80211_PARAM_MODE_11g;
    
    if(radio_type_g)
    {
        ptr=support_rate_g;
        len=sizeof(support_rate_g)/sizeof(unsigned short);
    }else if(radio_type_b)
    {
        ptr=support_rate_b;
        len=sizeof(support_rate_b)/sizeof(unsigned short);
    }else
    {
        ptr=support_rate_a;
        len=sizeof(support_rate_a)/sizeof(unsigned short);
    }

    /*set the bit of support rates*/
    for(i=0;i<count;i++)
    {
        if(debug_print)
            CWWTPDebugLog("rate is %d\n",rate[i]);
        for(m=0;m<len;m++)
        {
            if(rate[i]==ptr[m])
            {
                ratemask=ratemask | (0x01<<m);
                basic_rate_offset=m;
                break;
            }
        }
    }
    CWWTPDebugLog("rate num is %d,ratemask of support_rate is 0x%x,basic_rate_offset is %d,radiotype is %d\n",
                count,ratemask,basic_rate_offset,radiotype);

    /*rate doesn't exist or (1,2,5.5,11) is set with mode 11g.*/
    if(basic_rate_offset==-1 || (radio_type_g && !radio_type_b && basic_rate_offset<4))
    {
        ratemask=(radio_type_b && radio_type_g)?0x00ffff:(radio_type_b)?0xff:(radio_type_a)?0x15ff:0x150ff0;
        CWWTPDebugLog("Invalid ratemask,reset as 0x%x\n",ratemask);
        gRadioInfoValue[radioid].ratemask = ratemask;
        return CW_TRUE;
    }

    /*set the bit of basic rates*/
    if(radio_type_b){
		if(ratemask&basic_rate_mask_b){
			if(!radio_type_g)
			    ratemask = ratemask | ((ratemask & basic_rate_mask_b)  << basic_rate_offset_skip_b);
            else
			    ratemask = ratemask | ((ratemask & basic_rate_mask_b)  << basic_rate_offset_skip_g);
		}
		else
			ratemask = ratemask | (0x01<<(basic_rate_offset+ basic_rate_offset_skip_g));
	}
    else if(radio_type_g){
		if(ratemask&basic_rate_mask_gn)
			ratemask = ratemask | ((ratemask & basic_rate_mask_gn)  << basic_rate_offset_skip_g);
		else
			ratemask = ratemask | (0x01<<(basic_rate_offset+ basic_rate_offset_skip_g));
	}
	else if(radio_type_a){
		if(ratemask&basic_rate_mask_a)
			ratemask = ratemask | ((ratemask & basic_rate_mask_a)<< basic_rate_offset_skip_a);
		else
			ratemask = ratemask | (0x01<<(basic_rate_offset+ basic_rate_offset_skip_a));
	}

	CWWTPDebugLog("radio %d,rate mask:0x%x radiotype:%d",radioid,ratemask,radiotype);
	gRadioInfoValue[radioid].ratemask = ratemask;

    return CW_TRUE;
}
/*End:ligh modify the function on 2013.10.15*/
#endif
CWBool CWParseRateSet(CWProtocolMessage *msgPtr, int len ,BindingRate *valPtr)				//pei add 0714
{
	int i=0/*,m=0*/;
	unsigned char length;
	unsigned short *rate=NULL;
	int count;
	CWParseMessageElementStart();
	valPtr->radioID = CWProtocolRetrieve8(msgPtr);
	length = CWProtocolRetrieve8(msgPtr);
	count = length/2;
	valPtr->rate = CWProtocolRetrieve16(msgPtr);
	CW_CREATE_ARRAY_ERR(rate, count*sizeof(unsigned short), short int, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	rate[0] = valPtr->rate;
	for(i=1;i<count;i++)
	{	
		rate[i] = CWProtocolRetrieve16(msgPtr);
		if(valPtr->rate > rate[i]) 
		{
			valPtr->rate=rate[i]; 	
		}
	}
	if(length == 2)
	{
		gRadioInfoValue[valPtr->radioID].rateAutoEnable = CW_FALSE;
	}
	else if((gRadioInfoValue[valPtr->radioID].radiotype==5)&&(rate[count-1]>110))
	{
		gRadioInfoValue[valPtr->radioID].rateAutoEnable = CW_FALSE;
	}
	else
	{
		gRadioInfoValue[valPtr->radioID].rateAutoEnable = CW_TRUE;
	}
	
	CWGetRateMask(rate, count, valPtr->radioID);
	CW_FREE_OBJECT(rate);
	CWParseMessageElementEnd();

}
CWBool CWParseRadioTypeSet(CWProtocolMessage *msgPtr, int len ,BindingRadioType *valPtr)	//pei add 0714
{
	CWParseMessageElementStart();
	valPtr->radioID = CWProtocolRetrieve8(msgPtr);
	valPtr->radiotype = CWProtocolRetrieve32(msgPtr);
//	printf("valPtr->radioID = %d valPtr->radiotype = %d\n", valPtr->radioID, valPtr->radiotype);
	CWParseMessageElementEnd();	

}
CWBool CWParseMacOperation(CWProtocolMessage *msgPtr, int len, MacOperationValues *valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->reserved = CWProtocolRetrieve8(msgPtr);
	valPtr->rts_threshold = CWProtocolRetrieve16(msgPtr);
	valPtr->short_retry = CWProtocolRetrieve8(msgPtr);
	valPtr->long_retry = CWProtocolRetrieve8(msgPtr);
	valPtr->fragmentation_threshold = CWProtocolRetrieve16(msgPtr);
	valPtr->tx_msdu_lifetime = CWProtocolRetrieve32(msgPtr);
	valPtr->rx_msdu_lifetime = CWProtocolRetrieve32(msgPtr);
//	printf("valPtr->radio_id = %d valPtr->fragmentation_threshold = %d\n", valPtr->radio_id, valPtr->fragmentation_threshold);
	CWParseMessageElementEnd();

}
CWBool CWParseWTPRadioConfiguration(CWProtocolMessage *msgPtr, int len ,WTPRadioConfigurationValues *valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->is_short_preamble = CWProtocolRetrieve8(msgPtr);
	valPtr->num_of_bssids = CWProtocolRetrieve8(msgPtr);
	valPtr->dtim_period = CWProtocolRetrieve8(msgPtr);
	
	void *ptr = NULL;
	ptr =  (unsigned char *)CWProtocolRetrieveRawBytes(msgPtr,6);
	CW_COPY_MEMORY(valPtr->bssid, ptr, 6);
	CW_FREE_OBJECT(ptr);
	valPtr->beacon_period = CWProtocolRetrieve16(msgPtr);
	memset(valPtr->country_code, 0, 4);
	
	ptr = NULL;
	ptr =  (unsigned char *)CWProtocolRetrieveRawBytes(msgPtr,4);
	CW_COPY_MEMORY(valPtr->country_code, ptr, 4);
	CW_FREE_OBJECT(ptr);
	CWWTPDebugLog("***wifi%d country_code:[%s]", valPtr->radio_id,valPtr->country_code);

	CWParseMessageElementEnd();

}
CWBool CWParseNeighborDeadInterval(CWProtocolMessage *msgPtr, int len, CWBindingConfigurationUpdateRequestValues *valPtr)	//pei add 0714
{
	CWParseMessageElementStart();
	valPtr->neighborDeadInterval = CWProtocolRetrieve32(msgPtr);
	CWWTPDebugLog("neighborDeadInterval:[%d]", valPtr->neighborDeadInterval);
	CWParseMessageElementEnd();	
}
CWBool acw_parse_ampdu_configuration(CWProtocolMessage *msgPtr, int len ,ampdu_op_type *valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->op_type= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id= CWProtocolRetrieve8(msgPtr);
	valPtr->state= CWProtocolRetrieve8(msgPtr);
	valPtr->ampduframes= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 2;  //jump reseved
	valPtr->ampdulimit= CWProtocolRetrieve32(msgPtr);
	CWWTPDebugLog("ampdu ( radio_id=%d wlan_id=%d state=%d ampdulimit=%d  ampduframes=%d)",valPtr->radio_id,valPtr->wlan_id,valPtr->state,valPtr->ampdulimit,valPtr->ampduframes);
	CWParseMessageElementEnd();
}
/*yuan add for amsdu*/
CWBool acw_parse_amsdu_configuration(CWProtocolMessage *msgPtr, int len ,amsdu_op_type *valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->op_type= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id= CWProtocolRetrieve8(msgPtr);
	valPtr->state= CWProtocolRetrieve8(msgPtr);
	valPtr->amsduframes= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 2;  //jump reseved
	valPtr->amsdulimit= CWProtocolRetrieve32(msgPtr);
	CWWTPDebugLog("amsdu (radio_id=%d wlan_id=%d state=%d amsdulimit=%d amsduframes=%d) \n",valPtr->radio_id,valPtr->wlan_id,valPtr->state,valPtr->amsdulimit,valPtr->amsduframes);
	CWParseMessageElementEnd();
}
CWBool acw_parse_puren_mixed_op_configuration(CWProtocolMessage *msgPtr, int len, puren_mixed_op_type *valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->op_type= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id= CWProtocolRetrieve8(msgPtr);
	valPtr->state= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 3; //jump reseved
	CWWTPDebugLog("puren_mixed_op ( valPtr->op_type = %d valPtr->radio_id = %d valPtr->wlan_id=%d valPtr->state= %d )",valPtr->op_type,valPtr->radio_id,valPtr->wlan_id,valPtr->state);
	CWParseMessageElementEnd();
}
CWBool acw_dhcp_snooping_configuration(CWProtocolMessage *msgPtr, int len ,dhcp_snooping_type *valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->op_type= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id= CWProtocolRetrieve8(msgPtr);
	valPtr->state= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 3;  //jump reseved
	CWWTPDebugLog("dhcp_snooping ( valPtr->op_type = %d valPtr->radio_id = %d valPtr->wlan_id=%d valPtr->state= %d )",valPtr->op_type,valPtr->radio_id,valPtr->wlan_id,valPtr->state);
	CWParseMessageElementEnd();
}
CWBool acw_sta_info_report_configuration(CWProtocolMessage *msgPtr, int len ,sta_info_repport_type *valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->op_type= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id= CWProtocolRetrieve8(msgPtr);
	valPtr->state= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 3;  //jump reseved
	CWWTPDebugLog("sta_info_report (valPtr->op_type = %d valPtr->radio_id = %d valPtr->wlan_id=%d valPtr->state= %d )",valPtr->op_type,valPtr->radio_id,valPtr->wlan_id,valPtr->state);
	CWParseMessageElementEnd();
}
CWBool acw_parse_channel_extoffset_configuration(CWProtocolMessage *msgPtr, int len ,channel_extoffset_op_type*valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->op_type= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id= CWProtocolRetrieve8(msgPtr);
	char extoffset = CWProtocolRetrieve8(msgPtr);
	valPtr->state = extoffset;
#ifdef APV6
	autelan_printf("------------aqtype: APV6\n");
	if(extoffset == 255)
		valPtr->state = -1;
#endif
	valPtr->current_mode= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 2;  //jump reseved
	CWWTPDebugLog("channel_extoffset ( valPtr->op_type = %d valPtr->radio_id = %d valPtr->wlan_id=%d valPtr->state= %d  valPtr->current_mode= %d )",valPtr->op_type,valPtr->radio_id,valPtr->wlan_id,valPtr->state,valPtr->current_mode);
	CWParseMessageElementEnd();
}
CWBool acw_parse_tx_chainmask_configuration(CWProtocolMessage *msgPtr, int len ,tx_chainmask_op_type*valPtr)	//pei add 0722
{
	CWParseMessageElementStart();
	valPtr->op_type= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id= CWProtocolRetrieve8(msgPtr);
	valPtr->state= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 3;  //jump reseved
	CWWTPDebugLog("tx_chainmask ( valPtr->op_type = %d valPtr->radio_id = %d valPtr->wlan_id=%d valPtr->state= %d  )",valPtr->op_type,valPtr->radio_id,valPtr->wlan_id,valPtr->state);
	CWParseMessageElementEnd();
}
CWBool acw_parse_rx_chainmask_configuration(CWProtocolMessage *msgPtr, int len ,rx_chainmask_op_type*valPtr)	
{
	CWParseMessageElementStart();
	valPtr->op_type= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->wlan_id= CWProtocolRetrieve8(msgPtr);
	valPtr->state= CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 3;  //jump reseved
	CWWTPDebugLog("rx_chainmask ( valPtr->op_type = %d valPtr->radio_id = %d valPtr->wlan_id=%d valPtr->state= %d  )",valPtr->op_type,valPtr->radio_id,valPtr->wlan_id,valPtr->state);
	CWParseMessageElementEnd();
}
CWBool acw_parse_ack_timeout_configuration(CWProtocolMessage *msgPtr, int len ,ack_timeout_op_type*valPtr)	
{
	CWParseMessageElementStart();
	valPtr->op_type = CWProtocolRetrieve8(msgPtr);
	(msgPtr->offset) += 1; //jump wtp_radio_set_type 
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->state = CWProtocolRetrieve8(msgPtr);
	valPtr->distance = CWProtocolRetrieve32(msgPtr);
	CWWTPDebugLog("ack_timeout  valPtr->radio_id = %d valPtr->state = %d valPtr->distance = %d ",valPtr->radio_id,valPtr->state,valPtr->distance);
	CWParseMessageElementEnd();
}

CWBool CWBindingSaveConfigurationUpdateRequest(void* bindingValuesPtr, CWProtocolResultCode* resultCode)
{
	char cmdbuf[128];
	int thinapCheckThreshold = 0;
	unsigned char radioID = 10;		// default 10,no special meaning
	if(bindingValuesPtr==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	CWBindingConfigurationUpdateRequestValues *bindingPtr=(CWBindingConfigurationUpdateRequestValues*)bindingValuesPtr; 

	if(bindingPtr->neighborDeadInterval !=0)
	{
		gCWNeighborDeadInterval = bindingPtr->neighborDeadInterval;
		thinapCheckThreshold = gCWNeighborDeadInterval/gEchoInterval+6;
		memset(cmdbuf, 0, 128);
		autelan_sprintf(cmdbuf, "echo %d > /proc/sys/dev/wifi0/thinap_check_threshold", thinapCheckThreshold);
		_system(cmdbuf);
		
		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		strcpy(confArg.conf_arg,"gCWNeighborDeadInterval");
		autelan_sprintf(confArg.conf_value,"%d",gCWNeighborDeadInterval);
		CWSaveConfigInfo(CONF_WTPD,&confArg,CW_TRUE);
	}
	if (bindingPtr->qosCount>0) {
		if(!CWBindingSetQosValues1(bindingPtr->qosCount, bindingPtr->radioQosValues, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->radioQosValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->radioQosValues->radioID;
		CW_FREE_OBJECT(bindingPtr->radioQosValues);
	}
	if(bindingPtr->directSequeueControlCount>0){
		if(!CWBindingSetDirectSequenceControlValues(bindingPtr->directSequeueControlValues, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->directSequeueControlValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->directSequeueControlValues->radio_id;
		CW_FREE_OBJECT(bindingPtr->directSequeueControlValues);
	}
	if (bindingPtr->rateCount>0)
	{
		
		if(!CWBindingSetRate(bindingPtr->rateSetValues, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->rateSetValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->rateSetValues->radioID;
		CW_FREE_OBJECT(bindingPtr->rateSetValues);
	}
	if (bindingPtr->radioTypeCount>0)
	{

		if(!CWBindingSetRadioType(bindingPtr->radioTypeSetValues, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->radioTypeSetValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->radioTypeSetValues->radioID;
		CW_FREE_OBJECT(bindingPtr->radioTypeSetValues);
	}
	if (bindingPtr->macOperationCount>0)
	{
		
		if(!CWBindingSetMacOperation(bindingPtr->macOperation, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->macOperation);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->macOperation->radio_id;
		CW_FREE_OBJECT(bindingPtr->macOperation);
	}
	if (bindingPtr->radioConfigCount>0)
	{

		if(!CWBindingSetRadioConfig(bindingPtr->radioConfiguration, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->radioConfiguration);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->radioConfiguration->radio_id;
		CW_FREE_OBJECT(bindingPtr->radioConfiguration);
	}
       if(bindingPtr->ampdu_op_count>0){	
		if(!acw_binding_set_ampdu(bindingPtr->ampdu_op_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->ampdu_op_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->ampdu_op_values->radio_id;
		CW_FREE_OBJECT(bindingPtr->ampdu_op_values);
	}
	 /* yuan add*/ 
       if(bindingPtr->amsdu_op_count>0){	
		if(!acw_binding_set_amsdu(bindingPtr->amsdu_op_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->amsdu_op_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->amsdu_op_values->radio_id;
		CW_FREE_OBJECT(bindingPtr->amsdu_op_values);
	}
       if(bindingPtr->puren_mixed_op_count>0){	
		if(!acw_binding_set_puren_mixed(bindingPtr->puren_mixed_op_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->puren_mixed_op_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->puren_mixed_op_values->radio_id;
		CW_FREE_OBJECT(bindingPtr->puren_mixed_op_values);
	}
       if(bindingPtr->dhcp_snooping_count>0){	
		if(!acw_binding_set_dhcp_snooping(bindingPtr->dhcp_snooping_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->dhcp_snooping_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->dhcp_snooping_values->radio_id;
		CW_FREE_OBJECT(bindingPtr->dhcp_snooping_values);
	}
/*       if(bindingPtr->sta_info_repport_count>0){	
		if(!acw_binding_set_sta_info_report(bindingPtr->sta_info_repport_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->sta_info_repport_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(bindingPtr->sta_info_repport_values);
	}*/
       if(bindingPtr->channel_extoffset_op_count>0){	
		if(!acw_binding_set_channel_extoffset(bindingPtr->channel_extoffset_op_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->channel_extoffset_op_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->channel_extoffset_op_values->radio_id;
		CW_FREE_OBJECT(bindingPtr->channel_extoffset_op_values);
	}
       if(bindingPtr->tx_chainmask_op_count>0){	
		if(!acw_binding_set_tx_chainmask(bindingPtr->tx_chainmask_op_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->tx_chainmask_op_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->tx_chainmask_op_values->radio_id;
		CW_FREE_OBJECT(bindingPtr->tx_chainmask_op_values);
	}
       if(bindingPtr->rx_chainmask_op_count>0){	
		if(!acw_binding_set_rx_chainmask(bindingPtr->rx_chainmask_op_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->rx_chainmask_op_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->rx_chainmask_op_values->radio_id;
		CW_FREE_OBJECT(bindingPtr->rx_chainmask_op_values);
	}
	   if(bindingPtr->ack_timeout_op_count>0){	
		if(!acw_binding_set_ack_timeout(bindingPtr->ack_timeout_op_values, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->ack_timeout_op_values);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->ack_timeout_op_values->radio_id;
		CW_FREE_OBJECT(bindingPtr->ack_timeout_op_values);
	}
	if(bindingPtr->txPowerCount>0){	
		if(!CWBindingSetTxPower(bindingPtr->txPowerValues, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->txPowerValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		radioID = bindingPtr->txPowerValues->radio_id;
		CW_FREE_OBJECT(bindingPtr->txPowerValues);
	}
	//save the updowntime
	if(radioID != 10)
	{
		CWWTPWlan *ptr = NULL;
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next)
		{
			if(ptr->radio_id == radioID)
			{
				unsigned char wlan_updown_time = 0;
				wlan_updown_time = ptr->wlan_updown_time;  
				
				ConfArg confArg;
				memset(&confArg,0,sizeof(ConfArg));
				confArg.radioID = ptr->radio_id;
				confArg.wlanID	= ptr->wlan_id;
				strcpy(confArg.conf_arg,"wlan_updown_time");
				autelan_sprintf(confArg.conf_value,"%d",wlan_updown_time);
				CWSaveConfigInfo(CONF_WLAN,&confArg,CW_TRUE);
		
			}
		}
	}	 
	CW_FREE_OBJECT(bindingPtr);

	return CW_TRUE;
}

CWBool CWBindingParseConfigurationUpdateRequest (char *msg, int len, void **valuesPtr) 
{
	int i,j,k,l,m,n,t,dhcp_snooping_count,sta_info_repport_count,ampdu_op_count,puren_mixed_op_count,tx_chainmask_op_count,channel_extoffset_op_count,amsdu_op_count,rx_chainmask_op_count,ack_timeout_op_count;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	unsigned char value_in_case;
	unsigned char value_in_case_type;
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	CWBindingConfigurationUpdateRequestValues *auxBindingPtr;
	CW_CREATE_OBJECT_ERR(auxBindingPtr, CWBindingConfigurationUpdateRequestValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
	*valuesPtr = (void *)auxBindingPtr;
	/*init CWProtocolConfigurationUpdateRequestValues->bindingValues*/
	auxBindingPtr->neighborDeadInterval=0;
	auxBindingPtr->qosCount = 0;
	auxBindingPtr->directSequeueControlCount =0;
	auxBindingPtr->txPowerCount =0;
	auxBindingPtr->rateCount = 0;
	auxBindingPtr->radioTypeCount = 0;
	auxBindingPtr->macOperationCount = 0;
	auxBindingPtr->radioConfigCount = 0;
	auxBindingPtr->adminStateCount=0;
	auxBindingPtr->dhcp_snooping_count=0;
	auxBindingPtr->sta_info_repport_count=0;
	auxBindingPtr->ampdu_op_count=0;
	auxBindingPtr->amsdu_op_count=0;/*yuan add ...*/
	auxBindingPtr->puren_mixed_op_count=0;
	auxBindingPtr->channel_extoffset_op_count=0;
	auxBindingPtr->tx_chainmask_op_count=0;
	auxBindingPtr->rx_chainmask_op_count=0;
	auxBindingPtr->ack_timeout_op_count=0;
	auxBindingPtr->radioQosValues=NULL;	
	auxBindingPtr->directSequeueControlValues = NULL;	
	auxBindingPtr->txPowerValues = NULL;	
	auxBindingPtr->rateSetValues = NULL;	
	auxBindingPtr->radioTypeSetValues = NULL;	
	auxBindingPtr->macOperation = NULL;	
	auxBindingPtr->radioConfiguration = NULL;
	auxBindingPtr->adminState=NULL;
	auxBindingPtr->dhcp_snooping_values= NULL;
	auxBindingPtr->sta_info_repport_values= NULL;
	auxBindingPtr->ampdu_op_values= NULL;
	auxBindingPtr->amsdu_op_values= NULL;/*yuan add ...*/
	auxBindingPtr->puren_mixed_op_values= NULL;
	auxBindingPtr->channel_extoffset_op_values= NULL;
	auxBindingPtr->tx_chainmask_op_values= NULL;
	auxBindingPtr->rx_chainmask_op_values= NULL;
	auxBindingPtr->ack_timeout_op_values= NULL;

	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		switch(elemType) {
			case BINDING_MSG_ELEMENT_TYPE_WTP_QOS:
				(auxBindingPtr->qosCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_DIRECT_SEQUEUE_CONTROL:
				(auxBindingPtr->directSequeueControlCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_TX_POWER:
				(auxBindingPtr->txPowerCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RATE_SET:
				(auxBindingPtr->rateCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RADIO_TYPE_SET:
				(auxBindingPtr->radioTypeCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_MAC_OPERATE:
				(auxBindingPtr->macOperationCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_WTP_RADIO_CONFIGURATON:
				(auxBindingPtr->radioConfigCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_NEIGHBORDEAD_INTERVAL:
				if(!(CWParseNeighborDeadInterval(&completeMsg, elemLen, auxBindingPtr)))
				{
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				break;
			case BINDING_MSG_ELEMENT_TYPE_WTP_RADIO_SET:
				completeMsg.offset+=1;
				value_in_case = CWProtocolRetrieve8(&completeMsg);
				switch(value_in_case){
					case DHCP_SNOOPING:
						(completeMsg.offset)-=2;
						(auxBindingPtr->dhcp_snooping_count)++;
					break;
					case STA_INFO_REPORT:
						(completeMsg.offset)-=2;
						(auxBindingPtr->sta_info_repport_count)++;
					break;
					case AMPDU_OP:
						(completeMsg.offset)-=2;
						(auxBindingPtr->ampdu_op_count)++;
					break;
					/*yuan add ...*/
					case AMSDU_OP:
						(completeMsg.offset)-=2;
						(auxBindingPtr->amsdu_op_count)++;
					break;
					case PUREN_MIXED_OP:
						(completeMsg.offset)-=2;
						(auxBindingPtr->puren_mixed_op_count)++;
					break;
					case CHANNEL_EXTOFFSET:
						(completeMsg.offset)-=2;
						(auxBindingPtr->channel_extoffset_op_count)++;
					break;
					case TX_CHAINMASK:
						(completeMsg.offset)-=2;
						(auxBindingPtr->tx_chainmask_op_count)++;
					break;
					case RX_CHAINMASK:
						(completeMsg.offset)-=2;
						(auxBindingPtr->rx_chainmask_op_count)++;
					break;
					case ACK_TIMEOUT:
						(completeMsg.offset)-=2;
						(auxBindingPtr->ack_timeout_op_count)++;
					break;
					default:
						(completeMsg.offset)-=2;
						CWWTPDebugLog("Unrecognized  wtp_radio_set TYPE :%d, in Binging of ConfigUpdate Request", value_in_case);
					break;
				}
				completeMsg.offset += elemLen;
				break;
			default:
				if(CWBindingCheckType(elemType)) 
				{
					CWWTPDebugLog("Unrecognized Binding Message Element:%d, in Binging of ConfigUpdate Request", elemType);
				}
				completeMsg.offset += elemLen;
				break;
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	// actually read each radio info
	CW_CREATE_ARRAY_ERR(auxBindingPtr->radioQosValues, auxBindingPtr->qosCount, Radio_QosValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->directSequeueControlValues,auxBindingPtr->directSequeueControlCount,DirectSequeueControlValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->txPowerValues, auxBindingPtr->txPowerCount, TxPowerValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CW_CREATE_ARRAY_ERR(auxBindingPtr->rateSetValues, auxBindingPtr->rateCount, BindingRate, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->radioTypeSetValues, auxBindingPtr->radioTypeCount, BindingRadioType, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->macOperation, auxBindingPtr->macOperationCount, MacOperationValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->radioConfiguration, auxBindingPtr->radioConfigCount, WTPRadioConfigurationValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->ampdu_op_values,auxBindingPtr->ampdu_op_count, ampdu_op_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->amsdu_op_values,auxBindingPtr->amsdu_op_count, amsdu_op_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););/*yuan add ...*/
	CW_CREATE_ARRAY_ERR(auxBindingPtr->puren_mixed_op_values,auxBindingPtr->puren_mixed_op_count, puren_mixed_op_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->dhcp_snooping_values,auxBindingPtr->dhcp_snooping_count, dhcp_snooping_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->sta_info_repport_values,auxBindingPtr->sta_info_repport_count, sta_info_repport_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->channel_extoffset_op_values,auxBindingPtr->channel_extoffset_op_count, channel_extoffset_op_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->tx_chainmask_op_values,auxBindingPtr->tx_chainmask_op_count, tx_chainmask_op_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->rx_chainmask_op_values,auxBindingPtr->rx_chainmask_op_count, rx_chainmask_op_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->ack_timeout_op_values,auxBindingPtr->ack_timeout_op_count, ack_timeout_op_type, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	i=0,j=0,k=0,l=0,m=0,n=0,t=0,dhcp_snooping_count=0,sta_info_repport_count=0,ampdu_op_count=0,puren_mixed_op_count=0,tx_chainmask_op_count=0,channel_extoffset_op_count=0,amsdu_op_count=0,rx_chainmask_op_count=0,ack_timeout_op_count=0;
	completeMsg.offset = 0;
	while(completeMsg.offset < len) {
		unsigned short int type=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&elemLen);		

		switch(type) {
			case BINDING_MSG_ELEMENT_TYPE_WTP_QOS:{
				unsigned char tagPackets;
				if(!(CWParseWTPQoS(&completeMsg, elemLen, &(auxBindingPtr->radioQosValues[i].radioID), &tagPackets, auxBindingPtr->radioQosValues[i].qosValues)))
				{
					CW_FREE_OBJECT(auxBindingPtr->radioQosValues);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				i++;
				break;
			}
			case BINDING_MSG_ELEMENT_TYPE_DIRECT_SEQUEUE_CONTROL:{
				if(!(CWParseDireceSequeueControl(&completeMsg, elemLen,&(auxBindingPtr->directSequeueControlValues[j]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->directSequeueControlValues);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				j++;
				break;
			}
			case BINDING_MSG_ELEMENT_TYPE_TX_POWER:{
				if(!(CWParseTxPower(&completeMsg, elemLen,&(auxBindingPtr->txPowerValues[k]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->txPowerValues);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				k++;
				break;
			}
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RATE_SET:{	     		 //pei add 0715
				 if(!(CWParseRateSet(&completeMsg, elemLen,&(auxBindingPtr->rateSetValues[l]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->rateSetValues);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				l++;
				break;
			}
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RADIO_TYPE_SET:{	//pei add 0715
				 if(!(CWParseRadioTypeSet(&completeMsg, elemLen,&(auxBindingPtr->radioTypeSetValues[m]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->radioTypeSetValues);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				m++;
				break;
			}
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_MAC_OPERATE:{	     //pei add 0722
				 if(!(CWParseMacOperation(&completeMsg, elemLen,&(auxBindingPtr->macOperation[n]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->macOperation);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				n++;
				break;
			}
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_WTP_RADIO_CONFIGURATON:{	//pei add 0722
				 if(!(CWParseWTPRadioConfiguration(&completeMsg, elemLen,&(auxBindingPtr->radioConfiguration[t]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->radioConfiguration);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				t++;
				break;
			}
			case BINDING_MSG_ELEMENT_TYPE_WTP_RADIO_SET:{
				completeMsg.offset+=1;
				value_in_case_type = CWProtocolRetrieve8(&completeMsg);
				switch(value_in_case_type){
					case DHCP_SNOOPING:
						(completeMsg.offset)-=2;
						if(!(acw_dhcp_snooping_configuration(&completeMsg, elemLen,&(auxBindingPtr->dhcp_snooping_values[dhcp_snooping_count])))){
							CW_FREE_OBJECT(auxBindingPtr->dhcp_snooping_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						dhcp_snooping_count++;
					break;
					case STA_INFO_REPORT:
						(completeMsg.offset)-=2;
						if(!(acw_sta_info_report_configuration(&completeMsg, elemLen,&(auxBindingPtr->sta_info_repport_values[sta_info_repport_count])))){
							CW_FREE_OBJECT(auxBindingPtr->sta_info_repport_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						sta_info_repport_count++;
					break;
					case AMPDU_OP:
						(completeMsg.offset)-=2;
						if(!(acw_parse_ampdu_configuration(&completeMsg, elemLen,&(auxBindingPtr->ampdu_op_values[ampdu_op_count])))){
							CW_FREE_OBJECT(auxBindingPtr->ampdu_op_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						ampdu_op_count++;
					break;
					/*yuan add ...*/
					case AMSDU_OP:
						(completeMsg.offset)-=2;
						if(!(acw_parse_amsdu_configuration(&completeMsg, elemLen,&(auxBindingPtr->amsdu_op_values[amsdu_op_count])))){
							CW_FREE_OBJECT(auxBindingPtr->amsdu_op_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						amsdu_op_count++;
					break;
					case PUREN_MIXED_OP:
						(completeMsg.offset)-=2;
						if(!(acw_parse_puren_mixed_op_configuration(&completeMsg, elemLen,&(auxBindingPtr->puren_mixed_op_values[puren_mixed_op_count])))){
							CW_FREE_OBJECT(auxBindingPtr->puren_mixed_op_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						puren_mixed_op_count++;
					break;
					case CHANNEL_EXTOFFSET:
						(completeMsg.offset)-=2;
						if(!(acw_parse_channel_extoffset_configuration(&completeMsg, elemLen,&(auxBindingPtr->channel_extoffset_op_values[channel_extoffset_op_count])))){
							CW_FREE_OBJECT(auxBindingPtr->channel_extoffset_op_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						channel_extoffset_op_count++;
					break;
					case TX_CHAINMASK:
						(completeMsg.offset)-=2;
						if(!(acw_parse_tx_chainmask_configuration(&completeMsg, elemLen,&(auxBindingPtr->tx_chainmask_op_values[tx_chainmask_op_count])))){
							CW_FREE_OBJECT(auxBindingPtr->tx_chainmask_op_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						tx_chainmask_op_count++;
					break;
					case RX_CHAINMASK:
						(completeMsg.offset)-=2;
						if(!(acw_parse_rx_chainmask_configuration(&completeMsg, elemLen,&(auxBindingPtr->rx_chainmask_op_values[rx_chainmask_op_count])))){
							CW_FREE_OBJECT(auxBindingPtr->rx_chainmask_op_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						rx_chainmask_op_count++;
					break;
					case ACK_TIMEOUT:
						(completeMsg.offset)-=2;
						if(!(acw_parse_ack_timeout_configuration(&completeMsg, elemLen,&(auxBindingPtr->ack_timeout_op_values[ack_timeout_op_count])))){
							CW_FREE_OBJECT(auxBindingPtr->ack_timeout_op_values);
							CW_FREE_OBJECT(*valuesPtr);
							return CW_FALSE; // will be handled by the caller
						}
						ack_timeout_op_count++;
					break;
					default:
						(completeMsg.offset)-=2;
					//	CWWTPDebugLog("Unrecognized  wtp_radio_set TYPE :%d, in Binging of ConfigUpdate Request", value_in_case);
						CWWTPDebugLog("Unrecognized  wtp_radio_set TYPE :%d, in Binging of ConfigUpdate Request", value_in_case_type);//zengmin add by Coverity 2013-06-13
						completeMsg.offset += elemLen;
					break;
				}
				break;  
                     }
			default:
				completeMsg.offset += elemLen;
				break;
		}
	}
	
	
	return CW_TRUE;
}

CWBool CWBindingSaveConfigureResponse(void* bindingValuesPtr, CWProtocolResultCode* resultCode)
{	
	if(bindingValuesPtr==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	int i=0,j=0;
	char cmdbuf[128];
	char CountryCode[4]={0};
	CWBindingConfigurationRequestValues* bindingPtr=(CWBindingConfigurationRequestValues*)bindingValuesPtr; 

#if 0
	if (bindingPtr->qosCount>0) 
	{
		if(!CWBindingSetQosValues(bindingPtr->qosCount, bindingPtr->radioQosValues, resultCode))
		{
			CW_FREE_OBJECT(bindingPtr->radioQosValues);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(bindingPtr->radioQosValues);
	}
#else
	if (bindingPtr->qosCount>0) 
	{
		//memcpy(&gRadioQosValues, bindingPtr->radioQosValues, sizeof(Radio_QosValues)*8);
		memcpy(&gRadioQosValues, bindingPtr->radioQosValues, sizeof(Radio_QosValues)); //zengmin modify:out-of-bounds access by Coverity 2013-06-09
		CW_FREE_OBJECT(bindingPtr->radioQosValues);
		CWWTPDebugLog("Save config state QoS:\nradio_id:%d", gRadioQosValues.radioID);
		//printf("Save config state QoS:\nradio_id:%d\n", gRadioQosValues.radioID);
		for(i=0; i<NUM_QOS_PROFILES*2; i++)
		{
			CWWTPDebugLog("WMMObject:%d QosOrder:%d Length:%d QueueDepth:%d CWMin:%d CWMax:%d AIFS:%d TXOPlimit:%d", 
				gRadioQosValues.qosValues[i].WMMObject, gRadioQosValues.qosValues[i].QosOrder, gRadioQosValues.qosValues[i].Length, 
				gRadioQosValues.qosValues[i].QueueDepth, gRadioQosValues.qosValues[i].CWMin, gRadioQosValues.qosValues[i].CWMax, 
				gRadioQosValues.qosValues[i].AIFS, gRadioQosValues.qosValues[i].TXOPlimit);
			//printf("WMMObject:%d QosOrder:%d Length:%d QueueDepth:%d CWMin:%d CWMax:%d AIFS:%d TXOPlimit:%d\n", 
			//	gRadioQosValues.qosValues[i].WMMObject, gRadioQosValues.qosValues[i].QosOrder, gRadioQosValues.qosValues[i].Length, 
			//	gRadioQosValues.qosValues[i].QueueDepth, gRadioQosValues.qosValues[i].CWMin, gRadioQosValues.qosValues[i].CWMax, 
			//	gRadioQosValues.qosValues[i].AIFS, gRadioQosValues.qosValues[i].TXOPlimit);
			if(gRadioQosValues.qosValues[i].WMMObject == 0)
			{
				CWWTPDebugLog("ACK:%d mapstate:%d", gRadioQosValues.qosValues[i].ACK, gRadioQosValues.qosValues[i].mapstate);
				//printf("ACK:%d mapstate:%d\n", gRadioQosValues.qosValues[i].ACK, gRadioQosValues.qosValues[i].mapstate);
				if(gRadioQosValues.qosValues[i].mapstate == 1)
				{
					CWWTPDebugLog("wmm_map_dot1p:%d dot1p_map_wmm_num:%d", gRadioQosValues.qosValues[i].wmm_map_dot1p, 
						gRadioQosValues.qosValues[i].dot1p_map_wmm_num);
					//printf("wmm_map_dot1p:%d dot1p_map_wmm_num:%d\n", gRadioQosValues.qosValues[i].wmm_map_dot1p, 
					//	gRadioQosValues.qosValues[i].dot1p_map_wmm_num);
					int k=0;
					for(k=0;k<gRadioQosValues.qosValues[i].dot1p_map_wmm_num;k++)
					{
						CWWTPDebugLog("dot1p_map_wmm[%d]:%d", k, gRadioQosValues.qosValues[i].dot1p_map_wmm[k]);
						//printf("dot1p_map_wmm[%d]:%d\n", k, gRadioQosValues.qosValues[i].dot1p_map_wmm[k]);
					}
				}
			}
		}
	}
#endif
	unsigned char radioId=0;
	for(i=0;i<bindingPtr->directSequeueControlCount;i++)
	{
		radioId = bindingPtr->directSequeueControl[i].radio_id;
		gRadioInfoValue[radioId].channel = bindingPtr->directSequeueControl[i].current_channel;
		CWWTPDebugLog("radioId:%d channel:%d", radioId, gRadioInfoValue[radioId].channel);

		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"channel");
		autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioId].channel);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);
	}
	CW_FREE_OBJECT(bindingPtr->directSequeueControl);

	for(i=0;i<bindingPtr->txPowerCount;i++)
	{
		radioId = bindingPtr->txPower[i].radio_id;
		gRadioInfoValue[radioId].txtype = bindingPtr->txPower[i].reserved;
		gRadioInfoValue[radioId].txpower = bindingPtr->txPower[i].current_power;
		CWWTPDebugLog("radioId:%d  txtype:%d txpower:%d", radioId, gRadioInfoValue[radioId].txtype, gRadioInfoValue[radioId].txpower);

		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"txtype");
		autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioId].txtype);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
		
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"txpower");
		autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioId].txpower);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	}
	CW_FREE_OBJECT(bindingPtr->txPower);

	for(i=0;i<bindingPtr->rateCount;i++)
	{
		radioId = bindingPtr->rateSetValues[i].radioID;
		gRadioInfoValue[radioId].rate = bindingPtr->rateSetValues[i].rate;
		CWWTPDebugLog("radioId:%d rate:%d", radioId, gRadioInfoValue[radioId].rate/10);

		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"rate");
		autelan_sprintf(confArg.conf_value,"%hd",gRadioInfoValue[radioId].rate);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	}
	CW_FREE_OBJECT(bindingPtr->rateSetValues);
	for(i=0;i<bindingPtr->radioTypeCount;i++)
	{
		radioId = bindingPtr->radioTypeSetValues[i].radioID;
		gRadioInfoValue[radioId].radiotype = bindingPtr->radioTypeSetValues[i].radiotype;
		CWWTPDebugLog("radioId:%d radiotype:%d", radioId, gRadioInfoValue[radioId].radiotype);
		
		if(gConfigureStateRate[radioId].count !=0)
		{
			CWGetRateMask(gConfigureStateRate[radioId].rate, gConfigureStateRate[radioId].count,radioId);
			CW_FREE_OBJECT(gConfigureStateRate[radioId].rate);
			gConfigureStateRate[radioId].rate=NULL;
		}
		
		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"radiotype");
		autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioId].radiotype);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	}
	CW_FREE_OBJECT(bindingPtr->radioTypeSetValues);

	for(i=0;i<bindingPtr->macOperationCount;i++)
	{
		radioId = bindingPtr->macOperation[i].radio_id;
		gRadioInfoValue[radioId].fragThreshold = bindingPtr->macOperation[i].fragmentation_threshold;
		gRadioInfoValue[radioId].rtsThreshold = bindingPtr->macOperation[i].rts_threshold;
		gRadioInfoValue[radioId].shortRetry = bindingPtr->macOperation[i].short_retry;
		gRadioInfoValue[radioId].longRetry = bindingPtr->macOperation[i].long_retry;
		CWWTPDebugLog("radioId:%d fragThreshold:%d rtsThreshold:%d shortRetry:%d longRetry:%d", radioId, gRadioInfoValue[radioId].fragThreshold,
			gRadioInfoValue[radioId].rtsThreshold, gRadioInfoValue[radioId].shortRetry, gRadioInfoValue[radioId].longRetry);
		
		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"fragThreshold");
		autelan_sprintf(confArg.conf_value,"%hd",gRadioInfoValue[radioId].fragThreshold);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);

		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"rtsThreshold");
		autelan_sprintf(confArg.conf_value,"%hd",gRadioInfoValue[radioId].rtsThreshold);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);

		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"shortRetry");
		autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioId].shortRetry);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);

		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;		
		strcpy(confArg.conf_arg,"longRetry");
		autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioId].longRetry);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	}
	CW_FREE_OBJECT(bindingPtr->macOperation);
	
	for(i=0;i<bindingPtr->radioConfigCount;i++)
	{
		radioId = bindingPtr->radioConfiguration[i].radio_id;
		gRadioInfoValue[radioId].preamble = bindingPtr->radioConfiguration[i].is_short_preamble;
		gRadioInfoValue[radioId].dtim = bindingPtr->radioConfiguration[i].dtim_period;
		gRadioInfoValue[radioId].beaconInterval = bindingPtr->radioConfiguration[i].beacon_period;
		CWWTPDebugLog("radioId:%d preamble:%d dtim:%d beaconInterval:%d", radioId, gRadioInfoValue[radioId].preamble,
			gRadioInfoValue[radioId].dtim, gRadioInfoValue[radioId].beaconInterval);

		memset(CountryCode, 0, 4);
		memcpy(CountryCode, bindingPtr->radioConfiguration[i].country_code, 4);
		for(j=0;j<4;j++){
			if(CountryCode[j] == ' ')
				CountryCode[j] = '\0';
		}
		CWWTPDebugLog("wifi%d CountryCode:[%s]",radioId,CountryCode);
		
		autelan_sprintf(cmdbuf, "/usr/sbin/set_countrycode %d %s >/dev/null 2>&1",radioId,CountryCode);
		_system(cmdbuf);
		
		memset(cmdbuf,0,128);
		autelan_sprintf(cmdbuf, "iwpriv wifi%d setCountry %s >/dev/null",radioId,CountryCode);
		_system(cmdbuf);
		
		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"preamble");
		autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioId].preamble);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
		
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"dtim");
		autelan_sprintf(confArg.conf_value,"%d",gRadioInfoValue[radioId].dtim);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
		
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = radioId;
		strcpy(confArg.conf_arg,"beaconInterval");
		autelan_sprintf(confArg.conf_value,"%hd",gRadioInfoValue[radioId].beaconInterval);
		CWSaveConfigInfo(CONF_RADIO,&confArg,CW_FALSE);
	}

	CW_FREE_OBJECT(bindingPtr->radioConfiguration);

	return CW_TRUE;
}

CWBool CWBindingParseConfigureResponse (char *msg, int len, void **valuesPtr){
	int i,j,k,l,m,n,t;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	

	completeMsg.msg = msg;
	completeMsg.offset = 0;

	CWBindingConfigurationRequestValues *auxBindingPtr;
	CW_CREATE_OBJECT_ERR(auxBindingPtr, CWBindingConfigurationRequestValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
	*valuesPtr = (void *)auxBindingPtr;

	auxBindingPtr->qosCount = 0;
	auxBindingPtr->radioQosValues = NULL;
	auxBindingPtr->directSequeueControlCount = 0;
	auxBindingPtr->directSequeueControl = NULL;              //pei add 0623
	auxBindingPtr->txPowerCount = 0;
	auxBindingPtr->txPower = NULL;			              //pei add 0623
	auxBindingPtr->rateCount = 0;
	auxBindingPtr->rateSetValues = NULL;
	auxBindingPtr->radioTypeCount = 0;
	auxBindingPtr->radioTypeSetValues = NULL;
	auxBindingPtr->macOperationCount = 0;
	auxBindingPtr->macOperation = NULL;          //pei add 0722
	auxBindingPtr->radioConfigCount = 0;
	auxBindingPtr->radioConfiguration = NULL;   //pei add 0722

	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);

		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		if(CWBindingCheckType(elemLen))
			{}
		switch(elemType) {
			case BINDING_MSG_ELEMENT_TYPE_WTP_QOS:
				(auxBindingPtr->qosCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_DIRECT_SEQUEUE_CONTROL:      //pei add 0623
				(auxBindingPtr->directSequeueControlCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_TX_POWER:            			   //pei add 0623
				(auxBindingPtr->txPowerCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RATE_SET:      		//pei add 0715
				(auxBindingPtr->rateCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RADIO_TYPE_SET:         //pei add 0715
				(auxBindingPtr->radioTypeCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_MAC_OPERATE:         //pei add 0722
				(auxBindingPtr->macOperationCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_WTP_RADIO_CONFIGURATON:         //pei add 0722
				(auxBindingPtr->radioConfigCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
/*			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_ADMINISTRATIVE_STATE:         //pei add 0722
				if(!(CWParseAdminState(&completeMsg, elemLen, auxBindingPtr->adminState)))
				{
					CW_FREE_OBJECT(auxBindingPtr->adminState);
					CW_FREE_OBJECT(valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				break;*/
			default:
				if(CWBindingCheckType(elemType)) 
				{
					CWWTPDebugLog("Unrecognized Binding Message Element:%d, in Config Response", elemType);
				}
				completeMsg.offset += elemLen;
				break;
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	// actually read each radio info
	CW_CREATE_ARRAY_ERR(auxBindingPtr->directSequeueControl, auxBindingPtr->directSequeueControlCount, DirectSequeueControlValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->radioQosValues, auxBindingPtr->qosCount, Radio_QosValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->txPower, auxBindingPtr->txPowerCount, TxPowerValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	         //pei add 0623
	CW_CREATE_ARRAY_ERR(auxBindingPtr->rateSetValues, auxBindingPtr->rateCount, BindingRate, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););        //pei add 0623
	CW_CREATE_ARRAY_ERR(auxBindingPtr->radioTypeSetValues, auxBindingPtr->radioTypeCount, BindingRadioType, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	 //pei add 0623
	CW_CREATE_ARRAY_ERR(auxBindingPtr->macOperation, auxBindingPtr->macOperationCount, MacOperationValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	 //pei add 0722
	CW_CREATE_ARRAY_ERR(auxBindingPtr->radioConfiguration, auxBindingPtr->radioConfigCount, WTPRadioConfigurationValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	 //pei add 0722

	i=0;j=0,k=0,l=0,m=0,n=0,t=0;
	completeMsg.offset = 0;
	while(completeMsg.offset < len) {
		unsigned short int type=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&elemLen);		

		switch(type) {
			case BINDING_MSG_ELEMENT_TYPE_WTP_QOS:{
				unsigned char tagPackets;
				if(!(CWParseWTPQoS(&completeMsg, elemLen, &(auxBindingPtr->radioQosValues[i].radioID), &tagPackets, auxBindingPtr->radioQosValues[i].qosValues)))
				{
					CW_FREE_OBJECT(auxBindingPtr->radioQosValues);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				i++;
				break;
			}
			case BINDING_MSG_ELEMENT_TYPE_DIRECT_SEQUEUE_CONTROL:      //pei add 0623
				if(!(CWParseIeee80211DirectSequenceControl(&completeMsg, elemLen, &(auxBindingPtr->directSequeueControl[t]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->directSequeueControl);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				t++;
				break;
			case BINDING_MSG_ELEMENT_TYPE_TX_POWER:            			   //pei add 0623
				if(!(CWParseIeee80211TxPower(&completeMsg, elemLen, &(auxBindingPtr->txPower[j]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->txPower);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				j++;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RATE_SET:      		//pei add 0715
				if(!(CWParseRateSet2(&completeMsg, elemLen, &(auxBindingPtr->rateSetValues[k]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->rateSetValues);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				k++;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_RADIO_TYPE_SET:         //pei add 0715
				if(!(CWParseRadioTypeSet(&completeMsg, elemLen, &(auxBindingPtr->radioTypeSetValues[l]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->radioTypeSetValues);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				l++;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_MAC_OPERATE:         //pei add 0722
				if(!(CWParseMacOperation(&completeMsg, elemLen, &(auxBindingPtr->macOperation[m]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->macOperation);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				m++;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_WTP_RADIO_CONFIGURATON:         //pei add 0722
				if(!(CWParseWTPRadioConfiguration(&completeMsg, elemLen, &(auxBindingPtr->radioConfiguration[n]))))
				{
					CW_FREE_OBJECT(auxBindingPtr->radioConfiguration);
					CW_FREE_OBJECT(*valuesPtr);
					return CW_FALSE; // will be handled by the caller
				}
				n++;
				break;
			default:
				completeMsg.offset += elemLen;
				break;
		}
	}
	
	
	return CW_TRUE;
}
/***************************************************************************
 * 
 * Function:  CWBindingParseIEEE80211WlanConfigurationRequest
 *
 * Purpose:   parse binding message element of ieee80211 waln configuration 
 *
 * Inputs:  msg (the control message)
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CWBool CWBindingParseIEEE80211WlanConfigurationRequest(char *msg, int len, void **valuesPtr)
{
	int i;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG,NULL);
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	completeMsg.msgLen = len;
	CWBINDINGIEEE80211WlanconfigurationRequestValues *auxBindingPtr;
	CW_CREATE_OBJECT_ERR(auxBindingPtr, CWBINDINGIEEE80211WlanconfigurationRequestValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
	*valuesPtr = (void *)auxBindingPtr;
	
	auxBindingPtr->addwlancount = 0;
	auxBindingPtr->addWlanValues = NULL;
	auxBindingPtr->infoelecount = 0;       //pei 0606
	auxBindingPtr->ieee80211_InfoEleValues = NULL;       //pei 0604
	auxBindingPtr->deleteWlanCout =0;
	auxBindingPtr->deleteWlanValues = NULL;
	auxBindingPtr->updateWlanCount = 0;
	auxBindingPtr->updateWlanValues = NULL;
	auxBindingPtr->wlanVlanCount = 0;
	auxBindingPtr->wlanVlanValues = NULL;
	auxBindingPtr->wapiCount = 0;  //pei add 0309
	auxBindingPtr->wapiValues = NULL;
	auxBindingPtr->ChangeTunnelModeCount = 0;
	auxBindingPtr->changeWlanTunnelMode = NULL;
	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		


		switch(elemType) {
			case BINDING_MSG_ELEMENT_TYPE_CHANGE_WLAN_TUNNEL_MODE:
				(auxBindingPtr->ChangeTunnelModeCount)++; 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_ADD_WLAN:
				(auxBindingPtr->addwlancount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_DELETE_WLAN:
				(auxBindingPtr->deleteWlanCout)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_UPDATE_WLAN:
				(auxBindingPtr->updateWlanCount)++; // just count 
				completeMsg.offset += elemLen;
				break;	
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_INFO_ELEMENT:              //pei add 0605
				(auxBindingPtr->infoelecount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_WLAN_VLAN_INFO:         //pei add 0305
				(auxBindingPtr->wlanVlanCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_WAPI_CER_INFO_ELEMENT:
				(auxBindingPtr->wapiCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			default:
				if(CWBindingCheckType(elemType)) 
				{
					CWWTPDebugLog("Unrecognized Binding Message Element:%d, in Binding of 80211 Wlan Config Request", elemType);
				}
				completeMsg.offset += elemLen;
				break;
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	// actually read each radio info
	CW_CREATE_ARRAY_ERR(auxBindingPtr->addWlanValues, auxBindingPtr->addwlancount, AddWlanValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CW_CREATE_ARRAY_ERR(auxBindingPtr->ieee80211_InfoEleValues, auxBindingPtr->infoelecount, Ieee80211InfoEleValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	//pei 0604
	CW_CREATE_ARRAY_ERR(auxBindingPtr->deleteWlanValues, auxBindingPtr->deleteWlanCout, DeleteWlanValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->updateWlanValues, auxBindingPtr->updateWlanCount, UpdateWlanValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->ieee80211_InfoEleValues->wtp_ie, auxBindingPtr->infoelecount, WPA_IE, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	//pei 0606
	CW_CREATE_ARRAY_ERR(auxBindingPtr->wlanVlanValues, auxBindingPtr->wlanVlanCount, WlanVlanValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	//pei 0305
	CW_CREATE_ARRAY_ERR(auxBindingPtr->wapiValues, auxBindingPtr->wapiCount, WapiValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	//pei 0309
	CW_CREATE_ARRAY_ERR(auxBindingPtr->changeWlanTunnelMode, auxBindingPtr->ChangeTunnelModeCount, ChangeTunnelMode, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	//pei 0309

	i=0;
	completeMsg.offset = 0;
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		switch(elemType) {
			case BINDING_MSG_ELEMENT_TYPE_CHANGE_WLAN_TUNNEL_MODE:
				if(!(CWBindingParseChangeWlanTunnelMode(&completeMsg, elemLen,auxBindingPtr->changeWlanTunnelMode))){
					completeMsg.offset += elemLen;
				}
				break;
			case BINDING_MSG_ELEMENT_TYPE_ADD_WLAN:
				if(!(CWParseAddWlan(&completeMsg, elemLen,auxBindingPtr->addWlanValues))){
				completeMsg.offset += elemLen;
				}
				break;
			case BINDING_MSG_ELEMENT_TYPE_DELETE_WLAN:
				if(!(CWParseDeleteWlan(&completeMsg, elemLen,&auxBindingPtr->deleteWlanValues[i]))){
				completeMsg.offset += elemLen;
				}
				break;
			case BINDING_MSG_ELEMENT_TYPE_UPDATE_WLAN:
				if(!(CWParseUpdateWlan(&completeMsg, elemLen,auxBindingPtr->updateWlanValues))){
				completeMsg.offset += elemLen;
				}
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_INFO_ELEMENT:              //pei add 0605
				if(!(CWParseInformationElement(&completeMsg, elemLen,auxBindingPtr->ieee80211_InfoEleValues))){
				completeMsg.offset += elemLen;
				}
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_WLAN_VLAN_INFO:         //pei add 0305
				if(!(CWParseWlanVlan(&completeMsg, elemLen,auxBindingPtr->wlanVlanValues))){
				completeMsg.offset += elemLen;
				}
				break;
			case BINDING_MSG_ELEMENT_TYPE_WAPI_CER_INFO_ELEMENT:
				if(!(CWParseWapi(&completeMsg, elemLen,auxBindingPtr->wapiValues))){
				completeMsg.offset += elemLen;
				}
				break;
			default:
				completeMsg.offset += elemLen;
				break;
		}

	}
	
	return CW_TRUE;
}

CWBool CWBindingSaveIEEE80211WlanConfigurationRequest(void* bindingValuesPtr, unsigned char *created_radio_id, unsigned char* created_wlan_id, CWProtocolResultCode* resultCode)
{	
	if(bindingValuesPtr==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	CWBINDINGIEEE80211WlanconfigurationRequestValues* bindingPtr = (CWBINDINGIEEE80211WlanconfigurationRequestValues*)bindingValuesPtr; 
	char wlanname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	
	if(bindingPtr->addwlancount > 0&&bindingPtr->infoelecount > 0&&bindingPtr->wlanVlanCount > 0){
		*created_radio_id = bindingPtr->addWlanValues->radio_id;
		*created_wlan_id = bindingPtr->addWlanValues->wlan_id;
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == bindingPtr->addWlanValues->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
		if(gApScanningEnable == 1)
		{
			_system("killall iwlist_scan > /dev/null 2>&1");
			autelan_system("/sbin/ifconfig athscanwifi0 down > /dev/null 2>&1 &");
			if(0==Check_Interface_State("athscanwifi1"))
				autelan_system("ifconfig athscanwifi1 down > /dev/null 2>&1 &");
		}
//		if(!CWBindingAddWlan(bindingPtr->addWlanValues,resultCode)){       //yuan lai de            modified by peiwenhui 0604
//		if(!CWBindingAddWlan(bindingPtr->addWlanValues, bindingPtr->ieee80211_InfoEleValues->wtp_ie, resultCode)){
		if(!CWBindingAddWlan(bindingPtr->addWlanValues, bindingPtr->ieee80211_InfoEleValues->wtp_ie, bindingPtr->wlanVlanValues, bindingPtr->wapiValues, resultCode)){
			CW_FREE_OBJECT(bindingPtr->addWlanValues->key);/*csc add 080716*/
			CW_FREE_OBJECT(bindingPtr->addWlanValues->ssid);/*csc add 080716*/
			CW_FREE_OBJECT(bindingPtr->addWlanValues);
			CW_FREE_OBJECT(bindingPtr->ieee80211_InfoEleValues->wtp_ie);  //pei add 0606
			CW_FREE_OBJECT(bindingPtr->ieee80211_InfoEleValues);  //pei add 0605
			CW_FREE_OBJECT(bindingPtr->wlanVlanValues);  //pei add 0309
			CW_FREE_OBJECT(bindingPtr->wapiValues->cerAsValue);
			CW_FREE_OBJECT(bindingPtr->wapiValues->cerAeValue);
			CW_FREE_OBJECT(bindingPtr->wapiValues->asValue);
			CW_FREE_OBJECT(bindingPtr->wapiValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == bindingPtr->addWlanValues->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
		if(gApScanningEnable == 1)
		{
			char wifi0mode[10]={0};
			char wifi1mode[10]={0};
			autelan_system("/sbin/ifconfig athscanwifi0 up > /dev/null 2>&1 &");
			memset(cmdbuf, 0, 128);
			if(0==Check_Interface_State("athscanwifi1"))
				autelan_system("ifconfig athscanwifi1 up > /dev/null 2>&1 &");
			
			if((gRadioInfoValue[0].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
				memcpy(wifi0mode,"11a",sizeof("11a"));
			else memcpy(wifi0mode,"11bg",sizeof("11bg"));
			
			if(0==Check_Interface_State("athscanwifi1"))
			{
				if((gRadioInfoValue[1].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
					 memcpy(wifi1mode,"11a",sizeof("11a"));
				else memcpy(wifi1mode,"11bg",sizeof("11bg"));
		
				autelan_sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -c %s -I %d -i 5 -n 2 > /dev/null 2>&1 &",wifi0mode,wifi1mode, gApScanningInterval/3>60?gApScanningInterval/3:60);
			}
			else
				autelan_sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -I %d -i 5 -n 1 > /dev/null 2>&1 &",wifi0mode, gApScanningInterval/3>60?gApScanningInterval/3:60);
			_system(cmdbuf);
		}
		CW_FREE_OBJECT(bindingPtr->addWlanValues->key);/*csc add 080716*/
		CW_FREE_OBJECT(bindingPtr->addWlanValues->ssid);/*csc add 080716*/
		CW_FREE_OBJECT(bindingPtr->addWlanValues);
		CW_FREE_OBJECT(bindingPtr->ieee80211_InfoEleValues->wtp_ie);  //pei add 0606
		CW_FREE_OBJECT(bindingPtr->ieee80211_InfoEleValues);  //pei add 0605
		CW_FREE_OBJECT(bindingPtr->wlanVlanValues);  //pei add 0309
		CW_FREE_OBJECT(bindingPtr->wapiValues->cerAsValue);
		CW_FREE_OBJECT(bindingPtr->wapiValues->cerAeValue);
		CW_FREE_OBJECT(bindingPtr->wapiValues->asValue);
		CW_FREE_OBJECT(bindingPtr->wapiValues);
	}
	if(bindingPtr->deleteWlanCout > 0){
		*created_radio_id = bindingPtr->deleteWlanValues->radio_id;
		*created_wlan_id = bindingPtr->deleteWlanValues->wlan_id;
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == bindingPtr->deleteWlanValues->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s down > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
		if(gApScanningEnable == 1)
		{
			_system("killall iwlist_scan > /dev/null 2>&1");
			autelan_system("/sbin/ifconfig athscanwifi0 down > /dev/null 2>&1 &");
			if(0==Check_Interface_State("athscanwifi1"))
			 autelan_system("ifconfig athscanwifi1 down >/dev/null 2>&1 &");
		}
		if(!CWBindingDeleteWlan(bindingPtr->deleteWlanCout,bindingPtr->deleteWlanValues,resultCode)){
			CW_FREE_OBJECT(bindingPtr->deleteWlanValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->radio_id == bindingPtr->deleteWlanValues->radio_id)
			{
				ptr->wlan_updown_time++;  //pei add 0225
				CWWTPGetWlanName(wlanname,ptr->radio_id,ptr->wlan_id);
				autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
				autelan_system(cmdbuf);
			}
		}
		if(gApScanningEnable == 1)
		{
			char wifi0mode[10]={0};
			char wifi1mode[10]={0};
			autelan_system("/sbin/ifconfig athscanwifi0 up > /dev/null 2>&1 &");
			memset(cmdbuf, 0, 128);
			if(0==Check_Interface_State("athscanwifi1"))
				autelan_system("ifconfig athscanwifi1 up >/dev/null 2>&1 &");
			
			
			if((gRadioInfoValue[0].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
				memcpy(wifi0mode,"11a",sizeof("11a"));
			else memcpy(wifi0mode,"11bg",sizeof("11bg"));
			
			if(0==Check_Interface_State("athscanwifi1"))
			{
				if((gRadioInfoValue[1].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
					memcpy(wifi1mode,"11a",sizeof("11a"));
				else memcpy(wifi1mode,"11bg",sizeof("11bg"));
				autelan_sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -c %s -I %d -i 5 -n 2 > /dev/null 2>&1 &",wifi0mode,wifi1mode,gApScanningInterval/3>60?gApScanningInterval/3:60);
			}
			else
				autelan_sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s  -I %d -i 5 -n 1 > /dev/null 2>&1 &",wifi0mode,gApScanningInterval/3>60?gApScanningInterval/3:60);
			_system(cmdbuf);
		}
		CW_FREE_OBJECT(bindingPtr->deleteWlanValues);
	}
	if(bindingPtr->updateWlanCount > 0){
		*created_radio_id = bindingPtr->updateWlanValues->radio_id;
		*created_wlan_id = bindingPtr->updateWlanValues->wlan_id;
		if(!CWBindingUpdateWlan(bindingPtr->updateWlanCount,bindingPtr->updateWlanValues,resultCode)){
			CW_FREE_OBJECT(bindingPtr->updateWlanValues->key);
			CW_FREE_OBJECT(bindingPtr->updateWlanValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(bindingPtr->updateWlanValues->key);
		CW_FREE_OBJECT(bindingPtr->updateWlanValues);
	}
	if(bindingPtr->ChangeTunnelModeCount>0){
		*created_radio_id = bindingPtr->changeWlanTunnelMode->radio_id;
		*created_wlan_id  = bindingPtr->changeWlanTunnelMode->wlan_id;
		if(!CWBindingSaveChangeWlanTunnelMode(bindingPtr->ChangeTunnelModeCount,bindingPtr->changeWlanTunnelMode,resultCode)){
			CW_FREE_OBJECT(bindingPtr->changeWlanTunnelMode);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
	}		
	CW_FREE_OBJECT(bindingPtr);
	return CW_TRUE;
}
CWBool CWBindingParseSTAConfigurationRequest(char *msg, int len, void **valuesPtr)
{
	int i;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG,NULL);
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	completeMsg.msgLen = len;
	CWBINDINGSTAConfigurationRequestValues *auxBindingPtr;
	CW_CREATE_OBJECT_ERR(auxBindingPtr, CWBINDINGSTAConfigurationRequestValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
	*valuesPtr = (void *)auxBindingPtr;
	
	auxBindingPtr->addstacount = 0;
	auxBindingPtr->addSTAValues = NULL;
	auxBindingPtr->deletestacount =0;
	auxBindingPtr->deleteSTAValues = NULL;
	auxBindingPtr->stationsessionkeycount =0;
	auxBindingPtr->staSessionKeyValues = NULL;
	auxBindingPtr->infoelecount = 0;      //pei add 0606
	auxBindingPtr->ieee80211_InfoEleValues = NULL;      //pei add 0606
	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		switch(elemType) {
			case CW_MSG_ELEMENT_ADD_STATION_CW_TYPE:
				(auxBindingPtr->addstacount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE:
				(auxBindingPtr->deletestacount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_STATION_SESSION_KEY:
				(auxBindingPtr->stationsessionkeycount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_INFO_ELEMENT:              //pei add 0605
				(auxBindingPtr->infoelecount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			default:
				if(CWBindingCheckType(elemType)) 
				{
					CWWTPDebugLog("Unrecognized Binding Message Element:%d, in Binding of Sta Config Request", elemType);
				}
				completeMsg.offset += elemLen;
				break;
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	// actually read each radio info
	CW_CREATE_ARRAY_ERR(auxBindingPtr->addSTAValues , auxBindingPtr->addstacount, AddSTAValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CW_CREATE_ARRAY_ERR(auxBindingPtr->deleteSTAValues, auxBindingPtr->deletestacount, DeleteSTAValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->staSessionKeyValues, auxBindingPtr->stationsessionkeycount, STASessionKeyValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(auxBindingPtr->ieee80211_InfoEleValues, auxBindingPtr->infoelecount, Ieee80211InfoEleValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););   //pei add 0605
	CW_CREATE_ARRAY_ERR(auxBindingPtr->ieee80211_InfoEleValues->wtp_ie, auxBindingPtr->infoelecount, WPA_IE, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	//pei 0606
	i=0;
	completeMsg.offset = 0;
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		switch(elemType) {
			case CW_MSG_ELEMENT_ADD_STATION_CW_TYPE:
				 if(!(CWParseAddSta(&completeMsg, elemLen,auxBindingPtr->addSTAValues))){
				 completeMsg.offset += elemLen;
				 }
				 break;
			case CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE:
				 if(!(CWParseDeleteSta(&completeMsg, elemLen,auxBindingPtr->deleteSTAValues))){
				 completeMsg.offset += elemLen;
				 }
				 break;
			case BINDING_MSG_ELEMENT_TYPE_STATION_SESSION_KEY:				
				 if(!(CWParseSTASessionKey(&completeMsg, elemLen,auxBindingPtr->staSessionKeyValues))){
				 completeMsg.offset += elemLen;
				 }
				 break;
			case BINDING_MSG_ELEMENT_TYPE_IEEE80211_INFO_ELEMENT:              //pei add 0606
				 if(!(CWParseInformationElement(&completeMsg, elemLen,auxBindingPtr->ieee80211_InfoEleValues))){
				 completeMsg.offset += elemLen;
				 }
				 break;
			default:
				completeMsg.offset += elemLen;
				break;
		}

	}
	
	return CW_TRUE;
}
CWBool CWBindingSaveSTAConfigurationRequest(void* bindingValuesPtr, CWProtocolResultCode* resultCode){
	if(bindingValuesPtr==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;
	
	CWBINDINGSTAConfigurationRequestValues* bindingPtr = (CWBINDINGSTAConfigurationRequestValues*)bindingValuesPtr; 
	if(bindingPtr->addstacount > 0){
		if(!CWBindingAddSta(bindingPtr->addSTAValues,resultCode)){
			CW_FREE_OBJECT(bindingPtr->addSTAValues->mac_addr);/*csc add 080716*/
			CW_FREE_OBJECT(bindingPtr->addSTAValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}	
		CW_FREE_OBJECT(bindingPtr->addSTAValues->mac_addr);/*csc add 080716*/
		CW_FREE_OBJECT(bindingPtr->addSTAValues);
		
	}
	if(bindingPtr->deletestacount > 0){
		if(!CWBindingDeleteSta(bindingPtr->deleteSTAValues,resultCode)){
			CW_FREE_OBJECT(bindingPtr->deleteSTAValues->mac_addr);
			CW_FREE_OBJECT(bindingPtr->deleteSTAValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(bindingPtr->deleteSTAValues->mac_addr);
		CW_FREE_OBJECT(bindingPtr->deleteSTAValues);
	}
	if(bindingPtr->stationsessionkeycount > 0&&bindingPtr->infoelecount > 0){
		if(!CWBindingSetSTASessionKey(bindingPtr->staSessionKeyValues, bindingPtr->ieee80211_InfoEleValues, resultCode)){
			CW_FREE_OBJECT(bindingPtr->staSessionKeyValues->mac);
			CW_FREE_OBJECT(bindingPtr->staSessionKeyValues->Pairwise_RSC);
			CW_FREE_OBJECT(bindingPtr->staSessionKeyValues->Pairwise_TSC);
			CW_FREE_OBJECT(bindingPtr->staSessionKeyValues->key);
			CW_FREE_OBJECT(bindingPtr->staSessionKeyValues);
			CW_FREE_OBJECT(bindingPtr->ieee80211_InfoEleValues->wtp_ie);
			CW_FREE_OBJECT(bindingPtr->ieee80211_InfoEleValues);
			CW_FREE_OBJECT(bindingPtr);
			return CW_FALSE;
		}		
		CW_FREE_OBJECT(bindingPtr->staSessionKeyValues->mac);
		CW_FREE_OBJECT(bindingPtr->staSessionKeyValues->Pairwise_RSC);
		CW_FREE_OBJECT(bindingPtr->staSessionKeyValues->Pairwise_TSC);
		CW_FREE_OBJECT(bindingPtr->staSessionKeyValues->key);
		CW_FREE_OBJECT(bindingPtr->staSessionKeyValues);
		CW_FREE_OBJECT(bindingPtr->ieee80211_InfoEleValues->wtp_ie);
		CW_FREE_OBJECT(bindingPtr->ieee80211_InfoEleValues);
	}
	CW_FREE_OBJECT(bindingPtr);
	return CW_TRUE;

}

#define CONF_WTPD_PATH 		"/tmp/conf_wtpd"
#define CONF_RADIO_PATH 	"/tmp/conf_wtpd/radio"

CWBool CWSaveAddwlanConfigInfo(CWWTPWlan *wlancreating)
{
	ConfArg confArg;
	int wlan_count = 0;
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;

	for(ptr = wtp_wlan_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next)
	{
		if(wlancreating->radio_id == ptr->radio_id)
		{
			wlan_count++;
		}
	}
	
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = wlancreating->radio_id;
	memcpy(confArg.conf_arg,"wlan_count",11);
	autelan_sprintf(confArg.conf_value,"%d",wlan_count);
	CWSaveConfigInfo(CONF_RADIO,&confArg,CW_TRUE);

	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = wlancreating->radio_id;
	confArg.wlanID	= wlancreating->wlan_id;
	memcpy(confArg.conf_arg,"radio_id",9);
	autelan_sprintf(confArg.conf_value,"%d",wlancreating->radio_id);
	CWSaveConfigInfo(CONF_WLAN,&confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = wlancreating->radio_id;
	confArg.wlanID	= wlancreating->wlan_id;
	strcpy(confArg.conf_arg,"wlan_id");
	autelan_sprintf(confArg.conf_value,"%d",wlancreating->wlan_id);
	CWSaveConfigInfo(CONF_WLAN,&confArg,CW_FALSE);
	
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = wlancreating->radio_id;
	confArg.wlanID	= wlancreating->wlan_id;
	strcpy(confArg.conf_arg,"vlan_id");
	autelan_sprintf(confArg.conf_value,"%hd",wlancreating->vlan_id);
	CWSaveConfigInfo(CONF_WLAN,&confArg,CW_FALSE);
	
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = wlancreating->radio_id;
	confArg.wlanID	= wlancreating->wlan_id;
	strcpy(confArg.conf_arg,"wlan_updown_time");
	autelan_sprintf(confArg.conf_value,"%d",wlancreating->wlan_updown_time);
	CWSaveConfigInfo(CONF_WLAN,&confArg,CW_FALSE);
	
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = wlancreating->radio_id;
	confArg.wlanID	= wlancreating->wlan_id;
	strcpy(confArg.conf_arg,"wlan_mac_mode");
	autelan_sprintf(confArg.conf_value,"%d",wlancreating->wlan_mac_mode);
	CWSaveConfigInfo(CONF_WLAN,&confArg,CW_FALSE);
	
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = wlancreating->radio_id;
	confArg.wlanID	= wlancreating->wlan_id;
	strcpy(confArg.conf_arg,"wlan_tunnel_mode");
	autelan_sprintf(confArg.conf_value,"%d",wlancreating->wlan_tunnel_mode);
	CWSaveConfigInfo(CONF_WLAN,&confArg,CW_FALSE);

	memset(&confArg,0,sizeof(confArg));
	confArg.radioID = wlancreating->radio_id;
	confArg.wlanID = wlancreating->wlan_id;
	strcpy(confArg.conf_arg,"ssid");
	strcpy(confArg.conf_value,wlancreating->essid);
	CWSaveConfigInfo(CONF_WLAN,&confArg,CW_FALSE);
	return CW_TRUE;
}
#if 0
CWBool CWSaveEMenuConfig(Conf_Type conf_type,ConfArg confArg)
{
	int nParams = 7;/*  the size of array */
	char eMenuArgs[7][16] = {/* radio level */"channel","txpower","radiotype","cwmmode","extoffset",
						      /*  wlan level */"aclpolicy","ssid"};
							/* notice: modify the size of array(eMenuArgs) if you add or del  some param*/
	int needSave = 0;
	
	int i;
	for(i = 0; i < nParams; i++){
		if(memcmp(confArg.conf_arg, eMenuArgs[i], sizeof(eMenuArgs[i])) == 0){
			needSave = 1;
			break;
		}
	}
	if(needSave == 0) return CW_TRUE;

	if(memcmp(confArg.conf_arg, "radiotype", sizeof("radiotype")) == 0){
		char *mode;
		get_radio_mode(atoi(confArg.conf_value), &mode);		//mode like "11bg"
		strncpy(confArg.conf_value,mode,strlen(mode));	
		CW_FREE_OBJECT(mode);
		memset(confArg.conf_arg, 0, sizeof(confArg.conf_arg));
		strncpy(confArg.conf_arg, "mode", sizeof("mode"));
	}
	if(memcmp(confArg.conf_arg, "cwmmode", sizeof("cwmmode")) == 0){
		strncpy(confArg.conf_value,"cwwmode", sizeof("cwwmode"));
	}
	int rid = confArg.radioID;
	int wid = confArg.wlanID;
	if(CONF_WTPD == conf_type)
	{
		CWXmlWriteWTPDParam(_jffs_config_xml_, confArg.conf_arg,confArg.conf_value);
	}
	else if(CONF_RADIO == conf_type)
	{
		CWXmlWriteRadioParam( _jffs_config_xml_,rid, confArg.conf_arg,confArg.conf_value);
	}
	else if(CONF_WLAN == conf_type)
	{
		CWXmlWriteWlanParam(_jffs_config_xml_,rid, wid, confArg.conf_arg,confArg.conf_value);
	}

	return CW_TRUE;
}
#endif
CWBool CWSaveEMenuConfig(Conf_Type conf_type,ConfArg *confArg)
{
	if(confArg == NULL)
		return CW_FALSE;
	int nParams = 7;/*  the size of array */
	char eMenuArgs[7][16] = {/* radio level */"channel","txpower","radiotype","cwmmode","extoffset",
						      /*  wlan level */"aclpolicy","ssid"};
							/* notice: modify the size of array(eMenuArgs) if you add or del  some param*/
	int needSave = 0;
	
	int i;
	for(i = 0; i < nParams; i++){
		if(memcmp(confArg->conf_arg, eMenuArgs[i], sizeof(eMenuArgs[i])) == 0){
			needSave = 1;
			break;
		}
	}
	if(needSave == 0) return CW_TRUE;

	if(memcmp(confArg->conf_arg, "radiotype", sizeof("radiotype")) == 0){
		char *mode;
		get_radio_mode(atoi(confArg->conf_value), &mode);		//mode like "11bg"
		strncpy(confArg->conf_value,mode,strlen(mode));	
		CW_FREE_OBJECT(mode);
		memset(confArg->conf_arg, 0, sizeof(confArg->conf_arg));
		strncpy(confArg->conf_arg, "mode", sizeof("mode"));
	}
	if(memcmp(confArg->conf_arg, "cwmmode", sizeof("cwmmode")) == 0){
		strncpy(confArg->conf_value,"cwwmode", sizeof("cwwmode"));
	}
	int rid = confArg->radioID;
	int wid = confArg->wlanID;
	if(CONF_WTPD == conf_type)
	{
		CWXmlWriteWTPDParam(_jffs_config_xml_, confArg->conf_arg,confArg->conf_value);
	}
	else if(CONF_RADIO == conf_type)
	{
		CWXmlWriteRadioParam( _jffs_config_xml_,rid, confArg->conf_arg,confArg->conf_value);
	}
	else if(CONF_WLAN == conf_type)
	{
		CWXmlWriteWlanParam(_jffs_config_xml_,rid, wid, confArg->conf_arg,confArg->conf_value);
	}

	return CW_TRUE;
}

#if 0
CWBool CWSaveConfigInfo(Conf_Type conf_type,ConfArg confArg,CWBool chkdir)
{
	CWBool bRet = CW_TRUE;
	//int debug_print_dwq = 1;
	//if(debug_print_dwq)
	//	printf("conf_type:%d\t radioID:%d\t wlanID:%d\t %s\t = %s\n ",conf_type,confArg.radioID,confArg.wlanID,confArg.conf_arg,confArg.conf_value);
	
	if(CONF_WTPD == conf_type)
	{
		//make sure the dir of  "conf_wtpd" is exist		
		if(chkdir == CW_TRUE)
		{
			if(0 != access(CONF_WTPD_PATH, F_OK))
			{
				if(0 != mkdir(CONF_WTPD_PATH,S_IRWXU))
					CWWTPDebugLog("create path:%s error!",CONF_WTPD_PATH);
			}
		}
		//save the attribute of the configuration of "wtpd"
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		autelan_sprintf(filepath,"%s/%s",CONF_WTPD_PATH,confArg.conf_arg);
		if((fd = autelan_open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			autelan_printf("create file %s failed!\n",filepath);			
		}
		else
		{
			autelan_write(fd,confArg.conf_value,sizeof(confArg.conf_value));
			autelan_close(fd);
		}
	}
	else if(CONF_RADIO == conf_type)
	{
		//make sure the dir of  "conf_wtpd/radio" is exist
		char pathbuf[128];
		memset(pathbuf,0,128);
		autelan_sprintf(pathbuf,"%s%d",CONF_RADIO_PATH,confArg.radioID);	
		//if(debug_print_dwq)
		//	printf("CONF_RADIO pathbuf=%s \t %s=%s\n",pathbuf,confArg.conf_arg,confArg.conf_value);
		if(chkdir == CW_TRUE)
		{
			if(0 != access(pathbuf, F_OK))
			{
				if(0 != mkdir(pathbuf,S_IRWXU))
					CWWTPDebugLog("create path:%s error!",pathbuf);
			}	
		}
		//save the attribute of the configuration of "radio"
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		autelan_sprintf(filepath,"%s/%s",pathbuf,confArg.conf_arg);
		if((fd = autelan_open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			autelan_printf("create file %s failed!\n",filepath);			
		}
		else
		{
			autelan_write(fd,confArg.conf_value,sizeof(confArg.conf_value));
			autelan_close(fd);
		}
	}
	else if(CONF_WLAN ==conf_type)
	{
		//make sure the dir of "conf_wtpd/radio/wlan" is exist
		char pathbuf1[128];
		char pathbuf2[128];
		memset(pathbuf1,0,128);
		memset(pathbuf2,0,128);
		autelan_sprintf(pathbuf1,"%s%d/wlaninfo",CONF_RADIO_PATH,confArg.radioID);		
		autelan_sprintf(pathbuf2,"%s%d/wlaninfo/wlan%d",CONF_RADIO_PATH,confArg.radioID,confArg.wlanID); 			
		//if(debug_print_dwq)
		//	printf("CONF_WLAN pathbuf=%s \t %s=%s\n",pathbuf2,confArg.conf_arg,confArg.conf_value);
		if(chkdir == CW_TRUE)
		{
			if(0 != access(pathbuf1, F_OK))
			{
				if(0 != mkdir(pathbuf1,S_IRWXU))
					CWWTPDebugLog("create path:%s error!",pathbuf1);
			}			
			if(0 != access(pathbuf2, F_OK))
			{
				if(0 != mkdir(pathbuf2,S_IRWXU))
					CWWTPDebugLog("create path:%s error!",pathbuf2);
			}	
		}
		//save the attribute of the configuration of "wlan"		
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		autelan_sprintf(filepath,"%s/%s",pathbuf2,confArg.conf_arg);
		if((fd = autelan_open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			autelan_printf("create file %s failed!\n",filepath);			
		}
		else
		{
			autelan_write(fd,confArg.conf_value,sizeof(confArg.conf_value));
			autelan_close(fd);
		}
	}
	else
	{
		bRet = CW_FALSE;
		dpf("CONF_return_false\n");		
	}

	if(gEMenuConf.e_swtich == 1 && gRunRetry == 0){
		CWSaveEMenuConfig(conf_type, confArg);
	}
	return bRet;
}

#endif

CWBool CWSaveConfigInfo(Conf_Type conf_type,ConfArg *confArg,CWBool chkdir)
{
	CWBool bRet = CW_TRUE;
	//int debug_print_dwq = 1;
	//if(debug_print_dwq)
	//	printf("conf_type:%d\t radioID:%d\t wlanID:%d\t %s\t = %s\n ",conf_type,confArg.radioID,confArg.wlanID,confArg.conf_arg,confArg.conf_value);
	if(confArg == NULL)
		return CW_FALSE;
		
	if(CONF_WTPD == conf_type)
	{
		//make sure the dir of  "conf_wtpd" is exist		
		if(chkdir == CW_TRUE)
		{
			if(0 != access(CONF_WTPD_PATH, F_OK))
			{
				if(0 != mkdir(CONF_WTPD_PATH,S_IRWXU))
					CWWTPDebugLog("create path:%s error!",CONF_WTPD_PATH);
			}
		}
		//save the attribute of the configuration of "wtpd"
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		autelan_sprintf(filepath,"%s/%s",CONF_WTPD_PATH,confArg->conf_arg);
		if((fd = autelan_open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			autelan_printf("create file %s failed!\n",filepath);			
		}
		else
		{
			autelan_write(fd,confArg->conf_value,sizeof(confArg->conf_value));
			autelan_close(fd);
		}
	}
	else if(CONF_RADIO == conf_type)
	{
		//make sure the dir of  "conf_wtpd/radio" is exist
		char pathbuf[128];
		memset(pathbuf,0,128);
		autelan_sprintf(pathbuf,"%s%d",CONF_RADIO_PATH,confArg->radioID);	
		//if(debug_print_dwq)
		//	printf("CONF_RADIO pathbuf=%s \t %s=%s\n",pathbuf,confArg.conf_arg,confArg.conf_value);
		if(chkdir == CW_TRUE)
		{
			if(0 != access(pathbuf, F_OK))
			{
				if(0 != mkdir(pathbuf,S_IRWXU))
					CWWTPDebugLog("create path:%s error!",pathbuf);
			}	
		}
		//save the attribute of the configuration of "radio"
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		autelan_sprintf(filepath,"%s/%s",pathbuf,confArg->conf_arg);
		if((fd = autelan_open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			autelan_printf("create file %s failed!\n",filepath);			
		}
		else
		{
			autelan_write(fd,confArg->conf_value,sizeof(confArg->conf_value));
			autelan_close(fd);
		}
	}
	else if(CONF_WLAN ==conf_type)
	{
		//make sure the dir of "conf_wtpd/radio/wlan" is exist
		char pathbuf1[128];
		char pathbuf2[128];
		memset(pathbuf1,0,128);
		memset(pathbuf2,0,128);
		autelan_sprintf(pathbuf1,"%s%d/wlaninfo",CONF_RADIO_PATH,confArg->radioID);		
		autelan_sprintf(pathbuf2,"%s%d/wlaninfo/wlan%d",CONF_RADIO_PATH,confArg->radioID,confArg->wlanID); 			
		//if(debug_print_dwq)
		//	printf("CONF_WLAN pathbuf=%s \t %s=%s\n",pathbuf2,confArg.conf_arg,confArg.conf_value);
		if(chkdir == CW_TRUE)
		{
			if(0 != access(pathbuf1, F_OK))
			{
				if(0 != mkdir(pathbuf1,S_IRWXU))
					CWWTPDebugLog("create path:%s error!",pathbuf1);
			}			
			if(0 != access(pathbuf2, F_OK))
			{
				if(0 != mkdir(pathbuf2,S_IRWXU))
					CWWTPDebugLog("create path:%s error!",pathbuf2);
			}	
		}
		//save the attribute of the configuration of "wlan"		
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		autelan_sprintf(filepath,"%s/%s",pathbuf2,confArg->conf_arg);
		if((fd = autelan_open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			autelan_printf("create file %s failed!\n",filepath);			
		}
		else
		{
			autelan_write(fd,confArg->conf_value,sizeof(confArg->conf_value));
			autelan_close(fd);
		}
	}
	else
	{
		bRet = CW_FALSE;
		dpf("CONF_return_false\n");		
	}

	if(gEMenuConf.e_swtich == 1 && gRunRetry == 0){
		CWSaveEMenuConfig(conf_type, confArg);
	}
	return bRet;
}

CWBool CWBindingSaveChangeWlanTunnelMode(int ChangeTunnelModeCount,ChangeTunnelMode *changeTunnelMode,CWProtocolResultCode *resultCode)
{	
	char cmdline[64]={0};
	if (changeTunnelMode == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	/* Begin: Modified by dongzw for fix to switch tunnel mode between 802.11 & 802.3 & ipip failure bug 2013-03-21 TESTBED-373 */
	if (changeTunnelMode->tunnel_mode==CW_802_DOT_3_TUNNEL || changeTunnelMode->tunnel_mode==CW_802_DOT_11_TUNNEL) {
			autelan_sprintf(cmdline,"brctl delif default ath.%d-%d", changeTunnelMode->radio_id,changeTunnelMode->wlan_id);
			_system(cmdline);

			if (changeTunnelMode->tunnel_mode==CW_802_DOT_3_TUNNEL)	 {	
				autelan_sprintf(cmdline,"echo 2 > /proc/sys/net/ath.%d-%d/vap_splitmac",changeTunnelMode->radio_id,changeTunnelMode->wlan_id);
				_system(cmdline);
			}
			else if (changeTunnelMode->tunnel_mode==CW_802_DOT_11_TUNNEL) {
				autelan_sprintf(cmdline,"echo 1 > /proc/sys/net/ath.%d-%d/vap_splitmac",changeTunnelMode->radio_id,changeTunnelMode->wlan_id);
				_system(cmdline);
			}
	}
	else if(changeTunnelMode->tunnel_mode==CW_802_IPIP_TUNNEL) {
		autelan_sprintf(cmdline,"brctl addif default ath.%d-%d", changeTunnelMode->radio_id,changeTunnelMode->wlan_id);
		_system(cmdline);
		
		autelan_sprintf(cmdline,"echo 1 > /proc/sys/net/ath.%d-%d/vap_ipinip", changeTunnelMode->radio_id,changeTunnelMode->wlan_id);
		_system(cmdline);
		
		autelan_sprintf(cmdline,"echo 0 > /proc/sys/net/ath.%d-%d/vap_splitmac",changeTunnelMode->radio_id,changeTunnelMode->wlan_id);
		_system(cmdline);
	}
	/* End: Modified by dongzw for fix to switch tunnel mode between 802.11 & 802.3 & ipip failure bug 2013-03-21 TESTBED-373 */

	*resultCode = CW_PROTOCOL_SUCCESS;
	return CW_TRUE;
}
