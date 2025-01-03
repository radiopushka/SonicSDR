#include <stdio.h>
#include <unistd.h>
#include "terminal.h"

void mvprint(int x,int y,char* string){
  printf("\033[%d;%dH%s",x,y,string);
}
void mvmove(int x,int y){
  printf("\033[%d;%dH",x,y);
}
void clear(){
  printf("\033c");
}
void refresh(){
  fflush(stdout);
}
void setup_terminal(){
   set_conio_terminal_mode();
}

void nocurs(){
  printf("\e[?25l");
}
void curs(){
  printf("\e[?25h");
}


int wgetch()
{
  if(!kbhit())
    return -1;

  char buff;
  int loc=read(0,&buff,sizeof(char));
  if(loc!=-1)
    return buff;

  return -1;
}
