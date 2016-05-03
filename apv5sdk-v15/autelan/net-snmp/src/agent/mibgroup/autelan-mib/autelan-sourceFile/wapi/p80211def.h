/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: p80211def.h
* description:  implementation for definitions of types and structures.
* 
*
* 
************************************************************************************/

#ifndef _P80211DEF_H

#define _P80211DEF_H


typedef unsigned char   UINT8;

typedef unsigned short  UINT16;

typedef unsigned long   UINT32;




typedef signed char     INT8;

typedef signed short    INT16;

typedef signed long     INT32;


#define SIOCIWFIRSTPRIV	0x8BE0
#define P80211_IFTEST		(SIOCIWFIRSTPRIV + 7)

#define P80211_IFREQ		(SIOCIWFIRSTPRIV + 9)



#define P80211_IOCTL_MAGIC	(0x4a2d464dUL)



#define MSG_BUFF_LEN	4000
 //512

#define WLAN_DEVNAMELEN_MAX	16



#define DIDmsg_dot11req_mibget 	1

#define DIDmsg_dot11req_mibset	2


#define P80211ENUM_resultcode_success		1

#define P80211ENUM_resultcode_invalid_parameters	2

#define P80211ENUM_resultcode_not_supported	3

#define P80211ENUM_resultcode_timeout		4

#define P80211ENUM_resultcode_too_many_req	5

#define P80211ENUM_resultcode_refused		6

#define P80211ENUM_resultcode_bss_already	7

#define P80211ENUM_resultcode_invalid_access	8

#define P80211ENUM_resultcode_invalid_mibattribute	9

#define P80211ENUM_resultcode_cant_set_readonly_mib	10

#define P80211ENUM_resultcode_implementation_failure	11


#define P80211ENUM_truth_false			0

#define P80211ENUM_truth_true			1



#define P80211ENUM_msgitem_status_data_ok		0

#define P80211ENUM_msgitem_status_no_value		1

#define P80211ENUM_msgitem_status_invalid_itemname	2

#define P80211ENUM_msgitem_status_invalid_itemdata	3

#define P80211ENUM_msgitem_status_missing_itemdata	4

#define P80211ENUM_msgitem_status_incomplete_itemdata	5

#define P80211ENUM_msgitem_status_invalid_msg_did	6

#define P80211ENUM_msgitem_status_invalid_mib_did	7

#define P80211ENUM_msgitem_status_missing_conv_func	8

#define P80211ENUM_msgitem_status_string_too_long	9

#define P80211ENUM_msgitem_status_data_out_of_range	10

#define P80211ENUM_msgitem_status_string_too_short	11

#define P80211ENUM_msgitem_status_missing_valid_func	12

#define P80211ENUM_msgitem_status_unknown		13

#define P80211ENUM_msgitem_status_invalid_did		14

#define P80211ENUM_msgitem_status_missing_print_func	15


#define P80211ENUM_msgitem_type_uint 		0
#define P80211ENUM_msgitem_type_pstrd 		1

#define DIDmib_cat_dot11smt 21000


#define DIDmib_dot11smt_dot11StationConfigTable 21100


#define DIDmib_dot11smt_dot11StationConfigTable_dot11StationID 21101


#define DIDmib_dot11smt_dot11StationConfigTable_dot11MediumOccupancyLimit 21102


#define DIDmib_dot11smt_dot11StationConfigTable_dot11CFPollable 21103


#define DIDmib_dot11smt_dot11StationConfigTable_dot11CFPPeriod 21104


#define DIDmib_dot11smt_dot11StationConfigTable_dot11CFPMaxDuration 21105


#define DIDmib_dot11smt_dot11StationConfigTable_dot11AuthenticationResponseTimeOut 21106


#define DIDmib_dot11smt_dot11StationConfigTable_dot11PrivacyOptionImplemented 21107


#define DIDmib_dot11smt_dot11StationConfigTable_dot11PowerManagementMode 21108

