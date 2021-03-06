//WNC \main\1               --          10083609     2012/01/09 10:33:01   
//WNC add the gobi driver by kathy                                   
//WNC ------------------------------------------------------------------    
//WNC \main\13              --          10083609     2011/12/21 11:48:51   
//WNC modify leds define and action for led spec changed             
//WNC ------------------------------------------------------------------    
//WNC \main\12              --          10083609     2011/12/07 15:38:32   
//WNC solve the sys led blinking bug                                 
//WNC ------------------------------------------------------------------    
//WNC \main\11              --          05071100     2011/11/30 14:41:09   
//WNC WNCNJ: Awhyni Add for UDP buffer control@20111130              
//WNC ------------------------------------------------------------------    
//WNC \main\9               --          10083609     2011/11/08 17:44:54   
//WNC modify the suspend time to 150s                                
//WNC ------------------------------------------------------------------    
//WNC \main\8               --          10083609     2011/10/17 15:18:03   
//WNC add the leds control about lab2                                
//WNC ------------------------------------------------------------------    
//WNC \main\5               --          10083609     2011/09/28 11:21:54   
//WNC update the usb suspend time                                    
//WNC ------------------------------------------------------------------    
/*===========================================================================
FILE:
   QCUSBNetDell.c

DESCRIPTION:
   Qualcomm USB Network device for Gobi 2000 (Dell)
   
FUNCTIONS:
   QCSuspend
   QCResume
   QCNetDriverBind
   QCNetDriverUnbind
   QCUSBNetURBCallback
   QCUSBNetTXTimeout
   QCUSBNetAutoPMThread
   QCUSBNetStartXmit
   QCUSBNetOpen
   QCUSBNetStop
   QCUSBNetProbe
   QCUSBNetModInit
   QCUSBNetModExit

Copyright (c) 2011, Code Aurora Forum. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Code Aurora Forum nor
      the names of its contributors may be used to endorse or promote
      products derived from this software without specific prior written
      permission.

Alternatively, provided that this notice is retained in full, this software
may be relicensed by the recipient under the terms of the GNU General Public
License version 2 ("GPL") and only version 2, in which case the provisions of
the GPL apply INSTEAD OF those given above.  If the recipient relicenses the
software under the GPL, then the identification text in the MODULE_LICENSE
macro must be changed to reflect "GPLv2" instead of "Dual BSD/GPL".  Once a
recipient changes the license terms to the GPL, subsequent recipients shall
not relicense under alternate licensing terms, including the BSD or dual
BSD/GPL terms.  In addition, the following license statement immediately
below and between the words START and END shall also then apply when this
software is relicensed under the GPL:

START

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 and only version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

END

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
===========================================================================*/

//---------------------------------------------------------------------------
// Include Files
//---------------------------------------------------------------------------

#include "QMIDevice.h"
#include <linux/usb.h> 
#include <linux/tty.h>
#include <linux/tty_flip.h>
/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
#include <linux/netdevice.h>
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/



//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

// Version Information
#define DRIVER_VERSION "1.0.170"
#define DRIVER_AUTHOR "Qualcomm Innovation Center"
#define DRIVER_DESC "QCUSBNet2kDell"
#define DRIVER_INF 3
//#define USBNETAUTOPM

// Debug flag
int debug = 0;

// Wait 5 seconds after enumeration for firmware to be ready?
int safeEnumDelay = 1;

// Allow user interrupts
int interruptible = 1;

// Class should be created during module init, so needs to be global
static struct class * gpClass;
/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
static unsigned int TX_Queue_len = 0;
struct net_device *gNet = NULL;
#define TX_QUEUE_MAX_LEN   2000
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
/*===========================================================================
METHOD:
   QCSuspend (Public Method)

DESCRIPTION:
   Stops QMI traffic while device is suspended

PARAMETERS
   pIntf          [ I ] - Pointer to interface
   powerEvent     [ I ] - Power management event

RETURN VALUE:
   int - 0 for success
         negative errno for failure
===========================================================================*/
int QCSuspend( 
   struct usb_interface *     pIntf,
   pm_message_t               powerEvent )
{
   struct usbnet * pDev;
   sQCUSBNet * pQCDev;
   printk(KERN_ALERT"->->->->->->QCSuspend->->->->->->->->\n");
   if (pIntf == 0)
   {
      return -ENOMEM;
   }
   
#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,23 ))
   pDev = usb_get_intfdata( pIntf );
#else
   pDev = (struct usbnet *)pIntf->dev.platform_data;
#endif

   if (pDev == NULL || pDev->net == NULL)
   {
      DBG( "failed to get netdevice\n" );
      return -ENXIO;
   }
   
   pQCDev = (sQCUSBNet *)pDev->data[0];
   if (pQCDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return -ENXIO;
   }

   // Is this autosuspend or system suspend?
   //    do we allow remote wakeup?
#ifdef CONFIG_PM
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,33 ))
   if (pDev->udev->auto_pm == 0)
#else
   if ((powerEvent.event & PM_EVENT_AUTO) == 0)
