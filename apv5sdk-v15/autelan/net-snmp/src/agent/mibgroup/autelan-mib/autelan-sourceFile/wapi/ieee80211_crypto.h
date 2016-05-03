/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: ieee80211_crypto.h
* description:  implementation for wireless protocol crypto-related definitions.
* 
*
* 
************************************************************************************/
#ifndef _NET80211_IEEE80211_CRYPTO_H_
#define _NET80211_IEEE80211_CRYPTO_H_

/*ljy--add begin for wapi*/
#include "ieee80211_crypto_wpi_sms4.h"
/*ljy--add end*/

/*
 * 802.11 protocol crypto-related definitions.
 */
#define	IEEE80211_KEYBUF_SIZE	16
#define	IEEE80211_MICBUF_SIZE	(8 + 8)		/* space for both TX & RX keys */
#define IEEE80211_TID_SIZE	17		/* total number of TIDs */

/*
 * Old WEP-style key.  Deprecated.
 */
struct ieee80211_wepkey {
	u_int wk_len;				/* key length in bytes */
	u_int8_t wk_key[IEEE80211_KEYBUF_SIZE];
};

struct ieee80211_cipher;

/*
 * Crypto key state.  There is sufficient room for all supported
 * ciphers (see below).  The underlying ciphers are handled
 * separately through loadable cipher modules that register with
 * the generic crypto support.  A key has a reference to an instance
 * of the cipher; any per-key state is hung off wk_private by the
 * cipher when it is attached.  Ciphers are automatically called
 * to detach and cleanup any such state when the key is deleted.
 *
 * The generic crypto support handles encap/decap of cipher-related
 * frame contents for both hardware- and software-based implementations.
 * A key requiring software crypto support is automatically flagged and
 * the cipher is expected to honor this and do the necessary work.
 * Ciphers such as TKIP may also support mixed hardware/software
 * encrypt/decrypt and MIC processing.
 */

typedef u_int16_t ieee80211_keyix_t;

/* XXX pack better? */
/* XXX 48-bit rsc/tsc */
struct ieee80211_key {
	u_int8_t wk_keylen;		/* key length in bytes */
	u_int8_t wk_flags;
#define	IEEE80211_KEY_XMIT	0x01	/* key used for xmit */
#define	IEEE80211_KEY_RECV	0x02	/* key used for recv */
#define	IEEE80211_KEY_GROUP	0x04	/* key used for WPA group operation */
#define	IEEE80211_KEY_SWCRYPT	0x10	/* host-based encrypt/decrypt */
#define	IEEE80211_KEY_SWMIC	0x20	/* host-based enmic/demic */
	ieee80211_keyix_t wk_keyix;	/* key index */
	u_int8_t wk_key[IEEE80211_KEYBUF_SIZE+IEEE80211_MICBUF_SIZE];
#define	wk_txmic	wk_key+IEEE80211_KEYBUF_SIZE+0	/* XXX can't () right */
#define	wk_rxmic	wk_key+IEEE80211_KEYBUF_SIZE+8	/* XXX can't () right */
	u_int64_t wk_keyrsc[IEEE80211_TID_SIZE];	/* key receive sequence counter */
	u_int64_t wk_keytsc;		/* key transmit sequence counter */
	
	/*ljy--add begin for wapi*/
	u_int8_t wk_recviv[16];
	u_int32_t wk_txiv[4];
	u_int32_t wk_rx;		/* key receive cipher packets counter*/
	u_int32_t wk_tx;		/* key transmit muticask cipher packets counter*/
	/*ljy--add end*/
	
	const struct ieee80211_cipher *wk_cipher;
	void *wk_private;		/* private cipher state */
};
#define	IEEE80211_KEY_COMMON 		/* common flags passed in by apps */\
	(IEEE80211_KEY_XMIT | IEEE80211_KEY_RECV | IEEE80211_KEY_GROUP)

/*
 * NB: these values are ordered carefully; there are lots of
 * of implications in any reordering.  In particular beware
 * that 4 is not used to avoid conflicting with IEEE80211_F_PRIVACY.
 */
#define	IEEE80211_CIPHER_WEP		0
#define	IEEE80211_CIPHER_TKIP		1
#define	IEEE80211_CIPHER_AES_OCB	2
#define	IEEE80211_CIPHER_AES_CCM	3
/*ljy--add begin for wapi*/
#define	IEEE80211_CIPHER_WPI_SMS4	4
/*ljy--add end*/
#define	IEEE80211_CIPHER_CKIP		5
#define	IEEE80211_CIPHER_NONE		6	/* pseudo value */

#define	IEEE80211_CIPHER_MAX		(IEEE80211_CIPHER_NONE + 1)

#define	IEEE80211_KEYIX_NONE	((ieee80211_keyix_t)-1)

