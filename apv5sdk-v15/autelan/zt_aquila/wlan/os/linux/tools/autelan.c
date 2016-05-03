/* ****************************************************************************************************
 * Filename: autelan.c
 *	Description: autelan private command for ap.
 * Project: autelan ap 2010
 * Author: xmeng
 * Date: 11/25/2008
 *****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/if.h>
#include <linux/wireless.h>
#include <netinet/ether.h>

#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define autelan_debug printf

#define	IEEE80211_IOCTL_AUTELAN_WMM		(SIOCDEVPRIVATE+17) /*autelan private wmm configure*/
#define	IEEE80211_IOCTL_TRAFFIC_LIMIT	(SIOCDEVPRIVATE+18) /*autelan private traffic limit*/
#define	IEEE80211_IOCTL_QUICK_ROAMING	(SIOCDEVPRIVATE+19) /*autelan private wds roaming*/
#define	IEEE80211_IOCTL_MAC_BINDING		(SIOCDEVPRIVATE+20)	/*ljy--add to develop the policy of mac binding*/
#define	IEEE80211_IOCTL_80211_CACHE		(SIOCDEVPRIVATE+21)	/*Added by duanmingzhe for 80211 cache*/
#define IEEE80211_ADDR_LEN 6


/*xmeng add for autelan private wmm configure*/
struct ieee80211_autelan_wmm_params {

#define TOS_FLAG 	1
#define GET_TOS_FLAG	2
#define TOS_INGRESS 	3
#define GET_TOS_INGRESS	4
#define TOS_EGRESS	5
#define GET_TOS_EGRESS	6
#define VLAN_FLAG	7
#define GET_VLAN_FLAG	8
#define VLAN_INGRESS 	9
#define GET_VLAN_INGRESS	10
#define VLAN_EGRESS		11
#define GET_VLAN_EGRESS		12

/*ljy--add begin to append the switch modified the best effort parameter*/
#define BE_FLAG		13
#define GET_BE_FLAG		14
/*ljy--add end*/

	unsigned char   type;  			/* request type*/
	unsigned int 	arg1;
	unsigned int 	arg2;
	unsigned int 	index; 
};
/*xmeng add for autelan private traffic limit*/

struct ieee80211_autelan_traffic_limit {

#define SET_VAP_TRAFFIC_LIMIT	1
#define GET_VAP_TRAFFIC_LIMIT	2
#define SET_NODE_TRAFFIC_LIMIT	3
#define GET_NODE_TRAFFIC_LIMIT	4
#define SET_EVERY_NODE_TRAFFIC_LIMIT	5
#define GET_EVERY_NODE_TRAFFIC_LIMIT	6
#define SET_VAP_TRAFFIC_LIMIT_FLAG	7
#define GET_VAP_TRAFFIC_LIMIT_FLAG	8
#define SET_EVERY_NODE_TRAFFIC_LIMIT_FLAG	9
#define GET_EVERY_NODE_TRAFFIC_LIMIT_FLAG	10
#define SET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG	11
#define GET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG	12

/*ljy--add begin to separate traffic limit between rx and tx*/
#define SET_VAP_TRAFFIC_LIMIT_SEND	13
#define GET_VAP_TRAFFIC_LIMIT_SEND	14
#define SET_NODE_TRAFFIC_LIMIT_SEND	15
#define GET_NODE_TRAFFIC_LIMIT_SEND	16
#define SET_EVERY_NODE_TRAFFIC_LIMIT_SEND	17
#define GET_EVERY_NODE_TRAFFIC_LIMIT_SEND	18
/*ljy--add end*/

	unsigned char   type;  			/* request type*/
	unsigned int 	arg1;
	u_int8_t macaddr[IEEE80211_ADDR_LEN];
};

struct ieee80211_autelan_quick_roaming {

#define SET_FLAG	1
#define GET_FLAG	2
#define ADD_NODE	3
#define DEL_NODE	4	
	unsigned char   type;  			/* request type*/
	unsigned int 	arg1;
};

/*ljy--add begin to develop the policy of mac binding*/
struct ieee80211_autelan_mac_binding {

#define SET_DHCP_ENABLE			1
#define GET_DHCP_ENABLE			2
#define SET_IP_AUTO_LEARNING	3
#define GET_IP_AUTO_LEARNING	4
#define SET_IP					5
#define GET_IP					6
#define SET_VLAN				7
#define GET_VLAN				8
//sheyanbin--add to through the pppoe frame
#define SET_PPPOE_ENABLE		9
#define GET_PPPOE_ENABLE		10
//sheyanbin--add end


	unsigned char   type;  			/* request type*/
	unsigned int 	arg1;
	u_int8_t macaddr[IEEE80211_ADDR_LEN];
	u_int8_t ipaddr[4];
};
/*ljy--add end*/

/*Begin:added by duanmingzhe for 80211 cache*/
struct ieee80211_autelan_80211_cache{
#define SET_ENABLE               1
#define GET_ENABLE               2
#define SET_CACHE_ENABLE         3
#define GET_CACHE_ENABLE         4
#define SET_CACHE_LIMIT          5
#define GET_CACHE_LIMIT 	     6
#define SET_CACHE_THRESHOLD_B    7
#define GET_CACHE_THRESHOLD_B    8
#define SET_CACHE_THRESHOLD_G    9
#define GET_CACHE_THRESHOLD_G    10
#define SET_CACHE_THRESHOLD_N    11
#define GET_CACHE_THRESHOLD_N    12
#define SET_CACHE_PRIORITY_B     13
#define GET_CACHE_PRIORITY_B     14
#define SET_CACHE_PRIORITY_G     15
#define GET_CACHE_PRIORITY_G     16
#define SET_CACHE_PRIORITY_N     17
#define GET_CACHE_PRIORITY_N     18
#define SET_CACHE_QUEUE_MAXLEN_B 19
#define GET_CACHE_QUEUE_MAXLEN_B 20
#define SET_CACHE_QUEUE_MAXLEN_G 21
#define GET_CACHE_QUEUE_MAXLEN_G 22
#define SET_CACHE_QUEUE_MAXLEN_N 23
#define GET_CACHE_QUEUE_MAXLEN_N 24
#define GET_CACHE_SEND_COUNT     25


    unsigned char   type;  			/* request type*/
    unsigned int    arg1;
};
/*End:added by duanmingzhe for 80211 cache*/
/*
 *autelan wmm help information
 */
