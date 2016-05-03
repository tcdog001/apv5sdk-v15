/*
 * =====================================================================================
 *
 *       Filename:  spectraldemo.c
 *
 *    Description:  User level app for spectral classifier demo

 *        Version:  1.0
 *        Created:  Wednesday 05 August 2009 12:10:27  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  S.Karthikeyan
 *        Company:  Atheros Communication Limited 
 *
 * =====================================================================================
 */


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


#include "spectral_data.h"
#include "spec_msg_proto.h"
#include "spectraldemo.h"
#include "classifier.h"
#include "daemon.h"
#include <time.h>

#define ATHPORT 8001   // the port users will be connecting to
#define BACKLOG 10    // how many pending connections queue will hold

#define	streq(a,b)	(strcasecmp(a,b) == 0)


#if 1
#ifndef NETLINK_GENERIC
    #define NETLINK_GENERIC 16
#endif
#endif
#ifndef MAX_PAYLOAD
#define MAX_PAYLOAD 1024
#endif

#define COLLECT_SPECT_SCAN_STATS (1)
#define MAX_TIME_DIFF (100000)
#define MAX_NETLINK_BUFFER_TO_READ (40)
#define SPECTRAL_SCAN_BUF_LEN (MAX_NETLINK_BUFFER_TO_READ*MAX_SPECTRAL_MSG_ELEMS)



/* prototypes */
void init_classifier_data();
void process_kernel_spectral_msg(SPECTRAL_SAMP_MSG *msg);
u_int8_t *parse_samp_msg(u_int8_t *buf, int *nbytes, int, int, int);
void send_samp_resp(int send_fd, int listener, u_int8_t *tmp, int *nbytes);
void parse_request_file();
int interference_found(struct ss* lwrband, struct ss* uprband);


/* globals */

/* the classifier related information is 
 * cached here in these structures
 */
struct ss lwrband;
struct ss uprband;
int spectral_debug_level=ATH_DEBUG_SPECTRAL;
int is_daemon = 0;
u_int32_t prevTimeStamp = 0;

struct sockaddr_nl src_addr, dst_addr;
struct nlmsghdr *nlh = NULL;
struct msghdr msg;
struct iovec iov;

int send_single=0;
int sock_fd;

int global_current_freq=2462;
int global_prev_freq=2462;
int global_minpwr=-100;
int global_changefreq=0;
int global_changechannel=0;
int global_rawfft=0;
int global_scaledata=0;
int global_userssi=1;
int global_flip=0;
int global_indexdata=0;
int global_onlyrssi=0;
int global_is_classify=0;
int total_send=0;

char *samp_resp;
char samp_msg[1024];

void clear_saved_samp_msgs(void)
{
   int i;

   memset(all_saved_samp, 0, sizeof(all_saved_samp));
   for (i=0; i< MAX_NUM_FREQ; i++){
        all_saved_samp[i].cur_send_index=-1;        
        all_saved_samp[i].cur_save_index=0;        
        all_saved_samp[i].count_saved=0;        
   }
    all_saved_samp[0].freq = 2422; 
    all_saved_samp[1].freq = 2462; 
}

void save_this_samp_msg(int current_freq, SPECTRAL_SAMP_MSG *save)
{
   SAVED_SAMP_MSG *save_to=NULL;
   SPECTRAL_SAMP_MSG *dest=NULL;
   int i, found;
    
   for (i=0; i< MAX_NUM_FREQ; i++){
       if (all_saved_samp[i].freq == current_freq) {
            save_to = &all_saved_samp[i];
            found=1;
            break;            
        }
   }

   SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d found=%d save->freq=%d\n", \
                    __func__, __LINE__, found, save->freq);   

   if(save_to) {

       SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d freq %u cur_send=%d \
                         cur_save=%d count=%d\n", __func__, __LINE__,\
                         save_to->freq, save_to->cur_send_index, \
                         save_to->cur_save_index, save_to->count_saved);

       if (save_to->count_saved == MAX_SAMP_SAVED) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s No more space freq=%d \
                             count_saved=%d\n",__func__, save_to->freq, \
                             save_to->count_saved);
            return;   
       }

       save_to->count_saved++;
       dest =  &save_to->samp_msg[save_to->cur_save_index];

       memcpy(dest,save, sizeof(SPECTRAL_SAMP_MSG));

       if(save_to->cur_send_index == -1) { //this is the first data we save
            save_to->cur_send_index = save_to->cur_save_index; //this is the first data we save
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d found=%d\n", __func__, __LINE__, found);   
       }

       save_to->cur_save_index++;

       SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s freq %u cur_send=%d \
                        cur_save=%d count=%d\n", __func__, \
                        save_to->freq, save_to->cur_send_index, \
                        save_to->cur_save_index, save_to->count_saved);

        if(save_to->cur_send_index == MAX_SAMP_SAVED) {
            save_to->cur_send_index = 0;

            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d freq %u cur_send=%d \
                             cur_save=%d count=%d\n", __func__, __LINE__, \
                             save_to->freq, save_to->cur_send_index, \
                             save_to->cur_save_index, save_to->count_saved);
        }

        if(save_to->cur_save_index == MAX_SAMP_SAVED) {
            if(save_to->cur_send_index !=0) {
                save_to->cur_save_index = 0;
            } else 
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Ran out out space freq=%d \
                                 cur_save=%d cur_send=%d\n",\
                                 current_freq, save_to->cur_save_index, 
                                 save_to->cur_send_index);
        }

        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d freq %u cur_send=%d \
                         cur_save=%d count=%d\n", __func__,\
                         __LINE__, save_to->freq,\
                         save_to->cur_send_index, \
                         save_to->cur_save_index, \
                         save_to->count_saved);
    }
}

