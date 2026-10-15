#include "menu.h"
#include "../video/graphics.h"
#include "../keyboard/i8042.h"
#include "../../game/digits.h"
#include "../../game/game.c"
#include <string.h>

static menu_state_t menu_state;


#define MENU_BG_COLOR     0x000080  
#define MENU_WINDOW_COLOR 0xC0C0C0 
#define MENU_BORDER_COLOR 0x000000  
#define MENU_TEXT_COLOR   0x2C3E50
#define MENU_SELECTED_COLOR 0xFF0000  
#define MENU_SELECTED_TEXT  0xFFFFFF

static const char* menu_items_text[MENU_ITEMS_COUNT] = {
    "LEVEL 1",
    "LEVEL 2", 
    "LEVEL 3",
    "HELP",
    "EXIT"
};
/**
 * @brief Initializes the main menu.
 *
 * Sets the default selected item, activates the menu and
 * computes the menu window dimensions and position.
 *
 * @return 0 on success, non-zero on error.
 */

int menu_init() {
    
    menu_state.selected_item = MENU_LEVEL_1;
    menu_state.is_active = true;
    
    menu_state.window_w = mode_info.XResolution / 2;
    menu_state.window_h = mode_info.YResolution / 2;
    menu_state.window_x = (mode_info.XResolution - menu_state.window_w) / 2;
    menu_state.window_y = (mode_info.YResolution - menu_state.window_h) / 2;
    
    return 0;
}
/**
 * @brief Draws the solid background for the menu.
 *
 * @return 0 on success, non-zero on error.
 */
static int draw_background() {
    return vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, MENU_BG_COLOR);
}
/**
 * @brief Draws the bordered window where menu items appear.
 *
 * @return 0 on success, non-zero on error.
 */
static int draw_menu_window() {
    if (vg_draw_rectangle(menu_state.window_x + 5, menu_state.window_y + 5, 
                         menu_state.window_w, menu_state.window_h, MENU_BORDER_COLOR))
        return 1;
    
    if (vg_draw_rectangle(menu_state.window_x, menu_state.window_y, 
                         menu_state.window_w, menu_state.window_h, MENU_WINDOW_COLOR))
        return 1;
    
    if (vg_draw_rectangle(menu_state.window_x, menu_state.window_y, 
                         menu_state.window_w, 3, MENU_BORDER_COLOR))
        return 1;
    
    return 0;
}
/**
 * @brief Draws the menu title bar with "SNAKE GAME".
 *
 * @return 0 on success, non-zero on error.
 */

static int draw_title() {
    uint16_t title_x = menu_state.window_x + 50;
    uint16_t title_y = menu_state.window_y + 30;
    
    if (vg_draw_rectangle(title_x, title_y, menu_state.window_w - 100, 40, MENU_SELECTED_COLOR))
        return 1;
    
    int text_x = title_x + (menu_state.window_w - 100) / 2 - 60;
    int text_y = title_y + 15;
    draw_text(text_x, text_y, "SNAKE GAME", 0xFFFFFF);
    
    return 0;
}
/**
 * @brief Renders each menu item, highlighting the selected one.
 *
 * @return 0 on success, non-zero on error.
 */
