/*===========================================================================
FILE:
   GobiUSBNet.c

DESCRIPTION:
   Qualcomm USB Network device for Gobi 3000
   
FUNCTIONS:
   GatherEndpoints
   GobiSuspend
   GobiResume
   GobiNetDriverBind
   GobiNetDriverUnbind
   GobiUSBNetURBCallback
   GobiUSBNetTXTimeout
   GobiUSBNetAutoPMThread
   GobiUSBNetStartXmit
   GobiUSBNetOpen
   GobiUSBNetStop
   GobiUSBNetProbe
   GobiUSBNetModInit
   GobiUSBNetModExit

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
#include <linux/module.h>
#include "Structs.h"
#include "QMIDevice.h"
#include "QMI.h"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

// Version Information
#define DRIVER_VERSION "1.0.26.1"
#define DRIVER_AUTHOR "Qualcomm Innovation Center"
#define DRIVER_DESC "GobiNet"

// Debug flag
int debug=0;

// Allow user interrupts
int interruptible = 1;

// Number of IP packets which may be queued up for transmit
int txQueueLength = 300;

// Mac addr param
char *mac_addr = NULL;

// Class should be created during module init, so needs to be global
static struct class * gpClass;

static void str2mac(const char *macaddr, unsigned char mac[6])
{
	unsigned int m[6];
	if (sscanf(macaddr, "%02x:%02x:%02x:%02x:%02x:%02x",
		   &m[0], &m[1], &m[2], &m[3], &m[4], &m[5]) != 6)
		printk(1, "Failed to parse mac address '%s'", macaddr);
	mac[0] = m[0];
	mac[1] = m[1];
	mac[2] = m[2];
	mac[3] = m[3];
	mac[4] = m[4];
	mac[5] = m[5];
}

/*===========================================================================
METHOD:
   GatherEndpoints (Public Method)

DESCRIPTION:
   Enumerate endpoints

PARAMETERS
   pIntf          [ I ] - Pointer to usb interface

RETURN VALUE:
   sEndpoints structure
              NULL for failure
===========================================================================*/
sEndpoints * GatherEndpoints( struct usb_interface * pIntf )
{
   int numEndpoints;
   int endpointIndex;
   sEndpoints * pOut;
   struct usb_host_endpoint * pEndpoint = NULL;
   
   pOut = kzalloc( sizeof( sEndpoints ), GFP_ATOMIC );
   if (pOut == NULL)
   {
      DBG( "unable to allocate memory\n" );
      return NULL;
   }

   pOut->mIntfNum = pIntf->cur_altsetting->desc.bInterfaceNumber;
   
   // Scan endpoints
   numEndpoints = pIntf->cur_altsetting->desc.bNumEndpoints;
   for (endpointIndex = 0; endpointIndex < numEndpoints; endpointIndex++)
   {
      pEndpoint = pIntf->cur_altsetting->endpoint + endpointIndex;
      if (pEndpoint == NULL)
      {
         DBG( "invalid endpoint %u\n", endpointIndex );
         kfree( pOut );
         return NULL;
      }
      
      if (usb_endpoint_dir_in( &pEndpoint->desc ) == true
      &&  usb_endpoint_xfer_int( &pEndpoint->desc ) == true)
      {
         pOut->mIntInEndp = pEndpoint->desc.bEndpointAddress;
      }
      else if (usb_endpoint_dir_in( &pEndpoint->desc ) == true
      &&  usb_endpoint_xfer_int( &pEndpoint->desc ) == false)
      {
         pOut->mBlkInEndp = pEndpoint->desc.bEndpointAddress;
      }
      else if (usb_endpoint_dir_in( &pEndpoint->desc ) == false
      &&  usb_endpoint_xfer_int( &pEndpoint->desc ) == false)
      {
         pOut->mBlkOutEndp = pEndpoint->desc.bEndpointAddress;
      }
   }

   if (pOut->mIntInEndp == 0
   ||  pOut->mBlkInEndp == 0
   ||  pOut->mBlkOutEndp == 0)
   {
      DBG( "One or more endpoints missing\n" );
      kfree( pOut );
      return NULL;
   }

   DBG( "intf %u\n", pOut->mIntfNum );
   DBG( "   int in  0x%02x\n", pOut->mIntInEndp );
   DBG( "   blk in  0x%02x\n", pOut->mBlkInEndp );
   DBG( "   blk out 0x%02x\n", pOut->mBlkOutEndp );

   return pOut;
}

