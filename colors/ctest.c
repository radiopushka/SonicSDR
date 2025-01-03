
#include <stdio.h>
#include "colors.c"

int main(){

  init_colors(1530);

  for(int i=0;i<1530;i++){
    fputs(get_colored_string(i),stdout);
    printf("%d",i);
  }
  printf("%s       \n",get_reset_string());

  
  free_color_info();
  return 0;
}
