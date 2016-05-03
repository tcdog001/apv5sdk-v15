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

/*
 * WPI SMS4 Crypto support
 */
#include <osdep.h>


#if ATH_SUPPORT_WAPI

#include "ieee80211_crypto_wpi_sms4_priv.h"

static	int nrefs = 0;

static	void *wpi_sms4_attach(struct ieee80211vap *, struct ieee80211_key *);
static	void wpi_sms4_detach(struct ieee80211_key *);
static	int wpi_sms4_setkey(struct ieee80211_key *);
static	int wpi_sms4_encap(struct ieee80211_key *, wbuf_t, u_int8_t);
static  int wpi_sms4_decap(struct ieee80211_key *, wbuf_t, int, struct ieee80211_rx_status *);
static	int wpi_sms4_enmic(struct ieee80211_key *, wbuf_t, int);
static	int wpi_sms4_demic(struct ieee80211_key *, wbuf_t, int, int, struct ieee80211_rx_status *);
/*zhaoyang1 transplant from 717*/
/*zhaoyang modify for replay trap CHINAMOBILE-119*/
int wapi_callback_wai(struct ieee80211vap *vap, u_int8_t *addr, u_int8_t *ie, u_int16_t mag_type);
/*zhaoyang modify end*/
/*zhaoyang1 transplant end*/

static const struct ieee80211_cipher wpi_sms4 = {
	"WPI_SMS4",
	IEEE80211_CIPHER_WAPI,
	IEEE80211_WPI_SMS4_KIDLEN +IEEE80211_WPI_SMS4_PADLEN + IEEE80211_WPI_SMS4_IVLEN,
	IEEE80211_WPI_SMS4_MICLEN,
	0,
	wpi_sms4_attach,
	wpi_sms4_detach,
	wpi_sms4_setkey,
	wpi_sms4_encap,
	wpi_sms4_decap,
	wpi_sms4_enmic,
	wpi_sms4_demic,
};
#if 0	
int wpi_cipher(
	u_int8_t *in,
	u_int8_t *out,
	int enc_length,
	int data_type,
	int QOS_en,
	int Endian_mode
	);
#endif
static void *
wpi_sms4_attach(struct ieee80211vap *vap, struct ieee80211_key *k)
{
    struct sms4_ctx *ctx;
    struct ieee80211com *ic = vap->iv_ic;

    ctx = (struct sms4_ctx *)OS_MALLOC(ic->ic_osdev, sizeof(struct sms4_ctx), GFP_KERNEL);
    if (ctx == NULL) {
        vap->iv_stats.is_crypto_nomem++;
        return NULL;
    }
	ctx->sms4c_vap = vap;
	ctx->sms4c_ic = vap->iv_ic;
	nrefs++;
    
    return ctx;
}

static void
wpi_sms4_detach(struct ieee80211_key *k)
{
    struct sms4_ctx *ctx = k->wk_private;

    KASSERT(nrefs > 0, ("imbalanced attach/detach"));
    OS_FREE(ctx);
    nrefs--;			/* NB: we assume caller locking */
}

static int
wpi_sms4_setkey(struct ieee80211_key *k)
{
    struct sms4_ctx *ctx = k->wk_private;
	if (k->wk_keylen != (128/NBBY)) 
	{
        IEEE80211_DPRINTF(ctx->sms4c_vap, IEEE80211_MSG_CRYPTO,
                          "%s: Invalid key length %u, expecting %u\n",
                          __func__, k->wk_keylen, 128/NBBY);
		return 0;
	}
	k->wk_keytsc = 1;		/* TSC starts at 1 */
	return 1;
}

void htonl_iv(u_int8_t *iv, u_int32_t *sta_iv)
{
	int i = 0;
	
	for(i=0; i<IEEE80211_WPI_SMS4_IVLEN/4; i++)
	{
		*(u_int32_t *)(iv+i*4) = htobe32(*(sta_iv + i));
	}
}
void update_send_iv(u_int32_t *iv, int type)
{
	longint_add(iv, type+1, IEEE80211_WPI_SMS4_IVLEN/4);
}

/*
 * Add privacy headers appropriate for the specified key.
 */

