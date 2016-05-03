
#ifndef _IEEE80211_TRAFFIC_LIMIT_H_
#define _IEEE80211_TRAFFIC_LIMIT_H_

/*Begin: added by wangjia, For traffic limit. 2012-10-25 */


#define IEEE80211_TL_ENQUEUE_OK                1
#define IEEE80211_TL_ENQUEUE_IC_IS_NULL       -1
#define IEEE80211_TL_ENQUEUE_IS_FULL          -2
#define IEEE80211_TL_ENQUEUE_PACKET_LEN       -3
#define IEEE80211_TL_ENQUEUE_CACHE_IS_DISABLE -4

#define IEEE80211_TL_ENABLE                    1
#define IEEE80211_TL_DISABLE                   0

#define IEEE80211_TL_COLOR_MARK_GREEN          1
#define IEEE80211_TL_COLOR_MARK_YELLOW         2
#define IEEE80211_TL_COLOR_MARK_RED            3
#define IEEE80211_TL_COLOR_MARK_INVALID        4

#define IEEE80211_TL_CACHE_ENABLE              1
#define IEEE80211_TL_CACHE_DISABLE             0

#define IEEE80211_TL_CACHEQ_DEPTH_LOW          0
#define IEEE80211_TL_CACHEQ_DEPTH_HIGH         1

#define IEEE80211_TL_CACHEQ_MAX_LEN          500

#define IEEE80211_TL_DEQUEUE_WEIGHTED         10

// Currently, we enqueue ALL wbufs.
#define IEEE80211_TL_CACHE_MIN_SIZE            0
#define IEEE80211_TL_CACHE_MAX_SIZE         2000

#define IEEE80211_TL_LOG_INFO               0x01
#define IEEE80211_TL_LOG_WARNING            0x02
#define IEEE80211_TL_LOG_ERROR              0x04

/**
 *  Properties used by Single Rate Three Color Marker algorithm.
 *  When CIR was set(updated), CBS and EBS should be recalculated.
 */
struct ieee80211_tl_srtcm
{
    spinlock_t sr_lock;                 /* spin lock */

    u_int64_t sr_cbs;                   /* Committed Burst Size */
	u_int64_t sr_ebs;                   /* Excess Burst Size */
	u_int64_t sr_cir;                   /* Committed Information Rate, Bps */
	
	u_int64_t sr_current_tc_num;        /* Current token number, 1 token for 1 byte to send */
	u_int64_t sr_current_te_num;

    u_int32_t sr_green_packet_count;    /* Used for statistics */
	u_int32_t sr_yellow_packet_count;
	u_int32_t sr_red_packet_count;

    struct timeval sr_timeval;          /* Last srtcm mark time */
};

struct ieee80211_tl_cache_queue_tx
{
	spinlock_t  cq_lock;
    u_int32_t   cq_bytes;               /* number of queued frames */
    u_int32_t   cq_len;                 /* number of queued frames */
	u_int32_t   cq_pre_depth;           /* the buffer queue depth */

	u_int32_t   cq_enqueue_count;       /* used for statistics */
	u_int32_t   cq_drop_count;

	u_int32_t   cq_max_len;             /* the buffer packet max number */
    u_int32_t   cq_weighted;            /* dequeue weight for one time */
	u_int32_t   cq_threshold_low;
	u_int32_t   cq_threshold_high;

    /* For downlink, just cache wbuf */
    wbuf_t      cq_whead;               /* pointer of queued head */
	wbuf_t      cq_wtail;               /* pointer of queued tail */
};

/**
 *  For uplink, we cached the wbuf, 
 *  and corresponding rx_status, subtype and dir.
 */
struct ieee80211_tl_cache_rx_extra
{
    int    ex_dir;
    int    ex_subtype;
    struct ieee80211_rx_status ex_rx_status;
    struct ieee80211_tl_cache_rx_extra *ex_next;
};

struct ieee80211_tl_cache_queue_rx
{
	spinlock_t  cq_lock;
    u_int32_t   cq_bytes;               /* number of queued frames */
    u_int32_t   cq_len;                 /* number of queued frames */
	u_int32_t   cq_pre_depth;           /* the buffer queue depth */

	u_int32_t   cq_enqueue_count;       /* used for statistics */
	u_int32_t   cq_drop_count;