u_int8_t *send_saved_samp_msg(int current_freq, int *nbytes, struct INTERF_SRC_RSP *interf)
{
   SAVED_SAMP_MSG *to_send=NULL;
   SPECTRAL_SAMP_MSG *send_samp_msg=NULL;
   SPECTRAL_SAMP_DATA *send_samp_data;
   int i, found;
   u_int8_t *tmp=NULL;

   for (i=0; i< MAX_NUM_FREQ; i++){
       if (all_saved_samp[i].freq == current_freq) {
            to_send = &all_saved_samp[i];
            found=1;
            break;            
        }
   } 


   if(to_send) {
       if((to_send->cur_send_index != -1) && (to_send->count_saved)) {

           to_send->count_saved--;
           send_samp_msg = &to_send->samp_msg[to_send->cur_send_index];
           send_samp_data = &send_samp_msg->samp_data;

                
           // Tue Dec 23 17:36:31 PST 2008 Send current interference report instead of what has been saved
           if(!interf->count) {
               tmp = build_single_samp_rsp (send_samp_msg->freq, nbytes, 
                                            send_samp_data->bin_pwr, 
                                            send_samp_data->bin_pwr_count, 
                                            send_samp_data->spectral_combined_rssi, 
                                            send_samp_data->spectral_lower_rssi, 
                                            send_samp_data->spectral_upper_rssi, 
                                            send_samp_data->spectral_max_scale, 
                                            send_samp_data->spectral_max_mag, 
                                            &send_samp_data->interf_list); 
           } else {
               tmp = build_single_samp_rsp (send_samp_msg->freq, nbytes, 
                                            send_samp_data->bin_pwr, 
                                            send_samp_data->bin_pwr_count, 
                                            send_samp_data->spectral_combined_rssi, 
                                            send_samp_data->spectral_lower_rssi, 
                                            send_samp_data->spectral_upper_rssi, 
                                            send_samp_data->spectral_max_scale, 
                                            send_samp_data->spectral_max_mag, 
                                            interf);
           }

           to_send->cur_send_index++;

           if(to_send->cur_save_index == MAX_SAMP_SAVED) {
                to_send->cur_save_index = 0;
            }
           if(to_send->cur_send_index == MAX_SAMP_SAVED) {
                if(to_send->cur_save_index !=0) {
                    to_send->cur_send_index = 0;
            } else 
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s Ran out out space freq=%d \
                                 cur_save=%d cur_send=%d\n",__func__,   
                                 current_freq, to_send->cur_save_index, 
                                 to_send->cur_send_index);
        }
      } else
          SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d Nothing to send\n", __func__, __LINE__);
    }
    return tmp;
}

void stop_scan(void)
{
    if(global_is_classify)
        global_is_classify=0;
    system("spectraltool stopscan");
}


u_int16_t ath_byte_swap(u_int8_t *in)
{
    u_int16_t ret=0;
    int len = strlen(in);
    char temp;

    in[2] = '\0';
    temp = in[0];
    in[0] = in[1];
    in[1] = temp;
    ret = (u_int16_t)atoi(in);
    return ret;
}

u_int8_t tmp_buf[1024]="010056001c097628099e2809bc1413382813602813882813b02813d82814002814282814502814782814a02814c82814f02815182815402815682815902815b82815e028160828163028165828168028";

u_int8_t tmp_ubuf[1024]={0};

u_int8_t convert_char (char x)
{
    u_int8_t ret=0;

    if(isalpha(x)) {
        x=toupper(x);
        ret =  (x - 'A');
    } else {
        ret =  (x - '0');
    }

    return ret;
}
void only_parse_samp_msg(u_int8_t *buf, int *nbytes)
{
    struct DATA_REQ_VAL *rval=NULL;
    struct FREQ_BW_REQ *req=NULL;
     u_int16_t num_elems, tlv_len;
    int i;
    struct TLV *rsp, *tlv;
    u_int8_t parse[17]={'\0'};
    u_int8_t temp;

    tlv = (struct TLV*)buf;
    
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"tlv->tag=%x sizeof(tag)=%d\n", \
                     tlv->tag, sizeof(tlv->tag));
    tlv_len = ntohs(tlv->len);
    rval = (struct DATA_REQ_VAL*)tlv->value;
    req = (struct FREQ_BW_REQ*)rval->data;

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"sizeof(tlv->len)=%d \
                     sizeof(rval->count)=%d\n", 
                     sizeof(tlv->len), sizeof(rval->count));
    num_elems = ntohs(rval->count);

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"AS PASSED IN tlv->len=%x \
                     num_elems=%x\n", tlv->len, rval->count);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"NTOHS tlv->len=%x \
                     num_elems=%x\n", tlv_len, num_elems);

    for (i=0; i < num_elems; i++) {
        req++;
    }

    return;
}

void parse_request_file()
{

    FILE *fptr=NULL;
    int i=0, j=0;
    char c[2]={'\0'};
    fptr = fopen("samp-msg.txt", "r");
    
    if (fptr) {
        while (!feof(fptr)) {
            fscanf(fptr, "%u", &tmp_ubuf[i]);
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"MSG %u \n", tmp_ubuf[i]);
            i++; 
        }
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"File contained %d bytes copied %d bytes\n", i, j);
        fclose(fptr);
    } else {

        perror("Cannot open samp-msg.txt");
    }

    only_parse_samp_msg(tmp_ubuf, &j);
    return ;
}

