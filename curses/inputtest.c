#include "curses.c"

int main(){
  setup_terminal();

  int c;
  while((c=wgetch())!='q'){
    if(c!=-1)
    printf("%d\n",c);

  }
  return 0;
}