	u_int32_t   cq_max_len;             /* the buffer packet max number */
    u_int32_t   cq_weighted;            /* dequeue weight for one time */
	u_int32_t   cq_threshold_low;
	u_int32_t   cq_threshold_high;

    wbuf_t      cq_whead;               /* pointer of queued head */
	wbuf_t      cq_wtail;               /* pointer of queued tail */
    
    struct ieee80211_tl_cache_rx_extra *cq_rx_head;
    struct ieee80211_tl_cache_rx_extra *cq_rx_tail;
    struct ieee80211_tl_cache_rx_extra  cq_rx_extra[IEEE80211_TL_CACHEQ_MAX_LEN];
};

#define IEEE80211_TL_RATE_TO_CIR(_rate) ((_rate * 1024) / 8)
#define IEEE80211_TL_CIR_TO_RATE(_cir)  ((_cir * 8) / 1024)

#define IEEE80211_TL_SRTCM_LOCK(_tl_srtcm)      spin_lock(&(_tl_srtcm)->sr_lock)
#define IEEE80211_TL_SRTCM_UNLOCK(_tl_srtcm)    spin_unlock(&(_tl_srtcm)->sr_lock)
#define IEEE80211_TL_SRTCM_DESTORY(_tl_srtcm)   spin_lock_destroy(&(_tl_srtcm)->sr_lock)


#define IEEE80211_TL_SRTCM_INIT(_srtcm) do {             \
        spin_lock_init(&((_srtcm)->sr_lock));            \
        IEEE80211_TL_SRTCM_LOCK(_srtcm);                 \
        (_srtcm)->sr_cir = 0;                            \
        (_srtcm)->sr_cbs = 0;                            \
        (_srtcm)->sr_ebs = 0;                            \
        (_srtcm)->sr_current_tc_num = 0;                 \
        (_srtcm)->sr_current_te_num = 0;                 \
        (_srtcm)->sr_green_packet_count = 0;             \
        (_srtcm)->sr_green_packet_count = 0;             \
        (_srtcm)->sr_red_packet_count   = 0;             \
        (_srtcm)->sr_timeval.tv_usec    = 0;             \
        IEEE80211_TL_SRTCM_UNLOCK(_srtcm);               \
    } while(0)   


#define IEEE80211_TL_SRTCM_INIT_FROM(_d_srtcm, _s_srtcm) do { \
        spin_lock_init(&((_d_srtcm)->sr_lock));            \
        IEEE80211_TL_SRTCM_LOCK(_s_srtcm);                 \
        IEEE80211_TL_SRTCM_LOCK(_d_srtcm);                 \
        (_d_srtcm)->sr_cir = (_s_srtcm)->sr_cir;           \
        (_d_srtcm)->sr_cbs = (_s_srtcm)->sr_cbs;           \
        (_d_srtcm)->sr_ebs = (_s_srtcm)->sr_ebs;           \
        (_d_srtcm)->sr_current_tc_num = (_s_srtcm)->sr_current_tc_num;  \
        (_d_srtcm)->sr_current_te_num = (_s_srtcm)->sr_current_te_num;  \
        (_d_srtcm)->sr_green_packet_count = 0;             \
        (_d_srtcm)->sr_green_packet_count = 0;             \
        (_d_srtcm)->sr_red_packet_count   = 0;             \
        (_d_srtcm)->sr_timeval.tv_usec    = 0;             \
        IEEE80211_TL_SRTCM_UNLOCK(_s_srtcm);               \
        IEEE80211_TL_SRTCM_UNLOCK(_d_srtcm);               \
    } while(0)

/**
 *  when cir is changed, others is updated at the same time.
 *  convert cir form kbps to Bps.
 */
#define IEEE80211_TL_SRTCM_SET_CIR(_srtcm, _rate) do {   \
        IEEE80211_TL_SRTCM_LOCK(_srtcm);                 \
        (_srtcm)->sr_cir = IEEE80211_TL_RATE_TO_CIR(_rate); \
        (_srtcm)->sr_cbs = (_srtcm)->sr_cir;             \
        (_srtcm)->sr_ebs = (_srtcm)->sr_cbs * 2;         \
        (_srtcm)->sr_current_tc_num = (_srtcm)->sr_cbs;  \
        (_srtcm)->sr_current_te_num = (_srtcm)->sr_ebs;  \
        IEEE80211_TL_SRTCM_UNLOCK(_srtcm);               \
    } while(0)
    

