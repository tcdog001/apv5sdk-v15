#include "spectral_defs.h"

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
   SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d found=%d save->freq=%d\n", __func__, __LINE__, found, save->freq);   
   if(save_to) {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d freq %u cur_send=%d cur_save=%d count=%d\n", __func__, __LINE__,save_to->freq, save_to->cur_send_index, save_to->cur_save_index, save_to->count_saved);
       if (save_to->count_saved == MAX_SAMP_SAVED) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s No more space freq=%d count_saved=%d\n",__func__, save_to->freq, save_to->count_saved);
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
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s freq %u cur_send=%d cur_save=%d count=%d\n", __func__, save_to->freq, save_to->cur_send_index, save_to->cur_save_index, save_to->count_saved);
        if(save_to->cur_send_index == MAX_SAMP_SAVED) {
                save_to->cur_send_index = 0;
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d freq %u cur_send=%d cur_save=%d count=%d\n", __func__, __LINE__, save_to->freq, save_to->cur_send_index, save_to->cur_save_index, save_to->count_saved);
       }
       if(save_to->cur_save_index == MAX_SAMP_SAVED) {
            if(save_to->cur_send_index !=0) {
                save_to->cur_save_index = 0;
            } else 
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"Ran out out space freq=%d cur_save=%d cur_send=%d\n",   
                        current_freq, save_to->cur_save_index, save_to->cur_send_index);
       }
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d freq %u cur_send=%d cur_save=%d count=%d\n", __func__, __LINE__, save_to->freq, save_to->cur_send_index, save_to->cur_save_index, save_to->count_saved);
    }
}

