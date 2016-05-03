/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: LocalManagerDefinition.h
* description:  the header file for the definition of data parameter
* 
*
* 
************************************************************************************/
#ifndef _LOCALMANAGERDEFINITION_H
#define _LOCALMANAGERDEFINITION_H

#include "Manager.h"


char *GetHardwareVersion();
char *GetSoftwareVersion();
char *GetOUI();
char *GetProductClass();
char *GetSerialNumber();
char *GetManufacturer();



char *GetWorkTime();
char *GetCPU();
char *GetTemp();

char *GetIPAddress();
char *GetSubnetMask();
char *GetMACAddress();
char *GetLANStatus();
char *GetLANMaxBitRate();

char *GetSuccessCount();
char *GetFailureCount();
char *GetAverageResponseTime();
char *GetMinimumResponseTime();
char *GetMaximumResponseTime();

#if 0
char *GetDeviceSummary();
char *GetHardwareVersion();
void SetHardwareVersion(char *);

char *GetSoftwareVersion();
char *GetConnectionRequestURL();
char *GetParameterKey();

char *GetManufacturer();
char *GetManufacturerOUI();
char *GetModelName();
char *GetDescription();
char *GetProductClass();
char *GetSerialNumber();
char *GetProvisioningCode();
char *GetUpTime();


char *GetPeriodicInfTime();
#endif
char *GetPeriodicInfEnable();
char *GetPeriodicInfInterval();


int SetEnable(char *str_arg,char *str_szval);
int SetChannel(char *str_arg,char *str_szval);
int SetSSID(char *str_arg,char *str_szval);
//int SetSSIDAdvertisementEnable(char *str_arg,char *str_szval);
int SetSSIDHide(char *str_arg,char *str_szval);
int  SetSSIDManage(char *str_arg,char *str_szval);
int  SetStandard(char *str_arg,char *str_szval);
int  SetAPModuleEnable(char *str_arg,char *str_szval);
int SetRadioEnabled(char *str_arg,char *str_szval);
int  SetPowerlevel(char *str_arg,char *str_szval);
int  SetWEPEncryptionLevel(char *str_arg,char *str_szval);
int  SetBasicAuthencationMode(char *str_arg,char *str_szval);
int SetWPAEncryptionModes(char *str_arg,char *str_szval);
int SetBeaconType(char *str_arg,char *str_szval);
int SetWEPKey(char *str_arg,char *str_szval); 
int SetPreSharedKey(char *str_arg,char *str_szval);
int SetKeyPassphrase(char *str_arg,char *str_szval);
int SetSSIDAdvertisementEnable(char *str_arg,char *str_szval);

int SetIPInterface1IPAddr(char *str_arg, char *str_szval);
int SetIPInterface1SubnetMask(char *str_arg,char *str_szval);

char * GetWEPKey(char *str_arg);
char * GetPreSharedKey(char *str_arg);
char * GetKeyPassphrase(char *str_arg);


char * GetAssociatedDeviceMACAddress(char *str_arg);
char * GetAssociatedDeviceIPAddress(char *str_arg);
char * GetAssociatedDeviceAuthenticationState(char *str_arg);
char * GetCTReceiveRate(char *str_arg);
char * GetCTSendRate(char *str_arg);
char * GetTotalBytes(char *str_arg);
char * GetCTReceivePower(char *str_arg);
char * GetCTReceiveNoise(char *str_arg);
char * GetCTPacketsError(char *str_arg);


char * GetCRCError(char *str_arg);
char * GetPHYError(char *str_arg);
char * GetMICError(char *str_arg);
char * GetKEYError(char *str_arg);

char * GetResource_using(char *str_arg);
char * GetAuthenticationUser(char *str_arg);
char * GetCTLANTotalBytesSent(char *str_arg);
char * GetCTLANTotalBytesReceived(char *str_arg);

char * GetCTWANTotalBytesSent(char *str_arg);
char * GetCTWANTotalBytesReceived(char *str_arg);

char * GetTotalBytesSentoper(char *str_arg);
char * GetLANPacketsError(char *str_arg);
char * GetWANPacketsError(char *str_arg);
char * GetBreakTime(char *str_arg);
char * GetAskPass(char *str_arg);
char * GetResponsePass(char *str_arg);
char * GetSuccessPass(char *str_arg);


char * GetTotalMem(char *str_arg);
char * GetUseMem(char *str_arg);
char * GetFlash(char *str_arg);
char * GetFreeFlash(char *str_arg);
char * GeWorkMode(char *str_arg);
char * GetMaxUser(char *str_arg);
char * GetIsolation(char *str_arg);
char * GetLogIP(char *str_arg);
char * GetVlanEnable(char *str_arg);
char * GetVlanVid(char *str_arg);

char *GetIPAddress(char *str_arg);
char *GetSubnetMask(char *str_arg);
char *GetDefaultGateway(char *str_arg);

char *GetMACAddress(char *str_arg);
char *GetLANStatus(char *str_arg);
char *GetLANMaxBitRate(char *str_arg);


char *GetIPInterface1Enable();
char *GetIPInterface1IPAddr();
char *GetIPInterface1SubnetMask();
char *GetIPInterface1AddressingType();


char *GetNTPServer1();
char *GetNTPServer2();
char *GetLocalTimeZone();
char *GetCurrentLocalTime();

char * GetDNSServersMode();
char * GetDNSServers1();
char * GetDNSServers2();

char * GetEnable(char *str_arg);
char * GetStatus(char *str_arg);
char * GetBSSID(char *str_arg);
char * GetChannel(char *str_arg);
char * GetChannelsInUse(char *str_arg);

char * GetSSID(char *str_arg);
char * GetSSIDHide(char *str_arg);

char * GetSSIDManage(char *str_arg);
char * GetWEPKeyIndex(char *str_arg);
char * GetBeaconType(char *str_arg);
char * GetPowerlevel(char *str_arg);
char * GetPowerValue(char *str_arg);

