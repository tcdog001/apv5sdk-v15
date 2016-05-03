#include "spectral_defs.h"

PER_FREQ_SAMP_MSG_POOL upper, lower;

// This is the interval in which the spectral report with the highest RSSI observed will be saved and sent to the GUI
int global_maxhold_interval=5000;

SPECTRAL_SAMP_MSG lower_max_rssi_msg, upper_max_rssi_msg;
int16_t lower_maxrssi, upper_maxrssi;
int32_t lower_last_tstamp, upper_last_tstamp;

void clear_max_rssi_data(void)
{
    memset(&lower_max_rssi_msg,0,sizeof(SPECTRAL_SAMP_MSG));
    memset(&upper_max_rssi_msg,0,sizeof(SPECTRAL_SAMP_MSG));
    lower_maxrssi=-120;
    upper_maxrssi=-120;
    lower_last_tstamp=0;
    upper_last_tstamp=0;
}

int get_next_index(int cur_index)
{
    if (cur_index < (MAX_SAVED_SAMP_MSGS - 1))
        return (cur_index + 1);

    if (cur_index == (MAX_SAVED_SAMP_MSGS - 1))
        return 0;
}

int can_send_saved_messages(void)
{
    if (lower.num_valid && upper.num_valid) {
        return 1;
    }
    return 0;
}

void update_saved_msg_ptrs(void)
{
    SAVED_SAMP_MESSAGE *tmp;
    int i=0;
    num_saved=0;
    num_sent=0;
    global_num_alarms=0;
    clear_max_rssi_data();
}

void init_freq_message_pools(void)
{
    PER_FREQ_SAMP_MSG_POOL *lower_pool = &lower;
    PER_FREQ_SAMP_MSG_POOL *upper_pool = &upper;

    lower_pool->freq = GUI_LOWER_FREQ;
    lower_pool->num_valid = 0;
    init_saved_message_pool(lower_pool);

    upper_pool->freq = GUI_UPPER_FREQ;
    upper_pool->num_valid = 0;
    init_saved_message_pool(upper_pool);

}

void init_saved_message_pool(PER_FREQ_SAMP_MSG_POOL *per_freq_pool)
{
    int i=0;
    SPECTRAL_SAMP_MSG *msg_ptr;
    SAVED_SAMP_MESSAGE *msg_pool;

    per_freq_pool->pool_ptr=&per_freq_pool->msg_pool[0];
    msg_pool = per_freq_pool->pool_ptr;


    for (i=0; i< MAX_SAVED_SAMP_MSGS; i++) {
        msg_pool[i].msg_valid=0;
        msg_pool[i].current_index=MAX_SAVED_SAMP_MSGS;
        msg_ptr = &msg_pool[i].saved_msg;
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d current_saved_msgs[%d]=%p msg_ptr=%p\n", __func__, __LINE__, i,&msg_pool[i], msg_ptr);
        memset((void*)msg_ptr, 0,sizeof(SPECTRAL_SAMP_MSG));     
    }

    num_saved=0;
}

void save_this_samp_message(SPECTRAL_SAMP_MSG *msg_to_save)
{
    int32_t tstamp = msg_to_save->samp_data.spectral_tstamp;
    int16_t rssi = msg_to_save->samp_data.spectral_rssi;

    if (msg_to_save->freq == GUI_LOWER_FREQ) {

        /* This is the first spectral report received for this channel, in this maxhold interval */
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d lower_tstamp=%d\n", __func__, __LINE__, lower_last_tstamp);

        if (lower_last_tstamp==0) 
            lower_last_tstamp=tstamp;

        if (rssi > lower_maxrssi) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d nexmaxrssi=%d oldmax=%d\n", __func__, __LINE__, lower_maxrssi, rssi);
            lower_maxrssi=rssi;
            memcpy(&lower_max_rssi_msg, msg_to_save, sizeof(SPECTRAL_SAMP_MSG));
        }

        /* We have exceeded our maxhold interval so now save the current max_rssi_msg to the pool for sending to the GUI */
        if((tstamp - lower_last_tstamp) >= global_maxhold_interval) {
           save_maxrssi_samp_message(&lower_max_rssi_msg);
            /* Maxrssi message saved, so reset max_rssi and tstamp */
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d save this one with rssi=%d tstamp_diff=%d\n", __func__, __LINE__, lower_maxrssi, (tstamp-lower_last_tstamp));
           lower_maxrssi=-120;
           lower_last_tstamp=0; 
        }

    }

    if (msg_to_save->freq == GUI_UPPER_FREQ) {
        /* This is the first spectral report received for this channel, in this maxhold interval */
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d lower_tstamp=%d\n", __func__, __LINE__, upper_last_tstamp);
        if (upper_last_tstamp==0) 
            upper_last_tstamp=tstamp;

        if (rssi > upper_maxrssi) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d nexmaxrssi=%d oldmax=%d\n", __func__, __LINE__, upper_maxrssi, rssi);
            upper_maxrssi=rssi;
            memcpy(&upper_max_rssi_msg, msg_to_save, sizeof(SPECTRAL_SAMP_MSG));
        }

        /* We have exceeded our maxhold interval so now save the current max_rssi_msg to the pool for sending to the GUI */
        if((tstamp - upper_last_tstamp) >= global_maxhold_interval) {
           save_maxrssi_samp_message(&upper_max_rssi_msg);
           SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d save this one with rssi=%d tstamp_diff=%d\n", __func__, __LINE__, upper_maxrssi, (tstamp-upper_last_tstamp));
            /* Maxrssi message saved, so reset max_rssi and tstamp */
           upper_maxrssi=-120;
           upper_last_tstamp=0; 
        }
    }
}