unsigned char module_addr[] = {0x02,0x50,0xf3,0x00,0x00,0x00};
unsigned char br_source_addr[] = {0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char broadcast_addr[] = {0xff,0xff,0xff,0xff,0xff,0xff};

int bridge_flag = 0;

static __be16 record_packet_tx(struct sk_buff *skb, struct net_device *dev)
{
    //printk( KERN_INFO "[%s] record_packet_tx", dev->name);

    // compare with device MAC in usb0 & source MAC in packet 
    if(skb->data[6]==dev->dev_addr[0] && skb->data[7]==dev->dev_addr[1] && skb->data[8]==dev->dev_addr[2])          
    {
        //printk( KERN_INFO "[%s] record_packet_tx from host", dev->name);
    }
    else
    {
        if(strcmp(dev->name,"usb0")==0 && skb->data[35]==0x44 && skb->data[37]==0x43)
        {
            bridge_flag = 1;

            //strncpy(br_source_addr, &(skb->data[6]), ETH_ALEN);
            br_source_addr[0]=skb->data[6];
            br_source_addr[1]=skb->data[7];
            br_source_addr[2]=skb->data[8];
            br_source_addr[3]=skb->data[9];
            br_source_addr[4]=skb->data[10];
            br_source_addr[5]=skb->data[11];

            printk( KERN_INFO "[%s] record_packet_tx from PC MAC: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", dev->name, br_source_addr[0], br_source_addr[1], br_source_addr[2], br_source_addr[3], br_source_addr[4], br_source_addr[5] );
        }
    }    
    return 1;
}

static __be16 rmnet_ip_type_trans(struct sk_buff *skb, struct net_device *dev)
{
    //printk( KERN_INFO "[%s] rmnet_ip_type_trans", dev->name);

    __be16 protocol = 0;
    
    unsigned char *dest_addr;
    struct ethhdr *eth;

    skb->dev = dev;

    if(bridge_flag == 1 && strcmp(dev->name,"usb0")==0)
        dest_addr = br_source_addr;
    else
        dest_addr = dev->dev_addr;   

    /* Determine L3 protocol */
    switch (skb->data[0] & 0xf0) 
    {
        case 0x40:
            skb_cow(skb, ETH_HLEN+2);           
            eth = skb_push(skb, ETH_HLEN);           
            eth->h_proto = htons(ETH_P_IP);            
            memcpy(eth->h_source, module_addr, ETH_ALEN);            
			if(skb->data[21]==0x43 && skb->data[23]==0x44 && skb->data[38]==0x80)  // for DHCP requset & ack packet with broadcast flag, write dst MAC to broadcast_addr
				memcpy(eth->h_dest, broadcast_addr, ETH_ALEN); 
			else
            memcpy(eth->h_dest, dest_addr, ETH_ALEN); 

            break;
        case 0x60:
            skb_cow(skb, ETH_HLEN+2);
            eth = skb_push(skb, ETH_HLEN);                           
            eth->h_proto = htons(ETH_P_IPV6);                
            memcpy(eth->h_source, module_addr, ETH_ALEN);                
            memcpy(eth->h_dest, dest_addr, ETH_ALEN); 
                
            break;
        default:
            //DBG( "[%s] rmnet_recv() L3 protocol ether header: 0x%02x, ip header: 0x%02x", dev->name, skb->data[0] & 0xf0, skb->data[14] & 0xf0);
            //if(skb->data[6]==0x02 && skb->data[7]==0x50 && skb->data[8]==0xf3)
            //{
                //DBG( "[%s] rmnet_recv() L3 protocol normal ether header: 0x%02x, ip header: 0x%02x", dev->name, skb->data[0] & 0xf0, skb->data[14] & 0xf0);
                
                //DBG( "rx src MAC = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", skb->data[6], skb->data[7], skb->data[8], skb->data[9], skb->data[10], skb->data[11] );

            //    return 0;
            //}
            //else
            //{                        
                // for packet with ether header and rewrite it
                switch (skb->data[14] & 0xf0) 
                {
                    case 0x40:                          
                        eth = (struct ethhdr *)(skb->data);
                        eth->h_proto = htons(ETH_P_IP);            
                        memcpy(eth->h_source, module_addr, ETH_ALEN);                
                        if(skb->data[35]==0x43 && skb->data[37]==0x44 && skb->data[52]==0x80)  // for DHCP requset & ack packet with broadcast flag, write dst MAC to broadcast_addr
                            memcpy(eth->h_dest, broadcast_addr, ETH_ALEN); 
                        else
                        memcpy(eth->h_dest, dest_addr, ETH_ALEN);

                        break;
                    case 0x60:                  
                        eth = (struct ethhdr *)(skb->data);
                        eth->h_proto = htons(ETH_P_IPV6);            
                        memcpy(eth->h_source, module_addr, ETH_ALEN);                
                        memcpy(eth->h_dest, dest_addr, ETH_ALEN);
                            
                        break;
                    default:
                        DBG( "[%s] rmnet_recv() L3 protocol remove ether header: 0x%02x, ip header: 0x%02x", dev->name, skb->data[0] & 0xf0, skb->data[14] & 0xf0);
                }  
            //}
    }

    //DBG( "rx dev->name = %s, protocol = %d, length = %d, data length = %d, mac length = %d, hdr length = %d\n", dev->name, protocol, skb->len, skb->data_len, skb->mac_len, skb->hdr_len );
    //DBG( "rx dev->dev_addr = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x, dev->addr_len = %d\n", dev->dev_addr[0], dev->dev_addr[1], dev->dev_addr[2], dev->dev_addr[3], dev->dev_addr[4], dev->dev_addr[5], dev->addr_len );
    
    //DBG( "rx skb->head = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", skb->head[0], skb->head[1], skb->head[2], skb->head[3], skb->head[4], skb->head[5] );
    //DBG( "rx ether header = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", skb->data[0], skb->data[1], skb->data[2], skb->data[3], skb->data[4], skb->data[5] );
    //DBG( "rx ip header = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", skb->data[14], skb->data[15], skb->data[16], skb->data[17], skb->data[18], skb->data[19] );

    return protocol;
}

struct sk_buff *GobiNetDriverTxFixup(
    struct usbnet  *pDev,
    struct sk_buff *pSKB,
    gfp_t flags)
{

    record_packet_tx(pSKB, pDev->net);
    return pSKB;
}

static int GobiNetDriverRxFixup(
    struct usbnet  *pDev,
    struct sk_buff *pSKB )
{
    pSKB->protocol = rmnet_ip_type_trans(pSKB, pDev->net);
    return 1;
}

/*===========================================================================
METHOD:
   GobiSuspend (Public Method)

DESCRIPTION:
   Stops QMI traffic while device is suspended

PARAMETERS
   pIntf          [ I ] - Pointer to interface
   powerEvent     [ I ] - Power management event

RETURN VALUE:
   int - 0 for success
         negative errno for failure
===========================================================================*/
#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,23 ))
int GobiSuspend( 
   struct usb_interface *     pIntf,
   pm_message_t               powerEvent )
{
   struct usbnet * pDev;
   sGobiUSBNet * pGobiDev;
   
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
   
   pGobiDev = (sGobiUSBNet *)pDev->data[0];
   if (pGobiDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return -ENXIO;
   }

   // Is this autosuspend or system suspend?
   //    do we allow remote wakeup?
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,33 ))
   if (pDev->udev->auto_pm == 0)