/* init with default value */
#define IEEE80211_TL_CACHEQ_INIT_TX(_ncq) do {                  \
        spin_lock_init(&((_ncq)->cq_lock));                     \
        (_ncq)->cq_bytes = 0;                                   \
        (_ncq)->cq_len = 0;                                     \
        (_ncq)->cq_pre_depth = 0;                               \
        (_ncq)->cq_enqueue_count = 0;                           \
        (_ncq)->cq_drop_count = 0;                              \
        (_ncq)->cq_max_len = IEEE80211_TL_CACHEQ_MAX_LEN;       \
        (_ncq)->cq_weighted = IEEE80211_TL_DEQUEUE_WEIGHTED;    \
        (_ncq)->cq_threshold_low = (_ncq)->cq_max_len/3;        \
        (_ncq)->cq_threshold_high = (_ncq)->cq_threshold_low*2; \
        (_ncq)->cq_whead = NULL;                                \
        (_ncq)->cq_wtail = NULL;                                \
    } while (0)

#define IEEE80211_TL_CACHEQ_INIT_RX(_ncq) do {                  \
        int index = 0;                                          \
        IEEE80211_TL_CACHEQ_INIT_TX(_ncq);                      \
        (_ncq)->cq_rx_head = NULL;                              \
        (_ncq)->cq_rx_tail = NULL;                              \
        OS_MEMZERO((_ncq)->cq_rx_extra, sizeof((_ncq)->cq_rx_extra));  \
        (_ncq)->cq_rx_head = (_ncq)->cq_rx_tail = (_ncq)->cq_rx_extra; \
        while(index < (IEEE80211_TL_CACHEQ_MAX_LEN - 1))        \
        {                                                       \
            (_ncq)->cq_rx_extra[index].ex_next = &((_ncq)->cq_rx_extra[index+1]); \
            index++; \
        } \
        (_ncq)->cq_rx_extra[index].ex_next = &((_ncq)->cq_rx_extra[0]); \
    } while(0)
    
#define IEEE80211_TL_CACHEQ_LOCK(_tl_cq)      spin_lock(&(_tl_cq)->cq_lock)
#define IEEE80211_TL_CACHEQ_UNLOCK(_tl_cq)    spin_unlock(&(_tl_cq)->cq_lock)
#define IEEE80211_TL_CACHEQ_FULL(_tl_cq)      ((_tl_cq)->cq_len >= (_tl_cq)->cq_max_len)
#define IEEE80211_TL_CACHEQ_POLL(_tl_cq, _w)  ((_w) = (_tl_cq)->cq_whead)

/* add wbuf to the cache queue */
#define IEEE80211_TL_CACHEQ_ENQUEUE(_tl_cq, _w) do {          \
        wbuf_set_next((_w), NULL);                            \
        if ((_tl_cq)->cq_wtail != NULL) {                     \
            wbuf_set_next((_tl_cq)->cq_wtail, (_w));          \
            (_tl_cq)->cq_wtail = (_w);                        \
        } else {                                              \
            (_tl_cq)->cq_whead = (_tl_cq)->cq_wtail = (_w);   \
        }                                                     \
        (_tl_cq)->cq_len++;                                   \
        (_tl_cq)->cq_bytes += (_w)->len;                      \
    } while (0)   

#define IEEE80211_TL_CACHEQ_DEQUEUE(_tl_cq, _w) do {      \
        (_w) = (_tl_cq)->cq_whead;                        \
        if(_w) {                                          \
            (_tl_cq)->cq_whead = wbuf_next(_w);           \
            wbuf_set_next(_w, NULL);                      \
            if ((_tl_cq)->cq_whead == NULL) {             \
                (_tl_cq)->cq_wtail = NULL;                \
            }                                             \
            --(_tl_cq)->cq_len;                           \
            (_tl_cq)->cq_bytes -= (_w)->len;              \
        }                                                 \
    } while (0)

