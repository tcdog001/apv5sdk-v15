#ifndef WDSFUNCEXPORT_H
#define WDSFUNCEXPORT_H
#include "CommonDefs.h"
/*WNC_Sunglow 20110607, add to show the format of WDS message call.*/
#define false 0
#define true 1
#define NAME_MAX_SIZE 128
#define PASSWORD_USERNAME_MAX_SIZE 256
struct ProfileSettingInfo
{
	int isModifyAPName;
	int isModifyprofileName;
    char profileName[NAME_MAX_SIZE]; //Profile Name 1.1 3GPP
    uint8 pdpType; //PDP Type 1.1 3GPP
    char apnName[NAME_MAX_SIZE]; //APN Name 1.1 3GPP
    uint32 primaryDnsIpv4AddrPref; //Primary DNS IPv4 Address Preference 1.1 3GPP
    uint32 secondaryDnsIpv4AddrPref; //Secondary DNS IPv4 Address Preference 1.1 3GPP

    uint8 pdpHeaderCompressionType;
    uint8 pdpDataCompressionType;

    //UMTS Requested QoS 1.1 3GPP
    int isUmtsRequestedQosTlv;
    uint8 reqTrafficClass;
    uint32 reqMaxUplinkBitrate;
    uint32 reqMaxDownlinkBitrate;
    uint32 reqGtdUplinkBitrate;
    uint32 reqGtdDownlinkBitrate;
    uint8 reqQosDeliveryOrder;
    uint32 reqMaxSduSize;
    uint8 reqSduErrorRatio;
    uint8 reqResidualBerRatio;
    uint8 reqDeliveryErroneousSdu;
    uint32 reqTransferDelay;
    uint32 reqTrafficHandlingPriority;

    //UMTS Minimum QoS 1.1 3GPP
    int isUmtsMinimumQosTlv;
    uint8 minTrafficClass;
    uint32 minMaxUplinkBitrate;
    uint32 minMaxDownlinkBitrate;
    uint32 minGtdUplinkBitrate;
    uint32 minGtdDownlinkBitrate;
    uint8 minQosDeliveryOrder;
    uint32 minMaxSduSize;
    uint8 minSduErrorRatio;
    uint8 minResidualBerRatio;
    uint8 minDeliveryErroneousSdu;
    uint32 minTransferDelay;
    uint32 minTrafficHandlingPriority;

    //GPRS Requested QoS 1.1 3GPP
    int isGprsRequestedQosTlv;
    uint32 reqPrecedenceClass;
    uint32 reqDelayClass;
    uint32 reqReliabilityClass;
    uint32 reqPeakThroughputClass;
    uint32 reqMeanThroughputClass;

    //GPRS Minimum QoS 1.1 3GPP
    int isGprsMinimumQosTlv;
    uint32 minPrecedenceClass;
    uint32 minDelayClass;
    uint32 minReliabilityClass;
    uint32 minPeakThroughputClass;
    uint32 minMeanThroughputClass;

	int isModifyUser;
    char username[PASSWORD_USERNAME_MAX_SIZE]; //User name 1.1 3GPP
    char password[PASSWORD_USERNAME_MAX_SIZE]; //Password 1.1 3GPP
    uint8 authPref; //Authentication Preference 1.1 3GPP
    uint32 ipv4AddrPref; //IPv4 Address Preference 1.1 3GPP
    uint8 pcscfAddrUsingPco;//P-CSCF Address using PCO Flag 1.3 3GPP

    uint8 pdpAccessControlType;
    uint8 pCscfAddressUsingDhcpFlagType;
    uint8 imCnFlag;
    uint8 pdpContextNumber;
    uint8 pdpContextSecondaryFLag;
    uint8 primaryId;
};