#endif
   {
      DBG( "device suspended to power level %d\n", 
           powerEvent.event );
      QSetDownReason( pQCDev, DRIVER_SUSPENDED );
   }
   else
   {
      DBG( "device autosuspend\n" );
   }

   if (powerEvent.event & PM_EVENT_SUSPEND)
   {
      // Stop QMI read callbacks
      KillRead( pQCDev );
      pDev->udev->reset_resume = 0;

      // Store power state to avoid duplicate resumes
      pIntf->dev.power.power_state.event = powerEvent.event;
   }
   else
   {
      // Other power modes cause QMI connection to be lost
      pDev->udev->reset_resume = 1;
   }
   #endif //CONFIG_PM
   // Run usbnet's suspend function
   return usbnet_suspend( pIntf, powerEvent );
}
   
/*===========================================================================
METHOD:
   QCResume (Public Method)

DESCRIPTION:
   Resume QMI traffic or recreate QMI device

PARAMETERS
   pIntf          [ I ] - Pointer to interface

RETURN VALUE:
   int - 0 for success
         negative errno for failure
===========================================================================*/
int QCResume( struct usb_interface * pIntf )
{
   struct usbnet * pDev;
   sQCUSBNet * pQCDev;
   int nRet;
   int oldPowerState;
   printk(KERN_ALERT"->->->->->->QCResume->->->->->->->->\n");
   if (pIntf == 0)
   {
      return -ENOMEM;
   }
   
#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,23 ))
   pDev = usb_get_intfdata( pIntf );
#else
   pDev = (struct usbnet *)pIntf->dev.platform_data;
#endif

   if (pDev == NULL || pDev->net == NULL)
   {
      DBG( "failed to get netdevice\n" );
      return -ENXIO;
   }
   
   pQCDev = (sQCUSBNet *)pDev->data[0];
   if (pQCDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return -ENXIO;
   }

   oldPowerState = pIntf->dev.power.power_state.event;
   pIntf->dev.power.power_state.event = PM_EVENT_ON;
   DBG( "resuming from power mode %d\n", oldPowerState );

   if (oldPowerState & PM_EVENT_SUSPEND)
   {
      // It doesn't matter if this is autoresume or system resume
      QClearDownReason( pQCDev, DRIVER_SUSPENDED );
   
      nRet = usbnet_resume( pIntf );
      if (nRet != 0)
      {
         DBG( "usbnet_resume error %d\n", nRet );
         return nRet;
      }

      // Restart QMI read callbacks
      nRet = StartRead( pQCDev );
      if (nRet != 0)
      {
         DBG( "StartRead error %d\n", nRet );
         return nRet;
      }

      // Kick Auto PM thread to process any queued URBs
      complete( &pQCDev->mAutoPM.mThreadDoWork );
   }
   else
   {
      DBG( "nothing to resume\n" );
      return 0;
   }
   return nRet;
}

/*===========================================================================
METHOD:
   QCNetDriverBind (Public Method)

DESCRIPTION:
   Setup in and out pipes

PARAMETERS
   pDev           [ I ] - Pointer to usbnet device
   pIntf          [ I ] - Pointer to interface

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
static int QCNetDriverBind( 
   struct usbnet *         pDev, 
   struct usb_interface *  pIntf )
{
   int numEndpoints;
   int endpointIndex;
   struct usb_host_endpoint * pEndpoint = NULL;
   struct usb_host_endpoint * pIn = NULL;
   struct usb_host_endpoint * pOut = NULL;
   
   // Verify one altsetting
   if (pIntf->num_altsetting != 1)
   {
      DBG( "invalid num_altsetting %u\n", pIntf->num_altsetting );
      return -ENODEV;
   }

   // Verify correct interface (0)
   //if (pIntf->cur_altsetting->desc.bInterfaceNumber != 0)
   /* Eaddy 20100909 : Porting 9600 driver */
   if (pIntf->cur_altsetting->desc.bInterfaceNumber != DRIVER_INF) 
   {
      DBG( "invalid interface %d\n", 
           pIntf->cur_altsetting->desc.bInterfaceNumber );
	  printk(KERN_ALERT"\n\n\n^^^^^^^^^^invalid interface %d\n\n", 
           pIntf->cur_altsetting->desc.bInterfaceNumber );
      return -ENODEV;
   }
   
   // Collect In and Out endpoints
   numEndpoints = pIntf->cur_altsetting->desc.bNumEndpoints;
   for (endpointIndex = 0; endpointIndex < numEndpoints; endpointIndex++)
   {
      pEndpoint = pIntf->cur_altsetting->endpoint + endpointIndex;
      if (pEndpoint == NULL)
      {
         DBG( "invalid endpoint %u\n", endpointIndex );
         return -ENODEV;
      }
      
      if (usb_endpoint_dir_in( &pEndpoint->desc ) == true
      &&  usb_endpoint_xfer_int( &pEndpoint->desc ) == false)
      {
         pIn = pEndpoint;
      }
      else if (usb_endpoint_dir_out( &pEndpoint->desc ) == true)
      {
         pOut = pEndpoint;
      }
   }
   
   if (pIn == NULL || pOut == NULL)
   {
      DBG( "invalid endpoints\n" );
      return -ENODEV;
   }

   if (usb_set_interface( pDev->udev, 
                          pIntf->cur_altsetting->desc.bInterfaceNumber,
                          0 ) != 0)
   {
      DBG( "unable to set interface\n" );
      return -ENODEV;
   }

   pDev->in = usb_rcvbulkpipe( pDev->udev,
                   pIn->desc.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK );
   pDev->out = usb_sndbulkpipe( pDev->udev,
                   pOut->desc.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK );
                   
   DBG( "in %x, out %x\n", 
        pIn->desc.bEndpointAddress, 
        pOut->desc.bEndpointAddress );

   // In later versions of the kernel, usbnet helps with this
