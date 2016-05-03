/*
*                Copyright 2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

#include "ap8xLnxAuthMode.h"
#include "ap8xLnxIoctlWiress.h"
#include <wlvmac.h>
#include <mlmeApi.h>
#include "wl.h"

int set_ieee80211_authmode(vmacApInfo_t *vmacSta_p, MIB_802DOT11 *mib, int value)
{
    int rc = 0;

//	return rc;
#ifdef MRVL_WPS_CLIENT
    vmacEntry_t  *vmacEntry_p = NULL;
    STA_SYSTEM_MIBS *pStaSystemMibs ;
#endif
    if ((value & 0x0000000F) < 0 || (value & 0x0000000F) > 6)
    {
        return -EOPNOTSUPP;
    }

#ifdef MRVL_WPS_CLIENT
    if((vmacEntry_p = sme_GetParentVMacEntry(vmacSta_p->VMacEntry.phyHwMacIndx)) != NULL)
    {
        pStaSystemMibs = sme_GetStaSystemMibsPtr(vmacEntry_p);
        if( pStaSystemMibs != NULL)
        {
            pStaSystemMibs->mib_StaCfg_p->wpawpa2Mode = value ;
        }
    }
#endif

/***************************************************
yuanding annotation to fix bug 
the new implement is in set_80211_keymgt_mode () 
***************************************************/
/*
    if (value == 0)
    {
        //set encryption disable
        mib->Privacy->RSNEnabled = 0;
        mib->RSNConfigWPA2->WPA2Enabled = 0;
        mib->RSNConfigWPA2->WPA2OnlyEnabled = 0;
    }
    else
    {  
        if(*(mib->mib_wpaWpa2Mode) != 2)
        {
            //set authmode
            mib->RSNConfigAuthSuites->AuthSuites[0] = 0x00;
            mib->RSNConfigAuthSuites->AuthSuites[1] = 0x50;
            mib->RSNConfigAuthSuites->AuthSuites[2] = 0xF2;
            mib->RSNConfigAuthSuites->AuthSuites[3] = 0x01;      // Auth8021x

            mib->RSNConfigAuthSuites->Enabled = TRUE;
            *(mib->mib_cipherSuite) = 2;
        }
        else
        {
            mib->WPA2AuthSuites->AuthSuites[0] = 0x00;
            mib->WPA2AuthSuites->AuthSuites[1] = 0x0F;
            mib->WPA2AuthSuites->AuthSuites[2] = 0xAC;
            mib->WPA2AuthSuites->AuthSuites[3] = 0x01;          // Auth8021x                    
            mib->WPA2AuthSuites->Enabled = TRUE;
            *(mib->mib_cipherSuite) = 4;
        }
    }
*/
    return rc;
}

int set_80211_privacy(vmacApInfo_t *vmacSta_p, MIB_802DOT11 *mib, int value)
{
    int rc = 0;
    //return rc;
    switch(value)
    {   
        case 1:
        {
            //set wpawpa2 disable
            mib->Privacy->RSNEnabled = 0;
            mib->RSNConfigWPA2->WPA2Enabled = 0;
            mib->RSNConfigWPA2->WPA2OnlyEnabled = 0;
            //set wep enable
            mib->AuthAlg->Enable = 1;
            mib->StationConfig->PrivOption = 1;
            mib->Privacy->PrivInvoked = 1;

            break;
        }
        default:
            break;
    }

    return rc;
}


/******************************************************
	yuanding add
	
	function : set the key management mode ( PSK , 802.1x )


*******************************************************/
int set_80211_keymgt_mode(vmacApInfo_t *vmacSta_p, MIB_802DOT11 *mib, int value)

{
	int err = -1;
	if(NULL == vmacSta_p || NULL == mib || value <0 )

	{
		WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "null pointer ,fail");
		return err; 
	}

#ifdef MRVL_WSC

    if ((value != 0) || ((value & 0x0000000F) != 0))
#else
        if (value != 0)
#endif
        {
			//wpa
			if( *(mib->mib_wpaWpa2Mode) == 1)
			{
                WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "wpa");
                mib->RSNConfigAuthSuites->AuthSuites[0] = 0x00;
                mib->RSNConfigAuthSuites->AuthSuites[1] = 0x50;
                mib->RSNConfigAuthSuites->AuthSuites[2] = 0xF2;

                if ( value ==1 )
                    mib->RSNConfigAuthSuites->AuthSuites[3] = 0x01;      // Auth8021x
                else
                    mib->RSNConfigAuthSuites->AuthSuites[3] = 0x02;      // AuthPSK

				mib->RSNConfigAuthSuites->Enabled = TRUE;
                *(mib->mib_cipherSuite) = 2;
			}

			//wpa2
    		else if( *(mib->mib_wpaWpa2Mode) == 2 )
			{
				WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "wpa2");

                mib->RSNConfigWPA2->WPA2Enabled = 1;
                mib->RSNConfigWPA2->WPA2OnlyEnabled = 1;

                mib->WPA2AuthSuites->AuthSuites[0] = 0x00;
                mib->WPA2AuthSuites->AuthSuites[1] = 0x0F;
                mib->WPA2AuthSuites->AuthSuites[2] = 0xAC;

                if ( value == 1 )
                    mib->WPA2AuthSuites->AuthSuites[3] = 0x01;          // Auth8021x
                else
                    mib->WPA2AuthSuites->AuthSuites[3] = 0x02;          // AuthPSK

                mib->WPA2AuthSuites->Enabled = TRUE;
                *(mib->mib_cipherSuite) = 4;
            } 
			//wpa/wpa2 mixed
            else
            {
           		WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "wpa/wpa2 mixed");
                mib->RSNConfigWPA2->WPA2Enabled = 1;
                mib->RSNConfigWPA2->WPA2OnlyEnabled = 0;
				
                mib->WPA2AuthSuites->AuthSuites[0] = 0x00;
                mib->WPA2AuthSuites->AuthSuites[1] = 0x0F;
                mib->WPA2AuthSuites->AuthSuites[2] = 0xAC;

                if ( value == 1 )
                    mib->WPA2AuthSuites->AuthSuites[3] = 0x01;          // Auth8021x
                else
                    mib->WPA2AuthSuites->AuthSuites[3] = 0x02;          // AuthPSK

                mib->WPA2AuthSuites->Enabled = TRUE;
                *(mib->mib_cipherSuite) = 4;
            }
            err = 0;
        }
	return err;
}