static void autelan_wmm_help () 
{
	printf("\nusage:: autelan wmm COMMAND [OPTION] ... \n");
	printf("OPTIONS: \n");
	printf("\ttos_flag\t\t[interface]\t[0|1]\n");
	printf("\tget_tos_flag\t\t[interface]\t\n");
	printf("\ttos_egress_map\t\t[interface]\t[index]\t[wmm-priority]\t[tos-qos]\n");
	printf("\tget_tos_egress_map\t[interface]\t\n");
	printf("\ttos_ingress_map\t\t[interface]\t[wmm-priority]\t[tos-qos]\n");
	printf("\tget_tos_ingress_map\t[interface]\t\n");
	printf("\t1p_flag\t\t\t[interface]\t[0|1]\n");
	printf("\tget_1p_flag\t\t[interface]\n");
	printf("\t1p_egress_map\t\t[interface]\t[wmm-priority]\t[vlan-qos]\n");
	printf("\tget_1p_egress_map\t[interface]\n");
	printf("\t1p_ingress_map\t\t[interface]\t[wmm-priority]\t[vlan-qos]\n");
	printf("\tget_1p_ingress_map\t[interface]\n");
	/*ljy--add begin to append the switch modified the best effort parameter*/
	printf("\tbe_flag\t\t\t[interface]\t[0|1]\n");
	printf("\tget_be_flag\t\t[interface]\t\n");
	/*ljy--add end*/
	printf("\n");
	//return 0;
}

static int autelan_ioctl(struct iwreq *iwr, int cmd) 
{
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		printf("1 function autelan_ioctl::socket error ; %s\n", strerror(errno));
		return -1;
	}
	
	if (ioctl(s, cmd, iwr) < 0) {
		printf("2 function autelan_ioctl::ioctl error ; %s %x\n", strerror(errno),(unsigned int)ioctl);
		close(s);
		return -1;
	}
	close(s);

	return 0;
	
}


static int tos_flag (int argc, char** argv) 
{
	if (argc < 5) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		int i = atoi(argv[4]);
		if (i < 0 || i > 1) {
			
			printf("Invalid tos flag value\n");
		} else {

			//autelan_debug("------get tos flg : %d \n", i);

			u_int8_t buf[1024];
			struct ieee80211_autelan_wmm_params a;
			a.type = TOS_FLAG;
			a.arg1 = i;
			a.arg2 = 0;
			memset(buf, 0x00, 1024);
			memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
			
			struct iwreq iwr;
			(void) memset(&iwr, 0, sizeof(iwr));
			(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
			iwr.u.data.pointer = (void *) buf;
			iwr.u.data.length = sizeof(buf);
			autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		}
		
	}

	return 0;
	
}

static int get_tos_flag (int argc, char** argv) 
{
	if (argc < 4) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_wmm_params a;
		a.type = GET_TOS_FLAG;
		a.arg1 = 0;
		a.arg2 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		printf("Autelan private wmm config : get tos flag :: %d\n", buf[0]);
	}

	return 0;
	
}

int tos_ingress_map (int argc, char** argv) 
{
	if (argc < 6) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		int i = atoi(argv[4]);
		int j = atoi(argv[5]);
		if (i < 0 || j < 0 || i > 3 || j > 255) {
			
			printf("Invalid ip tos map value\n");
		} else {

			//autelan_debug("------get tos ingress map : %d ::%d \n", i, j);


			u_int8_t buf[1024];
			struct ieee80211_autelan_wmm_params a;
			a.type = TOS_INGRESS;
			a.arg1 = i;
			a.arg2 = j;
			memset(buf, 0x00, 1024);
			memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
			
			struct iwreq iwr;
			(void) memset(&iwr, 0, sizeof(iwr));
			(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
			iwr.u.data.pointer = (void *) buf;
			iwr.u.data.length = sizeof(buf);
			autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		}
		
	}

	return 0;
	
}

static int get_tos_ingress_map (int argc, char** argv) 
{
	int i = 0;
	if (argc < 4) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		static unsigned char buf[1024];
		struct ieee80211_autelan_wmm_params a;
		a.type = GET_TOS_INGRESS;
		a.arg1 = 0;
		a.arg2 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		printf("Autelan private wmm config , get tos  ingress map :\n");
		printf("\t\twmm-qos\t::\ttos-qos\n");

		for ( ; i < 4; i++ ) {
			
			printf("\t\t%d\t::\t%d\n", i , buf[i]);
		}
	}

	return 0;
	
}

int tos_egress_map (int argc, char** argv) 
{
	if (argc < 7) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		int index = atoi(argv[4]);
		int i = atoi(argv[5]);
		int j = atoi(argv[6]);
		if (i < 0 || j < 0 || i > 3 || index < 0 || index > 7) {
			
			printf("Invalid ip tos map value\n");
		} else {

			//autelan_debug("------get tos ingress map : %d ::%d \n", i, j);

			u_int8_t buf[1024];
			struct ieee80211_autelan_wmm_params a;
			a.type = TOS_EGRESS;
			a.arg1 = i;
			a.arg2 = j;
			a.index = index;
			memset(buf, 0x00, 1024);
			memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
			
			struct iwreq iwr;
			(void) memset(&iwr, 0, sizeof(iwr));
			(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
			iwr.u.data.pointer = (void *) buf;
			iwr.u.data.length = sizeof(buf);
			autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		}
		
	}

	return 0;
	
}

static int get_tos_egress_map (int argc, char** argv) 
{
	int i = 0;
	if (argc < 4) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		static unsigned char buf[1024];
		struct ieee80211_autelan_wmm_params a;
		a.type = GET_TOS_EGRESS;
		a.arg1 = 0;
		a.arg2 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		printf("Autelan private wmm config , get tos  egress map :\n");
		printf("\t\tindex\t::\ttos-qos\t::\twmm-qos\n");

		for ( ; i < 8; i++ ) {
			
			printf("\t\t%d\t::\t%d\t::\t%d\n", i, buf[i*2] , buf[i*2+1]);
		}
	}

	return 0;
	
}

int vlan_flag (int argc, char** argv) 
{
	if (argc < 5) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		int i = atoi(argv[4]);
		if (i < 0 || i > 1) {
			
			printf("Invalid 1p flag value\n");
		} else {

			//autelan_debug("------get ip flg : %d \n", i);

			u_int8_t buf[1024];
			struct ieee80211_autelan_wmm_params a;
			a.type = VLAN_FLAG;
			a.arg1 = i;
			a.arg2 = 0;
			memset(buf, 0x00, 1024);
			memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
			
			struct iwreq iwr;
			(void) memset(&iwr, 0, sizeof(iwr));
			(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
			iwr.u.data.pointer = (void *) buf;
			iwr.u.data.length = sizeof(buf);
			autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		}
		
	}

	return 0;
	
}

static int get_vlan_flag (int argc, char** argv) 
{
	if (argc < 4) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_wmm_params a;
		a.type = GET_VLAN_FLAG;
		a.arg1 = 0;
		a.arg2 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		printf("Autelan private wmm config : get 1p flag :: %d\n", buf[0]);
	}

	return 0;
	
}

int vlan_egress_map (int argc, char** argv) 
{
	if (argc < 6) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		int i = atoi(argv[4]);
		int j = atoi(argv[5]);
		if (i < 0 || j < 0 ||i > 3 || j > 7) {
			
			printf("Invalid 1p map value\n");
		} else {

			//autelan_debug("------get tos egress map : %d ::%d \n", i, j);

			u_int8_t buf[1024];
			struct ieee80211_autelan_wmm_params a;
			a.type = VLAN_EGRESS;
			a.arg1 = i;
			a.arg2 = j;
			memset(buf, 0x00, 1024);
			memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
			
			struct iwreq iwr;
			(void) memset(&iwr, 0, sizeof(iwr));
			(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
			iwr.u.data.pointer = (void *) buf;
			iwr.u.data.length = sizeof(buf);
			autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		}
		
	}

	return 0;
	
}