#else
   if ((powerEvent.event & PM_EVENT_AUTO) == 0)
#endif
   {
      DBG( "device suspended to power level %d\n", 
           powerEvent.event );
      GobiSetDownReason( pGobiDev, DRIVER_SUSPENDED );
   }
   else
   {
      DBG( "device autosuspend\n" );
   }
     
   if (powerEvent.event & PM_EVENT_SUSPEND)
   {
      // Stop QMI read callbacks
      KillRead( pGobiDev );
#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,21 ))
      pDev->udev->reset_resume = 0;
#endif
      // Store power state to avoid duplicate resumes
      pIntf->dev.power.power_state.event = powerEvent.event;
   }
   else
   {
      // Other power modes cause QMI connection to be lost
#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,21 ))
      pDev->udev->reset_resume = 1;
#endif
   }
   
   // Run usbnet's suspend function
   return usbnet_suspend( pIntf, powerEvent );
}
#endif

   
/*===========================================================================
METHOD:
   GobiResume (Public Method)

DESCRIPTION:
   Resume QMI traffic or recreate QMI device

PARAMETERS
   pIntf          [ I ] - Pointer to interface

RETURN VALUE:
   int - 0 for success
         negative errno for failure
===========================================================================*/
int GobiResume( struct usb_interface * pIntf )
{
   struct usbnet * pDev;
   sGobiUSBNet * pGobiDev;
   int nRet;
   int oldPowerState;
   
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
   
   pGobiDev = (sGobiUSBNet *)pDev->data[0];
   if (pGobiDev == NULL)
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
      GobiClearDownReason( pGobiDev, DRIVER_SUSPENDED );
   
      nRet = usbnet_resume( pIntf );
      if (nRet != 0)
      {
         DBG( "usbnet_resume error %d\n", nRet );
         return nRet;
      }

      // Restart QMI read callbacks
      nRet = StartRead( pGobiDev );
      if (nRet != 0)
      {
         DBG( "StartRead error %d\n", nRet );
         return nRet;
      }

      // Kick Auto PM thread to process any queued URBs
      complete( &pGobiDev->mAutoPM.mThreadDoWork );
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
   GobiNetDriverBind (Public Method)

DESCRIPTION:
   Setup in and out pipes

PARAMETERS
   pDev           [ I ] - Pointer to usbnet device
   pIntf          [ I ] - Pointer to interface

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
static int GobiNetDriverBind( 
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

   // Verify correct interface (0 or 5)
   //if ( (pIntf->cur_altsetting->desc.bInterfaceNumber != 0)
   //&&   (pIntf->cur_altsetting->desc.bInterfaceNumber != 5) )
   if( !( (1 << pIntf->cur_altsetting->desc.bInterfaceNumber) & (int)pDev->driver_info->data ) )
   {
      DBG( "invalid interface %d\n", 
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
   GobiNetDriverUnbind (Public Method)

DESCRIPTION:
   Deregisters QMI device (Registration happened in the probe function)

PARAMETERS
   pDev           [ I ] - Pointer to usbnet device
   pIntfUnused    [ I ] - Pointer to interface

RETURN VALUE:
   None
===========================================================================*/
static void GobiNetDriverUnbind( 
   struct usbnet *         pDev, 
   struct usb_interface *  pIntf)
{
   sGobiUSBNet * pGobiDev = (sGobiUSBNet *)pDev->data[0];

   // Should already be down, but just in case...
   netif_carrier_off( pDev->net );

   DeregisterQMIDevice( pGobiDev );
   
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

   kfree( pGobiDev->mpEndpoints );
   kfree( pGobiDev );
   pGobiDev = NULL;
}

/*===========================================================================
METHOD:
   GobiUSBNetURBCallback (Public Method)

DESCRIPTION:
   Write is complete, cleanup and signal that we're ready for next packet

PARAMETERS
   pURB     [ I ] - Pointer to sAutoPM struct

RETURN VALUE:
   None
===========================================================================*/
void GobiUSBNetURBCallback( struct urb * pURB )
{
   unsigned long activeURBflags;
   sAutoPM * pAutoPM = (sAutoPM *)pURB->context;
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
   }

   // Remove activeURB (memory to be freed later)
   spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );

   // EAGAIN used to signify callback is done
   pAutoPM->mpActiveURB = ERR_PTR( -EAGAIN );

   spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );

   complete( &pAutoPM->mThreadDoWork );
   
   usb_free_urb( pURB );
}

/*===========================================================================
METHOD:
   GobiUSBNetTXTimeout (Public Method)

DESCRIPTION:
   Timeout declared by the net driver.  Stop all transfers

PARAMETERS
   pNet     [ I ] - Pointer to net device

RETURN VALUE:
   None
===========================================================================*/
void GobiUSBNetTXTimeout( struct net_device * pNet )
{
   struct sGobiUSBNet * pGobiDev;
   sAutoPM * pAutoPM;
   sURBList * pURBListEntry;
   unsigned long activeURBflags, URBListFlags;
   struct usbnet * pDev = netdev_priv( pNet );
   struct urb * pURB;

   if (pDev == NULL || pDev->net == NULL)
   {
      DBG( "failed to get usbnet device\n" );
      return;
   }
   
   pGobiDev = (sGobiUSBNet *)pDev->data[0];
   if (pGobiDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return;
   }
   pAutoPM = &pGobiDev->mAutoPM;

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
      atomic_dec( &pAutoPM->mURBListLen );
      usb_free_urb( pURBListEntry->mpURB );
      kfree( pURBListEntry );
      pURBListEntry = pAutoPM->mpURBList;
   }

   spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );

   complete( &pAutoPM->mThreadDoWork );

   return;
}