static int
wpi_sms4_encap(struct ieee80211_key *k, wbuf_t wbuf, u_int8_t keyid)
{
    struct sms4_ctx *ctx = k->wk_private;
	struct ieee80211vap *vap = ctx->sms4c_vap;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_frame *wh;
	int hdrlen;
	u_int8_t iv[IEEE80211_WPI_SMS4_IVLEN] = {0,};
	int ismsk = 0;
    u_int8_t *ivp;
    int i;

    wh = (struct ieee80211_frame *)wbuf_header(wbuf);
	hdrlen = ieee80211_hdrspace(ic, wbuf_header(wbuf));

	if((wh->i_addr1[0] & 0x01) != 0)
	{
		update_send_iv((u_int32_t *)k->wk_txiv, 0);
		ismsk = 1;
	}
	else
	{
		update_send_iv((u_int32_t *)k->wk_txiv, 1);
	}
	htonl_iv(iv, (u_int32_t *)k->wk_txiv);

#ifndef __CARRIER_PLATFORM__
    ivp = (u_int8_t *)wbuf_push(wbuf, wpi_sms4.ic_header);
    memmove(ivp, ivp + wpi_sms4.ic_header, hdrlen);
#else
    if (wbuf_is_encap_done(wbuf)) {
        ivp = (u_int8_t *)wbuf_header(wbuf);
    } else {
        ivp = (u_int8_t *)wbuf_push(wbuf, wpi_sms4.ic_header);
        memmove(ivp, ivp + wpi_sms4.ic_header, hdrlen);
    }
#endif

    ivp += hdrlen;
    ivp[0] = (keyid >> 6) & 0xFF;
    ivp[1] = 0;
    ivp += 2;
   	for(i=0; i<IEEE80211_WPI_SMS4_IVLEN; i++)
	{
		ivp[i] = (iv[IEEE80211_WPI_SMS4_IVLEN-1-i])&0xff;	
	}
    /*
     * MIC will be appended by DMA (Tx desc control register 8: ds_ctl6),
     * so encap function does not need to append the MIC data to wbuf here.
     */
    k->wk_keytsc++;
	return 1;
}

/*
 * Add MIC to the frame as needed.
 */
static int
wpi_sms4_enmic(struct ieee80211_key *k, wbuf_t wbuf, int force)
{
	return 1;
}

/*zhaoyang1 transplant from 717*/
/*zhaoyang modify for wpi error frame count CHINAMOBILE-116*/
/*
 * check kid used by sta.
 */
int ieee80211_check_kid(struct ieee80211_node *ni, int keyid)
{
	if(keyid >1)
	{
		IEEE80211_NOTE_MAC(ni->ni_vap, IEEE80211_MSG_CRYPTO,
			ni->ni_macaddr, "kid(0x%02x) is wrong\n", keyid);
		return -1;
	}
	if((ni->rekey_done == 0) && (keyid == ni->ni_wknext) )
	{
		ni->ni_wkused = keyid;
		ni->ni_wknext = (!ni->ni_wkused)&0x01;
		ni->rekey_done = 1;
	}
	else if(keyid != ni->ni_wkused)
	{
		IEEE80211_NOTE_MAC(ni->ni_vap, IEEE80211_MSG_CRYPTO,
			ni->ni_macaddr,"kid(0x%02x) is wrong, the ni_wkused is  %d\n", 
			keyid, ni->ni_wkused);
			return -1;
	}
	return 0;
}
/*zhaoyang modify end*/
/*zhaoyang1 transplant end*/
/*
 * Validate and strip privacy headers (and trailer) for a
 * received frame. The specified key should be correct but
 * is also verified.
 */