static int get_vlan_egress_map (int argc, char** argv) 
{
	int i = 0;
	if (argc < 4) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_wmm_params a;
		a.type = GET_VLAN_EGRESS;
		a.arg1 = 0;
		a.arg2 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		printf("Autelan private wmm config , get 1p vlan egress map :\n");
		printf("\t\tvlan-qos\t::\twmm-qos\n");
		for ( ; i < 8; i++ ) {
			
			printf("\t\t%d\t::\t%d\n", i , buf[i]);
		}
	}

	return 0;
	
}

int vlan_ingress_map (int argc, char** argv) 
{
	if (argc < 6) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		int i = atoi(argv[4]);
		int j = atoi(argv[5]);
		if (i < 0 || j < 0 || i > 3 || j > 7) {
			
			printf("Invalid 1p map value\n");
		} else {

			//autelan_debug("------get tos ingress map : %d ::%d \n", i, j);

			u_int8_t buf[1024];
			struct ieee80211_autelan_wmm_params a;
			a.type = VLAN_INGRESS;
			a.arg1 = i;
			a.arg2 = j;
			memset(buf, 0x00, 1024);
			memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
			
			struct iwreq iwr;
			(void) memset(&iwr, 0, sizeof(iwr));
			(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
			iwr.u.data.pointer = (void *) buf;
			iwr.u.data.length = sizeof(buf);
			autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		}
		
	}

	return 0;
	
}

static int get_vlan_ingress_map (int argc, char** argv) 
{
	int i = 0;
	if (argc < 4) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_wmm_params a;
		a.type = GET_VLAN_INGRESS;
		a.arg1 = 0;
		a.arg2 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		printf("Autelan private wmm config , get 1p vlan ingress map :\n");
		printf("\t\twmm-qos\t::\tvlan-qos\n");

		for ( ; i < 4; i++ ) {
			
			printf("\t\t%d\t::\t%d\n", i , buf[i]);
		}
	}

	return 0;
	
}

/*ljy--add begin to append the switch modified the best effort parameter*/
static int be_flag (int argc, char** argv) 
{
	if (argc < 5) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		int i = atoi(argv[4]);
		if (i < 0 || i > 1) {
			
			printf("Invalid tos flag value\n");
		} else {

			u_int8_t buf[1024];
			struct ieee80211_autelan_wmm_params a;
			a.type = BE_FLAG;
			a.arg1 = i;
			a.arg2 = 0;
			memset(buf, 0x00, 1024);
			memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
			
			struct iwreq iwr;
			(void) memset(&iwr, 0, sizeof(iwr));
			(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
			iwr.u.data.pointer = (void *) buf;
			iwr.u.data.length = sizeof(buf);
			autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		}
		
	}

	return 0;
	
}

static int get_be_flag (int argc, char** argv) 
{
	if (argc < 4) {
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;	
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_wmm_params a;
		a.type = GET_BE_FLAG;
		a.arg1 = 0;
		a.arg2 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_wmm_params));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[3], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_AUTELAN_WMM);
		printf("Autelan private wmm config : get be flag :: %d\n", buf[0]);
	}

	return 0;
	
}
/*ljy--add end*/

/*
 *this function deal with wmm stuff
 */
int autelan_wmm (int argc, char** argv)
{

	/*check arc number*/
	if (argc < 3) {
		
		/*should print autelan wmm help information*/
		autelan_wmm_help ();
		return -1;
	}

	if (strncmp (argv[2], "tos_flag", strlen("tos_flag")) == 0) {
		
		tos_flag(argc, argv);
	
	} else if (strncmp (argv[2], "get_tos_flag", strlen("get_tos_flag")) == 0) {
		
		get_tos_flag(argc, argv);
	
	} else if (strncmp (argv[2], "1p_flag", strlen("1p_flag")) == 0) {
		
		vlan_flag(argc, argv);
	
	} else if (strncmp (argv[2], "get_1p_flag", strlen("get_1p_flag")) == 0) {
		
		get_vlan_flag(argc, argv);
	
	}else if (strncmp (argv[2], "1p_egress_map", strlen("1p_egress-map")) == 0) {
		
		vlan_egress_map(argc, argv);
	
	} else if (strncmp (argv[2], "get_1p_egress_map", strlen("get_1p_egress-map")) == 0) {
		
		get_vlan_egress_map(argc, argv);
	
	} else if (strncmp (argv[2], "1p_ingress_map", strlen("1p_ingress_map")) == 0) {
		
		vlan_ingress_map(argc, argv);
	
	} else if (strncmp (argv[2], "get_1p_ingress_map", strlen("get_1p_ingress_map")) == 0) {
		
		get_vlan_ingress_map(argc, argv);
	
	} else if (strncmp (argv[2], "tos_ingress_map", strlen("tos_ingress_map")) == 0) {
		
		tos_ingress_map(argc, argv);
	
	} else if (strncmp (argv[2], "get_tos_ingress_map", strlen("get_tos_ingress_map")) == 0) {
		
		get_tos_ingress_map(argc, argv);
	
	} else if (strncmp (argv[2], "tos_egress_map", strlen("tos_egress_map")) == 0) {
		
		tos_egress_map(argc, argv);
	
	} else if (strncmp (argv[2], "get_tos_egress_map", strlen("get_tos_egress_map")) == 0) {
		
		get_tos_egress_map(argc, argv);
	
	/*ljy--add begin to append the switch modified the best effort parameter*/
	} else if (strncmp (argv[2], "be_flag", strlen("be_flag")) == 0) {
		
		be_flag(argc, argv);
	
	} else if (strncmp (argv[2], "get_be_flag", strlen("get_be_flag")) == 0) {
		
		get_be_flag(argc, argv);
	
	/*ljy--add end*/
	} else {
		//*should print autelan wmm help information*/
		autelan_wmm_help ();
	}
	
	return 0;

}


/*
 *autelan traffic limit help information
 */