void save_maxrssi_samp_message(SPECTRAL_SAMP_MSG *msg_to_save)
{
    int i=0, found=0;
    SPECTRAL_SAMP_MSG *msg_ptr;
    // We want to save messages to the current pool and send messages from the previously saved pool
    SAVED_SAMP_MESSAGE *saved_ptr=NULL;

 //   debug_print_samp_msg(msg_to_save);

    if (msg_to_save->freq == GUI_LOWER_FREQ) {
        saved_ptr = lower.pool_ptr;
        lower.num_valid++;
    }

    if (msg_to_save->freq == GUI_UPPER_FREQ) {
        saved_ptr = upper.pool_ptr;
        upper.num_valid++;
    }
    
    if(!saved_ptr) {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s %d No appropriate message pool found for msg with freq=%d\n", __func__, __LINE__, msg_to_save->freq);
        return;
    }
 
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d num_saved=%d\n", __func__, __LINE__, num_saved);

    /* A valid next index will always be stored in the first element of the message pool*/

    found = saved_ptr[0].current_index;

    i = get_next_index(found);

    saved_ptr[0].current_index = i;

    msg_ptr=&saved_ptr[i].saved_msg;
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d saved_ptr[%d]=%p\n", __func__, __LINE__, i, &saved_ptr[i]);
    saved_ptr[i].msg_valid=1;
    memcpy(msg_ptr, msg_to_save, sizeof(SPECTRAL_SAMP_MSG));
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d saved_ptr[%d]=%p msg_ptr=%p msg_to_save=%p current_index=%d\n", __func__, __LINE__, i, &saved_ptr[i], msg_ptr, msg_to_save, i);

    //Fri Feb  5 18:51:58 PST 2010 - Only increment count till MAX, but continue to save
    if (num_saved < MAX_SAVED_SAMP_MSGS)
        num_saved++;
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d num_saved=%d\n", __func__, __LINE__, num_saved);
    /* Do this only once here to force the first channel switch*/
    if (num_saved >= num_to_save && (total_channel_switches==1))  {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d num_saved=%d num_sent=%d, total_channel_switches=%d\n", __func__, __LINE__, num_saved, num_sent, total_channel_switches);
        switch_channel();
        start_scan();
    }
}

void send_samp_data_to_gui(SPECTRAL_SAMP_MSG* nl_samp_msg, int send_fd) 
{
    int nbytes=0;
    SPECTRAL_SAMP_DATA *nl_samp_data=&nl_samp_msg->samp_data;
    u_int8_t *tmp=NULL;

    if (nl_samp_msg->freq == GUI_LOWER_FREQ) {
        lower.num_valid--;
    }

    if (nl_samp_msg->freq == GUI_UPPER_FREQ) {
        upper.num_valid--;
    }

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d nl_samp_msg=%p gui_fd=%d\n", __func__,__LINE__, nl_samp_msg, send_fd);

    tmp = server_build_single_samp_rsp (nl_samp_msg->freq, &nbytes, nl_samp_data->bin_pwr, nl_samp_data->bin_pwr_count, nl_samp_data->spectral_combined_rssi, nl_samp_data->spectral_lower_rssi, nl_samp_data->spectral_upper_rssi, nl_samp_data->spectral_max_scale, nl_samp_data->spectral_max_mag, &nl_samp_data->interf_list);

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s Assemble response for freq %u\n", __func__,nl_samp_msg->freq);

    if (send(send_fd, tmp, nbytes, 0) == -1) {
        perror("send");
    } else{ 
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Sent %d bytes \n", nbytes);
    }
    num_sent++;
}

