#include "controls.h"
#include "menu.h"

#include "../engine/ui/ui_menu.h"
#include "../engine/graphics/renderer.h"
#include "../engine/graphics/font.h"
#include "../assets/assets.h"
#include "../config.h"

static const char *const keys[] = {
    "ARROWS", "SPACE HOLD", "SPACE RELEASE", "1", "2", "ESC", "Q"
};
static const char *const descs[] = {
    "MOVE", "CHARGE POWER", "FIRE", "BAZOOKA", "GRENADE", "PAUSE", "QUIT"
};
#define ROW_COUNT 7

static const char *const back_label[] = {"BACK"};
static UiMenu *back_btn = NULL;

static UiMenu *get_back(void) {
    if (back_btn != NULL) return back_btn;
    back_btn = ui_menu_create(SCREEN_W / 2, CONTROLS_BTN_Y,
                              MENU_BTN_W, MENU_BTN_H, 0, NULL, back_label, 1);
    return back_btn;
}

void controls_draw(void) {
    const Font *f = assets_get_font();

    menu_draw_bg();

    renderer_draw_rect(CONTROLS_TITLE_X, CONTROLS_TITLE_Y,
                       CONTROLS_TITLE_W, CONTROLS_TITLE_H, COLOR_MENU_BTN_BDR);
    renderer_draw_rect(CONTROLS_TITLE_X + MENU_BTN_BORDER, CONTROLS_TITLE_Y + MENU_BTN_BORDER,
                       CONTROLS_TITLE_W - 2 * MENU_BTN_BORDER, CONTROLS_TITLE_H - 2 * MENU_BTN_BORDER,
                       COLOR_MENU_TITLE_BG);
    if (f != NULL) {
        int tw = font_measure_string(f, "CONTROLS");
        int tx = CONTROLS_TITLE_X + (CONTROLS_TITLE_W - tw) / 2;
        int ty = CONTROLS_TITLE_Y + (CONTROLS_TITLE_H - FONT_GLYPH_HEIGHT) / 2;
        font_draw_string(f, "CONTROLS", tx, ty);
    }

    if (f != NULL) {
        for (int i = 0; i < ROW_COUNT; i++) {
            int y = CONTROLS_ROWS_Y + i * CONTROLS_ROW_H;
            font_draw_string(f, keys[i], CONTROLS_COL1_X, y);
            font_draw_string(f, descs[i], CONTROLS_COL2_X, y);
        }
    }

    UiMenu *m = get_back();
    if (m != NULL) ui_menu_draw(m, f);
}

ControlsAction controls_update(int16_t mx, int16_t my, bool lb_click, bool confirm) {
    UiMenu *m = get_back();
    if (m == NULL) return CONTROLS_ACTION_NONE;
    int i = ui_menu_update(m, mx, my, lb_click, 0, confirm);
    if (i >= 0) return CONTROLS_ACTION_BACK;
    return CONTROLS_ACTION_NONE;
}