char * GetAPModuleEnable(char *str_arg);
char * GetRadioEnabled(char *str_arg);
char * GetWPAAuthencationMode(char *str_arg);
char * GetWPAEncryptionModes(char *str_arg);
char * GetBasicAuthencationMode(char *str_arg);
char * GetWEPEncryptionLevel(char *str_arg);
char * GetStandard(char *str_arg);
char * GetBasicEncryptionModes(char *str_arg);
char * GetReceiveNoise(char *str_arg);
char * GetLANTotalBytesSent(char *str_arg);
char * GetLANTotalBytesReceived(char *str_arg);
char * GetWANTotalBytesSent(char *str_arg);
char * GetWANTotalBytesReceived(char *str_arg);
char * GetLANTotalPacketsSent(char *str_arg);
char * GetLANTotalPacketsReceived(char *str_arg);
char * GetWANTotalPacketsSent(char *str_arg);
char * GetWANTotalPacketsReceived(char *str_arg);
char * GetTotalAssociations(char *str_arg);
char * GetPossibleChannels(char *str_arg);
char * GetAutoChannelEnable(char *str_arg);
char * GetSSIDAdvertisementEnable(char *str_arg);
char * GetWEPKey(char *str_arg);
char * GetPreSharedKey(char *str_arg);
char * GetKeyPassphrase(char *str_arg);
char * GetAssociatedDeviceMACAddress(char *str_arg);
char * GetAssociatedDeviceIPAddress(char *str_arg);
char * GetAssociatedDeviceAuthenticationState(char *str_arg);
char * GetCTReceiveRate(char *str_arg);
char * GetCTSendRate(char *str_arg);
char * GetTotalBytes(char *str_arg);
char * GetCTReceivePower(char *str_arg);
char * GetCTReceiveNoise(char *str_arg);
char * GetCTPacketsError(char *str_arg);
char * GetCpeURL(char *str_arg);
char * SetCpeURL(char *str_arg, char *str_szval);
char * GetSfVer(char *str_arg);
char * GetHdVer(char *str_arg);

char * GetCRCError(char *str_arg);
char * GetPHYError(char *str_arg);
char * GetMICError(char *str_arg);
char * GetKEYError(char *str_arg);
char * GetResource_using(char *str_arg);
char * GetAuthenticationUser(char *str_arg);
char * GetCTLANTotalBytesSent(char *str_arg);
char * GetCTLANTotalBytesReceived(char *str_arg);

char * GetCTWANTotalBytesSent(char *str_arg);
char * GetCTWANTotalBytesReceived(char *str_arg);

char * GetTotalBytesSentoper(char *str_arg);
char * GetLANPacketsError(char *str_arg);
char * GetWANPacketsError(char *str_arg);
char * GetBreakTime(char *str_arg);
char * GetAskPass(char *str_arg);
char * GetResponsePass(char *str_arg);
char * GetSuccessPass(char *str_arg);
char * GetParKey(char *str_arg);
char * GetWanIp(char *str_arg);


char *GetMACAddress(char *str_arg);
int SetDiagnosticsState(char *str_arg,char *str_szval);
int SetInterface(char *str_arg,char *str_szval);
int SetHost(char *str_arg,char *str_szval);
int SetNumberOfRepetions(char *str_arg,char *str_szval);
int SetTimeout(char *str_arg,char *str_szval);
int SetDataBlockSize(char *str_arg,char *str_szval);
int SetDSCP(char *str_arg,char *str_szval);
int SetMaxUser(char *str_arg,char *str_szval);
char * GetMaxUser(char *str_arg);
char * GetFreeFlash(char *str_arg);
int SetIsolation(char *str_arg,char *str_szval);
char * GeWorkMode(char *str_arg);
int SetLogIP(char *str_arg,char *str_szval);
char *GetTeleACEnable(char *str_arg);
char *GetTeleACPass(char *str_arg);
char *GetDiagnosticsState(char *str_arg);

int SetMonEnable(char *str_arg,char *str_szval);
char* GetMonEnable(char *str_arg);
char* GetMonCount(char *str_arg);

int SetMonParaList(char *str_arg,char *str_szval);
char *GetMonParaList(char *str_arg);
int SetMonTimeList(char *str_arg,char *str_szval);
char *GetMonTimeList(char *str_arg);

int SetAlarmEnable(char *str_arg,char *str_szval);
char *GetAlarmEnable(char *str_arg);
char *GetAlarmCount(char *str_arg);
int SetAlarmParaList(char *str_arg,char *str_szval);
char *GetAlarmParaList(char *str_arg);
int SetAlarmTimeList(char *str_arg,char *str_szval);
char *GetAlarmTimeList(char *str_arg);
int SetAlarmLMax(char *str_arg,char *str_szval);
char *GetAlarmLMax(char *str_arg);
int SetAlarmLMin(char *str_arg,char *str_szval);
char *GetAlarmLMin(char *str_arg);
int SetAlarmMode(char *str_arg,char *str_szval);
char *GetAlarmMode(char *str_arg);


// internal table definition
// cf. ManagerDefinition.h for structure