#define DIDmib_dot11smt_dot11StationConfigTable_dot11DesiredSSID 21109


#define DIDmib_dot11smt_dot11StationConfigTable_dot11DesiredBSSType 21110


#define DIDmib_dot11smt_dot11StationConfigTable_dot11OperationalRateSet 21111


#define DIDmib_dot11smt_dot11StationConfigTable_dot11BeaconPeriod 21112


#define DIDmib_dot11smt_dot11StationConfigTable_dot11DTIMPeriod 21113


#define DIDmib_dot11smt_dot11StationConfigTable_dot11AssociationResponseTimeOut 21114


#define DIDmib_dot11smt_dot11StationConfigTable_dot11DisassociateReason 21115


#define DIDmib_dot11smt_dot11StationConfigTable_dot11DisassociateStation 21116


#define DIDmib_dot11smt_dot11StationConfigTable_dot11DeauthenticateReason 21117


#define DIDmib_dot11smt_dot11StationConfigTable_dot11DeauthenticateStation 21118


#define DIDmib_dot11smt_dot11StationConfigTable_dot11AuthenticateFailStatus 21119


#define DIDmib_dot11smt_dot11StationConfigTable_dot11AuthenticateFailStation 21120


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable 21200


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithm1 21201


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithmsEnable1 21202


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithm2 21203


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithmsEnable2 21204


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithm3 21205


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithmsEnable3 21206


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithm4 21207


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithmsEnable4 21208


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithm5 21209


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithmsEnable5 21210


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithm6 21211


#define DIDmib_dot11smt_dot11AuthenticationAlgorithmsTable_dot11AuthenticationAlgorithmsEnable6 21212


#define DIDmib_dot11smt_dot11WEPDefaultKeysTable 21300


#define DIDmib_dot11smt_dot11WEPDefaultKeysTable_dot11WEPDefaultKey0 21301


#define DIDmib_dot11smt_dot11WEPDefaultKeysTable_dot11WEPDefaultKey1 21302


#define DIDmib_dot11smt_dot11WEPDefaultKeysTable_dot11WEPDefaultKey2 21303


#define DIDmib_dot11smt_dot11WEPDefaultKeysTable_dot11WEPDefaultKey3 21304


#define DIDmib_dot11smt_dot11WEPKeyMappingsTable 21400


#define DIDmib_dot11smt_dot11WEPKeyMappingsTable_dot11WEPKeyMappingIndex 21401


#define DIDmib_dot11smt_dot11WEPKeyMappingsTable_dot11WEPKeyMappingAddress 21402


#define DIDmib_dot11smt_dot11WEPKeyMappingsTable_dot11WEPKeyMappingWEPOn 21403


#define DIDmib_dot11smt_dot11WEPKeyMappingsTable_dot11WEPKeyMappingValue 21404


#define DIDmib_dot11smt_dot11PrivacyTable 21500


#define DIDmib_dot11smt_dot11PrivacyTable_dot11PrivacyInvoked 21501


#define DIDmib_dot11smt_dot11PrivacyTable_dot11WEPDefaultKeyID 21502


#define DIDmib_dot11smt_dot11PrivacyTable_dot11WEPKeyMappingLength 21503


#define DIDmib_dot11smt_dot11PrivacyTable_dot11ExcludeUnencrypted 21504


#define DIDmib_dot11smt_dot11PrivacyTable_dot11WEPICVErrorCount 21505


#define DIDmib_dot11smt_dot11PrivacyTable_dot11WEPExcludedCount 21606


#define DIDmib_cat_dot11mac 22000


#define DIDmib_dot11mac_dot11OperationTable 22100

#define DIDmib_dot11mac_dot11OperationTable_dot11MACAddress 22101


#define DIDmib_dot11mac_dot11OperationTable_dot11RTSThreshold 22102


