#include "fourier.h"
#include<stdio.h>
#include<stdlib.h>
#include<alsa/asoundlib.h>
#include "colors/ncolors.h"
#include <math.h>
#include <locale.h>
#include "alsaconf.c"

int SIZE=4000;
int SKIP=0;
unsigned int D_SAMPLE=48000;
int LOWEST_F=11;
int FFT_SPEED_SPEC=0;
#define FFT_SPEED_SINGLE 0 

char* DEVICE="default";

//interface stuff
  int mouse_pointer=0;
  int start_freq=0;
  int stop_freq=0;

//chtype printout[]={' '|COLOR_PAIR((1<<7)|(7&0)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&1)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&3)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&2)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&6)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&4)<<4|(7&0)),' '|COLOR_PAIR((1<<7)|(7&5)<<4|(7&0))};

int print_offset=0;
double amp=30.0;

pthread_mutex_t syncm;

chtype getfromdecimal(float ftout){
  int normalization=ftout*amp;
  return get_color(normalization);
 }
chtype** screenbuff=NULL;
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
  int i;
  int i2;
  int nsize;
  if(screenbuff==NULL){
    bbsize=LINES;
    screenbuff=malloc(sizeof(chtype*)*(bbsize+1));
    ppsize=size;
    for(i=0;i<bbsize;i++){
      screenbuff[i]=malloc(sizeof(chtype)*(size+1));
      for(i2=0;i2<size+1;i2++){
        screenbuff[i][i2]=' ';
      }
    }
  }else if(ppsize!=size){
    nsize=LINES;
    chtype** nscrn=malloc(sizeof(chtype*)*(nsize+1));
    for(i=0;i<nsize;i++){
      nscrn[i]=malloc(sizeof(chtype)*(size+1));
      for(i2=0;i2<size+1;i2++){
        if(i2<ppsize&&i<bbsize){
          nscrn[i][i2]=screenbuff[i][i2];
        }else{
          nscrn[i][i2]=' ';
        }
      }
    }
    freescrbuff();
    screenbuff=nscrn;
    ppsize=size;
    bbsize=nsize;
  }
  clear();
  size_t cpsize=size*sizeof(chtype);
  for(i=bbsize-1;i>0;i--){
    memcpy(screenbuff[i],screenbuff[i-1],cpsize);
    for(i2=0;i2<size+1;i2++){
      	
      mvwaddch(stdscr,i,i2,screenbuff[i][i2]);
    }
  }
  for(i=0;i<size;i++){
    screenbuff[0][i]=getfromdecimal(trans[i]);
  }
  screenbuff[0][i]=0;
  mvprintw(LINES-1,0,"gain: %d *=%dHz",(int)amp,get_freq_at_index(mouse_pointer));
  mvprintw(0,0,"%dHz",get_freq_at_index(0));
  mvprintw(0,COLS-8,"%dHz",get_freq_at_index(get_fourier_size()-1));

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
  printf("alsa sample rate: %d\n",D_SAMPLE);
  printf("%s <alsa device> <sample rate> <recv buffer size> <skip buffer>\n",argv[0]);
  printf("defaults:\n");
  printf("%s default 48000 4000 0\n",argv[0]);

  bsize=SIZE;
  start_freq=D_SAMPLE/bsize;
  stop_freq=D_SAMPLE/2;

	//start ncurses here so that we can see alsa errors
  initscr();
  start_color();

  //254
  init_color_array(254);
 
  int pause=0;
  //init_colorpairs();
  nodelay(stdscr, TRUE);
  keypad(stdscr,TRUE);
	//Alsa might give us another sample rate
  char sdisp[100];
  sprintf(sdisp,"sample rate: %dkhz",D_SAMPLE/1000);
  int sslen=strlen(sdisp);//print out the sample rate at the bottom of the screen
  noecho();
  curs_set(0);
  init_ft(argn);
  int err;
  float* ppointer;
  char c=-1;

  //pause and start
  int fptog=0;

  while(c!='q'){
    
     if ((err = snd_pcm_readi (pcm_handle, buffer, bsize)) ==-EPIPE) {
      endwin();
      free_ft();
      freescrbuff();
      snd_pcm_close(pcm_handle);
      snd_config_update_free_global();
      free_colors();

      printf("%d\n",err);
      exit (1);
    }
    c=wgetch(stdscr);
    if(pause > SKIP && fptog == 0){
        f16_array_to_f(buffer,bsize,f16convert,channels);
        ppointer=produce_period_gram(f16convert);

      	int psize=get_fourier_size();
        printspect(ppointer,psize);
        mvprintw(LINES-1,COLS-sslen-1,"%s",sdisp);
        pause=0;
    }
    mvprintw(0,mouse_pointer,"*");
    pause++;
    if(prev_cols!=COLS){
     mouse_pointer=0;
      if(stop_freq-start_freq < COLS){
            start_freq=D_SAMPLE/bsize;
            stop_freq=D_SAMPLE/2;
      }

     reset_ft();
     prev_cols=COLS;
    }
    while(c!=-1){
      mvprintw(LINES/2,COLS/2,"%d",c);
      switch(c){
        case 3:
          if(amp<1000)
            amp++;
          mvprintw(LINES/2,COLS/2,"gain: %d",(int)amp);
          break;

        case 2:
          if(amp > 0)
            amp--;
          mvprintw(LINES/2,COLS/2,"gain: %d",(int)amp);
          break;

        case 5:
          if(mouse_pointer<COLS-4)
          mouse_pointer++;
          mvprintw(LINES/2,COLS/2,"pointer: %dHz",get_freq_at_index(mouse_pointer));
          break;
        case 4:
          if(mouse_pointer>0)
          mouse_pointer--;
          mvprintw(LINES/2,COLS/2,"pointer: %dHz",get_freq_at_index(mouse_pointer));
          break;
        case '\n':
          stop_freq=get_freq_at_index(mouse_pointer);
          if(stop_freq-start_freq < COLS){
            stop_freq=start_freq+COLS;
          }
          mouse_pointer=0;
          reset_ft();
          break;
        case 9:
          start_freq=get_freq_at_index(mouse_pointer);
          if(stop_freq-start_freq < COLS){
            start_freq=stop_freq-COLS;
          }

          mouse_pointer=0;
          reset_ft();
          break;
        case 'r':
          mouse_pointer=0;
          start_freq=D_SAMPLE/bsize;
          stop_freq=D_SAMPLE/2;
          reset_ft();
          break;
        case 'h':
          mvprintw(LINES/2-3,COLS/2,"arrow up - increase gain");
          mvprintw(LINES/2-2,COLS/2,"arrow down - decrease gain");
          mvprintw(LINES/2-1,COLS/2,"arrow right - move frequency cursor right");
          mvprintw(LINES/2,COLS/2,"arrow left - move frequency cursor left");
          mvprintw(LINES/2+1,COLS/2,"enter - set the waterfall's max frequency");
          mvprintw(LINES/2+2,COLS/2,"tab - set the waterfall's min frequency");
          mvprintw(LINES/2+3,COLS/2,"r - reset the waterfall");
          mvprintw(LINES/2+4,COLS/2,"q - quit");
          break;

        case ' ':
          fptog=~fptog;
          break;

          
          

          
      }
      if(c!='q')
        c=wgetch(stdscr);
      else 
        break;

      mvprintw(0,mouse_pointer,"*");
      refresh();
    }
    refresh();


  }

  endwin();
  free_ft();
  freescrbuff();
  snd_pcm_close(pcm_handle);
  snd_config_update_free_global();
  free_colors();
  return 0;
}
