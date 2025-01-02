#ifndef NCOLORS
#define NCOLORS

#include <ncurses.h>

void init_color_array(int size);
int get_color(int index);
void free_colors();
int get_colored();

#endif // !NCOLORS