#define DIDmib_dot11mac_dot11OperationTable_dot11ShortRetryLimit 22103


#define DIDmib_dot11mac_dot11OperationTable_dot11LongRetryLimit 22104


#define DIDmib_dot11mac_dot11OperationTable_dot11FragmentationThreshold 22105

#define DIDmib_dot11mac_dot11OperationTable_dot11MaxTransmitMSDULifetime 22106


#define DIDmib_dot11mac_dot11OperationTable_dot11MaxReceiveLifetime 22107


#define DIDmib_dot11mac_dot11OperationTable_dot11ManufacturerID 22108


#define DIDmib_dot11mac_dot11OperationTable_dot11ProductID 22109


#define DIDmib_dot11mac_dot11CountersTable 22200


#define DIDmib_dot11mac_dot11CountersTable_dot11TransmittedFragmentCount 22201


#define DIDmib_dot11mac_dot11CountersTable_dot11MulticastTransmittedFrameCount 22202


#define DIDmib_dot11mac_dot11CountersTable_dot11FailedCount 22203


#define DIDmib_dot11mac_dot11CountersTable_dot11RetryCount 22204


#define DIDmib_dot11mac_dot11CountersTable_dot11MultipleRetryCount 22205


#define DIDmib_dot11mac_dot11CountersTable_dot11FrameDuplicateCount 22206


#define DIDmib_dot11mac_dot11CountersTable_dot11RTSSuccessCount 22207


#define DIDmib_dot11mac_dot11CountersTable_dot11RTSFailureCount 22208


#define DIDmib_dot11mac_dot11CountersTable_dot11ACKFailureCount 22209


#define DIDmib_dot11mac_dot11CountersTable_dot11ReceivedFragmentCount 22210


#define DIDmib_dot11mac_dot11CountersTable_dot11MulticastReceivedFrameCount 22211


#define DIDmib_dot11mac_dot11CountersTable_dot11FCSErrorCount 22212


#define DIDmib_dot11mac_dot11CountersTable_dot11TransmittedFrameCount 22213


#define DIDmib_dot11mac_dot11CountersTable_dot11WEPUndecryptableCount 22214


#define DIDmib_dot11mac_dot11GroupAddressesTable 22300


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address1 22301


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address2 22302


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address3 22303


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address4 22304


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address5 22305


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address6 22306


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address7 22307


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address8 22308


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address9 22309


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address10 22310


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address11 22311


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address12 22312


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address13 22313


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address14 22314


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address15 22315


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address16 22316


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address17 22317


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address18 22318


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address19 22319


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address20 22320


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address21 22321


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address22 22322


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address23 22323


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address24 22324


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address25 22325

#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address26 22326


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address27 22327


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address28 22328


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address29 22329


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address30 22330


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address31 22331


#define DIDmib_dot11mac_dot11GroupAddressesTable_dot11Address32 22332


#define DIDmib_cat_dot11phy 23000


#define DIDmib_dot11phy_dot11PhyOperationTable 23100


#define DIDmib_dot11phy_dot11PhyOperationTable_dot11PHYType 23101


#define DIDmib_dot11phy_dot11PhyOperationTable_dot11CurrentRegDomain 23102


#define DIDmib_dot11phy_dot11PhyOperationTable_dot11TempType 23103


#define DIDmib_dot11phy_dot11PhyOperationTable_dot11ChannelAgilityPresent 23104


#define DIDmib_dot11phy_dot11PhyOperationTable_dot11ChannelAgilityEnabled 23105


#define DIDmib_dot11phy_dot11PhyAntennaTable 23200

#define DIDmib_dot11phy_dot11PhyAntennaTable_dot11CurrentTxAntenna 23201


#define DIDmib_dot11phy_dot11PhyAntennaTable_dot11DiversitySupport 23202


#define DIDmib_dot11phy_dot11PhyAntennaTable_dot11CurrentRxAntenna 23203