/*===========================================================================
METHOD:
   GobiUSBNetAutoPMThread (Public Method)

DESCRIPTION:
   Handle device Auto PM state asynchronously
   Handle network packet transmission asynchronously

PARAMETERS
   pData     [ I ] - Pointer to sAutoPM struct

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
static int GobiUSBNetAutoPMThread( void * pData )
{
   unsigned long activeURBflags, URBListFlags;
   sURBList * pURBListEntry;
   int status;
   struct usb_device * pUdev;
   sAutoPM * pAutoPM = (sAutoPM *)pData;
   struct urb * pURB;

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
            atomic_dec( &pAutoPM->mURBListLen );
            usb_free_urb( pURBListEntry->mpURB );
            kfree( pURBListEntry );
            pURBListEntry = pAutoPM->mpURBList;
         }

         spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );

         break;
      }
      
      // Is our URB active?
      spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );

      // EAGAIN used to signify callback is done
      if (IS_ERR( pAutoPM->mpActiveURB ) 
      &&  PTR_ERR( pAutoPM->mpActiveURB ) == -EAGAIN )
      {
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
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );
         continue;
      }
      
      // Is there a URB waiting to be submitted?
      spin_lock_irqsave( &pAutoPM->mURBListLock, URBListFlags );
      if (pAutoPM->mpURBList == NULL)
      {
         // No more URBs to submit, go back to sleep
         spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );
         continue;
      }

      // Pop an element
      pURBListEntry = pAutoPM->mpURBList;
      pAutoPM->mpURBList = pAutoPM->mpURBList->mpNext;
      atomic_dec( &pAutoPM->mURBListLen );
      spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );

      // Set ActiveURB
      pAutoPM->mpActiveURB = pURBListEntry->mpURB;
      spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );

      // Tell autopm core we need device woken up
      status = usb_autopm_get_interface( pAutoPM->mpIntf );
#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,23 ))
      if (status < 0)
      {
         DBG( "unable to autoresume interface: %d\n", status );

         // likely caused by device going from autosuspend -> full suspend
         if (status == -EPERM)
         {
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,33 ))
            pUdev->auto_pm = 0;
#endif
            GobiSuspend( pAutoPM->mpIntf, PMSG_SUSPEND );
         }

         // Add pURBListEntry back onto pAutoPM->mpURBList
         spin_lock_irqsave( &pAutoPM->mURBListLock, URBListFlags );
         pURBListEntry->mpNext = pAutoPM->mpURBList;
         pAutoPM->mpURBList = pURBListEntry;
         atomic_inc( &pAutoPM->mURBListLen );
         spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );
         
         spin_lock_irqsave( &pAutoPM->mActiveURBLock, activeURBflags );
         pAutoPM->mpActiveURB = NULL;
         spin_unlock_irqrestore( &pAutoPM->mActiveURBLock, activeURBflags );
         
         // Go back to sleep
         continue;
      }
#endif
      // Submit URB
      status = usb_submit_urb( pAutoPM->mpActiveURB, GFP_KERNEL );
      if (status < 0)
      {
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
   GobiUSBNetStartXmit (Public Method)

DESCRIPTION:
   Convert sk_buff to usb URB and queue for transmit

PARAMETERS
   pNet     [ I ] - Pointer to net device

RETURN VALUE:
   NETDEV_TX_OK on success
   NETDEV_TX_BUSY on error
===========================================================================*/
int GobiUSBNetStartXmit( 
   struct sk_buff *     pSKB,
   struct net_device *  pNet )
{
   unsigned long URBListFlags;
   struct sGobiUSBNet * pGobiDev;
   sAutoPM * pAutoPM;
   sURBList * pURBListEntry, ** ppURBListEnd;
   void * pURBData;
   struct usbnet * pDev = netdev_priv( pNet );
   struct driver_info *info = pDev->driver_info;
   