static void initProfileSettingInfo(struct ProfileSettingInfo* info)
{
	if(!info)
		return;
	info->isModifyprofileName = -1;
	info->isModifyAPName = -1;
    memset(info->profileName, 0, NAME_MAX_SIZE); //Profile Name 1.1 3GPP
    info->pdpType = -1; //PDP Type 1.1 3GPP
    memset(info->apnName, 0, NAME_MAX_SIZE); //APN Name 1.1 3GPP
    info->primaryDnsIpv4AddrPref = 0; //Primary DNS IPv4 Address Preference 1.1 3GPP
    info->secondaryDnsIpv4AddrPref = 0; //Secondary DNS IPv4 Address Preference 1.1 3GPP
    info->pdpHeaderCompressionType = -1;
    info->pdpDataCompressionType = -1;

    //UMTS Requested QoS 1.1 3GPP
    info->isUmtsRequestedQosTlv = false;

    //UMTS Minimum QoS 1.1 3GPP
    info->isUmtsMinimumQosTlv = false;

    //GPRS Requested QoS 1.1 3GPP
    info->isGprsRequestedQosTlv = false;

    //GPRS Minimum QoS 1.1 3GPP
    info->isGprsMinimumQosTlv = false;

	info->isModifyUser = -1;
    memset(info->username, 0, PASSWORD_USERNAME_MAX_SIZE); //User name 1.1 3GPP
    memset(info->password, 0, PASSWORD_USERNAME_MAX_SIZE); //Password 1.1 3GPP
    info->authPref = -1; //Authentication Preference 1.1 3GPP
    info->ipv4AddrPref = -1; //IPv4 Address Preference 1.1 3GPP
    info->pcscfAddrUsingPco = -1;//P-CSCF Address using PCO Flag 1.3 3GPP

    info->pdpAccessControlType = -1;
    info->pCscfAddressUsingDhcpFlagType = -1;
    info->imCnFlag = -1;
    info->pdpContextNumber = -1;
    info->pdpContextSecondaryFLag = -1;
    info->primaryId = -1;
}

static void setUmtsRequestedQosTlv(struct ProfileSettingInfo* info,
                            uint32 reqTrafficClass,
                            uint32 reqMaxUplinkBitrate,
                            uint32 reqMaxDownlinkBitrate,
                            uint32 reqGtdUplinkBitrate,
                            uint32 reqGtdDownlinkBitrate,
                            uint32 reqQosDeliveryOrder,
                            uint32 reqMaxSduSize,
                            uint32 reqSduErrorRatio,
                            uint32 reqResidualBerRatio,
                            uint32 reqDeliveryErroneousSdu,
                            uint32 reqTransferDelay,
                            uint32 reqTrafficHandlingPriority)
{
	if(!info)
		return;
    info->isUmtsMinimumQosTlv = true;
    info->reqTrafficClass = reqTrafficClass;
    info->reqMaxUplinkBitrate = reqMaxUplinkBitrate;
    info->reqMaxDownlinkBitrate = reqMaxDownlinkBitrate;
    info->reqGtdUplinkBitrate = reqGtdUplinkBitrate;
    info->reqGtdDownlinkBitrate = reqGtdDownlinkBitrate;
    info->reqQosDeliveryOrder = reqQosDeliveryOrder;
    info->reqMaxSduSize = reqMaxSduSize;
    info->reqSduErrorRatio = reqSduErrorRatio;
    info->reqResidualBerRatio = reqResidualBerRatio;
    info->reqDeliveryErroneousSdu = reqDeliveryErroneousSdu;
    info->reqTransferDelay = reqTransferDelay;
    info->reqTrafficHandlingPriority = reqTrafficHandlingPriority;
}

