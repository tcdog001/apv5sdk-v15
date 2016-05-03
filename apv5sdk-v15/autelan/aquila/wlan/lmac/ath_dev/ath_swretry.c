/*
 * Copyright (c) 2010, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "ath_internal.h"
#include "if_athrate.h"

#ifdef ATH_SWRETRY
/* Debug function to dump the status of the Queue
 * This function will be used handy to debug the 
 * buffer corruptions.
 * Prints Next, Previous and Last buffers of first
 * buffer of every frm in the Q. Also it prints all 
 * the control words of first desc of every frame and
 * status words of last desc of every frame.
 */
void
dumpTxQueue(struct ath_softc *sc, ath_bufhead *bfhead) 
{
    struct ath_buf *bf = TAILQ_FIRST(bfhead);
    struct ath_desc *ds, *lastds;
    struct ieee80211_frame      *wh;
  
    if (!bf)
        printk("EMPTY QUEUE\n");
 
    while (bf) {
       ds = bf->bf_desc;
       if (bf->bf_lastfrm) {
           wh = (struct ieee80211_frame *)wbuf_header(bf->bf_mpdu);
           lastds = bf->bf_lastfrm->bf_desc;
           printk("\nSWRInfo: seqno %d isswRetry %d retryCnt %d\n",wh ? (*(u_int16_t *)&wh->i_seq[0]) >> 4 : 0, bf->bf_isswretry,bf->bf_swretries);

          
       printk("Buffer #%p --> Next#%p Prev#%p Last#%p\n",bf, TAILQ_NEXT(bf,bf_list), 
                TAILQ_PREV(bf, ath_bufhead_s, bf_list), bf->bf_lastfrm);
       printk("                     Stas#%08X flag#%08X Node#%p\n", bf->bf_status, bf->bf_flags, bf->bf_node);

       printk("Descr  #%08X --> Next#%08X Data#%08X Ctl0#%08X Ctl1#%08X\n", bf->bf_daddr, ds->ds_link, ds->ds_data, ds->ds_ctl0, ds->ds_ctl1);
       printk("                     Ctl2#%08X Ctl3#%08X Sta0#%08X Sta1#%08X\n",ds->ds_hw[0], ds->ds_hw[1], lastds->ds_hw[2], lastds->ds_hw[3]);
       }       
       bf = TAILQ_NEXT(bf,bf_list);
    }                           

}

/* Interface function for the IEEE layer to manipulate
 * the software retry state. Used during BMISS and 
 * scanning state machine in IEEE layer
 */
void 
ath_set_swretrystate(ath_dev_t dev, ath_node_t node, int flag)
{
    struct ath_softc *sc = ATH_DEV_TO_SC(dev);
    struct ath_node *an = ATH_NODE(node);

    if (sc->sc_swRetryEnabled) {
        cmpxchg(&an->an_swrenabled, an->an_swrenabled, flag);

        DPRINTF(sc, ATH_DEBUG_SWR, "%s: SWRetry ---> %s\n",__func__, 
                                        flag ? "ENABLED" : "DISABLED");
    }
}

/* Check if a frame can be sw retried at this state of driver.
 * If the frame can be sw retried then mark the buf as it is 
 * going for MARKEDSWRETRY
 *
 * The following function is based on following assumptions:
 * 1. Multi/Broadcast frames which are xmited with needDestMask = 1
 *    are never retried because txok is always 1 for them (No ack)
 * 2. Multi/Broadcast frames which are xmited with needDestMask = 0
 *    are only retried if txok = 0, which means the previous frm got
 *    failed.
 * 3. Frames are not sw retried only after association, bymeans of 
 *    an_swrenabled flag.
 * 4. Come out of SWR mechanism when we are going to scan (bk/fore)
 */
int
ath_check_swretry_req(struct ath_softc *sc, struct ath_buf *bf)
{
    /* The following cases are handled here:
     * 1. Suppose BK scan thread trying to shift to foreign channel then
     *    it first tries to complete the frames on the Q. If not completed within
     *    time, it flushes the Q. If we are in middle of SW Retry process and another
     *    thread tries to shift the channel, then we complete the frames normally even
     *    if they are in filtered state. Otherwise there are race conditions which may 
     *    result in sending frames when channel is shifted.
     * 2. Letsay Bmiss happens in the middle of SW retry process. Then we should turn
     *    off the SW retry process as it is of no use retrying with different rates.
     *    This should be handled with an_swrenabled flag.
     * 3. If a SW retried frame is sucessfully transmitted, take care to complete this
     *    frame as any other frame.
     * 4. Frames which complete on first attempt pass through if condition as they are 
     *    not marked with MARKEDSWRETRY
     * 5. Frames which are marked with MARKEDSWRETRY only passes through else condition.
     */
     
    if (sc->sc_scanning || (!bf->bf_node) ||
          (bf->bf_node && !(ATH_NODE(bf->bf_node)->an_swrenabled))) {
        return AH_FALSE;
    }
    
    return AH_TRUE;
}

