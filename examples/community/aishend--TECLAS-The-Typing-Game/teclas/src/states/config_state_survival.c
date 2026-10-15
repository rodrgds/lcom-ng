#include "config_state_survival.h"
#include "../cursor.h"

#include <lcom/lcf.h>

#include "../config.h"
#include "../font.h"
#include "../ui.h"
#include "../video.h"
#include "../scancode.h"

#define THEMES_ROW1 5
#define START_BTN_W 240
#define START_BTN_H 50

bool config_state_survival_handle_key(uint8_t sc, GameConfig *cfg) {
    if (sc & 0x80) return false;
    return false;
}

bool config_state_survival_handle_click(GameConfig *cfg) {
    int ch = char_height;
    int cx = screen_width / 2;
    int y = screen_height / 8;

    y += ch * 2;

    y += ch;
    const char *theme_names[NUM_THEMES];
    for (int i = 0; i < NUM_THEMES; i++) theme_names[i] = color_themes[i].name;
    int hit_t1 = ui_hit_option_row(theme_names, THEMES_ROW1, cx, y, cursor_x, cursor_y);

    if (hit_t1 >= 0) {
        cfg->theme_idx = hit_t1;
        return false;
    }
    y += ch + 8;

    int hit_t2 = ui_hit_option_row(theme_names + THEMES_ROW1, NUM_THEMES - THEMES_ROW1, cx, y, cursor_x, cursor_y);

    if (hit_t2 >= 0) {
        cfg->theme_idx = THEMES_ROW1 + hit_t2;
        return false;
    }
    y += ch + 8 + ch * 2;

    int btn_x = cx - START_BTN_W / 2;
    return ui_button_hit(btn_x, y, START_BTN_W, START_BTN_H, cursor_x, cursor_y);
}

void config_state_survival_draw(const GameConfig *cfg) {
    const ColorTheme *t = &color_themes[cfg->theme_idx];
    video_clear(t->bg);

    uint16_t ch = char_height;
    uint16_t cx = screen_width / 2;
    uint16_t y = screen_height / 8;

    font_draw_string_colored("SETTINGS", cx - font_string_width("SETTINGS") / 2, y, t->timer, 0, false);
    y += ch * 2;

    font_draw_string_colored("THEME", cx - font_string_width("THEME") / 2, y, t->untyped, 0, false);
    y += ch;

    const char *theme_names[NUM_THEMES];
    for (int i = 0; i < NUM_THEMES; i++) theme_names[i] = color_themes[i].name;

    ui_draw_option_row(theme_names, THEMES_ROW1, cfg->theme_idx, cx, y, t->cur_bar, t->bg, t->untyped);
    y += ch + 8;

    ui_draw_option_row(theme_names + THEMES_ROW1, NUM_THEMES - THEMES_ROW1, cfg->theme_idx - THEMES_ROW1, cx, y, t->cur_bar, t->bg, t->untyped);
    y += ch + 8 + ch * 2;

    int btn_x = (int)cx - START_BTN_W / 2;
    ui_draw_button(btn_x, (int)y, START_BTN_W, START_BTN_H, "START", ui_button_hit(btn_x, (int)y, START_BTN_W, START_BTN_H, cursor_x, cursor_y));

    ui_draw_esc_hint();
}
