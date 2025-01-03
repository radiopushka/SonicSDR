#include <stdlib.h>
#include <stdio.h>

#define STRING_SIZE 22


struct RGB{
  unsigned char RED;
  unsigned char GREEN;
  unsigned char BLUE;
}; 

void get_RGB(struct RGB* rgb_st,int hue_value){

  unsigned char* ccv=(unsigned char*) rgb_st;
  unsigned char* R=ccv;
  unsigned char* G=ccv+1;
  unsigned char* B=ccv+2;





  if(hue_value<=255){
    *R=0;
    *G=0;
    *B=hue_value;
    return;
  }

  if(hue_value<=510){
    *R=0;
    *G=hue_value-255;
    *B=255;
    return;
  }

  if(hue_value<=765){
    *R=0;
    *G=255;
    *B=765-hue_value;
    return;
  }
  if(hue_value<=1020){
    *R=hue_value-765;
    *G=255;
    *B=0;
    return;
  }
  if(hue_value<=1275){
    *R=255;
    *G=1275-hue_value;
    *B=0;
    return;
  }
  if(hue_value>1275){
    *R=255;
    *G=0;
    *B=0;

  }


}

char* get_escape_string(struct RGB* rgb_st){
  unsigned char* ccv=(unsigned char*) rgb_st;
  unsigned char* R=ccv;
  unsigned char* G=ccv+1;
  unsigned char* B=ccv+2;


  char* esc_str=malloc(sizeof(char)*24);
  sprintf(esc_str,"\x1b[48;2;%d;%d;%dm ",*R,*G,*B);
  return esc_str;
}

char* get_end_string(){

  char* esc_str=malloc(sizeof(char)*24);
  sprintf(esc_str,"\x1b[0m");
  return esc_str;
}

char** color_array=NULL;
int color_array_size=0;
char* end_string=NULL;

void init_colors(int size){
  if(size>1275){
    size=1275;
  }
  color_array_size=size;
  color_array=malloc(sizeof(char*)*size);


  int interval=1275/size;
  int color_index=0;
  for(int i=0;i<size;i++){
    struct RGB* rgb_val=malloc(sizeof(struct RGB));
  
    get_RGB(rgb_val,color_index);
    color_array[i]=get_escape_string(rgb_val);
    free(rgb_val);

    color_index=color_index+interval;
  }
  end_string=get_end_string();

}

void free_color_info(){
  for(int i=0;i<color_array_size;i++){
    free(color_array[i]);
  }
  free(color_array);
  color_array=NULL;
  free(end_string);
  end_string=NULL;
  color_array_size=0;
}

//--
//do not free memory returned by these functions
char* get_colored_string(int index){

  if(index>=color_array_size){
    index=color_array_size-1;
  }
  return color_array[index];
}
char* get_reset_string(){
  return end_string;
}