/* This function is responsible for:
 * 1. reforming a frame which has to be sent for sw retry
 * 2. marking the last buf of the frame as STALE
 * 3. Copying the contents of stale buf into fresh buf and 
 *    assembling the bufs to form a chain.
 */
struct ath_buf *
ath_form_swretry_frm(struct ath_softc *sc, struct ath_txq *txq, ath_bufhead *bf_q, struct ath_buf *bf) {

    struct ath_buf *tbf, *pbf = NULL;
    struct ath_buf *lastbf = bf->bf_lastbf;
    int nmaps;

     /* If non successful frame is marked as STALE then
      * SWRetry code may try to use that buf which becomes
      * fatal. Handle this case here
      */
    ATH_TXBUF_LOCK(sc);
    tbf = TAILQ_FIRST(&sc->sc_txbuf);
    ASSERT(tbf != NULL); /* this can't be NULL */
    TAILQ_REMOVE(&sc->sc_txbuf, tbf, bf_list);
/*zhaoyang modify for add CL101673.fix.patch for dusi*/
//#if ATH_TX_BUF_FLOW_CNTL
#if 1
    sc->sc_txbuf_free--;
#endif
#if ATH_TX_BUF_FLOW_CNTL
/*zhaoyang modify end*/
    txq->axq_num_buf_used++;
#endif
    ATH_TXBUF_UNLOCK(sc);

    ATH_TXBUF_RESET(tbf, sc->sc_num_txmaps);
    ATH_TXBUF_SWRETRY_RESET(tbf);

    TAILQ_INSERT_TAIL(bf_q, tbf, bf_list);

    /* copy descriptor/bf content */
    tbf->bf_mpdu = lastbf->bf_mpdu;
    tbf->bf_node = lastbf->bf_node;
    for (nmaps = 0; nmaps < sc->sc_num_txmaps; nmaps++) {
        tbf->bf_buf_addr[nmaps] = lastbf->bf_buf_addr[nmaps];
        tbf->bf_buf_len[nmaps] = lastbf->bf_buf_len[nmaps];
    }
    OS_MEMCPY(tbf->bf_desc, lastbf->bf_desc, sc->sc_txdesclen);
    tbf->bf_state = lastbf->bf_state;
    tbf->bf_status = (lastbf->bf_status & ~ATH_BUFSTATUS_STALE);
    tbf->bf_flags = lastbf->bf_flags;
    ath_hal_setdesclink(sc->sc_ah, tbf->bf_desc, 0);

    pbf = TAILQ_PREV(tbf, ath_bufhead_s, bf_list);

    /* link it to the appropriate frame */
    if (pbf && !(pbf->bf_status & ATH_BUFSTATUS_STALE)) {
        ath_hal_setdesclink(sc->sc_ah, pbf->bf_desc, tbf->bf_daddr);

        /* Change the lastfrm/lastbf for the first buff of
         * the current frame/frag
         */

        while (pbf) {
           if (pbf->bf_lastbf) {
               /*Here pbf points to the first buf
                * of current frame
                */
                ASSERT(pbf->bf_lastbf == lastbf);
                /* If the above assert hits we are 
                 * in the verge of fatality.
                 */
                pbf->bf_lastfrm = tbf;
                pbf->bf_lastbf = tbf;
                break;
            }
            pbf = TAILQ_PREV(pbf, ath_bufhead_s, bf_list);
        }

        /* Need to clear the Tx Desc Status words. This will
         * be done at later stage as it is required for all the 
         * frags/frms
         */

    } else {
         /* This is the only and last buf of the current frame
          * Point bf to replaced buf.
          */
         bf = tbf;  
         tbf->bf_lastfrm = tbf;
         tbf->bf_lastbf = tbf;

         /*Reset the lastfrm/bf for the stale*/
         lastbf->bf_lastfrm = NULL; 
         lastbf->bf_lastbf = NULL;

         lastbf->bf_status &= ~ATH_BUFSTATUS_MARKEDSWRETRY;
         tbf->bf_status |= ATH_BUFSTATUS_MARKEDSWRETRY;
    }

    return bf;
}

/* This function drains the frames from the sw xmit 
 * Q to corresponding HWQ. Before calling this function
 * we should ensure there are no pending frames in the 
 * HW Q for sw retry process.
 */
