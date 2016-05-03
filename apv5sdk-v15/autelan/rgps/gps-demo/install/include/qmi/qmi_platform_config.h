
/******************************************************************************
  @file    qmi_platform_config.h
  @brief   Platform-specific external QMI definitions.

  DESCRIPTION
  This file contains platform specific configuration definitions
  for QMI interface library.


  INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  $Header: $ 
  $DateTime: $
  ---------------------------------------------------------------------------
  Copyright (c) 2007 - 2009 QUALCOMM Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/

#ifndef QMI_PLATFORM_CONFIG_H
#define QMI_PLATFORM_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum length of QMI port name (including null terminator) */
#define QMI_DEVICE_NAME_SIZE (100)

/* smd */
#define QMI_PORT_RMNET_0  "rmnet0"
#define QMI_PORT_RMNET_1  "rmnet1"
#define QMI_PORT_RMNET_2  "rmnet2"
#define QMI_PORT_RMNET_3  "rmnet3"
#define QMI_PORT_RMNET_4  "rmnet4"
#define QMI_PORT_RMNET_5  "rmnet5"
#define QMI_PORT_RMNET_6  "rmnet6"
#define QMI_PORT_RMNET_7  "rmnet7"
#define QMI_PORT_RMNET_8  "rmnet8"
/* sdio */
#define QMI_PORT_RMNET_SDIO_0 "rmnet_sdio0"
#define QMI_PORT_RMNET_SDIO_1 "rmnet_sdio1"
#define QMI_PORT_RMNET_SDIO_2 "rmnet_sdio2"
#define QMI_PORT_RMNET_SDIO_3 "rmnet_sdio3"
#define QMI_PORT_RMNET_SDIO_4 "rmnet_sdio4"
#define QMI_PORT_RMNET_SDIO_5 "rmnet_sdio5"
#define QMI_PORT_RMNET_SDIO_6 "rmnet_sdio6"
#define QMI_PORT_RMNET_SDIO_7 "rmnet_sdio7"
/* usb */
#define QMI_PORT_RMNET_USB_0 "rmnet_usb0"
#define QMI_PORT_RMNET_USB_1 "rmnet_usb1"
#define QMI_PORT_RMNET_USB_2 "rmnet_usb2"
#define QMI_PORT_RMNET_USB_3 "rmnet_usb3"
#define QMI_PORT_RMNET_USB_4 "rmnet_usb4"
#define QMI_PORT_RMNET_USB_5 "rmnet_usb5"
#define QMI_PORT_RMNET_USB_6 "rmnet_usb6"
#define QMI_PORT_RMNET_USB_7 "rmnet_usb7"

#define QMI_PORT_RMNET_SMUX_0 "rmnet_smux0"

/* Virtual port */
#define QMI_PORT_PROXY        "qmi_proxy"

/* This define is used to tell QMI library which port should be used for 
** internal QMI library communications with modem.  
** If you want to change the default port that is used internally in QMI 
** library, set the QMI_PLATFORM_INTERNAL_USE_PORT_ID #define to the string/function 
** you want to use as your default internal port.  For example if I wanted that port
** to be the RMNET_2 QMI port, I would define as follows:

#define QMI_PLATFORM_INTERNAL_USE_PORT_ID QMI_PORT_RMNET_2

** By default (this value not defined), QMI_PORT_RMNET_0 is default 
*/

/* This function will be defined in qmi_platform.c */
extern
const char * qmi_linux_get_internal_use_port
(
  void
);

#define QMI_PLATFORM_INTERNAL_USE_PORT_ID qmi_linux_get_internal_use_port()


#ifdef __cplusplus
}
#endif

#endif /* QMI_PLATFORM_CONFIG_H */

