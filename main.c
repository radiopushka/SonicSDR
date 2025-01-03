#include "fourier.h"
#include<stdio.h>
#include<stdlib.h>
#include<alsa/asoundlib.h>
#include "colors/colors.h"
#include <math.h>
#include <locale.h>
#include "alsaconf.c"
#include "curses/curses.c"
#include "KEYS.h"
#include <sys/ioctl.h>

int SIZE=4000;
int SKIP=0;
unsigned int D_SAMPLE=48000;
int LOWEST_F=11;
int FFT_SPEED_SPEC=0;
#define FFT_SPEED_SINGLE 0 

int LINES=50;
int COLS=100;


char* DEVICE="default";

//interface stuff
  int mouse_pointer=1;
  int start_freq=0;
  int stop_freq=0;

//chtype printout[]={' '|COLOR_PAIR((1<<7)|(7&0)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&1)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&3)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&2)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&6)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&4)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&5)<<4|(7&0))};

int print_offset=0;
double amp=100.0;

pthread_mutex_t syncm;


  struct winsize w;

void update_term_size(){
  ioctl(0, TIOCGWINSZ, &w);
  LINES=w.ws_row;
  COLS=w.ws_col;
}


char* getfromdecimal(float ftout){
  int normalization=ftout*amp;
  return get_colored_string(normalization);
 }
char*** screenbuff=NULL;
int bbsize=0;
void freescrbuff(){
  if(screenbuff==NULL){
    return;
  }
  int i;
  for(i=0;i<bbsize;i++){
    free(screenbuff[i]);
  }
  free(screenbuff);
  screenbuff=NULL;
}
int ppsize=0;
void printspect(float* trans,int size){
  int i=0;
  int i2=0;
  int nsize;
  if(screenbuff==NULL){
    bbsize=LINES-2;
    screenbuff=malloc(sizeof(char*)*(bbsize+1));
    ppsize=size;
    for(i=0;i<bbsize;i++){
      screenbuff[i]=malloc(sizeof(char*)*(size+1));
      for(i2=0;i2<size+1;i2++){
        screenbuff[i][i2]=get_reset_string();
      }
    }
  }else if(ppsize!=size){
    nsize=LINES-2;
    char*** nscrn=malloc(sizeof(char*)*(nsize+1));
    for(i=0;i<nsize;i++){
      nscrn[i]=malloc(sizeof(char*)*(size+1));
      for(i2=0;i2<size;i2++){
        if(i2<ppsize&&i<bbsize){
          nscrn[i][i2]=screenbuff[i][i2];
        }else{
          nscrn[i][i2]=get_reset_string();
        }
      }
    }
    freescrbuff();
    screenbuff=nscrn;
    ppsize=size;
    bbsize=nsize;
  }
  clear();
  size_t cpsize=size*sizeof(char*);
  for(i=bbsize-1;i>0;i--){
    memcpy(screenbuff[i],screenbuff[i-1],cpsize);
      mvmove(i+1,0);
      for(i2=0;i2<size;i2++){
      	if(i2>COLS-2)
          break;
        fputs(screenbuff[i][i2],stdout);
      }

  }

  printf(get_reset_string(),stdout);

  for(i=0;i<size;i++){
    screenbuff[0][i]=getfromdecimal(trans[i]);
  }
  screenbuff[0][i]=0;
  mvmove(LINES-1,0);
  printf("gain: %d *=%dHz",(int)amp,get_freq_at_index(mouse_pointer-1));
  mvmove(0,0);
  printf("%dHz",get_freq_at_index(0));
  mvmove(0,COLS-8);
  printf("%dHz",get_freq_at_index(get_fourier_size()-1));

}

float* f16convert=NULL;
short* buffer=NULL;
int bsize=2000;
void free_ft(){
  free(f16convert);
  free(buffer);
  f16convert=NULL;
  buffer=NULL;
  free_fourier_transform();
}
double gw;
int channels=1;
void init_ft(int argn){
  free(f16convert);
  free(buffer);
  init_fourier_transform(bsize,start_freq,stop_freq,COLS-4,D_SAMPLE); 

  f16convert=malloc(sizeof(float)*bsize);
  buffer=malloc(sizeof(short)*bsize*channels);
}
void reset_ft(){
  update_term_size();
  free_fourier_transform();
  init_fourier_transform(bsize,start_freq,stop_freq,COLS-4,D_SAMPLE); 

}
int new_data=0;
int argnextrac=0;

