#include "results.h"
#include "video.h"
#include <stdio.h>

void load_best_tests(Test best_tests[B_T_SIZE], int *count) {
    FILE *file = fopen("/home/lcom/labs/teclas/results/best_tests.txt", "r");

    *count = 0;

    if (file == NULL) {
        return;
    }

    while (*count < B_T_SIZE) {
        Test t;
        int fields_read = fscanf(file, "%s %d %d %d %d %d %d %d %d",
            t.name, &t.score, &t.wpm, &t.correct, &t.mistakes, &t.sec_duration, &t.day, &t.month, &t.year);

        if (fields_read != 9) break;

        best_tests[(*count)++] = t;
    }

    fclose(file);
}

void update_best_tests(Test best_tests[B_T_SIZE], int *count, Test new_test) {
    if (*count < B_T_SIZE) {
        best_tests[(*count)++] = new_test;
    } else {
        int fifth_index = 0;
        for (int i = 1; i < B_T_SIZE; i++) {
            if (best_tests[i].score < best_tests[fifth_index].score) fifth_index = i;
        }

        if (new_test.score <= best_tests[fifth_index].score) return;

        best_tests[fifth_index] = new_test;
    }

    for (int i = 1; i < *count; i++) {
        Test key = best_tests[i];

        int j = i - 1;
        while (j >= 0 && best_tests[j].score < key.score) {
            best_tests[j + 1] = best_tests[j];
            j--;
        }

        best_tests[j + 1] = key;
    }
}

void save_best_tests(Test best_tests[B_T_SIZE], int count) {
    FILE *file = fopen("/home/lcom/labs/teclas/results/best_tests.txt", "w");

    if (file == NULL) return;

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d %d %d %d %d %d %d %d\n",
            best_tests[i].name,
            best_tests[i].score,
            best_tests[i].wpm,
            best_tests[i].correct,
            best_tests[i].mistakes,
            best_tests[i].sec_duration,
            best_tests[i].day,
            best_tests[i].month,
            best_tests[i].year);
    }

    fclose(file);
}

void display_best_tests(Test best_tests[B_T_SIZE], int count, uint16_t x, uint16_t y) {
    Font *f = font_get();
    uint16_t line_h = f->char_height;
    uint16_t cw = f->char_width;

    int rank_w = 50;
    int col_w = (screen_width - 2 * x - rank_w) / 5;

    uint16_t cx_rank = x + rank_w / 2;
    uint16_t cx[5];
    for (int i = 0; i < 5; i++) cx[i] = x + rank_w + col_w / 2 + i * col_w;

    const char *headers[] = {"Score", "WPM", "Accuracy", "Time", "Date"};
    font_draw_string_colored("#", cx_rank - cw / 2, y, 0xFFFF00, 0, false);
    for (int i = 0; i < 5; i++) {
        font_draw_string_colored(headers[i], cx[i] - font_string_width(headers[i]) / 2, y, 0xFFFF00, 0, false);
    }

    char buf[16];
    for (int i = 0; i < B_T_SIZE; i++) {
        uint16_t row_y = y + line_h * 2 * (i + 1);

        snprintf(buf, sizeof(buf), "%d.", i + 1);
        font_draw_string_colored(buf, cx_rank - font_string_width(buf) / 2, row_y, 0xFFFFFF, 0, false);

        if (i < count) {
            Test *t = &best_tests[i];

            font_draw_string_colored(t->name, x + rank_w + cw, row_y, 0x888888, 0, false);

            snprintf(buf, sizeof(buf), "%d", t->score);
            font_draw_string_colored(buf, cx[0] - font_string_width(buf) / 2, row_y + line_h, VALUE_BG, 0, false);

            snprintf(buf, sizeof(buf), "%d", t->wpm);
            font_draw_string_colored(buf, cx[1] - font_string_width(buf) / 2, row_y + line_h, VALUE_BG, 0, false);

            int total = t->correct + t->mistakes;
            int acc = total > 0 ? (t->correct * 100) / total : 100;
            snprintf(buf, sizeof(buf), "%d%%", acc);
            font_draw_string_colored(buf, cx[2] - font_string_width(buf) / 2, row_y + line_h, VALUE_BG, 0, false);

            snprintf(buf, sizeof(buf), "%ds", t->sec_duration);
            font_draw_string_colored(buf, cx[3] - font_string_width(buf) / 2, row_y + line_h, VALUE_BG, 0, false);

            snprintf(buf, sizeof(buf), "%02d-%02d-%02d", t->day, t->month, t->year);
            font_draw_string_colored(buf, cx[4] - font_string_width(buf) / 2, row_y + line_h, VALUE_BG, 0, false);
        } else {
            for (int j = 0; j < 5; j++) {
                font_draw_string_colored("-", cx[j] - cw / 2, row_y + line_h, EMPTY_BG, 0, false); 
            }
        }
    }
}
