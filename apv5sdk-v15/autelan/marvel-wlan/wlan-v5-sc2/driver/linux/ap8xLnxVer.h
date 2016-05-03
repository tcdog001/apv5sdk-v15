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

#ifndef AP8X_VER_H_
#define AP8X_VER_H_

#ifdef WL_KERNEL_26
    #define OS_SUFFIX
#else
    #define OS_SUFFIX "-L24"
#endif

#ifdef SOC_W8363
    #ifdef WLMEM_DISABLED
        #define SOC_SUFFIX "-S"
    #else
        #define SOC_SUFFIX "-x"
    #endif
    #if NUMOFAPS != 1
        #ifdef V4FW
            #define FEATURE_SUFFIX "-MBSS-V4FW"
        #else
            #define FEATURE_SUFFIX "-MBSS"
        #endif
    #else
        #ifdef V4FW
            #define FEATURE_SUFFIX "-V4FW"
        #else
            #define FEATURE_SUFFIX
        #endif
    #endif
#elif defined(SOC_W8366)
    #define FEATURE_SUFFIX
    #define SOC_SUFFIX  "-W8366"
#elif defined(SOC_W8364)
    #define FEATURE_SUFFIX
    #define SOC_SUFFIX  "-W8364"
#elif defined(SOC_W8764)
    #define FEATURE_SUFFIX
    #define SOC_SUFFIX  "-W8764"
#else
    #define SOC_SUFFIX  "-W8363"
#endif

#ifdef AP82S
    #define PLATFORM_SUFFIX "(AP82S)"
#else
    #define PLATFORM_SUFFIX
#endif

//add by zhanxuechao for interface name 
#ifdef WL_KERNEL_26 /* %1d needed for L26; but not ok for L24 */
    #define DRV_NAME      "wifi%1d"
#else
    #define DRV_NAME      "wifi%d"
#endif

#define DRV_NAME_WDS  "%swds%1d"
#define DRV_NAME_VMAC "ath.%1d-%1d"
#define DRV_NAME_CLIENT "ath.%1d-sta%1d"

#define DRV_VERSION   "6.5.0.3" SOC_SUFFIX OS_SUFFIX PLATFORM_SUFFIX FEATURE_SUFFIX

#endif /* AP8X_VER_H_ */

