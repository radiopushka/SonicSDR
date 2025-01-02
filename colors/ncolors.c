
#include "ncolors.h"
#include<stdlib.h>

chtype* color_array;
int maxindex=0;

void free_colors(){
  free(color_array);
}

void init_color_array(int size){
  int step=1000/size;

  int shift=0;


  color_array=malloc(sizeof(chtype)*size);
  int array_count=0;
  chtype* color_array_cp=color_array;
  maxindex=size;
  for(int i =0;i<size;i++){
    int value=array_count;
    int third=1000/3;

    //the default color
    if(i==7)
      shift=1;

    if(value<third){
      init_color(i+shift,value,value,value);
    }else if(value<third*2){
      init_color(i+shift,value,value,value);
    }else{
      init_color(i+shift,value,value,value);

    }
    init_pair(i+shift,i+shift,i+shift);
    array_count=array_count+step;
    *color_array_cp=' '|COLOR_PAIR(i+shift);
    color_array_cp++;
  }
 
}

chtype get_color(int index){
  if(index>=maxindex){
    index=maxindex-1;
  }
  return color_array[index];
}