static void autelan_traffic_limit_help () 
{
	printf("\nusage:: autelan traffic limit COMMAND [OPTION] ... \n");
	printf("OPTIONS: \n");
	printf("\t[interface]\t\tset_vap_flag\t\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_vap_flag\n");
	printf("\t[interface]\t\tset_vap\t\t\t\t[traffic number]\n");
	printf("\t[interface]\t\tget_vap\n");

	/*ljy--add begin to separate traffic limit between rx and tx*/
	printf("\t[interface]\t\tset_vap_send\t\t\t[traffic number]\n");
	printf("\t[interface]\t\tget_vap_send\n");
	/*ljy--add end*/

	printf("\t[interface]\t\tset_every_node_flag\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_every_node_flag\n");
	printf("\t[interface]\t\tset_every_node\t\t\t[traffic number]\n");
	printf("\t[interface]\t\tget_every_node\n");

	/*ljy--add begin to separate traffic limit between rx and tx*/
	printf("\t[interface]\t\tset_every_node_send\t\t[traffic number]\n");
	printf("\t[interface]\t\tget_every_node_send\n");
	/*ljy--add end*/

	/*ljy--modified begin to adjust policy of traffic limit*/
	#if 0
	printf("\t[interface]\t\tset_specific_node_flag\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_specific_node_flag\n");
	#endif
	printf("\t[interface]\t\tset_specific_node_flag\t\t[node_mac]\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_specific_node_flag\t\t[node_mac]\n");
	/*ljy--modified end*/

	printf("\t[interface]\t\tset_specific_node\t\t[node_mac]\t\t[traffic number]\n");
	printf("\t[interface]\t\tget_specific_node\t\t[node_mac]\n");

	/*ljy--add begin to separate traffic limit between rx and tx*/
	printf("\t[interface]\t\tset_specific_node_send\t\t[node_mac]\t\t[traffic number]\n");
	printf("\t[interface]\t\tget_specific_node_send\t\t[node_mac]\n");
	/*ljy--add end*/

	printf("\n");
	//return 0;
}


/*
 *this function deal with multicast stuff
 */
int set_vap_traffic_limit (int argc, char** argv)
{
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = SET_VAP_TRAFFIC_LIMIT;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);

	}
	return 0;
	
}

/*
 *this function deal with multicast stuff
 */
int get_vap_traffic_limit (int argc, char** argv)
{
	if (argc < 4) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		memset(buf, 0x00, 1024);
		struct ieee80211_autelan_traffic_limit a;
		memset(&a, 0x00, sizeof(struct ieee80211_autelan_traffic_limit));
		a.type = GET_VAP_TRAFFIC_LIMIT;
		
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		printf("Autelan traffic_limit %s get_vap %d \n", argv[2], *((int *)buf));

	}
	return 0;
	
}


/*
 *this function deal with multicast stuff
 */
int set_node_traffic_limit (int argc, char** argv)
{
	if (argc < 6) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = SET_NODE_TRAFFIC_LIMIT;
		a.arg1 = atoi(argv[5]);
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0,&a1 ,&a2 ,&a3 ,&a4 ,&a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);

	}
	return 0;
	
}

/*
 *this function deal with multicast stuff
 */
int get_node_traffic_limit (int argc, char** argv)
{
	int ret = 0;
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = GET_NODE_TRAFFIC_LIMIT;
		a.arg1 = 0;
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0,&a1 ,&a2 ,&a3 ,&a4 ,&a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		ret = autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		if (ret >= 0) {
			printf("Autelan traffic_limit %s get_node %02x:%02x:%02x:%02x:%02x:%02x :: %d \n", argv[2], a.macaddr[0],a.macaddr[1],a.macaddr[2],a.macaddr[3],a.macaddr[4],a.macaddr[5], *((int *)buf));
		}
	}
	return 0;
	
}


/*
 *this function deal with traffic_limit
 */
int set_every_node_traffic_limit (int argc, char** argv)
{
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = SET_EVERY_NODE_TRAFFIC_LIMIT;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);

	}
	return 0;
	
}

/*
 *this function deal with traffic_limit
 */
int get_every_node_traffic_limit (int argc, char** argv)
{
	if (argc < 4) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = GET_EVERY_NODE_TRAFFIC_LIMIT;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		printf("Autelan traffic_limit %s get_every_node %d\n", argv[2], *((int *)buf));

	}
	return 0;
	
}


/*
 *this function deal with traffic_limit
 */
int set_vap_traffic_limit_flag (int argc, char** argv)
{
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = SET_VAP_TRAFFIC_LIMIT_FLAG;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);

	}
	return 0;
	
}

/*
 *this function deal with traffic_limit
 */
int get_vap_traffic_limit_flag (int argc, char** argv)
{
	if (argc < 4) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = GET_VAP_TRAFFIC_LIMIT_FLAG;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		printf("Autelan traffic_limit %s get_vap_flag %d\n",argv[2],  buf[0]);

	}
	return 0;
	
}

/*
 *this function deal with traffic_limit
 */
int set_every_node_traffic_limit_flag (int argc, char** argv)
{
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = SET_EVERY_NODE_TRAFFIC_LIMIT_FLAG;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);

	}
	return 0;
	
}

/*
 *this function deal with traffic_limit
 */
int get_every_node_traffic_limit_flag (int argc, char** argv)
{
	if (argc < 4) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = GET_EVERY_NODE_TRAFFIC_LIMIT_FLAG;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		printf("Autelan traffic_limit %s get_every_node_flag %d\n", argv[2], buf[0]);

	}
	return 0;
	
}


/*
 *this function deal with traffic_limit
 */
int set_specific_node_traffic_limit_flag (int argc, char** argv)
{
	if (argc < 6) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {
		/*ljy--modified begin to adjust policy of traffic limit*/
		#if 0
		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = SET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		#endif
		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = SET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG;
		a.arg1 = atoi(argv[5]);
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0,&a1 ,&a2 ,&a3 ,&a4 ,&a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		/*ljy--modified end*/
	}
	return 0;
	
}

/*
 *this function deal with traffic_limit
 */
int get_specific_node_traffic_limit_flag (int argc, char** argv)
{
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {
		/*ljy--modified begin to adjust policy of traffic limit*/
		#if 0
		static u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = GET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		printf("Autelan traffic_limit %s get_specific_node_flag %d\n", argv[2], buf[0]);
		#endif
		int ret = 0;
		static u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = GET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG;
		a.arg1 = 0;
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0,&a1 ,&a2 ,&a3 ,&a4 ,&a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		ret = autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		if (ret >= 0) {
			printf("Autelan traffic_limit %s get_specific_node_flag %02x:%02x:%02x:%02x:%02x:%02x :: %d \n", argv[2], a.macaddr[0],a.macaddr[1],a.macaddr[2],a.macaddr[3],a.macaddr[4],a.macaddr[5], buf[0]);
		}
		/*ljy--modified end*/
	}
	return 0;
	
}


/*ljy--add begin to separate traffic limit between rx and tx*/
/*
 *this function deal with multicast stuff
 */
int set_vap_traffic_limit_send (int argc, char** argv)
{
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = SET_VAP_TRAFFIC_LIMIT_SEND;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);

	}
	return 0;
	
}

/*
 *this function deal with multicast stuff
 */
int get_vap_traffic_limit_send (int argc, char** argv)
{
	if (argc < 4) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		memset(buf, 0x00, 1024);
		struct ieee80211_autelan_traffic_limit a;
		memset(&a, 0x00, sizeof(struct ieee80211_autelan_traffic_limit));
		a.type = GET_VAP_TRAFFIC_LIMIT_SEND;
		
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		printf("Autelan traffic_limit %s get_vap_send %d \n", argv[2], *((int *)buf));

	}
	return 0;
	
}

/*
 *this function deal with multicast stuff
 */
