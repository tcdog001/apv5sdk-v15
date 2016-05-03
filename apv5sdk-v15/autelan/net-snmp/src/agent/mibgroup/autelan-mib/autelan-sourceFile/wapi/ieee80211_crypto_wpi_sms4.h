/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: ieee80211_crypto_wpi_sms4.h
* description:  implementation for wpi-related variables definitions.
* 
*
* 
************************************************************************************/

#ifndef __CRYPTO_WPI_SMS4__H
#define __CRYPTO_WPI_SMS4__H
/* authenticate  mode */
#define	IEEE80211_AUTH_WAI	3

//#define	IEEE80211_F_WPA1	0x00800000	/* CONF: WPA enabled */
//#define	IEEE80211_F_WPA2	0x01000000	/* CONF: WPA2 enabled */
#define	IEEE80211_F_WAPI		0x02000000	/* CONF: WAPI enabled */
//#define	IEEE80211_F_WPA		0x03800000	/* CONF: WAPI/WPA/WPA2 enabled */

#define	IEEE80211_WPI_SMS4_KEYLEN		16	/* 128bit */
#define	IEEE80211_WPI_SMS4_IVLEN		16	/* 24bit */
#define	IEEE80211_WPI_SMS4_KIDLEN		1	/* 1 octet */
#define	IEEE80211_WPI_SMS4_PADLEN		1	/* 1 octet */
#define	IEEE80211_WPI_SMS4_CRCLEN		4	/* CRC-32 */
#define	IEEE80211_WPI_SMS4_MICLEN		16	/* trailing MIC */

#define	WAPI_OUI			0x721400
#define	WAPI_VERSION		1		/* current supported version */

/*Cipher suite*/
#define	WAPI_CSE_NULL			0x00 
#define	WAPI_CSE_WPI_SMS4		0x01

/*AKM suite */
#define	WAPI_ASE_NONE			0x00 
#define	WAPI_ASE_WAI_UNSPEC	0x01
#define	WAPI_ASE_WAI_PSK		0x02

#define	IEEE80211_WPI_NKID	2
#define	INSTALL_KEY_MAX_INACTIVITY 9

#define	USKREKEYCOUNT		0x4000000//0xA00000
#define	MSKREKEYCOUNT		0x4000000//0xA00000
#endif