void parse_samp_rsp(u_int8_t *buf, int *nbytes)
{
    struct SAMPLE_RSP *sample_rsp=NULL;
    struct FREQ_PWR_RSP *pwr_resp=NULL;
    struct INTERF_SRC_RSP *src_resp=NULL;
    struct INTERF_RSP *interf_resp=NULL;
    u_int16_t num_elems,tlv_len;
    int i;
    struct TLV *rsp, *tlv;

    tlv = (struct TLV*)buf;
    tlv_len = htons(tlv->len);
    sample_rsp = (struct SAMPLE_RSP *)tlv->value;

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"tlv->tag=%d tlv->len=%u \
                     rsp->freq=%u rsp->count=%u\n", tlv->tag, 
                     tlv->len, sample_rsp->freq, 
                     sample_rsp->sample_count);

    num_elems = ntohs(sample_rsp->sample_count);
    pwr_resp = (struct FREQ_PWR_RSP*)sample_rsp->samples;

    for (i=0; i < sample_rsp->sample_count; i++) {
        if (pwr_resp->pwr != 0xFF)
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"[%d]req->pow = %d\n", i, pwr_resp->pwr);
        pwr_resp++;
    }

    src_resp = (struct INTERF_SRC_RSP*)pwr_resp;
    interf_resp = (struct INTERF_RSP*)src_resp->interf;

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"interf min freq=%u\n", interf_resp->interf_min_freq);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"interf max freq=%u\n", interf_resp->interf_max_freq);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"interf type=%d\n", interf_resp->interf_type);

    return;
}

u_int8_t *build_fake_samp_rsp (u_int16_t freq, int pwr_count, u_int8_t fake_pwr_val, int *nbytes, struct INTERF_SRC_RSP *interf)
{

   int resp_alloc_size = sizeof(struct TLV);
   struct SAMPLE_RSP *sample_rsp=NULL;
   struct FREQ_PWR_RSP *pwr_resp=NULL;
   struct INTERF_SRC_RSP *src_resp=NULL;
   struct INTERF_RSP *interf_resp=NULL;
   int i;
   struct TLV *rsp;
   u_int8_t *tmp=NULL; 
    
    tmp = send_saved_samp_msg(freq, nbytes, interf);
    if (tmp) {
        if(global_prev_freq != freq)
            return tmp;
    }

    resp_alloc_size = sizeof(struct TLV);

    resp_alloc_size += (sizeof(struct SAMPLE_RSP) + sizeof(struct INTERF_SRC_RSP));
    resp_alloc_size += (128)*(sizeof(struct FREQ_PWR_RSP)) + sizeof(struct INTERF_RSP);

    
    rsp = (struct TLV *)malloc(resp_alloc_size); 
    rsp->tag = SAMP_RESPONSE;
    rsp->len = resp_alloc_size - 3;
    sample_rsp = (struct SAMPLE_RSP*)rsp->value;

    sample_rsp->freq = htons(freq);
    sample_rsp->sample_count = htons(128);
    
    pwr_resp = (struct FREQ_PWR_RSP*)sample_rsp->samples;

    for (i=0; i < pwr_count; i++) {
            if (global_minpwr)
                pwr_resp->pwr = abs(global_minpwr);
            else
                pwr_resp->pwr = fake_pwr_val;
            pwr_resp++;
    }

    src_resp = (struct INTERF_SRC_RSP*)pwr_resp;
    src_resp->count = htons(interf->count);
    for (i=0; i < interf->count; i++) {
        interf_resp = &src_resp->interf[i];
        interf_resp->interf_min_freq = htons(interf->interf[i].interf_min_freq);
        interf_resp->interf_max_freq = htons(interf->interf[i].interf_max_freq);
        interf_resp->interf_type = interf->interf[i].interf_type;
        //printf("%d interf type =%d\n", i, (int)interf_resp->interf_type);
    }
    if(interf->count)
        //printf("%s message contains %d interference reports count reported is %d\n", __func__, i, src_resp->count);
    *nbytes = resp_alloc_size;
    return (char *)rsp;
}

// This should take the netlink data received and convert it into a SAMP
// response message. This can then be posted to the listening client.
u_int8_t *build_single_samp_rsp (u_int16_t freq, 
                                 int *nbytes, 
                                 u_int8_t *bin_pwr, 
                                 int pwr_count, 
                                 u_int8_t rssi, 
                                 int8_t lower_rssi, 
                                 int8_t upper_rssi, 
                                 u_int8_t max_scale, 
                                 int16_t max_mag, 
                                 struct INTERF_SRC_RSP *interf)
{

    int resp_alloc_size = sizeof(struct TLV);
    struct SAMPLE_RSP *sample_rsp=NULL;
    struct FREQ_PWR_RSP *pwr_resp=NULL;
    struct INTERF_SRC_RSP *src_resp=NULL;
    struct INTERF_RSP *interf_resp=NULL;
    u_int16_t num_elems,tlv_len;
    int i, j;
    struct TLV *rsp;
    u_int8_t val;
    float pwr_val;
    static u_int8_t resp_num=0;
    float calc_maxmag = log10f(max_mag);

    resp_alloc_size = sizeof(struct TLV);

    resp_alloc_size += (sizeof(struct SAMPLE_RSP) + sizeof(struct INTERF_SRC_RSP));
    resp_alloc_size += (128)*(sizeof(struct FREQ_PWR_RSP)) + sizeof(struct INTERF_RSP);

    
    rsp = (struct TLV *)malloc(resp_alloc_size); 
    rsp->tag = SAMP_RESPONSE;
    rsp->len = resp_alloc_size - 3;
    sample_rsp = (struct SAMPLE_RSP*)rsp->value;

    sample_rsp->freq = htons(freq);
    sample_rsp->sample_count = htons(128);
    
    pwr_resp = (struct FREQ_PWR_RSP*)sample_rsp->samples;

