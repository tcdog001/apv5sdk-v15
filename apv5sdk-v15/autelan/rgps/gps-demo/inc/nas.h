#ifndef NAS_H
#define NAS_H

int initWDSResp(void);

#ifdef NAS_SUPPORT
/*WNC_ZYX 20110720 add for init NAS response functions.*/
int initNASResp(void);
#endif

#define CONFIG_LED_NUM1 0
#define CONFIG_LED_NUM2 7

// GARYeh 20120312: Add network provider
typedef enum uinetwk_network_e_type
{
UI_NETWK_TYPE_UNKNOWN_TYPE,
UI_NETWK_TYPE_GSM_900,  
UI_NETWK_TYPE_DCS_1800,
UI_NETWK_TYPE_PCS_1900,
UI_NETWK_TYPE_GSM_SAT,
UI_NETWK_TYPE_UMTS,
UI_NETWK_TYPE_CDMA_2000,
UI_NETWK_TYPE_LTE

}uinetwk_network_e_type;

// GARYeh 20120312: Add network provider
typedef struct uinetwk_network_info_s_type 
{
    unsigned int  mcc;
    unsigned int mnc;
    uinetwk_network_e_type network_type;
    char * short_name_ptr;
    char * full_name_ptr;
}uinetwk_network_info_s_type;


enum ENetworkSelectMode
{
    ENSM_NONE = 0x00,
    ENSM_GLOBALE = 0x01,
    ENSM_LTE = 0x02,
    ENSM_WCDMA = 0x04,
};

#endif