#if (LINUX_VERSION_CODE <= KERNEL_VERSION( 2,6,23 ))
   pIntf->dev.platform_data = (void *)pDev;
#endif

   return 0;
}

/*===========================================================================
METHOD:
   QCNetDriverUnbind (Public Method)

DESCRIPTION:
   Deregisters QMI device (Registration happened in the probe function)

PARAMETERS
   pDev           [ I ] - Pointer to usbnet device
   pIntfUnused    [ I ] - Pointer to interface

RETURN VALUE:
   None
===========================================================================*/
static void QCNetDriverUnbind( 
   struct usbnet *         pDev, 
   struct usb_interface *  pIntf)
{
   sQCUSBNet * pQCDev = (sQCUSBNet *)pDev->data[0];

   // Should already be down, but just in case...
   netif_carrier_off( pDev->net );

   DeregisterQMIDevice( pQCDev );
   
#if (LINUX_VERSION_CODE >= KERNEL_VERSION( 2,6,29 ))
   kfree( pDev->net->netdev_ops );
   pDev->net->netdev_ops = NULL;
#endif

#if (LINUX_VERSION_CODE <= KERNEL_VERSION( 2,6,23 ))
   pIntf->dev.platform_data = NULL;
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION( 2,6,19 ))
   pIntf->needs_remote_wakeup = 0;
#endif

   kfree( pQCDev );
   pQCDev = NULL;
}

/*===========================================================================
METHOD:
   QCUSBNetURBCallback (Public Method)

DESCRIPTION:
   Write is complete, cleanup and signal that we're ready for next packet

PARAMETERS
   pURB     [ I ] - Pointer to sAutoPM struct

RETURN VALUE:
   None
===========================================================================*/
void QCUSBNetURBCallback( struct urb * pURB )
{
   unsigned long activeURBflags;
   sAutoPM * pAutoPM = (sAutoPM *)pURB->context;
/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
//	 printk( KERN_ERR "QCUSBNetURBCallback start\n");
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
   
   if (pAutoPM == NULL)
   {
      // Should never happen
      DBG( "bad context\n" );
      return;
   }

   if (pURB->status != 0)
   {
      // Note that in case of an error, the behaviour is no different
      DBG( "urb finished with error %d\n", pURB->status );
      /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
 	//printk( KERN_ERR "urb finished with error %d\n", pURB->status);
      /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
	 
   }

   // Remove activeURB (memory to be freed later)
   spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );

   // EAGAIN used to signify callback is done
   pAutoPM->mpActiveURB = ERR_PTR( -EAGAIN );

   spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );

   complete( &pAutoPM->mThreadDoWork );
   
   usb_free_urb( pURB );
 /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
//    printk( KERN_ERR "QCUSBNetURBCallback end\n");
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/  
}

/*===========================================================================
METHOD:
   QCUSBNetTXTimeout (Public Method)

DESCRIPTION:
   Timeout declared by the net driver.  Stop all transfers

PARAMETERS
   pNet     [ I ] - Pointer to net device

RETURN VALUE:
   None
===========================================================================*/
void QCUSBNetTXTimeout( struct net_device * pNet )
{
   struct sQCUSBNet * pQCDev;
   sAutoPM * pAutoPM;
   sURBList * pURBListEntry;
   unsigned long activeURBflags, URBListFlags;
   struct usbnet * pDev = netdev_priv( pNet );
   struct urb * pURB;
   /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   // printk( KERN_ERR "QCUSBNetTXTimeout start\n");
   /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
 
   if (pDev == NULL || pDev->net == NULL)
   {
 /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
 //  printk( KERN_ERR "QCUSBNetTXTimeout err1\n");
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/  	
      DBG( "failed to get usbnet device\n" );
      return;
   }
   
   pQCDev = (sQCUSBNet *)pDev->data[0];
   if (pQCDev == NULL)
   {
 /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
  //  printk( KERN_ERR "QCUSBNetTXTimeout err2\n");
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/  	
      DBG( "failed to get QMIDevice\n" );
      return;
   }
   pAutoPM = &pQCDev->mAutoPM;

   DBG( "\n" );

   // Grab a pointer to active URB
   spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );
   pURB = pAutoPM->mpActiveURB;
   spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );

   // Stop active URB
   if (pURB != NULL)
   {
      usb_kill_urb( pURB );
   }

   // Cleanup URB List
   spin_lock_irqsave( &pAutoPM->mURBListLock, URBListFlags );

   pURBListEntry = pAutoPM->mpURBList;
   while (pURBListEntry != NULL)
   {
      pAutoPM->mpURBList = pAutoPM->mpURBList->mpNext;
      usb_free_urb( pURBListEntry->mpURB );
      kfree( pURBListEntry );
      pURBListEntry = pAutoPM->mpURBList;
   }
/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   TX_Queue_len = 0;
   pAutoPM->mpURBList = NULL;
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
   spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );

   complete( &pAutoPM->mThreadDoWork );
   /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   //printk( KERN_ERR "QCUSBNetTXTimeout end\n");
   /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
    return;
}

