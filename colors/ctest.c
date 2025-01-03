
#include <stdio.h>
#include "colors.c"

int main(){

  init_colors(1275);

  for(int i=0;i<1275;i++){
    fputs(get_colored_string(i),stdout);
  }
  fputs(get_reset_string(),stdout);
  printf("\n");
  free_color_info();
  return 0;
}