void
ath_tx_drain_sxmitq(struct ath_softc *sc, struct ath_node *an)
{
    struct ath_buf *bf = NULL;
    struct ath_txq *txq = NULL;
    struct ath_atx_tid *tid;
    ath_bufhead bf_head;
    int status;

    /* Ensure we have cleared all the swretry frames
     * by the time we reach to this function
     */
    ASSERT(!an->an_total_swrtx_pendfrms);

    do {
        ATH_NODE_SWRETRY_TXBUF_LOCK(an);
        bf = TAILQ_FIRST(&an->an_softxmit_q);
        if (!bf) {
            ATH_NODE_SWRETRY_TXBUF_UNLOCK(an);
            break;
        }

        TAILQ_INIT(&bf_head);
        txq = &sc->sc_txq[bf->bf_qnum];

        TAILQ_REMOVE_HEAD_UNTIL(&an->an_softxmit_q, &bf_head, bf->bf_lastbf, bf_list);              
        an->an_softxmit_qdepth--;
        ATH_NODE_SWRETRY_TXBUF_UNLOCK(an);

        DPRINTF(sc, ATH_DEBUG_SWR, "%s: drain frm with SeqNo%d from SxQ: SxQdepth %d pendfrms %d\n",__func__, 
                    (*(u_int16_t *)&((struct ieee80211_frame *)wbuf_header(bf->bf_mpdu))->i_seq[0]) >> 4,
                    an->an_softxmit_qdepth, an->an_total_swrtx_pendfrms);

        ATH_TXQ_LOCK(txq);
        status = ath_tx_txqaddbuf(sc, txq, &bf_head);
        ATH_TXQ_UNLOCK(txq);

        if (status != 0) {
            /* for aggr frame, complet it.
             * for non-aggr HT frame with single sub-frame, update the baw before
             * completing it.
             */
            if (!bf->bf_isaggr) {
                if (bf->bf_isampdu && bf->bf_nframes == 1) {
                    tid = ATH_AN_2_TID(an, bf->bf_tidno);
                    if (tid)
                        ath_tx_update_baw(sc, tid, bf->bf_seqno);
                }
#ifdef  ATH_SUPPORT_TxBF
                ath_tx_complete_buf(sc, bf, &bf_head, 0, 0, 0);
#else
                ath_tx_complete_buf(sc, bf, &bf_head, 0);
#endif
            }
            else {
                struct ath_buf *lastbf = bf->bf_lastbf;
                ath_tx_complete_aggr_rifs(sc, txq, bf, &bf_head,
                                          &((struct ath_desc *)(lastbf->bf_desc))->ds_txstat, 0);
            }
        }
    } while (TRUE);
    
    return;
}

void
ath_tx_flush_node_sxmitq(struct ath_softc *sc, struct ath_node *an)
{
    struct ath_buf *bf = NULL;
    struct ath_buf *lastbf = NULL;
    struct  ath_swretry_info *pInfo;
    ath_bufhead bf_head;
    struct ath_atx_tid *tid;
   
    do {
        TAILQ_INIT(&bf_head);   
        ATH_NODE_SWRETRY_TXBUF_LOCK(an);
        bf = TAILQ_FIRST(&an->an_softxmit_q);
        if (!bf) {
            ATH_NODE_SWRETRY_TXBUF_UNLOCK(an);
            break;
        }
        lastbf = bf->bf_lastfrm;
        TAILQ_REMOVE_HEAD_UNTIL(&an->an_softxmit_q, &bf_head, lastbf, bf_list);
        an->an_total_swrtx_flushfrms++;
        an->an_softxmit_qdepth--;

        pInfo = &an->an_swretry_info[bf->bf_qnum];
        pInfo->swr_num_pendfrms --;
        if (!pInfo->swr_num_pendfrms) {
            pInfo->swr_need_cleardest = AH_FALSE;
        }
        an->an_total_swrtx_pendfrms --;

        ATH_NODE_SWRETRY_TXBUF_UNLOCK(an);

        DPRINTF(sc, ATH_DEBUG_SWR, "%s: flush frm with SeqNo%d from SxQ: SxQdepth %d pendfrms %d\n",__func__, 
        (*(u_int16_t *)&((struct ieee80211_frame *)wbuf_header(bf->bf_mpdu))->i_seq[0]) >> 4,
        an->an_softxmit_qdepth, an->an_total_swrtx_pendfrms);

		if (bf->bf_isampdu) {
        	tid = ATH_AN_2_TID(an, bf->bf_tidno);
        	if (tid)
            	ath_tx_update_baw(sc, tid, bf->bf_seqno);
		}
#ifdef  ATH_SUPPORT_TxBF
        ath_tx_complete_buf(sc, bf, &bf_head, 0, 0, 0);
#else
        ath_tx_complete_buf(sc, bf, &bf_head, 0);
#endif

    } while (TRUE);
}