int set_node_traffic_limit_send (int argc, char** argv)
{
	if (argc < 6) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = SET_NODE_TRAFFIC_LIMIT_SEND;
		a.arg1 = atoi(argv[5]);
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0,&a1 ,&a2 ,&a3 ,&a4 ,&a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);

	}
	return 0;
	
}

/*
 *this function deal with multicast stuff
 */
int get_node_traffic_limit_send (int argc, char** argv)
{
	int ret = 0;
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = GET_NODE_TRAFFIC_LIMIT_SEND;
		a.arg1 = 0;
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0,&a1 ,&a2 ,&a3 ,&a4 ,&a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		ret = autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		if (ret >= 0) {
			printf("Autelan traffic_limit %s get_node_send %02x:%02x:%02x:%02x:%02x:%02x :: %d \n", argv[2], a.macaddr[0],a.macaddr[1],a.macaddr[2],a.macaddr[3],a.macaddr[4],a.macaddr[5], *((int *)buf));
		}
	}
	return 0;
	
}

/*
 *this function deal with traffic_limit
 */
int set_every_node_traffic_limit_send (int argc, char** argv)
{
	if (argc < 5) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = SET_EVERY_NODE_TRAFFIC_LIMIT_SEND;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);

	}
	return 0;
	
}

/*
 *this function deal with traffic_limit
 */
int get_every_node_traffic_limit_send (int argc, char** argv)
{
	if (argc < 4) {
		autelan_traffic_limit_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_traffic_limit a;
		a.type = GET_EVERY_NODE_TRAFFIC_LIMIT_SEND;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_traffic_limit));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_TRAFFIC_LIMIT);
		printf("Autelan traffic_limit %s get_every_node_send %d\n", argv[2], *((int *)buf));

	}
	return 0;
	
}
/*ljy--add end*/


/*
 *this function deal with traffic limit stuff
 */
int autelan_traffic_limit (int argc, char** argv)
{

	/*check arc number*/
	if (argc < 4) {
		
		/*should print autelan traffic limit help information*/
		autelan_traffic_limit_help ();
		return -1;
	}

	if (strncmp (argv[3], "get_vap_flag", strlen("get_vap_flag")) == 0) {
		
		get_vap_traffic_limit_flag(argc, argv);
	
	} else if (strncmp (argv[3], "set_vap_flag", strlen("set_vap_flag")) == 0) {
		
		set_vap_traffic_limit_flag(argc, argv);
	
	} else if (strncmp (argv[3], "get_every_node_flag", strlen("get_every_node_flag")) == 0) {
		
		get_every_node_traffic_limit_flag(argc, argv);
	
	} else if (strncmp (argv[3], "set_every_node_flag", strlen("set_every_node_flag")) == 0) {
		
		set_every_node_traffic_limit_flag(argc, argv);
	
	} else if (strncmp (argv[3], "get_specific_node_flag", strlen("get_specific_node_flag")) == 0) {
		
		get_specific_node_traffic_limit_flag(argc, argv);
	
	} else if (strncmp (argv[3], "set_specific_node_flag", strlen("set_specific_node_flag")) == 0) {
		
		set_specific_node_traffic_limit_flag(argc, argv);
	
	/*ljy--add begin to separate traffic limit between rx and tx*/
	} else if (strncmp (argv[3], "set_vap_send", strlen("set_vap_send")) == 0) {
		
		set_vap_traffic_limit_send(argc, argv);
	
	} else if (strncmp (argv[3], "get_vap_send", strlen("get_vap_send")) == 0) {
		
		get_vap_traffic_limit_send(argc, argv);
	
	} else if (strncmp (argv[3], "get_specific_node_send", strlen("get_specific_node_send")) == 0) {
		
		get_node_traffic_limit_send(argc, argv);
	
	} else if (strncmp (argv[3], "set_specific_node_send", strlen("set_specific_node_send")) == 0) {
		
		set_node_traffic_limit_send(argc, argv);
	
	} else if (strncmp (argv[3], "get_every_node_send", strlen("get_every_node_send")) == 0) {
		
		get_every_node_traffic_limit_send(argc, argv);
	
	} else if (strncmp (argv[3], "set_every_node_send", strlen("set_every_node_send")) == 0) {
		
		set_every_node_traffic_limit_send(argc, argv);
	/*ljy--add end*/
	
	} else if (strncmp (argv[3], "set_vap", strlen("set_vap")) == 0) {
		
		set_vap_traffic_limit(argc, argv);
	
	} else if (strncmp (argv[3], "get_vap", strlen("get_vap")) == 0) {
		
		get_vap_traffic_limit(argc, argv);
	
	} else if (strncmp (argv[3], "get_specific_node", strlen("get_specific_node")) == 0) {
		
		get_node_traffic_limit(argc, argv);
	
	} else if (strncmp (argv[3], "set_specific_node", strlen("set_specific_node")) == 0) {
		
		set_node_traffic_limit(argc, argv);
	
	} else if (strncmp (argv[3], "get_every_node", strlen("get_every_node")) == 0) {
		
		get_every_node_traffic_limit(argc, argv);
	
	} else if (strncmp (argv[3], "set_every_node", strlen("set_every_node")) == 0) {
		
		set_every_node_traffic_limit(argc, argv);
	
	} else {
		
		autelan_traffic_limit_help();
	
	}
	
	return 0;

}

/*************************begin wds roaming **********************/

/*
 *autelan quick roaming help information
 */
static void autelan_quick_roaming_help () 
{
	printf("\nusage:: autelan quick_roaming COMMAND [OPTION] ... \n");
	printf("OPTIONS: \n");
	printf("\t[interface]\t\tset_flag\t\t\t[ 0 | 1]\n");
	printf("\t[interface]\t\tget_flag\t\t\t[ 0 | 1]\n");
	printf("\t[interface]\t\tadd_node\t\t\t[macaddr]\n");
	printf("\n");
	//return 0;
}

int quick_roaming_set_flag(int argc, char** argv) 
{

	if (argc < 5) {
		autelan_quick_roaming_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_quick_roaming a;
		a.type = SET_FLAG;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_quick_roaming));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_QUICK_ROAMING);

	}
	return 0;
;
}

int quick_roaming_get_flag(int argc, char** argv) 
{

	if (argc < 4) {

		autelan_quick_roaming_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_quick_roaming a;
		a.type = GET_FLAG;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_quick_roaming));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_QUICK_ROAMING);
		printf("Autelan quick_roaming %s get_flag %d\n", argv[2], buf[0]);

	}
	return 0;
;
}

int quick_roaming_add_node(int argc, char** argv) 
{
	return 0;
}

/*
 *this function deal with wds roaming stuff
 */
int autelan_quick_roaming (int argc, char** argv)
{

	/*check arc number*/
	if (argc < 4) {
		
		/*should print autelan quick roaming help information*/
		autelan_quick_roaming_help ();
		return -1;
	}

	if (strncmp (argv[3], "set_flag", strlen("set_flag")) == 0) {
		
		quick_roaming_set_flag(argc, argv);
	
	} else if (strncmp (argv[3], "get_flag", strlen("get_flag")) == 0) { 
		
		quick_roaming_get_flag(argc, argv);
	
	} else {
		
		autelan_quick_roaming_help ();
	
	}
	
	return 0;

}


