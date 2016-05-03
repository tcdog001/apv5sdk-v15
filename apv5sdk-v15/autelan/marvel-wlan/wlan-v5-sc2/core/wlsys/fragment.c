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
#include "wldebug.h"
#include "ap8xLnxRegs.h"
#include "ap8xLnxDesc.h"
#include "ap8xLnxIntf.h"
#include "ap8xLnxXmit.h"
#include "ap8xLnxFwcmd.h"
#include "wltypes.h"
#include "wl_macros.h"
#include "IEEE_types.h"
#include "wl_mib.h"
#include "wl_hal.h"
#include "StaDb.h"
#include "ds.h"
#include "ap8xLnxDma.h"
#include "Fragment.h"
#include "wllog.h"

#define	IEEE80211_SEQ_SEQ_SHIFT			4
#define	IEEE80211_SEQ_FRAG_MASK			0x000f

struct ieee80211_frame
{
	IEEEtypes_FrameCtl_t FrmCtl;
	UINT8	dur[2];
	UINT8	addr1[IEEEtypes_ADDRESS_SIZE];
	UINT8	addr2[IEEEtypes_ADDRESS_SIZE];
	UINT8	addr3[IEEEtypes_ADDRESS_SIZE];
	UINT8	seq[2];
	UINT8   addr4[IEEEtypes_ADDRESS_SIZE];
} PACK;

struct ieee80211_qosframe
{
	IEEEtypes_FrameCtl_t FrmCtl;
	UINT8	dur[2];
	UINT8	addr1[IEEEtypes_ADDRESS_SIZE];
	UINT8	addr2[IEEEtypes_ADDRESS_SIZE];
	UINT8	addr3[IEEEtypes_ADDRESS_SIZE];
	UINT8	seq[2];
	UINT8	qos[2];
	UINT8   addr4[IEEEtypes_ADDRESS_SIZE];
};

static INLINE void
__wbuf_set_pktlen(struct sk_buff *skb, uint32_t len)
{
    if (skb->len > len) {
        skb_trim(skb, len);
    }
    else {
        skb_put(skb, (len - skb->len));
    }
}

enum wbuf_type{
    WBUF_TX_DATA = 0,               /* normal tx data frame sent from NDIS */
    WBUF_TX_MGMT,                   /* internally generated management frame */
    WBUF_TX_BEACON,                 /* internally generated beacon frame */
    WBUF_RX,                        /* rx buffer that will be used for DMA */
    WBUF_RX_INTERNAL,               /* rx buffer that won't be used for DMA */
    WBUF_TX_INTERNAL,
#ifndef UMAC
    WBUF_TX_COALESCING,
#endif
    WBUF_TX_CTL,                    /* used to send control frames, currently cfend */
    WBUF_MAX_TYPE
};

struct sk_buff * wbuf_alloc(enum wbuf_type type, unsigned int len)
{
	const unsigned int align = sizeof(unsigned int);
	struct sk_buff *skb;
	unsigned int buflen;
	if ((type == WBUF_TX_DATA) || (type == WBUF_TX_MGMT) ||
            (type == WBUF_TX_BEACON) || (type == WBUF_TX_INTERNAL) ||
            (type == WBUF_TX_CTL))
        buflen = roundup(len+MIN_HEAD_ROOM, 4);
    else
        buflen = roundup(len, 4);
	skb = dev_alloc_skb(buflen + align-1);
	if (skb != NULL)
    {
        u_int off = ((unsigned long) skb->data) % align;
        if (off != 0)
            skb_reserve(skb, align - off);
        if (type == WBUF_TX_DATA || type == WBUF_TX_MGMT || type ==
                WBUF_TX_BEACON || type == WBUF_TX_INTERNAL || type ==
                WBUF_TX_CTL)
            skb_reserve(skb, MIN_HEAD_ROOM);
    }
    return skb;
}

inline void ResetDeFragBufInfo(DeFragBufInfo_t *pDeFragBufInfo)
{
	pDeFragBufInfo->SeqNo = 0;
	pDeFragBufInfo->FragNo = 0;
	pDeFragBufInfo->pFrame= NULL;
}
inline UINT32 GetDeFragBuf(extStaDb_StaInfo_t *pStaInfo, UINT16 SeqNo)
{
	if ((pStaInfo->DeFragBufInfo.SeqNo == SeqNo) )
	{
		return 1;
	}
	return -1;
}


inline UINT32 GetDeFragFreeBuf(extStaDb_StaInfo_t *pStaInfo)
{
	if(pStaInfo->DeFragBufInfo.pFrame)
	{
		dev_kfree_skb_any((struct sk_buff *) pStaInfo->DeFragBufInfo.pFrame);
		ResetDeFragBufInfo(&pStaInfo->DeFragBufInfo);
	}
	return 1;
}


