#include "menu.h"

#include "../engine/ui/ui_menu.h"
#include "../engine/graphics/renderer.h"
#include "../engine/graphics/sprite.h"
#include "../assets/assets.h"
#include "../config.h"

static const char *const labels[] = {"PLAY", "CONTROLS", "EXIT"};
static const MenuAction actions[] = {MENU_ACTION_PLAY, MENU_ACTION_CONTROLS, MENU_ACTION_EXIT};

static UiMenu *menu = NULL;

// Returns the Y of the button block (first button when logo present, title when not).
static int btn_y(void) {
    const Sprite *logo = assets_get_logo_menu();
    if (logo != NULL) {
        int logo_h      = (int)sprite_get_height(logo);
        int btn_block_h = 3 * MENU_BTN_H + 2 * MENU_BTN_GAP;
        int top         = (SCREEN_H - logo_h - MENU_LOGO_GAP - btn_block_h) / 2;
        return top + logo_h + MENU_LOGO_GAP;
    }
    return (SCREEN_H - MENU_TITLE_H - 3 * (MENU_BTN_H + MENU_BTN_GAP) - MENU_BTN_GAP) / 2;
}

static UiMenu *get_menu(void) {
    if (menu != NULL) return menu;
    const Sprite *logo = assets_get_logo_menu();
    const char   *title = (logo != NULL) ? NULL : GAME_TITLE_TEXT;
    menu = ui_menu_create(SCREEN_W / 2, btn_y(), MENU_BTN_W, MENU_BTN_H, MENU_BTN_GAP,
                          title, labels, 3);
    return menu;
}

void menu_draw_bg(void) {
    renderer_draw_rect(0, 0, SCREEN_W, SCREEN_H, COLOR_MENU_BACKDROP);
}

void menu_draw(void) {
    UiMenu *m = get_menu();
    if (m == NULL) return;

    menu_draw_bg();

    const Sprite *logo = assets_get_logo_menu();
    int y      = btn_y();
    int menu_h;

    if (logo != NULL) {
        int logo_w = (int)sprite_get_width(logo);
        int logo_h = (int)sprite_get_height(logo);
        renderer_draw_sprite(logo, (SCREEN_W - logo_w) / 2, y - MENU_LOGO_GAP - logo_h);
        menu_h = 3 * MENU_BTN_H + 2 * MENU_BTN_GAP;
    } else {
        menu_h = MENU_TITLE_H + MENU_BTN_GAP + 3 * MENU_BTN_H + 2 * MENU_BTN_GAP;
    }

    int bx = SCREEN_W / 2 - MENU_BTN_W / 2 - MENU_BACKDROP_PAD;
    int by = y - MENU_BACKDROP_PAD;
    int bw = MENU_BTN_W + 2 * MENU_BACKDROP_PAD;
    int bh = menu_h + 2 * MENU_BACKDROP_PAD;

    renderer_draw_rect(bx - 2, by - 2, bw + 4, bh + 4, COLOR_MENU_BTN_BDR);
    renderer_draw_rect(bx, by, bw, bh, COLOR_MENU_BACKDROP);

    ui_menu_draw(m, assets_get_font());
}

MenuAction menu_update(int16_t mouse_x, int16_t mouse_y, bool lb_click,
                       int nav_delta, bool confirm) {
    UiMenu *m = get_menu();
    if (m == NULL) return MENU_ACTION_NONE;
    int i = ui_menu_update(m, mouse_x, mouse_y, lb_click, nav_delta, confirm);
    if (i < 0) return MENU_ACTION_NONE;
    return actions[i];
}