/*===========================================================================
METHOD:
   QCUSBNetAutoPMThread (Public Method)

DESCRIPTION:
   Handle device Auto PM state asynchronously
   Handle network packet transmission asynchronously

PARAMETERS
   pData     [ I ] - Pointer to sAutoPM struct

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
static int QCUSBNetAutoPMThread( void * pData )
{
   unsigned long activeURBflags, URBListFlags;
   sURBList * pURBListEntry;
   int status;
   struct usb_device * pUdev;
   sAutoPM * pAutoPM = (sAutoPM *)pData;
   struct urb * pURB;
   /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
//   printk( KERN_ERR "QCUSBNetAutoPMThread start\n");
   /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/

   if (pAutoPM == NULL)
   {
      DBG( "passed null pointer\n" );
      return -EINVAL;
   }   
   pUdev = interface_to_usbdev( pAutoPM->mpIntf );

   DBG( "traffic thread started\n" );

   while (pAutoPM->mbExit == false)
   {
      // Wait for someone to poke us
      wait_for_completion_interruptible( &pAutoPM->mThreadDoWork );
	     
      // Time to exit?
      if (pAutoPM->mbExit == true)
      {
         // Stop activeURB
         /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
	 //    printk( KERN_ERR "pAutoPM->mbExit:%d\n",pAutoPM->mbExit);
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
     
         spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );
         pURB = pAutoPM->mpActiveURB;
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );

         if (pURB != NULL)
         {
            usb_kill_urb( pURB );
         }
         // Will be freed in callback function

         // Cleanup URB List
         spin_lock_irqsave( &pAutoPM->mURBListLock, URBListFlags );

         pURBListEntry = pAutoPM->mpURBList;
         while (pURBListEntry != NULL)
         {
            pAutoPM->mpURBList = pAutoPM->mpURBList->mpNext;
            usb_free_urb( pURBListEntry->mpURB );
            kfree( pURBListEntry );
            pURBListEntry = pAutoPM->mpURBList;
         }
/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
         TX_Queue_len = 0;
	 pAutoPM->mpURBList = NULL;
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
	spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );

         break;
      }
      
      // Is our URB active?
      spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );

      // EAGAIN used to signify callback is done
      if (IS_ERR( pAutoPM->mpActiveURB ) 
      &&  PTR_ERR( pAutoPM->mpActiveURB ) == -EAGAIN )
      {
      	/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   	// printk( KERN_ERR "pAutoPM->mpActiveURB EAGAIN");
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/

         pAutoPM->mpActiveURB = NULL;

         // Restore IRQs so task can sleep
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );
         
         // URB is done, decrement the Auto PM usage count
         usb_autopm_put_interface( pAutoPM->mpIntf );

         // Lock ActiveURB again
         spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );
      }

      if (pAutoPM->mpActiveURB != NULL)
      {
         // There is already a URB active, go back to sleep
         /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
         // printk( KERN_ERR "pAutoPM->mpActiveURB != NULL");
	 /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
 
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );
         continue;
      }
      
      // Is there a URB waiting to be submitted?
      spin_lock_irqsave( &pAutoPM->mURBListLock, URBListFlags );
      if (pAutoPM->mpURBList == NULL)
      {
      	/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   	// printk( KERN_ERR "pAutoPM->mpURBList == NULL");
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/

         // No more URBs to submit, go back to sleep
         spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );
         continue;
      }

      // Pop an element
      pURBListEntry = pAutoPM->mpURBList;
      pAutoPM->mpURBList = pAutoPM->mpURBList->mpNext;
	/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
          TX_Queue_len--;
          if(TX_Queue_len == TX_QUEUE_MAX_LEN)
             netif_wake_queue (gNet);
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
      spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );

      // Set ActiveURB
      pAutoPM->mpActiveURB = pURBListEntry->mpURB;
      spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );

      // Tell autopm core we need device woken up
      
      status = usb_autopm_get_interface( pAutoPM->mpIntf );
      if (status < 0)
      {
      	/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
 	 //printk( KERN_ERR "unable to autoresume interface");
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
   
         DBG( "unable to autoresume interface: %d\n", status );

         // likely caused by device going from autosuspend -> full suspend
         if (status == -EPERM)
         {
         /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
  	// printk( KERN_ERR "EPERM");
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
   
#ifdef CONFIG_PM
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,33 ))
            pUdev->auto_pm = 0;
#endif
#endif //CONFIG_PM
             
            QCSuspend( pAutoPM->mpIntf, PMSG_SUSPEND );
         }

         // Add pURBListEntry back onto pAutoPM->mpURBList
         spin_lock_irqsave( &pAutoPM->mURBListLock, URBListFlags );
         pURBListEntry->mpNext = pAutoPM->mpURBList;
         pAutoPM->mpURBList = pURBListEntry;
         spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );
         
         spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );
         pAutoPM->mpActiveURB = NULL;
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );
         
         // Go back to sleep
         continue;
      }

      // Submit URB
      status = usb_submit_urb( pAutoPM->mpActiveURB, GFP_KERNEL );
      if (status < 0)
      {
      	/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
 	 //printk( KERN_ERR "Failed to submit URB: %d.  Packet dropped\n", status);
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
  
         // Could happen for a number of reasons
         DBG( "Failed to submit URB: %d.  Packet dropped\n", status );
         spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );
         usb_free_urb( pAutoPM->mpActiveURB );
         pAutoPM->mpActiveURB = NULL;
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );
         usb_autopm_put_interface( pAutoPM->mpIntf );

         // Loop again
         complete( &pAutoPM->mThreadDoWork );
      }
      
      kfree( pURBListEntry );
   }   
   
   DBG( "traffic thread exiting\n" );
   pAutoPM->mpThread = NULL;
   return 0;
}      



/*===========================================================================
METHOD:
   QCUSBNetStartXmit (Public Method)

DESCRIPTION:
   Convert sk_buff to usb URB and queue for transmit

PARAMETERS
   pNet     [ I ] - Pointer to net device

RETURN VALUE:
   NETDEV_TX_OK on success
   NETDEV_TX_BUSY on error
===========================================================================*/
int QCUSBNetStartXmit( 
   struct sk_buff *     pSKB,
   struct net_device *  pNet )
{
   unsigned long URBListFlags;
  /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   unsigned long activeURBflags; 
  /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
   struct sQCUSBNet * pQCDev;
   sAutoPM * pAutoPM;
   sURBList * pURBListEntry, ** ppURBListEnd;
   void * pURBData;
   struct usbnet * pDev = netdev_priv( pNet );
   
