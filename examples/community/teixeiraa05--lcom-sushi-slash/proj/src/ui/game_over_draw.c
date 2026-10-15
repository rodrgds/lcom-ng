#include "../../include/ui/game_over_draw.h"

#include <stdio.h>

#include "../../include/macros/menu.h"
#include "../../include/ui/ui.h"

typedef struct {
    GameOverOption option;
    button_t button;
} GameOverButtonSpec;

static button_t make_game_over_button(uint16_t x, uint16_t y, const char *label) {
    button_t button;
    button.x = x;
    button.y = y;
    button.width = GAME_OVER_BUTTON_W;
    button.height = GAME_OVER_BUTTON_H;
    button.color = MENU_BUTTON_COLOR;
    button.hover_color = MENU_BUTTON_HOVER_COLOR;
    button.label = label;
    button.hovered = false;
    return button;
}

static uint16_t button_text_x(const button_t *button, const Font *font) {
    uint16_t width = font_text_width(font, button->label);

    if (width >= button->width) return button->x;
    return button->x + (button->width - width) / 2;
}

static uint16_t button_text_y(const button_t *button, const Font *font) {
    if (font->char_width >= button->height) return button->y;
    return button->y + (button->height - font->char_width) / 2;
}

static int draw_button_with_label(const button_t *button, const Font *font) {
    if (ui_draw_button(button) != 0) return 1;
    return ui_draw_text(font,
                        button->label,
                        button_text_x(button, font),
                        button_text_y(button, font));
}

static GameOverButtonSpec game_over_button_at(uint8_t index) {
    GameOverButtonSpec spec;
    if (index == 0) {
        spec.option = GAME_OVER_OPTION_RESTART_SAME_SEED;
        spec.button = make_game_over_button(GAME_OVER_BUTTON_LEFT_X,
                                            GAME_OVER_BUTTON_Y,
                                            "restart same seed");
    }
    else {
        spec.option = GAME_OVER_OPTION_MAIN_MENU;
        spec.button = make_game_over_button(GAME_OVER_BUTTON_RIGHT_X,
                                            GAME_OVER_BUTTON_Y,
                                            "main menu");
    }
    return spec;
}

static int draw_stat_line(const Font *font, const char *text, uint16_t y) {
    uint16_t width = font_half_text_width(font, text);
    return font_draw_half_text_color(font,
                                     text,
                                     CENTER_X(width),
                                     y,
                                     GAME_OVER_STATS_COLOR);
}

static int draw_game_over_stats(const game_over_t *game_over, const Font *font) {
    char max_combo[32];
    char sushi_cut[32];
    char bombs_avoided[32];
    char time_survived[32];
    char sushi_missed[32];
    char seed_text[32];
    uint32_t minutes = game_over->stats.time_survived_seconds / 60;
    uint32_t seconds = game_over->stats.time_survived_seconds % 60;

    snprintf(max_combo, sizeof(max_combo), "max combo %u", (unsigned)game_over->stats.max_combo);
    snprintf(sushi_cut, sizeof(sushi_cut), "sushi cut %u", (unsigned)game_over->stats.sushi_cut);
    snprintf(bombs_avoided, sizeof(bombs_avoided), "bombs avoided %u", (unsigned)game_over->stats.bombs_avoided);
    snprintf(time_survived, sizeof(time_survived), "time survived %02u:%02u", (unsigned)minutes, (unsigned)seconds);
    snprintf(sushi_missed, sizeof(sushi_missed), "sushi missed %u", (unsigned)game_over->stats.sushi_missed);
    snprintf(seed_text, sizeof(seed_text), "seed %u", (unsigned)game_over->seed);

    if (draw_stat_line(font, max_combo, GAME_OVER_STATS_Y) != 0) return 1;
    if (draw_stat_line(font, sushi_cut, GAME_OVER_STATS_Y + GAME_OVER_STATS_GAP) != 0) return 1;
    if (draw_stat_line(font, bombs_avoided, GAME_OVER_STATS_Y + GAME_OVER_STATS_GAP * 2) != 0) return 1;
    if (draw_stat_line(font, time_survived, GAME_OVER_STATS_Y + GAME_OVER_STATS_GAP * 3) != 0) return 1;
    if (draw_stat_line(font, sushi_missed, GAME_OVER_STATS_Y + GAME_OVER_STATS_GAP * 4) != 0) return 1;
    return draw_stat_line(font, seed_text, GAME_OVER_STATS_Y + GAME_OVER_STATS_GAP * 5);
}

GameOverOption ui_game_over_hit_test(int x, int y) {
    for (uint8_t i = 0; i < 2; i++) {
        GameOverButtonSpec spec = game_over_button_at(i);
        if (ui_button_contains(&spec.button, x, y)) {
            return spec.option;
        }
    }
    return GAME_OVER_OPTION_NONE;
}

int ui_game_over_draw(const game_over_t *game_over, const Assets *assets) {
    if (game_over == NULL || assets == NULL) return 1;

    char score_text[24];
    snprintf(score_text, sizeof(score_text), "score %u", (unsigned)game_over->final_score);

    if (ui_draw_background(&assets->background_sprite) != 0) return 1;

    if (font_draw_scaled_text(&assets->game_font,
                              "GAME OVER",
                              CENTER_X(font_scaled_text_width(&assets->game_font, "GAME OVER", GAME_OVER_TITLE_SCALE)),
                              GAME_OVER_TITLE_Y,
                              GAME_OVER_TITLE_SCALE) != 0) {
        return 1;
    }

    if (font_draw_scaled_text(&assets->game_font,
                              score_text,
                              CENTER_X(font_scaled_text_width(&assets->game_font, score_text, GAME_OVER_SCORE_SCALE)),
                              GAME_OVER_SCORE_Y,
                              GAME_OVER_SCORE_SCALE) != 0) {
        return 1;
    }

    if (draw_game_over_stats(game_over, &assets->game_font) != 0) return 1;

    anim_draw(&assets->mascot_lose_anim, GAME_OVER_MASCOT_X, GAME_OVER_MASCOT_Y);

    for (uint8_t i = 0; i < 2; i++) {
        GameOverButtonSpec spec = game_over_button_at(i);
        spec.button.hovered = spec.option == game_over->hovered_option;

        if (draw_button_with_label(&spec.button, &assets->game_font) != 0) {
            return 1;
        }
    }

    return 0;
}