static int
wpi_sms4_decap(struct ieee80211_key *k, wbuf_t wbuf, int hdrlen, struct ieee80211_rx_status *rs)
{
    struct sms4_ctx *ctx = k->wk_private;
	struct ieee80211vap *vap = ctx->sms4c_vap;
	struct ieee80211_frame *wh = NULL;
    u_int8_t  *iv;
    int i, tid;
	u_int8_t ivp[IEEE80211_WPI_SMS4_IVLEN]={0,};
    int ismcast;
    struct ieee80211_mac_stats *mac_stats;
	/*zhaoyang1 transplant from 717*/
	/*zhaoyang modify for wpi error frame count CHINAMOBILE-116*/
	u_int8_t keyid = 0;
 	struct ieee80211_node *ni = NULL;
	struct ieee80211com *ic = vap->iv_ic;
	/*zhaoyang modify end*/
	/*zhaoyang1 transplant end*/
	/*
	 * Header should have extended IV and sequence number;
	 * verify the former and validate the latter.
	 */
	wh = (struct ieee80211_frame *)wbuf_header(wbuf);
    ismcast = IEEE80211_IS_MULTICAST(wh->i_addr1);
    mac_stats = ismcast? &vap->iv_multicast_stats : &vap->iv_unicast_stats;
	/*zhaoyang1 transplant from 717*/
	/*zhaoyang modify for wpi error frame count CHINAMOBILE-116*/
	if(!ismcast)
		ni = ieee80211_find_node(&ic->ic_sta,wh->i_addr2);
	/*zhaoyang modify end*/
	/*zhaoyang1 transplant end*/
    /* Get TID */
    tid = IEEE80211_NON_QOS_SEQ;
    if (IEEE80211_QOS_HAS_SEQ(wh)) {
        if ( (wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) == IEEE80211_FC1_DIR_DSTODS ) {
            tid = ((struct ieee80211_qosframe_addr4 *)wh)->i_qos[0] & IEEE80211_QOS_TID;
        } else {
            tid = ((struct ieee80211_qosframe *)wh)->i_qos[0] & IEEE80211_QOS_TID;
        }
    }

	/*zhaoyang1 transplant from 717*/
	/*zhaoyang modify for wpi error frame count CHINAMOBILE-116*/   	
	keyid = (wbuf_header(wbuf) + hdrlen)[0];
	if(ieee80211_check_kid(ni, keyid) != 0)
	{
		vap->iv_stats.is_wpi_no_key_error++;
		if(ni != NULL){
			ni->ni_stats.ns_wpi_no_key_error++;

			/*zhaoyang1 add start for ap reboot with wapi*/
			ieee80211_free_node(ni);
			/*zhaoyang1 add end*/
			
			//printk("ni no key error =%d\n",ni->ni_stats.ns_wpi_no_key_error);
		}
		return 0;
	}
	/*zhaoyang modify end*/
	/*zhaoyang1 transplant end*/

    /*
     * Copy up 802.11 header and strip crypto bits.
     */

    iv = wbuf_header(wbuf) + hdrlen + 2;/*header for wapi +2 = IV*/
	
	for(i=0; i<IEEE80211_WPI_SMS4_IVLEN; i++)
	{
		ivp[i] = (iv[IEEE80211_WPI_SMS4_IVLEN-1-i])&0xff;	
	}
	if(memcmp(k->wk_recviv, ivp, IEEE80211_WPI_SMS4_IVLEN) >= 0)
	{
		/*WPI replay*/
		IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO, 
                        wh->i_addr1, "%s", "WAPI IV replay attack");
		mac_stats->ims_rx_wpireplay++;
		/*zhaoyang1 transplant from 717*/
		/*zhaoyang modify for wpi error frame count CHINAMOBILE-116*/
		vap->iv_stats.is_re_wpi++;
		if(ni != NULL){
			ni->ni_stats.ns_re_wpi++;
			/*zhaoyang modify for replay trap CHINAMOBILE-119*/
			if(ni->ni_macaddr != NULL && vap->rd_trap)//zhaoyang modify for trap switch CHINAMOBILE-116
				wapi_callback_wai(ni->ni_vap, ni->ni_macaddr, NULL, WAPI_CHALLENGE_REPLAY_TRAP);
			/*zhaoyang modify end*/
			
			/*zhaoyang1 add start for ap reboot with wapi*/
			ieee80211_free_node(ni);
			/*zhaoyang1 add end*/
			
		}
		/*zhaoyang modify end*/
		/*zhaoyang1 transplant end*/
		return 0;
	} else {
	    if(vap->iv_opmode == IEEE80211_M_HOSTAP) {
            if((ivp[15] & 0x01 ) != 0) {
                IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO,
                    wh->i_addr1, "%s",  "AP WAPI IV is even");
				/*zhaoyang1 add start for ap reboot with wapi*/
				if (NULL != ni) {
					ieee80211_free_node(ni);
				}
				/*zhaoyang1 add end*/
                return 0;
            }
        }
        else {
            if(ismcast) {
                if(memcmp(k->wk_recviv, ivp, IEEE80211_WPI_SMS4_IVLEN) >= 0) {
                    /*WPI replay*/
                    IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO, 
                                    wh->i_addr1, "%s", "STA WAPI IV replay attack");
			mac_stats->ims_rx_wpireplay++;
			/*zhaoyang1 transplant from 717*/
					/*zhaoyang modify for wpi error frame count CHINAMOBILE-116*/
					vap->iv_stats.is_re_wpi++;
					if(ni != NULL){
						ni->ni_stats.ns_re_wpi++;
						/*zhaoyang modify for replay trap CHINAMOBILE-119*/
						if(ni->ni_macaddr != NULL && vap->rd_trap)//zhaoyang modify for trap switch CHINAMOBILE-116
							wapi_callback_wai(ni->ni_vap, ni->ni_macaddr, NULL, WAPI_CHALLENGE_REPLAY_TRAP);
						/*zhaoyang modify end*/
						
						/*zhaoyang1 add start for ap reboot with wapi*/
						ieee80211_free_node(ni);
						/*zhaoyang1 add end*/
			
					}
					/*zhaoyang modify end*/
			/*zhaoyang1 transplant end*/
                    return 0;
                }
            } else {
                if((ivp[15] & 0x01 ) == 0) {
                    IEEE80211_NOTE_MAC(vap, IEEE80211_MSG_CRYPTO,
                        wh->i_addr1, "%s",  "STA WAPI IV is odd");
					/*zhaoyang1 add start for ap reboot with wapi*/
					if (NULL != ni) {
						ieee80211_free_node(ni);
					}
					/*zhaoyang1 add end*/
                    return 0;
                }
            }
        }            
	} 
	/*zhaoyang1 transplant from 717*/
	/*zhaoyang modify for wpi error frame count CHINAMOBILE-116*/

	/*
	 * Check if the device handled the decrypt in hardware.
	 * If so we just strip the header; otherwise we need to
	 * handle the decrypt in software.  Note that for the
	 * latter we leave the header in place for use in the
	 * decryption work.
	 */
    if (rs->rs_flags & IEEE80211_RX_DECRYPT_ERROR) {
        /* 
         * Drop frames failed decryption in hardware
         * Prevent update stale iv after rekey
         */
        /*zhaoyang modify for add rd_trap switch*/
        mac_stats->ims_rx_wpimic++;
		vap->iv_stats.is_wpi_mic++;
		if(ni != NULL){
			ni->ni_stats.ns_wpi_mic++;
		}
        if(vap->rd_trap){
			/*zhaoyang modify for address redirection trap CHINAMOBILE-120*/
			wapi_callback_wai(vap,wh->i_addr2,NULL,WAPI_ADDR_RD_TRAP);
			/*zhaoyang modify end*/
        }

		/*zhaoyang1 add start for ap reboot with wapi*/
		if (NULL != ni) {
			ieee80211_free_node(ni);
		}
		/*zhaoyang1 add end*/

		return 0;
    }
	/*zhaoyang modify end*/
	/*zhaoyang1 transplant end*/
    memcpy(k->wk_recviv, ivp, IEEE80211_WPI_SMS4_IVLEN);

    memmove(wbuf_header(wbuf) + wpi_sms4.ic_header, wbuf_header(wbuf), hdrlen);
    wbuf_pull(wbuf, wpi_sms4.ic_header);
    while (wbuf_next(wbuf) != NULL)
        wbuf = wbuf_next(wbuf);
    wbuf_trim(wbuf, wpi_sms4.ic_trailer);
    /*
     * Ok to update rsc now.
     */
    k->wk_keyrsc[tid] ++;
	/*zhaoyang1 add start for ap reboot with wapi*/
	if (NULL != ni) {
		ieee80211_free_node(ni);
	}
	/*zhaoyang1 add end*/
    return 1;
}

/*
 * Verify and strip MIC from the frame.
 */
static int
wpi_sms4_demic(struct ieee80211_key *k, wbuf_t wbuf, int hdrlen, int force, struct ieee80211_rx_status * rs)
{
	return 1;
}

/*
 * Module attach
 */
void
ieee80211_crypto_register_sms4(struct ieee80211com *ic)
{
    ieee80211_crypto_register(ic, &wpi_sms4);
    return;
}

void
ieee80211_crypto_unregister_sms4(struct ieee80211com *ic)
{
    ieee80211_crypto_unregister(ic, &wpi_sms4);
    return;
}

#endif /*ATH_SUPPORT_WAPI*/
