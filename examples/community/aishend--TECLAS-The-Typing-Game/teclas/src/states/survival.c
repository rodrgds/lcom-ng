#include <lcom/lcf.h>
#include "survival.h"
#include "../cursor.h"
#include "../font.h"
#include "../video.h"
#include "../dictionary.h"
#include "../scancode.h"
#include "../ui.h"

#define TEXT_X 80
#define TEXT_X_END 944
#define TEXT_Y_BASE 300
#define ROW_GAP 16

#define COL_BAR 0xFF0000

#define BAR_SPEED_START 0.05f
#define BAR_SPEED_INC 0.00002f
#define GRACE_PERIOD_TICKS 300

#define POOL_SIZE 80
#define RESTART_BTN_W 240
#define RESTART_BTN_H 50

static Word pool[POOL_SIZE];
static int current_word;
static int first_visible;
static int row_end[3];
static uint16_t word_x[POOL_SIZE];

static bool game_done;

static int grace_ticks;
static int bar_word;
static float bar_pos;
static float bar_speed;
static bool started;

static int survival_points;

static GameConfig sv_cfg;

static void layout_words() {
    int cw = char_width;
    int cx = TEXT_X, row = 0;

    for (int r = 0; r < 3; r++) row_end[r] = POOL_SIZE;

    for (int i = first_visible; i < POOL_SIZE; i++) {
        int ww = pool[i].word_len * cw;
        if (cx + ww > TEXT_X_END && cx > TEXT_X) {
            row_end[row++] = i;
            if (row >= 3) break;
            cx = TEXT_X;
        }
        cx += ww + cw;
    }

    for (int r = 0; r < 3; r++) {
        int start = (r == 0) ? first_visible : row_end[r - 1];
        int stop  = row_end[r];
        int n = stop - start;
        if (n <= 0) continue;

        int row_w = (n - 1) * cw;
        for (int wi = start; wi < stop; wi++) row_w += pool[wi].word_len * cw;

        int x = (screen_width - row_w) / 2;
        for (int wi = start; wi < stop; wi++) {
            word_x[wi] = (uint16_t)x;
            x += pool[wi].word_len * cw + cw;
        }
    }
}

int survival_init() {
    survival_reset();
    return 0;
}

void survival_reset() {
    gen_words(pool, 0, POOL_SIZE, POOL_SIZE);
    current_word = 0;
    first_visible = 0;
    game_done = false;
    bar_word = 0;
    bar_pos = 0.0f;
    bar_speed = BAR_SPEED_START;
    grace_ticks = GRACE_PERIOD_TICKS;
    started = false;
    survival_points = 0;
    layout_words();
}

void survival_tick() {
    if (game_done) {
        return;
    }
    if (!started) return;

    if (grace_ticks > 0) {
        grace_ticks--;
        return;
    }

    bar_pos += bar_speed;
    bar_speed += BAR_SPEED_INC;

    while (bar_word < POOL_SIZE && bar_pos >= pool[bar_word].word_len) {
        bar_pos -= pool[bar_word].word_len;
        bar_word++;
    }

    if (bar_word >= POOL_SIZE) {
        game_done = true;
        return;
    }

    if (bar_word > current_word ||
       (bar_word == current_word && (int)bar_pos > pool[current_word].typed_len)) {
        game_done = true;
    }
}

void survival_handle_key(uint8_t sc) {
    if (game_done) {
        return;
    }
    if (sc & 0x80) return;

    char c = scancode_to_char(sc);
    if (c == 0) return;

    started = true;

    if (c == ' ') {
        Word *w = &pool[current_word];
        if (w->typed_len == w->word_len) {
            bool all_correct = true;
            for (int i = 0; i < w->word_len; i++) {
                if (w->states[i] != CORRECT) {
                    all_correct = false;
                    break;
                }
            }

            if (all_correct && current_word < POOL_SIZE - 1) {
                current_word++;
                survival_points++;
                if (current_word >= row_end[0]) {
                    first_visible = row_end[0];
                    layout_words();
                }
            }
        }

        return;
    }

    if (c == '\b') {
        if (pool[current_word].typed_len > 0) {
            pool[current_word].typed_len--;
            pool[current_word].states[pool[current_word].typed_len] = UNTYPED;
        }

        return;
    }

    Word *w = &pool[current_word];
    if (w->typed_len < w->word_len) {
        int pos = w->typed_len;
        w->states[pos] = (c == w->word[pos]) ? CORRECT : WRONG;
        w->typed_len++;
    }
}