    for (i=0; i < pwr_count; i++) {

        if (i==0 && global_indexdata == 1) {
            pwr_resp->pwr = resp_num++;
            pwr_resp++;
            continue;
        }
        val = (u_int8_t)(*(bin_pwr+i));
        if ( val == 0)
            val = 0x1;

        //Tue Nov  4 23:25:37 PST 2008 Special case when driver wants to fake data it passes 255 as the power value
        if(val == 255) {
            //pwr_val = 255;
            //Thu Nov  6 13:47:06 PST 2008 - Try to differentiate visually when the fake data comes
            pwr_val = global_minpwr;
            if (pwr_val < 0)
                pwr_val = abs(pwr_val);
            else
                pwr_val = 255;
            pwr_resp->pwr = (u_int8_t)pwr_val;
            pwr_resp++; continue;    
        }
        pwr_val = (float)val;

        /* Only use RSSI, lower rssi for lower bins and upper for upper bins*/
        if (global_onlyrssi) {
            if (i <= 63)
                    pwr_val = lower_rssi;
                else
                    pwr_val = upper_rssi;

            if (pwr_val < 0)
                pwr_val = abs(pwr_val);
            else
                pwr_val = 255;

        } else {
              /* If not to pass up raw FFT data as is without the dbm conversion*/
              if (!global_rawfft) {
                  pwr_val *= max_scale;
                  pwr_val = (log10f(pwr_val));
                  SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"fft val = %d pwr_val =%f calc_maxmag=%f\n",val, pwr_val, calc_maxmag);
                  pwr_val -= calc_maxmag;
                  pwr_val *= 20;
                  if (global_userssi) {
                      if (i <= 63)
                          pwr_val += lower_rssi;
                      else
                          pwr_val += upper_rssi;
                  }
                  /* If below the minimum power threshold specified, bump it up to minpwr. This means the GUI is more uniform*/
                  if (pwr_val < global_minpwr)
                      pwr_val = global_minpwr;

                  if (pwr_val < 0)
                      pwr_val = abs(pwr_val);
                  else
                      pwr_val = 255;
                      
              }
        }

        if(global_scaledata) {
            pwr_val *= 0.392;
        }
        
        if(!global_flip) {
            val = (u_int8_t)pwr_val;
        } else {
            val = (u_int8_t)(100 - pwr_val);
        }

        pwr_resp->pwr = val;
        pwr_resp++;
    }

    src_resp = (struct INTERF_SRC_RSP*)pwr_resp;
    src_resp->count = htons(interf->count);
    for (i=0; i < interf->count; i++) {
        interf_resp = &src_resp->interf[i];
        interf_resp->interf_min_freq = htons(interf->interf[i].interf_min_freq);
        interf_resp->interf_max_freq = htons(interf->interf[i].interf_max_freq);
        interf_resp->interf_type = interf->interf[i].interf_type;
        //printf("freq %d (%d) interf type =%d\n", freq, i, (int)interf_resp->interf_type);
    }

    *nbytes = resp_alloc_size;
    return (char *)rsp;

}

u_int8_t *build_single_freq_samp_rsp (u_int16_t freq, int *nbytes, u_int8_t *bin_pwr, int pwr_count)
{

    int resp_alloc_size = sizeof(struct TLV);
    struct SAMPLE_RSP *sample_rsp=NULL;
    struct FREQ_PWR_RSP *pwr_resp=NULL;
    struct INTERF_SRC_RSP *src_resp=NULL;
    struct INTERF_RSP *interf_resp=NULL;
    u_int16_t num_elems,tlv_len;
    int i;
    struct TLV *rsp;

    resp_alloc_size = sizeof(struct TLV);

    resp_alloc_size += (sizeof(struct SAMPLE_RSP) + sizeof(struct INTERF_SRC_RSP));
    resp_alloc_size += (128)*(sizeof(struct FREQ_PWR_RSP)) + sizeof(struct INTERF_RSP);

    rsp = (struct TLV *)malloc(resp_alloc_size); 
    rsp->tag = SAMP_RESPONSE;
    rsp->len = resp_alloc_size - 3;
    sample_rsp = (struct SAMPLE_RSP*)rsp->value;

    sample_rsp->freq = htons(freq);

    sample_rsp->sample_count = htons(128);
    
    pwr_resp = (struct FREQ_PWR_RSP*)sample_rsp->samples;
    for (i=0; i < pwr_count; i++) {
        pwr_resp->pwr = *(bin_pwr + i);
        pwr_resp++;
    }

    src_resp = (struct INTERF_SRC_RSP*)pwr_resp;
    src_resp->count = htons(1);
    interf_resp = (struct INTERF_RSP*)src_resp->interf;
    interf_resp->interf_min_freq = sample_rsp->freq;
    interf_resp->interf_max_freq = sample_rsp->freq;
    interf_resp->interf_type = INTERF_NONE;
    *nbytes = resp_alloc_size;
    return (char *)rsp;

}

void print_samp_msg (SPECTRAL_SAMP_MSG *samp)
{
    int i=0;
    SPECTRAL_SAMP_DATA *data = &(samp->samp_data);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"SPECTRAL_SAMP_MSG ");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"freq=%d \n", samp->freq);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"comb_rssi=%d\n", data->spectral_combined_rssi);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"max_scale=%d\n", data->spectral_max_scale);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"maxindex=%d\n", data->spectral_max_index);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"maxmag=%d\n", data->spectral_max_mag);
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"maxexp=%d\n", data->spectral_max_exp);
}