   if (pDev == NULL || pDev->net == NULL)
   {
      DBG( "failed to get usbnet device\n" );
      return NETDEV_TX_BUSY;
   }
   
   pGobiDev = (sGobiUSBNet *)pDev->data[0];
   if (pGobiDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return NETDEV_TX_BUSY;
   }
   pAutoPM = &pGobiDev->mAutoPM;
   
   if (GobiTestDownReason( pGobiDev, DRIVER_SUSPENDED ) == true)
   {
      // Should not happen
      DBG( "device is suspended\n" );
      dump_stack();
      return NETDEV_TX_BUSY;
   }
   
   // Convert the sk_buff into a URB

   // Check if buffer is full
   if (atomic_read( &pAutoPM->mURBListLen ) >= txQueueLength)
   {
      DBG( "not scheduling request, buffer is full\n" );
      return NETDEV_TX_BUSY;
   }

   // [[[ TimKang 20120716, add Raw IP mode
   if (info->tx_fixup)
   {
      pSKB = info->tx_fixup( pDev, pSKB, GFP_ATOMIC);
      if (pSKB == NULL)
      {
         DBG( "unable to tx_fixup skb\n" );
         return NETDEV_TX_BUSY;
      }
   }
   // ]]]

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
      DBG( "unable to allocate URB\n" );
      kfree( pURBListEntry );
      return NETDEV_TX_BUSY;
   }

   // Allocate URB transfer_buffer
   pURBData = kmalloc( pSKB->len, GFP_ATOMIC );
   if (pURBData == NULL)
   {
      DBG( "unable to allocate URB data\n" );
      usb_free_urb( pURBListEntry->mpURB );
      kfree( pURBListEntry );
      return NETDEV_TX_BUSY;
   }
   // Fill will SKB's data
   memcpy( pURBData, pSKB->data, pSKB->len );

   usb_fill_bulk_urb( pURBListEntry->mpURB,
                      pGobiDev->mpNetDev->udev,
                      pGobiDev->mpNetDev->out,
                      pURBData,
                      pSKB->len,
                      GobiUSBNetURBCallback,
                      pAutoPM );

   // [[[ TimKang 20120202, add for packet statistics
   {
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,31 ))
      struct net_device_stats * pStats = &(pGobiDev->mpNetDev->stats);
#else
      struct net_device_stats * pStats = &(pGobiDev->mpNetDev->net->stats);
#endif

      pStats->tx_packets++;
      pStats->tx_bytes += pSKB->len;
   }
   // ]]]
   // Free the transfer buffer on last reference dropped
#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,24 ))
   pURBListEntry->mpURB->transfer_flags |= URB_FREE_BUFFER;
#endif
   // Aquire lock on URBList
   spin_lock_irqsave( &pAutoPM->mURBListLock, URBListFlags );
   
   // Add URB to end of list
   ppURBListEnd = &pAutoPM->mpURBList;
   while ((*ppURBListEnd) != NULL)
   {
      ppURBListEnd = &(*ppURBListEnd)->mpNext;
   }
   *ppURBListEnd = pURBListEntry;
   atomic_inc( &pAutoPM->mURBListLen );

   spin_unlock_irqrestore( &pAutoPM->mURBListLock, URBListFlags );

   complete( &pAutoPM->mThreadDoWork );

   // Start transfer timer
   pNet->trans_start = jiffies;
   // Free SKB
   dev_kfree_skb_any( pSKB );

   return NETDEV_TX_OK;
}

