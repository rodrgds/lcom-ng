#include "game_over_state.h"
#include "../font.h"
#include "../ui.h"
#include "../video.h"
#include <stdio.h>

#define COL_LABEL 0x888888
#define COL_VALUE 0xFFFFFF
#define COL_ACCENT 0x64C864
#define COL_DIM 0x444444

static GameResults go_results;
static GameConfig go_cfg;

void game_over_state_init(GameResults results, GameConfig cfg) {
    go_results = results;
    go_cfg = cfg;
}

void game_over_state_draw() {
    const ColorTheme *t = &color_themes[go_cfg.theme_idx];
    GameResults *r = &go_results;
    uint16_t ch = char_height;
    char buf[32];

    video_clear(t->bg);

    font_draw_centered("results", 40, t->timer);

    video_draw_rect(80, 40 + ch + 8, screen_width - 160, 2, COL_DIM);

    uint16_t top_y = 40 + ch + 22;
    uint16_t col_w = (screen_width - 160) / 3;
    uint16_t col1_x = 80;
    uint16_t col2_x = col1_x + col_w;
    uint16_t col3_x = col2_x + col_w;

    snprintf(buf, sizeof(buf), "%d", r->wpm);
    font_draw_string_colored("wpm", col1_x, top_y, COL_LABEL, 0, false);
    font_draw_string_colored(buf, col1_x, top_y + ch + 4, COL_ACCENT, 0, false);

    snprintf(buf, sizeof(buf), "%d%%", r->accuracy);
    font_draw_string_colored("acc", col2_x, top_y, COL_LABEL, 0, false);
    font_draw_string_colored(buf, col2_x, top_y + ch + 4, COL_VALUE, 0, false);

    snprintf(buf, sizeof(buf), "%d%%", r->raw_consistency);
    font_draw_string_colored("raw cons", col3_x, top_y, COL_LABEL, 0, false);
    font_draw_string_colored(buf, col3_x, top_y + ch + 4, COL_VALUE, 0, false);

    uint16_t sep_y = top_y + ch * 2 + 18;
    video_draw_rect(80, sep_y, screen_width - 160, 2, COL_DIM);

    uint16_t lx = 80;
    uint16_t ly = sep_y + 18;

    snprintf(buf, sizeof(buf), "%d", r->correct_words);
    ly = ui_draw_stat("correct words:   ", buf, lx, ly, COL_LABEL, COL_ACCENT);

    snprintf(buf, sizeof(buf), "%d", r->wrong_words);
    ly = ui_draw_stat("wrong words:     ", buf, lx, ly, COL_LABEL, t->wrong_fg);

    int d = r->avg_errors_x10 / 10;
    int frac = r->avg_errors_x10 % 10;
    snprintf(buf, sizeof(buf), "%d.%d", d, frac);

    ly = ui_draw_stat("avg errors/word: ", buf, lx, ly, COL_LABEL, COL_VALUE);

    snprintf(buf, sizeof(buf), "%ds", r->time_played);
    ly = ui_draw_stat("time:            ", buf, lx, ly, COL_LABEL, COL_VALUE);

    snprintf(buf, sizeof(buf), "%d", r->correct_chars);
    ly = ui_draw_stat("correct chars:   ", buf, lx, ly, COL_LABEL, COL_ACCENT);

    snprintf(buf, sizeof(buf), "%d", r->wrong_chars);
    ui_draw_stat("wrong chars:     ", buf, lx, ly, COL_LABEL, t->wrong_fg);

    ui_draw_esc_hint();
}
