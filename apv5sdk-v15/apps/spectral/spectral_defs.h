#ifndef _SPECTRAL_STATE_H_
#define _SPECTRAL_STATE_H_

#ifndef WIN32 
#include <sys/ioctl.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/netlink.h>
#include <netinet/in.h>
#include <math.h>
#else
#include "spectral_types.h"
#endif

#include "spectral_data.h"
#include "spec_msg_proto.h"
#include "spectraldemo.h"
#include "classifier.h"


#ifdef  ATH_SPECTRAL_DAEMONIZE
#include "daemon.h"
#endif  /* ATH_SPECTRAL_DAEMONIZE */

#define ATHPORT 8001   // the port users will be connecting to
#define BACKLOG 10    // how many pending connections queue will hold

#define	streq(a,b)	(strcasecmp(a,b) == 0)


#ifndef NETLINK_GENERIC
    #define NETLINK_GENERIC 16
#endif

#ifndef MAX_PAYLOAD
#define MAX_PAYLOAD 1024
#endif

#ifndef MAX_SAVED_SAMP_MSGS
#define MAX_SAVED_SAMP_MSGS 10
#endif

#define GUI_UPPER_FREQ 2472
#define GUI_LOWER_FREQ 2432

#define GUI_IGNORE_THIS_BIN_VALUE 255

typedef enum {
    SPECTRAL_SERVER_STATE_IDLE=0,
    SPECTRAL_SERVER_STATE_GUI_CONNECTED,
    SPECTRAL_SERVER_STATE_SETUP_SCAN,
    SPECTRAL_SERVER_STATE_SCAN,
    SPECTRAL_SERVER_STATE_SCAN_COMPLETE,
    SPECTRAL_SERVER_STATE_ERROR,
} eSPECTRAL_SERVER_STATE;

typedef struct {
    int msg_valid;
    int current_index;
    SPECTRAL_SAMP_MSG saved_msg;
} SAVED_SAMP_MESSAGE;

typedef struct {
    int freq;
    int num_valid;
    SAVED_SAMP_MESSAGE msg_pool[MAX_SAVED_SAMP_MSGS];
    SAVED_SAMP_MESSAGE *pool_ptr;
} PER_FREQ_SAMP_MSG_POOL;

extern PER_FREQ_SAMP_MSG_POOL upper, lower;

extern SAVED_SAMP_MESSAGE current_saved_msgs[MAX_SAVED_SAMP_MSGS], *current_saved_pool;
extern SAVED_SAMP_MESSAGE prev_saved_msgs[MAX_SAVED_SAMP_MSGS], *prev_saved_pool;

extern int cur_saved_index;
extern int prev_saved_index;

extern int spectral_debug_level;
extern eSPECTRAL_SERVER_STATE current_server_state;

extern struct sockaddr_nl src_addr, dst_addr;
extern struct nlmsghdr *nlh;
extern struct msghdr msg;
extern struct iovec iov;

extern int send_single;
extern int num_saved;
extern int num_sent;
extern int num_responses_reqd, num_to_save;
extern int sock_fd;

extern int global_do_eacs;
extern int global_current_freq;
extern int global_prev_freq;
 extern int global_minpwr;
 extern int global_changefreq;
 extern int global_changechannel;
 extern int global_rawfft;
 extern int global_scaledata;
 extern int global_userssi;
 extern int global_flip;
 extern int global_indexdata;
 extern int global_onlyrssi;
 extern int global_is_classify;
 extern int total_send;
extern int total_channel_switches;

extern char *samp_resp;
extern char samp_msg[1024];

extern struct ss lwrband;
extern struct ss uprband;

#define MAX_INTERF_COUNT 10
extern struct INTERF_RSP global_interf_rsp[MAX_INTERF_COUNT];
extern int global_interf_count;

extern eSPECTRAL_SERVER_STATE current_server_state;
extern int total_channel_switches;
extern int num_sent;
extern int num_responses_reqd, num_to_save;
extern SPECTRAL_SAMP_MSG global_fake_samp_msg;

extern int global_num_alarms;
extern int global_maxhold_interval;

extern SPECTRAL_SAMP_MSG lower_max_rssi_msg, upper_max_rssi_msg;
extern int16_t lower_maxrssi, upper_maxrssi;
extern int32_t lower_last_tstamp, upper_last_tstamp;

/* prototypes */

u_int8_t *server_parse_samp_req_msg(u_int8_t *buf, int *nbytes, int, int, int);

u_int8_t *server_parse_samp_data_msg(u_int8_t *buf, int *nbytes, int recv_fd, int listener_fd, int netlink_fd);

void start_scan(void);
void stop_scan(void);
void switch_channel(void);
void update_global_freqs(void);

/* Message saving and sending saved messages */
void save_this_samp_message(SPECTRAL_SAMP_MSG *msg_to_save);
void update_saved_msg_ptrs(void);
void send_saved_msg_to_gui(int send_fd, SAVED_SAMP_MESSAGE *send_from_pool, int send_freq);
void save_maxrssi_samp_message(SPECTRAL_SAMP_MSG *msg_to_save);

void init_saved_message_pool(PER_FREQ_SAMP_MSG_POOL *per_freq_pool);
void init_freq_message_pools(void);
int can_send_saved_messages(void);

void server_send_samp_data_to_gui(SPECTRAL_SAMP_MSG* nl_samp_msg, int send_fd);
void server_send_maxrssi_samp_data_to_gui(SPECTRAL_SAMP_MSG* nl_samp_msg, int send_fd);
u_int8_t *timeout_send_fake_samp_rsp (u_int16_t freq, int pwr_count, u_int8_t fake_pwr_val, int *nbytes, struct INTERF_SRC_RSP *interf);
u_int8_t* server_build_single_samp_rsp (u_int16_t freq, int *nbytes, u_int8_t *bin_pwr, int pwr_count, u_int8_t rssi, int8_t lower_rssi, int8_t upper_rssi, u_int8_t max_scale, int16_t max_mag, struct INTERF_SRC_RSP *interf);

void add_fake_interference_report(struct INTERF_SRC_RSP *rsp);

/* Dummy SAMP message handling */
void init_dummy_samp_msg(void);
void update_dummy_samp_msg(int send_freq);

/* Debug printing function prototypes */
void print_saved_message_pool(SAVED_SAMP_MESSAGE *pool);
void print_current_prev_message_pools(void);

/* EACS related */
int is_dynamic_chan_change_reqd(int, int);
int is_interference_detected(struct ss *bd);
void start_eacs_scan(void);
void start_eacs_monitor_scan(void);
void stop_eacs_monitor_scan(void);
int eacs_main(int argc, char *argv[]);

#endif