#define DIDmib_dot11phy_dot11PhyTxPowerTable 23300


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11NumberSupportedPowerLevels 23301


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11TxPowerLevel1 23302


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11TxPowerLevel2 23303


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11TxPowerLevel3 23304


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11TxPowerLevel4 23305


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11TxPowerLevel5 23306


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11TxPowerLevel6 23307


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11TxPowerLevel7 23308


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11TxPowerLevel8 23309


#define DIDmib_dot11phy_dot11PhyTxPowerTable_dot11CurrentTxPowerLevel 23310


#define DIDmib_dot11phy_dot11PhyFHSSTable 23400


#define DIDmib_dot11phy_dot11PhyFHSSTable_dot11HopTime 23401


#define DIDmib_dot11phy_dot11PhyFHSSTable_dot11CurrentChannelNumber 23402


#define DIDmib_dot11phy_dot11PhyFHSSTable_dot11MaxDwellTime 23403


#define DIDmib_dot11phy_dot11PhyFHSSTable_dot11CurrentDwellTime 23404


#define DIDmib_dot11phy_dot11PhyFHSSTable_dot11CurrentSet 23405


#define DIDmib_dot11phy_dot11PhyFHSSTable_dot11CurrentPattern 23406


#define DIDmib_dot11phy_dot11PhyFHSSTable_dot11CurrentIndex 23407


#define DIDmib_dot11phy_dot11PhyDSSSTable 23500


#define DIDmib_dot11phy_dot11PhyDSSSTable_dot11CurrentChannel 23501


#define DIDmib_dot11phy_dot11PhyDSSSTable_dot11CCAModeSupported 23502
#define DIDmib_dot11phy_dot11PhyDSSSTable_dot11CurrentCCAMode 23503
#define DIDmib_dot11phy_dot11PhyDSSSTable_dot11EDThreshold 23504
#define DIDmib_dot11phy_dot11PhyDSSSTable_dot11ShortPreambleOptionImplemented 23505
#define DIDmib_dot11phy_dot11PhyDSSSTable_dot11PBCCOptionImplemented 23506
#define DIDmib_dot11phy_dot11PhyIRTable 23600
#define DIDmib_dot11phy_dot11PhyIRTable_dot11CCAWatchdogTimerMax 23601
#define DIDmib_dot11phy_dot11PhyIRTable_dot11CCAWatchdogCountMax 23602
#define DIDmib_dot11phy_dot11PhyIRTable_dot11CCAWatchdogTimerMin 23603
#define DIDmib_dot11phy_dot11PhyIRTable_dot11CCAWatchdogCountMin 23604
#define DIDmib_dot11phy_dot11RegDomainsSupportedTable 23700
#define DIDmib_dot11phy_dot11RegDomainsSupportedTable_dot11RegDomainsSupportIndex 23701
#define DIDmib_dot11phy_dot11RegDomainsSupportedTable_dot11RegDomainsSupportValue 23702
#define DIDmib_dot11phy_dot11AntennasListTable 23800
#define DIDmib_dot11phy_dot11AntennasListTable_dot11AntennaListIndex 23801
#define DIDmib_dot11phy_dot11AntennasListTable_dot11SupportedTxAntenna 23802
#define DIDmib_dot11phy_dot11AntennasListTable_dot11SupportedRxAntenna 23803
#define DIDmib_dot11phy_dot11AntennasListTable_dot11DiversitySelectionRx 23804
#define DIDmib_dot11phy_dot11SupportedDataRatesTxTable 23900
#define DIDmib_dot11phy_dot11SupportedDataRatesTxTable_dot11SupportedDataRatesTxIndex 23901
#define DIDmib_dot11phy_dot11SupportedDataRatesTxTable_dot11SupportedDataRatesTxValue 23902
#define DIDmib_dot11phy_dot11SupportedDataRatesRxTable 23950
#define DIDmib_dot11phy_dot11SupportedDataRatesRxTable_dot11SupportedDataRatesRxIndex 23951
#define DIDmib_dot11phy_dot11SupportedDataRatesRxTable_dot11SupportedDataRatesRxValue 23952




