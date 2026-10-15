#include "../include/game/pause_menu_state.h"
#include <stdio.h>

void pause_menu_init(pause_menu_t *menu) {
    if (menu == NULL) return;

    menu->hovered_option = PAUSE_OPTION_NONE;
}

PauseAction pause_menu_update(pause_menu_t *menu, PauseOption hovered_option, bool clicked) {
    if (menu == NULL) return PAUSE_ACTION_NONE;

    menu->hovered_option = hovered_option;
    if (!clicked) return PAUSE_ACTION_NONE;

    switch (hovered_option) {
        case PAUSE_OPTION_RESUME:
            return PAUSE_ACTION_RESUME;
        case PAUSE_OPTION_MAIN_MENU:
            return PAUSE_ACTION_MAIN_MENU;
        case PAUSE_OPTION_EXIT:
            return PAUSE_ACTION_EXIT;
        case PAUSE_OPTION_NONE:
        default:
            return PAUSE_ACTION_NONE;
    }
}