#if 0
STRUCT_MANAGER_INTERNALVALUES parameters_table[] = {
{NULL                 , GetDeviceSummary,        STRING, "Device.DeviceSummary",                         PASSIVE, READ, 0},
{SetHardwareVersion   , GetHardwareVersion,      STRING, "Device.DeviceInfo.HardwareVersion",            PASSIVE, READWRITE, 0},
{NULL                 , GetSoftwareVersion,      STRING, "Device.DeviceInfo.SoftwareVersion",            PASSIVE, READ, 0},
{NULL                 , GetConnectionRequestURL, STRING, "Device.ManagementServer.ConnectionRequestURL", PASSIVE, READ, 0},
{NULL                 , GetParameterKey,         STRING, "Device.ManagementServer.ParameterKey",         PASSIVE, READ, 0},

{NULL                 , GetPeriodicInfTime,      STRING, "Device.ManagementServer.PeriodicInformTime",   ACTIVE, READ, 0},
{NULL                 , GetPeriodicInfEnable,    STRING, "Device.ManagementServer.PeriodicInformEnable", ACTIVE, READ, 0},
{NULL                 , GetPeriodicInfInterval,  STRING, "Device.ManagementServer.PeriodicInformInterval",ACTIVE, READ, 0},

{NULL                 , GetManufacturer,         STRING, "Device.DeviceInfo.Manufacturer",               PASSIVE, READ, 0},
{NULL                 , GetManufacturerOUI,      STRING, "Device.DeviceInfo.ManufacturerOUI",            PASSIVE, READ, 0},
{NULL                 , GetModelName,            STRING, "Device.DeviceInfo.ModelName",                  PASSIVE, READ, 0},
{NULL                 , GetDescription,          STRING, "Device.DeviceInfo.Description",                PASSIVE, READ, 0},
{NULL                 , GetProductClass,         STRING, "Device.DeviceInfo.ProductClass",               PASSIVE, READ, 0},
{NULL                 , GetSerialNumber,         STRING, "Device.DeviceInfo.SerialNumber",               PASSIVE, READ, 0},
{NULL                 , GetProvisioningCode,     STRING, "Device.DeviceInfo.ProvisioningCode",           NONE, READ, 0},

{NULL                 , GetUpTime          ,     STRING, "Device.DeviceInfo.UpTime",                     NONE, READ, 0},

{ NULL                ,GetIPInterface1Enable,    STRING, "Device.LANDevice.INSTANCE1.LANHostConfigManagement.IPInterface.INSTANCE1.Enable",       NONE, READ, 0},
{NULL                 ,GetIPInterface1IPAddr,    STRING, "Device.LANDevice.INSTANCE1.LANHostConfigManagement.IPInterface.INSTANCE1.IPInterfaceIPAddress",       NONE, READ, 0},
{NULL                 ,GetIPInterface2Enable,    STRING, "Device.LANDevice.INSTANCE2.LANHostConfigManagement.IPInterface.INSTANCE1.Enable",       NONE, READ, 0},
{NULL                 ,GetIPInterface2IPAddr,    STRING, "Device.LANDevice.INSTANCE2.LANHostConfigManagement.IPInterface.INSTANCE1.IPInterfaceIPAddress",       NONE, READ, 0},

};
#endif
//////////////////update by wangjr 091124//////////////////////////////////////
static STRUCT_MANAGER_INTERNALVALUES parameters_table[] = {

//landevice
//#if 0
{SetIPInterface1IPAddr		,GetIPAddress,	 STRING, "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress",		ACTIVE, READWRITE, 0,1},
{SetIPInterface1SubnetMask	,GetSubnetMask,	 STRING, "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask", 	  NONE, READWRITE, 0,1},



//#if 0  ath0
{NULL	,GetLANStatus,  STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Status",	  NONE, READ, 0,1},
{NULL	,GetLANMaxBitRate,  STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MaxBitRate",	  NONE, READ, 0,1},
{NULL	,GetMACAddress,	 STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddress",	  NONE, READ, 0,1},
{SetEnable	  , GetEnable, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable", NONE, READWRITE, 0,1},
{NULL		  , GetStatus, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Status", NONE, READ, 0,1},
{NULL		  , GetBSSID,  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BSSID",  NONE, READ, 0,1},
{SetChannel   , GetChannel,STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Channel", ACTIVE, READWRITE, 0,1},
{NULL				  , GetChannelsInUse,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.GetChannelsInUse",	 PASSIVE, READ, 0,1},
{SetSSID	  , GetSSID,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSID",	ACTIVE, READWRITE, 0,1},

{SetRadioEnabled	  , GetRadioEnabled,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.RadioEnabled",	NONE, READWRITE, 0,1},
{NULL , GetWEPKeyIndex, 	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.GetWEPKeyIndex",   NONE, READ, 0,1},
{NULL , GetBeaconType,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BeaconType", NONE, READ, 0,1},
{SetWEPEncryptionLevel		, GetWEPEncryptionLevel,		STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPEncryptionLevel", 		   NONE, READWRITE, 0,1},
{SetBasicAuthencationMode	, GetBasicAuthencationMode, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BasicAuthencationMode",NONE, READWRITE, 0,1},
{SetWPAEncryptionModes		, GetWPAEncryptionModes,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WPAEncryptionModes",	NONE, READWRITE, 0,1},
{NULL , GetStandard,			STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Standard",				   NONE, READ, 0,1},
{NULL , GetBasicEncryptionModes,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BasicEncryptionModes",   NONE, READ, 0,1},
{NULL , GetPossibleChannels,		 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.PossibleChannels",				 NONE, READ, 0,1},
{NULL , GetAutoChannelEnable,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AutoChannelEnable",		   NONE, READ, 0,1},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPKey.1.WEPKey",NONE, READ, 0,1},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPKey.2.WEPKey",NONE, READ, 0,1},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPKey.3.WEPKey",NONE, READ, 0,1},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPKey.4.WEPKey",NONE, READ, 0,1},
{NULL , GetPreSharedKey,    STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.PreSharedKey.1.PreSharedKey", 	  NONE, READ, 0,1},
{NULL , GetKeyPassphrase,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.PreSharedKey.1.KeyPassphrase",		NONE, READ, 0,1},
{SetMaxUser   , GetMaxUser,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.MaxUser",	NONE, READWRITE, 0,1},
{SetIsolation	  , GetIsolation,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Isolation",	NONE, READWRITE, 0,1},
{NULL	  , GetVlanEnable,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.VlanEnable",	NONE, READ, 0,1},
{NULL	  , GetVlanVid,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.VlanVid",	NONE, READ, 0,1},
{SetSSIDHide	  , GetSSIDHide,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_SSIDHide",	NONE, READWRITE, 0,1},
{SetSSIDManage	  , GetSSIDManage,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Manage",	NONE, READWRITE, 0,1},
{SetPowerlevel	  , GetPowerlevel,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Powerlevel",	NONE, READWRITE, 0,1},
{NULL	  , GetPowerValue,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_PowerValue",	NONE, READ, 0,1},
{SetAPModuleEnable	  , GetAPModuleEnable,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_APModuleEnable",	NONE, READWRITE, 0,1},
{NULL				  , GetAuthenticationUser,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.AuthenticationUser",	NONE, READ, 0,1},
{NULL				  , GetReceiveNoise,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_ReceiveNoise",	NONE, READ, 0,1},
{NULL				  , GetLANTotalBytesSent,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_LAN-TotalBytesSent",   NONE, READ, 0,1},
{NULL				  , GetLANTotalBytesReceived,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_LAN-TotalBytesReceived",   NONE, READ, 0,1},
{NULL				  , GetWANTotalBytesSent,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_WAN-TotalBytesSent",   NONE, READ, 0,1},
{NULL				  , GetWANTotalBytesReceived,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_WAN-TotalBytesReceived",   NONE, READ, 0,1},
{NULL				  , GetLANTotalPacketsSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_LAN-TotalPacketsSent",	NONE, READ, 0,1},
{NULL				  , GetLANTotalPacketsReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_LAN-TotalPacketsReceived",	NONE, READ, 0,1},
{NULL				  , GetWANTotalPacketsSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_WAN-TotalPacketsSent",	NONE, READ, 0,1},
{NULL				  , GetWANTotalPacketsReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_WAN-TotalPacketsReceived",	NONE, READ, 0,1},
{NULL				  , GetTotalAssociations,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.TotalAssociations",	NONE, READ, 0,1},

{NULL				  , GetAssociatedDeviceMACAddress,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.AssociatedDeviceMACAddress",   NONE, READ, 0,1},
{NULL				  , GetAssociatedDeviceIPAddress,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.AssociatedDeviceIPAddress",	NONE, READ, 0,1},
{NULL				  , GetAssociatedDeviceAuthenticationState,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.AssociatedDeviceAuthenticationState",	NONE, READ, 0,1},
{NULL				  , GetCTReceiveRate, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_ReceiveRate",	NONE, READ, 0,1},
{NULL				  , GetCTSendRate,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_SendRate", NONE, READ, 0,1},
{NULL				  , GetTotalBytes,	  STRING,     "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_TotalBytes", NONE, READ, 0,1},
{NULL				  , GetCTReceivePower,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_ReceivePower", NONE, READ, 0,1},
{NULL				  , GetCTReceiveNoise,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_ReceiveNoise", NONE, READ, 0,1},
{NULL				  , GetCTPacketsError,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_PacketsError", NONE, READ, 0,1},


{NULL				  , GetCRCError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.CRCError",   PASSIVE, READ, 0,1},
{NULL				  , GetPHYError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.PHYError",   NONE, READ, 0,1},
{NULL				  , GetMICError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.MICError",   NONE, READ, 0,1},
{NULL				  , GetKEYError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.KEYError",   PASSIVE, READ, 0,1},
{NULL				  , GetResource_using,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.Resource_using",   NONE, READ, 0,1},
{NULL				  , GetCTLANTotalBytesSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.LAN-TotalBytesSent",	NONE, READ, 0,1},
{NULL				  , GetCTLANTotalBytesReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.LAN-TotalBytesReceived",	NONE, READ, 0,1},
{NULL				  , GetCTWANTotalBytesSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.WAN-TotalBytesSent",	NONE, READ, 0,1},
{NULL				  , GetCTWANTotalBytesReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.WAN-TotalBytesReceived",	NONE, READ, 0,1},
{NULL				  , GetLANPacketsError,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.LAN-PacketsError",	NONE, READ, 0,1},
{NULL				  , GetWANPacketsError,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.WAN-PacketsError",	NONE, READ, 0,1},
{NULL				  , GetBreakTime,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.BreakTime",	NONE, READ, 0,1},
{NULL				  , GetAskPass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.AskPass", NONE, READ, 0,1},
{NULL				  , GetResponsePass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.ResponsePass",	NONE, READ, 0,1},
{NULL				  , GetSuccessPass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.SuccessPass", NONE, READ, 0,1},
//#endif



//#if 0 // ath1
{NULL	,GetLANStatus,  STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.2.Status",	  NONE, READ, 0,0},
{NULL	,GetLANMaxBitRate,  STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.2.MaxBitRate",	  NONE, READ, 0,0},
{NULL	,GetMACAddress,	 STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.2.MACAddress",	  NONE, READ, 0,0},
{SetEnable	  , GetEnable, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.Enable", NONE, READWRITE, 0,0},
{NULL		  , GetStatus, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.Status", NONE, READ, 0,0},
{NULL		  , GetBSSID,  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.BSSID",  NONE, READ, 0,0},
{SetChannel   , GetChannel,STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.Channel", ACTIVE, READWRITE, 0,0},
{NULL				  , GetChannelsInUse,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.GetChannelsInUse",	 PASSIVE, READ, 0,0},
{SetSSID	  , GetSSID,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.SSID",	ACTIVE, READWRITE, 0,0},

{SetRadioEnabled	  , GetRadioEnabled,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.RadioEnabled",	NONE, READWRITE, 0,0},
{NULL , GetWEPKeyIndex, 	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.GetWEPKeyIndex",   NONE, READ, 0,0},
{NULL , GetBeaconType,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.BeaconType", NONE, READ, 0,0},
{SetWEPEncryptionLevel		, GetWEPEncryptionLevel,		STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.WEPEncryptionLevel", 		   NONE, READWRITE, 0,0},
{SetBasicAuthencationMode	, GetBasicAuthencationMode, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.BasicAuthencationMode",NONE, READWRITE, 0,0},
{SetWPAEncryptionModes		, GetWPAEncryptionModes,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.WPAEncryptionModes",	NONE, READWRITE, 0,0},
{NULL , GetStandard,			STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.Standard",				   NONE, READ, 0,0},
{NULL , GetBasicEncryptionModes,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.BasicEncryptionModes",   NONE, READ, 0,0},
{NULL , GetPossibleChannels,		 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.PossibleChannels",				 NONE, READ, 0,0},
{NULL , GetAutoChannelEnable,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AutoChannelEnable",		   NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.WEPKey.1.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.WEPKey.2.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.WEPKey.3.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.WEPKey.4.WEPKey",NONE, READ, 0,0},
{NULL , GetPreSharedKey,    STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.PreSharedKey.1.PreSharedKey", 	  NONE, READ, 0,0},
{NULL , GetKeyPassphrase,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.PreSharedKey.1.KeyPassphrase",		NONE, READ, 0,0},
{SetMaxUser   , GetMaxUser,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.MaxUser",	NONE, READWRITE, 0,0},
{SetIsolation	  , GetIsolation,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.Isolation",	NONE, READWRITE, 0,0},
{NULL	  , GetVlanEnable,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.VlanEnable",	NONE, READ, 0,0},
{NULL	  , GetVlanVid,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.VlanVid",	NONE, READ, 0,0},
{SetSSIDHide	  , GetSSIDHide,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_SSIDHide",	NONE, READWRITE, 0,0},
{SetSSIDManage	  , GetSSIDManage,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Manage",	NONE, READWRITE, 0,0},
{SetPowerlevel	  , GetPowerlevel,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Powerlevel",	NONE, READWRITE, 0,0},
{NULL	  , GetPowerValue,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_PowerValue",	NONE, READ, 0,0},
{SetAPModuleEnable	  , GetAPModuleEnable,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_APModuleEnable",	NONE, READWRITE, 0,0},
{NULL				  , GetAuthenticationUser,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.AuthenticationUser",	NONE, READ, 0,0},
{NULL				  , GetReceiveNoise,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_ReceiveNoise",	NONE, READ, 0,0},
{NULL				  , GetLANTotalBytesSent,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_LAN-TotalBytesSent",   NONE, READ, 0,0},
{NULL				  , GetLANTotalBytesReceived,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_LAN-TotalBytesReceived",   NONE, READ, 0,0},
{NULL				  , GetWANTotalBytesSent,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_WAN-TotalBytesSent",   NONE, READ, 0,0},
{NULL				  , GetWANTotalBytesReceived,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_WAN-TotalBytesReceived",   NONE, READ, 0,0},
{NULL				  , GetLANTotalPacketsSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_LAN-TotalPacketsSent",	NONE, READ, 0,0},
{NULL				  , GetLANTotalPacketsReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_LAN-TotalPacketsReceived",	NONE, READ, 0,0},
{NULL				  , GetWANTotalPacketsSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_WAN-TotalPacketsSent",	NONE, READ, 0,0},
{NULL				  , GetWANTotalPacketsReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_WAN-TotalPacketsReceived",	NONE, READ, 0,0},
{NULL				  , GetTotalAssociations,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.TotalAssociations",	NONE, READ, 0,0},

{NULL				  , GetAssociatedDeviceMACAddress,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.AssociatedDeviceMACAddress",   NONE, READ, 0,0},
{NULL				  , GetAssociatedDeviceIPAddress,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.AssociatedDeviceIPAddress",	NONE, READ, 0,0},
{NULL				  , GetAssociatedDeviceAuthenticationState,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.AssociatedDeviceAuthenticationState",	NONE, READ, 0,0},
{NULL				  , GetCTReceiveRate, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.X_CT-COM_ReceiveRate",	NONE, READ, 0,0},
{NULL				  , GetCTSendRate,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.X_CT-COM_SendRate", NONE, READ, 0,0},
{NULL				  , GetTotalBytes,	  STRING,     "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.X_CT-COM_TotalBytes", NONE, READ, 0,0},
{NULL				  , GetCTReceivePower,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.X_CT-COM_ReceivePower", NONE, READ, 0,0},
{NULL				  , GetCTReceiveNoise,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.X_CT-COM_ReceiveNoise", NONE, READ, 0,0},
{NULL				  , GetCTPacketsError,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.AssociatedDevice.1.X_CT-COM_PacketsError", NONE, READ, 0,0},


{NULL				  , GetCRCError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.CRCError",   PASSIVE, READ, 0,0},
{NULL				  , GetPHYError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.PHYError",   NONE, READ, 0,0},
{NULL				  , GetMICError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.MICError",   NONE, READ, 0,0},
{NULL				  , GetKEYError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.KEYError",   PASSIVE, READ, 0,0},
{NULL				  , GetResource_using,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.Resource_using",   NONE, READ, 0,0},
{NULL				  , GetCTLANTotalBytesSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.LAN-TotalBytesSent",	NONE, READ, 0,0},
{NULL				  , GetCTLANTotalBytesReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.LAN-TotalBytesReceived",	NONE, READ, 0,0},
{NULL				  , GetCTWANTotalBytesSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.WAN-TotalBytesSent",	NONE, READ, 0,0},
{NULL				  , GetCTWANTotalBytesReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.WAN-TotalBytesReceived",	NONE, READ, 0,0},
{NULL				  , GetLANPacketsError,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.LAN-PacketsError",	NONE, READ, 0,0},
{NULL				  , GetWANPacketsError,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.WAN-PacketsError",	NONE, READ, 0,0},
{NULL				  , GetBreakTime,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.BreakTime",	NONE, READ, 0,0},
{NULL				  , GetAskPass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.AskPass", NONE, READ, 0,0},
{NULL				  , GetResponsePass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.ResponsePass",	NONE, READ, 0,0},
{NULL				  , GetSuccessPass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_CT-COM_Stat.SuccessPass", NONE, READ, 0,0},
//#endif




//#if 0  //ath2
{NULL	,GetLANStatus,  STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.3.Status",	  NONE, READ, 0,0},
{NULL	,GetLANMaxBitRate,  STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.3.MaxBitRate",	  NONE, READ, 0,0},
{NULL	,GetMACAddress,	 STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.3.MACAddress",	  NONE, READ, 0,0},
{SetEnable	  , GetEnable, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.Enable", NONE, READWRITE, 0,0},
{NULL		  , GetStatus, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.Status", NONE, READ, 0,0},
{NULL		  , GetBSSID,  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.BSSID",  NONE, READ, 0,0},
{SetChannel   , GetChannel,STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.Channel", ACTIVE, READWRITE, 0,0},
{NULL				  , GetChannelsInUse,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.GetChannelsInUse",	 PASSIVE, READ, 0,0},
{SetSSID	  , GetSSID,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.SSID",	ACTIVE, READWRITE, 0,0},

{SetRadioEnabled	  , GetRadioEnabled,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.RadioEnabled",	NONE, READWRITE, 0,0},
{NULL , GetWEPKeyIndex, 	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.GetWEPKeyIndex",   NONE, READ, 0,0},
{NULL , GetBeaconType,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.BeaconType", NONE, READ, 0,0},
{SetWEPEncryptionLevel		, GetWEPEncryptionLevel,		STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.WEPEncryptionLevel", 		   NONE, READWRITE, 0,0},
{SetBasicAuthencationMode	, GetBasicAuthencationMode, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.BasicAuthencationMode",NONE, READWRITE, 0,0},
{SetWPAEncryptionModes		, GetWPAEncryptionModes,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.WPAEncryptionModes",	NONE, READWRITE, 0,0},
{NULL , GetStandard,			STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.Standard",				   NONE, READ, 0,0},
{NULL , GetBasicEncryptionModes,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.BasicEncryptionModes",   NONE, READ, 0,0},
{NULL , GetPossibleChannels,		 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.PossibleChannels",				 NONE, READ, 0,0},
{NULL , GetAutoChannelEnable,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AutoChannelEnable",		   NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.WEPKey.1.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.WEPKey.2.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.WEPKey.3.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.WEPKey.4.WEPKey",NONE, READ, 0,0},
{NULL , GetPreSharedKey,    STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.PreSharedKey.1.PreSharedKey", 	  NONE, READ, 0,0},
{NULL , GetKeyPassphrase,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.PreSharedKey.1.KeyPassphrase",		NONE, READ, 0,0},
{SetMaxUser   , GetMaxUser,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.MaxUser",	NONE, READWRITE, 0,0},
{SetIsolation	  , GetIsolation,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.Isolation",	NONE, READWRITE, 0,0},
{NULL	  , GetVlanEnable,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.VlanEnable",	NONE, READ, 0,0},
{NULL	  , GetVlanVid,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.VlanVid",	NONE, READ, 0,0},
{SetSSIDHide	  , GetSSIDHide,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_SSIDHide",	NONE, READWRITE, 0,0},
{SetSSIDManage	  , GetSSIDManage,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Manage",	NONE, READWRITE, 0,0},
{SetPowerlevel	  , GetPowerlevel,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Powerlevel",	NONE, READWRITE, 0,0},
{NULL	  , GetPowerValue,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_PowerValue",	NONE, READ, 0,0},
{SetAPModuleEnable	  , GetAPModuleEnable,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_APModuleEnable",	NONE, READWRITE, 0,0},
{NULL				  , GetAuthenticationUser,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.AuthenticationUser",	NONE, READ, 0,0},
{NULL				  , GetReceiveNoise,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_ReceiveNoise",	NONE, READ, 0,0},
{NULL				  , GetLANTotalBytesSent,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_LAN-TotalBytesSent",   NONE, READ, 0,0},
{NULL				  , GetLANTotalBytesReceived,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_LAN-TotalBytesReceived",   NONE, READ, 0,0},
{NULL				  , GetWANTotalBytesSent,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_WAN-TotalBytesSent",   NONE, READ, 0,0},
{NULL				  , GetWANTotalBytesReceived,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_WAN-TotalBytesReceived",   NONE, READ, 0,0},
{NULL				  , GetLANTotalPacketsSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_LAN-TotalPacketsSent",	NONE, READ, 0,0},
{NULL				  , GetLANTotalPacketsReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_LAN-TotalPacketsReceived",	NONE, READ, 0,0},
{NULL				  , GetWANTotalPacketsSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_WAN-TotalPacketsSent",	NONE, READ, 0,0},
{NULL				  , GetWANTotalPacketsReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_WAN-TotalPacketsReceived",	NONE, READ, 0,0},
{NULL				  , GetTotalAssociations,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.TotalAssociations",	NONE, READ, 0,0},

{NULL				  , GetAssociatedDeviceMACAddress,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.AssociatedDeviceMACAddress",   NONE, READ, 0,0},
{NULL				  , GetAssociatedDeviceIPAddress,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.AssociatedDeviceIPAddress",	NONE, READ, 0,0},
{NULL				  , GetAssociatedDeviceAuthenticationState,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.AssociatedDeviceAuthenticationState",	NONE, READ, 0,0},
{NULL				  , GetCTReceiveRate, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.X_CT-COM_ReceiveRate",	NONE, READ, 0,0},
{NULL				  , GetCTSendRate,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.X_CT-COM_SendRate", NONE, READ, 0,0},
{NULL				  , GetTotalBytes,	  STRING,     "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.X_CT-COM_TotalBytes", NONE, READ, 0,0},
{NULL				  , GetCTReceivePower,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.X_CT-COM_ReceivePower", NONE, READ, 0,0},
{NULL				  , GetCTReceiveNoise,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.X_CT-COM_ReceiveNoise", NONE, READ, 0,0},
{NULL				  , GetCTPacketsError,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.AssociatedDevice.1.X_CT-COM_PacketsError", NONE, READ, 0,0},


{NULL				  , GetCRCError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.CRCError",   PASSIVE, READ, 0,0},
{NULL				  , GetPHYError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.PHYError",   NONE, READ, 0,0},
{NULL				  , GetMICError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.MICError",   NONE, READ, 0,0},
{NULL				  , GetKEYError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.KEYError",   PASSIVE, READ, 0,0},
{NULL				  , GetResource_using,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.Resource_using",   NONE, READ, 0,0},
{NULL				  , GetCTLANTotalBytesSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.LAN-TotalBytesSent",	NONE, READ, 0,0},
{NULL				  , GetCTLANTotalBytesReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.LAN-TotalBytesReceived",	NONE, READ, 0,0},
{NULL				  , GetCTWANTotalBytesSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.WAN-TotalBytesSent",	NONE, READ, 0,0},
{NULL				  , GetCTWANTotalBytesReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.WAN-TotalBytesReceived",	NONE, READ, 0,0},
{NULL				  , GetLANPacketsError,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.LAN-PacketsError",	NONE, READ, 0,0},
{NULL				  , GetWANPacketsError,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.WAN-PacketsError",	NONE, READ, 0,0},
{NULL				  , GetBreakTime,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.BreakTime",	NONE, READ, 0,0},
{NULL				  , GetAskPass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.AskPass", NONE, READ, 0,0},
{NULL				  , GetResponsePass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.ResponsePass",	NONE, READ, 0,0},
{NULL				  , GetSuccessPass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_CT-COM_Stat.SuccessPass", NONE, READ, 0,0},
//#endif



//#if 0 // ath3
{NULL	,GetLANStatus,  STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.4.Status",	  NONE, READ, 0,0},
{NULL	,GetLANMaxBitRate,  STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.4.MaxBitRate",	  NONE, READ, 0,0},
{NULL	,GetMACAddress,	 STRING, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.4.MACAddress",	  NONE, READ, 0,0},
{SetEnable	  , GetEnable, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.Enable", NONE, READWRITE, 0,0},
{NULL		  , GetStatus, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.Status", NONE, READ, 0,0},
{NULL		  , GetBSSID,  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.BSSID",  NONE, READ, 0,0},
{SetChannel   , GetChannel,STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.Channel", ACTIVE, READWRITE, 0,0},
{NULL				  , GetChannelsInUse,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.GetChannelsInUse",	 PASSIVE, READ, 0,0},
{SetSSID	  , GetSSID,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.SSID",	ACTIVE, READWRITE, 0,0},

{SetRadioEnabled	  , GetRadioEnabled,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.RadioEnabled",	NONE, READWRITE, 0,0},
{NULL , GetWEPKeyIndex, 	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.GetWEPKeyIndex",   NONE, READ, 0,0},
{NULL , GetBeaconType,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.BeaconType", NONE, READ, 0,0},
{SetWEPEncryptionLevel		, GetWEPEncryptionLevel,		STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.WEPEncryptionLevel", 		   NONE, READWRITE, 0,0},
{SetBasicAuthencationMode	, GetBasicAuthencationMode, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.BasicAuthencationMode",NONE, READWRITE, 0,0},
{SetWPAEncryptionModes		, GetWPAEncryptionModes,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.WPAEncryptionModes",	NONE, READWRITE, 0,0},
{NULL , GetStandard,			STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.Standard",				   NONE, READ, 0,0},
{NULL , GetBasicEncryptionModes,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.BasicEncryptionModes",   NONE, READ, 0,0},
{NULL , GetPossibleChannels,		 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.PossibleChannels",				 NONE, READ, 0,0},
{NULL , GetAutoChannelEnable,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AutoChannelEnable",		   NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.WEPKey.1.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.WEPKey.2.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.WEPKey.3.WEPKey",NONE, READ, 0,0},
{NULL , GetWEPKey,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.WEPKey.4.WEPKey",NONE, READ, 0,0},
{NULL , GetPreSharedKey,    STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.PreSharedKey.1.PreSharedKey", 	  NONE, READ, 0,0},
{NULL , GetKeyPassphrase,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.PreSharedKey.1.KeyPassphrase",		NONE, READ, 0,0},
{SetMaxUser   , GetMaxUser,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.MaxUser",	NONE, READWRITE, 0,0},
{SetIsolation	  , GetIsolation,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.Isolation",	NONE, READWRITE, 0,0},
{NULL	  , GetVlanEnable,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.VlanEnable",	NONE, READ, 0,0},
{NULL	  , GetVlanVid,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.VlanVid",	NONE, READ, 0,0},
{SetSSIDHide	  , GetSSIDHide,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_SSIDHide",	NONE, READWRITE, 0,0},
{SetSSIDManage	  , GetSSIDManage,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Manage",	NONE, READWRITE, 0,0},
{SetPowerlevel	  , GetPowerlevel,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Powerlevel",	NONE, READWRITE, 0,0},
{NULL	  , GetPowerValue,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_PowerValue",	NONE, READ, 0,0},
{SetAPModuleEnable	  , GetAPModuleEnable,	 STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_APModuleEnable",	NONE, READWRITE, 0,0},
{NULL				  , GetAuthenticationUser,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.AuthenticationUser",	NONE, READ, 0,0},
{NULL				  , GetReceiveNoise,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_ReceiveNoise",	NONE, READ, 0,0},
{NULL				  , GetLANTotalBytesSent,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_LAN-TotalBytesSent",   NONE, READ, 0,0},
{NULL				  , GetLANTotalBytesReceived,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_LAN-TotalBytesReceived",   NONE, READ, 0,0},
{NULL				  , GetWANTotalBytesSent,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_WAN-TotalBytesSent",   NONE, READ, 0,0},
{NULL				  , GetWANTotalBytesReceived,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_WAN-TotalBytesReceived",   NONE, READ, 0,0},
{NULL				  , GetLANTotalPacketsSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_LAN-TotalPacketsSent",	NONE, READ, 0,0},
{NULL				  , GetLANTotalPacketsReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_LAN-TotalPacketsReceived",	NONE, READ, 0,0},
{NULL				  , GetWANTotalPacketsSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_WAN-TotalPacketsSent",	NONE, READ, 0,0},
{NULL				  , GetWANTotalPacketsReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_WAN-TotalPacketsReceived",	NONE, READ, 0,0},
{NULL				  , GetTotalAssociations,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.TotalAssociations",	NONE, READ, 0,0},

{NULL				  , GetAssociatedDeviceMACAddress,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.AssociatedDeviceMACAddress",   NONE, READ, 0,0},
{NULL				  , GetAssociatedDeviceIPAddress,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.AssociatedDeviceIPAddress",	NONE, READ, 0,0},
{NULL				  , GetAssociatedDeviceAuthenticationState,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.AssociatedDeviceAuthenticationState",	NONE, READ, 0,0},
{NULL				  , GetCTReceiveRate, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.X_CT-COM_ReceiveRate",	NONE, READ, 0,0},
{NULL				  , GetCTSendRate,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.X_CT-COM_SendRate", NONE, READ, 0,0},
{NULL				  , GetTotalBytes,	  STRING,     "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.X_CT-COM_TotalBytes", NONE, READ, 0,0},
{NULL				  , GetCTReceivePower,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.X_CT-COM_ReceivePower", NONE, READ, 0,0},
{NULL				  , GetCTReceiveNoise,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.X_CT-COM_ReceiveNoise", NONE, READ, 0,0},
{NULL				  , GetCTPacketsError,	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.AssociatedDevice.1.X_CT-COM_PacketsError", NONE, READ, 0,0},


{NULL				  , GetCRCError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.CRCError",   PASSIVE, READ, 0,0},
{NULL				  , GetPHYError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.PHYError",   NONE, READ, 0,0},
{NULL				  , GetMICError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.MICError",   NONE, READ, 0,0},
{NULL				  , GetKEYError,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.KEYError",   PASSIVE, READ, 0,0},
{NULL				  , GetResource_using,		  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.Resource_using",   NONE, READ, 0,0},
{NULL				  , GetCTLANTotalBytesSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.LAN-TotalBytesSent",	NONE, READ, 0,0},
{NULL				  , GetCTLANTotalBytesReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.LAN-TotalBytesReceived",	NONE, READ, 0,0},
{NULL				  , GetCTWANTotalBytesSent, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.WAN-TotalBytesSent",	NONE, READ, 0,0},
{NULL				  , GetCTWANTotalBytesReceived, 	  STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.WAN-TotalBytesReceived",	NONE, READ, 0,0},
{NULL				  , GetLANPacketsError,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.LAN-PacketsError",	NONE, READ, 0,0},
{NULL				  , GetWANPacketsError,   STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.WAN-PacketsError",	NONE, READ, 0,0},
{NULL				  , GetBreakTime,	STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.BreakTime",	NONE, READ, 0,0},
{NULL				  , GetAskPass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.AskPass", NONE, READ, 0,0},
{NULL				  , GetResponsePass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.ResponsePass",	NONE, READ, 0,0},
{NULL				  , GetSuccessPass, STRING, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_CT-COM_Stat.SuccessPass", NONE, READ, 0,0},
//#endif

//ip ping diag  
//#if 0                                                                    
{SetDiagnosticsState,  GetDiagnosticsState, STRING, "InternetGatewayDevice.IPPingDiagnostics.DiagnosticsState",	  NONE, READWRITE, 0},
{SetInterface, NULL	,  STRING, "InternetGatewayDevice.IPPingDiagnostics.Interface",	  NONE, WRITE, 0},
{SetHost, NULL ,  STRING, "InternetGatewayDevice.IPPingDiagnostics.Host",   NONE, WRITE, 0},
{SetNumberOfRepetions,NULL	,  STRING, "InternetGatewayDevice.IPPingDiagnostics.NumberOfRepetitions",	  NONE, WRITE, 0},
{SetTimeout,NULL	,  STRING, "InternetGatewayDevice.IPPingDiagnostics.Timeout",	  NONE, WRITE, 0},
{SetDataBlockSize,NULL	,  STRING, "InternetGatewayDevice.IPPingDiagnostics.DataBlockSize",   NONE, WRITE, 0},
{SetDSCP,NULL	,  STRING, "InternetGatewayDevice.IPPingDiagnostics.DSCP",	  NONE, WRITE, 0},
{NULL	,GetSuccessCount,  STRING, "InternetGatewayDevice.IPPingDiagnostics.SuccessCount",	  NONE, READ, 0},
{NULL	,GetFailureCount,  STRING, "InternetGatewayDevice.IPPingDiagnostics.FailureCount",	  NONE, READ, 0},
{NULL	,GetAverageResponseTime,  STRING, "InternetGatewayDevice.IPPingDiagnostics.AverageResponseTime",	  NONE, READ, 0},
{NULL	,GetMinimumResponseTime,  STRING, "InternetGatewayDevice.IPPingDiagnostics.MinimumResponseTime",	  NONE, READ, 0},
{NULL	,GetMaximumResponseTime,  STRING, "InternetGatewayDevice.IPPingDiagnostics.MaximumResponseTime",	  NONE, READ, 0},
//#endif

//#if 0
//wandevice
{NULL         , GetWanIp,  STRING, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.ExternalIPAddress", PASSIVE, READ,   0,1},

//managementserver
{NULL         , GetParKey, STRING, "InternetGatewayDevice.ManagementServer.ParameterKey"         , PASSIVE, READ,  0,1},
{SetCpeURL		  , GetCpeURL, STRING, "InternetGatewayDevice.ManagementServer.ConnectionRequestURL" , PASSIVE, READ,  0,1},
//#endif

//device info
//#if 0
{NULL  ,             GetManufacturer,      STRING, "InternetGatewayDevice.DeviceInfo.Manufacturer",            NONE, READ, 0,1},
{NULL				  , GetProductClass,		 STRING, "InternetGatewayDevice.DeviceInfo.ProductClass",				 PASSIVE, READ, 0,1},
{NULL				  , GetOUI,		 STRING, "InternetGatewayDevice.DeviceInfo.ManufacturerOUI",				 PASSIVE, READ, 0,1},

{NULL				  , GetSerialNumber,		 STRING, "InternetGatewayDevice.DeviceInfo.SerialNumber",				 NONE, READ, 0,1},
{NULL         , GetSoftwareVersion,  STRING, "InternetGatewayDevice.DeviceInfo.SoftwareVersion"            , PASSIVE, READ,  0,1},
{NULL         , GetHardwareVersion,  STRING, "InternetGatewayDevice.DeviceInfo.HardwareVersion"            , PASSIVE, READ,  0,1},
//#endif

//work state
//#if 0
{NULL			,GetWorkTime,	  STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_WorkTime", NONE, READ, 0,1},
{NULL			,GetCPU,	  STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_CPU", NONE, READ, 0,1},
{NULL			,GetTemp,	  STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_Temp", NONE, READ, 0,1},
{NULL			,GetTotalMem,	  STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_TotalMem", NONE, READ, 0,1},
{NULL			,GetUseMem,   STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_UseMem", NONE, READ, 0,1},
{NULL			,GetFlash,	 STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_Flash", NONE, READ, 0,1},
{NULL			,GetFreeFlash,	 STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_FreeFlash", NONE, READ, 0,1},
{NULL			,GeWorkMode,	 STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_WorkMode", NONE, READ, 0,1},
{SetLogIP			,GetLogIP,	 STRING, "InternetGatewayDevice.DeviceInfo.X_CT-COM_LogIP", PASSIVE, READ, 0,1},
//fake
{NULL,GetTeleACEnable,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_TeleComAccount.Enable", PASSIVE, READWRITE, 0,1},
{NULL,GetTeleACPass,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_TeleComAccount.Password", PASSIVE, READ, 0,1},
//#endif

#if 0
//monitor
{SetMonEnable,GetMonEnable,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.Enable", NONE, READWRITE, 0,1},
{NULL,GetMonCount,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorNumberOfEntries",NONE, READ, 0,1},
{SetMonParaList,GetMonParaList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.1.ParaList",NONE, READWRITE, 0,1},
{SetMonTimeList,GetMonTimeList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.1.TimeLists",NONE, READWRITE, 0,1},


{SetMonParaList,GetMonParaList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.2.ParaList",NONE, READWRITE, 0,1},
{SetMonTimeList,GetMonTimeList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.2.TimeLists",NONE, READWRITE, 0,1},
{SetMonParaList,GetMonParaList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.3.ParaList",NONE, READWRITE, 0,1},
{SetMonTimeList,GetMonTimeList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.3.TimeLists",NONE, READWRITE, 0,1},
{SetMonParaList,GetMonParaList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.4.ParaList",NONE, READWRITE, 0,1},
{SetMonTimeList,GetMonTimeList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.4.TimeLists",NONE, READWRITE, 0,1},
{SetMonParaList,GetMonParaList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.5.ParaList",NONE, READWRITE, 0,1},
{SetMonTimeList,GetMonTimeList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.5.TimeLists",NONE, READWRITE, 0,1},
#endif

#if 0

//alarm
{SetAlarmEnable,GetAlarmEnable,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.Enable", NONE, READWRITE, 0,1},
{NULL,GetAlarmCount,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmNumberOfEntries",NONE, READ, 0,1},
{SetAlarmParaList,GetAlarmParaList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.1.ParaList",NONE, READWRITE, 0,1},
{SetAlarmTimeList,GetAlarmTimeList,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.1.TimeLists",NONE, READWRITE, 0,1},
{SetAlarmLMax,GetAlarmLMax,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.1.Limit-Max",NONE, READWRITE, 0,1},
{SetAlarmLMin,GetAlarmLMin,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.1.Limit-Min",NONE, READWRITE, 0,1},
{SetAlarmMode,GetAlarmMode,STRING,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.1.Mode",NONE, READWRITE, 0,1},
#endif


{NULL                 , GetPeriodicInfEnable,  STRING, "InternetGatewayDevice.ManagementServer.PeriodicInformEnable",NONE, READWRITE, 0,1},
{NULL				  , GetPeriodicInfInterval,  STRING, "InternetGatewayDevice.ManagementServer.PeriodicInformInterval",NONE, READWRITE, 0,1},


};
//#endif


#endif // _LOCALMANAGERDEFINITION_H
