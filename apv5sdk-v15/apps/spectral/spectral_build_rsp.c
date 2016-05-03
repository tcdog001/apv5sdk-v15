#include "spectral_defs.h"

int8_t convert_rssi_to_dbm(int8_t rssi)
{
    return (rssi - 93);
}
// This should take the spectral data received over the netlink socket and convert it into a SAMP
// response message. This can then be posted to the listening client (GUI).
u_int8_t* server_build_single_samp_rsp (u_int16_t freq, int *nbytes, u_int8_t *bin_pwr, int pwr_count, u_int8_t rssi, int8_t lower_rssi, int8_t upper_rssi, u_int8_t max_scale, int16_t max_mag, struct INTERF_SRC_RSP *interf)
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

    lower_rssi = convert_rssi_to_dbm(lower_rssi);
    upper_rssi = convert_rssi_to_dbm(upper_rssi);
    rssi = convert_rssi_to_dbm(rssi);

    *nbytes = resp_alloc_size;
   
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1, "%s %d pwr_count=%d\n", __func__, __LINE__, pwr_count);

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"SPECTRALDEMO Reporting power levels rssi=%d max_scale=%d lower_rssi=%d upper_rssi=%d calc_maxmag= %f \n", rssi, max_scale, lower_rssi, upper_rssi, calc_maxmag);
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
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"fft val = %d pwr_val =%f calc_maxmag=%f\n",val, pwr_val, calc_maxmag);
            pwr_val -= calc_maxmag;
            pwr_val *= 20;
            if (global_userssi) {
                if (i <= 63)
                    pwr_val += lower_rssi;
                else
                    pwr_val += upper_rssi;
            }
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d fft val = %d pwr_val =%f calc_maxmag=%f\n",__func__, __LINE__, val, pwr_val, calc_maxmag);
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
        
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"line %d val =%d pwr_val=%f\n",__LINE__, val, pwr_val);
        if(!global_flip) {
            val = (u_int8_t)pwr_val;
        } else {
            val = (u_int8_t)(100 - pwr_val);
        }
        //Old implementation copies as is bin pwr data
        //pwr_resp->pwr = *(bin_pwr + i);
        pwr_resp->pwr = val;
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"[%d]=%d(%x) ", i, val, *(bin_pwr+i));
        pwr_resp++;
    }
    src_resp = (struct INTERF_SRC_RSP*)pwr_resp;

    //Tue Feb 16 13:40:58 PST 2010 - zero out the interference part of the response
    memset((void*)src_resp,0,sizeof(struct INTERF_SRC_RSP));

    src_resp->count = htons(0);
    
    /* Only add interference reporting for classify mode */
    if (global_is_classify)
        add_from_global_interference_report(src_resp);

    if(src_resp->count) {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1, "%s message for freq %d contains %d interference reports count reported is %d\n", __func__, freq, src_resp->count, src_resp->count);
        src_resp->count = htons(src_resp->count);

    }
    *nbytes = resp_alloc_size;
    return (char *)rsp;
}

