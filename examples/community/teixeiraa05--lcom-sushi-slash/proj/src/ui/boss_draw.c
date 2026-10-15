#include "../../include/ui/boss_draw.h"

#include <stdio.h>

#include "../../include/macros/global.h"
#include "../../include/ui/ui.h"

static void boss_draw_plus(int x, int y, uint8_t scale, uint32_t color) {
    int size = 8 * scale;
    int thickness = 2 * scale;
    int half = size / 2;

    ui_draw_rectangle(x, y + half - thickness / 2, size, thickness, color);
    ui_draw_rectangle(x + half - thickness / 2, y, thickness, size, color);
}

static int boss_draw_centered_text(const Font *font,
                                   const char *text,
                                   uint16_t y,
                                   uint8_t scale,
                                   uint32_t color) {
    uint16_t width = font_scaled_text_width(font, text, scale);
    return font_draw_scaled_text_color(font,
                                       text,
                                       CENTER_X(width),
                                       y,
                                       scale,
                                       color);
}

int ui_boss_draw(const boss_t *boss, bool active, const Assets *assets, uint32_t current_tick) {
    if (boss == NULL || assets == NULL || !active) return 0;

    const Font *font = &assets->game_font;
    char progress_text[24];
    char total_time_text[24];

    uint32_t total_seconds = (boss_total_ticks_left(boss, current_tick) + 59) / 60;

    snprintf(progress_text,
             sizeof(progress_text),
             "BOSS %u/%u",
             (unsigned)boss->combo.completed,
             (unsigned)boss->combo.total_letters);
    snprintf(total_time_text, sizeof(total_time_text), "TIME %02u", (unsigned)total_seconds);

    if (boss->sushi.xpm != NULL) {
        int boss_x = sushi_screen_x(&boss->sushi);
        int boss_y = sushi_screen_y(&boss->sushi);
        if (boss_x >= 0 && boss_y >= 0) {
            ui_draw_sprite(boss->sushi.xpm, (uint16_t)boss_x, (uint16_t)boss_y);
        }
    }

    if (boss->combo.current_letter.pixmap != NULL) {
        ui_draw_sprite(&boss->combo.current_letter, boss->combo.x, boss->combo.y);
    }

    if (boss_draw_centered_text(font, progress_text, 70, 2, BOSS_UI_TEXT_COLOR) != 0) return 1;
    return boss_draw_centered_text(font, total_time_text, 470, 2, BOSS_UI_TEXT_COLOR);
}

void ui_boss_reward_popup_draw(uint32_t points, uint8_t ticks_left, const Assets *assets) {
    if (points == 0 || ticks_left == 0 || assets == NULL) return;

    const Font *font = &assets->game_font;
    char points_text[8];
    uint8_t scale = 4;
    int plus_size = 8 * scale;
    int gap = 4 * scale;

    snprintf(points_text, sizeof(points_text), "%u", (unsigned)points);

    uint16_t number_width = font_scaled_text_width(font, points_text, scale);
    int line_width = plus_size + gap + number_width;
    int line_x = CENTER_X(line_width);
    int line_y = CENTER_Y(48);

    boss_draw_plus(line_x, line_y + 8, scale, BOSS_REWARD_POPUP_COLOR);
    font_draw_scaled_text_color(font,
                                points_text,
                                (uint16_t)(line_x + plus_size + gap),
                                (uint16_t)line_y,
                                scale,
                                BOSS_REWARD_POPUP_COLOR);
}
