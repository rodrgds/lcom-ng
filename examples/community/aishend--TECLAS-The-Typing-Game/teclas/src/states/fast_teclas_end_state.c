#include "fast_teclas_end_state.h"
#include "../cursor.h"
#include "../font.h"
#include "../ui.h"
#include "../results.h"
#include "../video.h"
#include "fast_teclas_state.h"
#include <lcom/lcf.h>
#include "../../lab1/rtc.h"

#define COL_LABEL 0x888888
#define COL_VALUE 0xFFFFFF
#define COL_ACCENT 0x64C864
#define COL_DIM 0x444444

#define RESTART_BTN_W 240
#define RESTART_BTN_H 50

static GameResults fe_results;
static GameConfig fe_cfg;
static Test fe_best[B_T_SIZE];
static int fe_best_count = 0;

void fast_teclas_end_state_init(GameConfig cfg) {
    fe_cfg = cfg;
    fe_results = fast_teclas_state_compute_results();

    load_best_tests(fe_best, &fe_best_count);
    rtc_date date;
    rtc_read_date(&date);

    Test t;
    strncpy(t.name, fe_cfg.player_name, 15);
    t.name[15] = '\0';
    t.score = fe_results.wpm * fe_results.accuracy / 100;
    t.wpm = fe_results.wpm;
    t.correct = fe_results.correct_chars;
    t.mistakes = fe_results.wrong_chars;
    t.sec_duration = fe_results.time_played;
    t.day = date.day;
    t.month = date.month;
    t.year = date.year;

    update_best_tests(fe_best, &fe_best_count, t);
    save_best_tests(fe_best, fe_best_count);
}

void fast_teclas_end_state_draw() {
    const ColorTheme *t = &color_themes[fe_cfg.theme_idx];
    GameResults *r = &fe_results;
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
    ly = ui_draw_stat("wrong chars:     ", buf, lx, ly, COL_LABEL, t->wrong_fg);

    int btn_x = ((int)screen_width - RESTART_BTN_W) / 2;
    int btn_y = (int)ly + 20;
    ui_draw_button(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, "RESTART",
                   ui_button_hit(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, cursor_x, cursor_y));

    ui_draw_esc_hint();
}

bool fast_teclas_end_state_handle_click() {
    uint16_t ch = char_height;
    uint16_t top_y = (uint16_t)(40 + ch + 22);
    uint16_t sep_y = top_y + ch * 2 + 18;
    uint16_t ly = (uint16_t)(sep_y + 18 + 6 * (ch + 6));
    int btn_x = ((int)screen_width - RESTART_BTN_W) / 2;
    int btn_y = (int)ly + 20;
    return ui_button_hit(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, cursor_x, cursor_y);
}