/*ljy--add begin to develop the policy of mac binding*/
/*
 *autelan mac binding help information
 */
static void autelan_mac_binding_help () 
{
	printf("\nusage:: autelan mac binding COMMAND [OPTION] ... \n");
	printf("OPTIONS: \n");
	printf("\t[interface]\t\tset_dhcp_enable\t\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_dhcp_enable\n");
	printf("\t[interface]\t\tset_ip_auto_learning\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_ip_auto_learning\n");
	printf("\t[interface]\t\tset_ip\t\t\t\t[node_mac]\t\t[ip address]\n");
	printf("\t[interface]\t\tget_ip\t\t\t\t[node_mac]\n");
	printf("\t[interface]\t\tset_vlan\t\t\t[node_mac]\t\t[vlan id]\n");
	printf("\t[interface]\t\tget_vlan\t\t\t[node_mac]\n");
	//sheyanbin--add to through the pppoe frame
	printf("\t[interface]\t\tset_pppoe_enable\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_pppoe_enable\n");
	//sheyanbin--add end
	printf("\n");
}

//sheyanbin--add to through the pppoe frame
int mac_binding_set_pppoe_enable (int argc, char** argv)
{
	if (argc < 5) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		a.type = SET_PPPOE_ENABLE;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);

	}
	return 0;
	
}

int mac_binding_get_pppoe_enable (int argc, char** argv)
{
	if (argc < 4) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		a.type = GET_PPPOE_ENABLE;
		a.arg1 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);
		printf("Autelan mac_binding %s get_pppoe_enable %d\n", argv[2], buf[0]);

	}
	return 0;
	
}
//sheyanbin--add end

int mac_binding_set_dhcp_enable (int argc, char** argv)
{
	if (argc < 5) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		a.type = SET_DHCP_ENABLE;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);

	}
	return 0;
	
}

int mac_binding_get_dhcp_enable (int argc, char** argv)
{
	if (argc < 4) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		a.type = GET_DHCP_ENABLE;
		a.arg1 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);
		printf("Autelan mac_binding %s get_dhcp_enable %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int mac_binding_set_ip_auto_learning (int argc, char** argv)
{
	if (argc < 5) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		a.type = SET_IP_AUTO_LEARNING;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);

	}
	return 0;
	
}

int mac_binding_get_ip_auto_learning (int argc, char** argv)
{
	int ret = 0;
	if (argc < 4) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		a.type = GET_IP_AUTO_LEARNING;
		a.arg1 = 0;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		ret = autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);
		if (ret >= 0) {
			printf("Autelan mac_binding %s get_ip_auto_learning %d\n", argv[2], buf[0]);
		}
	}
	return 0;
	
}

int mac_binding_set_ip (int argc, char** argv)
{
	if (argc < 6) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = SET_IP;
		a.arg1 = 0;
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0, &a1, &a2, &a3, &a4, &a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		a0 = a1 = a2 = a3 = a4 = a5 = 0;
		sscanf(argv[5], "%d.%d.%d.%d",&a0, &a1, &a2, &a3);
		a.ipaddr[0] = a0;
		a.ipaddr[1] = a1;
		a.ipaddr[2] = a2;
		a.ipaddr[3] = a3;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);

	}
	return 0;
	
}

int mac_binding_get_ip (int argc, char** argv)
{
	int ret = 0;
	if (argc < 5) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = GET_IP;
		a.arg1 = 0;
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0,&a1 ,&a2 ,&a3 ,&a4 ,&a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(a.ipaddr, 0x00, sizeof(a.ipaddr));
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		ret = autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);
		if (ret >= 0) {
			memcpy(&a, buf, sizeof(struct ieee80211_autelan_mac_binding));
			printf("Autelan mac_binding %s get_ip %02x:%02x:%02x:%02x:%02x:%02x :: %d.%d.%d.%d\n", argv[2], a.macaddr[0], a.macaddr[1], a.macaddr[2], a.macaddr[3], a.macaddr[4], a.macaddr[5], a.ipaddr[0], a.ipaddr[1], a.ipaddr[2], a.ipaddr[3]);
		}
	}
	return 0;
	
}

int mac_binding_set_vlan (int argc, char** argv)
{
	if (argc < 6) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = SET_VLAN;
		a.arg1 = atoi(argv[5]);
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0, &a1, &a2, &a3, &a4, &a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);

	}
	return 0;
	
}

int mac_binding_get_vlan (int argc, char** argv)
{
	int ret = 0;
	if (argc < 5) {
		autelan_mac_binding_help () ;
		return -1;
	} else {

		static u_int8_t buf[1024];
		struct ieee80211_autelan_mac_binding a;
		int a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
		a.type = GET_VLAN;
		a.arg1 = 0;
		sscanf(argv[4], "%x:%x:%x:%x:%x:%x",&a0,&a1 ,&a2 ,&a3 ,&a4 ,&a5);
		a.macaddr[0] = a0;
		a.macaddr[1] = a1;
		a.macaddr[2] = a2;
		a.macaddr[3] = a3;
		a.macaddr[4] = a4;
		a.macaddr[5] = a5;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_mac_binding));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		ret = autelan_ioctl(&iwr, IEEE80211_IOCTL_MAC_BINDING);
		if (ret >= 0) {
			printf("Autelan mac_binding %s get_vlan %02x:%02x:%02x:%02x:%02x:%02x :: %d\n", argv[2], a.macaddr[0], a.macaddr[1], a.macaddr[2], a.macaddr[3], a.macaddr[4], a.macaddr[5], *((u_int16_t *)buf));
		}
	}
	return 0;
	
}

/*
 *this function deal with mac binding
 */
int autelan_mac_binding (int argc, char** argv)
{

	/*check arc number*/
	if (argc < 4) {
		
		/*should print autelan mac binding help information*/
		autelan_mac_binding_help ();
		return -1;
	}

	if (strncmp (argv[3], "set_dhcp_enable", strlen("set_dhcp_enable")) == 0) {
		
		mac_binding_set_dhcp_enable(argc, argv);
	
	} else if (strncmp (argv[3], "get_dhcp_enable", strlen("get_dhcp_enable")) == 0) { 
		
		mac_binding_get_dhcp_enable(argc, argv);
	
	} else if (strncmp (argv[3], "set_ip_auto_learning", strlen("set_ip_auto_learning")) == 0) { 
		
		mac_binding_set_ip_auto_learning(argc, argv);
	
	} else if (strncmp (argv[3], "get_ip_auto_learning", strlen("get_ip_auto_learning")) == 0) { 
		
		mac_binding_get_ip_auto_learning(argc, argv);
	
	} else if (strncmp (argv[3], "set_ip", strlen("set_ip")) == 0) { 
		
		mac_binding_set_ip(argc, argv);
	
	} else if (strncmp (argv[3], "get_ip", strlen("get_ip")) == 0) { 
		
		mac_binding_get_ip(argc, argv);
	
	} else if (strncmp (argv[3], "set_vlan", strlen("set_vlan")) == 0) { 
		
		mac_binding_set_vlan(argc, argv);
	
	} else if (strncmp (argv[3], "get_vlan", strlen("get_vlan")) == 0) { 
		
		mac_binding_get_vlan(argc, argv);
		
	//sheyanbin--add to through the pppoe frame
	}
	else if (strncmp (argv[3], "set_pppoe_enable", strlen("set_pppoe_enable")) == 0) 
	{ 	
		mac_binding_set_pppoe_enable(argc, argv);	
	} 
	else if (strncmp (argv[3], "get_pppoe_enable", strlen("get_pppoe_enable")) == 0) 
	{ 		
		mac_binding_get_pppoe_enable(argc, argv);		
	//sheyanbin--add end
	} else {
		
		autelan_mac_binding_help ();
	
	}
	
	return 0;

}
/*ljy--add end*/


