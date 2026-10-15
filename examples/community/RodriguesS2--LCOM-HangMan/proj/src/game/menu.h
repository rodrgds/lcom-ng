#ifndef _MENU_H_
#define _MENU_H_
#include "../peripherals/i8042.h"
#include <stdbool.h>
#include <stdint.h>

extern bool gameRunning; // Variable to control the game loop

#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 50
#define BUTTON_COLOR 15
#define TEXT_COLOR 29
#define HIGHLIGHT_COLOR 47
#define BUTTON_SPACING 20

void draw_options();
void handle_menu_input(uint8_t scancode, int *selected_option);
void handle_menu_click(int x, int y, int *selected_option);
void update_selected_option(int mouse_x, int mouse_y, int *selected_option);

#endif