#define WAPI_AP 					30000

#define  WAPI_AP_LOCAL					(WAPI_AP + 0)
#define  WAPI_AP_LOCAL_ESSID				(WAPI_AP_LOCAL + 1)
#define  WAPI_AP_LOCAL_BSSID				(WAPI_AP_LOCAL + 2)
#define  WAPI_AP_LOCAL_CHANNELMASK			(WAPI_AP_LOCAL + 3)
#define  WAPI_AP_LOCAL_CHANNEL			(WAPI_AP_LOCAL + 4)
#define  WAPI_AP_LOCAL_MODE				(WAPI_AP_LOCAL + 5)
#define  WAPI_AP_LOCAL_APDENSITY			(WAPI_AP_LOCAL + 6)
#define  WAPI_AP_LOCAL_TXRATECONTROL			(WAPI_AP_LOCAL + 7)
#define  WAPI_AP_LOCAL_FWTXRATECONTROL		(WAPI_AP_LOCAL + 8)
#define  WAPI_AP_LOCAL_HOSTROAMING			(WAPI_AP_LOCAL + 9)
#define  WAPI_AP_LOCAL_NAME  			(WAPI_AP_LOCAL + 10)
#define  WAPI_AP_LOCAL_BEACONINTERVAL		(WAPI_AP_LOCAL + 11)
#define  WAPI_AP_LOCAL_DTIMPERIOD			(WAPI_AP_LOCAL + 12)
#define  WAPI_AP_LOCAL_RTSTHRESHOLD			(WAPI_AP_LOCAL + 13)
#define  WAPI_AP_LOCAL_FRAGTHRESHOLD			(WAPI_AP_LOCAL + 14)
#define  WAPI_AP_LOCAL_SHORTRETRYLIMIT		(WAPI_AP_LOCAL + 15)
#define  WAPI_AP_LOCAL_LONGRETRYLIMIT		(WAPI_AP_LOCAL + 16)
#define  WAPI_AP_LOCAL_TRANSLIFETIME			(WAPI_AP_LOCAL + 17)

#define  WAPI_AP_AP					(WAPI_AP + 50)
#define  WAPI_AP_AP_IFBRIDGEPACKETS			(WAPI_AP_AP + 1)
#define  WAPI_AP_AP_BRIDGEUNICAST			(WAPI_AP_AP + 2)
#define  WAPI_AP_AP_BRIDGEMULTICAST			(WAPI_AP_AP + 3)
#define  WAPI_AP_AP_OTHERAPPOLICY			(WAPI_AP_AP + 4)
#define  WAPI_AP_AP_MAXINACTIVITY			(WAPI_AP_AP + 5)
#define  WAPI_AP_AP_AUTHALGORITHM			(WAPI_AP_AP + 6)
#define  WAPI_AP_AP_LASTTXRATE				(WAPI_AP_AP + 7)
#define  WAPI_AP_AP_NULLFUNCACK			(WAPI_AP_AP + 8)

#define  WAPI_AP_WDS					(WAPI_AP + 100)
#define  WAPI_AP_WDS_MAXCONNECTIONS			(WAPI_AP_WDS + 1)
#define  WAPI_AP_WDS_CONNECTIONS				(WAPI_AP_WDS + 2)
#define  WAPI_AP_WDS_AUTOMAPWDS				(WAPI_AP_WDS + 3)
#define  WAPI_AP_WDS_INDEX					(WAPI_AP_WDS + 11)
#define  WAPI_AP_WDS_REMOTEADDR				(WAPI_AP_WDS + 12)
#define  WAPI_AP_WDS_STORAGETYPE				(WAPI_AP_WDS + 13)
#define  WAPI_AP_WDS_STATUS					(WAPI_AP_WDS + 14)

