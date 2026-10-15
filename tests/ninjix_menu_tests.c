#include "menu.h"
#include <stdio.h>
int main(void) {
    menu_t menu = {.current_screen = MENU_SCREEN_MAIN, .cursor_x = 400, .cursor_y = 300};
    menu_handle_keyboard(&menu, true, 0x50);
    menu_handle_keyboard(&menu, false, 0xD0);
    if (menu.selected_option != 1) {
        fprintf(stderr, "One Down press moved more than one menu row\n");
        return 2;
    }
    menu_handle_mouse_movement(&menu, -180, 80);
    if (menu_handle_mouse_click(&menu) != MENU_RESULT_PLAY_SINGLE) {
        fprintf(stderr, "Left edge of visible Play Single button is not clickable\n");
        return 1;
    }
    puts("Visible button edges are clickable");
    return 0;
}