/*
 *autelan help information
 */
static void autelan_help () 
{
	printf("\nusage:: autelan COMMAND [OPTION] ... \n");
	printf("OPTIONS: \n");
	printf("\twmm\t\t... ...\n");
	printf("\ttraffic_limit\t... ...\n");
	printf("\tquick_roaming\t... ...\n");
	/*ljy--add begin to develop the policy of mac binding*/
	printf("\tmac_binding\t... ...\n");
	/*ljy--add end*/
	printf("\t80211_cache\t... ...\n");//Added by duanmingzhe for 80211 cache
	printf("\n");
	//return 0;
}


/*Begin:added by duanmingzhe for 80211 cache*/
static void autelan_80211_cache_help () 
{
	printf("\nusage:: autelan 80211_cache  COMMAND [OPTION] ... \n");
	printf("OPTIONS: \n");	
	printf("\t[interface]\t\tset_enable\t\t\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_enable\n");
	printf("\t[interface]\t\tset_cache_enable\t\t\t[0 | 1]\n");
	printf("\t[interface]\t\tget_cache_enable\t\t\n");
	printf("\t[interface]\t\tset_cache_queue_maxlen_b\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_queue_maxlen_b\t\t\n");
	printf("\t[interface]\t\tset_cache_queue_maxlen_g\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_queue_maxlen_g\t\t\n");
	printf("\t[interface]\t\tset_cache_queue_maxlen_n\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_queue_maxlen_n\t\t\n");
	printf("\t[interface]\t\tset_cache_threshold_b\t\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_threshold_b\t\t\n");
	printf("\t[interface]\t\tset_cache_threshold_g\t\t\t[num]\n");	
	printf("\t[interface]\t\tget_cache_threshold_g\t\t\n");
	printf("\t[interface]\t\tset_cache_threshold_n\t\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_threshold_n\t\t\n");
	printf("\t[interface]\t\tset_cache_limit\t\t\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_limit\t\t\n");	
	printf("\t[interface]\t\tset_cache_priority_b\t\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_priority_b\t\t\n");
	printf("\t[interface]\t\tset_cache_priority_g\t\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_priority_g\t\t\n");
	printf("\t[interface]\t\tset_cache_priority_n\t\t\t[num]\n");
	printf("\t[interface]\t\tget_cache_priority_n\t\t\n");
	printf("\t[interface]\t\tget_cache_send_count\t\t\t\n");
	printf("\n");
}

