#ifndef NCOLORS
#define NCOLORS

#include <ncurses.h>

void init_color_array(int size);
chtype get_color(int index);
void free_colors();

#endif // !NCOLORS
