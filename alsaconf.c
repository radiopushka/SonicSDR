#include <alsa/asoundlib.h>
#include<stdio.h>

int configure_sound_card(snd_pcm_t* handle,unsigned int* samplerate,int* channels){
  snd_pcm_hw_params_t* aparams;
  snd_pcm_hw_params_alloca(&aparams);
  if(snd_pcm_hw_params_any(handle,aparams)<0){
    snd_pcm_hw_params_free(aparams);
    printf("unable to configure alsa device\n");
    return -1;
  }
  if(snd_pcm_hw_params_set_access(handle,aparams,SND_PCM_ACCESS_RW_INTERLEAVED)<0){
    snd_pcm_hw_params_free(aparams);
    printf("alsa unable to se access\n");
    return -1;
  }
  if(snd_pcm_hw_params_set_format(handle,aparams,SND_PCM_FORMAT_S16_LE)<0){
    snd_pcm_hw_params_free(aparams);
    printf("failed to set card format\n");
    return -1;
  }
  if(snd_pcm_hw_params_set_rate_near(handle,aparams,samplerate,0)<0){
    snd_pcm_hw_params_free(aparams);
    printf("failed to set samplerate\n");
    return -1;
  }
  if(channels>0){
    if(snd_pcm_hw_params_set_channels(handle,aparams,*channels)<0){
      int nc=1;
      if(*channels==1){
        nc=2;
      }
      *channels=nc;
      if(snd_pcm_hw_params_set_channels(handle,aparams,nc)<0){
       snd_pcm_hw_params_free(aparams);
       printf("unable to set channels\n");
       return -1;
      }
    }
  }
  if(snd_pcm_hw_params(handle,aparams)<0){
    snd_pcm_hw_params_free(aparams);
    printf("failed to configure sound card, alsa\n");
    return -1;

  }
  return 1;
}