int main(int argn,char* argv[]){

  int prev_cols=COLS;

  setlocale(LC_ALL, "");
 
  snd_pcm_t *pcm_handle;


  //ftaudio <alsa device> <sample rate> <recv buffer size> <skip buffer>
  char* aup=DEVICE;
  if(argn > 1){
    aup=argv[1];
  }

  if(argn > 2){
    D_SAMPLE=atoi(argv[2]);
  }

  if(argn > 3){
    SIZE=atoi(argv[3]);
  }

  if(argn > 4){
    SKIP=atoi(argv[4]);
  }

	
 
	if ( snd_pcm_open(&pcm_handle, aup,SND_PCM_STREAM_CAPTURE, 0) < 0){
		
		printf("unable to open device \n");
		return 0;
	} 
	if(configure_sound_card(pcm_handle,&D_SAMPLE,&channels)<0){
		snd_pcm_close(pcm_handle);
    snd_config_update_free_global();

		return 0;
	}
 
  bsize=SIZE;
  start_freq=D_SAMPLE/bsize;
  stop_freq=D_SAMPLE/2;


  setup_terminal();
  update_term_size();
  //254
  init_colors(1275);
 
  int pause=0;
  //init_colorpairs();
  //nodelay(stdw, TRUE);
  //keypad(stdw,TRUE);
	//Alsa might give us another sample rate
  char sdisp[100];
  sprintf(sdisp,"sample rate: %dkhz",D_SAMPLE/1000);
  int sslen=strlen(sdisp);//print out the sample rate at the bottom of the screen
  //noecho();
  //curs_set(0);
  mvmove(0,0);
  init_ft(argn);
  int err;
  float* ppointer;
  char c=-1;

  //pause and start
  int fptog=0;

  while(c!='q'){
    
     if ((err = snd_pcm_readi (pcm_handle, buffer, bsize)) ==-EPIPE) {
      free_ft();
      freescrbuff();
      snd_pcm_close(pcm_handle);
      snd_config_update_free_global();
      free_color_info();

      printf("%d\n",err);
      exit (1);
    }
    c=wgetch();
    if(pause > SKIP && fptog == 0){
        f16_array_to_f(buffer,bsize,f16convert,channels);
        ppointer=produce_period_gram(f16convert);

      	int psize=get_fourier_size();
        printspect(ppointer,psize);
        mvmove(LINES-1,COLS-sslen-1);
        printf("%s",sdisp);
        pause=0;
    }
    mvprint(0,mouse_pointer,"*");
    mvmove(0,mouse_pointer);
    pause++;

    update_term_size();

    if(prev_cols!=COLS){
     mouse_pointer=1;
      if(stop_freq-start_freq < COLS){
            start_freq=D_SAMPLE/bsize;
            stop_freq=D_SAMPLE/2;
      }

     reset_ft();
     prev_cols=COLS;
    }
    while(c!=-1 && c!=255){

      mvmove(LINES/2,COLS/2);
      printf("%d",c);
      switch(c){
        case ARROW_UP:
          if(amp<1000)
            amp++;

          mvmove(LINES/2,COLS/2);
          printf("gain: %d",(int)amp);
          break;

        case ARROW_DOWN:
          if(amp > 0)
            amp--;

          mvmove(LINES/2,COLS/2);
          printf("gain: %d",(int)amp);
          break;

        case ARROW_RIGHT:
          if(mouse_pointer<COLS-4)
            mouse_pointer++;

          mvmove(LINES/2,COLS/2);
          printf("pointer: %dHz",get_freq_at_index(mouse_pointer-1));
          break;
        case ARROW_LEFT:
          if(mouse_pointer>1)
            mouse_pointer--;

          mvmove(LINES/2,COLS/2);
          printf("pointer: %dHz",get_freq_at_index(mouse_pointer-1));
          break;
        case ENTER:
          stop_freq=get_freq_at_index(mouse_pointer-1);
          if(stop_freq-start_freq < COLS){
            stop_freq=start_freq+COLS;
          }
          mouse_pointer=1;
          reset_ft();
          break;
        case TAB:
          start_freq=get_freq_at_index(mouse_pointer-1);
          if(stop_freq-start_freq < COLS){
            start_freq=stop_freq-COLS;
          }

          mouse_pointer=1;
          reset_ft();
          break;
        case 'r':
          mouse_pointer=1;
          start_freq=D_SAMPLE/bsize;
          stop_freq=D_SAMPLE/2;
          reset_ft();
          break;
        case 'h':
          mvprint(LINES/2-3,COLS/2,"arrow up - increase gain");
          mvprint(LINES/2-2,COLS/2,"arrow down - decrease gain");
          mvprint(LINES/2-1,COLS/2,"arrow right - move frequency cursor right");
          mvprint(LINES/2,COLS/2,"arrow left - move frequency cursor left");
          mvprint(LINES/2+1,COLS/2,"enter - set the waterfall's max frequency");
          mvprint(LINES/2+2,COLS/2,"tab - set the waterfall's min frequency");
          mvprint(LINES/2+3,COLS/2,"r - reset the waterfall");
          mvprint(LINES/2+4,COLS/2,"q - quit");
          break;

        case ' ':
          fptog=~fptog;
          break;

          
          

          
      }
      if(c!='q')
        c=wgetch();
      else 
        break;

      mvprint(0,mouse_pointer,"*");
      mvmove(0,mouse_pointer);
      refresh();
    }
    refresh();


  }

  free_ft();
  freescrbuff();
  snd_pcm_close(pcm_handle);
  snd_config_update_free_global();
  printf("%s\n",get_reset_string());
  mvmove(0,0);
  clear();
  free_color_info();
  
  mvmove(1,0);
  printf("alsa sample rate: %d\n",D_SAMPLE);
  mvmove(2,0);
  printf("%s <alsa device> <sample rate> <recv buffer size> <skip buffer>\n",argv[0]);
  mvmove(3,0);
  printf("defaults:\n");
  mvmove(4,0);
  printf("%s default 48000 4000 0\n",argv[0]);
  mvmove(5,0);

  return 0;
}
