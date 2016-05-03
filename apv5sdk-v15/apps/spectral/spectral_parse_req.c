#include "spectral_defs.h"

void start_scan(void)
{
    system("spectraltool startscan");
    if (!global_is_classify)
        alarm(1);
    else
        ualarm(200000, 0);
}

void update_global_freqs(void)
{
    int temp_freq;

    temp_freq = global_current_freq;
    global_current_freq = global_prev_freq;
    global_prev_freq = temp_freq;
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d current_freq=%d prev_freq=%d\n", __func__, __LINE__, global_current_freq, global_prev_freq);
    update_saved_msg_ptrs();
}

void switch_channel(void)
{
    int change_chan;
    char cmd[256]={'\0'};

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d cancel prev alarm\n", __func__, __LINE__);
    alarm(0);
    (void)ualarm(0,0);

    //Mon Feb  8 10:46:33 PST 2010 always stop scan before changing channel
    stop_scan();

    if(global_changechannel) {
        update_global_freqs();
        if (global_current_freq == 2422)
            change_chan=5;
        if (global_current_freq == 2462)
            change_chan=13;
        sprintf(cmd,"%s %1d","iwconfig ath0 channel", change_chan);
        system(cmd);
        total_channel_switches++;

        /* Fri Feb  5 18:39:50 PST 2010 For classify scan and non classify scan, num_responses_reqs and num_to_save have different values so retain the original values set when parsing the SAMP REQUEST */
        //num_responses_reqd=1;
        //num_to_save=0;
    } else
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d Channel change is not required", __func__, __LINE__);
}



u_int8_t *server_parse_samp_req_msg(u_int8_t *buf, int *nbytes, int recv_fd, int listener_fd, int netlink_fd)
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
    SPECTRAL_SAMP_MSG *nl_samp_msg;
    SPECTRAL_SAMP_DATA *nl_samp_data;

    tlv = (struct TLV*)buf;
    tlv_len = htons(tlv->len);
    rval = (struct DATA_REQ_VAL*)tlv->value;
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
        req = (struct FREQ_BW_REQ*)rval->data;
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d[%d]req->freq = %u\n", __func__, __LINE__, j, req->freq);
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"[%d]req->bw = %s\n", j, (req->bw==BW_20)? "20MHz":"40MHz");
        if (!j)
            global_current_freq = req->freq;
        else
            global_prev_freq = req->freq;

        req++;
    }
        global_current_freq = 2462;
        global_prev_freq = 2422;

        if (tlv->tag == SAMP_FAST_REQUEST) {
            num_sent=0;
            num_responses_reqd=1;
            num_to_save=0;
            global_is_classify=0;
        } else {
            num_sent=0;
            num_responses_reqd=5;
            num_to_save=10;
            global_is_classify=1;
        }
        switch_channel();
        start_scan();

    return NULL;
}

#undef streq