#define  WAPI_AP_COUNTER				(WAPI_AP + 200)
#define  WAPI_AP_COUNTER_TXUNICASTFRAMES			(WAPI_AP_COUNTER + 1)
#define  WAPI_AP_COUNTER_TXMULTICASTFRAMES			(WAPI_AP_COUNTER + 2)
#define  WAPI_AP_COUNTER_TXFRAGMENTS				(WAPI_AP_COUNTER + 3)
#define  WAPI_AP_COUNTER_TXUNIICASTOCTETS			(WAPI_AP_COUNTER + 4)
#define  WAPI_AP_COUNTER_TXMULTICASTOCTETS			(WAPI_AP_COUNTER + 5)
#define  WAPI_AP_COUNTER_TXDEFEREDTRANSMISSIONS		(WAPI_AP_COUNTER + 6)
#define  WAPI_AP_COUNTER_TXSINGLERETRYFRAMES			(WAPI_AP_COUNTER + 7)
#define  WAPI_AP_COUNTER_TXMULTIPLERETRYFRAMES		(WAPI_AP_COUNTER + 8)
#define  WAPI_AP_COUNTER_TXRETRYLIMITEXCEEDED		(WAPI_AP_COUNTER + 9)
#define  WAPI_AP_COUNTER_TXDISCARDS				(WAPI_AP_COUNTER + 10)
#define  WAPI_AP_COUNTER_RXUNICASTFRAMES			(WAPI_AP_COUNTER + 11)
#define  WAPI_AP_COUNTER_RXMULTICASTFRAMES			(WAPI_AP_COUNTER + 12)
#define  WAPI_AP_COUNTER_RXFRAGMENTS				(WAPI_AP_COUNTER + 13)
#define  WAPI_AP_COUNTER_RXUNICASTOCTETS			(WAPI_AP_COUNTER + 14)
#define  WAPI_AP_COUNTER_RXMULTICASTOCTETS			(WAPI_AP_COUNTER + 15)
#define  WAPI_AP_COUNTER_RXFCSERRORS				(WAPI_AP_COUNTER + 16)
#define  WAPI_AP_COUNTER_RXDISCARDSNOBUFFER			(WAPI_AP_COUNTER + 17)
#define  WAPI_AP_COUNTER_RXDISCARDSWRONGSA			(WAPI_AP_COUNTER + 18)
#define  WAPI_AP_COUNTER_RXDISCARDSWEPNNDECRYPTABLE	(WAPI_AP_COUNTER + 19)
#define  WAPI_AP_COUNTER_RXMESSAGEINMSGFRAGMENTS		(WAPI_AP_COUNTER + 20)
#define  WAPI_AP_COUNTER_RXMESSAGEINBADMSGFRAGMENTS	(WAPI_AP_COUNTER + 21)
	
#define  WAPI_AP_SRT						(WAPI_AP + 250)
#define  WAPI_AP_SRT_JOINTIME				(WAPI_AP_SRT + 1)
#define  WAPI_AP_SRT_PREFERREDAP				(WAPI_AP_SRT + 2)
#define  WAPI_AP_SRT_COUNT					(WAPI_AP_SRT + 3)
#define  WAPI_AP_SRT_INDEX					(WAPI_AP_SRT + 11)
#define  WAPI_AP_SRT_CHANNELID				(WAPI_AP_SRT + 12)
#define  WAPI_AP_SRT_ANL					(WAPI_AP_SRT + 13)
#define  WAPI_AP_SRT_SL					(WAPI_AP_SRT + 14)
#define  WAPI_AP_SRT_BSSID					(WAPI_AP_SRT + 15)
#define  WAPI_AP_SRT_BEACONINTERVAL			(WAPI_AP_SRT + 16)
#define  WAPI_AP_SRT_CAPABILITY				(WAPI_AP_SRT + 17)
#define  WAPI_AP_SRT_SSID					(WAPI_AP_SRT + 18)
#define  WAPI_AP_SRT_SUPPORTEDRATES			(WAPI_AP_SRT + 19)
#define  WAPI_AP_SRT_RATE					(WAPI_AP_SRT + 20)