   DBG( "\n" );
      	/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
 	 //printk( KERN_ERR "QCUSBNetStartXmit start\n");
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
 
   
   if (pDev == NULL || pDev->net == NULL)
   {
   /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
//    printk( KERN_ERR "QCUSBNetStartXmit error1\n");
  /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
  
      DBG( "failed to get usbnet device\n" );
      return NETDEV_TX_BUSY;
   }
   
   pQCDev = (sQCUSBNet *)pDev->data[0];
   if (pQCDev == NULL)
   {
   /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
	//printk( KERN_ERR "QCUSBNetStartXmit error2\n");
   /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
  
      DBG( "failed to get QMIDevice\n" );
      return NETDEV_TX_BUSY;
   }
   pAutoPM = &pQCDev->mAutoPM;
   
   if (QTestDownReason( pQCDev, DRIVER_SUSPENDED ) == true)
   {
	 /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
        //printk( KERN_ERR "QCUSBNetStartXmit error3\n");
	/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/  	

      // Should not happen
      DBG( "device is suspended\n" );
      dump_stack();
      return NETDEV_TX_BUSY;
   }
   
   // Convert the sk_buff into a URB

   // Allocate URBListEntry
   pURBListEntry = kmalloc( sizeof( sURBList ), GFP_ATOMIC );
   if (pURBListEntry == NULL)
   {
      DBG( "unable to allocate URBList memory\n" );
      return NETDEV_TX_BUSY;
   }
   pURBListEntry->mpNext = NULL;

   // Allocate URB
   pURBListEntry->mpURB = usb_alloc_urb( 0, GFP_ATOMIC );
   if (pURBListEntry->mpURB == NULL)
   {
/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
    // printk( KERN_ERR "QCUSBNetStartXmit error4\n");
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/

      DBG( "unable to allocate URB\n" );
      kfree( pURBListEntry );
      return NETDEV_TX_BUSY;
   }

   // Allocate URB transfer_buffer
   pURBData = kmalloc( pSKB->len, GFP_ATOMIC );
   if (pURBData == NULL)
   {
  /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
  //    printk( KERN_ERR "QCUSBNetStartXmit error5\n");
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/ 	

      DBG( "unable to allocate URB data\n" );
      usb_free_urb( pURBListEntry->mpURB );
      kfree( pURBListEntry );
      return NETDEV_TX_BUSY;
   }
   // Fill will SKB's data
   memcpy( pURBData, pSKB->data, pSKB->len );
   /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
  //printk( KERN_ERR "QCUSBNetStartXmit pSKB->len:%d\n",pSKB->len);
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
 

   usb_fill_bulk_urb( pURBListEntry->mpURB,
                      pQCDev->mpNetDev->udev,
                      pQCDev->mpNetDev->out,
                      pURBData,
                      pSKB->len,
                      QCUSBNetURBCallback,
                      pAutoPM );
   
   // Free the transfer buffer on last reference dropped
   pURBListEntry->mpURB->transfer_flags |= URB_FREE_BUFFER;
   
   // Aquire lock on URBList
   spin_lock_irqsave( &pAutoPM->mURBListLock, URBListFlags );
   
   // Add URB to end of list
   ppURBListEnd = &pAutoPM->mpURBList;
   while ((*ppURBListEnd) != NULL)
   {
      ppURBListEnd = &(*ppURBListEnd)->mpNext;
   }
   *ppURBListEnd = pURBListEntry;
 /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   gNet = pNet;
   TX_Queue_len++;
   if(TX_Queue_len > TX_QUEUE_MAX_LEN)
      netif_stop_queue (pNet);
 /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
   spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );



    spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );
   /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   if(pAutoPM->mpActiveURB == NULL)
   	  complete( &pAutoPM->mThreadDoWork );
   /* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/ 
	 spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );

  
   // Start transfer timer
   pNet->trans_start = jiffies;
   // Free SKB
   dev_kfree_skb_any( pSKB );
   /*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
  //printk( KERN_ERR "QCUSBNetStartXmit end\n");
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/


   return NETDEV_TX_OK;
}

/*===========================================================================
METHOD:
   QCUSBNetOpen (Public Method)

DESCRIPTION:
   Wrapper to usbnet_open, correctly handling autosuspend
   Start AutoPM thread

PARAMETERS
   pNet     [ I ] - Pointer to net device

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int QCUSBNetOpen( struct net_device * pNet )
{
   int status = 0;
   struct sQCUSBNet * pQCDev;
   struct usbnet * pDev = netdev_priv( pNet );
   
   if (pDev == NULL)
   {
      DBG( "failed to get usbnet device\n" );
      return -ENXIO;
   }
   
   pQCDev = (sQCUSBNet *)pDev->data[0];
   if (pQCDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return -ENXIO;
   }

   DBG( "\n" );

   // Start the AutoPM thread
   pQCDev->mAutoPM.mpIntf = pQCDev->mpIntf;
   pQCDev->mAutoPM.mbExit = false;
   pQCDev->mAutoPM.mpURBList = NULL;
/*[--WNCNJ: Awhyni Add for UDP buffer control@20111130*/
   TX_Queue_len = 0;