static int draw_menu_items() {
    uint16_t item_height = 45;
    uint16_t item_width = menu_state.window_w - 80;
    uint16_t start_x = menu_state.window_x + 40;
    uint16_t start_y = menu_state.window_y + 100;
    uint16_t spacing = 10;
    
    for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
        uint16_t item_y = start_y + i * (item_height + spacing);
        uint32_t bg_color = (i == menu_state.selected_item) ? MENU_SELECTED_COLOR : MENU_WINDOW_COLOR;
        uint32_t border_color = (i == menu_state.selected_item) ? MENU_SELECTED_TEXT : MENU_BORDER_COLOR;
        uint32_t text_color = (i == menu_state.selected_item) ? MENU_SELECTED_TEXT : MENU_BORDER_COLOR;
        
        if (vg_draw_rectangle(start_x, item_y, item_width, item_height, bg_color))
            return 1;
        
        if (vg_draw_rectangle(start_x, item_y, item_width, 2, border_color))
            return 1;
        if (vg_draw_rectangle(start_x, item_y + item_height - 2, item_width, 2, border_color))
            return 1;
        if (vg_draw_rectangle(start_x, item_y, 2, item_height, border_color))
            return 1;
        if (vg_draw_rectangle(start_x + item_width - 2, item_y, 2, item_height, border_color))
            return 1;
        
        int text_x = start_x + item_width / 2 - (strlen(menu_items_text[i]) * 12) / 2;
        int text_y = item_y + item_height / 2 - 7; 
        draw_text(text_x, text_y, menu_items_text[i], text_color);
    }
    
    return 0;
}
/**
 * @brief Draws navigation instructions at the bottom of the menu.
 *
 * @return 0 on success, non-zero on error.
 */
static int draw_instructions() {
    uint16_t instr_y = menu_state.window_y + menu_state.window_h;
    uint16_t instr_x = menu_state.window_x + 20;
    
    if (vg_draw_rectangle(instr_x, instr_y, menu_state.window_w - 20, 25, MENU_BORDER_COLOR))
        return 1;
    
    draw_text(instr_x + 10, instr_y + 15, "USE ARROWS TO NAVIGATE ENTER TO SELECT", MENU_SELECTED_TEXT);
    
    return 0;
}
/**
 * @brief Draws the complete menu if it is active.
 *
 * Calls the background, window, title, items and instruction routines.
 *
 * @return 0 on success, non-zero on error.
 */
int menu_draw() {
    if (!menu_state.is_active)
        return 0;
    
    if (draw_background()) return 1;
    if (draw_menu_window()) return 1;
    if (draw_title()) return 1;
    if (draw_menu_items()) return 1;
    if (draw_instructions()) return 1;
    
    return 0;
}
/**
 * @brief Processes a key press to navigate or select menu items.
 *
 * @param scancode PS/2 scancode of the pressed key.
 */

void menu_handle_key(uint8_t scancode) {
    if (!menu_state.is_active)
        return;
    
    switch (scancode) {
        case ARROW_UP:
            if (menu_state.selected_item > 0)
                menu_state.selected_item--;
            else
                menu_state.selected_item = MENU_ITEMS_COUNT - 1;
            break;
             
        case ARROW_DOWN:
            if (menu_state.selected_item < MENU_ITEMS_COUNT - 1)
                menu_state.selected_item++;
            else
                menu_state.selected_item = 0;
            break;
            
        case ENTER_KEY:
            switch (menu_state.selected_item) {
                case MENU_LEVEL_1:
                    menu_set_active(false);   
                    printf("LEVEL 1 SELECTED\n");
                    game_set_level(1);    
                    game_start();
                    break;
                case MENU_LEVEL_2:
                    menu_set_active(false);   
                    printf("LEVEL 2 SELECTED\n");
                    game_set_level(2);    
                    game_start();
                    break;
                case MENU_LEVEL_3:
                    menu_set_active(false);   
                    printf("LEVEL 3 SELECTED\n");
                    game_set_level(3);    
                    game_start();
                    break;
                case MENU_HELP:
                    printf("HELP SELECTED\n");
                    help_screen();
                    break;
                case MENU_EXIT:
                    printf("EXIT SELECTED\n");
                    game_exit();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
/**
 * @brief Retrieves the currently selected menu item.
 *
 * @return The selected menu_item_t.
 */

menu_item_t menu_get_selected() {
    return menu_state.selected_item;
}
/**
 * @brief Checks if the menu is currently active.
 *
 * @return true if active, false otherwise.
 */

bool menu_is_active() {
    return menu_state.is_active;
}
/**
 * @brief Sets the menu active state.
 *
 * @param active true to show menu, false to hide.
 */

void menu_set_active(bool active) {
    menu_state.is_active = active;
}