#if defined(__KERNEL__) || defined(_KERNEL)

struct ieee80211com;
struct ieee80211vap;
struct ieee80211_node;
struct sk_buff;

void ieee80211_crypto_attach(struct ieee80211com *);
void ieee80211_crypto_detach(struct ieee80211com *);
void ieee80211_crypto_vattach(struct ieee80211vap *);
void ieee80211_crypto_vdetach(struct ieee80211vap *);
int ieee80211_crypto_newkey(struct ieee80211vap *, int, int,
	struct ieee80211_key *);
int ieee80211_crypto_delkey(struct ieee80211vap *, struct ieee80211_key *,
	struct ieee80211_node *);
int ieee80211_crypto_setkey(struct ieee80211vap *, struct ieee80211_key *,
	const u_int8_t macaddr[IEEE80211_ADDR_LEN], struct ieee80211_node *);
void ieee80211_crypto_delglobalkeys(struct ieee80211vap *);

/*
 * Template for a supported cipher.  Ciphers register with the
 * crypto code and are typically loaded as separate modules
 * (the null cipher is always present).
 * XXX may need refcnts
 */
struct ieee80211_cipher {
	const char *ic_name;		/* printable name */
	u_int ic_cipher;		/* IEEE80211_CIPHER_* */
	u_int ic_header;		/* size of privacy header (bytes) */
	u_int ic_trailer;		/* size of privacy trailer (bytes) */
	u_int ic_miclen;		/* size of mic trailer (bytes) */
	void *(*ic_attach)(struct ieee80211vap *, struct ieee80211_key *);
	void (*ic_detach)(struct ieee80211_key *);
	int (*ic_setkey)(struct ieee80211_key *);
	int (*ic_encap)(struct ieee80211_key *, struct sk_buff *, u_int8_t);
	int (*ic_decap)(struct ieee80211_key *, struct sk_buff *, int);
	int (*ic_enmic)(struct ieee80211_key *, struct sk_buff *, int);
	int (*ic_demic)(struct ieee80211_key *, struct sk_buff *, int, int);
};
extern const struct ieee80211_cipher ieee80211_cipher_none;

void ieee80211_crypto_register(const struct ieee80211_cipher *);
void ieee80211_crypto_unregister(const struct ieee80211_cipher *);
int ieee80211_crypto_available(struct ieee80211vap*, u_int);

struct ieee80211_key *ieee80211_crypto_encap(struct ieee80211_node *,
	struct sk_buff *);
struct ieee80211_key *ieee80211_crypto_decap(struct ieee80211_node *,
	struct sk_buff *, int);
/*ljy--add begin for wapi*/
struct ieee80211_key *ieee80211_wpi_sms4_crypto_encap(struct ieee80211_node *,
		struct sk_buff *);
struct ieee80211_key *ieee80211_wpi_sms4_crypto_decap(struct ieee80211_node *,
		struct sk_buff *, int);
/*ljy--add end*/

/*
 * Check and remove any MIC.
 */
static __inline int
ieee80211_crypto_demic(struct ieee80211vap *vap, struct ieee80211_key *k,
	struct sk_buff *skb, int hdrlen, int force)
{
	const struct ieee80211_cipher *cip = k->wk_cipher;
	return (cip->ic_miclen > 0 ? cip->ic_demic(k, skb, hdrlen, force) : 1);
}

/*
 * Add any MIC.
 */
static __inline int
ieee80211_crypto_enmic(struct ieee80211vap *vap, struct ieee80211_key *k,
	struct sk_buff *skb, int force)
{
	const struct ieee80211_cipher *cip = k->wk_cipher;
	return (cip->ic_miclen > 0 ? cip->ic_enmic(k, skb, force) : 1);
}

/* 
 * Reset key state to an unused state.  The crypto
 * key allocation mechanism ensures other state (e.g.
 * key data) is properly setup before a key is used.
 */
static __inline void
ieee80211_crypto_resetkey(struct ieee80211vap *vap, struct ieee80211_key *k,
	ieee80211_keyix_t ix)
{
	k->wk_cipher = &ieee80211_cipher_none;;
	k->wk_private = k->wk_cipher->ic_attach(vap, k);
	k->wk_keyix = ix;
	k->wk_flags = IEEE80211_KEY_XMIT | IEEE80211_KEY_RECV;
}

/*
 * Crypto-related notification methods.
 */
void ieee80211_notify_replay_failure(struct ieee80211vap *,
	const struct ieee80211_frame *, const struct ieee80211_key *,
	u_int64_t rsc);
void ieee80211_notify_michael_failure(struct ieee80211vap *,
	const struct ieee80211_frame *, ieee80211_keyix_t keyix);
#endif /* defined(__KERNEL__) || defined(_KERNEL) */
#endif /* _NET80211_IEEE80211_CRYPTO_H_ */