void send_saved_msg_to_gui(int send_fd, SAVED_SAMP_MESSAGE *send_from_pool, int send_freq)
{

    int i=0, found=0;
    SPECTRAL_SAMP_MSG *msg_ptr;
    // We only want to send messages from the previously saved pool
    SAVED_SAMP_MESSAGE *saved_ptr=send_from_pool;

    for (i=0; i< MAX_SAVED_SAMP_MSGS; i++) {
        if (saved_ptr[i].msg_valid == 1) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d saved_ptr[%d]=%p\n", __func__, __LINE__, i, &saved_ptr[i]);
            msg_ptr=&saved_ptr[i].saved_msg;
            found=1;
            break;
        }
    }

    if (found) {
        send_samp_data_to_gui(msg_ptr,send_fd);
        saved_ptr[i].msg_valid = 0; //Invalidate the message after you send it
    } else {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d No saved message to send\n", __func__, __LINE__);
        update_dummy_samp_msg(send_freq);
        send_samp_data_to_gui(&global_fake_samp_msg, send_fd);
    }
    return;
}

u_int8_t *timeout_send_fake_samp_rsp (u_int16_t freq, int pwr_count, u_int8_t fake_pwr_val, int *nbytes, struct INTERF_SRC_RSP *interf)
{

   int resp_alloc_size = sizeof(struct TLV);
   struct SAMPLE_RSP *sample_rsp=NULL;
   struct FREQ_PWR_RSP *pwr_resp=NULL;
   struct INTERF_SRC_RSP *src_resp=NULL;
   struct INTERF_RSP *interf_resp=NULL;
   int i;
   struct TLV *rsp;
   u_int8_t *tmp=NULL, *tmp2=NULL; 
    

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

    tmp2 = (u_int8_t*)pwr_resp;
    
    *nbytes = resp_alloc_size;
    return (char *)rsp;
}

void update_dummy_samp_msg(int send_freq)
{

    int i=0;
    struct INTERF_SRC_RSP *interf_resp=NULL;
    SPECTRAL_SAMP_MSG *dummy = &global_fake_samp_msg;
    SPECTRAL_SAMP_DATA *dummy_data = &(dummy->samp_data);

    dummy->freq = send_freq;
    dummy->freq_loading = 0;
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d dummy msg freq=%d\n", __func__, __LINE__, dummy->freq);
 
}

void init_dummy_samp_msg(void)
{

    int i=0;
    struct INTERF_SRC_RSP *interf_resp=NULL;
    SPECTRAL_SAMP_MSG *dummy = &global_fake_samp_msg;
    SPECTRAL_SAMP_DATA *dummy_data = &(dummy->samp_data);

    dummy->freq = 0;
    dummy->freq_loading = 0;
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d dummy msg freq=%d\n", __func__, __LINE__, dummy->freq);
 
    dummy_data->spectral_data_len=sizeof(SPECTRAL_SAMP_MSG);

    /* When sending a "fake" SAMP message, set all the RSSI fields in the SAMP message to 
    the minpwr level passed in at the command line. Also set each bin power level to 255 */
    dummy_data->spectral_rssi=global_minpwr;
    dummy_data->spectral_combined_rssi=global_minpwr;
    dummy_data->spectral_upper_rssi=global_minpwr;
    dummy_data->spectral_lower_rssi=global_minpwr;
    dummy_data->spectral_max_scale=0;
    dummy_data->spectral_bwinfo=0;
    dummy_data->spectral_tstamp=0;
    dummy_data->spectral_max_index=0;
    dummy_data->spectral_max_mag=0;
    dummy_data->spectral_max_exp=0;
    dummy_data->spectral_last_tstamp=0;
    dummy_data->spectral_upper_max_index=0;
    dummy_data->spectral_lower_max_index=0;
    dummy_data->spectral_nb_lower=0;
    dummy_data->spectral_nb_upper=0;
    dummy_data->bin_pwr_count=128;

    for (i=0; i < 128; i++) {
        dummy_data->bin_pwr[i] = GUI_IGNORE_THIS_BIN_VALUE;
    }

    interf_resp = (struct INTERF_SRC_RSP*)(&(dummy_data->interf_list));

    memset((void*)(interf_resp), 0, sizeof(struct INTERF_SRC_RSP));

    interf_resp->count = htons(0);
}