u_int8_t *parse_samp_msg(u_int8_t *buf, int *nbytes, int recv_fd, int listener_fd, int netlink_fd)
{
    int alloc_size = sizeof(struct TLV);
    int resp_alloc_size = sizeof(struct TLV);
    struct DATA_REQ_VAL *rval=NULL;
    struct SAMPLE_RSP *sample_rsp=NULL;
    struct FREQ_PWR_RSP *pwr_resp=NULL;
    struct INTERF_SRC_RSP *src_resp=NULL;
    struct INTERF_RSP *interf_resp=NULL;
    struct FREQ_BW_REQ *req=NULL;
    u_int16_t num_elems,tlv_len;
    int i, j;
    u_int8_t *tmp = NULL;
    struct TLV *rsp, *tlv;
    u_int16_t change_freq;
    char cmd[256]={'\0'};

    int fdmax=0;
    fd_set childfd;
    struct sockaddr_nl src_addr, dest_addr;
    socklen_t fromlen;
    struct nlmsghdr *nlh = NULL;
    int read_bytes, change_chan=3;
    struct msghdr msg;
    int num_responses_reqd=0, num_to_save;
    SPECTRAL_SAMP_MSG *nl_samp_msg;
    SPECTRAL_SAMP_DATA *nl_samp_data;

    tlv = (struct TLV*)buf;
    tlv_len = htons(tlv->len);
    rval = (struct DATA_REQ_VAL*)tlv->value;
    req = (struct FREQ_BW_REQ*)rval->data;
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"tlv->tag=%d tlv->len=%u rval->count=%u\n", tlv->tag, tlv->len, rval->count);
    num_elems = ntohs(rval->count);

    resp_alloc_size = sizeof(struct TLV);

    resp_alloc_size += (sizeof(struct SAMPLE_RSP) + sizeof(struct INTERF_SRC_RSP));
    resp_alloc_size += (128)*(sizeof(struct FREQ_PWR_RSP)) + sizeof(struct INTERF_RSP);
    rsp = (struct TLV *)malloc(resp_alloc_size); 
    rsp->tag = SAMP_RESPONSE;
    rsp->len = resp_alloc_size - 3;
    sample_rsp = (struct SAMPLE_RSP*)rsp->value;

    for (j=0; j < rval->count; j++) {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"[%d]req->freq = %u\n", j, req->freq);
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"[%d]req->bw = %s\n", j, (req->bw==BW_20)? "20MHz":"40MHz");
        change_freq = req->freq;
        stop_scan();
    
    
        if(global_changechannel) {

            global_prev_freq = global_current_freq;
            global_current_freq = change_freq;

            if (change_freq == 2422)
                change_chan=5;
            if (change_freq == 2462)
                change_chan=13;
#ifdef IWCONFIG_FREQUENCY
            sprintf(cmd,"%s %1d.%3dG","iwconfig ath0 freq", (change_freq/1000),(change_freq%1000));
#else
            sprintf(cmd,"%s %1d","iwconfig ath0 channel", change_chan);

#endif
            system(cmd);
         }


         nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG)));
         nlh->nlmsg_len = NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG));
         nlh->nlmsg_pid = getpid();
         nlh->nlmsg_flags = 0;

         iov.iov_base = (void *)nlh;
         iov.iov_len = nlh->nlmsg_len;

         memset(&dst_addr, 0, sizeof(dst_addr));
         dst_addr.nl_family = PF_NETLINK;
         dst_addr.nl_pid = 0;  /* self pid */

         /* interested in group 1<<0 */
         dst_addr.nl_groups = 1;
         memset(&msg, 0, sizeof(msg));
         msg.msg_name = (void *)&dst_addr;
         msg.msg_namelen = sizeof(dst_addr);
         msg.msg_iov = &iov;
         msg.msg_iovlen = 1;

         SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"Waiting for spectraldata from kernel netlink_fd=%d\n", netlink_fd);

         FD_ZERO(&childfd);
         FD_SET(netlink_fd,&childfd);
         if (netlink_fd > fdmax) {    // keep track of the maximum
             fdmax = netlink_fd;
         }   
         send_single = 0;
         if (tlv->tag == SAMP_FAST_REQUEST) {
             strcpy(cmd, "spectraltool startscan");
             num_responses_reqd=1;
             num_to_save=0;
         }
         else {
             strcpy(cmd, "spectraltool startclassifyscan");
             global_is_classify=1;
             num_responses_reqd=10;
             num_to_save=4;
         }
         system(cmd);

         while (1) {
             if (select(fdmax+1, &childfd, NULL, NULL, NULL) == -1) {
                 perror("select");
                 exit(1);
             }
             for(i = 0; i <= fdmax; i++) {
                 if (FD_ISSET(i, &childfd)) { // we got one!!
                     if (i == netlink_fd) {

                         fromlen = sizeof(src_addr);
                         memset(nlh, 0, NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG)));
                         memset(&msg, 0, sizeof(msg));
                         msg.msg_name = (void *)&dst_addr;
                         msg.msg_namelen = sizeof(dst_addr);
                         msg.msg_iov = &iov;
                         msg.msg_iovlen = 1;
                         recvmsg(netlink_fd, &msg, MSG_WAITALL);

                        nl_samp_msg = (SPECTRAL_SAMP_MSG*)NLMSG_DATA(nlh);
                        nl_samp_data = &nl_samp_msg->samp_data;


                        /* 
                         * Received Spectral sample message from Kernel
                         * pass it to classifier to check for interference
                         *
                         */
                        process_kernel_spectral_msg(nl_samp_msg);

                        if (send_single < num_responses_reqd && nlh->nlmsg_len) {

                             SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1," Received message payload:\
                                              %d bytes freq=%u send_single=%d\n",\
                                              nlh->nlmsg_len, nl_samp_msg->freq, send_single);
                             print_samp_msg (nl_samp_msg);
        
                             /* 
                              * Aug 27 Now for center freq of 2422, 
                              * actually use channel 5 pr 2432 so 
                              * modify the check for frequency mismatch 
                              * appropriately
                              *
                              */
                             if(global_changechannel) {
                                if (global_current_freq != nl_samp_msg->freq - 10) {
                                  SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1, "Frequency Mismathc");
                                  system(cmd);
                                  break; 
                                 }
                             }


                             if(send_single > num_to_save) {
                                 SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Saving response for freq %u\n", nl_samp_msg->freq);
                                 save_this_samp_msg(global_current_freq, nl_samp_msg);
                                 send_single++;
                                 break;
                             }


                             if(global_changechannel && global_is_classify && (global_current_freq == 2462)) {

                                        tmp = build_fake_samp_rsp (global_prev_freq, 
                                                                   nl_samp_data->bin_pwr_count, 
                                                                   255, 
                                                                   nbytes, 
                                                                   &nl_samp_data->interf_list);

                                        if(tmp) {
                                             if (send(recv_fd, tmp, *nbytes, 0) == -1)
                                                 perror("send");
                                         total_send++;
                                         free(tmp);
                                         tmp = NULL;     
                                         } else 
                                             SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d no saved msg to send\n");
                             }

                             tmp = build_single_samp_rsp (nl_samp_msg->freq, 
                                                          nbytes, 
                                                          nl_samp_data->bin_pwr, 
                                                          nl_samp_data->bin_pwr_count, 
                                                          nl_samp_data->spectral_combined_rssi, 
                                                          nl_samp_data->spectral_lower_rssi, 
                                                          nl_samp_data->spectral_upper_rssi, 
                                                          nl_samp_data->spectral_max_scale, 
                                                          nl_samp_data->spectral_max_mag, 
                                                          &nl_samp_data->interf_list);

                             SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Assemble response for freq %u\n", nl_samp_msg->freq);

                             if (send(recv_fd, tmp, *nbytes, 0) == -1) {
                                         perror("send");
                             } else {
                                 send_single++;
                                 total_send++;
                                 free(tmp);
                                 tmp = NULL;     
                                 if( global_changechannel && 
                                     global_is_classify && 
                                     (global_current_freq == 2422)) {
                                    
                                     tmp = build_fake_samp_rsp (global_prev_freq, 
                                                                nl_samp_data->bin_pwr_count,
                                                                255, 
                                                                nbytes, 
                                                                &nl_samp_data->interf_list);
                                    if(tmp) {
                                        if (send(recv_fd, tmp, *nbytes, 0) == -1)
                                             perror("send");
                                         total_send++;
                                         free(tmp);
                                         tmp = NULL;     
                                    } else SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d no saved msg to send\n");
                                 }
                                 break;
                             }
                         free(tmp);
                         tmp = NULL;     
                     } 
                    } /* end if (i == netlink_fd) */ 
                } /* end of if */
            } /* end of for */

            if (send_single == num_responses_reqd) {
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"NEXT FREQ total sent=%d\n", num_responses_reqd);
                break;
            } else SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,".");
      
         } /* end of while */

        req++;
        free((char*)nlh);
    } /* end of for */

    sample_rsp->sample_count = htons(128);
    pwr_resp = (struct FREQ_PWR_RSP*)sample_rsp->samples;
    for (i=0; i < 128; i++) {
        pwr_resp->pwr = 50;
        pwr_resp++;
    }

    src_resp = (struct INTERF_SRC_RSP*)pwr_resp;
    src_resp->count = htons(1);
    interf_resp = (struct INTERF_RSP*)src_resp->interf;
    interf_resp->interf_min_freq = sample_rsp->freq;
    interf_resp->interf_max_freq = sample_rsp->freq;
    interf_resp->interf_type = INTERF_NONE;

    rsp->len = htons((resp_alloc_size - 3));
    *nbytes = resp_alloc_size;

    return (char *)rsp;
    return NULL;
}

