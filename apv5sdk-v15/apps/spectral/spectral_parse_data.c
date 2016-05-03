#include "spectral_defs.h"

void server_process_kernel_spectral_msg(SPECTRAL_SAMP_MSG* msg) 
{

#define MAX_TIME_DIFF (100000)
    SPECTRAL_SAMP_DATA *data;

    struct ss* p_lwrband;
    struct ss* p_uprband;
    static int once_only=0;
    static int curr_freq=2412;

    u_int32_t is_ht2040 = 0;

    u_int32_t prevTimeStamp = 0;
    u_int32_t timeDiff;
    u_int32_t loopCnt;

    u_int32_t primRssi;
    int narrowband_lwr = 0;
    int peak_index = 0;
    int narrowband_upr = 0;


    data = &msg->samp_data;
    is_ht2040 = data->spectral_data_len > 100 ? 1 : 0;

    prevTimeStamp = data->spectral_last_tstamp;
    narrowband_lwr = data->spectral_nb_lower;
    narrowband_upr = data->spectral_nb_upper;

    peak_index = data->spectral_lower_max_index;
    primRssi = data->spectral_lower_rssi;

    if((global_do_eacs) && (msg->cw_interferer == 1)) {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s %d DYNAMIC channel change is required\n", __func__, __LINE__);
        start_eacs_scan();
        sleep(10);
        start_eacs_monitor_scan();
        return;
    }

    if(!once_only || (msg->freq != curr_freq)) {
        init_bandinfo(&lwrband, &uprband, 1);
        ms_init_classifier(&lwrband, &uprband, &data->classifier_params);
        once_only++;
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d msg->freq=%d curr_freq = %d\n", __func__, __LINE__, msg->freq, curr_freq);
        curr_freq = msg->freq;
    }

    classifier(&lwrband, data->spectral_tstamp, prevTimeStamp, primRssi,
               narrowband_lwr, peak_index);

    if (is_ht2040) {
        /* handle the dynamic ht2040 case here */
        peak_index = data->spectral_upper_max_index;
        primRssi = data->spectral_upper_rssi;
        classifier(&uprband, data->spectral_tstamp, prevTimeStamp, primRssi,
                    narrowband_upr, peak_index); 
    }

    prevTimeStamp = data->spectral_tstamp;
    print_detection(&lwrband, &uprband);
    if (global_do_eacs) {



        if (is_dynamic_chan_change_reqd(msg->freq_loading, data->spectral_tstamp)) {
             SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s %d DYNAMIC channel change is required\n", __func__, __LINE__);
            start_eacs_scan();
            exit(0);
        }
        if (is_interference_detected(&lwrband)) {
             SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s %d DYNAMIC channel change is required\n", __func__, __LINE__);
            start_eacs_scan();
            exit(0);
        } else if (is_interference_detected(&uprband)) {
             SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s %d DYNAMIC channel change is required\n", __func__, __LINE__);
            start_eacs_scan();
            exit(0);
        }

    } // end if(global_eacs)
}

void classify_mode_server_send_samp_data_to_gui(SPECTRAL_SAMP_MSG* nl_samp_msg, int send_fd){
        /*If a classify scan, then we stay in the same channel for a minute, so send fake data for the other channel. If we are the higher channel, send saved data BEFORE sending our own data */
        if (global_is_classify) {
            if (global_num_alarms>= 5) {
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"%s %d num_saved=%d num_sent=%d, num_alarms=%d\n", __func__, __LINE__, num_saved, num_sent, global_num_alarms);
                switch_channel();
                start_scan();

            }
            if (can_send_saved_messages()) {
                send_saved_msg_to_gui(send_fd, lower.pool_ptr, lower.freq);
                send_saved_msg_to_gui(send_fd, upper.pool_ptr, upper.freq);
                num_sent++;
            } else {
                    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d NO SAVED TO SEND num_saved=%d num_sent=%d, total_channel_switches=%d\n", __func__, __LINE__, num_saved, num_sent, total_channel_switches);
            }
        }
}

