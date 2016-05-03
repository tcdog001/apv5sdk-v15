//add begin for autelan wmm, moved from aquila//lhj//2012.06

#ifndef AP8X_LNX_WMM_TRAFFIC_H_
#define AP8X_LNX_WMM_TRAFFIC_H_
#include    <linux/module.h>

struct ieee80211_autelan_wmm_params {

#define TOS_FLAG    1
#define GET_TOS_FLAG    2
#define TOS_INGRESS     3
#define GET_TOS_INGRESS 4
#define TOS_EGRESS  5
#define GET_TOS_EGRESS  6
#define VLAN_FLAG   7
#define GET_VLAN_FLAG   8
#define VLAN_INGRESS    9
#define GET_VLAN_INGRESS    10
#define VLAN_EGRESS     11
#define GET_VLAN_EGRESS     12
#define BE_FLAG     13
#define GET_BE_FLAG     14

    UINT8   type;           /* request type*/
    UINT32  arg1;
    UINT32  arg2;
    UINT32  index; 
};

#define IEEE80211_ADDR_LEN      6
struct ieee80211_autelan_traffic_limit {

#define SET_VAP_TRAFFIC_LIMIT   1
#define GET_VAP_TRAFFIC_LIMIT   2
#define SET_NODE_TRAFFIC_LIMIT  3
#define GET_NODE_TRAFFIC_LIMIT  4
#define SET_EVERY_NODE_TRAFFIC_LIMIT    5
#define GET_EVERY_NODE_TRAFFIC_LIMIT    6
#define SET_VAP_TRAFFIC_LIMIT_FLAG  7
#define GET_VAP_TRAFFIC_LIMIT_FLAG  8
#define SET_EVERY_NODE_TRAFFIC_LIMIT_FLAG   9
#define GET_EVERY_NODE_TRAFFIC_LIMIT_FLAG   10
#define SET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG    11
#define GET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG    12

/*ljy--add begin to separate traffic limit between rx and tx*/
#define SET_VAP_TRAFFIC_LIMIT_SEND  13
#define GET_VAP_TRAFFIC_LIMIT_SEND  14
#define SET_NODE_TRAFFIC_LIMIT_SEND 15
#define GET_NODE_TRAFFIC_LIMIT_SEND 16
#define SET_EVERY_NODE_TRAFFIC_LIMIT_SEND   17
#define GET_EVERY_NODE_TRAFFIC_LIMIT_SEND   18
/*ljy--add end*/

    unsigned char   type;           /* request type*/
    unsigned int    arg1;
    u_int8_t macaddr[IEEE80211_ADDR_LEN];
};

int wmm_to_tos_map(int wmm_priority, struct ieee80211vap *vap) ;
int wmm_to_vlan_map(int wmm_priority, struct ieee80211vap *vap) ;
int tos_to_wmm_map (int vlan_priority, struct ieee80211vap *vap);
int vlan_to_wmm_map (int vlan_priority, struct ieee80211vap *vap);
int ieee80211_ioctl_autelan_wmm(struct net_device *netdev, char *param_str, char *ret_str);
void autelan_wmm_init(struct ieee80211vap *vap) ;
int ieee80211_ioctl_autelan_traffic_limit(struct net_device *dev, char *param_str, char *ret_str);
int vap_setup_timer(struct ieee80211vap *vap);

#ifdef AE5200_RT_SUPPORT


#define IEEE80211_QOS_TID               0x0f

int ieee_autelan_qos_decap(struct sk_buff **skb, struct net_device *dev, UINT16 frametype);

#endif
#endif
//add end for autelan wmm, moved from aquila//lhj//2012.06