void send_samp_resp(int send_fd, int listener, u_int8_t *tmp, int *nbytes)
{
    if (send_fd != listener) {
        if (send(send_fd, tmp, *nbytes, 0) == -1) {
            perror("send");
        }
    } 
}


int main(int argc, char *argv[])
{
    int listener, new_fd, newfd, addrlen;   // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;             // my address information
    struct sockaddr_in remoteaddr;          // connector's address information
    int sin_size, fdmax, i, nbytes, yes, j, nl_sock_fd;
    u_int8_t buf[MAX_PAYLOAD]={'\0'}, *tmp=NULL;;
    fd_set master;
    fd_set read_fds;
    struct sockaddr_nl src_addr, dest_addr;
    socklen_t fromlen;
    struct nlmsghdr *nlh = NULL;
    int read_bytes, paramvalue;
    struct msghdr msg;
    u_int8_t param[255]={'\0'};
    int on=0;

    SPECTRAL_SAMP_MSG *nl_samp_msg;
    SPECTRAL_SAMP_DATA *nl_samp_data;

    /* initialize the classifier related 
     * data structures
     */
    init_classifier_data();


    i = 1;

    while (i < argc) {

        if (streq(argv[i], "daemon"))
            is_daemon = 1;
        if(streq(argv[i], "rawfft")) 
            global_rawfft=1;
        if(streq(argv[i], "scaledata")) 
            global_scaledata=1;
        if(streq(argv[i], "userssi")) 
            global_userssi=1;
        if(streq(argv[i], "flip")) 
            global_flip=1;
        if(streq(argv[i], "indexdata")) 
            global_indexdata=1;
        if(streq(argv[i], "onlyrssi")) 
            global_onlyrssi=1;
        if(streq(argv[i], "channels")) 
            global_changechannel=1;
        if(streq(argv[i], "debug"))  {
            spectral_debug_level=(ATH_DEBUG_SPECTRAL << (atoi(argv[i+1])));         
                i++;
            }
        if(streq(argv[i], "minpwr"))  {
            global_minpwr=(atoi(argv[i+1]));         
                i++;
            }
            i ++;;
    }

    if (is_daemon)
        daemonize();

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"rawfft=%d scaledata=%d userssi=%d (100-x)=%d indexdata=%d onlyrssi=%d \
                                         change_channels=%d minpwrlevel=%d\n", 
                                         global_rawfft, global_scaledata, 
                                         global_userssi, global_flip, 
                                         global_indexdata, global_onlyrssi, 
                                         global_changechannel, global_minpwr);

    listener = socket(PF_INET, SOCK_STREAM, 0); // do some error checking!

    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) { 
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"socket option failed\n");
        exit (0);
    }

    memset(&my_addr, 0, sizeof(my_addr));

    /* interested in group 1<<0 */
    my_addr.sin_family = AF_INET;           // host byte order
    my_addr.sin_port = htons(ATHPORT);      // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY;   // auto-fill with my IP

    // don't forget your error checking for these calls:
    bind(listener, (struct sockaddr *)&my_addr, sizeof(my_addr));
    listen(listener, BACKLOG);
    sin_size = sizeof (remoteaddr);
    nl_sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_ATHEROS);

    if (nl_sock_fd < 0) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"socket errno=%d\n", nl_sock_fd);
            return nl_sock_fd;
    }

    // send_listener_msg(sock_fd);
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = PF_NETLINK;
    src_addr.nl_pid = getpid();  /* self pid */
    /* interested in group 1<<0 */
    src_addr.nl_groups = 1;
    
    if(read_bytes=bind(nl_sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr)) < 0) {
        if (read_bytes < 0)
            perror("bind(netlink)");
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"BIND errno=%d\n", read_bytes);
        return read_bytes;
    }

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG)));
    nlh->nlmsg_len = NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG));
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.nl_family = PF_NETLINK;
    dst_addr.nl_pid = 0;  /* self pid */

    /* interested in group 1<<0 */
    dst_addr.nl_groups = 1;
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&dst_addr;
    msg.msg_namelen = sizeof(dst_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(nl_sock_fd, &master);
    FD_SET(listener,&master);

    fdmax = ((listener > nl_sock_fd) ? listener : nl_sock_fd);

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"DEMO Server is up ... built at\
                     %s %s\n", __DATE__, __TIME__);
    clear_saved_samp_msgs();

    for(;;) {

           read_fds = master; // copy it

           if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
               perror("select");
               exit(1);
           }

           // run through the existing connections looking for data to read
           for(i = 0; i <= fdmax; i++) {
               if (FD_ISSET(i, &read_fds)) { // we got one!!
                   if (i == listener) {
                       // handle new connections
                       addrlen = sizeof(remoteaddr);
                       if ((newfd = accept(listener, \
                           (struct sockaddr *)&remoteaddr, &addrlen)) == -1) { 

                           perror("accept");
                       } else {
                           FD_SET(newfd, &master); // add to master set
                           if (newfd > fdmax) {    // keep track of the maximum
                               fdmax = newfd;
                           }
                           SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"DEMOserver:\
                                            new connection from %s on " \
                                            "socket %d\n", \
                                            inet_ntoa(remoteaddr.sin_addr), newfd);
                       }
                   } else {

                      // handle data sent up by kernel on the netlink socket
                      if (i == nl_sock_fd) {
                         /*
                          * XXX : Review this path
                          *
                          */


                         fromlen = sizeof(src_addr);
                         memset(nlh, 0, NLMSG_SPACE(sizeof(SPECTRAL_MSG)));
                         memset(&msg, 0, sizeof(msg));
                         msg.msg_name = (void *)&dst_addr;
                         msg.msg_namelen = sizeof(dst_addr);
                         msg.msg_iov = &iov;
                         msg.msg_iovlen = 1;
                         recvmsg(nl_sock_fd, &msg, MSG_WAITALL);


                         nl_samp_msg = (SPECTRAL_SAMP_MSG*)NLMSG_DATA(nlh);
                         nl_samp_data = &nl_samp_msg->samp_data;
                         process_kernel_spectral_msg(nl_samp_msg);

                         continue;
                      } 

                      // handle data from a client
                      if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                          // got error or connection closed by client
                          if (nbytes == 0) {
                              // connection closed
                              SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"selectserver: socket %d hung up\n", i);
                              SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"Stop spectral scan ....\n");
                              stop_scan();
                          } else {
                              perror("recv");
                          }
                          close(i); // bye!
                          FD_CLR(i, &master); // remove from master set
                      } else {
                          SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"REQ from the client %d bytes \n", nbytes);    
                          SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"clientfd=%d listener=%d netlinkfd=%d\n", i, listener, nl_sock_fd);

                          tmp=parse_samp_msg(buf, &nbytes, i, listener, nl_sock_fd);
                          free(tmp);
                          tmp=NULL;
                          continue;

                          // we got some data from a client
                          for(j = 0; j <= fdmax; j++) {
                              // send to everyone!
                              if (FD_ISSET(j, &master)) {
                                  // except the listener and ourselves
                                  if (j != listener) {
                                      SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Sending the client %d bytes \n", nbytes);    
                                      if (send(j, tmp, nbytes, 0) == -1) {
                                          perror("send");
                                      }
                                  }
                              }
                          }
                      }
                  } // it's SO UGLY!
              }
          }
    }

    close(listener);
    close(newfd);
    free(nlh);

    system("free");
    return 0;
}