/* WNCNJ: Awhyni Add for UDP buffer control@20111130 --]*/
   pQCDev->mAutoPM.mpActiveURB = NULL;
   spin_lock_init( &pQCDev->mAutoPM.mURBListLock );
   spin_lock_init( &pQCDev->mAutoPM.mActiveURBLock );
   init_completion( &pQCDev->mAutoPM.mThreadDoWork );
   
   pQCDev->mAutoPM.mpThread = kthread_run( QCUSBNetAutoPMThread, 
                              &pQCDev->mAutoPM, 
                              "QCUSBNetAutoPMThread" );
   if (IS_ERR( pQCDev->mAutoPM.mpThread ))
   {
      DBG( "AutoPM thread creation error\n" );
      return PTR_ERR( pQCDev->mAutoPM.mpThread );
   }

   // Allow traffic
   QClearDownReason( pQCDev, NET_IFACE_STOPPED );

   // Pass to usbnet_open if defined
   if (pQCDev->mpUSBNetOpen != NULL)
   {
      status = pQCDev->mpUSBNetOpen( pNet );
   
      // If usbnet_open was successful enable Auto PM
      if (status == 0)
      {
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,33 ))
         usb_autopm_enable( pQCDev->mpIntf );
#else
         usb_autopm_put_interface( pQCDev->mpIntf );
#endif
      }
   }
   else
   {
      DBG( "no USBNetOpen defined\n" );
   }
   
   return status;
}

/*===========================================================================
METHOD:
   QCUSBNetStop (Public Method)

DESCRIPTION:
   Wrapper to usbnet_stop, correctly handling autosuspend
   Stop AutoPM thread

PARAMETERS
   pNet     [ I ] - Pointer to net device

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int QCUSBNetStop( struct net_device * pNet )
{
   struct sQCUSBNet * pQCDev;
   struct usbnet * pDev = netdev_priv( pNet );

   if (pDev == NULL || pDev->net == NULL)
   {
      DBG( "failed to get netdevice\n" );
      return -ENXIO;
   }
   
   pQCDev = (sQCUSBNet *)pDev->data[0];
   if (pQCDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return -ENXIO;
   }

   // Stop traffic
   QSetDownReason( pQCDev, NET_IFACE_STOPPED );

   // Tell traffic thread to exit
   pQCDev->mAutoPM.mbExit = true;
   complete( &pQCDev->mAutoPM.mThreadDoWork );
   
   // Wait for it to exit
   while( pQCDev->mAutoPM.mpThread != NULL )
   {
      msleep( 100 );
   }
   DBG( "thread stopped\n" );

   // Pass to usbnet_stop, if defined
   if (pQCDev->mpUSBNetStop != NULL)
   {
      return pQCDev->mpUSBNetStop( pNet );
   }
   else
   {
      return 0;
   }
}

static __be16 rmnet_ip_type_trans(struct sk_buff *skb, struct net_device *dev)
{
    __be16 protocol = 0;

    unsigned char module_addr[] = {0x02,0x50,0xf3,0x00,0x00,0x00};
    struct ethhdr *eth;

    skb->dev = dev;

    /* Determine L3 protocol */
    switch (skb->data[0] & 0xf0) {
        //case 0x40:
        //    protocol = htons(ETH_P_IP);
        //    break;
        case 0x60:
            //protocol = htons(ETH_P_IPV6);
            //if(skb->data[12]!=0x86 && skb->data[13]!=0xdd)
            //{
                skb_cow(skb, ETH_HLEN+2);

                eth = skb_push(skb, ETH_HLEN);
                
                eth->h_proto = htons(ETH_P_IPV6);
                
                memcpy(eth->h_source, module_addr, ETH_ALEN);
                
                memcpy(eth->h_dest, dev->dev_addr, ETH_ALEN); 
            //}
            break;
        default:
     //       DBG("[%s] rmnet_recv() L3 protocol decode error: 0x%02x", dev->name, skb->data[0] & 0xf0);
            /* skb will be dropped in upper layer for unknown protocol */
    }

    //DBG("dev->name = %s, protocol = %d, length = %d, data length = %d, mac length = %d, hdr length = %d\n", dev->name, protocol, skb->len, skb->data_len, skb->mac_len, skb->hdr_len );
    //DBG("dev->dev_addr = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x, dev->addr_len = %d\n", dev->dev_addr[0], dev->dev_addr[1], dev->dev_addr[2], dev->dev_addr[3], dev->dev_addr[4], dev->dev_addr[5], dev->addr_len );
    
   // DBG("skb->head = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", skb->head[0], skb->head[1], skb->head[2], skb->head[3], skb->head[4], skb->head[5], skb->head[6] );
   // DBG("skb->data = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", skb->data[0], skb->data[1], skb->data[2], skb->data[3], skb->data[4], skb->data[5], skb->data[6] );

   
    return protocol;
}