void
ath_tx_modify_cleardestmask(struct ath_softc *sc, struct ath_txq *txq, ath_bufhead *bf_head)
{
    struct ath_hal *ah = sc->sc_ah;
    struct ath_buf  *bf = TAILQ_FIRST(bf_head);
    struct ath_desc *ds = bf->bf_desc;
    struct ath_node *an = ATH_NODE(bf->bf_node);
    struct ath_swretry_info *pInfo = &an->an_swretry_info[txq->axq_qnum];
    wbuf_t wbuf = bf->bf_mpdu;
    struct ieee80211_frame *wh = (struct ieee80211_frame *)wbuf_header(wbuf);
     
    if (pInfo->swr_need_cleardest || txq->axq_destmask) {
        pInfo->swr_need_cleardest = AH_FALSE;
        txq->axq_destmask = AH_FALSE;
        ath_hal_cleardestmask(ah, ds);
    }
    DPRINTF(sc, ATH_DEBUG_SWR, "%s: SeqNo%d cleardest %s\n",__func__,                 
            (*(u_int16_t *)&wh->i_seq[0]) >> 4, (ds->ds_ctl0 & 0x01000000)? "AH_TRUE" : "AH_FALSE");
}

void
ath_tx_modify_rateseries(struct ath_softc *sc, ath_bufhead *bf_head)
{
    struct ath_buf  *bf = TAILQ_FIRST(bf_head);
    struct ath_node *an = ATH_NODE(bf->bf_node);
    wbuf_t wbuf = bf->bf_mpdu;
    struct ath_rc_series rcs[4];
#ifdef ATHR_RNWF
    ieee80211_tx_control_t  *txctl = (ieee80211_tx_control_t *)wbuf_get_context(wbuf);
    struct ath_rc_series    *txctl_rcs = (struct ath_rc_series *)&txctl->priv[0];
#endif
    int isProbe, numtries = bf->bf_swretries;

    /* XXX TODO Is there a better way
     * of finding the alternates rates???
     */
    ath_rate_findrate(sc, an, bf->bf_shpreamble, wbuf_get_pktlen(bf->bf_mpdu),
                      ATH_TXMAXTRY, ATH_RC_PROBE_ALLOWED,
                      TID_TO_WME_AC(wbuf_get_tid(wbuf)), rcs, &isProbe,
                      (numtries > (sc->sc_num_swretries/2) ? AH_TRUE : AH_FALSE),
                      bf->bf_flags);

    /*
     * Fill in the TxContext which will be used in Tx completion path.
     * For now, this is only done in RNWF because txctl is a local variable in
     * other platforms.
     */
#ifdef ATHR_RNWF
    txctl_rcs[0] = rcs[0];
    txctl_rcs[1] = rcs[1];
    txctl_rcs[2] = rcs[2];
    txctl_rcs[3] = rcs[3];
#endif

    bf->bf_rcs[0] = rcs[0];
    bf->bf_rcs[1] = rcs[1];
    bf->bf_rcs[2] = rcs[2];
    bf->bf_rcs[3] = rcs[3];

    ath_buf_set_rate(sc, bf);
}

/* This function is responsible for queing the sw retry
 * frms onto the HWQ. Also handles the modification of 
 * clear destination mask, rate series and clearing status
 * words.
 */