void psamp_msg(SPECTRAL_SAMP_MSG *samp)
{
    int i=0;
    SPECTRAL_SAMP_DATA *data = &(samp->samp_data);
    printf("SPECTRAL_SAMP_MSG ");
    printf("freq=%d \n", samp->freq);
    printf("datalen = %d\n", data->spectral_data_len);
    printf("rssi = %d\n", data->spectral_rssi);
    printf("combined rssi = %d\n", data->spectral_combined_rssi);
    printf("upper rssi = %d\n", data->spectral_upper_rssi);
    printf("lower rssi = %d\n", data->spectral_lower_rssi);
    printf("max scale= %d\n", data->spectral_max_scale);
    printf("bwinfo = %d\n", data->spectral_bwinfo);
    printf("max index = %d\n", data->spectral_max_index);
    printf("time stamp = %d\n", data->spectral_tstamp);
    printf("bin pwr count = %d\n", data->bin_pwr_count);
    printf("max exp = %d\n", data->spectral_max_index);
    printf("maxindex=%d\n", data->spectral_max_index);
    printf("maxmag=%d\n", data->spectral_max_mag);
    printf("maxexp=%d\n", data->spectral_max_exp);


    for (i = 0; i < 8; i++)
        printf("%d ", data->bin_pwr[i]);

    printf("\n");
}