static void setUmtsMinimumQosTlv(struct ProfileSettingInfo* info,
                        uint32 minTrafficClass,
                        uint32 minMaxUplinkBitrate,
                        uint32 minMaxDownlinkBitrate,
                        uint32 minGtdUplinkBitrate,
                        uint32 minGtdDownlinkBitrate,
                        uint32 minQosDeliveryOrder,
                        uint32 minMaxSduSize,
                        uint32 minSduErrorRatio,
                        uint32 minResidualBerRatio,
                        uint32 minDeliveryErroneousSdu,
                        uint32 minTransferDelay,
                        uint32 minTrafficHandlingPriority)
{
	if(!info)
		return;
    info->isUmtsMinimumQosTlv = true;
    info->minTrafficClass = minTrafficClass;
    info->minMaxUplinkBitrate = minMaxUplinkBitrate;
    info->minMaxDownlinkBitrate = minMaxDownlinkBitrate;
    info->minGtdUplinkBitrate = minGtdUplinkBitrate;
    info->minGtdDownlinkBitrate = minGtdDownlinkBitrate;
    info->minQosDeliveryOrder = minQosDeliveryOrder;
    info->minMaxSduSize = minMaxSduSize;
    info->minSduErrorRatio = minSduErrorRatio;
    info->minResidualBerRatio = minResidualBerRatio;
    info->minDeliveryErroneousSdu = minDeliveryErroneousSdu;
    info->minTransferDelay = minTransferDelay;
    info->minTrafficHandlingPriority = minTrafficHandlingPriority;
}

static void setGprsRequestedQosTlv(struct ProfileSettingInfo* info,
                            uint32 reqPrecedenceClass,
                            uint32 reqDelayClass,
                            uint32 reqReliabilityClass,
                            uint32 reqPeakThroughputClass,
                            uint32 reqMeanThroughputClass)
{
	if(!info)
		return;
    info->isGprsRequestedQosTlv = true;
    info->reqPrecedenceClass = reqPrecedenceClass;
    info->reqDelayClass = reqDelayClass;
    info->reqReliabilityClass = reqReliabilityClass;
    info->reqPeakThroughputClass = reqPeakThroughputClass;
    info->reqMeanThroughputClass = reqMeanThroughputClass;
}

static void setGprsMinimumQosTlv(struct ProfileSettingInfo* info,
                        uint32 minPrecedenceClass,
                        uint32 minDelayClass,
                        uint32 minReliabilityClass,
                        uint32 minPeakThroughputClass,
                        uint32 minMeanThroughputClass)
{
	if(!info)
		return;
    info->isGprsMinimumQosTlv = true;
    info->minPrecedenceClass = minPrecedenceClass;
    info->minDelayClass = minDelayClass;
    info->minReliabilityClass = minReliabilityClass;
    info->minPeakThroughputClass = minPeakThroughputClass;
    info->minMeanThroughputClass = minMeanThroughputClass;
}


//This file is called for WDS Message

/*********************************************
QMI_WDS_EVENT_REPORT_IND
    This message indicates the WDS connection related state change.
*********************************************/
struct WDSEventReportInfo
{
    uint32 txOkCount;
    uint32 rxOkCount;
    uint32 txErrCount;
    uint32 rxErrCount;
    uint32 txOflCount;
    uint32 rxOflCount;
    uint32 currentChannelTxRate;
    uint32 currentChannelRxRate;
    uint8 dataBearerTech;
    uint8 dormancyStatus;
    uint8 currentNw;
    uint32 ratMask;
    uint32 soMask;
};

static void initWDSEventReportInfo(struct WDSEventReportInfo* info)
{
	if(!info)
		return;
    info->txOkCount = -1;
    info->rxOkCount = -1;
    info->txErrCount = -1;
    info->rxErrCount = -1;
    info->txOflCount = -1;
    info->rxOflCount = -1;
    info->currentChannelTxRate = -1;
    info->currentChannelRxRate = -1;
    info->dataBearerTech = -1;
    info->dormancyStatus = -1;
    info->currentNw = -1;
    info->ratMask = -1;
    info->soMask = -1;
}

