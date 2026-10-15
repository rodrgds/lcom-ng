#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <lcom/lcf.h>

#define NUM_CATEGORIES 4

void draw_categories(int selected_option);

void handle_categories_input(uint8_t scancode, int *selected_option);

void handle_categories_click(int x, int y, int *selected_option);

void update_categories_selected_option(int mouse_x, int mouse_y, int *selected_option);

#endif