static int GobiNetDriverRxFixup(
    struct usbnet  *pDev,
    struct sk_buff *pSKB )
{
    pSKB->protocol =  rmnet_ip_type_trans(pSKB, pDev->net);
    return 1;
}

/*=========================================================================*/
// Struct driver_info
/*=========================================================================*/
static const struct driver_info QCNetInfo= 
{
   .description   = "QCUSBNet Ethernet Device",
   .flags         = FLAG_ETHER,
   .bind          = QCNetDriverBind,
   .unbind        = QCNetDriverUnbind,
   .rx_fixup	  = GobiNetDriverRxFixup,
   .tx_fixup      = GobiNetDriverTxFixup,
   .data          = 0,
};

/*=========================================================================*/
// Qualcomm Gobi 2000 VID/PIDs
/*=========================================================================*/
static const struct usb_device_id QCVIDPIDTable [] =
{
   // Dell Gobi 2000
   { 
      //USB_DEVICE( 0x413c, 0x8186 ),
      /* Eaddy 20100909 : Porting 9600 driver */
      USB_DEVICE( 0x5c6, 0x9000 ),
      .driver_info = (unsigned long)&QCNetInfo 
   },
   { 
      //USB_DEVICE( 0x413c, 0x8186 ),
      /* Eaddy 20100909 : Porting 9600 driver */
      USB_DEVICE( 0x5c6, 0x9001 ),
      .driver_info = (unsigned long)&QCNetInfo 
   },
   
   { 
   	  USB_DEVICE( 0x5c6, 0x9002 ),
      .driver_info = (unsigned long)&QCNetInfo 
   },
   { 
   	  USB_DEVICE( 0x5c6, 0x9011 ),
      .driver_info = (unsigned long)&QCNetInfo 
   },

   //Terminating entry
   {}
};

MODULE_DEVICE_TABLE( usb, QCVIDPIDTable );

/*===========================================================================
METHOD:
   QCUSBNetProbe (Public Method)

DESCRIPTION:
   Run usbnet_probe
   Setup QMI device

PARAMETERS
   pIntf        [ I ] - Pointer to interface
   pVIDPIDs     [ I ] - Pointer to VID/PID table

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int QCUSBNetProbe( 
   struct usb_interface *        pIntf, 
   const struct usb_device_id *  pVIDPIDs )
{
   int status;
   struct usbnet * pDev;
   sQCUSBNet * pQCDev;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION( 2,6,29 ))
   struct net_device_ops * pNetDevOps;
#endif

   status = usbnet_probe( pIntf, pVIDPIDs );
   if(status < 0 )
   {
      DBG( "usbnet_probe failed %d\n", status );
	  /*[start]WNCNJ-- kathy kong for solving the sys led blinking bug @ 2011.12.07*/
      return status;
	  /*[stop]WNCNJ-- kathy kong for solving the sys led blinking bug @ 2011.12.07*/

   }

#ifdef CONFIG_PM
   pIntf->needs_remote_wakeup = 1;
/*[start]WNCNJ-- kathy kong for QMI device wakeup enabled and enable autosuspend,
and setthe timer of autosuspend delay @2011/08/30*/
   #ifdef CONFIG_WL55
   #if (LINUX_VERSION_CODE >= KERNEL_VERSION( 2,6,19 ))
   struct usb_device *usb_dev = interface_to_usbdev(pIntf);
   device_init_wakeup(&pIntf->dev, 1);
   usb_autopm_enable(pIntf);
   usb_dev->autosuspend_delay = 150 * HZ;   
   usb_dev->autosuspend_disabled = 0;
   #endif
   #endif
/*[stop]WNCNJ-- kathy kong for QMI device wakeup enabled and enable autosuspend,
and setthe timer of autosuspend delay @2011/08/30*/
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,23 ))
   pDev = usb_get_intfdata( pIntf );
#else
   pDev = (struct usbnet *)pIntf->dev.platform_data;
