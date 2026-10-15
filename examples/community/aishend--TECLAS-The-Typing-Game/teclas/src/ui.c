#include "ui.h"
#include <lcom/lcf.h>

uint16_t ui_draw_stat(const char *label, const char *value, uint16_t x, uint16_t y, uint32_t label_col, uint32_t value_col) {
    font_draw_string_colored(label, x, y, label_col, 0, false);
    font_draw_string_colored(value, x + font_string_width(label) + char_width, y, value_col, 0, false);

    return y + char_height + 6;
}

void ui_draw_button(int x, int y, int w, int h, const char *label, bool hovered) {
    uint32_t border = hovered ? UI_BTN_RIM_TOP_HOVER : UI_BTN_RIM_TOP;
    uint32_t text_col = hovered ? UI_BTN_TEXT_HOVER : UI_BTN_TEXT;

    video_draw_hline(x, y, w, border);
    video_draw_hline(x, y + h - 1, w, border);
    video_draw_rect(x, y, 1, h, border);
    video_draw_rect(x + w - 1, y, 1, h, border);

    uint16_t tw = font_string_width(label);
    font_draw_string_colored(label, x + (w - tw) / 2, y + (h - char_height) / 2, text_col, 0, false);
}

bool ui_button_hit(int x, int y, int w, int h, int mx, int my) {
    return mx >= x && mx < x + w && my >= y && my < y + h;
}

void ui_draw_option_row(const char **opts, int n, int sel, uint16_t cx, uint16_t y, uint32_t col_sel_bg, uint32_t col_sel_fg, uint32_t col_unsel_fg) {
    uint16_t cw = char_width;
    uint16_t ch = char_height;
    int gap = cw * 3;

    int total = 0;
    for (int i = 0; i < n; i++) {
        total += font_string_width(opts[i]);
        if (i < n - 1) total += gap;
    }
    uint16_t x = cx - (uint16_t)(total / 2);

    for (int i = 0; i < n; i++) {
        uint16_t ow = font_string_width(opts[i]);
        if (i == sel) {
            video_draw_rect(x - cw / 2, y - 4, ow + cw, ch + 8, col_sel_bg);
            font_draw_string_colored(opts[i], x, y, col_sel_fg, 0, false);
        } else {
            font_draw_string_colored(opts[i], x, y, col_unsel_fg, 0, false);
        }
        x += ow + (uint16_t)gap;
    }
}

int ui_hit_option_row(const char **opts, int n, int cx, int row_y, int mx, int my) {
    int cw = char_width;
    int ch = char_height;
    int gap = cw * 3;

    int total = 0;
    for (int i = 0; i < n; i++) {
        total += font_string_width(opts[i]);
        if (i < n - 1) total += gap;
    }
    int x = cx - total / 2;

    for (int i = 0; i < n; i++) {
        int ow = font_string_width(opts[i]);
        if (mx >= x - cw / 2 && mx < x + ow + cw / 2 && my >= row_y - 4 && my < row_y + ch + 4) {
            return i;
        }
        x += ow + gap;
    }
    
    return -1;
}

void ui_draw_esc_hint() {
    font_draw_string_colored("ESC TO RETURN", 20, screen_height - char_height - 20, 0x555555, 0, false);
}