int set_enable (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_ENABLE;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_enable (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_ENABLE;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}


int set_cache_enable (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_ENABLE;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_enable (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_ENABLE;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_enable %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_threshold_b (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_THRESHOLD_B;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_threshold_b (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_THRESHOLD_B;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_threshold_b %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_threshold_g (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_THRESHOLD_G;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_threshold_g (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_THRESHOLD_G;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_threshold_g %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_threshold_n (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_THRESHOLD_N;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_threshold_n (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_THRESHOLD_N;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_threshold_n %d\n", argv[2], buf[0]);

	}
	return 0;
	
}


int set_cache_limit (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_LIMIT;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_limit (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_LIMIT;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_limit %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_priority_b (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_PRIORITY_B;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_priority_b (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_PRIORITY_B;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_priority_b %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_priority_g (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_PRIORITY_G;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_priority_g (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_PRIORITY_G;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_priority_g %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_priority_n (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_PRIORITY_N;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_priority_n (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_PRIORITY_N;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_priority_n %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_queue_maxlen_b (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_QUEUE_MAXLEN_B;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_queue_maxlen_b (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_QUEUE_MAXLEN_B;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_queue_maxlen %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_queue_maxlen_g (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_QUEUE_MAXLEN_G;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_queue_maxlen_g (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_QUEUE_MAXLEN_G;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_queue_maxlen %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_queue_maxlen_n (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_QUEUE_MAXLEN_N;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_queue_maxlen_n (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_QUEUE_MAXLEN_N;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_queue_maxlen %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

#if 0
int set_cache_port (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_PORT;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_port (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_PORT;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_port %d\n", argv[2], buf[0]);

	}
	return 0;
	
}


int set_cache_lost (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_LOST;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_lost (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_LOST;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_lost %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_tos (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_TOS;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}

int get_cache_tos (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_TOS;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		printf("Autelan 80211_cache %s get_cache_send_count %d\n", argv[2], buf[0]);

	}
	return 0;
	
}

int set_cache_reserved_qlen (int argc, char** argv)
{
	if (argc < 5) {
		autelan_80211_cache_help () ;
		return -1;
	} else {
		u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = SET_CACHE_RESERVED_QLEN;
		a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}


int get_cache_node_len (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_NODE_LEN;
		//a.arg1 = atoi(argv[4]);
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);
		//printf("Autelan 80211_cache %s get_cache_send_count %d\n", argv[2], buf[0]);

	}
	return 0;
	
}
#endif
int get_cache_send_count (int argc, char** argv)
{
	if (argc < 4) {
		autelan_80211_cache_help () ;
		return -1;
	} else {

		static u_int32_t buf[1024];
		struct ieee80211_autelan_80211_cache a;
		a.type = GET_CACHE_SEND_COUNT;
		memset(buf, 0x00, 1024);
		memcpy(buf, &a, sizeof(struct ieee80211_autelan_80211_cache));
		
		struct iwreq iwr;
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) strncpy(iwr.ifr_name, argv[2], sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) buf;
		iwr.u.data.length = sizeof(buf);
		autelan_ioctl(&iwr, IEEE80211_IOCTL_80211_CACHE);

	}
	return 0;
	
}


int autelan_80211_cache(int argc, char** argv)
{

	/*check arc number*/
	if (argc < 4) {
		
		/*should print autelan mac binding help information*/
		autelan_80211_cache_help ();
		return -1;
	}

	if (strncmp (argv[3], "set_enable", strlen("set_enable")) == 0) {
			
		set_enable(argc, argv);
		
	} else if (strncmp (argv[3], "get_enable", strlen("get_enable")) == 0) { 
			
		get_enable(argc, argv);
		
	}else if (strncmp (argv[3], "set_cache_enable", strlen("set_cache_enable")) == 0) {
		
		set_cache_enable(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_enable", strlen("get_cache_enable")) == 0) { 
		
		get_cache_enable(argc, argv);
	
	} else if (strncmp (argv[3], "set_cache_threshold_b", strlen("set_cache_threshold_b")) == 0) { 
		
		set_cache_threshold_b(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_threshold_b", strlen("get_cache_threshold_b")) == 0) { 
		
		get_cache_threshold_b(argc, argv);
	
	}else if (strncmp (argv[3], "set_cache_threshold_g", strlen("set_cache_threshold_g")) == 0) { 
		
		set_cache_threshold_g(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_threshold_g", strlen("get_cache_threshold_g")) == 0) { 
		
		get_cache_threshold_g(argc, argv);
	
	}else if (strncmp (argv[3], "set_cache_threshold_n", strlen("set_cache_threshold_n")) == 0) { 
		
		set_cache_threshold_n(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_threshold_n", strlen("get_cache_threshold_n")) == 0) { 
		
		get_cache_threshold_n(argc, argv);
	
	}else if (strncmp (argv[3], "set_cache_queue_maxlen_b", strlen("set_cache_queue_maxlen_b")) == 0) { 
		
		set_cache_queue_maxlen_b(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_queue_maxlen_b", strlen("get_cache_queue_maxlen_b")) == 0) { 
		
		get_cache_queue_maxlen_b(argc, argv);
	
	}else if (strncmp (argv[3], "set_cache_queue_maxlen_g", strlen("set_cache_queue_maxlen_g")) == 0) { 
		
		set_cache_queue_maxlen_g(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_queue_maxlen_g", strlen("get_cache_queue_maxlen_g")) == 0) { 
		
		get_cache_queue_maxlen_g(argc, argv);
	
	}else if (strncmp (argv[3], "set_cache_queue_maxlen_n", strlen("set_cache_queue_maxlen_n")) == 0) { 
		
		set_cache_queue_maxlen_n(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_queue_maxlen_n", strlen("get_cache_queue_maxlen_n")) == 0) { 
		
		get_cache_queue_maxlen_n(argc, argv);
	
	}else if (strncmp (argv[3], "set_cache_limit", strlen("set_cache_limit")) == 0) { 
		
		set_cache_limit(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_limit", strlen("get_cache_limit")) == 0) { 
		
		get_cache_limit(argc, argv);
	
	}
	else if (strncmp (argv[3], "set_cache_priority_b", strlen("set_cache_priority_b")) == 0) 
	{ 	
		set_cache_priority_b(argc, argv);	
	} 
	else if (strncmp (argv[3], "get_cache_priority_b", strlen("get_cache_priority_b")) == 0) 
	{ 		
		get_cache_priority_b(argc, argv);	
		
	}else if (strncmp (argv[3], "set_cache_priority_g", strlen("set_cache_priority_g")) == 0) 
	{ 	
		set_cache_priority_g(argc, argv);	
	} 
	else if (strncmp (argv[3], "get_cache_priority_g", strlen("get_cache_priority_g")) == 0) 
	{ 		
		get_cache_priority_g(argc, argv);	
		
	}else if (strncmp (argv[3], "set_cache_priority_n", strlen("set_cache_priority_n")) == 0) 
	{ 	
		set_cache_priority_n(argc, argv);	
	} 
	else if (strncmp (argv[3], "get_cache_priority_n", strlen("get_cache_priority_n")) == 0) 
	{ 		
		get_cache_priority_n(argc, argv);	
		
	}
	#if 0
	else if (strncmp (argv[3], "set_cache_port", strlen("set_cache_port")) == 0) { 
		
		set_cache_port(argc, argv);
	
	} else if (strncmp (argv[3], "get_cache_port", strlen("get_cache_port")) == 0) { 
		
		get_cache_port(argc, argv);
		
	}
	else if (strncmp (argv[3], "set_cache_lost", strlen("set_cache_lost")) == 0) 
	{ 	
		set_cache_lost(argc, argv);	
	} 
	else if (strncmp (argv[3], "set_cache_lost", strlen("set_cache_lost")) == 0) 
	{ 		
		get_cache_lost(argc, argv);	
		
	}
	else if (strncmp (argv[3], "set_cache_tos", strlen("set_cache_tos")) == 0) 
	{ 	
		set_cache_tos(argc, argv);	
	} 
	else if (strncmp (argv[3], "get_cache_tos", strlen("get_cache_tos")) == 0) 
	{ 		
		get_cache_tos(argc, argv);	
		
	}else if (strncmp (argv[3], "get_cache_node_len", strlen("get_cache_node_len")) == 0) 
	{ 		
		get_cache_node_len(argc, argv);	
		
	}else if (strncmp (argv[3], "set_cache_reserved_qlen", strlen("set_cache_reserved_qlen")) == 0) 
	{ 		
		set_cache_reserved_qlen(argc, argv);	
		
	}
	#endif
	else if (strncmp (argv[3], "get_cache_send_count", strlen("get_cache_send_count")) == 0) 
	{ 		
		get_cache_send_count(argc, argv);	
		
	}else {
		
		autelan_80211_cache_help ();
	
	}
	
	return 0;

}
/*End:added by duanmingzhe for 80211 cache*/



/*
 *the main function for autelan private command
 */
int main (int argc, char** argv)
{
	/*check the arc number*/
	if(argc < 2) {
		autelan_help () ;
		printf("AUTELAN private command error: incomplete command\n");
		return 0;
	}

	if (strncmp (argv[1], "wmm", strlen("wmm")) == 0) {

		//autelan_debug("---begin autelan_wmm\n")	;
		
		if (autelan_wmm(argc, argv) < 0) {

			printf("AUTELAN private command wmm: wrong format\n");		
		}

	} else if (strncmp (argv[1], "traffic_limit", strlen("traffic_limit")) == 0) {

		//autelan_debug("---begin autelan_traffic_limit\n")	;
		
		if (autelan_traffic_limit(argc, argv) < 0) {

			printf("AUTELAN private command traffic_limit: wrong format\n");		
		}
	} else if (strncmp (argv[1], "quick_roaming", strlen("quick_roaming")) == 0) { 

		if (autelan_quick_roaming(argc, argv) < 0) {

			printf("AUTELAN private command quick_roaming: wrong format\n");		
		}
	/*ljy--add begin to develop the policy of mac binding*/
	} else if (strncmp (argv[1], "mac_binding", strlen("mac_binding")) == 0) {

		if (autelan_mac_binding(argc, argv) < 0) {

			printf("AUTELAN private command mac_binding: wrong format\n");		
		}
	/*ljy--add end*/
        /*Begin:added by duanmingzhe for 80211 cache*/
	}else if (strncmp (argv[1], "80211_cache", strlen("80211_cache")) == 0) {

		if (autelan_80211_cache(argc, argv) < 0) {

			printf("AUTELAN private command 80211_cache: wrong format\n");		
		}
        /*End:added by duanmingzhe for 80211 cache*/
	
	} else {

		printf("AUTELAN private command error: unknow command name\n");
		autelan_help();
	}
	return 0;
	
}