/* 
 * function : init_classifier_data()
 * initialize the classifier related data
 * the classifier related structures is global
 */

void init_classifier_data()
{
    init_bandinfo(&lwrband, &uprband, 1);
    reset_vbr(&lwrband);
    reset_vbr(&uprband);
    reset_bth(&lwrband);
    reset_bth(&uprband);
    reset_bts(&lwrband);
    reset_bts(&uprband);
    reset_cph(&lwrband);
    reset_cph(&uprband);
    reset_cwa(&lwrband);
    reset_cwa(&uprband);
    reset_mwo(&lwrband);
    reset_mwo(&uprband);
    reset_wln(&lwrband);
    reset_wln(&uprband);

}

int interference_found(struct ss* lwrband, struct ss* uprband)
{

    int found = 0;

    if (lwrband->count_bth || lwrband->count_bts ||
        lwrband->count_cph || lwrband->count_mwo ||
        lwrband->count_vbr || lwrband->count_cwa ||
        lwrband->count_wln || uprband->count_bth ||
        uprband->count_bts || uprband->count_cph ||
        uprband->count_mwo || uprband->count_vbr ||
        uprband->count_cwa || uprband->count_wln
        ) {
            found = 1;
     }

     return found;
}     

void process_kernel_spectral_msg(SPECTRAL_SAMP_MSG* msg) 
{

    SPECTRAL_SAMP_DATA *data;
    static int once_only=0;

    u_int32_t is_ht2040 = 0;

    u_int32_t timeDiff;
    u_int32_t loopCnt;

    u_int32_t primRssi;
    //u_int32_t extRssi;
    u_int32_t narrowband_lwr = 0;
    u_int32_t peak_index = 0;
    u_int32_t narrowband_upr = 0;

    time_t time;

    /* init the classifier information */
    data = &msg->samp_data;
    is_ht2040 = data->spectral_data_len > 100 ? 1 : 0;

    prevTimeStamp = data->spectral_last_tstamp;
    narrowband_lwr = data->spectral_nb_lower;
    narrowband_upr = data->spectral_nb_upper;

    peak_index = data->spectral_lower_max_index;
    primRssi = data->spectral_lower_rssi;

    localtime(&time);
    if(!once_only) {
        init_bandinfo(&lwrband, &uprband, 1);
        ms_init_classifier(&lwrband, &uprband, &data->classifier_params);
#if 0
        printf("%s %d %d %d\n",
          ctime(&time), is_ht2040, lwrband.b_chan_in_mhz, lwrband.dc_index);
#endif
        once_only++;
    }

    if (is_ht2040) {
#if 0
        printf("%d %d %d %d %d %d %d %d %d\n",
               data->spectral_tstamp, data->spectral_lower_rssi,
               narrowband_lwr, data->spectral_lower_max_index,
               data->spectral_upper_rssi, narrowband_upr,
               data->spectral_upper_max_index, data->spectral_combined_rssi);
#endif
    } else {
        if (!data->spectral_tstamp) {
#if 0
            printf("%d %d %d %d 0 0 0 %d\n",
                   data->spectral_tstamp, data->spectral_lower_rssi,
                   narrowband_lwr, data->spectral_lower_max_index,
                   data->spectral_combined_rssi);
#endif
            return;
        }
    }

    classifier(&lwrband, data->spectral_tstamp, prevTimeStamp, primRssi,
               narrowband_lwr, peak_index);

    if (is_ht2040) {
        /* handle the dynamic ht2040 case here */
        peak_index = data->spectral_upper_max_index;
        primRssi = data->spectral_upper_rssi;
        /* handle the dynamic ht2040 case here */
        classifier(&uprband, data->spectral_tstamp, prevTimeStamp, primRssi,
                    narrowband_upr, peak_index); 
    }

    prevTimeStamp = data->spectral_tstamp;
#if 0
    if (!is_daemon && interference_found(&lwrband, &uprband)) {
        printf("    BTH         BTS         CPH         MWO         VBR         CWA    RSSI GAP \n");
        printf("%4d %4d | %4d %4d | %4d %4d | %4d %4d | %4d %4d | %4d %4d | %4d %4d | RSSI = %d, %d\n",  
              lwrband.count_bth, uprband.count_bth,
              lwrband.count_bts, uprband.count_bts, 
              lwrband.count_cph, uprband.count_cph, 
              lwrband.count_mwo, uprband.count_mwo,
              lwrband.count_vbr, uprband.count_vbr, 
              lwrband.count_cwa, uprband.count_cwa,
              lwrband.count_wln, uprband.count_wln,
              (lwrband.average_rssi_total/lwrband.average_rssi_numbr),
              (uprband.average_rssi_total/uprband.average_rssi_numbr)
         );
     }
#endif
}     


#undef streq