/*===========================================================================
METHOD:
   GobiUSBNetOpen (Public Method)

DESCRIPTION:
   Wrapper to usbnet_open, correctly handling autosuspend
   Start AutoPM thread

PARAMETERS
   pNet     [ I ] - Pointer to net device

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int GobiUSBNetOpen( struct net_device * pNet )
{
   int status = 0;
   struct sGobiUSBNet * pGobiDev;
   struct usbnet * pDev = netdev_priv( pNet );
   
   if (pDev == NULL)
   {
      DBG( "failed to get usbnet device\n" );
      return -ENXIO;
   }
   
   pGobiDev = (sGobiUSBNet *)pDev->data[0];
   if (pGobiDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return -ENXIO;
   }

   DBG( "\n" );

   // Start the AutoPM thread
   pGobiDev->mAutoPM.mpIntf = pGobiDev->mpIntf;
   pGobiDev->mAutoPM.mbExit = false;
   pGobiDev->mAutoPM.mpURBList = NULL;
   pGobiDev->mAutoPM.mpActiveURB = NULL;
   spin_lock_init( &pGobiDev->mAutoPM.mURBListLock );
   spin_lock_init( &pGobiDev->mAutoPM.mActiveURBLock );
   atomic_set( &pGobiDev->mAutoPM.mURBListLen, 0 );
   init_completion( &pGobiDev->mAutoPM.mThreadDoWork );
   
   pGobiDev->mAutoPM.mpThread = kthread_run( GobiUSBNetAutoPMThread, 
                                               &pGobiDev->mAutoPM, 
                                               "GobiUSBNetAutoPMThread" );
   if (IS_ERR( pGobiDev->mAutoPM.mpThread ))
   {
      DBG( "AutoPM thread creation error\n" );
      return PTR_ERR( pGobiDev->mAutoPM.mpThread );
   }

   // Allow traffic
   GobiClearDownReason( pGobiDev, NET_IFACE_STOPPED );

   // Pass to usbnet_open if defined
   if (pGobiDev->mpUSBNetOpen != NULL)
   {
      status = pGobiDev->mpUSBNetOpen( pNet );
   
      // If usbnet_open was successful enable Auto PM
      if (status == 0)
      {
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,33 ))
         usb_autopm_enable( pGobiDev->mpIntf );
#else
         usb_autopm_put_interface( pGobiDev->mpIntf );
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
   GobiUSBNetStop (Public Method)

DESCRIPTION:
   Wrapper to usbnet_stop, correctly handling autosuspend
   Stop AutoPM thread

PARAMETERS
   pNet     [ I ] - Pointer to net device

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int GobiUSBNetStop( struct net_device * pNet )
{
   struct sGobiUSBNet * pGobiDev;
   struct usbnet * pDev = netdev_priv( pNet );

   if (pDev == NULL || pDev->net == NULL)
   {
      DBG( "failed to get netdevice\n" );
      return -ENXIO;
   }
   
   pGobiDev = (sGobiUSBNet *)pDev->data[0];
   if (pGobiDev == NULL)
   {
      DBG( "failed to get QMIDevice\n" );
      return -ENXIO;
   }

   // Stop traffic
   GobiSetDownReason( pGobiDev, NET_IFACE_STOPPED );

   // Tell traffic thread to exit
   pGobiDev->mAutoPM.mbExit = true;
   complete( &pGobiDev->mAutoPM.mThreadDoWork );
   
   // Wait for it to exit
   while( pGobiDev->mAutoPM.mpThread != NULL )
   {
      msleep( 100 );
   }
   DBG( "thread stopped\n" );

   // Pass to usbnet_stop, if defined
   if (pGobiDev->mpUSBNetStop != NULL)
   {
      return pGobiDev->mpUSBNetStop( pNet );
   }
   else
   {
      return 0;
   }
}

/*=========================================================================*/
// Struct driver_info
/*=========================================================================*/
static const struct driver_info GobiNetInfo = 
{
   .description   = "GobiNet Ethernet Device",
   .flags         = FLAG_ETHER,
   .bind          = GobiNetDriverBind,
   .unbind        = GobiNetDriverUnbind,
   .rx_fixup      = GobiNetDriverRxFixup,
   .tx_fixup      = GobiNetDriverTxFixup,
   .data          = 0,
};

static const struct driver_info mdm9200 = 
{
   .description   = "GobiNet Ethernet Device",
   .flags         = FLAG_ETHER,
   .bind          = GobiNetDriverBind,
   .unbind        = GobiNetDriverUnbind,
   .rx_fixup      = GobiNetDriverRxFixup,
   .tx_fixup      = GobiNetDriverTxFixup,
   .data          = 0x18,
};

static const struct driver_info mdm9x15 = 
{
   .description   = "GobiNet Ethernet Device",
   .flags         = FLAG_ETHER,
   .bind          = GobiNetDriverBind,
   .unbind        = GobiNetDriverUnbind,
   .rx_fixup      = GobiNetDriverRxFixup,
   .tx_fixup      = GobiNetDriverTxFixup,
   .data          = 0x10,
};