#endif

   if (pDev == NULL || pDev->net == NULL)
   {
      DBG( "failed to get netdevice\n" );
      usbnet_disconnect( pIntf );
	  /*[start]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
	  status = -ENXIO;
      goto ERR_STATUS;
	  /*[start]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
   }

   pQCDev = kmalloc( sizeof( sQCUSBNet ), GFP_KERNEL );
   if (pQCDev == NULL)
   {
      DBG( "falied to allocate device buffers" );
      usbnet_disconnect( pIntf );
	  /*[start]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
	  status = -ENOMEM;
      goto ERR_STATUS;
	  /*[start]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
   }
   
   pDev->data[0] = (unsigned long)pQCDev;
   
   pQCDev->mpNetDev = pDev;

   // Overload PM related network functions
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,29 ))
#ifndef USBNETAUTOPM
   pQCDev->mpUSBNetOpen = pDev->net->open;
   pDev->net->open = QCUSBNetOpen;
   pQCDev->mpUSBNetStop = pDev->net->stop;
   pDev->net->stop = QCUSBNetStop;
   pDev->net->hard_start_xmit = QCUSBNetStartXmit;
   pDev->net->tx_timeout = QCUSBNetTXTimeout;
#endif
#else
   pNetDevOps = kmalloc( sizeof( struct net_device_ops ), GFP_KERNEL );
   if (pNetDevOps == NULL)
   {
      DBG( "falied to allocate net device ops" );
      usbnet_disconnect( pIntf );
	  /*[start]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
	  status = -ENOMEM;
      goto ERR_STATUS;
	  /*[start]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
   }
   memcpy( pNetDevOps, pDev->net->netdev_ops, sizeof( struct net_device_ops ) );

#ifndef USBNETAUTOPM   
   pQCDev->mpUSBNetOpen = pNetDevOps->ndo_open;
   pNetDevOps->ndo_open = QCUSBNetOpen;
   pQCDev->mpUSBNetStop = pNetDevOps->ndo_stop;
   pNetDevOps->ndo_stop = QCUSBNetStop;
   pNetDevOps->ndo_start_xmit = QCUSBNetStartXmit;
   pNetDevOps->ndo_tx_timeout = QCUSBNetTXTimeout;
#endif

   pDev->net->netdev_ops = pNetDevOps;
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,31 ))
   memset( &(pQCDev->mpNetDev->stats), 0, sizeof( struct net_device_stats ) );
#else
   memset( &(pQCDev->mpNetDev->net->stats), 0, sizeof( struct net_device_stats ) );
#endif

   pQCDev->mpIntf = pIntf;
   memset( &(pQCDev->mMEID), '0', 14 );
   
   DBG( "Mac Address:\n" );
   PrintHex( &pQCDev->mpNetDev->net->dev_addr[0], 6 );

   pQCDev->mbQMIValid = false;
   memset( &pQCDev->mQMIDev, 0, sizeof( sQMIDev ) );
   pQCDev->mQMIDev.mbCdevIsInitialized = false;

   pQCDev->mQMIDev.mpDevClass = gpClass;
   
   init_completion( &pQCDev->mAutoPM.mThreadDoWork );
   spin_lock_init( &pQCDev->mQMIDev.mClientMemLock );

   // Default to device down
   pQCDev->mDownReason = 0;
   QSetDownReason( pQCDev, NO_NDIS_CONNECTION );

   #ifndef USBNETAUTOPM
   QSetDownReason( pQCDev, NET_IFACE_STOPPED );
   #endif
   // Register QMI
   status = RegisterQMIDevice( pQCDev );
   if (status != 0)
   {    
      // usbnet_disconnect() will call QCUSBNetUnbind() which will call
      // DeregisterQMIDevice() to clean up any partially created QMI device
      usbnet_disconnect( pIntf );
      /*[start]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
      goto ERR_STATUS;
	  /*[stop]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
   } 
   // Success
   return 0;

/*[start]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
ERR_STATUS:
       #ifdef CONFIG_LEDS_PCA9555
       #if (defined CONFIG_WL55_LAB2) && (defined CONFIG_LAB2_LED_SPEC1)
          ath_led_work(LED_WWAN_ERR,PCA9555_ON);
	   #endif 
	   #endif 
	   return status;
/*[stop]WNCNJ-- kathy kong for add the control of PCI-e NG @ 2011.10.13*/
}

static struct usb_driver QCUSBNet =
{
   .name       = "QCUSBNet2kDell",
   .id_table   = QCVIDPIDTable,
   .probe      = QCUSBNetProbe,
   .disconnect = usbnet_disconnect,
   .suspend    = QCSuspend,
   .resume     = QCResume,
/*[start]WNCNJ-- kathy for disable qcusbnet reset @2011.09.06*/
   .reset_resume = QCResume,
/*[stop]WNCNJ-- kathy for disable qcusbnet reset @2011.09.06*/
   .supports_autosuspend = true,
};

/*===========================================================================
METHOD:
   QCUSBNetModInit (Public Method)

DESCRIPTION:
   Initialize module
   Create device class
   Register out usb_driver struct

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
static int __init QCUSBNetModInit( void )
{
   gpClass = class_create( THIS_MODULE, "QCQMIDell" );
   if (IS_ERR( gpClass ) == true)
   {
      DBG( "error at class_create %ld\n",
           PTR_ERR( gpClass ) );
      return -ENOMEM;
   }

   // This will be shown whenever driver is loaded
   printk( KERN_INFO "%s: %s\n", DRIVER_DESC, DRIVER_VERSION );

   return usb_register( &QCUSBNet );
}
module_init( QCUSBNetModInit );

/*===========================================================================
METHOD:
   QCUSBNetModExit (Public Method)

DESCRIPTION:
   Deregister module
   Destroy device class

RETURN VALUE:
   void
===========================================================================*/
static void __exit QCUSBNetModExit( void )
{
   usb_deregister( &QCUSBNet );

   class_destroy( gpClass );
}
module_exit( QCUSBNetModExit );

#ifdef bool
#undef bool
#endif

MODULE_VERSION( DRIVER_VERSION );
MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
MODULE_LICENSE( "Dual BSD/GPL" );

module_param( debug, bool, S_IRUGO | S_IWUSR );
MODULE_PARM_DESC( debug, "Debuging enabled or not" );

module_param( safeEnumDelay, bool, S_IRUGO | S_IWUSR );
MODULE_PARM_DESC( safeEnumDelay, "Delay enumeration to allow firmware to be ready (needed on firmware < 3580)" );

module_param( interruptible, bool, S_IRUGO | S_IWUSR );
MODULE_PARM_DESC( interruptible, "Listen for and return on user interrupt" );
