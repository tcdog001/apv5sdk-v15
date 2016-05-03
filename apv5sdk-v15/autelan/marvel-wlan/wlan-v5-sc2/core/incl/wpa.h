/*
*                Copyright 2002-2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

#ifndef _WPA_H_
#define _WPA_H_
typedef enum
{
    KEYMGMT_ERROR,
    KEYMGMTTIMEOUTEVENT, //key management timeout
    GRPKEYTIMEOUTEVENT
}keymgmt_timout_msg_type_e;
typedef UINT8 keymgmt_timout_msg_type_t;

//Rahul
typedef struct
{
    //timer_Data_t timerData;
    //uint32   Id;
    IEEEtypes_MacAddr_t StnAddr;
    keymgmt_timout_msg_type_t type;
}
dist_PendingData_t;

/*----------------*/
/* Timer Messages */
/*----------------*/

typedef struct
{
    dist_PendingData_t PendingData_p;
    UINT8 Id;
}
distQ_TimerMsg_t;

typedef enum{
    WPAEVT_STA_AUTHENTICATED,
    WPAEVT_STA_ASSOCIATED,
    WPAEVT_STA_DEAUTHENTICATED,
    WPAEVT_STA_DISASSOCIATED,
    WPAEVT_STA_AUTHENTICATE_FAIL,
}WPA_ASSOC_TYPE;
typedef UINT8 WPA_ASSOC_TYPE_t;

typedef struct _StaAssocStateMsg
{
    unsigned char staMACAddr[6];
    WPA_ASSOC_TYPE_t assocType;
}
StaAssocStateMsg_t;

typedef enum
{
    STA_ASSOMSGRECVD,
    TIMERMSGRECVD,
    KEYMGMTINITMSGRECVD
}MsgType_e;
typedef UINT8 MsgType_t;

typedef struct
{
    MsgType_t MsgType;
    union
    {
        StaAssocStateMsg_t StaAssocStateMsg;
        distQ_TimerMsg_t distQ_TimerMsg;
    }msg;
}
DistTaskMsg_t;

#endif