HAL_STATUS
ath_tx_mpdu_resend(struct ath_softc *sc, struct ath_txq *txq, ath_bufhead *bf_head, struct ath_tx_status tx_status)
{
    struct ath_buf  *bf = TAILQ_FIRST(bf_head);
    struct ath_desc *ds = bf->bf_lastfrm->bf_desc;
    struct ath_node *an = ATH_NODE(bf->bf_node);
    struct ath_swretry_info *pInfo = &an->an_swretry_info[txq->axq_qnum];
    struct ieee80211_frame  *wh = (struct ieee80211_frame *)wbuf_header(bf->bf_mpdu);
    struct ath_tx_status ts;
    int    status;
    struct ath_atx_tid *tid;
     
    if (sc->sc_enhanceddmasupport)
        ts = tx_status;
    else
        ts = ds->ds_txstat;

    do {
        if (ts.ts_status & HAL_TXERR_XRETRY) {
            pInfo->swr_need_cleardest = AH_TRUE;
            bf->bf_swretries++;

            bf->bf_totaltries += ts.ts_longretry;
            bf->bf_totaltries += ts.ts_shortretry;
         
            /* Attempted max number of sw retries, 
             * clear the status and complete the buf
             */
         
            if (bf->bf_swretries > sc->sc_num_swretries) {
                bf->bf_status &= ~ATH_BUFSTATUS_MARKEDSWRETRY;         
                sc->sc_halstats.ns_swretryfailcount++;
                break;
            }   
         
            if (bf->bf_swretries == 1) {
                wh->i_fc[1] |= IEEE80211_FC1_RETRY;             
                sc->sc_halstats.ns_swretryframecount++;
                /* There are two cases here:
                 * 1. Frame which is failed with any error other than 
                 *    Excessive retry, fails second time with excessive
                 *    retry. In that case the pendfrms is incremented in 
                 *    the first time. Dont increment this counter again.
                 * 2. Frame which is failed for first time with excessive 
                 *    retry. Increment the pendfrms counter.
                 */
                if (!bf->bf_isswretry) {
                    pInfo->swr_num_pendfrms++;
                    an->an_total_swrtx_pendfrms++;
                }    
            }    
        } else {
            /* Frame failed because of any Tx Err other than excessive
             * retry like FIFO_UNDERUN/FILTERED. Increment the pendfrms 
             * on this Q.
             * XXX TBD the max number of times the frame has to be retried
             * for frames which failed on other tx errors is not done at the 
             * moment.
             */
            if (!bf->bf_isswretry) {
                if (!(an->an_total_swrtx_pendfrms))
                    pInfo->swr_need_cleardest = AH_TRUE;
                pInfo->swr_num_pendfrms++;
                an->an_total_swrtx_pendfrms++;
            }

            DPRINTF(sc, ATH_DEBUG_SWR, "%s: SWseqno%d SWRcleardest %d\n",__func__, 
                    (*(u_int16_t *)&wh->i_seq[0]) >> 4, pInfo->swr_need_cleardest);
        }    
      
        ASSERT(bf);
        ASSERT(!(bf->bf_status & ATH_BUFSTATUS_STALE));
 
        /* bf should be the first buff for the frame.
         */
        if (!(bf->bf_isswretry)) {
            /* Mark the frame which says the frame is going 
             * through sw retry process. All the frames which are 
             * sw retried are marked with this flag.
             */
            bf->bf_isswretry = 1;
        }
      
        /* Clear the status words, change the RateSeries  
         * and cleardestmask for the retry frame. Care
         * should be taken these words might be in LE which 
         * are sometimes not understandable by CPU
         */
        if (!sc->sc_enhanceddmasupport)
            ath_hal_cleartxdesc(sc->sc_ah, ds);
        ath_tx_modify_rateseries(sc, bf_head);
        ath_tx_modify_cleardestmask(sc, txq, bf_head);

        ATH_TXQ_LOCK(txq);
        status = ath_tx_txqaddbuf(sc, txq, bf_head);
        ATH_TXQ_UNLOCK(txq);    

        /*
         * Frame could not be successfully queued, drop it.
         */
        if (status != 0)
            break;

        return HAL_OK;
        
    } while(0);
     
     pInfo->swr_num_pendfrms--;
    if (an->an_total_swrtx_pendfrms != 0)
     	an->an_total_swrtx_pendfrms--;

     DPRINTF(sc, ATH_DEBUG_SWR, "%s: Dropping swretry SeqNo%d --> retrycnt %d totaltries %d\n",__func__,
             (*(u_int16_t *)&wh->i_seq[0]) >> 4, bf->bf_swretries, bf->bf_totaltries);

	if (bf->bf_isampdu) {	
		tid = ATH_AN_2_TID(an, bf->bf_tidno);
		if (tid)
		    ath_tx_update_baw(sc, tid, bf->bf_seqno);
	}
#ifdef ATH_SUPPORT_TxBF
    if (sc->sc_enhanceddmasupport)
        ath_tx_complete_buf(sc, bf, bf_head, 0, ts.ts_txbfstatus, 0);
    else
        ath_tx_complete_buf(sc, bf, bf_head, 0, 0, 0);
#else
     ath_tx_complete_buf(sc, bf, bf_head, 0);
#endif

    if (sc->sc_enhanceddmasupport) {
        ATH_TXQ_LOCK(txq);
        ath_txq_schedule(sc, txq);
        ATH_TXQ_UNLOCK(txq);
    }

     if (!an->an_total_swrtx_pendfrms) 
         ath_tx_drain_sxmitq(sc, an);

     return HAL_EINPROGRESS;
}
#endif
