#include "spectral_defs.h"

struct INTERF_RSP global_interf_rsp[MAX_INTERF_COUNT];
int global_interf_count=0;

void clear_global_interf_rsp()
{
    int i=0;
    static int num_resets=0;

    /* We want to clear up the interference reports once for every sweep of the spectrum on the GUI, so once after every two channel changes. This makes sure that when an interfer is in one channel but not in the other, it still shows up on the GUI. And then when the interferer goes away, it disappears from the GUI */
    if ((num_resets % 2) == 0) {
        global_interf_count=0;
        for (i=0;i<MAX_INTERF_COUNT; i++) {
            global_interf_rsp[i].interf_type=INTERF_NONE;
            global_interf_rsp[i].interf_min_freq=0;
            global_interf_rsp[i].interf_max_freq=0;
        }
    }
    num_resets++;
}

int update_global_interf(struct ss *bd)
{
   int i=global_interf_count;
   struct INTERF_RSP *interf_resp=NULL;
    int num_types_detected=0;

    if(bd->count_mwo) {
            interf_resp = &global_interf_rsp[INTERF_MW];
            interf_resp->interf_min_freq = (bd->mwo_min_freq / 1000);
            interf_resp->interf_max_freq = (bd->mwo_max_freq / 1000);
            interf_resp->interf_type = INTERF_MW;
            num_types_detected++;
        } 

        if(bd->count_bts) {
            interf_resp = &global_interf_rsp[INTERF_BT];
            interf_resp->interf_min_freq = (bd->bts_min_freq/1000);
            interf_resp->interf_max_freq = (bd->bts_max_freq/1000);
            interf_resp->interf_type = INTERF_BT;
            num_types_detected++;
        } 

        if(bd->count_cph) {
            interf_resp = &global_interf_rsp[INTERF_DECT];
            interf_resp->interf_min_freq = (bd->cph_min_freq/1000);
            interf_resp->interf_max_freq = (bd->cph_max_freq/1000);
            interf_resp->interf_type = INTERF_DECT;
            num_types_detected++;
        } 

        if(bd->count_cwa){
            interf_resp = &global_interf_rsp[INTERF_TONE];
            interf_resp->interf_min_freq = (bd->cwa_min_freq/1000);
            interf_resp->interf_max_freq = (bd->cwa_max_freq/1000);
            interf_resp->interf_type = INTERF_TONE;
            num_types_detected++;
        } 

        i = (bd->count_mwo + bd->count_bts + bd->count_bth + bd->count_cwa + bd->count_cph);

        if (i)
            global_interf_count=i;

        if (i)
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1, "global_interf_count=%d num_types=%d\n", global_interf_count, num_types_detected);

        return num_types_detected;
}

void add_from_global_interference_report(struct INTERF_SRC_RSP *rsp)
{

    int i=0, count=0;
    struct INTERF_RSP *interf_resp=NULL;

    interf_resp = &rsp->interf[0];

    for(i=0;i<MAX_INTERF_COUNT;i++){
            if(global_interf_rsp[i].interf_type != INTERF_NONE) {
                count++;
                interf_resp->interf_min_freq = htons(global_interf_rsp[i].interf_min_freq);
                interf_resp->interf_max_freq = htons(global_interf_rsp[i].interf_max_freq);
                interf_resp->interf_type = (global_interf_rsp[i].interf_type);
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1, "(%d) interf type =%d\n", i, (int)interf_resp->interf_type);
                interf_resp++;
            }

    }
    rsp->count = htons(count);

    if(global_interf_count)
       SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1, " %s global_interf_count=%d\n", __func__, global_interf_count);
}


void spectral_print_classifier_counts(struct ss *bd, const char *print_str)
{
    if(bd->count_mwo || bd->count_bts || bd->count_bth || bd->count_cwa || bd->count_wln || global_interf_count)
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL1,"%s FREQ=%d\n", print_str, bd->dc_in_mhz);
    if(bd->count_mwo)
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "count_mwo=%d\n",bd->count_mwo);
    if(bd->count_bts) {
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "count_bts=%d ",bd->count_bts);
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "bts_min_freq=%d ",bd->bts_min_freq);
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "bts_max_freq=%d\n",bd->bts_max_freq);
    }
    if(bd->count_bth)
    SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "count_bth=%d\n",bd->count_bth);
    if(bd->count_cph) {
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "count_cph=%d ",bd->count_cph);
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "cph_min_freq=%d ",bd->cph_min_freq);
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "cph_max_freq=%d\n",bd->cph_max_freq);
    }
    if(bd->count_cwa){
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "count_cwa=%d\n",bd->count_cwa);
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "cwa_min_freq=%d ",bd->cwa_min_freq);
        SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "cwa_max_freq=%d\n",bd->cwa_max_freq);
    }
    if(bd->count_wln)
    SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "count_wln=%d\n",bd->count_wln);
    if(bd->count_vbr)
    SPECTRAL_DPRINTF( ATH_DEBUG_SPECTRAL2, "count_vbr=%d\n",bd->count_vbr);

}

void print_detection(struct ss *lower_band, struct ss* upper_band)
{
    clear_global_interf_rsp();
    global_interf_count = update_global_interf(lower_band);
    global_interf_count += update_global_interf(upper_band);
}


void add_fake_interference_report(struct INTERF_SRC_RSP *rsp)
{

    int i=0, count=0;
    struct INTERF_RSP *interf_resp=NULL;
    static int debug_counter=0;
    static int interf_type = (int)(INTERF_NONE);

    debug_counter++;

    if((debug_counter%5)==0) {
        interf_type += 1;
        if (interf_type > INTERF_OTHER)
            interf_type = INTERF_NONE;
    }

    for(i=0;i<INTERF_OTHER;i++){
        count++;
        interf_resp = &rsp->interf[i];
        interf_resp->interf_min_freq = global_current_freq;
        interf_resp->interf_max_freq = global_current_freq;
        interf_resp->interf_type = (int)(interf_type);
        printf("(%d) interf type =%d\n", i, (int)interf_resp->interf_type);
    }
    rsp->count = htons(count);
}

