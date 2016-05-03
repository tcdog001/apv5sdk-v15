#ifndef NASFUNCEXPORT_H
#define NASFUNCEXPORT_H
#include "CommonDefs.h"
/*WNC_ZYX 20110720, add to show the format of nas message call.*/

//This file is called for NAS Message

//get signal strength
//QMI_NAS_GET_SIGNAL_STRENGTH
struct SignalStrengthListEntry
{
   int8 signalStrength;
   uint8 radioIf;
};

//{WNC:ricky 20120312: add RF Band info
struct RfBandInfoListEntry
{
  uint8 radioIf;
  uint16 activeBand;
  uint16 activeChannel;
};
//}

//WNC Helen 20120830: add for network scan
struct ThreeGPPNetworkInfoEntry
{
    uint16 mobile_country_code;
    uint16 mobile_network_code;
    uint8  network_status;  
};

struct NetworkRadioEntry
{
   uint16 mcc;
   uint16 mnc;
   uint8  rat;
};
//WNC Helen 20120830 END: add for network scan

#endif