#define  WAPI_AP_STA						(WAPI_AP + 300)
#define  WAPI_AP_STA_NUMBER					(WAPI_AP_STA + 1)
#define  WAPI_AP_STA_INDEX					(WAPI_AP_STA + 11)
#define  WAPI_AP_STA_USERS					(WAPI_AP_STA + 12)
#define  WAPI_AP_STA_ADDRESS					(WAPI_AP_STA + 13)
#define  WAPI_AP_STA_AID					(WAPI_AP_STA + 14)
#define  WAPI_AP_STA_FLAGS					(WAPI_AP_STA + 15)
#define  WAPI_AP_STA_CAPABILITY				(WAPI_AP_STA + 16)
#define  WAPI_AP_STA_LISTENINTERVAL			(WAPI_AP_STA + 17)
#define  WAPI_AP_STA_SUPPORTEDRATES			(WAPI_AP_STA + 18)
#define  WAPI_AP_STA_LASTAUTHEN				(WAPI_AP_STA + 19)
#define  WAPI_AP_STA_LASTASSOC				(WAPI_AP_STA + 20)
#define  WAPI_AP_STA_LASTRX					(WAPI_AP_STA + 21)
#define  WAPI_AP_STA_LASTTX					(WAPI_AP_STA + 22)
#define  WAPI_AP_STA_RXPACKETS				(WAPI_AP_STA + 23)
#define  WAPI_AP_STA_TXPACKETS				(WAPI_AP_STA + 24)
#define  WAPI_AP_STA_RXBYTES					(WAPI_AP_STA + 25)
#define  WAPI_AP_STA_TXBYTES					(WAPI_AP_STA + 26)
#define  WAPI_AP_STA_LASTRXSILENCE			(WAPI_AP_STA + 27)
#define  WAPI_AP_STA_LASTSIGNAL				(WAPI_AP_STA + 28)
#define  WAPI_AP_STA_LASTRXRATE				(WAPI_AP_STA + 29)
#define  WAPI_AP_STA_LASTRXFLOW				(WAPI_AP_STA + 30)
#define  WAPI_AP_STA_TXSUPPORTEDRATE			(WAPI_AP_STA + 31)
#define  WAPI_AP_STA_TXRATE					(WAPI_AP_STA + 32)
#define  WAPI_AP_STA_TXMAXRATE				(WAPI_AP_STA + 33)
#define  WAPI_AP_STA_TXSINCELASTFAILURE		(WAPI_AP_STA + 34)
#define  WAPI_AP_STA_AP					(WAPI_AP_STA + 35)
#define  WAPI_AP_STA_CHANLLENGE				(WAPI_AP_STA + 36)
#define  WAPI_AP_STA_SSID					(WAPI_AP_STA + 37)
#define  WAPI_AP_STA_CHANNEL					(WAPI_AP_STA + 38)
#define  WAPI_AP_STA_STATUS					(WAPI_AP_STA + 39)

#define  WAPI_AP_MAC					(WAPI_AP + 400)
#define  WAPI_AP_MAC_ENTRIES					(WAPI_AP_MAC + 1)
#define  WAPI_AP_MAC_POLICY					(WAPI_AP_MAC + 2)
#define  WAPI_AP_MAC_INDEX					(WAPI_AP_MAC + 11)
#define  WAPI_AP_MAC_ADDRESS					(WAPI_AP_MAC + 12)
#define  WAPI_AP_MAC_STORAGETYPE				(WAPI_AP_MAC + 13)
#define  WAPI_AP_MAC_STATUS		(WAPI_AP_MAC + 14)
#define  WAPI_AP_MIB_END			(WAPI_AP + 450)

