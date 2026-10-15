#ifndef MENU_H
#define MENU_H
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    MENU_LEVEL_1,
    MENU_LEVEL_2,
    MENU_LEVEL_3,
    MENU_HELP,
    MENU_EXIT,
    MENU_ITEMS_COUNT
} menu_item_t;

typedef struct {
    menu_item_t selected_item;
    bool is_active;
    uint16_t window_x, window_y;
    uint16_t window_w, window_h;
} menu_state_t;

int menu_init();
int menu_draw();
void menu_handle_key(uint8_t scancode);
menu_item_t menu_get_selected();
bool menu_is_active();
void menu_set_active(bool active);

#endif /* MENU_H */
