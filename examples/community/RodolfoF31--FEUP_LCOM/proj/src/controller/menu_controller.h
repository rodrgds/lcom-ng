#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H

#include <stdbool.h>
#include "../controller/game_controller.h"

#define MENU_LINE_SPACING 80 


typedef struct {
    char *label;
    bool selected;
} MenuItem;


typedef struct 
{
    Pixel allPosition[3]; 
    int index_prev_position;
    int index_position;
} Arrow;


typedef struct {
    MenuItem *items;
    int item_count;
    int selected_index;
    Arrow select_arrow;
} MenuState;



void init_menu(MenuState *menu);
void update_menu(MenuState *menu, int direction);
void draw_menu(MenuState *menu);
GameConfig select_menu_item(MenuState *menu);
GameConfig handle_menu_selection(const char *option);
void draw_update(MenuState *menu);


#endif 
