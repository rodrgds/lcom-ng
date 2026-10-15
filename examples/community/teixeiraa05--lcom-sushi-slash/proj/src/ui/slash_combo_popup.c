#include "../../include/ui/slash_combo_popup.h"

#include <stdio.h>

#include "../../include/macros/global.h"
#include "../../include/ui/ui.h"

static int combo_popup_clamp(int value, int size, int screen_size) {
    int max_value = screen_size - size;

    if (max_value < 0) max_value = 0;
    if (value < 0) return 0;
    if (value > max_value) return max_value;
    return value;
}

static void combo_popup_draw_plus(int x, int y, uint8_t scale, uint32_t color) {
    int size = 8 * scale;
    int thickness = 2 * scale;
    int half = size / 2;

    ui_draw_rectangle(x, y + half - thickness / 2, size, thickness, color);
    ui_draw_rectangle(x + half - thickness / 2, y, thickness, size, color);
}

void ui_combo_popup_draw(const slash_combo_t *combo, const Assets *assets) {
    if (combo == NULL || assets == NULL ||
        combo->popup_ticks == 0 ||
        combo->popup_count < SLASH_COMBO_MIN_CUTS) {
        return;
    }

    const Font *font = &assets->game_font;
    char bonus_text[4];
    snprintf(bonus_text, sizeof(bonus_text), "%u", (unsigned)combo->popup_count);

    uint8_t scale = 2;
    uint16_t title_width = font_scaled_text_width(font, "COMBO", scale);
    int title_x = combo_popup_clamp(combo->popup_x - title_width / 2,
                                    title_width,
                                    SCREEN_WIDTH);
    int title_y = combo_popup_clamp(combo->popup_y - 72,
                                    56,
                                    SCREEN_HEIGHT);

    font_draw_scaled_text_color(font,
                                "COMBO",
                                (uint16_t)title_x,
                                (uint16_t)title_y,
                                scale,
                                SLASH_COMBO_POPUP_COLOR);

    int plus_size = 8 * scale;
    int gap = 4 * scale;
    uint16_t number_width = font_scaled_text_width(font, bonus_text, scale);
    int line_width = plus_size + gap + number_width;
    int line_x = combo_popup_clamp(combo->popup_x - line_width / 2,
                                   line_width,
                                   SCREEN_WIDTH);
    int line_y = combo_popup_clamp(title_y + 32,
                                   24,
                                   SCREEN_HEIGHT);

    combo_popup_draw_plus(line_x, line_y + 4, scale, SLASH_COMBO_POPUP_COLOR);
    font_draw_scaled_text_color(font,
                                bonus_text,
                                (uint16_t)(line_x + plus_size + gap),
                                (uint16_t)line_y,
                                scale,
                                SLASH_COMBO_POPUP_COLOR);
}