/*=========================================================================*/
// Qualcomm Gobi 3000 VID/PIDs
/*=========================================================================*/
static const struct usb_device_id GobiVIDPIDTable [] =
{
   // Gobi 3000
   { 
      USB_DEVICE( 0x05c6, 0x920d ),
      .driver_info = (unsigned long)&GobiNetInfo 
   },
   // 9200
   { 
      USB_DEVICE( 0x05c6, 0x9011 ),
      .driver_info = (unsigned long)&mdm9200 
   },
   // 9x15 (Lab stage)
   { 
      USB_DEVICE( 0x05c6, 0x9025 ),
      .driver_info = (unsigned long)&mdm9x15 
   },
   // 9x15 (EPR stage)
   {
      USB_DEVICE( 0x05c6, 0x9046 ),
      .driver_info = (unsigned long)&mdm9x15
   },
// add private Vid/Pid arthur begin 20140208
   {
   	  USB_DEVICE( 0x1435, 0xd111 ),//DM11-1
	  .driver_info = (unsigned long)&mdm9x15
   },
   { 
      USB_DEVICE( 0x1435, 0xd112 ),//DM11-2
	  .driver_info = (unsigned long)&mdm9x15
   },
   { 
      USB_DEVICE( 0x1435, 0xd116 ),//DM11-6
	  .driver_info = (unsigned long)&mdm9x15
   },
// add private Vid/Pid arthur end 20140208
	  
   //Terminating entry
   { }
};

MODULE_DEVICE_TABLE( usb, GobiVIDPIDTable );

