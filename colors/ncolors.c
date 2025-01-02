
#include "ncolors.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

char* special_chars="  ``..--''::__,,^^==;;>><<++!!rrcc**//zz??ssLLTTvv))JJ77((||FFi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@@@@@@@";

int* color_array=NULL;

int maxval=0;
int maxindex=0;

int is_colored=1;

void free_colors(){
  printf("%d\n",maxval);
  free(color_array);
  color_array=NULL;
}




void init_color_array(int size){
  int step=1000/size;

  int shift=0;

  int DEBUG_NO_COLORS=0;

  //printf("has colors: %d\n",has_colors());
  //printf("supported colors: %d\n",COLORS );
color_array=malloc(sizeof(int)*size);
 if(COLORS<256 || !has_colors() || DEBUG_NO_COLORS==1){
    is_colored=-1;
  }


  int interval=size/(strlen(special_chars)-4);
  int spec_ch=0;

 
  int array_count=0;
  int* color_array_cp=color_array;
  maxindex=size;
  for(int i =0;i<size;i++){
    int value=array_count;

    //the default color
    if(i==7)
      shift=1;


    if(is_colored==-1){
      *color_array_cp=special_chars[spec_ch];
      //printf("%d\n",spec_ch);
      if(interval==0){
        spec_ch++;
      }else{
        if(i%interval == 0 && i!=0){
          spec_ch++;
        }
      }

    }else{
      init_color(i+shift,value,value,value);

      init_pair(i+shift,i+shift,i+shift);
      array_count=array_count+step;

      *color_array_cp=' '|COLOR_PAIR(i+shift);
    }
    color_array_cp++;
  }
 
}

int get_color(int index){
  if(index>=maxindex){
    index=maxindex-1;
  }
  if(index>maxval){
    maxval=index;
  }
  return color_array[index];
}


int get_colored(){
  return is_colored;
}
