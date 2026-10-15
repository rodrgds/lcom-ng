#include "words.h"
#include "dictionary.h"
#include "video.h"
#include <stdbool.h>

void typing_layout(TypingSession *s, int x_start, int x_end) {
    Font *f = font_get();
    int cw = f->char_width;
    int cx = x_start;
    int row = 0;

    for (int r = 0; r < TYPING_NUM_ROWS; r++) s->row_end[r] = s->first_visible + TYPING_POOL_SIZE;

    for (int i = s->first_visible; i < s->first_visible + TYPING_POOL_SIZE && row < TYPING_NUM_ROWS; i++) {
        int ww = s->pool[i % TYPING_POOL_SIZE].word_len * cw;
        if (cx + ww > x_end && cx > x_start) {
            s->row_end[row++] = i;
            cx = x_start;
        }
        cx += ww + cw;
    }

    for (int r = 0; r < TYPING_NUM_ROWS; r++) {
        int start = (r == 0) ? s->first_visible : s->row_end[r - 1];
        int stop = s->row_end[r];
        int n = stop - start;
        if (n <= 0) continue;

        int row_w = (n - 1) * cw;
        for (int wi = start; wi < stop; wi++) row_w += s->pool[wi % TYPING_POOL_SIZE].word_len * cw;

        int x = (screen_width - row_w) / 2;
        for (int wi = start; wi < stop; wi++) {
            s->word_x[wi % TYPING_POOL_SIZE] = (uint16_t) x;
            x += s->pool[wi % TYPING_POOL_SIZE].word_len * cw + cw;
        }
    }
}

void typing_type(TypingSession *s, char c) {
    Word *w = &s->pool[s->current % TYPING_POOL_SIZE];
    if (w->typed_len >= w->word_len) return;

    int pos = w->typed_len;
    w->states[pos] = (c == w->word[pos]) ? CORRECT : WRONG;
    w->typed_len++;
}

void typing_erase(TypingSession *s) {
    Word *w = &s->pool[s->current % TYPING_POOL_SIZE];
    if (w->typed_len == 0) return;

    w->typed_len--;
    w->states[w->typed_len] = UNTYPED;
}

void typing_advance(TypingSession *s, int x_start, int x_end) {
    s->current++;

    if (s->current < s->row_end[0]) return;

    int old_first = s->first_visible;
    s->first_visible = s->row_end[0];

    int freed = s->first_visible - old_first;
    for (int i = old_first; i < s->first_visible; i++) {
        Word *w = &s->pool[i % TYPING_POOL_SIZE];
        bool word_ok = (w->typed_len == w->word_len);
        for (int c = 0; c < w->typed_len; c++) {
            if (w->states[c] == CORRECT) s->acc_correct_chars++;
            else {
                s->acc_wrong_chars++;
                word_ok = false;
            }
        }
        if (word_ok) s->acc_correct_words++;
        else s->acc_wrong_words++;
    }

    gen_words(s->pool, old_first + TYPING_POOL_SIZE, freed, TYPING_POOL_SIZE);

    typing_layout(s, x_start, x_end);
}
