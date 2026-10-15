#include "pause_menu.h"

#include "../engine/ui/ui_menu.h"
#include "../assets/assets.h"
#include "../config.h"

static const char *const labels[] = {"RESUME", "CONTROLS", "EXIT TO MENU"};
static const PauseAction actions[] = {PAUSE_ACTION_RESUME, PAUSE_ACTION_CONTROLS, PAUSE_ACTION_EXIT_MENU};

static UiMenu *menu = NULL;

static UiMenu *get_menu(void) {
    if (menu != NULL) return menu;
    int y = (SCREEN_H - MENU_TITLE_H - 3 * (MENU_BTN_H + MENU_BTN_GAP) - MENU_BTN_GAP) / 2;
    menu = ui_menu_create(SCREEN_W / 2, y, MENU_BTN_W, MENU_BTN_H, MENU_BTN_GAP,
                          "PAUSED", labels, 3);
    return menu;
}

void pause_menu_draw(void) {
    UiMenu *m = get_menu();
    if (m == NULL) return;
    ui_menu_draw(m, assets_get_font());
}

PauseAction pause_menu_update(int16_t mouse_x, int16_t mouse_y, bool lb_click,
                              int nav_delta, bool confirm) {
    UiMenu *m = get_menu();
    if (m == NULL) return PAUSE_ACTION_NONE;
    int i = ui_menu_update(m, mouse_x, mouse_y, lb_click, nav_delta, confirm);
    if (i < 0) return PAUSE_ACTION_NONE;
    return actions[i];
}