/*===========================================================================
METHOD:
   GobiUSBNetProbe (Public Method)

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
int GobiUSBNetProbe( 
   struct usb_interface *        pIntf, 
   const struct usb_device_id *  pVIDPIDs )
{
   int status;
   struct usbnet * pDev;
   sGobiUSBNet * pGobiDev;
   sEndpoints * pEndpoints;
   int pipe;
   
   unsigned char my_mac_addr[ETH_ALEN];
   struct sockaddr addr;
   
#if (LINUX_VERSION_CODE >= KERNEL_VERSION( 2,6,29 ))
   struct net_device_ops * pNetDevOps;
#endif

   pEndpoints = GatherEndpoints( pIntf );
   if (pEndpoints == NULL)
   {
      return -ENODEV;
   }      

   status = usbnet_probe( pIntf, pVIDPIDs );
   if (status < 0)
   {
      DBG( "usbnet_probe failed %d\n", status );
      return status;
   }

#if (LINUX_VERSION_CODE >= KERNEL_VERSION( 2,6,19 ))
   pIntf->needs_remote_wakeup = 1;
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
      kfree( pEndpoints );
      return -ENXIO;
   }

//modify Mac from param arthur 20140122
if(mac_addr != NULL){
	
	str2mac(mac_addr,my_mac_addr);
	
	addr.sa_family = 1;
	memset(addr.sa_data,0,ETH_ALEN);
	memcpy(addr.sa_data, my_mac_addr, ETH_ALEN);
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,29 ))
	pDev->net->set_mac_address(pDev->net,&addr);
#else	
	pDev->net->netdev_ops->ndo_set_mac_address(pDev->net,&addr);
#endif
}
//check the first 4 bits of MAC addr , for they can't be 4 or 6 arthur 20140122
if(((pDev->net->dev_addr[0] & 0xf0) == 0x40) ||
	((pDev->net->dev_addr[0] & 0xf0) == 0x60))
{
	printk("Need to Modify Mac\n" );
	
	addr.sa_family = 1;
	memset(addr.sa_data,0,ETH_ALEN);
	memcpy(addr.sa_data, pDev->net->dev_addr, ETH_ALEN);
	addr.sa_data[0] |= 0x10;
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,29 ))
	pDev->net->set_mac_address(pDev->net,&addr);
#else	
	pDev->net->netdev_ops->ndo_set_mac_address(pDev->net,&addr);
#endif
}

   pGobiDev = kmalloc( sizeof( sGobiUSBNet ), GFP_KERNEL );
   if (pGobiDev == NULL)
   {
      DBG( "falied to allocate device buffers" );
      usbnet_disconnect( pIntf );
      kfree( pEndpoints );
      return -ENOMEM;
   }
   
   pDev->data[0] = (unsigned long)pGobiDev;
   
   pGobiDev->mpNetDev = pDev;
   pGobiDev->mpEndpoints = pEndpoints;

   // Clearing endpoint halt is a magic handshake that brings 
   // the device out of low power (airplane) mode
   // NOTE: FCC verification should be done before this, if required
   pipe = usb_sndbulkpipe( pGobiDev->mpNetDev->udev,
                           pGobiDev->mpEndpoints->mBlkOutEndp );
   usb_clear_halt( pGobiDev->mpNetDev->udev, pipe );

   // Overload PM related network functions
#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,29 ))
   pGobiDev->mpUSBNetOpen = pDev->net->open;
   pDev->net->open = GobiUSBNetOpen;
   pGobiDev->mpUSBNetStop = pDev->net->stop;
   pDev->net->stop = GobiUSBNetStop;
//   pDev->net->hard_start_xmit = GobiUSBNetStartXmit;
//   pDev->net->tx_timeout = GobiUSBNetTXTimeout;
#else
   pNetDevOps = kmalloc( sizeof( struct net_device_ops ), GFP_KERNEL );
   if (pNetDevOps == NULL)
   {
      DBG( "falied to allocate net device ops" );
      usbnet_disconnect( pIntf );
      return -ENOMEM;
   }
   memcpy( pNetDevOps, pDev->net->netdev_ops, sizeof( struct net_device_ops ) );
   
   pGobiDev->mpUSBNetOpen = pNetDevOps->ndo_open;
   pNetDevOps->ndo_open = GobiUSBNetOpen;
   pGobiDev->mpUSBNetStop = pNetDevOps->ndo_stop;
   pNetDevOps->ndo_stop = GobiUSBNetStop;
//   pNetDevOps->ndo_start_xmit = GobiUSBNetStartXmit;
//   pNetDevOps->ndo_tx_timeout = GobiUSBNetTXTimeout;

   pDev->net->netdev_ops = pNetDevOps;
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,31 ))
   memset( &(pGobiDev->mpNetDev->stats), 0, sizeof( struct net_device_stats ) );
#else
   memset( &(pGobiDev->mpNetDev->net->stats), 0, sizeof( struct net_device_stats ) );
#endif

   pGobiDev->mpIntf = pIntf;
   memset( &(pGobiDev->mMEID), '0', 14 );
   
   DBG( "Mac Address:\n" );
   PrintHex( &pGobiDev->mpNetDev->net->dev_addr[0], 6 );

   pGobiDev->mbQMIValid = false;
   memset( &pGobiDev->mQMIDev, 0, sizeof( sQMIDev ) );
   pGobiDev->mQMIDev.mbCdevIsInitialized = false;

   pGobiDev->mQMIDev.mpDevClass = gpClass;
   
   init_completion( &pGobiDev->mAutoPM.mThreadDoWork );
   spin_lock_init( &pGobiDev->mQMIDev.mClientMemLock );

   // Default to device down
   pGobiDev->mDownReason = 0;
   GobiSetDownReason( pGobiDev, NO_NDIS_CONNECTION );
   GobiSetDownReason( pGobiDev, NET_IFACE_STOPPED );

   // Register QMI
   status = RegisterQMIDevice( pGobiDev );
   if (status != 0)
   {
      // usbnet_disconnect() will call GobiNetDriverUnbind() which will call
      // DeregisterQMIDevice() to clean up any partially created QMI device
      usbnet_disconnect( pIntf );
      return status;
   }
   
   // Success
   return 0;
}

void Gobinet_disconnect (struct usb_interface *pIntf)
{
	struct usbnet * pDev;
	sGobiUSBNet * pGobiDev;

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

	pGobiDev = (sGobiUSBNet *)pDev->data[0];
	if (pGobiDev == NULL)
	{
	  DBG( "failed to get QMIDevice\n" );
	  return -ENXIO;
	}
#if 1
	DeregisterQMIDevice(pGobiDev);
#else
	cdev_del(&pGobiDev->mQMIDev.mCdev);
#endif
	usbnet_disconnect(pIntf);
}


static struct usb_driver GobiNet =
{
   .name       = "GobiNet",
   .id_table   = GobiVIDPIDTable,
   .probe      = GobiUSBNetProbe,
   //.disconnect = usbnet_disconnect,
   
   .disconnect = Gobinet_disconnect,
#if (LINUX_VERSION_CODE > KERNEL_VERSION( 2,6,23 ))
   .suspend    = GobiSuspend,
   .resume     = GobiResume,
   .supports_autosuspend = true,
#endif
};

/*===========================================================================
METHOD:
   GobiUSBNetModInit (Public Method)

DESCRIPTION:
   Initialize module
   Create device class
   Register out usb_driver struct

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
static int __init GobiUSBNetModInit( void )
{
   gpClass = class_create( THIS_MODULE, "GobiQMI" );
   if (IS_ERR( gpClass ) == true)
   {
      DBG( "error at class_create %ld\n",
           PTR_ERR( gpClass ) );
      return -ENOMEM;
   }

   // This will be shown whenever driver is loaded
   printk( KERN_INFO "%s: %s\n", DRIVER_DESC, DRIVER_VERSION );

   return usb_register( &GobiNet );
}
module_init( GobiUSBNetModInit );

/*===========================================================================
METHOD:
   GobiUSBNetModExit (Public Method)

DESCRIPTION:
   Deregister module
   Destroy device class

RETURN VALUE:
   void
===========================================================================*/
static void __exit GobiUSBNetModExit( void )
{
   usb_deregister( &GobiNet );

   class_destroy( gpClass );
}
module_exit( GobiUSBNetModExit );

MODULE_VERSION( DRIVER_VERSION );
MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
MODULE_LICENSE("Dual BSD/GPL");
 

#ifdef bool
#undef bool
#endif

module_param( mac_addr, charp, 0 );
MODULE_PARM_DESC( mac_addr, "Input ethernet Mac addr\n");

module_param( debug, bool, S_IRUGO | S_IWUSR );
MODULE_PARM_DESC( debug, "Debuging enabled or not" );

module_param( interruptible, bool, S_IRUGO | S_IWUSR );
MODULE_PARM_DESC( interruptible, "Listen for and return on user interrupt" );

module_param( txQueueLength, int, S_IRUGO | S_IWUSR );
MODULE_PARM_DESC( txQueueLength, 
                  "Number of IP packets which may be queued up for transmit" );

