#ifndef T_COLORS
#define T_COLORS

//max 1275
void init_colors(int color_size);


//get the string to print out to color square based on HUE
//------------------------------------
//do not free memory with these functions:
char* get_colored_string(int index);
//the string to reset the terminal to default colors
char* get_reset_string();
//------------------------------------
//

void free_color_info();

#endif // !T_COLORS