struct sk_buff *DeFragPck(struct net_device *dev,struct sk_buff *skb, extStaDb_StaInfo_t **pStaInfo)
{
	struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);
	vmacApInfo_t *vmacSta_p = wlpptr->vmacSta_p;
	UINT32 BuffNo;
	struct sk_buff *Tmpskb;
	struct ieee80211_frame *wh = (struct ieee80211_frame *) skb->data;
	UINT8 FragNo;
	UINT16 SeqNo;
	UINT16 ieee80211HdrLen = 0;
    *pStaInfo =  extStaDb_GetStaInfo(vmacSta_p,&(wh->addr2), 1);
	ieee80211HdrLen = sizeof(struct ieee80211_frame);
	if (IS_GROUP((UINT8 *)&(wh->addr1)) || !(*pStaInfo))
	{
		return skb;
	}
	Tmpskb = skb;
	FragNo = le16_to_cpu(*(u_int16_t *)(wh->seq)) & IEEE80211_SEQ_FRAG_MASK;
	SeqNo =  le16_to_cpu(*(u_int16_t *)(wh->seq)) >> IEEE80211_SEQ_SEQ_SHIFT;
	if (!wh->FrmCtl.MoreFrag && FragNo == 0)
		return skb;
	if (FragNo == 0)
	{ //first fragment
		BuffNo = GetDeFragFreeBuf(*pStaInfo);
		if (BuffNo != 0xFFFFFFFF)
		{
			(*pStaInfo)->DeFragBufInfo.pFrame = skb;
			if (wh->FrmCtl.MoreFrag)
			{
				if (skb_is_nonlinear(skb))
				{
					(*pStaInfo)->DeFragBufInfo.pFrame= skb_copy(skb, GFP_ATOMIC);
					(*pStaInfo)->DeFragBufInfo.pFrame->protocol = skb->protocol;
					(*pStaInfo)->DeFragBufInfo.pFrame->dev = skb->dev;
					dev_kfree_skb_any(skb);
				} else if (skb->end - skb->head < dev->mtu +
					ieee80211HdrLen)
				{
					(*pStaInfo)->DeFragBufInfo.pFrame = skb_copy_expand(skb, 0,
						(dev->mtu  +
						ieee80211HdrLen)
						- (skb->end - skb->head), GFP_ATOMIC);
					(*pStaInfo)->DeFragBufInfo.pFrame->protocol = skb->protocol;
					(*pStaInfo)->DeFragBufInfo.pFrame->dev = skb->dev;
					dev_kfree_skb_any(skb);
				}
			}
		} else /** No buffer **/
		{
			dev_kfree_skb_any((struct sk_buff *)Tmpskb);
			return NULL;
		}
	} else
	{   //Get the DeFrag buff with the given sequence no

		BuffNo = GetDeFragBuf(*pStaInfo, SeqNo);
		if (BuffNo != 0xFFFFFFFF)
		{
			if ((*pStaInfo)->DeFragBufInfo.FragNo + 1 == FragNo)
			{
				struct ieee80211_frame *wh1 = (struct ieee80211_frame *)
					(*pStaInfo)->DeFragBufInfo.pFrame->data;
				/* Copy current fragment at end of previous one */
				memcpy((*pStaInfo)->DeFragBufInfo.pFrame->tail,
					skb->data + ieee80211HdrLen,
					skb->len - ieee80211HdrLen
					);
				/* Update tail and length */
				skb_put((*pStaInfo)->DeFragBufInfo.pFrame,
					skb->len - ieee80211HdrLen);
				/* Keep a copy of last sequence and fragno */
				*(u_int16_t *) wh1->seq = *(u_int16_t *) wh1->seq;
				(*pStaInfo)->DeFragBufInfo.pFrame->protocol = skb->protocol;
				(*pStaInfo)->DeFragBufInfo.pFrame->dev = skb->dev;
				dev_kfree_skb_any(skb);

			} else  /** fragment not in sequence **/
			{
				dev_kfree_skb_any((struct sk_buff *) (*pStaInfo)->DeFragBufInfo.pFrame);
				ResetDeFragBufInfo(&((*pStaInfo)->DeFragBufInfo));
				dev_kfree_skb_any((struct sk_buff *)Tmpskb);
				return NULL;
			}
		} else  /** Error condition **/
		{
			dev_kfree_skb_any((struct sk_buff *)Tmpskb);
			return NULL;
		}
	}

	/** Last Fragment handle here **/
	if (wh->FrmCtl.MoreFrag == 0)
	{
		skb= (*pStaInfo)->DeFragBufInfo.pFrame;
		ResetDeFragBufInfo(&((*pStaInfo)->DeFragBufInfo));
		return(skb);
	} else
	{
		(*pStaInfo)->DeFragBufInfo.SeqNo = SeqNo;
		(*pStaInfo)->DeFragBufInfo.FragNo = FragNo;
		return NULL;
	}
}
/* lirui add for fragment begin */
#if 1
int
ieee80211_fragment(struct net_device *dev, struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo,unsigned int mtu, 
						unsigned int hdrsize, unsigned int ciphdrsize)
{
	struct ieee80211_qosframe *wh, *whf;
	struct sk_buff *wbuf, *prev, *wbuf0 = skb;
	u_int totalhdrsize, fragno, fragsize, off, remainder, payload;
	
    hdrsize = roundup(hdrsize, sizeof(u_int32_t));
	wh = (struct ieee80211_qosframe *) (wbuf0->data);
	/* NB: mark the first frag; it will be propagated below */
	wh->FrmCtl.MoreFrag = 1;
	totalhdrsize = hdrsize + ciphdrsize;
	fragno = 1;
	off = mtu;
	remainder = wbuf0->len - off;
	prev = wbuf0;
	do {
		fragsize = remainder + totalhdrsize;
		if (fragsize > mtu)
			fragsize = mtu;
        wbuf = wbuf_alloc(WBUF_TX_DATA, fragsize);
		if (wbuf == NULL)
			goto bad;

		memcpy(wbuf->cb, wbuf0->cb, sizeof(wbuf->cb));
		
		skb_copy_queue_mapping(wbuf, wbuf0);
		wbuf->priority = wbuf0->priority;
		wbuf->dev = wbuf0->dev;
        skb_put(wbuf, ciphdrsize);

        if (!skb_pull(wbuf,ciphdrsize))
			goto bad;

		/*
		 * Form the header in the fragment.  Note that since
		 * we mark the first fragment with the MORE_FRAG bit
		 * it automatically is propagated to each fragment; we
		 * need only clear it on the last fragment (done below).
		 */
		whf = (struct ieee80211_qosframe *)wbuf->data;
		memmove(whf, wh, hdrsize);
		*(u_int16_t *)&whf->seq[0] |= htole16(
			(fragno & IEEE80211_SEQ_FRAG_MASK) <<
            IEEE80211_SEQ_FRAG_SHIFT);
		fragno++;

		payload = fragsize - totalhdrsize;
		skb_copy_bits(wbuf0, off, (u_int8_t *)whf + totalhdrsize, payload);
		wbuf_set_pktlen(wbuf, totalhdrsize + payload);

		/* chain up the fragment */
        prev->next = wbuf;
		prev = wbuf;

		/* deduct fragment just formed */
		remainder -= payload;
		off += payload;
		wlxmit(dev, wbuf, IEEE_TYPE_DATA, pStaInfo, 0, 1);
	
	} while (remainder != 0);
	
	whf->FrmCtl.MoreFrag = 0;
	wbuf->next = NULL;
	/* strip first mbuf now that everything has been copied */
	//modify by lirui for fragment begin
	skb_trim(wbuf0, mtu);
	//modify by lirui for fragment end
	return true;

bad:
	printk("CHH(%s): Bad Frag Return, remainder = %d\n", __func__, remainder);
	return false;
}


