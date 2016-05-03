#ifndef _WTPD_PLATFORM_H_
#define _WTPD_PLATFORM_H_
#include "common.h"


extern CWBool PopenFile (char *cmd_str,char *str,int len );


//get product infomation
CWBool CWGetSN(char * sn);
CWBool CWGetBaseMac(char * basemac);
CWBool CWGetMode(char * mode);
CWBool CWGetRealMode(char * realMode);
CWBool CWGetSoftwareVersion(char * softwareVersion);
CWBool CWGetHardwareVersion(char * hardwareVersion);
CWBool CWGetApCode(char * WTPModelNum);
CWBool CWGetInternalSoftwareVersion(char * internalSoftwareVersion);
CWBool CWGetManuOption(char WTPRealModelNum[64], unsigned int *gManuOption);

//
CWBool CWGetTxpower(int *txpower, char *wlanname);
CWBool CWSetCountryCode(unsigned char countryCode[4]);

//configure parameters of split mac for vap

CWBool CWSetWlanTunnelMode(char *wlanname,int tunnel_mode);
CWBool CWEnableIpForward();
CWBool CWSetSpfastDown(int enable);
CWBool CWSetSpfastUp(int enable);
CWBool CWSetUdpPort();
CWBool CWSetAC_IP(char *ac_ip);
CWBool CWEnableIPinIP(char *wlanname,int enable);


CWBool CWSet_dhcp_snooping(unsigned char state);
CWBool CWEnable_linkcheck(int eth_num);
CWBool CWSaveACAddrFromDNS(char *ACDomainName);
int           CWGetACCountFromDNS(char *ACDomainName);
CWBool CWGetACAddrFromDNS(char *ACDomainName,int rowNum,char *ACAddr);
CWBool CWWTPSetStaticIP(WTPStaticIPValues *wtpStaticIPValues);
CWBool CWWTPSetStaticIPV2(WTPStaticIPValues *wtpStaticIPValues);
void	       CWSetLastConnectLocalIP(void);
CWBool CWSysReboot();
CWBool CWClearMemory();
CWBool CWGetUpgradeImage(unsigned char *image_name,unsigned char *ip,char *successfile);
CWBool CWac_get();
CWBool CWac_lose();
CWBool CWSetThinapState(int thinap_state);
CWBool CWSetThinapCheckTimer();
CWBool CWSetNeighborDeadInterval(int neighborDeadInterval);
CWBool CWSysUpgrade(char *imageName);
CWBool CWTime_Now(char *nowReadable,time_t now);
CWBool CWSetAntennaNone(char *wlanname);
CWBool CWPingTestAndSave(char *ip,char *filename);
CWBool CWSta_Access_limit();
CWBool acw_parse_dhcpsnooping_report(CWProtocolMessage *msgPtr, int len ,dhcpsnooping_report_type *valPtr);
CWBool CWNtp_TimeSync(char *ip);



#endif