typedef struct p80211ioctl_req
{
	char 	name[WLAN_DEVNAMELEN_MAX];
	void	*data;
	UINT32	magic;
	UINT16	len;
	UINT32	result;
} p80211ioctl_req_t;

typedef struct p80211itemd
{
	UINT32		msgcode;
	UINT32		did;
	UINT16		msgtype;
	UINT16		result;
	UINT16		status;
	UINT16		index;
	UINT8		data[0];
} p80211itemd_t;

typedef struct p80211pstrd
{
	UINT8		len;			
	UINT8		data[0];
} p80211pstrd_t;

#endif

#ifndef _COMM_IOCTL_H
#define _COMM_IOCTL_H
#define ATH
#ifdef ATH
#define WIRELESS_NAME 		"wlan0"
#else
#define WIRELESS_NAME 		"wlan0"
#endif
#define ETHERNET_NAME 		"eth0"

#define DEV_BR				"br0"	
#define WLAN_DEVNAMELEN_MAX  	16
#define MSG_BUFF_LEN            	4000
#define P80211_IOCTL_MAGIC      (0x4a2d464dUL)
#define CURR_KEY_LEN			16
#define WLAN_ALEN				6

#define P80211_PACKET_B2410_SEC         	(unsigned short)0xF501
#define P80211_PACKET_AP_HUNTRER 		(unsigned short)0xF601

#define SIOCIWFIRSTPRIV				0x8BE0	
#define	IEEE80211_IOCTL_MACCMD			17						/*ljy--modified to match driver ioctl num*/
#define	IEEE80211_IOCTL_ADDMAC			(SIOCIWFIRSTPRIV+22)	/*ljy--modified to match driver ioctl num*/
#define	IEEE80211_IOCTL_DELMAC			(SIOCIWFIRSTPRIV+24)	/*ljy--modified to match driver ioctl num*/

#define P80211_IOCTL_MACCMD			IEEE80211_IOCTL_MACCMD
#define P80211_IOCTL_MACADD			IEEE80211_IOCTL_ADDMAC
#define P80211_IOCTL_MACDEL			IEEE80211_IOCTL_DELMAC

#define P80211_IOCTL_WDS_ADD			(SIOCIWFIRSTPRIV+22)	/*ljy--modified to match driver ioctl num*/
#define P80211_IOCTL_WDS_DEL            (SIOCIWFIRSTPRIV+24)	/*ljy--modified to match driver ioctl num*/


#define P80211_IOCTL_SETWAPI_INFO		(SIOCIWFIRSTPRIV + 23)	/*ljy--modified to match driver ioctl num*/

#define IW_MAX_AP_IWN				(unsigned short)32
#define MAX_SSID_LEN				32		
#define ELEMENT_MAX_LEN 			32

#define IW_IS_SET(cmd)  (!((cmd) & 0x1))
#define IW_IS_GET(cmd)  ((cmd) & 0x1)
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
/********************
struct ioctl_drv
{
	unsigned short io_packet;
	struct  _iodata
	{
		unsigned short  wDataLen;
     		unsigned char   pbData[96];
    	}iodata;
}__attribute__ ((packed));
*******************************/
int new_socket_open_for_ioctl();
int new_notify_to_APdriver(UINT16 ioflag, int ioctl_fd,void *buf, int buf_len);
int cmd(unsigned short ioflag, int value);
int new_send_data(char *data_buf, int data_buf_len, const unsigned short packet_type);
int new_send_mac(unsigned short ioflag, char *mac, int *mac_len);
int get_wdsmac(unsigned short ioflag, char *mac, int *mac_len);
int br_device_ioctl(char *br_name, unsigned long arg0,
					unsigned long arg1,
					unsigned long arg2,
					unsigned long arg3);
int get_dev_name(void  *set_data, int *data_len, unsigned long io_type);
int br_addif_dev(char *br_name, char *dev_name);

#endif