/* for rx, enqueue rx_status/subtype/dir. Used only in IEEE80211_TL_RX_ENQUEUE */
#define IEEE80211_TL_ENQUEUE_RX_EXTRA(_tl_cq, _rs, _subtype, _dir) do {  \
        (_tl_cq)->cq_rx_tail->ex_dir = _dir;                   \
        (_tl_cq)->cq_rx_tail->ex_subtype = _subtype;           \
        OS_MEMCPY(&((_tl_cq)->cq_rx_tail->ex_rx_status), _rs,  \
                  sizeof(struct ieee80211_rx_status));         \
        (_tl_cq)->cq_rx_tail = (_tl_cq)->cq_rx_tail->ex_next;  \
    } while (0) 


/* for rx, dequeue rx_status/subtype/dir. Used only in IEEE80211_TL_RX_DEQUEUE */    
#define IEEE80211_TL_DEQUEUE_RX_EXTRA(_tl_cq, _rs, _subtype, _dir) do {  \
        _rs = &((_tl_cq)->cq_rx_head->ex_rx_status);          \
        _subtype = (_tl_cq)->cq_rx_head->ex_subtype;          \
        _dir     = (_tl_cq)->cq_rx_head->ex_dir;              \
        (_tl_cq)->cq_rx_head = (_tl_cq)->cq_rx_head->ex_next; \
    } while (0)

        
#define IEEE80211_TL_RX_ENQUEUE(_tl_cq, _w, _rs, _subtype, _dir) do { \
        IEEE80211_TL_CACHEQ_ENQUEUE(_tl_cq, _w);                      \
        IEEE80211_TL_ENQUEUE_RX_EXTRA(_tl_cq, _rs, _subtype, _dir);   \
    } while (0)

#define IEEE80211_TL_RX_DEQUEUE(_tl_cq, _w, _rs, _subtype, _dir) do { \
        IEEE80211_TL_CACHEQ_DEQUEUE(_tl_cq, _w);                      \
        IEEE80211_TL_DEQUEUE_RX_EXTRA(_tl_cq, _rs, _subtype, _dir);   \
    } while (0)

#define IEEE80211_TL_CACHEQ_DESTROY(_ncq) do {                  \
        spin_lock_destroy(&(_ncq)->cq_lock);                    \
        (_ncq)->cq_len = 0;                                     \
        KASSERT(((_ncq)->cq_whead == NULL), ("node cache queue is not empty")); \
    } while (0)


#define traffic_limit_debug_print(level, message, rest...) do {   \
        if(tl_debug_flag & level) {                         \
            printk(message, ## rest);                             \
        }                                                         \
    } while (0)


void ieee80211_tl_cacheq_detach(struct ieee80211_node *ni);
void ieee80211_tl_node_cacheq_drain(struct ieee80211_node *ni);

void ieee80211_tl_vap_init(struct ieee80211vap *vap);
void ieee80211_tl_node_init(struct ieee80211vap *vap, struct ieee80211_node *ni);

int ieee80211_tl_srtcm_meter(struct ieee80211_tl_srtcm *srtcm, wbuf_t wbuf);

int ieee80211_tl_vap_cache_enqueue_tx(struct ieee80211vap *vap, 
    wbuf_t wbuf, struct ieee80211com *ic);
void ieee80211_tl_vap_dequeue_cache_tx(struct ieee80211vap *vap);

int ieee80211_tl_vap_cache_enqueue_rx(struct ieee80211vap *vap, wbuf_t wbuf, 
    struct ieee80211_rx_status *rs, int subtype, int dir, struct ieee80211com *ic);

void ieee80211_tl_vap_dequeue_cache_rx(struct ieee80211vap *vap);

int ieee80211_tl_node_cache_enqueue_tx(struct ieee80211_node *ni, wbuf_t wbuf, 
    struct ieee80211com *ic);
void ieee80211_tl_node_dequeue_cache_tx(struct ieee80211_node *ni);

int ieee80211_tl_node_cache_enqueue_rx(struct ieee80211_node *ni, wbuf_t wbuf, 
    struct ieee80211_rx_status *rs, int subtype, int dir, struct ieee80211com *ic);
void ieee80211_tl_node_dequeue_cache_rx(struct ieee80211_node *ni);

void ieee80211_tl_cleanup_vap_cache_of_node(struct ieee80211_node *ni);

void ieee80211_tl_dequeue_cache_tx_tasklet_fn(struct ieee80211com *ic);
void ieee80211_tl_dequeue_cache_rx_tasklet_fn(struct ieee80211com *ic);

void ieee80211_tl_dequeue_cache_timer_fn(void);


#endif