bool ieee80211_check_and_fragment(struct net_device *dev, struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo)
{
    struct ieee80211_qosframe *wh = NULL;
    unsigned int hdrsize = 0;
	//struct sk_buff *skb_p = skb;
	//int i = 0;
	struct wlprivate *wlpptr = (struct wlprivate *)dev->ml_priv;
	vmacApInfo_t *vmacSta_p = wlpptr->vmacSta_p;
	unsigned int mtu = vmacSta_p->Mib802dot11->OperationTable->FragThresh;
	unsigned int ciphdrsize = 0;
    int txfrag;
	
	if(mtu >= 0x2300)
		return FRAG_FAIL;
	
	wh = (struct ieee80211_qosframe *)skb->data;
	if (wh->FrmCtl.Type != IEEE_TYPE_DATA)
		return FRAG_FAIL;
	hdrsize = sizeof(struct ieee80211_qosframe);
#if 0
	/*Check for address 4*/
	//hdrsize -= (((1 == wh->FrmCtl.FromDs) && (1 == wh->FrmCtl.ToDs)) ? 0 : 6);
	/*Check for QoS*/
	hdrsize += ((8 == wh->FrmCtl.Subtype) ? 2:0);
	/*Check for encryption*/
	ciphdrsize = (wh->FrmCtl.Wep & 1 ? 4 : 0);
#endif
    /*Check whether the fragment should be*/
    txfrag = ((skb->len > mtu) &&
              !IEEE80211_IS_MULTICAST(wh->addr1));		/* NB: don't fragment ff's */

    if (unlikely(txfrag)) {
		
        ieee80211_fragment(dev, skb, pStaInfo, mtu, hdrsize, ciphdrsize);
		return FRAG_SEND;
		
    }
	return FRAG_FAIL;  
}
#endif
/*lirui add for fragment end */