bool survival_is_done() {
    return game_done;
}

void survival_draw() {
    const ColorTheme *t = &color_themes[sv_cfg.theme_idx];
    video_clear(t->bg);

    int cw = char_width;
    int ch = char_height;

    if (grace_ticks > 0) {
        int secs = grace_ticks / 60;
        char s[2] = { '0' + secs, '\0' };
        uint16_t tw = font_string_width(s);
        font_draw_string_colored(s, (screen_width - tw) / 2, TEXT_Y_BASE - ch - 24, t->timer, 0, false);
    }

    if (!game_done) {
        for (int r = 0; r < 3; r++) {
            int start = (r == 0) ? first_visible : row_end[r - 1];
            uint16_t y = TEXT_Y_BASE + r * (ch + ROW_GAP);

            for (int wi = start; wi < row_end[r] && wi < POOL_SIZE; wi++) {
                const char *word = pool[wi].word;
                uint16_t x = word_x[wi];
                bool is_cur = (wi == current_word);
                int typed = pool[wi].typed_len;

                if (wi + 1 < row_end[r]) {
                    int sx = word_x[wi] + pool[wi].word_len * cw + cw / 2 - 1;
                    video_draw_rect(sx, y + ch / 2 - 1, 3, 3, t->untyped);
                }

                for (int ci = 0; word[ci] != '\0'; ci++, x += cw) {
                    if (wi == bar_word && ci == (int)bar_pos) video_draw_rect(x, y - 4, cw, ch + 8, COL_BAR);
                    if (is_cur && ci == typed) {
                        font_draw_char_colored(word[ci], x, y, t->cur_fg, t->cur_bg, true);
                        video_draw_rect(x, y, 2, ch, t->cur_bar);
                    } else if (ci < typed) {
                        if (pool[wi].states[ci] == CORRECT) font_draw_char_colored(word[ci], x, y, t->correct, 0, false);
                        else font_draw_char_colored(word[ci], x, y, t->wrong_fg, t->wrong_bg, true);
                    } else font_draw_char_colored(word[ci], x, y, t->untyped, 0, false);
                }
            }
        }

        char pts_str[12];
        snprintf(pts_str, sizeof(pts_str), "Points: %03d", survival_points);
        font_draw_string_colored(pts_str, 20, 40, 0xFFFFFF, 0, false);

        int dist = current_word - bar_word;
        char dist_str[14];
        snprintf(dist_str, sizeof(dist_str), "Distance: %03d", dist);
        uint16_t dw = font_string_width(dist_str);
        font_draw_string_colored(dist_str, screen_width - dw - 20, 40, 0xFFFFFF, 0, false);
    }

    ui_draw_esc_hint();

    if (game_done) {
        const char *go_str = "GAME OVER";
        uint16_t go_cy = screen_height / 2 - ch;

        char pts_str[12];
        snprintf(pts_str, sizeof(pts_str), "Points: %03d", survival_points);
        uint16_t tw3 = font_string_width(pts_str);
        font_draw_string_colored(pts_str, (screen_width - tw3) / 2, go_cy, 0xFFFFFF, 0, false);

        uint16_t tw1 = font_string_width(go_str);
        font_draw_string_colored(go_str, (screen_width - tw1) / 2, go_cy - ch - 12, 0xEF5350, 0, false);

        int btn_x = ((int)screen_width - RESTART_BTN_W) / 2;
        int btn_y = (int)go_cy + ch + 20;
        bool hovered = ui_button_hit(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, cursor_x, cursor_y);
        ui_draw_button(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, "RESTART", hovered);
    }
}

bool survival_handle_click() {
    if (!game_done) return false;
    int ch = char_height;
    int go_cy = (int)screen_height / 2 - ch;
    int btn_x = ((int)screen_width - RESTART_BTN_W) / 2;
    int btn_y = go_cy + ch + 20;
    if (!ui_button_hit(btn_x, btn_y, RESTART_BTN_W, RESTART_BTN_H, cursor_x, cursor_y)) return false;
    survival_reset();
    return true;
}

void survival_destroy() {}

int survival_get_survival_points() { return survival_points; }

void survival_set_config(GameConfig cfg) { sv_cfg = cfg; }