/*********************************************
QMI_WDS_START_NETWORK_INTERFACE
    This message activates a packet data session (if not already started)
on behalf of the requesting control point.
*********************************************/
struct StartNetworkInfo
{
    uint8 techPref; //Technology Preference 1.1 3GPP, 3GPP2
    uint8 profile3gppIndex; //3GPP Configured Profile Identifier 1.1 3GPP
    uint8 profile3gpp2Index; //3GPP2 Configured Profile Identifier 1.1 3GPP2
    char primaryDnsIpv4AddrPref[256]; //Primary DNS Address Preference 1.1 3GPP
    char secondaryDnsIpv4AddrPref[256]; //Secondary DNS Address Preference 1.1 3GPP
    char primaryNbnsAddrPref[256]; //Primary NBNS Address Preference 1.1 3GPP
    char secondaryNbnsAddrPref[256]; //Secondary NBNS Address Preference 1.1 3GPP
	bool isApnName;
    char apnName[NAME_MAX_SIZE]; //Context APN Name 1.1 3GPP
    char ipv4AddrPref[256]; //IP Address Preference 1.1 3GPP
    uint8 authPref; //Authentication Preference 1.1 3GPP, 3GPP2
	bool isUserName;
    char username[PASSWORD_USERNAME_MAX_SIZE]; //Username 1.1 3GPP, 3GPP2
    char password[PASSWORD_USERNAME_MAX_SIZE]; //Password 1.1 3GPP, 3GPP2
    uint8 pdpType;
    uint8 ipFamilyPref; //IP Family Preference 1.7 3GPP, 3GPP2
    uint8 callType; //Call Type Identifier 1.8 3GPP, 3GPP2
    
    char connected_cmd[64];			//arthur added 20140106
    char disconnected_cmd[64];		//arthur added 20140106
	
    /*
    int extTechPreference; //Extended Technology Preference 1.8 3GPP, 3GPP2
    */
};

static void initStartNetworkInfo(struct StartNetworkInfo* info)
{
	if(!info)
		return;
    info->techPref = -1; //Technology Preference 1.1 3GPP, 3GPP2
    info->profile3gppIndex = -1; //3GPP Configured Profile Identifier 1.1 3GPP
    info->profile3gpp2Index = -1; //3GPP2 Configured Profile Identifier 1.1 3GPP2
    memset(info->primaryDnsIpv4AddrPref, 0, 256); //Primary DNS Address Preference 1.1 3GPP
    memset(info->secondaryDnsIpv4AddrPref, 0, 256); //Secondary DNS Address Preference 1.1 3GPP
    memset(info->primaryNbnsAddrPref, 0, 256); //Primary NBNS Address Preference 1.1 3GPP
    memset(info->secondaryNbnsAddrPref, 0, 256); //Secondary NBNS Address Preference 1.1 3GPP
	info->isApnName = false;
    memset(info->apnName, 0, NAME_MAX_SIZE); //Context APN Name 1.1 3GPP
    memset(info->ipv4AddrPref, 0, 256); //IP Address Preference 1.1 3GPP
    info->authPref = -1; //Authentication Preference 1.1 3GPP, 3GPP2
	info->isUserName = false;
    memset(info->username, 0, PASSWORD_USERNAME_MAX_SIZE); //Username 1.1 3GPP, 3GPP2
    memset(info->password, 0, PASSWORD_USERNAME_MAX_SIZE); //Password 1.1 3GPP, 3GPP2
    info->ipFamilyPref = -1; //IP Family Preference 1.7 3GPP, 3GPP2
    info->callType = 1; //Call Type Identifier 1.8 3GPP, 3GPP2
}

/*********************************************
QMI_WDS_PKT_SRVC_STATUS_IND
    This message indicates a change in the current packet data connection status.
*********************************************/
enum ConnectionStatus{
    CON_WDS_PKT_DATA_DISCONNECTED = 0x01,
    CON_WDS_PKT_DATA_CONNECTED = 0x02,
    CON_WDS_PKT_DATA_SUSPENDED = 0x03,
    CON_WDS_PKT_DATA_AUTHENTICATING = 0x04
};

/*********************************************
QMI_WDS_GET_PROFILE_LIST
    This message retrieves a list of configured profiles present on the wireless device.
*********************************************/
struct ProfileInstance
{
    uint8 m_profileType;
    uint8 m_profileIndex;
    uint8 m_profileNameLen;
    const char* m_profileName;
};

#endif // DMSFUNEXPORT_H

