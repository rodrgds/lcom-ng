#include "fast_teclas_state.h"
#include "../cursor.h"
#include "../dictionary.h"
#include "../font.h"
#include "../game_results.h"
#include "../scancode.h"
#include "../ui.h"
#include "../video.h"
#include "../words.h"
#include "fast_teclas_end_state.h"
#include <lcom/lcf.h>

#define TEXT_X 80
#define TEXT_X_END 944
#define TEXT_Y_BASE 300
#define ROW_GAP 16
#define TICKS_PER_SEC 60

#define CFG_BTN_W 120
#define CFG_BTN_H 36
#define CFG_BTN_MARGIN 16
#define CFG_BTN_X (screen_width - CFG_BTN_W - CFG_BTN_MARGIN)
#define CFG_BTN_Y CFG_BTN_MARGIN

static TypingSession ft_words;
static int ticks_left;
static bool game_done;
static bool game_started;
static GameConfig ft_cfg;

static void draw_timer() {
    int secs = ticks_left / TICKS_PER_SEC;
    char buf[5];
    int len = 0;

    if (secs >= 100) buf[len++] = '0' + secs / 100;

    if (secs >= 10) buf[len++] = '0' + (secs / 10) % 10;

    buf[len++] = '0' + secs % 10;
    buf[len] = '\0';

    int ch = char_height;
    font_draw_centered(buf, TEXT_Y_BASE - ch - 24, color_themes[ft_cfg.theme_idx].timer);
}

int fast_teclas_state_init() {
    GameConfig def = {1, 0, ""};
    fast_teclas_state_reset(def);
    return 0;
}

void fast_teclas_state_reset(GameConfig cfg) {
    ft_cfg = cfg;
    ft_words.current = 0;
    ft_words.first_visible = 0;
    ft_words.acc_correct_chars = 0;
    ft_words.acc_wrong_chars = 0;
    ft_words.acc_correct_words = 0;
    ft_words.acc_wrong_words = 0;
    ticks_left = time_options[cfg.time_idx] * TICKS_PER_SEC;
    game_done = false;
    game_started = false;
    gen_words(ft_words.pool, 0, TYPING_POOL_SIZE, TYPING_POOL_SIZE);
    typing_layout(&ft_words, TEXT_X, TEXT_X_END);
}

bool fast_teclas_state_tick(GameConfig cfg) {
    if (game_done || !game_started) return game_done;

    if (--ticks_left == 0) {
        game_done = true;
        fast_teclas_end_state_init(cfg);
    }
    return game_done;
}

void fast_teclas_state_handle_key(uint8_t sc) {
    if (game_done) return;

    if (sc & 0x80) return;

    char c = scancode_to_char(sc);
    if (c == 0) return;

    if (c == ' ' && ft_words.pool[ft_words.current % TYPING_POOL_SIZE].typed_len == 0) return;

    if (c == ' ') {
        typing_advance(&ft_words, TEXT_X, TEXT_X_END);
        return;
    }

    if (c == '\b') {
        typing_erase(&ft_words);
        return;
    }

    if (!game_started) game_started = true;

    typing_type(&ft_words, c);
}

bool fast_teclas_state_handle_click() {
    return ui_button_hit(CFG_BTN_X, CFG_BTN_Y, CFG_BTN_W, CFG_BTN_H, cursor_x, cursor_y);
}

GameResults fast_teclas_state_compute_results() {
    GameResults r;
    r.correct_words = 0;
    r.wrong_words = 0;
    r.avg_errors_x10 = 0;
    r.wpm = 0;
    r.time_played = time_options[ft_cfg.time_idx];
    r.accuracy = 100;
    r.raw_consistency = 0;
    r.correct_chars = 0;
    r.wrong_chars = 0;

    int submitted = ft_words.current;
    int correct_chars = ft_words.acc_correct_chars;
    int wrong_chars = ft_words.acc_wrong_chars;
    int correct_words = ft_words.acc_correct_words;
    int wrong_words = ft_words.acc_wrong_words;

    for (int i = ft_words.first_visible; i < submitted; i++) {
        Word *w = &ft_words.pool[i % TYPING_POOL_SIZE];
        bool word_ok = (w->typed_len == w->word_len);
        for (int c = 0; c < w->typed_len; c++) {
            if (w->states[c] == CORRECT) correct_chars++;
            else {
                wrong_chars++;
                word_ok = false;
            }
        }
        if (word_ok) correct_words++;
        else wrong_words++;
    }

    /* count chars in current in-progress word (not submitted) */
    Word *cur = &ft_words.pool[ft_words.current % TYPING_POOL_SIZE];
    for (int c = 0; c < cur->typed_len; c++) {
        if (cur->states[c] == CORRECT) correct_chars++;
        else wrong_chars++;
    }

    int total_chars = correct_chars + wrong_chars;
    int time_secs = time_options[ft_cfg.time_idx];

    r.correct_words = correct_words;
    r.wrong_words = wrong_words;
    r.correct_chars = correct_chars;
    r.wrong_chars = wrong_chars;

    if (time_secs > 0) r.wpm = (correct_chars * 12) / time_secs;

    if (submitted > 0) r.avg_errors_x10 = (wrong_chars * 10) / submitted;

    if (total_chars > 0) r.accuracy = (correct_chars * 100) / total_chars;

    if (submitted > 0) r.raw_consistency = (correct_words * 100) / submitted;

    return r;
}

void fast_teclas_state_draw() {
    const ColorTheme *t = &color_themes[ft_cfg.theme_idx];
    int cw = char_width;
    int ch = char_height;

    video_clear(t->bg);

    for (int r = 0; r < TYPING_NUM_ROWS; r++) {
        int start = (r == 0) ? ft_words.first_visible : ft_words.row_end[r - 1];
        uint16_t y = TEXT_Y_BASE + r * (ch + ROW_GAP);

        for (int wi = start; wi < ft_words.row_end[r]; wi++) {
            int pi = wi % TYPING_POOL_SIZE;
            const char *word = ft_words.pool[pi].word;
            uint16_t x = ft_words.word_x[pi];
            bool is_cur = (wi == ft_words.current);
            int typed = ft_words.pool[pi].typed_len;

            if (wi + 1 < ft_words.row_end[r]) {
                int sx = ft_words.word_x[pi] + ft_words.pool[pi].word_len * cw + cw / 2 - 1;
                video_draw_rect(sx, y + ch / 2 - 1, 3, 3, t->untyped);
            }

            for (int ci = 0; word[ci] != '\0'; ci++, x += cw) {
                if (is_cur && ci == typed) {
                    font_draw_char_colored(word[ci], x, y, t->cur_fg, t->cur_bg, true);
                    video_draw_rect(x, y, 2, ch, t->cur_bar);

                    continue;
                }

                if (ci < typed) {
                    uint32_t fg = (ft_words.pool[pi].states[ci] == CORRECT) ? t->correct : t->wrong_fg;
                    bool has_bg = (ft_words.pool[pi].states[ci] != CORRECT);

                    font_draw_char_colored(word[ci], x, y, fg, t->wrong_bg, has_bg);

                    continue;
                }

                font_draw_char_colored(word[ci], x, y, t->untyped, 0, false);
            }
        }
    }

    draw_timer();
    ui_draw_button(CFG_BTN_X, CFG_BTN_Y, CFG_BTN_W, CFG_BTN_H, "CONFIG", ui_button_hit(CFG_BTN_X, CFG_BTN_Y, CFG_BTN_W, CFG_BTN_H, cursor_x, cursor_y));
    ui_draw_esc_hint();
}
