/*
 * Copyright (c) 2002-2006 Atheros Communications, Inc.
 * All rights reserved.
 * ---------------------------------------------------
 * Contains source related to spectraldemo user app
 */

#ifndef _SPECTRAL_DEMO_H_
#define _SPECTRAL_DEMO_H_

#include <spec_msg_proto.h>
#include <spectral_data.h>

enum {
        ATH_DEBUG_SPECTRAL       = 0x00000100,   /* Minimal SPECTRAL debug */
        ATH_DEBUG_SPECTRAL1      = 0x00000200,   /* Normal SPECTRAL debug */
        ATH_DEBUG_SPECTRAL2      = 0x00000400,   /* Maximal SPECTRAL debug */
        ATH_DEBUG_SPECTRAL3      = 0x00000800,   /* matched filterID display */
};

#define MAX_SAMP_SAVED 5
#define MAX_NUM_FREQ 2
#define SPECTRAL_DPRINTF(_m, _fmt, ...) do {            \
    if ((_m) <= spectral_debug_level) {                 \
        printf(_fmt, ##__VA_ARGS__);                    \
    }        \
} while (0)

typedef struct saved_samp_msg
{
  int freq;
  int count_saved;
  int cur_save_index;
  int cur_send_index;
  SPECTRAL_SAMP_MSG samp_msg[MAX_SAMP_SAVED];
}SAVED_SAMP_MSG;
//} __ATTRIB_PACK SAVED_SAMP_MSG;

SAVED_SAMP_MSG all_saved_samp[MAX_NUM_FREQ];

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

extern char *samp_resp;
extern char samp_msg[1024];

extern struct ss lwrband;
extern struct ss uprband;

extern struct sockaddr_nl src_addr, dst_addr;
extern struct nlmsghdr *nlh;
extern struct msghdr msg;
extern struct iovec iov;

extern int send_single;
extern int sock_fd;

u_int8_t *build_single_samp_rsp (u_int16_t freq, int *nbytes,
				 u_int8_t * bin_pwr, int pwr_count,
				 u_int8_t rssi, int8_t lower_rssi,
				 int8_t upper_rssi, u_int8_t max_scale,
				 int16_t max_mag,
				 struct INTERF_SRC_RSP *interf);

u_int8_t *build_fake_samp_rsp (u_int16_t freq, int pwr_count,
			       u_int8_t fake_pwr_val, int *nbytes,
			       struct INTERF_SRC_RSP *interf);

u_int8_t *parse_samp_msg(u_int8_t *buf, int *nbytes, int recv_fd, int listener_fd, int netlink_fd);

u_int8_t *parse_samp_data_msg(u_int8_t *buf, int *nbytes, int recv_fd, int listener_fd, int netlink_fd);

void process_kernel_spectral_msg(SPECTRAL_SAMP_MSG* msg);
void print_detection(struct ss *lower_band, struct ss* upper_band);
void print_classifier_counts(struct ss *bd, const char *print_str);

#endif