void server_send_samp_data_to_gui(SPECTRAL_SAMP_MSG* nl_samp_msg, int send_fd) 
{
    SPECTRAL_SAMP_MSG *msg_to_save = nl_samp_msg;
    int32_t tstamp = msg_to_save->samp_data.spectral_tstamp;
    int16_t rssi = msg_to_save->samp_data.spectral_rssi;

    if (num_sent < num_responses_reqd) {
        alarm(0);
    }

    if (msg_to_save->freq == GUI_LOWER_FREQ) {

        /* This is the first spectral report received for this channel, in this maxhold interval */

        if (lower_last_tstamp==0) 
            lower_last_tstamp=tstamp;

        if (rssi > lower_maxrssi) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d nexmaxrssi=%d oldmax=%d\n", __func__, __LINE__, lower_maxrssi, rssi);
            lower_maxrssi=rssi;
            memcpy(&lower_max_rssi_msg, msg_to_save, sizeof(SPECTRAL_SAMP_MSG));
        }

        /* We have exceeded our maxhold interval so now save the current max_rssi_msg to the pool for sending to the GUI */
        if((tstamp - lower_last_tstamp) >= global_maxhold_interval) {
           server_send_maxrssi_samp_data_to_gui(&lower_max_rssi_msg, send_fd);
            /* Maxrssi message srny, so reset max_rssi and tstamp */
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d send this one with rssi=%d tstamp_diff=%d\n", __func__, __LINE__, lower_maxrssi, (tstamp-lower_last_tstamp));
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
           server_send_maxrssi_samp_data_to_gui(&upper_max_rssi_msg, send_fd);
           SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d send this one with rssi=%d tstamp_diff=%d\n", __func__, __LINE__, upper_maxrssi, (tstamp-upper_last_tstamp));
            /* Maxrssi message saved, so reset max_rssi and tstamp */
           upper_maxrssi=-120;
           upper_last_tstamp=0; 
        }
    }
}

void server_send_maxrssi_samp_data_to_gui(SPECTRAL_SAMP_MSG* nl_samp_msg, int send_fd) 
{
    int nbytes=0;
    SPECTRAL_SAMP_DATA *nl_samp_data;
    u_int8_t *tmp=NULL;

    if (num_sent < num_responses_reqd) {
      //  alarm(0);
        nl_samp_data = &nl_samp_msg->samp_data;
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d num_sent=%d\n", __func__, __LINE__, num_sent);

        tmp = server_build_single_samp_rsp (nl_samp_msg->freq, &nbytes, nl_samp_data->bin_pwr, nl_samp_data->bin_pwr_count, nl_samp_data->spectral_combined_rssi, nl_samp_data->spectral_lower_rssi, nl_samp_data->spectral_upper_rssi, nl_samp_data->spectral_max_scale, nl_samp_data->spectral_max_mag, &nl_samp_data->interf_list);
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Assemble response for freq %u\n", nl_samp_msg->freq);
        if (send(send_fd, tmp, nbytes, 0) == -1) {
            perror("send");
        } else{ 
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Sent %d bytes \n", nbytes);
        }
        num_sent++;

        if((!global_is_classify) && (num_sent == num_responses_reqd)) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d num_sent=%d num_reqd=%d\n", __func__, __LINE__, num_sent, num_responses_reqd);
            /* Only do a channel switch after the first channel has been scanned, the second channels after sending num_responses_reqd should just hang around waiting for the next SAMP request to come from the GUI */
            if ((total_channel_switches % 2) != 0) {
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d total_channel_switches=%d\n", __func__, __LINE__, total_channel_switches);
                switch_channel(); 
                start_scan();
            } 
        }
    } else
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d num_sent=%d\n", __func__, __LINE__, num_sent);

}

u_int8_t *server_parse_samp_data_msg(u_int8_t *buf, int *nbytes, int recv_fd, int listener_fd, int netlink_fd)
{
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;
    struct msghdr msg;
    socklen_t fromlen;
    SPECTRAL_SAMP_MSG *nl_samp_msg;
    SPECTRAL_SAMP_DATA *nl_samp_data;

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

        if (nlh->nlmsg_len) {

                             SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2," Received message payload %d bytes freq=%u send_single=%d current_freq=%d\n",\
                                              nlh->nlmsg_len, nl_samp_msg->freq, send_single, global_current_freq);
                            /* Only do classifier processing if we are doing classification or running EACS for dynamic channel change */
                            if (global_is_classify || global_do_eacs)
                                server_process_kernel_spectral_msg(nl_samp_msg);

                             if (global_is_classify) {
                            /* In classify mode, must stay in the same channel for 1 second and send data simultaneously for previous channel and current channel */
                                save_this_samp_message(nl_samp_msg);

                            } else {
                            /* In non classify mode, send data to GUI and switch channels ASAP, no need to stay for 1 second in the same channel */
                             if(nl_samp_msg->freq == (global_current_freq + 10))
                                 server_send_samp_data_to_gui(nl_samp_msg, recv_fd);
                             }
        } else {
                            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2," Received message payload %d bytes freq=%u send_single=%d current_freq=%d\n",\
                                              nlh->nlmsg_len, nl_samp_msg->freq, send_single, global_current_freq);
        }

    free((char*)nlh);
    return NULL;
}

